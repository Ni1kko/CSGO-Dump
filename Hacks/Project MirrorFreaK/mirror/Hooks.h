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
	void DrawBeamd(Vector src, Vector end, Color color);
	void UndoHooks();


	extern Utilities::Memory::VMTManager VMTPanel; 
	extern Utilities::Memory::VMTManager VMTClient; 
	extern Utilities::Memory::VMTManager VMTClientMode;
	extern Utilities::Memory::VMTManager VMTModelRender;
	extern Utilities::Memory::VMTManager VMTPrediction; 
	extern Utilities::Memory::VMTManager VMTRenderView;
	extern Utilities::Memory::VMTManager VMTEventManager;
	extern Utilities::Memory::VMTManager VMTCreateMove;
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
