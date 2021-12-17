#include "main.h"

CNetvarManager g_pNetvarManager;

void CNetvarManager::SetColor( int color )
{
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), color );
}

int CNetvarManager::GetNetvarOld( char *szClassName, char *szVariable )
{
	ClientClass *pClass = g_pClient->GetAllClasses();

	for( ; pClass; pClass = pClass->NextClass() )
	{
		RecvTable *pTable = pClass->GetTable();

		if( !pTable ) continue;

		if( pTable->GetNumProps() <= 1 ) continue;

		for( int i = 0; i < pTable->GetNumProps(); i++ )
		{
			RecvProp *pProp = pTable->GetProp( i );

			if( !pProp ) continue;

			if( !strcmp( pTable->GetName(), szClassName ) && !strcmp( pProp->GetName(), szVariable ) )
			{
				/*	g_pNetvarManager.SetColor( 3 );
				printf( "> %s: ", pProp->GetName() );
				g_pNetvarManager.SetColor( 11 );
				printf( "0x%.4X\n", pProp->GetOffset() );
				g_pICvar->ConsoleColorPrintf( Color::Color( 0, 0, 0 ), "> %s: ", pProp->GetName() );
				g_pICvar->ConsoleColorPrintf( Color::Color( 255, 255, 255 ), "0x%.4X\n", pProp->GetOffset() );*/

				return pProp->GetOffset();
			}
			if( pProp->GetDataTable() )
			{
				RecvTable *pTable = pProp->GetDataTable();
				for( int i = 0; i < pTable->GetNumProps(); i++ )
				{
					RecvProp *pProp = pTable->GetProp( i );

					if( !pProp ) continue;

					if( !strcmp( pTable->GetName(), szClassName ) && !strcmp( pProp->GetName(), szVariable ) )
					{
						/*	g_pNetvarManager.SetColor( 3 );
						printf( "> %s: ", pProp->GetName() );
						g_pNetvarManager.SetColor( 11 );
						printf( "0x%.4X\n", pProp->GetOffset() );

						g_pICvar->ConsoleColorPrintf( Color::Color( 0, 0, 0 ), "> %s: ", pProp->GetName() );
						g_pICvar->ConsoleColorPrintf( Color::Color( 255, 255, 255 ), "0x%.4X\n", pProp->GetOffset() );*/
						return pProp->GetOffset();
					}
				}
			}
		}
	}
	return 0;
}

CNetVars *g_pNetVars = 0;

CNetVars::CNetVars( void )
{
	m_tables.clear();

	ClientClass *clientClass = g_pClient->GetAllClasses();
	if( !clientClass )
		return;

	while( clientClass )
	{
		RecvTable *recvTable = clientClass->GetTable();
		m_tables.push_back( recvTable );

		clientClass = clientClass->NextClass();
	}
}

DWORD CNetVars::GetOffset( const char *tableName, const char *propName )
{
	int offset = GetProp( tableName, propName );
	if( !offset )
	{
		return 0;
	}

	/*	g_pNetvarManager.SetColor( 3 );
	printf( "> %s: ", propName );
	g_pNetvarManager.SetColor( 11 );
	printf( "0x%02X\n", offset );
	g_pICvar->ConsoleColorPrintf( Color::Color( 0, 0, 0 ), "> %s: ", propName );
	g_pICvar->ConsoleColorPrintf( Color::Color( 255, 255, 255 ), "0x%02X\n", offset );*/

	return offset;
}

void* CNetVars::Hook( std::string table_to_hook, std::string prop_to_hook, NetvarHookCallback callback )
{
	auto client_class = g_pClient->GetAllClasses();

	// go through all classes
	while( client_class )
	{
		std::string table_name = client_class->GetTable()->m_pNetTableName;

		if( table_name == table_to_hook )
		{
			for( int i = 0; i < client_class->GetTable()->m_nProps; i++ )
			{
				auto& prop = client_class->GetTable()->m_pProps[ i ];
				std::string prop_name = prop.m_pVarName;

				if( prop_name == prop_to_hook )
				{
					auto original = prop.GetProxyFn();
					prop.SetProxyFn( callback );
					return original;
				}
			}
		}

		client_class = client_class->NextClass(); // go to the next class
	}
}

