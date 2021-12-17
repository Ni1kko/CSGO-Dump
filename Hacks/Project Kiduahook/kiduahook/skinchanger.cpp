#include "skinchanger.h"

CSkinChanger g_SkinChanger;

CBaseCombatWeapon* pWorldModel;
int iPassedIndex;
void ViewModelProxy( const CRecvProxyData *pData, void *pStruct, void *pOut )
{
	if( *( int* ) ( ( DWORD ) pData + 0x8 ) == g_pModel->GetModelIndex( "models/weapons/v_knife_default_t.mdl" )
		|| *( int* ) ( ( DWORD ) pData + 0x8 ) == g_pModel->GetModelIndex( "models/weapons/v_knife_default_ct.mdl" )
		|| *( int* ) ( ( DWORD ) pData + 0x8 ) == g_pModel->GetModelIndex( "models/weapons/v_knife_bayonet.mdl" )
		|| *( int* ) ( ( DWORD ) pData + 0x8 ) == g_pModel->GetModelIndex( "models/weapons/v_knife_flip.mdl" )
		|| *( int* ) ( ( DWORD ) pData + 0x8 ) == g_pModel->GetModelIndex( "models/weapons/v_knife_gut.mdl" )
		|| *( int* ) ( ( DWORD ) pData + 0x8 ) == g_pModel->GetModelIndex( "models/weapons/v_knife_karam.mdl" )
		|| *( int* ) ( ( DWORD ) pData + 0x8 ) == g_pModel->GetModelIndex( "models/weapons/v_knife_m9_bay.mdl" )
		|| *( int* ) ( ( DWORD ) pData + 0x8 ) == g_pModel->GetModelIndex( "models/weapons/v_knife_tactical.mdl" )
		|| *( int* ) ( ( DWORD ) pData + 0x8 ) == g_pModel->GetModelIndex( "models/weapons/v_knife_butterfly.mdl" )
		|| *( int* ) ( ( DWORD ) pData + 0x8 ) == g_pModel->GetModelIndex( "models/weapons/v_knife_falchion_advanced.mdl" )
		|| *( int* ) ( ( DWORD ) pData + 0x8 ) == g_pModel->GetModelIndex( "models/weapons/v_knife_push.mdl" )
		|| *( int* ) ( ( DWORD ) pData + 0x8 ) == g_pModel->GetModelIndex( "models/weapons/v_knife_survival_bowie.mdl" ) )
	{
		*( int* ) ( ( DWORD ) pData + 0x8 ) = iPassedIndex;
	}
	*( int* ) ( ( DWORD ) pOut ) = *( int* ) ( ( DWORD ) pData + 0x8 );
}


