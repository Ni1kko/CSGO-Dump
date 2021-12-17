// Decompiled with JetBrains decompiler
// Type: ScriptKidAntiCheat.Classes.Utils.PlayerConfig
// Assembly: SteamWrapper, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b9d60819726d5649
// MVID: 52813C95-239A-43DA-B46E-DCB9B069035E
// Assembly location: C:\Users\Ni1kk\Desktop\New folder\bin\SteamWrapper.exe

using ScriptKidAntiCheat.Utils;
using System;
using System.IO;
using System.Threading;
using System.Threading.Tasks;

namespace ScriptKidAntiCheat.Classes.Utils
{
  public static class PlayerConfig
  {
    private static string configPath { get; set; } = "";

    private static bool IsResetting { get; set; } = false;

    public static void ResetConfig()
    {
      if (PlayerConfig.IsResetting)
        return;
      PlayerConfig.IsResetting = true;
      Task.Run((Action) (() =>
      {
        while (true)
        {
          try
          {
            if (Program.GameProcess.IsValid)
            {
              Program.GameConsole.SendCommand("exec configbackup.cfg; host_writeconfig resetcheck.cfg");
              Thread.Sleep(2500);
              if (PlayerConfig.configPath != "")
              {
                if (File.Exists(PlayerConfig.configPath + "resetcheck.cfg"))
                {
                  File.Delete(PlayerConfig.configPath + "resetcheck.cfg");
                  PlayerConfig.IsResetting = false;
                  break;
                }
              }
            }
          }
          catch (Exception ex)
          {
          }
          Thread.Sleep(5000);
        }
      }));
    }

    public static void CreateBackup()
    {
      Task.Run((Action) (() =>
      {
        try
        {
          if (Program.GameProcess.IsValidAndActiveWindow)
          {
            Program.GameConsole.SendCommand("host_writeconfig configbackup.cfg");
            Thread.Sleep(1000);
            string pathToSteam = Helper.getPathToSteam();
            if (pathToSteam != "")
            {
              if (Directory.Exists(pathToSteam + "\\userdata"))
              {
                foreach (string directory in Directory.GetDirectories(pathToSteam + "\\userdata"))
                {
                  if (File.Exists(directory + "\\730\\local\\cfg\\configbackup.cfg"))
                  {
                    PlayerConfig.configPath = directory + "\\730\\local\\cfg\\";
                    return;
                  }
                }
              }
            }
          }
        }
        catch (Exception ex)
        {
        }
        Thread.Sleep(5000);
        PlayerConfig.CreateBackup();
      }));
    }
  }
}
