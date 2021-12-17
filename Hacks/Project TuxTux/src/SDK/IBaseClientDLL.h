#pragma once

struct RecvProp;

struct DVariant
{
	union
	{
		float m_Float;
		long m_Int;
		char *m_pString;
		void *m_pData;
		float m_Vector[3];
		int64_t m_Int64;
	};

	int m_Type;
};

struct CRecvProxyData
{
	const RecvProp* m_pRecvProp;
	DVariant m_Value;
	int m_iElement;
	int m_ObjectID;
};

struct RecvTable
{
	RecvProp *m_pProps;
	int m_nProps;
	void *m_pDecoder;
	char *m_pNetTableName;
	bool m_bInitialized;
	bool m_bInMainList;
};

typedef void (*RecvVarProxyFn) (const CRecvProxyData *pData, void *pStruct, void *pOut);

struct RecvProp
{
	char *m_pVarName;
	int m_RecvType;
	int m_Flags;
	int m_StringBufferSize;
	bool m_bInsideArray;
	const void *m_pExtraData;
	RecvProp *m_pArrayProp;
	void *m_ArrayLengthProxy;
	RecvVarProxyFn m_ProxyFn;
	void *m_DataTableProxyFn;
	RecvTable *m_pDataTable;
	int m_Offset;
	int m_ElementStride;
	int m_nElements;
	const char *m_pParentArrayPropName;
};

class IClientNetworkable;
typedef IClientNetworkable* (*CreateClientClassFn)(int entnum, int serialNum);
typedef IClientNetworkable* (*CreateEventFn)();

class ClientClass
{
public:
	CreateClientClassFn m_pCreateFn;
	CreateEventFn *m_pCreateEventFn;
	char* m_pNetworkName;
	RecvTable *m_pRecvTable;
	ClientClass* m_pNext;
	EClassIds m_ClassID;
};

class IBaseClientDLL
{
public:
	ClientClass* GetAllClasses()
	{
		typedef ClientClass* (* oGetAllClasses)(void*);
		return getvfunc<oGetAllClasses>(this, 8)(this);
	}
};

//test

const int k_unSteamAccountInstanceMask = 0x000FFFFF;
typedef int AccountID_t;
enum EChatSteamIDInstanceFlags
{
	k_EChatAccountInstanceMask = 0x00000FFF, // top 8 bits are flags

	k_EChatInstanceFlagClan = ( k_unSteamAccountInstanceMask + 1 ) >> 1,	// top bit
	k_EChatInstanceFlagLobby = ( k_unSteamAccountInstanceMask + 1 ) >> 2,	// next one down, etc
	k_EChatInstanceFlagMMSLobby = ( k_unSteamAccountInstanceMask + 1 ) >> 3,	// next one down, etc

	// Max of 8 flags
};

typedef int HServerQuery;

typedef int AppId_t;
const AppId_t k_uAppIdInvalid = 0x0;
const AppId_t k_nGameIDNotepad = 65535;
const AppId_t k_nGameIDCSSTestApp = 65534;
const AppId_t k_nGameIDDRMTestApp_Static = 6710;
const AppId_t k_nGameIDDRMTestApp_Blob = 6711;
const AppId_t k_nGameIDDRMTestApp_Dynamic = 6712;
const AppId_t k_nGameIDDRMTestApp_SDK = 6713;
const AppId_t k_nGameIDWinUI = 7;
const AppId_t k_nGameIDWinUI2 = 8;
const AppId_t k_nGameIDCS = 10;
const AppId_t k_nGameIDTFC = 20;
const AppId_t k_nGameIDDOD = 30;
const AppId_t k_nGameIDDMC = 40;
const AppId_t k_nGameIDOpFor = 50;
const AppId_t k_nGameIDRicochet = 60;
const AppId_t k_nGameIDHL1 = 70;
const AppId_t k_nGameIDCZero = 80;
const AppId_t k_nGameIDCSBeta = 150;
const AppId_t k_nGameIDMacVAC = 160;
const AppId_t k_nGameIDWinVAC = 202;
const AppId_t k_nGameIDScreenshots = 760;
const AppId_t k_nGameDRMTest = 199;
const AppId_t k_nGameIDBaseSourceSDK = 215;
const AppId_t k_nGameIDHL2 = 220;
const AppId_t k_nDepotHL2Buka = 235;
const AppId_t k_nGameIDCSS = 240;
const AppId_t k_nGameIDCSSBeta = 260;
const AppId_t k_nGameHL1SRC = 280;
const AppId_t k_nGameIDDODSRC = 300;
const AppId_t k_nGameIDHL2DM = 320;
const AppId_t k_nGameIDPortal = 400;
const AppId_t k_nGameIDHL2EP2 = 420;
const AppId_t k_nGameIDTF2 = 440;
const AppId_t k_nGameIDL4D = 500;
const AppId_t k_nGameIDL4DDemo = 530;
const AppId_t k_nGameIDL4D2 = 550;
const AppId_t k_nGameIDASW = 630;
const AppId_t k_nGameIDTF2Staging = 810;
const AppId_t k_nGameIDPortal2Main = 852;
const AppId_t k_nGameIDPortal2 = 620;
const AppId_t k_nGameIDASWMain = 877;
const AppId_t k_nGameIDDOTA = 882;
const AppId_t k_nGameIDASWStaging = 886;
const AppId_t k_nGameIDRedOrchestra = 1200;
const AppId_t k_nGameIDRedOrchestraBeta = 1210;
const AppId_t k_nGameIDKillingFloor = 1250;
const AppId_t k_nGameIDSin1 = 1309;
const AppId_t k_nGameIDEarth2160 = 1900;
const AppId_t k_nGameIDTheShip = 2400;
const AppId_t k_nGameIDTheShipBeta = 2410;
const AppId_t k_nGameIDDarkMessiahSP = 2100;
const AppId_t k_nGameIDDarkMessiahMPBeta = 2110;
const AppId_t k_nGameIDDarkMessiahMP = 2115;
const AppId_t k_nGameIDDarkMessiahSPDemo = 2120;
const AppId_t k_nGameIDDarkMessiahFix = 2130;
const AppId_t k_nGameRaceWTCC = 4230;
const AppId_t k_nGameIDLostPlanetOld = 6500;
const AppId_t k_nGameIDLostPlanet = 6510;
const AppId_t k_nGameIDNBA2K9 = 7740;
const AppId_t k_nGameIDCallofDuty4 = 7940;
const AppId_t k_nMLBFrontOfficeManager = 7780;
const AppId_t k_nGameIDMW2SP = 10180;
const AppId_t k_nGameIDMW2MP = 10190;
const AppId_t k_nGameIDIW5SP = 42680;
const AppId_t k_nGameIDIW5MP = 42690;
const AppId_t k_nGameIDCODBLOPSSP = 42700;
const AppId_t k_nGameIDCODBLOPSMP = 42710;
const AppId_t k_nGameIDEmpireTotalWar = 10500;
const AppId_t k_nGameCSSOnline = 11600;
const AppId_t k_nGameIDFirstSource = 200;
const AppId_t k_nGameIDLastSource = 999;
const AppId_t k_nGameIDFirstGoldSource = 10;
const AppId_t k_nGameIDLastGoldSource = 199;
const AppId_t k_nGameIDFirstNonSource = 1000;
const AppId_t k_nGameIDMax = 2147483647;
const AppId_t k_nGameIDStress = 30020;
const AppId_t k_nGameIDGCTest = 30100;
const AppId_t k_nAppATIDriver_Vista7_32 = 61800;
const AppId_t k_nAppATIDriver_Vista7_64 = 61810;
const AppId_t k_nAppATIDriver_XP_32 = 61820;
const AppId_t k_nAppATIDriver_XP_64 = 61830;


