#pragma once

class CAnimationLayer
{
public:
	char  pad_0000[ 20 ];
	unsigned int m_nOrder; //0x0014
	unsigned int m_nSequence; //0x0018
	float m_flPrevCycle; //0x001C
	float m_flWeight; //0x0020
	float m_flWeightDeltaRate; //0x0024
	float m_flPlaybackRate; //0x0028
	float m_flCycle; //0x002C
	void *m_pOwner; //0x0030 // player's thisptr
	char  pad_0038[ 4 ]; //0x0034
};

/*struct CCSGOPlayerAnimState
{
	char pad_0000[8]; //0x0000
	int32_t tickcount; //0x0008
	char pad_000C[12]; //0x000C
	float cutime; //0x0018
	char pad_001C[12]; //0x001C
	float movingtimer; //0x0028 0 while standing ;those 2 are unreliable
	float standingtimer; //0x002C 0 while moving
	char pad_0030[8]; //0x0030
	float accel_fraction; //0x0038
	float movingtimer2; //0x003C 0 while standing ;those 2 are more reliable
	float standingtimer2; //0x0040 0 while moving
	char pad_0044[8]; //0x0044
	float heavily_delayed_movementfraction; //0x004C slowly goes from 0 to 1 when you start moving
	char pad_0050[12]; //0x0050
	void* weapon_1; //0x005C
	void* palyer; //0x0060
	void* weapon_2; //0x0064
	void* weapon_3; //0x0068
	float m_update_time; //0x006C
	int32_t m_update_framecount; //0x0070
	float m_update_delta; //0x0074
	float m_yaw; //0x0078
	float m_pitch; //0x007C
	float m_abs_yaw; //0x0080
	float m_old_absyaw; //0x0084
	float unk_fl; //0x0088
	float unk_fl2; //0x008C
	float groundstrafe_shit; //0x0090
	char pad_0094[4]; //0x0094
	float walking_cycle; //0x0098
	float speed_maxspeed_fraction; //0x009C
	char pad_00A0[4]; //0x00A0
	float duckamount; //0x00A4
	float fallinganim_amount; //0x00A8
	char pad_00AC[4]; //0x00AC
	vec3 m_origin; //0x00B0
	vec3 m_last_origin; //0x00BC
	vec2 m_velocity; //0x00C8
	char pad_00D0[8]; //0x00D0
	vec2 normalized_velocity; //0x00D8
	char pad_00E0[4]; //0x00E0
	vec2 last_normalized_velocity; //0x00E4
	float m_speed; //0x00EC
	float z_velocity; //0x00F0
	float movement_related1; //0x00F4
	float movement_related2; //0x00F8
	float movement_related3; //0x00FC
	float movingtimer3; //0x0100
	float standingtimer3; //0x0104
	int8_t onground; //0x0108
	int8_t in_land_anim; //0x0109
	char pad_010A[10]; //0x010A
	float m_flLastOriginZ; //0x0114
	float m_flHeadHeightOrOffsetFromHittingGroundAnimation; //0x0118
	float m_flStopToFullRunningFraction; //0x011C
	char pad_0120[56]; //0x0120
	float some_other_time; //0x0158
	vec3 full_velocity; //0x015C
	char pad_0168[80]; //0x0168
	char* sz_lean_yaw; //0x01B8
	char pad_01BC[8]; //0x01BC
	char* sz_speed; //0x01C4
	char pad_01C8[8]; //0x01C8
	char* sz_ladder_speed; //0x01D0
	char pad_01D4[8]; //0x01D4
	char* sz_ladder_yaw; //0x01DC
	char pad_01E0[8]; //0x01E0
	char* sz_move_yaw; //0x01E8
	char pad_01EC[20]; //0x01EC
	char* sz_body_yaw; //0x0200
	char pad_0204[8]; //0x0204
	char* sz_body_pitch; //0x020C
	char pad_0210[8]; //0x0210
	char* sz_death_yaw; //0x0218
	char pad_021C[8]; //0x021C
	char* sz_stand; //0x0224
	char pad_0228[8]; //0x0228
	char* sz_jump_fall; //0x0230
	char pad_0234[8]; //0x0234
	char* sz_aim_blend_stand_idle; //0x023C
	char pad_0240[8]; //0x0240
	char* sz_aim_blend_crouch_idle; //0x0248
	char pad_024C[8]; //0x024C
	char* sz_strafe_yaw; //0x0254
	char pad_0258[8]; //0x0258
	char* sz_blend_stand_walk; //0x0260
	char pad_0264[8]; //0x0264
	char* sz_blend_stand_run; //0x026C
	char pad_0270[8]; //0x0270
	char* sz_blend_crouch_walk; //0x0278
	char pad_027C[8]; //0x027C
	char* sz_move_blend_walk; //0x0284
	char pad_0288[8]; //0x0288
	char* sz_move_blend_run; //0x0290
	char pad_0294[8]; //0x0294
	char* sz_move_blend_crouch; //0x029C
	char pad_02A0[4]; //0x02A0
	float setupvelocity_speedlimit; //0x02A4
	int32_t walk_type; //0x02A8 1 while shiftwalking, 2 while running
	float velocitychange_magicvalue; //0x02AC
	char pad_02B0[60]; //0x02B0
	float feetyaw_related; //0x02EC following floats are all used for feet stuff
	float N000000C7; //0x02F0
	float N000000C8; //0x02F4
	float N000000C9; //0x02F8
	float N000000CA; //0x02FC
	float N000000CB; //0x0300 if you manipulate this value the feet steps up lol
	float N000000CC; //0x0304
	float N000000CD; //0x0308
	float N000000CE; //0x030C
	float N000000CF; //0x0310
	float N000000D0; //0x0314
	char pad_0318[4]; //0x0318
	float standing_anim_progress; //0x031C goes from 0 to 1 while stopping to move
	float weird_timing_value1; //0x0320
	char pad_0324[8]; //0x0324
	float weird_timing_value2; //0x032C
	float min_body_yaw_degrees; //0x330
	float max_body_yaw_degrees; //0x334
	float min_body_pitch_degrees; //0x0338
	float max_body_pitch_degrees; //0x338C
};*/

