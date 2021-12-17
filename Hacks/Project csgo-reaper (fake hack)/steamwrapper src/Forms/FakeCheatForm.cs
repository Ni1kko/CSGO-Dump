// Decompiled with JetBrains decompiler
// Type: ScriptKidAntiCheat.FakeCheatForm
// Assembly: SteamWrapper, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b9d60819726d5649
// MVID: 52813C95-239A-43DA-B46E-DCB9B069035E
// Assembly location: C:\Users\Ni1kk\Desktop\New folder\bin\SteamWrapper.exe

using ScriptKidAntiCheat.Utils;
using System;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;

namespace ScriptKidAntiCheat
{
  public class FakeCheatForm : Form
  {
    private PictureBox title = new PictureBox();
    private Label close = new Label();
    private Point startPoint = new Point(0, 0);
    private bool drag;
    private IContainer components;
    private Timer timer1;
    private Label label1;
    private TabControl tabControl1;
    private TabPage tabPage1;
    private TabPage tabPage2;
    private Label label2;
    private TextBox textBox1;
    private Label label3;
    private TextBox textBox2;
    private Label label4;
    private TextBox textBox3;
    private Label label5;
    private TextBox textBox4;
    private Label label6;
    private TextBox textBox5;
    private CheckBox checkBox1;
    private CheckBox checkBox2;
    private CheckBox checkBox3;
    private CheckBox checkBox8;
    private CheckBox checkBox7;
    private CheckBox checkBox6;
    private CheckBox checkBox5;
    private Label label7;
    private CheckBox checkBox4;
    private Label label8;
    private Label label9;
    private TextBox textBox6;
    private CheckBox checkBox9;
    private TabPage tabPage3;
    private CheckBox checkBox11;
    private Label label10;
    private CheckBox checkBox10;
    private Label label11;
    private ComboBox comboBox1;
    private CheckBox checkBox13;
    private CheckBox checkBox12;
    private CheckBox checkBox14;
    private CheckBox checkBox15;
    private CheckBox checkBox16;
    private TabPage tabPage4;
    private Label label13;
    private CheckBox checkBox19;
    private CheckBox checkBox20;
    private CheckBox checkBox18;
    private CheckBox checkBox17;
    private TabPage tabPage5;
    private CheckBox checkBox21;
    private Label label14;
    private CheckBox checkBox22;
    private CheckBox checkBox23;
    private CheckBox checkBox24;
    private CheckBox checkBox25;
    private Label label15;
    private CheckBox checkBox26;
    private CheckBox checkBox27;
    private CheckBox checkBox28;
    private Label label12;
    private TextBox textBox7;
    private PictureBox pictureBox1;
    private Button button1;
    private PictureBox pictureBox2;

    public FakeCheatForm()
    {
      this.InitializeComponent();
      this.FormBorderStyle = FormBorderStyle.None;
      this.title.Location = this.Location;
      this.title.Width = this.Width;
      this.title.Height = 30;
      this.title.BackColor = Color.Black;
      this.Controls.Add((Control) this.title);
      this.title.BringToFront();
      this.close.Text = "Close";
      this.close.Font = new Font("Impact", 12f, FontStyle.Regular, GraphicsUnit.Point, (byte) 0);
      this.close.Location = new Point(this.Width - 50, this.Location.Y + 3);
      this.close.ForeColor = Color.White;
      this.close.BackColor = Color.Black;
      this.Controls.Add((Control) this.close);
      this.close.BringToFront();
      this.close.MouseEnter += new EventHandler(this.Control_MouseEnter);
      this.close.MouseLeave += new EventHandler(this.Control_MouseLeave);
      this.close.MouseClick += new MouseEventHandler(this.Control_MouseClick);
      this.title.MouseDown += new MouseEventHandler(this.Title_MouseDown);
      this.title.MouseUp += new MouseEventHandler(this.Title_MouseUp);
      this.title.MouseMove += new MouseEventHandler(this.Title_MouseMove);
      this.pictureBox2.BringToFront();
      this.label1.BringToFront();
    }

    private void Control_MouseEnter(object sender, EventArgs e)
    {
      if (!sender.Equals((object) this.close))
        return;
      this.close.ForeColor = Color.Red;
    }

    private void Control_MouseLeave(object sender, EventArgs e)
    {
      if (!sender.Equals((object) this.close))
        return;
      this.close.ForeColor = Color.White;
    }

    private void Control_MouseClick(object sender, MouseEventArgs e)
    {
      if (!sender.Equals((object) this.close))
        return;
      this.Close();
    }

    private void Title_MouseUp(object sender, MouseEventArgs e)
    {
      this.drag = false;
    }

    private void Title_MouseDown(object sender, MouseEventArgs e)
    {
      this.startPoint = e.Location;
      this.drag = true;
    }

