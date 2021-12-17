#pragma once
#include "../Include/GlobalIncludes.h"
#include "vmatrix.h"
#include "CTrace.h"
#include "qangle.h"
#include "vfunc.hpp"
typedef unsigned short ModelInstanceHandle_t;
class IMaterial;
class IMatRenderContext;
class IClientRenderable;
struct model_t;
struct studiohdr_t;
struct studiohwdata_t;
struct StudioDecalHandle_t
{
	int unused;
};
struct DrawModelInfo_t;
struct DrawModelState_t
{
	studiohdr_t*			m_pStudioHdr;
	studiohwdata_t*			m_pStudioHWData;
	IClientRenderable*		m_pRenderable;
	const matrix3x4_t*		m_pModelToWorld;
	StudioDecalHandle_t		m_decals;
	int						m_drawFlags;
	int						m_lod;
};
class IClientNetworkable;
class IClientThinkable;
class CollisionProperty
{
	public:
		Vector& VecMins()
		{
			if ( this )
				return *reinterpret_cast<Vector*>( ( DWORD )this + 0x8 );
		}
		Vector& VecMaxs()
		{
			if ( this )
				return *reinterpret_cast<Vector*>( ( DWORD )this + 0x14 );
		}
};
class IClientUnknown
{
	public:
		virtual CollisionProperty*		GetCollideable() = 0;
		virtual IClientNetworkable*	GetClientNetworkable() = 0;
		virtual IClientRenderable*	GetClientRenderable() = 0;
		virtual void*		GetIClientCBaseEntity() = 0;
		virtual void*		GetBaseCBaseEntity() = 0;
		virtual IClientThinkable*	GetClientThinkable() = 0;
};
class IClientThinkable
{
	public:
		virtual IClientUnknown*		GetIClientUnknown() = 0;
		virtual void				ClientThink() = 0;
		virtual void*				GetThinkHandle() = 0;
		virtual void				SetThinkHandle( void* hThink ) = 0;
		virtual void				Release() = 0;
};
class IClientNetworkable
{
public:
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual void			Release() = 0;
	virtual ClientClass* GetClientClass() = 0;// FOR NETVARS FIND YOURSELF ClientClass* stuffs
	virtual void             NotifyShouldTransmit(int state) = 0;
	virtual void             OnPreDataChanged(int updateType) = 0;
	virtual void             OnDataChanged(int updateType) = 0;
	virtual void             PreDataUpdate(int updateType) = 0;
	virtual void             PostDataUpdate(int updateType) = 0;
	virtual void			unknown() = 0;
	virtual bool			IsDormant(void) = 0;
	virtual int				Index(void) const = 0;
	virtual void			ReceiveMessage(int classID /*, bf_read &msg*/) = 0;
	virtual void* GetDataTableBasePtr() = 0;
	virtual void			SetDestroyedOnRecreateEntities(void) = 0;
};
typedef unsigned short ClientShadowHandle_t;
typedef unsigned short ClientRenderHandle_t;
class IClientRenderable
{
	public:
		virtual IClientUnknown*            GetIClientUnknown() = 0;
		virtual Vector const              &GetRenderOrigin( void ) = 0;
		virtual Vector const              &GetRenderAngles( void ) = 0;
		virtual bool                       ShouldDraw( void ) = 0;
		virtual int                        GetRenderFlags( void ) = 0; // ERENDERFLAGS_xxx
		virtual void                       Unused( void ) const {}
		virtual ClientShadowHandle_t       GetShadowHandle() const = 0;
		virtual ClientRenderHandle_t      &RenderHandle() = 0;
		virtual const model_t*             GetModel() const = 0;
		virtual int                        DrawModel( int flags, const int /*RenderableInstance_t*/ &instance ) = 0;
		virtual int                        GetBody() = 0;
		virtual void                       GetColorModulation( float* color ) = 0;
		virtual bool                       LODTest() = 0;
		virtual bool                       SetupBones( matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime ) = 0;
		virtual void                       SetupWeights( const matrix3x4_t* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights ) = 0;
		virtual void                       DoAnimationEvents( void ) = 0;
		virtual void* /*IPVSNotify*/       GetPVSNotifyInterface() = 0;
		virtual void                       GetRenderBounds( Vector &mins, Vector &maxs ) = 0;
		virtual void                       GetRenderBoundsWorldspace( Vector &mins, Vector &maxs ) = 0;
		virtual void                       GetShadowRenderBounds( Vector &mins, Vector &maxs, int /*ShadowType_t*/ shadowType ) = 0;
		virtual bool                       ShouldReceiveProjectedTextures( int flags ) = 0;
		virtual bool                       GetShadowCastDistance( float* pDist, int /*ShadowType_t*/ shadowType ) const = 0;
		virtual bool                       GetShadowCastDirection( Vector* pDirection, int /*ShadowType_t*/ shadowType ) const = 0;
		virtual bool                       IsShadowDirty() = 0;
		virtual void                       MarkShadowDirty( bool bDirty ) = 0;
		virtual IClientRenderable*         GetShadowParent() = 0;
		virtual IClientRenderable*         FirstShadowChild() = 0;
		virtual IClientRenderable*         NextShadowPeer() = 0;
		virtual int /*ShadowType_t*/       ShadowCastType() = 0;
		virtual void                       CreateModelInstance() = 0;
		virtual ModelInstanceHandle_t      GetModelInstance() = 0;
		virtual const matrix3x4_t         &RenderableToWorldTransform() = 0;
		virtual int                        LookupAttachment( const char* pAttachmentName ) = 0;
		virtual   bool                     GetAttachment( int number, Vector &origin, Vector &angles ) = 0;
		virtual bool                       GetAttachment( int number, matrix3x4_t &matrix ) = 0;
		virtual float*                     GetRenderClipPlane( void ) = 0;
		virtual int                        GetSkin() = 0;
		virtual void                       OnThreadedDrawSetup() = 0;
		virtual bool                       UsesFlexDelayedWeights() = 0;
		virtual void                       RecordToolMessage() = 0;
		virtual bool                       ShouldDrawForSplitScreenUser( int nSlot ) = 0;
		virtual uint8_t                      OverrideAlphaModulation( uint8_t nAlpha ) = 0;
		virtual uint8_t                      OverrideShadowAlphaModulation( uint8_t nAlpha ) = 0;
};
class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
	public:
		// Delete yourself.
		virtual void			Release( void ) = 0;

		// Network origin + angles
		virtual const Vector	&GetAbsOrigin( void ) const = 0;
		virtual const Vector	&GetAbsAngles( void ) const = 0;
};
struct StaticPropRenderInfo_t
{
	const matrix3x4_t*      pModelToWorld;
	const model_t*          pModel;
	IClientRenderable*      pRenderable;
	Vector*                 pLightingOrigin;
	short                   skin;
	ModelInstanceHandle_t   instance;
};
typedef int OverrideType_t;


