using System.Windows.Forms;
using DDX.Enums;

namespace DDX.Structs.Config
{
    public struct LockProperties
    {
        public int Length;
    }

    public struct VisibilityCheckProperties
    {
        public bool BspParsing;
    }

    public struct AimbotProperties
    {
        public LockProperties LockProp;
        public VisibilityCheckProperties VisibilityCheckProp;

        public Bone Bone;
        public bool Lock;
        public bool OnGroundOnly;
        public bool VisibilityCheck;
        public float Fov;
        public int Smooth;
        public Keys Key;
    }

    public struct RcsProperties
    {
        public bool AimbotOnly;
        public int PitchStrength;
        public int Smooth;
        public int YawStrength;
    }

    public struct SilentAimProperties
    {
        public VisibilityCheckProperties VisibilityCheckProp;

        public Bone Bone;
        public bool OnGroundOnly;
        public bool VisibilityCheck;
        public float Fov;
        public Keys Key;
    }

    public struct TriggerbotProperties
    {
        public int AfterDelay;
        public int BeforeDelay;
        public int ShootDelay;
        public Keys Key;
    }

    public struct WeaponProperties
    {
        public AimbotProperties AimbotProp;
        public RcsProperties RcsProp;
        public SilentAimProperties SilentAimProp;
        public TriggerbotProperties TriggerbotProp;

        public bool Aimbot;
        public bool Rcs;
        public bool SilentAim;
        public bool Triggerbot;
        public Keys TriggerbotWithAimbotKey;
    }

    public struct AimConfig
    {
        public WeaponProperties Default;
        public WeaponProperties Pistols;
        public WeaponProperties Snipers;
    }
}
