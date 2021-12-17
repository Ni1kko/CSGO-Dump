#pragma once

#include "sdk.h"

typedef unsigned short MaterialHandle_t;
enum MaterialVarFlags_t
{
	MATERIAL_VAR_DEBUG = ( 1 << 0 ),
	MATERIAL_VAR_NO_DEBUG_OVERRIDE = ( 1 << 1 ),
	MATERIAL_VAR_NO_DRAW = ( 1 << 2 ),
	MATERIAL_VAR_USE_IN_FILLRATE_MODE = ( 1 << 3 ),
	MATERIAL_VAR_VERTEXCOLOR = ( 1 << 4 ),
	MATERIAL_VAR_VERTEXALPHA = ( 1 << 5 ),
	MATERIAL_VAR_SELFILLUM = ( 1 << 6 ),
	MATERIAL_VAR_ADDITIVE = ( 1 << 7 ),
	MATERIAL_VAR_ALPHATEST = ( 1 << 8 ),
	MATERIAL_VAR_ZNEARER = ( 1 << 10 ),
	MATERIAL_VAR_MODEL = ( 1 << 11 ),
	MATERIAL_VAR_FLAT = ( 1 << 12 ),
	MATERIAL_VAR_NOCULL = ( 1 << 13 ),
	MATERIAL_VAR_NOFOG = ( 1 << 14 ),
	MATERIAL_VAR_IGNOREZ = ( 1 << 15 ),
	MATERIAL_VAR_DECAL = ( 1 << 16 ),
	MATERIAL_VAR_ENVMAPSPHERE = ( 1 << 17 ), // OBSOLETE
	MATERIAL_VAR_ENVMAPCAMERASPACE = ( 1 << 19 ), // OBSOLETE
	MATERIAL_VAR_BASEALPHAENVMAPMASK = ( 1 << 20 ),
	MATERIAL_VAR_TRANSLUCENT = ( 1 << 21 ),
	MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = ( 1 << 22 ),
	MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = ( 1 << 23 ), // OBSOLETE
	MATERIAL_VAR_OPAQUETEXTURE = ( 1 << 24 ),
	MATERIAL_VAR_ENVMAPMODE = ( 1 << 25 ), // OBSOLETE
	MATERIAL_VAR_SUPPRESS_DECALS = ( 1 << 26 ),
	MATERIAL_VAR_HALFLAMBERT = ( 1 << 27 ),
	MATERIAL_VAR_WIREFRAME = ( 1 << 28 ),
	MATERIAL_VAR_ALLOWALPHATOCOVERAGE = ( 1 << 29 ),
	MATERIAL_VAR_ALPHA_MODIFIED_BY_PROXY = ( 1 << 30 ),
	MATERIAL_VAR_VERTEXFOG = ( 1 << 31 ),
};

struct ModelRenderInfo_t
{
	Vector origin;
	QAngle angles;
	char pad0 [ 0x04 ]; // fix
	CBaseEntity *pRenderable;
	const model_t *pModel;
	const matrix3x4_t *pModelToWorld;
	const matrix3x4_t *pLightingOffset;
	const Vector *pLightingOrigin;
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
#define TEXTURE_GROUP_PIXEL_SHADErs					"Pixel Shaders"
#define TEXTURE_GROUP_VERTEX_SHADErs				"Vertex Shaders"
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE			"RenderTarget Surfaces"
#define TEXTURE_GROUP_MORPH_TARGETS					"Morph Targets"

struct MaterialVideoMode_t
{
	int m_Width;
	int m_Height;
	int m_Format;
	int m_RefreshRate;
};

struct MaterialSystem_Config_t
{
	MaterialVideoMode_t m_VideoMode;
	float m_fMonitorGamma;
	float m_fGammaTVRangeMin;
	float m_fGammaTVRangeMax;
	float m_fGammaTVExponent;
	bool m_bGammaTVEnabled;
	bool m_bTripleBuffered;
	int m_nAASamples;
	int m_nForceAnisotropicLevel;
	int m_nSkipMipLevels;
	int m_nDxSupportLevel;
	int m_nFlags;
	bool m_bEditMode;
	char m_nProxiesTestMode;
	bool m_bCompressedTextures;
	bool m_bFilterLightmaps;
	bool m_bFilterTextures;
	bool m_bReverseDepth;
	bool m_bBufferPrimitives;
	bool m_bDrawFlat;
	bool m_bMeasureFillRate;
	bool m_bVisualizeFillRate;
	bool m_bNoTransparency;
	bool m_bSoftwareLighting;
	bool m_bAllowCheats;
	char m_nShowMipLevels;
	bool m_bShowLowResImage;
	bool m_bShowNormalMap;
	bool m_bMipMapTextures;
	char m_nFullbright;
	bool m_bFastNoBump;
	bool m_bSuppressRendering;
	bool m_bDrawGray;
	bool m_bShowSpecular;
	bool m_bShowDiffuse;
	int m_nWindowedSizeLimitWidth;
	int m_nWindowedSizeLimitHeight;
	int m_nAAQuality;
	bool m_bShadowDepthTexture;
	bool m_bMotionBlur;
	bool m_bSupportFlashlight;
	bool m_bPaintEnabled;
	char pad [ 0xC ];
};

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
	virtual unsigned int GetPreviewImageProperties( int *width, int *height,
		unsigned int *imageFormat, bool* isTranslucent ) const = 0;

