#pragma once

#include "Engine.h"
class CSfxTable;

struct StartSoundParams_t
{
	StartSoundParams_t() :
		staticsound(false),
		userdata(0),
		soundsource(0),
		entchannel(CHAN_AUTO),
		pSfx(0),
		bUpdatePositions(true),
		fvol(1.0f),
		soundlevel(SNDLVL_NORM),
		flags(SND_NOFLAGS),
		pitch(PITCH_NORM),
		fromserver(false),
		delay(0.0f),
		speakerentity(-1),
		suppressrecording(false),
		initialStreamPosition(0)
	{
		origin.Init();
		direction.Init();
	}

	bool			staticsound;
	int				userdata;
	int				soundsource;
	int				entchannel;
	CSfxTable		*pSfx;
	Vector			origin;
	Vector			direction;
	bool			bUpdatePositions;
	float			fvol;
	soundlevel_t	soundlevel;
	int				flags;
	int				pitch;
	bool			fromserver;
	float			delay;
	int				speakerentity;
	bool			suppressrecording;
	int				initialStreamPosition;
};

int S_StartSound(StartSoundParams_t& params);



namespace Engine
{
	namespace Hook
	{
		bool Initialize();
		void Shutdown();

		extern vfunc_hook surfaceCursor;
		extern vfunc_hook panel;
		extern vfunc_hook client;
		extern vfunc_hook clientmode;
		extern vfunc_hook soundmode;
		extern vfunc_hook eventmanager;
		extern vfunc_hook device;

		using FrameStageNotify = void(__thiscall*)(IBaseClientDLL*, ClientFrameStage_t);
		using CreateMove = void(__thiscall *)(IClientMode*, float flInputSampleTime, CUserCmd* pCmd);
		using emitsound_fn = void(__thiscall *)(void* ecx, void* edx, void* filter, int iEntIndex, int iChannel, const char *pSoundEntry, unsigned int nSoundEntryHash, const char *pSample, float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch, const Vector *pOrigin, const Vector *pDirection, Vector * pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, void* params);
		using GetLobbyId = void(__thiscall *)(IClientMode*, float flInputSampleTime, CUserCmd* pCmd);
		using GameEvent = bool(__thiscall*)(void*, IGameEvent*);
		using ResetHook = void(__thiscall *)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
		using DrawModelExecute = void(__thiscall*)(IVModelRender*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
		typedef void(__thiscall* LockCursor)(void*);
		using ModelRender = void(__thiscall *)(IVModelRender* fix, IMatRenderContext* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld);
	}
}