#pragma once

#include "main.h"

class CGui
{
public:
	bool bCursorActive, bInComboBox, bInMultiCombo, bInDetectKey, bInGetText, bInColorPick, bInNumberInput;
	int iCursorPos[ 2 ];
	bool bUpScroll, bDownScroll, bSecUpScroll, bSecDownScroll, bSliderUp, bSliderDown, bSecSliderUp, bSecSliderDown;

	struct sColorPicker
	{
		bool bOpen, bActive;
	} sColorPicker[ 60 ];

	struct sComboBox
	{
		bool bOpen, bActive;
		int iSelect;
		char* Name;
	} sComboBox[ 60 ];

	struct sMultiComboBox
	{
		bool bOpen, bActive;
	} sMultiComboBox[ 60 ];

	struct sKeyBox
	{
		bool bOpen, bActive;
	} sKeyBox[ 60 ];

	struct sNumberInput
	{
		bool bOpen, bActive;
	} sNumberInput[ 60 ];

	CGui();

	void DrawMouse();
	void GetMouse();
	bool MouseOver( int x, int y, int w, int h );
	bool LeftClick( int x, int y, int w, int h );

	void Save();
	void Load();

	void DrawCheckbox( int x, int y, int& value, const char* text, int alpha, int distance = 230 );
	void DrawColorPicker( int x, int y, int w, int & r, int & g, int & b, int num, const char * text, int distancecolor, int alpha );
	void DrawSlider( int x, int y, int w, int h, float& value, int min, int max, const char* text, int alpha, bool style );
	void DrawKeybox( int x, int y, int & value, int num, const char * text, int alpha, int distance = 230 );
	void DrawCombobox( int x, int y, int & value, int num, int max, int w, int h, std::string * structs, const char * text, int alpha, int distance );
	void DrawMultiCombobox( int x, int y, int * value, int num, int max, int w, int h, std::string * structs, const char * text, int alpha );

	void DrawNumberBox( int x, int y, int & value, int num, int w, int h, const char * text, int alpha, int distance );

	void DrawMenu();

	bool bLClick, bRClick, bLeftClickReleased, bRightClickReleased;

	const char *GetKeyName( int iKey )
	{
		switch( iKey )
		{
		case 0:
			return "NONE"; break;
		case 1:
			return "MLEF"; break;
		case 2:
			return "MRIGHT"; break;
		case 3:
			return "MMID"; break;
		case 4:
			return "MMID"; break;
		case 5:
			return "M4"; break;
		case 6:
			return "M5"; break;
		case 8:
			return "BACK"; break;
		case 9:
			return "TAB"; break;
		case 13:
			return "ENT"; break;
		case 16:
			return "SHFT"; break;
		case 20:
			return "CAPS"; break;
		case 27:
			return "ESC"; break;
		case 32:
			return "SPC"; break;
		case 33:
			return "PUP"; break;
		case 34:
			return "PDWN"; break;
		case 35:
			return "END"; break;
		case 36:
			return "POS1"; break;
		case 37:
			return "LARW"; break;
		case 38:
			return "UARW"; break;
		case 39:
			return "RARW"; break;
		case 40:
			return "DARW"; break;
		case 45:
			return "INS"; break;
		case 46:
			return "DEL"; break;
		case 48:
			return "0"; break;
		case 49:
			return "1"; break;
		case 50:
			return "2"; break;
		case 51:
			return "3"; break;
		case 52:
			return "4"; break;
		case 53:
			return "5"; break;
		case 54:
			return "6"; break;
		case 55:
			return "7"; break;
		case 56:
			return "8"; break;
		case 57:
			return "9"; break;
		case 65:
			return "A"; break;
		case 66:
			return "B"; break;
		case 67:
			return "C"; break;
		case 68:
			return "D"; break;
		case 69:
			return "E"; break;
		case 70:
			return "F"; break;
		case 71:
			return "G"; break;
		case 72:
			return "H"; break;
		case 73:
			return "I"; break;
		case 74:
			return "J"; break;
		case 75:
			return "K"; break;
		case 76:
			return "L"; break;
		case 77:
			return "M"; break;
		case 78:
			return "N"; break;
		case 79:
			return "O"; break;
		case 80:
			return "P"; break;
		case 81:
			return "Q"; break;
		case 82:
			return "R"; break;
		case 83:
			return "S"; break;
		case 84:
			return "T"; break;
		case 85:
			return "U"; break;
		case 86:
			return "V"; break;
		case 87:
			return "W"; break;
		case 88:
			return "X"; break;
		case 89:
			return "Y"; break;
		case 90:
			return "Z"; break;
		case 97:
			return "1"; break;
		case 98:
			return "2"; break;
		case 99:
			return "3"; break;
		case 100:
			return "4"; break;
		case 101:
			return "5"; break;
		case 102:
			return "6"; break;
		case 103:
			return "7"; break;
		case 104:
			return "8"; break;
		case 105:
			return "9"; break;
		case 106:
			return "*"; break;
		case 107:
			return "+"; break;
		case 109:
			return "-"; break;
		case 111:
			return "/"; break;
		case 112:
			return "F1"; break;
		case 113:
			return "F2"; break;
		case 114:
			return "F3"; break;
		case 115:
			return "F4"; break;
		case 116:
			return "F5"; break;
		case 117:
			return "F6"; break;
		case 118:
			return "F7"; break;
		case 119:
			return "F8"; break;
		case 120:
			return "F9"; break;
		case 121:
			return "F10"; break;
		case 122:
			return "F11"; break;
		case 123:
			return "F12"; break;
		case 144:
			return "NUM"; break;
		case 160:
			return "LSHFT"; break;
		case 161:
			return "RSHFT"; break;
		case 162:
			return "LCTRL"; break;
		case 163:
			return "RCTRL"; break;
		case 164:
			return "ALT"; break;
		case 165:
			return "ALT2"; break;
		case 186:
			return "U"; break;
		case 187:
			return "+"; break;
		case 188:
			return ","; break;
		case 189:
			return "-"; break;
		case 190:
			return "."; break;
		case 191:
			return "#"; break;
		case 192:
			return "Ö"; break;
		}
		return "";
	}
};

extern CGui g_Gui;

enum Keys
{
	eKEY_SPACE = 32,
	eKEY_A = 65,
	eKEY_B,
	eKEY_C,
	eKEY_D,
	eKEY_E,
	eKEY_F,
	eKEY_G,
	eKEY_H,
	eKEY_I,
	eKEY_J,
	eKEY_K,
	eKEY_L,
	eKEY_M,
	eKEY_N,
	eKEY_O,
	eKEY_P,
	eKEY_Q,
	eKEY_R,
	eKEY_S,
	eKEY_T,
	eKEY_U,
	eKEY_V,
	eKEY_W,
	eKEY_X,
	eKEY_Y,
	eKEY_Z,
	eKEY_0 = 48,
	eKEY_1,
	eKEY_2,
	eKEY_3,
	eKEY_4,
	eKEY_5,
	eKEY_6,
	eKEY_7,
	eKEY_8,
	eKEY_9,
	eKEY_PLUS = 187,
	eKEY_KOMMA,
	eKEY_MINUS,
	eKEY_DOT,
	eKEY_SHARP,
	eKEY_SZ = 219,
	eKEY_WALL = 226,
};