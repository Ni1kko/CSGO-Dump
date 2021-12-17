#pragma once
#include "../SDK/GameEvents.h"
#include "../SDK/CBaseEntity.h"
#include "../Include/GlobalIncludes.h"
#include "../SDK/GlobalDefenitions.h"
#include "../SDK/Cbeam.h"

#define EVENT_HOOK( x )

class CGameEvents : public IGameEventListener
{
	public:
		void InitializeEventListeners();
		void RemoveListeners();
		void FireGameEvent( IGameEvent* game_event );
		void Register();
		int GetEventDebugID();
		std::vector<IGameEventListener*> listeners;
};

extern CGameEvents g_pGameEvents;

class CMyBulletTracer
{
	public:
		void CollectTracers( IGameEvent* pEvent );
		void DrawTracers(Vector src, Vector end, CColor color);
		void ClearTracers();
	private:
		class CMyBulletTracer_info
		{
			public:
				CMyBulletTracer_info(Vector starts, Vector positions, float times)
				{
					this->start = starts;
					this->position = positions;
					this->time = times;
				}

				Vector position;
				Vector start;
				float time;
		};

		std::vector<CMyBulletTracer_info> BulletTracerData;
};

class CSoundEsp {
public:
	void CollectSteps(IGameEvent* pEvent);
	void DrawSteps();
	void ClearSteps();
	void AddTrails();
	void DrawBeamPaw(Vector src, Vector end, CColor color);
	void AddStepManually(int ent, Vector origin);
private:
	class CSoundEsp_info {
	public:
		CSoundEsp_info(Vector origin, float time, CColor color) {
			this->origin = origin;
			this->time = time;
			this->color = color;
		}

		Vector origin;
		float time;
		CColor color;
		Beam_t* beamInfo;
	};

	std::vector<CSoundEsp_info> StepsData;
};

extern CSoundEsp* SoundEsp;
extern CMyBulletTracer* BulletTracer;
