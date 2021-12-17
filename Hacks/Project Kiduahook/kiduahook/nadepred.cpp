#include "nadepred.h"

enum acts
{
	ACT_NONE = 0,
	ACT_THROW,
	ACT_LOB,
	ACT_DROP,
};

int GrenadeType = 0;
acts act = ACT_NONE;
std::vector <Vector>path;
static int LastSimulateTick = 0;

bool ScreenTransform(const Vector& point, Vector& screen)
{
	float w;
	const matrix3x4_t& worldToScreen = g_pEngine->WorldToScreenMatrix();

	screen.x = worldToScreen[0][0] * point[0] + worldToScreen[0][1] * point[1] + worldToScreen[0][2] * point[2] + worldToScreen[0][3];
	screen.y = worldToScreen[1][0] * point[0] + worldToScreen[1][1] * point[1] + worldToScreen[1][2] * point[2] + worldToScreen[1][3];
	w = worldToScreen[3][0] * point[0] + worldToScreen[3][1] * point[1] + worldToScreen[3][2] * point[2] + worldToScreen[3][3];
	screen.z = 0.0f;

	bool behind = false;

	if (w < 0.001f) {
		behind = true;
		screen.x *= 100000;
		screen.y *= 100000;
	}
	else {
		behind = false;
		float invw = 1.0f / w;
		screen.x *= invw;
		screen.y *= invw;
	}

	return behind;
}

bool WorldToScreen(const Vector &origin, Vector &screen)
{
	if (!ScreenTransform(origin, screen))
	{
		int ScreenWidth, ScreenHeight;
		g_pEngine->GetScreenSize(ScreenWidth, ScreenHeight);
		float x = float(ScreenWidth) / 2;
		float y = (float)ScreenHeight / 2;
		x += 0.5f * screen.x * ScreenWidth + 0.5f;
		y -= 0.5f * screen.y * ScreenHeight + 0.5f;
		screen.x = x;
		screen.y = y;
		return true;
	}

	return false;
}

inline float CSGO_Armor(float flDamage, int ArmorValue)
{
	float flArmorRatio = 0.5f;
	float flArmorBonus = 0.5f;
	if (ArmorValue > 0) {
		float flNew = flDamage * flArmorRatio;
		float flArmor = (flDamage - flNew) * flArmorBonus;

		if (flArmor > static_cast<float>(ArmorValue))
		{
			flArmor = static_cast<float>(ArmorValue)* (1.f / flArmorBonus);
			flNew = flDamage - flArmor;
		}

		flDamage = flNew;
	}
	return flDamage;
}

//ThrowStrength = GetNetPropOffset("DT_BaseCSGrenade", "m_flThrowStrength");
//PinPulled = GetNetPropOffset("DT_BaseCSGrenade", "m_bPinPulled");

