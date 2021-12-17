using FadeAPI.Helpers;
using FadeAPI.Structs;

namespace FadeAPI.Window
{
    public class WindowCore
    {
        public WindowInfo GetWindowInfo()
        {
            //[swap]
            var windowRect = new Margins();
            var handle = System.Diagnostics.Process.GetProcessesByName("csgo")[0].MainWindowHandle;
            //[/swap]

            WinAPI.GetWindowRect(handle, ref windowRect);

            var returnWindowInfo = new WindowInfo
            {
                //[swap]
                Height = windowRect.Bottom - windowRect.Top,
                Y      = windowRect.Top,
                X      = windowRect.Left,
                Width  = windowRect.Right - windowRect.Left,
                //[/swap]
            };

            return returnWindowInfo;
        }
    }
}
