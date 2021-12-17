#pragma once

#include "CommonIncludes.h"

#include <map>

extern bool SaveFile;
extern bool LoadFile;

class CControl;
class CTab;
class CWindow;
class CGUI;
class CRadar;

extern CGUI GUI;
extern CRadar *Radar;

namespace Globals
{
	extern float MenuAlpha2;
}

namespace Globals
{
	extern float MenuAlpha4;
}

namespace Globals
{
	extern float MenuAlpha6;
}

namespace Globals
{
	extern float MenuAlpha8;
}

namespace Globals
{
	extern float MenuAlpha10;
}

namespace Globals
{
	extern float MenuAlpha22;
}

namespace Globals
{
	extern float MenuAlpha12;
}

namespace Globals
{
	extern float MenuAlpha14;
}

namespace Globals
{
	extern float MenuAlpha16;
}

namespace Globals
{
	extern float MenuAlpha18;
}

namespace Globals
{
	extern float MenuAlpha20;
}

namespace Globals
{
	extern float MenuAlpha24;
}

extern float MenuAlpha;
extern float MenuAlpha3;
extern float MenuAlpha5;
extern float MenuAlpha7;
extern float MenuAlpha9;
extern float MenuAlpha11;
extern float MenuAlpha17;
extern float MenuAlpha19;
extern float MenuAlpha21;
extern float MenuAlpha23;


enum UIFlags
{
	UI_None = 0x00,
	UI_Drawable = 0x01,
	UI_Clickable = 0x02,
	UI_Focusable = 0x04,
	UI_RenderFirst = 0x08,
	UI_SaveFile = 0x10
};

enum UIControlTypes
{
	UIC_CheckBox = 1,
	UIC_Slider,
	UIC_KeyBind,
	UIC_ComboBox,
	UIC_ItemSelector,
	UIC_Button,
	UIC_MultiBox,
	UIC_GroupBox,
	UIC_ListBox,
	UIC_ColorSelector,
	UIC_Label,
	UIC_TextField,
	UIC_dropdown
};

class CControl
{
	friend class CGUI;
	friend class CTab;
	friend class CWindow;
public:
	void SetPosition(int x, int y);
	void SetSize(int w, int h);
	void GetSize(int &w, int &h);
	void SetFileId(std::string fid);
	int FileControlType;

	bool Flag(int f);
	CControl* parent_group;
	int g_tab = 0;
protected:
	int m_x, m_y;
	int m_iWidth, m_iHeight;
	int m_Flags;

	CWindow* parent;

	std::string FileIdentifier;

	virtual void Draw(bool) = 0;
	virtual void OnUpdate() = 0;
	virtual void OnClick() = 0;

	POINT GetAbsolutePos();

};

class CTab
{
	friend class CControl;
	friend class CGUI;
	friend class CWindow;
public:
	void SetTitle(std::string name);
	void RegisterControl(CControl* control);
private:
	std::string Title;
	std::vector<CControl*> Controls;
	CWindow* parent;
};

class CWindow
{
	friend class CControl;
	friend class CGUI;
public:
	void SetPosition(int x, int y);
	void SetSize(int w, int h);
	void SetTitle(std::string title);
	void Open();
	void Close();
	void Toggle();
	CControl* GetFocus();
	bool m_bIsOpen;
	void RegisterTab(CTab* Tab);

	RECT GetDragArea();

	RECT GetClientArea();
	RECT GetTabArea();

private:
	void DrawControls();


	std::vector<CTab*> Tabs;
	CTab* SelectedTab;

	bool IsFocusingControl;
	CControl* FocusedControl;

	std::string Title;
	int m_x;
	int m_y;
	int m_iWidth;
	int m_iHeight;

};


class CRadar
{
	friend class CControl;
	friend class CGUI;
public:
	void SetPosition(int x, int y);
	void SetSize(int w, int h);
	void SetTitle(std::string title);

private:
	std::string Title;
	int m_x;
	int m_y;
	int m_iWidth;
	int m_iHeight;
};


class CGUI
{
public:
	CGUI();

	void Draw();

	void Update();


	bool DrawWindow(CWindow* window);

	bool epic_gamer_window(CWindow * window);

	void RegisterWindow(CWindow* window);

	void SaveWindowState(CWindow* window, std::string Filename);
	void LoadWindowState(CWindow* window, std::string Filename);

	void BindWindow(unsigned char Key, CWindow* window);

	bool GetKeyPress(unsigned int key);
	bool GetKeyState(unsigned int key);
	bool IsMouseInRegion(int x, int y, int x2, int y2);
	bool IsMouseInRegion(RECT region);
	POINT GetMouse();

private:

	bool keys[256];
	bool oldKeys[256];

	POINT Mouse;
	bool MenuOpen;
	bool IsDraggingWindow;
	CWindow* DraggingWindow;
	CRadar* DraggingRadar;

	int DragOffsetX; int DragOffsetY;

	std::vector<CWindow*> Windows;

	std::map<int, CWindow*> WindowBinds;

};