//NOTE: idk why but the line to trace is a bit short, its barely noticeable 
//		and the prediction fails if the grenade bounces through a player & windows
void CGrenadePrediction::DoNadePrediction(CBaseEntity* localPlayer, CBaseCombatWeapon* weapon)
{
	auto bestdmg = 0;
	static ConVar *sv_gravity = g_pICvar->FindVar("sv_gravity");
	static ConVar *molotov_throw_detonate_time = g_pICvar->FindVar("molotov_throw_detonate_time");
	static ConVar *weapon_molotov_maxdetonateslope = g_pICvar->FindVar("weapon_molotov_maxdetonateslope");

	auto GetGrenadeDetonateTime = [](int iwpnid)
	{
		switch (iwpnid)
		{
		case WEAPON_FLASH:
		case WEAPON_HE:
			return 1.5f;
			break;
		case WEAPON_INC:
		case WEAPON_MOLOTOV:
			return molotov_throw_detonate_time->GetFloat();
			break;
		case 68://tag grenade
			return 5.f;
			break;
		}

		return 3.f;
	};

	auto DrawLine = [](Vector start, Vector end, int r, int g, int b, int a = 255)
	{
		Vector startw2s, endw2s;

		if (g_pDebugOverlay->ScreenPosition(start, startw2s) || g_pDebugOverlay->ScreenPosition(end, endw2s))
			return;

		g_pSurface->DrawSetColor(r, g, b, a);
		g_pSurface->DrawLine(startw2s.x, startw2s.y, endw2s.x, endw2s.y);
	};

	auto PhysicsClipVelocity = [](const Vector& in, const Vector& normal, Vector& out, float overbounce)
	{
		int blocked = 0;
		float angle = normal[2];

		if (angle > 0)
			blocked |= 1; // floor

		if (!angle)
			blocked |= 2; // step

		float backoff = in.Dot(normal) * overbounce;

		for (int i = 0; i < 3; i++)
		{
			out[i] = in[i] - (normal[i] * backoff);

			if (out[i] > -0.1f && out[i] < 0.1f)
				out[i] = 0;
		}

		return blocked;
	};

	VEngineClient::player_info_t playerInfo;
	auto itemIndex = weapon->GetWeaponID();
	auto wpnData = weapon->GetWeaponData();

	if (!wpnData || !weapon->IsGrenade() || !weapon->bPinPulled())
		return;

	Vector angThrow, forward;

	g_pEngine->GetViewAngles(angThrow);

	angThrow.x = g_Math.NormalizePitch(angThrow.x);
	angThrow.x -= (90.f - abs(angThrow.x)) * 0.11111111f;

	g_Math.AngleVector(angThrow, forward);

	float throwStrengh = weapon->GetThrowStrength();
	float throwVelocity = fmin(fmax(750.0f/*wpndata throwvelo*/ * 0.9f, 15.f), 750.f);

	float throwHeight = (throwStrengh * 12.f) - 12.f;
	float v68 = throwVelocity * ((0.7f * throwStrengh) + 0.3f);

	// NOTE: m_vecEyePosition = GetAbsOrigin + m_vecViewOffset
	Vector startPos = localPlayer->GetEyePosition() + Vector(0, 0, throwHeight), endPos = startPos + (forward * 22.f);

	CEngineTraceClient::CTraceFilter filter;
	filter.pSkip = localPlayer;

	CEngineTraceClient::trace_t trace;
	CEngineTraceClient::Ray_t ray;
	ray.Init(startPos, endPos, Vector(-2.f, -2.f, -2.f), Vector(2.f, 2.f, 2.f));

	g_pEngineTrace->TraceRay(ray, MASK_SOLID | CONTENTS_CURRENT_90, &filter, &trace);

	endPos = trace.endpos - (forward * 6.f);
	auto throwPos = (localPlayer->GetVelocity() * 1.25f) + (forward * v68);

	float gravity = (sv_gravity->GetFloat() * 0.4f);

	for (int ticks = TIME_TO_TICKS(GetGrenadeDetonateTime(itemIndex)); ticks >= 0; --ticks)
	{
		auto throwDir = Vector(throwPos.x, throwPos.y, (throwPos.z + (throwPos.z - (gravity * g_pGlobalVars->interval_per_tick))) * 0.5f);

		auto temp = throwDir * g_pGlobalVars->interval_per_tick;

		throwPos.z -= gravity * g_pGlobalVars->interval_per_tick;

		auto src = endPos, end = endPos + temp;

		CEngineTraceClient::Ray_t ray1;
		ray1.Init(src, end, Vector(-2.f, -2.f, -2.f), Vector(2.f, 2.f, 2.f));
		g_pEngineTrace->TraceRay(ray1, MASK_SOLID | CONTENTS_CURRENT_90, &filter, &trace);

		if (trace.allSolid)
			throwPos = Vector(0, 0, 0);

		endPos = trace.endpos;
		DrawLine(src, endPos, 66, 170, 244);

		if (trace.fraction != 1.f)
		{
			float surfaceElasticity = 1.f;

			Vector throwPos2;

			PhysicsClipVelocity(throwPos, trace.plane.normal, throwPos2, 2.f);

			if (trace.m_pEnt && g_pEngine->GetPlayerInfo(trace.m_pEnt->GetIndex(), &playerInfo))
				surfaceElasticity = 0.3f;

			throwPos2 *= clamp(surfaceElasticity * 0.45f, 0.f, 0.9f);

			end = endPos + (throwPos2 * ((1.f - trace.fraction) * g_pGlobalVars->interval_per_tick));

			// TODO: do checks for smoke & molotov/incendiary
			// NOTE: the molotov and incendiary grenade only bounce of walls
			if (itemIndex == WEAPON_MOLOTOV
				|| itemIndex == WEAPON_INC)
			{
				if (trace.plane.normal.z >= cos(DEG2RAD(weapon_molotov_maxdetonateslope->GetFloat())))
					return;
			}

			// NOTE: the tactical awareness grenade sticks to surfaces
			if (itemIndex == 68)
				return;

			CEngineTraceClient::Ray_t ray2;
			ray2.Init(endPos, end, Vector(-2.f, -2.f, -2.f), Vector(2.f, 2.f, 2.f));
			g_pEngineTrace->TraceRay(ray2, MASK_SOLID | CONTENTS_CURRENT_90, &filter, &trace);

			DrawLine(endPos, end, 66, 170, 244);

			endPos = trace.endpos;
			throwPos = throwPos2;
		}

//		g_pDebugOverlay->AddBoxOverlay(endPos, Vector(-2, -2, -2), Vector(2, 2, 2), QAngle(0, 0, 0), 255, 255, 255, 62, g_pGlobalVars->interval_per_tick * 2.f);

		Vector absendpos = endPos;
		if (itemIndex == WEAPON_HE)
		{
			for (int i = 1; i <= 32; i++)
			{
				CBaseEntity *pEntity = g_pEntList->GetClientEntity(i);

				if (!pEntity || (pEntity->GetTeamNumber() == localPlayer->GetTeamNumber()))
					continue;

				float dist = (pEntity->GetAbsOrigin() - endPos).Length();

				if (dist < 350.0f)
				{
					CEngineTraceClient::CTraceFilterIgnoreGrenades filter;
					filter.pSkip = (CBaseEntity*)localPlayer;
					filter.m_icollisionGroup = 0;
					CEngineTraceClient::Ray_t ray;
					Vector NadeScreen;
					WorldToScreen(endPos, NadeScreen); //CAPPED

					Vector vPelvis = g_Math.GetHitspot(pEntity, 0);
					ray.Init(endPos, vPelvis);
					CEngineTraceClient::trace_t ptr;
					g_pEngineTrace->TraceRay(ray, MASK_SHOT, (CEngineTraceClient::ITraceFilter*)&filter, &ptr);

					if (ptr.m_pEnt == pEntity)
					{
						Vector PelvisScreen;

						WorldToScreen(vPelvis, PelvisScreen);

						static float a = 105.0f;
						static float b = 25.0f;
						static float c = 140.0f;

						float d = (((pEntity->GetAbsOrigin() - absendpos).Length() - b) / c);
						float flDamage = a * exp(-d * d);
						unsigned int LocalPlayerObserveTarget = localPlayer->GetObserverTarget();
						auto dmg = max(static_cast<int>(ceilf(CSGO_Armor(flDamage, pEntity->GetArmor()))), 0);

						char dmgstr[128];
						sprintf_s(dmgstr, "-%i", dmg);

						if (dmg > 0)
							g_Draw.DrawString(g_Draw.visualsfont, false, static_cast<int>(PelvisScreen.x), static_cast<int>(PelvisScreen.y), 255, 255, 255, 255, dmgstr);

						if (dmg > bestdmg)
						{
							bestdmg = dmg;
						}
					}
				}
			}
		}
	}
}

