#pragma once

#include "Definitions.hpp"

namespace SDK
{
	class PlayerInfo {
		private:
		DWORD __pad0[2];
	public:
		int m_nXuidLow;
		int m_nXuidHigh;
		char m_szPlayerName[128];
		int m_nUserID;
		char m_szSteamID[33];
		UINT m_nSteam3ID;
		char m_szFriendsName[128];
		bool m_bIsFakePlayer;
		bool m_bIsHLTV;
		DWORD m_dwCustomFiles[4];
		BYTE m_FilesDownloaded;
		char name[128];
	private:
		int __pad1;
	};
}