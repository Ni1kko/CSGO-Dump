#pragma once
#include "Hacks.h"

class fakelag
{
public:

	void break_lagcomp(bool & send_packet, int ammount);

	int break_lagcomp_mm();

	int break_lagcomp_mm(float ammount);

	int break_lagcomp_mm_2();

	void when_enemy_sight(bool & bSendPacket, IClientEntity * local, float ammount, bool & break_lc);

	void mirror_adaptive(bool & packet, IClientEntity * local, int ammount, bool break_lc, CUserCmd * cmd);

	void aimware_adaptive(bool & send_packet, IClientEntity * local, int ammount);

	void velocity(bool & packet, IClientEntity * local, int ammount, bool break_lc);

	void Fakelag(CUserCmd * pCmd, bool & bSendPacket);

	bool shot = false;
};

extern fakelag * c_fakelag;