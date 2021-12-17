#include "GreandeHelper.h"
#include "../../GUI/Gui.h"

void CGHelper::ChangeGHI(GHInfo* info, char NameBuf[], char DescpBuf[], char MapName[], bool AdvMap)
{
	TextEdit("Name", u8"Имя", info->name, NameBuf, 64);
	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();

	if (AdvMap)
	{
		static int SelecteMapMode = 0;
		/*const char** itemsGHSS = new const char*[2 + maps.size()];

		itemsGHSS[0] = new const char[sizeof((!MainSettings().Russian ? "Auto" : u8"Авто"))];
		itemsGHSS[0] = (!MainSettings().Russian ? "Auto" : u8"Авто");

		itemsGHSS[1] = new const char[sizeof((!MainSettings().Russian ? "Custom" : u8"Кастом"))];
		itemsGHSS[1] = (!MainSettings().Russian ? "Custom" : u8"Кастом");


		for (size_t i(2); i < maps.size() + 2; i++)
		{
			itemsGHSS[i] = new const char[sizeof(maps[i - 2].game_name.c_str())];
			itemsGHSS[i] = maps[i - 2].game_name.c_str();
		}*/

		VectorEx<const char*> itemsGHSS;
		itemsGHSS.push_back(!MainSettings().Russian ? "Auto" : u8"Авто");
		itemsGHSS.push_back(!MainSettings().Russian ? "Custom" : u8"Кастом");

		for (size_t i(2); i < maps.size() + 2; i++)
		{
			itemsGHSS.push_back(maps[i - 2].game_name.c_str());
		}

		X1Gui().ComboBox(!MainSettings().Russian ? XorStr("Map") : XorStr(u8"Карта"), &SelecteMapMode, itemsGHSS);

		if (SelecteMapMode == 1)
		{
			X1Gui().InputText(!MainSettings().Russian ? XorStr("Map Name") : XorStr(u8"Имя карты"), MapName, 300);
		}
		else if (SelecteMapMode == 0)
		{
			strcpy(MapName, string(I::Engine()->GetLevelName()).c_str());
		}
		else
		{
			strcpy(MapName, itemsGHSS[SelecteMapMode]);
		}
		

		//delete[] itemsGHSS;
	}
	else
	{
		X1Gui().InputText(!MainSettings().Russian ? XorStr("Map Name") : XorStr(u8"Имя карты"), MapName, 300);
	}
	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();

#define chu(e)  e

	if (!AdvMap)
	{

		VectorEx<const char*> itemsCSEng = { chu("Incendiary") , chu("Molotov"),chu("HE Granade"), chu("Smoke"), chu("Decoy"), chu("Flashbang") };
		VectorEx<const char*> itemsCSRus = { chu(u8"Зажигательная"), chu(u8"Молотов"), chu(u8"Осколочная"), chu(u8"Дым"), chu(u8"Ложная"), chu(u8"Световая") };

		int bg = 0;

		switch (info->grenade)
		{
		case WEAPON_INCGRENADE: bg = 0; break;
		case WEAPON_MOLOTOV: bg = 1; break;
		case WEAPON_HEGRENADE: bg = 2; break;
		case WEAPON_SMOKEGRENADE: bg = 3; break;
		case WEAPON_DECOY: bg = 4; break;
		case WEAPON_FLASHBANG: bg = 5; break;
		default: break;
		}

		DComboBox("Grenade", u8"Граната", bg, itemsCSEng, itemsCSRus, u8"");

		switch (bg)
		{
		case 0: info->grenade = WEAPON_INCGRENADE; break;
		case 1: info->grenade = WEAPON_MOLOTOV; break;
		case 2: info->grenade = WEAPON_HEGRENADE; break;
		case 3: info->grenade = WEAPON_SMOKEGRENADE; break;
		case 4: info->grenade = WEAPON_DECOY; break;
		case 5: info->grenade = WEAPON_FLASHBANG; break;
		default: break;
		}
	}
	else
	{

		VectorEx<const char*> itemsCSEng_ = { chu("Auto"), chu("Incendiary") , chu("Molotov"),chu("HE Granade"), chu("Smoke"), chu("Decoy"), chu("Flashbang") };
		VectorEx<const char*> itemsCSRus_ = { chu(u8"Авто"), chu(u8"Зажигательная"), chu(u8"Молотов"), chu(u8"Осколочная"), chu(u8"Дым"), chu(u8"Ложная"), chu(u8"Световая") };

		int bg = 0;

		switch (info->grenade)
		{
		case WEAPON_NONE: bg = 0; break;
		case WEAPON_INCGRENADE: bg = 1; break;
		case WEAPON_MOLOTOV: bg = 2; break;
		case WEAPON_HEGRENADE: bg = 3; break;
		case WEAPON_SMOKEGRENADE: bg = 4; break;
		case WEAPON_DECOY: bg = 5; break;
		case WEAPON_FLASHBANG: bg = 6; break;
		default: break;
		}

		DComboBox("Grenade", u8"Граната", bg, itemsCSEng_, itemsCSRus_, u8"");

		switch (bg)
		{
		case 0: info->grenade = WEAPON_NONE; break;
		case 1: info->grenade = WEAPON_INCGRENADE; break;
		case 2: info->grenade = WEAPON_MOLOTOV; break;
		case 3: info->grenade = WEAPON_HEGRENADE; break;
		case 4: info->grenade = WEAPON_SMOKEGRENADE; break;
		case 5: info->grenade = WEAPON_DECOY; break;
		case 6: info->grenade = WEAPON_FLASHBANG; break;
		default: break;
		}
	}
	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();
	SliderFloats("Direction X", u8"Направление X", info->directn.y, -179.9, 179.9, u8"");
	SliderFloats("Direction Y", u8"Направление Y", info->directn.x, -179.9, 179.9, u8"");

	//X1Gui().Spacing();
	//X1Gui().Separator();
	//X1Gui().Spacing();
	//X1Gui().Text(!MainSettings().Russian ? XorStr("Description:") : XorStr(u8"Описание:"));

/*	strcpy(DescpBuf, info->descn.c_str());
	X1Gui().InputTextMultiline("##Description0", DescpBuf, 300, Vec2(453, 140));
	info->descn = DescpBuf;*/

	//X1Gui().Spacing();
	//X1Gui().Separator();
	//X1Gui().Spacing();

	//DCheckBox("Show Direction", u8"Показывать направление", info->show_direction, u8"");	
};

