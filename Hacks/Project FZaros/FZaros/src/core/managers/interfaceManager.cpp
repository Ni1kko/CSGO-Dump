#include "interfaceManager.hpp"
#include "../../utilities/memory.hpp"
#include "../../utilities/includes.h"

unsigned long Globals::g_iModelBoneCounter;

void CInterfaceManager::initialize( )
{
	this->baseClient = ( CBaseClient* )g_InterfaceUtils->grabInterface( "client.dll", "VClient018" );
	this->gameEngine = ( CGameEngine* )g_InterfaceUtils->grabInterface( "engine.dll", "VEngineClient014" );
	this->gameUI = ( CGameUI* )g_InterfaceUtils->grabInterface( "client.dll", "GameUI011" );
	this->panel = ( CPanel* )g_InterfaceUtils->grabInterface( "vgui2.dll", "VGUI_Panel009" );
	this->surface = ( CSurface* )g_InterfaceUtils->grabInterface( "vguimatsurface.dll", "VGUI_Surface031" );
	this->clientEntity = ( IClientEntityList* )g_InterfaceUtils->grabInterface( "client.dll", "VClientEntityList003" );
	this->trace = ( IEngineTrace* )g_InterfaceUtils->grabInterface( "engine.dll", "EngineTraceClient004" );
	this->modelInfo = ( IVModelInfo* )g_InterfaceUtils->grabInterface( "engine.dll", "VModelInfoClient004" );
	this->cvar = ( ICvar* )g_InterfaceUtils->grabInterface( "vstdlib.dll", "VEngineCvar007" );
	this->materialSystem = ( IMaterialSystem* )g_InterfaceUtils->grabInterface( "materialsystem.dll", "VMaterialSystem080" );
	this->renderView = ( IRenderView* )g_InterfaceUtils->grabInterface( "engine.dll", "VEngineRenderView014" );
	this->modelRender = ( IVModelRender* )g_InterfaceUtils->grabInterface( "engine.dll", "VEngineModel016" );
	this->surfaceData = ( IPhysicsSurfaceProps* )g_InterfaceUtils->grabInterface( "vphysics.dll", "VPhysicsSurfaceProps001" );
	this->prediction = ( CPrediction* )g_InterfaceUtils->grabInterface( "client.dll", "VClientPrediction001" );
	this->glow = *reinterpret_cast< CGlowObjectManager** >( g_Memory->findPattern( "client.dll", "0F 11 05 ? ? ? ? 83 C8 01" ) + 3 );
	this->gameEvent = ( IGameEventManager2* )g_InterfaceUtils->grabInterface( "engine.dll", "GAMEEVENTSMANAGER002" );
	this->debugOverlay = ( IVDebugOverlay* )g_InterfaceUtils->grabInterface( "engine.dll", "VDebugOverlay004" );
	this->localize = ( ILocalize* )g_InterfaceUtils->grabInterface( "localize.dll", "Localize_001" );
	this->studioRender = ( CStudioRender* )g_InterfaceUtils->grabInterface( "studiorender.dll", "VStudioRender026" );
	this->gameMovement = ( IGameMovement* )g_InterfaceUtils->grabInterface( "client.dll", "GameMovement001" );
	this->engineSound = ( CEngineSound* )g_InterfaceUtils->grabInterface( "engine.dll", "IEngineSoundClient003" );
	this->inputSystem = ( IInputSystem* )g_InterfaceUtils->bruteforceInterface( "inputsystem.dll", "InputSystemVersion" );

	this->weaponSystem = *reinterpret_cast< IWeaponSystem** >(g_Memory->findPattern( "client.dll", "8B 35 ? ? ? ? FF 10 0F B7 C0" ) + 2);

	this->moveHelper = **reinterpret_cast< IMoveHelper*** >( g_Memory->findPattern( "client.dll", "8B 0D ? ? ? ? 8B 46 08 68" ) + 2 );

	this->renderBeams = *reinterpret_cast< IViewRenderBeams** >( g_Memory->findPattern( "client.dll", "A1 ? ? ? ? FF 10 A1 ? ? ? ? B9" ) + 1 );

	this->clientState = **( CClientState*** )( g_Memory->findPattern( "engine.dll", "A1 ? ? ? ? 8B 80 ? ? ? ? C3" ) + 1 );

	this->dx9Device = ( IDirect3DDevice9* ) **( uintptr_t** )( g_Memory->findPattern( "shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C" ) + 1 );
	this->clientMode = **reinterpret_cast< void*** >( ( *reinterpret_cast< uintptr_t * * >( this->baseClient ) )[ 10 ] + 5 );
	this->globalVars = **reinterpret_cast< IGlobalVars*** >( ( *reinterpret_cast< uintptr_t * * >( this->baseClient )[ 0 ] + 27 ) );

	Globals::g_iModelBoneCounter = **reinterpret_cast< unsigned long** >( g_Memory->findPattern( "client.dll", "80 3D ? ? ? ? 00 74 16 A1" ) + 10 ); //80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81
}

CInterfaceManager * g_Interfaces = new CInterfaceManager( );