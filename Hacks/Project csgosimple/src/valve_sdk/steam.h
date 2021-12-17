#pragma once

typedef int32_t HSteamPipe;
typedef int32_t HSteamUser;

enum EGCResults
{
	k_EGCResultOK = 0,
	k_EGCResultNoMessage = 1,           // There is no message in the queue
	k_EGCResultBufferTooSmall = 2,      // The buffer is too small for the requested message
	k_EGCResultNotLoggedOn = 3,         // The client is not logged onto Steam
	k_EGCResultInvalidMessage = 4,      // Something was wrong with the message being sent with SendMessage
};

class ISteamGameCoordinator
{
public:
	virtual EGCResults GCSendMessage(int unMsgType, const void *pubData, int cubData) = 0;
	virtual bool IsMessageAvailable(int *pcubMsgSize) = 0;
	virtual EGCResults RetrieveMessage(int *punMsgType, void *pubDest, int cubDest, int *pcubMsgSize) = 0;

};

class CSteamID
{
public:
	CSteamID()
	{
		m_steamid.m_comp.m_unAccountID = 0;
		m_steamid.m_comp.m_EAccountType = 0;
		m_steamid.m_comp.m_EUniverse = 0;
		m_steamid.m_comp.m_unAccountInstance = 0;
	}
	uint32_t GetAccountID() const { return m_steamid.m_comp.m_unAccountID; }

private:
	union SteamID_t
	{
		struct SteamIDComponent_t
		{
			uint32_t			m_unAccountID : 32;			// unique account identifier
			unsigned int		m_unAccountInstance : 20;	// dynamic instance ID (used for multiseat type accounts only)
			unsigned int		m_EAccountType : 4;			// type of account - can't show as EAccountType, due to signed / unsigned difference
			int					m_EUniverse : 8;	// universe this account belongs to
		} m_comp;

		uint64_t m_unAll64Bits;
	} m_steamid;
};

class ISteamUser
{
public:
	virtual uint32_t GetHSteamUser() = 0;
	virtual bool BLoggedOn() = 0;
	virtual CSteamID GetSteamID() = 0;
};

using HTTPRequestHandle = std::uint32_t;
enum : HTTPRequestHandle { INVALID_HTTPREQUEST_HANDLE = 0 };

using SteamAPICall_t = std::uint64_t;
enum : SteamAPICall_t { k_uAPICallInvalid = 0 };

enum class EHTTPMethod
{
	Invalid = 0,
	GET,
	HEAD,
	POST
};

class ISteamHTTP
{
public:
	virtual HTTPRequestHandle CreateHTTPRequest(EHTTPMethod eHTTPRequestMethod, const char* pchAbsoluteURL) = 0;
	virtual bool SetHTTPRequestContextValue(HTTPRequestHandle hRequest, std::uint64_t ulContextValue) = 0;
	virtual bool SetHTTPRequestNetworkActivityTimeout(HTTPRequestHandle hRequest, std::uint32_t unTimeoutSeconds) = 0;
	virtual bool SetHTTPRequestHeaderValue(HTTPRequestHandle hRequest, const char* pchHeaderName, const char* pchHeaderValue) = 0;
	virtual bool SetHTTPRequestGetOrPostParameter(HTTPRequestHandle hRequest, const char* pchParamName, const char* pchParamValue) = 0;
	virtual bool SendHTTPRequest(HTTPRequestHandle hRequest, SteamAPICall_t* pCallHandle) = 0;
	virtual bool SendHTTPRequestAndStreamResponse(HTTPRequestHandle hRequest, SteamAPICall_t* pCallHandle) = 0;
	virtual bool DeferHTTPRequest(HTTPRequestHandle hRequest) = 0;
	virtual bool PrioritizeHTTPRequest(HTTPRequestHandle hRequest) = 0;
	virtual bool GetHTTPResponseHeaderSize(HTTPRequestHandle hRequest, const char* pchHeaderName, std::uint32_t* unResponseHeaderSize) = 0;
	virtual bool GetHTTPResponseHeaderValue(HTTPRequestHandle hRequest, const char* pchHeaderName, std::uint8_t* pHeaderValueBuffer, std::uint32_t unBufferSize) = 0;
	virtual bool GetHTTPResponseBodySize(HTTPRequestHandle hRequest, std::uint32_t* unBodySize) = 0;
	virtual bool GetHTTPResponseBodyData(HTTPRequestHandle hRequest, std::uint8_t* pBodyDataBuffer, std::uint32_t unBufferSize) = 0;
	virtual bool GetHTTPStreamingResponseBodyData(HTTPRequestHandle hRequest, std::uint32_t cOffset, std::uint8_t* pBodyDataBuffer, std::uint32_t unBufferSize) = 0;
	virtual bool ReleaseHTTPRequest(HTTPRequestHandle hRequest) = 0;
	virtual bool GetHTTPDownloadProgressPct(HTTPRequestHandle hRequest, float* pflPercentOut) = 0;
	virtual bool SetHTTPRequestRawPostBody(HTTPRequestHandle hRequest, const char* pchContentType, std::uint8_t* pubBody, std::uint32_t unBodyLen) = 0;
};

class ISteamFriends;

class ISteamClient
{
public:
	ISteamUser *GetISteamUser(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion)
	{
		typedef ISteamUser*(__stdcall* func)(HSteamUser, HSteamPipe, const char *);
		return CallVFunction<func>(this, 5)(hSteamUser, hSteamPipe, pchVersion);
	}

	ISteamFriends *GetISteamFriends(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion)
	{
		typedef ISteamFriends*(__stdcall* func)(HSteamUser, HSteamPipe, const char *);
		return CallVFunction<func>(this, 8)(hSteamUser, hSteamPipe, pchVersion);
	}

	ISteamGameCoordinator* GetISteamGenericInterface(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion)
	{
		typedef ISteamGameCoordinator*(__stdcall* func)(HSteamUser, HSteamPipe, const char *);
		return CallVFunction<func>(this, 12)(hSteamUser, hSteamPipe, pchVersion);
	}

	ISteamHTTP* GetISteamHTTP(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion)
	{
		typedef ISteamHTTP*(__stdcall* func)(HSteamUser, HSteamPipe, const char *);
		return CallVFunction<func>(this, 23)(hSteamUser, hSteamPipe, pchVersion);
	}
};