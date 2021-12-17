using DDX.Enums;

namespace DDX.Extensions
{
    public static class RankResolver
    {
        public static string GetName(this RankId id)
        {
            if (G.C.Vis.Players.RankProp.Simple)
            {
                return id.GetSimpleName();
            }

            switch (id)
            {
                case RankId.DistinguishedMasterGuardian: return "Distinguished Master Guardian";
                case RankId.GlobalElite:                 return "Global Elite";
                case RankId.GoldNova1:                   return "Gold Nova 1";
                case RankId.GoldNova2:                   return "Gold Nova 2";
                case RankId.GoldNova3:                   return "Gold Nova 3";
                case RankId.GoldNovaMaster:              return "Gold Nova Master";
                case RankId.LegendaryEagle:              return "Legendary Eagle";
                case RankId.LegendaryEagleMaster:        return "Legendary Eagle Master";
                case RankId.MasterGuardian1:             return "Master Guardian 1";
                case RankId.MasterGuardian2:             return "Master Guardian 2";
                case RankId.MasterGuardianElite:         return "Master Guardian Elite";
                case RankId.Silver1:                     return "Silver 1";
                case RankId.Silver2:                     return "Silver 2";
                case RankId.Silver3:                     return "Silver 3";
                case RankId.Silver4:                     return "Silver 4";
                case RankId.SilverElite:                 return "Silver Elite";
                case RankId.SilverEliteMaster:           return "Silver Elite Master";
                case RankId.SupremeMasterFirstClass:     return "Supreme Master First Class";
                case RankId.Unranked:                    return "Unranked";
            }

            return id.ToString();
        }

        private static string GetSimpleName(this RankId id)
        {
            switch (id)
            {
                case RankId.DistinguishedMasterGuardian: return "DMG";
                case RankId.GlobalElite:                 return "GE";
                case RankId.GoldNova1:                   return "G1";
                case RankId.GoldNova2:                   return "G2";
                case RankId.GoldNova3:                   return "G3";
                case RankId.GoldNovaMaster:              return "GNM";
                case RankId.LegendaryEagle:              return "LE";
                case RankId.LegendaryEagleMaster:        return "LEM";
                case RankId.MasterGuardian1:             return "MG1";
                case RankId.MasterGuardian2:             return "MG2";
                case RankId.MasterGuardianElite:         return "MGE";
                case RankId.Silver1:                     return "S1";
                case RankId.Silver2:                     return "S2";
                case RankId.Silver3:                     return "S3";
                case RankId.Silver4:                     return "S4";
                case RankId.SilverElite:                 return "SE";
                case RankId.SilverEliteMaster:           return "SEM";
                case RankId.SupremeMasterFirstClass:     return "SMFC";
                case RankId.Unranked:                    return "UNR";
            }

            return id.ToString();
        }
    }
}