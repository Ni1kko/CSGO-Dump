using System.Windows.Media;
using SharpDX.Mathematics.Interop;
using SolidColorBrush = SharpDX.Direct2D1.SolidColorBrush;

namespace DDX.Extensions
{
    public static class ColorExtension
    {
        public static SolidColorBrush ToBrush(this Color clr)
        {
            return new SolidColorBrush(G.O.D, 
                new RawColor4(
                    clr.R / 255f, 
                    clr.G / 255f, 
                    clr.B / 255f, 
                    clr.A / 255f));
        }
    }
}
