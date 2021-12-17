using System;
using System.Collections.Generic;
using System.Windows.Forms;
using DDX.Structs;

namespace DDX.Keyboard
{
    public class KeyboardHook : IDisposable
    {
        private const int WmKeyup = 0x101;
        private const int WmKeydown = 0x100;
        private const int WmSyskeydown = 0x104;
        private const int WmSyskeyup = 0x105;
        private const int WhKeyboardLl = 0xD;

        public readonly List<Keys> HookedKeys;

        private readonly KeyboardHookProc _hookProcDelegate;

        private IntPtr _hook;

        public delegate int KeyboardHookProc(int code, int wParam, ref KeyboardHookStruct lParam);

        public event KeyEventHandler KeyDown;
        public event KeyEventHandler KeyUp;

        public KeyboardHook()
        {
            HookedKeys = new List<Keys>();

            _hookProcDelegate = HookProc;
        }

        public void Dispose()
        {
            Unhook();
        }

        public void Hook()
        {
            var hInstance = WinApi.LoadLibrary("user32");
            _hook = WinApi.SetWindowsHookEx(WhKeyboardLl, _hookProcDelegate, hInstance, 0);
        }

        public void Unhook()
        {
            if(_hook != IntPtr.Zero) WinApi.UnhookWindowsHookEx(_hook);
        }

        private int HookProc(int code, int wParam, ref KeyboardHookStruct lParam)
        {
            if (code < 0)
            {
                return WinApi.CallNextHookEx(_hook, code, wParam, ref lParam);
            }

            var key = (Keys)lParam.keyCode;

            if (!HookedKeys.Contains(key))
            {
                return WinApi.CallNextHookEx(_hook, code, wParam, ref lParam);
            }

            var kea = new KeyEventArgs(key);

            switch (wParam)
            {
                case WmKeydown:
                case WmSyskeydown:
                    KeyDown?.Invoke(this, kea);
                    break;
                case WmKeyup:
                case WmSyskeyup:
                    KeyUp?.Invoke(this, kea);
                    break;
            }

            return kea.Handled ? 1 : WinApi.CallNextHookEx(_hook, code, wParam, ref lParam);
        }
    }
}
