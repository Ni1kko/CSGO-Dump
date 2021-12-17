using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CsGoWH
{
    public partial class Form1 : Form
    {
        Overlay overlay;
        public Form1()
        {
            overlay = new Overlay();
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button1_MouseMove(object sender, MouseEventArgs e)
        {
            this.button1.BackColor = Color.FromArgb(160, 0, 0);
        }

        private void button1_MouseLeave(object sender, EventArgs e)
        {
            this.button1.BackColor = Color.FromArgb(61, 61, 61);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.WindowState = FormWindowState.Minimized;
        }

        private void button2_MouseMove(object sender, MouseEventArgs e)
        {
            this.button2.BackColor = Color.FromArgb(102, 0, 156);
        }

        private void button2_MouseLeave(object sender, EventArgs e)
        {
            this.button2.BackColor = Color.FromArgb(61, 61, 61);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.Close();
        }
        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.DrawRectangle(new Pen(Color.FromArgb(93, 0, 142), 10), 0, 0, this.Size.Width, this.Size.Height);
            e.Graphics.FillRectangle(new SolidBrush(Color.FromArgb(93, 0, 142)), 0, 0, this.Size.Width, 37);
            e.Graphics.FillRectangle(new SolidBrush(Color.FromArgb(93, 0, 142)), 0, this.Size.Height-37, this.Size.Width, 37);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (button3.Text == "Start")
            {
                button3.Text = "Stop";
                overlay = new Overlay();
                overlay.Show();
            }
            else if(button3.Text == "Stop")
            {
                button3.Text = "Start";
                overlay.Close();
            }
        }
    }
}
