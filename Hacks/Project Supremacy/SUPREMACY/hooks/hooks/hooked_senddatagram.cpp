#include "..\hooks.hpp"

#include "..\..\cheats\lagcompensation\lagcompensation.h"
#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);

using SendDatagram_t = int(__thiscall *)(void *, void *);

void NaNWalk(INetChannel * channel) {
	Vector angle = 0;
	g_csgo.m_engine()->GetViewAngles(angle);
	Vector velocity = g_ctx.m_local->m_vecVelocity();
	auto engine = g_csgo.m_engine();
	auto local = g_ctx.m_local;
	// 	static auto net_fakelag = g_csgo.m_cvar()->FindVar("net_fakelag");
	// 	static auto net_fakejitter = g_csgo.m_cvar()->FindVar("net_fakejitter");
	// 	static auto net_fakeloss = g_csgo.m_cvar()->FindVar("net_fakeloss");
	bool netfakelag = true;
	bool netfakejitter = true;
	bool netfakeloss = true;
	if (local->is_alive42() && engine->IsInGame() && engine->IsConnected())
	{
		if (g_cfg.nanwalk.enabled)
		{
			if (g_cfg.nanwalk.onlyonground && !local->m_fFlags() & FL_ONGROUND)
				return;

			if (GetAsyncKeyState(g_cfg.nanwalk.key))
			{
				int netchennel = RandomInt(1, 3);

				switch (netchennel)
				{
				case 1: channel->m_nInSequenceNr += g_cfg.nanwalk.amt * 20; break;
				case 2: channel->m_nInSequenceNr += g_cfg.nanwalk.amt * 10; break;
				case 3: channel->m_nInSequenceNr += g_cfg.nanwalk.amt * 15; break;
				default:channel->m_nInSequenceNr += g_cfg.nanwalk.amt * 20; break;
				}

				//special

// 				if (netfakejitter) {
// 					net_fakejitter->SetValue(g_cfg.nanwalk.amt * 3);
// 					netfakejitter = false;
// 				}
// 				else if (!netfakejitter)
// 				{
// 					net_fakejitter->SetValue(0);
// 					netfakejitter = true;
// 				}
// 				if (netfakelag) {
// 					net_fakelag->SetValue(g_cfg.nanwalk.amt * 5);
// 					netfakelag = false;
// 				}
// 				else if (!netfakelag)
// 				{
// 					net_fakelag->SetValue(0);
// 					netfakelag = true;
// 				}
// 				if (netfakeloss) {
// 					net_fakeloss->SetValue(g_cfg.nanwalk.amt * 2);
// 					netfakeloss = false;
// 				}
// 				else if (!netfakeloss)
// 				{
// 					net_fakeloss->SetValue(0);
// 					netfakeloss = true;
// 				}
			}
			// 			else
			// 			{
			// 				net_fakejitter->SetValue(0);
			// 				net_fakeloss->SetValue(0);
			// 				net_fakelag->SetValue(0);
			// 			}
		}
	}
}

int __fastcall hooks::hooked_senddatagram(void * net_channel, void *, void * datagram) {
	static auto original_fn = netchannel_hook->get_func_address<SendDatagram_t>(46);

	if (!g_cfg.misc.ping_spike || !g_ctx.available())
		return original_fn(net_channel, datagram);

	auto * channel = reinterpret_cast<INetChannel *>(net_channel);

	int instate = channel->m_nInReliableState;
	int insequencenr = channel->m_nInSequenceNr;

	lagcompensation::get().add_latency(channel);

	/*	NaNWalk(channel);*/

	int ret = original_fn(channel, datagram);

	channel->m_nInReliableState = instate;
	channel->m_nInSequenceNr = insequencenr;

	return ret;
}