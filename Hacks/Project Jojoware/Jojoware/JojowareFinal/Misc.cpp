#include "Misc.h"
using namespace std;
void Misc::Bhop()
{
	if (CM.WantBhop == 1)
	{
		int Flag = RPM<int>((Read.BasePointer + O.m_fFlags));
		if (Flag == 257 && GetAsyncKeyState(VK_SPACE))
		{
			WPM<int>((Init.ClientDLL + O.dwForceJump), 5);
		}
		else if (Flag == 263 && GetAsyncKeyState(VK_SPACE))
		{
			WPM<int>((Init.ClientDLL + O.dwForceJump), 5);
		}
		else if (Flag == 1281 && GetAsyncKeyState(VK_SPACE))
		{
			WPM<int>((Init.ClientDLL + O.dwForceJump), 5);
		}
		else if (Flag == 1287 && GetAsyncKeyState(VK_SPACE))
		{
			WPM<int>((Init.ClientDLL + O.dwForceJump), 5);
		}
		else
		{
			WPM<int>((Init.ClientDLL + O.dwForceJump), 4);
		}
	}
}
void Misc::RadarHack()
{
	for (int i = 0; i < 20; i++)
	{
		DWORD Entity = Read.GetEntity(i);
		int Enemyteam = Read.GetEntityTeam(Entity);
		int IsSpotted = RPM<int>((Entity + O.m_bSpotted));
		if (Read.Team != Enemyteam && IsSpotted != 1)
		{
			WPM<int>(Entity + O.m_bSpotted, 1);
		}
	}
}
void Misc::RankReveal()
{
	if (GetAsyncKeyState(VK_INSERT))
	{
		DWORD PlayerResource = RPM<DWORD>(Init.ClientDLL + O.dwPlayerResource);
		for (int i = 0; i < 12; i++)
		{
			int Rank = RPM< int >(PlayerResource + O.m_iCompetitiveRanking + i * 4);
			int enmteam = RPM< int >(PlayerResource + O.m_iTeam + i * 4);
			int enmscore = RPM< int >(PlayerResource + O.m_iScore + i * 4);
			int ping = RPM< int >(PlayerResource + O.m_iPing + i * 4);
			if (Rank < 0 && Rank > 19) return;
			if (enmteam > 1 && ping >= 5 && ping < 1000 && enmteam < 4)
			{
				if (enmteam != Read.Team)
				{
					cout << "[ENEMY] [SCORE: " << enmscore << "]" << " [RANK: " << CompRanks[Rank] << "]" << endl;
				}
				else
				{
					cout << "[TEAM] [SCORE: " << enmscore << "]" << " [RANK: " << CompRanks[Rank] << "]" << endl;
				}
			}
		}
		cout << endl << endl;
		Sleep(100);
	}
}
int c = 1;
void Misc::RenderChange()
{
	c++;
	if (c == 10)
	{
		if (CM.WantChamsR == 1)
		{
			float brightness = CM.ChamsBrightness;
			int t = Read.GetLocalTeam();
			for (int i = 1; i < 22; i++)
			{
				DWORD ent = Read.GetEntity(i);
				int entt = Read.GetEntityTeam(ent);
				int hp = Read.GetEntityHealth(ent);
				if (hp > 66) ChangeGreenCLR();
				else if (hp <= 66 && hp >= 33) ChangeYellowCLR();
				else ChangeRedCLR();
				if (t != entt && entt > 1)
				{
					WPM<int>(ent + O.m_clrRender, (BYTE)clr.r);
					WPM<int>(ent + O.m_clrRender + 0x1, (BYTE)clr.g);
					WPM<int>(ent + O.m_clrRender + 0x2, (BYTE)clr.b);
					WPM<int>(ent + O.m_clrRender + 0x3, (BYTE)255);
				}
				else
				{
					WPM<int>(ent + O.m_clrRender, (BYTE)-1);
					WPM<int>(ent + O.m_clrRender + 0x1, (BYTE)-1);
					WPM<int>(ent + O.m_clrRender + 0x2, (BYTE)-1);
					WPM<int>(ent + O.m_clrRender + 0x3, (BYTE)-1);
				}
			}
			int thisPtr = (int)(Init.EngineDLL + O.dwModelAmbientMin - 0x2c);
			int xored = *(int*)&brightness ^ thisPtr;
			WPM<int>(Init.EngineDLL + O.dwModelAmbientMin, xored);
		}
	}
	else if (c > 10) c = 1;
}