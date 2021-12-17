using System;
using System.Windows.Forms;
using DDX.Structs;
using SharpDX;
using SharpDX.Direct2D1;
using SharpDX.DXGI;
using AlphaMode = SharpDX.Direct2D1.AlphaMode;
using Factory = SharpDX.Direct2D1.Factory;

namespace DDX.Forms
{
    public partial class OverlayForm : Form
    {
        public readonly RenderTarget D;

        protected override void OnPaint(PaintEventArgs e)
        {
            var marg = new Margins
            {
                Left = 0,
                Top = 0,
                Right = Width,
                Bottom = Height,
            };

            WinApi.DwmExtendFrameIntoClientArea(Handle, ref marg);
        }

        protected override CreateParams CreateParams
        {
            get
            {
                var Params = base.CreateParams;
                Params.ExStyle |= 0x80;
                return Params;
            }
        }

        public OverlayForm()
        {
            OnResize(null);
            InitializeComponent();

            Left = 0;
            Top = 0;
            Width = (int)G.ScreenParameters.Width;
            Height = (int)G.ScreenParameters.Height;

            var initStyle = WinApi.GetWindowLong(Handle, -20);
            WinApi.SetWindowLong(Handle, -20, initStyle | 0x20 | 0x80000);
            WinApi.SetWindowPos(Handle, new IntPtr(-1), 0, 0, 0, 0, 0x1 | 0x2);

            // Makes antialiasing look better but doesn't work for all users
            if (G.C.Stts.BetterAntialiasing)
            {
                WinApi.SetLayeredWindowAttributes(Handle, 0, 255, 0x2);
            }

            var pixelFormat = new PixelFormat(Format.B8G8R8A8_UNorm, AlphaMode.Premultiplied);

            var renderTargetProperties = new RenderTargetProperties(pixelFormat)
            {
                MinLevel = FeatureLevel.Level_DEFAULT,
                Type = RenderTargetType.Hardware,
                Usage = RenderTargetUsage.None,
            };

            var renderProperties = new HwndRenderTargetProperties
            {
                Hwnd = Handle,
                PixelSize = new Size2(Width, Height),
                PresentOptions = PresentOptions.Immediately,
            };

            D = new WindowRenderTarget(new Factory(), renderTargetProperties, renderProperties);
        }
    }
}
