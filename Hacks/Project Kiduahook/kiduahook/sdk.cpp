#include "sdk.h"

HLCLient* g_pClient;
VEngineClient* g_pEngine;
VClientEntityList* g_pEntList;
CInput* g_pInput;
CPrediction* g_pPred;
CGameMovement* g_pGameMovement;
CGlobalVars* g_pGlobalVars;
VDebugOverlay* g_pDebugOverlay;
CEngineTraceClient* g_pEngineTrace;
VPhysicsSurfaceProps* g_pPhysics;
CGlowObjectManager* g_pGlowObjectManager;
IMoveHelper* g_pMoveHelper;
VGUI_Surface* g_pSurface;
IVRenderView* g_pRenderView;
VGUI_Panel* g_pPanel;
VModelInfoClient* g_pModel;
VEngineModel* g_pEngineModel;
ICVar* g_pICvar;
IMaterialSystem* g_pMatsystem;
IEffects* g_pEffects;
IEngineSoundClient* g_pEngineSound;
IGameEventManager2* g_pGameEventManager2;
IInputSystem* g_pInputSystem;
IViewRenderBeams* g_pRenderBeams;
IStudioRender*	g_pStudioRender;
CBaseClientState* g_pClientState;
IMemAlloc* g_pMemAlloc;
CHudChat* g_pHudChat;

IMaterial* BubbleMat = nullptr;
IMaterial* FlatMat = nullptr;
IMaterial* Ghost2Mat = nullptr;
IMaterial* TvMat = nullptr;
IMaterial* CryptsMat = nullptr;
IMaterial* PulseMat = nullptr;
IMaterial* BanannaMat = nullptr;
IMaterial* Ghost3Mat = nullptr;
IMaterial* NetMat = nullptr;
IMaterial* AnimMat = nullptr;

DWORD dwClient;
DWORD dwVGUIMAT;
DWORD dwVGUI2;
DWORD dwEngine;