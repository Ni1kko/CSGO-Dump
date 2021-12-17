using System;
using System.Runtime.InteropServices;

namespace DDX.Structs
{
    [StructLayout(LayoutKind.Explicit)]
    public unsafe struct Input
    {
        [FieldOffset(0x0)] public IntPtr m_pVftable;
        [FieldOffset(0x4)] public bool m_bTrackIRAvailable;
        [FieldOffset(0x5)] public bool m_bMouseInitialized;
        [FieldOffset(0x6)] public bool m_bMouseActive;
        [FieldOffset(0x7)] public bool m_bJoystickAdvancedInit;
        [FieldOffset(0x8)] public fixed byte jnk1[44];
        [FieldOffset(0x34)] public IntPtr m_pKeys;
        [FieldOffset(0x38)] public fixed byte jnk2[100];
        [FieldOffset(0x9C)] public bool m_bCameraInterceptingMouse;
        [FieldOffset(0x9D)] public bool m_bCameraInThirdPerson;
        [FieldOffset(0x9E)] public bool m_bCameraMovingWithMouse;
        [FieldOffset(0xA0)] public Vector3 m_vecCameraOffset;
        [FieldOffset(0xAC)] public bool m_bCameraDistanceMove;
        [FieldOffset(0xB0)] public int m_nCameraOldX;
        [FieldOffset(0xB4)] public int m_nCameraOldY;
        [FieldOffset(0xB8)] public int m_nCameraX;
        [FieldOffset(0xBC)] public int m_nCameraY;
        [FieldOffset(0xC0)] public bool m_bCameraIsOrthographic;
        [FieldOffset(0xC4)] public Vector3 m_vecPreviousViewAngles;
        [FieldOffset(0xD0)] public Vector3 m_vecPreviousViewAnglesTilt;
        [FieldOffset(0xDC)] public float m_flLastForwardMove;
        [FieldOffset(0xE0)] public int m_nClearInputState;
        [FieldOffset(0xE4)] public fixed byte jnk3[8];
        [FieldOffset(0xEC)] public IntPtr m_pCommands;
        [FieldOffset(0xF0)] public IntPtr m_pVerifiedCommands;
    }
}