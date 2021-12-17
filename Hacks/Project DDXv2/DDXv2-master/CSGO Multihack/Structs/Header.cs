namespace DDX.Structs
{
    public struct Header
    {
        public int Ident;
        public int Version;
        public Lump[] Lumps;
        public int MapRevision;
    }
}