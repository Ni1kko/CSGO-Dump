#pragma once

#include "includes.h"
#include "UTILS\vector3D.h"
#include "NewBacktracking.h"
#include "UTILS\vector2D.h"

using namespace std;

namespace SDK
{
	class CUserCmd;
	class CBaseEntity;

	struct mstudiobbox_t;
}

/*namespace FEATURES
{
	namespace RAGEBOT
	{

		class Aimbot
		{
		public:
			void Do(SDK::CUserCmd* cmd);

		private:
			struct PlayerAimbotInfo
			{
				int head_damage = 0, hitscan_damage = 0, tick, extrapolted_ticks;
				Vector head_position, hitscan_position;
				CBacktracking backtrack_record;
			};

		private:
			PlayerAimbotInfo player_aimbot_info[64];

		public:
			PlayerAimbotInfo GetPlayerAimbotInfo(SDK::CBaseEntity* entity)
			{
				return player_aimbot_info[entity->GetIndex()];
			}

		private:
			bool DoHitscan(SDK::CBaseEntity* entity, Vector& final_position, int& final_damage);
			bool DoHeadAim(SDK::CBaseEntity* entity, Vector& final_position, int& final_damage);

			bool IsAccurate(SDK::CBaseEntity* entity, Vector position);

			/// helper functions
			void GetMultipointPositions(SDK::CBaseEntity* entity, std::vector<Vector>& positions, int hitbox_index, float pointscale);
			Vector GetHitboxPosition(SDK::CBaseEntity* entity, int hitbox_index);
			static SDK::mstudiobbox_t* GetHitbox(SDK::CBaseEntity* entity, int hitbox_index);
		};

		extern Aimbot aimbot;

	}
}*/

		
	
