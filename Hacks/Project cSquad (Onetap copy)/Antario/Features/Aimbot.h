#pragma once
#include "..\SDK\CInput.h"
#include "..\SDK\CEntity.h"
#include "..\Utils\GlobalVars.h"
#include "..\SDK\ClientClass.h"
#include "..\SDK\ConVar.h"
#include "..\Settings.h"
#include "..\SDK\CTrace.h"
#include "AutoWall.h"
#include "..\SDK\Materials.h"
#include "..\Globals.h"
#include "FakeLag.h"
#include "..\Singleton.h"
enum HitboxList
{
	Head = 0,
	Neck,
	Pelvis,
	Stomach,
	LowerChest,
	Chest,
	UpperChest,
	RightThigh,
	LeftThigh,
	RightShin,
	LeftShin,
	RightFoot,
	LeftFoot,
	RightHand,
	LeftHand,
	RightUpperArm,
	RightLowerArm,
	LeftUpperArm,
	LeftLowerArm
};

class RageBot {
public:
	void PVSFix(ClientFrameStage_t stage);
	void minspeed(C_BaseEntity* local, CUserCmd* cmd);
	void DoResolver(ClientFrameStage_t stage);
	void DisableInterpolation(ClientFrameStage_t stage);
	bool CanShoot();
	bool CanShootAntiaim(CUserCmd* cmd);
	mstudiobbox_t* GetHitbox(C_BaseEntity* entity, int hitbox_index);
	Vector GetHitboxPosition(C_BaseEntity* pEntity, int Hitbox);
	void OnCreateMove();

	matrix3x4_t bonematrix[64][128];
};

extern RageBot* aimbot;

