using FadeAPI.GameObjects;
using FadeAPI.Helpers;
using FadeAPI.Keyboard;
using FadeAPI.LocalPlayer;
using FadeAPI.Overlay;
using FadeAPI.Players;
using FadeAPI.Process;
using FadeAPI.Window;

namespace FadeAPI
{
    public class Fade
    {
        //[swap]
        public PlayersCore Players { get; }
        public KeyboardCore Keyboard { get; }
        public WindowCore Window { get; }
        public OverlayCore Overlay { get; }
        public ProcessCore Process { get; }
        public KeyboardHook KeyboardHook { get; }
        public GameObjectsCore GameObjects { get; }
        public LocalPlayerCore LocalPlayer { get; }
        public Functions Functions { get; }
        //[/swap]

        public Fade(bool autoAttach = false)
        {
            //[swap]
            Window       = new WindowCore();
            GameObjects  = new GameObjectsCore();
            Players      = new PlayersCore();
            KeyboardHook = new KeyboardHook();
            LocalPlayer  = new LocalPlayerCore();
            Process      = new ProcessCore();
            Functions    = new Functions();
            Overlay      = new OverlayCore();
            Keyboard     = new KeyboardCore();
            //[/swap]

            if (autoAttach)
                Process.Attach();
        }

        public void Refresh()
        {
            Global.RefreshId++;
        }
    }
}
