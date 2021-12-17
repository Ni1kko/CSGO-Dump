#pragma once
#include "Tables.h"

void RankReveal()
{
	typedef bool(__cdecl *RankRevealFn)(float*);
	static RankRevealFn RankRevealEx = *(RankRevealFn*)(CSX::Memory::FindPattern(GetClientDll(), XorStr("\xC7\x00\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xEC\x08\x8D\x4E\x74"), XorStr("xx????x????xxxxxx"), 2));
	RankRevealEx(new float[3]{ 0.f, 0.f, 0.f });
}

bool __stdcall CreateMove(float flInputSampleTime, CUserCmd* pCmd)
{
	bool bReturn = HookTables::pCreateMove->GetTrampoline()(flInputSampleTime, pCmd);

	if (CGlobal::IsGameReady && pCmd->command_number != 0 && !CGlobal::FullUpdateCheck /*!CheatUnload*/)
	{
		CGlobal::GViewAngle = pCmd->viewangles;

		if (GP_EntPlayers)
			GP_EntPlayers->Update();

		if (GP_Esp->Rank && pCmd->buttons & IN_SCORE)
			RankReveal();

		if (GP_Esp)
			if (GP_Esp->GranadePrediction)
				grenade_prediction::Get().Tick(pCmd->buttons);

		DWORD* FirstP;
		__asm mov FirstP, ebp;

		bool bSendPacket = true;

		if (CGlobal::IsGuiVisble)
			pCmd->buttons &= ~IN_ATTACK;
		else if (GP_Skins && !CGlobal::IsGuiVisble)
			GP_Skins->SelectedWeapon = CGlobal::GetWeaponId();

#ifdef MULTIHACK_VERSION
		if (GP_LegitAim)
		{
			GP_LegitAim->SetSelectedWeapon();

			GP_LegitAim->BacktrackCreateMove(pCmd);

			if (GP_LegitAim->Enable)
				GP_LegitAim->CreateMove(bSendPacket, flInputSampleTime, pCmd);

			if (GP_LegitAim->TriggerEnable)
				GP_LegitAim->TriggerCreateMove(pCmd);
		}
#endif
		if (GP_Misc)
			GP_Misc->CreateMove(bSendPacket, flInputSampleTime, pCmd);

		CGlobal::ClampAngles(pCmd->viewangles);
		CGlobal::AngleNormalize(pCmd->viewangles);

		*(bool*)(*FirstP - 0x1C) = bSendPacket;

		if (!bSendPacket)
			return false;
	}

	return bReturn;
}