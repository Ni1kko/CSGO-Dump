#include "Protobuffs.h"
#include "Protobuffs\profile_info_changer.h"
#include "Protobuffs\inventory_changer.h"
#include "steam.h"
#include "IMemAlloc.h"
#include "rankchanger.h"
#include <bitset>
#include "../../SDK/CBaseEntity.h"
#define CAST(cast, address, add) reinterpret_cast<cast>((uint32_t)address + (uint32_t)add)

void Protobuffs::WritePacket(std::string packet, void* thisPtr, void* oldEBP, void *pubDest, uint32_t cubDest, uint32_t *pcubMsgSize)
{
	if ((uint32_t)packet.size() <= cubDest - 8)
	{
		memcpy((void*)((DWORD)pubDest + 8), (void*)packet.data(), packet.size());
		*pcubMsgSize = packet.size() + 8;
	}
	else if (INTERFACES::g_pMemAlloc)
	{
		auto memPtr = *CAST(void**, thisPtr, 0x14);
		auto memPtrSize = *CAST(uint32_t*, thisPtr, 0x18);
		auto newSize = (memPtrSize - cubDest) + packet.size() + 8;

		auto memory = INTERFACES::g_pMemAlloc->Realloc(memPtr, newSize + 4);

		*CAST(void**, thisPtr, 0x14) = memory;
		*CAST(uint32_t*, thisPtr, 0x18) = newSize;
		*CAST(void**, oldEBP, -0x14) = memory;

		memcpy(CAST(void*, memory, 24), (void*)packet.data(), packet.size());

		*pcubMsgSize = packet.size() + 8;
	}
}

void Protobuffs::ReceiveMessage(void* thisPtr, void* oldEBP, uint32_t messageType, void *pubDest, uint32_t cubDest, uint32_t *pcubMsgSize)
{
	if (messageType == k_EMsgGCCStrike15_v2_MatchmakingGC2ClientHello /*&& Shonax.RankChanger.enabled*/)
	{
		auto packet = profile_info_changer(pubDest, pcubMsgSize);
		WritePacket(packet, thisPtr, oldEBP, pubDest, cubDest, pcubMsgSize);
		//EquipAll();
	}
	else if (messageType == k_EMsgGCClientWelcome && (SETTINGS::settings.Inventarchanger.enable || SETTINGS::settings.Inventarchanger.medalenabled || SETTINGS::settings.Inventarchanger.musickitenabled))
	{
		auto packet = inventory_changer(pubDest, pcubMsgSize);
		WritePacket(packet, thisPtr, oldEBP, pubDest, cubDest, pcubMsgSize);
	}
}

bool Protobuffs::PreSendMessage(uint32_t &unMsgType, void* pubData, uint32_t &cubData)
{
	uint32_t MessageType = unMsgType & 0x7FFFFFFF;

	if (MessageType == k_EMsgGCAdjustItemEquippedState)
	{
		return inventory_changer_presend(pubData, cubData);
	}

	return true;
}

///////////////////////////////////
/******** Custom Messages ********/
///////////////////////////////////
bool Protobuffs::SendClientHello()
{
	// message CMsgClientHello {
	// 	repeated .CMsgSOCacheHaveVersion socache_have_versions = 2;
	// 	optional uint32 client_session_need = 3;
	// 	optional uint32 partner_accountflags = 7;
	// }
	ProtoWriter msg(7);
	msg.add(Field(3, TYPE_UINT32, (int64_t)1)); //client_session_need
	auto packet = msg.serialize();

	void* ptr = malloc(packet.size() + 8);

	if (!ptr)
		return false;

	((uint32_t*)ptr)[0] = k_EMsgGCClientHello | ((DWORD)1 << 31);
	((uint32_t*)ptr)[1] = 0;

	memcpy((void*)((DWORD)ptr + 8), (void*)packet.data(), packet.size());
	bool result = INTERFACES::g_SteamGameCoordinator->GCSendMessage(k_EMsgGCClientHello | ((DWORD)1 << 31), ptr, packet.size() + 8) == k_EGCResultOK;
	free(ptr);

	return result;
}

bool Protobuffs::SendMatchmakingClient2GCHello()
{
	ProtoWriter msg(0);
	auto packet = msg.serialize();
	void* ptr = malloc(packet.size() + 8);

	if (!ptr)
		return false;

	((uint32_t*)ptr)[0] = k_EMsgGCCStrike15_v2_MatchmakingClient2GCHello | ((DWORD)1 << 31);
	((uint32_t*)ptr)[1] = 0;

	memcpy((void*)((DWORD)ptr + 8), (void*)packet.data(), packet.size());
	bool result = INTERFACES::g_SteamGameCoordinator->GCSendMessage(k_EMsgGCCStrike15_v2_MatchmakingClient2GCHello | ((DWORD)1 << 31), ptr, packet.size() + 8) == k_EGCResultOK;
	free(ptr);

	return result;
}

