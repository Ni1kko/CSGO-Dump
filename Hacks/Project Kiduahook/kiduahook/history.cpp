#include "history.h"
#include "updateanimations.h"

#define BONE_USED_BY_HITBOX 0x100
#define BONE_USED_BY_ANYTHING 0x7FF00

CHistory g_History;

bool CHistory::IsPriorityRecord(CBaseEntity* pEnemy)
{
	CBaseEntity* pLocal = g_pEntList->GetClientEntity(g_pEngine->GetLocalPlayer());
	
	if (pLocal == nullptr)
		return false;

	CBaseCombatWeapon* pLocalWeapon = pLocal->GetWeapon();

	if (pLocalWeapon == nullptr)
		return false;

	CBaseCombatWeapon* pWeapon = pEnemy->GetWeapon();

	if (pWeapon == nullptr)
		return false;

	int iWpnID = pLocalWeapon->GetGroupID();

	//http://prntscr.com/oijik1

	float flShotTime = pWeapon->GetLastShotTime();

	//check if they are sideways and their max delta isnt 60 ??
	if ((pEnemy->GetSimulationTime() >= flShotTime && pEnemy->GetOldSimulationTime() < flShotTime) && g_Var.Ragebot[iWpnID].PriorityRecord[0])
		return true;

	if ((pWeapon->IsOther()|| pWeapon->IsKnife()) && g_Var.Ragebot[iWpnID].PriorityRecord[1])
		return true;

	if (!(pEnemy->GetFlags() & FL_ONGROUND) && g_Var.Ragebot[iWpnID].PriorityRecord[2])
		return true;

	if (pEnemy->GetVelocity().Length() > 170.f && g_Var.Ragebot[iWpnID].PriorityRecord[3])
		return true;

	return false;
}

sHistoryRecords::sHistoryRecords( CBaseEntity* pEnemy )
{
	m_vecOrigin = pEnemy->GetOrigin();
	m_vecAbsOrigin = pEnemy->GetAbsOrigin();
	m_angAbsAngles = pEnemy->GetAbsAngles();
	m_vecVelocity = pEnemy->GetVelocity();
	m_flSimulationTime = pEnemy->GetSimulationTime();
	m_fFlags = pEnemy->GetFlags();
	m_flCurTime = UserCmd::flFixedCurTime;
	m_pAnimState = *pEnemy->GetAnimState();
	m_flLowerBodyYawTarget = pEnemy->LowerBodyYawTarget();
	m_angEyeAngles = pEnemy->GetEyeAngles();
	m_flMaxAbsYaw = g_Misc.MaxAbsYawDelta(pEnemy);

	for( int i = 0; i < 128; i++ )
		m_Matrix[ i ] = pEnemy->GetBoneMatrix( i );

	memcpy( m_flPoseParameters, pEnemy->GetPoseParam(), 96 );

	for( int i = 0; i < 13; i++ )
		m_AnimLayer[ i ] = pEnemy->GetAnimationOverlay( i );

	m_vecBBMin = *( Vector* ) ( ( DWORD ) pEnemy + Offsets::Entity::m_Collision + Offsets::Entity::m_vecMins );
	m_vecBBMax = *( Vector* ) ( ( DWORD ) pEnemy + Offsets::Entity::m_Collision + Offsets::Entity::m_vecMaxs );

	m_bResolved = g_History.IsPriorityRecord( pEnemy );

	m_bIsSetup = false;
}

std::shared_ptr<sHistoryRecords> CHistory::GetCurrentRecord( CBaseEntity* pEnemy )
{
	if( vPlayerRecord[ pEnemy->GetIndex() ].empty() )
		return nullptr;

	return vPlayerRecord[ pEnemy->GetIndex() ][ 0 ];
}

std::shared_ptr<sHistoryRecords> CHistory::GetLastRecord(CBaseEntity * pEnemy)
{
	if (vPlayerRecord[pEnemy->GetIndex()].empty())
		return nullptr;

	auto Best = vPlayerRecord[pEnemy->GetIndex()].front();
	for (auto a : vPlayerRecord[pEnemy->GetIndex()])
		if (a->m_flSimulationTime < Best->m_flSimulationTime)
			Best = a;

	return Best;
}

std::deque<std::shared_ptr<sHistoryRecords>> CHistory::GetRecordsToScan(CBaseEntity* pEnemy, int iPosAdjustMode)
{
	auto& RecordsList = vPlayerRecord[ pEnemy->GetIndex() ];
	auto ScanList = std::deque<std::shared_ptr<sHistoryRecords>>();
	
	//this wont help much against fps drops
	//we wanna push the first and the last record
	//while here we check if lby updates and if it did we push it too
	//so we only have 3-4 records per player to scan otherwise it will rip our frames

	// Helpers
	auto IsRecordListed = [ &ScanList ] ( std::shared_ptr<sHistoryRecords> pRecord ) -> bool
	{
		for( auto i : ScanList )
		{
			if( i->m_flSimulationTime == pRecord->m_flSimulationTime )
				return true;

			// We have a record on list which is in same place, but current one isn't resolved. No need for >1 record if they're not resolved
			if ((pRecord->m_vecOrigin - i->m_vecOrigin).Length() < 12.5f && !pRecord->m_bResolved)
				return true;
		}

		return false;
	};

/*	pos adjust low:  newest and lby
	pos adjust med:  newest oldest and lby
	pos adjust high: newest oldest all lbys and mid record mby
	pos adjust very high: all valid records
	*/

	//since we dont have to care about fake anymore we will apply logic for pitch "backtrack on shot"
	//sorting shouldnt be too heavy right now we might should take yaw in count and prefer sideways since its easier to tap ?
	//or we simply just check damage and take highest damage record since they all should be resolved

	/*NEW*/
	//we also should target moving over stand since u cant fake that far in move in stand 58° in move 29°

	//we might keep the lambdas and just look for valid backtrack on shot would be best case imo

	//credits to nitro thanks for that useful info
	/*
	if they
	shoot in animtick
	u can
	set their
	yaw
	if they shoot in last simulated tick
	cuzpsilent fix
	u can shoot only thier pitch
	not their yaw
	if they shoot in neither of those
	force prev pitch and yaw
	and u got perfect fix
	*/

	auto PushFirstLBYUpdate = [&RecordsList, &ScanList, IsRecordListed]() {
		for (auto itt = RecordsList.begin(); itt != RecordsList.end(); itt++)
		{
			auto e = *itt;
			if (e && e->m_bResolved && !IsRecordListed(e) && g_History.GetDeltaTime(e) < 0.195f)
			{
				ScanList.push_back(e);
				break;
			}
		}
	};

	auto PushLastLBYUpdate = [&RecordsList, &ScanList, IsRecordListed]() {
		for (auto itt = RecordsList.rbegin(); itt != RecordsList.rend(); itt++)
		{
			auto e = *itt;
			if (e && e->m_bResolved && !IsRecordListed(e) && g_History.GetDeltaTime(e) < 0.195f)
			{
				ScanList.push_back(e);
				break;
			}
		}
	};

	auto PushLastRecord = [&RecordsList, &ScanList, IsRecordListed]() {
		for (auto itt = RecordsList.rbegin(); itt != RecordsList.rend(); itt++)
		{
			auto e = *itt;
			if (e && !IsRecordListed(e) && g_History.GetDeltaTime(e) < 0.195f)
			{
				ScanList.push_back(e);
				break;
			}
		}
	};

	auto PushFirstRecord = [&RecordsList, &ScanList, IsRecordListed]() {
		for (auto itt = RecordsList.begin(); itt != RecordsList.end(); itt++)
		{
			auto e = *itt;
			if (e && !IsRecordListed(e) && g_History.GetDeltaTime(e) < 0.195f)
			{
				ScanList.push_back(e);
				break;
			}
		}
	};

	auto PushAllLBYUpdates = [&RecordsList, &ScanList, IsRecordListed]() {
		for (auto pRecord : RecordsList)
		{
			if (pRecord && pRecord->m_bResolved && g_History.GetDeltaTime(pRecord) < 0.195f)
				ScanList.push_back(pRecord);
		}
	};

	//shouldnt we check if the record is already listed here too ?
	auto PushAllValids = [&RecordsList, &ScanList, IsRecordListed]() {
		for (auto pRecord : RecordsList)
		{
			if (pRecord && g_History.GetDeltaTime(pRecord) < 0.195f)
				ScanList.push_back(pRecord);
		}
	};

	switch (iPosAdjustMode)
	{
	case 1: // Low
		PushAllLBYUpdates();
//		PushLastRecord();
		break;
	case 2: // Med
		PushFirstLBYUpdate();//delay shot var
		PushLastLBYUpdate();
		PushFirstRecord();
		PushLastRecord();
		break;
	case 3: // High
		PushAllLBYUpdates();
		PushFirstRecord();
		PushLastRecord();
		break;
	case 4: // Very high
		PushAllLBYUpdates();
		PushAllValids();
		break;
	}

	return std::move( ScanList );
}

