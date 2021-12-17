using FadeAPI.Enums;
using FadeAPI.Helpers;
using FadeAPI.Memory;
using FadeAPI.Objects;
using FadeAPI.Structs;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace FadeAPI.LocalPlayer
{
    public class LocalPlayerCore
    {
        public LocalPlayerExecute Execute { get; }

        public LocalPlayerCore()
        {
            //[swap]
            Execute = new LocalPlayerExecute();
            Global.LocalPlayer = this;
            //[/swap]
        }

        private int _activeWeaponRead => BitConverter.ToInt32(Global.Memory.ReadBytes(ActiveWeaponHandle + MemoryOffset.ItemDefinitionIndex, 4), 0);
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

        private IntPtr _activeWeaponHandleRead
        {
            get
            {
                var weaponHandleId = BitConverter.ToInt32(Global.Memory.ReadBytes(Handle + MemoryOffset.ActiveWeapon, 4), 0) & 0xFFF;
                var weaponHandle = (IntPtr)BitConverter.ToInt32(Global.Memory.ReadBytes(MemoryBase.ClientDll + MemoryBase.EntityList.ToInt32() + (weaponHandleId - 1) * 0x10, 4), 0);

                return weaponHandle;
            }
        }
        private IntPtr _activeWeaponHandleReadLast;
        private ulong _activeWeaponHandleReadLastId;
        public IntPtr ActiveWeaponHandle
        {
            get
            {
                if (_activeWeaponHandleReadLastId < Global.RefreshId)
                {
                    _activeWeaponHandleReadLastId = Global.RefreshId;
                    _activeWeaponHandleReadLast = _activeWeaponHandleRead;
                }

                return _activeWeaponHandleReadLast;
            }
        }

        private byte _inGameRead => Global.Memory.ReadBytes(GlobalRead.ClientState + MemoryOffset.InGame, 1)[0];
        private byte _inGameReadLast;
        private ulong _inGameReadLastId;
        public bool InGame
        {
            get
            {
                if (_inGameReadLastId < Global.RefreshId)
                {
                    _inGameReadLastId = Global.RefreshId;
                    _inGameReadLast = _inGameRead;
                }

                return _inGameReadLast == 6 || _inGameReadLast == 7 ? true : false;
            }
        }

        private int _crosshairIdRead => BitConverter.ToInt32(Global.Memory.ReadBytes(Handle + MemoryOffset.CrosshairId, 4), 0);
        private int _crosshairIdReadLast;
        private ulong _crosshairIdReadLastId;
        public Player CrosshairPlayer
        {
            get
            {
                if (_crosshairIdReadLastId < Global.RefreshId)
                {
                    _crosshairIdReadLastId = Global.RefreshId;
                    _crosshairIdReadLast = _crosshairIdRead;
                }

                return new Player(_crosshairIdReadLast - 1);
            }
        }

        private byte _flagsRead => Global.Memory.ReadBytes(Handle + MemoryOffset.Flags, 1)[0];
        private byte _flagsReadLast;
        private ulong _flagsReadLastId;
        public Flags Flags
        {
            get
            {
                if (_flagsReadLastId < Global.RefreshId)
                {
                    _flagsReadLastId = Global.RefreshId;
                    _flagsReadLast = _flagsRead;
                }

                return _flagsReadLast == 0 || _flagsReadLast == 6 ? Flags.InAir : Flags.OnGround;
            }
        }

        private float _flashAlphaRead => BitConverter.ToSingle(Global.Memory.ReadBytes(Handle + MemoryOffset.FlashMaxAlpha, 4), 0);
        private float _flashAlphaReadLast;
        private ulong _flashAlphaReadLastId;
        public float FlashAlpha
        {
            get
            {
                if (_flashAlphaReadLastId < Global.RefreshId)
                {
                    _flashAlphaReadLastId = Global.RefreshId;
                    _flashAlphaReadLast = _flashAlphaRead;
                }

                return _flashAlphaReadLast;
            }
            set
            {
                if (value > 255f)
                    value = 255f;
                else if (value < 0f)
                    value = 0f;

                Global.Memory.WriteBytes(Handle + MemoryOffset.FlashMaxAlpha, BitConverter.GetBytes(value));
            }
        }

        public void ForceFullUpdate()
        {
            var buffer = BitConverter.GetBytes(-1);
            var flag = Global.Memory.ReadBytes(GlobalRead.ClientState + MemoryOffset.ForceFullUpdate, 4) != buffer;

            if(flag)
                Global.Memory.WriteBytes(GlobalRead.ClientState + MemoryOffset.ForceFullUpdate, buffer);
        }

        private int _fovRead => BitConverter.ToInt32(Global.Memory.ReadBytes(Handle + MemoryOffset.Fov, 4), 0);
        private int _fovReadLast;
        private ulong _fovReadLastId;
        public int Fov
        {
            get
            {
                if (_fovReadLastId < Global.RefreshId)
                {
                    _fovReadLastId = Global.RefreshId;
                    _fovReadLast = _fovRead;
                }

                return _fovReadLast;
            }
            set
            {
                if (value < 1)
                    value = 1;
                else if (value > 179)
                    value = 179;

                var buffer = BitConverter.GetBytes(value);
                Global.Memory.WriteBytes(Handle + MemoryOffset.Fov, buffer);
            }
        }

        private IntPtr _handleRead => (IntPtr)BitConverter.ToInt32(Global.Memory.ReadBytes(MemoryBase.ClientDll + MemoryBase.LocalPlayer.ToInt32(), 4), 0);
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

        private int _killsRead => BitConverter.ToInt32(Global.Memory.ReadBytes(GlobalRead.PlayerResource + MemoryOffset.Kills + (Global.LocalPlayerIndex + 1) * 0x4, 4), 0);
        private int _killsReadLast;
        private ulong _killsReadLastId;
        public int Kills
        {
            get
            {
                if (_killsReadLastId < Global.RefreshId)
                {
                    _killsReadLastId = Global.RefreshId;
                    _killsReadLast = _killsRead;
                }

                return _killsReadLast;
            }
        }

        public List<Weapon> MyWeapons
        {
            get
            {
                var returnList = new List<Weapon>(); 

                for (int i = 0; i < 64; i++)
                    returnList.Add(new Weapon(i));

                return returnList;
            }
        }

        private int _shotsFiredRead => BitConverter.ToInt32(Global.Memory.ReadBytes(Handle + MemoryOffset.ShotsFired, 4), 0);
        private int _shotsFiredReadLast;
        private ulong _shotsFiredReadId;
        public int ShotsFired
        {
            get
            {
                if (_shotsFiredReadId < Global.RefreshId)
                {
                    _shotsFiredReadId = Global.RefreshId;
                    _shotsFiredReadLast = _shotsFiredRead;
                }

                return _shotsFiredReadLast;
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

        private Vector2 _punchRead => StructHelper.ToObject<Vector2>(Global.Memory.ReadBytes(Handle + MemoryOffset.VecPunch, Marshal.SizeOf(new Vector2())));
        private Vector2 _punchReadLast;
        private ulong _punchReadLastId;
        public Vector2 GetPunch(bool forceUpdate = false)
        {
            if (forceUpdate || _punchReadLastId < Global.RefreshId)
            {
                _punchReadLastId = Global.RefreshId;
                _punchReadLast = _punchRead;
            }

            return _punchReadLast;
        }

        private bool _scopeScaleRead => BitConverter.ToBoolean(Global.Memory.ReadBytes(Handle + MemoryOffset.IsScoped, 1), 0);
        private bool _scopeScaleReadLast;
        private ulong _scopeScaleReadLastId;
        public float ScopeScale
        {
            get
            {
                if (_scopeScaleReadLastId < Global.RefreshId)
                {
                    _scopeScaleReadLastId = Global.RefreshId;
                    _scopeScaleReadLast = _scopeScaleRead;
                }

                if (!_scopeScaleReadLast)
                    return 1;

                return ZoomLevel == 1 ? 2.5f : 12;
            }
        }

        private bool _sendPacketsRead => Global.Memory.ReadBytes(MemoryBase.EngineDll + MemoryBase.SendPackets.ToInt32(), 1)[0] == 1;
        private bool _sendPacketsReadLast;
        private ulong _sendPacketsReadLastId;
        public bool SendPackets
        {
            get
            {
                if (_sendPacketsReadLastId < Global.RefreshId)
                {
                    _sendPacketsReadLastId = Global.RefreshId;
                    _sendPacketsReadLast = _sendPacketsRead;
                }

                return _sendPacketsReadLast;
            }
            set
            {
                var vbyte = value ? (byte)1 : (byte)0;
                Global.Memory.WriteBytes(MemoryBase.EngineDll + MemoryBase.SendPackets.ToInt32(), new [] { vbyte });
            }
        }

        private float _speedRead
        {
            get
            {
                var predictOrigin = Origin + Velocity;
                var speed = Functions.VectorDistance(Origin, predictOrigin);

                return speed;
            }
        }
        private float _speedReadLast;
        private ulong _speedReadLastId;
        public float Speed
        {
            get
            {
                if (_speedReadLastId < Global.RefreshId)
                {
                    _speedReadLastId = Global.RefreshId;
                    _speedReadLast = _speedRead;
                }

                return _speedReadLast;
            }
        }

        private Vector3 _velocityRead => StructHelper.ToObject<Vector3>(Global.Memory.ReadBytes(Handle + MemoryOffset.Velocity, Marshal.SizeOf(new Vector3())));
        private Vector3 _velocityReadLast;
        private ulong _velocityLastId;
        public Vector3 Velocity
        {
            get
            {
                if (_velocityLastId < Global.RefreshId)
                {
                    _velocityLastId = Global.RefreshId;
                    _velocityReadLast = _velocityRead;
                }

                return _velocityReadLast;
            }
        }

        private Vector2 _readViewAngles => StructHelper.ToObject<Vector2>(Global.Memory.ReadBytes(GlobalRead.ClientState + MemoryOffset.ViewAngles, Marshal.SizeOf(new Vector2())));
        private Vector2 _readViewAnglesLast;
        private ulong _readViewAnglesLastId;
        public Vector2 ViewAngles
        {
            get
            {
                if (_readViewAnglesLastId < Global.RefreshId)
                {
                    _readViewAnglesLastId = Global.RefreshId;
                    _readViewAnglesLast = _readViewAngles;
                }

                return _readViewAnglesLast;
            }
            set
            {
                if (float.IsNaN(value.X) || float.IsNaN(value.Y) || float.IsInfinity(value.X) || float.IsInfinity(value.Y))
                    return;

                value = Functions.NormalizeAngles(value);

                var angles = new Vector3(value.X, value.Y, 0f);
                Global.Memory.WriteBytes(GlobalRead.ClientState + MemoryOffset.ViewAngles, StructHelper.ToByteArray(angles));
            }
        }

        private Vector3 _readViewOrigin
        {
            get
            {
                var viewOffset = StructHelper.ToObject<Vector3>(Global.Memory.ReadBytes(Handle + MemoryOffset.VecViewOffset, Marshal.SizeOf(new Vector3())));
                return Origin + viewOffset;
            }
        }
        private Vector3 _readViewOriginLast;
        private ulong _readViewOriginLastId;
        public Vector3 ViewOrigin
        {
            get
            {
                if (_readViewOriginLastId < Global.RefreshId)
                {
                    _readViewOriginLastId = Global.RefreshId;
                    _readViewOriginLast = _readViewOrigin;
                }

                return _readViewOriginLast;
            }
        }

        private int _zoomLevelRead
        {
            get
            {
                var weaponHandleId = BitConverter.ToInt32(Global.Memory.ReadBytes(Handle + MemoryOffset.ActiveWeapon, 4), 0) & 0xFFF;
                var weaponHandle = (IntPtr)BitConverter.ToInt32(Global.Memory.ReadBytes(MemoryBase.ClientDll + MemoryBase.EntityList.ToInt32() + (weaponHandleId - 1) * 0x10, 4), 0);
                var weaponZoom = BitConverter.ToInt32(Global.Memory.ReadBytes(weaponHandle + MemoryOffset.ZoomLevel, 4), 0);

                return weaponZoom;
            }
        }
        private int _zoomLevelReadLast;
        private ulong _zoomLevelReadLastId;
        private int ZoomLevel
        {
            get
            {
                if (_zoomLevelReadLastId < Global.RefreshId)
                {
                    _zoomLevelReadLastId = Global.RefreshId;
                    _zoomLevelReadLast = _zoomLevelRead;
                }

                return _zoomLevelReadLast;
            }
        }
    }
}
