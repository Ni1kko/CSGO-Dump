using DDX.Enums;

namespace DDX.Extensions
{
    public static class WeaponResolver
    {
        public static string GetName(this WeaponId id)
        {
            switch (id)
            {
                case WeaponId.AK47:           return "AK-47";
                case WeaponId.AUG:            return "AUG";
                case WeaponId.AWP:            return "AWP";
                case WeaponId.C4:             return "C4";
                case WeaponId.CZ75Auto:       return "CZ75-Auto";
                case WeaponId.Decoy:          return "Decoy";
                case WeaponId.DesertEagle:    return "Desert Eagle";
                case WeaponId.DualBerettas:   return "Dual Berettas";
                case WeaponId.FAMAS:          return "FAMAS";
                case WeaponId.FiveSeveN:      return "Five-SeveN";
                case WeaponId.Flashbang:      return "Flashbang";
                case WeaponId.G3SG1:          return "G3SG1";
                case WeaponId.GalilAR:        return "Galil AR";
                case WeaponId.GGKnife:        return "Knife";
                case WeaponId.Glock18:        return "Glock-18";
                case WeaponId.HEGrenade:      return "HE Grenade";
                case WeaponId.Incgrenade:     return "Incgrenade";
                case WeaponId.Knife:          return "Knife";
                case WeaponId.KnifeBayonet:   return "Bayonet";
                case WeaponId.KnifeBowie:     return "Bowie Knife";
                case WeaponId.KnifeButterfly: return "Butterfly Knife";
                case WeaponId.KnifeFalchion:  return "Falchion Knife";
                case WeaponId.KnifeFlip:      return "Flip Knife";
                case WeaponId.KnifeGut:       return "Gut Knife";
                case WeaponId.KnifeHuntsman:  return "Huntsman Knife";
                case WeaponId.KnifeKarambit:  return "Karambit";
                case WeaponId.KnifeM9Bayonet: return "M9 Bayonet";
                case WeaponId.KnifePush:      return "Push Knife";
                case WeaponId.M4A1S:          return "M4A1-S";
                case WeaponId.M4A4:           return "M4A4";
                case WeaponId.M249:           return "M249";
                case WeaponId.MAC10:          return "MAC-10";
                case WeaponId.MAG7:           return "MAG-7";
                case WeaponId.Molotov:        return "Molotov";
                case WeaponId.MP7:            return "MP7";
                case WeaponId.MP9:            return "MP9";
                case WeaponId.Negev:          return "Negev";
                case WeaponId.Nova:           return "Nova";
                case WeaponId.P90:            return "P90";
                case WeaponId.P250:           return "P250";
                case WeaponId.P2000:          return "P2000";
                case WeaponId.PPBizon:        return "PP-Bizon";
                case WeaponId.R8Revolver:     return "R8 Revolver";
                case WeaponId.SawedOff:       return "Sawed-Off";
                case WeaponId.SCAR20:         return "SCAR-20";
                case WeaponId.SG553:          return "SG 553";
                case WeaponId.SmokeGrenade:   return "Smoke Grenade";
                case WeaponId.SSG08:          return "SSG 08";
                case WeaponId.Taser:          return "Zeus x27";
                case WeaponId.Tec9:           return "Tec-9";
                case WeaponId.UMP45:          return "UMP-45";
                case WeaponId.USP:            return "USP-S";
                case WeaponId.XM1014:         return "XM1014";
            }

            return id.ToString();
        }

        public static WeaponCategory GetCategory(this WeaponId id)
        {
            switch (id)
            {
                case WeaponId.DesertEagle:
                case WeaponId.DualBerettas:
                case WeaponId.FiveSeveN:
                case WeaponId.Glock18:
                case WeaponId.P250:
                case WeaponId.P2000:
                case WeaponId.R8Revolver:
                case WeaponId.Tec9:
                case WeaponId.USP:
                    return WeaponCategory.Pistol;
                case WeaponId.MAG7:
                case WeaponId.Nova:
                case WeaponId.SawedOff:
                case WeaponId.XM1014:
                    return WeaponCategory.Shotgun;
                case WeaponId.M249:
                case WeaponId.Negev:
                    return WeaponCategory.Heavy;
                case WeaponId.MAC10:
                case WeaponId.MP7:
                case WeaponId.MP9:
                case WeaponId.P90:
                case WeaponId.PPBizon:
                case WeaponId.UMP45:
                    return WeaponCategory.Smg;
                case WeaponId.AK47:
                case WeaponId.AUG:
                case WeaponId.FAMAS:
                case WeaponId.GalilAR:
                case WeaponId.M4A1S:
                case WeaponId.M4A4:
                case WeaponId.SG553:
                    return WeaponCategory.Rifle;
                case WeaponId.AWP:
                case WeaponId.G3SG1:
                case WeaponId.SCAR20:
                case WeaponId.SSG08:
                    return WeaponCategory.Sniper;
                case WeaponId.Taser:
                    return WeaponCategory.Gear;
                case WeaponId.Decoy:
                case WeaponId.Flashbang:
                case WeaponId.HEGrenade:
                case WeaponId.Incgrenade:
                case WeaponId.Molotov:
                case WeaponId.SmokeGrenade:
                    return WeaponCategory.Grenade;
                case WeaponId.GGKnife:
                case WeaponId.Knife:
                case WeaponId.KnifeBayonet:
                case WeaponId.KnifeBowie:
                case WeaponId.KnifeButterfly:
                case WeaponId.KnifeFalchion:
                case WeaponId.KnifeFlip:
                case WeaponId.KnifeGut:
                case WeaponId.KnifeHuntsman:
                case WeaponId.KnifeKarambit:
                case WeaponId.KnifeM9Bayonet:
                case WeaponId.KnifePush:
                    return WeaponCategory.Knife;
                case WeaponId.C4:
                    return WeaponCategory.Bomb;
            }

            return WeaponCategory.Default;
        }

        public static bool GetHasSkin(this WeaponId id)
        {
            return id.GetCanShoot();
        }

        public static bool GetCanShoot(this WeaponId id)
        {
            var cat = id.GetCategory();

            return cat == WeaponCategory.Pistol || 
                cat == WeaponCategory.Shotgun || 
                cat == WeaponCategory.Heavy ||
                cat == WeaponCategory.Smg || 
                cat == WeaponCategory.Rifle ||
                cat == WeaponCategory.Sniper;
        }

        public static bool GetIsRcsRestricted(this WeaponId id)
        {
            var cat = id.GetCategory();

            return !id.GetCanShoot() || 
                cat == WeaponCategory.Shotgun || 
                cat == WeaponCategory.Sniper || 
                id == WeaponId.R8Revolver;
        }
    }
}