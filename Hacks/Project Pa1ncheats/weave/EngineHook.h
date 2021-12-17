#pragma once
#include "Hooks.h"
#include <intrin.h>

int32_t __fastcall Hooked_IsBoxVisible(IEngineClient* engine_client, uint32_t, Vector& min, Vector& max)
{
	static auto BoxVisible = g_pEngineHook->GetOriginal< BoxVisibleFn >(32);

	static const auto ret = (hs::ret_engine::s().c_str());

	if (!memcmp(_ReturnAddress(), ret, 10))
		return 1;

	return BoxVisible(engine_client, min, max);
}

bool __fastcall Hooked_IsHLTV(IEngineClient* IEngineClient, uint32_t)
{
	static auto IsHLTV = g_pEngineHook->GetOriginal< IsHLTVFn >(93);

	static const auto return_to_setup_velocity = (csgo->Utils.FindPatternIDA(GetModuleHandleA(
		g_Modules[fnv::hash(hs::client_dll::s().c_str())].c_str()),
		hs::setup_vel::s().c_str()));
	static const auto return_to_accumulate_layers = (csgo->Utils.FindPatternIDA(GetModuleHandleA(
		g_Modules[fnv::hash(hs::client_dll::s().c_str())].c_str()),
		hs::accum_layers::s().c_str()));

	if (_ReturnAddress() == (uint32_t*)(return_to_accumulate_layers) && IEngineClient->IsInGame())
		return true;

	if (_ReturnAddress() == (uint32_t*)(return_to_setup_velocity) && csgo->UpdateMatrix && IEngineClient->IsInGame())
		return true;

	return IsHLTV(IEngineClient);
}

void __fastcall Hooked_FireEvent(IEngineClient* ecx)
{
	static auto FireEvent = g_pEngineHook->GetOriginal< fire_event_t >(59);

	auto ei = *(CEventInfo**)(std::uintptr_t(csgo->client_state) + 0x4E64);

	CEventInfo* next = nullptr;

	if (!ei)
		return FireEvent(ecx);

	do {
		next = ei->m_next;
		ei->m_fire_delay = 0.f;
		ei = next;
	} while (next);

	return FireEvent(ecx);
}