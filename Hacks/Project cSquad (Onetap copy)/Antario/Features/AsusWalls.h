#pragma once
#include "..\Utils\DrawManager.h"
#include "..\Utils\GlobalVars.h"
#include "..\Settings.h"
#include "..\SDK\ClientClass.h"
#include "..\SDK\ConVar.h"

void DoAsusWalls()
{
	static bool bPerformed = false, bLastSetting;
	static float bLastSettingWorldfl;
	static float bLastSettingsPropfl;

	if (!g::pLocalEntity || !g_pEngine->IsConnected() || !g_pEngine->IsInGame())
	{
		if (bPerformed)
			bPerformed = false;
		return;
	}
	
	if (bLastSettingWorldfl != g_Settings.flTransparentWorld || bLastSettingsPropfl != g_Settings.flTransparentProps)
	{
		bLastSettingWorldfl = g_Settings.flTransparentWorld;
		bLastSettingsPropfl = g_Settings.flTransparentProps;
		bPerformed = false;
	}

	if (!bPerformed)
	{
		for (auto i = g_pMaterialSystem->FirstMaterial(); i != g_pMaterialSystem->InvalidMaterial(); i = g_pMaterialSystem->NextMaterial(i))
		{
			IMaterial* pMaterial = g_pMaterialSystem->GetMaterial(i);

			if (!pMaterial || pMaterial->IsErrorMaterial())
				continue;


			if (strstr(pMaterial->GetTextureGroupName(), "World"))
			{
				pMaterial->AlphaModulate(g_Settings.flTransparentWorld / 100.f);
			}

			if ((strstr(pMaterial->GetTextureGroupName(), "StaticProp")))
			{
				pMaterial->AlphaModulate(g_Settings.flTransparentProps / 100.f);
			}
		} bPerformed = true;
	}
}

