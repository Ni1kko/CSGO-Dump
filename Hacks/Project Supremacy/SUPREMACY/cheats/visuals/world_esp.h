#pragma once

#include "..\..\includes.hpp"
#include "..\..\sdk\structs.hpp"

class weapon_t;
class Box;

class worldesp : public singleton< worldesp > {
public:
	void paint_traverse();

	void PlantedBomb(entity_t * ent, Box box);

	void grenades(weapon_t * m_nade, Box box);
	void BombTimer(IClientEntity * pEntity, ClientClass * cClass);
	void weapons(weapon_t * m_weapon, Box box);
};