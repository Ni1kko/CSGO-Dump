#include "Esp.h"
#include  "../../GUI/Gui.h"

#define SAME_LINE_1 115
#define SAME_LINE_2 230

#define PUSH_1 200
#define PUSH_2 400

void CEsp::Menu()
{
	auto LMenu = [&]()->void
	{
		static int VisType = 0;

		VectorEx<const char*>VisArrEng = { lolc("Player"), lolc("World") };
		VectorEx<const char*>VisArrRus = { lolc(u8"Игрок"),lolc(u8"Мир") };


		TabsLabels(VisType, VisArrEng, VisArrRus, Vec2(320, 19), false);

		X1Gui().SameLine(325);

		if(X1Gui().Button(ShowPreview ? (MainSettings().Russian ? u8"Предпросмотр >>>" : "Preview >>>") :
			(MainSettings().Russian ? u8"Предпросмотр <<<" : "Preview <<<"), Vec2(136, 19)))
			ShowPreview = !ShowPreview;

		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();

		if (VisType == 1)
		{
			X1Gui().PushItemWidth(PUSH_1);

			DCheckBox("Grenade", u8"Гранаты", WorldGrenade, u8"Показывает гранаты");
			if (WorldGrenade)
			{
				X1Gui().SameLine(SAME_LINE_1);
				X1Gui().PushItemWidth(PUSH_1);
				VectorEx<const char*>itemsGSEng = { lolc("Text") , lolc("Circle"), lolc("Icon") };
				VectorEx<const char*>itemsGSRus = { lolc(u8"Текст") , lolc(u8"Круг"), lolc(u8"Иконка") };
				DComboBox("Style##grenade", u8"Стиль##гранд", WorldGrenadeStyle, itemsGSEng, itemsGSRus, "");

				if (WorldGrenadeStyle != 1)
				{
					X1Gui().PushItemWidth(PUSH_2);
					SliderInts("Text Size##gre", u8"Текст##гр", TextGrenadeSize, 9, MAX_FONT_SIZE, u8"");
				}
				X1Gui().Spacing();

				DColorEdit("HE Grenade", u8"Осколочная граната", HEGrenadeColor);
				X1Gui().SameLine(SAME_LINE_2);
				DColorEdit("Molotov Grenade", u8"Коктейль Молотова", MolotovGrenadeColor);

				DColorEdit("Flash Grenade", u8"Световая граната", FlashGrenadeColor);
				X1Gui().SameLine(SAME_LINE_2);
				DColorEdit("Inc Grenade", u8"Зажигательная граната", IncGrenadeColor);

				DColorEdit("Smoke Grenade", u8"Дымовая граната", SmokeGrenadeColor);
				X1Gui().SameLine(SAME_LINE_2);
				DColorEdit("Decoy Grenade", u8"Ложная граната", DecoyGrenadeColor);


			}
			X1Gui().Spacing();
			X1Gui().Separator();
			X1Gui().Spacing();
			DCheckBox("Weapon", u8"Оружие", WorldWeapons, u8"Показывает оружие на земле");
			if (WorldWeapons)
			{
				X1Gui().SameLine(SAME_LINE_1);
				X1Gui().PushItemWidth(PUSH_1);
				VectorEx<const char*>itemsWSEng = { lolc("Text"), lolc("Icon") };
				VectorEx<const char*>itemsWSRus = { lolc(u8"Текст"), lolc(u8"Иконка") };
				DComboBox("Style##weapon", u8"Стиль##оруж", WorldWeaponsStyle, itemsWSEng, itemsWSRus, "");

				X1Gui().PushItemWidth(PUSH_2);
				SliderInts("Text Size##weapon", u8"Текст##оруж", TextWorldWeaponSize, 9, MAX_FONT_SIZE, u8"");

				X1Gui().Spacing();
				DColorEdit("Color##weapon", u8"Цвет##оруж", WorldWeaponsColor);

			}
			X1Gui().Spacing();
			X1Gui().Separator();
			X1Gui().Spacing();
			DCheckBox("Bomb", u8"Бомба", WorldBomb, u8"Показывает бомбу");
			if (WorldBomb)
			{
				X1Gui().SameLine(SAME_LINE_1);
				X1Gui().PushItemWidth(PUSH_1);
				VectorEx<const char*>itemsBSEng = { lolc("Text"), lolc("Icon") };
				VectorEx<const char*>itemsBSRus = { lolc(u8"Текст"), lolc(u8"Иконка") };
				DComboBox("Style##bomb", u8"Стиль##бомб", WorldBombStyle, itemsBSEng, itemsBSRus, "");

				X1Gui().PushItemWidth(PUSH_2);
				SliderInts("Text Size##bomb", u8"Размер текста##бомб", TextBombSize, 9, MAX_FONT_SIZE, u8"");

				X1Gui().Spacing();
				DColorEdit("Color##bomb", u8"Цвет##бомб", WorldBombColor);
			}
			DCheckBox("Bomb Info", u8"Информацию о бомбе", WorldBombInfo, u8"Показывает информацию о бомбу");
			X1Gui().Spacing();
			X1Gui().Separator();
			X1Gui().Spacing();
			DCheckBox("Granade Prediction", u8"Траектория гранаты", GranadePrediction, u8"Показывает траекторию полета гранаты");
			if (GranadePrediction)
			{
				X1Gui().Spacing();
				DColorEdit("Color##trajgr", u8"Цвет##тг", GranadePredictionColor);
			}
		}
		else if (VisType == 0)
		{

			DCheckBox("Enemy", u8"Враги", Enemy, u8"Работать на врагов");
			X1Gui().SameLine(SAME_LINE_1);
			DCheckBox("Team", u8"Команда", Team, u8"Работать на свою команду");
			X1Gui().SameLine(SAME_LINE_2);
			DCheckBox("Only Visible", u8"Только видимые", BoxVisbleOnly, u8"Работать только на видимых");
			X1Gui().PushItemWidth(100);
			DCheckBox("Bind", u8"Бинд", BindEnable.Enable, u8"Бинд включения визуалов");
			if (BindEnable.Enable)
			{
				X1Gui().SameLine(SAME_LINE_1);
				DCheckBox("Hold", u8"Удержание", BindEnable.Hold, u8"Включает работу визуалов только если кнопка зажата");
				X1Gui().SameLine(SAME_LINE_2);
				HotsKey("Button", u8"Кнопка", BindEnable.Button, u8"Задает кнопку активации визуалов");
			}

			X1Gui().Spacing();
			X1Gui().Separator();
			X1Gui().Spacing();

			X1Gui().PushItemWidth(PUSH_1);

			DCheckBox("Box", u8"Бокс", BoxEnable, u8"Включает бокс на игроке");
			if (BoxEnable)
			{
				X1Gui().SameLine(SAME_LINE_1);
				X1Gui().PushItemWidth(PUSH_1);
				VectorEx<const char*>itemsBSEng = { lolc("Full") , lolc("Angular"), lolc("3D"), lolc("Circled") };
				VectorEx<const char*>itemsBSRus = { lolc(u8"Полные") , lolc(u8"Угловые"), lolc(u8"3D"), lolc(u8"Закругленные") };
				DComboBox("Style##box", u8"Стиль##бокс", BoxStyle, itemsBSEng, itemsBSRus, "");
				if (BoxStyle != 2)
				{
					X1Gui().Spacing();
					X1Gui().PushItemWidth(PUSH_2);
					SliderInts("Size##box", u8"Размер##бокс", BoxSize, 1, 20, u8"Размер бокса");
				}
				X1Gui().PushItemWidth(PUSH_1);
				X1Gui().Spacing();
				DCheckBox("Outline##box", u8"Обводка##бокс", BoxOutline, u8"Включает обводку на боксе");
				X1Gui().Spacing();

				DColorEdit("Color CT", u8"Цвет КТ", BoxCT);
				X1Gui().SameLine(100);
				DColorEdit("Color T", u8"Цвет Т", BoxTT);

				X1Gui().SameLine(200);

				DColorEdit("Visible Color CT", u8"Видимый цвет КТ", BoxVisbleCT);
				X1Gui().SameLine(340);
				DColorEdit("Visible Color T", u8"Видимый цвет Т", BoxVisbleTT);
			}

			X1Gui().Spacing();
			X1Gui().Separator();
			X1Gui().Spacing();

			DCheckBox("Fill Box", u8"Заливка бокса", FillBox, u8"Включает заливку бокса");
			if (FillBox)
			{
				X1Gui().SameLine(SAME_LINE_2);
				DColorEdit("Color##fillbox", u8"Цвет##залбокс", FillBoxColor);
			}

			X1Gui().Spacing();
			X1Gui().Separator();
			X1Gui().Spacing();

			DCheckBox("Chams", u8"Чамсы", Chams, u8"Включает чамсы на игроке");
			if (Chams)
			{
				X1Gui().SameLine(SAME_LINE_1);
				X1Gui().PushItemWidth(PUSH_1);
				VectorEx<const char*>itemsCSEng = { lolc("Textured") , lolc("Flat"),lolc("Wireframe"), lolc("Chipolino"), lolc("Metallic") };
				VectorEx<const char*>itemsCSRus = { lolc(u8"Текстурные") ,lolc(u8"Плоские"), lolc(u8"Сетчатые"), lolc(u8"Чиполино"), lolc(u8"Металлические") };
				DComboBox("Style##chams", u8"Стиль##чамс", ChamsStyle, itemsCSEng, itemsCSRus, u8"Металлические чамсы работают только на самых высоких настройках");
				X1Gui().SameLine(SAME_LINE_1 + PUSH_1 + 40);
				DCheckBox("Only Visible##chams", u8"Видимые##чамс", ChamsVisbleOnly, u8"Только видимые чамсы");

				if (ChamsStyle == 3)
				{
					X1Gui().Spacing();
					X1Gui().PushItemWidth(PUSH_2);
					SliderFloats("Brightness", u8"Яркость", ChipolinoFixColor, 0.0000000001, 1, u8"");
				}
				X1Gui().Spacing();
				DColorEdit("Color CT##chams", u8"Цвет КТ##чамс", ChamsCT);
				X1Gui().SameLine(100);
				DColorEdit("Color T##chams", u8"Цвет Т##чамс", ChamsTT);

				X1Gui().SameLine(200);

				DColorEdit("Visible Color CT##chams", u8"Видимый цвет КТ##чамс", ChamsVisbleCT);
				X1Gui().SameLine(340);
				DColorEdit("Visible Color T##chams", u8"Видимый цвет Т##чамс", ChamsVisbleTT);
			}

			X1Gui().Spacing();
			X1Gui().Separator();
			X1Gui().Spacing();

			DCheckBox("Health", u8"Здоровье", Health, u8"Показывает здоровье игрока");
			X1Gui().SameLine(SAME_LINE_1);
			DCheckBox("Armor", u8"Броня", Armor, u8"Показывает броню игрока");
			X1Gui().Spacing();
			X1Gui().Separator();
			X1Gui().Spacing();
			DCheckBox("Name", u8"Имя", Name, u8"Показывает имя игрока");
			DCheckBox("Rank", u8"Ранг", Rank, u8"Показывает ранг игрока в табе");
			DCheckBox("Distance", u8"Дистанция", Distance, u8"Показывает дистанцию до игрока");
			DCheckBox("Weapon", u8"Оружие", Weapon, u8"Показывает оружие игрока");
			X1Gui().SameLine(SAME_LINE_1);
			DCheckBox("Ammo", u8"Патроны", Ammo, u8"Показывает патроны игрока");
			DCheckBox("Money", u8"Деньги", Money, u8"Показывает деньги игрока");
			X1Gui().Spacing();
			X1Gui().Separator();
			X1Gui().Spacing();
			DCheckBox("Flashing", u8"Ослепление", Flashing, u8"Работает если игрок ослеплен");
			X1Gui().SameLine();
			DCheckBox("Defusing", u8"Дифюзинг", Defusing, u8"Работает если игрок дифюзит");
			X1Gui().SameLine();
			DCheckBox("Scope", u8"Прицеливание", Scope, u8"Работает если игрок прицелился");
			X1Gui().Spacing();
			X1Gui().Separator();
			X1Gui().Spacing();

			DCheckBox("Head Esp", u8"Голова", HeadEsp, u8"Включает круг вокруг головы");
			if (HeadEsp)
			{
				X1Gui().SameLine(SAME_LINE_1);
				DColorEdit("Color##head", u8"Цвет##гол", HeadEspColor);
			}

			DCheckBox("Line", u8"Линии", Line, u8"Включает линии до игроков");
			if (Line)
			{
				X1Gui().SameLine(SAME_LINE_1);
				DColorEdit("Color##line", u8"Цвет##лин", LineColor);
			}

			DCheckBox("Skeleton", u8"Скелеты", Skeleton, u8"Включает скелеты игроков");
			if (Skeleton)
			{
				X1Gui().SameLine(SAME_LINE_1);
				DColorEdit("Color##skel", u8"Цвет##скел", SkeletonColor);
			}
			X1Gui().Spacing();
			X1Gui().Separator();
			X1Gui().Spacing();
			DCheckBox("Sound Esp", u8"Звуковое есп", SoundEspEnable, u8"Включает отображение шагов");
			if (SoundEspEnable)
			{
				X1Gui().SameLine(SAME_LINE_1);
				VectorEx<const char*>itemsSEEng = { lolc("Circle") , lolc("Dynamic Circle(+)"), lolc("Dynamic Circle(-)"), lolc("Box"),lolc("Filled Box"), lolc("Star") };
				VectorEx<const char*>itemsSERus = { lolc(u8"Круг") , lolc(u8"Динамический круг(+)"), lolc(u8"Динамический круг(-)"),lolc(u8"бокс"), lolc(u8"Залитый бокс"),lolc(u8"Звезда") };
				DComboBox("Style##sound", u8"Стиль##звук", SoundEspStyle, itemsSEEng, itemsSERus, "");

				X1Gui().Spacing();
				X1Gui().PushItemWidth(PUSH_2);
				SliderInts("Size##sound", u8"Размер##звук", SoundEspSize, 1, 100, u8"Размер звуков");
				X1Gui().PushItemWidth(PUSH_1);
				X1Gui().Spacing();
				DCheckBox("Distance##sound", u8"Дистанция##звук", SoundEspDistance, u8"Включает отображение дистанции до звука");
				if (SoundEspDistance)
				{
					X1Gui().SameLine(SAME_LINE_1);
					DColorEdit("Color##sounddistance", u8"Цвет##звукдиск", SoundDistanceColor);

					X1Gui().PushItemWidth(PUSH_2);
					SliderInts("Text Size##sound", u8"Текст##звукдиск", TextSoundSize, 9, MAX_FONT_SIZE, u8"");

				}
				DColorEdit("Color##sound", u8"Цвет##звук", SoundColor);
			}
		}
	};
	LMenu();
}

