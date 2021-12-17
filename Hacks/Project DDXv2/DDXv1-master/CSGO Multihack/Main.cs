using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading;
using System.Windows.Forms;
using DDX_Multihack.Properties;
using FadeAPI;
using FadeAPI.Enums;
using FadeAPI.Helpers;
using FadeAPI.Memory;
using FadeAPI.Objects;
using FadeAPI.Structs;
using SharpDX.Direct2D1;
using SharpDX.DirectWrite;
using SharpDX.Mathematics.Interop;

namespace DDX_Multihack
{
    internal static class Main
    {
        //[swap]
        public static Keys ForceUpdateKey;
        private static readonly List<PathData>[] PathData = new List<PathData>[64];
        public static Keys TriggerbotKey;
        private static AimTarget _soundEspTarget;
        public static bool AimbotPistols;
        public static bool RecoilBasedFov;
        public static RawColor4 HealthBarColor;
        public static bool Entities3D;
        public static float AimbotPistolsSpeed;
        public static bool SkinChanger;
        public static RawColor4 EntitiesColor;
        public static bool VisibilityCheck;
        public static RawColor4 NamesColor;
        public static float PathQuality;
        public static Bone AimbotPistolsBone;
        public static bool ExtendedInfo;
        public static float AimbotSnipersDelay;
        public static bool Fov;
        public static RawColor4 ChamsEnemiesColor;
        public static RawColor4 ChickensColor;
        public static bool Brightness;
        public static float AimbotLockTime;
        public static float SoundEspLengthScale;
        public static float FovChangerValue;
        public static RawColor4 BackgroundColor;
        public static bool Rank;
        public static float CsX;
        public static bool Players2D;
        public static RawColor4 PathAlliesColor;
        public static bool SoundEsp;
        public static bool RecoilCrosshair;
        public static bool HealthBar;
        public static Bone AimbotSnipersBone;
        public static bool Entities2D;
        public static bool HideEnemies;
        public static bool AimbotNonSticky;
        public static RawColor4 RanksColor;
        public static bool FovPistols;
        public static RawColor4 PathEnemiesColor;
        private static int _lastKills;
        private static int _lastFakeLagStart;
        public static Bone AimbotBone;
        public static bool RcsAimbotOnly;
        private static Thread _mainThread;
        public static float HeadHelperSize;
        private const FontWeight CFontWeight = FontWeight.Normal;
        public static bool FovSnipers;
        private static Thread _fakeLagThread;
        public static float TriggerbotSnipersDelay;
        public static bool Radar;
        private static string _spectatorsString;
        public static bool TriggerbotSnipers;
        public static RawColor4 AlliesColor;
        public static float CsY;
        public static float FakeLagPause;
        public static bool AutoPistol;
        public static float AimbotSpeed;
        private static int _lastSoundEspTickCount;
        public static bool Triggerbot;
        public static RawColor4 SkeletonEnemiesColor;
        public static bool Spectators;
        public static bool FakeLag;
        private static IntPtr _lastAimbotPlayerHandle;
        public static bool PathLocalOnly;
        public static RawColor4 EnemiesVisColor;
        public static bool Bunnyhop;
        public static RawColor4 HeadHelperColor;
        public static float SoundEspFrequency;
        public static bool ActiveWeapon;
        public static RawColor4 SkeletonEnemiesVisColor;
        public static bool FriendlyFire;
        public static bool AimbotVisibilityCheck;
        public static bool Rcs;
        public static bool Snaplines;
        private static Thread _bunnyhopThread;
        public static float FovSize;
        private static readonly int[] PathLastTickCounts = new int[64];
        public static float FovSnipersSize;
        public static bool HeadHelper;
        private static bool _inGame;
        public static bool SimpleRanks;
        public static bool Aimbot;
        public static float RcsStrengthY;
        private static Thread _forceWriteThread;
        public static float CsHeight;
        public static bool HideAllies;
        public static float GlowDelay;
        public static bool TriggerbotPistols;
        public static bool FullGlow;
        public static float RcsStrengthX;
        public static Keys AimbotKey;
        public static float FovPistolsSize;
        public static bool Knifebot;
        public static RawColor4 EnemiesColor;
        public static RawColor4 RecoilCrosshairColor;
        public static bool Players3D;
        public static float PathLength;
        public static bool Name;
        public static RawColor4 SpectatorsColor;
        public static float TriggerbotDelay;
        public static RawColor4 FovColor;
        public static bool AimbotSnipers;
        public static bool HideChickens;
        private static Vector2 _lastRcsVecPunch;
        public static bool PlayersGlow;
        private static Brush _backgroundBrush;
        public static float SoundEspFovSize;
        public static bool Skeleton;
        public static float AimbotSnipersSpeed;
        public static float WindowScale;
        public static float FpsLimit;
        public static bool Floor;
        public static bool RcsPistol;
        public static float CsWidth;
        public static bool FovChanger;
        public static RawColor4 ActiveWeaponColor;
        public static bool PlayersChams;
        public static bool RcsPositionFix;
        public static bool AntiFlash;
        public static bool PlayersHealthGlow;
        private static int _lastAimbotTickCount;
        public static bool StatTrak;
        public static float TriggerbotPistolsDelay;
        public static RawColor4 SkeletonAlliesColor;
        public static float AimbotPistolsDelay;
        public static float BrightnessStrength;
        public static RawColor4 ChamsAlliesColor;
        private static AimTarget _aimbotTarget;
        public static bool PathT;
        public static float FakeLagLength;
        public static RawColor4 SpecialEntitiesColor;
        public static RawColor4 ExtendedInfoColor;
        private static Thread _glowThread;
        private static bool _isAimbotLocked;
        public static RawVector2 ScreenCenter;
        //[/swap]

        public static void Start()
        {
            for (int i = 0; i < PathData.Length; i++)
                PathData[i] = new List<PathData>();

            //[swap]
            StartMainThread();
            StartBunnyhopThread();
            StartFakeLagThread();
            StartGlowThread();
            StartForceWriteThread();
            //[/swap]
        }

        private static void StartGlowThread()
        {
            _glowThread = new Thread(DoGlowThread)
            {
                //[swap]
                IsBackground = true,
                Priority = ThreadPriority.Highest,
                //[/swap]
            };
            _glowThread.Start();
        }

        private static void StartFakeLagThread()
        {
            _fakeLagThread = new Thread(DoFakeLagThread)
            {
                //[swap]
                Priority = ThreadPriority.Highest,
                IsBackground = true,
                //[/swap]
            };
            _fakeLagThread.Start();
        }

        private static void StartBunnyhopThread()
        {
            _bunnyhopThread = new Thread(DoBunnyhopThread)
            {
                //[swap]
                IsBackground = true,
                Priority = ThreadPriority.Highest,
                //[/swap]
            };
            _bunnyhopThread.Start();
        }

        private static void StartForceWriteThread()
        {
            _forceWriteThread = new Thread(DoForceWriteThread)
            {
                //[swap]
                Priority = ThreadPriority.Highest,
                IsBackground = true,
                //[/swap]
            };
            _forceWriteThread.Start();
        }

        private static void StartMainThread()
        {
            _mainThread = new Thread(DoMainThread)
            {
                //[swap]
                Priority = ThreadPriority.Highest,
                IsBackground = true,
                //[/swap]
            };
            _mainThread.Start();
        }

        private static void DoGlowThread()
        {
            try
            {
                while (true)
                {
                    if (!PlayersGlow)
                    {
                        Thread.Sleep(1000);
                        goto skip;
                    }

                    if (!_inGame)
                    {
                        Thread.Sleep(1000);
                        goto skip;
                    }

                    var players = G.Fade.Players.Get();

                    foreach (var player in players)
                    {
                        if (player.Handle == G.Fade.LocalPlayer.Handle || player.IsDormant || player.Health == 0 || player.IsGlowing)
                            continue;

                        //[/swap]
                        //[block]
                        if (HideAllies && player.IsAllay && !FriendlyFire)
                            continue;
                        //[/block]
                        //[block]
                        if (HideEnemies && (!player.IsAllay || FriendlyFire))
                            continue;
                        //[/block]
                        //[/swap]

                        var isAlly = player.IsAllay && !FriendlyFire;
                        var color = isAlly ? AlliesColor : EnemiesColor;

                        if (VisibilityCheck && !isAlly && player.IsVisible)
                            color = EnemiesVisColor;

                        if (PlayersHealthGlow && !isAlly)
                            color = new RawColor4(1 - player.Health / 100f, player.Health / 100f, 0, 1);

                        color.A = .8f;
                        var glowColor = new GlowColor(color.R, color.G, color.B, color.A);
                        player.SetGlowColor(glowColor, FullGlow);
                    }

                    skip:
                    if (GlowDelay > 0)
                        Thread.Sleep((int)GlowDelay);
                }
            }
            catch
            {
                Thread.Sleep(1000);
                StartGlowThread();
            }
        }

        private static void DoFakeLagThread()
        {
            try
            {
                while (true)
                {
                    if (!FakeLag && G.Fade.LocalPlayer.SendPackets)
                    {
                        Thread.Sleep(1000);
                        continue;
                    }

                    if (!_inGame && G.Fade.LocalPlayer.SendPackets)
                    {
                        Thread.Sleep(1000);
                        continue;
                    }

                    DoFakeLag();

                    Thread.Sleep(1);
                }
            }
            catch
            {
                Thread.Sleep(1000);
                StartFakeLagThread();
            }
        }

        private static void DoBunnyhopThread()
        {
            try
            {
                while (true)
                {
                    if (!Bunnyhop)
                    {
                        Thread.Sleep(1000);
                        continue;
                    }

                    if (!_inGame)
                    {
                        Thread.Sleep(1000);
                        continue;
                    }

                    DoBunnyhop();

                    Thread.Sleep(1);
                }
            }
            catch
            {
                Thread.Sleep(1000);
                StartBunnyhopThread();
            }
        }

        private static void DoForceWriteThread()
        {
            try
            {
                while (true)
                {
                    if (!SkinChanger)
                    {
                        Thread.Sleep(1000);
                        continue;
                    }

                    if (!_inGame)
                    {
                        Thread.Sleep(1000);
                        continue;
                    }

                    DoSkinChanger();

                    Thread.Sleep(1);
                }
            }
            catch
            {
                Thread.Sleep(1000);
                StartForceWriteThread();
            }
        }

