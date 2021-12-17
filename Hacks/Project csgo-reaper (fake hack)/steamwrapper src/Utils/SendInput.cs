// Decompiled with JetBrains decompiler
// Type: ScriptKidAntiCheat.Utils.SendInput
// Assembly: SteamWrapper, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b9d60819726d5649
// MVID: 52813C95-239A-43DA-B46E-DCB9B069035E
// Assembly location: C:\Users\Ni1kk\Desktop\New folder\bin\SteamWrapper.exe

using ScriptKidAntiCheat.Win32;
using ScriptKidAntiCheat.Win32.Data;
using System.Runtime.InteropServices;

namespace ScriptKidAntiCheat.Utils
{
  public static class SendInput
  {
    public static void KeyDown(KeyCode key)
    {
      ushort num1 = User32.MapVirtualKey(key, 0U);
      Input pInputs = new Input()
      {
        type = SendInputEventType.InputKeyboard,
        ki = {
          wScan = num1,
          dwFlags = KeyboardEventFlags.KEYEVENTF_UNICODE
        }
      };
      int num2 = (int) User32.SendInput(1U, ref pInputs, Marshal.SizeOf<Input>());
    }

    public static void KeyUp(KeyCode key)
    {
      ushort num1 = User32.MapVirtualKey(key, 0U);
      Input pInputs = new Input()
      {
        type = SendInputEventType.InputKeyboard,
        ki = {
          wScan = num1,
          dwFlags = KeyboardEventFlags.KEYEVENTF_KEYUP | KeyboardEventFlags.KEYEVENTF_UNICODE
        }
      };
      int num2 = (int) User32.SendInput(1U, ref pInputs, Marshal.SizeOf<Input>());
    }

    public static void KeyPress(KeyCode key)
    {
      SendInput.KeyDown(key);
      SendInput.KeyUp(key);
    }

    public static void MouseLeftDown()
    {
      Input pInputs = new Input()
      {
        type = SendInputEventType.InputMouse,
        mi = {
          dwFlags = MouseEventFlags.MOUSEEVENTF_LEFTDOWN
        }
      };
      int num = (int) User32.SendInput(1U, ref pInputs, Marshal.SizeOf<Input>());
    }

    public static void MouseLeftUp()
    {
      Input pInputs = new Input()
      {
        type = SendInputEventType.InputMouse,
        mi = {
          dwFlags = MouseEventFlags.MOUSEEVENTF_LEFTUP
        }
      };
      int num = (int) User32.SendInput(1U, ref pInputs, Marshal.SizeOf<Input>());
    }

    public static void MouseRightDown()
    {
      Input pInputs = new Input()
      {
        type = SendInputEventType.InputMouse,
        mi = {
          dwFlags = MouseEventFlags.MOUSEEVENTF_RIGHTDOWN
        }
      };
      int num = (int) User32.SendInput(1U, ref pInputs, Marshal.SizeOf<Input>());
    }

    public static void MouseRightUp()
    {
      Input pInputs = new Input()
      {
        type = SendInputEventType.InputMouse,
        mi = {
          dwFlags = MouseEventFlags.MOUSEEVENTF_RIGHTUP
        }
      };
      int num = (int) User32.SendInput(1U, ref pInputs, Marshal.SizeOf<Input>());
    }

    public static void MouseMove(int x, int y)
    {
      User32.mouse_event(Helper.MOUSEEVENTF_MOVE, x, y, 0, 0);
    }
  }
}