std::deque<std::shared_ptr<sHistoryRecords>>& CHistory::GetRecordsToDraw(CBaseEntity * pEnemy)
{
	return vPlayerRecord[pEnemy->GetIndex()];
}

void CHistory::UpdateSequence() 
{
	if( !g_pClientState )
		return;

	auto net_channel = g_pClientState->GetNetChannel();

	if( net_channel ) 
	{
		if( net_channel->m_nInSequenceNr > m_iLastSequence )
		{
			m_iLastSequence = net_channel->m_nInSequenceNr;
			m_dSequences.push_front( sSequence( net_channel->m_nInReliableState, net_channel->m_nOutReliableState, net_channel->m_nInSequenceNr, g_pGlobalVars->realtime ) );
		}

		if( m_dSequences.size() > 2048 )
			m_dSequences.pop_back();
	}
}

void CHistory::ClearSequence() 
{
	m_iLastSequence = 0;

	m_dSequences.clear();
}

void CHistory::FixAnimation( CBaseEntity* pLocal, CBaseEntity* pEntity )
{

/*Polak is stupid this wont fix animation good so we -

receive update on players in postdata update start 
backup new player data
calc vars from animation time ( velo, angles, origin, flags, animlayer, mins, maxs )
restore to animtime vars
resolve player angles
update animations
restore to player data receive from update
setupbones
save records
prevent player from being animated again until i want it

so basic ghetto jumpfall fix is just
if they choke and have simtime update on ground, u overwrite the flag
to be not on ground
until they are on ground for 2 updates in a ro
row

thats the real issue with jumpfall fix tho
is that u get an update on simtime update
and not the animupdate origin
if u can figure out animupdate origin
u dont need jumpfall fix

animation time = old simtime + ivpt

*    CCSPlayerAnimState::Update
*    55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24
*    found by xref of "keyvalues {\n" which will take you to CCSGOPlayerAnimState::CacheSequences
*    which is only called by CCSGOPlayerAnimState::Update
*
*  CreateCSGOPlayerAnimstate
*  55 8B EC 56 8B F1 B9 ? ? ? ? C7 46
*  last xref of CCSPlayerAnimState::Reset
*
*  CCSPlayerAnimState::Reset
*  56 6A 01 68 ? ? ? ? 8B F1
*  found by finding C_CSPlayer::Spawn which calls CCSPlayerAnimState::Update, as well as CCSPlayerAnimState::Reset
*/

	/*
    //set previous flags
	*( int* ) ( ( DWORD ) pEntity + Offsets::Player::m_fFlags ) = vPreviousRecord.m_fFlags;

	//strip the onground flag
	*( int* ) ( ( DWORD ) pEntity + Offsets::Player::m_fFlags ) &~~FL_ONGROUND;

	//has been onground for 2 consecutive ticks ?
	if( vRecords.front().m_fFlags & FL_ONGROUND && vPreviousRecord.m_fFlags & FL_ONGROUND )
		*( int* ) ( ( DWORD ) pEntity + Offsets::Player::m_fFlags ) |= FL_ONGROUND;
	else
	{

		//fix up
		if( vRecords.front().m_AnimLayer[ 4 ].m_fWeight != 1.f && vPreviousRecord.m_AnimLayer[ 4 ].m_fWeight == 1.f && vRecords.front().m_AnimLayer[ 5 ].m_fWeight != 0.f )
			*( int* ) ( ( DWORD ) pEntity + Offsets::Player::m_fFlags ) |= FL_ONGROUND;

		if( vRecords.front().m_fFlags & FL_ONGROUND && !( vPreviousRecord.m_fFlags & FL_ONGROUND ) )
			*( int* ) ( ( DWORD ) pEntity + Offsets::Player::m_fFlags ) &~~FL_ONGROUND;

	}*/
	//we have to recieve players RAW animlayers etc which we can only get in frame FRAME_NET_UPDATE_POSTDATAUPDATE_START after original call...

	/*should recalculate
      velocity and origin
      at animtime
      as well as duck amount*/

	//record->m_velocity = ( record->m_origin - m_player->get_oldorigin() ) * ( 1.f / ticks_to_time( record->m_lag ) );

	//store original data
	Vector oViewAngle = *(Vector*)((DWORD)pEntity + Offsets::Player::deathflag);
	float  oDuckAmount = pEntity->DuckAmount();
	int    oFlags = pEntity->GetFlags();
	int    oEffects = *(int*)((DWORD)pEntity + Offsets::Entity::m_fEffects);
	CAnimationLayer cLayer[13];
	std::memcpy(cLayer, pEntity->GetAnimOverlay(), (sizeof(CAnimationLayer) * pEntity->GetNumAnimOverlays()));

	//store globals
	float flRealTime = g_pGlobalVars->realtime;
	float flFrameCount = g_pGlobalVars->framecount;
	float flAbsoluteFrame = g_pGlobalVars->absoluteframetime;
	float flUnknown = g_pGlobalVars->unknown;
	float flCurTime = g_pGlobalVars->curtime;
	float flFrameTime = g_pGlobalVars->frametime;
	float flInterpAmount = g_pGlobalVars->interpolation_amount;
	int   iTickCount = g_pGlobalVars->tickcount;

	//simulate animations at anim update time from server
	float flAnimTime = pEntity->GetSimulationTime();
	int   iAnimTick = TIME_TO_TICKS(flAnimTime);

	//set globals correctly
	g_pGlobalVars->realtime = flAnimTime;
	g_pGlobalVars->curtime = flAnimTime;
	g_pGlobalVars->frametime = g_pGlobalVars->interval_per_tick;
	g_pGlobalVars->absoluteframetime = g_pGlobalVars->interval_per_tick;
	g_pGlobalVars->framecount = iAnimTick;
	g_pGlobalVars->tickcount = iAnimTick;
	g_pGlobalVars->interpolation_amount = 0.f;

	//overwrite effects
	*(int*)((DWORD)pEntity + Offsets::Entity::m_fEffects) &= ~0x1000;

	//overwrite velocity ( set abs velocity )
	pEntity->SetAbsVelocity(pEntity->GetVelocity());

	//overwrite angles
//	*(Vector*)((DWORD)pEntity + Offsets::Player::deathflag) = ;

	//overwrite duck amount
	*(float*)((DWORD)pEntity + Offsets::Player::m_flDuckAmount) = pEntity->DuckAmount();

	//overwrite flags
	*(int*)((DWORD)pEntity + Offsets::Player::m_fFlags) = pEntity->GetFlags();

	//fix falling legs
	if ((pEntity->GetFlags() & FL_ONGROUND))
		*(float*)((DWORD)pEntity->GetAnimState() + 0x110) = 0.0f;//time since in air

	if (g_Var.AntiAim.Resolve > 0)//we have to prevent setupbones from being called when we wanna resolve on update only... :c
		g_Resolver.InitFSN(pLocal, pEntity);

	//UpdateAnimsHook::ManualUpdateAnimations(pEntity);
	*(bool*)((DWORD)pEntity + Offsets::Player::m_bClientSideAnimation) = true;
	pEntity->UpdateAnimation();
	*(bool*)((DWORD)pEntity + Offsets::Player::m_bClientSideAnimation) = false;

	//restore globals
	g_pGlobalVars->realtime = flRealTime;
	g_pGlobalVars->framecount = flFrameCount;
	g_pGlobalVars->absoluteframetime = flAbsoluteFrame;
	g_pGlobalVars->curtime = flCurTime;
	g_pGlobalVars->frametime = flFrameTime;
	g_pGlobalVars->interpolation_amount = flInterpAmount;
	g_pGlobalVars->tickcount = iTickCount;

	//store abs yaw--

	//restore original data
//	*(Vector*)((DWORD)pEntity + Offsets::Player::deathflag) = oViewAngle;
	*(float*)((DWORD)pEntity + Offsets::Player::m_flDuckAmount) = oDuckAmount;
	*(int*)((DWORD)pEntity + Offsets::Entity::m_fEffects) = oEffects;
	*(int*)((DWORD)pEntity + Offsets::Player::m_fFlags) = oFlags;
	std::memcpy(pEntity->GetAnimOverlay(), cLayer, (sizeof(CAnimationLayer) * pEntity->GetNumAnimOverlays()));

	//last_setupbones_frame
	//*(int*)((DWORD)pEntity + 0xA68) = 0;

	/*	CAnimState *pAnimState = pEntity->GetAnimState();

		if( pAnimState )
		{
		//	if( pAnimState->m_iLastClientSideAnimationUpdateFramecount == g_pGlobalVars->framecount )
		//		pAnimState->m_iLastClientSideAnimationUpdateFramecount = g_pGlobalVars->framecount - 1;

			if( TIME_TO_TICKS( vRecords.at( 0 )->m_flSimulationTime - vRecords.at( 1 )->m_flSimulationTime ) >= 1 )
			{
				*( int* ) ( ( DWORD ) pEntity + Offsets::Player::m_fFlags ) = vRecords.at( 1 )->m_fFlags;

				*( int* ) ( ( DWORD ) pEntity + Offsets::Player::m_fFlags ) &~ ~FL_ONGROUND;

				if( vRecords.at( 0 )->m_fFlags & FL_ONGROUND && vRecords.at( 1 )->m_fFlags & FL_ONGROUND )
					*( int* ) ( ( DWORD ) pEntity + Offsets::Player::m_fFlags ) |= FL_ONGROUND;
				else
				{
					if( vRecords.at( 0 )->m_AnimLayer[ 4 ].m_flWeight != 1.f && vRecords.at( 1 )->m_AnimLayer[ 4 ].m_flWeight == 1.f && vRecords.at( 0 )->m_AnimLayer[ 5 ].m_flWeight != 0.f )
						*( int* ) ( ( DWORD ) pEntity + Offsets::Player::m_fFlags ) |= FL_ONGROUND;

					if( vRecords.at( 0 )->m_fFlags & FL_ONGROUND && !( vRecords.at( 1 )->m_fFlags & FL_ONGROUND ) )
						*( int* ) ( ( DWORD ) pEntity + Offsets::Player::m_fFlags ) &~ ~FL_ONGROUND;
				}
			}
		}*/
}

