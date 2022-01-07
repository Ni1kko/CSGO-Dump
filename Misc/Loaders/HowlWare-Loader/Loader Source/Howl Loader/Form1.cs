using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net;
using System.Security.Cryptography;
using System.Text;
using System.Windows.Forms;
using Microsoft.Win32;
using ManualMapInjection.Injection;

namespace Howl_Loader
{
    internal partial class Form1 : Form
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
        #endregion

        // Your cheat name to show in the registry
        private protected readonly string RegistryName = "Spatial";
        private protected readonly string endpoint = "http://wackyhacky.net";
        private protected bool devmode { get => true; }

       // URL Bytes to your cheat.txt
       private protected readonly byte[] urlBytes = new byte[] { 251, 183, 164, 52, 136, 217, 139, 225, 137, 252, 196, 196, 150, 75, 168, 57, 163, 5, 208, 145, 159, 28, 212, 54, 111, 252, 168, 147, 148, 18, 217, 24, 199, 99, 128, 238, 213, 251, 159, 54, 206, 115, 240, 0, 111, 192, 28, 59, 237, 121, 185, 166, 190, 82, 207, 109, 48, 227, 147, 87, 236, 121, 165, 234, 236, 49, 82, 100, 242, 4, 46, 33, 232, 158, 225, 254, 3, 48, 27, 223, 222, 53, 214, 167, 193, 96, 90, 79, 113, 102, 119, 179, 124, 121, 42, 113, 71, 78, 212, 223, 140, 107, 146, 29, 59, 231, 90, 225, 122, 47, 86, 5, };
        
        ///////////////////////////////
        private protected readonly byte[] PasswordHashByte = new byte[] { 74, 56, 106, 103, 116, 55, 56, 57, 74, 72, 104, 55, 84, 56, 117, 106, 56, 84, };
        private protected readonly byte[] SaltKeyByte = new byte[] { 97, 115, 100, 114, 103, 52, 53, 114, 51, 52, 114, 102, 51, 114, 103, 114, };
        private protected readonly string PasswordHash = "v*^FC%Fb9d237f97((";
        private protected readonly string SaltKey = "&^*Yeu^4gh)54eh$%&";
        private protected readonly string VIKey = "&$hf#&*hf!@6e$^hv(%";

        internal Form1() => InitializeComponent();

        private protected void Form1_Load(object sender, EventArgs e)
        {
            // Stupid line for the password text box, makes it so when text exists it's hidden
            txtPassword.isPassword = true;

            // Checking registry info
            RegistryKey key = Registry.CurrentUser.CreateSubKey(RegistryName);

            // If the entry doesn't exist, it will ignore it
            if (key.GetValue("Checked") == null || key.GetValue("Checked").ToString() == null) { }

            // If the value exists AND equals true, it will automatically login the user
            else if (key.GetValue("Checked").ToString() == "true")
            {
                txtUsername.Text = key.GetValue("Username").ToString();
                txtPassword.Text = key.GetValue("Password").ToString();
                bunifuSwitch1.Value = true;
                if(!string.IsNullOrWhiteSpace(txtUsername.Text) && !string.IsNullOrWhiteSpace(txtPassword.Text)) 
                    Login(txtUsername.Text, txtPassword.Text);
            }

            // Closes the registry key, VERY IMPORTANT THAT THIS STAYS HERE
            key.Close();

            // This adds the HWID/GUID into a text box for logged in users
            bunifuMaterialTextbox1.Text = GetMachineGuid();
        }

        // Discord button
        private protected void bunifuFlatButton2_Click(object sender, EventArgs e) => Process.Start("https://discord.gg/GyzJf5eNPe");

        // Load Cheat button
        private protected void bunifuFlatButton4_Click(object sender, EventArgs e)
        {
            CloseTabs();
            panel3.Visible = true;
        }

        // Settings button
        private protected void bunifuFlatButton5_Click(object sender, EventArgs e)
        {
            CloseTabs();
            panel4.Visible = true;
        }

        // This is the login button
        private protected void bunifuFlatButton6_Click(object sender, EventArgs e) => Login(txtUsername.Text, txtPassword.Text, devmode);