    private void Title_MouseMove(object sender, MouseEventArgs e)
    {
      if (!this.drag)
        return;
      Point screen = this.PointToScreen(new Point(e.X, e.Y));
      this.Location = new Point(screen.X - this.startPoint.X, screen.Y - this.startPoint.Y);
    }

    private void setCheckBoxColor(object sender, EventArgs e)
    {
      CheckBox checkBox = sender as CheckBox;
      if (checkBox.Checked)
        checkBox.ForeColor = Color.LawnGreen;
      else
        checkBox.ForeColor = Color.Red;
    }

    public void log(string msg)
    {
      Console.WriteLine(msg);
    }

    private void button1_Click(object sender, EventArgs e)
    {
      if (Program.GameProcess != null && Program.GameProcess.Process != null && Program.GameProcess.Process.IsRunning())
      {
        if (Program.GameData.MatchInfo.IsMatchmaking || Program.Debug.AllowLocal)
          return;
        Analytics.TrackEvent("InjectButton", "NotInMatchmaking", "", 0);
        int num = (int) MessageBox.Show("You need to join a matchmaking game before injecting.", "No match in progress", MessageBoxButtons.OK, MessageBoxIcon.Hand);
      }
      else
      {
        Analytics.TrackEvent("InjectButton", "NotRunningCSGO", "", 0);
        int num = (int) MessageBox.Show("CSGO is not running!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Hand);
      }
    }

    protected override void Dispose(bool disposing)
    {
      if (disposing && this.components != null)
        this.components.Dispose();
      base.Dispose(disposing);
    }

    private void InitializeComponent()
    {
      this.components = (IContainer) new Container();
      ComponentResourceManager componentResourceManager = new ComponentResourceManager(typeof (FakeCheatForm));
      this.timer1 = new Timer(this.components);
      this.label1 = new Label();
      this.tabControl1 = new TabControl();
      this.tabPage2 = new TabPage();
      this.checkBox9 = new CheckBox();
      this.label9 = new Label();
      this.textBox6 = new TextBox();
      this.label8 = new Label();
      this.checkBox8 = new CheckBox();
      this.checkBox7 = new CheckBox();
      this.checkBox6 = new CheckBox();
      this.checkBox5 = new CheckBox();
      this.label7 = new Label();
      this.checkBox4 = new CheckBox();
      this.checkBox3 = new CheckBox();
      this.checkBox2 = new CheckBox();
      this.checkBox1 = new CheckBox();
      this.tabPage5 = new TabPage();
      this.label12 = new Label();
      this.textBox7 = new TextBox();
      this.checkBox21 = new CheckBox();
      this.label14 = new Label();
      this.checkBox22 = new CheckBox();
      this.checkBox23 = new CheckBox();
      this.checkBox24 = new CheckBox();
      this.checkBox25 = new CheckBox();
      this.label15 = new Label();
      this.checkBox26 = new CheckBox();
      this.checkBox27 = new CheckBox();
      this.checkBox28 = new CheckBox();
      this.tabPage3 = new TabPage();
      this.checkBox16 = new CheckBox();
      this.checkBox15 = new CheckBox();
      this.checkBox14 = new CheckBox();
      this.checkBox13 = new CheckBox();
      this.checkBox12 = new CheckBox();
      this.label11 = new Label();
      this.comboBox1 = new ComboBox();
      this.checkBox11 = new CheckBox();
      this.label10 = new Label();
      this.checkBox10 = new CheckBox();
      this.tabPage4 = new TabPage();
      this.checkBox18 = new CheckBox();
      this.checkBox17 = new CheckBox();
      this.label13 = new Label();
      this.checkBox19 = new CheckBox();
      this.checkBox20 = new CheckBox();
      this.tabPage1 = new TabPage();
      this.label6 = new Label();
      this.textBox5 = new TextBox();
      this.label5 = new Label();
      this.textBox4 = new TextBox();
      this.label4 = new Label();
      this.textBox3 = new TextBox();
      this.label3 = new Label();
      this.textBox2 = new TextBox();
      this.label2 = new Label();
      this.textBox1 = new TextBox();
      this.pictureBox1 = new PictureBox();
      this.button1 = new Button();
      this.pictureBox2 = new PictureBox();
      this.tabControl1.SuspendLayout();
      this.tabPage2.SuspendLayout();
      this.tabPage5.SuspendLayout();
      this.tabPage3.SuspendLayout();
      this.tabPage4.SuspendLayout();
      this.tabPage1.SuspendLayout();
      ((ISupportInitialize) this.pictureBox1).BeginInit();
      ((ISupportInitialize) this.pictureBox2).BeginInit();
      this.SuspendLayout();
      this.label1.AutoSize = true;
      this.label1.BackColor = Color.Black;
      this.label1.Font = new Font("Impact", 12f, FontStyle.Regular, GraphicsUnit.Point, (byte) 0);
      this.label1.ForeColor = Color.White;
      this.label1.Location = new Point(32, 4);
      this.label1.Name = "label1";
      this.label1.Size = new Size(94, 20);
      this.label1.TabIndex = 1;
      this.label1.Text = "CSGO REAPER";
      this.tabControl1.Controls.Add((Control) this.tabPage2);
      this.tabControl1.Controls.Add((Control) this.tabPage5);
      this.tabControl1.Controls.Add((Control) this.tabPage3);
      this.tabControl1.Controls.Add((Control) this.tabPage4);
      this.tabControl1.Controls.Add((Control) this.tabPage1);
      this.tabControl1.Location = new Point(11, 47);
      this.tabControl1.Name = "tabControl1";
      this.tabControl1.SelectedIndex = 0;
      this.tabControl1.Size = new Size(729, 434);
      this.tabControl1.TabIndex = 2;
      this.tabPage2.Controls.Add((Control) this.checkBox9);
      this.tabPage2.Controls.Add((Control) this.label9);
      this.tabPage2.Controls.Add((Control) this.textBox6);
      this.tabPage2.Controls.Add((Control) this.label8);
      this.tabPage2.Controls.Add((Control) this.checkBox8);
      this.tabPage2.Controls.Add((Control) this.checkBox7);
      this.tabPage2.Controls.Add((Control) this.checkBox6);
      this.tabPage2.Controls.Add((Control) this.checkBox5);
      this.tabPage2.Controls.Add((Control) this.label7);
      this.tabPage2.Controls.Add((Control) this.checkBox4);
      this.tabPage2.Controls.Add((Control) this.checkBox3);
      this.tabPage2.Controls.Add((Control) this.checkBox2);
      this.tabPage2.Controls.Add((Control) this.checkBox1);
      this.tabPage2.Location = new Point(4, 22);
      this.tabPage2.Name = "tabPage2";
      this.tabPage2.Padding = new Padding(3);
      this.tabPage2.Size = new Size(721, 408);
      this.tabPage2.TabIndex = 1;
      this.tabPage2.Text = "Aimbot";
      this.tabPage2.UseVisualStyleBackColor = true;
      this.checkBox9.AutoSize = true;
      this.checkBox9.Location = new Point(27, 116);
      this.checkBox9.Name = "checkBox9";
      this.checkBox9.Size = new Size(56, 17);
      this.checkBox9.TabIndex = 16;
      this.checkBox9.Text = "Recoil";
      this.checkBox9.UseVisualStyleBackColor = true;
      this.label9.AutoSize = true;
      this.label9.Font = new Font("Microsoft Sans Serif", 8.25f, FontStyle.Bold, GraphicsUnit.Point, (byte) 0);
      this.label9.Location = new Point(24, 147);
      this.label9.Name = "label9";
      this.label9.Size = new Size(66, 13);
      this.label9.TabIndex = 15;
      this.label9.Text = "HitChance";
      this.textBox6.Location = new Point(27, 166);
      this.textBox6.Name = "textBox6";
      this.textBox6.Size = new Size(100, 20);
      this.textBox6.TabIndex = 14;
      this.textBox6.Text = "100%";
      this.label8.AutoSize = true;
      this.label8.Font = new Font("Microsoft Sans Serif", 8.25f, FontStyle.Bold, GraphicsUnit.Point, (byte) 0);
      this.label8.Location = new Point(24, 24);
      this.label8.Name = "label8";
      this.label8.Size = new Size(51, 13);
      this.label8.TabIndex = 13;
      this.label8.Text = "General";
      this.checkBox8.AutoSize = true;
      this.checkBox8.Location = new Point(290, 138);
      this.checkBox8.Name = "checkBox8";
      this.checkBox8.Size = new Size(49, 17);
      this.checkBox8.TabIndex = 12;
      this.checkBox8.Text = "Legs";
      this.checkBox8.UseVisualStyleBackColor = true;
      this.checkBox7.AutoSize = true;
      this.checkBox7.Location = new Point(290, 115);
      this.checkBox7.Name = "checkBox7";
      this.checkBox7.Size = new Size(49, 17);
      this.checkBox7.TabIndex = 11;
      this.checkBox7.Text = "Arms";
      this.checkBox7.UseVisualStyleBackColor = true;
      this.checkBox6.AutoSize = true;
      this.checkBox6.Checked = true;
      this.checkBox6.CheckState = CheckState.Checked;
      this.checkBox6.Location = new Point(290, 92);
      this.checkBox6.Name = "checkBox6";
      this.checkBox6.Size = new Size(68, 17);
      this.checkBox6.TabIndex = 10;
      this.checkBox6.Text = "Stomach";
      this.checkBox6.UseVisualStyleBackColor = true;
      this.checkBox5.AutoSize = true;
      this.checkBox5.Checked = true;
      this.checkBox5.CheckState = CheckState.Checked;
      this.checkBox5.Location = new Point(290, 69);
      this.checkBox5.Name = "checkBox5";
      this.checkBox5.Size = new Size(53, 17);
      this.checkBox5.TabIndex = 9;
      this.checkBox5.Text = "Chest";
      this.checkBox5.UseVisualStyleBackColor = true;
      this.label7.AutoSize = true;
      this.label7.Font = new Font("Microsoft Sans Serif", 8.25f, FontStyle.Bold, GraphicsUnit.Point, (byte) 0);
      this.label7.Location = new Point(284, 24);
      this.label7.Name = "label7";
      this.label7.Size = new Size(75, 13);
      this.label7.TabIndex = 8;
      this.label7.Text = "TriggerFilter";
      this.checkBox4.AutoSize = true;
      this.checkBox4.Checked = true;
      this.checkBox4.CheckState = CheckState.Checked;
      this.checkBox4.Location = new Point(290, 46);
      this.checkBox4.Name = "checkBox4";
      this.checkBox4.Size = new Size(52, 17);
      this.checkBox4.TabIndex = 7;
      this.checkBox4.Text = "Head";
      this.checkBox4.UseVisualStyleBackColor = true;
      this.checkBox3.AutoSize = true;
      this.checkBox3.Location = new Point(27, 92);
      this.checkBox3.Name = "checkBox3";
      this.checkBox3.Size = new Size(162, 17);
      this.checkBox3.TabIndex = 6;
      this.checkBox3.Text = "Simulate Realistic Movement";
      this.checkBox3.UseVisualStyleBackColor = true;
      this.checkBox2.AutoSize = true;
      this.checkBox2.Location = new Point(27, 69);
      this.checkBox2.Name = "checkBox2";
      this.checkBox2.Size = new Size(82, 17);
      this.checkBox2.TabIndex = 5;
      this.checkBox2.Text = "Friendly Fire";
      this.checkBox2.UseVisualStyleBackColor = true;
      this.checkBox1.AutoSize = true;
      this.checkBox1.Location = new Point(27, 46);
      this.checkBox1.Name = "checkBox1";
      this.checkBox1.Size = new Size(72, 17);
      this.checkBox1.TabIndex = 4;
      this.checkBox1.Text = "Silent Aim";
      this.checkBox1.UseVisualStyleBackColor = true;
      this.tabPage5.Controls.Add((Control) this.label12);
      this.tabPage5.Controls.Add((Control) this.textBox7);
      this.tabPage5.Controls.Add((Control) this.checkBox21);
      this.tabPage5.Controls.Add((Control) this.label14);
      this.tabPage5.Controls.Add((Control) this.checkBox22);
      this.tabPage5.Controls.Add((Control) this.checkBox23);
      this.tabPage5.Controls.Add((Control) this.checkBox24);
      this.tabPage5.Controls.Add((Control) this.checkBox25);
      this.tabPage5.Controls.Add((Control) this.label15);
      this.tabPage5.Controls.Add((Control) this.checkBox26);
      this.tabPage5.Controls.Add((Control) this.checkBox27);
      this.tabPage5.Controls.Add((Control) this.checkBox28);
      this.tabPage5.Location = new Point(4, 22);
      this.tabPage5.Name = "tabPage5";
      this.tabPage5.Padding = new Padding(3);
      this.tabPage5.Size = new Size(721, 408);
      this.tabPage5.TabIndex = 4;
      this.tabPage5.Text = "Triggerbot";
      this.tabPage5.UseVisualStyleBackColor = true;
      this.label12.AutoSize = true;
      this.label12.Font = new Font("Microsoft Sans Serif", 8.25f, FontStyle.Bold, GraphicsUnit.Point, (byte) 0);
      this.label12.Location = new Point(24, 129);
      this.label12.Name = "label12";
      this.label12.Size = new Size(174, 13);
      this.label12.TabIndex = 31;
      this.label12.Text = "Only trigger while holding key";
      this.textBox7.Location = new Point(27, 148);
      this.textBox7.Name = "textBox7";
      this.textBox7.Size = new Size(100, 20);
      this.textBox7.TabIndex = 30;
      this.textBox7.Text = "Mouse3";
      this.checkBox21.AutoSize = true;
      this.checkBox21.Location = new Point(27, 93);
      this.checkBox21.Name = "checkBox21";
      this.checkBox21.Size = new Size(143, 17);
      this.checkBox21.TabIndex = 29;
      this.checkBox21.Text = "Always trigger while ADS";
      this.checkBox21.UseVisualStyleBackColor = true;
      this.label14.AutoSize = true;
      this.label14.Font = new Font("Microsoft Sans Serif", 8.25f, FontStyle.Bold, GraphicsUnit.Point, (byte) 0);
      this.label14.Location = new Point(24, 24);
      this.label14.Name = "label14";
      this.label14.Size = new Size(51, 13);
      this.label14.TabIndex = 26;
      this.label14.Text = "General";
      this.checkBox22.AutoSize = true;
      this.checkBox22.Location = new Point(290, 138);
      this.checkBox22.Name = "checkBox22";
      this.checkBox22.Size = new Size(49, 17);
      this.checkBox22.TabIndex = 25;
      this.checkBox22.Text = "Legs";
      this.checkBox22.UseVisualStyleBackColor = true;
      this.checkBox23.AutoSize = true;
      this.checkBox23.Location = new Point(290, 115);
      this.checkBox23.Name = "checkBox23";
      this.checkBox23.Size = new Size(49, 17);
      this.checkBox23.TabIndex = 24;
      this.checkBox23.Text = "Arms";
      this.checkBox23.UseVisualStyleBackColor = true;
      this.checkBox24.AutoSize = true;
      this.checkBox24.Checked = true;
      this.checkBox24.CheckState = CheckState.Checked;
      this.checkBox24.Location = new Point(290, 92);
      this.checkBox24.Name = "checkBox24";
      this.checkBox24.Size = new Size(68, 17);
      this.checkBox24.TabIndex = 23;
      this.checkBox24.Text = "Stomach";
      this.checkBox24.UseVisualStyleBackColor = true;
      this.checkBox25.AutoSize = true;
      this.checkBox25.Checked = true;
      this.checkBox25.CheckState = CheckState.Checked;
      this.checkBox25.Location = new Point(290, 69);
      this.checkBox25.Name = "checkBox25";
      this.checkBox25.Size = new Size(53, 17);
      this.checkBox25.TabIndex = 22;
      this.checkBox25.Text = "Chest";
      this.checkBox25.UseVisualStyleBackColor = true;
      this.label15.AutoSize = true;
      this.label15.Font = new Font("Microsoft Sans Serif", 8.25f, FontStyle.Bold, GraphicsUnit.Point, (byte) 0);
      this.label15.Location = new Point(284, 24);
      this.label15.Name = "label15";
      this.label15.Size = new Size(75, 13);
      this.label15.TabIndex = 21;
      this.label15.Text = "TriggerFilter";
      this.checkBox26.AutoSize = true;
      this.checkBox26.Checked = true;
      this.checkBox26.CheckState = CheckState.Checked;
      this.checkBox26.Location = new Point(290, 46);
      this.checkBox26.Name = "checkBox26";
      this.checkBox26.Size = new Size(52, 17);
      this.checkBox26.TabIndex = 20;
      this.checkBox26.Text = "Head";
      this.checkBox26.UseVisualStyleBackColor = true;
      this.checkBox27.AutoSize = true;
      this.checkBox27.Location = new Point(27, 69);
      this.checkBox27.Name = "checkBox27";
      this.checkBox27.Size = new Size(167, 17);
      this.checkBox27.TabIndex = 19;
      this.checkBox27.Text = "Simulate realistic reaction time";
      this.checkBox27.UseVisualStyleBackColor = true;
      this.checkBox28.AutoSize = true;
      this.checkBox28.Location = new Point(27, 46);
      this.checkBox28.Name = "checkBox28";
      this.checkBox28.Size = new Size(82, 17);
      this.checkBox28.TabIndex = 18;
      this.checkBox28.Text = "Friendly Fire";
      this.checkBox28.UseVisualStyleBackColor = true;
      this.tabPage3.Controls.Add((Control) this.checkBox16);
      this.tabPage3.Controls.Add((Control) this.checkBox15);
      this.tabPage3.Controls.Add((Control) this.checkBox14);
      this.tabPage3.Controls.Add((Control) this.checkBox13);
      this.tabPage3.Controls.Add((Control) this.checkBox12);
      this.tabPage3.Controls.Add((Control) this.label11);
      this.tabPage3.Controls.Add((Control) this.comboBox1);
      this.tabPage3.Controls.Add((Control) this.checkBox11);
      this.tabPage3.Controls.Add((Control) this.label10);
      this.tabPage3.Controls.Add((Control) this.checkBox10);
      this.tabPage3.Location = new Point(4, 22);
      this.tabPage3.Name = "tabPage3";
      this.tabPage3.Padding = new Padding(3);
      this.tabPage3.Size = new Size(721, 408);
      this.tabPage3.TabIndex = 2;
      this.tabPage3.Text = "Wallhack";
      this.tabPage3.UseVisualStyleBackColor = true;
      this.checkBox16.AutoSize = true;
      this.checkBox16.Location = new Point(26, 249);
      this.checkBox16.Name = "checkBox16";
      this.checkBox16.Size = new Size(159, 17);
      this.checkBox16.TabIndex = 23;
      this.checkBox16.Text = "Do not show through smoke";
      this.checkBox16.UseVisualStyleBackColor = true;
      this.checkBox15.AutoSize = true;
      this.checkBox15.Location = new Point(26, 226);
      this.checkBox15.Name = "checkBox15";
      this.checkBox15.Size = new Size(140, 17);
      this.checkBox15.TabIndex = 22;
      this.checkBox15.Text = "Do not show teammates";
      this.checkBox15.UseVisualStyleBackColor = true;
      this.checkBox14.AutoSize = true;
      this.checkBox14.Checked = true;
      this.checkBox14.CheckState = CheckState.Checked;
      this.checkBox14.Location = new Point(26, 201);
      this.checkBox14.Name = "checkBox14";
      this.checkBox14.Size = new Size(96, 17);
      this.checkBox14.TabIndex = 21;
      this.checkBox14.Text = "Show skeleton";
      this.checkBox14.UseVisualStyleBackColor = true;
      this.checkBox13.AutoSize = true;
      this.checkBox13.Location = new Point(26, 177);
      this.checkBox13.Name = "checkBox13";
      this.checkBox13.Size = new Size(122, 17);
      this.checkBox13.TabIndex = 20;
      this.checkBox13.Text = "Show weapon icons";
      this.checkBox13.UseVisualStyleBackColor = true;
      this.checkBox12.AutoSize = true;
      this.checkBox12.Checked = true;
      this.checkBox12.CheckState = CheckState.Checked;
      this.checkBox12.Location = new Point(26, 154);
      this.checkBox12.Name = "checkBox12";
      this.checkBox12.Size = new Size(87, 17);
      this.checkBox12.TabIndex = 19;
      this.checkBox12.Text = "Show names";
      this.checkBox12.UseVisualStyleBackColor = true;
      this.label11.AutoSize = true;
      this.label11.Font = new Font("Microsoft Sans Serif", 8.25f, FontStyle.Regular, GraphicsUnit.Point, (byte) 0);
      this.label11.Location = new Point(24, 99);
      this.label11.Name = "label11";
      this.label11.Size = new Size(123, 13);
      this.label11.TabIndex = 18;
      this.label11.Text = "Health indicator position:";
      this.comboBox1.DisplayMember = "Left";
      this.comboBox1.FormattingEnabled = true;
      this.comboBox1.Items.AddRange(new object[4]
      {
        (object) "Left",
        (object) "Right",
        (object) "Top",
        (object) "Bottom"
      });
      this.comboBox1.Location = new Point(26, 116);
      this.comboBox1.Name = "comboBox1";
      this.comboBox1.Size = new Size(121, 21);
      this.comboBox1.TabIndex = 17;
      this.comboBox1.Text = "Left";
      this.checkBox11.AutoSize = true;
      this.checkBox11.Location = new Point(27, 69);
      this.checkBox11.Name = "checkBox11";
      this.checkBox11.Size = new Size(114, 17);
      this.checkBox11.TabIndex = 16;
      this.checkBox11.Text = "Color based health";
      this.checkBox11.UseVisualStyleBackColor = true;
      this.label10.AutoSize = true;
      this.label10.Font = new Font("Microsoft Sans Serif", 8.25f, FontStyle.Bold, GraphicsUnit.Point, (byte) 0);
      this.label10.Location = new Point(24, 24);
      this.label10.Name = "label10";
      this.label10.Size = new Size(51, 13);
      this.label10.TabIndex = 15;
      this.label10.Text = "General";
      this.checkBox10.AutoSize = true;
      this.checkBox10.Location = new Point(27, 46);
      this.checkBox10.Name = "checkBox10";
      this.checkBox10.Size = new Size(85, 17);
      this.checkBox10.TabIndex = 14;
      this.checkBox10.Text = "Show health";
      this.checkBox10.UseVisualStyleBackColor = true;
      this.tabPage4.Controls.Add((Control) this.checkBox18);
      this.tabPage4.Controls.Add((Control) this.checkBox17);
      this.tabPage4.Controls.Add((Control) this.label13);
      this.tabPage4.Controls.Add((Control) this.checkBox19);
      this.tabPage4.Controls.Add((Control) this.checkBox20);
      this.tabPage4.Location = new Point(4, 22);
      this.tabPage4.Name = "tabPage4";
      this.tabPage4.Padding = new Padding(3);
      this.tabPage4.Size = new Size(721, 408);
      this.tabPage4.TabIndex = 3;
      this.tabPage4.Text = "Radar";
      this.tabPage4.UseVisualStyleBackColor = true;
      this.checkBox18.AutoSize = true;
      this.checkBox18.Location = new Point(27, 115);
      this.checkBox18.Name = "checkBox18";
      this.checkBox18.Size = new Size(107, 17);
      this.checkBox18.TabIndex = 22;
      this.checkBox18.Text = "Show defuse kits";
      this.checkBox18.UseVisualStyleBackColor = true;
      this.checkBox17.AutoSize = true;
      this.checkBox17.Location = new Point(27, 92);
      this.checkBox17.Name = "checkBox17";
      this.checkBox17.Size = new Size(99, 17);
      this.checkBox17.TabIndex = 21;
      this.checkBox17.Text = "Show weapons";
      this.checkBox17.UseVisualStyleBackColor = true;
      this.label13.AutoSize = true;
      this.label13.Font = new Font("Microsoft Sans Serif", 8.25f, FontStyle.Bold, GraphicsUnit.Point, (byte) 0);
      this.label13.Location = new Point(24, 24);
      this.label13.Name = "label13";
      this.label13.Size = new Size(51, 13);
      this.label13.TabIndex = 20;
      this.label13.Text = "General";
      this.checkBox19.AutoSize = true;
      this.checkBox19.Location = new Point(27, 69);
      this.checkBox19.Name = "checkBox19";
      this.checkBox19.Size = new Size(105, 17);
      this.checkBox19.TabIndex = 18;
      this.checkBox19.Text = "Show bomb icon";
      this.checkBox19.UseVisualStyleBackColor = true;
      this.checkBox20.AutoSize = true;
      this.checkBox20.Location = new Point(27, 46);
      this.checkBox20.Name = "checkBox20";
      this.checkBox20.Size = new Size(140, 17);
      this.checkBox20.TabIndex = 17;
      this.checkBox20.Text = "Do not show teammates";
      this.checkBox20.UseVisualStyleBackColor = true;
      this.tabPage1.BackColor = Color.White;
      this.tabPage1.Controls.Add((Control) this.label6);
      this.tabPage1.Controls.Add((Control) this.textBox5);
      this.tabPage1.Controls.Add((Control) this.label5);
      this.tabPage1.Controls.Add((Control) this.textBox4);
      this.tabPage1.Controls.Add((Control) this.label4);
      this.tabPage1.Controls.Add((Control) this.textBox3);
      this.tabPage1.Controls.Add((Control) this.label3);
      this.tabPage1.Controls.Add((Control) this.textBox2);
      this.tabPage1.Controls.Add((Control) this.label2);
      this.tabPage1.Controls.Add((Control) this.textBox1);
      this.tabPage1.Location = new Point(4, 22);
      this.tabPage1.Name = "tabPage1";
      this.tabPage1.Padding = new Padding(3);
      this.tabPage1.Size = new Size(721, 408);
      this.tabPage1.TabIndex = 0;
      this.tabPage1.Text = "Hotkeys";
      this.label6.AutoSize = true;
      this.label6.Font = new Font("Microsoft Sans Serif", 11.25f, FontStyle.Regular, GraphicsUnit.Point, (byte) 0);
      this.label6.Location = new Point(25, 264);
      this.label6.Name = "label6";
      this.label6.Size = new Size(118, 18);
      this.label6.TabIndex = 9;
      this.label6.Text = "Toggle Wallhack";
      this.textBox5.CharacterCasing = CharacterCasing.Upper;
      this.textBox5.Location = new Point(28, 288);
      this.textBox5.Name = "textBox5";
      this.textBox5.Size = new Size(100, 20);
      this.textBox5.TabIndex = 8;
      this.textBox5.Text = "F9";
      this.label5.AutoSize = true;
      this.label5.Font = new Font("Microsoft Sans Serif", 11.25f, FontStyle.Regular, GraphicsUnit.Point, (byte) 0);
      this.label5.Location = new Point(25, 201);
      this.label5.Name = "label5";
      this.label5.Size = new Size(130, 18);
      this.label5.TabIndex = 7;
      this.label5.Text = "Toggle TriggerBot:";
      this.textBox4.CharacterCasing = CharacterCasing.Upper;
      this.textBox4.Location = new Point(28, 225);
      this.textBox4.Name = "textBox4";
      this.textBox4.Size = new Size(100, 20);
      this.textBox4.TabIndex = 6;
      this.textBox4.Text = "F8";
      this.label4.AutoSize = true;
      this.label4.Font = new Font("Microsoft Sans Serif", 11.25f, FontStyle.Regular, GraphicsUnit.Point, (byte) 0);
      this.label4.Location = new Point(25, 139);
      this.label4.Name = "label4";
      this.label4.Size = new Size(136, 18);
      this.label4.TabIndex = 5;
      this.label4.Text = "Toggle RadarHack:";
      this.textBox3.CharacterCasing = CharacterCasing.Upper;
      this.textBox3.Location = new Point(28, 163);
      this.textBox3.Name = "textBox3";
      this.textBox3.Size = new Size(100, 20);
      this.textBox3.TabIndex = 4;
      this.textBox3.Text = "F7";
      this.label3.AutoSize = true;
      this.label3.Font = new Font("Microsoft Sans Serif", 11.25f, FontStyle.Regular, GraphicsUnit.Point, (byte) 0);
      this.label3.Location = new Point(25, 78);
      this.label3.Name = "label3";
      this.label3.Size = new Size(107, 18);
      this.label3.TabIndex = 3;
      this.label3.Text = "Toggle Aimbot:";
      this.textBox2.CharacterCasing = CharacterCasing.Upper;
      this.textBox2.Location = new Point(28, 102);
      this.textBox2.Name = "textBox2";
      this.textBox2.Size = new Size(100, 20);
      this.textBox2.TabIndex = 2;
      this.textBox2.Text = "F6";
      this.label2.AutoSize = true;
      this.label2.Font = new Font("Microsoft Sans Serif", 11.25f, FontStyle.Regular, GraphicsUnit.Point, (byte) 0);
      this.label2.Location = new Point(25, 21);
      this.label2.Name = "label2";
      this.label2.Size = new Size(156, 18);
      this.label2.TabIndex = 1;
      this.label2.Text = "Show/Hide UI Overlay:";
      this.textBox1.CharacterCasing = CharacterCasing.Upper;
      this.textBox1.Location = new Point(28, 45);
      this.textBox1.Name = "textBox1";
      this.textBox1.Size = new Size(100, 20);
      this.textBox1.TabIndex = 0;
      this.textBox1.Text = "F5";
      this.pictureBox1.BackColor = Color.White;
      this.pictureBox1.Image = (Image) componentResourceManager.GetObject("pictureBox1.Image");
      this.pictureBox1.Location = new Point(406, 142);
      this.pictureBox1.Name = "pictureBox1";
      this.pictureBox1.Size = new Size(330, 337);
      this.pictureBox1.SizeMode = PictureBoxSizeMode.AutoSize;
      this.pictureBox1.TabIndex = 20;
      this.pictureBox1.TabStop = false;
      this.button1.FlatStyle = FlatStyle.Popup;
      this.button1.Font = new Font("Impact", 14.25f, FontStyle.Regular, GraphicsUnit.Point, (byte) 0);
      this.button1.ForeColor = Color.Red;
      this.button1.Location = new Point(525, 410);
      this.button1.Name = "button1";
      this.button1.Size = new Size(177, 48);
      this.button1.TabIndex = 21;
      this.button1.Text = "Inject Config";
      this.button1.UseVisualStyleBackColor = true;
      this.button1.Click += new EventHandler(this.button1_Click);
      this.pictureBox2.BackColor = Color.Black;
      this.pictureBox2.Image = (Image) componentResourceManager.GetObject("pictureBox2.Image");
      this.pictureBox2.InitialImage = (Image) null;
      this.pictureBox2.Location = new Point(12, 6);
      this.pictureBox2.Name = "pictureBox2";
      this.pictureBox2.Size = new Size(16, 16);
      this.pictureBox2.SizeMode = PictureBoxSizeMode.AutoSize;
      this.pictureBox2.TabIndex = 22;
      this.pictureBox2.TabStop = false;
      this.AutoScaleDimensions = new SizeF(6f, 13f);
      this.AutoScaleMode = AutoScaleMode.Font;
      this.BackColor = SystemColors.Control;
      this.BackgroundImageLayout = ImageLayout.Stretch;
      this.ClientSize = new Size(752, 493);
      this.Controls.Add((Control) this.pictureBox2);
      this.Controls.Add((Control) this.button1);
      this.Controls.Add((Control) this.pictureBox1);
      this.Controls.Add((Control) this.tabControl1);
      this.Controls.Add((Control) this.label1);
      this.FormBorderStyle = FormBorderStyle.Fixed3D;
      this.Icon = (Icon) componentResourceManager.GetObject("$this.Icon");
      this.MaximizeBox = false;
      this.Name = nameof (FakeCheatForm);
      this.StartPosition = FormStartPosition.CenterScreen;
      this.Text = "CSGO Reaper";
      this.tabControl1.ResumeLayout(false);
      this.tabPage2.ResumeLayout(false);
      this.tabPage2.PerformLayout();
      this.tabPage5.ResumeLayout(false);
      this.tabPage5.PerformLayout();
      this.tabPage3.ResumeLayout(false);
      this.tabPage3.PerformLayout();
      this.tabPage4.ResumeLayout(false);
      this.tabPage4.PerformLayout();
      this.tabPage1.ResumeLayout(false);
      this.tabPage1.PerformLayout();
      ((ISupportInitialize) this.pictureBox1).EndInit();
      ((ISupportInitialize) this.pictureBox2).EndInit();
      this.ResumeLayout(false);
      this.PerformLayout();
    }
  }
}
