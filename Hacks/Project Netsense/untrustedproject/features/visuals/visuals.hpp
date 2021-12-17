#pragma once
#include "../../singleton.hpp"
#include "../../valve_sdk/sdk.hpp"
#include "../../valve_sdk/csgostructs.hpp"
#include "../../helpers/math.hpp"
#include "../../config.hpp"
#include "glow.hpp"
#include <d3dx9.h>
#include <d3d9.h>
#include <map>

struct PlayersPreviousInfo_t {
	int hp = -1;
	int hpDifference = 0;
	float hpDiffTime = 0.f;

	int armor = -1;
	int armorDifference = 0;
	float armorDiffTime = 0.f;
};

class c_visuals {
private:
	struct sideinfo_t {
		std::string str;
		Color color;
		float size;
	}; 
	struct SoundInfo_t {
		int guid;
		float soundTime;
		float alpha;
		Vector soundPos;
	};

private:
	struct boxes_t {
		bool enabled = false;
		bool outline = false;
		bool filled = false;

		int type = 0;
		
		Color outline_clr = Color(0.f, 0.f, 0.f, 1.f);

		Color allies_vis_clr = Color(255, 255, 255);
		Color enemy_vis_clr = Color(255, 255, 255);
		Color allies_invis_clr = Color(255, 255, 255);
		Color enemy_invis_clr = Color(255, 255, 255);
		
		Color fill_color = Color(255, 255, 255);
		
	};

	struct skeleton_t {
		bool enabled = false;
		Color clr = Color(255, 255, 255);
	};

	struct esp_other_t {
		bool hp = false;
		bool name = false;
		bool weapon = false;
		bool ammo_bar = false;
		bool armor = false;
		/*bool snipeline = false;
		bool head_dot = false;*/

		bool team = false;
		bool visible_only = false;
		bool dormant = false;

		bool  flags = false;
		bool  flags_scoped = false;
		float flags_scoped_size = 8.f;
		bool  flags_reload = false;
		float flags_reload_size = 8.f;
		bool  flags_bomb = false;
		float flags_bomb_size = 8.f;
		bool  flags_flashed = false;
		float flags_flashed_size = 8.f;
		bool  flags_hk = false;
		float flags_hk_size = 8.f;
		bool  flags_defusing = false;
		float flags_defusing_size = 8.f;

		bool  sound_esp = false;
		int   sound_esp_type = 0;
		bool  sound_esp_onteam = false;
		float sound_esp_time = 0.5f;
		float sound_esp_radius = 15.f;

		float name_size = 8.f;
		float weapon_size = 8.f;

		Color name_clr = Color(255, 255, 255);
		Color weapon_clr = Color(255, 255, 255);
		/*Color snipeline_clr = Color(255, 255, 255);
		Color head_dot_clr = Color(255, 255, 255);*/
		Color sound_esp_clr = Color(255, 255, 255);
		Color hp_clr = Color(0, 255, 0);
		Color armor_clr = Color(0, 50, 255);
		Color ammobar_clr = Color(255, 255, 255);
	};

	struct glow_t {
		bool enabled = false;

		bool players = false;
		bool on_team = false;
		bool on_chickens = false;
		bool c4_carrier = false;
		bool c4_planted = false;
		bool defuse_kits = false;
		bool weapons = false;

		int type = 0;
		bool bloom = false;

		Color enemy_clr = Color(255, 255, 255);
		Color enemy_invis_clr = Color(255, 255, 255);

		Color allies_clr = Color(255, 255, 255);
		Color allies_invis_clr = Color(255, 255, 255);

		Color chickens_clr = Color(255, 255, 255);
		Color c4_carrier_clr = Color(255, 255, 255);
		Color c4_planted_clr = Color(255, 255, 255);
		Color defuse_kits_clr = Color(255, 255, 255);
		Color weapons_clr = Color(255, 255, 255);
	};

private:
	class visuals_player {
	private:
		int alpha = 255;
		bool is_visible = false;
		bool in_team = false;
		int32_t health = 75;
		int armor = 75;
		std::string playerName = "";
		std::string weaponName = "";
		C_BasePlayer* player = nullptr;
		RECT bbox{ 0,0,0,0 };
		Vector head_pos, feet_pos;
		PlayersPreviousInfo_t* previousInfo;
	private:
		void DrawBoxes();
		void DrawSkeleton();
		void DrawHP();
		void DrawName();
		void DrawArmor();
		void DrawWeapon();
		/*void DrawSnipeLine();
		void DrawHeadDot();*/
		void DrawFlags();
		void DrawAmmoBar();
	private:
		void ImplementAlpha(Color& clr) {
			if ((int)clr.a() > alpha)
				clr.SetColor(clr.r(), clr.g(), clr.b(), alpha);
		}
	public:
		visuals_player(C_BasePlayer* player, int alpha);
		bool IsValid();
		bool Draw();
		void Initialize();
	private:
		RECT GetBBox(C_BaseEntity* ent) {
			RECT rect = RECT();
			ICollideable* collideable = ent->GetCollideable();
			if (!collideable) return rect;

			Vector min = collideable->OBBMins();
			Vector max = collideable->OBBMaxs();

			Vector points[] = {
				Vector(min.x, min.y, min.z),
				Vector(min.x, max.y, min.z),
				Vector(max.x, max.y, min.z),
				Vector(max.x, min.y, min.z),
				Vector(max.x, max.y, max.z),
				Vector(min.x, max.y, max.z),
				Vector(min.x, min.y, max.z),
				Vector(max.x, min.y, max.z) };

			auto trans = ent->m_rgflCoordinateFrame();

			Vector pointsTransformed[8];
			Vector screen_points[8];

			bool init = false;

			for (int i = 0; i < 8; i++)
			{
				Math::VectorTransform(points[i], trans, pointsTransformed[i]);

				if (!Math::WorldToScreen(pointsTransformed[i], screen_points[i]))
					return rect;

				if (!init) {
					rect.left = screen_points[0].x;
					rect.top = screen_points[0].y;
					rect.right = screen_points[0].x;
					rect.bottom = screen_points[0].y;
					init = true;
				}
				else {
					if (rect.left > screen_points[i].x)
						rect.left = screen_points[i].x;
					if (rect.bottom < screen_points[i].y)
						rect.bottom = screen_points[i].y;
					if (rect.right < screen_points[i].x)
						rect.right = screen_points[i].x;
					if (rect.top > screen_points[i].y)
						rect.top = screen_points[i].y;
				}
			}

			return rect;
		}
	};
private:
	int EspAlphas[65];
	bool LastDormant[65];
	//IDirect3DTexture9* PreviewModel = nullptr;
	std::map< int, std::vector< SoundInfo_t > > m_Sounds;
	CUtlVector<SndInfo_t> sounds;
private:
	void UpdateSounds();
	void DrawSounds();
private:
	boxes_t boxes;
	skeleton_t skeleton;
	esp_other_t other;
public:
	glow_t glow;
public:
	void Run();
public:
	void Menu();
	void SetupValues();
public:
	PlayersPreviousInfo_t PreviousInfos[4096];
};

extern c_visuals* g_Visuals;