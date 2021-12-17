// Decompiled with JetBrains decompiler
// Type: ScriptKidAntiCheat.Utils.Helper
// Assembly: SteamWrapper, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b9d60819726d5649
// MVID: 52813C95-239A-43DA-B46E-DCB9B069035E
// Assembly location: C:\Users\Ni1kk\Desktop\New folder\bin\SteamWrapper.exe

using Microsoft.Win32;
using ScriptKidAntiCheat.Internal;
using ScriptKidAntiCheat.Internal.Raw;
using ScriptKidAntiCheat.Utils.Maths;
using ScriptKidAntiCheat.Win32;
using ScriptKidAntiCheat.Win32.Data;
using SharpDX;
using System;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using System.Windows.Forms;

namespace ScriptKidAntiCheat.Utils
{
  public static class Helper
  {
    public static int MOUSEEVENTF_MOVE = 1;
    public static int MaxStudioBones = 128;
    public static int pointerI = 0;
    public static string pointers = "";
    public static string PathToCSGO;
    public static string PathToSteam;
    public static string PlayerNickname;

    public static bool IsFileLocked(FileInfo file)
    {
      FileStream fileStream = (FileStream) null;
      try
      {
        fileStream = file.Open(FileMode.Open, FileAccess.ReadWrite, FileShare.None);
      }
      catch (IOException ex)
      {
        return true;
      }
      finally
      {
        fileStream?.Close();
      }
      return false;
    }

    public static Rectangle GetClientRectangle(IntPtr handle)
    {
      ScriptKidAntiCheat.Win32.Data.Point lpPoint;
      Rect lpRect;
      if (!User32.ClientToScreen(handle, out lpPoint) || !User32.GetClientRect(handle, out lpRect))
        return new Rectangle();
      return new Rectangle(lpPoint.X, lpPoint.Y, lpRect.Right - lpRect.Left, lpRect.Bottom - lpRect.Top);
    }

    public static ProcessModule GetProcessModule(
      this Process process,
      string moduleName)
    {
      if (process == null)
        return (ProcessModule) null;
      return process.Modules.OfType<ProcessModule>().FirstOrDefault<ProcessModule>((Func<ProcessModule, bool>) (a => string.Equals(a.ModuleName.ToLower(), moduleName.ToLower())));
    }

    public static Module GetModule(this Process process, string moduleName)
    {
      ProcessModule processModule = process.GetProcessModule(moduleName);
      if (processModule != null && !(processModule.BaseAddress == IntPtr.Zero))
        return new Module(process, processModule);
      return (Module) null;
    }

    public static bool IsRunning(this Process process)
    {
      try
      {
        Process.GetProcessById(process.Id);
      }
      catch (InvalidOperationException ex)
      {
        return false;
      }
      catch (ArgumentException ex)
      {
        return false;
      }
      return true;
    }

    public static string getPathToCSGO()
    {
      if (Helper.PathToCSGO != null)
        return Helper.PathToCSGO;
      try
      {
        object obj = Registry.CurrentUser.OpenSubKey("SOFTWARE\\Valve\\Steam\\").GetValue("SteamPath");
        if (obj != null)
        {
          string str1 = obj.ToString().Replace("/", "\\").Replace("/", "\\");
          if (Directory.Exists(str1 + "\\steamapps\\common\\Counter-Strike Global Offensive\\csgo"))
          {
            Helper.PathToCSGO = str1 + "\\steamapps\\common\\Counter-Strike Global Offensive\\csgo";
            return Helper.PathToCSGO;
          }
          if (File.Exists(str1 + "\\steamapps\\libraryfolders.vdf"))
          {
            StreamReader streamReader = new StreamReader(str1 + "\\steamapps\\libraryfolders.vdf");
            string input;
            while ((input = streamReader.ReadLine()) != null)
            {
              MatchCollection matchCollection = Regex.Matches(input, "([\"'])(?:(?=(\\\\?))\\2.)*?\\1");
              int result;
              if (matchCollection.Count >= 2 && int.TryParse(matchCollection[0].ToString().Replace("\"", ""), out result) && matchCollection[1].Length > 0)
              {
                string str2 = matchCollection[1].ToString().Replace("\"", "");
                if (Directory.Exists(str2 + "\\steamapps\\common\\Counter-Strike Global Offensive\\csgo"))
                {
                  Helper.PathToCSGO = str2 + "\\steamapps\\common\\Counter-Strike Global Offensive\\csgo";
                  Console.WriteLine(Helper.PathToCSGO);
                  return Helper.PathToCSGO;
                }
              }
            }
          }
        }
      }
      catch (Exception ex)
      {
      }
      string path1 = Environment.GetEnvironmentVariable("ProgramFiles(x86)") + "\\Steam\\steamapps\\common\\Counter-Strike Global Offensive\\csgo";
      string path2 = Environment.GetEnvironmentVariable("ProgramFiles") + "\\Steam\\steamapps\\common\\Counter-Strike Global Offensive\\csgo";
      if (Directory.Exists(path2))
      {
        Helper.PathToCSGO = path2;
        return Helper.PathToCSGO;
      }
      if (Directory.Exists(path1))
      {
        Helper.PathToCSGO = path1;
        return Helper.PathToCSGO;
      }
      Analytics.TrackEvent("Errors", "CouldNotFindCSGOPath", "", 0);
      return "";
    }