void CHistory::DisableInterpolation(CBaseEntity* pLocal)
{
	for (int i = 1; i <= 32; i++)
	{
		CBaseEntity* pEnemy = g_pEntList->GetClientEntity(i);

		if (!pEnemy || !pEnemy->PlayerCheckWithoutFrozen())
		{
			vPlayerRecord[i].clear();
			continue;
		}

		if (pLocal->GetTeamNumber() != pEnemy->GetTeamNumber())
			SetInterpolationFlags(pEnemy, 0);
	}
}

void CHistory::Store( CBaseEntity* pLocal, int iStage )
{
	static const auto sv_maxunlag = g_pICvar->FindVar( "sv_maxunlag" );

	for( int i = 1; i <= 32; i++ )
	{
		CBaseEntity* pEnemy = g_pEntList->GetClientEntity( i );

		if( !pEnemy->PlayerCheckWithoutFrozen() )
		{
			vPlayerRecord[ i ].clear();
			continue;
		}

		if (g_Misc.bInSameTeam(pLocal, pEnemy))
		{
			vPlayerRecord[i].clear();
			continue;
		}

		SetInterpolationFlags(pEnemy, 0);

		if( !vPlayerRecord[ i ].size() )
		{
			auto RecordToStore = std::make_shared<sHistoryRecords>(pEnemy);
			g_History.GetBoneMatrix(pEnemy, RecordToStore, nullptr);
			vPlayerRecord[ i ].insert( vPlayerRecord[ i ].begin(), RecordToStore);
			continue;
		}

		if( vPlayerRecord[ i ][ 0 ]->m_flSimulationTime != pEnemy->GetSimulationTime() )
		{
			*reinterpret_cast<int*> (reinterpret_cast<uintptr_t>(pEnemy) + 0xA30) = g_pGlobalVars->framecount;
			*reinterpret_cast<int*> (reinterpret_cast<uintptr_t>(pEnemy) + 0xA28) = 0;

			g_ResolveInfo[pEnemy->GetIndex()].flUpdateYaw = pEnemy->GetEyeAngles().y;

			FixAnimation(pLocal, pEnemy);

			auto sRecord = std::make_shared<sHistoryRecords>(pEnemy);
			g_History.GetBoneMatrix(pEnemy, sRecord, nullptr);


			g_ResolveInfo[i].iTick = TIME_TO_TICKS(pEnemy->GetSimulationTime() - vPlayerRecord[i][0]->m_flSimulationTime) - 1;
			sRecord->m_iLag = TIME_TO_TICKS(pEnemy->GetSimulationTime()- vPlayerRecord[i][0]->m_flSimulationTime) - 1;

			sRecord->m_vecPrevAbsOrigin = vPlayerRecord[ i ][ 0 ]->m_vecAbsOrigin;
			sRecord->m_flPrevSimulationTime = vPlayerRecord[ i ][ 0 ]->m_flSimulationTime;
			sRecord->m_flPrevCurTime = vPlayerRecord[ i ][ 0 ]->m_flCurTime;

			//fuck tickbase shifting we dont play like that but we should get rid of this ghetto shit since we have to animate them regardless since server does too...
			if (sRecord->m_iLag >= 0)
				vPlayerRecord[i].insert(vPlayerRecord[i].begin(), sRecord);
		}

		vPlayerRecord[i].erase(std::remove_if(vPlayerRecord[i].begin(), vPlayerRecord[i].end(), 
			[](std::shared_ptr<sHistoryRecords> Record) -> bool
			{
				return fabsf(UserCmd::flFixedCurTime - Record->m_flCurTime) > sv_maxunlag->GetFloat();
			}), vPlayerRecord[i].end());
	}
}

