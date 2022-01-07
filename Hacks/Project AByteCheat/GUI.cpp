#include "GUI.h"
#include "Menu.h"
#include "RenderManager.h"
#include <algorithm>
#include "tinyxml2.h"
#include <sstream>
#include "Controls.h"
float MenuAlpha = 0.05f;
float MenuAlpha3 = 0.05f;
float MenuAlpha5 = 0.05f;
float MenuAlpha7 = 0.05f;
float MenuAlpha9 = 0.05f;
float MenuAlpha11 = 0.05f;
float MenuAlpha13 = 0.05f;
float MenuAlpha15 = 0.05f;
float MenuAlpha17 = 0.05f;
float MenuAlpha19 = 0.05f;
float MenuAlpha21 = 0.05f;
float MenuAlpha23 = 0.05f;
float Globals::MenuAlpha24 = MenuAlpha23;
float Globals::MenuAlpha22 = MenuAlpha21;
float Globals::MenuAlpha20 = MenuAlpha19;
float Globals::MenuAlpha18 = MenuAlpha17;
float Globals::MenuAlpha16 = MenuAlpha15;
float Globals::MenuAlpha14 = MenuAlpha13;
float Globals::MenuAlpha12 = MenuAlpha11;
float Globals::MenuAlpha10 = MenuAlpha9;
float Globals::MenuAlpha8 = MenuAlpha7;
float Globals::MenuAlpha6 = MenuAlpha5;
float Globals::MenuAlpha2 = MenuAlpha;
float Globals::MenuAlpha4 = MenuAlpha3;
CGUI GUI;
bool SaveFile = false;
bool LoadFile = false;
CGUI::CGUI()
{
}
#define UI_CURSORSIZE       12
#define UI_CURSORFILL       Color(255,255,255)
#define UI_CURSOROUTLINE    Color(20,20,20,255)
#define UI_WIN_TOPHEIGHT	26
#define UI_WIN_TITLEHEIGHT	0
#define UI_TAB_WIDTH		150
#define UI_TAB_HEIGHT		32
#define UI_WIN_CLOSE_X		20
#define UI_WIN_CLOSE_Y      6
#define UI_CHK_SIZE			16
#define UI_COL_MAIN			Color(27, 220, 117, 255)
#define UI_COL_SHADOW		Color(0, 0, 0, 255)
#define COL_WHITE           Color(255, 100, 50, 255)
#define UI_COL_MAINE        Color(0, 204, 0, 255)
#define UI_COL_MAINDARK     Color(113, 236, 159, 255)
#define UI_COURSOUR			Color(255, 255, 255, 255)
#define UI_COL_FADEMAIN     Color(27, 206, 94, 255)
#define UI_COL_SHADOW		Color(0, 0, 0, 255)
#define UI_COL_CLIENTBACK   Color(238, 0, 50, 255)
#define UI_COL_TABSEPERATOR Color(229, 229, 229, 255)
#define UI_COL_TABTEXT      Color(255, 255, 255, 255)
#define UI_COL_GROUPOUTLINE Color(222, 100, 150, 255)
void CGUI::Draw()
{
	bool ShouldDrawCursor = false;
	for (auto window : Windows)
	{
		if (window->m_bIsOpen)
			MenuAlpha = min(MenuAlpha + 15, 255);
		else
			MenuAlpha = max(MenuAlpha - 15, 0);
		if (window->m_bIsOpen)
			MenuAlpha5 = min(MenuAlpha5 + 15, 120);
		else
			MenuAlpha5 = max(MenuAlpha5 - 15, 0);
		if (window->m_bIsOpen)
			MenuAlpha3 = min(MenuAlpha3 + 15, 15);
		else
			MenuAlpha3 = max(MenuAlpha3 - 15, 0);
		if (window->m_bIsOpen)
			MenuAlpha7 = min(MenuAlpha7 + 15, 200);
		else
			MenuAlpha7 = max(MenuAlpha7 - 15, 0);
		if (window->m_bIsOpen)
			MenuAlpha9 = min(MenuAlpha9 + 15, 245);
		else
			MenuAlpha9 = max(MenuAlpha9 - 15, 0);
		if (window->m_bIsOpen)
			MenuAlpha11 = min(MenuAlpha11 + 15, 80);
		else
			MenuAlpha11 = max(MenuAlpha11 - 15, 0);
		if (window->m_bIsOpen)
			MenuAlpha13 = min(MenuAlpha13 + 15, 140);
		else
			MenuAlpha13 = max(MenuAlpha13 - 15, 0);
		if (window->m_bIsOpen)
			MenuAlpha15 = min(MenuAlpha15 + 15, 40);
		else
			MenuAlpha15 = max(MenuAlpha15 - 15, 0);
		if (window->m_bIsOpen)
			MenuAlpha17 = min(MenuAlpha17 + 15, 50);
		else
			MenuAlpha17 = max(MenuAlpha17 - 15, 0);
		if (window->m_bIsOpen)
			MenuAlpha21 = min(MenuAlpha21 + 15, 1);
		else
			MenuAlpha21 = max(MenuAlpha21 - 15, 0);
		if (window->m_bIsOpen)
			MenuAlpha19 = min(MenuAlpha19 + 15, 100);
		else
			MenuAlpha19 = max(MenuAlpha19 - 15, 0);
		if (window->m_bIsOpen)
			MenuAlpha23 = min(MenuAlpha23 + 15, 255);
		else
			MenuAlpha23 = max(MenuAlpha23 - 15, 0);
		if (window->m_bIsOpen)
		{
			ShouldDrawCursor = true;
			DrawWindow(window);
		}
	}

}
void CGUI::Update()
{
	//Key Array
	std::copy(keys, keys + 255, oldKeys);
	for (int x = 0; x < 255; x++)
	{
		//oldKeys[x] = oldKeys[x] & keys[x];
		keys[x] = (GetAsyncKeyState(x));
	}
	// Mouse Location
	POINT mp; GetCursorPos(&mp);
	Mouse.x = mp.x; Mouse.y = mp.y;
	RECT Screen = Render::GetViewport();
	// Window Binds
	for (auto& bind : WindowBinds)
	{
		if (GetKeyPress(bind.first))
		{
			bind.second->Toggle();
		}
	}
	// Stop dragging
	if (IsDraggingWindow && !GetKeyState(VK_LBUTTON))
	{
		IsDraggingWindow = false;
		DraggingWindow = nullptr;
	}
	// If we are in the proccess of dragging a window
	if (IsDraggingWindow && GetKeyState(VK_LBUTTON) && !GetKeyPress(VK_LBUTTON))
	{
		if (DraggingWindow)
		{
			DraggingWindow->m_x = Mouse.x - DragOffsetX;
			DraggingWindow->m_y = Mouse.y - DragOffsetY;
		}
	}
	int w, h;
	int centerW, centerh;
	interfaces::engine->GetScreenSize(w, h);
	centerW = w / 2;
	centerh = h / 2;
	// Process some windows
	for (auto window : Windows)
	{
		if (window->m_bIsOpen)
		{
			// Used to tell the widget processing that there could be a click
			bool bCheckWidgetClicks = false;

			 //if (options::menu.MiscTab.watermark.GetState())
			//{
			//	Render::Text((centerW * 2) - 95, 25, Color(250, 250, 250, (MenuAlpha - 1)), Render::Fonts::esp, "Mirror v5 Remastered");
			//}

			Render::Clear(0, 0, w * 4, h * 4, Color(10, 10, 10, (MenuAlpha / 1.15)));
			// If the user clicks inside the window
			if (GetKeyPress(VK_LBUTTON))
			{
				if (IsMouseInRegion(window->m_x, window->m_y, window->m_x + window->m_iWidth, window->m_y + window->m_iHeight))
				{
					// Is it inside the client area?
					if (IsMouseInRegion(window->GetClientArea()))
					{
						// User is selecting a new tab
						if (IsMouseInRegion(window->GetTabArea()))
						{
							// Loose focus on the control
							window->IsFocusingControl = false;
							window->FocusedControl = nullptr;
							int iTab = 0;
							int TabCount = window->Tabs.size();
							if (TabCount) // If there are some tabs
							{
								int TabSize = (window->m_iWidth - 4 - 12) / TabCount;
								int Dist = Mouse.x - (window->m_x + 8);
								while (Dist > TabSize)
								{
									if (Dist > TabSize)
									{
										iTab++;
										Dist -= TabSize;
									}
								}
								window->SelectedTab = window->Tabs[iTab];
							}
						}
						else
							bCheckWidgetClicks = true;
					}
					else
					{
						// Must be in the around the title or side of the window
						// So we assume the user is trying to drag the window
						IsDraggingWindow = true;
						DraggingWindow = window;
						DragOffsetX = Mouse.x - window->m_x;
						DragOffsetY = Mouse.y - window->m_y;
						// Loose focus on the control
						window->IsFocusingControl = false;
						window->FocusedControl = nullptr;
					}
				}
				else
				{
					// Loose focus on the control
					window->IsFocusingControl = false;
					window->FocusedControl = nullptr;
				}
			}
			// Controls 
			if (window->SelectedTab != nullptr)
			{
				// Focused widget
				bool SkipWidget = false;
				CControl* SkipMe = nullptr;
				// this window is focusing on a widget??
				if (window->IsFocusingControl)
				{
					if (window->FocusedControl != nullptr)
					{
						CControl* control = window->FocusedControl;
						CGroupBox* group;
						if (control->FileControlType != UIControlTypes::UIC_GroupBox) group = control->parent_group ? (CGroupBox*)control->parent_group : nullptr;
						if (group != nullptr && control->FileControlType != UIControlTypes::UIC_GroupBox)
						{
							if ((group->group_tabs.size() > 0 && control->g_tab == group->selected_tab) || group->group_tabs.size() == 0)
							{
								// We've processed it once, skip it later
								SkipWidget = true;
								SkipMe = window->FocusedControl;
								POINT cAbs = window->FocusedControl->GetAbsolutePos();
								RECT controlRect = { cAbs.x, cAbs.y, window->FocusedControl->m_iWidth, window->FocusedControl->m_iHeight };
								window->FocusedControl->OnUpdate();
								if (window->FocusedControl->Flag(UIFlags::UI_Clickable) && IsMouseInRegion(controlRect) && bCheckWidgetClicks)
								{
									window->FocusedControl->OnClick();
									bCheckWidgetClicks = false;
								}
							}
						}
						else if (control->FileControlType == UIControlTypes::UIC_GroupBox || control->FileControlType != UIControlTypes::UIC_GroupBox && !control->parent_group)
						{
							// We've processed it once, skip it later
							SkipWidget = true;
							SkipMe = window->FocusedControl;
							POINT cAbs = window->FocusedControl->GetAbsolutePos();
							RECT controlRect = { cAbs.x, cAbs.y, window->FocusedControl->m_iWidth, window->FocusedControl->m_iHeight };
							window->FocusedControl->OnUpdate();
							if (window->FocusedControl->Flag(UIFlags::UI_Clickable) && IsMouseInRegion(controlRect) && bCheckWidgetClicks)
							{
								window->FocusedControl->OnClick();
								// If it gets clicked we loose focus
								window->IsFocusingControl = false;
								window->FocusedControl = nullptr;
								bCheckWidgetClicks = false;
							}
						}
					}
				}
				// Itterate over the rest of the control
				for (auto control : window->SelectedTab->Controls)
				{
					if (control != nullptr)
					{
						CGroupBox* group;
						if (control->FileControlType != UIControlTypes::UIC_GroupBox) group = control->parent_group ? (CGroupBox*)control->parent_group : nullptr;
						if (group != nullptr && control->FileControlType != UIControlTypes::UIC_GroupBox)
						{
							if (group->group_tabs.size() > 0 && control->g_tab == group->selected_tab || group->group_tabs.size() == 0)
							{
								if (SkipWidget && SkipMe == control)
									continue;
								POINT cAbs = control->GetAbsolutePos();
								RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
								control->OnUpdate();
								if (control->Flag(UIFlags::UI_Clickable) && IsMouseInRegion(controlRect) && bCheckWidgetClicks)
								{
									control->OnClick();
									bCheckWidgetClicks = false;
									// Change of focus
									if (control->Flag(UIFlags::UI_Focusable))
									{
										window->IsFocusingControl = true;
										window->FocusedControl = control;
									}
									else
									{
										window->IsFocusingControl = false;
										window->FocusedControl = nullptr;
									}
								}
							}
						}
						else if (control->FileControlType == UIControlTypes::UIC_GroupBox || control->FileControlType != UIControlTypes::UIC_GroupBox && !control->parent_group)
						{
							if (SkipWidget && SkipMe == control)
								continue;
							POINT cAbs = control->GetAbsolutePos();
							RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
							control->OnUpdate();
							if (control->Flag(UIFlags::UI_Clickable) && IsMouseInRegion(controlRect) && bCheckWidgetClicks)
							{
								control->OnClick();
								bCheckWidgetClicks = false;
								// Change of focus
								if (control->Flag(UIFlags::UI_Focusable))
								{
									window->IsFocusingControl = true;
									window->FocusedControl = control;
								}
								else
								{
									window->IsFocusingControl = false;
									window->FocusedControl = nullptr;
								}
							}
						}
					}
				}
				// We must have clicked whitespace
				if (bCheckWidgetClicks)
				{
					// Loose focus on the control
					window->IsFocusingControl = false;
					window->FocusedControl = nullptr;
				}
			}
		}
		else
		{
				//if (options::menu.MiscTab.watermark.GetState())
				//{
				//	Render::Text((centerW)-790, (centerh * 2) - 900, Color(255, 255, 255, (MenuAlpha - 3)), Render::Fonts::esp, "turbulence.cc [beta]");
				//}
		}//
	}
}
bool CGUI::GetKeyPress(unsigned int key)
{
	if (keys[key] == true && oldKeys[key] == false)
		return true;
	else
		return false;
}
bool CGUI::GetKeyState(unsigned int key)
{
	return keys[key];
}
bool CGUI::IsMouseInRegion(int x, int y, int x2, int y2)
{
	if (Mouse.x > x && Mouse.y > y && Mouse.x < x2 && Mouse.y < y2)
		return true;
	else
		return false;
}
bool CGUI::IsMouseInRegion(RECT region)
{
	return IsMouseInRegion(region.left, region.top, region.left + region.right, region.top + region.bottom);
}
POINT CGUI::GetMouse()
{
	return Mouse;
}
Color getRainbow(int speed, int offset)
{
	float hue = (float)((GetCurrentTime() + offset) % speed);
	hue /= speed;
	std::stringstream fuckoff;
	return Color::FromHSB(hue, 1.0F, 1.0F);
}