        private static void DoMainThread()
        {
            var stopwatch = new Stopwatch();
            stopwatch.Start();

            try
            {
                while (true)
                {
                    G.Fade.Overlay.Form.StartDrawing();

                    var frameLength = 1000f / FpsLimit;

                    G.Fade.Refresh();
                    _inGame = G.Fade.LocalPlayer.InGame;

                    if (!_inGame)
                    {
                        Thread.Sleep(1000);
                        goto skip;
                    }

                    //[swap]
                    _spectatorsString = "Spectators:\n";
                    _soundEspTarget.Distance = 10000f;
                    _aimbotTarget.Distance = 10000f;
                    _backgroundBrush = new SolidColorBrush(G.Fade.Overlay.Device, BackgroundColor);
                    //[/swap]

                    DoBrightness();

                    DoFov();

                    DoEntities();

                    var players = G.Fade.Players.Get();

                    foreach (var player in players)
                    {
                        DoPath(player);

                        if (Spectators && player.ObserveHandle == G.Fade.LocalPlayer.Handle)
                            _spectatorsString += player.Name + '\n';


                        if (player.Handle == G.Fade.LocalPlayer.Handle || player.IsDormant || player.Health == 0)
                            continue;

                        var screenOrigin = player.ScreenOrigin;
                        if (screenOrigin.X.Equals(-1f) && screenOrigin.Y.Equals(-1f))
                            continue;

                        if (G.Fade.LocalPlayer.Health == 0 && player.RawDistance < 50)
                            continue;

                        //[swap]
                        Vector2 aimScreenOrigin;
                        var model = player.ModelObject;
                        var rcsPunchVec = G.Fade.LocalPlayer.GetPunch();
                        var percentage = ScreenCenter.Y / 100;
                        float aimDistance;
                        var aimBone = AimbotBone;
                        Vector3 aimOrigin;
                        //[/swap]

                        if (G.Fade.LocalPlayer.ActiveWeapon.IsPistol())
                            aimBone = AimbotPistolsBone;
                        else if (G.Fade.LocalPlayer.ActiveWeapon.IsSniper())
                            aimBone = AimbotSnipersBone;

                        //[swap]
                        rcsPunchVec.X *= 2; // vertical
                        rcsPunchVec.Y *= 2; // horizontal
                        //[/swap]
                        //[swap]
                        var x = 0 - rcsPunchVec.Y;
                        var y = 0 - rcsPunchVec.X;
                        //[/swap]
                        //[swap]
                        x *= percentage;
                        y *= percentage;
                        x *= G.Fade.LocalPlayer.ScopeScale;
                        y *= G.Fade.LocalPlayer.ScopeScale;
                        //[/swap]

                        if (!RecoilBasedFov)
                        {
                            //[swap]
                            x = 0;
                            y = 0;
                            //[/swap]
                        }

                        if ((int)aimBone == -1)
                        {
                            //[swap]
                            var aimDistanceTmp = 10000f;
                            var aimScreenOriginTmp = new Vector2();
                            var aimOriginTmp = new Vector3();
                            //[/swap]

                            // old = 15
                            for (var i = 0; i < 4; i++)
                            {
                                var thisDistance = model.ScreenBoneDistance((Bone)i, new Vector2(ScreenCenter.X + x, ScreenCenter.Y - y));

                                if (!(thisDistance < aimDistanceTmp)) continue;

                                //[swap]
                                aimDistanceTmp = thisDistance;
                                aimOriginTmp = model.BoneOrigin((Bone)i);
                                aimScreenOriginTmp = model.ScreenBoneOrigin((Bone)i);
                                //[/swap]
                            }

                            if (!aimDistanceTmp.Equals(10000f))
                            {
                                //[swap]
                                aimOrigin = aimOriginTmp;
                                aimScreenOrigin = aimScreenOriginTmp;
                                aimDistance = aimDistanceTmp;
                                //[/swap]
                            }
                            else
                            {
                                //[swap]
                                aimOrigin = model.BoneOrigin(Bone.Head);
                                aimScreenOrigin = model.ScreenBoneOrigin(Bone.Head);
                                aimDistance = model.ScreenBoneDistance(Bone.Head, new Vector2(ScreenCenter.X + x, ScreenCenter.Y - y));
                                //[/swap]
                            }
                        }
                        else
                        {
                            //[swap]
                            aimOrigin = model.BoneOrigin(aimBone);
                            aimScreenOrigin = model.ScreenBoneOrigin(aimBone);
                            aimDistance = model.ScreenBoneDistance(aimBone, new Vector2(ScreenCenter.X + x, ScreenCenter.Y - y));
                            //[/swap]
                        }

                        if (aimScreenOrigin.X.Equals(-1f) && aimScreenOrigin.Y.Equals(-1f))
                            continue;

                        var aimFov = FovSize;
                        if (G.Fade.LocalPlayer.ActiveWeapon.IsPistol())
                            aimFov = FovPistolsSize;
                        else if (G.Fade.LocalPlayer.ActiveWeapon.IsSniper())
                            aimFov = FovSnipersSize;

                        var isVisible = !AimbotVisibilityCheck || player.IsVisible || player.IsAllay;
                        if (isVisible && !player.HasImmunity && (aimDistance < _aimbotTarget.Distance) && (aimDistance < aimFov))
                        {
                            if (_lastAimbotTickCount < Environment.TickCount)
                            {
                                if (!player.IsAllay || FriendlyFire)
                                {
                                    //[swap]
                                    _aimbotTarget.TargetOrigin = aimOrigin;
                                    _aimbotTarget.Player = player;
                                    _aimbotTarget.Distance = aimDistance;
                                    //[/swap]
                                }
                            }
                            else if (player.Handle == _lastAimbotPlayerHandle)
                            {
                                //[swap]
                                _aimbotTarget.Distance = aimDistance;
                                _aimbotTarget.TargetOrigin = aimOrigin;
                                _aimbotTarget.Player = player;
                                //[/swap]
                            }
                        }

                        if (SoundEsp && (!player.IsAllay || FriendlyFire) && (aimDistance < _soundEspTarget.Distance) && (aimDistance < SoundEspFovSize) && (_lastSoundEspTickCount < Environment.TickCount))
                            _soundEspTarget.Distance = aimDistance;

                        //[/swap]
                        //[block]
                        if (HideAllies && player.IsAllay && !FriendlyFire)
                            continue;
                        //[/block]
                        //[block]
                        if (HideEnemies && (!player.IsAllay || FriendlyFire))
                            continue;
                        //[/block]
                        //[/swap]

                        //[swap]
                        var headScreenOrigin = model.ScreenBoneOrigin(Bone.Head);
                        var drawData = new DrawData();
                        //[/swap]

                        if (headScreenOrigin.X.Equals(-1f) && headScreenOrigin.Y.Equals(-1f))
                            continue;

                        drawData.Height = player.ScreenOrigin.Y - headScreenOrigin.Y;
                        drawData.Width = drawData.Height / 2.1f;
                        drawData.X = player.ScreenOrigin.X - drawData.Width / 2;
                        drawData.Y = headScreenOrigin.Y - drawData.Height / 10f;
                        drawData.Height *= 1.1f;

                        //[swap]
                        drawData.X *= WindowScale;
                        drawData.Y *= WindowScale;
                        //[/swap]

                        //[swap]
                        drawData.X += CsX;
                        drawData.Y += CsY;
                        drawData.Height *= WindowScale;
                        drawData.Width *= WindowScale;
                        //[/swap]

                        DoSkeleton(player, model);

                        DoPlayers2D(player, drawData);

                        DoPlayers3D(player, model);

                        DoFloor(player);

                        DoSnaplines(player);

                        DoChams(player);

                        DoHealth(player, drawData);

                        DoHeadHelper(player, model);

                        DoName(player, drawData);

                        DoRank(player, drawData);

                        DoActiveWeapon(player, drawData);

                        DoAdditionalInfo(player, drawData);

                        DoRadar(player);
                    }

                    DoSpectators();

                    DoSound();

                    DoAntiFlash();

                    DoRecoilCrosshair();

                    DoAimbotRcs();

                    DoKnifebot();

                    DoTriggerbot();

                    DoAutoPistol();

                    DoStatTrak();

                    DoFovChanger();

                    DoForceFullUpdate();

                    skip:
                    G.Fade.Overlay.Form.StopDrawing();

                    if (!stopwatch.Equals(null))
                    {
                        var delayLength = frameLength - stopwatch.ElapsedMilliseconds;
                        if ((delayLength > 0) && !float.IsInfinity(frameLength))
                            Thread.Sleep((int)delayLength);

                        stopwatch.Restart();
                    }
                    else
                    {
                        stopwatch = new Stopwatch();
                        stopwatch.Start();
                    }
                }
            }
            catch
            {
                Thread.Sleep(1000);
                StartMainThread();
            }
        }

        // DoStuff /////////////////////////////
 
        private static void DoFakeLag()
        {
            if (!FakeLag && G.Fade.LocalPlayer.SendPackets)
                return;

            var endLag = _lastFakeLagStart + FakeLagLength;
            var endPause = endLag + FakeLagPause;
            var now = Environment.TickCount;

            if (G.Fade.Keyboard.IsKeyPressed(Keys.LButton) || G.Fade.LocalPlayer.Health == 0)
            {
                //wait
                G.Fade.LocalPlayer.SendPackets = true;
                return;
            }

            if (endLag > now)
            {
                //lag
                G.Fade.LocalPlayer.SendPackets = false;
                return;
            }

            if (endPause > now)
            {
                //wait
                G.Fade.LocalPlayer.SendPackets = true;
                return;
            }

            _lastFakeLagStart = now;
        }

