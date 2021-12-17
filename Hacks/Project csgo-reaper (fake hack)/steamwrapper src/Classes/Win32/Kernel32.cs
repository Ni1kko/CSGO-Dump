using System;
using System.Runtime.InteropServices;

/*
 * Credit: https://github.com/rciworks/RCi.Tutorials.Csgo.Cheat.External
 * */
namespace ScriptKidAntiCheat.Win32
{
    public static class Kernel32
    {
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern bool ReadProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, [Out, MarshalAs(UnmanagedType.AsAny)] object lpBuffer, int dwSize, out int lpNumberOfBytesRead);
    }
}