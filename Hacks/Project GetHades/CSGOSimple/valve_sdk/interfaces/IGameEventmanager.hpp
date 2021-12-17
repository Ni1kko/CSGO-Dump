#pragma once

#include <cstdint>

#define EVENT_DEBUG_ID_INIT 42
#define EVENT_DEBUG_ID_SHUTDOWN 13

class bf_write;
class bf_read;

class IGameEvent
{
public:
    virtual ~IGameEvent ( ) = 0;
    virtual const char* GetName ( ) const = 0;

    virtual bool IsReliable ( ) const = 0;
    virtual bool IsLocal ( ) const = 0;
    virtual bool IsEmpty ( const char* keyName = nullptr ) = 0;

    virtual bool GetBool ( const char* keyName = nullptr, bool defaultValue = false ) = 0;
    virtual int GetInt ( const char* keyName = nullptr, int defaultValue = 0 ) = 0;
    virtual uint64_t GetUint64 ( const char* keyName = nullptr, unsigned long defaultValue = 0 ) = 0;
    virtual float GetFloat ( const char* keyName = nullptr, float defaultValue = 0.0f ) = 0;
    virtual const char* GetString ( const char* keyName = nullptr, const char* defaultValue = "" ) = 0;
    virtual const wchar_t* GetWString ( const char* keyName, const wchar_t* defaultValue = L"" ) = 0;

    virtual void SetBool ( const char* keyName, bool value ) = 0;
    virtual void SetInt ( const char* keyName, int value ) = 0;
    virtual void SetUint64 ( const char* keyName, unsigned long value ) = 0;
    virtual void SetFloat ( const char* keyName, float value ) = 0;
    virtual void SetString ( const char* keyName, const char* value ) = 0;
    virtual void SetWString ( const char* keyName, const wchar_t* value ) = 0;
};

class IGameEventListener2
{
public:
    virtual ~IGameEventListener2 ( void )
    {
    }

    virtual void FireGameEvent ( IGameEvent* event ) = 0;
    virtual int GetEventDebugID ( void ) = 0;

public:
    int m_iDebugId;
};

class IGameEventManager2
{
public:
    virtual ~IGameEventManager2 ( ) = 0;
    virtual int load_events_from_file ( const char* filename ) = 0;
    virtual void reset ( ) = 0;
    virtual bool add_listener ( IGameEventListener2* listener, const char* name, bool bServerSide ) = 0;
    virtual bool find_listener ( IGameEventListener2* listener, const char* name ) = 0;
    virtual int remove_listener ( IGameEventListener2* listener ) = 0;
    virtual IGameEvent* create_event ( const char* name, bool bForce, unsigned int dwUnknown ) = 0;
    virtual bool fire_event ( IGameEvent* event, bool bDontBroadcast = false ) = 0;
    virtual bool fire_event_client_side ( IGameEvent* event ) = 0;
    virtual IGameEvent* duplicate_event ( IGameEvent* event ) = 0;
    virtual void free_event ( IGameEvent* event ) = 0;
    virtual bool serialize_event ( IGameEvent* event, bf_write* buf ) = 0;
    virtual IGameEvent* unserialize_event ( bf_read* buf ) = 0;
};
