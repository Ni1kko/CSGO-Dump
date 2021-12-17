#define MINIe

using System;
using System.Diagnostics;
using System.IO;
using System.Threading;
using System.Windows.Forms;
using System.Windows.Media;
using DDX.Enums;
using DDX.Extensions;
using DDX.Functions;
using DDX.Functions.Bsp;
using DDX.Keyboard;
using DDX.Objects;
using DDX.Structs;
// ReSharper disable CompareOfFloatsByEqualityOperator

namespace DDX.Core
{
    public class ThreadManager : IDisposable
    {
        public Thread ForceWriteThread;
        public Thread BunnyhopThread;
        public Thread GlowThread;
        public Thread FakeLagThread;
        public Thread ChatThread;
        public Thread MainThread;

        public void Dispose()
        {
            Stop();
        }

        public void Start()
        {
            ObjectHelper.AssignLocalPlayer();
            StartMainThread();

            StartBunnyhopThread();
            StartChatThread();
            StartFakeLagThread();

#if !MINI
            StartForceWriteThread();
            StartGlowThread();
#endif
        }

        private void StartForceWriteThread()
        {
            ForceWriteThread = new Thread(ForceWrite) {IsBackground = true};
            ForceWriteThread.Start();
        }

        private void StartBunnyhopThread()
        {
            BunnyhopThread = new Thread(Bunnyhop) {IsBackground = true};
            BunnyhopThread.Start();
        }

        private void StartGlowThread()
        {
            GlowThread = new Thread(Glow) {IsBackground = true};
            GlowThread.Start();
        }

        private void StartFakeLagThread()
        {
            FakeLagThread = new Thread(FakeLag) {IsBackground = true};
            FakeLagThread.Start();
        }

        private void StartChatThread()
        {
            ChatThread = new Thread(Chat) {IsBackground = true};
            ChatThread.Start();
        }

        private void StartMainThread()
        {
            MainThread = new Thread(Main) {IsBackground = true};
            MainThread.Start();
        }

        public void Stop()
        {
            BunnyhopThread?.Abort();
            ChatThread?.Abort();
            FakeLagThread?.Abort();
            ForceWriteThread?.Abort();
            GlowThread?.Abort();

            MainThread?.Abort();
        }

        private bool _inGame;

        private void ForceWrite()
        {
            try
            {
                while (true)
                {
                    if (!_inGame ||
                        !G.C.Misc.NoHands &&
                        !G.C.Misc.NoSmoke)
                    {
                        Thread.Sleep(1000);
                        continue;
                    }
                    
                    F.DoNoHands();
                    F.DoNoSmoke();

                    Thread.Sleep(1);
                }
            }
            catch (ThreadAbortException)
            {
            }
            catch (Exception ex)
            {
                Debug.Fail("", ex.ToString());
                G.Ref += 30;
                ForceWrite();
            }
        }

        private void Bunnyhop()
        {
            try
            {
                while (true)
                {
                    if (!_inGame ||
                        !G.C.Misc.Bunnyhop)
                    {
                        Thread.Sleep(1000);
                        continue;
                    }

                    if (!F.DoBunnyhop())
                    {
                        if (G.C.Misc.BunnyhopProp.AutoStrafe)
                        {
                            F.LastStrafeAngles = G.Local.GetViewAngles();

                            if (F.Strafing == StrafingState.Right)
                            {
                                G.Local.SetRight(0);
                                F.Strafing = StrafingState.Off;
                            }
                            else if (F.Strafing == StrafingState.Left)
                            {
                                G.Local.SetLeft(0);
                                F.Strafing = StrafingState.Off;
                            }
                        }
                    }

                    Thread.Sleep(1);
                }
            }
            catch (ThreadAbortException)
            {
            }
            catch (Exception ex)
            {
                Debug.Fail("", ex.ToString());
                G.Ref += 30;
                Bunnyhop();
            }
        }

