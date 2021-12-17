#pragma once

#include "MiscDefinitions.h"
#include "ClientRecvProps.h"
#include "offsets.h"
#include "Vector.h"
#include "MiscClasses.h"
#include "Vector2D.h"

struct PreviewImageRetVal_t;
struct MaterialPropertyTypes_t;
class IMaterialVar;
class ImageFormat;

typedef int64_t VertexFormat_t;


#define TEXTURE_GROUP_LIGHTMAP						"Lightmaps"
#define TEXTURE_GROUP_WORLD							"World textures"
#define TEXTURE_GROUP_MODEL							"Model textures"
#define TEXTURE_GROUP_VGUI							"VGUI textures"
#define TEXTURE_GROUP_PARTICLE						"Particle textures"
#define TEXTURE_GROUP_DECAL							"Decal textures"
#define TEXTURE_GROUP_SKYBOX						"SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS				"ClientEffect textures"
#define TEXTURE_GROUP_OTHER							"Other textures"
#define TEXTURE_GROUP_PRECACHED						"Precached"				// TODO: assign texture groups to the precached materials
#define TEXTURE_GROUP_CUBE_MAP						"CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET					"RenderTargets"
#define TEXTURE_GROUP_UNACCOUNTED					"Unaccounted textures"	// Textures that weren't assigned a texture group.
//#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER		"Static Vertex"
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER			"Static Indices"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP		"Displacement Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR	"Lighting Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD	"World Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS	"Model Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER	"Other Verts"
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER			"Dynamic Indices"
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER			"Dynamic Verts"
#define TEXTURE_GROUP_DEPTH_BUFFER					"DepthBuffer"
#define TEXTURE_GROUP_VIEW_MODEL					"ViewModel"
#define TEXTURE_GROUP_PIXEL_SHADERS					"Pixel Shaders"
#define TEXTURE_GROUP_VERTEX_SHADERS				"Vertex Shaders"
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE			"RenderTarget Surfaces"
#define TEXTURE_GROUP_MORPH_TARGETS					"Morph Targets"

class IMaterial
{
public:
	virtual const char *    GetName() const = 0;
	virtual const char *    GetTextureGroupName() const = 0;

	virtual PreviewImageRetVal_t GetPreviewImageProperties(int *width, int *height, ImageFormat *imageFormat, bool* isTranslucent) const = 0;

	virtual PreviewImageRetVal_t GetPreviewImage(unsigned char *data, int width, int height, ImageFormat imageFormat) const = 0;

	virtual int             GetMappingWidth() = 0;
	virtual int             GetMappingHeight() = 0;

	virtual int             GetNumAnimationFrames() = 0;

	virtual bool            InMaterialPage(void) = 0;
	virtual void            GetMaterialOffset(float *pOffset) = 0;
	virtual void            GetMaterialScale(float *pScale) = 0;
	virtual IMaterial       *GetMaterialPage(void) = 0;

	virtual IMaterialVar *  FindVar(const char *varName, bool *found, bool complain = true) = 0;

	virtual void            IncrementReferenceCount(void) = 0;
	virtual void            DecrementReferenceCount(void) = 0;

	inline void AddRef() { IncrementReferenceCount(); }
	inline void Release() { DecrementReferenceCount(); }

	virtual int             GetEnumerationID(void) const = 0;

	virtual void            GetLowResColorSample(float s, float t, float *color) const = 0;

	virtual void            RecomputeStateSnapshots() = 0;

	virtual bool            IsTranslucent() = 0;

	virtual bool            IsAlphaTested() = 0;

	virtual bool            IsVertexLit() = 0;

	virtual VertexFormat_t  GetVertexFormat() const = 0;

	virtual bool            HasProxy(void) const = 0;

	virtual bool            UsesEnvCubemap(void) = 0;

	virtual bool            NeedsTangentSpace(void) = 0;

	virtual bool            NeedsPowerOfTwoFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool            NeedsFullFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;


	void ColorModulation(float r, float g, float b)
	{
		typedef void(__thiscall* ColorModulation_t)(void*, float, float, float);
		return call_vfunc<ColorModulation_t>(this, 28)(this, r, g, b);

	}


	virtual bool            NeedsSoftwareSkinning(void) = 0;

	virtual void            AlphaModulate(float alpha) = 0;
	virtual void            ColorModulate(float r, float g, float b) = 0;

	virtual void            SetMaterialVarFlag(MaterialVarFlags_t flag, bool on) = 0;
	virtual bool            GetMaterialVarFlag(MaterialVarFlags_t flag) = 0;

	virtual void            GetReflectivity(Vector& reflect) = 0;

	virtual bool            GetPropertyFlag(MaterialPropertyTypes_t type) = 0;

	virtual bool            IsTwoSided() = 0;

	virtual void            SetShader(const char *pShaderName) = 0;

	virtual int             GetNumPasses(void) = 0;

	virtual int             GetTextureMemoryBytes(void) = 0;

	virtual void            Refresh() = 0;

	virtual bool            NeedsLightmapBlendAlpha(void) = 0;

	virtual bool            NeedsSoftwareLighting(void) = 0;

	virtual int             ShaderParamCount() const = 0;
	virtual IMaterialVar    **GetShaderParams(void) = 0;

