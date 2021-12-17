#include "Radar.h"
#include "../Setup.h"
#include "../../GUI/Gui.h"

void CRadar::RadarRange(float* x, float* y, float range)
{
	if (fabs((*x)) > range || fabs((*y)) > range)
	{
		if ((*y) > (*x))
		{
			if ((*y) > -(*x))
			{
				(*x) = range * (*x) / (*y);
				(*y) = range;
			}
			else
			{
				(*y) = -range * (*y) / (*x);
				(*x) = -range;
			}
		}
		else
		{
			if ((*y) > -(*x))
			{
				(*y) = range * (*y) / (*x);
				(*x) = range;
			}
			else
			{
				(*x) = -range * (*x) / (*y);
				(*y) = -range;
			}
		}
	}
}
#define PI 3.14159265

void CRadar::CalcRadarPoint(Vector vOrigin, float& screenx, float& screeny, bool ChechOut)
{
	Vector vAngle;

	I::Engine()->GetViewAngles(vAngle);

	float range = ((float)Range) / 1000.f;

	Vec2 DrawPos = X1Gui().GetCurWindowPos();
	Vec2 DrawSize = X1Gui().GetCurWindowSize();

	float r_1, r_2;
	float x_1, y_1;

	Vector vEyeOrigin = LocalRendOrig + LocalViewOfst;

	r_1 = -(vOrigin.y - vEyeOrigin.y);
	r_2 = vOrigin.x - vEyeOrigin.x;
	float Yaw = vAngle.y - 90.0f;

	float yawToRadian = Yaw * (float)(M_PI / 180.0F);
	x_1 = (float)(r_2 * (float)cos((double)(yawToRadian)) - r_1 * sin((double)(yawToRadian))) / 20.f;
	y_1 = (float)(r_2 * (float)sin((double)(yawToRadian)) + r_1 * cos((double)(yawToRadian))) / 20.f;

	x_1 *= range;
	y_1 *= range;

	float sizX = DrawSize.x / 2;
	float sizY = DrawSize.y / 2;

	x_1 += sizX;
	y_1 += sizY;

	if (!OnlyInRadar && ChechOut)
	{
		if (x_1 < 5)
			x_1 = 5;

		if (x_1 > DrawSize.x)
			x_1 = DrawSize.x;

		if (y_1 < 5)
			y_1 = 5;

		if (y_1 > DrawSize.y)
			y_1 = DrawSize.y;
	}
	x_1 += DrawPos.x;
	y_1 += DrawPos.y;

	screenx = x_1;
	screeny = y_1;
}

