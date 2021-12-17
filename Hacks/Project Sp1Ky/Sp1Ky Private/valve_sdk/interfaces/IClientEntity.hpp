#pragma once 
#include "IClientNetworkable.hpp"
#include "IClientRenderable.hpp"
#include "IClientUnknown.hpp"
#include "IClientThinkable.hpp"
struct SpatializationInfo_t;
class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
	virtual void Release(void) = 0;
};
#pragma pack(push, 1)
class WeapInfo_t
{
public:

	char pad00[0xC8];
	__int32 weapon_type;
	char padCC[0x20];
	__int32 m_iDamage;
	float m_fArmorRatio;
	char padF4[0x4];
	float m_fPenetration;
	char padFC[0x8];
	float m_fRange;
	float m_fRangeModifier;
	char pad10C[0x10];
	bool m_bHasSilencer;
};
class CCSWeaponInfo	
{
public:
	char _0x0000[20];
	__int32 max_clip;            //0x0014 
	char _0x0018[12];
	__int32 max_reserved_ammo;    //0x0024 
	char _0x0028[96];
	char* hud_name;                //0x0088 
	char* weapon_name;            //0x008C 
	char _0x0090[60];
	__int32 type;                //0x00CC 
	__int32 price;                //0x00D0 
	__int32 reward;                //0x00D4 
	char _0x00D8[20];
	BYTE full_auto;                //0x00EC 
	char _0x00ED[3];
	__int32 damage;                //0x00F0 
	float armor_ratio;            //0x00F4 
	__int32 bullets;            //0x00F8 
	float penetration;            //0x00FC 
	char _0x0100[8];
	float range;                //0x0108 
	float range_modifier;        //0x010C 
	char _0x0110[16];
	BYTE silencer;                //0x0120 
	char _0x0121[15];
	float max_speed;            //0x0130 
	float max_speed_alt;        //0x0134 
	char _0x0138[76];
	__int32 recoil_seed;        //0x0184 
	char _0x0188[32];
};
#pragma pack(pop)
class IWeaponSystem
{
	virtual void unused0() = 0;
	virtual void unused1() = 0;
public:
	virtual const CCSWeaponInfo* GetWpnData(unsigned ItemDefinitionIndex) = 0;
	virtual const Vector &GetAbsOrigin(void) const = 0;
	virtual const QAngle &GetAbsAngles(void) const = 0;
};