void CGrenadePrediction::Tick( int buttons )
{
	bool in_attack = ( buttons & IN_ATTACK );
	bool in_attack2 = ( buttons & IN_ATTACK2 );

	act = ( in_attack && in_attack2 ) ? ACT_LOB :
		( in_attack2 ) ? ACT_DROP :
		( in_attack ) ? ACT_THROW :
		ACT_NONE;
}

void CGrenadePrediction::PredictGrenade( CBaseEntity* pLocal )
{
	CBaseCombatWeapon* pWeapon = pLocal->GetWeapon();

	if( !pWeapon )
		return;

	if( pWeapon->IsGrenade() && act != ACT_NONE )
	{
		GrenadeType = *( short* )( ( DWORD )pWeapon + Offsets::Attribute::m_iItemDefinitionIndex );

		int tick = g_pGlobalVars->tickcount;

		if( LastSimulateTick != tick )
		{
			Simulate();
			LastSimulateTick = tick;
		}
	}
	else
	{
		GrenadeType = WEAPON_NONE;
	}
}

void CGrenadePrediction::PaintGrenadeTrajectory( CBaseEntity* pLocal )
{
	if( LastSimulateTick != 0 && LastSimulateTick == g_pGlobalVars->tickcount )
	{
		int size = path.size();

		if( GrenadeType && size > 1 )
		{
			Vector screen1, screen2;
			Vector prev = path[ 0 ];

			Vector endpos = path[ size - 1 ];
			Vector absendpos = endpos;

			float totaladded = 0.0f;

			while( totaladded < 30.0f )
			{
				if( g_pEngineTrace->GetPointContents( endpos ) == CONTENTS_EMPTY )
					break;

				totaladded += 2.0f;
				endpos.z += 2.0f;
			}

			auto bestdmg = 0;
			static Color redcol = { 255, 0, 0, 255 };
			static Color greencol = { 0, 255, 0, 255 };
			static Color yellowgreencol = { 177, 253, 2, 255 };
			static Color yellowcol = { 255, 255, 0, 255 };
			static Color orangecol = { 255, 128, 0, 255 };

			Color *BestColor = &redcol;

			CBaseCombatWeapon* pWeapon = pLocal->GetWeapon();

			if( pWeapon && *( short* )( ( DWORD )pWeapon + Offsets::Attribute::m_iItemDefinitionIndex ) == WEAPON_HE )
			{
				for( int i = 1; i <= 32; i++ )
				{
					CBaseEntity *pEntity = g_pEntList->GetClientEntity( i );

					if( !pEntity || ( pEntity->GetTeamNumber() == pLocal->GetTeamNumber() ) )
						continue;

					float dist = ( pEntity->GetAbsOrigin() - endpos ).Length();

					if( dist < 350.0f )
					{
						CEngineTraceClient::CTraceFilterIgnoreGrenades filter;
						filter.pSkip = ( CBaseEntity* )pLocal;
						filter.m_icollisionGroup = 0;
						CEngineTraceClient::Ray_t ray;
						Vector NadeScreen;
						WorldToScreen( endpos, NadeScreen ); //CAPPED

						Vector vPelvis = g_Math.GetHitspot( pEntity, 0 );
						ray.Init( endpos, vPelvis );
						CEngineTraceClient::trace_t ptr;
						g_pEngineTrace->TraceRay( ray, MASK_SHOT, ( CEngineTraceClient::ITraceFilter* )&filter, &ptr );

						if( ptr.m_pEnt == pEntity )
						{
							Vector PelvisScreen;

							WorldToScreen( vPelvis, PelvisScreen );

							static float a = 105.0f;
							static float b = 25.0f;
							static float c = 140.0f;

							float d = ( ( ( pEntity->GetAbsOrigin() - absendpos ).Length() - b ) / c );
							float flDamage = a*exp( -d * d );
							unsigned int LocalPlayerObserveTarget = pLocal->GetObserverTarget();
							auto dmg = max( static_cast<int>( ceilf( CSGO_Armor( flDamage, pEntity->GetArmor() ) ) ), 0 );

							char dmgstr[ 128 ];
							sprintf_s( dmgstr, "-%i", dmg );

							g_Draw.DrawString( g_Draw.visualsfont, false, static_cast< int >( PelvisScreen.x ), static_cast< int >( PelvisScreen.y ), 255, 255, 255, 255, dmgstr );

							if( dmg > bestdmg )
							{
								bestdmg = dmg;
							}
						}
					}
				}
			}

			for( auto& cur : path )
			{
				if( WorldToScreen( prev, screen1 ) && WorldToScreen( cur, screen2 ) )
				{
					g_Draw.LineRGBA( static_cast<int>( screen1.x ), static_cast<int>( screen1.y ), static_cast<int>( screen2.x ), static_cast<int>( screen2.y ), 66, 170, 244, 255 );
				}

				prev = cur;
			}

			for( auto it = OtherCollisions.begin( ), end = OtherCollisions.end( ); it != end; ++it )
			{
				g_pDebugOverlay->AddBoxOverlay(it->first, Vector(-2, -2, -2), Vector(2, 2, 2), QAngle(it->second.x, it->second.y, it->second.z), 255, 255, 255, 50, g_pGlobalVars->interval_per_tick);

			//	g_Draw.Draw3DCube( 2.f, it->second, it->first, 0, 255, 0, 200 );
			}

		//	g_Draw.Draw3DCube( 2.f, OtherCollisions.rbegin( )->second, OtherCollisions.rbegin( )->first, 255, 255, 255, 255 );
		}
	}
}

