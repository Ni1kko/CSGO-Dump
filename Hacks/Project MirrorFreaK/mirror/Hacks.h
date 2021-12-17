#pragma once

#pragma once

#include "CommonIncludes.h"
#include "SDK.h"
#include "Menu.h"
#include <vector>
namespace GlobalBREAK
{
	extern bool bVisualAimbotting;
	extern QAngle vecVisualAimbotAngs;
	extern int ChokeAmount;
	extern float flFakewalked;
	extern bool NewRound;
	extern bool bRainbowCross;
	extern QAngle fakeangleslocal;
	extern bool dohitmarker;
	extern bool WeaponFire;
	extern float LastTimeWeFired;
	extern int ShotsFiredLocally;
	extern char* breakmode;
	extern float hitchance;
	extern int ShotsHitPerEntity[65];
	extern bool HeadShottedEntity[65];
	extern float curFov;
	extern int breakangle;
	extern bool bUsingFakeAngles[65];
	extern float HitMarkerAlpha;
	extern int TicksOnGround;
	extern int ticks_while_unducked;
	extern int AnimationPitchFix;
	extern int NextPredictedLBYUpdate;
	extern int prevChoked;
	extern bool AAFlip;
	extern bool LEFT;
	extern bool BACK;
	extern bool RIGHT;
	extern char my_documents_folder[MAX_PATH];
	extern float smt;
	extern QAngle visualAngles;
	extern bool	bSendPacket;
	extern bool bAimbotting;
	extern CUserCmd* userCMD;
	extern char* szLastFunction;
	extern HMODULE hmDll;
	extern bool bFakewalking;
	extern Vector vecUnpredictedVel;
	extern float flFakeLatencyAmount;
	extern float flEstFakeLatencyOnServer;
	extern matrix3x4_t traceHitboxbones[128];
	extern std::array<std::string, 64> resolverModes;
}
namespace Hacks
{
	void SetupHacks();
	void DrawHacks();
	void MoveHacks(CUserCmd *pCmd, bool &bSendPacket);
}

class CHack
{
public:
	virtual void Draw() = 0;
	virtual void Move(CUserCmd *pCmd, bool &bSendPacket) = 0;
	virtual void Init() = 0;
};

class HackManager
{
public:
	void RegisterHack(CHack* hake);
	void Ready();

	void Draw();
	void Move(CUserCmd *pCmd,bool &bSendPacket);
	IClientEntity *pLocal();
private:
	std::vector<CHack*> Hacks;
	IClientEntity *pLocalInstance;
	bool IsReady;
};

extern HackManager hackManager;