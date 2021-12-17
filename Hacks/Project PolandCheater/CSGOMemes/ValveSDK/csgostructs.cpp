#include "csgostructs.hpp"
#include "../Helpers/math.hpp"
#include "../Helpers/utils.hpp"

bool C_BaseEntity::is_player()
{
    //index: 152
    //ref: "effects/nightvision"
    //sig: 8B 92 ? ? ? ? FF D2 84 C0 0F 45 F7 85 F6
    return CallVFunction<bool(__thiscall*)(C_BaseEntity*)>(this, 152)(this);
}

bool C_BaseEntity::is_weapon()
{
    //index: 160
    //ref: "CNewParticleEffect::DrawModel"
    //sig: 8B 80 ? ? ? ? FF D0 84 C0 74 6F 8B 4D A4
    return CallVFunction<bool(__thiscall*)(C_BaseEntity*)>(this, 160)(this);
}

bool C_BaseEntity::is_planted_c4()
{
    return GetClientClass()->m_ClassID == CPlantedC4;
}

bool C_BaseEntity::is_defuse_kit()
{
    return GetClientClass()->m_ClassID == CBaseAnimating;
}

CCSWeaponInfo* C_BaseCombatWeapon::get_weapon_data()
{
    if (!this)
        return nullptr;
    typedef CCSWeaponInfo*(__thiscall* tGetCSWpnData)(void*);
    return CallVFunction<tGetCSWpnData>(this, 446)(this);
}


bool C_BaseCombatWeapon::has_bullets()
{
    return !is_reloading() && m_iClip1() > 0;
}

bool C_BaseCombatWeapon::can_fire()
{
    if(is_reloading() || m_iClip1() <= 0)
        return false;

    auto local = C_BasePlayer::get_local_player();
    if(!local)
        return false;

    float flServerTime = local->m_nTickBase() * g_GlobalVars->interval_per_tick;

    return m_flNextPrimaryAttack() <= flServerTime;
}

bool C_BaseCombatWeapon::is_grenade()
{
    if (GetClientClass()->m_ClassID == CDecoyGrenade || GetClientClass()->m_ClassID == CHEGrenade || GetClientClass()->m_ClassID == CIncendiaryGrenade || GetClientClass()->m_ClassID == CMolotovGrenade || GetClientClass()->m_ClassID == CSensorGrenade || GetClientClass()->m_ClassID == CSmokeGrenade || GetClientClass()->m_ClassID == CFlashbang)
        return true;
    else
        return false;
}

/*bool C_BaseCombatWeapon::is_knife()
{
    return get_weapon_data()->m_WeaponType == WEAPONTYPE_KNIFE;
}*/

bool C_BaseCombatWeapon::is_knife()
{
    if (!this)
        return false;

    int weaponid = m_Item().m_iItemDefinitionIndex();

    if (weaponid == WEAPON_KNIFE_CT || weaponid == WEAPON_KNIFE_T || weaponid == WEAPON_KNIFE_BAYONET || weaponid == WEAPON_KNIFE_BUTTERFLY || weaponid == WEAPON_KNIFE_FALCHION || weaponid == WEAPON_KNIFE_FLIP || weaponid == WEAPON_KNIFE_GUT || weaponid == WEAPON_KNIFE_KARAMBIT || weaponid == WEAPON_KNIFE_M9BAYONET || weaponid == WEAPON_KNIFE_DAGGER || weaponid == WEAPON_KNIFE_HUNTSMAN || weaponid == WEAPON_KNIFE_BOWIE)
        return true;

    return false;
}

bool C_BaseCombatWeapon::is_rifle()
{
    if (this == nullptr) return false;

    ClientClass* pWeaponClass = this->GetClientClass();

    if (pWeaponClass->m_ClassID == CDEagle || pWeaponClass->m_ClassID == CWeaponElite || pWeaponClass->m_ClassID == CWeaponFiveSeven || pWeaponClass->m_ClassID == CWeaponGlock || pWeaponClass->m_ClassID == CWeaponHKP2000 || pWeaponClass->m_ClassID == CWeaponP250 || pWeaponClass->m_ClassID == CWeaponP228 || pWeaponClass->m_ClassID == CWeaponTec9 || pWeaponClass->m_ClassID == CWeaponUSP)
        return true;

    return false;
}

bool C_BaseCombatWeapon::is_pistol()
{
    if (this == nullptr) return false;

    ClientClass* pWeaponClass = this->GetClientClass();

    if (pWeaponClass->m_ClassID == CDEagle || pWeaponClass->m_ClassID == CWeaponElite || pWeaponClass->m_ClassID == CWeaponFiveSeven || pWeaponClass->m_ClassID == CWeaponGlock || pWeaponClass->m_ClassID == CWeaponHKP2000 || pWeaponClass->m_ClassID == CWeaponP250 || pWeaponClass->m_ClassID == CWeaponP228 || pWeaponClass->m_ClassID == CWeaponTec9 || pWeaponClass->m_ClassID == CWeaponUSP)
        return true;

    return false;
}

bool C_BaseCombatWeapon::is_sniper()
{
    if (this == nullptr) return false;

    ClientClass* pWeaponClass = this->GetClientClass();

    if (pWeaponClass->m_ClassID == CWeaponAWP || pWeaponClass->m_ClassID == CWeaponSSG08 || pWeaponClass->m_ClassID == CWeaponSCAR20 || pWeaponClass->m_ClassID == CWeaponG3SG1)
        return true;

    return false;
}

