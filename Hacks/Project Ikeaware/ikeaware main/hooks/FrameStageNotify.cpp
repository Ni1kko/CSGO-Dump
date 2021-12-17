#include "hooks.hpp"

void __fastcall Hooks::FrameStageNotify(void* _this, ClientFrameStage_t stage)
{
	static auto ofunc = hlclient_hook.get_original<decltype(&FrameStageNotify)>(index::FrameStageNotify);

	ofunc(g_CHLClient, stage);
}