bool CNetVars::HookProp( const char *tableName, const char *propName, RecvVarProxyFn fun )
{
	RecvProp *recvProp = 0;
	GetProp( tableName, propName, &recvProp );
	if( !recvProp )
		return false;

	recvProp->SetProxyFn( fun );

	return true;
}

int CNetVars::GetProp( const char *tableName, const char *propName, RecvProp **prop )
{
	RecvTable *recvTable = GetTable( tableName );
	if( !recvTable )
		return 0;

	int offset = GetProp( recvTable, propName, prop );
	if( !offset )
		return 0;

	return offset;
}

int CNetVars::GetProp( RecvTable *recvTable, const char *propName, RecvProp **prop )
{
	int extraOffset = 0;
	for( int i = 0; i < recvTable->m_nProps; ++i )
	{
		RecvProp *recvProp = &recvTable->m_pProps [ i ];
		RecvTable *child = recvProp->GetDataTable();

		if( child && ( child->m_nProps > 0 ) )
		{
			int tmp = GetProp( child, propName, prop );
			if( tmp )
				extraOffset += ( recvProp->GetOffset() + tmp );
		}

		if( strcmp( recvProp->m_pVarName, propName ) )
			continue;

		if( prop )
			*prop = recvProp;

		return ( recvProp->GetOffset() + extraOffset );
	}

	return extraOffset;
}

RecvTable *CNetVars::GetTable( const char *tableName )
{
	if( m_tables.empty() )
		return 0;

	for each ( RecvTable *table in m_tables )
	{
		if( !table )
			continue;

		if( _stricmp( table->m_pNetTableName, tableName ) == 0 )
			return table;
	}

	return 0;
}

void CNetVars::DumpTable( RecvTable *table )
{
	for( int i = 0; i < table->m_nProps; ++i )
	{
		RecvProp *prop = &table->m_pProps [ i ];
		if( !strcmp( prop->GetName(), "baseclass" ) )
		{
			continue;
		}

		if( strstr( prop->GetName(), "m_" ) )
		{
			if( prop->GetDataTable() )
			{
				DumpTable( prop->GetDataTable() );
			}
		}
	}
}

