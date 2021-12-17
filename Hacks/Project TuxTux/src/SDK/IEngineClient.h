#pragma once


#define FLOW_OUTGOING 0
#define FLOW_INCOMING 1
#define MAX_FLOWS 2	// in & out

//-----------------------------------------------------------------------------
// forward declarations
//-----------------------------------------------------------------------------

class ClientClass;
class CGlobalVarsBase;
typedef struct InputContextHandle_t__ *InputContextHandle_t;
struct client_textmessage_t;
struct model_t;
class SurfInfo;
class IMaterial;
class CSentence;
class CAudioSource;
class AudioState_t;
class ISpatialQuery;
class IMaterialSystem;
class CPhysCollide;
class IAchievementMgr;
class INetChannelInfo;
class ISPSharedMemory;
class CGamestatsData;
class KeyValues;
class CSteamAPIContext;
struct Frustum_t;

typedef void(*pfnDemoCustomDataCallback)(uint8_t *pData, size_t iSize);

enum ClientFrameStage_ta
{
    FRAME_UNDEFINED = -1,
    FRAME_START,
    FRAME_NET_UPDATE_START,
    FRAME_NET_UPDATE_POSTDATAUPDATE_START,
    FRAME_NET_UPDATE_POSTDATAUPDATE_END,
    FRAME_NET_UPDATE_END,
    FRAME_RENDER_START,
    FRAME_RENDER_END
};

// Used by RenderView
enum RenderViewInfo_t
{
    RENDERVIEW_UNSPECIFIED = 0,
    RENDERVIEW_DRAWVIEWMODEL = (1 << 0),
    RENDERVIEW_DRAWHUD = (1 << 1),
    RENDERVIEW_SUPPRESSMONITORRENDERING = (1 << 2),
};

typedef struct player_info_s
{
    int64_t         unknown;            //0x0000
    union
    {
        int64_t       steamID64;          //0x0008 - SteamID64
        struct
        {
            int32_t     xuid_low;
            int32_t     xuid_high;
        };
    };
    char            szName[128];        //0x0010 - Player Name
    int             userId;             //0x0090 - Unique Server Identifier
    char            szSteamID[20];      //0x0094 - STEAM_X:Y:Z
    char            pad_0x00A8[0x10];   //0x00A8
    unsigned long   iSteamID;           //0x00B8 - SteamID
    char            szFriendsName[128];
    bool            fakeplayer;
    bool            ishltv;
    unsigned int    customfiles[4];
    unsigned char   filesdownloaded;
} player_info_t;