bool C_BaseCombatWeapon::is_reloading()
{
    static auto inReload = *(uint32_t*)(utils::pattern_scan(GetModuleHandleW(L"client.dll"), "C6 87 ? ? ? ? ? 8B 06 8B CE FF 90") + 2);
    if (!this) return false;
    return *(bool*)((uintptr_t)this + inReload);
}

float C_BaseCombatWeapon::get_inaccuracy()
{
    return CallVFunction<float(__thiscall*)(void*)>(this, 469)(this);
}

float C_BaseCombatWeapon::get_spread()
{
    return CallVFunction<float(__thiscall*)(void*)>(this, 439)(this);
}

void C_BaseCombatWeapon::update_accuracy_penalty()
{
    CallVFunction<void(__thiscall*)(void*)>(this, 470)(this);
}

CUserCmd*& C_BasePlayer::m_pCurrentCommand()
{
    static auto currentCommand = *(uint32_t*)(utils::pattern_scan(GetModuleHandleW(L"client.dll"), "89 9F ? ? ? ? E8 ? ? ? ? 85 DB") + 2);
    return *(CUserCmd**)((uintptr_t)this + currentCommand);
}

Vector C_BasePlayer::get_eye_pos()
{
    return m_vecOrigin() + m_vecViewOffset();
}

player_info_t C_BasePlayer::get_player_info()
{
    player_info_t info;
    g_EngineClient->GetPlayerInfo(EntIndex(), &info);
    return info;
}

bool C_BasePlayer::is_alive()
{
    return m_lifeState() == LIFE_ALIVE;
}

bool C_BasePlayer::has_c4()
{
    static auto fnHasC4
        = reinterpret_cast<bool(__thiscall*)(void*)>(
            utils::pattern_scan(GetModuleHandleW(L"client.dll"), "56 8B F1 85 F6 74 31 ")
            );

    return fnHasC4(this);
}

Vector C_BasePlayer::get_hitbox_pos(int hitbox_id)
{
    matrix3x4_t boneMatrix[MAXSTUDIOBONES];

    if(SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f)) {
        auto studio_model = g_MdlInfo->GetStudiomodel(GetModel());
        if(studio_model) {
            auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(hitbox_id);
            if(hitbox) {
                auto
                    min = Vector{},
                    max = Vector{};

                math::vector_transform(hitbox->bbmin, boneMatrix[hitbox->bone], min);
                math::vector_transform(hitbox->bbmax, boneMatrix[hitbox->bone], max);

                return (min + max) / 2.0f;
            }
        }
    }
    return Vector{};
}

bool C_BasePlayer::get_hitbox_pos(int hitbox, Vector &output)
{
    if(hitbox >= HITBOX_MAX)
        return false;

    const model_t *model = this->GetModel();
    if(!model)
        return false;

    studiohdr_t *studioHdr = g_MdlInfo->GetStudiomodel(model);
    if(!studioHdr)
        return false;

    matrix3x4_t matrix[MAXSTUDIOBONES];
    if(!this->SetupBones(matrix, MAXSTUDIOBONES, 0x100, 0))
        return false;

    mstudiobbox_t *studioBox = studioHdr->GetHitboxSet(0)->GetHitbox(hitbox);
    if(!studioBox)
        return false;

    Vector min, max;

    math::vector_transform(studioBox->bbmin, matrix[studioBox->bone], min);
    math::vector_transform(studioBox->bbmax, matrix[studioBox->bone], max);

    output = (min + max) * 0.5f;

    return true;
}

Vector C_BasePlayer::get_bone_pos(int bone)
{
    matrix3x4_t boneMatrix[MAXSTUDIOBONES];

    if(SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, 0.0f)) {
        return Vector{ boneMatrix[bone].at(0) };
    }
    return Vector{};
}

bool C_BasePlayer::can_see_player(C_BasePlayer* player, int hitbox)
{
    CGameTrace tr;
    Ray_t ray;
    CTraceFilter filter;
    filter.pSkip = this;

    auto endpos = player->get_hitbox_pos(hitbox);

    ray.Init(get_eye_pos(), endpos);
    g_EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

    return (tr.hit_entity == player || tr.fraction > 0.97f);
}

bool C_BasePlayer::can_see_player(C_BasePlayer* entity, const Vector& pos)
{
    CGameTrace tr;
    Ray_t ray;
    CTraceFilter filter;
    filter.pSkip = this;

    ray.Init(get_eye_pos(), pos);
    g_EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

    return (tr.hit_entity == entity || tr.fraction > 0.97f);
}


void UTIL_TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask,
    const IClientEntity *ignore, int collisionGroup, trace_t *ptr)
{
    typedef int(__fastcall* UTIL_TraceLine_t)(const Vector&, const Vector&, unsigned int, const IClientEntity*, int, trace_t*);
    static UTIL_TraceLine_t TraceLine = (UTIL_TraceLine_t)utils::pattern_scan(GetModuleHandleW(L"client.dll"), "55 8B EC 83 E4 F0 83 EC 7C 56 52");
    TraceLine(vecAbsStart, vecAbsEnd, mask, ignore, collisionGroup, ptr);
}
trace_t Trace;
bool C_BasePlayer::is_visible(C_BasePlayer* pEntity, int BoneID)
{
    if (BoneID < 0) return false;
    Vector start = this->m_vecOrigin() + this->m_vecViewOffset();
    Vector end = pEntity->get_hitbox_pos(BoneID);


    UTIL_TraceLine(start, end, MASK_SOLID, this, 0, &Trace);

    if (Trace.hit_entity == this)
    {
        return true;
    }

    if (Trace.fraction == 1.0f)
    {
        return true;
    }

    return false;
}