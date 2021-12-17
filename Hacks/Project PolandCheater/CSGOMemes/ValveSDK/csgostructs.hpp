#pragma once

#include "interfaces.hpp"

#define NETVAR(type, name, table, netvar)                           \
    type& name##() const {                                          \
        static int _##name = netvars.find(table, netvar);     \
        return *(type*)((uintptr_t)this + _##name);                 \
    }

#define PNETVAR(type, name, table, netvar)                           \
    type* name##() const {                                          \
        static int _##name = netvars.find(table, netvar);     \
        return (type*)((uintptr_t)this + _##name);                 \
    }

#define NETPROP(funcname, tableName, propName) static RecvProp* funcname() \
{ \
	static auto pProp = netvars.get_prop(tableName, propName); \
	return pProp; \
}

enum CSWeaponType
{
    WEAPONTYPE_KNIFE = 0,
    WEAPONTYPE_PISTOL,
    WEAPONTYPE_SUBMACHINEGUN,
    WEAPONTYPE_RIFLE,
    WEAPONTYPE_SHOTGUN,
    WEAPONTYPE_SNIPER_RIFLE,
    WEAPONTYPE_MACHINEGUN,
    WEAPONTYPE_C4,
    WEAPONTYPE_GRENADE,
    WEAPONTYPE_UNKNOWN
};

class C_BaseEntity;

// Created with ReClass.NET by KN4CK3R
class CHudTexture
{
public:
    char szShortName[64];    //0x0000
    char szTextureFile[64];  //0x0040
    bool bRenderUsingFont;   //0x0080
    bool bPrecached;         //0x0081
    int8_t cCharacterInFont; //0x0082
    uint8_t pad_0083[1];     //0x0083
    uint32_t hFont;          //0x0084
    int32_t iTextureId;      //0x0088
    float afTexCoords[4];    //0x008C
    uint8_t pad_009C[16];    //0x009C
};

class CCStrike15ItemDefinition;
class CCSWeaponInfo
{
public:

    virtual ~CCSWeaponInfo() {};
    /*Parse(KeyValues *, char const*)
    RefreshDynamicParameters(void)
    GetPrimaryClipSize(C_EconItemView const*, int, float)const
    GetSecondaryClipSize(C_EconItemView const*, int, float)const
    GetDefaultPrimaryClipSize(C_EconItemView const*, int, float)const
    GetDefaultSecondaryClipSize(C_EconItemView const*, int, float)const
    GetPrimaryReserveAmmoMax(C_EconItemView const*, int, float)const
    GetSecondaryReserveAmmoMax(C_EconItemView const*, int, float)const*/

    char* m_szWeaponName; //0x0004 
    char pad_0x0008[0x8]; //0x0008
    CCStrike15ItemDefinition* m_pItemDefinition; //0x0010 
    int m_iMaxClip1; //0x0014 
    char pad_0x0018[0xC]; //0x0018
    int m_iMaxAmmo; //0x0024 
    char pad_0x0028[0x4]; //0x0028
    char* m_szWorldModel; //0x002C 
    char* m_szViewModel; //0x0030 
    char* m_szDroppedModel; //0x0034 
    char pad_0x0038[0x4]; //0x0038
    char* N0000054E; //0x003C 
    char pad_0x0040[0x38]; //0x0040
    char* N0000055D; //0x0078 
    char pad_0x007C[0x4]; //0x007C
    char* m_szAmmoType; //0x0080 
    char pad_0x0084[0x4]; //0x0084
    char* m_szLocalizedName; //0x0088 
    char pad_0x008C[0x3C]; //0x008C
    int m_eWeaponType; //0x00C8 
    int m_iPrice; //0x00CC 
    char pad_0x00D0[0x4]; //0x00D0
    char* m_szAnimationPrefix; //0x00D4 
    char pad_0x00D8[0x14]; //0x00D8
    int m_iDamage; //0x00EC 
    float m_fArmorRatio; //0x00F0 
    char pad_0x00F4[0x4]; //0x00F4
    float m_fPenetration; //0x00F8 
    char pad_0x00FC[0x8]; //0x00FC
    float m_fRange; //0x0104 
    float m_fRangeModifier; //0x0108 
    char pad_0x010C[0x10]; //0x010C
    unsigned char m_bHasSilencer; //0x011C 
    char pad_0x011D[0x10B]; //0x011D
    unsigned char m_bHasBurstmode; //0x0228 
    unsigned char m_bIsRevolver; //0x0229 
    char pad_0x022A[0x1BE]; //0x022A
}; 


