using System;
using System.Runtime.InteropServices;

namespace DDX.Structs
{
    [StructLayout(LayoutKind.Explicit)]
    public unsafe struct UserCmd
    {
        [FieldOffset(0x0)] public IntPtr pVft;
        [FieldOffset(0x4)] public int m_iCmdNumber;
        [FieldOffset(0x8)] public int m_iTickCount;
        [FieldOffset(0xC)] public Vector3 m_vecViewAngles;
        [FieldOffset(0x18)] public Vector3 m_vecAimDirection;
        [FieldOffset(0x24)] public float m_flForwardmove;
        [FieldOffset(0x28)] public float m_flSidemove;
        [FieldOffset(0x2C)] public float m_flUpmove;
        [FieldOffset(0x30)] public int m_iButtons;
        [FieldOffset(0x34)] public bool m_bImpulse;
        [FieldOffset(0x35)] public fixed byte jnk1[3];
        [FieldOffset(0x38)] public int m_iWeaponSelect;
        [FieldOffset(0x3C)] public int m_iWeaponSubtype;
        [FieldOffset(0x40)] public int m_iRandomSeed;
        [FieldOffset(0x44)] public short m_siMouseDx;
        [FieldOffset(0x46)] public short m_siMouseDy;
        [FieldOffset(0x48)] public bool m_bHasBeenPredicted;
        [FieldOffset(0x49)] public fixed byte jnk2[27];
    }
}