        private void Glow()
        {
            try
            {
                while (true)
                {
                    if (!_inGame ||
                        !G.C.Vis.Players.Glow &&
                        !G.C.Vis.Entities.Glow)
                    {
                        Thread.Sleep(1000);
                        continue;
                    }

                    var glowObjectHandle = GlobalRead.GetGlowObjectHandle();

                    for (var i = 0; i < GlobalRead.GetGlowObjectLength(); i++)
                    {
                        var glowHandle = glowObjectHandle + i * 0x38;
                        var entityHandle = G.Mem.Read<IntPtr>(glowHandle);

                        if (entityHandle == IntPtr.Zero ||
                            entityHandle == G.Local.Handle)
                        {
                            // Invalid
                            continue;
                        }

                        var e = new Entity(-1, entityHandle, glowHandle);
                        if (e.GetIsDormant())
                        {
                            // Not active
                            continue;
                        }

                        var classId = e.GetClassId();
                        var classCat = classId.GetCategory();
                        if (classCat == ClassCategory.Player)
                        {
                            // Player

                            if (!G.C.Vis.Players.Glow)
                            {
                                continue;
                            }

                            var p = new Player(-1, entityHandle, glowHandle);

                            if (!p.GetIsAlive())
                            {
                                // Dead
                                continue;
                            }

                            var isAlly = p.GetIsAlly();

                            if (G.C.Vis.HideAllies && isAlly ||
                                G.C.Vis.HideEnemies && !isAlly)
                            {
                                // Exclude
                                continue;
                            }

                            if (G.C.Vis.Players.GlowProp.Health)
                            {
                                // Glow.Health

                                Color minColor;
                                Color maxColor;

                                if (isAlly)
                                {
                                    // Ally

                                    minColor = G.C.Clr.GlowAlliesMinHealth;
                                    maxColor = G.C.Clr.GlowAlliesMaxHealth;
                                }
                                else
                                {
                                    // Enemy

                                    if (G.C.Vis.Players.VisibilityCheck && p.GetIsVisible())
                                    {
                                        // Visible

                                        minColor = G.C.Clr.GlowEnemiesMinHealthVisible;
                                        maxColor = G.C.Clr.GlowEnemiesMaxHealthVisible;
                                    }
                                    else
                                    {
                                        // Hidden

                                        minColor = G.C.Clr.GlowEnemiesMinHealth;
                                        maxColor = G.C.Clr.GlowEnemiesMaxHealth;
                                    }
                                }

                                var health = p.GetHealth();

                                var a = health / 25500f;
                                var b = (100 - health) / 25500f;

                                var final = new GlowColor(
                                    maxColor.R * a +
                                    minColor.R * b,
                                    maxColor.G * a +
                                    minColor.G * b,
                                    maxColor.B * a +
                                    minColor.B * b,
                                    maxColor.A * a +
                                    minColor.A * b);

                                p.SetGlowColor(final, 
                                    G.C.Vis.Players.GlowProp.Fill, 
                                    G.C.Vis.Players.GlowProp.Inner,
                                    G.C.Vis.Players.GlowProp.FillTransparent);
                            }
                            else
                            {
                                // Glow.Static

                                Color color;

                                if (isAlly)
                                {
                                    // Ally

                                    color = G.C.Clr.GlowAllies;
                                }
                                else
                                {
                                    // Enemy

                                    if (G.C.Vis.Players.VisibilityCheck && p.GetIsVisible())
                                    {
                                        // Visible

                                        color = G.C.Clr.GlowEnemiesVisible;
                                    }
                                    else
                                    {
                                        // Hidden

                                        color = G.C.Clr.GlowEnemies;
                                    }
                                }

                                p.SetGlowColor(color, G.C.Vis.Players.GlowProp.Fill, G.C.Vis.Players.GlowProp.Inner, G.C.Vis.Players.GlowProp.FillTransparent);
                            }
                        }
                        else
                        {
                            // Entity

                            if (!G.C.Vis.Entities.Glow)
                            {
                                continue;
                            }

                            if (G.C.Vis.Entities.ImportantOnly && !classId.IsImportant())
                            {
                                // Not important
                                continue;
                            }

                            if (G.C.Vis.HideChickens && classId == ClassId.CChicken)
                            {
                                // Exclude
                                continue;
                            }

                            Color color;

                            switch (classCat)
                            {
                                case ClassCategory.Bomb:
                                    color = G.C.Clr.GlowBomb;
                                    break;
                                case ClassCategory.Chicken:
                                    color = G.C.Clr.GlowChickens;
                                    break;
                                case ClassCategory.Gear:
                                    color = G.C.Clr.GlowGear;
                                    break;
                                case ClassCategory.Grenade:
                                    color = G.C.Clr.GlowGrenades;
                                    break;
                                case ClassCategory.Hostage:
                                    color = G.C.Clr.GlowHostages;
                                    break;
                                case ClassCategory.Weapon:
                                    color = G.C.Clr.GlowWeapons;
                                    break;
                                default:
                                    continue;
                            }

                            e.SetGlowColor(color, 
                                G.C.Vis.Entities.GlowProp.Fill, 
                                G.C.Vis.Entities.GlowProp.Inner,
                                G.C.Vis.Entities.GlowProp.FillTransparent);
                        }
                    }

                    Thread.Sleep(1);
                }
            }
            catch (ThreadAbortException)
            {
            }
            catch (Exception ex)
            {
                Debug.Fail("", ex.ToString());
                G.Ref += 30;
                Glow();
            }
        }

