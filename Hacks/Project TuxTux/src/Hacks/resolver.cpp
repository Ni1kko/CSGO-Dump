#include "resolver.h"

bool Settings::Resolver::resolveAll = false;
std::vector<std::pair<C_BasePlayer*, QAngle>> player_data;
float NormalizeYaw(float angle)
{


	while (angle > 180.f)
		angle -= 360.f;

	while (angle < -180.f)
		angle += 360.f;

	return angle;
}
float flAngleMod(float flAngle)
{
	return((360.0f / 65536.0f) * ((int32_t)(flAngle * (65536.0f / 360.0f)) & 65535));
}
 
float flApproachAngle(float flTarget, float flValue, float flSpeed)
{
	float flAdjustedSpeed = flSpeed;
	if (flAdjustedSpeed < 0.0f)
	{
		flAdjustedSpeed *= -1.0f;
	}
 
	float flAngleModTarget = flAngleMod(flTarget);
	float flAngleModValue = flAngleMod(flValue);
 
	float flDelta = (flAngleModTarget - flAngleModValue);
	if (flDelta >= -180.0f)
	{
		if (flDelta >= 180.0f)
		{
			flDelta -= 360.0f;
		}
	}
	else
	{
		if (flDelta <= -180.0f)
		{
			flDelta += 360.0f;
		}
	}
 
	float flReturn;
 
	if (flDelta <= flAdjustedSpeed)
	{
		if ((flAdjustedSpeed * -1.0f) <= flDelta)
		{
			flReturn = flAngleModTarget;
		}
		else
		{
			flReturn = (flAngleModValue - flAdjustedSpeed);
		}
	}
	else
	{
		flReturn = (flAngleModValue + flAdjustedSpeed);
	}
 
	return flReturn;
}
template<typename _Tp>
    constexpr const _Tp&
    Clamp(const _Tp& __val, const _Tp& __lo, const _Tp& __hi)
    {
      __glibcxx_assert(!(__hi < __lo));
      return (__val < __lo) ? __lo : (__hi < __val) ? __hi : __val;
    }
	int m_flChokedTime = 2;
void Resolver::Resolver( C_BasePlayer* player,int missedShot ) {
	  auto animState = player->GetAnimState( );
	 float m_flFakeGoalFeetYaw;
	  // Rebuild setup velocity to receive flMinBodyYaw and flMaxBodyYaw
	  Vector velocity = player->GetVelocity( );
	  float spd = velocity.LengthSqr( );
	  if ( spd > powf( 1.2f * 260.0f, 2.f ) ) {
		 Vector velocity_normalized = velocity.Normalize( );
		 velocity = velocity_normalized * ( 1.2f * 260.0f );
	  }
 
	  float v25 = Clamp( player->m_flDuckAmount( ) + animState->landingAnimationTimeLeftUntilDone, 0.0f, 1.0f );//m_fLandingDuckAdditiveSomething
	  float v26 = animState->duckProgress;
	  float v27 = m_flChokedTime * 6.0f;
	  float v28;
 
	  // clamp
	  if ( ( v25 - v26 ) <= v27 ) {
		 if ( -v27 <= ( v25 - v26 ) )
			v28 = v25;
		 else
			v28 = v26 - v27;
	  } else {
		 v28 = v26 + v27;
	  }
 
	  float flDuckAmount = Clamp( v28, 0.0f, 1.0f );
 
	  Vector animationVelocity = Vector(animState->velocityX,animState->velocityY,0);
	  float speed = std::fminf( animationVelocity.Length( ), 260.0f );
 
//	  auto weapon = ( C_WeaponCSBaseGun* ) player->m_hActiveWeapon( ).Get( );
 	C_BaseCombatWeapon* weapon = (C_BaseCombatWeapon*) entityList->GetClientEntityFromHandle(player->GetActiveWeapon());
	  float flMaxMovementSpeed = 260.0f;
	  if ( weapon ) {
		 flMaxMovementSpeed = std::fmaxf( weapon->GetCSWpnData()->GetMaxPlayerSpeed(), 0.001f );
	  }
 
	  float flRunningSpeed = speed / ( flMaxMovementSpeed * 0.520f );
	  float flDuckingSpeed = speed / ( flMaxMovementSpeed * 0.340f );
 
	  flRunningSpeed = Clamp( flRunningSpeed, 0.0f, 1.0f );
 
	  float flYawModifier = ( ( ( animState->onGround * -0.3f ) - 0.2f ) * flRunningSpeed ) + 1.0f;
	  if ( flDuckAmount > 0.0f ) {
		 float flDuckingSpeed = Clamp( flDuckingSpeed, 0.0f, 1.0f );
		 flYawModifier += ( flDuckAmount * flDuckingSpeed ) * ( 0.5f - flYawModifier );
	  }
 
	  float flMinBodyYaw = fabsf( -58 * flYawModifier );
	  float flMaxBodyYaw = fabsf( 58 * flYawModifier );
 
	  float flEyeYaw = player->GetEyeAngles( )->y;
	  float flEyeDiff = remainderf( flEyeYaw - m_flFakeGoalFeetYaw, 360.f );
 
	  if ( flEyeDiff <= flMaxBodyYaw ) {
		 if ( flMinBodyYaw > flEyeDiff )
			m_flFakeGoalFeetYaw = fabs( flMinBodyYaw ) + flEyeYaw;
	  } else {
		 m_flFakeGoalFeetYaw = flEyeYaw - fabs( flMaxBodyYaw );
	  }
 
	  m_flFakeGoalFeetYaw = std::remainderf( m_flFakeGoalFeetYaw, 360.f );
 
	  if ( speed > 0.1f || fabs( velocity.z ) > 100.0f ) {
		 m_flFakeGoalFeetYaw = flApproachAngle(
			flEyeYaw,
			m_flFakeGoalFeetYaw,
			( ( animState->onGround * 20.0f ) + 30.0f )
			* m_flChokedTime );
	  } else {
		 m_flFakeGoalFeetYaw = flApproachAngle(
			*player->GetLowerBodyYawTarget( ),
			m_flFakeGoalFeetYaw,
			m_flChokedTime * 100.0f );
	  }
 
	  float Left = flEyeYaw - flMinBodyYaw;
      float Right = flEyeYaw + flMaxBodyYaw;
 
	  float resolveYaw;
 
	  switch ( missedShot % 4 ) {
		 case 0: // brute left side
		 resolveYaw = m_flFakeGoalFeetYaw;
		 break;
		 case 1: // brute fake side
		 resolveYaw = m_flFakeGoalFeetYaw;
		 break;
		 case 2: // brute right side
		 resolveYaw = Left;
		 break;
		 case 3: // brute right side
		 resolveYaw = Right;
		 break;
		 default:
		 break;
	  }
 
	  animState->goalFeetYaw = resolveYaw;
   }


