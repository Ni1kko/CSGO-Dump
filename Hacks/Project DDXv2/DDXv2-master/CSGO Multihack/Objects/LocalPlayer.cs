using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using DDX.Enums;
using DDX.Extensions;
using DDX.Functions;
using DDX.Structs;

namespace DDX.Objects
{
    public class LocalPlayer : Player
    {
        public LocalPlayer() : base()
        {
        }

        public LocalPlayer(int index, IntPtr handle) : base(index, handle)
        {
        }

        private ulong _lastCanShootRef;
        private bool _lastCanShoot;
        public bool GetCanShoot(bool force = false)
        {
            if (!force && _lastCanShootRef == G.Ref)
                return _lastCanShoot;

            _lastCanShootRef = G.Ref;

            var weap = GetActiveWeapon();
            return _lastCanShoot = weap.GetAmmo() > 0 && weap.GetCanShoot();
        }

        private ulong _lastCrosshairPlayerRef;
        private Player _lastCrosshairPlayer;
        public Player GetCrosshairPlayer(bool force = false)
        {
            if (!force && _lastCrosshairPlayerRef == G.Ref)
                return _lastCrosshairPlayer;

            _lastCrosshairPlayerRef = G.Ref;

            var crosshairPlayerIndex = G.Mem.Read<int>(Handle + Offset.m_iCrossHairID);
            var crosshairPlayerHandle = GlobalRead.GetEntityHandleByIndex(crosshairPlayerIndex);

            return _lastCrosshairPlayer = new Player(crosshairPlayerIndex, crosshairPlayerHandle);
        }

        private ulong _lastIsAbleToShootRef;
        private bool _lastIsAbleToShoot;
        public bool GetIsAbleToShoot(bool force = false, bool includeKnife = false)
        {
            if (!force && _lastIsAbleToShootRef == G.Ref)
                return _lastIsAbleToShoot;

            _lastIsAbleToShootRef = G.Ref;

            if (!GetCanShoot() && !includeKnife)
            {
                return _lastIsAbleToShoot = false;
            }

            var weap = GetActiveWeapon();

            if (weap.GetAmmo() == 0 && !includeKnife)
            {
                return _lastIsAbleToShoot = false;
            }

            var serverTime = G.Mem.Read<int>(Handle + Offset.m_nTickBase) * 0.015625f;

            return _lastIsAbleToShoot = weap.GetNextPrimaryAttack() < serverTime;
        }

        private ulong _lastIsInGameRef;
        private bool _lastIsInGame;
        public bool GetIsInGame(bool force = false)
        {
            if (!force && _lastIsInGameRef == G.Ref)
                return _lastIsInGame;

            _lastIsInGameRef = G.Ref;
            var inGame = (InGame)G.Mem.Read<byte>(GlobalRead.GetClientStateHandle() + Offset.m_dwInGame);
            return _lastIsInGame = Handle != IntPtr.Zero && (inGame == InGame.Full || inGame == InGame.ChangeLevel);
        }

        private ulong _lastIsMouseEnableRef;
        private bool _lastIsMouseEnable;
        public bool GetIsMouseEnable(bool force = false)
        {
            if (!force && _lastIsMouseEnableRef == G.Ref)
                return _lastIsMouseEnable;

            _lastIsMouseEnableRef = G.Ref;

            var mouseEnableXor = G.Mem.Read<int>(Base.m_dwMouseEnable + Offset.m_dwMouseEnabledPointer);
            var mouseEnable = mouseEnableXor ^ (int)Base.m_dwMouseEnable;
            return _lastIsMouseEnable = mouseEnable == 1;
        }

        private ulong _lastIsScopedRef;
        private bool _lastIsScoped;
        public bool GetIsScoped(bool force = false)
        {
            if (!force && _lastIsScopedRef == G.Ref)
                return _lastIsScoped;

            _lastIsScopedRef = G.Ref;
            return _lastIsScoped = G.Mem.Read<byte>(Handle + Offset.m_bIsScoped) == 1;
        }

        private ulong _lastKillsRef;
        private int _lastKills;
        public int GetKills(bool force = false)
        {
            if (!force && _lastKillsRef == G.Ref)
                return _lastKills;

            _lastKillsRef = G.Ref;
            return _lastKills = G.Mem.Read<int>(GlobalRead.GetPlayerResourceHandle() + Offset.m_iKills + (Index + 1) * 0x4);
        }

        private ulong _lastFlashRef;
        private float _lastFlash;
        public float GetFlash(bool force = false)
        {
            if (!force && _lastFlashRef == G.Ref)
                return _lastFlash;

            _lastFlashRef = G.Ref;
            return _lastFlash = G.Mem.Read<float>(Handle + Offset.m_flFlashMaxAlpha);
        }

        private ulong _lastFlashDurationRef;
        private float _lastFlashDuration;
        public float GetFlashDuration(bool force = false)
        {
            if (!force && _lastFlashDurationRef == G.Ref)
                return _lastFlashDuration;

            _lastFlashDurationRef = G.Ref;
            return _lastFlashDuration = G.Mem.Read<float>(Handle + Offset.m_flFlashDuration);
        }

