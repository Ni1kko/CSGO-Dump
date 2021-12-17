#pragma once
#include "..\Utils\Utils.h"

class ConVar
{
public:
	char pad_0x0000[0x4];
	ConVar* pNext;
	int32_t bRegistered;
	char* pszName;
	char* pszHelpString;
	int32_t nFlags;
	char pad_0x0018[0x4];
	ConVar* pParent;
	char* pszDefaultValue;
	char* strString;
	int32_t StringLength;
	float fValue;
	int32_t nValue;
	int32_t bHasMin;
	float fMinVal;
	int32_t bHasMax;
	float fMaxVal;
	void* fnChangeCallback;

	float GetFloat(void) const
	{
		auto temp = *(int*)(&fValue);
		auto temp_result = (int)(temp ^ (DWORD)this);

		return *(float*)(&temp_result);
	}

	int GetInt(void) const
	{
		auto temp = *(int*)(&fValue);
		auto temp_result = (int)(temp ^ (DWORD)this);

		return *(int*)(&temp_result);
	}

	bool GetBool(void) const
	{
		return !!GetInt();
	}

	void SetValue(int value)
	{
		typedef void(__thiscall * SetValue_t)(void*, int);
		return Utils::GetVFunc<SetValue_t>(this, 16)(this, value);
	}

	void SetValue(const char* value)
	{
		return Utils::CallVFunc<14, void>(this, value);
	}
};//Size=0x0048

// For calling VMT functions in our classes
template< typename Function > Function call_vfunc4(PVOID Base, DWORD Index)
{
	PDWORD* VTablePointer = (PDWORD*)Base;
	PDWORD VTableFunctionBase = *VTablePointer;
	DWORD dwAddress = VTableFunctionBase[Index];
	return (Function)(dwAddress);
}

class IAppSystem
{
public:
	virtual ~IAppSystem()
	{
	}

	virtual void func0() = 0;
	virtual void func1() = 0;
	virtual void func2() = 0;
	virtual void func3() = 0;
	virtual void func4() = 0;
	virtual void func5() = 0;
	virtual void func6() = 0;
	virtual void func7() = 0;
	virtual void func8() = 0;
	virtual void func9() = 0;
};

class ICVar : public IAppSystem
{
public:
	virtual void			func10() = 0;
	virtual void			RegisterConCommand(ConVar *pCommandBase) = 0;
	virtual void			UnregisterConCommand(ConVar *pCommandBase) = 0;
	virtual void			func13() = 0;
	virtual ConVar			*FindVar(const char *var_name) = 0;
	//	virtual void            ConsoleColorPrintf(const Color& clr, const char *pFormat, ...) const = 0;
	virtual void			func15() = 0;
	virtual void			func16() = 0;
	virtual void			func17() = 0;
	virtual void			func18() = 0;
	virtual void			func19() = 0;
	virtual void			func20() = 0;

	void const ConsoleColorPrintf(const Color2& clr, const char *pFormat, ...)
	{
		typedef void(__cdecl *OriginalFn)(void*, const Color2&, const char *, ...);

		if (pFormat == nullptr)
			return;

		char buf[8192];

		va_list list;
		va_start(list, pFormat);
		_vsnprintf_s(buf, sizeof(buf) - 1, pFormat, list);
		va_end(list);
		buf[sizeof(buf) - 1] = 0;

		call_vfunc4<OriginalFn>(this, 25)(this, clr, pFormat);
	}
};

extern ICVar* g_pConVar;