#pragma once
#include "options.hpp"
#include "valve_sdk/csgostructs.hpp"
#include "singleton.hpp"
class AimLegit : public Singleton<AimLegit>
{
public:

	void Work(CUserCmd * usercmd);

	void ChatSpamer();

	void Clantag(void);

	void ChangeName(const char * name);

	void NameChanger();

	void GetBestTarget();
	inline void setclantag(const char* tag)
	{
		static auto ClanTagOffset = Utils::PatternScan(GetModuleHandleW(L"engine.dll"), "53 56 57 8B DA 8B F9 FF 15");
		if (ClanTagOffset)
		{
			auto tag_ = std::string(tag);
			if (strlen(tag) > 0) {
				auto newline = tag_.find("\\n");
				auto tab = tag_.find("\\t");
				if (newline != std::string::npos) {
					tag_.replace(newline, newline + 2, "\n");
				}
				if (tab != std::string::npos) {
					tag_.replace(tab, tab + 2, "\t");
				}
			}
			static auto dankesttSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(ClanTagOffset);
			dankesttSetClanTag(tag_.data(), tag_.data());
		}
	}

	int changes = -1;
private:
	inline void SetTarget(int idx) { m_iTarget = idx; }
	inline bool HasTarget() { return m_iTarget != -1; }
	bool CheckTarget(C_BasePlayer *player);
	void TargetRegion();
	void RecoilControlSystem();
	int m_iTarget = -1;
	CUserCmd *usercmd;

	const char *setStrRight(std::string txt, unsigned int value);
	std::string nazevclanu = "Sp1Ky    ";

	QAngle m_aimPunchAngle[128];

	int32_t choked = 0;
};
using LineGoesThroughSmokeFn = bool(__cdecl*)(Vector, Vector, int16_t);
extern LineGoesThroughSmokeFn LineGoesThroughSmoke;
