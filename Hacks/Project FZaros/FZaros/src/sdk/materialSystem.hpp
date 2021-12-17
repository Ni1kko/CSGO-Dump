#pragma once
//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//===========================================================================//

//-----------------------------------------------------------------------------
// forward declaraions
//-----------------------------------------------------------------------------

class IMaterialVar;
class ITexture;
class IMaterialProxy;
class Vector;

#include "keyValues.hpp"

//-----------------------------------------------------------------------------
// Flags for GetVertexFormat
//-----------------------------------------------------------------------------
#define	VERTEX_POSITION					0x0001
#define	VERTEX_NORMAL					0x0002
#define	VERTEX_COLOR					0x0004
#define	VERTEX_SPECULAR					0x0008

#define	VERTEX_TANGENT_S				0x0010
#define	VERTEX_TANGENT_T				0x0020
#define	VERTEX_TANGENT_SPACE			( VERTEX_TANGENT_S | VERTEX_TANGENT_T )

// Indicates we're using wrinkle
#define	VERTEX_WRINKLE					0x0040

// Indicates we're using bone indices
#define	VERTEX_BONE_INDEX				0x0080

// Indicates this is a vertex shader
#define	VERTEX_FORMAT_VERTEX_SHADER		0x0100

// Indicates this format shouldn't be bloated to cache align it
// (only used for VertexUsage)
#define	VERTEX_FORMAT_USE_EXACT_FORMAT	0x0200

// Indicates that compressed vertex elements are to be used (see also VertexCompressionType_t)
#define	VERTEX_FORMAT_COMPRESSED		0x400

// Update this if you add or remove bits...
#define	VERTEX_LAST_BIT					10

#define	VERTEX_BONE_WEIGHT_BIT			(VERTEX_LAST_BIT + 1)
#define	USER_DATA_SIZE_BIT				(VERTEX_LAST_BIT + 4)
#define	TEX_COORD_SIZE_BIT				(VERTEX_LAST_BIT + 7)

#define	VERTEX_BONE_WEIGHT_MASK			( 0x7 << VERTEX_BONE_WEIGHT_BIT )
#define	USER_DATA_SIZE_MASK				( 0x7 << USER_DATA_SIZE_BIT )

#define	VERTEX_FORMAT_FIELD_MASK		0x0FF

// If everything is off, it's an unknown vertex format
#define	VERTEX_FORMAT_UNKNOWN			0



//-----------------------------------------------------------------------------
// Macros for construction..
//-----------------------------------------------------------------------------
#define VERTEX_BONEWEIGHT( _n )				((_n) << VERTEX_BONE_WEIGHT_BIT)
#define VERTEX_USERDATA_SIZE( _n )			((_n) << USER_DATA_SIZE_BIT)
#define VERTEX_TEXCOORD_MASK( _coord )		(( 0x7ULL ) << ( TEX_COORD_SIZE_BIT + 3 * (_coord) ))
typedef unsigned __int64		uint64;
typedef uint64 VertexFormat_t;

inline VertexFormat_t VERTEX_TEXCOORD_SIZE(int nIndex, int nNumCoords)
{
	uint64 n64 = nNumCoords;
	uint64 nShift = TEX_COORD_SIZE_BIT + (3 * nIndex);
	return n64 << nShift;
}



//-----------------------------------------------------------------------------
// Gets at various vertex format info...
//-----------------------------------------------------------------------------
inline int VertexFlags(VertexFormat_t vertexFormat)
{
	return static_cast<int> (vertexFormat & ((1 << (VERTEX_LAST_BIT + 1)) - 1));
}

inline int NumBoneWeights(VertexFormat_t vertexFormat)
{
	return static_cast<int> ((vertexFormat >> VERTEX_BONE_WEIGHT_BIT) & 0x7);
}

inline int UserDataSize(VertexFormat_t vertexFormat)
{
	return static_cast<int> ((vertexFormat >> USER_DATA_SIZE_BIT) & 0x7);
}

