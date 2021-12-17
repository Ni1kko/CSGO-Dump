#include "includes.h"
#include "UTILS/interfaces.h"
#include "SDK/IEngine.h"
#include "SDK/CUserCmd.h"
#include "SDK/CBaseEntity.h"
#include "SDK/CClientEntityList.h"
#include "SDK/CTrace.h"
#include "SDK/CBaseWeapon.h"
#include "SDK/CGlobalVars.h"
#include "SDK/ConVar.h"
#include "FEATURES/AutoWall.h"
#include "FEATURES/Aimbot.h"
#include "FEATURES/Movement.h"
#include "HOOKS/hooks.h"
#include "NewAimbot.h"
#include "NewBacktracking.h"

/*#define WEAPON_TYPE_PISTOL 1
#define WEAPON_TYPE_SNIPER 5
#define WEAPON_TYPE_GRENADE 9
#define WEAPON_TYPE_KNIFE 0

#define enc_str(s) std::string(s)
#define enc_char(s) enc_str(s).c_str()
#define enc_wstr(s) std::wstring(enc_str(s).begin(), enc_str(s).end())
#define enc_wchar(s) enc_wstr(s).c_str()

float GetLatency()
{
	auto nci = INTERFACES::Engine->GetNetChannelInfo();
	if (!nci)
		return 0.f;

	return nci->GetLatency(FLOW_OUTGOING);
}


void inline SinCos1(float radians, float* sine, float* cosine)
{
	*sine = sin(radians);
	*cosine = cos(radians);
}

void AAngleVectors(const Vector& angles, Vector* forward, Vector* right, Vector* up)
{
	float sr, sp, sy, cr, cp, cy;
	SinCos1(DEG2RAD(angles[1]), &sy, &cy);
	SinCos1(DEG2RAD(angles[0]), &sp, &cp);
	SinCos1(DEG2RAD(angles[2]), &sr, &cr);

	if (forward)
	{
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}
	if (right)
	{
		right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
		right->y = (-1 * sr * sp * sy + -1 * cr * cy);
		right->z = -1 * sr * cp;
	}
	if (up)
	{
		up->x = (cr * sp * cy + -sr * -sy);
		up->y = (cr * sp * sy + -sr * cy);
		up->z = cr * cp;
	}
}

template <class T>
constexpr const T& Min(const T& x, const T& y)
{
	return (x > y) ? y : x;
}

template <class T>
constexpr const T& Max(const T& x, const T& y)
{
	return (x < y) ? y : x;
}

template <class T>
constexpr const T& Clamp(const T& v, const T& lo, const T& hi)
{
	return (v >= lo && v <= hi) ? v : (v < lo ? lo : hi);
}

bool IsWeaponGrenade12(SDK::CBaseWeapon* weapon)
{
	if (!weapon)
		return false;

	auto weapon_info = weapon->GetWeaponInfo();
	if (!weapon_info)
		return false;

	if (weapon_info->m_WeaponType == WEAPON_TYPE_GRENADE)
		return true;

	return false;
}

bool IsWeaponSniper12(SDK::CBaseWeapon* weapon)
{
	if (!weapon)
		return false;

	auto weapon_info = weapon->GetWeaponInfo();
	if (!weapon_info)
		return false;

	if (weapon_info->m_WeaponType == WEAPON_TYPE_SNIPER)
		return true;

	return false;
}
bool IsWeaponTaser12(SDK::CBaseWeapon* weapon)
{
	if (!weapon)
		return false;

	if (weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_TASER)
		return true;

	return false;
}

bool IsWeaponKnife12(SDK::CBaseWeapon* weapon)
{
	if (!weapon)
		return false;

	auto weapon_info = weapon->GetWeaponInfo();
	if (!weapon_info)
		return false;

	if (weapon_info->m_WeaponType == WEAPON_TYPE_KNIFE && !IsWeaponTaser12(weapon))
		return true;

	return false;
}



bool IsPressingMovementKeys12(SDK::CUserCmd* cmd)
{
	if (!cmd)
		return false;

	if (cmd->buttons & IN_FORWARD ||
		cmd->buttons & IN_BACK || cmd->buttons & IN_LEFT || cmd->buttons & IN_RIGHT ||
		cmd->buttons & IN_MOVELEFT || cmd->buttons & IN_MOVERIGHT)
		return true;

	return false;
}

void RotateMovement12(SDK::CUserCmd* cmd, float yaw)
{
	Vector viewangles;
	INTERFACES::Engine->GetViewAngles(viewangles);

	float rotation = DEG2RAD(viewangles.y - yaw);

	float cos_rot = cos(rotation);
	float sin_rot = sin(rotation);

	float new_forwardmove = (cos_rot * cmd->move.y) - (sin_rot * cmd->move.x);
	float new_sidemove = (sin_rot * cmd->move.y) + (cos_rot * cmd->move.x);

	cmd->move.y = new_forwardmove;
	cmd->move.x = new_sidemove;
}

int TicksToStop12(Vector velocity)
{
	static auto sv_maxspeed = INTERFACES::cvar->FindVar(enc_char("sv_maxspeed"));

	const float max_speed = 320.f;
	const float acceleration = 5.5f;
	const float max_accelspeed = acceleration * max_speed * INTERFACES::Globals->interval_per_tick;

	return velocity.Length() / max_accelspeed;
}

SDK::CBaseWeapon* GetWeapon12(SDK::CBaseEntity* entity)
{
	return reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(entity->GetActiveWeaponIndex()));
}

bool CanShoot(bool right_click)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player || local_player->GetHealth() <= 0 || local_player->GetNextAttack() > UTILS::GetCurtime())
		return false;

	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (!weapon)
		return false;

	/// special shit for nades
	if (IsWeaponGrenade12(weapon))
	{
		if (weapon->IsPinPulled())
			return false;

		if (weapon->GetThrowTime() <= 0 || weapon->GetThrowTime() > UTILS::GetCurtime())
			return false;

		return true;
	}

	if (right_click)
	{
		if (weapon->GetNextSecondaryAttack() >UTILS::GetCurtime())
			return false;
	}
	else
	{
		if (weapon->GetNextSecondaryAttack() > UTILS::GetCurtime())
			return false;

		/// revolver
		if (weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_REVOLVER && weapon->GetPostponeFireReadyTime() > UTILS::GetCurtime())
			return false;
	}

	return true;
}

namespace FEATURES
{
	namespace RAGEBOT
	{

		Aimbot aimbot;

		void Aimbot::Do(SDK::CUserCmd* cmd)
		{
			GLOBAL::can_shoot_someone = false;
			CBacktracking* backtrack = nullptr;
			
			Backtracking_Records* bt_records = nullptr;
			auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
			if (!local_player || local_player->GetHealth() <= 0 || !SETTINGS::settings.aim_bool ||
				IsWeaponKnife12(GetWeapon12(local_player)) ||IsWeaponGrenade12(GetWeapon12(local_player)))
				return;

			const auto localweapon = GetWeapon12(local_player);

			auto BoundingBoxCheck = [this, local_player](SDK::CBaseEntity* entity, const Backtracking_Records& record) -> bool
			{
				const auto bbmin = record.bbmin + record.vec_origin;
				const auto bbmax = record.bbmax + record.vec_origin;

				/// the 4 corners on the top, 1 for the head, 1 for the middle of the body, 1 for the feet
				Vector points[7];

				points[0] = GetHitboxPosition(entity, 0);
				points[1] = (bbmin + bbmax) * 0.5f;
				points[2] = Vector((bbmax.x + bbmin.x) * 0.5f, (bbmax.y + bbmin.y) * 0.5f, bbmin.z);

				points[3] = bbmax;
				points[4] = Vector(bbmax.x, bbmin.y, bbmax.z);
				points[5] = Vector(bbmin.x, bbmin.y, bbmax.z);
				points[6] = Vector(bbmin.x, bbmax.y, bbmax.z);

				for (const auto& point : points)
				{
					if (AutoWall->CalculateDamage(local_player->GetVecOrigin() + local_player->GetViewOffset(), point, local_player, entity, 1).damage > 0)
						return true;
				}

				return false;
			};

			/// loop through every entity to find valid ones to aimbot
			for (int i = 0; i < 64; i++)
			{
				/// reset player aimbot info
				player_aimbot_info[i].head_damage = -1, player_aimbot_info[i].hitscan_damage = -1;

				auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
				if (!entity || entity->GetIsDormant() || entity->GetImmunity() || entity->GetHealth() <= 0 || entity->GetTeam() == local_player->GetTeam())
					continue;

				int most_damage = 0;
				for (const auto& rec : backtrack->GetPriorityRecords(entity))
				{
					const int tick = backtrack->GetTickCount(rec);

					/// optimization
					if (!BoundingBoxCheck(entity, rec))
						continue;

					int random_ticks = 0;
					if (rec.is_exploiting)
						random_ticks = TIME_TO_TICKS(GetLatency());

					backtrack->Restore(entity, rec, random_ticks);
					backtrack->ApplyRestore(entity, TICKS_TO_TIME(tick));

					int cur_head_damage, cur_hitscan_damage;
					Vector cur_head_pos, cur_hitscan_pos;

					DoHeadAim(entity, cur_head_pos, cur_head_damage);
					DoHitscan(entity, cur_hitscan_pos, cur_hitscan_damage);

					if (cur_head_damage > most_damage || cur_hitscan_damage > most_damage)
					{
						most_damage = Max<int>(cur_head_damage, cur_hitscan_damage);

						player_aimbot_info[i].head_damage = cur_head_damage;
						player_aimbot_info[i].hitscan_damage = cur_hitscan_damage;

						player_aimbot_info[i].head_position = cur_head_pos;
						player_aimbot_info[i].hitscan_position = cur_hitscan_pos;

						player_aimbot_info[i].tick = tick;
						//player_aimbot_info[i].backtrack_record = rec;
						player_aimbot_info[i].extrapolted_ticks = random_ticks;

						if (cur_hitscan_damage > entity->GetHealth())
							break;
					}
				}
			}

			int highest_damage = 0, best_tick, last_extrapolated_ticks;
			Backtracking_Records last_backtrack_record;
			SDK::CBaseEntity* best_entity = nullptr;
			bool is_hitscan = false;
			Vector best_position;

			/// sort valid entities by most damage
			for (int i = 0; i < 64; i++)
			{
				auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
				if (!entity)
					continue;

				auto FillInfo = [&is_hitscan, &best_position, &best_entity, &last_backtrack_record, &last_extrapolated_ticks, &highest_damage, &best_tick, entity, i, this](bool hitscan) -> void
				{
					is_hitscan = hitscan, best_tick = player_aimbot_info[i].tick;
					best_entity = entity;
					
					last_extrapolated_ticks = player_aimbot_info[i].extrapolted_ticks;

					if (hitscan)
					{
						best_position = player_aimbot_info[i].hitscan_position;
						highest_damage = player_aimbot_info[i].hitscan_damage;
					}
					else
					{
						best_position = player_aimbot_info[i].head_position;
						highest_damage = player_aimbot_info[i].head_damage;
					}
				};
				const int hitscan_dmg = player_aimbot_info[i].hitscan_damage, head_dmg = player_aimbot_info[i].head_damage;

				/// if exploiting or jumping force baim and entirely ignore head
				const bool is_exploiting = bt_records->is_exploiting;
				if (is_exploiting || (!(entity->GetFlags() & FL_ONGROUND)))
				{
					if (hitscan_dmg > highest_damage)
						FillInfo(true);

					continue;
				}

				/// can't shoot this entity, continue
				if (hitscan_dmg <= 0 && head_dmg <= 0)
					continue;

				/// one of these is not valid, choose the one that is valid
				if (hitscan_dmg <= 0 || head_dmg <= 0)
				{
					FillInfo(hitscan_dmg > head_dmg);
					continue;
				}

				/// hitscan if lethal or if more than head damage
				if (hitscan_dmg > entity->GetHealth() || hitscan_dmg > head_dmg)
				{
					FillInfo(true);
					if (hitscan_dmg > entity->GetHealth())
						break;

					continue;
				}

				/// if resolved go for head
				/*if (FEATURES::RAGEBOT::resolver.IsResolved(player_aimbot_info[i].backtrack_record.resolve_record.resolve_type))
				{
				fcuk dis
					FillInfo(false);
					break;
				}*/
       
				/*const bool baim_if_unplugged = (SETTINGS::settings.baim_if_unplugged && fabs(UTILS::GetCurtime() - resolver->last_time_movingr) > 1.f);
			const bool prefer_baim = SETTINGS::settings.baimiflethal || !(entity->GetFlags() & FL_ONGROUND) || (SETTINGS::settings.baim_if_x_damage < player_aimbot_info[i].hitscan_damage);
				if (prefer_baim || baim_if_unplugged)
				{
					FillInfo(true);

					continue;
				}

				FillInfo(false);
			}

			static float duration_spent_aiming = 0.f;

			/// we have a valid target to shoot at
			if (best_entity)
			{
				GLOBAL::can_shoot_someone = true;

				duration_spent_aiming += INTERFACES::Globals->interval_per_tick;

				/// maybe implement a knifebot sometime in the future?
				const bool should_right_click = false;

				/// the weapon is ready to fire
				if (CanShoot(should_right_click))
				{
					/// autoscope
					if (!local_player->IsScopedIn() && CanShoot(true) && IsWeaponSniper12(GetWeapon12(local_player)) && local_player->GetFlags() & FL_ONGROUND)
						cmd->buttons |= IN_ATTACK2;

					/// need to restore to the player's backtrack record so that hitchance can work properly
					backtrack->Restore(best_entity, last_backtrack_record, last_extrapolated_ticks);
					backtrack->ApplyRestore(best_entity, best_tick);

					/// delay shot
					auto DelayShot = [best_entity, last_backtrack_record, highest_damage, local_player, is_hitscan]() -> bool
					{
						/// jumping
						if (!(best_entity->GetFlags() & FL_ONGROUND))
							return true;

						if (highest_damage >= best_entity->GetHealth() + 10)
							return true;

						if (best_entity->GetVelocity().Length2D() > 75.f || local_player->GetVelocity().Length2D() > 75.f) /// moving fast
							return duration_spent_aiming >= SETTINGS::settings.rage_lagcompensation;

						return true;
					};
					if (DelayShot())
					{
						if (SETTINGS::settings.autostop && local_player->GetFlags() & FL_ONGROUND)
						{
							GLOBAL::originalCMD.move.x = 0;
							GLOBAL::originalCMD.move.y = local_player->GetVelocity().Length2D() > 20.f ? 450.f : 0.f;

							RotateMovement12(cmd, UTILS::CalcAngle(Vector(0, 0, 0), local_player->GetVelocity()).y + 180.f);
						}
						const auto net_channel = INTERFACES::Engine->GetNetChannel();
						/// if is accurate enough to shoot, then shoot... (hitchance, spread limit, etc)
						if (IsAccurate(best_entity, best_position) && net_channel && net_channel->m_nChokedPackets)
						{
							

							if (should_right_click)
								cmd->buttons |= IN_ATTACK2;
							else
								cmd->buttons |= IN_ATTACK;

							cmd->viewangles = UTILS::CalcAngle(local_player->GetVecOrigin() + local_player->GetViewOffset(), best_position) - (local_player->GetPunchAngles() * 2.f);
							cmd->tick_count = best_tick;

							/// resolver
							auto IsSidewaysAntiaim = [is_hitscan, best_entity, local_player, last_backtrack_record]() -> bool
							{
								const float at_target_yaw = MATH::NormalizeYaw(UTILS::CalcAngle(local_player->GetVecOrigin(), best_entity->GetVecOrigin()).y);

								const float delta_1 = fabs(MATH::NormalizeYaw((at_target_yaw + 90.f) - last_backtrack_record.eye_angles.y)),
									delta_2 = fabs(MATH::NormalizeYaw((at_target_yaw - 90.f) - last_backtrack_record.eye_angles.y));

								return Min<float>(delta_1, delta_2) > 30.f;
							};
							

							/// psilent
							GLOBAL::should_send_packet = false;
							GLOBAL::disable_fakelag = true;
						}
					}
					else if (SETTINGS::settings.autostop && local_player->GetFlags() & FL_ONGROUND)
					{
						const int ticks_to_stop = TicksToStop12(local_player->GetVelocity());
						const auto new_eye_position = local_player->GetVecOrigin() + local_player->GetViewOffset() +
							(local_player->GetVelocity() * INTERFACES::Globals->interval_per_tick * ticks_to_stop);

						if (AutoWall->CalculateDamage(new_eye_position, GetHitboxPosition(best_entity, 0), local_player, best_entity, 1).damage > best_entity->GetHealth())
						{
							cmd->move.x = 0;
							cmd->move.y = local_player->GetVelocity().Length2D() > 13.f ? 450.f : 0.f;

							RotateMovement12(cmd, UTILS::CalcAngle(Vector(0, 0, 0), local_player->GetVelocity()).y + 180.f);
							std::cout << ticks_to_stop << std::endl;
						}
					}
				}
				else if (localweapon && localweapon->GetItemDefenitionIndex() == SDK::WEAPON_REVOLVER)
					cmd->buttons |= IN_ATTACK;

				
			}

			/// restore all players back to their current record
			for (int i = 0; i < 64; i++)
			{
				auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
				if (!entity || entity->GetIsDormant() || entity->GetImmunity() || entity->GetHealth() <= 0 || entity->GetTeam() == local_player->GetTeam())
					continue;

				backtrack->RestoreToCurrentRecord(entity);
			}

			if (!best_entity)
				duration_spent_aiming = 0.f;
		}

		bool Aimbot::DoHitscan(SDK::CBaseEntity* entity, Vector& final_position, int& final_damage)
		{
			/// default value
			final_damage = 0;

			auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
			if (!local_player)
				return false;

			auto weapon = GetWeapon12(local_player);
			if (!weapon)
				return false;

			const auto eye_position = local_player->GetVecOrigin() + local_player->GetViewOffset();
			const bool is_taser = IsWeaponTaser12(weapon);
			const bool should_ignore_limbs = entity->GetVelocity().Length2D() > 30.f;
			const float pointscale = SETTINGS::settings.bodypointscale * 0.01f;

			/// if taser or low on ammo, set minimum damage to their health so that it kills in a single shot
			const int minimum_damage = (is_taser || weapon->GetLoadedAmmo() <= 2) ? entity->GetHealth() + 10 : Min<int>(SETTINGS::settings.damage_val, entity->GetHealth() + 10);
			const int minimum_autowall_damage = (is_taser || weapon->GetLoadedAmmo() <= 2) ? entity->GetHealth() + 10 : Min<int>(SETTINGS::settings.damage_val, entity->GetHealth() + 10);

			/// functions for creating the hitscan positions
			auto ConstructHitscanMultipointPositions = [this, entity, pointscale, eye_position, local_player](bool ignore_limbs) -> std::vector<std::pair<Vector, int>>
			{
				std::vector<std::pair<Vector, int>> positions;

				auto CreateAndAddBufferToVec = [this, entity](std::vector<std::pair<Vector, int>>& vec, int hitbox_index, float pointscale)
				{
					std::vector<Vector> buffer;
					GetMultipointPositions(entity, buffer, hitbox_index, pointscale);

					for (const auto& x : buffer)
						vec.emplace_back(x, hitbox_index);
				};
				auto GetSpreadDistance = [this, local_player](float distance) -> float
				{
					auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
					if (!weapon)
						return 0.f;

					Vector viewangles;
					INTERFACES::Engine->GetViewAngles(viewangles);

					weapon->UpdateAccuracyPenalty();
					float spread = weapon->GetSpreadCone();
					float inaccuracy = weapon->GetInaccuracy();

					Vector forward, right, up;
					AAngleVectors(viewangles, &forward, &right, &up);

					float RandomA = 0;
					float RandomB = 1.0f - RandomA * RandomA;

					RandomA = M_PI_F * 2.0f;
					RandomB *= spread;

					float SpreadX1 = (cos(RandomA) * RandomB);
					float SpreadY1 = (sin(RandomA) * RandomB);

					float RandomC = 0;
					float RandomF = 1.0f - RandomC * RandomC;

					RandomC = M_PI_F * 2.0f;
					RandomF *= inaccuracy;

					float SpreadX2 = (cos(RandomC) * RandomF);
					float SpreadY2 = (sin(RandomC) * RandomF);

					float fSpreadX = SpreadX1 + SpreadX2;
					float fSpreadY = SpreadY1 + SpreadY2;

					Vector vSpreadForward;
					vSpreadForward[0] = forward[0] + (fSpreadX * right[0]) + (fSpreadY * up[0]);
					vSpreadForward[1] = forward[1] + (fSpreadX * right[1]) + (fSpreadY * up[1]);
					vSpreadForward[2] = forward[2] + (fSpreadX * right[2]) + (fSpreadY * up[2]);
					vSpreadForward.NormalizeInPlace();

					Vector qaNewAngle;
					MATH::VectorAngles(vSpreadForward, qaNewAngle);
					qaNewAngle = MATH::NormalizeAngle(qaNewAngle);

					Vector vEnd;
					MATH::AngleVectors(qaNewAngle, &vEnd);

					return ((vEnd - forward) * distance).Length();
				};
				auto GetIdealPointscale = [this, entity, eye_position, GetSpreadDistance](int hitbox_index) -> float
				{
					const auto hitbox = GetHitbox(entity, hitbox_index);
					if (!hitbox)
						return 0.f;

					const float hitbox_radius = hitbox->radius,
						spread_distance = GetSpreadDistance((eye_position - entity->GetVecOrigin()).Length());

					float ideal_pointscale = 0.f;
					if (spread_distance > hitbox_radius * 0.5f)
						return ideal_pointscale;

					ideal_pointscale = ((hitbox_radius * 0.5f) - spread_distance) / (hitbox_radius * 0.5f);

					return Clamp<float>(ideal_pointscale - 0.1f, 0.f, 0.8f);
				};


				
					CreateAndAddBufferToVec(positions, 6, GetIdealPointscale(6));
					CreateAndAddBufferToVec(positions, 5, GetIdealPointscale(5));
				
				
					CreateAndAddBufferToVec(positions, 4, GetIdealPointscale(4));
					CreateAndAddBufferToVec(positions, 3, GetIdealPointscale(3));
			
					CreateAndAddBufferToVec(positions, 18, GetIdealPointscale(18));
					CreateAndAddBufferToVec(positions, 16, GetIdealPointscale(16));
				
				
					CreateAndAddBufferToVec(positions, 9, GetIdealPointscale(9));
					CreateAndAddBufferToVec(positions, 8, GetIdealPointscale(8));
				

				return positions;
			};
			auto ConstructHitscanPositions = [this, entity](bool ignore_limbs) -> std::vector<std::pair<Vector, int>>
			{
				std::vector<std::pair<Vector, int>> positions;

				
					positions.emplace_back(GetHitboxPosition(entity, 6), 6);
					positions.emplace_back(GetHitboxPosition(entity, 5), 5);
			
				
					positions.emplace_back(GetHitboxPosition(entity, 4), 4);
					positions.emplace_back(GetHitboxPosition(entity, 3), 3);
				
					positions.emplace_back(GetHitboxPosition(entity, 18), 18);
					positions.emplace_back(GetHitboxPosition(entity, 16), 16);
				
				
					positions.emplace_back(GetHitboxPosition(entity, 9), 9);
					positions.emplace_back(GetHitboxPosition(entity, 8), 8);
				
				
					positions.emplace_back(GetHitboxPosition(entity, 13), 13);
					positions.emplace_back(GetHitboxPosition(entity, 12), 12);
					positions.emplace_back(GetHitboxPosition(entity, 11), 11);
					positions.emplace_back(GetHitboxPosition(entity, 10), 10);
				

				return positions;
			};

			/// get the positions
			const auto hitscan_mp_positions = ConstructHitscanMultipointPositions(should_ignore_limbs);
			const auto hitscan_positions = ConstructHitscanPositions(should_ignore_limbs);

			/// get the best multipoint hitscan position
			int mp_damage = -1, mp_min_damage = 0;
			Vector mp_position;
			for (const auto& pair : hitscan_mp_positions)
			{
				const auto info = AutoWall->CalculateDamage(eye_position, pair.first, local_player, entity, is_taser ? 0 : -1);
				const int min_dmg = info.did_penetrate_wall ? minimum_autowall_damage : minimum_damage;

				if (info.damage > mp_damage)
				{
					mp_damage = info.damage, mp_position = pair.first;
					mp_min_damage = info.did_penetrate_wall ? minimum_autowall_damage : minimum_damage;
				}
			}

			/// get the best hitscan position
			int hitscan_damage = -1, hitscan_min_damage = 0, hitscan_hitbox = 0;
			Vector hitscan_position;
			for (const auto& pair : hitscan_positions)
			{
				const auto info = AutoWall->CalculateDamage(eye_position, pair.first, local_player, entity, is_taser ? 0 : -1);
				const bool has_pos_already = hitscan_damage != -1;

				if (info.damage > hitscan_damage)
				{
					/// feet are the least priority
					//const bool is_feet = pair.second >= 10 && pair.second <= 13;
					//if (is_feet && has_pos_already)
					//continue;

					hitscan_hitbox = pair.second;
					hitscan_damage = info.damage, hitscan_position = pair.first;
					hitscan_min_damage = info.did_penetrate_wall ? minimum_autowall_damage : minimum_damage;
				}
			}

			/// no positions deal enough damage
			if (hitscan_damage < hitscan_min_damage && mp_damage < mp_min_damage)
				return false;

			/// one of these are not valid, choose the one that is valid
			if (hitscan_damage < hitscan_min_damage || mp_damage < mp_min_damage)
			{
				if (hitscan_damage >= hitscan_min_damage)
				{
					final_position = hitscan_position;
					final_damage = hitscan_damage;
					return true;
				}
				else
				{
					final_position = mp_position;
					final_damage = mp_damage;
					return true;
				}
			}

			/// just choose the position that does most damage gg
			auto PrioritiseHitscan = [mp_damage, hitscan_hitbox, hitscan_damage, entity]() -> bool
			{
				switch (hitscan_hitbox)
				{
				case 6:
				case 5:
				case 4:
				case 3:
				{
					/// is lethal
					if (hitscan_damage > entity->GetHealth() + 10)
						return true;

					if (mp_damage > entity->GetHealth())
						return false;

					/// less than 10 damage difference, prefer hitscan over multipoint 
					if (abs(mp_damage - hitscan_damage) < 10)
						return true;
				}
				default:
					return false;
				}
			};
			if (hitscan_damage > mp_damage || PrioritiseHitscan())
			{
				final_position = hitscan_position;
				final_damage = hitscan_damage;
			}
			else
			{
				final_position = mp_position;
				final_damage = mp_damage;
			}

			return true;
		}

		bool Aimbot::DoHeadAim(SDK::CBaseEntity* entity, Vector& final_position, int& final_damage)
		{
			/// default value
			final_damage = 0;

			auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
			if (!local_player)
				return false;

			auto weapon = GetWeapon12(local_player);
			if (!weapon)
				return false;

			if (IsWeaponTaser12(weapon))
				return false;

			const auto eye_position = local_player->GetVecOrigin() + local_player->GetViewOffset();
			const float pointscale = SETTINGS::settings.headpointscale * 0.01f;

			/// if taser or low on ammo, set minimum damage to their health so that it kills in a single shot
			const int minimum_damage = (weapon->GetLoadedAmmo() <= 2) ? entity->GetHealth() : Min<int>(SETTINGS::settings.damage_val, entity->GetHealth() + 10);
			const int minimum_autowall_damage = (weapon->GetLoadedAmmo() <= 2) ? entity->GetHealth() : Min<int>(SETTINGS::settings.damage_val, entity->GetHealth() + 10);

			/// center of the hitbox
			const auto head_center_position = GetHitboxPosition(entity, 0);
			const auto head_center_info = AutoWall->CalculateDamage(eye_position, head_center_position, local_player, entity, 1);
			const int head_center_damage = head_center_info.damage,
				head_center_min_damage = head_center_info.did_penetrate_wall ? minimum_autowall_damage : minimum_damage;

			/// the multipoints around the center of the hitbox
			std::vector<Vector> head_multipoints;
			GetMultipointPositions(entity, head_multipoints, 0, pointscale);

			/// get the multipoint position that does the most damage
			Vector best_multipoint_position;
			int best_multipoint_damage = -1, multipoint_min_damage = 0;
			for (const auto& x : head_multipoints)
			{
				const auto info = AutoWall->CalculateDamage(eye_position, x, local_player, entity, 1);

				if (info.damage > best_multipoint_damage)
				{
					best_multipoint_damage = info.damage, best_multipoint_position = x;
					multipoint_min_damage = info.did_penetrate_wall ? minimum_autowall_damage : minimum_damage;
				}
			}

			/// none are valid
			if (head_center_damage < minimum_damage && best_multipoint_damage < minimum_damage)
				return false;

			/// one of these aren't valid, choose the valid one
			if (head_center_damage < head_center_min_damage || best_multipoint_damage < multipoint_min_damage)
			{
				if (head_center_damage > best_multipoint_damage)
				{
					final_position = head_center_position;
					final_damage = head_center_damage;
				}
				else
				{
					final_position = best_multipoint_position;
					final_damage = best_multipoint_damage;
				}
			}
			else /// both are valid, do some more decision making 
			{
				/// decide whether to shoot the center of the hitbox or the multipoints of the hitbox
				if (head_center_damage > entity->GetHealth() + 10 || head_center_damage + 10 > best_multipoint_damage)
				{
					final_position = head_center_position;
					final_damage = head_center_damage;
				}
				else
				{
					final_position = best_multipoint_position;
					final_damage = best_multipoint_damage;
				}
			}

			return true;
		}

		bool Aimbot::IsAccurate(SDK::CBaseEntity* entity, Vector position)
		{
			auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
			if (!local_player)
				return false;

			auto weapon = GetWeapon12(local_player);
			if (!weapon)
				return false;

			if (IsWeaponKnife12(weapon) || IsWeaponTaser12(weapon))
				return true;

			
				auto RandomFloat = [](float min, float max) -> float
				{
					typedef float(*RandomFloat_t)(float, float);
					static RandomFloat_t m_RandomFloat = (RandomFloat_t)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat");
					return m_RandomFloat(min, max);
				};

				Vector angle = MATH::NormalizeAngle(UTILS::CalcAngle(local_player->GetVecOrigin() + local_player->GetViewOffset(), position));

				Vector forward, right, up;
				AAngleVectors(angle, &forward, &right, &up);

				int iHit = 0;
				weapon->UpdateAccuracyPenalty();
				for (int i = 0; i < 256; i++)
				{
					float RandomA = RandomFloat(0.0f, 1.0f);
					float RandomB = 1.0f - RandomA * RandomA;

					RandomA = RandomFloat(0.0f, M_PI_F * 2.0f);
					RandomB *= weapon->GetSpreadCone();

					float SpreadX1 = (cos(RandomA) * RandomB);
					float SpreadY1 = (sin(RandomA) * RandomB);

					float RandomC = RandomFloat(0.0f, 1.0f);
					float RandomF = 1.0f - RandomC * RandomC;

					RandomC = RandomFloat(0.0f, M_PI_F * 2.0f);
					RandomF *= weapon->GetInaccuracy();

					float SpreadX2 = (cos(RandomC) * RandomF);
					float SpreadY2 = (sin(RandomC) * RandomF);

					float fSpreadX = SpreadX1 + SpreadX2;
					float fSpreadY = SpreadY1 + SpreadY2;

					Vector vSpreadForward;
					vSpreadForward[0] = forward[0] + (fSpreadX * right[0]) + (fSpreadY * up[0]);
					vSpreadForward[1] = forward[1] + (fSpreadX * right[1]) + (fSpreadY * up[1]);
					vSpreadForward[2] = forward[2] + (fSpreadX * right[2]) + (fSpreadY * up[2]);
					vSpreadForward.NormalizeInPlace();

					Vector qaNewAngle;
					MATH::VectorAngles(vSpreadForward, qaNewAngle);
					qaNewAngle = MATH::NormalizeAngle(qaNewAngle);

					Vector vEnd;
					MATH::AngleVectors(qaNewAngle, &vEnd);
					vEnd = local_player->GetVecOrigin() + local_player->GetViewOffset() + (vEnd * 8192.f);

					SDK::trace_t trace;
					SDK::CTraceFilterOneEntity2 filter;
					filter.pEntity = entity;
					SDK::Ray_t ray;
					ray.Init(local_player->GetVecOrigin() + local_player->GetViewOffset(), vEnd);

					INTERFACES::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);
					if (trace.m_pEnt == entity)
						iHit++;

					if (iHit / 256.f >= SETTINGS::settings.chance_val / 100.f)
						return true;
				}

				return false;
		}

		void Aimbot::GetMultipointPositions(SDK::CBaseEntity* entity, std::vector<Vector>& positions, int hitbox_index, float pointscale)
		{
			const auto hitbox = GetHitbox(entity, hitbox_index);
			if (!hitbox)
				return;

			const float hitbox_radius = hitbox->radius * pointscale;

			// 2 spheres
			Vector Min = hitbox->bbmin, Max = hitbox->bbmax;

			if (hitbox->radius == -1.f)
			{
				const auto center = (Min + Max) * 0.5f;

				positions.emplace_back();
			}
			else
			{
				// Points 0 - 5 = min
				// Points 6 - 11 = max
				Vector P[12];
				for (int j = 0; j < 6; j++) { P[j] = Min; }
				for (int j = 7; j < 12; j++) { P[j] = Max; }

				P[1].x += hitbox_radius;
				P[2].x -= hitbox_radius;
				P[3].y += hitbox_radius;
				P[4].y -= hitbox_radius;
				P[5].z += hitbox_radius;

				P[6].x += hitbox_radius;
				P[7].x -= hitbox_radius;
				P[8].y += hitbox_radius;
				P[9].y -= hitbox_radius;
				P[10].z += hitbox_radius;
				P[11].z -= hitbox_radius;

				for (int j = 0; j < 12; j++)
				{
					MATH::VectorTransform(P[j], entity->GetBoneMatrix(hitbox->bone), P[j]);
					//Points[j] += OriginOffset;
					positions.push_back(P[j]);
				}
			}
		}

		Vector Aimbot::GetHitboxPosition(SDK::CBaseEntity* entity, int hitbox_index)
		{
			const auto hitbox = GetHitbox(entity, hitbox_index);
			if (!hitbox)
				return Vector(0, 0, 0);

			const auto bone_matrix = entity->GetBoneMatrix(hitbox->bone);

			Vector bbmin, bbmax;
			MATH::VectorTransform(hitbox->bbmin, bone_matrix, bbmin);
			MATH::VectorTransform(hitbox->bbmax, bone_matrix, bbmax);

			return (bbmin + bbmax) * 0.5f;
		}

		SDK::mstudiobbox_t* Aimbot::GetHitbox(SDK::CBaseEntity* entity, int hitbox_index)
		{
			if (entity->GetIsDormant() || entity->GetHealth() <= 0)
				return nullptr;

			const auto pModel = entity->GetModel();
			if (!pModel)
				return nullptr;

			auto pStudioHdr = INTERFACES::ModelInfo->GetStudioModel(pModel);
			if (!pStudioHdr)
				return nullptr;

			auto pSet = pStudioHdr->pHitboxSet(0);
			if (!pSet)
				return nullptr;

			if (hitbox_index >= pSet->numhitboxes || hitbox_index < 0)
				return nullptr;

			return pSet->GetHitbox(hitbox_index);
		}
	}

}
*/






























































