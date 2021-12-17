#include "../../utilities/includes.h"
#include "../../menu/menu.hpp"
#include "../../utilities/renderer_d3d.hpp"
#include "../../features/Events/player_hurt.hpp"
#include "../../features/Aimbot/Autowall.hpp"
#include "../../features//Misc/Misc.hpp"
#include "../../features/Visuals/Visuals.hpp"
#include <chrono>
#include <iomanip>

void __fastcall CHookManager::onPaint( void *ecx, void *edx, unsigned int panel, bool unk0, bool unk1 )
{
	static auto originalFn = g_Hooks->panelHook->getOriginalFunction<decltype( &onPaint )>( 41 );
	static unsigned int targetPanel = -1;
	static unsigned int hudZoomPanel = -1;

	if ( strcmp( g_Interfaces->panel->getPanelName( panel ), "HudZoom" ) == 0 )
		hudZoomPanel = panel;

	if ( panel == hudZoomPanel && config->get_bool("espRemovalsScope") && Globals::localPlayer && Globals::localPlayer->alive( ) && Globals::localPlayer->scoped( ) )
		return;

	originalFn( ecx, edx, panel, unk0, unk1 );

	if ( strcmp( g_Interfaces->panel->getPanelName( panel ), "MatSystemTopPanel" ) == 0 )
		targetPanel = panel;

	if ( panel != targetPanel )
		return;

	if ( !g_Renderer->isUsingSurface( ) )
		return;

	g_Features->onPaint( panel );

	auto getCurrentTime = [ ] ( ) -> std::string
	{
		auto now = std::chrono::system_clock::now( );
		auto in_time_t = std::chrono::system_clock::to_time_t( now );

		std::stringstream ss;
		ss << std::put_time( std::localtime( &in_time_t ), "%X" );
		return ss.str( );
	};

	int x, y;
	g_Interfaces->gameEngine->getScreenSize( x, y );
	x /= 2; y /= 2;


/*#ifndef _DEV // YO THIS IS PREMIUM AS FUCK HAHAHAHAHHAHAHHAHHAAHAHAHAHHAHAA ~alpha
#ifndef _BETA
	g_Renderer->fillRectangle( 5, 5, 207, 18, Color( 0, 125, 255, 210 ) );
	g_Renderer->text( 9, 7, g_Fonts->watermark, std::string( "dopamine | " ).append( __DATE__ ).append( " | " ).append( getCurrentTime( ) ), Color( 222, 222, 222 ) );
#else
	g_Renderer->fillRectangle( 5, 5, 250, 18, Color( 0, 125, 255, 210 ) );
	g_Renderer->text( 9, 7, g_Fonts->watermark, std::string( "dopamine (Beta) | " ).append( __DATE__ ).append( " | " ).append( getCurrentTime( ) ), Color( 222, 222, 222 ) );
#endif	
#else
	g_Renderer->fillRectangle( 5, 5, 245, 18, Color( 0, 125, 255, 210 ) );
	g_Renderer->text( 9, 7, g_Fonts->watermark, std::string( "dopamine (Dev) | " ).append( __DATE__ ).append( " | " ).append( getCurrentTime( ) ), Color( 222, 222, 222 ) );
#endif*/

	if (config->get_bool("miscWatermark"))
	{
		std::stringstream ss;
		auto net_channel = g_Interfaces->gameEngine->netchannelInfo();

		std::string incoming = Globals::localPlayer ? std::to_string((int)(net_channel->GetLatency(FLOW_INCOMING) * 1000)) : "0";

		std::string outgoing = Globals::localPlayer ? std::to_string((int)(net_channel->GetLatency(FLOW_OUTGOING) * 1000)) : "0";

		//int x, y, w, h;

		//int textsize = render::get().text_width(fonts[rolexTEXT], ss.str().c_str());
		int x1, y1;

		ss << "dopamine | " << GetUserName << " | incoming: " << incoming.c_str() << "ms" << " | outgoing: " << outgoing.c_str() << "ms" << " | " << __TIME__ << "";

		g_Interfaces->surface->calculateTextSize(g_Fonts->watermark, g_Utils->toWchar(ss.str().c_str()), x1, y1);

		int SizeQM = x1 + y1;

		g_Renderer->fillRectangle(5, 5, SizeQM, 18, Color(
			185,
			55,
			55,
			85));
		g_Renderer->rectangle(5, 5, SizeQM, 18, Color::Black);

		g_Renderer->text(9, 7, g_Fonts->watermark, ss.str().c_str(), Color(255, 255, 255, 200));

		/*g_Renderer->fillRectangle(5, 5, 235, 18, Color(
			config->get_color("clr_menu").r(),
			config->get_color("clr_menu").g(),
			config->get_color("clr_menu").b(),
			85));
		g_Renderer->text(9, 7, g_Fonts->watermark, std::string("Dopamine | ").append(__DATE__).append(" | ").append(getCurrentTime()), Color(222, 222, 222));*/
	}

	if (config->get_bool("espAutowallIndicator") && Globals::localPlayer && Globals::localPlayer->alive( ) && g_Interfaces->gameEngine->inGame( ) ) {
		float damage = 0.f;
		g_Renderer->fillRectangle( x - 1, y - 1, 3, 3, g_Autowall.CanWallbang( damage ) ? Color( 0, 255, 0 ) : Color( 255, 0, 0 ) );
		if (g_Autowall.CanWallbang(damage))
			g_Renderer->text(x + 5, y + 5, g_Fonts->visuals, std::to_string(damage), Color(0, 255, 0), true);
	}


	/*if (config->get_bool("aaEnable") && config_system.item.config.bind_antiaimflip_key && Globals::localPlayer && Globals::localPlayer->alive() && g_Interfaces->gameEngine->inGame())
	{
		if (GetAsyncKeyState(config_system.item.config.bind_antiaimflip_key))
		{
			g_Renderer->text(x + 40, y, (g_Fonts->indicator), ">", Color(255, 255, 255, 55), true);
			g_Renderer->text(x - 40, y, (g_Fonts->indicator), "<", Color::MenuMain, true);
		}
		else
		{
			g_Renderer->text(x + 40, y, (g_Fonts->indicator), ">", Color::MenuMain, true);
			g_Renderer->text(x - 40, y, (g_Fonts->indicator), "<", Color(255, 255, 255, 55), true);
		}
	}*/
}