inline int TexCoordSize(int nTexCoordIndex, VertexFormat_t vertexFormat)
{
	return static_cast<int> ((vertexFormat >> (TEX_COORD_SIZE_BIT + 3 * nTexCoordIndex)) & 0x7);
}

inline bool UsesVertexShader(VertexFormat_t vertexFormat)
{
	return (vertexFormat & VERTEX_FORMAT_VERTEX_SHADER) != 0;
}

// We're testing 2 normal compression methods
// One compressed normals+tangents into a SHORT2 each (8 bytes total)
// The other compresses them together, into a single UBYTE4 (4 bytes total)
// FIXME: pick one or the other, compare lighting quality in important cases
#define COMPRESSED_NORMALS_SEPARATETANGENTS_SHORT2	0
#define COMPRESSED_NORMALS_COMBINEDTANGENTS_UBYTE4	1
//#define COMPRESSED_NORMALS_TYPE						COMPRESSED_NORMALS_SEPARATETANGENTS_SHORT2
#define COMPRESSED_NORMALS_TYPE						COMPRESSED_NORMALS_COMBINEDTANGENTS_UBYTE4


//-----------------------------------------------------------------------------
// Shader state flags can be read from the FLAGS materialvar
// Also can be read or written to with the Set/GetMaterialVarFlags() call
// Also make sure you add/remove a string associated with each flag below to CShaderSystem::ShaderStateString in ShaderSystem.cpp
//-----------------------------------------------------------------------------
enum MaterialVarFlags_t
{
	MATERIAL_VAR_DEBUG = (1 << 0),
	MATERIAL_VAR_NO_DEBUG_OVERRIDE = (1 << 1),
	MATERIAL_VAR_NO_DRAW = (1 << 2),
	MATERIAL_VAR_USE_IN_FILLRATE_MODE = (1 << 3),

	MATERIAL_VAR_VERTEXCOLOR = (1 << 4),
	MATERIAL_VAR_VERTEXALPHA = (1 << 5),
	MATERIAL_VAR_SELFILLUM = (1 << 6),
	MATERIAL_VAR_ADDITIVE = (1 << 7),
	MATERIAL_VAR_ALPHATEST = (1 << 8),
	MATERIAL_VAR_MULTIPASS = (1 << 9),
	MATERIAL_VAR_ZNEARER = (1 << 10),
	MATERIAL_VAR_MODEL = (1 << 11),
	MATERIAL_VAR_FLAT = (1 << 12),
	MATERIAL_VAR_NOCULL = (1 << 13),
	MATERIAL_VAR_NOFOG = (1 << 14),
	MATERIAL_VAR_IGNOREZ = (1 << 15),
	MATERIAL_VAR_DECAL = (1 << 16),
	MATERIAL_VAR_ENVMAPSPHERE = (1 << 17),
	MATERIAL_VAR_NOALPHAMOD = (1 << 18),
	MATERIAL_VAR_ENVMAPCAMERASPACE = (1 << 19),
	MATERIAL_VAR_BASEALPHAENVMAPMASK = (1 << 20),
	MATERIAL_VAR_TRANSLUCENT = (1 << 21),
	MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = (1 << 22),
	MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = (1 << 23),
	MATERIAL_VAR_OPAQUETEXTURE = (1 << 24),
	MATERIAL_VAR_ENVMAPMODE = (1 << 25),
	MATERIAL_VAR_SUPPRESS_DECALS = (1 << 26),
	MATERIAL_VAR_HALFLAMBERT = (1 << 27),
	MATERIAL_VAR_WIREFRAME = (1 << 28),
	MATERIAL_VAR_ALLOWALPHATOCOVERAGE = (1 << 29),
	MATERIAL_VAR_IGNORE_ALPHA_MODULATION = (1 << 30),

	// NOTE: Only add flags here that either should be read from
	// .vmts or can be set directly from client code. Other, internal
	// flags should to into the flag enum in imaterialinternal.h
};