void CHistory::Restore( CBaseEntity* pEnemy, std::shared_ptr<sHistoryRecords> sRecord )
{
	bNeedRestore[ pEnemy->GetIndex() ] = true;

	*pEnemy->BoneCachePointer() = sRecord->m_Matrix;

	*( Vector* ) ( ( DWORD ) pEnemy + Offsets::Player::m_vecOrigin ) = sRecord->m_vecOrigin;
	*( Vector* ) ( ( DWORD ) pEnemy + Offsets::Entity::m_Collision + Offsets::Entity::m_vecMins ) = sRecord->m_vecBBMin;
	*( Vector* ) ( ( DWORD ) pEnemy + Offsets::Entity::m_Collision + Offsets::Entity::m_vecMaxs ) = sRecord->m_vecBBMax;

	pEnemy->SetAbsAngles( sRecord->m_angAbsAngles );
	pEnemy->SetAbsOrigin( sRecord->m_vecOrigin );
}

void CHistory::RestoreToCurrentRecord( CBaseEntity* pEnemy )
{
	if( !vPlayerRecord[ pEnemy->GetIndex() ].size() || !bNeedRestore[ pEnemy->GetIndex() ] )
		return;

	Restore( pEnemy, vPlayerRecord[pEnemy->GetIndex()].at(0));
	bNeedRestore[ pEnemy->GetIndex() ] = false;
}

int CHistory::GetTickCount(std::shared_ptr<sHistoryRecords> sRecord )
{
	return TIME_TO_TICKS(sRecord->m_flSimulationTime + GetLerpTime());
}

CHistory::CHistory()
{
}

class CStudioHdr
{
public:
	mutable const studiohdr_t		*m_pStudioHdr;
};

