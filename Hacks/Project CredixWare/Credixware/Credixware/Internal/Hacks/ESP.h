// Created by Credix - Licence: https://creativecommons.org/licenses/by-nc-sa/4.0/ 
#ifndef ESP_H
#define ESP_H

#include "../Headers/Interfaces.h"
#include "../Hacks/Settings.h"
#include "../Headers/MenuWrapper.h"

static BYTE iBoneLegL[] = { 24, 23, 22, 1 };
static BYTE iBoneLegR[] = { 27, 26, 25, 1 };

static BYTE iBoneBody[] = { 5, 4, 3, 2, 1 };

static BYTE iBoneArmL[] = { 9, 8, 7, 5 };
static BYTE iBoneArmR[] = { 15, 14, 13, 5 };

int enemiesNormalColor[4] = { 0, 0, 0, 0 };
int enemiesIgnoreZColor[4] = { 0, 0, 0, 0 };
int alliesNormalColor[4] = { 0, 0, 0, 0 };
int alliesIgnoreZColor[4] = { 0, 0, 0, 0 };


Vector2D GetBoneScreen(C_BaseEntity* BaseEntity, int index) {
	matrix3x4_t bonesMatrix[128];
	BaseEntity->SetupBones(bonesMatrix, 128, 0, 0.0f);
	matrix3x4_t boneMatrix = bonesMatrix[index];
	Vector boneVector = Vector(boneMatrix[0][3], boneMatrix[1][3], boneMatrix[2][3]);
	return WorldToScreen(boneVector);
}

void DrawBoneLine(C_BaseEntity* BaseEntity,int i1, int i2) {
	Vector2D boneScreen = GetBoneScreen(BaseEntity, i1);
	Vector2D boneScreen2 = GetBoneScreen(BaseEntity, i2);
	if (boneScreen.x < 5 || boneScreen.y < 5 || boneScreen2.x < 5 || boneScreen2.y < 5) {
		return;
	}
	else {
		DrawLine(boneScreen.x, boneScreen.y, boneScreen2.x, boneScreen2.y, enemiesIgnoreZColor);
	}
}