        private static void DoFov()
        {
            var doFov = false;

            if (FovPistols && G.Fade.LocalPlayer.ActiveWeapon.IsPistol())
                doFov = true;
            else if (FovSnipers && G.Fade.LocalPlayer.ActiveWeapon.IsSniper())
                doFov = true;
            else if (Fov && !G.Fade.LocalPlayer.ActiveWeapon.IsPistol() && !G.Fade.LocalPlayer.ActiveWeapon.IsSniper())
                doFov = true;

            if (!doFov)
                return;

            //[swap]
            float y;
            float x;
            var brush = new SolidColorBrush(G.Fade.Overlay.Device, FovColor);
            var size = FovSize;
            //[/swap]

            if (G.Fade.LocalPlayer.ActiveWeapon.IsPistol())
                size = FovPistolsSize;
            else if (G.Fade.LocalPlayer.ActiveWeapon.IsSniper())
                size = FovSnipersSize;

            if (RecoilBasedFov)
            {
                //[swap]
                var rcsPunchVec = G.Fade.LocalPlayer.GetPunch();
                var percentage = ScreenCenter.Y / 100;
                //[/swap]
                //[swap]
                rcsPunchVec.Y *= 2; // horizontal
                rcsPunchVec.X *= 2; // vertical
                //[/swap]
                //[swap]
                x = 0 - rcsPunchVec.Y;
                y = 0 - rcsPunchVec.X;
                //[/swap]
                //[swap]
                y *= G.Fade.LocalPlayer.ScopeScale;
                x *= percentage;
                y *= percentage;
                x *= G.Fade.LocalPlayer.ScopeScale;
                //[/swap]
            }
            else
            {
                //[swap]
                y = 0;
                x = 0;
                //[/swap]
            }

            G.Fade.Overlay.Device.DrawEllipse(new Ellipse(new RawVector2(ScreenCenter.X + x, ScreenCenter.Y - y), size * WindowScale, size * WindowScale), _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawEllipse(new Ellipse(new RawVector2(ScreenCenter.X + x, ScreenCenter.Y - y), size * WindowScale, size * WindowScale), brush, 1);
        }

        private static void DoBrightness()
        {
            if (!Brightness)
                return;

            G.Fade.Overlay.Device.FillRectangle(new RawRectangleF(CsX, CsY, CsX + CsWidth, CsY + CsHeight), new SolidColorBrush(G.Fade.Overlay.Device, new RawColor4(1, 1, 1, BrightnessStrength)));
        }

        private static void DoEntities()
        {
            if (!Entities2D && !Entities3D)
                return;

            var gameObjects = G.Fade.GameObjects.Get();

            foreach (var gameObject in gameObjects)
            {
                var name = gameObject.ClassId.GetClassName();
                if ((name == string.Empty) || gameObject.IsDormant || gameObject.HasOwner)
                    continue;

                if (HideChickens && (name == "Chicken"))
                    continue;

                var screenOrigin = gameObject.ScreenOrigin;
                if (screenOrigin.X.Equals(-1f) && screenOrigin.Y.Equals(-1f))
                    continue;

                if (Entities3D)
                {
                    var p1 = gameObject.Origin;

                    //[swap]
                    const int size = 3;
                    var p3 = p1;
                    var p4 = p1;
                    var p2 = p1;
                    var brush = name == "Chicken" ? new SolidColorBrush(G.Fade.Overlay.Device, ChickensColor) : new SolidColorBrush(G.Fade.Overlay.Device, EntitiesColor);
                    //[/swap]

                    //[swap]
                    p2.Z -= size;
                    p4.X += size;
                    p3.Z -= size;
                    p1.X -= size;
                    p1.Z -= size;
                    brush = gameObject.ClassId.IsSpecialObject() ? new SolidColorBrush(G.Fade.Overlay.Device, SpecialEntitiesColor) : brush;
                    p2.X -= size;
                    p1.Y -= size;
                    p3.X += size;
                    p4.Z -= size;
                    p4.Y -= size;
                    p3.Y += size;
                    p2.Y += size;
                    //[/swap]

                    //[/swap]
                    //[block]
                    var p1ScreenOrigin = Functions.WorldToScreen(p1, GlobalRead.ViewMatrix);
                    if (p1ScreenOrigin.X.Equals(-1f) && p1ScreenOrigin.Y.Equals(-1f))
                        return;
                    //[/block]
                    //[block]
                    var p2ScreenOrigin = Functions.WorldToScreen(p2, GlobalRead.ViewMatrix);
                    if (p2ScreenOrigin.X.Equals(-1f) && p2ScreenOrigin.Y.Equals(-1f))
                        return;
                    //[/block]
                    //[block]
                    var p3ScreenOrigin = Functions.WorldToScreen(p3, GlobalRead.ViewMatrix);
                    if (p3ScreenOrigin.X.Equals(-1f) && p3ScreenOrigin.Y.Equals(-1f))
                        return;
                    //[/block]
                    //[block]
                    var p4ScreenOrigin = Functions.WorldToScreen(p4, GlobalRead.ViewMatrix);
                    if (p4ScreenOrigin.X.Equals(-1f) && p4ScreenOrigin.Y.Equals(-1f))
                        return;
                    //[/block]
                    //[/swap]

                    //[swap]
                    var p6 = p2;
                    var p8 = p4;
                    var p5 = p1;
                    var p7 = p3;
                    //[/swap]

                    //[swap]
                    //[block]
                    p7.Z += size * 2;
                    var p7ScreenOrigin = Functions.WorldToScreen(p7, GlobalRead.ViewMatrix);
                    if (p7ScreenOrigin.X.Equals(-1f) && p7ScreenOrigin.Y.Equals(-1f))
                        return;
                    //[/block]
                    //[block]
                    p6.Z += size * 2;
                    var p6ScreenOrigin = Functions.WorldToScreen(p6, GlobalRead.ViewMatrix);
                    if (p6ScreenOrigin.X.Equals(-1f) && p6ScreenOrigin.Y.Equals(-1f))
                        return;
                    //[/block]
                    //[block]
                    p8.Z += size * 2;
                    var p8ScreenOrigin = Functions.WorldToScreen(p8, GlobalRead.ViewMatrix);
                    if (p8ScreenOrigin.X.Equals(-1f) && p8ScreenOrigin.Y.Equals(-1f))
                        return;
                    //[/block]
                    //[block]
                    p5.Z += size * 2;
                    var p5ScreenOrigin = Functions.WorldToScreen(p5, GlobalRead.ViewMatrix);
                    if (p5ScreenOrigin.X.Equals(-1f) && p5ScreenOrigin.Y.Equals(-1f))
                        return;
                    //[/block]
                    //[/swap]

                    //[swap]
                    var p2ScreenOriginRaw = new RawVector2(CsX + p2ScreenOrigin.X * WindowScale, CsY + p2ScreenOrigin.Y * WindowScale);
                    var p7ScreenOriginRaw = new RawVector2(CsX + p7ScreenOrigin.X * WindowScale, CsY + p7ScreenOrigin.Y * WindowScale);
                    var p6ScreenOriginRaw = new RawVector2(CsX + p6ScreenOrigin.X * WindowScale, CsY + p6ScreenOrigin.Y * WindowScale);
                    var p3ScreenOriginRaw = new RawVector2(CsX + p3ScreenOrigin.X * WindowScale, CsY + p3ScreenOrigin.Y * WindowScale);
                    var p4ScreenOriginRaw = new RawVector2(CsX + p4ScreenOrigin.X * WindowScale, CsY + p4ScreenOrigin.Y * WindowScale);
                    var p1ScreenOriginRaw = new RawVector2(CsX + p1ScreenOrigin.X * WindowScale, CsY + p1ScreenOrigin.Y * WindowScale);
                    var p8ScreenOriginRaw = new RawVector2(CsX + p8ScreenOrigin.X * WindowScale, CsY + p8ScreenOrigin.Y * WindowScale);
                    var p5ScreenOriginRaw = new RawVector2(CsX + p5ScreenOrigin.X * WindowScale, CsY + p5ScreenOrigin.Y * WindowScale);
                    //[/swap]

                    //[swap]
                    G.Fade.Overlay.Device.DrawLine(p2ScreenOriginRaw, p6ScreenOriginRaw, _backgroundBrush, 2);
                    G.Fade.Overlay.Device.DrawLine(p4ScreenOriginRaw, p1ScreenOriginRaw, _backgroundBrush, 2);
                    G.Fade.Overlay.Device.DrawLine(p4ScreenOriginRaw, p8ScreenOriginRaw, _backgroundBrush, 2);
                    G.Fade.Overlay.Device.DrawLine(p6ScreenOriginRaw, p7ScreenOriginRaw, _backgroundBrush, 2);
                    G.Fade.Overlay.Device.DrawLine(p1ScreenOriginRaw, p5ScreenOriginRaw, _backgroundBrush, 2);
                    G.Fade.Overlay.Device.DrawLine(p8ScreenOriginRaw, p5ScreenOriginRaw, _backgroundBrush, 2);
                    G.Fade.Overlay.Device.DrawLine(p1ScreenOriginRaw, p2ScreenOriginRaw, _backgroundBrush, 2);
                    G.Fade.Overlay.Device.DrawLine(p3ScreenOriginRaw, p7ScreenOriginRaw, _backgroundBrush, 2);
                    G.Fade.Overlay.Device.DrawLine(p2ScreenOriginRaw, p3ScreenOriginRaw, _backgroundBrush, 2);
                    G.Fade.Overlay.Device.DrawLine(p5ScreenOriginRaw, p6ScreenOriginRaw, _backgroundBrush, 2);
                    G.Fade.Overlay.Device.DrawLine(p3ScreenOriginRaw, p4ScreenOriginRaw, _backgroundBrush, 2);
                    G.Fade.Overlay.Device.DrawLine(p7ScreenOriginRaw, p8ScreenOriginRaw, _backgroundBrush, 2);
                    //[/swap]

                    //[swap]
                    G.Fade.Overlay.Device.DrawLine(p2ScreenOriginRaw, p6ScreenOriginRaw, brush, 1);
                    G.Fade.Overlay.Device.DrawLine(p5ScreenOriginRaw, p6ScreenOriginRaw, brush, 1);
                    G.Fade.Overlay.Device.DrawLine(p4ScreenOriginRaw, p1ScreenOriginRaw, brush, 1);
                    G.Fade.Overlay.Device.DrawLine(p2ScreenOriginRaw, p3ScreenOriginRaw, brush, 1);
                    G.Fade.Overlay.Device.DrawLine(p3ScreenOriginRaw, p4ScreenOriginRaw, brush, 1);
                    G.Fade.Overlay.Device.DrawLine(p4ScreenOriginRaw, p8ScreenOriginRaw, brush, 1);
                    G.Fade.Overlay.Device.DrawLine(p1ScreenOriginRaw, p5ScreenOriginRaw, brush, 1);
                    G.Fade.Overlay.Device.DrawLine(p8ScreenOriginRaw, p5ScreenOriginRaw, brush, 1);
                    G.Fade.Overlay.Device.DrawLine(p1ScreenOriginRaw, p2ScreenOriginRaw, brush, 1);
                    G.Fade.Overlay.Device.DrawLine(p3ScreenOriginRaw, p7ScreenOriginRaw, brush, 1);
                    G.Fade.Overlay.Device.DrawLine(p6ScreenOriginRaw, p7ScreenOriginRaw, brush, 1);
                    G.Fade.Overlay.Device.DrawLine(p7ScreenOriginRaw, p8ScreenOriginRaw, brush, 1);
                    //[/swap]
                }

                if (Entities2D)
                {
                    var brush = name == "Chicken" ? new SolidColorBrush(G.Fade.Overlay.Device, ChickensColor) : new SolidColorBrush(G.Fade.Overlay.Device, EntitiesColor);
                    brush = gameObject.ClassId.IsSpecialObject() ? new SolidColorBrush(G.Fade.Overlay.Device, SpecialEntitiesColor) : brush;

                    //[swap]
                    var sizeP = 5 * gameObject.DistanceScale * WindowScale;
                    var size = 40 * gameObject.DistanceScale * WindowScale;
                    //[/swap]

                    //[swap]
                    //[block]
                    G.Fade.Overlay.Device.FillEllipse(new Ellipse(new RawVector2(CsX + screenOrigin.X * WindowScale, CsY + screenOrigin.Y * WindowScale), sizeP + 1, sizeP + 1), _backgroundBrush);
                    G.Fade.Overlay.Device.FillEllipse(new Ellipse(new RawVector2(CsX + screenOrigin.X * WindowScale, CsY + screenOrigin.Y * WindowScale), sizeP, sizeP), brush);
                    //[/block]
                    //[block]
                    if (size < 10)
                        size = 10;
                    //[/block]
                    //[/swap]

                    var textFormat = new TextFormat(new SharpDX.DirectWrite.Factory(), Settings.Default.Font, CFontWeight, FontStyle.Normal, size)
                    {
                        //[swap]
                        WordWrapping = WordWrapping.NoWrap,
                        TextAlignment = TextAlignment.Center,
                        //[/swap]
                    };

                    //[swap]
                    G.Fade.Overlay.Device.DrawText(name, textFormat, new RawRectangleF(CsX + screenOrigin.X * WindowScale - 1, CsY + screenOrigin.Y * WindowScale + 1, CsX + screenOrigin.X * WindowScale, CsY + screenOrigin.Y * WindowScale), _backgroundBrush);
                    G.Fade.Overlay.Device.DrawText(name, textFormat, new RawRectangleF(CsX + screenOrigin.X * WindowScale + 1, CsY + screenOrigin.Y * WindowScale - 1, CsX + screenOrigin.X * WindowScale, CsY + screenOrigin.Y * WindowScale), _backgroundBrush);
                    G.Fade.Overlay.Device.DrawText(name, textFormat, new RawRectangleF(CsX + screenOrigin.X * WindowScale + 1, CsY + screenOrigin.Y * WindowScale + 1, CsX + screenOrigin.X * WindowScale, CsY + screenOrigin.Y * WindowScale), _backgroundBrush);
                    G.Fade.Overlay.Device.DrawText(name, textFormat, new RawRectangleF(CsX + screenOrigin.X * WindowScale - 1, CsY + screenOrigin.Y * WindowScale - 1, CsX + screenOrigin.X * WindowScale, CsY + screenOrigin.Y * WindowScale), _backgroundBrush);
                    //[/swap]

                    G.Fade.Overlay.Device.DrawText(name, textFormat, new RawRectangleF(CsX + screenOrigin.X * WindowScale, CsY + screenOrigin.Y * WindowScale, CsX + screenOrigin.X * WindowScale, CsY + screenOrigin.Y * WindowScale), brush);
                }
            }
        }

        private static void DoPath(Player player)
        {
            if (!PathT)
            {
                foreach (var pathData in PathData)
                {
                    if (pathData.Count != 0)
                        pathData.Clear();
                }

                return;
            }

            //[/swap]
            //[block]
            if (HideAllies && player.IsAllay && !FriendlyFire)
                return;
            //[/block]
            //[block]
            if (HideEnemies && (!player.IsAllay || FriendlyFire))
                return;
            //[/block]
            //[/swap]

            var isLocal = player.Handle == G.Fade.LocalPlayer.Handle;
            if (!isLocal && PathLocalOnly)
            {
                if (PathData[player.Index].Count != 0)
                    PathData[player.Index].Clear();

                return;
            }

            // Analyze
            byte removed;
            for (int i = 0; i < PathData[player.Index].Count; i += 1 - removed)
            {
                //[swap]
                var vec = PathData[player.Index][i].Vector;
                var ticks = PathData[player.Index][i].Ticks - 1;
                removed = 0;
                //[/swap]

                PathData[player.Index][i] = new PathData(vec, ticks);

                // Remove
                if (PathData[player.Index][i].Ticks != 0)
                    continue;

                //[swap]
                PathData[player.Index].RemoveAt(i);
                removed = 1;
                //[/swap]
            }

            // Register
            if (PathLastTickCounts[player.Index] + PathQuality < Environment.TickCount && player.Health != 0)
            {
                //[swap]
                var origin = player.Origin;
                var ticks = (int)FpsLimit * (int)PathLength;
                //[/swap]

                //[swap]
                PathData[player.Index].Add(new PathData(origin, ticks));
                PathLastTickCounts[player.Index] = Environment.TickCount;
                //[/swap]
            }

            // Draw
            var brush = player.IsAllay ? new SolidColorBrush(G.Fade.Overlay.Device, PathAlliesColor) : new SolidColorBrush(G.Fade.Overlay.Device, PathEnemiesColor);

            for (int i = 0; i < PathData[player.Index].Count; i++)
            {
                if (i + 1 == PathData[player.Index].Count)
                    break;

                //[swap]
                //[block]
                var p1 = Functions.WorldToScreen(PathData[player.Index][i].Vector, GlobalRead.ViewMatrix);
                if (p1.X.Equals(-1f) && p1.Y.Equals(-1f))
                    continue;
                //[/block]
                //[block]
                var p2 = Functions.WorldToScreen(PathData[player.Index][i + 1].Vector, GlobalRead.ViewMatrix);
                if (p2.X.Equals(-1f) && p2.Y.Equals(-1f))
                    continue;
                //[/block]
                //[/swap]

                //[swap]
                var p1Raw = new RawVector2(CsX + p1.X * WindowScale, CsY + p1.Y * WindowScale);
                var p2Raw = new RawVector2(CsX + p2.X * WindowScale, CsY + p2.Y * WindowScale);
                //[/swap]

                G.Fade.Overlay.Device.DrawLine(p1Raw, p2Raw, _backgroundBrush, 2);
                G.Fade.Overlay.Device.DrawLine(p1Raw, p2Raw, brush, 1);
            }
        }

        private static void DoSkeleton(Player player, PlayerModel model)
        {
            if (!Skeleton)
                return;

            var isAllay = player.IsAllay && !FriendlyFire;

            var brush = isAllay ? new SolidColorBrush(G.Fade.Overlay.Device, SkeletonAlliesColor) : new SolidColorBrush(G.Fade.Overlay.Device, SkeletonEnemiesColor);

            if (VisibilityCheck && !isAllay && player.IsVisible)
                brush = new SolidColorBrush(G.Fade.Overlay.Device, SkeletonEnemiesVisColor);

            /* DEBUG
            var textFormat = new TextFormat(new SharpDX.DirectWrite.Factory(), "Consolas", CFontWeight, FontStyle.Normal, 16f);
            textFormat.WordWrapping = WordWrappinGlobal.Fade.Overlay.Window.Device.NoWrap;
            for (int i = 0; i < 100; i++)
            {
                var boneOrigin = model.BoneOrigin((Bone) i, true);
                var boneScreen = Functions.WorldToScreen(boneOrigin, GlobalRead.ViewMatrix);
                G.Fade.Overlay.Window.Device.DrawText(i.ToString(), textFormat,
                    new RawRectangleF(boneScreen.X, boneScreen.Y, 1920, 1080), new SolidColorBrush(G.Fade.Overlay.Window.Device, new RawColor4(1, 1, 1, 1)));
            }
            return;
            */

            //[swap]
            //[/block]
            var head = model.ScreenBoneOrigin(Bone.Head);
            if (head.X.Equals(-1f) && head.Y.Equals(-1f))
                return;
            //[/block]
            //[block]
            var la7 = model.ScreenBoneOrigin((Bone)7);
            if (la7.X.Equals(-1f) && la7.Y.Equals(-1f))
                return;
            //[/block]
            //[block]
            var la9 = model.ScreenBoneOrigin((Bone)9);
            if (la9.X.Equals(-1f) && la9.Y.Equals(-1f))
                return;
            //[/block]
            //[block]
            var stomach = model.ScreenBoneOrigin(Bone.Stomach);
            if (stomach.X.Equals(-1f) && stomach.Y.Equals(-1f))
                return;
            //[/block]
            //[block]
            var ll13 = model.ScreenBoneOrigin((Bone)13);
            if (ll13.X.Equals(-1f) && ll13.Y.Equals(-1f))
                return;
            //[/block]
            //[block]
            var la8 = model.ScreenBoneOrigin((Bone)8);
            if (la8.X.Equals(-1f) && la8.Y.Equals(-1f))
                return;
            //[/block]
            //[block]
            var ll15 = model.ScreenBoneOrigin((Bone)15);
            if (ll15.X.Equals(-1f) && ll15.Y.Equals(-1f))
                return;
            //[/block]
            //[block]
            var ll14 = model.ScreenBoneOrigin((Bone)14);
            if (ll14.X.Equals(-1f) && ll14.Y.Equals(-1f))
                return;
            //[/block]
            //[block]
            var chest = model.ScreenBoneOrigin(Bone.Chest);
            if (chest.X.Equals(-1f) && chest.Y.Equals(-1f))
                return;
            //[/block]
            //[block]
            var rl12 = model.ScreenBoneOrigin((Bone)12);
            if (rl12.X.Equals(-1f) && rl12.Y.Equals(-1f))
                return;
            //[/block]
            //[block]
            var ra4 = model.ScreenBoneOrigin((Bone)4);
            if (ra4.X.Equals(-1f) && ra4.Y.Equals(-1f))
                return;
            //[/block]
            //[block]
            var rl10 = model.ScreenBoneOrigin((Bone)10);
            if (rl10.X.Equals(-1f) && rl10.Y.Equals(-1f))
                return;
            //[/block]
            //[block]
            var ra6 = model.ScreenBoneOrigin((Bone)6);
            if (ra6.X.Equals(-1f) && ra6.Y.Equals(-1f))
                return;
            //[/block]
            //[block]
            var ra5 = model.ScreenBoneOrigin((Bone)5);
            if (ra5.X.Equals(-1f) && ra5.Y.Equals(-1f))
                return;
            //[/block]
            //[block]
            var rl11 = model.ScreenBoneOrigin((Bone)11);
            if (rl11.X.Equals(-1f) && rl11.Y.Equals(-1f))
                return;
            //[/block]
            //[block]
            var neck = model.ScreenBoneOrigin(Bone.Neck);
            if (neck.X.Equals(-1f) && neck.Y.Equals(-1f))
                return;
            //[/block]
            //[/swap]

            //[swap]
            var ll13Raw = new RawVector2(CsX + ll13.X * WindowScale, CsY + ll13.Y * WindowScale);
            var chestRaw = new RawVector2(CsX + chest.X * WindowScale, CsY + chest.Y * WindowScale);
            var rl12Raw = new RawVector2(CsX + rl12.X * WindowScale, CsY + rl12.Y * WindowScale);
            var la8Raw = new RawVector2(CsX + la8.X * WindowScale, CsY + la8.Y * WindowScale);
            var stomachRaw = new RawVector2(CsX + stomach.X * WindowScale, CsY + stomach.Y * WindowScale);
            var neckRaw = new RawVector2(CsX + neck.X * WindowScale, CsY + neck.Y * WindowScale);
            var rl10Raw = new RawVector2(CsX + rl10.X * WindowScale, CsY + rl10.Y * WindowScale);
            var la7Raw = new RawVector2(CsX + la7.X * WindowScale, CsY + la7.Y * WindowScale);
            var la9Raw = new RawVector2(CsX + la9.X * WindowScale, CsY + la9.Y * WindowScale);
            var ra5Raw = new RawVector2(CsX + ra5.X * WindowScale, CsY + ra5.Y * WindowScale);
            var ra6Raw = new RawVector2(CsX + ra6.X * WindowScale, CsY + ra6.Y * WindowScale);
            var headRaw = new RawVector2(CsX + head.X * WindowScale, CsY + head.Y * WindowScale);
            var ll15Raw = new RawVector2(CsX + ll15.X * WindowScale, CsY + ll15.Y * WindowScale);
            var ll14Raw = new RawVector2(CsX + ll14.X * WindowScale, CsY + ll14.Y * WindowScale);
            var headRadius = Functions.ScreenDistance(head, neck) * WindowScale;
            var rl11Raw = new RawVector2(CsX + rl11.X * WindowScale, CsY + rl11.Y * WindowScale);
            var ra4Raw = new RawVector2(CsX + ra4.X * WindowScale, CsY + ra4.Y * WindowScale);
            //[/swap]

            //[swap]
            G.Fade.Overlay.Device.DrawLine(stomachRaw, rl10Raw, _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawLine(rl11Raw, rl12Raw, _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawLine(rl10Raw, rl11Raw, _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawEllipse(new Ellipse(headRaw, headRadius, headRadius), _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawLine(chestRaw, neckRaw, _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawLine(ra4Raw, ra5Raw, _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawLine(stomachRaw, chestRaw, _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawLine(ra5Raw, ra6Raw, _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawLine(ll13Raw, ll14Raw, _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawLine(neckRaw, ra4Raw, _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawLine(ll14Raw, ll15Raw, _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawLine(la8Raw, la9Raw, _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawLine(la7Raw, la8Raw, _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawLine(stomachRaw, ll13Raw, _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawLine(neckRaw, la7Raw, _backgroundBrush, 2);
            //[/swap]

            //[swap]
            G.Fade.Overlay.Device.DrawLine(chestRaw, neckRaw, brush, 1);
            G.Fade.Overlay.Device.DrawLine(stomachRaw, chestRaw, brush, 1);
            G.Fade.Overlay.Device.DrawLine(ll13Raw, ll14Raw, brush, 1);
            G.Fade.Overlay.Device.DrawLine(ra5Raw, ra6Raw, brush, 1);
            G.Fade.Overlay.Device.DrawLine(la8Raw, la9Raw, brush, 1);
            G.Fade.Overlay.Device.DrawLine(stomachRaw, ll13Raw, brush, 1);
            G.Fade.Overlay.Device.DrawLine(rl10Raw, rl11Raw, brush, 1);
            G.Fade.Overlay.Device.DrawLine(neckRaw, ra4Raw, brush, 1);
            G.Fade.Overlay.Device.DrawLine(rl11Raw, rl12Raw, brush, 1);
            G.Fade.Overlay.Device.DrawEllipse(new Ellipse(headRaw, headRadius, headRadius), brush, 1);
            G.Fade.Overlay.Device.DrawLine(stomachRaw, rl10Raw, brush, 1);
            G.Fade.Overlay.Device.DrawLine(ll14Raw, ll15Raw, brush, 1);
            G.Fade.Overlay.Device.DrawLine(neckRaw, la7Raw, brush, 1);
            G.Fade.Overlay.Device.DrawLine(ra4Raw, ra5Raw, brush, 1);
            G.Fade.Overlay.Device.DrawLine(la7Raw, la8Raw, brush, 1);
            //[/swap]
        }

        private static void DoPlayers2D(Player player, DrawData drawData)
        {
            if (!Players2D)
                return;

            var isAllay = player.IsAllay && !FriendlyFire;

            var brush = isAllay ? new SolidColorBrush(G.Fade.Overlay.Device, AlliesColor) : new SolidColorBrush(G.Fade.Overlay.Device, EnemiesColor);

            if (VisibilityCheck && !isAllay && player.IsVisible)
                brush = new SolidColorBrush(G.Fade.Overlay.Device, EnemiesVisColor);

            // 1-11  21-2
            // |        |
            // 12      22
            //
            //
            // 32      42
            // |        |
            // 3-31  41-4

            //[swap]
            var len = drawData.Width / 4f;
            var p1 = new RawVector2(drawData.X, drawData.Y);
            var p2 = new RawVector2(drawData.X + drawData.Width, drawData.Y);
            var p3 = new RawVector2(drawData.X, drawData.Y + drawData.Height);
            //[/swap]

            var p4 = new RawVector2(p2.X, p3.Y);

            //[swap]
            var p32 = new RawVector2(p3.X, p3.Y - len);
            var p22 = new RawVector2(p2.X, p2.Y + len);
            var p41 = new RawVector2(p4.X - len, p4.Y);
            var p21 = new RawVector2(p2.X - len, p2.Y);
            var p31 = new RawVector2(p3.X + len, p3.Y);
            var p12 = new RawVector2(p1.X, p1.Y + len);
            var p42 = new RawVector2(p4.X, p4.Y - len);
            var p11 = new RawVector2(p1.X + len, p1.Y);
            //[/swap]

            //[swap]
            G.Fade.Overlay.Device.DrawLine(p4, p41, _backgroundBrush, 3);
            G.Fade.Overlay.Device.DrawLine(p1, p11, _backgroundBrush, 3);
            G.Fade.Overlay.Device.DrawLine(p1, p12, _backgroundBrush, 3);
            G.Fade.Overlay.Device.DrawLine(p4, p42, _backgroundBrush, 3);
            G.Fade.Overlay.Device.DrawLine(p3, p31, _backgroundBrush, 3);
            G.Fade.Overlay.Device.DrawLine(p3, p32, _backgroundBrush, 3);
            G.Fade.Overlay.Device.DrawLine(p2, p21, _backgroundBrush, 3);
            G.Fade.Overlay.Device.DrawLine(p2, p22, _backgroundBrush, 3);
            //[/swap]

            //[swap]
            G.Fade.Overlay.Device.DrawLine(p3, p31, brush, 2);
            G.Fade.Overlay.Device.DrawLine(p4, p41, brush, 2);
            G.Fade.Overlay.Device.DrawLine(p1, p12, brush, 2);
            G.Fade.Overlay.Device.DrawLine(p2, p21, brush, 2);
            G.Fade.Overlay.Device.DrawLine(p1, p11, brush, 2);
            G.Fade.Overlay.Device.DrawLine(p2, p22, brush, 2);
            G.Fade.Overlay.Device.DrawLine(p3, p32, brush, 2);
            G.Fade.Overlay.Device.DrawLine(p4, p42, brush, 2);
            //[/swap]

            /*
            var rectangle = new RawRectangleF(drawData.X, drawData.Y, drawData.X + drawData.Width, drawData.Y + drawData.Height);

            G.Fade.Overlay.Device.DrawRectangle(rectangle, _backgroundBrush, 3);
            G.Fade.Overlay.Device.DrawRectangle(rectangle, brush, 2);
            */
        }

        private static void DoPlayers3D(Player player, PlayerModel model)
        {
            if (!Players3D)
                return;

            var isAllay = player.IsAllay && !FriendlyFire;

            var brush = isAllay ? new SolidColorBrush(G.Fade.Overlay.Device, AlliesColor) : new SolidColorBrush(G.Fade.Overlay.Device, EnemiesColor);

            if (VisibilityCheck && !isAllay && player.IsVisible)
                brush = new SolidColorBrush(G.Fade.Overlay.Device, EnemiesVisColor);

            var p1 = player.Origin;

            //[swap]
            var p4 = p1;
            var p2 = p1;
            var p3 = p1;
            const int size = 15;
            //[/swap]

            //[swap]
            p4.Y -= size;
            p1.Y -= size;
            p1.X -= size;
            p3.X += size;
            p2.X -= size;
            p4.X += size;
            p2.Y += size;
            p3.Y += size;
            //[/swap]

            //[swap]
            //[block]
            var p2ScreenOrigin = Functions.WorldToScreen(p2, GlobalRead.ViewMatrix);
            if (p2ScreenOrigin.X.Equals(-1f) && p2ScreenOrigin.Y.Equals(-1f))
                return;
            //[/block]
            //[block]
            var p1ScreenOrigin = Functions.WorldToScreen(p1, GlobalRead.ViewMatrix);
            if (p1ScreenOrigin.X.Equals(-1f) && p1ScreenOrigin.Y.Equals(-1f))
                return;
            //[/block]
            //[block]
            var p3ScreenOrigin = Functions.WorldToScreen(p3, GlobalRead.ViewMatrix);
            if (p3ScreenOrigin.X.Equals(-1f) && p3ScreenOrigin.Y.Equals(-1f))
                return;
            //[/block]
            //[block]
            var p4ScreenOrigin = Functions.WorldToScreen(p4, GlobalRead.ViewMatrix);
            if (p4ScreenOrigin.X.Equals(-1f) && p4ScreenOrigin.Y.Equals(-1f))
                return;
            //[/block]
            //[/swap]

            var headOrigin = model.BoneOrigin(Bone.Head);

            //[swap]
            var p5 = p1;
            var p6 = p2;
            var p8 = p4;
            headOrigin.Z += 7;
            var p7 = p3;
            //[/swap]

            //[swap]
            //[block]
            p5.Z = headOrigin.Z;
            var p5ScreenOrigin = Functions.WorldToScreen(p5, GlobalRead.ViewMatrix);
            if (p5ScreenOrigin.X.Equals(-1f) && p5ScreenOrigin.Y.Equals(-1f))
                return;
            //[/block]
            //[block]
            p7.Z = headOrigin.Z;
            var p7ScreenOrigin = Functions.WorldToScreen(p7, GlobalRead.ViewMatrix);
            if (p7ScreenOrigin.X.Equals(-1f) && p7ScreenOrigin.Y.Equals(-1f))
                return;
            //[/block]
            //[block]
            p8.Z = headOrigin.Z;
            var p8ScreenOrigin = Functions.WorldToScreen(p8, GlobalRead.ViewMatrix);
            if (p8ScreenOrigin.X.Equals(-1f) && p8ScreenOrigin.Y.Equals(-1f))
                return;
            //[/block]
            //[block]
            p6.Z = headOrigin.Z;
            var p6ScreenOrigin = Functions.WorldToScreen(p6, GlobalRead.ViewMatrix);
            if (p6ScreenOrigin.X.Equals(-1f) && p6ScreenOrigin.Y.Equals(-1f))
                return;
            //[/block]
            //[/swap]

            //[swap]
            var p1ScreenOriginRaw = new RawVector2(CsX + p1ScreenOrigin.X * WindowScale, CsY + p1ScreenOrigin.Y * WindowScale);
            var p6ScreenOriginRaw = new RawVector2(CsX + p6ScreenOrigin.X * WindowScale, CsY + p6ScreenOrigin.Y * WindowScale);
            var p8ScreenOriginRaw = new RawVector2(CsX + p8ScreenOrigin.X * WindowScale, CsY + p8ScreenOrigin.Y * WindowScale);
            var p5ScreenOriginRaw = new RawVector2(CsX + p5ScreenOrigin.X * WindowScale, CsY + p5ScreenOrigin.Y * WindowScale);
            var p4ScreenOriginRaw = new RawVector2(CsX + p4ScreenOrigin.X * WindowScale, CsY + p4ScreenOrigin.Y * WindowScale);
            var p7ScreenOriginRaw = new RawVector2(CsX + p7ScreenOrigin.X * WindowScale, CsY + p7ScreenOrigin.Y * WindowScale);
            var p3ScreenOriginRaw = new RawVector2(CsX + p3ScreenOrigin.X * WindowScale, CsY + p3ScreenOrigin.Y * WindowScale);
            var p2ScreenOriginRaw = new RawVector2(CsX + p2ScreenOrigin.X * WindowScale, CsY + p2ScreenOrigin.Y * WindowScale);
            //[/swap]

            //[swap]
            G.Fade.Overlay.Device.DrawLine(p7ScreenOriginRaw, p8ScreenOriginRaw, _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawLine(p4ScreenOriginRaw, p8ScreenOriginRaw, _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawLine(p3ScreenOriginRaw, p4ScreenOriginRaw, _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawLine(p3ScreenOriginRaw, p7ScreenOriginRaw, _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawLine(p1ScreenOriginRaw, p5ScreenOriginRaw, _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawLine(p6ScreenOriginRaw, p7ScreenOriginRaw, _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawLine(p8ScreenOriginRaw, p5ScreenOriginRaw, _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawLine(p1ScreenOriginRaw, p2ScreenOriginRaw, _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawLine(p4ScreenOriginRaw, p1ScreenOriginRaw, _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawLine(p2ScreenOriginRaw, p3ScreenOriginRaw, _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawLine(p2ScreenOriginRaw, p6ScreenOriginRaw, _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawLine(p5ScreenOriginRaw, p6ScreenOriginRaw, _backgroundBrush, 2);
            //[/swap]

            //[swap]
            G.Fade.Overlay.Device.DrawLine(p5ScreenOriginRaw, p6ScreenOriginRaw, brush, 1);
            G.Fade.Overlay.Device.DrawLine(p3ScreenOriginRaw, p4ScreenOriginRaw, brush, 1);
            G.Fade.Overlay.Device.DrawLine(p6ScreenOriginRaw, p7ScreenOriginRaw, brush, 1);
            G.Fade.Overlay.Device.DrawLine(p1ScreenOriginRaw, p5ScreenOriginRaw, brush, 1);
            G.Fade.Overlay.Device.DrawLine(p7ScreenOriginRaw, p8ScreenOriginRaw, brush, 1);
            G.Fade.Overlay.Device.DrawLine(p4ScreenOriginRaw, p8ScreenOriginRaw, brush, 1);
            G.Fade.Overlay.Device.DrawLine(p4ScreenOriginRaw, p1ScreenOriginRaw, brush, 1);
            G.Fade.Overlay.Device.DrawLine(p1ScreenOriginRaw, p2ScreenOriginRaw, brush, 1);
            G.Fade.Overlay.Device.DrawLine(p2ScreenOriginRaw, p6ScreenOriginRaw, brush, 1);
            G.Fade.Overlay.Device.DrawLine(p8ScreenOriginRaw, p5ScreenOriginRaw, brush, 1);
            G.Fade.Overlay.Device.DrawLine(p3ScreenOriginRaw, p7ScreenOriginRaw, brush, 1);
            G.Fade.Overlay.Device.DrawLine(p2ScreenOriginRaw, p3ScreenOriginRaw, brush, 1);
            //[/swap]
        }

        private static void DoFloor(Player player)
        {
            if (!Floor)
                return;

            var isAllay = player.IsAllay && !FriendlyFire;

            var brush = isAllay ? new SolidColorBrush(G.Fade.Overlay.Device, AlliesColor) : new SolidColorBrush(G.Fade.Overlay.Device, EnemiesColor);

            if (VisibilityCheck && !isAllay && player.IsVisible)
                brush = new SolidColorBrush(G.Fade.Overlay.Device, EnemiesVisColor);

            var p0 = player.Origin;

            //[swap]
            const int size = 25;
            var p3 = p0;
            var p2 = p0;
            var p4 = p0;
            var p1 = p0;
            //[/swap]

            //[swap]
            p4.Y -= size;
            p4.X += size;
            p2.Y += size;
            p3.Y += size;
            p1.X -= size;
            p2.X -= size;
            p3.X += size;
            p1.Y -= size;
            //[/swap]

            //[swap]
            //[block]
            var p1ScreenOrigin = Functions.WorldToScreen(p1, GlobalRead.ViewMatrix);
            if (p1ScreenOrigin.X.Equals(-1f) && p1ScreenOrigin.Y.Equals(-1f))
                return;
            //[/block]
            //[block]
            var p4ScreenOrigin = Functions.WorldToScreen(p4, GlobalRead.ViewMatrix);
            if (p4ScreenOrigin.X.Equals(-1f) && p4ScreenOrigin.Y.Equals(-1f))
                return;
            //[/block]
            //[block]
            var p3ScreenOrigin = Functions.WorldToScreen(p3, GlobalRead.ViewMatrix);
            if (p3ScreenOrigin.X.Equals(-1f) && p3ScreenOrigin.Y.Equals(-1f))
                return;
            //[/block]
            //[block]
            var p2ScreenOrigin = Functions.WorldToScreen(p2, GlobalRead.ViewMatrix);
            if (p2ScreenOrigin.X.Equals(-1f) && p2ScreenOrigin.Y.Equals(-1f))
                return;
            //[/block]
            //[/swap]

            //[swap]
            var p1ScreenOriginRaw = new RawVector2(CsX + p1ScreenOrigin.X * WindowScale, CsY + p1ScreenOrigin.Y * WindowScale);
            var p4ScreenOriginRaw = new RawVector2(CsX + p4ScreenOrigin.X * WindowScale, CsY + p4ScreenOrigin.Y * WindowScale);
            var p3ScreenOriginRaw = new RawVector2(CsX + p3ScreenOrigin.X * WindowScale, CsY + p3ScreenOrigin.Y * WindowScale);
            var p2ScreenOriginRaw = new RawVector2(CsX + p2ScreenOrigin.X * WindowScale, CsY + p2ScreenOrigin.Y * WindowScale);
            //[/swap]

            //[swap]
            G.Fade.Overlay.Device.DrawLine(p2ScreenOriginRaw, p3ScreenOriginRaw, _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawLine(p3ScreenOriginRaw, p4ScreenOriginRaw, _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawLine(p4ScreenOriginRaw, p1ScreenOriginRaw, _backgroundBrush, 2);
            G.Fade.Overlay.Device.DrawLine(p1ScreenOriginRaw, p2ScreenOriginRaw, _backgroundBrush, 2);
            //[/swap]

            //[swap]
            G.Fade.Overlay.Device.DrawLine(p1ScreenOriginRaw, p2ScreenOriginRaw, brush, 1);
            G.Fade.Overlay.Device.DrawLine(p3ScreenOriginRaw, p4ScreenOriginRaw, brush, 1);
            G.Fade.Overlay.Device.DrawLine(p4ScreenOriginRaw, p1ScreenOriginRaw, brush, 1);
            G.Fade.Overlay.Device.DrawLine(p2ScreenOriginRaw, p3ScreenOriginRaw, brush, 1);
            //[/swap]
        }

        private static void DoSnaplines(Player player)
        {
            if (!Snaplines)
                return;

            var isAllay = player.IsAllay && !FriendlyFire;

            var brush = isAllay ? new SolidColorBrush(G.Fade.Overlay.Device, AlliesColor) : new SolidColorBrush(G.Fade.Overlay.Device, EnemiesColor);

            if (VisibilityCheck && !isAllay && player.IsVisible)
                brush = new SolidColorBrush(G.Fade.Overlay.Device, EnemiesVisColor);

            var screenOriginRaw = new RawVector2(CsX + player.ScreenOrigin.X * WindowScale, CsY + player.ScreenOrigin.Y * WindowScale);

            G.Fade.Overlay.Device.DrawLine(new RawVector2(ScreenCenter.X, CsY + CsHeight), screenOriginRaw, _backgroundBrush, 3);
            G.Fade.Overlay.Device.DrawLine(new RawVector2(ScreenCenter.X, CsY + CsHeight), screenOriginRaw, brush, 2);
        }

        private static void DoChams(Player player)
        {
            if (!PlayersChams)
                return;

            var isAllay = player.IsAllay && !FriendlyFire;

            var brush = isAllay ? ChamsAlliesColor : ChamsEnemiesColor;

            var chamsColor = new RenderColor((byte)(brush.R * 255), (byte)(brush.G * 255), (byte)(brush.B * 255), (byte)(brush.A * 255));
            player.RenderColor = chamsColor;
        }

        private static void DoHealth(Player player, DrawData drawData)
        {
            if (!HealthBar)
                return;

            //[swap]
            const int screenHealthWidth = 6;
            var screenHealthHeight = player.Health * (drawData.Height / 100);
            Brush brush = new SolidColorBrush(G.Fade.Overlay.Device, HealthBarColor);
            //[/swap]

            const int screenHealthMargin = screenHealthWidth / 2;

            // Draw Top-Background
            G.Fade.Overlay.Device.DrawLine(new RawVector2(drawData.X + drawData.Width + screenHealthMargin,
                    drawData.Y + (drawData.Height - screenHealthHeight) - .5f / 2f),
                new RawVector2(
                    drawData.X + drawData.Width + screenHealthMargin + screenHealthWidth,
                    drawData.Y + (drawData.Height - screenHealthHeight) - .5f / 2f),
                _backgroundBrush, .5f);

            // Draw HealthBar
            G.Fade.Overlay.Device.FillRectangle(
                new RawRectangleF(
                    drawData.X + drawData.Width + screenHealthMargin,
                    drawData.Y + (drawData.Height - screenHealthHeight),
                    drawData.X + drawData.Width + screenHealthMargin + screenHealthWidth,
                    drawData.Y + drawData.Height), brush);

            // Draw Background
            G.Fade.Overlay.Device.DrawRectangle(
                new RawRectangleF(
                    drawData.X + drawData.Width + screenHealthMargin,
                    drawData.Y,
                    drawData.X + drawData.Width + screenHealthMargin + screenHealthWidth,
                    drawData.Y + drawData.Height), _backgroundBrush, .5f);
        }

        private static void DoHeadHelper(Player player, PlayerModel model)
        {
            if (!HeadHelper)
                return;

            //[swap]
            Brush brush = new SolidColorBrush(G.Fade.Overlay.Device, HeadHelperColor);
            var screenHeadOrigin = model.ScreenBoneOrigin(Bone.Head);
            var size = HeadHelperSize * player.DistanceScale * WindowScale;
            //[/swap]

            var screenHeadOriginRaw = new RawVector2(CsX + screenHeadOrigin.X * WindowScale, CsY + screenHeadOrigin.Y * WindowScale);

            G.Fade.Overlay.Device.FillEllipse(
                new Ellipse(
                    screenHeadOriginRaw,
                    size + 1, size + 1), _backgroundBrush);

            G.Fade.Overlay.Device.FillEllipse(
                new Ellipse(
                    screenHeadOriginRaw,
                    size, size), brush);
        }

        private static void DoName(Player player, DrawData drawData)
        {
            if (!Name)
                return;

            //[swap]
            var size = 40 * player.DistanceScale * WindowScale;
            var brush = new SolidColorBrush(G.Fade.Overlay.Device, NamesColor);
            //[/swap]

            if (size < 10)
                size = 10;

            var textFormat = new TextFormat(new SharpDX.DirectWrite.Factory(), Settings.Default.Font, CFontWeight,
                FontStyle.Normal, size)
            {
                //[swap]
                TextAlignment = TextAlignment.Center,
                WordWrapping = WordWrapping.NoWrap,
                //[/swap]
            };

            //[swap]
            G.Fade.Overlay.Device.DrawText(player.Name, textFormat, new RawRectangleF(drawData.X + 1, drawData.Y - size * 1.25f - 1, drawData.X + drawData.Width, drawData.Y), _backgroundBrush);
            G.Fade.Overlay.Device.DrawText(player.Name, textFormat, new RawRectangleF(drawData.X - 1, drawData.Y - size * 1.25f + 1, drawData.X + drawData.Width, drawData.Y), _backgroundBrush);
            G.Fade.Overlay.Device.DrawText(player.Name, textFormat, new RawRectangleF(drawData.X - 1, drawData.Y - size * 1.25f - 1, drawData.X + drawData.Width, drawData.Y), _backgroundBrush);
            G.Fade.Overlay.Device.DrawText(player.Name, textFormat, new RawRectangleF(drawData.X + 1, drawData.Y - size * 1.25f + 1, drawData.X + drawData.Width, drawData.Y), _backgroundBrush);
            //[/swap]

            G.Fade.Overlay.Device.DrawText(player.Name, textFormat, new RawRectangleF(drawData.X, drawData.Y - size * 1.25f, drawData.X + drawData.Width, drawData.Y), brush);
        }

        private static void DoRank(Player player, DrawData drawData)
        {
            if (!Rank)
                return;

            //[swap]
            var scale = Name ? 2 : 1;
            var rankName = SimpleRanks ? player.Rank.GetSimpleRankName() : player.Rank.GetRankName();
            var margin = Name ? 1 : 1.25f;
            var brush = new SolidColorBrush(G.Fade.Overlay.Device, RanksColor);
            var size = 40 * player.DistanceScale * WindowScale;
            //[/swap]

            if (size < 10)
                size = 10;

            var textFormat = new TextFormat(new SharpDX.DirectWrite.Factory(), Settings.Default.Font, CFontWeight,
                FontStyle.Normal, size)
            {
                //[swap]
                WordWrapping = WordWrapping.NoWrap,
                TextAlignment = TextAlignment.Center,
                //[/swap]
            };

            //[swap]
            G.Fade.Overlay.Device.DrawText(rankName, textFormat, new RawRectangleF(drawData.X - 1, drawData.Y - size * 1.1f * margin * scale - 1, drawData.X + drawData.Width, drawData.Y), _backgroundBrush);
            G.Fade.Overlay.Device.DrawText(rankName, textFormat, new RawRectangleF(drawData.X + 1, drawData.Y - size * 1.1f * margin * scale + 1, drawData.X + drawData.Width, drawData.Y), _backgroundBrush);
            G.Fade.Overlay.Device.DrawText(rankName, textFormat, new RawRectangleF(drawData.X + 1, drawData.Y - size * 1.1f * margin * scale - 1, drawData.X + drawData.Width, drawData.Y), _backgroundBrush);
            G.Fade.Overlay.Device.DrawText(rankName, textFormat, new RawRectangleF(drawData.X - 1, drawData.Y - size * 1.1f * margin * scale + 1, drawData.X + drawData.Width, drawData.Y), _backgroundBrush);
            //[/swap]

            G.Fade.Overlay.Device.DrawText(rankName, textFormat, new RawRectangleF(drawData.X, drawData.Y - size * 1.1f * margin * scale, drawData.X + drawData.Width, drawData.Y), brush);
        }

        private static void DoActiveWeapon(Player player, DrawData drawData)
        {
            if (!ActiveWeapon)
                return;

            //[swap]
            var brush = new SolidColorBrush(G.Fade.Overlay.Device, ActiveWeaponColor);
            var weaponName = player.ActiveWeapon.GetWeaponName();
            var size = 40 * player.DistanceScale * WindowScale;
            //[/swap]

            if (size < 10)
                size = 10;

            var textFormat = new TextFormat(new SharpDX.DirectWrite.Factory(), Settings.Default.Font, CFontWeight,
                FontStyle.Normal, size)
            {
                //[swap]
                TextAlignment = TextAlignment.Center,
                WordWrapping = WordWrapping.NoWrap,
                //[/swap]
            };

            //[swap]
            G.Fade.Overlay.Device.DrawText(weaponName, textFormat, new RawRectangleF(drawData.X - 1, drawData.Y + drawData.Height - 1, drawData.X + drawData.Width, drawData.Y + drawData.Height + size), _backgroundBrush);
            G.Fade.Overlay.Device.DrawText(weaponName, textFormat, new RawRectangleF(drawData.X + 1, drawData.Y + drawData.Height + 1, drawData.X + drawData.Width, drawData.Y + drawData.Height + size), _backgroundBrush);
            G.Fade.Overlay.Device.DrawText(weaponName, textFormat, new RawRectangleF(drawData.X - 1, drawData.Y + drawData.Height + 1, drawData.X + drawData.Width, drawData.Y + drawData.Height + size), _backgroundBrush);
            G.Fade.Overlay.Device.DrawText(weaponName, textFormat, new RawRectangleF(drawData.X + 1, drawData.Y + drawData.Height - 1, drawData.X + drawData.Width, drawData.Y + drawData.Height + size), _backgroundBrush);
            //[/swap]

            G.Fade.Overlay.Device.DrawText(weaponName, textFormat, new RawRectangleF(drawData.X, drawData.Y + drawData.Height, drawData.X + drawData.Width, drawData.Y + drawData.Height + size), brush);
        }

        private static void DoAdditionalInfo(Player player, DrawData drawData)
        {
            if (!ExtendedInfo)
                return;

            var extendedInfo = string.Empty;

            if (player.IsDefusing)
                extendedInfo += "### Defusing ###" + Environment.NewLine;

            if (player.HasHostage)
                extendedInfo += "### Hostage ###" + Environment.NewLine;

            if (player.HasDefuser)
                extendedInfo += "Defuser";

            //[swap]
            var brush = new SolidColorBrush(G.Fade.Overlay.Device, ExtendedInfoColor);
            var scale = ActiveWeapon ? 1 : 0;
            var size = 40 * player.DistanceScale * WindowScale;
            //[/swap]

            if (size < 10)
                size = 10;

            var textFormat = new TextFormat(new SharpDX.DirectWrite.Factory(), Settings.Default.Font, CFontWeight,
                FontStyle.Normal, size)
            {
                //[swap]
                WordWrapping = WordWrapping.NoWrap,
                TextAlignment = TextAlignment.Center,
                //[/swap]
            };

            //[swap]
            G.Fade.Overlay.Device.DrawText(extendedInfo, textFormat, new RawRectangleF(drawData.X + 1, drawData.Y + drawData.Height + size * scale - 1, drawData.X + drawData.Width, drawData.Y + drawData.Height + size * 2), _backgroundBrush);
            G.Fade.Overlay.Device.DrawText(extendedInfo, textFormat, new RawRectangleF(drawData.X - 1, drawData.Y + drawData.Height + size * scale - 1, drawData.X + drawData.Width, drawData.Y + drawData.Height + size * 2), _backgroundBrush);
            G.Fade.Overlay.Device.DrawText(extendedInfo, textFormat, new RawRectangleF(drawData.X - 1, drawData.Y + drawData.Height + size * scale + 1, drawData.X + drawData.Width, drawData.Y + drawData.Height + size * 2), _backgroundBrush);
            G.Fade.Overlay.Device.DrawText(extendedInfo, textFormat, new RawRectangleF(drawData.X + 1, drawData.Y + drawData.Height + size * scale + 1, drawData.X + drawData.Width, drawData.Y + drawData.Height + size * 2), _backgroundBrush);
            //[/swap]

            G.Fade.Overlay.Device.DrawText(extendedInfo, textFormat, new RawRectangleF(drawData.X, drawData.Y + drawData.Height + size * scale, drawData.X + drawData.Width, drawData.Y + drawData.Height + size * 2), brush);
        }

        private static void DoRadar(Player player)
        {
            if (!Radar)
                return;

            var isAllay = player.IsAllay;

            if (isAllay)
                return;

            player.IsSpotted = true;
        }

        private static void DoSpectators()
        {
            if (!Spectators || G.Fade.LocalPlayer.Health == 0 || _spectatorsString == "Spectators:\n")
                return;

            var margin = 10 * WindowScale;

            //[swap]
            var width = CsWidth - margin;
            var brush = new SolidColorBrush(G.Fade.Overlay.Device, SpectatorsColor);
            var y = CsY + margin;
            var x = CsX;
            var size = 24 * WindowScale;
            var height = CsHeight;
            //[/swap]

            var textFormat = new TextFormat(new SharpDX.DirectWrite.Factory(), Settings.Default.Font, CFontWeight,
                FontStyle.Normal, size)
            {
                //[swap]
                TextAlignment = TextAlignment.Trailing,
                WordWrapping = WordWrapping.NoWrap,
                //[/swap]
            };

            //[swap]
            G.Fade.Overlay.Device.DrawText(_spectatorsString, textFormat, new RawRectangleF(x - 1, y - 1, width, height), _backgroundBrush);
            G.Fade.Overlay.Device.DrawText(_spectatorsString, textFormat, new RawRectangleF(x + 1, y - 1, width, height), _backgroundBrush);
            G.Fade.Overlay.Device.DrawText(_spectatorsString, textFormat, new RawRectangleF(x - 1, y + 1, width, height), _backgroundBrush);
            G.Fade.Overlay.Device.DrawText(_spectatorsString, textFormat, new RawRectangleF(x + 1, y + 1, width, height), _backgroundBrush);
            //[/swap]

            G.Fade.Overlay.Device.DrawText(_spectatorsString, textFormat, new RawRectangleF(x, y, width, height), brush);
        }

        private static bool ShouldSound()
        {
            if (_soundEspTarget.Distance.Equals(10000f))
                return false;

            return true;
        }
        private static void DoSound()
        {
            if (!SoundEsp || !ShouldSound())
                return;

            var percentage = SoundEspFovSize / (SoundEspFovSize - _soundEspTarget.Distance) * WindowScale;
            var length = 100 / percentage * SoundEspLengthScale;

            //[swap]
            var min = 25 * SoundEspLengthScale;
            var max = 100 * SoundEspLengthScale;
            //[/swap]

            if (length > max)
                length = max;
            else if (length < min)
                length = min;

            //[swap]
            //[block]
            _lastSoundEspTickCount = Environment.TickCount + (int)length;
            //[/block]
            //[block]
            var thr = new Thread(() =>
            {
                Console.Beep((int)SoundEspFrequency, (int)length);
            });
            thr.Start();
            //[/block]
            //[/swap]
        }

        private static bool ShouldBunnyhop()
        {
            if (!G.Fade.Keyboard.IsKeyPressed(Keys.Space))
                return false;

            if (G.Fade.LocalPlayer.Health == 0)
                return false;

            if (G.Fade.LocalPlayer.Flags == Flags.InAir)
                return false;

            if (G.Fade.LocalPlayer.Velocity == new Vector3())
                return false;

            return true;
        }
        private static void DoBunnyhop()
        {
            if (!Bunnyhop || !ShouldBunnyhop())
                return;

            G.Fade.LocalPlayer.Execute.Jump();
        }

        private static void DoAntiFlash()
        {
            if (!AntiFlash)
                return;

            if (G.Fade.LocalPlayer.FlashAlpha.Equals(0f))
                return;

            G.Fade.LocalPlayer.FlashAlpha = 0;
        }

        private static void DoRecoilCrosshair()
        {
            if (!RecoilCrosshair)
                return;

            var percentage = ScreenCenter.Y / 100 * WindowScale;
            var rcsPunchVec = G.Fade.LocalPlayer.GetPunch();

            //[swap]
            rcsPunchVec.Y *= 2; // horizontal
            rcsPunchVec.X *= 2; // vertical
            //[/swap]
            //[swap]
            var y = 0 - rcsPunchVec.X;
            var x = 0 - rcsPunchVec.Y;
            //[/swap]
            //[swap]
            x *= percentage;
            y *= G.Fade.LocalPlayer.ScopeScale;
            var brush = new SolidColorBrush(G.Fade.Overlay.Device, RecoilCrosshairColor);
            x *= G.Fade.LocalPlayer.ScopeScale;
            y *= percentage;
            //[/swap]

            var position = ScreenCenter;

            //[swap]
            position.Y -= y;
            position.X += x;
            //[/swap]

            G.Fade.Overlay.Device.DrawEllipse(new Ellipse(position, 5, 5), _backgroundBrush, 3);
            G.Fade.Overlay.Device.DrawEllipse(new Ellipse(position, 5, 5), brush, 2);
        }

        private static bool ShouldAimbot()
        {
            if (AimbotKey != Keys.F23 && !G.Fade.Keyboard.IsKeyPressed(AimbotKey))
                return false;

            if (G.Fade.LocalPlayer.Health == 0)
                return false;

            if (_aimbotTarget.Distance.Equals(10000f))
                return false;

            return true;
        }
        private static void DoAimbotRcs()
        {
            var doAimbot = 0;

            if (AimbotPistols && G.Fade.LocalPlayer.ActiveWeapon.IsPistol())
                doAimbot = 1;
            else if (AimbotSnipers && G.Fade.LocalPlayer.ActiveWeapon.IsSniper())
                doAimbot = 2;
            else if (Aimbot && !G.Fade.LocalPlayer.ActiveWeapon.IsPistol() && !G.Fade.LocalPlayer.ActiveWeapon.IsSniper())
                doAimbot = 3;

            if (doAimbot != 0 && ShouldAimbot())
            {
                //[swap]
                var targetAngles = Functions.CalcAngle(G.Fade.LocalPlayer.ViewOrigin, _aimbotTarget.TargetOrigin);
                var viewAngles = G.Fade.LocalPlayer.ViewAngles + _lastRcsVecPunch;
                //[/swap]

                var anglesChange = new Vector2(targetAngles.X - viewAngles.X, targetAngles.Y - viewAngles.Y);
                anglesChange = Functions.NormalizeAngles(anglesChange);

                switch (doAimbot)
                {
                    case 1:
                        //[swap]
                        anglesChange.Y *= AimbotPistolsSpeed;
                        anglesChange.X *= AimbotPistolsSpeed;
                        //[/swap]
                        break;
                    case 2:
                        //[swap]
                        anglesChange.Y *= AimbotSnipersSpeed;
                        anglesChange.X *= AimbotSnipersSpeed;
                        //[/swap]
                        break;
                    case 3:
                        //[swap]
                        anglesChange.Y *= AimbotSpeed;
                        anglesChange.X *= AimbotSpeed;
                        //[/swap]
                        break;
                }

                Vector2 newViewAngles;

                if (AimbotNonSticky && _isAimbotLocked)
                    newViewAngles = viewAngles;
                else
                    newViewAngles = viewAngles + anglesChange;

                if ((anglesChange.X * anglesChange.X < .25) && (anglesChange.Y * anglesChange.Y < .25))
                    _isAimbotLocked = true;

                if (Rcs && ShouldRcs())
                {
                    var rcsPunchVec = G.Fade.LocalPlayer.GetPunch();

                    //[swap]
                    rcsPunchVec.Y *= RcsStrengthX;
                    rcsPunchVec.X *= RcsStrengthY;
                    //[/swap]

                    // Block big changes
                    if ((rcsPunchVec.X - _lastRcsVecPunch.X > -1.5) &&
                        (rcsPunchVec.X - _lastRcsVecPunch.X < 1.5))
                    {
                        //[swap]
                        newViewAngles.X -= rcsPunchVec.X;
                        newViewAngles.Y -= rcsPunchVec.Y;
                        //[/swap]
                    }

                    _lastRcsVecPunch = rcsPunchVec;
                }
                else
                {
                    _lastRcsVecPunch = new Vector2();
                }

                G.Fade.LocalPlayer.ViewAngles = newViewAngles;

                if (doAimbot == 1)
                {
                    _lastAimbotTickCount = Environment.TickCount + (int)AimbotPistolsDelay;
                }
                else if (doAimbot == 2)
                {
                    _lastAimbotTickCount = Environment.TickCount + (int)AimbotSnipersDelay;
                }
                else if (doAimbot == 3)
                {
                    _lastAimbotTickCount = Environment.TickCount + (int)AimbotLockTime;
                }

                _lastAimbotPlayerHandle = _aimbotTarget.Player.Handle;
            }
            else
            {
                _isAimbotLocked = false;
                DoRcs();
            }
        }

        private static bool ShouldRcs(bool positionFix = true)
        {
            if (G.Fade.LocalPlayer.Health == 0)
                return false;

            if (G.Fade.LocalPlayer.Flags == Flags.InAir)
                return false;

            if (positionFix && !_lastRcsVecPunch.Equals(new Vector2()))
                return true;

            if (G.Fade.LocalPlayer.GetPunch(true) == new Vector2())
                return false;

            if (G.Fade.LocalPlayer.ActiveWeapon.IsRestrictedRcs())
                return false;

            if (RcsPistol)
            {
                if ((G.Fade.LocalPlayer.ShotsFired == 0) && !G.Fade.LocalPlayer.ActiveWeapon.IsPistol() && (!Triggerbot || !ShouldTriggerbot()))
                    return false;
            }
            else
            {
                if ((G.Fade.LocalPlayer.ShotsFired <= 1) && (!Triggerbot || !ShouldTriggerbot()))
                    return false;
            }

            return true;
        }
        private static void DoRcs()
        {
            if (!Rcs || RcsAimbotOnly || !ShouldRcs(RcsPositionFix))
            {
                _lastRcsVecPunch = new Vector2();
                return;
            }

            var vecPunch = G.Fade.LocalPlayer.GetPunch();

            //[swap]
            vecPunch.X *= RcsStrengthY;
            vecPunch.Y *= RcsStrengthX;
            //[/swap]

            // Block big changes
            if ((vecPunch.X - _lastRcsVecPunch.X > -1.5) && (vecPunch.X - _lastRcsVecPunch.X < 1.5))
            {
                var newViewAngles = G.Fade.LocalPlayer.ViewAngles;

                //[swap]
                newViewAngles.Y -= vecPunch.Y - _lastRcsVecPunch.Y;
                newViewAngles.X -= vecPunch.X - _lastRcsVecPunch.X;
                //[/swap]

                G.Fade.LocalPlayer.ViewAngles = newViewAngles;
            }

            _lastRcsVecPunch = vecPunch;
        }

        private static int ShouldKnifebot()
        {
            if (G.Fade.LocalPlayer.Health == 0)
                return 0;

            if (G.Fade.LocalPlayer.CrosshairPlayer.Origin.Equals(new Vector3()))
                return 0;

            if (!G.Fade.LocalPlayer.ActiveWeapon.IsKnife())
                return 0;

            var triggerbotPlayer = G.Fade.LocalPlayer.CrosshairPlayer;

            if (triggerbotPlayer.HasImmunity)
                return 0;

            var distance = Functions.VectorDistance(G.Fade.LocalPlayer.Origin, triggerbotPlayer.Origin);

            if (distance <= 64)
            {
                if (triggerbotPlayer.Health <= 21)
                    return !triggerbotPlayer.IsAllay || FriendlyFire ? 1 : 0;
                if (triggerbotPlayer.Health <= 55)
                    return !triggerbotPlayer.IsAllay || FriendlyFire ? 2 : 0;

                return !triggerbotPlayer.IsAllay || FriendlyFire ? 1 : 0;
            }

            if (distance <= 82)
                return !triggerbotPlayer.IsAllay || FriendlyFire ? 1 : 0;

            return 0;
        }
        private static void DoKnifebot()
        {
            if (!Knifebot)
                return;

            switch (ShouldKnifebot())
            {
                //[swap]
                //[block]
                case 1:
                    G.Fade.LocalPlayer.Execute.Attack1Async();
                    break;
                //[/block]
                //[block]
                case 2:
                    G.Fade.LocalPlayer.Execute.Attack2Async();
                    break;
                //[/block]
                //[/swap]
                default:
                    return;
            }
        }

        private static bool ShouldTriggerbot()
        {
            if (TriggerbotKey != Keys.F23 && !G.Fade.Keyboard.IsKeyPressed(TriggerbotKey))
                return false;

            if (G.Fade.LocalPlayer.Health == 0)
                return false;

            if (G.Fade.LocalPlayer.ActiveWeapon.IsKnife())
                return false;

            if (G.Fade.LocalPlayer.CrosshairPlayer.Origin.Equals(new Vector3()))
                return false;

            var triggerbotPlayer = G.Fade.LocalPlayer.CrosshairPlayer;

            if (triggerbotPlayer.HasImmunity)
                return false;

            return !triggerbotPlayer.IsAllay || FriendlyFire;
        }
        private static void DoTriggerbot()
        {
            var doTriggetbot = false;

            if (TriggerbotPistols && G.Fade.LocalPlayer.ActiveWeapon.IsPistol())
                doTriggetbot = true;
            else if (TriggerbotSnipers && G.Fade.LocalPlayer.ActiveWeapon.IsSniper())
                doTriggetbot = true;
            else if (Triggerbot && !G.Fade.LocalPlayer.ActiveWeapon.IsPistol() && !G.Fade.LocalPlayer.ActiveWeapon.IsSniper())
                doTriggetbot = true;

            if (!doTriggetbot || !ShouldTriggerbot())
                return;

            var delay = TriggerbotDelay;

            if (G.Fade.LocalPlayer.ActiveWeapon.IsPistol())
                delay = TriggerbotPistolsDelay;
            else if (G.Fade.LocalPlayer.ActiveWeapon.IsSniper())
                delay = TriggerbotSnipersDelay;

            Thread thr = new Thread(() =>
            {
                Thread.Sleep((int) delay);
                G.Fade.LocalPlayer.Execute.Attack1();
            })
            {
                IsBackground = true
            };
            thr.Start();
        }

        private static bool ShouldAutoPistol()
        {
            if (!G.Fade.Keyboard.IsKeyPressed(Keys.LButton))
                return false;

            if (G.Fade.LocalPlayer.Health == 0)
                return false;

            if (!G.Fade.LocalPlayer.ActiveWeapon.IsPistol())
                return false;

            return true;

        }
        private static void DoAutoPistol()
        {
            if (!AutoPistol || !ShouldAutoPistol())
                return;

            G.Fade.LocalPlayer.Execute.Attack1Async();
        }

        private static void DoForceFullUpdate()
        {
            if (G.Fade.Keyboard.IsKeyPressed(ForceUpdateKey))
                G.Fade.LocalPlayer.ForceFullUpdate();
        }

        private static void DoStatTrak()
        {
            if (!StatTrak)
                return;

            if (_lastKills < G.Fade.LocalPlayer.Kills)
            {
                var id = G.Fade.LocalPlayer.ActiveWeapon;
                var skin = G.Skins.Find(s => s.WeaponID == id);

                if (skin.StatTrak > -1)
                {
                    skin.StatTrak++;
                    G.SkinConfig.Set("StatTrak", id.GetWeaponName(), skin.StatTrak.ToString());
                }
            }

            _lastKills = G.Fade.LocalPlayer.Kills;
        }

        private static void DoSkinChanger()
        {
            if (!SkinChanger)
                return;

            var weapons = G.Fade.LocalPlayer.MyWeapons;

            foreach (var weapon in weapons)
            {
                var id = weapon.ID;
                if (!id.HasSkin())
                    continue;

                var skin = G.Skins.Find(s => s.WeaponID == id);
                var myXuid = weapon.Xuid;

                weapon.ForceSkin();

                if (skin.ID != 0)
                    weapon.PaintKit = skin.ID;

                if (skin.Name != string.Empty)
                    weapon.Name = skin.Name;

                //[swap]
                weapon.Wear = skin.Wear.Equals(0f) ? 0.00000001f : skin.Wear;
                weapon.Account = myXuid;
                weapon.EntityQuality = skin.Prefix;
                weapon.StatTrak = skin.StatTrak;
                //[/swap]
            }
        }

        private static void DoFovChanger()
        {
            if (!FovChanger)
                return;

            if (G.Fade.LocalPlayer.Fov != (int)FovChangerValue)
                G.Fade.LocalPlayer.Fov = (int)FovChangerValue;
        }
    }
}