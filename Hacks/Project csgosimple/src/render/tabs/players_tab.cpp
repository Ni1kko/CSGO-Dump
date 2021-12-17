#include "../render.h"
#include "../../globals.h"

namespace render
{
	namespace menu
	{
		const auto blue = ImVec4(0.05f, 0.4f, 0.7f, 1.f);
		const auto orange = ImVec4(0.7f, 0.4f, 0.05f, 1.f);

		struct player_info_t
		{
			bool is_ct_team;
			std::string name;
			int wins;
			int rank;
			int money;
			int id;
		};

		std::vector<player_info_t> list;

		void render_players(const std::vector<player_info_t>& list, const bool& ct_team)
		{
			auto is_first = true;
			const std::vector<const char*> ranks =
			{
				___("Unknown", u8"����������"),
				___("Silver I", u8"������� - I"),
				___("Silver II", u8"������� - II"),
				___("Silver III", u8"������� - III"),
				___("Silver IV", u8"������� - IV"),
				___("Silver Elite", u8"������� - �����"),
				___("Silver Elite Master", u8"������� - ������� �������"),
				___("Gold Nova I", u8"������� ������ - I"),
				___("Gold Nova II", u8"������� ������ - II"),
				___("Gold Nova III", u8"������� ������ - III"),
				___("Gold Nova Master", u8"������� ������ - �������"),
				___("Master Guardian I", u8"�������-��������� - I"),
				___("Master Guardian II", u8"�������-��������� - II"),
				___("Master Guardian Elite", u8"�������-��������� - �����"),
				___("Distinguished Master Guardian", u8"����������� �������-���������"),
				___("Legendary Eagle", u8"����������� ������"),
				___("Legendary Eagle Master", u8"����������� ������-�������"),
				___("Supreme Master First Class", u8"������� ������� ������� �����"),
				___("The Global Elite", u8"��������� �����"),
			};

			for (const auto& item : list)
			{
				if ((ct_team && !item.is_ct_team) || (!ct_team && item.is_ct_team))
					continue;

				if (is_first)
				{
					is_first = false;
					ImGui::Separator();
				}

				columns(5);

				ImGui::SetColumnWidth(-1, 200.f);
				ImGui::PushStyleColor(ImGuiCol_Text, item.is_ct_team ? blue : orange);

				ImGui::Text(item.name.c_str());

				ImGui::NextColumn();

				ImGui::SetColumnWidth(-1, 60.f);
				ImGui::Text("$%d", item.money);

				ImGui::NextColumn();

				ImGui::SetColumnWidth(-1, 60.f);
				ImGui::Text("%d", item.wins);

				ImGui::NextColumn();

				ImGui::SetColumnWidth(-1, 60.f);
				ImGui::Text("%d", globals::team_damage[item.id]);

				ImGui::NextColumn();

				if (item.rank == 0)
					ImGui::Text("%s", ranks[item.rank]);
				else
					ImGui::Text("[%d] %s", item.rank, ranks[item.rank]);

				ImGui::PopStyleColor();

				columns(1);
			}
		}

		void players_tab()
		{
			columns(5);
			{
				ImGui::SetColumnWidth(-1, 200.f);
				ImGui::Text(___("Name", u8"���"));

				ImGui::NextColumn();

				ImGui::SetColumnWidth(-1, 60.f);
				ImGui::Text(___("Money", u8"������"));

				ImGui::NextColumn();

				ImGui::SetColumnWidth(-1, 60.f);
				ImGui::Text(___("Wins", u8"������"));

				ImGui::NextColumn();

				ImGui::SetColumnWidth(-1, 60.f);
				ImGui::Text(___("Damage", u8"�����"));

				ImGui::NextColumn();

				ImGui::Text(___("Rank", u8"������"));
			}
			columns(1);

			list.clear();

			auto player_resource = *interfaces::player_resource;
			if (!player_resource)
				return;

			for (int i = 1; i < interfaces::engine_client->GetMaxClients(); ++i)
			{
				auto* player = c_base_player::GetPlayerByIndex(i);
				if (!player || !player->IsPlayer())
					continue;

				if (player->m_iTeamNum() != team::team_ct && player->m_iTeamNum() != team::team_t)
					continue;

				const auto info = player->GetPlayerInfo();
				if (info.fakeplayer)
					continue;

				list.push_back({
					player->m_iTeamNum() == team::team_ct,
					info.szName,
					player_resource->GetWins()[player->GetIndex()],
					player_resource->GetRank()[player->GetIndex()],
					player->m_iAccount(),
					player->GetPlayerInfo().userId
					});
			}

			render_players(list, false);
			render_players(list, true);
		}
	}
}