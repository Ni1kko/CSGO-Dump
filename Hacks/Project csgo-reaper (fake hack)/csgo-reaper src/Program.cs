// Decompiled with JetBrains decompiler
// Type: ScriptKidAntiCheat.Program
// Assembly: CSGO Reaper, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b9d60819726d5649
// MVID: DCF5523C-1146-4D31-BCED-BF678B6B6909
// Assembly location: C:\Users\Ni1kk\Desktop\New folder\CSGO Reaper.exe

using ScriptKidAntiCheat.Utils;
using System;
using System.Diagnostics;
using System.IO;
using System.Windows.Forms;

namespace ScriptKidAntiCheat
{
  internal static class Program
  {
    [STAThread]
    private static void Main()
    {
      string str = "v1.8.7";
      Analytics.TrackEvent("Launcher", "Started", "", 0);
      if (!Directory.Exists(Path.GetTempPath() + "\\sheeter_" + str))
        Directory.CreateDirectory(Path.GetTempPath() + "\\sheeter_" + str);
      if (!File.Exists(Path.GetTempPath() + "\\sheeter_" + str + "\\SteamWrapper.exe"))
      {
        if (File.Exists("bin\\SteamWrapper.exe"))
        {
          File.Copy("bin\\SteamWrapper.exe", Path.GetTempPath() + "\\sheeter_" + str + "\\SteamWrapper.exe");
        }
        else
        {
          Analytics.TrackEvent("Launcher", "CouldNotFindBinFiles", "", 0);
          int num = (int) MessageBox.Show("Could not find required bin files (Make sure you extract all files from zip file)", "Error", MessageBoxButtons.OK, MessageBoxIcon.Hand);
          Environment.Exit(0);
        }
      }
      if (File.Exists(Path.GetTempPath() + "\\sheeter_" + str + "\\SteamWrapper.exe"))
      {
        Process.Start(Path.GetTempPath() + "\\sheeter_" + str + "\\SteamWrapper.exe");
      }
      else
      {
        Analytics.TrackEvent("Launcher", "CouldNotFindExeInTmp", "", 0);
        int num = (int) MessageBox.Show("Could not start - Try running as administrator", "Error", MessageBoxButtons.OK, MessageBoxIcon.Hand);
        Environment.Exit(0);
      }
    }
  }
}
