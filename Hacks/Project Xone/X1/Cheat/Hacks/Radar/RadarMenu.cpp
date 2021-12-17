#include "Radar.h"
#include"../../GUI/Gui.h"

void CRadar::Menu()
{

	VectorEx<const char*> RadTraceStyleEng = { lolc("Line"), lolc("Fov"),lolc("Fov + Line") };
	VectorEx<const char*> RadTraceStyleRus = { lolc(u8"Линия"),lolc(u8"Фов"), lolc(u8"Фов + Линия") };

	VectorEx<const char*> PointStyleEng = { lolc("Filled Box"), lolc("Box"),lolc("Filled Circle"),lolc("Circle"),lolc("None") };
	VectorEx<const char*> PointStyleRus = { lolc(u8"Залитый бокс"),lolc(u8"Бокс"), lolc(u8"Залитый круг"),lolc(u8"Круг"),lolc("None") };


	/*ColorEdit("ColorCT", u8"ColorCT", ColorCT);
	ColorEdit("ColorTT", u8"ColorCT", ColorTT);

	ColorEdit("ColorVisbleCT", u8"ColorVisbleCT", ColorVisbleCT);
	ColorEdit("ColorVisbleTT", u8"ColorVisbleTT", ColorVisbleTT);


	ColorEdit("ColorBackground", u8"ColorBackground", ColorBackground);

	ColorEdit("ColorLine", u8"ColorLine", ColorLine);

	ColorEdit("NameColor", u8"NameColor", NameColor);
*/

	X1Gui().PushItemWidth(360);

	DCheckBox("Radar In Game", u8"Внутриигровой радар", RadarInGame, u8"Показывает игровок на внутриигровом радаре");
	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();
	DCheckBox("Radar", u8"Радар", RadarActive, u8"Активирует радар чита");
	if (RadarActive)
	{
		X1Gui().SameLine();
		DCheckBox("With Map", u8"С картой", TexturedRadar, u8"Показывает карту на радаре чита");
	}
	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();
	if (RadarActive)
	{
		DCheckBox("Enemy", u8"Враги", Enemy, u8"Показывает врагов на радаре чита");

		X1Gui().SameLine();
		DCheckBox("Team", u8"Команда", Team, u8"Показывает команду на радаре чита");

		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();

		DColorEdit("Color CT", u8"Цвет КТ", ColorCT);
		X1Gui().SameLine();
		DColorEdit("Visble Color CT", u8"Видимый цвет КТ", ColorVisbleCT);


		DColorEdit("Color T", u8"Цвет Т", ColorTT);
		X1Gui().SameLine();
		DColorEdit("Visble Color T", u8"Видимый цвет Т", ColorVisbleTT);

		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();
		SliderInts("Zoom", u8"Приближение", Range, 1, 4000, u8"");
		X1Gui().Spacing();

		SliderInts("Position X", u8"Позиция по X", RadarPosX, 1, CGlobal::iScreenWidth, u8"");
		SliderInts("Position Y", u8"Позиция по Y", RadarPosY, 1, CGlobal::iScreenHeight, u8"");


		X1Gui().Spacing();

		SliderInts("Size X", u8"Размер по X", RadarSizeX, 1, 900, u8"");
		SliderInts("Size Y", u8"Размер по Y", RadarSizeY, 1, 900, u8"");


		X1Gui().Spacing();

		SliderInts("Alpha", u8"Прозрачность", Alpha, 1, 255, u8"");

		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();

		DCheckBox("Fov", u8"Фов", Fov, u8"Включает фов на радаре чита");

		X1Gui().SameLine();
		DCheckBox("Line", u8"Линии", Line, u8"Включает линии на радаре чита");
		X1Gui().SameLine();
		DCheckBox("OutLine", u8"Обводка", OutLine, u8"Включает обводку радара чита");
		X1Gui().SameLine();
		DCheckBox("Only In Radar", u8"Только  на радаре", OnlyInRadar, u8"Показывает игроков только в зоне чита");
		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();

		DColorEdit("Color Background", u8"Цвет фона", ColorBackground);
		DColorEdit("Color Line", u8"Цвет линий", ColorLine);

		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();

		SliderInts("Size##point", u8"Размер##прр", PointSize, 2, 50, u8"Размер точек игроков");
		DComboBox("Style##point", u8"Стиль##при", PointStyle, PointStyleEng, PointStyleRus, "Стиль точек игроков");
		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();

		DCheckBox("Sound", u8"Звук", Sound, u8"Показывает шаги на радаре чита");
		if (Sound)
		{
			SliderInts("Size##soud", u8"Размер##зв", SoundPointSize, 2, 50, u8"Размер точек шагов");
			DComboBox("Style##soud", u8"Стиль##зв", SoundPointStyle, PointStyleEng, PointStyleRus, "Стиль точек шагов");
			DColorEdit("Color##soud", u8"Цвет##зв", SoundColor);
		}

		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();

		DCheckBox("Trace", u8"Направление взгляда", Trace, u8"Показывает направления взглядов игроков на радаре чита");

		if (Trace)
		{
			SliderInts("Lenght##trace", u8"Длина##тр", TraceLenght, 2, 300, u8"Длина направлений игроков");
			DComboBox("Style##trace", u8"Стиль##тр", TraceStyle, RadTraceStyleEng, RadTraceStyleRus, "Стиль направлений игроков");
		}

		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Spacing();

		DCheckBox("Name", u8"Имена", Name, u8"Показывает имена игроков на радаре чита");
		if (Name)
		{
			SliderInts("Text Size##name", u8"Размер текста##им", TextNameSize, 9, MAX_FONT_SIZE, u8"");

			DColorEdit("Color##name", u8"Цвет##им", NameColor);
		}

	}
}