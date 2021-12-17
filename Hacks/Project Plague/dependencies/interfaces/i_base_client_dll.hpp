#pragma once
#include "../../source-sdk/classes/client_class.hpp"

enum cstrike15_user_message_t {
	cs_um_vguimenu = 1,
	cs_um_geiger = 2,
	cs_um_train = 3,
	cs_um_hudtext = 4,
	cs_um_saytext = 5,
	cs_um_saytext2 = 6,
	cs_um_textmsg = 7,
	cs_um_hudmsg = 8,
	cs_um_resethud = 9,
	cs_um_gametitle = 10,
	cs_um_shake = 12,
	cs_um_fade = 13,
	cs_um_rumble = 14,
	cs_um_closecaption = 15,
	cs_um_closecaptiondirect = 16,
	cs_um_sendaudio = 17,
	cs_um_rawaudio = 18,
	cs_um_voicemask = 19,
	cs_um_requeststate = 20,
	cs_um_damage = 21,
	cs_um_radiotext = 22,
	cs_um_hinttext = 23,
	cs_um_keyhinttext = 24,
	cs_um_processspottedentityupdate = 25,
	cs_um_reloadeffect = 26,
	cs_um_adjustmoney = 27,
	cs_um_updateteammoney = 28,
	cs_um_stopspectatormode = 29,
	cs_um_killcam = 30,
	cs_um_desiredtimescale = 31,
	cs_um_currenttimescale = 32,
	cs_um_achievementevent = 33,
	cs_um_matchendconditions = 34,
	cs_um_disconnecttolobby = 35,
	cs_um_playerstatsupdate = 36,
	cs_um_displayinventory = 37,
	cs_um_warmuphasended = 38,
	cs_um_clientinfo = 39,
	cs_um_xrankget = 40,
	cs_um_xrankupd = 41,
	cs_um_callvotefailed = 45,
	cs_um_votestart = 46,
	cs_um_votepass = 47,
	cs_um_votefailed = 48,
	cs_um_votesetup = 49,
	cs_um_serverrankrevealall = 50,
	cs_um_sendlastkillerdamagetoclient = 51,
	cs_um_serverrankupdate = 52,
	cs_um_itempickup = 53,
	cs_um_showmenu = 54,
	cs_um_bartime = 55,
	cs_um_ammodenied = 56,
	cs_um_markachievement = 57,
	cs_um_matchstatsupdate = 58,
	cs_um_itemdrop = 59,
	cs_um_glowpropturnoff = 60,
	cs_um_sendplayeritemdrops = 61
};

class i_base_client_dll {
public:
	c_client_class* get_client_classes() {
		using original_fn = c_client_class * (__thiscall*)(i_base_client_dll*);
		return (*(original_fn * *)this)[8](this);
	}

	bool dispatch_user_message(int msg_type, unsigned int arg1, unsigned int length, const void* data = nullptr) {
		using original_fn = bool(__thiscall*)(void*, int, unsigned int, unsigned int, const void*);
		return (*(original_fn * *)this)[38](this, msg_type, arg1, length, data);
	}
};

struct _CrtMemState;

#define MEMALLOC_VERSION 1

typedef size_t(*MemAllocFailHandler_t)(size_t);

//-----------------------------------------------------------------------------
// NOTE! This should never be called directly from leaf code
// Just use new,delete,malloc,free etc. They will call into this eventually
//-----------------------------------------------------------------------------
class i_mem_alloc
{
public:
	virtual ~i_mem_alloc();

	// Release versions
	virtual void* alloc(size_t nSize) = 0;
	virtual void* realloc(void* pMem, size_t nSize) = 0;
	virtual void free(void* pMem) = 0;
	virtual void* Expand_NoLongerSupported(void* pMem, size_t nSize) = 0;

	// Debug versions
		//virtual void *Alloc(size_t nSize, const char *pFileName, int nLine) = 0;
		//virtual void *Realloc(void *pMem, size_t nSize, const char *pFileName, int nLine) = 0;
		//virtual void  Free(void *pMem, const char *pFileName, int nLine) = 0;
		//virtual void *Expand_NoLongerSupported(void *pMem, size_t nSize, const char *pFileName, int nLine) = 0;

	// Returns size of a particular allocation
	virtual size_t GetSize(void* pMem) = 0;

	// Force file + line information for an allocation
	virtual void PushAllocDbgInfo(const char* pFileName, int nLine) = 0;
	virtual void PopAllocDbgInfo() = 0;

	// FIXME: Remove when we have our own allocator
	// these methods of the Crt debug code is used in our codebase currently
	virtual long CrtSetBreakAlloc(long lNewBreakAlloc) = 0;
	virtual	int CrtSetReportMode(int nReportType, int nReportMode) = 0;
	virtual int CrtIsValidHeapPointer(const void* pMem) = 0;
	virtual int CrtIsValidPointer(const void* pMem, unsigned int size, int access) = 0;
	virtual int CrtCheckMemory(void) = 0;
	virtual int CrtSetDbgFlag(int nNewFlag) = 0;
	virtual void CrtMemCheckpoint(_CrtMemState* pState) = 0;

	// FIXME: Make a better stats interface
	virtual void DumpStats() = 0;
	virtual void DumpStatsFileBase(char const* pchFileBase) = 0;

	// FIXME: Remove when we have our own allocator
	virtual void* CrtSetReportFile(int nRptType, void* hFile) = 0;
	virtual void* CrtSetReportHook(void* pfnNewHook) = 0;
	virtual int CrtDbgReport(int nRptType, const char* szFile,
		int nLine, const char* szModule, const char* pMsg) = 0;

	virtual int heapchk() = 0;

	virtual bool IsDebugHeap() = 0;

	virtual void GetActualDbgInfo(const char*& pFileName, int& nLine) = 0;
	virtual void RegisterAllocation(const char* pFileName, int nLine, int nLogicalSize, int nActualSize, unsigned nTime) = 0;
	virtual void RegisterDeallocation(const char* pFileName, int nLine, int nLogicalSize, int nActualSize, unsigned nTime) = 0;

	virtual int GetVersion() = 0;

	virtual void CompactHeap() = 0;

	// Function called when malloc fails or memory limits hit to attempt to free up memory (can come in any thread)
	virtual MemAllocFailHandler_t SetAllocFailHandler(MemAllocFailHandler_t pfnMemAllocFailHandler) = 0;

	virtual void DumpBlockStats(void*) = 0;

#if defined( _MEMTEST )	
	virtual void SetStatsExtraInfo(const char* pMapName, const char* pComment) = 0;
#endif

	// Returns 0 if no failure, otherwise the size_t of the last requested chunk
	//  "I'm sure this is completely thread safe!" Brian Deen 7/19/2012.
	virtual size_t MemoryAllocFailed() = 0;

	// handles storing allocation info for coroutines
	virtual int  GetDebugInfoSize() = 0;
	virtual void SaveDebugInfo(void* pvDebugInfo) = 0;
	virtual void RestoreDebugInfo(const void* pvDebugInfo) = 0;
	virtual void InitDebugInfo(void* pvDebugInfo, const char* pchRootFileName, int nLine) = 0;

	// Replacement for ::GlobalMemoryStatus which accounts for unused memory in our system
	virtual void GlobalMemoryStatus(size_t* pUsedMemory, size_t* pFreeMemory) = 0;
};