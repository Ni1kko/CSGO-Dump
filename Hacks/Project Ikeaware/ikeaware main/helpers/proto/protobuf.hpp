#pragma once
#include <windows.h>
#include <string>

#include "writer.hpp"
#include "message.hpp"
#include "..//..//valve_sdk/sdk.hpp"

namespace Protobuf
{
	void WritePacket(std::string packet, void* thisPtr, void* oldEBP, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize);
	void ReceiveMessage(void* thisPtr, void* oldEBP, uint32_t messageType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize);
	bool PreSendMessage(uint32_t& unMsgType, void* pubData, uint32_t& cubData);
	bool SendClientHello();
	bool SendMatchmakingClient2GCHello();
	bool EquipWeapon(int weaponid, int classid, int slotid);
};