	virtual bool            IsErrorMaterial() const = 0;

	virtual void            Unused() = 0;

	virtual float           GetAlphaModulation() = 0;
	virtual void            GetColorModulation(float *r, float *g, float *b) = 0;

	virtual bool            IsTranslucentUnderModulation(float fAlphaModulation = 1.0f) const = 0;

	virtual IMaterialVar *  FindVarFast(char const *pVarName, unsigned int *pToken) = 0;

	virtual void            SetShaderAndParams(void *pKeyValues) = 0;
	virtual const char *    GetShaderName() const = 0;

	virtual void            DeleteIfUnreferenced() = 0;

	virtual bool            IsSpriteCard() = 0;

	virtual void            CallBindProxy(void *proxyData) = 0;

	virtual void            RefreshPreservingMaterialVars() = 0;

	virtual bool            WasReloadedFromWhitelist() = 0;

	virtual bool            SetTempExcluded(bool bSet, int nExcludedDimensionLimit) = 0;

	virtual int             GetReferenceCount() const = 0;
};

class CMaterialSystem
{
public:

	IMaterial* FindMaterial(char const* pMaterialName, const char *pTextureGroupName, bool complain = true, const char *pComplainPrefix = NULL)
	{
		typedef IMaterial*(__thiscall* oFindMaterial)(PVOID, char const*, char const*, bool, char const*);
		return call_vfunc< oFindMaterial >(this, Offsets::VMT::MaterialSystem_FindMaterial)(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
	}

	IMaterial*	CreateMaterial(const char *pMaterialName, KeyValues *pVMTKeyValues)
	{
		typedef IMaterial* (__thiscall* oCreateMaterial)(PVOID, const char *, KeyValues*);
		return call_vfunc<oCreateMaterial>(this, Offsets::VMT::MaterialSystem_CreateMaterial)(this, pMaterialName, pVMTKeyValues);
	}

	MaterialHandle_t FirstMaterial()
	{
		typedef MaterialHandle_t(__thiscall* FirstMaterialFn)(void*);
		return call_vfunc<FirstMaterialFn>(this, 86)(this);
	}

	MaterialHandle_t NextMaterial(MaterialHandle_t h)
	{
		typedef MaterialHandle_t(__thiscall* NextMaterialFn)(void*, MaterialHandle_t);
		return call_vfunc<NextMaterialFn>(this, 87)(this, h);
	}

	MaterialHandle_t InvalidMaterial()
	{
		typedef MaterialHandle_t(__thiscall* InvalidMaterialFn)(void*);
		return call_vfunc<InvalidMaterialFn>(this, 88)(this);
	}

	IMaterial* GetMaterial(MaterialHandle_t h)
	{
		typedef IMaterial*(__thiscall* GetMaterialFn)(void*, MaterialHandle_t);
		return call_vfunc<GetMaterialFn>(this, 89)(this, h);
	}

};

class IVModelRender
{
public:
	void ForcedMaterialOverride(IMaterial *material, OverrideType_t type = OVERRIDE_NORMAL, int idk = NULL)
	{
		typedef void(__thiscall* Fn)(void*, IMaterial*, OverrideType_t, int);
		return call_vfunc<Fn>(this, Offsets::VMT::ModelRender_ForcedMaterialOverride)(this, material, type, idk);
	}

};

template <typename Fn> __forceinline Fn GetVirtualFunction(void* pClassBase, int nFunctionIndex) {
	return (Fn)((PDWORD)*(PDWORD*)pClassBase)[nFunctionIndex];
}

class CModelInfo
{
public:
	inline void* GetModel(int Index) 
	{
		return GetVirtualFunction<void*(__thiscall *)(void*, int)>(this, 1)(this, Index);
	}
	int	GetModelIndex(const char *name)
	{
		typedef int (__thiscall* oGetModelName)(PVOID, const char *);
		return call_vfunc< oGetModelName >(this, 2)(this, name);
	}
	inline const char* GetModelName(const void* Model) {
		return GetVirtualFunction<const char*(__thiscall *)(void*, const void*)>(this, 3)(this, Model);
	}

	void GetModelMaterials(const model_t *model, int count, IMaterial** ppMaterial)
	{
		typedef void *(__thiscall* oGetModelMaterials)(PVOID, const model_t*, int, IMaterial**);
		call_vfunc<oGetModelMaterials>(this, 19)(this, model, count, ppMaterial);
	}

	studiohdr_t* GetStudiomodel(const model_t* mod)
	{
		typedef studiohdr_t*(__stdcall* oGetStudiomodel)(const model_t*);
		return call_vfunc<oGetStudiomodel>(this, 32)(mod);
	}
};

class CVRenderView
{
public:
	void SetBlend(float alpha)
	{
		typedef void(__thiscall* oDrawModelExecute)(PVOID, float);
		return call_vfunc< oDrawModelExecute >(this, Offsets::VMT::RenderView_SetBlend)(this, alpha);
	}

	void SetColorModulation(float const* colors)
	{
		typedef void(__thiscall* oDrawModelExecute)(PVOID, float const*);
		return call_vfunc< oDrawModelExecute >(this, Offsets::VMT::RenderView_SetColorModulation)(this, colors);
	}
};