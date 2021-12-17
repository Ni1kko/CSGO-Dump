#pragma once

extern "C"
{
	#include "./lend/ld32.h"
}
#include <inttypes.h>
#include <stdio.h>
#include <windows.h>

extern LONG WINAPI My_UnhandledExceptionFilter(PEXCEPTION_POINTERS ExceptionInfo);