#pragma once
#include "../Include/GlobalIncludes.h"
#include "../SDK/IClient.h"
#include "../SDK/IClientMode.h"
#include "../SDK/ISurface.h"
#include "../SDK/ModelInfo.h"
#include "../SDK/IEngine.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CPanel.h"
#include "../SDK/GameEvents.h"
#include "../SDK/CGlowObjectManager.h"
#include "../SDK/ModelRender.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/CInput.h"
#include "../SDK/CTrace.h"
#include "../SDK/ConVar.h"
#include "../SDK/IMaterial.h"
#include "../SDK/IVDebugOverlay.h"
#include "../SDK/RenderView.h"
#include "../SDK/Cbeam.h"
#include "../SDK/physsurface.h"
#include "../SDK/CPrediction.h"
#include "../SDK/ILocalize.h"
#include "../SDK/IGameRules.h"
#include "../SDK/malloc.h"

namespace Interfaces
{
	class CInterfaceManager
	{
		public:
			CInterfaceManager( std::string szModuleName );

			template<typename T>
			T* GetInterface( std::string szInterfaceVersion );

			template<typename T>
			T* BruteInterface( std::string szInterface );
		private:
			CreateInterfaceFn m_hCreateInterface;
	};

	class CInterfaces
	{
	protected:
		CInterfaceManager *ClientManager,
						  *EngineManager,
						  *VGUIMatSurfaceManager,
						  *VGUI2Manager,
						  *VPhysicsManager,
						  *VSTDLibManager,
						  *MaterialSYSManager,
						  *VStudioRender,
						  *LocalizeManager;
	public:
		void Initialize();
		void Free();
	};

	extern CInterfaces* InterfaceLoader;

	extern IClient* Client;
	extern CModelInfo* ModelInfo;
	extern IMoveHelper* MoveHelper;
	extern IPrediction* Prediction;
	extern IGameMovement* GameMovement;
	extern ISurface* Surface;
	extern IEngine* Engine;
	extern CClientEntityList* EntityList;
	extern CPanel* GuiPanel;
	extern IClientMode* ClientMode;
	extern IGameEventManager* GameEvents;
	extern CModelRender* ModelRender;
	extern CGlowObjectManager* GlowObjectManager;
	extern CGlobalVars* Globals;
	extern CInput* Input;
	extern CTrace* Trace;
	extern ICvar* Var;
	extern IMaterialSystem* MaterialSystem;
	extern CRenderView* RenderView;
	extern IVDebugOverlay* DebugOverlay;
	extern IViewRenderBeams* RenderBeams;
	extern void* EngineSound;
	extern IPhysicsSurfaceProps* PhysSurface;
	extern CClientState* ClientState;
	extern ILocalize* Localize;
	extern IMemAlloc *MemAlloc;
	//extern IGameRules** GameRules;
	extern void(__cdecl* RandomSeed)(uint32_t seed);
	extern int(*RandomInt)(int, int);
}