#define iText(eng,rus) /*ImGui::Text(!MainCheatConfig.RusLanguage ? XorStr(eng) : XorStr(rus));*/

void CEsp::VisualPreview()
{
	//ImGuiStyle& style = ImGui::GetStyle();

	static MoveStruct *MV_Hp = &MVItemList[MI_HEALTH_BAR];
	static MoveStruct *MV_Armor = &MVItemList[MI_ARMOR_BAR];
	static MoveStruct *MV_Hp_Text = &MVItemList[MI_HEALTH_TEXT];
	static MoveStruct *MV_Armor_Text = &MVItemList[MI_ARMOR_TEXT];
	static MoveStruct *MV_Name = &MVItemList[MI_NAME_TEXT];
	static MoveStruct *MV_Weapon = &MVItemList[MI_WEAPON_TEXT];
	static MoveStruct *MV_Ammo = &MVItemList[MI_AMMO_TEXT];
	static MoveStruct *MV_Distance = &MVItemList[MI_DISTANCE_TEXT];
	static MoveStruct *MV_Money = &MVItemList[MI_MONEY_TEXT];
	static MoveStruct *MV_Scope = &MVItemList[MI_SCOPE_TEXT];
	static MoveStruct *MV_Flashed = &MVItemList[MI_FLASHED_TEXT];
	static MoveStruct *MV_Defusing = &MVItemList[MI_DEFUSING_TEXT];

	Vec2 ZoneStartPos = X1Gui().GetCursorPosEx();

	CustomVisuals(ZoneStartPos);

	if ((MITypes)MoveStruct::LastActive == MI_AMMO_TEXT)
	{
		iText("Selected: Ammo", u8"Выбрано: Патроны");

		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();
		VectorEx<const char*>itemsGSEng = { lolc("Type1") ,lolc("Type2"), lolc("Type3") };
		VectorEx<const char*>itemsGSRus = { lolc(u8"Тип1") , lolc(u8"Тип2"),lolc(u8"Тип3") };
		DComboBox("Style", u8"Стиль", AmmoStyle, itemsGSEng, itemsGSRus, "");
		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();
		SliderInts("Text Size", u8"Размер текста", TextAmmoSize, 9, MAX_FONT_SIZE, u8"");
		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();
		DColorEdit("Color", u8"Цвет", AmmoColor);
	}
	else if ((MITypes)MoveStruct::LastActive == MI_ARMOR_TEXT)
	{
		iText("Selected: Armor", u8"Выбрано: Броня");

		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();

		int OldStyle = ArmorStyle;

		VectorEx<const char*>NumStyle = { lolc("Line") , lolc("Text") };
		VectorEx<const char*>NumStyleRus = { lolc(u8"Линия") , lolc(u8"Текст") };

		DComboBox("Style", "Стиль", ArmorStyle, NumStyle, NumStyleRus, "");
		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();
		SliderInts("Text Size", u8"Размер текста", TextArmorSize, 9, MAX_FONT_SIZE, u8"");

		if (OldStyle != ArmorStyle)
			MoveStruct::LastActive = MI_ARMOR_BAR;

		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();
		DColorEdit("Color", u8"Цвет", ArmorColor);
	}
	else if ((MITypes)MoveStruct::LastActive == MI_ARMOR_BAR)
	{
		iText("Selected: Armor", u8"Выбрано: Броня");

		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();

		int OldStyle = ArmorStyle;

		VectorEx<const char*>NumStyle = { lolc("Line") , lolc("Text") };
		VectorEx<const char*>NumStyleRus = { lolc(u8"Линия") , lolc(u8"Текст") };

		DComboBox("Style", u8"Стиль", ArmorStyle, NumStyle, NumStyleRus, "");


		if (OldStyle != ArmorStyle)
			MoveStruct::LastActive = MI_ARMOR_TEXT;

		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();

		DColorEdit("Main Color", u8"Глваный цвет", ArmorColor);
		DColorEdit("Background Color", u8"Цвет фона", ArmorColor2);
		DColorEdit("Outline Color", u8"Цвет обводки", ArmorColor3);


		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();

		SliderInts("Separators", u8"Разделители", MV_Armor->NumParam1, 0, 10, u8"Количество разделителей");
		SliderInts("Tickness", u8"Толщина", MV_Armor->NumParam2, 0, 20, u8"Толщина полоски");

		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();
		DCheckBox("Background", u8"Фон", MV_Armor->boolParam1, u8"");
		X1Gui().SameLine();
		DCheckBox("Outline", u8"Обводка", MV_Armor->boolParam2, u8"");
		X1Gui().SameLine();
		DCheckBox("Inverse", u8"Инверсия", MV_Armor->boolParam3, u8"");
	}
	else if ((MITypes)MoveStruct::LastActive == MI_DISTANCE_TEXT)
	{
		iText("Selected: Selected", u8"Выбрано: Дистанция");
		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();

		SliderInts("Text Size", u8"Размер текста", TextDistanceSize, 9, MAX_FONT_SIZE, u8"");
		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();
		DColorEdit("Color", u8"Цвет", DistanceColor);
	}
	else if ((MITypes)MoveStruct::LastActive == MI_HEALTH_TEXT)
	{
		iText("Selected: Health", u8"Выбрано: Здоровье");

		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();

		VectorEx<const char*>NumStyle = { lolc("Line") , lolc("Text") };
		VectorEx<const char*>NumStyleRus = { lolc(u8"Линия") , lolc(u8"Текст") };


		int OldStyle = HealthStyle;

		DComboBox("Style", u8"Стиль", HealthStyle, NumStyle, NumStyleRus, "");
		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();
		SliderInts("Text Size", u8"Размер текста", TextHealthSize, 9, MAX_FONT_SIZE, u8"");

		if (OldStyle != HealthStyle)
			MoveStruct::LastActive = MI_HEALTH_BAR;

		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();
		DColorEdit("Color", u8"Цвет", HealthColor);
	}
	else if ((MITypes)MoveStruct::LastActive == MI_HEALTH_BAR)
	{
		iText("Selected: Health", u8"Выбрано: Здоровье");

		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();

		VectorEx<const char*>NumStyle = { lolc("Line") , lolc("Text") };
		VectorEx<const char*>NumStyleRus = { lolc(u8"Линия") , lolc(u8"Текст") };

		int OldStyle = HealthStyle;

		DComboBox("Style", u8"Стиль", HealthStyle, NumStyle, NumStyleRus, "");

		if (OldStyle != HealthStyle)
			MoveStruct::LastActive = MI_HEALTH_TEXT;

		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();

		DColorEdit("Main Color", u8"Глваный цвет", HealthColor);
		DColorEdit("Background Color", u8"Цвет фона", HealthColor2);
		DColorEdit("Outline Color", u8"Цвет обводки", HealthColor3);
		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();

		SliderInts("Separators", u8"Разделители", MV_Hp->NumParam1, 0, 10, u8"Количество разделителей");
		SliderInts("Tickness", u8"Толщина", MV_Hp->NumParam2, 0, 20, u8"Толщина полоски");

		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();
		DCheckBox("Background", u8"Фон", MV_Hp->boolParam1, u8"");
		X1Gui().SameLine();
		DCheckBox("Outline", u8"Обводка", MV_Hp->boolParam2, u8"");
		X1Gui().SameLine();
		DCheckBox("Inverse", u8"Инверсия", MV_Hp->boolParam3, u8"");
	}
	else if ((MITypes)MoveStruct::LastActive == MI_NAME_TEXT)
	{

		iText("Selected: Name", u8"Выбрано: Имя");

		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();

		SliderInts("Text Size", u8"Размер текста", TextNameSize, 9, MAX_FONT_SIZE, u8"");
		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();
		DColorEdit("Color", u8"Цвет", NameColor);


		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();

		DCheckBox("Background", u8"Фон", NameBackground, u8"Фон для имени");


		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();

		if (NameBackground)
		{
			X1Gui().Spacing();
			DColorEdit("Background Color", u8"Цвет", NameColorBack);
		}
	}
	else if ((MITypes)MoveStruct::LastActive == MI_WEAPON_TEXT)
	{
		iText("Selected: Weapon", u8"Выбрано: Оружие");
		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();
		VectorEx<const char*>itemsGSEng = { lolc("Text") ,lolc("Icon") };
		VectorEx<const char*>itemsGSRus = { lolc(u8"Текст") ,lolc(u8"Иконка") };
		DComboBox("Style", u8"Стиль", WeaponStyle, itemsGSEng, itemsGSRus, "");
		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();
		SliderInts("Text Size", u8"Размер текста", TextWeaponSize, 9, MAX_FONT_SIZE, u8"");
		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();
		DColorEdit("Color", u8"Цвет", WeaponColor);
	}
	else if ((MITypes)MoveStruct::LastActive == MI_MONEY_TEXT)
	{
		iText("Selected: Money", u8"Выбрано: Деньги");
		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();

		SliderInts("Text Size", "Размер текста", TextMoneySize, 9, MAX_FONT_SIZE, u8"");
		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();
		DColorEdit("Color", u8"Цвет", MoneyColor);
	}
	else if ((MITypes)MoveStruct::LastActive == MI_SCOPE_TEXT)
	{
		iText("Selected: Scoped", u8"Выбрано: Прицеливание");
		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();

		SliderInts("Text Size", u8"Размер текста", TextScopedSize, 9, MAX_FONT_SIZE, u8"");
		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();
		DColorEdit("Color", u8"Цвет", ScopeColor);
	}
	else if ((MITypes)MoveStruct::LastActive == MI_FLASHED_TEXT)
	{
		iText("Selected: Flashed", u8"Выбрано: Ослепление");
		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();

		SliderInts("Text Size", u8"Размер текста", TextFlashedSize, 9, MAX_FONT_SIZE, u8"");
		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();
		DColorEdit("Color", u8"Цвет", FlashingColor);

	}
	else if ((MITypes)MoveStruct::LastActive == MI_DEFUSING_TEXT)
	{
		iText("Selected: Defusing", u8"Выбрано: Дифюзинг");
		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();

		SliderInts("Text Size", u8"Размер текста", TextDefusingSize, 9, MAX_FONT_SIZE, u8"");
		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();
		DColorEdit("Color", u8"Цвет", DefusingColor);
	}

	float long_item_w = X1Gui().GetThis()->Info.Size.x - (X1Gui().GetStyle().wndPadding.x * 2);
	X1Gui().SetCursorPos(Vec2(X1Gui().GetStyle().wndPadding.x, 638), 0);
	if (X1Gui().Button(XorStr("Reset"), Vec2(long_item_w, 22)))
		MoveStruct::InitMItems = false;
}