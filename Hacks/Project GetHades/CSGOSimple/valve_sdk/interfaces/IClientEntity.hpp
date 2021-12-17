#pragma once

#include "IClientNetworkable.hpp"
#include "IClientRenderable.hpp"
#include "IClientUnknown.hpp"
#include "IClientThinkable.hpp"

struct SpatializationInfo_t;

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
    virtual void Release ( void ) = 0;
    virtual const Vector& get_abs_origin ( void ) const = 0;
    virtual const QAngle& get_abs_angles ( void ) const = 0;
};

#pragma pack(push, 1)
class CCSWeaponInfo //xSeeker
{
public:
    int8_t pad0[20];
    int32_t max_clip1;
    int8_t pad1[12];
    int32_t max_reserved_ammo;
    int8_t pad2[96];
    char* hud_name;
    char* weapon_name;
    int8_t pad3[56];
    int32_t weapon_type;
    int8_t pad4[4];
    int32_t weapon_price;
    int32_t kill_award;
    int8_t pad5[20];
    uint8_t full_auto;
    int8_t pad6[3];
    int32_t damage;
    float_t armor_ratio;
    int32_t bullets;
    float_t penetration;
    int8_t pad7[8];
    float_t range;
    float_t range_modifier;
    int8_t pad8[16];
    uint8_t has_silencer;
    int8_t pad9[15];
    float_t spread;
    float_t spread_alt;
    int8_t pad10[76];
    int32_t recoil_seed;
    int8_t pad11[32];
};
#pragma pack(pop)

class IWeaponSystem
{
    virtual void unused0 ( ) = 0;
    virtual void unused1 ( ) = 0;
public:
    virtual const CCSWeaponInfo* GetWpnData ( unsigned ItemDefinitionIndex ) = 0;
};
