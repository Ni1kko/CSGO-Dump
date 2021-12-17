using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace ScriptKidAntiCheat.Internal.Raw
{
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct PlayerInfo
    {
        public char __pad0;
        public int m_nXuidLow;
        public int m_nXuidHigh;
        public char m_szPlayerName;
        public int m_nUserID;
        public char m_szSteamID;
        public uint m_nSteam3ID;
        public char m_szFriendsName;
        public bool m_bIsFakePlayer;
        public bool m_bIsHLTV;
        public int m_dwCustomFiles;
        public char m_FilesDownloaded;
    }
}