bool CHistory::BuildBones(CBaseEntity* pEntity, int iMask, matrix3x4_t * pMatrix, std::shared_ptr<sHistoryRecords> pHistoryRecord)
{
	alignas(16) Vector pos[128];
	alignas(16) Quaternion quat[128];
	alignas(16) matrix3x4_t temp_out_matrix[128];

	int              backup_effects;
	Vector           backup_origin;
	QAngle			 backup_angles;
	float            backup_poses[24];
	CAnimationLayer  backup_layers[13];

	constexpr const auto HdrStudioOffset = 0x2948; // HdrStudio = *(_DWORD **)(pRenderable + 10568);// HdrStudio
	constexpr const auto WritableMatrixOffset = 0x26A4; // SolveDependencies arg

	// Second param in these is HDR
	typedef void(__thiscall* StandardBlendingRulesFn)(void*, CStudioHdr*, Vector*, Quaternion*, float, int);//line 403 setupbones
	static auto StandardBlendingRules = getvfunc<StandardBlendingRulesFn>(pEntity, 205); // reinterpret_cast<StandardBlendingRulesFn>(g_Pattern.PatternSearchIDA("client.dll", "55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? 56 8B 75 08 57 8B F9 85 F6"));

	typedef void(__thiscall* BuildTransformationsFn)(void*, CStudioHdr*, Vector*, Quaternion*, matrix3x4_t&, int, byte*);
	static auto BuildTransformations = getvfunc<BuildTransformationsFn>(pEntity, 189);//line 541 setupbones

	auto pRenderable = pEntity->GetClientRenderable();
	if (!pRenderable)
		return false;

	auto pHdr = *(CStudioHdr**)((DWORD)pRenderable + HdrStudioOffset);
	if (!pHdr || !pHdr->m_pStudioHdr)
		return false;

	// How to find these:
/*		Wrap_UpdateTargets(*(void **)(pRenderable + 9820), pos, &q, *(_DWORD *)(pRenderable + 0x2694), &v119);
		(*(void (__stdcall **)(DWORD))(*(_DWORD *)(pRenderable - 4) + 748))(CurrentTime);// CalculateIKLocks
		Wrap_SolveDependencies(*(void **)(pRenderable + 9820), pos, &q, *(_DWORD *)(pRenderable + 0x2694), &v119);*/
	auto backup_BoneArrayForWrite = *(void**)((DWORD)pRenderable + WritableMatrixOffset);

	backup_effects = *(int*)((DWORD)pEntity + Offsets::Entity::m_fEffects);
	backup_origin = pEntity->GetAbsOrigin();
	backup_angles = pEntity->GetAbsAngles();
	memcpy(backup_poses, pEntity->GetPoseParam(), 96);
	for (int i = 0; i < 13; i++)
		backup_layers[i] = pEntity->GetAnimationOverlay(i);

	//  Comment:  supermacy uses something called m_pred_origin??
	auto m_pred_origin = pHistoryRecord->m_vecOrigin;
	auto m_abs_ang = pHistoryRecord->m_angAbsAngles;

	// compute transform from raw data.
	matrix3x4_t transform;
	g_Math.AngleMatrix(m_abs_ang, m_pred_origin, transform);

	// set non interpolated data.
	*(int*)((DWORD)pEntity + Offsets::Entity::m_fEffects) = (backup_effects | 0x8);
	pEntity->SetAbsOrigin(m_pred_origin);
	pEntity->SetAbsAngles(m_abs_ang);
	memcpy(pEntity->GetPoseParam(), pHistoryRecord->m_flPoseParameters, 96);
	for (int i = 0; i < 13; i++)
		pEntity->GetAnimationOverlay(i) = pHistoryRecord->m_AnimLayer[i];

	// force game to call AccumulateLayers - pvs fix.
	//m_running = true;
	// ^ Idk what's that related to, need to reverse: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/game/client/c_baseanimating.cpp#L1978

	ZeroMemory(temp_out_matrix, sizeof(matrix3x4_t) * 128);

	// set bone array for write.
	*(void**)((DWORD)pRenderable + WritableMatrixOffset) = temp_out_matrix;

	// compute and build bones.
	// record->m_pred_time = SimulationTime? AnimTime?
	StandardBlendingRules(pEntity, pHdr, pos, quat, pHistoryRecord->m_flSimulationTime, iMask);

	uint8_t computed[0x100];
	std::memset(computed, 0, 0x100);
	BuildTransformations(pEntity, pHdr, pos, quat, transform, iMask, computed);

	// restore old matrix.
	*(void**)((DWORD)pRenderable + WritableMatrixOffset) = backup_BoneArrayForWrite;

	std::memcpy(pMatrix, temp_out_matrix, sizeof(matrix3x4_t) * 128);

	// restore original interpolated entity data.
	*(int*)((DWORD)pEntity + Offsets::Entity::m_fEffects) = backup_effects;
	pEntity->SetEffects(backup_effects);
	pEntity->SetAbsOrigin(backup_origin);
	pEntity->SetAbsAngles(backup_angles);
	memcpy(pEntity->GetPoseParam(), backup_poses, 96);
	for (int i = 0; i < 13; i++)
		pEntity->GetAnimationOverlay(i) = backup_layers[i];

	// revert to old game behavior.
	//m_running = false;

	return true;
}

bool CHistory::GetBoneMatrix(CBaseEntity* pEntity, std::shared_ptr<sHistoryRecords> pHistoryRecord, matrix3x4_t* pMatrix)
{
	if (!pHistoryRecord->m_bIsSetup)
	{
		auto IsUsingBacktrackChams = (g_Var.Visuals.Chams.BacktrackMaterial > 0);
		auto BoneMask = IsUsingBacktrackChams ? BONE_USED_BY_ANYTHING : BONE_USED_BY_HITBOX;
		if (!BuildBones(pEntity, BoneMask, pHistoryRecord->m_Matrix, pHistoryRecord))
			return false;

		pHistoryRecord->m_bIsSetup = true;
		pHistoryRecord->m_bIsChamsCompatible = IsUsingBacktrackChams;
	}

	// Exception thrown at 0x416542AE (kiduahook.dll) in csgo.exe: 0xC0000005: Access violation writing location 0x0007FF00.
	if (reinterpret_cast<uintptr_t>(pMatrix) > 0x400000 && pHistoryRecord->m_bIsSetup)
		std::memcpy(pMatrix, pHistoryRecord->m_Matrix, sizeof(matrix3x4_t) * 128);

	return true;
}

void CHistory::SetInterpolationFlags( CBaseEntity * pEnemy, int iFlag )
{
	//flyingllama: game sets it back to true in some places
	//this should work then ty flyingllama
	VarMapping_t* varmapping = reinterpret_cast<VarMapping_t*>( uintptr_t( pEnemy ) + 0x24 );

	if( varmapping )
		varmapping->m_nInterpolatedEntries = iFlag;

/*	PVOID pVarMapping = ( PVOID ) ( ( DWORD ) pEnemy + 0x24 );

	int iInterpolatedCount = *( PINT ) ( ( DWORD ) pVarMapping + 0x14 );

	for( int lerp = 0; lerp < iInterpolatedCount; lerp++ )
	{
		DWORD pVarMapEntry = *( PDWORD ) ( ( DWORD ) pVarMapping );
		pVarMapEntry += ( 0xC * lerp );

		*( PWORD ) ( ( DWORD ) pVarMapEntry + 0x2 ) = 0;
	}*/

//	*( int* ) ( ( DWORD ) pEnemy + Offsets::Player::m_fEffects ) |= 0x008; //we might wanna set that shit too so niggers gonna nig
}

void CHistory::ClearRecords()
{
	for (int i = 1; i <= 32; i++)
	{
		vPlayerRecord[i].clear();
	}
}

float CHistory::GetDeltaTime(std::shared_ptr<sHistoryRecords> sRecord )
{
	INetChannelInfo* pNetChannel = g_pEngine->GetNetChannelInfo();

	if( !pNetChannel )
		return FLT_MAX;

	float flCorrect = 0.f;

	static const auto sv_maxunlag = g_pICvar->FindVar("sv_maxunlag");

//	printf( "outgoing: %3.0f - incoming: %3.0f\n", pNetChannel->GetLatency( FLOW_OUTGOING )* 1000.f, pNetChannel->GetLatency( FLOW_INCOMING ) * 1000.f );

	flCorrect += pNetChannel->GetLatency( FLOW_OUTGOING );
	flCorrect += pNetChannel->GetLatency( FLOW_INCOMING );
	flCorrect += g_History.GetLerpTime();
	flCorrect = clamp( flCorrect, 0.f, sv_maxunlag->GetFloat());

	return fabs( flCorrect - ( UserCmd::flFixedCurTime - sRecord->m_flSimulationTime ) );
}

