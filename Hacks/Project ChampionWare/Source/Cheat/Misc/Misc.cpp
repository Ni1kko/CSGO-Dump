#include "Misc.h"
#include "../../Engine/Engine.h"
//#include "../../Engine/Hook.h"


using namespace Client;
//[junk_enable /]
//[enc_string_enable /]

struct CUSTOMVERTEX {
	FLOAT x, y, z;
	FLOAT rhw;
	DWORD color;
	//  FLOAT tu, tv;  
};

void CircleFilledDualColor(float x, float y, float rad, float rotate, int type, int resolution, DWORD color, DWORD color2, IDirect3DDevice9* m_device)
{
	LPDIRECT3DVERTEXBUFFER9 g_pVB2;

	std::vector<CUSTOMVERTEX> circle(resolution + 2);

	float angle = rotate * D3DX_PI / 180, pi = D3DX_PI;

	if (type == 1)
		pi = D3DX_PI; // Full circle
	if (type == 2)
		pi = D3DX_PI / 2; // 1/2 circle
	if (type == 3)
		pi = D3DX_PI / 4; // 1/4 circle

	pi = D3DX_PI / type; // 1/4 circle

	circle[0].x = x;
	circle[0].y = y;
	circle[0].z = 0;
	circle[0].rhw = 1;
	circle[0].color = color2;

	for (int i = 1; i < resolution + 2; i++)
	{
		circle[i].x = (float)(x - rad * cos(pi*((i - 1) / (resolution / 2.0f))));
		circle[i].y = (float)(y - rad * sin(pi*((i - 1) / (resolution / 2.0f))));
		circle[i].z = 0;
		circle[i].rhw = 1;
		circle[i].color = color;
	}

	// Rotate matrix
	int _res = resolution + 2;
	for (int i = 0; i < _res; i++)
	{
		circle[i].x = x + cos(angle)*(circle[i].x - x) - sin(angle)*(circle[i].y - y);
		circle[i].y = y + sin(angle)*(circle[i].x - x) + cos(angle)*(circle[i].y - y);
	}

	m_device->CreateVertexBuffer((resolution + 2) * sizeof(CUSTOMVERTEX), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB2, NULL);

	VOID* pVertices;
	g_pVB2->Lock(0, (resolution + 2) * sizeof(CUSTOMVERTEX), (void**)&pVertices, 0);
	memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(CUSTOMVERTEX));
	g_pVB2->Unlock();

	m_device->SetTexture(0, NULL);
	m_device->SetPixelShader(NULL);
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_device->SetStreamSource(0, g_pVB2, 0, sizeof(CUSTOMVERTEX));
	m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, resolution);
	if (g_pVB2 != NULL)
		g_pVB2->Release();
}

void CircleFilledRainbowColor(float x, float y, float rad, float rotate, int type, int resolution, IDirect3DDevice9* m_device)
{
	LPDIRECT3DVERTEXBUFFER9 g_pVB2;

	std::vector<CUSTOMVERTEX> circle(resolution + 2);

	float angle = rotate * D3DX_PI / 180, pi = D3DX_PI;

	if (type == 1)
		pi = D3DX_PI; // Full circle
	if (type == 2)
		pi = D3DX_PI / 2; // 1/2 circle
	if (type == 3)
		pi = D3DX_PI / 4; // 1/4 circle

	pi = D3DX_PI / type; // 1/4 circle

	circle[0].x = x;
	circle[0].y = y;
	circle[0].z = 0;
	circle[0].rhw = 1;
	circle[0].color = D3DCOLOR_RGBA(0, 0, 0, 0);

	float hue = 0.f;

	for (int i = 1; i < resolution + 2; i++)
	{
		circle[i].x = (float)(x - rad * cos(pi*((i - 1) / (resolution / 2.0f))));
		circle[i].y = (float)(y - rad * sin(pi*((i - 1) / (resolution / 2.0f))));
		circle[i].z = 0;
		circle[i].rhw = 1;

		auto clr = Color::FromHSB(hue, 1.f, 1.f);
		circle[i].color = D3DCOLOR_RGBA(clr.r(), clr.g(), clr.b(), clr.a());
		hue += 0.02;
	}

	// Rotate matrix
	int _res = resolution + 2;
	for (int i = 0; i < _res; i++)
	{
		float Vx1 = x + (cosf(angle) * (circle[i].x - x) - sinf(angle) * (circle[i].y - y));
		float Vy1 = y + (sinf(angle) * (circle[i].x - x) + cosf(angle) * (circle[i].y - y));

		circle[i].x = Vx1;
		circle[i].y = Vy1;
	}

	m_device->CreateVertexBuffer((resolution + 2) * sizeof(CUSTOMVERTEX), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB2, NULL);

	VOID* pVertices;
	g_pVB2->Lock(0, (resolution + 2) * sizeof(CUSTOMVERTEX), (void**)&pVertices, 0);
	memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(CUSTOMVERTEX));
	g_pVB2->Unlock();

	m_device->SetTexture(0, NULL);
	m_device->SetPixelShader(NULL);
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_device->SetStreamSource(0, g_pVB2, 0, sizeof(CUSTOMVERTEX));
	m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, resolution);
	if (g_pVB2 != NULL)
		g_pVB2->Release();
}