void CGrenadePrediction::Setup( CBaseEntity* pl, Vector& vecSrc, Vector& vecThrow, const Vector& angEyeAngles )
{
	CBaseEntity* pLocal = g_pEntList->GetClientEntity( g_pEngine->GetLocalPlayer() );

	if( !pLocal )
		return;

	Vector angThrow = angEyeAngles;
	float pitch = angThrow.x;

	if( angThrow.x <= 90.0f )
	{
		if( angThrow.x < -90.0f )
		{
			angThrow.x += 360.0f;
		}
	}
	else
	{
		angThrow.x -= 360.0f;
	}
	angThrow.x = angThrow.x - ( 90.0f - fabs( angThrow.x ) ) * 10.0f / 90.0f;

	float flVel = 750.0f * 0.9f;
	static const float power[ ] = { 1.0f, 1.0f, 0.5f, 0.0f };
	float b = power[ act ];
	// Clamped to [0,1]
	b = b * 0.7f;
	b = b + 0.3f;
	flVel *= b;

	Vector vForward, vRight, vUp;
	g_Math.AngleVector( angThrow, &vForward, &vRight, &vUp );

	vecSrc = pl->GetEyePosition();
	float off = ( power[ act ] * 12.0f ) - 12.0f;
	vecSrc.z += off;

	// Game calls UTIL_TraceHull here with hull and assigns vecSrc tr.endpos
	CEngineTraceClient::trace_t tr;
	Vector vecDest = vecSrc;

	vecDest.MultAdd( vForward, 22.0f );

	CEngineTraceClient::CGrenadePredictTraceFilter filter;
	filter.pSkip = ( CBaseEntity* )pLocal;
	filter.m_icollisionGroup = 0;
	HullTrace( vecSrc, vecDest, ( CEngineTraceClient::ITraceFilter* )&filter, tr );

	Vector vecBack = vForward; vecBack *= 6.0f;
	vecSrc = tr.endpos;
	vecSrc -= vecBack;

	// Finally calculate velocity
	vecThrow = pl->GetVelocity(); vecThrow *= 1.25f;
	vecThrow.MultAdd( vForward, flVel );
}

