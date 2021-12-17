#pragma once

#include "../math/VMatrix.hpp"

class IPanel;
class C_BaseEntity;

enum class ClearFlags_t
{
    VIEW_CLEAR_COLOR = 0x1,
    VIEW_CLEAR_DEPTH = 0x2,
    VIEW_CLEAR_FULL_TARGET = 0x4,
    VIEW_NO_DRAW = 0x8,
    VIEW_CLEAR_OBEY_STENCIL = 0x10,
    VIEW_CLEAR_STENCIL = 0x20,
};


enum class MotionBlurMode_t
{
    MOTION_BLUR_DISABLE = 1,
    MOTION_BLUR_GAME = 2,
    MOTION_BLUR_SFM = 3
};

struct CViewSetup
{
    char _0x0000[16];
    __int32 x;
    __int32 x_old;
    __int32 y;
    __int32 y_old;
    __int32 width;
    __int32    width_old;
    __int32 height;
    __int32    height_old;
    char _0x0030[128];
    float fov;
    float fovViewmodel;
    Vector origin;
    Vector angles;
    float zNear;
    float zFar;
    float zNearViewmodel;
    float zFarViewmodel;
    float m_flAspectRatio;
    float m_flNearBlurDepth;
    float m_flNearFocusDepth;
    float m_flFarFocusDepth;
    float m_flFarBlurDepth;
    float m_flNearBlurRadius;
    float m_flFarBlurRadius;
    float m_nDoFQuality;
    __int32 m_nMotionBlurMode;
    char _0x0104[68];
    __int32 m_EdgeBlur;
};
class CBaseViewport;
class CBaseHudWeaponSelection;
typedef unsigned long HCursor;
class CHudChat
{
public:
    void ChatPrintf(int iPlayerIndex, int iFilter, const char* fmt, ...)
    {
        char msg[1024];

        va_list args;
        va_start(args, fmt);
        vsprintf(msg, fmt, args);
        CallVFunction<void(__cdecl*)(void*, int, int, const char*, ...)>(this, 26)(this, iPlayerIndex, iFilter, fmt);
        va_end(args);
    }
};
class IClientMode
{
public:
    virtual             ~IClientMode() {}
    virtual int         ClientModeCSNormal(void *) = 0;
    virtual void        Init() = 0;
    virtual void        InitViewport() = 0;
    virtual void        Shutdown() = 0;
    virtual void        Enable() = 0;
    virtual void        Disable() = 0;
    virtual void        Layout() = 0;
    virtual IPanel*     GetViewport() = 0;
    virtual void*       GetViewportAnimationController() = 0;
    virtual void        ProcessInput(bool bActive) = 0;
    virtual bool        ShouldDrawDetailObjects() = 0;
    virtual bool        ShouldDrawEntity(C_BaseEntity *pEnt) = 0;
    virtual bool        ShouldDrawLocalPlayer(C_BaseEntity *pPlayer) = 0;
    virtual bool        ShouldDrawParticles() = 0;
    virtual bool        ShouldDrawFog(void) = 0;
    virtual void        OverrideView(CViewSetup *pSetup) = 0;
    virtual int         KeyInput(int down, int keynum, const char *pszCurrentBinding) = 0;
    virtual void        StartMessageMode(int iMessageModeType) = 0;
    virtual IPanel*     GetMessagePanel() = 0;
    virtual void        OverrideMouseInput(float *x, float *y) = 0;
    virtual bool        CreateMove(float flInputSampleTime, void* usercmd) = 0;
    virtual void        LevelInit(const char *newmap) = 0;
    virtual void        LevelShutdown(void) = 0;
public:
    void* ptr1;
    void* ptr2;
    CBaseViewport			*m_pViewport;
    // Message mode handling
    // All modes share a common chat interface
    CHudChat			*m_pChatElement; 
    HCursor			m_CursorNone;
    CBaseHudWeaponSelection *m_pWeaponSelection;
    int						m_nRootSize[2];
};