namespace Offsets
{
	void Init( void )
	{
		Player::m_iHealth = g_pNetvarManager.GetNetvarOld( "DT_BasePlayer", "m_iHealth" );
		Player::m_ArmorValue = g_pNetvarManager.GetNetvarOld( "DT_CSPlayer", "m_ArmorValue" );
		Player::m_iTeamNum = g_pNetvarManager.GetNetvarOld( "DT_BaseEntity", "m_iTeamNum" );
		Player::m_iAccount = g_pNetvarManager.GetNetvarOld( "DT_CSPlayer", "m_iAccount" );
		Player::m_bSpotted = g_pNetvarManager.GetNetvarOld( "DT_BaseEntity", "m_bSpotted" );
		Player::m_hOwner = g_pNetvarManager.GetNetvarOld( "DT_BaseEntity", "m_hOwnerEntity" );
		Player::m_bHasDefuser = g_pNetvarManager.GetNetvarOld( "DT_CSPlayer", "m_bHasDefuser" );
		Player::m_bIsDefusing = g_pNetvarManager.GetNetvarOld( "DT_CSPlayer", "m_bIsDefusing" );
		Player::m_iShotsFired = g_pNetvarManager.GetNetvarOld( "DT_CSLocalPlayerExclusive", "m_iShotsFired" );
		Player::m_fFlags = g_pNetvarManager.GetNetvarOld( "DT_BasePlayer", "m_fFlags" );
		Player::m_lifeState = g_pNetvarManager.GetNetvarOld( "DT_BasePlayer", "m_lifeState" );
		Player::m_hObserverTarget = g_pNetvarManager.GetNetvarOld( "DT_BasePlayer", "m_hObserverTarget" );
		Player::m_Local = g_pNetvarManager.GetNetvarOld( "DT_LocalPlayerExclusive", "m_Local" );
		Player::m_vecEyeAngles = g_pNetvarManager.GetNetvarOld( "DT_CSPlayer", "m_angEyeAngles[0]" );
		Player::m_bGunGameImmunity = g_pNetvarManager.GetNetvarOld( "DT_CSPlayer", "m_bGunGameImmunity" );
		Player::m_aimPunchAngle = 0x70;
		Player::m_aimPunchAngleVel = 0x7C;
		Player::m_ViewPunchAngle = 0x64;
		Player::m_BombTimer = g_pNetvarManager.GetNetvarOld( "DT_BaseCombatWeapon", "m_flModelScale" );
		Player::m_vecOrigin = g_pNetvarManager.GetNetvarOld( "DT_CSLocalPlayerExclusive", "m_vecOrigin" );
		Player::m_vecViewOffset = g_pNetvarManager.GetNetvarOld( "DT_LocalPlayerExclusive", "m_vecViewOffset[0]" );
		Player::m_nTickBase = g_pNetvarManager.GetNetvarOld( "DT_LocalPlayerExclusive", "m_nTickBase" );
		Player::m_hActiveWeapon = g_pNetvarManager.GetNetvarOld( "DT_BaseCombatCharacter", "m_hActiveWeapon" );
		Player::m_MyWeapons = g_pNetVars->GetOffset( "DT_BaseCombatCharacter", "m_MyWeapons" );
		Player::m_flFlashDuration = g_pNetvarManager.GetNetvarOld( "DT_CSPlayer", "m_flFlashDuration" );
		Player::m_flFlashMaxAlpha = g_pNetvarManager.GetNetvarOld( "DT_CSPlayer", "m_flFlashMaxAlpha" );
		Player::m_vecVelocity = g_pNetvarManager.GetNetvarOld( "DT_LocalPlayerExclusive", "m_vecVelocity[0]" );
		Player::m_bIsScoped = g_pNetvarManager.GetNetvarOld( "DT_CSPlayer", "m_bIsScoped" );
		Player::m_angEyeAngles = g_pNetvarManager.GetNetvarOld( "DT_CSPlayer", "m_angEyeAngles[0]" );
		Player::m_angEyeAnglesY = g_pNetvarManager.GetNetvarOld( "DT_CSPlayer", "m_angEyeAngles[1]" );
		Player::m_angRotation = g_pNetVars->GetOffset( "CCSPlayer", "m_angRotation" );
		Player::m_iCompetitiveRanking = g_pNetvarManager.GetNetvarOld( "DT_CSPlayerResource", "m_iCompetitiveRanking" );
		Player::m_dwWeaponTableIndex = g_pNetvarManager.GetNetvarOld( "WeaponTableIndex", "m_dw_WeaponTableIndex" );
		Player::m_iCompetitiveWins = g_pNetvarManager.GetNetvarOld( "DT_CSPlayerResource", "m_iCompetitiveWins" );
		Player::m_iPlayerC4 = g_pNetvarManager.GetNetvarOld( "DT_CSPlayerResource", "m_iPlayerC4" );
		Player::m_iKills = g_pNetvarManager.GetNetvarOld( "DT_CSPlayerResource", "m_iScore" );
		Player::m_szLastPlaceName = g_pNetvarManager.GetNetvarOld( "DT_BasePlayer", "m_szLastPlaceName" );
		Player::m_bHasHelmet = g_pNetvarManager.GetNetvarOld( "DT_CSPlayer", "m_bHasHelmet" );
		Player::m_nHitboxSet = g_pNetvarManager.GetNetvarOld( "DT_BaseAnimating", "m_nHitboxSet" );
		Player::m_totalHitsOnServer = g_pNetvarManager.GetNetvarOld( "DT_CSPlayer", "m_totalHitsOnServer" );
		Player::m_hConstraintEntity = g_pNetvarManager.GetNetvarOld( "DT_LocalPlayerExclusive", "m_hConstraintEntity" );
		Player::m_ZoomLevel = g_pNetVars->GetOffset( "DT_WeaponCSBaseGun", "m_zoomLevel" );
		Player::m_pCurrentCommand = Player::m_hConstraintEntity - 0xC;
		Player::m_pPlayerCommand = Player::m_hConstraintEntity - 0xB4;
		Player::deathflag = g_pNetVars->GetOffset( "DT_BasePlayer", "deadflag" ) + 0x4;
		Player::m_flMaxSpeed = g_pNetVars->GetOffset( "DT_CSPlayer", "m_flMaxspeed" );
		Player::m_flFlashBangTime = g_pNetVars->GetOffset( "DT_CSPlayer", "m_flFlashBangTime" );
		Player::m_vecBaseVelocity = g_pNetVars->GetOffset( "DT_BasePlayer", "m_vecBaseVelocity" );
		Player::m_nSequence = g_pNetVars->GetOffset( "DT_BasePlayer", "m_nSequence" );
		Player::m_flCycle = g_pNetVars->GetOffset( "DT_BasePlayer", "m_flCycle" );
		Player::m_flPoseParameter = g_pNetVars->GetOffset( "DT_BasePlayer", "m_flPoseParameter" );
		Player::m_nForceBone = g_pNetVars->GetOffset( "DT_BasePlayer", "m_nForceBone" );
		Player::m_flFallVelocity = g_pNetVars->GetOffset( "DT_BasePlayer", "m_flFallVelocity" );
		Player::m_vecBaseVelocity = g_pNetVars->GetOffset( "DT_BasePlayer", "vecBaseVelocity" );
		Player::m_flStepSize = g_pNetVars->GetOffset( "DT_BasePlayer", "m_flStepSize" );
		Player::m_flMaxspeed = g_pNetVars->GetOffset( "DT_BasePlayer", "m_flMaxspeed" );
		Player::m_bClientSideAnimation = g_pNetVars->GetOffset( "DT_BasePlayer", "m_bClientSideAnimation" );
		Player::m_bDucking = g_pNetVars->GetOffset( "DT_CSPlayer", "m_bDucking" );
		Player::m_flDuckAmount = g_pNetVars->GetOffset("DT_CSPlayer", "m_flDuckAmount");
		Player::m_bIsPlayerGhost = g_pNetVars->GetOffset( "DT_CSPlayer", "m_bIsPlayerGhost" );
		Player::m_hViewModel = g_pNetVars->GetOffset( "CBasePlayer", "m_hViewModel[0]" );
		Player::m_hWeapon = g_pNetVars->GetOffset( "CBaseViewModel", "m_hWeapon" );
		Player::m_fEffects = g_pNetVars->GetOffset( "CBaseViewModel", "m_fEffects" );
		Player::m_hWeaponWorldModel = g_pNetVars->GetOffset( "DT_BaseCombatWeapon", "m_hWeaponWorldModel" );

		Weapon::m_flNextPrimaryAttack = g_pNetvarManager.GetNetvarOld( "DT_LocalActiveWeaponData", "m_flNextPrimaryAttack" );
		Weapon::m_iViewModelIndex = g_pNetvarManager.GetNetvarOld( "DT_LocalWeaponData", "m_nViewModelIndex" );
		Weapon::m_nModelIndex = g_pNetVars->GetOffset( "DT_BaseCombatWeapon", "m_nModelIndex" );
		Weapon::m_hWeaponWorldModel = g_pNetVars->GetOffset( "DT_BaseCombatWeapon", "m_hWeaponWorldModel" );
		Weapon::m_iClip1 = g_pNetvarManager.GetNetvarOld( "DT_BaseCombatWeapon", "m_iClip1" );
		Weapon::m_iState = g_pNetvarManager.GetNetvarOld( "DT_BaseCombatWeapon", "m_iState" );
		Weapon::m_bIsBroken = g_pNetvarManager.GetNetvarOld( "DT_FuncConveyor", "m_bIsBroken" );
		Weapon::m_flNextAttack = g_pNetVars->GetOffset( "DT_BaseCombatCharacter", "m_flNextAttack" );
		Weapon::m_fThrowTime = g_pNetVars->GetOffset( "DT_HEGrenade", "m_fThrowTime" );
		Weapon::m_bPinPulled = g_pNetVars->GetOffset( "DT_HEGrenade", "m_bPinPulled" );
		Weapon::m_flRecoilIndex = g_pNetVars->GetOffset( "DT_BaseCSGrenade", "m_flRecoilIndex" );
		Weapon::m_flPostponeFireReadyTime = g_pNetVars->GetOffset( "DT_BaseCombatWeapon", "m_flPostponeFireReadyTime" );
		Weapon::m_iPrimaryReserveAmmoCount = g_pNetVars->GetOffset( "DT_BaseCombatWeapon", "m_iPrimaryReserveAmmoCount" );
		Weapon::m_iSecondaryReserveAmmoCount = g_pNetVars->GetOffset( "DT_BaseCombatWeapon", "m_iSecondaryReserveAmmoCount" );

		Entity::m_CollisionGroup = g_pNetvarManager.GetNetvarOld( "DT_BaseEntity", "m_CollisionGroup" );
		Entity::m_Collision = g_pNetvarManager.GetNetvarOld( "DT_BaseEntity", "m_Collision" );
		Entity::m_vecMins = g_pNetvarManager.GetNetvarOld( "DT_CollisionProperty", "m_vecMins" );
		Entity::m_vecMaxs = g_pNetvarManager.GetNetvarOld( "DT_CollisionProperty", "m_vecMaxs" );
		Entity::m_rgflCoordinateFrame = Entity::m_CollisionGroup - 0x30;

		Attribute::m_AttributeManager = g_pNetVars->GetOffset( "DT_BaseCombatWeapon", "m_AttributeManager" );
		Attribute::m_Item = g_pNetVars->GetOffset( "DT_BaseCombatWeapon", "m_Item" );
		Attribute::m_iItemDefinitionIndex = g_pNetVars->GetOffset( "DT_BaseCombatWeapon", "m_iItemDefinitionIndex" );

		Attribute::m_OriginalOwnerXuidLow = g_pNetVars->GetOffset( "DT_BaseCombatWeapon", "m_OriginalOwnerXuidLow" );
		Attribute::m_OriginalOwnerXuidHigh = g_pNetVars->GetOffset( "DT_BaseCombatWeapon", "m_OriginalOwnerXuidHigh" );
		Attribute::m_nFallbackSeed = g_pNetVars->GetOffset( "DT_BaseCombatWeapon", "m_nFallbackSeed" );
		Attribute::m_nFallbackPaintKit = g_pNetVars->GetOffset( "DT_BaseCombatWeapon", "m_nFallbackPaintKit" );
		Attribute::m_flFallbackWear = g_pNetVars->GetOffset( "DT_BaseCombatWeapon", "m_flFallbackWear" );
		Attribute::m_iItemIDHigh = g_pNetVars->GetOffset( "DT_BaseCombatWeapon", "m_iItemIDHigh" );
		Attribute::m_nFallbackStatTrak = g_pNetVars->GetOffset( "DT_BaseCombatWeapon", "m_nFallbackStatTrak" );
		Attribute::m_iEntityQuality = g_pNetVars->GetOffset( "DT_BaseCombatWeapon", "m_iEntityQuality" );
		Attribute::m_szCustomName = g_pNetVars->GetOffset( "DT_BaseCombatWeapon", "m_szCustomName" );
		Attribute::m_iAccountID = g_pNetVars->GetOffset( "DT_BaseCombatWeapon", "m_iAccountID" );
		Attribute::m_iViewModelIndex = g_pNetVars->GetOffset( "DT_BaseCombatWeapon", "m_iViewModelIndex" );
		Attribute::m_iWorldModelIndex = g_pNetVars->GetOffset( "DT_BaseCombatWeapon", "m_iWorldModelIndex" );
		Attribute::m_iWorldDroppedModelIndex = g_pNetVars->GetOffset( "DT_BaseCombatWeapon", "m_iWorldDroppedModelIndex" );

		Player::m_flSimulationTime = g_pNetVars->GetOffset( "DT_CSPlayer", "m_flSimulationTime" );
		Player::m_iObserverMode = g_pNetVars->GetOffset( "DT_CSPlayer", "m_iObserverMode" );
		Player::m_flDefuseLength = g_pNetVars->GetOffset( "DT_PlantedC4", "m_flDefuseCountDown" );
		Player::m_flC4Blow = g_pNetVars->GetOffset( "DT_PlantedC4", "m_flC4Blow" );
		Player::m_flLowerBodyYawTarget = g_pNetVars->GetOffset("DT_CSPlayer", "m_flLowerBodyYawTarget");
		Player::m_hMyWeapons = g_pNetVars->GetOffset("DT_BaseCombatCharacter", "m_hMyWeapons");

		Entity::m_fEffects = g_pNetVars->GetOffset("DT_BaseEntity", "m_fEffects");
		//printf("m_fEffects => 0x%p\n", Entity::m_fEffects);

		//Weapon::m_HandlebulletPenetration = g_Pattern.PatternSearch( "client.dll", ( PBYTE )"\x55\x8B\xEC\x83\xEC\x70\x8B\x55\x00\x56\x8B\x75\x00\x89\x4D\x00\x32\xC9\x8B\x12\x57\x8A\x46", "xxxxxxxx?xxx?xx?xxxxxxx", NULL, NULL );
		//Player::dwGlowObjectManager = *( DWORD* )( g_Pattern.PatternSearchIDA( "client.dll", "F3 ?? ?? ?? ?? ?? ?? ?? 83 C8 01 C7 05" ) + 0x4 );
		//Player::dwSmokeCount = g_Pattern.PatternSearch( "client.dll", ( PBYTE )"\xA3\x00\x00\x00\x00\x57\x8B\xCB", "x????xxx", NULL, NULL ) + 0x1;
		Player::m_dwSetupBonesRTADD = g_Pattern.PatternSearchIDA( g_Offset.szClient.c_str(), "55 8B EC 57 8B F9 8B 0D ?? ?? ?? ?? 8B 01 8B 80" ) + 0x16;
	}