/*static const unsigned char pic[] = { 15, 15, 15, 255, 56, 56, 56, 255, 15, 15, 15, 255, 56, 56, 56, 255, 15, 15, 15, 255, 56, 56, 56, 255, 56, 56, 56, 255, 56, 56, 56, 255, 15, 15, 15, 255, 56, 56, 56, 255, 15, 15, 15, 255, 56, 56, 56, 255, 56, 56, 56, 255, 56, 56, 56, 255, 15, 15, 15, 255, 56, 56, 56, 255, 15, 15, 15, 255, 56, 56, 56, 255, 15, 15, 15, 255, 56, 56, 56, 255, 15, 15, 15, 255, 56, 56, 56, 255, 56, 56, 56, 255, 56, 56, 56, 255, 15, 15, 15, 255, 56, 56, 56, 255, 15, 15, 15, 255, 56, 56, 56, 255, 56, 56, 56, 255, 56, 56, 56, 255, 15, 15, 15, 255, 56, 56, 56, 255 };
struct textures_s
{
	int Texture1;
} textures;

void GenerateTextures() //call this once
{
	textures.Texture1 = Interfaces::Surface->CreateNewTextureID(true);
	Interfaces::Surface->DrawSetTextureRGBA(textures.Texture1, pic, 4, 8);
}*/

