#pragma once

#include <set>
#include <fstream>
#include <link.h> // dl_iterate_phdr

#include "SDK/SDK.h"
#include "Utils/vmt.h"
#include "Utils/util.h"
#include "Utils/xorstring.h"

extern IBaseClientDLL* client;
extern ISurface* surface;
extern IVPanel* panel;
extern IEngineClient* engine;
extern IVEngineClient* engine_p;
extern IClientEntityList* entityList;
extern IVDebugOverlay* debugOverlay;
extern IVModelInfo* modelInfo;
extern IVModelRender* modelRender;
extern IClientMode* clientMode;
extern IEngineTrace* trace;
extern IInputSystem* inputSystem;
extern IInputInternal* inputInternal;
extern IMaterialSystem* material;
extern ICvar* cvar;
extern CGlobalVars* globalVars;
extern CEffects* effects;
extern IGameEventManager2* gameEvents;
extern IPhysicsSurfaceProps* physics;
extern CViewRender* viewRender;
extern IPrediction* prediction;
extern IGameMovement* gameMovement;
extern IMoveHelper* moveHelper;
extern CGlowObjectManager* glowManager;
extern ILauncherMgr* launcherMgr;
extern C_CSPlayerResource** csPlayerResource;
extern C_CSGameRules** csGameRules;
extern IEngineVGui* engineVGui;
extern IEngineSound* sound;
extern ILocalize* localize;
extern ICommandLine* commandline;

extern ISteamMatchmaking* SteamMatchmaking;
extern ISteamClient* SteamClient;

extern CInput* input;
extern IPanoramaUIEngine* panoramaEngine;
extern SteamMatchmakingServers002* SteamMatchmakingServers;

extern VMT* panelVMT;
extern VMT* clientVMT;
extern VMT* modelRenderVMT;
extern VMT* clientModeVMT;
extern VMT* gameEventsVMT;
extern VMT* viewRenderVMT;
extern VMT* inputInternalVMT;
extern VMT* materialVMT;
extern VMT* surfaceVMT;
extern VMT* launcherMgrVMT;
extern VMT* engineVGuiVMT;
extern VMT* soundVMT;
extern VMT* uiEngineVMT;

extern LineGoesThroughSmokeFn LineGoesThroughSmoke;
extern InitKeyValuesFn InitKeyValues;
extern LoadFromBufferFn LoadFromBuffer;

//extern RandomSeedFn RandomSeed;
//extern RandomFloatFn RandomFloat;
//extern RandomFloatExpFn RandomFloatExp;
//extern RandomIntFn RandomInt;
//extern RandomGaussianFloatFn RandomGaussianFloat;

extern SetNamedSkyBoxFn SetNamedSkyBox;

extern int* nPredictionRandomSeed;
extern CMoveData* g_MoveData;
extern bool* s_bOverridePostProcessingDisable;

extern uintptr_t oSwapWindow;
extern uintptr_t* swapWindowJumpAddress;

extern uintptr_t oPollEvent;
extern uintptr_t* polleventJumpAddress;

namespace Interfaces
{
	void FindInterfaces();
	void DumpInterfaces();
}