#define TEXTURE_GROUP_LIGHTMAP						        "Lightmaps"
#define TEXTURE_GROUP_WORLD							          "World textures"
#define TEXTURE_GROUP_MODEL							          "Model textures"
#define TEXTURE_GROUP_VGUI							          "VGUI textures"
#define TEXTURE_GROUP_PARTICLE						        "Particle textures"
#define TEXTURE_GROUP_DECAL							          "Decal textures"
#define TEXTURE_GROUP_SKYBOX						          "SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS				      "ClientEffect textures"
#define TEXTURE_GROUP_OTHER							          "Other textures"
#define TEXTURE_GROUP_PRECACHED						        "Precached"
#define TEXTURE_GROUP_CUBE_MAP						        "CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET					      "RenderTargets"
#define TEXTURE_GROUP_UNACCOUNTED					        "Unaccounted textures"
//#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER		  "Static Vertex"
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER			    "Static Indices"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP		"Displacement Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR	"Lighting Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD	"World Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS	"Model Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER	"Other Verts"
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER			  "Dynamic Indices"
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER			  "Dynamic Verts"
#define TEXTURE_GROUP_DEPTH_BUFFER					      "DepthBuffer"
#define TEXTURE_GROUP_VIEW_MODEL					        "ViewModel"
#define TEXTURE_GROUP_PIXEL_SHADERS					      "Pixel Shaders"
#define TEXTURE_GROUP_VERTEX_SHADERS				      "Vertex Shaders"
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE			  "RenderTarget Surfaces"
#define TEXTURE_GROUP_MORPH_TARGETS					      "Morph Targets"



//-----------------------------------------------------------------------------
// Internal flags not accessible from outside the material system. Stored in Flags2
//-----------------------------------------------------------------------------
enum MaterialVarFlags2_t
{
	// NOTE: These are for $flags2!!!!!
	//	UNUSED											= (1 << 0),

	MATERIAL_VAR2_LIGHTING_UNLIT = 0,
	MATERIAL_VAR2_LIGHTING_VERTEX_LIT = (1 << 1),
	MATERIAL_VAR2_LIGHTING_LIGHTMAP = (1 << 2),
	MATERIAL_VAR2_LIGHTING_BUMPED_LIGHTMAP = (1 << 3),
	MATERIAL_VAR2_LIGHTING_MASK =
	(MATERIAL_VAR2_LIGHTING_VERTEX_LIT |
		MATERIAL_VAR2_LIGHTING_LIGHTMAP |
		MATERIAL_VAR2_LIGHTING_BUMPED_LIGHTMAP),

	// FIXME: Should this be a part of the above lighting enums?
	MATERIAL_VAR2_DIFFUSE_BUMPMAPPED_MODEL = (1 << 4),
	MATERIAL_VAR2_USES_ENV_CUBEMAP = (1 << 5),
	MATERIAL_VAR2_NEEDS_TANGENT_SPACES = (1 << 6),
	MATERIAL_VAR2_NEEDS_SOFTWARE_LIGHTING = (1 << 7),
	// GR - HDR path puts lightmap alpha in separate texture...
	MATERIAL_VAR2_BLEND_WITH_LIGHTMAP_ALPHA = (1 << 8),
	MATERIAL_VAR2_NEEDS_BAKED_LIGHTING_SNAPSHOTS = (1 << 9),
	MATERIAL_VAR2_USE_FLASHLIGHT = (1 << 10),
	MATERIAL_VAR2_USE_FIXED_FUNCTION_BAKED_LIGHTING = (1 << 11),
	MATERIAL_VAR2_NEEDS_FIXED_FUNCTION_FLASHLIGHT = (1 << 12),
	MATERIAL_VAR2_USE_EDITOR = (1 << 13),
	MATERIAL_VAR2_NEEDS_POWER_OF_TWO_FRAME_BUFFER_TEXTURE = (1 << 14),
	MATERIAL_VAR2_NEEDS_FULL_FRAME_BUFFER_TEXTURE = (1 << 15),
	MATERIAL_VAR2_IS_SPRITECARD = (1 << 16),
	MATERIAL_VAR2_USES_VERTEXID = (1 << 17),
	MATERIAL_VAR2_SUPPORTS_HW_SKINNING = (1 << 18),
	MATERIAL_VAR2_SUPPORTS_FLASHLIGHT = (1 << 19),
};


