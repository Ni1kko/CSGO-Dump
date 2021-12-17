using System;

namespace FadeAPI.Structs
{
    using System.Globalization;

    public struct Signature
    {
        //[swap]
        public readonly int Offset;
        public readonly byte[] ByteArray;
        public readonly IntPtr Address;
        public readonly string Mask;
        //[/swap]

        public Signature(byte[] byteArray, string mask, int offset = 0)
        {
            //[swap]
            ByteArray = byteArray;
            Offset    = offset;
            Address   = IntPtr.Zero;
            Mask      = mask;
            //[/swap]
        }

        public Signature(IntPtr address)
        {
            //[swap]
            ByteArray = null;
            Mask      = string.Empty;
            Offset    = 0;
            Address   = address;
            //[/swap]
        }

        public Signature(string sig, int offset = 0)
        {
            var mask = string.Empty;
            var patternBlocks = sig.Split(new[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);

            var pattern = new byte[patternBlocks.Length];

            for (var i = 0; i < patternBlocks.Length; i++)
            {
                var block = patternBlocks[i];
                if (block == "?")
                {
                    mask += '?';
                    pattern[i] = 0;
                }
                else
                {
                    mask += 'x';

                    if (!byte.TryParse(patternBlocks[i], NumberStyles.HexNumber, CultureInfo.DefaultThreadCurrentCulture, out pattern[i]))
                    {
                        throw new Exception("Signature parse error");
                    }
                }
            }

            //[swap]
            ByteArray = pattern;
            Offset = offset;
            Address = IntPtr.Zero;
            Mask = mask;
            //[/swap]
        }
    }
}
