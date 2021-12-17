#pragma once

#include "Utils\Interfaces.h"
#include "SDK\IBaseClientDll.h"
#include "SDK\IClientMode.h"
#include "SDK\ISurface.h"
#include "SDK\IGameEvent.h"
#include "SDK\CInput.h"
#include "SDK\CModelRender.h"
#include "SDK\IVModelInfo.h"
#include "SDK\IMaterial.h"
#include "SDK\IVRenderView.h"
#include "SDK\IVRenderBeams.h"
#include "SDK\CGlowObjectManager.h"
#include <assert.h>


namespace vtable_indexes
{
	constexpr auto view         = 18;
	constexpr auto paint        = 41;
	constexpr auto createMove   = 24;
	constexpr auto lockCursor   = 67;
	constexpr auto frameStage   = 37;
	constexpr auto getviewmodelfov = 35;
	constexpr auto sceneEnd = 9;
	constexpr auto dme = 21;
	constexpr auto extraBonePro = 192;
}

class Event : public IGameEventListener
{
public:
	void FireGameEvent(IGameEvent *event);
	int  GetEventDebugID = 42;
	void Init()
	{
		g_pEventManager->AddListener(this, "player_hurt", false);
		g_pEventManager->AddListener(this, "player_death", false);
		g_pEventManager->AddListener(this, "bullet_impact", false);
		g_pEventManager->AddListener(this, "weapon_fire", false);
		g_pEventManager->AddListener(this, "round_start", false);
		g_pEventManager->AddListener(this, "item_purchase", false);
		
	}
};
extern Event g_Event;

struct impact_info {
	float x, y, z;
	long long time;
};

struct hitmarker_info {
	impact_info impact;
	int alpha;
};

class CAW_hitmaker {
public:
	void paint();


};

extern CAW_hitmaker* hitmarker_2;

extern std::vector<impact_info> impacts;
extern std::vector<hitmarker_info> hitmarkers;

class c_nadepoint {
public:
	c_nadepoint() {
		m_valid = false;
	}

	c_nadepoint(Vector start, Vector end, bool plane, bool valid, Vector normal, bool detonate) {
		m_start = start;
		m_end = end;
		m_plane = plane;
		m_valid = valid;
		m_normal = normal;
		m_detonate = detonate;
	}

	Vector m_start, m_end, m_normal;
	bool m_valid, m_plane, m_detonate;
};

class c_nade_prediction {
	std::array< c_nadepoint, 500 >	_points{ };
	bool		 _predicted = false;

	void predict(CUserCmd *ucmd);

public:
	bool detonated();
	void trace(CUserCmd *ucmd);
	void draw();
};

extern c_nade_prediction g_nadepred;

class CBoneBitList;
class VMTHook;
class ShadowVTManager;
class Hooks
{
public:
    // Initialization setup, called on injection
    static void Init();
    static void Restore();
	static void HookPlayers();

    /*---------------------------------------------*/
    /*-------------Hooked functions----------------*/
    /*---------------------------------------------*/

