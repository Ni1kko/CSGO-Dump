#pragma once
enum CSGO_ACTS
{
	ACT_CSGO_DEFUSE = 958,
	ACT_CSGO_DEFUSE_WITH_KIT = 959,
	ACT_CSGO_FLASHBANG_REACTION = 960,
	ACT_CSGO_FIRE_PRIMARY = 961,
	ACT_CSGO_FIRE_PRIMARY_OPT_1 = 962,
	ACT_CSGO_FIRE_PRIMARY_OPT_2 = 963,
	ACT_CSGO_FIRE_SECONDARY = 964,
	ACT_CSGO_RELOAD = 967,
	ACT_CSGO_RELOAD_START = 968,
	ACT_CSGO_RELOAD_LOOP = 969,
	ACT_CSGO_RELOAD_END = 970,
	ACT_CSGO_PLANT_BOMB = 978,
	ACT_CSGO_IDLE_TURN_BALANCEADJUST = 979,
	ACT_CSGO_IDLE_ADJUST_STOPPEDMOVING = 980,
};

class CAnimationLayer
{
	public:
		char  pad_0000[20];
		// These should also be present in the padding, don't see the use for it though
		//float	m_flLayerAnimtime;
		//float	m_flLayerFadeOuttime;
		unsigned int m_nOrder; //0x0014
		unsigned int m_nSequence; //0x0018
		float m_flPrevCycle; //0x001C
		float m_flWeight; //0x0020
		float m_flWeightDeltaRate; //0x0024
		float m_flPlaybackRate; //0x0028
		float m_flCycle; //0x002C
		void* m_pOwner; //0x0030 // player's thisptr
		char  pad_0038[4]; //0x0034
};
