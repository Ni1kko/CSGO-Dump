#include "Triggerbot.h"
using namespace std;
void Triggerbot::TriggerBot(int Button)
{
	if (GetAsyncKeyState(Button) || CM.TriggerbotModeToggle)
	{
		int LocalCID = RPM<int>((Read.BasePointer + O.oCrossID));
		if (LocalCID > 0 && LocalCID < 65)
		{
			DWORD Entity = Read.GetEntity(LocalCID);
			int Enemyhealth = Read.GetEntityHealth(Entity);
			int Enemyteam = Read.GetEntityTeam(Entity);

			if (Enemyteam != Read.Team && Enemyteam > 1 && Enemyhealth > 0)
			{
				WPM<int>((Init.ClientDLL + O.dwForceAttack), 5);
				Sleep(10);
				WPM<int>((Init.ClientDLL + O.dwForceAttack), 4);
			}
		}
	}
}
