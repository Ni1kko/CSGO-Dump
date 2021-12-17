#pragma once
#include "../Include/GlobalIncludes.h"
#include "vector3d.h"
#include "UtlVector.hpp"
#define END_OF_FREE_LIST -1
#define ENTRY_IN_USE -2

struct GlowObjectDefinition_t
{
	int32_t m_nNextFreeSlot;

	CBaseEntity* m_pEntity;
	union
	{
		Vector m_vGlowColor;
		struct
		{
			float   m_flRed;
			float   m_flGreen;
			float   m_flBlue;
		};
	};
	float m_flGlowAlpha;
	uint8_t pad_0014[4];
	float m_flSomeFloatThingy;
	uint8_t pad_001C[4];
	float m_flAnotherFloat;
	bool m_bRenderWhenOccluded;
	bool m_bRenderWhenUnoccluded;
	bool m_bFullBloomRender;
	uint8_t pad_0027[5];
	int32_t m_bPulsatingChams;
	int32_t m_nSplitScreenSlot;

	bool IsUnused() const
	{
		return m_nNextFreeSlot != ENTRY_IN_USE;
	}
	static const int END_OF_FREE_LIST1 = -1;
	static const int ENTRY_IN_USE1 = -2;
};

class CGlowObjectManager
{
	public:
		GlowObjectDefinition_t* m_GlowObjectDefinitions;
		int max_size;
		int pad;
		int size;
		GlowObjectDefinition_t* m_Unknown;
		int    currentObjects;
};

enum class EGlowStyle
{
	Outlined = 0,
	Model,
	Inline,
	Inline_Pulsate
};
