using System;
using System.Runtime.InteropServices;

namespace ScriptKidAntiCheat.Win32.Data
{
    /// <summary>
    /// https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-keybdinput
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct KeybdInput
    {
        public KeyCode wVk;
        public ushort wScan;
        public KeyboardEventFlags dwFlags;
        public uint time;
        public IntPtr dwExtraInfo;
    }
}
