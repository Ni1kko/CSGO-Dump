using FadeAPI.LocalPlayer;
using FadeAPI.Memory;
using FadeAPI.Overlay;

namespace FadeAPI.Helpers
{
    internal static class Global
    {
        //[swap]
        public static MemoryCore Memory;
        public static LocalPlayerCore LocalPlayer;
        public static int LocalPlayerIndex = 0;
        public static System.Diagnostics.Process CsProcess;
        public static long LastExecute = 0;
        public static ulong RefreshId = 1;
        public static OverlayForm OverlayForm;
        //[/swap]
    }
}