//-----------------------------------------------------------------------------
// Preview image return values
//-----------------------------------------------------------------------------
enum PreviewImageRetVal_t
{
	MATERIAL_PREVIEW_IMAGE_BAD = 0,
	MATERIAL_PREVIEW_IMAGE_OK,
	MATERIAL_NO_PREVIEW_IMAGE,
};


enum ImageFormat
{
	IMAGE_FORMAT_UNKNOWN = -1,
	IMAGE_FORMAT_RGBA8888 = 0,
	IMAGE_FORMAT_ABGR8888,
	IMAGE_FORMAT_RGB888,
	IMAGE_FORMAT_BGR888,
	IMAGE_FORMAT_RGB565,
	IMAGE_FORMAT_I8,
	IMAGE_FORMAT_IA88,
	IMAGE_FORMAT_P8,
	IMAGE_FORMAT_A8,
	IMAGE_FORMAT_RGB888_BLUESCREEN,
	IMAGE_FORMAT_BGR888_BLUESCREEN,
	IMAGE_FORMAT_ARGB8888,
	IMAGE_FORMAT_BGRA8888,
	IMAGE_FORMAT_DXT1,
	IMAGE_FORMAT_DXT3,
	IMAGE_FORMAT_DXT5,
	IMAGE_FORMAT_BGRX8888,
	IMAGE_FORMAT_BGR565,
	IMAGE_FORMAT_BGRX5551,
	IMAGE_FORMAT_BGRA4444,
	IMAGE_FORMAT_DXT1_ONEBITALPHA,
	IMAGE_FORMAT_BGRA5551,
	IMAGE_FORMAT_UV88,
	IMAGE_FORMAT_UVWQ8888,
	IMAGE_FORMAT_RGBA16161616F,
	IMAGE_FORMAT_RGBA16161616,
	IMAGE_FORMAT_UVLX8888,
	IMAGE_FORMAT_R32F,			// Single-channel 32-bit floating point
	IMAGE_FORMAT_RGB323232F,	// NOTE: D3D9 does not have this format
	IMAGE_FORMAT_RGBA32323232F,
	IMAGE_FORMAT_RG1616F,
	IMAGE_FORMAT_RG3232F,
	IMAGE_FORMAT_RGBX8888,

	IMAGE_FORMAT_NULL,			// Dummy format which takes no video memory

								// Compressed normal map formats
								IMAGE_FORMAT_ATI2N,			// One-surface ATI2N / DXN format
								IMAGE_FORMAT_ATI1N,			// Two-surface ATI1N format

								IMAGE_FORMAT_RGBA1010102,	// 10 bit-per component render targets
								IMAGE_FORMAT_BGRA1010102,
								IMAGE_FORMAT_R16F,			// 16 bit FP format

															// Depth-stencil texture formats
															IMAGE_FORMAT_D16,
															IMAGE_FORMAT_D15S1,
															IMAGE_FORMAT_D32,
															IMAGE_FORMAT_D24S8,
															IMAGE_FORMAT_LINEAR_D24S8,
															IMAGE_FORMAT_D24X8,
															IMAGE_FORMAT_D24X4S4,
															IMAGE_FORMAT_D24FS8,
															IMAGE_FORMAT_D16_SHADOW,	// Specific formats for shadow mapping
															IMAGE_FORMAT_D24X8_SHADOW,	// Specific formats for shadow mapping

																						// supporting these specific formats as non-tiled for procedural cpu access (360-specific)
																						IMAGE_FORMAT_LINEAR_BGRX8888,
																						IMAGE_FORMAT_LINEAR_RGBA8888,
																						IMAGE_FORMAT_LINEAR_ABGR8888,
																						IMAGE_FORMAT_LINEAR_ARGB8888,
																						IMAGE_FORMAT_LINEAR_BGRA8888,
																						IMAGE_FORMAT_LINEAR_RGB888,
																						IMAGE_FORMAT_LINEAR_BGR888,
																						IMAGE_FORMAT_LINEAR_BGRX5551,
																						IMAGE_FORMAT_LINEAR_I8,
																						IMAGE_FORMAT_LINEAR_RGBA16161616,