float CHistory::GetLerpTime()
{
	static auto cl_updaterate = g_pICvar->FindVar("cl_updaterate");
	static auto sv_minupdaterate = g_pICvar->FindVar("sv_minupdaterate");
	static auto sv_maxupdaterate = g_pICvar->FindVar("sv_maxupdaterate");
	static auto sv_client_min_interp_ratio = g_pICvar->FindVar("sv_client_min_interp_ratio");
	static auto sv_client_max_interp_ratio = g_pICvar->FindVar("sv_client_max_interp_ratio");
	static auto cl_interp_ratio = g_pICvar->FindVar("cl_interp_ratio");
	static auto cl_interp = g_pICvar->FindVar("cl_interp");

	float flUpdateRate = cl_updaterate->GetFloat();

	float flMinUpdateRate = sv_minupdaterate->GetFloat();
	float flMaxUpdateRate = sv_maxupdaterate->GetFloat();

	if( flMinUpdateRate && flMaxUpdateRate )
		flUpdateRate = clamp( flUpdateRate, flMaxUpdateRate, flMaxUpdateRate );

	float flLerpRatio = cl_interp_ratio->GetFloat();

	if( flLerpRatio == 0 )
		flLerpRatio = 1.0f;

	float flLerpAmount = cl_interp->GetFloat();

	float flMin = sv_client_min_interp_ratio->GetFloat();
	float flMax = sv_client_max_interp_ratio->GetFloat();

	if( flMin && flMax && flMin != -1 )
		flLerpRatio = clamp( flLerpRatio, flMin, flMax );
	else
		if( flLerpRatio == 0 )
			flLerpRatio = 1.0f;

	float flLerpTime = max( flLerpAmount, flLerpRatio / flUpdateRate );

	return flLerpTime;
}

int CHistory::GetLerpTicks()
{
	static const auto cl_interp_ratio = g_pICvar->FindVar( "cl_interp_ratio" );
	static const auto cl_updaterate = g_pICvar->FindVar( "cl_updaterate" );
	static const auto cl_interp = g_pICvar->FindVar( "cl_interp" );

	return TIME_TO_TICKS( max( cl_interp->GetFloat(), cl_interp_ratio->GetFloat() / cl_updaterate->GetFloat() ) );
}

