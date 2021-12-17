using System;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;
using CSGSI;
using CSGSI.Nodes;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;
using Steam4NET;
using Microsoft.Win32;
using System.Text.RegularExpressions;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using Win32Interop.Methods;
using NAudio.CoreAudioApi;
using NAudio.Wave;
using System.Media;
using System.Text;
using System.Globalization;

namespace CSGSITools
{
    public partial class CSGSITools_Form : MetroFramework.Forms.MetroForm
    {
        #region test
        private static ISteam006 steam006;
        private static ISteamClient012 steamclient;
        private static ISteamUser016 steamuser;
        private static ISteamFriends013 steamfriends013;
        private static ISteamFriends002 steamfriends002;
        private static int user;
        private static int pipe;
        private static CSteamID steamid;
        private EPersonaState CurrentState;

        private int LoadSteam()
        {
            if (Steamworks.Load(true))
            {
                Console.WriteLine("Ok, Steam Works!");
            }
            else
            {
                MessageBox.Show("Failed, Steam Works!");
                Console.WriteLine("Failed, Steam Works!");

                return -1;
            }

            steam006 = Steamworks.CreateSteamInterface<ISteam006>();

            steamclient = Steamworks.CreateInterface<ISteamClient012>();
            pipe = steamclient.CreateSteamPipe();
            user = steamclient.ConnectToGlobalUser(pipe);
            steamuser = steamclient.GetISteamUser<ISteamUser016>(user, pipe);
            steamfriends013 = steamclient.GetISteamFriends<ISteamFriends013>(user, pipe);
            steamfriends002 = steamclient.GetISteamFriends<ISteamFriends002>(user, pipe);
            CSteamID steamID = steamuser.GetSteamID();

            CurrentState = steamfriends002.GetFriendPersonaState(steamID);

            string ConvertTo64 = steamID.ConvertToUint64().ToString();
            txtBox_steamID.Text = ConvertTo64;
            steamid = steamID;
            if (steam006 == null)
            {
                Console.WriteLine("steam006 is null !");
                return -1;
            }
            if (steamclient == null)
            {
                Console.WriteLine("steamclient is null !");
                return -1;
            }


            return 0;

        }
        #endregion

        DisplayConfiguration.PHYSICAL_MONITOR[] physicalMonitors;

        GameStateListener gsl;

        public static bool IsPlanted;
        public static PlayerActivity Activity = PlayerActivity.Undefined;
        public static int Health = -1;
        public static int Ammo = -1;
        public static int AmmoMax = -1;
        public static int Armor = -1;
        public static PlayerTeam Team = PlayerTeam.Undefined;
        public static WeaponType WpType = WeaponType.Undefined;
        public static RoundWinTeam WinTeam;
        public static RoundPhase Phase;

        private static MapMode mode = MapMode.Undefined;
        private static readonly int[] scores = new int[2] { 0, 0 }; //CT-T
        private static string map = "Undefined";
        private static readonly int[] stats = new int[3] { 0, 0, 0 }; //Kills, Assists, Deaths
        private static readonly int[] Grenades = new int[3] { 0, 0, 0 }; //Flash, Smoke, Molly

        private static string csgoCFGPath;

        private MMDevice targetDevice;
        private WasapiLoopbackCapture capture = new WasapiLoopbackCapture();


        private void Audio_Load(object sender, EventArgs e)
        {
            //  this.capture.add_DataAvailable(new EventHandler<WaveInEventArgs>(this.waveIn_DataAvailable));

            MMDeviceEnumerator enumerator = new MMDeviceEnumerator();
            //  MMDeviceCollection source = enumerator.EnumerateAudioEndPoints(2, 1);
            //  comboAudioDevice.Items.AddRange(source.ToArray<MMDevice>());
            // comboAudioDevice.SelectedItem = enumerator.GetDefaultAudioEndpoint(0, 1);
            // this.targetDevice = comboAudioDevice.SelectedItem as MMDevice;

        }