        // The exit button
        private protected void bunifuFlatButton7_Click(object sender, EventArgs e) => Application.Exit();

        // The inject button
        private protected void bunifuFlatButton8_Click(object sender, EventArgs e) => InjectCheat();

        // Clears all data from the registry
        private protected void bunifuFlatButton9_Click(object sender, EventArgs e)
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

        // For saving the HWID to the clipboard
        private protected void bunifuFlatButton10_Click(object sender, EventArgs e) => Clipboard.SetText(GetMachineGuid());

        // This is the exit button, just closes the program
        private protected void bunifuImageButton2_Click(object sender, EventArgs e) => bunifuFlatButton7_Click(sender, e);

        // For disabling auto logins from the user settings.
        private protected void bunifuSwitch2_Click(object sender, EventArgs e)
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

        // For disabling auto inject 
        private protected void bunifuSwitch3_Click(object sender, EventArgs e)
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

        // If the process doesn't exist
        private protected void timer1_Tick(object sender, EventArgs e)
        {
            var name = "csgo";
            var target = Process.GetProcessesByName(name).FirstOrDefault();

            if (target != null)
            {
                timer1.Stop();
                var decryptedUrlBytes = Encoding.Default.GetString(AESDecryptBytes(urlBytes));
                string endFilePath = "C:\\Users\\" + Environment.UserName + "\\AppData\\cheat.dll";
                DecryptFile(decryptedUrlBytes, endFilePath);
            }
        }

        // This is for minimizing the sidebar
        private protected void logo_Click(object sender, EventArgs e)
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
        private protected void pictureBox2_Click(object sender, EventArgs e)
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

        // Doesn't work, can't be removed, it will break the program
        private protected void txtPassword_Click(object sender, EventArgs e) => txtPassword.ResetText();

        // Closes visible tabs then opens new ones when you click a button
        private protected void CloseTabs()
        {
            panel3.Visible = false;
            panel4.Visible = false;
        }

        // Starting the injections
        private protected void InjectCheat()
        {
            var name = "csgo";
            var target = Process.GetProcessesByName(name).FirstOrDefault();

            if (target == null)
                timer1.Start();
            else
            {
                var decryptedUrlBytes = Encoding.Default.GetString(AESDecryptBytes(urlBytes));
                string endFilePath = "C:\\Users\\" + Environment.UserName + "\\AppData\\cheat.dll";
                DecryptFile(decryptedUrlBytes, endFilePath);
            }
        }

        // This checks the login info compared to your website
        private protected void Login(string username, string password, bool bypass = false)
        {
            if (bypass)
            {
                loggedin();
                return;
            }

            try
            {
                // Fetching the User Token
                string token = GetUserToken();

                // Fetching the HWID
                string hwid = GetMachineGuid();
                 
                // Checking login info
                string result = GetLoginResult(username, password, hwid, token);

                if (result == null) /*throw new Exception("Login error...", new Exception ("Bad/No response from remote server"));*/ return;
                   
                if (result.Contains("password: true"))
                {
                    // Usergroup checks. 4 = admin, 8 and 9 = the first 2 custom groups you create
                    if (result.Contains("4") || result.Contains("8") || result.Contains("9"))
                    {
                        if (result.Contains("hwid: true"))
                        {
                            if (result.Contains("token: " + token)) 
                                loggedin();
                            else
                                throw new Exception("Network error...", new Exception("DNS spoofing the remote server has been detected"));
                        }
                        else
                            throw new Exception("HWID error...", new Exception("HWID change has been detected"));
                    }
                    else
                        throw new Exception("permissions error...", new Exception("Unknowing user group detected"));
                } else if (result.Contains("password: false"))
                    throw new Exception("Login error...", new Exception("Username or Password incorrect"));
                else
                    throw new Exception("Login error...", new Exception("Account not found"));
            }
            catch (Exception e)
            {
                MessageBox.Show(e?.InnerException?.Message ?? e.Message, e.InnerException?.Message == null ? "Spatial Loader" : e.Message);
            }
        }

