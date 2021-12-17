using System;

namespace DDX_Multihack
{
    internal static class Rnd
    {
        private static readonly Random rnd = new Random(Environment.TickCount);

        public static string GetRandomString(int length = 32)
        {
            var returnValue = string.Empty;

            for (int i = 0; i < length; i++)
            {
                var intChar = rnd.Next(33, 126 + 1);
                returnValue += (char)intChar;
            }

            return returnValue;
        }
    }
}