#include "Detour.h"
#include "HDE.h"

uint8_t GetFunctionLengthForTrampoline(DWORD Addr)
{
	hde32s disasm;
	uint8_t offset = 0;

	do
	{
		hde32_disasm((void*)(Addr + offset), &disasm);
		offset += disasm.len;
	} while (offset < 5);

	return offset;
}

template<typename retn>
inline retn AddTrampoline(BYTE * pSource, BYTE * pDestination, uint8_t len)
{
	DWORD dwProt = 0;
	BYTE *jmp = (BYTE*)malloc(len + 5);
	VirtualProtect(jmp, len + 5, PAGE_EXECUTE_READWRITE, &dwProt);
	memcpy(jmp, pSource, len);
	jmp += len;
	jmp[0] = 0xE9;
	*(DWORD*)(jmp + 1) = (DWORD)(pSource + len - jmp) - 5;

	VirtualProtect(pSource, len, PAGE_EXECUTE_READWRITE, &dwProt);
	memset(pSource, 0x90, len);
	pSource[0] = 0xE9;
	*(DWORD*)(pSource + 1) = (DWORD)(pDestination - pSource) - 5;
	VirtualProtect(pSource, len, dwProt, 0);

	return (retn)(jmp - len);
}

bool Detour::SetupHook(uint8_t * address_to_hook, BYTE * destination)
{
	this->m_length = GetFunctionLengthForTrampoline((DWORD)address_to_hook);
	this->m_orignal_bytes = new char[this->m_length];
	this->p_original_address = reinterpret_cast<void*>(address_to_hook);

	memcpy(this->m_orignal_bytes, this->p_original_address, this->Length());

	this->p_original = AddTrampoline<uintptr_t*>(reinterpret_cast<BYTE*>(this->p_original_address), destination, this->Length());

	return this->p_original != nullptr;
}
