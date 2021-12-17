using DDX.Enums;
using DDX.Objects;

namespace DDX.Structs
{
    public struct AimTarget
    {
        public Bone AimBone;
        public bool Selected;
        public float Distance;
        public Player Player;
        public ViewAngles ViewAngles;
    }
}
