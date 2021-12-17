using SharpDX.Mathematics.Interop;

namespace DDX.Extensions
{
    public static class RawVector2Extension
    {
        public static bool IsValid(this RawVector2 v)
        {
            return v.X != -1f && v.Y != -1f;
        }
    }
}