        public CSGSITools_Form()
        {
            InitializeComponent();

            this.FormBorderStyle = FormBorderStyle.None;
            Region = Region.FromHrgn(Gdi32.CreateRoundRectRgn(0, 0, Width, Height, 5, 5));

            lbl_currentMap.Visible = false;
            lbl_CTRounds.Visible = false;
            lbl_TRounds.Visible = false;
            lbl_playerstate.Visible = false;
            lbl_currentRoundState.Visible = false;

            cb_Alertsounds.SelectedIndex = 0;
            cb_focus.SelectedIndex = 0;
            combo_states.SelectedIndex = 8;

            TrolhaTimer.Tick += TrolhaTimer_Tick;
            lbl_version.Text = Program.Version;

            physicalMonitors = DisplayConfiguration.GetPhysicalMonitors(DisplayConfiguration.GetCurrentMonitor());
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            LoadCSGOFolder();
            metroTab_csgsiTools.SelectedIndex = 0;
            ps_status.Visible = true;
            CheckCSGOProcess();

            gslStart();
            LoadSteam();
            TrolhaTimer.Enabled = true;


            lbl_currentMap.Visible = true;
            lbl_CTRounds.Visible = true;
            lbl_TRounds.Visible = true;
            lbl_playerstate.Visible = true;
            lbl_currentRoundState.Visible = true;
        }

        public void KillProcess(string kill)
        {
            foreach (var process in Process.GetProcessesByName(kill))
            {
                process.Kill();
            }
        }


        public static void LoadCSGOFolder()
        {
            string csgoPath = tryLocatingCSGOFolder();
            csgoCFGPath = csgoPath + @"\csgo\cfg\";

            DirectoryInfo d = new DirectoryInfo(csgoCFGPath);
            FileInfo[] Files = d.GetFiles("gamestate_integration_sp0ok3rTool.cfg");

            if (Files.Length == 0)
            {
                Process.GetProcesses()
                         .Where(x => x.ProcessName.ToLower()
                                      .Contains("csgo"))
                         .ToList()
                         .ForEach(x => x.Kill());

                string fileToCopy = Program.ExecutablePath + "\\gamestate_integration_sp0ok3rTool.cfg";
                File.Copy(fileToCopy, csgoCFGPath + Path.GetFileName(fileToCopy));
            }
        }

        private void gslStart()
        {
            gsl = new GameStateListener(3000);
            //gsl.NewGameState += new NewGameStateHandler(Bomb_State);
            gsl.NewGameState += new NewGameStateHandler(Round_State);
            gsl.NewGameState += new NewGameStateHandler(Player_State);


            if (!gsl.Start())
            {
                Environment.Exit(0);
            }
            Console.WriteLine("Listening...");
        }

        private void CheckCSGOProcess()
        {
            Process[] ps = Process.GetProcessesByName("csgo");
            if (ps.Length == 0)
            {
                MessageBox.Show("Starting csgo for you... restarting " + Program.AppName + " in 35sec.", Program.AppName,
                MessageBoxButtons.OK, MessageBoxIcon.Error);

                Process.Start("steam://run/730");

                Thread.Sleep(35000);
                Process.Start(Application.ExecutablePath);
                Application.Exit();
            }
        }


        #region PlayerState
        private void Player_State(GameState gs)
        {

            if (gs.Player.SteamID.Equals(txtBox_steamID.Text.ToString()))
            {

                int Health = gs.Player.State.Health;
                int WeaponAmmoClip = gs.Player.Weapons.ActiveWeapon.AmmoClip;
                string CurrentWeapon = gs.Player.Weapons.ActiveWeapon.Name;

                if (gs.Player.Activity == PlayerActivity.Undefined)
                {
                    lbl_playerstate.ForeColor = Color.Gold;
                    lbl_playerstate.Text = "Undefined";

                }
                else if (gs.Player.Activity == PlayerActivity.Menu)
                {//Is in a menu (also ESC).

                    lbl_playerstate.ForeColor = Color.Gold;
                    lbl_playerstate.Text = "In Menu";
                }
                else if (gs.Player.Activity == PlayerActivity.Playing)
                {//Playing or spectating.
                    if (gs.Map.Mode != mode) mode = gs.Map.Mode;
                    if (gs.Map.Name != map) map = gs.Map.Name.Split('/').Last();

                    //KD Stats
                    if (gs.Player.MatchStats.Kills != stats[0]) stats[0] = gs.Player.MatchStats.Kills;
                    if (gs.Player.MatchStats.Assists != stats[1]) stats[1] = gs.Player.MatchStats.Assists;
                    if (gs.Player.MatchStats.Deaths != stats[2]) stats[2] = gs.Player.MatchStats.Deaths;

                    //lbl_playerKills.Text = stats[0].ToString();
                    // Grenades
                    if (gs.Player.State.Flashed != Grenades[0]) Grenades[0] = gs.Player.State.Flashed;
                    if (gs.Player.State.Smoked != Grenades[1]) Grenades[1] = gs.Player.State.Smoked;
                    if (gs.Player.State.Burning != Grenades[2]) Grenades[2] = gs.Player.State.Burning;

                    if (gs.Map.TeamCT.Score != scores[0]) scores[0] = gs.Map.TeamCT.Score;
                    if (gs.Map.TeamT.Score != scores[1]) scores[1] = gs.Map.TeamT.Score;

                    lbl_CTRounds.Text = scores[0].ToString();
                    lbl_TRounds.Text = scores[1].ToString();

                    lbl_playerScore.Text = "K/D/A: " + stats[0] + "/" + stats[1] + "/" + stats[2];




                    if (Health == 0)
                    {
                        lbl_playerstate.ForeColor = Color.DarkRed;
                        lbl_playerstate.Text = "Dead";
                    }
                    else
                    {
                        lbl_playerstate.ForeColor = Color.Green;
                        lbl_playerstate.Text = "Alive";
                    }
                }
                else if (gs.Player.Activity == PlayerActivity.TextInput)
                {//Console is open

                }

                //misc
                //FLASHED
                if (Grenades[0] > 0)
                {
                    lbl_playerstate.ForeColor = Color.White;
                    lbl_playerstate.Text = "Flashed";

                }
                //SMOKED
                if (Grenades[1] > 0)
                {
                    lbl_playerstate.ForeColor = Color.Gray;
                    lbl_playerstate.Text = "In Smoke";
                }
                //BURNING
                if (Grenades[2] > 0)
                {
                    lbl_playerstate.ForeColor = Color.Orange;
                    lbl_playerstate.Text = "In Molly";
                }
            }
            else
            {
                lbl_playerstate.ForeColor = Color.Gold;
                lbl_playerstate.Text = "Spectating...";
            }
        }
        #endregion