typedef enum EMatchMakingServerResponse
{
	eServerResponded = 0,
	eServerFailedToRespond,
	eNoServersListedOnMasterServer // for the Internet query type, returned in response callback if no servers of this type match
} EMatchMakingServerResponse;

typedef enum EMatchMakingType
{
	eInternetServer = 0,
	eLANServer,
	eFriendsServer,
	eFavoritesServer,
	eHistoryServer,
	eSpectatorServer,
	eInvalidServer 
} EMatchMakingType;
typedef void* HServerListRequest;

class ISteamMatchmakingServerListResponse002
{
public:
	// Server has responded ok with updated data
	virtual void ServerResponded( HServerListRequest hRequest, int iServer ) = 0; 

	// Server has failed to respond
	virtual void ServerFailedToRespond( HServerListRequest hRequest, int iServer ) = 0; 

	// A list refresh you had initiated is now 100% completed
	virtual void RefreshComplete( HServerListRequest hRequest, EMatchMakingServerResponse response ) = 0; 
};

//Typedef to the lastest version of the interface
typedef ISteamMatchmakingServerListResponse002 ISteamMatchmakingServerListResponse;


//==========================  Open Steamworks  ================================
//
// This file is part of the Open Steamworks project. All individuals associated
// with this project do not claim ownership of the contents
//
// The code, comments, and all related files, projects, resources,
// redistributables included with this project are Copyright Valve Corporation.
// Additionally, Valve, the Valve logo, Half-Life, the Half-Life logo, the
// Lambda logo, Steam, the Steam logo, Team Fortress, the Team Fortress logo,
// Opposing Force, Day of Defeat, the Day of Defeat logo, Counter-Strike, the
// Counter-Strike logo, Source, the Source logo, and Counter-Strike Condition
// Zero are trademarks and or registered trademarks of Valve Corporation.
// All other trademarks are property of their respective owners.
//
//=============================================================================


enum EAccountType
{
	k_EAccountTypeInvalid = 0,			
	k_EAccountTypeIndividual = 1,		// single user account
	k_EAccountTypeMultiseat = 2,		// multiseat (e.g. cybercafe) account
	k_EAccountTypeGameServer = 3,		// game server account
	k_EAccountTypeAnonGameServer = 4,	// anonymous game server account
	k_EAccountTypePending = 5,			// pending
	k_EAccountTypeContentServer = 6,	// content server
	k_EAccountTypeClan = 7,
	k_EAccountTypeChat = 8,
	k_EAccountTypeConsoleUser = 9,		// Fake SteamID for local PSN account on PS3 or Live account on 360, etc.
	k_EAccountTypeAnonUser = 10,

	// Max of 16 items in this field
	k_EAccountTypeMax
};
enum EUniverse
{
	k_EUniverseInvalid = 0,
	k_EUniversePublic = 1,
	k_EUniverseBeta = 2,
	k_EUniverseInternal = 3,
	k_EUniverseDev = 4,
//	k_EUniverseRC = 5, // Removed

	k_EUniverseMax
};
#pragma pack( push, 1 )
const unsigned int k_unSteamAccountIDMask = 0xFFFFFFFF;
// we allow 3 simultaneous user account instances right now, 1= desktop, 2 = console, 4 = web, 0 = all
const unsigned int k_unSteamUserDesktopInstance = 1;	 
const unsigned int k_unSteamUserConsoleInstance = 2;
const unsigned int k_unSteamUserWebInstance		= 4;
// Steam ID structure (64 bits total)
class CSteamID
{
public:

	//-----------------------------------------------------------------------------
	// Purpose: Constructor
	//-----------------------------------------------------------------------------
	CSteamID()
	{
		m_steamid.m_comp.m_unAccountID = 0;
		m_steamid.m_comp.m_EAccountType = k_EAccountTypeInvalid;
		m_steamid.m_comp.m_EUniverse = k_EUniverseInvalid;
		m_steamid.m_comp.m_unAccountInstance = 0;
	}


	//-----------------------------------------------------------------------------
	// Purpose: Constructor
	// Input  : unAccountID -	32-bit account ID
	//			eUniverse -		Universe this account belongs to
	//			eAccountType -	Type of account
	//-----------------------------------------------------------------------------
	CSteamID( int unAccountID, EUniverse eUniverse, EAccountType eAccountType )
	{
		Set( unAccountID, eUniverse, eAccountType );
	}


