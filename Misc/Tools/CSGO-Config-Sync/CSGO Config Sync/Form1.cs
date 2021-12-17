using Microsoft.Win32;
using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Windows.Forms;

namespace CSGO_Config_Sync
{
    public partial class Main : Form
    {
        public Main()
        {
            InitializeComponent();
            handleSteamPath();
            handleSteamID();
            this.list_steamUsernames.Visible = false;
            //populateList();
        }

        private void TextBox1_TextChanged(object sender, EventArgs e)
        {
            File.WriteAllText("steampath.txt", this.txtbox_steampath.Text.TrimStart().TrimEnd());
        }

        private void RegistrySteamPath()
        {
            RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\Valve\\Steam");
            string steamPath = key.GetValue("SteamPath").ToString();
            this.txtbox_steampath.Text = steamPath;
        }

        private void txt_steamID_TextChanged(object sender, EventArgs e)
        {
            if (this.txt_steamID.Text.Contains("U:1:"))
            {
                this.txt_steamID.Text = this.txt_steamID.Text.Replace("U:1:", "");
            }
            File.WriteAllText("steamid.txt", this.txt_steamID.Text.TrimStart().TrimEnd());
        }

        private void handleSteamPath()
        {
            try
            {
                if (File.Exists("steampath.txt"))
                {
                    string contents = File.ReadAllText("steampath.txt");
                    this.txtbox_steampath.Text = contents;

                    //Fixes an issue where the file exists but is empty
                    if (this.txtbox_steampath.Text.Trim() == null)
                    {
                        RegistrySteamPath();
                    }
                }
                else
                {
                    File.Create("steampath.txt");
                }

                if (this.txtbox_steampath.Text.EndsWith("\\") || this.txtbox_steampath.Text.EndsWith("/"))
                {
                    this.txtbox_steampath.Text = this.txtbox_steampath.Text.Remove(this.txtbox_steampath.Text.Length - 1);
                }
            }
            catch
            {
                this.txtbox_steampath.Text = "c:/program files (x86)/steam";
            }
        }

        private void handleSteamID()
        {
            try
            {
                if (File.Exists("steamid.txt"))
                {
                    string contents = File.ReadAllText("steamid.txt");
                    this.txt_steamID.Text = contents;
                } else
                {
                    File.Create("steamid.txt");
                }

                if (this.txt_steamID.Text.Contains("U:1:"))
                {
                    this.txt_steamID.Text = this.txt_steamID.Text.Replace("U:1:", "");
                }
            } catch
            {
                updateStatus("Please try again.", true);
            }
        }

        private void btn_sync_Click(object sender, EventArgs e)
        {
            try
            {
                updateStatus("Syncing config.");
                var diSource = new DirectoryInfo(txtbox_steampath.Text + "/userdata/" + txt_steamID.Text + "/730/");
                var directories = Directory.GetDirectories(this.txtbox_steampath.Text + "/" + "userdata/");
                foreach (string dir in directories)
                {
                    Debug.WriteLine(dir);
                    var diTarget = new DirectoryInfo(dir + "/730/");
                    CopyAll(diSource, diTarget);
                }
                Debug.WriteLine("File copy complete.");
                updateStatus("Config sync complete!");
            } catch
            {
                updateStatus("Unable to update configs, admin access needed?", true);
            }
        }

        private void updateStatus(string newStatus, bool error = false)
        {
            if (error == true)
            {
                this.lbl_status.Text = "Status: Error, " + newStatus;
            } else
            {
                this.lbl_status.Text = "Status: " + newStatus;
            }
        }

        public static void CopyAll(DirectoryInfo source, DirectoryInfo target)
        {
            Directory.CreateDirectory(target.FullName);

            // Copy each file into the new directory.
            foreach (FileInfo fi in source.GetFiles())
            {
                Debug.WriteLine(@"Copying {0}\{1}", target.FullName, fi.Name);
                try
                {
                    fi.CopyTo(Path.Combine(target.FullName, fi.Name), true);
                }
                catch
                {
                    Debug.WriteLine("File Skipped: " + fi.Name);
                }
            }

            // Copy each subdirectory using recursion.
            foreach (DirectoryInfo diSourceSubDir in source.GetDirectories())
            {
                DirectoryInfo nextTargetSubDir =
                    target.CreateSubdirectory(diSourceSubDir.Name);
                CopyAll(diSourceSubDir, nextTargetSubDir);
            }
        }

        private void populateList()
        {   
            var directories = Directory.GetDirectories(this.txtbox_steampath.Text + "/" + "userdata/");
            foreach (string dir in directories)
            {
                var username = File.ReadLines(dir + "/config/localconfig.vdf").Skip(23).Take(1).First().Replace("\"", "").Replace("PersonaName", "").Trim();
                this.list_steamUsernames.Items.Add(username);
            }
        }

        private void list_steamUsernames_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}
