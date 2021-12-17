#pragma once

#include "sdk.h"

#define VirtualFn( cast )typedef cast( __thiscall* OriginalFn )
class IGameEvent
{
public:
	std::string GetName( void )
	{
		VirtualFn( const char* )( PVOID );
		return std::string( getvfunc< OriginalFn >( this, 1 )( this ) );
	}
	bool GetBool( std::string strKeyName, bool bDefaultValue = false )
	{
		VirtualFn( bool )( PVOID, const char*, bool );
		const char* c = strKeyName.c_str();
		return getvfunc< OriginalFn >( this, 5 )( this, c, bDefaultValue );
	}
	int GetInt( std::string strKeyName, int iDefaultValue = 0 )
	{
		VirtualFn( int )( PVOID, const char*, int );
		const char* c = strKeyName.c_str();
		return getvfunc< OriginalFn >( this, 6 )( this, c, iDefaultValue );
	}
	float GetFloat( std::string strKeyName, float iDefaultValue = 0.f )
	{
		VirtualFn( float )( PVOID, const char*, float );
		const char* c = strKeyName.c_str();
		return getvfunc< OriginalFn >( this, 8 )( this, c, iDefaultValue );
	}
	const char* GetString( const char* keyname = nullptr, const char* default_value = "" )
	{
		VirtualFn( const char* )( PVOID, const char*, const char* );
		return getvfunc< OriginalFn >( this, 9 )( this, keyname, default_value );
	}
};

class IGameEventListener2
{
public:
	virtual ~IGameEventListener2( void ){}
	virtual void FireGameEvent( IGameEvent* pGameEvent ) = 0;
	virtual int GetEventDebugID( void )
	{
		return iDebug;
	}

public:
	int	iDebug;
};

class IGameEventManager2
{
public:
	bool AddListener( IGameEventListener2* pListener, std::string strName, bool bServerside )
	{
		VirtualFn( bool )( PVOID, IGameEventListener2*, const char*, bool );
		const char* c = strName.c_str();
		return getvfunc< OriginalFn >( this, 3 )( this, pListener, c, bServerside );
	}
	bool FireEventClientSide( IGameEvent* pGameEvent )
	{
		VirtualFn( bool )( PVOID, IGameEvent* );
		return getvfunc< OriginalFn >( this, 8 )( this, pGameEvent );
	}
};
extern IGameEventManager2* g_pGameEventManager2;

class CEvents : public IGameEventListener2
{
public:
	CEvents();
	void FireGameEvent( IGameEvent* pEvent );
	void Registerself();
	bool bHitted, bNewMap, bHit, bRoundStart, bNewRound;
	float flHitTime, flHurtTime, flImpactTime;

	bool bGetsPlanted, bAbortedPlant, bDefused, bResetBomb, bExploded, bDefusing, bUsingKit;
	Vector vImpact;
};

extern CEvents* g_pEvents;

typedef bool( __thiscall *FireEventClientSide_t )( void*, IGameEvent* );
extern FireEventClientSide_t oFireEventClientSide;

bool __fastcall HOOKED_FireEventClientSide( void* thisptr, void* edx, IGameEvent* pEvent );

void __fastcall HOOKED_FireEvent( void* ecx, void* edx );