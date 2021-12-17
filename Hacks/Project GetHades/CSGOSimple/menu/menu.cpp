#include "menu.hpp"
#define NOMINMAX
#include <Windows.h>
#include <chrono>

#include "../valve_sdk/csgostructs.hpp"
#include "../helpers/input.hpp"
#include "../options.h"
#include "../droid.hpp"
#include "hades_menu.h"

static con_var* cl_mouseenable = nullptr;

c_menu menu;

void c_menu::start_draw ( )
{
}

void c_menu::end_draw ( )
{
}

void c_menu::initialize ( )
{
    _visible = false;

    cl_mouseenable = interfaces::cvar->find_var ( xor_str ( "cl_mouseenable" ) );
}

void c_menu::shutdown ( ) const
{
    cl_mouseenable->set_value ( true );
}

void c_menu::show ( )
{
    _visible = true;
    cl_mouseenable->set_value ( false );
}

void c_menu::hide ( )
{
    _visible = false;
    cl_mouseenable->set_value ( true );
}

void c_menu::toggle ( )
{
    cl_mouseenable->set_value ( !_visible );
    _visible = !_visible;
}
