using DDX_Multihack.Properties;
using FadeAPI;
using FadeAPI.Enums;
using MahApps.Metro.Controls;
using SharpDX.Mathematics.Interop;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Net;
using System.Security.Cryptography;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Forms;
using System.Windows.Input;
using System.Windows.Media;
using FadeAPI.Config;
using FadeAPI.Helpers;
using SharpDX.Text;
using Application = System.Windows.Application;
using Clipboard = System.Windows.Forms.Clipboard;
using Color = System.Windows.Media.Color;
using ComboBox = System.Windows.Controls.ComboBox;
using KeyEventArgs = System.Windows.Forms.KeyEventArgs;
using MessageBox = System.Windows.MessageBox;
using NumericUpDown = MahApps.Metro.Controls.NumericUpDown;
using TextBox = System.Windows.Controls.TextBox;

namespace DDX_Multihack
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow
    {
        //[swap]
        private readonly double _defTop;
        private Mutex _mutex;
        private readonly double _defLeft;
        private readonly double _aspect;
        //[/swap]

        public MainWindow()
        {
            //[swap]
            const string trash6 = @"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";
            const string trash5 = @"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";
            const string trash2 = @"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";
            const string trash3 = @"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";
            InitializeComponent();
            const string trash8 = @"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";
            const string trash4 = @"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";
            const string trash7 = @"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";
            const string trash1 = @"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";
            //[/swap]

            //[swap]
            Title = trash1;
            Title = trash6;
            Title = trash5;
            _defLeft = Left;
            Title = trash8;
            Title = trash3;
            Title = trash7;
            Title = trash2;
            _defTop = Top;
            Title = trash4;
            _aspect = Width / Height;
            FillComboBoxes();
            //[/swap]

            //[swap]
            Height = Settings.Default.Height;
            Width = Settings.Default.Width;
            Title = Rnd.GetRandomString();
            //[/swap]
        }

        private void MetroWindow_Loaded(object sender, RoutedEventArgs e)
        {
            // Single instance
            if (!IsSingleInstance())
            {
                MessageBox.Show("Another DDX process is open", Rnd.GetRandomString(), MessageBoxButton.OK);
                Environment.Exit(0);
            }

            var directoryPath = Path.GetDirectoryName(Process.GetCurrentProcess().MainModule.FileName);

            //[swap]
            G.Config = new ConfigCore(AppDomain.CurrentDomain.BaseDirectory + @"\config.ini");
            G.SkinConfig = new ConfigCore(AppDomain.CurrentDomain.BaseDirectory + @"\skins.ini");
            G.Fade = new Fade();
            WinAPI.AddFontResource(directoryPath + @"\SquareFont.ttf");
            //[/swap]

            var attachResult = G.Fade.Process.Attach();

            switch (attachResult)
            {
                //[swap]
                //[block]
                case Attach.ProcessNotFound:
                    MessageBox.Show("CS:GO process not found", Rnd.GetRandomString(), MessageBoxButton.OK);
                    Environment.Exit(0);
                    break;
                //[/block]
                //[block]
                case Attach.Success:
                    break;
                //[/block]
                //[/swap]
                default:
                    MessageBox.Show("FadeAPI error - Process.Attach." + attachResult, Rnd.GetRandomString(), MessageBoxButton.OK);
                    Environment.Exit(0);
                    break;
            }

            //[swap]
            G.Fade.Overlay.ShowOverlay();
            G.Fade.KeyboardHook.KeyDown += KeyboardHook_KeyDown;
            G.Fade.KeyboardHook.HookedKeys.Add(Keys.Home);
            Main.Start();
            G.Fade.KeyboardHook.HookedKeys.Add(Keys.Insert);
            G.Fade.KeyboardHook.HookedKeys.Add(Keys.End);
            G.Fade.KeyboardHook.HookedKeys.Add(Keys.Delete);
            //[/swap]

            // Prepare for config load
            foreach (var slider in this.FindChildren<Slider>())
            {
                slider.Value = 1;
            }

            //[swap]
            //[block]
            if (!File.Exists(G.SkinConfig.Path))
                CreateSkinsConfig();
            //[/block]
            //[block]
            if (!File.Exists(G.Config.Path))
                CreateIniConfig();
            else
                LoadIniConfig(null, null);
            //[/block]
            //[/swap]

            //[swap]
            LoadSkinsConfig();
            LoadNetConfig();
            G.Fade.Overlay.Form.Text = Rnd.GetRandomString();
            //[/swap]

            //[swap]
            SkinWeapon.SelectedIndex = 0;
            RefreshAllToggles();
            StartPositionThread();
            //[/swap]
        }

        private void MetroWindow_Closing(object sender, CancelEventArgs e)
        {
            //[swap]
            SaveSkinsConfig();
            SaveNetConfig();
            //[/swap]
            G.Fade.Process.Detach();
            Environment.Exit(0);
        }

        private void MetroWindow_KeyDown(object sender, System.Windows.Input.KeyEventArgs e)
        {
            //[swap]
            //[block]
            if (e.Key == Key.Left)
            {
                if (TabControl.SelectedIndex > 0)
                    TabControl.SelectedIndex--;
            }
            //[/block]
            //[block]
            if (e.Key == Key.Right)
            {
                if (TabControl.SelectedIndex < TabControl.Items.Count - 1)
                    TabControl.SelectedIndex++;
            }
            //[/block]
            //[/swap]
        }

        private void MetroWindow_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            //[swap]
            Width = Height * _aspect;
            Left = _defLeft;
            Top = _defTop;
            //[/swap]
        }

        private void KeyboardHook_KeyDown(object sender, KeyEventArgs e)
        {
            //[swap]
            //[block]
            if (e.KeyCode == (Keys)(ExitKey.SelectedValue as ComboBoxItem).Value)
            {
                Close();
            }
            //[/block]
            //[block]
            if (e.KeyCode == (Keys)(ToggleKey.SelectedValue as ComboBoxItem).Value)
            {
                if (IsVisible)
                {
                    Hide();
                }
                else
                {
                    Show();
                    Activate();
                }
            }
            //[/block]
            //[/swap]
        }

        private bool IsSingleInstance()
        {
            var mutexValue = Convert.ToBase64String(MD5.Create().ComputeHash(System.Text.Encoding.Default.GetBytes($"DDX_{Environment.UserName}_{Environment.UserDomainName}")));

            try
            {
                Mutex.OpenExisting(mutexValue);
            }
            catch
            {
                _mutex = new Mutex(true, mutexValue);
                return true;
            }

            return false;
        }

        private void StartPositionThread()
        {
            Thread thr = new Thread(PositionThread)
            {
                IsBackground = true
            };
            thr.Start();
        }

        private void PositionThread()
        {
            try
            {
                while (true)
                {
                    var windowInfo = G.Fade.Window.GetWindowInfo();

                    Application.Current.Dispatcher.Invoke(() =>
                    {
                        //[swap]
                        Main.CsY = windowInfo.Y;
                        Main.CsHeight = windowInfo.Height;
                        Main.CsX = windowInfo.X;
                        Main.CsWidth = windowInfo.Width;
                        //[/swap]

                        if (Main.CsX != 0f && Main.CsY != 0f)
                        {
                            //[swap]
                            Main.CsHeight -= 26;
                            Main.CsY += 26;
                            //[/swap]
                        }

                        //[swap]
                        Top = Main.CsY + 10;
                        Left = Main.CsX + 10;
                        Main.ScreenCenter = new RawVector2(Main.CsX + Main.CsWidth / 2, Main.CsY + Main.CsHeight / 2);
                        Main.WindowScale = Main.CsHeight / Screen.PrimaryScreen.Bounds.Height;
                        //[/swap]
                    });

                    Thread.Sleep(1000);
                }
            }
            catch
            {
                if (Process.GetProcessesByName("csgo").Length == 0)
                {
                    Application.Current.Dispatcher.Invoke(() =>
                    {
                        Application.Current.Shutdown();
                    });
                }

                Thread.Sleep(100);
                StartPositionThread();
            }
        }

        private void FillComboBoxes()
        {
            //[swap]
            //[block]
            ToggleKey.Items.Add(new ComboBoxItem("Insert", Keys.Insert));
            ToggleKey.Items.Add(new ComboBoxItem("Home"  , Keys.Home));
            ToggleKey.Items.Add(new ComboBoxItem("Delete", Keys.Delete));
            ToggleKey.Items.Add(new ComboBoxItem("End"   , Keys.End));
            //[/block]
            //[block]
            AimbotPistolsBone.Items.Add(new ComboBoxItem("Dynamic"    , -1));
            AimbotPistolsBone.Items.Add(new ComboBoxItem("Head"       , Bone.Head));
            AimbotPistolsBone.Items.Add(new ComboBoxItem("Neck"       , Bone.Neck));
            AimbotPistolsBone.Items.Add(new ComboBoxItem("Chest"      , Bone.Chest));
            AimbotPistolsBone.Items.Add(new ComboBoxItem("Stomach"    , Bone.Stomach));
            AimbotPistolsBone.Items.Add(new ComboBoxItem("Left Arm"   , Bone.LeftArm));
            AimbotPistolsBone.Items.Add(new ComboBoxItem("Left Elbow" , Bone.LeftElbow));
            AimbotPistolsBone.Items.Add(new ComboBoxItem("Right Arm"  , Bone.RightArm));
            AimbotPistolsBone.Items.Add(new ComboBoxItem("Right Elbow", Bone.RightElbow));
            AimbotPistolsBone.Items.Add(new ComboBoxItem("Left Leg"   , Bone.LeftLeg));
            AimbotPistolsBone.Items.Add(new ComboBoxItem("Left Knee"  , Bone.LeftKnee));
            AimbotPistolsBone.Items.Add(new ComboBoxItem("Right Leg"  , Bone.RightLeg));
            AimbotPistolsBone.Items.Add(new ComboBoxItem("Right Knee" , Bone.RightKnee));
            //[/block]
            //[block]
            ExitKey.Items.Add(new ComboBoxItem("Insert", Keys.Insert));
            ExitKey.Items.Add(new ComboBoxItem("Home"  , Keys.Home));
            ExitKey.Items.Add(new ComboBoxItem("Delete", Keys.Delete));
            ExitKey.Items.Add(new ComboBoxItem("End"   , Keys.End));
            //[/block]
            //[block]
            SkinWeapon.Items.Add(new ComboBoxItem("AK-47"        , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("AUG"          , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("AWP"          , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("CZ75-Auto"    , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("Desert Eagle" , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("Dual Berettas", 0));
            SkinWeapon.Items.Add(new ComboBoxItem("FAMAS"        , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("Five-SeveN"   , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("G3SG1"        , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("Galil AR"     , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("Glock-18"     , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("M249"         , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("M4A1-S"       , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("M4A4"         , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("MAC-10"       , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("MAG-7"        , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("MP7"          , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("MP9"          , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("Negev"        , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("Nova"         , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("P2000"        , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("P250"         , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("P90"          , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("PP-Bizon"     , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("R8 Revolver"  , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("Sawed-Off"    , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("SCAR-20"      , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("SG 553"       , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("SSG 08"       , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("Tec-9"        , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("UMP-45"       , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("USP-S"        , 0));
            SkinWeapon.Items.Add(new ComboBoxItem("XM1014"       , 0));
            //[/block]
            //[block]
            TriggerbotKey.Items.Add(new ComboBoxItem("Disabled" , Keys.F24));
            TriggerbotKey.Items.Add(new ComboBoxItem("Always On", Keys.F23));
            TriggerbotKey.Items.Add(new ComboBoxItem("LCtrl"    , Keys.LControlKey));
            TriggerbotKey.Items.Add(new ComboBoxItem("RCtrl"    , Keys.RControlKey));
            TriggerbotKey.Items.Add(new ComboBoxItem("LShift"   , Keys.LShiftKey));
            TriggerbotKey.Items.Add(new ComboBoxItem("RShift"   , Keys.RShiftKey));
            TriggerbotKey.Items.Add(new ComboBoxItem("Alt"      , Keys.Menu));
            TriggerbotKey.Items.Add(new ComboBoxItem("LMB"      , Keys.LButton));
            TriggerbotKey.Items.Add(new ComboBoxItem("MMB"      , Keys.MButton));
            TriggerbotKey.Items.Add(new ComboBoxItem("RMB"      , Keys.RButton));
            TriggerbotKey.Items.Add(new ComboBoxItem("X1 Button", Keys.XButton1));
            TriggerbotKey.Items.Add(new ComboBoxItem("X2 Button", Keys.XButton2));
            TriggerbotKey.Items.Add(new ComboBoxItem("E"        , Keys.E));
            TriggerbotKey.Items.Add(new ComboBoxItem("Space"    , Keys.Space));
            //[/block]
            //[block]
            ForceUpdateKey.Items.Add(new ComboBoxItem("F1", Keys.F1));
            ForceUpdateKey.Items.Add(new ComboBoxItem("F2", Keys.F2));
            ForceUpdateKey.Items.Add(new ComboBoxItem("F3", Keys.F3));
            ForceUpdateKey.Items.Add(new ComboBoxItem("F4", Keys.F4));
            ForceUpdateKey.Items.Add(new ComboBoxItem("V" , Keys.V));
            //[/block]
            //[block]
            AimbotSnipersBone.Items.Add(new ComboBoxItem("Dynamic"    , -1));
            AimbotSnipersBone.Items.Add(new ComboBoxItem("Head"       , Bone.Head));
            AimbotSnipersBone.Items.Add(new ComboBoxItem("Neck"       , Bone.Neck));
            AimbotSnipersBone.Items.Add(new ComboBoxItem("Chest"      , Bone.Chest));
            AimbotSnipersBone.Items.Add(new ComboBoxItem("Stomach"    , Bone.Stomach));
            AimbotSnipersBone.Items.Add(new ComboBoxItem("Left Arm"   , Bone.LeftArm));
            AimbotSnipersBone.Items.Add(new ComboBoxItem("Left Elbow" , Bone.LeftElbow));
            AimbotSnipersBone.Items.Add(new ComboBoxItem("Right Arm"  , Bone.RightArm));
            AimbotSnipersBone.Items.Add(new ComboBoxItem("Right Elbow", Bone.RightElbow));
            AimbotSnipersBone.Items.Add(new ComboBoxItem("Left Leg"   , Bone.LeftLeg));
            AimbotSnipersBone.Items.Add(new ComboBoxItem("Left Knee"  , Bone.LeftKnee));
            AimbotSnipersBone.Items.Add(new ComboBoxItem("Right Leg"  , Bone.RightLeg));
            AimbotSnipersBone.Items.Add(new ComboBoxItem("Right Knee" , Bone.RightKnee));
            //[/block]
            //[block]
            AimbotBone.Items.Add(new ComboBoxItem("Dynamic"    , -1));
            AimbotBone.Items.Add(new ComboBoxItem("Head"       , Bone.Head));
            AimbotBone.Items.Add(new ComboBoxItem("Neck"       , Bone.Neck));
            AimbotBone.Items.Add(new ComboBoxItem("Chest"      , Bone.Chest));
            AimbotBone.Items.Add(new ComboBoxItem("Stomach"    , Bone.Stomach));
            AimbotBone.Items.Add(new ComboBoxItem("Left Arm"   , Bone.LeftArm));
            AimbotBone.Items.Add(new ComboBoxItem("Left Elbow" , Bone.LeftElbow));
            AimbotBone.Items.Add(new ComboBoxItem("Right Arm"  , Bone.RightArm));
            AimbotBone.Items.Add(new ComboBoxItem("Right Elbow", Bone.RightElbow));
            AimbotBone.Items.Add(new ComboBoxItem("Left Leg"   , Bone.LeftLeg));
            AimbotBone.Items.Add(new ComboBoxItem("Left Knee"  , Bone.LeftKnee));
            AimbotBone.Items.Add(new ComboBoxItem("Right Leg"  , Bone.RightLeg));
            AimbotBone.Items.Add(new ComboBoxItem("Right Knee" , Bone.RightKnee));
            //[/block]
            //[block]
            SkinPrefix.Items.Add(new ComboBoxItem("Disabled"  , 0));
            SkinPrefix.Items.Add(new ComboBoxItem("Genuine"   , 1));
            SkinPrefix.Items.Add(new ComboBoxItem("Vintage"   , 2));
            SkinPrefix.Items.Add(new ComboBoxItem("Star"      , 3));
            SkinPrefix.Items.Add(new ComboBoxItem("Community" , 5));
            SkinPrefix.Items.Add(new ComboBoxItem("Valve"     , 6));
            SkinPrefix.Items.Add(new ComboBoxItem("Prototype" , 7));
            SkinPrefix.Items.Add(new ComboBoxItem("Customized", 8));
            SkinPrefix.Items.Add(new ComboBoxItem("StatTrak"  , 9));
            SkinPrefix.Items.Add(new ComboBoxItem("Completed" , 10));
            SkinPrefix.Items.Add(new ComboBoxItem("Souvenir"  , 12));
            //[/block]
            //[block]
            AimbotKey.Items.Add(new ComboBoxItem("Disabled" , Keys.F24));
            AimbotKey.Items.Add(new ComboBoxItem("Always On", Keys.F23));
            AimbotKey.Items.Add(new ComboBoxItem("LCtrl"    , Keys.LControlKey));
            AimbotKey.Items.Add(new ComboBoxItem("RCtrl"    , Keys.RControlKey));
            AimbotKey.Items.Add(new ComboBoxItem("LShift"   , Keys.LShiftKey));
            AimbotKey.Items.Add(new ComboBoxItem("RShift"   , Keys.RShiftKey));
            AimbotKey.Items.Add(new ComboBoxItem("Alt"      , Keys.Menu));
            AimbotKey.Items.Add(new ComboBoxItem("LMB"      , Keys.LButton));
            AimbotKey.Items.Add(new ComboBoxItem("MMB"      , Keys.MButton));
            AimbotKey.Items.Add(new ComboBoxItem("RMB"      , Keys.RButton));
            AimbotKey.Items.Add(new ComboBoxItem("X1 Button", Keys.XButton1));
            AimbotKey.Items.Add(new ComboBoxItem("X2 Button", Keys.XButton2));
            AimbotKey.Items.Add(new ComboBoxItem("E"        , Keys.E));
            AimbotKey.Items.Add(new ComboBoxItem("Space"    , Keys.Space));
            //[/block]
            //[/swap]
        }

        private void RefreshAllToggles()
        {
            foreach (var button in this.FindChildren<ToggleSwitch>())
            {
                ToggleManager(button, null);
            }
        }

        private void LoadNetConfig()
        {
            //[swap]
            EntitiesB.Value           = Settings.Default.EntitiesColor.Color.B;
            RecoilCrosshairR.Value    = Settings.Default.RecoilCrosshairColor.Color.R;
            HealthBarG.Value          = Settings.Default.HealthBarColor.Color.G;
            FovB.Value                = Settings.Default.FovColor.Color.B;
            AdditionalInfoB.Value     = Settings.Default.AdditionalInfoColor.Color.B;
            FovG.Value                = Settings.Default.FovColor.Color.G;
            ActiveWeaponG.Value       = Settings.Default.ActiveWeaponColor.Color.G;
            HeadHelperB.Value         = Settings.Default.HeadHelperColor.Color.B;
            AlliesB.Value             = Settings.Default.AlliesColor.Color.B;
            SkeletonEnemiesVisB.Value = Settings.Default.SkeletonEnemiesVisColor.Color.B;
            ChickensB.Value           = Settings.Default.ChickensColor.Color.B;
            ChamsEnemiesG.Value       = Settings.Default.ChamsEnemiesColor.Color.G;
            ChamsAlliesR.Value        = Settings.Default.ChamsAlliesColor.Color.R;
            SpecialEntitiesR.Value    = Settings.Default.SpecialEntitiesColor.Color.R;
            SpectatorsB.Value         = Settings.Default.SpectatorsColor.Color.B;
            ActiveWeaponB.Value       = Settings.Default.ActiveWeaponColor.Color.B;
            SkeletonEnemiesG.Value    = Settings.Default.SkeletonEnemiesColor.Color.G;
            ActiveWeaponR.Value       = Settings.Default.ActiveWeaponColor.Color.R;
            NamesB.Value              = Settings.Default.NameColor.Color.B;
            EnemiesVisB.Value         = Settings.Default.EnemiesVisColor.Color.B;
            PathEnemiesB.Value        = Settings.Default.PathEnemiesColor.Color.B;
            HeadHelperR.Value         = Settings.Default.HeadHelperColor.Color.R;
            ChamsAlliesB.Value        = Settings.Default.ChamsAlliesColor.Color.B;
            ChamsEnemiesB.Value       = Settings.Default.ChamsEnemiesColor.Color.B;
            EntitiesR.Value           = Settings.Default.EntitiesColor.Color.R;
            SpecialEntitiesB.Value    = Settings.Default.SpecialEntitiesColor.Color.B;
            BackgroundG.Value         = Settings.Default.BackgroundColor.Color.G;
            RanksG.Value              = Settings.Default.RankColor.Color.G;
            EnemiesVisR.Value         = Settings.Default.EnemiesVisColor.Color.R;
            EnemiesVisG.Value         = Settings.Default.EnemiesVisColor.Color.G;
            SkeletonEnemiesR.Value    = Settings.Default.SkeletonEnemiesColor.Color.R;
            ChickensG.Value           = Settings.Default.ChickensColor.Color.G;
            EnemiesG.Value            = Settings.Default.EnemiesColor.Color.G;
            ChickensR.Value           = Settings.Default.ChickensColor.Color.R;
            HealthBarB.Value          = Settings.Default.HealthBarColor.Color.B;
            EnemiesR.Value            = Settings.Default.EnemiesColor.Color.R;
            EnemiesB.Value            = Settings.Default.EnemiesColor.Color.B;
            SkeletonAlliesG.Value     = Settings.Default.SkeletonAlliesColor.Color.G;
            RanksB.Value              = Settings.Default.RankColor.Color.B;
            FovR.Value                = Settings.Default.FovColor.Color.R;
            PathEnemiesR.Value        = Settings.Default.PathEnemiesColor.Color.R;
            AdditionalInfoR.Value     = Settings.Default.AdditionalInfoColor.Color.R;
            SpecialEntitiesG.Value    = Settings.Default.SpecialEntitiesColor.Color.G;
            BackgroundR.Value         = Settings.Default.BackgroundColor.Color.R;
            SkeletonAlliesR.Value     = Settings.Default.SkeletonAlliesColor.Color.R;
            PathAlliesR.Value         = Settings.Default.PathAlliesColor.Color.R;
            NamesG.Value              = Settings.Default.NameColor.Color.G;
            EntitiesG.Value           = Settings.Default.EntitiesColor.Color.G;
            RecoilCrosshairB.Value    = Settings.Default.RecoilCrosshairColor.Color.B;
            RecoilCrosshairG.Value    = Settings.Default.RecoilCrosshairColor.Color.G;
            RanksR.Value              = Settings.Default.RankColor.Color.R;
            SkeletonAlliesB.Value     = Settings.Default.SkeletonAlliesColor.Color.B;
            SkeletonEnemiesB.Value    = Settings.Default.SkeletonEnemiesColor.Color.B;
            SkeletonEnemiesVisR.Value = Settings.Default.SkeletonEnemiesVisColor.Color.R;
            NamesR.Value              = Settings.Default.NameColor.Color.R;
            ChamsAlliesG.Value        = Settings.Default.ChamsAlliesColor.Color.G;
            ChamsEnemiesR.Value       = Settings.Default.ChamsEnemiesColor.Color.R;
            HeadHelperG.Value         = Settings.Default.HeadHelperColor.Color.G;
            SpectatorsR.Value         = Settings.Default.SpectatorsColor.Color.R;
            AdditionalInfoG.Value     = Settings.Default.AdditionalInfoColor.Color.G;
            BackgroundB.Value         = Settings.Default.BackgroundColor.Color.B;
            PathAlliesB.Value         = Settings.Default.PathAlliesColor.Color.B;
            AlliesG.Value             = Settings.Default.AlliesColor.Color.G;
            HealthBarR.Value          = Settings.Default.HealthBarColor.Color.R;
            PathAlliesG.Value         = Settings.Default.PathAlliesColor.Color.G;
            PathEnemiesG.Value        = Settings.Default.PathEnemiesColor.Color.G;
            SkeletonEnemiesVisG.Value = Settings.Default.SkeletonEnemiesVisColor.Color.G;
            AlliesR.Value             = Settings.Default.AlliesColor.Color.R;
            SpectatorsG.Value         = Settings.Default.SpectatorsColor.Color.G;
            //[/swap]
        }

        private void SaveNetConfig()
        {
            //[swap]
            Settings.Default.BackgroundColor         = (SolidColorBrush)BackgroundColor.Background;
            Settings.Default.SpecialEntitiesColor    = (SolidColorBrush)SpecialEntitiesColor.Background;
            Settings.Default.AlliesColor             = (SolidColorBrush)AlliesColor.Background;
            Settings.Default.RecoilCrosshairColor    = (SolidColorBrush)RecoilCrosshairColor.Background;
            Settings.Default.HeadHelperColor         = (SolidColorBrush)HeadHelperColor.Background;
            Settings.Default.FovColor                = (SolidColorBrush)FovColor.Background;
            Settings.Default.SkeletonEnemiesVisColor = (SolidColorBrush)SkeletonEnemiesVisColor.Background;
            Settings.Default.ChamsEnemiesColor       = (SolidColorBrush)ChamsEnemiesColor.Background;
            Settings.Default.SkeletonEnemiesColor    = (SolidColorBrush)SkeletonEnemiesColor.Background;
            Settings.Default.PathAlliesColor         = (SolidColorBrush)PathAlliesColor.Background;
            Settings.Default.RankColor               = (SolidColorBrush)RanksColor.Background;
            Settings.Default.ChamsAlliesColor        = (SolidColorBrush)ChamsAlliesColor.Background;
            Settings.Default.EnemiesColor            = (SolidColorBrush)EnemiesColor.Background;
            Settings.Default.EnemiesVisColor         = (SolidColorBrush)EnemiesVisColor.Background;
            Settings.Default.Width                   = Width;
            Settings.Default.AdditionalInfoColor     = (SolidColorBrush)AdditionalInfoColor.Background;
            Settings.Default.EntitiesColor           = (SolidColorBrush)EntitiesColor.Background;
            Settings.Default.HealthBarColor          = (SolidColorBrush)HealthBarColor.Background;
            Settings.Default.NameColor               = (SolidColorBrush)NamesColor.Background;
            Settings.Default.SpectatorsColor         = (SolidColorBrush)SpectatorsColor.Background;
            Settings.Default.ChickensColor           = (SolidColorBrush)ChickensColor.Background;
            Settings.Default.Height                  = Height;
            Settings.Default.ActiveWeaponColor       = (SolidColorBrush)ActiveWeaponColor.Background;
            Settings.Default.PathEnemiesColor        = (SolidColorBrush)PathEnemiesColor.Background;
            Settings.Default.SkeletonAlliesColor     = (SolidColorBrush)SkeletonAlliesColor.Background;
            //[/swap]

            Settings.Default.Save();
        }

        private void CreateIniConfig()
        {
            //[swap]
            AimbotPistolsBone.SelectedIndex = 1;
            PathLocalOnly.IsChecked         = true;
            FovChangerValue.Value           = 130d;
            RcsPistol.IsChecked             = true;
            ExitKey.SelectedIndex           = 1;
            ForceUpdateKey.SelectedIndex    = 0;
            FovPistolsSize.Value            = 100d;
            SoundEspLengthScale.Value       = .5d;
            PathQuality.Value               = 900d;
            RecoilBasedFov.IsChecked        = true;
            AimbotPistolsLockTime.Value     = 250d;
            SoundEspFrequency.Value         = 800d;
            TriggerbotSnipersDelay.Value    = 150d;
            GlowDelay.Value                 = 1d;
            RcsStrengthX.Value              = 1.9d;
            AimbotKey.SelectedIndex         = 10;
            TriggerbotKey.SelectedIndex     = 11;
            BrightnessStrength.Value        = 10d;
            AimbotSpeed.Value               = 40d;
            FovSize.Value                   = 100d;
            AimbotLockTime.Value            = 250d;
            FakeLagPause.Value              = 250d;
            FpsLimit.Value                  = 120d;
            AimbotSnipersSpeed.Value        = 40d;
            PathLength.Value                = 10d;
            TriggerbotPistolsDelay.Value    = 150d;
            RcsStrengthY.Value              = 2d;
            AimbotSnipersBone.SelectedIndex = 3;
            HeadHelperSize.Value            = 15d;
            AimbotBone.SelectedIndex        = 0;
            FovSnipersSize.Value            = 100d;
            Antialiasing.IsChecked          = true;
            VisibilityCheck.IsChecked       = true;
            AimbotPistolsSpeed.Value        = 40d;
            SoundEspFovSize.Value           = 100d;
            AimbotSnipersLockTime.Value     = 250d;
            ToggleKey.SelectedIndex         = 0;
            TriggerbotDelay.Value           = 150d;
            FakeLagLength.Value             = 200d;
            //[/swap]

            SaveIniConfig(null, null);
        }

        private void LoadIniConfig(object sender, RoutedEventArgs e)
        {
            try
            {
                //[swap]
                SkinChanger.IsChecked           = ReadIniConfig("Misc", "SkinChanger") == "true";
                Entities2D.IsChecked            = ReadIniConfig("Visuals", "Entities2D") == "true";
                ExitKey.SelectedIndex           = int.Parse(ReadIniConfig("Settings", "ExitKey"));
                PlayersGlow.IsChecked           = ReadIniConfig("Visuals", "PlayersGlow") == "true";
                Entities3D.IsChecked            = ReadIniConfig("Visuals", "Entities3D") == "true";
                RcsAimbotOnly.IsChecked         = ReadIniConfig("Settings", "RcsAimbotOnly") == "true";
                FovSnipersSize.Value            = double.Parse(ReadIniConfig("Aim", "FovSnipersSize"));
                RcsPositionFix.IsChecked        = ReadIniConfig("Settings", "RcsPositionFix") == "true";
                SoundEspFrequency.Value         = double.Parse(ReadIniConfig("Sound", "SoundEspFrequency"));
                ToggleKey.SelectedIndex         = int.Parse(ReadIniConfig("Settings", "ToggleKey"));
                HideEnemies.IsChecked           = ReadIniConfig("Visuals", "HideEnemies") == "true";
                FullGlow.IsChecked              = ReadIniConfig("Settings", "FullGlow") == "true";
                AimbotPistolsSpeed.Value        = double.Parse(ReadIniConfig("Aim", "AimbotPistolsSpeed"));
                AimbotKey.SelectedIndex         = int.Parse(ReadIniConfig("Settings", "AimbotKey"));
                Players2D.IsChecked             = ReadIniConfig("Visuals", "Players2D") == "true";
                RecoilBasedFov.IsChecked        = ReadIniConfig("Settings", "RecoilBasedFov") == "true";
                Bunnyhop.IsChecked              = ReadIniConfig("Misc", "Bunnyhop") == "true";
                VisibilityCheck.IsChecked       = ReadIniConfig("Settings", "VisibilityCheck") == "true";
                AimbotLockTime.Value            = double.Parse(ReadIniConfig("Aim", "AimbotLockTime"));
                FovPistols.IsChecked            = ReadIniConfig("Aim", "FovPistols") == "true";
                Radar.IsChecked                 = ReadIniConfig("Misc", "Radar") == "true";
                AimbotPistolsLockTime.Value     = double.Parse(ReadIniConfig("Aim", "AimbotPistolsLockTime"));
                Spectators.IsChecked            = ReadIniConfig("Visuals", "Spectators") == "true";
                Brightness.IsChecked            = ReadIniConfig("Misc", "Brightness") == "true";
                FovSnipers.IsChecked            = ReadIniConfig("Aim", "FovSnipers") == "true";
                TriggerbotPistolsDelay.Value    = double.Parse(ReadIniConfig("Aim", "TriggerbotPistolsDelay"));
                AimbotSpeed.Value               = double.Parse(ReadIniConfig("Aim", "AimbotSpeed"));
                AimbotPistols.IsChecked         = ReadIniConfig("Aim", "AimbotPistols") == "true";
                AimbotBone.SelectedIndex        = int.Parse(ReadIniConfig("Aim", "AimbotBone"));
                AutoPistol.IsChecked            = ReadIniConfig("Aim", "AutoPistol") == "true";
                FriendlyFire.IsChecked          = ReadIniConfig("Settings", "FriendlyFire") == "true";
                Knifebot.IsChecked              = ReadIniConfig("Aim", "Knifebot") == "true";
                FakeLag.IsChecked               = ReadIniConfig("Misc", "FakeLag") == "true";
                BrightnessStrength.Value        = double.Parse(ReadIniConfig("Misc", "BrightnessStrength"));
                ForceUpdateKey.SelectedIndex    = int.Parse(ReadIniConfig("Settings", "ForceUpdateKey"));
                FpsLimit.Value                  = double.Parse(ReadIniConfig("Settings", "FpsLimit"));
                SimpleRanks.IsChecked           = ReadIniConfig("Settings", "SimpleRanks") == "true";
                Snaplines.IsChecked             = ReadIniConfig("Visuals", "Snaplines") == "true";
                GlowDelay.Value                 = double.Parse(ReadIniConfig("Settings", "GlowDelay"));
                PathLength.Value                = double.Parse(ReadIniConfig("Visuals", "PathLength"));
                TriggerbotPistols.IsChecked     = ReadIniConfig("Aim", "TriggerbotPistols") == "true";
                TriggerbotSnipersDelay.Value    = double.Parse(ReadIniConfig("Aim", "TriggerbotSnipersDelay"));
                PathT.IsChecked                 = ReadIniConfig("Visuals", "PathT") == "true";
                HideChickens.IsChecked          = ReadIniConfig("Visuals", "HideChickens") == "true";
                FovSize.Value                   = double.Parse(ReadIniConfig("Aim", "FovSize"));           
                ActiveWeapon.IsChecked          = ReadIniConfig("Visuals", "ActiveWeapon") == "true";
                Name.IsChecked                  = ReadIniConfig("Visuals", "Name") == "true";
                PlayersChams.IsChecked          = ReadIniConfig("Visuals", "PlayersChams") == "true";
                TriggerbotDelay.Value           = double.Parse(ReadIniConfig("Aim", "TriggerbotDelay"));
                HeadHelperSize.Value            = double.Parse(ReadIniConfig("Visuals", "HeadHelperSize"));
                RcsStrengthY.Value              = double.Parse(ReadIniConfig("Aim", "RcsStrengthY"));
                Rcs.IsChecked                   = ReadIniConfig("Aim", "Rcs") == "true";
                Fov.IsChecked                   = ReadIniConfig("Aim", "Fov") == "true";
                RcsPistol.IsChecked             = ReadIniConfig("Settings", "RcsPistol") == "true";
                TriggerbotSnipers.IsChecked     = ReadIniConfig("Aim", "TriggerbotSnipers") == "true";
                FovChanger.IsChecked            = ReadIniConfig("Misc", "FovChanger") == "true";
                Triggerbot.IsChecked            = ReadIniConfig("Aim", "Triggerbot") == "true";
                AdditionalInfo.IsChecked        = ReadIniConfig("Visuals", "AdditionalInfo") == "true";
                AimbotSnipersSpeed.Value        = double.Parse(ReadIniConfig("Aim", "AimbotSnipersSpeed"));
                PathQuality.Value               = double.Parse(ReadIniConfig("Settings", "PathQuality"));
                FovChangerValue.Value           = double.Parse(ReadIniConfig("Misc", "FovChangerValue"));
                SoundEspLengthScale.Value       = double.Parse(ReadIniConfig("Sound", "SoundEspLengthScale"), NumberStyles.AllowDecimalPoint);
                Aimbot.IsChecked                = ReadIniConfig("Aim", "Aimbot") == "true";
                SoundEspFovSize.Value           = double.Parse(ReadIniConfig("Sound", "SoundEspFovSize"));
                AimbotNonSticky.IsChecked       = ReadIniConfig("Settings", "AimbotNonSticky") == "true";
                AimbotSnipersBone.SelectedIndex = int.Parse(ReadIniConfig("Aim", "AimbotSnipersBone"));
                AimbotVisibilityCheck.IsChecked = ReadIniConfig("Settings", "AimbotVisibilityCheck") == "true";
                Players3D.IsChecked             = ReadIniConfig("Visuals", "Players3D") == "true";
                AimbotSnipers.IsChecked         = ReadIniConfig("Aim", "AimbotSnipers") == "true";
                FakeLagPause.Value              = double.Parse(ReadIniConfig("Misc", "FakeLagPause"));
                TriggerbotKey.SelectedIndex     = int.Parse(ReadIniConfig("Settings", "TriggerbotKey"));
                SoundEsp.IsChecked              = ReadIniConfig("Sound", "SoundEsp") == "true";
                FovPistolsSize.Value            = double.Parse(ReadIniConfig("Aim", "FovPistolsSize"));
                FakeLagLength.Value             = double.Parse(ReadIniConfig("Misc", "FakeLagLength"));
                PlayersHealthGlow.IsChecked     = ReadIniConfig("Settings", "PlayersHealthGlow") == "true";
                AimbotPistolsBone.SelectedIndex = int.Parse(ReadIniConfig("Aim", "AimbotPistolsBone"));
                StatTrak.IsChecked              = ReadIniConfig("Misc", "StatTrak") == "true";
                Skeleton.IsChecked              = ReadIniConfig("Visuals", "Skeleton") == "true";
                PathLocalOnly.IsChecked         = ReadIniConfig("Settings", "PathLocalOnly") == "true";
                AimbotSnipersLockTime.Value     = double.Parse(ReadIniConfig("Aim", "AimbotSnipersLockTime"));
                HideAllies.IsChecked            = ReadIniConfig("Visuals", "HideAllies") == "true";
                Antialiasing.IsChecked          = ReadIniConfig("Settings", "Antialiasing") == "true";
                Floor.IsChecked                 = ReadIniConfig("Visuals", "Floor") == "true";
                RcsStrengthX.Value              = double.Parse(ReadIniConfig("Aim", "RcsStrengthX"));
                Rank.IsChecked                  = ReadIniConfig("Visuals", "Rank") == "true";
                HeadHelper.IsChecked            = ReadIniConfig("Visuals", "HeadHelper") == "true";
                RecoilCrosshair.IsChecked       = ReadIniConfig("Visuals", "RecoilCrosshair") == "true";
                HealthBar.IsChecked             = ReadIniConfig("Visuals", "HealthBar") == "true";
                //[/swap]
            }
            catch (Exception ex)
            {
                var result = MessageBox.Show("One or more settings couldn't be loaded\nDo you want to restore default config?", Rnd.GetRandomString(), MessageBoxButton.YesNo);

                switch (result)
                {
                    case MessageBoxResult.Yes:
                        File.Delete("config.ini");
                        CreateIniConfig();
                        break;
                }
            }

            RefreshAllToggles();
        }

        private static string ReadIniConfig(string cat, string name)
        {
            var value = G.Config.Get(cat, name).Replace('.', ',');

            if (value != string.Empty)
                return value;

            MessageBox.Show($"Setting couldn't be loaded\nSection: {cat}\nName: {name}");
            return "0";
        }

        private void SaveIniConfig(object sender, RoutedEventArgs e)
        {
            //[swap]
            G.Config.Set("Visuals", "HideChickens", HideChickens.IsChecked == true ? "true" : "false");
            G.Config.Set("Aim", "AimbotSnipersSpeed", AimbotSnipersSpeed.Value.ToString("F"));
            G.Config.Set("Visuals", "AdditionalInfo", AdditionalInfo.IsChecked == true ? "true" : "false");
            G.Config.Set("Misc", "Bunnyhop", Bunnyhop.IsChecked == true ? "true" : "false");
            G.Config.Set("Visuals", "HealthBar", HealthBar.IsChecked == true ? "true" : "false");
            G.Config.Set("Visuals", "HeadHelper", HeadHelper.IsChecked == true ? "true" : "false");
            G.Config.Set("Visuals", "Players2D", Players2D.IsChecked == true ? "true" : "false");
            G.Config.Set("Misc", "FakeLag", FakeLag.IsChecked == true ? "true" : "false");
            G.Config.Set("Misc", "AntiFlash", AntiFlash.IsChecked == true ? "true" : "false");
            G.Config.Set("Aim", "TriggerbotSnipersDelay", TriggerbotSnipersDelay.Value.ToString("F"));
            G.Config.Set("Misc", "BrightnessStrength", BrightnessStrength.Value.ToString("F"));
            G.Config.Set("Settings", "RcsPistol", RcsPistol.IsChecked == true ? "true" : "false");
            G.Config.Set("Misc", "FovChangerValue", FovChangerValue.Value.ToString("F"));
            G.Config.Set("Aim", "AimbotBone", AimbotBone.SelectedIndex.ToString());
            G.Config.Set("Misc", "FakeLagPause", FakeLagPause.Value.ToString("F"));
            G.Config.Set("Aim", "FovSnipersSize", FovSnipersSize.Value.ToString("F"));
            G.Config.Set("Aim", "FovPistolsSize", FovPistolsSize.Value.ToString("F"));
            G.Config.Set("Settings", "AimbotKey", AimbotKey.SelectedIndex.ToString());
            G.Config.Set("Sound", "SoundEspFovSize", SoundEspFovSize.Value.ToString("F"));
            G.Config.Set("Misc", "Brightness", Brightness.IsChecked == true ? "true" : "false");
            G.Config.Set("Settings", "FpsLimit", FpsLimit.Value.ToString());
            G.Config.Set("Aim", "FovPistols", FovPistols.IsChecked == true ? "true" : "false");
            G.Config.Set("Aim", "RcsStrengthY", RcsStrengthY.Value.ToString("F"));
            G.Config.Set("Sound", "SoundEsp", SoundEsp.IsChecked == true ? "true" : "false");
            G.Config.Set("Aim", "AimbotSnipersLockTime", AimbotSnipersLockTime.Value.ToString("F"));
            G.Config.Set("Visuals", "Spectators", Spectators.IsChecked == true ? "true" : "false");
            G.Config.Set("Settings", "ToggleKey", ToggleKey.SelectedIndex.ToString());
            G.Config.Set("Aim", "AimbotSnipersBone", AimbotSnipersBone.SelectedIndex.ToString());
            G.Config.Set("Aim", "FovSnipers", FovSnipers.IsChecked == true ? "true" : "false");
            G.Config.Set("Settings", "FullGlow", FullGlow.IsChecked == true ? "true" : "false");
            G.Config.Set("Aim", "Fov", Fov.IsChecked == true ? "true" : "false");
            G.Config.Set("Visuals", "RecoilCrosshair", RecoilCrosshair.IsChecked == true ? "true" : "false");
            G.Config.Set("Visuals", "Snaplines", Snaplines.IsChecked == true ? "true" : "false");
            G.Config.Set("Visuals", "PathLength", PathLength.Value.ToString("F"));
            G.Config.Set("Visuals", "Skeleton", Skeleton.IsChecked == true ? "true" : "false");
            G.Config.Set("Aim", "AimbotPistolsBone", AimbotPistolsBone.SelectedIndex.ToString());
            G.Config.Set("Settings", "FriendlyFire", FriendlyFire.IsChecked == true ? "true" : "false");
            G.Config.Set("Settings", "RcsPositionFix", RcsPositionFix.IsChecked == true ? "true" : "false");
            G.Config.Set("Misc", "FovChanger", FovChanger.IsChecked == true ? "true" : "false");
            G.Config.Set("Aim", "FovSize", FovSize.Value.ToString("F"));
            G.Config.Set("Visuals", "Name", Name.IsChecked == true ? "true" : "false");
            G.Config.Set("Aim", "AimbotSnipers", AimbotSnipers.IsChecked == true ? "true" : "false");
            G.Config.Set("Aim", "RcsStrengthX", RcsStrengthX.Value.ToString("F"));
            G.Config.Set("Settings", "PathLocalOnly", PathLocalOnly.IsChecked == true ? "true" : "false");
            G.Config.Set("Visuals", "Players3D", Players3D.IsChecked == true ? "true" : "false");
            G.Config.Set("Aim", "Aimbot", Aimbot.IsChecked == true ? "true" : "false");
            G.Config.Set("Settings", "ForceUpdateKey", ForceUpdateKey.SelectedIndex.ToString());
            G.Config.Set("Visuals", "Entities3D", Entities3D.IsChecked == true ? "true" : "false");
            G.Config.Set("Aim", "TriggerbotDelay", TriggerbotDelay.Value.ToString("F"));
            G.Config.Set("Aim", "AimbotPistolsSpeed", AimbotPistolsSpeed.Value.ToString("F"));
            G.Config.Set("Sound", "SoundEspLengthScale", SoundEspLengthScale.Value.ToString("F"));
            G.Config.Set("Aim", "Rcs", Rcs.IsChecked == true ? "true" : "false");
            G.Config.Set("Settings", "RcsAimbotOnly", RcsAimbotOnly.IsChecked == true ? "true" : "false");
            G.Config.Set("Aim", "AimbotLockTime", AimbotLockTime.Value.ToString("F"));
            G.Config.Set("Settings", "SimpleRanks", SimpleRanks.IsChecked == true ? "true" : "false");
            G.Config.Set("Settings", "Antialiasing", Antialiasing.IsChecked == true ? "true" : "false");
            G.Config.Set("Settings", "AimbotNonSticky", AimbotNonSticky.IsChecked == true ? "true" : "false");
            G.Config.Set("Aim", "TriggerbotSnipers", TriggerbotSnipers.IsChecked == true ? "true" : "false");
            G.Config.Set("Aim", "TriggerbotPistols", TriggerbotPistols.IsChecked == true ? "true" : "false");
            G.Config.Set("Settings", "VisibilityCheck", VisibilityCheck.IsChecked == true ? "true" : "false");
            G.Config.Set("Visuals", "HideAllies", HideAllies.IsChecked == true ? "true" : "false");
            G.Config.Set("Settings", "GlowDelay", GlowDelay.Value.ToString());
            G.Config.Set("Visuals", "PathT", PathT.IsChecked == true ? "true" : "false");
            G.Config.Set("Settings", "TriggerbotKey", TriggerbotKey.SelectedIndex.ToString());
            G.Config.Set("Aim", "Triggerbot", Triggerbot.IsChecked == true ? "true" : "false");
            G.Config.Set("Visuals", "HideEnemies", HideEnemies.IsChecked == true ? "true" : "false");
            G.Config.Set("Aim", "AimbotPistolsLockTime", AimbotPistolsLockTime.Value.ToString("F"));
            G.Config.Set("Misc", "Radar", Radar.IsChecked == true ? "true" : "false");
            G.Config.Set("Settings", "PlayersHealthGlow", PlayersHealthGlow.IsChecked == true ? "true" : "false");
            G.Config.Set("Aim", "TriggerbotPistolsDelay", TriggerbotPistolsDelay.Value.ToString("F"));
            G.Config.Set("Visuals", "Entities2D", Entities2D.IsChecked == true ? "true" : "false");
            G.Config.Set("Aim", "Knifebot", Knifebot.IsChecked == true ? "true" : "false");
            G.Config.Set("Visuals", "PlayersChams", PlayersChams.IsChecked == true ? "true" : "false");
            G.Config.Set("Visuals", "Floor", Floor.IsChecked == true ? "true" : "false");
            G.Config.Set("Misc", "SkinChanger", SkinChanger.IsChecked == true ? "true" : "false");
            G.Config.Set("Aim", "AimbotPistols", AimbotPistols.IsChecked == true ? "true" : "false");
            G.Config.Set("Visuals", "PlayersGlow", PlayersGlow.IsChecked == true ? "true" : "false");
            G.Config.Set("Aim", "AimbotSpeed", AimbotSpeed.Value.ToString("F"));
            G.Config.Set("Settings", "AimbotVisibilityCheck", AimbotVisibilityCheck.IsChecked == true ? "true" : "false");
            G.Config.Set("Settings", "ExitKey", ExitKey.SelectedIndex.ToString());
            G.Config.Set("Aim", "AutoPistol", AutoPistol.IsChecked == true ? "true" : "false");
            G.Config.Set("Visuals", "HeadHelperSize", HeadHelperSize.Value.ToString("F"));
            G.Config.Set("Misc", "FakeLagLength", FakeLagLength.Value.ToString("F"));
            G.Config.Set("Misc", "StatTrak", StatTrak.IsChecked == true ? "true" : "false");
            G.Config.Set("Settings", "RecoilBasedFov", RecoilBasedFov.IsChecked == true ? "true" : "false");
            G.Config.Set("Visuals", "ActiveWeapon", ActiveWeapon.IsChecked == true ? "true" : "false");
            G.Config.Set("Visuals", "Rank", Rank.IsChecked == true ? "true" : "false");
            G.Config.Set("Settings", "PathQuality", PathQuality.Value.ToString("F"));
            G.Config.Set("Sound", "SoundEspFrequency", SoundEspFrequency.Value.ToString("F"));
            //[/swap]
        }

        private static void CreateSkinsConfig()
        {
            for (int i = 0; i < 65; i++)
            {
                if (!i.HasSkin())
                    continue;

                var name = i.GetWeaponName();
                //[swap]
                G.SkinConfig.Set("Wear", name, "0");
                G.SkinConfig.Set("Prefix", name, "0");
                G.SkinConfig.Set("StatTrak", name, "-1");
                G.SkinConfig.Set("Name", name, string.Empty);
                G.SkinConfig.Set("ID", name, "0");
                //[/swap]
            }
        }

        private static void LoadSkinsConfig()
        {
            if(G.Skins.Count != 0)
                G.Skins.Clear();

            try
            {
                for (int i = 0; i < 65; i++)
                {
                    if (!i.HasSkin())
                        continue;

                    var tempSkin = new Skin();
                    var name = i.GetWeaponName();
                    //[swap]
                    tempSkin.ID = int.Parse(G.SkinConfig.Get("ID", name));
                    tempSkin.Wear = float.Parse(G.SkinConfig.Get("Wear", name));
                    tempSkin.WeaponID = i;
                    tempSkin.Name = G.SkinConfig.Get("Name", name);
                    tempSkin.Prefix = int.Parse(G.SkinConfig.Get("Prefix", name));
                    tempSkin.StatTrak = int.Parse(G.SkinConfig.Get("StatTrak", name));
                    //[/swap]
                    G.Skins.Add(tempSkin);
                }
            }
            catch
            {
                File.Delete("skins.ini");
                CreateSkinsConfig();
                LoadSkinsConfig();
            }
        }

        private static void SaveSkinsConfig()
        {
            foreach (var skin in G.Skins)
            {
                var name = skin.WeaponID.GetWeaponName();
                //[swap]
                G.SkinConfig.Set("Prefix", name, skin.Prefix.ToString());
                G.SkinConfig.Set("ID", name, skin.ID.ToString());
                G.SkinConfig.Set("StatTrak", name, skin.StatTrak.ToString());
                G.SkinConfig.Set("Wear", name, skin.Wear.ToString());
                G.SkinConfig.Set("Name", name, skin.Name);
                //[/swap]
            }
        }

        private void ConfigShareGet(object sender, RoutedEventArgs e)
        {
            try
            {
                if (string.IsNullOrWhiteSpace(configId.Text))
                    return;

                //[swap]
                WebClient wc = new WebClient();
                var url = $"https://www.mpgh.net";
                //[/swap]

                wc.Proxy = null;
                var response = wc.DownloadString(url);

                //[swap]
                wc.Dispose();
                var config = GetBetween(response, "<return>", "</return>", 0);
                //[/swap]

                if (config == "null")
                {
                    MessageBox.Show("Config not found", Rnd.GetRandomString(), MessageBoxButton.OK);
                }
                else
                {
                    File.WriteAllText(G.Config.Path, config);
                    LoadIniConfig(null, null);

                    MessageBox.Show("Done!", Rnd.GetRandomString(), MessageBoxButton.OK);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("An error has occured\nPlease try again", Rnd.GetRandomString(), MessageBoxButton.OK);
            }
        }

        private void ConfigShareShare(object sender, RoutedEventArgs e)
        {
            try
            {
                if (string.IsNullOrWhiteSpace(configName.Text))
                    return;

                if (!configName.Text.All(char.IsLetterOrDigit))
                {
                    MessageBox.Show("Cofig name must be alphanumeric", Rnd.GetRandomString(), MessageBoxButton.OK);
                    return;
                }

                SaveIniConfig(null, null);
                var config = File.ReadAllText(G.Config.Path);

                var configBytes = Encoding.Default.GetBytes(config);
                var config64 = Convert.ToBase64String(configBytes);

                //[swap]
                WebClient wc = new WebClient();
                var url = $"https://www.mpgh.net";
                //[/swap]

                wc.Proxy = null;
                var response = wc.DownloadString(url);

                //[swap]
                var id = GetBetween(response, "<return>", "</return>", 0);
                wc.Dispose();
                //[/swap]

                id = id.Replace('$', '#');

                Clipboard.SetText(id);
                MessageBox.Show("Your ConfigShare ID: " + id, Rnd.GetRandomString(), MessageBoxButton.OK);
            }
            catch (Exception ex)
            {
                MessageBox.Show("An error has occured\nPlease try again", Rnd.GetRandomString(), MessageBoxButton.OK);
            }
        }

        private static string GetBetween(string source, string p1, string p2, int offset)
        {
            try
            {
                var split1 = source.Split(new[] {p1}, StringSplitOptions.RemoveEmptyEntries)[offset + 1];
                var split2 = split1.Split(new[] {p2}, StringSplitOptions.RemoveEmptyEntries)[0];
                return split2;
            }
            catch
            {
                return string.Empty;
            }
        }

        private void ToggleManager(object sender, RoutedEventArgs e)
        {
            var obj = sender as ToggleSwitch;
            var value = obj.IsChecked == true;

            switch (obj.Name)
            {
                //[swap]
                //[block]
                case "Bunnyhop":
                    Main.Bunnyhop = value;
                    return;
                //[/block]
                //[block]
                case "Snaplines":
                    Main.Snaplines = value;
                    return;
                //[/block]
                //[block]
                case "Entities3D":
                    Main.Entities3D = value;
                    return;
                //[/block]
                //[block]
                case "AimbotPistols":
                    Main.AimbotPistols = value;
                    return;
                //[/block]
                //[block]
                case "HeadHelper":
                    Main.HeadHelper = value;
                    return;
                //[/block]
                //[block]
                case "RcsPistol":
                    Main.RcsPistol = value;
                    return;
                //[/block]
                //[block]
                case "HideEnemies":
                    Main.HideEnemies = value;
                    return;
                //[/block]
                //[block]
                case "Name":
                    Main.Name = value;
                    return;
                //[/block]
                //[block]
                case "ActiveWeapon":
                    Main.ActiveWeapon = value;
                    return;
                //[/block]
                //[block]
                case "Brightness":
                    Main.Brightness = value;
                    return;
                //[/block]
                //[block]
                case "HideChickens":
                    Main.HideChickens = value;
                    return;
                //[/block]
                //[block]
                case "StatTrak":
                    Main.StatTrak = value;
                    return;
                //[/block]
                //[block]
                case "PlayersGlow":
                    Main.PlayersGlow = value;
                    return;
                //[/block]
                //[block]
                case "AimbotSnipers":
                    Main.AimbotSnipers = value;
                    return;
                //[/block]
                //[block]
                case "AdditionalInfo":
                    Main.ExtendedInfo = value;
                    return;
                //[/block]
                //[block]
                case "Aimbot":
                    Main.Aimbot = value;
                    return;
                //[/block]
                //[block]
                case "AimbotVisibilityCheck":
                    Main.AimbotVisibilityCheck = value;
                    return;
                //[/block]
                //[block]
                case "FovSnipers":
                    Main.FovSnipers = value;
                    return;
                //[/block]
                //[block]
                case "VisibilityCheck":
                    Main.VisibilityCheck = value;
                    return;
                //[/block]
                //[block]
                case "Antialiasing":
                    G.Fade.Overlay.Form.Antialiasing = value;
                    return;
                //[/block]
                //[block]
                case "Fov":
                    Main.Fov = value;
                    return;
                //[/block]
                //[block]
                case "AutoPistol":
                    Main.AutoPistol = value;
                    return;
                //[/block]
                //[block]
                case "HealthBar":
                    Main.HealthBar = value;
                    return;
                //[/block]
                //[block]
                case "TriggerbotSnipers":
                    Main.TriggerbotSnipers = value;
                    return;
                //[/block]
                //[block]
                case "Knifebot":
                    Main.Knifebot = value;
                    return;
                //[/block]
                //[block]
                case "RcsAimbotOnly":
                    Main.RcsAimbotOnly = value;
                    return;
                //[/block]
                //[block]
                case "FriendlyFire":
                    Main.FriendlyFire = value;
                    return;
                //[/block]
                //[block]
                case "Floor":
                    Main.Floor = value;
                    return;
                //[/block]
                //[block]
                case "RecoilCrosshair":
                    Main.RecoilCrosshair = value;
                    return;
                //[/block]
                //[block]
                case "Players3D":
                    Main.Players3D = value;
                    return;
                //[/block]
                //[block]
                case "Rank":
                    Main.Rank = value;
                    return;
                //[/block]
                //[block]
                case "FovChanger":
                    Main.FovChanger = value;
                    return;
                //[/block]
                //[block]
                case "AntiFlash":
                    Main.AntiFlash = value;
                    return;
                //[/block]
                //[block]
                case "Skeleton":
                    Main.Skeleton = value;
                    return;
                //[/block]
                //[block]
                case "SkinChanger":
                    Main.SkinChanger = value;
                    return;
                //[/block]
                //[block]
                case "Spectators":
                    Main.Spectators = value;
                    return;
                //[/block]
                //[block]
                case "PlayersChams":
                    Main.PlayersChams = value;
                    return;
                //[/block]
                //[block]
                case "PlayersHealthGlow":
                    Main.PlayersHealthGlow = value;
                    return;
                //[/block]
                //[block]
                case "Radar":
                    Main.Radar = value;
                    return;
                //[/block]
                //[block]
                case "RcsPositionFix":
                    Main.RcsPositionFix = value;
                    return;
                //[/block]
                //[block]
                case "Entities2D":
                    Main.Entities2D = value;
                    return;
                //[/block]
                //[block]
                case "PathT":
                    Main.PathT = value;
                    return;
                //[/block]
                //[block]
                case "FovPistols":
                    Main.FovPistols = value;
                    return;
                //[/block]
                //[block]
                case "SimpleRanks":
                    Main.SimpleRanks = value;
                    return;
                //[/block]
                //[block]
                case "Rcs":
                    Main.Rcs = value;
                    return;
                //[/block]
                //[block]
                case "FullGlow":
                    Main.FullGlow = value;
                    return;
                //[/block]
                //[block]
                case "FakeLag":
                    Main.FakeLag = value;
                    return;
                //[/block]
                //[block]
                case "RecoilBasedFov":
                    Main.RecoilBasedFov = value;
                    return;
                //[/block]
                //[block]
                case "HideAllies":
                    Main.HideAllies = value;
                    return;
                //[/block]
                //[block]
                case "PathLocalOnly":
                    Main.PathLocalOnly = value;
                    return;
                //[/block]
                //[block]
                case "Players2D":
                    Main.Players2D = value;
                    return;
                //[/block]
                //[block]
                case "TriggerbotPistols":
                    Main.TriggerbotPistols = value;
                    return;
                //[/block]
                //[block]
                case "Triggerbot":
                    Main.Triggerbot = value;
                    return;
                //[/block]
                //[block]
                case "AimbotNonSticky":
                    Main.AimbotNonSticky = value;
                    return;
                //[/block]
                //[block]
                case "SoundEsp":
                    Main.SoundEsp = value;
                    return;
                //[/block]
                //[/swap]
            }
        }

        private void SliderManager(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            var obj = sender as Slider;
            var value = obj.Value;

            switch (obj.Name)
            {
                //[swap]
                //[block]
                case "ChamsAlliesR":
                    {
                        byte b = ((SolidColorBrush)ChamsAlliesColor.Background).Color.B;
                        byte g = ((SolidColorBrush)ChamsAlliesColor.Background).Color.G;
                        Main.ChamsAlliesColor = new RawColor4((byte)value / 255f, g / 255f, b / 255f, 1f);
                        ChamsAlliesColor.Background = new SolidColorBrush(Color.FromRgb((byte)value, g, b));
                    }
                    return;
                //[/block]
                //[block]
                case "FovG":
                    {
                        byte b = ((SolidColorBrush)FovColor.Background).Color.B;
                        byte r = ((SolidColorBrush)FovColor.Background).Color.R;
                        Main.FovColor = new RawColor4(r / 255f, (byte)value / 255f, b / 255f, 1f);
                        FovColor.Background = new SolidColorBrush(Color.FromRgb(r, (byte)value, b));
                    }
                    return;
                //[/block]
                //[block]
                case "SkinWear":
                    foreach (var skin in G.Skins)
                        if (skin.WeaponID.GetWeaponName() == SkinWeapon.Text)
                            skin.Wear = (float)value;

                    SaveSkinsConfig();
                    return;
                //[/block]
                //[block]
                case "ChickensR":
                    {
                        byte g = ((SolidColorBrush)ChickensColor.Background).Color.G;
                        byte b = ((SolidColorBrush)ChickensColor.Background).Color.B;
                        ChickensColor.Background = new SolidColorBrush(Color.FromRgb((byte)value, g, b));
                        Main.ChickensColor = new RawColor4((byte)value / 255f, g / 255f, b / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "FovR":
                    {
                        byte g = ((SolidColorBrush)FovColor.Background).Color.G;
                        byte b = ((SolidColorBrush)FovColor.Background).Color.B;
                        Main.FovColor = new RawColor4((byte)value / 255f, g / 255f, b / 255f, 1f);
                        FovColor.Background = new SolidColorBrush(Color.FromRgb((byte)value, g, b));
                    }
                    return;
                //[/block]
                //[block]
                case "SpecialEntitiesG":
                    {
                        byte b = ((SolidColorBrush)SpecialEntitiesColor.Background).Color.B;
                        byte r = ((SolidColorBrush)SpecialEntitiesColor.Background).Color.R;
                        SpecialEntitiesColor.Background = new SolidColorBrush(Color.FromRgb(r, (byte)value, b));
                        Main.SpecialEntitiesColor = new RawColor4(r / 255f, (byte)value / 255f, b / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "EnemiesVisB":
                    {
                        byte r = ((SolidColorBrush)EnemiesVisColor.Background).Color.R;
                        byte g = ((SolidColorBrush)EnemiesVisColor.Background).Color.G;
                        Main.EnemiesVisColor = new RawColor4(r / 255f, g / 255f, (byte)value / 255f, 1f);
                        EnemiesVisColor.Background = new SolidColorBrush(Color.FromRgb(r, g, (byte)value));
                    }
                    return;
                //[/block]
                //[block]
                case "SkeletonEnemiesB":
                    {
                        byte r = ((SolidColorBrush)SkeletonEnemiesColor.Background).Color.R;
                        byte g = ((SolidColorBrush)SkeletonEnemiesColor.Background).Color.G;
                        SkeletonEnemiesColor.Background = new SolidColorBrush(Color.FromRgb(r, g, (byte)value));
                        Main.SkeletonEnemiesColor = new RawColor4(r / 255f, g / 255f, (byte)value / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "FovPistolsSize":
                    Main.FovPistolsSize = (float)value;
                    return;
                //[/block]
                //[block]
                case "PathQuality":
                    Main.PathQuality = (float)obj.Maximum - (float)value;
                    return;
                //[/block]
                //[block]
                case "ActiveWeaponB":
                    {
                        byte r = ((SolidColorBrush)ActiveWeaponColor.Background).Color.R;
                        byte g = ((SolidColorBrush)ActiveWeaponColor.Background).Color.G;
                        Main.ActiveWeaponColor = new RawColor4(r / 255f, g / 255f, (byte)value / 255f, 1f);
                        ActiveWeaponColor.Background = new SolidColorBrush(Color.FromRgb(r, g, (byte)value));
                    }
                    return;
                //[/block]
                //[block]
                case "EnemiesVisR":
                    {
                        byte b = ((SolidColorBrush)EnemiesVisColor.Background).Color.B;
                        byte g = ((SolidColorBrush)EnemiesVisColor.Background).Color.G;
                        Main.EnemiesVisColor = new RawColor4((byte)value / 255f, g / 255f, b / 255f, 1f);
                        EnemiesVisColor.Background = new SolidColorBrush(Color.FromRgb((byte)value, g, b));
                    }
                    return;
                //[/block]
                //[block]
                case "SkeletonEnemiesG":
                    {
                        byte r = ((SolidColorBrush)SkeletonEnemiesColor.Background).Color.R;
                        byte b = ((SolidColorBrush)SkeletonEnemiesColor.Background).Color.B;
                        SkeletonEnemiesColor.Background = new SolidColorBrush(Color.FromRgb(r, (byte)value, b));
                        Main.SkeletonEnemiesColor = new RawColor4(r / 255f, (byte)value / 255f, b / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "HeadHelperSize":
                    Main.HeadHelperSize = (float)value;
                    return;
                //[/block]
                //[block]
                case "SpectatorsB":
                    {
                        byte g = ((SolidColorBrush)SpectatorsColor.Background).Color.G;
                        byte r = ((SolidColorBrush)SpectatorsColor.Background).Color.R;
                        SpectatorsColor.Background = new SolidColorBrush(Color.FromRgb(r, g, (byte)value));
                        Main.SpectatorsColor = new RawColor4(r / 255f, g / 255f, (byte)value / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "EnemiesR":
                    {
                        byte g = ((SolidColorBrush)EnemiesColor.Background).Color.G;
                        byte b = ((SolidColorBrush)EnemiesColor.Background).Color.B;
                        EnemiesColor.Background = new SolidColorBrush(Color.FromRgb((byte)value, g, b));
                        Main.EnemiesColor = new RawColor4((byte)value / 255f, g / 255f, b / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "EnemiesB":
                    {
                        byte r = ((SolidColorBrush)EnemiesColor.Background).Color.R;
                        byte g = ((SolidColorBrush)EnemiesColor.Background).Color.G;
                        EnemiesColor.Background = new SolidColorBrush(Color.FromRgb(r, g, (byte)value));
                        Main.EnemiesColor = new RawColor4(r / 255f, g / 255f, (byte)value / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "RecoilCrosshairR":
                    {
                        byte b = ((SolidColorBrush)RecoilCrosshairColor.Background).Color.B;
                        byte g = ((SolidColorBrush)RecoilCrosshairColor.Background).Color.G;
                        Main.RecoilCrosshairColor = new RawColor4((byte)value / 255f, g / 255f, b / 255f, 1f);
                        RecoilCrosshairColor.Background = new SolidColorBrush(Color.FromRgb((byte)value, g, b));
                    }
                    return;
                //[/block]
                //[block]
                case "SkeletonEnemiesVisR":
                    {
                        byte b = ((SolidColorBrush)SkeletonEnemiesVisColor.Background).Color.B;
                        byte g = ((SolidColorBrush)SkeletonEnemiesVisColor.Background).Color.G;
                        SkeletonEnemiesVisColor.Background = new SolidColorBrush(Color.FromRgb((byte)value, g, b));
                        Main.SkeletonEnemiesVisColor = new RawColor4((byte)value / 255f, g / 255f, b / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "SkeletonAlliesB":
                    {
                        byte g = ((SolidColorBrush)SkeletonAlliesColor.Background).Color.G;
                        byte r = ((SolidColorBrush)SkeletonAlliesColor.Background).Color.R;
                        SkeletonAlliesColor.Background = new SolidColorBrush(Color.FromRgb(r, g, (byte)value));
                        Main.SkeletonAlliesColor = new RawColor4(r / 255f, g / 255f, (byte)value / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "NamesG":
                    {
                        byte r = ((SolidColorBrush)NamesColor.Background).Color.R;
                        byte b = ((SolidColorBrush)NamesColor.Background).Color.B;
                        Main.NamesColor = new RawColor4(r / 255f, (byte)value / 255f, b / 255f, 1f);
                        NamesColor.Background = new SolidColorBrush(Color.FromRgb(r, (byte)value, b));
                    }
                    return;
                //[/block]
                //[block]
                case "BackgroundB":
                    {
                        byte r = ((SolidColorBrush)BackgroundColor.Background).Color.R;
                        byte g = ((SolidColorBrush)BackgroundColor.Background).Color.G;
                        Main.BackgroundColor = new RawColor4(r / 255f, g / 255f, (byte)value / 255f, 1f);
                        BackgroundColor.Background = new SolidColorBrush(Color.FromRgb(r, g, (byte)value));
                    }
                    return;
                //[/block]
                //[block]
                case "HeadHelperR":
                    {
                        byte g = ((SolidColorBrush)HeadHelperColor.Background).Color.G;
                        byte b = ((SolidColorBrush)HeadHelperColor.Background).Color.B;
                        HeadHelperColor.Background = new SolidColorBrush(Color.FromRgb((byte)value, g, b));
                        Main.HeadHelperColor = new RawColor4((byte)value / 255f, g / 255f, b / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "ChamsAlliesG":
                    {
                        byte r = ((SolidColorBrush)ChamsAlliesColor.Background).Color.R;
                        byte b = ((SolidColorBrush)ChamsAlliesColor.Background).Color.B;
                        ChamsAlliesColor.Background = new SolidColorBrush(Color.FromRgb(r, (byte)value, b));
                        Main.ChamsAlliesColor = new RawColor4(r / 255f, (byte)value / 255f, b / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "ChickensB":
                    {
                        byte g = ((SolidColorBrush)ChickensColor.Background).Color.G;
                        byte r = ((SolidColorBrush)ChickensColor.Background).Color.R;
                        ChickensColor.Background = new SolidColorBrush(Color.FromRgb(r, g, (byte)value));
                        Main.ChickensColor = new RawColor4(r / 255f, g / 255f, (byte)value / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "AlliesG":
                    {
                        byte b = ((SolidColorBrush)AlliesColor.Background).Color.B;
                        byte r = ((SolidColorBrush)AlliesColor.Background).Color.R;
                        AlliesColor.Background = new SolidColorBrush(Color.FromRgb(r, (byte)value, b));
                        Main.AlliesColor = new RawColor4(r / 255f, (byte)value / 255f, b / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "FakeLagLength":
                    Main.FakeLagLength = (float)value;
                    return;
                //[/block]
                //[block]
                case "HeadHelperB":
                    {
                        byte r = ((SolidColorBrush)HeadHelperColor.Background).Color.R;
                        byte g = ((SolidColorBrush)HeadHelperColor.Background).Color.G;
                        Main.HeadHelperColor = new RawColor4(r / 255f, g / 255f, (byte)value / 255f, 1f);
                        HeadHelperColor.Background = new SolidColorBrush(Color.FromRgb(r, g, (byte)value));
                    }
                    return;
                //[/block]
                //[block]
                case "HeadHelperG":
                    {
                        byte b = ((SolidColorBrush)HeadHelperColor.Background).Color.B;
                        byte r = ((SolidColorBrush)HeadHelperColor.Background).Color.R;
                        HeadHelperColor.Background = new SolidColorBrush(Color.FromRgb(r, (byte)value, b));
                        Main.HeadHelperColor = new RawColor4(r / 255f, (byte)value / 255f, b / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "TriggerbotSnipersDelay":
                    Main.TriggerbotSnipersDelay = (float)value;
                    return;
                //[/block]
                //[block]
                case "FovB":
                    {
                        byte g = ((SolidColorBrush)FovColor.Background).Color.G;
                        byte r = ((SolidColorBrush)FovColor.Background).Color.R;
                        Main.FovColor = new RawColor4(r / 255f, g / 255f, (byte)value / 255f, 1f);
                        FovColor.Background = new SolidColorBrush(Color.FromRgb(r, g, (byte)value));
                    }
                    return;
                //[/block]
                //[block]
                case "SkeletonEnemiesVisB":
                    {
                        byte g = ((SolidColorBrush)SkeletonEnemiesVisColor.Background).Color.G;
                        byte r = ((SolidColorBrush)SkeletonEnemiesVisColor.Background).Color.R;
                        Main.SkeletonEnemiesVisColor = new RawColor4(r / 255f, g / 255f, (byte)value / 255f, 1f);
                        SkeletonEnemiesVisColor.Background = new SolidColorBrush(Color.FromRgb(r, g, (byte)value));
                    }
                    return;
                //[/block]
                //[block]
                case "SpecialEntitiesB":
                    {
                        byte g = ((SolidColorBrush)SpecialEntitiesColor.Background).Color.G;
                        byte r = ((SolidColorBrush)SpecialEntitiesColor.Background).Color.R;
                        Main.SpecialEntitiesColor = new RawColor4(r / 255f, g / 255f, (byte)value / 255f, 1f);
                        SpecialEntitiesColor.Background = new SolidColorBrush(Color.FromRgb(r, g, (byte)value));
                    }
                    return;
                //[/block]
                //[block]
                case "BackgroundR":
                    {
                        byte g = ((SolidColorBrush)BackgroundColor.Background).Color.G;
                        byte b = ((SolidColorBrush)BackgroundColor.Background).Color.B;
                        BackgroundColor.Background = new SolidColorBrush(Color.FromRgb((byte)value, g, b));
                        Main.BackgroundColor = new RawColor4((byte)value / 255f, g / 255f, b / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "NamesR":
                    {
                        byte b = ((SolidColorBrush)NamesColor.Background).Color.B;
                        byte g = ((SolidColorBrush)NamesColor.Background).Color.G;
                        Main.NamesColor = new RawColor4((byte)value / 255f, g / 255f, b / 255f, 1f);
                        NamesColor.Background = new SolidColorBrush(Color.FromRgb((byte)value, g, b));
                    }
                    return;
                //[/block]
                //[block]
                case "FovSnipersSize":
                    Main.FovSnipersSize = (float)value;
                    return;
                //[/block]
                //[block]
                case "EntitiesG":
                    {
                        byte b = ((SolidColorBrush)EntitiesColor.Background).Color.B;
                        byte r = ((SolidColorBrush)EntitiesColor.Background).Color.R;
                        EntitiesColor.Background = new SolidColorBrush(Color.FromRgb(r, (byte)value, b));
                        Main.EntitiesColor = new RawColor4(r / 255f, (byte)value / 255f, b / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "SpectatorsG":
                    {
                        byte r = ((SolidColorBrush)SpectatorsColor.Background).Color.R;
                        byte b = ((SolidColorBrush)SpectatorsColor.Background).Color.B;
                        Main.SpectatorsColor = new RawColor4(r / 255f, (byte)value / 255f, b / 255f, 1f);
                        SpectatorsColor.Background = new SolidColorBrush(Color.FromRgb(r, (byte)value, b));
                    }
                    return;
                //[/block]
                //[block]
                case "AimbotPistolsLockTime":
                    Main.AimbotPistolsDelay = (float)value;
                    return;
                //[/block]
                //[block]
                case "HealthBarR":
                    {
                        byte b = ((SolidColorBrush)HealthBarColor.Background).Color.B;
                        byte g = ((SolidColorBrush)HealthBarColor.Background).Color.G;
                        Main.HealthBarColor = new RawColor4((byte)value / 255f, g / 255f, b / 255f, 1f);
                        HealthBarColor.Background = new SolidColorBrush(Color.FromRgb((byte)value, g, b));
                    }
                    return;
                //[/block]
                //[block]
                case "SkeletonAlliesG":
                    {
                        byte r = ((SolidColorBrush)SkeletonAlliesColor.Background).Color.R;
                        byte b = ((SolidColorBrush)SkeletonAlliesColor.Background).Color.B;
                        SkeletonAlliesColor.Background = new SolidColorBrush(Color.FromRgb(r, (byte)value, b));
                        Main.SkeletonAlliesColor = new RawColor4(r / 255f, (byte)value / 255f, b / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "SpectatorsR":
                    {
                        byte b = ((SolidColorBrush)SpectatorsColor.Background).Color.B;
                        byte g = ((SolidColorBrush)SpectatorsColor.Background).Color.G;
                        Main.SpectatorsColor = new RawColor4((byte)value / 255f, g / 255f, b / 255f, 1f);
                        SpectatorsColor.Background = new SolidColorBrush(Color.FromRgb((byte)value, g, b));
                    }
                    return;
                //[/block]
                //[block]
                case "PathAlliesB":
                    {
                        byte r = ((SolidColorBrush)PathAlliesColor.Background).Color.R;
                        byte g = ((SolidColorBrush)PathAlliesColor.Background).Color.G;
                        Main.PathAlliesColor = new RawColor4(r / 255f, g / 255f, (byte)value / 255f, 1f);
                        PathAlliesColor.Background = new SolidColorBrush(Color.FromRgb(r, g, (byte)value));
                    }
                    return;
                //[/block]
                //[block]
                case "RcsStrengthY":
                    Main.RcsStrengthY = (float)value;
                    return;
                //[/block]
                //[block]
                case "PathEnemiesB":
                    {
                        byte r = ((SolidColorBrush)PathEnemiesColor.Background).Color.R;
                        byte g = ((SolidColorBrush)PathEnemiesColor.Background).Color.G;
                        PathEnemiesColor.Background = new SolidColorBrush(Color.FromRgb(r, g, (byte)value));
                        Main.PathEnemiesColor = new RawColor4(r / 255f, g / 255f, (byte)value / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "AimbotSpeed":
                    var fixedValue = (float)value / (float)obj.Maximum;
                    Main.AimbotSpeed = fixedValue;
                    return;
                //[/block]
                //[block]
                case "NamesB":
                    {
                        byte r = ((SolidColorBrush)NamesColor.Background).Color.R;
                        byte g = ((SolidColorBrush)NamesColor.Background).Color.G;
                        NamesColor.Background = new SolidColorBrush(Color.FromRgb(r, g, (byte)value));
                        Main.NamesColor = new RawColor4(r / 255f, g / 255f, (byte)value / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "ChamsEnemiesG":
                    {
                        byte r = ((SolidColorBrush)ChamsEnemiesColor.Background).Color.R;
                        byte b = ((SolidColorBrush)ChamsEnemiesColor.Background).Color.B;
                        ChamsEnemiesColor.Background = new SolidColorBrush(Color.FromRgb(r, (byte)value, b));
                        Main.ChamsEnemiesColor = new RawColor4(r / 255f, (byte)value / 255f, b / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "SpecialEntitiesR":
                    {
                        byte b = ((SolidColorBrush)SpecialEntitiesColor.Background).Color.B;
                        byte g = ((SolidColorBrush)SpecialEntitiesColor.Background).Color.G;
                        Main.SpecialEntitiesColor = new RawColor4((byte)value / 255f, g / 255f, b / 255f, 1f);
                        SpecialEntitiesColor.Background = new SolidColorBrush(Color.FromRgb((byte)value, g, b));
                    }
                    return;
                //[/block]
                //[block]
                case "AdditionalInfoB":
                    {
                        byte r = ((SolidColorBrush)AdditionalInfoColor.Background).Color.R;
                        byte g = ((SolidColorBrush)AdditionalInfoColor.Background).Color.G;
                        Main.ExtendedInfoColor = new RawColor4(r / 255f, g / 255f, (byte)value / 255f, 1f);
                        AdditionalInfoColor.Background = new SolidColorBrush(Color.FromRgb(r, g, (byte)value));
                    }
                    return;
                //[/block]
                //[block]
                case "TriggerbotPistolsDelay":
                    Main.TriggerbotPistolsDelay = (float)value;
                    return;
                //[/block]
                //[block]
                case "PathAlliesR":
                    {
                        byte b = ((SolidColorBrush)PathAlliesColor.Background).Color.B;
                        byte g = ((SolidColorBrush)PathAlliesColor.Background).Color.G;
                        Main.PathAlliesColor = new RawColor4((byte)value / 255f, g / 255f, b / 255f, 1f);
                        PathAlliesColor.Background = new SolidColorBrush(Color.FromRgb((byte)value, g, b));
                    }
                    return;
                //[/block]
                //[block]
                case "SkeletonEnemiesR":
                    {
                        byte b = ((SolidColorBrush)SkeletonEnemiesColor.Background).Color.B;
                        byte g = ((SolidColorBrush)SkeletonEnemiesColor.Background).Color.G;
                        SkeletonEnemiesColor.Background = new SolidColorBrush(Color.FromRgb((byte)value, g, b));
                        Main.SkeletonEnemiesColor = new RawColor4((byte)value / 255f, g / 255f, b / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "ActiveWeaponR":
                    {
                        byte g = ((SolidColorBrush)ActiveWeaponColor.Background).Color.G;
                        byte b = ((SolidColorBrush)ActiveWeaponColor.Background).Color.B;
                        ActiveWeaponColor.Background = new SolidColorBrush(Color.FromRgb((byte)value, g, b));
                        Main.ActiveWeaponColor = new RawColor4((byte)value / 255f, g / 255f, b / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "PathEnemiesR":
                    {
                        byte g = ((SolidColorBrush)PathEnemiesColor.Background).Color.G;
                        byte b = ((SolidColorBrush)PathEnemiesColor.Background).Color.B;
                        PathEnemiesColor.Background = new SolidColorBrush(Color.FromRgb((byte)value, g, b));
                        Main.PathEnemiesColor = new RawColor4((byte)value / 255f, g / 255f, b / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "AlliesR":
                    {
                        byte g = ((SolidColorBrush)AlliesColor.Background).Color.G;
                        byte b = ((SolidColorBrush)AlliesColor.Background).Color.B;
                        AlliesColor.Background = new SolidColorBrush(Color.FromRgb((byte)value, g, b));
                        Main.AlliesColor = new RawColor4((byte)value / 255f, g / 255f, b / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "PathLength":
                    Main.PathLength = (float)value;
                    return;
                //[/block]
                //[block]
                case "ChickensG":
                    {
                        byte r = ((SolidColorBrush)ChickensColor.Background).Color.R;
                        byte b = ((SolidColorBrush)ChickensColor.Background).Color.B;
                        ChickensColor.Background = new SolidColorBrush(Color.FromRgb(r, (byte)value, b));
                        Main.ChickensColor = new RawColor4(r / 255f, (byte)value / 255f, b / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "ActiveWeaponG":
                    {
                        byte r = ((SolidColorBrush)ActiveWeaponColor.Background).Color.R;
                        byte b = ((SolidColorBrush)ActiveWeaponColor.Background).Color.B;
                        Main.ActiveWeaponColor = new RawColor4(r / 255f, (byte)value / 255f, b / 255f, 1f);
                        ActiveWeaponColor.Background = new SolidColorBrush(Color.FromRgb(r, (byte)value, b));
                    }
                    return;
                //[/block]
                //[block]
                case "RcsStrengthX":
                    Main.RcsStrengthX = (float)value;
                    return;
                //[/block]
                //[block]
                case "SoundEspFovSize":
                    Main.SoundEspFovSize = (float)value;
                    return;
                //[/block]
                //[block]
                case "SkeletonAlliesR":
                    {
                        byte b = ((SolidColorBrush)SkeletonAlliesColor.Background).Color.B;
                        byte g = ((SolidColorBrush)SkeletonAlliesColor.Background).Color.G;
                        Main.SkeletonAlliesColor = new RawColor4((byte)value / 255f, g / 255f, b / 255f, 1f);
                        SkeletonAlliesColor.Background = new SolidColorBrush(Color.FromRgb((byte)value, g, b));
                    }
                    return;
                //[/block]
                //[block]
                case "AimbotSnipersLockTime":
                    Main.AimbotSnipersDelay = (float)value;
                    return;
                //[/block]
                //[block]
                case "EnemiesVisG":
                    {
                        byte r = ((SolidColorBrush)EnemiesVisColor.Background).Color.R;
                        byte b = ((SolidColorBrush)EnemiesVisColor.Background).Color.B;
                        EnemiesVisColor.Background = new SolidColorBrush(Color.FromRgb(r, (byte)value, b));
                        Main.EnemiesVisColor = new RawColor4(r / 255f, (byte)value / 255f, b / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "PathAlliesG":
                    {
                        byte r = ((SolidColorBrush)PathAlliesColor.Background).Color.R;
                        byte b = ((SolidColorBrush)PathAlliesColor.Background).Color.B;
                        PathAlliesColor.Background = new SolidColorBrush(Color.FromRgb(r, (byte)value, b));
                        Main.PathAlliesColor = new RawColor4(r / 255f, (byte)value / 255f, b / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "EnemiesG":
                    {
                        byte r = ((SolidColorBrush)EnemiesColor.Background).Color.R;
                        byte b = ((SolidColorBrush)EnemiesColor.Background).Color.B;
                        Main.EnemiesColor = new RawColor4(r / 255f, (byte)value / 255f, b / 255f, 1f);
                        EnemiesColor.Background = new SolidColorBrush(Color.FromRgb(r, (byte)value, b));
                    }
                    return;
                //[/block]
                //[block]
                case "ChamsEnemiesR":
                    {
                        byte b = ((SolidColorBrush)ChamsEnemiesColor.Background).Color.B;
                        byte g = ((SolidColorBrush)ChamsEnemiesColor.Background).Color.G;
                        Main.ChamsEnemiesColor = new RawColor4((byte)value / 255f, g / 255f, b / 255f, 1f);
                        ChamsEnemiesColor.Background = new SolidColorBrush(Color.FromRgb((byte)value, g, b));
                    }
                    return;
                //[/block]
                //[block]
                case "HealthBarB":
                    {
                        byte r = ((SolidColorBrush)HealthBarColor.Background).Color.R;
                        byte g = ((SolidColorBrush)HealthBarColor.Background).Color.G;
                        HealthBarColor.Background = new SolidColorBrush(Color.FromRgb(r, g, (byte)value));
                        Main.HealthBarColor = new RawColor4(r / 255f, g / 255f, (byte)value / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "ChamsEnemiesB":
                    {
                        byte g = ((SolidColorBrush)ChamsEnemiesColor.Background).Color.G;
                        byte r = ((SolidColorBrush)ChamsEnemiesColor.Background).Color.R;
                        ChamsEnemiesColor.Background = new SolidColorBrush(Color.FromRgb(r, g, (byte)value));
                        Main.ChamsEnemiesColor = new RawColor4(r / 255f, g / 255f, (byte)value / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "RanksR":
                    {
                        byte g = ((SolidColorBrush)RanksColor.Background).Color.G;
                        byte b = ((SolidColorBrush)RanksColor.Background).Color.B;
                        Main.RanksColor = new RawColor4((byte)value / 255f, g / 255f, b / 255f, 1f);
                        RanksColor.Background = new SolidColorBrush(Color.FromRgb((byte)value, g, b));
                    }
                    return;
                //[/block]
                //[block]
                case "SoundEspFrequency":
                    Main.SoundEspFrequency = (float)value;
                    return;
                //[/block]
                //[block]
                case "FovChangerValue":
                    Main.FovChangerValue = (float)value;
                    return;
                //[/block]
                //[block]
                case "EntitiesR":
                    {
                        byte g = ((SolidColorBrush)EntitiesColor.Background).Color.G;
                        byte b = ((SolidColorBrush)EntitiesColor.Background).Color.B;
                        EntitiesColor.Background = new SolidColorBrush(Color.FromRgb((byte)value, g, b));
                        Main.EntitiesColor = new RawColor4((byte)value / 255f, g / 255f, b / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "FovSize":
                    Main.FovSize = (float)value;
                    return;
                //[/block]
                //[block]
                case "RanksG":
                    {
                        byte b = ((SolidColorBrush)RanksColor.Background).Color.B;
                        byte r = ((SolidColorBrush)RanksColor.Background).Color.R;
                        RanksColor.Background = new SolidColorBrush(Color.FromRgb(r, (byte)value, b));
                        Main.RanksColor = new RawColor4(r / 255f, (byte)value / 255f, b / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "RecoilCrosshairG":
                    {
                        byte r = ((SolidColorBrush)RecoilCrosshairColor.Background).Color.R;
                        byte b = ((SolidColorBrush)RecoilCrosshairColor.Background).Color.B;
                        Main.RecoilCrosshairColor = new RawColor4(r / 255f, (byte)value / 255f, b / 255f, 1f);
                        RecoilCrosshairColor.Background = new SolidColorBrush(Color.FromRgb(r, (byte)value, b));
                    }
                    return;
                //[/block]
                //[block]
                case "AimbotSnipersSpeed":
                    var fixedValue2 = (float)value / (float)obj.Maximum;
                    Main.AimbotSnipersSpeed = fixedValue2;
                    return;
                //[/block]
                //[block]
                case "HealthBarG":
                    {
                        byte b = ((SolidColorBrush)HealthBarColor.Background).Color.B;
                        byte r = ((SolidColorBrush)HealthBarColor.Background).Color.R;
                        Main.HealthBarColor = new RawColor4(r / 255f, (byte)value / 255f, b / 255f, 1f);
                        HealthBarColor.Background = new SolidColorBrush(Color.FromRgb(r, (byte)value, b));
                    }
                    return;
                //[/block]
                //[block]
                case "ChamsAlliesB":
                    {
                        byte g = ((SolidColorBrush)ChamsAlliesColor.Background).Color.G;
                        byte r = ((SolidColorBrush)ChamsAlliesColor.Background).Color.R;
                        ChamsAlliesColor.Background = new SolidColorBrush(Color.FromRgb(r, g, (byte)value));
                        Main.ChamsAlliesColor = new RawColor4(r / 255f, g / 255f, (byte)value / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "AdditionalInfoG":
                    {
                        byte b = ((SolidColorBrush)AdditionalInfoColor.Background).Color.B;
                        byte r = ((SolidColorBrush)AdditionalInfoColor.Background).Color.R;
                        AdditionalInfoColor.Background = new SolidColorBrush(Color.FromRgb(r, (byte)value, b));
                        Main.ExtendedInfoColor = new RawColor4(r / 255f, (byte)value / 255f, b / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "BrightnessStrength":
                    Main.BrightnessStrength = (float)(value / 100f);
                    return;
                //[/block]
                //[block]
                case "AlliesB":
                    {
                        byte r = ((SolidColorBrush)AlliesColor.Background).Color.R;
                        byte g = ((SolidColorBrush)AlliesColor.Background).Color.G;
                        Main.AlliesColor = new RawColor4(r / 255f, g / 255f, (byte)value / 255f, 1f);
                        AlliesColor.Background = new SolidColorBrush(Color.FromRgb(r, g, (byte)value));
                    }
                    return;
                //[/block]
                //[block]
                case "FakeLagPause":
                    Main.FakeLagPause = (float)value;
                    return;
                //[/block]
                //[block]
                case "SoundEspLengthScale":
                    if (value.Equals(0))
                        value = 0.1;
                    Main.SoundEspLengthScale = (float)value;
                    return;
                //[/block]
                //[block]
                case "AdditionalInfoR":
                    {
                        byte g = ((SolidColorBrush)AdditionalInfoColor.Background).Color.G;
                        byte b = ((SolidColorBrush)AdditionalInfoColor.Background).Color.B;
                        AdditionalInfoColor.Background = new SolidColorBrush(Color.FromRgb((byte)value, g, b));
                        Main.ExtendedInfoColor = new RawColor4((byte)value / 255f, g / 255f, b / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "TriggerbotDelay":
                    Main.TriggerbotDelay = (float)value;
                    return;
                //[/block]
                //[block]
                case "AimbotPistolsSpeed":
                    var fixedValue1 = (float)value / (float)obj.Maximum;
                    Main.AimbotPistolsSpeed = fixedValue1;
                    return;
                //[/block]
                //[block]
                case "BackgroundG":
                    {
                        byte r = ((SolidColorBrush)BackgroundColor.Background).Color.R;
                        byte b = ((SolidColorBrush)BackgroundColor.Background).Color.B;
                        BackgroundColor.Background = new SolidColorBrush(Color.FromRgb(r, (byte)value, b));
                        Main.BackgroundColor = new RawColor4(r / 255f, (byte)value / 255f, b / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "EntitiesB":
                    {
                        byte g = ((SolidColorBrush)EntitiesColor.Background).Color.G;
                        byte r = ((SolidColorBrush)EntitiesColor.Background).Color.R;
                        EntitiesColor.Background = new SolidColorBrush(Color.FromRgb(r, g, (byte)value));
                        Main.EntitiesColor = new RawColor4(r / 255f, g / 255f, (byte)value / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "RecoilCrosshairB":
                    {
                        byte g = ((SolidColorBrush)RecoilCrosshairColor.Background).Color.G;
                        byte r = ((SolidColorBrush)RecoilCrosshairColor.Background).Color.R;
                        RecoilCrosshairColor.Background = new SolidColorBrush(Color.FromRgb(r, g, (byte)value));
                        Main.RecoilCrosshairColor = new RawColor4(r / 255f, g / 255f, (byte)value / 255f, 1f);
                    }
                    return;
                //[/block]
                //[block]
                case "AimbotLockTime":
                    Main.AimbotLockTime = (float)value;
                    return;
                //[/block]
                //[block]
                case "SkeletonEnemiesVisG":
                    {
                        byte b = ((SolidColorBrush)SkeletonEnemiesVisColor.Background).Color.B;
                        byte r = ((SolidColorBrush)SkeletonEnemiesVisColor.Background).Color.R;
                        Main.SkeletonEnemiesVisColor = new RawColor4(r / 255f, (byte)value / 255f, b / 255f, 1f);
                        SkeletonEnemiesVisColor.Background = new SolidColorBrush(Color.FromRgb(r, (byte)value, b));
                    }
                    return;
                //[/block]
                //[block]
                case "PathEnemiesG":
                    {
                        byte r = ((SolidColorBrush)PathEnemiesColor.Background).Color.R;
                        byte b = ((SolidColorBrush)PathEnemiesColor.Background).Color.B;
                        Main.PathEnemiesColor = new RawColor4(r / 255f, (byte)value / 255f, b / 255f, 1f);
                        PathEnemiesColor.Background = new SolidColorBrush(Color.FromRgb(r, (byte)value, b));
                    }
                    return;
                //[/block]
                //[block]
                case "RanksB":
                    {
                        byte r = ((SolidColorBrush)RanksColor.Background).Color.R;
                        byte g = ((SolidColorBrush)RanksColor.Background).Color.G;
                        RanksColor.Background = new SolidColorBrush(Color.FromRgb(r, g, (byte)value));
                        Main.RanksColor = new RawColor4(r / 255f, g / 255f, (byte)value / 255f, 1f);
                    }
                    return;
                //[/block]
                //[/swap]
            }
        }

        private void NumericManager(object sender, RoutedPropertyChangedEventArgs<double?> e)
        {
            var obj = sender as NumericUpDown;
            var value = (int)obj.Value;

            switch (obj.Name)
            {
                //[swap]
                //[block]
                case "FpsLimit":
                    value = value == 0 ? 1 : value;
                    Main.FpsLimit = value;
                    return;
                //[/block]
                //[block]
                case "GlowDelay":
                    Main.GlowDelay = value;
                    return;
                //[/block]
                //[block]
                case "SkinStatTrak":
                    foreach (var skin in G.Skins)
                        if (skin.WeaponID.GetWeaponName() == SkinWeapon.Text)
                            skin.StatTrak = value;

                    SaveSkinsConfig();
                    return;
                //[/block]
                //[/swap]
            }
        }

        private void ComboBoxManager(object sender, SelectionChangedEventArgs e)
        {
            var obj = sender as ComboBox;
            var item = obj.SelectedItem as ComboBoxItem;

            switch (obj.Name)
            {
                //[swap]
                //[block]
                case "AimbotBone":
                    Main.AimbotBone = (Bone)item.Value;
                    return;
                //[/block]
                //[block]
                case "TriggerbotKey":
                    Main.TriggerbotKey = (Keys)item.Value;
                    return;
                //[/block]
                //[block]
                case "ForceUpdateKey":
                    Main.ForceUpdateKey = (Keys)item.Value;
                    return;
                //[/block]
                //[block]
                case "AimbotSnipersBone":
                    Main.AimbotSnipersBone = (Bone)item.Value;
                    return;
                //[/block]
                //[block]
                case "AimbotKey":
                    Main.AimbotKey = (Keys)item.Value;
                    return;
                //[/block]
                //[block]
                case "Skin":
                    if (item == null)
                        return;

                    foreach (var skin in G.Skins)
                        if (skin.WeaponID.GetWeaponName() == SkinWeapon.Text)
                            skin.ID = (int)item.Value;

                    SaveSkinsConfig();
                    return;
                //[/block]
                //[block]
                case "AimbotPistolsBone":
                    Main.AimbotPistolsBone = (Bone)item.Value;
                    return;
                //[/block]
                //[block]
                case "SkinPrefix":
                    foreach (var skin in G.Skins)
                        if (skin.WeaponID.GetWeaponName() == SkinWeapon.Text)
                            skin.Prefix = (int)item.Value;

                    SaveSkinsConfig();
                    return;
                //[/block]
                //[/swap]
                case "SkinWeapon":
                    var source = string.Empty;
                    obj.Text = item.Text;
                    switch (item.Text)
                    {
                        //[swap]
                        //[block]
                        case "P250":
                            source = "15   -  Gunsmoke\r\n27   -  Bone Mask\r\n34   -  Metallic DDPAT\r\n77   -  Boreal Forest\r\n99   -  Sand Dune\r\n102  -  Whiteout\r\n135  -  Urban Perforated\r\n136  -  Waves Perforated\r\n141  -  Orange Peel\r\n143  -  Urban Masked\r\n147  -  Jungle Dashed\r\n148  -  Sand Dashed\r\n149  -  Urban Dashed\r\n162  -  Splash\r\n164  -  Modern Hunter\r\n168  -  Nuclear Threat\r\n199  -  Dry Season\r\n207  -  Facets\r\n219  -  Hive\r\n230  -  Steel Disruption\r\n258  -  Mehndi\r\n271  -  Undertow\r\n295  -  Franklin\r\n358  -  Supernova\r\n373  -  Contamination\r\n388  -  Cartel\r\n404  -  Muertos\r\n426  -  Valence\r\n466  -  Crimson Kimono\r\n467  -  Mint Kimono\r\n501  -  Wingshot\r\n551  -  Asiimov\r\n592  -  Iron Clad";
                            break;
                        //[/block]
                        //[block]
                        case "PP-Bizon":
                            source = "13   -  Blue Streak\r\n25   -  Forest Leaves\r\n70   -  Carbon Fiber\r\n104  -  Grassland Leaves\r\n148  -  Sand Dashed\r\n149  -  Urban Dashed\r\n159  -  Brass\r\n164  -  Modern Hunter\r\n171  -  Irradiated Alert\r\n203  -  Rust Coat\r\n224  -  Water Sigil\r\n236  -  Night Ops\r\n267  -  Cobalt Halftone\r\n306  -  Antique\r\n349  -  Osiris\r\n376  -  Chemical Green\r\n457  -  Bamboo Print\r\n508  -  Fuel Rod\r\n526  -  Photic Zone\r\n542  -  Judgement of Anubis\r\n594  -  Harvester";
                            break;
                        //[/block]
                        //[block]
                        case "FAMAS":
                            source = "22   -  Contrast Spray\r\n47   -  Colony\r\n92   -  Cyanospatter\r\n119  -  Sage Spray\r\n122  -  Jungle Spray\r\n124  -  Sand Spray\r\n154  -  Afterimage\r\n178  -  Doomkitty\r\n186  -  Wave Spray\r\n194  -  Spitfire\r\n218  -  Hexane\r\n244  -  Teardown\r\n260  -  Pulse\r\n288  -  Sergeant\r\n371  -  Styx\r\n429  -  Djinn\r\n477  -  Neural Net\r\n492  -  Survivor Z\r\n529  -  Valence";
                            break;
                        //[/block]
                        //[block]
                        case "Tec-9":
                            source = "2    -  Groundwater\r\n17   -  Urban DDPAT\r\n36   -  Ossified\r\n159  -  Brass\r\n179  -  Nuclear Threat\r\n206  -  Tornado\r\n211  -  Ocean Foam\r\n216  -  Blue Titanium\r\n220  -  Hemoglobin\r\n235  -  VariCamo\r\n236  -  Night Ops\r\n237  -  Urban Rubble\r\n238  -  VariCamo Blue\r\n240  -  CaliCamo\r\n242  -  Army Mesh\r\n248  -  Red Quartz\r\n249  -  Cobalt Quartz\r\n250  -  Full Stop\r\n252  -  Silver Quartz\r\n272  -  Titanium Bit\r\n289  -  Sandstorm\r\n296  -  Meteorite\r\n303  -  Isaac\r\n374  -  Toxic\r\n439  -  Hades\r\n457  -  Bamboo Print\r\n458  -  Bamboo Shadow\r\n459  -  Bamboo Forest\r\n463  -  Terrace\r\n520  -  Avalanche\r\n539  -  Jambiya\r\n555  -  Re-Entry\r\n599  -  Ice Cap";
                            break;
                        //[/block]
                        //[block]
                        case "MP7":
                            source = "5    -  Forest DDPAT\r\n11   -  Skulls\r\n15   -  Gunsmoke\r\n17   -  Urban DDPAT\r\n28   -  Anodized Navy\r\n83   -  Orange DDPAT\r\n84   -  Pink DDPAT\r\n102  -  Whiteout\r\n141  -  Orange Peel\r\n202  -  Jungle DDPAT\r\n209  -  Groundwater\r\n211  -  Ocean Foam\r\n250  -  Full Stop\r\n354  -  Urban Hazard\r\n365  -  Olive Plaid\r\n423  -  Armor Core\r\n442  -  Asterion\r\n443  -  Pathfinder\r\n444  -  Daedalus\r\n481  -  Nemesis\r\n500  -  Special Delivery\r\n536  -  Impire";
                            break;
                        //[/block]
                        //[block]
                        case "MP9":
                            source = "33   -  Hot Rod\r\n39   -  Bulldozer\r\n61   -  Hypnotic\r\n100  -  Storm\r\n141  -  Orange Peel\r\n148  -  Sand Dashed\r\n199  -  Dry Season\r\n262  -  Rose Iron\r\n329  -  Dark Age\r\n366  -  Green Plaid\r\n368  -  Setting Sun\r\n386  -  Dart\r\n403  -  Deadly Poison\r\n448  -  Pandora\'s Box\r\n482  -  Ruby Poison Dart\r\n549  -  Bioleak";
                            break;
                        //[/block]
                        //[block]
                        case "MAC-10":
                            source = "3    -  Candy Apple\r\n17   -  Urban DDPAT\r\n32   -  Silver\r\n38   -  Fade\r\n98   -  Ultraviolet\r\n101  -  Tornado\r\n188  -  Graven\r\n233  -  Tropical Storm\r\n246  -  Amber Fade\r\n253  -  Acid Fade\r\n284  -  Heat\r\n310  -  Curse\r\n333  -  Indigo\r\n337  -  Tatter\r\n343  -  Commuter\r\n372  -  Nuclear Garden\r\n402  -  Malachite\r\n433  -  Neon Rider\r\n498  -  Rangeen\r\n534  -  Lapis Gator\r\n589  -  Carnivore";
                            break;
                        //[/block]
                        //[block]
                        case "AUG":
                            source = "9    -  Bengal Tiger\r\n10   -  Copperhead\r\n33   -  Hot Rod\r\n46   -  Contractor\r\n47   -  Colony\r\n73   -  Wings\r\n100  -  Storm\r\n110  -  Condemned\r\n197  -  Anodized Navy\r\n280  -  Chameleon\r\n305  -  Torque\r\n375  -  Radiation Hazard\r\n444  -  Daedalus\r\n455  -  Akihabara Accept\r\n507  -  Ricochet\r\n541  -  Fleet Flock\r\n583  -  Aristocrat";
                            break;
                        //[/block]
                        //[block]
                        case "Desert Eagle":
                            source = "17   -  Urban DDPAT\r\n37   -  Blaze\r\n40   -  Night\r\n61   -  Hypnotic\r\n90   -  Mudder\r\n185  -  Golden Koi\r\n231  -  Cobalt Disruption\r\n232  -  Crimson Web\r\n237  -  Urban Rubble\r\n273  -  Heirloom\r\n296  -  Meteorite\r\n328  -  Hand Cannon\r\n347  -  Pilot\r\n351  -  Conspiracy\r\n397  -  Naga\r\n425  -  Bronze Deco\r\n468  -  Midnight Storm\r\n469  -  Sunset Storm 壱\r\n470  -  Sunset Storm 弐\r\n509  -  Corinthian\r\n527  -  Kumicho Dragon";
                            break;
                        //[/block]
                        //[block]
                        case "M249":
                            source = "22   -  Contrast Spray\r\n75   -  Blizzard Marbleized\r\n119  -  Sage Spray\r\n122  -  Jungle Spray\r\n124  -  Sand Spray\r\n186  -  Wave Spray\r\n202  -  Jungle DDPAT\r\n243  -  Gator Mesh\r\n266  -  Magma\r\n401  -  System Lock\r\n452  -  Shipping Forecast\r\n472  -  Impact Drill\r\n496  -  Nebula Crusader\r\n547  -  Spectre";
                            break;
                        //[/block]
                        //[block]
                        case "UMP-45":
                            source = "15   -  Gunsmoke\r\n17   -  Urban DDPAT\r\n37   -  Blaze\r\n70   -  Carbon Fiber\r\n93   -  Caramel\r\n169  -  Fallout Warning\r\n175  -  Scorched\r\n193  -  Bone Pile\r\n281  -  Corporal\r\n333  -  Indigo\r\n362  -  Labyrinth\r\n392  -  Delusion\r\n436  -  Grand Prix\r\n441  -  Minotaur\'s Labyrinth\r\n488  -  Riot\r\n556  -  Primal Saber";
                            break;
                        //[/block]
                        //[block]
                        case "R8 Revolver":
                            source = "12   -  Crimson Web\r\n27   -  Bone Mask\r\n135  -  Urban Perforated\r\n136  -  Waves Perforated\r\n141  -  Orange Peel\r\n143  -  Urban Masked\r\n147  -  Jungle Dashed\r\n148  -  Sand Dashed\r\n149  -  Urban Dashed\r\n199  -  Dry Season\r\n522  -  Fade\r\n523  -  Amber Fade\r\n595  -  Reboot";
                            break;
                        //[/block]
                        //[block]
                        case "Galil AR":
                            source = "76   -  Winter Forest\r\n83   -  Orange DDPAT\r\n119  -  Sage Spray\r\n192  -  Shattered\r\n216  -  Blue Titanium\r\n235  -  VariCamo\r\n236  -  Night Ops\r\n237  -  Urban Rubble\r\n238  -  VariCamo Blue\r\n240  -  CaliCamo\r\n241  -  Hunting Blind\r\n250  -  Full Stop\r\n264  -  Sandstorm\r\n297  -  Tuxedo\r\n308  -  Kami\r\n379  -  Cerberus\r\n398  -  Chatterbox\r\n428  -  Eco\r\n460  -  Aqua Terrace\r\n478  -  Rocket Pop\r\n494  -  Stone Cold\r\n546  -  Firefight";
                            break;
                        //[/block]
                        //[block]
                        case "CZ75-Auto":
                            source = "12   -  Crimson Web\r\n196  -  Emerald\r\n218  -  Hexane\r\n268  -  Tread Plate\r\n269  -  The Fuschia Is Now\r\n270  -  Victoria\r\n297  -  Tuxedo\r\n298  -  Army Sheen\r\n315  -  Poison Dart\r\n322  -  Nitro\r\n325  -  Chalice\r\n334  -  Twist\r\n350  -  Tigris\r\n366  -  Green Plaid\r\n435  -  Pole Position\r\n476  -  Yellow Jacket\r\n543  -  Red Astor";
                            break;
                        //[/block]
                        //[block]
                        case "AK-47":
                            source = "14   -  Red Laminate\r\n44   -  Case Hardened\r\n72   -  Safari Mesh\r\n122  -  Jungle Spray\r\n170  -  Predator\r\n172  -  Black Laminate\r\n176  -  Faded Zebra\r\n180  -  Fire Serpent\r\n226  -  Blue Laminate\r\n229  -  Azure Zebra\r\n282  -  Redline\r\n300  -  Emerald Pinstripe\r\n302  -  Vulcan\r\n316  -  Jaguar\r\n340  -  Jet Set\r\n341  -  First Class\r\n380  -  Wasteland Rebel\r\n394  -  Cartel\r\n422  -  Elite Build\r\n456  -  Hydroponic\r\n474  -  Aquamarine Revenge\r\n490  -  Frontside Misty\r\n506  -  Point Disarray\r\n524  -  Fuel Injector";
                            break;
                        //[/block]
                        //[block]
                        case "Five-SeveN":
                            source = "3    -  Candy Apple\r\n44   -  Case Hardened\r\n46   -  Contractor\r\n78   -  Forest Night\r\n141  -  Orange Peel\r\n151  -  Jungle\r\n210  -  Anodized Gunmetal\r\n223  -  Nightshade\r\n252  -  Silver Quartz\r\n254  -  Nitro\r\n265  -  Kami\r\n274  -  Copper Galaxy\r\n352  -  Fowl Play\r\n377  -  Hot Shot\r\n387  -  Urban Hazard\r\n427  -  Monkey Business\r\n464  -  Neon Kimono\r\n465  -  Orange Kimono\r\n466  -  Crimson Kimono\r\n510  -  Retrobution\r\n530  -  Triumvirate\r\n585  -  Violent Daimyo";
                            break;
                        //[/block]
                        //[block]
                        case "Nova":
                            source = "3    -  Candy Apple\r\n25   -  Forest Leaves\r\n62   -  Bloomstick\r\n99   -  Sand Dune\r\n104  -  Grassland Leaves\r\n107  -  Polar Mesh\r\n158  -  Walnut\r\n164  -  Modern Hunter\r\n166  -  Blaze Orange\r\n170  -  Predator\r\n176  -  Faded Zebra\r\n191  -  Tempest\r\n212  -  Graphite\r\n225  -  Ghost Camo\r\n229  -  Azure Zebra\r\n263  -  Rising Skull\r\n286  -  Antique\r\n294  -  Green Apple\r\n299  -  Caged Steel\r\n356  -  Koi\r\n450  -  Moon in Libra\r\n484  -  Ranger\r\n537  -  Hyper Beast\r\n590  -  Exo";
                            break;
                        //[/block]
                        //[block]
                        case "SSG 08":
                            source = "26   -  Lichen Dashed\r\n60   -  Dark Water\r\n96   -  Blue Spruce\r\n99   -  Sand Dune\r\n200  -  Mayan Dreams\r\n222  -  Blood in the Water\r\n233  -  Tropical Storm\r\n253  -  Acid Fade\r\n304  -  Slashed\r\n319  -  Detour\r\n361  -  Abyss\r\n503  -  Big Iron\r\n538  -  Necropos\r\n554  -  Ghost Crusader";
                            break;
                        //[/block]
                        //[block]
                        case "XM1014":
                            source = "42   -  Blue Steel\r\n95   -  Grassland\r\n96   -  Blue Spruce\r\n135  -  Urban Perforated\r\n166  -  Blaze Orange\r\n169  -  Fallout Warning\r\n205  -  Jungle\r\n238  -  VariCamo Blue\r\n240  -  CaliCamo\r\n314  -  Heaven Guard\r\n320  -  Red Python\r\n348  -  Red Leather\r\n370  -  Bone Machine\r\n393  -  Tranquility\r\n407  -  Quicksilver\r\n505  -  Scumbria\r\n521  -  Teclu Burner\r\n557  -  Black Tie";
                            break;
                        //[/block]
                        //[block]
                        case "G3SG1":
                            source = "6    -  Arctic Camo\r\n8    -  Desert Storm\r\n46   -  Contractor\r\n72   -  Safari Mesh\r\n74   -  Polar Camo\r\n147  -  Jungle Dashed\r\n195  -  Demeter\r\n229  -  Azure Zebra\r\n235  -  VariCamo\r\n236  -  Night Ops\r\n237  -  Urban Rubble\r\n238  -  VariCamo Blue\r\n240  -  CaliCamo\r\n250  -  Full Stop\r\n294  -  Green Apple\r\n382  -  Murky\r\n438  -  Chronos\r\n465  -  Orange Kimono\r\n493  -  Flux\r\n511  -  The Executioner\r\n545  -  Orange Crash";
                            break;
                        //[/block]
                        //[block]
                        case "Negev":
                            source = "28   -  Anodized Navy\r\n240  -  CaliCamo\r\n285  -  Terrain\r\n298  -  Army Sheen\r\n317  -  Bratatat\r\n355  -  Desert-Strike\r\n369  -  Nuclear Waste\r\n432  -  Man-o\'-war\r\n483  -  Loudmouth\r\n514  -  Power Loader";
                            break;
                        //[/block]
                        //[block]
                        case "P90":
                            source = "20   -  Virus\r\n67   -  Cold Blooded\r\n100  -  Storm\r\n111  -  Glacier Mesh\r\n124  -  Sand Spray\r\n156  -  Death by Kitty\r\n169  -  Fallout Warning\r\n175  -  Scorched\r\n182  -  Emerald Dragon\r\n228  -  Blind Spot\r\n234  -  Ash Wood\r\n244  -  Teardown\r\n283  -  Trigon\r\n311  -  Desert Warfare\r\n335  -  Module\r\n342  -  Leather\r\n359  -  Asiimov\r\n486  -  Elite Build\r\n516  -  Shapewood\r\n593  -  Chopper";
                            break;
                        //[/block]
                        //[block]
                        case "USP-S":
                            source = "25   -  Forest Leaves\r\n60   -  Dark Water\r\n104  -  Grassland Leaves\r\n183  -  Overgrowth\r\n217  -  Blood Tiger\r\n221  -  Serum\r\n236  -  Night Ops\r\n277  -  Stainless\r\n290  -  Guardian\r\n313  -  Orion\r\n318  -  Road Rash\r\n332  -  Royal Blue\r\n339  -  Caiman\r\n364  -  Business Class\r\n454  -  Para Green\r\n489  -  Torque\r\n504  -  Kill Confirmed\r\n540  -  Lead Conduit";
                            break;
                        //[/block]
                        //[block]
                        case "M4A4":
                            source = "8    -  Desert Storm\r\n16   -  Jungle Tiger\r\n17   -  Urban DDPAT\r\n101  -  Tornado\r\n155  -  Bullet Rain\r\n164  -  Modern Hunter\r\n167  -  Radiation Hazard\r\n176  -  Faded Zebra\r\n187  -  Zirka\r\n215  -  X-Ray\r\n255  -  Asiimov\r\n309  -  Howl\r\n336  -  Desert-Strike\r\n384  -  Griffin\r\n400  -  龍王 (Dragon King)\r\n449  -  Poseidon\r\n471  -  Daybreak\r\n480  -  Evil Daimyo\r\n512  -  Royal Paladin\r\n533  -  The Battlestar\r\n588  -  Desolate Space";
                            break;
                        //[/block]
                        //[block]
                        case "AWP":
                            source = "30   -  Snake Camo\r\n51   -  Lightning Strike\r\n72   -  Safari Mesh\r\n84   -  Pink DDPAT\r\n174  -  BOOM\r\n181  -  Corticera\r\n212  -  Graphite\r\n227  -  Electric Hive\r\n251  -  Pit Viper\r\n259  -  Redline\r\n279  -  Asiimov\r\n320  -  Red Python\r\n344  -  Dragon Lore\r\n395  -  Man-o\'-war\r\n424  -  Worm God\r\n446  -  Medusa\r\n451  -  Sun in Leo\r\n475  -  Hyper Beast\r\n525  -  Elite Build\r\n584  -  Phobos";
                            break;
                        //[/block]
                        //[block]
                        case "Dual Berettas":
                            source = "28   -  Anodized Navy\r\n43   -  Stained\r\n46   -  Contractor\r\n47   -  Colony\r\n153  -  Demolition\r\n190  -  Black Limba\r\n220  -  Hemoglobin\r\n249  -  Cobalt Quartz\r\n261  -  Marina\r\n276  -  Panther\r\n307  -  Retribution\r\n330  -  Briar\r\n396  -  Urban Shock\r\n447  -  Duelist\r\n450  -  Moon in Libra\r\n491  -  Dualing Dragons\r\n528  -  Cartel\r\n544  -  Ventilators";
                            break;
                        //[/block]
                        //[block]
                        case "SG 553":
                            source = "28   -  Anodized Navy\r\n39   -  Bulldozer\r\n98   -  Ultraviolet\r\n101  -  Tornado\r\n136  -  Waves Perforated\r\n186  -  Wave Spray\r\n243  -  Gator Mesh\r\n247  -  Damascus Steel\r\n287  -  Pulse\r\n298  -  Army Sheen\r\n363  -  Traveler\r\n378  -  Fallout Warning\r\n487  -  Cyrex\r\n519  -  Tiger Moth\r\n553  -  Atlas\r\n598  -  Aerial";
                            break;
                        //[/block]
                        //[block]
                        case "Sawed-Off":
                            source = "5    -  Forest DDPAT\r\n17   -  Urban DDPAT\r\n30   -  Snake Camo\r\n41   -  Copper\r\n83   -  Orange DDPAT\r\n84   -  Pink DDPAT\r\n119  -  Sage Spray\r\n171  -  Irradiated Alert\r\n202  -  Jungle DDPAT\r\n203  -  Rust Coat\r\n204  -  Mosaico\r\n246  -  Amber Fade\r\n250  -  Full Stop\r\n256  -  The Kraken\r\n345  -  First Class\r\n390  -  Highwayman\r\n405  -  Serenity\r\n434  -  Origami\r\n458  -  Bamboo Shadow\r\n517  -  Yorick\r\n552  -  Fubar\r\n596  -  Limelight";
                            break;
                        //[/block]
                        //[block]
                        case "SCAR-20":
                            source = "46   -  Contractor\r\n70   -  Carbon Fiber\r\n100  -  Storm\r\n116  -  Sand Mesh\r\n165  -  Splash Jam\r\n196  -  Emerald\r\n232  -  Crimson Web\r\n233  -  Tropical Storm\r\n298  -  Army Sheen\r\n312  -  Cyrex\r\n391  -  Cardiac\r\n406  -  Grotto\r\n502  -  Green Marine\r\n518  -  Outbreak\r\n597  -  Bloodsport";
                            break;
                        //[/block]
                        //[block]
                        case "Glock-18":
                            source = "2    -  Groundwater\r\n3    -  Candy Apple\r\n38   -  Fade\r\n40   -  Night\r\n48   -  Dragon Tattoo\r\n159  -  Brass\r\n208  -  Sand Dune\r\n230  -  Steel Disruption\r\n246  -  Amber Fade\r\n253  -  Acid Fade\r\n278  -  Blue Fissure\r\n293  -  Death Rattle\r\n353  -  Water Elemental\r\n367  -  Reactor\r\n381  -  Grinder\r\n399  -  Catacombs\r\n437  -  Twilight Galaxy\r\n479  -  Bunsen Burner\r\n495  -  Wraiths\r\n532  -  Royal Legion\r\n586  -  Wasteland Rebel";
                            break;
                        //[/block]
                        //[block]
                        case "P2000":
                            source = "21   -  Granite Marbleized\r\n32   -  Silver\r\n71   -  Scorpion\r\n95   -  Grassland\r\n104  -  Grassland Leaves\r\n184  -  Corticera\r\n211  -  Ocean Foam\r\n246  -  Amber Fade\r\n275  -  Red FragCam\r\n327  -  Chainmail\r\n338  -  Pulse\r\n346  -  Coach Class\r\n357  -  Ivory\r\n389  -  Fire Elemental\r\n443  -  Pathfinder\r\n485  -  Handgun\r\n515  -  Imperial\r\n550  -  Oceanic\r\n591  -  Imperial Dragon";
                            break;
                        //[/block]
                        //[block]
                        case "MAG-7":
                            source = "32   -  Silver\r\n34   -  Metallic DDPAT\r\n39   -  Bulldozer\r\n99   -  Sand Dune\r\n100  -  Storm\r\n171  -  Irradiated Alert\r\n177  -  Memento\r\n198  -  Hazard\r\n291  -  Heaven Guard\r\n385  -  Firestarter\r\n431  -  Heat\r\n462  -  Counter Terrace\r\n473  -  Seabird\r\n499  -  Cobalt Core\r\n535  -  Praetorian";
                            break;
                        //[/block]
                        //[block]
                        case "M4A1-S":
                            source = "60   -  Dark Water\r\n77   -  Boreal Forest\r\n189  -  Bright Water\r\n217  -  Blood Tiger\r\n235  -  VariCamo\r\n236  -  Night Ops\r\n237  -  Urban Rubble\r\n238  -  VariCamo Blue\r\n240  -  CaliCamo\r\n250  -  Full Stop\r\n254  -  Nitro\r\n257  -  Guardian\r\n301  -  Atomic Alloy\r\n321  -  Master Piece\r\n326  -  Knight\r\n360  -  Cyrex\r\n383  -  Basilisk\r\n430  -  Hyper Beast\r\n440  -  Icarus Fell\r\n445  -  Hot Rod\r\n497  -  Golden Coil\r\n548  -  Chantico\'s Fire\r\n587  -  Mecha Industries";
                            break;
                        //[/block]
                        //[/swap]
                    }

                    source = "0    -  Disabled\r\n" + source;
                    source = source.Replace("    ", "  ");
                    source = source.Replace("   ", "  ");

                    //[swap]
                    var lines = source.Split(new[] { Environment.NewLine }, StringSplitOptions.RemoveEmptyEntries);
                    var skinNames = new List<string>();
                    var skinIDs = new List<int>();
                    //[/swap]

                    foreach (var line in lines)
                    {
                        var split = line.Split(new[] { "  -  " }, StringSplitOptions.RemoveEmptyEntries);
                        //[swap]
                        skinIDs.Add(int.Parse(split[0]));
                        skinNames.Add(split[1]);
                        //[/swap]
                    }

                    Skin.Items.Clear();
                    for (int i = 0; i < skinNames.Count; i++)
                        Skin.Items.Add(new ComboBoxItem(skinNames[i], skinIDs[i]));

                    var selected = int.Parse(G.SkinConfig.Get("ID", item.Text));
                    var index = skinIDs.TakeWhile(i => i != selected).Count();
                    if (index > lines.Length - 1)
                        index = 0;

                    var prefixSelected = int.Parse(G.SkinConfig.Get("Prefix", item.Text));
                    var prefixIndex = SkinPrefix.Items.Cast<ComboBoxItem>().TakeWhile(skinPrefixItem => (int) skinPrefixItem.Value != prefixSelected).Count();
                    if (prefixIndex > SkinPrefix.Items.Count - 1)
                        prefixIndex = 0;

                    //[swap]
                    SkinPrefix.SelectedIndex = prefixIndex;
                    SkinName.Text = G.SkinConfig.Get("Name", item.Text);
                    SkinWear.Value = float.Parse(G.SkinConfig.Get("Wear", item.Text));
                    SkinStatTrak.Value = int.Parse(G.SkinConfig.Get("StatTrak", item.Text));
                    Skin.SelectedIndex = index;
                    //[/swap]

                    return;
            }
        }

        private void TextBoxManager(object sender, TextChangedEventArgs e)
        {
            var obj = sender as TextBox;
            var value = obj.Text;

            switch (obj.Name)
            {
                //[swap]
                //[block]
                case "SkinName":
                    foreach (var skin in G.Skins)
                        if (skin.WeaponID.GetWeaponName() == SkinWeapon.Text)
                            skin.Name = value;

                    SaveSkinsConfig();
                    return;
                //[/block]
                //[/swap]
            }
        }
    }
}