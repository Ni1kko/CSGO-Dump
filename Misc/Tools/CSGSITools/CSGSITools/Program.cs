using System;
using System.Diagnostics;
using System.IO;
using System.Windows.Forms;

namespace CSGSITools
{
    static class Program
    {
        public static string AppName = "CSGSITools";
        public static string Version = "1.0.0beta";

        public static readonly string ExecutablePath = Path.GetDirectoryName(Application.ExecutablePath);

        public static readonly Process[] CurrentProcesses = Process.GetProcesses();

        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new CSGSITools_Form());
        }
    }
}
