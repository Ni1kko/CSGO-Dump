using System;
using System.Diagnostics; 
using System.Collections.Generic; 
using System.Threading; 
using System.Windows.Forms; 
using GameInfoPlus.Classes;
using AffinityOffsetUpdater; 

namespace GameInfoPlus
{
    internal static class Program
    { 
        public static bool RunStartup()
        {
            _ = new Offsets();//get
            Memory.offsets = Offsets._RootObject;//set

            Process[] CSGO = Process.GetProcessesByName("csgo");
            if (CSGO.Length != 0)
            {
                Memory.Process = CSGO[0];
                Memory.ProcessHandle = Memory.OpenProcess(0x0008 | 0x0010 | 0x0020, false, Memory.Process.Id);
                foreach (ProcessModule Module in Memory.Process.Modules)
                {
                    switch (Module.ModuleName)
                    {
                        case "client_panorama.dll":
                            Memory.Client = Module.BaseAddress;
                            break;
                        case "engine.dll":
                            Memory.Engine = Module.BaseAddress;
                            Game.Engine = new Engine((int)Module.BaseAddress);
                            break;
                    }
                    
                }
                return true;
            }
            else
            {
                Utility.DisplayErrorMessage("No CSGO Could be detected, Please Ensure that CS:GO Is running before opening.", "Error - No CSGO");
                Environment.Exit(-1); 
                return false;
            }
        }

        /// <summary>
        /// 
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        { 
            if (RunStartup())
            {
                Memory.handle = Memory.FindWindow(null, Memory.WindName);
                new Thread(() =>
                {
                    Thread.CurrentThread.IsBackground = true;
                    while (true)
                    {
                        // Get Players
                        var oldEntityList = new List<Entity>();
                        oldEntityList.Clear();
                        for (int i = 1; i <= 64; i++)
                        {
                           var ent = new Entity(Entity.GetEntityBase(i));
                            if (ent.Valid) oldEntityList.Add(ent);
                        }
                        Game.EntityList = oldEntityList;
                        Thread.Sleep(1000);
                    }
                }).Start();
            }
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new MainForm());
        }
    }
}