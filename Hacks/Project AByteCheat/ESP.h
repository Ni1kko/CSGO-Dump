#pragma once

#include "Hacks.h"
extern int missedLogHits[65];
struct Box
{
	int x = -1, y = -1, w = -1, h = -1;
};
class CEsp : public CHack
{
public:
	void Init();
	void Draw();
	void DrawBombus(IClientEntity * pEntity);
	void Move(CUserCmd *pCmd, bool &bSendPacket);
private:

	IClientEntity * BombCarrier;

	struct ESPBox
	{
		int x, y, w, h;
	};

	void DrawWeaponIcon(IClientEntity * pEntity, Box size);

	void multipoint(IClientEntity * pEntity, Box size);

	void DrawWeapon(IClientEntity * pEntity, Box size);

	void DrawWeapon2(IClientEntity * pEntity, Box size);

	void DrawAmmo(IClientEntity * pEntity, Box size);

	void ammo_text(IClientEntity * pEntity, Box size);

	void DrawAmmo2(IClientEntity * pEntity, Box size);

	void DrawPlayer(IClientEntity* pEntity, player_info_t pinfo);

	void drawgayarrows(IClientEntity * entity);
	void SpecList();

	Color GetPlayerColor(IClientEntity* pEntity);

	void DrawGun(IClientEntity * pEntity, CEsp::ESPBox size);
	void corner_box(Box size, Color color, IClientEntity * pEntity);


	void FilledBox(CEsp::ESPBox size, Color color);
	void default_box(Box size, Color color);
	void bracket_box(Box size, Color color);
	bool get_box(IClientEntity * m_entity, Box & box, bool dynamic);
	bool GetBox(IClientEntity* pEntity, ESPBox &result);

	void DrawName(player_info_t pinfo, Box size, IClientEntity* pEntity);
	void ghost_name(player_info_t pinfo, Box size, IClientEntity * pEntity);
	void hp_battery(IClientEntity * pEntity, Box size);

	float DistanceTo(Vector vecSrc, Vector vecDst);

	void armor_bottom(IClientEntity * pEntity, Box size);

	void armor_battery(IClientEntity * pEntity, Box size);

	void armor_default(IClientEntity * pEntity, Box size);

	void hp_default(IClientEntity * pEntity, Box size);


	void DrawInfo(IClientEntity * pEntity, Box size);


	void DrawInfo2(IClientEntity * pEntity, Box size);

	void DrawCross(IClientEntity* pEntity);

	void DrawSkeleton(IClientEntity* pEntity);

	void DrawSkeleton_team(IClientEntity * pEntity);

	void DrawSkeleton2(IClientEntity * pEntity);

	void BoxAndText(IClientEntity * entity, std::string text);
	void DrawThrowable(IClientEntity * throwable);

	void draw_direction_arrow(Vector2D center, float flYaw, float flDistCenter, float flLength, Color clr);

	void enemy_out_of_range(IClientEntity * player, Color color);

	void DrawDrop(IClientEntity* pEntity, ClientClass* cClass);
	void DrawDropIcon(IClientEntity * pEntity, ClientClass * cClass);
	void DrawBombPlanted(IClientEntity* pEntity, ClientClass* cClass);
	void DrawBomb(IClientEntity* pEntity, ClientClass* cClass);
};

