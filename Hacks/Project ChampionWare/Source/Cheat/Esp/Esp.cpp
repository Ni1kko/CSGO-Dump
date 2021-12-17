#include "Esp.h"
#include "../../Engine/Render.h"
#include "../../SDK/beambullets.h"
#include "../../SDK/BeamsInclude.h"
#include "../../Engine/EventLogs.h"






std::vector<cbullet_tracer_info> logs;

SDK::PlayerInfo GetInfo(int Index)
{
	SDK::PlayerInfo info;
	Interfaces::Engine()->GetPlayerInfo(Index, &info);
	return info;
}

char* HitgroupToName(int hitgroup)
{
	switch (hitgroup)
	{
	case HITGROUP_HEAD:
		return "head";
	case HITGROUP_LEFTLEG:
		return "leg";
	case HITGROUP_RIGHTLEG:
		return "leg";
	case HITGROUP_LEFTARM:
		return "arm";
	case HITGROUP_RIGHTARM:
		return "arm";
	case HITGROUP_STOMACH:
		return "chest";
	default:
		return "chest";
	}
}

using namespace Client;
//[enc_string_enable /]
//[junk_enable /]

CEsp::CEsp()
{
	// Öâåòà õï áàðà

	CT_HP_ColorM.SetColor(255, 64, 64);
	TT_HP_ColorM.SetColor(255, 64, 64);

	// Öâåòà àðìîð áàðà

	CT_AR_ColorM.SetColor(255, 64, 64);
	TT_AR_ColorM.SetColor(255, 64, 64);

	visible_flat = nullptr;
	visible_tex = nullptr;
	hidden_flat = nullptr;
	hidden_tex = nullptr;

	fExplodeC4Timer = 0.f;
	fC4Timer = 0.f;
}

Color CEsp::GetPlayerColor(CPlayer* pPlayer)
{
	Color CT_Color(int(Settings::Esp::esp_Color_CT[0] * 255.f), int(Settings::Esp::esp_Color_CT[1] * 255.f), int(Settings::Esp::esp_Color_CT[2] * 255.f));
	Color TT_Color(int(Settings::Esp::esp_Color_TT[0] * 255.f), int(Settings::Esp::esp_Color_TT[1] * 255.f), int(Settings::Esp::esp_Color_TT[2] * 255.f));

	if (pPlayer->Team == TEAM_CT)
	{
		return CT_Color;
	}
	else if (pPlayer->Team == TEAM_TT)
	{
		return TT_Color;
	}

	return Color::White();
}

Color CEsp::GetPlayerVisibleColor(CPlayer* pPlayer)
{
	Color PlayerColor = GetPlayerColor(pPlayer);

	bool SetColor = false;

	// Ïîäñâå÷èâàòü çåë¸íûì åñëè èãðîê âèäåí:

	if (pPlayer->bVisible)
	{
		if (Settings::Esp::esp_Visible == 1) // Âñåõ
		{
			SetColor = true;
		}

		Color VCT_Color(int(Settings::Esp::esp_Color_VCT[0] * 255.f), int(Settings::Esp::esp_Color_VCT[1] * 255.f), int(Settings::Esp::esp_Color_VCT[2] * 255.f));
		Color VTT_Color(int(Settings::Esp::esp_Color_VTT[0] * 255.f), int(Settings::Esp::esp_Color_VTT[1] * 255.f), int(Settings::Esp::esp_Color_VTT[2] * 255.f));

		if (SetColor)
		{
			if (pPlayer->Team == TEAM_CT)
			{
				PlayerColor = VCT_Color;
			}
			else if (pPlayer->Team == TEAM_TT)
			{
				PlayerColor = VTT_Color;
			}
		}
	}

	return PlayerColor;
}

bool CEsp::CheckPlayerTeam(CPlayer* pPlayer)
{
	bool CheckTeam = false;
	bool PlayerVisible = pPlayer->bVisible;

	// Ïîêàçûâàåì esp òîëüêî íà:
	if (Settings::Esp::esp_Enemy && pPlayer->Team != g_pPlayers->GetLocal()->Team) // Ïðîòèâíèêîâ
		CheckTeam = true;

	if (Settings::Esp::esp_Team && pPlayer->Team == g_pPlayers->GetLocal()->Team) // Ñâîèõ
		CheckTeam = true;

	if (Settings::Esp::esp_Visible == 0 && !PlayerVisible)
		CheckTeam = false;

	return CheckTeam;
}

