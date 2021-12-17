#include "Skinchanger.h"

SkinChanger* skin_changer;

HANDLE worldmodel_handle;
C_BaseCombatWeapon* worldmodel;

void SkinChanger::KnifeChanger()
{
	const auto view_model_handle = g::pLocalEntity->GetViewModelIndex();
	if (!view_model_handle)
		return;

	const auto view_model = reinterpret_cast<C_BaseViewmodel*>(g_pEntityList->GetClientEntityFromHandle(view_model_handle));

	auto viewmodel_weapon = reinterpret_cast<C_BaseCombatWeapon*>(g_pEntityList->GetClientEntity(view_model->GetWeaponIndex()));
	if (!viewmodel_weapon)
		return;

	int iBayonet = g_pModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
	int iFlip = g_pModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
	int iGut = g_pModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
	int iKarambit = g_pModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
	int iHuntsman = g_pModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
	int iFalchion = g_pModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
	int iBowie = g_pModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");
	int iButterfly = g_pModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
	int iM9Bayonet = g_pModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
	int iDagger = g_pModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
	int Ursus = g_pModelInfo->GetModelIndex("models/weapons/v_knife_ursus.mdl");
	int Navaja = g_pModelInfo->GetModelIndex("models/weapons/v_knife_gypsy_jackknife.mdl");
	int Stiletto = g_pModelInfo->GetModelIndex("models/weapons/v_knife_stiletto.mdl");
	int Talon = g_pModelInfo->GetModelIndex("models/weapons/v_knife_widowmaker.mdl");

	if (!g::pLocalEntity) return;
	auto weapons = g::pLocalEntity->m_hMyWeapons();
	for (size_t i = 0; weapons[i] != INVALID_EHANDLE_INDEX; i++)
	{
		C_BaseEntity *pEntity = (C_BaseEntity*)g_pEntityList->GetClientEntityFromHandle(weapons[i]);

		if (pEntity)
		{
			C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)pEntity;

			if (pEntity->GetClientClass()->m_ClassID == (int)EClassIds::CKnife)
			{
				HANDLE worldmodel_handle2 = pWeapon->m_hWeaponWorldModel_h();
				if (worldmodel_handle2) worldmodel = (C_BaseCombatWeapon*)g_pEntityList->GetClientEntityFromHandle(pWeapon->m_hWeaponWorldModel_c());

				switch (g_Settings.iKnifeModel)
				{
				case 0:
					break;
				case 1:
					if (viewmodel_weapon == pWeapon && view_model->m_nModelIndex() != iBayonet)
						view_model->SetModelIndex(iBayonet);

					*pWeapon->ModelIndex() = iBayonet; // m_nModelIndex
					*pWeapon->ViewModelIndex() = iBayonet;
					if (worldmodel)
						*worldmodel->ModelIndex() = iBayonet + 1;

					*pWeapon->fixskins() = 500;
					*pWeapon->GetEntityQuality() = 3;
					break;
				case 2:
					
					if (viewmodel_weapon == pWeapon && view_model->m_nModelIndex() != iFlip)
						view_model->SetModelIndex(iFlip);

					*pWeapon->ModelIndex() = iFlip; // m_nModelIndex
					*pWeapon->ViewModelIndex() = iFlip;
					if (worldmodel)
						*worldmodel->ModelIndex() = iFlip + 1;

					*pWeapon->fixskins() = 505;
					*pWeapon->GetEntityQuality() = 3;
					break;
				case 3:
					
					if (viewmodel_weapon == pWeapon && view_model->m_nModelIndex() != iGut)
						view_model->SetModelIndex(iGut);

					*pWeapon->ModelIndex() = iGut; // m_nModelIndex
					*pWeapon->ViewModelIndex() = iGut;
					if (worldmodel)
						*worldmodel->ModelIndex() = iGut + 1;

					*pWeapon->fixskins() = 506;
					*pWeapon->GetEntityQuality() = 3;
					break;
				case 4:
					
					if (viewmodel_weapon == pWeapon && view_model->m_nModelIndex() != iKarambit)
						view_model->SetModelIndex(iKarambit);

					*pWeapon->ModelIndex() = iKarambit; // m_nModelIndex
					*pWeapon->ViewModelIndex() = iKarambit;
					if (worldmodel)
						*worldmodel->ModelIndex() = iKarambit + 1;

					*pWeapon->fixskins() = 507;
					*pWeapon->GetEntityQuality() = 3;
					break;
				case 5:
					
					if (viewmodel_weapon == pWeapon && view_model->m_nModelIndex() != iM9Bayonet)
						view_model->SetModelIndex(iM9Bayonet);

					*pWeapon->ModelIndex() = iM9Bayonet; // m_nModelIndex
					*pWeapon->ViewModelIndex() = iM9Bayonet;
					if (worldmodel)
						*worldmodel->ModelIndex() = iM9Bayonet + 1;

					*pWeapon->fixskins() = 508;
					*pWeapon->GetEntityQuality() = 3;
					break;
				case 6:
					if (viewmodel_weapon == pWeapon && view_model->m_nModelIndex() != iHuntsman)
						view_model->SetModelIndex(iHuntsman);

					*pWeapon->ModelIndex() = iHuntsman; // m_nModelIndex
					*pWeapon->ViewModelIndex() = iHuntsman;
					if (worldmodel)
						*worldmodel->ModelIndex() = iHuntsman + 1;

					*pWeapon->fixskins() = 509;
					*pWeapon->GetEntityQuality() = 3;
					break;
				case 7:
					
					if (viewmodel_weapon == pWeapon && view_model->m_nModelIndex() != iFalchion)
						view_model->SetModelIndex(iFalchion);

					*pWeapon->ModelIndex() = iFalchion; // m_nModelIndex
					*pWeapon->ViewModelIndex() = iFalchion;
					if (worldmodel)
						*worldmodel->ModelIndex() = iFalchion + 1;

					*pWeapon->fixskins() = 512;
					*pWeapon->GetEntityQuality() = 3;
					break;
				case 8:
					
					if (viewmodel_weapon == pWeapon && view_model->m_nModelIndex() != iBowie)
						view_model->SetModelIndex(iBowie);

					*pWeapon->ModelIndex() = iBowie; // m_nModelIndex
					*pWeapon->ViewModelIndex() = iBowie;
					if (worldmodel)
						*worldmodel->ModelIndex() = iBowie + 1;

					*pWeapon->fixskins() = 514;
					*pWeapon->GetEntityQuality() = 3;
					break;
				case 9:
					
					if (viewmodel_weapon == pWeapon && view_model->m_nModelIndex() != iButterfly)
						view_model->SetModelIndex(iButterfly);

					*pWeapon->ModelIndex() = iButterfly; // m_nModelIndex
					*pWeapon->ViewModelIndex() = iButterfly;
					if (worldmodel)
						*worldmodel->ModelIndex() = iButterfly + 1;

					*pWeapon->fixskins() = 515;
					*pWeapon->GetEntityQuality() = 3;
					break;
				case 10:
					
					if (viewmodel_weapon == pWeapon && view_model->m_nModelIndex() != iDagger)
						view_model->SetModelIndex(iDagger);

					*pWeapon->ModelIndex() = iDagger; // m_nModelIndex
					*pWeapon->ViewModelIndex() = iDagger;
					if (worldmodel)
						*worldmodel->ModelIndex() = iDagger + 1;

					*pWeapon->fixskins() = 516;
					*pWeapon->GetEntityQuality() = 3;
					break;
				case 11:
					
					if (viewmodel_weapon == pWeapon && view_model->m_nModelIndex() != Ursus)
						view_model->SetModelIndex(Ursus);

					*pWeapon->ModelIndex() = Ursus; // m_nModelIndex
					*pWeapon->ViewModelIndex() = Ursus;
					if (worldmodel)
						*worldmodel->ModelIndex() = Ursus + 1;

					*pWeapon->fixskins() = 519;
					*pWeapon->GetEntityQuality() = 3;
					break;
				case 12:
					
					if (viewmodel_weapon == pWeapon && view_model->m_nModelIndex() != Navaja)
						view_model->SetModelIndex(Navaja);

					*pWeapon->ModelIndex() = Navaja; // m_nModelIndex
					*pWeapon->ViewModelIndex() = Navaja;
					if (worldmodel)
						*worldmodel->ModelIndex() = Navaja + 1;

					*pWeapon->fixskins() = 520;
					*pWeapon->GetEntityQuality() = 3;
					break;
				case 13:
					
					if (viewmodel_weapon == pWeapon && view_model->m_nModelIndex() != Stiletto)
						view_model->SetModelIndex(Stiletto);

					*pWeapon->ModelIndex() = Stiletto; // m_nModelIndex
					*pWeapon->ViewModelIndex() = Stiletto;
					if (worldmodel)
						*worldmodel->ModelIndex() = Stiletto + 1;

					*pWeapon->fixskins() = 522;
					*pWeapon->GetEntityQuality() = 3;
					break;
				case 14:
					
					if (viewmodel_weapon == pWeapon && view_model->m_nModelIndex() != Talon)
						view_model->SetModelIndex(Talon);

					*pWeapon->ModelIndex() = Talon; // m_nModelIndex
					*pWeapon->ViewModelIndex() = Talon;
					if (worldmodel)
						*worldmodel->ModelIndex() = Talon + 1;

					*pWeapon->fixskins() = 523;
					*pWeapon->GetEntityQuality() = 3;
					break;
				}
			}
			*pWeapon->ItemIDHigh() = -1;
		}
	}
}

