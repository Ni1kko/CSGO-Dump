#pragma once

#include "IAppSystem.hpp"
#include "../misc/Studio.hpp"

class studiohdr_t;
struct studiohwdata_t;
struct vcollide_t;
struct virtualmodel_t;
struct vertexFileHeader_t;

enum MDLCacheDataType_t
{
    // Callbacks to Get called when data is loaded or unloaded for these:
    MDLCACHE_STUDIOHDR = 0,
    MDLCACHE_STUDIOHWDATA,
    MDLCACHE_VCOLLIDE,

    // Callbacks NOT called when data is loaded or unloaded for these:
    MDLCACHE_ANIMBLOCK,
    MDLCACHE_VIRTUALMODEL,
    MDLCACHE_VERTEXES,
    MDLCACHE_DECODEDANIMBLOCK
};

enum MDLCacheFlush_t
{
    MDLCACHE_FLUSH_STUDIOHDR = 0x01,
    MDLCACHE_FLUSH_STUDIOHWDATA = 0x02,
    MDLCACHE_FLUSH_VCOLLIDE = 0x04,
    MDLCACHE_FLUSH_ANIMBLOCK = 0x08,
    MDLCACHE_FLUSH_VIRTUALMODEL = 0x10,
    MDLCACHE_FLUSH_AUTOPLAY = 0x20,
    MDLCACHE_FLUSH_VERTEXES = 0x40,

    MDLCACHE_FLUSH_IGNORELOCK = 0x80000000,
    MDLCACHE_FLUSH_ALL = 0xFFFFFFFF
};

class IMDLCacheNotify
{
public:
    virtual void OnDataLoaded ( MDLCacheDataType_t type, mdl_handle_t handle ) = 0;
    virtual void OnDataUnloaded ( MDLCacheDataType_t type, mdl_handle_t handle ) = 0;
};

class IMDLCache : public IAppSystem
{
public:
    virtual void SetCacheNotify ( IMDLCacheNotify* pNotify ) = 0;
    virtual mdl_handle_t find_mdl ( const char* pMDLRelativePath ) = 0;
    virtual int add_ref ( mdl_handle_t handle ) = 0;
    virtual int release ( mdl_handle_t handle ) = 0;
    virtual int get_ref ( mdl_handle_t handle ) = 0;
    virtual studiohdr_t* get_studio_hdr ( mdl_handle_t handle ) = 0;
    virtual studiohwdata_t* get_hardware_data ( mdl_handle_t handle ) = 0;
    virtual vcollide_t* GetVCollide ( mdl_handle_t handle ) = 0;
    virtual unsigned char* GetAnimBlock ( mdl_handle_t handle, int nBlock ) = 0;
    virtual virtualmodel_t* GetVirtualModel ( mdl_handle_t handle ) = 0;
    virtual int GetAutoplayList ( mdl_handle_t handle, unsigned short** pOut ) = 0;
    virtual vertexFileHeader_t* GetVertexData ( mdl_handle_t handle ) = 0;
    virtual void TouchAllData ( mdl_handle_t handle ) = 0;
    virtual void set_user_data ( mdl_handle_t handle, void* pData ) = 0;
    virtual void* get_user_data ( mdl_handle_t handle ) = 0;
    virtual bool IsErrorModel ( mdl_handle_t handle ) = 0;
    virtual void Flush ( MDLCacheFlush_t nFlushFlags = MDLCACHE_FLUSH_ALL ) = 0;
    virtual void Flush ( mdl_handle_t handle, int nFlushFlags = MDLCACHE_FLUSH_ALL ) = 0;
    virtual const char* get_model_name ( mdl_handle_t handle ) = 0;
    virtual virtualmodel_t* GetVirtualModelFast ( const studiohdr_t* pStudioHdr, mdl_handle_t handle ) = 0;
    virtual void begin_lock ( ) = 0;
    virtual void end_lock ( ) = 0;
    virtual int* GetFrameUnlockCounterPtrOLD ( ) = 0;
    virtual void FinishPendingLoads ( ) = 0;
    virtual vcollide_t* GetVCollideEx ( mdl_handle_t handle, bool synchronousLoad = true ) = 0;
    virtual bool GetVCollideSize ( mdl_handle_t handle, int* pVCollideSize ) = 0;
    virtual bool GetAsyncLoad ( MDLCacheDataType_t type ) = 0;
    virtual bool SetAsyncLoad ( MDLCacheDataType_t type, bool bAsync ) = 0;
    virtual void BeginMapLoad ( ) = 0;
    virtual void EndMapLoad ( ) = 0;
    virtual void MarkAsLoaded ( mdl_handle_t handle ) = 0;
    virtual void InitPreloadData ( bool rebuild ) = 0;
    virtual void ShutdownPreloadData ( ) = 0;
    virtual bool IsDataLoaded ( mdl_handle_t handle, MDLCacheDataType_t type ) = 0;
    virtual int* GetFrameUnlockCounterPtr ( MDLCacheDataType_t type ) = 0;
    virtual studiohdr_t* LockStudioHdr ( mdl_handle_t handle ) = 0;
    virtual void UnlockStudioHdr ( mdl_handle_t handle ) = 0;
    virtual bool PreloadModel ( mdl_handle_t handle ) = 0;
    virtual void ResetErrorModelStatus ( mdl_handle_t handle ) = 0;
    virtual void MarkFrame ( ) = 0;
    virtual void BeginCoarseLock ( ) = 0;
    virtual void EndCoarseLock ( ) = 0;
    virtual void ReloadVCollide ( mdl_handle_t handle ) = 0;
};
