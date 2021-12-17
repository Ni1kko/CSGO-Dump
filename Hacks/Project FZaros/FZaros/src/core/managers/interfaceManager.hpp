#pragma once

#include "../../sdk/baseClient.hpp"
#include "../../sdk/gameEngine.hpp"
#include "../../sdk/gameUI.hpp"
#include "../../sdk/panel.hpp"
#include "../../sdk/surface.hpp"
#include "../../sdk/baseEntity.hpp"
#include "../../sdk/interfaces/engineTrace.hpp"
#include "../../sdk/modelInfo.hpp"
#include "../../sdk/interfaces/ICvar.hpp"
#include "../../sdk/materialSystem.hpp"
#include "../../sdk/interfaces/IRenderView.hpp"
#include "../../sdk/surfaceData.hpp"
#include "../../sdk/prediction.hpp"
#include "../../sdk/glowObjectManager.hpp"
#include "../../sdk/gameEventManager.hpp"
#include "../../sdk/interfaces/renderBeams.hpp"
#include "../../sdk/interfaces/debugOverlay.hpp"
#include "../../sdk/interfaces/ILocalize.hpp"
#include "../../sdk/interfaces/CStudioRender.hpp"
#include "../../sdk/interfaces/engineSound.hpp"
#include "../../sdk/interfaces/IInputSystem.hpp"
#include "../../sdk/prediction.hpp"
#include <d3d9.h>

class CInterfaceManager
{
public:
	void initialize( );

	CBaseClient* baseClient;
	CGameEngine* gameEngine;
	CGameUI* gameUI;
	CPanel* panel;
	CSurface* surface;
	IClientEntityList* clientEntity;
	IEngineTrace* trace;
	IVModelInfo* modelInfo;
	ICvar* cvar;
	IMaterialSystem* materialSystem;
	IRenderView* renderView;
	IVModelRender* modelRender;
	IPhysicsSurfaceProps* surfaceData;
	CPrediction* prediction;
	CGlowObjectManager* glow;
	IGameEventManager2* gameEvent;
	IViewRenderBeams* renderBeams;
	IVDebugOverlay* debugOverlay;
	ILocalize* localize;
	CStudioRender* studioRender;
	IGameMovement* gameMovement;
	IMoveHelper* moveHelper;
	CEngineSound* engineSound;
	//i_localize* interfaces::localize = nullptr;
	CClientState* clientState;
	IInputSystem* inputSystem;
	IWeaponSystem* weaponSystem;

	IDirect3DDevice9* dx9Device;
	void* clientMode;
	IGlobalVars* globalVars;

};

extern CInterfaceManager* g_Interfaces;