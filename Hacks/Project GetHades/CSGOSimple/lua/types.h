#include "../valve_sdk/csgostructs.hpp"
#include "../options.h"

struct lua_vector_t
{
    lua_vector_t ( ) = default;

    lua_vector_t ( const float val_x, const float val_y, const float val_z ) : x ( val_x ), y ( val_y ), z ( val_z )
    {
    };

    lua_vector_t ( vector_t v )
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
    }

    float x = std::numeric_limits< float >::max( );
    float y = std::numeric_limits< float >::max( );
    float z = std::numeric_limits< float >::max( );

    float dot ( const lua_vector_t& v_other ) const
    {
        return x * v_other.x + y * v_other.y + z * v_other.z;
    }
};

// ReSharper disable once CppInconsistentNaming
struct lua_vector2d_t
{
    lua_vector2d_t ( ) = default;

    lua_vector2d_t ( const float val_x, const float val_y ) : x ( val_x ), y ( val_y )
    {
    };
    float x = std::numeric_limits< float >::max( );
    float y = std::numeric_limits< float >::max( );
};

struct lua_angle_t
{
    lua_angle_t ( ) = default;

    lua_angle_t ( const float val_pitch, const float val_yaw, const float val_roll ) : pitch ( val_pitch ), yaw ( val_yaw ), roll ( val_roll )
    {
    };

    lua_angle_t ( qangle_t a )
    {
        this->pitch = a.pitch;
        this->yaw = a.yaw;
        this->roll = a.roll;
    }

    float pitch = std::numeric_limits< float >::max( );
    float yaw = std::numeric_limits< float >::max( );
    float roll = std::numeric_limits< float >::max( );
};

struct lua_player_info_t
{
    lua_player_info_t ( ) = default;

    lua_player_info_t ( std::string n, int uid, std::string stid, bool gtv ) : name ( std::move ( n ) ), userid ( uid ), steamid ( std::move ( stid ) ),
                                                                               is_gotv ( gtv )
    {
    };
    std::string name = "";
    int userid = -1;
    std::string steamid = "";
    bool is_gotv = false;
};

struct lua_color_t
{
    lua_color_t ( ) = default;

    lua_color_t ( int val_r, int val_g, int val_b, int val_a ) : r ( val_r ), g ( val_g ), b ( val_b ), a ( val_a )
    {
    };

    lua_color_t ( int val_r, int val_g, int val_b ) : r ( val_r ), g ( val_g ), b ( val_b )
    {
    };

    lua_color_t ( float h )
    {
        auto rr = 255.f, gg = 255.f, bb = 255.f;

        hsv_to_rgb ( h, 1.f, 1.f, rr, gg, bb );

        this->r = static_cast< int > ( rr * 255.f );
        this->g = static_cast< int > ( gg * 255.f );
        this->b = static_cast< int > ( bb * 255.f );
    }

    int r = 255;
    int g = 255;
    int b = 255;
    int a = 255;

    void hsv_to_rgb ( float h, float s, float v, float& out_r, float& out_g, float& out_b ) const
    {
        if ( s == 0.0f )
        {
            // gray
            out_r = out_g = out_b = v;
            return;
        }

        h = std::fmod ( h, 1.0f ) / ( 60.0f / 360.0f );
        int i = ( int )h;
        float f = h - ( float )i;
        float p = v * ( 1.0f - s );
        float q = v * ( 1.0f - s * f );
        float t = v * ( 1.0f - s * ( 1.0f - f ) );

        switch ( i )
        {
        case 0:
            out_r = v;
            out_g = t;
            out_b = p;
            break;

        case 1:
            out_r = q;
            out_g = v;
            out_b = p;
            break;

        case 2:
            out_r = p;
            out_g = v;
            out_b = t;
            break;

        case 3:
            out_r = p;
            out_g = q;
            out_b = v;
            break;

        case 4:
            out_r = t;
            out_g = p;
            out_b = v;
            break;

        case 5:
        default:
            out_r = v;
            out_g = p;
            out_b = q;
            break;
        }
    }
};

