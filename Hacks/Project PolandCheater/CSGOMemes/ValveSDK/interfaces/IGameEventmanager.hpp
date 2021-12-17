#pragma once

#include <cstdint>

#define EVENT_DEBUG_ID_INIT 42 
#define EVENT_DEBUG_ID_SHUTDOWN 13  

class bf_write;
class bf_read;


class IGameEvent {
public:
    const char* GetName() {
        return CallVFunction<const char*(__thiscall *)(void*)>(this, 1)(this);
    }

    int GetInt(const char* szKeyName, int nDefault = 0) {
        return CallVFunction<int(__thiscall *)(void*, const char*, int)>(this, 6)(this, szKeyName, nDefault);
    }

    const char* GetString(const char* szKeyName) {
        return CallVFunction<const char*(__thiscall *)(void*, const char*, int)>(this, 9)(this, szKeyName, 0);
    }

    void SetString(const char* szKeyName, const char* szValue) {
        return CallVFunction<void(__thiscall *)(void*, const char*, const char*)>(this, 16)(this, szKeyName, szValue);
    }
};

class IGameEventListener2 {
public:
    virtual	~IGameEventListener2(void) {};

    // FireEvent is called by EventManager if event just occured
    // KeyValue memory will be freed by manager if not needed anymore
    virtual void FireGameEvent(IGameEvent *event) = 0;

    virtual int	 GetEventDebugID(void) = 0;
};

class IGameEventManager2 {
public:
    virtual	~IGameEventManager2(void) {};

    // load game event descriptions from a file eg "resource\gameevents.res"
    virtual int LoadEventsFromFile(const char *filename) = 0;

    // removes all and anything
    virtual void  Reset() = 0;

    // adds a listener for a particular event
    virtual bool AddListener(IGameEventListener2 *listener, const char *name, bool bServerSide) = 0;

    // returns true if this listener is listens to given event
    virtual bool FindListener(IGameEventListener2 *listener, const char *name) = 0;

    // removes a listener 
    virtual void RemoveListener(IGameEventListener2 *listener) = 0;

    // create an event by name, but doesn't fire it. returns NULL is event is not
    // known or no listener is registered for it. bForce forces the creation even if no listener is active
    virtual IGameEvent *CreateEvent(const char *name, bool bForce = false, int *pCookie = NULL) = 0;

    // fires a server event created earlier, if bDontBroadcast is set, event is not send to clients
    virtual bool FireEvent(IGameEvent *event, bool bDontBroadcast = false) = 0;

    // fires an event for the local client only, should be used only by client code
    virtual bool FireEventClientSide(IGameEvent *event) = 0;

    // create a new copy of this event, must be free later
    virtual IGameEvent *DuplicateEvent(IGameEvent *event) = 0;

    // if an event was created but not fired for some reason, it has to bee freed, same UnserializeEvent
    virtual void FreeEvent(IGameEvent *event) = 0;

    // write/read event to/from bitbuffer
    virtual bool SerializeEvent(IGameEvent *event, bf_write *buf) = 0;
    virtual IGameEvent *UnserializeEvent(bf_read *buf) = 0; // create new KeyValues, must be deleted
};