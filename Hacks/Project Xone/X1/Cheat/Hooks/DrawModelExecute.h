#pragma once
#include "Tables.h"

void __fastcall DrawModelExecute(void* thisptr, int edx, IMatRenderContext* ctx, const DrawModelState_t &state,
	const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
{
	//ModelRenderTable.UnHook();

	if (GP_Esp && CGlobal::IsGameReady && ctx && &state && &pInfo && pCustomBoneToWorld /*&& !CheatUnload*/ && !CGlobal::FullUpdateCheck)
		GP_Esp->DrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

	if (GP_Misc && CGlobal::IsGameReady  && ctx && &state && &pInfo && pCustomBoneToWorld /*&& !CheatUnload*/ && !CGlobal::FullUpdateCheck)
		GP_Misc->DrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

	//ModelRenderTable.ReHook();
	HookTables::pDrawModelExecute->GetTrampoline()(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
	I::ModelRender()->ForcedMaterialOverride(0);	
}