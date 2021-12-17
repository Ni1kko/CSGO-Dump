 #include "render.h"
#include "../globals.h"
#include "../options.hpp"
#include "../helpers/imdraw.h"
#include "../helpers/entities.h"
#include "../features/features.h"
#include "..//helpers/notifies.h"

namespace render
{
	namespace timers
	{
		auto m_Framerate = 0.f;

		ConVar* cl_hud_playercount_pos = nullptr;

		decltype(entities::m_local) m_local;

		void render_panel()
		{
			if (!cl_hud_playercount_pos)
				cl_hud_playercount_pos = interfaces::cvar->find("cl_hud_playercount_pos");

			if (!cl_hud_playercount_pos)
				return;

			ImGui::PushFont(render::fonts::low_size);

			const auto screen_size = ImGui::GetIO().DisplaySize;

			const auto is_top = cl_hud_playercount_pos->GetBool();

			const auto start_color = ImGui::GetColorU32(ImVec4(0.1f, 0.1f, 0.1f, 0.0f));
			const auto end_color = ImGui::GetColorU32(ImVec4(0.1f, 0.1f, 0.1f, 1.f));

			static const auto fps_text_size = ImGui::CalcTextSize("FPS");

			const auto start_pos = ImVec2(screen_size.x / 2.f - 240.f, (is_top ? fps_text_size.y + 4.f : screen_size.y - fps_text_size.y - 4.f));
			const auto end_pos = ImVec2(screen_size.x / 2.f + 240.f, is_top ? 0.f : screen_size.y);

			globals::draw_list->AddRectFilledMultiColor(
				start_pos, ImVec2(start_pos.x + 240.f, end_pos.y), start_color, end_color, end_color, start_color
			);

			globals::draw_list->AddRectFilledMultiColor(
				ImVec2(start_pos.x + 240.f, start_pos.y), end_pos, end_color, start_color, start_color, end_color
			);

			char fps[256];
			sprintf(fps, "%4i", (int)(1.0f / m_Framerate));

			ImVec2 txt_size;

			//256 FPS - 36 and +-10 px
			txt_size = ImGui::CalcTextSize(fps);
			auto text_pos = ImVec2(start_pos.x + 240.f - 2.f - txt_size.x, !is_top ? end_pos.y - 2.f - txt_size.y : end_pos.y + 2.f);

			const auto fps_color = interfaces::global_vars->interval_per_tick * 0.8f < interfaces::global_vars->absoluteframetime ? ImVec4(1.f, 0.37f, 0.15f, 1.f) : ImVec4(0.64f, 0.82f, 0.45f, 1.f);

			//imdraw::outlined_text(fps, text_pos, ImGui::GetColorU32(fps_color));

			txt_size = ImGui::CalcTextSize("FPS");
			text_pos = ImVec2(start_pos.x + 240.f + 2.f, !is_top ? end_pos.y - 2.f - txt_size.y : end_pos.y + 2.f);
			//imdraw::outlined_text("FPS", text_pos, ImGui::GetColorU32(ImVec4::White));

			//

			if (m_local.is_matchmaking)
			{
				char bomb_time[16];
				sprintf_s(bomb_time, "%.2f", m_local.bomb_time);

				const auto ping_text_size = ImGui::CalcTextSize("BOMB");

				txt_size = ImGui::CalcTextSize(bomb_time);

				const auto bomb_color = m_local.bomb_time > 0.f ? ImVec4(0.88f, 0.82f, 0.45f, 1.f) : ImVec4(1.f, 1.f, 1.f, 1.f);


				text_pos = ImVec2(start_pos.x + 240.f - 2.f - txt_size.x - 25.f - 30.f - ping_text_size.x - 4.f - txt_size.x, !is_top ? end_pos.y - 2.f - txt_size.y : end_pos.y + 2.f);
				imdraw::outlined_text(bomb_time, text_pos, ImGui::GetColorU32(bomb_color));

				if (m_local.AfterPlant == false)
				{
					text_pos = ImVec2(start_pos.x + 240.f - 2.f - txt_size.x - 25.f - 30.f - ping_text_size.x, !is_top ? end_pos.y - 2.f - ping_text_size.y : end_pos.y + 2.f);
					imdraw::outlined_text("BOMB", text_pos, ImGui::GetColorU32(ImVec4::White));
				}

				if (m_local.bomb_indexStatus == 0 && m_local.isBombPlantedStatus && m_local.AfterPlant == true)
				{
					text_pos = ImVec2(start_pos.x + 240.f - 2.f - txt_size.x - 25.f - 30.f - ping_text_size.x, !is_top ? end_pos.y - 2.f - ping_text_size.y : end_pos.y + 2.f);
					imdraw::outlined_text("BOMB A", text_pos, ImGui::GetColorU32(ImVec4::White));
				}
				else if (m_local.bomb_indexStatus == 1 && m_local.isBombPlantedStatus && m_local.AfterPlant == true)
				{
					text_pos = ImVec2(start_pos.x + 240.f - 2.f - txt_size.x - 25.f - 30.f - ping_text_size.x, !is_top ? end_pos.y - 2.f - ping_text_size.y : end_pos.y + 2.f);
					imdraw::outlined_text("BOMB B", text_pos, ImGui::GetColorU32(ImVec4::White));
				}

				//

				char defuse_time[16];
				sprintf_s(defuse_time, "%.2f", m_local.defuse_time);

				txt_size = ImGui::CalcTextSize(defuse_time);
				text_pos = ImVec2(start_pos.x + 240.f + 2.f + 25.f + 30.f + 22.f - txt_size.x - 4.f, !is_top ? end_pos.y - 2.f - txt_size.y : end_pos.y + 2.f);

				const auto defuse_color = m_local.defuse_time > 0.f ? ImVec4(0.f, 1.f, 0.15f, 1.f) : ImVec4(1.f, 1.f, 1.f, 1.f);
				imdraw::outlined_text(defuse_time, text_pos, ImGui::GetColorU32(defuse_color));

				const auto speed_text_size = ImGui::CalcTextSize("DEFUSE");

				text_pos = ImVec2(start_pos.x + 240.f + 2.f + 25.f + 30.f + 22.f, !is_top ? end_pos.y - 2.f - speed_text_size.y : end_pos.y + 2.f);
				imdraw::outlined_text("DEFUSE", text_pos, ImGui::GetColorU32(ImVec4::White));

				//

				char hp[16];
				sprintf_s(hp, "%d", m_local.hp);

				const auto TXTsize = ImGui::CalcTextSize("HP");

				txt_size = ImGui::CalcTextSize(hp);

				//const auto bomb_color = m_local.bomb_time > 0.f ? ImVec4(0.88f, 0.82f, 0.45f, 1.f) : ImVec4(1.f, 1.f, 1.f, 1.f);



				if (g::local_player->IsAlive() && m_local.damage <= g::local_player->m_iHealth()) {
					text_pos = ImVec2(start_pos.x + 240.f - 2.f - txt_size.x, !is_top ? end_pos.y - 2.f - txt_size.y : end_pos.y + 2.f);
					imdraw::outlined_text(hp, text_pos, ImGui::GetColorU32(bomb_color));

					text_pos = ImVec2(start_pos.x + 240.f + 2.f, !is_top ? end_pos.y - 2.f - txt_size.y : end_pos.y + 2.f);
					imdraw::outlined_text("HP", text_pos, ImGui::GetColorU32(ImVec4::White));
				}

				//render fatal check
				if (g::local_player->IsAlive() && m_local.damage >= g::local_player->m_iHealth()) {
					auto txt_size2 = ImGui::CalcTextSize("0");
					text_pos = ImVec2(start_pos.x + 240.f - 2.f - txt_size2.x, !is_top ? end_pos.y - 2.f - txt_size2.y : end_pos.y + 2.f);
					imdraw::outlined_text("0", text_pos, ImGui::GetColorU32(ImVec4::Red));

					text_pos = ImVec2(start_pos.x + 240.f + 2.f, !is_top ? end_pos.y - 2.f - txt_size2.y : end_pos.y + 2.f);
					imdraw::outlined_text("HP", text_pos, ImGui::GetColorU32(ImVec4::White));
				}
			}

			ImGui::PopFont();
		}

		void show()
		{
			m_Framerate = 0.9 * m_Framerate + (1.0 - 0.9) * interfaces::global_vars->absoluteframetime;

			if (!interfaces::local_player || !render::fonts::low_size)
				return;

			if (entities::local_mutex.try_lock())
			{
				m_local = entities::m_local;

				entities::local_mutex.unlock();
			}

			render_panel();
		}
	}
}