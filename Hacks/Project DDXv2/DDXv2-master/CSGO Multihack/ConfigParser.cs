using System.IO;
using System.Windows.Forms;
using System.Windows.Media;
using DDX.Enums;
using DDX.Structs.Config;
using Newtonsoft.Json;

namespace DDX
{
    public static class ConfigParser
    {
        public static readonly string FilePath = Path.Combine(G.BaseDir, "config.json");

        public static void Save()
        {
            var json = JsonConvert.SerializeObject(G.C, Formatting.Indented);

            File.WriteAllText(FilePath, json);
        }

        public static void Load()
        {
            if (!File.Exists(FilePath))
            {
                if (!Directory.Exists(G.BaseDir))
                {
                    Directory.CreateDirectory(G.BaseDir);
                }

                Create();
                Save();
                return;
            }

            var json = File.ReadAllText(FilePath);

            G.C = JsonConvert.DeserializeObject<Config>(json);

            G.C.Misc.ChatInfo = false;
            G.C.Misc.Spam = false;
            G.C.Misc.FakeLag = false;
        }

        private static void Create()
        {
            G.C = new Config();

            G.C.Aim = new AimConfig();
            G.C.Clr = new ColorsConfig();
            G.C.Key = new KeysConfig();
            G.C.Misc = new MiscConfig();
            G.C.Stts = new SettingsConfig();
            G.C.Vis = new VisualsConfig();

            G.C.Aim.Default = new WeaponProperties();
            G.C.Aim.Default.AimbotProp = new AimbotProperties();
            G.C.Aim.Default.AimbotProp.LockProp = new LockProperties();
            G.C.Aim.Default.AimbotProp.VisibilityCheckProp = new VisibilityCheckProperties();
            G.C.Aim.Default.RcsProp = new RcsProperties();
            G.C.Aim.Default.SilentAimProp = new SilentAimProperties();
            G.C.Aim.Default.SilentAimProp.VisibilityCheckProp = new VisibilityCheckProperties();
            G.C.Aim.Default.TriggerbotProp = new TriggerbotProperties();
            G.C.Aim.Pistols = G.C.Aim.Default;
            G.C.Aim.Snipers = G.C.Aim.Default;

            G.C.Misc.BombDamageProp = new BombDamageProperties();
            G.C.Misc.BrightnessProp = new BrightnessProperties();
            G.C.Misc.BunnyhopProp = new BunnyhopProperties();
            G.C.Misc.ChatInfoProp = new ChatInfoProperties();
            G.C.Misc.FakeLagProp = new FakeLagProperties();
            G.C.Misc.FovChangerProp = new FovChangerProperties();
            G.C.Misc.SpamProp = new SpamProperties();
            G.C.Misc.SpectatorListProp = new SpectatorListProperties();

            G.C.Vis.Entities = new EntitiesConfig();
            G.C.Vis.Entities.ChamsProp = new ChamsProperties();
            G.C.Vis.Entities.GlowProp = new GlowProperties();
            G.C.Vis.Players = new PlayersConfig();
            G.C.Vis.Players.ActiveWeaponProp = new ActiveWeaponProperties();
            G.C.Vis.Players.ChamsProp = new ChamsProperties();
            G.C.Vis.Players.GlowProp = new GlowProperties();
            G.C.Vis.Players.SkeletonProp = new SkeletonProperties();
            G.C.Vis.Players.RankProp = new RankProperties();

            G.C.Aim.Default.AimbotProp.Bone = Bone.Dynamic;
            G.C.Aim.Default.AimbotProp.Fov = 10;
            G.C.Aim.Default.AimbotProp.Key = Keys.LButton;
            G.C.Aim.Default.AimbotProp.Lock = true;
            G.C.Aim.Default.AimbotProp.LockProp.Length = 350;
            G.C.Aim.Default.AimbotProp.Smooth = 50;
            G.C.Aim.Default.AimbotProp.VisibilityCheck = true;
            G.C.Aim.Default.AimbotProp.VisibilityCheckProp.BspParsing = true;
            G.C.Aim.Default.Rcs = true;
            G.C.Aim.Default.RcsProp.PitchStrength = 100;
            G.C.Aim.Default.RcsProp.Smooth = 70;
            G.C.Aim.Default.RcsProp.YawStrength = 100;
            G.C.Aim.Default.SilentAimProp.Bone = Bone.Head;
            G.C.Aim.Default.SilentAimProp.Fov = 8;
            G.C.Aim.Default.SilentAimProp.Key = Keys.MButton;
            G.C.Aim.Default.SilentAimProp.VisibilityCheck = true;
            G.C.Aim.Default.SilentAimProp.VisibilityCheckProp.BspParsing = true;
            G.C.Aim.Default.TriggerbotProp.Key = Keys.XButton1;
            G.C.Aim.Default.TriggerbotWithAimbotKey = Keys.XButton2;

            G.C.Aim.Pistols = G.C.Aim.Default;
            G.C.Aim.Snipers = G.C.Aim.Default;

            G.C.Aim.Pistols.AimbotProp.Bone = Bone.Head;
            G.C.Aim.Pistols.AimbotProp.Smooth = 50;
            G.C.Aim.Pistols.RcsProp.AimbotOnly = true;
            G.C.Aim.Snipers.AimbotProp.Smooth = 30;
            G.C.Aim.Snipers.Rcs = false;
            G.C.Aim.Snipers.TriggerbotProp.BeforeDelay = 50;

            G.C.Vis.AimFov = true;
            G.C.Vis.SilentAimFov = true;
            G.C.Vis.AimPoint = true;
            G.C.Vis.Players.SkeletonProp.Fill = true;
            G.C.Vis.Players.VisibilityCheck = true;
            G.C.Vis.Players.RankProp.Simple = true;

            G.C.Misc.BombDamageProp.Size = 16;
            G.C.Misc.BombDamageProp.X = 1060;
            G.C.Misc.BombDamageProp.Y = 540;
            G.C.Misc.BrightnessProp.Strength = 10;
            G.C.Misc.FakeLagProp.Length = 200;
            G.C.Misc.FakeLagProp.Pause = 250;
            G.C.Misc.FovChangerProp.Value = 110;
            G.C.Misc.SpamProp.Message = @"/\ DDX Multihack by Zaczero /\";
            G.C.Misc.SpamProp.SendAll = true;
            G.C.Misc.SpectatorListProp.Size = 14;
            G.C.Misc.SpectatorListProp.X = 1400;
            G.C.Misc.SpectatorListProp.Y = 350;
            
            G.C.Clr.AimFov = Color.FromArgb(255, 255, 0, 0);
            G.C.Clr.AimPoint = Color.FromArgb(255, 255, 255, 255);
            G.C.Clr.ArmorAllies = Color.FromArgb(255, 255, 255, 255);
            G.C.Clr.ArmorEnemies = Color.FromArgb(255, 255, 255, 255);
            G.C.Clr.ArmorEnemiesVisible = Color.FromArgb(255, 255, 255, 255);
            G.C.Clr.Background = Color.FromArgb(255, 0, 0, 0);
            G.C.Clr.ChamsAlliesMinHealth = Color.FromArgb(255, 255, 0, 155);
            G.C.Clr.ChamsAlliesMaxHealth = Color.FromArgb(255, 0, 128, 255);
            G.C.Clr.ChamsAllies = Color.FromArgb(255, 0, 255, 0);
            G.C.Clr.ChamsEnemiesMinHealth = Color.FromArgb(255, 255, 0, 155);
            G.C.Clr.ChamsEnemiesMinHealthVisible = Color.FromArgb(255, 255, 0, 155);
            G.C.Clr.ChamsEnemiesMaxHealth = Color.FromArgb(255, 0, 128, 255);
            G.C.Clr.ChamsEnemiesMaxHealthVisible = Color.FromArgb(255, 0, 128, 255);
            G.C.Clr.ChamsEnemies = Color.FromArgb(255, 255, 0, 0);
            G.C.Clr.ChamsEnemiesVisible = Color.FromArgb(255, 255, 255, 0);
            G.C.Clr.ChamsEntities = Color.FromArgb(255, 255, 255, 0);
            G.C.Clr.ChamsEntitiesImportant = Color.FromArgb(255, 255, 0, 255);
            G.C.Clr.EspAllies = Color.FromArgb(255, 255, 255, 255);
            G.C.Clr.EspEnemies = Color.FromArgb(255, 255, 35, 0);
            G.C.Clr.EspEnemiesVisible = Color.FromArgb(255, 255, 255, 0);
            G.C.Clr.EspEntities = Color.FromArgb(255, 255, 255, 255);
            G.C.Clr.EspEntitiesImportant = Color.FromArgb(255, 255, 0, 255);
            G.C.Clr.GlowAlliesMinHealth = Color.FromArgb(200, 0, 255, 135);
            G.C.Clr.GlowAlliesMaxHealth = Color.FromArgb(200, 0, 255, 135);
            G.C.Clr.GlowAllies = Color.FromArgb(200, 0, 255, 135);
            G.C.Clr.GlowBomb = Color.FromArgb(255, 244, 118, 65);
            G.C.Clr.GlowChickens = Color.FromArgb(155, 255, 255, 255);
            G.C.Clr.GlowEnemiesMinHealth = Color.FromArgb(200, 255, 0, 0);
            G.C.Clr.GlowEnemiesMinHealthVisible = Color.FromArgb(200, 255, 0, 0);
            G.C.Clr.GlowEnemiesMaxHealth = Color.FromArgb(200, 255, 15, 44);
            G.C.Clr.GlowEnemiesMaxHealthVisible = Color.FromArgb(200, 255, 190, 0);
            G.C.Clr.GlowEnemies = Color.FromArgb(200, 255, 15, 44);
            G.C.Clr.GlowEnemiesVisible = Color.FromArgb(200, 255, 190, 0);
            G.C.Clr.GlowGear = Color.FromArgb(155, 255, 255, 255);
            G.C.Clr.GlowGrenades = Color.FromArgb(255, 255, 35, 35);
            G.C.Clr.GlowHostages = Color.FromArgb(155, 255, 255, 255);
            G.C.Clr.GlowWeapons = Color.FromArgb(155, 255, 255, 255);
            G.C.Clr.HealthAllies = Color.FromArgb(255, 0, 255, 0);
            G.C.Clr.HealthEnemies = Color.FromArgb(255, 255, 0, 0);
            G.C.Clr.HealthEnemiesVisible = Color.FromArgb(255, 255, 0, 0);
            G.C.Clr.LocalChams = Color.FromArgb(255, 0, 0, 0);
            G.C.Clr.MoveHelper = Color.FromArgb(255, 0, 255, 0);
            G.C.Clr.RecoilCrosshair = Color.FromArgb(255, 255, 255, 255);
            G.C.Clr.Rotation = Color.FromArgb(255, 255, 0, 255);
            G.C.Clr.SilentAimFov = Color.FromArgb(255, 55, 155, 255);
            G.C.Clr.SkeletonAllies = Color.FromArgb(255, 255, 255, 255);
            G.C.Clr.SkeletonEnemies = Color.FromArgb(255, 255, 35, 0);
            G.C.Clr.SkeletonEnemiesVisible = Color.FromArgb(255, 255, 255, 0);
            G.C.Clr.SkeletonFillAllies = Color.FromArgb(255, 255, 255, 255);
            G.C.Clr.SkeletonFillEnemies = Color.FromArgb(255, 0, 255, 0);
            G.C.Clr.SkeletonFillEnemiesVisible = Color.FromArgb(255, 0, 255, 0);
            G.C.Clr.Text = Color.FromArgb(255, 255, 255, 255);
            G.C.Clr.TextNice = Color.FromArgb(255, 155, 255, 55);
            G.C.Clr.TextImportant = Color.FromArgb(255, 255, 155, 0);
            G.C.Clr.TextWarning = Color.FromArgb(255, 255, 0, 0);

            G.C.Key.Bunnyhop = Keys.Space;
            G.C.Key.Exit = Keys.Home;
            G.C.Key.ForceUpdate = Keys.F1;
            G.C.Key.Toggle = Keys.Insert;

            G.C.Stts.Antialiasing = true;
            G.C.Stts.BetterAntialiasing = true;
            G.C.Stts.ConfigVersion = Settings.ConfigVersion;
            G.C.Stts.FontName = "Exo 2";
            G.C.Stts.WindowWidth = 600;
        }
    }
}