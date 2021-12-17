
#include "visuals.hpp"

#include "../Options.hpp"
#include "../ValveSDK/csgostructs.hpp"
#include "../Helpers/math.hpp"
#include "../Helpers/utils.hpp"

#include <fstream>
#include <utility>

float CSGO_Armor(float flDamage, int ArmorValue)
{
    float flArmorRatio = 0.5f;
    float flArmorBonus = 0.5f;
    if (ArmorValue > 0) {
        float flNew = flDamage * flArmorRatio;
        float flArmor = (flDamage - flNew) * flArmorBonus;

        if (flArmor > static_cast<float>(ArmorValue)) {
            flArmor = static_cast<float>(ArmorValue) * (1.f / flArmorBonus);
            flNew = flDamage - flArmor;
        }

        flDamage = flNew;
    }
    return flDamage;
}
#define TICK_INTERVAL			(g_GlobalVars->interval_per_tick)
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
float damage;
char bombdamagestringdead[24];
char bombdamagestringalive[24];
C_BasePlayer *BombCarrier = nullptr;




RECT GetTextSize(DWORD font, const char* text)
{
    size_t origsize = strlen(text) + 1;
    const size_t newsize = 100;
    size_t convertedChars = 0;
    wchar_t wcstring[newsize];
    mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

    RECT rect; int x, y;
    g_VGuiSurface->GetTextSize(font, wcstring, x, y);
    rect.left = x; rect.bottom = y;
    rect.right = x;
    return rect;
}

vgui::HFont esp_font;
vgui::HFont defuse_font;
vgui::HFont dropped_weapons_font;
vgui::HFont weapons_font;
bool visuals::create_fonts()
{
    esp_font = g_VGuiSurface->CreateFont_();
    defuse_font = g_VGuiSurface->CreateFont_();
    dropped_weapons_font = g_VGuiSurface->CreateFont_();
    weapons_font = g_VGuiSurface->CreateFont_();

    g_VGuiSurface->SetFontGlyphSet(esp_font, "Tahoma", 11, 700, 0, 0, FONTFLAG_DROPSHADOW);
    g_VGuiSurface->SetFontGlyphSet(defuse_font, "Tahoma", 15, 700, 0, 0, FONTFLAG_DROPSHADOW);
    g_VGuiSurface->SetFontGlyphSet(weapons_font, "Tahoma", 10, 700, 0, 0, FONTFLAG_DROPSHADOW);
    g_VGuiSurface->SetFontGlyphSet(dropped_weapons_font, "Tahoma", 8, 700, 0, 0, FONTFLAG_DROPSHADOW);

    return true;
}
//--------------------------------------------------------------------------------
void visuals::destroy_fonts()
{

}
//--------------------------------------------------------------------------------




