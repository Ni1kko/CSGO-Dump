#pragma once
#include "..\Utils\Utils.h"
enum OverrideType_t1
{
	OVERRIDE_NORMAL = 0,
	OVERRIDE_BUILD_SHADOWS,
	OVERRIDE_DEPTH_WRITE,
};


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
};

typedef unsigned short ModelInstanceHandle_t;
typedef void* LightCacheHandle_t;
typedef void* StudioDecalHandle_t;
typedef int ImageFormat;
typedef int VertexFormat_t;
//typedef int MaterialPropertyTypes_t;
struct PreviewImageRetVal_t;
struct MaterialPropertyTypes_t;
typedef unsigned short MaterialHandle_t;
typedef int OverrideType_t;
class IClientRenderable;
class IMaterial;
class CStudioHdr;
class IMatRenderContext;
class DataCacheHandle_t;
class ITexture;
class IMaterialVar;
struct model_t;
struct mstudioanimdesc_t;
struct mstudioseqdesc_t {};
struct Ray_t;
struct DrawModelInfo_t;
struct studiohwdata_t;
struct MaterialLightingState_t;
struct ColorMeshInfo_t;


class Quaternion
{
public:
	float x, y, z, w;
	float operator[](int i) const { if (i == 1) return x; if (i == 2) return y; if (y == 3) return z; return w; };
	float& operator[](int i) { if (i == 1) return x; if (i == 2) return y; if (y == 3) return z; return w; };
};

class RadianEuler
{
public:
	float x, y, z;
};

struct mstudiobbox_t
{
	int					bone;
	int					group;				// intersection group
	Vector				bbmin;				// bounding box
	Vector				bbmax;
	int					szhitboxnameindex;	// offset to the name of the hitbox.
	int					unused[3];
	float				radius;
	int					unused2[4];

	const char* pszHitboxName()
	{
		if (szhitboxnameindex == 0)
			return "";

		return ((char*)this) + szhitboxnameindex;
	}

	mstudiobbox_t() {}

private:
	mstudiobbox_t(const mstudiobbox_t& vOther);
};

struct mstudiobone_t
{
	int					sznameindex;
	inline char * const pszName(void) const { return ((char *)this) + sznameindex; }
	int		 			parent;		// parent bone
	int					bonecontroller[6];	// bone controller index, -1 == none

											// default values
	Vector				pos;
	Quaternion			quat;
	RadianEuler			rot;
	// compression scale
	Vector				posscale;
	Vector				rotscale;

	matrix3x4_t			poseToBone;
	Quaternion			qAlignment;
	int					flags;
	int					proctype;
	int					procindex;		// procedural rule
	mutable int			physicsbone;	// index into physically simulated bone
	inline void *pProcedure() const { if (procindex == 0) return NULL; else return  (void *)(((byte *)this) + procindex); };
	int					surfacepropidx;	// index into string tablefor property name
	inline char * const pszSurfaceProp(void) const { return ((char *)this) + surfacepropidx; }
	int					contents;		// See BSPFlags.h for the contents flags

	int					unused[8];		// remove as appropriate

	mstudiobone_t() {}
private:
	// No copy constructors allowed
	mstudiobone_t(const mstudiobone_t& vOther);
};

struct mstudiohitboxset_t
{
	int	sznameindex;
	int	numhitboxes;
	int	hitboxindex;

	inline mstudiobbox_t* GetHitbox(int i) const
	{
		return (mstudiobbox_t*)(((DWORD)this) + hitboxindex) + i;
	};
};

struct studiohdr_t
{
	int	id;
	int	version;

	long checksum;	// this has to be the same in the phy and vtx files to load!

	char name[64];
	int	length;

	Vector	eyeposition;	// ideal eye position

	Vector	illumposition;	// illumination center

	Vector	hull_min;	// ideal movement hull size
	Vector	hull_max;

	Vector	view_bbmin;	// clipping bounding box
	Vector	view_bbmax;

	int	flags;

	int	numbones;	// bones
	int	boneindex;
	inline mstudiobone_t *pBone(int i) const { return (mstudiobone_t *)(((byte *)this) + boneindex) + i; };

	int	numbonecontrollers;	// bone controllers
	int	bonecontrollerindex;
	inline void *pBonecontroller(int i) const { return (((byte *)this) + bonecontrollerindex) + i; };

	int	numhitboxsets;
	int	hitboxsetindex;

	// Look up hitbox set by index
	mstudiohitboxset_t  *pHitboxSet(int i) const
	{
		return (mstudiohitboxset_t *)(((byte *)this) + hitboxsetindex) + i;
	};

