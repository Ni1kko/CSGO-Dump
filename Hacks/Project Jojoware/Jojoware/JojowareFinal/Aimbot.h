#pragma once
#include "Includes.h"
class Vector
{
public:
	float x, y, z;
};

class Aimbot
{
private:
	
	void SilentWriteMemory(Vector Target); //removed cause you should not paste
	void BackTrackEx(int ticks); //removed cause pasting is bad af
	Vector FindSmoothAngels(Vector Target);
	Vector calangle(Vector src, Vector dir, float xp, float yp);
	int FindClosestEnemy();
	void ReadMemory(int bone);
	int GetWeaponInHand();
	void WriteMemory();
	Vector Viewangles;
	int Enemyteam;
	DWORD Clientstate;
	int Enemyhealth;
	Vector HeadBonesEnemy;
	Vector MyLoc;
	Vector MyEyeLoc;
public:
	void Run();
}; extern Aimbot Am;

#define IS_GRENADE 1
#define IS_RIFLE 2
#define IS_SNIPER 3
#define IS_PISTOL 4
#define IS_SHOTGUN 6
#define IS_SMG 5
#define IS_UNKNOWN 7
#define WEAPON_AK47 7
#define WEAPON_M4A4 16
#define WEAPON_M4A1S  60
#define WEAPON_P2000 32
#define WEAPON_FIVE7 3
#define WEAPON_DEAGLE 1
#define WEAPON_USPS 61
#define WEAPON_P250 36
#define WEAPON_DUALS 2
#define WEAPON_GLOCK 4
#define WEAPON_TEC9 30
#define WEAPON_SSG08 40
#define WEAPON_MP9 34
#define WEAPON_MP7 33
#define WEAPON_NOVA 35
#define WEAPON_XM 25
#define WEAPON_SAWEDOFF 29
#define WEAPON_MAG7 27
#define WEAPON_GALIL 13
#define WEAPON_NEGEV 28
#define WEAPON_P90 19
#define WEAPON_MAC10 17
#define WEAPON_UMP 24
#define WEAPON_AWP 9
#define WEAPON_FAMAS 10
#define WEAPON_CZ75 63
#define WEAPON_DECOY 47
#define WEAPON_SMOKE 45
#define WEAPON_HEGRENADE 44
#define WEAPON_MOLOTOV 46
#define WEAPON_INCENDARY 48
#define WEAPON_FLASHBANG 43