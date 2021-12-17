using System.Runtime.InteropServices;

namespace FadeAPI.Structs
{
    internal static class StructHelper
    {
        public static byte[] ToByteArray<T>(T structure)
        {
            int size = Marshal.SizeOf(structure);

            //[swap]
            var ptr = Marshal.AllocHGlobal(size);
            var arr = new byte[size];
            //[/swap]

            Marshal.StructureToPtr(structure, ptr, true);
            Marshal.Copy(ptr, arr, 0, size);
            Marshal.FreeHGlobal(ptr);

            return arr;
        }

        public static T ToObject<T>(byte[] byteArray)
        {
            var type = typeof(T);

            var size = Marshal.SizeOf(type);
            var ptr = Marshal.AllocHGlobal(size);

            Marshal.Copy(byteArray, 0, ptr, size);
            var obj = (T)Marshal.PtrToStructure(ptr, type);
            Marshal.FreeHGlobal(ptr);

            return obj;
        }
    }
}
