using ScriptKidAntiCheat.Utils;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Timers;

namespace ScriptKidAntiCheat
{
    public class GameConsole
    {
        private System.Timers.Timer writeTimer;

        private string CFG_PATH;

        private FileInfo CFG_INFO;

        private bool isWriting = false;

        private List<string> CommandQueue = new List<string>();

        public static bool cfgIsReady = true;

        public GameConsole()
        {
            // Lets add required keybinding to users cs go config on first setup
            if(!checkIfCfgIsReady())
            {
                setupUserConfigs();
            }
            
            // Create cfg file used to send commands to console using exec
            CFG_PATH = Helper.getPathToCSGO() + @"\cfg\cheater.cfg";

            if (!File.Exists(CFG_PATH))
            {
                File.Create(CFG_PATH);
            }

            CFG_INFO = new FileInfo(CFG_PATH);

            // Start timer that writes our commands to our cheater.cfg
            writeTimer = new System.Timers.Timer(100);
            writeTimer.Elapsed += fileWriter;
            writeTimer.AutoReset = true;
            writeTimer.Enabled = true;
        }

        private void setupUserConfigs()
        {
            
            string[] users = Directory.GetDirectories(Helper.getPathToSteam() + @"\userdata");
            foreach (string user in users)
            {
                string userConfig = user + @"\730\local\cfg\config.cfg";

                // Add keybinding to execute our console commands to the player default config
                if (File.Exists(userConfig))
                {

                    // Add our keybinding to player cfg
                    using (StreamWriter sw = File.AppendText(userConfig))
                    {
                        // Bind F9 in player config to exec our cheater.cfg file
                        sw.WriteLine("\rbind \"F9\" \"exec cheater.cfg\"");
                    } 
                }
            }
        }

        public bool checkIfCfgIsReady()
        {
            bool isReady = true;

            string[] users = Directory.GetDirectories(Helper.getPathToSteam() + @"\userdata");
            foreach (string user in users)
            {
                string userConfig = user + @"\730\local\cfg\config.cfg";

                // Add keybinding to execute our console commands to the player default config
                if (File.Exists(userConfig))
                {

                    // Check if we already tempered with the config
                    using (StreamReader sr = new StreamReader(userConfig))
                    {
                        string contents = sr.ReadToEnd();
                        if (!contents.Contains("cheater.cfg"))
                        {
                            isReady = false;
                        }
                    }
                }
            }

            cfgIsReady = isReady;

            return isReady;
        }

        public void SendCommand(string Command)
        {
            CommandQueue.Add(Command);
        }

        private void fileWriter(Object source, ElapsedEventArgs e)
        {
            // Check if we have any commands queued and file is not being written to already
            if (Helper.IsFileLocked(CFG_INFO) || isWriting || CommandQueue.Count < 1)
            {
                return;
            }

            // Write our commands to cheater.cfg
            writeToCFG();
        }

        private void writeToCFG()
        {
            if (!Program.GameProcess.IsValidAndActiveWindow) return;

            isWriting = true;

            string combinedCommands = "";
            foreach (string Command in CommandQueue.ToList())
            {
                if (!Command.EndsWith(";"))
                {
                    combinedCommands += Command + ";";
                } else
                {
                    combinedCommands += Command;
                }
                CommandQueue.Remove(Command);
            }

            try
            {
                // Write commands to our cheater.cfg file
                using (var sw = new StreamWriter(CFG_PATH, false))
                {
                    sw.WriteLine(combinedCommands);
                    sw.Close();
                }
                //User32.SetForegroundWindow(MemoryReader.GameWindow);

                // Trigger console exec by simulating keypress F9
                SendInput.KeyPress(KeyCode.F9);
            }
            catch (IOException e)
            {
                
            }

            isWriting = false;

        }
    }
}
