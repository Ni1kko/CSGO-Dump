using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using DDX.Keyboard;
using DDX.Structs;

namespace DDX
{
    public static class WinApi
    {
        [DllImport("user32")]
        public static extern bool SetLayeredWindowAttributes(IntPtr hwnd, uint crKey, byte bAlpha, uint dwFlags);

        [DllImport("kernel32")]
        public static extern bool WriteProcessMemory(int hProcess, IntPtr lpBaseAddress, byte[] buffer, uint size, out int lpNumberOfBytesWritten);

        [DllImport("kernel32")]
        public static extern bool ReadProcessMemory(int hProcess, IntPtr lpBaseAddress, byte[] buffer, uint size, out int lpNumberOfBytesRead);

        [DllImport("kernel32")]
        public static extern int CreateRemoteThread(int hProcess, IntPtr lpThreadAttributes, uint dwStackSize, IntPtr lpStartAddress, IntPtr lpParameter, uint dwCreationFlags, IntPtr lpThreadId);

        [DllImport("kernel32")]
        public static extern int OpenProcess(uint dwDesiredAccess, bool bInheritHandle, int dwProcessId);

        [DllImport("user32")]
        public static extern IntPtr GetForegroundWindow();

        [DllImport("kernel32")]
        public static extern bool CloseHandle(int hObject);

        [DllImport("user32")]
        public static extern int GetWindowLong(IntPtr hWnd, int nIndex);

        [DllImport("user32")]
        public static extern int GetWindowThreadProcessId(IntPtr handle, out int processId);

        [DllImport("kernel32")]
        public static extern bool VirtualProtect(IntPtr lpAddress, uint dwSize, uint flNewProtect, ref uint lpflOldProtect);

        [DllImport("dwmapi")]
        public static extern void DwmExtendFrameIntoClientArea(IntPtr hWnd, ref Margins pMargins);

        [DllImport("kernel32")]
        public static extern bool VirtualFreeEx(int hProcess, IntPtr lpAddress, uint dwSize, uint dwFreeType);

        [DllImport("kernel32")]
        public static extern int CreateRemoteThread(int hProcess, uint lpThreadAttributes, uint dwStackSize, IntPtr lpStartAddress, IntPtr lpParameter, uint dwCreationFlags, uint lpThreadId);

        [DllImport("user32")]
        public static extern bool SetWindowPos(IntPtr hWnd, IntPtr hWndInsertAfter, int x, int y, int cx, int cy, uint uFlags);

        [DllImport("user32")]
        public static extern short GetAsyncKeyState(Keys vKey);

        [DllImport("kernel32")]
        public static extern IntPtr WaitForSingleObject(int hProcess, uint dwMilliseconds);

        [DllImport("kernel32")]
        public static extern IntPtr VirtualAllocEx(int hProcess, IntPtr lpAddress, uint dwSize, uint flAllocationType, uint flProtect);

        [DllImport("user32")]
        public static extern int SetWindowLong(IntPtr hWnd, int nIndex, int dwNewLong);

        [DllImport("gdi32")]
        public static extern int AddFontResource(string lpszFilename);

        [DllImport("user32")]
        public static extern IntPtr SetWindowsHookEx(int idHook, KeyboardHook.KeyboardHookProc callback, IntPtr hInstance, uint threadId);

        [DllImport("user32")]
        public static extern bool UnhookWindowsHookEx(IntPtr hInstance);

        [DllImport("user32")]
        public static extern int CallNextHookEx(IntPtr idHook, int nCode, int wParam, ref KeyboardHookStruct lParam);

        [DllImport("kernel32")]
        public static extern IntPtr LoadLibrary(string lpFileName);
    }
}