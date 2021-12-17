using System;
using System.Diagnostics;
using System.Text;
using DDX.Enums;
using DDX.Extensions;
using DDX.Functions;
using DDX.Structs;

namespace DDX.Objects
{
    public class Player : Entity
    {
        public Player() : base()
        {
        }

        public Player(int index, IntPtr handle) : base(index, handle)
        {
        }

        public Player(int index, IntPtr handle, IntPtr glowHandle) : base(index, handle, glowHandle)
        {
        }

        private ulong _lastActiveWeaponRef;
        private Weapon _lastActiveWeapon;
        public Weapon GetActiveWeapon(bool force = false)
        {
            if (!force && _lastActiveWeaponRef == G.Ref)
                return _lastActiveWeapon;

            _lastActiveWeaponRef = G.Ref;
            return _lastActiveWeapon = new Weapon(GetActiveWeaponHandle());
        }

        private ulong _lastActiveWeaponIndexRef;
        private int _lastActiveWeaponIndex;
        public int GetActiveWeaponIndex(bool force = false)
        {
            if (!force && _lastActiveWeaponIndexRef == G.Ref)
                return _lastActiveWeaponIndex;

            _lastActiveWeaponIndexRef = G.Ref;
            return _lastActiveWeaponIndex = G.Mem.Read<int>(Handle + Offset.m_hActiveWeapon) & 0xFFF;
        }

        private ulong _lastActiveWeaponHandleRef;
        private IntPtr _lastActiveWeaponHandle;
        public IntPtr GetActiveWeaponHandle(bool force = false)
        {
            if (!force && _lastActiveWeaponHandleRef == G.Ref)
                return _lastActiveWeaponHandle;

            var temp = GlobalRead.GetEntityHandleByIndex(GetActiveWeaponIndex());
            if (temp == IntPtr.Zero)
                return _lastActiveWeaponHandle;

            _lastActiveWeaponHandleRef = G.Ref;
            return _lastActiveWeaponHandle = temp;
        }

        private ulong _lastArmorRef;
        private int _lastArmor;
        public int GetArmor(bool force = false)
        {
            if (!force && _lastArmorRef == G.Ref)
                return _lastArmor;

            _lastArmorRef = G.Ref;
            return _lastArmor = G.Mem.Read<int>(Handle + Offset.m_ArmorValue);
        }

        private ulong _lastHasDefuserRef;
        private bool _lastHasDefuser;
        public bool GetHasDefuser(bool force = false)
        {
            if (!force && _lastHasDefuserRef == G.Ref)
                return _lastHasDefuser;

            _lastHasDefuserRef = G.Ref;
            return _lastHasDefuser = G.Mem.Read<byte>(Handle + Offset.m_bHasDefuser) == 1;
        }

        private ulong _lastHasHostageRef;
        private bool _lastHasHostage;
        public bool GetHasHostage(bool force = false)
        {
            if (!force && _lastHasHostageRef == G.Ref)
                return _lastHasHostage;

            _lastHasHostageRef = G.Ref;
            return _lastHasHostage = G.Mem.Read<byte>(Handle + Offset.m_bIsGrabbingHostag) == 1;
        }

        private ulong _lastHasImmunityRef;
        private bool _lastHasImmunity;
        public bool GetHasImmunity(bool force = false)
        {
            if (!force && _lastHasImmunityRef == G.Ref)
                return _lastHasImmunity;

            _lastHasImmunityRef = G.Ref;
            return _lastHasImmunity = G.Mem.Read<byte>(Handle + Offset.m_bGunGameImmunity) == 1;
        }

        private ulong _lastHealthRef;
        private int _lastHealth;
        public int GetHealth(bool force = false)
        {
            if (!force && _lastHealthRef == G.Ref)
                return _lastHealth;

            _lastHealthRef = G.Ref;
            return _lastHealth = G.Mem.Read<int>(Handle + Offset.m_iHealth);
        }

