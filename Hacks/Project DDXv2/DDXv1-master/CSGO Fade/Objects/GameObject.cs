using FadeAPI.Helpers;
using FadeAPI.Memory;
using FadeAPI.Structs;
using System;
using System.Runtime.InteropServices;

namespace FadeAPI.Objects
{
    public class GameObject
    {
        private readonly int _index;

        public GameObject(int index)
        {
            _index = index;
        }

        private int _classIdRead
        {
            get
            {
                var vt = (IntPtr)BitConverter.ToInt32(Global.Memory.ReadBytes(Handle + 0x8, 4), 0);
                var fn = (IntPtr)BitConverter.ToInt32(Global.Memory.ReadBytes(vt + 0x8, 4), 0);
                var cls = (IntPtr)BitConverter.ToInt32(Global.Memory.ReadBytes(fn + 0x1, 4), 0);
                //var clsn = Memory[cls, false].Read<IntPtr>(8);
                var clsid = BitConverter.ToInt32(Global.Memory.ReadBytes(cls + 0x14, 4), 0);

                return clsid;
            }
        }
        private int _classIdReadLast;
        private ulong _classIdReadLastId;
        public int ClassId
        {
            get
            {
                if (_classIdReadLastId < Global.RefreshId)
                {
                    _classIdReadLastId = Global.RefreshId;
                    _classIdReadLast = _classIdRead;
                }

                return _classIdReadLast;
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

        private IntPtr _handleRead => (IntPtr)BitConverter.ToInt32(Global.Memory.ReadBytes(MemoryBase.ClientDll + MemoryBase.EntityList.ToInt32() + _index * 0x10, 4), 0);
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

        private bool _hasOwnerRead => Origin.Equals(new Vector3());
        private bool _hasOwnerReadLast;
        private ulong _hasOwnerLastId;
        public bool HasOwner
        {
            get
            {
                if (_hasOwnerLastId < Global.RefreshId)
                {
                    _hasOwnerLastId = Global.RefreshId;
                    _hasOwnerReadLast = _hasOwnerRead;
                }

                return _hasOwnerReadLast;
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
    }
}
