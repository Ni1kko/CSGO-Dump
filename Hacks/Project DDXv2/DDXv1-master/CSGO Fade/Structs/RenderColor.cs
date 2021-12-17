namespace FadeAPI.Structs
{
    public struct RenderColor
    {
        public byte R;
        public byte G;
        public byte B;
        public byte A;

        public RenderColor(byte r, byte g, byte b, byte a)
        {
            //[swap]
            B = b;
            R = r;
            A = a;
            G = g;
            //[/swap]
        }
    }
}