void visuals::paint_traverse()
{

    if (g_Options.esp_enabled.get_bool())
    {
        auto local = C_BasePlayer::get_local_player();
        for (int i = 0; i < g_EntityList->GetHighestEntityIndex(); i++)
        {
            
            C_BasePlayer *entity = (C_BasePlayer*)g_EntityList->GetClientEntity(i);
            player_info_t pinfo;
            if (entity == local && local->is_alive() && g_EngineClient->GetPlayerInfo(g_EngineClient->GetLocalPlayer(), &pinfo))
            {
                if (g_Input->m_fCameraInThirdPerson)
                {
                    Vector max = entity->GetCollideable()->OBBMaxs();
                    Vector pos, pos3D;
                    Vector top, top3D;
                    pos3D = entity->m_vecOrigin();
                    top3D = pos3D + Vector(0, 0, max.z);

                    if (!math::world_to_screen(pos3D, pos) || !math::world_to_screen(top3D, top))
                        return;

                    static auto height = (pos.y - top.y);
                    static auto width = height / 4.f;
                    if (g_Options.esp_poland_box.get_bool())
                    {
                        Color color = misc::GetPlayerColor(entity);
                        player::player_box(top.x, top.y, width, height, color);
                    }
                    if (g_Options.esp_poland_hp.get_bool())
                        player::draw_health(pos, top, local->m_iHealth());

                    if (g_Options.esp_poland_name.get_bool())
                        DrawString2(esp_font, (int)top.x, (int)top.y - 6, Color::White, FONT_CENTER, pinfo.szName);

                }
            }
            if (entity &&  entity != local && !entity->IsDormant())
            {
                if (g_EngineClient->GetPlayerInfo(i, &pinfo) && entity->is_alive())
                {
                    if (g_Options.legitbot_backtrack.get_bool())
                    {

                        for (int t = 0; t <= 12; ++t)
                        {
                            Vector screenbacktrack[64][16];
                            if (headPositions[i][t].simtime > local->m_flSimulationTime() - 1)
                            {
                                if (math::world_to_screen(headPositions[i][t].hitboxPos, screenbacktrack[i][t]))
                                {

                                    g_VGuiSurface->DrawSetColor(Color::Red);
                                    g_VGuiSurface->DrawOutlinedRect(screenbacktrack[i][t].x, screenbacktrack[i][t].y, screenbacktrack[i][t].x + 2, screenbacktrack[i][t].y + 2);

                                }
                            }
                        }
                    }

                    player::draw_player(entity, pinfo, local);
                }

                ClientClass* cClass = (ClientClass*)entity->GetClientClass();
                if (g_Options.esp_poland_worldweapon.get_bool() && cClass->m_ClassID != CBaseWeaponWorldModel && ((strstr(cClass->m_pNetworkName, "Weapon") || cClass->m_ClassID == CDEagle || cClass->m_ClassID == CAK47)))
                {
                    misc::draw_drop(entity, cClass);
                }
                if (g_Options.esp_poland_c4world.get_bool())
                {
                    if (cClass->m_ClassID == CPlantedC4)
                        misc::draw_bomb_planted(entity, cClass);
                }

                if (cClass->m_ClassID == CC4)
                    misc::draw_bomb(entity, cClass);
                if (g_Options.esp_poland_grenade.get_bool() && strstr(cClass->m_pNetworkName, "Projectile"))
                {
                    misc::render_grenade(entity);
                }
            }
        }
        

        if (g_Options.esp_poland_spread_crosshair.get_bool())
        {
            static auto width = 0;
            static auto height = 0;
            
            g_EngineClient->GetScreenSize(width, height);
            if (local  && local->is_alive())
            {
                C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)local->m_hActiveWeapon().Get();
                float cone = pWeapon->get_spread() + pWeapon->get_inaccuracy();
                if (cone > 0.0f)
                {
                    int crossX = width / 2;
                    int crossY = height / 2;
                    if (cone < 0.01f) cone = 0.01f;
                    float size = (cone * height) * 0.7f;
                    g_VGuiSurface->DrawSetColor(Color(255, 0, 0, 100));
                    g_VGuiSurface->DrawOutlinedCircle(crossX, crossY, (int)size, 48);
                }
            }
        }
    }
}



std::string CleanItemName(std::string name)
{
    std::string Name = name;
    // Tidy up the weapon Name
    if (Name[0] == 'C')
        Name.erase(Name.begin());

    // Remove the word Weapon
    auto startOfWeap = Name.find("Weapon");
    if (startOfWeap != std::string::npos)
        Name.erase(Name.begin() + startOfWeap, Name.begin() + startOfWeap + 6);

    return Name;
}

wchar_t* CharToWideChar(const char* text)
{
    size_t size = strlen(text) + 1;
    wchar_t* wa = new wchar_t[size];
    mbstowcs_s(NULL, wa, size / 4, text, size);
    return wa;
}
Color visuals::misc::GetPlayerColor(C_BasePlayer* entity)
{
    auto local = C_BasePlayer::get_local_player();

    Color color;

    if (local->can_see_player(entity, 0))
        color = Color(235, 200, 0, 255);
    else
        color = Color(235, 50, 0, 255);

    return color;
}
bool done = false;
void visuals::draw_model_execute(const ModelRenderInfo_t& pInfo)
{
    misc::wireframe_hands(pInfo);
    misc::night_mode();
}

