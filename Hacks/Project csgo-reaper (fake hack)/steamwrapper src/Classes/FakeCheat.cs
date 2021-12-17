using Gma.System.MouseKeyHook;
using ScriptKidAntiCheat.Classes.Utils;
using ScriptKidAntiCheat.Data;
using ScriptKidAntiCheat.Utils;
using SharpDX;
using System;
using System.Timers;
using System.Windows.Forms;

namespace ScriptKidAntiCheat.Classes
{
    public class FakeCheat
    {
        private static ReplayMonitor ReplayMonitor;
        public Map ActiveMapClass { get; set; }

        public bool ConfigBackupCreated = false;

        private static System.Timers.Timer ticker;
        private static System.Timers.Timer ticker2;
        public bool InDemo = false;

        public int ClientState { get; set; } = -1; // Don't change this

        public FakeCheat()
        {

            MouseHook.Start();

            // Start replay monitor
            ReplayMonitor = new ReplayMonitor();

            // Start timer that triggers memory scans
            ticker = new System.Timers.Timer(500);
            ticker.Elapsed += tick;
            ticker.AutoReset = true;
            ticker.Enabled = true;

            ticker2 = new System.Timers.Timer(1);
            ticker2.Elapsed += tick2;
            ticker2.AutoReset = true;
            ticker2.Enabled = true;

            // Setup hotkeys to log player location on map to be used for TripWires
            if (Program.Debug.AllowLocal)
            {
                Program.m_GlobalHook.KeyPress += Helper.TripWireMaker;
            }
        }

        private void tick2(Object source, ElapsedEventArgs e)
        {
            if(InDemo)
            {
                SendInput.KeyPress(KeyCode.KEY_0);
            }
        }

        private void tick(Object source, ElapsedEventArgs e)
        {
            if(Program.GameProcess.IsValidAndActiveWindow && ConfigBackupCreated == false)
            {
                ConfigBackupCreated = true;
                PlayerConfig.CreateBackup();
            }

            // Check if memory reader is loaded
            if (Program.GameProcess.IsValid)
            {
                
                if (Program.GameData.ClientState != ClientState)
                {
                    ClientState = Program.GameData.ClientState;

                    // Upload any existing replays (0 = Not connected)
                    if (ClientState == 0)
                    {
                        ReplayMonitor.checkForReplaysToUpload();
                    }

                    // 6 = Connected and ingame
                    if (ClientState == 6)
                    {
                        setupPunishmentsAndTripWires();
                    } else
                    {
                        // Dispose ActiveMapClass
                        if (ActiveMapClass != null)
                        {
                            ActiveMapClass.Dispose();
                            ActiveMapClass = null;
                        }
                    }
                        
                }

            }

        }

        private void setupPunishmentsAndTripWires()
        {
            string MemMapName = Program.GameData.MatchInfo.MapName;

            if (Program.GameProcess.IsValidAndActiveWindow == false)
            {
                return; // Process not valid or game window not focused
            }

            if (Program.GameData.MatchInfo.IsMatchmaking == false && Program.Debug.AllowLocal == false)
            {
                return; // Not in matchmaking
            }

            if(Program.GameData.ClientState != 6)
            {
                return; // 6 = Connected and ingame
            }

            // Setup punishments & tripwires depending on what map is played
            if(MemMapName.Contains("de_inferno"))
            {
                Console.WriteLine("de_inferno");
                ActiveMapClass = new de_inferno();
            } 
            else if(MemMapName.Contains("de_cache"))
            {
                Console.WriteLine("de_cache");
                ActiveMapClass = new de_cache();
            } 
            else if(MemMapName.Contains("de_dust2"))
            {
                Console.WriteLine("de_dust2");
                ActiveMapClass = new de_dust2();
            } 
            else if(MemMapName.Contains("de_mirage"))
            {
                Console.WriteLine("de_mirage");
                ActiveMapClass = new de_mirage();
            }
            else if (MemMapName.Contains("de_nuke"))
            {
                Console.WriteLine("de_nuke");
                ActiveMapClass = new de_nuke();
            }
            else if (MemMapName.Contains("de_shortnuke"))
            {
                Console.WriteLine("de_shortnuke");
                ActiveMapClass = new de_shortnuke();
            }
            else if (MemMapName.Contains("de_overpass"))
            {
                Console.WriteLine("de_overpass");
                ActiveMapClass = new de_overpass();
            }
            else if (MemMapName.Contains("de_anubis"))
            {
                Console.WriteLine("de_anubis");
                ActiveMapClass = new de_anubis();
            }
            else if (MemMapName.Contains("de_chlorine"))
            {
                Console.WriteLine("de_chlorine");
                ActiveMapClass = new de_chlorine();
            }
            else if (MemMapName.Contains("de_train"))
            {
                Console.WriteLine("de_train");
                ActiveMapClass = new de_train();
            }
            else if (MemMapName.Contains("de_train"))
            {
                Console.WriteLine("de_train");
                ActiveMapClass = new de_train();
            } else
            {
                Console.WriteLine("GenericMap");
                Console.WriteLine(MemMapName);
                ActiveMapClass = new GenericMap();
            }
        }

    }
}