void CGHelper::Menu()
{
	float long_item_w = X1Gui().GetThis()->Info.Size.x - (X1Gui().GetStyle().wndPadding.x * 2);
	//int all = 0;

	//for (int i(0); i < maps.size(); i++)
	//{
	//	all += maps[i].helpers.size();
	//}

//	GP_IRender->render_String(17, Vec2(100, 100), Color::Red().GetImColor(), DT_LEFT, to_string(all).c_str());

	DCheckBox("Enable", u8"Активировать", Enable, u8"");

	X1Gui().SameLine();

	DCheckBox("Only for hand grenades", u8"Только для гранаты в руках", EnableFor1Grenade, u8"");

	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();
	DCheckBox("Show Description", u8"Показывать подсказку", ShowDiscp, u8"");
	X1Gui().SameLine();
	DCheckBox("Show Name", u8"Показывать название", ShowName, u8"");
	X1Gui().Spacing();
	SliderFloats("Distance Show Info", u8"Дистанция отображения информации", DistShowInfo, 1, 400, u8"");
	SliderFloats("Distance Show", u8"Дистанция отображения", DistShowHelp, 1, 2000, u8"");

	VectorEx<const char*> itemsCSEng = { chu("Type 1"), chu("Type 2")};
	VectorEx<const char*> itemsCSRus = { chu(u8"Тип 1"), chu(u8"Тип 2")};

	DComboBox("Crosshair", u8"Прицел", CrosshairStyle, itemsCSEng, itemsCSRus, u8"");

	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();

	static int Mode = 1;
	static GHInfo InfoProt;

	if (X1Gui().ListBoxHeader("##GHelpList", Vec2(long_item_w, 268)))
	{
		GuiStyle& style = X1Gui().GetStyle();

		Vec2 cur_menu_pos = X1Gui().GetCursorPos();
		Vec2 prev_pos_1 = Vec2(0, 0);
		Vec2 prev_pos_2 = Vec2(0, 0);

		auto CreateNode = [&](float cor_x, float len, bool check, Vec2 &prev) -> void
		{
			cur_menu_pos = X1Gui().GetCursorPosEx();
			Vec2 cur_pos = X1Gui().GetCursorPos();

			Vec2 next_pos = Vec2(cur_pos.x + len + cor_x, cur_pos.y);

			X1Gui().DrawLine(Vec2(cur_pos.x + cor_x, cur_pos.y + 9), Vec2(next_pos.x - 3, next_pos.y + 9), MainSettings().LineColor);
			X1Gui().DrawLine(Vec2(cur_pos.x + cor_x, (prev.y != 0) ? prev.y : (cur_pos.y - (check ? 9 : 3))), Vec2(cur_pos.x + cor_x, next_pos.y + 9), MainSettings().LineColor);

			prev = Vec2(cur_pos.x + cor_x, next_pos.y + 9);

			X1Gui().SetCursorPos(Vec2(cur_menu_pos.x + len + cor_x, cur_menu_pos.y), 0);
		};

		string CurrentMapName = I::Engine()->GetLevelName();

		for (int i = 0; i < maps.size(); i++)
		{
			CreateNode(3, 15, i != 0, prev_pos_1);

			bool selected_map = SelectedMap && SelectedMap == &maps[i];

			if (CurrentMapName == maps[i].game_name)
				style.clrText = Color::Green();

			bool tree_open = X1Gui().BeginTreeNode((maps[i].game_name + "##" + to_string(i)).c_str(), Vec2(long_item_w - 30, 0), false);
			if (X1Gui().IsItemClicked())
			{
				SelectedMap = &maps[i];
				SelectedGHInf = nullptr;
				Mode = 0;
			}

			if (CurrentMapName == maps[i].game_name)
				style.clrText = MainSettings().TextColor;

			if (tree_open)
			{
				for (int j = 0; j < maps[i].helpers.size(); j++)
				{
					X1Gui().Spacing();
					CreateNode(24, 20, j != 0, prev_pos_2);
					bool selected = SelectedGHInf && SelectedGHInf == &maps[i].helpers[j];

					if (X1Gui().SelectLabel((maps[i].helpers[j].name + " (" + GetGName(maps[i].helpers[j].grenade) + ")##" + to_string((i + 1) + (j + 1))).c_str(),
						selected, Vec2(selected ? 305 : 395, 0)))
					{
						SelectedGHInf = &maps[i].helpers[j];
						SelectedMap = nullptr;
						Mode = 0;
					}

					if (selected)
					{
						X1Gui().SameLine();

						style.clrFrame = Color(MainSettings().FrameColor.r() + 18.f,
							MainSettings().FrameColor.g() + 18.f,
							MainSettings().FrameColor.b() + 18.f, 255);
						float old_pad_y = style.itmPadding.y;
						style.itmPadding.y = 0;

						DCheckBox("Enable", u8"Актив.", maps[i].helpers[j].enable, u8"Включить отображение");

						style.clrFrame = MainSettings().FrameColor;
						style.itmPadding.y = old_pad_y;
					}
				}
				prev_pos_2 = Vec2(0, 0);
			}

			X1Gui().Spacing();
		}
	}
	X1Gui().ListBoxFooter();

	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();

	VectorEx<const char*> itemsCSSEng = { lolc("Editing"), lolc("Adding") };
	VectorEx<const char*> itemsCSSRus = { lolc(u8"Редактирование"), lolc(u8"Добавление") };

	TabsLabels(Mode, itemsCSSEng, itemsCSSRus, Vec2(long_item_w, 20), false);

	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();

	if (Mode == 0)
	{
		auto RemvMapHelp = [&](Map* map, GHInfo* info) -> void
		{
			if (!map || !info)
				return;

			if (map->helpers.size() == 1)
			{
				DeleteMap(map->game_name);
				map = nullptr;
				info = nullptr;
			}
			else
			{
				DeleteHelp(info);
				info = nullptr;
			}
		};

		if (SelectedGHInf)
		{
			char NameBuf[64] = { 0 };
			char DescpBuf[300] = { 0 };
			char MapNameBuf[300] = { 0 };

			Map* CurMap = GetMapByHelp(SelectedGHInf);
			if (CurMap)
			{
				string OldName = CurMap->game_name;

				strcpy(MapNameBuf, CurMap->game_name.c_str());
				ChangeGHI(SelectedGHInf, NameBuf, DescpBuf, MapNameBuf, false);

				if (OldName != string(MapNameBuf))
				{
					GHInfo tempInfo = *SelectedGHInf;// хрень
					RemvMapHelp(CurMap, SelectedGHInf);
					Add(string(MapNameBuf), tempInfo);

					Map* NewMap = GetMapByGName(string(MapNameBuf));
					if (NewMap)
						SelectedGHInf = &NewMap->helpers.back();
				}

				X1Gui().Spacing();
				X1Gui().Separator();
				X1Gui().Spacing();

				if (X1Gui().Button(!MainSettings().Russian ? XorStr("Remove") : XorStr(u8"Удалить"), Vec2(long_item_w, 22)))
					if (CurMap)
						RemvMapHelp(CurMap, SelectedGHInf);
			}
		}
		if (SelectedMap)
		{
			char MapNameBuf[300] = { 0 };

	/*		strcpy(MapNameBuf, SelectedMap->game_name.c_str());
			X1Gui().InputText(!MainSettings().Russian ? XorStr("Map") : XorStr(u8"Карта"), MapNameBuf, 300);
			SelectedMap->game_name = MapNameBuf;*/

			TextEdit("Map", u8"Карта", SelectedMap->game_name, MapNameBuf, 300);
			if(X1Gui().IsItemHovered())
				MainSettings().HintMsg = XorStr(u8"Очистите для автоматического выбора имени");

			if (SelectedMap->game_name.empty())
				SelectedMap->game_name = I::Engine()->GetLevelName();


			if (X1Gui().Button(!MainSettings().Russian ? XorStr("Remove") : XorStr(u8"Удалить"), Vec2(long_item_w, 22)))
			{
				while (SelectedMap)
				{
					if (SelectedMap->helpers.empty())
						SelectedMap = nullptr;
					else
						RemvMapHelp(SelectedMap, &SelectedMap->helpers[0]);
				}
			}
		}
	}
	else if (Mode == 1)
	{
		char NameBuf[64] = { 0 };
		char DescpBuf[300] = { 0 };
		char MapNameBuf[300] = { 0 };
		static string tempMapName = "";

		strcpy(MapNameBuf, tempMapName.c_str());
		ChangeGHI(&InfoProt, NameBuf, DescpBuf, MapNameBuf, true);
		tempMapName = MapNameBuf;

		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();

		if (X1Gui().Button(!MainSettings().Russian ? XorStr("Add") : XorStr(u8"Добавить"), Vec2(long_item_w, 22)))
		{
			AddHelp(tempMapName.empty() ? I::Engine()->GetLevelName() : tempMapName, InfoProt);

			InfoProt = GHInfo();
			SelectedGHInf = nullptr;
		}
	}
	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();
	if (X1Gui().Button(!MainSettings().Russian ? XorStr("Load Grenade Helper") : XorStr(u8"Загрузить раскидки"), Vec2(228, 22)))
	{
		if (LoadMaps());
		//	Menu::Message::Get().Start(!MainSettings().Russian ? XorStr("Settings loaded!") : XorStr(u8"Настройки загружены!"));
		//else
		//	Menu::Message::Get().Start(!MainSettings().Russian ? XorStr("Error load!") : XorStr(u8"Ошибка загрузки!"));
	}
	X1Gui().SameLine();
	if (X1Gui().Button(!MainSettings().Russian ? XorStr("Save Grenade Helper") : XorStr(u8"Сохранить раскидки"), Vec2(228, 22)))
	{
		if (SaveMaps());
		//	Menu::Message::Get().Start(!MainSettings().Russian ? XorStr("Settings saved!") : XorStr(u8"Настройки сохранены!"));
		//else
		//	Menu::Message::Get().Start(!MainSettings().Russian ? XorStr("Error save!") : XorStr(u8"Ошибка сохранения!"));	
	}
	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();
	if (X1Gui().Button(!MainSettings().Russian ? XorStr("Reset Settings Grenade Helper") : XorStr(u8"Сбросить настройки раскидок"), Vec2(long_item_w, 22)))
	{
		maps.clear();
		SelectedGHInf = nullptr;
		SelectedMap = nullptr;
	}
}