void Resolver::FrameStageNotify(ClientFrameStage_t stage)
{
	if (!engine->IsInGame())
		return;

	C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
	if (!localplayer)
		return;

	static std::array<int, 64> oldMissedShots = { 0 };

	if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
	//	PredictionSystem::StartPrediction_e();
		for (int i = 1; i < engine->GetMaxClients(); ++i)
		{
			C_BasePlayer* player = (C_BasePlayer*) entityList->GetClientEntity(i);

			if (!player
				|| player == localplayer
				|| player->GetDormant()
				|| !player->GetAlive()
				|| player->GetTeam() == localplayer->GetTeam())
				continue;

			CCSGOAnimState* animState = player->GetAnimState();
			if (!animState)
				continue;
	//		*player->ClientSideAnimation() = false;
			float maxDelta = AntiAim::GetMaxDelta(animState);

			int missedShot = LogShots::missedShots[player->GetIndex() - 1];
			int oldMissedShot = oldMissedShots[player->GetIndex() - 1];

//			if (missedShot <= oldMissedShot)
//				continue;
			
			player_data.push_back(std::pair<C_BasePlayer*, QAngle>(player, *player->GetEyeAngles()));
			
	auto v48 = 0.f;

	if (animState->feetShuffleSpeed >= 0.0f)
	{
		v48 = fminf(animState->feetShuffleSpeed2, 1.0f);
	}
	else
	{
		v48 = 0.0f;
	}

	float v49 = ((animState->runningAccelProgress * -0.30000001) - 0.19999999) * v48;

	float flYawModifier = v49 + 1.0;


	float flMaxYawModifier = maxDelta * flYawModifier;
	float flMinYawModifier = -maxDelta * flYawModifier;

	float newFeetYaw = 0.f;

	auto eyeYaw = animState->yaw;

	auto lbyYaw = animState->goalFeetYaw;

	float eye_feet_delta = fabs(eyeYaw - lbyYaw);

	if (eye_feet_delta <= flMaxYawModifier)
	{
		if (flMinYawModifier > eye_feet_delta)
		{
			newFeetYaw = fabs(flMinYawModifier) + eyeYaw;
		}
	}
	else
	{
		newFeetYaw = eyeYaw - fabs(flMaxYawModifier);
	}

	float v136 = fmod(newFeetYaw, 360.0);

	if (v136 > 180.0)
	{
		v136 = v136 - 360.0;
	}

	if (v136 < 180.0)
	{
		v136 = v136 + 360.0;
	}

	animState->goalFeetYaw = v136;


	}
	}
	else if (stage == ClientFrameStage_t::FRAME_RENDER_END)
	{
		for (unsigned long i = 0; i < player_data.size(); i++)
		{
			std::pair<C_BasePlayer*, QAngle> player_aa_data = player_data[i];
			*player_aa_data.first->GetEyeAngles() = player_aa_data.second;
		}

		oldMissedShots = LogShots::missedShots;
		
		player_data.clear();
	}
}

void Resolver::PostFrameStageNotify(ClientFrameStage_t stage)
{
}
