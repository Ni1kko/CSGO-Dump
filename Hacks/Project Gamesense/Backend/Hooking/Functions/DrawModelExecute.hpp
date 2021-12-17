#pragma once
#include "../../Include/GlobalIncludes.h"
#include "../../Configuration/Config.h"
#include "../../Interfaces/Interfaces.h"
#include "../../SDK/CUserCmd.h"
#include "../../SDK/GlobalDefenitions.h"
#include "../../SDK/CBaseEntity.h"
#include "../../SDK/ISurface.h"
#include "../../Features/Visuals/Visuals.h"
#include "../../SDK/ModelRender.h"
#include "../../SDK/CMaterialHelper.h"

#include "../Hooks/Hooks.h"

#include <fstream>

typedef void(__thiscall* fnDrawModelExecute)(void*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
fnDrawModelExecute oDrawModelExecute = nullptr;

namespace Cheat
{
	namespace Hooked
	{
		void __fastcall DrawModelExecuteHook(void* ecx, void* edx, IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& render_info, matrix3x4_t* matrix)
		{
			if (Cheat::Settings->UnloadCheat)
			{
				Cheat::Settings->UnloadReadyTable.DrawModelExecute = 0;
				return oDrawModelExecute(ecx, context, state, render_info, matrix);
			}
			Cheat::Settings->UnloadReadyTable.DrawModelExecute = (int)GetTickCount();

			if (Interfaces::ModelRender->IsForcedMaterialOverride())
				return oDrawModelExecute(ecx, context, state, render_info, matrix);

			const char* ModelName = Interfaces::ModelInfo->GetModelName((model_t*)render_info.pModel);
			auto ent = Interfaces::EntityList->GetClientEntity(render_info.entity_index);

			//Interfaces::MaterialSystem->FindMaterial("materials/glowOverlay.vmt", TEXTURE_GROUP_OTHER)
			//Interfaces::MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER)
			static IMaterial* second = nullptr;
			static IMaterial* second1 = nullptr;
			static IMaterial* second2 = nullptr;
			static IMaterial* fakesecond = nullptr;
			static IMaterial* covered = nullptr;
			static IMaterial* open = nullptr;
			static IMaterial* backtrack = MaterialHelper->CreateMaterial(false, false);
			static IMaterial* backtrack1 = MaterialHelper->CreateMaterial(false, false);

			if (ent)
			{
				if (G::LocalPlayer->GetIndex() == ent->GetIndex())
				{
					auto localcol = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_chamslocal_color"]);
					if (G::LocalPlayer->IsScoped()) {
						Interfaces::RenderView->SetBlend(localcol.a() / 255.f / 2.f);
					}
				}

				else
					/*if ( c_config::get()->b["vis_dontrenderteam"] && ent->GetTeam() == G::LocalPlayer->GetTeam() && ent != G::LocalPlayer )
						Interfaces::RenderView->SetBlend( 0 );*/
					if (c_config::get()->b["vis_chamself"] || c_config::get()->b["vis_chamally"] || c_config::get()->b["vis_chamenemy"]) {
						

						switch (c_config::get()->i["vis_chams"])
						{
						case 0:
							covered = MaterialHelper->CoveredLit;
							break;

						case 1:
							covered = MaterialHelper->CoveredFlat;
							break;

						case 2:
							covered = Interfaces::MaterialSystem->FindMaterial("simple_ignorez_shaded.vmt", TEXTURE_GROUP_MODEL);
							break;

						case 3:
							covered = Interfaces::MaterialSystem->FindMaterial("simple_ignorez_reflective.vmt", TEXTURE_GROUP_MODEL);
							break;

						case 4:
							covered = MaterialHelper->CoveredLit;
							break;
						}

						switch (c_config::get()->i["vis_chams"])
						{
						case 0:
							open = MaterialHelper->OpenLit;
							break;

						case 1:
							open = MaterialHelper->OpenFlat;
							break;

						case 2:
							open = Interfaces::MaterialSystem->FindMaterial("simple_regular_shaded.vmt", TEXTURE_GROUP_MODEL);
							break;

						case 3:
							open = Interfaces::MaterialSystem->FindMaterial("simple_regular_reflective.vmt", TEXTURE_GROUP_MODEL);
							break;

						case 4:
							open = MaterialHelper->OpenLit;
							break;

						}

						switch (c_config::get()->i["vis_chams"])
						{
						case 2:
							second = Interfaces::MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER);
							break;

						case 4:
							second = Interfaces::MaterialSystem->FindMaterial("glowOverlay.vmt", TEXTURE_GROUP_OTHER);
							break;
							
						default:
							second = nullptr;
							break;
						}

						static IMaterial* shadow = MaterialHelper->CoveredFlat;

						auto b_IsThirdPerson = *reinterpret_cast<bool*>(reinterpret_cast<DWORD>(Interfaces::Input) + 0xA5);

						if (open != nullptr && covered != nullptr)
						{
							if (c_config::get()->b["vis_ragdolls"]) {
								if (!ent->IsAlive() && ent->GetClientClass()->m_ClassID != (int)ClassID::CCSRagdoll)
									return oDrawModelExecute(ecx, context, state, render_info, matrix);
							}
							else {
								if (!ent->IsAlive())
									return oDrawModelExecute(ecx, context, state, render_info, matrix);
							}

							if (c_config::get()->b["vis_ragdolls"]) {
								if (ent->GetClientClass()->m_ClassID != (int)ClassID::CCSPlayer && ent->GetClientClass()->m_ClassID != (int)ClassID::CCSRagdoll)
									return oDrawModelExecute(ecx, context, state, render_info, matrix);
							}
							else {
								if (ent->GetClientClass()->m_ClassID != (int)ClassID::CCSPlayer)
									return oDrawModelExecute(ecx, context, state, render_info, matrix);
							}

							if (c_playerlist::get()->should_disable_visuals(ent))
								return oDrawModelExecute(ecx, context, state, render_info, matrix);

							auto glosscol = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_pearlescent_color"]);
							bool bFound = false;
							if (c_config::get()->i["vis_chams"] == 2) {
								auto pVar = covered->find_var("$phongtint", &bFound);
								(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)pVar + 44))((uintptr_t)pVar, glosscol.r() / 255.f, glosscol.g() / 255.f, glosscol.b() / 255.f); //tfw no IMaterialVar class

								auto pVar1 = open->find_var("$phongtint", &bFound);
								(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)pVar1 + 44))((uintptr_t)pVar1, glosscol.r() / 255.f, glosscol.g() / 255.f, glosscol.b() / 255.f); //tfw no IMaterialVar class



								if (c_config::get()->f["vis_reflectivity"] < 0.1f) {
									auto pVar2 = open->find_var("$phongexponent", &bFound);
									(*(void(__thiscall**)(int, float))(*(DWORD*)pVar2 + 44))((uintptr_t)pVar2, 0.f); //tfw no IMaterialVar class
								}
								else if (c_config::get()->f["vis_reflectivity"] > 14.9f) {
									auto pVar2 = open->find_var("$phongexponent", &bFound);
									(*(void(__thiscall**)(int, float))(*(DWORD*)pVar2 + 44))((uintptr_t)pVar2, 0.1f); //tfw no IMaterialVar class
								}
								else {
									auto pVar2 = open->find_var("$phongexponent", &bFound);
									(*(void(__thiscall**)(int, float))(*(DWORD*)pVar2 + 44))((uintptr_t)pVar2, 15.f - c_config::get()->f["vis_reflectivity"]); //tfw no IMaterialVar class
								}

								auto pVar3 = open->find_var("$phongboost", &bFound);
								(*(void(__thiscall**)(int, float))(*(DWORD*)pVar3 + 44))((uintptr_t)pVar3, c_config::get()->f["vis_shine"]); //tfw no IMaterialVar class

								auto pVar4 = open->find_var("$rimlightboost", &bFound);
								(*(void(__thiscall**)(int, float))(*(DWORD*)pVar4 + 44))((uintptr_t)pVar4, c_config::get()->f["vis_rim"]); //tfw no IMaterialVar class


								if (c_config::get()->f["vis_reflectivity"] < 0.1f) {
									auto pVar5 = covered->find_var("$phongexponent", &bFound);
									(*(void(__thiscall**)(int, float))(*(DWORD*)pVar5 + 44))((uintptr_t)pVar5, 0.f); //tfw no IMaterialVar class
								}
								else if (c_config::get()->f["vis_reflectivity"] < 14.9f) {
									auto pVar5 = covered->find_var("$phongexponent", &bFound);
									(*(void(__thiscall**)(int, float))(*(DWORD*)pVar5 + 44))((uintptr_t)pVar5, 0.1f); //tfw no IMaterialVar class
								}
								else {
									auto pVar5 = covered->find_var("$phongexponent", &bFound);
									(*(void(__thiscall**)(int, float))(*(DWORD*)pVar5 + 44))((uintptr_t)pVar5, 15.f - c_config::get()->f["vis_reflectivity"]); //tfw no IMaterialVar class
								}
								

								auto pVar6 = covered->find_var("$phongboost", &bFound);
								(*(void(__thiscall**)(int, float))(*(DWORD*)pVar6 + 44))((uintptr_t)pVar6, c_config::get()->f["vis_shine"]); //tfw no IMaterialVar class

								auto pVar7 = covered->find_var("$rimlightexponent", &bFound);
								(*(void(__thiscall**)(int, float))(*(DWORD*)pVar7 + 44))((uintptr_t)pVar7, c_config::get()->f["vis_rim"]); //tfw no IMaterialVar class
							}

							if (c_config::get()->i["vis_chams"] == 4) {
								auto pVar = second->find_var("$envmaptint", &bFound);
								(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)pVar + 44))((uintptr_t)pVar, glosscol.r() / 255.f, glosscol.g() / 255.f, glosscol.b() / 255.f); //tfw no IMaterialVar class

								auto pVar2 = second->find_var("$alpha", &bFound);
								(*(void(__thiscall**)(int, float))(*(DWORD*)pVar2 + 44))((uintptr_t)pVar2, glosscol.a() / 255.f); //tfw no IMaterialVar class
							}

							if (c_config::get()->i["vis_chams"] == 3) {
								auto pVar1 = covered->find_var("$envmaptint", &bFound);
								(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)pVar1 + 44))((uintptr_t)pVar1, glosscol.r() / 255.f, glosscol.g() / 255.f, glosscol.b() / 255.f); //tfw no IMaterialVar class
								auto pVar2 = open->find_var("$envmaptint", &bFound);
								(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)pVar2 + 44))((uintptr_t)pVar2, glosscol.r() / 255.f, glosscol.g() / 255.f, glosscol.b() / 255.f); //tfw no IMaterialVar class
							}
							

							/*if (ent != G::LocalPlayer && ent->GetTeam() == G::LocalPlayer->GetTeam() && c_config::get()->b["vis_chamally"])
							{
								if (ent->IsAlive() && !(std::string(ModelName)).find("models/player"))
								{
									auto col = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_chamally_color"]);
									Interfaces::RenderView->SetColorModulation(col);
									Interfaces::RenderView->SetBlend(col.a() / 255.f);
									Interfaces::ModelRender->ForcedMaterialOverride(open);
									oDrawModelExecute(ecx, context, state, render_info, matrix);
									Interfaces::ModelRender->ForcedMaterialOverride(pMatGloss);
									oDrawModelExecute(ecx, context, state, render_info, matrix);
									Interfaces::ModelRender->ForcedMaterialOverride(nullptr);
								}
							}*/

							if (ent && ent->GetClientClass()->m_ClassID == (int)ClassID::CCSRagdoll && c_config::get()->b["vis_ragdolls"]) {
								auto col1 = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_chamenemy_color"]);
								Interfaces::RenderView->SetColorModulation(col1);
								Interfaces::RenderView->SetBlend(col1.a() / 255.f);
								Interfaces::ModelRender->ForcedMaterialOverride(open);
								oDrawModelExecute(ecx, context, state, render_info, matrix);
								if (c_config::get()->i["vis_chams"] == 4) {
									second->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
									Interfaces::ModelRender->ForcedMaterialOverride(second);
									oDrawModelExecute(ecx, context, state, render_info, matrix);
								}

								Interfaces::ModelRender->ForcedMaterialOverride(nullptr);
							}

							if (ent != G::LocalPlayer && ent->GetTeam() != G::LocalPlayer->GetTeam() && (c_config::get()->b["vis_chamenemy"] || c_config::get()->b["vis_chamenemy_xqz"]))
							{
								if (!(std::string(ModelName)).find("models/player")) {
									if (c_config::get()->b["vis_chamshadow"]) {

										Vector orig = ent->GetAbsOrigin();

										if (g_BacktrackHelper->PlayerRecord[render_info.entity_index].records.size() > 0) {

											tick_record record = g_BacktrackHelper->PlayerRecord[render_info.entity_index].records.at(0);

											if (orig != record.m_vecAbsOrigin) {

												auto col = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_chamshadow_color"]);
												Interfaces::RenderView->SetColorModulation(col);

												Interfaces::RenderView->SetBlend(col.a() / 255.f);
												if (c_config::get()->b["vis_chamenemy_xqz"])
													backtrack->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
												else
													backtrack->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
												Interfaces::ModelRender->ForcedMaterialOverride(backtrack);
												oDrawModelExecute(ecx, context, state, render_info, record.boneMatrix);
												Interfaces::ModelRender->ForcedMaterialOverride(nullptr);
												
											}
										}
									}

									if (c_config::get()->b["vis_chamenemy_xqz"] && !(std::string(ModelName)).find("models/player") && ent->IsAlive()) {
										auto col = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_chamenemy_xqz_color"]);
										Interfaces::RenderView->SetColorModulation(col);
										Interfaces::RenderView->SetBlend(col.a() / 255.f);
										Interfaces::ModelRender->ForcedMaterialOverride(covered);
										oDrawModelExecute(ecx, context, state, render_info, matrix);
										if (c_config::get()->i["vis_chams"] == 4) {
											second->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
											Interfaces::ModelRender->ForcedMaterialOverride(second);
											oDrawModelExecute(ecx, context, state, render_info, matrix);
										}
										Interfaces::ModelRender->ForcedMaterialOverride(nullptr);
										
									}

									if (c_config::get()->b["vis_chamenemy"] && !(std::string(ModelName)).find("models/player") && ent->IsAlive()) {
										auto col1 = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_chamenemy_color"]);
										Interfaces::RenderView->SetColorModulation(col1);
										Interfaces::RenderView->SetBlend(col1.a() / 255.f);
										Interfaces::ModelRender->ForcedMaterialOverride(open);
										oDrawModelExecute(ecx, context, state, render_info, matrix);
										if (c_config::get()->i["vis_chams"] == 4) {
											second->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
											Interfaces::ModelRender->ForcedMaterialOverride(second);
											oDrawModelExecute(ecx, context, state, render_info, matrix);
										}

										Interfaces::ModelRender->ForcedMaterialOverride(nullptr);
										

									}


									/*if (c_config::get()->b["vis_chamshands"] && !(std::string(ModelName)).find("arms")) {
										auto col5 = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_hands_color"]);
										Interfaces::RenderView->SetColorModulation(col5);
										Interfaces::RenderView->SetBlend(col5.a() / 255.f);
										Interfaces::ModelRender->ForcedMaterialOverride(open);
										oDrawModelExecute(ecx, context, state, render_info, matrix);
										if (c_config::get()->i["vis_chams"] == 4 || c_config::get()->i["vis_chams"] == 2) {
											second->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
											Interfaces::ModelRender->ForcedMaterialOverride(second);
											oDrawModelExecute(ecx, context, state, render_info, matrix);
										}

										Interfaces::ModelRender->ForcedMaterialOverride(nullptr);

									}*/

									/*if (ent->GetTeam() == G::LocalPlayer->GetTeam())
									{
										if (ent == G::LocalPlayer) {

											if (c_config::get()->b["vis_chamally"]) {
												auto col1 = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_chamenemy_color"]);
												Interfaces::RenderView->SetColorModulation(col1);
												Interfaces::RenderView->SetBlend(col1.a() / 255.f);
												Interfaces::ModelRender->ForcedMaterialOverride(open);
												oDrawModelExecute(ecx, context, state, render_info, matrix);
												pMatGloss->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
												Interfaces::ModelRender->ForcedMaterialOverride(pMatGloss);
												oDrawModelExecute(ecx, context, state, render_info, matrix);
												Interfaces::ModelRender->ForcedMaterialOverride(nullptr);
											}
											else {
										
										oDrawModelExecute(ecx, context, state, render_info, matrix);
										}

									}*/

								}
							}


						}
					}
			}

			for (auto hk : c_lua::get()->hooks->getHooks("on_draw_model_execute"))
				hk.func(render_info);

			if (std::string(ModelName).find("arms") != std::string::npos) {
				if (!c_config::get()->b["vis_chamshands"]) {
					oDrawModelExecute(ecx, context, state, render_info, matrix);
				}
				else {
					static IMaterial* gloves = nullptr;
					switch (c_config::get()->i["vis_chamshandstype"])
					{
					case 0:
						gloves = MaterialHelper->OpenLit;
						break;

					case 1:
						gloves = MaterialHelper->OpenFlat;
						break;

					case 2:
						gloves = Interfaces::MaterialSystem->FindMaterial("simple_regular_shaded.vmt", TEXTURE_GROUP_MODEL);
						break;

					case 3:
						gloves = Interfaces::MaterialSystem->FindMaterial("simple_regular_reflective.vmt", TEXTURE_GROUP_MODEL);
						break;

					case 4:
						gloves = MaterialHelper->OpenLit;
						break;

					case 5:
						gloves = Interfaces::MaterialSystem->FindMaterial("models/props_interiors/tvebtest", TEXTURE_GROUP_MODEL);
						break;
					}
					switch (c_config::get()->i["vis_chamshandstype"])
					{
					case 2:
						second1 = Interfaces::MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER);
						break;

					case 4:
						second1 = Interfaces::MaterialSystem->FindMaterial("glowOverlay.vmt", TEXTURE_GROUP_OTHER);
						break;

					default:
						second1 = nullptr;
						break;
					}
					if (c_config::get()->i["vis_chamshandstype"] == 4 || c_config::get()->i["vis_chamshandstype"] == 2) {
						if (second1) {
							auto glosscol = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_pearlescenthands_color"]);
							bool bFound;
							auto pVar = second1->find_var("$envmaptint", &bFound);
							(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)pVar + 44))((uintptr_t)pVar, glosscol.r() / 255.f, glosscol.g() / 255.f, glosscol.b() / 255.f); //tfw no IMaterialVar class

							auto pVar2 = second1->find_var("$alpha", &bFound);
							(*(void(__thiscall**)(int, float))(*(DWORD*)pVar2 + 44))((uintptr_t)pVar2, glosscol.a() / 255.f); //tfw no IMaterialVar class
						}
					}

					if (c_config::get()->i["vis_chamshandstype"] == 3) {
						auto glosscol = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_pearlescenthands_color"]);
						bool bFound;
						auto pVar1 = gloves->find_var("$envmaptint", &bFound);
						(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)pVar1 + 44))((uintptr_t)pVar1, glosscol.r() / 255.f, glosscol.g() / 255.f, glosscol.b() / 255.f); //tfw no IMaterialVar class
					}
					auto col5 = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_hands_color"]);
					Interfaces::RenderView->SetColorModulation(col5);
					Interfaces::RenderView->SetBlend(col5.a() / 255.f);
					Interfaces::ModelRender->ForcedMaterialOverride(gloves);
					oDrawModelExecute(ecx, context, state, render_info, matrix);
					if (c_config::get()->i["vis_chamshandstype"] == 4) {
						second1->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						Interfaces::ModelRender->ForcedMaterialOverride(second1);
						oDrawModelExecute(ecx, context, state, render_info, matrix);
					}

					Interfaces::ModelRender->ForcedMaterialOverride(nullptr);
					
				}

			}

			if ((std::string(ModelName)).find("models/player")) {
				if (std::string(ModelName).find("arms") == std::string::npos)
					oDrawModelExecute(ecx, context, state, render_info, matrix);
			}
			else {
				if (!c_config::get()->b["vis_chamenemy"]) {
					oDrawModelExecute(ecx, context, state, render_info, matrix);
				}
				else if (ent == G::LocalPlayer) {
					if (c_config::get()->b["vis_fakeangles"]) {
						static IMaterial* fake = nullptr;
						switch (c_config::get()->i["vis_chamsfake"])
						{
						case 0:
							fake = MaterialHelper->OpenLit;
							break;

						case 1:
							fake = MaterialHelper->OpenFlat;
							break;

						case 2:
							fake = Interfaces::MaterialSystem->FindMaterial("simple_regular_shaded.vmt", TEXTURE_GROUP_MODEL);
							break;

						case 3:
							fake = Interfaces::MaterialSystem->FindMaterial("simple_regular_reflective.vmt", TEXTURE_GROUP_MODEL);
							break;

						case 4:
							fake = MaterialHelper->OpenLit;
							break;
						case 5:
							fake = Interfaces::MaterialSystem->FindMaterial("models/props_interiors/tvebtest", TEXTURE_GROUP_MODEL);
							break;
						}
						switch (c_config::get()->i["vis_chamsfake"])
						{
						case 2:
							fakesecond = Interfaces::MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER);
							break;

						case 4:
							fakesecond = Interfaces::MaterialSystem->FindMaterial("glowOverlay.vmt", TEXTURE_GROUP_OTHER);
							break;

						default:
							fakesecond = nullptr;
							break;
						}
						if (c_config::get()->i["vis_chamsfake"] == 4) {
							auto glosscol = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_pearlescentfake_color"]);
							bool bFound;
							auto pVar = fakesecond->find_var("$envmaptint", &bFound);
							(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)pVar + 44))((uintptr_t)pVar, glosscol.r() / 255.f, glosscol.g() / 255.f, glosscol.b() / 255.f); //tfw no IMaterialVar class

							auto pVar2 = fakesecond->find_var("$alpha", &bFound);
							(*(void(__thiscall**)(int, float))(*(DWORD*)pVar2 + 44))((uintptr_t)pVar2, glosscol.a() / 255.f); //tfw no IMaterialVar class
						}

						if (c_config::get()->i["vis_chamsfake"] == 3) {
							auto glosscol = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_pearlescentfake_color"]);
							bool bFound;
							auto pVar1 = fake->find_var("$envmaptint", &bFound);
							(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)pVar1 + 44))((uintptr_t)pVar1, glosscol.r() / 255.f, glosscol.g() / 255.f, glosscol.b() / 255.f); //tfw no IMaterialVar class
						}
						auto col = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_fakeangles_color"]);


						for (auto& i : G::FakeMatrix)
						{
							i[0][3] += render_info.origin.x;
							i[1][3] += render_info.origin.y;
							i[2][3] += render_info.origin.z;
						}
						Interfaces::RenderView->SetColorModulation(col);
						Interfaces::RenderView->SetBlend(col.a() / 255.f);
						Interfaces::ModelRender->ForcedMaterialOverride(fake);
						oDrawModelExecute(ecx, context, state, render_info, G::FakeMatrix);
						if (c_config::get()->i["vis_chamsfake"] == 4) {
							fakesecond->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
							Interfaces::ModelRender->ForcedMaterialOverride(fakesecond);
							oDrawModelExecute(ecx, context, state, render_info, G::FakeMatrix);
						}
						Interfaces::ModelRender->ForcedMaterialOverride(nullptr);

						for (auto& i : G::FakeMatrix)
						{
							i[0][3] -= render_info.origin.x;
							i[1][3] -= render_info.origin.y;
							i[2][3] -= render_info.origin.z;
						}
					}
					if (c_config::get()->b["vis_chamslocal"])
					{
						static IMaterial* self = nullptr;
						switch (c_config::get()->i["vis_chamslocaltype"])
						{
						case 0:
							self = MaterialHelper->OpenLit;
							break;

						case 1:
							self = MaterialHelper->OpenFlat;
							break;

						case 2:
							self = Interfaces::MaterialSystem->FindMaterial("simple_regular_shaded.vmt", TEXTURE_GROUP_MODEL);
							break;

						case 3:
							self = Interfaces::MaterialSystem->FindMaterial("simple_regular_reflective.vmt", TEXTURE_GROUP_MODEL);
							break;

						case 4:
							self = MaterialHelper->OpenLit;
							break;
						case 5:
							self = Interfaces::MaterialSystem->FindMaterial("models/props_interiors/tvebtest", TEXTURE_GROUP_MODEL);
							break;
						}
						switch (c_config::get()->i["vis_chamslocaltype"])
						{
						case 2:
							second2 = Interfaces::MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER);
							break;

						case 4:
							second2 = Interfaces::MaterialSystem->FindMaterial("glowOverlay.vmt", TEXTURE_GROUP_OTHER);
							break;

						default:
							second2 = nullptr;
							break;
						}

						if (c_config::get()->i["vis_chamslocaltype"] == 4) {
							auto glosscol321213 = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_pearlescentlocal_color"]);
							bool bFound;
							auto pVar = second2->find_var("$envmaptint", &bFound);
							(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)pVar + 44))((uintptr_t)pVar, glosscol321213.r() / 255.f, glosscol321213.g() / 255.f, glosscol321213.b() / 255.f); //tfw no IMaterialVar class

							auto pVar2 = second2->find_var("$alpha", &bFound);
							(*(void(__thiscall**)(int, float))(*(DWORD*)pVar2 + 44))((uintptr_t)pVar2, glosscol321213.a() / 255.f); //tfw no IMaterialVar class
						}

						if (c_config::get()->i["vis_chamslocaltype"] == 3) {
							auto glosscol = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_pearlescentlocal_color"]);
							bool bFound;
							auto pVar1 = self->find_var("$envmaptint", &bFound);
							(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)pVar1 + 44))((uintptr_t)pVar1, glosscol.r() / 255.f, glosscol.g() / 255.f, glosscol.b() / 255.f); //tfw no IMaterialVar class
						}
						auto col3341234123 = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_chamslocal_color"]);
						Interfaces::RenderView->SetColorModulation(col3341234123);
						Interfaces::RenderView->SetBlend(col3341234123.a() / 255.f);
						Interfaces::ModelRender->ForcedMaterialOverride(self);
						oDrawModelExecute(ecx, context, state, render_info, matrix);
						if (c_config::get()->i["vis_chamslocaltype"] == 4) {
							second2->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
							Interfaces::ModelRender->ForcedMaterialOverride(second2);
							oDrawModelExecute(ecx, context, state, render_info, matrix);
						}
						Interfaces::ModelRender->ForcedMaterialOverride(nullptr);
					}
					else {
						float col[3] = { 1.f, 1.f, 1.f };
						Interfaces::RenderView->SetColorModulation(col);
						Interfaces::RenderView->SetBlend(1.f);
						Interfaces::ModelRender->ForcedMaterialOverride(nullptr);
						oDrawModelExecute(ecx, context, state, render_info, matrix);
					}
				}
				if (ent->GetTeam() == G::LocalPlayer->GetTeam() && ent != G::LocalPlayer) {
					if (!c_config::get()->b["vis_chamally"]) {
						oDrawModelExecute(ecx, context, state, render_info, matrix);
					}
					else {
						static IMaterial* self = nullptr;
						static IMaterial* self2 = nullptr;
						switch (c_config::get()->i["vis_chamsally"])
						{
						case 0:
							self = MaterialHelper->OpenLit;
							break;

						case 1:
							self = MaterialHelper->OpenFlat;
							break;

						case 2:
							self = Interfaces::MaterialSystem->FindMaterial("simple_regular_shaded.vmt", TEXTURE_GROUP_MODEL);
							break;

						case 3:
							self = Interfaces::MaterialSystem->FindMaterial("simple_regular_reflective.vmt", TEXTURE_GROUP_MODEL);
							break;

						case 4:
							self = MaterialHelper->OpenLit;
							break;
						case 5:
							self = Interfaces::MaterialSystem->FindMaterial("models/props_interiors/tvebtest", TEXTURE_GROUP_MODEL);
							break;
						}
						switch (c_config::get()->i["vis_chamsally"])
						{
						case 2:
							second2 = Interfaces::MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER);
							break;

						case 4:
							second2 = Interfaces::MaterialSystem->FindMaterial("glowOverlay.vmt", TEXTURE_GROUP_OTHER);
							break;

						default:
							second2 = nullptr;
							break;
						}
						switch (c_config::get()->i["vis_chamsally"])
						{
						case 0:
							self2 = MaterialHelper->OpenLit;
							break;

						case 1:
							self2 = MaterialHelper->OpenFlat;
							break;

						case 2:
							self2 = Interfaces::MaterialSystem->FindMaterial("simple_regular_shaded.vmt", TEXTURE_GROUP_MODEL);
							break;

						case 3:
							self2 = Interfaces::MaterialSystem->FindMaterial("simple_regular_reflective.vmt", TEXTURE_GROUP_MODEL);
							break;

						case 4:
							self2 = MaterialHelper->OpenLit;
							break;
						case 5:
							self = Interfaces::MaterialSystem->FindMaterial("models/props_interiors/tvebtest", TEXTURE_GROUP_MODEL);
							break;
						}
						
						if (c_config::get()->i["vis_chamsally"] == 4 || c_config::get()->i["vis_chamsally"] == 2) {
							auto glosscol = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_pearlescentally_color"]);
							bool bFound;
							auto pVar = second2->find_var("$envmaptint", &bFound);
							(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)pVar + 44))((uintptr_t)pVar, glosscol.r() / 255.f, glosscol.g() / 255.f, glosscol.b() / 255.f); //tfw no IMaterialVar class

							auto pVar2 = second2->find_var("$alpha", &bFound);
							(*(void(__thiscall**)(int, float))(*(DWORD*)pVar2 + 44))((uintptr_t)pVar2, glosscol.a() / 255.f); //tfw no IMaterialVar class
						}

						if (c_config::get()->i["vis_chamsally"] == 3) {
							auto glosscol = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_pearlescentally_color"]);
							bool bFound;
							auto pVar1 = self->find_var("$envmaptint", &bFound);
							(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)pVar1 + 44))((uintptr_t)pVar1, glosscol.r() / 255.f, glosscol.g() / 255.f, glosscol.b() / 255.f); //tfw no IMaterialVar class
						}
						if (c_config::get()->b["vis_chamally_xqz"]) {
							auto col = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_chamallyxqz_color"]);
							Interfaces::RenderView->SetColorModulation(col);
							Interfaces::RenderView->SetBlend(col.a() / 255.f);
							self2->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
							Interfaces::ModelRender->ForcedMaterialOverride(self2);
							oDrawModelExecute(ecx, context, state, render_info, matrix);
							if (c_config::get()->i["vis_chamsally"] == 4) {
								second2->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
								Interfaces::ModelRender->ForcedMaterialOverride(second2);
								oDrawModelExecute(ecx, context, state, render_info, matrix);
							}
						}
						auto col3 = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_chamally_color"]);
						Interfaces::RenderView->SetColorModulation(col3);
						Interfaces::RenderView->SetBlend(col3.a() / 255.f);
						Interfaces::ModelRender->ForcedMaterialOverride(self);
						oDrawModelExecute(ecx, context, state, render_info, matrix);
						if (c_config::get()->i["vis_chamsally"] == 4) {
							second2->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
							Interfaces::ModelRender->ForcedMaterialOverride(second2);
							oDrawModelExecute(ecx, context, state, render_info, matrix);
						}
						Interfaces::ModelRender->ForcedMaterialOverride(nullptr);
					}
				}
			}



		}
	}
}