#include "Misc.h"
#include "../../GUI/Gui.h"

#define SAME_LINE_1 168

#define PUSH_1 200
#define PUSH_2 360
#define PUSH_3 240

void CMisc::Menu()
{
	DCheckBox("Bhop", u8"Бхоп", BHop, u8"Включает авто прыжок");
	if (BHop)
	{
		X1Gui().SameLine(SAME_LINE_1);
		DCheckBox("Auto Strafe", u8"Авто стрейф", AutoStrafe, u8"Включает авто стрейф");
	}
	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();
	DCheckBox("Auto Accept", u8"Авто принятие игры", AutoAccept, u8"Автоматическое принятие игры");

	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();

	DCheckBox("Spectator List", u8"Спектатор лист", SpectatorList, u8"Показывает смотрящих за вами");
	if (SpectatorList)
	{
		X1Gui().SameLine(SAME_LINE_1);
		X1Gui().PushItemWidth(PUSH_1);
		SliderInts("Alpha##spec", u8"Прозрачность##спек", SpectatorListAlpha, 0, 255, u8"");
		X1Gui().PushItemWidth(PUSH_2);
		SliderInts("Pos X##spec", u8"Позиция X##спек", SpectatorListPosX, 0, CGlobal::iScreenWidth, u8"");
		X1Gui().PushItemWidth(PUSH_2);
		SliderInts("Pos Y##spec", u8"Позиция Y##спек", SpectatorListPosY, 0, CGlobal::iScreenHeight, u8"");
		DCheckBox("Auto Hide", u8"Авто скрытие", SpectatorListAutoHide, u8"");
	}

	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();
	DCheckBox("Fov Changer", u8"Фов камеры", FovChanger, u8"Включает изменение угла обзора");
	if (FovChanger)
	{
		X1Gui().SameLine(SAME_LINE_1);
		X1Gui().PushItemWidth(PUSH_1);
		SliderInts("Fov View", u8"Угол обзора", FovView, 1, 180, u8"");
	}
	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();
	DCheckBox("Fov Model Changer", u8"Фов рук", FovModelChanger, u8"Включает изменение угла модели");
	if (FovModelChanger)
	{
		X1Gui().SameLine(SAME_LINE_1);
		X1Gui().PushItemWidth(PUSH_1);
		SliderInts("Fov Model", u8"Угол модели", FovModel, 1, 180, u8"");
	}
	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();
	DCheckBox("No Visual Recoil", u8"Без визуальной отдачи", NoVisualRecoil, u8"Отключает визуальную отдачу");
	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();
	DCheckBox("FreeCam", u8"Свободная камера", FreeCam, u8"Включает свободную камеру");
	if (FreeCam)
	{
		X1Gui().SameLine(SAME_LINE_1);
		X1Gui().PushItemWidth(PUSH_1);
		SliderInts("Speed##frecam", u8"Скорость##своб", FreeCamSpeed, 1, 40, u8"Скорость свободной камеры");

		HotsKey("Button##frcam", u8"Кнопка##своб", FreeCamBind.Button, u8"Задает кнопку активации");
		X1Gui().SameLine();
		DCheckBox("Hold##frcam", u8"Удержание##своб", FreeCamBind.Hold, u8"Включает свободную камеру");
	}

	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();

	DCheckBox("Crosshair", u8"Прицел", Crosshair, u8"Включает отображение кастомного прицела");
	if (Crosshair)
	{
		X1Gui().SameLine(SAME_LINE_1);
		X1Gui().PushItemWidth(PUSH_1);
		VectorEx<const char* >itemsCSSEng = { lolc("Type1") , lolc("Type2"), lolc("Type3") };
		VectorEx<const char* >itemsCSSRus = { lolc(u8"Тип1") , lolc(u8"Тип2"), lolc(u8"Тип3") };
		DComboBox("Style##Crossh", u8"Стиль##приц", CrosshairStyle, itemsCSSEng, itemsCSSRus, "");

		X1Gui().PushItemWidth(PUSH_2);
		SliderInts("Size##Crossh", u8"Размер##приц", CrosshairSize, 1, 100, u8"Изменяет размер прицела");

		DCheckBox("Recoil##Crossh", u8"Отдача##приц", CrosshairRecoil, u8"Включает отображение отдачи");
		X1Gui().SameLine(SAME_LINE_1);
		DCheckBox("Outline##Crossh", u8"Обводка##приц", CrosshairOutline, u8"Включает обводку прицела");

		DColorEdit("Color##Crossh", u8"Цвет##приц", CrosshairColor);
	}

	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();

	DCheckBox("Sniper Crosshair", u8"Снаперский прицел", SnipCrosshair, u8"Включает отображение кастомного прицела(только на снайперках)");
	if (SnipCrosshair)
	{
		if (MainSettings().Russian)
			X1Gui().SameLine(SAME_LINE_1 + 10);
		else
			X1Gui().SameLine(SAME_LINE_1);
		X1Gui().PushItemWidth(PUSH_1);
		VectorEx<const char* >itemsCSSEng = { lolc("Type1") , lolc("Type2"), lolc("Type3") };
		VectorEx<const char* >itemsCSSRus = { lolc(u8"Тип1") , lolc(u8"Тип2"), lolc(u8"Тип3") };
		DComboBox("Style##Crosshsnip", u8"Стиль##прицснип", SnipCrosshairStyle, itemsCSSEng, itemsCSSRus, "");

		X1Gui().PushItemWidth(PUSH_2);
		SliderInts("Size##Crosshsnip", u8"Размер##прицснип", SnipCrosshairSize, 1, 100, u8"Изменяет размер прицела");

		DCheckBox("Recoil##Crosshsnip", u8"Отдача##прицснип", SnipCrosshairRecoil, u8"Включает отображение отдачи");
		X1Gui().SameLine(SAME_LINE_1);
		DCheckBox("Outline##Crosshsnip", u8"Обводка##прицснип", SnipCrosshairOutline, u8"Включает обводку прицела");

		DColorEdit("Color##Crosshsnip", u8"Цвет##прицснип", SnipCrosshairColor);
	}

	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();
	DCheckBox("Spread Circle", u8"Круг разброса", SpreadCircel, u8"Включает отображение зоны разброса пуль");
	if (SpreadCircel)
	{
		X1Gui().SameLine(SAME_LINE_1);
		DColorEdit("Color##spread", u8"Цвет##разб", SpreadColor);
	}
	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();
	DCheckBox("Hit Marker", u8"Хит маркер", HitMarker, u8"Включает отображение попаданий");
	if (HitMarker)
	{
		X1Gui().SameLine(SAME_LINE_1);
		X1Gui().PushItemWidth(PUSH_3);
		SliderInts("Size##Hits", u8"Размер##хит", HitMarkerSize, 1, 10, u8"Изменяет размер хит маркера");
		X1Gui().PushItemWidth(PUSH_2);
		SliderInts("Tickness##Hits", u8"Толщина##хит", HitMarkerTickness, 1, 100, u8"Толщина линий");
		DColorEdit("Color##Hits", u8"Цвет##хит", HitMarkerColor);
	}
	/*X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();
	DCheckBox("Hit Trace", u8"Линии пападаний", HitTrace, u8"Включает отображение траектории попаданий");
	if (HitTrace)
	{
		X1Gui().SameLine(SAME_LINE_1);
		DColorEdit("Color##Hitstrr", u8"Цвет##хиттр", HitTraceColor);
	}*/


	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();

	DCheckBox("Hit Sound", u8"Хит саунд", HitSound, u8"Звуковое оповещение о попадании");
	if (HitSound)
	{
		if (!SoundList.empty())
		{
			X1Gui().SameLine(SAME_LINE_1);
			X1Gui().PushItemWidth(PUSH_1);
		
			VectorEx<const char*> itemsHSS;
			for (size_t i(0); i < SoundList.size(); i++)
			{
				itemsHSS.push_back(SoundList[i].c_str());
			}
			X1Gui().ComboBox(!MainSettings().Russian ? XorStr("Style##Hts") : XorStr(u8"Стиль##хтс"), &HitSoundStyle, itemsHSS);		
		}
	}

	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();
	DCheckBox("AntiFlash", u8"Анти флеш", AntiFlash, u8"Отключает ослепление");
	if (AntiFlash)
	{
		X1Gui().SameLine(SAME_LINE_1);
		X1Gui().PushItemWidth(PUSH_3);
		SliderInts("Alpha##flash", u8"Прозрачность", AntiFlashAlpha, 1, 255, u8"Максимальная прозрачность ослепления");
	}
	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();
	DCheckBox("Clan Tag", u8"Клан тег", ClanTagChanger, u8"Изменяет клан тег");
	if (ClanTagChanger)
	{
		X1Gui().SameLine(SAME_LINE_1);
		X1Gui().PushItemWidth(PUSH_1);
		VectorEx<const char* >itemsCSEng = { lolc("XONE > All"),lolc("VALVE"), lolc("Animation1"), lolc("Animation2"), lolc("Custom") };
		VectorEx<const char* >itemsCSRus = { lolc("XONE > All"),lolc("VALVE"),lolc(u8"Анимация1"), lolc(u8"Анимация2"), lolc(u8"Кастом") };
		DComboBox("Style##clan", u8"Стиль##клан", ClanTagChangerStyle, itemsCSEng, itemsCSRus, "");

		if (ClanTagChangerStyle == 4)
		{
			static char ClanBuf[64] = { 0 };
			TextEdit("Text##clan", u8"Текст##клан", ClanTagChangerText, ClanBuf, 64);
		}
	}
	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();
	DCheckBox("Chat Spam", u8"Чат спам", ChatSpam, u8"");
	if (ChatSpam)
	{
		X1Gui().SameLine(SAME_LINE_1);
		X1Gui().PushItemWidth(PUSH_1);
		VectorEx<const char* >itemsCSEng = { lolc("Standart"), lolc("Custom") };
		VectorEx<const char* >itemsCSRus = { lolc(u8"Стандарт"), lolc(u8"Кастом") };
		DComboBox("Style##spam", u8"Стиль##спам", ChatSpamStyle, itemsCSEng, itemsCSRus, "");
		if (ChatSpamStyle == 1)
		{
			static char ChatBuf[64] = { 0 };
			TextEdit("Text##spam", u8"Текст##спам", ChatSpamText, ChatBuf, 64);
		}
		if (X1Gui().Button(ChatSpamStart ? (true ? XorStr("Stop") : XorStr(u8"Стоп")) : (true ? XorStr("Start") : XorStr(u8"Старт"))))
			ChatSpamStart = !ChatSpamStart;

	}
	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();
	DCheckBox("LegitAA", u8"Легит АА", LegitAA, u8"Заметен при просмотре повтора игры.\nИспользовать только на свой страх и риск.");
	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();
	DCheckBox("Hand Chams", u8"Чамсы на руки", HandChams, u8"Включает чамсы на руках");
	if (HandChams)
	{
		X1Gui().SameLine(SAME_LINE_1);
		X1Gui().PushItemWidth(PUSH_1);
		VectorEx<const char* >itemsCSEng = { lolc("Textured"),lolc("Flat"), lolc("Wireframe"), lolc("Disable") };
		VectorEx<const char* >itemsCSRus = { lolc(u8"Текстурные"),lolc(u8"Плоские"), lolc(u8"Сетчатые"), lolc(u8"Убрать") };
		DComboBox("Style##hand", u8"Стиль##рук", HandChamsStyle, itemsCSEng, itemsCSRus, "");
		DColorEdit("Color##hand", u8"Цвет##рук", HandChamsColor);
	}

	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();
	DCheckBox("Damage Info", u8"Информация о уроне", DamageInfo, u8"Показывает урон");
	if (DamageInfo)
	{
		if (MainSettings().Russian)
			X1Gui().SameLine(SAME_LINE_1 + 15);
		else
			X1Gui().SameLine(SAME_LINE_1);
		X1Gui().PushItemWidth(PUSH_1);
		X1Gui().PushItemWidth(PUSH_1);
		VectorEx<const char* >itemsCSEng = { lolc("On Screen") , lolc("In World") };
		VectorEx<const char* >itemsCSRus = { lolc(u8"На экране"), lolc(u8"В мире") };
		DComboBox("Position##damage", u8"Позиция##ур", DamageInfoPosition, itemsCSEng, itemsCSRus, "");
		X1Gui().Spacing();
		if (MainSettings().Russian)
			X1Gui().SameLine(SAME_LINE_1 + 15);
		else
			X1Gui().SameLine(SAME_LINE_1);
		X1Gui().PushItemWidth(PUSH_1);
		X1Gui().PushItemWidth(PUSH_1);
		VectorEx<const char* >itemsDSSEng = { lolc("Damage") , lolc("Health") };
		VectorEx<const char* >itemsDSSRus = { lolc(u8"Урон"), lolc(u8"Здоровье") };
		DComboBox("Style##damage", u8"Стиль##ур", DamageInfoStyle, itemsDSSEng, itemsDSSRus, "");
		X1Gui().PushItemWidth(PUSH_2);
		SliderInts("Text Size##damage", u8"Размер текста##ур", TextDamageInfo, 9, MAX_FONT_SIZE, u8"");
		if (DamageInfoPosition == 0)
		{
			SliderFloats("Show Time", u8"Время показа", DamageInfoShowTime, 0.1, 10, "");
			SliderInts("Position X", u8"Позиция по X", DamageInfoPosX, 1, CGlobal::iScreenWidth, u8"");
			SliderInts("Position Y", u8"Позиция по Y", DamageInfoPosY, 1, CGlobal::iScreenHeight, u8"");
		}
		else if (DamageInfoPosition == 1)
		{
			SliderInts("Height", u8"Высота", DamageInfoHeight, 1, 100, u8"Максимальная высота");
			SliderInts("Speed", u8"Скорость", DamageInfoSpeed, 1, 100, u8"");
		}
		DColorEdit("Color##damage", u8"Цвет##ур", DamageInfoColor);
	}
	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();
	DCheckBox("Night Mode", u8"Ночной режим", NightMode, u8"Включает ночной режим");

	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();

	DCheckBox("Fake Lag", u8"Фейк лаг", FakeLag, u8"Использовать только на свой страх и риск.");
	if (FakeLag)
	{
		X1Gui().SameLine(SAME_LINE_1);
		X1Gui().PushItemWidth(PUSH_1);
		SliderInts("Factor", u8"Фактор", FakeLagFactor, 1, 50, u8"");
		DCheckBox("Bind##fake", u8"Бинд##фек", FakeLagBind.Enable, u8"");
		if (FakeLagBind.Enable)
		{
			X1Gui().SameLine(SAME_LINE_1);
			DCheckBox("Hold##fake", u8"Удержание##фек", FakeLagBind.Hold, u8"");
			X1Gui().PushItemWidth(PUSH_2);
			HotsKey("Button##fake", u8"Кнопка##фек", FakeLagBind.Button, u8"Задает кнопку активации");
		}
	}
	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();
	DCheckBox("Colored Walls", u8"Цветные стены", ColoredWalls, u8"");
	if (ColoredWalls)
	{
		X1Gui().SameLine(SAME_LINE_1);
		X1Gui().PushItemWidth(PUSH_1);
		/*	VectorEx<const char* >itemsCSEng = { lolc("Standart") , lolc("Textured"), lolc("Flat") };
			VectorEx<const char* >itemsCSRus = { lolc(u8"Стандарт"),lolc(u8"Текстурный"),lolc(u8"Плоский") };
			DComboBox("Style##clrwall", u8"Стиль##цлрв", ColoredWallsStyle, itemsCSEng, itemsCSRus, u8"Плоские и текстурные не сбрасываются");
	*/
		DColorEdit("Color##clrwall", u8"Цвет##цлрв", ColoredWallsColor);
	}
	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();
	DCheckBox("Knife Bot", u8"Авто нож", KnifeBot, u8"");

	if (KnifeBot)
	{
		X1Gui().SameLine(SAME_LINE_1);
		X1Gui().PushItemWidth(PUSH_1);
		SliderInts("Distance##knfb", u8"Дистанция##кфнб", KnifeBotDistance, 1, 100, u8"Дистанция в метрах");
		X1Gui().Spacing();
		X1Gui().SameLine(SAME_LINE_1);
		X1Gui().PushItemWidth(PUSH_1);
		VectorEx<const char* >itemsKNFEng = { lolc("All"), lolc("Team"), lolc("Enemy") };
		VectorEx<const char* >itemsKNFRus = { lolc(u8"Все"), lolc(u8"Команда"), lolc(u8"Враги") };
		DComboBox("Attack##knfb", u8"Атаковать##кфнб", KnifeBotFilter, itemsKNFEng, itemsKNFRus, "");
		X1Gui().PushItemWidth(PUSH_2);
		VectorEx<const char* >itemsKNFMEng = { lolc("Auto"), lolc("Left, Right"), lolc("Right"), lolc("Left") };
		VectorEx<const char* >itemsKNFMRus = { lolc("Auto"), lolc("Left, Right"), lolc("Right"), lolc("Left") };
		DComboBox("Mode##knfb", u8"Режим##кфнб", KnifeBotMode, itemsKNFMEng, itemsKNFMRus, "");

		DCheckBox("Bind##knfb", u8"Бинд##кфнб", KnifeBotBind.Enable, u8"");
		if (KnifeBotBind.Enable)
		{
			X1Gui().SameLine(SAME_LINE_1);
			DCheckBox("Hold##knfb", u8"Удержание##кфнб", KnifeBotBind.Hold, u8"");
			X1Gui().PushItemWidth(PUSH_2);
			HotsKey("Button##knfb", u8"Кнопка##кфнб", KnifeBotBind.Button, u8"Задает кнопку активации");
		}
	}
}