static bool initialized = false;

long __stdcall CHookManager::onPaintDX9(LPDIRECT3DDEVICE9 dx9Device, LPRECT sourceRect, LPRECT destRect, HWND destWindowOverride, LPRGNDATA dirtyRegion)
{
	static auto originalFn = g_Hooks->dx9DeviceHook->getOriginalFunction<decltype(&onPaintDX9)>(17);

	if (!initialized)
	{
		g_renderer_d3d->init(dx9Device);
		initialized = true;
	}

	g_renderer_d3d->begin();

	if (initialized)
	{
		if (!g_Interfaces->gameEngine->inGame() && !Globals::eventInfo.empty())
			Globals::eventInfo.clear();

		if (Globals::localPlayer && Globals::localPlayer->alive() && g_Interfaces->gameEngine->inGame()) {
			player_hurt::get().paint();
		}

		// LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOL ~alpha
		g_Misc.drawIndicators();
		g_Visuals.inaccuracyOverlay();

		menu->think();
		menu->draw();
	}

	g_renderer_d3d->end();

	if (!g_Renderer->isUsingDX9())
		return originalFn(dx9Device, sourceRect, destRect, destWindowOverride, dirtyRegion);

	g_Renderer->preRender();
	g_Features->onPaint();
	g_Renderer->postRender();

	return originalFn(dx9Device, sourceRect, destRect, destWindowOverride, dirtyRegion);
}