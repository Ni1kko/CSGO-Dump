using HWIDGrabber;
using ManualMapInjection.Injection;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Howl_Loader
{
    public partial class Form1 : Form
    {


        #region instructions
        // 95% of this is setup and ready to go, you only need to change a few text regions
        // To start off, please change the string RegistryName to whatever you want.
        // Then make a folder in your C drive named "Bytes" so that it is C:\Bytes
        // Then place your DLL into that folder, name it "c.dll"
        // Now open the Encrypter.exe
        // In the second box enter "C:\\Bytes\\c.dll" and in the last box enter "C:\\Bytes\\c.txt"
        // Now click the bottom button. Now upload that .txt to your website
        // Now navigate to the .txt on your website and copy the link
        // In the first textBox enter the full link from http all the way to txt and now hit encrypt
        // Copy from "new byte[] { ALL THE WAY TO }
        // Now change the byte[] urlBytes from new byte to the semi colon to what you got from encrypter.exe
        // Your cheat is now ready to go!
        
        // Notes about Josh and HowlWare
        // HowlWare is a paste.
        // Josh is known by people in the cheat for scamming.
        // Josh sends lackies after you if you do anything.
        // He lied to PayPal in the refund attempt
        // He treats his devs like shit
        // He is an annoying prick.
        // I advise you to not use his cheat or work with him
        #endregion

        // Your cheat name to show in the registry
        string RegistryName = "YOURCHEATNAME";

        // URL Bytes to your cheat.txt
        byte[] urlBytes = new byte[] { 38, 240, 197, 12, 234, 154, 252, 111, 158, 10, 50, 188, 85, 94, 32, 241, 52, 170, 36, 236, 184, 199, 238, 78, 95, 129, 228, 151, 41, 25, 198, 20, };

        public Form1()
        {
            // Removing this line will break everything
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // Stupid line for the password text box, makes it so when text exists it's hidden
            txtPassword.isPassword = true;

            // Checking registry info
            Microsoft.Win32.RegistryKey key;
            key = Microsoft.Win32.Registry.CurrentUser.CreateSubKey(RegistryName);

            // If the entry doesn't exist, it will ignore it
            if (key.GetValue("Checked") == null || key.GetValue("Checked").ToString() == null)
            {

            }
            // If the value exists AND equals true, it will automatically login the user
            else if (key.GetValue("Checked").ToString() == "true")
            {
                txtUsername.Text = key.GetValue("Username").ToString();
                txtPassword.Text = key.GetValue("Password").ToString();
                bunifuSwitch1.Value = true;
                Login(txtUsername.Text, txtPassword.Text);
            }

            // Closes the registry key, VERY IMPORTANT THAT THIS STAYS HERE
            key.Close();

            // This adds the HWID/GUID into a text box for logged in users
            bunifuMaterialTextbox1.Text = HWDI.GetMachineGuid();
        }

        // This is the exit button, just closes the program
        private void bunifuImageButton2_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        // This is for minimizing the sidebar
        private void logo_Click(object sender, EventArgs e)
        {
            // Hiding and showing specific images
            logo.Visible = false;
            pictureBox2.Visible = true;

            // Setting the size from large to minimized
            if (sidemenu.Width == 50)
            {
                // The animation stuff
                sidemenu.Visible = false;
                sidemenu.Width = 251;
                PanelAnimation.ShowSync(sidemenu);
                LogoAnimation.ShowSync(logo);
            }
            // Redundancy that will NEVER be called, but is here incase of a weird glitch
            else
            {
                // The animation stuff
                LogoAnimation.Hide(logo);
                sidemenu.Visible = false;
                sidemenu.Width = 50;
                PanelAnimation.ShowSync(sidemenu);
            }
        }

        // This is for returning the sidebar
        private void pictureBox2_Click(object sender, EventArgs e)
        {
            // Hiding and showing specific images
            logo.Visible = true;
            pictureBox2.Visible = false;

            // Setting the size from minimized view to large
            if (sidemenu.Width == 50)
            {
                // The animation stuff
                sidemenu.Visible = false;
                sidemenu.Width = 251;
                PanelAnimation.ShowSync(sidemenu);
                LogoAnimation.ShowSync(logo);
            }
            // Redundancy that will NEVER be called, but is here incase of a weird glitch
            else
            {
                // The animation stuff
                LogoAnimation.Hide(logo);
                sidemenu.Visible = false;
                sidemenu.Width = 50;
                PanelAnimation.ShowSync(sidemenu);
            }
        }

        // Ignore this call, I clicked the element by accident, removing this will break the program
        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

        // This is the login button
        private void bunifuFlatButton6_Click(object sender, EventArgs e)
        {
            // This sends the strings to the call below
            Login(txtUsername.Text, txtPassword.Text);
        }

        // This checks the login info compared to your website
        private void Login(string username, string password)
        {
            // Fetching the HWID
            string hwid = HWDI.GetMachineGuid();

            // Generating token for anti-php spoofing
            var chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
            var stringChars = new char[5];
            var random = new Random();

            // Actual generation ofthe token
            for (int i = 0; i < stringChars.Length; i++)
            {
                stringChars[i] = chars[random.Next(chars.Length)];
            }

            // The real token
            string token = new String(stringChars);

            // Checking login info
            var web = new WebClient();
            var result = web.DownloadString("http://krystal.zone/check.php?username=" + username + "&password=" + password + "&hwid=" + hwid + "&token=" + token);
            if (result != null && result.Contains("password: true"))
            {
                // Usergroup checks. 4 = admin, 8 and 9 = the first 2 custom groups you create
                if (result.Contains("4") || result.Contains("8") || result.Contains("9"))
                {
                    if (result.Contains("hwid: true"))
                    {
                        if (result.Contains("token: " + token))
                        {
                            // If they pass the login test, they get sent to this void.
                            loggedin();
                        }
                        else
                        {
                            // Some DNS spoofing measures
                            MessageBox.Show("ERROR: DNS Spoofing Detected...");
                        }
                    }
                    else
                    {
                        // If the HWID/GUID doesn't match
                        MessageBox.Show("HWID Incorrect");
                    }
                }
                else
                {
                    // Their group isn't in the allowed ones
                    MessageBox.Show("Incorrect user groups");
                }
            }
            // This is called when the user doesn't exist, leave the message like this
            else if (result.Contains("password: false"))
            {
                MessageBox.Show("Username or Password incorrect");
            }
        }

        // The exit button
        private void bunifuFlatButton7_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        // When a user's login info is correct
        private void loggedin()
        {
            // Starting the key reading
            Microsoft.Win32.RegistryKey key;
            key = Microsoft.Win32.Registry.CurrentUser.CreateSubKey(RegistryName);

            // Checking if the auto inject value exists
            if (key.GetValue("AutoInject") == null || key.GetValue("Checked").ToString() == null)
            {

            }
            else if (key.GetValue("AutoInject").ToString() == "true")
            {
                // Starts the injection process
                bunifuSwitch3.Value = true;
                InjectCheat();

            }
            else if (key.GetValue("AutoInject").ToString() == "false")
            {
                // Makes the auto inject button false
                bunifuSwitch3.Value = false;
            }

            // Saving info for auto login IF they logged in successfully
            if (bunifuSwitch1.Value == true)
            {
                key.SetValue("Username", txtUsername.Text);
                key.SetValue("Password", txtPassword.Text);
                key.SetValue("Checked", "true");

                bunifuSwitch2.Value = true;
            }
            // This still saves the usernames and passwords BUT will not auto login
            else if (bunifuSwitch1.Value == false)
            {
                key.SetValue("Username", txtUsername.Text);
                key.SetValue("Password", txtPassword.Text);
                key.SetValue("Checked", "false");

                bunifuSwitch2.Value = false;
            }

            // Closing the key
            key.Close();

            if (bunifuSwitch3.Value == true)
            {
                bunifuSwitch3.Value = true;

                // Run injection stuff here
            }
            else if (bunifuSwitch3.Value == false)
            {
                bunifuSwitch3.Value = false;
            }

            // Sidebar Buttons
            bunifuFlatButton3.Visible = false;
            bunifuFlatButton1.Visible = true;
            bunifuFlatButton2.Visible = true;
            bunifuFlatButton4.Visible = true;
            bunifuFlatButton5.Visible = true;
            bunifuFlatButton6.Visible = true;
            bunifuFlatButton3.selected = false;
            bunifuFlatButton4.selected = true;

            // Login elements
            panel1.Visible = false;
            panel3.Visible = true;
        }

        // Doesn't work, can't be removed, it will break the program
        private void txtPassword_Click(object sender, EventArgs e)
        {
            txtPassword.ResetText();
        }

        // Doesn't work, can't be removed, it will break the program
        private void txtPassword_OnValueChanged(object sender, EventArgs e)
        {

        }

        // Closes visible tabs then opens new ones when you click a button
        private void CloseTabs()
        {
            panel3.Visible = false;
            panel4.Visible = false;
        }

        // Load Cheat button
        private void bunifuFlatButton4_Click(object sender, EventArgs e)
        {
            CloseTabs();
            panel3.Visible = true;
        }

        // Settings button
        private void bunifuFlatButton5_Click(object sender, EventArgs e)
        {
            CloseTabs();
            panel4.Visible = true;
        }

        // Forums button
        private void bunifuFlatButton1_Click(object sender, EventArgs e)
        {
            Process.Start("https://emberservers.net");
        }

        // Discord button
        private void bunifuFlatButton2_Click(object sender, EventArgs e)
        {
            Process.Start("https://thaisen.pw");
        }

        // For saving the HWID to the clipboard
        private void bunifuFlatButton10_Click(object sender, EventArgs e)
        {
            Clipboard.SetText(HWDI.GetMachineGuid());
        }

        // For disabling auto logins from the user settings.
        private void bunifuSwitch2_Click(object sender, EventArgs e)
        {
            // Checking value of the switch
            if (bunifuSwitch2.Value == true)
            {
                // Writing to the registry entry
                Microsoft.Win32.RegistryKey key;
                key = Microsoft.Win32.Registry.CurrentUser.CreateSubKey(RegistryName);
                key.SetValue("Checked", "true");
                key.Close();
            }
            // Checking value of the switch
            else if (bunifuSwitch2.Value == false)
            {
                // Writing to the registry entry
                Microsoft.Win32.RegistryKey key;
                key = Microsoft.Win32.Registry.CurrentUser.CreateSubKey(RegistryName);
                key.SetValue("Checked", "false");
                key.Close();
            }
        }

        private void bunifuSwitch3_Click(object sender, EventArgs e)
        {
            // Checking value of the switch
            if (bunifuSwitch3.Value == true)
            {
                // Writing to the registry entry
                Microsoft.Win32.RegistryKey key;
                key = Microsoft.Win32.Registry.CurrentUser.CreateSubKey(RegistryName);
                key.SetValue("AutoInject", "true");
                key.Close();
            }
            // Checking value of the switch
            else if (bunifuSwitch3.Value == false)
            {
                // Writing to the registry entry
                Microsoft.Win32.RegistryKey key;
                key = Microsoft.Win32.Registry.CurrentUser.CreateSubKey(RegistryName);
                key.SetValue("AutoInject", "false");
                key.Close();
            }
        }

        // Clears all data from the registry
        private void bunifuFlatButton9_Click(object sender, EventArgs e)
        {
            // Writing to the registry entry
            Microsoft.Win32.RegistryKey key;
            key = Microsoft.Win32.Registry.CurrentUser.CreateSubKey(RegistryName);

            // Resetting the data
            key.SetValue("Username", string.Empty);
            key.SetValue("Password", string.Empty);
            key.SetValue("Checked", "false");
            key.SetValue("AutoInject", "false");

            // Closing the key
            key.Close();
        }

        // Starting the injections
        private void InjectCheat()
        {
            var name = "csgo";
            var target = Process.GetProcessesByName(name).FirstOrDefault();

            if (target == null)
            {
                timer1.Start();
            }
            else
            {
                var decryptedUrlBytes = Encoding.Default.GetString(AESDecryptBytes(urlBytes));
                string endFilePath = "C:\\Users\\" + Environment.UserName + "\\AppData\\cheat.dll";
                DecryptFile(decryptedUrlBytes, endFilePath);
            }
        }

        // If the process doesn't exist
        private void timer1_Tick(object sender, EventArgs e)
        {
            var name = "csgo";
            var target = Process.GetProcessesByName(name).FirstOrDefault();

            if (target == null)
            {
                
            }
            else
            {
                timer1.Stop();
                var decryptedUrlBytes = Encoding.Default.GetString(AESDecryptBytes(urlBytes));
                string endFilePath = "C:\\Users\\" + Environment.UserName + "\\AppData\\cheat.dll";
                DecryptFile(decryptedUrlBytes, endFilePath);
            }
        }

        static readonly byte[] PasswordHashByte = new byte[] { 74, 56, 106, 103, 116, 55, 56, 57, 74, 72, 104, 55, 84, 56, 117, 106, 56, 84, };
        static readonly byte[] SaltKeyByte = new byte[] { 97, 115, 100, 114, 103, 52, 53, 114, 51, 52, 114, 102, 51, 114, 103, 114, };

        public static byte[] AESDecryptBytes(byte[] cryptBytes)
        {
            byte[] clearBytes = null;

            // create a key from the password and salt, use 32K iterations
            var key = new Rfc2898DeriveBytes(PasswordHashByte, SaltKeyByte, 32768);

            using (Aes aes = new AesManaged())
            {
                // Loader by Thaisen#1337
                aes.KeySize = 256;
                aes.Key = key.GetBytes(aes.KeySize / 8);
                aes.IV = key.GetBytes(aes.BlockSize / 8);

                using (MemoryStream ms = new MemoryStream())
                {
                    using (CryptoStream cs = new CryptoStream(ms, aes.CreateDecryptor(), CryptoStreamMode.Write))
                    {
                        cs.Write(cryptBytes, 0, cryptBytes.Length);
                        cs.Close();
                    }
                    clearBytes = ms.ToArray();
                }
            }
            return clearBytes;
        }
        // now put tht txt file on the site
        static readonly string PasswordHash = "v*^FC%Fb9d237f97((";
        static readonly string SaltKey = "&^*Yeu^4gh)54eh$%&";
        static readonly string VIKey = "&$hf#&*hf!@6e$^hv(%";

        public static string DecryptString(string encryptedText)
        {
            byte[] cipherTextBytes = Convert.FromBase64String(encryptedText);
            byte[] keyBytes = new Rfc2898DeriveBytes(PasswordHash, Encoding.ASCII.GetBytes(SaltKey)).GetBytes(256 / 8);
            var symmetricKey = new RijndaelManaged() { Mode = CipherMode.CBC, Padding = PaddingMode.None };

            var decryptor = symmetricKey.CreateDecryptor(keyBytes, Encoding.ASCII.GetBytes(VIKey));
            var memoryStream = new MemoryStream(cipherTextBytes);
            var cryptoStream = new CryptoStream(memoryStream, decryptor, CryptoStreamMode.Read);
            byte[] plainTextBytes = new byte[cipherTextBytes.Length];

            int decryptedByteCount = cryptoStream.Read(plainTextBytes, 0, plainTextBytes.Length);
            memoryStream.Close();
            cryptoStream.Close();
            return Encoding.UTF8.GetString(plainTextBytes, 0, decryptedByteCount).TrimEnd("\0".ToCharArray());
        }

        public static void DecryptFile(string url, string file)
        {
            WebClient web = new WebClient();
            byte[] bytesDecrypted = System.Convert.FromBase64String(DecryptString(web.DownloadString(url)));

            var name = "csgo";
            var target = Process.GetProcessesByName(name).FirstOrDefault();
            var injector = new ManualMapInjector(target) { AsyncInjection = true };

            AutoClosingMessageBox.Show($"hmodule = 0x{injector.Inject(bytesDecrypted).ToInt64():x8}", "Howl Ware", 1);

            Application.Exit();
        }

        private void bunifuFlatButton8_Click(object sender, EventArgs e)
        {
            InjectCheat();
        }
    }
}
