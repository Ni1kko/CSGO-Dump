#pragma once

#include "../Misc/IHandleEntity.hpp"
#include "../misc/UtlVector.hpp"

class IClientNetworkable;
class IClientEntity;

class IClientEntityListener {
public:
    virtual void OnEntityCreated(C_BaseEntity* pEntity) {}
    virtual void OnEntityDeleted(C_BaseEntity* pEntity) {}
};

class IClientEntityList {
public:
    virtual IClientNetworkable*   GetClientNetworkable(int entnum) = 0;
    virtual void*                 vtablepad0x1(void) = 0;
    virtual void*                 vtablepad0x2(void) = 0;
    virtual IClientEntity*        GetClientEntity(int entNum) = 0;
    virtual IClientEntity*        GetClientEntityFromHandle(CBaseHandle hEnt) = 0;
    virtual int                   NumberOfEntities(bool bIncludeNonNetworkable) = 0;
    virtual int                   GetHighestEntityIndex(void) = 0;
    virtual void                  SetMaxEntities(int maxEnts) = 0;
    virtual int                   GetMaxEntities() = 0;

    void AddListenerEntity(IClientEntityListener* pListener) {
        m_entityListeners.AddToTail(pListener);
    }

    void RemoveListenerEntity(IClientEntityListener* pListener) {
        m_entityListeners.FindAndRemove(pListener);
    }
private:
    CUtlVector<IClientEntityListener*> m_entityListeners;
};