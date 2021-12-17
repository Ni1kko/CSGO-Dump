using FadeAPI.Helpers;
using SharpDX.Direct2D1;

namespace FadeAPI.Overlay
{
    public class OverlayCore
    {
        //[swap]
        public RenderTarget Device => Form.Device;
        public OverlayForm Form => Global.OverlayForm;
        //[/swap]

        public void ShowOverlay()
        {
            Global.OverlayForm = new OverlayForm();
            Global.OverlayForm.Show();
        }

        public void CloseOverlay()
        {
            if (Global.OverlayForm == null)
                return;

            Global.OverlayForm.Close();
        }
    }
}