	namespace Attribute
	{
		DWORD m_AttributeManager = NULL;
		DWORD m_Item = NULL;
		DWORD m_iItemDefinitionIndex = NULL;
		DWORD m_iItemIDHigh = NULL;
		DWORD m_iItemIDLow = NULL;
		DWORD m_OriginalOwnerXuidLow = NULL;
		DWORD m_OriginalOwnerXuidHigh = NULL;
		DWORD m_nFallbackPaintKit = NULL;
		DWORD m_nFallbackSeed = NULL;
		DWORD m_flFallbackWear = NULL;
		DWORD m_nFallbackStatTrak = NULL;
		DWORD m_iEntityQuality = NULL;
		DWORD m_szCustomName = NULL;
		DWORD m_iAccountID = NULL;
		DWORD m_iViewModelIndex = NULL;
		DWORD m_iWorldModelIndex = NULL;
		DWORD m_iWorldDroppedModelIndex = NULL;
	}

	namespace Player
	{
		DWORD   m_angRotation = NULL;
		DWORD   m_hConstraintEntity = NULL;
		DWORD	m_iHealth = NULL;
		DWORD	m_ArmorValue = NULL;
		DWORD	m_iTeamNum = NULL;
		DWORD   m_fEffects = NULL;
		DWORD	m_fFlags = NULL;
		DWORD   m_flDuckAmount = NULL;
		DWORD	m_iKills = NULL;
		DWORD   m_bHasDefuser = NULL;
		DWORD	m_bIsDefusing = NULL;
		DWORD	m_lifeState = NULL;
		DWORD   m_hOwner = NULL;
		DWORD   m_bIsPlayerGhost = NULL;
		DWORD   m_bIsScoped = NULL;
		DWORD	m_flFlashDuration = NULL;
		DWORD   m_flFlashBangTime = NULL;
		DWORD   m_dwLoadFromBuffer = NULL;
		DWORD   m_iAccount = NULL;
		DWORD   m_szLastPlaceName = NULL;
		DWORD   m_hObserverTarget = NULL;
		DWORD   m_flSimulationTime = NULL;
		DWORD   m_ZoomLevel = NULL;
		DWORD   m_pCurrentCommand = NULL;
		DWORD   m_pPlayerCommand = NULL;
		DWORD   dwGlowObjectManager = NULL;
		DWORD   dwSmokeCount = NULL;
		DWORD   m_flLowerBodyYawTarget = NULL;
		DWORD   m_hMyWeapons = NULL;
		DWORD   m_flMaxSpeed = NULL;
		DWORD   m_dwSetupBonesRTADD = NULL;
		DWORD   m_vecBaseVelocity = NULL;
		DWORD   m_flCycle = NULL;
		DWORD   m_flPoseParameter = NULL;
		DWORD   m_nSequence = NULL;
		DWORD   m_nForceBone = NULL;
		DWORD   m_bDucking = NULL;

