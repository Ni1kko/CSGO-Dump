using FadeAPI.Enums;
using FadeAPI.Helpers;
using FadeAPI.Memory;
using FadeAPI.Structs;
using System;
using System.Runtime.InteropServices;
using System.Text;

namespace FadeAPI.Objects
{
    public class Player
    {
        public readonly int Index;

        public Player(int index)
        {
            Index = index;
        }

        private int _activeWeaponRead
        {
            get
            {
                var weaponHandleId = BitConverter.ToInt32(Global.Memory.ReadBytes(Handle + MemoryOffset.ActiveWeapon, 4), 0) & 0xFFF;
                var weaponHandle = (IntPtr)BitConverter.ToInt32(Global.Memory.ReadBytes(MemoryBase.ClientDll + MemoryBase.EntityList.ToInt32() + (weaponHandleId - 1) * 0x10, 4), 0);
                var weapon = BitConverter.ToInt32(Global.Memory.ReadBytes(weaponHandle + MemoryOffset.ItemDefinitionIndex, 4), 0);

                return weapon;
            }
        }
        private int _activeWeaponReadLast;
        private ulong _activeWeaponReadLastId;
        public int ActiveWeapon
        {
            get
            {
                if (_activeWeaponReadLastId < Global.RefreshId)
                {
                    _activeWeaponReadLastId = Global.RefreshId;
                    _activeWeaponReadLast = _activeWeaponRead;
                }

                return _activeWeaponReadLast;
            }
        }

        private float _distanceRead => Functions.VectorDistance(Global.LocalPlayer.ViewOrigin, Origin);
        private float _distanceReadLast;
        private ulong _distanceReadLastId;
        public float Distance
        {
            get
            {
                if (_distanceReadLastId < Global.RefreshId)
                {
                    _distanceReadLastId = Global.RefreshId;
                    _distanceReadLast = _distanceRead;
                }

                return _distanceReadLast;
            }
        }

        private float _distanceScaleRead => 1 / Distance * 200 * Global.LocalPlayer.ScopeScale;
        private float _distanceScaleReadLast;
        private ulong _distanceScaleReadLastId;
        public float DistanceScale
        {
            get
            {
                if (_distanceScaleReadLastId < Global.RefreshId)
                {
                    _distanceScaleReadLastId = Global.RefreshId;
                    _distanceScaleReadLast = _distanceScaleRead;
                }

                return _distanceScaleReadLast;
            }
        }

        public void SetGlowColor(GlowColor gc, bool fullBloom = false)
        {
            //[swap]
            Global.Memory.WriteBytes(GlobalRead.GlowHandle + GlowIndex * 0x38 + 0x24, new byte[] { 0x1, 0x0, fullBloom ? (byte)0x1 : (byte)0x0 });
            Global.Memory.WriteBytes(GlobalRead.GlowHandle + GlowIndex * 0x38 + 0x4, StructHelper.ToByteArray(gc));
            //[/swap]
        }

        private int _glowIndexRead => BitConverter.ToInt32(Global.Memory.ReadBytes(Handle + MemoryOffset.GlowIndex, 4), 0);
        private int _glowIndexReadLast;
        private ulong _glowIndexReadLastId;
        private int GlowIndex
        {
            get
            {
                if (_glowIndexReadLastId < Global.RefreshId)
                {
                    _glowIndexReadLastId = Global.RefreshId;
                    _glowIndexReadLast = _glowIndexRead;
                }

                return _glowIndexReadLast;
            }
        }

        private IntPtr _handleRead => (IntPtr)BitConverter.ToInt32(Global.Memory.ReadBytes(MemoryBase.ClientDll + MemoryBase.EntityList.ToInt32() + Index * 0x10, 4), 0);
        private IntPtr _handleReadLast;
        private ulong _handleReadLastId;
        public IntPtr Handle
        {
            get
            {
                if (_handleReadLastId < Global.RefreshId)
                {
                    _handleReadLastId = Global.RefreshId;
                    _handleReadLast = _handleRead;
                }

                return _handleReadLast;
            }
        }

