#pragma once
#include "..\Utils\DrawManager.h"
#include "..\Utils\GlobalVars.h"
#include "..\Settings.h"
#include "..\SDK\ClientClass.h"
#include "..\SDK\ConVar.h"

void DoNightMode()
{
	static bool bPerformed = false, bLastSetting;

	if (!g::pLocalEntity || !g_pEngine->IsConnected() || !g_pEngine->IsInGame())
	{
		if (bPerformed)
			bPerformed = false;
		return;
	}

	if (bLastSetting != g_Settings.bNightMode)
	{
		bLastSetting = g_Settings.bNightMode;
		bPerformed = false;
	}

	if (!bPerformed)
	{
		static ConVar* r_drawspecificstaticprop = g_pConVar->FindVar("r_drawspecificstaticprop");
		r_drawspecificstaticprop->SetValue("r_drawspecificstaticprop 0");

		for (auto i = g_pMaterialSystem->FirstMaterial(); i != g_pMaterialSystem->InvalidMaterial(); i = g_pMaterialSystem->NextMaterial(i))
		{
			IMaterial* pMaterial = g_pMaterialSystem->GetMaterial(i);

			if (!pMaterial || pMaterial->IsErrorMaterial())
				continue;

			if (bLastSetting)
			{
				if (strstr(pMaterial->GetTextureGroupName(), "World"))
				{
					pMaterial->ColorModulate(0.4, 0.4, 0.4);
				//	pMaterial->ColorModulate(0.9490196078431373, 0.396078431372549, 0.1333333333333333);
				}

				if (strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
				{
					pMaterial->ColorModulate(0.3, 0.3, 0.3);
				//	pMaterial->ColorModulate(1, 0.596078431372549, 0.3333333333333333);
				}
				if (strstr(pMaterial->GetTextureGroupName(), "SkyBox"))
				{
					pMaterial->ColorModulate(0.3, 0, 0);
					//	pMaterial->ColorModulate(1, 0.596078431372549, 0.3333333333333333);
				}
			}	
			else
			{
				if (strstr(pMaterial->GetTextureGroupName(), "World"))
				{
					pMaterial->ColorModulate(1, 1, 1);
					//	pMaterial->ColorModulate(0.9490196078431373, 0.396078431372549, 0.1333333333333333);
				}

				if (strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
				{
					pMaterial->ColorModulate(1, 1, 1);
					//	pMaterial->ColorModulate(1, 0.596078431372549, 0.3333333333333333);
				}
				if (strstr(pMaterial->GetTextureGroupName(), "SkyBox"))
				{
					pMaterial->ColorModulate(1, 1, 1);
					//	pMaterial->ColorModulate(1, 0.596078431372549, 0.3333333333333333);
				}
			}
		} bPerformed = true;
	}
}