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
#include "SDK\Collideable.h"
#include "FEATURES/Backtracking.h"

#include "NewBacktracking.h"

#include "SDK/AnimLayer.h"
#include "SDK/CBaseAnimState.h"
#include "FEATURES/Resolver.h"



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
namespace FEATURESES
{
	namespace RAGEBOT
	{

		CBacktracking backtracking;

		Backtracking_Records::Backtracking_Records(SDK::CBaseEntity* ent)
		{
			vec_origin = ent->GetVecOrigin();
			abs_angles = ent->GetAbsAngles();
			eye_angles = ent->GetEyeAngles();
			render_angles = ent->GetRenderAngles();
			velocity = ent->GetVelocity();
			simulation_time = ent->GetSimTime();
			flags = ent->GetFlags();
			animstate = *ent->GetAnimState();
			curtime = INTERFACES::Globals->curtime;
			is_exploiting = false;

			for (int i = 0; i < 15; i++)
				anim_layers[i] = ent->GetAnimOverlay(i);
			for (int i = 0; i < 128; i++)
				bone_matrix[i] = ent->GetBoneMatrix(i);

			memcpy(pose_params, ent->GetPoseParamaters(), 96);

			
			auto collideable = ent->GetCollideable();

			if (!collideable)
				return;
		//	auto collideable = ent->GetCollideable();

			bbmin = collideable->Mins();
			bbmax = collideable->Maxs();

			player = ent;
		}

		Backtracking_Records* CBacktracking::GetCurrentRecord(SDK::CBaseEntity* entity)
		{
			if (!player_records[entity->GetIndex()].size())
				return nullptr;

			return &player_records[entity->GetIndex()][0];
		}

		void CBacktracking::UpdateIncomingSequences()
		{
			auto nci = INTERFACES::Engine->GetNetChannel();
			if (!nci)
				return;

			if (nci->m_nInSequenceNr > last_incoming_sequence || !nci->m_nInSequenceNr)
			{
				last_incoming_sequence = nci->m_nInSequenceNr;

				sequence_records.push_front(Incoming_Sequence_Record(nci->m_nInReliableState, nci->m_nOutReliableState, nci->m_nInSequenceNr, UTILS::GetCurtime()));
			}

			if (sequence_records.size() > 2048)
				sequence_records.pop_back();
		}

		void CBacktracking::AddLatency(SDK::NetChannel* net_channel, float latency)
		{
			for (const auto& sequence : sequence_records)
			{
				if (UTILS::GetCurtime() - sequence.time >= latency)
				{
					net_channel->m_nInReliableState = sequence.in_reliable_state;
					net_channel->m_nInSequenceNr = sequence.in_sequence_num;
					break;
				}
			}
		}

		void CBacktracking::Store()
		{
			auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
			if (!local_player || local_player->GetHealth() <= 0)
				return;

			for (int i = 0; i < 64; i++)
			{
				auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
				if (!entity || entity->GetHealth() <= 0 || entity->GetTeam() == local_player->GetTeam())
				{
					player_records[i].clear();
					continue;
				}

				if (entity->GetIsDormant() || entity->GetImmunity())
					continue;

				//SetInterpolationFlags(entity, DISABLE_INTERPOLATION);
				entity->UpdateClientSideAnimation();

				if (!player_records[i].size())
				{
					player_records[i].insert(player_records[i].begin(), Backtracking_Records(entity));
					continue;
				}

				// only add if simtime updates, so when player sends data
				if (player_records[i][0].simulation_time != entity->GetSimTime())
				{
					auto record = Backtracking_Records(entity);

					record.previous_vec_origin = player_records[i][0].vec_origin;
					record.previous_simulation_time = player_records[i][0].simulation_time;
					record.previous_curtime = player_records[i][0].curtime;

					player_records[i].insert(player_records[i].begin(), record);
				}

				// if more than 1 second worth of records pop back the last record
				if (INTERFACES::Globals->curtime - player_records[i].back().curtime > MAX_UNLAG)
					player_records[i].pop_back();
			}
		}

