using System.Runtime.InteropServices;

namespace ScriptKidAntiCheat.Win32.Data
{
    /// <summary>
    /// https://docs.microsoft.com/en-us/windows/win32/api/windef/ns-windef-point
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Point
    {
        public int X, Y;
        internal short x;
    }
}
