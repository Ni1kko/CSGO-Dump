// Decompiled with JetBrains decompiler
// Type: ScriptKidAntiCheat.Utils.Debug
// Assembly: SteamWrapper, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b9d60819726d5649
// MVID: 52813C95-239A-43DA-B46E-DCB9B069035E
// Assembly location: C:\Users\Ni1kk\Desktop\New folder\bin\SteamWrapper.exe

using System.IO;
using System.Text.RegularExpressions;

namespace ScriptKidAntiCheat.Utils
{
  public class Debug
  {
    public bool AllowLocal { get; set; }

    public bool AllowInWarmup { get; set; }

    public bool IgnoreActivateOnRound { get; set; }

    public int TripWireStage { get; set; }

    public bool DisableRunInBackground { get; set; }

    public bool DisableGoogleDriveUpload { get; set; }

    public bool DisableAcceptConditions { get; set; }

    public bool ShowDebugMessages { get; set; }

    public bool SkipAnalyticsTracking { get; set; }

    public bool SkipInitDelay { get; set; }

    public Debug()
    {
      if (!File.Exists("debug.txt"))
        return;
      typeof (Debug).GetFields();
      StreamReader streamReader = new StreamReader("debug.txt");
      string input;
      while ((input = streamReader.ReadLine()) != null)
      {
        Match match = Regex.Match(input, "\\A(?<name>.+)=(?<value>.+)\\z");
        if (match.Success)
        {
          string name = Regex.Replace(match.Groups["name"].Value, "\\s+", "");
          string s = Regex.Replace(match.Groups["value"].Value, "\\s+", "");
          int result;
          bool flag = int.TryParse(s, out result);
          if (s.ToLower() == "true" || s.ToLower() == "false")
            this.GetType().GetProperty(name).SetValue((object) this, (object) (s == "true"));
          else if (flag)
            this.GetType().GetProperty(name).SetValue((object) this, (object) result);
        }
      }
    }
  }
}
