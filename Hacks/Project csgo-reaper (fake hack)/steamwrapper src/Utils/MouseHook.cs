// Decompiled with JetBrains decompiler
// Type: ScriptKidAntiCheat.Utils.MouseHook
// Assembly: SteamWrapper, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b9d60819726d5649
// MVID: 52813C95-239A-43DA-B46E-DCB9B069035E
// Assembly location: C:\Users\Ni1kk\Desktop\New folder\bin\SteamWrapper.exe

using ScriptKidAntiCheat.Win32;
using ScriptKidAntiCheat.Win32.Data;
using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace ScriptKidAntiCheat.Utils
{
  public static class MouseHook
  {
    private static User32.LowLevelMouseProc _proc = new User32.LowLevelMouseProc(MouseHook.HookCallback);
    private static IntPtr _hookID = IntPtr.Zero;
    private const int WH_MOUSE_LL = 14;

    public static event EventHandler MouseAction = (_param1, _param2) => {};

    public static void Start()
    {
      MouseHook._hookID = MouseHook.SetHook(MouseHook._proc);
    }

    public static void stop()
    {
      User32.UnhookWindowsHookEx(MouseHook._hookID);
    }

    private static IntPtr SetHook(User32.LowLevelMouseProc proc)
    {
      using (Process currentProcess = Process.GetCurrentProcess())
      {
        using (ProcessModule mainModule = currentProcess.MainModule)
          return User32.SetWindowsHookEx(14, proc, User32.GetModuleHandle(mainModule.ModuleName), 0U);
      }
    }

    private static IntPtr HookCallback(int nCode, IntPtr wParam, IntPtr lParam)
    {
      if (nCode >= 0 && (int) wParam != 512)
      {
        MouseHook.MSLLHOOKSTRUCT structure = (MouseHook.MSLLHOOKSTRUCT) Marshal.PtrToStructure(lParam, typeof (MouseHook.MSLLHOOKSTRUCT));
        MouseHook.MouseAction((object) (MouseHook.MouseEvents) (int) wParam, new EventArgs());
      }
      return User32.CallNextHookEx(MouseHook._hookID, nCode, wParam, lParam);
    }

    public enum MouseEvents
    {
      WM_MOUSEMOVE = 512, // 0x00000200
      WM_LBUTTONDOWN = 513, // 0x00000201
      WM_LBUTTONUP = 514, // 0x00000202
      WM_RBUTTONDOWN = 516, // 0x00000204
      WM_RBUTTONUP = 517, // 0x00000205
      WM_MOUSEWHEEL = 522, // 0x0000020A
    }

    private struct MSLLHOOKSTRUCT
    {
      public Point pt;
      public uint mouseData;
      public uint flags;
      public uint time;
      public IntPtr dwExtraInfo;
    }
  }
}
