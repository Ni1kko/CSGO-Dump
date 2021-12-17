#pragma once

namespace SDK
{
	class CUserCmd;
	class CBaseWeapon;
}

class CMovement
{
public:
	void bunnyhop(SDK::CUserCmd* cmd);
	void autostrafer(SDK::CUserCmd* cmd);
	void duckinair(SDK::CUserCmd* cmd);
};

extern CMovement* movement;