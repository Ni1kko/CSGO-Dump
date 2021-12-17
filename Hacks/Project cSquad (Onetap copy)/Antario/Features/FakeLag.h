#pragma once
#include "algorithm"
#include "..\SDK\CInput.h"
#include "..\SDK\CEntity.h"
#include "..\Utils\GlobalVars.h"
#include "..\SDK\ClientClass.h"
#include "..\SDK\UtlVector.h"
#include "..\Settings.h"
#include "..\Globals.h"
#include "Aimbot.h"

static constexpr int MAX_CHOKE = 14;
static constexpr float TELEPORT_DISTANCE = 64.f;

class CFakeLag
{
public:
	int Fakelag_AdaptiveFactor();
	void do_fakelag(CUserCmd * cmd, C_BaseEntity * local);
	void fakeduck(CUserCmd * cmd);
	void fake_crouch(CUserCmd * cmd);
	void fakeduck2(CUserCmd * cmd, C_BaseEntity * local);
	void on_peek(bool canhit);
};

extern CFakeLag* fakelag;