//--- 3/30/18 ---//
struct ModelRenderInfo_t
{
	Vector origin;
	Vector angles;
	char pad[0x4];
	void* pRenderable;
	const model_t* pModel;
	const matrix3x4_t* pModelToWorld;
	const matrix3x4_t* pLightingOffset;
	const Vector* pLightingOrigin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitboxset;
	unsigned short instance;

	ModelRenderInfo_t()
	{
		pModelToWorld = NULL;
		pLightingOffset = NULL;
		pLightingOrigin = NULL;
	}
};

class IClientRenderable;
class IMaterial;
class CStudioHdr;
class IMatRenderContext;
class DataCacheHandle_t;
class ITexture;
class KeyValues;
struct model_t;
struct mstudioanimdesc_t;
struct mstudioseqdesc_t;
struct Ray_t;
struct DrawModelInfo_t;
struct studiohwdata_t;
struct MaterialLightingState_t;
struct ColorMeshInfo_t;

struct LightingQuery_t
{
	Vector                  m_LightingOrigin;
	ModelInstanceHandle_t   m_InstanceHandle;
	bool                    m_bAmbientBoost;
};

struct StaticLightingQuery_t : public LightingQuery_t
{
	IClientRenderable*        m_pRenderable;
};
typedef void* LightCacheHandle_t;
class CModelRender
{
	public:
		virtual int                     DrawModel( int flags, IClientRenderable* pRenderable, ModelInstanceHandle_t instance, int entity_index, const model_t* model, Vector const &origin, QAngle const &angles, int skin, int body, int hitboxset, const matrix3x4_t* modelToWorld = NULL, const matrix3x4_t* pLightingOffset = NULL ) = 0;
		virtual void                    ForcedMaterialOverride( IMaterial* newMaterial, OverrideType_t nOverrideType = 0, int nOverrides = 0 ) = 0;
		virtual bool                    IsForcedMaterialOverride( void ) = 0;
		virtual void                    SetViewTarget( const studiohdr_t* pStudioHdr, int nBodyIndex, const Vector &target ) = 0;
		virtual ModelInstanceHandle_t   CreateInstance( IClientRenderable* pRenderable, LightCacheHandle_t* pCache = NULL ) = 0;
		virtual void                    DestroyInstance( ModelInstanceHandle_t handle ) = 0;
		virtual void                    SetStaticLighting( ModelInstanceHandle_t handle, LightCacheHandle_t* pHandle ) = 0;
		virtual LightCacheHandle_t      GetStaticLighting( ModelInstanceHandle_t handle ) = 0;
		virtual bool                    ChangeInstance( ModelInstanceHandle_t handle, IClientRenderable* pRenderable ) = 0;
		virtual void                    AddDecal( ModelInstanceHandle_t handle, Ray_t const &ray, Vector const &decalUp, int decalIndex, int body, bool noPokeThru, int maxLODToDecal ) = 0;
		virtual void                    RemoveAllDecals( ModelInstanceHandle_t handle ) = 0;
		virtual bool                    ModelHasDecals( ModelInstanceHandle_t handle ) = 0;
		virtual void                    RemoveAllDecalsFromAllModels() = 0;
		virtual matrix3x4_t*            DrawModelShadowSetup( IClientRenderable* pRenderable, int body, int skin, DrawModelInfo_t* pInfo, matrix3x4_t* pCustomBoneToWorld = NULL ) = 0;
		virtual void                    DrawModelShadow( IClientRenderable* pRenderable, const DrawModelInfo_t &info, matrix3x4_t* pCustomBoneToWorld = NULL ) = 0;
		virtual bool                    RecomputeStaticLighting( ModelInstanceHandle_t handle ) = 0;
		virtual void                    ReleaseAllStaticPropColorData( void ) = 0;
		virtual void                    RestoreAllStaticPropColorData( void ) = 0;
		virtual int                     DrawModelEx( ModelRenderInfo_t &pInfo ) = 0;
		virtual int                     DrawModelExStaticProp( ModelRenderInfo_t &pInfo ) = 0;
		virtual bool                    DrawModelSetup( ModelRenderInfo_t &pInfo, DrawModelState_t* pState, matrix3x4_t** ppBoneToWorldOut ) = 0;
		virtual void                    DrawModelExecute( IMatRenderContext* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t* pCustomBoneToWorld = NULL ) = 0;
		virtual void                    SetupLighting( const Vector &vecCenter ) = 0;
		virtual int                     DrawStaticPropArrayFast( StaticPropRenderInfo_t* pProps, int count, bool bShadowDepth ) = 0;
		virtual void                    SuppressEngineLighting( bool bSuppress ) = 0;
		virtual void                    SetupColorMeshes( int nTotalVerts ) = 0;
		virtual void                    SetupLightingEx( const Vector &vecCenter, ModelInstanceHandle_t handle ) = 0;
		virtual bool                    GetBrightestShadowingLightSource( const Vector &vecCenter, Vector &lightPos, Vector &lightBrightness, bool bAllowNonTaggedLights ) = 0;
		virtual void                    ComputeLightingState( int nCount, const LightingQuery_t* pQuery, MaterialLightingState_t* pState, ITexture** ppEnvCubemapTexture ) = 0;
		virtual void                    GetModelDecalHandles( StudioDecalHandle_t* pDecals, int nDecalStride, int nCount, const ModelInstanceHandle_t* pHandles ) = 0;
		virtual void                    ComputeStaticLightingState( int nCount, const StaticLightingQuery_t* pQuery, MaterialLightingState_t* pState, MaterialLightingState_t* pDecalState, ColorMeshInfo_t** ppStaticLighting, ITexture** ppEnvCubemapTexture, DataCacheHandle_t* pColorMeshHandles ) = 0;
		virtual void                    CleanupStaticLightingState( int nCount, DataCacheHandle_t* pColorMeshHandles ) = 0;

};
