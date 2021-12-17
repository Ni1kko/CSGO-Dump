#pragma once

#include "../render.hpp"
#include "../helpers/math.hpp"
#include "../valve_sdk/csgostructs.hpp"

struct Sounds
{
	DWORD time;
	Vector origin;
	bool enemy;
	bool local;
};

class SoundEsp
{
public:
	std::vector<Sounds> sound;

	void Update()
	{
		for (size_t i = 0; i < sound.size(); i++) {
			if (sound[i].time + 800 <= GetTickCount()) {
				sound.erase(sound.begin() + i);
			}
		}
	}

	void AddSound(Vector origin, bool is_enemy, bool is_local)
	{
		for (auto& soundinfo : sound) {
			if (soundinfo.origin == origin)
				return;
		}

		Sounds entry;
		entry.time = GetTickCount();
		entry.origin = origin;
		entry.enemy = is_enemy;
		entry.local = is_local;
		sound.push_back(entry);
	}
};

inline SoundEsp* g_SoundEsp;

class Visuals
{
	CRITICAL_SECTION cs;

public:

	Visuals();
	~Visuals();

	class Player
	{
	public:
		struct
		{
			C_BasePlayer* pl;
			bool          is_enemy;
			bool          is_local;
			bool          is_visible;
			Color         clr;
			Vector        head_pos;
			Vector        feet_pos;
			RECT          bbox;
		} ctx;

		bool Begin(C_BasePlayer * pl);
		void RenderBox();
		void RenderName();
		void RenderWeaponName();
		void RenderHealth();
		void RenderArmour();
	};
	void RenderWeapon(C_BaseCombatWeapon* ent);
	void RenderDefuseKit(C_BaseEntity* ent);
	void RenderPlantedC4(C_BaseEntity* ent);
	void RenderItemEsp(C_BaseEntity* ent);
public:
	void AddToDrawList();
	void RenderSoundEsp();
	void Render();
};

inline Visuals* g_Visuals;