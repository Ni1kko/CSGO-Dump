using System;

namespace ScriptKidAntiCheat.Win32.Data
{
    /// <summary>
    /// https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-mouseinput
    /// </summary>
    [Flags]
    public enum KeyboardEventFlags : uint
    {
        KEYEVENTF_EXTENDEDKEY = 0x0001,
        KEYEVENTF_KEYUP = 0x0002,
        KEYEVENTF_SCANCODE = 0x0004,
        KEYEVENTF_UNICODE = 0x0008
    }
}