        private void FakeLag()
        {
            try
            {
                while (true)
                {
                    if (!_inGame &&
                        G.Local.GetSendPackets(true))
                    {
                        // Not ingame and send packets

                        Thread.Sleep(1000);
                        continue;
                    }

                    if (!G.C.Misc.FakeLag &&
                        G.Local.GetSendPackets(true))
                    {
                        // FakeLag off and send packets

                        Thread.Sleep(1000);
                        continue;
                    }

                    F.DoFakeLag();

                    Thread.Sleep(1);
                }
            }
            catch (ThreadAbortException)
            {
            }
            catch (Exception ex)
            {
                Debug.Fail("", ex.ToString());
                G.Ref += 30;
                FakeLag();
            }
        }

        private void Chat()
        {
            try
            {
                while (true)
                {
                    if (!_inGame ||
                        !G.C.Misc.Spam &&
                        !G.C.Misc.ChatInfo)
                    {
                        Thread.Sleep(1000);
                        continue;
                    }

                    F.DoSpam();
                    F.DoChatInfo();

                    // Sleep in functions
                }
            }
            catch (ThreadAbortException)
            {
            }
            catch (Exception ex)
            {
                Debug.Fail("", ex.ToString());
                G.Ref += 30;
                Chat();
            }
        }

        private void Main()
        {
            try
            {
                var stopwatch = new Stopwatch();

                while (true)
                {
                    stopwatch.Start();

                    G.Ref++;
                    ObjectHelper.AssignLocalPlayer();

                    var tmpInGame = G.Local.GetIsInGame();

                    if (tmpInGame && !_inGame)
                    {
                        if (!G.Local.GetIsAlive())
                        {
                            // Wait for player to spawn

                            Thread.Sleep(500);
                            continue;
                        }
                    }

                    _inGame = tmpInGame;

                    if (!_inGame)
                    {
                        // Not ingame

                        F.BeginDraw();
                        F.EndDraw();

                        Thread.Sleep(1000);
                        continue;
                    }

                    if (!G.Mem.IsProcessFocused())
                    {
                        // Process not focused

                        F.BeginDraw();
                        F.EndDraw();

                        Thread.Sleep(100);
                        continue;
                    }

                    if (G.Bsp == null)
                    {
                        // Bsp not initialized

                        var csgoDir = Path.GetDirectoryName(G.Mem.TargetProcess.MainModule.FileName);
                        var mapDir = GlobalRead.GetMapDir();

                        var mapName = GlobalRead.GetMapName();

                        G.Bsp = new BspParsing(Path.Combine(csgoDir, "csgo", mapDir), mapName);
                    }
                    else
                    {
                        // Bsp initialized

                        var mapName = GlobalRead.GetMapName();
                        if (mapName != G.Bsp.MapName)
                        {
                            // Map changed

                            var csgoDir = Path.GetDirectoryName(G.Mem.TargetProcess.MainModule.FileName);
                            var mapDir = GlobalRead.GetMapDir();

                            G.Bsp = new BspParsing(Path.Combine(csgoDir, "csgo", mapDir), mapName);
                        }
                    }

                    G.Now = Environment.TickCount;
                    G.Bomb = null;

                    F.BackgroundBrush = G.C.Clr.Background.ToBrush();
                    F.BeginDraw();

                    F.DoBrightness();

                    var processEntities = G.C.Vis.Entities.Chams ||
                                          G.C.Vis.Entities.Esp2D ||
                                          G.C.Vis.Entities.Esp3D ||
                                          G.C.Vis.Entities.Name ||
                                          G.C.Vis.Entities.Snaplines ||
                                          G.C.Vis.Entities.LocalChams;

                    var processBomb = (G.C.Vis.Entities.AdditionalInfo||
                                       G.C.Misc.BombDamage ||
                                       G.C.Misc.Radar) &&
                                       GlobalRead.GetIsBombPlanted();

                    if (processEntities || processBomb)
                    {
                        foreach (var e in ObjectHelper.GetEntities())
                        {
                            // Update handle
                            e.Handle = GlobalRead.GetEntityHandleByIndex(e.Index);
                            
                            if (e.Handle == IntPtr.Zero)
                            {
                                // Invalid
                                continue;
                            }

                            if (!G.Local.GetIsAlive() &&
                                G.Local.GetObserverTargetIndex() == e.Index &&
                                G.Local.GetObserverMode() != ObserverMode.FreeCam)
                            {
                                // Spectated by local player
                                continue;
                            }

                            if (processBomb)
                            {
                                // Assign bomb

                                if (e.GetClassId() == ClassId.CPlantedC4)
                                {
                                    G.Bomb = new Bomb(e.Index, e.Handle);

                                    F.DoRadar(e);

                                    if (!processEntities)
                                    {
                                        break;
                                    }
                                }
                            }

#if !MINI
                            if (G.C.Vis.Entities.LocalChams)
                            {
                                if (e.GetClassId() == ClassId.CPredictedViewModel)
                                {
                                    e.SetRenderColor(G.C.Clr.LocalChams);
                                }
                            }
#endif

                            if (G.C.Vis.Entities.ImportantOnly && !e.GetIsImportant())
                            {
                                // Not important
                                continue;
                            }

                            if (G.C.Vis.HideChickens && e.GetClassId() == ClassId.CChicken)
                            {
                                // Exclude
                                continue;
                            }

                            if (e.GetClassId() == ClassId.CPredictedViewModel)
                            {
                                // Exclude
                                continue;
                            }

                            var screenOrigin = e.GetScreenOrigin();
                            if (!screenOrigin.IsValid())
                            {
                                // Not on screen
                                continue;
                            }

                            F.Do3D(e);
                            F.Do2D(e);
                            F.DoSnaplines(e);
                            F.DoName(e);

#if !MINI
                            F.DoChams(e);
#endif
                        }
                    }

                    var cfg = FuncHelper.GetWeaponConfig();

                    var aimTarget = new AimTarget { Distance = 10000f };
                    var silentTarget = new AimTarget { Distance = 10000f };

                    var angles = G.Local.GetViewAngles();

                    var bulletAngles = cfg.Rcs ?
                        angles + G.Local.GetPunch() * 2f :
                        angles;

                    foreach (var p in ObjectHelper.GetPlayers())
                    {
                        // Update handle
                        p.Handle = GlobalRead.GetEntityHandleByIndex(p.Index);

                        if (p.Handle == IntPtr.Zero ||
                            p.Handle == G.Local.Handle)
                        {
                            // Invalid
                            continue;
                        }

                        if (!p.GetIsAlive())
                        {
                            // Dead
                            continue;
                        }

                        if (!G.Local.GetIsAlive() && 
                            G.Local.GetObserverTargetIndex() == p.Index &&
                            G.Local.GetObserverMode() != ObserverMode.FreeCam)
                        {
                            // Spectated by local player
                            continue;
                        }

                        var model = p.GetPlayerModel();

                        // Select AimTarget
                        if (cfg.Aimbot && 
                            !p.GetIsAlly() && 
                            G.Local.GetIsMouseEnable() && 
                            G.Local.GetIsAlive())
                        {
                            // Check weapon
                            if (G.Local.GetCanShoot())
                            {
                                // Check OnGround
                                if (!cfg.AimbotProp.OnGroundOnly || 
                                    p.GetIsOnGround())
                                {
                                    bool visible;

                                    if (cfg.AimbotProp.VisibilityCheckProp.BspParsing)
                                    {
                                        // Bsp

                                        visible = G.Bsp.IsVisible(G.Local.GetViewOrigin(),
                                            model.GetBoneOrigin(Bone.Chest));
                                    }
                                    else
                                    {
                                        // Normal

                                        visible = p.GetTeam() == G.Local.GetTeam() ||
                                                  !cfg.AimbotProp.VisibilityCheck ||
                                                  p.GetIsVisible();
                                    }

                                    // Check visibility
                                    if (visible)
                                    {
                                        ViewAngles targetAngles;
                                        float distance;

                                        var aimBone = Bone.Chest;

                                        if (cfg.AimbotProp.Bone == Bone.Dynamic)
                                        {
                                            // Bone.Dynamic

                                            var aimTargetAngles = new ViewAngles();
                                            var aimDistance = 10000f;

                                            for (var i = 0; i < 4; i++)
                                            {
                                                var thisTargetAngles = model.GetBoneAngles((Bone) i);
                                                var thisDistance = FuncHelper.DistanceViewAngles(bulletAngles, thisTargetAngles);

                                                if (thisDistance > aimDistance)
                                                {
                                                    // Distance is longer
                                                    continue;
                                                }

                                                aimTargetAngles = thisTargetAngles;
                                                aimDistance = thisDistance;
                                                                                                
                                                aimBone = (Bone)i;
                                            }

                                            if (aimDistance != 10000f)
                                            {
                                                // Dynamic found
                                                targetAngles = aimTargetAngles;
                                                distance = aimDistance;
                                            }
                                            else
                                            {
                                                // Dynamic not found
                                                targetAngles = model.GetBoneAngles(Bone.Chest);
                                                distance = FuncHelper.DistanceViewAngles(bulletAngles, targetAngles);
                                            }
                                        }
                                        else
                                        {
                                            // Bone.Static

                                            targetAngles = model.GetBoneAngles(cfg.AimbotProp.Bone);
                                            distance = FuncHelper.DistanceViewAngles(bulletAngles, targetAngles);

                                            aimBone = cfg.AimbotProp.Bone;
                                        }

                                        // Check Fov
                                        if (distance < aimTarget.Distance &&
                                            distance < cfg.AimbotProp.Fov)
                                        {
                                            // Check Lock
                                            if (!cfg.AimbotProp.Lock || 
                                                F.AimbotLockPlayerHandle == IntPtr.Zero ||
                                                F.AimbotLockLast + cfg.AimbotProp.LockProp.Length < G.Now ||
                                                F.AimbotLockPlayerHandle == p.Handle)
                                            {
                                                // Assign

                                                aimTarget.Selected = true;
                                                aimTarget.Player = p;

                                                aimTarget.ViewAngles = targetAngles;
                                                aimTarget.Distance = distance;
                                                aimTarget.AimBone = aimBone;
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        // Select SilentTarget
                        if (cfg.SilentAim && 
                            !p.GetIsAlly() && 
                            G.Local.GetIsMouseEnable() && 
                            G.Local.GetIsAlive() &&
                            (cfg.SilentAimProp.Key == Keys.F23 ||
                            KeyboardHelper.IsKeyPressed(cfg.SilentAimProp.Key)))
                        {
                            // Check weapon
                            if (G.Local.GetCanShoot())
                            {
                                // Check OnGround
                                if (!cfg.SilentAimProp.OnGroundOnly || 
                                    p.GetIsOnGround())
                                {
                                    bool visible;

                                    if (cfg.SilentAimProp.VisibilityCheckProp.BspParsing)
                                    {
                                        // Bsp

                                        visible = G.Bsp.IsVisible(G.Local.GetViewOrigin(),
                                            model.GetBoneOrigin(Bone.Chest));
                                    }
                                    else
                                    {
                                        // Normal

                                        visible = p.GetTeam() == G.Local.GetTeam() ||
                                                  !cfg.SilentAimProp.VisibilityCheck ||
                                                  p.GetIsVisible();
                                    }

                                    // Check visibility
                                    if (visible)
                                    {
                                        ViewAngles targetAngles;
                                        float distance;
                                        
                                        var aimBone = Bone.Chest;

                                        if (cfg.SilentAimProp.Bone == Bone.Dynamic)
                                        {
                                            // Bone.Dynamic

                                            var aimTargetAngles = new ViewAngles();
                                            var aimDistance = 10000f;

                                            for (var i = 0; i < 4; i++)
                                            {
                                                var thisTargetAngles = model.GetBoneAngles((Bone)i);
                                                var thisDistance = FuncHelper.DistanceViewAngles(bulletAngles, thisTargetAngles);

                                                if (thisDistance > aimDistance)
                                                {
                                                    // Distance is longer

                                                    continue;
                                                }

                                                aimTargetAngles = thisTargetAngles;
                                                aimDistance = thisDistance;
                                                
                                                aimBone = (Bone)i;
                                            }

                                            if (aimDistance != 10000f)
                                            {
                                                // Dynamic found

                                                targetAngles = aimTargetAngles;
                                                distance = aimDistance;
                                            }
                                            else
                                            {
                                                // Dynamic not found

                                                targetAngles = model.GetBoneAngles(Bone.Chest);
                                                distance = FuncHelper.DistanceViewAngles(bulletAngles, targetAngles);
                                            }
                                        }
                                        else
                                        {
                                            // Bone.Static

                                            targetAngles = model.GetBoneAngles(cfg.SilentAimProp.Bone);
                                            distance = FuncHelper.DistanceViewAngles(bulletAngles, targetAngles);

                                            aimBone = cfg.SilentAimProp.Bone;
                                        }

                                        // Check Fov
                                        if (distance < silentTarget.Distance &&
                                            distance < cfg.SilentAimProp.Fov)
                                        {
                                            // Assign

                                            silentTarget.Selected = true;
                                            silentTarget.Player = p;

                                            silentTarget.ViewAngles = targetAngles;
                                            silentTarget.Distance = distance;
                                            silentTarget.AimBone = aimBone;
                                        }
                                    }
                                }
                            }
                        }

                        if (G.C.Vis.HideAllies && p.GetIsAlly() ||
                            G.C.Vis.HideEnemies && !p.GetIsAlly())
                        {
                            // Exclude
                            continue;
                        }

                        var screenOrigin = p.GetScreenOrigin();
                        if (!screenOrigin.IsValid())
                        {
                            // Not on screen
                            continue;
                        }

                        var headRaw = model.GetScreenBoneOrigin(Bone.Head);
                        if (!headRaw.IsValid())
                        {
                            // Not on screen
                            continue;
                        }

                        var neckRaw = model.GetScreenBoneOrigin(Bone.Neck);
                        if (!neckRaw.IsValid())
                        {
                            // Not on screen
                            continue;
                        }

                        var headRadius = FuncHelper.Distance(headRaw, neckRaw);
                        if (headRadius > 2000)
                        {
                            continue;
                        }

                        var d = new DrawData();
                        var diff = screenOrigin.Y - headRaw.Y;
                        d.Width = diff / 2.1f;
                        d.X = screenOrigin.X - d.Width / 2;
                        d.Y = headRaw.Y - diff / 10;
                        d.Height = diff * 1.1f;

                        F.DoFloor(p);
                        F.Do3D(p, model);
                        F.DoRotation(p);
                        F.Do2D(p, d);
                        F.DoSkeleton(p, model);
                        F.DoSnaplines(p);

                        F.DoActiveWeapon(p, d);
                        F.DoAdditionalInfo(p, d);
                        F.DoArmor(p, d);
                        F.DoHealth(p, d);
                        F.DoName(p, d);
                        F.DoRank(p, d);

#if !MINI
                        F.DoChams(p);
                        F.DoRadar(p);
#endif
                    }

                    if (G.Local.GetIsAlive())
                    {
                        F.DoSilentAim(silentTarget, cfg);

                        if (!F.DoAimbot(aimTarget, cfg))
                        {
                            if (!KeyboardHelper.IsKeyPressed(cfg.AimbotProp.Key) &&
                                !KeyboardHelper.IsKeyPressed(cfg.TriggerbotWithAimbotKey))
                            {
                                // Free lock
                                F.AimbotLockLast = 0;
                                F.AimbotLockPlayerHandle = IntPtr.Zero;
                            }

                            F.DoRcs(cfg);
                        }

                        F.DoTriggerbot(cfg);

#if !MINI
                        F.DoKnifebot();
                        F.DoAutoPistol();
#endif

                        F.DoSilentAimFov(cfg);
                        F.DoAimFov(cfg);
                        F.DoMoveHelper();
                        F.DoAimPoint(aimTarget, cfg);
                        F.DoFastReload();
                        F.DoFastSnipers();
                        F.DoRecoilCrosshair();

#if !MINI
                        F.DoAutoDuck(cfg);
                        F.DoAutoStop(cfg);
                        F.DoNoViewModel();
                        F.DoThirdPersonMode();
#endif
                    }
                    else
                    {
                        // Free lock
                        F.AimbotLockLast = 0;
                        F.AimbotLockPlayerHandle = IntPtr.Zero;
                    }

                    F.DoAdditionalInfo();
                    F.DoBombDamage();
                    F.DoSpectatorList();

#if !MINI
                    F.DoForceUpdate();
                    F.DoFovChanger();
                    F.DoNoFlash();
                    F.DoNoShake();
#endif

                    F.EndDraw();

#region FpsCap

                    if (stopwatch != null)
                    {
                        var delayLength = 1000f / 60f - stopwatch.ElapsedMilliseconds;
                        if (delayLength >= 1)
                        {
                            Thread.Sleep((int)delayLength);
                        }

                        stopwatch.Reset();
                    }
                    else
                    {
                        stopwatch = new Stopwatch();
                    }

#endregion
                }
            }
            catch (ThreadAbortException)
            {
            }
            catch (Exception ex)
            {
                Debug.Fail("", ex.ToString());
                G.Ref += 30;
                Main();
            }
        }
    }
}