// table, prop, offset
std::unordered_map< std::string, std::unordered_map< std::string, int > > netvars;

struct lua_netvar_t
{
private:
    C_BaseEntity* ent = nullptr;
    int offset{ }; // = g_netvarsys.GetOffset(table, netvar);
public:
    lua_netvar_t ( ) = default;

    lua_netvar_t ( C_BaseEntity* entity, std::string table_name, std::string prop_name ) : ent ( entity )
    {
        try
        {
            offset = netvars[ table_name ][ prop_name ];

            if ( offset == 0 )
            {
                offset = g_netvarsys.GetOffset ( table_name, prop_name );
                netvars[ table_name ][ prop_name ] = offset;
            }
        }
        catch ( ... )
        {
            offset = g_netvarsys.GetOffset ( table_name, prop_name );
            netvars[ table_name ][ prop_name ] = offset;
        }
    }

    bool is_valid ( ) const
    {
        return ! ( offset == 0 || !ent );
    }

    bool get_bool ( ) const
    {
        if ( !this->is_valid( ) )
            return false;

        return *reinterpret_cast< bool* > ( reinterpret_cast< uintptr_t > ( ent ) + offset );
    }

    int get_int ( ) const
    {
        if ( !this->is_valid( ) )
            return 0;

        return *reinterpret_cast< int* > ( reinterpret_cast< uintptr_t > ( ent ) + offset );
    }

    float get_float ( ) const
    {
        if ( !this->is_valid( ) )
            return 0.f;

        return *reinterpret_cast< float* > ( reinterpret_cast< uintptr_t > ( ent ) + offset );
    }

    lua_vector_t get_vector ( ) const
    {
        if ( !this->is_valid( ) )
            return lua_vector_t ( 0, 0, 0 );

        return *reinterpret_cast< vector_t* > ( reinterpret_cast< uintptr_t > ( ent ) + offset );
    }

    lua_angle_t get_angle ( ) const
    {
        if ( !this->is_valid( ) )
            return lua_angle_t ( 0, 0, 0 );

        return *reinterpret_cast< qangle_t* > ( reinterpret_cast< uintptr_t > ( ent ) + offset );
    }

    void set_bool ( bool b ) const
    {
        if ( !this->is_valid( ) )
            return;

        *reinterpret_cast< bool* > ( reinterpret_cast< uintptr_t > ( ent ) + offset ) = b;
    }

    void set_int ( int i ) const
    {
        if ( !this->is_valid( ) )
            return;

        *reinterpret_cast< int* > ( reinterpret_cast< uintptr_t > ( ent ) + offset ) = i;
    }

    void set_float ( float f ) const
    {
        if ( !this->is_valid( ) )
            return;

        *reinterpret_cast< float* > ( reinterpret_cast< uintptr_t > ( ent ) + offset ) = f;
    }

    void set_vector ( lua_vector_t v ) const
    {
        if ( !this->is_valid( ) )
            return;

        *reinterpret_cast< vector_t* > ( reinterpret_cast< uintptr_t > ( ent ) + offset ) = vector_t ( v.x, v.y, v.z );
    }

    void set_angle ( lua_angle_t a ) const
    {
        if ( !this->is_valid( ) )
            return;

        *reinterpret_cast< qangle_t* > ( reinterpret_cast< uintptr_t > ( ent ) + offset ) = qangle_t ( a.pitch, a.yaw, a.roll );
    }
};

struct lua_entity_t
{
    C_BaseEntity* ent = nullptr;
    lua_entity_t ( ) = default;

    explicit lua_entity_t ( C_BaseEntity* e ) : ent ( e )
    {
    };

    explicit lua_entity_t ( int idx )
    {
        this->ent = reinterpret_cast< C_BaseEntity* > ( interfaces::entity_list->get_client_entity ( idx ) );
    }

