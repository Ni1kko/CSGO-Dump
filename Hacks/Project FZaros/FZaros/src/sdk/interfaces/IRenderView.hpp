#pragma once
class CViewSetup;
class IWorldRenderList;

typedef unsigned short LeafIndex_t;
enum
{
	INVALID_LEAF_INDEX = (LeafIndex_t)~0
};

struct WorldListLeafData_t
{
	LeafIndex_t     leafIndex;    // 16 bits
	int16_t         waterData;
	uint16_t        firstTranslucentSurface;    // engine-internal list index
	uint16_t        translucentSurfaceCount;    // count of translucent surfaces+disps
};

struct WorldListInfo_t
{
	int                     ViewFogVolume;
	int						LeafCount;
	bool                    bHasWater;
	WorldListLeafData_t*    pLeafDataList;
};


struct VPlane
{
	Vector3        Normal;
	vec_t        Dist;
};
#define FRUSTUM_NUMPLANES    6
typedef VPlane Frustum[FRUSTUM_NUMPLANES];

struct BrushVertex_t
{
	Vector3        Pos;
	Vector3        Normal;
	Vector3        TangentS;
	Vector3       TangentT;
	Vector2    TexCoord;
	Vector2    LightmapCoord;

private:
	BrushVertex_t(const BrushVertex_t& src);
};

struct VisOverrideData_t
{
	Vector3        vecVisOrigin;                    // The point to to use as the viewpoint for area portal backface cull checks.
	float        fDistToAreaPortalTolerance;    // The distance from an area portal before using the full screen as the viewable portion.
};


struct VisibleFogVolumeInfo_t
{
	int            nVisibleFogVolume;
	int            nVisibleFogVolumeLeaf;
	bool        bEyeInFogVolume;
	float       flDistanceToWater;
	float       flWaterHeight;
	IMaterial*  pFogVolumeMaterial;
};

class IBrushSurface
{
public:
	// Computes texture coordinates + lightmap coordinates given a world position
	virtual void ComputeTextureCoordinate(Vector const& worldPos, Vector2& texCoord) = 0;
	virtual void ComputeLightmapCoordinate(Vector const& worldPos, Vector2& lightmapCoord) = 0;

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

#define MAX_VIS_LEAVES    32
#define MAX_AREA_STATE_BYTES        32
#define MAX_AREA_PORTAL_STATE_BYTES 24


class IRenderView
{
	enum
	{
		VIEW_SETUP_VIS_EX_RETURN_FLAGS_USES_RADIAL_VIS = 0x00000001
	};
public:
	virtual void                DrawBrushModel(IClientEntity *baseentity, model_t *model, const Vector& origin, const Vector& angles, bool sort) = 0;
	virtual void                DrawIdentityBrushModel(IWorldRenderList *pList, model_t *model) = 0;
	virtual void                TouchLight(struct dlight_t *light) = 0;
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
	virtual void                GetVisibleFogVolume(const Vector& eyePoint, VisibleFogVolumeInfo_t *pInfo) = 0;
	virtual IWorldRenderList*   CreateWorldList() = 0;
	virtual void                BuildWorldLists(IWorldRenderList *pList, WorldListInfo_t* pInfo, int iForceFViewLeaf, const VisOverrideData_t* pVisData = NULL, bool bShadowDepth = false, float *pReflectionWaterHeight = NULL) = 0;
	virtual void                DrawWorldLists(IWorldRenderList *pList, unsigned long flags, float waterZAdjust) = 0;
	virtual int                 GetNumIndicesForWorldLists(IWorldRenderList *pList, unsigned long nFlags) = 0;
	virtual void                DrawTopView(bool enable) = 0;
	virtual void                TopViewBounds(Vector2 const& mins, Vector2 const& maxs) = 0;
	virtual void                DrawLights(void) = 0;
	virtual void                DrawMaskEntities(void) = 0;
	virtual void                DrawTranslucentSurfaces(IWorldRenderList *pList, int *pSortList, int sortCount, unsigned long flags) = 0;
	virtual void                DrawLineFile(void) = 0;
	virtual void                DrawLightmaps(IWorldRenderList *pList, int pageId) = 0;
	virtual void                ViewSetupVis(bool novis, int numorigins, const Vector origin[]) = 0;
	virtual bool                AreAnyLeavesVisible(int *leafList, int nLeaves) = 0;
	virtual    void             VguiPaint(void) = 0;
	virtual void                ViewDrawFade(uint8_t *color, IMaterial *pMaterial) = 0;
	virtual void                OLD_SetProjectionMatrix(float fov, float zNear, float zFar) = 0;
	virtual unsigned long       GetLightAtPoint(Vector& pos) = 0;
	virtual int                 GetViewEntity(void) = 0;
	virtual bool                IsViewEntity(int entindex) = 0;
	virtual float               GetFieldOfView(void) = 0;
	virtual unsigned char**     GetAreaBits(void) = 0;
	virtual void                SetFogVolumeState(int nVisibleFogVolume, bool bUseHeightFog) = 0;
	virtual void                InstallBrushSurfaceRenderer(IBrushRenderer* pBrushRenderer) = 0;
	virtual void                DrawBrushModelShadow(IClientRenderable *pRenderable) = 0;
	virtual    bool             LeafContainsTranslucentSurfaces(IWorldRenderList *pList, int sortIndex, unsigned long flags) = 0;
	virtual bool                DoesBoxIntersectWaterVolume(const Vector &mins, const Vector &maxs, int leafWaterDataID) = 0;
	virtual void                SetAreaState(unsigned char chAreaBits[MAX_AREA_STATE_BYTES], unsigned char chAreaPortalBits[MAX_AREA_PORTAL_STATE_BYTES]) = 0;
	virtual void                VGui_Paint(int mode) = 0;
	virtual void                Push3DView(const CViewSetup &view, int nFlags, ITexture* pRenderTarget, Frustum frustumPlanes) = 0;
	virtual void                Push2DView(const CViewSetup &view, int nFlags, ITexture* pRenderTarget, Frustum frustumPlanes) = 0;
	virtual void                PopView(Frustum frustumPlanes) = 0;
	virtual void                SetMainView(const Vector &vecOrigin, const Vector &angles) = 0;
	virtual void                ViewSetupVisEx(bool novis, int numorigins, const Vector origin[], unsigned int &returnFlags) = 0;
	virtual void                OverrideViewFrustum(Frustum custom) = 0;
	virtual void                DrawBrushModelShadowDepth(IClientEntity *baseentity, model_t *model, const Vector& origin, const Vector& angles, bool bSort) = 0;
	virtual void                UpdateBrushModelLightmap(model_t *model, IClientRenderable *pRenderable) = 0;
	virtual void                BeginUpdateLightmaps(void) = 0;
	virtual void                EndUpdateLightmaps(void) = 0;
	virtual void                OLD_SetOffCenterProjectionMatrix(float fov, float zNear, float zFar, float flAspectRatio, float flBottom, float flTop, float flLeft, float flRight) = 0;
	virtual void                OLD_SetProjectionMatrixOrtho(float left, float top, float right, float bottom, float zNear, float zFar) = 0;
	virtual void                Push3DView(const CViewSetup &view, int nFlags, ITexture* pRenderTarget, Frustum frustumPlanes, ITexture* pDepthTexture) = 0;
	virtual void                GetMatricesForView(const CViewSetup &view, VMatrix *pWorldToView, VMatrix *pViewToProjection, VMatrix *pWorldToProjection, VMatrix *pWorldToPixels) = 0;
};