DWORD color = D3DCOLOR_RGBA((int)(0 * 255.0f), (int)(0 * 255.0f), (int)(0 * 255.0f), (int)(1 * 255.0f));
DWORD color2 = D3DCOLOR_RGBA((int)(0 * 255.0f), (int)(0 * 255.0f), (int)(0 * 255.0f), (int)(0 * 255.0f));
float CBaseWeapon::GetInaccuracy()
{
	return GetMethod<float(__thiscall*)(void*)>(this, 476)(this);
}
float CBaseWeapon::GetSpreadCone()
{
	return GetMethod<float(__thiscall*)(void*)>(this, 446)(this);
}

void Crosshair(IDirect3DDevice9* pDevice)
{
	CBaseEntity* local = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

	Color AwpAimColor = Color(int(Settings::Misc::misc_AwpAimColor[0] * 255.f),
		int(Settings::Misc::misc_AwpAimColor[1] * 255.f),
		int(Settings::Misc::misc_AwpAimColor[2] * 255.f));

	if (Settings::Misc::ShowSpread)
	{
		int width, height;
		Interfaces::Engine()->GetScreenSize(width, height);
		if (/*local && */IsLocalAlive())
		{
			static Vector ViewAngles;
			Interfaces::Engine()->GetViewAngles(ViewAngles);
			ViewAngles += (local->GetAimPunchAngle()) * 2.f;

			static Vector fowardVec;
			AngleVectors(ViewAngles, fowardVec);
			fowardVec *= 10000;

			// Get ray start / end
			Vector start = local->GetOrigin() + local->GetViewOffset();
			Vector end = start + fowardVec, endScreen;

			CBaseWeapon* weaponbase = local->GetBaseWeapon();


			int iWeponIndex = GetWeaponSettingsSelectID();


			if (/*weaponbase->IsGun()*/(iWeponIndex > 0 && iWeponIndex < 41) || (iWeponIndex > 59 && iWeponIndex < 62) || iWeponIndex == 63 || iWeponIndex == 64)
			{
				float cone = weaponbase->GetInaccuracy();

				if (cone > 0.0f)
				{
					float size;
					Vector screen;

					if (cone < 0.01f) cone = 0.01f;

					float size1337 = (cone * height) * 0.7f;



					/*Вот это значит рейнбов круг*/
					CircleFilledRainbowColor(Client::iScreenWidth / 2, Client::iScreenHeight / 2, size1337, 0, 1, 50/*, color, color2*/, pDevice);
					/*А это двухцветный круг прямо как в ав*/
					CircleFilledDualColor(Client::iScreenWidth / 2, Client::iScreenHeight / 2, size1337, 0, 1, 50, color, color2, pDevice);

				}
			}



		}
	}
}