																						IMAGE_FORMAT_LE_BGRX8888,
																						IMAGE_FORMAT_LE_BGRA8888,

																						NUM_IMAGE_FORMATS
};

enum MaterialPropertyTypes_t
{
	MATERIAL_PROPERTY_NEEDS_LIGHTMAP = 0,					// bool
	MATERIAL_PROPERTY_OPACITY,								// int (enum MaterialPropertyOpacityTypes_t)
	MATERIAL_PROPERTY_REFLECTIVITY,							// Vector3
	MATERIAL_PROPERTY_NEEDS_BUMPED_LIGHTMAPS				// bool
};

struct MorphFormat_t;

//-----------------------------------------------------------------------------
// material interface
//-----------------------------------------------------------------------------
class IMaterial
{
public:
	// Get the name of the material.  This is a full path to 
	// the vmt file starting from "hl2/materials" (or equivalent) without
	// a file extension.
	virtual const char *	GetName() const = 0;
	virtual const char *	GetTextureGroupName() const = 0;

	// Get the preferred size/bitDepth of a preview image of a material.
	// This is the sort of image that you would use for a thumbnail view
	// of a material, or in WorldCraft until it uses materials to render.
	// separate this for the tools maybe
	virtual PreviewImageRetVal_t GetPreviewImageProperties(int *width, int *height,
		ImageFormat *imageFormat, bool* isTranslucent) const = 0;

	// Get a preview image at the specified width/height and bitDepth.
	// Will do resampling if necessary.(not yet!!! :) )
	// Will do color format conversion. (works now.)
	virtual PreviewImageRetVal_t GetPreviewImage(unsigned char *data,
		int width, int height,
		ImageFormat imageFormat) const = 0;
	// 
	virtual int				GetMappingWidth() = 0;
	virtual int				GetMappingHeight() = 0;

	virtual int				GetNumAnimationFrames() = 0;

	// For material subrects (material pages).  Offset(u,v) and scale(u,v) are normalized to texture.
	virtual bool			InMaterialPage(void) = 0;
	virtual	void			GetMaterialOffset(float *pOffset) = 0;
	virtual void			GetMaterialScale(float *pScale) = 0;
	virtual IMaterial		*GetMaterialPage(void) = 0;

	// find a vmt variable.
	// This is how game code affects how a material is rendered.
	// The game code must know about the params that are used by
	// the shader for the material that it is trying to affect.
	virtual IMaterialVar *	FindVar(const char *varName, bool *found, bool complain = true) = 0;

	// The user never allocates or deallocates materials.  Reference counting is
	// used instead.  Garbage collection is done upon a call to 
	// IMaterialSystem::UncacheUnusedMaterials.
	virtual void			IncrementReferenceCount(void) = 0;
	virtual void			DecrementReferenceCount(void) = 0;

	inline void AddRef() { IncrementReferenceCount(); }
	inline void Release() { DecrementReferenceCount(); }

	// Each material is assigned a number that groups it with like materials
	// for sorting in the application.
	virtual int 			GetEnumerationID(void) const = 0;

	virtual void			GetLowResColorSample(float s, float t, float *color) const = 0;

	// This computes the state snapshots for this material
	virtual void			RecomputeStateSnapshots() = 0;

	// Are we translucent?
	virtual bool			IsTranslucent() = 0;

	// Are we alphatested?
	virtual bool			IsAlphaTested() = 0;

	// Are we vertex lit?
	virtual bool			IsVertexLit() = 0;

	// Gets the vertex format
	virtual VertexFormat_t	GetVertexFormat() const = 0;

	// returns true if this material uses a material proxy
	virtual bool			HasProxy(void) const = 0;

