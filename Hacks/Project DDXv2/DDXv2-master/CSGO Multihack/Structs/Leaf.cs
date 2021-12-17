using DDX.Enums;

namespace DDX.Structs
{
    public struct Leaf
    {
        public ContentsFlag Contents;
        public short Cluster;
        public short Area;
        public short Flags;
        public short[] Mins;
        public short[] Maxs;
        public ushort Firstleafface;
        public ushort Numleaffaces;
        public ushort Firstleafbrush;
        public ushort Numleafbrushes;
        public short LeafWaterDataId;
    }
}