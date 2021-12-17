#pragma once
inline int UtlMemory_CalcNewAllocationCount(int nAllocationCount, int nGrowSize, int nNewSize, int nBytesItem)
{
	if (nGrowSize)
		nAllocationCount = ((1 + ((nNewSize - 1) / nGrowSize)) * nGrowSize);
	else
	{
		if (!nAllocationCount)
			nAllocationCount = (31 + nBytesItem) / nBytesItem;

		while (nAllocationCount < nNewSize)
			nAllocationCount *= 2;
	}

	return nAllocationCount;
}

template< class T, class I = int >
class CUtlMemory
{
public:
	T& operator[](I i)
	{
		return m_pMemory[i];
	}

	T* Base()
	{
		return m_pMemory;
	}

	int NumAllocated() const
	{
		return m_nAllocationCount;
	}

	void Grow(int num = 1)
	{
		if (IsExternallyAllocated())
			return;

		int nAllocationRequested = m_nAllocationCount + num;
		int nNewAllocationCount = UtlMemory_CalcNewAllocationCount(m_nAllocationCount, m_nGrowSize, nAllocationRequested, sizeof(T));

		if ((int)(I)nNewAllocationCount < nAllocationRequested)
		{
			if ((int)(I)nNewAllocationCount == 0 && (int)(I)(nNewAllocationCount - 1) >= nAllocationRequested)
				--nNewAllocationCount;
			else
			{
				if ((int)(I)nAllocationRequested != nAllocationRequested)
					return;

				while ((int)(I)nNewAllocationCount < nAllocationRequested)
					nNewAllocationCount = (nNewAllocationCount + nAllocationRequested) / 2;
			}
		}

		m_nAllocationCount = nNewAllocationCount;

		if (m_pMemory)
			m_pMemory = (T*)realloc(m_pMemory, m_nAllocationCount * sizeof(T));
		else
			m_pMemory = (T*)malloc(m_nAllocationCount * sizeof(T));
	}

	bool IsExternallyAllocated() const
	{
		return m_nGrowSize < 0;
	}

protected:
	T* m_pMemory;
	int m_nAllocationCount;
	int m_nGrowSize;
};

template <class T>
inline T* Construct(T* pMemory)
{
	return ::new(pMemory) T;
}

template <class T>
inline void Destruct(T* pMemory)
{
	pMemory->~T();
}

template< class T, class A = CUtlMemory<T> >
class CUtlVector
{
	typedef A CAllocator;
public:
	CAllocator m_Memory;
	int m_Size;

	T& operator[](int i)
	{
		return m_Memory[i];
	}

	T& Element(int i)
	{
		return m_Memory[i];
	}

	T* Base()
	{
		return m_Memory.Base();
	}

	int Count() const
	{
		return m_Size;
	}

	void RemoveAll()
	{
		for (int i = m_Size; --i >= 0; )
			Destruct(&Element(i));

		m_Size = 0;
	}

	int AddToTail()
	{
		return InsertBefore(m_Size);
	}

	int InsertBefore(int elem)
	{
		GrowVector();
		ShiftElementsRight(elem);
		Construct(&Element(elem));

		return elem;
	}

protected:
	void GrowVector(int num = 1)
	{
		if (m_Size + num > m_Memory.NumAllocated())
			m_Memory.Grow(m_Size + num - m_Memory.NumAllocated());

		m_Size += num;
		ResetDbgInfo();
	}

	void ShiftElementsRight(int elem, int num = 1)
	{
		int numToMove = m_Size - elem - num;
		if ((numToMove > 0) && (num > 0))
			memmove(&Element(elem + num), &Element(elem), numToMove * sizeof(T));
	}

	T* m_pElements;

	inline void ResetDbgInfo()
	{
		m_pElements = Base();
	}
};
enum EConVar
{
	ConVar_GetName = 5,
	ConVar_GetBaseName = 6,
	ConVar_GetFloat = 12,
	ConVar_GetInt = 13,
	ConVar_SetString = 14,
	ConVar_SetFloat,
	ConVar_SetInt
};

enum EICVar
{
	ICVar_FindVar = 15
};
typedef void(*FnChangeCallbackV1_t)(void);
typedef void(*FnChangeCallback_t)(void* var, const char *pOldValue, float flOldValue);

struct CVValue_t
{
	char* m_pszString;
	int m_StringLength;

	// Values
	float m_fValue;
	int m_nValue;
};