void visuals::player::draw_player(C_BasePlayer* entity, player_info_t pinfo, C_BasePlayer* local)
{

    Vector max = entity->GetCollideable()->OBBMaxs();
    Vector pos, pos3D;
    Vector top, top3D;
    pos3D = entity->m_vecOrigin();
    top3D = pos3D + Vector(0, 0, max.z);
    if (g_Options.esp_poland_filter_enemies_only.get_bool() && (entity->m_iTeamNum() == local->m_iTeamNum()))
        return;

    if (g_Options.esp_poland_dlight.get_bool())
        dlight(entity);

    if (!math::world_to_screen(pos3D, pos) || !math::world_to_screen(top3D, top))
        return;

    auto height = (pos.y - top.y);
    auto width = height / 4.f;






    if (g_Options.esp_poland_box.get_bool())
        player_box(top.x, top.y, width, height, misc::GetPlayerColor(entity));

    if (g_Options.esp_poland_hp.get_bool())
        draw_health(pos, top, entity->m_iHealth());

    if (g_Options.esp_poland_name.get_bool())
        DrawString2(esp_font, (int)top.x, (int)top.y - 6, Color::White, FONT_CENTER, pinfo.szName);

    int bottom = 0;

    std::vector<std::string> weapon;
    std::vector<std::string> bomb;

    auto pWeapon = entity->m_hActiveWeapon().Get();
    if (g_Options.esp_poland_weapon.get_bool() && pWeapon)
    {
        ClientClass* cClass = (ClientClass*)pWeapon->GetClientClass();
        if (cClass)
        {
            // Draw it
            weapon.push_back(CleanItemName(cClass->m_pNetworkName));
        }
    }

    if (g_Options.esp_poland_c4.get_bool() && entity == BombCarrier)
    {
        bomb.push_back("Bomb");
    }

    int i = 0;
    if (g_Options.esp_poland_weapon.get_bool())
    {

        for (auto Text : weapon)
        {
            DrawString2((DWORD)weapons_font, (int)top.x, int(top.y + height + 8 + (10 * bottom++)), Color::White, FONT_CENTER, "%s", Text.c_str());
            i++;
        }
    }
    if (g_Options.esp_poland_c4.get_bool())
    {
        for (auto Text : bomb)
        {
            DrawString2(weapons_font, (int)top.x, int(top.y + height + 8 + (10 * bottom++)), Color::Red, FONT_CENTER, Text.c_str());
            i++;
        }
    }
}

void visuals::player::player_box(float x, float y, float w, float h, Color clr)
{
    g_VGuiSurface->DrawSetColor(clr);
    g_VGuiSurface->DrawOutlinedRect(int(x - w), int(y), int(x + w), int(y + h));
    g_VGuiSurface->DrawSetColor(Color::Black);
    g_VGuiSurface->DrawOutlinedRect(int(x - w - 1), int(y - 1), int(x + w + 1), int(y + h + 1));
    g_VGuiSurface->DrawOutlinedRect(int(x - w + 1), int(y + 1), int(x + w - 1), int(y + h - 1));
}

