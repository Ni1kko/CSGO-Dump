#define _CRT_SECURE_NO_WARNINGS

#include "Hacks.h"
#include "Interfaces.h"
#include "RenderManager.h"

#include "ESP.h"
#include "Visuals.h"
#include "RageBot.h"
#include "MiscHacks.h"

CEsp Esp;
CVisuals Visuals;
CMiscHacks MiscHacks;
CAimbot RageBot;

void Hacks::SetupHacks()
{
	Esp.Init();
	Visuals.Init();
	MiscHacks.Init();
	RageBot.Init();

	hackManager.RegisterHack(&Esp);
	hackManager.RegisterHack(&Visuals);
	hackManager.RegisterHack(&MiscHacks);
	hackManager.RegisterHack(&RageBot);

	hackManager.Ready();
}


void SpecList()
{
	IClientEntity *pLocal = hackManager.pLocal();

	RECT scrn = Render::GetViewport();
	int ayy = 0;

	// Loop through all active entitys
	for (int i = 0; i < interfaces::ent_list->GetHighestEntityIndex(); i++)
	{

		// Get the entity
		IClientEntity *pEntity = interfaces::ent_list->get_client_entity(i);
		player_info_t pinfo;

		// The entity isn't some laggy peice of shit or something
		if (pEntity &&  pEntity != pLocal)
		{
			if (interfaces::engine->GetPlayerInfo(i, &pinfo) && !pEntity->IsAlive() && !pEntity->IsDormant())
			{
				HANDLE obs = pEntity->GetObserverTargetHandle();

				if (obs)
				{
					IClientEntity *pTarget = interfaces::ent_list->GetClientEntityFromHandle(obs);
					player_info_t pinfo2;
					if (pTarget)
					{

						if (interfaces::engine->GetPlayerInfo(pTarget->GetIndex(), &pinfo2))
						{
							if (strlen(pinfo.name) > 16)
							{

								pinfo.name[12] = 0;
								strcat(pinfo.name, "...");
								RECT TextSize = Render::GetTextSize(Render::Fonts::esp, pinfo.name);
								RECT nameSize = Render::GetTextSize(Render::Fonts::esp, pinfo.name);
								Render::Text(scrn.right - TextSize.right - 5, (scrn.top / 4) + (16 * ayy), pTarget->GetIndex() /*== pLocal->GetIndex()*/ ? Color(170, 0, 250, 255) : Color(255, 255, 255, 255), Render::Fonts::esp, pinfo.name);
								ayy++;
							}
						}

					}
				}

			}
		}
	}

}


void Hacks::DrawHacks()
{
	if (!options::menu.visuals_tab.Active.GetState())
		return;


	if (options::menu.visuals_tab.SpecList.GetState())
		SpecList();


	hackManager.Draw();
		
}

void Hacks::MoveHacks(CUserCmd * pCmd, bool & bSendPacket)
{
	Vector origView = pCmd->viewangles;
	IClientEntity *pLocal = hackManager.pLocal();
	hackManager.Move(pCmd, bSendPacket);

}

HackManager hackManager;

void HackManager::RegisterHack(CHack* hake)
{
	Hacks.push_back(hake);
	hake->Init();
}

void HackManager::Draw()
{
	if (!IsReady)
		return;

	pLocalInstance = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());
	if (!pLocalInstance) return;

	for (auto &hack : Hacks)
	{
		hack->Draw();
	}
}

void HackManager::Move(CUserCmd *pCmd,bool &bSendPacket)
{
	if (!IsReady)
		return;

	pLocalInstance = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());
	if (!pLocalInstance) return;

	for (auto &hack : Hacks)
	{
		hack->Move(pCmd,bSendPacket); 
	}
}

IClientEntity* HackManager::pLocal()
{
	return pLocalInstance;
}

void HackManager::Ready()
{
	IsReady = true;
}























































































































































































