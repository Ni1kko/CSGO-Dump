#include "KnifeAnimFix.h"

typedef void(*RecvVarProxyFn)(const CRecvProxyData* pData, void* pStruct, void* pOut);

#define SEQUENCE_DEFAULT_DRAW 0
#define SEQUENCE_DEFAULT_IDLE1 1
#define SEQUENCE_DEFAULT_IDLE2 2
#define SEQUENCE_DEFAULT_LIGHT_MISS1 3
#define SEQUENCE_DEFAULT_LIGHT_MISS2 4
#define SEQUENCE_DEFAULT_HEAVY_MISS1 9
#define SEQUENCE_DEFAULT_HEAVY_HIT1 10
#define SEQUENCE_DEFAULT_HEAVY_BACKSTAB 11
#define SEQUENCE_DEFAULT_LOOKAT01 12

#define SEQUENCE_BUTTERFLY_DRAW 0
#define SEQUENCE_BUTTERFLY_DRAW2 1
#define SEQUENCE_BUTTERFLY_LOOKAT01 13
#define SEQUENCE_BUTTERFLY_LOOKAT03 15

#define SEQUENCE_FALCHION_IDLE1 1
#define SEQUENCE_FALCHION_HEAVY_MISS1 8
#define SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP 9
#define SEQUENCE_FALCHION_LOOKAT01 12
#define SEQUENCE_FALCHION_LOOKAT02 13

#define SEQUENCE_DAGGERS_IDLE1 1
#define SEQUENCE_DAGGERS_LIGHT_MISS1 2
#define SEQUENCE_DAGGERS_LIGHT_MISS5 6
#define SEQUENCE_DAGGERS_HEAVY_MISS2 11
#define SEQUENCE_DAGGERS_HEAVY_MISS1 12

#define SEQUENCE_BOWIE_IDLE1 1


RecvVarProxyFn fnSequenceProxyFn = nullptr;

RecvVarProxyFn oRecvnModelIndex;

int iBayonet;
int iFlip;
int iGut;
int iKarambit;
int iHuntsman;
int iFalchion;
int iBowie;
int iButterfly;
int iM9Bayonet;
int iDagger;
int iUrsus;
int iNavaja;
int iStiletto;
int iTalon;
int iGunGame;

