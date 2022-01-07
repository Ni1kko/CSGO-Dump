#pragma once

extern float last_real;
extern float lineLBY;
extern float lineRealAngle;
extern float lineFakeAngle;
extern float lspeed;
extern float pitchmeme;
extern float lby2;
extern bool DoUnload;
extern bool bGlovesNeedUpdate;
extern int missedLogHits[65];

extern bool round_change;
#include "Utilities.h"
#include "Interfaces.h"
namespace Hooks
{
	void Initialise();

	void UndoHooks();


	extern Utilities::Memory::VMTManager VMTPanel; 
	extern Utilities::Memory::VMTManager VMTClient; 
	extern Utilities::Memory::VMTManager VMTClientMode;
	extern Utilities::Memory::VMTManager VMTModelRender;
	extern Utilities::Memory::VMTManager VMTPrediction; 
	extern Utilities::Memory::VMTManager VMTRenderView;
	extern Utilities::Memory::VMTManager VMTEventManager;
	extern Utilities::Memory::VMTManager VMTCreateMove;
	extern RecvVarProxyFn g_fnSequenceProxyFn;
	void SetViewModelSequence(const CRecvProxyData *pDataConst, void *pStruct, void *pOut);
};

namespace Resolver
{
	extern bool didhitHS;
	extern bool hitbaim;
	extern bool lbyUpdate;
}

class lag_fix
{
public:
	void lag_fix_main(IClientEntity* entity);
};

namespace bigboi
{
	extern bool freestand;
	extern int indicator;
	extern int freestandval;
	extern float current_yaw;
}
extern bool warmup;

class AnimatedClanTag
{
public:
//	void Tick();
};

extern AnimatedClanTag *animatedClanTag;

class beam
{
	struct BulletImpact_t
	{
		float			flImpactTime;
		Vector			vecImpactPos;
		Color			color;
		IClientEntity*	pPlayer;

		__forceinline BulletImpact_t()
		{
			vecImpactPos = { 0.0f, 0.0f, 0.0f };
			flImpactTime = 0.0f;
			color = (255, 255, 255);
			pPlayer = nullptr;
		}

		__forceinline BulletImpact_t(IClientEntity* player, Vector pos, float time, Color col = (255, 255, 255))
		{
			pPlayer = player;
			flImpactTime = time;
			vecImpactPos = pos;
			color = col;
		}
	};
	std::vector<std::pair<int, float>>		Entities;
	std::deque<BulletImpact_t>		Impacts;
public:

	void DrawBulletBeams();
	
	bool glow = false;

	float real = 0.f;

	Vector cham_origin;
	Vector cham_angle;
};
extern beam *c_beam;

