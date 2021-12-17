#include "hooks.hpp"
void __fastcall Hooks::SuppressLists(void* _this, int, int a2, bool a3)
{
	static auto ofunc = partition_hook.get_original<decltype(&SuppressLists)>(index::SuppressLists);

	static QAngle aimPunch, viewPunch;
	if (_ReturnAddress() == g_Memory->FrameRenderStart)
	{
		if (g_LocalPlayer)
		{
			auto state = g_LocalPlayer->GetPlayerAnimState();
			if (state)
			{
				if (g_LocalPlayer->m_vecVelocity().Length2D() < 3.f)
				{
					*reinterpret_cast<float*> (reinterpret_cast<uintptr_t> (state) + 292) = 0.f;
				}
				g_LocalPlayer->SetAbsAngles(QAngle(0.0f, g_LocalPlayer->GetPlayerAnimState()->m_flGoalFeetYaw, 0.0f));
			}
			if (g_Input->m_fCameraInThirdPerson)
			{
				g_LocalPlayer->GetVAngles() = tpangle;
				if (!g_LocalPlayer->IsAlive())
				{
					g_LocalPlayer->m_iObserverMode() = 5;
				}
				g_LocalPlayer->UpdateClientSideAnimation();
			}
		}
		g_Misc->HUDModulation();
		g_Misc->NightMode();
		if (g_LocalPlayer && g_LocalPlayer->IsAlive())
		{
			if (g_Options.misc.removals.no_visual_recoil)
			{
				aimPunch = g_LocalPlayer->m_aimPunchAngle();
				viewPunch = g_LocalPlayer->m_viewPunchAngle();

				g_LocalPlayer->m_aimPunchAngle() = QAngle{};
				g_LocalPlayer->m_viewPunchAngle() = QAngle{};
			}
		}

		static ConVar* zoom_sensitivity_ratio_mouse = g_CVar->FindVar("zoom_sensitivity_ratio_mouse");
		if (g_Options.misc.removals.no_zoom)
			zoom_sensitivity_ratio_mouse->SetValue(0);
		else
			zoom_sensitivity_ratio_mouse->SetValue(1);
		static ConVar* PostProcVar = g_CVar->FindVar("mat_postprocess_enable");

		if (g_Options.misc.removals.no_postprocess)
			PostProcVar->SetValue(0);
		else
			PostProcVar->SetValue(1);
	}
	else if (_ReturnAddress() == g_Memory->FrameRenderEnd)
	{
		if (!aimPunch.IsZero() || !viewPunch.IsZero())
		{
			g_LocalPlayer->m_aimPunchAngle() = aimPunch;
			g_LocalPlayer->m_viewPunchAngle() = viewPunch;

			aimPunch = QAngle{};
			viewPunch = QAngle{};
		}
	}
	else if (_ReturnAddress() == g_Memory->FrameNetUpdateStart)
	{
		if (g_Options.esp.sound_enemies || g_Options.esp.sound_teammates || g_Options.esp.sound_local)
		{
			static CUtlVector<SndInfo_t> sndList;
			g_EngineSound->GetActiveSounds(sndList);

			for (auto i = 0; i < sndList.Count(); i++)
			{
				const SndInfo_t sound = sndList.Element(i);

				if (!sound.m_nSoundSource)
					continue;

				if (sound.m_nChannel != 4)
					continue;

				if (!sound.m_bUpdatePositions)
					continue;

				C_BasePlayer* entity = C_BasePlayer::GetPlayerByIndex(sound.m_nSoundSource);

				if (!entity || !entity->IsPlayer() || !entity->IsAlive())
					continue;

				const bool is_enemy = entity->IsEnemy();

				const bool is_local = (entity == g_LocalPlayer);

				if (is_enemy && (!g_Options.esp.player_enemies || !g_Options.esp.sound_enemies))
					continue;

				if (!is_enemy && !is_local && (!g_Options.esp.player_teammates || !g_Options.esp.sound_teammates))
					continue;

				if (!is_enemy && !is_local && (!g_Options.esp.player_local || !g_Options.esp.sound_local))
					continue;

				if (!sound.m_pOrigin)
					continue;

				g_SoundEsp->AddSound(*sound.m_pOrigin, is_enemy, is_local);
			}

			sndList.RemoveAll();
		}
	}
	else if (_ReturnAddress() == g_Memory->FrameNetUpdateEnd)
	{
		g_Skins->FrameStageNotify(true);
		for (int i = 1; i <= g_GlobalVars->maxClients; i++)
		{
			C_BasePlayer* entity = C_BasePlayer::GetPlayerByIndex(i);
			if (!g_Ragebot->IsViable(entity)) continue;

			if (g_Options.ragebotconfig.bEnabled)
			{
				g_Ragebot->AnimationFix(entity);
			}
		}
	}

	ofunc(g_SpatialPartition, 0, a2, a3);
}