void CRadar::RenderPlayer(CBaseEntity* plocal)
{

	Vec2 TextOffset = Vec2(X1Gui().GetCursorPos().x - X1Gui().GetCursorPosEx().x, X1Gui().GetCursorPos().y - X1Gui().GetCursorPosEx().y);

	CEntityPlayer* pLocal = GP_EntPlayers->EntityLocal;
	for (int EntIndex = 0; EntIndex < MAX_ENTITY_PLAYERS; EntIndex++)
	{
		CEntityPlayer* Entity = &GP_EntPlayers->EntityPlayer[EntIndex];

		if (!Entity)
			continue;

		if (!Entity->IsUpdated)
			continue;

		if (Entity->IsLocal)
			continue;

		if (!Entity->IsPlayer)
			continue;

		if (Entity->IsDead)
			continue;


		if (Entity->HitBoxes[HITBOX_HEAD].IsZero())
			continue;

		if (RadarInGame)
			if (*Entity->BaseEntity->IsSpotted() == false)
				*Entity->BaseEntity->IsSpotted() = true;

		if (!RadarActive)
			continue;

		PLAYER_TEAM EntTeam = Entity->Team;

		if (!Team && EntTeam == pLocal->Team)
			continue;

		if (!Enemy && EntTeam != pLocal->Team)
			continue;

		Color point_color = (EntTeam == TEAM_CT) ?
			(Entity->IsVisble ? ColorVisbleCT : ColorCT) :
			(Entity->IsVisble ? ColorVisbleTT : ColorCT);

		float screenx = 0;
		float screeny = 0;
		CalcRadarPoint(Entity->RenderOrigin, screenx, screeny);

		Vec2 DrawPos = X1Gui().GetCursorPos();
		Vec2 DrawSize = X1Gui().GetCurWindowSize();

		if (Trace)
		{
			Vector vAngle;
			I::Engine()->GetViewAngles(vAngle);

			float pEntAng = Entity->EyeAngle.y;

			if (pEntAng < 0)
				pEntAng = 180 + ((180 + pEntAng));

			float Rotate = (270 - pEntAng) + vAngle.y;

			if (TraceStyle == 0)
			{
				float SmeshX = (TraceLenght * cos(Rotate * PI / 180.f));
				float SmeshY = (TraceLenght * sin(Rotate * PI / 180.f));

				X1Gui().DrawLine(
					Vec2(screenx - 1, screeny - 1),
					Vec2(screenx + SmeshX, screeny + SmeshY),
					Color::White());
			}
			else
			{
				float Smesh1X = (TraceLenght * cos((Rotate - 100.f / 2.f) * PI / 180.f));
				float Smesh1Y = (TraceLenght * sin((Rotate - 100.f / 2.f) * PI / 180.f));

				float Smesh2X = (TraceLenght * cos((Rotate + 100.f / 2.f) * PI / 180.f));
				float Smesh2Y = (TraceLenght * sin((Rotate + 100.f / 2.f) * PI / 180.f));

				X1Gui().DrawLine(
					Vec2(screenx - 1, screeny - 1),
					Vec2(screenx + Smesh1X, screeny + Smesh1Y),
					Color::White());

				X1Gui().DrawLine(
					Vec2(screenx - 1, screeny - 1),
					Vec2(screenx + Smesh2X, screeny + Smesh2Y),
					Color::White());

				if (TraceStyle == 2)
				{
					float SmeshX = (TraceLenght * cos(Rotate * PI / 180.f));
					float SmeshY = (TraceLenght * sin(Rotate * PI / 180.f));

					X1Gui().DrawLine(
						Vec2(screenx - 1, screeny - 1),
						Vec2(screenx + SmeshX, screeny + SmeshY),
						Color::White());
				}
			}
		}

		int style = PointStyle;
		switch (PointStyle)
		{
		case 0: X1Gui().DrawFilledBox(Vec2((float)screenx - PointSize / 2.f, (float)screeny - PointSize / 2.f),
			Vec2(PointSize / 2.f, PointSize / 2.f), point_color); break;
		case 1:X1Gui().DrawBox(Vec2((float)screenx - PointSize / 2.f, (float)screeny - PointSize / 2.f),
			Vec2((float)screenx + PointSize / 2.f, (float)screeny + PointSize / 2.f), point_color); break;
		case 2:X1Gui().DrawCircle((float)screenx, (float)screeny, PointSize, 22, point_color); break;
		case 3:X1Gui().DrawRing((float)screenx, (float)screeny, PointSize, 22, point_color, 1); break;
		default: break;
		}

		if (Name)
		{
			CFont* old_font = &(*GP_Render->m_pFont);
			GP_Render->m_pFont = GP_Render->SzFonts[TextNameSize];
			Vec2 text_size = X1Gui().CalcTextSize(Entity->Name);
			Vec2 C = Vec2(screenx, screeny - PointSize - 2.f - text_size.y);

			C.x -= text_size.x / 2.1f;

			X1Gui().TextColored(NameColor, C, Entity->Name.c_str());

			GP_Render->m_pFont = old_font;

		}
	}
}

