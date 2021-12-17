#pragma once

#include "..\Utils\DrawManager.h"
#include "..\Utils\GlobalVars.h"
#include "..\Settings.h"
#include "..\SDK\ClientClass.h"
#include "..\SDK\ConVar.h"

void DoSkyBox()
{
	static bool bPerformed = false, bLastSetting;

	if (!g::pLocalEntity || !g_pEngine->IsConnected() || !g_pEngine->IsInGame())
	{
		if (bPerformed)
			bPerformed = false;
		return;
	}

	if (bLastSetting != g_Settings.bNoSky)
	{
		bLastSetting = g_Settings.bNoSky;
		bPerformed = false;
	}

	if (!bPerformed)
	{
		for (auto i = g_pMaterialSystem->FirstMaterial(); i != g_pMaterialSystem->InvalidMaterial(); i = g_pMaterialSystem->NextMaterial(i))
		{
			IMaterial* pMaterial = g_pMaterialSystem->GetMaterial(i);

			if (!pMaterial || pMaterial->IsErrorMaterial())
				continue;

			if (bLastSetting)
			{
				if (strstr(pMaterial->GetTextureGroupName(), "SkyBox"))
				{
					pMaterial->ColorModulate(0, 0, 0);
				}
			}
			else
			{
				if ((strstr(pMaterial->GetTextureGroupName(), "SkyBox")))
				{
					pMaterial->ColorModulate(1, 1, 1);
				}
			}
		} bPerformed = true;
	}
}


void ResetWorld()
{
	for (auto i = g_pMaterialSystem->FirstMaterial(); i != g_pMaterialSystem->InvalidMaterial(); i = g_pMaterialSystem->NextMaterial(i))
	{
		IMaterial* pMaterial = g_pMaterialSystem->GetMaterial(i);

		if (!pMaterial || pMaterial->IsErrorMaterial())
			continue;

		if ((strstr(pMaterial->GetTextureGroupName(), "World")) || strstr(pMaterial->GetTextureGroupName(), "StaticProp") 
			|| strstr(pMaterial->GetTextureGroupName(), "SkyBox"))
		{
			pMaterial->ColorModulate(1.f, 1.f, 1.f);
			pMaterial->AlphaModulate(1.f);
		}
	}
}