class IVEngineClient
{
public:
    virtual int                   GetIntersectingSurfaces(const model_t *model, const Vector &vCenter, const float radius, const bool bOnlyVisibleSurfaces, SurfInfo *pInfos, const int nMaxInfos) = 0;
    virtual Vector                GetLightForPoint(const Vector &pos, bool bClamp) = 0;
    virtual IMaterial*            TraceLineMaterialAndLighting(const Vector &start, const Vector &end, Vector &diffuseLightColor, Vector& baseColor) = 0;
    virtual const char*           ParseFile(const char *data, char *token, int maxlen) = 0;
    virtual bool                  CopyFile(const char *source, const char *destination) = 0;
    virtual void                  GetScreenSize(int& width, int& height) = 0;
    virtual void                  ServerCmd(const char *szCmdString, bool bReliable = true) = 0;
    virtual void                  ClientCmd(const char *szCmdString) = 0;
    virtual bool                  GetPlayerInfo(int ent_num, player_info_t *pinfo) = 0;
    virtual int                   GetPlayerForUserID(int userID) = 0;
    virtual client_textmessage_t* TextMessageGet(const char *pName) = 0; // 10
    virtual bool                  Con_IsVisible(void) = 0;
    virtual int                   GetLocalPlayer(void) = 0;
    virtual const model_t*        LoadModel(const char *pName, bool bProp = false) = 0;
    virtual float                 GetLastTimeStamp(void) = 0;
    virtual CSentence*            GetSentence(CAudioSource *pAudioSource) = 0; // 15
    virtual float                 GetSentenceLength(CAudioSource *pAudioSource) = 0;
    virtual bool                  IsStreaming(CAudioSource *pAudioSource) const = 0;
    virtual void                  GetViewAngles(QAngle& va) = 0;
    virtual void                  SetViewAngles(QAngle& va) = 0;
    virtual int                   GetMaxClients(void) = 0; // 20
    virtual const char*           Key_LookupBinding(const char *pBinding) = 0;
    virtual const char*           Key_BindingForKey(int &code) = 0;
    virtual void                  Key_SetBinding(int, char const*) = 0;
    virtual void                  StartKeyTrapMode(void) = 0;
    virtual bool                  CheckDoneKeyTrapping(int &code) = 0;
    virtual bool                  IsInGame(void) = 0;
    virtual bool                  IsConnected(void) = 0;
    virtual bool                  IsDrawingLoadingImage(void) = 0;
    virtual void                  HideLoadingPlaque(void) = 0;
    virtual void                  Con_NPrintf(int pos, const char *fmt, ...) = 0; // 30
    virtual void                  Con_NXPrintf(const struct con_nprint_s *info, const char *fmt, ...) = 0;
    virtual int                   IsBoxVisible(const Vector& mins, const Vector& maxs) = 0;
    virtual int                   IsBoxInViewCluster(const Vector& mins, const Vector& maxs) = 0;
    virtual bool                  CullBox(const Vector& mins, const Vector& maxs) = 0;
    virtual void                  Sound_ExtraUpdate(void) = 0;
    virtual const char*           GetGameDirectory(void) = 0;
    virtual const matrix3x4_t&        WorldToScreenMatrix() = 0;
    virtual const matrix3x4_t&        WorldToViewMatrix() = 0;
    virtual int                   GameLumpVersion(int lumpId) const = 0;
    virtual int                   GameLumpSize(int lumpId) const = 0; // 40
    virtual bool                  LoadGameLump(int lumpId, void* pBuffer, int size) = 0;
    virtual int                   LevelLeafCount() const = 0;
    virtual ISpatialQuery*        GetBSPTreeQuery() = 0;
    virtual void                  LinearToGamma(float* linear, float* gamma) = 0;
    virtual float                 LightStyleValue(int style) = 0; // 45
    virtual void                  ComputeDynamicLighting(const Vector& pt, const Vector* pNormal, Vector& color) = 0;
    virtual void                  GetAmbientLightColor(Vector& color) = 0;
    virtual int                   GetDXSupportLevel() = 0;
    virtual bool                  SupportsHDR() = 0;
    virtual void                  Mat_Stub(IMaterialSystem *pMatSys) = 0; // 50
    virtual void                  GetChapterName(char *pchBuff, int iMaxLength) = 0;
    virtual char const*           GetLevelName(void) = 0;
    virtual char const*           GetLevelNameShort(void) = 0;
    virtual char const*           GetMapGroupName(void) = 0;
    virtual struct IVoiceTweak_s* GetVoiceTweakAPI(void) = 0;
    virtual void                  SetVoiceCasterID(unsigned int someint) = 0; // 56
    virtual void                  EngineStats_BeginFrame(void) = 0;
    virtual void                  EngineStats_EndFrame(void) = 0;
    virtual void                  FireEvents() = 0;
    virtual int                   GetLeavesArea(unsigned short *pLeaves, int nLeaves) = 0;
    virtual bool                  DoesBoxTouchAreaFrustum(const Vector &mins, const Vector &maxs, int iArea) = 0; // 60
    virtual int                   GetFrustumList(Frustum_t **pList, int listMax) = 0;
    virtual bool                  ShouldUseAreaFrustum(int i) = 0;
    virtual void                  SetAudioState(const AudioState_t& state) = 0;
    virtual int                   SentenceGroupPick(int groupIndex, char *name, int nameBufLen) = 0;
    virtual int                   SentenceGroupPickSequential(int groupIndex, char *name, int nameBufLen, int sentenceIndex, int reset) = 0;
    virtual int                   SentenceIndexFromName(const char *pSentenceName) = 0;
    virtual const char*           SentenceNameFromIndex(int sentenceIndex) = 0;
    virtual int                   SentenceGroupIndexFromName(const char *pGroupName) = 0;
    virtual const char*           SentenceGroupNameFromIndex(int groupIndex) = 0;
    virtual float                 SentenceLength(int sentenceIndex) = 0;
    virtual void                  ComputeLighting(const Vector& pt, const Vector* pNormal, bool bClamp, Vector& color, Vector *pBoxColors = NULL) = 0;
    virtual void                  ActivateOccluder(int nOccluderIndex, bool bActive) = 0;
    virtual bool                  IsOccluded(const Vector &vecAbsMins, const Vector &vecAbsMaxs) = 0; // 74
    virtual int                   GetOcclusionViewId(void) = 0;
    virtual void*                 SaveAllocMemory(size_t num, size_t size) = 0;
    virtual void                  SaveFreeMemory(void *pSaveMem) = 0;
    virtual INetChannelInfo*      GetNetChannelInfo(void) = 0;
    virtual void                  DebugDrawPhysCollide(const CPhysCollide *pCollide, IMaterial *pMaterial, const matrix3x4_t& transform, const uint8_t* color) = 0; //79
    virtual void                  CheckPoint(const char *pName) = 0; // 80
    virtual void                  DrawPortals() = 0;
    virtual bool                  IsPlayingDemo(void) = 0;
    virtual bool                  IsRecordingDemo(void) = 0;
    virtual bool                  IsPlayingTimeDemo(void) = 0;
    virtual int                   GetDemoRecordingTick(void) = 0;
    virtual int                   GetDemoPlaybackTick(void) = 0;
    virtual int                   GetDemoPlaybackStartTick(void) = 0;
    virtual float                 GetDemoPlaybackTimeScale(void) = 0;
    virtual int                   GetDemoPlaybackTotalTicks(void) = 0;
    virtual bool                  IsPaused(void) = 0; // 90
    virtual float                 GetTimescale(void) const = 0;
    virtual bool                  IsTakingScreenshot(void) = 0;
    virtual bool                  IsHLTV(void) = 0;
    virtual bool                  IsLevelMainMenuBackground(void) = 0;
    virtual void                  GetMainMenuBackgroundName(char *dest, int destlen) = 0;
    virtual void                  SetOcclusionParameters(const int /*OcclusionParams_t*/ &params) = 0; // 96
    virtual void                  GetUILanguage(char *dest, int destlen) = 0;
    virtual int                   IsSkyboxVisibleFromPoint(const Vector &vecPoint) = 0;
    virtual const char*           GetMapEntitiesString() = 0;
    virtual bool                  IsInEditMode(void) = 0; // 100
    virtual float                 GetScreenAspectRatio(int viewportWidth, int viewportHeight) = 0;
    virtual bool                  REMOVED_SteamRefreshLogin(const char *password, bool isSecure) = 0;
    virtual bool                  REMOVED_SteamProcessCall(bool & finished) = 0;
    virtual unsigned int          GetEngineBuildNumber() = 0; // engines build
    virtual const char *          GetProductVersionString() = 0; // mods version number (steam.inf)
    virtual void                  GrabPreColorCorrectedFrame(int x, int y, int width, int height) = 0;
    virtual bool                  IsHammerRunning() const = 0;
    virtual void                  ExecuteClientCmd(const char *szCmdString) = 0; //108
    virtual bool                  MapHasHDRLighting(void) = 0;
    virtual bool                  MapHasLightMapAlphaData(void) = 0;
    virtual int                   GetAppID() = 0;
    virtual Vector                GetLightForPointFast(const Vector &pos, bool bClamp) = 0;
    virtual void                  ClientCmd_Unrestricted(char  const*, int, bool) = 0;
    virtual void                  ClientCmd_Unrestricted(const char *szCmdString) = 0; // 114
    virtual void                  SetRestrictServerCommands(bool bRestrict) = 0;
    virtual void                  SetRestrictClientCommands(bool bRestrict) = 0;
    virtual void                  SetOverlayBindProxy(int iOverlayID, void *pBindProxy) = 0;
    virtual bool                  CopyFrameBufferToMaterial(const char *pMaterialName) = 0;
    virtual void                  ReadConfiguration(const int iController, const bool readDefault) = 0;
    virtual void                  SetAchievementMgr(IAchievementMgr *pAchievementMgr) = 0;
    virtual IAchievementMgr*      GetAchievementMgr() = 0;
    virtual bool                  MapLoadFailed(void) = 0;
    virtual void                  SetMapLoadFailed(bool bState) = 0;
    virtual bool                  IsLowViolence() = 0;
    virtual const char*           GetMostRecentSaveGame(void) = 0;
    virtual void                  SetMostRecentSaveGame(const char *lpszFilename) = 0;
    virtual void                  StartXboxExitingProcess() = 0;
    virtual bool                  IsSaveInProgress() = 0;
    virtual bool                  IsAutoSaveDangerousInProgress(void) = 0;
    virtual unsigned int          OnStorageDeviceAttached(int iController) = 0;
    virtual void                  OnStorageDeviceDetached(int iController) = 0;
    virtual char* const           GetSaveDirName(void) = 0;
    virtual void                  WriteScreenshot(const char *pFilename) = 0;
    virtual void                  ResetDemoInterpolation(void) = 0;
    virtual int                   GetActiveSplitScreenPlayerSlot() = 0;
    virtual int                   SetActiveSplitScreenPlayerSlot(int slot) = 0;
    virtual bool                  SetLocalPlayerIsResolvable(char const *pchContext, int nLine, bool bResolvable) = 0;
    virtual bool                  IsLocalPlayerResolvable() = 0;
    virtual int                   GetSplitScreenPlayer(int nSlot) = 0;
    virtual bool                  IsSplitScreenActive() = 0;
    virtual bool                  IsValidSplitScreenSlot(int nSlot) = 0;
    virtual int                   FirstValidSplitScreenSlot() = 0; // -1 == invalid
    virtual int                   NextValidSplitScreenSlot(int nPreviousSlot) = 0; // -1 == invalid
    virtual ISPSharedMemory*      GetSinglePlayerSharedMemorySpace(const char *szName, int ent_num = (1 << 11)) = 0;
    virtual void                  ComputeLightingCube(const Vector& pt, bool bClamp, Vector *pBoxColors) = 0;
    virtual void                  RegisterDemoCustomDataCallback(const char* szCallbackSaveID, pfnDemoCustomDataCallback pCallback) = 0;
    virtual void                  RecordDemoCustomData(pfnDemoCustomDataCallback pCallback, const void *pData, size_t iDataLength) = 0;
    virtual void                  SetPitchScale(float flPitchScale) = 0;
    virtual float                 GetPitchScale(void) = 0;
    virtual bool                  LoadFilmmaker() = 0;
    virtual void                  UnloadFilmmaker() = 0;
    virtual void                  SetLeafFlag(int nLeafIndex, int nFlagBits) = 0;
    virtual void                  RecalculateBSPLeafFlags(void) = 0;
    virtual bool                  DSPGetCurrentDASRoomNew(void) = 0;
    virtual bool                  DSPGetCurrentDASRoomChanged(void) = 0;
    virtual bool                  DSPGetCurrentDASRoomSkyAbove(void) = 0;
    virtual float                 DSPGetCurrentDASRoomSkyPercent(void) = 0;
    virtual void                  SetMixGroupOfCurrentMixer(const char *szgroupname, const char *szparam, float val, int setMixerType) = 0;
    virtual int                   GetMixLayerIndex(const char *szmixlayername) = 0;
    virtual void                  SetMixLayerLevel(int index, float level) = 0;
    virtual int                   GetMixGroupIndex(char  const* groupname) = 0;
    virtual void                  SetMixLayerTriggerFactor(int i1, int i2, float fl) = 0;
    virtual void                  SetMixLayerTriggerFactor(char  const* char1, char  const* char2, float fl) = 0;
    virtual bool                  IsCreatingReslist() = 0;
    virtual bool                  IsCreatingXboxReslist() = 0;
    virtual void                  SetTimescale(float flTimescale) = 0;
    virtual void                  SetGamestatsData(CGamestatsData *pGamestatsData) = 0;
    virtual CGamestatsData*       GetGamestatsData() = 0;
    virtual void                  GetMouseDelta(int &dx, int &dy, bool b) = 0; // unknown
    virtual   const char*         Key_LookupBindingEx(const char *pBinding, int iUserId = -1, int iStartCount = 0, int iAllowJoystick = -1) = 0;
    virtual int                   Key_CodeForBinding(char  const*, int, int, int) = 0; // 186
    virtual void                  UpdateDAndELights(void) = 0;
    virtual int                   GetBugSubmissionCount() const = 0;
    virtual void                  ClearBugSubmissionCount() = 0;
    virtual bool                  DoesLevelContainWater() const = 0;
    virtual float                 GetServerSimulationFrameTime() const = 0;
    virtual void                  SolidMoved(class IClientEntity *pSolidEnt, class ICollideable *pSolidCollide, const Vector* pPrevAbsOrigin, bool accurateBboxTriggerChecks) = 0;
    virtual void                  TriggerMoved(class IClientEntity *pTriggerEnt, bool accurateBboxTriggerChecks) = 0;
    virtual void                  ComputeLeavesConnected(const Vector &vecOrigin, int nCount, const int *pLeafIndices, bool *pIsConnected) = 0;
    virtual bool                  IsInCommentaryMode(void) = 0;
    virtual void                  SetBlurFade(float amount) = 0;
    virtual bool                  IsTransitioningToLoad() = 0;
    virtual void                  SearchPathsChangedAfterInstall() = 0;
    virtual void                  ConfigureSystemLevel(int nCPULevel, int nGPULevel) = 0;
    virtual void                  SetConnectionPassword(char const *pchCurrentPW) = 0;
    virtual CSteamAPIContext*     GetSteamAPIContext() = 0;
    virtual void                  SubmitStatRecord(char const *szMapName, unsigned int uiBlobVersion, unsigned int uiBlobSize, const void *pvBlob) = 0;
    virtual void                  ServerCmdKeyValues(KeyValues *pKeyValues) = 0; // 203
    virtual void                  SpherePaintSurface(const model_t* model, const Vector& location, unsigned char chr, float fl1, float fl2) = 0;
    virtual bool                  HasPaintmap(void) = 0;
    virtual void                  EnablePaintmapRender() = 0;
    //virtual void                TracePaintSurface( const model_t *model, const Vector& position, float radius, CUtlVector<Color>& surfColors ) = 0;
    virtual void                  SphereTracePaintSurface(const model_t* model, const Vector& position, const Vector &vec2, float radius, /*CUtlVector<unsigned char, CUtlMemory<unsigned char, int>>*/ int& utilVecShit) = 0;
    virtual void                  RemoveAllPaint() = 0;
    virtual void                  PaintAllSurfaces(unsigned char uchr) = 0;
    virtual void                  RemovePaint(const model_t* model) = 0;
    virtual bool                  IsActiveApp() = 0;
    virtual bool                  IsClientLocalToActiveServer() = 0;
    virtual void                  TickProgressBar() = 0;
    virtual InputContextHandle_t  GetInputContext(int /*EngineInputContextId_t*/ id) = 0;
    virtual void                  GetStartupImage(char* filename, int size) = 0;
    virtual bool                  IsUsingLocalNetworkBackdoor(void) = 0;
    virtual void                  SaveGame(const char*, bool, char*, int, char*, int) = 0;
    virtual void                  GetGenericMemoryStats(/* GenericMemoryStat_t */ void **) = 0;
    virtual bool                  GameHasShutdownAndFlushedMemory(void) = 0;
    virtual int                   GetLastAcknowledgedCommand(void) = 0;
    virtual void                  FinishContainerWrites(int i) = 0;
    virtual void                  FinishAsyncSave(void) = 0;
    virtual int                   GetServerTick(void) = 0;
    virtual const char*           GetModDirectory(void) = 0;
    virtual bool                  AudioLanguageChanged(void) = 0;
    virtual bool                  IsAutoSaveInProgress(void) = 0;
    virtual void                  StartLoadingScreenForCommand(const char* command) = 0;
    virtual void                  StartLoadingScreenForKeyValues(KeyValues* values) = 0;
    virtual void                  SOSSetOpvarFloat(const char*, float) = 0;
    virtual void                  SOSGetOpvarFloat(const char*, float &) = 0;
    virtual bool                  IsSubscribedMap(const char*, bool) = 0;
    virtual bool                  IsFeaturedMap(const char*, bool) = 0;
    virtual void                  GetDemoPlaybackParameters(void) = 0;
    virtual int                   GetClientVersion(void) = 0;
    virtual bool                  IsDemoSkipping(void) = 0;
    virtual void                  SetDemoImportantEventData(const KeyValues* values) = 0;
    virtual void                  ClearEvents(void) = 0;
    virtual int                   GetSafeZoneXMin(void) = 0;
    virtual bool                  IsVoiceRecording(void) = 0;
    virtual void                  ForceVoiceRecordOn(void) = 0;
    virtual bool IsReplay(void) = 0;
};


