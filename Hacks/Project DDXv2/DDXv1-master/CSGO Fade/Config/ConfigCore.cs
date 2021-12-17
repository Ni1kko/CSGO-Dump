using System.Text;
using FadeAPI.Helpers;

namespace FadeAPI.Config
{
    public class ConfigCore
    {
        public readonly string Path;

        public ConfigCore(string path)
        {
            Path = path;
        }

        public string Get(string category, string name)
        {
            StringBuilder temp = new StringBuilder(255);
            WinAPI.GetPrivateProfileString(category, name, string.Empty, temp, 255, Path);
            return temp.ToString();
        }

        public void Set(string category, string name, string value)
        {
            WinAPI.WritePrivateProfileString(category, name, value, Path);
        }
    }
}