void CRadar::RenderSounds(CBaseEntity* plocal)
{
	/*if (GP_Esp)
	{
		if (plocal)
		{
			for (size_t i = 0; i < GP_Esp->SoundEsp.Sounds.size(); i++)
			{
				if (GP_Esp->SoundEsp.Sounds[i].Team != (PLAYER_TEAM)plocal->GetTeam() ? Enemy : Team)
				{
					float screenx = 0;
					float screeny = 0;
					float h = 0;
					CalcRadarPoint(GP_Esp->SoundEsp.Sounds[i].Pos, screenx, screeny, h);

					int style = SoundPointStyle;
					float size = (float)SoundPointSize;

					float Color_R = SoundColor.G1R();
					float Color_G = SoundColor.G1G();
					float Color_B = SoundColor.G1B();

					ImDrawList* Draw = ImGui::GetWindowDrawList();

					if (style == 0)
					{
						Draw->AddRectFilled(Vec2((float)screenx - size / 2.f, (float)screeny - size / 2.f),
							Vec2((float)screenx + size / 2.f, (float)screeny + size / 2.f),
							Color(Color_R, Color_G, Color_B, 1.f));
					}
					else if (style == 1)
					{
						Draw->AddRect(Vec2((float)screenx - size / 2.f, (float)screeny - size / 2.f),
							Vec2((float)screenx + size / 2.f, (float)screeny + size / 2.f),
							Color(Color_R, Color_G, Color_B, 1.f));
					}
					else if (style == 2)
					{
						Draw->AddCircleFilled(Vec2((float)screenx, (float)screeny),
							size,
							Color(Color_R, Color_G, Color_B, 1.f));
					}
					else if (style == 3)
					{
						Draw->AddCircle(Vec2((float)screenx, (float)screeny),
							size,
							Color(Color_R, Color_G, Color_B, 1.f));
					}
				}
				if (GP_Esp->SoundEsp.Sounds[i].EndTime <= I::GlobalVars()->curtime)
					GP_Esp->SoundEsp.Sounds.erase(GP_Esp->SoundEsp.Sounds.begin() + i);
			}
		}
	}*/


}

struct MapInfo
{
	string Name = "";
	Vector Pos = Vector(0, 0, 0);
	Vector Size = Vector(0, 0, 0);
};

