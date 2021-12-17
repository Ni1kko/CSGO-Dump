//#include "cheats/skinchanger/skinchanger.h"
//#include "Glovechanger.h"

/*

typedef unsigned long CBaseHandle;
#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);

void GloveChanger()
{
	if (SETTINGS::settings.glovesenabled)
	{
		SDK::player_info_t localPlayerInfo;

		SDK::CBaseEntity *pLocal = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (pLocal)
		{
			if (INTERFACES::Engine->GetPlayerInfo(INTERFACES::Engine->GetLocalPlayer(), &localPlayerInfo))
			{
				DWORD* hMyWearables = (DWORD*)((size_t)pLocal + OFFSETS::m_hMyWearables);

				if (hMyWearables)
				{
					if (!INTERFACES::ClientEntityList->GetClientEntityFromHandle((DWORD)hMyWearables[0]))
					{
						static SDK::ClientClass* pClass;

						if (!pClass)
							pClass = INTERFACES::Client->GetAllClasses();
						while (pClass)
						{
							if (pClass->m_ClassID == 45)
								break;
							pClass = pClass->m_pNext;
						}

						int iEntry = INTERFACES::ClientEntityList->GetHighestEntityIndex() + 1;
						int iSerial = RandomInt(0x0, 0xFFF);

						pClass->m_pCreateFn(iEntry, iSerial);
						hMyWearables[0] = iEntry | (iSerial << 16);

						SDK::CBaseEntity* pEnt = INTERFACES::ClientEntityList->GetClientEntityFromHandle((DWORD)hMyWearables[0]);

						if (pEnt)
						{
							int modelindex = 0;

							if (SETTINGS::settings.gloves == 1)
								modelindex = INTERFACES::ModelInfo->GetModelIndex(("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl"));
							else if (SETTINGS::settings.gloves == 2)
								modelindex = INTERFACES::ModelInfo->GetModelIndex(("models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl"));
							else if (SETTINGS::settings.gloves == 3)
								modelindex = INTERFACES::ModelInfo->GetModelIndex(("models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl"));
							else if (SETTINGS::settings.gloves == 4)
								modelindex = INTERFACES::ModelInfo->GetModelIndex(("models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl"));
							else if (SETTINGS::settings.gloves == 5)
								modelindex = INTERFACES::ModelInfo->GetModelIndex(("models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl"));
							else if (SETTINGS::settings.gloves == 6)
								modelindex = INTERFACES::ModelInfo->GetModelIndex(("models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl"));

							int ItemDefinitionIndex;
							if (SETTINGS::settings.gloves == 1)
							{
								ItemDefinitionIndex = 5027;
							}
							else if (SETTINGS::settings.gloves == 2)
							{
								ItemDefinitionIndex = 5030;
							}
							else if (SETTINGS::settings.gloves == 3)
							{
								ItemDefinitionIndex = 5031;
							}
							else if (SETTINGS::settings.gloves == 4)
							{
								ItemDefinitionIndex = 5032;
							}
							else if (SETTINGS::settings.gloves == 5)
							{
								ItemDefinitionIndex = 5033;
							}
							else if (SETTINGS::settings.gloves == 6)
							{
								ItemDefinitionIndex = 5034;
							}
							else
								ItemDefinitionIndex = 0;

							int paintkit;
							if (SETTINGS::settings.gloves == 1)
							{
								switch (SETTINGS::settings.skingloves)
								{
								case 0:
									paintkit = 10006;//bloodhound_black_silver
									break;
								case 1:
									paintkit = 10007;//bloodhound_snakeskin_brass
									break;
								case 2:
									paintkit = 10008;//bloodhound_metallic
									break;
								case 3:
									paintkit = 10039;//bloodhound_guerrilla
									break;
								}
							}
							else if (SETTINGS::settings.gloves == 2)
							{
								switch (SETTINGS::settings.skingloves)
								{
								case 0:
									paintkit = 10018;//sporty_light_blue
									break;
								case 1:
									paintkit = 10037;//sporty_purple
									break;
								case 2:
									paintkit = 10038;//sporty_green
									break;
								case 3:
									paintkit = 10019;//sporty_military
									break;
								}
							}
							else if (SETTINGS::settings.gloves == 3)
							{
								switch (SETTINGS::settings.skingloves)
								{
								case 0:
									paintkit = 10013;//slick_black
									break;
								case 1:
									paintkit = 10015;//slick_military
									break;
								case 2:
									paintkit = 10016;//slick_red
									break;
								case 3:
									paintkit = 10040;//slick_snakeskin_yellow
									break;
								}
							}
							else if (SETTINGS::settings.gloves == 4)
							{
								switch (SETTINGS::settings.skingloves)
								{
								case 0:
									paintkit = 10009;//handwrap_leathery
									break;
								case 1:
									paintkit = 10010;//handwrap_camo_grey
									break;
								case 2:
									paintkit = 10021;//handwrap_red_slaughter
									break;
								case 3:
									paintkit = 10036;//handwrap_fabric_orange_camo
									break;
								}
							}
							else if (SETTINGS::settings.gloves == 5)
							{
								switch (SETTINGS::settings.skingloves)
								{
								case 0:
									paintkit = 10024;//motorcycle_basic_black
									break;
								case 1:
									paintkit = 10026;//motorcycle_mint_triangle
									break;
								case 2:
									paintkit = 10027;//motorcycle_mono_boom
									break;
								case 3:
									paintkit = 10028;//motorcycle_triangle_blue
									break;
								}
							}
							else if (SETTINGS::settings.gloves == 6)
							{
								switch (SETTINGS::settings.skingloves)
								{
								case 0:
									paintkit = 10030;//specialist_ddpat_green_camo
									break;
								case 1:
									paintkit = 10033;//specialist_kimono_diamonds_red
									break;
								case 2:
									paintkit = 10034;//specialist_emerald_web
									break;
								case 3:
									paintkit = 10035;//specialist_orange_white
									break;
								}
							}

							*(int*)((DWORD)pEnt + OFFSETS::m_iItemDefinitionIndex) = ItemDefinitionIndex;
							*(int*)((DWORD)pEnt + 0x2FB0) = -1;
							*(int*)((DWORD)pEnt + 0x2F9C) = 4;
							*(int*)((DWORD)pEnt + 0x2FB8) = localPlayerInfo.xuidlow;
							*(float*)((DWORD)pEnt + 0x31B0) = SETTINGS::settings.glovewear;
							*(int*)((DWORD)pEnt + 0x31AC) = 0;
							*(int*)((DWORD)pEnt + 0x31B4) = -1;
							*(int*)((DWORD)pEnt + 0x31A8) = paintkit;

							pEnt->SetModelIndex(modelindex);
							pEnt->PreDataUpdate(DATA_UPDATE_CREATED);
						}
					}
				}
			}
		}
	}
}*/