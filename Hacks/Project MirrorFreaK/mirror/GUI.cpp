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
	Interfaces::Engine->GetScreenSize(w, h);
	centerW = w / 2;
	centerh = h / 2;
	// Process some windows
	for (auto window : Windows)
	{
		if (window->m_bIsOpen)
		{
			// Used to tell the widget processing that there could be a click
			bool bCheckWidgetClicks = false;

			//--

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
			if (Options::Menu.VisualsTab.watermark.GetState())
			{
				Render::Text((centerW * 2) - 300, 35, Color(150, 10, 230, (MenuAlpha - 1)), Render::Fonts::xd, "Genocide.cc");
			}
		}
	
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
	fuckoff << "All of you are Jewish, FreaK is the queen of Japan, so suck it";
	return Color::FromHSB(0.4F, 1.0F, hue);
}bool CGUI::DrawWindow(CWindow* window)
{
	float cr = Options::Menu.ColorsTab.Menu.GetValue()[0];
	float cg = Options::Menu.ColorsTab.Menu.GetValue()[1];
	float cb = Options::Menu.ColorsTab.Menu.GetValue()[2];

	float outl_r = Options::Menu.ColorsTab.outl_r.GetValue();
	float outl_g = Options::Menu.ColorsTab.outl_g.GetValue();
	float outl_b = Options::Menu.ColorsTab.outl_b.GetValue();

	float inl_r = Options::Menu.ColorsTab.inl_r.GetValue();
	float inl_g = Options::Menu.ColorsTab.inl_g.GetValue();
	float inl_b = Options::Menu.ColorsTab.inl_b.GetValue();

	float inr_r = Options::Menu.ColorsTab.inr_r.GetValue();
	float inr_g = Options::Menu.ColorsTab.inr_g.GetValue();
	float inr_b = Options::Menu.ColorsTab.inr_b.GetValue();

	float outr_r = Options::Menu.ColorsTab.outr_r.GetValue();
	float outr_g = Options::Menu.ColorsTab.outr_g.GetValue();
	float outr_b = Options::Menu.ColorsTab.outr_b.GetValue();
	RECT TextSize = Render::GetTextSize(Render::Fonts::Menu, window->Title.c_str());
	int TextX = window->m_x + (window->m_iWidth / 2) - (TextSize.left / 2);

	Render::Clear(window->m_x, window->m_y, window->m_iWidth, window->m_iHeight, Color(12, 12, 12, MenuAlpha));
	Render::Clear(window->m_x, window->m_y + 2 + 20, window->m_iWidth, 4, Color(33, 33, 33, MenuAlpha));
	
	//Tab
	int TabCount = window->Tabs.size();
	if (TabCount) // If there are some tabs
	{
		int TabSize = (window->m_iWidth - 4 - 12) / TabCount;
		for (int i = 0; i < TabCount; i++)
		{
			RECT TabArea = { window->m_x + 8 + (i*TabSize), window->m_y + 1 + 27, TabSize, 29 };
			CTab *tab = window->Tabs[i];
			Color txtColor = Color(250, 250, 250, (MenuAlpha / 1.1));
			if (window->SelectedTab == tab)
			{
				RECT TextSize = Render::GetTextSize(Render::Fonts::Tabs, tab->Title.c_str());
			//	Render::gradient_verticle(TabArea.left, TabArea.top, TabArea.right, TabArea.bottom + TextSize.bottom - 10, Color(20, 20, 20, MenuAlpha), Color(7, 7, 7, MenuAlpha));
				txtColor = Color(cr, cg, cb, (MenuAlpha / 1.1));
			}
			else if (IsMouseInRegion(TabArea))
			{
				txtColor = Color(210, 210, 210, (MenuAlpha / 1.1));
			}
			RECT TextSize = Render::GetTextSize(Render::Fonts::Tabs, tab->Title.c_str());
			Render::Text(TabArea.left + (TabSize / 2) - (TextSize.right / 2), TabArea.top + 8, txtColor, Render::Fonts::Tabs, tab->Title.c_str());
		}
	}

	Render::Outline(window->m_x - 1, window->m_y - 1, window->m_iWidth + 2, window->m_iHeight + 2, UI_COL_SHADOW);
	Render::Outline(window->m_x - 2, window->m_y - 2, window->m_iWidth + 4, window->m_iHeight + 4, Color(40, 40, 40, MenuAlpha));
	Render::Outline(window->m_x - 3, window->m_y - 3, window->m_iWidth + 6, window->m_iHeight + 6, Color(30, 30, 30, MenuAlpha));
	Render::Outline(window->m_x - 4, window->m_y - 4, window->m_iWidth + 8, window->m_iHeight + 8, Color(30, 30, 30, MenuAlpha));
	Render::Outline(window->m_x - 5, window->m_y - 5, window->m_iWidth + 10, window->m_iHeight + 10, Color(30, 30, 30, MenuAlpha));
	Render::Outline(window->m_x - 6, window->m_y - 6, window->m_iWidth + 12, window->m_iHeight + 12, Color(40, 40, 40, MenuAlpha));
	Render::Outline(window->m_x - 7, window->m_y - 7, window->m_iWidth + 14, window->m_iHeight + 14, Color(20, 20, 20, MenuAlpha));
	static float rainbow;

	if (Options::Menu.ColorsTab.MenuBar.GetIndex() == 0)
	{
		Render::GradientB(window->m_x + 0, window->m_y + 0, (window->m_iWidth - 0) / 2, 2, Color(outl_r * (255 / 255.f), outl_g * (255 / 255.f), outl_b * (255 / 255.f), MenuAlpha), Color(inl_r * (255 / 255.f), inl_g * (255 / 255.f), inl_b * (255 / 255.f), MenuAlpha), 1);
		Render::GradientB(window->m_x + 0 + (window->m_iWidth - 0) / 2, window->m_y + 0, (window->m_iWidth - 0) / 2, 2, Color(inr_r * (255 / 255.f), inr_g * (255 / 255.f), inr_b * (255 / 255.f), MenuAlpha), Color(outr_r * (255 / 255.f), outr_g * (255 / 255.f), outr_b * (255 / 255.f), MenuAlpha), 1);

	}

	if (Options::Menu.ColorsTab.MenuBar.GetIndex() == 1) {
		Render::DrawRectRainbow(window->m_x + 0, window->m_y + 0, (window->m_iWidth - 0), 2, 0.0020f, rainbow, MenuAlpha);
	}

	if (Options::Menu.ColorsTab.MenuBar.GetIndex() == 2) {
		Color uremam1 = getRainbow(23010, 1000);
		Color uremam2 = getRainbow(23010, 2000);
		Color uremam3 = getRainbow(23010, 3000);
		Color uremam4 = getRainbow(23010, 4000);
		Color uremam5 = getRainbow(23010, 5000);
		Color uremam6 = getRainbow(23010, 6000);
		Color uremam7 = getRainbow(23010, 7000);
		Color uremam8 = getRainbow(23010, 8000);
		Render::gradient_horizontal(window->m_x + 0, window->m_y + 0, (window->m_iWidth - 0) / 8, 2, Color(uremam1.r(), uremam1.g(), uremam1.b(), MenuAlpha), Color(uremam1.r(), uremam1.g(), uremam1.b(), MenuAlpha));
		Render::gradient_horizontal(window->m_x + 0 + (window->m_iWidth - 0) / 8, window->m_y + 0, (window->m_iWidth - 0) / 8, 2, Color(uremam2.r(), uremam2.g(), uremam2.b(), MenuAlpha), Color(uremam2.r(), uremam2.g(), uremam2.b(), MenuAlpha));
		Render::gradient_horizontal(window->m_x + 0 + (window->m_iWidth - 0) / 8 + (window->m_iWidth - 0) / 8, window->m_y + 0, (window->m_iWidth - 0) / 8, 2, Color(uremam3.r(), uremam3.g(), uremam3.b(), MenuAlpha), Color(uremam3.r(), uremam3.g(), uremam3.b(), MenuAlpha));
		Render::gradient_horizontal(window->m_x + 0 + (window->m_iWidth - 0) / 8 + (window->m_iWidth - 0) / 8 + (window->m_iWidth - 0) / 8, window->m_y + 0, (window->m_iWidth - 0) / 8, 2, Color(uremam4.r(), uremam4.g(), uremam4.b(), MenuAlpha), Color(uremam4.r(), uremam4.g(), uremam4.b(), MenuAlpha)); // 4/8
		Render::gradient_horizontal(window->m_x + 0 + (window->m_iWidth - 0) / 8 + (window->m_iWidth - 0) / 8 + (window->m_iWidth - 0) / 8 + (window->m_iWidth - 0) / 8, window->m_y + 0, (window->m_iWidth - 0) / 8, 2, Color(uremam5.r(), uremam5.g(), uremam5.b(), MenuAlpha), Color(uremam5.r(), uremam5.g(), uremam5.b(), MenuAlpha)); // 5/8
		Render::gradient_horizontal(window->m_x + 0 + (window->m_iWidth - 0) / 8 + (window->m_iWidth - 0) / 8 + (window->m_iWidth - 0) / 8 + (window->m_iWidth - 0) / 8 + (window->m_iWidth - 0) / 8, window->m_y + 0, (window->m_iWidth - 0) / 8, 2, Color(uremam6.r(), uremam6.g(), uremam6.b(), MenuAlpha), Color(uremam6.r(), uremam6.g(), uremam6.b(), MenuAlpha)); // 6/8
		Render::gradient_horizontal(window->m_x + 0 + (window->m_iWidth - 0) / 8 + (window->m_iWidth - 0) / 8 + (window->m_iWidth - 0) / 8 + (window->m_iWidth - 0) / 8 + (window->m_iWidth - 0) / 8 + (window->m_iWidth - 0) / 8, window->m_y + 0, (window->m_iWidth - 0) / 8, 2, Color(uremam7.r(), uremam7.g(), uremam7.b(), MenuAlpha), Color(uremam7.r(), uremam7.g(), uremam7.b(), MenuAlpha)); // 7/8
		Render::gradient_horizontal(window->m_x + 0 + (window->m_iWidth - 0) / 8 + (window->m_iWidth - 0) / 8 + (window->m_iWidth - 0) / 8 + (window->m_iWidth - 0) / 8 + (window->m_iWidth - 0) / 8 + (window->m_iWidth - 0) / 8 + (window->m_iWidth - 0) / 8, window->m_y + 0, (window->m_iWidth - 0) / 8, 2, Color(uremam8.r(), uremam8.g(), uremam8.b(), MenuAlpha), Color(uremam8.r(), uremam8.g(), uremam8.b(), MenuAlpha)); // 8/8
	}
	//	else if (Options::Menu.ColorsTab.MenuBar.GetIndex() == 3)
	//	{

	//	}
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
	tinyxml2::XMLElement *Root = Doc.NewElement("Mirror.tk");
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
						CDropBox* cdropbox = nullptr;

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
						case UIControlTypes::UIC_dropdown:
						{
							cdropbox = (CDropBox*)Control;
							std::string xd;
							for (int i = 0; i < cdropbox->items.size(); i++)
							{
								std::string status;
								status = cdropbox->items[i].GetSelected ? "1" : "0";
								xd = xd + status;
							}
							ControlElement->SetText(xd.c_str());
							break;

						}
						}
					}
				}
			}
		}
	}
	if (Doc.SaveFile(Filename.c_str()) != tinyxml2::XML_NO_ERROR)
	{
		MessageBox(NULL, "Unable to save config file. Please try again", "Mirror.tk", MB_OK);
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
										CDropBox* cdropbox = nullptr;
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
										case UIControlTypes::UIC_dropdown:
											cdropbox = (CDropBox*)Control;
											for (int i = 0; i < cdropbox->items.size(); i++)
											{
												cdropbox->items[i].GetSelected = ControlElement->GetText()[i] == '1' ? true : false;
											}
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