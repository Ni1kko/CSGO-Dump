using System;
using System.Windows.Forms;
using FadeAPI.Helpers;
using FadeAPI.Structs;
using SharpDX;
using SharpDX.Direct2D1;
using SharpDX.DXGI;
using SharpDX.Mathematics.Interop;
using AlphaMode = SharpDX.Direct2D1.AlphaMode;
using Factory = SharpDX.Direct2D1.Factory;
using TextAntialiasMode = SharpDX.Direct2D1.TextAntialiasMode;

namespace FadeAPI.Overlay
{
    public partial class OverlayForm : Form
    {
        //[swap]
        public readonly RenderTarget Device;
        public bool Antialiasing;
        //[/swap]

        public OverlayForm()
        {
            //[swap]
            var screenWidth = Screen.PrimaryScreen.Bounds.Width;
            var initialStyle = WinAPI.GetWindowLong(Handle, -20);
            var screenHeight = Screen.PrimaryScreen.Bounds.Height;
            //[/swap]

            //[swap]
            WinAPI.SetWindowLong(Handle, -20, initialStyle | 0x80000 | 0x20);
            WinAPI.SetWindowPos(Handle, new IntPtr(-1), 0, 0, 0, 0, 0);
            //[/swap]

            OnResize(null);
            InitializeComponent();

            //[swap]
            TopMost = true;
            var factory = new Factory();
            Height = screenHeight;
            Width = screenWidth;
            Visible = true;
            //[/swap]

            var renderProperties = new HwndRenderTargetProperties()
            {
                //[swap]
                Hwnd = Handle,
                PixelSize = new Size2(screenWidth, screenHeight),
                PresentOptions = PresentOptions.Immediately,
                //[/swap]
            };

            //[swap]
            Device = new WindowRenderTarget(factory, new RenderTargetProperties(new PixelFormat(Format.B8G8R8A8_UNorm, AlphaMode.Premultiplied)), renderProperties);
            Text = "FadeAPI Overlay";
            //[/swap]
        }

        private void Window_FormClosing(object sender, FormClosingEventArgs e)
        {
            Application.Exit();
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            var marg = new Margins()
            {
                //[swap]
                Bottom = Height,
                Right = Width,
                Left = 0,
                Top = 0,
                //[/swap]
            };

            WinAPI.DwmExtendFrameIntoClientArea(Handle, ref marg);
        }

        public void StartDrawing()
        {
            Device.BeginDraw();

            //[swap]
            Device.Clear(new RawColor4(0, 0, 0, 0));
            Device.TextAntialiasMode = TextAntialiasMode.Default;
            Device.AntialiasMode = Antialiasing ? AntialiasMode.PerPrimitive : AntialiasMode.Aliased;
            //[/swap]
        }

        public void StopDrawing()
        {
            Device.EndDraw();
        }
    }
}