        private bool _hasDefuserRead => BitConverter.ToBoolean(Global.Memory.ReadBytes(Handle + MemoryOffset.HasDefuser, 1), 0);
        private bool _hasDefuserReadLast;
        private ulong _hasDefuserReadLastId;
        public bool HasDefuser
        {
            get
            {
                if (_hasDefuserReadLastId < Global.RefreshId)
                {
                    _hasDefuserReadLastId = Global.RefreshId;
                    _hasDefuserReadLast = _hasDefuserRead;
                }

                return _hasDefuserReadLast;
            }
        }

        private bool _hasHelmetRead => Convert.ToBoolean(BitConverter.ToInt32(Global.Memory.ReadBytes(Handle + MemoryOffset.HasHelmet, 4), 0));
        private bool _hasHelmetReadLast;
        private ulong _hasHelmetReadLastId;
        public bool HasHelmet
        {
            get
            {
                if (_hasHelmetReadLastId < Global.RefreshId)
                {
                    _hasHelmetReadLastId = Global.RefreshId;
                    _hasHelmetReadLast = _hasHelmetRead;
                }

                return _hasHelmetReadLast;
            }
        }

        private bool _hasHostageRead => BitConverter.ToBoolean(Global.Memory.ReadBytes(Handle + MemoryOffset.IsGrabbingHostage, 1), 0);
        private bool _hasHostageReadLast;
        private ulong _hasHostageReadLastId;
        public bool HasHostage
        {
            get
            {
                if (_hasHostageReadLastId < Global.RefreshId)
                {
                    _hasHostageReadLastId = Global.RefreshId;
                    _hasHostageReadLast = _hasHostageRead;
                }

                return _hasHostageReadLast;
            }
        }

        private bool _hasImmunityRead => BitConverter.ToBoolean(Global.Memory.ReadBytes(Handle + MemoryOffset.GunGameImmunity, 1), 0);
        private bool _hasImmunityReadLast;
        private ulong _hasImmunityReadLastId;
        public bool HasImmunity
        {
            get
            {
                if (_hasImmunityReadLastId < Global.RefreshId)
                {
                    _hasImmunityReadLastId = Global.RefreshId;
                    _hasImmunityReadLast = _hasImmunityRead;
                }

                return _hasImmunityReadLast;
            }
        }

        private int _healthRead => BitConverter.ToInt32(Global.Memory.ReadBytes(Handle + MemoryOffset.Health, 4), 0);
        private int _healthReadLast;
        private ulong _healthReadLastId;
        public int Health
        {
            get
            {
                if (_healthReadLastId < Global.RefreshId)
                {
                    _healthReadLastId = Global.RefreshId;
                    _healthReadLast = _healthRead;
                }

                return _healthReadLast;
            }
        }

        private bool _isAllayRead => Team == Global.LocalPlayer.Team;
        private bool _isAllayReadLast;
        private ulong _isAllayReadLastId;
        public bool IsAllay
        {
            get
            {
                if (_isAllayReadLastId < Global.RefreshId)
                {
                    _isAllayReadLastId = Global.RefreshId;
                    _isAllayReadLast = _isAllayRead;
                }

                return _isAllayReadLast;
            }
        }

        private bool _isDefusingRead => BitConverter.ToBoolean(Global.Memory.ReadBytes(Handle + MemoryOffset.IsDefusing, 1), 0);
        private bool _isDefusingReadLast;
        private ulong _isDefusingReadLastId;
        public bool IsDefusing
        {
            get
            {
                if (_isDefusingReadLastId < Global.RefreshId)
                {
                    _isDefusingReadLastId = Global.RefreshId;
                    _isDefusingReadLast = _isDefusingRead;
                }

                return _isDefusingReadLast;
            }
        }

        private byte _isDormantRead => Global.Memory.ReadBytes(Handle + MemoryOffset.Dormant, 1)[0];
        private bool _isDormantReadLast;
        private ulong _isDormantReadLastId;
        public bool IsDormant
        {
            get
            {
                if (_isDormantReadLastId < Global.RefreshId)
                {
                    _isDormantReadLastId = Global.RefreshId;
                    _isDormantReadLast = _isDormantRead != 0;
                }

                return _isDormantReadLast;
            }
        }

