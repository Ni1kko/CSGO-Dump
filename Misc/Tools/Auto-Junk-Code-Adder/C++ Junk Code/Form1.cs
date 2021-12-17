using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace C___Junk_Code
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        int count;
        int runt;

        private void Form1_Load(object sender, EventArgs e)
        {
            string[] lines = System.IO.File.ReadAllLines(@"C:\txt of all the files");
            listBox1.Items.AddRange(lines);
            label1.Text = "Count: " + count;
            label2.Text = "Total: " + listBox1.Items.Count;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            timer1.Start();
        }

        string file;
        string junk;

        public void Write()
        {
            file = listBox1.Items[count].ToString();
            junk = richTextBox1.Text;

            string existing = File.ReadAllText(file);
            string createText = existing + Environment.NewLine + junk;

            File.WriteAllText(file, createText + Environment.NewLine);

            count = count + 1;
            label1.Text = "Count: " + count;
        }

        private void webBrowser1_DocumentCompleted(object sender, WebBrowserDocumentCompletedEventArgs e)
        {
            richTextBox1.Text = webBrowser1.Document.Body.InnerText;
            Write();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (count == listBox1.Items.Count)
            {
                count = 0;
                runt = runt + 1; // times to run
                label3.Text = "Passes: " + runt;

                if (runt == 1)
                {
                    timer1.Stop();
                    MessageBox.Show("Complete", "C++ Junk Adder");
                }
            }
            else
            {
                webBrowser1.Navigate("http://junkcode.gehaxelt.in/");
            }
        }
    }
}
