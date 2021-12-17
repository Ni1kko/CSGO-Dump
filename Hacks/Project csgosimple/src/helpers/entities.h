#pragma once

#include "../valve_sdk/csgostructs.hpp"

#include <list>
#include <mutex>

namespace entities
{
#pragma pack(push, 1)
	struct local_data_t
	{
		Vector local_pos;
		Vector world_pos;
		QAngle angles;
		bool is_scoped;
		bool is_defusing;
		bool is_flashed;
		bool is_alive;
		int tick_base;
		int health;
		int armor_value;

		bool has_sniper;

		int hp = 0;
		int damage;
		float bomb_time = 0.f;
		float defuse_time = 0.f;
		int bomb_indexStatus;

		bool isBombPlantedStatus;

		bool AfterPlant;

		bool is_matchmaking = false;

		int bombIndex;
		bool IsBombPlanted;

		c_base_player* local;
	};
#pragma pack(pop)

#pragma pack(push, 1)
	struct player_data_t
	{
		int index = 0;

		std::string name;
		std::string weapon;


		bool is_shooting = false;
		Vector shot_origin;

		bool is_enemy;
		bool is_scoped;
		bool is_flashed;
		bool is_defusing;
		bool is_dormant;
		bool is_reloading;
		bool has_helmet;
		bool has_kevlar;
		bool is_c4_carrier;
		bool has_defkit;

		c_base_combat_weapon* weapData;

		bool in_smoke;
		bool is_visible;

		float m_iHealth;
		float m_iMoney;
		float m_iAmmo;
		float m_MaxAmmo; //was int
		float m_ArmorValue;
		float m_flShotTime;
		float m_flSimulationTime;

		Vector points[8];

		RECT box;
		Vector eye_pos;
		Vector offset;
		Vector origin;
		Vector world_pos;
		QAngle angles;

		std::array<Vector, 4> hitboxes[HITBOX_MAX];
	};
#pragma pack(pop)

	struct tick_data_t
	{
		int tick_count;

		player_data_t players[MAX_PLAYERS];
	};

	extern std::mutex locker;
	extern std::mutex local_mutex;

	extern local_data_t m_local;
	extern std::list<tick_data_t> m_items;

	void fetch_targets(CUserCmd* cmd);
}