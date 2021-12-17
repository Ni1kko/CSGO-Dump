using System;
using System.Text;
using DDX.Extensions;
using DDX.Structs;

namespace DDX.Functions
{
    public static class GlobalRead
    {
        private static ulong _lastClientStateHandleRef;
        private static IntPtr _lastClientStateHandle;
        public static IntPtr GetClientStateHandle(bool force = false)
        {
            if (!force && _lastClientStateHandleRef == G.Ref)
                return _lastClientStateHandle;

            _lastClientStateHandleRef = G.Ref;
            return _lastClientStateHandle = G.Mem.Read<IntPtr>(Base.m_dwClientState);
        }

        private static ulong _lastCurrentSequenceNumberRef;
        private static int _lastCurrentSequenceNumber;
        public static int GetCurrentSequenceNumber(bool force = false)
        {
            if (!force && _lastCurrentSequenceNumberRef == G.Ref)
                return _lastCurrentSequenceNumber;

            _lastCurrentSequenceNumberRef = G.Ref;
            return _lastCurrentSequenceNumber = G.Mem.Read<int>(GetClientStateHandle() + Offset.m_iCurrentSequenceNumber) + 2;
        }

        public static IntPtr GetEntityHandleByIndex(int index)
        {
            return G.Mem.Read<IntPtr>(Base.m_dwEntityList + (index - 1) * 0x10);
        }

        public static IntPtr GetWeaponHandleByIndex(int index)
        {
            var entityIndex = G.Mem.Read<int>(G.Local.Handle + Offset.m_hMyWeapons + (index - 1) * 0x4) & 0xFFF;
            return GetEntityHandleByIndex(entityIndex);
        }

        private static ulong _lastEntityListLengthRef;
        private static int _lastEntityListLength;
        public static int GetEntityListLength(bool force = false)
        {
            if (!force && _lastEntityListLengthRef == G.Ref)
                return _lastEntityListLength;

            _lastEntityListLengthRef = G.Ref;
            return _lastEntityListLength = G.Mem.Read<int>(Base.m_dwEntityListLength);
        }

        private static ulong _lastCurrentTimeRef;
        private static float _lastCurrentTime;
        public static float GetCurrentTime(bool force = false)
        {
            if (!force && _lastCurrentTimeRef == G.Ref)
                return _lastCurrentTime;
    
            _lastCurrentTimeRef = G.Ref;
            return _lastCurrentTime = G.Mem.Read<float>(GetGlobalVariablesHandle() + 0x10);
        }

        private static ulong _lastGlobalVariablesHandleRef;
        private static IntPtr _lastGlobalVariablesHandle;
        public static IntPtr GetGlobalVariablesHandle(bool force = false)
        {
            if (!force && _lastGlobalVariablesHandleRef == G.Ref)
                return _lastGlobalVariablesHandle;

            _lastGlobalVariablesHandleRef = G.Ref;
            return _lastGlobalVariablesHandle = G.Mem.Read<IntPtr>(Base.m_dwGlobalVars);
        }

        private static ulong _lastGlowObjectHandleRef;
        private static IntPtr _lastGlowObjectHandle;
        public static IntPtr GetGlowObjectHandle(bool force = false)
        {
            if (!force && _lastGlowObjectHandleRef == G.Ref)
                return _lastGlowObjectHandle;

            _lastGlowObjectHandleRef = G.Ref;
            return _lastGlowObjectHandle = G.Mem.Read<IntPtr>(Base.m_dwGlowObject);
        }

        private static ulong _lastGlowObjectLengthRef;
        private static int _lastGlowObjectLength;
        public static int GetGlowObjectLength(bool force = false)
        {
            if (!force && _lastGlowObjectLengthRef == G.Ref)
                return _lastGlowObjectLength;

            _lastGlowObjectLengthRef = G.Ref;
            return _lastGlowObjectLength = G.Mem.Read<int>(Base.m_dwGlowObject + 0xC);
        }