class IEngineClient
{
public:
	typedef struct player_info_s
	{
		int64_t __pad0;
		union {
			int64_t xuid;
			struct {
				int xuidlow;
				int xuidhigh;
			};
		};
		char name[128];
		int userid;
		char guid[33];
		unsigned int friendsid;
		char friendsname[128];
		bool fakeplayer;
		bool ishltv;
		unsigned int customfiles[4];
		unsigned char filesdownloaded;
	} player_info_t;

	void GetScreenSize(int& width, int& height)
	{
		typedef void (* oGetScreenSize)(void*, int&, int&);
		return getvfunc<oGetScreenSize>(this, 5)(this, width, height);
	}

	bool GetPlayerInfo(int iIndex, player_info_t *pInfo)
	{
		typedef bool (* oGetPlayerInfo)(void*, int, player_info_t*);
		return getvfunc<oGetPlayerInfo>(this, 8)(this, iIndex, pInfo);
	}

	int GetPlayerForUserID(int UserID)
	{
		typedef int (* oGetPlayerForUserID)(void*, int);
		return getvfunc<oGetPlayerForUserID>(this, 9)(this, UserID);
	}

	int GetLocalPlayer()
	{
		typedef int (* oGetLocalPlayer)(void*);
		return getvfunc<oGetLocalPlayer>(this, 12)(this);
	}

