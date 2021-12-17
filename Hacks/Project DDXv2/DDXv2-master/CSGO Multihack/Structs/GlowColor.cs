using System.Windows.Media;

namespace DDX.Structs
{
    public struct GlowColor
    {
        // 0.0 - 1.0
        public float R;
        public float G;
        public float B;
        public float A;

        public GlowColor(float r, float g, float b, float a)
        {
            R = r;
            G = g;
            B = b;
            A = a;
        }

        public GlowColor(Color clr)
        {
            R = clr.R / 255f;
            G = clr.G / 255f;
            B = clr.B / 255f;
            A = clr.A / 255f;
        }
    }
}
