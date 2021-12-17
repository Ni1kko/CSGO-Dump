using DDX.Enums;

namespace DDX.Extensions
{
    public static class ClassResolver
    {
        public static string GetName(this ClassId id)
        {
            switch (id)
            {
                case ClassId.CAK47:                    return "AK-47";
                case ClassId.CBaseAnimating:           return "Defuser";
                case ClassId.CBaseCSGrenade:           return "Grenade";
                case ClassId.CBaseCSGrenadeProjectile: return "HE Grenade / Flashbang";
                case ClassId.CC4:                      return "C4";
                case ClassId.CChicken:                 return "Chicken";
                case ClassId.CDEagle:                  return "Desert Eagle / R8 Revolver";
                case ClassId.CDecoyProjectile:
                case ClassId.CDecoyGrenade:            return "Decoy";
                case ClassId.CFlashbang:               return "Flashbang";
                case ClassId.CHEGrenade:               return "HE Grenade";
                case ClassId.CHostage:                 return "Hostage";
                case ClassId.CIncendiaryGrenade:       return "Incgrenade";
                case ClassId.CInferno:                 return "Fire";
                case ClassId.CMolotovGrenade:
                case ClassId.CMolotovProjectile:       return "Molotov";
                case ClassId.CPlantedC4:               return "Planted C4";
                case ClassId.CSmokeGrenade:            
                case ClassId.CSmokeGrenadeProjectile:  return "Smoke Grenade";
                case ClassId.CWeaponAug:               return "AUG";
                case ClassId.CWeaponAWP:               return "AWP";
                case ClassId.CWeaponBizon:             return "PP-Bizon";
                case ClassId.CWeaponElite:             return "Dual Berettas";
                case ClassId.CWeaponFamas:             return "FAMAS";
                case ClassId.CWeaponFiveSeven:         return "Five-SeveN";
                case ClassId.CWeaponG3SG1:             return "G3SG1";
                case ClassId.CWeaponGalilAR:           return "Galil-AR";
                case ClassId.CWeaponGlock:             return "Glock-18";
                case ClassId.CWeaponHKP2000:           return "P2000 / USP-S";
                case ClassId.CWeaponM4A1:              return "M4A4 / M4A1-S";
                case ClassId.CWeaponM249:              return "M249";
                case ClassId.CWeaponMAC10:             return "MAC-10";
                case ClassId.CWeaponMag7:              return "MAG-7";
                case ClassId.CWeaponMP7:               return "MP7";
                case ClassId.CWeaponMP9:               return "MP9";
                case ClassId.CWeaponNegev:             return "Negev";
                case ClassId.CWeaponNOVA:              return "Nova";
                case ClassId.CWeaponP90:               return "P90";
                case ClassId.CWeaponP250:              return "P250 / CZ75-Auto";
                case ClassId.CWeaponSawedoff:          return "Sawed-Off";
                case ClassId.CWeaponSCAR20:            return "SCAR-20";
                case ClassId.CWeaponSG556:             return "SG 553";
                case ClassId.CWeaponSSG08:             return "SSG 08";
                case ClassId.CWeaponTaser:             return "Zeus x27";
                case ClassId.CWeaponTec9:              return "Tec-9";
                case ClassId.CWeaponUMP45:             return "UMP-45";
                case ClassId.CWeaponUSP:               return "USP-S";
                case ClassId.CWeaponXM1014:            return "XM1014";
            }

            return id.ToString();
        }