class C_EconItemView
{
public:
    NETVAR(int32_t, m_bInitialized, "DT_BaseAttributableItem", "m_bInitialized");
    NETVAR(int32_t, m_iItemDefinitionIndex, "DT_BaseAttributableItem", "m_iItemDefinitionIndex");
    NETVAR(int32_t, m_iEntityLevel, "DT_BaseAttributableItem", "m_iEntityLevel");
    NETVAR(int32_t, m_iAccountID, "DT_BaseAttributableItem", "m_iAccountID");
    NETVAR(int32_t, m_iItemIDLow, "DT_BaseAttributableItem", "m_iItemIDLow");
    NETVAR(int32_t, m_iItemIDHigh, "DT_BaseAttributableItem", "m_iItemIDHigh");
    NETVAR(int32_t, m_iEntityQuality, "DT_BaseAttributableItem", "m_iEntityQuality");
};

class C_BaseEntity : public IClientEntity
{
public:
    static __forceinline C_BaseEntity* get_entity_by_index(int index)
    {
        return static_cast<C_BaseEntity*>(g_EntityList->GetClientEntity(index));
    }
    static __forceinline C_BaseEntity* get_entity_from_handle(CBaseHandle h)
    {
        return static_cast<C_BaseEntity*>(g_EntityList->GetClientEntityFromHandle(h));
    }

    NETVAR(CHandle<C_BasePlayer>, m_hOwnerEntity, "DT_BaseEntity", "m_hOwnerEntity");
    NETVAR(int32_t, m_nModelIndex, "DT_BaseViewModel", "m_nModelIndex");
    NETVAR(int32_t, m_iTeamNum, "DT_BaseEntity", "m_iTeamNum");
    NETVAR(Vector, m_vecOrigin, "DT_BaseEntity", "m_vecOrigin");
    NETVAR(bool, m_bShouldGlow, "DT_DynamicProp", "m_bShouldGlow");
    NETVAR(float, m_flSimulationTime, "DT_BaseEntity", "m_flSimulationTime");

    CBaseHandle GetOwnerHandle() //for that im sorry too xD
    {
        static int phook = netvars.find("DT_BaseEntity", "m_hOwnerEntity");
        return *(CBaseHandle*)((uintptr_t)this + phook);
    }

    const matrix3x4_t& m_rgflCoordinateFrame()
    {
        static auto _m_rgflCoordinateFrame = netvars.find("DT_BaseEntity", "m_CollisionGroup") - 0x30;
        return *(matrix3x4_t*)((uintptr_t)this + _m_rgflCoordinateFrame);
    }

    bool is_player();
    bool is_weapon();
    bool is_planted_c4();
    bool is_defuse_kit();
};

class C_PlantedC4
{
public:
    NETVAR(bool, m_bBombTicking, "DT_PlantedC4", "m_bBombTicking");
    NETVAR(bool, m_bBombDefused, "DT_PlantedC4", "m_bBombDefused");
    NETVAR(float, m_flC4Blow, "DT_PlantedC4", "m_flC4Blow");
    NETVAR(float, m_flTimerLength, "DT_PlantedC4", "m_flTimerLength");
    NETVAR(float, m_flDefuseLength, "DT_PlantedC4", "m_flDefuseLength");
    NETVAR(float, m_flDefuseCountDown, "DT_PlantedC4", "m_flDefuseCountDown");
    NETVAR(CHandle<C_BasePlayer>, m_hBombDefuser, "DT_PlantedC4", "m_hBombDefuser");
};

