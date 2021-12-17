#pragma once

namespace SDK
{
	class CUserCmd;
	class CBaseEntity;
}

class c_fake_walk
{
public:
	int choked;
	void do_fake_walk(SDK::CUserCmd * cmd);
};

extern c_fake_walk* fakewalk;