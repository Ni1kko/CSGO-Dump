using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Reflection;
using System.Threading;
using System.Diagnostics;

namespace csgo_wingman_idler
{
    public partial class Form1 : Form
    {
        public class ClickOnPointTool
        {
            [DllImport("user32.dll")]
            static extern bool ClientToScreen(IntPtr hWnd, ref Point lpPoint);
            [DllImport("user32.dll")]
            internal static extern uint SendInput(uint nInputs, [MarshalAs(UnmanagedType.LPArray), In] INPUT[] pInputs, int cbSize);
            #pragma warning disable 649
            internal struct INPUT
            {
                public UInt32 Type;
                public MOUSEKEYBDHARDWAREINPUT Data;
            }

            [StructLayout(LayoutKind.Explicit)]
            internal struct MOUSEKEYBDHARDWAREINPUT
            {
                [FieldOffset(0)]
                public MOUSEINPUT Mouse;
            }
            internal struct MOUSEINPUT
            {
                public Int32 X; public Int32 Y; public UInt32 MouseData; public UInt32 Flags; public UInt32 Time; public IntPtr ExtraInfo;
            }

            #pragma warning restore 649
            public static void ClickOnPoint(IntPtr wndHandle, Point clientPoint)
            {
                var oldPos = Cursor.Position;
                ClientToScreen(wndHandle, ref clientPoint);
                Cursor.Position = new Point(clientPoint.X, clientPoint.Y);
                var inputMouseDown = new INPUT();
                inputMouseDown.Type = 0;
                inputMouseDown.Data.Mouse.Flags = 0x0002;
                var inputMouseUp = new INPUT();
                inputMouseUp.Type = 0;
                inputMouseUp.Data.Mouse.Flags = 0x0004;
                var inputs = new INPUT[] { inputMouseDown, inputMouseUp };
                SendInput((uint)inputs.Length, inputs, Marshal.SizeOf(typeof(INPUT)));
                // Cursor.Position = oldPos;
            }

        }

        sealed class Win32
        {
            [DllImport("user32.dll")]
            static extern IntPtr GetDC(IntPtr hwnd);
            [DllImport("user32.dll")]
            static extern Int32 ReleaseDC(IntPtr hwnd, IntPtr hdc);
            [DllImport("gdi32.dll")]
            static extern uint GetPixel(IntPtr hdc, int nXPos, int nYPos);
            [DllImport("user32.dll", EntryPoint = "FindWindow", SetLastError = true)]
            static extern IntPtr FindWindowByCaption(IntPtr ZeroOnly, string lpWindowName);
            static IntPtr FindWindowByCaption(string caption)
            {
                return FindWindowByCaption(IntPtr.Zero, caption);
            }
            static public System.Drawing.Color GetPixelColor(IntPtr hwnd, int x, int y)
            {
                IntPtr hdc = GetDC(hwnd);
                uint pixel = GetPixel(hdc, x, y);
                ReleaseDC(hwnd, hdc);
                Color color = Color.FromArgb((int)(pixel & 0x000000FF),
                                (int)(pixel & 0x0000FF00) >> 8,
                                (int)(pixel & 0x00FF0000) >> 16);
                return color;
            }
        }


        public Form1()
        {
            InitializeComponent();
        }
        private void Form1_Load(object sender, EventArgs e)
        {
        }
        [DllImport("USER32.DLL", CharSet = CharSet.Unicode)]
        public static extern IntPtr FindWindow(string lpClassName, string lpWindowName);
        [DllImport("gdi32.dll")]
        static extern uint GetPixel(IntPtr hdc, int nXPos, int nYPos);
        IntPtr[] csgo = new IntPtr[20];

        int p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, p5x, p5y, p6x, p6y, p7x, p7y;