class C_BaseAttributableItem : public C_BaseEntity
{
public:
    NETVAR(uint64_t, m_OriginalOwnerXuid, "DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
    NETVAR(int32_t, m_OriginalOwnerXuidLow, "DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
    NETVAR(int32_t, m_OriginalOwnerXuidHigh, "DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh");
    NETVAR(int32_t, m_nFallbackStatTrak, "DT_BaseAttributableItem", "m_nFallbackStatTrak");
    NETVAR(int32_t, m_nFallbackPaintKit, "DT_BaseAttributableItem", "m_nFallbackPaintKit");
    NETVAR(int32_t, m_nFallbackSeed, "DT_BaseAttributableItem", "m_nFallbackSeed");
    NETVAR(float_t, m_flFallbackWear, "DT_BaseAttributableItem", "m_flFallbackWear");

    C_EconItemView& m_Item()
    {
        // Cheating. It should be this + m_Item netvar but then the netvars inside C_EconItemView wont work properly.
        // A real fix for this requires a rewrite of the netvar manager
        return *(C_EconItemView*)this;
    }
};
class CCSGrenade
{
public:
    NETVAR(float_t, m_fThrowTime, "DT_BaseCSGrenade", "m_fThrowTime");
};

class C_BaseCombatWeapon : public C_BaseAttributableItem
{
public:
    NETVAR(float_t, m_flNextPrimaryAttack, "DT_BaseCombatWeapon", "m_flNextPrimaryAttack");

    
    NETVAR(float_t, m_flNextSecondaryAttack, "DT_BaseCombatWeapon", "m_flNextSecondaryAttack");
    NETVAR(int32_t, m_iClip1, "DT_BaseCombatWeapon", "m_iClip1");
    NETVAR(int32_t, m_iClip2, "DT_BaseCombatWeapon", "m_iClip2");
    NETVAR(float_t, m_flRecoilIndex, "DT_WeaponCSBase", "m_flRecoilIndex");
    NETVAR(int, m_iViewModelIndex, "DT_BaseCombatWeapon", "m_iViewModelIndex");
    NETVAR(int, m_iWorldModelIndex, "DT_BaseCombatWeapon", "m_iWorldModelIndex");
    CCSWeaponInfo* get_weapon_data();
    bool has_bullets();
    bool can_fire();
    bool is_grenade();
    bool is_knife();
    bool is_reloading();
	bool is_rifle();
	bool is_pistol();
	bool is_sniper();
    float get_inaccuracy();
    float get_spread();
    void update_accuracy_penalty();

};
class CollisionProperty
{
public:
    NETVAR(Vector, GetMins, "DT_BaseEntity", "m_vecMins");
    NETVAR(Vector, GetMaxs, "DT_BaseEntity", "m_vecMaxs");
};

class C_BasePlayer : public C_BaseEntity
{
public:
    static __forceinline C_BasePlayer* get_local_player()
    {
        return static_cast<C_BasePlayer*>(get_entity_by_index(g_EngineClient->GetLocalPlayer()));
    }
    static __forceinline C_BasePlayer* get_player_by_uid(int id)
    {
        return static_cast<C_BasePlayer*>(get_entity_by_index(g_EngineClient->GetPlayerForUserID(id)));
    }
    static __forceinline C_BasePlayer* get_player_by_index(int i)
    {
        return static_cast<C_BasePlayer*>(get_entity_by_index(i));
    }

    NETVAR(bool, m_bGunGameImmunity, "DT_CSPlayer", "m_bGunGameImmunity");
    NETVAR(int32_t, m_iHealth, "DT_BasePlayer", "m_iHealth");
    NETVAR(int32_t, m_lifeState, "DT_BasePlayer", "m_lifeState");
    NETVAR(int32_t, m_fFlags, "DT_BasePlayer", "m_fFlags");
    NETVAR(int32_t, m_nTickBase, "DT_BasePlayer", "m_nTickBase");
    NETVAR(Vector, m_vecViewOffset, "DT_BasePlayer", "m_vecViewOffset[0]");
    NETVAR(QAngle, m_viewPunchAngle, "DT_BasePlayer", "m_viewPunchAngle");
    NETVAR(QAngle, m_aimPunchAngle, "DT_BasePlayer", "m_aimPunchAngle");
    NETVAR(CHandle<C_BaseCombatWeapon>, m_hActiveWeapon, "DT_BaseCombatCharacter", "m_hActiveWeapon");
    NETVAR(float_t, m_flNextAttack, "DT_BaseCombatCharacter", "m_flNextAttack");
    NETVAR(int32_t, m_iShotsFired, "DT_CSPlayer", "m_iShotsFired");
    NETVAR(QAngle, m_angEyeAngles, "DT_CSPlayer", "m_angEyeAngles[0]");
    NETVAR(CHandle<C_BaseViewModel>, m_hViewModel, "DT_BasePlayer", "m_hViewModel[0]");
    NETVAR(Vector, m_vecVelocity, "DT_BasePlayer", "m_vecVelocity[0]");
    NETVAR(float, m_flMaxspeed, "DT_BasePlayer", "m_flMaxspeed");
    NETVAR(int, m_ArmorValue, "DT_CSPlayer", "m_ArmorValue");
    NETVAR(bool, m_bHasHelmet, "DT_CSPlayer", "m_bHasHelmet");
    NETVAR(bool, m_bIsScoped, "DT_CSPlayer", "m_bIsScoped");;
    NETVAR(float, m_flLowerBodyYawTarget, "DT_CSPlayer", "m_flLowerBodyYawTarget");
    NETVAR(CHandle<C_BasePlayer>, m_hObserverTarget, "DT_BasePlayer", "m_hObserverTarget");
    PNETVAR(CHandle<C_BaseCombatWeapon>, m_hMyWeapons, "DT_BaseCombatCharacter", "m_hMyWeapons");
    PNETVAR(CHandle<C_BaseAttributableItem>, m_hMyWearables, "DT_BaseCombatCharacter", "m_hMyWearables");
    PNETVAR(CollisionProperty, collisionProperty, "DT_BaseEntity", "m_Collision");
    NETPROP(GetSequence, "DT_BaseAnimating", "m_nSequence");
    int GetMoveType() 
    {
        static int hehe = netvars.find("DT_BaseEntity", "m_nRenderMode"); // yes i know its gay xd
        return *(int*)((DWORD)this + (hehe + 1));
    }







    CUserCmd*& m_pCurrentCommand();

    Vector        get_eye_pos();
    player_info_t get_player_info();
    bool          is_alive();
    bool          has_c4();
    Vector        get_hitbox_pos(int hitbox_id);
    bool          get_hitbox_pos(int hitbox, Vector &output);
    Vector        get_bone_pos(int bone);
    bool          can_see_player(C_BasePlayer* player, int hitbox);
    bool          can_see_player(C_BasePlayer* entity, const Vector& pos);
    bool          is_visible(C_BasePlayer* pEntity, int BoneID);
};

class C_BaseViewModel : public C_BaseEntity
{
public:
    NETVAR(int32_t, m_nModelIndex, "DT_BaseViewModel", "m_nModelIndex");
    NETVAR(int32_t, m_nViewModelIndex, "DT_BaseViewModel", "m_nViewModelIndex");
    NETVAR(CHandle<C_BaseCombatWeapon>, m_hWeapon, "DT_BaseViewModel", "m_hWeapon");
    NETVAR(CHandle<C_BasePlayer>, m_hOwner, "DT_BaseViewModel", "m_hOwner");
    NETPROP(GetSequenceProp, "DT_BaseViewModel", "m_nSequence");
    inline DWORD GetOwner() {
        static int hehe = netvars.find("DT_BaseViewModel", "m_hOwner");
        return *(PDWORD)((DWORD)this + hehe);
    }
};
void UTIL_TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask,
    const IClientEntity *ignore, int collisionGroup, trace_t *ptr);