void CMisc::Emit_Sound(const char* sound_entry)
{

	//static auto oemitsound = clientmode.get_original<emitsound_fn>(5);

	static auto SetLocalPlayerReadyFn = reinterpret_cast<bool(__stdcall*)(const char*)>(CSX::Memory::FindPatternV2("client_panorama.dll", "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12"));

	
	if (!strcmp(sound_entry, "UIPanorama.popup_accept_match_beep") && Settings::Misc::Autoaccept && !Interfaces::Engine()->IsInGame())
	{
		SetLocalPlayerReadyFn("accept");

		// Hacemos click en el boton
		HWND Hwnd;
		if ((Hwnd = FindWindow(NULL, "Valve001")) && GetForegroundWindow() == Hwnd) // Gets the csgo window and checks if the active window is csgos window // EDIT: Changed from Valve001 to Counter-Strike: Global Offensive.
		{
			RECT lprect;
			GetClientRect(Hwnd, &lprect); // Gets the resolution of the window
			SendMessage(Hwnd, WM_MOUSEMOVE, 0, MAKELPARAM(lprect.right / 2, lprect.bottom / 2)); // Moves the mouse into the middle of the window
			mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); // click 
		}
		//SetLocalPlayerReadyFn("");

	}



}



//void AutoAccept()
//{
//	if (Settings::Misc::Autoaccept && !strcmp(pSoundEntry, "UIPanorama.popup_accept_match_beep"))
//	{	
//		static auto SetLocalPlayerReadyFn = reinterpret_cast<bool(__stdcall*)(const char*)>(CSX::Memory::FindPatternV2("client_panorama.dll", "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12"));
//		if (SetLocalPlayerReadyFn)
//			SetLocalPlayerReadyFn("");
//		//static auto SetLocalPlayerReadyFn = reinterpret_cast<bool(__stdcall*)(const char*)>(CSX::Memory::FindPatternV2("client_panorama.dll", "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12"));
//		//HWND Hwnd;
//		//if ((Hwnd = FindWindow(NULL, "Counter-Strike: Global Offensive")) && GetForegroundWindow() == Hwnd) // Gets the csgo window and checks if the active window is csgos window // EDIT: Changed from Valve001 to Counter-Strike: Global Offensive.
//		//{
//		//	RECT lprect;
//		//	GetClientRect(Hwnd, &lprect); // Gets the resolution of the window
//		//	SendMessage(Hwnd, WM_MOUSEMOVE, 0, MAKELPARAM(lprect.right / 2, lprect.bottom / 2)); // Moves the mouse into the middle of the window
//		//	mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); // click 
//		//}
//		//if (SetLocalPlayerReadyFn)
//		//	SetLocalPlayerReadyFn(""); // Calling with "" because then it accepts everytime. Calling it with "deferred" dont always work.
//	}
//}



void CMisc::OnRender()
{
	if (Settings::Misc::misc_Punch)
	{
		int punch_x = (int)g_vCenterScreen.x + (int)g_pPlayers->GetLocal()->vAimPunch.y * 10;
		int punch_y = (int)g_vCenterScreen.y + (int)g_pPlayers->GetLocal()->vAimPunch.x * 10;

		g_pRender->DrawFillBox(punch_x - 1, punch_y - 1, 3, 3, Color::Green());






	}






	if (Settings::Misc::misc_Punch2)
	{
		CBaseEntity* localplayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());
		if (Interfaces::Engine()->IsInGame() && localplayer)
		{
			Vector ViewAngles;
			Interfaces::Engine()->GetViewAngles(ViewAngles);
			ViewAngles += (localplayer->GetAimPunchAngle()) * 2.f;

			Vector fowardVec;
			AngleVectors(ViewAngles, fowardVec);
			fowardVec *= 10000;

			Vector start = localplayer->GetEyePosition();
			Vector end = start + fowardVec, endScreen;

			if (WorldToScreen(end, endScreen) && IsLocalAlive())
			{
				g_pRender->DrawLine(endScreen.x + 7, endScreen.y + 0.5, endScreen.x - 8, endScreen.y + 0.5, Color::Red());
				g_pRender->DrawLine(endScreen.x + 0, endScreen.y + 7, endScreen.x + 0, endScreen.y - 8, Color::Red());
			}
		}
	}
}

