#pragma once

#include "sdk.h"

#ifndef CONCAT_IMPL
#define CONCAT_IMPL(x, y) x##y
#ifndef MACRO_CONCAT
#define MACRO_CONCAT(x, y) CONCAT_IMPL(x, y)
#ifndef PAD
#define PAD(SIZE) BYTE MACRO_CONCAT(_pad, __COUNTER__)[SIZE];
#endif
#endif
#endif

struct WeaponInfo
{
	int _id = 0;
	std::string _name = "";
};

class CHudTexture 
{
public:
	PAD( 0x82 );
	char cCharacterInFont;
};

/*struct weapon_info_t
{
	char pad_0x0000[0x4]; //0x0000
	char* weapon_name; //0x0004 
	char pad_0x0008[0xC]; //0x0008
	__int32 max_clip_ammo; //0x0014 
	char pad_0x0018[0xC]; //0x0018
	__int32 max_ammo; //0x0024 
	char pad_0x0028[0x4]; //0x0028
	char* world_model; //0x002C 
	char* view_model; //0x0030 
	char* dropped_model; //0x0034 
	char pad_0x0038[0x4]; //0x0038
	char* shot_type_name; //0x003C 
	char pad_0x0040[0x28]; //0x0040
	char* silenced_shot_type_name; //0x0068 
	char pad_0x006C[0xC]; //0x006C
	char* nearly_empty_shot_type_name; //0x0078 
	char pad_0x007C[0x4]; //0x007C
	char* ammo_type; //0x0080 
	char pad_0x0084[0x4]; //0x0084
	char* hud_name; //0x0088 
	char* weapon_name2; //0x008C 
	char pad_0x0090[0x34]; //0x0090
	some_weird_struct* some_weird_pointer; //0x00C4 
	__int32 weapon_type; //0x00C8 
	char pad_0x00CC[0x4]; //0x00CC
	__int32 price; //0x00D0 
	__int32 kill_reward; //0x00D4 
	char* weapon_name_readable; //0x00D8 
	float N00000038; //0x00DC 
	float N00000039; //0x00E0 
	float N0000003A; //0x00E4 
	float N0000003B; //0x00E8 
	__int8 full_auto; //0x00EC 
	char pad_0x00ED[0x3]; //0x00ED
	__int32 damage; //0x00F0 
	float armor_ratio; //0x00F4 
	char pad_0x00F8[0x4]; //0x00F8
	float penetration; //0x00FC 
	float N00000041; //0x0100 
	float N00000042; //0x0104 
	float range; //0x0108 
	float range_modifier; //0x010C 
	char pad_0x0110[0x10]; //0x0110
	__int8 has_silencer; //0x0120 
	char pad_0x0121[0x3]; //0x0121
	void** some_weird_string_list; //0x0124 
	char pad_0x0128[0x8]; //0x0128
	float max_speed; //0x0130 
	float max_speed_alt; //0x0134 
	float move_speed_modifier; //0x0138 
	float spread; //0x013C 
	float spread_alt; //0x0140 
	float inaccuracy_crouch; //0x0144 
	float inaccuracy_crouch_alt; //0x0148 
	float inaccuracy_stand; //0x014C 
	float inaccuracy_stand_alt; //0x0150 
	float inaccuracy_jump_start; //0x0154 
	float inaccuracy_jump; //0x0158 
	float inaccuracy_jump_alt; //0x015C 
	float inaccuracy_land; //0x0160 
	float inaccuracy_land_alt; //0x0164 
	float inaccuracy_ladder; //0x0168 
	float inaccuracy_ladder_alt; //0x016C 
	float inaccuracy_fire; //0x0170 
	float inaccuracy_fire_alt; //0x0174 
	float inaccuracy_move; //0x0178 
	float inaccuracy_move_alt; //0x017C 
	char pad_0x0180[0x48]; //0x0180
	__int32 zoom_levels; //0x01C8 
	__int32 zoom_fov1; //0x01CC 
	__int32 zoom_fov2; //0x01D0 
	char pad_0x01D4[0xC]; //0x01D4
	char* weapon_type_name; //0x01E0 
	char pad_0x01E4[0x8]; //0x01E4
	char* ammo_shell_name; //0x01EC 
	char* tracer_name; //0x01F0 
	char pad_0x01F4[0x8]; //0x01F4
	char* N00000080; //0x01FC 
	char pad_0x0200[0x4]; //0x0200
	char* muzzleflash_name; //0x0204 
	char pad_0x0208[0x4]; //0x0208
	char* muzzlesmoke_name; //0x020C 
	char pad_0x0210[0x4]; //0x0210
	char* zoom_name; //0x0214 
	char* zoom_name_2; //0x0218 
	char pad_0x021C[0x24]; //0x021C

}; //Size=0x0240*/

