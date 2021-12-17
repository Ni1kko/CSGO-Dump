#pragma once

class IVRenderView
{
public:
	void SetColorModulation(float const* blend)
	{
		CallVFunction<void(__thiscall*)(void*, float const*)>(this, 6)(this, blend);
	}

	inline void SetColorModulation(float r, float g, float b)
	{
		float clr[3] = { r, g, b };
		SetColorModulation(clr);
	}
};
