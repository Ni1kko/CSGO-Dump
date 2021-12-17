#pragma once
#include "../../Include/GlobalIncludes.h"
#include "../../Hashing/xorstr.h"
#include "../../Utility/Utilities.h"
#include <assert.h>

namespace Cheat
{
	namespace Hooking
	{
		class VMTHook
		{
		protected:
			inline uint32_t GetMethodCount(uintptr_t* vtable_start)
			{
				uint32_t len = -1;

				do ++len; while (vtable_start[len]);

				return len;
			}

			uint32_t method_count;
			uintptr_t original_backup[2000]{};
			uintptr_t* original_vtable;
			size_t original_vtable_size;
		public:
			VMTHook() : method_count(0), original_vtable(nullptr), original_vtable_size(0)
			{
			}
			VMTHook(void* base) : method_count(0), original_vtable(nullptr), original_vtable_size(0)
			{
				assert(base != nullptr);
				original_vtable = *reinterpret_cast<uintptr_t**>(base);
				method_count = GetMethodCount(original_vtable);
				assert(method_count != 0);
				original_vtable_size = method_count * sizeof(uintptr_t);
				assert(sizeof(original_backup) >= original_vtable_size);
				memcpy(original_backup, original_vtable, original_vtable_size);
			}

			template<typename T>
			inline void Hook(uint32_t index, T method)
			{
				assert(index < method_count);
				DWORD old_protection;
				VirtualProtect(&original_vtable[index], sizeof(uintptr_t), PAGE_READWRITE, &old_protection);
				original_vtable[index] = reinterpret_cast<uintptr_t>(method);
				VirtualProtect(&original_vtable[index], sizeof(uintptr_t), old_protection, &old_protection);
			}

			inline void Unhook(uint32_t index)
			{
				assert(index < method_count);
				DWORD old_protection;
				VirtualProtect(&original_vtable[index], sizeof(uintptr_t), PAGE_READWRITE, &old_protection);
				original_vtable[index] = original_backup[index];
				VirtualProtect(&original_vtable[index], sizeof(uintptr_t), old_protection, &old_protection);
			}

			template<typename T>
			inline T GetOriginal(uint32_t index)
			{
				return (T)original_backup[index];
			}

			inline void RestoreTable()
			{
				DWORD old_protection;
				VirtualProtect(original_vtable, original_vtable_size, PAGE_READWRITE, &old_protection);
				memcpy(original_vtable, original_backup, original_vtable_size);
				VirtualProtect(original_vtable, original_vtable_size, old_protection, &old_protection);
			}
		};
	}
}