	// Get a preview image at the specified width/height and bitDepth.
	// Will do resampling if necessary.(not yet!!! :) )
	// Will do color format conversion. (works now.)
	virtual unsigned int GetPreviewImage( unsigned char *data,
		int width, int height,
		unsigned int imageFormat ) const = 0;
	// 
	virtual int				GetMappingWidth() = 0;
	virtual int				GetMappingHeight() = 0;

	virtual int				GetNumAnimationFrames() = 0;

	// For material subrects (material pages).  Offset(u,v) and scale(u,v) are normalized to texture.
	virtual bool			InMaterialPage( void ) = 0;
	virtual	void			GetMaterialOffset( float *pOffset ) = 0;
	virtual void			GetMaterialScale( float *pScale ) = 0;
	virtual IMaterial		*GetMaterialPage( void ) = 0;

	// find a vmt variable.
	// This is how game code affects how a material is rendered.
	// The game code must know about the params that are used by
	// the shader for the material that it is trying to affect.
	virtual unsigned int *	FindVar( const char *varName, bool *found, bool complain = true ) = 0;

	// The user never allocates or deallocates materials.  Reference counting is
	// used instead.  Garbage collection is done upon a call to 
	// IMaterialSystem::UncacheUnusedMaterials.
	virtual void			IncrementReferenceCount( void ) = 0;
	virtual void			DecrementReferenceCount( void ) = 0;

	inline void AddRef() { IncrementReferenceCount(); }
	inline void Release() { DecrementReferenceCount(); }

	// Each material is assigned a number that groups it with like materials
	// for sorting in the application.
	virtual int 			GetEnumerationID( void ) const = 0;

	virtual void			GetLowResColorSample( float s, float t, float *color ) const = 0;

	// This computes the state snapshots for this material
	virtual void			RecomputeStateSnapshots() = 0;

	// Are we translucent?
	virtual bool			IsTranslucent() = 0;

	// Are we alphatested?
	virtual bool			IsAlphaTested() = 0;

	// Are we vertex lit?
	virtual bool			IsVertexLit() = 0;

	// Gets the vertex format
	virtual void	GetVertexFormat() const = 0;

	// returns true if this material uses a material proxy
	virtual bool			HasProxy( void ) const = 0;

	virtual bool			UsesEnvCubemap( void ) = 0;

	virtual bool			NeedsTangentSpace( void ) = 0;

	virtual bool			NeedsPowerOfTwoFrameBufferTexture( bool bCheckSpecificToThisFrame = true ) = 0;
	virtual bool			NeedsFullFrameBufferTexture( bool bCheckSpecificToThisFrame = true ) = 0;

	// returns true if the shader doesn't do skinning itself and requires
	// the data that is sent to it to be preskinned.
	virtual bool			NeedsSoftwareSkinning( void ) = 0;

	// Apply constant color or alpha modulation
	virtual void			AlphaModulate( float alpha ) = 0;
	virtual void			ColorModulate( float r, float g, float b ) = 0;

	// Material Var flags...
	virtual void			SetMaterialVarFlag( MaterialVarFlags_t flag, bool on ) = 0;
	virtual bool			GetMaterialVarFlag( MaterialVarFlags_t flag ) const = 0;

	// Gets material reflectivity
	virtual void			GetReflectivity( Vector& reflect ) = 0;

	// Gets material property flags
	virtual bool			GetPropertyFlag( unsigned int type ) = 0;

	// Is the material visible from both sides?
	virtual bool			IsTwoSided() = 0;

	// Sets the shader associated with the material
	virtual void			SetShader( const char *pShaderName ) = 0;