	void GetViewAngles(QAngle& angle)
	{
		typedef void (* oGetViewAngles)(void*, QAngle&);
		return getvfunc<oGetViewAngles>(this, 18)(this, angle);
	}

	void SetViewAngles(QAngle& angle)
	{
		typedef void (* oSetViewAngles)(void*, QAngle&);
		return getvfunc<oSetViewAngles>(this, 19)(this, angle);
	}

	int GetMaxClients()
	{
		typedef int (* oGetMaxClients)(void*);
		return getvfunc<oGetMaxClients>(this, 20)(this);
	}

	bool IsInGame()
	{
		typedef bool (* oIsInGame)(void*);
		return getvfunc<oIsInGame>(this, 26)(this);
	}

	INetChannelInfo* GetNetChannelInfo()
	{
		typedef INetChannelInfo* (* oGetNetChannelInfo) (void*);
		return getvfunc<oGetNetChannelInfo>(this, 78)(this);
	}
    INetChannela* GetNetChannelInfoa()
	{
		typedef INetChannela* (* oGetNetChannelInfo) (void*);
		return getvfunc<oGetNetChannelInfo>(this, 78)(this);
	}
    //

	bool IsTakingScreenshot()
	{
		typedef bool (* oIsTakingScreenshot)(void*);
		return getvfunc<oIsTakingScreenshot>(this, 92)(this);
	}

	void ExecuteClientCmd(const char* szCmdString)
	{
		typedef void (* oExecuteClientCmd)(void*, const char*);
		return getvfunc<oExecuteClientCmd>(this, 108)(this, szCmdString);
	}

	void ClientCmd_Unrestricted(const char* szCmdString)
	{
		typedef void (* oClientCmd_Unrestricted)(void*, const char*);
		return getvfunc<oClientCmd_Unrestricted>(this, 113)(this, szCmdString);
	}
};


//test 2

typedef int HSteamPipe;
// handle to single instance of a steam user
typedef int HSteamUser;
// function prototype
#if defined( POSIX )
#define __cdecl
#endif

