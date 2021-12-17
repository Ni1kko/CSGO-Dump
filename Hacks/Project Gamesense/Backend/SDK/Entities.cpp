#include "CBaseEntity.h"

int	CBaseViewModel::GetModelIndex()
{
	// DT_BaseViewModel -> m_nModelIndex
	return *(int*)((DWORD)this + NetVarManager->getOffs("CBasePlayer", "m_nModelIndex"));
}

void CBaseViewModel::SetModelIndex(int nModelIndex)
{
	typedef void(__thiscall* OriginalFn)(PVOID, int);
	vfunc< OriginalFn >(this, 75)(this, nModelIndex);
}

void CBaseViewModel::SetWeaponModel(const char* Filename, IClientEntity* Weapon)
{
	typedef void(__thiscall* SetWeaponModelFn)(void*, const char*, IClientEntity*);
	return vfunc<SetWeaponModelFn>(this, 247)(this, Filename, Weapon);
}

DWORD CBaseViewModel::GetOwner()
{
	// DT_BaseViewModel -> m_hOwner
	return *(PDWORD)((DWORD)this + NetVarManager->getOffs("CBaseViewModel", "m_hOwner"));
}

DWORD CBaseViewModel::GetWeapon()
{
	// DT_BaseViewModel -> m_hWeapon
	return *(PDWORD)((DWORD)this + NetVarManager->getOffs("CBaseViewModel", "m_hWeapon"));
}

int* CBaseViewModel::m_nModelIndex()
{
	return (int*)((DWORD)this + NetVarManager->getOffs("CBaseViewModel", "m_nModelIndex"));
}

int* CEconEntity::GetFallbackPaintKit()
{
	return (int*)((DWORD)this + NetVarManager->getOffs("CEconEntity", "m_nFallbackPaintKit"));
}

int* CBaseAttributableItem::GetItemDefinitionIndex()
{
	// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iItemDefinitionIndex
	return (PINT)((DWORD)this + NetVarManager->getOffs("CBaseAttributableItem", "m_iItemDefinitionIndex"));
}

int* CBaseAttributableItem::GetItemIDHigh()
{
	// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iItemIDHigh
	return (int*)((DWORD)this + NetVarManager->getOffs("CBaseAttributableItem", "m_iItemIDHigh"));
}

int* CBaseAttributableItem::GetAccountID()
{
	// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iAccountID
	return (int*)((DWORD)this + NetVarManager->getOffs("CBaseAttributableItem", "m_iAccountID"));
}

int* CBaseAttributableItem::GetEntityQuality()
{
	// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iEntityQuality
	return (int*)((DWORD)this + NetVarManager->getOffs("CBaseAttributableItem", "m_iEntityQuality")); // k
}

int* CBaseAttributableItem::GetOriginalOwnerXuidLow()
{
	// DT_BaseAttributableItem -> m_OriginalOwnerXuidLow
	return (int*)((DWORD)this + NetVarManager->getOffs("CBaseAttributableItem", "m_OriginalOwnerXuidLow"));
}

int* CBaseAttributableItem::GetOriginalOwnerXuidHigh()
{
	// DT_BaseAttributableItem -> m_OriginalOwnerXuidHigh
	return (int*)((DWORD)this + NetVarManager->getOffs("CBaseAttributableItem", "m_OriginalOwnerXuidHigh"));
}

int* CBaseAttributableItem::GetFallbackPaintKit()
{
	// DT_BaseAttributableItem -> m_nFallbackPaintKit
	return (int*)((DWORD)this + NetVarManager->getOffs("CBaseAttributableItem", "m_nFallbackPaintKit"));
}

float* CBaseAttributableItem::GetFallbackWear()
{
	// DT_BaseAttributableItem -> m_flFallbackWear
	return (float*)((DWORD)this + NetVarManager->getOffs("CBaseAttributableItem", "m_flFallbackWear"));
}

int* CBaseAttributableItem::GetFallbackSeed()
{
	// DT_BaseAttributableItem -> m_nFallbackSeed
	return (int*)((DWORD)this + NetVarManager->getOffs("CBaseAttributableItem", "m_nFallbackSeed"));
}

int* CBaseAttributableItem::GetFallbackStatTrak()
{
	// DT_BaseAttributableItem -> m_nFallbackStatTrak
	return (int*)((DWORD)this + NetVarManager->getOffs("CBaseAttributableItem", "m_nFallbackStatTrak"));
}

CBaseViewModel* CBaseAttributableItem::GetViewModel()
{
	return (CBaseViewModel*)(DWORD)this;
}

int* CBaseAttributableItem::m_nModelIndex()
{
	return (int*)((DWORD)this + NetVarManager->getOffs("CBaseEntity", "m_nModelIndex"));
}

void CBaseAttributableItem::SetModelIndex(int nModelIndex)
{
	typedef void(__thiscall* OriginalFn)(PVOID, int);
	vfunc< OriginalFn >(this, 75)(this, nModelIndex);
}