void Hooked_RecvProxy_Viewmodel(CRecvProxyData *pData, void *pStruct, void *pOut)
{

int	default_t = g_pModelInfo->GetModelIndex("models/weapons/v_knife_default_t.mdl");
int	default_ct = g_pModelInfo->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
	iBayonet = g_pModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
	iFlip = g_pModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
	iGut = g_pModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
	iKarambit = g_pModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
	iHuntsman = g_pModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
	iFalchion = g_pModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
	iBowie = g_pModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");
	iButterfly = g_pModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
	iM9Bayonet = g_pModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
	iDagger = g_pModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
	int iUrsus = g_pModelInfo->GetModelIndex("models/weapons/v_knife_ursus.mdl");
	int iNavaja = g_pModelInfo->GetModelIndex("models/weapons/v_knife_gypsy_jackknife.mdl");
	int iStiletto = g_pModelInfo->GetModelIndex("models/weapons/v_knife_stiletto.mdl");
	int iTalon = g_pModelInfo->GetModelIndex("models/weapons/v_knife_widowmaker.mdl");

	iGunGame = g_pModelInfo->GetModelIndex("models/weapons/v_knife_gg.mdl");

	C_BaseEntity* pLocal = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (g_Settings.bKnifeChanger && pLocal)
	{
		if (pLocal->IsAlive() && (
			pData->m_Value.m_Int == default_t ||
			pData->m_Value.m_Int == default_ct ||
			pData->m_Value.m_Int == iBayonet ||
			pData->m_Value.m_Int == iFlip ||
			pData->m_Value.m_Int == iGunGame ||
			pData->m_Value.m_Int == iGut ||
			pData->m_Value.m_Int == iKarambit ||
			pData->m_Value.m_Int == iM9Bayonet ||
			pData->m_Value.m_Int == iHuntsman ||
			pData->m_Value.m_Int == iBowie ||
			pData->m_Value.m_Int == iButterfly ||
			pData->m_Value.m_Int == iFalchion ||
			pData->m_Value.m_Int == iDagger ||
			pData->m_Value.m_Int == iNavaja ||
			pData->m_Value.m_Int == iStiletto ||
			pData->m_Value.m_Int == iUrsus ||
			pData->m_Value.m_Int == iTalon))
		{
			
			switch (g_Settings.iKnifeModel)
			{
			case 0:
				break;
			case 1:
				pData->m_Value.m_Int = iBayonet;
				break;
			case 2:
				pData->m_Value.m_Int = iFlip;
				break;
			case 3:
				pData->m_Value.m_Int = iGut;
				break;
			case 4:
				pData->m_Value.m_Int = iKarambit;
				break;
			case 5:
				pData->m_Value.m_Int = iM9Bayonet;
				break;
			case 6:
				pData->m_Value.m_Int = iHuntsman;
				break;
			case 7:
				pData->m_Value.m_Int = iFalchion;
				break;
			case 8:
				pData->m_Value.m_Int = iBowie;
				break;
			case 9:
				pData->m_Value.m_Int = iButterfly;
				break;
			case 10:
				pData->m_Value.m_Int = iDagger;
				break;
			case 11:
				pData->m_Value.m_Int = iUrsus;
				break;
			case 12:
				pData->m_Value.m_Int = iNavaja;
				break;
			case 13:
				pData->m_Value.m_Int = iStiletto;
				break;
			case 14:
				pData->m_Value.m_Int = iTalon;
				break;
			}
		}
	}

	// Carry on the to original proxy
	oRecvnModelIndex(pData, pStruct, pOut);
}

void SetViewModelSequence2(const CRecvProxyData *pDataConst, void *pStruct, void *pOut)
{

	// Make the incoming data editable.
	CRecvProxyData* pData = const_cast<CRecvProxyData*>(pDataConst);

	// Confirm that we are replacing our view model and not someone elses.
	C_BaseViewmodel* pViewModel = static_cast<C_BaseViewmodel*>(pStruct);

	if (pViewModel)
	{
		C_BaseEntity* pOwner = static_cast<C_BaseEntity*>(g_pEntityList->GetClientEntity(pViewModel->GetOwner() & 0xFFF));

		// Compare the owner entity of this view model to the local player entity.
		if (pOwner && pOwner->EntIndex() == g_pEngine->GetLocalPlayer())
		{
			// Get the filename of the current view model.
			std::string szModel = g_pModelInfo->GetModelNamevoid(g_pModelInfo->GetModel(pViewModel->GetModelIndex()));

			// Store the current sequence.
			int m_nSequence = pData->m_Value.m_Int;
			if (szModel == "models/weapons/v_knife_butterfly.mdl")
			{
				// Fix animations for the Butterfly Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
					break;
				default:
					m_nSequence++;
				}
			}
			else if (szModel == "models/weapons/v_knife_falchion_advanced.mdl")
			{
				// Fix animations for the Falchion Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_FALCHION_IDLE1; break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomInt(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
					break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence--;
				}
			}
			else if (szModel == "models/weapons/v_knife_push.mdl")
			{
				// Fix animations for the Shadow Daggers.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_DAGGERS_IDLE1; break;
				case SEQUENCE_DEFAULT_LIGHT_MISS1:
				case SEQUENCE_DEFAULT_LIGHT_MISS2:
					m_nSequence = RandomInt(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
					break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomInt(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
					break;
				case SEQUENCE_DEFAULT_HEAVY_HIT1:
				case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence += 3; break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence += 2;
				}
			}
			else if (szModel == "models/weapons/v_knife_survival_bowie.mdl")
			{
				// Fix animations for the Bowie Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_BOWIE_IDLE1;
					break;
				default:
					m_nSequence--;
				}
			}
			else if (szModel == "models/weapons/v_knife_ursus.mdl")
			{
				// Fix animations for the Bowie Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
					break;
				default:
					m_nSequence++;
					break;
				}
			}
			else if (szModel == "models/weapons/v_knife_stiletto.mdl")
			{
				// Fix animations for the Bowie Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(12, 13);
					break;
				}
			}
			else if (szModel == "models/weapons/v_knife_widowmaker.mdl")
			{
				// Fix animations for the Bowie Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(14, 15);
					break;
				}
			}
			pData->m_Value.m_Int = m_nSequence;
		}
	}
	fnSequenceProxyFn(pData, pStruct, pOut);
}
void AnimationFixHook()
{
	for (ClientClass* pClass = g_pClientDll->GetAllClasses(); pClass; pClass = pClass->pNext) {
		if (!strcmp(pClass->pNetworkName, "CBaseViewModel")) {
			// Search for the 'm_nModelIndex' property.
			RecvTable* pClassTable = pClass->pRecvTable;

			for (int nIndex = 0; nIndex < pClassTable->nProps; nIndex++) {
				RecvProp* pProp = &pClassTable->pProps[nIndex];

				if (!pProp || strcmp(pProp->pVarName, "m_nSequence"))
					continue;

				// Store the original proxy function.
				fnSequenceProxyFn = static_cast<RecvVarProxyFn>(pProp->ProxyFn);

				// Replace the proxy function with our sequence changer.
				pProp->ProxyFn = static_cast<RecvVarProxyFn>(SetViewModelSequence2);

				break;
			}

			break;
		}
	}
}