    bool is_valid ( ) const
    {
        return ! ( !ent || ent->is_dormant( ) );
    }

    int get_index ( ) const
    {
        if ( !is_valid( ) )
            return -1;

        return ent->ent_index( );
    }

    int get_team_number ( ) const
    {
        if ( !is_valid( ) )
            return -1;

        return ent->team_num( );
    }

    lua_vector_t get_abs_origin ( ) const
    {
        if ( !is_valid( ) )
            return lua_vector_t( );

        const auto v = ent->get_abs_origin( );
        return lua_vector_t ( v.x, v.y, v.z );
    }

    lua_vector_t get_mins ( ) const
    {
        if ( !is_valid( ) )
            return lua_vector_t( );

        const auto v = ent->get_collideable( )->obb_mins( );
        return lua_vector_t ( v.x, v.y, v.z );
    }

    lua_vector_t get_maxs ( ) const
    {
        if ( !is_valid( ) )
            return lua_vector_t( );

        const auto v = ent->get_collideable( )->obb_maxs( );
        return lua_vector_t ( v.x, v.y, v.z );
    }

    int get_health ( ) const
    {
        if ( !is_valid( ) )
            return 0;

        return dynamic_cast< C_BasePlayer* > ( ent )->health( );
    }

    bool is_player ( ) const
    {
        if ( !is_valid( ) )
            return false;

        return ent->is_player( );
    }

    bool is_alive ( ) const
    {
        if ( !is_valid( ) )
            return false;

        return dynamic_cast< C_BasePlayer* > ( ent )->is_alive( );
    }

    lua_vector_t get_hitbox_position ( const int hitbox ) const
    {
        if ( !is_valid( ) )
            return lua_vector_t( );

        const auto v = dynamic_cast< C_BasePlayer* > ( ent )->get_hitbox_pos ( hitbox );
        return lua_vector_t ( v.x, v.y, v.z );
    }

    lua_vector_t get_bone_position ( const int hitgroup ) const
    {
        if ( !is_valid( ) )
            return lua_vector_t( );

        const auto v = dynamic_cast< C_BasePlayer* > ( ent )->get_bone_pos ( hitgroup );
        return lua_vector_t ( v.x, v.y, v.z );
    }

    float get_weapon_spread ( ) const
    {
        if ( !is_valid( ) )
            return 0.f;

        const auto weapon = dynamic_cast< C_BasePlayer* > ( ent )->active_weapon( ).get( );

        if ( weapon )
            return weapon->get_spread( );

        return 0.f;
    }

    float get_weapon_inaccuracy ( ) const
    {
        if ( !is_valid( ) )
            return 0.f;

        const auto weapon = dynamic_cast< C_BasePlayer* > ( ent )->active_weapon( ).get( );

        if ( weapon )
            return weapon->get_inaccuracy( );

        return 0.f;
    }

    lua_angle_t get_eye_angles ( ) const
    {
        if ( !is_valid( ) || !is_player( ) )
            return lua_angle_t( );

        const auto a = dynamic_cast< C_BasePlayer* > ( ent )->eye_angles( );
        return lua_angle_t ( a.pitch, a.yaw, a.roll );
    }

    void set_eye_angles ( const lua_angle_t ang ) const
    {
        if ( !is_valid( ) || !is_player( ) )
            return;

        dynamic_cast< C_BasePlayer* > ( ent )->eye_angles( ) = qangle_t ( ang.pitch, ang.yaw, ang.roll );
    }

    lua_netvar_t get_netvar ( std::string table, std::string prop )
    {
        return lua_netvar_t ( this->ent, table, prop );
    }
};

struct lua_setting_var_t
{
    config_saver_struct_t cfg;
    lua_setting_var_t ( ) = default;

    explicit lua_setting_var_t ( std::string name )
    {
        this->cfg = g_options.get_option_data ( std::move ( name ) );
    }

    bool is_valid ( ) const
    {
        return cfg.ptr != nullptr;
    }

