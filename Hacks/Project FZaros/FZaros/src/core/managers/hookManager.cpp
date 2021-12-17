#include "hookManager.hpp"

#include "../../features/Events/player_hurt.hpp"
#include "../../features/Events/bullet_impact.hpp"
#include "../../features/Events/weapon_fire.hpp"
#include "../../features/Events/player_death.hpp"
#include "../../features/Events/round_start.hpp"
#include "../../features/Misc/Animations.hpp"
#include "../../menu/menu.hpp"

#include "../../utilities/includes.h"

HWND Globals::csgoWindow;
WNDPROC Globals::wndProcOrig;
uint8_t* present_address;
uint8_t* reset_address;

LRESULT __stdcall windowProcedure( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );

void CHookManager::initialize( )
{
	// this is where we hook shit if u didn't know. basically, slay all niggers ~alpha
	player_hurt::get( ).registerSelf( );
	bullet_impact::get( ).registerSelf( );
	weapon_fire::get( ).registerSelf( );
	player_death::get( ).registerSelf( );

	this->dx9DeviceHook = new CVFTHook( g_Interfaces->dx9Device );
	this->baseClientHook = new CVFTHook( g_Interfaces->baseClient );
	this->clientModeHook = new CVFTHook( g_Interfaces->clientMode );
	this->surfaceHook = new CVFTHook( g_Interfaces->surface );
	this->panelHook = new CVFTHook( g_Interfaces->panel );
	this->renderViewHook = new CVFTHook( g_Interfaces->renderView );
	this->materialHook = new CVFTHook( g_Interfaces->materialSystem );
	this->modelRenderHook = new CVFTHook( g_Interfaces->modelRender );
	this->gameEngineHook = new CVFTHook( g_Interfaces->gameEngine );

	//this->gameEventHook = new CVFTHook(g_Interfaces->gameEvent);

	this->dx9DeviceHook->swapFunction( 17, CHookManager::onPaintDX9 );
	this->dx9DeviceHook->swapFunction( 16, CHookManager::onResizeDX9 );
	this->baseClientHook->swapFunction( 37, CHookManager::onFrameStage );
	this->gameEngineHook->swapFunction( 101, CHookManager::aspectRatio );
	this->clientModeHook->swapFunction( 24, CHookManager::onMove );
	this->clientModeHook->swapFunction( 18, CHookManager::overrideView );
	this->surfaceHook->swapFunction( 116, CHookManager::onResize );
	this->surfaceHook->swapFunction( 67, CHookManager::lockCursor );
	this->panelHook->swapFunction( 41, CHookManager::onPaint );
	this->renderViewHook->swapFunction( 9, CHookManager::sceneEnd );
	this->materialHook->swapFunction( 42, CHookManager::beginFrame );
	this->modelRenderHook->swapFunction( 21, CHookManager::drawModelExecute );

	//this->gameEventHook->swapFunction(9, CHookManager::fireEventClientSide);

	g_Interfaces->cvar->FindVar( "mat_postprocess_enable" )->SetValue( 0 );

	Globals::csgoWindow = FindWindow("Valve001", NULL);

	Globals::wndProcOrig = (WNDPROC)SetWindowLongPtrA(Globals::csgoWindow, GWL_WNDPROC, (LONG)windowProcedure);
}

void CHookManager::shutdown( )
{
	g_Interfaces->cvar->FindVar( "mat_postprocess_enable" )->SetValue( 1 );

	player_hurt::get( ).unRegisterSelf( );
	bullet_impact::get( ).unRegisterSelf( );
	weapon_fire::get( ).unRegisterSelf( );
	player_death::get( ).unRegisterSelf( );

	this->dx9DeviceHook->release( );
	this->baseClientHook->release( );
	this->clientModeHook->release( );
	this->surfaceHook->release( );
	this->panelHook->release( );
	this->renderViewHook->release( );
	this->materialHook->release( );
	this->modelRenderHook->release( );
	this->gameEngineHook->release( );

	**reinterpret_cast<void***>(present_address) = reinterpret_cast<void*>(original_present);
	**reinterpret_cast<void***>(reset_address) = reinterpret_cast<void*>(original_reset);

	SetWindowLongPtrA( Globals::csgoWindow, GWL_WNDPROC, ( LONG )Globals::wndProcOrig );

}

CHookManager* g_Hooks = new CHookManager( );

LRESULT __stdcall windowProcedure(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
	if (GetForegroundWindow() != Globals::csgoWindow) {
		menu->opened = false;
		return CallWindowProcA(Globals::wndProcOrig, hwnd, message, wparam, lparam);
	}

	// hahahahahahhahhahahahahahahahahhahaha using getasynckeystate in wndproc hahahahahahahahahahhahaha ~alpha
	static bool pressed = false;
	if (!pressed && GetAsyncKeyState(VK_INSERT)) {
		pressed = true;
	}
	else if (pressed && !GetAsyncKeyState(VK_INSERT)) {
		pressed = false;

		menu->opened = !menu->opened;
	}

	if (menu->opened) {
		if (message == WM_MOUSEWHEEL)
			menu->set_mouse_wheel(GET_WHEEL_DELTA_WPARAM(wparam) / WHEEL_DELTA);

		return false;
	}

	return CallWindowProcA(Globals::wndProcOrig, hwnd, message, wparam, lparam);
}