        // When a user's login info is correct
        private protected void loggedin()
        {
            // Starting the key reading
            RegistryKey key = Registry.CurrentUser.CreateSubKey(RegistryName);

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

        private protected byte[] AESDecryptBytes(byte[] cryptBytes)
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

        private protected string DecryptString(string encryptedText)
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

        private protected void DecryptFile(string url, string file)
        {
            byte[] bytesDecrypted = null;

            try
            {
                using WebClient web = new WebClient();
                bytesDecrypted = Convert.FromBase64String(DecryptString(web.DownloadString(url)));
                web.Dispose();
            } 
            catch
            {
                Environment.Exit(-1);
            }

            Process[] targets = Process.GetProcessesByName("csgo");

            if (bytesDecrypted == null || targets.Length < 1) {
                if (bytesDecrypted == null) Environment.Exit(-1);
                if (targets.Length < 1) AutoClosingMessageBox.Show($"CSGO.exe not running", "Spatial Loader", 1);
                return;
            }

            Process target = targets.First();
            ManualMapInjector injector = new ManualMapInjector(target) { AsyncInjection = true };
            IntPtr injection = injector.Inject(bytesDecrypted);

            AutoClosingMessageBox.Show($"hmodule = 0x{injection.ToInt64()}:x8", "Spatial Loader", 1);

            Application.Exit();
        }

        private protected string GetLoginResult(string username, string password, string hwid, string token, string requestvars = "")
        {
            string result = null;

            try
            {
                if (string.IsNullOrWhiteSpace(username)) throw new Exception("Login error...", new Exception("Username is null, Empty, or consists only of white-space characters."));
                if (string.IsNullOrWhiteSpace(password)) throw new Exception("Login error...", new Exception("Password is null, Empty, or consists only of white-space characters."));
                if (string.IsNullOrWhiteSpace(hwid)) throw new KeyNotFoundException("HardwareID error...", new Exception("HardwareID is null, Empty, or consists only of white-space characters."));
                if (string.IsNullOrWhiteSpace(token)) throw new KeyNotFoundException("Token error...", new Exception("Usertoken is null, Empty, or consists only of white-space characters."));

                var sv = requestvars.StartsWith("?") ? "&" : "?";
               
                requestvars += $"{sv}username=" + username;
                requestvars += "&password=" + password;
                requestvars += "&hwid=" + hwid;
                requestvars += "&token=" + token;

                if (!string.IsNullOrEmpty(requestvars))
                {
                    using (WebClient wc = new WebClient())
                    {
                        result = wc.DownloadString($"{endpoint}/check.php{requestvars}");
                        wc.Dispose();
                    };
                }
                
            }
            catch (WebException e)
            {
                MessageBox.Show(e.InnerException?.Message ?? e.Message, "Spatial Loader");
                Environment.Exit(-1);
            }
            catch (Exception e)
            {
                MessageBox.Show(e?.InnerException?.Message ?? e.Message, e.InnerException?.Message == null ? "Spatial Loader" : e.Message);
            }

            return result;
        }

        private protected string GetUserToken()
        {
            // Generating token for anti-php spoofing
            var chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
            var stringChars = new char[5];
            var random = new Random();

            // Actual generation of the token
            for (int i = 0; i < stringChars.Length; i++)
                stringChars[i] = chars[random.Next(chars.Length)];
            
            // The real token
            return new string(stringChars);
        }

        private protected string GetMachineGuid()
        {
            string location = @"SOFTWARE\Microsoft\Cryptography";
            string name = "MachineGuid";

            using (RegistryKey localMachineX64View = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry64))
            {
                using (RegistryKey rk = localMachineX64View.OpenSubKey(location))
                {
                    if (rk == null)
                        throw new KeyNotFoundException(string.Format("Key Not Found: {0}", location));

                    object machineGuid = rk.GetValue(name);
                    if (machineGuid == null)
                        throw new IndexOutOfRangeException(string.Format("Index Not Found: {0}", name));

                    return machineGuid.ToString();
                }
            }
        }
    }
}