        private ulong _lastForceFullUpdateRef;
        private int _lastForceFullUpdate;
        public int GetForceFullUpdate(bool force = false)
        {
            if (!force && _lastForceFullUpdateRef == G.Ref)
                return _lastForceFullUpdate;

            _lastForceFullUpdateRef = G.Ref;
            return _lastForceFullUpdate = G.Mem.Read<int>(GlobalRead.GetClientStateHandle() + Offset.m_hForceFullUpdate);
        }

        private ulong _lastFovRef;
        private int _lastFov;
        public int GetFov(bool force = false)
        {
            if (!force && _lastFovRef == G.Ref)
                return _lastFov;

            _lastFovRef = G.Ref;
            return _lastFov = G.Mem.Read<int>(Handle + Offset.m_iDefaultFOV);
        }

        private List<Weapon> _lastMyWeapons;
        public List<Weapon> GetMyWeapons(bool force = false)
        {
            if (!force && _lastMyWeapons != null)
            {
                return _lastMyWeapons;
            }

            var weapList = new List<Weapon>();
            for (var i = 0; i < 64; i++)
            {
                var weapId = (WeaponId) i;
                if (weapId.GetHasSkin())
                {
                    weapList.Add(new Weapon(i));
                }
            }

            return _lastMyWeapons = weapList;
        }

        private ulong _lastPunchRef;
        private ViewAngles _lastPunch;
        public ViewAngles GetPunch(bool force = false)
        {
            if (!force && _lastPunchRef == G.Ref)
                return _lastPunch;

            _lastPunchRef = G.Ref;
            return _lastPunch = G.Mem.Read<ViewAngles>(Handle + Offset.m_vecPunch);
        }

        private ulong _lastScopeScaleRef;
        private float _lastScopeScale;
        public float GetScopeScale(bool force = false)
        {
            if (!force && _lastScopeScaleRef == G.Ref)
                return _lastScopeScale;

            _lastScopeScaleRef = G.Ref;

            if (!GetIsScoped())
                return _lastScopeScale = 1f;

            return _lastScopeScale = GetZoomLevel() == 1 ? 2.5f : 12f;
        }

        private ulong _lastSendPacketsRef;
        private bool _lastSendPackets;
        public bool GetSendPackets(bool force = false)
        {
            if (!force && _lastSendPacketsRef == G.Ref)
                return _lastSendPackets;

            _lastSendPacketsRef = G.Ref;
            return _lastSendPackets = G.Mem.Read<byte>(Base.m_bSendPackets) == 1;
        }

        private ulong _lastShotsFiredRef;
        private int _lastShotsFired;
        public int GetShotsFired(bool force = false)
        {
            if (!force && _lastShotsFiredRef == G.Ref)
                return _lastShotsFired;

            _lastShotsFiredRef = G.Ref;
            return _lastShotsFired = G.Mem.Read<int>(Handle + Offset.m_iShotsFired);
        }

        private ulong _lastThirdPersonModeRef;
        private int _lastThirdPersonMode;
        public int GetThirdPersonMode(bool force = false)
        {
            if (!force && _lastThirdPersonModeRef == G.Ref)
                return _lastThirdPersonMode;

            _lastThirdPersonModeRef = G.Ref;
            return _lastThirdPersonMode = G.Mem.Read<int>(Handle + Offset.m_iObserverMode);
        }

        private ulong _lastVelocityRef;
        private Vector3 _lastVelocity;
        public Vector3 GetVelocity(bool force = false)
        {
            if (!force && _lastVelocityRef == G.Ref)
                return _lastVelocity;

            _lastVelocityRef = G.Ref;
            return _lastVelocity = G.Mem.Read<Vector3>(Handle + Offset.m_vecVelocity);
        }

        private ulong _lastViewAnglesRef;
        private ViewAngles _lastViewAngles;
        public ViewAngles GetViewAngles(bool force = false)
        {
            if (!force && _lastViewAnglesRef == G.Ref)
                return _lastViewAngles;

            _lastViewAnglesRef = G.Ref;
            return _lastViewAngles = G.Mem.Read<ViewAngles>(GlobalRead.GetClientStateHandle() + Offset.m_dwViewAngles);
        }

        private ulong _lastViewOriginRef;
        private Vector3 _lastViewOrigin;
        public Vector3 GetViewOrigin(bool force = false)
        {
            if (!force && _lastViewOriginRef == G.Ref)
                return _lastViewOrigin;

            _lastViewOriginRef = G.Ref;
            return _lastViewOrigin = GetOrigin() + G.Mem.Read<Vector3>(Handle + Offset.m_vecViewOffset);
        }

        private ulong _lastZoomLevelRef;
        private int _lastZoomLevel;
        public int GetZoomLevel(bool force = false)
        {
            if (!force && _lastZoomLevelRef == G.Ref)
                return _lastZoomLevel;

            _lastZoomLevelRef = G.Ref;
            return _lastZoomLevel = G.Mem.Read<int>(GetActiveWeaponHandle() + Offset.m_zoomLevel);
        }

