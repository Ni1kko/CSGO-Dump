#include "movement.h"
#include "../../helpers/math.hpp"

void c_movement::fix_movement ( usercmd_t* cmd, qangle_t angles ) const
{
    auto delta_view = cmd->viewangles.yaw - angles.yaw;
    float f1;
    float f2;

    if ( angles.yaw < 0.f )
        f1 = 360.0f + angles.yaw;
    else
        f1 = angles.yaw;

    if ( cmd->viewangles.yaw < 0.0f )
        f2 = 360.0f + cmd->viewangles.yaw;
    else
        f2 = cmd->viewangles.yaw;

    if ( f2 < f1 )
        delta_view = abs ( f2 - f1 );
    else
        delta_view = 360.0f - abs ( f1 - f2 );

    delta_view = 360.0f - delta_view;

    cmd->forwardmove = cos ( deg2rad ( delta_view ) ) * globals.old_forwardmove + cos ( deg2rad ( delta_view + 90.f ) ) * globals.old_sidemove;
    cmd->sidemove = sin ( deg2rad ( delta_view ) ) * globals.old_forwardmove + sin ( deg2rad ( delta_view + 90.f ) ) * globals.old_sidemove;

    cmd->buttons &= ~IN_MOVERIGHT;
    cmd->buttons &= ~IN_MOVELEFT;
    cmd->buttons &= ~IN_FORWARD;
    cmd->buttons &= ~IN_BACK;
}
