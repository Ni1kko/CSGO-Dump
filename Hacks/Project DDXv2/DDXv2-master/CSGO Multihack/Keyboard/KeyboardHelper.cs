using System.Windows.Forms;

namespace DDX.Keyboard
{
    public static class KeyboardHelper
    {
        public static bool IsKeyPressed(Keys key)
        {
            return (WinApi.GetAsyncKeyState(key) & 0x8000) > 0;
        }
    }
}