	//-----------------------------------------------------------------------------
	// Purpose: Constructor
	// Input  : unAccountID -	32-bit account ID
	//			unAccountInstance - instance
	//			eUniverse -		Universe this account belongs to
	//			eAccountType -	Type of account
	//-----------------------------------------------------------------------------
	CSteamID( int unAccountID, unsigned int unAccountInstance, EUniverse eUniverse, EAccountType eAccountType )
	{
#if defined(_SERVER) && defined(Assert)
		Assert( ! ( ( k_EAccountTypeIndividual == eAccountType ) && ( unAccountInstance > k_unSteamUserWebInstance ) ) );	// enforce that for individual accounts, instance is always 1
#endif // _SERVER
		InstancedSet( unAccountID, unAccountInstance, eUniverse, eAccountType );
	}


	//-----------------------------------------------------------------------------
	// Purpose: Constructor
	// Input  : ulSteamID -		64-bit representation of a Steam ID
	// Note:	Will not accept a int or int32 as input, as that is a probable mistake.
	//			See the stubbed out overloads in the private: section for more info.
	//-----------------------------------------------------------------------------
	CSteamID( int ulSteamID )
	{
		SetFromint( ulSteamID );
	}


	//-----------------------------------------------------------------------------
	// Purpose: Sets parameters for steam ID
	// Input  : unAccountID -	32-bit account ID
	//			eUniverse -		Universe this account belongs to
	//			eAccountType -	Type of account
	//-----------------------------------------------------------------------------
	void Set( int unAccountID, EUniverse eUniverse, EAccountType eAccountType )
	{
		m_steamid.m_comp.m_unAccountID = unAccountID;
		m_steamid.m_comp.m_EUniverse = eUniverse;
		m_steamid.m_comp.m_EAccountType = eAccountType;

		if ( eAccountType == k_EAccountTypeClan )
		{
			m_steamid.m_comp.m_unAccountInstance = 0;
		}
		else
		{
			// by default we pick the desktop instance
			m_steamid.m_comp.m_unAccountInstance = k_unSteamUserDesktopInstance;
		}
	}


	//-----------------------------------------------------------------------------
	// Purpose: Sets parameters for steam ID
	// Input  : unAccountID -	32-bit account ID
	//			eUniverse -		Universe this account belongs to
	//			eAccountType -	Type of account
	//-----------------------------------------------------------------------------
	void InstancedSet( int unAccountID, int unInstance, EUniverse eUniverse, EAccountType eAccountType )
	{
		m_steamid.m_comp.m_unAccountID = unAccountID;
		m_steamid.m_comp.m_EUniverse = eUniverse;
		m_steamid.m_comp.m_EAccountType = eAccountType;
		m_steamid.m_comp.m_unAccountInstance = unInstance;
	}


	//-----------------------------------------------------------------------------
	// Purpose: Initializes a steam ID from its 52 bit parts and universe/type
	// Input  : ulIdentifier - 52 bits of goodness
	//-----------------------------------------------------------------------------
	void FullSet( int ulIdentifier, EUniverse eUniverse, EAccountType eAccountType )
	{
		m_steamid.m_comp.m_unAccountID = ( ulIdentifier & k_unSteamAccountIDMask );						// account ID is low 32 bits
		m_steamid.m_comp.m_unAccountInstance = ( ( ulIdentifier >> 32 ) & k_unSteamAccountInstanceMask );			// account instance is next 20 bits
		m_steamid.m_comp.m_EUniverse = eUniverse;
		m_steamid.m_comp.m_EAccountType = eAccountType;
	}


	//-----------------------------------------------------------------------------
	// Purpose: Initializes a steam ID from its 64-bit representation
	// Input  : ulSteamID -		64-bit representation of a Steam ID
	//-----------------------------------------------------------------------------
	void SetFromint( int ulSteamID )
	{
		m_steamid.m_unAll64Bits = ulSteamID;
	}

#ifdef TSTEAMGLOBALUSERID_H // NO_STEAM would not include TSteamGlobalUserID.h, thus we lose this facet of CSteamID
	//-----------------------------------------------------------------------------
	// Purpose: Initializes a steam ID from a Steam2 ID structure
	// Input:	pTSteamGlobalUserID -	Steam2 ID to convert
	//			eUniverse -				universe this ID belongs to
	//-----------------------------------------------------------------------------
	void SetFromSteam2( TSteamGlobalUserID *pTSteamGlobalUserID, EUniverse eUniverse )
	{
		m_steamid.m_comp.m_unAccountID = pTSteamGlobalUserID->m_SteamLocalUserID.Split.Low32bits * 2 + 
			pTSteamGlobalUserID->m_SteamLocalUserID.Split.High32bits;
		m_steamid.m_comp.m_EUniverse = eUniverse;		// set the universe
		m_steamid.m_comp.m_EAccountType = k_EAccountTypeIndividual; // Steam 2 accounts always map to account type of individual
		m_steamid.m_comp.m_unAccountInstance = k_unSteamUserDesktopInstance; // Steam2 only knew desktop instances
	}

	//-----------------------------------------------------------------------------
	// Purpose: Fills out a Steam2 ID structure
	// Input:	pTSteamGlobalUserID -	Steam2 ID to write to
	//-----------------------------------------------------------------------------
	void ConvertToSteam2( TSteamGlobalUserID *pTSteamGlobalUserID ) const
	{
		// only individual accounts have any meaning in Steam 2, only they can be mapped
		// Assert( m_steamid.m_comp.m_EAccountType == k_EAccountTypeIndividual );

		pTSteamGlobalUserID->m_SteamInstanceID = 0;
		pTSteamGlobalUserID->m_SteamLocalUserID.Split.High32bits = m_steamid.m_comp.m_unAccountID % 2;
		pTSteamGlobalUserID->m_SteamLocalUserID.Split.Low32bits = m_steamid.m_comp.m_unAccountID / 2;
	}
#endif // TSTEAMGLOBALUSERID_H

