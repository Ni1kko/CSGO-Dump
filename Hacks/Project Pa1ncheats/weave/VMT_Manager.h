#pragma once
#include "DLL_MAIN.h"
#include <assert.h>
typedef DWORD** PPDWORD;
class VFTableHook
{
	VFTableHook(const VFTableHook&) = delete;
public:
	VFTableHook(void* ppClass)
	{
		m_ppClassBase = (PPDWORD)ppClass;
		m_pOriginalVMTable = *(PPDWORD)ppClass;
		uint32_t dwLength = CalculateLength();

		m_pNewVMTable = new DWORD[dwLength];
		memcpy(m_pNewVMTable, m_pOriginalVMTable, dwLength * sizeof(DWORD));

		DWORD old;
		VirtualProtect(m_ppClassBase, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
		*m_ppClassBase = m_pNewVMTable;
		VirtualProtect(m_ppClassBase, sizeof(DWORD), old, &old);
	}
	~VFTableHook()
	{
		RestoreTable();
		delete[] m_pNewVMTable;
	}

	void RestoreTable()
	{
		DWORD old;
		VirtualProtect(m_ppClassBase, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
		*m_ppClassBase = m_pOriginalVMTable;
		VirtualProtect(m_ppClassBase, sizeof(DWORD), old, &old);
	}

	template<class Type>
	Type Hook(uint32_t index, Type fnNew)
	{
		DWORD dwOld = (DWORD)m_pOriginalVMTable[index];
		m_pNewVMTable[index] = (DWORD)fnNew;
		m_vecHookedIndexes.insert(std::make_pair(index, (DWORD)dwOld));
		return (Type)dwOld;
	}

	void Unhook(uint32_t index)
	{
		auto it = m_vecHookedIndexes.find(index);
		if (it != m_vecHookedIndexes.end()) {
			m_pNewVMTable[index] = (DWORD)it->second;
			m_vecHookedIndexes.erase(it);
		}
	}

	template<class Type>
	Type GetOriginal(uint32_t index)
	{
		return (Type)m_pOriginalVMTable[index];
	}

private:
	uint32_t CalculateLength()
	{
		uint32_t dwIndex = 0;
		if (!m_pOriginalVMTable) return 0;
		for (dwIndex = 0; m_pOriginalVMTable[dwIndex]; dwIndex++)
		{
			if (IsBadCodePtr((FARPROC)m_pOriginalVMTable[dwIndex]))
			{
				break;
			}
		}
		return dwIndex;
	}

private:
	std::map<uint32_t, DWORD> m_vecHookedIndexes;

	PPDWORD m_ppClassBase;
	PDWORD m_pOriginalVMTable;
	PDWORD m_pNewVMTable;
};
#pragma endregion

typedef DWORD** PPDWORD;



class ProtectGuard
{
public:

	ProtectGuard(void *base, uint32_t len, uint32_t protect)
	{
		this->base = base;
		this->len = len;

		if (!VirtualProtect(base, len, protect, (PDWORD)&old_protect))
			throw std::runtime_error("Failed to protect region!");
	}

	~ProtectGuard()
	{
		VirtualProtect(base, len, old_protect, (PDWORD)&old_protect);
	}

private:

	void *base;
	uint32_t len;
	uint32_t old_protect;
};

class VMTHook
{
public:
	bool hooked;
	uintptr_t* shadow_vtable;
	VMTHook() : class_base(nullptr), method_count(0), shadow_vtable(nullptr), original_vtable(nullptr) {}
	VMTHook(void *base) : class_base(base), method_count(0), shadow_vtable(nullptr), original_vtable(nullptr) {}
	~VMTHook()
	{
		RestoreTable();

		delete[] shadow_vtable;
	}

	inline void Setup(void *base = nullptr)
	{
		if (base != nullptr)
			class_base = base;

		if (class_base == nullptr)
			return;

		original_vtable = *(uintptr_t**)class_base;
		method_count = GetMethodCount(original_vtable);

		if (method_count == 0)
			return;

		shadow_vtable = new uintptr_t[method_count + 1]();

		shadow_vtable[0] = original_vtable[-1];
		std::memcpy(&shadow_vtable[1], original_vtable, method_count * sizeof(uintptr_t));

		try
		{
			auto guard = ProtectGuard{ class_base, sizeof(uintptr_t), PAGE_READWRITE };
			*(uintptr_t**)class_base = &shadow_vtable[1];
		}
		catch (...)
		{
			delete[] shadow_vtable;
		}
	}

	template<typename T>
	inline void Hook(uint32_t index, T method)
	{
		assert(index < method_count);
		shadow_vtable[index + 1] = reinterpret_cast<uintptr_t>(method);
		hooked = true;
	}

	inline void Unhook(uint32_t index)
	{
		assert(index < method_count);
		shadow_vtable[index + 1] = original_vtable[index];
		hooked = false;
	}

	template<typename T>
	inline T GetOriginal(uint32_t index)
	{
		return (T)original_vtable[index];
	}

	inline void RestoreTable()
	{
		try
		{
			if (original_vtable != nullptr)
			{
				auto guard = ProtectGuard{ class_base, sizeof(uintptr_t), PAGE_READWRITE };
				*(uintptr_t**)class_base = original_vtable;
				original_vtable = nullptr;
			}
		}
		catch (...) {}
	}

private:

	inline uint32_t GetMethodCount(uintptr_t *vtable_start)
	{
		uint32_t len = -1;

		do ++len; while (vtable_start[len]);

		return len;
	}

	void *class_base;
	uint32_t method_count;
	uintptr_t *original_vtable;
};