        private ulong _lastIsAliveRef;
        private bool _lastIsAlive;
        public bool GetIsAlive(bool force = false)
        {
            if (!force && _lastIsAliveRef == G.Ref)
                return _lastIsAlive;

            _lastIsAliveRef = G.Ref;
            return _lastIsAlive = GetLifeState() == LifeState.Alive && GetHealth() > 0;
        }

        private ulong _lastIsAllyRef;
        private bool _lastIsAlly;
        public bool GetIsAlly(bool force = false)
        {
            if (!force && _lastIsAllyRef == G.Ref)
                return _lastIsAlly;

            _lastIsAllyRef = G.Ref;
            return _lastIsAlly = GetTeam() == G.Local.GetTeam() && !G.C.Stts.FriendlyFire;
        }

        private ulong _lastIsDefusingRef;
        private bool _lastIsDefusing;
        public bool GetIsDefusing(bool force = false)
        {
            if (!force && _lastIsDefusingRef == G.Ref)
                return _lastIsDefusing;

            _lastIsDefusingRef = G.Ref;
            return _lastIsDefusing = G.Mem.Read<byte>(Handle + Offset.m_bIsDefusing) == 1;
        }

        private ulong _lastIsOnGroundRef;
        private bool _lastIsOnGround;
        public bool GetIsOnGround(bool force = false)
        {
            if (!force && _lastIsOnGroundRef == G.Ref)
                return _lastIsOnGround;

            _lastIsOnGroundRef = G.Ref;

            var flags = (Flags)G.Mem.Read<int>(Handle + Offset.m_fFlags);
            return _lastIsOnGround = flags != Flags.InAirStand && flags != Flags.InAirCrouched;
        }

        private ulong _lastIsVisibleRef;
        private bool _lastIsVisible;
        public bool GetIsVisible(bool force = false)
        {
            if (!force && _lastIsVisibleRef == G.Ref)
                return _lastIsVisible;

            _lastIsVisibleRef = G.Ref;

            var binary = Convert.ToString(GetSpottedMask(), 2);
            var binaryChars = binary.ToCharArray();

            var localIndex = G.Local.Index - 1;

            if (binaryChars.Length <= localIndex)
                return false;

            Array.Reverse(binaryChars);
            return _lastIsVisible = binaryChars[localIndex] == '1';
        }

        public bool GetIsVisible(Vector3 v, bool force = false)
        {
            if (!force && _lastIsVisibleRef == G.Ref)
                return _lastIsVisible;

            if (G.Bsp == null)
            {
                return false;
            }

            _lastIsVisibleRef = G.Ref;
            return _lastIsVisible = G.Bsp.IsVisible(G.Local.GetViewOrigin(), v);
        }

        private ulong _lastLifeStateRef;
        private LifeState _lastLifeState;
        public LifeState GetLifeState(bool force = false)
        {
            if (!force && _lastLifeStateRef == G.Ref)
                return _lastLifeState;

            _lastLifeStateRef = G.Ref;
            return _lastLifeState = (LifeState)G.Mem.Read<byte>(Handle + Offset.m_lifeState);
        }

        private ulong _lastNameRef;
        private string _lastName;
        public string GetName(bool force = false)
        {
            if (!force && _lastNameRef == G.Ref)
                return _lastName;

            _lastNameRef = G.Ref;

            var rawName = G.Mem.ReadString(GlobalRead.GetRadarBaseHandle() + Index * 0x1E0 + Offset.m_szName, Encoding.Unicode);
            return _lastName = rawName.CutNull();
        }

        private ulong _lastPlaceNameRef;
        private string _lastPlaceName;
        public string GetPlaceName(bool force = false)
        {
            if (!force && _lastPlaceNameRef == G.Ref)
                return _lastPlaceName;

            _lastPlaceNameRef = G.Ref;

            var placeRaw = G.Mem.ReadString(Handle + Offset.m_szLastPlaceName, Encoding.Default, 18);
            return _lastPlaceName = placeRaw.CutNull();
        }

