#include "GlowEsp.h"

void Glow::GlowEsp()
{
	if (CM.WantESP == 1)
	{
		DWORD Glowobject = RPM<DWORD>(Init.ClientDLL + O.dwGlowObjectManager);
		for (int i = 0; i < 20; i++)
		{
			DWORD Ent = Read.GetEntity(i);
			int GlowIndex = RPM<int>(Ent + O.m_iGlowIndex);
			if (Ent == 0) continue;
			bool dormant = RPM<int>(Ent + O.m_bDormant);
			if (dormant) continue;
			int EnTeam = RPM<int>(Ent + O.oTeam);
			EnemyGlow = RPM<GlowStruct>(Glowobject + (GlowIndex * 0x38) + 0x4);
			if (EnTeam == Read.Team)
			{
				EnemyGlow.r = 1.0f;
				EnemyGlow.g = 1.0f;
				EnemyGlow.b = 1.0f;
				EnemyGlow.a = 1.0f;
			}
			else
			{
				EnemyGlow.a = 1.0f;
				int hp = Read.GetEntityHealth(Ent);
				if (hp > 66) ChangeGreenGlow();
				else if (hp <= 66 && hp >= 33) ChangeYellowGlow();
				else ChangeRedGlow();
				if (CM.WantChams != 0) SetGlowStyle(CM.WantChams);

			}
			EnemyGlow.m_bRenderWhenOccluded = true;
			EnemyGlow.m_bRenderWhenUnoccluded = false;
			WPM<GlowStruct>(Glowobject + (GlowIndex * 0x38) + 0x4, EnemyGlow);
		}
	}
}