int w, h;
void DrawBones(C_BaseEntity* BaseEntity, const char* string) {
	return;
	//if (!Settings::Visuals::bBoneESP) {
	//	return;
	//}
	std::string str = std::string(str);
	if (!_strcmpi("models/player/custom_player/legacy/tm_leet_variantA.mdl", string)) {
		DrawBoneLine(BaseEntity, 77, 75);
		DrawBoneLine(BaseEntity, 75, 74);
		DrawBoneLine(BaseEntity, 74, 79);
		DrawBoneLine(BaseEntity, 79, 78);
		DrawBoneLine(BaseEntity, 78, 73);

		DrawBoneLine(BaseEntity, 70, 68);
		DrawBoneLine(BaseEntity, 68, 67);
		DrawBoneLine(BaseEntity, 67, 72);
		DrawBoneLine(BaseEntity, 72, 71);
		DrawBoneLine(BaseEntity, 71, 66);

		DrawBoneLine(BaseEntity, 73, 3);
		DrawBoneLine(BaseEntity, 66, 3);
		DrawBoneLine(BaseEntity, 3, 4);
		DrawBoneLine(BaseEntity, 4, 5);
		DrawBoneLine(BaseEntity, 5, 6);
		DrawBoneLine(BaseEntity, 6, 7);
		DrawBoneLine(BaseEntity, 7, 8);
		DrawBoneLine(BaseEntity, 7, 39);
		DrawBoneLine(BaseEntity, 7, 11);

		DrawBoneLine(BaseEntity, 39, 64);
		DrawBoneLine(BaseEntity, 64, 65);
		DrawBoneLine(BaseEntity, 65, 40);
		DrawBoneLine(BaseEntity, 40, 63);
		DrawBoneLine(BaseEntity, 63, 41);

		DrawBoneLine(BaseEntity, 11, 36);
		DrawBoneLine(BaseEntity, 36, 37);
		DrawBoneLine(BaseEntity, 37, 12);
		DrawBoneLine(BaseEntity, 12, 35);
		DrawBoneLine(BaseEntity, 35, 13);
	}
	else if (!_strcmpi("models/player/custom_player/legacy/tm_leet_variantB.mdl", string)) {
		DrawBoneLine(BaseEntity, 77, 75);
		DrawBoneLine(BaseEntity, 75, 74);
		DrawBoneLine(BaseEntity, 74, 79);
		DrawBoneLine(BaseEntity, 79, 78);
		DrawBoneLine(BaseEntity, 78, 73);

		DrawBoneLine(BaseEntity, 70, 68);
		DrawBoneLine(BaseEntity, 68, 67);
		DrawBoneLine(BaseEntity, 67, 72);
		DrawBoneLine(BaseEntity, 72, 71);
		DrawBoneLine(BaseEntity, 71, 66);

		DrawBoneLine(BaseEntity, 73, 3);
		DrawBoneLine(BaseEntity, 66, 3);
		DrawBoneLine(BaseEntity, 3, 4);
		DrawBoneLine(BaseEntity, 4, 5);
		DrawBoneLine(BaseEntity, 5, 6);
		DrawBoneLine(BaseEntity, 6, 7);
		DrawBoneLine(BaseEntity, 7, 8);
		DrawBoneLine(BaseEntity, 7, 39);
		DrawBoneLine(BaseEntity, 7, 11);

		DrawBoneLine(BaseEntity, 39, 64);
		DrawBoneLine(BaseEntity, 64, 65);
		DrawBoneLine(BaseEntity, 65, 40);
		DrawBoneLine(BaseEntity, 40, 63);
		DrawBoneLine(BaseEntity, 63, 41);

		DrawBoneLine(BaseEntity, 11, 36);
		DrawBoneLine(BaseEntity, 36, 37);
		DrawBoneLine(BaseEntity, 37, 12);
		DrawBoneLine(BaseEntity, 12, 35);
		DrawBoneLine(BaseEntity, 35, 13);
	}
	else if (!_strcmpi("models/player/custom_player/legacy/tm_leet_variantC.mdl", string)) {
		/*for (int i = 0; i < 99; i++) {
			Vector2D bone = GetBoneScreen(BaseEntity, i);
			char buf[52] = { 0 };
			sprintf(buf, "%i", i);
			DrawString(bone.x, bone.y, buf);
		}*/
		DrawBoneLine(BaseEntity, 77, 75);
		DrawBoneLine(BaseEntity, 75, 74);
		DrawBoneLine(BaseEntity, 74, 79);
		DrawBoneLine(BaseEntity, 79, 78);
		DrawBoneLine(BaseEntity, 78, 73);

		DrawBoneLine(BaseEntity, 70, 68);
		DrawBoneLine(BaseEntity, 68, 67);
		DrawBoneLine(BaseEntity, 67, 72);
		DrawBoneLine(BaseEntity, 72, 71);
		DrawBoneLine(BaseEntity, 71, 66);

		DrawBoneLine(BaseEntity, 73, 3);
		DrawBoneLine(BaseEntity, 66, 3);
		DrawBoneLine(BaseEntity, 3, 4);
		DrawBoneLine(BaseEntity, 4, 5);
		DrawBoneLine(BaseEntity, 5, 6);
		DrawBoneLine(BaseEntity, 6, 7);
		DrawBoneLine(BaseEntity, 7, 8);
		DrawBoneLine(BaseEntity, 7, 39);
		DrawBoneLine(BaseEntity, 7, 11);

		DrawBoneLine(BaseEntity, 39, 65);
		DrawBoneLine(BaseEntity, 65, 40);
		DrawBoneLine(BaseEntity, 40, 63);
		DrawBoneLine(BaseEntity, 63, 41);

		DrawBoneLine(BaseEntity, 11, 37);
		DrawBoneLine(BaseEntity, 37, 12);
		DrawBoneLine(BaseEntity, 12, 35);
		DrawBoneLine(BaseEntity, 35, 13);
	}
	else if (!_strcmpi("models/player/custom_player/legacy/tm_leet_variantD.mdl", string)) {
		DrawBoneLine(BaseEntity, 77, 75);
		DrawBoneLine(BaseEntity, 75, 74);
		DrawBoneLine(BaseEntity, 74, 79);
		DrawBoneLine(BaseEntity, 79, 78);
		DrawBoneLine(BaseEntity, 78, 73);

		DrawBoneLine(BaseEntity, 70, 68);
		DrawBoneLine(BaseEntity, 68, 67);
		DrawBoneLine(BaseEntity, 67, 72);
		DrawBoneLine(BaseEntity, 72, 71);
		DrawBoneLine(BaseEntity, 71, 66);

		DrawBoneLine(BaseEntity, 73, 3);
		DrawBoneLine(BaseEntity, 66, 3);
		DrawBoneLine(BaseEntity, 3, 4);
		DrawBoneLine(BaseEntity, 4, 5);
		DrawBoneLine(BaseEntity, 5, 6);
		DrawBoneLine(BaseEntity, 6, 7);
		DrawBoneLine(BaseEntity, 7, 8);
		DrawBoneLine(BaseEntity, 7, 39);
		DrawBoneLine(BaseEntity, 7, 11);

		DrawBoneLine(BaseEntity, 39, 64);
		DrawBoneLine(BaseEntity, 64, 65);
		DrawBoneLine(BaseEntity, 65, 40);
		DrawBoneLine(BaseEntity, 40, 63);
		DrawBoneLine(BaseEntity, 63, 41);

		DrawBoneLine(BaseEntity, 11, 36);
		DrawBoneLine(BaseEntity, 36, 37);
		DrawBoneLine(BaseEntity, 37, 12);
		DrawBoneLine(BaseEntity, 12, 35);
		DrawBoneLine(BaseEntity, 35, 13);
	}
	else if (!_strcmpi("models/player/custom_player/legacy/tm_leet_variantE.mdl", string)) {
		DrawBoneLine(BaseEntity, 77, 75);
		DrawBoneLine(BaseEntity, 75, 74);
		DrawBoneLine(BaseEntity, 74, 79);
		DrawBoneLine(BaseEntity, 79, 78);
		DrawBoneLine(BaseEntity, 78, 73);

		DrawBoneLine(BaseEntity, 70, 68);
		DrawBoneLine(BaseEntity, 68, 67);
		DrawBoneLine(BaseEntity, 67, 72);
		DrawBoneLine(BaseEntity, 72, 71);
		DrawBoneLine(BaseEntity, 71, 66);

		DrawBoneLine(BaseEntity, 73, 3);
		DrawBoneLine(BaseEntity, 66, 3);
		DrawBoneLine(BaseEntity, 3, 4);
		DrawBoneLine(BaseEntity, 4, 5);
		DrawBoneLine(BaseEntity, 5, 6);
		DrawBoneLine(BaseEntity, 6, 7);
		DrawBoneLine(BaseEntity, 7, 8);
		DrawBoneLine(BaseEntity, 7, 39);
		DrawBoneLine(BaseEntity, 7, 11);

		DrawBoneLine(BaseEntity, 39, 65);
		DrawBoneLine(BaseEntity, 65, 40);
		DrawBoneLine(BaseEntity, 40, 63);
		DrawBoneLine(BaseEntity, 63, 41);

		DrawBoneLine(BaseEntity, 11, 37);
		DrawBoneLine(BaseEntity, 37, 12);
		DrawBoneLine(BaseEntity, 12, 35);
		DrawBoneLine(BaseEntity, 35, 13);
	}
	else if (str.find("models/player/custom_player/legacy/ctm_sas_variantA.mdl")) {
		
	}
	else if (str.find("models/player/custom_player/legacy/ctm_sas_variantB.mdl")) {
		
	}
	else if (str.find("models/player/custom_player/legacy/ctm_sas_variantC.mdl")) {
		
	}
	else if (str.find("models/player/custom_player/legacy/ctm_sas_variantD.mdl")) {
		
	}
	else if (str.find("models/player/custom_player/legacy/ctm_sas_variantE.mdl")) {
		
	}
}