void CGrenadePrediction::Simulate()
{
	CBaseEntity* pLocal = g_pEntList->GetClientEntity( g_pEngine->GetLocalPlayer() );

	if( !pLocal )
		return;

	Vector vecSrc, vecThrow;
	Vector eyeangs;
	g_pEngine->GetViewAngles( eyeangs );
	Setup( pLocal, vecSrc, vecThrow, eyeangs );

	float interval = g_pGlobalVars->interval_per_tick;

	// Log positions 20 times per sec
	int logstep = static_cast< int >( 0.05f / interval );
	int logtimer = 0;

	path.clear();
	OtherCollisions.clear( );
	for( unsigned int i = 0; i < /*path.max_size( ) - 1*/1024; ++i ) {
		if( !logtimer )
			path.push_back( vecSrc );

		int s = Step( vecSrc, vecThrow, i, interval );
		if( ( s & 1 ) ) break;

		// Reset the log timer every logstep OR we bounced
		if( ( s & 2 ) || logtimer >= logstep ) logtimer = 0;
		else ++logtimer;

		if( vecThrow == Vector() )
			break;
	}
	path.push_back( vecSrc );
}

int CGrenadePrediction::Step( Vector& vecSrc, Vector& vecThrow, int tick, float interval )
{
	// Apply gravity
	Vector move;
	AddGravityMove( move, vecThrow, interval, false );

	// Push entity
	CEngineTraceClient::trace_t tr;
	PushEntity( vecSrc, move, tr );

	int result = 0;
	// Check ending conditions
	if( CheckDetonate( vecThrow, tr, tick, interval ) || tr.fraction == 0.0f ) {
		result |= 1;
	}

	// Resolve collisions
	if( tr.fraction != 1.0f ) {
		result |= 2; // Collision!
		ResolveFlyCollisionCustom( tr, vecThrow, interval );
	}

	if( ( result & 1 ) || vecThrow == Vector( 0, 0, 0 ) || tr.fraction != 1.0f )
	{
		Vector angles;
		g_Math.VectorAngles( ( tr.endpos - tr.start ).Normalized( ), angles );
		OtherCollisions.push_back( std::make_pair( tr.endpos, angles ) );
	}

	// Set new position
	vecSrc = tr.endpos;

	return result;
}

