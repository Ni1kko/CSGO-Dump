#pragma once

#include "sdk.h"

class ConVar
{
public:
	char			 m_pad_0x0000[ 0x4 ];
	ConVar*			 m_pNext;
	__int32			 m_bRegistered;
	char*			 m_pszName;
	char*			 m_pszHelpString;
	__int32			 m_nFlags;
	char			 m_pad_0x0018[ 0x4 ];
	ConVar*			 m_pParent;
	char*			 m_pszDefaultValue;
	char*		     m_strstring;
	__int32			 m_StringLength;
	float			 m_fValue;
	__int32			 m_nValue;
	__int32			 m_bHasMin;
	float			 m_fMinVal;
	__int32			 m_bHasMax;
	float			 m_fMaxVal;
	void*			 m_fnChangeCallback;

	/*
	// Auto reconstructed from vtable block @ 0x0005FAB0
	// from "filesystem_stdio.dylib", by ida_vtables.idc
	0	ConVar::IsFlagSet(int)const
	1	ConVar::AddFlags(int)
	2	ConCommandBase::RemoveFlags(int)
	3	ConVar::GetFlags(void)const
	4	ConVar::GetName(void)const
	5	ConVar::GetHelpText(void)const
	6	ConVar::IsRegistered(void)const
	7	ConCommandBase::GetDLLIdentifier(void)const
	8	ConCommandBase::Create(char const*,char const*,int)
	9	ConVar::Init(void)
	10	ConVar::GetBaseName(void)const
	11	ConVar::GetSplitScreenPlayerslot(void)const
	12	ConVar::GetFloat(void)const
	13	ConVar::GetInt(void)const
	14	ConVar::SetValue(char const*)
	15	ConVar::SetValue(float)
	16	ConVar::SetValue(int)
	17	ConVar::SetValue(Color)
	18	ConVar::InternalSetValue(char const*)
	19	ConVar::InternalSetFloatValue(float)
	20	ConVar::InternalSetIntValue(int)
	21	ConVar::InternalSetColorValue(Color)
	22	ConVar::ClampValue(float &)
	23	ConVar::ChangeStringValue(char const*,float)
	24	ConVar::Create(char const*,char const*,int,char const*,bool,float,bool,float,void (*)(IConVar *,char const*,float))
	*/

	bool GetBool()
	{
		typedef bool( __thiscall* OriginalFn )( PVOID );
		return getvfunc< OriginalFn >( this, 11 )( this );
	}

	float GetFloat()
	{
		typedef float( __thiscall* OriginalFn )( PVOID );
		return getvfunc< OriginalFn >( this, 12 )( this );
	}

	int GetInt()
	{
		typedef int( __thiscall* OriginalFn )( PVOID );
		return getvfunc< OriginalFn >( this, 13 )( this );
	}

	void SetValue( const char* value )
	{
		typedef void( __thiscall* OriginalFn )( void*, const char* );
		return getvfunc< OriginalFn >( this, 14 )( this, value );
	}

	void SetValue( float value )
	{
		typedef void( __thiscall* OriginalFn )( void*, float );
		return getvfunc< OriginalFn >( this, 15 )( this, value );
	}

	void SetValue( int value )
	{
		typedef void( __thiscall* OriginalFn )( void*, int );
		return getvfunc< OriginalFn >( this, 16 )( this, value );
	}

	void SetValue( Color value )
	{
		typedef void( __thiscall* OriginalFn )( void*, Color );
		return getvfunc< OriginalFn >( this, 17 )( this, value );
	}

	char* GetName()
	{
		typedef char*( __thiscall* OriginalFn )( void* );
		return getvfunc< OriginalFn >( this, 5 )( this );
	}

	char* GetDefault()
	{
		return m_pszDefaultValue;
	}
};

class ICVar
{
public:
	ConVar* FindVar( const char *var )
	{
		typedef ConVar*( __thiscall* FindVarFn )( void*, const char * );
		return getvfunc<FindVarFn>( this, 15 )( this, var );
	}
	template<typename... Args>
	void const ConsoleColorPrintf( const Color& clr, const char *pFormat, Args... args )
	{
		typedef void( __cdecl *OriginalFn )( void*, const Color&, const char *, ... );
		return getvfunc<OriginalFn>( this, 25 )( this, clr, pFormat, args... );
	}
};

extern ICVar* g_pICvar;

class LobbyScreen_Scaleform
{
public:
	enum EChatMessageType
	{
		ChatMessageType_Chat = 1,
		ChatMessageType_Joined = 2,
		ChatMessageType_Left = 3,
		ChatMessageType_Kicked = 4
	};
};

class ServerMsg_CheckReservation { };

class ServerConfirmedReservationCheckCallback
{
public:
	void*   vtable; //0x0000
	char   pad_0004[ 64 ]; //0x0004
	char   m_szMapName[ 60 ]; //0x0044
	class   ServerMsg_CheckReservation** m_pCServerMsg_CheckReservation; //0x0080
	char   pad_0084[ 268 ]; //0x0084
}; //Size: 0x0190