class ConVar
{
public:
	const char* GetString()
	{
		typedef const char*(__thiscall* GetString_t)(void*);
		return getvfunc<GetString_t>(this, 11)(this);
	}

	float GetFloat()
	{
		typedef float(__thiscall* GetFloat_t)(void*);
		return getvfunc <GetFloat_t>(this, 12)(this);
	}

	int GetInt()
	{
		typedef int(__thiscall* GetInt_t)(void*);
		return getvfunc <GetInt_t>(this, 13)(this);
	}

	void SetValue(const char *value)
	{
		typedef void(__thiscall* SetValue_t)(void*, const char*);
		return getvfunc<SetValue_t>(this, 14)(this, value);
	}

	void SetValue(float value)
	{
		typedef void(__thiscall* SetValue_t)(void*, float);
		return getvfunc<SetValue_t>(this, 15)(this, value);
	}

	void SetValue(int value)
	{
		typedef void(__thiscall* SetValue_t)(void*, int);
		return getvfunc<SetValue_t>(this, 16)(this, value);
	}

	/*void SetValue(color_t value)
	{
	typedef void(__thiscall* SetValue_t)(void*, color_t);
	return getvfunc<SetValue_t>(this, 17)(this, value);
	}
	*/


	char* GetName()
	{
		typedef char*(__thiscall* OriginalFn)(void*);
		return getvfunc<OriginalFn>(this, 5)(this);
	}


	void* virtualtable;
	ConVar* m_pNext;
	int m_bRegistered;
	char* m_pszName;
	char* m_pszHelpString;
	int m_nFlags;
	FnChangeCallbackV1_t m_fnChangeCallbacksV1;
	ConVar* m_pParent;
	char* m_pszDefaultValue;
	CVValue_t m_Value;
	int m_bHasMin;
	float m_fMinVal;
	int m_bHasMax;
	float m_fMaxVal;
	CUtlVector<FnChangeCallback_t> m_fnChangeCallbacks; // note: this is also accessible as FnChangeCallback_t* instead of CUtlVector
	float GetValue()
	{
		DWORD xored = *(DWORD*)&m_pParent->m_Value.m_fValue ^ (DWORD)this;
		return *(float*)&xored;
	}
	int GetValueN()
	{
		return (int)(m_pParent->m_Value.m_nValue ^ (DWORD)this);
	}
	bool GetBool() { return !!GetInt(); }
};//Size=0x0048*

class IAppSystem
{
public:
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

class color_t;

struct CVarDLLIdentifier_t;
class ConCommandBase
{
public:
	PVOID pVMTBase; // 0x00

	ConCommandBase *m_pNext; // 0x04
	bool m_bRegistered; // 0x08
	const char *m_pszName; // 0x0C
	const char *m_pszHelpString; // 0x10
	int m_nFlags; // 0x14

	ConCommandBase *s_pConCommandBases; // 0x18
	void* s_pAccessor; // 0x1C
}; // Size: 0x0020
class ConCommand;

class IConsoleDisplayFunc
{
public:
	virtual void color_tPrint(const uint8_t* clr, const char *pMessage) = 0;
	virtual void Print(const char *pMessage) = 0;
	virtual void DPrint(const char *pMessage) = 0;
};

class ICVar
{
public:
	template <typename... Values>
	void Consolecolor_tPrintf(const color_t& Msgcolor_t, const char* szMsgFormat, Values... Parameters)
	{
		typedef void(*oConsolecolor_tPrintf)(void*, const color_t&, const char*, ...);
		return getvfunc<oConsolecolor_tPrintf>(this, 25)(this, Msgcolor_t, szMsgFormat, Parameters...);
	}

	void UnregisterConCommand(ConVar* pCommandBase)
	{
		typedef ConVar*(__thiscall* FindVarFn)(void*, ConVar*);
		getvfunc< FindVarFn >(this, 13)(this, pCommandBase);
	}
	ConVar * FindVar(const char* var_name)
	{
		typedef ConVar*(__thiscall* FindVarFn)(void*, const char*);
		return getvfunc< FindVarFn >(this, 16)(this, var_name);
	}

	void RegisterConCommand(ConVar* pCommandBase)
	{
		typedef ConVar*(__thiscall* FindVarFn)(void*, ConVar*);
		getvfunc< FindVarFn >(this, 12)(this, pCommandBase);
	}
};
