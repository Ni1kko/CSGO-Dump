#pragma once

namespace OFFSETS
{
	extern uintptr_t m_iHealth;
	extern uintptr_t m_fFlags;
	extern uintptr_t m_vecVelocity;
	extern uintptr_t m_flLowerBodyYawTarget;
	extern uintptr_t deadflag;
	extern uintptr_t m_vecOrigin;
	extern uintptr_t m_iTeamNum;
	extern uintptr_t m_nTickBase;
	extern uintptr_t m_bDormant;
	extern uintptr_t animstate;
	extern uintptr_t m_Collision;
	extern uintptr_t m_angEyeAngles;
	extern uintptr_t m_flSimulationTime;
	extern uintptr_t m_vecViewOffset;
	extern uintptr_t m_dwBoneMatrix;
	extern uintptr_t m_aimPunchAngle;
	extern uintptr_t m_bGunGameImmunity;
	extern uintptr_t m_nForceBone;
	extern uintptr_t m_flPoseParameter;
	extern uintptr_t dwGlowObjectManager;
	extern uintptr_t m_flNextPrimaryAttack;
	extern uintptr_t m_flNextAttack;
	extern uintptr_t m_hActiveWeapon;
	extern uintptr_t m_nSequence;

	extern uintptr_t m_flDuckAmount;
	extern uintptr_t m_flDuckSpeed;

	extern uintptr_t m_ArmorValue;
	extern uintptr_t m_bHasHelmet;
	extern uintptr_t m_iObserverMode;
	extern uintptr_t m_bIsScoped;
	extern uintptr_t m_flCycle;
	extern uintptr_t m_hObserverTarget;

	extern uintptr_t m_iAccount;
	extern uintptr_t m_iPlayerC4;
	extern uintptr_t dwPlayerResource;
	extern uintptr_t m_iItemDefinitionIndex;
	extern uintptr_t m_lifeState;
	extern uintptr_t m_flPostponeFireReadyTime;
	extern uintptr_t m_fThrowTime;
	extern uintptr_t m_bPinPulled;
	extern uintptr_t m_MoveType;
	extern uintptr_t m_viewPunchAngle;
	extern uintptr_t invalidateBoneCache;
	extern uintptr_t m_iShotsFired;
	extern uintptr_t m_bClientSideAnimation;
	extern uintptr_t m_iItemIDHigh;
	extern uintptr_t m_iEntityQuality;
	extern uintptr_t m_iAccountID;
	extern uintptr_t m_flFallbackWear;
	extern uintptr_t m_nFallbackSeed;
	extern uintptr_t m_nFallbackStatTrak;
	extern uintptr_t m_nFallbackPaintKit;
	extern uintptr_t m_hMyWearables;
	extern uintptr_t m_nModelIndex;
	extern uintptr_t m_hOwner;
	extern uintptr_t m_iViewModelIndex;
	extern uintptr_t m_iWorldModelIndex;
	extern uintptr_t m_hWeaponWorldModel;
	extern uintptr_t m_Item;
	extern uintptr_t m_OriginalOwnerXuidLow;
	extern uintptr_t m_OriginalOwnerXuidHigh;
	extern uintptr_t m_iClip1;
	extern uintptr_t m_hMyWeapons;
	extern uintptr_t animlayer;
	void InitOffsets();
}