#if defined( __SNC__ )
	#pragma diag_suppress=1700	   // warning 1700: class "%s" has virtual functions but non-virtual destructor
#endif

class ISteamUser;
class ISteamGameServer;
class ISteamFriends;
class ISteamUtils;
class ISteamContentServer;
class ISteamMatchmakingServers;
class ISteamUserStats;
class ISteamApps;
class ISteamNetworking;
class ISteamRemoteStorage;
class ISteamScreenshots;
class ISteamMusic;
class ISteamMusicRemote;
class ISteamGameServerStats;
class ISteamPS3OverlayRender;
class ISteamHTTP;
class ISteamController;
class ISteamUGC;
class ISteamAppList;
class ISteamHTMLSurface;
class ISteamInventory;
class ISteamVideo;
class ISteamParentalSettings;
enum ELobbyType
{
	k_ELobbyTypePrivate = 0,		// only way to join the lobby is to invite to someone else
	k_ELobbyTypeFriendsOnly = 1,	// shows for friends or invitees, but not in lobby list
	k_ELobbyTypePublic = 2,			// visible for friends and in lobby list
	k_ELobbyTypeInvisible = 3,		// returned by search, but not visible to other friends 
									//    useful if you want a user in two lobbies, for example matching groups together
									//	  a user can be in only one regular lobby, and up to two invisible lobbies
};

#if ( defined(STEAM_API_EXPORTS) || defined(STEAM_API_NODLL) ) && !defined(API_GEN)
#define STEAM_PRIVATE_API( ... ) __VA_ARGS__
#elif defined(STEAM_API_EXPORTS) && defined(API_GEN)
#define STEAM_PRIVATE_API( ... )
#else
#define STEAM_PRIVATE_API( ... ) protected: __VA_ARGS__ public:
#endif
// lobby search filter tools
enum ELobbyComparison
{
	k_ELobbyComparisonEqualToOrLessThan = -2,
	k_ELobbyComparisonLessThan = -1,
	k_ELobbyComparisonEqual = 0,
	k_ELobbyComparisonGreaterThan = 1,
	k_ELobbyComparisonEqualToOrGreaterThan = 2,
	k_ELobbyComparisonNotEqual = 3,
};

// lobby search distance. Lobby results are sorted from closest to farthest.
enum ELobbyDistanceFilter
{
	k_ELobbyDistanceFilterClose,		// only lobbies in the same immediate region will be returned
	k_ELobbyDistanceFilterDefault,		// only lobbies in the same region or near by regions
	k_ELobbyDistanceFilterFar,			// for games that don't have many latency requirements, will return lobbies about half-way around the globe
	k_ELobbyDistanceFilterWorldwide,	// no filtering, will match lobbies as far as India to NY (not recommended, expect multiple seconds of latency between the clients)
};
typedef int SteamAPICall_t;
enum EChatEntryType
{
	k_EChatEntryTypeInvalid = 0, 
	k_EChatEntryTypeChatMsg = 1,		// Normal text message from another user
	k_EChatEntryTypeTyping = 2,			// Another user is typing (not used in multi-user chat)
	k_EChatEntryTypeInviteGame = 3,		// Invite from other user into that users current game
	k_EChatEntryTypeEmote = 4,			// text emote message (deprecated, should be treated as ChatMsg)
	//k_EChatEntryTypeLobbyGameStart = 5,	// lobby game is starting (dead - listen for LobbyGameCreated_t callback instead)
	k_EChatEntryTypeLeftConversation = 6, // user has left the conversation ( closed chat window )
	// Above are previous FriendMsgType entries, now merged into more generic chat entry types
	k_EChatEntryTypeEntered = 7,		// user has entered the conversation (used in multi-user chat and group chat)
	k_EChatEntryTypeWasKicked = 8,		// user was kicked (data: 64-bit steamid of actor performing the kick)
	k_EChatEntryTypeWasBanned = 9,		// user was banned (data: 64-bit steamid of actor performing the ban)
	k_EChatEntryTypeDisconnected = 10,	// user disconnected
	k_EChatEntryTypeHistoricalChat = 11,	// a chat message from user's chat history or offilne message
	//k_EChatEntryTypeReserved1 = 12, // No longer used
	//k_EChatEntryTypeReserved2 = 13, // No longer used
	k_EChatEntryTypeLinkBlocked = 14, // a link was removed by the chat filter.
};

#ifdef API_GEN
# define CLANG_ATTR(ATTR) __attribute__((annotate( ATTR )))
#else
# define CLANG_ATTR(ATTR)
#endif

#define METHOD_DESC(DESC) CLANG_ATTR( "desc:" #DESC ";" )
#define IGNOREATTR() CLANG_ATTR( "ignore" )
#define OUT_STRUCT() CLANG_ATTR( "out_struct: ;" )
#define OUT_STRING() CLANG_ATTR( "out_string: ;" )
#define OUT_ARRAY_CALL(COUNTER,FUNCTION,PARAMS) CLANG_ATTR( "out_array_call:" #COUNTER "," #FUNCTION "," #PARAMS ";" )
#define OUT_ARRAY_COUNT(COUNTER, DESC) CLANG_ATTR( "out_array_count:" #COUNTER  ";desc:" #DESC )
#define ARRAY_COUNT(COUNTER) CLANG_ATTR( "array_count:" #COUNTER ";" )
#define ARRAY_COUNT_D(COUNTER, DESC) CLANG_ATTR( "array_count:" #COUNTER ";desc:" #DESC )
#define BUFFER_COUNT(COUNTER) CLANG_ATTR( "buffer_count:" #COUNTER ";" )
#define OUT_BUFFER_COUNT(COUNTER) CLANG_ATTR( "out_buffer_count:" #COUNTER ";" )
#define OUT_STRING_COUNT(COUNTER) CLANG_ATTR( "out_string_count:" #COUNTER ";" )
#define DESC(DESC) CLANG_ATTR("desc:" #DESC ";")
#define CALL_RESULT(RESULT_TYPE) CLANG_ATTR("callresult:" #RESULT_TYPE ";")
#define CALL_BACK(RESULT_TYPE) CLANG_ATTR("callback:" #RESULT_TYPE ";")
// maximum number of characters a lobby metadata key can be
#define k_nMaxLobbyKeyLength 255

