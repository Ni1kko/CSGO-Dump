using System;
using System.Runtime.InteropServices;

namespace DDX_Multihack
{
    internal static class WinAPI
    {
        //[swap]
        //[block]
        [DllImport("kernel32")]
        public static extern bool IsDebuggerPresent();
        //[/block]
        //[block]
        [DllImport("kernel32", SetLastError = true, ExactSpelling = true)]
        public static extern bool CheckRemoteDebuggerPresent(IntPtr hProcess, ref bool isDebuggerPresent);
        //[/block]
        //[block]
        [DllImport("kernel32", SetLastError = true)]
        public static extern bool VirtualProtect(IntPtr address, uint size, uint newProtect, out uint oldProtect);
        //[/block]
        //[block]
        [DllImport("gdi32", EntryPoint = "AddFontResourceW", SetLastError = true)]
        public static extern int AddFontResource([In][MarshalAs(UnmanagedType.LPWStr)] string lpFileName);
        //[/block]
        //[/swap]
    }
}
