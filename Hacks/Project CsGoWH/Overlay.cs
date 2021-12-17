using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Text;
using System.Linq;
using System.Numerics;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
namespace CsGoWH
{
    public partial class Overlay : Form//через свойство
    {
        Memory memory;
        public static Graphics g;
        public static Pen myPen = new Pen(Color.FromArgb(93, 0, 142), 2);
        public static Pen myPen2 = new Pen(Color.FromArgb(255, 0, 0), 1);
        public static Int32 myAdress;
        public static Int32 Enemy;
        public static Vector2 ScreenWH;
        //public readonly int[] BoneNumbers1 = new int[] { 43, 42, 41, 7, 11, 12, 13 };
        //public readonly int[] BoneNumbers2 = new int[] { 79, 78, 77, 0, 70, 71, 72 };
        //public readonly int[] BoneNumbers3 = new int[] { 8, 7, 0 };

        [DllImport("user32.dll", EntryPoint = "GetWindowLong")]
        static extern int GetWindowLong(IntPtr hWnd, int nIndex);

        [DllImport("user32.dll", EntryPoint = "SetWindowLong")]
        private static extern int SetWindowLong(IntPtr hWnd, int nIndex, int dwNewLong);

        public Overlay()
        {
            memory = new Memory("csgo", "client.dll");
            InitializeComponent();
            ScreenWH = new System.Numerics.Vector2(Screen.PrimaryScreen.WorkingArea.Width, Screen.PrimaryScreen.WorkingArea.Height);
        }

        private void Overlay_Load(object sender, EventArgs e)
        {
            this.BackColor = Color.White;
            this.TransparencyKey = Color.White;
            this.TopMost = true;
            this.DoubleBuffered = true;
            this.FormBorderStyle = FormBorderStyle.None;
            int initialStyle = GetWindowLong(this.Handle, -20);
            SetWindowLong(this.Handle, -20, initialStyle | 0x80000 | 0x20);
            this.Size = new Size(Screen.PrimaryScreen.Bounds.Width, Screen.PrimaryScreen.Bounds.Height);
            this.Top = 0;
            this.Left = 0;
            timer1.Start();
            //Activate();
        }

        private void Overlay_FormClosing(object sender, FormClosingEventArgs e)
        {
            timer1.Stop();
            memory.MemCloseHandle();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            this.Refresh();
        }

        private void Overlay_Paint(object sender, PaintEventArgs e)
        {
            g = e.Graphics;
            DrawPlayers();
        }

        private void DrawPlayers()
        {
            myAdress = memory.ReadInt(memory.StartAddres + signatures.dwLocalPlayer);
            int team = memory.ReadInt(myAdress + netvars.m_iTeamNum);
            int healt = memory.ReadInt(myAdress + netvars.m_iHealth);
            g.TextRenderingHint = TextRenderingHint.SingleBitPerPixel;
            g.CompositingQuality = CompositingQuality.HighSpeed;

            for (int i = 0; i < 64; i++)
            {
                Enemy = memory.ReadInt(memory.StartAddres + signatures.dwEntityList + i * 0x10);
                int EnTeam = memory.ReadInt(Enemy + netvars.m_iTeamNum);
                int EnHealth = memory.ReadInt(Enemy + netvars.m_iHealth);
                if (team == EnTeam || EnHealth <= 0)
                    continue;
                Vector3 EnPos = memory.ReadXYZ(Enemy + netvars.m_vecOrigin);
                Vector3 EnHead = memory.ReadBone(Enemy + netvars.m_dwBoneMatrix, 8);
                float[] MyViewMatrix = memory.ReadMatrix(memory.StartAddres + signatures.dwViewMatrix, 16);//count
                Vector2 pos = Math.WorldToScreenVector(EnPos, MyViewMatrix, ScreenWH);
                if (pos.X == 1 && pos.Y == 1)
                    continue;
                Vector2 HeadPos = Math.WorldToScreenVector(EnHead, MyViewMatrix, ScreenWH);
                int height = (int)(pos.Y - HeadPos.Y);
                int width = height / 2; 
                g.DrawString(EnHealth.ToString(), new Font("Arial", 10), Brushes.Blue, pos.X - (width / 2) + 10, pos.Y - height - 20);
                g.DrawRectangle(myPen2, pos.X - (width / 2), pos.Y - height, width, height + 25);
                g.DrawLine(myPen, 960, 1080, pos.X, pos.Y + 20);
            }
        }
    }
}
