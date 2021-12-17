using System;
using System.Windows.Forms;
using DDX.Keyboard;
using DDX.Views;

namespace DDX.Services
{
    public class KeyboardService : IDisposable
    {
        private readonly MenuWindow _window;
        private readonly KeyboardHook _hook;

        private bool _working;

        public KeyboardService(MenuWindow window)
        {
            _window = window;
            _hook = new KeyboardHook();
        }

        public void Dispose()
        {
            Stop();
        }

        public void Start()
        {
            if (_working) return;

            _hook.Hook();

            _hook.HookedKeys.Add(Keys.Delete);
            _hook.HookedKeys.Add(Keys.End);
            _hook.HookedKeys.Add(Keys.F9);
            _hook.HookedKeys.Add(Keys.F10);
            _hook.HookedKeys.Add(Keys.F11);
            _hook.HookedKeys.Add(Keys.F12);
            _hook.HookedKeys.Add(Keys.Home);
            _hook.HookedKeys.Add(Keys.Insert);
            _hook.HookedKeys.Add(Keys.PageDown);
            _hook.HookedKeys.Add(Keys.PageUp);

            _hook.KeyDown += KeyDown;

            _working = true;
        }

        public void Stop()
        {
            if (!_working) return;

            _hook.KeyDown -= KeyDown;

            _hook.HookedKeys.Clear();

            _hook.Unhook();

            _working = false;
        }

        private void KeyDown(object sender, KeyEventArgs e)
        {
            // Exit
            if (e.KeyCode == G.C.Key.Exit)
            {
                _window.Dispatcher.Invoke(() =>
                {
                    _window.Close();
                });
            }

            // Toggle menu window
            if (e.KeyCode == G.C.Key.Toggle)
            {
                _window.Dispatcher.Invoke(() =>
                {
                    if (_window.IsVisible)
                    {
                        _window.Hide();
                    }
                    else
                    {
                        _window.Show();
                        _window.Activate();
                    }
                });
            }
        }
    }
}
