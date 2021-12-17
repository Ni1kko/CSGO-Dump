using FadeAPI.Helpers;
using System;
using Encoding = System.Text.Encoding;

namespace FadeAPI.Memory
{
    internal class MemoryCore
    {
        private readonly int _processHandle;

        public MemoryCore(System.Diagnostics.Process process)
        {
            //[swap]
            var processId = process.Id;
            const int access = 0x10 | 0x20 | 0x8;
            //[/swap]

            _processHandle = WinAPI.OpenProcess(access, false, processId);
        }

        ~MemoryCore()
        {
            WinAPI.CloseHandle(_processHandle);
        }

        public byte[] ReadBytes(IntPtr address, int bufferSize)
        {
            var buffer = new byte[bufferSize];
            WinAPI.ReadProcessMemory(_processHandle, address, buffer, buffer.Length, 0);
            return buffer;
        }

        public string ReadString(IntPtr address, int bufferSize)
        {
            var buffer = ReadBytes(address, bufferSize);
            var result = Encoding.Default.GetString(buffer);
            return result.TrimEnd('\0');
        }

        public void WriteBytes(IntPtr address, byte[] buffer)
        {
            WinAPI.WriteProcessMemory(_processHandle, address, buffer, buffer.Length, 0);
        }
    }
}