int screenWidth, screenHeight;
namespace ESP {
	void Init() {
		enemiesNormalColor[0] = Settings::Visuals::espEnemiesNormalR;
		enemiesNormalColor[1] = Settings::Visuals::espEnemiesNormalG;
		enemiesNormalColor[2] = Settings::Visuals::espEnemiesNormalB;
		enemiesNormalColor[3] = 255;

		enemiesIgnoreZColor[0] = Settings::Visuals::espEnemiesIgnoreZR;
		enemiesIgnoreZColor[1] = Settings::Visuals::espEnemiesIgnoreZG;
		enemiesIgnoreZColor[2] = Settings::Visuals::espEnemiesIgnoreZB;
		enemiesIgnoreZColor[3] = 255;

		alliesNormalColor[0] = Settings::Visuals::espAlliesNormalR;
		alliesNormalColor[1] = Settings::Visuals::espAlliesNormalG;
		alliesNormalColor[2] = Settings::Visuals::espAlliesNormalB;
		alliesNormalColor[3] = 255;

		alliesIgnoreZColor[0] = Settings::Visuals::espAlliesIgnoreZR;
		alliesIgnoreZColor[1] = Settings::Visuals::espAlliesIgnoreZG;
		alliesIgnoreZColor[2] = Settings::Visuals::espAlliesIgnoreZB;
		alliesIgnoreZColor[3] = 255;
		g_pEngineClient->GetScreenSize(screenWidth, screenHeight);
	}
	void Draw() {
		// TODO: Add ray tracing for visiblity check
		if (!Settings::Visuals::bEspEnabled) {
			return;
		}
		IClientEntity* LocalEntity = g_pClientEntityList->GetClientEntity(g_pEngineClient->GetLocalPlayer());
		if (!LocalEntity)
			return;

		ClientClass* LocalEntityClass = LocalEntity->GetClientClass();
		if (!LocalEntityClass)
			return;

		C_BaseEntity* LocalBaseEntity = LocalEntity->GetBaseEntity();
		if (!LocalBaseEntity)
			return;

		int LocalTeam = *reinterpret_cast<int*>((DWORD)LocalEntity->GetBaseEntity() + Offsets::m_iTeamNum);

		Vector LocalPlayerLoc = GetBone(LocalBaseEntity, 1);

		for (int i = 0; i < g_pClientEntityList->GetHighestEntityIndex(); i++) {
			IClientEntity* Entity = g_pClientEntityList->GetClientEntity(i);
			if (!Entity)
				continue;

			ClientClass* EntityClass = Entity->GetClientClass();
			if (!EntityClass)
				continue;

			if (EntityClass->m_ClassID != CCSPlayer || Entity->entindex() == g_pEngineClient->GetLocalPlayer())
				continue;

			int EntityTeam = *reinterpret_cast<int*>((DWORD)Entity->GetBaseEntity() + Offsets::m_iTeamNum);
			if ((Settings::Visuals::bESPEnemiesNormal || Settings::Visuals::bESPEnemiesIgnoreZ) && LocalTeam != EntityTeam) {
				C_BaseEntity* BaseEntity = Entity->GetBaseEntity();
				if (!BaseEntity)
					continue;

				int health = *reinterpret_cast<int*>((DWORD)Entity->GetBaseEntity() + Offsets::m_iHealth);
				if (health <= 0 || BaseEntity->IsDormant())
					continue;

				const char* modelName = g_pModelInfo->GetModelName(Entity->GetModel());

				Vector top = GetBone(BaseEntity, 8);
				top.z += 10.0f;
				Vector bottom = GetBone(BaseEntity, 1);
				Vector2D topScreen = WorldToScreen(top);
				Vector2D bottomScreen = WorldToScreen(bottom);
				float height = topScreen.y - bottomScreen.y;
				float width = height / 3;
				int centery = topScreen.y + (height / 2);
				int topx = bottomScreen.x - width;
				int topy = bottomScreen.x += width;
				int botx = centery - ((height - 2) * 1.3);
				int boty = (centery + ((height / 2) * 1.2));

				player_info_t EntityInfo;
				g_pEngineClient->GetPlayerInfo(Entity->entindex(), &EntityInfo);

				// TODO: Make optimization less aggresive
				if ((topx > screenWidth || topx <= 1) ||
					(topy > screenWidth || topy <= 1) ||
					(botx > screenWidth || botx <= 1) ||
					(boty > screenWidth || boty <= 1)) {
					continue;
				}

				if (Settings::Visuals::espBoxType == ESPBOX_DEFAULT) {
					DrawLine(topx, topScreen.y, topy, topScreen.y, enemiesIgnoreZColor);
					DrawLine(topx, bottomScreen.y, topy, bottomScreen.y, enemiesIgnoreZColor);
					DrawLine(topx, bottomScreen.y, topx, topScreen.y, enemiesIgnoreZColor);
					DrawLine(topy, bottomScreen.y, topy, topScreen.y, enemiesIgnoreZColor);
				}
				else if (Settings::Visuals::espBoxType == ESPBOX_CORNERED) {
					DrawLine(topy, topScreen.y, topy + 5, topScreen.y, enemiesIgnoreZColor);
					DrawLine(topy, topScreen.y, topy, topScreen.y + 5, enemiesIgnoreZColor);

					DrawLine(topx, topScreen.y, topx - 5, topScreen.y, enemiesIgnoreZColor);
					DrawLine(topx, topScreen.y, topx, topScreen.y + 5, enemiesIgnoreZColor);

					DrawLine(topy, bottomScreen.y, topy + 5, bottomScreen.y, enemiesIgnoreZColor);
					DrawLine(topy, bottomScreen.y, topy, bottomScreen.y - 5, enemiesIgnoreZColor);

					DrawLine(topx, bottomScreen.y, topx - 5, bottomScreen.y, enemiesIgnoreZColor);
					DrawLine(topx, bottomScreen.y, topx, bottomScreen.y - 5, enemiesIgnoreZColor);
				}
				else if (Settings::Visuals::espBoxType == ESPBOX_LINES) {
					DrawLine(topx, bottomScreen.y, topy, bottomScreen.y, enemiesIgnoreZColor);
					DrawLine(topy, bottomScreen.y, topy, topScreen.y, enemiesIgnoreZColor);
				}
				else if (Settings::Visuals::espBoxType == ESPBOX_VERTICALLINE) {
					DrawLine(topy, bottomScreen.y, topy, topScreen.y, enemiesIgnoreZColor);
				}
				else if (Settings::Visuals::espBoxType == ESPBOX_HORIZONTALLINE) {
					DrawLine(topx, bottomScreen.y, topy, bottomScreen.y, enemiesIgnoreZColor);
				}

				if (Settings::Visuals::bESPName) {
					GetStringSize(w, h, EntityInfo.player_name);
					DrawString(topScreen.x - (w / 2), topScreen.y - h, EntityInfo.player_name, enemiesIgnoreZColor);
				}

				char healthbuf[52] = { 0 };
				sprintf(healthbuf, "%i HP", health);
				char healthbuf2[52] = { 0 };
				sprintf(healthbuf2, "%i", health);
				if (Settings::Visuals::bESPHealth) {
					GetStringSize(w, h, healthbuf);
					DrawString(topScreen.x - (w / 2), bottomScreen.y + 5, healthbuf, enemiesIgnoreZColor);
				}

				if (Settings::Visuals::espHealtBar == HEALTHBAR_BAR) {
					float height = (float)bottomScreen.y - (float)topScreen.y - 1.0f;
					int r = (float)(100 - health) * 2.55f;
					int g = (float)(health) * 2.55f;
					int health_color[4] = { (int)r, (int)g, 0, 255 };
					DrawBox(topy - 6, topScreen.y, 4, height + 2, enemiesIgnoreZColor);
					DrawBox(topy - 5, topScreen.y + 1 + (height - ((float)health * height) / 100.0f), 2, ((float)health * height) / 100.0f, health_color);
				} else if (Settings::Visuals::espHealtBar == HEALTHBAR_TEXT) {
					float height = (float)bottomScreen.y - (float)topScreen.y - 1.0f;
					int r = (float)(100 - health) * 2.55f;
					int g = (float)(health) * 2.55f;
					int health_color[4] = { (int)r, (int)g, 0, 255 };
					DrawBox(topy - 6, topScreen.y, 4, height + 2, enemiesIgnoreZColor);
					DrawBox(topy - 5, topScreen.y + 1 + (height - ((float)health * height) / 100.0f), 2, ((float)health * height) / 100.0f, health_color);
					GetStringSize(w, h, healthbuf2, fontSmall);
					DrawString(topy - (w / 2) - 3, topScreen.y + 1 + (height - ((float)health * height) / 100.0f) - (h/2), healthbuf2, fontSmall, enemiesIgnoreZColor);
				}

				if (Settings::Visuals::bESPBone) {
					DrawBones(BaseEntity, g_pModelInfo->GetModelName(Entity->GetModel()));
				}
			}
			else if ((Settings::Visuals::bESPAlliesNormal || Settings::Visuals::bESPAlliesIgnoreZ) && LocalTeam == EntityTeam) {
				C_BaseEntity* BaseEntity = Entity->GetBaseEntity();
				if (!BaseEntity)
					continue;

				int health = *reinterpret_cast<int*>((DWORD)Entity->GetBaseEntity() + Offsets::m_iHealth);
				if (health <= 0 || BaseEntity->IsDormant())
					continue;

				const char* modelName = g_pModelInfo->GetModelName(Entity->GetModel());

				Vector top = GetBone(BaseEntity, 8);
				top.z += 10.0f;
				Vector bottom = GetBone(BaseEntity, 1);
				Vector2D topScreen = WorldToScreen(top);
				Vector2D bottomScreen = WorldToScreen(bottom);
				float height = topScreen.y - bottomScreen.y;
				float width = height / 3;
				int centery = topScreen.y + (height / 2);
				int topx = bottomScreen.x - width;
				int topy = bottomScreen.x += width;
				int botx = centery - ((height - 2) * 1.3);
				int boty = (centery + ((height / 2) * 1.2));

				player_info_t EntityInfo;
				g_pEngineClient->GetPlayerInfo(Entity->entindex(), &EntityInfo);

				// TODO: Make optimization less aggresive
				if ((topx > screenWidth || topx <= 1) ||
					(topy > screenWidth || topy <= 1) ||
					(botx > screenWidth || botx <= 1) ||
					(boty > screenWidth || boty <= 1)) {
					continue;
				}

				if (Settings::Visuals::espBoxType == ESPBOX_DEFAULT) {
					DrawLine(topx, topScreen.y, topy, topScreen.y, alliesIgnoreZColor);
					DrawLine(topx, bottomScreen.y, topy, bottomScreen.y, alliesIgnoreZColor);
					DrawLine(topx, bottomScreen.y, topx, topScreen.y, alliesIgnoreZColor);
					DrawLine(topy, bottomScreen.y, topy, topScreen.y, alliesIgnoreZColor);
				}
				else if (Settings::Visuals::espBoxType == ESPBOX_CORNERED) {
					DrawLine(topx, topScreen.y, topy, topScreen.y, alliesIgnoreZColor);
					DrawLine(topx, bottomScreen.y, topy, bottomScreen.y, alliesIgnoreZColor);
					DrawLine(topx, bottomScreen.y, topx, topScreen.y, alliesIgnoreZColor);
					DrawLine(topy, bottomScreen.y, topy, topScreen.y, alliesIgnoreZColor);
				}
				else if (Settings::Visuals::espBoxType == ESPBOX_LINES) {
					DrawLine(topx, bottomScreen.y, topy, bottomScreen.y, alliesIgnoreZColor);
					DrawLine(topy, bottomScreen.y, topy, topScreen.y, alliesIgnoreZColor);
				}
				else if (Settings::Visuals::espBoxType == ESPBOX_VERTICALLINE) {
					DrawLine(topy, bottomScreen.y, topy, topScreen.y, alliesIgnoreZColor);
				}
				else if (Settings::Visuals::espBoxType == ESPBOX_HORIZONTALLINE) {
					DrawLine(topx, bottomScreen.y, topy, bottomScreen.y, alliesIgnoreZColor);
				}

				if (Settings::Visuals::bESPName) {
					GetStringSize(w, h, EntityInfo.player_name);
					DrawString(topScreen.x - (w / 2), topScreen.y - h, EntityInfo.player_name, alliesIgnoreZColor);
				}

				char healthbuf[52] = { 0 };
				sprintf(healthbuf, "%i HP", health);
				char healthbuf2[52] = { 0 };
				sprintf(healthbuf2, "%i", health);
				if (Settings::Visuals::bESPHealth) {
					GetStringSize(w, h, healthbuf);
					DrawString(topScreen.x - (w / 2), bottomScreen.y + 5, healthbuf, alliesIgnoreZColor);
				}

				if (Settings::Visuals::espHealtBar == HEALTHBAR_BAR) {
					float height = (float)bottomScreen.y - (float)topScreen.y - 1.0f;
					int r = (float)(100 - health) * 2.55f;
					int g = (float)(health) * 2.55f;
					int health_color[4] = { (int)r, (int)g, 0, 255 };
					DrawBox(topy - 6, topScreen.y, 4, height + 2, alliesIgnoreZColor);
					DrawBox(topy - 5, topScreen.y + 1 + (height - ((float)health * height) / 100.0f), 2, ((float)health * height) / 100.0f, health_color);
				}
				else if (Settings::Visuals::espHealtBar == HEALTHBAR_TEXT) {
					float height = (float)bottomScreen.y - (float)topScreen.y - 1.0f;
					int r = (float)(100 - health) * 2.55f;
					int g = (float)(health) * 2.55f;
					int health_color[4] = { (int)r, (int)g, 0, 255 };
					DrawBox(topy - 6, topScreen.y, 4, height + 2, alliesIgnoreZColor);
					DrawBox(topy - 5, topScreen.y + 1 + (height - ((float)health * height) / 100.0f), 2, ((float)health * height) / 100.0f, health_color);
					GetStringSize(w, h, healthbuf2, fontSmall);
					DrawString(topy - (w / 2), topScreen.y + 1 + (height - ((float)health * height) / 100.0f) - (height / 2), healthbuf2, fontSmall, alliesIgnoreZColor);
				}

				if (Settings::Visuals::bESPBone) {
					DrawBones(BaseEntity, g_pModelInfo->GetModelName(Entity->GetModel()));
				}
			}



			C_BaseEntity* EntityBase = (C_BaseEntity*)g_pClientEntityList->GetClientEntity(i);
		}
	}
};

#endif