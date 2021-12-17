#pragma once
#include "../Include/GlobalIncludes.h"

#define TEXTURE_GROUP_LIGHTMAP						xorstr_("Lightmaps")
#define TEXTURE_GROUP_WORLD							xorstr_("World textures")
#define TEXTURE_GROUP_MODEL							xorstr_("Model textures")
#define TEXTURE_GROUP_VGUI							xorstr_("VGUI textures")
#define TEXTURE_GROUP_PARTICLE						xorstr_("Particle textures")
#define TEXTURE_GROUP_DECAL							xorstr_("Decal textures")
#define TEXTURE_GROUP_SKYBOX						xorstr_("SkyBox textures")
#define TEXTURE_GROUP_CLIENT_EFFECTS				xorstr_("ClientEffect textures")
#define TEXTURE_GROUP_OTHER							xorstr_("Other textures")
#define TEXTURE_GROUP_PRECACHED						xorstr_("Precached")				// TODO: assign texture groups to the precached materials
#define TEXTURE_GROUP_CUBE_MAP						xorstr_("CubeMap textures")
#define TEXTURE_GROUP_RENDER_TARGET					xorstr_("RenderTargets")
#define TEXTURE_GROUP_UNACCOUNTED					xorstr_("Unaccounted textures")	// Textures that weren't assigned a texture group.
//#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER		xorstr_("Static Vertex")
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER			xorstr_("Static Indices")
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP		xorstr_("Displacement Verts")
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR	xorstr_("Lighting Verts")
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD	xorstr_("World Verts")
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS	xorstr_("Model Verts")
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER	xorstr_("Other Verts")
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER			xorstr_("Dynamic Indices")
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER			xorstr_("Dynamic Verts")
#define TEXTURE_GROUP_DEPTH_BUFFER					xorstr_("DepthBuffer")
#define TEXTURE_GROUP_VIEW_MODEL					xorstr_("ViewModel")
#define TEXTURE_GROUP_PIXEL_SHADERS					xorstr_("Pixel Shaders")
#define TEXTURE_GROUP_VERTEX_SHADERS				xorstr_("Vertex Shaders")
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE			xorstr_("RenderTarget Surfaces")
#define TEXTURE_GROUP_MORPH_TARGETS					xorstr_("Morph Targets")

typedef unsigned short MaterialHandle_t;

#include "color.h"
#include "vfunc.hpp"
class KeyValues
{
	public:
		char pad[0x20];
};
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
	//	MATERIAL_VAR_UNUSED					  = (1 << 9),
	MATERIAL_VAR_ZNEARER = ( 1 << 10 ),
	MATERIAL_VAR_MODEL = ( 1 << 11 ),
	MATERIAL_VAR_FLAT = ( 1 << 12 ),
	MATERIAL_VAR_NOCULL = ( 1 << 13 ),
	MATERIAL_VAR_NOFOG = ( 1 << 14 ),
	MATERIAL_VAR_IGNOREZ = ( 1 << 15 ),
	MATERIAL_VAR_DECAL = ( 1 << 16 ),
	MATERIAL_VAR_ENVMAPSPHERE = ( 1 << 17 ), // OBSOLETE
	//	MATERIAL_VAR_UNUSED					  = (1 << 18),
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

	// NOTE: Only add flags here that either should be read from
	// .vmts or can be set directly from client code. Other, internal
	// flags should to into the flag enum in IMaterialInternal.h
};

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
	bool m_bCompressedTextures; // nothin
	bool m_bFilterLightmaps;
	bool m_bFilterTextures; // nothin
	bool m_bReverseDepth; // looks like asus walls
	bool m_bBufferPrimitives;
	bool m_bDrawFlat;
	bool m_bMeasureFillRate;
	bool m_bVisualizeFillRate; // red everywhere not usable
	bool m_bNoTransparency;
	bool m_bSoftwareLighting;
	bool m_bAllowCheats;
	char m_nShowMipLevels; // rainbows EVERYWHERE :O
	bool m_bShowLowResImage; // minecraft mode
	bool m_bShowNormalMap; // nothin too
	bool m_bMipMapTextures; // nothin too
	char m_nFullbright; // good for fps
	bool m_bFastNoBump;
	bool m_bSuppressRendering;
	bool m_bDrawGray; // gray walls
	bool m_bShowSpecular; // nothin too
	bool m_bShowDiffuse;
	int m_nWindowedSizeLimitWidth;
	int m_nWindowedSizeLimitHeight;
	int m_nAAQuality;
	bool m_bShadowDepthTexture; // nothing afaik
	bool m_bMotionBlur;
	bool m_bSupportFlashlight; // nothin afaik
	bool m_bPaintEnabled;
	char pad[0xC];
};

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

class IMaterialVar {};

