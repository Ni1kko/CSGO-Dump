// Decompiled with JetBrains decompiler
// Type: ScriptKidAntiCheat.Utils.Offsets
// Assembly: SteamWrapper, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b9d60819726d5649
// MVID: 52813C95-239A-43DA-B46E-DCB9B069035E
// Assembly location: C:\Users\Ni1kk\Desktop\New folder\bin\SteamWrapper.exe

using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Net;
using System.Reflection;
using System.Text.RegularExpressions;
using System.Windows.Forms;

namespace ScriptKidAntiCheat.Utils
{
  public static class Offsets
  {
    public const string OffsetsURL = "https://gitlab.com/sheetergang/csgo-memory-offsets/-/raw/master/offsets.txt";
    public const int MAXSTUDIOBONES = 128;
    public const float weapon_recoil_scale = 2f;
    public static int dwClientState;
    public static int dwClientState_Map;
    public static int dwClientState_ViewAngles;
    public static int dwClientState_State;
    public static int dwLocalPlayer;
    public static int dwEntityList;
    public static int m_bSpotted;
    public static int m_bDormant;
    public static int m_pStudioHdr;
    public static int dwGameRulesProxy;
    public static int dwGlobalVars;
    public static int m_vecOrigin;
    public static int m_vecViewOffset;
    public static int m_vecVelocity;
    public static int m_dwBoneMatrix;
    public static int m_lifeState;
    public static int m_iHealth;
    public static int m_iTeamNum;
    public static int m_bIsQueuedMatchmaking;
    public static int m_flNextAttack;
    public static int m_nTickBase;
    public static int m_bBombPlanted;
    public static int m_hActiveWeapon;
    public static int m_iItemDefinitionIndex;
    public static int m_bIsDefusing;
    public static int m_bHasDefuser;
    public static int m_bIsScoped;
    public static int m_iShotsFired;
    public static int m_aimPunchAngle;
    public static int m_bStartedArming;
    public static int m_bFreezePeriod;
    public static int m_bWarmupPeriod;
    public static int m_totalRoundsPlayed;
    public static int m_fRoundStartTime;
    public static int m_IRoundTime;
    public static int m_iFOV;

    static Offsets()
    {
      FieldInfo[] fields = typeof (Offsets).GetFields();
      try
      {
        StreamReader streamReader = !System.IO.File.Exists("Offsets\\offsets.txt") ? new StreamReader(new WebClient().OpenRead("https://gitlab.com/sheetergang/csgo-memory-offsets/-/raw/master/offsets.txt")) : new StreamReader("Offsets\\offsets.txt");
        string input;
        while ((input = streamReader.ReadLine()) != null)
        {
          Match match = Regex.Match(input, "\\A(?<name>.+) = (?<value>.+)\\z");
          if (match.Success)
          {
            string s = match.Groups["value"].Value;
            int result;
            if (int.TryParse(s, out result) || int.TryParse(s.Substring(2, s.Length - 2), NumberStyles.HexNumber, (IFormatProvider) null, out result))
            {
              FieldInfo fieldInfo = ((IEnumerable<FieldInfo>) fields).FirstOrDefault<FieldInfo>((Func<FieldInfo, bool>) (fi =>
              {
                if (string.Equals(fi.Name, match.Groups["name"].Value))
                  return fi.FieldType == typeof (int);
                return false;
              }));
              if ((object) fieldInfo != null)
              {
                // ISSUE: explicit non-virtual call
                __nonvirtual (fieldInfo.SetValue((object) null, (object) result));
              }
            }
          }
        }
      }
      catch (Exception ex)
      {
        int num = (int) MessageBox.Show("Could not load memory offsets", "Error", MessageBoxButtons.OK, MessageBoxIcon.Hand);
        Environment.Exit(0);
      }
    }
  }
}