	//-----------------------------------------------------------------------------
	// Purpose: Converts steam ID to its 64-bit representation
	// Output : 64-bit representation of a Steam ID
	//-----------------------------------------------------------------------------
	int ConvertToint() const
	{
		return m_steamid.m_unAll64Bits;
	}


	//-----------------------------------------------------------------------------
	// Purpose: Converts the static parts of a steam ID to a 64-bit representation.
	//			For multiseat accounts, all instances of that account will have the
	//			same static account key, so they can be grouped together by the static
	//			account key.
	// Output : 64-bit static account key
	//-----------------------------------------------------------------------------
	int GetStaticAccountKey() const
	{
		// note we do NOT include the account instance (which is a dynamic property) in the static account key
		return (int) ( ( ( (int) m_steamid.m_comp.m_EUniverse ) << 56 ) + ((int) m_steamid.m_comp.m_EAccountType << 52 ) + m_steamid.m_comp.m_unAccountID );
	}


	//-----------------------------------------------------------------------------
	// Purpose: create an anonymous game server login to be filled in by the AM
	//-----------------------------------------------------------------------------
	void CreateBlankAnonLogon( EUniverse eUniverse )
	{
		m_steamid.m_comp.m_unAccountID = 0;
		m_steamid.m_comp.m_EAccountType = k_EAccountTypeAnonGameServer;
		m_steamid.m_comp.m_EUniverse = eUniverse;
		m_steamid.m_comp.m_unAccountInstance = 0;
	}