bool CGrenadePrediction::CheckDetonate( const Vector& vecThrow, const CEngineTraceClient::trace_t& tr, int tick, float interval )
{
	switch( GrenadeType )
	{
	case WEAPON_HE:
	case WEAPON_FLASH:
		// Pure timer based, detonate at 1.5s, checked every 0.2s
		return static_cast< float >( tick )*interval > 1.5f && !( tick%static_cast< int >( 0.2f / interval ) );

	case WEAPON_DECOY:
	case WEAPON_SMOKE:
		// Velocity must be <0.1, this is only checked every 0.2s
		if( vecThrow.Length() < 0.1f ) {
			int det_tick_mod = static_cast< int >( 0.2f / interval );
			return !( tick%det_tick_mod );
		}
		return false;

	case WEAPON_INC:
	case WEAPON_MOLOTOV:
		// Detonate when hitting the floor
		if( tr.fraction != 1.0f && tr.plane.normal.z > 0.7f )
			return true;

		return static_cast< float >( tick )*interval > 2.35f && !( tick%static_cast< int >( 0.05f / interval ) );

		// OR we've been flying for too long

	default:
		return false;
	}
}

void CGrenadePrediction::AddGravityMove( Vector& move, Vector& vel, float frametime, bool onground )
{
	static ConVar *sv_gravity = g_pICvar->FindVar( "sv_gravity" );

	Vector basevel( 0.0f, 0.0f, 0.0f );

	move.x = ( vel.x + basevel.x ) * frametime;
	move.y = ( vel.y + basevel.y ) * frametime;

	if( onground ) {
		move.z = ( vel.z + basevel.z ) * frametime;
	}
	else {
		// Game calls GetActualGravity( this );
		float gravity = ( sv_gravity ? sv_gravity->GetFloat() : 800.0f ) * 0.4f;

		float newZ = vel.z - ( gravity * frametime );
		move.z = ( ( vel.z + newZ ) / 2.0f + basevel.z ) * frametime;

		vel.z = newZ;
	}
}

void CGrenadePrediction::HullTrace( Vector src, Vector end, CEngineTraceClient::ITraceFilter* filter, CEngineTraceClient::trace_t& tr )
{
	CEngineTraceClient::Ray_t ray;

	static const Vector mins( -2.0f, -2.0f, -2.0f );
	static const Vector maxs( 2.0f, 2.0f, 2.0f );

	ray.Init( src, end, mins, maxs );

	g_pEngineTrace->TraceRay( ray, 0x200400B, filter, &tr );
}

void CGrenadePrediction::PushEntity( Vector& src, const Vector& move, CEngineTraceClient::trace_t& tr )
{
	CBaseEntity* pLocal = g_pEntList->GetClientEntity( g_pEngine->GetLocalPlayer() );

	if( !pLocal )
		return;

	Vector vecAbsEnd = src + move;

	// Trace through world
	CEngineTraceClient::CGrenadePredictTraceFilter filter;
	filter.pSkip = ( CBaseEntity* )pLocal;
	filter.m_icollisionGroup = 0;

	HullTrace( src, vecAbsEnd, ( CEngineTraceClient::ITraceFilter* )&filter, tr );
}

float DotProduct( Vector a, Vector b )
{
	float one[ ] = { a.x, a.y, a.z };
	float two[ ] = { b.x, b.y, b.z };

	return std::inner_product( one, one + 3, two, 0.0f );
}