MapInfo GetMap()
{
	const char* MapName = I::Engine()->GetLevelName();

	if (strstr(MapName, XorStr("ar_baggage")))		return { XorStr("ar_baggage"), Vector(-2150, 2280, 0), Vector(4100, 4100, 0) };
	if (strstr(MapName, XorStr("ar_dizzy")))		return { XorStr("ar_dizzy"), Vector(-2512, 1536, 0), Vector(3100, 3100, 0) };
	if (strstr(MapName, XorStr("ar_monastery")))    return { XorStr("ar_monastery"), Vector(-1687, 1640, 0), Vector(3100, 3100, 0) };
	if (strstr(MapName, XorStr("ar_shoots")))	    return { XorStr("ar_shoots"), Vector(-2150, 2582, 0), Vector(4100, 4100, 0) };
	if (strstr(MapName, XorStr("coop_cementplant")))return { XorStr("coop_cementplant"), Vector(-5160, 5859, 0), Vector(12100, 12100, 0) };

	if (strstr(MapName, XorStr("cs_agency")))	return { XorStr("cs_agency"), Vector(-2947, 2492, 0), Vector(5100, 5100, 0) };
	if (strstr(MapName, XorStr("cs_assault")))	return { XorStr("cs_assault"), Vector(4041, 7838, 0), Vector(4700, 4700, 0) };
	if (strstr(MapName, XorStr("cs_backalley")))return { XorStr("cs_backalley"), Vector(-2485, 3200, 0), Vector(3600, 3600, 0) };
	if (strstr(MapName, XorStr("cs_cruise")))	return { XorStr("cs_cruise"), Vector(-3507, 2498, 0), Vector(5100, 5100, 0) };
	if (strstr(MapName, XorStr("cs_insertion")))return { XorStr("cs_insertion"), Vector(-4888, 4884, 0), Vector(10100, 10100, 0) };
	if (strstr(MapName, XorStr("cs_italy")))	return { XorStr("cs_italy"), Vector(-2647, 2592, 0), Vector(4700, 4700, 0) };
	if (strstr(MapName, XorStr("cs_militia")))  return { XorStr("cs_militia"), Vector(-1474, 2296, 0), Vector(4600, 4600, 0) };
	if (strstr(MapName, XorStr("cs_office")))	return { XorStr("cs_office"), Vector(-1838, 1858, 0), Vector(4200, 4200, 0) };
	if (strstr(MapName, XorStr("cs_rush")))		return { XorStr("cs_rush"), Vector(-2950, 3350, 0), Vector(5300, 5300, 0) };
	if (strstr(MapName, XorStr("cs_siege")))	return { XorStr("cs_siege"), Vector(-1193, 3515, 0), Vector(6600, 6600, 0) };
	if (strstr(MapName, XorStr("cs_workout")))	return { XorStr("cs_workout"), Vector(-2454, 3125, 0), Vector(6600, 6600, 0) };

	if (strstr(MapName, XorStr("de_ali")))	    return { XorStr("de_ali"), Vector(-2064, 2920, 0), Vector(5100, 5100, 0) };
	if (strstr(MapName, XorStr("de_austria")))	return { XorStr("de_austria"), Vector(-2877, 2930, 0), Vector(5900, 5900, 0) };
	if (strstr(MapName, XorStr("de_aztec")))	return { XorStr("de_aztec"), Vector(-3200, 2841, 0), Vector(6100, 6100, 0) };
	if (strstr(MapName, XorStr("de_bank")))		return { XorStr("de_bank"), Vector(-2000, 1493, 0), Vector(4100, 4100, 0) };
	if (strstr(MapName, XorStr("de_bazaar")))	return { XorStr("de_bazaar"), Vector(-2434, 2179, 0), Vector(5100, 5100, 0) };
	if (strstr(MapName, XorStr("de_blackgold")))return { XorStr("de_blackgold"), Vector(-1100, 1425, 0), Vector(5400, 5400, 0) };
	if (strstr(MapName, XorStr("de_cache")))	return { XorStr("de_cache"), Vector(-2000, 3250, 0), Vector(5600, 5600, 0) };
	if (strstr(MapName, XorStr("de_canals")))	return { XorStr("de_canals"), Vector(-2496, 1792, 0), Vector(4100, 4100, 0) };
	if (strstr(MapName, XorStr("de_castle")))	return { XorStr("de_castle"), Vector(-3378, 2756, 0), Vector(5600, 5600, 0) };
	if (strstr(MapName, XorStr("de_cbble")))	return { XorStr("de_cbble"), Vector(-3840, 3072, 0), Vector(6100, 6100, 0) };
	if (strstr(MapName, XorStr("de_chinatown")))return { XorStr("de_chinatown"), Vector(-1735, 3232, 0), Vector(4100, 4100, 0) };
	if (strstr(MapName, XorStr("de_coast")))	return { XorStr("de_coast"), Vector(-3028, 4122, 0), Vector(5600, 5600, 0) };
	if (strstr(MapName, XorStr("de_dust2")))	return { XorStr("de_dust2"), Vector(-2476, 3239, 0), Vector(4500, 4500, 0) };
	if (strstr(MapName, XorStr("de_dust")))		return { XorStr("de_dust"), Vector(-2850, 4073, 0), Vector(6100, 6100, 0) };
	if (strstr(MapName, XorStr("de_empire")))	return { XorStr("de_empire"), Vector(-2165, 2000, 0), Vector(4600, 4600, 0) };
	if (strstr(MapName, XorStr("de_facade")))	return { XorStr("de_facade"), Vector(-90, 5659, 0), Vector(6000, 6000, 0) };
	if (strstr(MapName, XorStr("de_gwalior")))	return { XorStr("de_gwalior"), Vector(-1145, 2688, 0), Vector(5100, 5100, 0) };
	if (strstr(MapName, XorStr("de_inferno")))	return { XorStr("de_inferno"), Vector(-2087, 3870, 0), Vector(5000, 5000, 0) };
	if (strstr(MapName, XorStr("de_lake")))		return { XorStr("de_lake"), Vector(1200, -700, 0), Vector(5300, 5300, 0) };
	if (strstr(MapName, XorStr("de_lite")))		return { XorStr("de_lite"), Vector(-2012, 2928, 0), Vector(5100, 5100, 0) };
	if (strstr(MapName, XorStr("de_log")))		return { XorStr("de_log"), Vector(-411, 759, 0), Vector(6600, 6600, 0) };
	if (strstr(MapName, XorStr("de_marquis")))	return { XorStr("de_marquis"), Vector(-1877, 3199, 0), Vector(5100, 5100, 0) };
	if (strstr(MapName, XorStr("de_mikla")))	return { XorStr("de_mikla"), Vector(711, 2383, 0), Vector(4200, 4200, 0) };
	if (strstr(MapName, XorStr("de_mirage")))	return { XorStr("de_mirage"), Vector(-3230, 1713, 0), Vector(5100, 5100, 0) };
	if (strstr(MapName, XorStr("de_mist")))		return { XorStr("de_mist"), Vector(-5150, 2080, 0), Vector(4900, 4900, 0) };
	if (strstr(MapName, XorStr("de_nuke")))		return { XorStr("de_nuke"), Vector(-3453, 2887, 0), Vector(7100, 7100, 0) };
	if (strstr(MapName, XorStr("de_overgrown")))return { XorStr("de_overgrown"), Vector(-3376, 5563, 0), Vector(7100, 7100, 0) };
	if (strstr(MapName, XorStr("de_overpass")))	return { XorStr("de_overpass"), Vector(-4831, 1781, 0), Vector(5300, 5300, 0) };
	if (strstr(MapName, XorStr("de_rails")))	return { XorStr("de_rails"), Vector(-2199, 2874, 0), Vector(4600, 4600, 0) };
	if (strstr(MapName, XorStr("de_resort")))	return { XorStr("de_resort"), Vector(-506, 2713, 0), Vector(5600, 5600, 0) };
	if (strstr(MapName, XorStr("de_royal")))	return { XorStr("de_royal"), Vector(-2343, 2644, 0), Vector(4100, 4100, 0) };
	if (strstr(MapName, XorStr("de_ruins")))	return { XorStr("de_ruins"), Vector(-2443, 2485, 0), Vector(6350, 6350, 0) };
	if (strstr(MapName, XorStr("de_safehouse")))return { XorStr("de_safehouse"), Vector(-240, 2650, 0), Vector(4520, 4520, 0) };
	if (strstr(MapName, XorStr("de_santorini")))return { XorStr("de_santorini"), Vector(-2135, 1400, 0), Vector(4100, 4100, 0) };
	if (strstr(MapName, XorStr("de_seaside")))	return { XorStr("de_seaside"), Vector(-4161, 3680, 0), Vector(7100, 7100, 0) };
	if (strstr(MapName, XorStr("de_season")))	return { XorStr("de_season"), Vector(-1003, 2521, 0), Vector(5100, 5100, 0) };
	if (strstr(MapName, XorStr("de_shipped")))	return { XorStr("de_shipped"), Vector(-2432, 2663, 0), Vector(5900, 5900, 0) };
	if (strstr(MapName, XorStr("de_shortdust")))return { XorStr("de_shortdust"), Vector(-2318, 2337, 0), Vector(3700, 3700, 0) };
	if (strstr(MapName, XorStr("de_shortnuke")))return { XorStr("de_shortnuke"), Vector(-3453, 2887, 0), Vector(7100, 7100, 0) };
	if (strstr(MapName, XorStr("de_shorttrain")))return{ XorStr("de_shorttrain"), Vector(-2477, 2392, 0), Vector(4800, 4800, 0) };
	if (strstr(MapName, XorStr("de_stmarc")))	return { XorStr("de_stmarc"), Vector(-9383, 9099, 0), Vector(4100, 4100, 0) };
	if (strstr(MapName, XorStr("de_sugarcane")))return { XorStr("de_sugarcane"), Vector(-4015, 2000, 0), Vector(4350, 4350, 0) };
	if (strstr(MapName, XorStr("de_thrill")))	return { XorStr("de_thrill"), Vector(-3276, 2973, 0), Vector(5600, 5600, 0) };
	if (strstr(MapName, XorStr("de_train")))	return { XorStr("de_train"), Vector(-2477, 2392, 0), Vector(4800, 4800, 0) };
	if (strstr(MapName, XorStr("de_tulip")))	return { XorStr("de_tulip"), Vector(3402, 5583, 0), Vector(5600, 5600, 0) };
	if (strstr(MapName, XorStr("de_vertigo")))	return { XorStr("de_vertigo"), Vector(-3168, 1762, 0), Vector(4100, 4100, 0) };
	if (strstr(MapName, XorStr("de_zoo")))		return { XorStr("de_zoo"), Vector(-2435, 6116, 0), Vector(7100, 7100, 0) };
	if (strstr(MapName, XorStr("gd_bank")))		return { XorStr("gd_bank"), Vector(-2000, 1493, 0), Vector(4100, 4100, 0) };
	if (strstr(MapName, XorStr("gd_cbble")))	return { XorStr("gd_cbble"), Vector(-3840, 3072, 0), Vector(6100, 6100, 0) };
	if (strstr(MapName, XorStr("gd_crashsite")))return { XorStr("gd_crashsite"), Vector(-2212, 1437, 0), Vector(3600, 3600, 0) };
	if (strstr(MapName, XorStr("gd_lake")))		return { XorStr("gd_lake"), Vector(1200, -700, 0), Vector(5300, 5300, 0) };
	if (strstr(MapName, XorStr("gd_rialto")))	return { XorStr("gd_rialto"), Vector(-1260, 1836, 0), Vector(3100, 3100, 0) };
	if (strstr(MapName, XorStr("training1")))	return { XorStr("training1"), Vector(-2510, 2000, 0), Vector(5100, 5100, 0) };

	return { XorStr("Null"), Vector(0, 0, 0), Vector(5100, 5100, 0) };
}

