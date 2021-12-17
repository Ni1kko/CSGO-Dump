#pragma once
#include "Hooks.h"
#include "iBaseClientDLL.h"

int __fastcall Hooked_SendDatagram(void* netchan, void*, void *datagram)
{
	if (!H::SendDatagram)
		H::SendDatagram = csgo->g_pNetChannelHook->GetOriginal<send_datagram_fn>(46);

	if (!interfaces.engine->IsInGame() || csgo->DoUnload)
		return H::SendDatagram(netchan, datagram);

	const auto net_channel = reinterpret_cast<INetChannel*> (netchan);
	const auto backup_seqnr = net_channel->iInSequenceNr;
	g_Backtrack->DoPingSpike(net_channel);
	const auto ret = H::SendDatagram(net_channel, datagram);
	net_channel->iInSequenceNr = backup_seqnr;
	return ret;
}

bool __fastcall Hooked_SendNetMsg(INetChannel* pNetChan, void* edx, INetMessage& msg, bool bForceReliable, bool bVoice)
{
    auto SendNetMsg = csgo->g_pNetChannelHook->GetOriginal<send_net_msg_fn>(40);

    if (msg.GetType() == 14) // Return and don't send messsage if its FileCRCCheck
        return false;

    if (msg.GetGroup() == 9) // Fix lag when transmitting voice and fakelagging
        bVoice = true;

    return SendNetMsg(pNetChan, msg, bForceReliable, bVoice);
}

void __fastcall Hooked_ProcessPacket(void* ecx, void* edx, void* packet, bool header) {

    auto ProcessPacket = csgo->g_pNetChannelHook->GetOriginal<ProcessPacket_t>(39);
    if (!csgo->client_state->pNetChannel)
        return ProcessPacket(ecx, packet, header);

    ProcessPacket(ecx, packet, header);

    // get this from CL_FireEvents string "Failed to execute event for classId" in engine.dll
    for (CEventInfo* it{ csgo->client_state->pEvents }; it != nullptr; it = it->m_next) {
        if (!it->m_class_id)
            continue;

        // set all delays to instant.
        it->m_fire_delay = 0.f;
    }

    interfaces.engine->FireEvents();
}