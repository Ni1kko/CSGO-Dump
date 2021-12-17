#include "spammers.h"

std::string Tag = " ";
std::string Tag2 = " ";
void spammers::clan_tag()
{
	auto set_clan_tag = [](const char* tag, const char* clan_name) -> void
	{
		static auto pSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(util::pattern_scan("engine.dll", "53 56 57 8B DA 8B F9 FF 15"));
		pSetClanTag(tag, clan_name);
	};
	if (g_cfg.misc.clantag_spammer)
	{
		static size_t lastTime = 0;

		if (GetTickCount() > lastTime)
		{
			Tag += Tag.at(0);
			Tag.erase(0, 1);
			switch (int(g_csgo.m_globals()->m_curtime * 2.4) % 27)
			{
			case 0: set_clan_tag("supremacy beta    ", "supremacy beta    "); break;
			case 1: set_clan_tag(" supremacy beta   ", " supremacy beta   "); break;
			case 2: set_clan_tag("  supremacy beta  ", "  supremacy beta  "); break;
			case 3: set_clan_tag("   supremacy beta ", "   supremacy beta "); break;
			case 4: set_clan_tag("    supremacy beta", "    supremacy beta"); break;
			case 5: set_clan_tag("a    supremacy bet", "a    supremacy bet"); break;
			case 6: set_clan_tag("ta    supremacy be", "ta    supremacy be"); break;
			case 7: set_clan_tag("eta    supremacy b", "eta    supremacy b"); break;
			case 8: set_clan_tag("beta    supremacy ", "beta    supremacy "); break;
			case 9: set_clan_tag(" beta    supremacy", " beta    supremacy"); break;
			case 10: set_clan_tag(" supremacy  beta ", " supremacy  beta   "); break;
			}
			lastTime = GetTickCount() + 650;
		}

	}
	else
	{
		auto cl_clanid = g_csgo.m_cvar()->FindVar("cl_clanid");
		cl_clanid->SetValue(-1);
	}
}