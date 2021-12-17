///*///////////////////////
//////////////////////////
/////PASTED BY IMORINEL///
/////////////////////////
/////////////////////*///
//#include "../../stdafx.h"
//#include "SettingsManager.h"
//#include "Menu.h"
//
//int MenuRed;
//int MenuGreen;
//int MenuBlue;
//int MenuAlpha;
//int TextRed;
//int TextGreen;
//int TextBlue;
//int TextAlpha;
//int BackgroundRed;
//int BackgroundGreen;
//int BackgroundBlue;
//int BackgroundAlpha;
//int OutlineRed;
//int OutlineGreen;
//int OutlineBlue;
//int OutlineAlpha;
//
//void NoSky(void)
//{
//
//	for (MaterialHandle_t i = Interfaces.pMaterialSystem->FirstMaterial(); i != Interfaces.pMaterialSystem->InvalidMaterial(); i = Interfaces.pMaterialSystem->NextMaterial(i))
//	{
//		IMaterial *pMaterial = Interfaces.pMaterialSystem->GetMaterial(i);
//
//		if (!pMaterial)
//			continue;
//
//		if (strstr(pMaterial->GetTextureGroupName(), XorStr("Sky")))
//		{
//			pMaterial->ColorModulate(0.0f, 0.0f, 0.0f);
//		}
//	}
//}
//
//void DisableNoSky(void)
//{
//
//	for (MaterialHandle_t i = Interfaces.pMaterialSystem->FirstMaterial(); i != Interfaces.pMaterialSystem->InvalidMaterial(); i = Interfaces.pMaterialSystem->NextMaterial(i))
//	{
//		IMaterial *pMaterial = Interfaces.pMaterialSystem->GetMaterial(i);
//
//		if (!pMaterial)
//			continue;
//
//		if (strstr(pMaterial->GetTextureGroupName(), XorStr("Sky")))
//		{
//			pMaterial->ColorModulate(1.00f, 1.00f, 1.00f);
//		}
//	}
//}
//
//void ASUS(void)
//{
//
//	for (MaterialHandle_t i = Interfaces.pMaterialSystem->FirstMaterial(); i != Interfaces.pMaterialSystem->InvalidMaterial(); i = Interfaces.pMaterialSystem->NextMaterial(i))
//	{
//		IMaterial *pMaterial = Interfaces.pMaterialSystem->GetMaterial(i);
//
//		if (!pMaterial)
//			continue;
//
//		if (strstr(pMaterial->GetTextureGroupName(), XorStr("World")))
//		{
//			pMaterial->AlphaModulate(0.4f);
//			pMaterial->SetMaterialVarFlag(MATERIAL_VAR_OPAQUETEXTURE, true);
//			pMaterial->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
//		}
//	}
//}
//
//void DisableASUS(void)
//{
//
//	for (MaterialHandle_t i = Interfaces.pMaterialSystem->FirstMaterial(); i != Interfaces.pMaterialSystem->InvalidMaterial(); i = Interfaces.pMaterialSystem->NextMaterial(i))
//	{
//		IMaterial *pMaterial = Interfaces.pMaterialSystem->GetMaterial(i);
//
//		if (!pMaterial)
//			continue;
//
//		if (strstr(pMaterial->GetTextureGroupName(), XorStr("World")))
//		{
//			pMaterial->AlphaModulate(1.0f);
//			pMaterial->SetMaterialVarFlag(MATERIAL_VAR_OPAQUETEXTURE, false);
//			pMaterial->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
//		}
//	}
//}
//
//void Colors(void)
//{
//
//	for (MaterialHandle_t i = Interfaces.pMaterialSystem->FirstMaterial(); i != Interfaces.pMaterialSystem->InvalidMaterial(); i = Interfaces.pMaterialSystem->NextMaterial(i))
//	{
//		IMaterial *pMaterial = Interfaces.pMaterialSystem->GetMaterial(i);
//
//		if (!pMaterial)
//			continue;
//
//		if (strstr(pMaterial->GetTextureGroupName(), XorStr("World")))
//		{
//			float r = Settings.GetSetting(Tab_Colors, Colors_WorldR) / 2550;
//			float g = Settings.GetSetting(Tab_Colors, Colors_WorldG) / 2550;
//			float b = Settings.GetSetting(Tab_Colors, Colors_WorldB) / 2550;
//			pMaterial->ColorModulate(r, g, b);
//		}
//	}
//}
//
//void DisableColors(void)
//{
//
//	for (MaterialHandle_t i = Interfaces.pMaterialSystem->FirstMaterial(); i != Interfaces.pMaterialSystem->InvalidMaterial(); i = Interfaces.pMaterialSystem->NextMaterial(i))
//	{
//		IMaterial *pMaterial = Interfaces.pMaterialSystem->GetMaterial(i);
//
//		if (!pMaterial)
//			continue;
//
//		if (strstr(pMaterial->GetTextureGroupName(), XorStr("World")))
//		{
//			pMaterial->ColorModulate(1, 1, 1);
//		}
//	}
//}
//
//void Night(void)
//{
//	for (MaterialHandle_t i = Interfaces.pMaterialSystem->FirstMaterial(); i != Interfaces.pMaterialSystem->InvalidMaterial(); i = Interfaces.pMaterialSystem->NextMaterial(i))
//	{
//		IMaterial *pMaterial = Interfaces.pMaterialSystem->GetMaterial(i);
//
//		if (!pMaterial)
//			continue;
//
//		if (strstr(pMaterial->GetTextureGroupName(), XorStr("World")))
//		{
//			pMaterial->ColorModulate(0.02f, 0.02f, 0.02f);
//		}
//
//		if (strstr(pMaterial->GetTextureGroupName(), XorStr("Lighs")))
//		{
//			pMaterial->ColorModulate(0.00f, 0.00f, 0.00f);
//			pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
//		}
//
//
//	}
//}
//
//
//void DisableNight(void)
//{
//
//	for (MaterialHandle_t i = Interfaces.pMaterialSystem->FirstMaterial(); i != Interfaces.pMaterialSystem->InvalidMaterial(); i = Interfaces.pMaterialSystem->NextMaterial(i))
//	{
//		IMaterial *pMaterial = Interfaces.pMaterialSystem->GetMaterial(i);
//
//		if (!pMaterial)
//			continue;
//
//		if (strstr(pMaterial->GetTextureGroupName(), XorStr("World")))
//		{
//			pMaterial->ColorModulate(1.00f, 1.00f, 1.00f);
//		}
//	}
//}
//
//void ForceUpdate()
//{
//	Interfaces.pEngine->ClientCmd_Unrestricted("cl_fullupdate", 0);
//
//	int worldtype = Settings.GetSetting(Tab_Changer, Changer_World);
//	if (worldtype == 0 || worldtype == 1)
//	{
//		DisableASUS();
//		DisableColors();
//	}
//	else if (worldtype == 2)
//		ASUS();
//	else if (worldtype == 3)
//		Night();
//
//	if (Settings.GetSetting(Tab_Changer, Changer_NoSky))
//		NoSky();
//	else
//		DisableNoSky();
//}
//
//float GlobalMenuAlpha = 0.f;
//
///* func buttons here */
//void Unhook();
//
//void SaveSettings();
//
//void LoadSettings();
//
//void baimsub();
//
//void resolversub();
//
////void ResetSettings();
//
//bool savesettings = false;
//bool loadsettings = false;
//bool resetsettings = false;
//
//int phase1 = 0;
//int phase2 = 30;
//int phase3 = 60;
//int phase4 = 90;
//
//bool Clicked_This_Frame;
//bool Holding_Mouse_1;
//bool Dont_Click;
//bool Holding_Menu;
//int Menu_Drag_X;
//int Menu_Drag_Y;
//int Tab_Count = 0;
//int M_Drag_X;
//int M_Drag_Y;
//bool keys[256];
//bool oldKeys[256];
//
//bool GetKeyPress(unsigned int key)
//{
//	try
//	{
//		if (keys[key] == true && oldKeys[key] == false)
//			return true;
//		else
//			return false;
//	}
//	catch (...)
//	{
//	}
//}
//
//std::vector< char* > KeyStrings = { "[ _ ]", "[M1]", "[M2]", "[BRK]", "[M3]", "[M4]", "[M5]",
//"[ _ ]", "[BSPC]", "[TAB]", "[ _ ]", "[ _ ]", "[ _ ]", "[ENTER]", "[ _ ]", "[ _ ]", "[SHFT]",
//"[CTRL]", "[ALT]","[PAUSE]","[CAPS]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
//"[ESC]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[SPACE]","[PGUP]", "[PGDOWN]", "[END]", "[HOME]", "[Left]",
//"[UP]", "[RIGHT]", "[DOWN]", "[ _ ]", "[PRNT]", "[ _ ]", "[PRTSCR]", "[INS]","[DEL]", "[ _ ]", "[0]", "[1]",
//"[2]", "[3]", "[4]", "[5]", "[6]", "[7]", "[8]", "[9]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
//"[ _ ]", "[A]", "[B]", "[C]", "[D]", "[E]", "[F]", "[G]", "[H]", "[I]", "[J]", "[K]", "[L]", "[M]", "[N]", "[O]", "[P]", "[Q]", "[R]", "[S]", "[T]", "[U]",
//"[V]", "[W]", "[X]","[Y]", "[Z]", "[LFTWIN]", "[RGHTWIN]", "[ _ ]", "[ _ ]", "[ _ ]", "[NUM0]", "[NUM1]",
//"[NUM2]", "[NUM3]", "[NUM4]", "[NUM5]", "[NUM6]","[NUM7]", "[NUM8]", "[NUM9]", "[*]", "[+]", "[_]", "[-]", "[.]", "[/]", "[F1]", "[F2]", "[F3]",
//"[F4]", "[F5]", "[F6]", "[F7]", "[F8]", "[F9]", "[F10]", "[F11]", "[F12]","[F13]", "[F14]", "[F15]", "[F16]", "[F17]", "[F18]", "[F19]", "[F20]", "[F21]",
//"[F22]", "[F23]", "[F24]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]","[ _ ]", "[ _ ]", "[ _ ]",
//"NUM LOCK", "SCROLL LOCK", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
//"[ _ ]", "[ _ ]","[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[LSHFT]", "[RSHFT]", "[LCTRL]",
//"[RCTRL]", "[LMENU]", "[RMENU]", "[ _ ]","[ _ ]", "[ _ ]","[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
//"[ _ ]", "[ _ ]", "[ _ ]", "[NTRK]", "[PTRK]", "[STOP]", "[PLAY]", "[ _ ]", "[ _ ]",
//"[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", ";", "+", ",", "-", ".", "/?", "~", "[ _ ]", "[ _ ]",
//"[ _ ]", "[ _ ]","[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
//"[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]","[ _ ]",
//"[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[{", "\\|", "}]", "'\"", "[ _ ]",
//"[ _ ]", "[ _ ]", "[ _ ]","[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
//"[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
//"[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
//"[ _ ]", "[ _ ]" };
//
//void DrawMouse()
//{
//	POINT Mouse;
//	POINT mp;
//	GetCursorPos(&mp);
//	ScreenToClient(GetForegroundWindow(), &mp);
//	Mouse.x = mp.x;
//	Mouse.y = mp.y;
//	static int Texturer = Interfaces.pSurface->CreateNewTextureID(true);
//	unsigned char buffer[4] = { 255, 255, 255, GlobalMenuAlpha };
//
//	Interfaces.pSurface->DrawSetTextureRGBA(Texturer, buffer, 1, 1);
//	Interfaces.pSurface->DrawSetTexture(Texturer);
//	Interfaces.pSurface->DrawSetColor(255, 255, 255, GlobalMenuAlpha);
//	/**/
//	Vertex_t Verts[3];
//	Verts[0].x = Mouse.x;
//	Verts[0].y = Mouse.y;
//	Verts[1].x = Mouse.x + 10;
//	Verts[1].y = Mouse.y + 4;
//	Verts[2].x = Mouse.x;
//	Verts[2].y = Mouse.y + 10;
//	Interfaces.pSurface->DrawTexturedPolygon(3, Verts);
//}
//
//void CMenu::Set_Current_Tab(int tab)
//{
//	Current_tab = tab;
//}
//
//int CMenu::GetCurret_Tab()
//{
//	return Current_tab;
//}
//
//void CMenu::Update_Frame()
//{
//	if (!Holding_Mouse_1)
//	{
//		if (GetClicked())
//		{
//			Holding_Menu = true;
//		}
//		else
//		{
//			Holding_Menu = false;
//		}
//	}
//
//	if (Holding_Menu)
//	{
//		MenuPOS NewPOS;
//		POINT Mouse;
//		POINT mp;
//		GetCursorPos(&mp);
//		ScreenToClient(GetForegroundWindow(), &mp);
//		Mouse.x = mp.x;
//		Mouse.y = mp.y;
//		NewPOS.x = Mouse.x - Menu_Drag_X;
//		NewPOS.y = Mouse.y - Menu_Drag_Y;
//		this->pos = NewPOS;
//	}
//}
//
//bool CMenu::GetClicked()
//{
//	if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
//	{
//		return false;
//	}
//
//	POINT Mouse;
//	POINT mp;
//	GetCursorPos(&mp);
//	ScreenToClient(GetForegroundWindow(), &mp);
//	Mouse.x = mp.x;
//	Mouse.y = mp.y;
//
//	if (Mouse.x > this->pos.x && Mouse.y > this->pos.y && Mouse.x < this->pos.x + 705 && Mouse.y < this->pos.y + 20)
//	{
//		if (!Holding_Mouse_1)
//		{
//			Menu_Drag_X = Mouse.x - pos.x;
//			Menu_Drag_Y = Mouse.y - pos.y;
//		}
//		return true;
//	}
//	else if (Mouse.x > this->pos.x && Mouse.y > this->pos.y && Mouse.x < this->pos.x + 175 && Mouse.y < this->pos.y + 150)
//	{
//		if (!Holding_Mouse_1)
//		{
//			Menu_Drag_X = Mouse.x - pos.x;
//			Menu_Drag_Y = Mouse.y - pos.y;
//		}
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
//bool CMenu::GetClickedM()
//{
//	if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
//	{
//		return false;
//	}
//
//	POINT Mouse;
//	POINT mp;
//	GetCursorPos(&mp);
//	ScreenToClient(GetForegroundWindow(), &mp);
//	Mouse.x = mp.x;
//	Mouse.y = mp.y;
//
//
//	if (Mouse.x > this->Mpos.x  && Mouse.y > this->Mpos.y && Mouse.x < this->Mpos.x + 200 && Mouse.y < this->Mpos.y + 30)
//	{
//		if (!Holding_Mouse_1)
//		{
//			M_Drag_X = Mouse.x - Mpos.x;
//			M_Drag_Y = Mouse.y - Mpos.y;
//		}
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
//CMenu Menu;
//class CButton;
//class CSlider;
//class CDropbox;
//class CDropbox2;
//class CKeybutton;
//class CTab;
//class CMenuBox;
//
//class CButton
//{
//private:
//	MenuPOS pos;
//	MenuPOS offset;
//	int Tab;
//	int Sub;
//
//	int Setting;
//	char* Name = "ERROR";
//
//	bool Hovering()
//	{
//		POINT Mouse;
//		POINT mp;
//		GetCursorPos(&mp);
//		ScreenToClient(GetForegroundWindow(), &mp);
//		Mouse.x = mp.x;
//		Mouse.y = mp.y;
//		if (Mouse.x > this->pos.x && Mouse.y > this->pos.y && Mouse.x < this->pos.x + 8 && Mouse.y < this->pos.y + 8)
//		{
//			return true;
//		}
//		return false;
//	}
//
//	bool Clicked()
//	{
//		if (!Clicked_This_Frame)
//		{
//			return false;
//		}
//		if (Holding_Mouse_1)
//		{
//			return false;
//		}
//		if (Dont_Click)
//		{
//			return false;
//		}
//
//		if (Hovering())
//		{
//			return true;
//		}
//		return false;
//	}
//
//public:
//	void Update()
//	{
//		this->pos.x = Menu.pos.x + this->offset.x;
//		this->pos.y = Menu.pos.y + this->offset.y;
//
//		if (Menu.GetCurret_Tab() == Tab)
//		{
//			if (Clicked())
//			{
//				Dont_Click = true;
//				Settings.SetSetting(Tab, Setting, !Settings.GetMenuSetting(Tab, Setting));
//			}
//		}
//	}
//
//	void Draw()
//	{
//		if (Menu.GetCurret_Tab() == Tab)
//		{
//			Interfaces.pSurface->DrawSetColor(5, 5, 5, GlobalMenuAlpha);
//			Interfaces.pSurface->DrawOutlinedRect(this->pos.x, this->pos.y, this->pos.x + 8, this->pos.y + 8);
//			Interfaces.pSurface->DrawSetColor(65, 65, 65, GlobalMenuAlpha);
//			Interfaces.pSurface->DrawFilledRect(this->pos.x + 1, this->pos.y + 1, this->pos.x + 7, this->pos.y + 7);
//
//			if (Settings.GetMenuSetting(this->Tab, this->Setting))
//			{
//				Interfaces.pSurface->DrawSetColor(255, 255, 255, GlobalMenuAlpha);
//				Interfaces.pSurface->DrawFilledRect(this->pos.x + 1, this->pos.y + 1, this->pos.x + 7, this->pos.y + 7);
//			}
//			//Interfaces.pSurface->DrawSetColor(25, 25, 25, 255);
//			//Interfaces.pSurface->DrawOutlinedRect(this->pos.x, this->pos.y, this->pos.x + 12, this->pos.y + 12);
//			Interfaces.pSurface->DrawT(this->pos.x + 20, this->pos.y - 3, CColor(255, 255, 255, GlobalMenuAlpha), Hacks.Font_Controls, false, this->Name);
//		}
//	}
//
//	void Init(int Tab, int Setting, bool defult, int x, int y, char* name, int sub)
//	{
//		this->Tab = Tab;
//		this->Setting = Setting;
//		this->offset.y = y;
//		this->offset.x = x;
//		this->Name = name;
//		this->Sub = sub;
//
//		Settings.SetSetting(Tab, Setting, defult);
//	}
//};
//
//class CSlider
//{
//private:
//	int Tab = 0;
//	int SubTab = 0;
//	int Setting = 0;
//	double Max = 100;
//	double Min = 0;
//	MenuPOS pos;
//	MenuPOS offset;
//	char* Name = "ERROR";
//	//char* SValue = "0";
//	bool Is_Holding;
//
//	bool GetClicked()
//	{
//		if (!Clicked_This_Frame)
//		{
//			this->Is_Holding = false;
//			return false;
//		}
//		if (Holding_Mouse_1)
//		{
//			if (!this->Is_Holding)
//			{
//				return false;
//			}
//		}
//		if (Dont_Click)
//			return false;
//
//		POINT Mouse;
//		POINT mp;
//		GetCursorPos(&mp);
//		ScreenToClient(GetForegroundWindow(), &mp);
//		Mouse.x = mp.x;
//		Mouse.y = mp.y;
//
//		if (Mouse.x > this->pos.x && Mouse.y > this->pos.y && Mouse.x < this->pos.x + 155 && Mouse.y < this->pos.y + 13)
//		{
//			this->Is_Holding = true;
//			return true;
//		}
//		else if (this->Is_Holding)
//		{
//			return true;
//		}
//		else
//		{
//			return false;
//		}
//	}
//
//public:
//	void Draw()
//	{
//		if (Menu.GetCurret_Tab() == Tab)
//		{
//			double Ratio = Settings.GetMenuSetting(this->Tab, this->Setting) / (this->Max - this->Min);
//			double Location = Ratio * 150;
//			Interfaces.pSurface->DrawSetColor(65, 65, 65, GlobalMenuAlpha);
//			Interfaces.pSurface->DrawFilledRect(this->pos.x + 1, this->pos.y + 6, this->pos.x + 154, this->pos.y + 12);
//			Interfaces.pSurface->DrawSetColor(255, 255, 255, GlobalMenuAlpha);
//			Interfaces.pSurface->DrawFilledRect(this->pos.x + 1, this->pos.y + 6, this->pos.x + Location, this->pos.y + 12); //test
//			Interfaces.pSurface->DrawSetColor(20, 20, 20, GlobalMenuAlpha);
//			Interfaces.pSurface->DrawOutlinedRect(this->pos.x, this->pos.y + 5, this->pos.x + 155, this->pos.y + 13);
//			Interfaces.pSurface->DrawT(this->pos.x, this->pos.y - 10, CColor(255, 255, 255, GlobalMenuAlpha), Hacks.Font_Controls, false, "%s", this->Name, (float)Settings.GetMenuSetting(this->Tab, this->Setting));
//			float Value1 = (float)Settings.GetMenuSetting(this->Tab, this->Setting);
//			char vl[128];
//			sprintf(vl, "%.f", Value1);
//			Interfaces.pSurface->DrawT(this->pos.x + Location, this->pos.y + 3.5, CColor(255, 255, 255, GlobalMenuAlpha), Hacks.Font_Controls, false, vl);
//		}
//	}
//
//	void Init(int x, int y, double min, double max, double defult, char* name, int tab, int setting, int sub)
//	{
//		offset.x = x;
//		offset.y = y;
//		Tab = tab;
//		Setting = setting;
//		Max = max;
//		Min = min;
//		Name = name;
//		SubTab = sub;
//		Settings.SetSetting(Tab, Setting, defult);
//	}
//
//	void Update()
//	{
//		this->pos.x = Menu.pos.x + offset.x;
//		this->pos.y = Menu.pos.y + offset.y;
//
//		// get clicked and changing value
//		if (Menu.GetCurret_Tab() == Tab)
//		{
//			if (this->GetClicked())
//			{
//				Dont_Click = true;
//				POINT Mouse;
//				POINT mp;
//				GetCursorPos(&mp);
//				ScreenToClient(GetForegroundWindow(), &mp);
//				Mouse.x = mp.x;
//				Mouse.y = mp.y;
//				// get differance
//				double idifference;
//				idifference = Mouse.x - this->pos.x;
//				// Set Value
//				double value = ((idifference / 150) * (this->Max - this->Min));
//				if (value < Min)
//				{
//					value = Min;
//				}
//				else if (value > Max)
//				{
//					value = Max;
//				}
//
//				Settings.SetSetting(Tab, Setting, value);
//			}
//		}
//	}
//};
//
//class CDropBox
//{
//private:
//	int Tab = 0;
//	int Setting = 0;
//
//	char* Name = "ERROR";
//	char* Parts[256];
//	int Amount = 0;
//
//	bool Dropping = false;
//
//	MenuPOS pos;
//	MenuPOS offset;
//	int Sub = 0;
//
//	bool GetClicked()
//	{
//		if (!Clicked_This_Frame)
//		{
//			return false;
//		}
//		if (Holding_Mouse_1)
//		{
//			return false;
//		}
//		if (Dont_Click)
//		{
//			return false;
//		}
//		POINT Mouse;
//		POINT mp;
//		GetCursorPos(&mp);
//		ScreenToClient(GetForegroundWindow(), &mp);
//		Mouse.x = mp.x;
//		Mouse.y = mp.y;
//
//		if (Mouse.x > this->pos.x && Mouse.y > this->pos.y && Mouse.x < this->pos.x + 155 && Mouse.y < this->pos.y + 20)
//		{
//			return true;
//		}
//		else
//		{
//			return false;
//		}
//	}
//
//	int GetPartClicked()
//	{
//		if (!Clicked_This_Frame)
//		{
//			return -1;
//		}
//		if (Holding_Mouse_1)
//		{
//			return -1;
//		}
//		if (Dont_Click)
//		{
//			return -1;
//		}
//
//		POINT Mouse;
//		POINT mp;
//		GetCursorPos(&mp);
//		ScreenToClient(GetForegroundWindow(), &mp);
//		Mouse.x = mp.x;
//		Mouse.y = mp.y;
//
//		for (int i = 1; i < this->Amount; i++)
//		{
//			if (Mouse.x > this->pos.x && Mouse.y > this->pos.y + (20 * (i)) && Mouse.x < this->pos.x + 155 && Mouse.y < this->pos.y + (20 * (i + 1)))
//			{
//				return i;
//			}
//		}
//		return -1;
//	}
//
//public:
//	void Update()
//	{
//		pos.x = Menu.pos.x + offset.x;
//		pos.y = Menu.pos.y + offset.y;
//
//		if (Tab == Menu.GetCurret_Tab())
//		{
//			if (this->GetClicked())
//			{
//				if (this->Dropping == true)
//				{
//					this->Dropping = false;
//				}
//				else
//				{
//					this->Dropping = true;
//				}
//				Dont_Click = true;
//			}
//			else if (this->Dropping)
//			{
//				int index = this->GetPartClicked();
//				if (index != -1)
//				{
//					Settings.SetSetting(this->Tab, this->Setting, index);
//					this->Dropping = false;
//					Dont_Click = true;
//				}
//				else if (Clicked_This_Frame && !Holding_Mouse_1)
//				{
//					Dropping = false;
//				}
//			}
//			else
//			{
//				Dropping = false;
//			}
//		}
//		else
//		{
//			Dropping = false;
//		}
//	}
//
//	void Draw()
//	{
//		if (Tab == Menu.GetCurret_Tab())
//		{
//			Interfaces.pSurface->DrawSetColor(35, 35, 35, GlobalMenuAlpha);
//			Interfaces.pSurface->DrawFilledRect(this->pos.x + 1, this->pos.y + 1, this->pos.x + 154, this->pos.y + 19);
//			Interfaces.pSurface->DrawSetColor(5, 5, 5, GlobalMenuAlpha);
//			Interfaces.pSurface->DrawOutlinedRect(this->pos.x, this->pos.y, this->pos.x + 155, this->pos.y + 20);
//			Interfaces.pSurface->DrawT(this->pos.x + 1, this->pos.y - 13, CColor(255, 255, 255, GlobalMenuAlpha), Hacks.Font_Controls, false, this->Name);
//
//			if (this->Dropping)
//			{
//				for (int i = 1; i < this->Amount; i++)
//				{
//					Interfaces.pSurface->DrawSetColor(35, 35, 35, GlobalMenuAlpha); //dark top color
//					Interfaces.pSurface->DrawFilledRect(this->pos.x, this->pos.y + (20 * i), this->pos.x + 155, this->pos.y + (20 * i) + 20);
//					Interfaces.pSurface->DrawSetColor(5, 5, 5, GlobalMenuAlpha);
//					Interfaces.pSurface->DrawOutlinedRect(pos.x, pos.y, pos.x + 155, pos.y + (20 * i) + 21);
//					Interfaces.pSurface->DrawT(this->pos.x + 11, this->pos.y + 4 + (20 * i), CColor(255, 255, 255, GlobalMenuAlpha), Hacks.Font_Controls, false, this->Parts[i]);
//					//part 2
//					int value = (int)Settings.GetMenuSetting(Tab, Setting);
//					if (value >= this->Amount)
//						value = 0;
//					//Interfaces.pSurface->DrawT(pos.x + 11, pos.y + 4, CColor(255, 255, 255, 255), Hacks.Font_Tahoma, false, Parts[value]);
//
//					static int Texture = Interfaces.pSurface->CreateNewTextureID(true); //need to make a texture with procedural true
//					unsigned char buffer[4] = { 152, 152, 152, GlobalMenuAlpha };//{ color.r(), color.g(), color.b(), color.a() };
//
//					Interfaces.pSurface->DrawSetTextureRGBA(Texture, buffer, 1, 1); //Texture, char array of texture, width, height
//					Interfaces.pSurface->DrawSetColor(152, 152, 152, GlobalMenuAlpha); // keep this full color and opacity use the RGBA @top to set values.
//					Interfaces.pSurface->DrawSetTexture(Texture); // bind texture
//
//					Vertex_t Verts2[3];
//					Verts2[0].x = pos.x + 145;
//					Verts2[0].y = pos.y + 8;
//					Verts2[1].x = pos.x + 150;
//					Verts2[1].y = pos.y + 8;
//					Verts2[2].x = pos.x + 147.5;
//					Verts2[2].y = pos.y + 11;
//
//					Interfaces.pSurface->DrawTexturedPolygon(3, Verts2);
//				}
//				int value = (int)Settings.GetMenuSetting(Tab, Setting);
//				if (value >= this->Amount)
//					value = 0;
//				Interfaces.pSurface->DrawT(this->pos.x + 11, this->pos.y + 4, CColor(255, 255, 255, GlobalMenuAlpha), Hacks.Font_Controls, false, this->Parts[value]);
//			}
//			else
//			{
//				int value = (int)Settings.GetMenuSetting(Tab, Setting);
//				if (value >= this->Amount)
//					value = 0;
//				Interfaces.pSurface->DrawT(this->pos.x + 11, this->pos.y + 4, CColor(255, 255, 255, GlobalMenuAlpha), Hacks.Font_Controls, false, this->Parts[value]);
//
//				static int Texture = Interfaces.pSurface->CreateNewTextureID(true); //need to make a texture with procedural true
//				unsigned char buffer[4] = { 152, 152, 152, GlobalMenuAlpha };//{ color.r(), color.g(), color.b(), color.a() };
//
//				Interfaces.pSurface->DrawSetTextureRGBA(Texture, buffer, 1, 1); //Texture, char array of texture, width, height
//				Interfaces.pSurface->DrawSetColor(152, 152, 152, GlobalMenuAlpha); // keep this full color and opacity use the RGBA @top to set values.
//				Interfaces.pSurface->DrawSetTexture(Texture); // bind texture
//
//				Vertex_t Verts2[3];
//				Verts2[0].x = pos.x + 145;
//				Verts2[0].y = pos.y + 8;
//				Verts2[1].x = pos.x + 150;
//				Verts2[1].y = pos.y + 8;
//				Verts2[2].x = pos.x + 147.5;
//				Verts2[2].y = pos.y + 11;
//
//				Interfaces.pSurface->DrawTexturedPolygon(3, Verts2);
//			}
//		}
//	}
//
//	void Init(int x, int y, int tab, int setting, char* name, int parts, std::vector< char* > arr, int sub)
//	{
//		Name = name;
//		Amount = parts;
//
//		for (int i = 0; i < parts; i++)
//		{
//			Parts[i] = arr[i];
//		}
//
//		Tab = tab;
//		Setting = setting;
//		offset.x = x;
//		offset.y = y;
//		Sub = sub;
//	}
//};
//
//class CDropBox2
//{
//private:
//	int Setting = 0;
//
//	char* Name = "ERROR";
//	char* Parts[256];
//	int Amount = 0;
//
//	bool Dropping = false;
//
//	MenuPOS pos;
//	MenuPOS offset;
//	int Sub = 0;
//
//	bool GetClicked()
//	{
//		if (!Clicked_This_Frame)
//		{
//			return false;
//		}
//		if (Holding_Mouse_1)
//		{
//			return false;
//		}
//		if (Dont_Click)
//		{
//			return false;
//		}
//		POINT Mouse;
//		POINT mp;
//		GetCursorPos(&mp);
//		ScreenToClient(GetForegroundWindow(), &mp);
//		Mouse.x = mp.x;
//		Mouse.y = mp.y;
//
//		if (Mouse.x > this->pos.x && Mouse.y > this->pos.y && Mouse.x < this->pos.x + 155 && Mouse.y < this->pos.y + 20)
//		{
//			return true;
//		}
//		else
//		{
//			return false;
//		}
//	}
//
//	int GetPartClicked()
//	{
//		if (!Clicked_This_Frame)
//		{
//			return -1;
//		}
//		if (Holding_Mouse_1)
//		{
//			return -1;
//		}
//		if (Dont_Click)
//		{
//			return -1;
//		}
//
//		POINT Mouse;
//		POINT mp;
//		GetCursorPos(&mp);
//		ScreenToClient(GetForegroundWindow(), &mp);
//		Mouse.x = mp.x;
//		Mouse.y = mp.y;
//
//		for (int i = 1; i < this->Amount; i++)
//		{
//			if (Mouse.x > this->pos.x && Mouse.y > this->pos.y + (20 * (i)) && Mouse.x < this->pos.x + 155 && Mouse.y < this->pos.y + (20 * (i + 1)))
//			{
//				return i;
//			}
//		}
//		return -1;
//	}
//
//public:
//	void Update()
//	{
//		pos.x = Menu.pos.x + offset.x;
//		pos.y = Menu.pos.y + offset.y;
//
//
//		if (this->GetClicked())
//		{
//			if (this->Dropping == true)
//			{
//				this->Dropping = false;
//			}
//			else
//			{
//				this->Dropping = true;
//			}
//			Dont_Click = true;
//		}
//		else if (this->Dropping)
//		{
//			int index = this->GetPartClicked();
//			if (index != -1)
//			{
//				Settings.SetSetting2(this->Setting, index);
//				this->Dropping = false;
//				Dont_Click = true;
//			}
//			else if (Clicked_This_Frame && !Holding_Mouse_1)
//			{
//				Dropping = false;
//			}
//		}
//		else
//		{
//			Dropping = false;
//		}
//	}
//
//	void Draw()
//	{
//		Interfaces.pSurface->DrawSetColor(35, 35, 35, GlobalMenuAlpha);
//		Interfaces.pSurface->DrawFilledRect(this->pos.x + 1, this->pos.y + 1, this->pos.x + 154, this->pos.y + 19);
//		Interfaces.pSurface->DrawSetColor(5, 5, 5, GlobalMenuAlpha);
//		Interfaces.pSurface->DrawOutlinedRect(this->pos.x, this->pos.y, this->pos.x + 155, this->pos.y + 20);
//		Interfaces.pSurface->DrawT(this->pos.x + 1, this->pos.y - 13, CColor(255, 255, 255, GlobalMenuAlpha), Hacks.Font_Controls, false, this->Name);
//
//		if (this->Dropping)
//		{
//			for (int i = 1; i < this->Amount; i++)
//			{
//				Interfaces.pSurface->DrawSetColor(35, 35, 35, GlobalMenuAlpha); //dark top color
//				Interfaces.pSurface->DrawFilledRect(this->pos.x, this->pos.y + (20 * i), this->pos.x + 155, this->pos.y + (20 * i) + 20);
//				Interfaces.pSurface->DrawSetColor(5, 5, 5, GlobalMenuAlpha);
//				Interfaces.pSurface->DrawOutlinedRect(pos.x, pos.y, pos.x + 155, pos.y + (20 * i) + 21);
//				Interfaces.pSurface->DrawT(this->pos.x + 11, this->pos.y + 4 + (20 * i), CColor(255, 255, 255, GlobalMenuAlpha), Hacks.Font_Controls, false, this->Parts[i]);
//				//part 2
//				int value = (int)Settings.GetMenuSetting2(Setting);
//				if (value >= this->Amount)
//					value = 0;
//				//Interfaces.pSurface->DrawT(pos.x + 11, pos.y + 4, CColor(255, 255, 255, 255), Hacks.Font_Tahoma, false, Parts[value]);
//
//				static int Texture = Interfaces.pSurface->CreateNewTextureID(true); //need to make a texture with procedural true
//				unsigned char buffer[4] = { 152, 152, 152, GlobalMenuAlpha };//{ color.r(), color.g(), color.b(), color.a() };
//
//				Interfaces.pSurface->DrawSetTextureRGBA(Texture, buffer, 1, 1); //Texture, char array of texture, width, height
//				Interfaces.pSurface->DrawSetColor(152, 152, 152, GlobalMenuAlpha); // keep this full color and opacity use the RGBA @top to set values.
//				Interfaces.pSurface->DrawSetTexture(Texture); // bind texture
//
//				Vertex_t Verts2[3];
//				Verts2[0].x = pos.x + 145;
//				Verts2[0].y = pos.y + 8;
//				Verts2[1].x = pos.x + 150;
//				Verts2[1].y = pos.y + 8;
//				Verts2[2].x = pos.x + 147.5;
//				Verts2[2].y = pos.y + 11;
//
//				Interfaces.pSurface->DrawTexturedPolygon(3, Verts2);
//			}
//			int value = (int)Settings.GetMenuSetting2(Setting);
//			if (value >= this->Amount)
//				value = 0;
//			Interfaces.pSurface->DrawT(this->pos.x + 11, this->pos.y + 4, CColor(255, 255, 255, GlobalMenuAlpha), Hacks.Font_Controls, false, this->Parts[value]);
//		}
//		else
//		{
//			int value = (int)Settings.GetMenuSetting2(Setting);
//			if (value >= this->Amount)
//				value = 0;
//			Interfaces.pSurface->DrawT(this->pos.x + 11, this->pos.y + 4, CColor(255, 255, 255, GlobalMenuAlpha), Hacks.Font_Controls, false, this->Parts[value]);
//
//			static int Texture = Interfaces.pSurface->CreateNewTextureID(true); //need to make a texture with procedural true
//			unsigned char buffer[4] = { 152, 152, 152, GlobalMenuAlpha };//{ color.r(), color.g(), color.b(), color.a() };
//
//			Interfaces.pSurface->DrawSetTextureRGBA(Texture, buffer, 1, 1); //Texture, char array of texture, width, height
//			Interfaces.pSurface->DrawSetColor(152, 152, 152, GlobalMenuAlpha); // keep this full color and opacity use the RGBA @top to set values.
//			Interfaces.pSurface->DrawSetTexture(Texture); // bind texture
//
//			Vertex_t Verts2[3];
//			Verts2[0].x = pos.x + 145;
//			Verts2[0].y = pos.y + 8;
//			Verts2[1].x = pos.x + 150;
//			Verts2[1].y = pos.y + 8;
//			Verts2[2].x = pos.x + 147.5;
//			Verts2[2].y = pos.y + 11;
//
//			Interfaces.pSurface->DrawTexturedPolygon(3, Verts2);
//		}
//	}
//
//	void Init(int x, int y, int setting, char* name, int parts, std::vector< char* > arr, int sub)
//	{
//		Name = name;
//		Amount = parts;
//
//		for (int i = 0; i < parts; i++)
//		{
//			Parts[i] = arr[i];
//		}
//
//		Setting = setting;
//		offset.x = x;
//		offset.y = y;
//		Sub = sub;
//	}
//};
//
//class CKeyButton
//{
//private:
//	MenuPOS pos;
//	MenuPOS offset;
//
//	int Tab;
//	int Setting;
//	bool Getting_New_Key;
//	char* Name = "ERROR";
//	int Sub = 0;
//
//	bool GetClicked()
//	{
//		if (!Clicked_This_Frame)
//		{
//			return false;
//		}
//		if (Holding_Mouse_1)
//		{
//			return false;
//		}
//		if (Dont_Click)
//		{
//			return false;
//		}
//
//		POINT Mouse;
//		POINT mp;
//		GetCursorPos(&mp);
//		ScreenToClient(GetForegroundWindow(), &mp);
//		Mouse.x = mp.x;
//		Mouse.y = mp.y;
//
//		if (Mouse.x > this->pos.x && Mouse.y > this->pos.y && Mouse.x < this->pos.x + 15 && Mouse.y < this->pos.y + 20)
//		{
//			return true;
//		}
//		else
//		{
//			return false;
//		}
//	}
//
//public:
//	void Draw()
//	{
//		if (Menu.GetCurret_Tab() == Tab)
//		{
//			//Interfaces.pSurface->DrawT(this->pos.x, this->pos.y - 12, CColor(255, 255, 255, 255), Hacks.Font_Tahoma, false, "%s", this->Name);
//			//Interfaces.pSurface->DrawSetColor(39, 42, 47, 255);
//			//Interfaces.pSurface->DrawFilledRect(this->pos.x, this->pos.y, this->pos.x + 80, this->pos.y + 20);
//			//Interfaces.pSurface->DrawSetColor(25, 25, 25, 255);
//			//Interfaces.pSurface->DrawOutlinedRect(this->pos.x, this->pos.y, this->pos.x + 80, this->pos.y + 20);
//			if (this->Getting_New_Key)
//			{
//				Interfaces.pSurface->DrawT(this->pos.x, this->pos.y, CColor(110, 110, 110, GlobalMenuAlpha), Hacks.Font_Controls, false, "[ _ ]");
//			}
//			else
//			{
//				if (Settings.GetMenuSetting(this->Tab, this->Setting) == -1)
//					Interfaces.pSurface->DrawT(this->pos.x, this->pos.y, CColor(90, 90, 90, GlobalMenuAlpha), Hacks.Font_Controls, false, "[ _ ]");
//				else
//				{
//					char* NameOfKey = KeyStrings[(int)Settings.GetMenuSetting(this->Tab, this->Setting)];
//					Interfaces.pSurface->DrawT(this->pos.x, this->pos.y, CColor(90, 90, 90, GlobalMenuAlpha), Hacks.Font_Controls, false, NameOfKey);
//				}
//			}
//		}
//	}
//
//	void Update()
//	{
//		pos.x = Menu.pos.x + offset.x;
//		pos.y = Menu.pos.y + offset.y;
//
//		if (Menu.GetCurret_Tab() == Tab)
//		{
//			if (Getting_New_Key)
//			{
//				for (int i = 0; i < 255; i++)
//				{
//					if (GetKeyPress(i))
//					{
//						if (i == VK_INSERT)
//						{
//							this->Getting_New_Key = false;
//							Settings.SetSetting(Tab, Setting, -1);
//							break;
//						}
//						if (i == VK_ESCAPE)
//						{
//							Settings.SetSetting(Tab, Setting, -1);
//							this->Getting_New_Key = false;
//							break;
//						}
//						Settings.SetSetting(Tab, Setting, i);
//						this->Getting_New_Key = false;
//						break;
//					}
//				}
//			}
//			else if (this->GetClicked())
//			{
//				this->Getting_New_Key = !this->Getting_New_Key;
//			}
//		}
//	}
//
//	void Init(int x, int y, int tab, int setting, char* name, int sub)
//	{
//		offset.x = x;
//		offset.y = y;
//		Tab = tab;
//		Setting = setting;
//		Name = name;
//		Sub = sub;
//		Settings.SetSetting(Tab, Setting, -1);
//	}
//};
//
//class CTab
//{
//public:
//	void Update()
//	{
//		pos.x = Menu.pos.x + 0;
//		pos.y = Menu.pos.y + 170 + ((270 / Tab_Count) * Tab);
//
//		if (GetClicked())
//		{
//			Dont_Click = true;
//			Menu.Set_Current_Tab(Tab);
//		}
//	}
//
//	void Draw()
//	{
//		if (Menu.GetCurret_Tab() == Tab)
//		{
//			Interfaces.pSurface->DrawSetColor(75, 75, 75, GlobalMenuAlpha);
//			Interfaces.pSurface->DrawFilledRect(pos.x - 1, pos.y - 2, pos.x + 174, pos.y + 12 + (220 / Tab_Count));
//			Interfaces.pSurface->DrawSetColor(5, 5, 5, GlobalMenuAlpha);
//			Interfaces.pSurface->DrawFilledRect(pos.x, pos.y - 1, pos.x + 175, pos.y + 11 + (220 / Tab_Count));
//			Interfaces.pSurface->DrawSetColor(25, 25, 25, GlobalMenuAlpha);
//			Interfaces.pSurface->DrawFilledRect(pos.x, pos.y, pos.x + 176, pos.y + 10 + (220 / Tab_Count));
//			Interfaces.pSurface->DrawT(pos.x + 60, pos.y + (220 / Tab_Count / 2) - 3, CColor(255, 255, 255, (GlobalMenuAlpha / 255.f) * 240.f), Hacks.Font_Tab, false, Name);
//			Interfaces.pSurface->DrawT(pos.x + 20 + IconOffset, pos.y + (220 / Tab_Count / 2) - 5, CColor(255, 255, 255, (GlobalMenuAlpha / 255.f) * 240.f), Hacks.Font_Icons, false, Icon);
//		}
//		else
//		{
//			Interfaces.pSurface->DrawT(pos.x + 60, pos.y + (220 / Tab_Count / 2) - 3, CColor(255, 255, 255, (GlobalMenuAlpha / 255.f) * 110.f), Hacks.Font_Tab, false, Name);
//			Interfaces.pSurface->DrawT(pos.x + 20 + IconOffset, pos.y + (220 / Tab_Count / 2) - 5, CColor(255, 255, 255, (GlobalMenuAlpha / 255.f) * 110.f), Hacks.Font_Icons, false, Icon);
//		}
//	}
//
//	void Init(char* name, int tab, char* icon, int iconoffset)
//	{
//		Name = name;
//		Tab = tab;
//		Icon = icon;
//		IconOffset = iconoffset;
//	}
//
//private:
//	int Tab;
//	MenuPOS pos;
//	char* Name = "ERROR";
//	char* Icon = "ERROR";
//	int IconOffset;
//
//	bool GetClicked()
//	{
//		if (!Clicked_This_Frame)
//		{
//			return false;
//		}
//		if (Holding_Mouse_1)
//		{
//			return false;
//		}
//
//		POINT Mouse;
//		POINT mp;
//		GetCursorPos(&mp);
//		ScreenToClient(GetForegroundWindow(), &mp);
//		Mouse.x = mp.x;
//		Mouse.y = mp.y;
//
//		if (Mouse.x > pos.x && Mouse.y > pos.y && Mouse.x < pos.x + 175 && Mouse.y < pos.y + ((270 / Tab_Count)))
//		{
//			return true;
//		}
//		else
//		{
//			return false;
//		}
//	}
//};
//
//class CMenuBox
//{
//public:
//	void Init(char* name, int x, int y, int w, int h, int tab, int sub)
//	{
//		Name = name;
//		offset.x = x;
//		offset.y = y;
//		size.x = w;
//		size.y = h;
//		Tab = tab;
//		SubTab = sub;
//	}
//
//	void Update()
//	{
//		pos.x = Menu.pos.x + offset.x;
//		pos.y = Menu.pos.y + offset.y;
//	}
//
//	void Draw()
//	{
//		if (Tab == Menu.GetCurret_Tab())
//		{
//			int Width = 0, Height = 0;
//			char Buffer[256] = { '\0' };
//			va_list Args;
//			va_start(Args, Name);
//			vsprintf_s(Buffer, Name, Args);
//			va_end(Args);
//			size_t Size = strlen(Buffer);
//			/* set up size.xidebuffer*/
//			wchar_t* wBuffer = new wchar_t[Size];
//			/* char -> size.xchar */
//			mbstowcs_s(nullptr, wBuffer, Size, Buffer, Size - 1);
//			/* check center */
//			Interfaces.pSurface->GetTextSize(Hacks.Font_Controls, wBuffer, Width, Height);
//			Interfaces.pSurface->DrawSetColor(5, 5, 5, GlobalMenuAlpha);
//			Interfaces.pSurface->DrawLine(pos.x, pos.y, pos.x, pos.y + size.y);
//			Interfaces.pSurface->DrawLine(pos.x, pos.y + size.y, pos.x + size.x, pos.y + size.y);
//			Interfaces.pSurface->DrawLine(pos.x + size.x, pos.y, pos.x + size.x, pos.y + size.y);
//			Interfaces.pSurface->DrawLine(pos.x, pos.y, pos.x + 8, pos.y);
//			Interfaces.pSurface->DrawLine(pos.x + (Width)+14, pos.y, pos.x + size.x, pos.y);
//			Interfaces.pSurface->DrawSetColor(65, 65, 65, GlobalMenuAlpha);
//			Interfaces.pSurface->DrawLine(pos.x + 1, pos.y + 1, pos.x + 1, pos.y + size.y - 1);
//			Interfaces.pSurface->DrawLine(pos.x + 1, pos.y + size.y - 1, pos.x + size.x - 1, pos.y + size.y - 1);
//			Interfaces.pSurface->DrawLine(pos.x + size.x - 1, pos.y + 1, pos.x + size.x - 1, pos.y + size.y - 1);
//			Interfaces.pSurface->DrawLine(pos.x + 1, pos.y + 1, pos.x + 7, pos.y + 1);
//			Interfaces.pSurface->DrawLine(pos.x + (Width)+14, pos.y + 1, pos.x + size.x - 1, pos.y + 1);
//			Interfaces.pSurface->DrawT(pos.x + 10, pos.y - 7, CColor(255, 255, 255, GlobalMenuAlpha), Hacks.Font_Controls, false, Name);
//		}
//	}
//
//private:
//	MenuPOS pos;
//	MenuPOS offset;
//	MenuPOS size;
//	char* Name = "ERROR";
//	int Tab;
//	char* Icon = "ERROR";
//	int SubTab = 0;
//};
//
//class CFunctionButton
//{
//public:
//	void Update()
//	{
//		this->pos.x = Menu.pos.x + this->offset.x;
//		this->pos.y = Menu.pos.y + this->offset.y;
//
//		if (Menu.GetCurret_Tab() == Tab)
//		{
//			if (this->GetClicked())
//			{
//				DWORD disfunc = Function;
//				__asm
//				{
//					CALL disfunc
//				}
//			}
//		}
//	}
//
//	void Draw()
//	{
//		if (Menu.GetCurret_Tab() == Tab)
//		{
//			Interfaces.pSurface->DrawSetColor(5, 5, 5, GlobalMenuAlpha);
//			Interfaces.pSurface->DrawFilledRect(this->pos.x, this->pos.y, this->pos.x + 155, this->pos.y + 20);
//			Interfaces.pSurface->DrawSetColor(50, 50, 50, GlobalMenuAlpha);
//			Interfaces.pSurface->DrawOutlinedRect(this->pos.x + 1, this->pos.y + 1, this->pos.x + 154, this->pos.y + 19);
//			Interfaces.pSurface->DrawSetColor(35, 35, 35, GlobalMenuAlpha);
//			Interfaces.pSurface->DrawFilledRect(this->pos.x + 2, this->pos.y + 2, this->pos.x + 153, this->pos.y + 18);
//			Interfaces.pSurface->DrawT(this->pos.x + 77.5, this->pos.y + 3, CColor(255, 255, 255, GlobalMenuAlpha), Hacks.Font_Controls, true, Name);
//		}
//	}
//
//	void Init(int x, int y, DWORD function, char* name, int tab)
//	{
//		Function = function;
//		Name = name;
//		offset.x = x;
//		offset.y = y;
//		Tab = tab;
//	}
//
//	bool GetClicked()
//	{
//		if (!Clicked_This_Frame)
//		{
//			return false;
//		}
//		if (Holding_Mouse_1)
//		{
//			return false;
//		}
//		if (Dont_Click)
//		{
//			return false;
//		}
//
//		POINT Mouse;
//		POINT mp;
//		GetCursorPos(&mp);
//		ScreenToClient(GetForegroundWindow(), &mp);
//		Mouse.x = mp.x;
//		Mouse.y = mp.y;
//
//		if (Mouse.x > this->pos.x && Mouse.y > this->pos.y && Mouse.x < this->pos.x + 150 && Mouse.y < this->pos.y + 20)
//		{
//			return true;
//		}
//
//		return false;
//	}
//
//private:
//	DWORD Function;
//	char* Name = "NO NAME";
//	MenuPOS pos;
//	MenuPOS offset;
//	int Tab = -1;
//};
//
//class CFunctionButton2
//{
//public:
//	void Update()
//	{
//		this->pos.x = Menu.pos.x + this->offset.x;
//		this->pos.y = Menu.pos.y + this->offset.y;
//
//
//		if (this->GetClicked())
//		{
//			DWORD disfunc = Function;
//			__asm
//			{
//				CALL disfunc
//			}
//		}
//
//	}
//
//	void Draw()
//	{
//
//		Interfaces.pSurface->DrawSetColor(5, 5, 5, GlobalMenuAlpha);
//		Interfaces.pSurface->DrawFilledRect(this->pos.x, this->pos.y, this->pos.x + 155, this->pos.y + 20);
//		Interfaces.pSurface->DrawSetColor(50, 50, 50, GlobalMenuAlpha);
//		Interfaces.pSurface->DrawOutlinedRect(this->pos.x + 1, this->pos.y + 1, this->pos.x + 154, this->pos.y + 19);
//		Interfaces.pSurface->DrawSetColor(35, 35, 35, GlobalMenuAlpha);
//		Interfaces.pSurface->DrawFilledRect(this->pos.x + 2, this->pos.y + 2, this->pos.x + 153, this->pos.y + 18);
//		Interfaces.pSurface->DrawT(this->pos.x + 77.5, this->pos.y + 3, CColor(255, 255, 255, GlobalMenuAlpha), Hacks.Font_Controls, true, Name);
//
//	}
//
//	void Init(int x, int y, DWORD function, char* name)
//	{
//		Function = function;
//		Name = name;
//		offset.x = x;
//		offset.y = y;
//	}
//
//	bool GetClicked()
//	{
//		if (!Clicked_This_Frame)
//		{
//			return false;
//		}
//		if (Holding_Mouse_1)
//		{
//			return false;
//		}
//		if (Dont_Click)
//		{
//			return false;
//		}
//
//		POINT Mouse;
//		POINT mp;
//		GetCursorPos(&mp);
//		ScreenToClient(GetForegroundWindow(), &mp);
//		Mouse.x = mp.x;
//		Mouse.y = mp.y;
//
//		if (Mouse.x > this->pos.x && Mouse.y > this->pos.y && Mouse.x < this->pos.x + 150 && Mouse.y < this->pos.y + 20)
//		{
//			return true;
//		}
//
//		return false;
//	}
//
//private:
//	DWORD Function;
//	char* Name = "NO NAME";
//	MenuPOS pos;
//	MenuPOS offset;
//};
//
//std::vector< CSlider > Sliders;
//std::vector< CButton > Buttons;
//std::vector< CDropBox > Dropboxes;
//std::vector< CDropBox2 > Dropboxes2;
//std::vector< CKeyButton > KeyButtons;
//std::vector< CMenuBox > MenuBoxs;
//std::vector< CTab > Tabs;
//
//std::vector< CFunctionButton > FuncButtons;
//std::vector< CFunctionButton2 > FuncButtons2;
//
//int Dropboxs_Amount = 0;
//int Dropboxs_Amount2 = 0;
//
//void AddNewButton(int Tab, int Setting, bool defult, int x, int y, char* name)
//{
//	CButton Filler;
//	Filler.Init(Tab, Setting, defult, x, y, name, -1);
//	Buttons.push_back(Filler);
//}
//
//void AddNewSlider(int x, int y, double min, double max, double defult, char* name, int tab, int setting)
//{
//	CSlider Slid;
//	Slid.Init(x, y, min, max, defult, name, tab, setting, -1);
//	Sliders.push_back(Slid);
//}
//
//void AddNewDropbox(int x, int y, int tab, int setting, char* name, int parts, std::vector< char* > arr)
//{
//	CDropBox Dropper;
//	Dropper.Init(x, y, tab, setting, name, parts, arr, -1);
//	Dropboxes.push_back(Dropper);//
//	Dropboxs_Amount++;
//}
//
//void AddNewDropbox2(int x, int y, int setting, char* name, int parts, std::vector< char* > arr)
//{
//	CDropBox2 Dropper;
//	Dropper.Init(x, y, setting, name, parts, arr, -1);
//	Dropboxes2.push_back(Dropper);//
//	Dropboxs_Amount2++;
//}
//
//void AddNewKeyButton(int x, int y, int tab, int setting, char* name)
//{
//	CKeyButton KeyButton;
//	KeyButton.Init(x, y, tab, setting, name, -1);
//	KeyButtons.push_back(KeyButton);
//}
//
//void AddNewTab(char* Name, int Tab, char* Icon, int IconOffset)
//{
//	CTab Filler;
//	Filler.Init(Name, Tab, Icon, IconOffset);
//	Tabs.push_back(Filler);
//	Tab_Count++;
//}
//
//void AddNewMenuBox(char* name, int x, int y, int w, int h, int tab)
//{
//	CMenuBox Filler;
//	Filler.Init(name, x, y, w, h, tab, -1);
//	MenuBoxs.push_back(Filler);
//}
//
//void AddNewFuncButton(int x, int y, DWORD function, char* name, int tab)
//{
//	CFunctionButton Filler;
//	Filler.Init(x, y, function, name, tab);
//	FuncButtons.push_back(Filler);
//}
//
//void AddNewFuncButton2(int x, int y, DWORD function, char* name)
//{
//	CFunctionButton2 Filler;
//	Filler.Init(x, y, function, name);
//	FuncButtons2.push_back(Filler);
//}
//
//extern player_info_t* Info;
//std::vector< char* > ingameNames{ "NONE", "DESERT-EAGLE", "DUAL-BERETTAS", "FIVE-SEVEN", "GLOCK", "AK-47", "AUG", "AWP", "FAMAS", "G3SG1",
//"GALIL-AR", "M249", "M4A4", "MAC-10", "P90", "UMP-45", "XM1014", "PP-BIZON", "MAG-7", "NEGEV", "SAWED-OFF", "TEC-9",
//"P2000", "MP7", "MP9", "NOVA", "P250", "SCAR-20", "SG-553", "SSG-08", "CT-Knife", "T-Knife", "M4A1-S", "USP-S", "CZ75-AUTO", "R8-Revolver" };
//int page[256];
//int selected[256];
//int realselected[256];
//
//void drawselectbox(int amount, char* arr[100], int width, int height, int setting, int realvals[100])
//{
//	int startx = Menu.pos.x + width + 3;
//	int starty = Menu.pos.y + height + 3;
//	Interfaces.pSurface->DrawSetColor(255, 255, 255, GlobalMenuAlpha);
//	Interfaces.pSurface->DrawOutlinedRect(startx - 2, starty - 2, startx + 137, starty + 202);
//
//	for (int i = 0; i < 10; i++)
//	{
//		int number = i + 10 * page[setting];
//		if (number > amount)
//		{
//		}
//		else
//		{
//			Interfaces.pSurface->DrawSetColor(255, 255, 255, GlobalMenuAlpha);
//			Interfaces.pSurface->DrawFilledRect(startx, starty + i * 20, startx + 120, starty + i * 20 + 20);
//			Interfaces.pSurface->DrawSetColor(30, 30, 30, GlobalMenuAlpha);
//			Interfaces.pSurface->DrawFilledRect(startx + 1, starty + i * 20 + 1, startx + 119, starty + i * 20 + 19);
//			if (number == selected[setting])
//			{
//				Interfaces.pSurface->DrawT(startx + 3, starty + i * 20 + 3, CColor(255, 255, 255, GlobalMenuAlpha), Hacks.Font_Controls, false, arr[number]);
//				realselected[setting] = realvals[number];
//			}
//			else
//				Interfaces.pSurface->DrawT(startx + 3, starty + i * 20 + 3, CColor(255, 255, 255, GlobalMenuAlpha), Hacks.Font_Controls, false, arr[number]);
//		}
//	}
//	double drawbarheight = (10 * 200);
//	drawbarheight /= amount;
//	if (drawbarheight > 200)
//		drawbarheight = 200;
//	Interfaces.pSurface->DrawSetColor(255, 255, 255, GlobalMenuAlpha);
//	Interfaces.pSurface->DrawOutlinedRect(startx + 125, starty, startx + 135, starty + 200);
//	Interfaces.pSurface->DrawFilledRect(startx + 127, starty + page[setting] * drawbarheight, startx + 133, starty + drawbarheight + page[setting] * drawbarheight);
//	// scroll
//	if (Clicked_This_Frame)
//	{
//		if (!Holding_Mouse_1)
//		{
//			if (!Dont_Click)
//			{
//				POINT Mouse;
//				POINT mp;
//				GetCursorPos(&mp);
//				ScreenToClient(GetForegroundWindow(), &mp);
//				Mouse.x = mp.x;
//				Mouse.y = mp.y;
//				if (Mouse.x > startx + 125 && Mouse.y > starty && Mouse.x < startx + 135 && Mouse.y < starty + 200)
//				{
//					int percentage = (Mouse.y - starty);
//					for (int i = 0; i < 10; i++)
//					{
//						if (drawbarheight * i < percentage && percentage < drawbarheight * (i + 1))
//						{
//							page[setting] = i;
//						}
//					}
//				}
//			}
//		}
//	}
//	// getting clicked
//	if (Clicked_This_Frame)
//	{
//		if (!Holding_Mouse_1)
//		{
//			if (!Dont_Click)
//			{
//				POINT Mouse;
//				POINT mp;
//				GetCursorPos(&mp);
//				ScreenToClient(GetForegroundWindow(), &mp);
//				Mouse.x = mp.x;
//				Mouse.y = mp.y;
//				if (Mouse.x > startx && Mouse.y > starty && Mouse.x < startx + 120 && Mouse.y < starty + 200)
//				{
//					int percentage = (Mouse.y - starty);
//					for (int i = 0; i < 10; i++)
//					{
//						if (20 * i < percentage && percentage < 20 * (i + 1))
//						{
//							int number = i + 10 * page[setting];
//							selected[setting] = number;
//						}
//					}
//				}
//			}
//		}
//	}
//}
//
//void skinchanger()
//{
//}
//
//void SetupMenu()
//{
//	char* arr[512] = { "ERROR","ERROR","ERROR","ERROR","ERROR","ERROR","ERROR","ERROR","ERROR","ERROR","ERROR", };
//
//	//Tabs
//	AddNewTab("RAGE", Tab_Ragebot, "s", 0);
//	AddNewTab("ESP", Tab_Visuals, "t", 0);
//	AddNewTab("CHANGER", Tab_Changer, "u", 0);
//	AddNewTab("MISC", Tab_Misc, "v", 0);
//
//	/* Ragebot */
//
//	AddNewMenuBox("Aimbot", 195, 20, 235, 455, Tab_Ragebot);
//	{
//		//Dropbox Config
//		std::vector< char* > Selection = { "Distance", "Distance","Cycle" };
//		std::vector< char* > Hitbox = { "Head", "Head", "Neck", "Chest", "Pelvis" };
//		std::vector< char* > MultipointType = { "Compression", "Compression", "Max", };
//		std::vector< char * > MultiBox = { "Off", "Off", "Vitals", "Max" };
//
//		//Controls
//		AddNewButton(Tab_Ragebot, Ragebot_AimbotEnabled, false, 215, 40, "Enabled");
//		AddNewDropbox(235, 70, Tab_Ragebot, Ragebot_Selection, "Target selection", 3, Selection);
//		AddNewDropbox(235, 110, Tab_Ragebot, Ragebot_Hitbox, "Target hitbox", 5, Hitbox);
//		AddNewButton(Tab_Ragebot, Ragebot_Multipoint, false, 215, 140, "Multi-point");
//		AddNewDropbox(235, 155, Tab_Ragebot, Ragebot_MultipointType, "", 3, MultipointType);
//		AddNewSlider(235, 175, 0, 100, 50, "", Tab_Ragebot, Ragebot_PS);
//		AddNewDropbox(235, 205, Tab_Ragebot, Ragebot_Multibox, "Multi-box", 4, MultiBox);
//		AddNewButton(Tab_Ragebot, Ragebot_Velocity, false, 215, 235, "Velocity multi-box");
//		AddNewButton(Tab_Ragebot, Ragebot_AutoShoot, false, 215, 255, "Automatic fire");
//		AddNewButton(Tab_Ragebot, Ragebot_Autoscope, false, 215, 275, "Automatic scope");
//		AddNewButton(Tab_Ragebot, Ragebot_SilentAim, false, 215, 295, "Silent aim");
//		AddNewSlider(235, 325, 0, 100, 0, "Hit chance", Tab_Ragebot, Ragebot_Hitchance);
//		AddNewSlider(235, 355, 0, 120, 0, "Penetration damage", Tab_Ragebot, Ragebot_MinDamage);
//		AddNewButton(Tab_Ragebot, Ragebot_AcceptKill, false, 215, 380, "Accept kill damage");
//	}
//
//	AddNewMenuBox("Other", 450, 20, 235, 230, Tab_Ragebot);
//	{
//		//Dropbox Config
//		//std::vector<char*> Accuracy = { "Off", "Off", "FPS Save", "Medium", "Maximum" };
//		std::vector< char* > PreferBodyAim = { "Off", "Off", "Adaptive", "Max" };
//		std::vector< char* > Resolver = {
//			"Off", // изи0
//			"Off", // изи1
//			"On",
//		};
//		std::vector< char* > ResolverX = {
//			"Off", // изи0
//			"Off", // изи1
//			"On",
//		};
//
//		//Controls
//		//AddNewButton(Tab_Ragebot, Ragebot_Resolver, false, 470, 40, "Anti Aim Correction");
//		//AddNewDropbox(490, 70, Tab_Ragebot, Ragebot_Accuracy, "Accuracy Boost", 5, Accuracy);
//		AddNewDropbox(490, 40, Tab_Ragebot, Ragebot_PreferBodyAim, "Prefer body-aim type", 4, PreferBodyAim);
//
//		AddNewButton(Tab_Ragebot, Ragebot_PositionAdjustment, false, 470, 70, "Position adjustment");
//		//AddNewDropbox(490, 100, Tab_Ragebot, Ragebot_ResolverX, "Resolver Pitch", 4, PreferBodyAim);
//		AddNewButton(Tab_Ragebot, Ragebot_Resolver, false, 470, 90, "Resolver");
//		AddNewDropbox(490, 120, Tab_Ragebot, Ragebot_ResolverY, "Resolver mode", 3, Resolver);
//		AddNewDropbox(490, 150, Tab_Ragebot, Ragebot_ResolverX, "Resolver pitch", 3, ResolverX);
//		AddNewButton(Tab_Ragebot, Ragebot_BaimEdge, false, 490, 180, "Prefer body-aim when edge");
//		AddNewSlider(490, 215, 0, 100, 0, "Prefer body-aim while X hp", Tab_Ragebot, Ragebot_BaimHP);
//
//	}
//
//	AddNewMenuBox("Anti-Aim", 450, 260, 235, 215, Tab_Ragebot);
//	{
//		//Dropbox Config
//		std::vector< char* > Antiaim_Pitch = { "Off", "Off","Emotion", "Down", "Minimal", };
//		std::vector< char* > Antiaim_Yaw = { "Off", "Off", "Sideways Distortion", "180", "Crooked Distortion", "Spin", "180 Distortion", "Switch" };
//		std::vector< char* > AtTargets = { "Off", "Off", "Nospread", "Normal" };
//
//		AddNewButton(Tab_Ragebot, AA_Enable, false, 470, 280, "Enable");
//		AddNewDropbox(490, 310, Tab_Ragebot, AntiAim_Pitch, "Pitch", 5, Antiaim_Pitch);
//		AddNewDropbox(490, 350, Tab_Ragebot, AntiAim_Yaw, "Yaw", 8, Antiaim_Yaw);
//		AddNewDropbox(490, 390, Tab_Ragebot, AA_AtTarget, "At Targets", 4, AtTargets);
//		AddNewButton(Tab_Ragebot, AntiAim_FakeHitbox, false, 470, 420, "Fake Hitbox");
//	}
//
//	/* MISC */
//	{
//		AddNewMenuBox("Miscellaneous", 195, 20, 235, 455, Tab_Misc);
//		{
//			//Dropbox Config
//			std::vector< char* > AutoStraferMode = { "Silent", "Silent", "Normal", "Sideways", "Forward" };
//			std::vector< char* > ClanTag = { "usia.cc", "usia.cc", "skeet.cc" };
//
//
//			//Controls
//			AddNewSlider(235, 40, 0, 40, 0, "Field of View", Tab_Misc, Misc_Fov);
//			AddNewButton(Tab_Misc, Misc_AutoJump, false, 215, 60, "Automatic bunnyhop");
//			AddNewButton(Tab_Misc, Misc_AutoStrafe, false, 215, 80, "Automatic strafe");
//			AddNewDropbox(235, 95, Tab_Misc, Misc_AutoStraferMode, "", 5, AutoStraferMode);
//
//			AddNewButton(Tab_Misc, Misc_ZStrafe, false, 215, 145, "Z-Strafe");
//			AddNewKeyButton(390, 145, Tab_Misc, Misc_ZStrafeKey, "");
//			AddNewButton(Tab_Misc, Misc_CircleStrafe, false, 215, 185, "Pre-speed");
//			AddNewKeyButton(390, 185, Tab_Misc, Misc_CircleStrafeKey, "");
//			AddNewButton(Tab_Misc, Misc_AutoAccept, true, 215, 205, "Auto Accept");
//			AddNewButton(Tab_Misc, Misc_KnifeBot, false, 215, 245, "Knife bot");
//		}
//		AddNewMenuBox("Settings", 450, 20, 235, 160, Tab_Misc);
//		{
//			//Controls
//			AddNewButton(Tab_Misc, Misc_AntiUntrusted, true, 470, 40, "Anti-untrusted");
//		}
//		/* Visual */
//		AddNewMenuBox("Player ESP", 195, 20, 235, 285, Tab_Visuals);
//		{
//			//Dropbox Config
//			//std::vector<char*> VisBox = { "Frame" ,"Frame", "Alt" };
//
//			//Controls
//			AddNewButton(Tab_Visuals, Visuals_BoxEnabled, false, 215, 40, "Bounding Box");
//			AddNewButton(Tab_Visuals, Visuals_HealthBar, false, 215, 80, "Health Bar");
//			AddNewButton(Tab_Visuals, Visuals_ArmorBar, false, 215, 100, "Show ammo");
//			AddNewButton(Tab_Visuals, Visuals_Name, false, 215, 140, "Show name");
//			AddNewButton(Tab_Visuals, Visuals_Weapons, false, 215, 160, "Draw weapons");
//			AddNewButton(Tab_Visuals, Visuals_Flashed, false, 215, 180, "Flashed");
//			AddNewButton(Tab_Visuals, Visuals_AimLines, false, 215, 200, "Aim Lines");
//			AddNewButton(Tab_Visuals, Visuals_Skeltal, false, 215, 220, "Skeleton");
//			AddNewButton(Tab_Visuals, Visuals_Hostage, false, 215, 240, "Hostage");
//			AddNewButton(Tab_Visuals, Visuals_Flags, false, 215, 260, "Flags");
//			AddNewButton(Tab_Visuals, Visuals_XQZ, false, 215, 120, "Visible only");
//			AddNewButton(Tab_Visuals, Visuals_EspTeam, false, 215, 60, "Show teammates");
//		}
//		AddNewMenuBox("Chams", 195, 325, 235, 150, Tab_Visuals);
//		{
//			//Controls
//			AddNewButton(Tab_Visuals, Visuals_Chams, false, 215, 350, "Chams");
//			AddNewButton(Tab_Visuals, Visuals_ChamsXQZ, false, 215, 370, "Visible only");
//			AddNewButton(Tab_Visuals, Visuals_ChamsGuns, false, 215, 390, "Dropped weapons (chams)");
//			AddNewButton(Tab_Visuals, Visuals_ChamsTeam, false, 215, 410, "Show teammates (chams)");
//		}
//
//		AddNewMenuBox("Other ESP", 450, 20, 235, 180, Tab_Visuals);
//		{
//			//Controls
//			AddNewButton(Tab_Visuals, Visuals_DroppedWeapons, false, 470, 40, "Dropped weapons");
//			AddNewButton(Tab_Visuals, Visuals_Crosshair, false, 470, 60, "Crosshair");
//			AddNewButton(Tab_Visuals, Visuals_CrosshairDynamic, false, 470, 80, "Crosshair Dynamic");
//			AddNewButton(Tab_Visuals, Visuals_C4, false, 470, 100, "Bomb");
//			AddNewButton(Tab_Visuals, Visuals_EngineRadar, false, 470, 120, "Engine radar");
//		}
//
//		AddNewMenuBox("Effects", 450, 220, 235, 255, Tab_Visuals);
//		{
//			//Controls
//			AddNewButton(Tab_Visuals, Visuals_NoRecoil, false, 470, 240, "Remove visual recoil");
//			AddNewButton(Tab_Visuals, Visuals_NoFlash, false, 470, 260, "Remove flash");
//			AddNewButton(Tab_Visuals, Visuals_NoSmoke, false, 470, 280, "Remove smoke");
//			AddNewButton(Tab_Visuals, Visuals_NoScope, false, 470, 310, "Remove scope");
//			std::vector< char* > tptype{
//				"Off",
//				"Off",
//				"Fake",
//				"Real",
//				"Both",
//			};
//			AddNewDropbox(470, 380, Tab_Visuals, Visuals_ThirdPerson, "Force third-person", 5, tptype);
//		}
//
//		/* Config */
//
//		//Dropbox Config
//		std::vector< char* > ConfigType = { "Default", "Default", "Legit","HvH", "Rage", "Casual", "Custom1", "Custom2" };
//
//		//Controls
//		AddNewDropbox2(10, 470, Config_type, " ", 8, ConfigType);
//		AddNewFuncButton2(10, 495, (DWORD)LoadSettings, "Load");
//		AddNewFuncButton2(10, 520, (DWORD)SaveSettings, "Save");
//		//AddNewFuncButton(195, 185, (DWORD)ResetSettings, "Reset", Tab_Config);
//		AddNewFuncButton2(10, 545, (DWORD)ForceUpdate, "Update");
//		AddNewFuncButton2(10, 570, (DWORD)Unhook, "Un-load");
//
//
//		/* Changer */
//		AddNewMenuBox("SkyBox Changer", 195, 20, 235, 70, Tab_Changer);
//		{
//			std::vector< char* > sky = { "Disable", "Disable", "BAGGAGE","TIBET","EMBASSY","ITALY","JUNGLE","NUKEBLANK","OFFICE","DAYLIGHT01","DAYLIGHT02","DAYLIGHT03","DAYLIGHT04","CLOUDY01","NIGHT02","NIGHT02B","DUST","VENICE","VERTIGO","VIETNAM" };
//
//			//AddNewButton(Tab_Changer, Changer_bSkyName, false, 215, 130, "Enable SkyBox Name");
//			AddNewDropbox(215, 40, Tab_Changer, Changer_SkyName, " ", 18, sky);
//			AddNewButton(Tab_Changer, Changer_NoSky, false, 215, 70, "No Sky");
//		}
//
//		AddNewMenuBox("World", 450, 20, 235, 60, Tab_Changer);
//		{
//			//Dropbox Config
//			std::vector< char* > world = { "Disable", "Disable", "NightMode" };
//
//			//Controls
//			AddNewDropbox(490, 40, Tab_Changer, Changer_World, " ", 3, world);
//		}
//
//		
//	}
//}
//
//void DrawMenu()
//{
//	unsigned int Build = Interfaces.pEngine->GetEngineBuildNumber();
//	unsigned int Build2 = Interfaces.pEngine->GetEngineBuildNumber();
//	static bool firsttime = true;
//	static float rainbow;
//	int x, y, rx, ry, ox, oy, width, height, tab, tick;
//	width = height = 706;
//	Dont_Click = false;
//
//	/* UPDATE KEYS */
//	std::copy(keys, keys + 255, oldKeys);
//	for (int x = 0; x < 255; x++)
//	{
//		keys[x] = (GetAsyncKeyState(x));
//	}
//
//	static int Clock = 0;
//	if (GlobalMenuAlpha == 0 || GlobalMenuAlpha == 255)
//	{
//		if (GetKeyPress(VK_INSERT))
//		{
//			Clock = 0;
//			Menu.Opened = !Menu.Opened;
//		}
//	}
//
//	double timeSoFar1 = (double)Clock / 70.f;
//	Clock++;
//
//	if (Menu.Opened)
//	{
//		if (GlobalMenuAlpha != 255)
//			if (GlobalMenuAlpha > 255)
//				GlobalMenuAlpha = 255;
//			else
//				GlobalMenuAlpha = timeSoFar1 * 255.f;
//	}
//	else if (GlobalMenuAlpha != 0)
//	{
//		if (GlobalMenuAlpha < 0)
//			GlobalMenuAlpha = 0;
//		else
//			GlobalMenuAlpha = 255 - (timeSoFar1 * 255.f);
//	}
//
//	if (firsttime)
//	{
//		if (Build == Build2)
//		{
//			SetupMenu();
//			firsttime = false;
//		}
//		else
//		{
//			Unhook();
//		}
//	}
//
//	if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
//	{
//		Holding_Mouse_1 = false;
//	}
//	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
//	{
//		Clicked_This_Frame = true;
//	}
//	else
//	{
//		Clicked_This_Frame = false;
//	}
//
//	Menu.Update_Frame();
//
//	//Drawing the menu base
//	Interfaces.pSurface->DrawSetColor(5, 5, 5, GlobalMenuAlpha);
//	Interfaces.pSurface->DrawFilledRect(Menu.pos.x - 2, Menu.pos.y - 2, Menu.pos.x + 707, Menu.pos.y + 607);
//	Interfaces.pSurface->DrawSetColor(75, 75, 75, GlobalMenuAlpha);
//	Interfaces.pSurface->DrawOutlinedRect(Menu.pos.x - 1, Menu.pos.y - 1, Menu.pos.x + 706, Menu.pos.y + 606);
//	Interfaces.pSurface->DrawSetColor(22.5, 22.5, 22.5, GlobalMenuAlpha);
//	Interfaces.pSurface->DrawFilledRect(Menu.pos.x, Menu.pos.y, Menu.pos.x + 175, Menu.pos.y + 605);
//	Interfaces.pSurface->DrawSetColor(25, 25, 25, GlobalMenuAlpha);
//	Interfaces.pSurface->DrawFilledRect(Menu.pos.x + 176, Menu.pos.y, Menu.pos.x + 705, Menu.pos.y + 605);
//	Interfaces.pSurface->DrawSetColor(75, 75, 75, GlobalMenuAlpha);
//	Interfaces.pSurface->DrawLine(Menu.pos.x + 174, Menu.pos.y, Menu.pos.x + 174, Menu.pos.y + 605);
//	Interfaces.pSurface->DrawLine(Menu.pos.x + 10, Menu.pos.y + 140, Menu.pos.x + 165, Menu.pos.y + 140);
//	Interfaces.pSurface->DrawLine(Menu.pos.x + 10, Menu.pos.y + 460, Menu.pos.x + 165, Menu.pos.y + 460);
//	Interfaces.pSurface->DrawSetColor(5, 5, 5, GlobalMenuAlpha);
//	Interfaces.pSurface->DrawLine(Menu.pos.x + 10, Menu.pos.y + 141, Menu.pos.x + 165, Menu.pos.y + 141);
//	Interfaces.pSurface->DrawString(Hacks.Font_Logo, Menu.pos.x + 25, Menu.pos.y + 50, 0, CColor(255, 255, 255, GlobalMenuAlpha), L"");
//
//	for (CTab& Tab : Tabs)
//		Tab.Draw();
//
//	for (CMenuBox& MenuBox : MenuBoxs)
//		MenuBox.Draw();
//
//	for (CFunctionButton& FuncButton : FuncButtons)
//		FuncButton.Draw();
//
//	for (CFunctionButton2& FuncButton2 : FuncButtons2)
//		FuncButton2.Draw();
//
//	for (CKeyButton& KeyButton : KeyButtons)
//		KeyButton.Draw();
//
//	for (CSlider& slider : Sliders)
//		slider.Draw();
//
//	for (CButton& Button : Buttons)
//		Button.Draw();
//
//	std::reverse(Dropboxes.begin(), Dropboxes.end());
//
//	for (CDropBox& Dropbox : Dropboxes)
//		Dropbox.Draw();
//
//	for (CDropBox2& Dropbox2 : Dropboxes2)
//		Dropbox2.Draw();
//
//	std::reverse(Dropboxes.begin(), Dropboxes.end());
//
//	if (Menu.Opened)
//	{
//		DrawMouse();
//		for (CTab& Tab : Tabs)
//			Tab.Update();
//
//		for (CMenuBox& MenuBox : MenuBoxs)
//			MenuBox.Update();
//
//		for (CDropBox& Dropbox : Dropboxes)
//			Dropbox.Update();
//
//		for (CDropBox2& Dropbox2 : Dropboxes2)
//			Dropbox2.Update();
//
//		for (CSlider& slider : Sliders)
//			slider.Update();
//
//		for (CButton& Button : Buttons)
//			Button.Update();
//
//		for (CKeyButton& KeyButton : KeyButtons)
//			KeyButton.Update();
//
//		for (CFunctionButton2& FuncButton2 : FuncButtons2)
//			FuncButton2.Update();
//	}
//	if (Clicked_This_Frame)
//	{
//		Holding_Mouse_1 = true;
//	}
//}