		void CBacktracking::Restore(SDK::CBaseEntity* entity, Backtracking_Records rec, int extrapolate_ticks)
		{
			needs_to_restore[entity->GetIndex()] = true;

			auto extrapolation_amount = Vector(0, 0, 0);
			if (extrapolate_ticks != 0)
			{
				const auto acceleration = Vector(0, 0, 0);
				const float delta_time = (rec.curtime - rec.previous_curtime);
				auto velocity = (rec.vec_origin - rec.previous_vec_origin) / (delta_time > 0.f ? delta_time : 1.f);

				while (extrapolate_ticks > 0)
				{
					extrapolation_amount += velocity * INTERFACES::Globals->interval_per_tick;
					velocity += acceleration * INTERFACES::Globals->interval_per_tick;

					extrapolate_ticks--;
				}
				while (extrapolate_ticks < 0)
				{
					extrapolation_amount += velocity * INTERFACES::Globals->interval_per_tick;
					velocity += acceleration * INTERFACES::Globals->interval_per_tick;

					extrapolate_ticks++;
				}
			}

			memcpy(entity->GetPoseParamaters(), rec.pose_params, 96);
			memcpy(entity->GetAnimState(), &rec.animstate, sizeof(SDK::CBaseAnimState));

			for (int i = 0; i < 15; i++)
				entity->SetAnimOverlay(i, rec.anim_layers[i]);

			auto collideable = entity->GetCollideable();
			if (!collideable)
				return;

			//*reinterpret_cast<Vector*>(uintptr_t(entity) + OFFSETS::m_angRotation) = Vector(0, 0, 0);
			//            DT_BaseAnimating m_bClientSideAnimation -------------------------------------------------------------------------- 0x288c
			//			  DT_BaseAnimating m_bClientSideFrameReset

			entity->SetEyeAngles(rec.eye_angles);
			entity->SetAbsAngles(Vector(0, rec.abs_angles.y, 0));
			entity->SetRenderAngles(rec.render_angles);
			entity->SetAbsOrigin(rec.vec_origin + extrapolation_amount);
			entity->SetVelocity(rec.velocity);
			entity->SetFlags(rec.flags);

			collideable->Mins() = rec.bbmin;
			collideable->Maxs() = rec.bbmax;
		}

		void CBacktracking::ApplyRestore(SDK::CBaseEntity* entity, float curtime)
		{
			/// pvs fix
			*reinterpret_cast<int*>(uintptr_t(entity) + 0xA30) = INTERFACES::Globals->framecount;
			*reinterpret_cast<int*>(uintptr_t(entity) + 0xA28) = 0;

			InvalidateBoneCache(entity);

			entity->UpdateClientSideAnimation();
			entity->SetupBones(NULL, 128, BONE_USED_BY_ANYTHING, curtime);
		}

		void CBacktracking::RestoreToCurrentRecord(SDK::CBaseEntity* entity)
		{
			if (!player_records[entity->GetIndex()].size() || !needs_to_restore[entity->GetIndex()])
				return;

			auto record = player_records[entity->GetIndex()][0];

			Restore(entity, record);
			ApplyRestore(entity, INTERFACES::Globals->curtime);

			needs_to_restore[entity->GetIndex()] = false;
		}

		int CBacktracking::GetTickCount(Backtracking_Records record)
		{
			int ideal_tick = TIME_TO_TICKS(record.simulation_time) + GetLerpTicks();

			return ideal_tick;
		}

		void CBacktracking::SetInterpolationFlags(SDK::CBaseEntity * entity, int flag)
		{
			auto VarMap = reinterpret_cast<uintptr_t>(entity) + 36; // tf2 = 20
			auto VarMapSize = *reinterpret_cast<int*>(VarMap + 20);

			for (auto index = 0; index < VarMapSize; index++)
				*reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(VarMap) + index * 12) = flag;
		}

