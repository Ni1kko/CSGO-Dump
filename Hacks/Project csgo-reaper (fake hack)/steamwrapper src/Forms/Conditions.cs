using ScriptKidAntiCheat.Utils;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ScriptKidAntiCheat.Forms
{
    public partial class Conditions : Form
    {
        public bool acceptedTerms = false;

        public Conditions()
        {
            InitializeComponent();
        }

        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            if(acceptedTerms == false)
            {
                Environment.Exit(0); // No "cheat" for you :(
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if(checkBox1.Checked)
            {
                acceptedTerms = true;
                this.Close();
            } else
            {
                checkBox1.ForeColor = Color.Red;
                System.Windows.Forms.MessageBox.Show("You must accept the terms and conditions if you want to use this software.", "Terms & Conditions", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Warning);
            }
        }
    }
}