	static void     __stdcall   FrameStageNotify(ClientFrameStage_t curStage);
    static bool     __fastcall  CreateMove(IClientMode*, void*, float, CUserCmd*);
    static void     __fastcall  LockCursor(ISurface*, void*);
	static void		__fastcall  PaintTraverse(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
	static float __stdcall GetViewmodelFOV();
	static void     __fastcall  OverrideView(void* ecx, void* edx, CViewSetup* pSetup);
	static void     __fastcall  SceneEnd(void *ecx, void *edx);
    static LRESULT  __stdcall   WndProc   (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void     __fastcall  DrawModelExecute(void* ecx, void* edx, IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& render_info, matrix3x4_t* matrix);
	static void     __fastcall  DoExtraBonesProcessing(void * ECX, void * EDX, void * unkn1, void * unkn2, void * unkn3, void * unkn4, CBoneBitList & unkn5, void * unkn6);
	static bool __fastcall SendNetMsg(NetChannel* pNetChan, void* edx, INetMessage& msg, bool bForceReliable, bool bVoice);
private:
    /*---------------------------------------------*/
    /*-------------VMT Hook pointers---------------*/
    /*---------------------------------------------*/

	std::unique_ptr<VMTHook> pClientHook;
    std::unique_ptr<VMTHook> pClientModeHook;
    std::unique_ptr<VMTHook> pSurfaceHook;
	std::unique_ptr<VMTHook> pPanelHook;
	std::unique_ptr<VMTHook> pRenderViewHook;
	std::unique_ptr<ShadowVTManager> pPlayerHook[65];
	std::unique_ptr<VMTHook> pModelHook;

    /*---------------------------------------------*/
    /*-------------Hook prototypes-----------------*/
    /*---------------------------------------------*/

	typedef void (__stdcall*  FrameStageNotify_t) (ClientFrameStage_t);
    typedef bool (__fastcall* CreateMove_t) (IClientMode*, void*, float, CUserCmd*);
    typedef void (__fastcall* LockCursor_t) (ISurface*, void*);
	typedef void (__thiscall* PaintTraverse_t) (PVOID, unsigned int, bool, bool);
	typedef void (__fastcall* OverrideView_t) (void*, void*, CViewSetup*);
	typedef float(__stdcall *oGetViewModelFOV)();
	typedef void(__fastcall* SceneEnd_t) (void*, void*);
	typedef void(__thiscall* DrawModelExecute_t) (void*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
	typedef void(__thiscall* ExtraBoneProcess_t) (void*, void*, void*, void*, void*, CBoneBitList&, void*);
private:
    HWND                           hCSGOWindow             = nullptr; // CSGO window handle
    bool                           bInitializedDrawManager = false;   // Check if we initialized our draw manager
    WNDPROC                        pOriginalWNDProc        = nullptr; // Original CSGO window proc
};

extern Hooks g_Hooks;

class ProtectGuard
{
public:

	ProtectGuard(void *base, uint32_t len, uint32_t protect)
	{
		this->base = base;
		this->len = len;

		if (!VirtualProtect(base, len, protect, (PDWORD)&old_protect))
			throw std::runtime_error("Failed to protect region!");
	}

	~ProtectGuard()
	{
		VirtualProtect(base, len, old_protect, (PDWORD)&old_protect);
	}

private:

	void *base;
	uint32_t len;
	uint32_t old_protect;
};

class VMTHook
{

public:

	VMTHook() : class_base(nullptr), method_count(0), shadow_vtable(nullptr), original_vtable(nullptr) {}
	//VTManager(void *base) : class_base(base), method_count(0), shadow_vtable(nullptr), original_vtable(nullptr) {}

	VMTHook(void* base) { Setup(base); }

	~VMTHook()
	{
		RestoreTable();

		delete[] shadow_vtable;
	}

	inline void Setup(void *base = nullptr)
	{
		if (base != nullptr)
			class_base = base;

		if (class_base == nullptr)
			return;

		original_vtable = *(uintptr_t**)class_base;
		method_count = GetMethodCount(original_vtable);

		if (method_count == 0)
			return;

		shadow_vtable = new uintptr_t[method_count + 1]();

		shadow_vtable[0] = original_vtable[-1];
		std::memcpy(&shadow_vtable[1], original_vtable, method_count * sizeof(uintptr_t));

		try
		{
			auto guard = ProtectGuard{ class_base, sizeof(uintptr_t), PAGE_READWRITE };
			*(uintptr_t**)class_base = &shadow_vtable[1];
		}
		catch (...)
		{
			delete[] shadow_vtable;
		}
	}

	template<typename T>
	inline void Hook(uint32_t index, T method)
	{
		assert(index < method_count);
		shadow_vtable[index + 1] = reinterpret_cast<uintptr_t>(method);
	}

	inline void Unhook(uint32_t index)
	{
		assert(index < method_count);
		shadow_vtable[index + 1] = original_vtable[index];
	}

	template<typename T>
	inline T GetOriginal(uint32_t index)
	{
		return (T)original_vtable[index];
	}

	inline void RestoreTable()
	{
		try
		{
			if (original_vtable != nullptr)
			{
				auto guard = ProtectGuard{ class_base, sizeof(uintptr_t), PAGE_READWRITE };
				*(uintptr_t**)class_base = original_vtable;
				original_vtable = nullptr;
			}
		}
		catch (...) {}
	}

private:

	inline uint32_t GetMethodCount(uintptr_t *vtable_start)
	{
		uint32_t len = -1;

		do ++len; while (vtable_start[len]);

		return len;
	}

	void *class_base;
	uint32_t method_count;
	uintptr_t *shadow_vtable;
	uintptr_t *original_vtable;
};

class ShadowVTManager // GLAD
{

public:

	ShadowVTManager() : class_base(nullptr), method_count(0), shadow_vtable(nullptr), original_vtable(nullptr) {}
	ShadowVTManager(void *base) : class_base(base), method_count(0), shadow_vtable(nullptr), original_vtable(nullptr) {}
	~ShadowVTManager()
	{
		RestoreTable();

		delete[] shadow_vtable;
	}

	inline void Setup(void *base = nullptr)
	{
		if (base != nullptr)
			class_base = base;

		if (class_base == nullptr)
			return;

		original_vtable = *(uintptr_t**)class_base;
		method_count = GetMethodCount(original_vtable);

		if (method_count == 0)
			return;

		shadow_vtable = new uintptr_t[method_count + 1]();

		shadow_vtable[0] = original_vtable[-1];
		std::memcpy(&shadow_vtable[1], original_vtable, method_count * sizeof(uintptr_t));

		try
		{
			auto guard = ProtectGuard{ class_base, sizeof(uintptr_t), PAGE_READWRITE };
			*(uintptr_t**)class_base = &shadow_vtable[1];
		}
		catch (...)
		{
			delete[] shadow_vtable;
		}
	}

	template<typename T>
	inline void Hook(uint32_t index, T method)
	{
		assert(index < method_count);
		shadow_vtable[index + 1] = reinterpret_cast<uintptr_t>(method);
	}

	inline void Unhook(uint32_t index)
	{
		assert(index < method_count);
		shadow_vtable[index + 1] = original_vtable[index];
	}

	template<typename T>
	inline T GetOriginal(uint32_t index)
	{
		return (T)original_vtable[index];
	}

	inline void RestoreTable()
	{
		try
		{
			if (original_vtable != nullptr)
			{
				auto guard = ProtectGuard{ class_base, sizeof(uintptr_t), PAGE_READWRITE };
				*(uintptr_t**)class_base = original_vtable;
				original_vtable = nullptr;
			}
		}
		catch (...) {}
	}

private:

	inline uint32_t GetMethodCount(uintptr_t *vtable_start)
	{
		uint32_t len = -1;

		do ++len; while (vtable_start[len]);

		return len;
	}

	void *class_base;
	uint32_t method_count;
	uintptr_t *shadow_vtable;
	uintptr_t *original_vtable;
};