        #region BombState
        private void Bomb_State(GameState gs)
        {
            if (gs.Round.Phase == RoundPhase.Live &&
               gs.Bomb.State == BombState.Planted &&
               gs.Previously.Bomb.State == BombState.Planting)
            {
                //lbl_bombCurrentState.ForeColor = Color.Red;
                //lbl_bombCurrentState.Text = "Bomb has been planted";

                float a = gs.Bomb.Countdown;
                Console.WriteLine(a);
                IsPlanted = true;

            }
            else if (gs.Round.Bomb == BombState.Exploded)
            {
                //lbl_bombCurrentState.ForeColor = Color.Red;
                //lbl_bombCurrentState.Text = "Exploded";

            }
            else if (gs.Round.Bomb == BombState.Defused)
            {
                //lbl_bombCurrentState.ForeColor = Color.DodgerBlue;
                //lbl_bombCurrentState.Text = "Defused";

            }
            else
            {
                IsPlanted = false;
                //lbl_bombCurrentState.ForeColor = Color.Red;
                //lbl_bombCurrentState.Text = "No server";

            }
        }
        #endregion

        #region RoundState
        private void Round_State(GameState gs)
        {
            lbl_currentMap.ForeColor = Color.White;
            lbl_currentMap.Text = gs.Map.Name.Split('/').Last();

            if (gs.Round.Phase == RoundPhase.Over)
            {
                if (gs.Round.WinTeam != RoundWinTeam.Undefined)
                {
                    if (gs.Round.WinTeam == RoundWinTeam.CT)
                    {
                        lbl_currentRoundState.ForeColor = Color.DodgerBlue;
                    }
                    else
                    {
                        lbl_currentRoundState.ForeColor = Color.IndianRed;
                    }
                    lbl_currentRoundState.Text = gs.Round.WinTeam + " - Wins";
                }
            }
            else if (gs.Round.Phase == RoundPhase.Live)
            {
                lbl_currentRoundState.ForeColor = Color.DarkGreen;
                lbl_currentRoundState.Text = "Live";

                if (cb_focus.SelectedIndex == 1
                    && gs.Player.SteamID.Equals(txtBox_steamID.Text.ToString())
                    && gs.Player.State.Health == 100)
                {
                    FocusProcess("csgo");
                }


            }
            else if (gs.Round.Phase == RoundPhase.FreezeTime)
            {
                lbl_currentRoundState.ForeColor = Color.DarkOrange;
                lbl_currentRoundState.Text = "* Freeze Time *";


                if (cb_focus.SelectedIndex == 2
                    && gs.Player.SteamID.Equals(txtBox_steamID.Text.ToString())
                    && gs.Player.State.Health == 100)
                {

                    FocusProcess("csgo");
                }

                if (cb_Alertsounds.SelectedIndex != 0 && GetForegroundProcessName() != "csgo")
                {
                    var wtf = Properties.Resources.ResourceManager.GetObject(cb_Alertsounds.SelectedItem.ToString());
                    SoundPlayer snd = new SoundPlayer((Stream)wtf);
                    snd.Play();

                    KillProcess("chrome");
                }
            }

            else if (gs.Round.Phase == RoundPhase.FreezeTime || gs.Round.Phase == RoundPhase.Undefined && gs.Map.Round == 0)
            {
                //
            }
            else if (gs.Round.Phase == RoundPhase.Undefined)
            {
                lbl_currentRoundState.ForeColor = Color.Red;
                lbl_currentRoundState.Text = "No server";

                lbl_currentMap.ForeColor = Color.Red;
                lbl_currentMap.Text = "No server";


                lbl_CTRounds.ForeColor = Color.Red;
                lbl_CTRounds.Text = "0";
                lbl_TRounds.ForeColor = Color.Red;
                lbl_TRounds.Text = "0";

            }
            else if (gs.Round.Phase != RoundPhase.Undefined)
            {

            }
        }
        #endregion