void CSkinChanger::ChangePattern( CBaseEntity* pLocal )
{
	UINT* hWeapons = ( UINT* ) ( ( DWORD ) pLocal + 0x2DF8);

	static int iItemIndex = pLocal->GetTeamNumber() == 2 ? 42 : 59;
	static int iModelIndex = 63;

	if( !hWeapons )
		return;

	for( int i = 0; hWeapons [ i ]; i++ )  // for( int i = 0; i < g_pEntList->GetHighestEntityIndex(); i++ ) nice prank soll noch mehr fps ziehen or wattttttttttt 
	{
		CBaseCombatWeapon* pWeaponHandle = ( CBaseCombatWeapon* ) g_pEntList->GetClientEntityFromHandle( hWeapons [ i ] );

		if( !pWeaponHandle || pWeaponHandle == nullptr )
			continue;

		if( pWeaponHandle->IsOther() )
			continue;

		ClientClass* pClass = ( ( CBaseEntity* ) pWeaponHandle )->GetClientClass();

		if( !pClass || pClass == nullptr )
			continue;

		if( g_pEntList->GetClientEntityFromHandle( *( PULONG ) ( ( DWORD ) pWeaponHandle + Offsets::Player::m_hOwner ) ) != pLocal )
			continue;

		HANDLE hWorldWeaponHandle = pWeaponHandle->GetWeaponWorldModel();

		if( hWorldWeaponHandle )
			pWorldModel = ( CBaseCombatWeapon* ) g_pEntList->GetClientEntityFromHandle( hWorldWeaponHandle );
		//150 253 seed m9 casehardened

		if( pClass->GetClassID() == CKnife )
		{
			switch( g_Skins.Knife )
			{
			case 0:
				iItemIndex = pLocal->GetTeamNumber() == 2 ? 42 : 59;
				iModelIndex = pLocal->GetTeamNumber() == 2 ? g_pModel->GetModelIndex( "models/weapons/v_knife_default_t.mdl" ) : g_pModel->GetModelIndex( "models/weapons/v_knife_default_ct.mdl" );
				break;
			case 1:
				iItemIndex = 500;
				iModelIndex = g_pModel->GetModelIndex( "models/weapons/v_knife_bayonet.mdl" );
				break;
			case 2:
				iItemIndex = 505;
				iModelIndex = g_pModel->GetModelIndex( "models/weapons/v_knife_flip.mdl" );
				break;
			case 3:
				iItemIndex = 506;
				iModelIndex = g_pModel->GetModelIndex( "models/weapons/v_knife_gut.mdl" );
				break;
			case 4:
				iItemIndex = 507;
				iModelIndex = g_pModel->GetModelIndex( "models/weapons/v_knife_karam.mdl" );
				break;
			case 5:
				iItemIndex = 508;
				iModelIndex = g_pModel->GetModelIndex( "models/weapons/v_knife_m9_bay.mdl" );
				break;
			case 6:
				iItemIndex = 509;
				iModelIndex = g_pModel->GetModelIndex( "models/weapons/v_knife_tactical.mdl" );
				break;
			case 7:
				iItemIndex = 515;
				iModelIndex = g_pModel->GetModelIndex( "models/weapons/v_knife_butterfly.mdl" );
				break;
			case 8:
				iItemIndex = 512;
				iModelIndex = g_pModel->GetModelIndex( "models/weapons/v_knife_falchion_advanced.mdl" );
				break;
			case 9:
				iItemIndex = 516;
				iModelIndex = g_pModel->GetModelIndex( "models/weapons/v_knife_push.mdl" );
				break;
			case 10:
				iItemIndex = 514;
				iModelIndex = g_pModel->GetModelIndex( "models/weapons/v_knife_survival_bowie.mdl" );
				break;
			}

			iPassedIndex = iModelIndex;
		    *( int* ) ( ( DWORD ) pWeaponHandle + Offsets::Weapon::m_nModelIndex ) = iModelIndex;

			if( pWorldModel && pWorldModel != nullptr )//needed otherwise console holocaust
				*( int* ) ( ( DWORD ) pWorldModel + Offsets::Weapon::m_nModelIndex ) = iModelIndex + 1;

			*( int* ) ( ( DWORD ) pWeaponHandle + Offsets::Attribute::m_iItemDefinitionIndex ) = iItemIndex;


			bool bDone = false;

			while( pClass && !bDone )
			{
				const char *pszName = pClass->GetTable()->GetName();

				if( !strcmp( pszName, "DT_BaseViewModel" ) )
				{
					for( int i = 0; i < pClass->GetTable()->GetNumProps(); i++ )
					{
						RecvProp *pProp = pClass->GetTable()->GetPropA( i );

						if( !strcmp( pProp->GetName(), "m_nModelIndex" ) )
						{
							pProp->SetProxyFn( ViewModelProxy );

							bDone = true;
						}
					}
				}
				pClass = pClass->NextClass();
			}
		}

		if( g_Skins.Enable[ pWeaponHandle->GetWeaponID() ] )
		{
			int iSkin = g_Skins.PaintKit [ pWeaponHandle->GetWeaponID() ];
			float flCondition = 100.f - ( g_Skins.Condition[ pWeaponHandle->GetWeaponID() ] / 100.f ) - 99.f;//0 - 1
			int iSeed = 253;
			int iStattrak = 0;

			pWeaponHandle->SetPattern( pLocal, iSkin, 0.001f, iSeed, iStattrak, "" );
		}
	}
}

void* CL_CreateDLLEntity(int iEnt, int iClass, int iSerialNum)
{
	CClientClass* pClient = g_pClient->AllClasses();

	if (!pClient)
		return false;

	while (pClient)
	{
		if (pClient->m_ClassID == iClass)
			return pClient->m_pCreateFn(iEnt, iSerialNum);
		pClient = pClient->m_pNext;
	}
	return false;
}

