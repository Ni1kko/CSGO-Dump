using System;
using System.Diagnostics;
using System.IO;
using System.Windows;
using DDX.Core;
using DDX.Forms;
using DDX.Functions.Bsp;
using DDX.Objects;
using DDX.Structs;
using DDX.Structs.Config;
using SharpDX.Mathematics.Interop;
// ReSharper disable InconsistentNaming

namespace DDX
{
    /// <summary>
    /// Global variables
    /// </summary>
    public static class G
    {
        public static readonly ScreenParameters ScreenParameters = new ScreenParameters
        {
            Width  = (float)SystemParameters.PrimaryScreenWidth,
            Height = (float)SystemParameters.PrimaryScreenHeight,

            Center = new RawVector2(
                (float)SystemParameters.PrimaryScreenWidth / 2f,
                (float)SystemParameters.PrimaryScreenHeight / 2f),
        };

        public static Memory Mem;
        public static BspParsing Bsp;
        public static OverlayForm O;
        public static Config C;
        public static LocalPlayer Local;
        public static Bomb Bomb;

        public static string BaseDir = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments), "DDX");

        public static ulong Ref = 30;
        public static int Now;
    }

    public static class Module
    {
        public static ProcessModule Client;
        public static ProcessModule Engine;
    }

    public static class Base
    {
        public static IntPtr m_dwInput;
        public static IntPtr m_bSendPackets;
        public static IntPtr m_dwAttack1;
        public static IntPtr m_dwAttack2;
        public static IntPtr m_dwBackward;
        public static IntPtr m_dwBombPlanted;
        public static IntPtr m_dwClientCmd;
        public static IntPtr m_dwClientState;
        public static IntPtr m_dwDuck;
        public static IntPtr m_dwEntityList;
        public static IntPtr m_dwEntityListLength;
        public static IntPtr m_dwForward;
        public static IntPtr m_dwGlobalVars;
        public static IntPtr m_dwGlowObject;
        public static IntPtr m_dwJump;
        public static IntPtr m_dwLeft;
        public static IntPtr m_dwLocalPlayer;
        public static IntPtr m_dwMouseEnable;
        public static IntPtr m_dwNoShake;
        public static IntPtr m_dwPlayerResource;
        public static IntPtr m_dwRadarBase;
        public static IntPtr m_dwRight;
        public static IntPtr m_dwViewMatrix;
    }

    public static class Offset
    {
        public const int m_aimPunchAngle          = 0x301C;
        public const int m_angRotation            = 0x12C;
        public const int m_ArmorValue             = 0xAA04;
        public const int m_bBombPlanted           = 0x99D;
        public const int m_bBombTicking           = 0x2970;
        public const int m_bDormant               = 0xE9;
        public const int m_bGunGameImmunity       = 0x38B0;
        public const int m_bHasDefuser            = 0xAA14;
        public const int m_bHasHeavyArmor         = 0xA9FD;
        public const int m_bHasHelmet             = 0xA9FC;
        public const int m_bInReload              = 0x3245;
        public const int m_bIsDefusing            = 0x38A4;
        public const int m_bIsGrabbingHostag      = 0x38A5;
        public const int m_bIsScoped              = 0x389C;
        public const int m_bSpotted               = 0x939;
        public const int m_bSpottedByMask         = 0x97C;
        public const int m_clrRender              = 0x70;
        public const int m_dwBoneMatrix           = 0x2698;
        public const int m_dwInGame               = 0x100;
        public const int m_dwLocalPlayerIndex     = 0x178;
        public const int m_dwMapDirectory         = 0x180;
        public const int m_dwMapname              = 0x284;
        public const int m_dwModel                = 0x6C;
        public const int m_dwMouseEnabledPointer  = 0x30;
        public const int m_dwRadarBasePointer     = 0x54;
        public const int m_dwViewAngles           = 0x4D0C;
        public const int m_fFlags                 = 0x100;
        public const int m_flC4Blow               = 0x297C;
        public const int m_flDefuseLength         = 0x2990;
        public const int m_flDefuseCountDown      = 0x2994;
        public const int m_flFallbackWear         = 0x3178;
        public const int m_flFlashDuration        = 0xA308;
        public const int m_flFlashMaxAlpha        = 0xA304;
        public const int m_flNextPrimaryAttack    = 0x31D8;
        public const int m_hActiveWeapon          = 0x2EE8;
        public const int m_hForceFullUpdate       = 0x16C;
        public const int m_hMyWeapons             = 0x2DE8;
        public const int m_hViewModel             = 0x32FC;
        public const int m_hObserverTarget        = 0x3380;
        public const int m_iAccountID             = 0x2FA8;
        public const int m_iClip1                 = 0x3204;
        public const int m_iCompetitiveRanking    = 0x1A44;
        public const int m_iCrossHairID           = 0xAA70;
        public const int m_iCurrentSequenceNumber = 0x4CA8;
        public const int m_iDefaultFOV            = 0x332C;
        public const int m_iEntityQuality         = 0x2F8C;
        public const int m_iGlowIndex             = 0xA320;
        public const int m_iHealth                = 0xFC;
        public const int m_iItemDefinitionIndex   = 0x2F88;
        public const int m_iItemIDHigh            = 0x2FA0;
        public const int m_iKills                 = 0xBE8;
        public const int m_iObserverMode          = 0x336C;
        public const int m_iPlayerC4              = 0x161C;
        public const int m_iShotsFired            = 0xA2C0;
        public const int m_iTeamNum               = 0xF0;
        public const int m_lifeState              = 0x25B;
        public const int m_nFallbackPaintKit      = 0x3170;
        public const int m_nFallbackStatTrak      = 0x317C;
        public const int m_nModelIndex            = 0x254;
        public const int m_nTickBase              = 0x3424;
        public const int m_OriginalOwnerXuidLow   = 0x3168;
        public const int m_szCustomName           = 0x301C;
        public const int m_szLastPlaceName        = 0x35A8;
        public const int m_szName                 = 0x24;
        public const int m_vecOrigin              = 0x134;
        public const int m_vecPunch               = 0x301C;
        public const int m_vecVelocity            = 0x110;
        public const int m_vecViewOffset          = 0x104;
        public const int m_viewPunchAngle         = 0x3010;
        public const int m_zoomLevel              = 0x3350;
    }
}