        private static ulong _lastIsBombPlantedRef;
        private static bool _lastIsBombPlanted;
        public static bool GetIsBombPlanted(bool force = false)
        {
            if (!force && _lastIsBombPlantedRef == G.Ref)
                return _lastIsBombPlanted;

            _lastIsBombPlantedRef = G.Ref;
            return _lastIsBombPlanted = G.Mem.Read<byte>(Base.m_dwBombPlanted) == 1;
        }

        private static ulong _lastLocalPlayerHandleRef;
        private static IntPtr _lastLocalPlayerHandle;
        public static IntPtr GetLocalPlayerHandle(bool force = false)
        {
            if (!force && _lastLocalPlayerHandleRef == G.Ref)
                return _lastLocalPlayerHandle;

            _lastLocalPlayerHandleRef = G.Ref;
            return _lastLocalPlayerHandle = G.Mem.Read<IntPtr>(Base.m_dwLocalPlayer);
        }

        private static ulong _lastMapDirRef;
        private static string _lastMapDir;
        public static string GetMapDir(bool force = false)
        {
            if (!force && _lastMapDirRef == G.Ref)
                return _lastMapDir;

            _lastMapDirRef = G.Ref;
            return _lastMapDir = G.Mem.ReadString(GetClientStateHandle() + Offset.m_dwMapDirectory, Encoding.Default).CutNull().TrimEnd('p') + 'p'; // .bspppp
        }

        private static ulong _lastMapNameRef;
        private static string _lastMapName;
        public static string GetMapName(bool force = false)
        {
            if (!force && _lastMapNameRef == G.Ref)
                return _lastMapName;

            _lastMapNameRef = G.Ref;
            return _lastMapName = G.Mem.ReadString(GetClientStateHandle() + Offset.m_dwMapname, Encoding.Default).CutNull();
        }

        private static ulong _lastLocalPlayerIndexRef;
        private static int _lastLocalPlayerIndex;
        public static int GetLocalPlayerIndex(bool force = false)
        {
            if (!force && _lastLocalPlayerIndexRef == G.Ref)
                return _lastLocalPlayerIndex;

            _lastLocalPlayerIndexRef = G.Ref;
            return _lastLocalPlayerIndex = G.Mem.Read<int>(GetClientStateHandle() + Offset.m_dwLocalPlayerIndex);
        }

        private static ulong _lastPlayerC4Ref;
        private static int _lastPlayer4;
        public static int GetPlayerC4(bool force = false)
        {
            if (!force && _lastPlayerC4Ref == G.Ref)
                return _lastPlayer4;

            _lastPlayerC4Ref = G.Ref;
            return _lastPlayer4 = G.Mem.Read<int>(GetPlayerResourceHandle() + Offset.m_iPlayerC4);
        }

        private static ulong _lastPlayerResourceHandleRef;
        private static IntPtr _lastPlayerResourceHandle;
        public static IntPtr GetPlayerResourceHandle(bool force = false)
        {
            if (!force && _lastPlayerResourceHandleRef == G.Ref)
                return _lastPlayerResourceHandle;

            _lastPlayerResourceHandleRef = G.Ref;
            return _lastPlayerResourceHandle = G.Mem.Read<IntPtr>(Base.m_dwPlayerResource);
        }

        private static ulong _lastRadarHandleRef;
        private static IntPtr _lastRadarHandle;
        public static IntPtr GetRadarBaseHandle(bool force = false)
        {
            if (!force && _lastRadarHandleRef == G.Ref)
                return _lastRadarHandle;

            _lastRadarHandleRef = G.Ref;

            var radar = G.Mem.Read<IntPtr>(Base.m_dwRadarBase);
            return _lastRadarHandle = G.Mem.Read<IntPtr>(radar + Offset.m_dwRadarBasePointer);
        }

        private static ulong _lastViewMatrixRef;
        private static Matrix4x4 _lastViewMatrix;
        public static Matrix4x4 GetViewMatrix(bool force = false)
        {
            if (!force && _lastViewMatrixRef == G.Ref)
                return _lastViewMatrix;

            _lastViewMatrixRef = G.Ref;
            return _lastViewMatrix = G.Mem.Read<Matrix4x4>(Base.m_dwViewMatrix);
        }
    }
}