void CSkinChanger::ChangeGloves( CBaseEntity* pLocal )
{
	if( g_Skins.Glove == 0 )
		return;

	static bool bNeedUpdate;
	static int iSkin;
	static int iGlove;
	static std::string szModel;
	static float flLastSpawn;
	float flSpawn = pLocal->GetSpawnTime();

	if( flLastSpawn != flSpawn || *( PINT ) ( ( DWORD ) g_pClientState + 0x174 ) == -1 )
	{
		flLastSpawn = flSpawn;

		int* pWearAble = pLocal->GetWearables();

		if (pWearAble == nullptr)
			return;

		static CBaseHandle hGlove = 0;

		CBaseCombatWeapon* pGlove = (CBaseCombatWeapon*)g_pEntList->GetClientEntityFromHandle(pWearAble[0]);

		if (!pGlove)
		{
			CBaseCombatWeapon* pLocalGlove = (CBaseCombatWeapon*)g_pEntList->GetClientEntityFromHandle(hGlove);

			if (pLocalGlove)
			{
				pWearAble[0] = hGlove;
				pGlove = pLocalGlove;
			}
		}

		if (pLocal->GetHealth() <= 0)
		{
			if (pGlove)
			{
				pGlove->SetDestroyedOnRecreateEntities();
				pGlove->Release();
			}
			return;
		}

		if (!pGlove)
		{
			int iEnt = g_pEntList->GetHighestEntityIndex() + 1;
			int iSerialNumber = g_Math.RandomFloat(0x0, 0xFFF);

			CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)CL_CreateDLLEntity(iEnt, CEconWearable, iSerialNumber);

			if (pWeapon)
			{
				int iIndex = iEnt | (iSerialNumber << 16);
				hGlove = iIndex;
				*(DWORD*)((DWORD)pLocal + g_pNetVars->GetOffset("DT_BaseCombatCharacter", "m_hMyWearables")) = iIndex;
				pGlove = (CBaseCombatWeapon*)g_pEntList->GetClientEntity(iEnt);
			}
		}

		*reinterpret_cast<int*>(uintptr_t(pGlove) + 0x64) = -1;

		if (pGlove)
		{
			VEngineClient::player_info_t LocalPlayerInfo;

			if (!g_pEngine->GetPlayerInfo(pLocal->GetIndex(), &LocalPlayerInfo))
				return;

			*(int*)((DWORD)pGlove + Offsets::Attribute::m_iItemIDHigh) = -1;
			*(int*)((DWORD)pGlove + Offsets::Attribute::m_iEntityQuality) = 4;
			*(int*)((DWORD)pGlove + Offsets::Attribute::m_iAccountID) = LocalPlayerInfo.xuid;
			*(float*)((DWORD)pGlove + Offsets::Attribute::m_flFallbackWear) = 0.00000001f;

			iSkin = g_Skins.GlovePaint;

			switch (g_Skins.Glove)
			{
			case 1:
				iGlove = 5027;
				szModel = "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl";
				break;
			case 2:
				iGlove = 5030;
				szModel = "models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl";
				break;
			case 3:
				iGlove = 5031;
				szModel = "models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl";
				break;
			case 4:
				iGlove = 5032;
				szModel = "models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl";
				break;
			case 5:
				iGlove = 5033;
				szModel = "models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl";
				break;
			case 6:
				iGlove = 5034;
				szModel = "models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl";
				break;
			case 7:
				iGlove = 5035;
				szModel = "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl";
				break;
			}

			*(int*)((DWORD)pGlove + Offsets::Attribute::m_iItemDefinitionIndex) = iGlove;
			*(int*)((DWORD)pGlove + Offsets::Attribute::m_nFallbackPaintKit) = iSkin;

			if (g_pModel == nullptr)
				return;

			pGlove->SetGloveModelIndex(g_pModel->GetModelIndex(szModel.c_str()));

			pGlove->PreDataUpdate(0);
		}


	/*	DWORD* hMyWearables = ( DWORD* ) ( ( size_t ) pLocal + 0x2F04);

		if( hMyWearables == nullptr )
			return;

		if( !g_pEntList->GetClientEntity( hMyWearables[ 0 ] & 0xFFF ) )
		{
			for( CClientClass* pClass = g_pClient->AllClasses(); pClass; pClass = pClass->m_pNext )
			{
				if( pClass == nullptr )
					continue;

				if( pClass->m_ClassID != CEconWearable )
					continue;

				int iEntry = ( g_pEntList->GetHighestEntityIndex() + 1 );
				int	iSerial = g_Math.RandomFloat( 0x0, 0xFFF );

				pClass->m_pCreateFn( iEntry, iSerial );
				hMyWearables [ 0 ] = iEntry | ( iSerial << 16 );
				
				break;
			}
		}

		CBaseCombatWeapon* pWeapon = ( CBaseCombatWeapon* ) g_pEntList->GetClientEntity( hMyWearables[ 0 ] & 0xFFF );

		if( !pWeapon || pWeapon == nullptr )
			return;

		VEngineClient::player_info_t LocalPlayerInfo;

		if( !g_pEngine->GetPlayerInfo( pLocal->GetIndex(), &LocalPlayerInfo ) )
			return;

		*( int* ) ( ( DWORD ) pWeapon + Offsets::Attribute::m_iItemIDHigh ) = -1;
		*( int* ) ( ( DWORD ) pWeapon + Offsets::Attribute::m_iEntityQuality ) = 4;
		*( int* ) ( ( DWORD ) pWeapon + Offsets::Attribute::m_iAccountID ) = LocalPlayerInfo.xuid;
		*( float* ) ( ( DWORD ) pWeapon + Offsets::Attribute::m_flFallbackWear ) = 0.00000001f;

		iSkin = g_Skins.GlovePaint;

		switch( g_Skins.Glove )
		{
		case 1:
			iGlove = 5027;
			szModel = "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl";
			break;
		case 2:
			iGlove = 5030;
			szModel = "models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl";
			break;
		case 3:
			iGlove = 5031;
			szModel = "models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl";
			break;
		case 4:
			iGlove = 5032;
			szModel = "models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl";
			break;
		case 5:
			iGlove = 5033;
			szModel = "models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl";
			break;
		case 6:
			iGlove = 5034;
			szModel = "models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl";
			break;
		case 7:
			iGlove = 5035;
			szModel = "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl";
			break;
		}

		*( int* ) ( ( DWORD ) pWeapon + Offsets::Attribute::m_iItemDefinitionIndex ) = iGlove;
		*( int* ) ( ( DWORD ) pWeapon + Offsets::Attribute::m_nFallbackPaintKit ) = iSkin;
		
		if( g_pModel == nullptr )
			return;

		pWeapon->SetGloveModelIndex( g_pModel->GetModelIndex( szModel.c_str() ) );

		pWeapon->PreDataUpdate( 0 );*/
	}
}