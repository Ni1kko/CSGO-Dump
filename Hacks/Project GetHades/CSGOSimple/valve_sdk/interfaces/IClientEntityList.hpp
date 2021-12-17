#pragma once
#include "../misc/IHandleEntity.hpp"
#include "../misc/UtlVector.hpp"

class IClientNetworkable;
class IClientEntity;

class IClientEntityListener
{
public:
    virtual void on_entity_created ( C_BaseEntity* entity )
    {
    }

    virtual void on_entity_deleted ( C_BaseEntity* entity )
    {
    }
};

class IClientEntityList
{
public:
    virtual IClientNetworkable* get_client_networkable ( int entnum ) = 0;
    virtual void* vtablepad0x1 ( void ) = 0;
    virtual void* vtablepad0x2 ( void ) = 0;
    virtual IClientEntity* get_client_entity ( int entNum ) = 0;
    virtual IClientEntity* get_client_entity_from_handle ( CBaseHandle hEnt ) = 0;
    virtual int number_of_entities ( bool bIncludeNonNetworkable ) = 0;
    virtual int get_highest_entity_index ( void ) = 0;
    virtual void set_max_entities ( int maxEnts ) = 0;
    virtual int get_max_entities ( ) = 0;

    void add_listener_entity ( IClientEntityListener* pListener )
    {
        m_entityListeners.AddToTail ( pListener );
    }

    void remove_listener_entity ( IClientEntityListener* pListener )
    {
        m_entityListeners.FindAndRemove ( pListener );
    }

private:
    CUtlVector< IClientEntityListener* > m_entityListeners;
};