        #region FocusProcess - http://josephgozlan.blogspot.com/2013/02/c-bring-another-application-to.html
        [DllImport("user32.dll")]
        public static extern bool ShowWindowAsync(HandleRef hWnd, int nCmdShow);
        [DllImport("user32.dll")]
        public static extern bool SetForegroundWindow(IntPtr WindowHandle);
        public const int SW_RESTORE = 9;

        private void FocusProcess(string procName)
        {
            Process[] objProcesses = Process.GetProcessesByName(procName);
            if (objProcesses.Length > 0)
            {
                IntPtr hWnd = IntPtr.Zero;
                hWnd = objProcesses[0].MainWindowHandle;
                ShowWindowAsync(new HandleRef(null, hWnd), SW_RESTORE);
                SetForegroundWindow(objProcesses[0].MainWindowHandle);
            }
        }


        [DllImport("user32.dll")]
        private static extern Int32 GetWindowThreadProcessId(IntPtr hWnd, out uint lpdwProcessId);

        // Returns the name of the process owning the foreground window.
        private string GetForegroundProcessName()
        {
            IntPtr hwnd = GetForegroundWindow();

            // The foreground window can be NULL in certain circumstances, 
            // such as when a window is losing activation.
            if (hwnd == null)
                return "Unknown";

            uint pid;
            GetWindowThreadProcessId(hwnd, out pid);

            foreach (Process p in Process.GetProcesses())
            {
                if (p.Id == pid)
                    return p.ProcessName;
            }

            return "Unknown";
        }
        #endregion

        #region WindowMover

        private const int WM_NCLBUTTONDBLCLK = 0x00A3;

        protected override void WndProc(ref Message m)
        {
            if (m.Msg == WM_NCLBUTTONDBLCLK)
            {
                m.Result = IntPtr.Zero;
                return;
            }

            switch (m.Msg)
            {
                case 0x84:
                    base.WndProc(ref m);
                    if ((int)m.Result == 0x1)
                        m.Result = (IntPtr)0x2;
                    return;
            }
            base.WndProc(ref m);
        }
        #endregion

        public static void SetStatus(int Number)
        {
            if (Number < 8)
            {
                steamfriends002.SetPersonaState((EPersonaState)int.Parse(Number.ToString()));
            }
        }

        private void TrolhaTimer_Tick(object sender, EventArgs e)
        {
            string FriendPersonaState = steamfriends002.GetFriendPersonaState(steamid.ConvertToUint64()).ToString().Replace("k_EPersonaState", "");

            switch (FriendPersonaState)
            {
                case "Offline":
                    lbl_currentSteamState.ForeColor = Color.Gray;
                    lbl_currentSteamState.Text = FriendPersonaState;
                    break;
                case "Online":
                    lbl_currentSteamState.ForeColor = Color.DodgerBlue;
                    lbl_currentSteamState.Text = FriendPersonaState;
                    break;
                case "Away":
                case "Busy":
                case "Snooze":
                    lbl_currentSteamState.ForeColor = Color.Orange;
                    lbl_currentSteamState.Text = FriendPersonaState;
                    break;
                case "LookingToTrade":
                case "LookingToPlay":
                    lbl_currentSteamState.ForeColor = Color.DodgerBlue;
                    lbl_currentSteamState.Text = FriendPersonaState;
                    break;
                default:
                    lbl_currentSteamState.ForeColor = Color.White;
                    lbl_currentSteamState.Text = FriendPersonaState;
                    break;
            }

        }