		std::vector<Backtracking_Records> CBacktracking::GetPriorityRecords(SDK::CBaseEntity* entity)
		{
			std::vector<Backtracking_Records> priority_records;
			const int player_index = entity->GetIndex();

			auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
			if (!local_player || local_player->GetHealth() <= 0 || player_records[player_index].size() <= 0)
				return priority_records;

			/// try to find all the records where they're resolved		
			std::vector<Backtracking_Records> resolved_records;
			for (const auto& record : player_records[player_index])
			{
				if (GetDeltaTime(record) >= MAX_BACKTRACK_RANGE && !record.is_exploiting)
					continue;

				/*if (FEATURES::RAGEBOT::resolver.IsResolved(record.resolve_record.resolve_type))
					resolved_records.push_back(record);*/
			}

			/// no resolved records found :(
			if (!resolved_records.size())
			{
				for (const auto& record : player_records[player_index])
				{
					if (GetDeltaTime(record) >= MAX_BACKTRACK_RANGE)
						continue;

					if (resolver->AntiPener(entity, entity->EasyEyeAngles()->yaw, 10.f))
					{
						priority_records.push_back(record);
						return priority_records;
					}
				}

				for (const auto& record : player_records[player_index])
				{
					if (GetDeltaTime(record) >= MAX_BACKTRACK_RANGE)
						continue;

					priority_records.push_back(record);
					break;
				}

				return priority_records;
			}

			/// sort through the resolved records (the could be many if they're moving) and get 2 good ones

			/// not enough records to need sorting
			if (resolved_records.size() <= 2)
				return resolved_records;

			const auto left_yaw = UTILS::CalcAngle(local_player->GetVecOrigin(), entity->GetVecOrigin()).y + 90.f,
				right_yaw = UTILS::CalcAngle(local_player->GetVecOrigin(), entity->GetVecOrigin()).y - 90.f;

			/// try to find one where they're yaw is sideways to us (easier to hit fam)
			Backtracking_Records sideways_rec;
			float lowest_sideways_delta = 180.f;
			for (const auto& record : resolved_records)
			{
				const float current_delta = Min<float>(fabs(MATH::NormalizeYaw(record.eye_angles.y - left_yaw)), fabs(MATH::NormalizeYaw(record.eye_angles.y - right_yaw)));
				if (current_delta < lowest_sideways_delta)
				{
					lowest_sideways_delta = current_delta;
					sideways_rec = record;
				}
			}

			/// find the farthest away record from the sideways_rec (for diversity, since the sideways_rec could be behind a wall or smthn)
			Backtracking_Records opposite_rec;
			float highest_delta = 0.f;
			for (const auto& record : resolved_records)
			{
				const float current_delta = fabs(MATH::NormalizeYaw(record.eye_angles.y - sideways_rec.eye_angles.y));
				if (current_delta > highest_delta)
				{
					highest_delta = current_delta;
					opposite_rec = record;
				}
			}

			if (highest_delta > 45.f)
			{
				priority_records.push_back(opposite_rec);
				priority_records.push_back(sideways_rec);
			}
			else /// return the first record and the slowest record
			{
				Backtracking_Records slowest_record = resolved_records.back();
				float slowest_speed = slowest_record.velocity.Length2D();
				for (const auto& record : resolved_records)
				{
					const float speed = record.velocity.Length2D();
					if (speed < slowest_speed)
					{
						slowest_speed = speed;
						slowest_record = record;
					}
				}

				if (fabs(slowest_speed - resolved_records.back().velocity.Length2D()) < 30.f)
					slowest_record = resolved_records.back();

				priority_records.push_back(resolved_records.front());
				priority_records.push_back(slowest_record);
			}

			return priority_records;
		}

		std::vector<Backtracking_Records> CBacktracking::GetValidRecords(SDK::CBaseEntity* entity)
		{
			std::vector<Backtracking_Records> valid_records;

			for (const auto& record : player_records[entity->GetIndex()])
			{
				if (GetDeltaTime(record) < MAX_BACKTRACK_RANGE)
					valid_records.push_back(record);
			}

			return valid_records;
		}

		std::vector<Backtracking_Records> CBacktracking::GetRecords(SDK::CBaseEntity* entity)
		{
			return player_records[entity->GetIndex()];
		}

		bool CBacktracking::GetExtrapolatedRecord(SDK::CBaseEntity* entity, Backtracking_Records& record)
		{
			const int player_index = entity->GetIndex();

			if (!player_records[player_index].size())
				return false;

			record = player_records[player_index][0];

			float time_to_extrapolate_to;
			if (GetDeltaTime(record) > MAX_BACKTRACK_RANGE)
				time_to_extrapolate_to = INTERFACES::Globals->curtime;
			else /// breaking lag compensation
				return false;

			if (!Extrapolate(entity, record, time_to_extrapolate_to))
				return false;

			return true;
		}

