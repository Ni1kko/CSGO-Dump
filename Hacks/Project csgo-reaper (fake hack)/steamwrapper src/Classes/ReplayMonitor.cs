using System;
using System.Threading;
using System.Threading.Tasks;
using System.IO;
using ScriptKidAntiCheat.Utils;
using ScriptKidAntiCheat.Win32;

namespace ScriptKidAntiCheat
{
    class ReplayMonitor
    {
        public GoogleDriveUploader GoogleDriveUploader = new GoogleDriveUploader();

        public ReplayMonitor()
        {
            // Generate new google drive token (saved in token.json)
            // GoogleDriveUploader.generateNewToken();
        }

        public void checkForReplaysToUpload()
        {
            if (Program.Debug.DisableGoogleDriveUpload == true) return;

            var CSGO_PATH = Helper.getPathToCSGO();

            try
            {
                if (CSGO_PATH != "")
                {
                    string[] demos = Directory.GetFiles(CSGO_PATH, "*#SHEETER.dem");

                    foreach (string demo in demos)
                    {
                        FileInfo file = new FileInfo(demo);

                        // Analytics.TrackEvent("Replays", "Created");
                        if (Helper.IsFileLocked(file) || file.Length < 3000000)
                        {
                            continue;
                        }

                        // If player is in a match at this point lets stop any ongoing recordings just as a failsafe
                        if(Program.GameProcess.IsValid && Program.GameData.ClientState == 6) // 6 = Connected and ingame
                        {
                            if (!Program.GameProcess.IsValidAndActiveWindow)
                            {
                                User32.SetForegroundWindow(Program.GameProcess.Process.MainWindowHandle);
                                Program.GameConsole.SendCommand("stop");
                            }
                            else if (Program.GameProcess.IsValid)
                            {
                                Program.GameConsole.SendCommand("stop");
                            }
                        }

                        // Let's upload replay to google drive
                        var t = Task.Run(() => {
                            GoogleDriveUploader.UploadFile(file);
                        });
                    }

                }

            }
            catch (Exception ex)
            {
                // YEET
            }

        }

        private void OnNewReplayCreation(object source, FileSystemEventArgs e)
        {
            
           
        }

    }
}