bool CGUI::DrawWindow(CWindow* window)
{
	//	Interfaces::Surface->DrawSetColor(255, 255, 255, 255);
	//	Interfaces::Surface->DrawSetTexture(textures.Texture1);
	//	Interfaces::Surface->DrawTexturedRect(0, 1000, 1000, 0);
	RECT TextSize = Render::GetTextSize(Render::Fonts::Menu, window->Title.c_str());
	int TextX = window->m_x + (window->m_iWidth / 2) - (TextSize.left / 2);
	Render::Clear(window->m_x + 8 - 90, window->m_y + 1 + 28 * 2, window->m_iWidth - 4 - 12 + 90, window->m_iHeight - 2 - 8 - 26, Color(28, 28, 28, MenuAlpha));
	Render::Clear(window->m_x + 8 - 90, window->m_y + 1 + 28 * 2 - 6, window->m_iWidth - 4 - 12 + 90, 6, Color(40, 40, 40, MenuAlpha));
	Render::Clear(window->m_x + 8 - 90, window->m_y + 1 + 28 * 2 + window->m_iHeight - 2 - 8 - 26, window->m_iWidth - 4 - 12 + 90, 6, Color(40, 40, 40, MenuAlpha));
	Render::Clear(window->m_x + 8 - 90 - 6, window->m_y + 1 + 28 * 2 - 6, 6, window->m_iHeight - 2 - 8 - 26 + 12, Color(40, 40, 40, MenuAlpha));
	Render::Clear(window->m_x + 8 + window->m_iWidth - 4 - 12, window->m_y + 1 + 28 * 2 - 6, 6, window->m_iHeight - 2 - 8 - 26 + 12, Color(40, 40, 40, MenuAlpha));

	//Tab
	Render::Clear(window->m_x + 8 - 90, window->m_y + 1 + 28 * 2, 90, window->m_iHeight - 2 - 8 - 26, Color(21, 21, 19, MenuAlpha));

	/* Menu outline - done, needs fix - make center thinner by 1px (4->3) */
	Render::Outline(window->m_x + 8 - 6 - 90, window->m_y + 1 + 28 * 2 - 6, window->m_iWidth - 4 - 12 + 90 + 12, window->m_iHeight - 2 - 8 - 26 + 12, Color(60, 60, 60, MenuAlpha));
	Render::Outline(window->m_x + 8 - 5 - 90, window->m_y + 1 + 28 * 2 - 5, window->m_iWidth - 4 - 12 + 90 + 10, window->m_iHeight - 2 - 8 - 26 + 10, Color(60, 60, 60, MenuAlpha));
	Render::Outline(window->m_x + 8 - 90, window->m_y + 1 + 28 * 2, window->m_iWidth - 4 - 12 + 90, window->m_iHeight - 2 - 8 - 26, Color(60, 60, 60, MenuAlpha));
	Render::Outline(window->m_x + 8 - 6 - 90, window->m_y + 1 + 28 * 2 - 6, window->m_iWidth - 4 - 12 + 90 + 12, window->m_iHeight - 2 - 8 - 26 + 12, Color(10, 10, 10, MenuAlpha));


	/* ??? */
	//Render::Clear(window->m_x + 8, window->m_y + 1 + 28 * 2, 1, window->m_iHeight - 2 - 8 - 26 - 1, Color(129, 129, 129, 255));

	//Tab
	int tabcount = window->Tabs.size();
	if (tabcount) // If there are some tabs
	{
		bool isOut = true;

		for (int i = 0; i < tabcount; i++)
		{
			CTab *tab = window->Tabs[i];

			float xAxis;
			float yAxis;

			/* Space from up */
			float yWinPos = window->m_y + 1 + 28 * 2;


			float yWinHeight = (window->m_iHeight) - 2 - 8 - 26;

			float intercept = (yWinHeight - 40) / tabcount;
			int factor = i;

			yAxis = yWinPos + 16 + (factor * intercept) - 10 + 20;

			RECT TabDrawArea = { window->m_x + 8 - 90 + 1, yAxis - 5, 90 - 1, intercept };

			RECT TextSize;
			TextSize = Render::GetTextSize(Render::Fonts::Tabs, tab->Title.c_str());

			RECT ClickTabArea = { xAxis,
				yAxis,
				TextSize.right,
				TextSize.bottom };

			if (GetAsyncKeyState(VK_LBUTTON))
			{
				if (IsMouseInRegion(TabDrawArea))
				{
					window->SelectedTab = window->Tabs[i];
				}
			}

			if (window->SelectedTab == tab)
			{
				xAxis = window->m_x + 8 - (45 + TextSize.right / 2);
				Render::Clear(TabDrawArea.left, TabDrawArea.top, TabDrawArea.right, TabDrawArea.bottom, Color(28, 28, 28, MenuAlpha));
				Render::Line(TabDrawArea.left, TabDrawArea.top, TabDrawArea.left + TabDrawArea.right, TabDrawArea.top, Color(48, 48, 48, MenuAlpha));
				Render::Line(TabDrawArea.left, TabDrawArea.top + TabDrawArea.bottom, TabDrawArea.left + TabDrawArea.right, TabDrawArea.top + TabDrawArea.bottom, Color(48, 48, 48, MenuAlpha));
				Render::Text(TabDrawArea.left + (TabDrawArea.right / 2) - (TextSize.right / 2), TabDrawArea.top + (TabDrawArea.bottom / 2) - (TextSize.bottom / 2), Color(210, 210, 210, MenuAlpha), Render::Fonts::Tabs, tab->Title.c_str());
			}
			else {
				xAxis = window->m_x + 8 - (45 + TextSize.right / 2);
				Render::Text(TabDrawArea.left + (TabDrawArea.right / 2) - (TextSize.right / 2), TabDrawArea.top + (TabDrawArea.bottom / 2) - (TextSize.bottom / 2), Color(90, 90, 90, MenuAlpha), Render::Fonts::Tabs, tab->Title.c_str());
			}

			//Render::Clear(window->m_x + 8, window->m_y + 1 + 27, window->m_iWidth - 4 - 12, 2, Color(65, 55, 55, 255));
		}
	}


	static float rainbow;

	/* Gradient bar (need to fix second bar alpha being lower so the background is darker */
	//fix background
	Render::GradientSideways(window->m_x - 81, window->m_y + 58, window->m_iWidth / 2 + 66, 1, Color(20, 20, 20, MenuAlpha), Color(20, 20, 20, MenuAlpha), 1);
	Render::GradientSideways((window->m_x + window->m_iWidth / 2) - 81, window->m_y + 58, window->m_iWidth / 2 + 72, 1, Color(20, 20, 20, MenuAlpha), Color(20, 20, 20, MenuAlpha), 1);
	Render::GradientSideways(window->m_x - 81, window->m_y + 59, window->m_iWidth / 2 + 66, 1, Color(20, 20, 20, MenuAlpha), Color(20, 20, 20, MenuAlpha), 1);
	Render::GradientSideways((window->m_x + window->m_iWidth / 2) - 81, window->m_y + 59, window->m_iWidth / 2 + 72, 1, Color(20, 20, 20, MenuAlpha), Color(20, 20, 20, MenuAlpha), 1);
	Render::GradientSideways(window->m_x - 81, window->m_y + 60, window->m_iWidth / 2 + 66, 1, Color(20, 20, 20, MenuAlpha), Color(20, 20, 20, MenuAlpha), 1);
	Render::GradientSideways((window->m_x + window->m_iWidth / 2) - 81, window->m_y + 60, window->m_iWidth / 2 + 72, 1, Color(20, 20, 20, MenuAlpha), Color(20, 20, 20, MenuAlpha), 1);

	//1st
	//bg
	Render::GradientSideways(window->m_x - 80, window->m_y + 59, window->m_iWidth / 2 + 34, 1, Color(100, 100, 100, MenuAlpha), Color(100, 100, 100, MenuAlpha), 1);
	Render::GradientSideways((window->m_x + window->m_iWidth / 2) - 45, window->m_y + 59, window->m_iWidth / 2 + 35, 1, Color(100, 100, 100, MenuAlpha), Color(100, 100, 100, MenuAlpha), 1);

	//line
	Render::GradientSideways(window->m_x - 80, window->m_y + 59, window->m_iWidth / 2 + 34, 1, Color(0, 160, 255, MenuAlpha - 100), Color(160, 0, 255, MenuAlpha - 100), 1);
	Render::GradientSideways((window->m_x + window->m_iWidth / 2) - 45, window->m_y + 59, window->m_iWidth / 2 + 35, 1, Color(160, 0, 255, MenuAlpha - 100), Color(255, 255, 0, MenuAlpha - 100), 1);

	//2nd
	//bg
	Render::GradientSideways(window->m_x - 80, window->m_y + 60, window->m_iWidth / 2 + 34, 1, Color(0, 0, 0, MenuAlpha), Color(0, 0, 0, MenuAlpha), 1);
	Render::GradientSideways((window->m_x + window->m_iWidth / 2) - 45, window->m_y + 60, window->m_iWidth / 2 + 35, 1, Color(0, 0, 0, MenuAlpha), Color(0, 0, 0, MenuAlpha), 1);

	//line
	Render::GradientSideways(window->m_x - 80, window->m_y + 60, window->m_iWidth / 2 + 34, 1, Color(0, 160, 255, MenuAlpha - 180), Color(160, 0, 255, MenuAlpha - 180), 1);
	Render::GradientSideways((window->m_x + window->m_iWidth / 2) - 45, window->m_y + 60, window->m_iWidth / 2 + 35, 1, Color(160, 0, 255, MenuAlpha - 180), Color(255, 255, 0, MenuAlpha - 180), 1);


	// Controls
	if (window->SelectedTab != nullptr)
	{
		// Focused widget
		bool SkipWidget = false;
		CControl* SkipMe = nullptr;
		// this window is focusing on a widget??
		if (window->IsFocusingControl)
		{
			if (window->FocusedControl != nullptr)
			{
				CControl* control = window->FocusedControl;
				CGroupBox* group;
				if (control->FileControlType != UIControlTypes::UIC_GroupBox) group = control->parent_group ? (CGroupBox*)control->parent_group : nullptr;
				if (group != nullptr && control->FileControlType != UIControlTypes::UIC_GroupBox)
				{
					if (group->group_tabs.size() > 0 && control->g_tab == group->selected_tab || group->group_tabs.size() == 0)
					{
						SkipWidget = true;
						SkipMe = window->FocusedControl;
					}
				}
				else if (control->FileControlType == UIControlTypes::UIC_GroupBox || control->FileControlType != UIControlTypes::UIC_GroupBox && !control->parent_group)
				{
					SkipWidget = true;
					SkipMe = window->FocusedControl;
				}
			}
		}
		// Itterate over all the other controls
		for (auto control : window->SelectedTab->Controls)
		{
			if (SkipWidget && SkipMe == control)
				continue;
			if (control != nullptr && control->Flag(UIFlags::UI_Drawable))
			{
				CGroupBox* group;
				if (control->FileControlType != UIControlTypes::UIC_GroupBox) group = control->parent_group ? (CGroupBox*)control->parent_group : nullptr;
				if (group != nullptr && control->FileControlType != UIControlTypes::UIC_GroupBox)
				{
					if (group->group_tabs.size() > 0 && control->g_tab == group->selected_tab || group->group_tabs.size() == 0)
					{
						POINT cAbs = control->GetAbsolutePos();
						RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
						bool hover = false;
						if (IsMouseInRegion(controlRect))
						{
							hover = true;
						}
						control->Draw(hover);
					}
				}
				else if (control->FileControlType == UIControlTypes::UIC_GroupBox || control->FileControlType != UIControlTypes::UIC_GroupBox && !control->parent_group)
				{
					POINT cAbs = control->GetAbsolutePos();
					RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
					bool hover = false;
					if (IsMouseInRegion(controlRect))
					{
						hover = true;
					}
					control->Draw(hover);
				}
			}
		}
		// Draw the skipped widget last
		if (SkipWidget)
		{
			auto control = window->FocusedControl;
			if (control != nullptr && control->Flag(UIFlags::UI_Drawable))
			{
				CControl* control = window->FocusedControl;
				CGroupBox* group;
				if (control->FileControlType != UIControlTypes::UIC_GroupBox) group = control->parent_group ? (CGroupBox*)control->parent_group : nullptr;
				if (group != nullptr && control->FileControlType != UIControlTypes::UIC_GroupBox)
				{
					if (group->group_tabs.size() > 0 && control->g_tab == group->selected_tab || group->group_tabs.size() == 0)
					{
						POINT cAbs = control->GetAbsolutePos();
						RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
						bool hover = false;
						if (IsMouseInRegion(controlRect))
						{
							hover = true;
						}
						control->Draw(hover);
					}
				}
				else if (control->FileControlType == UIControlTypes::UIC_GroupBox || control->FileControlType != UIControlTypes::UIC_GroupBox && !control->parent_group)
				{
					POINT cAbs = control->GetAbsolutePos();
					RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
					bool hover = false;
					if (IsMouseInRegion(controlRect))
					{
						hover = true;
					}
					control->Draw(hover);
				}
			}
		}
	}
	return true;
}
void CGUI::RegisterWindow(CWindow* window)
{
	Windows.push_back(window);
	// Resorting to put groupboxes at the start
	for (auto tab : window->Tabs)
	{
		for (auto control : tab->Controls)
		{
			if (control->Flag(UIFlags::UI_RenderFirst))
			{
				CControl * c = control;
				tab->Controls.erase(std::remove(tab->Controls.begin(), tab->Controls.end(), control), tab->Controls.end());
				tab->Controls.insert(tab->Controls.begin(), control);
			}
		}
	}
}
void CGUI::BindWindow(unsigned char Key, CWindow* window)
{
	if (window)
		WindowBinds[Key] = window;
	else
		WindowBinds.erase(Key);
}
void CGUI::SaveWindowState(CWindow* window, std::string Filename)
{
	tinyxml2::XMLDocument Doc;
	tinyxml2::XMLElement *Root = Doc.NewElement("ByteCheats");
	Doc.LinkEndChild(Root);
	if (Root && window->Tabs.size() > 0)
	{
		for (auto Tab : window->Tabs)
		{
			tinyxml2::XMLElement *TabElement = Doc.NewElement(Tab->Title.c_str());
			Root->LinkEndChild(TabElement);
			if (TabElement && Tab->Controls.size() > 1)
			{
				for (auto Control : Tab->Controls)
				{
					if (Control && Control->Flag(UIFlags::UI_SaveFile) && Control->FileIdentifier.length() > 1 && Control->FileControlType)
					{
						tinyxml2::XMLElement *ControlElement = Doc.NewElement(Control->FileIdentifier.c_str());
						TabElement->LinkEndChild(ControlElement);
						if (!ControlElement)
						{
							return;
						}
						CCheckBox* cbx = nullptr;
						CComboBox* cbo = nullptr;
						CKeyBind* key = nullptr;
						CSlider* sld = nullptr;
						CListBox* lsbox = nullptr;
						CColorSelector* clse = nullptr;
						switch (Control->FileControlType)
						{
						case UIControlTypes::UIC_CheckBox:
							cbx = (CCheckBox*)Control;
							ControlElement->SetText(cbx->GetState());
							break;
						case UIControlTypes::UIC_ComboBox:
							cbo = (CComboBox*)Control;
							ControlElement->SetText(cbo->GetIndex());
							break;
						case UIControlTypes::UIC_KeyBind:
							key = (CKeyBind*)Control;
							ControlElement->SetText(key->GetKey());
							break;
						case UIControlTypes::UIC_Slider:
							sld = (CSlider*)Control;
							ControlElement->SetText(sld->GetValue());
							break;
						case UIControlTypes::UIC_ListBox:
							lsbox = (CListBox*)Control;
							ControlElement->SetText(lsbox->GetIndex());
							break;
						case UIControlTypes::UIC_ColorSelector:
							clse = (CColorSelector*)Control;
							char buffer[128];
							float r, g, b, a;
							r = clse->GetValue()[0];
							g = clse->GetValue()[1];
							b = clse->GetValue()[2];
							a = clse->GetValue()[3];
							sprintf_s(buffer, "%1.f %1.f %1.f %1.f", r, g, b, a);
							ControlElement->SetText(buffer);
							break;
						}
					}
				}
			}
		}
	}
	if (Doc.SaveFile(Filename.c_str()) != tinyxml2::XML_NO_ERROR)
	{
		MessageBox(NULL, "ERROR 404", "ByteCheats", MB_OK);
	}
}
void CGUI::LoadWindowState(CWindow* window, std::string Filename)
{
	tinyxml2::XMLDocument Doc;
	if (Doc.LoadFile(Filename.c_str()) == tinyxml2::XML_NO_ERROR)
	{
		tinyxml2::XMLElement *Root = Doc.RootElement();
		if (Root)
		{
			if (Root && window->Tabs.size() > 0)
			{
				for (auto Tab : window->Tabs)
				{
					tinyxml2::XMLElement *TabElement = Root->FirstChildElement(Tab->Title.c_str());
					if (TabElement)
					{
						if (TabElement && Tab->Controls.size() > 0)
						{
							for (auto Control : Tab->Controls)
							{
								if (Control && Control->Flag(UIFlags::UI_SaveFile) && Control->FileIdentifier.length() > 1 && Control->FileControlType)
								{
									tinyxml2::XMLElement *ControlElement = TabElement->FirstChildElement(Control->FileIdentifier.c_str());
									if (ControlElement)
									{
										CCheckBox* cbx = nullptr;
										CComboBox* cbo = nullptr;
										CKeyBind* key = nullptr;
										CSlider* sld = nullptr;
										CListBox* lsbox = nullptr;
										CColorSelector* clse = nullptr;
										switch (Control->FileControlType)
										{
										case UIControlTypes::UIC_CheckBox:
											cbx = (CCheckBox*)Control;
											cbx->SetState(ControlElement->GetText()[0] == '1' ? true : false);
											break;
										case UIControlTypes::UIC_ComboBox:
											cbo = (CComboBox*)Control;
											cbo->SelectIndex(atoi(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_KeyBind:
											key = (CKeyBind*)Control;
											key->SetKey(atoi(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_Slider:
											sld = (CSlider*)Control;
											sld->SetValue(atof(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_ListBox:
											lsbox = (CListBox*)Control;
											lsbox->SelectItem(atoi(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_ColorSelector:
											clse = (CColorSelector*)Control;
											int r, g, b, a;
											std::stringstream ss(ControlElement->GetText());
											ss >> r >> g >> b >> a;
											clse->SetColor(r, g, b, a);
											break;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}























































































































































