		DWORD	m_flDefuseLength = NULL;
		DWORD   m_iCompetitiveWins = NULL;
		DWORD   m_iPlayerC4 = NULL;
		DWORD   m_dwWeaponTableIndex = NULL;
		DWORD   m_bSpotted = NULL;
		DWORD   m_iCompetitiveRanking = NULL;
		DWORD   m_BombTimer = NULL;
		DWORD	m_flFlashMaxAlpha = NULL;
		DWORD	m_Local = NULL;
		DWORD   m_flC4Blow = NULL;
		DWORD	m_bGunGameImmunity = NULL;
		DWORD	m_vecPunchAngle = NULL;
		DWORD	m_aimPunchAngle = NULL;
		DWORD   m_aimPunchAngleVel = NULL;
		DWORD   m_ViewPunchAngle = NULL;
		DWORD	m_vecOrigin = NULL;
		DWORD	m_vecViewOffset = NULL;
		DWORD	m_nTickBase = NULL;
		DWORD   m_totalHitsOnServer = NULL;

		DWORD	m_hActiveWeapon = NULL;
		DWORD	m_MyWeapons = NULL;
		DWORD	m_vecVelocity = NULL;
		DWORD	m_vecEyeAngles = NULL;
		DWORD	m_angEyeAngles = NULL;
		DWORD   m_angEyeAnglesY = NULL;
		DWORD	m_bHasHelmet = NULL;
		DWORD	m_nHitboxSet = NULL;
		DWORD	m_clrRender = NULL;
		DWORD	m_iShotsFired = NULL;
		DWORD   m_iObserverMode = NULL;
		DWORD   deathflag = NULL;
		DWORD   m_flFallVelocity = NULL;
		DWORD   vecBaseVelocity = NULL;
		DWORD   m_flStepSize = NULL;
		DWORD   m_flMaxspeed = NULL;
		DWORD   m_bClientSideAnimation = NULL;
		DWORD   m_hViewModel = NULL;
		DWORD   m_hWeapon = NULL;
		DWORD   m_hWeaponWorldModel = NULL;
	}

	namespace Weapon
	{
		DWORD	m_iClip1 = NULL;
		DWORD	m_flNextPrimaryAttack = NULL;
		DWORD   m_flPostponeFireReadyTime = NULL;
		DWORD	m_iState = NULL;
		DWORD   m_iViewModelIndex = NULL;
		DWORD   m_nModelIndex = NULL;
		DWORD   m_bIsBroken = NULL;
		DWORD   m_flNextAttack = NULL;
		DWORD   m_bPinPulled = NULL;
		DWORD   m_fThrowTime = NULL;
		DWORD   m_flRecoilIndex = NULL;
		DWORD   m_HandlebulletPenetration = NULL;
		DWORD   m_iPrimaryReserveAmmoCount = NULL;
		DWORD   m_iSecondaryReserveAmmoCount = NULL;
		DWORD   m_hWeaponWorldModel = NULL;
	}

	namespace Entity
	{
		DWORD m_CollisionGroup = NULL;
		DWORD m_Collision = NULL;
		DWORD m_vecMins = NULL;
		DWORD m_vecMaxs = NULL;
		DWORD m_rgflCoordinateFrame = NULL;
		DWORD m_fEffects = NULL;
	}
}