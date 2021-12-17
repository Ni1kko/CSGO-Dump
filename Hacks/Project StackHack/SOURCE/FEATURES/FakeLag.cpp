#include "../includes.h"
#include "../UTILS/interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CTrace.h"
#include "../SDK/CBaseWeapon.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/NetChannel.h"
#include "../SDK/CBaseAnimState.h"
#include "../SDK/ConVar.h"
#include "../FEATURES/Fakewalk.h"
#include "../FEATURES/AutoWall.h"
#include "../FEATURES/FakeLag.h"
#include "../FEATURES/Aimbot.h"
#include "../FEATURES/Backtracking.h"

int CFakeLag::lag_comp_break()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player || local_player->GetHealth() <= 0)
		return 1;

	auto velocity = local_player->GetVelocity();
	velocity.z = 0.f;
	auto speed = velocity.Length();
	auto distance_per_tick = speed * INTERFACES::Globals->interval_per_tick;
	int choked_ticks = std::ceilf(64.f / distance_per_tick);
	return std::min<int>(choked_ticks, 14.f);
}

template <class T>
constexpr const T& Max(const T& x, const T& y)
{
	return (x < y) ? y : x;
}

void CFakeLag::do_fakelag(SDK::CUserCmd* cmd)
{

	//GLOBAL::should_send_packet = true;
	int choke_amount;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player || local_player->GetHealth() <= 0)
		return;

	auto net_channel = INTERFACES::Engine->GetNetChannel();
	if (!net_channel)
		return;

	if (SETTINGS::settings.fake_duck && GetAsyncKeyState(VK_SHIFT))
		choke_amount = 14;
	else if (SETTINGS::settings.lag_bool)
	{
		//	if (fake_walk)
		//		choke_amount = 14;
		//	else
		//	{
		choke_amount = 1;
		if (!(local_player->GetFlags() & FL_ONGROUND))
		{
			switch (SETTINGS::settings.lag_type % 2)
			{
			case 0: choke_amount = SETTINGS::settings.jump_lag >= 1 ? SETTINGS::settings.jump_lag : 1; break;
			case 1: choke_amount = lag_comp_break(); break; // adaptive in menu, just breaks lag comp
			}
		}
		else if (local_player->GetVelocity().Length2D() > SETTINGS::settings.stand_velocity_threshold)
		{
			switch (SETTINGS::settings.lag_type % 2)
			{
			case 0: choke_amount = SETTINGS::settings.move_lag >= 1 ? SETTINGS::settings.move_lag : 1; break;
			case 1: choke_amount = lag_comp_break(); break;
			}

			/*if (SETTINGS::settings.fakelag_onpeek)
			{
				auto GetHitboxPosition = [local_player](int hitbox_index) -> Vector
				{
					if (local_player->GetIsDormant() || local_player->GetHealth() <= 0)
						return Vector(0, 0, 0);

					const auto pModel = local_player->GetModel();
					if (!pModel)
						return Vector(0, 0, 0);

					auto pStudioHdr = INTERFACES::ModelInfo->GetStudioModel(pModel);
					if (!pStudioHdr)
						return Vector(0, 0, 0);

					auto pSet = pStudioHdr->pHitboxSet(0);
					if (!pSet)
						return Vector(0, 0, 0);

					if (hitbox_index >= pSet->numhitboxes || hitbox_index < 0)
						return Vector(0, 0, 0);

					auto hitbox = pSet->GetHitbox(hitbox_index);
					if (!hitbox)
						return Vector(0, 0, 0);

					const auto bone_matrix = local_player->GetBoneMatrix(hitbox->bone);
					const auto bone_pos = Vector(bone_matrix[0][3], bone_matrix[1][3], bone_matrix[2][3]);

					return ((hitbox->bbmax + hitbox->bbmin) * 0.5f) + bone_pos;
				};

				local_player->SetupBones(nullptr, 128, BONE_USED_BY_ANYTHING, UTILS::GetCurtime());
				const auto end_position_1 = GetHitboxPosition(18),
					end_position_2 = GetHitboxPosition(16); //18, 16

				for (int i = 0; i < 64; i++)
				{
					auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
					if (!entity || entity->GetIsDormant() || entity->GetHealth() <= 0 || entity->GetTeam() == local_player->GetTeam())
						continue;

					const auto damage_1 = AutoWall->CalculateDamage(entity->GetVecOrigin() + Vector(0, 0, 64.f), local_player->GetVecOrigin(),
						entity, local_player, 1).damage, damage_2 = AutoWall->CalculateDamage(entity->GetVecOrigin() + Vector(0, 0, 64.f),
							local_player->GetVecOrigin(), entity, local_player, 1).damage;

					if (Max<int>(damage_1, damage_2) > 0)
					{
						GLOBAL::choke_amount = 14;
						break;
					}
				}
			}*/
		}
		else
		{
			switch (SETTINGS::settings.lag_type % 2)
			{
			case 0: choke_amount = SETTINGS::settings.stand_lag >= 1 ? SETTINGS::settings.stand_lag : 1; break;
			case 1: choke_amount = SETTINGS::settings.stand_lag >= 1 ? SETTINGS::settings.stand_lag : 1; break;
			}
		}


	}
	else
		choke_amount = 1;

	if (net_channel->m_nChokedPackets >= min(14, choke_amount))// || (net_channel->m_nChokedPackets > 0 && GLOBAL::disable_fakelag))
	{
		//GLOBAL::disable_fakelag = false;
		GLOBAL::should_send_packet = true;
		
	}
	else
	{ 
	//	GLOBAL::disable_fakelag = true;
		GLOBAL::should_send_packet = false;
		
		
	}
	
}

CFakeLag* fakelag = new CFakeLag();













































































































































































































































































