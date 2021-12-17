using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Forms;
using System.Windows.Interop;
using DDX.Enums;
using DDX.Extensions;
using ComboBox = System.Windows.Controls.ComboBox;
using ComboBoxItem = DDX.Structs.ComboBoxItem;
using MessageBox = System.Windows.MessageBox;
using NumericUpDown = MahApps.Metro.Controls.NumericUpDown;
using TextBox = System.Windows.Controls.TextBox;

namespace DDX.Views
{
    /// <summary>
    /// Interaction logic for MenuWindow.xaml
    /// </summary>
    public partial class MenuWindow
    {
        private readonly double _initialLeft;
        private readonly double _initialTop;
        private readonly double _initialScale;

        private bool _disableEvents;

        public MenuWindow()
        {
            _disableEvents = true;

            InitializeComponent();
            InitializeComboBoxes();
            InitializeEvents();

            // Store few initial values for proper window resizing
            _initialLeft = Left;
            _initialTop = Top;
            _initialScale = Width / Height;

            SizeChanged += OnSizeChanged;
            Loaded += OnLoad;
        }

        private void OnSizeChanged(object sender, SizeChangedEventArgs sizeChangedEventArgs)
        {
            Left = _initialLeft;
            Top = _initialTop;
            Height = Width / _initialScale;
        }
        private void OnLoad(object sender, RoutedEventArgs e)
        {
            // Hide from alt-tab
            var wndHelper = new WindowInteropHelper(this);
            var exStyle = WinApi.GetWindowLong(wndHelper.Handle, -20);
            WinApi.SetWindowLong(wndHelper.Handle, -20, exStyle | 0x80);

            ConfigToControls();

            _disableEvents = false;
        }

        private void InitializeComboBoxes()
        {
            var tmp = new List<ComboBox>();

            tmp.Add(DefaultAimbotKey);
            tmp.Add(DefaultSilentAimKey);
            tmp.Add(DefaultTriggerbotWithAimbotKey);
            tmp.Add(DefaultTriggerbotKey);
            tmp.Add(PistolsAimbotKey);
            tmp.Add(PistolsSilentAimKey);
            tmp.Add(PistolsTriggerbotWithAimbotKey);
            tmp.Add(PistolsTriggerbotKey);
            tmp.Add(SnipersAimbotKey);
            tmp.Add(SnipersSilentAimKey);
            tmp.Add(SnipersTriggerbotWithAimbotKey);
            tmp.Add(SnipersTriggerbotKey);
            foreach (var box in tmp)
            {
                box.Items.Add(new ComboBoxItem("Disabled", Keys.F24));
                box.Items.Add(new ComboBoxItem("AlwaysOn", Keys.F23));
                box.Items.Add(new ComboBoxItem("LButton" , Keys.LButton));
                box.Items.Add(new ComboBoxItem("MButton" , Keys.MButton));
                box.Items.Add(new ComboBoxItem("RButton" , Keys.RButton));
                box.Items.Add(new ComboBoxItem("XButton1", Keys.XButton1));
                box.Items.Add(new ComboBoxItem("XButton2", Keys.XButton2));
                box.Items.Add(new ComboBoxItem("E"       , Keys.E));
                box.Items.Add(new ComboBoxItem("C"       , Keys.C));
                box.Items.Add(new ComboBoxItem("V"       , Keys.V));
                box.Items.Add(new ComboBoxItem("Alt"     , Keys.Menu));
            }

            tmp.Clear();
            tmp.Add(DefaultAimbotBone);
            tmp.Add(DefaultSilentAimBone);
            tmp.Add(PistolsAimbotBone);
            tmp.Add(PistolsSilentAimBone);
            tmp.Add(SnipersAimbotBone);
            tmp.Add(SnipersSilentAimBone);
            foreach (var box in tmp)
            {
                box.Items.Add(new ComboBoxItem("Dynamic"  , Bone.Dynamic));
                box.Items.Add(new ComboBoxItem("Stomach"  , Bone.Stomach));
                box.Items.Add(new ComboBoxItem("Chest"    , Bone.Chest));
                box.Items.Add(new ComboBoxItem("Neck"     , Bone.Neck));
                box.Items.Add(new ComboBoxItem("Head"     , Bone.Head));
                box.Items.Add(new ComboBoxItem("Right Arm", Bone.RightArm));
                box.Items.Add(new ComboBoxItem("Left Arm" , Bone.LeftArm));
                box.Items.Add(new ComboBoxItem("Right Leg", Bone.RightLeg));
                box.Items.Add(new ComboBoxItem("Left Leg" , Bone.LeftLeg));
            }

            tmp.Clear();
            tmp.Add(ExitKey);
            tmp.Add(ToggleKey);
            foreach (var box in tmp)
            {
                box.Items.Add(new ComboBoxItem("Insert"  , Keys.Insert));
                box.Items.Add(new ComboBoxItem("Home"    , Keys.Home));
                box.Items.Add(new ComboBoxItem("Delete"  , Keys.Delete));
                box.Items.Add(new ComboBoxItem("End"     , Keys.End));
                box.Items.Add(new ComboBoxItem("PageUp"  , Keys.PageUp));
                box.Items.Add(new ComboBoxItem("PageDown", Keys.PageDown));
                box.Items.Add(new ComboBoxItem("F9"      , Keys.F9));
                box.Items.Add(new ComboBoxItem("F10"     , Keys.F10));
                box.Items.Add(new ComboBoxItem("F11"     , Keys.F11));
                box.Items.Add(new ComboBoxItem("F12"     , Keys.F12));
            }

            tmp.Clear();
            tmp.Add(BunnyhopKey);
            foreach (var box in tmp)
            {
                box.Items.Add(new ComboBoxItem("Space"   , Keys.Space));
                box.Items.Add(new ComboBoxItem("MButton" , Keys.MButton));
                box.Items.Add(new ComboBoxItem("XButton1", Keys.XButton1));
                box.Items.Add(new ComboBoxItem("XButton2", Keys.XButton2));
                box.Items.Add(new ComboBoxItem("Alt"     , Keys.Menu));
            }

            tmp.Clear();
            tmp.Add(ForceUpdateKey);
            foreach (var box in tmp)
            {
                box.Items.Add(new ComboBoxItem("F1", Keys.F1));
                box.Items.Add(new ComboBoxItem("F2", Keys.F2));
                box.Items.Add(new ComboBoxItem("F3", Keys.F3));
                box.Items.Add(new ComboBoxItem("F4", Keys.F4));
                box.Items.Add(new ComboBoxItem("V" , Keys.V));
            }
        }
        private void InitializeEvents()
        {
            var tmp = new List<Grid>();
            tmp.Add(SettingsGrid);
            tmp.Add(DefaultAimGrid);
            tmp.Add(KeysGrid);
            tmp.Add(PistolsAimGrid);
            tmp.Add(VisualsGrid);
            tmp.Add(SnipersAimGrid);
            tmp.Add(MiscGrid);
            tmp.Add(PlayersGrid);
            tmp.Add(EntitiesGrid);

            // Add event function to every control
            foreach (var grid in tmp)
            {
                foreach (var child in grid.Children)
                {
                    if (child is System.Windows.Controls.CheckBox chk)
                    {
                        chk.Click += Control_OnChange;
                    }
                    else if(child is Slider sli)
                    {
                        sli.ValueChanged += Control_OnChange;
                    }
                    else if(child is ComboBox com)
                    {
                        com.SelectionChanged += Control_OnChange;
                    }
                    else if (child is TextBox txt)
                    {
                        txt.TextChanged += Control_OnChange;
                    }
                    else if (child is NumericUpDown num)
                    {
                        num.ValueChanged += Control_OnChange;
                    }
                }
            }
        }

