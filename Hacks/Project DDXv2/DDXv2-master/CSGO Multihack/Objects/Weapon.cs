using System;
using System.Diagnostics;
using System.Text;
using DDX.Enums;
using DDX.Extensions;
using DDX.Functions;

namespace DDX.Objects
{
    public class Weapon
    {
        public readonly int Index;
        public HandleMethod HandleMethod;

        private readonly IntPtr _forceHandle;

        public Weapon(int index)
        {
            Index = index;
            HandleMethod = HandleMethod.EntityList;
        }

        public Weapon(IntPtr handle)
        {
            _forceHandle = handle;
            HandleMethod = HandleMethod.ForceHandle;
        }

        private ulong _lastAmmoRef;
        private int _lastAmmo;
        public int GetAmmo(bool force = false)
        {
            if (!force && _lastAmmoRef == G.Ref)
                return _lastAmmo;

            _lastAmmoRef = G.Ref;
            return _lastAmmo = G.Mem.Read<int>(GetHandle() + Offset.m_iClip1);
        }

        private ulong _lastCanShootRef;
        private bool _lastCanShoot;
        public bool GetCanShoot(bool force = false)
        {
            if (!force && _lastCanShootRef == G.Ref)
                return _lastCanShoot;

            _lastCanShootRef = G.Ref;
            return _lastCanShoot = GetId().GetCanShoot();
        }

        private ulong _lastCategoryRef;
        private WeaponCategory _lastCategory;
        public WeaponCategory GetCategory(bool force = false)
        {
            if (!force && _lastCategoryRef == G.Ref)
                return _lastCategory;

            _lastCategoryRef = G.Ref;
            return _lastCategory = GetId().GetCategory();
        }

        private ulong _lastHandleRef;
        private IntPtr _lastHandle;
        public IntPtr GetHandle(bool force = false)
        {
            if (!force && _lastHandleRef == G.Ref)
                return _lastHandle;

            _lastHandleRef = G.Ref;

            if(HandleMethod == HandleMethod.EntityList)
                return _lastHandle = GlobalRead.GetWeaponHandleByIndex(Index);
            return _lastHandle = _forceHandle;
        }

        private ulong _lastHasSkinRef;
        private bool _lastHasSkin;
        public bool GetHasSkin(bool force = false)
        {
            if (!force && _lastHasSkinRef == G.Ref)
                return _lastHasSkin;

            _lastHasSkinRef = G.Ref;
            return _lastHasSkin = GetId().GetHasSkin();
        }

        private ulong _lastIdRef;
        private WeaponId _lastId;
        public WeaponId GetId(bool force = false)
        {
            if (!force && _lastIdRef == G.Ref)
                return _lastId;

            _lastIdRef = G.Ref;
            return _lastId = (WeaponId)G.Mem.Read<int>(GetHandle() + Offset.m_iItemDefinitionIndex);
        }

        private ulong _lastIsRcsRestrictedRef;
        private bool _lastIsRcsRestricted;
        public bool GetIsRcsRestricted(bool force = false)
        {
            if (!force && _lastIsRcsRestrictedRef == G.Ref)
                return _lastIsRcsRestricted;

            _lastIsRcsRestrictedRef = G.Ref;
            return _lastIsRcsRestricted = GetId().GetIsRcsRestricted();
        }

        private ulong _lastInReloadRef;
        private bool _lastInReload;
        public bool GetInReload(bool force = false)
        {
            if (!force && _lastInReloadRef == G.Ref)
                return _lastInReload;

            _lastInReloadRef = G.Ref;
            return _lastInReload = G.Mem.Read<byte>(GetHandle() + Offset.m_bInReload) == 1;
        }

        private ulong _lastNameRef;
        private string _lastName;
        public string GetName(bool force = false)
        {
            if (!force && _lastNameRef == G.Ref)
                return _lastName;

            _lastNameRef = G.Ref;
            return _lastName = GetId().GetName();
        }

        private ulong _lastNextPrimaryAttackRef;
        private float _lastNextPrimaryAttack;
        public float GetNextPrimaryAttack(bool force = false)
        {
            if (!force && _lastNextPrimaryAttackRef == G.Ref)
                return _lastNextPrimaryAttack;

            _lastNextPrimaryAttackRef = G.Ref;
            return _lastNextPrimaryAttack = G.Mem.Read<float>(GetHandle() + Offset.m_flNextPrimaryAttack);
        }

        private ulong _lastXuidRef;
        private int _lastXuid;
        public int GetXuid(bool force = false)
        {
            if (!force && _lastXuidRef == G.Ref)
                return _lastXuid;

            _lastXuidRef = G.Ref;
            return _lastXuid = G.Mem.Read<int>(GetHandle() + Offset.m_OriginalOwnerXuidLow);
        }

        public void SetForceFallbackValues()
        {
            G.Mem.Write(GetHandle() + Offset.m_iItemIDHigh, -1);
        }

        public void SetAccountId(int value)
        {
            G.Mem.Write(GetHandle() + Offset.m_iAccountID, value);
        }

        public void SetEntityQuality(int value)
        {
            G.Mem.Write(GetHandle() + Offset.m_iEntityQuality, value);
        }

        public void SetCustomName(string value)
        {
            var niceName = value.Length <= 32 ? value : value.Substring(0, 32);
            G.Mem.WriteString(GetHandle() + Offset.m_szCustomName, niceName, Encoding.Default);
        }

        public void SetPaintKit(int value)
        {
            G.Mem.Write(GetHandle() + Offset.m_nFallbackPaintKit, value);
        }

        public void SetStatTrak(int value)
        {
            G.Mem.Write(GetHandle() + Offset.m_nFallbackStatTrak, value);
        }

        public void SetWear(float value)
        {
            G.Mem.Write(GetHandle() + Offset.m_flFallbackWear, value);
        }
    }
}
