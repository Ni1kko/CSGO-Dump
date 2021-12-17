#pragma once

class IGameRules
{
public:
	bool IsFreezePeriod() {

		return *(bool*)((unsigned int)this + 0x20);
	}
	bool IsValveDS() {

		return *(bool*)((unsigned int)this + 0x75);
	}
	bool IsBombPlanted() {

		return *(bool*)((unsigned int)this + 0x8D1);
	}

};