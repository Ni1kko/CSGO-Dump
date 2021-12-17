#pragma once
#include "../valve_sdk/csgostructs.hpp"
class C_BasePlayer;
class C_BaseEntity;
class C_BaseCombatWeapon;
class C_PlantedC4;
class Color;
class ClientClass;
class CUserCmd;
#include <vector>
extern unsigned long esp_font;
extern float ESP_Fade[64];
extern C_BasePlayer* local_observed;
namespace Visuals
{
    namespace Player
    {
        bool Begin(C_BasePlayer* pl);
        void RenderBox();
        void RenderName();
        void RenderHealth();
        void RenderArmour();
        void RenderWeapon();
		void BacktrackSkelt();
        void RenderSnapline();
    }
    namespace Misc
    {
		void RenderCrosshair();
        void RenderWeapon(C_BaseCombatWeapon* ent);
        void RenderDefuseKit(C_BaseEntity* ent);
        void RenderPlantedC4(C_BaseEntity* ent);
		void ThirdPerson();
		void ThirdPerson(QAngle);
    }
    bool CreateFonts();
    void DestroyFonts();
	void GetTextSize(unsigned long font, const char * txt, int & width, int & height);
	void Spectators();
	void DrawWatermark();
	RECT GetViewport();
	void DrawFilledRect(int x, int y, int w, int h);
	void SkyColor();
	bool IsVisibleScan(C_BasePlayer *player);
	void NightMode();
	bool ValidPlayer(C_BasePlayer * player, bool count_step);
	void DrawCapsuleOverlay(int idx, float duration);
	void LagCompHitbox(int index);
	void Draw3dCube(float scalar, QAngle angles, Vector middle_origin, Color outline);
	void Draw3DCube(float scalar, QAngle angles, Vector middle_origin, Color outline);
	void DrawString(unsigned long font, int x, int y, Color color, unsigned long alignment, const char * msg, ...);
	void DrawString(unsigned long font, bool center, int x, int y, Color c, const char * fmt, ...);
	void TextW(bool center, unsigned long font, int x, int y, Color c, wchar_t * pszString);
}
