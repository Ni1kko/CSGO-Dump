using ScriptKidAntiCheat.Classes;
using System;
using System.Windows.Forms;
using ScriptKidAntiCheat.Data;
using ScriptKidAntiCheat.Utils;
using Gma.System.MouseKeyHook;
using ScriptKidAntiCheat.Forms;
using System.Diagnostics;

namespace ScriptKidAntiCheat
{
    static class Program
    {
        public static string version = "v1.8.7";
        public static GameProcess GameProcess;
        public static GameConsole GameConsole;
        public static FakeCheat FakeCheat;
        public static GameData GameData;
        public static Utils.Debug Debug;
        public static IKeyboardMouseEvents m_GlobalHook;

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            Debug = new Utils.Debug();

            // EDIT THIS ACCORDING TO YOUR NEEDS:
            Debug.AllowLocal = true;
            Debug.AllowInWarmup = true;
            Debug.DisableGoogleDriveUpload = true;
            Debug.DisableRunInBackground = false;
            Debug.DisableAcceptConditions = false;
            Debug.IgnoreActivateOnRound = true;
            Debug.ShowDebugMessages = true;
            Debug.TripWireStage = 2;
            // ---

            // Make the cheater accept terms and conditions
            if (Debug.DisableAcceptConditions == false)
            {
                Application.Run(new Conditions());
            }

            // Setup keyboard / mouse event handler
            m_GlobalHook = Hook.GlobalEvents();

            // Check how many instances of the fake cheat is running
            Process[] isAlreadyInitialized = Process.GetProcessesByName("CSGO Reaper"); // SteamWrapper is our fake process name

            // Setup our memory reader and fake cheat process (only if its not already running)
            if (Helper.getPathToCSGO() != "" && isAlreadyInitialized.Length == 1)
            {
                GameConsole = new GameConsole();
                GameProcess = new GameProcess();
                GameData = new GameData(GameProcess);
                GameProcess.Start();
                GameData.Start();
                FakeCheat = new FakeCheat();
            }

            // Run fake ui form
            Application.Run(new FakeCheatForm());

            // Run hidden application once they close main window (only if its not already running)
            if (isAlreadyInitialized.Length == 1 && Debug.DisableRunInBackground != true)
            {
                System.Windows.Forms.MessageBox.Show("The fake cheat will now keep running in the background! You need to close the process ScriptKidAntiCheat in Task Manager to shut it down.", "WARNING", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Warning);
                Application.Run(new Hidden());
            }

            if(Helper.getPathToCSGO() != "" && isAlreadyInitialized.Length == 1)
            {
                Dispose();
            }
        }

        private static void Dispose()
        {
            GameData.Dispose();
            GameData = default;
            GameProcess.Dispose();
            GameProcess = default;
        }

    }
}