void visuals::misc::box_and_text(C_BasePlayer* entity, std::string text)
{
    ESPBox Box;
    std::vector<std::string> Info;
    if (get_box(entity, Box))
    {
        Info.push_back(text);
        if (g_Options.esp_poland_grenade.get_bool())
        {
            g_VGuiSurface->DrawSetColor(Color(255, 255, 255, 255));
            g_VGuiSurface->DrawOutlinedRect(Box.x, Box.y, Box.w, Box.h);

            g_VGuiSurface->DrawSetColor(Color(10, 10, 10, 150));
            g_VGuiSurface->DrawOutlinedRect(Box.x - 1, Box.y - 1, Box.w + 2, Box.h + 2);
            g_VGuiSurface->DrawOutlinedRect(Box.x + 1, Box.y + 1, Box.w - 2, Box.h - 2);
            int i = 0;
            for (auto grenade : Info)
            {
                Text(Box.x + 1, Box.y + 1, Color(255, 255, 255, 255), esp_font, grenade.c_str());
                i++;
            }
        }
    }
}
void visuals::player::draw_health(Vector bot, Vector top, int health)
{
    if (health > 100)
        health = 100;
    float h = (bot.y - top.y);
    float offset = (h / 4.f) + 5;


    UINT hp = UINT(h - (UINT)((h * health) / 100)); // Percentage

    int Red = int(255 - (health*2.55));
    int Green = int(health*2.55);

    g_VGuiSurface->DrawSetColor(Color(Red, Green, 0, 255));
    g_VGuiSurface->DrawLine(int((top.x - offset)), int(top.y + hp), int((top.x - offset)), int(top.y + h));
}
void visuals::misc::draw_bomb(C_BasePlayer* entity, ClientClass* cClass)
{
    // Null it out incase bomb has been dropped or planted
    BombCarrier = nullptr;
    C_BaseCombatWeapon *BombWeapon = (C_BaseCombatWeapon *)entity;
    Vector vOrig; Vector vScreen;
    vOrig = entity->m_vecOrigin();
    bool adopted = true;
    CBaseHandle parent = BombWeapon->GetOwnerHandle();
    if (parent.Get() || (vOrig.x == 0 && vOrig.y == 0 && vOrig.z == 0))
    {
        C_BasePlayer* pParentEnt = (C_BasePlayer*)(g_EntityList->GetClientEntityFromHandle(parent));
        if (pParentEnt && pParentEnt->is_alive())
        {
            BombCarrier = pParentEnt;
            adopted = false;
        }
    }
    if (g_Options.esp_poland_c4world.get_bool())
    {
        if (adopted)
        {
            if (math::world_to_screen(vOrig, vScreen))
            {
                Text((int)vScreen.x, (int)vScreen.y, Color(112, 20, 20, 255), esp_font, "Bomb");
            }
        }
    }
}
void visuals::misc::render_grenade(C_BasePlayer* entity)
{
    model_t* nadeModel = (model_t*)entity->GetModel();

    if (!nadeModel)
        return;

    studiohdr_t* hdr = g_MdlInfo->GetStudiomodel(nadeModel);

    if (!hdr)
        return;

    if (!strstr(hdr->szName, "thrown") && !strstr(hdr->szName, "dropped"))
        return;

    std::string nadeName = "Unknown Grenade";

    IMaterial* mats[32];
    g_MdlInfo->GetModelMaterials(nadeModel, hdr->numtextures, mats);

    for (int i = 0; i < hdr->numtextures; i++)
    {
        IMaterial* mat = mats[i];
        if (!mat)
            continue;

        if (strstr(mat->GetName(), "flashbang"))
        {
            nadeName = "Flashbang";
            break;
        }
        else if (strstr(mat->GetName(), "m67_grenade") || strstr(mat->GetName(), "hegrenade"))
        {
            nadeName = "HE";
            break;
        }
        else if (strstr(mat->GetName(), "smoke"))
        {
            nadeName = "Smoke";
            break;
        }
        else if (strstr(mat->GetName(), "decoy"))
        {
            nadeName = "Decoy";
            break;
        }
        else if (strstr(mat->GetName(), "incendiary") || strstr(mat->GetName(), "molotov"))
        {
            nadeName = "Molotov";
            break;
        }
    }

    box_and_text(entity, nadeName);
}

void visuals::misc::draw_bomb_planted(C_BasePlayer* entity, ClientClass* cClass)
{
    // Null it out incase bomb has been dropped or planted
    BombCarrier = nullptr;

    Vector vOrig; Vector vScreen;
    vOrig = entity->m_vecOrigin();
    C_PlantedC4* Bomb = (C_PlantedC4*)entity;
    auto local = C_BasePlayer::get_local_player();
    float flBlow = Bomb->m_flC4Blow();
    float lifetime = flBlow - (g_GlobalVars->interval_per_tick * local->m_nTickBase());
    if (math::world_to_screen(vOrig, vScreen))
    {
        if (local->is_alive())
        {
            float flDistance = local->get_eye_pos().DistTo(entity->get_eye_pos());
            float a = 450.7f;
            float b = 75.68f;
            float c = 789.2f;
            float d = ((flDistance - b) / c);
            float flDamage = a*exp(-d * d);

            damage = float((std::max)((int)ceilf(CSGO_Armor(flDamage, local->m_ArmorValue())), 0));

            sprintf_s(bombdamagestringdead, sizeof(bombdamagestringdead) - 1, "ur dead nigga");
            sprintf_s(bombdamagestringalive, sizeof(bombdamagestringalive) - 1, "Health left: %.0f", local->m_iHealth() - damage);
            if (damage >= local->m_iHealth())
            {
                Text((int)vScreen.x, int(vScreen.y + 10), Color(250, 42, 42, 255), defuse_font, bombdamagestringdead);
            }
            else if (local->m_iHealth() > damage)
            {
                Text((int)vScreen.x, int(vScreen.y + 10), Color(0, 255, 0, 255), defuse_font, bombdamagestringalive);
            }
        }
        char buffer[64];
        if (lifetime > 0.01f && !Bomb->m_bBombDefused())
        {
            sprintf_s(buffer, "Bomb: %.1f", lifetime);
            Text((int)vScreen.x, (int)vScreen.y, Color(250, 42, 42, 255), esp_font, buffer);
        }

    }
    static auto width = 0;
    static auto height = 0;
    g_EngineClient->GetScreenSize(width, height);
    static auto halfX = width / 2;
    static auto halfY = height / 2;


    if (Bomb->m_hBombDefuser() > 0)
    {
        float countdown = Bomb->m_flDefuseCountDown() - (local->m_nTickBase() * g_GlobalVars->interval_per_tick);
        if (countdown > 0.01f)
        {
            if (lifetime > countdown)
            {
                char defuseTimeString[24];
                sprintf_s(defuseTimeString, sizeof(defuseTimeString) - 1, "Defusing: %.1f", countdown);
                Text(halfX - 50, halfY + 200, Color(0, 255, 0, 255), defuse_font, defuseTimeString);
            }
            else
            {
                Text(halfX - 50, halfY + 200, Color(255, 0, 0, 255), defuse_font, "Nu time! Git gud!");
            }
        }
    }
}