std::shared_ptr<sHistoryRecords> CHistory::GetBestRecordAndPoint( CBaseEntity* pLocal, CBaseEntity* pEnemy, CBaseCombatWeapon* pWeapon, CUserCmd* pCmd, int iWpnID, Vector& vFinal )
{
	std::shared_ptr<sHistoryRecords> sFinalRecord = nullptr;
	static float flMinDamage;

	auto ScanRecordsList = g_History.GetRecordsToScan(pEnemy, g_Var.Ragebot[iWpnID].PosAdjust);
	if (ScanRecordsList.empty())
		return nullptr;

	if( g_Var.Ragebot[ iWpnID ].AutoMindDamage )
	{
		if( pEnemy->GetHealth() > 0 && pEnemy->GetHealth() <= 20 )
		{
			switch( pWeapon->GetWeaponID() )
			{
			case WEAPON_AK47:
				flMinDamage = 5; break;
			case WEAPON_SCOUT:
				flMinDamage = 35; break;
			case WEAPON_AWP:
				flMinDamage = pEnemy->GetHealth() + 10; break;
			case WEAPON_SCAR20:
			case WEAPON_G3SG1:
				flMinDamage = 10; break;
			case WEAPON_TASER:
				flMinDamage = pEnemy->GetHealth() + 10; break;
			default:
				flMinDamage = 10; break;
			}
		}
		else
			if( pEnemy->GetHealth() > 20 && pEnemy->GetHealth() <= 50 )
			{
				switch( pWeapon->GetWeaponID() )
				{
				case WEAPON_AK47:
					flMinDamage = 15; break;
				case WEAPON_SCOUT:
					flMinDamage = 55; break;
				case WEAPON_AWP:
					flMinDamage = pEnemy->GetHealth() + 10; break;
				case WEAPON_SCAR20:
				case WEAPON_G3SG1:
					flMinDamage = 25; break;
				case WEAPON_R8REVOLVER:
					flMinDamage = 30; break;
				case WEAPON_TASER:
					flMinDamage = pEnemy->GetHealth() + 10; break;
				default:
					flMinDamage = 20; break;
				}
			}
			else
				if( pEnemy->GetHealth() > 50 && pEnemy->GetHealth() <= 125 )
				{
					switch( pWeapon->GetWeaponID() )
					{
					case WEAPON_AK47:
						flMinDamage = 25; break;
					case WEAPON_SCOUT:
						flMinDamage = 80; break;
					case WEAPON_AWP:
						flMinDamage = pEnemy->GetHealth() + 10; break;
					case WEAPON_SCAR20:
					case WEAPON_G3SG1:
						flMinDamage = 40; break;
					case WEAPON_R8REVOLVER:
						flMinDamage = 60; break;
					case WEAPON_TASER:
						flMinDamage = pEnemy->GetHealth() + 10; break;
					default:
						flMinDamage = 20; break;
					}
				}
	}
	else
	{
		if( pWeapon->GetWeaponID() == WEAPON_TASER )
			flMinDamage = pEnemy->GetHealth() + 10;
		else
		{
			if (g_Var.Ragebot[iWpnID].Mindamage > pEnemy->GetHealth())
				flMinDamage = pEnemy->GetHealth() + 10;
			else
				flMinDamage = g_Var.Ragebot[iWpnID].Mindamage;
		}
	}

	auto CurrentRestore = sPreBacktrackRecord(pEnemy);

	for (auto& sRecord : ScanRecordsList)
	{
		g_History.Restore(pEnemy, sRecord);

		Vector vHitBox = Vector(0.f, 0.f, 0.f);
		Vector vFilteredBox = Vector(0.f, 0.f, 0.f);
		Vector vBodyBox = Vector(0.f, 0.f, 0.f);

		bool bShouldBody = false;

		float flBaimDmg = 0.f;
		float flCurrentDamage = 0.f;
		float flDamage = 0.f;

		matrix3x4_t matrix[128];

		if (!GetBoneMatrix(pEnemy, sRecord, matrix))
			continue;

		if (g_Var.Ragebot[iWpnID].Multibox)
		{
		/*	if (g_Var.Ragebot[iWpnID].BodyAimHp > 0.9 && g_Var.Ragebot[iWpnID].BodyAimHp >= pEnemy->GetHealth())
			{
				for (int i = HITBOX_PELVIS; i <= HITBOX_THORAX; i++)
				{
					vBodyBox = g_Rage.CalculatePoint(pLocal, pEnemy, pWeapon, i, flMinDamage, matrix, flDamage, g_Var.Ragebot[iWpnID].Multipoint);

					if (flDamage > flBaimDmg)
					{
						flBaimDmg = flDamage;
						vFilteredBox = vBodyBox;
					}
				}
			}
			else
				vFilteredBox = g_Rage.CalculatePoint(pLocal, pEnemy, pWeapon, g_Var.Ragebot[iWpnID].Spot, flMinDamage, matrix, flDamage, g_Var.Ragebot[iWpnID].Multipoint);*/

			//check if bodyaim would be a good idea and can do enough damage
			for (int i = HITBOX_PELVIS; i <= HITBOX_THORAX; i++)
			{
				vBodyBox = g_Rage.CalculatePoint(pLocal, pEnemy, pWeapon, i, flMinDamage, matrix, flDamage, g_Var.Ragebot[iWpnID].Multipoint);

				if (flDamage > flBaimDmg)
				{
					flBaimDmg = flDamage;
					vFilteredBox = vBodyBox;
					break;
				}
			}

			if (g_Var.Ragebot[iWpnID].BodyAimHp > 0.9 && g_Var.Ragebot[iWpnID].BodyAimHp >= pEnemy->GetHealth())
				bShouldBody = true;
			else
				if (flDamage > pEnemy->GetHealth())
					bShouldBody = true;
				else
					vFilteredBox = g_Rage.CalculatePoint(pLocal, pEnemy, pWeapon, g_Var.Ragebot[iWpnID].Spot, flMinDamage, matrix, flDamage, g_Var.Ragebot[iWpnID].Multipoint);

			// remove resolved maybe so it baims on lethal no matter what
			if (/*!sRecord->m_bResolved && */( bShouldBody || flDamage > flMinDamage ) && g_Var.Ragebot[iWpnID].Spot != HITBOX_HEAD)
				vHitBox = vFilteredBox;
			else
			{
				for (int i = 0; i < HITBOX_MAX; i++)
				{
					if (i <= 1)
					{
						if (GetAsyncKeyState(g_Var.Ragebot[iWpnID].ForceBaim) && g_Var.Ragebot[iWpnID].ForceBaim > 0)
							continue;

						if (pWeapon->GetWeaponID() == WEAPON_TASER)
							continue;

						if (!g_Var.Ragebot[iWpnID].Multiboxes[0])
							continue;

						if (g_Var.Ragebot[iWpnID].IgnoreHead[0] && !(sRecord->m_fFlags & FL_ONGROUND))
							continue;

						if (g_Var.Ragebot[iWpnID].IgnoreHead[1] && !sRecord->m_bResolved)
							continue;

						if (g_Var.Ragebot[iWpnID].IgnoreHead[2] && sRecord->m_flMaxAbsYaw > 32.f)
							continue;

						if (g_Var.Ragebot[iWpnID].BodyAfter > 0.9 && g_Var.Ragebot[iWpnID].BodyAfter <= g_Rage.iShotsFired[pEnemy->GetIndex()])
							continue;
					}

					if (!g_Var.Ragebot[iWpnID].Multiboxes[1] && (i <= 6 && i >= 2))
						continue;

					if (!g_Var.Ragebot[iWpnID].Multiboxes[3] && (i <= 10 && i >= 7))
						continue;

					if (!g_Var.Ragebot[iWpnID].Multiboxes[4] && (i <= 12 && i >= 11))
						continue;

					if (!g_Var.Ragebot[iWpnID].Multiboxes[2] && (i <= 18 && i >= 13))
						continue;

					if (i >= 7 && i <= 18)
					{
						if (g_Var.Ragebot[iWpnID].Limbs[0] && sRecord->m_vecVelocity.Length() > 25.f)
							continue;

						if (g_Var.Ragebot[iWpnID].Limbs[1] && !(sRecord->m_fFlags & FL_ONGROUND))
							continue;
					}

					vFilteredBox = g_Rage.CalculatePoint(pLocal, pEnemy, pWeapon, i, flMinDamage, matrix, flDamage, g_Var.Ragebot[iWpnID].Multipoint);

					if (vFilteredBox == Vector(0.f, 0.f, 0.f))
						continue;

					if (flDamage > flCurrentDamage)
					{
						flCurrentDamage = flDamage;
						vHitBox = vFilteredBox;
						break;
					}
				}
			}
		}
		else
		{
			if (g_Var.Ragebot[iWpnID].BodyAfter >= 1 && g_Var.Ragebot[iWpnID].BodyAfter <= g_Rage.iShotsFired[g_Rage.iTarget])
				vHitBox = g_Rage.CalculatePoint(pLocal, pEnemy, pWeapon, HITBOX_PELVIS, flMinDamage, matrix, flDamage, g_Var.Ragebot[iWpnID].Multipoint);
			else
				vHitBox = g_Rage.CalculatePoint(pLocal, pEnemy, pWeapon, g_Var.Ragebot[iWpnID].Spot, flMinDamage, matrix, flDamage, g_Var.Ragebot[iWpnID].Multipoint);
		}

		if (vHitBox != Vector(0.f, 0.f, 0.f))
		{
			sFinalRecord = sRecord;
			vFinal = vHitBox;
			break;
		}
	}

	
/*	for( auto& sRecord : ScanRecordsList)
	{
		g_History.Restore( pEnemy, sRecord );

		Vector vHitBox = Vector( 0.f, 0.f, 0.f );
		matrix3x4_t matrix[ 128 ];

		if (!GetBoneMatrix(pEnemy, sRecord, matrix))
			continue;

		float flDamage = 0.f;

		if( g_Var.Ragebot[ iWpnID ].Multibox )
		{
			Vector vPriorbox, vTempBox;
			float flBaimDmg = 0.f;

			for (int i = HITBOX_PELVIS; i <= HITBOX_THORAX; i++)
			{
				if (g_Var.Ragebot[iWpnID].BodyAimHp > 0.9 && g_Var.Ragebot[iWpnID].BodyAimHp >= pEnemy->GetHealth())
				{
					vTempBox = g_Rage.CalculatePoint(pLocal, pEnemy, pWeapon, i, flMinDamage, matrix, flDamage, g_Var.Ragebot[iWpnID].Multipoint);

					if (flDamage > flBaimDmg)
					{
						flBaimDmg = flDamage;
						vPriorbox = vTempBox;
					}
				}
				else
					vPriorbox = g_Rage.CalculatePoint(pLocal, pEnemy, pWeapon, g_Var.Ragebot[iWpnID].Spot, flMinDamage, matrix, flDamage, g_Var.Ragebot[iWpnID].Multipoint);
			}

			//baim if lethal get body dmg > health vhitbox = leathbox break

			if( g_Misc.IsVisible( pLocal, pEnemy, vPriorbox ) && g_Var.Ragebot[iWpnID].Spot != HITBOX_HEAD)
				vHitBox = vPriorbox;
			else
			{
				Vector vScreen = Vector( 0.f, 0.f, 0.f );
				Vector vHitbox = Vector( 0.f, 0.f, 0.f );
				float flCurrentDamage = 0.f;

				for( int i = 0; i < HITBOX_MAX; i++ )
				{
					if( i <= 1 )
					{
						if (GetAsyncKeyState(g_Var.Ragebot[iWpnID].ForceBaim) && g_Var.Ragebot[iWpnID].ForceBaim > 0)
							continue;

						if( pWeapon->GetWeaponID() == WEAPON_TASER )
							continue;

						if( !g_Var.Ragebot[ iWpnID ].Multiboxes[ 0 ] )
							continue;

						if( g_Var.Ragebot[ iWpnID ].IgnoreHead[ 0 ] && !( sRecord->m_fFlags & FL_ONGROUND ) )
							continue;

						if( g_Var.Ragebot[ iWpnID ].IgnoreHead[ 1 ] && !sRecord->m_bResolved )
							continue;

						if( g_Var.Ragebot[ iWpnID ].IgnoreHead[ 2 ] && sRecord->m_flMaxAbsYaw > 32.f )
							continue;

						if( g_Var.Ragebot[ iWpnID ].BodyAfter > 0.9 && g_Var.Ragebot[ iWpnID ].BodyAfter <= g_Rage.iShotsFired[ pEnemy->GetIndex() ] )
							continue;
					}

					if( !g_Var.Ragebot[ iWpnID ].Multiboxes[ 1 ] && ( i <= 6 && i >= 2 ) )
						continue;

					if( !g_Var.Ragebot[ iWpnID ].Multiboxes[ 3 ] && ( i <= 10 && i >= 7 ) )
						continue;

					if( !g_Var.Ragebot[ iWpnID ].Multiboxes[ 4 ] && ( i <= 12 && i >= 11 ) )
						continue;

					if( !g_Var.Ragebot[ iWpnID ].Multiboxes[ 2 ] && ( i <= 18 && i >= 13 ) )
						continue;

					if( i >= 7 && i <= 18 )
					{
						if( g_Var.Ragebot[ iWpnID ].Limbs[ 0 ] && sRecord->m_vecVelocity.Length() > 25.f )
							continue;

						if( g_Var.Ragebot[ iWpnID ].Limbs[ 1 ] && !( sRecord->m_fFlags & FL_ONGROUND ) )
							continue;
					}

					vHitbox = g_Rage.CalculatePoint( pLocal, pEnemy, pWeapon, i, flMinDamage, matrix, flDamage, g_Var.Ragebot[ iWpnID ].Multipoint );

					if( vHitbox == Vector( 0.f, 0.f, 0.f ) )
						continue;

					if( flDamage > flCurrentDamage )
					{
						flCurrentDamage = flDamage;
						vHitBox = vHitbox;
						break;
					}
				}
			}
		}
		else
		{
			if( g_Var.Ragebot[ iWpnID ].BodyAfter >= 1 && g_Var.Ragebot[ iWpnID ].BodyAfter <= g_Rage.iShotsFired[ g_Rage.iTarget ] )
				vHitBox = g_Rage.CalculatePoint( pLocal, pEnemy, pWeapon, HITBOX_PELVIS, flMinDamage, matrix, flDamage, g_Var.Ragebot[ iWpnID ].Multipoint );
			else
				vHitBox = g_Rage.CalculatePoint( pLocal, pEnemy, pWeapon, g_Var.Ragebot[ iWpnID ].Spot, flMinDamage, matrix, flDamage, g_Var.Ragebot[ iWpnID ].Multipoint );
		}

		if( vHitBox != Vector( 0.f, 0.f, 0.f ) && flDamage >= flMinDamage)
		{
			sRecord->m_vecPosition = pLocal->GetEyePosition();
			sFinalRecord = sRecord;
			vFinal = vHitBox;
			break;
		}
	}*/

//	if (pCmd->buttons & IN_ATTACK && g_Misc.CanShoot(pLocal, pWeapon) && sFinalRecord != nullptr)
//		g_Visuals.DrawHitbox(pEnemy->EntIndex(), sFinalRecord->m_Matrix);

	//maybe restore earlier already in the loop when its not needed anymore ? AND to current bone matrix :thinking:
	//RestoreToCurrentRecord( pEnemy );
	//*pEnemy->BoneCachePointer() = ( matrix3x4* ) OriginalMatrixPtr[ pEnemy->GetIndex() ];
	CurrentRestore.Restore();

	return sFinalRecord;
}