		bool CBacktracking::Extrapolate(SDK::CBaseEntity* entity, Backtracking_Records& record, float time)
		{
			const int player_index = entity->GetIndex();
			const float time_delta = time - record.curtime;

			static auto sv_gravity = INTERFACES::cvar->FindVar("sv_gravity");
			static auto sv_jump_impulse = INTERFACES::cvar->FindVar("sv_jump_impulse");

			/// need 3 records to extrapolate
			if (player_records[player_index].size() < 3)
				return false;

			/// to check if ground is below the nigger, and speed is per second
			auto IsObjectInWay = [](Vector origin, Vector velocity, Vector& end) -> bool
			{
				SDK::trace_t trace;
				SDK::CTraceWorldOnly filter;
				SDK::Ray_t ray;
				ray.Init(origin, origin + (velocity * INTERFACES::Globals->interval_per_tick));

				INTERFACES::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);

				end = trace.end;

				return trace.flFraction < 1.f;
			};

			auto record_1 = player_records[player_index][0], record_2 = player_records[player_index][1], record_3 = player_records[player_index][2];
			record = record_1;

			/// velocity and acceleration are per second, not per tick
			Vector velocity = record.velocity;
			Vector acceleration = ((record_1.velocity - record_2.velocity) + (record_2.velocity - record_3.velocity)) / (record_1.simulation_time - record_2.simulation_time);
			acceleration.z = -sv_gravity->GetFloat();

			bool was_object_in_way_last_tick = false;
			float curtime = record.simulation_time;
			while (curtime < time)
			{
				Vector vel_change = velocity * INTERFACES::Globals->interval_per_tick;

				record.vec_origin += vel_change;
				record.bbmax += vel_change;
				record.bbmin += vel_change;

				velocity += acceleration * INTERFACES::Globals->interval_per_tick;

				Vector end;
				if (IsObjectInWay(record.vec_origin, velocity, end))
				{
					record.vec_origin = end;

					if (!was_object_in_way_last_tick)
						velocity.z = sv_jump_impulse->GetFloat();
					else
						break;

					was_object_in_way_last_tick = true;
				}
				else
					was_object_in_way_last_tick = false;

				curtime += INTERFACES::Globals->interval_per_tick;
			}

			return true;
		}

		void CBacktracking::InvalidateBoneCache(SDK::CBaseEntity* entity)
		{
			static uintptr_t InvalidateBoneCache = (DWORD)UTILS::FindSignature("client_panorama.dll", "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81");
			static uintptr_t g_iModelBoneCounter = **(uintptr_t**)(InvalidateBoneCache + 10); //	Offsets::InvalidateBoneCache = FindPatternIDA("client.dll", "80 3D ? ? ? ? 00 74 16 A1");
			*(int*)((uintptr_t)entity + OFFSETS::m_nForceBone + 0x20) = 0; //m_nForceBone + 0x20
			*(uintptr_t*)((uintptr_t)entity + 0x2914) = 0xFF7FFFFF; // m_flLastBoneSetupTime = -FLT_MAX;
			*(uintptr_t*)((uintptr_t)entity + 0x2680) = (g_iModelBoneCounter - 1); // m_iMostRecentModelBoneCounter = g_iModelBoneCounter - 1;
		}

		float CBacktracking::GetDeltaTime(Backtracking_Records record)
		{
			auto nci = INTERFACES::Engine->GetNetChannelInfo();
			if (!nci)
				return FLT_MAX;

			float correct = 0.f;
			correct += nci->GetLatency(FLOW_OUTGOING);
			correct += nci->GetLatency(FLOW_INCOMING);
			correct += TICKS_TO_TIME(GetLerpTicks());
			correct = Clamp<float>(correct, 0.f, 1.f);

			return fabs(correct - (UTILS::GetCurtime() - record.simulation_time));
		}

		int CBacktracking::GetLerpTicks()
		{
			static const auto cl_interp_ratio = INTERFACES::cvar->FindVar("cl_interp_ratio");
			static const auto cl_updaterate = INTERFACES::cvar->FindVar("cl_updaterate");
			static const auto cl_interp = INTERFACES::cvar->FindVar("cl_interp");

			return TIME_TO_TICKS(max(cl_interp->GetFloat(), cl_interp_ratio->GetFloat() / cl_updaterate->GetFloat()));
		}
	}
}




























































