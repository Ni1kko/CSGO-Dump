
#define END_OF_FREE_LIST -1
#define ENTRY_IN_USE -2


struct GlowObjectDefinition_t
{
	GlowObjectDefinition_t() { memset(this, 0, sizeof(*this)); }

	class CBaseEntity* m_pEntity;    //0x0000
	union
	{
		Vector m_vGlowColor;           //0x0004
		struct
		{
			float   m_flRed;           //0x0004
			float   m_flGreen;         //0x0008
			float   m_flBlue;          //0x000C
		};
	};
	float   m_flAlpha;                 //0x0010
	uint8_t pad_0014[4];               //0x0014
	float   m_flSomeFloat;             //0x0018
	uint8_t pad_001C[4];               //0x001C
	float   m_flAnotherFloat;          //0x0020
	bool    m_bRenderWhenOccluded;     //0x0024
	bool    m_bRenderWhenUnoccluded;   //0x0025
	bool    m_bFullBloomRender;        //0x0026
	uint8_t pad_0027[5];               //0x0027
	int32_t m_nGlowStyle;              //0x002C
	int32_t m_nSplitScreenSlot;        //0x0030
	int32_t m_nNextFreeSlot;           //0x0034

	bool IsUnused() const { return m_nNextFreeSlot != ENTRY_IN_USE; }
}; //Size: 0x0038 (56)

class CGlowObjectManager
{
public:
	int RegisterGlowObject(CBaseEntity *pEntity, const Vector &vGlowColor, float flGlowAlpha, bool bRenderWhenOccluded, bool bRenderWhenUnoccluded, int nSplitScreenSlot)
	{
		int nIndex;
		if (m_nFirstFreeSlot == END_OF_FREE_LIST) {
			nIndex = -1;
		}
		else {
			nIndex = m_nFirstFreeSlot;
			m_nFirstFreeSlot = m_GlowObjectDefinitions[nIndex].m_nNextFreeSlot;
		}

		m_GlowObjectDefinitions[nIndex].m_pEntity = pEntity;
		m_GlowObjectDefinitions[nIndex].m_vGlowColor = vGlowColor;
		m_GlowObjectDefinitions[nIndex].m_flAlpha = flGlowAlpha;
		m_GlowObjectDefinitions[nIndex].m_bRenderWhenOccluded = bRenderWhenOccluded;
		m_GlowObjectDefinitions[nIndex].m_bRenderWhenUnoccluded = bRenderWhenUnoccluded;
		m_GlowObjectDefinitions[nIndex].m_nSplitScreenSlot = nSplitScreenSlot;
		m_GlowObjectDefinitions[nIndex].m_nNextFreeSlot = ENTRY_IN_USE;

		return nIndex;
	}

	void UnregisterGlowObject(int nGlowObjectHandle)
	{
		m_GlowObjectDefinitions[nGlowObjectHandle].m_nNextFreeSlot = m_nFirstFreeSlot;
		m_GlowObjectDefinitions[nGlowObjectHandle].m_pEntity = NULL;
		m_nFirstFreeSlot = nGlowObjectHandle;
	}

	void SetEntity(int nGlowObjectHandle, CBaseEntity *pEntity)
	{
		m_GlowObjectDefinitions[nGlowObjectHandle].m_pEntity = pEntity;
	}

	void SetColor(int nGlowObjectHandle, const Vector &vGlowColor)
	{
		m_GlowObjectDefinitions[nGlowObjectHandle].m_vGlowColor = vGlowColor;
	}

	void SetAlpha(int nGlowObjectHandle, float flAlpha)
	{
		m_GlowObjectDefinitions[nGlowObjectHandle].m_flAlpha = flAlpha;
	}

	void SetRenderFlags(int nGlowObjectHandle, bool bRenderWhenOccluded, bool bRenderWhenUnoccluded)
	{
		m_GlowObjectDefinitions[nGlowObjectHandle].m_bRenderWhenOccluded = bRenderWhenOccluded;
		m_GlowObjectDefinitions[nGlowObjectHandle].m_bRenderWhenUnoccluded = bRenderWhenUnoccluded;
	}

	GlowObjectDefinition_t* m_GlowObjectDefinitions; //0x0000

	int GetSize()
	{
		return *reinterpret_cast<int*>(uintptr_t(this) + 0xC);
	}

	int m_nFirstFreeSlot;                              //0x000C
}; extern CGlowObjectManager* g_GlowManager;
/*enum class EGlowStyle
{
	/// <summary>
	/// Default outline glow effect.
	/// </summary>
	Outlined = 0,
	/// <summary>
	/// Glowing model.
	/// </summary>
	Model,
	/// <summary>
	/// Inline glow effect.
	/// </summary>
	Inline,
	/// <summary>
	/// Inline pulsating glow effect.
	/// </summary>
	Inline_Pulsate
};
class CGlowObjectDefinition
{
public:
	uint32_t get_entity()
	{
		return baseEntity;
	}
	bool IsUnused() const
	{
		return nextFreeSlot != ENTRY_IN_USE;
	}
public:
	uint32_t baseEntity;           /// 0x00
	float    red;                  /// 0x04
	float    green;                /// 0x08
	float    blue;                 /// 0x0C
	float    alpha;                /// 0x10
private:
	uint8_t  _pad0014[0x10];     /// 0x14
public:
	bool     renderWhenOccluded;   /// 0x24
	bool     renderWhenUnoccluded; /// 0x25
	bool     fullBloomRender;      /// 0x26
private:
	uint8_t  __pad0027[0x5];     /// 0x27
public:
	int32_t  style;                /// 0x2C
	int32_t  splitScreenSlot;      /// 0x30
	int32_t  nextFreeSlot;         /// 0x34
public:
	static constexpr int32_t END_OF_FREE_LIST = -1;
	static constexpr int32_t ENTRY_IN_USE = -2;
}; /// Size=0x38
class CGlowObjectManager
{
public:
	CGlowObjectDefinition* m_GlowObjectDefinitions;
	int GetSize()
	{
		return *reinterpret_cast<int*>(uintptr_t(this) + 0xC);
	}
private:
};*/