void CRadar::DrawMapImage(CBaseEntity* plocal)
{
	MapInfo CurrentMap = GetMap();

	static string OldMapName = "";

	if (CurrentMap.Name != XorStr("Null"))
	{
		if (OldMapName != CurrentMap.Name)
			RadarMapImage = nullptr;

		if (RadarMapImage == nullptr)
			D3DXCreateTextureFromFileEx(GP_Render->m_pDevice, string(CSX::Utils::GetModuleBaseDir(0) + XorStr("\\csgo\\resource\\overviews\\") + CurrentMap.Name + XorStr("_radar.dds")).c_str(), 1223, 1223,
				D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &RadarMapImage);

		float MapH = CurrentMap.Size.y;
		float MapW = CurrentMap.Size.x;

		Vec2 MapStartPos(CurrentMap.Pos.x, CurrentMap.Pos.y);

		Vec2 MapPoint1 = MapStartPos;
		Vec2 MapPoint2(MapStartPos.x + MapW, MapStartPos.y);
		Vec2 MapPoint3(MapStartPos.x + MapW, MapStartPos.y - MapH);
		Vec2 MapPoint4(MapStartPos.x, MapStartPos.y - MapH);

		CalcRadarPoint(Vector(MapPoint1.x, MapPoint1.y, 0), MapPoint1.x, MapPoint1.y, false);
		CalcRadarPoint(Vector(MapPoint2.x, MapPoint2.y, 0), MapPoint2.x, MapPoint2.y, false);
		CalcRadarPoint(Vector(MapPoint3.x, MapPoint3.y, 0), MapPoint3.x, MapPoint3.y, false);
		CalcRadarPoint(Vector(MapPoint4.x, MapPoint4.y, 0), MapPoint4.x, MapPoint4.y, false);

		X1Gui().DrawImage(RadarMapImage, MapPoint2, MapPoint3, MapPoint4, MapPoint1);
	}
	OldMapName = CurrentMap.Name;
}