        #region ComboBox_Select_States_steam
        private void combo_states_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (this.combo_states.SelectedIndex < 8)
            {
                SetStatus(this.combo_states.SelectedIndex);
            }
        }
        #endregion
        public string getActiveWindowName()
        {
            try
            {
                var activatedHandle = GetForegroundWindow();

                Process[] processes = Process.GetProcesses();
                foreach (Process clsProcess in processes)
                {

                    if (activatedHandle == clsProcess.MainWindowHandle)
                    {
                        string processName = clsProcess.ProcessName;

                        return processName;
                    }
                }
            }
            catch { }
            return null;
        }

        [DllImport("user32.dll", CharSet = CharSet.Auto, ExactSpelling = true)]
        private static extern IntPtr GetForegroundWindow();

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            //SetStatus(1);
            Steamworks.Load(false);
            if (gsl != null)
            {
                gsl.Stop();
            }
            Application.ExitThread();
            Environment.Exit(0);
        }

        #region LINKS
        private void pictureBox_Github_Click(object sender, EventArgs e)
        {
            Process.Start("https://github.com/sp0ok3r/CSGSITools");
        }

        private void metroLink_spk_Click(object sender, EventArgs e)
        {
            Process.Start("http://steamcommunity.com/profiles/76561198041931474");
        }

        private void metroLink_spkMusic_Click(object sender, EventArgs e)
        {
            Process.Start("https://www.youtube.com/watch?v=YSOUfUsM0zY");
        }

        private void metroLink_valveGSI_Click(object sender, EventArgs e)
        {
            Process.Start("https://developer.valvesoftware.com/wiki/Counter-Strike:_Global_Offensive_Game_State_Integration");
        }

        private void metroLink_Rakijah_Click(object sender, EventArgs e)
        {
            Process.Start("https://github.com/rakijah/CSGSI");
        }

        private void metroLink_Json_Click(object sender, EventArgs e)
        {
            Process.Start("https://www.newtonsoft.com/json");
        }

        private void metroLink_Steam4net_Click(object sender, EventArgs e)
        {
            Process.Start("https://github.com/SteamRE/Steam4NET");
        }

        private void metroLink_Metro_Click(object sender, EventArgs e)
        {
            Process.Start("http://denricdenise.info/metroframework-faq/");
        }
        #endregion

        // https://developer.valvesoftware.com/wiki/Counter-Strike:_Global_Offensive_Game_State_Integration#Locating_CS:GO_Install_Directory
        // Improved csgo installation detection by bernieplayshd #14
        private static string tryLocatingCSGOFolder()
        {
            // Locate the Steam installation directory
            string steamDir = (string)Registry.GetValue(@"HKEY_CURRENT_USER\Software\Valve\Steam", "SteamPath", ""),
                   libsFile = steamDir + @"\steamapps\libraryfolders.vdf";

            Regex regex = new Regex("\"\\d+\".*\"(.*?)\"", RegexOptions.Compiled);

            List<string> libraries = new List<string> { steamDir.Replace('/', '\\') };

            // Find all Steam game libraries
            if (File.Exists(libsFile))
            {
                foreach (string line in File.ReadAllLines(libsFile))
                {
                    foreach (Match match in regex.Matches(line))
                    {
                        if (match.Success && match.Groups.Count != 0)
                        {
                            libraries.Add(match.Groups[1].Value.Replace("\\\\", "\\"));
                            break;
                        }
                    }
                }
            }

            // Search them for the CS:GO installation
            foreach (string lib in libraries)
            {
                string csgoDir = lib + @"\steamapps\common\Counter-Strike Global Offensive";
                if (Directory.Exists(csgoDir))
                {
                    return csgoDir;
                }
            }

            return null;
        }

        private void btn_getMonitor_Click(object sender, EventArgs e)
        {
            //MessageBox.Show(DisplayConfiguration.GetCurrentMonitor().ToString());


            foreach (DisplayConfiguration.PHYSICAL_MONITOR physicalMonitor in physicalMonitors)
            {
                Console.WriteLine(DisplayConfiguration.GetMonitorCapabilities(physicalMonitor));
                Console.WriteLine(physicalMonitor.hPhysicalMonitor.ToInt32());
                //DisplayConfiguration.SetMonitorBrightness(physicalMonitor, brightnessSlider.Value / brightnessSlider.Maximum);
            }
        }

        private void metroLabel13_Click(object sender, EventArgs e)
        {

        }
    }
}