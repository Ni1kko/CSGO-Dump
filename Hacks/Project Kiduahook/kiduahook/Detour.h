#pragma once

#include <cstdint>
#include <Windows.h>

class Detour
{
public:
	Detour() : p_original_address(0), m_length(0), p_original(0), m_orignal_bytes(0) { }
	~Detour() { delete[] this->m_orignal_bytes; }

	uint8_t Length() { return this->m_length; }

	bool SetupHook(uint8_t* address_to_hook, BYTE* destination);

	bool Detach()
	{
		unsigned long old_protection = 0;

		VirtualProtect(this->p_original_address, this->Length(), PAGE_EXECUTE_READWRITE, &old_protection);
		memcpy(this->p_original_address, this->m_orignal_bytes, this->Length());
		VirtualProtect(this->p_original_address, this->Length(), old_protection, &old_protection);
		return true;
	}

	template < typename retn >
	retn GetOriginal()
	{
		return (retn)(this->p_original);
	};

private:
	void* p_original_address;
	uint8_t m_length;
	uintptr_t* p_original;
	char* m_orignal_bytes;
};