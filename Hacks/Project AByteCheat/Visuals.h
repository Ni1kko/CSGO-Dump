#pragma once

#include "Hacks.h"



class CVisuals : public CHack
{
public:
	void Init();
	void Draw();
	void Move(CUserCmd *pCmd, bool &bSendPacket);
private:
	void DrawCrosshair();
	void NoScopeCrosshair();
	void DrawRecoilCrosshair();
	void SpreadCrosshair();
	void AutowallCrosshair();
};

struct impact_info {
	float x, y, z;
	long long time;
};

struct hitmarker_info {
	impact_info impact;
	int alpha;
};

extern std::vector<impact_info> impacts;
extern std::vector<hitmarker_info> Xhitmarkers;