void CMisc::OnCreateMove(CUserCmd* pCmd)
{
	CBaseEntity* localplayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

	bool in_water = g_pPlayers->GetLocal()->m_pEntity->m_nWaterLevel() >= 2;
	bool on_ladder = g_pPlayers->GetLocal()->m_pEntity->movetype() == MOVETYPE_LADDER;
	bool noclip = g_pPlayers->GetLocal()->m_pEntity->movetype() == MOVETYPE_NOCLIP;
	bool spectate = g_pPlayers->GetLocal()->m_pEntity->movetype() == MOVETYPE_OBSERVER;



	if (Interfaces::Engine()->IsConnected() && Interfaces::Engine()->IsInGame() && Settings::Misc::Ragdoll)
	{
		ConVar* pastergay = Interfaces::GetConVar()->FindVar("cl_ragdoll_gravity");
		if (pastergay)
			pastergay->SetValue(-100);
	}
	else if (Interfaces::Engine()->IsConnected() && Interfaces::Engine()->IsInGame() && !Settings::Misc::Ragdoll)
	{
		ConVar* pastergay = Interfaces::GetConVar()->FindVar("cl_ragdoll_gravity");
		if (pastergay)
			pastergay->SetValue(600);
	}



	ConVar* lefthandknife = Interfaces::GetConVar()->FindVar("cl_righthand");
	if (Settings::Misc::misc_LeftHandKnife)
	{
		if (Settings::Misc::misc_LeftHandKnife && Interfaces::Engine()->GetLocalPlayer() && g_pPlayers->GetLocal()->WeaponName.find("Knife") != string::npos && Interfaces::Engine()->IsInGame())
		{
			lefthandknife->SetValue(0);
		}
		else
		{
			lefthandknife->SetValue(1);
		}
	}
	else
	{
		//off 
	}

	if (Settings::Misc::misc_RightHandKnife)
	{
		if (Settings::Misc::misc_RightHandKnife && Interfaces::Engine()->GetLocalPlayer() && g_pPlayers->GetLocal()->WeaponName.find("Knife") != string::npos && Interfaces::Engine()->IsInGame())
		{
			lefthandknife->SetValue(1);
		}
		else
		{
			lefthandknife->SetValue(0);
		}
	}
	else
	{
		//off 
	}

	


	if (Settings::Misc::misc_Bhop)
	{
		static bool bLastJumped = false;
		static bool bShouldFake = false;

		if (!localplayer)
			return;

		if (localplayer->GetMoveType() == MOVETYPE_LADDER || localplayer->GetMoveType() == MOVETYPE_NOCLIP)
			return;

		if (!bLastJumped && bShouldFake)
		{
			bShouldFake = false;
			pCmd->buttons |= IN_JUMP;
		}
		else if (pCmd->buttons & IN_JUMP)
		{
			if (g_pPlayers->GetLocal()->iFlags & FL_ONGROUND)
			{
				bLastJumped = true;
				bShouldFake = true;
			}
			else
			{
				pCmd->buttons &= ~IN_JUMP;
				bLastJumped = false;
			}
		}
		else
		{
			bLastJumped = false;
			bShouldFake = false;
		}
	}

/*	if (Settings::Misc::misc_Bhop)
	{
		static bool bLastJumped = false;
		static bool bShouldFake = false;

		if (!localplayer)
			return;

		if (localplayer->GetMoveType() == MOVETYPE_LADDER || localplayer->GetMoveType() == MOVETYPE_NOCLIP)
			return;

		if (!bLastJumped && bShouldFake)
		{
			bShouldFake = false;
			pCmd->buttons |= IN_JUMP;
		}
		else if (pCmd->buttons & IN_JUMP)
		{
			if (g_pPlayers->GetLocal()->iFlags & FL_ONGROUND)
			{
				bLastJumped = true;
				bShouldFake = true;
			}
			else
			{
				pCmd->buttons &= ~IN_JUMP;
				bLastJumped = false;
			}
		}
		else
		{
			bLastJumped = false;
			bShouldFake = false;
		}
	}
/*	if (Settings::Misc::misc_EdgeJump)
	{
		if (localplayer->GetMoveType() == MOVETYPE_LADDER || localplayer->GetMoveType() == MOVETYPE_NOCLIP)
			return;

		switch (CurrentState)
		{
		case STATE_BEGIN:
			if (g_pPlayers->GetLocal()->iFlags & FL_ONGROUND && !(localplayer->GetFlags() & FL_ONGROUND) && !(pCmd->buttons & IN_JUMP))
			{
				pCmd->buttons |= IN_JUMP;

				CurrentState = STATE_DUCKING;
			}
			break;
		case STATE_DUCKING:
			if (localplayer->GetFlags() & FL_ONGROUND)
				CurrentState = STATE_BEGIN;
			else
				pCmd->buttons |= IN_DUCK;

			break;
		}
	}
*/
	if (Settings::Misc::misc_AutoStrafe && !(g_pPlayers->GetLocal()->iFlags & FL_ONGROUND) && !on_ladder && !noclip && !spectate && !in_water)
	{
		static float float1;
		static float float2;

		if (!(g_pPlayers->GetLocal()->iFlags & FL_ONGROUND))
		{
			float2 = CSX::Utils::RandomIntRange(-440, -450);
			float1 = CSX::Utils::RandomIntRange(440, 450);
		}

		if (pCmd->mousedx < 0)
		{
			pCmd->sidemove = float2;
		}
		else if (pCmd->mousedx > 0)
		{
			pCmd->sidemove = float1;
		}
	}

	ConVar* skybox = Interfaces::GetConVar()->FindVar("sv_skyname");

	if (Settings::Misc::misc_SkyName) { if (skybox) skybox->SetValue(Settings::Misc::misc_SkyName); }
	if (Settings::Misc::misc_NoSky) { if (skybox) skybox->SetValue("sky_l4d_rural02_ldr"); }

	ConVar* snow = Interfaces::GetConVar()->FindVar("mat_drawgray");

	if (Settings::Misc::misc_Snow)
	{
		snow->SetValue(1);
	}
	else
	{
		snow->SetValue(0);
	}

	if (Settings::Misc::misc_Duck)
	{
		pCmd->buttons |= IN_BULLRUSH;
	}

	if (Settings::Misc::misc_antiafk)
	{
		static bool Jitter;
		Jitter = !Jitter;
		if (Jitter)
			pCmd->sidemove += 450;
		else
			pCmd->sidemove = -450;

		if (!Jitter)
			pCmd->forwardmove = -450;
		else
			pCmd->forwardmove = +450;

		pCmd->buttons += IN_MOVELEFT;
	}

	ConVar* postprocess = Interfaces::GetConVar()->FindVar("mat_postprocess_enable");

	if (Settings::Misc::misc_EPostprocess)
	{
		if (Interfaces::Engine()->IsInGame())
		{
			postprocess->SetValue(0);
		}
	}
	else
	{
		if (Interfaces::Engine()->IsInGame())
		{
			postprocess->SetValue(1);
		}
	}

}

