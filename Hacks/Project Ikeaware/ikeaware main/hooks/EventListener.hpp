#pragma once

#include "..//valve_sdk/sdk.hpp"

class CGameEventListener final : public IGameEventListener2
{
	void FireGameEvent(IGameEvent* p_event) override;
	int GetEventDebugID() override { return EVENT_DEBUG_ID_INIT; }
};