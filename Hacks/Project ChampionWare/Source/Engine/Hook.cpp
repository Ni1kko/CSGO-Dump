
#include "Hook.h"
#include "intrin.h"




namespace Engine
{
	namespace Hook
	{
		CSX::Hook::VTable IDirect3DDevice9Table;
		CSX::Hook::VTable ModelRenderTable; 

		vfunc_hook surfaceCursor;
		vfunc_hook panel;
		vfunc_hook client;
		vfunc_hook clientmode;
		vfunc_hook soundmode;
		vfunc_hook eventmanager;
		vfunc_hook device;

		IDirect3DDevice9* g_pDevice = nullptr;
		LockCursor oLockCursor;
		DrawModelExecute original_dme;

		typedef HRESULT(WINAPI* Present_t)(IDirect3DDevice9* pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
		Present_t Present_o;

		typedef HRESULT(WINAPI* Reset_t)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
		Reset_t Reset_o;

		HRESULT WINAPI Hook_Present(IDirect3DDevice9* pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
		{
			//Crosshair(pDevice);
			//AutoAccept();
			
			Client::OnRender();

			

			return Present_o(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
		}

		void __stdcall Hook_LockCursor(void* xd)
		{
			oLockCursor = surfaceCursor.get_original<LockCursor>(TABLE::ISurface::LockCursor);

			if (bIsGuiVisible)
			{
				Interfaces::Surface()->UnlockCursor();
				return;
			}

			oLockCursor(Interfaces::Surface());
		}

	 	HRESULT WINAPI Hook_Reset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
		{
			Client::OnLostDevice();

			HRESULT hRes = Reset_o(pDevice, pPresentationParameters);

			Client::OnResetDevice();

			return hRes;
		} 


		void __stdcall Hook_CreateMove(float flInputSampleTime, CUserCmd* pCmd)
		{
			static auto ofunc = clientmode.get_original<CreateMove>(TABLE::IClientMode::CreateMove);
			ofunc(Interfaces::ClientMode(), flInputSampleTime, pCmd);
			Client::OnCreateMove(pCmd);
		}

		
		

		void __stdcall emit_sound(void* ecx, void* edx, void* filter, int iEntIndex, int iChannel, const char *pSoundEntry, unsigned int nSoundEntryHash, const char *pSample, float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch, const Vector *pOrigin, const Vector *pDirection, Vector * pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, /*StartSoundParams_t&*/ void* params)
		{
			static auto oemitsound = soundmode.get_original<emitsound_fn>(5);

			Client::Emit_Sound(pSoundEntry);

			oemitsound(ecx, edx, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, flVolume, flAttenuation, nSeed, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity, params);
		}


		void __stdcall Hook_GetLobbyId(float flInputSampleTime, CUserCmd* pCmd)
		{
			/*static auto ofunc = clientmode.get_original<GetLobbyId>(match_session, 4)(match_session);
			ofunc(Interfaces::ClientMode(), flInputSampleTime, pCmd);
			Client::OnCreateMove(pCmd);*/
		}


		bool __fastcall Hook_FireEventClientSideThink(void* ecx, void* edx, IGameEvent* pEvent)
		{
			static auto ofunc = eventmanager.get_original<GameEvent>(TABLE::IGameEventManager2::FireEventClientSide);
			if (!pEvent)
				return ofunc(ecx, pEvent);
			Client::OnFireEventClientSideThink(pEvent);
			return ofunc(ecx, pEvent);
		}

		void __stdcall Hook_FrameStageNotify(ClientFrameStage_t stage)
		{
			static auto ofunc = client.get_original<FrameStageNotify>(37);
			ofunc(Interfaces::Client(), stage);
			Skin_OnFrameStageNotify(stage);
			Gloves_OnFrameStageNotify(stage);

		}

		void WINAPI Hook_DrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld = NULL)
		{
			ModelRenderTable.UnHook();

			if (ctx && pCustomBoneToWorld)
				Client::OnDrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);

			Interfaces::ModelRender()->DrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);

			if (ctx && pCustomBoneToWorld && Client::g_pEsp && Settings::Esp::esp_Chams && Interfaces::Engine()->IsInGame())
			{
				Interfaces::ModelRender()->ForcedMaterialOverride(0);
			}

			ModelRenderTable.ReHook();
		}

		bool Initialize()
		{
			if (!CSX::Utils::IsModuleLoad(D3D9_DLL))
				return false;

			if (!CSX::Utils::IsModuleLoad(SHADERPIDX9_DLL))
				return false;

			if (!CSX::Utils::IsModuleLoad(GAMEOVERLAYRENDERER_DLL))
				return false;

			DWORD d3d9TablePtrPtr = CSX::Memory::FindPattern(SHADERPIDX9_DLL, D3D9_PATTERN, D3D9_MASK, 1);
			DWORD_PTR** dwPresent_o = (DWORD_PTR**)CSX::Memory::FindPattern(GAMEOVERLAYRENDERER_DLL, GMOR_PATTERN, GMOR_MASK, 1);

			if (d3d9TablePtrPtr && dwPresent_o)
			{
				g_pDevice = (IDirect3DDevice9*)(**(PDWORD*)d3d9TablePtrPtr);

				if (device.setup(g_pDevice) && /*direct3d.setup*/ IDirect3DDevice9Table.InitTable (g_pDevice))
				{

					DWORD_PTR* dwAddress = *dwPresent_o;
					Present_o = (Present_t)(*dwAddress);
					*dwAddress = (DWORD_PTR)(&Hook_Present);

					Reset_o = (Reset_t)IDirect3DDevice9Table.GetHookIndex(D3D9::TABLE::Reset, Hook_Reset);

					device.hook_index(D3D9::TABLE::Reset, Hook_Reset);

					if (!ModelRenderTable.InitTable(Interfaces::ModelRender()))
						return false;

					ModelRenderTable.HookIndex(TABLE::IVModelRender::DrawModelExecute, Hook_DrawModelExecute);

					if (!surfaceCursor.setup(Interfaces::Surface()))
						return false;

					surfaceCursor.hook_index(TABLE::ISurface::LockCursor, Hook_LockCursor);

					if (!clientmode.setup(Interfaces::ClientMode()))
						return false;

					clientmode.hook_index(TABLE::IClientMode::CreateMove, Hook_CreateMove);

					/*if (!soundmode.setup(Interfaces::Sound()))
						return false;

					soundmode.hook_index(5, emit_sound);*/

					

					if (!eventmanager.setup(Interfaces::GameEvent()))
						return false;

					eventmanager.hook_index(TABLE::IGameEventManager2::FireEventClientSide, Hook_FireEventClientSideThink);

					if (!client.setup(Interfaces::Client()))
						return false;

					client.hook_index(TABLE::IBaseClientDLL::hkFrameStageNotify, Hook_FrameStageNotify);

					if (Client::Initialize(g_pDevice))

					return true;

				}
			}

			return false;
		}

		void Shutdown()
		{

			IDirect3DDevice9Table.UnHook();
			ModelRenderTable.UnHook(); 
																	
		    surfaceCursor.unhook_all();
			device.unhook_all();
			clientmode.unhook_all();
			//soundmode.unhook_all();
			eventmanager.unhook_all();
			client.unhook_all();
		}
	}
}