std::vector<const char*> smoke_materials =
{
	"particle/vistasmokev1/vistasmokev1_smokegrenade",
	"particle/vistasmokev1/vistasmokev1_emods",
	"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	"particle/vistasmokev1/vistasmokev1_fire",
};

void CMisc::OnDrawModelExecute()
{
	static bool NoFlashReset = false;
	static bool NoSmoke = false;

	if (Settings::Misc::misc_NoFlash && !NoFlashReset)
	{
		IMaterial* flash = Interfaces::MaterialSystem()->FindMaterial(
			"effects\\flashbang", TEXTURE_GROUP_CLIENT_EFFECTS);

		IMaterial* flashWhite = Interfaces::MaterialSystem()->FindMaterial("effects\\flashbang_white",
			TEXTURE_GROUP_CLIENT_EFFECTS);

		if (flash && flashWhite)
		{
			flash->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			flashWhite->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);

			NoFlashReset = true;
		}
	}
	else if (!Settings::Misc::misc_NoFlash && NoFlashReset)
	{
		IMaterial* flash = Interfaces::MaterialSystem()->FindMaterial(
			"effects\\flashbang", TEXTURE_GROUP_CLIENT_EFFECTS);

		IMaterial* flashWhite = Interfaces::MaterialSystem()->FindMaterial("effects\\flashbang_white",
			TEXTURE_GROUP_CLIENT_EFFECTS);

		if (flash && flashWhite)
		{
			flash->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
			flashWhite->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);

			NoFlashReset = false;
		}
	}
	if (Settings::Misc::misc_NoSmoke)
	{
		IMaterial* vistasmokev1_smokegrenade = Interfaces::MaterialSystem()->FindMaterial("particle/vistasmokev1/vistasmokev1_smokegrenade", TEXTURE_GROUP_CLIENT_EFFECTS);

		IMaterial* vistasmokev1_emods = Interfaces::MaterialSystem()->FindMaterial("particle/vistasmokev1/vistasmokev1_emods", TEXTURE_GROUP_CLIENT_EFFECTS);

		IMaterial* vistasmokev1_emods_impactdust = Interfaces::MaterialSystem()->FindMaterial("particle/vistasmokev1/vistasmokev1_emods_impactdust", TEXTURE_GROUP_CLIENT_EFFECTS);

		IMaterial* vistasmokev1_fire = Interfaces::MaterialSystem()->FindMaterial("particle/vistasmokev1/vistasmokev1_fire", TEXTURE_GROUP_CLIENT_EFFECTS);

		if (vistasmokev1_smokegrenade && vistasmokev1_emods && vistasmokev1_emods_impactdust && vistasmokev1_fire)
		{
			vistasmokev1_smokegrenade->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			vistasmokev1_emods->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			vistasmokev1_emods_impactdust->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			vistasmokev1_fire->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);

			NoSmoke = true;
		}

		//(PBYTE)"\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0"
		//"\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0"
		//static auto linegoesthrusmoke = CSX::Memory::FindPattern(CLIENT_DLL, SMOK_PATTERN, "xxxxxxxx????xxx");
		static auto linegoesthrusmoke = CSX::Memory::FindPattern(CLIENT_DLL, "\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0", "xxxxxxxx????xxx", 0);
		static auto smokecout = *(DWORD*)(linegoesthrusmoke + 0x8);
		bool dayn = true;
		if (dayn) *(int*)(smokecout) = 0;


	}
	else if (!Settings::Misc::misc_NoSmoke)
	{
		IMaterial* vistasmokev1_smokegrenade = Interfaces::MaterialSystem()->FindMaterial("particle/vistasmokev1/vistasmokev1_smokegrenade", TEXTURE_GROUP_CLIENT_EFFECTS);

		IMaterial* vistasmokev1_emods = Interfaces::MaterialSystem()->FindMaterial("particle/vistasmokev1/vistasmokev1_emods", TEXTURE_GROUP_CLIENT_EFFECTS);

		IMaterial* vistasmokev1_emods_impactdust = Interfaces::MaterialSystem()->FindMaterial("particle/vistasmokev1/vistasmokev1_emods_impactdust", TEXTURE_GROUP_CLIENT_EFFECTS);

		IMaterial* vistasmokev1_fire = Interfaces::MaterialSystem()->FindMaterial("particle/vistasmokev1/vistasmokev1_fire", TEXTURE_GROUP_CLIENT_EFFECTS);

		if (vistasmokev1_smokegrenade && vistasmokev1_emods && vistasmokev1_emods_impactdust && vistasmokev1_fire)
		{
			vistasmokev1_smokegrenade->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
			vistasmokev1_emods->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
			vistasmokev1_emods_impactdust->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
			vistasmokev1_fire->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);

			NoSmoke = false;

		}
	}
}