void SkinChanger::ChangeSkins()
{
	if (!g::pLocalEntity) return;
	auto weapons = g::pLocalEntity->m_hMyWeapons();
	for (size_t i = 0; weapons[i] != INVALID_EHANDLE_INDEX; i++)
	{
		C_BaseEntity *pEntity = (C_BaseEntity*)g_pEntityList->GetClientEntityFromHandle(weapons[i]);

		if (pEntity)
		{
			C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)pEntity;

			HANDLE worldmodel_handle2 = pWeapon->m_hWeaponWorldModel_h();
			if (worldmodel_handle2) worldmodel = (C_BaseCombatWeapon*)g_pEntityList->GetClientEntityFromHandle(pWeapon->m_hWeaponWorldModel_c());

			int weapon = *pWeapon->fixskins();
			switch (weapon)
			{
				case ItemDefinitionIndex::WEAPON_DEAGLE:
				{
					*pWeapon->FallbackPaintKit() = 841;
					break;
				}
				case ItemDefinitionIndex::WEAPON_ELITE:
				{
					*pWeapon->FallbackPaintKit() = 276;
					break;
				}
				case ItemDefinitionIndex::WEAPON_FIVESEVEN:
				{
					*pWeapon->FallbackPaintKit() = 427;
					break;
				}
				case ItemDefinitionIndex::WEAPON_GLOCK:
				{
					*pWeapon->FallbackPaintKit() = 38;
					break;
				}
				case ItemDefinitionIndex::WEAPON_AK47:
				{
					*pWeapon->FallbackPaintKit() = 639;
					break;
				}
				case ItemDefinitionIndex::WEAPON_AUG:
				{
					*pWeapon->FallbackPaintKit() = 9;
					break;
				}
				case ItemDefinitionIndex::WEAPON_AWP:
				{
					*pWeapon->FallbackPaintKit() = 838;
					break;
				}
				case ItemDefinitionIndex::WEAPON_SCAR20:
				{
					*pWeapon->FallbackPaintKit() = 312;
						break;
				}
				case ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET:
				{
					*pWeapon->FallbackPaintKit() = 858;
					break;
				}
			}
			*pWeapon->ItemIDHigh() = -1;
			*pWeapon->FallbackWear() = 0.000001f;
		}
	}
}