#pragma once
#include "..\SDK\CInput.h"
#include "..\SDK\CEntity.h"
#include "..\Utils\GlobalVars.h"
#include "..\SDK\ClientClass.h"
#include "..\SDK\UtlVector.h"
#include "..\Globals.h"

class CGlowObjectManager
{
public:
	class GlowObjectDefinition_t {
	public:
		void Set(Color color, float bloom, int style)
		{
			m_vGlowColor = Vector(color.red / 255.f, color.green / 255.f, color.blue / 255.f);
			m_flGlowAlpha = color.alpha / 255.f;
			m_bRenderWhenOccluded = true;
			m_bRenderWhenUnoccluded = false;
			m_flBloomAmount = bloom;
			m_nGlowStyle = style;
		}

		C_BaseEntity* GetEnt() {
			return m_hEntity;
		}

		bool IsUnused() const { return m_nNextFreeSlot != GlowObjectDefinition_t::ENTRY_IN_USE; }

	public:
		C_BaseEntity*    m_hEntity;
		Vector            m_vGlowColor;
		float            m_flGlowAlpha;

		char            unknown[4]; //pad 
		float            flUnk; //confirmed to be treated as a float while reversing glow functions 
		float            m_flBloomAmount;
		float            localplayeriszeropoint3;


		bool            m_bRenderWhenOccluded;
		bool            m_bRenderWhenUnoccluded;
		bool            m_bFullBloomRender;
		char            unknown1[1]; //pad 


		int                m_nFullBloomStencilTestValue; // 0x28 
		int                m_nGlowStyle;
		int                m_nSplitScreenSlot; //Should be -1 

											   // Linked list of free slots 
		int                m_nNextFreeSlot;

		// Special values for GlowObjectDefinition_t::m_nNextFreeSlot 
		static const int END_OF_FREE_LIST = -1;
		static const int ENTRY_IN_USE = -2;
	};


	CUtlVector< GlowObjectDefinition_t > m_GlowObjectDefinitions;
	int m_nFirstFreeSlot;
};

class Do
{
public:
	void DrawGlow();
};

extern Do* DoGlow;
extern CGlowObjectManager* g_pGlowManager;