using System.Windows.Forms;
using FadeAPI.Enums;
using FadeAPI.Helpers;

namespace FadeAPI.Keyboard
{
    public class KeyboardCore
    {
        public bool IsKeyPressed(Keys key)
        {
            var keyState = WinAPI.GetAsyncKeyState(key);
            return (keyState & 0x8000) > 0;
        }
    }
}
