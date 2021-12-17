#pragma once

#include "Convar.hpp"
class SpoofedConvar {
public:
    SpoofedConvar();
    SpoofedConvar(const char* szCVar);
    SpoofedConvar(ConVar* pCVar, char* newname);

    ~SpoofedConvar();

    bool           IsSpoofed();
    void           Spoof();

    void           SetFlags(int flags);
    int            GetFlags();

    void           SetBool(bool bValue);
    void           SetInt(int iValue);
    void           SetFloat(float flValue);
    void           SetString(const char* szValue);

private:
    ConVar* m_pOriginalCVar = NULL;
    ConVar* m_pDummyCVar = NULL;

    char m_szDummyName[128];
    char m_szDummyValue[128];
    char m_szOriginalName[128];
    char m_szOriginalValue[128];
    int m_iOriginalFlags;
    char* m_szReplacementName;
};

class MinspecCvar
{
public:
    MinspecCvar(const char* szCVar, char* newname, float newvalue);
    ~MinspecCvar();

    bool ValidCvar();
    void Spoof();

    template<typename T>
    void SetValue(T value);

    int	GetInt();
    float GetFloat();
    const char* GetString();
private:
    ConVar* m_pConVar;

    const char* m_szOriginalName;
    char* m_szReplacementName;
    float m_OriginalValue;
    float m_newvalue;
};