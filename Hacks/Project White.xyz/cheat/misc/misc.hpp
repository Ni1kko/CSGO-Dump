#pragma once
#include "../other/singleton.hpp"
#include "../../sdk/structs.hpp"

class Misc : public Singleton<Misc> {
	friend class Singleton<Misc>;
public:
	std::string OldSkyname = "";
	bool NightmodeDone = true;
	void UpdateWorldTextures();
	void NightMode();
	void Chatspam();
	void Radiospam();
	void Namespam();
	void Clantag();
	void BunnyHop(CUserCmd* cmd);
	void AutoStrafe(CUserCmd* cmd);
	void EdgeJump(CUserCmd* cmd);
	void LeftKnife();
	void SniperCrosshair();
	void RecoilCrosshair();
	void correct_mouse(CUserCmd* cmd);
	void DisablePanoramaBlur();
	void No3dSky();
	void NoShadows();
	void DisablePostProcessing();
	void Desync(CUserCmd* cmd, bool* send_packet);
	void AgentChanger(int v_char, int p_id);
};