#include "Speck.h"
#include "../../includes.hpp"
#include "..\..\sdk\structs.hpp"
std::vector<int> speclist::GetObservervators(int playerId)
{
	std::vector<int> SpectatorList;

	player_t* pPlayer = (player_t*)g_csgo.m_entitylist()->GetClientEntity(playerId);

	if (!pPlayer)
		return SpectatorList;

	if (pPlayer->is_alive())
	{
		player_t* pObserverTarget = (player_t*)g_csgo.m_entitylist()->GetClientEntityFromHandle(pPlayer->m_hObserverTarget());

		if (!pObserverTarget)
			return SpectatorList;

		pPlayer = pObserverTarget;
	}
	//g_pPlayers
	player_info_t Pinfo;
	g_csgo.m_engine()->GetPlayerInfo(playerId, &Pinfo);

	for (int PlayerIndex = 0; PlayerIndex < Pinfo.userId; PlayerIndex++)
	{
		player_t* pCheckPlayer = (player_t*)g_csgo.m_entitylist()->GetClientEntity(PlayerIndex);

		if (!pCheckPlayer)
			continue;

		if (pCheckPlayer->IsDormant() || !pCheckPlayer->is_alive())
			continue;

		player_t* pObserverTarget = (player_t*)g_csgo.m_entitylist()->GetClientEntityFromHandle(pCheckPlayer->m_hObserverTarget());

		if (!pObserverTarget)
			continue;

		if (pPlayer != pObserverTarget)
			continue;

		SpectatorList.push_back(PlayerIndex);
	}

	return SpectatorList;
}

void speclist::OnRenderSpectatorList()
{
	if (g_cfg.misc.spec_list && g_csgo.m_engine()->IsInGame())
	{
		int DrawIndex = 1;
		std::string spectators;
		for (int playerId : GetObservervators(g_csgo.m_engine()->GetLocalPlayer()))
		{
			if (playerId == g_csgo.m_engine()->GetLocalPlayer())
				continue;

			player_t* pPlayer = (player_t*)g_csgo.m_entitylist()->GetClientEntity(playerId);

			if (!pPlayer)
				continue;

			player_info_t Pinfo;
			g_csgo.m_engine()->GetPlayerInfo(playerId, &Pinfo);

			if (Pinfo.fakeplayer)
				continue;

			std::string Name = Pinfo.szName;

			spectators += std::string(Pinfo.szName) + " > " + Pinfo.szName + "\n";

			if (Name != "")
			{
				int
					screen_x = render::get().viewport().right / 2,
					screen_y = render::get().viewport().left / 2,
					screen_w = render::get().viewport().top / 2,
					screen_h = render::get().viewport().bottom / 2;

				render::get().rect_filled(screen_x - 110, screen_y - 110, screen_w - 110, screen_h - 110, Color(44, 44, 44));
				render::get().text(fonts[esp], screen_w - 100, screen_h - 100, Color(255, 255, 255), HFONT_CENTERED_NONE, spectators.c_str());
			}
		}
	}
}