        private void Control_OnChange(object sender, RoutedEventArgs routedEventArgs)
        {
            // Values are being loaded.
            if (_disableEvents)
            {
                return;
            }

            ControlsToConfig();
            ConfigParser.Save();
        }

        private void ConfigToControls()
        {
            AimFov.IsChecked = G.C.Vis.AimFov;
            AimPoint.IsChecked = G.C.Vis.AimPoint;
            Antialiasing.IsChecked = G.C.Stts.Antialiasing;
            AutoDuck.IsChecked = G.C.Misc.AutoDuck;
            AutoPistol.IsChecked = G.C.Misc.AutoPistol;
            AutoStop.IsChecked = G.C.Misc.AutoStop;
            BombDamage.IsChecked = G.C.Misc.BombDamage;
            Brightness.IsChecked = G.C.Misc.Brightness;
            BrightnessStrength.Value = G.C.Misc.BrightnessProp.Strength;
            Bunnyhop.IsChecked = G.C.Misc.Bunnyhop;
            BunnyhopAutoStrafe.IsChecked = G.C.Misc.BunnyhopProp.AutoStrafe;
            BunnyhopKey.SelectedIndex = GetComboBoxIndexByKey(BunnyhopKey, G.C.Key.Bunnyhop);
            ChatInfo.IsChecked = G.C.Misc.ChatInfo;
            ChatInfoSendAll.IsChecked = G.C.Misc.ChatInfoProp.SendAll;
            DefaultAimbot.IsChecked = G.C.Aim.Default.Aimbot;
            DefaultAimbotBone.SelectedIndex = GetComboBoxIndexByBone(G.C.Aim.Default.AimbotProp.Bone);
            DefaultAimbotFov.Value = G.C.Aim.Default.AimbotProp.Fov;
            DefaultAimbotKey.SelectedIndex = GetComboBoxIndexByKey(DefaultAimbotKey, G.C.Aim.Default.AimbotProp.Key);
            DefaultAimbotLock.IsChecked = G.C.Aim.Default.AimbotProp.Lock;
            DefaultAimbotLockLength.Value = G.C.Aim.Default.AimbotProp.LockProp.Length;
            DefaultAimbotOnGroundOnly.IsChecked = G.C.Aim.Default.AimbotProp.OnGroundOnly;
            DefaultAimbotSmooth.Value = G.C.Aim.Default.AimbotProp.Smooth;
            DefaultAimbotVisibilityCheck.IsChecked = G.C.Aim.Default.AimbotProp.VisibilityCheck;
            DefaultAimbotVisibilityCheckBsp.IsChecked = G.C.Aim.Default.AimbotProp.VisibilityCheckProp.BspParsing;
            DefaultRcs.IsChecked = G.C.Aim.Default.Rcs;
            DefaultRcsAimbotOnly.IsChecked = G.C.Aim.Default.RcsProp.AimbotOnly;
            DefaultRcsPitchStrength.Value = G.C.Aim.Default.RcsProp.PitchStrength;
            DefaultRcsSmooth.Value = G.C.Aim.Default.RcsProp.Smooth;
            DefaultRcsYawStrength.Value = G.C.Aim.Default.RcsProp.YawStrength;
            DefaultSilentAim.IsChecked = G.C.Aim.Default.SilentAim;
            DefaultSilentAimBone.SelectedIndex = GetComboBoxIndexByBone(G.C.Aim.Default.SilentAimProp.Bone);
            DefaultSilentAimFov.Value = G.C.Aim.Default.SilentAimProp.Fov;
            DefaultSilentAimKey.SelectedIndex = GetComboBoxIndexByKey(DefaultSilentAimKey, G.C.Aim.Default.SilentAimProp.Key);
            DefaultSilentAimOnGroundOnly.IsChecked = G.C.Aim.Default.SilentAimProp.OnGroundOnly;
            DefaultSilentAimVisibilityCheck.IsChecked = G.C.Aim.Default.SilentAimProp.VisibilityCheck;
            DefaultSilentAimVisibilityCheckBsp.IsChecked = G.C.Aim.Default.SilentAimProp.VisibilityCheckProp.BspParsing;
            DefaultTriggerbot.IsChecked = G.C.Aim.Default.Triggerbot;
            DefaultTriggerbotAfterDelay.Value = G.C.Aim.Default.TriggerbotProp.AfterDelay;
            DefaultTriggerbotBeforeDelay.Value = G.C.Aim.Default.TriggerbotProp.BeforeDelay;
            DefaultTriggerbotKey.SelectedIndex = GetComboBoxIndexByKey(DefaultTriggerbotKey, G.C.Aim.Default.TriggerbotProp.Key);
            DefaultTriggerbotShootDelay.Value = G.C.Aim.Default.TriggerbotProp.ShootDelay;
            DefaultTriggerbotWithAimbotKey.SelectedIndex = GetComboBoxIndexByKey(DefaultTriggerbotWithAimbotKey, G.C.Aim.Default.TriggerbotWithAimbotKey);
            EntitiesAdditionalInfo.IsChecked = G.C.Vis.Entities.AdditionalInfo;
            EntitiesChams.IsChecked = G.C.Vis.Entities.Chams;
            EntitiesEsp2D.IsChecked = G.C.Vis.Entities.Esp2D;
            EntitiesEsp3D.IsChecked = G.C.Vis.Entities.Esp3D;
            EntitiesGlow.IsChecked = G.C.Vis.Entities.Glow;
            EntitiesGlowFill.IsChecked = G.C.Vis.Entities.GlowProp.Fill;
            EntitiesGlowFillTransparent.IsChecked = G.C.Vis.Entities.GlowProp.FillTransparent;
            EntitiesGlowInner.IsChecked = G.C.Vis.Entities.GlowProp.Inner;
            EntitiesImportantOnly.IsChecked = G.C.Vis.Entities.ImportantOnly;
            EntitiesLocalChams.IsChecked = G.C.Vis.Entities.LocalChams;
            EntitiesName.IsChecked = G.C.Vis.Entities.Name;
            EntitiesSnaplines.IsChecked = G.C.Vis.Entities.Snaplines;
            ExitKey.SelectedIndex = GetComboBoxIndexByKey(ExitKey, G.C.Key.Exit);
            FakeLag.IsChecked = G.C.Misc.FakeLag;
            FakeLagLength.Value = G.C.Misc.FakeLagProp.Length;
            FakeLagPause.Value = G.C.Misc.FakeLagProp.Pause;
            FastReload.IsChecked = G.C.Misc.FastReload;
            FastSnipers.IsChecked = G.C.Misc.FastSnipers;
            FirendlyFire.IsChecked = G.C.Stts.FriendlyFire;
            ForceUpdateKey.SelectedIndex = GetComboBoxIndexByKey(ForceUpdateKey, G.C.Key.ForceUpdate);
            FovChanger.IsChecked = G.C.Misc.FovChanger;
            FovChangerValue.Value = G.C.Misc.FovChangerProp.Value;
            HideAllies.IsChecked = G.C.Vis.HideAllies;
            HideChickens.IsChecked = G.C.Vis.HideChickens;
            HideEnemies.IsChecked = G.C.Vis.HideEnemies;
            Knifebot.IsChecked = G.C.Misc.Knifebot;
            MoveHelper.IsChecked = G.C.Misc.MoveHelper;
            NoFlash.IsChecked = G.C.Misc.NoFlash;
            NoHands.IsChecked = G.C.Misc.NoHands;
            NoShake.IsChecked = G.C.Misc.NoShake;
            NoSmoke.IsChecked = G.C.Misc.NoSmoke;
            NoViewModel.IsChecked = G.C.Misc.NoViewModel;
            PistolsAimbot.IsChecked = G.C.Aim.Pistols.Aimbot;
            PistolsAimbotBone.SelectedIndex = GetComboBoxIndexByBone(G.C.Aim.Pistols.AimbotProp.Bone);
            PistolsAimbotFov.Value = G.C.Aim.Pistols.AimbotProp.Fov;
            PistolsAimbotKey.SelectedIndex = GetComboBoxIndexByKey(PistolsAimbotKey, G.C.Aim.Pistols.AimbotProp.Key);
            PistolsAimbotLock.IsChecked = G.C.Aim.Pistols.AimbotProp.Lock;
            PistolsAimbotLockLength.Value = G.C.Aim.Pistols.AimbotProp.LockProp.Length;
            PistolsAimbotOnGroundOnly.IsChecked = G.C.Aim.Pistols.AimbotProp.OnGroundOnly;
            PistolsAimbotSmooth.Value = G.C.Aim.Pistols.AimbotProp.Smooth;
            PistolsAimbotVisibilityCheck.IsChecked = G.C.Aim.Pistols.AimbotProp.VisibilityCheck;
            PistolsAimbotVisibilityCheckBsp.IsChecked = G.C.Aim.Pistols.AimbotProp.VisibilityCheckProp.BspParsing;
            PistolsRcs.IsChecked = G.C.Aim.Pistols.Rcs;
            PistolsRcsAimbotOnly.IsChecked = G.C.Aim.Pistols.RcsProp.AimbotOnly;
            PistolsRcsPitchStrength.Value = G.C.Aim.Pistols.RcsProp.PitchStrength;
            PistolsRcsSmooth.Value = G.C.Aim.Pistols.RcsProp.Smooth;
            PistolsRcsYawStrength.Value = G.C.Aim.Pistols.RcsProp.YawStrength;
            PistolsSilentAim.IsChecked = G.C.Aim.Pistols.SilentAim;
            PistolsSilentAimBone.SelectedIndex = GetComboBoxIndexByBone(G.C.Aim.Pistols.SilentAimProp.Bone);
            PistolsSilentAimFov.Value = G.C.Aim.Pistols.SilentAimProp.Fov;
            PistolsSilentAimKey.SelectedIndex = GetComboBoxIndexByKey(PistolsSilentAimKey, G.C.Aim.Pistols.SilentAimProp.Key);
            PistolsSilentAimOnGroundOnly.IsChecked = G.C.Aim.Pistols.SilentAimProp.OnGroundOnly;
            PistolsSilentAimVisibilityCheck.IsChecked = G.C.Aim.Pistols.SilentAimProp.VisibilityCheck;
            PistolsSilentAimVisibilityCheckBsp.IsChecked = G.C.Aim.Pistols.SilentAimProp.VisibilityCheckProp.BspParsing;
            PistolsTriggerbot.IsChecked = G.C.Aim.Pistols.Triggerbot;
            PistolsTriggerbotAfterDelay.Value = G.C.Aim.Pistols.TriggerbotProp.AfterDelay;
            PistolsTriggerbotBeforeDelay.Value = G.C.Aim.Pistols.TriggerbotProp.BeforeDelay;
            PistolsTriggerbotKey.SelectedIndex = GetComboBoxIndexByKey(PistolsTriggerbotKey, G.C.Aim.Pistols.TriggerbotProp.Key);
            PistolsTriggerbotShootDelay.Value = G.C.Aim.Pistols.TriggerbotProp.ShootDelay;
            PistolsTriggerbotWithAimbotKey.SelectedIndex = GetComboBoxIndexByKey(PistolsTriggerbotWithAimbotKey, G.C.Aim.Pistols.TriggerbotWithAimbotKey);
            PlayersActiveWeapon.IsChecked = G.C.Vis.Players.ActiveWeapon;
            PlayersActiveWeaponAmmo.IsChecked = G.C.Vis.Players.ActiveWeaponProp.Ammo;
            PlayersAdditionalInfo.IsChecked = G.C.Vis.Players.AdditionalInfo;
            PlayersArmor.IsChecked = G.C.Vis.Players.Armor;
            PlayersChams.IsChecked = G.C.Vis.Players.Chams;
            PlayersChamsHealth.IsChecked = G.C.Vis.Players.ChamsProp.Health;
            PlayersEsp2D.IsChecked = G.C.Vis.Players.Esp2D;
            PlayersEsp3D.IsChecked = G.C.Vis.Players.Esp3D;
            PlayersFloor.IsChecked = G.C.Vis.Players.Floor;
            PlayersGlow.IsChecked = G.C.Vis.Players.Glow;
            PlayersGlowFill.IsChecked = G.C.Vis.Players.GlowProp.Fill;
            PlayersGlowFillTransparent.IsChecked = G.C.Vis.Players.GlowProp.FillTransparent;
            PlayersGlowHealth.IsChecked = G.C.Vis.Players.GlowProp.Health;
            PlayersGlowInner.IsChecked = G.C.Vis.Players.GlowProp.Inner;
            PlayersHealth.IsChecked = G.C.Vis.Players.Health;
            PlayersName.IsChecked = G.C.Vis.Players.Name;
            PlayersRank.IsChecked = G.C.Vis.Players.Rank;
            PlayersRankSimple.IsChecked = G.C.Vis.Players.RankProp.Simple;
            PlayersRotationArrow.IsChecked = G.C.Vis.Players.RotationArrow;
            PlayersSkeleton.IsChecked = G.C.Vis.Players.Skeleton;
            PlayersSkeletonFill.IsChecked = G.C.Vis.Players.SkeletonProp.Fill;
            PlayersSnaplines.IsChecked = G.C.Vis.Players.Snaplines;
            PlayersVisibilityCheck.IsChecked = G.C.Vis.Players.VisibilityCheck;
            Radar.IsChecked = G.C.Misc.Radar;
            RecoilCrosshair.IsChecked = G.C.Vis.RecoilCrosshair;
            SilentAimFov.IsChecked = G.C.Vis.SilentAimFov;
            SnipersAimbot.IsChecked = G.C.Aim.Snipers.Aimbot;
            SnipersAimbotBone.SelectedIndex = GetComboBoxIndexByBone(G.C.Aim.Snipers.AimbotProp.Bone);
            SnipersAimbotFov.Value = G.C.Aim.Snipers.AimbotProp.Fov;
            SnipersAimbotKey.SelectedIndex = GetComboBoxIndexByKey(SnipersAimbotKey, G.C.Aim.Snipers.AimbotProp.Key);
            SnipersAimbotLock.IsChecked = G.C.Aim.Snipers.AimbotProp.Lock;
            SnipersAimbotLockLength.Value = G.C.Aim.Snipers.AimbotProp.LockProp.Length;
            SnipersAimbotOnGroundOnly.IsChecked = G.C.Aim.Snipers.AimbotProp.OnGroundOnly;
            SnipersAimbotSmooth.Value = G.C.Aim.Snipers.AimbotProp.Smooth;
            SnipersAimbotVisibilityCheck.IsChecked = G.C.Aim.Snipers.AimbotProp.VisibilityCheck;
            SnipersAimbotVisibilityCheckBsp.IsChecked = G.C.Aim.Snipers.AimbotProp.VisibilityCheckProp.BspParsing;
            SnipersRcs.IsChecked = G.C.Aim.Snipers.Rcs;
            SnipersRcsAimbotOnly.IsChecked = G.C.Aim.Snipers.RcsProp.AimbotOnly;
            SnipersRcsPitchStrength.Value = G.C.Aim.Snipers.RcsProp.PitchStrength;
            SnipersRcsSmooth.Value = G.C.Aim.Snipers.RcsProp.Smooth;
            SnipersRcsYawStrength.Value = G.C.Aim.Snipers.RcsProp.YawStrength;
            SnipersSilentAim.IsChecked = G.C.Aim.Snipers.SilentAim;
            SnipersSilentAimBone.SelectedIndex = GetComboBoxIndexByBone(G.C.Aim.Snipers.SilentAimProp.Bone);
            SnipersSilentAimFov.Value = G.C.Aim.Snipers.SilentAimProp.Fov;
            SnipersSilentAimKey.SelectedIndex = GetComboBoxIndexByKey(SnipersSilentAimKey, G.C.Aim.Snipers.SilentAimProp.Key);
            SnipersSilentAimOnGroundOnly.IsChecked = G.C.Aim.Snipers.SilentAimProp.OnGroundOnly;
            SnipersSilentAimVisibilityCheck.IsChecked = G.C.Aim.Snipers.SilentAimProp.VisibilityCheck;
            SnipersSilentAimVisibilityCheckBsp.IsChecked = G.C.Aim.Snipers.SilentAimProp.VisibilityCheckProp.BspParsing;
            SnipersTriggerbot.IsChecked = G.C.Aim.Snipers.Triggerbot;
            SnipersTriggerbotAfterDelay.Value = G.C.Aim.Snipers.TriggerbotProp.AfterDelay;
            SnipersTriggerbotBeforeDelay.Value = G.C.Aim.Snipers.TriggerbotProp.BeforeDelay;
            SnipersTriggerbotKey.SelectedIndex = GetComboBoxIndexByKey(SnipersTriggerbotKey, G.C.Aim.Snipers.TriggerbotProp.Key);
            SnipersTriggerbotShootDelay.Value = G.C.Aim.Snipers.TriggerbotProp.ShootDelay;
            SnipersTriggerbotWithAimbotKey.SelectedIndex = GetComboBoxIndexByKey(SnipersTriggerbotWithAimbotKey, G.C.Aim.Snipers.TriggerbotWithAimbotKey);
            Spam.IsChecked = G.C.Misc.Spam;
            SpamMessage.Text = G.C.Misc.SpamProp.Message;
            SpamSendAll.IsChecked = G.C.Misc.SpamProp.SendAll;
            SpectatorList.IsChecked = G.C.Misc.SpectatorList;
            ThirdPersonMode.IsChecked = G.C.Misc.ThirdPersonMode;
            ToggleKey.SelectedIndex = GetComboBoxIndexByKey(ToggleKey, G.C.Key.Toggle);
            Width = G.C.Stts.WindowWidth;
        }
        private void ControlsToConfig()
        {
            G.C.Aim.Default.Aimbot = DefaultAimbot.IsChecked == true;
            G.C.Aim.Default.AimbotProp.Bone = (Bone)(DefaultAimbotBone.SelectedItem as ComboBoxItem).Value;
            G.C.Aim.Default.AimbotProp.Fov = (int)DefaultAimbotFov.Value;
            G.C.Aim.Default.AimbotProp.Key = (Keys)(DefaultAimbotKey.SelectedItem as ComboBoxItem).Value;
            G.C.Aim.Default.AimbotProp.Lock = DefaultAimbotLock.IsChecked == true;
            G.C.Aim.Default.AimbotProp.LockProp.Length = (int)DefaultAimbotLockLength.Value;
            G.C.Aim.Default.AimbotProp.OnGroundOnly = DefaultAimbotOnGroundOnly.IsChecked == true;
            G.C.Aim.Default.AimbotProp.Smooth = (int)DefaultAimbotSmooth.Value;
            G.C.Aim.Default.AimbotProp.VisibilityCheck = DefaultAimbotVisibilityCheck.IsChecked == true;
            G.C.Aim.Default.AimbotProp.VisibilityCheckProp.BspParsing = DefaultAimbotVisibilityCheckBsp.IsChecked == true;
            G.C.Aim.Default.Rcs = DefaultRcs.IsChecked == true;
            G.C.Aim.Default.RcsProp.AimbotOnly = DefaultRcsAimbotOnly.IsChecked == true;
            G.C.Aim.Default.RcsProp.PitchStrength = (int)DefaultRcsPitchStrength.Value;
            G.C.Aim.Default.RcsProp.Smooth = (int)DefaultRcsSmooth.Value;
            G.C.Aim.Default.RcsProp.YawStrength = (int)DefaultRcsYawStrength.Value;
            G.C.Aim.Default.SilentAim = DefaultSilentAim.IsChecked == true;
            G.C.Aim.Default.SilentAimProp.Bone = (Bone)(DefaultSilentAimBone.SelectedItem as ComboBoxItem).Value;
            G.C.Aim.Default.SilentAimProp.Fov = (float)DefaultSilentAimFov.Value;
            G.C.Aim.Default.SilentAimProp.Key = (Keys)(DefaultSilentAimKey.SelectedItem as ComboBoxItem).Value;
            G.C.Aim.Default.SilentAimProp.OnGroundOnly = DefaultSilentAimOnGroundOnly.IsChecked == true;
            G.C.Aim.Default.SilentAimProp.VisibilityCheck = DefaultSilentAimVisibilityCheck.IsChecked == true;
            G.C.Aim.Default.SilentAimProp.VisibilityCheckProp.BspParsing = DefaultSilentAimVisibilityCheckBsp.IsChecked == true;
            G.C.Aim.Default.Triggerbot = DefaultTriggerbot.IsChecked == true;
            G.C.Aim.Default.TriggerbotProp.AfterDelay = (int)DefaultTriggerbotAfterDelay.Value;
            G.C.Aim.Default.TriggerbotProp.BeforeDelay = (int)DefaultTriggerbotBeforeDelay.Value;
            G.C.Aim.Default.TriggerbotProp.Key = (Keys)(DefaultTriggerbotKey.SelectedItem as ComboBoxItem).Value;
            G.C.Aim.Default.TriggerbotProp.ShootDelay = (int)DefaultTriggerbotShootDelay.Value;
            G.C.Aim.Default.TriggerbotWithAimbotKey = (Keys)(DefaultTriggerbotWithAimbotKey.SelectedItem as ComboBoxItem).Value;
            G.C.Aim.Pistols.Aimbot = PistolsAimbot.IsChecked == true;
            G.C.Aim.Pistols.AimbotProp.Bone = (Bone)(PistolsAimbotBone.SelectedItem as ComboBoxItem).Value;
            G.C.Aim.Pistols.AimbotProp.Fov = (int)PistolsAimbotFov.Value;
            G.C.Aim.Pistols.AimbotProp.Key = (Keys)(PistolsAimbotKey.SelectedItem as ComboBoxItem).Value;
            G.C.Aim.Pistols.AimbotProp.Lock = PistolsAimbotLock.IsChecked == true;
            G.C.Aim.Pistols.AimbotProp.LockProp.Length = (int)PistolsAimbotLockLength.Value;
            G.C.Aim.Pistols.AimbotProp.OnGroundOnly = PistolsAimbotOnGroundOnly.IsChecked == true;
            G.C.Aim.Pistols.AimbotProp.Smooth = (int)PistolsAimbotSmooth.Value;
            G.C.Aim.Pistols.AimbotProp.VisibilityCheck = PistolsAimbotVisibilityCheck.IsChecked == true;
            G.C.Aim.Pistols.AimbotProp.VisibilityCheckProp.BspParsing = PistolsAimbotVisibilityCheckBsp.IsChecked == true;
            G.C.Aim.Pistols.Rcs = PistolsRcs.IsChecked == true;
            G.C.Aim.Pistols.RcsProp.AimbotOnly = PistolsRcsAimbotOnly.IsChecked == true;
            G.C.Aim.Pistols.RcsProp.PitchStrength = (int)PistolsRcsPitchStrength.Value;
            G.C.Aim.Pistols.RcsProp.Smooth = (int)PistolsRcsSmooth.Value;
            G.C.Aim.Pistols.RcsProp.YawStrength = (int)PistolsRcsYawStrength.Value;
            G.C.Aim.Pistols.SilentAim = PistolsSilentAim.IsChecked == true;
            G.C.Aim.Pistols.SilentAimProp.Bone = (Bone)(PistolsSilentAimBone.SelectedItem as ComboBoxItem).Value;
            G.C.Aim.Pistols.SilentAimProp.Fov = (float)PistolsSilentAimFov.Value;
            G.C.Aim.Pistols.SilentAimProp.Key = (Keys)(PistolsSilentAimKey.SelectedItem as ComboBoxItem).Value;
            G.C.Aim.Pistols.SilentAimProp.OnGroundOnly = PistolsSilentAimOnGroundOnly.IsChecked == true;
            G.C.Aim.Pistols.SilentAimProp.VisibilityCheck = PistolsSilentAimVisibilityCheck.IsChecked == true;
            G.C.Aim.Pistols.SilentAimProp.VisibilityCheckProp.BspParsing = PistolsSilentAimVisibilityCheckBsp.IsChecked == true;
            G.C.Aim.Pistols.Triggerbot = PistolsTriggerbot.IsChecked == true;
            G.C.Aim.Pistols.TriggerbotProp.AfterDelay = (int)PistolsTriggerbotAfterDelay.Value;
            G.C.Aim.Pistols.TriggerbotProp.BeforeDelay = (int)PistolsTriggerbotBeforeDelay.Value;
            G.C.Aim.Pistols.TriggerbotProp.Key = (Keys)(PistolsTriggerbotKey.SelectedItem as ComboBoxItem).Value;
            G.C.Aim.Pistols.TriggerbotProp.ShootDelay = (int)PistolsTriggerbotShootDelay.Value;
            G.C.Aim.Pistols.TriggerbotWithAimbotKey = (Keys)(PistolsTriggerbotWithAimbotKey.SelectedItem as ComboBoxItem).Value;
            G.C.Aim.Snipers.Aimbot = SnipersAimbot.IsChecked == true;
            G.C.Aim.Snipers.AimbotProp.Bone = (Bone)(SnipersAimbotBone.SelectedItem as ComboBoxItem).Value;
            G.C.Aim.Snipers.AimbotProp.Fov = (int)SnipersAimbotFov.Value;
            G.C.Aim.Snipers.AimbotProp.Key = (Keys)(SnipersAimbotKey.SelectedItem as ComboBoxItem).Value;
            G.C.Aim.Snipers.AimbotProp.Lock = SnipersAimbotLock.IsChecked == true;
            G.C.Aim.Snipers.AimbotProp.LockProp.Length = (int)SnipersAimbotLockLength.Value;
            G.C.Aim.Snipers.AimbotProp.OnGroundOnly = SnipersAimbotOnGroundOnly.IsChecked == true;
            G.C.Aim.Snipers.AimbotProp.Smooth = (int)SnipersAimbotSmooth.Value;
            G.C.Aim.Snipers.AimbotProp.VisibilityCheck = SnipersAimbotVisibilityCheck.IsChecked == true;
            G.C.Aim.Snipers.AimbotProp.VisibilityCheckProp.BspParsing = SnipersAimbotVisibilityCheckBsp.IsChecked == true;
            G.C.Aim.Snipers.Rcs = SnipersRcs.IsChecked == true;
            G.C.Aim.Snipers.RcsProp.AimbotOnly = SnipersRcsAimbotOnly.IsChecked == true;
            G.C.Aim.Snipers.RcsProp.PitchStrength = (int)SnipersRcsPitchStrength.Value;
            G.C.Aim.Snipers.RcsProp.Smooth = (int)SnipersRcsSmooth.Value;
            G.C.Aim.Snipers.RcsProp.YawStrength = (int)SnipersRcsYawStrength.Value;
            G.C.Aim.Snipers.SilentAim = SnipersSilentAim.IsChecked == true;
            G.C.Aim.Snipers.SilentAimProp.Bone = (Bone)(SnipersSilentAimBone.SelectedItem as ComboBoxItem).Value;
            G.C.Aim.Snipers.SilentAimProp.Fov = (float)SnipersSilentAimFov.Value;
            G.C.Aim.Snipers.SilentAimProp.Key = (Keys)(SnipersSilentAimKey.SelectedItem as ComboBoxItem).Value;
            G.C.Aim.Snipers.SilentAimProp.OnGroundOnly = SnipersSilentAimOnGroundOnly.IsChecked == true;
            G.C.Aim.Snipers.SilentAimProp.VisibilityCheck = SnipersSilentAimVisibilityCheck.IsChecked == true;
            G.C.Aim.Snipers.SilentAimProp.VisibilityCheckProp.BspParsing = SnipersSilentAimVisibilityCheckBsp.IsChecked == true;
            G.C.Aim.Snipers.Triggerbot = SnipersTriggerbot.IsChecked == true;
            G.C.Aim.Snipers.TriggerbotProp.AfterDelay = (int)SnipersTriggerbotAfterDelay.Value;
            G.C.Aim.Snipers.TriggerbotProp.BeforeDelay = (int)SnipersTriggerbotBeforeDelay.Value;
            G.C.Aim.Snipers.TriggerbotProp.Key = (Keys)(SnipersTriggerbotKey.SelectedItem as ComboBoxItem).Value;
            G.C.Aim.Snipers.TriggerbotProp.ShootDelay = (int)SnipersTriggerbotShootDelay.Value;
            G.C.Aim.Snipers.TriggerbotWithAimbotKey = (Keys)(SnipersTriggerbotWithAimbotKey.SelectedItem as ComboBoxItem).Value;
            G.C.Key.Bunnyhop = (Keys)(BunnyhopKey.SelectedItem as ComboBoxItem).Value;
            G.C.Key.Exit = (Keys)(ExitKey.SelectedItem as ComboBoxItem).Value;
            G.C.Key.ForceUpdate = (Keys)(ForceUpdateKey.SelectedItem as ComboBoxItem).Value;
            G.C.Key.Toggle = (Keys)(ToggleKey.SelectedItem as ComboBoxItem).Value;
            G.C.Misc.AutoDuck = AutoDuck.IsChecked == true;
            G.C.Misc.AutoPistol = AutoPistol.IsChecked == true;
            G.C.Misc.AutoStop = AutoStop.IsChecked == true;
            G.C.Misc.BombDamage = BombDamage.IsChecked == true;
            G.C.Misc.Brightness = Brightness.IsChecked == true;
            G.C.Misc.BrightnessProp.Strength = (int)BrightnessStrength.Value;
            G.C.Misc.Bunnyhop = Bunnyhop.IsChecked == true;
            G.C.Misc.BunnyhopProp.AutoStrafe = BunnyhopAutoStrafe.IsChecked == true;
            G.C.Misc.ChatInfo = ChatInfo.IsChecked == true;
            G.C.Misc.ChatInfoProp.SendAll = ChatInfoSendAll.IsChecked == true;
            G.C.Misc.FakeLag = FakeLag.IsChecked == true;
            G.C.Misc.FakeLagProp.Length = (int)FakeLagLength.Value;
            G.C.Misc.FakeLagProp.Pause = (int)FakeLagPause.Value;
            G.C.Misc.FastReload = FastReload.IsChecked == true;
            G.C.Misc.FastSnipers = FastSnipers.IsChecked == true;
            G.C.Misc.FovChanger = FovChanger.IsChecked == true;
            G.C.Misc.FovChangerProp.Value = (int)FovChangerValue.Value;
            G.C.Misc.Knifebot = Knifebot.IsChecked == true;
            G.C.Misc.MoveHelper = MoveHelper.IsChecked == true;
            G.C.Misc.NoFlash = NoFlash.IsChecked == true;
            G.C.Misc.NoHands = NoHands.IsChecked == true;
            G.C.Misc.NoShake = NoShake.IsChecked == true;
            G.C.Misc.NoSmoke = NoSmoke.IsChecked == true;
            G.C.Misc.NoViewModel = NoViewModel.IsChecked == true;
            G.C.Misc.Radar = Radar.IsChecked == true;
            G.C.Misc.Spam = Spam.IsChecked == true;
            G.C.Misc.SpamProp.Message = SpamMessage.Text;
            G.C.Misc.SpamProp.SendAll = SpamSendAll.IsChecked == true;
            G.C.Misc.SpectatorList = SpectatorList.IsChecked == true;
            G.C.Misc.ThirdPersonMode = ThirdPersonMode.IsChecked == true;
            G.C.Stts.Antialiasing = Antialiasing.IsChecked == true;
            G.C.Stts.FriendlyFire = FirendlyFire.IsChecked == true;
            G.C.Stts.WindowWidth = (int)Width;
            G.C.Vis.AimFov = AimFov.IsChecked == true;
            G.C.Vis.AimPoint = AimPoint.IsChecked == true;
            G.C.Vis.Entities.AdditionalInfo = EntitiesAdditionalInfo.IsChecked == true;
            G.C.Vis.Entities.Chams = EntitiesChams.IsChecked == true;
            G.C.Vis.Entities.Esp2D = EntitiesEsp2D.IsChecked == true;
            G.C.Vis.Entities.Esp3D = EntitiesEsp3D.IsChecked == true;
            G.C.Vis.Entities.Glow = EntitiesGlow.IsChecked == true;
            G.C.Vis.Entities.GlowProp.Fill = EntitiesGlowFill.IsChecked == true;
            G.C.Vis.Entities.GlowProp.FillTransparent = EntitiesGlowFillTransparent.IsChecked == true;
            G.C.Vis.Entities.GlowProp.Inner = EntitiesGlowInner.IsChecked == true;
            G.C.Vis.Entities.ImportantOnly = EntitiesImportantOnly.IsChecked == true;
            G.C.Vis.Entities.LocalChams = EntitiesLocalChams.IsChecked == true;
            G.C.Vis.Entities.Name = EntitiesName.IsChecked == true;
            G.C.Vis.Entities.Snaplines = EntitiesSnaplines.IsChecked == true;
            G.C.Vis.HideAllies = HideAllies.IsChecked == true;
            G.C.Vis.HideChickens = HideChickens.IsChecked == true;
            G.C.Vis.HideEnemies = HideEnemies.IsChecked == true;
            G.C.Vis.Players.ActiveWeapon = PlayersActiveWeapon.IsChecked == true;
            G.C.Vis.Players.ActiveWeaponProp.Ammo = PlayersActiveWeaponAmmo.IsChecked == true;
            G.C.Vis.Players.AdditionalInfo = PlayersAdditionalInfo.IsChecked == true;
            G.C.Vis.Players.Armor = PlayersArmor.IsChecked == true;
            G.C.Vis.Players.Chams = PlayersChams.IsChecked == true;
            G.C.Vis.Players.ChamsProp.Health = PlayersChamsHealth.IsChecked == true;
            G.C.Vis.Players.Esp2D = PlayersEsp2D.IsChecked == true;
            G.C.Vis.Players.Esp3D = PlayersEsp3D.IsChecked == true;
            G.C.Vis.Players.Floor = PlayersFloor.IsChecked == true;
            G.C.Vis.Players.Glow = PlayersGlow.IsChecked == true;
            G.C.Vis.Players.GlowProp.Fill = PlayersGlowFill.IsChecked == true;
            G.C.Vis.Players.GlowProp.FillTransparent = PlayersGlowFillTransparent.IsChecked == true;
            G.C.Vis.Players.GlowProp.Health = PlayersGlowHealth.IsChecked == true;
            G.C.Vis.Players.GlowProp.Inner = PlayersGlowInner.IsChecked == true;
            G.C.Vis.Players.Health = PlayersHealth.IsChecked == true;
            G.C.Vis.Players.Name = PlayersName.IsChecked == true;
            G.C.Vis.Players.Rank = PlayersRank.IsChecked == true;
            G.C.Vis.Players.RankProp.Simple = PlayersRankSimple.IsChecked == true;
            G.C.Vis.Players.RotationArrow = PlayersRotationArrow.IsChecked == true;
            G.C.Vis.Players.Skeleton = PlayersSkeleton.IsChecked == true;
            G.C.Vis.Players.SkeletonProp.Fill = PlayersSkeletonFill.IsChecked == true;
            G.C.Vis.Players.Snaplines = PlayersSnaplines.IsChecked == true;
            G.C.Vis.Players.VisibilityCheck = PlayersVisibilityCheck.IsChecked == true;
            G.C.Vis.RecoilCrosshair = RecoilCrosshair.IsChecked == true;
            G.C.Vis.SilentAimFov = SilentAimFov.IsChecked == true;
        }

        private int GetComboBoxIndexByBone(Bone bone)
        {
            var index = 0;

            // Get index by checking item values
            foreach (var item in DefaultAimbotBone.Items)
            {
                var obj = item as ComboBoxItem;
                if ((int) obj.Value == (int) bone)
                {
                    return index;
                }

                index++;
            }

            return 0;
        }
        private static int GetComboBoxIndexByKey(ItemsControl cbox, Keys key)
        {
            var index = 0;

            // Get index by checking item values
            foreach (var item in cbox.Items)
            {
                var obj = item as ComboBoxItem;
                if ((int) obj.Value == (int) key)
                {
                    return index;
                }

                index++;
            }

            return 0;
        }

        private void ConfigRestoreDefault(object sender, RoutedEventArgs e)
        {
            var result = MessageBox.Show("Are you sure?\nYou will lost all your custom settings!", "DDX Multihack", MessageBoxButton.YesNo, MessageBoxImage.Question);
            if (result != MessageBoxResult.Yes)
            {
                return;
            }

            File.Delete(ConfigParser.FilePath);
            ConfigParser.Load();

            _disableEvents = true;
            ConfigToControls();
            _disableEvents = false;
        }
    }
}
