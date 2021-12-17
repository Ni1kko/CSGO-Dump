#pragma once

#include "Recv.hpp"

class ClientClass;
class IClientNetworkable;

typedef IClientNetworkable* ( *CreateClientClassFn ) ( int entnum, int serial_num );
typedef IClientNetworkable* ( *CreateEventFn ) ( );

class ClientClass
{
public:
    CreateClientClassFn create_fn;
    CreateEventFn create_event_fn;
    char* network_name;
    RecvTable* recv_table;
    ClientClass* p_next;
    class_id_t class_id;
};
