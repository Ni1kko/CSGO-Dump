#pragma once

typedef unsigned char byte;
#define _CRT_SECURE_NO_WARNINGS

#define CONCAT_IMPL(x, y) x##y
#define MACRO_CONCAT(x, y) CONCAT_IMPL(x, y)
#define PAD(size) byte MACRO_CONCAT(_pad, __COUNTER__)[size];
	
struct surface_physics_params
{
	float friction;
	float elasticity;
	float density;
	float thickness;
	float dampening;
};

/*
* surface_audio_params
*/
struct surface_audio_params
{
	float reflectivity;
	float hardnessfactor;
	float roughnessfactor;
	float roughthreshold;
	float hardthreshold;
	float hardvelocitythreshold;
};

/*
* surface_sound_names
*/
struct surface_sound_names
{
	unsigned short walkstepleft;
	unsigned short walkstepright;
	unsigned short runstepleft;
	unsigned short runstepright;
	unsigned short impactsoft;
	unsigned short impacthard;
	unsigned short scrapesmooth;
	unsigned short scraperough;
	unsigned short bulletimpact;
	unsigned short rolling;
	unsigned short breaksound;
	unsigned short strainsound;
};

/*
* surface_sound_handles
*/
struct surface_sound_handles
{
	short walkstepleft;
	short walkstepright;
	short runstepleft;
	short runstepright;
	short impactsoft;
	short impacthard;
	short scrapesmooth;
	short scraperough;
	short bulletimpact;
	short rolling;
	short breaksound;
	short strainsound;
};

/*
* surface_game_props
*/
struct surface_game_props
{
	float maxspeedfactor;
	float jumpfactor;
	PAD(0x4)
		float penetrationmodifier;
	float damagemodifier;
	unsigned short material;
	PAD(0x3)
};

struct surface_data
{
	surface_physics_params physics;
	surface_audio_params audio;
	surface_sound_names sounds;
	surface_game_props game;
	surface_sound_handles soundhandles;
};