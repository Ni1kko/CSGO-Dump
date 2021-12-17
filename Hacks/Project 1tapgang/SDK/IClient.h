#pragma once
#include <Windows.h>
#include "../UTILS/vmt.h"


namespace SDK
{
	class ClientClass;

	class IClient
	{
	public:
		ClientClass* GetAllClasses(void)
		{
			typedef ClientClass* (__thiscall* OriginalFn)(PVOID); //Anything inside a VTable is a __thiscall unless it completly disregards the thisptr. You can also call them as __stdcalls, but you won't have access to the __thisptr.
			return VMT::VMTHookManager::GetFunction<OriginalFn>(this, 8)(this); //Return the pointer to the head CClientClass.
		}

		void force_update()
		{
			*reinterpret_cast<int*>(std::uintptr_t(this) + 0x174) = -1;
		};
	};
}
