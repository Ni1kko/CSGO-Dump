#include "../singleton.hpp"
#include "../valve_sdk/csgostructs.hpp"

#pragma once
class NoSmoke : public Singleton<NoSmoke>
{
	public:
		void OnFrameStageNotify();
};

