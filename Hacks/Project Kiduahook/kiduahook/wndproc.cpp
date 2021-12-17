#include "wndproc.h"

LRESULT __stdcall HOOKED_WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( uMsg == WM_MOUSEWHEEL )
	{
		if( ( short ) HIWORD( wParam ) < 0 )
		{
			g_Gui.bDownScroll = true;
			g_Gui.bSecDownScroll = true;
			g_Gui.bUpScroll = false;
			g_Gui.bSecUpScroll = false;
			g_Gui.bSliderDown = true;
			g_Gui.bSliderUp = false;
			g_Gui.bSecSliderUp = false;
			g_Gui.bSecSliderDown = true;
		}
		else
		{
			g_Gui.bUpScroll = true;
			g_Gui.bSecUpScroll = true;
			g_Gui.bDownScroll = false;
			g_Gui.bSecDownScroll = false;
			g_Gui.bSliderDown = false;
			g_Gui.bSliderUp = true;
			g_Gui.bSecSliderUp = true;
			g_Gui.bSecSliderDown = false;
		}
	}

	return CallWindowProc( OldWindowProc, hWnd, uMsg, wParam, lParam );
}
