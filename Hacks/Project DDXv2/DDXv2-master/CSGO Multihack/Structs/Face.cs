namespace DDX.Structs
{
    public struct Face
    {
        public ushort PlaneNumber;
        public byte Side;
        public byte OnNode;
        public int FirstEdge;
        public short NumEdges;
        public short Texinfo;
        public short Dispinfo;
        public short SurfaceFogVolumeId;
        public byte[] Styles;
        public int LightOffset;
        public float Area;
        public int[] LightmapTextureMinsInLuxels;
        public int[] LightmapTextureSizeInLuxels;
        public int OriginalFace;
        public ushort NumPrims;
        public ushort FirstPrimId;
        public uint SmoothingGroups;
    }
}