class CWeaponData
{
public:
	char pad_0x0000[ 0x4 ]; //0x0000
	char* weapon_name; //0x0004 
	char pad_0x0008[ 0xC ]; //0x0008
	__int32 max_clip_ammo; //0x0014 
	char pad_0x0018[ 0xC ]; //0x0018
	__int32 max_reserved_ammo; //0x0024 
	char pad_0x0028[ 0x4 ]; //0x0028
	char* world_model; //0x002C 
	char* view_model; //0x0030 
	char* dropped_model; //0x0034 
	char pad_0x0038[ 0x4 ]; //0x0038
	char* sound_single_shot; //0x003C 
	char pad_0x0040[ 0x38 ]; //0x0040
	char* sound_nearlyempty; //0x0078 
	char pad_0x007C[ 0x4 ]; //0x007C
	char* ammo_type; //0x0080 
	char pad_0x0084[ 0x4 ]; //0x0084
	char* hud_name; //0x0088 
	char* weapon_name2; //0x008C 
	char pad_0x0090[ 0x38 ]; //0x0090
	__int32 weapon_type; //0x00C8 
	char pad_0x00CC[ 0x4 ]; //0x00CC
	__int32 weapon_price; //0x00D0 
	char pad_0x00D4[ 0x8 ]; //0x00D4
	float cycletime; //0x00DC 
	char pad_0x00E0[ 0x4 ]; //0x00E0
	float time_to_idle; //0x00E4 
	float idle_interval; //0x00E8 
	char pad_0x00EC[ 0x4 ]; //0x00EC
	__int32 damage; //0x00F0 
	float armor_ratio; //0x00F4 
	char pad_0x00F8[ 0x4 ]; //0x00F8
	float penetration; //0x00FC 
	char pad_0x0100[ 0x8 ]; //0x0100
	float range; //0x0108 
	float range_modifier; //0x010C 
	char pad_0x0110[ 0x18 ]; //0x0110
	__int32 crosshair_min_distance; //0x0128 
	__int32 crosshair_delta_distance; //0x012C 
	float movement_speed; //0x0130 
	float movement_speed_zoom; //0x0134 
	char pad_0x0138[ 0x4C ]; //0x0138
	__int32 recoil_seed; //0x0184 
	char pad_0x0188[ 0x40 ]; //0x0188
	__int32 zoom_levels; //0x01C8 
	__int32 zoom_fov_1; //0x01CC 
	__int32 zoom_fov_2; //0x01D0 
	float zoom_time_0; //0x01D4 
	float zoom_time_1; //0x01D8 
	float zoom_time_2; //0x01DC 
	char* addon_location; //0x01E0 
	char pad_0x01E4[ 0x8 ]; //0x01E4
	char* eject_brass_effect; //0x01EC 
	char* tracer_effect; //0x01F0 
	char pad_0x01F4[ 0x8 ]; //0x01F4
	char* muzzle_flash_effect_1st_person; //0x01FC 
	char pad_0x0200[ 0x4 ]; //0x0200
	char* muzzle_flash_effect_3rd_person; //0x0204 
	char pad_0x0208[ 0x4 ]; //0x0208
	char* heat_effect; //0x020C 
	char pad_0x0210[ 0x4 ]; //0x0210
	char* zoom_in_sound; //0x0214 
	char* zoom_out_sound; //0x0218 
	char pad_0x021C[ 0xCC ]; //0x021C

	char *bulletType()
	{
		return ( char* ) ( ( DWORD )this + 0x0080 );
	}
};