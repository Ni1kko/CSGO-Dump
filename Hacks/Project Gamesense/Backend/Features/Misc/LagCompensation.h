#pragma once
#include "../../SDK/IEngine.h"
#include "../../SDK/CBaseEntity.h"
#include <deque>

#define TIME_TO_TICKS( dt )		( (int)( 0.5 + (float)(dt) / Interfaces::Globals->interval_per_tick ) )
#define TICKS_TO_TIME( t )		(	Interfaces::Globals->interval_per_tick *( t ) )

namespace Features
{
	struct TickInfo
	{
		float SimTime = -1.f;
		Vector Head;
		bool Shot;
		float Velocity;
		matrix3x4_t Matrix[128];
		float Pitch;
	};

	class CLagCompensation
	{
		public:
			void CreateRecord(CBaseEntity* Entity);
			void FrameStage(ClientFrameStage_t stage);
			matrix3x4_t pMatrix[65][128];
			std::deque<TickInfo> PlayerRecord[65] = {};
			float ShotTime[65];
			int ShotTick[65];
			int Sequence[65];
			void ClearRecords(int i);
			float LerpTime();
			bool ValidTick(float simtime);

			template<class T, class U>
			T clamp(T in, U low, U high);
		private:
	};

	extern CLagCompensation* LagCompensation;
}