#include "hooks.hpp"

#include "..//helpers/proto/protobuf.hpp"

EGCResult __fastcall Hooks::GCRetrieveMsg(void* _this, void*, uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize)
{
	static auto ofunc = steamapi_hook.get_original<decltype(&GCRetrieveMsg)>(index::RetrieveMsg);
	auto status = ofunc(_this, 0, punMsgType, pubDest, cubDest, pcubMsgSize);

	if (status == k_EGCResultOK)
	{
		void* thisPtr = nullptr;
		__asm mov thisPtr, ebx;
		auto oldEBP = *reinterpret_cast<void**>((uint32_t)_AddressOfReturnAddress() - 4);

		uint32_t messageType = *punMsgType & 0x7FFFFFFF;
		Protobuf::ReceiveMessage(thisPtr, oldEBP, messageType, pubDest, cubDest, pcubMsgSize);
	}

	return status;
}