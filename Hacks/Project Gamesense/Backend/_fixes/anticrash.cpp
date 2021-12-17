#include "anticrash.h"

extern LONG WINAPI My_UnhandledExceptionFilter(PEXCEPTION_POINTERS ExceptionInfo)
{
    printf("ExceptionCode: 0x%X\nEip: %p\n", (UINT)ExceptionInfo->ExceptionRecord->ExceptionCode, (PVOID)ExceptionInfo->ContextRecord->Eip);
    MEMORY_BASIC_INFORMATION mbi;
    ZeroMemory(&mbi, sizeof(mbi));
    VirtualQuery((LPCVOID)ExceptionInfo->ContextRecord->Eip, &mbi, sizeof(mbi));
    if (mbi.Protect == PAGE_EXECUTE_READ)
        ExceptionInfo->ContextRecord->Eip += length_disasm((PVOID)ExceptionInfo->ContextRecord->Eip); // or 3
    else
    {
        ZeroMemory(&mbi, sizeof(mbi));
        VirtualQuery((LPCVOID)ExceptionInfo->ContextRecord->Esp, &mbi, sizeof(mbi));
        UINT_PTR max_p_ret_addr_offset = (UINT_PTR)mbi.BaseAddress + mbi.RegionSize - (UINT_PTR)ExceptionInfo->ContextRecord->Esp;
        for (UINT_PTR p_ret_addr_offset = 0; p_ret_addr_offset < max_p_ret_addr_offset; p_ret_addr_offset += sizeof(UINT_PTR))
        {
            UINT_PTR ret_addr = *(PUINT_PTR)(ExceptionInfo->ContextRecord->Esp + p_ret_addr_offset);
            ZeroMemory(&mbi, sizeof(mbi));
            VirtualQuery((LPCVOID)ret_addr, &mbi, sizeof(mbi));
            if (mbi.Protect == PAGE_EXECUTE_READ)
            {
                ExceptionInfo->ContextRecord->Eip = ret_addr;
                break;
            }
        }
    }
    return EXCEPTION_CONTINUE_EXECUTION;
}