//-----------------------------------------------------------------------------
// Purpose: Functions for match making services for clients to get to favorites
//			and to operate on game lobbies.
//-----------------------------------------------------------------------------
class ISteamMatchmaking
{
public:
	// game server favorites storage
	// saves basic details about a multiplayer game server locally

	// returns the number of favorites servers the user has stored
	virtual int GetFavoriteGameCount() = 0;
	
	// returns the details of the game server
	// iGame is of range [0,GetFavoriteGameCount())
	// *pnIP, *pnConnPort are filled in the with IP:port of the game server
	// *punFlags specify whether the game server was stored as an explicit favorite or in the history of connections
	// *pRTime32LastPlayedOnServer is filled in the with the Unix time the favorite was added
	virtual bool GetFavoriteGame( int iGame, AppId_t *pnAppID, int *pnIP, int *pnConnPort, int *pnQueryPort, int *punFlags, int *pRTime32LastPlayedOnServer ) = 0;

	// adds the game server to the local list; updates the time played of the server if it already exists in the list
	virtual int AddFavoriteGame( AppId_t nAppID, int nIP, int nConnPort, int nQueryPort, int unFlags, int rTime32LastPlayedOnServer ) = 0;
	
	// removes the game server from the local storage; returns true if one was removed
	virtual bool RemoveFavoriteGame( AppId_t nAppID, int nIP, int nConnPort, int nQueryPort, int unFlags ) = 0;

	///////
	// Game lobby functions

	// Get a list of relevant lobbies
	// this is an asynchronous request
	// results will be returned by LobbyMatchList_t callback & call result, with the number of lobbies found
	// this will never return lobbies that are full
	// to add more filter, the filter calls below need to be call before each and every RequestLobbyList() call
	// use the CCallResult<> object in steam_api.h to match the SteamAPICall_t call result to a function in an object, e.g.
	/*
		class CMyLobbyListManager
		{
			CCallResult<CMyLobbyListManager, LobbyMatchList_t> m_CallResultLobbyMatchList;
			void FindLobbies()
			{
				// SteamMatchmaking()->AddRequestLobbyListFilter*() functions would be called here, before RequestLobbyList()
				SteamAPICall_t hSteamAPICall = SteamMatchmaking()->RequestLobbyList();
				m_CallResultLobbyMatchList.Set( hSteamAPICall, this, &CMyLobbyListManager::OnLobbyMatchList );
			}
			void OnLobbyMatchList( LobbyMatchList_t *pLobbyMatchList, bool bIOFailure )
			{
				// lobby list has be retrieved from Steam back-end, use results
			}
		}
	*/
	// 
	CALL_RESULT( LobbyMatchList_t )
	virtual SteamAPICall_t RequestLobbyList() = 0;
	// filters for lobbies
	// this needs to be called before RequestLobbyList() to take effect
	// these are cleared on each call to RequestLobbyList()
	virtual void AddRequestLobbyListStringFilter( const char *pchKeyToMatch, const char *pchValueToMatch, ELobbyComparison eComparisonType ) = 0;
	// numerical comparison
	virtual void AddRequestLobbyListNumericalFilter( const char *pchKeyToMatch, int nValueToMatch, ELobbyComparison eComparisonType ) = 0;
	// returns results closest to the specified value. Multiple near filters can be added, with early filters taking precedence
	virtual void AddRequestLobbyListNearValueFilter( const char *pchKeyToMatch, int nValueToBeCloseTo ) = 0;
	// returns only lobbies with the specified number of slots available
	virtual void AddRequestLobbyListFilterSlotsAvailable( int nSlotsAvailable ) = 0;
	// sets the distance for which we should search for lobbies (based on users IP address to location map on the Steam backed)
	virtual void AddRequestLobbyListDistanceFilter( ELobbyDistanceFilter eLobbyDistanceFilter ) = 0;
	// sets how many results to return, the lower the count the faster it is to download the lobby results & details to the client
	virtual void AddRequestLobbyListResultCountFilter( int cMaxResults ) = 0;

	virtual void AddRequestLobbyListCompatibleMembersFilter( CSteamID steamIDLobby ) = 0;

	// returns the CSteamID of a lobby, as retrieved by a RequestLobbyList call
	// should only be called after a LobbyMatchList_t callback is received
	// iLobby is of the range [0, LobbyMatchList_t::m_nLobbiesMatching)
	// the returned CSteamID::IsValid() will be false if iLobby is out of range
	virtual CSteamID GetLobbyByIndex( int iLobby ) = 0;

	// Create a lobby on the Steam servers.
	// If private, then the lobby will not be returned by any RequestLobbyList() call; the CSteamID
	// of the lobby will need to be communicated via game channels or via InviteUserToLobby()
	// this is an asynchronous request
	// results will be returned by LobbyCreated_t callback and call result; lobby is joined & ready to use at this point
	// a LobbyEnter_t callback will also be received (since the local user is joining their own lobby)
	CALL_RESULT( LobbyCreated_t )
	virtual SteamAPICall_t CreateLobby( ELobbyType eLobbyType, int cMaxMembers ) = 0;

	// Joins an existing lobby
	// this is an asynchronous request
	// results will be returned by LobbyEnter_t callback & call result, check m_EChatRoomEnterResponse to see if was successful
	// lobby metadata is available to use immediately on this call completing
	CALL_RESULT( LobbyEnter_t )
	virtual SteamAPICall_t JoinLobby( CSteamID steamIDLobby ) = 0;

	// Leave a lobby; this will take effect immediately on the client side
	// other users in the lobby will be notified by a LobbyChatUpdate_t callback
	virtual void LeaveLobby( CSteamID steamIDLobby ) = 0;

	// Invite another user to the lobby
	// the target user will receive a LobbyInvite_t callback
	// will return true if the invite is successfully sent, whether or not the target responds
	// returns false if the local user is not connected to the Steam servers
	// if the other user clicks the join link, a GameLobbyJoinRequested_t will be posted if the user is in-game,
	// or if the game isn't running yet the game will be launched with the parameter +connect_lobby <64-bit lobby id>
	virtual bool InviteUserToLobby( CSteamID steamIDLobby, CSteamID steamIDInvitee ) = 0;

