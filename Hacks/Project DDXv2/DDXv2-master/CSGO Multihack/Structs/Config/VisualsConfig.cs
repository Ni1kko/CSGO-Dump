namespace DDX.Structs.Config
{
    public struct ActiveWeaponProperties
    {
        public bool Ammo;
    }

    public struct ChamsProperties
    {
        public bool Health;
    }

    public struct GlowProperties
    {
        public bool Fill;
        public bool FillTransparent;
        public bool Health;
        public bool Inner;
    }

    public struct EntitiesConfig
    {
        public ChamsProperties ChamsProp;
        public GlowProperties GlowProp;

        public bool AdditionalInfo;
        public bool Chams;
        public bool Esp2D;
        public bool Esp3D;
        public bool Glow;
        public bool ImportantOnly;
        public bool LocalChams;
        public bool Name;
        public bool Snaplines;
    }

    public struct SkeletonProperties
    {
        public bool Fill;
    }

    public struct RankProperties
    {
        public bool Simple;
    }

    public struct PlayersConfig
    {
        public ActiveWeaponProperties ActiveWeaponProp;
        public ChamsProperties ChamsProp;
        public GlowProperties GlowProp;
        public SkeletonProperties SkeletonProp;
        public RankProperties RankProp;

        public bool Chams;
        public bool Esp2D;
        public bool Esp3D;
        public bool Floor;
        public bool Glow;
        public bool Snaplines;
        public bool ActiveWeapon;
        public bool AdditionalInfo;
        public bool Armor;
        public bool Health;
        public bool Name;
        public bool Rank;
        public bool RotationArrow;
        public bool Skeleton;
        public bool VisibilityCheck;
    }

    public struct VisualsConfig
    {
        public EntitiesConfig Entities;
        public PlayersConfig Players;

        public bool AimFov;
        public bool AimPoint;
        public bool HideAllies;
        public bool HideChickens;
        public bool HideEnemies;
        public bool RecoilCrosshair;
        public bool SilentAimFov;
    }
}