bool Protobuffs::EquipWeapon(int weaponid, int classid, int slotid)
{
	// message CMsgAdjustItemEquippedState {
	// 	optional uint64 item_id = 1;
	// 	optional uint32 new_class = 2;
	// 	optional uint32 new_slot = 3;
	// 	optional bool swap = 4;
	// }
	ProtoWriter msg(4);
	msg.add(Field(1, TYPE_UINT64, (int64_t)(/*START_ITEM_INDEX*/10000 + weaponid))); //item_id
	msg.add(Field(2, TYPE_UINT32, (int64_t)classid)); //new_class
	msg.add(Field(3, TYPE_UINT32, (int64_t)slotid)); //new_slot
	msg.add(Field(4, TYPE_BOOL, (int64_t)true)); //swap
	auto packet = msg.serialize();

	void* ptr = malloc(packet.size() + 8);

	if (!ptr)
		return false;

	((uint32_t*)ptr)[0] = k_EMsgGCAdjustItemEquippedState | ((DWORD)1 << 31);
	((uint32_t*)ptr)[1] = 0;

	memcpy((void*)((DWORD)ptr + 8), (void*)packet.data(), packet.size());
	bool result = INTERFACES::g_SteamGameCoordinator->GCSendMessage(k_EMsgGCAdjustItemEquippedState | ((DWORD)1 << 31), ptr, packet.size() + 8) == k_EGCResultOK;
	free(ptr);

	return result;
}
static int GetSlotID(int definition_index)
{
	switch (definition_index)
	{
	case WEAPON_KNIFE_BAYONET:
	case WEAPON_KNIFE_FLIP:
	case WEAPON_KNIFE_GUT:
	case WEAPON_KNIFE_KARAMBIT:
	case WEAPON_KNIFE_M9_BAYONET:
	case WEAPON_KNIFE_TACTICAL:
	case WEAPON_KNIFE_FALCHION:
	case WEAPON_KNIFE_SURVIVAL_BOWIE:
	case WEAPON_KNIFE_BUTTERFLY:
	case WEAPON_KNIFE_PUSH:
		return 0;
	case WEAPON_USP_SILENCER:
	case WEAPON_HKP2000:
	case WEAPON_GLOCK:
		return 2;
	case WEAPON_ELITE:
		return 3;
	case WEAPON_P250:
		return 4;
	case WEAPON_TEC9:
	case WEAPON_CZ75A:
	case WEAPON_FIVESEVEN:
		return 5;
	case WEAPON_DEAGLE:
	case WEAPON_REVOLVER:
		return 6;
	case WEAPON_MP9:
	case WEAPON_MAC10:
		return 8;
	case WEAPON_MP7:
		return 9;
	case WEAPON_UMP45:
		return 10;
	case WEAPON_P90:
		return 11;
	case WEAPON_BIZON:
		return 12;
	case WEAPON_FAMAS:
	case WEAPON_GALILAR:
		return 14;
	case WEAPON_M4A1_SILENCER:
	case WEAPON_M4A1:
	case WEAPON_AK47:
		return 15;
	case WEAPON_SSG08:
		return 16;
	case WEAPON_SG553:
	case WEAPON_AUG:
		return 17;
	case WEAPON_AWP:
		return 18;
	case WEAPON_G3SG1:
	case WEAPON_SCAR20:
		return 19;
	case WEAPON_NOVA:
		return 20;
	case WEAPON_XM1014:
		return 21;
	case WEAPON_SAWEDOFF:
	case WEAPON_MAG7:
		return 22;
	case WEAPON_M249:
		return 23;
	case WEAPON_NEGEV:
		return 24;
	case GLOVE_STUDDED_BLOODHOUND:
	case GLOVE_SPORTY:
	case GLOVE_SLICK:
	case GLOVE_LEATHER_WRAP:
	case GLOVE_MOTORCYCLE:
	case GLOVE_SPECIALIST:
		return 41;
	default:
		return -1;
	}
}
std::string cleanstd;
void Protobuffs::EquipAll()
{
	for (auto& x : SETTINGS::settings.Inventarchanger.g_skins) {
		auto def_index = (ItemDefinitionIndex)x.wId;
		EquipWeapon(def_index, TEAM_COUNTER_TERRORIST, GetSlotID(def_index));
		EquipWeapon(def_index, TEAM_TERRORIST, GetSlotID(def_index));
	}
}