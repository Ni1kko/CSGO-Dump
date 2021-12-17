using System;
using System.Collections.Generic;
using System.Linq;
using FadeAPI.Structs;

namespace FadeAPI.Helpers
{
    internal static class SignatureScanner
    {
        //[swap]
        private static byte[] _dump;
        public static readonly List<Signature> SignatureList = new List<Signature>();
        //[/swap]

        public static void Scan(IntPtr address, int size)
        {
            DumpMemory(address, size);

            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < SignatureList.Count; j++)
                {
                    if (SignatureList[j].Address == IntPtr.Zero && CheckSignature(i, SignatureList[j]))
                    {
                        var offset = SignatureList[j].Offset;
                        SignatureList[j] = new Signature(address + i + offset);

                        if (SignatureList.All(s => s.Address != IntPtr.Zero))
                        {
                            return;
                        }
                    }
                }
            }
        }

        private static void DumpMemory(IntPtr address, int size)
        {
            _dump = Global.Memory.ReadBytes(address, size);
        }

        private static bool CheckSignature(int index, Signature sig)
        {
            for (int i = 0; i < sig.ByteArray.Length; i++)
            {
                if (sig.Mask[i] == '?')
                    continue;

                if (sig.ByteArray[i] != _dump[index + i])
                    return false;
            }

            return true;
        }
    }
}
