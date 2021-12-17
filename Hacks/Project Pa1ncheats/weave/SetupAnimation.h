#pragma once
#include "Hooks.h"

void __fastcall Hooked_StandardBlendingRules(IBasePlayer* player, void* edx, c_studio_hdr* hdr, Vector* pos, Quaternion* q, float curTime, int boneMask)
{
	*(int*)((DWORD)player + 0x2698) = 0;
	boneMask |= 0x200;

	player->GetEffects() |= 8;
	H::StandardBlendingRules(player, hdr, pos, q, curTime, boneMask);
	player->GetEffects() &= ~8;
}

void __fastcall Hooked_BuildTransformations(void* ecx, void* edx, int a2, int a3, int a4, int a5, int a6, int a7) {
	auto entity = (IBasePlayer*)ecx;
	if (!entity || !entity->IsPlayer() || !entity->isAlive())
		return H::BuildTransformations(ecx, a2, a3, a4, a5, a6, a7);

	auto hdr = (c_studio_hdr*)a2;

	// backup bone flags.
	const auto backup_bone_flags = hdr->m_boneFlags;

	// stop procedural animations.
	for (auto i = 0; i < hdr->m_boneFlags.Count(); i++)
		hdr->m_boneFlags.Element(i) &= ~0x04;

	//auto backup_jiggle_bones = *(bool*)((DWORD)entity + 0x292C);

	//*(bool*)((DWORD)entity + 0x292C) = false;

	H::BuildTransformations(ecx, a2, a3, a4, a5, a6, a7);

	hdr->m_boneFlags = backup_bone_flags;
}

bool __fastcall Hooked_SetupBones(void* ecx, void* edx, matrix* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
{
	if (csgo->DoUnload)
		return H::SetupBones(ecx, edx, pBoneToWorldOut, nMaxBones, boneMask, currentTime);

	auto entity = (IBasePlayer*)((uintptr_t)ecx - 4);
	if (entity == nullptr || csgo->local == nullptr || !pBoneToWorldOut)
		return H::SetupBones(ecx, edx, pBoneToWorldOut, nMaxBones, boneMask, currentTime);

	if (entity->IsPlayer() && entity->isAlive()) {

		*(int*)((DWORD)entity + 0x2698) = 0;
		boneMask |= 0x200;

		if (entity == csgo->local) {
			return H::SetupBones(ecx, edx, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
		}
		else {
			if (entity->GetTeam() == csgo->local->GetTeam())
				return H::SetupBones(ecx, edx, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
			else {
				if (csgo->UpdateMatrix)
					return H::SetupBones(ecx, edx, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
				else {
					memcpy(pBoneToWorldOut, entity->GetBoneCache().Base(), sizeof(matrix) * nMaxBones);
					return true;
				}
			}
		}
	}
	else
		return H::SetupBones(ecx, edx, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
}

bool __fastcall Hooked_ShouldSkipAnimFrame()
{
	return false;
}

void __fastcall Hooked_DoExtraBoneProcessing(IBasePlayer* player, uint32_t, c_studio_hdr* hdr, Vector* pos, Quaternion* q,
	const matrix& mat, uint8_t* bone_computed, void* context)
{
	return;
}

void __fastcall Hooked_UpdateClientSideAnimation(IBasePlayer* player, uint32_t)
{
	if (csgo->DoUnload)
		return H::UpdateClientSideAnimation(player);

	if (!player || !csgo->local || !csgo->local->isAlive())
	{
		H::UpdateClientSideAnimation(player);
		return;
	}

	if (csgo->local == player && csgo->ShouldUpdate
		|| player != csgo->local && csgo->EnableBones)
		H::UpdateClientSideAnimation(player);
}