        private byte _isGlowingRead => Global.Memory.ReadBytes(GlobalRead.GlowHandle + GlowIndex * 0x38 + 0x24, 1)[0];
        private bool _isGlowingReadLast;
        private ulong _isGlowingReadLastId;
        public bool IsGlowing
        {
            get
            {
                if (_isGlowingReadLastId < Global.RefreshId)
                {
                    _isGlowingReadLastId = Global.RefreshId;
                    _isGlowingReadLast = _isGlowingRead != 0;
                }

                return _isGlowingReadLast;
            }
        }

        private bool _isSpectatedByLocalPlayerRead
        {
            get
            {
                if (Global.LocalPlayer.Health != 0)
                    return false;

                return Functions.VectorDistance(Global.LocalPlayer.Origin, Origin) < 50;
            }
        }
        private bool _isSpectatedByLocalPlayerReadLast;
        private ulong _isSpectatedByLocalPlayergReadLastId;
        public bool IsSpectatedByLocalPlayer
        {
            get
            {
                if (_isSpectatedByLocalPlayergReadLastId < Global.RefreshId)
                {
                    _isSpectatedByLocalPlayergReadLastId = Global.RefreshId;
                    _isSpectatedByLocalPlayerReadLast = _isSpectatedByLocalPlayerRead;
                }

                return _isSpectatedByLocalPlayerReadLast;
            }
        }

        public bool IsSpotted
        {
            set
            {
                var buffer = BitConverter.GetBytes(value);
                Global.Memory.WriteBytes(Handle + MemoryOffset.Spotted, buffer);
            }
        }

        private bool _isVisibleRead
        {
            get
            {
                var binary = Convert.ToString(SpottedMask, 2);
                var binaryChars = binary.ToCharArray();

                if (binaryChars.Length - 1 < Global.LocalPlayerIndex)
                    return false;

                Array.Reverse(binaryChars);
                return binaryChars[Global.LocalPlayerIndex] == '1';
            }
        }
        private bool _isVisibleReadLast;
        private ulong _isVisibleReadLastId;
        public bool IsVisible
        {
            get
            {
                if (_isVisibleReadLastId < Global.RefreshId)
                {
                    _isVisibleReadLastId = Global.RefreshId;
                    _isVisibleReadLast = _isVisibleRead;
                }

                return _isVisibleReadLast;
            }
        }

        private string _readModelName
        {
            get
            {
                var modelHandle = (IntPtr)BitConverter.ToInt32(Global.Memory.ReadBytes(Handle + MemoryOffset.Model, 4), 0);
                var model = Global.Memory.ReadString(modelHandle + 0x4, 64);
                var split1 = model.Split(new[] { ".mdl" }, StringSplitOptions.None)[0];
                var split2Array = split1.Split('/');
                var split2 = split2Array[split2Array.Length - 1];
                var split3Array = split2.Split('_');
                model = split3Array[0] + '_' + split3Array[1];
                return model;
            }
        }
        private string _readModelNameLast;
        private ulong _readModelNameLastId;
        public PlayerModel ModelObject
        {
            get
            {
                if (_readModelNameLastId < Global.RefreshId)
                {
                    _readModelNameLastId = Global.RefreshId;
                    _readModelNameLast = _readModelName;
                }

                return new PlayerModel(Handle, _readModelNameLast);
            }
        }

        private string _readName
        {
            get
            {
                var nameCharArray = Encoding.Unicode.GetString(Global.Memory.ReadBytes(GlobalRead.RadarHandle + (Index + 1) * 0x1E0 + MemoryOffset.Name, 32)).ToCharArray();
                var name = string.Empty;
                var lastEmpty = false;

                foreach (var nameChar in nameCharArray)
                {
                    if (nameChar == '\0')
                    {
                        if (lastEmpty)
                            break;

                        lastEmpty = true;
                        continue;
                    }

                    lastEmpty = false;
                    name += nameChar;
                }

                return name;
            }
        }
        private string _readNameLast;
        private ulong _readNameLastId;
        public string Name
        {
            get
            {
                if (_readNameLastId < Global.RefreshId)
                {
                    _readNameLastId = Global.RefreshId;
                    _readNameLast = _readName;
                }

                return _readNameLast;
            }
        }