/*void CMisc::OnPlaySound(const char* pszSoundName)
{
	if (Settings::Misc::misc_AutoAccept && !strcmp(pszSoundName, "!UI/competitive_accept_beep.wav"))
	{
		typedef void(*IsReadyCallBackFn)();

		IsReadyCallBackFn IsReadyCallBack = 0;

		if (!IsReadyCallBack)
		{
			IsReadyCallBack = (IsReadyCallBackFn)(
				CSX::Memory::FindPattern(CLIENT_DLL, "55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ? ? ? ? 57 83 BE", 0));
#if ENABLE_DEBUG_FILE == 1
			CSX::Log::Add("::IsReadyCallBack = %X", IsReadyCallBack);
#endif
		}

		if (IsReadyCallBack)
		{
			IsReadyCallBack();
		}
	}
}*/
/*
void CMisc::OnOverrideView(CViewSetup * pSetup)
{
	//if (Settings::Misc::misc_FovChanger && !Interfaces::Engine()->IsTakingScreenshot()) - screenshot shit
	if (Settings::Misc::misc_FovChanger)
	{
		CBaseEntity* pPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

		if (!pPlayer)
			return;

		if (pPlayer->GetIsScoped())
			return;

		if (pPlayer->IsDead())
		{
			if (pPlayer->GetObserverMode() == ObserverMode_t::OBS_MODE_IN_EYE && pPlayer->GetObserverTarget())
				pPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntityFromHandle(pPlayer->GetObserverTarget());

			if (!pPlayer)
				return;
		}

		pSetup->fov = (float)Settings::Misc::misc_FovView;
	}
}

void CMisc::OnGetViewModelFOV(float& fov)
{
	// if (Settings::Misc::misc_FovChanger && !Interfaces::Engine()->IsTakingScreenshot()) - screenshot shit
	if (Settings::Misc::misc_FovChanger)
	{
		CBaseEntity* pPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

		if (!pPlayer)
			return;

		if (pPlayer->IsDead())
		{
			if (pPlayer->GetObserverMode() == ObserverMode_t::OBS_MODE_IN_EYE && pPlayer->GetObserverTarget())
				pPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntityFromHandle(pPlayer->GetObserverTarget());

			if (!pPlayer)
				return;
		}

		fov = (float)Settings::Misc::misc_FovModelView;
	}
}
*/
vector<int> CMisc::GetObservervators(int playerId)
{
	vector<int> SpectatorList;

	CBaseEntity* pPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(playerId);

	if (!pPlayer)
		return SpectatorList;

	if (pPlayer->IsDead())
	{
		CBaseEntity* pObserverTarget = (CBaseEntity*)Interfaces::EntityList()->GetClientEntityFromHandle(pPlayer->GetObserverTarget());

		if (!pObserverTarget)
			return SpectatorList;

		pPlayer = pObserverTarget;
	}

	for (int PlayerIndex = 0; PlayerIndex < g_pPlayers->GetSize(); PlayerIndex++)
	{
		CBaseEntity* pCheckPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(PlayerIndex);

		if (!pCheckPlayer)
			continue;

		if (pCheckPlayer->IsDormant() || !pCheckPlayer->IsDead())
			continue;

		CBaseEntity* pObserverTarget = (CBaseEntity*)Interfaces::EntityList()->GetClientEntityFromHandle(pCheckPlayer->GetObserverTarget());

		if (!pObserverTarget)
			continue;

		if (pPlayer != pObserverTarget)
			continue;

		SpectatorList.push_back(PlayerIndex);
	}

	return SpectatorList;
}