void visuals::misc::draw_drop(C_BasePlayer* entity, ClientClass* cClass)
{
    Vector Box;
    C_BaseCombatWeapon* Weapon = (C_BaseCombatWeapon*)entity;
    C_BasePlayer* plr = (C_BasePlayer*)g_EntityList->GetClientEntityFromHandle(Weapon->GetOwnerHandle());
    if (!plr && math::world_to_screen(Weapon->m_vecOrigin(), Box))
    {
        std::string ItemName = CleanItemName(cClass->m_pNetworkName);
        RECT TextSize = GetTextSize(dropped_weapons_font, ItemName.c_str());
        Text(int(Box.x - (TextSize.right / 2)), int(Box.y - 16), Color(255, 255, 255, 255), dropped_weapons_font, ItemName.c_str());
    }
}



bool visuals::misc::get_box(C_BasePlayer* entity, ESPBox &result)
{
    // Variables
    Vector  vOrigin, min, max, sMin, sMax, sOrigin,
        flb, brt, blb, frt, frb, brb, blt, flt;
    float left, top, right, bottom;

    // Get the locations
    vOrigin = entity->m_vecOrigin();
    min = entity->collisionProperty()->GetMins() + vOrigin;
    max = entity->collisionProperty()->GetMaxs() + vOrigin;

    // Points of a 3d bounding box
    Vector points[] = { Vector(min.x, min.y, min.z),
        Vector(min.x, max.y, min.z),
        Vector(max.x, max.y, min.z),
        Vector(max.x, min.y, min.z),
        Vector(max.x, max.y, max.z),
        Vector(min.x, max.y, max.z),
        Vector(min.x, min.y, max.z),
        Vector(max.x, min.y, max.z) };

    // Get screen positions
    if (!math::world_to_screen(points[3], flb) || !math::world_to_screen(points[5], brt)
        || !math::world_to_screen(points[0], blb) || !math::world_to_screen(points[4], frt)
        || !math::world_to_screen(points[2], frb) || !math::world_to_screen(points[1], brb)
        || !math::world_to_screen(points[6], blt) || !math::world_to_screen(points[7], flt))
        return false;

    // Put them in an array (maybe start them off in one later for speed?)
    Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

    // Init this shit
    left = flb.x;
    top = flb.y;
    right = flb.x;
    bottom = flb.y;

    // Find the bounding corners for our box
    for (int i = 1; i < 8; i++)
    {
        if (left > arr[i].x)
            left = arr[i].x;
        if (bottom < arr[i].y)
            bottom = arr[i].y;
        if (right < arr[i].x)
            right = arr[i].x;
        if (top > arr[i].y)
            top = arr[i].y;
    }

    // Width / height
    result.x = (int)left;
    result.right = (int)right;
    result.y = (int)top;
    result.w = int(right - left);
    result.h = int(bottom - top);
    result.bottom = (int)bottom;

    return true;
}

void visuals::player::dlight(C_BasePlayer* entity)
{
    dlight_t* elight = g_DLight->CL_AllocElight(entity->EntIndex());
    elight->origin = entity->m_vecOrigin();
    elight->radius = 300;
    elight->color.b = 255;
    elight->color.g = 144;
    elight->color.r = 30;
    elight->color.exponent = 1000;
    elight->die = g_GlobalVars->curtime + 0.05f;
    elight->decay = elight->radius;
    elight->key = entity->EntIndex();

    dlight_t* dlight = g_DLight->CL_AllocDlight(entity->EntIndex());
    dlight->origin = entity->m_vecOrigin();
    dlight->radius = 300;
    dlight->color.b = 255;
    dlight->color.g = 144;
    dlight->color.r = 30;
    dlight->color.exponent = 1000;
    dlight->die = g_GlobalVars->curtime + 0.05f;
    dlight->decay = dlight->radius;
    dlight->key = entity->EntIndex();
}

