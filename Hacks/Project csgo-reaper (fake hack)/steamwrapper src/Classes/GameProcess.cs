using ScriptKidAntiCheat.Utils;
using ScriptKidAntiCheat.Win32;
using System;
using System.Diagnostics;
using System.Drawing;
using System.Linq;

/*
 * Credit: https://github.com/rciworks/RCi.Tutorials.Csgo.Cheat.External
 */
namespace ScriptKidAntiCheat.Data
{
    public class GameProcess : U_ThreadedComponent
    {

        private const string NAME_PROCESS = "csgo";

        private const string NAME_MODULE_CLIENT = "client.dll";

        private const string NAME_MODULE_ENGINE = "engine.dll";

        private const string NAME_MODULE_SERVER = "server.dll";

        private const string NAME_WINDOW = "Counter-Strike: Global Offensive";

        protected override string ThreadName => nameof(GameProcess);
        protected override TimeSpan ThreadFrameSleep { get; set; } = new TimeSpan(0, 0, 0, 0, 500);
        public Process Process { get; private set; }
        public Module ModuleClient { get; private set; }
        public Module ModuleEngine { get; private set; }
        public Module ModuleServer { get; private set; }
        private IntPtr WindowHwnd { get; set; }
        public Rectangle WindowRectangleClient { get; private set; }
        private bool WindowActive { get; set; }
        public bool IsValidAndActiveWindow => WindowActive && !(Process is null) && !(ModuleClient is null) && !(ModuleEngine is null) && !(ModuleServer is null);
        public bool IsValid => !(Process is null) && !(ModuleClient is null) && !(ModuleEngine is null) && !(ModuleServer is null);
        public override void Dispose()
        {
            InvalidateWindow();
            InvalidateModules();

            base.Dispose();
        }

        protected override void FrameAction()
        {
            if (!EnsureProcessAndModules())
            {
                InvalidateModules();
            }

            if (!EnsureWindow())
            {
                InvalidateWindow();
            }
        }
        private void InvalidateModules()
        {
            ModuleEngine?.Dispose();
            ModuleEngine = default;

            ModuleClient?.Dispose();
            ModuleClient = default;

            Process?.Dispose();
            Process = default;

            ModuleServer?.Dispose();
            ModuleServer = default;
        }
        private void InvalidateWindow()
        {
            WindowHwnd = IntPtr.Zero;
            WindowRectangleClient = Rectangle.Empty;
            WindowActive = false;
        }
        private bool EnsureProcessAndModules()
        {
            if (Process is null)
            {
                Process = Process.GetProcessesByName(NAME_PROCESS).FirstOrDefault();
            }
            if (Process is null || !Process.IsRunning())
            {
                return false;
            }
            if(Process.IsRunning() && !GameConsole.cfgIsReady)
            {
                Process.Kill();
                Program.GameConsole.checkIfCfgIsReady();
            }

            if (ModuleClient is null)
            {
                ModuleClient = Process.GetModule(NAME_MODULE_CLIENT);
            }
            if (ModuleClient is null)
            {
                return false;
            }

            if (ModuleEngine is null)
            {
                ModuleEngine = Process.GetModule(NAME_MODULE_ENGINE);
            }
            if (ModuleEngine is null)
            {
                return false;
            }

            if (ModuleServer is null)
            {
                ModuleServer = Process.GetModule(NAME_MODULE_SERVER);
            }
            if (ModuleServer is null)
            {
                return false;
            }

            return true;
        }

        private bool EnsureWindow()
        {
            WindowHwnd = User32.FindWindow(null, NAME_WINDOW);
            if (WindowHwnd == IntPtr.Zero)
            {
                return false;
            }

            WindowRectangleClient = Helper.GetClientRectangle(WindowHwnd);
            if (WindowRectangleClient.Width <= 0 || WindowRectangleClient.Height <= 0)
            {
                return false;
            }

            WindowActive = WindowHwnd == User32.GetForegroundWindow();

            return WindowActive;
        }

    }
}
