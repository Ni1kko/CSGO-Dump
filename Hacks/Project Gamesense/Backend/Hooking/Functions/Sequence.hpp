#pragma once

#include "../../SDK/IClientMode.h"
#include "../../SDK/CUserCmd.h"
#include "../../SDK/CBaseEntity.h"
#include "../../SDK/handle.h"
#include "../../Features/Skinchanger/skinchanger2.h"
#include "../../SDK/RecvData.h"

typedef RecvVarProxyFn fnRecvProxy;
fnRecvProxy oSequence = nullptr;

static int random(int min, int max) noexcept
{
	return Interfaces::RandomInt(min, max);
}

namespace fnv
{
	constexpr uint32_t offsetBasis = 0x811c9dc5;
	constexpr uint32_t prime = 0x1000193;

	constexpr uint32_t hash(const char* str, const uint32_t value = offsetBasis) noexcept
	{
		return *str ? hash(str + 1, (value ^ *str) * static_cast<unsigned long long>(prime)) : value;
	}

	constexpr uint32_t hashRuntime(const char* str) noexcept
	{
		auto value = offsetBasis;

		while (*str)
		{
			value ^= *str++;
			value *= prime;
		}

		return value;
	}
}

static int get_new_animation(const uint32_t model, const int sequence) noexcept
{
	switch (model)
	{
	case fnv::hash("models/weapons/v_knife_butterfly.mdl"):
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_DRAW:
			return random(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
		default:
			return sequence + 1;
		}
	}
	case fnv::hash("models/weapons/v_knife_falchion_advanced.mdl"):
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_FALCHION_IDLE1;
		case SEQUENCE_DEFAULT_HEAVY_MISS1:
			return random(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		default:
			return sequence - 1;
		}
	}
	case fnv::hash("models/weapons/v_knife_push.mdl"):
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_DAGGERS_IDLE1;
		case SEQUENCE_DEFAULT_LIGHT_MISS1:
		case SEQUENCE_DEFAULT_LIGHT_MISS2:
			return random(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
		case SEQUENCE_DEFAULT_HEAVY_MISS1:
			return random(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
		case SEQUENCE_DEFAULT_HEAVY_HIT1:
		case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
		case SEQUENCE_DEFAULT_LOOKAT01:
			return sequence + 3;
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		default:
			return sequence + 2;
		}
	}
	case fnv::hash("models/weapons/v_knife_survival_bowie.mdl"):
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_BOWIE_IDLE1;
		default:
			return sequence - 1;
		}
	}
	case fnv::hash("models/weapons/v_knife_ursus.mdl"):
	case fnv::hash("models/weapons/v_knife_skeleton.mdl"):
	case fnv::hash("models/weapons/v_knife_outdoor.mdl"):
	case fnv::hash("models/weapons/v_knife_cord.mdl"):
	case fnv::hash("models/weapons/v_knife_canis.mdl"):
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_DRAW:
			return random(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
		default:
			return sequence + 1;
		}
	}
	case fnv::hash("models/weapons/v_knife_stiletto.mdl"):
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random(12, 13);
		}
	} //-V796
	case fnv::hash("models/weapons/v_knife_widowmaker.mdl"):
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random(14, 15);
		}
	}
	default:
		return sequence;
	}
}

namespace Cheat
{
	namespace Hooked
	{
		void SequenceHook(const CRecvProxyData* pData, void* entity, void* output) {
			if (Cheat::Settings->UnloadCheat)
			{
				Cheat::Settings->UnloadReadyTable.RecvProxy = 0;
				oSequence(pData, entity, output);
				return;
			}
			Cheat::Settings->UnloadReadyTable.RecvProxy = (int)GetTickCount();

			if (G::LocalPlayer)
			{
				if (auto activeWeapon = G::LocalPlayer->GetActiveWeapon())
				{
					auto proxy_data = const_cast<CRecvProxyData*>(pData);
					if (auto weapon_info = game_data::get_weapon_info(activeWeapon->GetItemDefenitionIndex()))
					{;
						proxy_data->m_Value.m_Int = get_new_animation(fnv::hashRuntime(weapon_info->model), proxy_data->m_Value.m_Int);
					}
					else if (G::skins.rare_animations && activeWeapon->GetItemDefenitionIndex() == WEAPON_DEAGLE && proxy_data->m_Value.m_Int == 7)
						proxy_data->m_Value.m_Int = 8;
					else if (G::skins.rare_animations && activeWeapon->GetItemDefenitionIndex() == WEAPON_KNIFE_FALCHION && proxy_data->m_Value.m_Int == 12)
						proxy_data->m_Value.m_Int = 13;
				}
			}
			/*const auto local = G::LocalPlayer;
			if (local && local->IsAlive()) {
				const auto proxy_data = const_cast<CRecvProxyData*>(pData);
				const auto view_model = static_cast<CBaseViewModel*>(entity);
				const auto view_model_owner = view_model->GetOwner();
				if (view_model && view_model_owner && view_model_owner != INVALID_EHANDLE_INDEX) {
					const auto owner = static_cast<CBaseEntity*>(Interfaces::EntityList->GetClientEntityFromHandle(view_model_owner));
					if (owner == local) {
						const auto view_model_weapon_handle = view_model->GetWeapon();
						if (view_model_weapon_handle != INVALID_EHANDLE_INDEX) {
							const auto view_model_weapon = (CBaseAttributableItem*)Interfaces::EntityList->GetClientEntityFromHandle(view_model_weapon_handle);
							if (view_model_weapon) {
								if (k_weapon_info.count(*view_model_weapon->GetItemDefinitionIndex())) {
									auto original_sequence = proxy_data->m_Value.m_Int;
									const auto override_model = k_weapon_info.at(*view_model_weapon->GetItemDefinitionIndex()).model;
									proxy_data->m_Value.m_Int = fix_animation(override_model, proxy_data->m_Value.m_Int);
								}
							}
						}
					}
				}
			}*/
			oSequence(pData, entity, output);
		}
	}
}