        void OneWindow()
        {
            IntPtr win = FindWindow("Valve001", "Counter-Strike: Global Offensive");
            if (win == IntPtr.Zero) { toolStripStatusLabel2.Text = "CS:GO is not running!"; return; }
            int r = 0; int mm = 0;
            int iter = Int32.Parse(textBox1.Text);
            var pixel1 = Win32.GetPixelColor(win, p1x, p1y);
            var pixel2 = Win32.GetPixelColor(win, p2x, p2y);
            var pixel7 = Win32.GetPixelColor(win, p7x, p7y);
            if (pixel1.R != 255 && pixel1.G != 255 & pixel1.B != 255 && pixel2.R != 0 && pixel2.G != 0 & pixel2.B != 0) { toolStripStatusLabel2.Text = "You're not in the main menu!";  goto ll2; }
            else
            {
                toolStripStatusLabel1.Text = "Working";
                for (int i = 0; i < iter; i++)
                {
                    mm++;
                    toolStripStatusLabel2.Text = "Iteration: " + (i) + "/" + iter + " (reconnects: " + r + ")";
                    int m = 0;
                    while (m < 500)
                    {
                        m++;
                        pixel1 = Win32.GetPixelColor(win, p1x, p1y);
                        pixel2 = Win32.GetPixelColor(win, p2x, p2y);
                        pixel7 = Win32.GetPixelColor(win, p7x, p7y);
                        if (pixel1.R == 255 && pixel1.G == 255 && pixel1.B == 255 && pixel2.R == 0 && pixel2.G == 0 && pixel2.B == 0 && pixel7.R == 0) { goto l2; }
                        if (pixel1.R == 255 && pixel1.G == 255 && pixel1.B == 255 && pixel2.R == 0 && pixel2.G == 0 && pixel2.B == 0 && pixel7.R != 0) { goto l1; }
                        System.Threading.Thread.Sleep(5000);
                        Application.DoEvents();
                    }
                l1: Cursor.Position = new Point(p7x, p7y);                           // Reconnect Button
                    ClickOnPointTool.ClickOnPoint(win, Cursor.Position); r++; goto ll1;
                l2: Cursor.Position = new Point(p1x, p1y);                            // Play Button
                    ClickOnPointTool.ClickOnPoint(win, Cursor.Position);
                    System.Threading.Thread.Sleep(1000); 
                    Cursor.Position = new Point(p4x, p4y);                            // Wingman mode select
                    ClickOnPointTool.ClickOnPoint(win, Cursor.Position);
                    System.Threading.Thread.Sleep(1000);
                    var pixel5 = Win32.GetPixelColor(win, p5x, p5y);
                    Cursor.Position = new Point(p5x, p5y);                          // Search the match Button
                    ClickOnPointTool.ClickOnPoint(win, Cursor.Position);
                    System.Threading.Thread.Sleep(300); 
                    var pixel6 = Win32.GetPixelColor(win, p5x, p5y);
                    if (pixel6.R != pixel5.R) { goto l3; }
                    System.Threading.Thread.Sleep(500); 
                    Cursor.Position = new Point(p5x, p5y);                          // Search the match Button one more time
                    ClickOnPointTool.ClickOnPoint(win, Cursor.Position);
                l3: System.Threading.Thread.Sleep(1000);
                    var pixel3 = Win32.GetPixelColor(win, p3x, p3y);
                    int n = 0;
                    while (n < 150)
                    {
                        System.Threading.Thread.Sleep(5000); 
                        n++;
                        var pixel4 = Win32.GetPixelColor(win, p3x, p3y);
                        if (pixel4.R != pixel3.R) { goto l4; }
                        Application.DoEvents();
                    }
                l4: var pixel8 = Win32.GetPixelColor(win, p3x, p3y);
                    System.Threading.Thread.Sleep(1000); 
                    Cursor.Position = new Point(p6x, p6y);
                    ClickOnPointTool.ClickOnPoint(win, Cursor.Position);
                    System.Threading.Thread.Sleep(1000); 
                    Cursor.Position = new Point(p6x, p6y);
                    ClickOnPointTool.ClickOnPoint(win, Cursor.Position);
                    int p = 0;
                    while (p < 30)
                    {
                        System.Threading.Thread.Sleep(3000); 
                        p++;
                        var pixel9 = Win32.GetPixelColor(win, p3x, p3y);
                        if (pixel9.R == pixel8.R && pixel9.G == pixel8.G) { goto l4; }
                        Application.DoEvents();
                    }
                    Application.DoEvents();
               ll1: System.Threading.Thread.Sleep(1000); 
                }
            }
            pixel1 = Win32.GetPixelColor(win, p1x, p1y);
            pixel2 = Win32.GetPixelColor(win, p2x, p2y);
            pixel7 = Win32.GetPixelColor(win, p7x, p7y);
            if (pixel1.R == 255 && pixel1.G == 255 && pixel1.B == 255 && pixel2.R == 0 && pixel2.G == 0 && pixel2.B == 0 && pixel7.R == 0) { toolStripStatusLabel1.Text = "Ready"; toolStripStatusLabel2.Text = "Done: " + mm + " cycles" + " (reconnects: " + r + "of this reconnects"; return; }
        ll2: System.Threading.Thread.Sleep(1000);
        }


        Thread Thread1;
        private void button1_Click(object sender, EventArgs e)
        {
                if (radioButton1.Checked == true)
                {
                    p1x = 15; p1y = 35;
                    p2x = 30; p2y = 32;
                    p7x = 300; p7y = 18;
                    p3x = 190; p3y = 173;
                    p4x = 90; p4y = 55;
                    p5x = 300; p5y = 285;
                    p6x = 200; p6y = 160;

                }
                if (radioButton2.Checked == true)
                {
                    p1x = 24; p1y = 56;
                    p2x = 48; p2y = 52;
                    p7x = 480; p7y = 29;
                    p3x = 304; p3y = 277;
                    p4x = 144; p4y = 88;
                    p5x = 480; p5y = 456;
                    p6x = 320; p6y = 256;

                }
                Thread1 = new Thread(new ThreadStart(OneWindow));
                Thread1.Start();
         }

        private void button4_Click(object sender, EventArgs e)
        {
            MessageBox.Show("CSGO-wingman-idler v0.1." + "\n" + "CS:GO resolution: 400x300 or 640x480, Fonts and backgrounds disabled." + "\n" + "Supported single window (Later Sandbox)", "CSGO-Wingman-Idler v0.1");
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Thread1.Abort();
            Environment.Exit(0);
        }

        private void button3_Click(object sender, EventArgs e)
        {

        }
    }
}