class IMaterial
{
	public:
		virtual const char* get_name() const = 0;
		virtual const char* get_texture_group_name() const = 0;
		virtual void* get_pi_properties(int* width, int* height, void* imageFormat, bool* isTranslucent) const = 0;
		virtual void* get_pi(unsigned char* data, int width, int height, void* imageFormat) const = 0;
		virtual int get_mapping_w() = 0;
		virtual int get_mapping_h() = 0;
		virtual int get_num_anim_frames() = 0;
		virtual bool in_material_page(void) = 0;
		virtual void get_mat_offset(float* pOffset) = 0;
		virtual void get_mat_scale(float* pScale) = 0;
		virtual IMaterial *get_mat_page(void) = 0;
		virtual IMaterialVar * find_var(const char* varName, bool* found, bool complain = true) = 0;
		virtual void inc_ref(void) = 0;
		virtual void dec_ref(void) = 0;
		virtual int get_enum_id(void) const = 0;
		virtual void get_color_sample(float s, float t, float* color) const = 0;
		virtual void recompute() = 0;
		virtual bool is_transcluent() = 0;
		virtual bool is_alpha_tested() = 0;
		virtual bool is_vertex_lit() = 0;
		virtual void* get_vertex_fmt() const = 0;
		virtual bool has_proxy(void) const = 0;
		virtual bool uses_env_cubemap(void) = 0;
		virtual bool needs_tg_space(void) = 0;
		virtual bool omgsolongfunc(bool bCheckSpecificToThisFrame = true) = 0;
		virtual bool fuckthisshit(bool bCheckSpecificToThisFrame = true) = 0;
		virtual bool needs_software_skinning(void) = 0;
		virtual void alpha_modulate(float alpha) = 0;
		virtual void color_modulate(float r, float g, float b) = 0;
		virtual void set_flag(MaterialVarFlags_t flag, bool on) = 0;
		virtual bool get_flag(MaterialVarFlags_t flag) const = 0;
		void SetMaterialVarFlag( MaterialVarFlags_t flag, bool on )
		{
			typedef void( __thiscall * OriginalFn )( PVOID, MaterialVarFlags_t, bool );
			return vfunc<OriginalFn>( this, 29 )( this, flag, on );
		}
		/*IMaterial* FindVar(char const *pVarName, bool* found, bool complain = true)
		{
			typedef IMaterial*(__thiscall* Fn)(PVOID, const char*, bool, bool);
			return vfunc<Fn>(this, 11)(this, pVarName, found, complain);
		}*/
		void AlphaModulate( float alpha )
		{
			typedef void( __thiscall * Fn )( void*, float );
			return vfunc<Fn>( this, 27 )( this, alpha );
		}
		void ColorModulate( float r, float g, float b )
		{
			typedef void( __thiscall * Fn )( void*, float, float, float );
			return vfunc<Fn>( this, 28 )( this, r, g, b );
		}
		void ColorModulate( CColor color )
		{
			typedef void( __thiscall * Fn )( void*, float, float, float );
			return vfunc<Fn>( this, 28 )( this, color.RGBA[0] / 255.f, color.RGBA[1] / 255.f, color.RGBA[2] / 255.f );
		}
		const char* GetTextureGroupName()
		{
			typedef const char* ( __thiscall * oGetTextureGroupName )( void* );
			return vfunc<oGetTextureGroupName>( this, 1 )( this );
		}
		const char* GetMaterialName()
		{
			typedef const char* ( __thiscall * oGetName )( void* );
			return vfunc<oGetName>( this, 0 )( this );
		}
		void SetShader( const char* pShaderName )
		{
			typedef void( __thiscall * SetShaderFn )( void*, const char* pShaderName );
			vfunc<SetShaderFn>( this, 34 )( this, pShaderName );
		}
		void IncrementReferenceCount()
		{
			typedef void( __thiscall * Fn )( void* );
			return vfunc<Fn>( this, 12 )( this );
		}
		void DecrementReferenceCount()
		{
			typedef void( __thiscall * Fn )( void* );
			return vfunc<Fn>( this, 13 )( this );
		}
		float GetAlphaModulation()
		{
			typedef float( __thiscall * Fn )( void* );
			return vfunc<Fn>( this, 44 )( this );
		}
		void GetColorModulation( float* red, float* green, float* blue )
		{
			///45
			typedef void( __thiscall * Fn )( void*, float*, float*, float* );
			vfunc<Fn>( this, 45 )( this, red, green, blue );
		}
};
class IMaterialSystem // works
{
	public:

		IMaterial* CreateMaterial( const char* c, KeyValues* k )
		{
			typedef IMaterial*( __thiscall * tCreateMaterial )( PVOID, const char*, KeyValues* );
			return vfunc<tCreateMaterial>( this, 83 )( this, c, k );
		}
		IMaterial* FindMaterial( char const* pMaterialName, const char* pTextureGroupName, bool complain = true, const char* pComplainPrefix = NULL )
		{
			typedef IMaterial*( __thiscall * OriginalFn )( PVOID, char const*, const char*, bool, const char* );
			return vfunc<OriginalFn>( this, 84 )( this, pMaterialName, pTextureGroupName, complain, pComplainPrefix );
		}
		MaterialHandle_t FirstMaterial()
		{
			typedef MaterialHandle_t( __thiscall * oFirstMaterial )( void* );
			return vfunc<oFirstMaterial>( this, 86 )( this );
		}

		MaterialHandle_t NextMaterial( MaterialHandle_t h )
		{
			typedef MaterialHandle_t( __thiscall * oNextMaterial )( void*, MaterialHandle_t );
			return vfunc<oNextMaterial>( this, 87 )( this, h );
		}

		MaterialHandle_t InvalidMaterial()
		{
			typedef MaterialHandle_t( __thiscall * oInvalidMaterial )( void* );
			return vfunc<oInvalidMaterial>( this, 88 )( this );
		}

		IMaterial* GetMaterial( MaterialHandle_t h )
		{
			typedef IMaterial*( __thiscall * oGetMaterial )( void*, MaterialHandle_t );
			return vfunc<oGetMaterial>( this, 89 )( this, h );
		}
};