void CGrenadePrediction::ResolveFlyCollisionCustom( CEngineTraceClient::trace_t& tr, Vector& vecVelocity, float interval )
{
	// Calculate elasticity
	float flSurfaceElasticity;

	//Don't bounce off of players with perfect elasticity
	if( tr.m_pEnt && tr.m_pEnt->IsPlayer() )
	{
		flSurfaceElasticity = 0.3f;
	}
	else {
		flSurfaceElasticity = 1.0f;
	}

	if( tr.contents & CONTENTS_WINDOW )
	{
		vecVelocity *= 0.65f;
		Vector dummy;
		CEngineTraceClient::trace_t exit;
		Vector newprev;
		if( path.size() > 1 )
		{
			Vector cur = path[ path.size() - 1 ];
			for( unsigned int i = path.size() - 2; i >= 0; i-- )
			{
				newprev = path[ i ];
				if( newprev != cur && ( newprev - cur ).Length() > 0.01f )
				{
					break;
				}
			}
			Vector angle;
			g_Math.CalcAngle( newprev, cur, angle );

			if( angle != Vector( 0.0f, 0.0f, 0.0f ) )
			{
				Vector vDir;
				g_Math.AngleVector( angle, vDir );
				vDir.Normalized();
				//VectorNormalizeFast( vDir );
				float flDistance = 0;
				Vector end;

				while( flDistance <= 90.0f ) {
					flDistance += 4.0f;

					end = tr.endpos + ( vDir * flDistance );

					if( ( g_pEngineTrace->GetPointContents( end ) & MASK_SOLID ) == 0 )
					{
						tr.endpos = end;
						break;
					}
				}
			}
		}
		return;
	}

	// ReadFloat((uintptr_t)&entersurf->physics.elasticity); //1.0;  // Assume all surfaces have the same elasticity
	float flGrenadeElasticity = 0.45f; // GetGrenadeElasticity()
	float flTotalElasticity = flGrenadeElasticity * flSurfaceElasticity;
	if( flTotalElasticity > 0.9f ) flTotalElasticity = 0.9f;
	if( flTotalElasticity < 0.0f ) flTotalElasticity = 0.0f;

	// Calculate bounce
	Vector vecAbsVelocity;
	PhysicsClipVelocity( vecVelocity, tr.plane.normal, vecAbsVelocity, 2.0f );
	vecAbsVelocity *= flTotalElasticity;

	float flSpeedSqr = DotProduct( vecVelocity, vecVelocity );

	if( tr.plane.normal.z > 0.7f ) // Floor
	{
		// Verify that we have an entity.
		vecVelocity = vecAbsVelocity;
		if( flSpeedSqr < ( 20 * 20 ) )
		{
			vecAbsVelocity.Zero();
			vecVelocity = vecAbsVelocity;
		}
		else
		{
			vecAbsVelocity *= ( ( 1.0f - tr.fraction ) * interval );
			PushEntity( tr.endpos, vecAbsVelocity, tr );
		}
	}
	else
	{
		if( flSpeedSqr < ( 20 * 20 ) )
		{
			vecAbsVelocity.Zero();
			vecVelocity = vecAbsVelocity;
		}
		else {
			vecVelocity = vecAbsVelocity;
		}
	}
}

unsigned char CGrenadePrediction::PhysicsClipVelocity( const Vector& in, const Vector& normal, Vector& out, float overbounce )
{
	static const float STOP_EPSILON = 0.1f;

	float    backoff;
	float    change;
	float    angle;
	int        i;
	unsigned char blocked;

	blocked = 0;

	angle = normal[ 2 ];

	if( angle > 0 ) {
		blocked |= 1;        // floor
	}
	if( !angle ) {
		blocked |= 2;        // step
	}

	backoff = in.Dot( normal ) * overbounce;

	for( i = 0; i < 3; i++ ) {
		change = normal[ i ] * backoff;
		out[ i ] = in[ i ] - change;
		if( out[ i ] > -STOP_EPSILON && out[ i ] < STOP_EPSILON ) {
			out[ i ] = 0;
		}
	}

	return blocked;
}
