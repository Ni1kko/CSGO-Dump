using System.Windows.Media;

namespace DDX.Structs
{
    public struct ColorRender
    {
        // 0 - 255
        public byte R;
        public byte G;
        public byte B;
        public byte A;

        public ColorRender(byte r, byte g, byte b, byte a)
        {
            R = r;
            G = g;
            B = b;
            A = a;
        }

        public ColorRender(Color clr)
        {
            R = clr.R;
            G = clr.G;
            B = clr.B;
            A = clr.A;
        }
    }
}