        private ulong _lastPlayerModelRef;
        private Model _lastPlayerModel;
        public Model GetPlayerModel(bool force = false)
        {
            if (!force && _lastPlayerModelRef == G.Ref)
                return _lastPlayerModel;

            _lastPlayerModelRef = G.Ref;

            var modelHandle = G.Mem.Read<IntPtr>(Handle + Offset.m_dwModel);
            if (modelHandle == IntPtr.Zero)
            {
                return new Model(IntPtr.Zero, string.Empty);
            }

            var modelName = G.Mem.ReadString(modelHandle + 0x4, Encoding.Default, 64);

            var fin1 = modelName.Remove(modelName.IndexOf(".mdl", StringComparison.CurrentCulture));
            var fin2 = fin1.Substring(fin1.LastIndexOf('/') + 1);
            var index1 = fin2.IndexOf('_');
            var index2 = fin2.IndexOf('_', index1 + 1);
            var fin3 = index2 != -1 ? fin2.Remove(index2) : fin2;
            modelName = fin3;

            return _lastPlayerModel = new Model(Handle, modelName);
        }

        private ulong _lastRankIdRef;
        private RankId _lastRankId;
        public RankId GetRankId(bool force = false)
        {
            if (!force && _lastRankIdRef == G.Ref)
                return _lastRankId;

            _lastRankIdRef = G.Ref;
            return _lastRankId = (RankId)G.Mem.Read<int>(GlobalRead.GetPlayerResourceHandle() + Offset.m_iCompetitiveRanking + Index * 0x4);
        }

        private ulong _lastObserverModeRef;
        private ObserverMode _lastObserverMode;
        public ObserverMode GetObserverMode(bool force = false)
        {
            if (!force && _lastObserverModeRef == G.Ref)
                return _lastObserverMode;

            _lastObserverModeRef = G.Ref;
            return _lastObserverMode = (ObserverMode)G.Mem.Read<int>(Handle + Offset.m_iObserverMode);
        }

        private ulong _lastObserverPlayerRef;
        private Player _lastObserverPlayer;
        public Player GetObserverPlayer(bool force = false)
        {
            if (!force && _lastObserverPlayerRef == G.Ref)
                return _lastObserverPlayer;

            _lastObserverPlayerRef = G.Ref;

            if (GetIsAlive())
            {
                return _lastObserverPlayer = this;
            }
            else
            {
                var index = G.Local.GetObserverTargetIndex();
                return _lastObserverPlayer = new Player(index, GlobalRead.GetEntityHandleByIndex(index));
            }
        }

        private ulong _lastObserverTargetIndexRef;
        private int _lastObserverTargetIndex;
        public int GetObserverTargetIndex(bool force = false)
        {
            if (!force && _lastObserverTargetIndexRef == G.Ref)
                return _lastObserverTargetIndex;

            _lastObserverTargetIndexRef = G.Ref;
            return _lastObserverTargetIndex = G.Mem.Read<int>(Handle + Offset.m_hObserverTarget) & 0xFFF;
        }

        private ulong _lastSpottedMaskRef;
        private int _lastSpottedMask;
        public int GetSpottedMask(bool force = false)
        {
            if (!force && _lastSpottedMaskRef == G.Ref)
                return _lastSpottedMask;

            _lastSpottedMaskRef = G.Ref;
            return _lastSpottedMask = G.Mem.Read<int>(Handle + Offset.m_bSpottedByMask);
        }

        private ulong _lastTeamRef;
        private Team _lastTeam;
        public Team GetTeam(bool force = false)
        {
            if (!force && _lastTeamRef == G.Ref)
                return _lastTeam;

            _lastTeamRef = G.Ref;
            _lastTeam = (Team)G.Mem.Read<byte>(Handle + Offset.m_iTeamNum);

            Debug.Assert(_lastTeam != Team.Default);

            return _lastTeam;
        }

        public void SetSpotted(bool spotted)
        {
            G.Mem.Write(Handle + Offset.m_bSpotted, spotted ? (byte)0x1 : (byte)0x0);
        }
    }
}
