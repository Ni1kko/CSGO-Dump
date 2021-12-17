using System;
using System.IO;

namespace DDX.Functions.Bsp
{
    public static class UtilityReader
    {
        public static bool BigEndian;

        public static byte ReadByte(Stream stream)
        {
            var buffer = ReadBytes(stream, 1);
            return buffer[0];
        }

        public static short ReadShort(Stream stream)
        {
            var buffer = ReadBytes(stream, 2);
            if (BigEndian) Array.Reverse(buffer);
            return BitConverter.ToInt16(buffer, 0);
        }

        public static ushort ReadUShort(Stream stream)
        {
            var buffer = ReadBytes(stream, 2);
            if (BigEndian) Array.Reverse(buffer);
            return BitConverter.ToUInt16(buffer, 0);
        }

        public static int ReadInt(Stream stream)
        {
            var buffer = ReadBytes(stream, 4);
            if (BigEndian) Array.Reverse(buffer);
            return BitConverter.ToInt32(buffer, 0);
        }

        public static uint ReadUInt(Stream stream)
        {
            var buffer = ReadBytes(stream, 4);
            if (BigEndian) Array.Reverse(buffer);
            return BitConverter.ToUInt32(buffer, 0);
        }

        public static long ReadLong(Stream stream)
        {
            var buffer = ReadBytes(stream, 8);
            if (BigEndian) Array.Reverse(buffer);
            return BitConverter.ToInt64(buffer, 0);
        }

        public static float ReadFloat(Stream stream)
        {
            var buffer = ReadBytes(stream, 4);
            if (BigEndian) Array.Reverse(buffer);
            return BitConverter.ToSingle(buffer, 0);
        }

        public static byte[] ReadBytes(Stream stream, int count)
        {
            var buffer = new byte[count];
            stream.Read(buffer, 0, count);
            return buffer;
        }
    }
}
