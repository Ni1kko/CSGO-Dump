 #include "render.h"
#include "../options.hpp"
#include "../globals.h"

namespace render
{
	namespace spectators
	{
		void show()
		{
			if (!render::fonts::spectators || !interfaces::local_player)
				return;

			c_base_player* player = interfaces::local_player;
			if (player->m_hObserverTarget())
				player = c_base_player::GetEntityFromHandle<c_base_player>(player->m_hObserverTarget());

			if (!player || !player->IsPlayer())
				return;

			auto offset_y = 5.f;
			const auto screen_size = ImGui::GetIO().DisplaySize;
			const auto color = ImGui::GetColorU32(player->IsFlashed() ? ImVec4::Black : ImVec4::White);

			for (int i = 1; i < interfaces::engine_client->GetMaxClients(); ++i)
			{
				auto entity = c_base_player::GetPlayerByIndex(i);
				if (!entity || !entity->IsPlayer() || entity->is_dormant() || entity->IsAlive() || entity == player || entity == interfaces::local_player)
					continue;

				const auto observer_target = c_base_player::GetEntityFromHandle(entity->m_hObserverTarget());
				if (!observer_target || observer_target != player)
					continue;

				auto info = entity->GetPlayerInfo();
				if (!info.fakeplayer)
				{
					const auto text_size = ImGui::CalcTextSize(info.szName);
					const auto pos = ImVec2(screen_size.x - text_size.x - 5.f, offset_y);

					globals::draw_list->AddText(pos, color, info.szName);

					offset_y += text_size.y + 5.f;
				}
			}
		}
	}
}
