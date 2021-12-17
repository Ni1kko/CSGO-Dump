#include "hooks.hpp"

void __fastcall Hooks::ReadPacketEntities(void* _this, int, void* entInfo)
{
	auto ofunc = clientstate_hook.get_original<decltype(&ReadPacketEntities)>(index::ReadPacketEntities);

	ofunc(_this, 0, entInfo);
	if (g_EngineClient->IsInGame())
		g_Skins->FrameStageNotify(false);
}