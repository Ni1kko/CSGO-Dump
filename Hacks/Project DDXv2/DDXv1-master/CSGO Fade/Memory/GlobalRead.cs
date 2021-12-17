using FadeAPI.Helpers;
using FadeAPI.Structs;
using System;
using System.Runtime.InteropServices;

namespace FadeAPI.Memory
{
    public static class GlobalRead
    {
        private static IntPtr _clientStateRead => (IntPtr)BitConverter.ToInt32(Global.Memory.ReadBytes(MemoryBase.EngineDll + MemoryBase.ClientState.ToInt32(), 4), 0);
        private static IntPtr _clientStateReadLast;
        private static ulong _clientStateReadLastId;
        public static IntPtr ClientState
        {
            get
            {
                if (_clientStateReadLastId < Global.RefreshId)
                {
                    _clientStateReadLastId = Global.RefreshId;
                    _clientStateReadLast = _clientStateRead;
                }

                return _clientStateReadLast;
            }
        }

        private static IntPtr _glowHandleRead => (IntPtr)BitConverter.ToInt32(Global.Memory.ReadBytes(MemoryBase.ClientDll + MemoryBase.GlowObject.ToInt32(), 4), 0);
        private static IntPtr _glowHandleReadLast;
        private static ulong _glowHandleReadLastId;
        public static IntPtr GlowHandle
        {
            get
            {
                if (_glowHandleReadLastId < Global.RefreshId)
                {
                    _glowHandleReadLastId = Global.RefreshId;
                    _glowHandleReadLast = _glowHandleRead;
                }

                return _glowHandleReadLast;
            }
        }

        private static int _gameObjectsCountRead => BitConverter.ToInt32(Global.Memory.ReadBytes(MemoryBase.EngineDll + MemoryBase.EntityListLength.ToInt32(), 4), 0);
        private static int _gameObjectsCountReadLast;
        private static ulong _gameObjectsCountReadLastId;
        public static int GameObjectsCount
        {
            get
            {
                if (_gameObjectsCountReadLastId < Global.RefreshId)
                {
                    _gameObjectsCountReadLastId = Global.RefreshId;
                    _gameObjectsCountReadLast = _gameObjectsCountRead;
                }

                return _gameObjectsCountReadLast;
            }
        }

        private static IntPtr _playerResourceRead => (IntPtr)BitConverter.ToInt32(Global.Memory.ReadBytes(MemoryBase.ClientDll + MemoryBase.PlayerResource.ToInt32(), 4), 0);
        private static IntPtr _playerResourceReadLast;
        private static ulong _playerResourceReadLastId;
        public static IntPtr PlayerResource
        {
            get
            {
                if (_playerResourceReadLastId < Global.RefreshId)
                {
                    _playerResourceReadLastId = Global.RefreshId;
                    _playerResourceReadLast = _playerResourceRead;
                }

                return _playerResourceReadLast;
            }
        }

        private static IntPtr _radarHandleRead
        {
            get
            {
                var radarBasePointer = (IntPtr)BitConverter.ToInt32(Global.Memory.ReadBytes(MemoryBase.ClientDll + MemoryBase.Radar.ToInt32(), 4), 0);
                return (IntPtr)BitConverter.ToInt32(Global.Memory.ReadBytes(radarBasePointer + MemoryOffset.RadarBasePointer, 4), 0);
            }
        }
        private static IntPtr _radarHandleReadLast;
        private static ulong _radarHandleReadLastId;
        public static IntPtr RadarHandle
        {
            get
            {
                if (_radarHandleReadLastId < Global.RefreshId)
                {
                    _radarHandleReadLastId = Global.RefreshId;
                    _radarHandleReadLast = _radarHandleRead;
                }

                return _radarHandleReadLast;
            }
        }

        private static Matrix4x4 _viewMatrixRead => StructHelper.ToObject<Matrix4x4>(Global.Memory.ReadBytes(MemoryBase.ClientDll + MemoryBase.ViewMatrix.ToInt32(), Marshal.SizeOf(new Matrix4x4())));
        private static Matrix4x4 _viewMatrixReadLast;
        private static ulong _viewMatrixReadLastId;
        public static Matrix4x4 ViewMatrix
        {
            get
            {
                if (_viewMatrixReadLastId < Global.RefreshId)
                {
                    _viewMatrixReadLastId = Global.RefreshId;
                    _viewMatrixReadLast = _viewMatrixRead;
                }

                return _viewMatrixReadLast;
            }
        }
    }
}