	// Lobby iteration, for viewing details of users in a lobby
	// only accessible if the lobby user is a member of the specified lobby
	// persona information for other lobby members (name, avatar, etc.) will be asynchronously received
	// and accessible via ISteamFriends interface
	
	// returns the number of users in the specified lobby
	virtual int GetNumLobbyMembers( CSteamID steamIDLobby ) = 0;
	// returns the CSteamID of a user in the lobby
	// iMember is of range [0,GetNumLobbyMembers())
	// note that the current user must be in a lobby to retrieve CSteamIDs of other users in that lobby
	virtual CSteamID GetLobbyMemberByIndex( CSteamID steamIDLobby, int iMember ) = 0;

	// Get data associated with this lobby
	// takes a simple key, and returns the string associated with it
	// "" will be returned if no value is set, or if steamIDLobby is invalid
	virtual const char *GetLobbyData( CSteamID steamIDLobby, const char *pchKey ) = 0;
	// Sets a key/value pair in the lobby metadata
	// each user in the lobby will be broadcast this new value, and any new users joining will receive any existing data
	// this can be used to set lobby names, map, etc.
	// to reset a key, just set it to ""
	// other users in the lobby will receive notification of the lobby data change via a LobbyDataUpdate_t callback
	virtual bool SetLobbyData( CSteamID steamIDLobby, const char *pchKey, const char *pchValue ) = 0;

	// returns the number of metadata keys set on the specified lobby
	virtual int GetLobbyDataCount( CSteamID steamIDLobby ) = 0;

	// returns a lobby metadata key/values pair by index, of range [0, GetLobbyDataCount())
	virtual bool GetLobbyDataByIndex( CSteamID steamIDLobby, int iLobbyData, char *pchKey, int cchKeyBufferSize, char *pchValue, int cchValueBufferSize ) = 0;

	// removes a metadata key from the lobby
	virtual bool DeleteLobbyData( CSteamID steamIDLobby, const char *pchKey ) = 0;

	// Gets per-user metadata for someone in this lobby
	virtual const char *GetLobbyMemberData( CSteamID steamIDLobby, CSteamID steamIDUser, const char *pchKey ) = 0;
	// Sets per-user metadata (for the local user implicitly)
	virtual void SetLobbyMemberData( CSteamID steamIDLobby, const char *pchKey, const char *pchValue ) = 0;
	
	// Broadcasts a chat message to the all the users in the lobby
	// users in the lobby (including the local user) will receive a LobbyChatMsg_t callback
	// returns true if the message is successfully sent
	// pvMsgBody can be binary or text data, up to 4k
	// if pvMsgBody is text, cubMsgBody should be strlen( text ) + 1, to include the null terminator
	virtual bool SendLobbyChatMsg( CSteamID steamIDLobby, const void *pvMsgBody, int cubMsgBody ) = 0;
	// Get a chat message as specified in a LobbyChatMsg_t callback
	// iChatID is the LobbyChatMsg_t::m_iChatID value in the callback
	// *pSteamIDUser is filled in with the CSteamID of the member
	// *pvData is filled in with the message itself
	// return value is the number of bytes written into the buffer
	virtual int GetLobbyChatEntry( CSteamID steamIDLobby, int iChatID, OUT_STRUCT() CSteamID *pSteamIDUser, void *pvData, int cubData, EChatEntryType *peChatEntryType ) = 0;

	// Refreshes metadata for a lobby you're not necessarily in right now
	// you never do this for lobbies you're a member of, only if your
	// this will send down all the metadata associated with a lobby
	// this is an asynchronous call
	// returns false if the local user is not connected to the Steam servers
	// results will be returned by a LobbyDataUpdate_t callback
	// if the specified lobby doesn't exist, LobbyDataUpdate_t::m_bSuccess will be set to false
	virtual bool RequestLobbyData( CSteamID steamIDLobby ) = 0;
	
	// sets the game server associated with the lobby
	// usually at this point, the users will join the specified game server
	// either the IP/Port or the steamID of the game server has to be valid, depending on how you want the clients to be able to connect
	virtual void SetLobbyGameServer( CSteamID steamIDLobby, int unGameServerIP, int unGameServerPort, CSteamID steamIDGameServer ) = 0;
	// returns the details of a game server set in a lobby - returns false if there is no game server set, or that lobby doesn't exist
	virtual bool GetLobbyGameServer( CSteamID steamIDLobby, int *punGameServerIP, int *punGameServerPort, OUT_STRUCT() CSteamID *psteamIDGameServer ) = 0;

	// set the limit on the # of users who can join the lobby
	virtual bool SetLobbyMemberLimit( CSteamID steamIDLobby, int cMaxMembers ) = 0;
	// returns the current limit on the # of users who can join the lobby; returns 0 if no limit is defined
	virtual int GetLobbyMemberLimit( CSteamID steamIDLobby ) = 0;

	// updates which type of lobby it is
	// only lobbies that are k_ELobbyTypePublic or k_ELobbyTypeInvisible, and are set to joinable, will be returned by RequestLobbyList() calls
	virtual bool SetLobbyType( CSteamID steamIDLobby, ELobbyType eLobbyType ) = 0;

	// sets whether or not a lobby is joinable - defaults to true for a new lobby
	// if set to false, no user can join, even if they are a friend or have been invited
	virtual bool SetLobbyJoinable( CSteamID steamIDLobby, bool bLobbyJoinable ) = 0;

	// returns the current lobby owner
	// you must be a member of the lobby to access this
	// there always one lobby owner - if the current owner leaves, another user will become the owner
	// it is possible (bur rare) to join a lobby just as the owner is leaving, thus entering a lobby with self as the owner
	virtual CSteamID GetLobbyOwner( CSteamID steamIDLobby ) = 0;

	// changes who the lobby owner is
	// you must be the lobby owner for this to succeed, and steamIDNewOwner must be in the lobby
	// after completion, the local user will no longer be the owner
	virtual bool SetLobbyOwner( CSteamID steamIDLobby, CSteamID steamIDNewOwner ) = 0;

