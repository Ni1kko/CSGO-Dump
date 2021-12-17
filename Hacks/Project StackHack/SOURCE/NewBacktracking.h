#pragma once
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




#include "SDK/AnimLayer.h"
#include "SDK/CBaseAnimState.h"
#include "FEATURES/Resolver.h"

namespace SDK
{
	class CBaseEntity;
	class NetChannel;
}

namespace FEATURESES
{
	namespace RAGEBOT
	{

#define ENABLE_INTERPOLATION 1
#define DISABLE_INTERPOLATION 0

		static constexpr float MAX_UNLAG = 1.f;
		static constexpr float MAX_BACKTRACK_RANGE = 0.2f;

		struct Backtracking_Records
		{
			Backtracking_Records() {};
			explicit Backtracking_Records(SDK::CBaseEntity*);

			Vector vec_origin;
			Vector eye_angles;
			Vector abs_angles;
			Vector render_angles;
			Vector bbmin;
			Vector bbmax;
			Vector velocity;

			Vector previous_vec_origin;

			float simulation_time;
			float previous_simulation_time;
			float curtime;
			float previous_curtime;
			bool is_exploiting;
			int flags;

			SDK::CBaseEntity* player;
			SDK::CAnimationLayer anim_layers[15];
			SDK::CBaseAnimState animstate;
			matrix3x4_t bone_matrix[128];
			float pose_params[24];


		};

		struct Incoming_Sequence_Record
		{
			Incoming_Sequence_Record(int in_reliable, int out_reliable, int in_sequence, float realtime)
			{
				in_reliable_state = in_reliable;
				out_reliable_state = out_reliable;
				in_sequence_num = in_sequence;

				time = realtime;
			}

			int in_reliable_state;
			int out_reliable_state;
			int in_sequence_num;

			float time;
		};

		class CBacktracking
		{
		public:
			void Store();
			void Restore(SDK::CBaseEntity*, Backtracking_Records, int extrapolate_ticks = 0);
			void ApplyRestore(SDK::CBaseEntity* entity, float curtime);
			void RestoreToCurrentRecord(SDK::CBaseEntity*);

			void UpdateIncomingSequences();
			void AddLatency(SDK::NetChannel* net_channel, float latency);

			std::vector<Backtracking_Records> GetPriorityRecords(SDK::CBaseEntity* entity); // gets the records in order of most usable, lby update, etcs
			std::vector<Backtracking_Records> GetValidRecords(SDK::CBaseEntity* entity);
			std::vector<Backtracking_Records> GetRecords(SDK::CBaseEntity* entity);
			Backtracking_Records* GetCurrentRecord(SDK::CBaseEntity* entity);

			std::vector<Backtracking_Records>& GetShotRecords()
			{
				return shot_at_records;
			}

			void AddShotRecord(Backtracking_Records record)
			{
				shot_at_records.insert(shot_at_records.begin(), record);

				if (shot_at_records.size() > 3)
					shot_at_records.pop_back();
			}

			bool GetExtrapolatedRecord(SDK::CBaseEntity* entity, Backtracking_Records& record);
			bool Extrapolate(SDK::CBaseEntity* entity, Backtracking_Records& record, float time);

			static float GetDeltaTime(Backtracking_Records record);
			static int GetLerpTicks();
			static int GetTickCount(Backtracking_Records);
			static void SetInterpolationFlags(SDK::CBaseEntity* entity, int flag);

		private:
			bool needs_to_restore[64];
			int last_incoming_sequence = 0;
			std::deque<Incoming_Sequence_Record> sequence_records;
			std::vector<Backtracking_Records> player_records[64];
			std::vector<Backtracking_Records> shot_at_records;

			static void InvalidateBoneCache(SDK::CBaseEntity* entity);
		};
		//extern Backtracking_Records bt_rc;
		extern CBacktracking backtrack;
	}
}