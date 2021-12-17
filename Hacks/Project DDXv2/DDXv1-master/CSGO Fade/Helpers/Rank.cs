namespace FadeAPI.Helpers
{
    public static class Rank
    {
        public static string GetRankName(this int id)
        {
            switch (id)
            {
                //[swap]
                //[block]
                case 4:
                    return "Silver 4";
                //[/block]
                //[block]
                case 17:
                    return "Supreme Master First Class";
                //[/block]
                //[block]
                case 10:
                    return "Gold Nova Master";
                //[/block]
                //[block]
                case 9:
                    return "Gold Nova 3";
                //[/block]
                //[block]
                case 13:
                    return "Master Guardian Elite";
                //[/block]
                //[block]
                case 5:
                    return "Silver Elite";
                //[/block]
                //[block]
                case 11:
                    return "Master Guardian 1";
                //[/block]
                //[block]
                case 18:
                    return "Global Elite";
                //[/block]
                //[block]
                case 3:
                    return "Silver 3";
                //[/block]
                //[block]
                case 12:
                    return "Master Guardian 2";
                //[/block]
                //[block]
                case 2:
                    return "Silver 2";
                //[/block]
                //[block]
                case 7:
                    return "Gold Nova 1";
                //[/block]
                //[block]
                case 6:
                    return "Silver Elite Master";
                //[/block]
                //[block]
                case 14:
                    return "Distinguished Master Guardian";
                //[/block]
                //[block]
                case 8:
                    return "Gold Nova 2";
                //[/block]
                //[block]
                case 1:
                    return "Silver 1";
                //[/block]
                //[block]
                case 0:
                    return "Unranked";
                //[/block]
                //[block]
                case 15:
                    return "Legendary Eagle";
                //[/block]
                //[block]
                case 16:
                    return "Legendary Eagle Master";
                //[/block]
                //[/swap]
            }

            return string.Empty;
        }

        public static string GetSimpleRankName(this int id)
        {
            switch (id)
            {
                //[swap]
                //[block]
                case 5:
                    return "SE";
                //[/block]
                //[block]
                case 14:
                    return "DMG";
                //[/block]
                //[block]
                case 11:
                    return "MG1";
                //[/block]
                //[block]
                case 3:
                    return "S3";
                //[/block]
                //[block]
                case 6:
                    return "SEM";
                //[/block]
                //[block]
                case 9:
                    return "G3";
                //[/block]
                //[block]
                case 8:
                    return "G2";
                //[/block]
                //[block]
                case 10:
                    return "GNM";
                //[/block]
                //[block]
                case 18:
                    return "GE";
                //[/block]
                //[block]
                case 12:
                    return "MG2";
                //[/block]
                //[block]
                case 4:
                    return "S4";
                //[/block]
                //[block]
                case 2:
                    return "S2";
                //[/block]
                //[block]
                case 1:
                    return "S1";
                //[/block]
                //[block]
                case 15:
                    return "LE";
                //[/block]
                //[block]
                case 7:
                    return "G1";
                //[/block]
                //[block]
                case 16:
                    return "LEM";
                //[/block]
                //[block]
                case 17:
                    return "SMFC";
                //[/block]
                //[block]
                case 0:
                    return "UNR";
                //[/block]
                //[block]
                case 13:
                    return "MGE";
                //[/block]
                //[/swap]
            }

            return string.Empty;
        }
    }
}