	// link two lobbies for the purposes of checking player compatibility
	// you must be the lobby owner of both lobbies
	virtual bool SetLinkedLobby( CSteamID steamIDLobby, CSteamID steamIDLobbyDependent ) = 0;

#ifdef _PS3
	// changes who the lobby owner is
	// you must be the lobby owner for this to succeed, and steamIDNewOwner must be in the lobby
	// after completion, the local user will no longer be the owner
	virtual void CheckForPSNGameBootInvite( unsigned int iGameBootAttributes  ) = 0;
#endif
	CALL_BACK( LobbyChatUpdate_t )
};
//-----------------------------------------------------------------------------
// Purpose: Interface to creating a new steam instance, or to
//			connect to an existing steam instance, whether it's in a
//			different process or is local.
//
//			For most scenarios this is all handled automatically via SteamAPI_Init().
//			You'll only need these APIs if you have a more complex versioning scheme,
//			or if you want to implement a multiplexed gameserver where a single process
//			is handling multiple games at once with independent gameserver SteamIDs.
//-----------------------------------------------------------------------------
class ISteamClient
{
public:
	// Creates a communication pipe to the Steam client.
	// NOT THREADSAFE - ensure that no other threads are accessing Steamworks API when calling
	virtual HSteamPipe CreateSteamPipe() = 0;

	// Releases a previously created communications pipe
	// NOT THREADSAFE - ensure that no other threads are accessing Steamworks API when calling
	virtual bool BReleaseSteamPipe( HSteamPipe hSteamPipe ) = 0;

	// connects to an existing global user, failing if none exists
	// used by the game to coordinate with the steamUI
	// NOT THREADSAFE - ensure that no other threads are accessing Steamworks API when calling
	virtual HSteamUser ConnectToGlobalUser( HSteamPipe hSteamPipe ) = 0;

	// used by game servers, create a steam user that won't be shared with anyone else
	// NOT THREADSAFE - ensure that no other threads are accessing Steamworks API when calling
	virtual HSteamUser CreateLocalUser( HSteamPipe *phSteamPipe, EAccountType eAccountType ) = 0;

	// removes an allocated user
	// NOT THREADSAFE - ensure that no other threads are accessing Steamworks API when calling
	virtual void ReleaseUser( HSteamPipe hSteamPipe, HSteamUser hUser ) = 0;

	// retrieves the ISteamUser interface associated with the handle
	virtual ISteamUser *GetISteamUser( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;

	// retrieves the ISteamGameServer interface associated with the handle
	virtual ISteamGameServer *GetISteamGameServer( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;

	// set the local IP and Port to bind to
	// this must be set before CreateLocalUser()
	virtual void SetLocalIPBinding( int unIP, int usPort ) = 0; 

	// returns the ISteamFriends interface
	virtual ISteamFriends *GetISteamFriends( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;

	// returns the ISteamUtils interface
	virtual ISteamUtils *GetISteamUtils( HSteamPipe hSteamPipe, const char *pchVersion ) = 0;

	// returns the ISteamMatchmaking interface
	virtual ISteamMatchmaking *GetISteamMatchmaking( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;

	// returns the ISteamMatchmakingServers interface
	virtual ISteamMatchmakingServers *GetISteamMatchmakingServers( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;

	// returns the a generic interface
	virtual void *GetISteamGenericInterface( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;

	// returns the ISteamUserStats interface
	virtual ISteamUserStats *GetISteamUserStats( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;

	// returns the ISteamGameServerStats interface
	virtual ISteamGameServerStats *GetISteamGameServerStats( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;

	// returns apps interface
	virtual ISteamApps *GetISteamApps( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;

	// networking
	virtual ISteamNetworking *GetISteamNetworking( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;

	// remote storage
	virtual ISteamRemoteStorage *GetISteamRemoteStorage( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;

	// user screenshots
	virtual ISteamScreenshots *GetISteamScreenshots( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;

	// Deprecated. Applications should use SteamAPI_RunCallbacks() or SteamGameServer_RunCallbacks() instead.
	STEAM_PRIVATE_API( virtual void RunFrame() = 0; )

	// returns the number of IPC calls made since the last time this function was called
	// Used for perf debugging so you can understand how many IPC calls your game makes per frame
	// Every IPC call is at minimum a thread context switch if not a process one so you want to rate
	// control how often you do them.
	virtual int GetIPCCallCount() = 0;

	// API warning handling
	// 'int' is the severity; 0 for msg, 1 for warning
	// 'const char *' is the text of the message
	// callbacks will occur directly after the API function is called that generated the warning or message.
	virtual void SetWarningMessageHook( void* pFunction ) = 0;

	// Trigger global shutdown for the DLL
	virtual bool BShutdownIfAllPipesClosed() = 0;

	// Expose HTTP interface
	virtual ISteamHTTP *GetISteamHTTP( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;

	// Deprecated - the ISteamUnifiedMessages interface is no longer intended for public consumption.
	STEAM_PRIVATE_API( virtual void *DEPRECATED_GetISteamUnifiedMessages( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0 ; )

	// Exposes the ISteamController interface
	virtual ISteamController *GetISteamController( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;

	// Exposes the ISteamUGC interface
	virtual ISteamUGC *GetISteamUGC( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;

	// returns app list interface, only available on specially registered apps
	virtual ISteamAppList *GetISteamAppList( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;
	
	// Music Player
	virtual ISteamMusic *GetISteamMusic( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;

	// Music Player Remote
	virtual ISteamMusicRemote *GetISteamMusicRemote(HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion) = 0;

	// html page display
	virtual ISteamHTMLSurface *GetISteamHTMLSurface(HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion) = 0;

	// Helper functions for internal Steam usage
	STEAM_PRIVATE_API( virtual void DEPRECATED_Set_SteamAPI_CPostAPIResultInProcess( void (*)() ) = 0; )
	STEAM_PRIVATE_API( virtual void DEPRECATED_Remove_SteamAPI_CPostAPIResultInProcess( void (*)() ) = 0; )
	STEAM_PRIVATE_API( virtual void Set_SteamAPI_CCheckCallbackRegisteredInProcess( void* func ) = 0; )

	// inventory
	virtual ISteamInventory *GetISteamInventory( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;

	// Video
	virtual ISteamVideo *GetISteamVideo( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;

	// Parental controls
	virtual ISteamParentalSettings *GetISteamParentalSettings( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;
};