void AnimationFixUnHook()
{
	for (ClientClass* pClass = g_pClientDll->GetAllClasses(); pClass; pClass = pClass->pNext) {
		if (!strcmp(pClass->pNetworkName, "CBaseViewModel")) {
			// Search for the 'm_nModelIndex' property.
			RecvTable* pClassTable = pClass->pRecvTable;

			for (int nIndex = 0; nIndex < pClassTable->nProps; nIndex++) {
				RecvProp* pProp = &pClassTable->pProps[nIndex];

				if (!pProp || strcmp(pProp->pVarName, "m_nSequence"))
					continue;

				// Replace the proxy function with our sequence changer.
				pProp->ProxyFn = fnSequenceProxyFn;

				break;
			}

			break;
		}
	}
}

void NetvarUnHook()
{
	AnimationFixUnHook();
	ClientClass *pClass = g_pClientDll->GetAllClasses();
	while (pClass)
	{
		const char *pszName = pClass->pRecvTable->pNetTableName;
		if (!strcmp(pszName, "DT_BaseViewModel"))
		{
			for (int i = 0; i < pClass->pRecvTable->nProps; i++)
			{
				RecvProp *pProp = &(pClass->pRecvTable->pProps[i]);
				const char *name = pProp->pVarName;

				// Knives
				if (!strcmp(name, "m_nModelIndex"))
				{
					oRecvnModelIndex = (RecvVarProxyFn)pProp->ProxyFn;
					pProp->ProxyFn = (RecvVarProxyFn)Hooked_RecvProxy_Viewmodel;
				}
			}
		}
		pClass = pClass->pNext;
	}
}

void NetvarHook()
{
	AnimationFixHook();
	ClientClass *pClass = g_pClientDll->GetAllClasses();
	while (pClass)
	{
		const char *pszName = pClass->pRecvTable->pNetTableName;
		if (!strcmp(pszName, "DT_BaseViewModel"))
		{
			for (int i = 0; i < pClass->pRecvTable->nProps; i++)
			{
				RecvProp *pProp = &(pClass->pRecvTable->pProps[i]);
				const char *name = pProp->pVarName;

				// Knives
				if (!strcmp(name, "m_nModelIndex"))
				{
					oRecvnModelIndex = (RecvVarProxyFn)pProp->ProxyFn;
					pProp->ProxyFn = (RecvVarProxyFn)Hooked_RecvProxy_Viewmodel;
				}
			}
		}
		pClass = pClass->pNext;
	}
}