	// Can't be const because the material might have to precache itself.
	virtual int				GetNumPasses( void ) = 0;

	// Can't be const because the material might have to precache itself.
	virtual int				GetTextureMemoryBytes( void ) = 0;

	// Meant to be used with materials created using CreateMaterial
	// It updates the materials to reflect the current values stored in the material vars
	virtual void			Refresh() = 0;

	// GR - returns true is material uses lightmap alpha for blending
	virtual bool			NeedsLightmapBlendAlpha( void ) = 0;

	// returns true if the shader doesn't do lighting itself and requires
	// the data that is sent to it to be prelighted
	virtual bool			NeedsSoftwareLighting( void ) = 0;

	// Gets at the shader parameters
	virtual int				ShaderParamCount() const = 0;
	virtual unsigned int	**GetShaderParams( void ) = 0;

	// Returns true if this is the error material you get back from IMaterialSystem::FindMaterial if
	// the material can't be found.
	virtual bool			IsErrorMaterial() const = 0;

	virtual void			Unused() = 0;

	// Gets the current alpha modulation
	virtual float			GetAlphaModulation() = 0;
	virtual void			GetColorModulation( float *r, float *g, float *b ) = 0;

	// Is this translucent given a particular alpha modulation?
	virtual bool			IsTranslucentUnderModulation( float fAlphaModulation = 1.0f ) const = 0;

	// fast find that stores the index of the found var in the string table in local cache
	virtual unsigned int *	FindVarFast( char const *pVarName, unsigned int *pToken ) = 0;

	// Sets new VMT shader parameters for the material
	virtual void			SetShaderAndParams( unsigned int *pKeyValues ) = 0;
	virtual const char *	GetShaderName() const = 0;

	virtual void			DeleteIfUnreferenced() = 0;

	virtual bool			IsSpriteCard() = 0;

	virtual void			CallBindProxy( void *proxyData ) = 0;

	virtual void			RefreshPreservingMaterialVars() = 0;

	virtual bool			WasReloadedFromWhitelist() = 0;

	virtual bool			SetTempExcluded( bool bSet, int nExcludedDimensionLimit ) = 0;

	virtual int				GetReferenceCount() const = 0;
};

class IVRenderView
{
public:
	void SetColorModulation( float  const* blend )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, float  const* blend );
		getvfunc<OriginalFn>( this, 6 )( this, blend );
	}
	void SetColorRGB( float r, float g, float b )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, float * );
		
		float color[ 4 ];
		color[ 0 ] = r / 255.f;
		color[ 1 ] = g / 255.f;
		color[ 2 ] = b / 255.f;
		color[ 3 ] = 1.f;

		getvfunc<OriginalFn>( this, 6 )( this, color );
	}
	void SetBlend(float blend)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, float blend);
		getvfunc<OriginalFn>(this, 4)(this, blend);
	}
	float GetBlend()
	{
		typedef float(__thiscall* OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 5)(this);
	}
	void GetColorRGB(float* rgb)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, float *);
		getvfunc<OriginalFn>(this, 7)(this, rgb);
	}
};
extern IVRenderView* g_pRenderView;

class IMaterialSystem
{
public:
	IMaterial* FindMaterial( const char* pMaterialName, const char* pTextureGroupName, bool complain = true, const char* pComplainPrefix = 0 )
	{
		typedef IMaterial*( __thiscall* OriginalFn )( PVOID, const char*, const char*, bool, const char* );
		return getvfunc<OriginalFn>( this, 84 )( this, pMaterialName, pTextureGroupName, complain, pComplainPrefix );
	}
	MaterialHandle_t FirstMaterial()
	{
		typedef MaterialHandle_t( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 86 )( this );
	}
	MaterialHandle_t NextMaterial( MaterialHandle_t h )
	{
		typedef MaterialHandle_t( __thiscall* OriginalFn )( PVOID, MaterialHandle_t );
		return getvfunc<OriginalFn>( this, 87 )( this, h );
	}
	MaterialHandle_t InvalidMaterial()
	{
		typedef MaterialHandle_t( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 88 )( this );
	}
	IMaterial* GetMaterial( MaterialHandle_t h )
	{
		typedef IMaterial*( __thiscall* OriginalFn )( PVOID, MaterialHandle_t );
		return getvfunc<OriginalFn>( this, 89 )( this, h );
	}
};
extern IMaterialSystem* g_pMatsystem;

extern std::vector< IMaterial* > vWorldMaterials;
extern std::vector< IMaterial* > vSkybox;