void CRadar::Draw()
{
	if (Enable)
	{
		if (RadarActive)
		{
			CBaseEntity* plocal = (CBaseEntity*)I::EntityList()->GetClientEntity(I::Engine()->GetLocalPlayer());
			if (plocal)
			{
				if (plocal->IsDead())
				{
					CBaseEntity* pObserverTarget = (CBaseEntity*)I::EntityList()->GetClientEntityFromHandle(plocal->GetObserverTarget());

					if (!pObserverTarget)
						return;

					plocal = pObserverTarget;
				}

				if (!plocal)
					return;

				if (plocal == (CBaseEntity*)0xE || plocal == (CBaseEntity*)0xF)
					return;


				if (!CGlobal::FullUpdateCheck)
				{
					LocalRendOrig = plocal->GetRenderOrigin();
					LocalViewOfst = plocal->GetViewOffset();
				}

				X1Gui().SetNextWindowPos(Vec2(RadarPosX, RadarPosY));

				Vec2 old_pad = X1Gui().GetStyle().wndPadding;
				color_t old_color = X1Gui().GetStyle().clrBackground;

				X1Gui().GetStyle().wndPadding = Vec2(0, 0);
				X1Gui().GetStyle().clrBackground = Color(ColorBackground.r(), ColorBackground.g(), ColorBackground.b(), Alpha);

				if (X1Gui().Begin(XorStr("RadarWin"), GWF_NoTitleBar, Vec2(RadarSizeX, RadarSizeY), Vec2(1, 1)))
				{
					Vec2 DrawPos = X1Gui().GetCursorPos();
					Vec2 DrawSize = X1Gui().GetCurWindowSize();

					if (TexturedRadar && CGlobal::IsGameReady)
						DrawMapImage(plocal);

					if (CGlobal::IsGameReady)
						RenderPlayer(plocal);

					if (Fov)
					{
						float FovStor = (DrawSize.y / tan(((180.f - (CGlobal::GFovView + (CGlobal::GFovView / 5.f))) / 2.f) * PI / 180.f));

						X1Gui().DrawLine(
							Vec2((DrawPos.x + DrawSize.x / 2.f) - FovStor / 2.f, DrawPos.y),
							Vec2(DrawPos.x + DrawSize.x / 2.f, DrawPos.y + DrawSize.y / 2.f),
							ColorLine);

						X1Gui().DrawLine(
							Vec2((DrawPos.x + DrawSize.x / 2.f) + FovStor / 2.f, DrawPos.y),
							Vec2(DrawPos.x + DrawSize.x / 2.f, DrawPos.y + DrawSize.y / 2.f),
							ColorLine);
					}
					if (Line)
					{
						X1Gui().DrawLine(
							Vec2(DrawPos.x + DrawSize.x / 2.f, DrawPos.y),
							Vec2(DrawPos.x + DrawSize.x / 2.f, DrawPos.y + DrawSize.y),
							ColorLine);

						X1Gui().DrawLine(
							Vec2(DrawPos.x, DrawPos.y + DrawSize.y / 2.f),
							Vec2(DrawPos.x + DrawSize.x, DrawPos.y + DrawSize.y / 2.f),
							ColorLine);
					}

					if (OutLine)
					{
						X1Gui().DrawBox(Vec2(DrawPos.x, DrawPos.y),
							Vec2(DrawPos.x + DrawSize.x, DrawPos.y + DrawSize.y),
							ColorLine, X1Gui().GetStyle().wndRounding);
					}

				}
				X1Gui().End();

				X1Gui().GetStyle().wndPadding = old_pad;
				X1Gui().GetStyle().clrBackground = old_color;
			}
		}
		if (RadarInGame)
		{
			if (CGlobal::IsGameReady)
			{
				CBaseEntity* plocal = (CBaseEntity*)I::EntityList()->GetClientEntity(I::Engine()->GetLocalPlayer());
				if (plocal)
				{
					RenderPlayer(plocal);
				}
			}
		}
	}
}