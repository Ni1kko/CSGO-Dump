#include "../../utilities/includes.h"
#include "../../utilities/renderer_d3d.hpp"

void __fastcall CHookManager::onResize( void *ecx, void *edx, int oldWidth, int oldHeight )
{
	static auto originalFn = g_Hooks->surfaceHook->getOriginalFunction<decltype( &onResize )>( 116 );

	g_Renderer->invalidate( );
	g_Renderer->initialize( nullptr );

	originalFn( ecx, edx, oldWidth, oldHeight );
}

long __stdcall CHookManager::onResizeDX9( LPDIRECT3DDEVICE9 dx9Device, D3DPRESENT_PARAMETERS *presentParameters )
{
	static auto originalFn = g_Hooks->dx9DeviceHook->getOriginalFunction<decltype( &onResizeDX9 )>( 16 );

	g_Renderer->invalidate( );
	g_renderer_d3d->invalidate( );

	long originalCallResult = originalFn( dx9Device, presentParameters );

	g_Renderer->initialize( dx9Device );
	g_renderer_d3d->init( dx9Device );

	return originalCallResult;
}