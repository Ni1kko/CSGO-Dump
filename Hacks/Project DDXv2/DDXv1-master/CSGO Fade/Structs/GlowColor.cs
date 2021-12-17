namespace FadeAPI.Structs
{
    public struct GlowColor
    {
        public float R;
        public float G;
        public float B;
        public float A;

        public GlowColor(float r, float g, float b, float a)
        {
            //[swap]
            R = r;
            A = a;
            G = g;
            B = b;
            //[/swap]
        }
    }
}