	//-----------------------------------------------------------------------------
	// Purpose: create an anonymous game server login to be filled in by the AM
	//-----------------------------------------------------------------------------
	void CreateBlankAnonUserLogon( EUniverse eUniverse )
	{
		m_steamid.m_comp.m_unAccountID = 0;
		m_steamid.m_comp.m_EAccountType = k_EAccountTypeAnonUser;
		m_steamid.m_comp.m_EUniverse = eUniverse;
		m_steamid.m_comp.m_unAccountInstance = 0;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Is this an anonymous game server login that will be filled in?
	//-----------------------------------------------------------------------------
	bool BBlankAnonAccount() const
	{
		return m_steamid.m_comp.m_unAccountID == 0 && BAnonAccount() && m_steamid.m_comp.m_unAccountInstance == 0;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Is this a game server account id?
	//-----------------------------------------------------------------------------
	bool BGameServerAccount() const
	{
		return m_steamid.m_comp.m_EAccountType == k_EAccountTypeGameServer || m_steamid.m_comp.m_EAccountType == k_EAccountTypeAnonGameServer;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Is this a content server account id?
	//-----------------------------------------------------------------------------
	bool BContentServerAccount() const
	{
		return m_steamid.m_comp.m_EAccountType == k_EAccountTypeContentServer;
	}


	//-----------------------------------------------------------------------------
	// Purpose: Is this a clan account id?
	//-----------------------------------------------------------------------------
	bool BClanAccount() const
	{
		return m_steamid.m_comp.m_EAccountType == k_EAccountTypeClan;
	}


	//-----------------------------------------------------------------------------
	// Purpose: Is this a chat account id?
	//-----------------------------------------------------------------------------
	bool BChatAccount() const
	{
		return m_steamid.m_comp.m_EAccountType == k_EAccountTypeChat;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Is this a chat account id?
	//-----------------------------------------------------------------------------
	bool IsLobby() const
	{
		return ( m_steamid.m_comp.m_EAccountType == k_EAccountTypeChat )
			&& ( m_steamid.m_comp.m_unAccountInstance & k_EChatInstanceFlagLobby );
	}


	//-----------------------------------------------------------------------------
	// Purpose: Is this an individual user account id?
	//-----------------------------------------------------------------------------
	bool BIndividualAccount() const
	{
		return m_steamid.m_comp.m_EAccountType == k_EAccountTypeIndividual || m_steamid.m_comp.m_EAccountType == k_EAccountTypeConsoleUser;
	}


	//-----------------------------------------------------------------------------
	// Purpose: Is this an anonymous account?
	//-----------------------------------------------------------------------------
	bool BAnonAccount() const
	{
		return m_steamid.m_comp.m_EAccountType == k_EAccountTypeAnonUser || m_steamid.m_comp.m_EAccountType == k_EAccountTypeAnonGameServer;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Is this an anonymous user account? ( used to create an account or reset a password )
	//-----------------------------------------------------------------------------
	bool BAnonUserAccount() const
	{
		return m_steamid.m_comp.m_EAccountType == k_EAccountTypeAnonUser;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Is this a faked up Steam ID for a PSN friend account?
	//-----------------------------------------------------------------------------
	bool BConsoleUserAccount() const
	{
		return m_steamid.m_comp.m_EAccountType == k_EAccountTypeConsoleUser;
	}


	// simple accessors
	void SetAccountID( int unAccountID )		{ m_steamid.m_comp.m_unAccountID = unAccountID; }
	void SetAccountInstance( int unInstance ){ m_steamid.m_comp.m_unAccountInstance = unInstance; }
	void ClearIndividualInstance()				{ if ( BIndividualAccount() ) m_steamid.m_comp.m_unAccountInstance = 0; }
	bool HasNoIndividualInstance() const		{ return BIndividualAccount() && (m_steamid.m_comp.m_unAccountInstance==0); }
	AccountID_t GetAccountID() const			{ return m_steamid.m_comp.m_unAccountID; }
	int GetUnAccountInstance() const			{ return m_steamid.m_comp.m_unAccountInstance; }
	EAccountType GetEAccountType() const		{ return ( EAccountType ) m_steamid.m_comp.m_EAccountType; }
	EUniverse GetEUniverse() const				{ return m_steamid.m_comp.m_EUniverse; }
	void SetEUniverse( EUniverse eUniverse )	{ m_steamid.m_comp.m_EUniverse = eUniverse; }
	bool IsValid() const;

	// this set of functions is hidden, will be moved out of class
	explicit CSteamID( const char *pchSteamID, EUniverse eDefaultUniverse = k_EUniverseInvalid );

#ifdef _MSC_VER
	#pragma warning(push) 
	#pragma warning(disable: 4996) 
#endif

	const char * Render() const				// renders this steam ID to string
	{
		const int k_cBufLen = 30;
		const int k_cBufs = 4;
		char* pchBuf;

		static char rgchBuf[k_cBufs][k_cBufLen];
		static int nBuf = 0;

		pchBuf = rgchBuf[nBuf++];
		nBuf %= k_cBufs;

		switch(m_steamid.m_comp.m_EAccountType)
		{
		case k_EAccountTypeInvalid:
		case k_EAccountTypeIndividual:
			sprintf(pchBuf, "STEAM_0:%u:%u", (m_steamid.m_comp.m_unAccountID % 2) ? 1 : 0, (int)m_steamid.m_comp.m_unAccountID / 2);
			break;
		default:
			sprintf(pchBuf, "%llu", ConvertToint());
		}
		return pchBuf;
	}
	static const char * Render( int ulSteamID )	// static method to render a int representation of a steam ID to a string
	{
		return CSteamID(ulSteamID).Render();
	}

	const char *SteamRender() const // renders this steam ID to string using the new rendering style
	{
		const int k_cBufLen = 37;
		const int k_cBufs = 4;
		char* pchBuf;

		static char rgchBuf[k_cBufs][k_cBufLen];
		static int nBuf = 0;

		pchBuf = rgchBuf[nBuf++];
		nBuf %= k_cBufs;

		switch (m_steamid.m_comp.m_EAccountType)
		{
		case k_EAccountTypeAnonGameServer:
			sprintf(pchBuf, "[A:%u:%u:%u]", m_steamid.m_comp.m_EUniverse, m_steamid.m_comp.m_unAccountID, m_steamid.m_comp.m_unAccountInstance);
			break;
		case k_EAccountTypeGameServer:
			sprintf(pchBuf, "[G:%u:%u]", m_steamid.m_comp.m_EUniverse, m_steamid.m_comp.m_unAccountID);
			break;
		case k_EAccountTypeMultiseat:
			sprintf(pchBuf, "[M:%u:%u:%u]", m_steamid.m_comp.m_EUniverse, m_steamid.m_comp.m_unAccountID, m_steamid.m_comp.m_unAccountInstance);
			break;
		case k_EAccountTypePending:
			sprintf(pchBuf, "[P:%u:%u]", m_steamid.m_comp.m_EUniverse, m_steamid.m_comp.m_unAccountID);
			break;
		case k_EAccountTypeContentServer:
			sprintf(pchBuf, "[C:%u:%u]", m_steamid.m_comp.m_EUniverse, m_steamid.m_comp.m_unAccountID);
			break;
		case k_EAccountTypeClan:
			sprintf(pchBuf, "[g:%u:%u]", m_steamid.m_comp.m_EUniverse, m_steamid.m_comp.m_unAccountID);
			break;
		case k_EAccountTypeChat:
			switch (m_steamid.m_comp.m_unAccountInstance & ~k_EChatAccountInstanceMask)
			{
			case k_EChatInstanceFlagClan:
				sprintf(pchBuf, "[c:%u:%u]", m_steamid.m_comp.m_EUniverse, m_steamid.m_comp.m_unAccountID);
				break;
			case k_EChatInstanceFlagLobby:
				sprintf(pchBuf, "[L:%u:%u]", m_steamid.m_comp.m_EUniverse, m_steamid.m_comp.m_unAccountID);
				break;
			default:
				sprintf(pchBuf, "[T:%u:%u]", m_steamid.m_comp.m_EUniverse, m_steamid.m_comp.m_unAccountID);
				break;
			}
			break;
		case k_EAccountTypeInvalid:
			sprintf(pchBuf, "[I:%u:%u]", m_steamid.m_comp.m_EUniverse, m_steamid.m_comp.m_unAccountID);
			break;
		case k_EAccountTypeIndividual:
			sprintf(pchBuf, "[U:%u:%u]", m_steamid.m_comp.m_EUniverse, m_steamid.m_comp.m_unAccountID);
			break;
		default:
			sprintf(pchBuf, "[i:%u:%u]", m_steamid.m_comp.m_EUniverse, m_steamid.m_comp.m_unAccountID);
			break;
		}

		return pchBuf;
	}

	static const char *SteamRender( int ulSteamID )	// static method to render a int representation of a steam ID to a string
	{
		return CSteamID(ulSteamID).SteamRender();
	}

#ifdef _MSC_VER
	#pragma warning(pop) 
#endif

	void SetFromString( const char *pchSteamID, EUniverse eDefaultUniverse );
	bool SetFromSteam2String( const char *pchSteam2ID, EUniverse eUniverse );

	inline bool operator==( const CSteamID &val ) const { return m_steamid.m_unAll64Bits == val.m_steamid.m_unAll64Bits; }
	inline bool operator!=( const CSteamID &val ) const { return !operator==( val ); }
	inline bool operator<( const CSteamID &val ) const { return m_steamid.m_unAll64Bits < val.m_steamid.m_unAll64Bits; }
	inline bool operator>( const CSteamID &val ) const { return m_steamid.m_unAll64Bits > val.m_steamid.m_unAll64Bits; }

	// DEBUG function
	bool BValidExternalSteamID() const;

private:
	// These are defined here to prevent accidental implicit conversion of a u32AccountID to a CSteamID.
	// If you get a compiler error about an ambiguous constructor/function then it may be because you're
	// passing a 32-bit int to a function that takes a CSteamID. You should explicitly create the SteamID
	// using the correct Universe and account Type/Instance values.


	// 64 bits total
	union SteamID_t
	{
		struct SteamIDComponent_t
		{
#ifdef VALVE_BIG_ENDIAN
			EUniverse			m_EUniverse : 8;	// universe this account belongs to
			unsigned int		m_EAccountType : 4;			// type of account - can't show as EAccountType, due to signed / unsigned difference
			unsigned int		m_unAccountInstance : 20;	// dynamic instance ID
			int				m_unAccountID : 32;			// unique account identifier
#else
			int				m_unAccountID : 32;			// unique account identifier
			unsigned int		m_unAccountInstance : 20;	// dynamic instance ID
			unsigned int		m_EAccountType : 4;			// type of account - can't show as EAccountType, due to signed / unsigned difference
			EUniverse			m_EUniverse : 8;	// universe this account belongs to
#endif
		} m_comp;

		int m_unAll64Bits;
	} m_steamid;
};

#ifndef NO_CSTEAMID_STL
inline std::ostream& operator<<(std::ostream& out, const CSteamID& s)
{
//	out << s.Render();
	return out;
}
#endif //NO_CSTEAMID_STL

inline bool CSteamID::IsValid() const
{
	if ( m_steamid.m_comp.m_EAccountType <= k_EAccountTypeInvalid || m_steamid.m_comp.m_EAccountType >= k_EAccountTypeMax )
		return false;
	
	if ( m_steamid.m_comp.m_EUniverse <= k_EUniverseInvalid || m_steamid.m_comp.m_EUniverse >= k_EUniverseMax )
		return false;

	if ( m_steamid.m_comp.m_EAccountType == k_EAccountTypeIndividual )
	{
		if ( m_steamid.m_comp.m_unAccountID == 0 || m_steamid.m_comp.m_unAccountInstance > k_unSteamUserWebInstance )
			return false;
	}

	if ( m_steamid.m_comp.m_EAccountType == k_EAccountTypeClan )
	{
		if ( m_steamid.m_comp.m_unAccountID == 0 || m_steamid.m_comp.m_unAccountInstance != 0 )
			return false;
	}

	if ( m_steamid.m_comp.m_EAccountType == k_EAccountTypeGameServer )
	{
		if ( m_steamid.m_comp.m_unAccountID == 0 )
			return false;
		// Any limit on instances?  We use them for local users and bots
	}
	return true;
}

// generic invalid CSteamID
#define k_steamIDNil CSteamID()

// This steamID comes from a user game connection to an out of date GS that hasnt implemented the protocol
// to provide its steamID
#define k_steamIDOutofDateGS CSteamID( 0, 0, k_EUniverseInvalid, k_EAccountTypeInvalid )
// This steamID comes from a user game connection to an sv_lan GS
#define k_steamIDLanModeGS CSteamID( 0, 0, k_EUniversePublic, k_EAccountTypeInvalid )
// This steamID can come from a user game connection to a GS that has just booted but hasnt yet even initialized
// its steam3 component and started logging on.
#define k_steamIDNotInitYetGS CSteamID( 1, 0, k_EUniverseInvalid, k_EAccountTypeInvalid )
// This steamID can come from a user game connection to a GS that isn't using the steam authentication system but still
// wants to support the "Join Game" option in the friends list
#define k_steamIDNonSteamGS CSteamID( 2, 0, k_EUniverseInvalid, k_EAccountTypeInvalid )


#ifdef STEAM
// Returns the matching chat steamID, with the default instance of 0
// If the steamID passed in is already of type k_EAccountTypeChat it will be returned with the same instance
inline CSteamID ChatIDFromSteamID( const CSteamID &steamID )
{
	if(steamID.GetEAccountType() == k_EAccountTypeClan)
	{
		return CSteamID(steamID.GetAccountID(), k_EChatInstanceFlagClan, steamID.GetEUniverse(), k_EAccountTypeChat);
	}
	else if(steamID.GetEAccountType() == k_EAccountTypeChat)
	{
		return steamID;
	}
	else
	{
		return k_steamIDNil;
	}
}

// Returns the matching clan steamID, with the default instance of 0
// If the steamID passed in is already of type k_EAccountTypeClan it will be returned with the same instance
inline CSteamID ClanIDFromSteamID( const CSteamID &steamID )
{
	if(steamID.GetEAccountType() == k_EAccountTypeChat && steamID.GetUnAccountInstance() & k_EChatInstanceFlagClan)
	{
		return CSteamID(steamID.GetAccountID(), 0, steamID.GetEUniverse(), k_EAccountTypeClan);
	}
	else if(steamID.GetEAccountType() == k_EAccountTypeClan)
	{
		return steamID;
	}
	else
	{
		return k_steamIDNil;
	}
}

// Asserts steamID type before conversion
CSteamID ChatIDFromClanID( const CSteamID &steamIDClan );
// Asserts steamID type before conversion
CSteamID ClanIDFromChatID( const CSteamID &steamIDChat );

#endif // _STEAM
const int k_cbMaxGameServerGameDir = 32;
const int k_cbMaxGameServerMapName = 32;
const int k_cbMaxGameServerGameDescription = 64;
const int k_cbMaxGameServerName = 64;
const int k_cbMaxGameServerTags = 128;
const int k_cbMaxGameServerGameData = 2048;

/// Store key/value pair used in matchmaking queries.
///
/// Actually, the name Key/Value is a bit misleading.  The "key" is better
/// understood as "filter operation code" and the "value" is the operand to this
/// filter operation.  The meaning of the operand depends upon the filter.
struct MatchMakingKeyValuePair_t
{
	MatchMakingKeyValuePair_t() { m_szKey[0] = m_szValue[0] = 0; }
	MatchMakingKeyValuePair_t( const char *pchKey, const char *pchValue )
	{
		strncpy( m_szKey, pchKey, sizeof(m_szKey) ); // this is a public header, use basic c library string funcs only!
		m_szKey[ sizeof( m_szKey ) - 1 ] = '\0';
		strncpy( m_szValue, pchValue, sizeof(m_szValue) );
		m_szValue[ sizeof( m_szValue ) - 1 ] = '\0';
	}
	char m_szKey[ 256 ];
	char m_szValue[ 256 ];
};




// servernetadr_t is all the addressing info the serverbrowser needs to know about a game server,
// namely: its IP, its connection port, and its query port.
class servernetadr_t 
{
public:

	servernetadr_t() : m_usConnectionPort( 0 ), m_usQueryPort( 0 ), m_unIP( 0 ) {}
	
	void	Init( unsigned int ip, int usQueryPort, int usConnectionPort );
#ifdef NETADR_H
	netadr_t	GetIPAndQueryPort();
#endif
	
	// Access the query port.
	int	GetQueryPort() const;
	void	SetQueryPort( int usPort );

	// Access the connection port.
	int	GetConnectionPort() const;
	void	SetConnectionPort( int usPort );

	// Access the IP
	int GetIP() const;
	void SetIP( int );

	// This gets the 'a.b.c.d:port' string with the connection port (instead of the query port).
	const char *GetConnectionAddressString() const;
	const char *GetQueryAddressString() const;

	// Comparison operators and functions.
	bool	operator<(const servernetadr_t &netadr) const;
	void operator=( const servernetadr_t &that )
	{
		m_usConnectionPort = that.m_usConnectionPort;
		m_usQueryPort = that.m_usQueryPort;
		m_unIP = that.m_unIP;
	}

	
private:
	const char *ToString( int unIP, int usPort ) const;
	int	m_usConnectionPort;	// (in HOST byte order)
	int	m_usQueryPort;
	int  m_unIP;
};


inline void	servernetadr_t::Init( unsigned int ip, int usQueryPort, int usConnectionPort )
{
	m_unIP = ip;
	m_usQueryPort = usQueryPort;
	m_usConnectionPort = usConnectionPort;
}

#ifdef NETADR_H
inline netadr_t servernetadr_t::GetIPAndQueryPort()
{
	return netadr_t( m_unIP, m_usQueryPort );
}
#endif

inline int servernetadr_t::GetQueryPort() const
{
	return m_usQueryPort;
}

inline void	servernetadr_t::SetQueryPort( int usPort )
{
	m_usQueryPort = usPort;
}

inline int servernetadr_t::GetConnectionPort() const
{
	return m_usConnectionPort;
}

inline void	servernetadr_t::SetConnectionPort( int usPort )
{
	m_usConnectionPort = usPort;
}

inline int servernetadr_t::GetIP() const
{
	return m_unIP;
}

inline void	servernetadr_t::SetIP( int unIP )
{
	m_unIP = unIP;
}

inline const char *servernetadr_t::ToString( int unIP, int usPort ) const
{
	static char s[4][64];
	static int nBuf = 0;
	unsigned char *ipByte = (unsigned char *)&unIP;
#ifdef VALVE_BIG_ENDIAN
	V_snprintf (s[nBuf], sizeof( s[nBuf] ), "%u.%u.%u.%u:%i", (int)(ipByte[0]), (int)(ipByte[1]), (int)(ipByte[2]), (int)(ipByte[3]), usPort );
#else
//	V_snprintf (s[nBuf], sizeof( s[nBuf] ), "%u.%u.%u.%u:%i", (int)(ipByte[3]), (int)(ipByte[2]), (int)(ipByte[1]), (int)(ipByte[0]), usPort );
#endif
	const char *pchRet = s[nBuf];
	++nBuf;
	nBuf %= ( (sizeof(s)/sizeof(s[0])) );
	return pchRet;
}

inline const char* servernetadr_t::GetConnectionAddressString() const
{
	return ToString( m_unIP, m_usConnectionPort );
}

inline const char* servernetadr_t::GetQueryAddressString() const
{
	return ToString( m_unIP, m_usQueryPort );	
}

inline bool servernetadr_t::operator<(const servernetadr_t &netadr) const
{
	return ( m_unIP < netadr.m_unIP ) || ( m_unIP == netadr.m_unIP && m_usQueryPort < netadr.m_usQueryPort );
}

#pragma pack( pop )


class gameserveritem_t
{
public:
	gameserveritem_t();

	const char* GetName() const;
	void SetName( const char *pName );

public:
	servernetadr_t m_NetAdr;		// IP/Query Port/Connection Port for this server
	int m_nPing;					// current ping time in milliseconds
	bool m_bHadSuccessfulResponse;	// server has responded successfully in the past
	bool m_bDoNotRefresh;			// server is marked as not responding and should no longer be refreshed
	char m_szGameDir[32];			// current game directory
	char m_szMap[32];				// current map
	char m_szGameDescription[64];	// game description
	int m_nAppID;				// Steam App ID of this server
	int m_nPlayers;					// current number of players on the server
	int m_nMaxPlayers;				// Maximum players that can join this server
	int m_nBotPlayers;				// Number of bots (i.e simulated players) on this server
	bool m_bPassword;				// true if this server needs a password to join
	bool m_bSecure;					// Is this server protected by VAC
	int m_ulTimeLastPlayed;		// time (in unix time) when this server was last played on (for favorite/history servers)
	int	m_nServerVersion;			// server version as reported to Steam

private:
	char m_szServerName[64];		//  Game server name

	// For data added after SteamMatchMaking001 add it here
public:
	char m_szGameTags[128];			// the tags this server exposes
	CSteamID m_steamID;				// steamID of the game server - invalid if it's doesn't have one (old server, or not connected to Steam)
};


inline gameserveritem_t::gameserveritem_t()
{
	m_szGameDir[0] = m_szMap[0] = m_szGameDescription[0] = m_szServerName[0] = 0;
	m_bHadSuccessfulResponse = m_bDoNotRefresh = m_bPassword = m_bSecure = false;
	m_nPing = m_nAppID = m_nPlayers = m_nMaxPlayers = m_nBotPlayers = m_ulTimeLastPlayed = m_nServerVersion = 0;
	m_szGameTags[0] = 0;
}

inline const char* gameserveritem_t::GetName() const
{
	// Use the IP address as the name if nothing is set yet.
	if ( m_szServerName[0] == 0 )
		return m_NetAdr.GetConnectionAddressString();
	else
		return m_szServerName;
}

#ifdef _S4N_
	#define strncpy(...)
#elif defined(_MSC_VER)
	#pragma warning(push) 
	#pragma warning(disable: 4996) 
#endif

inline void gameserveritem_t::SetName( const char *pName )
{
	strncpy( m_szServerName, pName, sizeof( m_szServerName ) );
	m_szServerName[ sizeof( m_szServerName ) - 1 ] = '\0';
}

class ISteamMatchmakingPingResponse
{
public:
	// Server has responded successfully and has updated data
	virtual void ServerResponded( gameserveritem_t &server ) = 0;

	// Server failed to respond to the ping request
	virtual void ServerFailedToRespond() = 0;
};
class ISteamMatchmakingRulesResponse
{
public:
	// Got data on a rule on the server -- you'll get one of these per rule defined on
	// the server you are querying
	virtual void RulesResponded( const char *pchRule, const char *pchValue ) = 0;

	// The server failed to respond to the request for rule details
	virtual void RulesFailedToRespond() = 0;

	// The server has finished responding to the rule details request 
	// (ie, you won't get anymore RulesResponded callbacks)
	virtual void RulesRefreshComplete() = 0;
};
class ISteamMatchmakingPlayersResponse
{
public:
	// Got data on a new player on the server -- you'll get this callback once per player
	// on the server which you have requested player data on.
	virtual void AddPlayerToList( const char *pchName, int nScore, float flTimePlayed ) = 0;

	// The server failed to respond to the request for player details
	virtual void PlayersFailedToRespond() = 0;

	// The server has finished responding to the player details request 
	// (ie, you won't get anymore AddPlayerToList callbacks)
	virtual void PlayersRefreshComplete() = 0;
};

class SteamMatchmakingServers002
{
public:
	// Request a new list of servers of a particular type.  These calls each correspond to one of the EMatchMakingType values.
	// Each call allocates a new asynchronous request object.
	// Request object must be released by calling ReleaseRequest( hServerListRequest )
	virtual HServerListRequest RequestInternetServerList( AppId_t iApp, MatchMakingKeyValuePair_t **ppchFilters, int nFilters, ISteamMatchmakingServerListResponse *pRequestServersResponse );
	virtual HServerListRequest RequestLANServerList( AppId_t iApp, ISteamMatchmakingServerListResponse *pRequestServersResponse );
	virtual HServerListRequest RequestFriendsServerList( AppId_t iApp, MatchMakingKeyValuePair_t **ppchFilters, int nFilters, ISteamMatchmakingServerListResponse *pRequestServersResponse );
	virtual HServerListRequest RequestFavoritesServerList( AppId_t iApp, MatchMakingKeyValuePair_t **ppchFilters, int nFilters, ISteamMatchmakingServerListResponse *pRequestServersResponse );
	virtual HServerListRequest RequestHistoryServerList( AppId_t iApp, MatchMakingKeyValuePair_t **ppchFilters, int nFilters, ISteamMatchmakingServerListResponse *pRequestServersResponse );
	virtual HServerListRequest RequestSpectatorServerList( AppId_t iApp, MatchMakingKeyValuePair_t **ppchFilters, int nFilters, ISteamMatchmakingServerListResponse *pRequestServersResponse );
    
	// Releases the asynchronous request object and cancels any pending query on it if there's a pending query in progress.
	// RefreshComplete callback is not posted when request is released.
	virtual void ReleaseRequest( HServerListRequest hServerListRequest );
    
	/* the filters that are available in the ppchFilters params are:
     
     "map"		- map the server is running, as set in the dedicated server api
     "dedicated" - reports bDedicated from the API
     "secure"	- VAC-enabled
     "full"		- not full
     "empty"		- not empty
     "noplayers" - is empty
     "proxy"		- a relay server
     
     */
    
	// Get details on a given server in the list, you can get the valid range of index
	// values by calling GetServerCount().  You will also receive index values in
	// ISteamMatchmakingServerListResponse::ServerResponded() callbacks
	virtual gameserveritem_t *GetServerDetails( HServerListRequest hRequest, int iServer );
    
	// Cancel an request which is operation on the given list type.  You should call this to cancel
	// any in-progress requests before destructing a callback object that may have been passed
	// to one of the above list request calls.  Not doing so may result in a crash when a callback
	// occurs on the destructed object.
	// Canceling a query does not release the allocated request handle.
	// The request handle must be released using ReleaseRequest( hRequest )
	virtual void CancelQuery( HServerListRequest hRequest );
    
	// Ping every server in your list again but don't update the list of servers
	// Query callback installed when the server list was requested will be used
	// again to post notifications and RefreshComplete, so the callback must remain
	// valid until another RefreshComplete is called on it or the request
	// is released with ReleaseRequest( hRequest )
	virtual void RefreshQuery( HServerListRequest hRequest );
    
	// Returns true if the list is currently refreshing its server list
	virtual bool IsRefreshing( HServerListRequest hRequest );
    
	// How many servers in the given list, GetServerDetails above takes 0... GetServerCount() - 1
	virtual int GetServerCount( HServerListRequest hRequest );
    
	// Refresh a single server inside of a query (rather than all the servers )
	virtual void RefreshServer( HServerListRequest hRequest, int iServer );
    
    
	//-----------------------------------------------------------------------------
	// Queries to individual servers directly via IP/Port
	//-----------------------------------------------------------------------------
    
	// Request updated ping time and other details from a single server
	virtual HServerQuery PingServer( int unIP, int usPort, ISteamMatchmakingPingResponse *pRequestServersResponse );
    
	// Request the list of players currently playing on a server
	virtual HServerQuery PlayerDetails( int unIP, int usPort, ISteamMatchmakingPlayersResponse *pRequestServersResponse );
    
	// Request the list of rules that the server is running (See ISteamMasterServerUpdater->SetKeyValue() to set the rules server side)
	virtual HServerQuery ServerRules( int unIP, int usPort, ISteamMatchmakingRulesResponse *pRequestServersResponse );
    
	// Cancel an outstanding Ping/Players/Rules query from above.  You should call this to cancel
	// any in-progress requests before destructing a callback object that may have been passed
	// to one of the above calls to avoid crashing when callbacks occur.
	virtual void CancelServerQuery( HServerQuery hServerQuery );
};
