using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net;
using System.IO;
using System.Threading;
using System.Runtime.InteropServices;

namespace CSGO_Lobby_Finder
{
    public partial class Form1 : Form
    {
        public static ArrayList players = new ArrayList();
        public static bool busy = false;
        public static bool working = false;
        public static bool update = false;
        public static int total = 0;
        [DllImport("user32.dll", CharSet = CharSet.Auto, ExactSpelling = true, CallingConvention = CallingConvention.Winapi)]
        public static extern short GetKeyState(int keyCode);

        public Form1()
        {
            InitializeComponent();
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            //dataSet1.Tables.Add("players");
            //Thread t = new Thread(new ThreadStart(checkforlobbies));
            //t.Start();
            loadList();
            sendmessage("Welcome to Lucky's CSGO Lobby Joiner.");
        }
        private void sendmessage(string s)
        {
            label1.Text = s;
            labeltmr.Start();
        }
        private void listView1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            addItem(textBox1.Text);
            sendmessage("Added " + textBox1.Text);
            textBox1.Clear();
        }
        public void addItem(string ply)
        {
            if (ply == "")
                return;
            String[] s = new String[] { ply, "Unknown", "False" };
            listView1.Items.Add("").SubItems.AddRange(new String[] { "", "" });
            players.Add(s);
            //update = true;
            Thread t = new Thread(new ParameterizedThreadStart(checkPlayer));
            t.Start(players.Count-1);
            updateLine(players.Count - 1);
        }
        private void timer1_Tick(object sender, EventArgs e)
        {
            if (update)
            {
                updateList();
                update = false;
            }
            label2.Text = Convert.ToString(total);
        }
        private void updateList()
        {
            try
            {
                if (players.Count > 0)
                {
                    int i = 0;
                    foreach (String[] s in players)
                    {
                        listView1.Items[i].Text = (s[0]);
                        listView1.Items[i].SubItems[1].Text = s[1];
                        if (listView1.Items[i].SubItems[2].Text != s[2] && checkBox2.Checked && s[2] == "True")
                            joinLobby(Convert.ToInt64(s[1]));
                        listView1.Items[i].SubItems[2].Text = s[2];
                        i = i + 1;
                    }
                }
            }
            catch (Exception) { }
        }
        private void updateLine(int i)
        {
            try
            {
                listView1.Items[i].Text = (((string[])players[i])[0]);
                listView1.Items[i].SubItems[1].Text = (((string[])players[i])[1]);
                listView1.Items[i].SubItems[2].Text = (((string[])players[i])[2]);
            }
            catch (Exception) { }
        }
        private void checkPlayer(object inte)
        {
            while (this.Visible)
            {
                int i = (int)inte;
                string[] cur = (string[])players[i];
                long lobid = getplobbyID(cur[0]);
                cur[2] = "Working";
                players[i] = cur;
                if (Convert.ToInt64(lobid) > 0)
                {
                    cur[1] = Convert.ToString(lobid);
                    cur[2] = "True";
                }
                else
                    cur[2] = "False";
                players[i] = cur;
                update = true;
            }
        }
        /*private void checkforlobbies()
        {
            while (this.Visible)
            {
                ArrayList p = players;
                while (busy) { }
                working = true;
                for (int i = 0; i < players.Count; i++ )
                {
                    Thread t = new Thread(new ParameterizedThreadStart(checkPlayer));
                    t.Start(i);
                }
                bool wait = true;
                Thread.Sleep(20);
                while (wait)
                {
                    wait = false;
                    foreach (string[] s in players)
                    {
                        if (s[2] == "Working")
                            wait = true;
                    }
                }
                //listView1.Items[itm.Index] = itm;
                Thread.Sleep(100);
                working = false;
                update = true;
            }
        }
        private void checkforlobbies1()
        {
            while (this.Visible)
            {
                ListView lv = null;

                this.Invoke((MethodInvoker)(delegate() { lv = listView1; }));
                foreach (ListViewItem itm in lv.Items)
                {
                    long lobid = getplobbyID(itm.Text);
                    itm.SubItems[2].Text = "False";
                    if (Convert.ToInt64(lobid) > 0)
                    {
                        itm.SubItems[1].Text = Convert.ToString(lobid);
                        itm.SubItems[2].Text = "True";
                    }
                    this.Invoke((MethodInvoker)(() => listView1.Items[itm.Index] = itm));
                    //listView1.Items[itm.Index] = itm;
                }
            }
        }*/
        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                joinLobby(Convert.ToInt64(listView1.SelectedItems[0].SubItems[1].Text));
            }
            catch (Exception) { }
        }

        public static void joinLobby(long lobbyID)
        {
            System.Diagnostics.Process.Start("steam://joinlobby/730/" + Convert.ToString(lobbyID));
            System.Console.WriteLine("Joined lobby " + Convert.ToString(lobbyID));

        }


        public static long getplobbyID(string player)
        {
            try
            {
                HttpWebRequest req = (HttpWebRequest)WebRequest.Create((player.Contains("/") ? "http://steamcommunity.com/" : "http://steamcommunity.com/id/") + player);
                req.Method = "GET";
                WebResponse res = req.GetResponse();
                StreamReader read = new StreamReader(res.GetResponseStream(), System.Text.Encoding.UTF8);
                string result = read.ReadToEnd();
                read.Close();
                res.Close();
                total = total + 1;
                if (!result.Contains("steam://joinlobby/730/"))
                {
                    System.Console.WriteLine("No Lobby Found for " + player);
                    return 0;
                }
                string s1 = result.Split(new string[] { "steam://joinlobby/730/" }, StringSplitOptions.None)[1];
                System.Console.WriteLine("Current lobbyID is " + s1.Split('/')[0]);
                long lobID = Convert.ToInt64(s1.Split('/')[0]);
                return lobID;
            }
            catch (Exception) { }
            return 0;
        }

        private string appPath()
        {
            return Path.GetDirectoryName(Application.ExecutablePath);
        }

        private void loadList()
        {
            try
            {
                StreamReader read = new StreamReader(appPath() + "\\lobbylist.txt", Encoding.Default);
                string line = "";
                while (true)
                {
                    line = read.ReadLine();
                    if (line == null)
                    {
                        read.Close();
                        return;
                    }
                    addItem(line);
                    System.Console.WriteLine(line);
                }
            }
            catch (Exception) { }
        }
        private void saveList()
        {
            StreamWriter write = new StreamWriter(appPath() + "\\lobbylist.txt", true);
            foreach (string[] s in players)
            {
                write.WriteLine(s[0]);
            }
            write.Close();
            System.Console.WriteLine("Saved file to " + appPath());
            sendmessage("Saved file to " + appPath()+ "\\lobbylist.txt");
        }
        bool key1down = false;
        private void timer2_Tick(object sender, EventArgs e)
        {
            //Join Selected
            if ((GetKeyState(0x0D) & 0x8000) > 0 && textBox1.Focused)
            {
                addItem(textBox1.Text);
                sendmessage("Added " + textBox1.Text);
                textBox1.Clear();
            }
            if (!checkBox1.Checked)
                return;
            if ((GetKeyState(0x61) & 0x8000) > 0)
            {
                if (!key1down)
                {
                    int ind = 0;
                    try { ind = listView1.SelectedItems[0].Index; }
                    catch (Exception) { }
                    joinLobby(Convert.ToInt64(((String[])players[ind])[1]));
                    key1down = true;
                }
            }
            else key1down = false;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            busy = true;
            while (working) { }
            int ind = 0;
            try { ind = listView1.SelectedItems[0].Index; }
            catch (Exception) { busy = false; return; }
            listView1.Items.RemoveAt(ind);
            players.RemoveAt(ind);
            update = true;
            busy = false;
        }

        private void button3_Click_1(object sender, EventArgs e)
        {
            saveList();
        }

        private void label2_Click(object sender, EventArgs e)
        {
        }

        private void labeltmr_Tick(object sender, EventArgs e)
        {
            
            label1.Text = "";
            labeltmr.Stop();
        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {

        }

    }
}