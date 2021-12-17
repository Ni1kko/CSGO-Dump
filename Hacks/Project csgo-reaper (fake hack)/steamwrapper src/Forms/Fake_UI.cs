using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Timers;
using System.Windows.Forms;
using ScriptKidAntiCheat.Utils;
using WMPLib;
using Gma.System.MouseKeyHook;
using System.IO;

namespace ScriptKidAntiCheat
{
    public partial class FakeCheatForm : Form
    {

        private PictureBox title = new PictureBox(); // create a PictureBox
        private Label close = new Label(); // simulates the this.close box

        private bool drag = false; // determine if we should be moving the form
        private Point startPoint = new Point(0, 0); // also for the moving


        public FakeCheatForm()
        {

            InitializeComponent();
            
            this.FormBorderStyle = FormBorderStyle.None;
            this.title.Location = this.Location; 
            this.title.Width = this.Width;
            this.title.Height = 30;
            this.title.BackColor = Color.Black;
            this.Controls.Add(this.title);
            this.title.BringToFront();
            this.close.Text = "Close";
            this.close.Font = new System.Drawing.Font("Impact", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.close.Location = new Point(this.Width - 50, this.Location.Y + 3);
            this.close.ForeColor = Color.White;
            this.close.BackColor = Color.Black;
            this.Controls.Add(this.close);
            this.close.BringToFront();

            this.close.MouseEnter += new EventHandler(Control_MouseEnter);
            this.close.MouseLeave += new EventHandler(Control_MouseLeave);
            this.close.MouseClick += new MouseEventHandler(Control_MouseClick);

            // finally, wouldn't it be nice to get some moveability on this control?
            this.title.MouseDown += new MouseEventHandler(Title_MouseDown);
            this.title.MouseUp += new MouseEventHandler(Title_MouseUp);
            this.title.MouseMove += new MouseEventHandler(Title_MouseMove);

            pictureBox2.BringToFront();

            // Quick buttons
            Program.m_GlobalHook.OnCombination(new Dictionary<Combination, Action>
            {
                {Combination.FromString("F5"), () => { 
                    if(!Program.FakeCheat.InDemo)
                    {
                        Program.FakeCheat.InDemo = true;
                    } else
                    {
                        Program.FakeCheat.InDemo = false;
                    }
                 }},
            });

            label1.BringToFront();


        }

        private void Control_MouseEnter(object sender, EventArgs e)
        {
            if (sender.Equals(this.close))
                this.close.ForeColor = Color.Red;
        }

        private void Control_MouseLeave(object sender, EventArgs e)
        { // return them to their default colours
            if (sender.Equals(this.close))
                this.close.ForeColor = Color.White;
        }

        private void Control_MouseClick(object sender, MouseEventArgs e)
        {
            if (sender.Equals(this.close))
                this.Close(); // close the form
        }

        void Title_MouseUp(object sender, MouseEventArgs e)
        {
            this.drag = false;
        }

        void Title_MouseDown(object sender, MouseEventArgs e)
        {
            this.startPoint = e.Location;
            this.drag = true;
        }

        void Title_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.drag)
            { // if we should be dragging it, we need to figure out some movement
                Point p1 = new Point(e.X, e.Y);
                Point p2 = this.PointToScreen(p1);
                Point p3 = new Point(p2.X - this.startPoint.X,
                                     p2.Y - this.startPoint.Y);
                this.Location = p3;
            }
        }

        private void setCheckBoxColor(Object sender, EventArgs e)
        {
            var cb = sender as CheckBox;
            if(cb.Checked)
            {
                cb.ForeColor = Color.LawnGreen;
            } else
            {
                cb.ForeColor = Color.Red;
            }
        }

        public void log(string msg)
        {
            Console.WriteLine(msg);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (Program.GameProcess != null && Program.GameProcess.Process != null && Program.GameProcess.Process.IsRunning())
            {
                if (!Program.GameData.MatchInfo.IsMatchmaking && !Program.Debug.AllowLocal)
                {
                    MessageBox.Show("You need to join a matchmaking game before injecting.", "No match in progress", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Error);
                }
            }
            else
            {
                MessageBox.Show("CSGO is not running!", "Error", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Error);
            }
        }
    }
}