    public static string getPathToSteam()
    {
      if (Helper.PathToSteam != null)
        return Helper.PathToSteam;
      object obj = Registry.CurrentUser.OpenSubKey("SOFTWARE\\Valve\\Steam\\").GetValue("SteamPath");
      if (obj != null)
      {
        string path = obj.ToString().Replace("/", "\\").Replace("/", "\\");
        if (Directory.Exists(path))
        {
          Helper.PathToSteam = path;
          return Helper.PathToSteam;
        }
      }
      string path1 = Environment.GetEnvironmentVariable("ProgramFiles(x86)") + "\\Steam";
      string path2 = Environment.GetEnvironmentVariable("ProgramFiles") + "\\Steam";
      if (Directory.Exists(path2))
      {
        Helper.PathToSteam = path2;
        return Helper.PathToSteam;
      }
      if (Directory.Exists(path1))
      {
        Helper.PathToSteam = path1;
        return Helper.PathToSteam;
      }
      Analytics.TrackEvent("Errors", "CouldNotFindSteamPath", "", 0);
      return "";
    }

    public static string getPlayerNickname()
    {
      if (Helper.PlayerNickname != null)
        return Helper.PlayerNickname;
      string pathToSteam = Helper.getPathToSteam();
      if (pathToSteam != "" && Directory.Exists(pathToSteam + "\\userdata"))
      {
        string path1 = "";
        DateTime dateTime = new DateTime();
        foreach (string directory in Directory.GetDirectories(pathToSteam + "\\userdata"))
        {
          string path2 = directory + "\\730\\local\\cfg\\config.cfg";
          if (File.Exists(path2))
          {
            DateTime lastWriteTime = File.GetLastWriteTime(path2);
            if (dateTime == new DateTime() || dateTime < lastWriteTime)
            {
              dateTime = lastWriteTime;
              path1 = path2;
            }
          }
        }
        if (path1 != "")
        {
          try
          {
            Match match = Regex.Match(File.ReadAllText(path1), "(?<=^name\\s\").*(?=\")", RegexOptions.Multiline);
            if (!match.Success || match.Value.Length < 4)
              return "Sheeter";
            Helper.PlayerNickname = match.Value.Substring(0, match.Value.Length - 3) + "---";
            return Helper.PlayerNickname;
          }
          catch (IOException ex)
          {
          }
        }
      }
      return "Sheeter";
    }

    public static int IntersectsHitBox(Line3D aimRayWorld, Entity entity, float offset = 1f)
    {
      for (int index = 0; index < entity.StudioHitBoxSet.numhitboxes; ++index)
      {
        mstudiobbox_t studioHitBox = entity.StudioHitBoxes[index];
        int bone = studioHitBox.bone;
        if (bone >= 0 && bone <= Helper.MaxStudioBones && (double) studioHitBox.radius > 0.0)
        {
          Matrix matrix = entity.BonesMatrices[bone];
          Line3D other = new Line3D(matrix.Transform(studioHitBox.bbmin), matrix.Transform(studioHitBox.bbmax));
          ValueTuple<Vector3, Vector3> valueTuple = aimRayWorld.ClosestPointsBetween(other, true);
          Vector3 vector3 = Vector3.op_Subtraction(valueTuple.Item2, valueTuple.Item1);
          if ((double) ((Vector3) ref vector3).Length() < (double) studioHitBox.radius * (double) offset)
            return index;
        }
      }
      return -1;
    }

    public static void TripWireMaker(object sender, KeyPressEventArgs e)
    {
      if (e.KeyChar == 'c')
      {
        Helper.pointers = "";
        Helper.pointerI = 0;
      }
      if (e.KeyChar != 'p')
        return;
      IntPtr num = Program.GameProcess.ModuleClient.Read<IntPtr>(Offsets.dwLocalPlayer);
      if (!(num != IntPtr.Zero))
        return;
      Vector3 vector3 = Vector3.op_Addition(Program.GameProcess.Process.Read<Vector3>(num + Offsets.m_vecOrigin), Program.GameProcess.Process.Read<Vector3>(num + Offsets.m_vecViewOffset));
      ++Helper.pointerI;
      Helper.pointers = Helper.pointers + "x" + Helper.pointerI.ToString() + " = " + ((int) vector3.X).ToString() + ", y" + Helper.pointerI.ToString() + " = " + ((int) vector3.Y).ToString() + ",";
      if (Helper.pointerI == 4)
      {
        Console.WriteLine("######################################");
        Console.WriteLine("TRIPWIRE LOCATION:");
        Console.WriteLine(Helper.pointers);
        Console.WriteLine("z = " + ((int) vector3.Z).ToString());
        Console.WriteLine("######################################");
        Helper.pointers = "";
        Helper.pointerI = 0;
      }
      else
        Helper.pointers += "\r";
    }
  }
}
