using System;
using System.Collections.Generic;
using System.Windows.Forms;
using FadeAPI.Helpers;

namespace FadeAPI.Keyboard
{
    public class KeyboardHook
    {
        public delegate int KeyboardHookProc(int code, int wParam, ref KeyboardHookStruct lParam);

        public struct KeyboardHookStruct
        {
            public int keyCode;
            public int time;
            public int extra;
            public int scanCode;
            public int flags;
        }

        //[swap]
        private const int WmKeyup = 0x101;
        private const int WmKeydown = 0x100;
        private const int WmSyskeydown = 0x104;
        private const int WmSyskeyup = 0x105;
        public event KeyEventHandler KeyDown;
        public readonly List<Keys> HookedKeys = new List<Keys>();
        private IntPtr _hook = IntPtr.Zero;
        private readonly KeyboardHookProc _hookProcDelegate;
        private const int WhKeyboardLl = 0xD;
        public event KeyEventHandler KeyUp;
        //[/swap]

        public KeyboardHook()
        {
            _hookProcDelegate = HookProc;
            Hook();
        }

        ~KeyboardHook()
        {
            Unhook();
        }

        private void Hook()
        {
            var hInstance = WinAPI.LoadLibrary("user32");
            _hook = WinAPI.SetWindowsHookEx(WhKeyboardLl, _hookProcDelegate, hInstance, 0);
        }

        private void Unhook()
        {
            WinAPI.UnhookWindowsHookEx(_hook);
        }

        private int HookProc(int code, int wParam, ref KeyboardHookStruct lParam)
        {
            if (code < 0)
                return WinAPI.CallNextHookEx(_hook, code, wParam, ref lParam);

            Keys key = (Keys)lParam.keyCode;

            if (!HookedKeys.Contains(key))
                return WinAPI.CallNextHookEx(_hook, code, wParam, ref lParam);

            var kea = new KeyEventArgs(key);

            if ((wParam == WmKeydown || wParam == WmSyskeydown) && (KeyDown != null))
                KeyDown(this, kea);
            else if (wParam == WmKeyup || wParam == WmSyskeyup)
                KeyUp?.Invoke(this, kea);

            return kea.Handled ? 1 : WinAPI.CallNextHookEx(_hook, code, wParam, ref lParam);
        }
    }
}
