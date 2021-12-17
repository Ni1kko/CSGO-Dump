#pragma once

class C_BasePlayer;
class CUserCmd;
class IGameEvent;

class Misc
{
public:
	Misc() {}

	void BunnyHop(CUserCmd* cmd);
	void AutoStafe(CUserCmd* cmd);

	void Fakelag(CUserCmd* cmd, bool& bSendPacket);
	void HUDModulation();
	void ToggleVGuiModulation(bool enable);
	void NightMode();
	void ThirdPerson();
	void BuyInfo(IGameEvent* event);
	void SpectatorList();
};

inline Misc* g_Misc;