void CEsp::Dlight(CPlayer* pPlayer)
{
	if (Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
	{
		Vector getorig = pPlayer->m_pEntity->GetOrigin();
		Vector getheadorig = pPlayer->m_pEntity->GetEyePosition();
		Color EspVisibleColor = GetPlayerVisibleColor(pPlayer);
		dlight_t* pElight = Interfaces::Effects()->CL_AllocElight(pPlayer->m_pEntity->EntIndex());
		pElight->color.r = float(Settings::Esp::esp_Dlight[0] * 255.f);
		pElight->color.g = float(Settings::Esp::esp_Dlight[1] * 255.f);;
		pElight->color.b = float(Settings::Esp::esp_Dlight[2] * 255.f);;
		pElight->color.exponent = 8.f;
		pElight->m_Direction = getheadorig;
		pElight->origin = getheadorig;
		pElight->radius = 200.0f;
		pElight->die = Interfaces::GlobalVars()->curtime + 0.1f;
		pElight->decay = 25.0f;
		pElight->key = pPlayer->m_pEntity->EntIndex();
		dlight_t* pDlight = Interfaces::Effects()->CL_AllocDlight(pPlayer->m_pEntity->EntIndex());
		pDlight->color.r = float(Settings::Esp::esp_Dlight[0] * 255.f);;
		pDlight->color.g = float(Settings::Esp::esp_Dlight[1] * 255.f);;
		pDlight->color.b = float(Settings::Esp::esp_Dlight[2] * 255.f);;
		pDlight->color.exponent = 8.f;
		pDlight->m_Direction = getorig;
		pDlight->origin = getorig;
		pDlight->radius = 100.f;
		pDlight->die = Interfaces::GlobalVars()->curtime + 0.1f;
		pDlight->decay = pDlight->radius / 5.f;
		pDlight->key = pPlayer->m_pEntity->EntIndex();
	}
}

void CEsp::Ambient()
{
	if (Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
	{
		float AmbientRedAmount = Settings::Esp::esp_Ambient[0];
		float AmbientGreenAmount = Settings::Esp::esp_Ambient[1];
		float AmbientBlueAmount = Settings::Esp::esp_Ambient[2];

		ConVar* AmbientRedCvar = Interfaces::GetConVar()->FindVar("mat_ambient_light_r");
		*(float*)((DWORD)&AmbientRedCvar->fnChangeCallback + 0xC) = NULL;
		AmbientRedCvar->SetValue(AmbientRedAmount);

		ConVar* AmbientGreenCvar = Interfaces::GetConVar()->FindVar("mat_ambient_light_g");
		*(float*)((DWORD)&AmbientGreenCvar->fnChangeCallback + 0xC) = NULL;
		AmbientGreenCvar->SetValue(AmbientGreenAmount);

		ConVar* AmbientBlueCvar = Interfaces::GetConVar()->FindVar("mat_ambient_light_b");
		*(float*)((DWORD)&AmbientBlueCvar->fnChangeCallback + 0xC) = NULL;
		AmbientBlueCvar->SetValue(AmbientBlueAmount);
	}
}

void CEsp::HitEvents(IGameEvent* event)
{
	if (!Settings::Esp::esp_hitevent)
		return;

	if (!strcmp(event->GetName(), "player_hurt"))
	{
		int attackerid = event->GetInt("attacker");
		int entityid = Interfaces::Engine()->GetPlayerForUserID(attackerid);
		if (entityid == Interfaces::Engine()->GetLocalPlayer())
		{
			int nUserID = event->GetInt("attacker");
			int nDead = event->GetInt("userid");
			if (nUserID || nDead)
			{
				SDK::PlayerInfo killed_info = GetInfo(Interfaces::Engine()->GetPlayerForUserID(nDead));
				EventLog->AddToLog("Damaged %s in the %s for %i damage (%i health left)",
					killed_info.m_szPlayerName,
					HitgroupToName(event->GetInt("hitgroup")),
					event->GetInt("dmg_health"),
					event->GetInt("health"));
			}
		}
	}
}

bool nightz = false;
void CEsp::NightMode()
{
	if (Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
	{
		if (Settings::Esp::esp_NightMode)
		{
			if (!nightz)
			{
				static auto sv_skyname = Interfaces::GetConVar()->FindVar("sv_skyname");
				static auto r_DrawSpecificStaticProp = Interfaces::GetConVar()->FindVar("r_DrawSpecificStaticProp");
				r_DrawSpecificStaticProp->SetValue(1);
				sv_skyname->SetValue("sky_csgo_night02");

				for (MaterialHandle_t i = Interfaces::MaterialSystem()->FirstMaterial(); i != Interfaces::MaterialSystem()->InvalidMaterial(); i = Interfaces::MaterialSystem()->NextMaterial(i))
				{
					IMaterial *pMaterial = Interfaces::MaterialSystem()->GetMaterial(i);

					if (!pMaterial)
						continue;

					const char* group = pMaterial->GetTextureGroupName();
					const char* name = pMaterial->GetName();

					if (strstr(group, "World textures"))
					{
						pMaterial->ColorModulate(0.10, 0.10, 0.10);
					}
					if (strstr(group, "StaticProp"))
					{
						pMaterial->ColorModulate(0.30, 0.30, 0.30);
					}
					if (strstr(name, "models/props/de_dust/palace_bigdome"))
					{
						pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
					}
					if (strstr(name, "models/props/de_dust/palace_pillars"))
					{
						pMaterial->ColorModulate(0.30, 0.30, 0.30);
					}

					if (strstr(group, "Particle textures"))
					{
						pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
					}
					nightz = true;
				}

			}

		}
		else
		{
			if (nightz)
			{
				for (MaterialHandle_t i = Interfaces::MaterialSystem()->FirstMaterial(); i != Interfaces::MaterialSystem()->InvalidMaterial(); i = Interfaces::MaterialSystem()->NextMaterial(i))
				{
					IMaterial *pMaterial = Interfaces::MaterialSystem()->GetMaterial(i);

					if (!pMaterial)
						continue;

					const char* group = pMaterial->GetTextureGroupName();
					const char* name = pMaterial->GetName();

					if (strstr(group, "World textures"))
					{

						pMaterial->ColorModulate(1, 1, 1);
					}
					if (strstr(group, "StaticProp"))
					{

						pMaterial->ColorModulate(1, 1, 1);
					}
					if (strstr(name, "models/props/de_dust/palace_bigdome"))
					{
						pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
					}
					if (strstr(name, "models/props/de_dust/palace_pillars"))
					{

						pMaterial->ColorModulate(1, 1, 1);
					}
					if (strstr(group, "Particle textures"))
					{
						pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
					}
				}

				nightz = false;
			}
		}
	}
}


void CEsp::HitmarkerEvents(IGameEvent* event)
{
	if (Settings::Misc::misc_HitMarker & !strcmp(event->GetName(), "player_hurt"))
	{
		int attacker = event->GetInt("attacker");
		if (Interfaces::Engine()->GetPlayerForUserID(attacker) == Interfaces::Engine()->GetLocalPlayer()) {
			switch (Settings::Misc::misc_HitMarkerSound)
			{
			case 0: break;
			case 1: PlaySoundA(rawData, NULL, SND_ASYNC | SND_MEMORY); break;
			case 2: PlaySoundA(pew, NULL, SND_ASYNC | SND_MEMORY); break;
			case 3: PlaySoundA(roblox, NULL, SND_ASYNC | SND_MEMORY); break;
			case 4: PlaySoundA(skeethitmarker_wav, NULL, SND_ASYNC | SND_MEMORY); break;
			case 5: PlaySoundA(hitler_wav, NULL, SND_ASYNC | SND_MEMORY); break;
			case 6: PlaySoundA(headmeme, NULL, SND_ASYNC | SND_MEMORY); break;
			case 7: PlaySoundA(FadeCSGO, NULL, SND_ASYNC | SND_MEMORY); break;
			}
			Settings::hitmarkerAlpha = 1.f;
		}
	}

	if (Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
	{
		if (Settings::Esp::esp_beams_tracer & !strcmp(event->GetName(), "bullet_impact"))
		{
			CBaseEntity* LocalPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

			if (LocalPlayer)
			{
				auto index = Interfaces::Engine()->GetPlayerForUserID(event->GetInt("userid"));

				if (index != Interfaces::Engine()->GetLocalPlayer())
					return;

				auto local = static_cast<CBaseEntity*>(Interfaces::EntityList()->GetClientEntity(index));

				Vector position(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));

				Ray_t ray;

				ray.Init(local->GetEyePosition(), position);

				CTraceFilter filter;

				filter.pSkip = local;

				trace_t tr;
				Interfaces::EngineTrace()->TraceRay(ray, MASK_SHOT, &filter, &tr);

				logs.push_back(cbullet_tracer_info(local->GetEyePosition(), position, Interfaces::GlobalVars()->curtime, Color(255, 255, 255, 255)));

				if (!local)
					return;

				for (size_t i = 0; i < logs.size(); i++)
				{
					auto current = logs.at(i);

					current.color = Color(int(Settings::Esp::flTracer_Beam[0] * 255.f),
						int(Settings::Esp::flTracer_Beam[1] * 255.f),
						int(Settings::Esp::flTracer_Beam[2] * 255.f)); //color of local player's tracers
					BeamInfo_t beamInfo;
					beamInfo.m_nType = TE_BEAMPOINTS;
					beamInfo.m_pszModelName = "sprites/physbeam.vmt";
					beamInfo.m_nModelIndex = -1;
					beamInfo.m_flHaloScale = 0.0f;
					beamInfo.m_flLife = 3.0f;
					beamInfo.m_flWidth = 2.0f;
					beamInfo.m_flEndWidth = 2.0f;
					beamInfo.m_flFadeLength = 0.0f;
					beamInfo.m_flAmplitude = 2.0f;
					beamInfo.m_flBrightness = 255.f;
					beamInfo.m_flSpeed = 0.5f;
					beamInfo.m_nStartFrame = 0;
					beamInfo.m_flFrameRate = 0.f;
					beamInfo.m_flRed = current.color.r();
					beamInfo.m_flGreen = current.color.g();
					beamInfo.m_flBlue = current.color.b();
					beamInfo.m_nSegments = 2;
					beamInfo.m_bRenderable = true;
					beamInfo.m_nFlags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM;



					


					beamInfo.m_vecStart = LocalPlayer->GetEyePosition();
					beamInfo.m_vecEnd = current.dst;
					if (Settings::Esp::esp_beams_tracer)
					{

						auto beam = shit::g_pViewRenderBeams()->CreateBeamPoints(beamInfo);
						if (beam)
							shit::g_pViewRenderBeams()->DrawBeam(beam);
					}

					logs.erase(logs.begin() + i);
				}


			}
		}
	}
}

void CEsp::renderBeams()
{

	if (Settings::Esp::esp_beams_tracer)
		return;

	auto local = static_cast<CBaseEntity*>(Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer()));

	if (!local)
		return;
	for (size_t i = 0; i < logs.size(); i++)
	{

		auto current = logs.at(i);

		current.color = Color(int(Settings::Esp::flTracer_Beam[0] * 255.f),
			int(Settings::Esp::flTracer_Beam[1] * 255.f),
			int(Settings::Esp::flTracer_Beam[2] * 255.f));

		if (Settings::Esp::esp_beams_tracer)
			Interfaces::DebugOverlay()->AddLineOverlay(current.src, current.dst, current.color.r(), current.color.g(), current.color.g(), true, -1.f);


		Interfaces::DebugOverlay()->AddBoxOverlay(current.dst, Vector(-2, -2, -2), Vector(2, 2, 2), Vector(0, 0, 0), current.color.r(), current.color.g(), current.color.g(), 127, -1.f);

		if (fabs(Interfaces::GlobalVars()->curtime - current.time) > 5.f)
			logs.erase(logs.begin() + i);
	}
}

int StringToWeapon(std::string weapon) {
	if (!strcmp(weapon.c_str(), "smokegrenade"))
		return 45;
	if (!strcmp(weapon.c_str(), "flashbang"))
		return 43;
	if (!strcmp(weapon.c_str(), "incgrenade"))
		return 46;
}

void CEsp::AsusWalls()
{


	if (Settings::Esp::esp_AsusWalls)
	{

		static auto r_DrawSpecificStaticProp = Interfaces::GetConVar()->FindVar("r_DrawSpecificStaticProp");
		r_DrawSpecificStaticProp->SetValue(1);

		for (MaterialHandle_t i = Interfaces::MaterialSystem()->FirstMaterial(); i != Interfaces::MaterialSystem()->InvalidMaterial(); i = Interfaces::MaterialSystem()->NextMaterial(i))
		{
			IMaterial *pMaterial = Interfaces::MaterialSystem()->GetMaterial(i);

			if (!pMaterial)
				continue;

			const char* group = pMaterial->GetTextureGroupName();
			const char* name = pMaterial->GetName();

			double XD = 100;
			double Opacity = int(Settings::Esp::esp_WallsOpacity);
			double RealOpacity = 0.75f/*Opacity / XD*/;

			/*if (strstr(group, "StaticProp"))
			{
			pMaterial->AlphaModulate(RealOpacity);
			pMaterial->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
			}*/
			if (strstr(group, "World textures"))
			{
				pMaterial->AlphaModulate(RealOpacity);
				pMaterial->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
			}
			if (strstr(name, "models/props/de_dust/palace_bigdome"))
			{
				pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
				pMaterial->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
			}
			if (strstr(name, "models/props/de_dust/palace_pillars"))
			{
				pMaterial->AlphaModulate(RealOpacity);
				pMaterial->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
			}

			if (strstr(group, "Particle textures"))
			{
				pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
				pMaterial->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
			}

		}



	}
}
	

void CEsp::DrawHitmarker()
{
	

	if (Settings::hitmarkerAlpha < 0.f)
		Settings::hitmarkerAlpha = 0.f;
	else if (Settings::hitmarkerAlpha > 0.f)
		Settings::hitmarkerAlpha -= 0.01f;

	int W, H;
	Interfaces::Engine()->GetScreenSize(W, H);

	float r = Settings::Misc::misc_HitMarkerColor[0] * 255.f;
	float g = Settings::Misc::misc_HitMarkerColor[1] * 255.f;
	float b = Settings::Misc::misc_HitMarkerColor[2] * 255.f;

	if (Settings::hitmarkerAlpha > 0.f)
	{
		g_pRender->DrawLine(W / 2 - 10, H / 2 - 10, W / 2 - 5, H / 2 - 5, Color(r, g, b, (Settings::hitmarkerAlpha * 255.f)));
		g_pRender->DrawLine(W / 2 - 10, H / 2 + 10, W / 2 - 5, H / 2 + 5, Color(r, g, b, (Settings::hitmarkerAlpha * 255.f)));
		g_pRender->DrawLine(W / 2 + 10, H / 2 - 10, W / 2 + 5, H / 2 - 5, Color(r, g, b, (Settings::hitmarkerAlpha * 255.f)));
		g_pRender->DrawLine(W / 2 + 10, H / 2 + 10, W / 2 + 5, H / 2 + 5, Color(r, g, b, (Settings::hitmarkerAlpha * 255.f)));
	}

	

}






void hitmarker::player_hurt_listener::start()
{
	if (!Interfaces::GameEvent()->AddListener(this, "player_hurt", false))
	{
		throw exception("Failed to register the event");
	}
	if (!Interfaces::GameEvent()->AddListener(this, "bullet_impact", false))
	{
		throw exception("Failed to register the event");
	}
}
void hitmarker::player_hurt_listener::stop()
{
	Interfaces::GameEvent()->RemoveListener(this);
}
void hitmarker::player_hurt_listener::FireGameEvent(IGameEvent *event)
{
	g_pEsp->HitmarkerEvents(event);
	g_pEsp->HitEvents(event);
}
int hitmarker::player_hurt_listener::GetEventDebugID(void)
{
	return 0x2A;
}

void CEsp::OnRender()
{

	if (g_pAimbot)
		g_pAimbot->OnRender();

	if (Settings::Misc::misc_HitMarker)
		DrawHitmarker();

	renderBeams();

	Ambient();
	AsusWalls();

	if (Settings::Misc::misc_AwpAim && IsLocalAlive() && g_pPlayers->GetLocal()->WeaponIndex == WEAPON_AWP)
	{
		Color AwpAimColor = Color(int(Settings::Misc::misc_AwpAimColor[0] * 255.f),
			int(Settings::Misc::misc_AwpAimColor[1] * 255.f),
			int(Settings::Misc::misc_AwpAimColor[2] * 255.f));

		g_pRender->DrawOutlineBox(iScreenWidth / 2 - 1, iScreenHeight / 2 - 1, 3, 3, AwpAimColor);
	}
	if (Settings::Misc::misc_AwpAim && IsLocalAlive() && g_pPlayers->GetLocal()->WeaponIndex == WEAPON_SCAR20)
	{
		Color AwpAimColor = Color(int(Settings::Misc::misc_AwpAimColor[0] * 255.f),
			int(Settings::Misc::misc_AwpAimColor[1] * 255.f),
			int(Settings::Misc::misc_AwpAimColor[2] * 255.f));

		g_pRender->DrawOutlineBox(iScreenWidth / 2 - 1, iScreenHeight / 2 - 1, 3, 3, AwpAimColor);
	}
	if (Settings::Misc::misc_AwpAim && IsLocalAlive() && g_pPlayers->GetLocal()->WeaponIndex == WEAPON_SSG08)
	{
		Color AwpAimColor = Color(int(Settings::Misc::misc_AwpAimColor[0] * 255.f),
			int(Settings::Misc::misc_AwpAimColor[1] * 255.f),
			int(Settings::Misc::misc_AwpAimColor[2] * 255.f));

		g_pRender->DrawOutlineBox(iScreenWidth / 2 - 1, iScreenHeight / 2 - 1, 3, 3, AwpAimColor);
	}
	if (Settings::Misc::misc_AwpAim && IsLocalAlive() && g_pPlayers->GetLocal()->WeaponIndex == WEAPON_G3SG1)
	{
		Color AwpAimColor = Color(int(Settings::Misc::misc_AwpAimColor[0] * 255.f),
			int(Settings::Misc::misc_AwpAimColor[1] * 255.f),
			int(Settings::Misc::misc_AwpAimColor[2] * 255.f));

		g_pRender->DrawOutlineBox(iScreenWidth / 2 - 1, iScreenHeight / 2 - 1, 3, 3, AwpAimColor);
	}

	IMaterial *xblur_mat = Interfaces::MaterialSystem()->FindMaterial("dev/blurfilterx_nohdr", TEXTURE_GROUP_OTHER, true);
	IMaterial *yblur_mat = Interfaces::MaterialSystem()->FindMaterial("dev/blurfiltery_nohdr", TEXTURE_GROUP_OTHER, true);
	IMaterial *scope = Interfaces::MaterialSystem()->FindMaterial("dev/scope_bluroverlay", TEXTURE_GROUP_OTHER, true);
	xblur_mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
	yblur_mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
	scope->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);

	NightMode();

	for (BYTE PlayerIndex = 0; PlayerIndex < g_pPlayers->GetSize(); PlayerIndex++)
	{
		CPlayer* pPlayer = g_pPlayers->GetPlayer(PlayerIndex);

		if (pPlayer && pPlayer->m_pEntity && pPlayer->bUpdate && CheckPlayerTeam(pPlayer))
		{
			DrawPlayerEsp(pPlayer);

			if (Settings::Esp::esp_Skeleton)
				DrawPlayerSkeleton(pPlayer);

			if (Settings::Esp::esp_BulletTrace)
				DrawPlayerBulletTrace(pPlayer);

			if (Settings::Esp::esp_Dlightz)
				Dlight(pPlayer);
		}
	}

	if (Settings::Esp::esp_BombTimer)
	{
		if (bC4Timer && iC4Timer)
		{
			float fTimeStamp = Interfaces::Engine()->GetLastTimeStamp();

			if (!fExplodeC4Timer)
			{
				fExplodeC4Timer = fTimeStamp + (float)iC4Timer;
			}
			else
			{
				fC4Timer = fExplodeC4Timer - fTimeStamp;

				if (fC4Timer < 0.f)
					fC4Timer = 0.f;
			}
		}
		else
		{
			fExplodeC4Timer = 0.f;
			fC4Timer = 0.f;

			Color GrenadeHelper = Color(int(Settings::Esp::GrenadeHelper[0] * 255.f),
				int(Settings::Esp::GrenadeHelper[1] * 255.f),
				int(Settings::Esp::GrenadeHelper[2] * 255.f));

			

		}
	}

	if ((Settings::Esp::esp_Bomb || Settings::Esp::esp_BombPlanted || Settings::Esp::esp_WorldWeapons || Settings::Esp::esp_WorldGrenade || Settings::Esp::esp_Animals) && Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
	{
		for (int EntIndex = 0; EntIndex < Interfaces::EntityList()->GetHighestEntityIndex(); EntIndex++)
		{
			CBaseEntity* pEntity = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(EntIndex);

			if (!pEntity || pEntity->IsPlayer())
				continue;

			const model_t* pModel = pEntity->GetModel();
			int iHpAmY = 1;
			if (pModel)
			{
				const char* pModelName = Interfaces::ModelInfo()->GetModelName(pModel);

				if (pModelName)
				{
					Vector vEntScreen;

					if (WorldToScreen(pEntity->GetRenderOrigin(), vEntScreen))
					{
						if (Settings::Esp::esp_Animals && (pEntity->GetClientClass()->m_ClassID == (int)CLIENT_CLASS_ID::CChicken))
						{
							g_pRender->DrawOutlineBox((int)vEntScreen.x - 5, (int)vEntScreen.y - 20, 10, 10, Color::Aqua());
							g_pRender->Text((int)vEntScreen.x, (int)vEntScreen.y, true, true, Color::YellowGreen(),
								"Chicken");
						}

						if (Settings::Esp::esp_Animals && (pEntity->GetClientClass()->m_ClassID == (int)CLIENT_CLASS_ID::CFish))
						{
							g_pRender->Text((int)vEntScreen.x, (int)vEntScreen.y, true, true, Color::Green(),
								"Fish");
						}

						if (Settings::Esp::esp_BombPlanted && (pEntity->GetClientClass()->m_ClassID == (int)CLIENT_CLASS_ID::CPlantedC4))
						{
							g_pRender->Text((int)vEntScreen.x, (int)vEntScreen.y + 10, true, true, Color::Yellow(), "%0.2f", g_pEsp->fC4Timer);
						}

						if (Settings::Esp::esp_BombPlanted && pEntity->GetClientClass()->m_ClassID == (int)CLIENT_CLASS_ID::CPlantedC4)
						{
							g_pRender->Text((int)vEntScreen.x, (int)vEntScreen.y, true, true, Color::Red(),
								"[C4 PLANTED]");
						}

						switch (Settings::Esp::esp_WeaponIcon)
						{
						
						case 1:
							if (Settings::Esp::esp_Bomb && pEntity->GetClientClass()->m_ClassID == (int)CLIENT_CLASS_ID::CC4)
							{
								g_pRender->Text((int)vEntScreen.x, (int)vEntScreen.y + iHpAmY, true, true, Color::Aqua(),
									"[C4]");
								iHpAmY += 20;
							}
							break;
						case 2:
							if (Settings::Esp::esp_Bomb && pEntity->GetClientClass()->m_ClassID == (int)CLIENT_CLASS_ID::CC4)
							{
								g_pRender->WepIcon((int)vEntScreen.x, (int)vEntScreen.y + iHpAmY, true, true, Color::Aqua(), "o");
								iHpAmY += 20;
							}
							break;
						}
						if (Settings::Esp::esp_WorldWeapons && !strstr(pModelName, "models/weapons/w_eq_")
							&& !strstr(pModelName, "models/weapons/w_ied"))
						{
							if (strstr(pModelName, "models/weapons/w_") && strstr(pModelName, "_dropped.mdl"))
							{
								string WeaponName = pModelName + 17;

								Color ColorWorldWeapon = Color(int(Settings::Esp::WorldWeapon_Color[0] * 255.f),
									int(Settings::Esp::WorldWeapon_Color[1] * 255.f),
									int(Settings::Esp::WorldWeapon_Color[2] * 255.f));

								WeaponName[WeaponName.size() - 12] = '\0';

								if (strstr(pModelName, "models/weapons/w_rif") && strstr(pModelName, "_dropped.mdl"))
								{
									WeaponName.erase(0, 4);
								}
								else if (strstr(pModelName, "models/weapons/w_pist") && strstr(pModelName, "_dropped.mdl") && !strstr(pModelName, "models/weapons/w_pist_223"))
								{
									WeaponName.erase(0, 5);
								}
								else if (strstr(pModelName, "models/weapons/w_smg") && strstr(pModelName, "_dropped.mdl"))
								{
									WeaponName.erase(0, 4);
								}
								else if (strstr(pModelName, "models/weapons/w_mach") && strstr(pModelName, "_dropped.mdl"))
								{
									WeaponName.erase(0, 5);
								}
								else if (strstr(pModelName, "models/weapons/w_shot") && strstr(pModelName, "_dropped.mdl"))
								{
									WeaponName.erase(0, 5);
								}
								else if (strstr(pModelName, "models/weapons/w_snip") && strstr(pModelName, "_dropped.mdl"))
								{
									WeaponName.erase(0, 5);
								}
								if (strstr(pModelName, "models/weapons/w_pist_223") && strstr(pModelName, "_dropped.mdl"))
								{
									WeaponName = "usp_s";
								}
								if (strstr(pModelName, "models/weapons/w_pist_hkp2000") && strstr(pModelName, "_dropped.mdl"))
								{
									WeaponName = "p2000";
								}
								if (strstr(pModelName, "models/weapons/w_pist_cz_75") && strstr(pModelName, "_dropped.mdl"))
								{
									WeaponName = "cz75";
								}
								if (strstr(pModelName, "models/weapons/w_rif_m4a1") && strstr(pModelName, "_dropped.mdl"))
								{
									WeaponName = "m4a1";
								}
								if (strstr(pModelName, "models/weapons/w_rif_m4a1_s") && strstr(pModelName, "_dropped.mdl"))
								{
									WeaponName = "m4a1_s";
								}
								if (strstr(pModelName, "models/weapons/w_rif_sg556") && strstr(pModelName, "_dropped.mdl"))
								{
									WeaponName = "sg553";
								}

								g_pRender->Text((int)vEntScreen.x, (int)vEntScreen.y, true, true, ColorWorldWeapon,
									WeaponName.c_str());
							}
						}

						if (Settings::Esp::esp_WorldGrenade &&
							(strstr(pModelName, "models/weapons/w_eq_") ||
								strstr(pModelName, "models/Weapons/w_eq_")))
						{
							if (strstr(pModelName, "_dropped.mdl"))
							{
								string WeaponName = pModelName + 20;

								WeaponName[WeaponName.size() - 12] = '\0';

								Color GrenadeColor = Color::White();

								if (strstr(pModelName, "fraggrenade"))
								{
									WeaponName = "grenade";
									GrenadeColor = Color::OrangeRed();
								}
								else if (strstr(pModelName, "molotov"))
								{
									WeaponName = "fire_molo";
									GrenadeColor = Color::Orange();
								}
								else if (strstr(pModelName, "incendiarygrenade"))
								{
									WeaponName = "fire_ince";
									GrenadeColor = Color::Orange();
								}
								else if (strstr(pModelName, "flashbang"))
								{
									WeaponName = "flash";
									GrenadeColor = Color::Yellow();
								}
								else if (strstr(pModelName, "smokegrenade"))
								{
									WeaponName = "smoke";
									GrenadeColor = Color::Color::LightGray();
								}

								g_pRender->Text((int)vEntScreen.x, (int)vEntScreen.y, true, true, GrenadeColor,
									WeaponName.c_str());
							}
							else if (strstr(pModelName, "smokegrenade_thrown.mdl") && Settings::Misc::misc_NoSmoke == true)
							{
								string WeaponName = "Smoke";
								g_pRender->DrawOutlineBox((int)vEntScreen.x - 20, (int)vEntScreen.y - 35, 40, 40, Color::Purple());
								g_pRender->Text((int)vEntScreen.x, (int)vEntScreen.y, true, true, Color::MediumPurple(),
									WeaponName.c_str());

							}
							else if (strstr(pModelName, "smokegrenade_thrown.mdl"))
							{
								string WeaponName = "smoke";

								g_pRender->Text((int)vEntScreen.x, (int)vEntScreen.y, true, true, Color::Gray(),
									WeaponName.c_str());
							}
							else if (strstr(pModelName, "taser.mdl") && Settings::Esp::esp_Outline == true)
							{
								string WeaponName = "zeus";
								g_pRender->Text((int)vEntScreen.x, (int)vEntScreen.y, true, true, Color::LightSkyBlue(),
									WeaponName.c_str());

							}
							else if (strstr(pModelName, "w_defuser.mdl"))
							{
								string WeaponName = "defuse kit";

								g_pRender->Text((int)vEntScreen.x, (int)vEntScreen.y, true, true, Color::LimeGreen(),
									WeaponName.c_str());
							}
						}
					}
				}
			}
		}
	}

	// Draw Ticks
	if (Settings::Aimbot::aim_Backtrack && Settings::Aimbot::aim_DrawBacktrack)
	{
		for (int i = 0; i < Interfaces::EntityList()->GetHighestEntityIndex(); i++)
		{
			CBaseEntity* local = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());
			CBaseEntity *entity = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(i);
			CPlayer* pPlayer = g_pPlayers->GetPlayer(i);
			PlayerInfo pinfo;
			if (entity == nullptr)
				continue;
			if (entity == local)
				continue;
			if (entity->IsDormant())
				continue;
			if (entity->GetTeam() == local->GetTeam())
				continue;
			if (Interfaces::Engine()->GetPlayerInfo(i, &pinfo) && !entity->IsDead())
			{
				if (Settings::Esp::esp_Visible >= 3)
				{
					if (!local->IsDead() && pPlayer->bVisible)
					{
						for (int t = 0; t < Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_backtrack; ++t)
						{
							Vector screenbacktrack[64][12];

							if (headPositions[i][t].simtime && headPositions[i][t].simtime + 1 > local->GetSimTime())
							{
								if (WorldToScreen(headPositions[i][t].hitboxPos, screenbacktrack[i][t]))
								{
									g_pRender->DrawFillBox(screenbacktrack[i][t].x, screenbacktrack[i][t].y, 4, 4, g_pEsp->GetPlayerColor(pPlayer));
								}
							}
						}
					}
					else
					{
						memset(&headPositions[0][0], 0, sizeof(headPositions));
					}
				}
				else
				{
					if (!local->IsDead())
					{
						for (int t = 0; t < Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_backtrack; ++t)
						{
							Vector screenbacktrack[64][12];

							if (headPositions[i][t].simtime && headPositions[i][t].simtime + 1 > local->GetSimTime())
							{
								if (WorldToScreen(headPositions[i][t].hitboxPos, screenbacktrack[i][t]))
								{
									g_pRender->DrawFillBox(screenbacktrack[i][t].x, screenbacktrack[i][t].y, 4, 4, g_pEsp->GetPlayerColor(pPlayer));
								}
							}
						}
					}
					else
					{
						memset(&headPositions[0][0], 0, sizeof(headPositions));
					}
				}
			}
		}
	}
}

void MsgFunc_ServerRankRevealAll()
{
	using tServerRankRevealAllFn = bool(__cdecl*)(int*);
	static tServerRankRevealAllFn ServerRankRevealAll = 0;

	if (!ServerRankRevealAll)
	{
		ServerRankRevealAll = (tServerRankRevealAllFn)(
			CSX::Memory::FindPattern(CLIENT_DLL, 
				"55 8B EC 8B 0D ? ? ? ? 85 C9 75 28 A1 ? ? ? ? 68 ? ? ? ? 8B 08 8B 01 FF 50 04 85 C0 74 0B 8B C8 E8 ? ? ? ? 8B C8 EB 02 33 C9 89 0D ? ? ? ? 8B 45 08", 0));
	}

	if (ServerRankRevealAll)
	{
		int fArray[3] = { 0,0,0 };
		ServerRankRevealAll(fArray);
	}
}

void CEsp::OnCreateMove(CUserCmd* pCmd)
{
	if (Settings::Esp::esp_Rank && pCmd->buttons & IN_SCORE)
	{
		MsgFunc_ServerRankRevealAll();
	}
	
}

void CEsp::OnReset()
{
	if (Settings::Esp::esp_BombTimer)
	{
		if (Settings::Esp::esp_BombTimer > 60)
			Settings::Esp::esp_BombTimer = 60;

		bC4Timer = false;
		iC4Timer = Settings::Esp::esp_BombTimer;
	}
	//g_pEsp->SoundEsp.Sound.clear();
}

void CEsp::OnEvents(IGameEvent* pEvent)
{
	if (g_pEsp && Settings::Esp::esp_BombTimer)
	{
		if (!strcmp(pEvent->GetName(), "bomb_defused") || !strcmp(pEvent->GetName(), "bomb_exploded"))
		{
			bC4Timer = false;
		}
		else if (!strcmp(pEvent->GetName(), "bomb_planted"))
		{
			bC4Timer = true;
		}
	}
}

void CEsp::OnDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
{
	if (!g_pPlayers || Interfaces::Engine()->IsTakingScreenshot() || !Interfaces::Engine()->IsConnected() || !pInfo.pModel)
		return;

	static bool InitalizeMaterial = false;

	if (!InitalizeMaterial)
	{
		visible_flat = CreateMaterial(true, false);
		visible_tex = CreateMaterial(false, false);
		hidden_flat = CreateMaterial(true, true);
		hidden_tex = CreateMaterial(false, true);
		InitalizeMaterial = true;

		return;
	}

	string strModelName = Interfaces::ModelInfo()->GetModelName(pInfo.pModel);

	if (strModelName.size() <= 1)
		return;



	if (Interfaces::Engine()->IsInGame() && IsLocalAlive()) {


		if (strModelName.find("arms") != string::npos)
		{
			

			switch (Settings::Esp::esp_rendergrenadecheck)
			{
			case 0:
			{
				IMaterial* WireHands = Interfaces::MaterialSystem()->FindMaterial(strModelName.c_str(), TEXTURE_GROUP_MODEL);
				WireHands->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
			}

				break;
			case 1: 
			{
				IMaterial* WireHands = Interfaces::MaterialSystem()->FindMaterial(strModelName.c_str(), TEXTURE_GROUP_MODEL);
				
				
				


				WireHands->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
				WireHands->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
				WireHands->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
				WireHands->SetMaterialVarFlag(MATERIAL_VAR_FLAT, false);
				ForceMaterial(Color(255, 255, 255), WireHands);
			}
				 break; // Wireframe
//			
			
			}

		}

	}

	if (Settings::Esp::esp_Chams && Client::g_pPlayers && Client::g_pPlayers->GetLocal() && strModelName.find("models/player") != string::npos && Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
	{
		IClientEntity* pBaseEntity = Interfaces::EntityList()->GetClientEntity(pInfo.entity_index);

		if (pBaseEntity && pBaseEntity->GetClientClass()->m_ClassID == (int)CLIENT_CLASS_ID::CCSPlayer)
		{
			CPlayer* pPlayer = g_pPlayers->GetPlayer(pInfo.entity_index);

			if (pPlayer && pPlayer->bUpdate)
			{
				bool CheckTeam = false;

				Color TeamHideColor;
				Color TeamVisibleColor;

				if (Settings::Esp::esp_Enemy && pPlayer->Team != g_pPlayers->GetLocal()->Team) // Ïðîòèâíèêîâ
					CheckTeam = true;

				if (Settings::Esp::esp_Team && pPlayer->Team == g_pPlayers->GetLocal()->Team) // Ñâîèõ
					CheckTeam = true;

				int playerHealth = pPlayer->iHealth;

				if (pPlayer->Team == TEAM_CT)
				{
					TeamHideColor = Color(int(Settings::Esp::chams_Color_CT[0] * 255.f),
						int(Settings::Esp::chams_Color_CT[1] * 255.f),
						int(Settings::Esp::chams_Color_CT[2] * 255.f));
				}
				else if (pPlayer->Team == TEAM_TT)
				{
					TeamHideColor = Color(int(Settings::Esp::chams_Color_TT[0] * 255.f),
						int(Settings::Esp::chams_Color_TT[1] * 255.f),
						int(Settings::Esp::chams_Color_TT[2] * 255.f));
				}

				bool SetColor = false;

				if (Settings::Esp::esp_ChamsVisible == 1) // chams wall
				{
					SetColor = true;
				}

				if (SetColor)
				{
					if (pPlayer->Team == TEAM_CT)
					{
						TeamVisibleColor = Color(int(Settings::Esp::chams_Color_VCT[0] * 255.f),
							int(Settings::Esp::chams_Color_VCT[1] * 255.f),
							int(Settings::Esp::chams_Color_VCT[2] * 255.f));
					}
					else if (pPlayer->Team == TEAM_TT)
					{
						TeamVisibleColor = Color(int(Settings::Esp::chams_Color_VTT[0] * 255.f),
							int(Settings::Esp::chams_Color_VTT[1] * 255.f),
							int(Settings::Esp::chams_Color_VTT[2] * 255.f));
					}
				}
				else
					TeamVisibleColor = TeamHideColor;

				if (CheckTeam)
				{
					if (Settings::Esp::esp_ChamsVisible >= 1) //chams vis
					{
						if (Settings::Esp::esp_Chams == 3)
						{
							ForceMaterial(TeamHideColor, hidden_flat);
							hidden_flat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
						}
						else if (Settings::Esp::esp_Chams == 4)
						{
							ForceMaterial(TeamHideColor, hidden_tex);
							hidden_tex->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
						}
						else if (Settings::Esp::esp_Chams == 6)
						{
							float Blend[3] = { (float)TeamVisibleColor[0] / 255.f * Settings::Esp::esp_Chams_Light,
								(float)TeamVisibleColor[1] / 255.f * Settings::Esp::esp_Chams_Light, 
								(float)TeamVisibleColor[2] / 255.f * Settings::Esp::esp_Chams_Light };
							Interfaces::RenderView()->SetColorModulation(Blend);
						}
					}
					else
					{
						if (Settings::Esp::esp_Chams == 1)
						{
							ForceMaterial(TeamHideColor, hidden_flat);
							hidden_flat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						}
						else if (Settings::Esp::esp_Chams == 2)
						{
							ForceMaterial(TeamHideColor, hidden_tex);
							hidden_tex->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						}
						else if (Settings::Esp::esp_Chams == 6)
						{
							float Blend[3] = { (float)TeamVisibleColor[0] / 255.f * Settings::Esp::esp_Chams_Light, 
								(float)TeamVisibleColor[1] / 255.f * Settings::Esp::esp_Chams_Light, 
								(float)TeamVisibleColor[2] / 255.f * Settings::Esp::esp_Chams_Light };
							Interfaces::RenderView()->SetColorModulation(Blend);
						}
					}

					Interfaces::ModelRender()->DrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);

					if (Settings::Esp::esp_Chams == 1 || Settings::Esp::esp_Chams == 3)
					{
						ForceMaterial(TeamVisibleColor, visible_flat);
						visible_flat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
					}
					else if (Settings::Esp::esp_Chams == 2 || Settings::Esp::esp_Chams == 4)
					{
						ForceMaterial(TeamVisibleColor, visible_tex);
						visible_tex->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
					}
					else if (Settings::Esp::esp_Chams == 6)
					{
						float Blend[3] = { (float)TeamVisibleColor[0] / 255.f * Settings::Esp::esp_Chams_Light, 
							(float)TeamVisibleColor[1] / 255.f * Settings::Esp::esp_Chams_Light, 
							(float)TeamVisibleColor[2] / 255.f * Settings::Esp::esp_Chams_Light };
						Interfaces::RenderView()->SetColorModulation(Blend);
					}
				}
			}
		}
	}
}

void CEsp::DrawPlayerEsp(CPlayer* pPlayer)
{
	bool bOriginScreen = (pPlayer->vOriginScreen.x > 0 && pPlayer->vOriginScreen.y > 0);
	bool bHitBoxScreen = (pPlayer->vHitboxHeadScreen.x > 0 && pPlayer->vHitboxHeadScreen.y > 0);

	if (!bOriginScreen && !bHitBoxScreen)
		return;

	Vector vLineOrigin;

	if (Settings::Esp::esp_Size < 1)
	{
		Settings::Esp::esp_Size = 1;
	}
	else if (Settings::Esp::esp_Size > 10)
	{
		Settings::Esp::esp_Size = 10;
	}

	int Height = (int)pPlayer->vOriginScreen.y - (int)pPlayer->vHitboxHeadScreen.y;

	if (Height < 18)
		Height = 18;

	int Width = Height / 2;

	int x = (int)pPlayer->vHitboxHeadScreen.x - Width / 2;
	int y = (int)pPlayer->vHitboxHeadScreen.y;

	vLineOrigin = pPlayer->vHitboxHeadScreen;
	vLineOrigin.y += Height;

	Color EspPlayerColor = GetPlayerColor(pPlayer);
	Color EspVisibleColor = GetPlayerVisibleColor(pPlayer);

	if (Settings::Esp::esp_Size)
	{
		if (Settings::Esp::esp_Style == 0) //None
		{
		}
		else if (Settings::Esp::esp_Style == 1)  //Box
		{
			if (!Settings::Esp::esp_Outline)
			{
				g_pRender->DrawBox(x, y, Width, Height, EspVisibleColor);
			}
			else if (Settings::Esp::esp_Outline)
			{
				g_pRender->DrawOutlineBox(x, y, Width, Height, EspVisibleColor);
			}
		}
		else if (Settings::Esp::esp_Style == 2)  //CoalBox
		{
			if (!Settings::Esp::esp_Outline)
			{
				g_pRender->DrawCoalBox(x, y, Width, Height, EspVisibleColor);
			}
			else if (Settings::Esp::esp_Outline)
			{
				g_pRender->DrawOutlineCoalBox(x, y, Width, Height, EspVisibleColor);
			}
		}

		else if (Settings::Esp::esp_Style == 3)
		{
			if (!Settings::Esp::esp_Outline)
			{
				g_pRender->GenuineBox(x, y, Width, Height, EspVisibleColor);
			}
			else if (Settings::Esp::esp_Outline)
			{
				g_pRender->GenuineOutlineBox(x, y, Width, Height, EspVisibleColor);
			}
		}
	}



	if (Settings::Esp::esp_Glow)
	{
		



	}


	if (Settings::Esp::esp_Line)
	{
		g_pRender->DrawLine((int)vLineOrigin.x, (int)vLineOrigin.y, iScreenWidth / 2, iScreenHeight, EspVisibleColor);
	}

	if (Settings::Esp::esp_Name && Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
	{
		
		if(strlen(pPlayer->Name.c_str()) > 0)
			g_pRender->Text((int)vLineOrigin.x, (int)pPlayer->vHitboxHeadScreen.y - 20, true, true, EspPlayerColor, pPlayer->Name.c_str());
	}

	int iHpAmY = 1;

	if (Settings::Esp::esp_Health >= 1 && Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
	{
		Color Minus = Color::Red();

		if (pPlayer->Team == TEAM_CT)
		{
			Minus = CT_HP_ColorM;
		}
		else if (pPlayer->Team == TEAM_TT)
		{
			Minus = TT_HP_ColorM;
		}

		int iHealth = pPlayer->iHealth;

		if (iHealth)
		{
			if (Settings::Esp::esp_Health == 1) //Number
			{
				g_pRender->Text((int)vLineOrigin.x, (int)vLineOrigin.y + iHpAmY, true, true, EspVisibleColor, to_string(iHealth).c_str());
				iHpAmY += 10;
			}
			else if (Settings::Esp::esp_Health == 2) //Bottom
			{
				g_pRender->DrawHorBar(x, (int)vLineOrigin.y + iHpAmY, Width, 5, iHealth, Color::LawnGreen(), Minus);
				iHpAmY += 6;
			}
			else if (Settings::Esp::esp_Health == 3) //Left
			{
				g_pRender->DrawVerBar(x - 6, (int)pPlayer->vHitboxHeadScreen.y, 5, Height, iHealth, Color::LawnGreen(), Minus);
			}
			else if (Settings::Esp::esp_Health == 4) //edgy
			{
				float flBoxes = std::ceil(pPlayer->iHealth / 10.f);
				float flX = x - 6;
				float flY = y - 1;
				float flHeight = Height / 10.f;
				float flMultiplier = 12 / 360.f;
				flMultiplier *= flBoxes - 1;
				Color ColHealth = Color::FromHSB(flMultiplier, 1, 1);

				g_pRender->DrawBox(flX, flY, 4, Height + 2, Color(80, 80, 80, 125));
				g_pRender->DrawOutlineBox(flX, flY, 4, Height + 2, Color::Black());
				g_pRender->DrawBox(flX + 1, flY, 2, flHeight * flBoxes + 1, ColHealth);

				for (int i = 0; i < 10; i++)
				{
					g_pRender->DrawLine(flX, flY + i * flHeight, flX + 4, flY + i * flHeight, Color::Black());
				}
			}
		}
	}

	if (Settings::Esp::esp_Armor >= 1 && Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected()) //None
	{
		Color Minus = Color::Red();

		if (pPlayer->Team == TEAM_CT)
		{
			Minus = CT_AR_ColorM;
		}
		else if (pPlayer->Team == TEAM_TT)
		{
			Minus = TT_AR_ColorM;
		}

		int iArmor = pPlayer->iArmor;

		if (iArmor)
		{
			if (Settings::Esp::esp_Armor == 1) //Number
			{
				g_pRender->Text((int)vLineOrigin.x, (int)vLineOrigin.y + iHpAmY, true, true, EspVisibleColor, to_string(iArmor).c_str());
				iHpAmY += 10;
			}
			if (Settings::Esp::esp_Armor == 2) //Bottom
			{
				g_pRender->DrawHorBar(x, (int)vLineOrigin.y + iHpAmY, Width, 5, iArmor, Color::Cyan(), Minus);
				iHpAmY += 6;
			}
			else if (Settings::Esp::esp_Armor == 3) //Right
			{
				g_pRender->DrawVerBar(x + Width + 1, (int)pPlayer->vHitboxHeadScreen.y, 5, Height, iArmor, Color::Cyan(), Minus);
			}
			else if (Settings::Esp::esp_Armor == 4) //Edgy
			{
				float flBoxes = std::ceil(pPlayer->iArmor / 10.f);
				float flX = x + Width + 1;
				float flY = y - 1;
				float flHeight = Height / 10.f;
				float flMultiplier = 12 / 360.f;
				flMultiplier *= flBoxes - 1;
				Color ColArmor = Color::White();

				g_pRender->DrawBox(flX, flY, 4, Height + 2, Color::White());
				g_pRender->DrawOutlineBox(flX, flY, 4, Height + 2, Color::Black());
				g_pRender->DrawBox(flX + 1, flY, 2, flHeight * flBoxes + 1, ColArmor);

				for (int i = 0; i < 10; i++)
				{
					g_pRender->DrawLine(flX, flY + i * flHeight, flX + 4, flY + i * flHeight, Color::Black());
				}
			}
		}
	}

	if (Settings::Esp::esp_Distance && g_pPlayers->GetLocal()->bAlive)
	{
		int Distance = pPlayer->iDistance;
		g_pRender->Text((int)vLineOrigin.x, (int)vLineOrigin.y + iHpAmY, true, true, EspVisibleColor, to_string(Distance).c_str());
	}

	if (Settings::Esp::esp_Ammo && pPlayer->iWAmmo)
	{
		int Ammo = pPlayer->iWAmmo;
		g_pRender->Text((int)vLineOrigin.x, (int)vLineOrigin.y + iHpAmY, true, true, EspVisibleColor, to_string(Ammo).c_str());
		iHpAmY += 15;
	}

	if (Settings::Esp::esp_Infoz)
	{
		if (Settings::Esp::esp_Defusing)
		{
			if (Interfaces::Engine()->IsInGame())
			{
				if (pPlayer->m_pEntity->IsDefusing())
				{
					int iHpAmY = 15;
					g_pRender->Text((int)vLineOrigin.x, (int)vLineOrigin.y + iHpAmY, true, true, EspVisibleColor, ("Defusing"));
				}
			}
		}
		if (Settings::Esp::esp_Scoped)
		{
			if (Interfaces::Engine()->IsInGame())
			{
				if (pPlayer->m_pEntity->GetIsScoped())
				{
					int iHpAmY = 15;
					g_pRender->Text((int)vLineOrigin.x, (int)vLineOrigin.y + iHpAmY, true, true, EspVisibleColor, ("Scoped"));
				}
			}
		}
		if (Settings::Esp::esp_Flashed)
		{
			if (Interfaces::Engine()->IsInGame())
			{
				if (pPlayer->m_pEntity->IsFlashed())
				{
					int iHpAmY = 15;
					g_pRender->Text((int)vLineOrigin.x, (int)vLineOrigin.y + iHpAmY, true, true, EspVisibleColor, ("Flashed"));
				}
			}
		}
	}

	switch (Settings::Esp::esp_WeaponIcon)
	{
	case 1:
		if (!pPlayer->WeaponName.empty() && /*IsLocalAlive()*/Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
		{
			string WeaponStr = pPlayer->WeaponName;
			//weapons
			string search = "M4A1";
			string replace = "m4a1";
			string search1 = "M4_SILENCER";
			string replace1 = "m4a1_s";
			string search2 = "AK47";
			string replace2 = "ak47";
			string search3 = "Elites";
			string replace3 = "elite";
			string search4 = "DesertEagle";
			string replace4 = "deagle";
			string search5 = "Aug";
			string replace5 = "aug";
			string search6 = "AWP";
			string replace6 = "awp";
			string search7 = "Famas";
			string replace7 = "famas";
			string search8 = "FiveSeven";
			string replace8 = "fiveseven";
			string search9 = "G3SG1";
			string replace9 = "g3sg1";
			string search10 = "GalilAR";
			string replace10 = "galilar";
			string search11 = "Glock18";
			string replace11 = "glock18";
			string search12 = "M249";
			string replace12 = "m249";
			string search13 = "MAC10";
			string replace13 = "mac10";
			string search14 = "P90";
			string replace14 = "p90";
			string search15 = "UMP45";
			string replace15 = "ump45";
			string search16 = "mx1014";
			string replace16 = "xm1014";
			string search17 = "Bizon";
			string replace17 = "bizon";
			string search18 = "Mag7";
			string replace18 = "mag7";
			string search19 = "Negev";
			string replace19 = "negev";
			string search20 = "Sawedoff";
			string replace20 = "sawedoff";
			string search21 = "Tec9";
			string replace21 = "tec9";
			string search22 = "Taser";
			string replace22 = "zeus";
			string search23 = "HKP2000";
			string replace23 = "p2000";
			string search24 = "MP7";
			string replace24 = "mp7";
			string search25 = "Nova";
			string replace25 = "nova";
			string search26 = "P250";
			string replace26 = "p250";
			string search27 = "CZ75";
			string replace27 = "cz75";
			string search28 = "SCAR20";
			string replace28 = "scar20";
			string search29 = "SG556";
			string replace29 = "sg553";
			string search30 = "SSG08";
			string replace30 = "ssG08";
			string search31 = "REVOLVER";
			string replace31 = "revolver";
			string search51 = "USP_SILENCER";
			string replace51 = "usp_s";
			string search52 = "MP9";
			string replace52 = "mp9";
			string search53 = "MP5SD";
			string replace53 = "mp5-sd";

			//grenades
			string search32 = "HE_Grenade";
			string replace32 = "grenade";
			string search33 = "Smoke_Grenade";
			string replace33 = "smoke";
			string search34 = "C4";
			string replace34 = "c4";
			string search35 = "MOLOTOV";
			string replace35 = "fire_molotov";
			string search36 = "IncGrenade";
			string replace36 = "fire_inc";
			string search37 = "FLASHBANG";
			string replace37 = "flash";
			string search38 = "DECOY";
			string replace38 = "decoy";

			//others
			string search54 = "DEFUSE_KIT";
			string replace54 = "defuse_kit";

			//knifes
			string search39 = "KnifeBayonet";
			string replace39 = "Knife Bayonet";
			string search40 = "KnifeFlip";
			string replace40 = "Knife Flip";
			string search41 = "KnifeGut";
			string replace41 = "Knife Gut";
			string search42 = "KnifeM9";
			string replace42 = "Knife M9bayonet";
			string search43 = "KnifeKaram";
			string replace43 = "Knife Karambit";
			string search44 = "KnifeTactical";
			string replace44 = "Knife Huntsman";
			string search45 = "Knife_Butterfly";
			string replace45 = "Knife Butterfly";
			string search46 = "knife_falchion_advanced";
			string replace46 = "Knife Falchion";
			string search47 = "knife_push";
			string replace47 = "Knife Shadow";
			string search48 = "knife_survival_bowie";
			string replace48 = "Knife Bowie";
			string search49 = "Knife_T";
			string replace49 = "Knife T";
			string search50 = "Knife";
			string replace50 = "Knife CT";


			//weapons
			for (int i = WeaponStr.find(search); i >= 0; i = WeaponStr.find(search))
				WeaponStr.replace(i, search.size(), replace);
			for (int i = WeaponStr.find(search1); i >= 0; i = WeaponStr.find(search1))
				WeaponStr.replace(i, search1.size(), replace1);
			for (int i = WeaponStr.find(search2); i >= 0; i = WeaponStr.find(search2))
				WeaponStr.replace(i, search2.size(), replace2);
			for (int i = WeaponStr.find(search3); i >= 0; i = WeaponStr.find(search3))
				WeaponStr.replace(i, search3.size(), replace3);
			for (int i = WeaponStr.find(search4); i >= 0; i = WeaponStr.find(search4))
				WeaponStr.replace(i, search4.size(), replace4);
			for (int i = WeaponStr.find(search5); i >= 0; i = WeaponStr.find(search5))
				WeaponStr.replace(i, search5.size(), replace5);
			for (int i = WeaponStr.find(search6); i >= 0; i = WeaponStr.find(search6))
				WeaponStr.replace(i, search6.size(), replace6);
			for (int i = WeaponStr.find(search7); i >= 0; i = WeaponStr.find(search7))
				WeaponStr.replace(i, search7.size(), replace7);
			for (int i = WeaponStr.find(search8); i >= 0; i = WeaponStr.find(search8))
				WeaponStr.replace(i, search8.size(), replace8);
			for (int i = WeaponStr.find(search9); i >= 0; i = WeaponStr.find(search9))
				WeaponStr.replace(i, search9.size(), replace9);
			for (int i = WeaponStr.find(search10); i >= 0; i = WeaponStr.find(search10))
				WeaponStr.replace(i, search10.size(), replace10);
			for (int i = WeaponStr.find(search11); i >= 0; i = WeaponStr.find(search11))
				WeaponStr.replace(i, search11.size(), replace11);
			for (int i = WeaponStr.find(search12); i >= 0; i = WeaponStr.find(search12))
				WeaponStr.replace(i, search12.size(), replace12);
			for (int i = WeaponStr.find(search13); i >= 0; i = WeaponStr.find(search13))
				WeaponStr.replace(i, search13.size(), replace13);
			for (int i = WeaponStr.find(search14); i >= 0; i = WeaponStr.find(search14))
				WeaponStr.replace(i, search14.size(), replace14);
			for (int i = WeaponStr.find(search15); i >= 0; i = WeaponStr.find(search15))
				WeaponStr.replace(i, search15.size(), replace15);
			for (int i = WeaponStr.find(search16); i >= 0; i = WeaponStr.find(search16))
				WeaponStr.replace(i, search16.size(), replace16);
			for (int i = WeaponStr.find(search17); i >= 0; i = WeaponStr.find(search17))
				WeaponStr.replace(i, search17.size(), replace17);
			for (int i = WeaponStr.find(search18); i >= 0; i = WeaponStr.find(search18))
				WeaponStr.replace(i, search18.size(), replace18);
			for (int i = WeaponStr.find(search19); i >= 0; i = WeaponStr.find(search19))
				WeaponStr.replace(i, search19.size(), replace19);
			for (int i = WeaponStr.find(search20); i >= 0; i = WeaponStr.find(search20))
				WeaponStr.replace(i, search20.size(), replace20);
			for (int i = WeaponStr.find(search21); i >= 0; i = WeaponStr.find(search21))
				WeaponStr.replace(i, search21.size(), replace21);
			for (int i = WeaponStr.find(search22); i >= 0; i = WeaponStr.find(search22))
				WeaponStr.replace(i, search22.size(), replace22);
			for (int i = WeaponStr.find(search23); i >= 0; i = WeaponStr.find(search23))
				WeaponStr.replace(i, search23.size(), replace23);
			for (int i = WeaponStr.find(search24); i >= 0; i = WeaponStr.find(search24))
				WeaponStr.replace(i, search24.size(), replace24);
			for (int i = WeaponStr.find(search25); i >= 0; i = WeaponStr.find(search25))
				WeaponStr.replace(i, search25.size(), replace25);
			for (int i = WeaponStr.find(search26); i >= 0; i = WeaponStr.find(search26))
				WeaponStr.replace(i, search26.size(), replace26);
			for (int i = WeaponStr.find(search27); i >= 0; i = WeaponStr.find(search27))
				WeaponStr.replace(i, search27.size(), replace27);
			for (int i = WeaponStr.find(search28); i >= 0; i = WeaponStr.find(search28))
				WeaponStr.replace(i, search28.size(), replace28);
			for (int i = WeaponStr.find(search29); i >= 0; i = WeaponStr.find(search29))
				WeaponStr.replace(i, search29.size(), replace29);
			for (int i = WeaponStr.find(search30); i >= 0; i = WeaponStr.find(search30))
				WeaponStr.replace(i, search30.size(), replace30);
			for (int i = WeaponStr.find(search31); i >= 0; i = WeaponStr.find(search31))
				WeaponStr.replace(i, search31.size(), replace31);
			for (int i = WeaponStr.find(search32); i >= 0; i = WeaponStr.find(search32))
				WeaponStr.replace(i, search32.size(), replace32);
			for (int i = WeaponStr.find(search33); i >= 0; i = WeaponStr.find(search33))
				WeaponStr.replace(i, search33.size(), replace33);
			for (int i = WeaponStr.find(search34); i >= 0; i = WeaponStr.find(search34))
				WeaponStr.replace(i, search34.size(), replace34);
			for (int i = WeaponStr.find(search35); i >= 0; i = WeaponStr.find(search35))
				WeaponStr.replace(i, search35.size(), replace35);
			for (int i = WeaponStr.find(search36); i >= 0; i = WeaponStr.find(search36))
				WeaponStr.replace(i, search36.size(), replace36);
			for (int i = WeaponStr.find(search37); i >= 0; i = WeaponStr.find(search37))
				WeaponStr.replace(i, search37.size(), replace37);
			for (int i = WeaponStr.find(search38); i >= 0; i = WeaponStr.find(search38))
				WeaponStr.replace(i, search38.size(), replace38);
			for (int i = WeaponStr.find(search39); i >= 0; i = WeaponStr.find(search39))
				WeaponStr.replace(i, search39.size(), replace39);
			for (int i = WeaponStr.find(search40); i >= 0; i = WeaponStr.find(search40))
				WeaponStr.replace(i, search40.size(), replace40);
			for (int i = WeaponStr.find(search41); i >= 0; i = WeaponStr.find(search41))
				WeaponStr.replace(i, search41.size(), replace41);
			for (int i = WeaponStr.find(search42); i >= 0; i = WeaponStr.find(search42))
				WeaponStr.replace(i, search42.size(), replace42);
			for (int i = WeaponStr.find(search43); i >= 0; i = WeaponStr.find(search43))
				WeaponStr.replace(i, search43.size(), replace43);
			for (int i = WeaponStr.find(search44); i >= 0; i = WeaponStr.find(search44))
				WeaponStr.replace(i, search44.size(), replace44);
			for (int i = WeaponStr.find(search45); i >= 0; i = WeaponStr.find(search45))
				WeaponStr.replace(i, search45.size(), replace45);
			for (int i = WeaponStr.find(search46); i >= 0; i = WeaponStr.find(search46))
				WeaponStr.replace(i, search46.size(), replace46);
			for (int i = WeaponStr.find(search47); i >= 0; i = WeaponStr.find(search47))
				WeaponStr.replace(i, search47.size(), replace47);
			for (int i = WeaponStr.find(search48); i >= 0; i = WeaponStr.find(search48))
				WeaponStr.replace(i, search48.size(), replace48);
			for (int i = WeaponStr.find(search49); i >= 0; i = WeaponStr.find(search49))
				WeaponStr.replace(i, search49.size(), replace49);
			for (int i = WeaponStr.find(search50); i >= 0; i = WeaponStr.find(search50))
				WeaponStr.replace(i, search50.size(), replace50);
			for (int i = WeaponStr.find(search51); i >= 0; i = WeaponStr.find(search51))
				WeaponStr.replace(i, search51.size(), replace51);
			for (int i = WeaponStr.find(search52); i >= 0; i = WeaponStr.find(search52))
				WeaponStr.replace(i, search52.size(), replace52);
			for (int i = WeaponStr.find(search53); i >= 0; i = WeaponStr.find(search53))
				WeaponStr.replace(i, search53.size(), replace53);
			for (int i = WeaponStr.find(search54); i >= 0; i = WeaponStr.find(search54))
				WeaponStr.replace(i, search54.size(), replace54);


			if (Settings::Esp::esp_Ammo && pPlayer->iWAmmo)
			{
				WeaponStr += "   [";
				WeaponStr += to_string(pPlayer->iWAmmo);
				WeaponStr += "]";
			}

			g_pRender->Text((int)vLineOrigin.x, (int)vLineOrigin.y + iHpAmY, true, true, EspVisibleColor, WeaponStr.c_str());
			iHpAmY += 10;
		}break;
	case 2:
		if (!pPlayer->WeaponName.empty() && /*IsLocalAlive()*/Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
		{
			string WeaponStr = pPlayer->WeaponName;
			//weapons

			string search = "M4A1";
			string replace = "S";
			string search1 = "M4_SILENCER";
			string replace1 = "T";
			string search2 = "AK47";
			string replace2 = "W";
			string search3 = "Elites";
			string replace3 = "B";
			string search4 = "DesertEagle";
			string replace4 = "A";
			string search5 = "Aug";
			string replace5 = "U";
			string search6 = "AWP";
			string replace6 = "Z";
			string search7 = "Famas";
			string replace7 = "R";
			string search8 = "FiveSeven";
			string replace8 = "C";
			string search9 = "G3SG1";
			string replace9 = "X";
			string search10 = "GalilAR";
			string replace10 = "Q";
			string search11 = "Glock18";
			string replace11 = "D";
			string search12 = "M249";
			string replace12 = "g";
			string search13 = "MAC10";
			string replace13 = "K";
			string search14 = "P90";
			string replace14 = "P";
			string search15 = "UMP45";
			string replace15 = "L";
			string search16 = "xm1014";
			string replace16 = "b";
			string search17 = "Bizon";
			string replace17 = "M";
			string search18 = "Mag7";
			string replace18 = "d";
			string search19 = "Negev";
			string replace19 = "f";
			string search20 = "Sawedoff";
			string replace20 = "c";
			string search21 = "Tec9";
			string replace21 = "H";
			string search22 = "Taser";
			string replace22 = "h";
			string search23 = "HKP2000";
			string replace23 = "E";
			string search24 = "MP7";
			string replace24 = "N";
			string search25 = "Nova";
			string replace25 = "e";
			string search26 = "P250";
			string replace26 = "F";
			string search27 = "CZ75";
			string replace27 = "I";
			string search28 = "SCAR20";
			string replace28 = "Y";
			string search29 = "SG556";
			string replace29 = "V";
			string search30 = "SSG08";
			string replace30 = "a";
			string search31 = "REVOLVER";
			string replace31 = "J";
			string search51 = "USP_SILENCER";
			string replace51 = "G";
			string search52 = "MP9";
			string replace52 = "O";
			string search53 = "MP5SD";
			string replace53 = "z";
			//grenades
			string search32 = "HE_Grenade";
			string replace32 = "j";
			string search33 = "Smoke_Grenade";
			string replace33 = "k";
			string search34 = "C4";
			string replace34 = "o";
			string search35 = "MOLOTOV";
			string replace35 = "l";
			string search36 = "IncGrenade";
			string replace36 = "n";
			string search37 = "FLASHBANG";
			string replace37 = "i";
			string search38 = "DECOY";
			string replace38 = "m";
			//Others
			string search54 = "DEFUSE_KIT";
			string replace54 = "r";
			//knifes
			string search39 = "KnifeBayonet";
			string replace39 = "1";
			string search40 = "KnifeFlip";
			string replace40 = "2";
			string search41 = "KnifeGut";
			string replace41 = "3";
			string search42 = "KnifeM9";
			string replace42 = "5";
			string search43 = "KnifeKaram";
			string replace43 = "4";
			string search44 = "KnifeTactical";
			string replace44 = "6";
			string search45 = "Knife_Butterfly";
			string replace45 = "8";
			string search46 = "knife_falchion_advanced";
			string replace46 = "0";
			string search47 = "knife_push";
			string replace47 = "9";
			string search48 = "knife_survival_bowie";
			string replace48 = "7";
			string search49 = "Knife_T";
			string replace49 = "[";
			string search50 = "Knife";
			string replace50 = "]";


			//weapons
			for (int i = WeaponStr.find(search); i >= 0; i = WeaponStr.find(search))
				WeaponStr.replace(i, search.size(), replace);
			for (int i = WeaponStr.find(search1); i >= 0; i = WeaponStr.find(search1))
				WeaponStr.replace(i, search1.size(), replace1);
			for (int i = WeaponStr.find(search2); i >= 0; i = WeaponStr.find(search2))
				WeaponStr.replace(i, search2.size(), replace2);
			for (int i = WeaponStr.find(search3); i >= 0; i = WeaponStr.find(search3))
				WeaponStr.replace(i, search3.size(), replace3);
			for (int i = WeaponStr.find(search4); i >= 0; i = WeaponStr.find(search4))
				WeaponStr.replace(i, search4.size(), replace4);
			for (int i = WeaponStr.find(search5); i >= 0; i = WeaponStr.find(search5))
				WeaponStr.replace(i, search5.size(), replace5);
			for (int i = WeaponStr.find(search6); i >= 0; i = WeaponStr.find(search6))
				WeaponStr.replace(i, search6.size(), replace6);
			for (int i = WeaponStr.find(search7); i >= 0; i = WeaponStr.find(search7))
				WeaponStr.replace(i, search7.size(), replace7);
			for (int i = WeaponStr.find(search8); i >= 0; i = WeaponStr.find(search8))
				WeaponStr.replace(i, search8.size(), replace8);
			for (int i = WeaponStr.find(search9); i >= 0; i = WeaponStr.find(search9))
				WeaponStr.replace(i, search9.size(), replace9);
			for (int i = WeaponStr.find(search10); i >= 0; i = WeaponStr.find(search10))
				WeaponStr.replace(i, search10.size(), replace10);
			for (int i = WeaponStr.find(search11); i >= 0; i = WeaponStr.find(search11))
				WeaponStr.replace(i, search11.size(), replace11);
			for (int i = WeaponStr.find(search12); i >= 0; i = WeaponStr.find(search12))
				WeaponStr.replace(i, search12.size(), replace12);
			for (int i = WeaponStr.find(search13); i >= 0; i = WeaponStr.find(search13))
				WeaponStr.replace(i, search13.size(), replace13);
			for (int i = WeaponStr.find(search14); i >= 0; i = WeaponStr.find(search14))
				WeaponStr.replace(i, search14.size(), replace14);
			for (int i = WeaponStr.find(search15); i >= 0; i = WeaponStr.find(search15))
				WeaponStr.replace(i, search15.size(), replace15);
			for (int i = WeaponStr.find(search16); i >= 0; i = WeaponStr.find(search16))
				WeaponStr.replace(i, search16.size(), replace16);
			for (int i = WeaponStr.find(search17); i >= 0; i = WeaponStr.find(search17))
				WeaponStr.replace(i, search17.size(), replace17);
			for (int i = WeaponStr.find(search18); i >= 0; i = WeaponStr.find(search18))
				WeaponStr.replace(i, search18.size(), replace18);
			for (int i = WeaponStr.find(search19); i >= 0; i = WeaponStr.find(search19))
				WeaponStr.replace(i, search19.size(), replace19);
			for (int i = WeaponStr.find(search20); i >= 0; i = WeaponStr.find(search20))
				WeaponStr.replace(i, search20.size(), replace20);
			for (int i = WeaponStr.find(search21); i >= 0; i = WeaponStr.find(search21))
				WeaponStr.replace(i, search21.size(), replace21);
			for (int i = WeaponStr.find(search22); i >= 0; i = WeaponStr.find(search22))
				WeaponStr.replace(i, search22.size(), replace22);
			for (int i = WeaponStr.find(search23); i >= 0; i = WeaponStr.find(search23))
				WeaponStr.replace(i, search23.size(), replace23);
			for (int i = WeaponStr.find(search24); i >= 0; i = WeaponStr.find(search24))
				WeaponStr.replace(i, search24.size(), replace24);
			for (int i = WeaponStr.find(search25); i >= 0; i = WeaponStr.find(search25))
				WeaponStr.replace(i, search25.size(), replace25);
			for (int i = WeaponStr.find(search26); i >= 0; i = WeaponStr.find(search26))
				WeaponStr.replace(i, search26.size(), replace26);
			for (int i = WeaponStr.find(search27); i >= 0; i = WeaponStr.find(search27))
				WeaponStr.replace(i, search27.size(), replace27);
			for (int i = WeaponStr.find(search28); i >= 0; i = WeaponStr.find(search28))
				WeaponStr.replace(i, search28.size(), replace28);
			for (int i = WeaponStr.find(search29); i >= 0; i = WeaponStr.find(search29))
				WeaponStr.replace(i, search29.size(), replace29);
			for (int i = WeaponStr.find(search30); i >= 0; i = WeaponStr.find(search30))
				WeaponStr.replace(i, search30.size(), replace30);
			for (int i = WeaponStr.find(search31); i >= 0; i = WeaponStr.find(search31))
				WeaponStr.replace(i, search31.size(), replace31);
			for (int i = WeaponStr.find(search32); i >= 0; i = WeaponStr.find(search32))
				WeaponStr.replace(i, search32.size(), replace32);
			for (int i = WeaponStr.find(search33); i >= 0; i = WeaponStr.find(search33))
				WeaponStr.replace(i, search33.size(), replace33);
			for (int i = WeaponStr.find(search34); i >= 0; i = WeaponStr.find(search34))
				WeaponStr.replace(i, search34.size(), replace34);
			for (int i = WeaponStr.find(search35); i >= 0; i = WeaponStr.find(search35))
				WeaponStr.replace(i, search35.size(), replace35);
			for (int i = WeaponStr.find(search36); i >= 0; i = WeaponStr.find(search36))
				WeaponStr.replace(i, search36.size(), replace36);
			for (int i = WeaponStr.find(search37); i >= 0; i = WeaponStr.find(search37))
				WeaponStr.replace(i, search37.size(), replace37);
			for (int i = WeaponStr.find(search38); i >= 0; i = WeaponStr.find(search38))
				WeaponStr.replace(i, search38.size(), replace38);
			for (int i = WeaponStr.find(search39); i >= 0; i = WeaponStr.find(search39))
				WeaponStr.replace(i, search39.size(), replace39);
			for (int i = WeaponStr.find(search40); i >= 0; i = WeaponStr.find(search40))
				WeaponStr.replace(i, search40.size(), replace40);
			for (int i = WeaponStr.find(search41); i >= 0; i = WeaponStr.find(search41))
				WeaponStr.replace(i, search41.size(), replace41);
			for (int i = WeaponStr.find(search42); i >= 0; i = WeaponStr.find(search42))
				WeaponStr.replace(i, search42.size(), replace42);
			for (int i = WeaponStr.find(search43); i >= 0; i = WeaponStr.find(search43))
				WeaponStr.replace(i, search43.size(), replace43);
			for (int i = WeaponStr.find(search44); i >= 0; i = WeaponStr.find(search44))
				WeaponStr.replace(i, search44.size(), replace44);
			for (int i = WeaponStr.find(search45); i >= 0; i = WeaponStr.find(search45))
				WeaponStr.replace(i, search45.size(), replace45);
			for (int i = WeaponStr.find(search46); i >= 0; i = WeaponStr.find(search46))
				WeaponStr.replace(i, search46.size(), replace46);
			for (int i = WeaponStr.find(search47); i >= 0; i = WeaponStr.find(search47))
				WeaponStr.replace(i, search47.size(), replace47);
			for (int i = WeaponStr.find(search48); i >= 0; i = WeaponStr.find(search48))
				WeaponStr.replace(i, search48.size(), replace48);
			for (int i = WeaponStr.find(search49); i >= 0; i = WeaponStr.find(search49))
				WeaponStr.replace(i, search49.size(), replace49);
			for (int i = WeaponStr.find(search50); i >= 0; i = WeaponStr.find(search50))
				WeaponStr.replace(i, search50.size(), replace50);
			for (int i = WeaponStr.find(search51); i >= 0; i = WeaponStr.find(search51))
				WeaponStr.replace(i, search51.size(), replace51);
			for (int i = WeaponStr.find(search52); i >= 0; i = WeaponStr.find(search52))
				WeaponStr.replace(i, search52.size(), replace52);
			for (int i = WeaponStr.find(search53); i >= 0; i = WeaponStr.find(search53))
				WeaponStr.replace(i, search53.size(), replace53);
			for (int i = WeaponStr.find(search54); i >= 0; i = WeaponStr.find(search54))
				WeaponStr.replace(i, search54.size(), replace54);

			g_pRender->WepIcon((int)vLineOrigin.x, (int)vLineOrigin.y + iHpAmY, true, true, EspVisibleColor, WeaponStr.c_str());
			iHpAmY += 10;
		}
		break;
	}
}

void CEsp::DrawPlayerSkeleton(CPlayer* pPlayer)
{
	Color EspVisibleColor = GetPlayerVisibleColor(pPlayer);

	for (BYTE IndexArray = 0; IndexArray < 18; IndexArray++)
	{
		DrawHitBoxLine(pPlayer->vHitboxSkeletonArray[IndexArray], EspVisibleColor);
	}
}

void CEsp::DrawPlayerBulletTrace(CPlayer* pPlayer)
{
	Color EspColor = GetPlayerColor(pPlayer);

	if (!pPlayer->vBulletTraceArray[0].IsZero() && !pPlayer->vBulletTraceArray[1].IsZero())
	{
		g_pRender->DrawLine(
			(int)pPlayer->vBulletTraceArray[0].x, (int)pPlayer->vBulletTraceArray[0].y,
			(int)pPlayer->vBulletTraceArray[1].x, (int)pPlayer->vBulletTraceArray[1].y, EspColor);

		g_pRender->DrawFillBox((int)pPlayer->vBulletTraceArray[1].x - 2, (int)pPlayer->vBulletTraceArray[1].y - 2, 4, 4, EspColor);
	}
}

void CEsp::DrawHitBoxLine(Vector* vHitBoxArray, Color color)
{
	Vector vHitBoxOneScreen;
	Vector vHitBoxTwoScreen;

	if (vHitBoxArray[0].IsZero() || !vHitBoxArray[0].IsValid() || vHitBoxArray[1].IsZero() || !vHitBoxArray[1].IsValid())
		return;

	if (WorldToScreen(vHitBoxArray[0], vHitBoxOneScreen) && WorldToScreen(vHitBoxArray[1], vHitBoxTwoScreen))
	{
		g_pRender->DrawLine((int)vHitBoxOneScreen.x, (int)vHitBoxOneScreen.y,
			(int)vHitBoxTwoScreen.x, (int)vHitBoxTwoScreen.y, color);
	}
}