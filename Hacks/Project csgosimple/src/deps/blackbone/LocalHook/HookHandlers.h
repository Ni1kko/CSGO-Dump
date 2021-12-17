#pragma once

#include "LocalHookBase.h"
#define BLACKBONE_API

namespace blackbone
{
    class BLACKBONE_API NoClass { };

    template<typename Fn, class C>
    struct HookHandler;

}

template<typename _Tgt, typename _Src>
inline _Tgt brutal_cast(const _Src& src)
{
	static_assert(sizeof(_Tgt) == sizeof(_Src), "Operand size mismatch");
	union _u { _Src s; _Tgt t; } u;
	u.s = src;
	return u.t;
}

#include "HookHandlerCdecl.h"

#ifndef USE64
#include "HookHandlerStdcall.h"
#include "HookHandlerThiscall.h"
#include "HookHandlerFastcall.h"
#endif