sPreBacktrackRecord::sPreBacktrackRecord(CBaseEntity * pEnemy)
{
	m_pEnt = pEnemy;
	m_vecOrigin = pEnemy->GetOrigin();
	m_vecAbsOrigin = pEnemy->GetAbsOrigin();
	m_angAbsAngles = pEnemy->GetAbsAngles();

	m_vecBBMin = *( Vector* ) ( ( DWORD ) pEnemy + Offsets::Entity::m_Collision + Offsets::Entity::m_vecMins );
	m_vecBBMax = *( Vector* ) ( ( DWORD ) pEnemy + Offsets::Entity::m_Collision + Offsets::Entity::m_vecMaxs );
	
	m_BoneCachePtr = *pEnemy->BoneCachePointer();
}

void sPreBacktrackRecord::Restore()
{
	*(Vector*)((DWORD)m_pEnt + Offsets::Player::m_vecOrigin) = m_vecOrigin;
	m_pEnt->SetAbsOrigin( m_vecAbsOrigin );
	m_pEnt->SetAbsAngles( m_angAbsAngles );

	*( Vector* ) ( ( DWORD ) m_pEnt + Offsets::Entity::m_Collision + Offsets::Entity::m_vecMins ) = m_vecBBMin;
	*( Vector* ) ( ( DWORD ) m_pEnt + Offsets::Entity::m_Collision + Offsets::Entity::m_vecMaxs ) = m_vecBBMax;
	
	*m_pEnt->BoneCachePointer() = m_BoneCachePtr;
}