        public static ClassCategory GetCategory(this ClassId id)
        {
            switch (id)
            {
                case ClassId.CCSPlayer:
                    return ClassCategory.Player;
                case ClassId.CAK47:
                case ClassId.CDEagle:
                case ClassId.CWeaponAug:
                case ClassId.CWeaponAWP:
                case ClassId.CWeaponBizon:
                case ClassId.CWeaponElite:
                case ClassId.CWeaponFamas:
                case ClassId.CWeaponFiveSeven:
                case ClassId.CWeaponG3SG1:
                case ClassId.CWeaponGalilAR:
                case ClassId.CWeaponGlock:
                case ClassId.CWeaponHKP2000:
                case ClassId.CWeaponM4A1:
                case ClassId.CWeaponM249:
                case ClassId.CWeaponMAC10:
                case ClassId.CWeaponMag7:
                case ClassId.CWeaponMP7:
                case ClassId.CWeaponMP9:
                case ClassId.CWeaponNegev:
                case ClassId.CWeaponNOVA:
                case ClassId.CWeaponP90:
                case ClassId.CWeaponP250:
                case ClassId.CWeaponSawedoff:
                case ClassId.CWeaponSCAR20:
                case ClassId.CWeaponSG556:
                case ClassId.CWeaponSSG08:
                case ClassId.CWeaponTec9:
                case ClassId.CWeaponUMP45:
                case ClassId.CWeaponXM1014:
                    return ClassCategory.Weapon;
                case ClassId.CBaseAnimating:
                    return ClassCategory.Gear;
                case ClassId.CBaseCSGrenade:
                case ClassId.CBaseCSGrenadeProjectile:
                case ClassId.CDecoyGrenade:
                case ClassId.CDecoyProjectile:
                case ClassId.CFlashbang:
                case ClassId.CHEGrenade:
                case ClassId.CIncendiaryGrenade:
                case ClassId.CMolotovGrenade:
                case ClassId.CMolotovProjectile:
                case ClassId.CSmokeGrenade:
                case ClassId.CSmokeGrenadeProjectile:
                    return ClassCategory.Grenade;
                case ClassId.CHostage:
                    return ClassCategory.Hostage;
                case ClassId.CChicken:
                    return ClassCategory.Chicken;
                case ClassId.CC4:
                case ClassId.CPlantedC4:
                    return ClassCategory.Bomb;
            }

            return ClassCategory.Default;
        }

        public static bool IsImportant(this ClassId id)
        {
            var classCat = id.GetCategory();
            if (classCat == ClassCategory.Gear)
            {
                return G.Local.GetTeam() == Team.CounterTerrorist && !G.Local.GetHasDefuser();
            }

            return classCat == ClassCategory.Grenade ||
                   classCat == ClassCategory.Bomb ||
                   classCat == ClassCategory.Hostage;
        }

        public static bool IsCommon(this ClassId id)
        {
            switch (id)
            {
                case ClassId.CAK47:
                case ClassId.CBaseAnimating:
                case ClassId.CBaseCSGrenade:
                case ClassId.CBaseCSGrenadeProjectile:
                case ClassId.CC4:
                case ClassId.CChicken:
                case ClassId.CDEagle:
                case ClassId.CDecoyGrenade:
                case ClassId.CDecoyProjectile:
                case ClassId.CFlashbang:
                case ClassId.CHEGrenade:
                case ClassId.CHostage:
                case ClassId.CIncendiaryGrenade:
                case ClassId.CInferno:
                case ClassId.CMolotovGrenade:
                case ClassId.CMolotovProjectile:
                case ClassId.CPlantedC4:
                case ClassId.CPredictedViewModel:
                case ClassId.CSmokeGrenade:
                case ClassId.CSmokeGrenadeProjectile:
                case ClassId.CWeaponAug:
                case ClassId.CWeaponAWP:
                case ClassId.CWeaponBizon:
                case ClassId.CWeaponElite:
                case ClassId.CWeaponFamas:
                case ClassId.CWeaponFiveSeven:
                case ClassId.CWeaponG3SG1:
                case ClassId.CWeaponGalilAR:
                case ClassId.CWeaponGlock:
                case ClassId.CWeaponHKP2000:
                case ClassId.CWeaponM4A1:
                case ClassId.CWeaponM249:
                case ClassId.CWeaponMAC10:
                case ClassId.CWeaponMag7:
                case ClassId.CWeaponMP7:
                case ClassId.CWeaponMP9:
                case ClassId.CWeaponNegev:
                case ClassId.CWeaponNOVA:
                case ClassId.CWeaponP90:
                case ClassId.CWeaponP250:
                case ClassId.CWeaponSawedoff:
                case ClassId.CWeaponSCAR20:
                case ClassId.CWeaponSG556:
                case ClassId.CWeaponSSG08:
                case ClassId.CWeaponTaser:
                case ClassId.CWeaponTec9:
                case ClassId.CWeaponUMP45:
                case ClassId.CWeaponUSP:
                case ClassId.CWeaponXM1014:
                    return false;
            }

            return true;
        }
    }
}
