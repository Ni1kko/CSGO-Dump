#pragma once

#include "../misc/usercmd_t.hpp"

#define multiplayer_backup 150

class bf_write;
class bf_read;

class c_input
{
public:
    char pad_0x00[0x0C];
    bool m_trackir_available;
    bool m_mouse_initiated;
    bool m_mouse_active;
    bool m_fJoystickAdvancedInit;
    char pad_0x08[0x2C];
    void* m_pKeys;
    char pad_0x38[0x6C];
    bool m_fCameraInterceptingMouse;
    bool m_fCameraInThirdPerson;
    bool m_fCameraMovingWithMouse;
    Vector m_vecCameraOffset;
    bool m_fCameraDistanceMove;
    int m_nCameraOldX;
    int m_nCameraOldY;
    int m_nCameraX;
    int m_nCameraY;
    bool m_CameraIsOrthographic;
    QAngle m_angPreviousViewAngles;
    QAngle m_angPreviousViewAnglesTilt;
    float m_flLastForwardMove;
    int m_nClearInputState;
    char pad_0xE4[0x8];
    usercmd_t* m_pCommands;
    c_verified_user_cmd* m_pVerifiedCommands;

    inline usercmd_t* get_user_cmd ( int sequence_number );
    inline usercmd_t* get_user_cmd ( int nSlot, int sequence_number );
    inline c_verified_user_cmd* get_verified_cmd ( int sequence_number );
};

usercmd_t* c_input::get_user_cmd ( int sequence_number )
{
    return CallVFunction< usercmd_t* ( __thiscall*) ( void*, int, int ) > ( this, 8 ) ( this, 0, sequence_number );
}

usercmd_t* c_input::get_user_cmd ( int nSlot, int sequence_number )
{
    return CallVFunction< usercmd_t* ( __thiscall*) ( void*, int, int ) > ( this, 8 ) ( this, nSlot, sequence_number );
}

c_verified_user_cmd* c_input::get_verified_cmd ( int sequence_number )
{
    auto verifiedCommands = * ( c_verified_user_cmd** )( reinterpret_cast< uint32_t > ( this ) + 0xF8 );
    return &verifiedCommands[ sequence_number % multiplayer_backup ];
}
