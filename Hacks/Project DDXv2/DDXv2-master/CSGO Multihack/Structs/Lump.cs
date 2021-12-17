using DDX.Enums;

namespace DDX.Structs
{
    public struct Lump
    {
        public int Offset;
        public int Length;
        public int Version;
        public int FourCc;
        public LumpType Type;
    }
}