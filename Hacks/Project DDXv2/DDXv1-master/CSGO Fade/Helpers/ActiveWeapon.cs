namespace FadeAPI.Helpers
{
    public static class ActiveWeapon
    {
        public static string GetWeaponName(this int id)
        {
            switch (id)
            {
                //[swap]
                //[block]
                case 61:
                    return "USP-S";
                //[/block]
                //[block]
                case 48:
                    return "Incgrenade";
                //[/block]
                //[block]
                case 1:
                    return "Desert Eagle";
                //[/block]
                //[block]
                case 17:
                    return "MAC-10";
                //[/block]
                //[block]
                case 64:
                    return "R8 Revolver";
                //[/block]
                //[block]
                case 43:
                    return "Flashbang";
                //[/block]
                //[block]
                case 516:
                    return "Push Knife";
                //[/block]
                //[block]
                case 14:
                    return "M249";
                //[/block]
                //[block]
                case 34:
                    return "MP9";
                //[/block]
                //[block]
                case 40:
                    return "SSG 08";
                //[/block]
                //[block]
                case 30:
                    return "Tec-9";
                //[/block]
                //[block]
                case 8:
                    return "AUG";
                //[/block]
                //[block]
                case 3:
                    return "Five-SeveN";
                //[/block]
                //[block]
                case 24:
                    return "UMP-45";
                //[/block]
                //[block]
                case 507:
                    return "Karambit";
                //[/block]
                //[block]
                case 512:
                    return "Falchion Knife";
                //[/block]
                //[block]
                case 32:
                    return "P2000";
                //[/block]
                //[block]
                case 42:
                    return "Knife";
                //[/block]
                //[block]
                case 514:
                    return "Bowie Knife";
                //[/block]
                //[block]
                case 60:
                    return "M4A1-S";
                //[/block]
                //[block]
                case 19:
                    return "P90";
                //[/block]
                //[block]
                case 505:
                    return "Flip Knife";
                //[/block]
                //[block]
                case 38:
                    return "SCAR-20";
                //[/block]
                //[block]
                case 49:
                    return "C4";
                //[/block]
                //[block]
                case 47:
                    return "Decoy";
                //[/block]
                //[block]
                case 36:
                    return "P250";
                //[/block]
                //[block]
                case 515:
                    return "Butterfly Knife";
                //[/block]
                //[block]
                case 2:
                    return "Dual Berettas";
                //[/block]
                //[block]
                case 4:
                    return "Glock-18";
                //[/block]
                //[block]
                case 31:
                    return "Zeus x27";
                //[/block]
                //[block]
                case 13:
                    return "Galil AR";
                //[/block]
                //[block]
                case 509:
                    return "Huntsman Knife";
                //[/block]
                //[block]
                case 506:
                    return "Gut Knife";
                //[/block]
                //[block]
                case 9:
                    return "AWP";
                //[/block]
                //[block]
                case 39:
                    return "SG 553";
                //[/block]
                //[block]
                case 45:
                    return "Smoke Grenade";
                //[/block]
                //[block]
                case 508:
                    return "M9 Bayonet";
                //[/block]
                //[block]
                case 10:
                    return "FAMAS";
                //[/block]
                //[block]
                case 27:
                    return "MAG-7";
                //[/block]
                //[block]
                case 11:
                    return "G3SG1";
                //[/block]
                //[block]
                case 33:
                    return "MP7";
                //[/block]
                //[block]
                case 7:
                    return "AK-47";
                //[/block]
                //[block]
                case 26:
                    return "PP-Bizon";
                //[/block]
                //[block]
                case 63:
                    return "CZ75-Auto";
                //[/block]
                //[block]
                case 29:
                    return "Sawed-Off";
                //[/block]
                //[block]
                case 46:
                    return "Molotov";
                //[/block]
                //[block]
                case 44:
                    return "HE Grenade";
                //[/block]
                //[block]
                case 25:
                    return "XM1014";
                //[/block]
                //[block]
                case 500:
                    return "Bayonet";
                //[/block]
                //[block]
                case 16:
                    return "M4A4";
                //[/block]
                //[block]
                case 35:
                    return "Nova";
                //[/block]
                //[block]
                case 28:
                    return "Negev";
                //[/block]
                //[block]
                case 59:
                    return "Knife";
                //[/block]
                //[/swap]
            }

            return string.Empty;
        }

        public static bool IsKnife(this int id)
        {
            switch (id)
            {
                //[swap]
                case 42:
                case 500:
                case 514:
                case 509:
                case 59:
                case 506:
                case 505:
                case 515:
                case 508:
                case 516:
                case 512:
                case 507:
                //[/swap]
                    return true;
            }

            return false;
        }

        public static bool IsSniper(this int id)
        {
            switch (id)
            {
                //[swap]
                case 9:
                case 38:
                case 40:
                case 11:
                //[/swap]
                    return true;
            }

            return false;
        }

        public static bool IsPistol(this int id)
        {
            switch (id)
            {
                //[swap]
                case 1:
                case 61:
                case 30:
                case 3:
                case 2:
                case 36:
                case 4:
                case 32:
                //[/swap]
                    return true;
            }

            return false;
        }

        public static bool HasSkin(this int id)
        {
            switch (id)
            {
                //[swap]
                case 500:
                case 3:
                case 17:
                case 63:
                case 10:
                case 1:
                case 13:
                case 2:
                case 29:
                case 505:
                case 9:
                case 26:
                case 36:
                case 64:
                case 4:
                case 60:
                case 25:
                case 19:
                case 508:
                case 32:
                case 512:
                case 24:
                case 514:
                case 7:
                case 506:
                case 28:
                case 30:
                case 34:
                case 8:
                case 11:
                case 14:
                case 61:
                case 40:
                case 38:
                case 27:
                case 515:
                case 33:
                case 509:
                case 507:
                case 16:
                case 35:
                case 39:
                //[/swap]
                    return true;
            }

            return false;
        }

        public static bool IsRestrictedRcs(this int id)
        {
            switch (id)
            {
                //[swap]
                case 9:
                case 11:
                case 40:
                case 25:
                case 27:
                case 35:
                case 1:
                case 38:
                case 29:
                //[/swap]
                    return true;
            }

            return false;
        }
    }
}
