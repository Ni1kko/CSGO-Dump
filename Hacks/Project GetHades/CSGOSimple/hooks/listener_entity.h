#pragma once
#include "../valve_sdk/csgostructs.hpp"

struct container_t
{
    //std::unique_ptr<c_vmt> m_vmt;
    vfunc_hook vmt;
    int m_index;
    bool m_hooked;
};

class c_listener_entity : public IClientEntityListener
{
public:
    void on_entity_created ( C_BaseEntity* ent ) override;
    void on_entity_deleted ( C_BaseEntity* ent ) override;

    bool init ( );
    void remove ( );

    std::array< container_t, 64 > m_track;
private:
    void hook_entity ( C_BaseEntity* ent );
    void hook_weapon ( C_BaseEntity* ent );
};

extern c_listener_entity g_listener_entity;
