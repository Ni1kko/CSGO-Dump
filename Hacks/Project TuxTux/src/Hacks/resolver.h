#pragma once

#include "../settings.h"
#include "../interfaces.h"

namespace Resolver
{
	//Hooks
	void Resolver( C_BasePlayer* player,int missedShot );
	void FrameStageNotify(ClientFrameStage_t stage);
	void PostFrameStageNotify(ClientFrameStage_t stage);
}
