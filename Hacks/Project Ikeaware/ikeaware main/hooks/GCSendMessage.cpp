#include "hooks.hpp"

#include "..//helpers/proto/protobuf.hpp"

EGCResult __fastcall Hooks::GCSendMessage(void* _this, void*, uint32_t unMsgType, const void* pubData, uint32_t cubData)
{
	static auto ofunc = steamapi_hook.get_original<decltype(&GCSendMessage)>(index::SendMsg);

	Protobuf::PreSendMessage(unMsgType, const_cast<void*>(pubData), cubData);

	return ofunc(_this, 0, unMsgType, pubData, cubData);
}