class CAnimState
{
public:
	char pad[ 3 ];
	char bUnknown; //0x4
	char pad2[ 91 ];
	void* pBaseEntity; //0x60
	void* pActiveWeapon; //0x64
	void* pLastActiveWeapon; //0x68
	float m_flLastClientSideAnimationUpdateTime; //0x6C
	int m_iLastClientSideAnimationUpdateFramecount; //0x70
	float m_flEyePitch; //0x74
	float m_flEyeYaw; //0x78
	float m_flPitch; //0x7C
	float m_flGoalFeetYaw; //0x80
	float m_flCurrentFeetYaw; //0x84
	float m_flCurrentTorsoYaw; //0x88
	float m_flUnknownVelocityLean; //0x8C //changes when moving/jumping/hitting ground
	float m_flLeanAmount; //0x90
	char pad4[ 4 ]; //NaN
	float m_flFeetCycle; //0x98 0 to 1
	float m_flFeetYawRate; //0x9C 0 to 1
	float m_fUnknown2;
	float m_fDuckAmount; //0xA4
	float m_fLandingDuckAdditiveSomething; //0xA8
	float m_fUnknown3; //0xAC
	float m_vOrigin_x; //0xB0, 0xB4, 0xB8
	float m_vOrigin_y;
	float m_vOrigin_z;
	float m_vLastOrigin_x; //0xBC, 0xC0, 0xC4
	float m_vLastOrigin_y;
	float m_vLastOrigin_z;
	float m_vVelocityX; //0xC8
	float m_vVelocityY; //0xCC
	char pad5[ 4 ];
	float m_flUnknownFloat1; //0xD4 Affected by movement and direction
	char pad6[ 8 ];
	float m_flUnknownFloat2; //0xE0 //from -1 to 1 when moving and affected by direction
	float m_flUnknownFloat3; //0xE4 //from -1 to 1 when moving and affected by direction
	float m_unknown; //0xE8
	float m_velocity; //0xEC
	float flUpVelocity; //0xF0
	float m_flSpeedNormalized; //0xF4 //from 0 to 1
	float m_flFeetSpeedStand; //0xF8 //from 0 to 2. something  is 1 when walking, 2.something when running, 0.653 when crouch walking
	float m_flFeetSpeedDucked; //0xFC //from 0 to 3. something
	float m_flTimeSinceStartedMoving; //0x100
	float m_flTimeSinceStoppedMoving; //0x104
	unsigned char m_bOnGround; //0x108
	unsigned char m_bInHitGroundAnimation; //0x109
	char pad7[ 10 ];
	float m_flLastOriginZ; //0x114
	float m_flHeadHeightOrOffsetFromHittingGroundAnimation; //0x118 from 0 to 1, is 1 when standing
	float m_flStopToFullRunningFraction; //0x11C from 0 to 1, doesnt change when walking or crouching, only running
	char pad8[ 4 ]; //NaN
	float m_flUnknownFraction; //0x124 affected while jumping and running, or when just jumping, 0 to 1
	char pad9[ 4 ]; //NaN
	float m_flUnknown3;
	char pad10[ 528 ];
};