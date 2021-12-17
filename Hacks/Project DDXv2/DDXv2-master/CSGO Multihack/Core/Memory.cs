using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;
using DDX.Enums;

namespace DDX.Core
{
    public class Memory : IDisposable
    {
        public readonly Process MyProcess = Process.GetCurrentProcess();
        public readonly Process TargetProcess;
        public readonly int Handle;

        public Memory(Process targetProcess)
        {
            TargetProcess = targetProcess;
            Handle = WinApi.OpenProcess((uint)AccessRights.AllAccess, false, targetProcess.Id);
        }

        public void Dispose()
        {
            WinApi.CloseHandle(Handle);
        }

        public bool IsProcessFocused()
        {
            var activatedHandle = WinApi.GetForegroundWindow();
            if (activatedHandle == IntPtr.Zero)
            {
                return false;
            }

            WinApi.GetWindowThreadProcessId(activatedHandle, out int activeProcId);

            return activeProcId == TargetProcess.Id || activeProcId == MyProcess.Id;
        }

        public T Read<T>(IntPtr address)
        {
            var size = Marshal.SizeOf<T>();

            var buffer = Read(address, size);

            var ptr = Marshal.AllocHGlobal(size);
            Marshal.Copy(buffer, 0, ptr, size);
            var str = Marshal.PtrToStructure<T>(ptr);
            Marshal.FreeHGlobal(ptr);

            return str;
        }

        public string ReadString(IntPtr address, Encoding encoding, int bufferSize = 512)
        {
            var buffer = Read(address, bufferSize);
            return encoding.GetString(buffer).TrimEnd('\0');
        }

        public byte[] Read(IntPtr address, int bufferSize)
        {
            var buffer = new byte[bufferSize];
            WinApi.ReadProcessMemory(Handle, address, buffer, (uint)buffer.Length, out int processed);

            if (processed != buffer.Length)
            {
                Debug.WriteLine($"ReadProcessMemory fail at 0x{address.ToString("X")}");
            }

            return buffer;
        }

        public void Write<T>(IntPtr address, T str)
        {
            var size = Marshal.SizeOf<T>();

            var buffer = new byte[size];

            var ptr = Marshal.AllocHGlobal(size);
            Marshal.StructureToPtr(str, ptr, true);
            Marshal.Copy(ptr, buffer, 0, size);
            Marshal.FreeHGlobal(ptr);

            Write(address, buffer);
        }

        public void WriteString(IntPtr address, string str, Encoding encoding)
        {
            Write(address, encoding.GetBytes(str));
        }

        public void Write(IntPtr address, byte[] buffer)
        {
            // 0xFFFFF
            if (address.ToInt32() < 0xFFFFF)
            {
                Debug.WriteLine($"WriteProcessMemory fail at 0x{address.ToString("X")}");
                return;
            }

            WinApi.WriteProcessMemory(Handle, address, buffer, (uint)buffer.Length, out int processed);

            if (processed != buffer.Length)
            {
                if (processed != 0)
                {
                    TargetProcess.Kill();
                }

                Debug.Fail($"WriteProcessMemory fail at 0x{address.ToString("X")}");
                return;
            }
        }
    }
}
