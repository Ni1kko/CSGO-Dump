using FadeAPI.Enums;

namespace FadeAPI.Helpers
{
    public static class ObjectClass
    {
        public static string GetClassName(this int id)
        {
            switch (id)
            {
                //[swap]
                //[block]
                case 86:
                    return "Hostage";
                //[/block]
                //[block]
                case 41:
                    return "Decoy";
                //[/block]
                //[block]
                case 2:
                    return "Defuser";
                //[/block]
                //[block]
                case 202:
                    return "AUG";
                //[/block]
                //[block]
                case 220:
                    return "MAG-10";
                //[/block]
                //[block]
                case 224:
                    return "MP9";
                //[/block]
                //[block]
                case 225:
                    return "Negev";
                //[/block]
                //[block]
                case 39:
                    return "Desert Eagle";
                //[/block]
                //[block]
                case 107:
                    return "Planted C4";
                //[/block]
                //[block]
                case 235:
                    return "SG 556";
                //[/block]
                //[block]
                case 209:
                    return "Dual Berettas";
                //[/block]
                //[block]
                case 215:
                    return "Glock-18";
                //[/block]
                //[block]
                case 217:
                    return "M249";
                //[/block]
                //[block]
                case 31:
                    return "Chicken";
                //[/block]
                //[block]
                case 205:
                    return "PP-Bizon";
                //[/block]
                //[block]
                case 132:
                    return "Smoke Grenade";
                //[/block]
                //[block]
                case 229:
                    return "P90";
                //[/block]
                //[block]
                case 228:
                    return "P250 / CZ75-Auto";
                //[/block]
                //[block]
                case 221:
                    return "MAG-7";
                //[/block]
                //[block]
                case 212:
                    return "G3SG1";
                //[/block]
                //[block]
                case 203:
                    return "AWP";
                //[/block]
                //[block]
                case 223:
                    return "MP7";
                //[/block]
                //[block]
                case 210:
                    return "FAMAS";
                //[/block]
                //[block]
                case 84:
                    return "HE Grenade";
                //[/block]
                //[block]
                case 9:
                    return "HE Grenade / Flashbang";
                //[/block]
                //[block]
                case 1:
                    return "AK-47";
                //[/block]
                //[block]
                case 88:
                    return "Fire";
                //[/block]
                //[block]
                case 238:
                    return "Tec-9";
                //[/block]
                //[block]
                case 241:
                    return "USP-S";
                //[/block]
                //[block]
                case 29:
                    return "C4";
                //[/block]
                //[block]
                case 97:
                    return "Molotov";
                //[/block]
                //[block]
                case 230:
                    return "Sawed-Off";
                //[/block]
                //[block]
                case 231:
                    return "SCAR-20";
                //[/block]
                //[block]
                case 226:
                    return "Nova";
                //[/block]
                //[block]
                case 216:
                    return "P2000 / USP-S";
                //[/block]
                //[block]
                case 242:
                    return "XM1014";
                //[/block]
                //[block]
                case 8:
                    return "Grenade";
                //[/block]
                //[block]
                case 66:
                    return "Flashbang";
                //[/block]
                //[block]
                case 237:
                    return "Zeus x27";
                //[/block]
                //[block]
                case 219:
                    return "M4A1";
                //[/block]
                //[block]
                case 240:
                    return "UMP-45";
                //[/block]
                //[block]
                case 211:
                    return "Five-SeveN";
                //[/block]
                //[block]
                case 236:
                    return "SSG 08";
                //[/block]
                //[block]
                case 214:
                    return "Galil-AR";
                //[/block]
                //[block]
                case 96:
                    return "Molotov";
                //[/block]
                //[block]
                case 87:
                    return "Incgrenade";
                //[/block]
                //[/swap]
            }

            return string.Empty;
        }

        public static bool IsSpecialObject(this int id)
        {
            switch (id)
            {
                //[swap]
                //[block]
                case 2:
                    return Global.LocalPlayer.Team == TeamNum.CounterTerrorist && !Global.LocalPlayer.HasDefuser;
                //[/block]
                //[block]
                case 86:
                    return true;
                //[/block]
                //[block]
                case 41:
                    return true;
                //[/block]
                //[block]
                case 88:
                    return true;
                //[/block]
                //[block]
                case 29:
                    return true;
                //[/block]
                //[block]
                case 132:
                    return true;
                //[/block]
                //[block]
                case 9:
                    return true;
                //[/block]
                //[block]
                case 97:
                    return true;
                //[/block]
                //[block]
                case 107:
                    return true;
                //[/block]
                //[/swap]
            }

            return false;
        }
    }
}
