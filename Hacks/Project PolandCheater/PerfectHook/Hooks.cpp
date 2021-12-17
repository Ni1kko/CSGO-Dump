#include "HookIncludes.h"
#include "HooksList.h"
#include "Listener.h"



namespace hooks
{
    vfunc_hook panel;
    vfunc_hook client;
    vfunc_hook clientmode;
    vfunc_hook modelrender;
    vfunc_hook prediction;
    vfunc_hook surface;
    vfunc_hook eventmanager;
    vfunc_hook d3d;
    vfunc_hook viewrender;
    vfunc_hook engine;
    vfunc_hook sv_cheats;

    void initialize()
    {
        client.setup(g_CHLClient);
        client.hook_index(36, hkFrameStageNotify);
        client.hook_index(21, hkCreateMove);

        //prediction.setup(g_Prediction);
        //prediction.hook_index(14, hkInPrediction);

        clientmode.setup(g_ClientMode);
        clientmode.hook_index(18, hkOverrideView);
        clientmode.hook_index(35, hkGetViewModelFOV);
        clientmode.hook_index(44, hkDoPostScreenSpaceEffects);

        panel.setup(g_Panel);
        panel.hook_index(41, hkPaintTraverse);

        modelrender.setup(g_ModelRender);
        modelrender.hook_index(21, hkDrawModelExecute);

        surface.setup(g_Surface);
        surface.hook_index(82, hkPlaySound);

        //engine.setup(g_Engine);
        //engine.hook_index(27, hkIsConnected);
        //engine.hook_index(93, hkIsHltv);




        ConVar* sv_cheats_con = g_CVar->FindVar("sv_cheats");
        sv_cheats.setup(sv_cheats_con);
        sv_cheats.hook_index(13, hkSvCheatsGetBool);



        m_present = U::pattern_scan(GetModuleHandleW(L"gameoverlayrenderer.dll"), "FF 15 ? ? ? ? 8B F8 85 DB 74 1F") + 0x2;//big ( large ) obs bypass
        m_reset = U::pattern_scan(GetModuleHandleW(L"gameoverlayrenderer.dll"), "FF 15 ? ? ? ? 8B F8 85 FF 78 18") + 0x2;  //big ( large ) obs bypass



        oPresent = **reinterpret_cast<Present_T**>(m_present);
        oReset = **reinterpret_cast<Reset_t**>(m_reset);

        **reinterpret_cast<void***>(m_present) = reinterpret_cast<void*>(&hkPresent);
        **reinterpret_cast<void***>(m_reset) = reinterpret_cast<void*>(&hkReset);




        /*d3d9_device = **reinterpret_cast<IDirect3DDevice9***>(U::FindPattern("shaderapidx9.dll", (PBYTE)"\xA1\x00\x00\x00\x00\x50\x8B\x08\xFF\x51\x0C", "x????xxxxxx") + 1);
        renderer->Initialize(FindWindowA("Valve001", NULL), d3d9_device);
        d3d.Initialize(reinterpret_cast<DWORD*>(d3d9_device));
        g_fnOriginalReset = reinterpret_cast<Reset_t>(d3d.Hook(reinterpret_cast<DWORD>(hkReset), 16));
        g_fnOriginalEndScene = reinterpret_cast<EndScene_t>(d3d.Hook(reinterpret_cast<DWORD>(hkEndScene), 42));*/

        item_purchase::singleton()->initialize();
    }
    void cleanup()
    {
        item_purchase::singleton()->remove();
        client.unhook_all();
        prediction.unhook_all();
        clientmode.unhook_all();
        panel.unhook_all();
        modelrender.unhook_all();
        surface.unhook_all();
        //EventManager.Unhook();
        viewrender.unhook_all();
        //d3d.Unhook();
        sv_cheats.unhook_all();
        engine.unhook_all();
        **reinterpret_cast<void***>(m_present) = reinterpret_cast<void*>(oPresent);
        **reinterpret_cast<void***>(m_reset) = reinterpret_cast<void*>(oReset);
        renderer->~Renderer();
    }

}