        private IntPtr _observeHandleRead => (IntPtr)BitConverter.ToInt32(Global.Memory.ReadBytes(Handle + MemoryOffset.ObserverTarget, 4), 0);
        private IntPtr _observeHandleReadLast;
        private ulong _observeHandleReadLastId;
        public IntPtr ObserveHandle
        {
            get
            {
                if (_observeHandleReadLastId < Global.RefreshId)
                {
                    _observeHandleReadLastId = Global.RefreshId;
                    _observeHandleReadLast = _observeHandleRead;
                }

                return _observeHandleReadLast;
            }
        }

        private Vector3 _originRead => StructHelper.ToObject<Vector3>(Global.Memory.ReadBytes(Handle + MemoryOffset.VecOrigin, Marshal.SizeOf(new Vector3())));
        private Vector3 _originReadLast;
        private ulong _originReadLastId;
        public Vector3 Origin
        {
            get
            {
                if (_originReadLastId < Global.RefreshId)
                {
                    _originReadLastId = Global.RefreshId;
                    _originReadLast = _originRead;
                }

                return _originReadLast;
            }
        }

        private int _rankRead => BitConverter.ToInt32(Global.Memory.ReadBytes(GlobalRead.PlayerResource + MemoryOffset.CompetitiveRanking + Index * 0x4, 4), 0);
        private int _rankReadLast;
        private ulong _rankReadLastId;
        public int Rank
        {
            get
            {
                if (_rankReadLastId < Global.RefreshId)
                {
                    _rankReadLastId = Global.RefreshId;
                    _rankReadLast = _rankRead;
                }

                return _rankReadLast;
            }
        }

        private float _rawDistanceRead => Functions.VectorDistance(Global.LocalPlayer.Origin, Origin);
        private float _rawDistanceReadLast;
        private ulong _rawDistanceReadLastId;
        public float RawDistance
        {
            get
            {
                if (_rawDistanceReadLastId < Global.RefreshId)
                {
                    _rawDistanceReadLastId = Global.RefreshId;
                    _rawDistanceReadLast = _rawDistanceRead;
                }

                return _rawDistanceReadLast;
            }
        }

        public RenderColor RenderColor
        {
            set
            {
                //[swap]
                Global.Memory.WriteBytes(Handle + MemoryOffset.ColorRender, StructHelper.ToByteArray(value));
                //[/swap]
            }
        }

        private Vector2 _screenOriginRead => Functions.WorldToScreen(Origin, GlobalRead.ViewMatrix);
        private Vector2 _screenOriginReadLast;
        private ulong _screenOriginReadLastId;
        public Vector2 ScreenOrigin
        {
            get
            {
                if (_screenOriginReadLastId < Global.RefreshId)
                {
                    _screenOriginReadLastId = Global.RefreshId;
                    _screenOriginReadLast = _screenOriginRead;
                }

                return _screenOriginReadLast;
            }
        }

        private int _spottedMaskRead => BitConverter.ToInt32(Global.Memory.ReadBytes(Handle + MemoryOffset.SpottedByMask, 4), 0);
        private int _spottedMaskReadLast;
        private ulong _spottedMaskReadLastId;
        private int SpottedMask
        {
            get
            {
                if (_spottedMaskReadLastId < Global.RefreshId)
                {
                    _spottedMaskReadLastId = Global.RefreshId;
                    _spottedMaskReadLast = _spottedMaskRead;
                }

                return _spottedMaskReadLast;
            }
        }

        private byte _teamRead => Global.Memory.ReadBytes(Handle + MemoryOffset.TeamNum, 1)[0];
        private byte _teamReadLast;
        private ulong _teamReadLastId;
        public TeamNum Team
        {
            get
            {
                if (_teamReadLastId < Global.RefreshId)
                {
                    _teamReadLastId = Global.RefreshId;
                    _teamReadLast = _teamRead;
                }

                return (TeamNum)_teamReadLast;
            }
        }
    }
}