void visuals::misc::night_mode()
{
    static float r;
    static float g;
    static float b;

    if (g_Options.misc_nightmode.get_bool())
    {
        if (!done)
        {
            char m_szLevelNameShort[65];
            sprintf_s(m_szLevelNameShort, sizeof(m_szLevelNameShort) - 1, "materials_%s.txt", g_ClientState->m_szLevelNameShort);
            auto s = std::ofstream(m_szLevelNameShort);
            std::ostream& stream = s;
            
            static auto sv_skyname = g_CVar->FindVar("sv_skyname");
            static auto r_DrawSpecificStaticProp = g_CVar->FindVar("r_DrawSpecificStaticProp");
            r_DrawSpecificStaticProp->SetValue(1);
            sv_skyname->SetValue("sky_csgo_night02");

            for (MaterialHandle_t i = g_MatSystem->FirstMaterial(); i != g_MatSystem->InvalidMaterial(); i = g_MatSystem->NextMaterial(i))
            {
                IMaterial *pMaterial = g_MatSystem->GetMaterial(i);

                if (!pMaterial)
                    continue;

                const char* group = pMaterial->GetTextureGroupName();
                const char* name  = pMaterial->GetName();

                stream << name << '\n';



                if (strstr(group, TEXTURE_GROUP_WORLD))
                {
                    pMaterial->GetColorModulation(&r, &g, &b);
                    pMaterial->ColorModulate(r * 0.10, g * 0.10, b * 0.10);
                }
                if (strstr(group, "StaticProp"))
                {
                    pMaterial->GetColorModulation(&r, &g, &b);
                    pMaterial->ColorModulate(r * 0.30, g * 0.30, b * 0.30);
                }
                if (strstr(name, "models/props/de_dust/palace_bigdome"))
                {
                    pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
                }
                if (strstr(name, "models/props/de_dust/palace_pillars"))
                {
                    pMaterial->GetColorModulation(&r, &g, &b);
                    pMaterial->ColorModulate(r * 0.30, g * 0.30, b * 0.30);
                }

                if (strstr(group, TEXTURE_GROUP_PARTICLE))
                {
                    pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
                }
                done = true;
            }

        }
    }
    else
    {
        if (done)
        {
            for (MaterialHandle_t i = g_MatSystem->FirstMaterial(); i != g_MatSystem->InvalidMaterial(); i = g_MatSystem->NextMaterial(i))
            {
                IMaterial *pMaterial = g_MatSystem->GetMaterial(i);

                if (!pMaterial)
                    continue;

                const char* group = pMaterial->GetTextureGroupName();
                const char* name = pMaterial->GetName();

                if (strstr(group, TEXTURE_GROUP_WORLD))
                {
                    pMaterial->GetColorModulation(&r, &g, &b);
                    pMaterial->ColorModulate(r * 10, g * 10, b * 10);
                }
                if (strstr(group, "StaticProp"))
                {
                    pMaterial->GetColorModulation(&r, &g, &b);
                    pMaterial->ColorModulate(r * 3.33335, g * 3.33335, b * 3.33335);
                }
                if (strstr(name, "models/props/de_dust/palace_bigdome"))
                {
                    pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
                }
                if (strstr(name, "models/props/de_dust/palace_pillars"))
                {
                    pMaterial->GetColorModulation(&r, &g, &b);
                    pMaterial->ColorModulate(r * 3.33335, g * 3.33335, b * 3.33335);
                }
                if (strstr(group, TEXTURE_GROUP_PARTICLE))
                {
                    pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
                }
            }
            done = false;
        }

    }
}

void visuals::misc::wireframe_hands(const ModelRenderInfo_t& pInfo)
{
    if (g_EngineClient->IsInGame())
    {
        if (g_Options.misc_wireframe_hands.get_bool())
        {
            const char * ModelName = g_MdlInfo->GetModelName((model_t*)pInfo.pModel);
            if (strstr(ModelName, "arms"))
            {
                IMaterial* Hands = g_MatSystem->FindMaterial(ModelName, "Model textures");
                if (!g_Input->m_fCameraInThirdPerson)
                {
                    Hands->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
                    g_MdlRender->ForcedMaterialOverride(Hands);
                }
                else
                {
                    Hands->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
                    g_MdlRender->ForcedMaterialOverride(Hands);
                }
            }
        }
    }
}