void CMisc::KillSpam(IGameEvent* pEvent) {




	const char* szEventName = pEvent->GetName();




	if (!strcmp(szEventName, "player_death") && Settings::Misc::misc_killspam)
	{
		int Attacker = pEvent->GetInt("attacker");
		int UserID = pEvent->GetInt("userid");

		CBaseEntity* Entity = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetPlayerForUserID(UserID));



		if (Interfaces::Engine()->GetPlayerForUserID(Attacker) == Interfaces::Engine()->GetLocalPlayer())

		{
			int impres;
			impres = rand() % 3 + 1;
			switch (impres) {

			case 1: Interfaces::Engine()->ClientCmd("say Tapped by Aimware.cc <33"); break;
			case 2: Interfaces::Engine()->ClientCmd("say \\_/ <-- cry in here , africa needs water"); break;
			case 3: Interfaces::Engine()->ClientCmd("say Choose your excuse: 1.Lags | 2.New mouse | 3.Low FPS | 4.Low team | 5.Hacker | 6.Lucker | 7.Smurf | 8.Hitbox | 9.Tickrate."); break;

			}

		}
	}

}

//auto meme_inviter(bool get_collection_size) -> uint16_t {
//	
//#pragma pack(push, 1)
//	struct item_t {
//		uint16_t idx_next_0, unk_idx_2, idx_prev_4, unk_val_6;
//		uint64_t steam_id_8;
//		uint32_t* unk_ptr_16;
//	}; //Size: 0x0014
//#pragma pack(pop)
//
//	static const auto collection = *(uint32_t**)(CSX::Memory::FindPatternV2("client_panorama.dll", "8B 35 ? ? ? ? 66 3B D0 74 07") + 2);
//
//	if (*collection) {
//		auto invite_to_lobby = [](uint64_t id) {
//			class IMatchFramework; // GetMatchSession:13
//			class ISteamMatchmaking; // InviteUserToLobby:16
//
//			using GetLobbyId = uint64_t(__thiscall*)(void*);
//			using GetMatchSession = uintptr_t * (__thiscall*)(IMatchFramework*);
//			using InviteUserToLobby = bool(__thiscall*)(ISteamMatchmaking*, uint64_t, uint64_t);
//
//			static const auto match_framework = **reinterpret_cast<IMatchFramework***>(CSX::Memory::FindPatternV2("client_panorama.dll", "8B 0D ? ? ? ? 8B 01 FF 50 2C 8D 4B 18") + 0x2);
//			static const auto steam_matchmaking = **reinterpret_cast<ISteamMatchmaking***>(CSX::Memory::FindPatternV2("client_panorama.dll", "8B 3D ? ? ? ? 83 EC 08 8B 4D 08 8B C4") + 0x2);
//
//			const auto match_session = clientmode.get_original<GetMatchSession>(match_framework, 13)(match_framework);
//			if (match_session) {
//				const uint64_t my_lobby_id = CallVFunction<GetLobbyId>(match_session, 4)(match_session);
//				CallVFunction<InviteUserToLobby>(steam_matchmaking, 16)(steam_matchmaking, my_lobby_id, id);
//			}
//		};
//
//		auto max_index = ((uint16_t*)collection)[9];
//
//		if (get_collection_size)
//			return max_index;
//
//		for (uint16_t i = 0; i <= max_index; ++i) {
//			auto item = &((item_t*)*collection)[i];
//			invite_to_lobby(item->steam_id_8);
//		}
//
//		return max_index;
//	}
//
//	return 0;
//}