	virtual bool			UsesEnvCubemap(void) = 0;

	virtual bool			NeedsTangentSpace(void) = 0;

	virtual bool			NeedsPowerOfTwoFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool			NeedsFullFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;

	// returns true if the shader doesn't do skinning itself and requires
	// the data that is sent to it to be preskinned.
	virtual bool			NeedsSoftwareSkinning(void) = 0;

	// Apply constant color or alpha modulation
	virtual void			AlphaModulate(float alpha) = 0;
	virtual void			ColorModulate(float r, float g, float b) = 0;

	// Material Var flags...
	virtual void			SetMaterialVarFlag(MaterialVarFlags_t flag, bool on) = 0;
	virtual bool			GetMaterialVarFlag(MaterialVarFlags_t flag) const = 0;

	// Gets material reflectivity
	virtual void			GetReflectivity(Vector& reflect) = 0;

	// Gets material property flags
	virtual bool			GetPropertyFlag(MaterialPropertyTypes_t type) = 0;

	// Is the material visible from both sides?
	virtual bool			IsTwoSided() = 0;

	// Sets the shader associated with the material
	virtual void			SetShader(const char *pShaderName) = 0;

	// Can't be const because the material might have to precache itself.
	virtual int				GetNumPasses(void) = 0;

	// Can't be const because the material might have to precache itself.
	virtual int				GetTextureMemoryBytes(void) = 0;

	// Meant to be used with materials created using CreateMaterial
	// It updates the materials to reflect the current values stored in the material vars
	virtual void			Refresh() = 0;

	// GR - returns true is material uses lightmap alpha for blending
	virtual bool			NeedsLightmapBlendAlpha(void) = 0;

	// returns true if the shader doesn't do lighting itself and requires
	// the data that is sent to it to be prelighted
	virtual bool			NeedsSoftwareLighting(void) = 0;

	// Gets at the shader parameters
	virtual int				ShaderParamCount() const = 0;
	virtual IMaterialVar	**GetShaderParams(void) = 0;

	// Returns true if this is the error material you get back from IMaterialSystem::FindMaterial if
	// the material can't be found.
	virtual bool			IsErrorMaterial() const = 0;

	virtual void			SetUseFixedFunctionBakedLighting(bool bEnable) = 0;

	// Gets the current alpha modulation
	virtual float			GetAlphaModulation() = 0;
	virtual void			GetColorModulation(float *r, float *g, float *b) = 0;

	// Gets the morph format
	virtual MorphFormat_t	GetMorphFormat() const = 0;

	// fast find that stores the index of the found var in the string table in local cache
	virtual IMaterialVar *	FindVarFast(char const *pVarName, unsigned int *pToken) = 0;

	// Sets new VMT shader parameters for the material
	virtual void			SetShaderAndParams(void *pKeyValues) = 0;
	virtual const char *	GetShaderName() const = 0;

	virtual void			DeleteIfUnreferenced() = 0;

	virtual bool			IsSpriteCard() = 0;

	virtual void			CallBindProxy(void *proxyData) = 0;

	virtual IMaterial		*CheckProxyReplacement(void *proxyData) = 0;

	virtual void			RefreshPreservingMaterialVars() = 0;

	virtual bool			WasReloadedFromWhitelist() = 0;

	virtual bool			IsPrecached() const = 0;
};

inline bool IsErrorMaterial(IMaterial *pMat)
{
	return !pMat || pMat->IsErrorMaterial();
}

typedef unsigned short MaterialHandle_t;

