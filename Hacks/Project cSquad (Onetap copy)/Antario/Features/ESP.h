#pragma once
#include "..\Utils\DrawManager.h"
#include "..\Utils\GlobalVars.h"
#include "..\Settings.h"
#include "..\SDK\ClientClass.h"

class ESP
{
public:
	void DrawProjectiles(C_BaseEntity * entity);
	void Render();
	void set_hitmarker_time(float time);
	void DrawHitmarker();
	void SpectatorList();
private:
	void RenderBox(C_BaseEntity* pEnt);
	void RenderHealth(C_BaseEntity * pEnt);
	void RenderHealthBar(C_BaseEntity * pEnt);
	void RenderAmmo(C_BaseEntity * pEnt);
	void RenderName(C_BaseEntity* pEnt, int iterator);
    void RenderWeaponName(C_BaseEntity* pEnt);
	void DroppedWeapons();
	void DrawSkeleton(C_BaseEntity * pEntity);

	void DroppedWeapons(C_BaseEntity * entity);

    int localTeam{};
    Color teamColor{ 150, 200, 60, 255 };
    Color enemyColor{ 255, 255, 255, 255 };
};
extern ESP g_ESP;