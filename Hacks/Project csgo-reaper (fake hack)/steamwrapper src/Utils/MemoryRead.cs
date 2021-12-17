// Decompiled with JetBrains decompiler
// Type: ScriptKidAntiCheat.Utils.MemoryRead
// Assembly: SteamWrapper, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b9d60819726d5649
// MVID: 52813C95-239A-43DA-B46E-DCB9B069035E
// Assembly location: C:\Users\Ni1kk\Desktop\New folder\bin\SteamWrapper.exe

using ScriptKidAntiCheat.Win32;
using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;

namespace ScriptKidAntiCheat.Utils
{
  internal static class MemoryRead
  {
    public static T Read<T>(this Process process, IntPtr lpBaseAddress) where T : unmanaged
    {
      return MemoryRead.Read<T>(process.Handle, lpBaseAddress);
    }

    public static T Read<T>(this Module module, int offset) where T : unmanaged
    {
      if (module != null)
        return MemoryRead.Read<T>(module.Process.Handle, module.ProcessModule.BaseAddress + offset);
      return default (T);
    }

    public static string ReadString(
      this Module module,
      IntPtr lpBaseAddress,
      int offset,
      int bytesToRead)
    {
      return MemoryRead.ReadString(module.Process.Handle, lpBaseAddress + offset, bytesToRead);
    }

    public static T Read<T>(IntPtr hProcess, IntPtr lpBaseAddress) where T : unmanaged
    {
      int dwSize = Marshal.SizeOf<T>();
      object lpBuffer = (object) default (T);
      if (!Program.GameProcess.IsValid)
        return default (T);
      int lpNumberOfBytesRead;
      Kernel32.ReadProcessMemory(hProcess, lpBaseAddress, lpBuffer, dwSize, out lpNumberOfBytesRead);
      if (lpNumberOfBytesRead != dwSize)
        return default (T);
      return (T) lpBuffer;
    }

    public static string ReadString(IntPtr hProcess, IntPtr BaseAddress, int bytesToRead)
    {
      int dwSize = bytesToRead;
      byte[] bytes = new byte[bytesToRead];
      int lpNumberOfBytesRead;
      Kernel32.ReadProcessMemory(hProcess, BaseAddress, (object) bytes, dwSize, out lpNumberOfBytesRead);
      if (lpNumberOfBytesRead == dwSize)
        return Encoding.ASCII.GetString(bytes);
      return (string) null;
    }
  }
}