class IMaterialSystem
{
public:
	IMaterial* FindMaterial(char const* pMaterialName, const char *pTextureGroupName, bool complain = true, const char *pComplainPrefix = NULL)
	{
		using OriginalFn = VFUNC_SIGNATURE(IMaterial*, char const*, const char*, bool, const char*);
		return ((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[84])(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
	}

	IMaterial*	CreateMaterial(const char *pMaterialName, KeyValues *pVMTKeyValues)
	{
		using OriginalFn = VFUNC_SIGNATURE(IMaterial*, const char*, KeyValues*);
		return ((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[83])(this, pMaterialName, pVMTKeyValues);
	}

	MaterialHandle_t FirstMaterial()
	{
		using OriginalFn = VFUNC_SIGNATURE(MaterialHandle_t);
		return ((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[86])(this);
	}

	MaterialHandle_t NextMaterial(MaterialHandle_t h)
	{
		using OriginalFn = VFUNC_SIGNATURE(MaterialHandle_t, MaterialHandle_t);
		return ((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[87])(this, h);
	}

	MaterialHandle_t InvalidMaterial()
	{
		using OriginalFn = VFUNC_SIGNATURE(MaterialHandle_t);
		return ((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[88])(this);
	}

	IMaterial* GetMaterial(MaterialHandle_t h)
	{
		using OriginalFn = VFUNC_SIGNATURE(IMaterial*, MaterialHandle_t);
		return ((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[89])(this, h);
	}

};
class CStudioHdr;
class DataCacheHandle_t;
class IMatRenderContext;

typedef void* LightCacheHandle_t;
typedef void* StudioDecalHandle_t;
typedef int OverrideType_t;

#define CONCAT_IMPL(x, y) x##y
#define MACRO_CONCAT(x, y) CONCAT_IMPL(x, y)
#define insert_pad(size) byte MACRO_CONCAT(_pad, __COUNTER__)[size]

struct DrawModelInfo_t
{
	studiohdr_t* studio_hdr;
	void* hardware_data;
	char __pad001[ 0x4 ];
	int skin;
	int body;
	int hitbox_set;
	CBaseEntity* entity;
	int lod;
	void* colour_mesh;
	bool static_lighting;
	char __pad002[ 0x8 ];
};

struct studiohwdata_t;
struct MaterialLightingState_t;

struct DrawModelState_t
{
	studiohdr_t*            pStudioHdr;
	studiohwdata_t*         pStudioHWData;
	IClientRenderable*      pRenderable;
	const matrix3x4_t*      pModelToWorld;
	StudioDecalHandle_t     decals;
	int                     drawFlags;
	int                     lod;
};

struct StaticPropRenderInfo_t
{
	const matrix3x4_t*      pModelToWorld;
	const model_t*          pModel;
	IClientRenderable*      pRenderable;
	Vector3*                 pLightingOrigin;
	short                   skin;
	ModelInstanceHandle_t   instance;
};

struct ModelRenderInfo_t
{
	Vector3                  origin;
	Vector3                  angles;
	char pad[0x4];
	IClientRenderable*      pRenderable;
	const model_t*          pModel;
	const matrix3x4_t*      pModelToWorld;
	const matrix3x4_t*      pLightingOffset;
	const Vector*           pLightingOrigin;
	int                     flags;
	int                     entity_index;
	int                     skin;
	int                     body;
	int                     hitboxset;
	ModelInstanceHandle_t   instance;

	ModelRenderInfo_t()
	{
		pModelToWorld = NULL;
		pLightingOffset = NULL;
		pLightingOrigin = NULL;
	}
};

struct LightingQuery_t
{
	Vector3                  LightingOrigin;
	ModelInstanceHandle_t   InstanceHandle;
	bool                    bAmbientBoost;
};

struct StaticLightingQuery_t : public LightingQuery_t
{
	IClientRenderable*        pRenderable;
};

struct ColorMeshInfo_t;

class IVModelRender
{
public:
	virtual int                     DrawModel(int flags, IClientRenderable *pRenderable, ModelInstanceHandle_t instance, int entity_index, const model_t *model, Vector const& origin, Vector const& angles, int skin, int body, int hitboxset, const matrix3x4_t *modelToWorld = NULL, const matrix3x4_t *pLightingOffset = NULL) = 0;
	virtual void                    ForcedMaterialOverride(IMaterial *newMaterial, OverrideType_t nOverrideType = 0, int nOverrides = 0) = 0;
	virtual bool                    IsForcedMaterialOverride(void) = 0; // hey hey!!!!! this is used to fix glow being retarded in DrawModel / DrawModelExecute. I bet you didn't know that! ~alpha
	virtual void                    SetViewTarget(const CStudioHdr *pStudioHdr, int nBodyIndex, const Vector& target) = 0;
	virtual ModelInstanceHandle_t   CreateInstance(IClientRenderable *pRenderable, LightCacheHandle_t *pCache = NULL) = 0;
	virtual void                    DestroyInstance(ModelInstanceHandle_t handle) = 0;
	virtual void                    SetStaticLighting(ModelInstanceHandle_t handle, LightCacheHandle_t* pHandle) = 0;
	virtual LightCacheHandle_t      GetStaticLighting(ModelInstanceHandle_t handle) = 0;
	virtual bool                    ChangeInstance(ModelInstanceHandle_t handle, IClientRenderable *pRenderable) = 0;
	virtual void                    AddDecal(ModelInstanceHandle_t handle, C_Ray const& ray, Vector const& decalUp, int decalIndex, int body, bool noPokeThru, int maxLODToDecal) = 0;
	virtual void                    RemoveAllDecals(ModelInstanceHandle_t handle) = 0;
	virtual bool                    ModelHasDecals(ModelInstanceHandle_t handle) = 0;
	virtual void                    RemoveAllDecalsFromAllModels() = 0;
	virtual matrix3x4_t*            DrawModelShadowSetup(IClientRenderable *pRenderable, int body, int skin, DrawModelInfo_t *pInfo, matrix3x4_t *pCustomBoneToWorld = NULL) = 0;
	virtual void                    DrawModelShadow(IClientRenderable *pRenderable, const DrawModelInfo_t &info, matrix3x4_t *pCustomBoneToWorld = NULL) = 0;
	virtual bool                    RecomputeStaticLighting(ModelInstanceHandle_t handle) = 0;
	virtual void                    ReleaseAllStaticPropColorData(void) = 0;
	virtual void                    RestoreAllStaticPropColorData(void) = 0;
	virtual int                     DrawModelEx(ModelRenderInfo_t &pInfo) = 0;
	virtual int                     DrawModelExStaticProp(ModelRenderInfo_t &pInfo) = 0;
	virtual bool                    DrawModelSetup(ModelRenderInfo_t &pInfo, DrawModelState_t *pState, matrix3x4_t **ppBoneToWorldOut) = 0;
	virtual void                    DrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld = NULL) = 0;
	virtual void                    SetupLighting(const Vector &vecCenter) = 0;
	virtual int                     DrawStaticPropArrayFast(StaticPropRenderInfo_t *pProps, int count, bool bShadowDepth) = 0;
	virtual void                    SuppressEngineLighting(bool bSuppress) = 0;
	virtual void                    SetupColorMeshes(int nTotalVerts) = 0;
	virtual void                    SetupLightingEx(const Vector &vecCenter, ModelInstanceHandle_t handle) = 0;
	virtual bool                    GetBrightestShadowingLightSource(const Vector &vecCenter, Vector& lightPos, Vector& lightBrightness, bool bAllowNonTaggedLights) = 0;
	virtual void                    ComputeLightingState(int nCount, const LightingQuery_t *pQuery, MaterialLightingState_t *pState, ITexture **ppEnvCubemapTexture) = 0;
	virtual void                    GetModelDecalHandles(StudioDecalHandle_t *pDecals, int nDecalStride, int nCount, const ModelInstanceHandle_t *pHandles) = 0;
	virtual void                    ComputeStaticLightingState(int nCount, const StaticLightingQuery_t *pQuery, MaterialLightingState_t *pState, MaterialLightingState_t *pDecalState, ColorMeshInfo_t **ppStaticLighting, ITexture **ppEnvCubemapTexture, DataCacheHandle_t *pColorMeshHandles) = 0;
	virtual void                    CleanupStaticLightingState(int nCount, DataCacheHandle_t *pColorMeshHandles) = 0;
};