        public void SetFov(int value)
        {
            G.Mem.Write(Handle + Offset.m_iDefaultFOV, value);
        }

        public void SetFlash(float value)
        {
            G.Mem.Write(Handle + Offset.m_flFlashMaxAlpha, value);
        }

        public void SetFlashDuration(float value)
        {
            G.Mem.Write(Handle + Offset.m_flFlashDuration, value);
        }

        public void SetModelIndex(int value)
        {
            G.Mem.Write(Handle + Offset.m_nModelIndex, value);
        }

        public void SetSendPackets(bool value)
        {
            G.Mem.Write(Base.m_bSendPackets, value ? (byte)0x1 : (byte)0x0);
        }

        public void SetThirdPersonMode(int value)
        {
            G.Mem.Write(Handle + Offset.m_iObserverMode, value);
        }

        public void SetViewAngles(ViewAngles value)
        {
            if (float.IsNaN(value.Pitch) || float.IsNaN(value.Yaw) || float.IsInfinity(value.Pitch) || float.IsInfinity(value.Yaw))
                return;

            value = value.NormalizeAngles();

            G.Mem.Write(GlobalRead.GetClientStateHandle() + Offset.m_dwViewAngles, value);
        }

        private int _lastSilentAim;
        public unsafe void SetViewAnglesSilent(ViewAngles value)
        {
            if (float.IsNaN(value.Pitch) || 
                float.IsNaN(value.Yaw) || 
                float.IsInfinity(value.Pitch) ||
                float.IsInfinity(value.Yaw))
            {
                return;
            }

            if (_lastSilentAim + 25 > G.Now)
            {
                return;
            }

            _lastSilentAim = G.Now;

            value = value.NormalizeAngles();

            SetSendPackets(false);

            var currentSequenceNumber = GlobalRead.GetCurrentSequenceNumber(true);

            var input = G.Mem.Read<Input>(Base.m_dwInput);

            var pUserCmd    = input.m_pCommands         +  currentSequenceNumber      % 150 *  sizeof(UserCmd);
            var pUserCmdOld = input.m_pCommands         + (currentSequenceNumber - 1) % 150 *  sizeof(UserCmd);
            var pUserCmdVer = input.m_pVerifiedCommands + (currentSequenceNumber - 1) % 150 * (sizeof(UserCmd) + 0x4);

            while (G.Mem.Read<int>(pUserCmd + 0x4) != currentSequenceNumber)
            {
                Thread.Yield();
            }

            G.Mem.Write(pUserCmdOld + 0xC, value);
            G.Mem.Write(pUserCmdOld + 0x30, 1);

            G.Mem.Write(pUserCmdVer + 0xC, value);
            G.Mem.Write(pUserCmdVer + 0x30, 1);

            Thread.Sleep(7);

            SetSendPackets(true);
        }

        public void SetForceFullUpdate()
        {
            G.Mem.Write(GlobalRead.GetClientStateHandle() + Offset.m_hForceFullUpdate, -1);
        }

        public void SetAttack1(byte value)
        {
            G.Mem.Write(Base.m_dwAttack1, value);
        }

        public void SetAttack2(byte value)
        {
            G.Mem.Write(Base.m_dwAttack2, value);
        }

        public void SetJump(byte value)
        {
            G.Mem.Write(Base.m_dwJump, value);
        }

        public void SetDuck(byte value)
        {
            G.Mem.Write(Base.m_dwDuck, value);
        }

        public void SetForward(byte value)
        {
            G.Mem.Write(Base.m_dwForward, value);
        }

        public void SetBackward(byte value)
        {
            G.Mem.Write(Base.m_dwBackward, value);
        }

        public void SetLeft(byte value)
        {
            G.Mem.Write(Base.m_dwLeft, value);
        }

        public void SetRight(byte value)
        {
            G.Mem.Write(Base.m_dwRight, value);
        }

        public void SendClientCmd(string cmd)
        {
            if (cmd.Length > 120)
            {
                cmd = cmd.Substring(0, 120);
            }

            var address = WinApi.VirtualAllocEx(G.Mem.Handle, IntPtr.Zero, 
                (uint)((cmd.Length + 1) * Marshal.SizeOf(typeof(char))), 
                (uint)AllocationType.Commit | (uint)AllocationType.Reserve,
                (uint)Protection.PageReadWrite);

            G.Mem.WriteString(address, cmd, Encoding.Default);

            var threadHandle = WinApi.CreateRemoteThread(G.Mem.Handle, IntPtr.Zero, 0u, Base.m_dwClientCmd, address, 0u, IntPtr.Zero);

            WinApi.WaitForSingleObject(threadHandle, 0xFFFFFFFF);
            WinApi.VirtualFreeEx(G.Mem.Handle, address, 0u, (uint)FreeType.MemRelease);
            WinApi.CloseHandle(threadHandle);
        }
    }
}
