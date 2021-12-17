using System;

namespace DDX.Extensions
{
    public static class StringExtension
    {
        public static string CutNull(this string str)
        {
            var chars = str.ToCharArray();
            var result = string.Empty;
            var lastNull = false;

            foreach (var c in chars)
            {
                if (c == '\0')
                {
                    if (lastNull)
                        break;

                    lastNull = true;
                }
                else
                {
                    result += c;
                    lastNull = false;
                }
            }

            return result;
        }

        public static string GetBetween(this string str, string tag, int offset = 0)
        {
            var res1 = str.Split(new[] { tag }, StringSplitOptions.RemoveEmptyEntries)[offset + 1];
            var res2 = res1.Split(new[] { tag }, StringSplitOptions.RemoveEmptyEntries)[0];
            return res2;
        }
    }
}