	// Calls through to hitbox to determine size of specified set
	inline mstudiobbox_t *pHitbox(int i, int set) const
	{
		mstudiohitboxset_t *s = pHitboxSet(set);

		if (!s)
			return NULL;

		return s->GetHitbox(i);
	};

	// Calls through to set to get hitbox count for set
	inline int  iHitboxCount(int set) const
	{
		mstudiohitboxset_t const *s = pHitboxSet(set);
		if (!s)
			return 0;

		return s->numhitboxes;
	};

	int	numanim;	// animations/poses
	int	animdescindex;	// animation descriptions
	inline void *pAnimdesc(int i) const { return (((byte *)this) + animdescindex) + i; };

	int 	numanimgroup;
	int 	animgroupindex;
	inline  void *pAnimGroup(int i) const { return (((byte *)this) + animgroupindex) + i; };

	int 	numbonedesc;
	int 	bonedescindex;
	inline  void *pBoneDesc(int i) const { return (((byte *)this) + bonedescindex) + i; };

	int	numseq;		// sequences
	int	seqindex;
	inline mstudioseqdesc_t *pSeqdesc(int i) const { if (i < 0 || i >= numseq) i = 0; return (mstudioseqdesc_t *)(((byte *)this) + seqindex) + i; };
	int	sequencesindexed;	// initialization flag - have the sequences been indexed?

	int	numseqgroups;		// demand loaded sequences
	int	seqgroupindex;
	inline  void *pSeqgroup(int i) const { return (((byte *)this) + seqgroupindex) + i; };

	int	numtextures;		// raw textures
	int	textureindex;
	inline void *pTexture(int i) const { return (((byte *)this) + textureindex) + i; };

	int	numcdtextures;		// raw textures search paths
	int	cdtextureindex;
	inline char			*pCdtexture(int i) const { return (((char *)this) + *((int *)(((byte *)this) + cdtextureindex) + i)); };

	int	numskinref;		// replaceable textures tables
	int	numskinfamilies;
	int	skinindex;
	inline short		*pSkinref(int i) const { return (short *)(((byte *)this) + skinindex) + i; };

	int	numbodyparts;
	int	bodypartindex;
	inline void	*pBodypart(int i) const { return (((byte *)this) + bodypartindex) + i; };

	int	numattachments;		// queryable attachable points
	int	attachmentindex;
	inline void	*pAttachment(int i) const { return (((byte *)this) + attachmentindex) + i; };

	int	numtransitions;		// animation node to animation node transition graph
	int	transitionindex;
	inline byte	*pTransition(int i) const { return (byte *)(((byte *)this) + transitionindex) + i; };

	int	numflexdesc;
	int	flexdescindex;
	inline void *pFlexdesc(int i) const { return (((byte *)this) + flexdescindex) + i; };

	int	numflexcontrollers;
	int	flexcontrollerindex;
	inline void *pFlexcontroller(int i) const { return (((byte *)this) + flexcontrollerindex) + i; };

	int	numflexrules;
	int	flexruleindex;
	inline void *pFlexRule(int i) const { return (((byte *)this) + flexruleindex) + i; };

	int	numikchains;
	int	ikchainindex;
	inline void *pIKChain(int i) const { return (((byte *)this) + ikchainindex) + i; };

	int	nummouths;
	int	mouthindex;
	inline void *pMouth(int i) const { return (((byte *)this) + mouthindex) + i; };

	int	numposeparameters;
	int	poseparamindex;
	inline void *pPoseParameter(int i) const { return (((byte *)this) + poseparamindex) + i; };

	int	surfacepropindex;
	inline char * const pszSurfaceProp(void) const { return ((char *)this) + surfacepropindex; }

	// Key values
	int	keyvalueindex;
	int	keyvaluesize;
	inline const char * KeyValueText(void) const { return keyvaluesize != 0 ? ((char *)this) + keyvalueindex : NULL; }

	int	numikautoplaylocks;
	int	ikautoplaylockindex;
	inline void *pIKAutoplayLock(int i) const { return (((byte *)this) + ikautoplaylockindex) + i; };

	float mass;		// The collision model mass that jay wanted
	int	contents;
	int	unused[9];	// remove as appropriate
};

