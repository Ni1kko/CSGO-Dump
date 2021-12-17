#include "knife_fix.hpp"
#include <stdlib.h>
#include <string.h>


KnifeAnimFix* g_KnifeAnimFix = new KnifeAnimFix();


int KnifeAnimFix::RandomSeq(const int low, const int high) {
	return rand() % (high - low + 1) + low;
}

int KnifeAnimFix::Fix(const char* model, const int sequence) {
	enum ESequence
	{
		SEQUENCE_DEFAULT_DRAW = 0,
		SEQUENCE_DEFAULT_IDLE1 = 1,
		SEQUENCE_DEFAULT_IDLE2 = 2,
		SEQUENCE_DEFAULT_LIGHT_MISS1 = 3,
		SEQUENCE_DEFAULT_LIGHT_MISS2 = 4,
		SEQUENCE_DEFAULT_HEAVY_MISS1 = 9,
		SEQUENCE_DEFAULT_HEAVY_HIT1 = 10,
		SEQUENCE_DEFAULT_HEAVY_BACKSTAB = 11,
		SEQUENCE_DEFAULT_LOOKAT1 = 12,

		SEQUENCE_FALCHION_IDLE1 = 1,
		SEQUENCE_FALCHION_HEAVY_MISS1 = 8,
		SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP = 9,
		SEQUENCE_FALCHION_LOOKAT1 = 12,
		SEQUENCE_FALCHION_LOOKAT2 = 13,

		SEQUENCE_BOWIE_IDLE1 = 1,

		SEQUENCE_BUTTERFLY_DRAW = 0,
		SEQUENCE_BUTTERFLY_DRAW2 = 1,
		SEQUENCE_BUTTERFLY_LOOKAT1 = 13,
		SEQUENCE_BUTTERFLY_LOOKAT3 = 15,

		SEQUENCE_DAGGERS_IDLE1 = 1,
		SEQUENCE_DAGGERS_LIGHT_MISS1 = 2,
		SEQUENCE_DAGGERS_LIGHT_MISS5 = 6,
		SEQUENCE_DAGGERS_HEAVY_MISS2 = 11,
		SEQUENCE_DAGGERS_HEAVY_MISS1 = 12,

		SEQUENCE_URSUS_DRAW = 0,
		SEQUENCE_URSUS_DRAW2 = 1,
		SEQUENCE_URSUS_LOOKAT1 = 13,
		SEQUENCE_URSUS_LOOKAT3 = 15,

		SEQUENCE_STILETTO_LOOKAT1 = 12,
		SEQUENCE_STILETTO_LOOKAT2 = 13,

		SEQUENCE_TALON_LOOKAT1 = 14,
		SEQUENCE_TALON_LOOKAT2 = 15
	};


	if (strstr(model, "models/weapons/v_knife_canis.mdl") || strstr(model, "models/weapons/v_knife_outdoor.mdl") || strstr(model, "models/weapons/v_knife_cord.mdl") || strstr(model, "models/weapons/v_knife_skeleton.mdl")) {
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_DRAW:
			return RandomSeq(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
		case SEQUENCE_DEFAULT_LOOKAT1:
			return RandomSeq(SEQUENCE_BUTTERFLY_LOOKAT1, 14);
		default:
			return sequence + 1;
		}
	}

	if (strstr(model, "models/weapons/v_knife_butterfly.mdl")) {
		switch (sequence) {
		case SEQUENCE_DEFAULT_DRAW:
			return RandomSeq(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
		case SEQUENCE_DEFAULT_LOOKAT1:
			return RandomSeq(SEQUENCE_BUTTERFLY_LOOKAT1, SEQUENCE_BUTTERFLY_LOOKAT3);
		default:
			return sequence + 1;
		}
	}
	else if (strstr(model, "models/weapons/v_knife_falchion_advanced.mdl")) {
		switch (sequence) {
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_FALCHION_IDLE1;
		case SEQUENCE_DEFAULT_HEAVY_MISS1:
			return RandomSeq(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
		case SEQUENCE_DEFAULT_LOOKAT1:
			return RandomSeq(SEQUENCE_FALCHION_LOOKAT1, SEQUENCE_FALCHION_LOOKAT2);
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		default:
			return sequence - 1;
		}
	}
	else if (strstr(model, "models/weapons/v_knife_push.mdl")) {
		switch (sequence) {
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_DAGGERS_IDLE1;
		case SEQUENCE_DEFAULT_LIGHT_MISS1:
		case SEQUENCE_DEFAULT_LIGHT_MISS2:
			return RandomSeq(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
		case SEQUENCE_DEFAULT_HEAVY_MISS1:
			return RandomSeq(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
		case SEQUENCE_DEFAULT_HEAVY_HIT1:
		case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
		case SEQUENCE_DEFAULT_LOOKAT1:
			return sequence + 3;
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		default:
			return sequence + 2;
		}
	}
	else if (strstr(model, "models/weapons/v_knife_survival_bowie.mdl")) {
		switch (sequence) {
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_BOWIE_IDLE1;
		default:
			return sequence - 1;
		}
	}
	else if (strstr(model, "models/weapons/v_knife_widowmaker.mdl")) {
		switch (sequence) {
		case SEQUENCE_DEFAULT_LOOKAT1:
			return RandomSeq(SEQUENCE_TALON_LOOKAT1, SEQUENCE_TALON_LOOKAT2);
			break;
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		default:
			return sequence + 2;
			break;
		}
	}
	else if (strstr(model, "models/weapons/v_knife_ursus.mdl")) {
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_DRAW:
			return RandomSeq(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
		case SEQUENCE_DEFAULT_LOOKAT1:
			return RandomSeq(SEQUENCE_BUTTERFLY_LOOKAT1, 14);
		default:
			return sequence + 1;
			break;
		}
	}
	else if (strstr(model, "models/weapons/v_knife_ursus.mdl")) {
		return 0;
	}
	else if (strstr(model, "models/weapons/v_knife_cord.mdl")) {
		return 0;
	}
	else if (strstr(model, "models/weapons/v_knife_canis.mdl")) {
		return 0;
	}
	else if (strstr(model, "models/weapons/v_knife_outdoor.mdl")) {
		return 0;
	}
	else if (strstr(model, "models/weapons/v_knife_skeleton.mdl"))
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_DRAW:
			return RandomSeq(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
		case SEQUENCE_DEFAULT_LOOKAT1:
			return RandomSeq(SEQUENCE_BUTTERFLY_LOOKAT1, 14);
		default:
			return sequence + 1;
		}
	}
	else {
		return sequence;
	}
}