void CMisc::OnRenderSpectatorList()
{

	//clantag
	if (Settings::Misc::misc_Clan > 0 && IsLocalAlive())
		ClanTag();

	if (Settings::Misc::misc_Spectators && Interfaces::Engine()->IsInGame())
	{
		int DrawIndex = 1;

		for (int playerId : GetObservervators(Interfaces::Engine()->GetLocalPlayer()))
		{
			if (playerId == Interfaces::Engine()->GetLocalPlayer())
				continue;

			CBaseEntity* pPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(playerId);

			if (!pPlayer)
				continue;

			PlayerInfo Pinfo;
			Interfaces::Engine()->GetPlayerInfo(playerId, &Pinfo);

			if (Pinfo.m_bIsFakePlayer)
				continue;

			if (g_pRender)
			{
				string Name = Pinfo.m_szPlayerName;

				if (Name != "")
				{
					Color PlayerObsColor;
					//[junk_disable /]
					switch (pPlayer->GetObserverMode())
					{
					case ObserverMode_t::OBS_MODE_IN_EYE:
						Name.append(" - Perspective");
						PlayerObsColor = Color::White();
						break;
					case ObserverMode_t::OBS_MODE_CHASE:
						Name.append(" - 3rd person");
						PlayerObsColor = Color::White();
						break;
					case ObserverMode_t::OBS_MODE_ROAMING:
						Name.append(" - Free look");
						PlayerObsColor = Color::White();
						break;
					case ObserverMode_t::OBS_MODE_DEATHCAM:
						Name.append(" - Deathcam");
						PlayerObsColor = Color::White();
						break;
					case ObserverMode_t::OBS_MODE_FREEZECAM:
						Name.append(" - Freezecam");
						PlayerObsColor = Color::White();
						break;
					case ObserverMode_t::OBS_MODE_FIXED:
						Name.append(" - Fixed");
						PlayerObsColor = Color::White();
						break;
					default:
						break;
					}
					//[junk_enable /]
					g_pRender->Text(15, 502 + (DrawIndex * 13), false, true, PlayerObsColor, "%s", Name.c_str());
					DrawIndex++;
				}
			}
		}
	}
}















































































































































