struct DrawModelState_t
{
	studiohdr_t*            m_pStudioHdr;
	studiohwdata_t*         m_pStudioHWData;
	IClientRenderable*      m_pRenderable;
	const matrix3x4_t*      m_pModelToWorld;
	StudioDecalHandle_t     m_decals;
	int                     m_drawFlags;
	int                     m_lod;
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

struct ModelRenderInfo_t
{
	Vector                  origin;
	QAngle                  angles;
	char                    pad[4];
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
	Vector                  m_LightingOrigin;
	ModelInstanceHandle_t   m_InstanceHandle;
	bool                    m_bAmbientBoost;
};

struct StaticLightingQuery_t : public LightingQuery_t
{
	IClientRenderable*        m_pRenderable;
};

class IMaterial
{
public:
	virtual const char*             GetName() const = 0;
	virtual const char*             GetTextureGroupName() const = 0;
	virtual PreviewImageRetVal_t    GetPreviewImageProperties(int *width, int *height, ImageFormat *imageFormat, bool* isTranslucent) const = 0;
	virtual PreviewImageRetVal_t    GetPreviewImage(unsigned char *data, int width, int height, ImageFormat imageFormat) const = 0;
	virtual int                     GetMappingWidth() = 0;
	virtual int                     GetMappingHeight() = 0;
	virtual int                     GetNumAnimationFrames() = 0;
	virtual bool                    InMaterialPage(void) = 0;
	virtual    void                 GetMaterialOffset(float *pOffset) = 0;
	virtual void                    GetMaterialScale(float *pScale) = 0;
	virtual IMaterial*              GetMaterialPage(void) = 0;
	virtual IMaterialVar*           FindVar(const char *varName, bool *found, bool complain = true) = 0;
	virtual void                    IncrementReferenceCount(void) = 0;
	virtual void                    DecrementReferenceCount(void) = 0;
	inline void                     AddRef() { IncrementReferenceCount(); }
	inline void                     Release() { DecrementReferenceCount(); }
	virtual int                     GetEnumerationID(void) const = 0;
	virtual void                    GetLowResColorSample(float s, float t, float *color) const = 0;
	virtual void                    RecomputeStateSnapshots() = 0;
	virtual bool                    IsTranslucent() = 0;
	virtual bool                    IsAlphaTested() = 0;
	virtual bool                    IsVertexLit() = 0;
	virtual VertexFormat_t          GetVertexFormat() const = 0;
	virtual bool                    HasProxy(void) const = 0;
	virtual bool                    UsesEnvCubemap(void) = 0;
	virtual bool                    NeedsTangentSpace(void) = 0;
	virtual bool                    NeedsPowerOfTwoFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool                    NeedsFullFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool                    NeedsSoftwareSkinning(void) = 0;
	virtual void                    AlphaModulate(float alpha) = 0;
	virtual void                    ColorModulate(float r, float g, float b) = 0;
	virtual void                    SetMaterialVarFlag(MaterialVarFlags_t flag, bool on) = 0;
	virtual bool                    GetMaterialVarFlag(MaterialVarFlags_t flag) const = 0;
	virtual void                    GetReflectivity(Vector& reflect) = 0;
	virtual bool                    GetPropertyFlag(MaterialPropertyTypes_t type) = 0;
	virtual bool                    IsTwoSided() = 0;
	virtual void                    SetShader(const char *pShaderName) = 0;
	virtual int                     GetNumPasses(void) = 0;
	virtual int                     GetTextureMemoryBytes(void) = 0;
	virtual void                    Refresh() = 0;
	virtual bool                    NeedsLightmapBlendAlpha(void) = 0;
	virtual bool                    NeedsSoftwareLighting(void) = 0;
	virtual int                     ShaderParamCount() const = 0;
	virtual IMaterialVar**          GetShaderParams(void) = 0;
	virtual bool                    IsErrorMaterial() const = 0;
	virtual void                    Unused() = 0;
	virtual float                   GetAlphaModulation() = 0;
	virtual void                    GetColorModulation(float *r, float *g, float *b) = 0;
	virtual bool                    IsTranslucentUnderModulation(float fAlphaModulation = 1.0f) const = 0;
	virtual IMaterialVar*           FindVarFast(char const *pVarName, unsigned int *pToken) = 0;
	virtual void                    SetShaderAndParams(KeyValues *pKeyValues) = 0;
	virtual const char*             GetShaderName() const = 0;
	virtual void                    DeleteIfUnreferenced() = 0;
	virtual bool                    IsSpriteCard() = 0;
	virtual void                    CallBindProxy(void *proxyData) = 0;
	virtual void                    RefreshPreservingMaterialVars() = 0;
	virtual bool                    WasReloadedFromWhitelist() = 0;
	virtual bool                    SetTempExcluded(bool bSet, int nExcludedDimensionLimit) = 0;
	virtual int                     GetReferenceCount() const = 0;
};

class IMaterialProxyFactory;
class ITexture;
class IMaterialSystemHardwareConfig;
class CShadowMgr;

struct MaterialSystem_SortInfo_t
{
	IMaterial	*material;
	int			lightmapPageID;
};

enum MaterialThreadMode_t
{
	MATERIAL_SINGLE_THREADED,
	MATERIAL_QUEUED_SINGLE_THREADED,
	MATERIAL_QUEUED_THREADED
};

enum MaterialContextType_t
{
	MATERIAL_HARDWARE_CONTEXT,
	MATERIAL_QUEUED_CONTEXT,
	MATERIAL_NULL_CONTEXT
};

enum
{
	MATERIAL_ADAPTER_NAME_LENGTH = 512
};

struct ApplicationInstantCountersInfo_t
{
	uint32_t m_nCpuActivityMask;
	uint32_t m_nDeferredWordsAllocated;
};
struct MaterialAdapterInfo_t
{
	char m_pDriverName[MATERIAL_ADAPTER_NAME_LENGTH];
	unsigned int m_VendorID;
	unsigned int m_DeviceID;
	unsigned int m_SubSysID;
	unsigned int m_Revision;
	int m_nDXSupportLevel;			// This is the *preferred* dx support level
	int m_nMinDXSupportLevel;
	int m_nMaxDXSupportLevel;
	unsigned int m_nDriverVersionHigh;
	unsigned int m_nDriverVersionLow;
};

struct MaterialVideoMode_t
{
	int m_Width;			// if width and height are 0 and you select 
	int m_Height;			// windowed mode, it'll use the window size
	ImageFormat m_Format;	// use ImageFormats (ignored for windowed mode)
	int m_RefreshRate;		// 0 == default (ignored for windowed mode)
};

struct MaterialSystem_Config_t;

typedef void(*MaterialBufferReleaseFunc_t)(int nChangeFlags);	// see RestoreChangeFlags_t
typedef void(*MaterialBufferRestoreFunc_t)(int nChangeFlags);	// see RestoreChangeFlags_t
typedef void(*ModeChangeCallbackFunc_t)(void);
typedef void(*EndFrameCleanupFunc_t)(void);
typedef bool(*EndFramePriorToNextContextFunc_t)(void);
typedef void(*OnLevelShutdownFunc_t)(void *pUserData);
struct MaterialSystemHWID_t;
struct AspectRatioInfo_t;

enum HDRType_t
{
	HDR_TYPE_NONE,
	HDR_TYPE_INTEGER,
	HDR_TYPE_FLOAT,
};

enum RenderTargetSizeMode_t
{
	RT_SIZE_NO_CHANGE = 0,
	RT_SIZE_DEFAULT = 1,
	RT_SIZE_PICMIP = 2,
	RT_SIZE_HDR = 3,
	RT_SIZE_FULL_FRAME_BUFFER = 4,
	RT_SIZE_OFFSCREEN = 5,
	RT_SIZE_FULL_FRAME_BUFFER_ROUNDED_UP = 6
};


enum MaterialRenderTargetDepth_t
{
	MATERIAL_RT_DEPTH_SHARED = 0x0,
	MATERIAL_RT_DEPTH_SEPARATE = 0x1,
	MATERIAL_RT_DEPTH_NONE = 0x2,
	MATERIAL_RT_DEPTH_ONLY = 0x3,
};

enum CompiledVtfFlags
{
	TEXTUREFLAGS_POINTSAMPLE = 0x00000001,
	TEXTUREFLAGS_TRILINEAR = 0x00000002,
	TEXTUREFLAGS_CLAMPS = 0x00000004,
	TEXTUREFLAGS_CLAMPT = 0x00000008,
	TEXTUREFLAGS_ANISOTROPIC = 0x00000010,
	TEXTUREFLAGS_HINT_DXT5 = 0x00000020,
	TEXTUREFLAGS_PWL_CORRECTED = 0x00000040,
	TEXTUREFLAGS_NORMAL = 0x00000080,
	TEXTUREFLAGS_NOMIP = 0x00000100,
	TEXTUREFLAGS_NOLOD = 0x00000200,
	TEXTUREFLAGS_ALL_MIPS = 0x00000400,
	TEXTUREFLAGS_PROCEDURAL = 0x00000800,
	TEXTUREFLAGS_ONEBITALPHA = 0x00001000,
	TEXTUREFLAGS_EIGHTBITALPHA = 0x00002000,
	TEXTUREFLAGS_ENVMAP = 0x00004000,
	TEXTUREFLAGS_RENDERTARGET = 0x00008000,
	TEXTUREFLAGS_DEPTHRENDERTARGET = 0x00010000,
	TEXTUREFLAGS_NODEBUGOVERRIDE = 0x00020000,
	TEXTUREFLAGS_SINGLECOPY = 0x00040000,
	TEXTUREFLAGS_PRE_SRGB = 0x00080000,
	TEXTUREFLAGS_UNUSED_00100000 = 0x00100000,
	TEXTUREFLAGS_UNUSED_00200000 = 0x00200000,
	TEXTUREFLAGS_UNUSED_00400000 = 0x00400000,
	TEXTUREFLAGS_NODEPTHBUFFER = 0x00800000,
	TEXTUREFLAGS_UNUSED_01000000 = 0x01000000,
	TEXTUREFLAGS_CLAMPU = 0x02000000,
	TEXTUREFLAGS_VERTEXTEXTURE = 0x04000000,
	TEXTUREFLAGS_SSBUMP = 0x08000000,
	TEXTUREFLAGS_UNUSED_10000000 = 0x10000000,
	TEXTUREFLAGS_BORDER = 0x20000000,
	TEXTUREFLAGS_UNUSED_40000000 = 0x40000000,
	TEXTUREFLAGS_UNUSED_80000000 = 0x80000000
};

class IShader;

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

template< typename Function > Function call_vfunc3(PVOID Base, DWORD Index)
{
	PDWORD* VTablePointer = (PDWORD*)Base;
	PDWORD VTableFunctionBase = *VTablePointer;
	DWORD dwAddress = VTableFunctionBase[Index];
	return (Function)(dwAddress);
}

class CMaterialSystem
{
public:
	IMaterial* FindMaterial(char const* pMaterialName, const char *pTextureGroupName, bool complain = true, const char *pComplainPrefix = NULL)
	{
		typedef IMaterial*(__thiscall* oFindMaterial)(PVOID, char const*, char const*, bool, char const*);
		return call_vfunc3< oFindMaterial >(this, 84)(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
	}
	IMaterial*	CreateMaterial(const char *pMaterialName, KeyValues *pVMTKeyValues)
	{
		typedef IMaterial* (__thiscall* oCreateMaterial)(PVOID, const char *, KeyValues*);
		return call_vfunc3<oCreateMaterial>(this, 83)(this, pMaterialName, pVMTKeyValues);
	}
	MaterialHandle_t FirstMaterial()
	{
		typedef MaterialHandle_t(__thiscall* Fn)(void*);
		return Utils::GetVFunc<Fn>(this, 86)(this);
	}

	MaterialHandle_t NextMaterial(MaterialHandle_t h)
	{
		typedef MaterialHandle_t(__thiscall* Fn)(void*, MaterialHandle_t);
		return Utils::GetVFunc<Fn>(this, 87)(this, h);
	}

	MaterialHandle_t InvalidMaterial()
	{
		typedef MaterialHandle_t(__thiscall* Fn)(void*);
		return Utils::GetVFunc<Fn>(this, 88)(this);
	}
	IMaterial* GetMaterial(MaterialHandle_t h)
	{
		typedef IMaterial*(__thiscall* Fn)(void*, MaterialHandle_t);
		return Utils::GetVFunc<Fn>(this, 89)(this, h);
	}
};
extern CMaterialSystem* g_pMaterialSystem;

class IVModelRender
{
public:
	virtual int                     DrawModel(int flags, IClientRenderable *pRenderable, ModelInstanceHandle_t instance, int entity_index, const model_t *model, Vector const& origin, QAngle const& angles, int skin, int body, int hitboxset, const matrix3x4_t *modelToWorld = NULL, const matrix3x4_t *pLightingOffset = NULL) = 0;
	virtual void                    ForcedMaterialOverride(IMaterial *newMaterial, OverrideType_t nOverrideType = 0, int nOverrides = 0) = 0;
	virtual bool                    IsForcedMaterialOverride(void) = 0;
	virtual void                    SetViewTarget(const CStudioHdr *pStudioHdr, int nBodyIndex, const Vector& target) = 0;
	virtual ModelInstanceHandle_t   CreateInstance(IClientRenderable *pRenderable, LightCacheHandle_t *pCache = NULL) = 0;
	virtual void                    DestroyInstance(ModelInstanceHandle_t handle) = 0;
	virtual void                    SetStaticLighting(ModelInstanceHandle_t handle, LightCacheHandle_t* pHandle) = 0;
	virtual LightCacheHandle_t      GetStaticLighting(ModelInstanceHandle_t handle) = 0;
	virtual bool                    ChangeInstance(ModelInstanceHandle_t handle, IClientRenderable *pRenderable) = 0;
	virtual void                    AddDecal(ModelInstanceHandle_t handle, Ray_t const& ray, Vector const& decalUp, int decalIndex, int body, bool noPokeThru, int maxLODToDecal) = 0;
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
extern IVModelRender* g_pModelRender;

typedef unsigned short LeafIndex_t;

struct WorldListLeafData_t
{
	LeafIndex_t     leafIndex;    // 16 bits
	int16_t         waterData;
	uint16_t        firstTranslucentSurface;    // engine-internal list index
	uint16_t        translucentSurfaceCount;    // count of translucent surfaces+disps
};

struct VisibleFogVolumeInfo_t
{
	int            m_nVisibleFogVolume;
	int            m_nVisibleFogVolumeLeaf;
	bool        m_bEyeInFogVolume;
	float       m_flDistanceToWater;
	float       m_flWaterHeight;
	IMaterial*  m_pFogVolumeMaterial;
};



struct BrushVertex_t
{
	Vector        m_Pos;
	Vector        m_Normal;
	Vector        m_TangentS;
	Vector        m_TangentT;
	Vector2D    m_TexCoord;
	Vector2D    m_LightmapCoord;

private:
	BrushVertex_t(const BrushVertex_t& src);

};
class IBrushSurface
{
public:
	// Computes texture coordinates + lightmap coordinates given a world position
	virtual void ComputeTextureCoordinate(Vector const& worldPos, Vector2D& texCoord) = 0;
	virtual void ComputeLightmapCoordinate(Vector const& worldPos, Vector2D& lightmapCoord) = 0;

