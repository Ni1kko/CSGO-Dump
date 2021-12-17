// Decompiled with JetBrains decompiler
// Type: ScriptKidAntiCheat.Utils.Analytics
// Assembly: CSGO Reaper, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b9d60819726d5649
// MVID: DCF5523C-1146-4D31-BCED-BF678B6B6909
// Assembly location: C:\Users\Ni1kk\Desktop\New folder\CSGO Reaper.exe

using System;
using System.Collections.Generic;
using System.Net.Http;
using System.Security.Cryptography;
using System.Text;

namespace ScriptKidAntiCheat.Utils
{
  internal static class Analytics
  {
    private static readonly HttpClient client = new HttpClient();
    private static string UAID = "UA-148967474-3";
    private static string cid = BitConverter.ToString(((HashAlgorithm) CryptoConfig.CreateFromName("MD5")).ComputeHash(new UTF8Encoding().GetBytes(Environment.UserName))).Replace("-", string.Empty).ToLower();

    public static async void TrackEvent(string Category, string Action, string label = "", int value = 0)
    {
      Dictionary<string, string> dictionary = new Dictionary<string, string>()
      {
        {
          "v",
          "1"
        },
        {
          "tid",
          Analytics.UAID
        },
        {
          "cid",
          Analytics.cid.ToString()
        },
        {
          "t",
          "event"
        },
        {
          "ec",
          Category
        },
        {
          "ea",
          Action
        }
      };
      if (label != "" && value != 0)
      {
        dictionary.Add("el", label);
        dictionary.Add("ev", value.ToString());
      }
      FormUrlEncodedContent urlEncodedContent = new FormUrlEncodedContent((IEnumerable<KeyValuePair<string, string>>) dictionary);
      string str = await (await Analytics.client.PostAsync("http://www.google-analytics.com/collect", (HttpContent) urlEncodedContent)).Content.ReadAsStringAsync();
    }
  }
}
