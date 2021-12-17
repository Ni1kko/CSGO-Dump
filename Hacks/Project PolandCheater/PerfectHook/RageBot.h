
#pragma once

#include "SDK.h"
#include "singleton.hpp"

class ragebot
    : public singleton<ragebot>
{
public:

    ragebot();

    void OnCreateMove(CInput::CUserCmd *pCmd, bool& bSendPacket);
	bool hit_chance(C_BaseEntity* local, CInput::CUserCmd* cmd, CBaseCombatWeapon* weapon, C_BaseEntity* target);
private:
    float FovToPlayer(Vector ViewOffSet, Vector View, C_BaseEntity* pEntity, int aHitBox);
	int GetTargetCrosshair();
	bool TargetMeetsRequirements(C_BaseEntity* pEntity);
	int HitScan(C_BaseEntity* pEntity);
    void DoNoRecoil(CInput::CUserCmd* pCmd);
    bool AimAtPoint(C_BaseEntity* pLocal, Vector point, CInput::CUserCmd *pCmd);
	void DoAimbot(CInput::CUserCmd *pCmd, bool& bSendPacket);
	void DoAntiAim(CInput::CUserCmd *pCmd, bool& bSendPacket);
private:
	bool IsLocked;
	int TargetID;
	int HitBox;
	Vector AimPoint;
	C_BaseEntity* pTarget;
};

inline bool CanAttack()
{
    auto local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());

    if (local && local->IsAlive())
    {
        CBaseCombatWeapon* weapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(local->GetActiveWeaponHandle());
        if (weapon)
        {
            if (weapon->GetAmmoInClip() < 1 && !MiscFunctions::IsKnife(weapon))
                return false;

            bool revolver_can_shoot = true;
            if (weapon->m_AttributeManager()->m_Item()->GetItemDefinitionIndex() == 64)
            {
                float time_to_shoot = weapon->m_flPostponeFireReadyTime() - local->GetTickBase() * g_Globals->interval_per_tick;
                revolver_can_shoot = time_to_shoot <= g_Globals->absoluteframetime;
            }

            float time = local->GetTickBase() * g_Globals->interval_per_tick;
            float next_attack = weapon->GetNextPrimaryAttack();
            return revolver_can_shoot && !(next_attack > time);
        }
    }

    return false;
}