	// Gets the vertex data for this surface
	virtual int  GetVertexCount() const = 0;
	virtual void GetVertexData(BrushVertex_t* pVerts) = 0;

	// Gets at the material properties for this surface
	virtual IMaterial* GetMaterial() = 0;
};

class IBrushRenderer
{
public:
	// Draws the surface; returns true if decals should be rendered on this surface
	virtual bool RenderBrushModelSurface(IClientEntity* pBaseEntity, IBrushSurface* pBrushSurface) = 0;
};

class IWorldRenderList /*: public IRefCounted*/
{
};


struct WorldListInfo_t
{
	int                     m_ViewFogVolume;
	int                     m_LeafCount;
	bool                    m_bHasWater;
	WorldListLeafData_t*    m_pLeafDataList;
};

struct VisOverrideData_t
{
	Vector        m_vecVisOrigin;                    // The point to to use as the viewpoint for area portal backface cull checks.
	float        m_fDistToAreaPortalTolerance;    // The distance from an area portal before using the full screen as the viewable portion.
};

#define MAX_VIS_LEAVES    32
#define MAX_AREA_STATE_BYTES        32
#define MAX_AREA_PORTAL_STATE_BYTES 24

struct VPlane
{
	Vector        m_Normal;
	vec_t        m_Dist;
};

#define FRUSTUM_NUMPLANES    6
typedef VPlane Frustum[FRUSTUM_NUMPLANES];

class CVRenderView
{
public:
	enum
	{
		VIEW_SETUP_VIS_EX_RETURN_FLAGS_USES_RADIAL_VIS = 0x00000001
	};
public:
	virtual void                DrawBrushModel(IClientEntity* baseentity, model_t* model, const Vector& origin, const QAngle& angles, bool sort) = 0;
	virtual void                DrawIdentityBrushModel(IWorldRenderList* pList, model_t* model) = 0;
	virtual void                TouchLight(struct dlight_t* light) = 0;
	virtual void                Draw3DDebugOverlays(void) = 0;
	virtual void                SetBlend(float blend) = 0;
	virtual float               GetBlend(void) = 0;
	virtual void                SetColorModulation(float const* blend) = 0;
	inline void                 SetColorModulation(float r, float g, float b)
	{
		float clr[3] = { r, g, b };
		SetColorModulation(clr);
	}
	virtual void                GetColorModulation(float* blend) = 0;
	virtual void                SceneBegin(void) = 0;
	virtual void                SceneEnd(void) = 0;
	virtual void                GetVisibleFogVolume(const Vector& eyePoint, VisibleFogVolumeInfo_t* pInfo) = 0;
	virtual IWorldRenderList*   CreateWorldList() = 0;
	virtual void                BuildWorldLists(IWorldRenderList* pList, WorldListInfo_t* pInfo, int iForceFViewLeaf, const VisOverrideData_t* pVisData = NULL, bool bShadowDepth = false, float* pReflectionWaterHeight = NULL) = 0;
	virtual void                DrawWorldLists(IWorldRenderList* pList, unsigned long flags, float waterZAdjust) = 0;
	virtual int                 GetNumIndicesForWorldLists(IWorldRenderList* pList, unsigned long nFlags) = 0;
	virtual void                DrawTopView(bool enable) = 0;
	virtual void                TopViewBounds(Vector2D const& mins, Vector2D const& maxs) = 0;
	virtual void                DrawLights(void) = 0;
	virtual void                DrawMaskEntities(void) = 0;
	virtual void                DrawTranslucentSurfaces(IWorldRenderList* pList, int* pSortList, int sortCount, unsigned long flags) = 0;
	virtual void                DrawLineFile(void) = 0;
	virtual void                DrawLightmaps(IWorldRenderList* pList, int pageId) = 0;
	virtual void                ViewSetupVis(bool novis, int numorigins, const Vector origin[]) = 0;
	virtual bool                AreAnyLeavesVisible(int* leafList, int nLeaves) = 0;
	virtual    void             VguiPaint(void) = 0;
	virtual void                ViewDrawFade(uint8_t* color, IMaterial* pMaterial) = 0;
	virtual void                OLD_SetProjectionMatrix(float fov, float zNear, float zFar) = 0;
	virtual unsigned long       GetLightAtPoint(Vector& pos) = 0;
	virtual int                 GetViewEntity(void) = 0;
	virtual bool                IsViewEntity(int entindex) = 0;
	virtual float               GetFieldOfView(void) = 0;
	virtual unsigned char**     GetAreaBits(void) = 0;
	virtual void                SetFogVolumeState(int nVisibleFogVolume, bool bUseHeightFog) = 0;
	virtual void                InstallBrushSurfaceRenderer(IBrushRenderer* pBrushRenderer) = 0;
	virtual void                DrawBrushModelShadow(IClientRenderable* pRenderable) = 0;
	virtual    bool             LeafContainsTranslucentSurfaces(IWorldRenderList* pList, int sortIndex, unsigned long flags) = 0;
	virtual bool                DoesBoxIntersectWaterVolume(const Vector& mins, const Vector& maxs, int leafWaterDataID) = 0;
	virtual void                SetAreaState(unsigned char chAreaBits[MAX_AREA_STATE_BYTES], unsigned char chAreaPortalBits[MAX_AREA_PORTAL_STATE_BYTES]) = 0;
	virtual void                VGui_Paint(int mode) = 0;
	virtual void                empty2(void) = 0;
	virtual void                empty1(void) = 0;
	virtual void                PopView(Frustum frustumPlanes) = 0;
	virtual void                SetMainView(const Vector& vecOrigin, const QAngle& angles) = 0;
	virtual void                ViewSetupVisEx(bool novis, int numorigins, const Vector origin[], unsigned int& returnFlags) = 0;
	virtual void                OverrideViewFrustum(Frustum custom) = 0;
	virtual void                DrawBrushModelShadowDepth(IClientEntity* baseentity, model_t* model, const Vector& origin, const QAngle& angles, bool bSort) = 0;
	virtual void                UpdateBrushModelLightmap(model_t* model, IClientRenderable* pRenderable) = 0;
	virtual void                BeginUpdateLightmaps(void) = 0;
	virtual void                EndUpdateLightmaps(void) = 0;
	virtual void                OLD_SetOffCenterProjectionMatrix(float fov, float zNear, float zFar, float flAspectRatio, float flBottom, float flTop, float flLeft, float flRight) = 0;
	virtual void                OLD_SetProjectionMatrixOrtho(float left, float top, float right, float bottom, float zNear, float zFar) = 0;
	virtual void                empty(void) = 0;
	virtual void                empty4(void) = 0;
};
extern CVRenderView* g_pRenderView;

struct vcollide_t
{
	unsigned short solidCount : 15;
	unsigned short isPacked : 1;
	unsigned short descSize;
	// VPhysicsSolids
	CPhysCollide   **solids;
	char           *pKeyValues;
	void           *pUserData;
};

enum RenderableTranslucencyType_t
{
	RENDERABLE_IS_OPAQUE = 0,
	RENDERABLE_IS_TRANSLUCENT,
	RENDERABLE_IS_TWO_PASS,    // has both translucent and opaque sub-partsa
};

struct virtualmodel_t;

typedef unsigned short MDLHandle_t;

class CModelInfo
{
public:
	virtual                                 ~CModelInfo(void) {}
	virtual const model_t*                  GetModel(int modelindex) const = 0;
	virtual int                             GetModelIndex(const char *name) const = 0;
	virtual const char*                     GetModelName(const model_t *model) const = 0;
	virtual vcollide_t*                     GetVCollide(const model_t *model) const = 0;
	virtual vcollide_t*                     GetVCollide(int modelindex) const = 0;
	virtual void                            GetModelBounds(const model_t *model, Vector& mins, Vector& maxs) const = 0;
	virtual void                            GetModelRenderBounds(const model_t *model, Vector& mins, Vector& maxs) const = 0;
	virtual int                             GetModelFrameCount(const model_t *model) const = 0;
	virtual int                             GetModelType(const model_t *model) const = 0;
	virtual void*                           GetModelExtraData(const model_t *model) = 0;
	virtual bool                            ModelHasMaterialProxy(const model_t *model) const = 0;
	virtual bool                            IsTranslucent(model_t const* model) const = 0;
	virtual bool                            IsTranslucentTwoPass(const model_t *model) const = 0;
	virtual void                            Unused0() {};
	virtual RenderableTranslucencyType_t    ComputeTranslucencyType(const model_t *model, int nSkin, int nBody) = 0;
	virtual int                             GetModelMaterialCount(const model_t* model) const = 0;
	virtual void                            GetModelMaterials(const model_t *model, int count, IMaterial** ppMaterial) = 0;
	virtual bool                            IsModelVertexLit(const model_t *model) const = 0;
	virtual const char*                     GetModelKeyValueText(const model_t *model) = 0;
	virtual bool                            GetModelKeyValue(const model_t *model, CStudioHdr &buf) = 0;
	virtual float                           GetModelRadius(const model_t *model) = 0;
	virtual CStudioHdr*                     GetStudioHdr(MDLHandle_t handle) = 0;
	virtual const studiohdr_t*              FindModel(const studiohdr_t *pStudioHdr, void **cache, const char *modelname) const = 0;
	virtual const studiohdr_t*              FindModel(void *cache) const = 0;
	virtual virtualmodel_t*                 GetVirtualModel(const studiohdr_t *pStudioHdr) const = 0;
	virtual uint8_t*                        GetAnimBlock(const studiohdr_t *pStudioHdr, int iBlock) const = 0;
	virtual void                            GetModelMaterialColorAndLighting(const model_t *model, Vector const& origin, Vector const& angles, model_t* pTrace, Vector& lighting, Vector& matColor) = 0;
	virtual void                            GetIlluminationPoint(const model_t *model, IClientRenderable *pRenderable, Vector const& origin, Vector const& angles, Vector* pLightingCenter) = 0;
	virtual int                             GetModelContents(int modelIndex) const = 0;
	virtual void							unused_30() = 0;
	virtual void							unused_31() = 0;
	virtual studiohdr_t*                    GetStudiomodel(const model_t *mod) = 0;
	virtual int                             GetModelSpriteWidth(const model_t *model) const = 0;
	virtual int                             GetModelSpriteHeight(const model_t *model) const = 0;
	virtual void                            SetLevelScreenFadeRange(float flMinSize, float flMaxSize) = 0;
	virtual void                            GetLevelScreenFadeRange(float *pMinArea, float *pMaxArea) const = 0;
	virtual void                            SetViewScreenFadeRange(float flMinSize, float flMaxSize) = 0;
	virtual unsigned char                   ComputeLevelScreenFade(const Vector &vecAbsOrigin, float flRadius, float flFadeScale) const = 0;
	virtual unsigned char                   ComputeViewScreenFade(const Vector &vecAbsOrigin, float flRadius, float flFadeScale) const = 0;
	virtual int                             GetAutoplayList(const studiohdr_t *pStudioHdr, unsigned short **pAutoplayList) const = 0;
	virtual CPhysCollide*                   GetCollideForVirtualTerrain(int index) = 0;
	virtual bool                            IsUsingFBTexture(const model_t *model, int nSkin, int nBody, IClientRenderable* *pClientRenderable) const = 0;
	virtual const model_t*                  FindOrLoadModel(const char *name) const = 0;
	virtual MDLHandle_t                     GetCacheHandle(const model_t *model) const = 0;
	virtual int                             GetBrushModelPlaneCount(const model_t *model) const = 0;
	virtual void                            GetBrushModelPlane(const model_t *model, int nIndex, model_t &plane, Vector *pOrigin) const = 0;
	virtual int                             GetSurfacepropsForVirtualTerrain(int index) = 0;
	virtual bool                            UsesEnvCubemap(const model_t *model) const = 0;
	virtual bool                            UsesStaticLighting(const model_t *model) const = 0;
	inline const char* GetModelNamevoid(const void* Model) {
		using Fn = char*(__thiscall*)(void*, const void*);
		return (*(Fn**)this)[3](this, Model);
	}
	inline void* GetModel(int Index) {
		using Fn = void*(__thiscall*)(CModelInfo*, int);
		return (*(Fn**)this)[1](this, Index);
	}
};
extern CModelInfo* g_pModelInfo;
