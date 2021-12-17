#include <chrono>
#include <thread>
#include <windows.h>

#include "../menu/main/setup.hpp"
#include "../utilities/includes.h"
#include "../features/features.hpp"
#include "../features/Misc/Animations.hpp"

//#define _DEV

void chams_material_setup() {
	std::ofstream("csgo/materials/dopamine_material.vmt") << R"#("VertexLitGeneric" {
            "$basetexture" "vgui/white_additive"
            "$ignorez"      "0"
            "$envmap"       ""
            "$nofog"        "1"
            "$model"        "1"
            "$nocull"       "0"
            "$selfillum"    "1"
            "$halflambert"  "1"
            "$znearer"      "0"
            "$flat"         "0"
			"$phong"		"1"
			"$rimlight"		"1"
        })#";

	std::ofstream("csgo/materials/dopamine_reflective.vmt") << R"#("VertexLitGeneric" {
			"$basetexture" "vgui/white_additive"
			"$ignorez" "0"
			"$envmap" "env_cubemap"
			"$normalmapalphaenvmapmask" "1"
			"$envmapcontrast"  "1"
			"$nofog" "1"
			"$model" "1"
			"$nocull" "0"
			"$selfillum" "1"
			"$halflambert" "1"
			"$znearer" "0"
			"$flat" "1"
		})#";

	printf("Materials initialized!\n");
}

void initializeCheat( void *modulePtr )
{
	while ( !GetModuleHandleA( "serverbrowser.dll" ) )
		std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );

#if defined(_DEV) || defined(_DEBUG)
	AllocConsole( );
	freopen_s( reinterpret_cast< FILE** >( stdout ), "CONOUT$", "w", stdout );
#endif

	g_Interfaces->initialize( );

	chams_material_setup( );

	g_NetVarManager = new CNetVarManager( );

	//this is my cool feature system ;))))))) ~czapek
	g_Features->registerFeature( "Visuals", new CVisuals( ) );
	g_Features->registerFeature( "Misc", new CMisc( ) );

	g_Renderer->initialize( ( LPDIRECT3DDEVICE9 )g_Interfaces->dx9Device );
	g_Renderer->useSurface( );

	setup->setup( );
	g_Hooks->initialize( );

	g_Interfaces->gameUI->messageBox( "Dopamine", "Successfully loaded! Make sure Multisampling Anti-Aliasing Mode is OFF!" );

	while ( !config->get_bool("unload"))
		std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

	g_Interfaces->inputSystem->enableInput( true );

	g_Features->unregisterFeature( "Visuals" );
	g_Features->unregisterFeature( "Misc" );

	g_Hooks->shutdown( );

	std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

#if defined(_DEV) || defined(_DEBUG)
	fclose( stdout );
	FreeConsole( );
#endif

	FreeLibraryAndExitThread( reinterpret_cast< HMODULE >( modulePtr ), 0 );
}

bool __stdcall DllMain( void* Module, unsigned long Reason, void* Reserved )
{
	DisableThreadLibraryCalls( ( HMODULE )Module );
	switch ( Reason ) {
	case DLL_PROCESS_ATTACH:
	{
		CreateDirectoryA(("C:\\dopamine"), NULL);
		CreateDirectoryA(("C:\\dopamine\\logs"), NULL);
		CreateDirectoryA(("C:\\dopamine\\configs"), NULL);
		CreateDirectoryA(("C:\\dopamine\\resources"), NULL);
		CreateDirectoryA(("C:\\dopamine\\resources\\fonts"), NULL);

		CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(initializeCheat), Module, 0, 0);
	}
		break;
	}

	return true;
}