    // get functions
    int get_int ( ) const
    {
        switch ( static_cast< config_types_t > ( cfg.type ) )
        {
        case config_types_t::t_qangle:
        case config_types_t::t_color:
        case config_types_t::t_vector:
        case config_types_t::t_none:
        case config_types_t::t_rbot_configs:
            return 0;

        case config_types_t::t_bool:
            return static_cast< int > ( *reinterpret_cast< bool* > ( cfg.ptr ) );

        case config_types_t::t_float:
            return static_cast< int > ( *reinterpret_cast< float* > ( cfg.ptr ) );

        case config_types_t::t_int:
            return *reinterpret_cast< int* > ( cfg.ptr );
        }

        return 0;
    }

    float get_float ( ) const
    {
        switch ( static_cast< config_types_t > ( cfg.type ) )
        {
        case config_types_t::t_qangle:
        case config_types_t::t_color:
        case config_types_t::t_vector:
        case config_types_t::t_none:
        case config_types_t::t_rbot_configs:
            return 0.f;

        case config_types_t::t_bool:
            return static_cast< float > ( *reinterpret_cast< bool* > ( cfg.ptr ) );

        case config_types_t::t_float:
            return *reinterpret_cast< float* > ( cfg.ptr );

        case config_types_t::t_int:
            return static_cast< float > ( *reinterpret_cast< int* > ( cfg.ptr ) );
        }

        return 0.f;
    }

    bool get_bool ( ) const
    {
        switch ( static_cast< config_types_t > ( cfg.type ) )
        {
        case config_types_t::t_qangle:
        case config_types_t::t_color:
        case config_types_t::t_vector:
        case config_types_t::t_none:
        case config_types_t::t_rbot_configs:
            return false;

        case config_types_t::t_bool:
            return *reinterpret_cast< bool* > ( cfg.ptr );

        case config_types_t::t_float:
            return static_cast< bool > ( *reinterpret_cast< float* > ( cfg.ptr ) );

        case config_types_t::t_int:
            return static_cast< bool > ( *reinterpret_cast< int* > ( cfg.ptr ) );
        }

        return false;
    }

    lua_color_t get_color ( ) const
    {
        switch ( static_cast< config_types_t > ( cfg.type ) )
        {
        case config_types_t::t_color:
            {
                auto tmp = *reinterpret_cast< Color* > ( cfg.ptr );
                return lua_color_t ( tmp.r( ), tmp.g( ), tmp.b( ) );
            }

        case config_types_t::t_qangle:
        case config_types_t::t_vector:
        case config_types_t::t_none:
        case config_types_t::t_rbot_configs:
        case config_types_t::t_bool:
        case config_types_t::t_float:
        case config_types_t::t_int:
            return lua_color_t ( 255, 255, 255, 255 );
        }

        return lua_color_t ( 255, 255, 255, 255 );
    }

    // set functions
    void set_int ( const int num ) const
    {
        switch ( static_cast< config_types_t > ( cfg.type ) )
        {
        case config_types_t::t_qangle:
        case config_types_t::t_color:
        case config_types_t::t_vector:
        case config_types_t::t_none:
        case config_types_t::t_rbot_configs:
            return;

        case config_types_t::t_bool:
            *reinterpret_cast< bool* > ( cfg.ptr ) = static_cast< bool > ( num );
            return;

        case config_types_t::t_float:
            *reinterpret_cast< float* > ( cfg.ptr ) = static_cast< float > ( num );
            return;

        case config_types_t::t_int:
            *reinterpret_cast< int* > ( cfg.ptr ) = num;
        }
    }

