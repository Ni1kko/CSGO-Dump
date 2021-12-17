#pragma once

class ICvar : public IAppSystem
{
public:
	ConVar* find(const char* name)
	{
		return CallVFunction<ConVar*(__thiscall*)(ICvar*, const char*)>(this, 16)(this, name);
	}

	void call_callbacks(ConVar* var, const char* pOldString, float flOldValue)
	{
		CallVFunction<void(__thiscall*)(ICvar*, ConVar *, const char*, float)>(this, 22)(this, var, pOldString, flOldValue);
	}
};