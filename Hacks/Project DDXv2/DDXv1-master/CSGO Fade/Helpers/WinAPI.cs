using System;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;
using FadeAPI.Keyboard;
using FadeAPI.Structs;

namespace FadeAPI.Helpers
{
    internal static class WinAPI
    {
        //[swap]
        //[block]
        [DllImport("kernel32")]
        public static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder retVal, int size, string filePath);
        //[/block]
        //[block]
        [DllImport("kernel32")]
        public static extern bool ReadProcessMemory(int hProcess, IntPtr lpBaseAddress, byte[] buffer, int size, int lpNumberOfBytesRead);
        //[/block]
        //[block]
        [DllImport("kernel32")]
        public static extern long WritePrivateProfileString(string section, string key, string val, string filePath);
        //[/block]
        //[block]
        [DllImport("user32")]
        public static extern short GetAsyncKeyState(Keys arrowKeys);
        //[/block]
        //[block]
        [DllImport("user32")]
        public static extern int SetWindowLong(IntPtr hWnd, int nIndex, int dwNewLong);
        //[/block]
        //[block]
        [DllImport("user32")]
        public static extern bool UnhookWindowsHookEx(IntPtr hInstance);
        //[/block]
        //[block]
        [DllImport("kernel32")]
        public static extern IntPtr LoadLibrary(string lpFileName);
        //[/block]
        //[block]
        [DllImport("user32")]
        public static extern bool GetWindowRect(IntPtr hwnd, ref Margins rectangle);
        //[/block]
        //[block]
        [DllImport("user32", SetLastError = true)]
        public static extern int GetWindowLong(IntPtr hWnd, int nIndex);
        //[/block]
        //[block]
        [DllImport("kernel32")]
        public static extern bool WriteProcessMemory(int hProcess, IntPtr lpBaseAddress, byte[] buffer, int size, int lpNumberOfBytesWritten);
        //[/block]
        //[block]
        [DllImport("user32")]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool SetWindowPos(IntPtr hWnd, IntPtr hWndInsertAfter, int x, int y, int cx, int cy, uint uFlags);
        //[/block]
        //[block]
        [DllImport("kernel32")]
        public static extern bool CloseHandle(int hObject);
        //[/block]
        //[block]
        [DllImport("user32")]
        public static extern int CallNextHookEx(IntPtr idHook, int nCode, int wParam, ref KeyboardHook.KeyboardHookStruct lParam);
        //[/block]
        //[block]
        [DllImport("dwmapi")]
        public static extern void DwmExtendFrameIntoClientArea(IntPtr hWnd, ref Margins pMargins);
        //[/block]
        //[block]
        [DllImport("kernel32")]
        public static extern int OpenProcess(uint dwDesiredAccess, bool bInheritHandle, int dwProcessId);
        //[/block]
        //[block]
        [DllImport("user32")]
        public static extern IntPtr SetWindowsHookEx(int idHook, KeyboardHook.KeyboardHookProc callback, IntPtr hInstance, uint threadId);
        //[/block]
        //[/swap]
    }
}
