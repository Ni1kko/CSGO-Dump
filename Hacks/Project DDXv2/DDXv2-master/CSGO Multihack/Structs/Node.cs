namespace DDX.Structs
{
    public struct Node
    {
        public int Planenum;
        public int[] Children;
        public short[] Mins;
        public short[] Maxs;
        public ushort Firstface; 
        public ushort Numfaces;
        public short Area;
        public short Paddding;
    };
}