//#include "Skins.h"
//#include "Glovechanger.h"
//
//
//
//
//typedef unsigned long CBaseHandle;
//#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);
//
//void GloveChanger()
//{
//	if (SETTINGS::settings.glovesenabled)
//	{
//		player_info_t localPlayerInfo;
//		player_t *pLocal = (player_t*)g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer());
//		if (pLocal)
//		{
//			if (g_csgo.m_engine()->GetPlayerInfo(pLocal->EntIndex(), &localPlayerInfo))
//			{
//				DWORD* hMyWearables = (DWORD*)((size_t)pLocal + OFFSETS::m_hMyWearables);
//
//				if (hMyWearables)
//				{
//					if (!g_csgo.ClientEntityList->GetClientEntityFromHandle((DWORD)hMyWearables[0]))
//
//					{
//						static SDK::ClientClass* pClass;
//
//						if (!pClass)
//							pClass = INTERFACES::Client->GetAllClasses();
//						while (pClass)
//						{
//							if (pClass->m_ClassID == 45)
//								break;
//							pClass = pClass->m_pNext;
//						}
//
//						int iEntry = INTERFACES::ClientEntityList->GetHighestEntityIndex() + 1;
//						int iSerial = RandomInt(0x0, 0xFFF);
//
//						pClass->m_pCreateFn(iEntry, iSerial);
//						hMyWearables[0] = iEntry | (iSerial << 16);
//
//						SDK::CBaseEntity* pEnt = INTERFACES::ClientEntityList->GetClientEntityFromHandle((DWORD)hMyWearables[0]);
//
//						if (pEnt)
//						{
//							int modelindex = 0;
//
//							if (SETTINGS::settings.gloves == 1)
//								modelindex = INTERFACES::ModelInfo->GetModelIndex(("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl"));
//							else if (SETTINGS::settings.gloves == 2)
//								modelindex = INTERFACES::ModelInfo->GetModelIndex(("models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl"));
//							else if (SETTINGS::settings.gloves == 3)
//								modelindex = INTERFACES::ModelInfo->GetModelIndex(("models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl"));
//							else if (SETTINGS::settings.gloves == 4)
//								modelindex = INTERFACES::ModelInfo->GetModelIndex(("models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl"));
//							else if (SETTINGS::settings.gloves == 5)
//								modelindex = INTERFACES::ModelInfo->GetModelIndex(("models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl"));
//							else if (SETTINGS::settings.gloves == 6)
//								modelindex = INTERFACES::ModelInfo->GetModelIndex(("models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl"));
//
//							int ItemDefinitionIndex;
//							if (SETTINGS::settings.gloves == 1)
//							{
//								ItemDefinitionIndex = 5027;
//							}
//							else if (SETTINGS::settings.gloves == 2)
//							{
//								ItemDefinitionIndex = 5030;
//							}
//							else if (SETTINGS::settings.gloves == 3)
//							{
//								ItemDefinitionIndex = 5031;
//							}
//							else if (SETTINGS::settings.gloves == 4)
//							{
//								ItemDefinitionIndex = 5032;
//							}
//							else if (SETTINGS::settings.gloves == 5)
//							{
//								ItemDefinitionIndex = 5033;
//							}
//							else if (SETTINGS::settings.gloves == 6)
//							{
//								ItemDefinitionIndex = 5034;
//							}
//							else
//								ItemDefinitionIndex = 0;
//
//							int paintkit;
//							if (SETTINGS::settings.gloves == 1)
//							{
//								switch (SETTINGS::settings.skingloves)
//								{
//								case 0:
//									paintkit = 10006;
//									break;
//								case 1:
//									paintkit = 10007;
//									break;
//								case 2:
//									paintkit = 10008;
//									break;
//								case 3:
//									paintkit = 10039;
//									break;
//								}
//							}
//							else if (SETTINGS::settings.gloves == 2)
//							{
//								switch (SETTINGS::settings.skingloves)
//								{
//								case 0:
//									paintkit = 10018;
//									break;
//								case 1:
//									paintkit = 10037;
//									break;
//								case 2:
//									paintkit = 10038;
//									break;
//								case 3:
//									paintkit = 10019;
//									break;
//								}
//							}
//							else if (SETTINGS::settings.gloves == 3)
//							{
//								switch (SETTINGS::settings.skingloves)
//								{
//								case 0:
//									paintkit = 10013;
//									break;
//								case 1:
//									paintkit = 10015;
//									break;
//								case 2:
//									paintkit = 10016;
//									break;
//								case 3:
//									paintkit = 10040;
//									break;
//								}
//							}
//							else if (SETTINGS::settings.gloves == 4)
//							{
//								switch (SETTINGS::settings.skingloves)
//								{
//								case 0:
//									paintkit = 10009;
//									break;
//								case 1:
//									paintkit = 10010;
//									break;
//								case 2:
//									paintkit = 10021;
//									break;
//								case 3:
//									paintkit = 10036;
//									break;
//								}
//							}
//							else if (SETTINGS::settings.gloves == 5)
//							{
//								switch (SETTINGS::settings.skingloves)
//								{
//								case 0:
//									paintkit = 10024;
//									break;
//								case 1:
//									paintkit = 10026;
//									break;
//								case 2:
//									paintkit = 10027;
//									break;
//								case 3:
//									paintkit = 10028;
//									break;
//								}
//							}
//							else if (SETTINGS::settings.gloves == 6)
//							{
//								switch (SETTINGS::settings.skingloves)
//								{
//								case 0:
//									paintkit = 10030;
//									break;
//								case 1:
//									paintkit = 10033;
//									break;
//								case 2:
//									paintkit = 10034;
//									break;
//								case 3:
//									paintkit = 10035;
//									break;
//								}
//							}
//
//							*(int*)((DWORD)pEnt + OFFSETS::m_iItemDefinitionIndex) = ItemDefinitionIndex;
//							*(int*)((DWORD)pEnt + 0x2FB0) = -1;
//							*(int*)((DWORD)pEnt + 0x2F9C) = 4;
//							*(int*)((DWORD)pEnt + 0x2FB8) = localPlayerInfo.xuidlow;
//							*(float*)((DWORD)pEnt + 0x31B0) = 1;
//							*(int*)((DWORD)pEnt + 0x31AC) = 0;
//							*(int*)((DWORD)pEnt + 0x31B4) = -1;
//							*(int*)((DWORD)pEnt + 0x31A8) = paintkit;
//
//							pEnt->SetModelIndex(modelindex);
//							pEnt->PreDataUpdate(DATA_UPDATE_CREATED);
//						}
//					}
//				}
//
//			}
//		}
//	}
//}