    void set_float ( const float num ) const
    {
        switch ( static_cast< config_types_t > ( cfg.type ) )
        {
        case config_types_t::t_qangle:
        case config_types_t::t_color:
        case config_types_t::t_vector:
        case config_types_t::t_none:
        case config_types_t::t_rbot_configs:
            return;

        case config_types_t::t_bool:
            *reinterpret_cast< bool* > ( cfg.ptr ) = static_cast< bool > ( num );
            return;

        case config_types_t::t_float:
            *reinterpret_cast< float* > ( cfg.ptr ) = num;
            return;

        case config_types_t::t_int:
            *reinterpret_cast< int* > ( cfg.ptr ) = static_cast< int > ( num );
        }
    }

    void set_bool ( const bool b ) const
    {
        switch ( static_cast< config_types_t > ( cfg.type ) )
        {
        case config_types_t::t_qangle:
        case config_types_t::t_color:
        case config_types_t::t_vector:
        case config_types_t::t_none:
        case config_types_t::t_rbot_configs:
            return;

        case config_types_t::t_bool:
            *reinterpret_cast< bool* > ( cfg.ptr ) = b;
            return;

        case config_types_t::t_float:
            *reinterpret_cast< float* > ( cfg.ptr ) = static_cast< float > ( b );
            return;

        case config_types_t::t_int:
            *reinterpret_cast< int* > ( cfg.ptr ) = static_cast< int > ( b );
        }
    }

    void set_color ( const lua_color_t clr ) const
    {
        switch ( static_cast< config_types_t > ( cfg.type ) )
        {
        case config_types_t::t_color:
            {
                *reinterpret_cast< Color* > ( cfg.ptr ) = Color ( clr.r, clr.g, clr.b, clr.a );
            }

        case config_types_t::t_qangle:
        case config_types_t::t_vector:
        case config_types_t::t_none:
        case config_types_t::t_rbot_configs:
        case config_types_t::t_bool:
        case config_types_t::t_float:
        case config_types_t::t_int:
            break;
        }
    }
};

struct lua_convar_t
{
    con_var* cvar = nullptr;
    lua_convar_t ( ) = default;

    explicit lua_convar_t ( std::string name )
    {
        cvar = interfaces::cvar->find_var ( name.data( ) );
    }

    bool is_valid ( ) const
    {
        return cvar != nullptr;
    }

    // get functions
    bool get_bool ( ) const
    {
        if ( is_valid( ) )
            return false;

        return cvar->get_bool( );
    }

    int get_int ( ) const
    {
        if ( is_valid( ) )
            return 0;

        return cvar->get_int( );
    }

    float get_float ( ) const
    {
        if ( !is_valid( ) )
            return 0.f;

        return cvar->get_flags( );
    }

    // set functions
    void set_bool ( const bool b ) const
    {
        if ( !is_valid( ) )
            return;

        cvar->set_value ( b );
    }

    void set_int ( const int in ) const
    {
        if ( !is_valid( ) )
            return;

        cvar->set_value ( in );
    }

    void set_float ( const float fl ) const
    {
        if ( !is_valid( ) )
            return;

        cvar->set_value ( fl );
    }
};

struct lua_usercmd_t
{
    usercmd_t* cmd = nullptr;
    lua_usercmd_t ( ) = default;;

    explicit lua_usercmd_t ( usercmd_t* p_cmd )
    {
        this->cmd = p_cmd;
    };

    lua_angle_t get_view_angles ( ) const
    {
        return lua_angle_t ( cmd->viewangles );
    }

    void set_view_angles ( const lua_angle_t ang ) const
    {
        cmd->viewangles = qangle_t ( ang.pitch, ang.yaw, ang.roll );
    }

    int get_buttons ( ) const
    {
        return cmd->buttons;
    }

    void set_buttons ( const int buttons ) const
    {
        cmd->buttons = buttons;
    }
};

struct lua_trace_t
{
    int contents = 0;
    float fraction = 0.f;
    lua_entity_t entity;

    lua_trace_t ( CGameTrace* trace ) : contents ( trace->contents ), fraction ( trace->fraction ),
                                        entity ( static_cast< C_BaseEntity* > ( trace->hit_entity ) )
    {
    };
    lua_trace_t ( ) = default;
};
