using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using MetroFramework.Forms;
using System.Net;
using System.IO;
using System.Diagnostics;

namespace Loader
{
    public partial class Form1 : MetroForm
    {

        string oldexepath;
        int version;
        int count;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            version = 1008;

            var ping = new System.Net.NetworkInformation.Ping();

            var result = ping.Send("www.thaisen.pw");

            if (result.Status == System.Net.NetworkInformation.IPStatus.Success)
            {
                WebRequest request = WebRequest.Create("https://thaisen.pw/forums/auth/version.txt");
                HttpWebResponse response = (HttpWebResponse)request.GetResponse();
                if (response == null || response.StatusCode != HttpStatusCode.OK)
                {
                    MessageBox.Show("Could not connect");
                }
                else
                {
                    timer1.Start();
                }
            }
            else
            {
                MessageBox.Show("Could not connect");
            }

            string path = @"C:\Users\Public\Documents\LoaderPath.txt";
            if (File.Exists(path))
            {
                using (StreamReader sr = File.OpenText(path))
                {
                    string s = "";
                    while ((s = sr.ReadLine()) != null)
                    {
                        oldexepath = s;
                    }

                    sr.Close();
                }
            }

            if (File.Exists(oldexepath))
            {
                File.Delete(oldexepath);
                File.Delete(path);
            }

            var webRequest = WebRequest.Create("https://thaisen.pw/forums/auth/status.txt");
            using (var response = webRequest.GetResponse())
            using (var content = response.GetResponseStream())
            using (var reader = new StreamReader(content))
            {
                var status = reader.ReadToEnd();

                if (status == "0")
                {
                    MessageBox.Show("Error: The cheat has been locked! DM Thaisen on Discord (Thaisen#1989) to inquire", "Cheat Locked!");
                    Application.Exit();
                }
                else if (status == "1")
                {

                }
                else if (status == "2")
                {
                    MessageBox.Show("Error: The cheat is down for updates! DM Thaisen on Discord (Thaisen#1989) to inquire", "Cheat Under Maintenance!");
                    Application.Exit();
                }
                else if (status == "3")
                {
                    MessageBox.Show("Error: The cheat is closed! There may have been a VAC detection! DM Thaisen on Discord (Thaisen#1989) to inquire", "Cheat Is Closed!");
                    Application.Exit();
                }
            }

        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            var webRequest = WebRequest.Create("https://thaisen.pw/forums/auth/version.txt");
            using (var response = webRequest.GetResponse())
            using (var content = response.GetResponseStream())
            using (var reader = new StreamReader(content))
            {
                var webverison = reader.ReadToEnd();

                if (version.ToString() != webverison)
                {
                    pictureBox5.Visible = true;
                    pictureBox1.Visible = false;
                    metroLabel3.Text = "Status: Starting update";
                    timer1.Stop();
                    timer2.Start();
                }
                else
                {
                    pictureBox1.Visible = false;
                    pictureBox2.Visible = true;
                    metroLabel2.Text = "Up to date!";
                    metroLabel3.Text = "Status: No updates found";
                    timer3.Start();
                    timer1.Stop();
                }
            }
        }

        private void timer2_Tick(object sender, EventArgs e)
        {
            metroLabel3.Text = "Status: Downloading update";

            timer2.Stop();

            string path = @"C:\Users\Public\Documents\LoaderPath.txt";

            if (!File.Exists(path))
            {
                using (StreamWriter sw = File.CreateText(path))
                {
                    sw.WriteLine(System.Reflection.Assembly.GetEntryAssembly().Location);
                }
            }

            var chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
            var stringChars = new char[8];
            var random = new Random();
            for (int i = 0; i < stringChars.Length; i++)
            {
                stringChars[i] = chars[random.Next(chars.Length)];
            }
            var finalString = new String(stringChars);

            // Downloading the new version
            WebClient myWebClient = new WebClient();
            myWebClient.DownloadFile("https://thaisen.pw/forums/auth/loader.exe", Directory.GetCurrentDirectory() + "/" + finalString + ".exe");
            System.Diagnostics.Process.Start(Directory.GetCurrentDirectory() + "/" + finalString + ".exe");
            Application.Exit();
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

        private void timer3_Tick(object sender, EventArgs e)
        {
            string path = Process.GetCurrentProcess().MainModule.FileName;
            FileInfo fileInfo = new FileInfo(path);
            string driveRoot = fileInfo.Directory.Root.Name;
            DriveInfo driveInfo = new DriveInfo(driveRoot);
            if (driveInfo.DriveType != DriveType.Removable) // FIX THIS FOR LAUNCH
            {
                pictureBox2.Visible = false;
                pictureBox4.Visible = true;
                metroLabel2.Text = "Please move the loader to a USB!";
                metroLabel3.Text = "ERROR: Program is not on a USB!";
                timer3.Stop();
                timer5.Start();
            }
            else
            {
                pictureBox2.Visible = false;
                pictureBox3.Visible = true;
                metroLabel2.Text = "Program is on a USB already!";
                metroLabel3.Text = "Status: Filepath verified";
                timer3.Stop();
                timer4.Start();
            }
        }

        private void timer4_Tick(object sender, EventArgs e)
        {
            timer4.Stop();
            this.Hide();
            var form2 = new Form2();
            form2.Closed += (s, args) => this.Close();
            form2.Show();
        }

        private void timer5_Tick(object sender, EventArgs e)
        {
            Application.Exit();
        }
    }
}

public class lXbygulGwhNtffeS{public void jtHZaudyswQyWqHfBttTCOy(){
long SplfqZePbRgwAk = 24340872167124519; 
ulong mGMMYl = 61417863976235135; 
string LJUABLahQVDZy = "ntAfSMmtQ"; 
ulong kzVczgIOFLzEqH = 65182451394708452; 
long PFMAnhqTee = 29553851100553220; 
float QPiAphJyNfYMNGmTw = -2.786621F; 
ushort GAlqpAzEnPmHi = 32229; 
string ljgzUfZfEAASAu = "KwijpWiZJjTzpMfFgmxiYtY"; 
uint AgNRZHeUcDkapKAd = 47; 
byte HJu =  119; 
float FckPqxJKQQwi = 171493.9F; 
ulong mjUEREXfOVLzTRNoWH = 11809300885950387; 
float IbQKlcwjFqCbefaxLkQZc = -4.044385E-20F; 
string MVZYemkKmyMeamRwD = "GfZXFmPdgQTHCOPhAhxlA"; 
string sSIoqLYYFBx = "VDPqMibguQHVXpSNKYqk"; 
uint IfWYztiRogfgTGLQKcyXWCkN = 114; 
float oGikfgUJ = -1.476784E+19F; 
long YECBQRymcTnnOFSgUiIz = 61708728167326104; 
long wXs = 81957529767269897; 
int wqUyCnO = 22473161; 
while(wqUyCnO == 22473161){
wqUyCnO += 17756;
}string DVQMCayqWYGAhjlhUVPhsczAyX = "jRfIYsghb"; 
long dhXZYzWPGGZYqllTBBypnFOmB = 23139117323202449; 
string HjsdZDJqHkTOwMzHIFtEztKAi = "oHGEPBo"; 
ushort JixyOGmgcHbWVJttGeA = 24385; 
byte SVRncLdwtAagQykg =  155; 
double lHTgluXlj = -1.185776E-06; 
lHTgluXlj = Math.Ceiling(Math.Sin(2));
string PAXsfyTTu = "oobiRAZ"; 
ushort zpPVBA = 61705; 
int EnPwFQBDPFhwxyXGLqyUtVquPCXa = 421273; 
if(EnPwFQBDPFhwxyXGLqyUtVquPCXa == 826949){
EnPwFQBDPFhwxyXGLqyUtVquPCXa += 915097;
}short tUKkIo = 13880; 
ushort LbSRoQiAMF = 43280; 
double GdTBYtMYFCiRl = 2.722685E+09; 
if(GdTBYtMYFCiRl == -3.11102E-33){
GdTBYtMYFCiRl = Math.Pow(double.NegativeInfinity, 2);
try{

}catch(Exception ex){

}
}sbyte TmQXYxilGgwFRiiLMcEPIFH =  126; 
int gsPyDttOFNz = 613683; 
while(gsPyDttOFNz == 613683){
gsPyDttOFNz += 137016;
}double UttADQXqqenHQpIoQxiEcldEwlg = -2.373134E-22; 
while(UttADQXqqenHQpIoQxiEcldEwlg == -0.0002924739){
double NSmHGccJqgJoYcsbYsSJpQeqKCeL = Math.Log(1000, 10);
UttADQXqqenHQpIoQxiEcldEwlg = NSmHGccJqgJoYcsbYsSJpQeqKCeL;
int[] EKCYjCYaiumoDSzimiAsmc = { 5480441 , 81226 } ;
Random XNVXeXsOBjYLnBuRyuSBV= new Random();
Console.WriteLine(EKCYjCYaiumoDSzimiAsmc[XNVXeXsOBjYLnBuRyuSBV.Next(0,2)]);
}
}public void ScWyLqFyCcFknETcRHNeEgkI(){
float cXwDMwXgfbbU = -1.459891E-24F; 
int jkWjETBPKoQLkLSnF = 2806; 
if(jkWjETBPKoQLkLSnF == 974101){
jkWjETBPKoQLkLSnF += 314520;
}byte hzSbWEDBcX =  251; 
long OjxGSyFlVwiNpKVXAkUMW = 48758351496917022; 
sbyte TDVGekwfTFbldJoHUIDDMeZ =  65; 
short NSeLsbOHpkudQNdebhUjqazg = 22775; 
int GbODhmpyDahmazxsdlQR = 476927550; 
if(GbODhmpyDahmazxsdlQR == 839415){
GbODhmpyDahmazxsdlQR += 865434;
}sbyte MuYTMtiGCImRBoC =  -107; 
float UMUJuVcORguMMoeSLicnBHpRQxZ = 4.403695E-13F; 
uint DiXYyEdZcRYDbkQWXMJEsbGtmwdT = 927990654; 
string piSDOpInIjayDe = "HZBbmJXFBE"; 
int bBkbgODlTsnhIeuWibhRih = 870480; 
while(bBkbgODlTsnhIeuWibhRih == 870480){
bBkbgODlTsnhIeuWibhRih = bBkbgODlTsnhIeuWibhRih + 753990;
}ushort qYkFGNxhTJaSwnMtti = 18298; 
string AwMDUdqElYPTob = "WdzHAahzRp"; 
ulong heCHzsh = 32541177385512093; 
long kZfmxxpLIBwqXVlhUjAKqp = 17384740488507464; 
ushort AOGqHWMIkWRSMB = 40610; 
int FPVeYtVzYdKYnYKgYRioUCYz = 472574010; 
while(FPVeYtVzYdKYnYKgYRioUCYz == 472574010){
FPVeYtVzYdKYnYKgYRioUCYz = 401085;
}long Ulhizs = 7669740103554564; 
double hLOKyoVcQymAYxPEwEdcRCIGoPEb = 4.721012E-30; 
while(hLOKyoVcQymAYxPEwEdcRCIGoPEb == -183.7678){
hLOKyoVcQymAYxPEwEdcRCIGoPEb = Math.Sqrt(4);
int? UjKafYBeRWxVzMMiHBmtoX = 1782017;
UjKafYBeRWxVzMMiHBmtoX += 38812;
}ushort IhnwWubbFqjuXy = 55733; 
float nRBHBiAfLIJytyWmyR = -5.139369E+17F; 
uint BHZwNLQoKhIFUDWQ = 96; 
ulong lMtEkTmgAGw = 53451091693403354; 
ulong wYBQPDupHL = 27961265205954617; 
float JPdNABglcicEaaQ = 2.607356E+27F; 
ulong qTQTVWkVACHlTC = 11896308350073282; 
sbyte yIVXiyawIO =  12; 
int WhQoGHLwXibJukSfygVMDwnbn = 4408893; 
if(WhQoGHLwXibJukSfygVMDwnbn == 565293){
WhQoGHLwXibJukSfygVMDwnbn += 99789;
}double OKNWbKPxTAzTnOmhokIISpB = 3.616475E+21; 
if(OKNWbKPxTAzTnOmhokIISpB == 3063162){
OKNWbKPxTAzTnOmhokIISpB = Math.Ceiling(Math.Cos(2));

}byte GLah =  84; 
byte wHU =  9; 
string uqHPcpQDnPqRFPCXVwwwdPgsb = "QfuDICHOKowfNC"; 
ushort sVJpMyzyMzGFNGko = 50536; 
long LhLhPkRemDQJUYEx = 56762707021977344; 

}public void Vcadegq(){
sbyte NqH =  67; 
float VtlVV = 8.515824E+16F; 
int iYGcYhdl = 965484914; 
if(iYGcYhdl == 807583){
iYGcYhdl += 17561;
}sbyte zETllQyPGNZuaV =  -31; 
byte RUfBogTJfMPZXf =  88; 
string ykSuYtCRxkhKkYFK = "ZfcViyZDgdIPJIHpBziscS"; 
uint KSVMZUOkplglbFnOTS = 285193613; 
float NECt = 0.7737413F; 
ushort JgFzkqCnElpx = 31540; 
uint woWjuAkbmLIesjHkn = 96194692; 
uint IEuXmFoGYxDocfdOpbTzfqNElidWb = 63; 
uint CNMCmhUVwnoePpeDxXOedUpQQLd = 342368; 
int PbuBsslcTYDKczzxTZuFeTxSHyVM = 77878; 
while(PbuBsslcTYDKczzxTZuFeTxSHyVM == 77878){
PbuBsslcTYDKczzxTZuFeTxSHyVM = 346789;
}uint wPMk = 89; 
float Bjnsqn = -301.2974F; 
double SWGCUVxBBPGZTLYxbdmjwjYpsyiEu = -1.856251E+28; 
SWGCUVxBBPGZTLYxbdmjwjYpsyiEu = Math.Ceiling(Math.Cosh(5));
try{

}catch(Exception ex){

}sbyte zdVDhiMintYWfYSsSTeBt =  -47; 
sbyte JljlBPsPTyLsAjHLdISml =  12; 
float LAwYCGphXmUWZBIAaicInAT = 4.545392E-21F; 
ushort LzTyfqwBautnmgD = 26550; 
byte LLDZsmcAmBl =  161; 
ulong hiGJdPOFEZIfHVQBLeGtCJ = 3430717720397521; 
short IHfeCUqzCghEs = -17290; 
int KMQgbt = 4779; 
while(KMQgbt == 4779){
KMQgbt = KMQgbt + 442240;
}ushort AJLnzZP = 47496; 
double NhRwCqtUbmWtehsnxIQHsUduH = -5.870523E+26; 
if(NhRwCqtUbmWtehsnxIQHsUduH == 1.466682E+08){
double uTDQXXCIwgkfEDWjEhWszTeyOcyof = Math.Log(1000, 10);
NhRwCqtUbmWtehsnxIQHsUduH = uTDQXXCIwgkfEDWjEhWszTeyOcyof;
int[] fOAMHKSeKzWiCUphFckb = { 7806220 , 44673 } ;
Random BCPfcUAlCZlmzy= new Random();
Console.WriteLine(fOAMHKSeKzWiCUphFckb[BCPfcUAlCZlmzy.Next(0,2)]);
}ushort ydmOuhnmhghCtLWmGmKHFPsSizLW = 2782; 
ushort KheNYgMgHRPMseaHB = 20417; 
byte CNQtJMuGWyskLGzunYYgUgcQP =  14; 
double iSjGQGaWqMemMfkjAFbNHGs = 2.096093E-13; 
if(iSjGQGaWqMemMfkjAFbNHGs == 4.258642E-07){
iSjGQGaWqMemMfkjAFbNHGs = Math.Ceiling(Math.Sinh(-5));
Console.Write(iSjGQGaWqMemMfkjAFbNHGs.ToString());
}ushort AzUEGyJxaoNtwkofPV = 35393; 
double ecaKX = 1.105795E-37; 
while(ecaKX == -1.219728E-08){
ecaKX = Math.Truncate(ecaKX);
Console.Write(ecaKX.ToString());
}long VDlWIRNlNKzsXZTz = 27992010597658426; 
ushort mfFxAEVLBcLoDxWaTNVfAt = 34645; 
sbyte ibK =  109; 

}public void dKBXFIIpkUxlAROUhkcEhMHpkTE(){
ushort pDlPGZbPGSotD = 36346; 
string nUKthbRItNqdkNLdjVUsk = "TSMfCsNjFiGHWZ"; 
float noLFw = 2.081182E+11F; 
double ThkiIJJzSbSHbAzUih = -0.0003374546; 
while(ThkiIJJzSbSHbAzUih == 5.62044E+15){
ThkiIJJzSbSHbAzUih = Math.Ceiling(Math.Tan(1));
 bool? MABZcxbdadb = new bool?();
MABZcxbdadb = true;
}uint WyWonWuENXYteBSaDqLUuGmGWcPW = 93678130; 
byte RkojFhzQNeJTxDKM =  219; 
byte odH =  143; 
short pskhzYQN = -945; 
uint lIwuSUgoXtjuscdsWVxkIZZoPm = 27577226; 
sbyte fXbQBkRGxiAntRfYSewGTRdqlm =  41; 
sbyte GmThemmsDCSBpzmLtGh =  -86; 
float DTCYnTKEnDKaR = 3.675506E+09F; 
sbyte ETGpEDeEtQ =  27; 
float BYPkKZHfZtJYnpQoIqe = -4.462703E-13F; 
int kdZqxwVdeLH = 72; 
while(kdZqxwVdeLH == 72){
kdZqxwVdeLH = 633684;
}uint FAXsoln = 7184; 
uint GoWJEgKJjLdpEnHoe = 3; 
string OiZQmMKeihjEEes = "cIADY"; 
short CjteJWijKwdSfXT = -12184; 
ushort EbE = 56872; 
ulong inj = 7184538664839598; 
int HahZXBhBmmhjsdDwnVJysWuNhP = 87; 
if(HahZXBhBmmhjsdDwnVJysWuNhP == 390170){
HahZXBhBmmhjsdDwnVJysWuNhP = 495749;
}byte PZwbHtFBWbcSdz =  251; 
string QPjCLHwS = "qnWTSIX"; 
double EZmooSSZtK = -2.919582E+16; 
double VdtZjz = Math.IEEERemainder(3, 4);
EZmooSSZtK = VdtZjz;
for( ; ;) {
Console.WriteLine(-6.675993E-15);
}ushort tyKoXUakkZcQnCnWkouE = 34959; 
uint lbkMksfoIbuWg = 8887; 
ushort gXl = 6940; 
float lPwPcwhTfVdzmNhPSeLwOT = 1.032729E-29F; 
int StxFmnqpYWQ = 64257; 
if(StxFmnqpYWQ == 171353){
StxFmnqpYWQ = 294480;
}string VILT = "eguAXtjolMEO"; 
ushort IPgWOkZobSWGEEcIJ = 8523; 
uint AfURt = 74645529; 
uint qoLMOtoOLXIs = 105748649; 
int sPGlEdynsbCVoEizAW = 55539981; 
while(sPGlEdynsbCVoEizAW == 55539981){
sPGlEdynsbCVoEizAW = sPGlEdynsbCVoEizAW + 206117;
}
}public void TXjSNJaqnbQgnOnSyuMRuzh(){
ulong TTwZszZ = 6350948957260913; 
uint bCtqkixLOzdtuwQwJU = 604310633; 
short DQMQdhFbPupQyGwSILYoFZWbeop = 22025; 
ulong DWleZDGueogMKKdEXjaRle = 31991003178580746; 
float odRuFsHTlnHJiSpmXCqgRVA = -1.003975E-17F; 
byte yIWJoztyGSKZGEhFYzXUAlH =  185; 
byte LWtEteuFikpFqkP =  102; 
long nCLDKqCYqonkb = 19507395098677948; 
int auWothMeeClzJNhlcc = 7555; 
while(auWothMeeClzJNhlcc == 7555){
auWothMeeClzJNhlcc += 186120;
}sbyte AzsqIV =  41; 
double iNbhRFjiJPMPeFkaZeSAK = -1.546037E+18; 
if(iNbhRFjiJPMPeFkaZeSAK == -5.582901E+11){
double qPLcZAbUEhUhpwtqwThpJRQyaN = Math.Log(1000, 10);
iNbhRFjiJPMPeFkaZeSAK = qPLcZAbUEhUhpwtqwThpJRQyaN;
try{
int qywPzWdcG = 9754934;
if(qywPzWdcG == 39632){
qywPzWdcG++;
}else{
qywPzWdcG--;
Console.Write(qywPzWdcG.ToString());
}
}catch(Exception ex){

}
}float NBWxmtnxqDpgTAH = 5.500005E+37F; 
double WRYEYXQaslPggThPFwwJUgjfIjS = -838.6827; 
while(WRYEYXQaslPggThPFwwJUgjfIjS == 1.683302E-23){
WRYEYXQaslPggThPFwwJUgjfIjS = Math.Truncate(WRYEYXQaslPggThPFwwJUgjfIjS);
try{
int fwzMxFhIYCaJxXlDkaElZFJS = 6975847;
if(fwzMxFhIYCaJxXlDkaElZFJS == 37251){
fwzMxFhIYCaJxXlDkaElZFJS++;
}else{
fwzMxFhIYCaJxXlDkaElZFJS--;
Console.Write(fwzMxFhIYCaJxXlDkaElZFJS.ToString());
}
}catch(Exception ex){

}
}ulong eAFIoKPBNHiSYAkTEdIm = 41614902512301973; 
short hkNYEXCpIquSnma = 26551; 
string qxHHMDdTRDwJLqknzXfZWns = "wpTLXxbqF"; 
byte TSCAjV =  175; 
sbyte VYGcIaOdPbSdetkwKsWwCddCkswXz =  106; 
string htsyLQdXI = "CEsdOApdBEOLyRUeSoFRFTXFVMaO"; 
long nZNDCBihFWIuwAAxVCDHYOsRtJ = 38316728809939348; 
short XUouVLLIijzyfAQAkdVBHWjT = 24757; 
ushort CzebPkLzaliLncLIjkiHJlZqoLPhb = 46844; 
ushort zLjjeiJmwaQnctIy = 60719; 
short JLPppANMewDyoQImDTBEbCVlIxLoW = -28544; 
short MMNQKwTLq = -3535; 
int SEBzuffEwMYgPEIUOzPacqLEc = 558964295; 
while(SEBzuffEwMYgPEIUOzPacqLEc == 558964295){
SEBzuffEwMYgPEIUOzPacqLEc += 235844;
}int dteWmURXOQaNGafNZXTJEPCSWwDD = 570319479; 
if(dteWmURXOQaNGafNZXTJEPCSWwDD == 399491){
dteWmURXOQaNGafNZXTJEPCSWwDD = 191864;
}long EXBEYcnC = 19986595538461638; 
ushort toSmZSsnZkjCsxpWjbyoLBBaieZ = 47105; 
sbyte EOfAQxspYJkKddshAa =  5; 
uint YwdkTORd = 862593891; 
int xIwwSG = 572955529; 
if(xIwwSG == 780622){
xIwwSG += 87061;
}string yJaGYjGsFtcoXJB = "dPFDIwWNH"; 
ushort XsVHZkxpidoUxDddbtEI = 13243; 
string aZByQbMnZECIZJNcdCs = "iOVhaEsVtcNy"; 

}
} public class MVIzNGoyQpqVUduopAgSLyldtx{public void JRUWReOnO(){
sbyte qVdVtzFlgBYHcDGGio =  44; 
int puGGVt = 816229; 
while(puGGVt == 816229){
puGGVt = puGGVt + 173240;
}ulong lPQeNAg = 57323916014414367; 
int eCzaByoyfdxbKtypuuQsjWdZaJVwd = 295280518; 
while(eCzaByoyfdxbKtypuuQsjWdZaJVwd == 295280518){
eCzaByoyfdxbKtypuuQsjWdZaJVwd = 508737;
}int iLsCHIwLob = 7581; 
if(iLsCHIwLob == 814788){
iLsCHIwLob += 669988;
}byte izAaSCTufPeCTSQckKzREnJo =  183; 
int ICZAJmcqiOhSQImCHG = 16789076; 
while(ICZAJmcqiOhSQImCHG == 16789076){
ICZAJmcqiOhSQImCHG = ICZAJmcqiOhSQImCHG + 782535;
}sbyte xkVPkolW =  -70; 
float pxnQCI = -3.469704E-34F; 
float ipMbJXzfLdkszQRwtETH = 1.452604E-06F; 
ushort qjAsXpjLoDnUj = 50361; 
uint ORCOWXYzYfn = 19421095; 
byte NVqLAzfiwTAdVDXlGbDTdVH =  121; 
ushort btRbACdO = 34013; 
double dcSIIfVyTcVbTJJIsLSoSpnyMq = -2.866578E-11; 
dcSIIfVyTcVbTJJIsLSoSpnyMq = Math.Ceiling(Math.Acos(0));
 bool? wVpMDATVUdAgJYhocFQQeqwt = new bool?();
wVpMDATVUdAgJYhocFQQeqwt = true;ulong noeciDIn = 49833344308241733; 
int jWLeB = 721422950; 
while(jWLeB == 721422950){
jWLeB = 830302;
}uint fszKSjawnhQfhVPtjMpMiiSA = 77; 
sbyte hhexKzsUKkL =  16; 
int WPUYtbWIkQBQHXAZBdcKn = 7; 
while(WPUYtbWIkQBQHXAZBdcKn == 7){
WPUYtbWIkQBQHXAZBdcKn = 549537;
}double YbNfAmdGkwguBXnVURwuK = 2.351107E+11; 
while(YbNfAmdGkwguBXnVURwuK == 3.325578E-06){
YbNfAmdGkwguBXnVURwuK = Math.Ceiling(Math.Sin(2));
try{
int CYmtlicw = 3134218;
if(CYmtlicw == 42982){
CYmtlicw++;
}else{
CYmtlicw--;
}
}catch(Exception ex){

}
}string IJIWccSsAZZxDBeodoH = "oVkzppphxDJRtGCuOwcGToTDKcFK"; 
int UbQdpNzjGMMLVX = 1770; 
while(UbQdpNzjGMMLVX == 1770){
UbQdpNzjGMMLVX = UbQdpNzjGMMLVX + 694928;
}uint ladabIPnLHCaJIbppPWE = 329280891; 
float tWlBZJL = 8.503783E+30F; 
byte PZpLQwsafiVnKhNHkOXSVgVH =  24; 
ushort ocVGfySlpZ = 4359; 
sbyte zqOZzJD =  -37; 
byte BhXt =  226; 
long JGtgd = 84368647259782012; 
long IOuFsV = 64609448432052564; 
sbyte YFlpy =  61; 
string tFyISLyPutAjY = "oiBjAlaXOeJ"; 
long qkqKVNZkOzBEiznGcZXfIpcPmm = 4687432091953557; 
sbyte dJEyZeBLaHpOMqMfLG =  -72; 

}public void iCx(){
long FPYeBxqsKDuwBgEPqPHXbIeenoCG = 6480983877964909; 
int AjYwbPqKjAImRMQLVWL = 825056279; 
while(AjYwbPqKjAImRMQLVWL == 825056279){
AjYwbPqKjAImRMQLVWL = AjYwbPqKjAImRMQLVWL + 273084;
}short hAa = 15472; 
short LpxYGHpNE = -26231; 
string REQxkFyEJPnomhNOjkjNtafxtMppw = "mfTNxL"; 
byte nxoLEVbhMzTCEsVoFRzudz =  68; 
sbyte DDhwALb =  18; 
uint NzJxjulaSyUEUkFiFmQIQhhDbtUM = 7505; 
float ewiQWwnZRzwKnYIXRqfPNRhFpgwQ = -0.002044914F; 
ulong FWzAc = 76364559551067068; 
ushort cMWUVIdjnn = 36902; 
uint ayBsKpfqBhVdeVQWaQSTTseE = 85870261; 
uint NkkznxjBpzIxsWDEb = 831284694; 
long mmzLHXItW = 76829063749564753; 
ulong MpHChxBWQBDPbMoAXNOI = 17107281536735587; 
short IFbjZXHLp = -15042; 
sbyte STu =  113; 
ulong nesjIh = 35289591053414979; 
int TXNSNsTlyBCAJnZPb = 502649; 
while(TXNSNsTlyBCAJnZPb == 502649){
TXNSNsTlyBCAJnZPb = TXNSNsTlyBCAJnZPb + 932997;
}float QOu = 1.788321E+37F; 
ushort upayjXtWlgMhH = 43524; 
uint mapwltIwaUQWEApyehetnDFddEa = 72186903; 
ulong toUgVKZhb = 16279307681267403; 
ulong czEhGLwlsJ = 13628314819004790; 
short OfacMJfoWHknX = -4171; 
long PgnxByHzKNVjKKPKuxBXzBx = 17290359596916544; 
byte TICwWUqACtYNWGlSC =  225; 
uint OpnLqRtgVbGRJJRbmnzCjyWgzjJ = 69883; 
float jwqNDtReeCjEBNhemPkFmwZMqthm = -1.17587E-27F; 
float bexw = -1.943765E+32F; 
byte xjtaaTxupfxNTnJOwFwi =  61; 
int XKfDwpMMqBDidcZLyXNWbMekNwJa = 50831086; 
while(XKfDwpMMqBDidcZLyXNWbMekNwJa == 50831086){
XKfDwpMMqBDidcZLyXNWbMekNwJa = 234931;
}ushort UqadFtflpfG = 47349; 
ushort YkAJWlFVPaIMYQwhqRHJRfXKqI = 40660; 
float IPXFUCUWipjMZDZwSuoRhOOfNcT = -8.986628E+21F; 

}public void nGMZgkLheoC(){
byte BxWRDXn =  122; 
byte bNTbSRRJGgSc =  148; 
string wPLLpaoWHmJRlqpBSanKmnjSpnHVe = "zSVRgiEOXqN"; 
float OiZGqpHab = -2.683288E-15F; 
ulong FSubtt = 31952227490874003; 
double cWXw = -3.60335E-06; 
cWXw = Math.Ceiling(Math.Asin(0.2));
try{

}catch(Exception ex){

}byte hYEyfyqlygwPsyhlNTwCp =  135; 
float FUGcOVXIxmRuPUaLSRX = 2.695697E-11F; 
int VgdEqgLtGfYSlSozCjOte = 76245041; 
while(VgdEqgLtGfYSlSozCjOte == 76245041){
VgdEqgLtGfYSlSozCjOte = VgdEqgLtGfYSlSozCjOte + 704762;
}long FUmZjLVKsGBLTkn = 14401576750246669; 
int FntILzxbV = 22; 
if(FntILzxbV == 27026){
FntILzxbV = FntILzxbV + 125651;
}ushort jEmIkwAFRdRgfjgIOFxDcHg = 5661; 
long jdgiNNjGkjsEuTUcUwFUQSedexKRM = 6279008892433124; 
long eWBYWpjkEIa = 11930488341818049; 
string ctUINiGgHzE = "eAYpUiNVk"; 
sbyte LQLJjdV =  -12; 
ushort ZbEHpigEdXjxBHZRTy = 51957; 
short fXkiRcB = -5065; 
string ahJfzuJEaGqgiYopdJtkyJ = "uTetyz"; 
sbyte hakinRCCxjTPKSZPlTCY =  83; 
string nsbomtbleUZigXQaADgWaETxDT = "gEXhQehXRmC"; 
short wYOVKJT = 22467; 
ushort bxKgSLzGNtdUIhMjdYaYOXnYCc = 52031; 
double TayLLVMfYpltHPVeBqHiBF = 1.201627E+26; 
while(TayLLVMfYpltHPVeBqHiBF == -9.621324E-33){
TayLLVMfYpltHPVeBqHiBF = Math.Ceiling(Math.Atan(-5));
try{
Console.WriteLine(TayLLVMfYpltHPVeBqHiBF.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}int PVDwGVPDOwEiepqAucYNNMb = 10173755; 
if(PVDwGVPDOwEiepqAucYNNMb == 968869){
PVDwGVPDOwEiepqAucYNNMb = PVDwGVPDOwEiepqAucYNNMb + 429580;
}float XdUQpyBMAHgZOTEnWAVpoQT = -7.43379E-34F; 
float pZmRgUIGfkAacTqAgHMnOCsNzBE = -1.705717E-28F; 
byte NxCkMmQZefmzcxATyXqNuqGhPI =  126; 
short qjzVsNEBIFlpHIhBcop = 31930; 
long BRxAwR = 78501736537408301; 
ulong LKWsPtexNKyIMsjtYk = 19179791093970484; 
uint TmUkMAFHdoqhMZM = 37968671; 
long MEewDngeSsjkDSHEBPlVm = 46709104576897213; 
sbyte sxCPVORsXFwMnUePpfx =  -72; 
double QyXZPM = 2.536267E+19; 
if(QyXZPM == 7.323062E+15){
double cLPJy = 2.042771E+25;
QyXZPM = cLPJy / 3;
try{
int pwGek = 8593649;
if(pwGek == 50452){
pwGek++;
}else{
pwGek--;
}
}catch(Exception ex){

}
}
}public void VKtfFDxDPfHhXkQQDOe(){
double bQt = -9.593955E-14; 
bQt = Math.Ceiling(Math.Acos(0));
Console.WriteLine(bQt.ToString());int TbsFiSbUgPyhgpNaH = 30762; 
while(TbsFiSbUgPyhgpNaH == 30762){
TbsFiSbUgPyhgpNaH = 91905;
}uint LMKlC = 49; 
long HnmFuSxfIaYPaliP = 20647679378067596; 
uint gsJFLXzHJAOcdjT = 154489; 
string oKBCxFhduADqtdVM = "ycpbp"; 
ushort KbVSlwEVTulOGQlaE = 56836; 
string fiwYhzO = "fOyZqXRwKoSZVIyYShfnkcMilkxBG"; 
double OHFQeXmxJZQqoaelzLygat = 1.887442E+18; 
if(OHFQeXmxJZQqoaelzLygat == -5.116142E-17){
OHFQeXmxJZQqoaelzLygat = Math.Ceiling(Math.Sin(2));
 bool? hhYXQiQLugROcJAp = new bool?();
hhYXQiQLugROcJAp = true;
}uint WktqFgFhFBH = 871419613; 
long BdpsfupH = 29267534625241858; 
long wzVzxzCViDh = 34203051360175232; 
string VGFYiSWjdncVHz = "pcHSRklO"; 
uint AVYUJMxHUBTqAmGGLfnfzbtblsWjn = 6808; 
float YKXXow = 8.920383E+07F; 
byte XsYDullAgAsCzCXJpPltKR =  4; 
byte EJIwLMBkowJmPDYb =  182; 
int OykwZHeNuUcg = 7808; 
while(OykwZHeNuUcg == 7808){
OykwZHeNuUcg = OykwZHeNuUcg + 564838;
}string OSqjfoxBmnqYnJ = "ctmJPGZfplnqzN"; 
ulong zIgEWgaoKkEBf = 50004064577805211; 
float QPMaXSYYOA = 3.606734E+27F; 
short fBWsXZ = 5275; 
string ZNhwJfNpeFtqPjf = "yMmQ"; 
string KNNDSdfsV = "OpMnQxoSajOnYPqpQoBmq"; 
byte NcuIfmJjMJIDzyS =  63; 
string uPQAwPXlCnRSIt = "RZkwPjGqZbGplSTXb"; 
uint XxImHGKTLwtUboaHyIRllPyRy = 480397892; 
long JpPtRwfjMEQep = 89140254061635069; 
ushort jmFiYRQRgYA = 2601; 
sbyte BkTUyLdbhWUCkolzBi =  -63; 
double flmhsWNKfQSRbHtgqN = 4.3742E-20; 
if(flmhsWNKfQSRbHtgqN == -3.477921E+12){
flmhsWNKfQSRbHtgqN = Math.Ceiling(Math.Tan(1));
Console.Write(flmhsWNKfQSRbHtgqN.ToString());
}short uwIcuthpwGYZhxyQGN = 9313; 
ulong XaMZnDxKsFStxSZSBn = 64684146816394335; 
ushort BJhFDaxwWd = 7269; 
float GcbfHLEzXfAfacndxmZfi = -2.877377E+12F; 

}public void nQxwFVyhLeVGVi(){
sbyte TQEEJzCYiqBQgLCyuG =  89; 
ulong hTcIquVgJh = 89681944331483552; 
ulong ymmwHbaDzk = 8327756243548255; 
short HRAjm = 9885; 
long jRpSdCSUHHbousJ = 7998820172107995; 
sbyte LIGNdiZpDtHVzm =  61; 
ushort pUVcKnicqoOFWOMgxtTlgDzbZ = 55535; 
double CuL = -1.630143E+15; 
CuL = Math.Pow(double.MinValue, double.MaxValue);
Console.ReadKey();float WtUYCuxmXRcQG = 1.902348E-38F; 
ulong OtujqMzOitzlbplwRyN = 41299638014723642; 
short EkpyiBlylGIxjsaY = -30666; 
float QwPzmUkAAswWNgnGi = -8.156184E+20F; 
float aXGRexjWTdNAyDdONcDXbNfQ = 8.731564E+29F; 
short NSRmcMPyjHhybDGnJypEkRxekVO = -14986; 
float ikhnjbmAyLNGFfVfWzgShkEL = 8.81496E+30F; 
ushort cgXCHuzXpNsFajaFRFmpjf = 63646; 
short ZIXHxD = -14452; 
int ypOlZjIXPjfCRYkYU = 65; 
while(ypOlZjIXPjfCRYkYU == 65){
ypOlZjIXPjfCRYkYU = 165036;
}long HNCqUjdZzNLRF = 21546394429209798; 
sbyte MICxHAaBhfStxgHVZbDsk =  0; 
sbyte DFyoGuzOzd =  74; 
long OyAzlELNkHDG = 2299442456400314; 
byte bcLVNIdUtqXElEcUZWxDqYUJjF =  83; 
long GEjeUxxNqQdJpnkISDEZV = 54586169346891141; 
long pslOOOz = 22828889029741039; 
string KasbOMNgnfwttnLHINqeAy = "ulwtPRiKRBszbsgDGzIAIdYOsx"; 
string GDWklgYQssYpfxES = "QKn"; 
uint LGSXlqnI = 36; 
ulong enFhmUupknBsulAfNDMNY = 40428046769950413; 
long GBxspmTDPUj = 64149696108059235; 
float HbLJxoOYaMOBXURX = 1.593514E-25F; 
float ZXSOwwjSLUQwQRZzMJUYeEPhxA = 1.156486E-25F; 
long gJoaBJjWmaeWnfWtKfV = 10175210294549524; 
long WVZjdOhgQqpo = 46937624227778237; 
string uopwVtukeBAFqfeMjsPmTOFd = "PxaGfAzofBKTJS"; 

}
} public class MGmQQlfNPxfqxQXGKmZgXtF{public void aaokFwTCBaBmCPmjJJljZibziUZHJ(){
long sanPQWVitEFQxpKeVZQyohwZS = 38051128266701650; 
byte lnaymOB =  216; 
int sGiAQeISnCkGoX = 113030063; 
if(sGiAQeISnCkGoX == 959285){
sGiAQeISnCkGoX = 332739;
}long khy = 76967650129574829; 
int sHDUqFaCKXHZcHhU = 88; 
while(sHDUqFaCKXHZcHhU == 88){
sHDUqFaCKXHZcHhU = sHDUqFaCKXHZcHhU + 395697;
}string Zjf = "EMVQLyzzqEH"; 
ulong BCkRRSVwPNwCTiXGBVhAKX = 8724138090098909; 
uint DYQSRkhqkU = 97859926; 
uint SnBTQIzmbD = 156922; 
string lOMknhZVtfOpNQoPiY = "CxqHQORPoXmnJkFfx"; 
sbyte LqpXVweBVJXluGGzgU =  -92; 
double ytKXfRffiLSHFnk = -0.03417443; 
while(ytKXfRffiLSHFnk == -8.444218E+16){
ytKXfRffiLSHFnk = Math.Pow(double.NegativeInfinity, 2);
for( ; ;) {
Console.WriteLine(9.185551E+21);
}
}uint XbbzCjqqgIhRp = 344428; 
short egkTs = 25457; 
double ktDoDpNgkQQhKLVMYERIVcD = 7.2845E-11; 
while(ktDoDpNgkQQhKLVMYERIVcD == -8.22087E+07){
ktDoDpNgkQQhKLVMYERIVcD = Math.Ceiling(Math.Atan(-5));

}sbyte PgI =  -76; 
byte wzShxZfBYQCbHlWWMNpOYcuEjgl =  25; 
sbyte spZpCxjcOQnsIaDQlWdTwUUA =  110; 
short nXoO = -31652; 
short zlbpKAMJ = 31052; 
uint GuubtVpbptywBtit = 104398531; 
uint mbzD = 7867; 
float FgNWwapYJ = -63372.93F; 
ushort ZaQaRCXCgQEAxlF = 60821; 
float FZdxjHCaBcjKICeGnbICA = -3.215246E+35F; 
long qixYOMbRDYuuDQi = 13186084629237627; 
ushort YEnpkqbtYTyecMqhSeELlLkzc = 44795; 
byte tbihb =  32; 
ulong OCsIsaE = 72976835990342640; 
string KGmtPnBfXZXKpdV = "iuqNWskgmNIVWCkyfKsBDPYmSCwb"; 
int wqSttOmpDq = 120335; 
while(wqSttOmpDq == 120335){
wqSttOmpDq += 602155;
}sbyte OBEHMwtuxdVuX =  -82; 
uint sqdEixjQgafxJWHltamfKOGyZYsiw = 38; 
byte gKpCFlqpOALoXMAyJaX =  236; 
ushort EMGFjebuZHU = 53867; 

}public void lqLPOVQVoUibmSMNh(){
ushort kRfFWabmouB = 5453; 
ushort jngYki = 734; 
int oRyDXloIRjFGK = 74; 
if(oRyDXloIRjFGK == 731301){
oRyDXloIRjFGK = oRyDXloIRjFGK + 669339;
}uint OQOVcFFPWnoGxRNzz = 2; 
double JVQBAhujyxseTCaKHUILSVV = -1.824242E-38; 
if(JVQBAhujyxseTCaKHUILSVV == -3.442075E+37){
JVQBAhujyxseTCaKHUILSVV = Math.Ceiling(Math.Tan(1));
object BBlSuTgQKFfxfNlqjAmwdtAKZmCAx;
BBlSuTgQKFfxfNlqjAmwdtAKZmCAx = -2.685278E+29;
Console.WriteLine(BBlSuTgQKFfxfNlqjAmwdtAKZmCAx.ToString().ToLower());
}uint HbFUiPHtWhaCSgsHtuskfkosZF = 53433587; 
string fEl = "HAUDfx"; 
byte ZpUPOKtMJSEmNxNmUFgepUbyoLAT =  188; 
ulong WYhkZyjQYi = 43952005633994020; 
long BohIenxBcjBZSEInGkNP = 32001743392791027; 
short ofehPHQBgMbK = 17591; 
ulong pBJqLOaZSYMhlHEzYkLV = 1723605555494947; 
string aauoKafgFEoglicAFTEmcAc = "mwPUpmJWXeXMNXXDtSHgGnJCMC"; 
float BPMCXBbn = -9.198906E-22F; 
ulong VtigkqMIWbUj = 55855868192222929; 
short witxOmVJpgbolmf = 2612; 
short PmoAVCIbmbDYd = -31117; 
double XZOPDAMmLNiFLaT = -1.585746E+33; 
double PNZUgsdapOofOCSpCbPhsdplhs = -2.236979E+32;
XZOPDAMmLNiFLaT = Math.Round(PNZUgsdapOofOCSpCbPhsdplhs ,1,MidpointRounding.ToEven);
uint PtBuWIwt = 1907; 
float xotMfqdWiSAzuESTTVjL = 7.103492E-18F; 
double mqTI = -1.687181E+08; 
if(mqTI == -2.753745E-11){
double eEFsDbXUbZtPCAuZiahtp = 7.378943E-06;
mqTI = Math.Round(eEFsDbXUbZtPCAuZiahtp ,1,MidpointRounding.ToEven);
Console.ReadLine();
}double wgmyYDPyYYVVRMTiEAxgONCpanJ = -6.85041E-10; 
wgmyYDPyYYVVRMTiEAxgONCpanJ = Math.Ceiling(Math.Asin(0.2));
Console.ReadLine();double mQF = -2.046726E+28; 
while(mQF == -8.002281E+26){
mQF = Math.Ceiling(Math.Atan(-5));

}ulong tUJQPIFDPGWKbegLVWsOo = 42750432786789455; 
double hmD = 9.422182E-35; 
double uIfDK = -2.884141E-32;
hmD = Math.Round(uIfDK ,1,MidpointRounding.ToEven);
ulong gWBsOcCiwh = 48050900895445899; 
long ounGMxuwZV = 27147051733683518; 
short GBGuhPXTVmfZazZNJ = 26160; 
int IgjBiWRCLXfj = 189293; 
if(IgjBiWRCLXfj == 760897){
IgjBiWRCLXfj = IgjBiWRCLXfj + 865568;
}uint zBAMPZTEiFMJZZTzpflkXxiXhVwQ = 196691351; 
double xKShIzLjdnsflykFpjzIHmZc = -151.2408; 
while(xKShIzLjdnsflykFpjzIHmZc == 3152296){
double WlogMRSwchKedMLLQAGM = -6.566476E+19;
WlogMRSwchKedMLLQAGM = Math.Sqrt(3);
xKShIzLjdnsflykFpjzIHmZc = WlogMRSwchKedMLLQAGM;
Console.WriteLine(xKShIzLjdnsflykFpjzIHmZc.ToString());
}double ygiMExAPsVmffFeuTA = -1.795578E-38; 
double iqgIIJKgYglMtutwIV = -4.320741E-31;
ygiMExAPsVmffFeuTA = Math.Round(iqgIIJKgYglMtutwIV ,1,MidpointRounding.ToEven);
Console.Write(ygiMExAPsVmffFeuTA.ToString());double JzpFkUsIlAfqCctMHWSWasyEw = -8.643203E-09; 
if(JzpFkUsIlAfqCctMHWSWasyEw == -1.804565E-12){
double DOwjpuZXiaWEJsXZfPZaQpbNja = 7.888168E-11;
JzpFkUsIlAfqCctMHWSWasyEw = Math.Round(DOwjpuZXiaWEJsXZfPZaQpbNja);
Console.ReadKey();
}uint gHWsXHnTJxpFhBUmyxEjTGFMTfpfg = 670143; 
float PwImRysFzZlSWV = -3.449022E-21F; 

}public void IVUngCckASqi(){
ulong Nzhse = 81617134935871498; 
short WkcTPVNYhJUGbqGawMUztPDiZq = -29753; 
double JMWKsEHnoWjaBaKjHZnsj = -1.723475E+11; 
JMWKsEHnoWjaBaKjHZnsj = Math.Truncate(JMWKsEHnoWjaBaKjHZnsj);
object xZuxzsjkCNlLpBtydSaxHtPWBg;
xZuxzsjkCNlLpBtydSaxHtPWBg = -8.686099E-23;
Console.WriteLine(xZuxzsjkCNlLpBtydSaxHtPWBg.ToString().ToLower());short qsnJmfMIbmkawmjtldjyV = -10828; 
byte uCVzFzjteuBIhXfJjTfRZQFGDNGmQ =  230; 
ulong epoMkncDdSVbMClnXBnclxYEXoWRS = 2482122295054484; 
ushort WQDOhUDTaCsooIM = 50322; 
long LPVqkLgnMxk = 30362402735423301; 
long KuxVpBIoDTNUBYGDFFSHUifJb = 82731824491451813; 
uint uAx = 195637883; 
uint iEyeTHYBMIRbWBnNTiNWclsJUaWkc = 68; 
long VZDkXng = 62666180144216788; 
sbyte EMyDWWkPjTKNyFYiSaCMDV =  103; 
int qJBwVwx = 32; 
if(qJBwVwx == 207138){
qJBwVwx = qJBwVwx + 622260;
}double qwNw = -2.650929E-38; 
if(qwNw == 1.248394E+30){
qwNw = Math.Ceiling(Math.Sinh(-5));
object zKgWhumtOGbzYitiZUfTCAadAp;
zKgWhumtOGbzYitiZUfTCAadAp = 2.480089E-14;
Console.WriteLine(zKgWhumtOGbzYitiZUfTCAadAp.ToString().ToLower());
}float LlWqYKcyROaPiBYZttwiX = -59.68997F; 
float QlZPPFyLWuREnRnXGbQZUCmfL = -3.575098E+11F; 
ushort MKLbziydJciEemghweDhks = 95; 
long YnYTdGXdZkLgCPJQTWZHxPngYoe = 58292983925158725; 
float kGxlAecxbUoTssw = -5.989094E-33F; 
uint aSUOCRS = 813580; 
string uoPqJq = "uXKlKXwPlThZJVJsU"; 
int RVwHGlwUBad = 67; 
while(RVwHGlwUBad == 67){
RVwHGlwUBad = 531464;
}uint hXtMFZsRskLWVKsAHZpeaoc = 777092107; 
sbyte zVklgUfAtzjQHnucKPeiqqERK =  -74; 
ushort SUzXxhTWhYFcPkYbyjKRGlt = 7104; 
long BhqtDWBsHAaltAuzVLkbtgtDuBCCh = 42308085701252242; 
ulong ICxFAbkQjxhqIU = 1064566292885554; 
double ZYIYWTJT = -8.241136E-17; 
while(ZYIYWTJT == 8.734738E+30){
ZYIYWTJT = Math.Ceiling(Math.Sin(2));
try{
int unDlFBtlTqOojsshJSVmPqcTk = 1125634;
if(unDlFBtlTqOojsshJSVmPqcTk == 43800){
unDlFBtlTqOojsshJSVmPqcTk++;
}else{
unDlFBtlTqOojsshJSVmPqcTk--;
}
}catch(Exception ex){

}
}sbyte cFadkKbUwiTOL =  -119; 
byte yckYVCeSAbDAFFeWXlKypRdmKc =  242; 
double dpozl = -1.725894E-34; 
if(dpozl == -0.1502886){
dpozl = Math.Floor(0.007649408);
 bool? bsCbLNMjXhn = new bool?();
bsCbLNMjXhn = true;
}sbyte wsfSfgyjHjTpsJfXzQfmDPguk =  36; 
ulong SXheORndYuQBzkNXGFhojfNnH = 447967730670898; 
int KcOCCubZdZGP = 304; 
while(KcOCCubZdZGP == 304){
KcOCCubZdZGP = KcOCCubZdZGP + 489084;
}
}public void HMjTzSCIRnNshRSd(){
float dHUhgJnbgWFcVbYTaHAjpJZNk = 6.264582E-15F; 
long XBYoXDTGzGBFxMG = 42053593096795556; 
int CuHhXwtHEjxHOYAfmCaYGT = 383705938; 
if(CuHhXwtHEjxHOYAfmCaYGT == 370206){
CuHhXwtHEjxHOYAfmCaYGT = 602628;
}short gouExil = -2585; 
uint hXTCjyau = 2493; 
string kGfIxmzCbN = "wchljCqUPaQQ"; 
long wkl = 49046770714389384; 
uint MZZZiwGzzbkQSVtmnwQzHacm = 5; 
ushort nldzMNFVQbH = 26751; 
string WibHgzJUDIYahLWNT = "apjmYN"; 
string pyxmP = "xeeUlAWVHtKxA"; 
long UYdiAuLsKkoDbhkypTE = 21179869292563056; 
long ECmsBxAtdoaMMuKXWVNwVtEVh = 87719609016206482; 
uint JbjBMM = 23; 
int SqlsFypsnVjYqkYiDR = 98; 
if(SqlsFypsnVjYqkYiDR == 201969){
SqlsFypsnVjYqkYiDR = SqlsFypsnVjYqkYiDR + 268411;
}string NubJRgasokaEXqZXNHWLKYDDjmW = "ThgYqJKVpFdcYZWNi"; 
double YbOXEqpkORzAEkyzgecmoHdHhD = 8.758903E+17; 
YbOXEqpkORzAEkyzgecmoHdHhD = Math.Ceiling(Math.Acos(0));
try{
Console.WriteLine(YbOXEqpkORzAEkyzgecmoHdHhD.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}ulong YtMZxtqATSQwGsxAblPV = 47177663567256204; 
short CYbZ = 7102; 
int CNdFLkVaVamLweALSgcqA = 124; 
if(CNdFLkVaVamLweALSgcqA == 138089){
CNdFLkVaVamLweALSgcqA += 814803;
}short tGMRPAzMpPLglQwtsXXcnjTDn = -18998; 
byte qpEuFiDwBTGFaYDqsjmyGt =  240; 
byte nDiMiEFlMuZlpCdmZSAqsboHDk =  19; 
double gYtuCqHtJsobtnFDebIjjGBz = 1.413477; 
if(gYtuCqHtJsobtnFDebIjjGBz == -482.5615){
gYtuCqHtJsobtnFDebIjjGBz = Math.Ceiling(Math.Asin(0.2));
object UCbfsSAtPwXJhVmm;
UCbfsSAtPwXJhVmm = 2.986583E+08;
}short eebRguUbdDikWXbokyHYCGP = -6929; 
ulong LEhFjjWAsInzTUIzwan = 5727505085969044; 
short AoIdCJqUA = -8781; 
double yFtaMEMqck = 7.28296E-30; 
if(yFtaMEMqck == 3.525218E+07){
double dFnTtHwskqFVIonuCxAhX = Math.IEEERemainder(3, 4);
yFtaMEMqck = dFnTtHwskqFVIonuCxAhX;
Console.Write(yFtaMEMqck.ToString());
}double dqhELLEeFTfE = 5.815726E-29; 
dqhELLEeFTfE = Math.Ceiling(Math.Sinh(-5));
Console.ReadKey();float kAnLob = -2.750098E-38F; 
ushort LCGScAlITnoqWZHHkK = 60820; 
ushort OOtklVxnkwUOVygJZaElhND = 55647; 
float neJQPeYQWNmHRFQuI = 1.050862E-08F; 
int CAY = 6680; 
while(CAY == 6680){
CAY += 30782;
}byte qJHsGuGPQeCuxsjYx =  195; 

}public void LyqCM(){
int FgMQkUCLphXyOmlGNWaeD = 477807865; 
while(FgMQkUCLphXyOmlGNWaeD == 477807865){
FgMQkUCLphXyOmlGNWaeD = FgMQkUCLphXyOmlGNWaeD + 935244;
}uint agVNV = 487157938; 
string THS = "bbbYZSzSFFLBhKIscuYaIp"; 
string LTxN = "MdulRTysOSeclwDjl"; 
ulong RLXAIS = 54932781086937437; 
short CpnpRkZoDTYwUzSAfpVQPEMuOg = -4413; 
ushort MJqVDaTXypHMJXPT = 42157; 
float gOafQAdXETCKOGOQk = 4.901305F; 
short ZCJZoefHXCgpUmM = 14202; 
int wAdoeVGSgUgFkgAe = 26; 
while(wAdoeVGSgUgFkgAe == 26){
wAdoeVGSgUgFkgAe += 407851;
}double HswuUsbfHhe = 1.059654E-08; 
HswuUsbfHhe = Math.Sqrt(4);
object JMBlYOSbXdbHKxQFMeyMRmc;
JMBlYOSbXdbHKxQFMeyMRmc = 2.291758E-38;uint lYigKJPLUDPVwK = 63; 
float poNWWpbwfqEHntsRQ = -1.277312E-07F; 
double UWAnCDOHbhBDHciJp = -4.621831E+27; 
if(UWAnCDOHbhBDHciJp == -29423.5){
double wDETJlXawtnI = -3.80484E-30;
UWAnCDOHbhBDHciJp = Math.Round(wDETJlXawtnI ,1,MidpointRounding.ToEven);
try{
Console.WriteLine(UWAnCDOHbhBDHciJp.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}ulong UVIhG = 81996263542876831; 
ushort SOyaOSjoSFdSIOkHASEVAaz = 32212; 
ulong DFpJHwZHCBkeZlLHQEDjehA = 44026090531195735; 
double boGZiXJTa = -9.021934E+15; 
while(boGZiXJTa == -1.545441E+28){
boGZiXJTa = Math.Exp(2);
try{

}catch(Exception ex){

}
}ushort IEAiCVuelEoc = 18340; 
int qydhAmXcRllFdGQgf = 966031; 
while(qydhAmXcRllFdGQgf == 966031){
qydhAmXcRllFdGQgf += 548741;
}string uijdeXsPsitxs = "eKBJWpqpAwxjDpgjcUaOeeNYNn"; 
float MyyLOHTNQGGNceJPHVwbIhfz = -44102.14F; 
ulong WDV = 1965166129608551; 
double utyomLawYqSTShE = -8.938437E+35; 
if(utyomLawYqSTShE == -4.701585E+25){
double XsDAHRVAzFmPJjiSOFYkdP = 2.180213E+23;
utyomLawYqSTShE = Math.Floor(XsDAHRVAzFmPJjiSOFYkdP);

}double ymuaJKVSmJmQROLwozmcjqTbp = 3.742995E-37; 
if(ymuaJKVSmJmQROLwozmcjqTbp == -2.015561E-30){
ymuaJKVSmJmQROLwozmcjqTbp = Math.Exp(2);
try{
int BLkTIfusKWJPJhJiXCPGual = 4250735;
if(BLkTIfusKWJPJhJiXCPGual == 59793){
BLkTIfusKWJPJhJiXCPGual++;
}else{
BLkTIfusKWJPJhJiXCPGual--;
Console.Write(BLkTIfusKWJPJhJiXCPGual.ToString());
}
}catch(Exception ex){

}
}short ZHTMIFpxYnMZIBzAptBzcEuIii = 20928; 
byte EmDupXVWTMxWNqZMkCsnmVceQQxl =  52; 
int OgAwbjGsEEExHdDp = 4145; 
if(OgAwbjGsEEExHdDp == 623937){
OgAwbjGsEEExHdDp = OgAwbjGsEEExHdDp + 464321;
}float IjyFACJMqmJQmVXDGjPLK = -2.32196E-07F; 
string BiRnFTpkQ = "SMaScVReuwMdqyutxJkZiNx"; 
short sRyNutGxhfQFqNPSucGCyRtUkxHaQ = 6345; 
string KTKdtsZcgHj = "FPeWwhOLQQCB"; 
uint CXMFGDbAUycMzbcEuR = 2099569; 
short qLGJQuBUsjFQRc = -12436; 
uint Hqe = 642332280; 

}
} public class JJNIlwLTymeGbZXMIYafTgIjan{public void QaKkDKRWZdMoAIphDNxnDdUJgU(){
uint DaD = 97139498; 
uint SNcDWkcXJVKCyDYfyEZpNfbeIhQ = 9845; 
uint pXOzw = 193839; 
uint AflO = 369749; 
sbyte EVedZlhNMXUOuJY =  -89; 
float EKeEFndVQHUmI = -1.6852E-34F; 
short dwaqLGLJDoMJRhdWcPwuZpRad = -5852; 
uint zpGLcSUNRcTdtVbYqlZzQpFAIY = 759001860; 
int mof = 197142084; 
while(mof == 197142084){
mof = mof + 635172;
}double qfaWqaenTJ = 6.409599E-07; 
if(qfaWqaenTJ == -1.162704E-26){
qfaWqaenTJ = Math.Pow(2, 2.1);
 bool? opGakzkFEpXNzLswTdQbDDYD = new bool?();
opGakzkFEpXNzLswTdQbDDYD = true;
}short adpV = -6852; 
byte xtZfZjzllexalxMALRQJzRLmTZjw =  193; 
byte LqQK =  222; 
long AklJyFQMgGnF = 80702081613152820; 
byte ZczYcLJwMTRsqGxPtgygTFgiJXgOq =  21; 
byte eAlikACecLTwOmNZgJmDofgwWope =  149; 
float IJUnS = -4.181217E-19F; 
int FsdhLLwxjhIJRJIYsQlVjxBiHk = 87225896; 
if(FsdhLLwxjhIJRJIYsQlVjxBiHk == 605612){
FsdhLLwxjhIJRJIYsQlVjxBiHk = FsdhLLwxjhIJRJIYsQlVjxBiHk + 861162;
}string XWcZKlUTUxmw = "amoBEbKKhSVBAYCypDPFzLLW"; 
short aapDdW = -18501; 
ushort GKtcRuJPegAjntxFUyRsoDs = 11101; 
ushort JuG = 50924; 
double ekVNWAYjPW = 1.763555E+22; 
ekVNWAYjPW = Math.Sqrt(4);
Console.Write(ekVNWAYjPW.ToString());ulong ehPVfBLOBLHodoGo = 72060325424233223; 
int GQqtRDiCJKGRPINLlFauN = 590687; 
if(GQqtRDiCJKGRPINLlFauN == 255544){
GQqtRDiCJKGRPINLlFauN = 640480;
}int zBUnwVUygTegDHV = 11423610; 
if(zBUnwVUygTegDHV == 419483){
zBUnwVUygTegDHV = zBUnwVUygTegDHV + 339732;
}ushort VmX = 12915; 
byte bFVFdjiMDtNTnf =  186; 
float jPJklCYhfe = -4.875923E+34F; 
string LIafojCAxMYEtWD = "noZY"; 
ulong DlPcWyYeKNCUbk = 59382130182186457; 
byte ibGcRjYcMofOplmeBsckb =  122; 
float QnSolcdGZgU = 5.050593E+26F; 
double AOziNfALkz = 1.056631E-24; 
if(AOziNfALkz == 5.982953E-37){
AOziNfALkz = Math.Pow(5, 2);
for( ; ;) {
Console.WriteLine(-2.095623E-21);
}
}float xGWsFd = -5.607898E+36F; 

}public void WaKT(){
byte iPQKgEIUuutWRIGFcOLhmgtu =  108; 
float CNKzkuJqRUQNDOLDXkgDsysX = 8.694183E-26F; 
sbyte cxxnoPghKWiDhpPKlb =  -87; 
ushort EwiooxewMDbofnwzD = 30485; 
ulong DSacFwbIIMUaMazanZZs = 52066776789201210; 
ushort KEwMZMugL = 58765; 
ushort NaIbDfPukDAuFQ = 59268; 
int qtZoxbqFZbNGgneXGZWNfLWmZbg = 3; 
while(qtZoxbqFZbNGgneXGZWNfLWmZbg == 3){
qtZoxbqFZbNGgneXGZWNfLWmZbg += 772038;
}byte CwhpYpxEdbPdo =  100; 
ushort QYTPHyNowVmESSXSVUEDzJYxKgSkk = 62327; 
float FhtSUxNeISVddTLleWiuR = 12.16449F; 
sbyte yfA =  22; 
uint ydEYK = 92464; 
double hAUHOUtRwHfnPyS = 5.904465E+10; 
if(hAUHOUtRwHfnPyS == 4.203946E-05){
hAUHOUtRwHfnPyS = Math.Ceiling(Math.Asin(0.2));
int? sZUtfeodYxR = 2130299;
sZUtfeodYxR += 49934;
}ulong SMsbjCKWPwOHYzNABssjbdl = 34267969178830393; 
byte oxBhgLzfWBzeowwtP =  150; 
float CMOAhJ = 4.329708E-07F; 
ushort laEfKclquIUee = 36697; 
ulong dnJXOY = 86300595322631799; 
byte MEo =  117; 
uint Mtgykjmy = 20587069; 
ulong GlO = 10216009469110372; 
byte WzTtBnpFTCp =  205; 
short sRCVyoZSYmoXaZcdGYWgoMt = -27517; 
sbyte xxMLzHoAgIFkWVsdAHeDmqDQWqTj =  -6; 
int yOCTEfmOIICSuzu = 3; 
if(yOCTEfmOIICSuzu == 235352){
yOCTEfmOIICSuzu += 743101;
}double JfUWOhNfaCjgywbPXl = 5.502568E-06; 
JfUWOhNfaCjgywbPXl = Math.Ceiling(Math.Sin(2));
try{
int BqksXIZZnZUEJWHMFm = 8908252;
if(BqksXIZZnZUEJWHMFm == 52123){
BqksXIZZnZUEJWHMFm++;
}else{
BqksXIZZnZUEJWHMFm--;
Console.Write(BqksXIZZnZUEJWHMFm.ToString());
}
}catch(Exception ex){

}double XFlNZS = 5.074664E-26; 
XFlNZS = Math.Exp(2);
Console.Write(XFlNZS.ToString());uint MqLhARkAFnNMXMmHyWCu = 13351; 
sbyte aHVAFgyWbzQixcttbiENROYSNOHX =  -17; 
string LiOhSVLV = "QRKzVdkwDitnywf"; 
byte dNQAAAXofaIUBypnfWad =  124; 
double ElwnLWnNJVkSXfYqqMjQd = 0.7440498; 
if(ElwnLWnNJVkSXfYqqMjQd == 1.586156E-34){
ElwnLWnNJVkSXfYqqMjQd = Math.Ceiling(Math.Asin(0.2));
try{

}catch(Exception ex){

}
}double lnTAoysCSyjOzj = 0.001052151; 
lnTAoysCSyjOzj = Math.Sqrt(4);
try{
Console.WriteLine(lnTAoysCSyjOzj.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}string WqVjlND = "RkX"; 

}public void XIUNxpmfRkQYqgeUwKSbwI(){
ulong zVXVOjPfsuiANE = 31375453667634253; 
float JJFgs = 2.402578E-14F; 
short AoBsssIwKGXXFhRnesBqeUCdS = -16890; 
sbyte JOxcTInnUwqEJoSpUh =  -113; 
uint SmFQYNGNyAfmdNViUeXTEuK = 93; 
long bkjLjFRQBSmUUJByVlpFCWhZ = 63219428938386731; 
short EDnPWIlXwqIjdkcXOOHdnfdjIdXH = -10193; 
double tFXfYigoIt = 5.920697; 
tFXfYigoIt = Math.Floor(-8.86336E-07);
object AYLJFmTPcCzQgzgtnIBeZ;
AYLJFmTPcCzQgzgtnIBeZ = -5.926165E-34;
Console.WriteLine(AYLJFmTPcCzQgzgtnIBeZ.ToString().ToLower());byte QjgOqEhlSzEJ =  165; 
string XKD = "QBgludZhSSnPoUI"; 
long HkMkUbcSCeBomEqpMuRJYNL = 20654284992373197; 
string nqzQnWAdtSYzT = "nXADAQYPTAtPFIzimoB"; 
float lSAzzsGFVMTCKfIjMojxzzYxAqRGk = 2.63548E+13F; 
double HceZebzskYNBmVCQZCapyAOUcYhBa = -8.235341E-16; 
if(HceZebzskYNBmVCQZCapyAOUcYhBa == 52001.36){
HceZebzskYNBmVCQZCapyAOUcYhBa = Math.Ceiling(Math.Sinh(-5));
Console.WriteLine(HceZebzskYNBmVCQZCapyAOUcYhBa.ToString());
}float tnCsPMySBpCCSCxjVehWaBPs = -1.958995E+17F; 
sbyte wpuIYIUBsuPHOwiQkjtK =  59; 
string gUZPSzjjdGQxjWU = "jIWSBpBhFsZhYsxVsS"; 
byte ZTbbikLaXYYTqHjcQkYQuRk =  128; 
uint lLj = 90189181; 
short gFqGUJpFAjGtgRsitfgUx = -8432; 
sbyte CDgYaQdUHjjbTxWDIlVH =  26; 
sbyte kUDRPYzmNxJe =  -41; 
uint FJgLMXHpoWiF = 2091; 
uint cuiWdyZoWiWyGksFa = 6285; 
byte UuxEEmUGtlcoUHKi =  156; 
byte JSPEbj =  179; 
string FFWFlbhaNsBjeJDXTi = "EIwucADIfRwuTipaPozcTz"; 
long nOWFjlGhbZBWjk = 13827506034617898; 
int OIO = 40011973; 
while(OIO == 40011973){
OIO = OIO + 964914;
}string oDYzTk = "tUIuRFwut"; 
uint CdpgVQCzYNVhwY = 8; 
int LViXeFNuuQjlaNsKy = 824562886; 
while(LViXeFNuuQjlaNsKy == 824562886){
LViXeFNuuQjlaNsKy = 937471;
}ushort ByMmxIBQS = 43033; 
int EYLuCxMoM = 16982438; 
if(EYLuCxMoM == 522683){
EYLuCxMoM = EYLuCxMoM + 241497;
}byte TDYOLFgagYhJxie =  199; 

}public void TYfNgMkgBOoXCU(){
ushort sYeGzTuFuu = 29790; 
int pEeanIngKNRNIjQRcGd = 40647398; 
while(pEeanIngKNRNIjQRcGd == 40647398){
pEeanIngKNRNIjQRcGd += 176037;
}double ECtNnIBjkkXmmhgIlnSRJENqOuSkl = -4.761216E-32; 
double DfgZG = 1.067523E+08;
ECtNnIBjkkXmmhgIlnSRJENqOuSkl = Math.Ceiling(DfgZG);
try{

}catch(Exception ex){

}ulong ZwlhOeMaFxIbJRDqD = 8154257043124617; 
long jcxU = 82740585961609492; 
long tVNKIhxAqOxUtPtIZwdbC = 28737727236557964; 
string lwLtLbDeDHLLJyO = "PTsqKeKiwMkLPcSWqpkzJxIG"; 
ushort CwQmBelcpLj = 44038; 
ulong MCXYRS = 22351256052627285; 
sbyte lMVuEosilfzN =  60; 
sbyte IDdNhzJPFabHcThSWVXAzO =  -36; 
int iPNLtDNIbMQiDZ = 553980458; 
if(iPNLtDNIbMQiDZ == 383796){
iPNLtDNIbMQiDZ += 534573;
}long pdnCRPInUjA = 38021025379038199; 
double gsK = -2.945849E+32; 
if(gsK == 220.9042){
gsK = Math.Pow(2, 2.1);
try{
int LMxkyDeinHtNoZnoNbhVSpJ = 3966240;
if(LMxkyDeinHtNoZnoNbhVSpJ == 20669){
LMxkyDeinHtNoZnoNbhVSpJ++;
}else{
LMxkyDeinHtNoZnoNbhVSpJ--;
Console.Write(LMxkyDeinHtNoZnoNbhVSpJ.ToString());
}
}catch(Exception ex){

}
}long btlNiYJaTYDZZTwdfhKyBSpNJXc = 39936870120092229; 
double EUPxEyYsKQuTXp = -66.0487; 
if(EUPxEyYsKQuTXp == -1.433333E-25){
EUPxEyYsKQuTXp = Math.Pow(double.NegativeInfinity, 2);
try{
Console.WriteLine(EUPxEyYsKQuTXp.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}uint XZPRpq = 957585; 
ulong IaeSiT = 68859521520463662; 
float WRINwHKtSNpnNX = 64.9497F; 
int OppEuQHXxgQUh = 977511; 
if(OppEuQHXxgQUh == 625561){
OppEuQHXxgQUh = 302549;
}string bpMaxEgTHOPyxxDfsxSfyj = "DXFLoTDalCKMZEDYmB"; 
sbyte sbxLhxOcjyXmRNjcRSRZeODW =  63; 
long DPzFGswkBBUESeBQO = 52252942271969863; 
long LFzBX = 17325084707798052; 
ushort JDXoYVEYPsqPoioqj = 62175; 
byte HcZWeYSndVlCzLKYjgKjg =  156; 
long HKGcXwJ = 64990626841629301; 
string pUTf = "pEzqhdndEasRowsQpJlSTMQd"; 
ushort WJgQMRc = 53900; 
ushort EGm = 39960; 
sbyte iUoLMPJmZkHnWFOwVZ =  -96; 
int NoCOzcqmLwHLpEVOKwhaTQBjPUPn = 69; 
while(NoCOzcqmLwHLpEVOKwhaTQBjPUPn == 69){
NoCOzcqmLwHLpEVOKwhaTQBjPUPn = 609968;
}ushort OXnNqaoQaDGaSHkEYFe = 61168; 
ulong YEmceahhiLho = 32905085201094053; 
sbyte nOwCAKBXC =  -5; 

}public void xYKAoWoITtVTEGzxEVlwdiMpz(){
long CzHliJzFOzWuWlcpUs = 59284637452740262; 
string mmtaHtOjlhNjfaKXIBojaSR = "EQIBkKiTFKsBeVNEyTeNGLMUppoC"; 
string NwwxyRLnYCYTFwJiTMOLMgNesS = "OjVu"; 
long LXWfJzWTjVDONfqkgxEOohGIpe = 23174065217916515; 
float NdjnSSDljqFMMEd = -1.038503E+24F; 
float EqnPFThWgSxyNlTzdm = -3.352831E+31F; 
ushort tyZASbtaFpywpgznGu = 45492; 
double uOUSBTfZlCYhZX = 3.858544E+23; 
if(uOUSBTfZlCYhZX == -6.076775E-30){
double ZsWNZPH = 3.508125E-30;
uOUSBTfZlCYhZX = Math.Round(ZsWNZPH ,1,MidpointRounding.ToEven);
object ZJeIsinas;
ZJeIsinas = 6.718353E-22;
}long JXpfVqBeFFqqgBI = 70534255396100799; 
sbyte NgNIBjBNcLHcjlquwZCOUzge =  -17; 
double gBpUU = -4.04438E-14; 
if(gBpUU == -1.728038E+20){
gBpUU = Math.Ceiling(Math.Acos(0));

}byte oxqqZnmQEEjOStyF =  220; 
long VnHXCjKugyjWAMuTBY = 48531364777215499; 
ulong ZpRaADqlJA = 79081806653805679; 
long yBWG = 89228755886208250; 
ushort uRGixXuhjaiXaW = 61980; 
string lQMQAukmgQAxXwPMU = "oNjlNIopWyQtRVMj"; 
float JGVywWpg = 9.438754E+17F; 
byte ulgxJqqZddBjyOxNZEL =  189; 
int dnFSsnWHNEcqcFmumu = 855412336; 
while(dnFSsnWHNEcqcFmumu == 855412336){
dnFSsnWHNEcqcFmumu = 720323;
}double BDgqbBcQiSRQYddyqXgg = 1.638778E+25; 
while(BDgqbBcQiSRQYddyqXgg == 2.255823E-14){
double ziTSLNtAwCComxuD = Math.IEEERemainder(3, 4);
BDgqbBcQiSRQYddyqXgg = ziTSLNtAwCComxuD;
object KmiFSWJcDaLHqtKJXXnqLYVw;
KmiFSWJcDaLHqtKJXXnqLYVw = -3.435308E-08;
Console.WriteLine(KmiFSWJcDaLHqtKJXXnqLYVw.ToString().ToLower());
}float MGzhzdMqNZZznXBfQDwGzJ = 1.356218E+34F; 
uint tbBFLVFJu = 430667; 
ulong SbTmpapPuxoasCnncXC = 21410358286529539; 
float DdJdWDEFwsWizZFkFtt = -4.715325E-08F; 
short VewiqcebVxkRtHXw = 31132; 
long HFFANKfGFVMFjlEOYCumC = 23552546728696027; 
long uFMKVqGIzkxAfjSaFPSYTEygpmEN = 85849098494381540; 
ushort JxsUWbfRkRUmEwMJd = 53738; 
sbyte tIOShWpwDhTTEcgwGZH =  -22; 
string cNTmXAooUBRSibJB = "aMKndXaJBoqcejMDUFbYVZ"; 
ulong nmtbLU = 6825528657884315; 
double TExjYlPqfonxICRaVEaQFYYH = -1273223; 
TExjYlPqfonxICRaVEaQFYYH = Math.Ceiling(Math.Tanh(0.1));
try{
int QZuoXPdeFQwwBzowYsD = 9197993;
if(QZuoXPdeFQwwBzowYsD == 26613){
QZuoXPdeFQwwBzowYsD++;
}else{
QZuoXPdeFQwwBzowYsD--;
Console.Write(QZuoXPdeFQwwBzowYsD.ToString());
}
}catch(Exception ex){

}ushort yblMNHOMBmSqbSJ = 10319; 
float ztlRISb = 0.004656825F; 

}
} public class WBTQIgttjOMJHmRojmZJQkCOPcm{public void uuKXhUPaucJUAoXgxQN(){
double sIP = 1.871332E+23; 
if(sIP == 2.478631E+36){
sIP = Math.Ceiling(Math.Atan(-5));

}ushort GfBewkftKLoLwFlLxLnEyCZO = 24422; 
int dTaffhwRuKHl = 3874; 
while(dTaffhwRuKHl == 3874){
dTaffhwRuKHl += 4925;
}double tSylzIQqefMLPTcAKFjEaN = 2.64447E-37; 
tSylzIQqefMLPTcAKFjEaN = Math.Ceiling(Math.Cosh(5));
 bool? ZxxYLHeHyYmUTRdVXMmW = new bool?();
ZxxYLHeHyYmUTRdVXMmW = true;string XfjtkZsJNKZgQZICFCIAPeeOGwt = "VlPEDCagLAGdDoYDX"; 
float dmCTpTFgTbwOQGRNCVwEnYBQ = -3.240961E+31F; 
int AfDQLHootszLYwhfoMPyLCyICpBf = 71639837; 
if(AfDQLHootszLYwhfoMPyLCyICpBf == 281162){
AfDQLHootszLYwhfoMPyLCyICpBf = 648164;
}long zFcjqDuixLGTRuCtfglXJVo = 44251483441797734; 
short wIKJGgihnkQLIosjuohxKdE = -12086; 
long hZMF = 37763016456803833; 
uint cQw = 46286201; 
long UjzjPnUdqeuaDYj = 24581347835461290; 
ushort PhtUITSYGmPmwEbubeIyL = 63006; 
string tMRXnVbFeBZfBjJlCbWMuinoAb = "PXWYRgBBWbVLKlki"; 
float WURwxVJbpzkzDymumI = -83.98967F; 
sbyte ZKNjHqmAjatLEgKtBapPQcpzzyzoY =  14; 
double wWUhImoqPQoje = 1.779563E+30; 
if(wWUhImoqPQoje == 1.714303E+28){
double iXtoMiTXCHaZpH = -9.551735E-27;
wWUhImoqPQoje = Math.Round(iXtoMiTXCHaZpH);
 bool? MZaaNftHDtE = new bool?();
MZaaNftHDtE = true;
}sbyte RoIieDnSjhTxlti =  -79; 
byte fGBtxkjRnTpNtNGCiLfXodoBuXE =  101; 
double FNBxxAXzTyZmnuDxOfRK = -0.0001485505; 
if(FNBxxAXzTyZmnuDxOfRK == -2.525646E+10){
FNBxxAXzTyZmnuDxOfRK = Math.Pow(double.MinValue, double.MaxValue);
Console.Write(FNBxxAXzTyZmnuDxOfRK.ToString());
}double CXfhKSPAngGFXfGelXBsCi = 1.523582E+13; 
if(CXfhKSPAngGFXfGelXBsCi == 3.562163E+22){
double kOXGTSHJQdHLwG = Math.Log(1000, 10);
CXfhKSPAngGFXfGelXBsCi = kOXGTSHJQdHLwG;

}double jdXNObWClehOmCx = 1.594542E+34; 
double kBwDHTwsnUhsZYo = -5.428051E+12;
jdXNObWClehOmCx = Math.Round(kBwDHTwsnUhsZYo);
int[] pJmmkQVqWQcnCTCCmH = { 1368557 , 89104 } ;
Random VsJPAttdLZzUbk= new Random();
Console.WriteLine(pJmmkQVqWQcnCTCCmH[VsJPAttdLZzUbk.Next(0,2)]);ulong mjMLCzCkGHAyxcXxeiHTNURMkszWa = 18272549033219729; 
byte knnNNFmDxWGfYmTltsZBjaGjC =  61; 
uint TcBPUsxGzgehXPcZwYxL = 340561375; 
int IKIkJ = 22; 
if(IKIkJ == 599824){
IKIkJ += 683137;
}ushort RkGRnNmmSDuYGP = 37859; 
ulong MCyugkLlXuVEesfJDFNQUIFt = 37715946043250253; 
long ThWbKaVFp = 3003478076581341; 
byte aVeWdDqeyZUfnTRAXNBVNUosl =  8; 
sbyte VYXZ =  -103; 
ulong sOkSAtWyGxmdQWMpCDoef = 76923295135580041; 
string ayjALNVbCRxKVYXsgjIwyQMEMbB = "eeNYEDmHHxaEHCOmJVRMechY"; 
float yjMHOJlfWydzgtMqMKi = -6.350196E+22F; 
double zUQk = 2.011558E+16; 
if(zUQk == 1.654572E+22){
zUQk = Math.Pow(double.NegativeInfinity, 2);
Console.WriteLine(zUQk.ToString());
}
}public void nJxMbRNo(){
short nJbMOYLAZwfFVImLUUfKQW = 26750; 
int spFwYaDi = 977121; 
while(spFwYaDi == 977121){
spFwYaDi = spFwYaDi + 684955;
}ushort bnUeJNPz = 21967; 
double iJVsOyCxIfdbwEwzJjOijKAPIa = -2.249827; 
while(iJVsOyCxIfdbwEwzJjOijKAPIa == 1.694271E-30){
iJVsOyCxIfdbwEwzJjOijKAPIa = Math.Pow(5, 2);
try{
Console.WriteLine(iJVsOyCxIfdbwEwzJjOijKAPIa.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}uint mpuSCnAUquVDBjwHfYmlqtpdwmPLE = 28755904; 
short ZNCNgzMOp = 4439; 
int qPPCoo = 159386697; 
while(qPPCoo == 159386697){
qPPCoo = 995853;
}short nAOqDeGpCyTUYB = 1187; 
uint PMX = 46; 
double oAwhYqSD = -1.591754E+14; 
if(oAwhYqSD == -8.751096E+37){
oAwhYqSD = Math.Ceiling(Math.Asin(0.2));
Console.ReadLine();
}short NwWXQcJdNpXcd = -8785; 
short pbXpGRciVHYkARduXVB = -29525; 
string xtugUkWgYgQeylfaPWtOcieyF = "mDehSoBzGFWAWRlOIRwwtRoXgFJp"; 
ulong nWCYYtbRoB = 18426565698564115; 
uint BnciDxmhbm = 20289544; 
long pZXYTTNRCURu = 51309465847644280; 
ulong Utw = 2584114506407695; 
short fsaKJCKM = -22419; 
short lAokHAx = -23734; 
int eFDqNSbAaz = 89; 
if(eFDqNSbAaz == 463884){
eFDqNSbAaz = eFDqNSbAaz + 595880;
}ushort adoMb = 15693; 
short GFuuXjKWaOzsWJIlhTfq = 28203; 
uint cdpgCatDpbflxeGCTkHNKg = 17; 
short cEhgYLeSsqHch = 27239; 
int hqRylgCq = 1187; 
while(hqRylgCq == 1187){
hqRylgCq = hqRylgCq + 908707;
}double QgXPwj = -5.127313E-08; 
if(QgXPwj == 1.407526E+18){
double LfzZNs = -4.320457E+30;
QgXPwj = LfzZNs * 2;
try{
int QTSwpQ = 581143;
if(QTSwpQ == 58238){
QTSwpQ++;
}else{
QTSwpQ--;
Console.Write(QTSwpQ.ToString());
}
}catch(Exception ex){

}
}long VlQWXbNFqlZSOoXxEc = 28779558499262923; 
uint uxPRIkCfJpdWelHbxxTepaSa = 373962973; 
double NTqwAXkUxikEORNmYQkwwzAiJy = -2.325597E-12; 
while(NTqwAXkUxikEORNmYQkwwzAiJy == 4.57693E+37){
NTqwAXkUxikEORNmYQkwwzAiJy = Math.Ceiling(Math.Tan(1));

}byte sgbfYMZeMjewRVwJkxWdnajapqMH =  24; 
sbyte wgkuVxLqXjSDXuCxwYwjmXjJ =  -43; 
double PIZ = -7.21986E-39; 
while(PIZ == -3.765633E+12){
PIZ = Math.Ceiling(Math.Sin(2));

}double ewjBntBHnjobVXniC = -1.656323E+14; 
while(ewjBntBHnjobVXniC == 3.695084E+15){
ewjBntBHnjobVXniC = Math.Ceiling(Math.Acos(0));
Console.ReadLine();
}double Tnqk = 6.291235E-23; 
Tnqk = Math.Sqrt(4);
for( ; ;) {
Console.WriteLine(58317.81);
}long EiUFNIgdJnqBWHRXQltdUBPy = 85110390269008426; 

}public void TlBUiGGcwJYOICQXISRkE(){
float uxwM = 1.742338E+28F; 
double ngRdwqcIHOaARRJDAqpwYiaeY = -1.692336E-29; 
while(ngRdwqcIHOaARRJDAqpwYiaeY == 8.216234E+28){
ngRdwqcIHOaARRJDAqpwYiaeY = Math.Ceiling(Math.Cos(2));
for( ; ;) {
Console.WriteLine(-6.863839E-24);
}
}uint BpHUZfPl = 300; 
uint IIOnQxCQRmKhYOy = 71084800; 
string zaUTSPfsqYzekUmzDAHYKFMtm = "AVkIiCiidzOOWzKejRsi"; 
uint LoUgeeiTnB = 21100591; 
short gKQXVbQP = 13544; 
ulong hoFuUbeiljWNbyeaj = 6210797575560654; 
string nxyWFfFWQEAuA = "ceajWOBRbGFyHRZQH"; 
float HQMOnsHyBnxLQOHJdzOEnUywRq = -0.3686755F; 
ulong aXGBTLppMSpqaDFVPKZhWqSpGNtq = 15035518561212372; 
sbyte VWtNMWnXTU =  80; 
double yBHbKeoVgqZtQHiIy = -1.199245E-30; 
yBHbKeoVgqZtQHiIy = Math.Floor(-4.127561E+21);
try{

}catch(Exception ex){

}sbyte zwO =  41; 
int sxUQVsfjkJzjEgCMTcBzDLBRYgWJ = 433078868; 
while(sxUQVsfjkJzjEgCMTcBzDLBRYgWJ == 433078868){
sxUQVsfjkJzjEgCMTcBzDLBRYgWJ = 291510;
}float XKxdNsumnCwZzMCLhguIWdROw = -5.184146E-23F; 
ulong cHnlLOjHyMkpnDyMXj = 84022359179483417; 
float IJpgHIlVgOOXseKwjqIBucymZT = -3.279085E+20F; 
byte YQUqYPwdKGqCF =  47; 
ushort DnFbLVluzG = 13623; 
string NxyMNP = "dagVZfIbjSznRKqXjhKcEASsFtWW"; 
byte QSuzkChJbFmkkHhXlyxAnEdNuE =  40; 
ulong HiMSpbHTsAP = 39634363526419112; 
byte gAALFmOIeIttsuwCtUFC =  15; 
float IJNxYXanWDJBKlgLPSlnSwxhMKnaw = 3.80908E+13F; 
int CSbSQQKdt = 818320; 
if(CSbSQQKdt == 718268){
CSbSQQKdt = CSbSQQKdt + 306239;
}byte PKRGglm =  154; 
long KaToTemitf = 59903504578562713; 
uint YXOlSZNHuGPleCadAhXFHDWgp = 66018103; 
short InIgSpMlHyUqHO = 17570; 
short UACgYRJUkxbIWcqzkNqh = -25797; 
byte nRSKdmaOh =  206; 
long iNoJyYmIULELwDTwbxiCImnofgG = 34271274219637286; 
long dUiSFsEmhnBuGNMHhmVAZDptGymS = 31548233725498819; 
string cVKcYSemGUgkgUmlWCEQm = "fMBKagl"; 

}public void QjHzja(){
ulong FzosRsLCqugVDxYkwVQYCnSXaAeX = 64083553496940768; 
uint pNaZtHapEtkxcojQxpGqssnNYK = 88451308; 
uint ATKCFdXDUlXKVLNgFw = 61; 
int AjDYAnaKXgRaUqNCLgS = 416042821; 
while(AjDYAnaKXgRaUqNCLgS == 416042821){
AjDYAnaKXgRaUqNCLgS = 448003;
}int Sen = 859914713; 
if(Sen == 382297){
Sen = Sen + 501037;
}double FKCkoRSUQwtdLWOKkfP = 24.99701; 
double baXmCeDiskeEPDNGzaDUepJnLefJx = 2.888914E+13;
FKCkoRSUQwtdLWOKkfP = Math.Floor(baXmCeDiskeEPDNGzaDUepJnLefJx);
for( ; ;) {
Console.WriteLine(-7.665314E+07);
}double AZJmAahAPIazgZhy = 1128370; 
if(AZJmAahAPIazgZhy == 0.6262212){
AZJmAahAPIazgZhy = Math.Ceiling(Math.Cos(2));

}short MclLxV = 11365; 
long lbDiDiKjntQssIO = 25669858931830299; 
double ZdtaDYfwXzMdPFAtACo = 1.631351E+18; 
while(ZdtaDYfwXzMdPFAtACo == -0.0007872778){
ZdtaDYfwXzMdPFAtACo = Math.Sqrt(4);
int[] uODJipx = { 5327454 , 62415 } ;
Random GJUqZcLJIXpylbwcpQVQQZlxKy= new Random();
Console.WriteLine(uODJipx[GJUqZcLJIXpylbwcpQVQQZlxKy.Next(0,2)]);
}sbyte sfiLXwwRMsYnNlMCTstB =  -36; 
ushort GcGyzIDBeBaFj = 18995; 
uint ZHzeD = 537427723; 
byte KiEimJnYLDJqYXjiRUOMIDlsnuTUo =  217; 
ulong tzPzZqSRMGAMTObK = 73500426548574641; 
double VTPuKSPh = 0.8948758; 
if(VTPuKSPh == 6.836194E-25){
VTPuKSPh = Math.Sqrt(4);
for( ; ;) {
Console.WriteLine(-1.136926E-25);
}
}long xDPKR = 83672614743774063; 
int bbzwE = 90960575; 
if(bbzwE == 739528){
bbzwE += 155379;
}string piSWSlJVXLJZNK = "dBLEXdubFAeABDAXKGHqNcZBF"; 
float eqQxRVeV = 1.715356E-36F; 
int sVhmx = 72568877; 
while(sVhmx == 72568877){
sVhmx = 746386;
}double gsdbBjZcTpKPc = -4107425; 
while(gsdbBjZcTpKPc == 5.188752E+15){
gsdbBjZcTpKPc = Math.Ceiling(Math.Sin(2));
object UnRGb;
UnRGb = -4.011294E+15;
}sbyte bJeZaDbwNxe =  -6; 
uint RhsjlMahVOVjSGCJquALxWtz = 9637; 
sbyte XoNwtbfqTNl =  -26; 
short OqdQKwTefKZOtUPUQmHJjGbBRR = 1117; 
byte EheuDibwEnKzTjcxFJkDYXYO =  161; 
long pEZTVJn = 76785634455749561; 
byte XkqwZdlhpISHjpOTaMSFuOPFIBCkS =  120; 
ulong KYRwtMqANMFaxVsF = 77340333029583437; 
ushort NzDapSRsWkzsAnwsRJjPiN = 10266; 
double UNPSbtJZIZSWNwNidYg = -1.316572E-18; 
while(UNPSbtJZIZSWNwNidYg == -3.379388E+07){
UNPSbtJZIZSWNwNidYg = Math.Sqrt(4);
Console.Write(UNPSbtJZIZSWNwNidYg.ToString());
}string NnOzC = "MAwQigLwNiqHPlBmZVmIpTl"; 
double RaNawNEWyEpJjFMlpJWJgCQFB = -3.558261E+28; 
if(RaNawNEWyEpJjFMlpJWJgCQFB == 2.89268E-06){
RaNawNEWyEpJjFMlpJWJgCQFB = Math.Ceiling(Math.Asin(0.2));

}byte XOW =  49; 

}public void xHoXhtFdVheTOtnfb(){
ulong JqHFGddZYlIAYgYzQ = 14519282764391906; 
short kRYVWZuCkPbKUeuHSmIfNPV = -2205; 
byte EYPSRonoTiZpMBFGOJOkqhjmG =  139; 
long bwVBKfxw = 13170531734504323; 
int BhCgblKZAwPDloHdn = 247740; 
while(BhCgblKZAwPDloHdn == 247740){
BhCgblKZAwPDloHdn = 80031;
}byte wLbQMWYHdbBKUtKQKGaHjgJfsp =  192; 
double VTdqmHB = 1.127963E-24; 
double tueEJWbd = 1.087975E-26;
VTdqmHB = Math.Round(tueEJWbd ,1,MidpointRounding.ToEven);
try{
Console.WriteLine(VTdqmHB.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}float YwYYWJwAZS = -2.351379E+21F; 
uint zNMspQyaeKaM = 3903; 
uint jCMxpHWzABFYip = 578163; 
sbyte LuOAlaQYqoXjq =  -124; 
double IzbfcRQnTQkzROnyHX = 1.841275E-29; 
while(IzbfcRQnTQkzROnyHX == 5.904103E-15){
double IBojFaGJYCZq = 3.765454E+09;
IzbfcRQnTQkzROnyHX = Math.Ceiling(IBojFaGJYCZq);
Console.ReadLine();
}ulong hLMRkBGsw = 13820788169037021; 
int NCoMSmwjBNCNTQtpBuzKgayhwde = 42929533; 
while(NCoMSmwjBNCNTQtpBuzKgayhwde == 42929533){
NCoMSmwjBNCNTQtpBuzKgayhwde += 382695;
}ulong LTZT = 71989059884204053; 
string DwcmWPNegqOhKNlxHtcMIyCB = "TPUSnwVeVTTnIyapHMIewfETimH"; 
sbyte hfUCscaSmS =  105; 
float PTti = 1.516585E+32F; 
string bLTwAOlbJbOQRN = "YMpuazuTNlqpKAaoLcSgwbf"; 
float PQERhcmcfw = -6.737854E+11F; 
uint XHRg = 15; 
ushort DJNcOYGtWHyfkIRcG = 11222; 
string FeFHjZVAEV = "LRmnUeeooxMzQkwRpQPBadjXyef"; 
float dgQbKMnYtWXBTQSSGPwOiFK = 4.152487E-09F; 
float IpRQYlCkpWyfzhTWTRah = -8.976702E-13F; 
sbyte pLeFzxbHPeHIDgYosFCViKWHo =  42; 
float uOuplYAmhWMTVQpuCltMSLPIQiJH = 0.0002822969F; 
short GembLpxEIMhwTIastDfdoeeJ = 30350; 
ulong coTzKHWnREnJkQQe = 79218773341879037; 
byte lBCaEdYQtnOm =  117; 
ulong uiPfTUnAZXeFXsTYGlqwxNRZkLu = 62023645277096928; 
string htyXaCcAAbzBpibgu = "IsPViBSYJdZZdFTx"; 
uint UywkOBLKWWlhSZkufINmkXecT = 494785229; 
uint THTLjmWdxFojAFWMBkaOKAkKkHWs = 3487; 
ulong kMc = 98830878207909; 

}
} public class ydMti{public void sPAMnuxjADp(){
int eQoZNCHucDmAtQglhUEjcZ = 517142512; 
while(eQoZNCHucDmAtQglhUEjcZ == 517142512){
eQoZNCHucDmAtQglhUEjcZ = 866946;
}sbyte fyMGFnidYRDWollmUEoYsJMtJeRO =  -1; 
short OmeyQOOkTddVMIgKNolDeEjGyL = -10106; 
double xuEJYOKjPwAgsxUo = 5.728367E-39; 
double OjcAZfwRLxJAlNTyAYTNxMh = -0.05263031;
xuEJYOKjPwAgsxUo = Math.Round(OjcAZfwRLxJAlNTyAYTNxMh ,1,MidpointRounding.ToEven);
 bool? GwfsVpWkFHLPpdoIZlPBmT = new bool?();
GwfsVpWkFHLPpdoIZlPBmT = true;short QxGdNipDoQPiD = 12323; 
sbyte hcCjz =  63; 
short loQGiCTD = -349; 
double SbWAGjLEnf = 3.446076E-34; 
SbWAGjLEnf = Math.Ceiling(Math.Tan(1));
 bool? lJIxoclEVaNibfolZykIiWuYkMHe = new bool?();
lJIxoclEVaNibfolZykIiWuYkMHe = true;ushort TVJgLeiTenCku = 15918; 
byte PpnJk =  144; 
ushort CJBysL = 16017; 
string FEpZblpPsHsDQSmgMDEZNjpzC = "bFMCubBuQJUgNeYTTWd"; 
int QmpHhKbeunRyxjLdRmqbxc = 251533; 
if(QmpHhKbeunRyxjLdRmqbxc == 483438){
QmpHhKbeunRyxjLdRmqbxc = QmpHhKbeunRyxjLdRmqbxc + 19640;
}float twXTksxUlsA = -1.843092E-07F; 
ushort JqUn = 52649; 
string NDoOIcbtjakdUwAEYtIWa = "DgHjjWzFNdnKJwFyLtTThAeqBIjNV"; 
float gKOZigz = 1.44853E-31F; 
float mhQkMdYCJocgQdboJCM = 1.487745E+13F; 
float hZUxwplAOYDlXCpVfPulTdDYWhS = 6.652259E-14F; 
short WulIkSI = 4492; 
byte PohmRdNtGHWoHb =  21; 
ushort AnzyiwPHSMUedzkiQePwLedexLM = 16364; 
long jEOVhpNZGTsHZsTnDhLfQWo = 60024660183209292; 
int dCpqhqVwRzWcefMkhiFCCXmnMLNfu = 160748746; 
while(dCpqhqVwRzWcefMkhiFCCXmnMLNfu == 160748746){
dCpqhqVwRzWcefMkhiFCCXmnMLNfu = dCpqhqVwRzWcefMkhiFCCXmnMLNfu + 915690;
}ulong XuRQKZztZfpzhSoeay = 59948455941379445; 
float NmyjVIkPOMS = 7.139705E+22F; 
float FyIlfTdPkPDCtWlaJfhHNJf = 0.1890952F; 
long qoFhzgSShhPNyJyl = 31715027808504297; 
float nZWqfMztNAGIduBeeSUt = 6.770354E+34F; 
short MjuQIPFxoRTSWQ = -29425; 
string dVYHNoMqOx = "oAXpKzFVsJMXUjsmCJhoQRdHcq"; 
ulong UJzUtMgQTDhykgLHsYdxubjIGhczw = 69765915910628753; 
float icCuYBLdJgyodntFTz = 4.337389E-18F; 
byte JuDoPPFfaxBtVuSIkhVet =  86; 
double jfo = -2.391609E-26; 
if(jfo == -32138.74){
jfo = Math.Ceiling(Math.Cos(2));
try{

}catch(Exception ex){

}
}
}public void BeAnGWkmZSsNNnVa(){
uint tELiVaxPQooDkzRNIZkaZfjJHzF = 94; 
long ZTRTP = 67878217519162673; 
double IUWaGWPBWnucDyw = -2.366872E+25; 
if(IUWaGWPBWnucDyw == -1.088203E-15){
IUWaGWPBWnucDyw = Math.Ceiling(Math.Atan(-5));
try{
Console.WriteLine(IUWaGWPBWnucDyw.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}long UdbfqzYG = 47443838559207880; 
string EwmTObgahqowwe = "peVKDxOqoTNbw"; 
long oFnYdoXapaYqqHpPg = 31107587536020094; 
ulong YaaZM = 70549002500823582; 
ushort GKEf = 29943; 
long eHbhsxkNMYokYapbmaLFGHKaiZczh = 28235156795758999; 
int iRXwzMUucoIqeNftyWbYsuMGOfXRY = 91; 
while(iRXwzMUucoIqeNftyWbYsuMGOfXRY == 91){
iRXwzMUucoIqeNftyWbYsuMGOfXRY = 538298;
}ulong klnoFZjAHOFoHzyWGGOe = 51637574547754944; 
int cksLlLNKOwDfhqtDsGzh = 42; 
if(cksLlLNKOwDfhqtDsGzh == 290585){
cksLlLNKOwDfhqtDsGzh = 584648;
}float liaQVYMtXVlMiWSt = 2.652125E-21F; 
uint uBdt = 436716297; 
int yJxISmyFwMkiGZGxtMYg = 395188; 
while(yJxISmyFwMkiGZGxtMYg == 395188){
yJxISmyFwMkiGZGxtMYg += 822636;
}sbyte fIWzteWnAFUWzmYQbqxuVI =  125; 
sbyte zccEtDbgMAOPMPB =  -110; 
short PScJpxoaOuMRyIPaVbTKfbTPMOkb = -13419; 
uint qDLkpwPNHntPHwVbfaMTAjIQNYm = 548346460; 
byte LlzunMyAGf =  72; 
byte NmluQMeaEJxgVV =  114; 
short SjuaIDi = -31992; 
long JCgOkXldBNT = 86510149414078341; 
short ZhymeTwfKctiywAyUljJRWFoM = -7706; 
byte AYhNozJtJBfdQXQKAYtMOLJ =  203; 
string ZcEhCLGJGMVHTYMgOugfT = "gUQOaCYOosJGzHPBZuygqeOSeu"; 
double XoJYQfWKsNKeLRxEYnAf = -1.816421E-20; 
while(XoJYQfWKsNKeLRxEYnAf == 4.928215E+11){
XoJYQfWKsNKeLRxEYnAf = Math.Pow(double.MinValue, double.MaxValue);
object KfilKtxAJkURVT;
KfilKtxAJkURVT = 1.503947E+15;
Console.WriteLine(KfilKtxAJkURVT.ToString().ToLower());
}ushort nzgxldibopcsVeSLJagfLuLQPCP = 10165; 
int cAmpGsOxeHMWbOBDJoltTtToj = 770482342; 
while(cAmpGsOxeHMWbOBDJoltTtToj == 770482342){
cAmpGsOxeHMWbOBDJoltTtToj = 277565;
}string znDeFzBWlVBuP = "WplTAWmXJHYg"; 
ulong zPusLCEBXydRMxIzsV = 884111861781686; 
sbyte FWgzLWzyCiJnQxtOP =  39; 
byte pkndMmNztEEHtcVaqGgQXjXjl =  35; 
long HIbgUqluJdMzj = 36629301167765473; 
short iCuTfneRFDqRMC = -3851; 

}public void bQCsmUIHXfZFjmWUokTFBy(){
sbyte EmhVSi =  -37; 
ushort oSXYZjo = 57390; 
ulong ZoAHqHyDBISlFLwOaMNOPODxUuZ = 22490289883737828; 
float GQouH = 2.024201E+20F; 
short nmEl = -22008; 
int ZglMcM = 645219567; 
if(ZglMcM == 490442){
ZglMcM += 82606;
}sbyte cEEKwWzthVPgRfMs =  -111; 
float FNu = 5.034421E-30F; 
string THTtsKiElJZDNHIGZpWVVRVjjH = "DMIJ"; 
uint CcRBPEfYnUdsCfuWhwFj = 95; 
string KiMHGRFZCVkFQejNRXHXRR = "CKGElenBEbfRGSAsxoeR"; 
int QfdlSsjqdVSBjWantUmwIDLnEVs = 216963; 
if(QfdlSsjqdVSBjWantUmwIDLnEVs == 997708){
QfdlSsjqdVSBjWantUmwIDLnEVs = QfdlSsjqdVSBjWantUmwIDLnEVs + 46749;
}int ABamWpmXkYfIDsiWkEkmmwPIXa = 11241221; 
while(ABamWpmXkYfIDsiWkEkmmwPIXa == 11241221){
ABamWpmXkYfIDsiWkEkmmwPIXa += 463319;
}int BNtI = 958; 
while(BNtI == 958){
BNtI = 303554;
}uint PlcMCwRuH = 72; 
short VLgDbfIeASx = 4410; 
int fSIUAiKkt = 67; 
if(fSIUAiKkt == 630145){
fSIUAiKkt = 180242;
}ushort aaswunczjRipt = 41504; 
sbyte iLCZkmiGKqdSwJaVdywDkSMEFwosC =  102; 
double BInVIyjORNLdkj = -1750.431; 
BInVIyjORNLdkj = Math.Ceiling(Math.Atan(-5));
 bool? mIdjVTXWJcwRaIWMDS = new bool?();
mIdjVTXWJcwRaIWMDS = true;short gnyxUxiFESQLQkWQDbFQmbuGb = 3425; 
ulong ddXGLRPzfxxMDwdRPiB = 44359709423142759; 
sbyte SwqJEBAVjaYxyKgFSmRYDRScOj =  -56; 
int ZkmJFnaedYVEWFhC = 957401; 
while(ZkmJFnaedYVEWFhC == 957401){
ZkmJFnaedYVEWFhC += 763387;
}byte hqaDoUulTujQLNSxfBPoWu =  155; 
byte BkljpJlYBCmmyPiFKLCus =  207; 
ulong JkqZY = 62389025133777876; 
string MYRklFiILGKmJdSsKu = "BfqmaL"; 
ulong AGaAxIhQxBGFBBDGu = 76333804307221147; 
string yuLRAzdKcLfnGCnxR = "QHaDM"; 
float gJTEeWjufKwLKffxQAVnZYD = -0.02940579F; 
long BwBVJRetPjgXOWUARTLgYKjjcQGNi = 1360033850205177; 
ulong TGNpmKDgqfAMCODbQlpSSlq = 26289004886287405; 
float gRC = -1.286928E-10F; 
double BKnIJuYpTVSOoXUI = -1744.252; 
while(BKnIJuYpTVSOoXUI == 1.485446E+10){
BKnIJuYpTVSOoXUI = Math.Pow(double.NegativeInfinity, 2);
int? oLNJWugGeljlJMuTAu = 4760814;
oLNJWugGeljlJMuTAu += 61043;
}
}public void JGasRVstKdZGiNPgLVXROP(){
byte GhtRQZJgzABGIuD =  245; 
sbyte Btd =  -109; 
ushort cZKECOXUuEMKCBEKwTg = 43935; 
byte zpsZKNo =  216; 
ushort MNOQoeFLDHleISxHz = 52922; 
double XMwTCtkD = -8.673204E-23; 
while(XMwTCtkD == 1.114038E-24){
double NupoxgFXtJADcozTiFaqUnV = -2.395757E+34;
XMwTCtkD = NupoxgFXtJADcozTiFaqUnV / 3;
for( ; ;) {
Console.WriteLine(9.099172E-36);
}
}int kmdu = 954669; 
while(kmdu == 954669){
kmdu += 37519;
}int VaSQYjbmWlJKQ = 6815; 
if(VaSQYjbmWlJKQ == 131395){
VaSQYjbmWlJKQ = 91243;
}float cyPDDWVsonkm = -1.930444E+12F; 
byte uHMgqhZEuFonyKbXJcSE =  19; 
byte mgUIXzxwEycfBt =  194; 
string zkhXIHVFmnEsRJDgkzZxBbfYiLR = "FBFVaWeTgzGAZc"; 
long SdNOAZpU = 9115838871048632; 
ulong ZMJJqUFKaNTAxypU = 16609095533144828; 
sbyte CIOXJJKBcZCjN =  89; 
byte ZDaniXgkcKYNEuBYBVtWWcdk =  204; 
double qtXqfSgHIKUlE = 1457.911; 
qtXqfSgHIKUlE = Math.Ceiling(Math.Tanh(0.1));
Console.ReadLine();ushort LQJIszLP = 32518; 
float CltXNNVOH = 2.239064E+16F; 
sbyte JZOFOpRXVSxTiSxWEQOZJBGA =  -59; 
uint hVMuaHTYsKfbja = 6203; 
ushort eEUASdZXjgiuoplTKbizxlpUelyP = 38016; 
double UlDiYDJdJucyM = 3.254688E+36; 
while(UlDiYDJdJucyM == 3.521714E+23){
double UBTmauGGktjOnhNh = -2.215889E-14;
UlDiYDJdJucyM = Math.Floor(UBTmauGGktjOnhNh);

}ulong zUKCHDniaxpsPBBT = 44673761844490418; 
long LbWQbiEQzXy = 62204962384056713; 
byte qoSHAjOQtVfQpSlmkwgOZbExHZ =  60; 
double hiPBy = -2.959182E+17; 
if(hiPBy == -39.70898){
double bqOMCPs = 1.465259E+23;
bqOMCPs = Math.Sqrt(3);
hiPBy = bqOMCPs;
int? TpMxZBXRMkNqoSz = 861164;
TpMxZBXRMkNqoSz += 59453;
}string Ika = "YnU"; 
short zUcojMBBySujpfmmJaPZjknwABQTf = -32661; 
ulong mfw = 14882599912119713; 
ulong QqeNPomfoPmGIPJVIuyfs = 49363990522307166; 
ulong FCMKjLfpLzWXpiNadT = 10172557675788359; 
ushort kzPaElJeWeAkDdNHxZUBgFMFpg = 21103; 
ushort uQcplflJVudBIdZuFcixYVUlUMH = 48678; 
byte xVnjCnxUpXBXBFRazBNjMe =  37; 

}public void WAWRwAAAzjXimzDLXKMkWauowTnfZ(){
string sOomcsGAQzbAqHJOaYcmbsWcVeIwD = "tBykGuXlA"; 
short jVNUCdINfmhW = 18276; 
double RmqYLjXFfCcfbnROzKUSxjhubZJyY = -149925.5; 
RmqYLjXFfCcfbnROzKUSxjhubZJyY = Math.Truncate(RmqYLjXFfCcfbnROzKUSxjhubZJyY);
try{
int CBGUXuTwLhUXcNFfDOWdtj = 5918577;
if(CBGUXuTwLhUXcNFfDOWdtj == 93548){
CBGUXuTwLhUXcNFfDOWdtj++;
}else{
CBGUXuTwLhUXcNFfDOWdtj--;
Console.Write(CBGUXuTwLhUXcNFfDOWdtj.ToString());
}
}catch(Exception ex){

}int LNweekOgGLjgCyesaSsqqgs = 59754792; 
if(LNweekOgGLjgCyesaSsqqgs == 779532){
LNweekOgGLjgCyesaSsqqgs = 570448;
}double fgRuPsRZhxcU = 5.164946E-28; 
while(fgRuPsRZhxcU == 4.944322E+08){
fgRuPsRZhxcU = Math.Sqrt(4);
object XfMXqYHCRwQuEsHnx;
XfMXqYHCRwQuEsHnx = -5.80766E-21;
}string LMDjolVw = "mXzPbdxpjhxejOhwnzBnPtfun"; 
uint ByHqGRXGxWspQUldaGgCXw = 2169236; 
ulong hbURdCCqVSDQR = 16803524468932135; 
ushort pePgYOfKGEkKVMGTQP = 31284; 
double BkPUnQKoeGLOz = 7.174727E+17; 
if(BkPUnQKoeGLOz == -7317.154){
BkPUnQKoeGLOz = Math.Floor(8.485128E+21);
try{
int kEYhqXtEJ = 7572802;
if(kEYhqXtEJ == 59383){
kEYhqXtEJ++;
}else{
kEYhqXtEJ--;
Console.Write(kEYhqXtEJ.ToString());
}
}catch(Exception ex){

}
}float CKbEgtaootqQxgJbKcfqHp = -1.565111E-25F; 
sbyte DVykeZCcVFfd =  67; 
float NuOYxCfmQ = -1.871035E+16F; 
uint dkESyPQeSensntkd = 621981815; 
short eUWskLPy = 10505; 
long uBaZLbmHYehUoAGhiUjTGynxjokHN = 72046351292591379; 
ushort cJZpgWZHTFDHEzOs = 61684; 
byte PwQHTYMLKiGOUOtgkG =  35; 
sbyte jIscjtPXyOXGKFel =  -116; 
sbyte ulpynQJSkqD =  58; 
ulong eGOzE = 69245050840913711; 
byte nViAQNn =  224; 
byte lBBFG =  226; 
string CFYqOz = "MyzccGIzJWxulZ"; 
short MDusieshttBHFkWgKddoQRizE = -30516; 
float jAQhkPmPugmHfpKJebUz = -8.700114E-16F; 
long VbaVuVO = 2047702716818116; 
ulong hNDRxjAhAtDhtHTPIljOsuztfuus = 77428974427145403; 
sbyte zRBOVClZTdee =  102; 
sbyte cHNnjLcQ =  -94; 
short HVeTOqHpNtnF = -1124; 
double fBpnMl = 2.108876E-07; 
if(fBpnMl == 6.008938E-31){
fBpnMl = Math.Ceiling(Math.Tan(1));
try{

}catch(Exception ex){

}
}short UhWbRkBaCoH = -17232; 
int RRWWbUDBS = 871096; 
if(RRWWbUDBS == 12451){
RRWWbUDBS = RRWWbUDBS + 641483;
}long VcBUUqpkubAqpBPVRhSgMIxP = 55100548527504962; 

}
} 
public class bCxowWyMekowSu{public void OmYHHKgBemU(){
uint LNgLtwtxhoVoCJNYAfdqqfyGTNL = 19707377; 
short ByC = -31902; 
short FbxaT = 7406; 
double qfbt = 9452051; 
if(qfbt == 2.151279E-27){
double RGKAwQmuRZIwUUeHp = 5675810;
qfbt = Math.Floor(RGKAwQmuRZIwUUeHp);
Console.ReadLine();
}short JVPaKN = 25755; 
string pDRbRAzRqlGxyMEBN = "XZchHqEuocfuc"; 
string wKwebClclV = "LLljk"; 
uint uXbAiVBzxkwMWjDhbbhxMSLWRlBG = 169965289; 
sbyte GyJfmWCBFEebIBZWiDjiddkCl =  -99; 
long wVFnfEgzBxUfBISiOKnqmFypYWOC = 75543059065926743; 
double VdtBxNJeIwlbNDRsKQzooiPG = -1.633379E-11; 
while(VdtBxNJeIwlbNDRsKQzooiPG == 2.503265E+27){
VdtBxNJeIwlbNDRsKQzooiPG = Math.Floor(3487.443);
Console.WriteLine(VdtBxNJeIwlbNDRsKQzooiPG.ToString());
}sbyte fAMJDAkuFEJAVH =  -15; 
string RSyoeb = "nKGfotMLlCDoVMt"; 
double HuGPz = -6.370518E+09; 
while(HuGPz == 1.177777E-17){
double aznuSUee = Math.IEEERemainder(3, 4);
HuGPz = aznuSUee;
int[] ueiguzVsJKw = { 2003782 , 73794 } ;
Random QDZGPdCHBtCQHqBcaCjOR= new Random();
Console.WriteLine(ueiguzVsJKw[QDZGPdCHBtCQHqBcaCjOR.Next(0,2)]);
}uint tVNisAqcGFoV = 295065; 
sbyte qNaDsAcXSsg =  -1; 
long GRxnwhPyiUkOaMlhBmcyByLZZ = 69084951986209909; 
ulong zzlUZCTEgqFzpMLRUykqhfxnCoZgi = 14034250634851738; 
short VHTE = 10364; 
ushort CdGiztkyStbYs = 45770; 
int wGYllEZbDCuoKnCPSgNVBac = 16; 
if(wGYllEZbDCuoKnCPSgNVBac == 374533){
wGYllEZbDCuoKnCPSgNVBac = wGYllEZbDCuoKnCPSgNVBac + 652804;
}short ssU = -2362; 
double jiYeFGXYVOESmfkXHlwEY = -2.618406E-05; 
jiYeFGXYVOESmfkXHlwEY = Math.Pow(2, 2.1);
Console.ReadKey();long odDlm = 14936836017514125; 
short XumuLpNRbfQ = 26665; 
string xGcQSCsQSLljtoPMmDmV = "YyFXPDlCfxSBGAMYfX"; 
ulong hDChCWzcoSnIgeziyScZfnMouJNPd = 38534074339583895; 
ushort SdjmnijuCaDBmV = 51131; 
float RmOSjJYCLJjkJdnbxkVVbzfjol = 71.25365F; 
byte KKcDFIIEJwkLDAajSXnARKTXRsg =  150; 
int UyoPZAhgAGtSDBLpVGiftRmNeZReL = 5218; 
while(UyoPZAhgAGtSDBLpVGiftRmNeZReL == 5218){
UyoPZAhgAGtSDBLpVGiftRmNeZReL += 504727;
}ulong OBCbMzIgYhsmBy = 48582457611042182; 
double dwmepRSaWUhfHeHtFHfR = -7.054859E+24; 
if(dwmepRSaWUhfHeHtFHfR == 1.310226E-38){
dwmepRSaWUhfHeHtFHfR = Math.Pow(double.NegativeInfinity, 2);
try{

}catch(Exception ex){

}
}double kRblJCEeJHcLIaAQcHQ = -2.703979E-36; 
while(kRblJCEeJHcLIaAQcHQ == -2.930582E-35){
kRblJCEeJHcLIaAQcHQ = Math.Ceiling(Math.Cos(2));
int[] SWYEEDIYSzfax = { 2106390 , 3719 } ;
Random IKhKMNYejFZiUyutMKEq= new Random();
Console.WriteLine(SWYEEDIYSzfax[IKhKMNYejFZiUyutMKEq.Next(0,2)]);
}ushort uLfSgZftyheYTOalsNNtFwIkFzMxy = 6043; 

}public void HpjpaHgYjAGMM(){
short BYKqYzoauYdVnI = -16498; 
long sNzIJyZGlODTYKVSiXONffXqoQqX = 6721259323660792; 
float SBKMNKNgwohceLAWRlQgtTf = -1.167751E+30F; 
short FdopWYwfmcttmFh = -7220; 
string JOGzUSgiNuYwIRukhdfYleJF = "TyTDDkzdm"; 
byte SgiyjNlahSwDYcyend =  143; 
int bkbum = 19789337; 
while(bkbum == 19789337){
bkbum = 405520;
}int taVJPzSJywzVUBegkaKIARVh = 92; 
while(taVJPzSJywzVUBegkaKIARVh == 92){
taVJPzSJywzVUBegkaKIARVh += 649044;
}uint XiJyiFneLxEQDAsffcopLXqDeNHSq = 828814243; 
ulong zYMSIXGMFWcmeFEFbWOtjnZSgs = 20261897872982493; 
string HnzfJFganoZicnpUc = "KjYULoCHpSbxzi"; 
uint XOz = 69678021; 
int ZERtmVh = 4329; 
if(ZERtmVh == 237185){
ZERtmVh = 893890;
}int DiEbonYSQRJbpDxuMebYpTW = 7163; 
while(DiEbonYSQRJbpDxuMebYpTW == 7163){
DiEbonYSQRJbpDxuMebYpTW = 880531;
}sbyte GzkkoeJpTbMbCFUSUEiGhSFxKmop =  -91; 
float YKUcDeqgpdOhXFSuKRoByLCPNTPB = -6.692462E+22F; 
short aeNsICq = 16194; 
int SCtKIRSSFAStzmh = 29; 
if(SCtKIRSSFAStzmh == 820038){
SCtKIRSSFAStzmh += 71838;
}uint GhSRBNhADXaTZwckXJkkSq = 61241659; 
uint CmIxMts = 772249483; 
uint aqfyaoxzuiLlzthTpUnCmpqk = 833698055; 
sbyte HBjHBwXs =  -104; 
ulong bjhgaRaidFBcVAaDKaVCiilRT = 12283308383697344; 
string qejptVIbG = "znhBTm"; 
short lISYpnbO = -14485; 
sbyte PMBTmKyBKCHCgGLwLWlj =  -88; 
uint puusPVnGHCtQuYwURw = 63272831; 
long gLsFTqRcWBqijzSqJFx = 61863807667734880; 
string OwRVLnKajcXOE = "TzlOWcANhkMGgXDnwaQ"; 
long eAmAkip = 61035700351568907; 
sbyte tYVbnHjegMzWHWXVxPYYo =  58; 
float oGMEkMd = -3.776732E-27F; 
ulong RNgRTdudxSCVCjBYesgt = 48059314772039670; 
ushort mOUogioYfVUJyqNLjuKO = 63957; 
float taN = -3.460003E-09F; 

}public void ZyBJOFN(){
string dqJgmkAYH = "ZWKkjGKwNBnug"; 
float fyuZKNZKXptfSFQUbfIulpmq = -1.094483E-32F; 
byte BYK =  53; 
ulong gNfZXIamNVR = 60072060532486666; 
uint QkcWGDZjYHhsCzcPci = 38; 
double qBDmudUMwMmNqCccqe = -1.477092E-33; 
double RkfzgFmTDutiWollxfOgCNZRWtV = -1.184597E-15;
qBDmudUMwMmNqCccqe = Math.Ceiling(RkfzgFmTDutiWollxfOgCNZRWtV);
try{
int XbjdCpnfieIAJY = 5951373;
if(XbjdCpnfieIAJY == 4956){
XbjdCpnfieIAJY++;
}else{
XbjdCpnfieIAJY--;
}
}catch(Exception ex){

}float qTangmbFp = -1.759969E+24F; 
float qbM = -2.474318F; 
int OIDOtERgedWiAQUuCBCkPgLap = 207579339; 
while(OIDOtERgedWiAQUuCBCkPgLap == 207579339){
OIDOtERgedWiAQUuCBCkPgLap = OIDOtERgedWiAQUuCBCkPgLap + 647173;
}string azJqlVEwVBmzJq = "xHjSZwdI"; 
double zytylIxSMzskdpFYP = 8.225313E+08; 
while(zytylIxSMzskdpFYP == 2.372284E+10){
zytylIxSMzskdpFYP = Math.Sqrt(4);
try{
int GWSSlEUmaGicAZRTsBZwGNMBC = 914233;
if(GWSSlEUmaGicAZRTsBZwGNMBC == 39147){
GWSSlEUmaGicAZRTsBZwGNMBC++;
}else{
GWSSlEUmaGicAZRTsBZwGNMBC--;
}
}catch(Exception ex){

}
}ulong lQWFSyKykTjqpNoqfmPYoSc = 65208409645388158; 
short NlyGDVtCpxcCuTXubySGQA = -7697; 
ulong zKJuwgkTFECSMEqwxYxaENpRU = 25850743750427276; 
ulong lLgVo = 74653822131027927; 
uint tCYNnMuR = 401171; 
uint gJpJhXAeLpcgh = 950441550; 
string sKNoCDMUWAsRT = "BPyCfZOusuxAdzRFtOux"; 
sbyte OgFgFOkzgnRJDsLtRbIXqs =  89; 
string mcqfFyyAPaqXZwZ = "UniIwWAU"; 
string sPjtxnDJqMTsCQuiNquwSHJOxeKu = "qDyf"; 
long JpxEc = 74873518353856720; 
byte iiBFeMfxX =  87; 
int GZixlUL = 87267594; 
while(GZixlUL == 87267594){
GZixlUL = GZixlUL + 27450;
}uint wyTsqfRiuXBEtDTAl = 230669; 
long eUpSI = 71626754985943798; 
long dZmcOROzMpqiZSBSPQzpDDBKEzqZO = 32823502775053731; 
byte npwFCputZUpOMLuwBzDApAPLOogq =  23; 
ushort HGTjahYYui = 6879; 
short yioAy = -955; 
double JKIOBLQmidnDGKhTBtnCBip = -6.256811E-26; 
JKIOBLQmidnDGKhTBtnCBip = Math.Exp(2);
try{
Console.WriteLine(JKIOBLQmidnDGKhTBtnCBip.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}string PUjtZACiejFXHJSwnUTnLSlk = "yoopkJSzQPOGbiNWsLIebpQRBU"; 
float OdWJZBCjlTUCMARU = -1.265186E+23F; 
byte fqthFfRgAzPpQsKMSfHCHD =  177; 
uint fsCAVNLJhuxLOdTJwLOdELuPe = 95; 

}public void LdDHUqz(){
ulong IFMeIJYnbdc = 11046468063092143; 
double uSOCXdRPQyFetNgcZWcwCc = 3.159364E+32; 
while(uSOCXdRPQyFetNgcZWcwCc == 4.305065E+26){
uSOCXdRPQyFetNgcZWcwCc = Math.Ceiling(Math.Asin(0.2));
Console.Write(uSOCXdRPQyFetNgcZWcwCc.ToString());
}uint ewmyocRtzOttZ = 79617864; 
ulong msWTfzEtauVnWsJZqPZdUn = 14378715019015638; 
byte EPczEBZWJ =  18; 
short WeCwLtIceLRYjwtMhRCT = -23587; 
long hojxQPduRZAEwSVbzAkiS = 30823770859939771; 
int CnuufBXRmaqaZ = 83676160; 
if(CnuufBXRmaqaZ == 231935){
CnuufBXRmaqaZ = CnuufBXRmaqaZ + 795037;
}string WASkkVnJBnDnF = "pazPCCIXhCXeFpiuDcAaFODiVGJhM"; 
ulong MwMCYwxnCNZsqmgpYAfMBOE = 48900442733659716; 
byte NkJSPkZgOYnGxl =  236; 
ulong LJGamXSsaB = 58061534650248343; 
string yjogGNQoOeltgHh = "FuAnRBOLTFwpaRAIUpRBuAwBh"; 
uint VoOwMBxiDeOHXwEmaq = 282505; 
double EzQPHyfYYgoWTwfjKQF = 6.60315E+33; 
while(EzQPHyfYYgoWTwfjKQF == -2.105326E+14){
double wjJnHpMoFX = 5.973431E-31;
EzQPHyfYYgoWTwfjKQF = wjJnHpMoFX / 3;

}double bqqjaqjqNYAqmUN = -2.373782E+14; 
if(bqqjaqjqNYAqmUN == 2.179489E+26){
double PZlAy = 2.141081E-34;
PZlAy = Math.Sqrt(3);
bqqjaqjqNYAqmUN = PZlAy;
int[] fLFnwthobeFhNqIlAJtkeKYtqu = { 5429628 , 45731 } ;
Random eLTRd= new Random();
Console.WriteLine(fLFnwthobeFhNqIlAJtkeKYtqu[eLTRd.Next(0,2)]);
}sbyte iQoVtENY =  -30; 
float IEnX = -123.965F; 
long wMlXatcFhnBGokzcKuzRBXaY = 34052868996036236; 
ulong LzWSnCqYFKTmNyfknTcUn = 32298162318874219; 
byte YPHAagRfWBDsqfkGUulIKA =  237; 
ushort NKuGBPQsY = 58826; 
uint XfexOf = 7207; 
sbyte hmcghVFbASlnm =  -17; 
ushort xMMZtkNAWnhkWtEK = 6429; 
short gEuHUNWwGUKTIZnKH = 32342; 
sbyte WBD =  -79; 
float uegoCiVUpWbfRlxQhFlwBkJ = 1.079861E+32F; 
long qGHajNgtRnpsCXNKqICiWGgtYXzn = 64155051111434825; 
long SsKTFCRlLyNCqauCbMAmbloTiwQ = 51503999494832870; 
byte daFDMmaBJfyZqjS =  202; 
ushort GdRKjLUMudoCVjkcYLULpYntBo = 23462; 
double gCJDCjf = 6.71587E-11; 
gCJDCjf = Math.Sqrt(4);
Console.ReadLine();sbyte gudfiVUtORYD =  -45; 
sbyte XCMzGtHh =  88; 

}public void KTHoVJoDy(){
short QmOHtjRguyOluWzqRAYBllSz = -29630; 
byte SiKaU =  217; 
ushort nYSmjP = 1269; 
ushort WHmtxqyczQEFwoFefYXLyTtZ = 52533; 
byte ZlPMkPcCwaLx =  215; 
byte wcSCTJmhdQdMsdnnElYzQJdsQsOi =  21; 
ushort fAAzqgnqKMeMWhcLz = 10006; 
ulong jTNgNYA = 51013900369073511; 
byte GWgoUoNMXhkOHoaqmslMsLsHDQ =  196; 
int cKj = 859322927; 
if(cKj == 621547){
cKj += 860395;
}short ZwPRoYUHdMxULBAUEA = 16000; 
string gMVyaXOxPLucIMXW = "qFWhDnGPAptZIgfld"; 
float UtoafkobcSdODYchhhtEy = -1.873396E+12F; 
uint wxhFnQFTnbhcFQXnmEXx = 811990; 
string YXdLplGLOsLOgyPbzVn = "UfEsxojMMffFwCGpMUDbxyolejgQ"; 
ulong XHCeMTuMnbcUZIlfsGRzpFsmByQ = 35014284948385545; 
uint qQNW = 59; 
uint DyHZZSCZpFsooFncbzcTIWiAOcaPd = 289514; 
double nYZisnuHwIkQjPNCLUDDIqGnh = -1.128592E+34; 
nYZisnuHwIkQjPNCLUDDIqGnh = Math.Ceiling(Math.Acos(0));
 bool? jWtLbgqEUBUVzNO = new bool?();
jWtLbgqEUBUVzNO = true;long DzCdEVnemERDSetNwckQA = 63391272125948046; 
string EzIawYRqDAJTK = "JBQbsVonSjxzZGkuAmjinekdXOFLI"; 
short nqDQSLNAhfoGyFXYkgiZwJCME = -13097; 
ulong EpEGC = 51824323415950455; 
ulong cYzO = 10775515974302873; 
int QlDYJWRDJeexPCuPBmIHzNfC = 671; 
if(QlDYJWRDJeexPCuPBmIHzNfC == 996974){
QlDYJWRDJeexPCuPBmIHzNfC = QlDYJWRDJeexPCuPBmIHzNfC + 148273;
}byte EsHFPKdRkBouKckoYPYdPYqcfbMG =  44; 
float CUhTyUQZQjQxXzLSj = 0.01105364F; 
double jghGaEzO = 0.007401997; 
while(jghGaEzO == -5.134766E+35){
jghGaEzO = Math.Ceiling(Math.Sin(2));
Console.Write(jghGaEzO.ToString());
}string yMXuHwNCWpeQEpiRwwU = "SCIGfWpC"; 
float QqoHuZjN = -3.353228E-17F; 
uint dOZZRMcsIlfSNVdsnLfkLMBN = 949795; 
double HJcFNdyT = 2.976987E-30; 
if(HJcFNdyT == 0.002641213){
HJcFNdyT = Math.Ceiling(Math.Atan(-5));
Console.WriteLine(HJcFNdyT.ToString());
}double GtIkO = 2.471934E+20; 
while(GtIkO == -1.358115E+15){
GtIkO = Math.Pow(double.MinValue, double.MaxValue);
Console.Write(GtIkO.ToString());
}int suDUTHVZcnWutEExtJlatgFTQla = 725790; 
while(suDUTHVZcnWutEExtJlatgFTQla == 725790){
suDUTHVZcnWutEExtJlatgFTQla = suDUTHVZcnWutEExtJlatgFTQla + 745153;
}float MusQHAuXFRnmjWlOfFHD = -8.385891E+22F; 

}
} public class dIHAnHuUICURcbSGBYiac{public void ZQAAIOxgnyWLniaZg(){
short WHLoejNm = 32488; 
sbyte cPHCIQZOYHyBnyUsNKF =  5; 
float QfVoCupVBybJuheNBkZtsFzfx = 1.404629E-22F; 
float EwEOkZBHtnGzdfaAFcNmaiuCzAlHP = 1.554137E+15F; 
byte VXbyVjhEghWuVoz =  117; 
int OzFZuacpLOURCEwZhhYcKTFKU = 47; 
while(OzFZuacpLOURCEwZhhYcKTFKU == 47){
OzFZuacpLOURCEwZhhYcKTFKU = 750007;
}sbyte ZWLUVEb =  -29; 
int uJoHsYVhpAEiVUUf = 5433; 
if(uJoHsYVhpAEiVUUf == 511073){
uJoHsYVhpAEiVUUf = uJoHsYVhpAEiVUUf + 12855;
}long CILflFXJetjaFwgiIcxKG = 43228804855643175; 
float uYGeKnQVLCUdCLTBYwNeyEadKHUfx = -0.003138234F; 
byte wQcZpxVSg =  183; 
sbyte OAfgoldNNBQXDwctKApcLR =  -95; 
float CJYRlqSStWhOYhiwyAUZfyUuC = -3.761072E+11F; 
ushort ZWslp = 49706; 
int VBjXe = 47; 
if(VBjXe == 728230){
VBjXe = 342272;
}sbyte nsmULeOFpdJyNtaPDIuF =  -47; 
uint KcamwzWxpmzLFfqFJQAGyCAsQzfcd = 565402116; 
short uDSLRsfuIskoIwJldLPRWC = -22176; 
int yWtTssKgYjDzUSyulPio = 16; 
while(yWtTssKgYjDzUSyulPio == 16){
yWtTssKgYjDzUSyulPio = 12308;
}byte BQSuoctwNBoaChJlgNiTqxLM =  252; 
short oTWbsfUHiqijRzcHLA = -21947; 
ulong ARpeRuzdegwazeCQAXUHCAUXDsu = 73196764837513091; 
string WBLxDekOjstBRkDnguZ = "sCAZjndUEAQoAjDxijYwJjsZaOYP"; 
long unQenwbMmoapjLDdaH = 79976756518023041; 
byte YwookaeCAwWhwyWK =  102; 
short IxX = 16467; 
short EzySlP = -25814; 
byte NSZMeiHbFGKCLHaMi =  236; 
sbyte cbJXiICHKnGRWUKeL =  45; 
byte awthIFTSyfcTYJzMibDyHZqpa =  222; 
int WVbAJpCIJCpEPKjFlMTmwYVKg = 208943434; 
while(WVbAJpCIJCpEPKjFlMTmwYVKg == 208943434){
WVbAJpCIJCpEPKjFlMTmwYVKg += 139025;
}ulong cVC = 79082853682917438; 
long IWPNenaswXZstSeADGuCEMPIBJCke = 4239529419513503; 
short TOIIXDMisDJnynjxWhglwMPmQcMu = -17687; 
long mkIBOVeRfPAiIxpzYOLlgffDg = 88418797667685183; 

}public void wcPOxBRpbyWDhQBENGBzKg(){
sbyte tPAnW =  -52; 
ushort CVamJkskuQeJGFRGq = 21620; 
string xjboXKwIbEtMZublPmUDnp = "UcxA"; 
ulong UWGmMfqTMoZQLjRxtKlDGQdxBygg = 73056860938943177; 
ulong mnLsdYUMDglyKNm = 4613677330943117; 
byte AOQjacjJUxXSnNfCOVaULD =  199; 
double sdOGNZpBGEjeD = 4.613254E-34; 
if(sdOGNZpBGEjeD == 1.674597E+23){
sdOGNZpBGEjeD = Math.Ceiling(Math.Cos(2));
int[] TQIYxdgyIqIjVyltWSWMzHZoOZYOG = { 637619 , 41674 } ;
Random kGGkkMUfGLJTI= new Random();
Console.WriteLine(TQIYxdgyIqIjVyltWSWMzHZoOZYOG[kGGkkMUfGLJTI.Next(0,2)]);
}sbyte jMZaydkBmRtjxesRogcEKlAARl =  -72; 
string YWTIgOcGsNCwLYVBViG = "btXtDjYmBjk"; 
float VnVADp = -5.05907E-15F; 
byte tBiZmFYVdGNDFYAfXQPfhDKnPLab =  91; 
float ZRxFTtjMJCo = -7.581186E-20F; 
ushort JWZytCCePQRV = 12940; 
uint XDkpOMiuHMNdgETuxT = 41194; 
string VAO = "MIOcyZjeFMXXGHB"; 
uint MHyuCVwubOAA = 640408826; 
long paBHFnNTcZgyAnQCD = 70342209438638618; 
long lLFJemwpGZUOyT = 29330218908753824; 
double lyRklQPnWSBizpbkdXjCIUMZNMJX = -2.223569E+18; 
while(lyRklQPnWSBizpbkdXjCIUMZNMJX == -4.311946E+18){
lyRklQPnWSBizpbkdXjCIUMZNMJX = Math.Ceiling(Math.Tan(1));
try{
Console.WriteLine(lyRklQPnWSBizpbkdXjCIUMZNMJX.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}uint seWENFKtMJw = 97; 
string sYYXuenoFjf = "JIonsNeAPzlfnLXpuMCt"; 
ulong SIWUfl = 29846328681803965; 
ushort csUECTMTJKhXV = 56552; 
long SCPHqzITwZhTysKkTZJjAm = 86437013259884982; 
float LWeAWQcFKahCMTx = -7.967748E-39F; 
double WjzmG = 4.035165E+24; 
double JhCmjocX = -2.366772E+21;
WjzmG = Math.Floor(JhCmjocX);
Console.ReadLine();ulong xlTwcLUeQIb = 50761032006855626; 
int SHliuUSOGiXiO = 24; 
while(SHliuUSOGiXiO == 24){
SHliuUSOGiXiO = 355728;
}ulong FBaYioUYMCAhXFGIlIRcpkze = 17401853514803366; 
float nflkkFYPUHyaQzHExC = 1.223877E+15F; 
uint cWhRwsJDikntq = 418264; 
string leCpRHPCwVMOSM = "eUcoctj"; 
int qzmNyh = 15; 
while(qzmNyh == 15){
qzmNyh += 628802;
}double kptp = 2.959196E-05; 
double ygZdbzuZttcdnPFt = -2.261048E-32;
kptp = Math.Floor(ygZdbzuZttcdnPFt);
object RSZKckik;
RSZKckik = 3.719121E-06;ushort bDhHGgpaRpixkGuWEf = 36589; 

}public void LhuDMZdtQACThEMeZedUebySuIIqW(){
short COxTUiG = 25200; 
int XjqO = 2806; 
if(XjqO == 307757){
XjqO = XjqO + 932007;
}uint JUJL = 162834; 
string MyBqZeLLSVuk = "QhSfdetazGuJAfC"; 
byte IxQmqAJ =  253; 
ulong EVauCnES = 43070073009181160; 
float JBZjBJ = -3.788108E+34F; 
ushort AIigoUQey = 14371; 
float VnPtFjteVcJDq = -8.064778E-22F; 
ushort qgFUstfIzijUm = 53797; 
int nGLwheeFoq = 977881; 
if(nGLwheeFoq == 33257){
nGLwheeFoq = nGLwheeFoq + 653712;
}ushort gemBEVJMfbikIlcndIBYm = 15518; 
string FbWgbKgCyEKBbNcQMxZdweJjWsLth = "xUdnWCYDKxUHNeD"; 
uint qGKmRmYFA = 7485; 
byte YNuZgQyOQBSjMDiOsFIuf =  98; 
ulong kFHeJyjksazhmNX = 24545468489501766; 
long QhNfQEFYWPCjsesqHSLebCFH = 39598079228877460; 
double TpxfCyTipX = -1341.959; 
double GOnDfjBXMoqIcmd = 7.164367E-24;
TpxfCyTipX = Math.Round(GOnDfjBXMoqIcmd);
try{
int pnkSlHltElKVxWdpBF = 7189129;
if(pnkSlHltElKVxWdpBF == 3941){
pnkSlHltElKVxWdpBF++;
}else{
pnkSlHltElKVxWdpBF--;
Console.Write(pnkSlHltElKVxWdpBF.ToString());
}
}catch(Exception ex){

}ushort MYXDHoEFpOdYmqSdVcDbNaXVFDS = 27610; 
sbyte OfAe =  21; 
ushort fDSpdFtqXQEc = 48691; 
string AshLbbYexhAmZXziNzMQ = "LLJxEEHMnutcfHhW"; 
float pgyxiFstuAIR = 2.784643E-38F; 
byte OHAoyzs =  90; 
sbyte fkUqqlTfwJcoDddkihexQsR =  116; 
float yFURPfnuAhAdhwYz = 205.7082F; 
sbyte ewkmojnLYNtWzRPo =  -73; 
int XaiFWMfZJzsNoStxctIDFRExMOD = 33092637; 
if(XaiFWMfZJzsNoStxctIDFRExMOD == 246294){
XaiFWMfZJzsNoStxctIDFRExMOD = XaiFWMfZJzsNoStxctIDFRExMOD + 19454;
}long zWQoCjj = 36816669082458308; 
ulong EbofyMSiiBJbPeMHLVBleftwlqQ = 83972868823297052; 
double bajYgtJCEyBn = -5.079447E-12; 
if(bajYgtJCEyBn == 3.085425){
bajYgtJCEyBn = Math.Ceiling(Math.Tanh(0.1));
Console.ReadKey();
}string djw = "XcxbAheA"; 
short VGIjthagLwTdtjo = 2343; 
ulong JYWcTwLlfIjFXLgtG = 51528409593071492; 
string UJmNXqDRsBS = "mAgbsWgyhXaEThEENYZESe"; 

}public void DTVmmuUPSOJgNRTIbsDiWJKTP(){
ulong dtCnUhJEiyJUcnihPkbQMUpXlpWUU = 7375192296304650; 
uint ZenKjsyBUOBUJOXnjdEbEkOK = 428061137; 
byte IzNSpwbLzjfGPP =  11; 
sbyte osLChHqMykPq =  19; 
long gcSxaJEnMlQZXYmPiJqLdBU = 71462564007997790; 
float szNXGeKLeYosHuOj = -1.556135E+08F; 
long ZKGisFFEM = 44949242196088308; 
float FHZZDFikbqIOHNHBI = 1523658F; 
int AgZ = 49; 
while(AgZ == 49){
AgZ = 768774;
}short FdcGaLdEjuDldnSeoZYHW = -16066; 
uint BaXcg = 6913900; 
byte YjEK =  5; 
uint PauHWUVNIsggQJNqCXeaObyypoie = 27; 
ushort BbRWTUiGej = 25943; 
ulong WzulSbNWAiOEXpIMRJu = 51645665256586592; 
ulong VEHJSuQPTllIsaTd = 81724665971843060; 
ulong oLEwuuZyEGhUeWRXXxhpCH = 89136451599801746; 
string SgefkzSSJUxmVzRHLXp = "SAjIQ"; 
ulong RpNSWECbYRLMAFpIHkISZMyCHqXN = 11457686729599153; 
double AKux = 1.081331E+23; 
double yhXXfTlkFGOJcuwpYFN = -262.0938;
AKux = yhXXfTlkFGOJcuwpYFN * 2;
 bool? zDTHCwByjCtZhzEUKKRVdGOQwFVT = new bool?();
zDTHCwByjCtZhzEUKKRVdGOQwFVT = true;byte AHDuwumXZiJzZbQVbHgZsezlMclW =  82; 
int jZOpyGjMoioSlbGdMZX = 168381312; 
if(jZOpyGjMoioSlbGdMZX == 586887){
jZOpyGjMoioSlbGdMZX += 416200;
}sbyte IthpHtDzzkM =  -34; 
short URQcmmjgZAXtQynMjUooCtgTIz = 12281; 
int SaGIYn = 43; 
if(SaGIYn == 920142){
SaGIYn += 124809;
}uint JsnEekMbReaq = 696241; 
double ODIagAqLSiipI = 4.968238E-11; 
if(ODIagAqLSiipI == -1.681433E+38){
ODIagAqLSiipI = Math.Ceiling(Math.Acos(0));
object xbTYpGpqPc;
xbTYpGpqPc = 3.824297E-14;
}sbyte pXycRQpkidfCwiUVRlCB =  0; 
byte SQLhROVWkKpZISoYlVk =  157; 
ulong PGBFSzmoJhdqGl = 41244580028122732; 
sbyte mcDnoGo =  31; 
float bfHmVxJWbhWqmc = 91832.46F; 
ushort sGCXBqsPWZZcSyyikablDRy = 63258; 
ulong mVQAwMiDnnVdGpUfAKWYoAUdSSCj = 2372089695243066; 
double EnTpiingwMsxsOb = 7.013742E+11; 
double sNgsWclygcfIfZpLQMyPSOiuCqVow = 141007.7;
EnTpiingwMsxsOb = Math.Round(sNgsWclygcfIfZpLQMyPSOiuCqVow);
int[] UXwbVkt = { 8283097 , 58843 } ;
Random UopIuLJzpVVgugFFN= new Random();
Console.WriteLine(UXwbVkt[UopIuLJzpVVgugFFN.Next(0,2)]);
}public void XcUlgE(){
byte VKU =  132; 
string YzTMgGlReqV = "KOtRoeGlDRwdCQBIDHaucehfLa"; 
long UfYDqmUyIduQVSNhcPkqfFnyqBQFG = 71809706400356804; 
float nWcaAmkAjqacWPtSpRClQxo = 3.179396E-10F; 
double wmCpo = -7.618603E-36; 
double bHmFljA = 3.242192E+21;
wmCpo = Math.Round(bHmFljA);
int zBLYCefOSXndq = 957066168; 
while(zBLYCefOSXndq == 957066168){
zBLYCefOSXndq = zBLYCefOSXndq + 294155;
}int ZoEa = 174782; 
while(ZoEa == 174782){
ZoEa = 472022;
}ulong wNhpNqPNjXOOmPYS = 47119680527754942; 
sbyte ZVekTACDWcQVgaATUcCPNOkhOGJm =  -1; 
string WiPsTIZdnl = "SVcVmOZgxxLskkLXxnQoc"; 
float TNqJHlVYmDcVpfTsUntJSlQ = 1.813592E-29F; 
sbyte DQzXHbgihVftVCzzQhpuSE =  105; 
double KiV = 2.014284E+32; 
KiV = Math.Floor(8.688036E-13);
Console.WriteLine(KiV.ToString());long unlJzaXwzmkZdZdeQLuD = 71338348815961557; 
int UTlNOcyDnxm = 398277; 
while(UTlNOcyDnxm == 398277){
UTlNOcyDnxm = UTlNOcyDnxm + 140491;
}string QfFqfKoSRTMinOh = "qNqkbHnxYpyLj"; 
uint FQlQkoDgczjGKCmZMmSF = 805971; 
float iSisQmFJtkhtbX = 2.154083E-32F; 
ulong oMSbFUMpLyqTmPYRItf = 15907115048418541; 
float OpOPPCeDhnnHBCgqBXsukpEYGpzS = -6.164183E+21F; 
sbyte SsWhEdznUnAfRDQn =  65; 
sbyte BbANzNPjzcllshOPhFZOxA =  -104; 
uint QzgcAkehcFztGzwOUZkIchXdOJJO = 304126; 
string ZQIoJgYRhdpEpwDgFaDBUSUJBdoR = "qNNECuPRgfNatBZRt"; 
ulong fDjwNCdgWYOF = 53172248683062032; 
string ozVPABqVRgqXLZgsZczmSSs = "WTdKJntXptmLBYbD"; 
ulong hHn = 3455996195294557; 
ulong yfuTHxIlEOtHsNwSbbVxQLMlbxO = 8955427952412277; 
int eugiQ = 864809; 
if(eugiQ == 73658){
eugiQ += 553867;
}byte DXxseN =  98; 
sbyte jIHCSsMeUXQMWHPnagTqOtkME =  -86; 
uint jtclXnjz = 272389; 
long iiiFyuGpIddGCRdR = 56101154286983377; 
double PJypMNwshGjHojDiEf = -6.322298E-06; 
PJypMNwshGjHojDiEf = Math.Ceiling(Math.Tan(1));
Console.Write(PJypMNwshGjHojDiEf.ToString());ushort YVzOcmVR = 29480; 

}
} public class efUjiBw{public void cnltnMudeTAQRVhMxKhDjB(){
string dmIFqRttAWugYG = "NtYibqHqiXiNAtTDCkRo"; 
string QNmBf = "VCbNRcwiksbd"; 
ushort zHidUaGtSVhqHJDkVCloxIWwWyX = 45983; 
short jNRfGiF = -20264; 
int MCJgG = 415650; 
if(MCJgG == 4320){
MCJgG = 453432;
}uint hAQLNgycVbJLuABcuHLVgEUYQQ = 31396; 
byte qMKUK =  159; 
ulong SdicRAZwkZUKluN = 73654738105340239; 
ushort RLXbBQUnWOINdNwYwGygBSXZei = 7179; 
string EVtOjZPbtLjaxoGeSIRjTAqlnsYN = "WsKSpsTnZUNGWbUngdDsUmBnSdeEh"; 
uint bbdytAhfXMQTOeEWyyYhnwweJ = 416779155; 
string npgAbLFTE = "OYXGGduMjnhHfjqEH"; 
int OCdhTKwKtMtZcUVjjpPPanflMeHjW = 46; 
while(OCdhTKwKtMtZcUVjjpPPanflMeHjW == 46){
OCdhTKwKtMtZcUVjjpPPanflMeHjW = OCdhTKwKtMtZcUVjjpPPanflMeHjW + 803350;
}string QdWkubaRoeNkZhnRQZAxS = "PclNjnoaidVAtl"; 
float GmeuxaUZXbjmhVpERto = 4.091894E+36F; 
int MDnWJoAObYltmqzZPhlQidbTm = 33328154; 
if(MDnWJoAObYltmqzZPhlQidbTm == 938117){
MDnWJoAObYltmqzZPhlQidbTm += 978796;
}int wsSidkKyninWCdmtuBMNRJJBwJVYR = 9113494; 
if(wsSidkKyninWCdmtuBMNRJJBwJVYR == 390012){
wsSidkKyninWCdmtuBMNRJJBwJVYR = 830946;
}double RLXDsVVOWLLeoGxmThN = 6.937387E-13; 
RLXDsVVOWLLeoGxmThN = Math.Ceiling(Math.Sin(2));
object zNONzXjTC;
zNONzXjTC = 6.114693E+22;
Console.WriteLine(zNONzXjTC.ToString().ToLower());float wRAGEzQCdGzKJZWFnyfSes = -370.8409F; 
int hZAzBhtNsWnTuMcadcncEesJPSB = 650118681; 
while(hZAzBhtNsWnTuMcadcncEesJPSB == 650118681){
hZAzBhtNsWnTuMcadcncEesJPSB = 996612;
}long RcFfppWIZqw = 5018089573358286; 
short HQJJ = 9163; 
ushort pCqTM = 26241; 
byte AXXMHytsuOfjEOcJxpmkmVxJWSqz =  2; 
uint ZEgPwpRLHnzo = 520907791; 
byte DzIywhbKEBnPPfVqT =  103; 
sbyte xYIsFGHWPHChMqPaseXS =  7; 
ushort UbcVBsgxB = 25945; 
int FQpSP = 87472; 
if(FQpSP == 981225){
FQpSP = FQpSP + 121787;
}string pVYOyneZMNpDKfF = "kbNilkjcliqyCoVVZyky"; 
sbyte cfzeSdZoewBmMXtUlfMW =  9; 
short IzxRuEmVgAEYILhsgdXdWXnEYDek = -26701; 
ulong fOiQHxlFL = 25914394828559467; 
sbyte FTFkaLjTTSzcRwnzkzVBWUz =  -68; 
byte EBwZRWfsgtBDie =  110; 

}public void tNlQJYAKOmnFQLusaE(){
sbyte yUaDWznVXIpQetCMZwOGb =  53; 
byte JCCwYhDFYNfAMKTjKE =  190; 
sbyte zBxjJsFQCIMJxUwXSXPfLSqncZpC =  20; 
double YeHkbeloQXPxftQ = -2.385922E+23; 
while(YeHkbeloQXPxftQ == -2.006014E+25){
YeHkbeloQXPxftQ = Math.Pow(double.MinValue, double.MaxValue);
object tSHIZiWwkUlKAdseQaeuFS;
tSHIZiWwkUlKAdseQaeuFS = 1151582;
Console.WriteLine(tSHIZiWwkUlKAdseQaeuFS.ToString().ToLower());
}ushort CWDbl = 18526; 
short poajtYXURbmetTIiHj = 24808; 
long CRMebDyIXMD = 73827757844659114; 
double DMmqMIotwbwJnOzofswB = -6.948176E+29; 
if(DMmqMIotwbwJnOzofswB == 3.74064E-29){
DMmqMIotwbwJnOzofswB = Math.Ceiling(Math.Tanh(0.1));
object hosUAGKSGwwIyEjZyFfFbD;
hosUAGKSGwwIyEjZyFfFbD = -9.195045E-18;
}sbyte JFuNefQsQfdzoSbsGgwyzTqDtAMV =  74; 
ulong OPxW = 65660881143815380; 
sbyte PaQTkLphLz =  75; 
byte tFtUDdcpKTVfUYVsFKBkmFzWb =  149; 
string GdYgJHnHKXlodYehUOqMwQg = "ocFPfyTYbpKHUOxnKnIHzlg"; 
short qbnnoqQXjSGbzBme = -15451; 
byte ozmgqcbhiApdWppPwFKNYUJ =  179; 
double llNuDjYtZadVXkmiHzUPKMhMS = 1588.644; 
while(llNuDjYtZadVXkmiHzUPKMhMS == 1.737877E-08){
double RQwOXfeEppMAIAAQm = 4875.37;
llNuDjYtZadVXkmiHzUPKMhMS = Math.Round(RQwOXfeEppMAIAAQm);
Console.Write(llNuDjYtZadVXkmiHzUPKMhMS.ToString());
}int dsqMtmVAHZLfkOkfcbwKcy = 7; 
while(dsqMtmVAHZLfkOkfcbwKcy == 7){
dsqMtmVAHZLfkOkfcbwKcy += 178455;
}int FyHPIecMuGGodgL = 80381874; 
if(FyHPIecMuGGodgL == 974851){
FyHPIecMuGGodgL = 959276;
}double TKGqnztdMWiwnzwgfdqHPe = 1.907541E-09; 
TKGqnztdMWiwnzwgfdqHPe = Math.Ceiling(Math.Sin(2));
int? XPWlyVXhBjsJP = 8635258;
XPWlyVXhBjsJP += 28698;ulong iPpSoUbKakXUCesiHEMLFCMoaKqu = 52033172622569588; 
string OLkBunkbMMNRbo = "ctIcaSFDIApwyeOXb"; 
sbyte PbAehyeZ =  120; 
int tIBXNwAmdEihDbdTNWY = 72150162; 
if(tIBXNwAmdEihDbdTNWY == 505555){
tIBXNwAmdEihDbdTNWY = tIBXNwAmdEihDbdTNWY + 527653;
}int AFVTHYbgJV = 350140; 
while(AFVTHYbgJV == 350140){
AFVTHYbgJV = 38576;
}int TWceFqcaQVHQawjgZIh = 55129516; 
while(TWceFqcaQVHQawjgZIh == 55129516){
TWceFqcaQVHQawjgZIh = 400490;
}double EGpXKlUIH = -6.591927E-08; 
if(EGpXKlUIH == 7.382954E+23){
EGpXKlUIH = Math.Pow(5, 2);
int[] FfOEVFIEUWbwwDdiwuRRQpRYkdMu = { 7501590 , 13443 } ;
Random slIhEnPpGpUzpbgIZ= new Random();
Console.WriteLine(FfOEVFIEUWbwwDdiwuRRQpRYkdMu[slIhEnPpGpUzpbgIZ.Next(0,2)]);
}int caJggnoPsRTndCLunRooSQdXsAo = 33079980; 
while(caJggnoPsRTndCLunRooSQdXsAo == 33079980){
caJggnoPsRTndCLunRooSQdXsAo += 828019;
}int lYayZACmzb = 2421; 
while(lYayZACmzb == 2421){
lYayZACmzb = 724775;
}double ncmWMPHYFyfBnxzmAxmuoPqDF = 2.742762E-27; 
if(ncmWMPHYFyfBnxzmAxmuoPqDF == 4.317872E-18){
ncmWMPHYFyfBnxzmAxmuoPqDF = Math.Ceiling(Math.Cos(2));
object tiqtGfCUWjjiaEnVmwHi;
tiqtGfCUWjjiaEnVmwHi = 4.132676E-14;
Console.WriteLine(tiqtGfCUWjjiaEnVmwHi.ToString().ToLower());
}long SlUScMIzXoEznKHmU = 53098877148464155; 
ulong MwgwqDUPX = 6298017206287665; 
uint fnRsDnFFYHTe = 24154948; 
uint cQUmVuUKNmetAanzpMLXVdFgNXV = 7; 
ushort nfLBgZnyMtgFojlfXswObCDaYkO = 32308; 
double MHqsQmPAgzcnZEO = -4.50819E-18; 
MHqsQmPAgzcnZEO = Math.Ceiling(Math.Tan(1));
object mAWQYeAVYLBeLeXkmnOMWmlsFEDBG;
mAWQYeAVYLBeLeXkmnOMWmlsFEDBG = 3.500196E+19;
}public void ZnEl(){
ushort ZomQwUgIUlRzuYjLJV = 20421; 
short ZHaxftCyPwj = -6276; 
float yoNbtgEHNTZFSSoFJVelaIm = -1.470579E-17F; 
uint kfQHdDkMwWTWiNtThy = 518827; 
long jNopGO = 14500344344146628; 
double VcIfUJPYV = -1.775412E+09; 
VcIfUJPYV = Math.Truncate(VcIfUJPYV);
object eHwSWUCz;
eHwSWUCz = 9.99862E-11;ulong kuohqQufbsQpVxehmYgldZstI = 68994456527863344; 
sbyte dKKqzkNURoVoecBIpWDPlb =  106; 
uint daQGkNYMKayDIJbpfCeMeIsPORgP = 5062; 
string kuukWnGCXDubdeInCzgBRNSx = "bRtHjqjYJsnnRtRVDPDnp"; 
long jSmVuCXzCYCVymVOljcwN = 65328366522824216; 
short sqhjOWw = 20329; 
ushort UMpoztJHbpdmoWPSDQeWIB = 15072; 
ulong EpchRQu = 59833642469168726; 
double WZefbkuVuFMBdtBAfUcH = -1.094155E-26; 
while(WZefbkuVuFMBdtBAfUcH == 5.097462E+10){
WZefbkuVuFMBdtBAfUcH = Math.Pow(5, 2);

}float OpGeZuzkQdGEJUwltWpUbitW = -8.440584F; 
byte uXpOlaAzynXFPCBXWuWgDGdq =  103; 
double FWJGyhKQSNSsJuXPfYMfAJwa = -4.193085E+08; 
if(FWJGyhKQSNSsJuXPfYMfAJwa == -1.917154E+22){
FWJGyhKQSNSsJuXPfYMfAJwa = Math.Ceiling(Math.Acos(0));
 bool? QXsJRoWCJARbxaYOAdy = new bool?();
QXsJRoWCJARbxaYOAdy = true;
}double aeiJuJLJxFYQliPBTuRBVpNwAIPbc = -2.156167E+23; 
while(aeiJuJLJxFYQliPBTuRBVpNwAIPbc == 27.75313){
double UqdsPqeDhzjwNoGwcLqNjNUJG = -4.670483E+32;
aeiJuJLJxFYQliPBTuRBVpNwAIPbc = Math.Floor(UqdsPqeDhzjwNoGwcLqNjNUJG);
try{

}catch(Exception ex){

}
}sbyte wMBDMnyLqkKQfYA =  -80; 
double mnEQYUoshldmJx = -2.273361E-34; 
if(mnEQYUoshldmJx == 7.740357E-39){
mnEQYUoshldmJx = Math.Ceiling(Math.Tanh(0.1));
int[] KfuTTFOGnuflIdCny = { 5869254 , 22953 } ;
Random WJjcS= new Random();
Console.WriteLine(KfuTTFOGnuflIdCny[WJjcS.Next(0,2)]);
}double CkEyamDEEc = -8.342261E-26; 
while(CkEyamDEEc == -3.211257E-09){
CkEyamDEEc = Math.Ceiling(Math.Sinh(-5));
try{
int GPxIbxZSGOqFOXkV = 9539609;
if(GPxIbxZSGOqFOXkV == 21687){
GPxIbxZSGOqFOXkV++;
}else{
GPxIbxZSGOqFOXkV--;
}
}catch(Exception ex){

}
}short NLC = -18665; 
int kWyGItqqcUFIYhFqORBtFGg = 660008103; 
if(kWyGItqqcUFIYhFqORBtFGg == 590702){
kWyGItqqcUFIYhFqORBtFGg += 23833;
}byte LLAGsLkqDXfOZqpmgxxXL =  179; 
byte GPCXepM =  78; 
ulong UXzDlGDA = 1087775562230974; 
double FoYaAqa = 1.762749E-25; 
if(FoYaAqa == -2.833123E-20){
double ytPFjQukNPJOIWGPuELBlUyh = -8.711851E+22;
ytPFjQukNPJOIWGPuELBlUyh = Math.Sqrt(3);
FoYaAqa = ytPFjQukNPJOIWGPuELBlUyh;
Console.ReadKey();
}byte FWGz =  162; 
ulong oOjHQSEFwQEeUozw = 41740404873804749; 
string QciSdWh = "NMjAhj"; 
double AzlBuNfCRKxHR = 1.164075E-26; 
AzlBuNfCRKxHR = Math.Ceiling(Math.Cosh(5));
Console.ReadKey();short yxNiNsf = 23626; 
long XwHYgSEpyq = 48263145660506442; 
sbyte isuxglyjEkXDSngGVdLgbzfady =  43; 

}public void GRy(){
ushort PfFajSVutNltChzEXZSYc = 55581; 
long tsJDxFMOD = 87168515022149013; 
long LTIDpkyISGyHWwqIRjsYO = 28734343014481676; 
byte hqiZuMQtsYWUiemgNkX =  226; 
string NUKuQUkUxoEYooN = "XVbQZaSgpMwYER"; 
ushort FKFooZVVhzCIp = 12260; 
ulong tqmbSkzMcDeeAmpGSktFLzgYagJB = 19757496313256099; 
int CulSmMURwUhWP = 80; 
while(CulSmMURwUhWP == 80){
CulSmMURwUhWP += 195741;
}byte LCX =  67; 
float YhcIdbyJZtTluejMOtDxMp = 2.480976E-23F; 
long puQS = 88574185914286311; 
string qxQRuJPjRZOFMwaIwRWbMdThenQ = "hAtCoIeoBUQcm"; 
sbyte ReKSZannCpbimDJNf =  87; 
ushort qHl = 19085; 
string LhBaUPHCcdw = "RuTlRkmARMRhLj"; 
float kjSuP = 3.168904E-16F; 
sbyte BxmCMBFsKbahfPbbFAYpspK =  113; 
int SlaHheNypPa = 102143532; 
while(SlaHheNypPa == 102143532){
SlaHheNypPa += 25135;
}sbyte VeuRtkjqQwkMoZwJtscTWwJAl =  -121; 
long MdDnBgHlywHxX = 58974741095524565; 
int zYoUBxVFqEQFNSWkitjkYLHWiz = 39291624; 
while(zYoUBxVFqEQFNSWkitjkYLHWiz == 39291624){
zYoUBxVFqEQFNSWkitjkYLHWiz += 693299;
}sbyte epllfPCQ =  -52; 
float ikkhSC = 0.1889991F; 
short NstMcRBwaUgwDOHZXmuJhwNPanw = -17364; 
short ejRoqXTBdYbwoZaNIxCSFsfK = 16143; 
short ELPwodlPWFmxJgyjwIN = -24593; 
ulong nGYLzjMegDZtoPnEMscjehwa = 64704666338220353; 
float yjyEFbcSzKypRIVSVwdFLgj = 5.656658E+26F; 
ulong EufD = 68340862895662621; 
string lKLEDV = "mSgFhldqZAMSZAzs"; 
sbyte NopQMNIdzzxzuoREqCZXG =  -49; 
uint fHx = 21830201; 
long qEUPGixftXdOFUBfxbxhWtj = 68882531853364777; 
string LfVABkuUwGaCDTfFCaTGASXtR = "PSMbcjbXUPGG"; 
double bnBOjCVYjgEpUyqShCJixCiuD = 2.551772; 
while(bnBOjCVYjgEpUyqShCJixCiuD == 1.094576E-13){
bnBOjCVYjgEpUyqShCJixCiuD = Math.Pow(double.NegativeInfinity, 2);
int? yNpFGbHdWOuAefktbpZ = 1260946;
yNpFGbHdWOuAefktbpZ += 22001;
}
}public void QngRPzQCALBBJLEbPGCjuSfsbikIV(){
sbyte ZoYDfGOdfgobkzxalxoTzGbF =  116; 
sbyte PHnhHpWBlLiyxaCwcfRVTGiY =  -68; 
int oumuFXRYyR = 669478687; 
if(oumuFXRYyR == 948856){
oumuFXRYyR += 202481;
}short YSTX = -20642; 
double gnVjzoVabHgYqFBG = -1.161024E-32; 
while(gnVjzoVabHgYqFBG == -1.264184E-05){
gnVjzoVabHgYqFBG = Math.Sqrt(4);
int? QlZAymSTGUcbHYOGtImG = 9975688;
QlZAymSTGUcbHYOGtImG += 11319;
}sbyte xRBEAFtSxcXbuiwwdsTCEsGEGO =  18; 
float FHTFNXRMcP = -8.704507E-15F; 
int PNkLTHssIlHodkXYLaBER = 78048; 
while(PNkLTHssIlHodkXYLaBER == 78048){
PNkLTHssIlHodkXYLaBER = 573986;
}long ECk = 25191077131978140; 
double ZHYyTHPdd = -4.12409E+23; 
if(ZHYyTHPdd == 8.352898E-11){
ZHYyTHPdd = Math.Pow(2, 2.1);
int[] jwYPzhGadimVVVDgy = { 2870765 , 10820 } ;
Random IBxbCtmXkqT= new Random();
Console.WriteLine(jwYPzhGadimVVVDgy[IBxbCtmXkqT.Next(0,2)]);
}ulong FBasHVZXHlfuL = 63537435445101719; 
string TFDBXog = "dACpuCOQqJDPPxZDxJwQtwBW"; 
uint hOeubJBycUik = 38; 
short tFUmu = -3239; 
int enTpwIZGOcuaMWHUoqteukCbCnbVb = 9519; 
if(enTpwIZGOcuaMWHUoqteukCbCnbVb == 100502){
enTpwIZGOcuaMWHUoqteukCbCnbVb += 300452;
}ulong jcuVExeZntxYaYceBGUMSPgH = 29489584211087596; 
float foYiQ = 6.656055E-05F; 
string UFXapUGIA = "bXwVOSZXZpxNQf"; 
double OfqPfoQClbfEikejglRhLtkqRR = -3.994436E+16; 
OfqPfoQClbfEikejglRhLtkqRR = Math.Ceiling(Math.Asin(0.2));
 bool? OOllHEnPhwUaXoR = new bool?();
OOllHEnPhwUaXoR = true;ushort eaRKzEwdKBFCkEopLGoNaVciFBA = 61000; 
string IskIjPpLxyDkgEfDaKjuu = "FJViXIeYezabTmqDtBaZjSkPPAdbA"; 
ulong IoJNDNbRIGzsdkfLSlzpKu = 85136167656581184; 
string oNtMjBOQsKstLBgF = "OkPbAcmVUaeIHsdDYcqSu"; 
uint zJXacadmqBLK = 2722; 
uint KfRoAKhMqoRNyQNomSqtCckV = 759675152; 
ulong VJKnCl = 89408475277940390; 
long iqPCNxyCWeoHVVzMXaQJGDl = 18776183210053284; 
short xMBYluAkhqxXa = 21199; 
long gQBwaiHfEtMKxCtmYnzjYOL = 81937833604339086; 
byte mTNsU =  91; 
byte uuTIP =  200; 
int iYMsyQsfUfELwppkCGiIGVeVkeEtS = 521004836; 
if(iYMsyQsfUfELwppkCGiIGVeVkeEtS == 240563){
iYMsyQsfUfELwppkCGiIGVeVkeEtS = iYMsyQsfUfELwppkCGiIGVeVkeEtS + 618339;
}short SIyCIyPPDKeeJf = -20153; 
int snTRhsmYmWEVUiKbPKYMgYypgIwz = 1921; 
if(snTRhsmYmWEVUiKbPKYMgYypgIwz == 45400){
snTRhsmYmWEVUiKbPKYMgYypgIwz += 872873;
}float wFLokDwCkTJB = -1.826831E-35F; 

}
} public class xlURuQELAWOKcdmTaosGjZJCFE{public void fmZVdmpxeicACRTlsxTszXaIldxK(){
ushort tmRUzzAbLxNIhLYPQP = 20924; 
ulong eLkzhmsJaQH = 28203669839449869; 
ulong MxGQQoKxmIt = 9998172316836666; 
sbyte LHfJGVigiwVb =  -127; 
ushort GIsXImNhno = 19934; 
uint ckdjFXaPAh = 38; 
uint dmyIlUmaPYn = 114967; 
double StpuUdYsLyctAfAh = 2.520706E-29; 
StpuUdYsLyctAfAh = Math.Pow(2, 2.1);
try{
int QLsjnFcNlNSb = 2819108;
if(QLsjnFcNlNSb == 13348){
QLsjnFcNlNSb++;
}else{
QLsjnFcNlNSb--;
Console.Write(QLsjnFcNlNSb.ToString());
}
}catch(Exception ex){

}sbyte kNIPLFbImP =  -75; 
int XTYlExEDhTAKGZqsJEVuX = 590893; 
if(XTYlExEDhTAKGZqsJEVuX == 463838){
XTYlExEDhTAKGZqsJEVuX = 894561;
}long pKAbpfDVSLLENdUiefF = 35190456389202779; 
short LfCVWRiaJoTPxQ = 23191; 
byte jCLuxspXpMziTFOmAhGiJQlMh =  108; 
float WUOtfqCRFbnOlXHeKRQKfC = 4.755408E+11F; 
byte ito =  248; 
byte BFb =  107; 
double dCdWJ = -2.422012E+28; 
dCdWJ = Math.Pow(double.MinValue, double.MaxValue);
int? kgGOnGiSzOwdyjiXVusZGzSYYtW = 3464611;
kgGOnGiSzOwdyjiXVusZGzSYYtW += 72314;string XtJVTVIoMnIQVKOPQhQOIk = "CscQDdZ"; 
uint XIYbwNNKI = 251660; 
ulong NxTJOJjVndNkgDQeflMOwTm = 24970040232137965; 
double NGGXTpqpqJFkRn = -3.133891E+33; 
while(NGGXTpqpqJFkRn == 5.170227E-19){
NGGXTpqpqJFkRn = Math.Ceiling(Math.Acos(0));
for( ; ;) {
Console.WriteLine(-12.34118);
}
}byte LdnBqYdbTiUIahoWFbJ =  225; 
string gYsxCOaDChcfIjpDcuWjVixsJQlM = "dkqFNXpiUxpbq"; 
int GUBeHcZoXRcXDZOLCogyXL = 882730; 
while(GUBeHcZoXRcXDZOLCogyXL == 882730){
GUBeHcZoXRcXDZOLCogyXL += 157283;
}double iECLNPpheteVtNLZOjf = 5.159797E+20; 
iECLNPpheteVtNLZOjf = Math.Ceiling(Math.Acos(0));
Console.WriteLine(iECLNPpheteVtNLZOjf.ToString());ulong AmgcV = 8499089522686202; 
short XJplVVzUWDjpRmKdkAyDR = -26362; 
uint EEG = 8262; 
short nmUAXdmTjbsjAMeMzwIUGa = 16343; 
float gpJspjcFRWcXNNZXGmCzVboARg = 3.098296E-25F; 
uint mBbHQ = 58015425; 
uint SpFJPbPMJIWaNluXeajBipTgMKC = 113527144; 
double tetKiptAy = -8.623452E-07; 
if(tetKiptAy == 8.370417){
double KMNuaMCxzWfATGMwEVuDk = -0.0493594;
tetKiptAy = Math.Floor(KMNuaMCxzWfATGMwEVuDk);
object zNesGHjmnxkzYdIhNSxtXjQ;
zNesGHjmnxkzYdIhNSxtXjQ = 1.953833E+16;
}int CXFSNuy = 28205937; 
while(CXFSNuy == 28205937){
CXFSNuy = CXFSNuy + 936427;
}int HxfBRtklq = 671322; 
if(HxfBRtklq == 987205){
HxfBRtklq = HxfBRtklq + 117622;
}
}public void LFHXiusssYnMeTm(){
string SlURMNQpLnpdKPQiLdLpJdmeN = "SfAhyoH"; 
float OlHYRRHyfbnJjzDtPDMhOYPdE = 9.024677E+14F; 
float QbxPXbWdfTAbHhdzAI = 4.818139E+31F; 
ulong MUMcQyDuWgPTpaRTmxHp = 71161371531162364; 
string cVPhoDdUl = "BLXslNRcHGwlozYc"; 
long VYLYpKeUehLoQsjhPYM = 6380122720284663; 
float pimUzZmODwmnsJKsqQd = -1.528492E-27F; 
long pNK = 77547765401552073; 
ulong ywssWtlEzHENopHJOPzHiWhOAnMDE = 16261165444650617; 
long OyyRf = 64890391998570297; 
ulong QBmFSJ = 28859276636030797; 
ushort kcLLiMfcHMEZiupDpNUuJ = 57292; 
float fPxNzJVfqJYcNd = 4.57873E-32F; 
ushort XgfIBHOe = 10399; 
string bbqjoFuosEYRzJAwmPmPQ = "yWMpzoQZuBRwoIHTyzEPdIZpIZakB"; 
short oUcEwyZgfAsizZctSOusOkaUQXx = 9978; 
uint FzfFCClelx = 349029; 
uint LABpFGixgXh = 87; 
sbyte zasIRUUdTzChNDYUwNLjqxg =  -114; 
float isiyyI = -7.759431E-13F; 
ulong fKHxRxIRdzwTic = 83539580291184968; 
ulong MDUxbUdwPDPMfnsUPoZuLo = 50494342085733057; 
uint VWKnClQiuaeGnZkUHCnEouax = 7908; 
long wYAdaYRjEetDAQZKkESMVEztyYB = 7299787941482839; 
ushort aGoiapChLeAuplPX = 63849; 
double BgHbypnpiIbFcEjkXDDUmWkNT = -2.954593E-30; 
while(BgHbypnpiIbFcEjkXDDUmWkNT == -2.879942E-30){
BgHbypnpiIbFcEjkXDDUmWkNT = Math.Truncate(BgHbypnpiIbFcEjkXDDUmWkNT);

}byte PRVjmPztSSoqpYoacBmbVCpf =  39; 
float KtTXsnMGFNVACjQZxTaUAuMU = 5.290664E+19F; 
byte mBPbwYOZZVXdEsLmhaLRfDUlhX =  85; 
short YeFdTteYTeExQZCVXpgK = -2113; 
ulong bsNLHDTyHzqbQtZLZGooWhwoD = 56946675195501890; 
byte MfJzwaHWYzYLDYFgd =  43; 
uint UWzhnlJQxRhPFRmFqhUnolyQFHOm = 9; 
int OpJ = 63762074; 
if(OpJ == 169865){
OpJ += 334249;
}float gqjgMLaPVniIIAJD = 0.0005613317F; 

}public void NqnhaoizXQXMXy(){
ulong AwBcDcoINFI = 14612637680196353; 
sbyte jiSEmcZXDljWiihhwTLcHwxHhh =  123; 
float abuDyplIAoapK = -1.800199E-11F; 
uint gPQwo = 37; 
sbyte ONkwechDC =  -76; 
ulong LTWAWFKMWkdh = 81374005397458386; 
ushort CAydkXhgI = 10133; 
long wWUWtyKuoEJctcTfDQkPWA = 45206306234628500; 
int AqHmhdsCqbWXQuLhecESWTPVFzT = 83076; 
if(AqHmhdsCqbWXQuLhecESWTPVFzT == 496818){
AqHmhdsCqbWXQuLhecESWTPVFzT = AqHmhdsCqbWXQuLhecESWTPVFzT + 15382;
}ulong bxGYAzKzVEuGJhlICl = 11823170814987064; 
float ukxXcQJMzdetmuU = -1.972938E-24F; 
float uToonpPW = 1.185634E+14F; 
double oIleKgyESz = -3.637842E-27; 
while(oIleKgyESz == -3.234884E-13){
oIleKgyESz = Math.Ceiling(Math.Tan(1));
object apjkNotVtfRgYdLStyzpkIa;
apjkNotVtfRgYdLStyzpkIa = 6.702961E+20;
Console.WriteLine(apjkNotVtfRgYdLStyzpkIa.ToString().ToLower());
}short dqABspxlXZwGTziOstOu = -18111; 
ulong LKxbJWggPYSEsTfsHbVPDkVsBWnp = 10191949138830103; 
double OoQSGHaUQV = -2.358151E-38; 
if(OoQSGHaUQV == -1.882599E-15){
OoQSGHaUQV = Math.Ceiling(Math.Asin(0.2));
object EeubodRNcHZ;
EeubodRNcHZ = -4.118855E-34;
Console.WriteLine(EeubodRNcHZ.ToString().ToLower());
}uint zsRWQkG = 201248; 
double hwKNWnDelyywxsuFdSxplcbglSWZO = 4.942135E-23; 
double YsNIn = 0.001022758;
hwKNWnDelyywxsuFdSxplcbglSWZO = YsNIn / 3;
int[] FwGDNOKNftECeqE = { 5812630 , 77403 } ;
Random nlCLxRVPcTeE= new Random();
Console.WriteLine(FwGDNOKNftECeqE[nlCLxRVPcTeE.Next(0,2)]);short nxsEi = -5009; 
uint alwflzU = 18; 
short AAm = 30694; 
ulong RATJfiqdzmIZZhSO = 18611982655104710; 
string lORGbzFesWFAV = "dbOl"; 
byte jkh =  182; 
sbyte OpJnVTFMWElTkjuVQV =  -85; 
float htPPtNdjbGfzzLowdbMUigAQ = 9.608765E-39F; 
string oumDlPGXUACgCCVXfuN = "fBMTyZGaTYTtKxU"; 
long ElKeqBFAFtHl = 79634712930773287; 
ushort ljGdW = 40273; 
sbyte eInFDhcuOZhtSH =  118; 
ulong osffVfYZAaB = 29015156665089511; 
double gtkjNPBalGkQOHndpjCoFdhDUBwid = 8.502039E-11; 
gtkjNPBalGkQOHndpjCoFdhDUBwid = Math.Truncate(gtkjNPBalGkQOHndpjCoFdhDUBwid);
Console.ReadLine();string xdAdlSNmzuekWTgM = "MSwtCWOdpLFgEkDWYcNqloolSnSC"; 
ulong mVXlqRJfUgMaWgifVjDnYRtwLyDjC = 10138444567351365; 
short LnShHGktidMtIZzbxDDCTnAEIc = 37; 

}public void GJBVEpRZfJMPAdufPUxlR(){
short fIQ = -32384; 
ushort oBEfCNAEQPsiAWNQNbQTtONH = 18148; 
float HunOpukRiiJY = -7.298218E+30F; 
uint eecssopUPkEwBNXRJwSiCbcPnfEIe = 48; 
double PJDlAZseuZfQQpxkRpVctydbn = -2.764799E-21; 
PJDlAZseuZfQQpxkRpVctydbn = Math.Ceiling(Math.Atan(-5));
try{

}catch(Exception ex){

}double PITIDPDOkjQHbHesjcEw = 4.512993E+19; 
if(PITIDPDOkjQHbHesjcEw == -9.071207E-23){
double gmSUDqSKHNmDCwmOF = -1.235596E-17;
PITIDPDOkjQHbHesjcEw = Math.Round(gmSUDqSKHNmDCwmOF ,1,MidpointRounding.ToEven);

}short gsdpGCWkijXWuDpCUjnMmnqqQT = -29853; 
string JtaFIUJmARXEeeVZaqWYMPta = "GSbNAMAUwBQWqNIIow"; 
short ihiwqPCfddU = -31721; 
double bbI = -4.495768E+35; 
if(bbI == -2.513988E+13){
bbI = Math.Pow(double.NegativeInfinity, 2);
Console.ReadKey();
}short dnU = -27563; 
float YogsTIqIXNRX = 7.85395E-31F; 
short FmCcLf = 18800; 
int hnYMhtPFgFbhNdkxZDnzHEm = 93691242; 
if(hnYMhtPFgFbhNdkxZDnzHEm == 552029){
hnYMhtPFgFbhNdkxZDnzHEm = 829741;
}long BBGLtsVQtbsTBnZRjqHLDChfFb = 72109756173368055; 
uint hsRedNUxoKMAXRylkWpZ = 4985; 
sbyte ggsHQFtONtKR =  -5; 
uint QHLQUoNSHoaDDwCnIlB = 943770245; 
ushort HLqXMEhGNJ = 20236; 
uint IKlbuoLqPbQt = 899; 
string ZFKwsefEqanzKnUmUhH = "ZzEIKWRJBFCAfZNeHfaEEILubmICg"; 
int czcGCabMwmxdNzO = 6928; 
if(czcGCabMwmxdNzO == 623550){
czcGCabMwmxdNzO = czcGCabMwmxdNzO + 787641;
}sbyte wsOj =  30; 
ushort pDXfzbduld = 39171; 
ushort qgFXVmUXbqWKVwInNQw = 18432; 
sbyte UfHaNopnMbwDMnOQotDqnkaxDGwf =  -6; 
byte fztlzpUB =  151; 
long FVotJuAXCbMPJKfkzgUjPzR = 49425609405514940; 
double BHegiXaGCMhtKLDsfxtLy = 6.061659E-30; 
if(BHegiXaGCMhtKLDsfxtLy == 3.468991E-15){
double giOehcadojTEmMKW = -1.084769E-11;
BHegiXaGCMhtKLDsfxtLy = Math.Round(giOehcadojTEmMKW);
Console.ReadLine();
}byte jgKqOeY =  151; 
short pxtQgHeWYOiYzpdOWXsgLHGLC = -16177; 
long iEWJNoseyRxtRBFyZlIumZi = 66998580970144157; 
float sNuSBkjXZpeCZokbhMoGpBjuVxC = -3.119284E+12F; 
short DDyANjGAazxnKdAmWXIJWZnG = 22386; 
int WeJcoKBDZCPyRClohUmApxiGkaHTE = 8423; 
if(WeJcoKBDZCPyRClohUmApxiGkaHTE == 197260){
WeJcoKBDZCPyRClohUmApxiGkaHTE += 680591;
}
}public void qxgEiCBLgARgKK(){
sbyte GdJBVdRodIZLKhmISRXLNJ =  -47; 
ulong yzLjAiThgcfBiAnFSACCPbZlinT = 64426777647757950; 
int fEzgEE = 14; 
while(fEzgEE == 14){
fEzgEE = fEzgEE + 689090;
}sbyte jPngxAzalpgMhgtHEbJzNVdwlyU =  -105; 
string CcndMYGzbfIeshZRxY = "AfpaJhoLSUYGuXJOdyduwaf"; 
double QIEVwubVqEylbGWymN = 1.084743E-34; 
while(QIEVwubVqEylbGWymN == -7.722483E-28){
QIEVwubVqEylbGWymN = Math.Sqrt(4);
Console.ReadKey();
}byte zRUBjGlkzttoaRnYznsfoNH =  116; 
sbyte lKXzQnsMwLFBAMMUZCkywU =  -82; 
short SyeAMkxwytCnaEfXnxE = -8099; 
double HtwOemIqsBGTYRnmJAiebsj = 2.537526E+36; 
HtwOemIqsBGTYRnmJAiebsj = Math.Ceiling(Math.Atan(-5));
Console.WriteLine(HtwOemIqsBGTYRnmJAiebsj.ToString());long CQZaQXNqp = 49892700084970581; 
float uXLajbYVJDLmJSBmB = 0.4999967F; 
uint mKpRWOeAh = 9991358; 
double ooRQPYRbdPGSCAuj = -2.4394E+36; 
while(ooRQPYRbdPGSCAuj == -2.528371E-17){
ooRQPYRbdPGSCAuj = Math.Ceiling(Math.Cos(2));
Console.ReadKey();
}string IXw = "YcjQLFGW"; 
long dsCCODGTgjKNy = 2975382510028834; 
ushort XQobibAswaQOHJFHz = 27187; 
ulong lBlGAeXeyXKyIRELABxRHRnMI = 43691331824928180; 
int xFJDdewVyq = 1165; 
if(xFJDdewVyq == 889999){
xFJDdewVyq += 151036;
}double YjulAXEZTiVyfpdCgfQYsMlWRwpQo = 7.473397E-37; 
while(YjulAXEZTiVyfpdCgfQYsMlWRwpQo == 1.222378E+07){
YjulAXEZTiVyfpdCgfQYsMlWRwpQo = Math.Truncate(YjulAXEZTiVyfpdCgfQYsMlWRwpQo);
try{
Console.WriteLine(YjulAXEZTiVyfpdCgfQYsMlWRwpQo.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}byte sLY =  138; 
int uYKRVBtChljhYaNTiUAMAqwtbxO = 639129; 
while(uYKRVBtChljhYaNTiUAMAqwtbxO == 639129){
uYKRVBtChljhYaNTiUAMAqwtbxO = 200772;
}sbyte mLCjWMjOgySRmnzDdcSwc =  14; 
short zNRUUNpXSqAiHLDaoEoWjGBm = -21804; 
long RsLGKwNMjUpTtg = 51344514065173417; 
float AnuGSl = 6.30622E+15F; 
sbyte zGgLPNUZtfZ =  48; 
double nHFzSQdqBFgxbhTXpFLbIoTk = 16230.49; 
if(nHFzSQdqBFgxbhTXpFLbIoTk == -2.0092E-28){
double FqlIgStnheCFs = 1.940893E-28;
nHFzSQdqBFgxbhTXpFLbIoTk = FqlIgStnheCFs / 3;

}short MXNQyyszm = 10455; 
int FYqemmoWAfGIIfHOxO = 486868982; 
if(FYqemmoWAfGIIfHOxO == 458327){
FYqemmoWAfGIIfHOxO = 821262;
}long IZzhcCjOjBTltuCtO = 81919532501844396; 
uint ddhCBuLGpyqdezUWgVjuxYgIS = 608532; 
ushort iVfDKgKbZsS = 43314; 
ulong VyClHmuK = 53010025942861425; 
string qogDkBwqkVgEsMINlkqnYgW = "mnzobNCqqeqBdtla"; 

}
} public class ocAEeAsMsLoqNPjsIoWcfpyuP{public void mwhJSZpxJ(){
string ViPfDKclZEoghw = "oYMZF"; 
sbyte SmXFJSpaqfgVMfhWDHwhPJY =  -61; 
string OKsLKAshqC = "YajWYmi"; 
string JYhcitjWY = "nKEwdGJ"; 
int IEFOGOEGllQxPm = 58161499; 
if(IEFOGOEGllQxPm == 305203){
IEFOGOEGllQxPm = 534297;
}short wiinzuGgppH = 15453; 
ushort ekCnGNYLiNzxQRndtefs = 10348; 
ushort soiaeVBPdUfAGabWEqL = 65485; 
ushort bFPSmWDHbBIFjYxNTPIXwDKq = 43503; 
string hCayxjEnFlmJ = "mxSGRaRdBRzVsztzVy"; 
byte Guebclw =  156; 
long CGgeKYLdAIlnHLCDeLkPJOcAY = 34538086310096793; 
long SPnPuORYCWq = 57684944119554821; 
sbyte EHfRHTBUcbVYwJNRWQaRQBD =  35; 
sbyte sczVQxADoihXNVpeAVwPYQNpOFVqX =  -116; 
string MJCzwhlAzSuFJYc = "CMbIl"; 
short CohgkBiGleKletSJsKWpfxiIQCRQk = -31330; 
ushort uJainaaYp = 16522; 
sbyte YMYMJcKYoaTegAqJ =  48; 
short kKyRwtKwqJYZT = -13920; 
long dEfgiqyXuPVbOKsnXTWQC = 78329980032578240; 
string QzQ = "fFllUlByH"; 
uint HIqKbZtShjaNhBZhGtOf = 954029; 
ushort KMHcFaxWU = 22019; 
string YIYayGIceSdLUdUeSseKk = "apsp"; 
byte OdTQAo =  46; 
double OJYothSIVhBfq = -3363.383; 
if(OJYothSIVhBfq == -0.04150605){
OJYothSIVhBfq = Math.Ceiling(Math.Tanh(0.1));

}uint bJLdRGFaZhAFNOomNIgSPZyxkh = 8507; 
long FcpkgkKGLaFHbMHsUkjOzpyHdc = 42596566436042612; 
int nDIeEgWfmiXMQSm = 66; 
if(nDIeEgWfmiXMQSm == 269140){
nDIeEgWfmiXMQSm = 605423;
}long eZwNbL = 27595246896224422; 
string stquVezz = "xld"; 
ushort oSFwEfhtVwuTbEHOUBEKd = 58516; 
short laBXMGzOUl = 24720; 
short XaZdYzWabcVSmCM = 10; 

}public void GuEpiEFjIpWGZPKooXAFHf(){
ushort njXFfAdNnhHA = 50123; 
sbyte ywTqhZucmlDXElVfBYkbYGZ =  -105; 
sbyte jqRikCgjc =  -81; 
ulong nsUhKtDCBBtcfBDgPRwOiWPd = 25104332185976309; 
short LThRVGCGMJYOdKZZukOZGs = 3931; 
ulong GsCLfqILuQKZYWPmEkgfxaqbSaXX = 81536022513365510; 
double GqqdJToawoYNcgxLAGbBTfuKsoy = -1.605243E+10; 
if(GqqdJToawoYNcgxLAGbBTfuKsoy == 274.1158){
GqqdJToawoYNcgxLAGbBTfuKsoy = Math.Ceiling(Math.Atan(-5));
try{

}catch(Exception ex){

}
}long MnyIMaYm = 58690547332884408; 
sbyte OxRNBoDXDmLjGDDpnxBIbF =  -94; 
int GPRxeIouu = 246613757; 
while(GPRxeIouu == 246613757){
GPRxeIouu = GPRxeIouu + 155641;
}sbyte zMCLwGIefkkALipmAUAMBzGx =  79; 
byte pWoIsDmgwcDkiisZxdUJ =  183; 
double zUJtna = -1.204332E-30; 
double TXybUjXJpDizisWDWs = -1.993751E-27;
zUJtna = TXybUjXJpDizisWDWs / 3;
object iZgoXIakRjjUkWkKcXCks;
iZgoXIakRjjUkWkKcXCks = 1.384818E+24;
Console.WriteLine(iZgoXIakRjjUkWkKcXCks.ToString().ToLower());byte RoEqsVGsjKt =  46; 
float kfahNPyMmCneemJHYqPKDLjTx = 5.99426E-17F; 
byte FpVT =  44; 
string gSEzSfb = "cnQs"; 
long McuinAWhMjAjJJlskV = 12834991771092755; 
long QhOXEAT = 50311316870770327; 
ushort JxtQLcUijdGodqRQOqEwCGjPYRAaF = 16638; 
uint sqiapqffTPuwnYxudOzIEYycmESTD = 570093399; 
double GXNiRBykyDNlOCZufX = 6.208867E+21; 
double zlDakzSIaKklnbxfxLRAcTz = Math.Log(1000, 10);
GXNiRBykyDNlOCZufX = zlDakzSIaKklnbxfxLRAcTz;
 bool? LiETcdOY = new bool?();
LiETcdOY = true;uint yjVi = 56; 
sbyte ODjitpSIKOQetkQydyOuCUMEeLX =  -95; 
string kLB = "ZDUoXRHPgusuKmuzfHPPsU"; 
float eJsLGJDeFIlmDOBWNNNs = -3.269058E+14F; 
string MEVFVLtfMmknAVjzRmPcah = "sgTcGhzxuHIZbOYjUnfBoDlyRDo"; 
uint UYKFIDycJNXmRQHSWhKTdVOUHHPV = 208494; 
string YzBJXwtkBhlBhnyNFGVwFIUYkP = "bODHyLeotpYVCOzsWKnMticpQpIn"; 
short CyhysyYBOQdYexGKtH = -25281; 
short oXcFSLlQhdQYPOBNexbuwVyJueGLf = 28594; 
string aGlBeSA = "MKHMmapJeXokeWDtAukOeC"; 
long dIXiCaJl = 75368743007450953; 
ulong GiIPdecTbLHMIkZwJFcggeocoQnR = 14265196338686737; 
int FTgJYYFjHy = 88451392; 
while(FTgJYYFjHy == 88451392){
FTgJYYFjHy = FTgJYYFjHy + 653187;
}
}public void CUGBnRlPmNQOBoySwaiazfqyf(){
sbyte ejMfBqpuIXNpcUTMkPxginbHR =  -91; 
long wDiwfagGCHox = 72064333083887643; 
int fIJbIRieosKXY = 77; 
while(fIJbIRieosKXY == 77){
fIJbIRieosKXY += 778840;
}short FAZWnzfZ = -22549; 
string ObyBMdspxysXNmWLg = "zeqpkBhKeus"; 
string gshiX = "nGgKzRNmDlKqOZKKWkzR"; 
double cZXXuskFGdYd = 2.101274E+14; 
while(cZXXuskFGdYd == -2.616853E+26){
double zudEcxyxXLgKVEhNBxwb = -2.231484E-05;
cZXXuskFGdYd = Math.Round(zudEcxyxXLgKVEhNBxwb ,1,MidpointRounding.ToEven);
for( ; ;) {
Console.WriteLine(9.785585E+08);
}
}long OKTXxeUFLNZhZDSgeAnOXmBqh = 40079788197465202; 
ulong jBuAJodIetRfHPjXEXTVBjDQC = 41225646551909962; 
ulong GKZFATGxXpOCeQaNClRpa = 69589851655473877; 
float RUgEmFttbVNFAOxVNNLZQyx = 8.571003E+10F; 
sbyte Skt =  55; 
ulong DWHYl = 13472718019083535; 
sbyte ptNgXUYu =  -14; 
uint gDBiJjYBWRPWBZDQXIcunZSCSHFR = 5833; 
uint BpHGIKg = 28; 
float tCxlgwd = -3.673618E+19F; 
ushort JhLY = 20269; 
string gosoGxktWinBSfuQzax = "zxhFPPyHzVJue"; 
int ERdWZbO = 902917; 
if(ERdWZbO == 538531){
ERdWZbO += 950514;
}short lYUIgKhOBQZE = 25888; 
string UwnYQZuextLYlSDm = "sgJLBxyywcEfayTx"; 
ushort RnZzsRUyoxMVWYEAkNtPtYQBuW = 53685; 
double VRCRZLYBUPRQch = -3.5056E+16; 
while(VRCRZLYBUPRQch == -8.104447E+08){
double dHzSyzd = 1.035561;
VRCRZLYBUPRQch = Math.Round(dHzSyzd ,1,MidpointRounding.ToEven);
try{
int tGkeW = 8157984;
if(tGkeW == 20251){
tGkeW++;
}else{
tGkeW--;
Console.Write(tGkeW.ToString());
}
}catch(Exception ex){

}
}ushort JMykBTCbRtQVbOwBwjtWCWgCIhb = 42295; 
long FQzfKTN = 85754961591002106; 
string cuoAHRBtNFV = "tSUtIPZyVeCNBNZTF"; 
ushort QESYmtKYZXMWoSqdD = 12328; 
int QNHuYSDYTSIPqcfYBTHZXQaAi = 26; 
if(QNHuYSDYTSIPqcfYBTHZXQaAi == 691048){
QNHuYSDYTSIPqcfYBTHZXQaAi += 439705;
}long FMc = 70420959051530006; 
short GlMtDJDFUZMhkRFIPjwYPNqcbcUl = -22539; 
byte TilCbiQgOgwKMpQmiPjWVM =  88; 
sbyte VJqBPqxInTYZoEzYcHSucFMlQL =  -18; 
double RhReJgBAHZP = 5.702552E-36; 
while(RhReJgBAHZP == -7.449164E+24){
double XSeBNQpWfKdUzsxLhUGyYAw = -1.897368E+28;
RhReJgBAHZP = Math.Round(XSeBNQpWfKdUzsxLhUGyYAw);
Console.ReadKey();
}float JbMKtnPNzxxysQiI = 1.431396E-27F; 

}public void KlazIQDNG(){
ushort zNhVqf = 36377; 
long FwAUCHT = 46762116227576939; 
short EdTQBitqXKpxzdebYHnM = -23717; 
double XEK = 0.0008851453; 
while(XEK == 1.881211E-35){
XEK = Math.Ceiling(Math.Tanh(0.1));
int? WmRlVS = 5921627;
WmRlVS += 34706;
}string afsGSUZYehUxU = "makKpp"; 
sbyte PdlwkAWWWoYClMToatIwMIH =  -82; 
short qawVbOlnqnuyJBch = -18522; 
int tUBhWKWIObgycfM = 91; 
if(tUBhWKWIObgycfM == 262664){
tUBhWKWIObgycfM += 200895;
}short wFVELgtEybMXFQSAkefzmjRqqXsgH = -20275; 
short cVfEbglCu = -3046; 
long qhgsdcQieSMkNE = 19319250834813906; 
float KoyeTFdPBwaiRacFtiikj = 2.48966E+23F; 
uint ANhBxMSnyIOFMoBPXKsDcWOjsTB = 6802; 
uint AZjtRVneJiVttoaITJxmlyVS = 990746; 
double ElDFYTo = 87.89931; 
ElDFYTo = Math.Ceiling(Math.Tanh(0.1));
int[] CyGpuAa = { 8996951 , 33744 } ;
Random gXZEEHOBLaFcRsF= new Random();
Console.WriteLine(CyGpuAa[gXZEEHOBLaFcRsF.Next(0,2)]);ulong mAjfRVEdlOR = 45465684483660474; 
long nFHPmUniTLYIjNWWSiIaDCMTSKhV = 27302377823574591; 
int EaoVtcSALhZ = 83; 
while(EaoVtcSALhZ == 83){
EaoVtcSALhZ += 379484;
}ulong NXDNtVYzqFQOatCGILIiKasZa = 10984238453618190; 
string tanWZuFCVVtZPpTtXMEhNFEM = "QVJLiQCXDiFuPIO"; 
string ydIWpNVlynKVdPxC = "jAVQMeRbmgoZidP"; 
string gBeDBhOzM = "SbAuhSqzoiJLYQUdG"; 
int ojUSQIWyV = 33145623; 
while(ojUSQIWyV == 33145623){
ojUSQIWyV = 556552;
}double NNg = -5.519702E+25; 
while(NNg == 3.428533E+28){
NNg = Math.Ceiling(Math.Sin(2));
Console.ReadKey();
}ulong mzBQkjDhalgkPemTFs = 41505763676731910; 
uint guYEzz = 595389; 
uint DkmwSb = 887062; 
long UQcPXYOiBaAzLybGmENpC = 38522358378965345; 
double xdOlpsjGAKudAHdBQtlPagsilxEZ = -7.030792E-07; 
while(xdOlpsjGAKudAHdBQtlPagsilxEZ == 5.825934E+07){
xdOlpsjGAKudAHdBQtlPagsilxEZ = Math.Ceiling(Math.Cos(2));
Console.ReadKey();
}long zocNh = 33997166934397767; 
short WJwa = 1254; 
string PYltRASGL = "tEexlcCYVdotMNT"; 
uint WPOnhekAdCOCCD = 6958; 
uint PlHLHUbG = 960572869; 
ushort ABpdWQXjk = 31069; 

}public void WhCNytCcJkDeoPkiUmDhmdRcnkz(){
int NqapqKEegElwVO = 92; 
while(NqapqKEegElwVO == 92){
NqapqKEegElwVO += 711469;
}long XJqI = 18516898145082891; 
double DOYeIdNJqMqpPfSijVOKtqZKQmG = 8.836549E-15; 
if(DOYeIdNJqMqpPfSijVOKtqZKQmG == 6.170743E-17){
DOYeIdNJqMqpPfSijVOKtqZKQmG = Math.Ceiling(Math.Cos(2));
Console.ReadKey();
}sbyte WssofTJx =  115; 
ulong ISwkuUb = 9871606670233979; 
string tqdkchiIbegSEGoTs = "NkZyUUXVhRWHNnQhcHJXFLmDs"; 
ulong RIzQDtKeSDVximgasLwqScxDJjw = 52026068112895421; 
byte buncjYGhjBLyKURXAcPlV =  33; 
short ybUfQsigUqppgnIdQuzKKPtdFgFWp = -31654; 
ulong cpGheAqRqEEtFCRlwMxBIJnJJQC = 60179863047862864; 
float AZqdHtgkZzdpsCFFmmub = 0.1111556F; 
byte IddNLQimaEVMeybpgyJsytwLlnpiU =  202; 
long Lsx = 82602709982915824; 
ulong SAQkeVAQWBnItByBatk = 20718252504858749; 
double cHdHetOAXQUkjPR = -3.526022E+33; 
cHdHetOAXQUkjPR = Math.Ceiling(Math.Cosh(5));
try{

}catch(Exception ex){

}long gKlpWFIoURFYRsXUE = 71864663055346805; 
short iOVPwZpOnMGLhMusbjqklVbKAkiy = 7028; 
short VHHA = 1053; 
double jYBE = 9.028692E-27; 
jYBE = Math.Floor(4.837349E+15);
 bool? BlTtZas = new bool?();
BlTtZas = true;byte zCRPoZLucbTzEYwgMSHS =  143; 
int fGm = 19; 
while(fGm == 19){
fGm += 398922;
}uint OTVJ = 7400404; 
sbyte TjVpja =  60; 
uint VSweTCMjcXCqOEzZAkDkd = 55; 
ulong MQjocbyCGDDdYU = 47975576593915364; 
int qKPjpGSoJJhLCU = 925190993; 
while(qKPjpGSoJJhLCU == 925190993){
qKPjpGSoJJhLCU = qKPjpGSoJJhLCU + 14631;
}double BpkbFqxgGIjNYtlnlZLnwoNHIFN = -77.38432; 
if(BpkbFqxgGIjNYtlnlZLnwoNHIFN == 1.12335E+08){
BpkbFqxgGIjNYtlnlZLnwoNHIFN = Math.Floor(1.449523E-08);
object EEuaFlGGMF;
EEuaFlGGMF = -1.077223E+11;
}float fBLxNpUVYVxUbVGWyOWE = -6.511495E-05F; 
double FjelIZqyBqAMUiYoq = 3.242296E-23; 
double PzjpoJEiSDkImZFYtKxUcnPHlLle = 2.005119E-16;
FjelIZqyBqAMUiYoq = PzjpoJEiSDkImZFYtKxUcnPHlLle / 3;
Console.WriteLine(FjelIZqyBqAMUiYoq.ToString());int paRQQYWDSJiKhzuLl = 13; 
if(paRQQYWDSJiKhzuLl == 863112){
paRQQYWDSJiKhzuLl = 442278;
}uint DHnjDF = 44; 
uint wHJBqzIKfE = 3; 
sbyte afWlTcyE =  -51; 
long sJspeRKGkY = 78348788022663953; 
double BzcjxI = -3.318448E-28; 
while(BzcjxI == 7.652523E+26){
double PptaMswYlXDxxHknlsDNUGiTyNDfu = -5.669558E+37;
BzcjxI = Math.Round(PptaMswYlXDxxHknlsDNUGiTyNDfu ,1,MidpointRounding.ToEven);
object RxCQqkOfAQl;
RxCQqkOfAQl = -8.677792E+17;
}
}
} public class uQWZkecRSBNgOMnwpNlY{public void VLbUdcoHdLRIRKtkBsUNzMGy(){
float TXnihVQSAeUfh = -3.47424E+19F; 
byte PWzXbUjQwqlqjeEbKjssUEVR =  247; 
byte pujTZnWQCu =  109; 
float dsNF = 179.6765F; 
short pez = 22306; 
ushort UlaIZQGUwkylcnncHmspoCRqm = 28438; 
double ZpMbFxbFCM = -7.725449E-38; 
while(ZpMbFxbFCM == -2.602283E+34){
ZpMbFxbFCM = Math.Ceiling(Math.Acos(0));
int? lKBUQqTeFojUXUDLTBCB = 9983922;
lKBUQqTeFojUXUDLTBCB += 80825;
}sbyte YUgNEBJEV =  -79; 
ulong NmAidfBYpetowVctQognfGdQ = 74256125154873073; 
ushort bTZqCUokWUCSsJzoNGcC = 45138; 
sbyte KGYUaTp =  18; 
ushort NTitzOlYVJjoMODcPmVVUYULpM = 36375; 
string qHpgYFPoWn = "QDPyYuTkqXedIzYPYzdMllJEc"; 
float NiNNTYe = -7.337788E-13F; 
byte MVSsKsmEIIaQRR =  219; 
uint FTkwemgbEjleEuHGCMh = 867384161; 
uint qAlBNsWWaxzB = 970518; 
string JMqghbzdntkoeLINcCUW = "MuJHnlJeTiHBeAhDwjobhluhIR"; 
short MyWeeOWsIATPPZujEOalBjI = -10648; 
int LBZCYxWUsxmDxqDPVWqBliSjEUR = 26; 
if(LBZCYxWUsxmDxqDPVWqBliSjEUR == 593643){
LBZCYxWUsxmDxqDPVWqBliSjEUR = LBZCYxWUsxmDxqDPVWqBliSjEUR + 489692;
}byte xsgB =  112; 
float HXXbsKJLoMaeJDJQXeFTHQHIdln = -4.563295E-05F; 
ulong UHqDUmDDhuDmd = 11686070768567638; 
float mQppYfwDnBpgysaOEwDyAeEEdJMOt = 1.060164E+08F; 
double IdM = 8.12137E+32; 
while(IdM == -1.584158E+17){
double yXhAZZOUuBimNlpuIjtVqeQo = Math.Log(1000, 10);
IdM = yXhAZZOUuBimNlpuIjtVqeQo;
for( ; ;) {
Console.WriteLine(-1.437175E-24);
}
}double dHZVZtnDKdEDTpXnD = -7.224827E+28; 
if(dHZVZtnDKdEDTpXnD == 6.335615E-22){
dHZVZtnDKdEDTpXnD = Math.Ceiling(Math.Cosh(5));
object SSSWhAmkudjIgfyGU;
SSSWhAmkudjIgfyGU = -1.01012E+24;
Console.WriteLine(SSSWhAmkudjIgfyGU.ToString().ToLower());
}int TfMXjtXYWJgQNpHaKh = 10; 
while(TfMXjtXYWJgQNpHaKh == 10){
TfMXjtXYWJgQNpHaKh = TfMXjtXYWJgQNpHaKh + 586644;
}double XbYCbiBTNOJiwWPJiLxKP = 3.979337E+18; 
if(XbYCbiBTNOJiwWPJiLxKP == 0.0598134){
XbYCbiBTNOJiwWPJiLxKP = Math.Ceiling(Math.Acos(0));
int[] UESsBNmpCESh = { 5302220 , 19608 } ;
Random VboUDDwaAHk= new Random();
Console.WriteLine(UESsBNmpCESh[VboUDDwaAHk.Next(0,2)]);
}double TIOdzuVtUNzMBjSD = 4.31855E+36; 
TIOdzuVtUNzMBjSD = Math.Exp(2);
Console.ReadLine();uint RfnRCIlqTRPOBkgkXVhdIqGm = 70747112; 
ushort DhJlHLGhJptfCHTeBceCgORuCwgB = 33994; 
string QnmPMiDh = "GZCHCbCazBNFGRRPqnLkEeHJ"; 
sbyte SnShVXVoWhljTBTfca =  -62; 
int yxZuUSWl = 357652; 
if(yxZuUSWl == 968698){
yxZuUSWl = 180794;
}byte pFPKqVJ =  138; 

}public void ukszcG(){
ushort swSSNbfYcwPtJUqF = 62934; 
ulong zVTCNynBIFOyMz = 5703272079376269; 
long AUjzAUNBqk = 58138260245842527; 
uint oYcUIoDiJiROk = 45441; 
float PaNRngCCwf = 7.482425E-13F; 
float clqZC = 2.884443E-16F; 
float qFRitiuV = 1.363738E-22F; 
sbyte BLEbSCsliHROePDUFCcYY =  74; 
ushort xXhtbkegO = 24926; 
uint OYKoXFbLNdeFGBppi = 72; 
long pFUtYkXDLa = 10442696054990852; 
ushort LAtTENCVPYGIQlytPWYteic = 3561; 
ushort FuJzwgsYERla = 53367; 
byte cNNwcxPJnbRcaJDypAkig =  60; 
short ApSJHgPfNKbwoIqVQ = 14090; 
ushort EdqoatWCFdDQxj = 60378; 
ulong fcgmMppIhiIhpswRVY = 80148952349127930; 
long lZfMFq = 79579028642578763; 
float ntoLgZCl = 2.722398E+19F; 
string PWZlHiwhdWEYIND = "gJXUOcWUkYalyUlQolTsiH"; 
sbyte lLhsPpcuxPeSRhKMZfaIijLiL =  67; 
long TjDACfIcVFeUWH = 64711919812538979; 
ushort GzJca = 63047; 
long HHB = 78749224213450959; 
ushort gMD = 25211; 
string qPEUMRzTdxGcX = "cnpgEwidmHVqYdANgJXEpmcNOwOM"; 
byte UFSxXSxwRI =  57; 
byte WYwSwtqpWeuHQkDsh =  163; 
ushort YNqzDfIpLSpsdLgPtDssVZXAz = 52207; 
sbyte TcJaPwEiHWppMaFeOcEf =  -105; 
ulong GodVZQSaNYaxIipDIgXhxlcHVuSMw = 61465693473397771; 
uint tPWEqMEWWyAVZgGEtm = 12858; 
int fFBXzsFfGShCFtBJLVqyWnVpbwzB = 46653033; 
while(fFBXzsFfGShCFtBJLVqyWnVpbwzB == 46653033){
fFBXzsFfGShCFtBJLVqyWnVpbwzB = 406131;
}long cumlsQSIOyD = 66256741165455409; 
float kVdTewgFJDjLjGpIefgwn = 687617.8F; 

}public void lyZWLQStRISzjtnuLdfVtDTel(){
float cVxwJqkHHxkTTKQUCO = 0.002672439F; 
ulong MLkyKAKFULRlQzumihYNHYBjl = 26173121902782116; 
byte MklIMWb =  134; 
uint ITqA = 5894; 
long ZkOkakzVbnEpwtGo = 31056713385435767; 
double FslibNK = 3.399959E+28; 
FslibNK = Math.Floor(-4.68223E-37);
for( ; ;) {
Console.WriteLine(-0.01860861);
}long MSRnOSwsocBBy = 47493680860178719; 
uint oTpsmtCxaL = 3759; 
long xGmVYgaHCewxsZwwQTlFCwzAiY = 27220127978474334; 
ushort BWDtnIqkWQaDbFUBhcq = 1484; 
sbyte MZuwgMbcXwGPVTpTpTdmEsY =  -81; 
int VCjxGszNjDYTCGiTTGXlKakoNyX = 196200784; 
while(VCjxGszNjDYTCGiTTGXlKakoNyX == 196200784){
VCjxGszNjDYTCGiTTGXlKakoNyX += 824521;
}byte ATBPmqhJOUyszdKAslslVqVK =  40; 
ushort DQCOcpPJasQKVCqywo = 7133; 
uint BUKYzZViXRHSNFSlgwbJuJgD = 448398; 
long UBucUOcWSPEqUBcCiR = 77174411307591303; 
ushort NgxbosnyxFXZDWuqSKPUHGtLo = 49510; 
sbyte lBYdsliLtCbClFfFUiuDQ =  115; 
string cCkhOMMCmFlWG = "HqOKXpImIbBPQgtDUnzFbRabWI"; 
ulong oItiYZCcPfaMMPUi = 59623638632045404; 
uint SQp = 75500120; 
short hXZIqQRFpOOnMSlX = 22331; 
ushort NkqFXwMWhMNcsLmERUCpieaSWWKM = 11961; 
int zsQDoRtcmGTKSEMwEAoudqdPJk = 649482; 
while(zsQDoRtcmGTKSEMwEAoudqdPJk == 649482){
zsQDoRtcmGTKSEMwEAoudqdPJk = zsQDoRtcmGTKSEMwEAoudqdPJk + 754796;
}uint nKDhXNzRnLowhokCgmVlPhoaUd = 7228; 
string GfiYobSwCCWlcsAmd = "xzENSoeg"; 
ushort mUuNPLfjliZjJLLtp = 36956; 
sbyte CHahoEZSnTUiHpiIhjxslpbVkmQS =  -117; 
uint CbyaGUlylMnVsnRDtDdkfgNLlGeXb = 14205448; 
ulong UtgipDOVMkHYuJHuKxoYnTu = 25861258457185857; 
int hzVKucZpouVgNSX = 573321; 
while(hzVKucZpouVgNSX == 573321){
hzVKucZpouVgNSX += 827455;
}uint YSx = 564834; 
double pNbBmkMKpuoJYzAhaVbp = 6.288567E-34; 
if(pNbBmkMKpuoJYzAhaVbp == 8.013393E+09){
pNbBmkMKpuoJYzAhaVbp = Math.Sqrt(4);
int? TGOPTsUTqpuq = 1882836;
TGOPTsUTqpuq += 18347;
}uint TzgCTV = 72768243; 
float PtZZXaKcKQkhgIIpm = -6.495919E-31F; 

}public void ygwAVqNQ(){
string JFEQnwppGRqgBIVLqzUDIEHGeJYBd = "mUcLhHFqumaXzoUpGNEj"; 
string VypeYXMGyQ = "ZExePBofa"; 
string DLfqMOckWTHYYFwcIIFO = "jMAMykRloeZjoVGZHkYDlTss"; 
int BuyHzwpRsZHGnDk = 656446580; 
if(BuyHzwpRsZHGnDk == 789585){
BuyHzwpRsZHGnDk = 694838;
}long dbyHkEqtggaLyPIHFTyDHtaW = 21506859248317927; 
float uGbPemaGAAnROjaicVm = -3.865749E-26F; 
long BkT = 34324702768850787; 
ushort FPOj = 35016; 
string CJEFcjtJUBLxJFBFlWUOdhbXVP = "EmgcFhRASHonWyNyMjxMyekDmImQu"; 
int JnlPwNzfnSfJXWJXppK = 7617; 
while(JnlPwNzfnSfJXWJXppK == 7617){
JnlPwNzfnSfJXWJXppK += 492127;
}string PXVaekaHaLJqLndhUq = "mGFKWBLsTijKpt"; 
sbyte itRayXpJmIYYbKsjjdPYj =  -62; 
double uThyQatYDEVOcqYp = -6.574828E+31; 
while(uThyQatYDEVOcqYp == -234.7704){
uThyQatYDEVOcqYp = Math.Floor(8.468398E-16);
try{
int YIEYGPAguteatLLtBxdjZC = 175868;
if(YIEYGPAguteatLLtBxdjZC == 83308){
YIEYGPAguteatLLtBxdjZC++;
}else{
YIEYGPAguteatLLtBxdjZC--;
}
}catch(Exception ex){

}
}byte NhGc =  95; 
ulong xhMXmkIM = 50640941327778868; 
short TXKsNtKiayRWnzJcxAuhnZxXWH = -13087; 
short gUgpjBPBewtndkjUBcmTpBsIhF = 24429; 
uint wfduoTDLHdYTDqeaTjPlcwF = 41; 
int DAhGHnNJppTOqIjUZna = 17533102; 
while(DAhGHnNJppTOqIjUZna == 17533102){
DAhGHnNJppTOqIjUZna = 386038;
}string KaCaZLmakz = "sFPIPLaxyFTBGPw"; 
float oSTlDZdJOpUdeI = 9.032713E-07F; 
uint wXCzWoYILRAWkFTQLsGlQGMZauf = 3667; 
double eeTZTWRWFnVomjSeojnWGQnfQkC = -26.06262; 
if(eeTZTWRWFnVomjSeojnWGQnfQkC == 1.515997E-18){
eeTZTWRWFnVomjSeojnWGQnfQkC = Math.Ceiling(Math.Cos(2));
Console.ReadKey();
}short QjahDfYMZe = -483; 
uint MWIeidkyNIKZipSTA = 18; 
double IDLhKlzyOEGBbJwSySCfRdCqhtQf = 0.09160092; 
IDLhKlzyOEGBbJwSySCfRdCqhtQf = Math.Ceiling(Math.Acos(0));
Console.ReadKey();uint ADfpenLMSb = 31615235; 
float IsEjlb = -6.205189E-08F; 
string jzflNUIJklZRyHdi = "LRwfiTVOOYONxWeCGgHh"; 
uint YtYfIhRUXfdtIFGwbGkzYVTWOpVoS = 448110; 
string MTbgckNX = "bJJcnAWjebWaacBcsqmDcyIcjV"; 
long nYuzBfTknLcwtIwxej = 32021947317728573; 
float aGTsOsWgHzAUjoOHoS = -9.546827E-09F; 
string YajVcjnBY = "oojISK"; 
int MoP = 69296545; 
if(MoP == 227980){
MoP = 314627;
}
}public void FIlxD(){
ulong SiToNTgBtGUfgPGlhJNx = 23560074225632492; 
short TMwZZCNMZxJeqkMfjoYxdNQsoCP = 5245; 
uint JjkzNiLgPdzxKqOmZhDLKUXURf = 57003203; 
long MinfNzUSzDwcAVX = 36034959427422427; 
ulong OmpIImIy = 23323909939948298; 
sbyte apHwbWjcyzIWdhCVYkFjQ =  -49; 
ulong hEIAzUoJuYayDtktzGP = 63126369537853166; 
string XSKDcmznKEEjYDegdsFakMkB = "QNoCTPnkUxfqVBgNfO"; 
string ZQTiZDqbd = "lIiYlBiNQOoYegYQiR"; 
uint eXioFZTgCp = 248357; 
sbyte jzhXnGEsYmW =  -125; 
ulong jjuBDQAnzTtmHclAqcRPWBI = 56077016138197892; 
int gWqWXKsRV = 28; 
if(gWqWXKsRV == 982502){
gWqWXKsRV += 519829;
}sbyte WUjXQKCiXLfMoDC =  72; 
double fUEtsAXaAtHCTiHC = 0.0009445107; 
while(fUEtsAXaAtHCTiHC == -6.662086E-16){
double ZleHyNPbzwlUDDLZQjhDxRWfNEduK = Math.Log(1000, 10);
fUEtsAXaAtHCTiHC = ZleHyNPbzwlUDDLZQjhDxRWfNEduK;

}ulong WTAhXxhg = 89861253955076163; 
double AILho = 2.036501E+19; 
while(AILho == 8.316183E+36){
AILho = Math.Ceiling(Math.Tanh(0.1));
int? shiAPFLuwMiExGiVNLXgWBkUwk = 5231122;
shiAPFLuwMiExGiVNLXgWBkUwk += 94614;
}float ndFMyCoBsICnTLmZpUqqJpJKbVUy = -5.608132E+08F; 
ushort AsWxhWOdUaleYDznwOUHHAmXwUKD = 41282; 
float MnFP = 1.014846E+20F; 
float tgmyJdFLAxGbKMOqZfYamlkyb = 15.06945F; 
float NXAFXBAucFTRi = -3.069601E-29F; 
string sZAmPwQQEhWPxKEOqMzfyNknX = "CejdbdxkGBqgYDKOttEFHUpdSzIU"; 
ushort PkpRULGAfSskdcxjOWx = 43696; 
ushort LtqHOglExDQaMfRgJZhTnLlwR = 17682; 
int KMzMlXBXBmWAnCxQwdpqs = 26955843; 
while(KMzMlXBXBmWAnCxQwdpqs == 26955843){
KMzMlXBXBmWAnCxQwdpqs = 563004;
}string WQBBDU = "dePPGUGwcJGNowaOYoqcy"; 
string LuAQFywY = "HMNpnBLybTuRVQMMWcDklR"; 
byte Udwf =  22; 
string HLUfDJKAyETbFItQEOeJwTOAQAKx = "fGtZVNgb"; 
long clDeHsP = 63084337494596749; 
int WuodYnnsYBbKUmPTGbySOgh = 82; 
while(WuodYnnsYBbKUmPTGbySOgh == 82){
WuodYnnsYBbKUmPTGbySOgh = WuodYnnsYBbKUmPTGbySOgh + 264576;
}byte HsZjHweeXoNI =  216; 
ulong BnyZMYVLZQlolJsqdLgwqVchyCXAE = 14244952702541783; 
short uLIauwgSwFXJXLsHF = -18183; 

}
} 
public class KkSVWQROSdDUzVSBWoxXBJGMCZ{public void dMSAszlolQnwxgRd(){
sbyte ANiNbfzmxtbGgtXoOc =  124; 
int wlHGKJTa = 692159603; 
if(wlHGKJTa == 153186){
wlHGKJTa = 42200;
}byte pKNaecTnHKUz =  34; 
ushort eCtdBDgqxFmGJxfYqLXytmD = 7136; 
ushort sZaPwTyWBXPY = 46461; 
ulong sqxSxkRC = 7623241487286230; 
byte ijhqIitiNWhjsagnaWgnX =  38; 
uint olJJCBJZBnDHcaJ = 4700; 
byte UtBjMcIuVSaNaIJ =  10; 
ushort LIlniKezBkamTPKK = 41389; 
string bWHTgNgBCCYtYnNOGlHkRfn = "yuJZkdpbFntEdOfCb"; 
sbyte WDSbYfbdzaTXWJexN =  -111; 
sbyte zkWOLUjKRgf =  -84; 
long sZFsLTjZSbLqyVZssy = 6958935300340861; 
double lsULntpeRut = -1.404753E-30; 
while(lsULntpeRut == -5.469812E+30){
lsULntpeRut = Math.Ceiling(Math.Cosh(5));
 bool? gaceFVBXeGNGqLaWH = new bool?();
gaceFVBXeGNGqLaWH = true;
}double EKcTpIBfZslaXUFEjWikFsoBT = -7.56256E+31; 
double JMxbsdPlMYbMVUzOiNHOkkw = Math.Log(1000, 10);
EKcTpIBfZslaXUFEjWikFsoBT = JMxbsdPlMYbMVUzOiNHOkkw;
try{
int EDMwuljlThiFfwtGpAgEcAQ = 191430;
if(EDMwuljlThiFfwtGpAgEcAQ == 51565){
EDMwuljlThiFfwtGpAgEcAQ++;
}else{
EDMwuljlThiFfwtGpAgEcAQ--;
}
}catch(Exception ex){

}short KaDHALIF = 8604; 
double BsSFecT = -4.402282E+12; 
if(BsSFecT == 2.532273E+08){
double ODmheNkJGOSHTwUigaKIJ = -1.033516E-25;
BsSFecT = Math.Round(ODmheNkJGOSHTwUigaKIJ ,1,MidpointRounding.ToEven);
try{
int nJgYupzm = 1510974;
if(nJgYupzm == 19673){
nJgYupzm++;
}else{
nJgYupzm--;
}
}catch(Exception ex){

}
}double klunjEAmzFZ = 1.186684E-14; 
while(klunjEAmzFZ == -2.380601E+24){
double yutQBTPoWW = Math.IEEERemainder(3, 4);
klunjEAmzFZ = yutQBTPoWW;
try{
int HVUwXuVdZIcYLYRnKeAPTQSENqO = 6676589;
if(HVUwXuVdZIcYLYRnKeAPTQSENqO == 12184){
HVUwXuVdZIcYLYRnKeAPTQSENqO++;
}else{
HVUwXuVdZIcYLYRnKeAPTQSENqO--;
}
}catch(Exception ex){

}
}short uwMNDhQzJm = -5967; 
double cQcp = 2.772583E+09; 
while(cQcp == 1.701747E+19){
cQcp = Math.Ceiling(Math.Sinh(-5));
int? ikISGNqmyPEEwVAzyJbfiSaVXmKL = 9643702;
ikISGNqmyPEEwVAzyJbfiSaVXmKL += 60156;
}int sXpcmNwPDgg = 37372; 
while(sXpcmNwPDgg == 37372){
sXpcmNwPDgg = 795818;
}short MVsdHlZn = 8542; 
ulong XUVgpdQgFmBhLEEzxBwpIIuRxaUJ = 26837375486793189; 
int LzMJMXlOIBdODNlxopBeLpjgpqO = 7153903; 
while(LzMJMXlOIBdODNlxopBeLpjgpqO == 7153903){
LzMJMXlOIBdODNlxopBeLpjgpqO = 840338;
}uint XFZVxFIstGbLMEIoSMCjunFs = 1936; 
uint CfMWbXnSpudAcXQCoGbhx = 529992385; 
int qwJZHCmeYQm = 108526735; 
if(qwJZHCmeYQm == 282974){
qwJZHCmeYQm = qwJZHCmeYQm + 798080;
}double JcgyycdOisnMFgH = 409.4293; 
while(JcgyycdOisnMFgH == 9.895114E+13){
JcgyycdOisnMFgH = Math.Ceiling(Math.Cos(2));

}uint VhmdkgEgzJobiAfZIoZC = 8795; 
double XtdfMsWcuz = -2531784; 
while(XtdfMsWcuz == -3.366184E+24){
XtdfMsWcuz = Math.Ceiling(Math.Sinh(-5));
Console.ReadLine();
}string KydiUgQOHGEQRaopnjDm = "gihfADpBccNotuKbKJEEm"; 
uint SuhKFSeteNZEoVcwEBnFDQC = 619722; 
int FBYKwGwoMq = 714566078; 
if(FBYKwGwoMq == 683198){
FBYKwGwoMq = FBYKwGwoMq + 673717;
}uint QwfHmZdYWjbSK = 98; 

}public void ASfdEYTHqeLPHHThae(){
ulong jYnZHxJ = 3146097423693098; 
float zQRDzoionsopUSFmQu = 1014678F; 
sbyte gpIuzxVdCicECn =  120; 
long JeKsetVsacDVLWlybJCUeR = 59773139601446446; 
short iJSAxecWMb = -3483; 
ushort iRmQgAGyNGStq = 10481; 
byte HbKNnMbdN =  6; 
double GidusZbZUzjRLRxdage = 7.977744E+21; 
GidusZbZUzjRLRxdage = Math.Pow(double.NegativeInfinity, 2);
try{
int ypYdG = 6602817;
if(ypYdG == 11196){
ypYdG++;
}else{
ypYdG--;
}
}catch(Exception ex){

}float yhChW = 0.001870224F; 
float yCfIOMFpyAy = 0.09143757F; 
byte qGgVMflAs =  155; 
double NFdpbLPomuIhxpokHtFXXoQpwEfk = -1.299223E-35; 
double ixPLVYXXYAxQbS = Math.IEEERemainder(3, 4);
NFdpbLPomuIhxpokHtFXXoQpwEfk = ixPLVYXXYAxQbS;
int[] zPNUzGFckHGqHzEQyXPlbpStydKuk = { 1781795 , 10947 } ;
Random PbSQtXHnX= new Random();
Console.WriteLine(zPNUzGFckHGqHzEQyXPlbpStydKuk[PbSQtXHnX.Next(0,2)]);float pqHSXlkwakNXhWuyuWkSbRF = -1.487529E+23F; 
sbyte iiaHgiDCbQkMhaPPQeTk =  -124; 
byte DmVbMANhbRFblZFHOGusIjjcR =  62; 
string uXUGVsytlhQcoIiUDZDPkzeG = "VitcSjAyXNdXZfTDluHP"; 
short ZaoA = 23089; 
long EWTnZaiep = 28162588840517256; 
int JmZdzRpSjtOsqPjuQkJZeLyVaJ = 719294; 
if(JmZdzRpSjtOsqPjuQkJZeLyVaJ == 908098){
JmZdzRpSjtOsqPjuQkJZeLyVaJ += 345989;
}double aaFTuyKhZMDbHCxXeUnFPYQdcQsFg = -0.0002000529; 
while(aaFTuyKhZMDbHCxXeUnFPYQdcQsFg == -6.989071E-12){
double QkpXMLsZcWoOabdqjckCRFfeaUoh = 5.861254E-26;
aaFTuyKhZMDbHCxXeUnFPYQdcQsFg = QkpXMLsZcWoOabdqjckCRFfeaUoh / 3;
 bool? igdcnRmeMcN = new bool?();
igdcnRmeMcN = true;
}byte PjbGTXadtPgQOR =  174; 
ulong QSPzeaoVaSWcUxIBIYkm = 83637995698831321; 
uint uxwpaEwWKjNK = 109098087; 
string Atz = "gSzDTHuzxo"; 
string yJzBIFwdfLobBdq = "mbqDtilCYSxu"; 
byte AwfoljIEtVXujwExbQoblwYBeoxW =  139; 
ulong bUilTVl = 25227709318349958; 
double XCRHeiLeyGkXNLbENVBajLCQ = 1.490965E-27; 
if(XCRHeiLeyGkXNLbENVBajLCQ == 1.468115E+18){
XCRHeiLeyGkXNLbENVBajLCQ = Math.Pow(double.NegativeInfinity, 2);

}double nQCFWmmoIzzLIwEYNmFjEoJbz = -8.623724E-15; 
if(nQCFWmmoIzzLIwEYNmFjEoJbz == 1.23539E+36){
double omNHSRCalkwCZVxPnKoQmYhBfzVfq = 1.591089E-13;
omNHSRCalkwCZVxPnKoQmYhBfzVfq = Math.Sqrt(3);
nQCFWmmoIzzLIwEYNmFjEoJbz = omNHSRCalkwCZVxPnKoQmYhBfzVfq;
Console.Write(nQCFWmmoIzzLIwEYNmFjEoJbz.ToString());
}int pChKVKV = 97; 
while(pChKVKV == 97){
pChKVKV = 989630;
}ulong CYQkZ = 62712731819025633; 
sbyte GFGDJCbb =  -24; 
ushort swwZCDFkoqZ = 58502; 
int pJUPtHEaPTSCygDAW = 7972; 
if(pJUPtHEaPTSCygDAW == 592166){
pJUPtHEaPTSCygDAW += 430077;
}short ZyfTSHKbZtShl = -29931; 

}public void NsGCgZfzJPuFeWpLmHxtizhW(){
byte ByNKsLs =  0; 
float YUguUZydzbFfKJGBQJHojJoZAIL = 0.0001163849F; 
long ebwMsAbAi = 55860620591599327; 
string UKuGLpGhUlozHPPBenWnbOjyn = "kdweUG"; 
float QELwLxNqgmUCzKxdpAh = 4.660145E+22F; 
sbyte WqFeCMQsKEaZlPKE =  0; 
uint IWDNICIsPwyeQKdOdgqlF = 8940; 
byte yLknHVIoezEuFWEUtbXlzjoibGyio =  27; 
short sOjQ = -4391; 
string AQQMlAu = "jOqkoXYAnwcmazA"; 
string hCbRMNyoTzxJycJHgmwXUaetnWwuR = "bpPMeY"; 
short yaUWxADRQtuGUkdcegxI = 3116; 
ulong dyMyfTMYKnzP = 15506676418864204; 
string lmQ = "MIXDKfIJQ"; 
uint byXeuLqxwKYfCQReLpdlA = 86757249; 
sbyte agFsh =  56; 
byte UFuBaIpUQwDOCxNT =  3; 
double NlkwEQauENdm = 6.714468E+16; 
while(NlkwEQauENdm == 1.215863E-22){
NlkwEQauENdm = Math.Ceiling(Math.Tanh(0.1));
 bool? FcWOCxxn = new bool?();
FcWOCxxn = true;
}string qHQRoVWJCuhaDnCzCeaeZS = "QAPCEpWJGeBeSZtNQf"; 
double XQbeGiWCgVZNAbZsAttt = 8.938143E+14; 
while(XQbeGiWCgVZNAbZsAttt == 2.496955E-36){
XQbeGiWCgVZNAbZsAttt = Math.Ceiling(Math.Asin(0.2));
Console.ReadKey();
}double PyXZjhSQnFTAgdNlBftMqIBWWc = 1.372333E+36; 
if(PyXZjhSQnFTAgdNlBftMqIBWWc == -1.159827E-17){
PyXZjhSQnFTAgdNlBftMqIBWWc = Math.Pow(5, 2);
try{
Console.WriteLine(PyXZjhSQnFTAgdNlBftMqIBWWc.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}ulong qdPANLDEFAiymgjtkY = 16095451031823636; 
short ynmeMwnKHyADNzokhDEVQVKfGQ = -28931; 
string dyxtKfqmdLRSBMOAkjcYbEBNy = "DFgz"; 
double jSEacjctXXdR = 4.518079E-35; 
if(jSEacjctXXdR == -4.261604E+32){
jSEacjctXXdR = Math.Ceiling(Math.Atan(-5));
object fGPzJIiNxagKadAJbuNtJBd;
fGPzJIiNxagKadAJbuNtJBd = 2.265869E+11;
}ushort aijuzpeMZnIBVidFCzgF = 40774; 
sbyte bMMTPmbHOaetAqJRbZodQ =  62; 
ulong HhlWEgQbVBhJNxpA = 32994755612518293; 
float eGqbSoYkmHafcHkdzYia = -1.387884E+07F; 
long fjTmKRDYcXbkLUITKCjJATFm = 54136834488209480; 
double VJxMCwdccfGatFpp = -6183.101; 
while(VJxMCwdccfGatFpp == -5.556633E-14){
VJxMCwdccfGatFpp = Math.Ceiling(Math.Tan(1));
int? QOEWhj = 9127847;
QOEWhj += 63652;
}byte SymYOEFkhZXkyA =  124; 
short XRhZjCgkp = 9806; 
short pwlheNY = 8364; 
ushort wctFHfwXWUKc = 62643; 

}public void cyJWgNyVmaSEMdsMBJQGwfGG(){
float PtL = -0.0187174F; 
byte wfIecAKVAQsNcus =  152; 
byte amsuXRdbfXdkO =  134; 
int pYLIyj = 68; 
if(pYLIyj == 257135){
pYLIyj += 863048;
}float yOCezaeahQbatMRobRh = 1.051557E-32F; 
ushort DVRgJzIFkjABpJKcEboQUHyQtSH = 17752; 
ushort VCdUYiITxCBlRRTlCGqOEV = 51472; 
int WzbOQSthAoXxxKBwLFNpJQGWhqaWI = 2; 
if(WzbOQSthAoXxxKBwLFNpJQGWhqaWI == 889344){
WzbOQSthAoXxxKBwLFNpJQGWhqaWI = 56014;
}long fHKgdqDWPeVRtTpbnpf = 7995691513887243; 
byte WCIQPGYd =  42; 
int daslffonLMlEWSC = 159162; 
if(daslffonLMlEWSC == 574289){
daslffonLMlEWSC = daslffonLMlEWSC + 965082;
}byte SEnaEmsYeLIPenlMqkeEzK =  245; 
long ngQjzbBQoAJReLasOhSodxkWhstT = 19519808966866006; 
uint GKgZPOEUmAekLDfwURVTcgJ = 10093158; 
string dnbwUQaCiie = "xtmfTOACGAhizkJLAbAT"; 
ulong AHlekuLbWlQkAB = 36914105316659378; 
sbyte WJhpbhKkUZTGTPEeRFDWsnMhUz =  -69; 
int lMVCshCmwZ = 55876170; 
if(lMVCshCmwZ == 488761){
lMVCshCmwZ += 638068;
}byte aoCKgb =  46; 
uint XZwDVcWInYeCHcRxFOtMX = 5120; 
long lqNCGTsRAEm = 11160410867976582; 
sbyte tmKnCmRPLLALlKMjfJf =  61; 
long zFxhUTc = 95825640204798; 
byte HCcO =  218; 
ulong kjOADEDtWk = 26988524657755004; 
long gBQIfSFAlHuGQhWLeDNy = 47779677270835522; 
byte pshghKtguKYdhpRioL =  114; 
long jJmsVNhqhxVkIqbzJZmqXidzkd = 3025963474108155; 
sbyte UPlXkQlmcTAYPLGLkpVbG =  70; 
sbyte nqizMKFYksETNofhWQxsFAbyc =  126; 
string OeefJuJUHNC = "nBOygMXVkNeP"; 
short nPpMwMIojfnyktHlCiodedMF = -11920; 
short ffba = -13294; 
ulong gUFUsPLYqIjJCFnYlVAoKkcLwe = 55117402587530406; 
byte HHRAsfMewIPySgPeQzjWSTA =  61; 

}public void iPhCkY(){
byte CqFdaWFzXpOp =  57; 
long tail = 67973670745384359; 
short DjJGgpPkbPCFyUDLWU = -2018; 
long CcNbaHEyzHNnUwgfKfuniZtK = 65703758779270163; 
int ahVEfJkODiPqEAxGmPeyGDJc = 147436; 
if(ahVEfJkODiPqEAxGmPeyGDJc == 11827){
ahVEfJkODiPqEAxGmPeyGDJc = 822221;
}sbyte Jfu =  120; 
byte ZLcwxj =  230; 
byte COQwLHhizzS =  22; 
double TNMb = 1.849603E+13; 
while(TNMb == -0.001561327){
double ukfzhnSeFjwxuB = -1.851619E-30;
TNMb = ukfzhnSeFjwxuB / 3;
object bbIjILihCRxpu;
bbIjILihCRxpu = 7.011977E+31;
}ushort CjYeeOL = 13138; 
int paMaJnQfCfNYD = 626214; 
while(paMaJnQfCfNYD == 626214){
paMaJnQfCfNYD = paMaJnQfCfNYD + 921723;
}sbyte EOtpmsokhDDuX =  45; 
long fpZNk = 63821620234857751; 
float nza = 1.358934E-20F; 
short xWCUtEjEGRWgQuWsjABXN = 7323; 
float spCnWXWyMwUdlpKKkYbHCMbHs = -1.897049E+24F; 
byte wXQbKDdbWIIUzwuBTgTPQN =  198; 
int oGnUWRgJMq = 74218; 
while(oGnUWRgJMq == 74218){
oGnUWRgJMq = 620395;
}long qtMdqHgAYW = 36655496151908353; 
ulong ZRClNe = 33877429100484996; 
ushort DkoHNySj = 12925; 
ulong pSnfRQBpBSBqmLLItRoO = 42527054547124104; 
int qbXwHFEAosHhXscUcWcPAtUbyFRcV = 63; 
while(qbXwHFEAosHhXscUcWcPAtUbyFRcV == 63){
qbXwHFEAosHhXscUcWcPAtUbyFRcV = qbXwHFEAosHhXscUcWcPAtUbyFRcV + 524160;
}uint RegdTCXgdOJjp = 251423; 
short AXgVYbNCEeLBuwREBkRVLGR = 29927; 
double QiTGFuOZcEqzT = -8.398552E-10; 
while(QiTGFuOZcEqzT == -2.540216E-05){
QiTGFuOZcEqzT = Math.Ceiling(Math.Tanh(0.1));
Console.ReadLine();
}float UqXUTnaEuNbHUUCMyKUOmwgn = -6.814749E-08F; 
uint QdDeYNn = 446773391; 
float xxNTejsxcIMMPu = -1816.858F; 
uint JCyQSNpj = 660179335; 
sbyte DnFmzqADcdaYwBesNKV =  -77; 
ulong OpZHFZDUmKoyfygDWLcWhGCz = 37578773149188019; 
ulong BomRNAZhQpKYwIlkOjhTHiP = 3577084711189187; 
short tXSoeYDXZPkPxWbNlzEQ = 28211; 
sbyte AMGAiuLQttpb =  50; 

}
} public class VKkGWItEMPqoS{public void CtCLmFsEz(){
uint EoZSwfUqtpPsj = 47; 
double aRhTCAnaSFPfbskMkssuLpJxYAVN = -1.104212E-13; 
aRhTCAnaSFPfbskMkssuLpJxYAVN = Math.Ceiling(Math.Tan(1));
byte ScHfNLi =  4; 
byte pEVsWauhVH =  94; 
int RqxxVdQ = 37; 
while(RqxxVdQ == 37){
RqxxVdQ = 687292;
}uint ZyOhidNeg = 936381653; 
uint zdFQmUPO = 2690; 
byte wjXmwqmzhiyTlweeloFZEFGpLKNP =  186; 
uint iBjOUQSMBKadoGMFSQLC = 67; 
byte REICMEyNKBnsb =  165; 
byte uEdoDBqKZe =  233; 
double WHA = -4.907407E+24; 
WHA = Math.Sqrt(4);
int[] pmXDHIzMNTOekLVgdEgdohDtF = { 5526973 , 29438 } ;
Random MuGPiUnaHdkOtoODGV= new Random();
Console.WriteLine(pmXDHIzMNTOekLVgdEgdohDtF[MuGPiUnaHdkOtoODGV.Next(0,2)]);short KodVzVBhKFXfWdfY = 13648; 
float jdRVpjRpNCIQoilKqMnwtAztH = 2.707016E-19F; 
short xIjJzixStNLIFOHbOIXLt = 19084; 
byte zyLFzoAXAHqTyh =  211; 
double SXZEOOMSqShpIpHxNjazRuem = 3.391319E-27; 
double UCRmTLKZMgxsHSpKyLXQCYyTMQGP = 2.782272E+09;
SXZEOOMSqShpIpHxNjazRuem = UCRmTLKZMgxsHSpKyLXQCYyTMQGP * 2;
object qtNOPjTApz;
qtNOPjTApz = -2.232106E-32;
Console.WriteLine(qtNOPjTApz.ToString().ToLower());long wWe = 51546961897562187; 
byte meC =  225; 
byte yBcTjlQJpgBOTDpYRKRJQwwOy =  225; 
ushort UlgTdclBscUNLwPOkTAJLnnScEb = 3297; 
string xpPVOdaHLR = "PGOPeYuHIEE"; 
ushort iGXnRAaixdHFXPILOEiJsVI = 32945; 
string GHxxLSVbNhgMQkQhtDgkgjNWTawMx = "XylSUtNZsP"; 
long cAEcOXSjDftSRxUAELxjGzjJAXqx = 49601887604067014; 
short lzTlMSdtazAmxHszqEoVDQGwP = -22681; 
float qHo = 8.520563E-28F; 
ulong YqtNqFfWTXhYEYc = 72724551570122732; 
ushort YccuiSeKhuhXQXpxTN = 30283; 
long pcnOMJLy = 25762559643204848; 
int ecnxFUVNagSDeZPtIZOGy = 39; 
while(ecnxFUVNagSDeZPtIZOGy == 39){
ecnxFUVNagSDeZPtIZOGy += 286037;
}string FOtqctFWUzuSYJEGAbT = "QLEoWm"; 
float aqqGUgwtzlcHkyOBuOAC = 3.496987E+11F; 
int wTQTjeuQz = 919358514; 
if(wTQTjeuQz == 782002){
wTQTjeuQz += 946701;
}float kgmZmWtXc = 2.062875E-26F; 

}public void uSsLfjbpDiuSmioCkuGRHnG(){
uint liXyVhjlcedE = 26035740; 
int DLbNwNdHRNJoqgflRsTadSHbjHj = 22; 
if(DLbNwNdHRNJoqgflRsTadSHbjHj == 356707){
DLbNwNdHRNJoqgflRsTadSHbjHj = 13605;
}float EFOXA = -4.603031E-14F; 
int AUUVYSM = 154507; 
if(AUUVYSM == 701675){
AUUVYSM = AUUVYSM + 74781;
}string ODZwTupoxcCIdwuWNl = "CoNoftmTkyctAFk"; 
sbyte fVNPiGySykJeOIfzHznbnUbHCfSu =  43; 
string SlyR = "AJsJoxJiEOxPWVTfXgJlUfndO"; 
string whQTqRnZ = "fFNRkTcNJcyOhXUeOFXtuoVllnbO"; 
double XIWVa = -4.03705E-09; 
XIWVa = Math.Ceiling(Math.Cos(2));
object WFWpUUiHQaRShCMqpsjKUm;
WFWpUUiHQaRShCMqpsjKUm = -6.75762E+32;byte dIIuoJWBTPWax =  176; 
string zCUNSQJCNDkd = "RlaDzdTeWbKmiazgj"; 
short NnqdgCCnYgINzSYbDWyYHjoDAjAJP = -18774; 
ushort klpnBhTbXQUXnokwkZ = 59252; 
byte ttEZOEWcSzOMeCmDKBwcxa =  207; 
short nkZezQpjRRT = 15523; 
short FXYfsgkiNYSBzBhptDoJBtX = 30694; 
ushort yDnKtQxOQfyVzhfeyalX = 23060; 
ulong Czpy = 62358477800978761; 
long cFceKKbSydhDwVWNxpIBNldtQCF = 4857302047844975; 
ushort TJfaJal = 19580; 
string pcnoubiiRCKnheVwnoKfkBCEE = "CBdkRdOKocjVuxnbEUAF"; 
uint enN = 615741008; 
double QCKMNBJeHLGByPTlsUm = 9.076316E-24; 
QCKMNBJeHLGByPTlsUm = Math.Ceiling(Math.Asin(0.2));
byte iKtpqMimslnayjVdnjOcSeNI =  160; 
long yaUtINtLtKWUplPaThRxxQENdB = 9879162105378421; 
ulong yTGneADuq = 16858638800028525; 
byte HyhpgGuHxYmkxA =  140; 
int CEsXnjwOdpVZdkUAzePpEAJKSfwee = 34; 
if(CEsXnjwOdpVZdkUAzePpEAJKSfwee == 881899){
CEsXnjwOdpVZdkUAzePpEAJKSfwee = 970425;
}byte zsnMCdoAHtNsLYuOnxcUIeiOqK =  8; 
double AjmIHaMBEbxtkl = 2.755323E+11; 
double WsktjAwSbqdhiBKe = 1.021775E+18;
AjmIHaMBEbxtkl = Math.Round(WsktjAwSbqdhiBKe ,1,MidpointRounding.ToEven);
try{
Console.WriteLine(AjmIHaMBEbxtkl.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}ulong RCdouVxkuJqfm = 36505485366714645; 
uint SNk = 69964359; 
string KhNHsIpNNhhWIHPi = "cQdtxKXuo"; 
uint DjmMjVZis = 15724484; 
int LFIKcUwdEYGUWfmyQldgejDVwMN = 60868574; 
if(LFIKcUwdEYGUWfmyQldgejDVwMN == 899003){
LFIKcUwdEYGUWfmyQldgejDVwMN = 241468;
}
}public void IScmswf(){
sbyte kHSnPMMDueyRBbMopgNyNtLHOywwD =  98; 
long Wkq = 88654454583078372; 
ulong qgQpYbnsLlSWkAqppaAxyUpK = 27015979123875019; 
float yqGeESuIlYAsqZHGEmMusyQfS = -4.260933E-25F; 
sbyte gNLbtcaLlxHbPJsk =  -97; 
ushort ClalwBAaBAmqJngelOpYWHUqRe = 23536; 
int zVMSqFngBoBTcBlYeYNIEgx = 420196628; 
if(zVMSqFngBoBTcBlYeYNIEgx == 339588){
zVMSqFngBoBTcBlYeYNIEgx = zVMSqFngBoBTcBlYeYNIEgx + 668941;
}ushort hWlbECAsJTY = 47017; 
uint fUgJtMgGVjqAVxuLWyeYtWQz = 45122875; 
int ePGwXapbOJpKjDbbLeG = 94; 
if(ePGwXapbOJpKjDbbLeG == 500571){
ePGwXapbOJpKjDbbLeG = ePGwXapbOJpKjDbbLeG + 626563;
}uint idIaVfBBgEVCUf = 9393; 
byte wPQVONE =  212; 
int fTfPyFhXfIuEJbUzPDMW = 30365; 
if(fTfPyFhXfIuEJbUzPDMW == 222057){
fTfPyFhXfIuEJbUzPDMW = 717280;
}int pUydDTwnyg = 30; 
if(pUydDTwnyg == 832802){
pUydDTwnyg = 617827;
}ulong fPapYafHdekOEMYTIkkqbQHHa = 14801739100216640; 
ushort RnZWxYVQpdKEsqeOHwx = 52462; 
short tppCKJU = 25293; 
sbyte ighGJnO =  -37; 
sbyte pEp =  -105; 
sbyte TwEHi =  23; 
short UJGVnmWxUBukiAfsIdKoR = 20075; 
double uGPzpDaF = 0.004812966; 
uGPzpDaF = Math.Exp(2);
Console.WriteLine(uGPzpDaF.ToString());byte dQTXAT =  13; 
double nbPF = -3.170748E+09; 
if(nbPF == -3.032767E+22){
nbPF = Math.Ceiling(Math.Sinh(-5));
try{
Console.WriteLine(nbPF.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}float qmDu = 6.583684E-21F; 
string dDlijWcblgVmmRgyhGxAZkU = "tcibOePDBUlRMR"; 
double djlfmHQB = -1.244173E-30; 
if(djlfmHQB == -2.903274E+23){
djlfmHQB = Math.Exp(2);
Console.WriteLine(djlfmHQB.ToString());
}float nTP = 1.121587E-29F; 
byte uiTjypnZWqXoPinflLKGHENMcaSu =  43; 
sbyte YttjyVbOQCoTA =  -19; 
int SaOEa = 654702; 
while(SaOEa == 654702){
SaOEa = SaOEa + 141186;
}sbyte zUQSXYWxZQkkWT =  -107; 
short noyWZtuCtCLdubOTqqHzb = 2755; 
uint JTjEsdDEXghSKBABxpExk = 6002; 
ulong qGDySJMROngiRi = 10875213742633589; 

}public void SgcLYbypnwgbzageywaVmtqFh(){
short elsVChPKqPXOQqX = 23115; 
ulong hRYabQzVnEAPyDKEteWBG = 65405373803848856; 
int BEVayAdkCyG = 232084240; 
while(BEVayAdkCyG == 232084240){
BEVayAdkCyG = 395595;
}string XSWmeniIk = "pQLJnYlDDlepUudZZxfnXf"; 
int IyFqQMoSTAlHUkYlpPSRhIYCienM = 931347; 
if(IyFqQMoSTAlHUkYlpPSRhIYCienM == 218603){
IyFqQMoSTAlHUkYlpPSRhIYCienM += 755923;
}string uUNQXmxwhxePzTYinNQZAchL = "EoWLbwhiiRwcbUV"; 
byte tMaAnJYFWzXbqsMbsu =  240; 
uint gbXGpbDglSUPWJxLZoHYg = 93; 
byte SAJbJB =  247; 
long mLcTYHwzKGdmuJppteqCEbdOzy = 17658083001590208; 
ushort JfqZEIuqgUyQsMpxIiplHo = 26421; 
uint Ctgnh = 33; 
byte HmNqYVHMHFaMCumWnVN =  47; 
string LRUcondZLfENe = "Jpjw"; 
int IRAtZZUqTnEdCmDWyWGtJITUiOo = 2660; 
if(IRAtZZUqTnEdCmDWyWGtJITUiOo == 354779){
IRAtZZUqTnEdCmDWyWGtJITUiOo = IRAtZZUqTnEdCmDWyWGtJITUiOo + 579806;
}sbyte UqCcpuNfyDAkSAMTnPmmZXG =  -77; 
byte JdVDlAtAJGWUoZWCxsJDy =  183; 
uint yEqWN = 720515679; 
short HLJkUqZSsRgWuAmqyCnCJmXCQ = -25986; 
long yNNUGcJ = 11298039811067189; 
float idddGouyPiuOhu = 5.143082E+12F; 
float FdmqqRZdgalSX = -6.883116E-35F; 
sbyte cFdRysxRUoBfXMsRpAQi =  87; 
ulong eCiukXkmL = 88394666727223036; 
ulong yDzYinUaIIlXQpsZ = 42636622583789653; 
string ZndMsess = "LGEafqWDFz"; 
string xMPnSOGHOBJUXoGXVohGybItNoST = "gyqbnDDozBpMMBkWJcQMZHpqyAwe"; 
ushort EVNKHcsTKNdIyPRT = 48040; 
uint xdmchXAgaEPhyT = 940520325; 
ushort bUhFQtdJsjCAFNUIYyqSGNhWsoUGG = 51299; 
uint xTKHoSmUZzFpFBZcmPUBGDuoM = 2455; 
uint ycDKKdLcnsgjxTg = 83; 
short pSsRUZlTzwUdGNwCxcGVynZZx = 17354; 
short JnhUUwk = 7890; 
sbyte JLzjlHtn =  6; 

}public void bqKfmxucStugJcj(){
byte ZjjCPxktUx =  61; 
string GhlThsEbIzbh = "MRDcxaattesVlVAMsZhaBNl"; 
float USHlqMhubKtRRalaZldUXVxe = -3.568641E-16F; 
uint hmZFJmDhugbsjCkkfVBZVZCBiZ = 966612391; 
ushort WbmnlOLPiFzdHSjLL = 27993; 
long YuPAYiMz = 33213938255044842; 
sbyte oxBqNVBRsJpyowmkEDmOodclsAj =  97; 
short JjGPgoLIHTYhQdhYtZqCmHNq = 7469; 
byte oYmMHACagRAoY =  128; 
double tPhmZdufkDQDbxoiUVxKthsIocb = -5.688864E+28; 
tPhmZdufkDQDbxoiUVxKthsIocb = Math.Ceiling(Math.Acos(0));
Console.ReadLine();sbyte fXbzisU =  -128; 
float OJHQFACTgpGiDJSKIKkURfgVHOU = -2.156753E-35F; 
ulong HudCwMPXROe = 30237602934434373; 
long PZdJCLiR = 3827209426507370; 
sbyte YTMufNfotLtCxE =  -110; 
sbyte uaSTaGglomHTDhWkiBRAuUpDm =  -38; 
uint dcmoThuzoEXnFiQ = 479588; 
ushort DxpncmHxAKoHIgJhmasauOzW = 39321; 
int UleQdRDWbOPkblMUOa = 306329330; 
while(UleQdRDWbOPkblMUOa == 306329330){
UleQdRDWbOPkblMUOa = UleQdRDWbOPkblMUOa + 549492;
}uint wwPYuJ = 690154; 
ulong wTkPfjIu = 7538162936690865; 
long xYkDqXxgh = 20418330458062615; 
float ijXnHdLQINVnzalnOZSDnsZc = 3.493569E-38F; 
short Ynht = 19112; 
sbyte czLEzHgPulPUfAd =  59; 
uint sCQkUJGMHStFCAsCiPcw = 9353; 
sbyte FRQgmFUCHNyGJRFbYN =  124; 
short PKoDEpoKfmwulG = -28950; 
float zcXSYPbNTqjbyIXR = -8.142224E+23F; 
sbyte EKnGSBAckxkCiznX =  -39; 
int UnQLYeIIFzEdQVsWPOqZzoZD = 119062; 
if(UnQLYeIIFzEdQVsWPOqZzoZD == 987541){
UnQLYeIIFzEdQVsWPOqZzoZD = UnQLYeIIFzEdQVsWPOqZzoZD + 376561;
}string FammOZoCjTVFHtwG = "DAiaEsGnnnSxQJlzdzQJfiQAQ"; 
ulong uwVdLcbyURzaIIYALxf = 74621568967945534; 
long JJkPNkTtjhtOl = 8418424230495805; 
sbyte KwuL =  -54; 

}
} public class jffpckhmlUxf{public void BoMGZs(){
uint wyVFLFHCttAEbacnyifztlmGaDFN = 800726386; 
ushort PuOIjMsHCMTC = 61161; 
uint BmziQ = 262; 
int jpFiaTEE = 136510704; 
if(jpFiaTEE == 236559){
jpFiaTEE = 394218;
}string RzXItMJzxdBAIFfOQFyUhi = "IhpEPOAnSwxZOeWRzHfM"; 
double eWtaaQIXiEISo = -6.318617E-36; 
if(eWtaaQIXiEISo == 8.376326E+14){
eWtaaQIXiEISo = Math.Ceiling(Math.Tanh(0.1));
Console.ReadKey();
}int dIxfyOMXppgOObduJA = 922961; 
if(dIxfyOMXppgOObduJA == 372993){
dIxfyOMXppgOObduJA += 536379;
}byte zUpxXEDzsbCYzA =  117; 
ulong bfIgxWnQfgJFWoUUb = 87996913506431719; 
long TKbb = 31425750975405108; 
double GRxU = 1.791038E-07; 
GRxU = Math.Ceiling(Math.Atan(-5));
ulong KzePiPJXMEu = 52325359393992862; 
uint stuoGipWPW = 79307123; 
ulong XjBcQWTfzDCxbcSCzYmc = 59943981844466278; 
double HsHzyocfMEwmVROigne = -0.0004673323; 
HsHzyocfMEwmVROigne = Math.Ceiling(Math.Asin(0.2));
Console.Write(HsHzyocfMEwmVROigne.ToString());uint AJetWubNxMKJdaeXhbXxuUdnWOXT = 77; 
int DHFxQkyiHPstTjcjGKfcMbDFFR = 157286; 
while(DHFxQkyiHPstTjcjGKfcMbDFFR == 157286){
DHFxQkyiHPstTjcjGKfcMbDFFR += 645379;
}sbyte gyxJgXFOQGCKuNMNnAJAiBeZ =  83; 
double oMAkLjddFOXDizXkXcVxKxK = 7.549015E-32; 
oMAkLjddFOXDizXkXcVxKxK = Math.Ceiling(Math.Cosh(5));
 bool? wylpfwZjUUClueapK = new bool?();
wylpfwZjUUClueapK = true;uint RqwGLCydDsbXmRZh = 80; 
byte UNThmgujOxatIYSfUGPthbUJL =  89; 
float WeSTZKG = -5.021276E-05F; 
byte EgnogqIooZUm =  109; 
string JTSkyUplmKEhuXsNJIllmUMOq = "cDjb"; 
float ShbNllPznVaGEj = -4.259768E-13F; 
double lGjPFOgeT = -2.73119E+25; 
double WFBxSOXsTgj = 7.385656E-29;
lGjPFOgeT = Math.Floor(WFBxSOXsTgj);
 bool? XQkldDmHgtuHBTPMxVaw = new bool?();
XQkldDmHgtuHBTPMxVaw = true;long kKJYudyhyzeuOlOomlayQlpsHmH = 38764828105387003; 
short tqMpnuOQGgBTRVSwkeYupsg = -4461; 
long eKLplCxDUVngDSCDaggU = 82423206295744690; 
ushort jpG = 41588; 
short epUYx = -14520; 
string gdDgSNsapRYHNMxPQBeTLYBVCyyL = "hywxZSQVF"; 
string JjZPJJqtTfiWjhmhDHGwp = "PhtDkOTGTkxwjBYMlTf"; 
string gZBloNLmnZJucwSwfzDAnlYBO = "MiRwMolPIiTqVbitGlNbhfgpfuDF"; 
int aaysaCxTmtimNKDOECpVHMNNPGc = 289920; 
while(aaysaCxTmtimNKDOECpVHMNNPGc == 289920){
aaysaCxTmtimNKDOECpVHMNNPGc = aaysaCxTmtimNKDOECpVHMNNPGc + 217745;
}
}public void hSUDaHWMYtSUtxNqRboJonm(){
long wAVEVIDbaCKUaBJYU = 70545796303957931; 
uint qDyTGkaGhbhcMpSsVcyRB = 70690703; 
ushort TPIhThHwXkADJeKa = 59183; 
uint NLIFcULLBJjMRdsMwsyoTCIOftQl = 768973311; 
float eOIQlkGniygMOzIAptn = 7.189972E-05F; 
byte kdTz =  173; 
string MMSEHdxDAkNjgxw = "aHLqWFBiiSKxMsMWjKLuUjRFcNT"; 
string nfIQiTSYOChlJwPuZRhzRZxLaaR = "gnjLyoUNbHEAUXxhEhkOcjdW"; 
ulong dnJXPhbcSAsxPuBbst = 53784313578638135; 
ulong NWjm = 83899632216685892; 
uint fWQgtRmaSLJdZjKwPWsZpVpey = 176648207; 
float mCRjNOIzbAYPMeAFVTbXJpVdzCjGL = 4.413794E-05F; 
long cFiRUmxbWOxt = 5386901138181145; 
double XHOMkEbOsc = -1.285555E-06; 
while(XHOMkEbOsc == -1.790818E-16){
XHOMkEbOsc = Math.Ceiling(Math.Tan(1));

}float MVFffJBbcqxGZWuklta = 4.947588E+20F; 
long ElMjOazKeJQRgXpcuWlhViF = 71363061431082545; 
float gbUu = -4.466925E+17F; 
string HqXjemnVpHODBTuHdo = "qRmxBZ"; 
ulong WxJVjlEnRHzQtxLfbuoEnHmpjGJNu = 30258047705435513; 
ulong qWFEaFzPVfgwgKjLoj = 56878140019756647; 
byte felzoQYHKdmLMQO =  161; 
byte pBgDcpoaZOfLIQqZ =  173; 
ulong JfqMEblXIZOOjHxTOsEcVjnQjCZ = 26810806139632688; 
ushort OEBQtHbkaVdlWDIfOZzRBDccQBnmH = 53185; 
ushort bUZO = 3596; 
byte DmFiGVjARebjNfDzl =  131; 
ushort StOFkzbjcETdCyWEVtyhGbxiGHKWz = 26278; 
ulong GlKjAaYhT = 80955189702299772; 
double TdpuRWGYEOfLIPGzoN = 9.390317E-32; 
TdpuRWGYEOfLIPGzoN = Math.Ceiling(Math.Atan(-5));
try{
int TcutOoOzFAt = 1668177;
if(TcutOoOzFAt == 44522){
TcutOoOzFAt++;
}else{
TcutOoOzFAt--;
}
}catch(Exception ex){

}double lTZWBoRLshGQnGix = -7.158681E-20; 
if(lTZWBoRLshGQnGix == 0.02811782){
double ueXEiqVIYIPzRCAwdpiZFfFIzaVuo = -8.077457E+16;
lTZWBoRLshGQnGix = ueXEiqVIYIPzRCAwdpiZFfFIzaVuo / 3;
Console.Write(lTZWBoRLshGQnGix.ToString());
}ushort HmAxiQeEeFwFSLEabuiQqpMK = 47979; 
short IAPkAYyTcG = 14800; 
byte muEdSnDEYVeXgFAqCPKSCx =  19; 
short VGuSBwphyEWMI = 22655; 
uint MKELnstYDBQbujQqoKWIhLz = 776096; 

}public void KbDFUXldPNAEfEmxpZFnOmNG(){
ushort VQHaldWzJsxXHTQpucRLFtwMwzWB = 1822; 
ushort WgWcwyOCxH = 41946; 
float astMzyTKPNtDwVGNgxyldAQLQuS = 30.60131F; 
ushort XzBM = 2736; 
sbyte bEEEhuEB =  -124; 
int OpkjJQB = 664795; 
if(OpkjJQB == 593689){
OpkjJQB += 244441;
}sbyte KwdUiEIWwTJAq =  -20; 
sbyte tpyNeTNQLkA =  -18; 
short ZqwNLhxgtFftPqZpjgCFMoPwpRJn = -21189; 
double lHzUs = -1742164; 
lHzUs = Math.Exp(2);
try{

}catch(Exception ex){

}short kSglMizQERcmRhzEbZl = 21383; 
uint Gann = 51643115; 
ulong UDSjUoLnZfcEhhKMsaYwqbABehlg = 53833894114119648; 
ushort niSNafOYUzpCVHDpGTcq = 7247; 
long FpBE = 71679522717350441; 
ulong FbtNfnNRxEVuNadydC = 10802938754728644; 
short fkiTONbSJF = -32386; 
uint NTXILNBgMnk = 6688; 
short DReHVzL = -17451; 
ushort HZZ = 14597; 
ulong BeQBECUXMSguWYJe = 30547820980716829; 
string EHeNCVXOKgSOFaWhUdqq = "wtCCfWXagC"; 
double WXnSYpkyioDmKoo = 3.62763E-13; 
while(WXnSYpkyioDmKoo == -0.007159918){
WXnSYpkyioDmKoo = Math.Floor(1.638051E-12);
try{
Console.WriteLine(WXnSYpkyioDmKoo.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}double VCuFem = 3.92354E-39; 
if(VCuFem == 1.782769E+30){
VCuFem = Math.Ceiling(Math.Cos(2));
try{
int wzHEsXtYfARlnKMA = 4456055;
if(wzHEsXtYfARlnKMA == 95761){
wzHEsXtYfARlnKMA++;
}else{
wzHEsXtYfARlnKMA--;
Console.Write(wzHEsXtYfARlnKMA.ToString());
}
}catch(Exception ex){

}
}long wnFKxIAUhZJuXUwPYjKeP = 70923577255174391; 
string unJDSV = "PHkSsfq"; 
ushort hBKGmnFVWQCgekZUfS = 17883; 
sbyte qEURucUXgs =  57; 
sbyte oYpwTgKOHbtaLxkXlmcqpQX =  76; 
ulong kKYPyQbpVbELZhxBLCBQKGJZVf = 13398198148363782; 
float QREEYuBjuCnVUPfxoYiFbYUsgnSya = -8.764106E-32F; 
double yKTXXRoeA = 1.968023E-10; 
yKTXXRoeA = Math.Ceiling(Math.Atan(-5));
try{

}catch(Exception ex){

}short CgxEffUBZQxzxKeptZNAZkMYmpMR = -32523; 
sbyte keEZlXGYpXWffbt =  -2; 
string QtcDOYpuzpQucB = "qluTKTtNayDcfGWUQJxjOxIpGJ"; 

}public void ApiVUxmPQZHsCyGbflVkOCxZL(){
int eVEdnacTySC = 60955473; 
if(eVEdnacTySC == 112516){
eVEdnacTySC = eVEdnacTySC + 891251;
}sbyte UNhwDeKzdYU =  -85; 
byte ghTmVtsAF =  42; 
sbyte DptWxCQwNEXBaZFDYU =  12; 
long wALXTZQYuhZl = 69419875838458796; 
ulong DktlVPPBKGAlgRJZ = 77158244731138791; 
string kySsjqEVWAFHdbi = "ztMNuDxhZQsshhmejyEsJqxiJl"; 
float uWAYnAW = 1591.441F; 
short IaLt = -7986; 
double OPmAxGyR = -2.364377E-28; 
double cqteIkIDVuWqXYckyyeVVAT = -2.883166E+12;
OPmAxGyR = Math.Floor(cqteIkIDVuWqXYckyyeVVAT);
try{
Console.WriteLine(OPmAxGyR.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}double aVbHOJCixugNGNzNjGuPL = 6.383418E+31; 
if(aVbHOJCixugNGNzNjGuPL == 1.90182E+28){
aVbHOJCixugNGNzNjGuPL = Math.Ceiling(Math.Acos(0));
Console.WriteLine(aVbHOJCixugNGNzNjGuPL.ToString());
}double fSDLEXhjWodGFpnoy = -6.125756E-34; 
double AYQLf = 4.665404E-38;
fSDLEXhjWodGFpnoy = AYQLf / 3;
try{
int TUWWVpEPdjOlfzplVTlzzSDWDzjul = 4478598;
if(TUWWVpEPdjOlfzplVTlzzSDWDzjul == 47288){
TUWWVpEPdjOlfzplVTlzzSDWDzjul++;
}else{
TUWWVpEPdjOlfzplVTlzzSDWDzjul--;
}
}catch(Exception ex){

}float QaPtABzjKtuCasZuygiEUJNBPdUL = -2.048039E+15F; 
ulong GWM = 76106139674686936; 
long PudCdMqoncgm = 63795475680910139; 
ushort eGeHsxoRVSfiaKRitZyLqUFIACo = 18614; 
double TdYUpzxyt = 7.563825E-38; 
while(TdYUpzxyt == -1.606595E-16){
TdYUpzxyt = Math.Ceiling(Math.Sinh(-5));
object TNpAtXDAuSTXopgyZqDOaH;
TNpAtXDAuSTXopgyZqDOaH = 1.092792E+08;
}uint QfeVQmVNFcPJmn = 381715339; 
uint kdDtAupGJMbEKjBj = 20908186; 
string IfB = "UqOpyi"; 
long pstxUyGKGkEiLbsGid = 44321282485018457; 
uint HZhBazm = 8128; 
string kFswyCqwTZ = "huCXlSA"; 
short zYzcGccilOaPeOxjZbwDlaGizS = 17165; 
double mwSC = -1.145262E+25; 
if(mwSC == -8.183274E-17){
mwSC = Math.Ceiling(Math.Asin(0.2));
 bool? ytoFNodeZthnTXjcLlbdc = new bool?();
ytoFNodeZthnTXjcLlbdc = true;
}long FcIGGTfBYhkOYPtz = 28728237654974784; 
double NsYFzWGBZMhXOZLieXjp = -1.771257E+35; 
while(NsYFzWGBZMhXOZLieXjp == 3.074969E+27){
NsYFzWGBZMhXOZLieXjp = Math.Ceiling(Math.Asin(0.2));
object eYzIwPTpMsqgaFGhoPUXHMnVhP;
eYzIwPTpMsqgaFGhoPUXHMnVhP = 1.69555E-14;
}int VlHsXbxjaie = 43; 
if(VlHsXbxjaie == 359206){
VlHsXbxjaie += 694137;
}int HGbYmbbhtnyHplEc = 866; 
if(HGbYmbbhtnyHplEc == 117858){
HGbYmbbhtnyHplEc = HGbYmbbhtnyHplEc + 346979;
}double iupSwyYzXAWLTmtqVTdWECPPT = 5.744552E+22; 
if(iupSwyYzXAWLTmtqVTdWECPPT == 1.804989E+07){
iupSwyYzXAWLTmtqVTdWECPPT = Math.Ceiling(Math.Tan(1));
Console.Write(iupSwyYzXAWLTmtqVTdWECPPT.ToString());
}uint sJE = 54; 
sbyte EXg =  -56; 
sbyte SoGoqLHP =  5; 
long ZqefxKRG = 58286821350629230; 
long KqlxBoTboBnTlRZt = 12387610097736474; 

}public void mpqzOB(){
ulong nRsZDRQmwupsiMLYOsGVOtxaDpmZD = 38858049251946761; 
ushort umIMTapYCFsLwUSkVusx = 16149; 
int LSToSnOLquLlJgHDZmqMujRff = 69046928; 
while(LSToSnOLquLlJgHDZmqMujRff == 69046928){
LSToSnOLquLlJgHDZmqMujRff = 955169;
}ushort wWTaLyKNTbqKJjenSsiVSxCa = 49996; 
byte FMfcxqwm =  48; 
ulong nPIYBiBwxzGPNWOzX = 70539424164582114; 
ulong UtnWUBodunTVtBsNMYXtfISznPVpN = 23346662930084301; 
uint nySukddcsUOCgeuSmBQJfG = 328875510; 
float WhiUpFjmJVmnIukMDxYZwT = -6.237877E-14F; 
ushort sTGJzCCAhVTzFjsTx = 32304; 
int TwytcDsTpcPRCOlpIZxMNimpSeRPI = 448146; 
if(TwytcDsTpcPRCOlpIZxMNimpSeRPI == 392905){
TwytcDsTpcPRCOlpIZxMNimpSeRPI = TwytcDsTpcPRCOlpIZxMNimpSeRPI + 662434;
}double mGViIiefbWQXbl = -1.053274E-34; 
mGViIiefbWQXbl = Math.Ceiling(Math.Sin(2));
try{
Console.WriteLine(mGViIiefbWQXbl.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}int mNq = 82; 
if(mNq == 862793){
mNq = mNq + 893907;
}float KSDg = 2.261278E-28F; 
short sBeqWQAtE = -26739; 
string TJXVOoIUQfOMQXjAObYp = "akBhwukshnOTljaUIsl"; 
uint ylMtCZGZFDfLbTz = 749917; 
ushort xDJSaSEOiGgJNQiEZPPTwGsDMIi = 13820; 
short yUZpdVOuMBYkyuyTtU = 24135; 
short JXBBRjffsKsyHhm = 12327; 
ushort mPGcnUjiYfSonsaePWAQmL = 43793; 
byte RHNQlkjWiGCS =  153; 
long pJlamdwayCtpkHADfqK = 7245745411838439; 
sbyte zljmTBEAJapxGooSUYpKdw =  5; 
double qSqzfFLpn = 1.700321E-15; 
if(qSqzfFLpn == -9.991892E-14){
qSqzfFLpn = Math.Pow(2, 2.1);
try{
Console.WriteLine(qSqzfFLpn.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}int VJUGV = 913719; 
while(VJUGV == 913719){
VJUGV = VJUGV + 653244;
}string UPVpRdlibSIIfBNPTXZxyCWlMwu = "PTNfYcpMptTjsWTQlYQeimW"; 
uint GytSJFxKAuieCJckbzAdP = 52; 
long ZNFoFC = 64647621569311266; 
ulong BSCKMLoNUgOxxTB = 68764945420842039; 
float SepynhRMH = -2.364349E-20F; 
string uozSuyOARETKkgZWHfXJxUHYpHZXK = "FnVMsttZayeNaByOdbSQaioq"; 
int JACOzQQaYFOAOGDH = 49; 
if(JACOzQQaYFOAOGDH == 865074){
JACOzQQaYFOAOGDH = JACOzQQaYFOAOGDH + 114971;
}uint EYfUQwYFjRKTymZoX = 52; 
float Fml = -5.786579E-11F; 

}
} public class QOTIE{public void ZkALVkDHjoWVKiSAoMTFJCZLTHxqi(){
ulong jgUzbAFbPYk = 45230326938023495; 
uint PYpylO = 273280767; 
ulong ztYNOPLQZ = 29491693875459676; 
int meSq = 41; 
while(meSq == 41){
meSq = 810604;
}long OaNNypDCOAxpPxLsSNCtnkC = 17127825620387135; 
double XWcTLmuSao = 9.030031E+12; 
while(XWcTLmuSao == 6.937764E+18){
double osAEAGgwBlW = Math.Log(1000, 10);
XWcTLmuSao = osAEAGgwBlW;
Console.ReadKey();
}int THeLZmBLncpFOhSlILicWAFff = 7086; 
while(THeLZmBLncpFOhSlILicWAFff == 7086){
THeLZmBLncpFOhSlILicWAFff = 79433;
}byte InThjPkDOOEATKEdlL =  221; 
string WubZFICwshYdafUYe = "VXAhMbGmHgbgwkh"; 
string TSiFgKhxGGxSEKehmYhbOJ = "tsb"; 
ushort kNOWYuagVTOns = 47242; 
sbyte judyXZhmaaXkRQ =  -65; 
sbyte OkTSAUiKYm =  -28; 
byte KcEFiXXXxlFBVYyQoeGIZEBlEK =  129; 
float ZNNdgpUIlHohqsYnqEVMTJh = 142.6456F; 
float Cqbzyc = -7.500776E-05F; 
uint yPIDidzqBJlDXlfFiUqFYClks = 2121; 
sbyte fgUzxUPniLFJyblUZuPtSUpgU =  94; 
double yjSSpzuCqdBsqqZiM = 26.94324; 
while(yjSSpzuCqdBsqqZiM == -6.967631E-15){
yjSSpzuCqdBsqqZiM = Math.Ceiling(Math.Cosh(5));
try{

}catch(Exception ex){

}
}ushort ZOAq = 45598; 
ushort kDKJhWqTAyZawVbXZUQ = 27822; 
ushort DlutTxNaSaiVNSuSSuIJcxpMb = 9340; 
double AxTXiuZzoPcUgAFZRBhiEgcx = 9.969265E+18; 
double WBxglMtSVEdqNDDR = Math.Log(1000, 10);
AxTXiuZzoPcUgAFZRBhiEgcx = WBxglMtSVEdqNDDR;
object PmtuxixGTDHxpxjZiSGPPpIEZfajl;
PmtuxixGTDHxpxjZiSGPPpIEZfajl = -3.534578E+30;double aqtDJzoMkzGPwqiltEY = 4.113036E+08; 
while(aqtDJzoMkzGPwqiltEY == 33.75615){
aqtDJzoMkzGPwqiltEY = Math.Floor(3.513668E-29);
try{
int uwwcayE = 6217402;
if(uwwcayE == 91577){
uwwcayE++;
}else{
uwwcayE--;
Console.Write(uwwcayE.ToString());
}
}catch(Exception ex){

}
}float sIgZYeRLRscMjFgwKK = -6984.842F; 
int wZukcAdTElzELFsw = 84703388; 
while(wZukcAdTElzELFsw == 84703388){
wZukcAdTElzELFsw = wZukcAdTElzELFsw + 684065;
}uint pVKnLWypYTlgGOcqHXORmG = 525057656; 
string YHDUPMAIUg = "QiIPiGRtlPYPKeNVJALbilwY"; 
ulong ddAHYTVBROgceZchQAZRSPb = 25328865662802910; 
long wCNTFRfUinYPGwLZawVYZVocJQQJQ = 9508843500052818; 
short JUXJCcbk = 16554; 
uint IiRLYA = 7089; 
byte axdpaXsCLmLMjmdbmNZHGdiK =  56; 
sbyte MfjkEdx =  43; 
ushort sBeKJbmxVYFIXmALFiXnueVOGx = 34102; 

}public void gJN(){
int iUmVWWbWnpgFeiXLTzm = 18995789; 
while(iUmVWWbWnpgFeiXLTzm == 18995789){
iUmVWWbWnpgFeiXLTzm += 315833;
}string BkomkbbOs = "QImVsgOPj"; 
uint EqnjASxJG = 58760395; 
float BLYdVDduFzcRaQSsAPfEZ = 5.666964E+29F; 
string MjFf = "FkQFwHiLUHyTYTksAbFpgT"; 
uint JEQWaitzoS = 835919543; 
string wkjOVzfgYbRKmh = "PGM"; 
ulong VRQlKLnYFfZtqdGt = 89002943378103309; 
sbyte WkzAmheBBEu =  115; 
string kgCScyti = "HJnOOqtmbguOcBeTR"; 
short EbYinOOcaMNIXOJRRZuqigqtR = 12879; 
ushort GyYkiCG = 33331; 
long ZaZotBoAxauJnUyY = 59050573823611091; 
ushort OCzwhhiqZuqUNReqnu = 50539; 
double eOkUjQc = -3.338926E-07; 
while(eOkUjQc == -1.607477E+26){
double aTlAW = Math.Log(1000, 10);
eOkUjQc = aTlAW;
int? JUCapXSAtJNnCYsgjFMYbXdq = 1083383;
JUCapXSAtJNnCYsgjFMYbXdq += 69736;
}uint ydZMzVTtMpqJ = 73294047; 
ulong KcxpcfAc = 6109992043860227; 
ushort AaqFPHJdCTtUuOfqydnlo = 21482; 
int bcyxOKSplclQTfnQHZFGKQcJqoFw = 21892145; 
while(bcyxOKSplclQTfnQHZFGKQcJqoFw == 21892145){
bcyxOKSplclQTfnQHZFGKQcJqoFw += 391464;
}int impRbcJqDoysPzFpMZmlaO = 48; 
while(impRbcJqDoysPzFpMZmlaO == 48){
impRbcJqDoysPzFpMZmlaO = 13207;
}long wzDC = 31071762205426420; 
sbyte RmGo =  32; 
string whqMuhRNGBIKzU = "QnRkHYxGdpIcISQmuEHZqkPllSbYg"; 
uint IhS = 337688; 
ushort yzZCmxNfiUPmnf = 55602; 
double FzFYOXyAFnZUQuAQXgiZ = 4.974084E+34; 
while(FzFYOXyAFnZUQuAQXgiZ == -8.016866E-31){
FzFYOXyAFnZUQuAQXgiZ = Math.Floor(9.952959E-30);

}ushort zucqoqSk = 57777; 
float jqZxfsIJjVeyeSlTUsJTzSlRuDkwH = 5.870186E-16F; 
double cnOsyoEdCkntXXAdzYTk = 0.9774363; 
while(cnOsyoEdCkntXXAdzYTk == -1.319928E-27){
cnOsyoEdCkntXXAdzYTk = Math.Pow(5, 2);
object HXzyGMshTxMIDWKkwMxSASIlhVWF;
HXzyGMshTxMIDWKkwMxSASIlhVWF = -7.082556E-31;
Console.WriteLine(HXzyGMshTxMIDWKkwMxSASIlhVWF.ToString().ToLower());
}long NksHHzZlDCQOaXFtzRaxoQIA = 11581730482080970; 
byte gKhEOJAwlOlDJaTwRwo =  68; 
uint DTgklUoLdBBtQUkVcqXTFTmXNllC = 49320314; 
string fkhoxEyFgBLLbojYjVsXqIsJGjyaI = "MaACnGNWAAUsNdiDWzcnqeK"; 
string zSzJmfyXtPKkBbEkmxTd = "VoBcmZXuxHqxTQOomltxwtnAsxyAc"; 
string RZKEEYHZgADXhtOHkQWZ = "AHECabLR"; 

}public void zqqZAyimjpHmmXSxf(){
sbyte PlKgeyssfbDXVhFblG =  -86; 
float zYGGsXlKMeV = 6.554653E-28F; 
string OkmDhedkItM = "JVHRX"; 
byte qmdBMAfJu =  241; 
long Ofom = 704670532475344; 
byte TEOenXJUMcKLBdbxJma =  81; 
double tJGyqBbVAgmfGCUoJKuTtmDpSDKPM = 5.795843E+32; 
tJGyqBbVAgmfGCUoJKuTtmDpSDKPM = Math.Ceiling(Math.Asin(0.2));
try{

}catch(Exception ex){

}double iOEMnLecygwgIIaQcEqVyn = 3.532205E+07; 
while(iOEMnLecygwgIIaQcEqVyn == -3.982754E+32){
iOEMnLecygwgIIaQcEqVyn = Math.Pow(double.NegativeInfinity, 2);
object nUaYe;
nUaYe = 6.104742E+24;
Console.WriteLine(nUaYe.ToString().ToLower());
}int Skb = 219748; 
if(Skb == 810456){
Skb = Skb + 554368;
}double NfENWAggyS = -6.912967E-19; 
while(NfENWAggyS == 3.033844E-30){
NfENWAggyS = Math.Pow(double.NegativeInfinity, 2);
object ggAfhNXBLftELeclKoTghVeU;
ggAfhNXBLftELeclKoTghVeU = -4.691391E+21;
Console.WriteLine(ggAfhNXBLftELeclKoTghVeU.ToString().ToLower());
}double nUXTEyyconLVKHlcGPyj = 18.51534; 
double KLHVzUbhaN = Math.Log(1000, 10);
nUXTEyyconLVKHlcGPyj = KLHVzUbhaN;
Console.Write(nUXTEyyconLVKHlcGPyj.ToString());string PxFokIIzNDxhFgdjaliw = "fWqFGYVTsgZ"; 
long XCsKpwbs = 82612335899777246; 
int bIsKAmLGKNFuxxzujDRpWOuYn = 689458873; 
if(bIsKAmLGKNFuxxzujDRpWOuYn == 364747){
bIsKAmLGKNFuxxzujDRpWOuYn = bIsKAmLGKNFuxxzujDRpWOuYn + 770016;
}sbyte oYbTnbeZKMoNzFktlwFTmVnn =  -35; 
ushort EYGqYxGgzxCWBMqlbe = 14530; 
ulong AtNXjpCuKNA = 83569085298667769; 
float MWLAdgVKljhHmeF = 1.70728E-10F; 
ulong dMOKbd = 20487103500761948; 
float ybzfiHLzEsljyxkbBkPMXmK = -3.394294E-38F; 
double DGwmKwqhRVKWybpgJST = 6.618655E+29; 
DGwmKwqhRVKWybpgJST = Math.Pow(double.NegativeInfinity, 2);
for( ; ;) {
Console.WriteLine(-6.168556E+15);
}double GqUnkFVSkhyldCee = 2.076738E+15; 
if(GqUnkFVSkhyldCee == 3.142433E+29){
GqUnkFVSkhyldCee = Math.Floor(-5.455777E+27);
object WTnSFlzooqlGmyLVCaQCFjuUAAPx;
WTnSFlzooqlGmyLVCaQCFjuUAAPx = 1.879334E+29;
}double RWoKOdbsNAttexbWQWtoRBwQ = 1.64849E-37; 
double KXBdPGPocjRMyVyRw = 3.683033E+09;
RWoKOdbsNAttexbWQWtoRBwQ = KXBdPGPocjRMyVyRw / 3;
 bool? XpBAYNhselRFSuACSapFZP = new bool?();
XpBAYNhselRFSuACSapFZP = true;float HXXXZhFBXpDTBgLiZWwLIKAkGa = 1.314613E-37F; 
long CniyYcRtGXJGBDqOqWfFIdigkX = 27806989379619749; 
ulong KciJqGqybc = 4898935293028580; 
byte fclaFebHpfMwPdd =  71; 
ulong mbNLeiOASApcHQ = 3641847553724694; 
int hWpNhRPuQAQiMuXdmflCp = 4761; 
if(hWpNhRPuQAQiMuXdmflCp == 496423){
hWpNhRPuQAQiMuXdmflCp = 900745;
}ushort GtZycEiTbqaLWlg = 64339; 
long OLNVYacuTytlJWtWkg = 62364222065429351; 
byte ubqXPWgLLKuZwILLAPJeqZaai =  252; 
float WELkPlaSfxxMQJkCMRpcfcKxegAu = -1.096906E-13F; 
ulong diOhZjjjISGdsXoJPAoqJRST = 51903213320131636; 
int IHVcDCIgwmyHdgRQFCc = 140736; 
while(IHVcDCIgwmyHdgRQFCc == 140736){
IHVcDCIgwmyHdgRQFCc += 284829;
}
}public void dCybbRyMoIOMWNKCxFwmjZ(){
float aoNiAwpZORzfKtlHjsSjcTFoPGQ = 0.0276833F; 
string dLBaNenlTsOXfnESc = "PdRhIThKyuTGF"; 
uint puwiszDVfYSoVAakOOsKZK = 57302243; 
long cHUsnNmlPUiJKUmhMjQFmkxifR = 71841900291695642; 
ushort aOKebJKgcedyyRYNmcHSkLmxBxz = 10382; 
ushort gjzshXALEEBogaOQuWJLezTF = 45133; 
short nFFjDLmYTZeKanDA = -28741; 
short FlJKcDALLJqTIRylgYtYFUcVQom = 11200; 
ushort ZQmOLoFUDIcZCoCfxS = 36716; 
string CKtqKiOTOpQnhJ = "GUghHlxLVFsNtLSuf"; 
ushort zfUxbg = 54759; 
string qlMuHbADMwwjeZzNswuiVVLS = "kwM"; 
uint bPlIu = 86516976; 
double skpJKa = 1.036112E-10; 
if(skpJKa == -4.567225E+13){
skpJKa = Math.Ceiling(Math.Cosh(5));
try{
Console.WriteLine(skpJKa.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}int ypkdmFJHJUP = 327022; 
if(ypkdmFJHJUP == 210913){
ypkdmFJHJUP = 695729;
}ushort WNKYf = 57235; 
double BQGXohwZWRlhulUX = 13.27722; 
double kHEOqekneiRdXAwWCyuz = 6.517144E+34;
BQGXohwZWRlhulUX = kHEOqekneiRdXAwWCyuz * 2;
 bool? McGZWgGSOqdBRlFMbTpddUYazg = new bool?();
McGZWgGSOqdBRlFMbTpddUYazg = true;ulong eFBQpinuaEEfjpmQCsBeGsFFb = 33070681147594130; 
byte VKDItLUF =  216; 
short fEwHTcfLlMZWOOGszfznjZfsgZLgu = 13882; 
ulong nbwaCpbcjkYyVOOUOsxAtcP = 35561914202188105; 
uint mIMsFRhdxdYbIxslRsbontkloEzQh = 21400076; 
float zJfbCjCnwXZQfP = 4.28595E-28F; 
long HolLTxBdbtsWNSeFj = 60819696767073440; 
ushort wiORJAhFF = 13536; 
ulong JCyihWzkhMBGTVzlyOlSykeV = 32421544784614455; 
ushort EJSZsetleKIiXNbVgRZVszyDVHmb = 40256; 
float hjVsPuzwg = -6.272672E-23F; 
sbyte meEWxJgJuCGel =  -49; 
string RsGkyBYcwlCJwSwgscpRcq = "XNENJpmEduKpgGVWMfC"; 
uint gznDfANi = 25; 
long MJzUOpMiJzoLDoBy = 3796085197183624; 
short DQBucgbP = -6550; 
double cXPPqCJjDZRZoc = -33.6896; 
if(cXPPqCJjDZRZoc == 1.99607E-14){
cXPPqCJjDZRZoc = Math.Ceiling(Math.Atan(-5));
Console.WriteLine(cXPPqCJjDZRZoc.ToString());
}ushort tQZAXItiIcbOkpAzdGgZDsFMCsP = 56219; 

}public void Wtl(){
long EYDpKieEYhTeGsuFR = 35609248542656166; 
int BhLKPdJNBVNVYUYuyTyMyzxfZE = 1845; 
if(BhLKPdJNBVNVYUYuyTyMyzxfZE == 287614){
BhLKPdJNBVNVYUYuyTyMyzxfZE = BhLKPdJNBVNVYUYuyTyMyzxfZE + 365482;
}sbyte FxmydbXGTJd =  -13; 
long uHOwASwbMGLlOzjNH = 1626077935692070; 
ushort RGNcCTOPVZUTmVDLHNRmzwfc = 43869; 
long kSAP = 4546172619747163; 
short KoNNCENORBlYipxALtUREXwknPjqf = 14810; 
string zmPgsohdoyaZDIVOgW = "joCxNTDzMt"; 
ushort fxJAVBpgwlQDgWxIHUwj = 12249; 
ushort oXRcmQRwLTPVDUhZnRPTYzYpQx = 58329; 
float YlyIEtCeycqFRfJqbTOzUhtuFU = 1.722904E+36F; 
uint QVZDNIsmZ = 622197; 
float OzohsoYPEftXQSddJDDEllwF = -1.046296E-10F; 
string OUmSasFWoNzJCxmPHFWPnipYdpFCc = "TZnZbJSBXzmgOWdUbPZHY"; 
long lLGFoPKOdUGUAw = 82598785548218779; 
sbyte tUinPDwLjDbow =  -108; 
int mKjmicFzAtwKTbYKEkkwGx = 508919854; 
while(mKjmicFzAtwKTbYKEkkwGx == 508919854){
mKjmicFzAtwKTbYKEkkwGx = mKjmicFzAtwKTbYKEkkwGx + 942149;
}ushort aBtuuOsUlq = 19451; 
long whMShEsLjTifRMKOmsIfltVp = 66362371830445516; 
float DtMdCtbDQHkoEcIgYZIG = 5.733734E-37F; 
byte tBtZckZdjjiqbj =  205; 
short JEnf = -20895; 
string KDaUype = "fpKbabBFIgetTqeOj"; 
ushort TzYpjykpAjfGUeIkL = 33069; 
ulong biYHYeKosy = 1403279625260204; 
uint QMawCCjolgtnweAKBs = 1011; 
ulong XcXwBfTFdLakzphEeyUwcldYoZo = 21716180659997555; 
double gaewHcJnjHjDWnT = -6.611039E-14; 
gaewHcJnjHjDWnT = Math.Ceiling(Math.Sin(2));
byte YXCWRwOcoqxHPltPiGDw =  46; 
double DMhawlWBmgjq = -4.160339E+26; 
while(DMhawlWBmgjq == 2.355947E-30){
double oLICSILWTQyZGZjJusQsQiBFU = Math.Log(1000, 10);
DMhawlWBmgjq = oLICSILWTQyZGZjJusQsQiBFU;
object xGWlB;
xGWlB = 6.12374E+31;
Console.WriteLine(xGWlB.ToString().ToLower());
}byte MyxuafkdIYMXVxjBNeMjiUPkglP =  44; 
float ZcDfLhBRAgPySFTnJ = 8.9462E-29F; 
byte PgaNRIknPpbkfZKuQMc =  2; 
double GuMcksSDScIWkVCtTBo = 1.379692E+14; 
GuMcksSDScIWkVCtTBo = Math.Floor(1.507371E+07);
int? qGiYJIXylbtVsddxg = 4531715;
qGiYJIXylbtVsddxg += 82390;long EQcfYMwXePzokzx = 66629511893142369; 

}
} public class YUOGWRHlMKTDM{public void DeZcsDdkgQqfMIbWzq(){
uint gdYPbmilDCFThkmKOVUO = 7588; 
ulong oQfYkQkCcdyAgjaetSJGkyXBwsiEy = 17192824679953066; 
byte Mpd =  213; 
string OEEUQuzeiNRTFESHIepNdig = "QWjQWogGTNIPFJHsSohmJ"; 
short HieIZCqKFxUeUkJiTG = -18366; 
double WdcZzYVPaUaytEHRHheDdcsLE = 3.849242E-28; 
if(WdcZzYVPaUaytEHRHheDdcsLE == -2.165644E-17){
WdcZzYVPaUaytEHRHheDdcsLE = Math.Ceiling(Math.Acos(0));
 bool? MJMmDeHTBOYHBghzPRWkBgNci = new bool?();
MJMmDeHTBOYHBghzPRWkBgNci = true;
}ulong EnRGqGwWCABpGVKel = 57301564347698558; 
ushort HAAupMFjPqLEswyiNoyeqikesmM = 25692; 
uint ZSUKPxbfjEaMUNCHp = 68057737; 
float DpsFyoucdakKmqSJJHagIeTeFnyEL = 1.390704E-30F; 
long EyCDeqhIdgEPcIxJXHZ = 50317875886053023; 
float qgUAdJUqfEoTAEYpoDHbVOpfc = 1.47286E-19F; 
string oehqR = "ZJpYYDsRcKPnNAhYzDPVDRP"; 
ushort wUlRAhCJR = 63924; 
int EJADWmwmEBBWpGFCVjiUSMDJfaEDm = 910828; 
if(EJADWmwmEBBWpGFCVjiUSMDJfaEDm == 992324){
EJADWmwmEBBWpGFCVjiUSMDJfaEDm = 895001;
}float tHEtjJVtpBT = 1.253026E-39F; 
ulong QMgRfRyVKTZYGawqMLlD = 51084580539855003; 
uint ZmQnLPTcOzVLUBjKpsUjh = 771824058; 
byte kkzOz =  135; 
int ZTNhqpHzJWNHSK = 47; 
while(ZTNhqpHzJWNHSK == 47){
ZTNhqpHzJWNHSK = 925790;
}short hAFjAEuooXopUHyCY = -29518; 
byte AVaGmCjZugFLezMnEhRcdu =  164; 
float CPubStUJFbmz = 9.901158E+33F; 
double wUisEkxViXQlk = 5.081695E-31; 
if(wUisEkxViXQlk == -4.242011E-18){
double DNLbAxzPFkTJIBAzNqtASUDJzWWZ = 1.177516E+17;
wUisEkxViXQlk = DNLbAxzPFkTJIBAzNqtASUDJzWWZ / 3;
Console.WriteLine(wUisEkxViXQlk.ToString());
}int ACUYHfjidRkFtdzC = 298988288; 
if(ACUYHfjidRkFtdzC == 319352){
ACUYHfjidRkFtdzC = ACUYHfjidRkFtdzC + 209555;
}int STnsJkGY = 48; 
if(STnsJkGY == 469847){
STnsJkGY = STnsJkGY + 644440;
}ulong QxLVoPZmGgmMHH = 69166452583689380; 
uint kmxTgHcpDCfUBIUdUbMDXTsDpI = 766838726; 
uint FZXRKctffZdXifBpOqQcSQeXWqVTE = 742669; 
short DWgXJXSKOWNzZKmbZTDXAhP = 25578; 
double tgACHmOjGiuteuXsdUszWHJSFGj = -9.184787E-18; 
while(tgACHmOjGiuteuXsdUszWHJSFGj == -4.825453E-21){
tgACHmOjGiuteuXsdUszWHJSFGj = Math.Ceiling(Math.Atan(-5));

}int nxmVTczlztN = 24; 
while(nxmVTczlztN == 24){
nxmVTczlztN = nxmVTczlztN + 879595;
}sbyte wakbfNSPzYCXaECpxCjKDNMsIF =  -92; 
sbyte aLVJtioduhibmamWxNxLZlkKAqIn =  90; 
ushort GNSynHnIwlmctbJpN = 31465; 

}public void kRLwWPAkjcBbkZ(){
int ouEZpiebzNjRJRaVP = 174896; 
if(ouEZpiebzNjRJRaVP == 338559){
ouEZpiebzNjRJRaVP = ouEZpiebzNjRJRaVP + 926870;
}long wdGUkccNAtViAecMJWjFIDs = 76986145503116800; 
uint kVeAgnLgejyfMosgXlwNdfSLFpY = 2388; 
ulong xxnojXmCJAzEtVVSWwKZwCMRe = 65502670193829705; 
short JzZ = -21635; 
sbyte KNnFDAQJybTDRxVLxkgEqL =  -32; 
string pbUluXlBoLPzmCEElt = "DsxVfYbPc"; 
int ZtkamyWydeHb = 735398; 
while(ZtkamyWydeHb == 735398){
ZtkamyWydeHb += 673794;
}int esg = 238901; 
if(esg == 773761){
esg += 272331;
}ushort jVVLwnl = 19402; 
sbyte PohVpUiZLRfAFnIlKOtq =  -90; 
sbyte WyfHbLZYtxtpcYGWZXODD =  -117; 
long IdiEyWPAHFk = 59090412185819642; 
string YGneSuttG = "UNQZYCw"; 
uint IUJyOhNMQARKpHe = 45007459; 
double GDwfayqLumjOpqow = -3.417261E-17; 
if(GDwfayqLumjOpqow == -1.94766E+31){
GDwfayqLumjOpqow = Math.Ceiling(Math.Sinh(-5));
object VSllZNFJWJhuwthEH;
VSllZNFJWJhuwthEH = -4.14931E-07;
}double jtxawCxaK = 5.113708E+13; 
while(jtxawCxaK == 122146.5){
jtxawCxaK = Math.Ceiling(Math.Tanh(0.1));

}float oSUSpgKnfmEOcSyedxZZfGBK = 1.09559E-26F; 
ulong bgDaFpkqdKkzxMidDPUiZEdBzS = 73042198459774162; 
byte LDiNHgaulCEuxygQnITycbCNHCZTy =  14; 
sbyte zjtaHhCTDKoXoomUexT =  43; 
double WJaxffghWiuXWmefiffaVtNF = -1.317043E-12; 
if(WJaxffghWiuXWmefiffaVtNF == -2.371865E+20){
WJaxffghWiuXWmefiffaVtNF = Math.Ceiling(Math.Asin(0.2));
try{
int YNRbuWZVpnZugYd = 9983705;
if(YNRbuWZVpnZugYd == 82636){
YNRbuWZVpnZugYd++;
}else{
YNRbuWZVpnZugYd--;
Console.Write(YNRbuWZVpnZugYd.ToString());
}
}catch(Exception ex){

}
}sbyte bebxSoKSJuXotSNENGiE =  -128; 
int oFDMOfDBPBweVmTW = 724911; 
while(oFDMOfDBPBweVmTW == 724911){
oFDMOfDBPBweVmTW += 736381;
}uint uDaYuIDH = 582094; 
ulong oDWNfTiPYNRqTJKSXCgDesqke = 81076482308086838; 
string VQpWAELlIMKeMXKKVyiM = "kLIBNbRjeARSbPcHGHZugJzwBuim"; 
string UqpLKeIwMCUbElTmLiIcpQj = "QDlmatTEfsGA"; 
uint eYYjUzAefmC = 6930; 
double bxdgXUhSwwtJpVlGtKu = -5.699261E+13; 
if(bxdgXUhSwwtJpVlGtKu == -2.556949E+32){
double ZMMJTYeVDtFxWgE = Math.IEEERemainder(3, 4);
bxdgXUhSwwtJpVlGtKu = ZMMJTYeVDtFxWgE;
object iUHhEpVWX;
iUHhEpVWX = 19.02439;
Console.WriteLine(iUHhEpVWX.ToString().ToLower());
}sbyte iDBpwWJDDtEitYchEwYAmknUgQaSs =  17; 
int zsMtxb = 46844469; 
while(zsMtxb == 46844469){
zsMtxb = 491112;
}sbyte sRoJNJcGLYoHiFWQ =  -79; 
double wKalKLDJAdHbOOHzjEJRUeTH = 1.608398E-33; 
wKalKLDJAdHbOOHzjEJRUeTH = Math.Ceiling(Math.Cosh(5));
int TfxPL = 138825411; 
while(TfxPL == 138825411){
TfxPL = TfxPL + 41340;
}
}public void BQIcXuBhdpPqKTjAYHhuWkLL(){
sbyte GaJfeCL =  25; 
ushort yXgfZz = 2431; 
int efRQIpjtISo = 94; 
if(efRQIpjtISo == 775756){
efRQIpjtISo = efRQIpjtISo + 486749;
}uint lnGewdcaAbwzjhuccOkaPUhdNhaA = 36; 
short ZdqczbwMGpLqltfY = 4435; 
float ZnziqfDqndDZgLVAfQqEbt = -3.745173E-08F; 
uint QRubbWhH = 2228; 
uint ZXWKhJcpEHpUahmoKHPcLgpLXlb = 82595969; 
uint KYkRuIqkQthHj = 499878; 
string KTXKxzjSIkNVIxAdmGVPqSIswZJ = "BfjykgWYDKnAaYNyF"; 
short WJDWVFl = 4173; 
float FnyPIsCb = -8.960175E+14F; 
uint CCEswPtfjOfQNTXi = 8551; 
double eNXwQLtGkwoGIxgbtD = -1.938225E-05; 
if(eNXwQLtGkwoGIxgbtD == 3.337758E-37){
eNXwQLtGkwoGIxgbtD = Math.Ceiling(Math.Tan(1));
object iCLwNUOzBipMWQMJDtt;
iCLwNUOzBipMWQMJDtt = 5.276666E-08;
Console.WriteLine(iCLwNUOzBipMWQMJDtt.ToString().ToLower());
}string DaOhEbdyWscRCbf = "GyouyGMjCWDzNEMVDOq"; 
long Ngao = 50425340806139530; 
ulong ayIa = 32954651652389992; 
short hoNLxDSpoeyqdOqaNqaB = 13913; 
uint OutAgACKpWbnVieLikKiDDGlPoH = 26235919; 
long WKuZpcdMQOuhucBxSIGRpboqk = 49061970610253126; 
sbyte FBiOI =  -100; 
uint wiwlioSPdISCUudPDUtUXYYDoW = 918692930; 
long XMRhUiQDZwZdMPNEkkyulONGVy = 16050313146112510; 
long KNAUYZ = 18034501881505449; 
ulong znIjZklyYXHMbcWDeHTyLD = 56549858869047763; 
string BJzSCBhA = "cWbtiWSXKdhSoZWT"; 
ushort kBkmXTxlsFea = 39007; 
uint WJGuSJSSdBXWt = 6758; 
uint fiWYpQZhXXEnTJAJysqpzEJsIhMZX = 1; 
long fVxD = 78100997093408705; 
int SznVTskB = 500011; 
if(SznVTskB == 156132){
SznVTskB += 78417;
}string UWUkOOcf = "LzK"; 
byte WdcXUCKhgqRFuWXfzOHb =  3; 
ulong txdwhUiTCoTEC = 8644271974361912; 
ushort tfhVmLd = 13165; 

}public void gKJBQfCmYzpoBtxSnFhA(){
byte RZmLpZYWhyekyRFKutaASdeUSNLdY =  29; 
double ugTKnfPCobqSkpVFmFjOA = 4.581704E-28; 
double XcCuRpYVxoEzkQNlKDSgRuVhkFZ = 5627.466;
ugTKnfPCobqSkpVFmFjOA = Math.Floor(XcCuRpYVxoEzkQNlKDSgRuVhkFZ);
try{
int mmuenpQzGHs = 7081784;
if(mmuenpQzGHs == 15621){
mmuenpQzGHs++;
}else{
mmuenpQzGHs--;
Console.Write(mmuenpQzGHs.ToString());
}
}catch(Exception ex){

}string gtjlpSgBbqlQMAWTel = "zlaYiaIgfmYDepCh"; 
double loOlIUSQOGBNPkzIGqf = -9.477916E-39; 
if(loOlIUSQOGBNPkzIGqf == 1.182349E+16){
loOlIUSQOGBNPkzIGqf = Math.Pow(2, 2.1);
for( ; ;) {
Console.WriteLine(-3791.094);
}
}double NaoIYHeJQoMK = 2.856431E-38; 
if(NaoIYHeJQoMK == -2.294339E+09){
NaoIYHeJQoMK = Math.Ceiling(Math.Tanh(0.1));
try{
int DAqybmYcaYCEDs = 5106366;
if(DAqybmYcaYCEDs == 61994){
DAqybmYcaYCEDs++;
}else{
DAqybmYcaYCEDs--;
Console.Write(DAqybmYcaYCEDs.ToString());
}
}catch(Exception ex){

}
}ulong KClEmaTslaUpaadasyJIi = 9546911482963637; 
int WZDnxNdRUbRYyKXiILpqcXeB = 551421770; 
while(WZDnxNdRUbRYyKXiILpqcXeB == 551421770){
WZDnxNdRUbRYyKXiILpqcXeB = WZDnxNdRUbRYyKXiILpqcXeB + 750089;
}sbyte HjXpyNHnnHPGqRC =  -90; 
long GbEmNZVbtUshMhVtsmMILVeccwhzj = 82478065933379805; 
ushort XuwtNVgVILYAnx = 2912; 
short XBjqRySpTfVGaRzYQOmoDq = -23577; 
double BOkENRjQxhuifgXD = -1.444658E+29; 
BOkENRjQxhuifgXD = Math.Ceiling(Math.Sin(2));
object IoMAyqLkTULcoiuRnDaVnQ;
IoMAyqLkTULcoiuRnDaVnQ = 4.307452E-33;
Console.WriteLine(IoMAyqLkTULcoiuRnDaVnQ.ToString().ToLower());byte hcEHyPXLQepYogdUySaRk =  175; 
ulong LQJopEiT = 4660957192279209; 
double XxjBPdsRoiBxzymtVH = -1.654053E+12; 
while(XxjBPdsRoiBxzymtVH == -3.244004E+32){
XxjBPdsRoiBxzymtVH = Math.Ceiling(Math.Tanh(0.1));
try{

}catch(Exception ex){

}
}ushort Sme = 59044; 
byte kPquDokSEbeRFpMFJSEDR =  173; 
ushort QbyKahHMnbKiFKLmEDbcIqYeonEl = 21435; 
long CuuECRCRxutEDGDZZgqYoItnAd = 10365152022393540; 
sbyte kgjwcMFIoOwAMoEfaNETbgey =  -8; 
sbyte SaYa =  -29; 
ushort PTXVVDcLZLJTIzaouQFpNdc = 48546; 
double INHpOacYALheSbQphtPGzGSLEkP = -5.72917E-37; 
INHpOacYALheSbQphtPGzGSLEkP = Math.Truncate(INHpOacYALheSbQphtPGzGSLEkP);
try{
int XqycVVNDlWWPUXgSgBmzx = 6191964;
if(XqycVVNDlWWPUXgSgBmzx == 59042){
XqycVVNDlWWPUXgSgBmzx++;
}else{
XqycVVNDlWWPUXgSgBmzx--;
Console.Write(XqycVVNDlWWPUXgSgBmzx.ToString());
}
}catch(Exception ex){

}ulong mHCSwhV = 10089754352561783; 
sbyte ZjymQDJwtqzpZp =  -39; 
string LBqoeeEbMVbxgwehPGupVy = "SxcRx"; 
sbyte DwjKzJOhVfUkeH =  40; 
double WuhjKXKQn = -0.008996769; 
double GmxWxuILowLsx = -3.873976E+33;
WuhjKXKQn = Math.Floor(GmxWxuILowLsx);
try{
int cXlSyAUkmooSROnQncMPZXODZIn = 9995520;
if(cXlSyAUkmooSROnQncMPZXODZIn == 4891){
cXlSyAUkmooSROnQncMPZXODZIn++;
}else{
cXlSyAUkmooSROnQncMPZXODZIn--;
}
}catch(Exception ex){

}uint NDMjesS = 650260; 
ushort ihMaVfiKTQlPomZTG = 30643; 
long NOUYmmOJjg = 57780627719827166; 
uint RtHsCOxWfAmZXiQunNOIqBANGGfWx = 181746718; 
ushort Myu = 9487; 
uint zRRELUjJADyUWy = 636259924; 
short UNWIABmoSZlMwmMppRBWOTwWHt = 31714; 

}public void mXjegBnPBQqqqxckyOhkhHhZgufbq(){
double fCn = -2.819989E+29; 
fCn = Math.Sqrt(4);
Console.ReadKey();ulong JZEHQiLa = 7750978596326712; 
int eSgpNzKBXlEjPCtpBUiRHVTjUTLj = 1169; 
if(eSgpNzKBXlEjPCtpBUiRHVTjUTLj == 991336){
eSgpNzKBXlEjPCtpBUiRHVTjUTLj += 49718;
}int dsmeWplAxOSAXKdxTNpjZXxTOBk = 282671681; 
while(dsmeWplAxOSAXKdxTNpjZXxTOBk == 282671681){
dsmeWplAxOSAXKdxTNpjZXxTOBk += 965015;
}ushort iIeVpKtNf = 960; 
byte FcBymMuWg =  233; 
ushort weukiEgCAyV = 51680; 
ulong ZAHjqSEnGPOQXZECgmnPAlohE = 26486423947281514; 
ushort UpWFBMhzGxtGgYYqJBasNkz = 40116; 
ulong VCzIdhDmJljJNcsOJMjVOLORzjIdZ = 66929129456106693; 
double ldToNhZNXZZC = 2.238335E-07; 
ldToNhZNXZZC = Math.Floor(8.516343E+09);
try{

}catch(Exception ex){

}ulong pMzTbShxcdYhtxdsgLfuQTBm = 84831345456963558; 
int CDAfQNLZVPlgULG = 22; 
while(CDAfQNLZVPlgULG == 22){
CDAfQNLZVPlgULG = CDAfQNLZVPlgULG + 994499;
}ushort lfZkgUJCj = 36258; 
double BTaUsKRYLaHiWxAhsViyeqmaRDF = -1.346689E-21; 
if(BTaUsKRYLaHiWxAhsViyeqmaRDF == -489132.3){
BTaUsKRYLaHiWxAhsViyeqmaRDF = Math.Pow(double.NegativeInfinity, 2);
 bool? CjotzPjGadQfLN = new bool?();
CjotzPjGadQfLN = true;
}long dNjAaUFjlLHyMclyHisfEg = 67785959589528198; 
uint aculSLey = 75400009; 
float QRVNQdufCFgXSnpSBbKii = -9.442772E-06F; 
long aFVdVwQokgtnHNxJxKwV = 1634035929135005; 
long uuleQzzbGjpIIhXET = 9036178144773336; 
int BQZ = 76164760; 
if(BQZ == 739508){
BQZ = BQZ + 974243;
}string XXkwdlFQMxyJdtPiQ = "tzTizXkHxDZYwYCqqpnq"; 
long iwUEdgVtYzTeGFJOAnjxQcQN = 51051482206517519; 
double oznUUdLcECfptXIERa = -2.514014E-21; 
oznUUdLcECfptXIERa = Math.Ceiling(Math.Atan(-5));
try{

}catch(Exception ex){

}byte usWC =  201; 
uint uumslwCFIDSoxRllKQV = 155847; 
ulong EYp = 79632175534383890; 
uint yKgaFLKJkNOligm = 87909370; 
double ItKtWmnIeojcXqN = 2.385546E+37; 
ItKtWmnIeojcXqN = Math.Ceiling(Math.Acos(0));
object dFuaLQUwsncZc;
dFuaLQUwsncZc = -1.840233E-23;
Console.WriteLine(dFuaLQUwsncZc.ToString().ToLower());float MwPWBIaJzgHtxYqCMbKzbBQk = 1.049659E-22F; 
sbyte jRxhmPIGVUuNqG =  17; 
float IjbaGIpAsOlGFW = -54881.27F; 
int IKzgHqMGSHWhxWX = 33653430; 
while(IKzgHqMGSHWhxWX == 33653430){
IKzgHqMGSHWhxWX += 581962;
}sbyte JUbwaq =  113; 
sbyte NgWhXCEK =  -57; 

}
} public class PJDkqpJPFnAhUzEQlFWLoVCNwoaPS{public void ythFuzfKKfsCsRKFmyFgpYNE(){
long gQPboTXPQgapdfKCVRtNELdRcD = 75708780826666822; 
short VIQzFDaEEWu = -1500; 
ushort cWjYpwNYZMJlpqBxxDOuHmaV = 11004; 
string AmgiMoLxKuVjWEWJePslKd = "ARLchQ"; 
double QpLmLTDXXG = 3.754661E+24; 
if(QpLmLTDXXG == 2.108316E-12){
double WfRObxfHsbmDHNefUkp = Math.IEEERemainder(3, 4);
QpLmLTDXXG = WfRObxfHsbmDHNefUkp;
object uHPmfYzSkylajYCKRGlkWLcmdX;
uHPmfYzSkylajYCKRGlkWLcmdX = -6.727353E+23;
}ushort VgtCAGEqVJFCihCZkKYzGV = 35029; 
ushort HGFpOKXSqbpffbS = 12574; 
string OEKaSgVXqgWg = "FYdSaZahZIJRfkUfJgnDEOQOGNywQ"; 
short WkcLgbiSjckOWajMVaRJaJEx = 21231; 
sbyte lmDGqIRNHd =  -1; 
ulong BndA = 75917662755191398; 
double QmHGUYNWFIyTgemDJwnOi = 7877349; 
if(QmHGUYNWFIyTgemDJwnOi == -3.229529E+25){
double aktYbBZyGP = -120.0552;
QmHGUYNWFIyTgemDJwnOi = aktYbBZyGP / 3;
int? bzSQBTsggkBdJCC = 5160292;
bzSQBTsggkBdJCC += 16865;
}string EkQAxFcidGYzaWTUehyXRyW = "dfypOPXKMOphpIOWa"; 
uint WelkjAOitMgVfon = 322331; 
sbyte woIoMmGMExuCWsZaySVmRZ =  30; 
int GTkTcmyPndjnQBYFNkMYkjDgnQEwU = 420926178; 
if(GTkTcmyPndjnQBYFNkMYkjDgnQEwU == 313454){
GTkTcmyPndjnQBYFNkMYkjDgnQEwU += 133459;
}ushort SBjWMInPMIqXI = 4959; 
ulong GOwpeFmKbUtICkAtFjVKCbzCnqD = 7496246769888430; 
uint dpnmBVSoHmBhYtyietEpUb = 5351203; 
sbyte FIDaOOXIBUIxagG =  -66; 
long bZme = 26062913820490748; 
int icqkNGAM = 3217; 
if(icqkNGAM == 530079){
icqkNGAM = 649267;
}ushort KHuKbekATUzuVL = 53179; 
uint xnXQwAbFoPWm = 1271; 
float xCj = 1.344354E+27F; 
sbyte SRqT =  -93; 
sbyte ABsletwGohsfNpnmINEtAiGSA =  -62; 
byte ZdHUA =  118; 
short UZbCGQmcoNxNEF = 11697; 
sbyte MpqUANRiqHkByHobSs =  41; 
long OKWD = 53531861225632249; 
short hXCumjsoibDRghMMlauBYHu = -10340; 
double aLQ = -5.346155E+24; 
double wCKACUaNFKMPjugQOCEjzljIAXkDK = -5.582673E-34;
aLQ = Math.Ceiling(wCKACUaNFKMPjugQOCEjzljIAXkDK);
int? mFTRAeBqixqZab = 6712278;
mFTRAeBqixqZab += 6322;ushort PnZoxdSpQTIzVOLthuhmNwCOABQU = 7230; 
string WGfIEqRjypjuzuCJWMKj = "VhiuNkPWmY"; 

}public void LkMTPfhc(){
short LWDadtXR = 24030; 
sbyte VtayWnQfbmjsaJ =  -65; 
long TRFHTcjlGtKSReuQOQRL = 63126834320062512; 
int sQKoXbDPbpRlpUgnj = 79; 
if(sQKoXbDPbpRlpUgnj == 863055){
sQKoXbDPbpRlpUgnj += 411544;
}double aZnKEfJHd = -3.736648E+28; 
if(aZnKEfJHd == 9.699848E-30){
double BUjDXjJfPMdpzwMcXcnfzkhF = -5.096479E-19;
BUjDXjJfPMdpzwMcXcnfzkhF = Math.Sqrt(3);
aZnKEfJHd = BUjDXjJfPMdpzwMcXcnfzkhF;
try{
Console.WriteLine(aZnKEfJHd.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}int DzwBjpZTppNFpdWDzNnQbuB = 98222877; 
while(DzwBjpZTppNFpdWDzNnQbuB == 98222877){
DzwBjpZTppNFpdWDzNnQbuB = DzwBjpZTppNFpdWDzNnQbuB + 77103;
}string jcApPLgSgnxOFqzhYhzZEJNMhYN = "KpPPpwDDQNhRqUgCG"; 
int qBqbcGaqBoNiRYGhRzHzjPVslaH = 67676825; 
if(qBqbcGaqBoNiRYGhRzHzjPVslaH == 126153){
qBqbcGaqBoNiRYGhRzHzjPVslaH += 542445;
}string atoNjodbmcFqTM = "pcYhNDFZPbYsIYdHGPDFPD"; 
long jfNJoHKOWXlhhuo = 4288221607285721; 
string xYCpgWO = "BYHPWLiskxLN"; 
double lAKjxcammJLATIh = -3.273781E+22; 
if(lAKjxcammJLATIh == -5.783443E+23){
lAKjxcammJLATIh = Math.Ceiling(Math.Tanh(0.1));
int[] cHXBLqnLXdyRBkNHLneRPRG = { 8150215 , 14424 } ;
Random NMIlMYwlVVeQTAOAtLLabR= new Random();
Console.WriteLine(cHXBLqnLXdyRBkNHLneRPRG[NMIlMYwlVVeQTAOAtLLabR.Next(0,2)]);
}byte abhgYlblOYJcYZJlWGfXOkBFyd =  29; 
int VVBYSXtdPXTPHMPRqMLqQ = 212607487; 
while(VVBYSXtdPXTPHMPRqMLqQ == 212607487){
VVBYSXtdPXTPHMPRqMLqQ += 807360;
}uint JUfxESdidgcphgJmhg = 733448; 
ulong qwnTZL = 18566590022914557; 
ulong dTLJnGIbIPDO = 8161451815752966; 
sbyte StKwuVyIZGBPdGafSMNml =  -75; 
sbyte BNQdLFAcltasZSkMFZPWnt =  104; 
long JKSXUyIkYVIftWwgyEKGIgZ = 18406990266118558; 
float pbUIe = -5.646598E+31F; 
ushort aXEHlc = 24147; 
ulong QkffDKqWlklZCORZcUCAslOl = 12964681200412732; 
ushort pGakAjpieKghAiNHiiRp = 30150; 
int TFxIwDPbiEUzkZ = 406466108; 
while(TFxIwDPbiEUzkZ == 406466108){
TFxIwDPbiEUzkZ += 143863;
}string sGTjN = "akfiMigGcC"; 
float oIYFGLDHqNAfnxsOmQP = -1.684937E+23F; 
uint XyL = 3056305; 
float sQoESxgOa = -9.429937E+10F; 
sbyte nLZVpQttlNJQOHagUYBOfuKRKFWH =  -7; 
ulong EKGOmzQxkASgcGOdPgHg = 56446043342483222; 
long NUxfefiTOJdDoomnHkVBhASJyy = 24446120187860243; 
ulong KXBJVVtXcHRcDFuXokMmcNIIMtYD = 10295004604923983; 
long FmgUGRGql = 27971366263354022; 
double TWCWQFFOTB = -4.633074E-38; 
TWCWQFFOTB = Math.Pow(double.NegativeInfinity, 2);

}public void uneDxxAzSuoxMEiPS(){
sbyte oEjsFXUznIJRWaPJbkNFHmt =  -97; 
double ICpWFUKKCXkWBGCXlQjDsxWUh = 2.486545E-31; 
while(ICpWFUKKCXkWBGCXlQjDsxWUh == -8.827354E-36){
ICpWFUKKCXkWBGCXlQjDsxWUh = Math.Ceiling(Math.Cosh(5));

}ushort BaNgQMfze = 1258; 
ushort juesKuAyXYewYeDEU = 55038; 
int USmLPV = 22; 
if(USmLPV == 27405){
USmLPV = 308298;
}byte JAPOfgxVpCWqi =  182; 
sbyte FmxnIshuDOknUxXPk =  18; 
ushort PfMz = 46439; 
double MOzNzcbXPUXNqswlq = 2.220312E-26; 
if(MOzNzcbXPUXNqswlq == 1610381){
MOzNzcbXPUXNqswlq = Math.Ceiling(Math.Atan(-5));
try{
int iUmReLtUuCDXIhHq = 5307608;
if(iUmReLtUuCDXIhHq == 93907){
iUmReLtUuCDXIhHq++;
}else{
iUmReLtUuCDXIhHq--;
}
}catch(Exception ex){

}
}ulong pkWhww = 68247221344399273; 
ushort Lhn = 64144; 
ulong CpFZVYTNEOENeqGPmMuztZCVUg = 71147926100655141; 
long EwBnaaHIPRcfwWVDFoBD = 82048702623848165; 
sbyte tJIYlRkfztsTfdetYxAgleV =  -96; 
float pNOBHyDbztyxKdqARhjVEcJfCY = 1.045502E+36F; 
uint NCgsCzqXMU = 789; 
int WDMwPMnOWXqndIZIOEBjHV = 3548; 
if(WDMwPMnOWXqndIZIOEBjHV == 149892){
WDMwPMnOWXqndIZIOEBjHV = WDMwPMnOWXqndIZIOEBjHV + 873332;
}ushort EAyYjYloiP = 107; 
double ibmuJYegOEweamuSWFu = -3.589109E+10; 
while(ibmuJYegOEweamuSWFu == 1.62339E+24){
ibmuJYegOEweamuSWFu = Math.Pow(double.MinValue, double.MaxValue);
try{
int MSxlkGGXP = 4466310;
if(MSxlkGGXP == 68344){
MSxlkGGXP++;
}else{
MSxlkGGXP--;
}
}catch(Exception ex){

}
}sbyte EzfA =  -116; 
uint WOlonFchBFbtqhSinW = 50049538; 
ushort RxAF = 52532; 
float efEzknJcVKdGlsHwpQULpSE = -2.633412E+36F; 
sbyte WQQJPjuwzzVgUiftlGUkKkajPV =  -92; 
ushort DjAcuMlfb = 18847; 
ushort XhWuKaCjAqOeXiMQGTTcWiWox = 4943; 
uint UmkpztNddPfUGn = 21085105; 
ushort ekaxAhwtUOkIY = 39726; 
uint plgyH = 415100; 
long ROYyJwMTcZnxyOdVeAZSwR = 17190288556515879; 
uint tKJuxWktAOWGlJSycDCexQpa = 5950; 
double THVozzdNfKzhnGsHVUhRee = -2.816125E-08; 
if(THVozzdNfKzhnGsHVUhRee == -4.175532E-11){
THVozzdNfKzhnGsHVUhRee = Math.Ceiling(Math.Tanh(0.1));

}float DfLEOuVSMmsEdJfMUR = -6.500391E-22F; 
string YURGdUHbXklADwJUPmmWtl = "LJMfnSRTFCRhQM"; 
sbyte akLogqFfLca =  -119; 

}public void ZgsMzgXq(){
ulong igsHXLnThIVlL = 66425120126182456; 
float IGwzxsGkZmSCiPKHuyjTuoNx = 1.921312E+21F; 
sbyte SMtiqsKbDmSCCxVZTgOBJyk =  118; 
string usTYOHiTsnUJjQfJehFJqYtOEFaJd = "CuhsXqhEzksJe"; 
long jWXuZ = 59753814131420475; 
ushort HBPXNACR = 56317; 
ulong BKuqJTMJuIcndbzb = 83160086466215849; 
float hVGIeeHATtYaZDmxbCFmLVnyLg = -4.065258E-08F; 
ushort wRBPMQcMEXIF = 10995; 
sbyte QMYf =  34; 
sbyte ZEhlIiXXOkbBXhBjRytpYI =  26; 
sbyte iuVwsoQlJSWHBiUOGixEJiV =  -53; 
uint JnQOsYb = 481273; 
ulong DwV = 17850983851909320; 
uint tzjDeazGUkHDxVBXG = 67; 
sbyte cOBsuUGLYRtOnuTPzuAowJk =  -56; 
long tPHXnGtPpKJkKfTUVtmPhBHig = 2819038119372935; 
int QebEOwzdjfnKtxBOS = 87; 
if(QebEOwzdjfnKtxBOS == 173432){
QebEOwzdjfnKtxBOS = QebEOwzdjfnKtxBOS + 464776;
}ulong wCzWhyBhFcQTQAtKyyc = 2122342687425341; 
ulong KIHkXtTZgbJNDPuLUcDBZuMxXI = 26967674332532744; 
float iJSfBKpwtfxba = -9.420179E+17F; 
long pToVknpPMqQFudnUZClj = 40063271598046952; 
uint EFOHtw = 47934757; 
double DpOtAQYbihbDB = -4.603588E-25; 
if(DpOtAQYbihbDB == 9.354727E-25){
DpOtAQYbihbDB = Math.Ceiling(Math.Cosh(5));
object RHglWFBdPcuebdoxdRw;
RHglWFBdPcuebdoxdRw = 9.760721E-18;
}short aSHNTicepqCTyMBFbux = -9826; 
double QQMqxkHU = -2.996908E-06; 
double JbZpnDljpsXKOfZIQbQbDNSzx = -3.08624E+29;
QQMqxkHU = Math.Round(JbZpnDljpsXKOfZIQbQbDNSzx ,1,MidpointRounding.ToEven);
byte URmUjsJJWADCuIZ =  198; 
byte PgJWCHFpShyZBRnKkbxpiYkwEA =  130; 
long njNIhHePYpGoTBBkWYLmLxYsWL = 67051539422593791; 
int KssjjVmLQaoyKjHBPxiRkwMyQiCGm = 388639; 
while(KssjjVmLQaoyKjHBPxiRkwMyQiCGm == 388639){
KssjjVmLQaoyKjHBPxiRkwMyQiCGm = KssjjVmLQaoyKjHBPxiRkwMyQiCGm + 524241;
}int BdPjmCVxtmdMCqzmMOn = 91179656; 
while(BdPjmCVxtmdMCqzmMOn == 91179656){
BdPjmCVxtmdMCqzmMOn += 644276;
}uint GSBhJcUntuI = 887916729; 
ulong xVy = 22176895570793129; 
double gbuDPkoFSewJIcfTeQuPVY = -8.200028E+33; 
double weihqZehKBxERc = 1.839994E+12;
gbuDPkoFSewJIcfTeQuPVY = Math.Ceiling(weihqZehKBxERc);
Console.Write(gbuDPkoFSewJIcfTeQuPVY.ToString());sbyte HFla =  -19; 

}public void phUqOI(){
int wcJEspxJf = 388066484; 
while(wcJEspxJf == 388066484){
wcJEspxJf = wcJEspxJf + 91498;
}short tiSWdOUWdewbblMZeKEmWJPHXAqHe = -3208; 
ulong Cqwz = 86987888475762607; 
string dJIqFTCfdKulZFxnRGst = "KQnpQsnzntEHowIY"; 
ulong TCwnLezoNSBRKTHhtTubPBmIPp = 52201878454737678; 
uint exfiRBGxXyKKZgcPFMetnOAwn = 602578836; 
int lNWw = 7799559; 
if(lNWw == 218845){
lNWw = lNWw + 214117;
}short RLqGgGEgZKsJfEpWKUXVbtLLOoj = -29113; 
sbyte tBQz =  -15; 
string PhnbyeC = "nolObcqntTzZH"; 
float wBbXqSSIgmNVNJTEnipAaYodY = -6.097472E-21F; 
ushort ZdDXqSCxgkDwIeBnhEG = 33963; 
short QpsRykXY = -15006; 
int chjkYxQqhgYNds = 789136568; 
if(chjkYxQqhgYNds == 471955){
chjkYxQqhgYNds = chjkYxQqhgYNds + 626869;
}float PmlFBwpziXX = 3.24216E+07F; 
double cjhPjhcSEoz = 3.197731E-07; 
if(cjhPjhcSEoz == -5.470089E+22){
double WZuWwaKYaPeOV = Math.Log(1000, 10);
cjhPjhcSEoz = WZuWwaKYaPeOV;
int? RLhNwIobRUpb = 3905949;
RLhNwIobRUpb += 17758;
}uint ROjLJ = 395261477; 
uint hQGGCseUFftWTRHW = 956; 
long dyDKJETfqhXVDJQIlZSLLsQceh = 46474392546498121; 
ushort nVWYzZDxiyukbZKqmZhMHA = 39956; 
uint WDDQelVmeuqcUmMPtUH = 74682910; 
double ebGKAJdPowsCxaHelA = 5.897105E+32; 
while(ebGKAJdPowsCxaHelA == -5.177288E-19){
ebGKAJdPowsCxaHelA = Math.Ceiling(Math.Cos(2));
Console.WriteLine(ebGKAJdPowsCxaHelA.ToString());
}ushort bckSmVjKixdYxgwUXFpuMe = 27170; 
uint SQsfMCLFFFPRnUF = 90042621; 
short YbnUSKumUaZWHNwweSEx = 26003; 
short RDGdJLLPRNOAw = 4316; 
double xsoREFJidTABOqVN = 2.52123E+11; 
while(xsoREFJidTABOqVN == -4.308483E-21){
xsoREFJidTABOqVN = Math.Ceiling(Math.Sinh(-5));
for( ; ;) {
Console.WriteLine(-1.391985E-31);
}
}double JqSwlLEd = -0.01004136; 
JqSwlLEd = Math.Pow(5, 2);
Console.ReadLine();long mGelzKGGAEcRNHIEDtRzOpip = 40899578911685631; 
double PhatUHtbSkwNwiQIOLul = -9.836726E+33; 
while(PhatUHtbSkwNwiQIOLul == 6.758976E-05){
double wokUfSADiAjfNzYVbntY = 9.653429E-28;
PhatUHtbSkwNwiQIOLul = wokUfSADiAjfNzYVbntY * 2;
for( ; ;) {
Console.WriteLine(-3.923187E-14);
}
}long iAZPplwLDsbbLhXbIlMtlEq = 42061176740113740; 
short xKRqCPsP = -263; 
int yyhnFNjdssCGhHEWMTaSmJQoyfK = 44; 
while(yyhnFNjdssCGhHEWMTaSmJQoyfK == 44){
yyhnFNjdssCGhHEWMTaSmJQoyfK = 1594;
}ushort OllXXCbaTVeKLYSHWAMJOHSQAcdH = 63865; 
ulong GMKQKBVcqlJWttxZhFNHmnNh = 25705355530029985; 

}
} 
public class AUBWIuBdxiufXeoyFRPXmCNuKz{public void xBimEyWZqmJHJy(){
sbyte LcwFYj =  75; 
uint waGYhgmjYdDnVRIbfYj = 207164989; 
sbyte eubqnTylMKIhxqaj =  -98; 
ushort KtGTPlOETlGDuo = 34242; 
uint fPYqCSRFmJkKePdAPx = 47; 
uint fBpEafgnS = 805878552; 
ushort qyeiC = 50919; 
short DDTOyCdGTwbdSKyaX = 22337; 
long ZRpUJLNbKnFLlLE = 6465324176849591; 
uint skHaekKcRDMlYeNPbVFQqSh = 48; 
int ogupSP = 32; 
while(ogupSP == 32){
ogupSP += 599860;
}string IuhSMzLQPbNuKxAdCBCdwVylTyS = "mJapCWKyZnNexzssEyfKRwF"; 
uint ZAMX = 969091378; 
short WEfbxSOMcemOyHWEeJxs = -23080; 
double eAmDxYBdoyyAVlpgGqXPqqMHt = 4.22341E-28; 
double bDjpsOCaYpZhasOCQajiANN = 0.2260602;
bDjpsOCaYpZhasOCQajiANN = Math.Sqrt(3);
eAmDxYBdoyyAVlpgGqXPqqMHt = bDjpsOCaYpZhasOCQajiANN;
try{
int IQNBXBjkOXoZjmptwHkDQdMMX = 427799;
if(IQNBXBjkOXoZjmptwHkDQdMMX == 63826){
IQNBXBjkOXoZjmptwHkDQdMMX++;
}else{
IQNBXBjkOXoZjmptwHkDQdMMX--;
}
}catch(Exception ex){

}float ZFBaZW = -4.777407E-09F; 
short YpaEdbDIKXWZtiBltfhGFFMJehkWp = 25911; 
string yFiiXpgjnuXTkaksAODdlMuuFs = "WQGsPVcCPasocYYq"; 
byte SydWTyCqYIMjPEGQktI =  7; 
sbyte hzxqhzAfmujNNUxaXiQyWwQFlOJm =  67; 
int izUoFACocRTuqZbRLukxz = 7105; 
while(izUoFACocRTuqZbRLukxz == 7105){
izUoFACocRTuqZbRLukxz = izUoFACocRTuqZbRLukxz + 846617;
}sbyte PmsILboSFRBw =  96; 
ulong dbAFkXYSEilYNSDWIxW = 56238670436516612; 
sbyte KaFyeYMjnUxRwOAzcC =  125; 
string SYXAbDoSGzYRoTiHBIUQMuDBxiN = "hVkZdISbH"; 
double xxqI = -0.0001487622; 
if(xxqI == -6.166653E+31){
xxqI = Math.Ceiling(Math.Sinh(-5));

}ulong YEjEWMyubmYLJoPlwJokIktp = 25999065796235871; 
double sRylbtXWVSXyXlayYiKFgNkUmIcw = -1.031493E-29; 
while(sRylbtXWVSXyXlayYiKFgNkUmIcw == -5.925814E+16){
double jIWmLoCsTqybSXFkuPFHEZtp = 1.240413E+33;
jIWmLoCsTqybSXFkuPFHEZtp = Math.Sqrt(3);
sRylbtXWVSXyXlayYiKFgNkUmIcw = jIWmLoCsTqybSXFkuPFHEZtp;
object TMkocKOCNsxyKC;
TMkocKOCNsxyKC = 7.531264E+24;
Console.WriteLine(TMkocKOCNsxyKC.ToString().ToLower());
}float hAgghYiidyctgWcG = -4.099893E+15F; 
sbyte GAZwnXLAEnhIDXCcmk =  -12; 
ushort ZQxnE = 12997; 
ushort GlVCoUJ = 33122; 
long jmJGPowRjwasSmNBHJDhLNpqYb = 5882560024773230; 
byte wuagSNa =  250; 
sbyte uJOPnaYEh =  -17; 

}public void qHOdoDfEzQAZGelIwoXNqjLIEqAQ(){
float YjRkeNmdDjVwKxUKiK = -1.356369E-11F; 
int jKGhVfAYdp = 88459561; 
if(jKGhVfAYdp == 157300){
jKGhVfAYdp += 396720;
}string uqNOSVBoOUiPOAa = "ZgFXRhFFfFKQFFCOKEYlC"; 
ushort KIHsGPoCmThpbQdHmMhJmlKLj = 23530; 
ushort kBPiE = 18359; 
float kQZPQdyWxnB = 8.09557E+09F; 
double BtNGzXWimwjpMeZNNVOxxN = -3.527845E+26; 
if(BtNGzXWimwjpMeZNNVOxxN == 8.092816E+19){
BtNGzXWimwjpMeZNNVOxxN = Math.Exp(2);
Console.ReadLine();
}ushort wULPcnFKEysB = 49161; 
string bInRToHgnyoucSOlCuIDs = "nTEPxPhwYOMlkdVPCdPNx"; 
short TDRPlRgfBqquumdTRZTzBJQ = 8906; 
long NSENFnVfLmF = 46324582176381160; 
long DykYOEUmjMkeUenC = 56138003965131708; 
float oGRx = 1.535573E+19F; 
string kwtPQKbTBBuVbBEhzhRjkZHWg = "onwNQybdTThXWtywbpVWcO"; 
string XNWSGkOyJfwOyOLWTYHKZ = "lzNPYpbkJBkLPcCjy"; 
long LoQXK = 77549571976645913; 
long CyfQlzxzyUDMYI = 68134843229940624; 
uint RLLc = 57; 
short bsCdeIyHnRtchloYEtEkOukfX = -29787; 
double NupTCLLGfqFTAXbw = -0.0008810948; 
double hdHhw = 0.003739204;
NupTCLLGfqFTAXbw = hdHhw / 3;
int? UUBACyBJSmPLLRtQdAgseaVCTtHd = 2902757;
UUBACyBJSmPLLRtQdAgseaVCTtHd += 45655;byte ssIVpSTYZH =  36; 
ulong lidmBhNlVBDJms = 60358365620117301; 
short HULkiUWPIJCMhNOpA = -30602; 
ushort fmdBMnyclxGfRCHHxDdaFNaER = 26598; 
uint QGVPJAxtQkzUyTElcwUGggU = 965406571; 
ushort smAKqMKmGB = 54906; 
ushort fmRlejELjOBw = 33189; 
double JLZQmlegpChhNHXMkYDB = 5.406426E-15; 
while(JLZQmlegpChhNHXMkYDB == 3.207647E+21){
JLZQmlegpChhNHXMkYDB = Math.Floor(22995.89);
Console.WriteLine(JLZQmlegpChhNHXMkYDB.ToString());
}short MccjjOyANQYU = 16169; 
string VxKQTunlSuZQyfA = "xHKebjS"; 
sbyte BWaDemosjakIxyXyRkPkY =  123; 
ushort HCncXyLEF = 9159; 
ushort nJOhLyneoCZiMIj = 65509; 
string LBDNVDYlwMpHcoeBjixIBCMoYfD = "qpnLC"; 
long SUBNkizbCPJzKe = 36058806646086359; 

}public void PPaTVfzFe(){
short lGJwlAbAjBoISaeRmhgpmcILXk = -12464; 
int djEfRSHPGqVsKGNAhiSQnkxYcHj = 968510108; 
while(djEfRSHPGqVsKGNAhiSQnkxYcHj == 968510108){
djEfRSHPGqVsKGNAhiSQnkxYcHj += 474218;
}int pMDahz = 62; 
if(pMDahz == 967100){
pMDahz += 554335;
}ushort kQbyNISOFcjJIQipNTfAyOFhKs = 14420; 
int OddNdNIZLMWnDgARAQl = 511752; 
while(OddNdNIZLMWnDgARAQl == 511752){
OddNdNIZLMWnDgARAQl += 935951;
}ushort kUOwxmhRYgWFcC = 21110; 
string jlVIfq = "CZgfpZHQ"; 
int xeXpBawpBKE = 1802; 
if(xeXpBawpBKE == 423237){
xeXpBawpBKE += 946368;
}byte DPfEDUygbBZAfXMeleRJl =  127; 
short HIoQkVUbtfeCqCuxdU = -8887; 
ulong kyoyRXqfFzcyCXVFI = 10345412086019963; 
ulong KuGsgDcxjZstXDVPIceQEA = 12146040172622884; 
ushort dmIMYgVUz = 34231; 
long oDidbqsPfYgkDkuUtAq = 41209346905268726; 
uint kscOlgw = 12; 
uint fIgegxiONaBYjacimyhSARI = 627399741; 
string pdYQmXEGVZkEzad = "gKtTNjYiHYLnnZkzcZTKOkm"; 
string ugibUKRxdBknkKfeQjo = "pqWPNCzht"; 
string xqmsdQgMnTSkzZkSpNFXaYPpdkun = "kKcdFPWXqcoJ"; 
ushort tzf = 29651; 
ulong JiuPcWzCXoTxTudjP = 83005236741073741; 
short XxSXNFLKbonaIqwZcj = 23938; 
ulong XjJY = 51639483190187991; 
ushort ffqfRhKbUCglUTVWB = 28691; 
uint ZmiYjaxWRGPnoPpU = 6545772; 
string tVohSqKxSnicnTCSWsWQ = "OkoNTWeDakywRfdXbgTqad"; 
uint NZwmyXsglXFLZw = 387995000; 
int SdMIyOzUVldALLJTaUCRfcTpiT = 373574; 
if(SdMIyOzUVldALLJTaUCRfcTpiT == 146709){
SdMIyOzUVldALLJTaUCRfcTpiT = 472556;
}long kALfCI = 11630418360389264; 
string wfTOOP = "wzFmeXod"; 
int EAjhUsTcEbMmyHIsRkcGYtx = 3025; 
if(EAjhUsTcEbMmyHIsRkcGYtx == 410551){
EAjhUsTcEbMmyHIsRkcGYtx = EAjhUsTcEbMmyHIsRkcGYtx + 698648;
}int czYJuSYBNZLkOyhbXGNfeb = 802673097; 
while(czYJuSYBNZLkOyhbXGNfeb == 802673097){
czYJuSYBNZLkOyhbXGNfeb += 857287;
}byte zsXoKPzYmBSLoG =  61; 
sbyte RhGZQXAIaAiypXfmqCtm =  -6; 
ulong DYsmmtMtDbwdL = 54932309143041088; 

}public void CXDNzYeTWj(){
string dbzLmnofGxVUwGVUYPSedny = "KBVOoKfkZWEuj"; 
long sujKecJq = 20697988269230829; 
ulong daMtCQWpKUzbBKnEAL = 18356946452232947; 
sbyte YTalORfKAZTtNPsOTREBHESn =  81; 
byte MZDpRMhsQ =  223; 
byte KFFeARBREZ =  64; 
short gFiwbNwDF = -7136; 
long KbIGxDUGETWbzsxpwIMMmdKgZWPg = 68615865587236089; 
float gysKJmJg = 7.557945E+12F; 
short hWJYK = 7360; 
short HANOOSOMgpDHykCn = -7858; 
int xzSafQNEhCWzk = 340553; 
if(xzSafQNEhCWzk == 703318){
xzSafQNEhCWzk = 581574;
}double zWZGgKHXwuFgkVcPAHXbnHkkS = -3.281671E-26; 
if(zWZGgKHXwuFgkVcPAHXbnHkkS == 8.312243E+28){
zWZGgKHXwuFgkVcPAHXbnHkkS = Math.Ceiling(Math.Tan(1));
try{
int POPBBpCRPBHXBWeIRVIUdTA = 6055919;
if(POPBBpCRPBHXBWeIRVIUdTA == 60337){
POPBBpCRPBHXBWeIRVIUdTA++;
}else{
POPBBpCRPBHXBWeIRVIUdTA--;
}
}catch(Exception ex){

}
}short pDRsGPSd = -4547; 
ulong UYI = 16845683086821145; 
int GSLSzxloqJOqJaabZJduLggh = 36; 
if(GSLSzxloqJOqJaabZJduLggh == 911001){
GSLSzxloqJOqJaabZJduLggh = GSLSzxloqJOqJaabZJduLggh + 857762;
}ushort WcNOeaGZDLEoTEGxUIzgnldzLDJfI = 34628; 
ushort nQuHnmXZddRaDRn = 39369; 
ulong QmuoKLYncCqA = 34364584177981827; 
sbyte KAckudWTndspMgeZfKIZ =  -77; 
ushort EMQKyQkxOjhEVlAqVsim = 16081; 
sbyte pjb =  -117; 
double qiynQzzEtzddIEgE = 2.917703E-26; 
qiynQzzEtzddIEgE = Math.Pow(double.MinValue, double.MaxValue);
 bool? eNbNDTfjFEUM = new bool?();
eNbNDTfjFEUM = true;double HjPWZRSQOUFzVQRBok = 1.733609E+15; 
if(HjPWZRSQOUFzVQRBok == 3.739602E+14){
HjPWZRSQOUFzVQRBok = Math.Ceiling(Math.Acos(0));
Console.ReadLine();
}float NYnPcGOEhjYpSEozgLOBayHAL = -2.043854E+32F; 
short pIwkqEoeluXhkkoJzeTfiDd = 19684; 
ushort psOdKJxRSCDeBaatUpRsQlqRm = 16470; 
string EOEGRBmWp = "KnMttOf"; 
ushort SeQIOYcaeaWRyGkmeX = 61731; 
byte NecMHEGxSEMTJkhOfQIOBiVuqmICg =  74; 
ulong fEeZNWh = 48204487408504177; 
ushort zYgo = 32659; 
uint jJutVepENDNefhbjJm = 9194; 
double itzmzIqKTGeBQeC = 0.008393697; 
while(itzmzIqKTGeBQeC == -7.774681E+12){
double oVyOzciEIMBbVJGIldKga = 8.375391E+11;
itzmzIqKTGeBQeC = Math.Round(oVyOzciEIMBbVJGIldKga);
int[] UOZInOBhPXDkDkRyBSeFB = { 8569427 , 68549 } ;
Random QMHxGYeIKFAfEniWuXZTfMdxJS= new Random();
Console.WriteLine(UOZInOBhPXDkDkRyBSeFB[QMHxGYeIKFAfEniWuXZTfMdxJS.Next(0,2)]);
}uint mthbYmaRhCcgmYLyIDnEyhF = 74969896; 

}public void JkOOg(){
ushort XsgXCp = 60881; 
double PzQYzeLlkpwXoeOCoU = 1.183523E+22; 
if(PzQYzeLlkpwXoeOCoU == -2.617578E-08){
double FAfmsHUsgQOGFBDDQZjI = 2.693799E+37;
PzQYzeLlkpwXoeOCoU = Math.Ceiling(FAfmsHUsgQOGFBDDQZjI);
Console.ReadKey();
}ushort dIPzgtQzOVUmAKLMgsSZEzAjAdUI = 14718; 
byte QbQFd =  183; 
ulong VLtROVwVCtgLHIutFiGYdAilDR = 32716730561910499; 
double jHMnehsoeB = -1.331143E-27; 
while(jHMnehsoeB == -1.028206E+37){
jHMnehsoeB = Math.Exp(2);
try{
int lfNUzKBNdtPxUWWEYgp = 3107335;
if(lfNUzKBNdtPxUWWEYgp == 97972){
lfNUzKBNdtPxUWWEYgp++;
}else{
lfNUzKBNdtPxUWWEYgp--;
Console.Write(lfNUzKBNdtPxUWWEYgp.ToString());
}
}catch(Exception ex){

}
}byte ylsfFHUZYjpjVdzdYdnfBQkfdgm =  125; 
short HFMlTkihxRUZVfqQqzyCalLguHbm = -30009; 
uint wKbEMyJknVRHy = 5168; 
float OnPOb = 5.164586E+33F; 
ushort GRliVfYxAoUCGtJniHjFnVTNWA = 51045; 
long tXdiSRRZfZLJaMlaEiU = 76441342114179560; 
double mwqnQgqQsNCRndHz = 1.44292E-18; 
if(mwqnQgqQsNCRndHz == 6.930668E+17){
mwqnQgqQsNCRndHz = Math.Exp(2);
Console.WriteLine(mwqnQgqQsNCRndHz.ToString());
}ulong bAVGEOXAWLAZ = 6875289764694426; 
ushort tnWDjMjW = 27064; 
ulong hyAFQwZnDiQEFtSWYFYwiWSgHJ = 23490115168850167; 
byte NqgVzoTbE =  176; 
short qQkqyuL = -18422; 
int XhuxcmhBUzgG = 64; 
while(XhuxcmhBUzgG == 64){
XhuxcmhBUzgG += 887590;
}string DXpRmweJSowbHutZCcmHTdQTc = "HEUuzQiPQaRDiHhAJFcfZQ"; 
byte CTIxXAhxOCOCPyR =  151; 
float EthYTGzuukQXMRowTFiMRpTuOI = 3.087219E+09F; 
long OzmglVpD = 360672378583027; 
ulong luFlxganAOjlNSLjxsLGGuepFgOo = 8792135435553006; 
int bffYwDtOpfSVBngjs = 289441526; 
while(bffYwDtOpfSVBngjs == 289441526){
bffYwDtOpfSVBngjs = 356198;
}ulong Dalt = 71621377814239039; 
long DWRRIypxARJDCasUNOuWTCguzJyqG = 41267547529039338; 
uint oMfBwLnWtlaERfLfbPxsZa = 832010784; 
int YSIoZdncek = 2201094; 
if(YSIoZdncek == 188018){
YSIoZdncek = YSIoZdncek + 740590;
}string GtLFZ = "LDufLnMoYgNxnelWpOaBsZd"; 
ulong mWUliEt = 76487334239637576; 
uint dFWNzYTJdXEAwLz = 94486791; 
uint ScPDuFuCOmhx = 5; 
float zRwOYaEfHPtySYbkTzHwnbDidxew = 13.10069F; 
ushort CPZQCKtRAJWFkW = 44918; 

}
} public class kXELxAJxomKCVhjfxWWdZlJi{public void PWFfY(){
double NpjXXJ = -2.952465E-36; 
if(NpjXXJ == 29.4067){
NpjXXJ = Math.Exp(2);
 bool? imTFfoQKggAkKE = new bool?();
imTFfoQKggAkKE = true;
}short lJwsfKuoUktNZ = 29102; 
string FxIgNDFLDxUkAtNjE = "THuqbyeV"; 
ulong KGAfiKwaxiHK = 3168734206205718; 
uint jOdxgnu = 4; 
float mbWXObPYMNwEjDLCsIVfnUB = -2.442543E+15F; 
string EgGlgQVRJTMbiaDMxOkTTwN = "DgMA"; 
short BlLnlRfBnOoMbIGGN = -27040; 
ulong dIBjUhu = 50465686724499254; 
uint VPLGVByFgtf = 925228; 
ushort NkySMCtdyDQmKSnUofoR = 18967; 
string hhdFOOogOHyJnKCPP = "MncfmOMTAQqHDsNJo"; 
long XGoixcGljZjOqBoDNzcU = 22923044096410690; 
sbyte FwRfSWyfGcICDTocagmPajlQtdL =  126; 
long VBgnwPWkJtfHSZmOpbZw = 75148550572346367; 
ulong lywYhguozBfQzUJtqlEFOJa = 88787389325382201; 
double UgJkUSftbDAoXDY = 4323.108; 
if(UgJkUSftbDAoXDY == 0.0004231122){
UgJkUSftbDAoXDY = Math.Ceiling(Math.Sinh(-5));
Console.WriteLine(UgJkUSftbDAoXDY.ToString());
}short VgiXPthClMdKjP = -28654; 
long QxHbXVqqzEuqhytGgpYIHSxWD = 39667853088118759; 
long BIuYkSGq = 1693283426923341; 
uint ZjXZK = 589707; 
int bxMcsxmSXMsOQbBHiqneQlYt = 9797; 
if(bxMcsxmSXMsOQbBHiqneQlYt == 517826){
bxMcsxmSXMsOQbBHiqneQlYt += 953023;
}double IcEUdLTIbSgqg = -2.210433E-24; 
if(IcEUdLTIbSgqg == -2.560956E-11){
double YPzsQtVaqaQXfNyG = 1.065829E-28;
IcEUdLTIbSgqg = Math.Ceiling(YPzsQtVaqaQXfNyG);
Console.ReadLine();
}float KJjRCYTcxZfeKsGYqqesOceFGaWi = 2.806353E-13F; 
uint noNdIZDLRtEKMbk = 9485; 
byte mYChIEGkCcKlfyIQuGDqqOtCwz =  102; 
float PKlEzoAMhfAcPbJggz = -2.556119E-32F; 
byte HuVCzFaNiiNHMszJHihemBdoD =  190; 
long oaiOtYdRJWQVL = 59053765703843311; 
uint dXgFgqGZMUHziaZmOoHZKsltS = 736567; 
double yuLRheRmtWBcX = -6.223083E-37; 
if(yuLRheRmtWBcX == 2.547931E+20){
yuLRheRmtWBcX = Math.Truncate(yuLRheRmtWBcX);
Console.ReadKey();
}uint zROiWgthmnaDcCOZ = 5254; 
ushort JgHcaDFOJNSFVHa = 52007; 
int AjxuWaMSfstLLZZZQskLiu = 614410; 
while(AjxuWaMSfstLLZZZQskLiu == 614410){
AjxuWaMSfstLLZZZQskLiu = 428696;
}long CYtzblcojGpiym = 28945330512222442; 

}public void UEfIQWxGpjKQuqlXNX(){
uint GwZXolA = 544444; 
float GTuAUWElsOZZYNxZIQWSXnA = 0.02949921F; 
int tFxZcAx = 92; 
while(tFxZcAx == 92){
tFxZcAx = tFxZcAx + 28125;
}byte iuGwajSlSHHZPquxaqNjIW =  195; 
double CJAZPzUIlfetdDYC = -37739.68; 
while(CJAZPzUIlfetdDYC == -0.1318077){
CJAZPzUIlfetdDYC = Math.Ceiling(Math.Sinh(-5));
int[] PJotwBaVVWwXyHJhTzKhdyif = { 6070235 , 41839 } ;
Random jLidCnCUmApPTliUE= new Random();
Console.WriteLine(PJotwBaVVWwXyHJhTzKhdyif[jLidCnCUmApPTliUE.Next(0,2)]);
}float MfIwsQ = -2.476375E+21F; 
long BiTkQRRtJUc = 5626630653058495; 
uint NouzFBIiyhfEhPGabscNlNXcC = 299886750; 
string ESBjAOUzMkqj = "kMXGZEOY"; 
byte lNGZqqX =  160; 
byte QUpFuJbLCohMEbDlfWZ =  188; 
byte tJoxTOEhPEfEu =  82; 
ushort lpCcyVbAjFyf = 14405; 
string zqwCIpYQmhuBEHZHlQOK = "kkXyIPlnqyWOWHlYPOcmA"; 
short cwyTkXlpypYngzeZBMDVKzNHaNI = -5066; 
string yiVpUbjWbpfhzyXsfBLODC = "tkFJQTyoQnNTQsQFcPEKlQDZoKII"; 
sbyte niVkDfotudFapULEhZoVJY =  121; 
float EIGtgkPheZFYFQLpJwBfkB = 4.524928E-18F; 
long IslbojdAYyKZnSPTk = 71035872640971578; 
int MCdZqIFDyWdLwUf = 267612126; 
if(MCdZqIFDyWdLwUf == 139907){
MCdZqIFDyWdLwUf = MCdZqIFDyWdLwUf + 655601;
}short oOhgYzOGcLLs = 29792; 
double tsJosXhiMLzspap = -3.273914E+23; 
while(tsJosXhiMLzspap == -3.685191E+20){
tsJosXhiMLzspap = Math.Ceiling(Math.Sin(2));
Console.ReadLine();
}ulong wtl = 64138241015052935; 
double GxMWnyXSzmFWHVJAD = -3.148854E-28; 
if(GxMWnyXSzmFWHVJAD == 1.140877E-23){
GxMWnyXSzmFWHVJAD = Math.Sqrt(4);
try{
int QhXgInecjmOgtWVwcTZcjgqdU = 6237684;
if(QhXgInecjmOgtWVwcTZcjgqdU == 22252){
QhXgInecjmOgtWVwcTZcjgqdU++;
}else{
QhXgInecjmOgtWVwcTZcjgqdU--;
Console.Write(QhXgInecjmOgtWVwcTZcjgqdU.ToString());
}
}catch(Exception ex){

}
}long sEFTBRIKRTmgNMf = 2776140736068227; 
ulong qWYAdyJQcatxTDGIDHta = 18437905688903880; 
long UxQliUKjNxFpKMMLHjewhRO = 54486481676138648; 
sbyte nfWHItL =  117; 
short XwPlJuOgLwlHcIaJOtSjxUxgxG = 19705; 
string QuyQXRtNiTyljRMWIcoHZFsfhydZJ = "dFKhVbPQfRHVGAuXlcLjPDBy"; 
double bsIBOuetP = 3071.438; 
if(bsIBOuetP == 3.838863E+15){
bsIBOuetP = Math.Sqrt(4);
Console.Write(bsIBOuetP.ToString());
}sbyte PbUnycVa =  64; 
byte QQMopglTwLfoTo =  84; 
short JNKAbyJoT = 22606; 
sbyte uQDJQVUWddG =  -128; 

}public void MzJMaYfNIqaCWOtdaxodEbVDGc(){
ushort uiGhKGpYZDTTbD = 47832; 
uint hUgBSdHpWqLEggXEJ = 4774; 
long SqzwezEwcaoxayinWCfYROoFuS = 75456878347282393; 
short JfOFdUUpwhgJOpMSs = 17018; 
string cTal = "HgJR"; 
string qhHGRNCUWUYjmMMiZHqUZ = "lBPeDCxOkAtzPCgLVeRT"; 
sbyte eKOAqzh =  -116; 
uint KKVgQNk = 471431764; 
sbyte zRPWkbnLmcgCcwfqckXHh =  -126; 
byte fGaUiUSuWAGSYEgyTqleDhSL =  182; 
ushort PCCCyGZ = 53542; 
int uDt = 7433; 
if(uDt == 796137){
uDt = 243535;
}short dAOROflYNpiNMdTw = 30808; 
sbyte YNDxAOTUQ =  13; 
short zdJLbOLKBOSRLkQHhTjFHatMCdn = -29429; 
float xwzuLJdtS = 9.17493E-19F; 
uint kULMnYmNWtLnzLWiXAIPoq = 366174; 
short hDPbEphnhRRqaeIA = -89; 
string flYgjUgGKyUyswOQKYVQugk = "esHAEhPRuGknjxFgx"; 
long QnuzWlnXBFmtZbw = 82870460039039430; 
string jAHntOtKIuoLTKmojomhml = "OuMTPIWhk"; 
short NNHKhRTJqHgqZEuOstQUAXm = 24114; 
float UFcMmdtfXBIBgwoEUGHd = 2.106897E-16F; 
ushort EOwynPAOQBWnHGudYTGTgSluGV = 4271; 
long QVGkihzJW = 2789934667218623; 
double kEfSJBNMCa = 4.71967E-12; 
double yzWwbb = -3.26373E+07;
kEfSJBNMCa = Math.Ceiling(yzWwbb);
try{
Console.WriteLine(kEfSJBNMCa.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}float DuubcZNWCaK = 0.2069582F; 
int JzYgAHkKbWSZDUfBMzsAOD = 765117; 
while(JzYgAHkKbWSZDUfBMzsAOD == 765117){
JzYgAHkKbWSZDUfBMzsAOD = 301872;
}byte EdtuCVYptKqLsekakcwAXygdXgakR =  20; 
uint zcsOHhskDxeJidjuEokeUMagXox = 904571; 
ulong ojwloMxucKkWCtz = 53734562346582316; 
string clRqMXoPYPzNRVJgUhuJQA = "ztDzogVxPuBmAGcqNVhDFai"; 
string XBI = "asWDzsXizaJcsJtqIiCSRI"; 
ulong WizkByqGmLPbEKamfhnV = 11010191509326694; 
double UsFnGpTMIbKcPITqOXhnMJuS = -9.93021E-06; 
UsFnGpTMIbKcPITqOXhnMJuS = Math.Ceiling(Math.Sinh(-5));
Console.WriteLine(UsFnGpTMIbKcPITqOXhnMJuS.ToString());
}public void bQesyccuYplAZlVkmZhlpC(){
float MtFHtufNfHEKGSxmFI = 3.687855E+36F; 
float tChtlDaFwBXBHezxb = 1.070006E-24F; 
uint sIkGuDUIJSTNk = 6; 
uint iDMCldNelsPhGePqgbTWORafWMIdO = 80443; 
ushort bwHOSfFzBijfAcOewdYjDlqDHFAx = 44245; 
short nujFJhEoVnymJQaxJTZssfkwP = -27979; 
string nGiPks = "LpzeWSPdVeZJNGiRDKYzijPPGdx"; 
string fIcpUogkPc = "EbopQuqCePGflTNC"; 
uint ATxOOPxXsSpbGOIzVgUzZ = 7378; 
float BBDQyhOGCpjdPnBTScAkdiSyOXG = 1.2903E-35F; 
long gBSnoUhVLD = 19788863619589905; 
int imKTqJ = 5; 
while(imKTqJ == 5){
imKTqJ = 552724;
}float cDXZ = -1.199146E+37F; 
long PJHfuZ = 52681285688725821; 
float XGPewhuUlwwRiTiGxbzallnfJIQUf = -9.656763E+32F; 
byte YcFkGLHnMTYFCJTdRfduQN =  182; 
short xOsBOVAlfjQEwRqSNBsoC = -24004; 
double QokTPWksmCjXoOYYAsxAcBwoNDdm = -6.469021E-36; 
if(QokTPWksmCjXoOYYAsxAcBwoNDdm == -1.26824E+23){
QokTPWksmCjXoOYYAsxAcBwoNDdm = Math.Ceiling(Math.Asin(0.2));
int[] ktUgTezmUVmBFjBItZeLYlZFqBtGP = { 1268130 , 36624 } ;
Random kqaAfBMmKUcgWmyZp= new Random();
Console.WriteLine(ktUgTezmUVmBFjBItZeLYlZFqBtGP[kqaAfBMmKUcgWmyZp.Next(0,2)]);
}byte mOnZYupQtLTzuLWAedZWfoj =  165; 
float NhqkmlBLXDN = -5.911847E-30F; 
sbyte xqjjICCBAlctsI =  116; 
int hIHizizIYJOtNFWTAJY = 465556823; 
while(hIHizizIYJOtNFWTAJY == 465556823){
hIHizizIYJOtNFWTAJY = hIHizizIYJOtNFWTAJY + 625821;
}float jTidZYgsAAnYhMDKSOhMHfBSYZFhX = 1.412284E-06F; 
uint jnOPGPwHmytlLQnWBWy = 128827; 
short tqeat = 31117; 
ushort dMsYyOUlZMeuJhGniql = 29889; 
double cARjwzCPcISRoBtSQhGKzNbYukLRR = -8.467796E+18; 
cARjwzCPcISRoBtSQhGKzNbYukLRR = Math.Pow(5, 2);
long ntHXoKmllSxmRhOyzmSKF = 17050743298339502; 
ushort SRKqE = 57529; 
int ujthDRksBmQjo = 1616139; 
while(ujthDRksBmQjo == 1616139){
ujthDRksBmQjo = ujthDRksBmQjo + 189708;
}float inkROFxgRkEUTyBkpEGUn = 1.411968E+17F; 
byte PeQFApKM =  179; 
byte iqOmPkJclpwoBUBKFXwjDZGYAGC =  86; 
float hilFcZPsfcpJZVuQpLZGePobsi = 6.207736E+22F; 
ulong Jqhshmc = 61357200061224609; 

}public void aLFKpRsoPtTklToZGWuYBHNgOu(){
float LTXWhziZkdTSZTE = 1.109079E-09F; 
float XlVHVYUttYzWVXTEVwWcO = 9.646857E-17F; 
ulong dCZIdXNVLVmQmbYnf = 28403669792708047; 
long yPVmWgdSxULpb = 56337555829488289; 
double IWnbwfIUswEogPJpJijCtwza = -1.926123E-28; 
IWnbwfIUswEogPJpJijCtwza = Math.Pow(double.MinValue, double.MaxValue);
Console.Write(IWnbwfIUswEogPJpJijCtwza.ToString());ulong FbSw = 65433607425079315; 
int VZKCZMLdkULwbbonuXNNCAB = 4776; 
while(VZKCZMLdkULwbbonuXNNCAB == 4776){
VZKCZMLdkULwbbonuXNNCAB += 749965;
}uint JYAxA = 903854; 
uint sBQYK = 37542854; 
short qiCgTlwAzcUHZqLPcyCGPR = -1398; 
ushort waJDCQMNZxqECemaaYSG = 30300; 
ushort eiIkRMXA = 44969; 
string ylMWXmhUZUulCtoF = "THSjcQFtTFUabWHAbHaodaG"; 
ushort toLXgSDgjHclowwhDd = 42837; 
byte XYNNAMnpKSbwGJBTxNNsUSoEmN =  56; 
int CXXjMw = 9460; 
while(CXXjMw == 9460){
CXXjMw = 277970;
}float PbRJMUngqVWZfJiA = 7.651012F; 
long iVdkUmj = 36553873897552574; 
uint dALSwGcnMpJOLU = 612389; 
long UYGPALsUknS = 10018592816131223; 
short obXhWCiBkbywbHEPoaZBBuO = -6219; 
ulong BgHlEIpEFXIHDjital = 78287472810801767; 
double mUmDqtabUZzPeybHgIzPqguD = -4.427694E-16; 
mUmDqtabUZzPeybHgIzPqguD = Math.Ceiling(Math.Sinh(-5));
 bool? cCbXChVfFhqXbsoHjH = new bool?();
cCbXChVfFhqXbsoHjH = true;long kYUSAR = 60908186514180500; 
uint WhiXCo = 405; 
float EXgXzeIAYJzVVcysieL = 6.624847E+35F; 
ulong dcweKGVISXUmiaCIoGdpR = 49286089537106058; 
short HSmdVda = 29897; 
float aHXa = 1.653244E-17F; 
short ywhVwTwbhYkwPxBPnwWW = 7131; 
int IjmQbdmqkS = 757057; 
if(IjmQbdmqkS == 716369){
IjmQbdmqkS += 431192;
}ushort UVcNznEfhMuswbUhBMidt = 62499; 
double ZEpKFPDYDnLJlULYlVVB = 2.131293E+27; 
while(ZEpKFPDYDnLJlULYlVVB == -4.832766E+27){
double poXZwOUYUBt = 2.197103;
poXZwOUYUBt = Math.Sqrt(3);
ZEpKFPDYDnLJlULYlVVB = poXZwOUYUBt;
object qkVGajRz;
qkVGajRz = -3.132205E-07;
Console.WriteLine(qkVGajRz.ToString().ToLower());
}int mYOJQgRasuCsBcofISUZdz = 894809656; 
if(mYOJQgRasuCsBcofISUZdz == 626539){
mYOJQgRasuCsBcofISUZdz = mYOJQgRasuCsBcofISUZdz + 759833;
}string KeGLD = "DDfzjjA"; 

}
} public class Bcje{public void NhYuEHOgTdfBdVmKyMQAgEfNw(){
ulong ZOuDADWqWMKbdPbJAZYfnltcpu = 24805348139855515; 
ulong ACGYgFqYOLNLf = 58471727371297877; 
short fXlgTCLmp = 32120; 
long YtaYKVw = 74077885541247153; 
uint gcZho = 5472; 
double yycyxMyLIwxLWRZ = -9.558504E+08; 
while(yycyxMyLIwxLWRZ == -3.603894E+09){
yycyxMyLIwxLWRZ = Math.Pow(double.NegativeInfinity, 2);
int[] hkhhFPaftk = { 2949597 , 25188 } ;
Random aeJbWoGasT= new Random();
Console.WriteLine(hkhhFPaftk[aeJbWoGasT.Next(0,2)]);
}ulong KYDxIKtbRzmWAmHddPYMJWZR = 58983075008518729; 
ushort URzKMBSYnkPZ = 52994; 
short oPBnbgNYMsTd = 26051; 
ushort GoDgsQGuczUKIPb = 5532; 
double fpDsKqkhSM = -7.797439E+26; 
while(fpDsKqkhSM == 57.1292){
fpDsKqkhSM = Math.Ceiling(Math.Asin(0.2));
try{
Console.WriteLine(fpDsKqkhSM.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}byte NtMahipSYmwPPXSJ =  102; 
uint XRPHfZBQHUzpgVUg = 163947978; 
byte GoXuatuAoiAkeZoJYNG =  4; 
int JXAWnUUQUAjW = 668537068; 
if(JXAWnUUQUAjW == 619937){
JXAWnUUQUAjW = 757897;
}int Bzw = 2305; 
while(Bzw == 2305){
Bzw = 171150;
}byte kYmGZVdeBFAoZbheHP =  147; 
sbyte wGSkDmLgnJIKWZYIcUADJeybi =  -24; 
byte cZxFd =  182; 
ulong uwyJtYlVRGcc = 39361936031039079; 
sbyte bhbHeomcGljGnHyRjKTDjEsiU =  24; 
ushort BACzkDuoHMUWIPaR = 56103; 
int WznuAyG = 46278360; 
if(WznuAyG == 736901){
WznuAyG = WznuAyG + 483405;
}float DfKMuLmhjyQWUuECppxAcme = -8.51825E-38F; 
int VBPZwdVmUhgjzJqdXJCMmUnDxdu = 30201051; 
while(VBPZwdVmUhgjzJqdXJCMmUnDxdu == 30201051){
VBPZwdVmUhgjzJqdXJCMmUnDxdu = 809663;
}ulong IuywSYhJtjfX = 50088673075592669; 
string mEqKkXTBw = "JqJJBnJNflUsp"; 
sbyte YhTnlXgpwhTdiOjQTO =  59; 
byte cAIfQfDQqUmlflQ =  1; 
byte xDamCHj =  127; 
double wbEFSeWIlhwSEBenHScCuPHFpMy = 9.593404E+34; 
if(wbEFSeWIlhwSEBenHScCuPHFpMy == 2.425561E-14){
wbEFSeWIlhwSEBenHScCuPHFpMy = Math.Ceiling(Math.Acos(0));
int[] yyCHuZnfHhKkl = { 312950 , 23074 } ;
Random MWgOXXz= new Random();
Console.WriteLine(yyCHuZnfHhKkl[MWgOXXz.Next(0,2)]);
}byte DRM =  195; 
double LbFTJgNWqunuhyESJTb = -8.162465E-09; 
if(LbFTJgNWqunuhyESJTb == -9.179606E+27){
double RhDsKCZiHkcQtRHDkIdP = -7.163569E-23;
LbFTJgNWqunuhyESJTb = Math.Floor(RhDsKCZiHkcQtRHDkIdP);
Console.Write(LbFTJgNWqunuhyESJTb.ToString());
}short AKc = 1350; 
int MBdKpsZZiiqGqSlpLlmO = 202222142; 
if(MBdKpsZZiiqGqSlpLlmO == 818066){
MBdKpsZZiiqGqSlpLlmO = 706306;
}
}public void gHqHezI(){
float SdnGlMpCXqfzPjYRnSQC = 8.583398E+12F; 
float jStRDmgJaEzIXHCeVly = -241477.2F; 
long ysQQHSqVijJx = 85857649467448263; 
byte qTHpLHBluaBBVAMxBQJJoy =  70; 
byte MxmMogk =  33; 
ulong Wjq = 88335364346170329; 
uint moKIQDxUFwhZjmAeUptjAQliKZmsI = 30511048; 
short zZyGAwEogq = 17787; 
sbyte AhyLOF =  0; 
short ycBXQpelYfTMMncA = -6200; 
float UszzZDAfUXSLOYZDweCFnkUs = 1.151777E-15F; 
ushort UuksKLwgYEBwpjjofweNNLI = 26904; 
float BJzfQKhMkBM = -1.855165E-40F; 
string SFREgndaGmmkIaiNPA = "jMISORmFZKDGu"; 
string UgQSNcx = "TTDzBLbNXaHTSjMdMTKe"; 
short HTjz = -24474; 
string XMqNdPYlsNTXWGGNhqKuNfTssqQm = "nbFZRZnpfOwNStgRhwybf"; 
string xIiSTZ = "NZMCeLnXfGqycD"; 
short VMNBkWslEmUhsIgwpKbw = -30839; 
short wDjoRKIZEha = -15722; 
sbyte zkRIAY =  86; 
ulong GyzVgs = 73734518930009293; 
int CVmqaquaoSAjOCqy = 437946; 
if(CVmqaquaoSAjOCqy == 357503){
CVmqaquaoSAjOCqy = CVmqaquaoSAjOCqy + 783412;
}uint cnuIKPBkkgYcFAfndzHo = 337926412; 
sbyte nYMCx =  71; 
sbyte uGnMXNpAqoZIEoTeqoOUhlP =  52; 
sbyte tMdZOxYkIZl =  -33; 
short hgXFszBZfHj = -23827; 
string FiREEHtFxs = "EJsHUTXYyManjtLQ"; 
ushort xPHwLHAhIQLXELGdOPh = 24733; 
byte miDCEVHkTRaYaEUhYMgMlhm =  131; 
sbyte JgFWIZwJDyT =  -83; 
string CyXTWFLbjGQxlACeVb = "nmBHXjbDqbxo"; 
ulong EjPktFEFHWYfkNVhyDSRO = 11278580473736705; 
string qaNJpMUCyOJGEIRYnIjEUyY = "lhYkKJCkHiHezdGwEauceLziiMH"; 

}public void jxUUcRxhOJSOl(){
uint ubMAjcOaTcVQCKQcXbhbjoMcHHyIU = 60; 
float LdhxlysxwaSgaVyXHVNe = -9.687364E+21F; 
sbyte KTDFggMyei =  77; 
int GMFQGPnUVHMZKmmTMkWY = 729762444; 
while(GMFQGPnUVHMZKmmTMkWY == 729762444){
GMFQGPnUVHMZKmmTMkWY = GMFQGPnUVHMZKmmTMkWY + 292101;
}short wcSfSoaiio = 21943; 
string wPmRFyUAEtYUIQUtKcpBazSOH = "uldLxtV"; 
sbyte WgdhSuPRKTkkAgQ =  -11; 
byte ARWWzkycIwTWGYBtX =  62; 
long yglIiVwXVfnmjSxpCaISLPNtXNn = 10266913404729764; 
ushort nNDPAgDhHMNDjSXFOjI = 24861; 
float xHhmPWLwSOcLmmfckJklq = -15.38362F; 
long nlYchwZNEqWImqGx = 80801705932092181; 
long ZiHtwgyNCKMtKtdYJViwYqFPfgCka = 58415549055433373; 
byte QjAQWKSjGxCWPppaJFRnnFlMSimc =  132; 
double RzZtlABIf = -9.825801E+33; 
RzZtlABIf = Math.Floor(5.722747E+17);
int[] XOyPJIiKzQWeCiWLaqJymfx = { 3225086 , 15753 } ;
Random bslcDB= new Random();
Console.WriteLine(XOyPJIiKzQWeCiWLaqJymfx[bslcDB.Next(0,2)]);byte DUJzonGQoWmXHEnEXqDBBS =  84; 
ushort pxggXAOQPGccCbwRxRBQeMI = 50140; 
double ZGOlLEoKYRAJqxzgRX = -68.27153; 
while(ZGOlLEoKYRAJqxzgRX == -4.045356E-29){
ZGOlLEoKYRAJqxzgRX = Math.Pow(double.MinValue, double.MaxValue);
Console.Write(ZGOlLEoKYRAJqxzgRX.ToString());
}byte cRJBlmBgbTIIOKGyWORgYA =  70; 
long oPPfHgSQbLMDbedcHjSbxcbFtNiEg = 57405445516448259; 
string MPRNYTUUKOdsp = "JDlNjNUYnKnhiF"; 
uint GpFPgl = 306341; 
string zSzQcwmgoZLLCoBOChARKp = "DiONBoxXVKAnc"; 
uint ITUcoBxUPpnsLyQ = 163; 
sbyte WInINSScQbcazne =  2; 
int IaUgoZxmWMBfzzfFDQtkhkyyHSMp = 81346094; 
if(IaUgoZxmWMBfzzfFDQtkhkyyHSMp == 186700){
IaUgoZxmWMBfzzfFDQtkhkyyHSMp = IaUgoZxmWMBfzzfFDQtkhkyyHSMp + 287622;
}short EQtokAV = -24705; 
int EFVUMohwPyEyAxmgxSgEV = 11; 
if(EFVUMohwPyEyAxmgxSgEV == 741907){
EFVUMohwPyEyAxmgxSgEV += 443742;
}string Iop = "WohuVtqPOBVFgpWcddcaZPuzYTY"; 
float wghmD = 1.086152E-28F; 
sbyte CAPPhJjqUxBbWFmpVhUJwbu =  -3; 
long tNsENPXpCDkj = 42093671046250272; 
uint VWHxmxsAfoePpxZqJ = 8092429; 
int KfMB = 38; 
if(KfMB == 825152){
KfMB += 166615;
}long RSxWqDBYcyVfSwSIIUGYTAkRVV = 35696274177990042; 

}public void FDusjEIamjHJOCfw(){
double JaqHGbxAzuHhPJWWnf = -930.5198; 
while(JaqHGbxAzuHhPJWWnf == -1.932483E-23){
JaqHGbxAzuHhPJWWnf = Math.Ceiling(Math.Tan(1));
 bool? nuewYMS = new bool?();
nuewYMS = true;
}double QlVcPoiVHLqXeTlqBxl = -4.714416E+28; 
double GMBfsOojdKdYSkOxXSOPNxJONyLSQ = -2.292014E-17;
QlVcPoiVHLqXeTlqBxl = Math.Floor(GMBfsOojdKdYSkOxXSOPNxJONyLSQ);
object kpUkkGpEcnQsmHJiZDlBBkjdNtUcx;
kpUkkGpEcnQsmHJiZDlBBkjdNtUcx = -1.537436E+34;uint cJQTMZAZgRzRLmhJGePnZOoOQ = 56265163; 
short bkqUcaNEPaFAIOcN = 27378; 
uint dcDOHkSn = 435; 
string WjAeHgWJmueQdkPsZlbUxSl = "zxLKkMtCMnIphJx"; 
int TXKXMLxxAnhbipdXbGjxmyBaEeQDz = 32; 
if(TXKXMLxxAnhbipdXbGjxmyBaEeQDz == 901770){
TXKXMLxxAnhbipdXbGjxmyBaEeQDz = 562731;
}float BNMomLylBDXEsgcmj = 4.363586E-07F; 
ushort tIUTXpYqGnNiwBTzgaNcP = 4584; 
uint YgdmElihzThfRxowAUUojwWHjP = 72; 
long UpfbURlLOteGK = 19232543064406195; 
float lJEjydO = -7.68416E-05F; 
uint YSZPC = 166995; 
ulong BjFLtn = 34344146381029009; 
float pHPHajIgHFBO = 5.450474E-25F; 
byte OOJceSR =  47; 
int SxhmXMfxkSthfqCyeON = 32618607; 
while(SxhmXMfxkSthfqCyeON == 32618607){
SxhmXMfxkSthfqCyeON = SxhmXMfxkSthfqCyeON + 68225;
}byte RnyPGauqlZuIPIu =  247; 
ushort GJqBHOcxYoLHaiMmoiNcOhMZ = 20563; 
uint gXaWiwsyEgpIKNibeHFZkV = 367086; 
byte qbzWsjid =  234; 
uint sDlILigYCdpGfItMAAkxILXdqZHO = 47; 
int KeMdsqANwwLhbwsoZzKELcpIF = 32; 
if(KeMdsqANwwLhbwsoZzKELcpIF == 354542){
KeMdsqANwwLhbwsoZzKELcpIF = 840927;
}string AVbVothazamaPcV = "qPIPcfHOdAjaOkcEILDA"; 
short KeCDlcdHPgwdk = 4528; 
byte utonRQjhl =  65; 
double teBxsXeDnAUFLyKdFLaAt = -7.60823E-16; 
if(teBxsXeDnAUFLyKdFLaAt == -1.407831E-35){
teBxsXeDnAUFLyKdFLaAt = Math.Truncate(teBxsXeDnAUFLyKdFLaAt);
object ffDCmzpYPqkoxj;
ffDCmzpYPqkoxj = -4.466361E+13;
}ulong dpVoOxRoZfMEjklgJXD = 64324587352813188; 
short SduiUnSehRDXLO = -12937; 
long qoVIgU = 60071519680437004; 
int TgcqwoWtcBfaCNPnL = 371548; 
if(TgcqwoWtcBfaCNPnL == 762206){
TgcqwoWtcBfaCNPnL = 718580;
}sbyte WNOfDzN =  109; 
ushort HnqMbsDGGOlLuLARiLgifwTiz = 34955; 
sbyte AbsdgViZtP =  -44; 
sbyte hfwXktcHMpcQlMluimBingEwh =  17; 

}public void DAyutYsYpylnigH(){
ulong pmVjWFkCRYDbCZPIVAhoWJ = 81256742824200484; 
string sSeUInGUlSDZQWPyyHmH = "APHwcxbInDAWUJJU"; 
float HUmcegSdRgAAsewhTczzGSFlYoddV = -7.633603E-31F; 
int HGNlxLAEYFUiPjuBN = 61666540; 
while(HGNlxLAEYFUiPjuBN == 61666540){
HGNlxLAEYFUiPjuBN = 778599;
}short dPd = 18708; 
sbyte bWXMzJWxsYVHnKdjeJ =  10; 
sbyte AggmSyktWxPQaTl =  85; 
uint SQsnzBNBGYUKwmZfCQQzHkMdwDg = 268449335; 
float sGNKsfkJHBqSmzlbDRUswbaoI = -1.134766E-17F; 
float uWlPfDeIDIaphiXpsAGjCEZuzxp = 0.003075536F; 
string RsNmEedgV = "tZiUbm"; 
float ZoGJ = 2.539896E-09F; 
ulong SpKlAiCuiXwWukotLchFRByizCAM = 45619958156784463; 
ushort IjzOLdSWJOamXHlkgsSBO = 62170; 
long NanczNYCWbuXgcPupCTLMwkd = 81062452402516529; 
uint bLPPyVQApWloddEzjleYc = 7878; 
byte mneGagKxybeFCeXVmeoRRiwpyLJG =  140; 
int VFSkILTpPCSHczTJhgcQn = 370612; 
while(VFSkILTpPCSHczTJhgcQn == 370612){
VFSkILTpPCSHczTJhgcQn += 771603;
}sbyte zdURsiaJUo =  -78; 
ushort ApYCtSFcBAAQwwkNgQXLaqzY = 52028; 
long SanSSyAadhD = 84954837038115112; 
double PWNAmJOhSaNIDMbmHItWwhG = 2.305254E-15; 
if(PWNAmJOhSaNIDMbmHItWwhG == -1.107294E+20){
PWNAmJOhSaNIDMbmHItWwhG = Math.Ceiling(Math.Atan(-5));
Console.Write(PWNAmJOhSaNIDMbmHItWwhG.ToString());
}sbyte LRXujoYJL =  -114; 
ulong MSuKGnzecFRfQmiwfJsBxJNXD = 2576362519103229; 
ushort VWANYmajgZRFWfDDbLnuc = 7387; 
ushort tkksQy = 50316; 
sbyte EmzGVtmxzGXLIg =  -95; 
sbyte ftLZHPAsuObIuKlLhtTeW =  -101; 
short XEB = -99; 
ushort dejfqwwyXyZmCGkOeVMaBO = 17521; 
string abKbcoZWKKXzosRoLwweKEM = "jAFOxkpQfsqtMwViZqSVnoQD"; 
float QsSZexoTRDLkBzugtXbyE = -3.344328F; 
byte sMQDU =  175; 
string MwXXfnVdSEsyPRtBgWTCzgBSGU = "KAqWTKsPdyjmJgtLLiPqCt"; 
short YRAwFUfslCkFCI = -1701; 

}
} public class xbodXKjwfwPKQEjqiVYcjWj{public void sSahVOj(){
string bUAdlAoIepjewCJqUxHJx = "BSfMeJbBqlMZPJLmXw"; 
long fHaeWpqQbsEum = 18767364088702364; 
int XtwMZHHAkZbzTRbPPqlzZJsMqpcpx = 6; 
if(XtwMZHHAkZbzTRbPPqlzZJsMqpcpx == 655711){
XtwMZHHAkZbzTRbPPqlzZJsMqpcpx = 53896;
}long EsiuSsulYAQWLFou = 64434593910117375; 
short iWlX = -18194; 
ulong okZcegpXBsQ = 25479353286992331; 
ulong RiIFdQjyZbnbGxupqaRYd = 84945272990300952; 
float DbHybqzaqgABqClOo = -0.003529909F; 
string SPGIgNngzyoiB = "aYYsYTaBSFJLtficKLNfKl"; 
string UEkUSKksQPlQpKAkusWbZwuh = "wOFwucOXjyuIgIitpVFUtNZkCMspZ"; 
ulong jcVyVHXwNySglfDn = 80049731641276983; 
long tTJKzYtYjGkkmNtASxBHOfY = 35786723862448785; 
short hmdMcZUNmVXcABVpaXSebanV = -18660; 
string HmZQRUwfBpNPxHUBjbwibxRYxWl = "hYhCbdmBmefidwwZmixygAd"; 
float SVkxbtVJIXChlOstumfVmeGa = 1.858482E+17F; 
byte OHIUnLiicelMjBxy =  222; 
float wde = 5.370925E-20F; 
double EOojFofxUapITCGiqpatMNJhhYLT = -1.856093E+14; 
EOojFofxUapITCGiqpatMNJhhYLT = Math.Ceiling(Math.Cos(2));
try{
int DujozQdOXglaValdBXyhqep = 6021506;
if(DujozQdOXglaValdBXyhqep == 60629){
DujozQdOXglaValdBXyhqep++;
}else{
DujozQdOXglaValdBXyhqep--;
Console.Write(DujozQdOXglaValdBXyhqep.ToString());
}
}catch(Exception ex){

}short iRnLThgHsiAmJNwzdVE = 9271; 
double ujLWOwEfzVIQZAQGxFKlaj = -1.827274E-25; 
while(ujLWOwEfzVIQZAQGxFKlaj == -2.372902E-32){
double IJzye = Math.IEEERemainder(3, 4);
ujLWOwEfzVIQZAQGxFKlaj = IJzye;
try{

}catch(Exception ex){

}
}uint YjOzKEOK = 31; 
int dLWOmizHd = 427139; 
while(dLWOmizHd == 427139){
dLWOmizHd = dLWOmizHd + 600882;
}long TWSYOYDWLk = 18376802996744377; 
short NPDgUhEN = 6440; 
uint YcxpuNnpMuRDYCdyofKxVpABetLeG = 782900; 
short lYAiQWMn = 416; 
int WlFFQKoIVedJEWlaciKqGAsME = 278854; 
while(WlFFQKoIVedJEWlaciKqGAsME == 278854){
WlFFQKoIVedJEWlaciKqGAsME += 160890;
}ushort JFSPfoVggxehoWmEWKeQmk = 28529; 
float bkwQzYWJWUsukWUPjzdnFnJbV = -5.330128E-20F; 
string AzhYXddAqJyizsbVjL = "jbcMtGmNCuUkLSeyxjxnSRo"; 
float GfgWIgLDAUDyMOsKqCfxzMpt = 1.119204E+08F; 
long yVtNjWNBgRUnjJlYCZKwdFC = 84451018594984331; 
string XfFCQnPbRDBHRm = "BqRWVoKStdDNFTMuRSSlEHyM"; 
ulong umbbIsjDtzVeggmmkymCjhfiJOZ = 11751007237326767; 
ushort swkDcAmANMMYqxazkLlhTsTti = 20699; 

}public void FQUgkRwSbcYhJSJW(){
string cVxTHZoJo = "aCStMYIKSFBhVEYxlEEojwRfgwdM"; 
sbyte SDCySBkFwKjZezMUZSVjNLfZOFL =  59; 
byte UWBWMYHzMzl =  162; 
sbyte ejqstmxkRYigxCJmPzteQnAhMO =  -59; 
uint yYfDmV = 264448; 
short ZUJfCOFCePzJ = 17534; 
short dRLiG = 6629; 
double euacFjHeZxftjgiXtgljMJOjRzi = -2.655904E+29; 
if(euacFjHeZxftjgiXtgljMJOjRzi == 7.003325E+19){
euacFjHeZxftjgiXtgljMJOjRzi = Math.Ceiling(Math.Atan(-5));
object VKqskCxyJg;
VKqskCxyJg = -3.654048E-38;
}byte gsAdhleooCO =  32; 
string yXfutpNikDPCHZksURj = "hUVhIXbujtDPxXfuypcVDYgWn"; 
int eeWIXXlzDbXMxotYSZa = 66; 
if(eeWIXXlzDbXMxotYSZa == 906568){
eeWIXXlzDbXMxotYSZa = eeWIXXlzDbXMxotYSZa + 581832;
}ulong ojOZTbqWpn = 35389087016024592; 
int HjZoWnCWknbdiJWnOcd = 8715274; 
while(HjZoWnCWknbdiJWnOcd == 8715274){
HjZoWnCWknbdiJWnOcd += 137458;
}ushort Ujqzwah = 41653; 
sbyte zcGBuLbl =  35; 
ushort okPpwTC = 18479; 
sbyte hDfHNUQHca =  -118; 
ulong pZzj = 61887830863953389; 
double hZhHsZWqBAUIRlGFx = -0.02289551; 
if(hZhHsZWqBAUIRlGFx == -1.286701E-15){
hZhHsZWqBAUIRlGFx = Math.Exp(2);
try{
int SgRdRczOLUgJUsTTz = 2950188;
if(SgRdRczOLUgJUsTTz == 80986){
SgRdRczOLUgJUsTTz++;
}else{
SgRdRczOLUgJUsTTz--;
Console.Write(SgRdRczOLUgJUsTTz.ToString());
}
}catch(Exception ex){

}
}int GZVapFIxislUky = 4628; 
if(GZVapFIxislUky == 998046){
GZVapFIxislUky = 86999;
}sbyte TGkWOcYViOunyzNZjmpXIBcbCJUxg =  58; 
ulong pIgpPuQFzmizAWZW = 13020817178647625; 
int ufhCPyXV = 800092258; 
while(ufhCPyXV == 800092258){
ufhCPyXV = 810532;
}ushort zuqFCFJXMntcMiqNNGZLxX = 30215; 
ulong FYjFnIEqcKWbYJPx = 41832591825043220; 
int FRMoCWGeWGztyGQMKzuMP = 192565203; 
while(FRMoCWGeWGztyGQMKzuMP == 192565203){
FRMoCWGeWGztyGQMKzuMP += 946117;
}long xiMaEdMFNhaxTMiXRsU = 14774203308308965; 
int duZXzBxNsUDdhWq = 3007; 
if(duZXzBxNsUDdhWq == 14555){
duZXzBxNsUDdhWq += 312270;
}ushort ctyWSHhIxfhJNbzulMkzzsk = 978; 
int KDayGhKHWFbEEfkmgXZnEtRBYnfVp = 15324649; 
while(KDayGhKHWFbEEfkmgXZnEtRBYnfVp == 15324649){
KDayGhKHWFbEEfkmgXZnEtRBYnfVp = 936911;
}int YAaZgeUtIhenllnaowqM = 472429; 
while(YAaZgeUtIhenllnaowqM == 472429){
YAaZgeUtIhenllnaowqM += 313585;
}byte FkLiGGhyZnynWh =  83; 
sbyte Iez =  98; 
ushort yFTgB = 10247; 
double QTzOxyVIG = -5.038209E+23; 
if(QTzOxyVIG == -3.132331E+35){
QTzOxyVIG = Math.Ceiling(Math.Acos(0));
 bool? HobbHuztUJfZzudu = new bool?();
HobbHuztUJfZzudu = true;
}
}public void fWGpjqVhyBZZqJqZmD(){
double NszOeO = -6.194779E+13; 
NszOeO = Math.Pow(2, 2.1);
object tcYRNyS;
tcYRNyS = 3.200618E+18;
Console.WriteLine(tcYRNyS.ToString().ToLower());long gcxwU = 5095240331397349; 
string lCeVscBSSOxzZUyXEwoVWusDzkRc = "FZhUfcTKpq"; 
string kFHzUbLNYoNKfbRiCxh = "zDnPnEUS"; 
int nempMQLeEi = 29; 
while(nempMQLeEi == 29){
nempMQLeEi += 460900;
}ulong kKiTj = 7567700034204028; 
float LbJxQpb = -8.484622E-06F; 
float dilYDQPQHUfuVxhdASTmbnfGmEbYO = -1.040122E-21F; 
long kNZ = 67528507379886812; 
int OaJlCGEGWPcHLYPQbxJu = 6885; 
if(OaJlCGEGWPcHLYPQbxJu == 659235){
OaJlCGEGWPcHLYPQbxJu += 150046;
}int MKcRZGQapCJHhwn = 939639937; 
if(MKcRZGQapCJHhwn == 341990){
MKcRZGQapCJHhwn = 617140;
}ulong PGlndsAEtce = 8060193379350459; 
uint OclYwLlTBkSZumkO = 7323; 
long YoEcCbkMhCjSlQbEZpJHnKSSl = 40667045840774793; 
long AzjytBAMZbaNFlIPb = 87879249855181316; 
byte MnNNMEnDeQTogPXTRlgmo =  88; 
string segncOtQiQNylnojqKDZiP = "NpDUFAXwujauHxaAqxcfqbBHDW"; 
double GPeEEktXATOqkutnkTINpjlDgJD = 1.429106E+23; 
double fyQxdcZyILUXIlxJokG = 2.27074E-37;
GPeEEktXATOqkutnkTINpjlDgJD = fyQxdcZyILUXIlxJokG * 2;
object VJHhECfPjEOhOclK;
VJHhECfPjEOhOclK = -98662.93;ushort DZV = 50165; 
ushort wWDWLuTeIGdBsjQIbgdDFdTNyOh = 49466; 
int hymtPRsAdjkAScpQekkVYOK = 309434329; 
while(hymtPRsAdjkAScpQekkVYOK == 309434329){
hymtPRsAdjkAScpQekkVYOK += 470395;
}double gAUzGnYwY = -6342.681; 
if(gAUzGnYwY == 6.920426E-13){
double HJWCKQuOtdyPDag = -3.379255E-22;
gAUzGnYwY = HJWCKQuOtdyPDag * 2;

}string TNTofTZIMLjoKJuUecHIFseoN = "sMSzsnknCWdCNzDByhykCkfZCh"; 
byte mBIFDlBnQnsWdw =  37; 
uint EphedVXSDsgHTWkbPbanAZBX = 7136; 
sbyte CgfdHiYSdqiGLMaUtHWshUou =  -95; 
byte FPkQtNgpwld =  254; 
long tXzSwDNZHEIPQJHSLXxStcTBPSFx = 1682551064729262; 
float MIEGiHiEWzJEBgESF = -8.942105E-33F; 
double DYqu = 8.33031E-19; 
if(DYqu == 9.661136E-07){
DYqu = Math.Pow(double.MinValue, double.MaxValue);

}short ODQxSGRHSFuSdXjHxRhLcgdDKSk = -19210; 
ushort OyQopQZaanHDQHRso = 12476; 
float hhwHSIAmNawHjqRWcPItIwRTdB = -2.953239E-06F; 
ulong DauEuHbixIqM = 50059290798288348; 
short CykMpDEqqcCuyumG = 29240; 

}public void UTsZbWhS(){
string hQHR = "czWt"; 
long AGfpnjTDzNO = 16453452077708680; 
int DBjCxKsuVqlpUum = 6821; 
if(DBjCxKsuVqlpUum == 265831){
DBjCxKsuVqlpUum = DBjCxKsuVqlpUum + 292943;
}uint unbRGVWBecS = 105615563; 
float SBChExClLGiUSIDqiakQmwTM = -1.021406E+35F; 
ulong mbyhsU = 82504451944090585; 
short yadTCMKW = 26422; 
sbyte xHQIEHuZKibDYMqFb =  -116; 
short keJnAVXGCqBmmqAFWAumzcHbxyfg = -18789; 
sbyte UEgsqYCiJJPzsJOpHmMJ =  11; 
long AhzHS = 50295398722079108; 
sbyte hHupLEJFotIhzUAyflAIkTsJgTSud =  42; 
string jIeen = "XBQEjmkUs"; 
double WDujzOooMjSLoCGqDXQQujUnYRH = 1.726749E-32; 
while(WDujzOooMjSLoCGqDXQQujUnYRH == -16.4755){
WDujzOooMjSLoCGqDXQQujUnYRH = Math.Pow(5, 2);
object cEWIWCfBNJflIhGkmX;
cEWIWCfBNJflIhGkmX = -4.420603E-32;
Console.WriteLine(cEWIWCfBNJflIhGkmX.ToString().ToLower());
}int hRpZQ = 373850388; 
while(hRpZQ == 373850388){
hRpZQ = 87242;
}ulong GofNbMId = 51762195801746117; 
double sDBc = -1.069201E+09; 
while(sDBc == 1.774201E+25){
sDBc = Math.Pow(5, 2);
int[] ZExpXNgA = { 9510869 , 40301 } ;
Random PGzgGbZmPNACuwagwBlceJqcPSc= new Random();
Console.WriteLine(ZExpXNgA[PGzgGbZmPNACuwagwBlceJqcPSc.Next(0,2)]);
}sbyte ygKXFkxmywQfTAFF =  -86; 
ushort CmUyRfn = 27872; 
short ZbSJmcuKywWZmOHnDzDjmTC = -17079; 
int DKTSpOdAzu = 438812971; 
if(DKTSpOdAzu == 202519){
DKTSpOdAzu = 903413;
}long lCZ = 67608695408469011; 
sbyte GfYALRFehWOplpfS =  -98; 
double hCacnxTpoBVU = 2.224588E-10; 
hCacnxTpoBVU = Math.Ceiling(Math.Asin(0.2));
Console.ReadLine();byte OEKUWtJHIyExzdeqZBylA =  66; 
short LkpfSHMcRiOFaVADdIoydfsmNn = -3539; 
ushort biBoSjLcDPaLBGpoApdbUhSY = 55266; 
ulong TctFjtnQZnphMOACToM = 57622241385852138; 
float qCydESajkRslQqbjhsmwHZKflf = 5.297754E-31F; 
sbyte QauWIMTehVAqJyuqKLWd =  72; 
string zlsSq = "zaUeEbqRTJpqdjqWpplLUwgib"; 
float WWLpTCmJVFlfxcBQcHNMRNDV = -1.002748E+09F; 
int cULcLlwUisxQQCxqdkQIuGdDogXi = 7979; 
while(cULcLlwUisxQQCxqdkQIuGdDogXi == 7979){
cULcLlwUisxQQCxqdkQIuGdDogXi += 302247;
}int Tju = 64; 
if(Tju == 702259){
Tju = 192951;
}double pqkSQQYEyQiNEnmBtdiUBVwzURH = -2.305775E+21; 
pqkSQQYEyQiNEnmBtdiUBVwzURH = Math.Ceiling(Math.Tanh(0.1));
Console.ReadLine();
}public void DszVw(){
sbyte ZReASxDxGWPMbl =  -86; 
int xBzQIRQbyW = 934854896; 
if(xBzQIRQbyW == 479288){
xBzQIRQbyW = xBzQIRQbyW + 867071;
}float QDjhd = -7.968068E-37F; 
long dZwTuMjA = 37350750303402803; 
uint AedpSigtddnxbOVHNccc = 196992; 
short FjmOPnNOEODH = -25478; 
ulong mNbJgk = 59495661739690592; 
ushort oNLMJWt = 38052; 
ulong BSnloCZVPoz = 53013800887596062; 
ulong xgGmUubzpuyT = 35710086469086211; 
uint hKODDGfCbjdZeLIYu = 94; 
ushort VbIwzAjOjCjRTZW = 32161; 
short xsOtnQQGVMINNFddmIfhegWOR = 9589; 
float kMLzcnC = -3.803498E+15F; 
uint SFSEzfyPGOQNotjOoxaDGXI = 191408; 
string KzcZtLU = "DlDzIuSmuu"; 
double eTBmILZhVXyuyX = 2.916924E-14; 
while(eTBmILZhVXyuyX == 6.84633E-05){
eTBmILZhVXyuyX = Math.Pow(double.NegativeInfinity, 2);

}short eGZpIpUsgUtagSKiX = -21581; 
sbyte ulilXNLphldqTsOquyP =  -9; 
ushort GitDQPRdcz = 60029; 
long TyNtUF = 53724256842426266; 
ushort AtKieGZSuWWzdiFxjkVTHeUT = 56362; 
string qswcMzoDelBcmyVYYJfgMl = "XDhAhwzRpaVIBZTjaPG"; 
uint JiJxMgWj = 94222086; 
double MyAFYh = -1.478691E+09; 
while(MyAFYh == -7.039779E-36){
MyAFYh = Math.Ceiling(Math.Cos(2));
int? iRLIFSuqhffUL = 1538481;
iRLIFSuqhffUL += 45322;
}ushort BhWFQbQhUt = 46711; 
ulong RGTjDL = 13608225411736226; 
ulong seiaytUPdbEagndu = 30622628229492344; 
string ELpRyjcIDnDNKz = "dgChpLJtluPNyTzAxVaPiA"; 
byte tXzMPRGUHFDNdFOMUs =  19; 
ushort yGPdOgRKJCETXOslXEuP = 58485; 
sbyte pMxRhnURwRWWsKJbLOkXcYTfRx =  -3; 
double KHOpMzqsoKwOLicbRZDnDKaJ = -1.546675E+24; 
while(KHOpMzqsoKwOLicbRZDnDKaJ == 2.007361E-14){
double bMjiHGlqKxdQisQReaxXJmaHegWg = -3.555818E+16;
KHOpMzqsoKwOLicbRZDnDKaJ = bMjiHGlqKxdQisQReaxXJmaHegWg / 3;
try{
Console.WriteLine(KHOpMzqsoKwOLicbRZDnDKaJ.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}byte cGMuNJkgyEyPnLOETLoPKKle =  51; 
sbyte KIPicajllCwZUTtmCuTTuwOPI =  43; 

}
} public class ftuHjHRCzIwuPe{public void DbzMwVqxLLqlyoT(){
string uEeSRVlscsVFfZdPV = "NqWnCPXKWSutmFIUalZHLglusE"; 
ushort FLmyDxgJFfbpVhYuXKfpXlYSP = 19302; 
sbyte eDVXtfekFXRQyYoUjKL =  15; 
sbyte xJzfTsMwImoBFbkdWQcNQZSoS =  123; 
double JfFEz = -3.302136E-30; 
if(JfFEz == -1.685717E-20){
JfFEz = Math.Ceiling(Math.Asin(0.2));
Console.ReadKey();
}long ejJw = 19762823339039185; 
ulong bqLCdqkgxNExqoNmjjYIWimEnIlZ = 32418170654950177; 
long osT = 49113981506311794; 
int EXiZTehmw = 21; 
if(EXiZTehmw == 790083){
EXiZTehmw += 231086;
}uint LuDiUHPYwpMu = 178924198; 
ushort XuuPYLmDOTnInBhGwpsjcdJqt = 2393; 
uint wTxVQTpwSKuZhaubtgKUifY = 82499345; 
byte TMdwWmAgVysSukNOMWsccJI =  244; 
byte EFpUOeFfxJOo =  115; 
ulong TAEaooKxsmNziMjefhFJ = 74901207814411219; 
string xzGuijdXXcTiXHaHwumFJgTdOczUN = "aWts"; 
float SPDaahkDPM = 1.409487E-09F; 
long SVBzqpGCHgMikVbXQFQM = 50969663979066224; 
short idkf = -20032; 
sbyte UbBJpixnRXoBSV =  -57; 
int TlenUQtonFYIXeLVDVEpdyyuYNDH = 428136247; 
while(TlenUQtonFYIXeLVDVEpdyyuYNDH == 428136247){
TlenUQtonFYIXeLVDVEpdyyuYNDH = TlenUQtonFYIXeLVDVEpdyyuYNDH + 290609;
}byte bgakhucEzHTSFFpaLnTxOKphmNARz =  19; 
sbyte TNUwUshKuTVsTKckgfimx =  75; 
byte RTYuUlfuAkZR =  180; 
float sficUO = 6.280961E-18F; 
int nLsVUldDDEbuOPiOBTJLkB = 9935; 
if(nLsVUldDDEbuOPiOBTJLkB == 257801){
nLsVUldDDEbuOPiOBTJLkB += 755357;
}short UFwKHBOAFYWniqeYqu = 25062; 
sbyte eGSANAseNZ =  -49; 
byte KyZehIZbnKPWsUVSQTTioPYDjTbww =  215; 
double biHHYmllgmQVdEwFymX = -2.392786E-05; 
if(biHHYmllgmQVdEwFymX == 9.584691E+29){
biHHYmllgmQVdEwFymX = Math.Truncate(biHHYmllgmQVdEwFymX);
int? cYdAH = 3893594;
cYdAH += 20962;
}float lqituzQdHVwXmQiIyCRa = 2.727132E+14F; 
uint qjKaA = 587643; 
float ZlgOPBwtTSROGfhzUiT = -3.757126E+28F; 
long QdckOPEkqDqnEbTJb = 34136880169778894; 
uint GmMzMpJpLMoHEoHAk = 8280; 

}public void xbBRYlDLgMqglmYYGe(){
int VMhWMOdRjjgzmYfEMUBAOYjYwhwp = 6670; 
if(VMhWMOdRjjgzmYfEMUBAOYjYwhwp == 650702){
VMhWMOdRjjgzmYfEMUBAOYjYwhwp += 866520;
}long TFejiLBWGTUmgumLFgjWC = 54570876599763573; 
double upxLgPpqcymtDnhjWxjDCJMi = 1.852258E-32; 
while(upxLgPpqcymtDnhjWxjDCJMi == 1.834535E-08){
upxLgPpqcymtDnhjWxjDCJMi = Math.Ceiling(Math.Tan(1));
 bool? mabnBWSyGNANkzlpnoM = new bool?();
mabnBWSyGNANkzlpnoM = true;
}short Jdz = 4156; 
string sPRiD = "sthVdWf"; 
byte dFMXSaDlHCTmjkQJHD =  181; 
short LZlIZ = -27573; 
uint ERNdyGz = 61243384; 
sbyte PEkIJ =  101; 
sbyte qmytuWA =  -37; 
int pSWbdcLgjMFsWbuG = 518367; 
while(pSWbdcLgjMFsWbuG == 518367){
pSWbdcLgjMFsWbuG = 748085;
}float jWgyshYmSmppiNAhXCLwEZqwZPw = 9.478089E+27F; 
short DabRhWmObOsAJLdyNbsubbxqjFL = -3350; 
int ZuEqNogVskKOKIlBILVqJX = 883718; 
if(ZuEqNogVskKOKIlBILVqJX == 598508){
ZuEqNogVskKOKIlBILVqJX = 693905;
}long lQkoPYVqYPpRKgLns = 1195810779668246; 
string KxlKynPZTEuSnFqNgbRnuiHiQF = "GlnYlOelUMeTDUgRLYWReGnwkmOz"; 
uint dTSqiOFFheitxabhntnsWjUN = 996288184; 
double NZaHLeJlalMslfQSJMjmtdYuki = 3.708872E+27; 
double ieDJqqPgbIVfRubegQLAKaGwCk = Math.IEEERemainder(3, 4);
NZaHLeJlalMslfQSJMjmtdYuki = ieDJqqPgbIVfRubegQLAKaGwCk;
try{
int DDBmHOfcTVFWXMpdczXaqtfl = 2620206;
if(DDBmHOfcTVFWXMpdczXaqtfl == 10728){
DDBmHOfcTVFWXMpdczXaqtfl++;
}else{
DDBmHOfcTVFWXMpdczXaqtfl--;
Console.Write(DDBmHOfcTVFWXMpdczXaqtfl.ToString());
}
}catch(Exception ex){

}short ENnZdWPVOdTfziqpVxi = -4084; 
short zoYVN = -7496; 
string JHXzzLJamypqueFcJWjmp = "XWpzoYbhiwUdumhaKGHVjMgXL"; 
ushort VmRjOuSKbhaxKZenVxpLs = 58236; 
long EHuIkPtNV = 55972010116740935; 
int SKjWWe = 213080; 
if(SKjWWe == 664203){
SKjWWe = 344114;
}string EGhZkzlPmBwTjVKqHiQ = "QDuKzEOFSsmWepOFXKqdVdx"; 
long cUDBjOADnPblzBIL = 63183755833727089; 
sbyte XRMSMgLixJkdI =  17; 
ulong QBNtAiKtngBxb = 79719330760415545; 
ushort UCoe = 63828; 
uint osAeYtuMTOhsFFHohh = 81635021; 
long NeEuySRkdLTKRiWXoHaJWcByR = 52633471433298624; 
uint uBEWCVJLyAUNysjTcQY = 698087334; 
int EYFFdg = 199520; 
if(EYFFdg == 570422){
EYFFdg = 526545;
}ushort KAOnscYWLRUXdmTXEUPuZX = 62088; 
short EcoguUROhspjGsDhImO = 12941; 

}public void chqRQJ(){
ulong LMCy = 69706875231909611; 
sbyte UDNXDmmsLjGzBUXoFkfMxGPMXFDy =  90; 
double HXgmOdSgzPtbsSjmfMBNMXyBo = 5.21222E-38; 
if(HXgmOdSgzPtbsSjmfMBNMXyBo == 4.158292E+15){
double ZCuHjkMeHWcEx = 0.07234256;
HXgmOdSgzPtbsSjmfMBNMXyBo = ZCuHjkMeHWcEx / 3;
try{

}catch(Exception ex){

}
}short mHndgF = -15602; 
short OJjh = 25345; 
double WhpxbfzWjdiysZimIXFCAWNJM = 1.255359E+08; 
if(WhpxbfzWjdiysZimIXFCAWNJM == 5.878386E-13){
double EouVehiqTdxQXcFwD = Math.IEEERemainder(3, 4);
WhpxbfzWjdiysZimIXFCAWNJM = EouVehiqTdxQXcFwD;
Console.ReadKey();
}float hFUNkGJQlbhu = -3.298335E-31F; 
long DRqRbjWLwPTVSKdZkuLRa = 85739283614806075; 
float BVQTXFaqXIiqxMNXCYhjKuQKx = -1.142342E-33F; 
string mViDXtacRfukdOi = "OeCcGwcuJhfffxHCXTLMaPAsN"; 
float TNZHHHTIlCZSRSOUPmUWu = -9.586809E+26F; 
byte cxQZGjpL =  185; 
byte KBAzmPwZXaUUmVKeSYpu =  225; 
ulong EjcJHsKgfJgjoO = 20574024530319769; 
int eVeyMwQNXeVnjfPHnU = 21832373; 
while(eVeyMwQNXeVnjfPHnU == 21832373){
eVeyMwQNXeVnjfPHnU = eVeyMwQNXeVnjfPHnU + 329888;
}ushort LpaymwIAFL = 17605; 
byte KStlkSiAAombo =  98; 
short tBNcyLANBRBHLCd = -8815; 
string ftNAjAlQkbOxXapb = "kBjXYtGM"; 
ushort DOYkUZQoYKUp = 17973; 
ulong ByZdUApKpSSSChZhORhK = 32118271535474521; 
short UltqyVHIASle = 9513; 
short hnh = 31587; 
int AggGnH = 63; 
if(AggGnH == 993302){
AggGnH = AggGnH + 350697;
}double GzkKeCfqpyWGySoUFsOkwflMjnBb = 1.25176E-32; 
if(GzkKeCfqpyWGySoUFsOkwflMjnBb == 2.122755E+12){
GzkKeCfqpyWGySoUFsOkwflMjnBb = Math.Ceiling(Math.Tanh(0.1));
int? msSiIEODB = 4405224;
msSiIEODB += 60533;
}float yNknoD = -56.38978F; 
sbyte JVWFiUSVx =  12; 
double bWEFz = -2.162708E+16; 
if(bWEFz == 5.877066E+18){
double EnkKxZLOwKoGbckCioksMnLKd = -9.092307E-38;
bWEFz = Math.Round(EnkKxZLOwKoGbckCioksMnLKd);
int[] WUVusqEKMfXunBU = { 5223291 , 62930 } ;
Random TxKVoEMQSfoE= new Random();
Console.WriteLine(WUVusqEKMfXunBU[TxKVoEMQSfoE.Next(0,2)]);
}float MqAPulKL = 1.916597E+25F; 
ushort mYzOQuk = 10700; 
sbyte hshf =  7; 
byte aifn =  194; 
long aeVHNYoyhEDUHCOgxJdIjhIoLyJH = 37610743908254423; 
uint wgOKkFDIGBmcKFKZNT = 620808; 
double pJPRJcEfPbI = 5.936609E-19; 
double RitHHo = Math.Log(1000, 10);
pJPRJcEfPbI = RitHHo;
object nTIfRfVtVBeALkuSqNfnWuUTzg;
nTIfRfVtVBeALkuSqNfnWuUTzg = 8.839673E-15;
Console.WriteLine(nTIfRfVtVBeALkuSqNfnWuUTzg.ToString().ToLower());
}public void glYqyilPXdkBXUYOHK(){
ulong jDmRiPyGIiHsKsmIVgKgWww = 79729748817422582; 
ushort QuFYgiIMGgPxSQFHpTAaKQOgePsQ = 37249; 
ulong IluTYJKOY = 88114803655812846; 
ushort ACbwmaGzU = 23514; 
float hQUgkzXHXHfCYpAAAqFsxppLH = 6.489986E-29F; 
float jQwgjlCuBDMc = 2.28399E-16F; 
float AxXBfYDOzwSAiB = -5.500121E-14F; 
string NRLwOdYfZP = "oJayoguitllmHIPublZiuYzCcM"; 
string UCaZOVjMRbTuZhLcCEdN = "JWTZHypjzwEbMnRlmALzcEasGilnx"; 
string IqIlGgZhzTDsHps = "UTRoesoXeW"; 
uint ykcOzWVKdclem = 2501; 
int ltbnVsuFLuocLhJF = 69608910; 
while(ltbnVsuFLuocLhJF == 69608910){
ltbnVsuFLuocLhJF = ltbnVsuFLuocLhJF + 871810;
}long IjRIMbMfJyTDA = 80058424143988151; 
long VEAIIaPMsKHHM = 66625770765414310; 
short fBiui = 5949; 
sbyte PaQWem =  -27; 
string UMBTT = "QqVpdUwmgHyuhAGH"; 
sbyte DSGVFIGoF =  -105; 
uint OmmatcmnbmtNKpHSm = 489055; 
byte npbTUNnLPpqOGSkhgg =  142; 
ushort kpzTlnaObZGlWAWkhzpZElWcZd = 29890; 
ushort gzWULdaDZIzmWOqMDJsoW = 64236; 
long uoYtwUiQF = 78693805418732225; 
long RRMHzG = 56584544976519700; 
long weIgfAJITTPckyWt = 61400228546924955; 
string OLzoNKaRQqnCbqi = "OFQol"; 
double NBpamRN = 5.014949E+29; 
if(NBpamRN == 1.952942E+17){
NBpamRN = Math.Ceiling(Math.Sinh(-5));
try{
Console.WriteLine(NBpamRN.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}double TKYmyVuzPJ = -2.150236E-17; 
while(TKYmyVuzPJ == 1.311196E+26){
TKYmyVuzPJ = Math.Ceiling(Math.Cosh(5));
 bool? AdHWGioNKduywNNlBIxQsbCjllo = new bool?();
AdHWGioNKduywNNlBIxQsbCjllo = true;
}ushort nWDyDyLuTJScgElllVKICxcK = 32426; 
long SjHEQjgEnHdoegMgeQALLhQxgwO = 47588121842795594; 
byte fbTaaXFhH =  151; 
uint oYTaXjLtkxwwSMuOfuDnCUZO = 69820428; 
long lnBQmyC = 15123589719921604; 
long zhoEQclcmowZfumABhWV = 88671028150057721; 
byte KSpgg =  137; 

}public void GwloOaigWTDKTI(){
long ObqNe = 24685910632801415; 
long mLdUMLGShlcDVFOqyZODfQPqCtpCV = 24018961586728306; 
ulong thjjCbCgpZBLGPgSyWNttueWpJAo = 67344446136628615; 
double moZmaDwgXPfcSbduFYdm = 1.495095E+28; 
while(moZmaDwgXPfcSbduFYdm == -5.312513E-29){
double FuNWQ = -1.008159E-25;
FuNWQ = Math.Sqrt(3);
moZmaDwgXPfcSbduFYdm = FuNWQ;
try{

}catch(Exception ex){

}
}ushort bsGbzF = 34463; 
ulong iScjEuCMhYwmdoMGauhfe = 88015382048095327; 
double QWclTLWxZTtkxXTYcUwzjtGGR = 3.97731E-22; 
while(QWclTLWxZTtkxXTYcUwzjtGGR == -2.289095E+35){
double GpadSUeX = 2.179173E-05;
QWclTLWxZTtkxXTYcUwzjtGGR = Math.Round(GpadSUeX);
 bool? UTXytYTA = new bool?();
UTXytYTA = true;
}ushort KJswLFLELBiZEJCwBJgbPgFCuAu = 60502; 
short ulXDLH = -24715; 
sbyte xAsoOhEupBfSEdsfDd =  81; 
string kPlFfXcaX = "otfLDxRRuRedwmnRiFyhDpgqlKg"; 
long kButGQAytPJMmxjhgtYq = 4847229148357078; 
ulong SpfiiqLLQyOtQQO = 16083410541297584; 
ushort xcQEnQmJUWsG = 6130; 
byte xwECDWYIQtbgoFtCiMPNnUhCHB =  175; 
float wQkFeNKNGRyywajlKYeCujUpCG = 1.497381E-19F; 
string bldOuDRCjRJPDUsoSUXpWCFtzlW = "dAkxMKQktHGIwiPnTGHaXlNIpMZ"; 
uint LphHVyLGmdTehVknpUON = 4978; 
float VXSMEfZLoDJqzaAMdIH = 2.509291E+19F; 
double pCtaRlNqkxDMYGYcgdhPFmTQ = -1.154491E-08; 
if(pCtaRlNqkxDMYGYcgdhPFmTQ == 1.575618E+12){
double hCjjfiADlzUVoCfeGAzGgldJEKxjd = 2.130548E-14;
pCtaRlNqkxDMYGYcgdhPFmTQ = hCjjfiADlzUVoCfeGAzGgldJEKxjd * 2;

}byte SUGmmGjtOQIkLzhT =  67; 
short onYdhSYMibZXjBKLQxdzRtEgcIzW = -18660; 
ushort lnaYlURzuoUbq = 50859; 
float KbNsxjSeVKSaAub = 7.033985E+17F; 
long JiCzQYYJjXsmsjjOVydYbxW = 39288645636025598; 
byte ajohEWtzCJgEyARoGfdKJ =  188; 
byte zXPhaJWFe =  139; 
short VHsCKBAOPBdCZJwhpFanXcKGhlyTN = 3301; 
uint iRuZODZilzyYTmhqmOoACdbTWMt = 46773; 
int lbh = 1617; 
while(lbh == 1617){
lbh = 764673;
}int bzFLMVdfGYnZteQCcbiCgGykX = 87381600; 
if(bzFLMVdfGYnZteQCcbiCgGykX == 829635){
bzFLMVdfGYnZteQCcbiCgGykX += 395281;
}uint KbDnlZzwWbcYbPntNtqFdLxatOa = 8; 
string ENdKdmmRcLfjFwmVWFHpycWS = "FCUyYxqTmzkYekfADoUXBSGgTexyN"; 
short RlLzLstdnPqsWbXSu = 786; 
float ObVzNWOFUaUgVXuMtNwgilHhjBJ = -2.652156E+32F; 

}
} public class cWWxNPqmL{public void FPinVjxZHkzHeKMCwNommKJ(){
long XkAjGwgbHDuctmadKyLOwRlLltEY = 5544604663244502; 
double bPqGzhqAcBniDlXUDfYOYlRi = 7.821385E-31; 
while(bPqGzhqAcBniDlXUDfYOYlRi == -116.6159){
bPqGzhqAcBniDlXUDfYOYlRi = Math.Pow(double.MinValue, double.MaxValue);

}short bGqPqTxmpShiaefPEVjIPjB = 29755; 
string NleDkciZLocyEUdaSgXMnt = "hCKPHYSetMHgjhOHfZbuRMgsoFO"; 
short apxtqXbCmDbTj = 12140; 
long OTTeASNmZdwuqAckAMtwjNTkMfSy = 86839972977718827; 
float ThEjumCHjO = 5.19249E+22F; 
ushort ppNaaIsuaomzxtZ = 61134; 
double olSlSatHJeWd = 1.833247E-25; 
if(olSlSatHJeWd == -1.422539E+18){
olSlSatHJeWd = Math.Ceiling(Math.Sin(2));
try{
int dqlHVnxHcpteE = 7773356;
if(dqlHVnxHcpteE == 64842){
dqlHVnxHcpteE++;
}else{
dqlHVnxHcpteE--;
Console.Write(dqlHVnxHcpteE.ToString());
}
}catch(Exception ex){

}
}string IGWUqnJbkVTEFDMYtC = "RlaVJiEzPPIExqIY"; 
short CpOIsTIkUkYM = 12560; 
float ijsILnMNQ = 2.541485E-21F; 
int ymXExHSlLiUg = 4491; 
while(ymXExHSlLiUg == 4491){
ymXExHSlLiUg = 58284;
}sbyte qmhoqEePDXOfppOHnwodday =  -124; 
uint yQyuqQgQajfImTDcJ = 38889977; 
ulong XJIeeObKpQgRPgnsiGYZBoFC = 64751075919807459; 
long BgQomcegKtYQyNnLAhxPpI = 40565821830021309; 
sbyte heGgCUtVUtGlDQLK =  -65; 
short BkVwScKQqLEzMlAJsggiGPBUWc = 24059; 
int kInqfHolKOeRebjqVTZdg = 5633; 
while(kInqfHolKOeRebjqVTZdg == 5633){
kInqfHolKOeRebjqVTZdg += 716178;
}long zQi = 64097163641048004; 
byte JtcqBNpDbiTACS =  21; 
sbyte SVGklXVK =  117; 
double bOmwUL = 4.981655E-24; 
if(bOmwUL == 7.30217E+17){
bOmwUL = Math.Ceiling(Math.Asin(0.2));
try{
int CnWzGRiUd = 7340187;
if(CnWzGRiUd == 31756){
CnWzGRiUd++;
}else{
CnWzGRiUd--;
}
}catch(Exception ex){

}
}uint PXjigiWiL = 33468515; 
ulong edEIL = 35607121644583418; 
string xxMoqVegjXRdRHGNbHDc = "mSaZbFKxGZXgnMngNwjZXuKU"; 
byte bzODEHZTiFLopquLFXTX =  142; 
float HoktcSThgNuE = -1.521422E+11F; 
string qgwEKwyfSFmPNa = "dgghnsmXaox"; 
int VNicDBDQhqIkReByG = 41; 
while(VNicDBDQhqIkReByG == 41){
VNicDBDQhqIkReByG = 432421;
}string OddjunmRVjexjmly = "WQAWymMAukbzeGqe"; 
int yAGOcwuVVwbxRjJXuGFEqQq = 99647695; 
while(yAGOcwuVVwbxRjJXuGFEqQq == 99647695){
yAGOcwuVVwbxRjJXuGFEqQq += 822465;
}int qHAOYUiOeCCsqzYuRQhkLAY = 627955; 
while(qHAOYUiOeCCsqzYuRQhkLAY == 627955){
qHAOYUiOeCCsqzYuRQhkLAY = qHAOYUiOeCCsqzYuRQhkLAY + 2116;
}sbyte eqI =  50; 

}public void qwKclSBbQzamaJuEJsdRIchR(){
double NVBxaKOjcwfeZ = 7.449074E+36; 
double OzwYETgonTcRe = Math.Log(1000, 10);
NVBxaKOjcwfeZ = OzwYETgonTcRe;
try{

}catch(Exception ex){

}ulong WMQaRGt = 62054288043251889; 
uint swlFOSRkDkzzoin = 173940579; 
ulong qbjxNUIVTdqZdWXVoFFhT = 86973860427156469; 
double CyZgxhZAKfX = 3.676455E-28; 
if(CyZgxhZAKfX == -4.2744E-09){
double oFERslVbSsBlTNPDePBNpfUbWbJdj = 7.979152E-14;
CyZgxhZAKfX = Math.Round(oFERslVbSsBlTNPDePBNpfUbWbJdj ,1,MidpointRounding.ToEven);
object aaXThOkmYHsGLeu;
aaXThOkmYHsGLeu = 1.841797E+36;
Console.WriteLine(aaXThOkmYHsGLeu.ToString().ToLower());
}ulong MSJWaoAszDxkWoICkmXVsRmqny = 77037455326213860; 
long ZtNCapLhfJunkogfOU = 56663797765150638; 
byte DEoLHdC =  215; 
uint UbfelFcujRhUea = 75331822; 
double KiUWeDGCuKIgOoFsaAPA = -3.870511E-32; 
while(KiUWeDGCuKIgOoFsaAPA == -2.58167E+30){
KiUWeDGCuKIgOoFsaAPA = Math.Ceiling(Math.Tan(1));
try{
int NalYzDGIIbLC = 8930799;
if(NalYzDGIIbLC == 93691){
NalYzDGIIbLC++;
}else{
NalYzDGIIbLC--;
}
}catch(Exception ex){

}
}byte BGfK =  251; 
string McXXJNz = "Mzhjut"; 
int RVSxFFEVfqiA = 77805277; 
while(RVSxFFEVfqiA == 77805277){
RVSxFFEVfqiA = 161063;
}ushort NqeihiRlNN = 53547; 
long moOgFEhAcRsEYsXZItlXqGAQKnw = 14728458889885673; 
uint tkFnQLBNFjRUleuEMDIPYx = 1466; 
ushort TzRdtQ = 29425; 
short hIJCUZdfeF = 23962; 
sbyte ZWOcU =  49; 
ulong UyUJSbVBQbxXRlWcbNQUqmECbPAU = 16634065540657049; 
ulong cXhFnDxhmNZMahUSGIUsHcDbkA = 51654492002900103; 
long NMZafUDMUEqNTJMGpxlauguOxM = 18180192870801860; 
sbyte ITmNARgIiuRMNtyQXJzHskFthP =  117; 
long AnRET = 28870064028212084; 
int Vaaw = 673235587; 
if(Vaaw == 567252){
Vaaw = Vaaw + 490205;
}uint oUFLSAEJfZiUwbH = 147290; 
float HGDyhPDSwGzndsBgoUQ = 0.8542095F; 
sbyte WWRfaOcxIJTUSNyyWpIidHMeCUU =  -42; 
int igyKGCpJDdt = 81; 
if(igyKGCpJDdt == 468059){
igyKGCpJDdt += 667124;
}ushort yUwSZSojNFfTLUqBMxigWgmOF = 52628; 
float gTdlBAfxOnyRqqEdxjIoBeAb = 4.882664E-21F; 
double HInjXAcTCUt = -2.737809E+21; 
double EjquhHpcubLmEPQgZnU = 2.151711E-13;
HInjXAcTCUt = EjquhHpcubLmEPQgZnU / 3;
 bool? CRyBGsadiyqMypUcqi = new bool?();
CRyBGsadiyqMypUcqi = true;int LyYqTHQELfKKtYD = 248289472; 
while(LyYqTHQELfKKtYD == 248289472){
LyYqTHQELfKKtYD = 114878;
}byte JnlcsLQCCNDsMAaTQ =  126; 
short kApeCHpuWMOs = -201; 

}public void pdHFyTUFhWRGRpmjDqQTsiXgBMu(){
ushort QZjSRjXmlcyyGqunWQZ = 9328; 
ulong yDHDgfA = 28243540526173360; 
ulong gpWpotxMSDTEWwoTHDIlncRNhqYYG = 8615647815193414; 
float ikRoEHA = 4.469839E-34F; 
byte LuZCLd =  121; 
byte XuyfSfm =  103; 
sbyte xdOuzbyOngIDVPzoc =  33; 
long QtGKk = 32249613899798389; 
uint dWaCAnokdBwAIHEdGNaMdyd = 68; 
byte sQNDTtbwsbfhRBhDiQofdSu =  198; 
short QJHSRRlBWwqYDfGllbAG = -20644; 
byte VUtuiKBxYkkEtPAknVpENlknjAR =  159; 
short MzFNLXyydR = -4538; 
double NqDmyMRlKTIi = 1.037979E+14; 
if(NqDmyMRlKTIi == -4.260807E+13){
NqDmyMRlKTIi = Math.Pow(double.NegativeInfinity, 2);
for( ; ;) {
Console.WriteLine(2.270075E+33);
}
}double HxgbbcnxEzBFUjkqeF = 3.513879E+16; 
while(HxgbbcnxEzBFUjkqeF == 7.503754E-24){
HxgbbcnxEzBFUjkqeF = Math.Floor(-1.634547E-05);
try{

}catch(Exception ex){

}
}uint EbnmdZokdFFXNBLtZSSU = 511750311; 
int VUS = 315445; 
while(VUS == 315445){
VUS = 72727;
}byte iaGybMtdPm =  155; 
byte exQYCH =  182; 
string mqjhRUTidTClCt = "wQiFyRPpILuUu"; 
ushort yAkONonTVzjHiNLUEdxELzgGEybDx = 28954; 
string MexcaNGegXmfmkHE = "QQQXUnYtGjonMyzzKUdpITQzsxOk"; 
string sRyGGHjDSQyK = "DpQwAPIDqDmoW"; 
string FyBOsnSzQgRR = "hcnTYzmhCb"; 
byte sPbokpJHSXuoJq =  36; 
long anJYuwnotpzNkcxmtZzQiFQhk = 75627191648256403; 
short NFyYuNaRakTHUYcps = 11049; 
float wzBNYkGpjJgkVCJIVaeci = 1.547105E+23F; 
double PpKVToJIHfod = 9.094529E-39; 
if(PpKVToJIHfod == -8.131727E+14){
PpKVToJIHfod = Math.Pow(2, 2.1);
Console.ReadKey();
}ulong YDURxu = 59498036166014335; 
long NCnFFYA = 4616136125003696; 
ulong KpIwCBkpnZgPlgdkT = 63532584203319228; 
uint aVedsmwBodOlacnqYsTdCpIk = 77; 
ushort hpFiaMkwjWPCR = 40563; 
ulong eEHtUkb = 51924969029513436; 

}public void UasZAFloSSgcEsmVDB(){
sbyte hZBXnTQEACipWlIQ =  -59; 
ulong AssCcdYhajshMNzxUBwL = 52637125597755856; 
short xHNSHiPzyToLQuhtxEzQO = -13055; 
float cLKD = -7.208307E-11F; 
byte QpNLuUfpHwcDwVYnIXlmICWgOK =  204; 
double loeGxUAHxZDyiis = -2.874732E-14; 
while(loeGxUAHxZDyiis == -1.703988E+18){
double aknuGWKe = -1.472046E+30;
loeGxUAHxZDyiis = Math.Round(aknuGWKe ,1,MidpointRounding.ToEven);
Console.ReadKey();
}byte uJfmPKnyjM =  231; 
ulong AIwbPedaqOwuGMJlDgGsGc = 20111927676681088; 
uint opaHblDJhocULSjYGkHGoy = 43; 
short eCypAWoLeoLbjqFXjqaATdcJhp = 17596; 
float RjEsjgalSQgFZN = 4.405012E-20F; 
int pas = 7912; 
while(pas == 7912){
pas = pas + 25544;
}uint hihxomehJakELqCTVTphxEo = 858405; 
string wgEjeCyKHYiLAQeVae = "HWnEUgLsqEUKElK"; 
long fRf = 69301248401922238; 
short JotwxIgEKcuj = -24601; 
sbyte oGzQgeQiZwtotJhYUaaVFqh =  77; 
string xxOMSgChGgKakmTfu = "csOUCeAigkhYyINEItunDmOkdbnUQ"; 
ulong zXhN = 87622877349604058; 
short IKb = -16651; 
int AMfxQFDRBRFtthJaoV = 78; 
while(AMfxQFDRBRFtthJaoV == 78){
AMfxQFDRBRFtthJaoV = 29944;
}float ykxqlIiiE = 4.905349E+24F; 
long fmLuTHpIiqHugjSzaaeDEhquiFXj = 69393791396515074; 
ushort SuEFn = 58718; 
ulong ezqLENMhuYVplnKfyGuxRjlpdViK = 43057346301076548; 
float ZjywEZHeTELyhJAt = -1.533808E+31F; 
byte IJE =  11; 
sbyte eALZPjmXGkZwHl =  44; 
double GYDR = -7.907608E+32; 
if(GYDR == -6659533){
double GwmhUhtTPCjjd = -7.702555E+25;
GYDR = GwmhUhtTPCjjd * 2;
 bool? asJGYdHFVDz = new bool?();
asJGYdHFVDz = true;
}int FVYFFPdPJQEQIQexiGnXOVGBUKWXD = 235129009; 
if(FVYFFPdPJQEQIQexiGnXOVGBUKWXD == 357683){
FVYFFPdPJQEQIQexiGnXOVGBUKWXD = FVYFFPdPJQEQIQexiGnXOVGBUKWXD + 946061;
}long lTEEqZcQ = 27243048551236259; 
int KoyLUSgm = 56671593; 
while(KoyLUSgm == 56671593){
KoyLUSgm = KoyLUSgm + 869182;
}ushort WNLtyWDK = 24865; 
sbyte TnCBhnhnu =  64; 
uint LyepAOGoQJdCYJmOOMOqPGmZhcUHh = 621474; 

}public void uOZWtHdVOQxjkZC(){
uint uBJ = 207350529; 
sbyte jfhxU =  122; 
sbyte JGgjJqwg =  43; 
float YidOiBuFOfszjFUNm = -4.947787E+08F; 
string KlCGSsRLYtZhIkWgqZVAuteq = "XFPEhJ"; 
string OoWCipiEUPNsqCuYHWALCwqhj = "VNbgRKXqUVZFhKN"; 
double EsYPwkRXkjqosYPs = 3.049232E-09; 
while(EsYPwkRXkjqosYPs == 1.471872E+19){
EsYPwkRXkjqosYPs = Math.Ceiling(Math.Cos(2));
for( ; ;) {
Console.WriteLine(1.994815E+35);
}
}uint UMQsmkglteMjgwwxegnO = 83433258; 
long egdTMAmd = 49853542279154787; 
ushort FaQFIWflecUKUcdHng = 8764; 
byte TSJIJnmshQgtklJt =  126; 
uint OBwjjS = 88497; 
string JkZhdMhhWZJSTXm = "qJQumOIZzZzIzQejiUzFgkh"; 
uint lDAzWHyXxBUnKkUE = 68; 
string BJgVzJYzJG = "CpTbki"; 
uint saKcojVzKEbgCdmZD = 582948; 
double IZoQwtYRi = 1.047918E-08; 
while(IZoQwtYRi == -1.288753E+16){
IZoQwtYRi = Math.Ceiling(Math.Tan(1));
int? tZiWFTxcteIqQU = 3300289;
tZiWFTxcteIqQU += 94045;
}ulong dWbLRaQLCsOOCHZQBGeHBGPTmgK = 22763071196571111; 
double SWZkfKNoWbELoMMShtz = -9.418342E-27; 
while(SWZkfKNoWbELoMMShtz == 4.765331E-14){
SWZkfKNoWbELoMMShtz = Math.Truncate(SWZkfKNoWbELoMMShtz);

}int ooZMcIVt = 7524; 
while(ooZMcIVt == 7524){
ooZMcIVt = 900444;
}sbyte ZPCdqN =  -7; 
float KNhRMTmFylXPsMgfwwqQFpRQxeP = 3399.674F; 
float RiQtUNDBUbSZJksIc = -2.740249E+34F; 
double gbptWqwHeIc = 9.161492E+13; 
if(gbptWqwHeIc == 28.12085){
gbptWqwHeIc = Math.Ceiling(Math.Acos(0));
 bool? KjGcxKlE = new bool?();
KjGcxKlE = true;
}uint yatmXMaIuWUiDqq = 98; 
double PZVXWjKPKyWtPBGofqXIpwbRHjG = -3.007439E-38; 
double DdxdmXqkzEfJWclL = Math.IEEERemainder(3, 4);
PZVXWjKPKyWtPBGofqXIpwbRHjG = DdxdmXqkzEfJWclL;
for( ; ;) {
Console.WriteLine(-4.094919E-13);
}sbyte caJcTOojRiWkohLl =  43; 
long QiEcPVAKo = 88971022307651386; 
ushort cAJ = 29578; 
ulong jWbqNCbRA = 14047233526203339; 
string HPnaLusRIGOhMPQFZysCMiEjTmnu = "TJwIBgyWWpXuLtlLj"; 
int hnhWMAkylegzfXoOWsAkJ = 6741; 
while(hnhWMAkylegzfXoOWsAkJ == 6741){
hnhWMAkylegzfXoOWsAkJ += 130720;
}string qzKHEmnMNKioJkSPRRywFptK = "bOFeOpskIUGyS"; 
uint bciIqWZiRnpFKGkbeZlCzJ = 98031934; 
ushort ebplsZoMuDgMtzcGOQyMJGyT = 34420; 

}
} 
public class FbNDGsJTuSaUJFbMpyECxiWlD{public void EYwyOUzsafFGiFhjImePPotK(){
double okwwfCZRPLzX = -2.550084E-30; 
while(okwwfCZRPLzX == -199661){
double YulCJgOanFL = Math.IEEERemainder(3, 4);
okwwfCZRPLzX = YulCJgOanFL;
for( ; ;) {
Console.WriteLine(-1.844494E-09);
}
}float iuCDgilqnxOLEJuC = -2.297962E-14F; 
double zzlfNCzjh = -4.088785E+29; 
double YFlsmMGmswKwSHBFcLixPGKcjeCeI = -8.656399E+25;
zzlfNCzjh = Math.Floor(YFlsmMGmswKwSHBFcLixPGKcjeCeI);
object qswafkAAAid;
qswafkAAAid = 0.0001422484;double iCnfEW = -2.16812E+11; 
iCnfEW = Math.Ceiling(Math.Cosh(5));
ushort wJKgfexpfkpCOMaDWzxgdDCxhU = 20154; 
byte OgEPtoKLqwaHBpCi =  156; 
sbyte HwEzooPSgnZkPkQsdNNwyqSFcw =  -2; 
int anCLxFekI = 4992; 
if(anCLxFekI == 548347){
anCLxFekI = 439839;
}short lxKXywTO = -4651; 
short YxUPlGgZyBsHLhxzmR = -31753; 
ulong mRjdToESJWapCl = 62981908003424881; 
long PYQbXHXwAhMFKLspDhPEsqjVke = 80212233902313087; 
short ADVcYFQuBHKZZCTofgBzPURSeAl = -23472; 
sbyte ytxzRamMFOBQw =  19; 
ushort zIAJlaQQfFLcYAsJk = 7634; 
sbyte DCLTctDNXZA =  -66; 
uint aEJAiyEHslokOewSF = 4106; 
string XXGHSycSgezExAPaLXLNWizERGysf = "kRuCcGcSpaBOl"; 
sbyte HqGGSTqKpdPAbPmaLNMUHiUxi =  115; 
float LmhXqOknSMmCTgezngad = -1.460238E+38F; 
ulong Ami = 35871746549838156; 
string RhnFYhYAkbBexhcXPELxgyeFNRJ = "tBihbExYNz"; 
sbyte RCokJtaauXkgnHOudKRhAKfKT =  -37; 
string MdkqzTGFiTVuQIUwpbTwPcVcI = "shwFsppioqNUIwbiljOxsRPLuRAI"; 
string hhKCByiEBGeHtEJxtzuGR = "okFCtLjCRIfHiGkgUEaIJi"; 
short zkbYybnxSxSzWYeytZOxCN = -7073; 
string mEQGicgS = "dhKldQuKONNMjVADnKuVjLRYNk"; 
uint RJuMGcVWOhVYcRY = 31675694; 
uint IpdV = 7293; 
double sqMzTqWTftbfgaEPfOxTs = 2.059235E-25; 
while(sqMzTqWTftbfgaEPfOxTs == -4.678041E-29){
double aZJsDFbJcHgYTnotWHNnPFpe = Math.Log(1000, 10);
sqMzTqWTftbfgaEPfOxTs = aZJsDFbJcHgYTnotWHNnPFpe;
Console.Write(sqMzTqWTftbfgaEPfOxTs.ToString());
}ulong tUVsUjjuAYwKFZXyXzpfsT = 4735187921068878; 
short CsZMmMpDquGtWqTPCCQYwJtBsY = -27664; 
double bGY = 265.4241; 
if(bGY == -5.073949E-38){
double NOXmiaWd = -1.858851E-24;
bGY = Math.Round(NOXmiaWd);
Console.WriteLine(bGY.ToString());
}int GOxGqVgDQXDlip = 418314; 
if(GOxGqVgDQXDlip == 422852){
GOxGqVgDQXDlip = GOxGqVgDQXDlip + 404474;
}uint RtlyxbZwymI = 36173548; 

}public void zdcygLmuKpJG(){
string uCNjXoWctTzeDw = "pFwXhWPTYoNdyKWP"; 
float FaV = -4.25822E-06F; 
ulong xxKi = 81826557643859812; 
long zGJPVjlAtxeIdYuFRgGTMBeLZm = 81519666860479182; 
sbyte mLCGSWcneqXjzo =  13; 
int PgXBYPJGaVdkDAXTYsfF = 18422602; 
if(PgXBYPJGaVdkDAXTYsfF == 814241){
PgXBYPJGaVdkDAXTYsfF = 618079;
}long wNoNHTtahU = 34600192142422796; 
uint aqAWyZJOqALsgtkmGqEKfWLed = 71; 
double OANeRod = -1.073346E-10; 
if(OANeRod == 1.187422E-22){
OANeRod = Math.Ceiling(Math.Tanh(0.1));
int? SmnJGEaXmoegZl = 9056065;
SmnJGEaXmoegZl += 13370;
}ushort jEsRnmek = 29053; 
float dtDafIlKAwIGjIDu = -1.637783E+23F; 
int LRczbkjKZkTKfMXE = 496730748; 
while(LRczbkjKZkTKfMXE == 496730748){
LRczbkjKZkTKfMXE = 441552;
}double CXijMTTwSuPlqdYUzDKoDYBz = 1.9829E+36; 
while(CXijMTTwSuPlqdYUzDKoDYBz == 1.277197E+11){
double mXecZYgszRMWMmSCnVgNHmExLAN = -7.349953E+23;
CXijMTTwSuPlqdYUzDKoDYBz = Math.Floor(mXecZYgszRMWMmSCnVgNHmExLAN);
object ECpdgUSzEEXdgPCYMGkun;
ECpdgUSzEEXdgPCYMGkun = -1.860977E-29;
Console.WriteLine(ECpdgUSzEEXdgPCYMGkun.ToString().ToLower());
}byte IiCaPWtuUZbeNmA =  246; 
short MsXIgDEVEYRftFQVajYkWosPmw = -7946; 
string uWAkoXRAZbZyUswDOGxnSlpahXya = "uMizXjb"; 
short dIYxahZObWWlzPXQfhGzdsARqGo = 20199; 
double htkSDPpwlhbtVBdWVWlJRcxTSLhg = -4.879099E-08; 
htkSDPpwlhbtVBdWVWlJRcxTSLhg = Math.Ceiling(Math.Sinh(-5));
Console.ReadLine();uint tehGfHmozwJ = 138958; 
string YWSEzRHfPmdSdTmJnESNbRPcHWdlp = "etYJIMOSbANYstcPiwkMuWomY"; 
sbyte PXDOwQ =  119; 
sbyte UwlCVxCtaczxaRNIaExidL =  51; 
uint YVALqhukqVOjKRYwWmV = 70230813; 
ulong PfumD = 7722017013130881; 
string JVdlJWZzpNyOhzQQUjPK = "XVSlKMdzOUysnEZGAgi"; 
ulong dAekAKUVbNMscMZlZYjV = 6275863025677631; 
byte cGUYTxNajbiDMh =  181; 
string RatAhZUH = "HdQHPeLGKfzeTEzZO"; 
sbyte yQKpwIdPRWUHZsqPKkEmdSg =  -8; 
int gYyVBuRgiMXOwYMpU = 78472383; 
while(gYyVBuRgiMXOwYMpU == 78472383){
gYyVBuRgiMXOwYMpU = 522831;
}ulong PjVJGKkLTCesVxBQSiGRWPGB = 63319602064159015; 
long MjVzDc = 86514892093145890; 
byte GEVXTZFwUImCFQhHKPLqV =  98; 
int fTnjTRaiwxyQDMnMGA = 917037; 
if(fTnjTRaiwxyQDMnMGA == 458834){
fTnjTRaiwxyQDMnMGA += 726776;
}short niE = 29202; 

}public void lYqRZMwCdIefhsseYCYMwQK(){
ulong wCVTERGzJlRkUnakYBnLeptWVoI = 79500203619031052; 
byte IAQAHAgBTdnXBxox =  20; 
float iZYnj = -5.339423E-20F; 
short EJZJRLpbylyQlwbltHLWgSzd = -618; 
string KlHzuiqXkeGgQgnY = "uoWEWcQtDuJMjViQjGPRNcIUzQ"; 
sbyte BUPTUGKkeWOjRfSPJkaixqG =  4; 
uint fqIbff = 44999480; 
float FPSElYLnRGaIkSlmQYqi = -5.928474E-18F; 
ushort BsDcQJnOlkjUtyXyHsKSgyg = 52014; 
ulong pWLLsptfdTxJyiqlwaIqUtaTDX = 75998367248767834; 
short dCSUBmtebXUB = 1221; 
double ggAY = 5.786716E+32; 
ggAY = Math.Ceiling(Math.Sin(2));
try{
int ORHZWSFc = 4141436;
if(ORHZWSFc == 41275){
ORHZWSFc++;
}else{
ORHZWSFc--;
Console.Write(ORHZWSFc.ToString());
}
}catch(Exception ex){

}byte xhKNtytCuLCN =  38; 
double IqXZZEsIOQZC = -8.146208E+12; 
while(IqXZZEsIOQZC == -435717.6){
double gUHzzcItBWTgnfJywmPd = Math.IEEERemainder(3, 4);
IqXZZEsIOQZC = gUHzzcItBWTgnfJywmPd;
int[] WqTUpXRHnMCyqC = { 2522586 , 54865 } ;
Random CLnnTguOERXiJMHfkW= new Random();
Console.WriteLine(WqTUpXRHnMCyqC[CLnnTguOERXiJMHfkW.Next(0,2)]);
}sbyte NdEemTCE =  -70; 
sbyte LntmIdmCEodtOiUHHpBeYwqXjKP =  97; 
uint ZGRnYznSE = 6598210; 
string UKLPWsmUnMbVpWxcXo = "bqpzDAjxtPMH"; 
string SymAUSCnsMTPtICRPGFIZlFlioMWU = "CeXXz"; 
uint EcxROZE = 756874; 
sbyte UCagcxYGTaqGfGtmj =  70; 
float icIkEhQlmaIwij = -2.726471E-11F; 
sbyte cCYSGimyqkuMuQ =  108; 
long RTPDVGNtTtWx = 62331764296826562; 
short AYfQGRBHSzyQjzp = 22999; 
sbyte lANPpawHtFFdHncuuQyeaVSmCyZ =  -102; 
ushort SWFukRKKIewYzWbOS = 10719; 
ulong eAwcUpHBnTaomRNIAldkKLis = 6214565810833739; 
short ysCCAVjjwPefLHjtoG = -26105; 
int uqcuST = 28792964; 
if(uqcuST == 183350){
uqcuST += 970018;
}long ugYMhaAotnEqQ = 10440477720487996; 
string hHbzVUBUhcmPmpT = "DHeXPQCcZydoyklquIRefDFwTtxn"; 
int GhePlRYcKqfZAzRkyXRWwFAPRZbK = 42; 
while(GhePlRYcKqfZAzRkyXRWwFAPRZbK == 42){
GhePlRYcKqfZAzRkyXRWwFAPRZbK += 59447;
}float jJYusqYAZgcgFJVzyIBaIpXlQoY = -3.126071E+32F; 
float xHVaOYxEeHaPKXUe = -1.33163F; 

}public void PRDRmULNEdsLEgHHAJIgBbewtIC(){
byte VjENacJUQJ =  33; 
string SBNWxIdpw = "OtwONgyFsyyJTP"; 
double eYNKaCdoiEJylTjYB = 3.932063E+22; 
while(eYNKaCdoiEJylTjYB == 5.783839E-18){
eYNKaCdoiEJylTjYB = Math.Ceiling(Math.Acos(0));
for( ; ;) {
Console.WriteLine(1.500987E+29);
}
}ulong ZqWN = 43722440135238621; 
sbyte QqDeAJMosLpsob =  3; 
double WOcEtABSGHlOB = -4.866826E-26; 
if(WOcEtABSGHlOB == -5.018193E-11){
WOcEtABSGHlOB = Math.Ceiling(Math.Asin(0.2));

}float gEoVaoK = -1.757569E-26F; 
double qYcFZBAsKAwcBdZMQiJTDUKmKO = 1.488747E-10; 
while(qYcFZBAsKAwcBdZMQiJTDUKmKO == -4.764815E+29){
double mTsuNeNhKMBPSaDTZBeqyoQU = Math.Log(1000, 10);
qYcFZBAsKAwcBdZMQiJTDUKmKO = mTsuNeNhKMBPSaDTZBeqyoQU;
Console.Write(qYcFZBAsKAwcBdZMQiJTDUKmKO.ToString());
}sbyte DnLw =  -80; 
int CMnoLnLOfpK = 7996; 
while(CMnoLnLOfpK == 7996){
CMnoLnLOfpK = CMnoLnLOfpK + 777601;
}string SPagVUdWxegRLjnUFKPM = "skJnwnqLEAWuDNBBFsuIsoJYAYi"; 
uint ueZnNAFsSBHMkS = 21376485; 
ushort yOuaque = 36069; 
string KPxfPnjuWfQmViMSO = "gydOcQMOctOFjjwdIddCWXIlopt"; 
byte lnAHjxHzWNHtblQHqWGRSBJOm =  246; 
sbyte RAMxCVQenYZOPGLpyhVMRFXt =  67; 
short LGMulmSHybjSgwMstXRcmQsqedyXm = 2422; 
short jVeADFOBXZmompOqfmFxW = -13036; 
ulong mwohQUuVJCIkplQOipfnDddPogS = 71945404656610069; 
sbyte HLdFsHLIEaU =  -18; 
ulong HkAsjqBTKc = 42034084960838392; 
long DFbJkytgLfYEgIf = 88289368685996845; 
string tTVUCUUO = "MbiQhMYthEeR"; 
float KesoXS = 6.905297E+21F; 
int yRE = 245154; 
while(yRE == 245154){
yRE += 790138;
}ulong eCTKpSwGzWuDNlQjFXWIWyhtphB = 22957821900170421; 
int FHZnjfedoRkxaHgT = 96; 
if(FHZnjfedoRkxaHgT == 506883){
FHZnjfedoRkxaHgT = 391227;
}sbyte oIgWsBDImLaq =  111; 
ushort scoaqpKYKUiUlXiYkzIYCYi = 47100; 
ulong EjoHQWNkoq = 14154018897863360; 
sbyte qKdMQTowktUEmdNBJdd =  -65; 
sbyte JSO =  83; 
long uSDfDhWTfERXHqZVxIg = 28531026237308709; 
short lYY = -18228; 
long sRO = 40533057478350578; 

}public void QaNwfLUqoSjhGJFSTGS(){
long pHeIGaFxKWfSpJqnRSnqqNBHHxyDa = 15164030184875196; 
ulong KoVFVdpoanEcOEuH = 45340897914684521; 
sbyte MxJhXHdqEbxhGEq =  -96; 
ushort yzLQabXmkM = 21126; 
sbyte BwzsTHSqX =  125; 
ushort tkw = 33754; 
float dgOYLNVwIjLwqayOVMhky = -27667.1F; 
double znQIQDMqbnZACKLy = -3.925701E+36; 
znQIQDMqbnZACKLy = Math.Ceiling(Math.Sinh(-5));
Console.ReadKey();byte KhbIyuJdpVHX =  229; 
byte HACy =  153; 
ulong pOGxAcbGpFiwpsUJBxjkOG = 34620737788854655; 
string DYSmcaUGbKFcOAWLNRab = "GXKCjKXxlyPEB"; 
float AIHBCFidbjIn = 9.80485E+30F; 
string CZmWbiLTpXiP = "QzmHFlfkB"; 
uint yTIsCxpBjfmFnWpqWNjFwtSl = 39511775; 
int KXh = 49; 
while(KXh == 49){
KXh = 512687;
}short UHJGgDUZKnV = -21682; 
ushort VwdbxIhPoRm = 62855; 
byte RyhQF =  110; 
short NOLJQwWNOOKLFbwgeKjfqRsiqPim = -24911; 
sbyte abXIppOSMSCdznBBPb =  -122; 
long mjGVwCqamJHJjKKHteQpSymQOUV = 32150813936752094; 
float MVOxRAiVuoE = 1.210825E+28F; 
ushort kwcQWQhBHAMTyCoBWSpIjbTaXdVt = 39739; 
short xYo = 25308; 
double XVBFuhXfGMtfOIXEONiC = -2.836834E+26; 
XVBFuhXfGMtfOIXEONiC = Math.Ceiling(Math.Cosh(5));
try{

}catch(Exception ex){

}byte UinqbxeHQRAVaeezGxQyGDmnun =  28; 
ushort VOTIRfFyGmicTKCbiFosmbAeQmsWZ = 61942; 
ulong LIUdGnjYAbZ = 36374880182781198; 
long bNTnOSENIzJASqlQzwVwpSlbp = 83607907413333634; 
uint VaPSxGlREqKPdBcmQomAi = 840904; 
double xdmxLWwqIqP = 369714; 
while(xdmxLWwqIqP == 3.079644E-32){
double IsayZXxRaCuiRBZCYwSw = Math.IEEERemainder(3, 4);
xdmxLWwqIqP = IsayZXxRaCuiRBZCYwSw;
Console.ReadKey();
}uint gGQotiWSix = 62818408; 
ushort tCbCRVAhYfVKholIMip = 11148; 
short cRXAhkIabTjLNwEpl = 6293; 

}
} public class YHKuJbHGBKDqkpRzkEqNoBXd{public void AxDQ(){
string XLupVsynzCFgpA = "YuWWtMUSByJHsiFeP"; 
ushort FHRaVgNpTl = 45330; 
ushort EBCcHULJcTUSlMfeUxPL = 1777; 
float huaQGxbGQdbdSIDyVLGCNXhKuIk = 7.197148E+22F; 
float GJFtIHdflbmdPkJmysYUZ = -4.111007E-11F; 
sbyte mpwYlcQV =  -30; 
ushort nnURSGuABTsAM = 25514; 
ulong QaKkoxiupIgaZxhwPnZi = 26091471419031844; 
float JFzwZVKkeRfBCjztBdLXKzuGj = 2.833004E+34F; 
short xOSAzYD = 2406; 
int mLFihcWHTVIsFG = 145898582; 
while(mLFihcWHTVIsFG == 145898582){
mLFihcWHTVIsFG += 221169;
}short ubJdmbckWSXHzlfolDnceuCdimCq = 17384; 
short sfopDAeQVEIZQwYgLcVXq = 30129; 
short aBGUaALyEiFBjWDQhMszxuLIkf = 21091; 
short wsuRwpcTjQxSkxIQyXPAfQFGQ = -4736; 
int YKVJxFiQkJOGAWYWWGlYdRNh = 78; 
if(YKVJxFiQkJOGAWYWWGlYdRNh == 764164){
YKVJxFiQkJOGAWYWWGlYdRNh = 779972;
}string KmVBUOxGnRGOjjajRIKkkIMgWNHIS = "cxIOVmazbijGLllZYS"; 
ulong OfeRPcBPEhgOOLOjgw = 18041921134005658; 
double VXOPToPXqRZSMlUU = -5.48178E-08; 
VXOPToPXqRZSMlUU = Math.Ceiling(Math.Sinh(-5));
double Djf = -3529096; 
while(Djf == -1.799568E-31){
double FllOgwRwKCcd = Math.IEEERemainder(3, 4);
Djf = FllOgwRwKCcd;
Console.ReadLine();
}sbyte Kyx =  -56; 
byte nBWiLHlhFlCHiGQipOp =  32; 
sbyte KiE =  -85; 
int DHTlmeFiFhswhxMPGu = 53; 
while(DHTlmeFiFhswhxMPGu == 53){
DHTlmeFiFhswhxMPGu = 644792;
}double ymQO = 1840.983; 
if(ymQO == 1.682901E+08){
double eaHEhxVYLnVDie = 1.713543E-12;
ymQO = Math.Floor(eaHEhxVYLnVDie);

}long NGIBzVBylh = 34005724735349997; 
float yGeqAkknqpRD = 4.900911E-15F; 
byte EEGRyyZptqBD =  247; 
string dEqQgBpanKmgVpDMzShyIAcVZ = "PYiKJpmANYCAeSnDsgk"; 
short ZQGc = 13540; 
float iFBwQBMqSFbWTxDEkYNp = -3.0126E-32F; 
sbyte KJDPiDHXAGpizfJFfWLNenyHylw =  -109; 
double yexFMhXWioXeyngt = -3.023726E-31; 
if(yexFMhXWioXeyngt == -2.142461E-24){
yexFMhXWioXeyngt = Math.Ceiling(Math.Cos(2));
object qiLbOqXaFlgudzfVGmMOCaBY;
qiLbOqXaFlgudzfVGmMOCaBY = -1.3812E-15;
Console.WriteLine(qiLbOqXaFlgudzfVGmMOCaBY.ToString().ToLower());
}double eGyTEalpJNyX = 3.201989E+32; 
while(eGyTEalpJNyX == -2.254619E-08){
double fuUgJjVxZJiTlWQRXnOZqqOoh = -1.172404E+08;
eGyTEalpJNyX = fuUgJjVxZJiTlWQRXnOZqqOoh * 2;

}byte YJyIKadOIkPJZ =  250; 

}public void bkFXNCmxN(){
float WpQblllHTicgW = 4.061706E+30F; 
int KsjJsCKNZpzSLmKRodOXGZDytfd = 72264792; 
if(KsjJsCKNZpzSLmKRodOXGZDytfd == 844924){
KsjJsCKNZpzSLmKRodOXGZDytfd = 922262;
}short wIMEPgolWNOiWtpHChHnLP = 15010; 
short yLKqXHWsB = -14123; 
sbyte AZRqNHLFWhFObRLoeb =  29; 
double qqxGVZgRRwegdc = -1.662637E+24; 
double fVogWInAJnfh = -2.137283E+15;
qqxGVZgRRwegdc = Math.Ceiling(fVogWInAJnfh);
 bool? IFOfNBHXPDzgdyUMacpKHTjjEV = new bool?();
IFOfNBHXPDzgdyUMacpKHTjjEV = true;byte wkuthG =  24; 
string PuEDSfPZU = "cVYW"; 
ushort QBLwoBRaxAjzFMVqiUMdtJKA = 32184; 
short qgkynXEqeuHWnD = 4803; 
sbyte VpwihMMektyh =  82; 
byte YdATkcWCawQWDXzBPDsFiVojwzt =  17; 
int jnjLYj = 69; 
if(jnjLYj == 979410){
jnjLYj += 781578;
}ushort qPjgMTIFAGNcPozNKSYTkDN = 55208; 
double xeIbGuACTG = -3.668082E-22; 
if(xeIbGuACTG == 0.02475129){
xeIbGuACTG = Math.Ceiling(Math.Atan(-5));
object TdUJSBxAF;
TdUJSBxAF = -1.224019E+08;
Console.WriteLine(TdUJSBxAF.ToString().ToLower());
}string OGpqpCuIZzXxFnLO = "zRnUbPbdJzZ"; 
short jlpeSNQadiOYDKZE = -12978; 
byte PzmGOGMQnmHd =  100; 
short nMgBwYYouJAVzfcB = -8782; 
byte CbXHDzfXDOxftHcwmyQQ =  99; 
float SXnKmLk = 3.482931E-18F; 
ulong QzqeEbobwCYDfoxKPcEymKT = 70061825150473398; 
sbyte HRFeiUOBtw =  38; 
float hwRaxLLsBfGlw = -2.83422E-18F; 
ushort eBQnAyRGHiQXTokkogsblVpy = 61996; 
sbyte ZSNFAeEphdgIYawPCVwHQu =  -123; 
sbyte WwDjjsp =  -30; 
ulong NTFpSpYdgizubRfODCzVFRHwE = 50980526750754189; 
sbyte WXFDqRqUXLDtMewQOVRm =  107; 
byte NioSfwt =  48; 
sbyte ijRXZhdnXKLbpobCVbtwVMKOSt =  -115; 
double zEBuBEVF = 1.38561E+22; 
zEBuBEVF = Math.Ceiling(Math.Asin(0.2));
int[] lFJWN = { 6836064 , 65493 } ;
Random pgAchIXl= new Random();
Console.WriteLine(lFJWN[pgAchIXl.Next(0,2)]);uint wCOiKJ = 501321808; 
ushort ednKHzfmdc = 45335; 
short XgbWuCUBZnFLwndVZYmMbc = -5038; 

}public void ybzgxFAxcPQcIMDYCOfQJYk(){
byte sdd =  112; 
int VLmztcOduOiIGybmnxnkn = 610038797; 
while(VLmztcOduOiIGybmnxnkn == 610038797){
VLmztcOduOiIGybmnxnkn = 268133;
}sbyte BxZxe =  105; 
short miWNLLcBpFOae = 20185; 
long wxXehZE = 80813790036816106; 
uint SamqyEmEFikBeaKPQbIYm = 2698; 
int eMQLqed = 3237; 
while(eMQLqed == 3237){
eMQLqed = eMQLqed + 839890;
}double KCERtnFXjDtjTExGMIitRySmdg = -1.335807E+20; 
double MJymUBOGhYV = 2.930631E-33;
KCERtnFXjDtjTExGMIitRySmdg = MJymUBOGhYV / 3;
try{
int dNJFbmJpNlZQiohugdpL = 5701711;
if(dNJFbmJpNlZQiohugdpL == 78416){
dNJFbmJpNlZQiohugdpL++;
}else{
dNJFbmJpNlZQiohugdpL--;
Console.Write(dNJFbmJpNlZQiohugdpL.ToString());
}
}catch(Exception ex){

}sbyte jIOsuMZxZaFp =  -31; 
double zBe = 3.957278E-15; 
if(zBe == 2.141331E+07){
zBe = Math.Ceiling(Math.Acos(0));
try{
int OEdqeLBNFSAAhRcCciYbDCW = 5990031;
if(OEdqeLBNFSAAhRcCciYbDCW == 26502){
OEdqeLBNFSAAhRcCciYbDCW++;
}else{
OEdqeLBNFSAAhRcCciYbDCW--;
Console.Write(OEdqeLBNFSAAhRcCciYbDCW.ToString());
}
}catch(Exception ex){

}
}uint SxSIkmlXkhyVqTjlIOcTi = 4855; 
string nonnqWPHpigKnec = "FWfcAUpKpdRkIBzXOjWXYJTlInbLd"; 
int KaTFztbYtZEYm = 57; 
if(KaTFztbYtZEYm == 699186){
KaTFztbYtZEYm = KaTFztbYtZEYm + 188801;
}double heaVc = 4.587683E-08; 
if(heaVc == 1.257918E+07){
double kPVZEMlE = Math.IEEERemainder(3, 4);
heaVc = kPVZEMlE;
 bool? gdolaUHoCVSJwAHMAwYj = new bool?();
gdolaUHoCVSJwAHMAwYj = true;
}ushort pWmFdfAdDxBdiLespmy = 29904; 
double lzZMpPSRldhqcnHfDdsCFjK = -2155576; 
while(lzZMpPSRldhqcnHfDdsCFjK == 2.389265E-15){
lzZMpPSRldhqcnHfDdsCFjK = Math.Ceiling(Math.Tanh(0.1));
object wkIBETBknIpTYmzNXaGdOnR;
wkIBETBknIpTYmzNXaGdOnR = 945.4692;
}sbyte RKAfzfpdD =  -91; 
ushort XWAYguwgXVCbiVHKCkOEQN = 50762; 
ushort ishdDwSQtPN = 44598; 
uint pOapWhyXEAGDqtRWkhfJYbmduAK = 7926; 
sbyte acqyPPxSlSkPIzaezYxEFEQWoB =  112; 
double zlhyGFBaWbgPpTR = 1.725672E-21; 
while(zlhyGFBaWbgPpTR == 9.349164E-11){
double DnxluinSAuNGc = -4.379892E-23;
zlhyGFBaWbgPpTR = Math.Round(DnxluinSAuNGc ,1,MidpointRounding.ToEven);
try{
int dDWpPoKYicVsDcnZQ = 5103045;
if(dDWpPoKYicVsDcnZQ == 94165){
dDWpPoKYicVsDcnZQ++;
}else{
dDWpPoKYicVsDcnZQ--;
}
}catch(Exception ex){

}
}short HmyLLlyPibAtENzi = -8752; 
int MFJNfswZuIxOsFHGyxSaI = 875583998; 
while(MFJNfswZuIxOsFHGyxSaI == 875583998){
MFJNfswZuIxOsFHGyxSaI += 619223;
}sbyte NWZUnGUYAqsoldhNm =  -91; 
ulong hbMCFbTLBMRMmzHLxTeYNHXfALwQ = 40763655426747048; 
byte plNiyOAHcNxxpud =  206; 
byte dHbTGcZeYLUaeVBY =  145; 
float EjjGSJJtp = -7.197812E-10F; 
string bUI = "xxGKjsQPMbJCVqQkFH"; 
short bjkXKJYFHTZjmXDhlDtRtQgHZRu = 19054; 
int ywENDmkJZBtkiSczoYOJYcnABk = 57; 
if(ywENDmkJZBtkiSczoYOJYcnABk == 287392){
ywENDmkJZBtkiSczoYOJYcnABk = ywENDmkJZBtkiSczoYOJYcnABk + 189239;
}string xFHAupWEDTyhInFBDwbUUwg = "mqjHK"; 
sbyte jjuxzSPFy =  60; 
ushort wbsEPZyoTigUbyDm = 3605; 

}public void oTuHnY(){
byte fJOREhAVMcUOwImSsbdAwbpDC =  82; 
ushort fOOupXTOCbFZwgLA = 23841; 
short IHDolVOxetkpmLYQYnAyzO = -1688; 
short xeeopQjlGQU = 3219; 
uint ACBPwqMqnxpMkTRnT = 24; 
double RJhODbdIUemAMaVXYdZFeQJY = 2.122083E+07; 
if(RJhODbdIUemAMaVXYdZFeQJY == -1.647236E-13){
double NJTsaToJKusdmzTVpV = 3.937916E+18;
RJhODbdIUemAMaVXYdZFeQJY = Math.Ceiling(NJTsaToJKusdmzTVpV);
try{

}catch(Exception ex){

}
}float EauwafoACSLXUDxoGLnTRohnXKU = 4.84974E+34F; 
long wgngWbap = 81119405259103295; 
uint NchVXeoSfzplBCSHpQGwAn = 391789; 
uint iuBXTWtckIdBiiSeVDZECfcaa = 1365; 
sbyte zyNxfWUqh =  24; 
ushort ypkEPIyEwscwVpdsBXGPoQNjp = 5621; 
string YNOXGLdQS = "lGKCRfwsBQSDRk"; 
double EHEamLFjGIFePjmnAd = 1.917259E+37; 
if(EHEamLFjGIFePjmnAd == 2.127225E+37){
double muwWEXbhymRjBaBWiBp = 2.676283E+21;
muwWEXbhymRjBaBWiBp = Math.Sqrt(3);
EHEamLFjGIFePjmnAd = muwWEXbhymRjBaBWiBp;
object cOzaAQIWlogsPjMbhOc;
cOzaAQIWlogsPjMbhOc = 8.663E-16;
}ulong ziPnNEuUnbUaRPN = 63643091832598349; 
byte aKcYRpUdtIpGaWlZmQWIqY =  135; 
float BaM = 1.352127E+29F; 
uint ECNTqWxMZWzhxnTLORpgmEwbijXpo = 15; 
uint qczjsI = 3788; 
byte uBuxNIoYeEuMZgLLhYgx =  213; 
ulong bflwCwjYNcVaCONkJZjcVMGcqzHBt = 81300871304645739; 
byte DVFIJegjWwOyxIMeoAidcOK =  156; 
uint dubwwKxlIh = 790; 
double koLfGQsjUxdSsiLOkYhwaNiBZdbj = 522224.6; 
double FaKVUA = Math.Log(1000, 10);
koLfGQsjUxdSsiLOkYhwaNiBZdbj = FaKVUA;
for( ; ;) {
Console.WriteLine(3.382851E-13);
}string uHUzAVNKjW = "olWEiptzcQkEaKRa"; 
uint KlEn = 915380392; 
short iADMxcHJhARfXqdQYzjdUPJU = -14773; 
int LWatbmtgUWpthWqD = 1805; 
while(LWatbmtgUWpthWqD == 1805){
LWatbmtgUWpthWqD += 164970;
}ulong uMmWtRJyQD = 8118786866425274; 
sbyte JebhDlqLVBhCjMYoayOdmSEGZFx =  -74; 
ushort ChZSGpLetgAXDowu = 36311; 
ushort CSgxfH = 52736; 
uint hYQfsuZaDqfJEV = 62; 
int pokFbcdRJw = 87689083; 
while(pokFbcdRJw == 87689083){
pokFbcdRJw = pokFbcdRJw + 296870;
}double qtGiouSknzXXc = 627.4835; 
while(qtGiouSknzXXc == 1.994086E-24){
qtGiouSknzXXc = Math.Sqrt(4);
try{

}catch(Exception ex){

}
}
}public void pjEkNTEJnLELQajNYwZjXqJEsDUAz(){
uint SSYnAwiNzfKYLlwjxymmYq = 5; 
double aSkZekOZMTRMKqQIC = -4.418049E-20; 
while(aSkZekOZMTRMKqQIC == 2.144363E-13){
aSkZekOZMTRMKqQIC = Math.Ceiling(Math.Cosh(5));
int[] qDAbAjLBCtKNVxp = { 8252020 , 41347 } ;
Random YPGDJVSCcZBlldUyMWAfbdOjm= new Random();
Console.WriteLine(qDAbAjLBCtKNVxp[YPGDJVSCcZBlldUyMWAfbdOjm.Next(0,2)]);
}double ooQxKRgKxMxKXZAJAozzmbZ = 9.611914E-23; 
double TPNKLQCmNOojcween = -3.267504E-17;
ooQxKRgKxMxKXZAJAozzmbZ = TPNKLQCmNOojcween * 2;
object BBnpTlVXfmgUWRilPYKNlLzMPUQK;
BBnpTlVXfmgUWRilPYKNlLzMPUQK = 1.746797E+19;string CYwkgqsGFwAUNwgef = "cYEzOlOXotxAQDlXSTiwiqiDLMe"; 
string dxZKQ = "dYcjjyjcZieFWJPDmWLOMPa"; 
ulong hOSk = 35387369824730297; 
long YYnjuGghspBicZcanXohbKsRcMoFg = 61513641457258785; 
int hWawCKOuQXQVcCCXoHyh = 32118190; 
if(hWawCKOuQXQVcCCXoHyh == 867128){
hWawCKOuQXQVcCCXoHyh += 493377;
}ulong fnnDIpglbnQgnqAIqF = 31337153140403795; 
double uZCcWjoDKmmMuoWaRpTqRUKoM = 12.79757; 
if(uZCcWjoDKmmMuoWaRpTqRUKoM == 2.004418E-30){
uZCcWjoDKmmMuoWaRpTqRUKoM = Math.Ceiling(Math.Acos(0));

}short WGXJLObuPQGMqpMAxRwTktqnJOo = -21210; 
sbyte GPqeFKoIJ =  101; 
sbyte PWyINhiPInux =  47; 
int KaXzTlDjQu = 7279; 
while(KaXzTlDjQu == 7279){
KaXzTlDjQu += 171777;
}uint TFwQaahasGswUwbuFHMxnf = 7277; 
long CxgGSeTLR = 35040594562610045; 
double wBQKIFhyeyOsKiNAMUNNnDiFd = 9.735947E+14; 
while(wBQKIFhyeyOsKiNAMUNNnDiFd == 4.660314E-31){
double lNLMAaKHOndGjMmxGYgkETqlGfU = -1.27249E-22;
wBQKIFhyeyOsKiNAMUNNnDiFd = Math.Floor(lNLMAaKHOndGjMmxGYgkETqlGfU);
int[] CcPWZ = { 1737997 , 98225 } ;
Random ZwNcHOVBzqZutByeeptxzftg= new Random();
Console.WriteLine(CcPWZ[ZwNcHOVBzqZutByeeptxzftg.Next(0,2)]);
}ulong CDuPmIGEu = 12073028180032872; 
sbyte AEyYzEEqTc =  29; 
float ZJZVXkgMUULWtnbNYFlb = 3.387108E-16F; 
long FVkKq = 3348822607619716; 
byte KZEFJNbfpCEVyuRXKZVpbZywh =  96; 
double dhobLNCcgLaqEQVhNRdt = -2.324519E-13; 
while(dhobLNCcgLaqEQVhNRdt == 1.512835E+23){
double iyoQtbhmCjKSPRZYFM = Math.IEEERemainder(3, 4);
dhobLNCcgLaqEQVhNRdt = iyoQtbhmCjKSPRZYFM;
try{
Console.WriteLine(dhobLNCcgLaqEQVhNRdt.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}short MlcCyqwxIXcFlpNaEtkmxfNodz = -9637; 
byte lHAqHLCiJgoRdAbjDdfXpStRcyH =  123; 
ulong YYlDxpDmFGkRUmV = 41918013366536361; 
byte tnIojdcxkhqGCcMIoA =  127; 
ulong YfWHeZNIWFLZXdcU = 84883159211298121; 
ulong EaymVNAfwhMIfdoOoEGGEZJNp = 48944287450497334; 
byte NkKJYBSIViqoyT =  162; 
ushort YjmFqLWRPVbgguRSI = 35504; 
int UIeMqOFZOCMyTAtHaRx = 750096; 
if(UIeMqOFZOCMyTAtHaRx == 612680){
UIeMqOFZOCMyTAtHaRx += 832357;
}uint OAyPpdQHqeCmEgWcmjEmIbj = 816073895; 
sbyte SdyA =  99; 
ulong QAq = 85774510260968946; 

}
} public class StQPWxmnfNpFYkaXET{public void HyDpyMCPkiQYItlmKbo(){
uint HCUd = 73; 
ulong sgniWSgp = 80116194930722139; 
ulong lNXpVosDZWJmmZOFScCQ = 71867735063381604; 
ushort xFoCBamckZRtSjHRfdI = 4369; 
byte OYWK =  161; 
double mdlhwBdXLzNqQSDZsKVpQ = 6.643465E-28; 
while(mdlhwBdXLzNqQSDZsKVpQ == -1.921666E-08){
double RuntXWFkyKETwFUNujlY = -4.925015E+24;
mdlhwBdXLzNqQSDZsKVpQ = Math.Ceiling(RuntXWFkyKETwFUNujlY);
 bool? SKVNQZglGeItcxUGuhmlN = new bool?();
SKVNQZglGeItcxUGuhmlN = true;
}ulong WVDsikYYHlcRLyUH = 81958607984355764; 
int VcTZ = 62577787; 
while(VcTZ == 62577787){
VcTZ += 611869;
}long IjzFLcngQldJaaKYQCmLN = 40997769199403920; 
uint oVWVDqeKGEzmJZjYOXethsZ = 550958860; 
float AdFRtozPOefgILSHh = -1.125185E+27F; 
int VJJzJhGWVZsJOGGbMfYkkxTJQyJp = 85429703; 
if(VJJzJhGWVZsJOGGbMfYkkxTJQyJp == 250567){
VJJzJhGWVZsJOGGbMfYkkxTJQyJp = VJJzJhGWVZsJOGGbMfYkkxTJQyJp + 610119;
}int WsKgSSSfNzTelG = 169043; 
if(WsKgSSSfNzTelG == 728233){
WsKgSSSfNzTelG = 478485;
}ulong JlAokdqIwibjetwSssFOVsEEefyAQ = 8992686446168495; 
int POecLdcCbOcetUbdqIWk = 61; 
if(POecLdcCbOcetUbdqIWk == 596753){
POecLdcCbOcetUbdqIWk = POecLdcCbOcetUbdqIWk + 284906;
}double hKWkPLPZRpTAbFwMOM = 1.816364E-24; 
while(hKWkPLPZRpTAbFwMOM == 8.559275E-11){
hKWkPLPZRpTAbFwMOM = Math.Ceiling(Math.Acos(0));
object geSOLYnpgB;
geSOLYnpgB = -1.499926E-26;
}short oicX = 18835; 
sbyte IyAfcLqTCzXJhqeGpi =  -70; 
sbyte WFwIXdlmfKoxnQ =  100; 
string ajFEp = "HXjLjEMPDZiLSeyeHfGw"; 
ulong lUBjCkhbESGXOLOtbKtU = 53477381723551148; 
int XVTYMpkabCHCxqeU = 7391; 
while(XVTYMpkabCHCxqeU == 7391){
XVTYMpkabCHCxqeU += 928254;
}string VOPugEDlydULdQYyhaISmkbObh = "VGpwbSadXBQTzYwdsxtR"; 
uint TfdnkGLNatZVIGhJTTdMbXq = 19; 
float hKsIqghNjqNHuskByUDqPVJEB = 2.404005E+21F; 
ulong PWeqOEcJMIPATXsflKtQyUmHDcl = 1567694369064261; 
ushort oGPhRMlNRWYqzndwWzUJ = 29887; 
ulong sbUEdydbDWyXeFJGibUMfm = 77427674926225515; 
ulong BTyfXhhp = 41360812463263032; 
byte MIEdGesEKEBFKbjmxkgDIaTEYl =  212; 
short DCglAOeHymt = 17186; 
ushort FQikukOUPKLlLOa = 6686; 
ushort dnhctsjme = 40611; 
double OPmEoNPayeCINyBncpGqTfLgpCxQ = -1.957156E-28; 
if(OPmEoNPayeCINyBncpGqTfLgpCxQ == -3.589457E+30){
OPmEoNPayeCINyBncpGqTfLgpCxQ = Math.Sqrt(4);
object oVRtbApMwQlBZeWqhmN;
oVRtbApMwQlBZeWqhmN = 1.053388E+31;
}byte oXHHFssLyBRZVtImBhoM =  115; 

}public void dxK(){
sbyte MeeRfMyItBGiVAgxlfszgsdk =  -65; 
sbyte jmTkWzCHHsJMnzMnN =  100; 
double mqFRqsIKuaTFZ = -2.574456E-19; 
while(mqFRqsIKuaTFZ == -4.795187E-18){
mqFRqsIKuaTFZ = Math.Ceiling(Math.Acos(0));
object koatRCnWnJoqJEp;
koatRCnWnJoqJEp = -5.22052E-13;
Console.WriteLine(koatRCnWnJoqJEp.ToString().ToLower());
}string coUfZMVbgOWpiDVXVPAhQGq = "fwA"; 
long WQomUmbOyqBatFJAPLYMVJJhTYyYU = 52572340210937172; 
string pTizzYgxbI = "qLCZdnggtVQcUyNo"; 
float uLRMxJTMkkj = 246.2201F; 
ushort tKTLjZWRCKt = 33039; 
ulong aMKpfHWJFn = 89757722741552193; 
ushort BOTFHszUpbdicxjAAxxPwK = 5405; 
int dITVFcYgeOmSfKNictEkLdUTx = 209527; 
if(dITVFcYgeOmSfKNictEkLdUTx == 327611){
dITVFcYgeOmSfKNictEkLdUTx = 824540;
}long MDLFYMZebY = 58617021538969285; 
short uhaNZzgRbCNXDFtgc = 4658; 
string EgJflWQcznYYAiUFDXGB = "hdBkLubbGHcoKXnAUB"; 
string etNEPyuLOFeYDcxchUCD = "aTHVJQlGfIyDSAPpXwmZqna"; 
int NICkq = 97; 
if(NICkq == 83547){
NICkq = NICkq + 499123;
}sbyte gIieIXIxLDIMkznDyNTe =  84; 
double KwPIzbX = -4.820246E-18; 
if(KwPIzbX == -1.772617E-16){
double dVIbfTzgCLYxqHbQjAzJ = 2.083487E+24;
KwPIzbX = Math.Round(dVIbfTzgCLYxqHbQjAzJ);
Console.Write(KwPIzbX.ToString());
}byte SHpyIMkjxslcLsxdDtJucyWFd =  76; 
string OKQpTQqSPILRDEo = "dVQFPnBTDRPuLLtsLNwNDVjVaEo"; 
float SSzhbChAxIBiWOcaXgdn = -7.48365E-30F; 
ulong DshLVEl = 63822407178533888; 
byte SneVEGgh =  225; 
string kGpLcoGJtxFSxC = "MddKKyUdASKnYNaEaB"; 
ulong aUYWcHaQOnwItaTdejtYpDR = 13622139485628795; 
short NDRehIcyeolycUXTXzRh = 20800; 
uint dgEbafzVKdaHycY = 549570923; 
short lnFwpgJbuuNkRofNY = 5382; 
int sHtC = 364; 
while(sHtC == 364){
sHtC = 982916;
}uint RoljALmVqd = 50; 
double iqsoZtWmjGUdanSikwTztDcWnn = -4.70437E+16; 
iqsoZtWmjGUdanSikwTztDcWnn = Math.Ceiling(Math.Sin(2));
try{
int bzAxW = 4488089;
if(bzAxW == 40961){
bzAxW++;
}else{
bzAxW--;
Console.Write(bzAxW.ToString());
}
}catch(Exception ex){

}int EtJEwVlgclHMCmghciFnXkDggk = 9290; 
while(EtJEwVlgclHMCmghciFnXkDggk == 9290){
EtJEwVlgclHMCmghciFnXkDggk = EtJEwVlgclHMCmghciFnXkDggk + 882524;
}ulong iICXBgP = 18651135589722611; 
double nQtVsRDyjKpPsbNWudzGfJ = -0.0003966675; 
while(nQtVsRDyjKpPsbNWudzGfJ == 2.01426E-12){
nQtVsRDyjKpPsbNWudzGfJ = Math.Pow(double.NegativeInfinity, 2);
try{
int ZApxADdhCtMfetHFqVoEzKHeI = 3519136;
if(ZApxADdhCtMfetHFqVoEzKHeI == 28743){
ZApxADdhCtMfetHFqVoEzKHeI++;
}else{
ZApxADdhCtMfetHFqVoEzKHeI--;
}
}catch(Exception ex){

}
}float OCeJbSTGS = -5.493871E+17F; 

}public void moV(){
byte otiMMJAeQXZiaosNgnyYbhAEOe =  211; 
byte dyGMtdVLBcT =  154; 
int UUxShumfa = 254430; 
if(UUxShumfa == 893012){
UUxShumfa += 591715;
}double SmOMBtxHlxcHlFtyWFKq = -1.459397E+14; 
SmOMBtxHlxcHlFtyWFKq = Math.Ceiling(Math.Asin(0.2));
Console.ReadKey();ulong oceFDnuWoTN = 1927892402878171; 
short XsYGKMhSMeBKT = 24914; 
short jpGeMYLuVGzHHztnyVqKgZSCleEP = -17447; 
sbyte SaxjmliKY =  35; 
int enCkmLmmYDFQjydTHuWhR = 6140; 
if(enCkmLmmYDFQjydTHuWhR == 276868){
enCkmLmmYDFQjydTHuWhR += 126285;
}string lscDoOmLTq = "ZSnp"; 
ushort ZPipcFLxTKIVbYFjYMj = 34616; 
byte JQiOdTiYgpiEnqUYjOsgxeGPSsz =  203; 
float gVtefshnBRnUEsMyK = -3.468341E-15F; 
string wjgAFktVLutfFjnkj = "DQVnBTCC"; 
byte wJlA =  217; 
double MlGCgPtQyVPYizlBbOkHhJ = -7.809015E+34; 
if(MlGCgPtQyVPYizlBbOkHhJ == 1.635244E-15){
MlGCgPtQyVPYizlBbOkHhJ = Math.Floor(2.343401E-37);
Console.Write(MlGCgPtQyVPYizlBbOkHhJ.ToString());
}byte DKKPWjjuI =  240; 
byte sznTyJxXoOCUoSWgPylagfHuZ =  210; 
ushort qAc = 25094; 
long QgYbbsA = 74817284877630614; 
string kLkLlSUUHpuHQLhlhbQ = "NxTCSiS"; 
byte qnPmTSJ =  113; 
ushort odhuceIgdbFscyVSOR = 44988; 
float ZJMo = -5.099128E+08F; 
ushort SzPLyBeGbRxjbOjEZlqsbeRNEBow = 27516; 
sbyte Ank =  -73; 
int IGXRlNanJclzhpQobVOIbgWk = 19411085; 
if(IGXRlNanJclzhpQobVOIbgWk == 935510){
IGXRlNanJclzhpQobVOIbgWk = IGXRlNanJclzhpQobVOIbgWk + 916999;
}float IOqHuCPsRAUzdWnPk = -2.556208E-19F; 
short VOBJMhcQjqNlJRt = 31507; 
sbyte AfxQCmyhMVgFMIBfNZRXItNCIig =  -65; 
ushort VQcpOj = 811; 
string nkFxgKFFe = "EdtilmgsVitfkNGaqDxUqe"; 
long DMdXHRcEbDRbDIQdHRfVTia = 54254245306248094; 
uint GGAijMdgPdJVEz = 74174535; 
short AZNTuMlgBz = -31971; 

}public void RhtGWcqUyfbPe(){
sbyte WlIKoQWVzKNl =  63; 
double kGfu = -2.179118; 
while(kGfu == 1907.046){
kGfu = Math.Ceiling(Math.Asin(0.2));
Console.ReadKey();
}long mPLujkTkpmCGoUDENlNmbRLdY = 42215115948873707; 
int DCmShXHkhXWYkTZhwZ = 806831; 
while(DCmShXHkhXWYkTZhwZ == 806831){
DCmShXHkhXWYkTZhwZ += 425573;
}long qxEOVAMqacBu = 68042118635527023; 
long PFdHINgzTRgQGkIucylTT = 10219549290830809; 
uint kJIHYKnSZkCiK = 7866; 
long YhdFnPQS = 71582370329079416; 
ushort gXEJLOFmJqOWOGAkSuzQXUSOZIb = 24378; 
long GRnsIMHoyoZagAdbnHZgyDTGXmf = 87717488632547482; 
double wXOEEXjRWNsKYtwuBxdY = 4.640003E-09; 
while(wXOEEXjRWNsKYtwuBxdY == 8.748542E+24){
wXOEEXjRWNsKYtwuBxdY = Math.Ceiling(Math.Sinh(-5));

}long PjJhIGo = 19857379669317723; 
ushort IGdhGwmgU = 55321; 
long OAGlglWB = 9741933909337632; 
int KLhUDZGaj = 42287952; 
while(KLhUDZGaj == 42287952){
KLhUDZGaj = KLhUDZGaj + 598908;
}ulong uQVfiDUbye = 9777093817988887; 
double UXGG = -4.577132E-36; 
if(UXGG == -5.59183E-24){
UXGG = Math.Floor(-7.483278E+17);
try{

}catch(Exception ex){

}
}short YIDZ = 18145; 
string FzaDDOCfGkBh = "flxwldpaMibgnDpdRhQkTNguESZ"; 
double BqMiwXNkgNLLqTpYxokkcPdLXcs = -2.41754E-36; 
double pXSFESKueADUQxhDPMSLnbltG = -1.743004E-30;
BqMiwXNkgNLLqTpYxokkcPdLXcs = Math.Round(pXSFESKueADUQxhDPMSLnbltG);
try{
Console.WriteLine(BqMiwXNkgNLLqTpYxokkcPdLXcs.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}short qNKgifJGF = -4716; 
ulong hsEGBtLXgWwNWGKBLHhohkXSZL = 6750049467077975; 
float fPCexugwsWjEwhUYnB = 1.141525E+33F; 
ushort GQnkslYERpOgwWedSNeDAd = 33349; 
uint yaZEx = 911606656; 
sbyte iMQlcVxPfUPoN =  119; 
float IYSSlDpM = -1.616674E+31F; 
byte ZuwSXaNtnGLjgLXHNQKbftOWcqhHw =  19; 
sbyte KyPLzW =  -41; 
ulong BWcmJLCF = 31404605550606269; 
uint GTgXjzEAJVmhQzXRFyMKuGGb = 8731080; 
float BEieagMSdEMEKibGqCjRnsoiUPw = -8.6222E-19F; 
sbyte fHIZfNih =  42; 
ulong NbLia = 2358486133923441; 
sbyte ZjkMMFYsHW =  -123; 

}public void WTiuBeiF(){
short MPjxnuZjGaRSOYW = 28714; 
string OJemZbIgCRuWAEZC = "xCoqFtxpatdfjzE"; 
string QGtAQiOhTiQKBmTlBAwM = "joJWYJE"; 
string tQTRNtiYHtC = "LkGGBfkU"; 
long isAHpLDSBjgCPZQJBAcDScjZzecpy = 16518607941002349; 
sbyte zwPS =  -44; 
string wHPSFMR = "gghAhl"; 
long uXRZUwsop = 25365630425821976; 
short xBGXIpCnBtSYRQiwVBeoa = -15535; 
uint lonZdXnESDlMPZiKt = 84; 
double uGOYIntRaJhaeMt = -946.7799; 
if(uGOYIntRaJhaeMt == 1.020466E-31){
double yGVquWTwFcRJBbwJVMS = -2.225661E-29;
uGOYIntRaJhaeMt = Math.Floor(yGVquWTwFcRJBbwJVMS);
 bool? mQIRWoQjnqjBbsbxeQNxbV = new bool?();
mQIRWoQjnqjBbsbxeQNxbV = true;
}short pNdqoJqffpfuGIZWKPfKCKZSGI = -22789; 
short kayFqlCshymqw = 9374; 
ulong NilEwHMmHsGOkMIBYMUoJPowy = 34182863216658474; 
string AVPYFojYZHNgxQNsJNAhXTRgRe = "unlEEjShFjOoa"; 
uint pPKyfCfZyoLTWGeJhBVQLNcjX = 651556; 
sbyte VyAJfEc =  -21; 
ushort TnnPoSqWDBCGPfJgDMBACTYRCRljQ = 2563; 
string NBbCxYNqH = "fIIoJtZZbpVUzZoxmyGg"; 
sbyte yoTodgKEZiTTOqjl =  -84; 
float QaMqTccl = 3.556717E+16F; 
short ACFjsxtpTpyqMaFlYmbnNW = -12162; 
uint XZlbUKTJyusihRQPDqBEwF = 445051510; 
float NKbYDkfzwx = -8.032845E-17F; 
double EInHmcRwlAkGQVpshPk = 2.127569E+34; 
if(EInHmcRwlAkGQVpshPk == 0.2924514){
EInHmcRwlAkGQVpshPk = Math.Pow(2, 2.1);

}int QLBnGbtNIhaLVbWtfGHPBO = 6376; 
while(QLBnGbtNIhaLVbWtfGHPBO == 6376){
QLBnGbtNIhaLVbWtfGHPBO = QLBnGbtNIhaLVbWtfGHPBO + 515852;
}uint TpetXilFVqKECTmTSCAL = 68924378; 
long kbATXQESlZoLHCdOhBLoeWmsu = 77605991182646528; 
byte WJsIeOS =  85; 
uint CmpyMEhZwMnOhzATGQ = 982375; 
sbyte sthePS =  26; 
uint GLNTkHApl = 199678215; 
sbyte ThCUilhbUEAwUWoHP =  -56; 
ushort zJduGnmgBkgUWQAaDVKuskLhnzq = 45503; 
byte xEQodkehnVZbCJxFzdxoe =  226; 

}
} public class ZDmYWghXDWRPDdINoVu{public void OuqKbLJZmmUFDYRNQli(){
string YxfLUU = "QphtKnRFkEWkyhKob"; 
byte EmVXcNtAQ =  245; 
byte hQhjWfCUohlCmpXEZnhdyIqnUteGq =  199; 
string ZEOupXNiBsXwbuKNQPAD = "hCAATpDbVEBhFCJCDmcBYEfKV"; 
byte JsQCozQSbnVyIkhG =  202; 
uint xTbknDybSE = 800823; 
int QTKebkKOJHmHQupRXpm = 80427101; 
if(QTKebkKOJHmHQupRXpm == 139675){
QTKebkKOJHmHQupRXpm = 539055;
}double xYoGzqQRqzBOmEJkuXHMcMbGg = 1.93993E+31; 
xYoGzqQRqzBOmEJkuXHMcMbGg = Math.Pow(2, 2.1);
try{
Console.WriteLine(xYoGzqQRqzBOmEJkuXHMcMbGg.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}float ZVTxARfNnGUbEAoWoAVsEpUgEp = 7.123941E-16F; 
ulong eRo = 11718229898423926; 
string iepdpoHElIBpKb = "TjWQamBcMfNKccRhVAULbml"; 
int fzT = 78; 
while(fzT == 78){
fzT += 135922;
}short QHdjIcEXEGFBuAG = 20505; 
ulong LGYsQVOITXY = 44939736980771720; 
long ZJnb = 70806511167714919; 
byte MBzRiwaiHZUAJQ =  13; 
sbyte uemLWKpwtRjh =  69; 
int SAYRUxlpdRcnhW = 155717386; 
while(SAYRUxlpdRcnhW == 155717386){
SAYRUxlpdRcnhW = 943448;
}sbyte LQR =  -91; 
ushort FRaSuzHiRllViPiUUwiABJOE = 61918; 
long ROiaNaxLm = 78758283096310843; 
short PuSRVwZXoVcxVLlPwJkAVMdWeKC = -770; 
byte sElbUSZXmGOchQjtojyBoHYSa =  13; 
short MlTANzEAkTQuQyxGMSFnNMuscKD = 13742; 
int aRDdlaMnpKlLi = 371742; 
while(aRDdlaMnpKlLi == 371742){
aRDdlaMnpKlLi = aRDdlaMnpKlLi + 205527;
}ushort izbiaS = 35212; 
ushort VTURT = 64012; 
float lwUyDUccEYYZZLBlmStBVl = -1.097199E-24F; 
float OtQqDukYG = 0.03570912F; 
long EZgxTEyejsJFaGqRamK = 22327121801177458; 
ulong fLN = 7009765509598013; 
ushort EagNDP = 57598; 
sbyte CPcoyqUkzZKdeHWbORR =  -115; 
byte enjDIfATPUUAjYBWqjUERHKdztkM =  196; 
double euEL = 3.605855E-30; 
double jGyYBRAIiHgyFfdoUNeRSPeG = 7472164;
euEL = Math.Round(jGyYBRAIiHgyFfdoUNeRSPeG);
try{
int CfSlxUTTWmkWzwV = 6933536;
if(CfSlxUTTWmkWzwV == 24717){
CfSlxUTTWmkWzwV++;
}else{
CfSlxUTTWmkWzwV--;
}
}catch(Exception ex){

}
}public void tHDitxUs(){
sbyte CRF =  -92; 
byte NcXztdDxVwoTYEkBNHamxqaBBiK =  174; 
byte zYSUhZNO =  210; 
short fmXha = -8561; 
int adXgXzwcxYkuThMXpaewlNZQ = 16975220; 
while(adXgXzwcxYkuThMXpaewlNZQ == 16975220){
adXgXzwcxYkuThMXpaewlNZQ += 372553;
}uint eTmgBSUxMWpQGUlYBaRaojFm = 15; 
ushort hpaczwpZVGyzgIKoyijSXsEb = 36181; 
uint VoZbszfnwWmqjFlc = 564513969; 
short HAoQVXDRQNYBZVuKVEeaauntjS = -32701; 
byte oJzkGTwajgwVnyJXNsojCh =  88; 
uint SmlWHdXofCeB = 3723; 
string KMyJeNWFwxBLdjgpZTNHidDdzHhWF = "yYmjRxVPmfqHBjD"; 
byte WuOBxGxQ =  157; 
sbyte kyFZaYZCzUsLw =  -78; 
int oDDsBlxlDRWasRukmCFP = 341746; 
if(oDDsBlxlDRWasRukmCFP == 212943){
oDDsBlxlDRWasRukmCFP = 24908;
}ulong EJspQYgncQT = 85340750531940223; 
long gzTGRgXWahAzfITz = 74950028130025565; 
int hVxClSfRykALZRiqcJuAVLD = 40238825; 
if(hVxClSfRykALZRiqcJuAVLD == 944317){
hVxClSfRykALZRiqcJuAVLD = hVxClSfRykALZRiqcJuAVLD + 374773;
}double kad = 2.319746E-35; 
while(kad == -5.540794E+29){
double JZWkDCPfOn = 3.915689E-12;
kad = Math.Floor(JZWkDCPfOn);
Console.ReadKey();
}long nKuWCZCYk = 28304346944303869; 
int qQLpxdlpiXROwbmpDbIsnwWaHmQK = 757992; 
while(qQLpxdlpiXROwbmpDbIsnwWaHmQK == 757992){
qQLpxdlpiXROwbmpDbIsnwWaHmQK = qQLpxdlpiXROwbmpDbIsnwWaHmQK + 550355;
}ushort EuXToJsauhbWuD = 17515; 
long mIWULJjm = 57564689440184431; 
float ieOdFcVHAkTpbALNVUyCSLO = 1.112866E+18F; 
double ACBioDQfHxidUYeiYtlKWQgMYjpU = -3.656782E-16; 
if(ACBioDQfHxidUYeiYtlKWQgMYjpU == -4.711536E+10){
ACBioDQfHxidUYeiYtlKWQgMYjpU = Math.Pow(double.NegativeInfinity, 2);
object jVPzACMQahHNfVBVRueOKkTBCoMOb;
jVPzACMQahHNfVBVRueOKkTBCoMOb = -1.893666E+20;
}byte YPfMRMqEGyPHGQfxQcbAZXJT =  98; 
sbyte AsSjeFJZQ =  40; 
short OzikWludYjEllVKQbgm = -9904; 
long RkNmKcXRpOBxnaQ = 43476593172453752; 
int jUSYJwYVoIYyJDsfgUQLPHKaouHsI = 52; 
while(jUSYJwYVoIYyJDsfgUQLPHKaouHsI == 52){
jUSYJwYVoIYyJDsfgUQLPHKaouHsI += 492550;
}sbyte THsDB =  123; 
ushort nddIWhxZViEjSJoOGmefDpucj = 2035; 
byte yinIqiapE =  180; 
long fylf = 80792392698173489; 
float oYKazVUiC = 2.381675E+30F; 

}public void TKJYkUCbafdngkPdVTlaNdTcHDl(){
ulong XVBkaeD = 64864198884053291; 
float TmfQmJBUiPRGsI = 1.141668E+16F; 
float jEdg = -1.03557E-20F; 
uint AinURJijJZLsKmZpRo = 92334837; 
string ubZiapsNgTnKiqGnYhyhYJ = "BtX"; 
int oQBGDPPmVIJSSWSReaeILDq = 475893; 
if(oQBGDPPmVIJSSWSReaeILDq == 714377){
oQBGDPPmVIJSSWSReaeILDq = oQBGDPPmVIJSSWSReaeILDq + 13707;
}uint cMGyptJhuDttpAFpcRixtTWc = 48391542; 
double KcdmxS = 2.171378E-29; 
if(KcdmxS == -11.04121){
KcdmxS = Math.Exp(2);
Console.ReadLine();
}ushort nnYoLEczgwPGKedQkA = 21333; 
float RLmFopyutQHpxHhuqBJC = 1.030511E-16F; 
string ZxXUkNtQBXx = "MKxmGdNFiomzVwTZIs"; 
double JWGSsDiHyQQAjFIId = -1.321656E+31; 
while(JWGSsDiHyQQAjFIId == 4.664736E-05){
JWGSsDiHyQQAjFIId = Math.Floor(-7.832716E+14);
object qOlVlxmZkOW;
qOlVlxmZkOW = -0.01894891;
Console.WriteLine(qOlVlxmZkOW.ToString().ToLower());
}string zWMOUKchVbzBudKRoYJW = "xLEAEUYLmIXseFMWLwZPw"; 
int umblfthmYeZckhoq = 7285486; 
while(umblfthmYeZckhoq == 7285486){
umblfthmYeZckhoq = umblfthmYeZckhoq + 513014;
}sbyte AXVFDJhnSki =  -71; 
uint SqewZneDCOwejLYUhUfM = 15603284; 
uint KoXMjaYdPgkfEXCUkX = 131324; 
sbyte AxoXdogQVKTCLKq =  -34; 
short QsocBSpjskBQMqul = 20267; 
uint lXKWZlBlcmSERF = 328553597; 
ulong xWyITXV = 12879591963493018; 
double EgwpR = 2.082234E+09; 
if(EgwpR == 1.764847){
double aFctFZFdNbKhCLVNYNUAskuTG = 2.046801E+13;
EgwpR = Math.Round(aFctFZFdNbKhCLVNYNUAskuTG);
try{

}catch(Exception ex){

}
}short zCWZSRamWAV = -32523; 
ulong tQhMMtnMEQotdHCweNbUcsTNFYi = 59842460482524234; 
short HyYcMY = -25319; 
long NuMCwhYSSaXwgNk = 3614490143138341; 
byte ypdSquNcgk =  200; 
byte NemCDtqLAKN =  183; 
float DGzdgzlXSVaCHjsaDx = -1.067628E+08F; 
long RzgTVFXHpgARjxI = 63515814812432004; 
string gsAhqGlVePaWnEIalFTbPa = "suOtCTkAXghPlzAZBnWfIIkuixP"; 
string IYXgxgNDmLbTodQgOwCNFtO = "UnLjujedjNohjIPqNymEUltEun"; 
ushort mnh = 19065; 
byte lixD =  38; 
string YKDOJWgHkahfjZJVgPg = "cQVFaWgxGVjojXVCQQMsnucn"; 

}public void jbe(){
int oNanDhOXON = 70062909; 
while(oNanDhOXON == 70062909){
oNanDhOXON += 849596;
}double dgeqdPBJO = 4724095; 
dgeqdPBJO = Math.Ceiling(Math.Tan(1));
Console.ReadKey();uint ICdgPNLcdFnXM = 96; 
long YQHsXVwMNtBaIBSsVM = 74858771690949104; 
float inwjaheqTb = 1.052984E+08F; 
long btkcxdwGQpoxH = 57048252578507376; 
uint qZAxCzUbeDDxRsWKOTDyeVFCELQIT = 527874; 
float uUcXIUhWojUddOMyh = -2.980741E-18F; 
string Uww = "SQJmiCbLUyzUNgYwCPLLzAWekLnm"; 
float BzHZPdzKfJSsiqnEw = -3.029525E-16F; 
int eLLzsUEbznOTtWZm = 33108522; 
if(eLLzsUEbznOTtWZm == 855272){
eLLzsUEbznOTtWZm = eLLzsUEbznOTtWZm + 681800;
}string gqFRIofXmbxNWCyjVIIueAQun = "JRuIHwJSsbo"; 
ulong iiQGqBlplULbk = 64198201020374655; 
long EGwbediwVh = 52729938209106486; 
string WJGxtaeLCejdCU = "aWAhlcAQT"; 
double yAOcpCpkqUZYIdYyU = 1.062442E+35; 
if(yAOcpCpkqUZYIdYyU == 6.675114E-15){
yAOcpCpkqUZYIdYyU = Math.Ceiling(Math.Sin(2));
try{

}catch(Exception ex){

}
}short INJREFQBPSTQJo = -3860; 
sbyte YUc =  -71; 
uint pOxbEUdjXHtVtPeWpQbbb = 43; 
float dmBxhmjgdscgDmHLI = 6.955068E-25F; 
short dqWbElkzIhsF = -36; 
uint EOPujOpxbPNDuptmmDha = 573614367; 
ushort aZJxkLDcNZgUgPfFOU = 58355; 
byte JQinVecsAMatktkfTlqmKQwi =  122; 
sbyte xElaKiOsjAhoHBL =  -64; 
byte CVwVzU =  168; 
int fTuZPPHGOnLpjybbGMXLNZDhbXkcC = 35; 
while(fTuZPPHGOnLpjybbGMXLNZDhbXkcC == 35){
fTuZPPHGOnLpjybbGMXLNZDhbXkcC += 652484;
}uint BtAZUWZDRqzuTShLTlgpmXSF = 69224; 
long tkaPClftzLScDGkGMIIljImXBiD = 63441180340976192; 
int KSINPmhteNH = 22852015; 
if(KSINPmhteNH == 411800){
KSINPmhteNH = KSINPmhteNH + 446255;
}long XcntKDREpbPzxFocPaAuue = 71062277363998834; 
ushort uyJnwAXkXJSspokgXQguXpqS = 30730; 
float PLDDRGYeYWaHpdnyLmlfhKKskqbS = -4.263652E-29F; 
float EDhBwgaNqjmkWjRqKTGGlMPf = -1.298243E-23F; 
short FgaRfjdNVObFlAPX = -4248; 

}public void wbBKikBkEROcAkPC(){
sbyte jwfNfewV =  85; 
string NXSJCPqZUXPpU = "mZToQQLsENatJVxomtB"; 
float UVtYxyCnmuflgTeCHbkJwtfIh = -1.793597E-07F; 
int LwQcPtdApusidqsVhDc = 7542; 
if(LwQcPtdApusidqsVhDc == 296793){
LwQcPtdApusidqsVhDc = 156225;
}ushort QMQpcYJDZoiqBbcp = 41793; 
float qcNMaEtmSddBUtSm = 0.5171196F; 
string kwPtHYkCoLp = "yIezdZQMJLRzVOoXIOoRDD"; 
short mtfctRXgOsdmnujOKsHAFpyTKnT = 1830; 
ushort iUsjkKUOtSOKLEKb = 45073; 
short CgjWQqXWlseXwMojjU = -27068; 
sbyte TWZPMVGUiIEGXUsY =  119; 
int nNIwnUsXxtBhGfdJkkhexqcdLC = 4675; 
if(nNIwnUsXxtBhGfdJkkhexqcdLC == 224346){
nNIwnUsXxtBhGfdJkkhexqcdLC = nNIwnUsXxtBhGfdJkkhexqcdLC + 210886;
}short VNkBMVtXj = 342; 
long YAUkhORzxWLFKNCU = 7269293707461489; 
uint zmLAhCxkDSFHPzuTQZbHQ = 376281097; 
uint KssKGpENyoFNWPiiAjJoALKWW = 202691; 
short UATlJyjdBXDbTqiPgTkSzqQLj = -10834; 
double xOcfoWEQMCnqZeZmfnHu = 2.880445E-30; 
xOcfoWEQMCnqZeZmfnHu = Math.Ceiling(Math.Sinh(-5));
 bool? goZtfSDagEAXTyxmsJCJ = new bool?();
goZtfSDagEAXTyxmsJCJ = true;uint JawXek = 373430842; 
string XlPoZpDQaasqaccARsInTXDdwggbf = "HOETygYlsESXkAOqZsVGuEMOfZJ"; 
long SiCKftFNhGWCFadxWNdoegPgBC = 33239420475809210; 
uint pnffjFSNYdPEIq = 24; 
int PLuuiEpDWuhznl = 666383; 
while(PLuuiEpDWuhznl == 666383){
PLuuiEpDWuhznl = PLuuiEpDWuhznl + 964009;
}sbyte obhyMAuAEkSRDObW =  -105; 
uint IIyGBPDcZkBFHpoxtmMSQhyQ = 612193244; 
ushort NIjEgiySRwIIiywncGkBm = 22818; 
long tJZBjIKUsTpqbXzzTyFXG = 82202297621296412; 
double uZBkyiXJjMNpog = -2.286228E-20; 
if(uZBkyiXJjMNpog == -6.527142E+18){
double dpaHOaUtBwyVsBMbpVPulS = Math.Log(1000, 10);
uZBkyiXJjMNpog = dpaHOaUtBwyVsBMbpVPulS;
Console.WriteLine(uZBkyiXJjMNpog.ToString());
}sbyte THTCa =  107; 
byte ckCFmQktQ =  36; 
string bhuNaWJIDCJaVnGFJeu = "RaiIGmb"; 
byte zOHIBdMlgCemELCDCJZkGKMOUO =  6; 
double hNVxPdNbsujUsDDpWnoIed = 4.930571E-37; 
if(hNVxPdNbsujUsDDpWnoIed == -1.222396E+21){
double cNBZPiROIOBjoPqnhizYBMs = Math.Log(1000, 10);
hNVxPdNbsujUsDDpWnoIed = cNBZPiROIOBjoPqnhizYBMs;
Console.Write(hNVxPdNbsujUsDDpWnoIed.ToString());
}float IEJROPwSMZIhSJnpQmoI = 422.9475F; 
int zdXl = 9288; 
if(zdXl == 762353){
zdXl = zdXl + 655337;
}
}
} public class JJlEQlPywXmAjNOJQTkbfZHs{public void SjGQjVtjTGHEJAxQeiekF(){
ulong ynaTDHJJcUTYbiH = 61451876721958775; 
sbyte OlbQCgfeLiDib =  81; 
int WFKQCwHz = 61234525; 
while(WFKQCwHz == 61234525){
WFKQCwHz += 318248;
}short UAPuISB = 16145; 
ulong paebacNflDuqStFDSzPRIwTtoD = 50752315189190446; 
string tVcoCqeNPDIxQeZcXWkV = "MNybED"; 
sbyte xoinYjXwBoqpgEMV =  28; 
short dbZfyiBHiGmYOXm = -9574; 
byte hSVcHxIUGZiaDwk =  24; 
string yjMfyDOQmqDtTkz = "zBEauEyDEBzicS"; 
ulong TQVeMcusoRjpsmfXCgXKC = 25476703542548117; 
int wFTAdpuKxgkMEthkqwCHWVc = 80; 
if(wFTAdpuKxgkMEthkqwCHWVc == 357421){
wFTAdpuKxgkMEthkqwCHWVc += 559224;
}uint nKdOMzNbIhxBEqq = 501617; 
int kCDXRBDMKqnbgVKXcAGxNEtTABuAR = 239970; 
if(kCDXRBDMKqnbgVKXcAGxNEtTABuAR == 594668){
kCDXRBDMKqnbgVKXcAGxNEtTABuAR = kCDXRBDMKqnbgVKXcAGxNEtTABuAR + 302594;
}double MlnjMcLugoFTtoaUikHhKz = -8.533248E-30; 
if(MlnjMcLugoFTtoaUikHhKz == 4.343975E-23){
double GOlpnslaCJAslPGyjl = -9.364354E+17;
MlnjMcLugoFTtoaUikHhKz = Math.Round(GOlpnslaCJAslPGyjl ,1,MidpointRounding.ToEven);
Console.ReadKey();
}byte dSzAPAokBeKiEoRhPPJsR =  191; 
int EcfXXtsQa = 414679; 
while(EcfXXtsQa == 414679){
EcfXXtsQa = 540877;
}double PsaXZtL = -8.998576E+28; 
while(PsaXZtL == -4.653731E-05){
double lVNtsnhMTigT = 2.614455E+25;
PsaXZtL = Math.Ceiling(lVNtsnhMTigT);
Console.Write(PsaXZtL.ToString());
}ulong qdXNksPBZDOxfzzesCjmkNyFMKtP = 12081595412745511; 
long pSAER = 71151798765703670; 
byte BMFHtaBZZVAMFhHeNynkqO =  163; 
double CoxxgWkDzPMPendPTAIsXzo = 1.2977E+27; 
double ITSMeET = Math.Log(1000, 10);
CoxxgWkDzPMPendPTAIsXzo = ITSMeET;
Console.Write(CoxxgWkDzPMPendPTAIsXzo.ToString());short pGJdOsWLEmaSIWyedcPLp = 32636; 
long eKbpniyOS = 6324350854679381; 
string XLq = "hFCdiHHupXwYdpGyuQmZWOPOD"; 
string HlZLD = "SULNenhlqRMWCYuzHGhzsWt"; 
ulong BhCZhplSklxVKpCeNGWdSac = 20438612559918328; 
uint ZEfHliBAdqMXEWoHtFpxNWBfQRD = 668931032; 
ulong pVZLMwzCxxEOKpNGJfWMRGdi = 50199058764441882; 
short xcElDkVcUaihQPclT = 21276; 
ulong YKRR = 39844654125184804; 
uint ULbyDMQ = 72668952; 
string VmlOiNOenENkNEUQnWdjxKJVqkxna = "jYORNwjTY"; 
string IjEh = "nUJWDSITEkFpxYqA"; 
string uOxW = "zGuXagPBeORWCGdJqpVnCRTya"; 

}public void EEhjtesdjpiUHNPLBDGcPT(){
string czCJoINXXoEk = "VwNEzwOByJhz"; 
ulong pRXWKDPgUDK = 30187919681988551; 
short hTzpzlSGPtjzqyfZsUJ = 4892; 
ulong jULTkpBqZVyLhYAG = 62497333544113680; 
byte fXZoVP =  215; 
long FGFbDfyunaHogBNIsIlwa = 53376306063245744; 
string TBoUZHouxIPlnFRYxlHDh = "lIkkXWsCfKmsOPRGNbc"; 
byte OlggdolgfczCmnIKPKtbUCeY =  237; 
ulong gQA = 31002998006054135; 
float gwaHPQCBjWXsKlVmghkm = -2.01495E-31F; 
short yYIxQkGMQMVABzjgoCW = 24774; 
uint QJVlpVTHub = 39; 
byte ieLZqlDYpwMJYeXMiydjhZCFV =  175; 
float qUwgnsY = -2.790169E+24F; 
double pLSsAAHXPKJSIpZJi = 2.808701E-30; 
if(pLSsAAHXPKJSIpZJi == 2.081858E-12){
pLSsAAHXPKJSIpZJi = Math.Ceiling(Math.Cos(2));
try{
int zpHcenNeAIbNQoKPIKUVYsk = 3342967;
if(zpHcenNeAIbNQoKPIKUVYsk == 98956){
zpHcenNeAIbNQoKPIKUVYsk++;
}else{
zpHcenNeAIbNQoKPIKUVYsk--;
Console.Write(zpHcenNeAIbNQoKPIKUVYsk.ToString());
}
}catch(Exception ex){

}
}ulong eLEjbx = 81443230465991212; 
byte IeaKyl =  137; 
byte huqSEp =  194; 
ulong nRWSOzSqfZnDq = 76109890087546319; 
float tdTbeKp = -1.100761E-20F; 
uint cxkQqCSfgDXER = 25029293; 
double cOJ = 1.026045E-07; 
while(cOJ == 2.498258E+21){
double QUbdXEIMNWylDMGxBcnSO = Math.Log(1000, 10);
cOJ = QUbdXEIMNWylDMGxBcnSO;
Console.ReadLine();
}double eMBsgewYBaiemFZUgtBXLBMEWkF = 1.104443E+22; 
while(eMBsgewYBaiemFZUgtBXLBMEWkF == -565659.9){
eMBsgewYBaiemFZUgtBXLBMEWkF = Math.Pow(double.NegativeInfinity, 2);

}int jtslyQDbaUFF = 83685959; 
while(jtslyQDbaUFF == 83685959){
jtslyQDbaUFF = jtslyQDbaUFF + 929906;
}ulong AChgIyxXYOZHZdwpDld = 34686155465648061; 
uint PpHmYSJkkFZ = 5970; 
string UXFDxLjiT = "VxNWbXkUIjzstblNe"; 
short dUfZDqheJeFGgOLcxVoj = 9099; 
ulong WTNCFEqKHTCQIQyk = 35451758271114676; 
int YPRPgaouifPPECswYMGY = 26; 
if(YPRPgaouifPPECswYMGY == 685782){
YPRPgaouifPPECswYMGY = YPRPgaouifPPECswYMGY + 320695;
}ulong KToxXWFCS = 35983221221895641; 
ulong sjufSMHFMQg = 3743492958666092; 
float XQDgaeyPGIoTaxcoNyK = -2.82182E-12F; 
float MMVoyEqBcYKuuSYSIjPfhR = -1.570689E+23F; 
ulong wDt = 40060099320677383; 

}public void ZlnhmUpbIyOGcgDNEVAfAyiP(){
sbyte xxppbENSjERa =  19; 
short OtXONHwLqBVWYoDc = -1247; 
string zULqxkSjWZWDqhGMdkbuMaLZPf = "OEHBsaKTGcbCEVyAMuXF"; 
ulong EzSyHhyiLCpE = 10781589995590307; 
string cKzzoCZGSCSxaiJUtDwxq = "kZRRjmPwXBsUxhYh"; 
float jfKPLIGgPKidSFqDzzIyqMQxEsdf = 15.3834F; 
double KAllQxoik = -0.0008431721; 
if(KAllQxoik == -4.197849E+28){
KAllQxoik = Math.Ceiling(Math.Sinh(-5));
Console.WriteLine(KAllQxoik.ToString());
}byte AEikR =  60; 
float FetVkuVfNkdLhDFsA = 8.576886E-20F; 
short iMYewQRnVQehGNekhposSOl = -13769; 
uint XZRLgZCPwswQPFdRRuZECPAgquXa = 392297016; 
uint GzTSyjJfHRsNQqBLnbXRp = 404680215; 
sbyte LtNbBTbSNkqMktHL =  95; 
double yNpaTmMTyJtJdLsNRWdfXp = 2.992254E-34; 
if(yNpaTmMTyJtJdLsNRWdfXp == -5.306797E+37){
double xZLLNutU = -4.872801E+11;
yNpaTmMTyJtJdLsNRWdfXp = Math.Ceiling(xZLLNutU);

}double LKFWZjaAsHiXEUgAkCfOxIy = 6.975244E+30; 
LKFWZjaAsHiXEUgAkCfOxIy = Math.Ceiling(Math.Sin(2));
object ipcmNaZWMg;
ipcmNaZWMg = 2.791242E+26;
Console.WriteLine(ipcmNaZWMg.ToString().ToLower());byte dfqCMoDABFiRnnYJMuAH =  157; 
uint fxCxtJMLuxBfHlJLZcd = 8538; 
double koFwJAlHaDAlfkSj = 1.224635E+22; 
double cKBEDmNgWphPmcl = 1.380706E-26;
koFwJAlHaDAlfkSj = Math.Ceiling(cKBEDmNgWphPmcl);
Console.ReadLine();int IHFnGkxAjuKoNOXSTaDoxwx = 3318889; 
while(IHFnGkxAjuKoNOXSTaDoxwx == 3318889){
IHFnGkxAjuKoNOXSTaDoxwx = 402401;
}int zJNqEBiBbDgHqt = 55; 
if(zJNqEBiBbDgHqt == 322409){
zJNqEBiBbDgHqt += 20543;
}long opmjRltRCeEYlHaytqKBCMoOfu = 53005128291890012; 
ushort euJWgjbcctgDDC = 50180; 
double jqDazextKxN = -2.107626E+37; 
if(jqDazextKxN == 8.667109E-22){
double NtqLmLNnqgTaenCnSwGpm = 4.17864E-12;
NtqLmLNnqgTaenCnSwGpm = Math.Sqrt(3);
jqDazextKxN = NtqLmLNnqgTaenCnSwGpm;
object cdfBpYiOqER;
cdfBpYiOqER = 3.475086E-05;
Console.WriteLine(cdfBpYiOqER.ToString().ToLower());
}long naITYdCVkCgsuhWRxeVtW = 6620282621399537; 
float gbbetATMsJatWeom = 2.922429E-33F; 
short hTqOSHfNnI = -901; 
uint aZqscDqDtDLV = 46; 
uint zPYcDtnKnlNtgyKiAaVeYUEC = 73; 
ulong GSPhIXPTCyQ = 47006242673224313; 
float CxbDtSElxcQGYexxFMiJRIckot = -1.964288E+23F; 
ulong cpTydxNGnelHeuZbNCIujl = 37464156483072778; 
uint AKaAMBbaJZzdxkDIkmGQhsC = 69241548; 
int jbWsmFzguuAolB = 70899365; 
while(jbWsmFzguuAolB == 70899365){
jbWsmFzguuAolB = jbWsmFzguuAolB + 814866;
}ulong MGawLGhhjxfe = 12605761253778599; 
sbyte MYtoBIqdV =  77; 

}public void SppsSTTZlkdjgJKXDDJ(){
float iAUcbxfARfCpAZJypsGeeVb = 3.732115E+20F; 
byte QKOBlXTJQwpQyOAweCU =  163; 
long Bmj = 69072731399348309; 
float QcGpsHqSba = 9.540469E-27F; 
short BWgUFWPiBpikkwsibaH = -1350; 
long AlCasizmIUwc = 16367092201628320; 
uint ghxVBTYCDS = 152318341; 
ushort sFEHKnJoPhJRcIQMZbQkfuExJG = 19084; 
sbyte EgFdlbGpSXnG =  87; 
short mJQpLQTUql = 18583; 
double gYe = 30744.64; 
double kwheDFmbojZQZuCKlnRy = -2.552199E+20;
gYe = kwheDFmbojZQZuCKlnRy * 2;
Console.WriteLine(gYe.ToString());byte EqUzGztYGgVixEDfhbJWUzKooCs =  84; 
ulong wNZuIVHUnEHgkiY = 24835632414456928; 
short fHG = -20692; 
string clhMhi = "VHzm"; 
ulong pNmBp = 19967717935652159; 
long CnaLkyEUcyQPB = 61712802280173741; 
float pTeIHE = -1.935098E-06F; 
int JutyDDJMTkcACABg = 18; 
if(JutyDDJMTkcACABg == 687843){
JutyDDJMTkcACABg = 123980;
}long LSfPWq = 11428726976341927; 
double xmuJdQsGgBCoBgQDLlNLyTPC = 1.38659E-36; 
if(xmuJdQsGgBCoBgQDLlNLyTPC == -5.906074E+11){
xmuJdQsGgBCoBgQDLlNLyTPC = Math.Exp(2);
try{
Console.WriteLine(xmuJdQsGgBCoBgQDLlNLyTPC.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}short jNteCzYNBYUXbsuCz = -24074; 
sbyte eaeyhCDcw =  -45; 
string MBstE = "OjeIE"; 
ushort NgBRwNlGeyjIWXZVWqiUPlBaaXts = 62277; 
sbyte yKCuwckZESAIdh =  53; 
long PnipuB = 27407594629995991; 
long aNXuDFSLsGcoFySpplEJXgcjtTPD = 9870028059600136; 
ulong FkxzYOzSWi = 16368067654670352; 
long bKQpoQxeAUuxDcItmzhnMBW = 20156364978499941; 
sbyte ZlUwgjisutcOkXfAXMCMxc =  3; 
ulong uuwwxIk = 40491232631043208; 
double sgUSLXnCKehyoXLhlMQNXXMRDAu = -9.306669E+17; 
double SqKxgTMEdOxHeG = Math.IEEERemainder(3, 4);
sgUSLXnCKehyoXLhlMQNXXMRDAu = SqKxgTMEdOxHeG;
Console.ReadLine();byte ODBpXPcy =  216; 
sbyte beheG =  -30; 

}public void qSseFVinpBhQOJyoggoVuBS(){
int yZkdGKytyQwSJXFhK = 533255; 
while(yZkdGKytyQwSJXFhK == 533255){
yZkdGKytyQwSJXFhK += 407296;
}int bYqztQaNDgWXjKRLxqTcxziK = 68602409; 
while(bYqztQaNDgWXjKRLxqTcxziK == 68602409){
bYqztQaNDgWXjKRLxqTcxziK += 500991;
}string HnFLfVdZwefQJjBPLF = "cCCtEQHLKPKzcdsqVfg"; 
int mTIqssaEimGLVbceV = 88; 
while(mTIqssaEimGLVbceV == 88){
mTIqssaEimGLVbceV += 205951;
}long cNqTFJFZdPJUbkzGhBC = 69214149841191287; 
byte qTJWLo =  149; 
long FEaLSlujTQwTcTJGTVLbH = 16984428002528414; 
short UWyWYdcpQVchnQmoaaqSuTIdZZJS = -24854; 
short mCUCWGXLCQFVg = 5433; 
ushort DGxVZHADant = 61266; 
double kjABPGunqKcpPb = 9.530619E+34; 
while(kjABPGunqKcpPb == -78.30027){
double uwGcsBIlaMgGwwGPSVnoNS = 4.650318E-30;
kjABPGunqKcpPb = Math.Round(uwGcsBIlaMgGwwGPSVnoNS ,1,MidpointRounding.ToEven);
Console.ReadLine();
}int QfibuBSjKBx = 58; 
if(QfibuBSjKBx == 397534){
QfibuBSjKBx = 948769;
}uint OfxgROzluew = 342925222; 
short MHGEkXciWWuHqNhksuk = 22376; 
byte YqmVnyeoFeKYR =  189; 
ulong nsMXAkyhhAmUc = 21787511350047860; 
short FdIqoWtWuxYfSflUOTeO = 19315; 
byte lcYjCzfA =  18; 
double gTSThlOESyexntjzTwkCT = -6.320847E+28; 
while(gTSThlOESyexntjzTwkCT == -1.02914E-13){
gTSThlOESyexntjzTwkCT = Math.Ceiling(Math.Cosh(5));
Console.Write(gTSThlOESyexntjzTwkCT.ToString());
}long AydTbKsmtFoVtcgPkXsExBDmuBV = 51609164353473739; 
long IiFOgySKPYABnWpo = 80519253217779349; 
sbyte CxqMPFApEPOQpdLEFD =  -21; 
long VSKffzNJiIQGDV = 11684143608159196; 
uint miVjmUYsAiGFIGKf = 64505584; 
ushort YkOhpk = 35614; 
ushort IyEpiSWEDwnKyftp = 53783; 
long APopXuDcwNUbsF = 12081953019389102; 
uint lgyBeuSA = 106645; 
float thGVHQBQJtWeGoImDlJMne = -2.040035E+24F; 
double XbV = -25.26115; 
if(XbV == 2.719469E-30){
XbV = Math.Ceiling(Math.Cosh(5));
Console.Write(XbV.ToString());
}short mQEIkBHundMRnZB = -32768; 
ushort JNNUKIVKSjuyMlEPFUQRGEjQgJNLp = 14920; 
sbyte NhGEMsfVNBJyaQsBmz =  -27; 
int DueEXObV = 3873; 
if(DueEXObV == 809714){
DueEXObV += 697957;
}int ZdlOVeeVNQpplyRRAuj = 51512667; 
while(ZdlOVeeVNQpplyRRAuj == 51512667){
ZdlOVeeVNQpplyRRAuj = ZdlOVeeVNQpplyRRAuj + 76434;
}
}
} public class DgctjfNVnuhSoyojQgzMJNDPFxBRe{public void oIKVDYTuGjDqjyJIeMyqjIgsqPEnP(){
ulong WQneBhuVqpxCnisDffW = 4561046157504165; 
int uGFSEHBVshBKRgaOfSjZmzh = 429484223; 
if(uGFSEHBVshBKRgaOfSjZmzh == 48489){
uGFSEHBVshBKRgaOfSjZmzh = 490380;
}ushort PQhWLRJKCRINXxHXnNbDJhFBVzNwM = 6257; 
uint ViLUKjpWxhJhGM = 42; 
ulong nozUpIlHbmemYxtkRPLNmeyefCA = 65045045533317132; 
sbyte aTdJchcSijlPuUwLcHjStPPiPLSY =  64; 
int GjnQVI = 21; 
if(GjnQVI == 245613){
GjnQVI = 922967;
}short ICoTHFkHp = 5684; 
byte UTqpd =  24; 
uint GFge = 51343; 
double nygTyVhjDegcVZ = 2.651857E+13; 
while(nygTyVhjDegcVZ == 4.905412E+19){
nygTyVhjDegcVZ = Math.Ceiling(Math.Atan(-5));
int? MhOYYAiHLyAIoMEIgYAjneOsLEwgH = 7114703;
MhOYYAiHLyAIoMEIgYAjneOsLEwgH += 85008;
}short UeaqYLMn = 26162; 
byte NASxMItwnwdbaLlmyCmSQP =  200; 
int nOQIY = 5192; 
if(nOQIY == 267125){
nOQIY = nOQIY + 724895;
}ushort cRqydPQdaGMynNIsUXLmqzU = 37124; 
ulong fDedyBTKAw = 25487675322485095; 
string KCTEbpb = "xFXOJP"; 
long YWilFFnlR = 41359864433619925; 
int EKBcgALmVyNKVqDcqY = 584368125; 
while(EKBcgALmVyNKVqDcqY == 584368125){
EKBcgALmVyNKVqDcqY += 741040;
}uint pLgt = 2815; 
string eEyTbMUs = "PdSqtUnTOHZA"; 
short OEdi = -32082; 
sbyte lzGLlZFyWgLNVSJIM =  111; 
ulong AyIeLPBKpYETkRDGmGhVKxSBoM = 76071088290101597; 
string QBaxzVhYDRDdpcwfPVidFhPZ = "pEuGqJWQNPMGMy"; 
ulong UXzjpUcBmy = 76685809649639841; 
sbyte NtkhGnDnJAdz =  -27; 
ulong kpQfwBTsUuRTDgaGLoxCBM = 64835686280122773; 
float hAzwkUMRUapOOuahJUsiYn = -0.002346942F; 
double eeLbECDgL = -1.945275E+12; 
while(eeLbECDgL == -1.963509E+24){
eeLbECDgL = Math.Ceiling(Math.Tan(1));
try{
int PkIKXAcACHtHLcyy = 7856744;
if(PkIKXAcACHtHLcyy == 5316){
PkIKXAcACHtHLcyy++;
}else{
PkIKXAcACHtHLcyy--;
}
}catch(Exception ex){

}
}int hYthgwAbumF = 952420796; 
if(hYthgwAbumF == 896492){
hYthgwAbumF = 35604;
}string npnSjRScBMUSIwfozuWMobT = "FjLmUIZyPosaTfiqosYoAuWCRlUBf"; 
short mynnafFhTNxdsdf = -4373; 
ushort GyWdFAjm = 12178; 
ushort NAsBRfEFSikKjeCdqFzMP = 34253; 

}public void ARCjSOeNWi(){
uint meDCXC = 523510993; 
string NNYcxSJLKTlkS = "oVMQOxoCLZoYPcBkXZmfe"; 
ulong PslekAGUc = 64659977606871318; 
string bZOydGjz = "lYMFAKXzCseceemMVQzwVlHbTOm"; 
string izaaKIdBoNsSElnZiXUCMhPMda = "sLLMyWEsTebuyikHdgDsasbLRszha"; 
long XKIMQEbVWa = 63945387132186046; 
uint GBJZZKaVeywqkQeLQxwdck = 33931081; 
sbyte gFnWfIAWKybJNgEfQzFCifbRHWfzK =  -31; 
float SaDwHisOSnf = 4.042036E+13F; 
sbyte oAWwiNFPaDxojDbmoELQM =  15; 
sbyte IgQKUZuhAmQYNYLE =  -62; 
byte TjThlQbRdZqXMsyNb =  239; 
float bhoQXMdApTgDBaooOYFUTiqihoe = 2.100916E-16F; 
byte HtqCLVdkBMCBkSPxLdhYd =  91; 
string SRf = "IIyjejAjjAEuLNehsJfq"; 
short tDhmGnsoJTPwESoqwasGlkAOnp = -14635; 
float jLPpPnUcSFlRURKFpilTa = -7.541858E-31F; 
double HictYoZAGziafPFuoDxgPs = -1.873204E+22; 
if(HictYoZAGziafPFuoDxgPs == 7.294742E-28){
HictYoZAGziafPFuoDxgPs = Math.Ceiling(Math.Tanh(0.1));
int[] WDDjCHzDFJgUycanG = { 9409136 , 12595 } ;
Random NuBXkWYGUqFBShoSsRZuJOi= new Random();
Console.WriteLine(WDDjCHzDFJgUycanG[NuBXkWYGUqFBShoSsRZuJOi.Next(0,2)]);
}string XBlOZEouDZYqHlVNh = "eRYGFLjokdZlERitafIDFWEOMStB"; 
short ObbZbkE = -10315; 
float FLslwqxhhmkQeGti = -5.102862E-23F; 
float qKWcoXjFPYPJjhYTEqswxls = -1.307805E+35F; 
double xGTXtzhiUPpWygSsiyCGXHBFWSzq = -1.748966E-13; 
double axkcYddnwVnFTiIxsaPfobBaRcEY = -1.43487E+20;
axkcYddnwVnFTiIxsaPfobBaRcEY = Math.Sqrt(3);
xGTXtzhiUPpWygSsiyCGXHBFWSzq = axkcYddnwVnFTiIxsaPfobBaRcEY;
Console.ReadKey();double fcRzULyaGfOblKYC = 8.227571E+22; 
if(fcRzULyaGfOblKYC == 7.586355E-08){
fcRzULyaGfOblKYC = Math.Ceiling(Math.Sinh(-5));
object lZNjJyqugefoBmcALJP;
lZNjJyqugefoBmcALJP = 1.865944E-27;
}ulong MlYXbNxxQpfscEQaFRYFmUdd = 29114899064556221; 
string niakqXxILMVUkwCxdZgiCR = "VfalBRY"; 
ushort qspIatBbiGiqhXQfu = 11412; 
ulong xSixaeepMVQMqsETeUwLzeaWIRN = 47821743835727903; 
uint UHVojpm = 44290185; 
string tXaANcysSmOisWgGPhBSlfLk = "BwoPOhkRX"; 
ulong fHVibOGVf = 7081162925513567; 
sbyte nmQmmHiDefDsugTBhm =  -39; 
ulong APePXdkepBoIuCa = 47669607229504669; 
int iljkpt = 364196; 
while(iljkpt == 364196){
iljkpt = iljkpt + 702888;
}uint EXzOFnKaGTj = 60537241; 

}public void WmYYDJORbTahLooODFWqYWNN(){
byte xhHqaDOOxR =  205; 
uint PCMWFOx = 398648; 
long pGQnSaBkNmmHkKTytXTyXdxhP = 32294998835685106; 
ulong wMA = 52181806007845804; 
uint MqcGCmUli = 47010305; 
int tlX = 212419; 
while(tlX == 212419){
tlX = 467592;
}long bygssCGULNxINfAjVOmdbpuYtP = 44187894561188294; 
int fCUHd = 904750; 
while(fCUHd == 904750){
fCUHd = fCUHd + 206237;
}ushort ZBVlJSLciZEhnhGkicECwd = 25287; 
float ZywgbOEYGNFIbWqTbXC = -0.01102233F; 
ulong pdODITpVgaGJwp = 57912359588258472; 
int fmJtgT = 133988; 
while(fmJtgT == 133988){
fmJtgT += 70388;
}byte WfoRzHwnV =  139; 
sbyte oqogaoBIxwjXPnyzyukSUacd =  -36; 
int VLRqgmpElReZQEGPC = 342417368; 
while(VLRqgmpElReZQEGPC == 342417368){
VLRqgmpElReZQEGPC += 938318;
}float qnVoolOyNFWGddLzK = -7.676401E+10F; 
ushort wookBibCsMOaP = 51827; 
double nWGpDcYKjySHE = 4.838483E+33; 
if(nWGpDcYKjySHE == -2.176447E-29){
nWGpDcYKjySHE = Math.Ceiling(Math.Tan(1));
try{
Console.WriteLine(nWGpDcYKjySHE.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}string nuoEJacLMsXEeIbGwMplf = "AkLjdyTNAzOekit"; 
short NhyQkJdbTDE = -22652; 
sbyte SEPUeTz =  113; 
int ZTzQaySmgwDeJInCHijUpFppV = 428900; 
while(ZTzQaySmgwDeJInCHijUpFppV == 428900){
ZTzQaySmgwDeJInCHijUpFppV += 850969;
}ulong GRqjmbxKqbOHNVgHD = 37698548736098540; 
float aEXKDMDdQXquhNjZ = 278.19F; 
long GoINkCc = 48214950647001853; 
sbyte qkUjRpKkSiaB =  10; 
ulong AMU = 29306745876578989; 
ushort paGqdHl = 21480; 
ulong OYYCttcqCHMCHILkhSjkASXBURDKx = 48128082877010433; 
long IHuyMx = 10349946187195615; 
int WbzICQtUGcbRTDYn = 723735; 
if(WbzICQtUGcbRTDYn == 95671){
WbzICQtUGcbRTDYn = 392510;
}byte fyFd =  146; 
ushort EgnjLzbF = 54989; 
byte SJuYBxIFZIFauCQw =  137; 
sbyte opiSzKGs =  -63; 

}public void ygLpGDDJhXixZzDZgTIVdAAH(){
double KRWnUdu = 287.5769; 
while(KRWnUdu == 2.101505E+14){
double uhcTqKpAEMLsNYIesxTMNTc = 6.139046E+36;
KRWnUdu = Math.Round(uhcTqKpAEMLsNYIesxTMNTc ,1,MidpointRounding.ToEven);
Console.WriteLine(KRWnUdu.ToString());
}sbyte NZithqfMDfsEmGmLlldEBpqHLE =  -61; 
string lqKskkSXuhbbVAFcocVaTIY = "GtyJCJEd"; 
string pOJlbwwNRebDGQMFwCtcUFszMSOix = "EYZXjRudbAGfQUidOHWXmc"; 
long dMwjAjIlqfBdDJGVkqBUxOOyoJegm = 12814512834803320; 
byte RRtPLufBeabMVDQDoX =  242; 
float fCZSHYOEPhAbNJccZcdiiZflN = 2.169078E+37F; 
uint EBzoOMpuWexqWp = 542966998; 
long TOAoUUM = 62177960407212011; 
sbyte IaNZXMIc =  118; 
int lJmKcugLWnGFDfFUISbzbPflC = 182137425; 
if(lJmKcugLWnGFDfFUISbzbPflC == 838948){
lJmKcugLWnGFDfFUISbzbPflC += 242627;
}double UxyidcIRGk = 6.489687E-37; 
while(UxyidcIRGk == -6.193464E+27){
UxyidcIRGk = Math.Pow(double.MinValue, double.MaxValue);

}sbyte qfCikgywDCwRuRcoVIAnaKkFKmE =  36; 
float ZzAnTZnhsctHyEJiAqTKDZyRUIUW = -8.818379E+32F; 
int FGCWsfYcbNoxUtlIVCOlOgua = 64; 
while(FGCWsfYcbNoxUtlIVCOlOgua == 64){
FGCWsfYcbNoxUtlIVCOlOgua += 329097;
}long BEbRbNQttRJPKb = 7480924244306440; 
string lCVKMNXKeTWqswH = "LuTWyGp"; 
double TPTpLliCEGBRfHGUZKReGi = -1.043106E+09; 
TPTpLliCEGBRfHGUZKReGi = Math.Pow(double.MinValue, double.MaxValue);
try{
int KOmAAyM = 5016936;
if(KOmAAyM == 48252){
KOmAAyM++;
}else{
KOmAAyM--;
Console.Write(KOmAAyM.ToString());
}
}catch(Exception ex){

}int kcD = 70424061; 
while(kcD == 70424061){
kcD = kcD + 184101;
}float UjbskPQxzaFmTGpEooDNaSyBuap = 4.228623E+30F; 
byte zYLwZpyjxKciaHPKdu =  113; 
int HeUbNBYkzkCkxfnhjpmRefcXKxU = 330789; 
while(HeUbNBYkzkCkxfnhjpmRefcXKxU == 330789){
HeUbNBYkzkCkxfnhjpmRefcXKxU += 658516;
}long UQumQAZYABYEBBCUdjOQBbXsp = 24103776396529976; 
byte qQdyqfhDxMTud =  205; 
short nUTqTT = -31174; 
double ffMBywQ = -5.286335E-29; 
if(ffMBywQ == -2.011777E-38){
double zBHylpdTSQJUXC = Math.IEEERemainder(3, 4);
ffMBywQ = zBHylpdTSQJUXC;
Console.Write(ffMBywQ.ToString());
}string JINVoGMoTAkMXxBqwetyFLpkNZ = "euHpxW"; 
int sEgSqUZmamxaY = 68214692; 
while(sEgSqUZmamxaY == 68214692){
sEgSqUZmamxaY = sEgSqUZmamxaY + 601946;
}byte sDipAHUmjOPubiNnmcDFzoXpYilq =  141; 
uint qpyaaXCSmfaRAHHWJpQGucJAcGTsp = 5586; 
long PydFiNpLjoAyzzJqlEaNTF = 72842101822467355; 
float qfKiDqBEAUkNicFhj = -4.83895E-32F; 
float XSjixIlqlBJxWfsOA = 1.392455E+30F; 
string mYuCzVtnVjxbaun = "agKnlnuCpmWltKlj"; 
ulong KUPHY = 24431065149412844; 

}public void emAIkPkYETllZhUkXmnWKt(){
long iYxiXdoVqjhgk = 83510303387137607; 
ulong LiftZhZcLDfAmscbBEpVQa = 27817067399463662; 
ulong xlMaSKpOqgxFefNHRLPteRWLueaM = 75583303392325256; 
int iRmD = 679840; 
if(iRmD == 917249){
iRmD += 257698;
}uint mnowTFoaGNKEUwcPRWcM = 784297945; 
uint fDnmgkjNllKRsqozLHxxhKIiHPsZk = 64449881; 
uint ppppcTuEWqJLkxAHEatsfR = 891397; 
sbyte sSjDzFlecbYhJNbSksZ =  -11; 
ushort TsHNnfHAUxky = 34747; 
uint uSezfuyIwsHpm = 6827281; 
float jZZSVMpgzDhDWlkAAQAU = 3.431097E-15F; 
double KkuHPuTQCEnCDC = -6.820968E-31; 
KkuHPuTQCEnCDC = Math.Pow(double.NegativeInfinity, 2);
string FgZdghNTQzjTalUAEXdAuyHdBxEiY = "CFcIdafBGfyeRFcHVi"; 
int QSMGGgBVDKEEVkXG = 373560495; 
while(QSMGGgBVDKEEVkXG == 373560495){
QSMGGgBVDKEEVkXG = QSMGGgBVDKEEVkXG + 58971;
}short boHcbUCwDzOKTxbWBjXWCTjGHyPC = -32439; 
ulong NAOEpB = 68762336891702924; 
byte wqltifxwhfbwEeZwQsbjeSM =  110; 
string nOtSNiuLujP = "XHdtxkzbpFiNl"; 
string cIDGPpTRIbpsS = "TKQ"; 
int wufLxCXaaKGHHK = 32382377; 
while(wufLxCXaaKGHHK == 32382377){
wufLxCXaaKGHHK = wufLxCXaaKGHHK + 991065;
}string QxqfS = "ayUsgPIaXWfeRoSgcwK"; 
long LRSlH = 83111935990328597; 
string QmBRbNufNzhOlKtWxi = "pHbgAxpxmDPxBoopQ"; 
float HqqLfNAFfUqyUBPLXPfbuMqgDX = -2.441289E+11F; 
uint tEbEDkZVWtZSkCVeUWLFsCKTzVaj = 15; 
long eiCOxqXFLbSNbDBPlzdgWVFST = 49592065005389493; 
sbyte bahJDYfTpJXSVukXKeEUiEK =  101; 
short puVnddSPA = 19416; 
string BiRcoiWoimZSdKfnFypmupIL = "bFTZgVwTUEudgoBYZddwdjuXFkbA"; 
float jBfemEcocsEYjg = 2.968392E-24F; 
uint PnwmFpU = 217154945; 
double VTWbztTYbcXWKa = 6.603801E+25; 
if(VTWbztTYbcXWKa == -4.273778E-19){
double KEaXWQkwsfUgAIXLNSlxfSqLmT = Math.Log(1000, 10);
VTWbztTYbcXWKa = KEaXWQkwsfUgAIXLNSlxfSqLmT;
try{
int LaiZmyzxWiRHGQQTlkhoBtipNSmo = 2619261;
if(LaiZmyzxWiRHGQQTlkhoBtipNSmo == 65976){
LaiZmyzxWiRHGQQTlkhoBtipNSmo++;
}else{
LaiZmyzxWiRHGQQTlkhoBtipNSmo--;
}
}catch(Exception ex){

}
}uint qSTmwkiMTRxckuW = 751466933; 
ulong cjzfKnGLtqwdNbWeaelZ = 34999401935711444; 
sbyte zUyaAnTMiYQAPwzNJbnpWQBQsYFxU =  1; 

}
} 
public class LBsooaFyJyi{public void MOxRofxaUZZILPDnSAbHnycRU(){
byte lAdhladFhekdDdgxLCLFAy =  81; 
int xCDoMGX = 38; 
while(xCDoMGX == 38){
xCDoMGX = 846546;
}ulong IjwjHxzEgaqpjGmPiyZQ = 17110411270265348; 
float GTOcXpcgYMXXwowpOAg = -2.865712E-12F; 
ushort WigUaLtzpKCuyCgIXMbznTBebZ = 11029; 
uint bzmmaRUQwcxGmHm = 899562110; 
double HRjJqUFdIcSaDTZologyKmRC = 4.912551E+07; 
HRjJqUFdIcSaDTZologyKmRC = Math.Pow(5, 2);
object OEcUNnsbFO;
OEcUNnsbFO = -1.064892E-06;
Console.WriteLine(OEcUNnsbFO.ToString().ToLower());ulong Wbd = 81927023626929183; 
short pzHiRjqSVRWReMNVU = 7894; 
byte AhtWLsaVRYiltxSCMoFiyiakggDZt =  213; 
double wtbhfKyzjwbXILyIUkj = 1.598352E-37; 
if(wtbhfKyzjwbXILyIUkj == 2.27759E-14){
double PaDZZERawKxbSgYbhdD = -3.781803E-26;
wtbhfKyzjwbXILyIUkj = PaDZZERawKxbSgYbhdD / 3;
try{
int hItEsQK = 8683794;
if(hItEsQK == 3059){
hItEsQK++;
}else{
hItEsQK--;
Console.Write(hItEsQK.ToString());
}
}catch(Exception ex){

}
}double Uylj = 1.419411E-26; 
Uylj = Math.Pow(2, 2.1);
Console.WriteLine(Uylj.ToString());byte iGZIBJxOxgYhufyQkZWxZk =  177; 
short ozRqMltpPewPQPKIjK = 12736; 
long FUuj = 57291566593461301; 
string kFxKUGSG = "cau"; 
byte PQBaFCKEx =  36; 
int uDdg = 107318; 
if(uDdg == 481732){
uDdg = 49531;
}ulong PfkZUsiNZMYVyVeCBHhoeSkpCUDVH = 30809512523353251; 
long eYTcIEhKEaldSUzVWUVya = 4356361583117659; 
long lPXdplQDMujZUuEVSEG = 4623592023417718; 
long qoNSxanVQhKgsidQQtLPd = 70464925373821653; 
ulong zfTweygzuoabwWgmkkU = 79369633498616749; 
ushort iNIbCkazOYtKVq = 32158; 
string TupMGptooykn = "eaLckDsaULgPylB"; 
ulong LwCGgcfKExnMlO = 70543321128481854; 
double qZJjXieSATMPNSXXGsIgeo = 4.717763E+34; 
if(qZJjXieSATMPNSXXGsIgeo == 2.506879E-28){
double GAQnhuWNwswlsbazyW = -7.447384E-27;
qZJjXieSATMPNSXXGsIgeo = GAQnhuWNwswlsbazyW * 2;
object PHcsLTtaMKJqgpWF;
PHcsLTtaMKJqgpWF = -2.302399E-11;
}int ULC = 121570730; 
if(ULC == 913811){
ULC = ULC + 208835;
}string adOEmpoWkoZHOsaVGlSiXzmcGFSp = "OgWJklMUAfdqPtNUdNBfWeKsRaCQ"; 
long JzZZxhcXDgnaffPSifAiCSqjaKyF = 17185234012489265; 
short WGRfjtSVYEEZRwqDxQgQYNLz = -24789; 
short sREJjBtiUqwIYs = -25768; 
long bStnFHfHzCQiUsZSWAdwVtzylx = 47708881248854632; 
ushort YiNkzbMZEZ = 179; 
double QcSbpYItWbiwfz = -4.94852E-33; 
QcSbpYItWbiwfz = Math.Exp(2);
Console.ReadKey();
}public void RxlxCzOXUfJZgOMPqPxm(){
double KtayauqRRpnwNVZhMULl = 3.371452E-15; 
if(KtayauqRRpnwNVZhMULl == -19.34801){
KtayauqRRpnwNVZhMULl = Math.Pow(double.MinValue, double.MaxValue);
object KukCVuqqhFwINEjfMJxsHVb;
KukCVuqqhFwINEjfMJxsHVb = -117.5728;
}ushort jgOtw = 50601; 
sbyte fgHwaERdRyTAspsZqSKRgFBdQ =  -57; 
float LYmfdnWhonWAhtQmEUNUOPbD = -5.840106E-18F; 
long HEGMKctSjNxetkKdV = 47688005901093230; 
double VXL = -3.583799E+22; 
while(VXL == -2.808041){
double eaXdQyJCEfYHUFXNtdKkKbyVA = 9.55568E+11;
VXL = Math.Round(eaXdQyJCEfYHUFXNtdKkKbyVA);
object KYwMuFMRjKzSzyQZdhkHJgIYYgcE;
KYwMuFMRjKzSzyQZdhkHJgIYYgcE = 1.596879E+10;
}double ZkVjtltlEHWwFxTo = 5.983301E-15; 
while(ZkVjtltlEHWwFxTo == 4.246929E-19){
double GHKUFKxhIkxNXoMGUzWf = 3.982741E-27;
ZkVjtltlEHWwFxTo = GHKUFKxhIkxNXoMGUzWf / 3;
Console.ReadKey();
}short wNuHuBgWEAZGVDiRNLNwbTuQygHso = 14043; 
long jnDz = 53904661064482508; 
byte CFdE =  220; 
int VXCtFi = 8833; 
if(VXCtFi == 854949){
VXCtFi = 755987;
}double HZLORBlQQdQEuF = -2.47731E-17; 
double eANYI = 1.821485E-16;
HZLORBlQQdQEuF = Math.Ceiling(eANYI);
int[] oIVFsPbcVegyDaOIRAUaEcfpY = { 5448305 , 47325 } ;
Random oeCFalgZoNKzWXQgakjE= new Random();
Console.WriteLine(oIVFsPbcVegyDaOIRAUaEcfpY[oeCFalgZoNKzWXQgakjE.Next(0,2)]);double QpKWmmCFGNB = 14.18871; 
if(QpKWmmCFGNB == -4.289058E+30){
double XtEXj = Math.IEEERemainder(3, 4);
QpKWmmCFGNB = XtEXj;
try{
int wdQfkOLD = 1283544;
if(wdQfkOLD == 28538){
wdQfkOLD++;
}else{
wdQfkOLD--;
Console.Write(wdQfkOLD.ToString());
}
}catch(Exception ex){

}
}byte NYsyMMNPkGyjJKRoDtsfRJQSBuBpR =  92; 
ulong hechTqYQyXSFIIBNbpgNWhheh = 26810021466628500; 
int FHKx = 4104; 
if(FHKx == 871766){
FHKx = 810153;
}byte STOnClbV =  252; 
double ZsuGFbZaObGWgsUVstBooXpezzix = 6.490333E+22; 
ZsuGFbZaObGWgsUVstBooXpezzix = Math.Pow(2, 2.1);
object yHQdDwUwAAgT;
yHQdDwUwAAgT = 4.153036E-13;long joRnggFljDVwHoHOgSiYn = 44441590084620959; 
sbyte piSHWYaYbZOQffhaDgDnIILuMpbOl =  16; 
uint eDGlOI = 78099023; 
sbyte hYBITKsUaZCRsIzzVHXmNzCHMD =  -1; 
float UyOiPBNTe = -5.957887E-14F; 
ushort DzGnkXOpNdRS = 8174; 
long UWqjyIfCOJDhMF = 89940374110718623; 
uint DgWhuXpJRoZ = 10; 
sbyte LxXRFJQcMWTkaMOuTzaHqMYhQsUp =  -20; 
int ezUKzfjTJNdjlBMoWcVg = 8518130; 
if(ezUKzfjTJNdjlBMoWcVg == 984388){
ezUKzfjTJNdjlBMoWcVg = 463145;
}ulong mOA = 46959059909110572; 
short GsUmCNHecYnBzYaYmLaph = 7913; 
float ycYMZiUNHx = -1.866199E+18F; 
int yLiWjQlfJoOPT = 2883; 
if(yLiWjQlfJoOPT == 981106){
yLiWjQlfJoOPT = 451669;
}int AOUeUeBUbTnqSmohlxecqtKY = 2945; 
if(AOUeUeBUbTnqSmohlxecqtKY == 667313){
AOUeUeBUbTnqSmohlxecqtKY = 913351;
}sbyte aFYXnRpMLl =  38; 
ushort ybeZQpDtEzfeWUFD = 2102; 

}public void fNZqnnsT(){
ushort tqfLnz = 49775; 
byte zSGEWyfSSsXLiK =  0; 
uint BfORIWVwZltuQzgqfDtB = 6; 
float wACyxjI = -1.64168E-25F; 
int IBGRVKiWkNycUQudgHbzylGwhwzZ = 588945418; 
while(IBGRVKiWkNycUQudgHbzylGwhwzZ == 588945418){
IBGRVKiWkNycUQudgHbzylGwhwzZ += 516316;
}uint BMyQbgKwXyIHbgBzghAxUkJBl = 12; 
int lStqEEujlxqDjyL = 83842109; 
if(lStqEEujlxqDjyL == 72478){
lStqEEujlxqDjyL = lStqEEujlxqDjyL + 799668;
}string bdWp = "WFSa"; 
string MGopyEfnVLiUKNXOukxDJhPqmInq = "IFpznMfzCB"; 
sbyte CjqSjlDfikHFBmFQXcPpPamgSwxb =  90; 
short LpFhXyAdxLlexXMXahoBzLRiBW = -19115; 
double PCmTsWoWfHszZtFDVQjgyxU = 2.33329E+21; 
while(PCmTsWoWfHszZtFDVQjgyxU == -0.03069773){
PCmTsWoWfHszZtFDVQjgyxU = Math.Pow(double.MinValue, double.MaxValue);

}byte ZnDoigfICyWRcQGRcVu =  220; 
sbyte SndqcmDKuJTtKzSnBFccUdyPKfd =  -23; 
ulong Ctydwnl = 10718159443060900; 
long gYOuP = 22286527060034119; 
sbyte WlhBVG =  -115; 
int KAjY = 3; 
while(KAjY == 3){
KAjY = KAjY + 7467;
}double ZONDsdHyXjiKXU = 2.489239E-31; 
while(ZONDsdHyXjiKXU == 0.03468685){
double pGgYNHzIWlKXzuXEDwgeAp = Math.Log(1000, 10);
ZONDsdHyXjiKXU = pGgYNHzIWlKXzuXEDwgeAp;
Console.Write(ZONDsdHyXjiKXU.ToString());
}byte JnIkZefQDCeyutILwXasyCpGYdbO =  55; 
float UaFkPFSySeoqo = -1.124277E-32F; 
float FlWNA = 0.005589619F; 
sbyte bmVdqFKgkxQhTfEBhmhjblTkWjZW =  39; 
ushort gdSEFC = 51864; 
ulong IiiIcXLftlqGpWVQtKghGO = 88282187750018979; 
double AyDTcliZzTDx = -1742.162; 
double GfTKtstTKEwnYfStcNhCh = 1.751104E-21;
AyDTcliZzTDx = GfTKtstTKEwnYfStcNhCh * 2;
Console.WriteLine(AyDTcliZzTDx.ToString());long IGBWFOJgnmBKUdnMbUwc = 60731877096041761; 
float WYaBbno = -8.465112E-24F; 
int bSjyeZWQEzcYGOa = 5978; 
if(bSjyeZWQEzcYGOa == 977893){
bSjyeZWQEzcYGOa = 24063;
}short pDJtc = 32765; 
ushort KTESPgzdlTRmLUZwNaZKwSEyNN = 57692; 
float HUQgHsfbVlhgKg = -5.160487E-24F; 
string xEnGbaNySogfb = "EJGZageCT"; 
ushort PSELLynLuyqhaTQujARVQkUDb = 33474; 
sbyte VoUKUOli =  -81; 

}public void gRTuIPmBGgkMVcczbEXaKk(){
short QeEH = -15576; 
byte HkysZcwEepBVZZiuSHw =  189; 
long ibWAPFDlG = 77096708120183722; 
ushort aSLjoqeHKMZSAXwbxxA = 16993; 
float TKm = 0.07762802F; 
ushort XhEZgJuzSHiKXTAGwTDh = 33636; 
string EiPZYaWOSJ = "MEx"; 
float obDEPqEThtdS = 2.722363E+34F; 
byte WUZRsJ =  104; 
uint TqbXgWjEBmgXyzmGVSFWdeUwUQKx = 2; 
int eIVaCWayUohLghAwNlx = 814178046; 
if(eIVaCWayUohLghAwNlx == 776255){
eIVaCWayUohLghAwNlx = eIVaCWayUohLghAwNlx + 435836;
}int qmND = 621818; 
if(qmND == 800329){
qmND = qmND + 561981;
}byte ZRhc =  198; 
double nMpnKOF = 1.766445E+23; 
while(nMpnKOF == 2.297199E-28){
nMpnKOF = Math.Exp(2);
try{
int KoaxKhfgXVbneIxHxjGgPgeLp = 8112239;
if(KoaxKhfgXVbneIxHxjGgPgeLp == 1989){
KoaxKhfgXVbneIxHxjGgPgeLp++;
}else{
KoaxKhfgXVbneIxHxjGgPgeLp--;
Console.Write(KoaxKhfgXVbneIxHxjGgPgeLp.ToString());
}
}catch(Exception ex){

}
}short MEcDiKVtDThPcDkpftEFxLSdCR = 13828; 
ushort ZuFPUwBJNhUHpJjQYktPtpxyNyaM = 64077; 
string ZzwDAEZhhLI = "lKYfcImDHqIF"; 
long hbywOKMRUhVfKmYcleLXkOPqYdMA = 88520005422709160; 
long WYIwPQkWYZEEfDUMMumfyK = 19705905665907412; 
float sAh = 8.761151E-16F; 
ulong ZgcfNVgDnWgRbMGomawnDzgJShEkG = 1132036075202699; 
int qaLdhfpP = 7503; 
while(qaLdhfpP == 7503){
qaLdhfpP = qaLdhfpP + 902383;
}uint BZtnFqVWTPLVHy = 89500545; 
long dnSwZECtozwAwCuZNZbpHaeO = 8976627914141987; 
byte RnEeaPMlcxCtuiCHKHTqidielHih =  48; 
ushort elzKfxJYlFwX = 42986; 
byte YlTDgAyVOOeATuHFbFexgVzwj =  7; 
long NOfCSuIWnIqSIpLVbXQ = 54980227985937255; 
string ysfQRiZJEHzMLpunmPRkEtm = "eaNuNYcLG"; 
sbyte plbRhLmzpqSUKZdpaTRkefTMQ =  -20; 
int GFNgDBdckYJBpVWUlfdwKIbSOXM = 34; 
while(GFNgDBdckYJBpVWUlfdwKIbSOXM == 34){
GFNgDBdckYJBpVWUlfdwKIbSOXM += 100880;
}short MFLsjVbfcjqUiiZAKtklXEUtFO = 24949; 
long XXwYckpCOTAIDYJxdEbsQptUfVi = 34721033966506832; 
byte VgYKYWALEuFlWCkSseHFBTgcGia =  116; 
uint sTyBhxcJSuT = 53; 

}public void EyxlkiZezHwcaWdQW(){
string mbMPwtOQJm = "LqqFzhMOm"; 
short nuQfexgGcySV = -14384; 
uint JbSytpSlVdMwyVdhPZVOsZpos = 6279048; 
byte TwcfHALVPVbwiUbtaJLZpdWt =  181; 
uint LEfKmHxcJKoRxslZdUsie = 9196; 
short tlA = 20294; 
uint ugPZsKkOPMStJMSoHddSz = 3203; 
int IOKUwXljaVKHsUo = 117847766; 
if(IOKUwXljaVKHsUo == 250581){
IOKUwXljaVKHsUo = 906313;
}byte ieFwMHkcQjDKVBUtPGj =  170; 
float IojOZsO = 2.739214E-12F; 
uint iCQQCADPSTxPkt = 2326; 
byte IMEPaxolXISnWSanMZdD =  62; 
sbyte TYolBZwHgEkLQRuCuBpD =  105; 
double WFOWIthIwCeLQLkPmD = -1.131838E+18; 
double KcWELWtsHMcNnYWQ = -3.735382E-05;
WFOWIthIwCeLQLkPmD = Math.Ceiling(KcWELWtsHMcNnYWQ);
Console.Write(WFOWIthIwCeLQLkPmD.ToString());uint BqpkstgyGITj = 43; 
sbyte zfiClXnHuE =  -119; 
double tWHqtnyG = 8.364273E-31; 
double IosTMbuWgEcKRP = 1.435424E+13;
IosTMbuWgEcKRP = Math.Sqrt(3);
tWHqtnyG = IosTMbuWgEcKRP;
Console.WriteLine(tWHqtnyG.ToString());short VlwmLuVXpGAYJXMDSgJj = -30652; 
string beCxdRU = "CVGQaxDyoONLGwdtIU"; 
ushort GYhF = 58612; 
string hjXSfxbXccgLf = "KtwnpDGbhPi"; 
ushort ZDNaWXcwnqWInFyaqCjqNWFNq = 38320; 
uint fbiJMGcwpbkOdUwczHcYhaRm = 53277147; 
string XZKq = "mpmhmWICcKopwn"; 
float DnLDDHejVKJDZyps = 5.394741E-12F; 
ulong hUlKuxfqCXEPjDwMZP = 72123455271355877; 
int uXycynsJcaigCOJYfTs = 44624; 
if(uXycynsJcaigCOJYfTs == 76608){
uXycynsJcaigCOJYfTs = 489168;
}byte fIJynUAWFABsW =  47; 
ulong ZHMfyunytNfDPFtEmUmbt = 79097561991327103; 
uint mpVtyNeWMtldBP = 141405382; 
double XebWLo = -6.750222E+17; 
while(XebWLo == -2.285621E+08){
double qSmUlWQiXCIhAtyeKesIxwqGGX = -2.872454E+22;
XebWLo = qSmUlWQiXCIhAtyeKesIxwqGGX * 2;

}int HfdOnjB = 4336; 
while(HfdOnjB == 4336){
HfdOnjB += 516967;
}short DzaqkbwjgjQmxcZzZEOxoZuJiCF = -1333; 
float JUFGeQoUsgk = 8.725272E+33F; 
int WeqyThXNUHDeqhK = 938200; 
while(WeqyThXNUHDeqhK == 938200){
WeqyThXNUHDeqhK = WeqyThXNUHDeqhK + 923504;
}
}
} public class QXjielHltcQklymbaiMg{public void ZDLXMJLnGgcCnnumKcLfJ(){
byte ngWliVpHPPYRwzlt =  142; 
double OVOJYiTPpgabaMcyQQxCtpBmNpxY = 0.004422007; 
OVOJYiTPpgabaMcyQQxCtpBmNpxY = Math.Ceiling(Math.Atan(-5));
Console.ReadKey();string ZCpqufib = "WISVdcMOhqitbWpwolOsRcilnYS"; 
uint TggZ = 68008; 
long KKatnIQSRXFZVeT = 64941425948364948; 
byte MojbtzTG =  242; 
int NYDutuxamDgOnNpyj = 8422; 
while(NYDutuxamDgOnNpyj == 8422){
NYDutuxamDgOnNpyj = 184615;
}int GRzzLtXAqILD = 121963; 
while(GRzzLtXAqILD == 121963){
GRzzLtXAqILD += 949904;
}long BWocaJwfDYWPabNEYC = 29131939198084221; 
long GRzTC = 22936718654757845; 
float tLVDytJmEwAwlajxfYqBLAdmcidb = 1.641832E-34F; 
uint qqEuKtmL = 591623846; 
float cDHJsZsdHqmTHJB = -1.92131E-16F; 
long BVndVxPMMLGFfEHkADuKE = 19793685229063659; 
float wnUwDOALmDmGsaNgoayaMVYgPzLdR = 4.632296E-38F; 
long MjpkidxZShgqedDmXuOESlBpOG = 38223126675765560; 
int yVgWaaUNRPKuhQUsmgbBJZxDa = 5732; 
if(yVgWaaUNRPKuhQUsmgbBJZxDa == 581939){
yVgWaaUNRPKuhQUsmgbBJZxDa = 792753;
}short MQbnRERsOzZVJWG = 31773; 
sbyte wMfhyzEVIy =  108; 
sbyte asWwZTewE =  -81; 
sbyte CyHsyXZBouHsFtDKVWuwJkZEMBH =  -97; 
string SDXlVbWEjAXFYGMFRHpwAHPNmp = "dGXqSzwDeuszCNq"; 
ulong zemtSgJaQIukNkZFDTYmhefEdIk = 35895588261144850; 
sbyte elzn =  -45; 
string bqqitTYUtANtYKojWjSaZlpTX = "CyIAYRcoCMpcfoRNPwgUkuQb"; 
int AbRQ = 46; 
while(AbRQ == 46){
AbRQ = 505095;
}byte BWqJhDUUeDjlxUMcGhIHWVqayML =  216; 
float YqBUVkDClmgJZWRGth = 4.785099E-26F; 
double SGXGFql = -8.923417E+25; 
if(SGXGFql == -223.145){
double YCAdWwAnVuiSsEfIGP = Math.IEEERemainder(3, 4);
SGXGFql = YCAdWwAnVuiSsEfIGP;
int[] GlpQweGytYUR = { 1490136 , 37021 } ;
Random jJTmygKKj= new Random();
Console.WriteLine(GlpQweGytYUR[jJTmygKKj.Next(0,2)]);
}string WwVPuQFJVm = "fauJzbkYCuVYNCbOmHxeWMf"; 
sbyte CEEdlAow =  -104; 
sbyte MblhXuDUayimtc =  -39; 
string iRuUkzWzHVoVHDIu = "blwngCiliNtpkaUyPm"; 
byte DBDoKpAnYtUCTOagIlbokIc =  185; 
int wUwzSp = 6401; 
while(wUwzSp == 6401){
wUwzSp = 102160;
}
}public void RcVmQSRyxaNPEkYCVz(){
float DUGSBBFhlNAzDHkmpulTMKZ = 8.292465E+11F; 
long ewfgcRtbWy = 6323903755190834; 
short WaLlBlYaS = 26862; 
long SlAMfGyH = 1857195317117603; 
string pstwseLUGQTsiaL = "VcPYfdeJdqLVYTotZuGtSxUub"; 
byte HNSxnewNVQdtlnKsbtQVNBBtPwqaz =  41; 
ushort TeyVlUxQEguEVbufhdSakpmgmAHT = 49837; 
ushort NwgAhROggW = 326; 
short gWyOblTwpuMDRqJMnEc = -21513; 
float iOClntWMdkYHZpbTSCCS = -9.233778E-38F; 
ushort pBfuTtYiNAOKlPXAzEpBFkqsLuMB = 42112; 
string htFWOmCT = "mPTADsHcEbtTMUsfiNKzIy"; 
uint oHpITjmXLFnQsSQtOJXGIqx = 238285; 
float BSudPmgjVbYiuPxhgGD = 5.794652E-14F; 
double KYBWfXkSwcF = 4.687293E-32; 
if(KYBWfXkSwcF == 0.001784989){
KYBWfXkSwcF = Math.Ceiling(Math.Tan(1));
Console.Write(KYBWfXkSwcF.ToString());
}sbyte tNOTqKLK =  8; 
int DPHqlgyQjSgAUOmiX = 32290850; 
while(DPHqlgyQjSgAUOmiX == 32290850){
DPHqlgyQjSgAUOmiX = DPHqlgyQjSgAUOmiX + 686483;
}sbyte pbyknbyM =  34; 
string QTZeXtYtlnUeeCOVkpJMfBgV = "aFVQK"; 
ulong OxsePlqzS = 8442833537876267; 
sbyte wYnLoNeZW =  73; 
string DBIpJiKgYZjUPHiAiJ = "wKysoAMdsLmuKPNqyOGKBQZnxOu"; 
float EFRnUHUeitQXfsxMGBec = -9.443647E+17F; 
long YuGKiVBfQuOfxfQhT = 83503863495769261; 
uint noStRTooAbjWmSLgLEppAKXjc = 317543; 
ulong ZhfHemRXb = 56176646702267929; 
string eGKUnmqEsJptDCdBU = "oiFTi"; 
long diJuRBMxOIHomRuNKAo = 6725781484859870; 
string XlyankdJiBhEWPUxkImVwVuUXb = "EVEFWLEQxioYH"; 
short BYhhI = 26271; 
long GFqgycREclR = 77764661977774977; 
ushort VJGFqwwMQ = 25474; 
uint FYaGFfOFGlMQPqVcWonWncKVM = 133968; 
long RlSsJhHVGnnjSI = 85468044209894331; 
uint DECoeU = 96; 

}public void iPZUzg(){
sbyte hqaaMxwaYW =  62; 
sbyte daLMPfH =  -44; 
ulong kodKSKcpJQkkABeRhfogXp = 41240827312275000; 
int qGQMIMEpWGesWSCkIYYBbKOVyeFd = 905842; 
if(qGQMIMEpWGesWSCkIYYBbKOVyeFd == 248883){
qGQMIMEpWGesWSCkIYYBbKOVyeFd = 258088;
}float KtiwDYkslAblmdBLccVIRVnEhA = 1.568208E-37F; 
double eKYIBoQDXdAxKYc = -1.372476E+24; 
if(eKYIBoQDXdAxKYc == -1.588687E+31){
double cPeFnccjsOu = 1.080281E-24;
eKYIBoQDXdAxKYc = Math.Ceiling(cPeFnccjsOu);
for( ; ;) {
Console.WriteLine(-2.473242E-19);
}
}uint zEoVAaCZRbKl = 84; 
float pMRKyQxcPPSI = 1.953871E-28F; 
ushort QDCMOZ = 5332; 
long mWhqSVIlnncmPVIgmg = 44059083102297008; 
long gEpIhgkToMX = 69404629109446269; 
int FCTuBJmlDPxnqeLYwfJLOSCiRY = 295131; 
if(FCTuBJmlDPxnqeLYwfJLOSCiRY == 640898){
FCTuBJmlDPxnqeLYwfJLOSCiRY += 411121;
}string DcxnIyYIMWBGTq = "VYP"; 
short eMaKDCkXuphamUDICgykNgfFJke = -24268; 
short nSdfsQcMLflamxOAsYPstRsghYGw = -30490; 
short eIzbpXgDMGPxqZ = 15748; 
float QiTgqXExBXYtjHKtKhsPu = -466.3391F; 
byte dNEwahweWGTzpN =  182; 
float QaiiCYfcIRueosGHKnj = 1.855996E-37F; 
uint DonbRUtwHUPJDcmCOztspbaI = 2516; 
float zCVFpkHUnXnhoxNjAJd = 2.533691E-06F; 
ulong CIhzBoGOEcNWfZ = 85544542566777586; 
byte UoQh =  149; 
sbyte jMetWhMfF =  36; 
sbyte eCAHjnpNhdzGqxeFlzzjw =  74; 
int bGnkFpOG = 50564618; 
while(bGnkFpOG == 50564618){
bGnkFpOG = 647463;
}int nxfgmAcBdhTfjZopYnGFaehSoT = 817922800; 
while(nxfgmAcBdhTfjZopYnGFaehSoT == 817922800){
nxfgmAcBdhTfjZopYnGFaehSoT = 587132;
}float VYKyYCDOhyxIxJkQEg = -1.400855E-36F; 
float nmRuCljC = 1.065724E-16F; 
byte dpGBpeblXLwHxoGBYbD =  214; 
long LxGEZpFHzhwSFUkcRXaHmE = 7433677803114455; 
ulong TsGiVNeLIpmnlF = 54818412034294095; 
uint FxGNKU = 51; 
ushort YlobheSoZWpFXuCWuIMgEPTzFAgqb = 49778; 
byte SmGAwnOgNDeDRazllNBxPn =  105; 

}public void sQyJXBn(){
byte FMLObBgTGubLqHEaVfnmQ =  241; 
int ygFReilnTSmnOzjmtDjUuzaiT = 5025127; 
while(ygFReilnTSmnOzjmtDjUuzaiT == 5025127){
ygFReilnTSmnOzjmtDjUuzaiT += 876287;
}int TpVVpP = 165354; 
if(TpVVpP == 473570){
TpVVpP = TpVVpP + 389336;
}short xWbTmOLyZ = -24783; 
ushort xgQnUZGgVhoqdONLGHDPExGAYeGT = 28751; 
ushort PAssosBWAOi = 21875; 
short LPAaZRtEHuaBqoQtNKAR = 354; 
float wytfKcaweqSnogSyEXkTXs = 5.345908E-38F; 
byte gbzSyg =  221; 
string DGLKC = "ZVjAaBlUmVTpYPcJsVD"; 
double WmTLwlcpRSnJmYxFCkGUglWERVobU = -1.124927E+24; 
if(WmTLwlcpRSnJmYxFCkGUglWERVobU == 8.86767E+08){
double FojFwcGHMQGdRKWbTLqoUBXa = -3.900219E+12;
WmTLwlcpRSnJmYxFCkGUglWERVobU = Math.Ceiling(FojFwcGHMQGdRKWbTLqoUBXa);

}uint fVSPddFsqVzqKqAFGg = 918933806; 
uint ExmTCZBtXSAHdDEatcGlyxTLkO = 591561428; 
int eEdmswDkDFRXXGScmWbFNT = 2785; 
while(eEdmswDkDFRXXGScmWbFNT == 2785){
eEdmswDkDFRXXGScmWbFNT += 551812;
}long hVPlNVERjiziAauAAnThBVE = 66075955952883695; 
double PyWiLSWAwMSgEtoUW = 5.830351E-17; 
double CIPaYpAHKEz = -9.439107E-13;
PyWiLSWAwMSgEtoUW = CIPaYpAHKEz * 2;
Console.ReadKey();long KwjkzilzoMJCiwgXtHIEPouOysCtD = 28027285573399552; 
ulong BzyRHeIxMkWGJ = 15725003447211513; 
int VJjTypYzBGmhfszNJQVQhGaTt = 68; 
while(VJjTypYzBGmhfszNJQVQhGaTt == 68){
VJjTypYzBGmhfszNJQVQhGaTt = 996806;
}byte HCiylkgNMwawoZSWnNXkQKoIpA =  110; 
int SAGZnARpoxGPdxkazebSftncccWl = 146380; 
if(SAGZnARpoxGPdxkazebSftncccWl == 503675){
SAGZnARpoxGPdxkazebSftncccWl = SAGZnARpoxGPdxkazebSftncccWl + 810409;
}float uxaBaHYpswNneHSuwORzlEWkDFP = -4.94013E+13F; 
float nCoGbdCRzu = 1.26229E+14F; 
long JgTpssMYxKQ = 60699818814884240; 
sbyte NQaQpUXseoUoVUyV =  88; 
short EofGQDoEuHDphCR = 27755; 
short AczOHqwWjUdYWZqcDkHjkmbpDM = 1998; 
double NeYgcsnZPIOkTtuBxKEZfZ = 2.648408E-17; 
if(NeYgcsnZPIOkTtuBxKEZfZ == 3.10415E-20){
double QxjxQeRUHccODTZsUIPgbXZHOzJ = -2.534822E-26;
NeYgcsnZPIOkTtuBxKEZfZ = Math.Round(QxjxQeRUHccODTZsUIPgbXZHOzJ);

}ushort lacEEumSfMVzyUgqf = 16882; 
byte NpwhRZoDWHwoZFIRuZfXiZuPUOOUN =  225; 
int RtOVFUNEfLWzqGzTmoXfh = 8246; 
if(RtOVFUNEfLWzqGzTmoXfh == 620031){
RtOVFUNEfLWzqGzTmoXfh = RtOVFUNEfLWzqGzTmoXfh + 271795;
}string uPbdLzHQM = "AmCSzdVzo"; 
short IzXyDDDbYYHKqMWUuHgRdBcyTGy = -9458; 
long hZO = 79814419716132074; 
ulong nTDjfDiBRhzRgxBFWfjqXQH = 58990887820779850; 

}public void oNOEpwqQCBqRpAFUFbkRTyR(){
ushort HjfXtMsXK = 35946; 
ushort WnBOuQxAPGBwLkNnuURW = 21234; 
short lkThOYUGXkmINKcAIuYyb = 11665; 
long hZeEQLzqG = 67441666249596288; 
string jzDZEqujCsUIDAHZPU = "QlNWchwzYZN"; 
ushort afCOGaClwCWYMXNdU = 43767; 
float FkqyDBRhTxkPVsHxYTu = 6.305848E-11F; 
sbyte LQYpjIwepTRXQ =  70; 
string ejttZphc = "qpeanemAQHHFj"; 
long zsUVduRIVIpFaMwu = 85389436029814936; 
ulong zZhxHUDTVIxlZaAIjs = 18930683428756321; 
byte hinSgAVRBzawchRNYyVWtKf =  182; 
string sLFVzYHOLM = "EeBeJhLqEzeWI"; 
short OquzcDPThkmFacOn = -3964; 
sbyte utwSLCutAQmPW =  36; 
double oulKQPkt = 7.274356E-07; 
while(oulKQPkt == 1.169689E-17){
oulKQPkt = Math.Ceiling(Math.Tanh(0.1));
int[] eKGFYJNPSQLpaHAhzzfHI = { 3166786 , 44048 } ;
Random ppywLoaoCuYZeGy= new Random();
Console.WriteLine(eKGFYJNPSQLpaHAhzzfHI[ppywLoaoCuYZeGy.Next(0,2)]);
}long maKiUuWf = 16808662771133676; 
short DYyPDn = -11449; 
short DTL = -23248; 
string yuwHhEbOjaLbkHY = "lhjUKMzywSDtul"; 
short yMUHmNhEBjnORm = -24573; 
long fIDX = 24519290331415352; 
long XMnEzNqizaHtA = 14872043010456377; 
int ApXjLoCRDSQpbEJIYMXjDq = 27653079; 
while(ApXjLoCRDSQpbEJIYMXjDq == 27653079){
ApXjLoCRDSQpbEJIYMXjDq += 65691;
}int tfuaOnB = 634541; 
if(tfuaOnB == 166482){
tfuaOnB = tfuaOnB + 71115;
}uint iIcAdUEquiiDKYzXzmwAL = 20496538; 
ulong gwedmummFcFoE = 62371402458749632; 
float iWzVJcYmHneWAAngPV = -9.489466E-33F; 
float JULhKWWiiKpQWhPzFkHPXaAGOp = 1.527382E+26F; 
string wNKomEjBscqzeuGJEgspq = "zfWIw"; 
sbyte uwdfzoEgHUkne =  88; 
int AWeZfNuBNuooEsF = 9; 
while(AWeZfNuBNuooEsF == 9){
AWeZfNuBNuooEsF = AWeZfNuBNuooEsF + 261912;
}short ecCWlgcmmUHyMpgnHaD = -9164; 
string nDopcjXTXkgpZWEQitLhbwXVTSXTq = "LZCeOYIWeTouXxpwpjK"; 
float jIXYTGtxjleCqBhOUhGilnRSIy = 1684443F; 

}
} public class COiCNBqFgVsnAUH{public void SGWiHxbe(){
long VRepwfOZJzVaEZQjNmm = 81918888234573016; 
ushort fMeTgGBARAmABlioRcWsyJPR = 49125; 
float HyBEBqiDMRhxtpLWO = 5.862046E-08F; 
byte bsYPIe =  94; 
ushort mgmeUbpVNhqKyzKGEcMMaSsItFeD = 51497; 
float PfPEbjowJ = 1.451846E+26F; 
byte UzFUniecH =  182; 
double OFLoWOgTVCEIU = 1.298873E-06; 
if(OFLoWOgTVCEIU == 3.855468E-34){
OFLoWOgTVCEIU = Math.Ceiling(Math.Atan(-5));
Console.ReadKey();
}int etEOOSasknHZLfnjVfWlqpFV = 775521; 
while(etEOOSasknHZLfnjVfWlqpFV == 775521){
etEOOSasknHZLfnjVfWlqpFV = 645495;
}sbyte OuLsSdWHqVlXguqYXAXSTtyOOJp =  -128; 
long BEgMlpuWqSjoUxdH = 20174644776012368; 
string XXEiNVXRuJQwi = "Ebh"; 
sbyte EAbKhVnDzXiTSJRsyshf =  -59; 
short bwRt = 24171; 
ushort CjFHzVVmxnztAUlQYMs = 43721; 
sbyte ZaumyaopCwfSxZnlkVxjV =  96; 
int RiQFTHBOASjFllyVYfMdmIylCMC = 233456; 
if(RiQFTHBOASjFllyVYfMdmIylCMC == 118075){
RiQFTHBOASjFllyVYfMdmIylCMC = RiQFTHBOASjFllyVYfMdmIylCMC + 374779;
}uint okKyVUssFQKZZy = 231155; 
ulong DIjLEd = 72040771644470451; 
ulong iLnJRjVoTxSNCYLLYYRSkFHH = 22004092771772588; 
byte oQYgptM =  229; 
float luFeRT = -1.554742E-31F; 
ushort GFdHscteBsKtAsKUMRJyfpNAwWk = 46873; 
double wHesEKNqPnZIuzySUlwWgnxWPoS = 5.556572E-22; 
while(wHesEKNqPnZIuzySUlwWgnxWPoS == 1.485825E+12){
wHesEKNqPnZIuzySUlwWgnxWPoS = Math.Sqrt(4);
Console.ReadLine();
}ushort OLlbzqsECJtSs = 2261; 
ulong SgFBhZpJYgWgNJZoDVpeViKkW = 29113779482354652; 
long xhWmOuDdqwkRTKeox = 23326880812720382; 
ushort JuhwhbgDhHJCLtetQLyNMVnDOoQ = 54960; 
double stuAUJiZdPNgXJOkZCFeETx = 2.120369E-25; 
double FtnoglAwjRHJ = 1.8337E+26;
stuAUJiZdPNgXJOkZCFeETx = Math.Round(FtnoglAwjRHJ);
int[] OEonmUjAWOpgEEqlDyDE = { 5515340 , 95159 } ;
Random AdOciVNlOHFOBRe= new Random();
Console.WriteLine(OEonmUjAWOpgEEqlDyDE[AdOciVNlOHFOBRe.Next(0,2)]);string iOstVsSGAwfDbujoBBldZ = "nigLscNmJGwSuHg"; 
long KqonQIdDuqCFbLZLMBjlkaQTlM = 67639460138064750; 
uint LjyBdhYMkJIUwhJgXzIKCw = 9; 
sbyte uuOtOtimaCdJidbdJDeGMbqxlqaT =  -75; 
short ttgOFYS = 26298; 
byte ihhdooFIyNAZCkscshxtpc =  181; 

}public void LocoyfQIRVDUi(){
short cwkNDItRVwCHKzVugxNtZmojsWSi = 8026; 
long TYkuXeoiMqzgUVA = 41092730255193193; 
double gawHQjkmWIIAdXoSnbbBfwoT = 5.375751E+12; 
if(gawHQjkmWIIAdXoSnbbBfwoT == -1.067819E-20){
double FRdUDWgmUYn = -0.0009192075;
FRdUDWgmUYn = Math.Sqrt(3);
gawHQjkmWIIAdXoSnbbBfwoT = FRdUDWgmUYn;

}string RVfqsMPjhTddiwHRaHyoejzoLBIFJ = "scgJwEbT"; 
int qLe = 3697; 
if(qLe == 771618){
qLe = qLe + 920330;
}ushort buOVIxRGRejCXCbWBZcQgayYue = 9178; 
double JTJkqqIsNOhjmPVgjhQsaqP = 2.484908E+10; 
JTJkqqIsNOhjmPVgjhQsaqP = Math.Ceiling(Math.Sinh(-5));
try{
int CRkWqENseQ = 9556336;
if(CRkWqENseQ == 2255){
CRkWqENseQ++;
}else{
CRkWqENseQ--;
}
}catch(Exception ex){

}float wYpHXsfQgMMcTJbBabqyFRmq = -5.608332E+14F; 
int NCSDJCoEVEV = 302236524; 
if(NCSDJCoEVEV == 867202){
NCSDJCoEVEV = 146392;
}uint pwdDVKyjFMZnnOPcFUiustz = 437517; 
sbyte hOmtjTXnVJZWT =  -74; 
short RWpIhBOZZYgwXWAfPooxqt = 20039; 
ushort BUtGXpiaIIJ = 8228; 
uint IZmukNDpZ = 86; 
sbyte VVtzgWqIHwubJXupoQ =  123; 
short IyeY = 6274; 
long kViMQMmfniDkgFbESsDPhZK = 60473320912610598; 
uint EyLlIkWLoc = 27; 
ulong yHTlHcS = 47872391685572668; 
long JaWEhpskgGdSWFffIdIFcTzxXlByP = 53396078820852111; 
int qCbAxONnHKN = 1313; 
while(qCbAxONnHKN == 1313){
qCbAxONnHKN += 805053;
}uint GVQVxVUqkThSlbNq = 4; 
ushort IjXT = 49515; 
string FwFgHMtZwBGFFDxDwARBptaZUx = "RwXWhuboWHMBgTcs"; 
ulong xQAlIDuVBEluFmk = 64822017592561669; 
byte VJYLInbkCzGaGOuC =  59; 
float dDbApVqzUkZlmkL = 1.52345E+29F; 
ushort TpjicOFk = 60422; 
ulong clqe = 16627752683532681; 
sbyte Dmt =  77; 
float Bfb = -1.278549E-26F; 
float YLLLwJQhNUia = 1.313107E+27F; 
long wDCciBClubPEXkqWsFVzWfV = 35824847027722931; 
sbyte qHyHRjzjApuTmjDDmJjXlRpH =  -26; 
long LicFUHNGFTo = 44474677768569958; 

}public void ImQWg(){
double EApRlfTN = -3.023498E-05; 
EApRlfTN = Math.Sqrt(4);
object oaecZGdogYZhMHfRLdAkoH;
oaecZGdogYZhMHfRLdAkoH = -1.081361E-07;sbyte Ghm =  -93; 
uint JDayqYVpuIKSBgoGGAcmTWYN = 650443; 
float GgtjONjNzloXRYw = -1.379593E+14F; 
int cMZDNekVNEmbKdyLmn = 80154; 
if(cMZDNekVNEmbKdyLmn == 538111){
cMZDNekVNEmbKdyLmn = cMZDNekVNEmbKdyLmn + 145511;
}ushort exVCYdTe = 55893; 
ulong FHfDRyikkGUtmhMjzPf = 53111630297284543; 
int iFkKeJI = 5208; 
if(iFkKeJI == 359982){
iFkKeJI = 930825;
}sbyte kcItlTwoNEOxdfORkDb =  -117; 
string JiAeQXTGRVZXOMsSupNoz = "qlEnNe"; 
short UgFCtAUgdyxgMSxgscAtiMoL = -19446; 
uint mBDkPqbbTtlsSXnmpTsuAQyWK = 90960212; 
long STqRJIG = 31392253214399374; 
sbyte HwUAYSShsxdHBixuSbAVB =  -79; 
short MclDKInKRZYAMJTbzKpl = 3443; 
float ztqYCnjjyfmOWGpzKitQBkW = 1.222092E-25F; 
sbyte xRTeeRpOjsKNwHQ =  6; 
ushort VXxikHcjelmxJPtR = 50523; 
short bhWGSffZxXjLKDhnlb = -16188; 
int bVNeBNJUu = 850799; 
while(bVNeBNJUu == 850799){
bVNeBNJUu = bVNeBNJUu + 335494;
}double EdfHHlVBEUC = 1.857256E-29; 
double Cssxebsqn = -4.325088E-39;
EdfHHlVBEUC = Math.Round(Cssxebsqn ,1,MidpointRounding.ToEven);
try{
int UzjBGuKRFWT = 8115127;
if(UzjBGuKRFWT == 80381){
UzjBGuKRFWT++;
}else{
UzjBGuKRFWT--;
}
}catch(Exception ex){

}byte JCGTElSKKmmqqlLHDqWwzn =  184; 
ulong ZDdRMzemzRePQnsei = 61575299805339712; 
uint TQlwQQqEJOTYPhd = 402297198; 
int NdFqpQFPdZegejZlylxnYzKJp = 4900; 
while(NdFqpQFPdZegejZlylxnYzKJp == 4900){
NdFqpQFPdZegejZlylxnYzKJp = NdFqpQFPdZegejZlylxnYzKJp + 148830;
}string UPunzWFkamF = "IOPPWHGYsSnuWaZqmXJ"; 
float Pkw = 8.003573E+33F; 
double ScYledJDLDW = 277.0767; 
if(ScYledJDLDW == 5.881238E-36){
ScYledJDLDW = Math.Ceiling(Math.Acos(0));
try{
Console.WriteLine(ScYledJDLDW.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}long cKxJcwAFti = 83353547221129356; 
uint qdPeWfawUe = 8810; 
double SXWROEROEXoxBhcOhFasXYVq = 2.441243E+33; 
while(SXWROEROEXoxBhcOhFasXYVq == -1.5256E+16){
SXWROEROEXoxBhcOhFasXYVq = Math.Pow(2, 2.1);
try{
int cFeRjNcphlRgaEOHRfVuAcClsB = 3276854;
if(cFeRjNcphlRgaEOHRfVuAcClsB == 94800){
cFeRjNcphlRgaEOHRfVuAcClsB++;
}else{
cFeRjNcphlRgaEOHRfVuAcClsB--;
}
}catch(Exception ex){

}
}ulong AuZcSzJcyZhbRHMhsiVuk = 36833828921303454; 
byte cxqbUytFybIdkCDSQhIahbszHeHeo =  145; 
string kZfFAG = "KZFztEQfRwMeAnlSadTuQQnpT"; 
string SkxXskpXbDDZURZQ = "DACSZgeCGkbUMoGtcXUuz"; 

}public void TjqkozeZERaEO(){
ushort JSeqxNZndonXHST = 6328; 
ushort iGsCOSHyH = 41514; 
byte hCkUufQdbHTVnVaBygLMGsFPfg =  19; 
byte ZgtdjhNMMyOosjDMxnTRfZ =  124; 
double nQWxPmbEscBBRdO = -5.631373E+20; 
nQWxPmbEscBBRdO = Math.Floor(-2.745696E-20);
Console.ReadLine();sbyte cTdlixyzOAkLmbaSeKfRmMR =  -7; 
int LWWSQVEmdihtbKgTApNqOGUCaK = 32277484; 
if(LWWSQVEmdihtbKgTApNqOGUCaK == 38697){
LWWSQVEmdihtbKgTApNqOGUCaK += 589484;
}long LBDWJpgaYS = 11257654581967812; 
ulong gfcuRfkmM = 51165502868089299; 
int yjNIcRpbnGgdBTwhGzie = 87443; 
if(yjNIcRpbnGgdBTwhGzie == 215790){
yjNIcRpbnGgdBTwhGzie += 715123;
}sbyte WzedKUndtyaDCeWHGJGKqFshhAdto =  -7; 
ushort KqfQCduGFqP = 10177; 
ushort DzhnJDfyJqfWVkQVOUb = 44613; 
float YgfDnZfZ = 7.790795E-24F; 
long WGkbQUllJOKFhjQJE = 3521130146958168; 
sbyte fsJDVlsnkL =  61; 
string yICkxBu = "jywUd"; 
double EfjTLGfUZlPtQVFXRRYpeQoFH = 1.861407E-25; 
double LeBTtCqNDYIhFiFAFd = 4.659571E+25;
EfjTLGfUZlPtQVFXRRYpeQoFH = Math.Floor(LeBTtCqNDYIhFiFAFd);
 bool? RFmYxkZMEXdokAxXyyYsnpX = new bool?();
RFmYxkZMEXdokAxXyyYsnpX = true;ushort eoYecIGIHuABHORxUFdZj = 25583; 
uint lBqcsSFyV = 3880; 
float lsb = 9.354841E-32F; 
string zNpSEImhNwAKQ = "TDpDpPQcGtuZwIlbafsePz"; 
byte AKFxKBOKbpsQLxP =  62; 
int hXzEmFWLuusLWu = 8313; 
while(hXzEmFWLuusLWu == 8313){
hXzEmFWLuusLWu = 371238;
}short FKXep = -5093; 
short pfAspmaVyZBYftsptXz = 25139; 
ushort bldcsPH = 9618; 
short uPdmkEdeRghkSuZl = -9997; 
int sXylgdnonVkopLaEppeePU = 547; 
while(sXylgdnonVkopLaEppeePU == 547){
sXylgdnonVkopLaEppeePU += 363729;
}long gxZx = 16980814150486479; 
ulong GSSBSQJ = 47079323339789008; 
sbyte VFjPOoUOz =  29; 
ulong VhBdlyujpCTwk = 32331062915610433; 
double KHt = 10866.34; 
KHt = Math.Ceiling(Math.Sinh(-5));
try{
Console.WriteLine(KHt.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}short coVMMpkRx = -20139; 

}public void XASFdtfHfmCNJBRzfiok(){
byte uWDTBZnmdMDtuOOtxXJdm =  0; 
uint mlVSeIgZxoeWcjTft = 171752176; 
ulong pfdLNMqC = 88044599668828015; 
ushort wFEWfJR = 20447; 
byte DKuRK =  91; 
double RhqVPy = -8.31827E+27; 
RhqVPy = Math.Pow(double.MinValue, double.MaxValue);
Console.ReadLine();short lVebQYWdmJQuQBTP = -29195; 
ulong SQc = 70209585577758326; 
double CJEAP = -1.601291E+36; 
CJEAP = Math.Truncate(CJEAP);
Console.ReadLine();short JuDQWpyqQ = -22992; 
uint xTDOjWglOAWFyBdlV = 949074; 
sbyte pfQXVOxLTKpEwGjyK =  -30; 
sbyte cAiyYtx =  -65; 
byte skyZfNMRohLBUP =  156; 
string UTO = "eRWARWsZ"; 
uint jBysfgxByLTf = 471549389; 
uint tRXqCuxkmtBAoRuSNQ = 4884; 
ushort AUGuxcsZSyXlDCb = 13111; 
float dmeNGWkHqbshXChWqdMgXtTn = 5.257224E-35F; 
int nTQD = 718653783; 
if(nTQD == 352106){
nTQD += 753745;
}byte jQMpHLAweHjXzKSKuRlDt =  39; 
short lQilFWEFQbXhIDH = 20743; 
int YHJSdo = 248487138; 
if(YHJSdo == 527323){
YHJSdo = 723551;
}short yme = 21488; 
uint QhcFhyNKbR = 4099; 
double jMPtMTssf = -1.714799E-34; 
if(jMPtMTssf == 1.377235E+32){
jMPtMTssf = Math.Ceiling(Math.Cosh(5));

}sbyte aKHKBncHEiclEZEAQXmzHu =  -76; 
long nJCQpXTPaiUZlAQfRaEZNO = 10050782658128200; 
int XXlFBQmkjyERJAF = 430903; 
while(XXlFBQmkjyERJAF == 430903){
XXlFBQmkjyERJAF = XXlFBQmkjyERJAF + 630714;
}long AXOtTpthVDwcnmlFbNsQpkVeXTjdn = 39641888314810277; 
ushort VQoJnmypkkOOH = 17873; 
float obKwZfpTFoEPoBFSFsyZa = 6.230013E+08F; 
float EYTBCHEspyGccwPB = -4.568732E-34F; 
double JaKGWjnRVINwSUxgxtcJZ = -1.248565E+36; 
if(JaKGWjnRVINwSUxgxtcJZ == 3.667983E+22){
JaKGWjnRVINwSUxgxtcJZ = Math.Ceiling(Math.Tan(1));
int? capLkb = 3776805;
capLkb += 79289;
}int ZZwCyswHCJHyL = 56; 
while(ZZwCyswHCJHyL == 56){
ZZwCyswHCJHyL = 433533;
}
}
} public class eeQUnpYzlKmRClq{public void aDSwpxsj(){
int ZoJXMYsfpVjC = 1272; 
while(ZoJXMYsfpVjC == 1272){
ZoJXMYsfpVjC = ZoJXMYsfpVjC + 826518;
}ushort VALtbcDbiIMGpWqKQnXnzwWw = 955; 
uint LbtbCOPxAE = 9314; 
long qXIjEkqPKhgThp = 38120004129705596; 
ulong jSCARgAqnShznlymXWs = 56353075280873435; 
double WFshbZMKSXDIP = 1.430483E-15; 
if(WFshbZMKSXDIP == -1.072115E-16){
double VHFLCldDUSCYqmPqlmLEeYPEg = -8.891968E+30;
WFshbZMKSXDIP = Math.Floor(VHFLCldDUSCYqmPqlmLEeYPEg);
try{
Console.WriteLine(WFshbZMKSXDIP.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}uint lEdOVOlXPVeyoQmWmGuTquNUCNa = 860560; 
long cIWbAoVhbYPWaMEoIJRtHmGES = 65030446734530968; 
sbyte QtON =  33; 
short sVDyNhYysUbqjQb = -9295; 
byte fJZnqKRVKOVUWjbjVLbuc =  10; 
short xNcPVlOaFcCJzeFSHRyjDy = 30424; 
float ZVqqPHdtnR = 1.023895E+10F; 
string yZU = "UJsMQfiOlFe"; 
sbyte PkAVqxKPwzojmakEQajIhdjWnelha =  -24; 
uint fTPxDfgEkB = 193221; 
string EfYjwFUCQLbEHmkCO = "ZadukObLUHxIGZTNCfjRZUwFceuZz"; 
float KMzkdziUjiLL = -9.525435E-32F; 
sbyte cLULe =  -40; 
double eFz = -1.745359E-29; 
while(eFz == -2.526079E-20){
double gPQoslZIojhAUZsoFinsPzfzZSO = Math.Log(1000, 10);
eFz = gPQoslZIojhAUZsoFinsPzfzZSO;
Console.ReadLine();
}ushort QuZzOkaawHOuQSZYgpFEpTwiQTuoS = 53325; 
sbyte tJcGuIUypoeRwfbIihBLBQJXYQJU =  81; 
long FCRagQ = 17358225634835231; 
int KRNgLpuxEafHFZZHFhApEeV = 788095760; 
if(KRNgLpuxEafHFZZHFhApEeV == 305141){
KRNgLpuxEafHFZZHFhApEeV += 368445;
}int iubZdeGyoguyfLNZAwBJ = 274036066; 
while(iubZdeGyoguyfLNZAwBJ == 274036066){
iubZdeGyoguyfLNZAwBJ += 921363;
}float gnedatWmDZWD = 8.780508E+15F; 
byte iIQgcED =  226; 
float OUfRwhinpacqyNPAyAzXJidgbWg = 1.203781E-16F; 
int jkfFbhLFPtMukfSkcsDgcw = 218715678; 
if(jkfFbhLFPtMukfSkcsDgcw == 982852){
jkfFbhLFPtMukfSkcsDgcw = jkfFbhLFPtMukfSkcsDgcw + 809524;
}uint wYN = 8647; 
sbyte iNsMVKFXOSa =  -83; 
ulong xNxJ = 22137197812113447; 
ulong NIWmMTUFjuhMqnSYYHMXZlpFeRxjD = 28985143432436786; 
ulong TjRDcCcYHgkjnuR = 40377914012164025; 
string DtRLcMcjiTOhJmdbSQcsaEudHKNFG = "xzPDBhlmEwiZeBcRS"; 

}public void ngcHI(){
long YZNdtYzFHyotfNWPGlIjazTaQSJqz = 39506169202422319; 
sbyte xKwzdOyM =  -40; 
short xuaBZAgMnCph = -25592; 
sbyte znDlOpxblAGaPmsfpZhd =  -29; 
ulong FbbOVJfDoswoiigDba = 24513073106496164; 
ushort iYcnIKYjNCCtfXpJuXjXjxfdbbEkD = 46251; 
short mfuqohIAjWQBzaJdcQWBMzQMTZmHf = 17346; 
long lCfZ = 5474265620624082; 
int FasBhMOPnPpIzoVbtxo = 27378307; 
if(FasBhMOPnPpIzoVbtxo == 681096){
FasBhMOPnPpIzoVbtxo = FasBhMOPnPpIzoVbtxo + 500183;
}string KkbMDfnwlnGGVn = "mqKVCesoppYSqHf"; 
long epe = 2383049499813857; 
ulong RtARKFZFCqsWLwEmXOfFGBG = 18940496586338095; 
long EAGtBWCKwEZNICHjo = 66138653839954309; 
double pdkjgJwhWXUxSEVjXGylA = 3.125246E+17; 
if(pdkjgJwhWXUxSEVjXGylA == -3.284661E-31){
pdkjgJwhWXUxSEVjXGylA = Math.Ceiling(Math.Cosh(5));
try{

}catch(Exception ex){

}
}float iIXgEWJHZXPuALcN = 2.06754E-05F; 
float AZmXwIbB = 6.00935E-38F; 
float VUQAZtgqhze = 4.843873E-38F; 
short gbPxIxIajuZLVViQYczAF = 26406; 
int zpBbaVLVfXUAbEmUmUHeaJk = 994505; 
if(zpBbaVLVfXUAbEmUmUHeaJk == 484170){
zpBbaVLVfXUAbEmUmUHeaJk += 545748;
}float lycYq = 7.272587E+09F; 
double KdFXaJPuPF = -1.250698E+15; 
if(KdFXaJPuPF == -1.310389E+17){
KdFXaJPuPF = Math.Pow(2, 2.1);

}long BdCxKyLdJxLfboTWlOheKb = 40011828294927343; 
ushort kUfwWnGJHDhsi = 35020; 
int chFYKDbAgLfKRKki = 316026752; 
if(chFYKDbAgLfKRKki == 182785){
chFYKDbAgLfKRKki += 464024;
}string hAzgTFJOyfYLUzKITHnlhWRXsZsS = "xqFhIQjeUGDbsmMLAnCH"; 
byte DjOPlICOUl =  210; 
sbyte PuE =  -63; 
long EYaMkQKflTLRluhO = 11142400668677250; 
short klWpVaTg = 11459; 
short zEZDDoViwGNTbZjwc = 29727; 
string pwZFyiWXKXeLSlWQScZnkCVFzs = "pMGQiJyldEuettXAyBbbPduDaXM"; 
long AugfnGkegWcwZSlzYHVmcze = 16977112639312725; 
double zFinZfOMupEYHQlhebqRwLap = 3.528791E+21; 
if(zFinZfOMupEYHQlhebqRwLap == -2.845897E+27){
double GBmcjZBh = Math.IEEERemainder(3, 4);
zFinZfOMupEYHQlhebqRwLap = GBmcjZBh;
Console.Write(zFinZfOMupEYHQlhebqRwLap.ToString());
}byte PDMHpiIGJadfcLcwALM =  20; 
short gdkmy = -31011; 

}public void PDoUnnoXmNJgGxRnDcRPGMcXf(){
byte wJwdMHPUzOkOyOzlYcXNK =  77; 
ulong jGllcBqPaFToRKXBQCsAq = 11686114148017540; 
uint fTNTfuDRYyxOSVjuofxWhXpsWlkeD = 432053; 
int FNweQRgnLucVBCfX = 78; 
if(FNweQRgnLucVBCfX == 573516){
FNweQRgnLucVBCfX = FNweQRgnLucVBCfX + 855136;
}sbyte uXUiznDaQnmhOpznpJ =  -19; 
int ywLfhyHHU = 740377101; 
if(ywLfhyHHU == 691967){
ywLfhyHHU = ywLfhyHHU + 873193;
}double PyScfmnYiPpCxVxFpXCidqQVtVsB = 4.886378E+30; 
while(PyScfmnYiPpCxVxFpXCidqQVtVsB == 1.390875E-15){
double ksixgYJpezbpZDMsOUzHWJj = -2.318478E+28;
ksixgYJpezbpZDMsOUzHWJj = Math.Sqrt(3);
PyScfmnYiPpCxVxFpXCidqQVtVsB = ksixgYJpezbpZDMsOUzHWJj;
Console.ReadKey();
}sbyte bXfAsuNFDRVozKGGeeCzdckJIZucp =  -45; 
ushort NmABoTyiuKgUPkgkPuPbWJWZNyS = 23139; 
double nluaCkjepkHtduVqTyTjmW = 6.548964E-08; 
nluaCkjepkHtduVqTyTjmW = Math.Exp(2);
int? aLdEuDFCdwFwAPzsKYcyUJpzjDj = 4356656;
aLdEuDFCdwFwAPzsKYcyUJpzjDj += 35839;byte GwldlCMkbsuFtStlEdF =  124; 
int GWAMXIXelXkEXPhVO = 3736; 
if(GWAMXIXelXkEXPhVO == 837495){
GWAMXIXelXkEXPhVO = GWAMXIXelXkEXPhVO + 144147;
}byte ibnlsOOZcQoEBFdnXf =  112; 
ushort VFdIpofdHOPlhOGKSTjxFUYT = 22290; 
byte ppYMLTOnxkEczJWxbBqsR =  116; 
int lmXkb = 20312647; 
while(lmXkb == 20312647){
lmXkb += 813406;
}ulong bdttAiAnRMOIyXsaVUSM = 60065735862686891; 
byte TSi =  195; 
string YZqoeSuCjEEJSp = "LYbQCH"; 
uint mszaRSVAKXVcWEEHafWMZJhh = 866624335; 
sbyte gXzQBidWGREKneUMXmkhwwPxLM =  81; 
int qcYKbsMz = 54; 
if(qcYKbsMz == 632054){
qcYKbsMz = 20033;
}sbyte lyVPxkLIazMhcfOsz =  57; 
short GZlMhnRLyIOuHUzoVQOgspKE = 8425; 
int aOwaVMuWmkjHit = 206574914; 
while(aOwaVMuWmkjHit == 206574914){
aOwaVMuWmkjHit = aOwaVMuWmkjHit + 536436;
}sbyte AxJXhyppJeFRRWcITIlX =  -71; 
string iGNseqTobntoYqXEXHWCec = "CciLQLpkwuL"; 
uint wekYPQDEzId = 9179; 
string qoDHgokpWH = "FMdX"; 
float RHiSxXNnAN = -1.032948E-07F; 
sbyte UHaUfSdtbjPnsM =  -70; 
ulong BNQzoUxUsQsIChUR = 51133019921195777; 
uint ZdiFacJmDIoXGMFZOc = 71962783; 
uint Uxh = 279126; 
sbyte JLGZLVtMNPp =  117; 

}public void FmDnEkgjdZ(){
uint dXiMyyOFWUWKXURknanwNly = 88623338; 
float dYhFMglHYjaoqCusKpTpJuVWdMdaS = 2.259268F; 
float yLGPpkDMnTbmMWTtTN = -1.189744E-37F; 
int JckCZdlOyJi = 86857481; 
if(JckCZdlOyJi == 576697){
JckCZdlOyJi = JckCZdlOyJi + 384436;
}sbyte FuCUJqPjHZO =  -82; 
float gbE = 5.296848E+21F; 
byte AAwteadbGosmLYW =  40; 
sbyte jjTGPKAqLMQjaFTSe =  102; 
sbyte EqMiaLwWgbjTwbGbG =  100; 
long UNpqlxoOtSUbcHYSyLFdnD = 9141246645083545; 
byte jtIQbwXF =  108; 
long nhiQPcOEVpHyqCIOoI = 48363057672003882; 
sbyte cNZIHZXBugZWknqTDWpnuHJXg =  89; 
string yphPUXChqZQsFguiItEn = "RoNKUxwebC"; 
ulong pddYkbnMjDEfYKmlAoNoMtGbwfaBf = 20499270408812076; 
byte soDeoMHLjijFNOA =  61; 
double cEeaOHgmeeGwkVp = 2.071374E-08; 
double QqWyDKCgqFoGsq = -1.895155E-17;
cEeaOHgmeeGwkVp = QqWyDKCgqFoGsq / 3;
int[] HDfPY = { 7079690 , 68973 } ;
Random oUuBcJgNPLjx= new Random();
Console.WriteLine(HDfPY[oUuBcJgNPLjx.Next(0,2)]);long DzdHGKAdyoppIl = 62900699290348208; 
string yCRIgfknqkZHLqJFupcN = "RfEwicdCcgCu"; 
ushort dTSWsec = 64261; 
ushort IqbYEnZYq = 18819; 
int mlYcgNnezlohGM = 391404; 
if(mlYcgNnezlohGM == 325965){
mlYcgNnezlohGM = mlYcgNnezlohGM + 852736;
}long OSafXPgDdgE = 13930001169035413; 
short xbUtSLFoIYONWzgcbPAXWih = 6033; 
double WxwLb = -6.402517E-13; 
while(WxwLb == -9.710297E+22){
WxwLb = Math.Pow(double.NegativeInfinity, 2);
try{
int WCktSmUowMP = 9225090;
if(WCktSmUowMP == 27294){
WCktSmUowMP++;
}else{
WCktSmUowMP--;
}
}catch(Exception ex){

}
}long wJOYPcxPGiPBWY = 37581675254667955; 
ushort ljApaBqISPYjAVB = 23563; 
short DLi = -4326; 
long IcweWohanoMGDf = 3991766903219972; 
int qYd = 8449; 
if(qYd == 802634){
qYd += 509886;
}string YflMDIXVjiVQjmyTbPCyf = "uNXlTiGZWeByPZG"; 
ushort AcHwR = 28284; 
sbyte buPnbYGECLzOqOOpjlQoNyynHK =  -119; 
ushort cEqOtSOQMVfVTno = 8326; 
int LnqkFHUfEWCHsWkeAhhZXWa = 467335; 
while(LnqkFHUfEWCHsWkeAhhZXWa == 467335){
LnqkFHUfEWCHsWkeAhhZXWa = LnqkFHUfEWCHsWkeAhhZXWa + 876704;
}
}public void ihaVpghR(){
double UmnSnfEiKPPhYHRPnRNnW = 3.01178E-23; 
while(UmnSnfEiKPPhYHRPnRNnW == 9.830822E-10){
UmnSnfEiKPPhYHRPnRNnW = Math.Ceiling(Math.Tan(1));
try{

}catch(Exception ex){

}
}string TGYemndIRYxN = "LhtETUsJmPztkEzSRxpbmB"; 
double MULJEwuIAURqpg = 1.541399E+30; 
double KFQaCf = Math.IEEERemainder(3, 4);
MULJEwuIAURqpg = KFQaCf;
Console.WriteLine(MULJEwuIAURqpg.ToString());string uSCNxHftoQLcCOxJQMXznqU = "yTqgYRhjGtyCngUfbhuQL"; 
byte eJnk =  27; 
string zcYSpDoYwzJCyiqUuJS = "xfWdZkQlyKGcgbUuExRqRcNWh"; 
byte UOPUCnkhsgUSeNcBLSHuxnWp =  243; 
float butaVQxWyJQdPWTC = -6.29829E+13F; 
int FOCceCPsGSzWKNiOZgYcJR = 85279279; 
if(FOCceCPsGSzWKNiOZgYcJR == 843346){
FOCceCPsGSzWKNiOZgYcJR = FOCceCPsGSzWKNiOZgYcJR + 471086;
}int IHtgWnSqVubdpGjTubNeDhL = 64649; 
if(IHtgWnSqVubdpGjTubNeDhL == 684339){
IHtgWnSqVubdpGjTubNeDhL += 142193;
}short pPTkSEDRVWJkFbFBZJbuQ = 14686; 
uint AVRdhfij = 69992430; 
long yuspRkRhTX = 15991849713731147; 
float qJLWMAJQOAGUppdFpL = 6689405F; 
float qDPpFVWhdJR = -1.270191E+35F; 
ulong wFqDauusmkijELwunKZsi = 40588017747423240; 
string spVVgkcT = "WkCewLtxIoDwXxBnhxPhNYwjaRUI"; 
ulong ilRdwYGqBc = 78494089732346803; 
ushort YPnMjYPseERLHsgpFUb = 36397; 
long VMHEjOJlyaniqtXeTximlXkPD = 64999122984989613; 
sbyte CfHPaJSgRyzbNtBbFuwFYCRaOm =  -9; 
string ixljAwZmIhEuRuseuORJznyCDWan = "bOkQiJamSnXBOWlSkgh"; 
uint tAHuAUYKHDuLIHutFILqSDBUhLq = 6921; 
short TPKInExKOcDlLDPgoRzHAxhe = 20826; 
double lPDNZnOCWXYfPiYXooqPqsF = -1.375682E-08; 
if(lPDNZnOCWXYfPiYXooqPqsF == 4.159899E-16){
lPDNZnOCWXYfPiYXooqPqsF = Math.Ceiling(Math.Atan(-5));
object zeosgQXYdMZLtTpAwL;
zeosgQXYdMZLtTpAwL = 1.535408E-13;
Console.WriteLine(zeosgQXYdMZLtTpAwL.ToString().ToLower());
}uint TegqfuPQJmmKgBDPLDHscbW = 398039895; 
ushort GCOQOqkVMkPpXWUoDiOPenujiTf = 46030; 
float kWMzjnU = -0.007804319F; 
ushort FQFVlKxiNuOGFQqgFHVkBdEaTEzN = 39586; 
ushort KwTAy = 53085; 
short iJDeOxphJShpQEhxBmFcJlKDtnek = 13831; 
short qpKNCYZQEExQUydofmyZkllnXZ = 31594; 
ulong sexqYRagRHyByaAl = 60685989964436073; 
short edwPCZmplzZzVHdioHZzO = -18238; 
ulong CkTy = 18588813053144733; 

}
} public class bezmgPamXTzlQJqnSVTqtm{public void fQPSqQyGUgzn(){
int kHyMPFqwkLLQVhwTF = 7711; 
if(kHyMPFqwkLLQVhwTF == 442898){
kHyMPFqwkLLQVhwTF = 348581;
}double xAnFeqwONInxGFJaLGdgB = 1.43313E-15; 
if(xAnFeqwONInxGFJaLGdgB == -984.1755){
double OnBcsedQ = -0.3665078;
xAnFeqwONInxGFJaLGdgB = OnBcsedQ / 3;
try{
Console.WriteLine(xAnFeqwONInxGFJaLGdgB.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}int kVYFTaWDmFXUAKRzNP = 57; 
if(kVYFTaWDmFXUAKRzNP == 398031){
kVYFTaWDmFXUAKRzNP = 61619;
}uint aKUQVamEiwdEHgBxABPOtEZKPuYZ = 80; 
uint fXZlJxJWIVCwuihYdgUUnLExLja = 36497; 
byte tUizKgoNdSSgycEpGl =  1; 
double WHIutECAwhVusiClLzRPtRbwFg = -1.949424E-28; 
WHIutECAwhVusiClLzRPtRbwFg = Math.Ceiling(Math.Tanh(0.1));
Console.Write(WHIutECAwhVusiClLzRPtRbwFg.ToString());uint jjsJoodtuIHQeIwiPuoHUftWlO = 200378861; 
long QCtaTzuFuq = 30626600260778339; 
long pukTYdQwyCt = 56026090474937491; 
double kaXEYzWuKuLEtYxqAbQJYgTY = -3.380119E-20; 
while(kaXEYzWuKuLEtYxqAbQJYgTY == 2.681998E-05){
kaXEYzWuKuLEtYxqAbQJYgTY = Math.Ceiling(Math.Cosh(5));
int[] MWpWEeoSRTbdjb = { 1543712 , 27112 } ;
Random OWtMpTXwjJ= new Random();
Console.WriteLine(MWpWEeoSRTbdjb[OWtMpTXwjJ.Next(0,2)]);
}ushort MbmnWoIEeictyz = 17534; 
ushort OaPATKwJBPCgnxQSweRPP = 16584; 
ulong jRjJXxzgPYunJXSsR = 118237132158311; 
uint uxG = 518261057; 
byte pcAMLepuq =  73; 
string jzmAXjRmwNZyyyueK = "dTZxHQS"; 
sbyte sjMfDLJqDgXJLKbQDcconKlKhli =  13; 
ulong xHSoAcKHyMOfRXNCsiYGeOPQG = 31676079284905475; 
long pQwkYgWFqBhTnXHTaEhNJS = 41175418158644391; 
long fmxLWYaAhizozPKdqy = 69627264583118224; 
short ymzgTFNbBVWbEJGBtNlcuFsJIe = -22508; 
sbyte YXkGfbVK =  -39; 
ushort cJDwBBYMsCfbBpBTVPzOyERDYM = 26802; 
ushort KQaqsaLbWhhgxQJSVKpG = 10025; 
string UHxzSaxPfCdIMeKhoPRYIpIaOhL = "beRB"; 
long gKWDTbBtcFshXNlDaRpcnEZwKKo = 29683280843446527; 
double NVQynncI = 1.172398E-23; 
NVQynncI = Math.Floor(-9.809783E-39);
try{
int eAsCmVKuqjwoKVLdpIlhSxei = 9383412;
if(eAsCmVKuqjwoKVLdpIlhSxei == 63759){
eAsCmVKuqjwoKVLdpIlhSxei++;
}else{
eAsCmVKuqjwoKVLdpIlhSxei--;
Console.Write(eAsCmVKuqjwoKVLdpIlhSxei.ToString());
}
}catch(Exception ex){

}ushort ynsdOVJu = 11257; 
double fGMnD = -0.6791457; 
double WyEiQJC = 2.99736E+15;
fGMnD = Math.Round(WyEiQJC ,1,MidpointRounding.ToEven);
Console.WriteLine(fGMnD.ToString());string OYPGlRlJwjj = "huZdSxNt"; 
float dfkXGiA = -1247.885F; 
sbyte DKgxOaMxeepdPqKcHFFkLzFVKuo =  -75; 
string dIdoGLTI = "waXNd"; 
ulong xGWg = 67080045298647957; 

}public void uMBtVbIZBMZjZAikNLBgIXJ(){
short qpLidCGIVLKBFScfcVTXXTnOLytF = -26671; 
sbyte dlxELXRyktCdPA =  50; 
sbyte yaFcPsZzbWyGhaDQoGnhoqilzJ =  15; 
ushort AnaoBgeBunUxeKatRctYnBUx = 839; 
int StEgPF = 54610192; 
if(StEgPF == 326856){
StEgPF = 633463;
}float pUsuLjIhSIGkxBZ = -212762.3F; 
short amgQhfSugtWAxzBYGCcouzKCWBFh = -21281; 
int TcKTlidjn = 794994778; 
if(TcKTlidjn == 922200){
TcKTlidjn = TcKTlidjn + 148854;
}byte WDuSCZEDDGUHCKlEfcyMFn =  201; 
uint OxFXVGOqSlHi = 3407; 
string qKEjDN = "EXJTLyg"; 
long SBNDYGHagpJzGwMLtGmFlsQ = 73154432567324488; 
long SpUFBhMcDdmdLYCWhq = 9583569589034510; 
double uncwYVNeRgTyCCJoDXC = -0.007148179; 
if(uncwYVNeRgTyCCJoDXC == 3.029337E+18){
uncwYVNeRgTyCCJoDXC = Math.Ceiling(Math.Asin(0.2));

}long XzEbfxnaFFLyYuQ = 18052439535829977; 
double VbfeTgaLJzXLHi = -1.08644E+37; 
while(VbfeTgaLJzXLHi == -1.500263E+22){
VbfeTgaLJzXLHi = Math.Exp(2);
 bool? LNpWSyPGChGsJuhb = new bool?();
LNpWSyPGChGsJuhb = true;
}byte uUXqlxhLdLHfkEgOVsNHKqhOsCHyn =  242; 
ulong hBPnk = 23437991223940152; 
byte YxIlelDodNiZLQKxgVCX =  179; 
ulong DoBhQCFuQgUJbGLLUGkMCbIhQf = 58830034167311433; 
sbyte EdMOAstSQgJQwkIsAcqBHtsLQXEu =  94; 
double yBNgQTymiJPqJxVk = 2.186231E+21; 
double MbXCPYZTIl = -2171497;
yBNgQTymiJPqJxVk = Math.Ceiling(MbXCPYZTIl);
string ljOHDdOhcEJco = "xICeagjX"; 
ulong dxX = 79632047493588681; 
short PTnpExG = -7993; 
long FwLznpVinyMEfglSmzYTcSe = 17735629653840921; 
long JsgFpSunddTy = 35245573340933049; 
float MqmMcEfDMsekyOfmXGEGZP = -1.089198E-37F; 
uint GLUsYmCXHPsEQRi = 326176; 
uint bqtZAlCRlLXgl = 5876; 
double IutRJAhpUtQNy = -3.876262E-05; 
double gxwplJJftkTOSXi = Math.IEEERemainder(3, 4);
IutRJAhpUtQNy = gxwplJJftkTOSXi;
try{
int KFSMdPlAyF = 400280;
if(KFSMdPlAyF == 82260){
KFSMdPlAyF++;
}else{
KFSMdPlAyF--;
}
}catch(Exception ex){

}short qBZilCHOhfGLlfZENcQSgi = -24814; 
int WzGhKHxYleSnowmtRNDaQoahMnZax = 158224; 
while(WzGhKHxYleSnowmtRNDaQoahMnZax == 158224){
WzGhKHxYleSnowmtRNDaQoahMnZax = 285369;
}string GyA = "MaagpyH"; 
int pxEFiIyyxxHwDIWz = 92992952; 
while(pxEFiIyyxxHwDIWz == 92992952){
pxEFiIyyxxHwDIWz += 223513;
}
}public void dRenZawRsyW(){
short RPcHGFibSOjseJxsBuhefA = 16765; 
string zxaFnXGtjHecA = "NcoldUWVsZzSdPjGq"; 
string mhhixEQFZUbqpaRubRnKjyDebfxU = "jFodemwGUSImoXbpIiSluhPd"; 
short PGdgphWOQpMnkNxPhYGxVBC = 17566; 
ushort FJxbPlMqzPDNDBqBhMEuXePGaQyV = 56526; 
byte oOtEm =  91; 
ulong BDUoHmwxfeutPjZZ = 69150174640984580; 
byte mZjWdhtkdRdQhtFtqRzUof =  195; 
ushort uCOCL = 48860; 
ushort VZxmi = 1210; 
byte Efi =  233; 
long zwGyZZpxhkQTKHFKteJMlt = 79526943001808617; 
int uIdsVmOxOARkypq = 41760821; 
if(uIdsVmOxOARkypq == 75232){
uIdsVmOxOARkypq = uIdsVmOxOARkypq + 248442;
}double emApiNxMhGszcDmP = -4.456295E-14; 
if(emApiNxMhGszcDmP == 1.68323E+19){
emApiNxMhGszcDmP = Math.Ceiling(Math.Tanh(0.1));

}int kxxyQFAUEsSOCWQwBbLAaxtN = 94795439; 
while(kxxyQFAUEsSOCWQwBbLAaxtN == 94795439){
kxxyQFAUEsSOCWQwBbLAaxtN = 518819;
}long ooXGyfq = 7065671498617042; 
long PenbgOO = 35595620309281898; 
long zlYkXuKVRhyfFEgEoLxxh = 39681922315162032; 
int WFAHEKYSKFfoGpW = 20; 
if(WFAHEKYSKFfoGpW == 732052){
WFAHEKYSKFfoGpW = WFAHEKYSKFfoGpW + 684923;
}int CoLPIjckz = 69; 
if(CoLPIjckz == 94335){
CoLPIjckz = 935406;
}int pTNRbTWTGJgNJVPpsYsTUqIIuIf = 799488803; 
while(pTNRbTWTGJgNJVPpsYsTUqIIuIf == 799488803){
pTNRbTWTGJgNJVPpsYsTUqIIuIf += 162949;
}float RTwAwPVbBVyPoV = -7.629772E-27F; 
sbyte KZZkeytOadjITFPAXxX =  -8; 
double ZogMW = 8.810203E+23; 
ZogMW = Math.Ceiling(Math.Asin(0.2));
try{
int jgVdqIZsnphynKdTabRclaTT = 4501326;
if(jgVdqIZsnphynKdTabRclaTT == 64740){
jgVdqIZsnphynKdTabRclaTT++;
}else{
jgVdqIZsnphynKdTabRclaTT--;
Console.Write(jgVdqIZsnphynKdTabRclaTT.ToString());
}
}catch(Exception ex){

}double ITDjnbqPlDNFDPDjSqOnwKRWGEZAN = 2.512192E+36; 
ITDjnbqPlDNFDPDjSqOnwKRWGEZAN = Math.Ceiling(Math.Sin(2));
try{
int AEOTO = 8043188;
if(AEOTO == 16158){
AEOTO++;
}else{
AEOTO--;
}
}catch(Exception ex){

}ushort RUQhVTdblMZVdOaEPuePWYai = 61185; 
long jRYhUmOTh = 41452496770362297; 
uint geKoPwdFTSUHlnHUkmEeIoV = 519064823; 
double tefxIHiBiBCjNQJVbmwIqeQG = -4.920147E+20; 
while(tefxIHiBiBCjNQJVbmwIqeQG == -2.383087E+26){
double GLBNxlacNzuJdRoYtTGfnzonAg = -7.59995E+23;
tefxIHiBiBCjNQJVbmwIqeQG = GLBNxlacNzuJdRoYtTGfnzonAg / 3;

}float KiQloFLJUISDVWaWjJAeLZK = 8.80172E+30F; 
short EGAhMcymZpmaGXXnOtLu = -4959; 
ulong UQkQc = 39387484674759867; 
ushort gByJbGXIUhOLWDjV = 15626; 
ulong GqClwDBxVaJQUTmmj = 57898906522109561; 
short DqgeKtBPiFncfBibHksURJpZYAZdJ = -22057; 

}public void EfEYfPWZ(){
ushort eeIWTHnpUCgp = 26396; 
sbyte bggkuyfauKXDDcmLkVBNFIdfSL =  65; 
ulong dmLxdeJPOcVYdmlWeMmI = 89771757063649597; 
string JKquqxlgzYEV = "TFOBuRiOZBgQiS"; 
float OLG = 1.061603E-06F; 
ushort IKWQKYnn = 5181; 
float DoHPZlSFKjkEhFw = -9.111696E+14F; 
byte gaAugRybzPLmcVLpCuZLuWRKbyfP =  55; 
short EagOycVBXDjtPYXhIynTLLDDkj = 20126; 
short MIQjWhlWhmmGUcikEbLeo = -2932; 
short xiTzEEnmERlUtiTRLlBjamQJtqHh = -26181; 
byte npTONoOsFYacd =  208; 
short uhTudXiWiskemWghpxXEAwgDX = 25140; 
double akNSdAlZ = -1.101973E+28; 
if(akNSdAlZ == -5.747706E-28){
akNSdAlZ = Math.Ceiling(Math.Cos(2));
 bool? ZbqxibXVYGtbN = new bool?();
ZbqxibXVYGtbN = true;
}sbyte jbHPLHtWLWo =  10; 
byte PioThuNFOA =  125; 
ushort FzjbaWqlCjXNIwsIkjENd = 23844; 
string bnNDGmhoqYOyCVnGUdnZ = "fBYuXEpVHGlw"; 
uint NjO = 26637455; 
string xJIUfpVYSZeeqVNOiJKR = "KsDcaUCwfyaXK"; 
long qBRnuXOGgpjeZwGXEknMqeRfw = 38280200251898738; 
double ISxxxWMxNRQCaBA = -4.270154E+16; 
while(ISxxxWMxNRQCaBA == 7.427773){
double aucdbiETdXYfhPSPFJenGSClcJ = Math.IEEERemainder(3, 4);
ISxxxWMxNRQCaBA = aucdbiETdXYfhPSPFJenGSClcJ;
try{
Console.WriteLine(ISxxxWMxNRQCaBA.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}int NZYEsQVWoBaJuAKJuzIf = 652709; 
while(NZYEsQVWoBaJuAKJuzIf == 652709){
NZYEsQVWoBaJuAKJuzIf = NZYEsQVWoBaJuAKJuzIf + 288461;
}byte IaSVs =  211; 
short wKFOmZnexFOtfsYmL = -368; 
sbyte GomcH =  86; 
short IepDigmKUT = 23027; 
uint onMCQXpbFkSKk = 126454179; 
ushort hwLZxqnkjYfotRIHL = 19918; 
string CQUYldUzTYjfmsMy = "uTWzpGZjNkukcBIeWaqCMNty"; 
ulong lPZcbekRpxec = 82726818314183275; 
float DbpqwVdIBAJYujk = 4.362084E-26F; 
short kjk = -32566; 
float gHXnAXDBOmYMKJoOJWGtmQLE = 3.07314E+14F; 
sbyte GnqaxDnof =  -32; 

}public void Cgy(){
int lZzHMyCowBCpWtAQPoVAVXohuaIo = 64402124; 
while(lZzHMyCowBCpWtAQPoVAVXohuaIo == 64402124){
lZzHMyCowBCpWtAQPoVAVXohuaIo = lZzHMyCowBCpWtAQPoVAVXohuaIo + 751625;
}short kCChNcGWaXsMUjNRDgboKqmL = 19375; 
byte qmNgibLFmONN =  219; 
short gyIWwMpxmdLc = -26980; 
float QtBKnKnRiK = -1.762806E+08F; 
uint PuiX = 1960; 
ulong TRHb = 66982098159364099; 
double licJTGzllfQsy = 364.7731; 
if(licJTGzllfQsy == 2.722876E+14){
licJTGzllfQsy = Math.Ceiling(Math.Asin(0.2));
Console.ReadLine();
}byte CCkc =  92; 
short RdCbwL = -8177; 
ulong JUdUBEUeUlqShVpXkaSuIRIid = 60351500589822156; 
long qjs = 68359283255798858; 
long kejKCwj = 48842788816290614; 
uint TUAVZNTzUMCIBjOtqtJjMXDzi = 28; 
uint TtQIq = 41; 
byte lsqkHJJjM =  184; 
string NVIp = "VqbMV"; 
uint tbtLZjesidhq = 507207110; 
short QYsDzcgyisljejCqTZSGpXjDHK = 25442; 
long pxFFxkODVTpUVDIXUnZDRzU = 45983540888305813; 
sbyte KSVIGIYNOXgCLuu =  9; 
int xVgWSDWTStZNZehykLMpoG = 79; 
while(xVgWSDWTStZNZehykLMpoG == 79){
xVgWSDWTStZNZehykLMpoG = 121497;
}string zmkjtnqO = "tLN"; 
ulong ObXcumiKQnyQTqTqPQS = 42494569301352652; 
ushort tGDQxbbEMMObOyXMDuZUJ = 37848; 
float hLbBgqAfMZxJVayoSOcddHgpFdi = 6.197347E-30F; 
int QKI = 72493255; 
while(QKI == 72493255){
QKI = 405905;
}byte oPanRDWKbVCRRBEMWdhZ =  249; 
sbyte HYnqMossIRxsqopmHEyCfoTRhhnE =  -36; 
byte JgRuoEkkuK =  142; 
float uim = -1.347334E+08F; 
ulong wCISsfyjFaOZuDNkB = 63599053899747734; 
ushort xZUSYDMSgzZcjgbHuWEzyHXNDF = 8450; 
sbyte AQoW =  -64; 
string KiQxJKGM = "RRDuMJVZWuPbhotxG"; 

}
} public class IJLgWfRXkfhQOaklLOEMpQUp{public void SliCUxadUYdQLVyEPzPRgHOQWEGSU(){
uint OwgdUIwqJgMUchwaSgiBsfzSl = 352372; 
int RhjtgowERDV = 1354; 
while(RhjtgowERDV == 1354){
RhjtgowERDV = RhjtgowERDV + 751984;
}long BENLLZzJWnSxpQbSYDBW = 46503441325744057; 
double zzMKeMeyzuIHApZgFEfnLRm = 1.793149E+23; 
while(zzMKeMeyzuIHApZgFEfnLRm == -4.195224E+17){
zzMKeMeyzuIHApZgFEfnLRm = Math.Exp(2);
Console.WriteLine(zzMKeMeyzuIHApZgFEfnLRm.ToString());
}short ZgihOM = -22753; 
long ldGTpjHxbKLnj = 25659760990340140; 
int lmXjqyJmL = 11; 
if(lmXjqyJmL == 510481){
lmXjqyJmL = lmXjqyJmL + 659225;
}byte umIfETPmobsxGeNGStGsaoZtx =  3; 
sbyte VkGNc =  57; 
byte ZfJuhWbwpRects =  207; 
sbyte VQen =  -60; 
long MLqexFtTUqHMLlkdnE = 51194003086911663; 
long VfgLyf = 21721897161765204; 
sbyte Ldx =  79; 
long QLkdIJZOKYUmYPjVoGFNzqF = 87767882401900549; 
float ioxBuyYyRmaQkm = -4.640439E+24F; 
ulong RlXnAfSVXNoVtXaRPAOwVhGo = 1953206831999361; 
string jJzVQqIABliVUcQtz = "dwTKHJHuCpXBkxTROqtUJxNxHtg"; 
long CMEYHPIJ = 45395670486060137; 
string Mcc = "AaFlHIxdmC"; 
float ZFtCEIkh = -9.493193E-17F; 
int mHwVGtwJhmlDtwhZlwP = 3735; 
while(mHwVGtwJhmlDtwhZlwP == 3735){
mHwVGtwJhmlDtwhZlwP = mHwVGtwJhmlDtwhZlwP + 152627;
}uint eRUZjyZYamCyUf = 434; 
ulong QiiAlWTAolbQ = 41940645802425588; 
string uyhNK = "VhhieuqpnGGYgKiNWjGPpXQuPzpLO"; 
long AlUFgeTMmwSI = 20281386002299877; 
double UJXsNu = 2.355559E-32; 
while(UJXsNu == 2100.841){
double fbJdzstzpe = Math.IEEERemainder(3, 4);
UJXsNu = fbJdzstzpe;
try{

}catch(Exception ex){

}
}float LTiNUDQbZBNDQ = -8.092341E+34F; 
double DqkWOIISgGXcxHDaUyYlGa = -34336.83; 
DqkWOIISgGXcxHDaUyYlGa = Math.Ceiling(Math.Sinh(-5));
 bool? TZDTjDWEZKgDeHYnUdQFxqQXDRpw = new bool?();
TZDTjDWEZKgDeHYnUdQFxqQXDRpw = true;uint qydhBDiGqtlBdlZwpPXZ = 265; 
long YQJnFbkmVSx = 63508401446299683; 
string fDwDXkRfCp = "ONiKTVaoiZ"; 
int QMOkaQja = 679778; 
while(QMOkaQja == 679778){
QMOkaQja = QMOkaQja + 269609;
}long RtgtCgAUWdnUpnSnBqGQ = 28027076797212818; 
long xGktzPSlKJm = 27889097254955126; 

}public void sXJUyRERblpwjGIIKXQdxnIW(){
int LTNdGGmVXA = 324148414; 
if(LTNdGGmVXA == 224571){
LTNdGGmVXA = 17033;
}byte sSKJoUFNwOsdoMp =  41; 
sbyte dYGLhoIiLtbiRwdjELFbF =  92; 
uint jADUjkaNBDXlSILwbfilMmWXaf = 60; 
int ekBNB = 73010464; 
if(ekBNB == 507093){
ekBNB = ekBNB + 358541;
}short ozRqWajaAKlgXDGTdduj = -4773; 
ushort MfgcKmgJTyFHziPlGudGBn = 40482; 
int gUVyYe = 8433; 
if(gUVyYe == 228057){
gUVyYe += 792002;
}double OtbsJtVgLoRPQsSk = -0.01158602; 
while(OtbsJtVgLoRPQsSk == 56304.87){
double tsPMcDxHUpmAWWjjajHGioTZCIN = -7.858568E-24;
OtbsJtVgLoRPQsSk = Math.Round(tsPMcDxHUpmAWWjjajHGioTZCIN);
try{
int hQxpaguIMwhVbGM = 881771;
if(hQxpaguIMwhVbGM == 66638){
hQxpaguIMwhVbGM++;
}else{
hQxpaguIMwhVbGM--;
Console.Write(hQxpaguIMwhVbGM.ToString());
}
}catch(Exception ex){

}
}uint jdGCVkatRuPgjRotmFk = 22565704; 
uint imVjDYgBdcVj = 93791461; 
long wMMIwxnGtpEJByPwIMynZ = 11724892075064994; 
long yGVQYBUWNZpQmuzaGmB = 85321095845199069; 
double KjqMKgcFIRUOQKoJj = -5.374262E-38; 
if(KjqMKgcFIRUOQKoJj == 4.574705E-30){
KjqMKgcFIRUOQKoJj = Math.Pow(double.NegativeInfinity, 2);

}uint uADdFeKoVXNhFo = 40; 
int lwZxnzbKgcBREJieqGeKlszemuP = 543153858; 
if(lwZxnzbKgcBREJieqGeKlszemuP == 226161){
lwZxnzbKgcBREJieqGeKlszemuP += 334878;
}short cxYlebUXIPdueZBY = 216; 
short IUXUjTKchjYnAkhfJsuaIsuRpi = -31941; 
long bYE = 85130749018757921; 
ushort MOJBFiWTSGfokpAgFaSZbfkutaBDa = 8574; 
int GIG = 15317310; 
if(GIG == 836778){
GIG = 773469;
}sbyte szGnugYX =  67; 
uint XGDnhEJuSLceK = 706835; 
float PxakUWTWNNgwciQCkKZdUIbiuoApu = 1.567892E+16F; 
long ZiqAjdKKq = 15875042750282959; 
uint LQegwSRGyDDQcyicazbbHotDmk = 41641205; 
uint dRzSXxUcVIsymgPFOdQNCNBxlxyD = 198121913; 
byte gAALsjANhYDuJj =  9; 
byte JyDGMxppbHCZsiMyXqOHdlE =  148; 
string DkgObujQjNIPudiupD = "NQPRXMTnGJxTYDhTfjnFhPCZYQhWK"; 
uint WaGoOOticifFgNTJlmjdk = 95; 
float nQThEWEhZBStntfNalwbkwlfgVJAw = -2.728274E+28F; 
string qAYBbzlDygmapYUepjQDPOTE = "aIndMBHPEWTgmZNjANnZeRW"; 
ulong eodBaDHuMbQYz = 10741280544380251; 
string dtpaFEJi = "AJIlDoDtmNBYWkacJtTeVO"; 

}public void beZhKDbQcSeAPd(){
ushort WiIN = 14288; 
int ghoXWkQeQ = 23800073; 
if(ghoXWkQeQ == 553539){
ghoXWkQeQ += 128930;
}float kWygRMlZnoWVmbkzTTawFfGmN = -2.076058E+21F; 
int nBQqRDoffoxcYIV = 170651039; 
if(nBQqRDoffoxcYIV == 18319){
nBQqRDoffoxcYIV = nBQqRDoffoxcYIV + 143984;
}byte gylfqktDXJCZPgFwAWtOE =  147; 
short yKtFLfCpbRJTFxYUNMA = -7589; 
ushort gMEhjeOAgDeGIHROSRgl = 63765; 
double LRcYzaReYjPEHaD = 9.908283E-05; 
while(LRcYzaReYjPEHaD == 2.985936E+14){
double qdYUImkKioSqhgpzoygxScjO = -3.440939E+21;
qdYUImkKioSqhgpzoygxScjO = Math.Sqrt(3);
LRcYzaReYjPEHaD = qdYUImkKioSqhgpzoygxScjO;
 bool? xmpWSmHucXyMZfwshlNqld = new bool?();
xmpWSmHucXyMZfwshlNqld = true;
}uint xsauyNIRGditztSHnppsBSsMizT = 388954; 
byte SRpHcKnekHcFBCjbTdqIiFh =  101; 
int nlY = 162158555; 
while(nlY == 162158555){
nlY = 321044;
}uint NPXakfL = 243127; 
float ZyZxcFEn = 2.80753E+31F; 
string xbgq = "GljkISgnuHXAyWs"; 
sbyte lBUM =  -29; 
ulong cTmKNCYePlTbHZqFTiJYHPl = 84892688190241489; 
byte yiPGxBTISLkdqC =  20; 
float bEMQgnInZUiiAsAIEjRDZxIQqOd = -3.191853E+32F; 
ushort nVKwhAHpUhs = 29877; 
int ztsLwNsmqWFWVAsjHqFDU = 7891; 
if(ztsLwNsmqWFWVAsjHqFDU == 171115){
ztsLwNsmqWFWVAsjHqFDU = 947815;
}float wIYafSuMoCIZbEOQlEhYKBaTdQkF = -7.108727E-11F; 
byte zqWoHVQIYzsQXETKbffGQVlV =  163; 
sbyte pHQoVoSCKIAjpNAZlijWBWBEpnHZ =  12; 
string HBY = "aiixCecicDVPSpVa"; 
ulong jxtflZIITYGHSKs = 72464215517072868; 
ulong buzPJHdojqChUWi = 4350558465570482; 
ushort ReXZPsXFIkGjTjqCsIs = 53351; 
sbyte UUZPGwkcqhbjSDgouY =  20; 
int LEFZbMaeb = 625244990; 
while(LEFZbMaeb == 625244990){
LEFZbMaeb += 67498;
}string NDu = "UyhKsPwcntlDsjpuPEJ"; 
byte onwKGsdoulbhHRgxpKWGbOBuWUaO =  155; 
byte UjIFiNZ =  185; 
ushort OUhkwLsiYATM = 37952; 
double OaNMsoJicgXZlYKqY = -2.519154E-20; 
if(OaNMsoJicgXZlYKqY == 2.76896E-12){
OaNMsoJicgXZlYKqY = Math.Floor(-321.722);
Console.Write(OaNMsoJicgXZlYKqY.ToString());
}ushort VaywscnqefxYIqdXSezziLgeipKTt = 3936; 

}public void lMF(){
string BYTgU = "bZbENaAydkEXQjTtmbsuXbdkmco"; 
double TlqFmJhdFEJuWZ = 3.494617E+11; 
double PAYaVQqKMBajZltDBMoIzJaab = Math.IEEERemainder(3, 4);
TlqFmJhdFEJuWZ = PAYaVQqKMBajZltDBMoIzJaab;
Console.ReadLine();short zXYIm = 22845; 
ushort NNcFKnnRzb = 25024; 
int hemxhdbenwecyGqLGaxxIg = 73; 
while(hemxhdbenwecyGqLGaxxIg == 73){
hemxhdbenwecyGqLGaxxIg = hemxhdbenwecyGqLGaxxIg + 963535;
}uint RKN = 3301; 
float keOwkzsXClIXgTaXidaVBi = -3.575158E+33F; 
string mEwpdozZuWIbyJRPLB = "athRMEppXjcKRsG"; 
byte DlhtzgAephHseMzVzckfMKMO =  248; 
uint smRKRyclzsfDmINxxJYSzEOs = 4083; 
sbyte TBtu =  115; 
short SAzPUcA = 32399; 
int BXyZnpeaJqOz = 377693; 
while(BXyZnpeaJqOz == 377693){
BXyZnpeaJqOz = 367818;
}double LFWBUWYUdQYEkZQyIcGjouoZutaet = 7.526459E-36; 
if(LFWBUWYUdQYEkZQyIcGjouoZutaet == -3994326){
LFWBUWYUdQYEkZQyIcGjouoZutaet = Math.Pow(double.MinValue, double.MaxValue);
try{

}catch(Exception ex){

}
}ushort gLIApManpSwU = 25641; 
ushort sgD = 32361; 
ushort RQhUqoxNRVhafDLybsUgwniEF = 34267; 
byte BhuVZCpVAFYyFBzXmToSqVfkGn =  105; 
int FpQdEmJBmbElQXxtyDU = 22065533; 
while(FpQdEmJBmbElQXxtyDU == 22065533){
FpQdEmJBmbElQXxtyDU = FpQdEmJBmbElQXxtyDU + 371632;
}uint cipMgNnUohtanQxaSMG = 943274; 
long hLBOwnFFKcIGqWoyACdhldCHY = 28963967452420763; 
short bGYTHLE = 11496; 
double kfdEogZUBFdiUQHXMdW = -9.908276E+36; 
if(kfdEogZUBFdiUQHXMdW == 5.71524E+20){
kfdEogZUBFdiUQHXMdW = Math.Ceiling(Math.Sin(2));
Console.ReadLine();
}double aJqQRulSfpXCHxVxJUGTqoV = 3092904; 
aJqQRulSfpXCHxVxJUGTqoV = Math.Ceiling(Math.Acos(0));
Console.WriteLine(aJqQRulSfpXCHxVxJUGTqoV.ToString());string UIjPnEkKyqIEFloqVWXwjGSa = "htWja"; 
ushort JXOFUFZOFlNOU = 38073; 
double ThwOfIGnPjLf = -3.34889E-17; 
while(ThwOfIGnPjLf == -7.121478E+29){
ThwOfIGnPjLf = Math.Ceiling(Math.Tan(1));

}byte QUgVpCRskLedGJY =  100; 
sbyte AHJSbWlYG =  71; 
short uTBGwacphow = 8141; 
int STgQdkwZcEPXTjJsWc = 87; 
while(STgQdkwZcEPXTjJsWc == 87){
STgQdkwZcEPXTjJsWc += 5787;
}byte sOfkkckyetRQLgVwQXQyhSIPstGN =  66; 
int gCfSHURtNNx = 31; 
if(gCfSHURtNNx == 332974){
gCfSHURtNNx += 874763;
}double xGjQQAgpTRxzjycKnFooCZFcLMN = -1.161915E-18; 
if(xGjQQAgpTRxzjycKnFooCZFcLMN == -2.104733E+18){
double WMptAEYCqnlImRTMFKuxag = 2.032739E-12;
xGjQQAgpTRxzjycKnFooCZFcLMN = Math.Round(WMptAEYCqnlImRTMFKuxag);
 bool? cIPKKRmqODoVPXacMxAfdysph = new bool?();
cIPKKRmqODoVPXacMxAfdysph = true;
}short HFwuWdqyJUxwXuKFHMGoAD = 5998; 

}public void AHYWdoexKqfReLhubTzjCsNwVI(){
double WbmeaCUpeuuuQZMHoTAWFlnHeushU = 1.972613E-37; 
while(WbmeaCUpeuuuQZMHoTAWFlnHeushU == -1.993828E-29){
WbmeaCUpeuuuQZMHoTAWFlnHeushU = Math.Pow(double.NegativeInfinity, 2);

}byte QWybkucwNnBZLbFESZlXHOe =  124; 
double slIdtLSWJfflRCcqjtH = -4.332142E-16; 
slIdtLSWJfflRCcqjtH = Math.Pow(double.MinValue, double.MaxValue);
try{

}catch(Exception ex){

}uint fcqzCp = 5100; 
sbyte ZMjqwYlIMelcfPhRLAE =  -36; 
sbyte ihbiTbLnoYJnBKnwESeNfuTcWCpL =  23; 
long OybEbolejXyPntSnzDoVLnwSPnZm = 84163709243224007; 
uint lgqOVP = 59633780; 
int dnShoQmBbjPNpZNzXPklz = 78; 
while(dnShoQmBbjPNpZNzXPklz == 78){
dnShoQmBbjPNpZNzXPklz += 574406;
}double GjjJpidTStOO = -7.745136E+21; 
if(GjjJpidTStOO == -0.009602563){
GjjJpidTStOO = Math.Pow(double.MinValue, double.MaxValue);
int[] JFewxzdlAZRslVXOpPoffGtbm = { 47 , 76917 } ;
Random zmtoNuQCtRNzXVgRYB= new Random();
Console.WriteLine(JFewxzdlAZRslVXOpPoffGtbm[zmtoNuQCtRNzXVgRYB.Next(0,2)]);
}string jIKPQULdldiUZQ = "klyDLDfaSStZoHSbzDccLFXEWR"; 
short KodWpYOnDloTeISXyUx = -3825; 
ushort KqXN = 9931; 
int UGUmBqDOyK = 8516; 
if(UGUmBqDOyK == 245619){
UGUmBqDOyK = 249080;
}byte cMZgbuSlHFMZDRhZJHwGEqpjakfFP =  146; 
ushort NPeTo = 49766; 
int uoKXbygAKFQHREw = 43658876; 
if(uoKXbygAKFQHREw == 825015){
uoKXbygAKFQHREw += 284540;
}byte dhIlOMnKVcWloH =  89; 
sbyte dHUkORmiutyAINjgT =  31; 
ulong ltNwUUEUsfeeI = 45123987731270497; 
string tRknpMexaDNqK = "zqTmtRnnsRdZDNIqzaiZuFKHnh"; 
byte dCVmcOUUwdblfeAtjpjqO =  173; 
string hKAUHmYkEEjQWRWOTOVwZWU = "eqyYYWWuzh"; 
string oZmifRJDVJxs = "xzaOJNlQkPpP"; 
string HHPsONEAT = "NoimqtxUfFCzWHQw"; 
float dkXufFcVfqIFPXGAa = -1112153F; 
double RtwKwLiLhCtUbwJ = 3.814259E-12; 
if(RtwKwLiLhCtUbwJ == 1.721914E-23){
RtwKwLiLhCtUbwJ = Math.Pow(double.NegativeInfinity, 2);
 bool? UTjXPoLOCWWIiaNcEniiFf = new bool?();
UTjXPoLOCWWIiaNcEniiFf = true;
}short YQccWAXnQga = 11873; 
int BeOEUwKMLPuf = 990576465; 
if(BeOEUwKMLPuf == 424235){
BeOEUwKMLPuf += 821470;
}byte ktDcuQFRMSXznKhuVOsUGlemJQFYF =  130; 
long wFggDMje = 88395828372223829; 
float uJAtsZoptE = -4.939196E+09F; 
byte hOcKEaSj =  148; 
byte Nllw =  195; 
double gpgcLblhSlsGT = -1.054919E+10; 
gpgcLblhSlsGT = Math.Ceiling(Math.Tan(1));
Console.ReadLine();
}
} 
public class BRLCtfIcuaUCaI{public void sAUpHdqFSJUnAdKANpXU(){
short wcgGWlTCdMkit = -8123; 
ushort Jtjm = 55317; 
float RhZLChkdOtYdRibg = -5.715734E+16F; 
short idpLpcLHEKbCEDpyXBs = -7363; 
float xPupGTImeCBmqORnuAxF = 1.138E-35F; 
short AoTFiFtXMBHCpax = 21468; 
double GAaqlDGJgTX = -7.402036E-28; 
while(GAaqlDGJgTX == -1.41887E+33){
GAaqlDGJgTX = Math.Pow(double.MinValue, double.MaxValue);
int[] cGGjyTxhcqZByB = { 671125 , 56100 } ;
Random ztnftjfRWmfqxRwwpLMQqJYVEBJnJ= new Random();
Console.WriteLine(cGGjyTxhcqZByB[ztnftjfRWmfqxRwwpLMQqJYVEBJnJ.Next(0,2)]);
}string FbySkFkXPJWsCZqIVKPM = "IQutCTCRhTKdKSPXEefCT"; 
int bDbBqHOuOqXWlcAJMKfIQloSgCKqF = 19277916; 
if(bDbBqHOuOqXWlcAJMKfIQloSgCKqF == 801046){
bDbBqHOuOqXWlcAJMKfIQloSgCKqF += 363540;
}ulong sjktAWNMWDUexuSO = 35095690341591657; 
ushort egfQxKJiuAUnppDfjYQmiosd = 13286; 
int CpYuoXKRGHggjwmZsT = 88032398; 
while(CpYuoXKRGHggjwmZsT == 88032398){
CpYuoXKRGHggjwmZsT = CpYuoXKRGHggjwmZsT + 181935;
}ulong iHPOcG = 10763851577463502; 
int WAdSGSVoajDMdQk = 58332946; 
while(WAdSGSVoajDMdQk == 58332946){
WAdSGSVoajDMdQk = WAdSGSVoajDMdQk + 45136;
}float BodmtEmnqmbPGfXBpL = 7.796123E+17F; 
float fdqJ = 8.804031F; 
ushort DxfVEEDGDLOWlzcaG = 44334; 
int zsDdMzjaWLXMRPKGKLDuY = 15740266; 
if(zsDdMzjaWLXMRPKGKLDuY == 911354){
zsDdMzjaWLXMRPKGKLDuY += 825377;
}long pcY = 32444004682129800; 
ushort JSObexOyqVglLNkOc = 53912; 
int ZGIYRdMBMICybxbpa = 811399007; 
while(ZGIYRdMBMICybxbpa == 811399007){
ZGIYRdMBMICybxbpa = ZGIYRdMBMICybxbpa + 983970;
}sbyte JcqZfZszRDdjPjbLnLKtKkoLU =  63; 
string mJw = "BhibuBGIjYjTYgkOPJG"; 
float OhyOGftVqlh = -1.118985E-05F; 
long bapNfbiLawNNCplEdDqN = 10969286689909868; 
ulong NCzE = 78781755216752975; 
ushort TtRGfIeEfWIo = 49423; 
uint TnpXYJknYDyj = 8058; 
string EoUWbw = "NfXixD"; 
double bSAUZnAyLJeScJy = 8.678738E+20; 
if(bSAUZnAyLJeScJy == 8.592884E-34){
bSAUZnAyLJeScJy = Math.Ceiling(Math.Atan(-5));
try{
int RjnXEde = 1401048;
if(RjnXEde == 84674){
RjnXEde++;
}else{
RjnXEde--;
}
}catch(Exception ex){

}
}string ZiKqtUdzkWfuYjIneDAjhyuYtGO = "SCXDtS"; 
float zWQCPlxAfA = -5.893034E+07F; 
short aFsQNCPLXOgXHJZDbfptPiNYwVbUT = 24260; 
byte PTCtkEe =  88; 
uint YbwSGNBDzooXjSRtwRBUReD = 156996685; 

}public void hHszaYUbjLEEpnupuWTSJJC(){
float mSGWUAx = 5.766713E+25F; 
int janpciJfQSClACKHfpeAeCpNgo = 29; 
while(janpciJfQSClACKHfpeAeCpNgo == 29){
janpciJfQSClACKHfpeAeCpNgo += 471218;
}long guIDPAaxapMHfwkT = 56529104858146392; 
byte dtTJYcDZwoYEBFzICZaBjKe =  95; 
float MDLyKgsbxHEssJdg = -2.015371E+31F; 
float ThIhgKBKFtoARzqZnXGSkzi = 3.44465E-32F; 
byte qxcKiAeHEVEpZGJycqdB =  221; 
short Tdpc = 2423; 
ushort PcXcinCKfaNSOeKHGlDagBaJPZsjH = 57345; 
ushort lFfCtXyuWslcObFZi = 28522; 
long ZtSobomiVqcmEjGiYinJPheO = 99457249343468; 
byte ESMRAdKnAFpZAFwwaaib =  5; 
float eZdtAOCqTc = 3.664668E+09F; 
ulong gKjYBJBzpzqsZt = 67517549076842837; 
ushort kAYmxWEXIYTqZcBiPHJGc = 8714; 
ulong qRUpUjQ = 42724337001775104; 
ushort tLbAJbefDPaiPIdMPWGbmcyiFcB = 19079; 
uint CFoMKwMJJQPkswP = 48; 
float oVqxtHpmTVGCQHdmxUdXI = 2.47682E-29F; 
byte lcLctzEJFjEkiWeuiTOcnZFn =  39; 
ushort xUcoPdHg = 38337; 
int AycTkxBb = 9689; 
if(AycTkxBb == 487060){
AycTkxBb = 121266;
}short BlzyUamg = 18012; 
uint PhCPBPXtqawOiOffmypEixTfUgh = 4877; 
byte eJCLUMizPCNbkfzpJPUZKyYMtox =  229; 
ushort IUgeqeHcludZCbxuJaxQ = 31533; 
uint jlHKQsGpnehUTVfOtJ = 54917182; 
uint LWXoZGwzEhgoWSKuRLHePVuhEzD = 641; 
sbyte YenCLNBsZUQBaNEYmeYRVWxgPD =  74; 
long diwwohkJqZIqsIjZ = 49262507121460553; 
short cIhMiYPSVLufLGspWeqBDolgwiwM = -31921; 
ulong peq = 30442616355655300; 
long FoJOIjscOQuOxapa = 12270613059933828; 
uint OqoHYcDMCgAHdQxYCoWpfBoQ = 46; 
long PBoomSxbOhRskLzjBW = 56600843554263532; 

}public void nczBBtYXbSktkuujuzFfw(){
string ojzJU = "XERc"; 
byte YpHpYmN =  112; 
short DPHpDimSen = -18327; 
double qcLeUeo = -2.813175E-14; 
if(qcLeUeo == -1.036301E-06){
qcLeUeo = Math.Pow(5, 2);
try{
int cVlOuUJwfmlGG = 3785595;
if(cVlOuUJwfmlGG == 10537){
cVlOuUJwfmlGG++;
}else{
cVlOuUJwfmlGG--;
Console.Write(cVlOuUJwfmlGG.ToString());
}
}catch(Exception ex){

}
}short NzUsKPmMewVHK = 23147; 
short NLpqSXGT = 5947; 
long bdCSJfAVXkUDRtye = 82119486839920470; 
long YDQTaNcnlIDDmCkXwZSiAXuS = 47356943732168558; 
byte hWeekhBDhQDNlBHEDOPBoaiYDG =  52; 
byte ipJtttnTpP =  179; 
long QEpEibkxBDeJEgOgCFWonYATuShs = 12237136659336916; 
long yiaSIOFKPlAMPkOtJRdyjpUSMnFZR = 72746048933748983; 
ushort MnKdVTYyOqz = 44289; 
short kQgohguiGSEopnlca = -26711; 
byte qiAmYZNQRJtDEJmN =  4; 
uint oWEJSA = 55417396; 
long LptxSjbpU = 57142295852518826; 
long aAbCXXFOBINtxcEfsagM = 86734386343299505; 
byte tzUVKMKOwznwCJgXLpamGz =  228; 
double PcVuZam = 2.176009E+21; 
if(PcVuZam == -0.02608421){
double EFIzdKOakmHqYfU = 8.208563E-13;
PcVuZam = EFIzdKOakmHqYfU * 2;
 bool? KOuxyaSNuRyjXiQbUlCeeoUiMuW = new bool?();
KOuxyaSNuRyjXiQbUlCeeoUiMuW = true;
}string qtIQuTlLddndZfTwBaiSMbdUhVMwI = "PxFuukgHOOXyhuXUucUIig"; 
double nppOJzhR = -3.271479E-12; 
if(nppOJzhR == 0.00521709){
nppOJzhR = Math.Ceiling(Math.Tanh(0.1));
object ytgeUtXoHfuPefwQqkuoU;
ytgeUtXoHfuPefwQqkuoU = 8.545991E+36;
}ulong dFRSLspGnNTdiInDDsiRsZxMpsK = 79617487164818158; 
string ElfZxCUBboZBsuFHYDehfCtJUgVQd = "eplDYfcRVyDkodLVgX"; 
short CCnTUbT = -4804; 
sbyte GexzjJoQwzE =  -94; 
long GUmpYYaYyUbEcsJXmHN = 40840284290153566; 
short TGuUHnaPDyfoIHCyH = 8509; 
double zlWubP = -1.414828E-31; 
while(zlWubP == -2.3282E+33){
zlWubP = Math.Ceiling(Math.Cosh(5));
int? TSgVs = 9678324;
TSgVs += 22513;
}float pCNNLFpSelDJBtWF = 2.775296E+23F; 
int NRFJTMYPZCmiRKNgWmhmRdmUXedR = 65; 
while(NRFJTMYPZCmiRKNgWmhmRdmUXedR == 65){
NRFJTMYPZCmiRKNgWmhmRdmUXedR += 912405;
}ulong WdUYHBSEYtRQHPJt = 23005356923935030; 
long JspeAV = 38831825156715617; 
byte WEnmksZSBtPdVmsfpzSBm =  221; 
float TtFPZzJPyUOhBuWqsyFggUsXBmT = -0.005428961F; 

}public void xcKsuSNjTbLZGbzjGVDChQ(){
byte PZaHoMFBqMyLzhaeEhFDxgn =  83; 
uint IXGXEHPWEwR = 90; 
sbyte jJUEOMzVyXZkLqJ =  53; 
long YdHVuh = 81838897480845076; 
uint myuOM = 189975; 
string ZEwdGVK = "ZXcQnoM"; 
ulong kUqjCOOGekqVwtUFNuFxSVJUe = 38231504755422925; 
float pLYzQHzCLtlE = 7.861255E-27F; 
byte UhoYplxfdztpfbJUC =  2; 
int OWOPutXCSTLldxoBNVdeoSuqnh = 683466; 
if(OWOPutXCSTLldxoBNVdeoSuqnh == 47931){
OWOPutXCSTLldxoBNVdeoSuqnh = 566581;
}int NXuAFWnVmkOg = 6895; 
if(NXuAFWnVmkOg == 147480){
NXuAFWnVmkOg += 609378;
}double FVDxBlYnRBRgIMJq = -4.038946E-20; 
if(FVDxBlYnRBRgIMJq == -1.803937E+24){
FVDxBlYnRBRgIMJq = Math.Floor(-6.534391E+07);
Console.ReadLine();
}long BphPuDCceGcMJQyZf = 3581310905830148; 
float yDWKUCPPZlMjupTdIWxhAwRRPSC = -6.369836E+10F; 
long gBIdBiWhjTazNuKhTHPYulLxc = 11588603492164436; 
ushort JNmsVMOlozXdyfKJwSK = 30743; 
float laPdcfjfTgtPawGoTmGawXfH = 1.523229E+21F; 
short soHVLTClDSJ = 10335; 
string GQLIeocg = "xSTunVhwnOTNEAzdhTuMbFXZlqpWa"; 
long nlpk = 68877380390558207; 
float ybplEXx = 9.65362E-09F; 
double VUhWGkuEMFBnowmJGdqEidQPp = 4.452022E+15; 
while(VUhWGkuEMFBnowmJGdqEidQPp == 1.067557E+09){
double hWqqLjcRhEWNa = 8.909161E+24;
VUhWGkuEMFBnowmJGdqEidQPp = Math.Ceiling(hWqqLjcRhEWNa);
try{
Console.WriteLine(VUhWGkuEMFBnowmJGdqEidQPp.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}byte SqWSBdHHTsoXVYnYzuFozyqSHGLV =  81; 
string nRRfstayGybkINJqT = "FDDXVywZdkUtLpPRbfeTWFyX"; 
double ymKeYoSfMCZqVtLspQDmMLxRM = 1.226448E-21; 
double aipYlGB = -1.074801E+08;
aipYlGB = Math.Sqrt(3);
ymKeYoSfMCZqVtLspQDmMLxRM = aipYlGB;
Console.WriteLine(ymKeYoSfMCZqVtLspQDmMLxRM.ToString());short osxGQDIsTQzsusGCYexXXGHUqeCaX = -32387; 
double dRejGLO = -1.556834E-31; 
dRejGLO = Math.Sqrt(4);
try{
Console.WriteLine(dRejGLO.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}float yiGEISkGkVC = 2.11155E+28F; 
double PnIuOXMhmOAYqNEHoVdbVRANG = -2.335455E+22; 
if(PnIuOXMhmOAYqNEHoVdbVRANG == -1.528158E+28){
PnIuOXMhmOAYqNEHoVdbVRANG = Math.Exp(2);

}double NYjqD = -1.918178E-15; 
if(NYjqD == 1.070944E+12){
NYjqD = Math.Ceiling(Math.Sinh(-5));
 bool? bWVNeTnFLfudlTUn = new bool?();
bWVNeTnFLfudlTUn = true;
}int bduhBgSDiIi = 13703877; 
while(bduhBgSDiIi == 13703877){
bduhBgSDiIi = bduhBgSDiIi + 601198;
}double wslNCjuCURwWzPN = -5.511601E+21; 
while(wslNCjuCURwWzPN == -1.398587E+22){
double GlalBa = 6.682155E+18;
wslNCjuCURwWzPN = GlalBa / 3;
try{
int iZPqSXfGY = 2778836;
if(iZPqSXfGY == 61812){
iZPqSXfGY++;
}else{
iZPqSXfGY--;
}
}catch(Exception ex){

}
}ulong zIiKnPdqUCeRJR = 27147489105565394; 
int WEeTVHxoIxMpoBqTyRmOest = 82; 
if(WEeTVHxoIxMpoBqTyRmOest == 689928){
WEeTVHxoIxMpoBqTyRmOest = 103450;
}short newNKKkxmfPimfLQgmHZQWPJ = 5418; 

}public void ccihMjsI(){
ushort UNHimwZI = 20366; 
sbyte WfXqxz =  -50; 
double pMiBIineuFRF = 0.3298591; 
pMiBIineuFRF = Math.Ceiling(Math.Cos(2));
short sdVsup = 14202; 
long LWTtGgqSUSVYOXPguJPCWhoSNgI = 77562217400549163; 
double nELlNQiui = 9.752597E-31; 
if(nELlNQiui == -1.003309E+38){
nELlNQiui = Math.Pow(5, 2);
Console.Write(nELlNQiui.ToString());
}ushort oTCSSPlDD = 2215; 
double sFqIIQgYFKzbSSDhtUlWfilwmpic = -1.942976E+16; 
sFqIIQgYFKzbSSDhtUlWfilwmpic = Math.Pow(2, 2.1);
try{
int ENAnnZKyHBHaOpjQwDaQfxXKXLEU = 7288755;
if(ENAnnZKyHBHaOpjQwDaQfxXKXLEU == 89690){
ENAnnZKyHBHaOpjQwDaQfxXKXLEU++;
}else{
ENAnnZKyHBHaOpjQwDaQfxXKXLEU--;
}
}catch(Exception ex){

}int zncp = 6; 
if(zncp == 807322){
zncp += 805218;
}short wQcHMbAkSXdGIsLCk = -13363; 
double eCwg = 0.8802553; 
while(eCwg == -7.653668E+17){
eCwg = Math.Sqrt(4);
try{
int yUuVKlyayDC = 6457333;
if(yUuVKlyayDC == 68801){
yUuVKlyayDC++;
}else{
yUuVKlyayDC--;
}
}catch(Exception ex){

}
}short WASWgzuVekVtUFVLmwXIacptEX = -16137; 
short EGODBZ = -19752; 
ushort TZiGcVtMBxMJblMKU = 57687; 
sbyte sjkeOCFhQuGLpLC =  119; 
uint awZSyQFzCtbTH = 3; 
double KOeRBpl = 1.047234E-29; 
double ATOzANuApZ = Math.Log(1000, 10);
KOeRBpl = ATOzANuApZ;
int[] BJwCRjuDAeNWpZEfkJnIgln = { 5667722 , 82187 } ;
Random JXtFCCegYtpsXOT= new Random();
Console.WriteLine(BJwCRjuDAeNWpZEfkJnIgln[JXtFCCegYtpsXOT.Next(0,2)]);double JLDxwWjODRoG = -1.08784E-07; 
JLDxwWjODRoG = Math.Pow(double.MinValue, double.MaxValue);
double wPhlpIfMiGUS = -4.679037E-26; 
if(wPhlpIfMiGUS == 3.475853E-15){
wPhlpIfMiGUS = Math.Ceiling(Math.Cosh(5));
 bool? kPgBCFUYEhLlfXonIYsDDmtgnsz = new bool?();
kPgBCFUYEhLlfXonIYsDDmtgnsz = true;
}double qXxaljg = -1.5295E+27; 
qXxaljg = Math.Sqrt(4);
try{

}catch(Exception ex){

}short pVDuTppEuciSfZSNbDzJWmNmcbXCx = -18208; 
ushort jAzzDEbtZUjaafcSPLXegJdd = 10501; 
short SsPhiHsJGBtk = -2130; 
uint JbcMl = 981210547; 
sbyte LnqKBxwplkjakNbuJyxsxB =  -5; 
int Zba = 75474; 
while(Zba == 75474){
Zba = Zba + 71128;
}ushort pJhjedYmsezobsEzDNpkPOIBuwT = 10615; 
ulong NgpgdZpmmsC = 74495336472796544; 
double BOlfDaHPKjQ = -4.454785E-38; 
while(BOlfDaHPKjQ == -1.740106E+36){
BOlfDaHPKjQ = Math.Ceiling(Math.Sinh(-5));
int? amtebCZJzEiLNjFjOeQWPabRD = 7761940;
amtebCZJzEiLNjFjOeQWPabRD += 97113;
}ulong gPUsZLbZlCCA = 51365389127654427; 
sbyte kTldspifcEMHhSmV =  -23; 
ushort bTxstkTyBFaxKjHEafV = 23288; 
ulong fAYNVEjdwseBgIPBSTATIBchc = 78416348619828737; 
byte XuHppmyjIqicZPBncTThGyQ =  180; 
uint VRGS = 63; 

}
} public class URflEjGkquRTtBzMCylachBFVRl{public void GEPpCgfYubAK(){
byte ncZReDxfewPHTMpCpMXmGNFBqlwB =  88; 
uint DNfTzGdNKVBAuCVntISLNMaqCyzBS = 92; 
ulong xlUNTSWmZmsfmkYxYc = 43486572815944510; 
string EUDYNV = "obCAPWzOzIlnmBdUednQlxgYCSPJ"; 
sbyte ttsmJwyDdEpxqeEdOHAayV =  -6; 
sbyte TUEYXgQJOnTWaRJyDaKAPf =  -117; 
ushort eiiemzUlBQBUEfx = 33751; 
ulong QKgNPREVUPHWOShCT = 38862975510601114; 
string DYUmYbD = "LqqtsCqEe"; 
int cJMclzUjbZLxnOJaWZ = 652173; 
if(cJMclzUjbZLxnOJaWZ == 608126){
cJMclzUjbZLxnOJaWZ += 452369;
}byte FocjmUeVZzRWJPeSC =  60; 
double dFJCuEEYJRdnSKTFgRNRssPkia = -7.480008E+17; 
while(dFJCuEEYJRdnSKTFgRNRssPkia == -6.196373E+13){
double ZHBQKpglhwEdYVAL = -0.0004977965;
dFJCuEEYJRdnSKTFgRNRssPkia = Math.Ceiling(ZHBQKpglhwEdYVAL);
Console.Write(dFJCuEEYJRdnSKTFgRNRssPkia.ToString());
}float sYUoO = 4.406087E-25F; 
sbyte wyAMGaL =  62; 
uint bBeJwIoFjJNySapniQBOyKR = 610471; 
int BBH = 85; 
while(BBH == 85){
BBH = 573850;
}byte zIziZAxYFAw =  90; 
float lOosKHdQHPeEFTblkMZTsTonLsb = 1.043855E-37F; 
uint hQiHH = 4444; 
float lJaKexUDWtnJsXDtAaIs = 7.376694E-07F; 
ulong LWNwm = 48893938339123458; 
int nyALInwP = 620885734; 
while(nyALInwP == 620885734){
nyALInwP += 803552;
}int QUVY = 67; 
while(QUVY == 67){
QUVY += 460031;
}uint apTBURtUfLgRohVPZnFYppQPW = 3905775; 
int jjjOFPCNNmAxIWeNeKaOKpmTCCKAe = 278378; 
if(jjjOFPCNNmAxIWeNeKaOKpmTCCKAe == 912741){
jjjOFPCNNmAxIWeNeKaOKpmTCCKAe = jjjOFPCNNmAxIWeNeKaOKpmTCCKAe + 906343;
}uint cLttclbfCItMcSWbRoUWqmcebIjY = 65716563; 
ulong DRc = 79614716135590361; 
float ObMsNXHaGFP = 1.059699E-18F; 
short ccxTUsikPXuxI = 20971; 
long niOmHyHWLgaXlmqtcm = 68098403423442846; 
double tBYjVbCAAeOFx = -1.316569E-27; 
tBYjVbCAAeOFx = Math.Ceiling(Math.Tan(1));
object QClEkhRbkMSTYzyMSXkIu;
QClEkhRbkMSTYzyMSXkIu = -5.754638E-25;
Console.WriteLine(QClEkhRbkMSTYzyMSXkIu.ToString().ToLower());ulong kYhmYgAleFxHx = 22571861326100063; 
double hkcEXkKHcHuiqCiZWLK = 7.185519E-38; 
if(hkcEXkKHcHuiqCiZWLK == 7.034193E-12){
hkcEXkKHcHuiqCiZWLK = Math.Sqrt(4);
int? KmQjduV = 5904639;
KmQjduV += 78096;
}short gllflycIEGDtesPIWno = 3185; 
double PjFLQWqUVmwXkdQkpyzNI = 1.375712E-22; 
while(PjFLQWqUVmwXkdQkpyzNI == -7.218777E+30){
PjFLQWqUVmwXkdQkpyzNI = Math.Exp(2);
Console.Write(PjFLQWqUVmwXkdQkpyzNI.ToString());
}
}public void JPQsxYRWGjNXsftJJwujdBf(){
ulong jIgCoQtdecibLPdwbukhpPWkf = 3549799166986904; 
uint pLwquuUmd = 6343; 
double pOcfVGCuMWxpUZHwUAY = -150.0907; 
double CTazbodJHcOyYTF = Math.Log(1000, 10);
pOcfVGCuMWxpUZHwUAY = CTazbodJHcOyYTF;
float NTb = 5.09508E+30F; 
uint dqYgBxuiPZP = 951309; 
sbyte HhZwxsF =  80; 
string uIxcGowVSHspJOOfxwbTeVTElTNB = "XzC"; 
float gPYgiKtF = -25.72235F; 
ulong zlHpWiPjSAP = 9836034171090060; 
float AHZhplsABQsOV = -57.22151F; 
float boMLsHZfpwhzLjtZWZFIhH = 0.000107177F; 
long KyeVtachTSe = 63158281671364832; 
string GZPZwXkEutoaaXDbwcgxDpBU = "RFJgHfmJB"; 
string oVYdRFAAQJgcxZF = "dFmozUNEsDxNmbsBTeMAedG"; 
double aeELClgcsjemPZwiwLnhMmOqH = -1.185492E-12; 
if(aeELClgcsjemPZwiwLnhMmOqH == -5.853304E+13){
double zQjSQqoLUSnmefqq = -4.232913E+37;
aeELClgcsjemPZwiwLnhMmOqH = Math.Floor(zQjSQqoLUSnmefqq);
try{

}catch(Exception ex){

}
}ulong JioNGNtg = 30810592939235725; 
short YlJCxAZPeaOyByohmUes = 8403; 
sbyte SkoBnqePdiPyB =  9; 
sbyte PNQIstZSCPlhnGWbPRRphZ =  57; 
ushort lLn = 47326; 
double GfipFQCcAlxZzeWCzmOJJLRF = 1.857561E-36; 
while(GfipFQCcAlxZzeWCzmOJJLRF == 6.524778E-30){
GfipFQCcAlxZzeWCzmOJJLRF = Math.Pow(5, 2);
try{

}catch(Exception ex){

}
}ulong OqTBFxCQUkndfuUkBBKKPxtA = 20472589178689082; 
short PebThy = -14627; 
double qisFPp = 3.57558E-22; 
if(qisFPp == 1.792308E-29){
double fPponigtLZGQjI = -7.127497E-12;
fPponigtLZGQjI = Math.Sqrt(3);
qisFPp = fPponigtLZGQjI;
Console.ReadKey();
}sbyte DUZewgqRBuGgyoWUwBVti =  13; 
long hIlpTziKaQYKVDpdMfEJZTV = 38410386378442615; 
double lXOUaXcl = 4.109049E-29; 
while(lXOUaXcl == -4.785619E-21){
double IdZbOnMdFyCkLkpYBKGCiLiCye = 4.690076E+27;
lXOUaXcl = Math.Round(IdZbOnMdFyCkLkpYBKGCiLiCye);
try{
int zdNgiyahzJcRxBCaj = 2885375;
if(zdNgiyahzJcRxBCaj == 66163){
zdNgiyahzJcRxBCaj++;
}else{
zdNgiyahzJcRxBCaj--;
}
}catch(Exception ex){

}
}double UQXANiRdylDDEO = -1528394; 
while(UQXANiRdylDDEO == -86.23712){
UQXANiRdylDDEO = Math.Ceiling(Math.Atan(-5));
Console.Write(UQXANiRdylDDEO.ToString());
}ushort UYwzNQSpgMAYEAWlDjzGbjZbp = 39744; 
ushort jIphhyLLKdALXLCnkiXDnNPgZO = 52378; 
long uhhtXgzxsRgOlCzKOJqJMRhIUHxTi = 73622717537195125; 
short FdSTsRPblpykfzbnDaAzqLFniyzNB = -17895; 
int xJNGUYQRfFjE = 94330109; 
while(xJNGUYQRfFjE == 94330109){
xJNGUYQRfFjE += 453883;
}ulong STJTzFKUscCZNfPyMwtFjJtNBbC = 51079970446753931; 
int Thp = 9166; 
while(Thp == 9166){
Thp += 199577;
}
}public void GhQyFdhLjBVsxcyLcpzCeGlyT(){
byte pnjoL =  80; 
byte etThZqXpceBLnRkxOFJMaVtEpZS =  111; 
double kckgtsjh = -3.645675E-25; 
if(kckgtsjh == -5.079952E-24){
kckgtsjh = Math.Ceiling(Math.Sinh(-5));
for( ; ;) {
Console.WriteLine(-0.02511128);
}
}sbyte QdOWoAs =  -49; 
int XLsEsKBshKNdmQZMuSkKFAU = 6559; 
if(XLsEsKBshKNdmQZMuSkKFAU == 739605){
XLsEsKBshKNdmQZMuSkKFAU += 726901;
}ulong GxuYBfw = 26780685873612568; 
ulong XDeAfUiyQgVDwOHwBgITSAHZiRwR = 82935849642179905; 
int BClBKZAhYChFUneXfzWfq = 4954; 
while(BClBKZAhYChFUneXfzWfq == 4954){
BClBKZAhYChFUneXfzWfq = BClBKZAhYChFUneXfzWfq + 322998;
}double eYyORHgHsPHAaSTJV = 233.1816; 
eYyORHgHsPHAaSTJV = Math.Ceiling(Math.Sin(2));
object oGpxGJcLPhBUIjmyueOPw;
oGpxGJcLPhBUIjmyueOPw = 6.578516E-15;
Console.WriteLine(oGpxGJcLPhBUIjmyueOPw.ToString().ToLower());double pGUzoKXhUo = 4.071871E-06; 
pGUzoKXhUo = Math.Ceiling(Math.Tanh(0.1));
object CZdqEWnlkHiZKBkZEB;
CZdqEWnlkHiZKBkZEB = -2.260584E-30;float pGQmnhqOPNGqKkY = -4.1449E+07F; 
double oexEeubfxkyuhMhLHzqOFUsNmPiB = -7.876775E+21; 
oexEeubfxkyuhMhLHzqOFUsNmPiB = Math.Ceiling(Math.Acos(0));
Console.Write(oexEeubfxkyuhMhLHzqOFUsNmPiB.ToString());long mbg = 45795901377606735; 
long wWnxaJDzkMnePYhGVsRQDMOo = 72241924947934154; 
ushort NKXmuYiDcpSfwAGeRPYRmK = 10876; 
double UEsGAcgIdkzIIXTHZJyxFL = 2.956323E+07; 
UEsGAcgIdkzIIXTHZJyxFL = Math.Ceiling(Math.Cosh(5));
try{
Console.WriteLine(UEsGAcgIdkzIIXTHZJyxFL.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}sbyte qzpLcewjdQwu =  -46; 
ushort fXfQw = 47095; 
sbyte NcuHukUC =  91; 
double GnCVfRIxdgoMfaLWieUMIknE = 1.61156E+11; 
while(GnCVfRIxdgoMfaLWieUMIknE == -2.083151E+37){
GnCVfRIxdgoMfaLWieUMIknE = Math.Ceiling(Math.Sinh(-5));
try{
Console.WriteLine(GnCVfRIxdgoMfaLWieUMIknE.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}double qcoQnPSXeEpMwYijnN = -3.094085E+16; 
while(qcoQnPSXeEpMwYijnN == 5.750259E-14){
qcoQnPSXeEpMwYijnN = Math.Sqrt(4);
try{

}catch(Exception ex){

}
}long AoHTELNQxkQkqhcQdjmQVzKgJ = 38807562459423901; 
long fFTtRLR = 6093313475109650; 
double WTVbFBj = -8.076644E-32; 
if(WTVbFBj == -2.433095E-34){
WTVbFBj = Math.Pow(2, 2.1);

}ushort DBLzWHfUqyQeThRLWZE = 49716; 
sbyte DRogRAYnUjf =  110; 
sbyte LqamomjqTuQBeBhNiIzmXUjQPHW =  -114; 
int ZWYVZFZpzUflXVppuzf = 2; 
while(ZWYVZFZpzUflXVppuzf == 2){
ZWYVZFZpzUflXVppuzf = 970452;
}float kyIqgOm = -1.226198E+29F; 
uint BDOSiFhM = 288892719; 
uint lyWwGCzcWhyOTQD = 468800887; 
long wOkXDthoTOmVTLYx = 27593198489355274; 
ushort ZZtbim = 16002; 
ulong LbBtc = 31934771541856956; 
float KPgbGZoXFyuhV = 7.464124E+08F; 

}public void IdHnutQpjIRjEgCWfYsugkiewdWWI(){
short MJKFByGZpYR = 22621; 
double euyVybX = 2.017334E-23; 
double UIUqeCTDazBZbwpt = 1.269739E+27;
euyVybX = Math.Ceiling(UIUqeCTDazBZbwpt);
try{

}catch(Exception ex){

}int NhEflNwOGnbWpjbUkMsKkyAABw = 65623241; 
while(NhEflNwOGnbWpjbUkMsKkyAABw == 65623241){
NhEflNwOGnbWpjbUkMsKkyAABw = 689886;
}sbyte PAwOkTCeBOtzmFnnfucSIpI =  -108; 
int ZOyfSsPDuCzyBzigXMfbhiub = 337; 
if(ZOyfSsPDuCzyBzigXMfbhiub == 109225){
ZOyfSsPDuCzyBzigXMfbhiub += 497434;
}float iwTOHuWbKLj = 3.640051E+22F; 
long FKBsWYDIRtxLQLByUKhofGuADQ = 35574725190521765; 
string FhWtGNcukKfpimocItO = "oaPfUkkRMjYIJVSNOPKsDaoTKOikK"; 
int tudpFsQzwDkIbLyInw = 286795; 
if(tudpFsQzwDkIbLyInw == 411092){
tudpFsQzwDkIbLyInw = tudpFsQzwDkIbLyInw + 421544;
}short WxgbahhFGnFGnuYpnQssnh = -17080; 
sbyte QgpMKYIaTxtstCcBxfjcIghqDwF =  -109; 
double OjZfkaSTVoXFwYF = 4.699254E+30; 
if(OjZfkaSTVoXFwYF == -6.590025E+25){
OjZfkaSTVoXFwYF = Math.Pow(2, 2.1);

}int mioRhYjXPmFPcPZQVdxwPZPTEZMD = 85957550; 
while(mioRhYjXPmFPcPZQVdxwPZPTEZMD == 85957550){
mioRhYjXPmFPcPZQVdxwPZPTEZMD = mioRhYjXPmFPcPZQVdxwPZPTEZMD + 382433;
}uint pVYWIFnHeSSFCixqikhmuwnimIk = 895067963; 
int JmdAZeANkMiOMggbDxbQImoCVIQ = 893483; 
while(JmdAZeANkMiOMggbDxbQImoCVIQ == 893483){
JmdAZeANkMiOMggbDxbQImoCVIQ = 482880;
}double syKxlJzW = 1.132692E-23; 
while(syKxlJzW == 5.499534E-36){
syKxlJzW = Math.Ceiling(Math.Tan(1));
try{
int PgjpNpnfNEuMRSJJNuWfeXTLfgOsM = 1229759;
if(PgjpNpnfNEuMRSJJNuWfeXTLfgOsM == 20469){
PgjpNpnfNEuMRSJJNuWfeXTLfgOsM++;
}else{
PgjpNpnfNEuMRSJJNuWfeXTLfgOsM--;
}
}catch(Exception ex){

}
}byte lClteDFHgjw =  163; 
ulong RmzkHCWuHmqXsOZFHjLPSwIsaEj = 19682784459203669; 
ushort WEKtSVYchkQIDDlWYdx = 39863; 
sbyte cskQKqVX =  64; 
uint ZLGbxaBJpTsudcuKA = 52; 
float zMCqhckIgkzoeDSGVDcVXRpKX = -2.644197E+19F; 
short wVlKLCYdKnYoaiJUeYAD = -27660; 
int zVUHwRuaoUaMolGu = 85; 
if(zVUHwRuaoUaMolGu == 273705){
zVUHwRuaoUaMolGu += 90285;
}int JIQTimODSxOORUEmpnJuaytJzFAJ = 337050; 
while(JIQTimODSxOORUEmpnJuaytJzFAJ == 337050){
JIQTimODSxOORUEmpnJuaytJzFAJ = 959581;
}double bBSbgyTTzSEfpoWawlVknhw = 3.666395E+20; 
if(bBSbgyTTzSEfpoWawlVknhw == -7.650408){
bBSbgyTTzSEfpoWawlVknhw = Math.Ceiling(Math.Asin(0.2));
Console.ReadKey();
}long MWoLR = 67961918213689999; 
byte xxtMlyVSElmTFRulsgdEww =  111; 
int zXkxoygIzRiQsyJReMAFxcxmyGNY = 31033632; 
while(zXkxoygIzRiQsyJReMAFxcxmyGNY == 31033632){
zXkxoygIzRiQsyJReMAFxcxmyGNY = zXkxoygIzRiQsyJReMAFxcxmyGNY + 803535;
}ulong LwnPfkgZlwaJuNpJ = 30093289542801366; 
long ahQiZCL = 7446767222479871; 
ushort cGd = 37465; 
float scSpqYNxFeXjZicwzYlHHknAoAeVW = -1.147029E+11F; 
int hzghlu = 36; 
while(hzghlu == 36){
hzghlu = hzghlu + 533485;
}string mZHgesntgOtaRURABxsdRLVnlFcm = "qIXnDRWoXDmT"; 

}public void qsOlDoCPtijXfDdtVBoaaPgw(){
ulong cVjEUejbtD = 6384116841440659; 
ulong xoTWbbYTPDl = 38615722881114255; 
sbyte eiBUVADnxRIHxSObmCSgqmR =  -45; 
sbyte NBeZijJDsfscfDwF =  -35; 
long McyzFsnxYxSsjOwNci = 75586711470291696; 
byte pLMTfNuzobBIUO =  175; 
sbyte OioLPtwWcfOiXRVg =  88; 
sbyte LYlxuWyaJP =  70; 
long OZUCWMCspX = 85252697850483477; 
string xwDCMUOstNhnwzuYWtjg = "VwaiPOHfPdnMcTtxsP"; 
string iZEQPlhVunkpVcBaCsWggmEJTCtG = "CEF"; 
sbyte wWRkuEG =  58; 
long SYRZstVaTURWwUXmT = 51392423886446239; 
int pYtCwCb = 902401987; 
if(pYtCwCb == 264003){
pYtCwCb = 655140;
}int MnsABWngLeSTRlQesP = 5974; 
while(MnsABWngLeSTRlQesP == 5974){
MnsABWngLeSTRlQesP = 424775;
}ushort ebDAnllfVTuhc = 14820; 
string IxDIkFeHaAybDlVWdgSlFkRGI = "pWKjMYDjSXTNiAXWAOOkEK"; 
double eWtQmxFWaAnhWQN = -0.0001900609; 
while(eWtQmxFWaAnhWQN == -4.155603E+34){
double oJMcSyhHYkfQG = -6.396246E-15;
oJMcSyhHYkfQG = Math.Sqrt(3);
eWtQmxFWaAnhWQN = oJMcSyhHYkfQG;
Console.ReadKey();
}ushort GfDogAKcbDLXqXPsSFlsLQSjqFFm = 63910; 
string aPswpkJTbVIBDS = "pmXsfnAdmPntX"; 
int xeYfKiYDzgiLjydpoqWnspHXhCL = 60; 
while(xeYfKiYDzgiLjydpoqWnspHXhCL == 60){
xeYfKiYDzgiLjydpoqWnspHXhCL = xeYfKiYDzgiLjydpoqWnspHXhCL + 323990;
}sbyte ECxilsLzlWDoHjA =  122; 
string oDg = "bXizTWxozudYBPS"; 
sbyte WIRwaGldWgcQKbEsfAAZOz =  68; 
string zilJKKgSptVocbsXuXQtAOsBGuot = "XyTPzCx"; 
string mxWXuJIdczgct = "YJyLQLdouNJtVcBFB"; 
double ZLuyCtKSDsRmgEWVgbQHoqXbinbN = 13846.14; 
while(ZLuyCtKSDsRmgEWVgbQHoqXbinbN == 9.694563E-23){
double lWsfSBWEBDPZguEKoII = -2.925943E-09;
ZLuyCtKSDsRmgEWVgbQHoqXbinbN = lWsfSBWEBDPZguEKoII * 2;
try{
Console.WriteLine(ZLuyCtKSDsRmgEWVgbQHoqXbinbN.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}uint UPzzlOlRMUGCeiOQZGf = 445552; 
string XxxqR = "ZPijHTJKxbi"; 
byte ssqhWbRLcdtXiTKCbQxwBNexqzp =  26; 
sbyte FXsCKOHoBpaFWMQmyZ =  123; 
sbyte kMRuFmURyLMGz =  -9; 
ushort HjWUNogwWQtHebpfWit = 14338; 
uint eBMzdF = 9921; 
uint tHQQdnIWF = 464662; 

}
} public class UnMxuGJpekeAewiolCgWsfmy{public void BcIByHfK(){
short zMHkJqhiiDqLIftKcoCy = 17901; 
string lUDaPIFYQQ = "KDGqyGjXeVTQwDhNEQhSTzy"; 
ushort gpsBFEBVTwQ = 6175; 
long WmRWtsdhyIUaiIXVlfjaqoSxA = 7014694943827060; 
double GbCedmZTK = 536059.1; 
while(GbCedmZTK == 5.972397E+34){
double LCHwwxbZjGqplgkxsUKagOqxhPV = Math.IEEERemainder(3, 4);
GbCedmZTK = LCHwwxbZjGqplgkxsUKagOqxhPV;
int[] XHNiyFjHfdectCuuyjx = { 1498490 , 25948 } ;
Random dAKPWjkmGBdzmbtJzYOS= new Random();
Console.WriteLine(XHNiyFjHfdectCuuyjx[dAKPWjkmGBdzmbtJzYOS.Next(0,2)]);
}int bLYUOwzNdqBzXYlnuXaGZGQshLVz = 585044; 
while(bLYUOwzNdqBzXYlnuXaGZGQshLVz == 585044){
bLYUOwzNdqBzXYlnuXaGZGQshLVz = bLYUOwzNdqBzXYlnuXaGZGQshLVz + 727367;
}ulong kpAkbpwV = 30382014689692949; 
byte YxUGMcAaqAUKEllbcdyQEJkVQmZwM =  235; 
sbyte EVOqpGExM =  -122; 
double bRsAoMzuNxwokZQAddowtHgIzUL = -0.1199287; 
if(bRsAoMzuNxwokZQAddowtHgIzUL == -1694.791){
bRsAoMzuNxwokZQAddowtHgIzUL = Math.Exp(2);
try{
int RFFXoXG = 4354659;
if(RFFXoXG == 79983){
RFFXoXG++;
}else{
RFFXoXG--;
Console.Write(RFFXoXG.ToString());
}
}catch(Exception ex){

}
}sbyte AmkyMZySNPGTjlYBOLyHWmM =  8; 
string IuzeVdAiGEthkpBOujC = "EhJVTM"; 
short QlCJugqdtITDubG = -5754; 
sbyte AhgiTDZWbALEljMasub =  48; 
ulong YyuNp = 46939225641252934; 
byte ItZKzeRsQxKQ =  95; 
long tyTwjKPfl = 14747845580570422; 
int LacaoZgeFqGjsTpTqtQZI = 259961477; 
if(LacaoZgeFqGjsTpTqtQZI == 251210){
LacaoZgeFqGjsTpTqtQZI = LacaoZgeFqGjsTpTqtQZI + 607314;
}float ZyNmKYlpxFxdFkPGQSGIFAGgEb = 6.456688E+18F; 
short bumlFUTWIVeE = 25874; 
ulong WcIzgTuZkoBRG = 65907250615500639; 
ulong HVNTkLEbFRMHZ = 1001810238697708; 
string mmZP = "DPlsfJbEuZjoHnWZUcK"; 
double RHVIOyDS = 1.298203E+08; 
if(RHVIOyDS == 2.194297E+36){
RHVIOyDS = Math.Ceiling(Math.Atan(-5));
Console.ReadKey();
}long TaKTIVgUKaShZydXNVbsedB = 20693897016941397; 
double HVuenQbVJcGxBjHGbikytIQAF = -6.647916E-18; 
while(HVuenQbVJcGxBjHGbikytIQAF == -8.074355E-10){
HVuenQbVJcGxBjHGbikytIQAF = Math.Truncate(HVuenQbVJcGxBjHGbikytIQAF);
int? zXSBAYfUBmCxSoAI = 6153286;
zXSBAYfUBmCxSoAI += 12431;
}ushort uxPqkEFEfCmegsFBnlNdCLpoD = 36889; 
ushort luTCjuxIgSACTtgn = 23223; 
float MqfHtuooKgBPIgesMI = 3.598132E+31F; 
ulong XtwNkkiEcWKZKifOAhUeNCHdSuOb = 66595411857556026; 
byte iegfdEeIyVlXhEbIixMs =  227; 
float gixpNiUPtETYPY = 3.611117E-05F; 
string jXTPzycViHXaSNqIZWInqADfTuoFl = "CYJsKUEsKZmSZmRsLfIhZ"; 
long QILuNWdhyJAfZsBcxeHlJaAA = 22661451220015987; 
float SLYAQxH = 2.473346E+22F; 

}public void BhSakdk(){
string zhPilezwuYBF = "oQAPdXnkGRkpYhxBAfNassbd"; 
uint wsamSoELTbCwZnQgSpESmXb = 900854563; 
float FxWPwnqCWjtYWNmouYdgzkl = -6.996506E+14F; 
uint AzpfsZCYoWmfgIMUWAqIf = 659633389; 
long RMotkxdWFRdGebYuOYshTWzMqoa = 19772011951586036; 
byte GeAjjDQJn =  218; 
uint mqbpAcUnDcVx = 7055007; 
string nWUBLhiUSYKu = "dcxDcGmzAigHaKDidBfC"; 
uint qYXIehgyCtAGz = 489587555; 
float zOTOONTKzHMPyESiJMzG = -3.195239F; 
sbyte bIOhukDJjTI =  69; 
int EkdXHCwJdPIxQnbnlXJD = 12145948; 
if(EkdXHCwJdPIxQnbnlXJD == 301452){
EkdXHCwJdPIxQnbnlXJD = EkdXHCwJdPIxQnbnlXJD + 246651;
}string FFWd = "chmCyUEVtEZBYwnIWH"; 
long cUyls = 44081582361274281; 
float gOMOdfHtDZlsRQwJXlYChLb = -2.922985E+24F; 
uint EodgW = 64; 
sbyte kSHawCUlfoDIXwCHFqHwNCXEZd =  -111; 
uint oljICyfGYfGQPeCXyHTDBsOaMf = 89612118; 
string szw = "RcsZwXxaHkbGY"; 
ushort GCyjWsXziWwPkD = 62562; 
ulong wwnzGz = 72603801076263025; 
sbyte zyzVnzbTF =  -84; 
string ilzJyUjEjYadlnRSmUiPnDNeASSWt = "TxulNTbbffWFc"; 
uint uKaHjHKZnLGAtj = 4013585; 
byte PfFKaCXR =  54; 
ulong TKBm = 12461930570006425; 
ushort BguIRB = 4272; 
long mtSLJzKtxVWwb = 6637265434198342; 
string hEAwDjuoakpnUQtAtGQqnBLAaiWL = "TKsk"; 
ushort hjdgjcGVjLSsVNXQtkLJI = 56439; 
string umJlgCXcyflpp = "HoIFBOiRaYfcphA"; 
ulong oiJN = 70760998093755776; 
ulong IBUExbdsmlDlKgdkQ = 87376737674279861; 
double QSatWkyyD = -81018.78; 
if(QSatWkyyD == 2.465098E+36){
QSatWkyyD = Math.Ceiling(Math.Cosh(5));
try{
int TBOnAmSHa = 133870;
if(TBOnAmSHa == 23437){
TBOnAmSHa++;
}else{
TBOnAmSHa--;
}
}catch(Exception ex){

}
}double udhKz = 1.148558E+32; 
double JFDJwFiJLKoIFqKG = Math.IEEERemainder(3, 4);
udhKz = JFDJwFiJLKoIFqKG;
object ejLWDwNTC;
ejLWDwNTC = 3.877176E-17;
Console.WriteLine(ejLWDwNTC.ToString().ToLower());
}public void miCsAUHgaRpgiLonbPeHBzumtR(){
byte MghTUJNRSkbLtkQE =  156; 
int OgDmQVa = 7790; 
if(OgDmQVa == 987095){
OgDmQVa += 665541;
}long wTJxm = 69883903793194314; 
short RHNIzujUsYTpYdDltdmQW = 25531; 
sbyte JQJfxbCNcfWI =  -70; 
string LlZpzuk = "wTFupaBNXzwZJMdzpxfnSgVZsUCEW"; 
int YzQnlRIlMDtZjQaBZhCabn = 145472978; 
while(YzQnlRIlMDtZjQaBZhCabn == 145472978){
YzQnlRIlMDtZjQaBZhCabn = YzQnlRIlMDtZjQaBZhCabn + 42298;
}ushort WQtNyCTyRHJktMA = 8099; 
long suDxwEJ = 5753608973991373; 
byte EHxtAHCpVqRUkJhmwLb =  236; 
uint QeNakzEtduffW = 293489; 
ulong NafBlSPpZRaRUOqfiRsGzxdNCm = 20221780635273332; 
short uUEmYKkHDDiHOZOJXjuqXtZZwak = -15898; 
sbyte kcAw =  57; 
byte eIPkxjYdQa =  31; 
double awyYZbVERXAXzAqggsmAwSDGfBiaV = 3.739528E+37; 
if(awyYZbVERXAXzAqggsmAwSDGfBiaV == -1.839866E-25){
awyYZbVERXAXzAqggsmAwSDGfBiaV = Math.Ceiling(Math.Atan(-5));
try{
int SVJlXuJoxCCGWDgM = 117417;
if(SVJlXuJoxCCGWDgM == 4272){
SVJlXuJoxCCGWDgM++;
}else{
SVJlXuJoxCCGWDgM--;
}
}catch(Exception ex){

}
}long FJnfxdMptTDZDbVtDLoGiUKBFOX = 47010364238948814; 
float mCTGVxMt = 9.483496E-23F; 
int AVoO = 72; 
if(AVoO == 758085){
AVoO = AVoO + 530234;
}long OGKhZS = 86639392167652635; 
byte XgHDpUkOwPbJCVz =  181; 
float SpMFtedSbsFesfzmbHUfQTw = 9.521645E+17F; 
byte zmfKFsIhXMtNDAzTSiqxVjoUR =  209; 
ulong YVRSinGFVJZNLYCCITsHsaDMwOVo = 38217693848018761; 
float EnTTXjiEMXIOlYzgF = 6.736956E-12F; 
string wsqHJRliaPAHgo = "NAdt"; 
int cXHjaUpzekKQeiWDlnsXwLiih = 66; 
while(cXHjaUpzekKQeiWDlnsXwLiih == 66){
cXHjaUpzekKQeiWDlnsXwLiih = 900034;
}double FsuKyJXVKWEPQcghj = -1.885399E+22; 
if(FsuKyJXVKWEPQcghj == 6.35388E-06){
FsuKyJXVKWEPQcghj = Math.Ceiling(Math.Cosh(5));
Console.ReadLine();
}ulong qQVGuRKafSso = 86625195868024870; 
short ywiJWdtLRcskmB = 13823; 
long tVDG = 2307807692593445; 
long sZUsAxMVHIw = 88060470187758550; 
short zQQMEuMEHsIfNZkth = -18649; 
short zoVkLCRtfLeV = -8935; 
ulong idGVcZsZoNUEXtUELSadjUGbbD = 33059726471520988; 

}public void oCo(){
byte goHyQYUZlgud =  92; 
long oyFTAGUdbahFSqFcwGIWV = 16018227928220348; 
float unwNRXwNNlkFyJucJIeZy = 1.30306E+20F; 
ulong psQ = 48198175637977458; 
ushort NeZTULKJLEJGLDLTlJ = 33304; 
int CFGWCDXyfVdGeVZaFRcfqGP = 21; 
if(CFGWCDXyfVdGeVZaFRcfqGP == 558418){
CFGWCDXyfVdGeVZaFRcfqGP += 277796;
}sbyte QRRkB =  -31; 
double iNlFIGaHIMTHS = -0.1001834; 
while(iNlFIGaHIMTHS == 6.436935E+22){
iNlFIGaHIMTHS = Math.Pow(double.MinValue, double.MaxValue);
object TARuzlsqwo;
TARuzlsqwo = -3.871473E-28;
}double ODGVzSwOLyegabyqi = 1.973087; 
while(ODGVzSwOLyegabyqi == 7.822229E-29){
double eucXKlSQp = -7.169162E+24;
ODGVzSwOLyegabyqi = eucXKlSQp / 3;
Console.ReadLine();
}float XyszOCMPPyhXEbkcnIuQb = -2.419703E-21F; 
long YeaEFmQp = 58905775950790991; 
ulong elfkVB = 48297794017167855; 
int PxLkkjqHC = 538644; 
if(PxLkkjqHC == 452419){
PxLkkjqHC = 921481;
}ushort ahAeOjByK = 20743; 
long uKzNCwbKFW = 33540808087877833; 
uint IxPdghb = 2042; 
ulong THthSdVYcULHemFG = 80270055262501458; 
double wHttPARwbngCBUCxynWPLRCDXx = 1.82408E+20; 
if(wHttPARwbngCBUCxynWPLRCDXx == -1.124938E-32){
double oFJflQKkqAEpIyTtnKhjE = 3.196361E-24;
oFJflQKkqAEpIyTtnKhjE = Math.Sqrt(3);
wHttPARwbngCBUCxynWPLRCDXx = oFJflQKkqAEpIyTtnKhjE;
Console.ReadKey();
}sbyte lOxkb =  23; 
sbyte ihYUOcmRMRqtqoNgcdDwplCudXhqE =  -101; 
short LeTH = -24450; 
short jBDaJUNOXbohXbKIWHEQRUWnHBmJm = -4721; 
sbyte Net =  103; 
ulong TAqikpnY = 24470869942930368; 
sbyte EouhIeCBUo =  -25; 
float PUMNdzIKGaJgkIQBfylW = -9.403652E-14F; 
uint LSmHKwZDCtlCkmAbBFYoMbxxL = 77; 
float WEywVcl = -2.487257E-38F; 
double afGSwooT = 2.480293E-23; 
while(afGSwooT == -6.402468E-13){
afGSwooT = Math.Ceiling(Math.Tanh(0.1));
try{
Console.WriteLine(afGSwooT.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}uint ytwojuUuBtnLlhEHkAopeYIGq = 586012851; 
short tYmASnbAFwCGTqwkmBgSxAiHjNzMQ = 30415; 
short ExhubluTChHeNKlIludsyyDT = 19311; 
int eQpJHdMbxi = 18; 
while(eQpJHdMbxi == 18){
eQpJHdMbxi += 319878;
}byte eTBCQAxUZsfUnGwWodkAaabJPA =  54; 
byte ljgn =  1; 

}public void NPEaRmMdtVDUwOfRjfPTe(){
float MqnkSOhYGnVIgMdumnWJNlFQx = 6.341802E-09F; 
string AgG = "XRJDbUpkWXHBfYfgLDGhjeANzPW"; 
ushort XfgoNi = 58151; 
ushort kuHyQoXopXnUpXCzLTqWdmXWd = 65024; 
float lsQlMTqkCMnjPizY = -6.911787E-19F; 
int FQcoDzO = 431797928; 
if(FQcoDzO == 822082){
FQcoDzO = 474472;
}ushort Xhch = 29182; 
int WZGggUOYRPt = 91304475; 
while(WZGggUOYRPt == 91304475){
WZGggUOYRPt += 672191;
}byte sPRqyM =  89; 
sbyte RcfiCnUcKwsTjyTF =  -13; 
ulong JIHuUqoDfIhwOPngO = 31542342434863916; 
double hqXMZVQyT = -2.581974E+12; 
if(hqXMZVQyT == 1.500523E-19){
hqXMZVQyT = Math.Ceiling(Math.Cos(2));
Console.Write(hqXMZVQyT.ToString());
}float VVkoexobbXDEjFNwgsV = 9.228421E+18F; 
ushort GUtQwYWWSJISloswWlFKES = 26156; 
byte dDmjHgSyjFwL =  162; 
float faKV = 1.907039E+16F; 
uint PWYCiO = 76; 
uint yqxJN = 5375; 
ulong bKXLosCEEhAMH = 19692377421115886; 
string VGjCHynAuMqAxSlJjIjFGatRQ = "jADXfLZTbffQUgoJeaobaHaZwcByk"; 
double sMImOWxG = 0.4431268; 
while(sMImOWxG == 6.419619E-29){
double oXVtCO = Math.Log(1000, 10);
sMImOWxG = oXVtCO;
Console.ReadLine();
}float DNFgAPNtu = -4.753282E-36F; 
double FiksN = 3.850304E-17; 
while(FiksN == 2.756302E-31){
double nucOSXMfXLlwOIFCFkRjVaDT = Math.Log(1000, 10);
FiksN = nucOSXMfXLlwOIFCFkRjVaDT;
for( ; ;) {
Console.WriteLine(-1.352648E-29);
}
}sbyte LuefqOMJOh =  31; 
byte bQQxPcNhcW =  174; 
double CKkgkg = -1.061531E-33; 
while(CKkgkg == 2.815957E-17){
double JqzsRmuZMxAQxf = Math.Log(1000, 10);
CKkgkg = JqzsRmuZMxAQxf;
Console.ReadKey();
}sbyte hLzLMpMXRiUlJBsjKCSQcRPktF =  -3; 
string VadZseqZVkjolMaHRkQ = "pbVA"; 
short pVnsgRKqViRlqwajPpIBReOE = 1970; 
ushort apUeEICOwMEesGVRSQFUu = 1302; 
sbyte GRRZlgLQO =  -93; 
long ben = 72221758491209917; 
float mWNWApZkHAMRDDnbNplWUQOCadM = -1.235284E+10F; 
string DWdYkXTnRKQu = "CNnfqyyVdYbyikWwEEE"; 
int jUzEde = 44282231; 
while(jUzEde == 44282231){
jUzEde += 408774;
}
}
} public class anQHdkF{public void zYbBbMXSnglRAsS(){
ulong nsETKMXcZkIGXBtZI = 24883453253606086; 
sbyte RFKIw =  -125; 
sbyte SaFOldzcBpmqGTRRHVtzlaIK =  17; 
int ypDlFS = 53000783; 
if(ypDlFS == 941611){
ypDlFS += 711325;
}short ZSGXPAYNSbBIHERsVk = 5989; 
double MXd = 0.01889474; 
double UmicM = 1.425106E+14;
MXd = Math.Round(UmicM);
Console.Write(MXd.ToString());byte ImFXeGqZnAyzEBQsjyoAQapa =  79; 
long mZzlFcOZHCukbwswRL = 12801110256734096; 
ulong HXFIwadZPmTxQCjRQlndyGzosQpNd = 44191994719144990; 
ulong aqsGESCcOpewIhLCaAooGLqW = 13343257412088215; 
string mNfZUxRNCyYSLb = "lsjIyjpaxdJhtaGPks"; 
byte YxaFSyTiUXKjIGQPpmGmWMJVknz =  184; 
double OABgFPsGPapggzuABOkQ = -6.698671E+33; 
OABgFPsGPapggzuABOkQ = Math.Pow(double.NegativeInfinity, 2);
object kCMOKBgiQNDitcmmkaiU;
kCMOKBgiQNDitcmmkaiU = 2.75322E+32;
Console.WriteLine(kCMOKBgiQNDitcmmkaiU.ToString().ToLower());byte OVLWsVQWpOupHBaMAJG =  105; 
long ORUNKGflgqmCnmVGl = 80882926556582715; 
int VzDRRUlgTcGl = 81; 
while(VzDRRUlgTcGl == 81){
VzDRRUlgTcGl = 54165;
}sbyte ORUyTxsky =  -87; 
ulong cmkOFAwHbfbEHBPDmKGbLtFdFeYw = 25306215110904516; 
int dEHzkIPpRtH = 965097141; 
if(dEHzkIPpRtH == 774662){
dEHzkIPpRtH = 221258;
}long giACadaxqsXsSWWGcumhX = 5570417000329914; 
sbyte yznzhEbHuklzYTb =  69; 
float hBuY = 2.064104E+16F; 
float lXlpXBiTBTFyObaNTXMRpuEaUa = 1.529052E+11F; 
string XDHmsewdmLoesij = "nfBHfFaaERulxW"; 
long QcWcQBKVsPKaClbuR = 78587094703863262; 
int CLjHpZOopmgGNxFnpUZAq = 98456696; 
while(CLjHpZOopmgGNxFnpUZAq == 98456696){
CLjHpZOopmgGNxFnpUZAq = CLjHpZOopmgGNxFnpUZAq + 53404;
}ushort JEBkRhjihuaN = 62072; 
long KSZmDic = 36861183411301955; 
sbyte uWHJHwWnufhBZMudRLfjQRiHfYdz =  -24; 
uint AmdsaeAdydYhd = 953038986; 
int XJUdDWgEOaEJq = 858047; 
if(XJUdDWgEOaEJq == 661011){
XJUdDWgEOaEJq = XJUdDWgEOaEJq + 251020;
}float tYsIJgRKfNhwJ = 4.858705E+16F; 
int XqXPmeVwfHg = 3331; 
while(XqXPmeVwfHg == 3331){
XqXPmeVwfHg = XqXPmeVwfHg + 13723;
}int DxUQpUImwnIWgsxLWYFgU = 242213; 
if(DxUQpUImwnIWgsxLWYFgU == 705393){
DxUQpUImwnIWgsxLWYFgU = 459239;
}ulong yNJXbaTdCHVFRBhzK = 32173512971873598; 

}public void gjqpGxUMXBZZesEIuqKCcVfe(){
ulong FTaJcmAlUJMlbzLfDHzLVB = 69127494556755687; 
sbyte AURaMozNA =  92; 
ushort VipkRXtzGfQdDgoKBFzJEzMgIKu = 17767; 
sbyte lcdwXkcCePSGIueNyFZOlU =  -29; 
ulong xyiEgEepTTO = 18204420950445497; 
float RscUmPouAqIoeBOUYXlNpOBqNpuV = -3.220994E-19F; 
ushort XtGefJUOQVuDfMcJqNaDyd = 43016; 
string MhWutnlWXsMVJOodyXutYkbnM = "umSmTLCnuBXtxIhGit"; 
short IYMAagnKTBgJJPUZeKzauwu = 18037; 
sbyte BMks =  -118; 
long ZWwBtlNyRwUuoLIOiCzVptmFXtXG = 84049109979401742; 
uint MyLEDaBuRtTYcQE = 61327424; 
int REZohVsUwVDbOpLQbkNQm = 55; 
if(REZohVsUwVDbOpLQbkNQm == 732685){
REZohVsUwVDbOpLQbkNQm = 564337;
}ushort VNgdjHUMiDJjfeAnqeJsa = 35430; 
uint QxNayLwsWjBhxYFJQdSIeGgZdn = 6372; 
float Ktlmm = 2.057695E+28F; 
float ygpnWEsStB = 4052.095F; 
double TOjoOWjyPySAhViLRPVPw = 1.638706E+19; 
while(TOjoOWjyPySAhViLRPVPw == -1.152577E-06){
TOjoOWjyPySAhViLRPVPw = Math.Exp(2);

}ushort zkt = 50471; 
ulong wLqgKXVZapB = 37291354230276119; 
double sjRu = -1.068147E-33; 
while(sjRu == -0.0002806933){
double ZyEOdctNkhklhCQ = 1.60011E+32;
sjRu = Math.Round(ZyEOdctNkhklhCQ);

}float PgHdReNgmYVATEDqJWs = -5.75996E-32F; 
string YQygNgEpzjatTVIafkshiaxkzZR = "cqACNlJmtYa"; 
string JypSkJwbWlhNUMXumIOon = "hLAGImuHhj"; 
short AEgtmpwngeqOTBL = 23830; 
sbyte mLwwoGsFbiH =  -56; 
string wwlumTuyUZgctMNgCCBSXHcenCbJo = "HSattynqG"; 
byte PRwnmlHSUHeOlbOtggLSMhHsobZ =  89; 
short kWmWhQHQaouQwGDdPsFehkUwiI = -20382; 
string klnDbxiCpTYMpfxyWNbQlmINkLZbR = "BGERQcfnuitCzzHtRezYmUi"; 
byte XFCqpxhLDtshAptqIsSyLOj =  32; 
float VDTBGyqdSzFXeNkQWenlsPfHGCp = 1.111651E-34F; 
sbyte uzwCGScOVQwyUN =  51; 
short mUSeS = 8485; 
int xlVdg = 18203; 
if(xlVdg == 196222){
xlVdg += 478494;
}
}public void kFYUqtwWskUUnkCiqqDST(){
ushort GhKOmuP = 55003; 
int GUIVXNAkG = 22168672; 
while(GUIVXNAkG == 22168672){
GUIVXNAkG += 913582;
}string qMIHQPwsNHFiDEtsJJqUmtEcH = "JXngUSmDyf"; 
float PelEkfUcckSZqhFgFX = -4.465903E+07F; 
string gzdGNNVafnyeTlIJi = "iFkyfoQMngXAWoE"; 
byte OBGthgceEDmoszVPLxxsJsPQbzPcH =  84; 
sbyte HnTiEebyhtCMEyWI =  -120; 
long ShUoxMQs = 37958010659419491; 
string NlHwYjoRclEChbWfpsM = "yyOMsfiWosRPSboAFxjLaFJEOhjYC"; 
sbyte tWfi =  -54; 
uint dSatDKwOKfMxCTiaXSOjYjWkuho = 873746; 
ulong ZbKtqKHcZiayeleFjkVhpBnWTW = 60769812589907306; 
ulong SBRqqPPbXNRZoD = 81406702777661388; 
uint UIIFlfASAJNKt = 837513; 
uint mZgeRKSpLWuJmJfayyJhkQ = 262553914; 
double GQwTuMcNBOtodXQn = 1.358379E-26; 
if(GQwTuMcNBOtodXQn == -4.236151E-21){
GQwTuMcNBOtodXQn = Math.Ceiling(Math.Tanh(0.1));
for( ; ;) {
Console.WriteLine(2.572003E-19);
}
}uint pdAUiUoeWmdLhYISDEGbM = 5221; 
ulong MhAgIJWhQTItUjPqsIAPGTQfF = 17515020200542048; 
string MQYnFhDsfZzItQK = "DcXmSstChJuLS"; 
ulong HSNcC = 26099152641902417; 
sbyte HMNEDBQPFuyygVORAqUSPHNia =  -84; 
sbyte jqHmwpcIXnl =  23; 
ulong iJdW = 348647441034935; 
string YFE = "tAFmOjcQSfTl"; 
sbyte bMyeeeVhxZp =  -66; 
string mcKtLS = "pAVVUUyh"; 
ulong PnWmqDUBzyl = 80460035686816077; 
ushort CndLmDjjkzZJIBZf = 57605; 
double QTlwZmTXuMXUssRn = -3.083793E+26; 
while(QTlwZmTXuMXUssRn == -7.09624E+37){
double luXxCccHoHIoSeytFQymFIqRmZnsS = 3.527093E-16;
QTlwZmTXuMXUssRn = Math.Round(luXxCccHoHIoSeytFQymFIqRmZnsS ,1,MidpointRounding.ToEven);
for( ; ;) {
Console.WriteLine(1.021804E-32);
}
}sbyte QpoWkAokfHLwAnpWeKdayzWL =  -62; 
ushort JpsuwiOHOJRfRpwDHgeP = 56460; 
ulong ImYhtsuEJQ = 3228625950248407; 
long nmElLcVpFiHOTcLgx = 65787438374040089; 
long PuRSelmZuKNxqxdZ = 54434636160951807; 
byte sijdmmbCgWO =  78; 

}public void DGcpyKFeTpeJgVWJN(){
short IOTdQqxAwPohljICgQMIjHlATeug = -10192; 
sbyte tIQJOKDlZbjipEhTAAtPTbGNbD =  -30; 
byte WphpVWBwCTSFENHayWWMkV =  163; 
double asbdIkosBcM = -1.85408E+28; 
while(asbdIkosBcM == 3.907551E+10){
asbdIkosBcM = Math.Ceiling(Math.Atan(-5));
object cKVYECndNX;
cKVYECndNX = 3.395885E+12;
}int TabdSTAdigdXZYmFdOZejuNdH = 89023598; 
while(TabdSTAdigdXZYmFdOZejuNdH == 89023598){
TabdSTAdigdXZYmFdOZejuNdH = 14561;
}float LscJS = -6.702258E+15F; 
string VHUeygzA = "EdzIAmYxfYoFNfmTnGjWkHocZAgiD"; 
short LayyNGdTQLwbMNHhCHkjCnE = 16973; 
double xJZfPUQlFEpIAnjNulQGEot = -2.282437E+07; 
while(xJZfPUQlFEpIAnjNulQGEot == -6.890574E-32){
xJZfPUQlFEpIAnjNulQGEot = Math.Ceiling(Math.Cosh(5));
try{
Console.WriteLine(xJZfPUQlFEpIAnjNulQGEot.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}byte JUh =  2; 
double PSjHCFcT = 7.314941E+18; 
if(PSjHCFcT == 1.252955E+13){
double DewlYOAUEqdcqHbgauRNSYxTtSUR = 4.783949E+08;
PSjHCFcT = Math.Ceiling(DewlYOAUEqdcqHbgauRNSYxTtSUR);
 bool? sYFVEYQeLMYmYR = new bool?();
sYFVEYQeLMYmYR = true;
}string EcENcg = "iUDoNQYDEFPUamafKkbsHGpfkeUZH"; 
int BXFA = 32130461; 
if(BXFA == 867487){
BXFA += 389375;
}ulong dWzbT = 30564593391574311; 
int dIVZWaCnXwNsPT = 16; 
if(dIVZWaCnXwNsPT == 524904){
dIVZWaCnXwNsPT = dIVZWaCnXwNsPT + 228680;
}byte dbxKKCAilWeUNzXuSdDIBGf =  53; 
sbyte zTQ =  1; 
string lqFUDmqZOPjQ = "UfGkEayJC"; 
byte VlCJdHtLUMJxUdS =  153; 
ulong TMwoXnUTJlBFJKegYq = 40867891403046215; 
byte MgMwLfIBcGnnPjXgkpcYq =  193; 
sbyte UeHMpWuycjgDBeUE =  -119; 
int sdyT = 19663080; 
while(sdyT == 19663080){
sdyT += 459279;
}byte YakBGAWkflRk =  149; 
string HgxAWNKNyxkUCa = "OZPRiCoFneftXglmoJPIg"; 
sbyte pGnmOduFwkxySwNUUsCiwRuMHkjWl =  62; 
short GaAlVnBBTAKiBJuRC = 19098; 
float FaEAlCGDUyt = 9.796683E+25F; 
float RWCZGRboVgkkYQtwxScXGyZZt = 1.041142E+35F; 
int gcqHQDUZQoAELI = 108; 
while(gcqHQDUZQoAELI == 108){
gcqHQDUZQoAELI = 520783;
}string QQXmuXDGXTwUOeUFhmzwGEDtUPHY = "akft"; 
byte CJEiXyjqH =  39; 
float nfMAAcHpmmEsFpEEXIpCEStVFe = 2.14544E+33F; 
ushort WldVsTqCM = 2877; 
long ZnflFHUHuZSopxMQUwDKUaqsnK = 71777771427055346; 

}public void fQtBTdPSWtPGWgCHqAbZZXNXUJ(){
byte YdFAGEHZCVKGe =  41; 
ushort AdcELPFOOLTicxGjTwTSPoasACfxQ = 27702; 
float DPzRHHJjAQMUKzXfpANKoCjkd = 1.149048E-33F; 
byte hYsRcGnXOEmMKtYDgIbFa =  32; 
short jDaFnNXDABsSucITJhCWB = 29692; 
int kIXbcGJ = 64; 
if(kIXbcGJ == 499808){
kIXbcGJ = 588384;
}long bHKmjbghdahVTGuimXSuG = 28388911244649270; 
ushort nayBoQK = 26516; 
sbyte ATbUK =  77; 
ushort AuDMoekLi = 24461; 
short WxISUVwefsDDLpgWUGjbUY = -16883; 
string DLtoEHqcyhdkiaPuqgg = "abZYoRDjqQYjHmsjcQCLkAnhc"; 
long GwHERfiFBMwaNdhLHjTEWJ = 13437240668548124; 
float EfXOKmgdzIRzIQOHdOHXjgOWRoc = -2.546903E+15F; 
uint NUByI = 36; 
string dQfoSfKLRODMg = "LxqaXukVtWEeEgsgBLNnUubAeL"; 
int tWZsHaPapfw = 17210285; 
while(tWZsHaPapfw == 17210285){
tWZsHaPapfw = 717262;
}ulong DsLnSYTZqwSVmPTCyW = 81603550886467138; 
sbyte MXROq =  -90; 
string JJgtOayGzjiSoGZmApakaUTDSOmtE = "EeMV"; 
float PTKCUMskgoPgYEobhSXOQFB = 7.205505E+19F; 
string uGdf = "NkzHGzpoDJxEY"; 
sbyte aPDXUYfYksLOQ =  47; 
double cjwuOfIxJCJkhhWaHJ = -4.967947E+08; 
if(cjwuOfIxJCJkhhWaHJ == -8.020026E-23){
double nULAGGduuDN = -1.143176E+10;
cjwuOfIxJCJkhhWaHJ = Math.Round(nULAGGduuDN ,1,MidpointRounding.ToEven);
Console.Write(cjwuOfIxJCJkhhWaHJ.ToString());
}uint RSfNwjSkEtXnEywDZgbMFqpmoNoT = 2545; 
sbyte SDefUafJCquJfqBq =  -94; 
float VfNtACTyJVZYpmyxaspJZbsknXI = -4.868464E+35F; 
byte MstjufjGEaDxODtdwJVAYRkVUNCf =  137; 
float AwXmGXdcpRNMIyFFIVUlzd = 2.158928E+37F; 
float zpHEed = 5.196598E-18F; 
string QoN = "sGFJkf"; 
int piUkIJnxIXszANx = 73754428; 
if(piUkIJnxIXszANx == 381159){
piUkIJnxIXszANx += 402700;
}int shOTSzdOPEaazKUwbaHtSXnH = 185022; 
if(shOTSzdOPEaazKUwbaHtSXnH == 528651){
shOTSzdOPEaazKUwbaHtSXnH = shOTSzdOPEaazKUwbaHtSXnH + 75369;
}sbyte NtDc =  100; 
ulong XFjEVANXcSoatP = 39829098799825756; 

}
} public class mLuYme{public void RBlGJP(){
long thRlQemdQnDxg = 16802522340332955; 
uint ASVbOPOfwnbAZyyQ = 1; 
long KLIaHNMTqsMCTHstUc = 51286702700757961; 
ulong oCtW = 6576374806816463; 
string PdCBHtfdVaCtDDDAVXZIEQYPqXQ = "iaIylgteHGduYkGtw"; 
float nmLZybuqZeNi = 5892.648F; 
int VsdhxjzToUFKsjGuAFDpyakxRQt = 882224333; 
while(VsdhxjzToUFKsjGuAFDpyakxRQt == 882224333){
VsdhxjzToUFKsjGuAFDpyakxRQt = 403049;
}short asJXeOSLDxLYQAtJeamyVh = 9186; 
short QzYTBNoczFcRIyCGpIZ = -24907; 
float BBNoRhSQSLsqnoTRcK = -2.977266E+31F; 
short qSyDMSGBIkPlYyflVhjJNqzqjlM = -2538; 
int LVmGjVRnxa = 35; 
while(LVmGjVRnxa == 35){
LVmGjVRnxa += 34907;
}short MRPnMIhZugtAfh = 26241; 
short TZtnjuHMjzuoqPhV = -11902; 
long ByjFykCLVWUcyKIJbXhDyhC = 71605115050451478; 
ulong gScJwVcJKMPoxW = 58507651096288579; 
ushort IwLBzsKseHqSGZHcfTNwNmNQful = 6547; 
int lQzgbakCkqbykPGNdgAV = 31; 
if(lQzgbakCkqbykPGNdgAV == 169546){
lQzgbakCkqbykPGNdgAV = lQzgbakCkqbykPGNdgAV + 407748;
}long wefFMyuBtzYGREIxupbYjspfGL = 6221670507232521; 
sbyte kxLNue =  -31; 
int TdAIhDT = 6772; 
if(TdAIhDT == 772906){
TdAIhDT += 8193;
}double Uzo = 2.722369; 
while(Uzo == -8.263499){
double FoEfYLXMHdneYemKHcskyKSWQDeQH = -49.85617;
Uzo = Math.Round(FoEfYLXMHdneYemKHcskyKSWQDeQH);
Console.Write(Uzo.ToString());
}double hjobMGJKMe = 2.017987E+11; 
hjobMGJKMe = Math.Ceiling(Math.Asin(0.2));
try{
int qGuVKNWwautjipxBfNpRkcYmIC = 2548849;
if(qGuVKNWwautjipxBfNpRkcYmIC == 55573){
qGuVKNWwautjipxBfNpRkcYmIC++;
}else{
qGuVKNWwautjipxBfNpRkcYmIC--;
Console.Write(qGuVKNWwautjipxBfNpRkcYmIC.ToString());
}
}catch(Exception ex){

}int CCgWwTUjiwSdgcHnHhnhFSGUChg = 55; 
if(CCgWwTUjiwSdgcHnHhnhFSGUChg == 552684){
CCgWwTUjiwSdgcHnHhnhFSGUChg += 145334;
}ushort KWbRPHZVunP = 5941; 
string zhfozqJWlwmMSICh = "VMNXsFqABFX"; 
byte MdgEdAhnJdHNKHEcPqtm =  243; 
float mCfEPQYMMLFK = -1.649842E+08F; 
byte cZcmJfjIYNyLnFYKSL =  223; 
string WtyLsVcfWtxCOqnVHKhsyFbtmzhM = "RAMlGfGFCpjgDbtajAmLOkaMKBs"; 
short zUOqk = -26197; 
float oikIpfuXkVDIsdYUbgHza = 2.300993E-32F; 
ulong BcxwQsWApU = 61704446695499955; 
int KMwVUodzxLMdxuQcXCwEGici = 83520472; 
if(KMwVUodzxLMdxuQcXCwEGici == 524878){
KMwVUodzxLMdxuQcXCwEGici = KMwVUodzxLMdxuQcXCwEGici + 841213;
}byte BZEOI =  134; 

}public void BozdGilHIHcJmcbiC(){
long lcpuXEzHE = 73278290303564594; 
sbyte yZxkq =  -116; 
uint ZbzQbex = 4791; 
byte BAgCJiQFImmiNbxmlQRZqzwAFeH =  204; 
double gCKtIGpzObByRVNTLPGSjAIkEbdHW = 509.3568; 
if(gCKtIGpzObByRVNTLPGSjAIkEbdHW == 1.920643E+13){
gCKtIGpzObByRVNTLPGSjAIkEbdHW = Math.Ceiling(Math.Asin(0.2));
object UzQGCKCkABRhObuswOWwz;
UzQGCKCkABRhObuswOWwz = -9.418238E-31;
Console.WriteLine(UzQGCKCkABRhObuswOWwz.ToString().ToLower());
}string xjticMfkZQpbPK = "mojuOHAKiI"; 
uint IURhUgmusBpPoyDpxWPBnOmx = 938919; 
byte WAkCBWzqhJxUCpXNZufqdjnm =  219; 
string UHuGXTyySpPnsYlhN = "UZXQByhRXMNlYJBu"; 
int OdzaeTexkfNsScBMYbFdg = 452233468; 
while(OdzaeTexkfNsScBMYbFdg == 452233468){
OdzaeTexkfNsScBMYbFdg = OdzaeTexkfNsScBMYbFdg + 709411;
}int fOXuBtKcTUeL = 25; 
while(fOXuBtKcTUeL == 25){
fOXuBtKcTUeL += 884999;
}long lKAQCERGsethzeesPXqJRK = 35214841938530619; 
short aVgwogSXCdKPBWlZhJoIDbTCG = -9521; 
short BhjqODg = 4302; 
double dIeugOapGDfdCNMKnZbFQ = 2.090862E+24; 
dIeugOapGDfdCNMKnZbFQ = Math.Pow(double.MinValue, double.MaxValue);
int? INybbgegfeVc = 1373223;
INybbgegfeVc += 21570;float eaG = -6881198F; 
short XjdGqsXPOWazTsYf = -12499; 
ushort HoaTXLhBebbGhe = 17851; 
double ECqnDbsogMbIgUoa = -1.803645E+14; 
if(ECqnDbsogMbIgUoa == 1.559117E-15){
ECqnDbsogMbIgUoa = Math.Floor(-3.119661E+31);

}float aMUYDpoejVLuZbGDGKD = -2.644239E+22F; 
byte WQfOziIfxRJGluuUYtnq =  14; 
short YmfMssyD = 19934; 
ulong XhxDQ = 53262410474122876; 
sbyte QEYCUFGG =  -94; 
uint xpNZnTQgxlyx = 343276; 
ulong bHIRMEgDTF = 80840653412564019; 
uint bpoyGlMZ = 1505; 
ushort epFYAJHT = 11844; 
byte KFWHfK =  38; 
ushort SFlLs = 13490; 
byte RhjLidPjNEQtRHbR =  49; 
int KGHpkOpqBeTHnzgbbbwBBgxtf = 22; 
while(KGHpkOpqBeTHnzgbbbwBBgxtf == 22){
KGHpkOpqBeTHnzgbbbwBBgxtf += 397467;
}uint sCTjRYXTNoeTXCVKFZPPfq = 184896; 
byte TOcfXLsU =  234; 
double RjcyIOSqAh = 5.761017E-32; 
while(RjcyIOSqAh == 16689.31){
RjcyIOSqAh = Math.Ceiling(Math.Tan(1));
int? uBcGYxOPMqyDL = 8597518;
uBcGYxOPMqyDL += 76090;
}
}public void uiAmShFaAshXpTPMZSTmOcGwkV(){
ulong VEpHHkmewMMNXXPAaDHgqygoZBfs = 11936963286588470; 
string qeHLqngfoQmguJnIG = "xaHffVCqKQtoSTy"; 
uint DBHfSiTlo = 43; 
long IMcHAaUkYMiZ = 29242996921019199; 
ushort gZsESNTQQeIqhHlltxFaFP = 29058; 
sbyte RglqMNOWWG =  -111; 
short igqhVesmigONalpxhsDlobHIAds = 24761; 
ushort bhXynyJxT = 23780; 
ulong Iay = 68276530210275734; 
string gBKISMzSXeGPDYSOLAjy = "tHjVsRxVy"; 
ulong VRgXZObHpjLVSPiDFMdO = 49440777466916197; 
double XApY = 1.098512E+17; 
XApY = Math.Ceiling(Math.Sinh(-5));
float RyfzjMChmmKBUhnTquldczeojsi = -1.670631E+25F; 
long wWCyIeKy = 62266532523435113; 
sbyte YYDJPSts =  76; 
short DmqFcoFtKGPIXIXZYlKbKceSJ = 10343; 
long KeoowGb = 72350717949893920; 
int qGbRSug = 24; 
while(qGbRSug == 24){
qGbRSug = 466530;
}double DxKGpBlqHencx = -3.593989E-05; 
while(DxKGpBlqHencx == 2.141839){
DxKGpBlqHencx = Math.Pow(2, 2.1);
 bool? PNTIPEsSeSVgqcUeuacbEoOQ = new bool?();
PNTIPEsSeSVgqcUeuacbEoOQ = true;
}string xdVcOVgsbtosEoJ = "BYcwPoD"; 
uint GDJWKjxqCGYIlaUe = 22782848; 
ushort mJc = 51561; 
byte MHIJgBtDitzwsjTbcKLCnYhphd =  33; 
uint WBULRyQJT = 447715; 
short SCHmUkhlHtdKFNzqCduSo = -17760; 
float KBMdsijnmFgHKUjVtb = -3.909746E-29F; 
float mLq = -1.450961E-13F; 
ushort xuGJ = 3571; 
int zeUyiPyZHRIH = 465945931; 
while(zeUyiPyZHRIH == 465945931){
zeUyiPyZHRIH += 730980;
}ulong nbMaRPkQkBYHsRxnhSYbD = 27317381012870648; 
sbyte WoTeytTGuFIOgTaocbkgd =  67; 
ulong aYlpgl = 64357917846506035; 
double nmExVzXHJxAEMYHiDMeAhDAhZOJ = -2.968699E+25; 
nmExVzXHJxAEMYHiDMeAhDAhZOJ = Math.Exp(2);
int[] iCxfQYmBDzDVOwYICgYV = { 8773653 , 9199 } ;
Random JCChdO= new Random();
Console.WriteLine(iCxfQYmBDzDVOwYICgYV[JCChdO.Next(0,2)]);ulong QIdEAKauUCmf = 79616770072768990; 
uint eFAaVXNjtwQfNnLSzYBMdtPEA = 3640; 

}public void OwGyS(){
double AeAPdILiRoWzBniaBN = 1.051255E+19; 
AeAPdILiRoWzBniaBN = Math.Ceiling(Math.Atan(-5));
double ZfeaXsThbMksNwtSsSDaAMfyHpoAj = 114.8485; 
double UneELgHlXlRJBJcY = -201952.1;
ZfeaXsThbMksNwtSsSDaAMfyHpoAj = Math.Floor(UneELgHlXlRJBJcY);
int lWEgbfHLObDmaRZnbMzItHjAdlW = 70242398; 
while(lWEgbfHLObDmaRZnbMzItHjAdlW == 70242398){
lWEgbfHLObDmaRZnbMzItHjAdlW += 386028;
}string zPWHwQgYcqPDQeZiYgY = "FNedPiSaLQlTXlCAe"; 
sbyte RlJg =  -90; 
short fKLUnkwCaMKTRlXgnmfVG = 5869; 
int eSdgpWGWPfcipQDa = 414916; 
while(eSdgpWGWPfcipQDa == 414916){
eSdgpWGWPfcipQDa = eSdgpWGWPfcipQDa + 236277;
}short UeuUbseCfjQwgnkefOdLjPRzK = -26016; 
uint tPMSEmml = 715145; 
float bCcfZhTjkgKaiE = 6.626751E-33F; 
byte kWVUhqyYRUaZDuTIKJNLzDlnZme =  181; 
long GiX = 45658524924546472; 
string ZikEohBTYsD = "ILHVUlNoRnRbWU"; 
float Doaczm = -2.553226E-32F; 
ulong QEigIRwiSnUkzAVMzLqiH = 12109153567748658; 
ushort DgkwjfLHaHObIlq = 53535; 
uint MOCNFc = 309849; 
ulong YAWjxPsendMGNeqDTsGhZTQs = 83001797742925146; 
short JBKEGSUmDxwgaywbjcLGQfKEXPT = 3291; 
uint dSnSxR = 288917192; 
ulong nMptWfHAtBXXiMeDRgHQZqL = 23109673313055078; 
float WsePpIRhtib = -6.686571E-29F; 
long NTZMJWPWaUatmzzOp = 26261284869445628; 
ulong GiJmlTpwhGymthwdVonEJ = 53294086369338393; 
sbyte mTniOqHPAWdpiTQ =  -57; 
string WSlklZLOXgNxhIcgjtfZHMnAOHGGl = "qktjRmmcInzatAcdPxmcJxies"; 
byte MxFufCfQCdXQQTgMRC =  156; 
short yLSjsiiUbnSIJBHcTtznKBaS = 6593; 
ushort CKsEMRP = 19096; 
long kNsWVBPeigtBKXhgeeWLaFV = 76796971855098083; 
byte pJGgqOakNxqQbauwFNKtbNGSspL =  206; 
sbyte tmhGtoCaNqVAmiFkGkpnCdjSUEcO =  69; 
string kfPc = "JwHeU"; 
int tyCAynhKABRxodoMQlOlMnLtzfBCj = 216957; 
while(tyCAynhKABRxodoMQlOlMnLtzfBCj == 216957){
tyCAynhKABRxodoMQlOlMnLtzfBCj = tyCAynhKABRxodoMQlOlMnLtzfBCj + 810490;
}ushort dFBHqELotOau = 44854; 

}public void SgsnfNhbsNGBWoxzkmUuMImGu(){
long efq = 56078864906956911; 
long jcmNaRIwSUDOApekTMUw = 14570916322433710; 
double TuDmjKw = -1625679; 
while(TuDmjKw == -1887425){
double JkJhuTWnUfglADRzAKbhkmf = -5.867048E-23;
TuDmjKw = JkJhuTWnUfglADRzAKbhkmf / 3;
Console.ReadKey();
}double mxZWNbYMWcKXVbTHueFPeq = 1.179657E+07; 
if(mxZWNbYMWcKXVbTHueFPeq == -9.852166E+14){
double StHQfjdoKGt = -1.631323E-05;
mxZWNbYMWcKXVbTHueFPeq = StHQfjdoKGt / 3;
try{
int BcwKGJbfIWVuUtqNohQ = 7943481;
if(BcwKGJbfIWVuUtqNohQ == 17389){
BcwKGJbfIWVuUtqNohQ++;
}else{
BcwKGJbfIWVuUtqNohQ--;
Console.Write(BcwKGJbfIWVuUtqNohQ.ToString());
}
}catch(Exception ex){

}
}double TeyqeZkRImoT = -9.837857E+32; 
TeyqeZkRImoT = Math.Ceiling(Math.Asin(0.2));
 bool? pcJJeNzJXCyfeIVBeFhbEG = new bool?();
pcJJeNzJXCyfeIVBeFhbEG = true;float uuUteRWAROpeVMJgLO = 4.166354E-25F; 
double JPkAqGQHISiaNHKljfDIebEHg = -1.446656E+25; 
if(JPkAqGQHISiaNHKljfDIebEHg == -0.08785495){
JPkAqGQHISiaNHKljfDIebEHg = Math.Ceiling(Math.Tan(1));
int? FsVOIpiXyt = 1037689;
FsVOIpiXyt += 71171;
}byte PLSWmepReTEnFE =  5; 
double BzCOfSuYcxqSLfTgzYXymKtctRLB = -3.364355E-38; 
while(BzCOfSuYcxqSLfTgzYXymKtctRLB == 1.189475E+36){
double IKSzXkJYYtNKhfFdzLQezw = 7979168;
BzCOfSuYcxqSLfTgzYXymKtctRLB = Math.Round(IKSzXkJYYtNKhfFdzLQezw ,1,MidpointRounding.ToEven);
try{
int bOuUNCHxRofYoALduzYQi = 4001414;
if(bOuUNCHxRofYoALduzYQi == 2821){
bOuUNCHxRofYoALduzYQi++;
}else{
bOuUNCHxRofYoALduzYQi--;
}
}catch(Exception ex){

}
}ulong cAgMBtz = 74838115628278095; 
short AoSMNzHMWaTyNQkFDDQbidLhz = 5486; 
uint xEguZEVMYypaBZFtFpMwtp = 829029414; 
byte IwEKmiYVps =  225; 
uint iVzHflPCiAsDiSe = 1258; 
ulong jLWAinjhbF = 64213895313933085; 
long FwOzVkhepUEjGFnNmwptVe = 31095604715719542; 
int yZLPWyEXffRhaBG = 856965057; 
while(yZLPWyEXffRhaBG == 856965057){
yZLPWyEXffRhaBG += 572618;
}short AIRZJsMOaqgMHUzQQRtmQnGWZ = 8968; 
double kazTgikhfzVFuqRRUO = 1.691586E+38; 
while(kazTgikhfzVFuqRRUO == -0.05221068){
kazTgikhfzVFuqRRUO = Math.Pow(2, 2.1);
for( ; ;) {
Console.WriteLine(-1.844133E-27);
}
}long ATaCGLGRx = 86612608351062945; 
float jTsKBTngsocHoGRensUG = 1.219811E-25F; 
ushort LpKRpeTqLFCthi = 33876; 
long FmCtxPdpmfys = 1418770085971243; 
ushort nwLVKdyWeSlnjVUdoBHBdYxY = 31491; 
ulong WljpAVPZgIBeaSpbG = 87808187416643026; 
uint MoZMxYDCLUCNswWOwOwHIOXRTBc = 372672; 
ushort HXZcKgWdgyqGgetG = 16947; 
sbyte yWnVnYsiTWVfipuUQstlHAKo =  -84; 
short QMBUPnF = 28913; 
int bMdqXchlsioM = 55; 
while(bMdqXchlsioM == 55){
bMdqXchlsioM = bMdqXchlsioM + 151305;
}double ZMWQDxtbf = 730474.6; 
while(ZMWQDxtbf == 329.5256){
double JxCdRiTdDXHVqxYstoPnPCJgfAF = 9.444594E+09;
ZMWQDxtbf = Math.Floor(JxCdRiTdDXHVqxYstoPnPCJgfAF);
int? HGczCKWVNWzTxukChuziARbf = 1678861;
HGczCKWVNWzTxukChuziARbf += 1111;
}short YyIyucsqIm = 16486; 
float yBxAoPFXqPoYEiYpZmtVmjXRLePJP = 0.002596823F; 
long thyM = 18726847291603768; 
uint TtaJZtaMnEGOkaVhRoIWkbCEE = 1588; 

}
} public class VDoAmnV{public void ORBncHUTgHJ(){
byte iQtKDGSSxeGlQQLngCWeGuZPu =  222; 
int IyXMXt = 73182100; 
if(IyXMXt == 691823){
IyXMXt = IyXMXt + 662823;
}string qNUclRiGZfMfkPR = "pKcLkZgUnyeZssRnbhc"; 
byte qINYbyRZShXVpzaGtmsGaPOMjnI =  70; 
uint cMFLZ = 30; 
long CpUwoge = 5528508110230022; 
float TaPOPECOfkpGAYqHd = 4.650034E-15F; 
sbyte AFflOyQwgbYQ =  -18; 
sbyte WwMMsIjVOw =  -27; 
float bSTpOjuwKCa = -4.461054E+17F; 
short jqPcPbLJeEOLoJqRuFztohJbmQTmT = 13746; 
int okMyGsdoqjOMazeNtLH = 255938; 
if(okMyGsdoqjOMazeNtLH == 49392){
okMyGsdoqjOMazeNtLH = okMyGsdoqjOMazeNtLH + 652013;
}ulong ZiyPuXhxdXUkbBjcetXbPLgU = 89458810446995540; 
float egTwuOTbaPzBGadXF = 1.634487E+15F; 
ulong ncE = 4974565398470800; 
short xciKYihNLqwJzeo = -10041; 
double egDQeoQdWeUzBuhgEbOVLCEmELiWS = 0.5090848; 
egDQeoQdWeUzBuhgEbOVLCEmELiWS = Math.Sqrt(4);
float lXoDGzgafj = -1.313951E+08F; 
double DSuKbDFAhF = 1.57177E-05; 
if(DSuKbDFAhF == 10290.46){
DSuKbDFAhF = Math.Pow(5, 2);

}sbyte koBbzBbpdHNiqLDVBuh =  -112; 
string pcbbXYOdRbt = "mDVWmuPouYfuzdOqaUUfCTYzZx"; 
int DaQL = 465416291; 
while(DaQL == 465416291){
DaQL = DaQL + 586527;
}sbyte oPXqXIEUkydWgmqtFxWQRMN =  -30; 
sbyte FZGOqxMgRNWHC =  -40; 
float SfWTFDgewEIsWDcDiDcEo = -1.600612E-30F; 
long axFyXVFX = 36416593654595094; 
uint NOtkoKtzYoou = 982837; 
short EgDIzIdwOJwfTNaLOetT = 21571; 
ulong UdzXoI = 36754598804685683; 
float eLYyGonFknEGqGyROZIEZLONZj = 8.223984E+34F; 
byte fwVjnhSsCGPTMbehh =  9; 
long yWInUuXlyxDpsKiiKgsoYwzc = 89850631846720549; 
byte diNHpUjHVedACbS =  233; 
ushort KwQ = 2962; 
uint AOukEFwRdUHaxdRzesA = 9893; 

}public void FXUNuabRIaK(){
sbyte DAZdqK =  -70; 
ushort wSAujkpzDeGjixpQjuQl = 16370; 
int oVoYfniKOTX = 53215; 
while(oVoYfniKOTX == 53215){
oVoYfniKOTX += 816137;
}uint zFBXtdK = 1260; 
short juPCjzVlkjtwSp = -18758; 
float EjDZLdhLP = 7.622147E+31F; 
double bcuqKEOCdCVFpZ = -6.726942E-25; 
while(bcuqKEOCdCVFpZ == 1.710693E-27){
bcuqKEOCdCVFpZ = Math.Ceiling(Math.Tanh(0.1));
 bool? qfHVjnfLIbWnBROhgZBdKjnfbflGn = new bool?();
qfHVjnfLIbWnBROhgZBdKjnfbflGn = true;
}long UmouZq = 81343083770926138; 
string xtSLKMZDOCDhdJqH = "UlYpUoRZWAKtT"; 
ulong sAFAOayinMGz = 33364392160891046; 
float jCdb = 1.626072E+19F; 
byte iVPjPFdSSuy =  15; 
long oRcTsRTiMMBoodwNoK = 29570544098462806; 
int sMDQsGmmZSuwxaicKBoayffNgMbE = 44; 
if(sMDQsGmmZSuwxaicKBoayffNgMbE == 256924){
sMDQsGmmZSuwxaicKBoayffNgMbE = 925572;
}ulong jKWKQiOypCDDDGVQKEBmHQJGFJeDo = 18824127243285387; 
int osJyGjzb = 529315; 
if(osJyGjzb == 684182){
osJyGjzb = osJyGjzb + 420907;
}int ubqfgekhotcst = 92422043; 
while(ubqfgekhotcst == 92422043){
ubqfgekhotcst += 219099;
}double scwfICaCqydIPzXmAckgBilhgIQG = 4113997; 
if(scwfICaCqydIPzXmAckgBilhgIQG == -1.639326E+11){
double azDkHXVYKEmYZFqaPyjRnYaLwaM = -1.027835E-36;
azDkHXVYKEmYZFqaPyjRnYaLwaM = Math.Sqrt(3);
scwfICaCqydIPzXmAckgBilhgIQG = azDkHXVYKEmYZFqaPyjRnYaLwaM;
int[] yZqAEIVUkpjxdE = { 6607743 , 8819 } ;
Random BaZjTYShIbzSPGlOeKjQY= new Random();
Console.WriteLine(yZqAEIVUkpjxdE[BaZjTYShIbzSPGlOeKjQY.Next(0,2)]);
}ulong TEYBdCWhOALi = 50561899203025984; 
ulong nZDfMU = 28026769524415222; 
float EADLbgBFuEOkhYHIDNpY = 6.122913E+35F; 
ulong ufHO = 57495929555847475; 
int fhzXL = 11; 
if(fhzXL == 885361){
fhzXL = 309873;
}int meUJGLBpVA = 66338024; 
if(meUJGLBpVA == 761434){
meUJGLBpVA = 492770;
}double IeyIhBamHXTFNzqFUVRBAnbQjI = 1.078425E-16; 
if(IeyIhBamHXTFNzqFUVRBAnbQjI == 4.567361E-13){
IeyIhBamHXTFNzqFUVRBAnbQjI = Math.Pow(double.MinValue, double.MaxValue);
Console.ReadLine();
}byte YGpazBim =  219; 
byte ljEnGFoNTIfyTdCtGg =  138; 
ulong TYBOCVaxcXHhDxeTUDSskuDThy = 67811697693665705; 
short dBdJnpQK = -2336; 
short IxALEKyJAxI = 7525; 
string dnAQpLSdTQtAjeUETuShnAJSEHZcT = "wUZKjbqThFyEEJqeAtOVEmOW"; 
long FHmKMPkeuzBBUSmZmm = 3296499239193662; 
short ligcZGxKHyTItznTFZzVIEaCmaIFg = 17381; 
uint BmSls = 522380; 
short DsPubqzMIVhhtp = 9946; 

}public void FQYXSET(){
double hgGbBgzjAfkkhkKjSzTuXgW = -3.620088E-33; 
double TGUjSLhOKYUkxXyGo = -180981.7;
TGUjSLhOKYUkxXyGo = Math.Sqrt(3);
hgGbBgzjAfkkhkKjSzTuXgW = TGUjSLhOKYUkxXyGo;
sbyte eLGWoQxSOVRSNzHRyyJosmlZMxBVw =  110; 
ulong SnIdBtNygMfFCXgBQVWReQHSAHjS = 55496613129241965; 
float peLliGhHyPoZfAaZSb = 4.213168E+22F; 
int iUhGomAIXIKlGSgSIaLSjwEpwX = 102397755; 
while(iUhGomAIXIKlGSgSIaLSjwEpwX == 102397755){
iUhGomAIXIKlGSgSIaLSjwEpwX = iUhGomAIXIKlGSgSIaLSjwEpwX + 31076;
}double VQeDqUgPSHpTTi = 1.26372E+38; 
while(VQeDqUgPSHpTTi == 4.802571E-32){
double SkqwuGZQIljWp = 4.017326E+15;
VQeDqUgPSHpTTi = SkqwuGZQIljWp / 3;
int? LuEXZCdJlbEkWnLgAmsazNjSQ = 9635380;
LuEXZCdJlbEkWnLgAmsazNjSQ += 48018;
}ulong BNJsYjLgqUuyJPonInuVfuKNM = 11490593583140789; 
ulong CnWbgBmCOSoAPfqcKeULfd = 58985451361662999; 
byte WdO =  4; 
long mwTTWVyeQ = 35706778303066; 
uint pHClphDJmDLq = 3314; 
uint zSmhaipPWbAmdAWypJwFMiDLt = 1948; 
uint ZFDBn = 17; 
ushort qhqMpnIpFOH = 16933; 
string VTfopCOPYOKWyWMQwNe = "snnZTnddHxVRYXHIhOTSDCjEctRME"; 
ulong UibnIGgxbzKh = 11342827842388567; 
long lyQdLWAyqBSdbJXJYcbqRuTuHK = 56057989049768176; 
double zEUjWFgiozSjaRpbGp = -1.118156E+27; 
while(zEUjWFgiozSjaRpbGp == -4.87609E-06){
zEUjWFgiozSjaRpbGp = Math.Pow(double.NegativeInfinity, 2);
Console.Write(zEUjWFgiozSjaRpbGp.ToString());
}int qnhMJGPDjZeIuinnwDPbCBebEPU = 5923; 
if(qnhMJGPDjZeIuinnwDPbCBebEPU == 148518){
qnhMJGPDjZeIuinnwDPbCBebEPU = 479277;
}sbyte TMLjSyHwfMPlyIGstaEtRj =  -99; 
sbyte LFXwBWioxuCKsWEUfZpODyDZwf =  -95; 
float UKGDphTJZGQHXandIaeazL = -3.639754E+23F; 
ushort nbYZuQnDDKW = 8971; 
string LkfuBgGxnVNfbVnjAQhZTRLZ = "ZswzEhCY"; 
short mFQpnMSOcl = -14125; 
ulong ntkfZdVUBgiBP = 77970238097564785; 
string JAUnbdINWlZgftbCUw = "NzIPACUcoahcN"; 
byte CHeQURStmnPcSqMKmpknfbLw =  186; 
sbyte tNP =  -82; 
byte qSwyeCFUewBKzbLDpUMDaJAVKd =  187; 
ulong HyeEjCAqmhPHtbH = 34862200297300653; 
ushort TIsANbPOwyXxfoBlkcNXV = 41236; 
long MYQHBIjEIUqThSBgZSXEKNGYGd = 80480981707357164; 
ulong kimOntOlXIdtCHYdJejYtSa = 52089991224690766; 
long MVQFj = 60694371038809876; 

}public void BCKEWoOwthQNAWHeTQwoQMhzAP(){
float ZRdyPVWHxCVupqDJPyPtsohhFYcPt = -8.335182E-34F; 
sbyte OwCesXFaZIqUaY =  -5; 
sbyte JcaAILKVSRShmkzP =  119; 
uint aExAkttHQnEZEGUDafVgLRjAgMT = 44341739; 
float cxEiV = -130304.4F; 
ulong odUHoR = 24319725573660727; 
byte uWszAGj =  236; 
sbyte DZxYoypPSIXDkpxqu =  72; 
ushort ZbtZCqkqHJTYyO = 18360; 
short tsZtn = 3164; 
int SLcbilwAO = 512876; 
if(SLcbilwAO == 447932){
SLcbilwAO = 975376;
}byte hjOZeybhlaxxcMAQOKGiozyzzKlOI =  166; 
double OuftPmOnbUWkZ = -7.834073E+15; 
OuftPmOnbUWkZ = Math.Ceiling(Math.Acos(0));
Console.ReadLine();float uOBIGV = -7.878733E+34F; 
double BDPDpuak = -3382.523; 
if(BDPDpuak == 3.095874E-18){
double aQxofIGYnmNMl = 1.074903E-33;
BDPDpuak = aQxofIGYnmNMl * 2;

}ulong uZzEIDFkkMtEMSsELCJbo = 12264794066867466; 
ushort koHVjUfMXWDuxEdhx = 14420; 
uint PDKuSTVsUo = 9501; 
ulong hnfXhKGP = 80516954024416572; 
float OiMJF = -6.147665E-36F; 
short OSIIGHlwPcAdPKaQFILkWjpzkiFAA = 11905; 
float RLSXdygBxEsDsaZALSZFqfbECjx = -6.635616E-36F; 
ulong fjEmdSdNgBkOtSOfkieOHKnFLKnw = 73648733529491570; 
ulong WIIfTdgqJMOdkmXZpxWZDuE = 85557659817443786; 
ushort zmyfBRjztbD = 41421; 
ulong yBKqQxQjCdamNntmWftNMZAUP = 31672159044404530; 
short muwPZCnLtend = 27828; 
string fxndey = "QDj"; 
long ajdZkzUgSB = 18827240871958148; 
uint xdmFNZkVlFTFomnCpfcGbXnF = 86; 
float iUbaITSXWzllmYUnIMe = 2.481824E-23F; 
ulong JzWPzPJzUgFMsDSWwupGRTQs = 18747706888386580; 
ushort tCMeu = 10109; 
byte seJ =  60; 
int PPJEUgYPJFMwPMefmipnjq = 3256; 
if(PPJEUgYPJFMwPMefmipnjq == 357534){
PPJEUgYPJFMwPMefmipnjq += 828300;
}
}public void LKRBohFfoCpfEThtZ(){
ushort VmSAWoBzEsFMOhnXV = 58933; 
float EAZXIZiHydXaPRVn = 6.170292E-25F; 
float OKbNLPDX = -2.631247E+32F; 
byte eTYwmUSXgUaBuEW =  240; 
byte dLaKMTzmlSykqCRAWWTBbYqXeUJBs =  38; 
float LpSCzwheWaIaAVEAJVH = -1.215113E-32F; 
ulong eudFRkXUeZSAbJpbCPPEaqpyjqiSy = 7647156313750410; 
float TZfEbIwCRtU = -6.560413E-22F; 
short XOdHuBb = -25219; 
ushort gqBLUiHLzVFVNoPPPs = 30455; 
int HlRtZdOxPSTIZ = 322064332; 
if(HlRtZdOxPSTIZ == 936511){
HlRtZdOxPSTIZ = HlRtZdOxPSTIZ + 212789;
}byte NuFcbPRGMKIVi =  125; 
sbyte qUfTnKdLmNtzTXwmoZURm =  -46; 
double WRdikNQwkl = 5.334845E-38; 
if(WRdikNQwkl == -5.333031E-30){
WRdikNQwkl = Math.Exp(2);
try{

}catch(Exception ex){

}
}string VmMqPOdzkYs = "paJtphpTXmSx"; 
byte dZmtwzaZZDwp =  5; 
sbyte bVqfphxKVboPksIwogxLdl =  84; 
int spbfYZgghWqCpUkIpqsfKQIqhf = 59; 
while(spbfYZgghWqCpUkIpqsfKQIqhf == 59){
spbfYZgghWqCpUkIpqsfKQIqhf = 83763;
}ulong XSuEXbiId = 33807204760914388; 
float xjDILYSQCJAgmNdX = -5.822159E-20F; 
ulong pXbMlZngYuxUV = 73303615208633817; 
byte ZFoegVnSPbAjAhRwLKLS =  38; 
byte lIxbDiEgodpbhXVhlKC =  159; 
float wRUEOZVtuANZdx = 1.282221E+29F; 
long lVkfjgJzPDAhGGMQmFgYu = 7729724699332799; 
uint sYTuXkoIOUpjtUl = 141318383; 
byte jfNNoJKny =  92; 
float EpehjNlPFXWsedtS = -2.779698E-05F; 
double WIoqFl = 9.329668E+16; 
if(WIoqFl == 5.407336E+26){
WIoqFl = Math.Ceiling(Math.Cos(2));
Console.Write(WIoqFl.ToString());
}uint dYxWZgDZ = 889918730; 
ulong WGFzgXYHtw = 55178400306754860; 
uint dgpSPCZWFkFAJhPloNJ = 904393416; 
float JiuRcTOPeuhjQpMKGXbbhRptY = -9.91545E-37F; 
uint kGMjoIKuKXcRwB = 50; 
double WDddWTSJSSaTTSotTPxaBJboVZYkN = -1.225489E-21; 
WDddWTSJSSaTTSotTPxaBJboVZYkN = Math.Ceiling(Math.Acos(0));
object HpdWz;
HpdWz = -1.772468E-22;
}
} 
public class fOzsKgZsEpNOSgBdkZNYFJh{public void CseZsFZNtWdpCwYtSkJZYeDHPeWN(){
sbyte ixGBRqbZaYpZwVdjDQatgtZiY =  -20; 
ulong jmJmICOHKaEIsSk = 5133629965401326; 
sbyte MLPxyDwBEPNodsThxADlsb =  -58; 
sbyte AaMjUFLEBV =  84; 
double mzihPcyw = 3.145052E+09; 
mzihPcyw = Math.Truncate(mzihPcyw);
Console.WriteLine(mzihPcyw.ToString());byte ndiOGVNpdylHQjYhDufunkUifuTG =  205; 
float MkxPlOMlkefGEZiBLUOnPtQIciZJ = -8.622523E-37F; 
sbyte kABxtuNtRhUpiFpj =  109; 
float LbEisuP = 1.183816E-35F; 
ulong ajXF = 58555011069545129; 
ushort zBzyTQMsTxzIphbFILGN = 8873; 
long EklLiMRDIgDT = 12920783905283142; 
uint NOqMDK = 781025; 
long OhVmHfDGbUQBNngo = 14107121892254306; 
ushort uyihfTxcfMwcDTO = 3681; 
short placFmqWgnbtpyPiLfx = 24493; 
ulong dLiwUoYOIwflgkxqSWQiMiaMxXmt = 73596059230742608; 
float tcYsGHn = -4.708044E-14F; 
int umBcGkVKxZXbFHZPKuOcgxJ = 775655; 
while(umBcGkVKxZXbFHZPKuOcgxJ == 775655){
umBcGkVKxZXbFHZPKuOcgxJ = 391936;
}uint GJHNwfskgmHz = 42; 
string MGRELDCHYEYn = "SqoYIqtYUFKqiH"; 
ulong YAgOSIXDUHdhaPRh = 46605470255087574; 
long RbhmCVMgaeRBE = 9162871341017123; 
sbyte CmssUiyWbgyQWuIfhwUPNpE =  -68; 
sbyte FxDRwxMoGZiESXDiP =  -31; 
sbyte zsxVfQeqalKgm =  -38; 
int tkL = 9; 
if(tkL == 658360){
tkL = tkL + 931047;
}int WlFCjgMdlaIMGqwoup = 33; 
while(WlFCjgMdlaIMGqwoup == 33){
WlFCjgMdlaIMGqwoup = 783968;
}int eoREOYBmhewLAaBosaMzf = 67685061; 
while(eoREOYBmhewLAaBosaMzf == 67685061){
eoREOYBmhewLAaBosaMzf = 876963;
}byte FLPWB =  7; 
ushort JlJktjqTK = 52875; 
string kZBcfEwGzjwemiEuHOYYecKO = "nMAs"; 
uint DRyTFjNRXHROARFXjHWmW = 82708964; 
long pNaSRV = 73922422687818358; 
byte XbsnokXzpDTxmOLNMMoyNYfLEQ =  0; 

}public void phUIMtZQMzTfPCCocRLLhCeEKNgY(){
float KAkZhiIkKUPVamTjIkxR = 1.908168E+17F; 
string QPcqYZlaUf = "uUPutIShRVlCRLBeVCeYYuV"; 
sbyte fplLIPaVAzoS =  81; 
string RFntFnl = "yUpAQgPZZHbmqGyigkH"; 
short lmezVhugC = 5987; 
sbyte KPRqpkOLAolPQRkgemtIUulHkaV =  124; 
ushort zbWtIQGRRaSHNWapJdWmbolyap = 36658; 
byte ooOoHR =  50; 
float juCmwsPmIoHVsR = -5.680324E-27F; 
float QAtsnxM = 1.296754E+16F; 
string gfQykUCMPTTJ = "JnDkWzLgPIuteSgeCTHLVBj"; 
ulong gXCxbpWQGJZTCGn = 41276485598469269; 
ushort namYRheNNQBjLNsyEzih = 13834; 
string tqMjMuuZVg = "OoKSPCVgOQNiAEfYBJTmxmNU"; 
int YsttQ = 54169; 
if(YsttQ == 51169){
YsttQ = YsttQ + 774464;
}long ImelyGhlGHwTqcUsBDQkK = 27815166444419346; 
int jPZl = 34; 
while(jPZl == 34){
jPZl += 479182;
}float MXAzCiEzBYBa = -4.732224E+14F; 
short zybJaEkesRTaAnCMQkRQVBHXa = -24054; 
int nZhpbkRQ = 190476; 
while(nZhpbkRQ == 190476){
nZhpbkRQ += 414879;
}sbyte oDYmxPjTTBVXRdJUbZQtK =  13; 
double xMnHSJeBuW = 7.796352E-05; 
if(xMnHSJeBuW == -3.59408E+21){
double CpTYtBzgMGNaMEKQ = -2.019169E+21;
xMnHSJeBuW = Math.Round(CpTYtBzgMGNaMEKQ ,1,MidpointRounding.ToEven);
object jwwEV;
jwwEV = 13.13079;
}float VWUzBeXlmnfVpdXoNwBn = -1332773F; 
long MlXGyH = 57678246766131072; 
byte uqjRqc =  67; 
long QIgRHyqoOMngsPHGdwtdzXAkuTSu = 35653170541944317; 
uint TAIkECWZ = 55; 
int nIUHATiV = 762775; 
if(nIUHATiV == 479768){
nIUHATiV = nIUHATiV + 990161;
}string UKhIeSDUwmu = "ouRoRFBcdGiOQGOfTMMEBGfswVgx"; 
sbyte lmA =  -71; 
int WpeU = 992179; 
while(WpeU == 992179){
WpeU += 381514;
}float eyaKBQyJnAUWyeXd = -2.77344E+32F; 
short YLiDHReBuSSbdIsIZD = 14971; 
long YPyxiywciTEenBtnycJLWnxH = 43084788036489300; 
int aVMfyuKzlYnWQgYuCAcYSKsQ = 320420; 
if(aVMfyuKzlYnWQgYuCAcYSKsQ == 747395){
aVMfyuKzlYnWQgYuCAcYSKsQ = aVMfyuKzlYnWQgYuCAcYSKsQ + 80906;
}
}public void tAz(){
uint CqGoazcztWcJOVD = 1517; 
long pEWDgObLgXUysIBSjTOXtHUyInXU = 52325460010700435; 
int zBZNVBzAKbmdsAuKFKJzVSQg = 610734164; 
while(zBZNVBzAKbmdsAuKFKJzVSQg == 610734164){
zBZNVBzAKbmdsAuKFKJzVSQg = zBZNVBzAKbmdsAuKFKJzVSQg + 846628;
}uint lwumYGLtYiFVHNihShV = 215430; 
sbyte HKSTplPTSEISXfgLYYpAMnUAgyh =  84; 
ushort YudBOEfNwhjoMsZVkcLyXBVIhsPt = 48672; 
float YwVFm = 0.0001335702F; 
short FkEhpyCDAklffaYzAx = -9629; 
ushort NaCdkTQONgNLhTMXNqLRmTC = 41697; 
double wXgNseWQRkbdeELPNezwYglBM = -1.10434E+11; 
while(wXgNseWQRkbdeELPNezwYglBM == 2.337765E+27){
wXgNseWQRkbdeELPNezwYglBM = Math.Pow(double.NegativeInfinity, 2);
Console.Write(wXgNseWQRkbdeELPNezwYglBM.ToString());
}uint xcdPPKBb = 3147; 
ushort sWoiZa = 62471; 
ushort TmkKuymXKgPNGkDbVQttAlMuVtFj = 65207; 
sbyte aim =  14; 
ulong fummdEjFuNMMhdjFazHCQOaScX = 55841555112826008; 
ulong fgwICQBHTzSeQx = 44904129056399284; 
byte ddQQwd =  247; 
double PzUdZZanuY = 6.902883E-32; 
while(PzUdZZanuY == -4.645855E-14){
double fzbLWnFzZLkQBTRbY = Math.Log(1000, 10);
PzUdZZanuY = fzbLWnFzZLkQBTRbY;
int[] bdZyFbIOAtT = { 854850 , 23254 } ;
Random ATQNOXPuCbDjh= new Random();
Console.WriteLine(bdZyFbIOAtT[ATQNOXPuCbDjh.Next(0,2)]);
}int tiRPkdubn = 2424; 
if(tiRPkdubn == 491584){
tiRPkdubn = tiRPkdubn + 345511;
}short aZaokpWDNaCQoBP = -28701; 
uint jcDnQx = 50206777; 
int zRDkRJdmFYiTUmydS = 9767; 
if(zRDkRJdmFYiTUmydS == 872022){
zRDkRJdmFYiTUmydS = 174135;
}ushort oWNiEEK = 44540; 
ushort WzqYG = 29271; 
int DYpioRNEBCPfFIi = 53; 
if(DYpioRNEBCPfFIi == 63994){
DYpioRNEBCPfFIi += 3961;
}ushort jAiCGClttUuHwSg = 41809; 
double aciUoAnonxTgAz = -0.3677627; 
if(aciUoAnonxTgAz == 6.969797E-29){
aciUoAnonxTgAz = Math.Pow(double.MinValue, double.MaxValue);
try{
Console.WriteLine(aciUoAnonxTgAz.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}ulong IiQnCSzWGOqbEnS = 8278411055641672; 
uint MLWZbFzjqtombL = 60856299; 
sbyte XCykWX =  -39; 
sbyte aFL =  97; 
long RQhCXEjTRm = 40274823705773309; 
int aZBPZIWEyFGAIYjVBd = 57; 
if(aZBPZIWEyFGAIYjVBd == 625240){
aZBPZIWEyFGAIYjVBd = 971575;
}string ANMTbcndnSptD = "HGwlHwUUhuH"; 
uint bPssCNFhiSFEEkFL = 6074; 

}public void CSWhmu(){
short BFGqlCDmnWAOGXhzORJ = -29669; 
byte jRPAYIeHeYj =  32; 
string EBsbbYpORSCELzuwLp = "kwODMEUhHRCEAT"; 
int mAHakZwZXYuWHmKmUKTiQYGzgRJ = 305264242; 
while(mAHakZwZXYuWHmKmUKTiQYGzgRJ == 305264242){
mAHakZwZXYuWHmKmUKTiQYGzgRJ += 136828;
}int WeokkZ = 60023845; 
if(WeokkZ == 968102){
WeokkZ += 525812;
}ushort nKPZwe = 18003; 
float TupyKJW = 1.32962E+22F; 
string NiFzJ = "btSnipn"; 
ulong EmL = 8395820396297913; 
uint WSBiA = 3981; 
sbyte qqmdU =  -121; 
float ecIIfKYdDlKmjHOHWyaOZ = 1.356661E+37F; 
ushort zVtzwYbbLNSUxQhDfWcLSVKEGgyb = 8184; 
long cmMHOFGgnI = 49006058842564821; 
byte UWKeElQSoCaUaAPIKjuXAPPdGyP =  163; 
float QjVQtgDIdCONSNHqzwg = -1.906154E+24F; 
uint nufhHzfxiYJLRZj = 67; 
sbyte fSiN =  110; 
string nYuBOQsLHxnYWi = "eGOjbQsooyaOsHeIifjkhcJkTtfs"; 
ulong LiUiNEYNaGWKfdNbaeDOeTMXQ = 30044749927261705; 
ushort STiTBKxfEoyWFzRsaQZNSxW = 23729; 
long wxqVWiJpViRlbdJNiJsg = 37985824546233294; 
float WQjToZPIsonnDI = 1.984307E+21F; 
ulong dennaEopWalNHYwBlbbhPKYBpdT = 34218264339475944; 
ulong JQGFZc = 59306972480714044; 
uint GfugNYXLkNfjssasGsElxEQ = 18069696; 
string wEkBwqgPBmKuKSVtmXFdmI = "mKSnqcpNXJmCgamKTlEAfVYTtBkD"; 
long azNHXQDcPMjANMypOs = 15099441036565776; 
long URRNesJPPBcxhQxRpHimxcMSEwThP = 12673409048373145; 
ushort ucJCnpEfVxSSk = 11328; 
uint SeZjKABZjLyqFIzWTl = 18625797; 
ulong zDZoXMXNExGaqTVl = 27396735812083369; 
uint XVYZuoPqHx = 4071; 
int byhaLEqFaICVIeiVKNNJOcOF = 33; 
while(byhaLEqFaICVIeiVKNNJOcOF == 33){
byhaLEqFaICVIeiVKNNJOcOF = byhaLEqFaICVIeiVKNNJOcOF + 481803;
}string qNIVwSCXXt = "qyzILhMMyIUbZJEYutqqCGmEUyn"; 

}public void SlPLwhxgEJBRCNEH(){
ushort IlSTgFHbtLkVNmUlUlTiHS = 15970; 
ushort LSAQPcGgiQbHY = 6114; 
byte tOLWJ =  235; 
byte oSsOnfPZxAxNgkFJFOFySGVgJeme =  134; 
uint PFCBWKRScackAJyXijcqUq = 97005035; 
long lfjTLyVOIiFKIZNVDxPuxzkgdUAi = 23447732846461805; 
long oFoLbosz = 59152689866181716; 
uint VcPQYNXiwhTJBbmshjfLxWP = 36289368; 
ushort SJuhCzLRGuAHDOSUjFT = 35752; 
int xlfjQqYniyDheBaaskQkHX = 57568439; 
if(xlfjQqYniyDheBaaskQkHX == 590807){
xlfjQqYniyDheBaaskQkHX = xlfjQqYniyDheBaaskQkHX + 44508;
}int ATc = 24; 
while(ATc == 24){
ATc += 659177;
}ushort slbdwKsTimJMsezECISMWJhXKe = 62392; 
byte ZaDxsoMHBhnmixORgLcOEBKQIW =  173; 
string tpMd = "jlf"; 
short PNX = -31802; 
uint ANmugDuUBozqNPxGJD = 83; 
uint ilDLRLm = 63; 
uint kiixJXRW = 576890771; 
long UtBEjduTIcpFaSwLKIAIZh = 2430217762369468; 
long yBIsqgdWmGChSFxRDfm = 45660410685758699; 
string ODNsL = "BjcuDF"; 
int tGtOINickJCBu = 89930725; 
if(tGtOINickJCBu == 794559){
tGtOINickJCBu = 39606;
}float cPQgzVHAMKGDJth = -1.896793E-30F; 
long WMsdQkMMoD = 53874052081082531; 
int RCEzZWXM = 759199; 
while(RCEzZWXM == 759199){
RCEzZWXM += 764197;
}double LLZWMjqWOWcSMdpUAAywiUe = 1.703983E-19; 
if(LLZWMjqWOWcSMdpUAAywiUe == -370.8788){
LLZWMjqWOWcSMdpUAAywiUe = Math.Ceiling(Math.Tan(1));
int? TQxgnodDzkqOaQkDibbgbY = 4915335;
TQxgnodDzkqOaQkDibbgbY += 46536;
}string NujFPFcyTlQcHBobIFdCKUL = "onRP"; 
double LTMmcRsFmyeFfTDPuXYEg = 6.026476E-09; 
while(LTMmcRsFmyeFfTDPuXYEg == -8.438329E+27){
double OmAjBXptyzKcqHyfP = 7.116064E-29;
LTMmcRsFmyeFfTDPuXYEg = Math.Floor(OmAjBXptyzKcqHyfP);
object TRuDQzljjikpNKReVsGzaBBUeEN;
TRuDQzljjikpNKReVsGzaBBUeEN = 2.738434;
}sbyte LzejnpzNFXGaTAUbUtqB =  -13; 
byte ikfqFBgPwYnIguRlkwsYQy =  245; 
byte MOMCeGjyOVYh =  248; 
float zikbFSOHc = -3.745384E-29F; 
string nVMmV = "SNumqtejtzSelCfBImWGAYgJwYCej"; 
float MmZbzdVT = 5.197276F; 
long hkcVEZOmzppAU = 28434597998798195; 

}
} public class fSlAHxMtne{public void EYzsdXewGLxWAhSscBQBIUTfk(){
long ssBfb = 74929093044818412; 
double VEKVNsPdKSyEiJfthuT = 1.837343E-34; 
if(VEKVNsPdKSyEiJfthuT == 7.98711E-10){
double fEkWGcYpwo = 8.235236E+24;
VEKVNsPdKSyEiJfthuT = fEkWGcYpwo * 2;
for( ; ;) {
Console.WriteLine(4.80153E+17);
}
}uint fLQXTAhiPmsQdqiJklpVxBhsfeakZ = 7199; 
int JtXM = 468755099; 
while(JtXM == 468755099){
JtXM = JtXM + 749669;
}int AgEtVNUqGDFDyROAXjA = 733356; 
if(AgEtVNUqGDFDyROAXjA == 968829){
AgEtVNUqGDFDyROAXjA = 219474;
}double NUcxOENHXx = 1.678887E-32; 
while(NUcxOENHXx == 9.134615E-18){
double nfKWVQncTNMGhX = -2.993256E-30;
NUcxOENHXx = nfKWVQncTNMGhX / 3;
Console.ReadLine();
}short dBxBqzSqFmw = 31200; 
float Hxn = -2.893744E-08F; 
sbyte jdqViiyyOw =  -32; 
float HpyBGXLEnHuMzsOajhVhbTy = -2.755222E+07F; 
sbyte wURRbgftqYN =  35; 
ushort LVxWiLQJxEmzKM = 49611; 
float VzIoJyPtGsZy = -5.685907E-05F; 
uint MpXahxxAMynFExt = 46485226; 
int iPgbzHEEsRkwgNZYxPSkAbhEl = 21; 
if(iPgbzHEEsRkwgNZYxPSkAbhEl == 989202){
iPgbzHEEsRkwgNZYxPSkAbhEl += 364952;
}double FqJGnAuFSdJBbh = 89965.73; 
while(FqJGnAuFSdJBbh == 2.114229E+24){
FqJGnAuFSdJBbh = Math.Ceiling(Math.Acos(0));
int[] wqshpzHIXAcjbtlLcD = { 6201427 , 43276 } ;
Random cmjbczkYRyLkod= new Random();
Console.WriteLine(wqshpzHIXAcjbtlLcD[cmjbczkYRyLkod.Next(0,2)]);
}byte OBhOLj =  37; 
uint mcBKpVTWolppKeBPjmo = 12898320; 
ushort aFnbknCZTCSYVpOuoxgqkWQewIYC = 62609; 
float zRfBfcTIINmxORgFzxSZqZe = 2.857337E-33F; 
ulong HEoabaP = 60721740599872973; 
uint XBbnodAffTwANuxqTm = 35; 
ulong wZyYPJOB = 14738199479717226; 
short QUWhtUwwtpzhKgRM = -26889; 
string qnQqFnSYmGTFXWFDSomHDDup = "bxTuJbAKfZlZEqU"; 
uint iQAMClfEYyRwnOUATDSZEEEtVSWs = 289149; 
double fHuERMAjNRKF = 4.163337E-18; 
double CsFlg = -1.825289E+13;
fHuERMAjNRKF = CsFlg / 3;
try{
Console.WriteLine(fHuERMAjNRKF.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}string jZkSb = "CLuoHLmujKFMVepgKtFXZmmN"; 
byte TUDIPSZodzXpIefoLfpnWWPcoRn =  17; 
float zIdMSsSDVN = 1.044042E+09F; 
string juMASKChxXXXeAsXEWzOaomVl = "yhkECpVXSUfOlgNpkfOVmtj"; 
double QhmSVgQ = -2.33591E-32; 
if(QhmSVgQ == -2.271378E+33){
QhmSVgQ = Math.Ceiling(Math.Tan(1));
 bool? hhqAdX = new bool?();
hhqAdX = true;
}short gSfMMftfRmhRZJWFiyKzM = -1584; 
ushort KKDo = 37488; 
ushort pMzazuskgbFquXgOHFDM = 60541; 

}public void TXIycbbDdDwGReCIyjQlD(){
ushort hafDWW = 46343; 
string MHTCtaqpiFXcVPfcmZgsW = "gxQByXAfHtBMTklZoxTAc"; 
double elkZBKXEKNbBxLSE = -1.913171E+25; 
if(elkZBKXEKNbBxLSE == 946685.2){
elkZBKXEKNbBxLSE = Math.Truncate(elkZBKXEKNbBxLSE);
try{
int boeKymUkputBZFqwTI = 6453007;
if(boeKymUkputBZFqwTI == 13027){
boeKymUkputBZFqwTI++;
}else{
boeKymUkputBZFqwTI--;
Console.Write(boeKymUkputBZFqwTI.ToString());
}
}catch(Exception ex){

}
}string lXUnYyV = "IypopRDdOqNgSQlSJAY"; 
string BNlAQstC = "KQTdEGVNIxnWbptTqxL"; 
string zASxhLePBAiDjIXmOmH = "VSCPoAyTGzWJAYkqbIxURYgXftRI"; 
ushort EjedLGXcgInkibDBckx = 40181; 
byte QMgXM =  147; 
long IPxZVNEwnGuYBPHGQeuFpHkQuyt = 11061581399225410; 
uint igQXIXRlmFbeBgHjeGbEHGyt = 45; 
uint aGoPhRHYFIJKuFhTqnyHnj = 6170; 
double hDJxNjxglIomXKAIFyXsjczFcWa = 5.647427E-06; 
hDJxNjxglIomXKAIFyXsjczFcWa = Math.Ceiling(Math.Tanh(0.1));
byte EZlOdpJDYjXJFNLo =  38; 
short CPwgcylV = 21705; 
float QGklxxf = 0.002601112F; 
sbyte uMPenbBbCekPKL =  -89; 
float qSwSIQt = -0.000337898F; 
short oiAQcfwKsYi = -19839; 
ushort tdGRCCnEXqHNEMVwNQJjNtGu = 48696; 
int yBkiIaboMNB = 824892866; 
while(yBkiIaboMNB == 824892866){
yBkiIaboMNB = yBkiIaboMNB + 147954;
}byte CJOWksCkda =  119; 
byte YkQcM =  44; 
double CyjImUSgbhyjycHKFZQgPYn = -61.82701; 
while(CyjImUSgbhyjycHKFZQgPYn == 2.476792E+08){
CyjImUSgbhyjycHKFZQgPYn = Math.Ceiling(Math.Asin(0.2));
Console.ReadKey();
}byte PKIbUkPkVTEqFuxiVQb =  136; 
short LwdiTdac = 5409; 
string RmYAeIkuyoJ = "wnOGaPlsLpFYmYLq"; 
long ToEZOgzK = 5664660440782516; 
long fzamtIGBIZlgWUIlwtWwBHgpYd = 29541582927372368; 
byte kGWVtmAzyK =  57; 
double GAJYmtAqKoofdcYd = 1.809607E+24; 
if(GAJYmtAqKoofdcYd == 4.950698E-09){
GAJYmtAqKoofdcYd = Math.Ceiling(Math.Cosh(5));
for( ; ;) {
Console.WriteLine(-6.849418E+25);
}
}uint kOQ = 762540; 
long zCzQkLdHlnwLsYnBbyRswmKqiopZ = 79882612010069052; 
byte MBbKyDGNhbWmPb =  238; 
short lRhHWweUHiokbqkoG = 14806; 
short LPhLxhncBRd = -20874; 

}public void QuldPPeW(){
ulong tqCgLLXjpGXQxKxkOgbuQffak = 12907130309030222; 
double uUaEsDIOEmaXCHEbYUxankB = 1.232509E+20; 
uUaEsDIOEmaXCHEbYUxankB = Math.Ceiling(Math.Sinh(-5));
Console.ReadKey();float scgASZZIToSgEAHubuIiWdIGajMq = 6.831118E+09F; 
sbyte YZsLFouifRmOGtStpn =  58; 
long LbnuBzJmJclfo = 72171170077202218; 
string QUaIs = "bqVCjmYdqdQM"; 
double ObJpkmkuzZIUHWy = 0.01354163; 
double fVHeFMVnFHqPlKxehHT = 2.014923E+16;
ObJpkmkuzZIUHWy = Math.Ceiling(fVHeFMVnFHqPlKxehHT);
try{
Console.WriteLine(ObJpkmkuzZIUHWy.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}string wLQoFzOhWSDQnehAgyaQgRzaGx = "COEjnPkotubcJcFhCNjqEhpnOMkM"; 
int QJhCmtsJn = 6; 
if(QJhCmtsJn == 100724){
QJhCmtsJn = QJhCmtsJn + 101878;
}ushort ODJXmwetuzpw = 35795; 
int KgFsVgQkPTzioOsk = 38; 
while(KgFsVgQkPTzioOsk == 38){
KgFsVgQkPTzioOsk = 913575;
}short nchSVcmMyEfIBNAkY = -9957; 
float PlQG = -6.499292E-18F; 
sbyte ihp =  70; 
float dTYX = -1.870948E+18F; 
int MLILKxUizRblOblDQHWlEo = 3403; 
if(MLILKxUizRblOblDQHWlEo == 682547){
MLILKxUizRblOblDQHWlEo = 622806;
}byte chy =  12; 
uint YqELg = 672383493; 
uint FobHxgJQuixypTlMnWzOUPL = 116383; 
int clfEHOFljIZPTuqc = 59906126; 
if(clfEHOFljIZPTuqc == 450856){
clfEHOFljIZPTuqc = 347272;
}double dkLSQEABTIGFoFQKLIceogFuEM = 0.02064966; 
while(dkLSQEABTIGFoFQKLIceogFuEM == 4.941385E-36){
double wcMwGhzWlOMOomZXQ = 1.792645E-19;
dkLSQEABTIGFoFQKLIceogFuEM = Math.Round(wcMwGhzWlOMOomZXQ ,1,MidpointRounding.ToEven);
for( ; ;) {
Console.WriteLine(-3.194523E+28);
}
}byte FZtAXtAGVCVDPwkMMxlqwgY =  169; 
uint WSCZYkKtQZmtys = 22068594; 
ushort JpgqBCZcUTtzaLX = 33270; 
ushort eWDLTxGlAtYTGjKCZlpF = 21943; 
double WpQ = 2.206032E-13; 
WpQ = Math.Pow(2, 2.1);
string uSoDPDzSLNdOcFnEtmybfJdcMhGT = "WGYngCOfBJMOelnUOaEsz"; 
ushort xaRLPlQIWGiP = 9268; 
sbyte VdBzSFLmbyPhYhbB =  -74; 
uint CmpQYTLaP = 91; 
ushort QneBSZ = 37447; 
ulong NQnaeSBUfXoUEmGnCFhEVe = 87685164505069942; 
byte zQBADYPSBVcYilmbAlpuwkQILns =  188; 
sbyte syMtxNMfLEQpcac =  -98; 
float fJDosRBmZTOnHc = 4.556109E-36F; 

}public void GmQQAouCNFfhQFFyW(){
int WceVSiOTsyXOGaz = 877689; 
while(WceVSiOTsyXOGaz == 877689){
WceVSiOTsyXOGaz = 385155;
}sbyte GlufiniQKwkgABqXHLxceQQ =  -3; 
sbyte hLypaeOoAJyd =  23; 
int HmtwUYX = 57507; 
if(HmtwUYX == 958110){
HmtwUYX = HmtwUYX + 116690;
}long xVYRgzttufsjaJAEfRUyDyem = 22040348192546163; 
int lDeOzLnUYoCOWUEwPglml = 822585765; 
if(lDeOzLnUYoCOWUEwPglml == 920679){
lDeOzLnUYoCOWUEwPglml = 288093;
}double oYL = 3.626448E-16; 
while(oYL == 0.01393121){
oYL = Math.Floor(-8.666749E-12);
Console.ReadKey();
}byte bQOSR =  198; 
uint wHRkaUmsnVMKsyUwiOMsCWPouR = 9556; 
short MNEgxaiWe = -27510; 
long ZWyCNFVsKdcgsASDQjAodbjYgkaU = 29506443815515226; 
float dQcWCJKyOWiGfUFxTXMzuCEbey = -5.458709E-08F; 
uint EZLXfJmgACoKydLHgPcjLYgs = 539049; 
byte Ren =  71; 
double tjUiPWjNykjLeApzpGtodGStzExlo = -4.998835E-21; 
if(tjUiPWjNykjLeApzpGtodGStzExlo == 2.879451E-05){
double ZgaXBViOyNLtPJVIWOqCLkBXycYjY = Math.Log(1000, 10);
tjUiPWjNykjLeApzpGtodGStzExlo = ZgaXBViOyNLtPJVIWOqCLkBXycYjY;
object qGlngeWjKgJCVHEedCXjkn;
qGlngeWjKgJCVHEedCXjkn = 1.407049E+21;
Console.WriteLine(qGlngeWjKgJCVHEedCXjkn.ToString().ToLower());
}sbyte gSFBcRGkHHZkTqKGmiBwKMnQTmK =  69; 
long bCUVJgqhuldiicwghOhQomUKjwEP = 1825611488234709; 
short IDnePnsCEkuMfWGQIPIjMnDDBFek = 5197; 
uint XQjfGDjzGsIStuV = 20636440; 
byte bQQgluZLFjjgXeNyVVRanyp =  5; 
float xjiF = 2.467703E+29F; 
double yIgKYLGSGnxlKNHWaPtkAEAPo = -1.141967E+35; 
yIgKYLGSGnxlKNHWaPtkAEAPo = Math.Pow(5, 2);
for( ; ;) {
Console.WriteLine(8.560918E-36);
}long kJPomoxkPsQOVaQ = 88556506632234214; 
ulong aJPmlXmhMfMbzDbiOs = 14910890332201143; 
byte zuNhYMlGHXDwEGjUdeUbedofOeku =  20; 
ulong qMoKssFdMqwDQPqiM = 75857104494679747; 
string uUCRCRxlKnWSDAbhLJCA = "xVzXwUVoCwp"; 
byte jjkATMuDIUQFgxFTzkSFqX =  247; 
string gWBWhSGFSoSWlPQeOObJtnzpxc = "yJumkiFM"; 
ulong HBbjaogMWmwcBDiXtBTyjBeHhV = 15136028824077895; 
sbyte ZztYqFAQQw =  -86; 
float nPdT = -1.0674E+13F; 
sbyte ubeReeiGscnwVY =  102; 
sbyte gGboRNNHYtZDjIDaSFynglApkzt =  98; 
ushort pUWYldf = 7613; 

}public void oqjfLjchuMwctyTOUf(){
long suTlR = 70031099538706086; 
string XXgOghq = "hZeWxcjqXublUzXoIewVYmAptoWRO"; 
short XbwLlMhJFceCBLPnAZ = 22122; 
sbyte MwZV =  -1; 
ulong ggtEGRejzNMDBwRTUL = 33773098047981380; 
uint GxQMXEDy = 50717868; 
double HFPNmoHIUDfjBiZfKBIwo = 1578505; 
if(HFPNmoHIUDfjBiZfKBIwo == 1.846511E+37){
HFPNmoHIUDfjBiZfKBIwo = Math.Exp(2);
Console.WriteLine(HFPNmoHIUDfjBiZfKBIwo.ToString());
}int OlRKYjAYBBTssUskfp = 32585346; 
while(OlRKYjAYBBTssUskfp == 32585346){
OlRKYjAYBBTssUskfp += 70675;
}double NRjcOlORSPK = 4.744316E+32; 
if(NRjcOlORSPK == -66.88848){
NRjcOlORSPK = Math.Pow(2, 2.1);
for( ; ;) {
Console.WriteLine(1.668243E+10);
}
}float OpeGjXCIZtWRuFIfuMcQNEAs = 6.323893E+13F; 
string OalhptiDjY = "PuwyEocwlYkmnueckQN"; 
long XlUYgPCaxfWwfmws = 36369641993601951; 
ulong OkqYNypfSiCzyBCwQToRigKKeUJO = 81135543554840920; 
long tzfZumRIzEtTXyo = 32400996098503129; 
uint KmknnAkLtBQMjMxTxxfutlyamu = 8002; 
string CLEfLaGtkeihFXAkGU = "OywJh"; 
uint PkaUeuIQZehdbwm = 23; 
string lHBBcOALo = "fcxGml"; 
ushort pZOBPYwcCTJC = 25091; 
byte WnRoLYfFKtKTcMyOjSLV =  6; 
ushort SCfZlHbOMVwBsxXKtjwdge = 6541; 
short efAJJnGzDYN = 6332; 
double XJFXXHlfqmhsLGtFNxNIhCDedI = 8.768073E-37; 
double wDhhLeYOlBqEZeYk = -3.750004E-10;
XJFXXHlfqmhsLGtFNxNIhCDedI = Math.Ceiling(wDhhLeYOlBqEZeYk);
try{

}catch(Exception ex){

}short RgWGbQO = 3265; 
long VhlcnzHjQWRhJZtyiKCSgfuW = 52607972011005106; 
short gfGQGinZIxqJzXmF = 9170; 
string QJP = "peAEZssIQQiSmSsdsFhyXGo"; 
short unQjaCRssoMgGoIpOCSofZMoaJlC = -10600; 
ulong PhGFeDzPosAVDHXNoAhjd = 31014386742760640; 
short aCFWYZLENpM = 4585; 
double diJcWXbjHHtgOiDbozZ = -0.4585939; 
while(diJcWXbjHHtgOiDbozZ == 5.169577E+14){
diJcWXbjHHtgOiDbozZ = Math.Ceiling(Math.Sinh(-5));
int[] itgPNn = { 6713808 , 85869 } ;
Random cCRjiicIOJyZfhCbU= new Random();
Console.WriteLine(itgPNn[cCRjiicIOJyZfhCbU.Next(0,2)]);
}string ifHhTclYhb = "KdPKLSD"; 
float WgymlSWI = -4.211071E-21F; 
sbyte jkTwkcpgQJGHooctRpcgaTtcJuW =  -124; 
sbyte HVWnATVoAPmMumBQXWQ =  15; 

}
} public class bjZTqRRNRfoZlcLLbPiyDRmT{public void zgCSxZkEPG(){
long uibeDJJYMijQfVgIfnkXfFSLpUug = 2601571831216136; 
sbyte ahEVEoWLNBTE =  -14; 
uint OQQqqOJnDUoMN = 321070034; 
short BNncukaEPFgUnBZW = -29675; 
byte RpQFqTaaoDiUxlscKmstGPNCjCuot =  76; 
ulong FsxkKiWjtjMaGLKIVyoJL = 36310338994175861; 
long ZMzoePhtCqhawcNVASNu = 50294407899627076; 
ushort gLIDNngwGSNeOsDSYaQtWXM = 21775; 
long FzDewAYOjysSozOemUHRmPyVbtSbR = 68379381098488064; 
uint ngZheAfLsFSLSL = 733049474; 
ulong Idb = 63876306304723979; 
ulong lMysLm = 74483320589663843; 
uint HXcUIk = 410456; 
long dgFzfkEz = 52236205901408287; 
byte tfzWDtw =  14; 
uint zbobLCjBOXQyXZuALnPEMZZobCM = 57; 
byte cNJa =  150; 
long HIoIUnsWiADd = 4979341509760094; 
double PdWDUOLo = 1.818803E+22; 
PdWDUOLo = Math.Pow(2, 2.1);
object BCDKtxSlMquFkTBOnozfHftqdU;
BCDKtxSlMquFkTBOnozfHftqdU = 6.662982E+32;
Console.WriteLine(BCDKtxSlMquFkTBOnozfHftqdU.ToString().ToLower());byte hlGzGoMpqyFse =  12; 
uint pmQFOFa = 97811090; 
short LjMzqIuoZBBGapbyTNEuKncYk = 22550; 
int ElUBmzbfGYnKLNCJPIQ = 7520; 
while(ElUBmzbfGYnKLNCJPIQ == 7520){
ElUBmzbfGYnKLNCJPIQ += 417116;
}double xgNnzc = 1.578327E+17; 
double RUzTJWMujEbqTxeVepqFYTKPIWb = -294.5673;
xgNnzc = Math.Floor(RUzTJWMujEbqTxeVepqFYTKPIWb);
object nZRqmADokLNE;
nZRqmADokLNE = 5.155732E-35;
Console.WriteLine(nZRqmADokLNE.ToString().ToLower());sbyte xDagTkoZsWbMesRldoUtcQpDo =  63; 
ushort xKtUjRo = 25851; 
string RMJk = "pLjTZFcKcgN"; 
ulong lzSfQgDJDhCndxjZziK = 1649188377600212; 
ushort qNybJiElLVCJJNEIGVhSKEYcYglIM = 15560; 
ushort hJkudlwnHONmiiQmmy = 17880; 
uint DoOPudCXVAzDXB = 541621839; 
byte tBUkWRLUPpYuoYTiGu =  114; 
int iTqMnnLkJRLdTUZCqsWdB = 2151; 
if(iTqMnnLkJRLdTUZCqsWdB == 210696){
iTqMnnLkJRLdTUZCqsWdB = 509119;
}long UFeFUkIDAJDaKgAmqwdTIobB = 28643126176084425; 
string OlHzZJsYRKudRh = "MjQxQgGDEdMqZkoFPZnsmH"; 

}public void wuBKysgysMWHamqkoiJcunssNC(){
sbyte eEQzesucoyikyHtFdFo =  -40; 
ushort YjFFyhciWVmBsPWyCJToVRZ = 29931; 
int UNnimNhqFHUBUlagCmYWoFJ = 6772241; 
if(UNnimNhqFHUBUlagCmYWoFJ == 164213){
UNnimNhqFHUBUlagCmYWoFJ = 853213;
}sbyte gZKnoUN =  39; 
uint bkoEaOHoPoNA = 48495173; 
short JfHnZ = -2007; 
ushort OBjYLmOFzEmuuc = 3952; 
float UdSyHqkYklIbltEOSQAEIEDBha = -5.61019E-21F; 
double GbOwfwEagiZaBafElkKGC = -131937.4; 
if(GbOwfwEagiZaBafElkKGC == -1.653993E-14){
double mYcBGGmRmhjSJpFHcCuE = 4.268885E-14;
GbOwfwEagiZaBafElkKGC = Math.Floor(mYcBGGmRmhjSJpFHcCuE);
object itzBjPTlLBWb;
itzBjPTlLBWb = 1.766586E+29;
}ushort JbfUntXaTAPkRHBToZ = 43415; 
int kKisEOzmyDwTfUlUcyz = 410727; 
if(kKisEOzmyDwTfUlUcyz == 448329){
kKisEOzmyDwTfUlUcyz = kKisEOzmyDwTfUlUcyz + 937084;
}double XwJ = -1.384475; 
XwJ = Math.Ceiling(Math.Sin(2));
object VchxUXBOTXIyUd;
VchxUXBOTXIyUd = 5.253553E+25;
Console.WriteLine(VchxUXBOTXIyUd.ToString().ToLower());int ubMOpiCTXIQ = 494905; 
if(ubMOpiCTXIQ == 351975){
ubMOpiCTXIQ = 765348;
}int touNuMcaMRqG = 6505; 
if(touNuMcaMRqG == 373563){
touNuMcaMRqG += 706812;
}float ClmDDpFqfznPTXjaaoqcnhQcHcBPC = 1.742916E-34F; 
long GQgbRjegLVnDCMszXodtQMtxkWA = 4002033658622524; 
sbyte Yifm =  103; 
int CAeqiqEnZ = 1054; 
if(CAeqiqEnZ == 699065){
CAeqiqEnZ = 822107;
}float psVtTVIykxYfYgVigVLSi = 0.0004632623F; 
byte wtOuLshowW =  143; 
long JFxTYVFiahkKpPfcBKUJeyDTdA = 69449061322705234; 
byte wpIEIf =  102; 
byte KwDcmUmJZPkbEQzQOs =  115; 
double icDYHkbqbZcqpteCOEJXJDzbeS = 8.989853E-26; 
double tZdHKksXPiKmxtoZOP = Math.IEEERemainder(3, 4);
icDYHkbqbZcqpteCOEJXJDzbeS = tZdHKksXPiKmxtoZOP;
int? CUOKREhlfdFSMCLXYMp = 7786744;
CUOKREhlfdFSMCLXYMp += 81167;int HJFI = 28102196; 
if(HJFI == 254937){
HJFI = HJFI + 897780;
}string JAiHbfmlXPeXWdCeYtCOfPNWziL = "iqbGSVPKKtsIDsgSEHL"; 
long MSUtnOTMmhSwIcGNAfbnRwG = 73983895465408198; 
ushort SyA = 8674; 
byte gWYxkAmVoCfeCKwWpl =  225; 
uint ajRU = 4090; 
ushort GfHmDVsTYwBHpAhPmpOGWhnm = 48029; 
uint MgDTeT = 67; 
ushort SYOdcqQtUMCcbHa = 42307; 
ulong zRtBbQGMxCwOkQMpwqZDTQyTSP = 2648939110643337; 
byte fqXYyqVnXgBLpLlKIk =  0; 

}public void tBJmkWHBJYJZGhkdubfC(){
float VMnUuuK = -1.220706E+22F; 
float DkFJKOCLu = 2.228374E+36F; 
uint fhBhoHfzNjFWZqDmR = 183639; 
ulong dWGwKmERBEWnMPlUDThOOPa = 34531016096689316; 
ulong PbkUNGtCHmGIInDqtnGba = 38105813944285513; 
ushort zgGXNaZPnPlhDwsexUIfoAOIQOoo = 5251; 
int WmohCcqQptAsmPSMB = 5808; 
if(WmohCcqQptAsmPSMB == 938294){
WmohCcqQptAsmPSMB = WmohCcqQptAsmPSMB + 269887;
}long sXgZLLLJloUmmRbfdqMQhCTYbPbAd = 21358990673521634; 
float IOKaAXznxDxTFCSRRei = 2.935248E+32F; 
long FHoUHdIW = 56467522793090195; 
string BFXHMwyOssaaTKYDet = "cSjPGAGT"; 
uint spLiuwenYVUySMpWWubfBuhj = 705373785; 
short sgfCZYZboRznTnfyoGazzJEl = 22303; 
short YhBlKeoWRPt = -3173; 
float nHfVNllnLUqKTqJEUKIJaUP = 1.834645E+21F; 
byte snJWigLjczpefdKyHU =  253; 
byte mnUKQkMtSBgEITabYiEhhHfFFCFp =  115; 
sbyte PXIeaOmsWZQjXiCzjKwIM =  -19; 
string HipenHwWKQqGaPanMQHhqxhw = "duGcB"; 
double TRyRyhepEUBh = 1.096275E+32; 
while(TRyRyhepEUBh == -1.057291E+14){
TRyRyhepEUBh = Math.Truncate(TRyRyhepEUBh);
Console.Write(TRyRyhepEUBh.ToString());
}float FOwPQesmwRnQxFKLYpVwFlTgFpXQl = -1.362819E-15F; 
float cMCyuzYidXtjCHKpGTnqzutUojB = -725.4985F; 
string ByVuT = "mERXlBo"; 
uint dJnBGxbAyItHikYRHcRSNHmRhtox = 2297; 
float usgS = 2.304769E-36F; 
ushort zXzMACdtTgFyyJjyxysmh = 39845; 
sbyte PomRegXG =  37; 
sbyte PhBqHoWDmHxG =  55; 
sbyte wyKiCTRmXjnloNjZQopuo =  103; 
sbyte XkcTEsKCKwLAVAHAYVEgsc =  33; 
ulong IGpw = 42651414308198131; 
double FBMDTcKdpIQxuqZtUFmPW = 5.558751E-11; 
if(FBMDTcKdpIQxuqZtUFmPW == 1.311655E-36){
FBMDTcKdpIQxuqZtUFmPW = Math.Sqrt(4);
for( ; ;) {
Console.WriteLine(-8.617123E+11);
}
}uint zaUVVT = 270274; 
short VSQXTWUcNswxfaQQ = 10281; 
ulong MZEH = 31582911568314842; 

}public void taKilVA(){
string dehLbUUeekKkGBskFbgyZy = "xmcfpIEN"; 
byte bWz =  125; 
uint SFQBpmYOBqXHUtFLHTLzZOEyBz = 172948; 
int UMehnSiWUjooGZKXSV = 304199; 
while(UMehnSiWUjooGZKXSV == 304199){
UMehnSiWUjooGZKXSV = 36735;
}float btZSR = 3.815952E-24F; 
double lDMfNDKZCiLhFNE = 0.9013794; 
lDMfNDKZCiLhFNE = Math.Pow(2, 2.1);
object NGGmSjXxHwlqcVHhnBAWWN;
NGGmSjXxHwlqcVHhnBAWWN = -2.767572E-30;
Console.WriteLine(NGGmSjXxHwlqcVHhnBAWWN.ToString().ToLower());string jUHSNaTJcjFzt = "lNGZuXyEpxLC"; 
int filywJeo = 859621336; 
while(filywJeo == 859621336){
filywJeo = 24219;
}long zNb = 3102067887342389; 
int jhkxJDUYhdLlWTNKTg = 926057; 
while(jhkxJDUYhdLlWTNKTg == 926057){
jhkxJDUYhdLlWTNKTg = jhkxJDUYhdLlWTNKTg + 513263;
}ulong bOOJpMALJLeehzNcqJpGujINVY = 63699024441173107; 
uint jzOYNk = 417; 
int ZXimIojgARfaOqkPyyCtutXH = 848733585; 
if(ZXimIojgARfaOqkPyyCtutXH == 843383){
ZXimIojgARfaOqkPyyCtutXH = 184115;
}double hfZDBaEVBEoKajLyOYO = 7.878821; 
if(hfZDBaEVBEoKajLyOYO == -2.469157E+08){
hfZDBaEVBEoKajLyOYO = Math.Ceiling(Math.Cos(2));

}string fGuTemdshfauCMPiZysQuFDQlQ = "lxMhqnXa"; 
uint LaibZWmyPDIwsjuylACcEHoc = 923196048; 
ushort KgFMptoyjOuIptxnDQeCOUbzAymdM = 6922; 
byte BnOJ =  247; 
double ZVXmy = -3.295555E+28; 
ZVXmy = Math.Pow(double.MinValue, double.MaxValue);
 bool? TLFhl = new bool?();
TLFhl = true;uint GgesaAxeAYHjxUzlWFyMHTb = 445073; 
double RAuKPdfefPskKxImLYloa = 1.410407; 
if(RAuKPdfefPskKxImLYloa == 6.611735E+17){
double IUcQGGf = Math.Log(1000, 10);
RAuKPdfefPskKxImLYloa = IUcQGGf;

}ulong YzhfPjKtgGGcw = 33130995330196505; 
float WAVseMzzWpBdRSml = 1.340106E+20F; 
ulong kxfappHNiNHhfZlGTRFIJhK = 9354046773676527; 
ushort foAuUcPeLpjKsXS = 40805; 
long gihecNtfEqgE = 471824436662274; 
double PDXRAcCOgTDNljpblMaOzzXh = -1.016597E+22; 
while(PDXRAcCOgTDNljpblMaOzzXh == 2.702763E+34){
PDXRAcCOgTDNljpblMaOzzXh = Math.Ceiling(Math.Cos(2));
int[] aklpqFQGLGTRYZguU = { 7230793 , 70288 } ;
Random MUZzKjJnKfVHtoYJTkhacZqGD= new Random();
Console.WriteLine(aklpqFQGLGTRYZguU[MUZzKjJnKfVHtoYJTkhacZqGD.Next(0,2)]);
}double oVXnmIfAC = 2.096635E-37; 
while(oVXnmIfAC == -3.554413E+31){
oVXnmIfAC = Math.Pow(double.NegativeInfinity, 2);
Console.ReadLine();
}string zNMfYZfOmgBPIVzy = "tDfklmYIKpeMeDw"; 
double ihNtghQwMETJoIHifi = -3.786331E+30; 
if(ihNtghQwMETJoIHifi == -2.087403){
double bciEfRdNpLflwpPBJeYwJ = 0.001015011;
ihNtghQwMETJoIHifi = Math.Floor(bciEfRdNpLflwpPBJeYwJ);
int[] ZcnWIZVTzOtDaO = { 3767419 , 93333 } ;
Random kVpyZWZFBMSJATZEAijfPQQJMk= new Random();
Console.WriteLine(ZcnWIZVTzOtDaO[kVpyZWZFBMSJATZEAijfPQQJMk.Next(0,2)]);
}uint YJqgkOhfZyZyNk = 312624024; 
ushort AkUtQIdueIMPDddeOissJlwujNgqd = 40034; 
float PtyqCFSKdQJBtk = 4.091656E+33F; 
ushort xHYdqmPBjdCMqzPpaiGM = 36928; 
sbyte JnSDVaYSjWw =  -85; 

}public void yLcjCbuP(){
sbyte AjLckLqIenpp =  77; 
uint UAllsPVpETogwkCPDBWts = 93; 
ulong SoqVVg = 3204074412995997; 
int UhjVFVyDAOQTfsLDye = 509753; 
if(UhjVFVyDAOQTfsLDye == 667607){
UhjVFVyDAOQTfsLDye = 731477;
}ushort VEaGZKtPzcNSDWRPxg = 22031; 
string hWYLzqcaAzyBaaU = "TPfBpUJPGDDKPYCut"; 
sbyte IbUCsqPYEpqDWXmDslBhnmj =  -101; 
double tkCXdhUmmBZnNiBWfAnZGMDw = -1.355822E-25; 
double EYFyHljIKKMEezsHshK = -1.390497E+26;
tkCXdhUmmBZnNiBWfAnZGMDw = EYFyHljIKKMEezsHshK / 3;
Console.Write(tkCXdhUmmBZnNiBWfAnZGMDw.ToString());sbyte JwsWqSsULXRdtxohLTJxNEmESnKi =  71; 
int HBmZOSahHVjHYkNzIx = 6216; 
while(HBmZOSahHVjHYkNzIx == 6216){
HBmZOSahHVjHYkNzIx = 204200;
}sbyte Odchiff =  -37; 
byte fxBoKLbCDWntfQcNxZ =  74; 
string cUGdWfAMeCHZiskVdWFCjIiV = "YmfwKgdiPdDVk"; 
ulong DPzwIuWyRRscLCFuLKK = 70643727514277358; 
ushort IaSiYYujOn = 104; 
string nFqdtHjJSkCQNizdaeECiiRB = "MDjjXxKVdeMcqjjkGQPptlQWgNi"; 
ushort GfNRjHeZzUPSUKeMa = 44640; 
float pEsbjCANndjmm = -0.0002227707F; 
double lhDelgWazzDLjfdockcNMIIgsLuKD = 0.7420901; 
if(lhDelgWazzDLjfdockcNMIIgsLuKD == -1.205216E+20){
double exxbLcXbeYgY = 299.9759;
lhDelgWazzDLjfdockcNMIIgsLuKD = Math.Ceiling(exxbLcXbeYgY);
Console.ReadKey();
}ushort EuqQSBkcVDFKkERdGu = 43909; 
long aydPWEITlxUpIUAIBO = 3615192664844440; 
sbyte CEWXO =  124; 
int maVXEiKgKuEKsxfGNDeYlCTRyDsI = 942740; 
if(maVXEiKgKuEKsxfGNDeYlCTRyDsI == 318268){
maVXEiKgKuEKsxfGNDeYlCTRyDsI = 542186;
}ushort SYCqseSCCxNezpjl = 58847; 
int LgeBaRbHNSVtPcFOPbOHgK = 2952; 
while(LgeBaRbHNSVtPcFOPbOHgK == 2952){
LgeBaRbHNSVtPcFOPbOHgK = LgeBaRbHNSVtPcFOPbOHgK + 189796;
}byte pOMRJtOyBAqzeJHdXcTDujg =  247; 
int PPB = 5798; 
if(PPB == 993267){
PPB = 549098;
}sbyte YtlZsBnoJFKJWRjpIsfH =  65; 
byte FYXoVAbffAxZKIDcKSKV =  107; 
sbyte EuuBPH =  -17; 
uint MLIauksPHAEiBSBJepwOdqlASu = 529138328; 
double zKYDmYZDJ = -0.3519444; 
zKYDmYZDJ = Math.Pow(2, 2.1);
for( ; ;) {
Console.WriteLine(-9.716348E+34);
}long iJQUkYkOdSiYOEfiuoilxpZxxFNg = 41226579330965327; 
string xOU = "opYMoVA"; 
byte VezBTJuyHleoOkcZEdKftSuq =  73; 

}
} public class KzL{public void RHoALbBKHN(){
long QCQSBeVMMSPexcLglS = 53193674277236517; 
uint zUx = 419328454; 
ulong TCxB = 52913785990193195; 
long uyOOOQzgsSl = 48612403006065115; 
string FPsusOPuGPkRLexIjIaFMjFnZ = "MMEZDyJAPJxuwbKNMdaOi"; 
sbyte OJaFwfwkdwJcdR =  34; 
byte iloOkPlZVZyCnkBDpPpCTmt =  97; 
ushort YNYHVeRmGXTI = 36613; 
float GBgYKQkiANbpLUWSVpTRAao = -1.23279E-11F; 
long FEK = 34763550424556835; 
ulong OFzTTOftKfYqKcoGk = 4392184714684567; 
ulong XIiFADpcVCxkFFIVyMpCwzHCkQ = 15897435766606317; 
sbyte BBzPtZtdPSawhLhePnzzaI =  -30; 
long EUWTIhKxWuXmChwfEMtPtGzS = 74766021103378777; 
ulong QfykgKqYkpjRmfdtyVaTwXweoJsmM = 60251871647111934; 
short JyuuhIMNKtqnXfVESKt = 27407; 
uint elZPttKOOLep = 4219; 
string nxEWITwXFMDYAGoxgp = "hwtoscWGGiHN"; 
uint oxWsCKkRbkHRMniwJ = 4; 
string hVgkUMnPPXYcxjjsdGGhqthICq = "xkAyLxJhBQhIYWblmJOp"; 
string dFEwVCONAKEeRFpBmHNcJDWYIkphV = "NbEyhUTpCgtOqydTfxWNwhDbpBJGN"; 
long HSAWDCtyw = 7371914275190479; 
float fUkhEFzCgHlnDSVMWRBCsMY = 3.490376E-31F; 
uint Wsxo = 71; 
double sxKWqdamRiswlLXYwX = -2.859802E-07; 
while(sxKWqdamRiswlLXYwX == 1.144357E+14){
sxKWqdamRiswlLXYwX = Math.Ceiling(Math.Sin(2));
Console.WriteLine(sxKWqdamRiswlLXYwX.ToString());
}double qocKpOnzQjZfgEFetWBXJkX = -1.841184E+29; 
if(qocKpOnzQjZfgEFetWBXJkX == 1.891622E+16){
qocKpOnzQjZfgEFetWBXJkX = Math.Ceiling(Math.Asin(0.2));
int[] YefqSQkZDesLBTximp = { 9496464 , 5276 } ;
Random KxKHTqtccKF= new Random();
Console.WriteLine(YefqSQkZDesLBTximp[KxKHTqtccKF.Next(0,2)]);
}uint oFobWpadLWZWsYFVUfhnfnzlaAOi = 9692; 
sbyte jEESTluXJs =  121; 
long wadzPKSVmRagcayDSONQLMPuMb = 55865064544581100; 
int KKCnouXMEMRkXwQ = 239522; 
if(KKCnouXMEMRkXwQ == 674216){
KKCnouXMEMRkXwQ += 233829;
}long NbgKHdLsLxYTimzBpPcRZkz = 10419123897166949; 
byte XOmiKpITufOYDIqOuTUjTNDEtVSVI =  124; 
uint NTlAkqHAwYGeKPmg = 282607367; 
uint bcgjlKQPyMk = 586458715; 
byte KwSEcmuOU =  35; 

}public void UEJLetalzKMbmCC(){
float JNMB = -11660.17F; 
int ZLwcInlcOQlfMISTsQCYuF = 52; 
while(ZLwcInlcOQlfMISTsQCYuF == 52){
ZLwcInlcOQlfMISTsQCYuF = 150011;
}ushort zafZfIFiRxAZzLoMoaVShGbFquKU = 43338; 
short eHRqlaN = 3280; 
byte jZfAufplJc =  23; 
byte PTYBILYLBCmusIudLD =  118; 
long YHWfXntIZldScczmAe = 12032413213488736; 
uint iXBHCaRlcwLyFcCojDoGVRlzHzZo = 8; 
string QxAbMypckljetuHChhJCMTbqMbZ = "bCdY"; 
ushort HiDADMmmuJHgelSygawAiWwi = 19296; 
long mbJilLENzZJGWoA = 58376879477067412; 
ushort DUHL = 50438; 
ushort SoZkLAwx = 33817; 
uint zzwAkKybzKNKb = 280412; 
sbyte RXcKGzNHonPToixoRfWI =  -112; 
long nMVBfGYNxuNFExKSaJSVxJiiBqwj = 76227595631786037; 
long IbqBsADFSShmVYkOBFDNZOkOudVh = 59893127668016869; 
int pKHWzDcfyAutAjwXYTREfoEwgpk = 77853; 
if(pKHWzDcfyAutAjwXYTREfoEwgpk == 384936){
pKHWzDcfyAutAjwXYTREfoEwgpk = pKHWzDcfyAutAjwXYTREfoEwgpk + 188143;
}int SfdpILnHwNd = 44; 
while(SfdpILnHwNd == 44){
SfdpILnHwNd += 148277;
}double Qaf = -8.399392E+22; 
if(Qaf == -2.314115E+10){
Qaf = Math.Ceiling(Math.Sin(2));
Console.Write(Qaf.ToString());
}ulong EDiPMGaeGQWIxSPFGoVhdkqzGlEZ = 82799355339722179; 
string hgTdXXyCi = "utMJJFTskLjAbbnJbGhkfmCtgOnZ"; 
byte bedGGykfIVswq =  91; 
string eSQoWRuXSgRtDVpTdRJe = "xwQyVYlTKWiozJ"; 
ushort ZykLgOzH = 55631; 
int FAqAKYlJyZRbThdikJddlUJ = 39; 
while(FAqAKYlJyZRbThdikJddlUJ == 39){
FAqAKYlJyZRbThdikJddlUJ = 655518;
}double FqkHusxA = -6.252152E-22; 
double cFmXOABLQmelEWaBQl = -3.310432E-07;
FqkHusxA = Math.Ceiling(cFmXOABLQmelEWaBQl);
object QOiOSlVzWYgSncKlRRKWEf;
QOiOSlVzWYgSncKlRRKWEf = 2.130089E+11;long PqpfeMJYQF = 50231276473549479; 
string mxeamyQVnkyRjwhmP = "iaHaJQBTM"; 
uint dcTZebsmo = 632796048; 
double KGgFJDxTVkNaWlOoBuMsKji = 1.359843E+25; 
if(KGgFJDxTVkNaWlOoBuMsKji == -1.807149E+22){
KGgFJDxTVkNaWlOoBuMsKji = Math.Ceiling(Math.Acos(0));
try{
int cBxDNEiHTxSkWbVLGFj = 1735023;
if(cBxDNEiHTxSkWbVLGFj == 63362){
cBxDNEiHTxSkWbVLGFj++;
}else{
cBxDNEiHTxSkWbVLGFj--;
Console.Write(cBxDNEiHTxSkWbVLGFj.ToString());
}
}catch(Exception ex){

}
}string BMZVbgTTsknsm = "zfdTHEwQLWExkedAwECWRo"; 
short SPLqLBIonesCcoZxY = 12409; 
string sGWP = "TPKQpViOCbSTefkonhfNDxfE"; 
sbyte gnojlTWCSLXzYjiNCmhEUFQOeyQb =  -66; 

}public void JgLsOVzqQYWDnB(){
short jWEPVSwCdDyBfOcUowbQOGwn = -14416; 
byte iHqqpxonTzyCRLj =  73; 
byte XlbVcnSzCbFCpBL =  103; 
byte MlgeRBjxSUkfEpjPqYxmFdzSVmK =  89; 
sbyte RwzoEZqTwBMHxYnxynsBjMZYpINo =  -80; 
int RnVdPHoNjzyEVCYIBFAPwaIMe = 502852; 
if(RnVdPHoNjzyEVCYIBFAPwaIMe == 243279){
RnVdPHoNjzyEVCYIBFAPwaIMe = RnVdPHoNjzyEVCYIBFAPwaIMe + 39399;
}long KVsBVzPiBhcNhdyqalLLOwRHj = 36547454302930285; 
ulong HGgGfKHV = 8922498605811624; 
string YcodMqWfKSJ = "GYwTRLTH"; 
int hAY = 20; 
while(hAY == 20){
hAY += 654327;
}double eQKVDTQJaaOmIZbBAlWbtMXVPfLg = 7.808567E-32; 
while(eQKVDTQJaaOmIZbBAlWbtMXVPfLg == -4.269264E+16){
eQKVDTQJaaOmIZbBAlWbtMXVPfLg = Math.Floor(-14.9626);
object uECuzTPCXeYxhWCDPKXtDOAQLho;
uECuzTPCXeYxhWCDPKXtDOAQLho = -2.367606E+13;
Console.WriteLine(uECuzTPCXeYxhWCDPKXtDOAQLho.ToString().ToLower());
}long ESLewEnzoMR = 31494480078747399; 
int tLmkjJNpbLZmmNUoQktTNIBNPtd = 79837674; 
if(tLmkjJNpbLZmmNUoQktTNIBNPtd == 874381){
tLmkjJNpbLZmmNUoQktTNIBNPtd += 111148;
}ushort hyUlVBhn = 814; 
float TVNndIzLiLjAwRFZBnA = -6.220599E-28F; 
float NRVZzsGtBVjBiaCJF = -2.413005E-08F; 
byte XYzupCHZCMzatZxi =  168; 
string DsKFlqxYZGVL = "HcPTxFiRYuYiLAQafBWudJ"; 
sbyte tmdTDIuHaM =  -29; 
short DMdRBFXL = -5642; 
uint HXUeYpJXVCBjBpnqZsiZeAQDO = 570635920; 
uint jEXMm = 5994; 
ulong IKcyuJcDEC = 23445373074643117; 
sbyte qgNgSMXDzndJ =  14; 
long zWIegAlocpRlkLjBezxYCX = 37859469669449963; 
int mIOFdcZSfRLRaAMSnNYkg = 18218452; 
if(mIOFdcZSfRLRaAMSnNYkg == 64507){
mIOFdcZSfRLRaAMSnNYkg += 411800;
}ushort gZFNhZEXGxczFSKmeEjZIQCAsEAzl = 37294; 
string FSYSfdTAhVIqVNCUAoVA = "xsqyewEA"; 
sbyte cwwUNUSgbLKRNhkEFbGYX =  38; 
uint iBIubJoackGVBT = 526528; 
long UPCFFim = 46496333749428054; 
double mlyeswKPDCodMlGqPNMUEV = -8.837822E-29; 
while(mlyeswKPDCodMlGqPNMUEV == 2.601191E+19){
mlyeswKPDCodMlGqPNMUEV = Math.Ceiling(Math.Cosh(5));
int? xepnRYHhehqNpEBIEUfpnDFKRgLl = 4185901;
xepnRYHhehqNpEBIEUfpnDFKRgLl += 98821;
}byte UAb =  189; 
short XpJTAHyLmwQbtzUezMjLRGLugGz = 20333; 
byte kGPTWDCk =  70; 

}public void wtljdtGNgPaoDDdqGnMUhHzbk(){
byte ADsEnztpz =  96; 
uint NZMJxkVclQFX = 4764; 
ushort OUkZSgfkPlNRimGXeXYVx = 62918; 
string ohbKXupPdKDEiCzHTCUkyeUZk = "SpAAmQTuLLexmuWYNzKc"; 
ulong zsbUdQtkncAoCZoFyN = 58216156079692108; 
short qwohToShysMcaBy = -29819; 
double TWJhbXsaNKQFHFCUTSOqxmggheaH = 9.138007E-16; 
if(TWJhbXsaNKQFHFCUTSOqxmggheaH == -0.003581557){
TWJhbXsaNKQFHFCUTSOqxmggheaH = Math.Ceiling(Math.Acos(0));
try{

}catch(Exception ex){

}
}double ReYVRO = 1.596912E+07; 
if(ReYVRO == 1.030884E+08){
ReYVRO = Math.Pow(double.NegativeInfinity, 2);
object MPGLHtQQeMAxRqAZqjYDTjLjxdPH;
MPGLHtQQeMAxRqAZqjYDTjLjxdPH = -3.911539E+17;
}string ZLwMU = "qelMYXnYOYdCdwnEFiPOdzkiZJ"; 
double kmghpRTqR = -1.602434E+13; 
if(kmghpRTqR == -1.02734E+15){
kmghpRTqR = Math.Ceiling(Math.Atan(-5));
Console.ReadLine();
}int XmfHLyyBWHwsEeFh = 78; 
if(XmfHLyyBWHwsEeFh == 489765){
XmfHLyyBWHwsEeFh += 327271;
}ulong DxX = 56866540705708978; 
short FKZlxCWWjbUPPKmHmZGC = -27133; 
string AcbXggTiqo = "xfFecPQesudFLomI"; 
byte gODBwBiwFQQqS =  140; 
uint mhchXRLZMU = 458104; 
short TzHPkBYWLdVIkanbDBexb = -25991; 
int ypdDP = 78163; 
if(ypdDP == 916575){
ypdDP += 937960;
}short csZppyYmoHLMKqVsVHOTD = -10085; 
float whjZjEodWsAiff = 4.848224E+11F; 
int hKtRejisioizgLyEVinYcMqHP = 8356; 
if(hKtRejisioizgLyEVinYcMqHP == 797637){
hKtRejisioizgLyEVinYcMqHP += 909630;
}sbyte MjkdFcZRR =  -28; 
string zkPLDkDuIOxsMbMswfttdkwBmAp = "ngkqqM"; 
short ccLFtL = 7088; 
double SqDpIxMaHRmQnycUxgBEfaZ = 1.428101E+07; 
if(SqDpIxMaHRmQnycUxgBEfaZ == -1.048311E-09){
SqDpIxMaHRmQnycUxgBEfaZ = Math.Ceiling(Math.Acos(0));
int? qRMsdslGzIqBbUgHBPdPpeImC = 3052475;
qRMsdslGzIqBbUgHBPdPpeImC += 8857;
}short TCmjzRpHhCM = -23228; 
byte yckDlhWLkIlmmj =  239; 
uint JjPldFMGyqBWELnORfMtNSh = 1580; 
short YlEgKINcXykuyzXPRx = 18528; 
int XzxdkgbQbxdNaNuUkRoRIqpHMnUQ = 34; 
if(XzxdkgbQbxdNaNuUkRoRIqpHMnUQ == 685836){
XzxdkgbQbxdNaNuUkRoRIqpHMnUQ = 634541;
}string hczBMmWbhIyMDVHFAQpHhPs = "mkNKAKctMHMhsOewuoAdRimiLsTT"; 
short FsLDnCBlDyFdZ = 27962; 
sbyte lMMUlTpnfdXEYhQ =  -82; 
string buxQAEUiMbA = "iqZlfhhxiLcyxjEWmwIwcJU"; 
ulong xlha = 4975110502043002; 

}public void cJbGhqsZtpZYWqueLAxkjJ(){
double xgEOIdoXsBtYtxCExccWibgQUKIwj = -563.1564; 
while(xgEOIdoXsBtYtxCExccWibgQUKIwj == 1.49833E-36){
double qpacGwYwLnYTPhHZRuZ = Math.Log(1000, 10);
xgEOIdoXsBtYtxCExccWibgQUKIwj = qpacGwYwLnYTPhHZRuZ;
int[] UOZYZlsnAQSLNWg = { 6936322 , 37679 } ;
Random LSzZuxpEuNOkObZ= new Random();
Console.WriteLine(UOZYZlsnAQSLNWg[LSzZuxpEuNOkObZ.Next(0,2)]);
}float nHyYjt = 0.01558197F; 
string yTdKZoUwYbyywEmzgAzRiqTPSPn = "oHuYnIXTKBPNfowSKRkTLLlAalO"; 
sbyte nSj =  -124; 
string qIzjNH = "OHLLMUWafbEuj"; 
int UXbUSSxJFMfhd = 2058; 
if(UXbUSSxJFMfhd == 246828){
UXbUSSxJFMfhd = 308222;
}double PoBUjZiQGHQDWNBhTPC = -1.959155E+35; 
if(PoBUjZiQGHQDWNBhTPC == 2.052879E+09){
double ufhUHQfJCaITmSZfoGhyKzGzOFH = 2.755354E+26;
PoBUjZiQGHQDWNBhTPC = ufhUHQfJCaITmSZfoGhyKzGzOFH * 2;
try{
int qjmxBAWW = 1302177;
if(qjmxBAWW == 87051){
qjmxBAWW++;
}else{
qjmxBAWW--;
Console.Write(qjmxBAWW.ToString());
}
}catch(Exception ex){

}
}short LskWAK = -25323; 
string xsHSpKa = "zHtJaQRSNqeABD"; 
ushort cHRbAaQQUXlBaZQZeK = 43457; 
byte xKDueNsaCITVdQKSYWyQDPcFqROB =  27; 
ushort UjGGXoIwnyPspsGWjghfOkxuI = 41434; 
double mZRV = -1.159753E+26; 
mZRV = Math.Ceiling(Math.Sinh(-5));
int? kXpqFioGUlzsVzYuFQnbcHlT = 6055714;
kXpqFioGUlzsVzYuFQnbcHlT += 98504;ushort OILEMxFnRUiWuJbY = 14366; 
long seWgpNbFSBVQzEsbJlKCZHyAaFWQH = 76780974263486476; 
uint QLSNNWeLtcItN = 174989; 
double HyfMQlYaJLVzCXjhQTRgUL = 3.792677E+32; 
HyfMQlYaJLVzCXjhQTRgUL = Math.Pow(double.NegativeInfinity, 2);
try{
int TiRmzZCEbjykiPNgTsmhGsOIMICn = 69218;
if(TiRmzZCEbjykiPNgTsmhGsOIMICn == 89907){
TiRmzZCEbjykiPNgTsmhGsOIMICn++;
}else{
TiRmzZCEbjykiPNgTsmhGsOIMICn--;
}
}catch(Exception ex){

}byte jZPGWJyQPZGJpBwjB =  211; 
uint ubQpMqnCo = 35; 
short eiOTfLSRtluCRCaQIlixZgijzUG = -5940; 
double IHmNlsIqwfGwbPwP = 3.009052E+22; 
if(IHmNlsIqwfGwbPwP == 2.003781E+09){
IHmNlsIqwfGwbPwP = Math.Truncate(IHmNlsIqwfGwbPwP);
object qQnqcWibiAjlIBRA;
qQnqcWibiAjlIBRA = -8.158075E+29;
Console.WriteLine(qQnqcWibiAjlIBRA.ToString().ToLower());
}sbyte IbIeVgfjiqEGG =  43; 
short ywKllXMUN = -17395; 
ushort EEymAUxdyoXBL = 34595; 
string LhshWLQHISjKpNqJLMGGUqgLmoPh = "iCKVPjGUnyTNQGH"; 
long OLXnPYVydubHUHVJizqAPB = 21903523356522243; 
long aBCdkjuEPyMPFmtQ = 73588896543630559; 
sbyte zsJhFZJC =  29; 
ushort RyYdNL = 21731; 
ulong hCdAQOxEjQDnWZLbUHTMgcH = 53263589439000479; 
int udXojsQEfqQRSVy = 423; 
if(udXojsQEfqQRSVy == 466327){
udXojsQEfqQRSVy += 63019;
}int VdPKh = 50379483; 
if(VdPKh == 942288){
VdPKh += 803382;
}uint ONKnchbMRiSjiwbJwhwNYbkwIJRF = 50722898; 
float tBAo = -7.97017E+23F; 
ushort GtdVXBYszZUPwPYMDoGtqxdpnQR = 42960; 

}
} public class MAFMAhGYozhLAfJGeIFDONTjbFj{public void OHgKpxNAoc(){
double sOmRzJRpuLjilfhZHwKV = 6.448368E-15; 
if(sOmRzJRpuLjilfhZHwKV == 9.348023E-15){
sOmRzJRpuLjilfhZHwKV = Math.Pow(2, 2.1);
int[] koOTUsaKcEFBaoPW = { 3371375 , 16213 } ;
Random thxoVhAFJ= new Random();
Console.WriteLine(koOTUsaKcEFBaoPW[thxoVhAFJ.Next(0,2)]);
}string gksbiOGqxJMmh = "WCMdYXLKoDhpgn"; 
short bSHHxtiZ = -24934; 
string TXzLbOMEdXn = "fKyVPTTI"; 
short kIUH = 663; 
double sMXXHRMRRfCuLkmNIB = 1.870545E+37; 
while(sMXXHRMRRfCuLkmNIB == -1.623159E-30){
double MQRwmNaLkqmzTLeYNqmGSYVlg = -1.480398E-11;
MQRwmNaLkqmzTLeYNqmGSYVlg = Math.Sqrt(3);
sMXXHRMRRfCuLkmNIB = MQRwmNaLkqmzTLeYNqmGSYVlg;

}string UsWzReAMOEmPAnblexx = "VRNyQNkkDnyqSBzHSxN"; 
byte agiqRhDR =  123; 
double FyKdFzJeUojAfWWYP = -1.620566E-09; 
while(FyKdFzJeUojAfWWYP == 1.467712E-19){
double fYFIudoWNwQwbtOJoVR = Math.IEEERemainder(3, 4);
FyKdFzJeUojAfWWYP = fYFIudoWNwQwbtOJoVR;
Console.ReadKey();
}ushort tBNpzJHdtYhnfOhhsqbSQVBsII = 30825; 
long BSVCkbTmMEqwf = 36916347509941543; 
string bFcyawIQkXKSfmMUNoVGNLx = "lzPAUbzBzdTWiFSSeNfzwR"; 
double wTLXYxapiqk = 2.253682E+07; 
if(wTLXYxapiqk == 6.980429E-25){
wTLXYxapiqk = Math.Ceiling(Math.Cos(2));

}string PgRFjIYeYJDzTNaIgdRDQEpfSWd = "FeV"; 
sbyte mOudpQdmXSqEGbfYZk =  101; 
double ekQMMmFXAUNxUHZG = 4.10508E+23; 
if(ekQMMmFXAUNxUHZG == 2.35363E-16){
double mUXcGTageiDEsFEzpy = 2.617821E+16;
mUXcGTageiDEsFEzpy = Math.Sqrt(3);
ekQMMmFXAUNxUHZG = mUXcGTageiDEsFEzpy;
Console.Write(ekQMMmFXAUNxUHZG.ToString());
}ushort ARKjztVVXcMhfjRwD = 62307; 
float WXLphiYWkQNyPXTQjKDtwTBtp = -4.193582E-29F; 
uint ESPCEFCfucouYLg = 2714; 
ushort qblTuAw = 3979; 
ushort ZdczzHXXWkt = 22189; 
long InYyitdcLYixsZwbiCePwYmV = 76955907431767875; 
short kgBYUdmXtdpcoY = 3556; 
float RmVw = 6.990284E-38F; 
string ejMLDjifWOnCjHbCzGiGyOkPyfXN = "aCfyaQUBlQTgWt"; 
byte tSkktXeSyB =  81; 
double mxmxKWmhdiRhalPpzBwbAenNCbcy = -5.207976E-39; 
while(mxmxKWmhdiRhalPpzBwbAenNCbcy == -5.978553E+30){
mxmxKWmhdiRhalPpzBwbAenNCbcy = Math.Ceiling(Math.Acos(0));
int? QTdJslymwJNnYesPFtyJqhMaBE = 1267049;
QTdJslymwJNnYesPFtyJqhMaBE += 5479;
}sbyte lqyjCXLwYdDFtFNSnTOPHhbx =  34; 
string FbzhiRlGFbKq = "aCTbRZNTlJ"; 
ushort ouhepbDCH = 7843; 
ushort VtOgYiREDAOfoZFI = 17160; 
uint seUUyeCoagQI = 167686282; 
ushort ylHUdS = 52658; 
long GcQpxVnhOawaApbyUQyMRTWV = 11705600950449928; 
byte RcMYFl =  138; 

}public void giXeYVFyjbsYXNfU(){
ushort AHEAAfRtAZ = 47317; 
float ljqMOXPiB = -9.187035E-29F; 
sbyte WkidXSDoMVWIJSm =  23; 
ushort MnQgPGaqWIfZFyubjHk = 27797; 
string tWAnweffCEexwRIPwDcGWUiMqA = "HIHuZESVTH"; 
uint WlQfDVhb = 372147; 
ushort ItxXzOOW = 60844; 
float zUjeTEVPhUsgufP = 2.498168E+24F; 
byte LnmwMHgUyfTFneN =  85; 
byte QnzIFmpolcSK =  34; 
double AfkkAyzMKyJnpebHPEnaWY = -7.838518E+12; 
if(AfkkAyzMKyJnpebHPEnaWY == -4.860852E-05){
AfkkAyzMKyJnpebHPEnaWY = Math.Pow(2, 2.1);
object uadIHKKqyo;
uadIHKKqyo = 3.448284E-36;
}ulong elkJ = 56982955443970685; 
double VnUhBlHDWgou = -0.8749055; 
if(VnUhBlHDWgou == -1.155778E-15){
VnUhBlHDWgou = Math.Floor(-1.807876E+23);
Console.ReadKey();
}double iIP = -1.798617E+28; 
double OGdKXEPbwYZwqYGC = Math.Log(1000, 10);
iIP = OGdKXEPbwYZwqYGC;
double HqdLugftJdqCfYe = 5.385715E-16; 
if(HqdLugftJdqCfYe == -4.018712E-29){
HqdLugftJdqCfYe = Math.Pow(2, 2.1);
try{
Console.WriteLine(HqdLugftJdqCfYe.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}string LcLBxN = "sIiAugjbmERsFWDESzqaNqlc"; 
uint xuaHVtLXYeQUCxLNgTAOy = 1688; 
uint cfMcbxfcLKQbGIyHHXyOYWUoaj = 5529; 
long kkCOTnZUW = 37068402958101781; 
byte hQnM =  181; 
float kxcSdE = -0.03388625F; 
uint ARPjINaOdffgunJhBCx = 490579934; 
ulong NcDSeFgAqoplKxSBdmXIPadbOi = 63654321287070821; 
string eUWjWH = "SecZLJbtLDidMZBdVJeInPkD"; 
long MwipOLhllXMjk = 45574649837599105; 
sbyte QbBSZBdUAnqWGjuOyV =  2; 
ulong sLyKSFYIcPdFRsomSi = 85566312225045518; 
uint PcZPuazpdhhWem = 480421; 
byte OSeCGNPEBfPF =  60; 
long DSZPn = 56372626199304484; 
string PxGfwIqmymaRAsBzVBSDuzKZh = "XesUMaAKqsZglxkGCDntqCzLPBXD"; 
ushort NGYzKlVDCZs = 52559; 
string APnwqOW = "MPGwHH"; 
sbyte dXmZFthO =  59; 
double ZbobCBpkZmCkIwHTiToGn = -1.802016E-15; 
ZbobCBpkZmCkIwHTiToGn = Math.Pow(5, 2);
Console.ReadLine();
}public void wehdyDqOjJTag(){
uint EfecE = 3466; 
double caiALiISWdfsnLVNCDiLPWpgCdAg = 1.39641E-33; 
if(caiALiISWdfsnLVNCDiLPWpgCdAg == -8.679278E-32){
caiALiISWdfsnLVNCDiLPWpgCdAg = Math.Ceiling(Math.Cosh(5));
try{
Console.WriteLine(caiALiISWdfsnLVNCDiLPWpgCdAg.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}float sBflgpzmLLVujd = 1.008306E-10F; 
short ogUFEmSyiycPLc = 17695; 
string iJHgyqBulNHnQkIHpcOCNWT = "JhbjVZTGgoxLI"; 
short PtkqAljsmuhwZZCblQmyoppzl = 24659; 
string COXYak = "iCGthAPPQNFTGFnkUwSDAcNeGkbRA"; 
ushort uPbtAHxf = 62877; 
uint BLIZuDnDcDiJAfPBO = 455155116; 
double tEkeFlOEzpU = -6.22732E+35; 
while(tEkeFlOEzpU == -0.0004484704){
tEkeFlOEzpU = Math.Ceiling(Math.Asin(0.2));
int[] mnHBENchgYutYWYYcYe = { 4959191 , 1748 } ;
Random QhuzNuVaLaheXQuX= new Random();
Console.WriteLine(mnHBENchgYutYWYYcYe[QhuzNuVaLaheXQuX.Next(0,2)]);
}long eCbCiSjkzXNsuCOfUhOjSbxgQpxi = 56799169668380472; 
sbyte pVkKDOnQKxbodsmAeceVLSxaIy =  -7; 
long miAZ = 50432452449822590; 
string ZgnXgPQcpjkhyzjbfoD = "xnDocAuBWEwcKlxUuBsVmjS"; 
short UuuPfttpnjMFYUHwofNPhykc = -12790; 
long WIBmxmBhfZMQjbNxcKp = 78986699026041765; 
int iqzzOyJOXpqyNB = 567847; 
while(iqzzOyJOXpqyNB == 567847){
iqzzOyJOXpqyNB = 406701;
}string salZkgE = "qNUMWJm"; 
short FeJfuYYTketOCO = -18550; 
ushort Zzl = 50820; 
byte jOYFDaFNpXhhqtcYc =  68; 
sbyte bju =  77; 
double pVJOfItDzqwtwSGscQuHU = -9.640449E+36; 
if(pVJOfItDzqwtwSGscQuHU == -1.749014E+35){
pVJOfItDzqwtwSGscQuHU = Math.Ceiling(Math.Asin(0.2));
object BPOULEJSouKgFxsPnVJsgahdyX;
BPOULEJSouKgFxsPnVJsgahdyX = 1.165231E-33;
}ushort fsRPTQNnexOwkLBILxNqWqh = 30823; 
ushort GDBIfHkEbIpnFhAzyLh = 56767; 
short tjwLCgRPk = -9216; 
ushort jjOxqgjHKS = 21699; 
ushort FRyUypDptiplwfeNlyXUqYoLau = 1430; 
long DCTYSWotVOcRXQbyjVbeU = 13206354849446630; 
float oTHQ = 1.809999E-05F; 
byte iQtqKVcthnmQzZFQf =  29; 
uint ppNGbLNXqZtaWt = 284867604; 
uint MzBizo = 67532; 
int oQLCXSlSdNp = 160095836; 
while(oQLCXSlSdNp == 160095836){
oQLCXSlSdNp += 884241;
}byte fgooYuaVhSyfXfyIBoScdgakNR =  192; 

}public void ugpunjAiZCN(){
sbyte XdaGIsSLyBTbiQYgeChHLBbCdROH =  -57; 
long DKfgcZGfzUFXGxsfuBbVZFBlxcADR = 35299899102821722; 
long gzxJMfEweLMufmNaXjTLaBdnVJi = 67350009910431551; 
ushort VOgTIuRfjfjcBtijY = 60100; 
int hOdWxpZKKJCqoz = 40; 
if(hOdWxpZKKJCqoz == 355056){
hOdWxpZKKJCqoz = hOdWxpZKKJCqoz + 114938;
}byte LDYeFMS =  215; 
ulong nbwgLjxqwhwCFSNbqNyfE = 51080246266635287; 
byte SVSdndVKDwbLdOjYgqDwewIfN =  213; 
ushort jleG = 10373; 
short GNPAYxdULCQfkTDRai = 22638; 
ulong nMpxHRBppwDNjE = 44963250634708127; 
int LkjIwexpEgsbzcsFEEugmsaoqAiF = 715438170; 
if(LkjIwexpEgsbzcsFEEugmsaoqAiF == 144629){
LkjIwexpEgsbzcsFEEugmsaoqAiF += 354324;
}long HSVlYljdljTPlu = 82905415122288853; 
short WSySkbTgsWXLIihbiXdgHutzmZO = 12207; 
long lMCZpgPqTkqN = 13496728846043945; 
byte RkoCzZGkm =  48; 
uint LkjJSalReSQUWdaBLutZiYVqLOIQM = 96167347; 
sbyte yFmEGCpxth =  120; 
ushort GwYeBSQzRMpTYe = 23286; 
ushort yUJGA = 30272; 
ulong eTdDQRWWoilRHgfpQHTWLIxXZnKd = 62473182393825445; 
ulong bUtWJWfGypPtPSFRlxxgECgSkzc = 43435461736358821; 
double OZBRuQuosanttWZDHs = -6.823645E+17; 
while(OZBRuQuosanttWZDHs == 1.204159E-23){
double FJQUmGnzyWoiMNDSyPVnDMEWDflbb = 5.757353E+25;
FJQUmGnzyWoiMNDSyPVnDMEWDflbb = Math.Sqrt(3);
OZBRuQuosanttWZDHs = FJQUmGnzyWoiMNDSyPVnDMEWDflbb;
int[] RPHDfaNeyDHqiOa = { 5891108 , 37757 } ;
Random TNeVLxSeTsgtmxJooNUXWoqOhifjX= new Random();
Console.WriteLine(RPHDfaNeyDHqiOa[TNeVLxSeTsgtmxJooNUXWoqOhifjX.Next(0,2)]);
}short cibDdLwXD = 11354; 
ulong uSaTqcwUccEwpOCpJgoYPhjnHM = 38441233496357272; 
ulong epGDYziaFHm = 42321020062836095; 
byte NmGRswbJXRwZklsBNaqY =  44; 
long AVU = 34725688809879100; 
short aLGcg = -10021; 
int RqYTWVIPOmAclgVzapLLdGxC = 267136140; 
if(RqYTWVIPOmAclgVzapLLdGxC == 399501){
RqYTWVIPOmAclgVzapLLdGxC += 272597;
}float ckIFVCPcFZbpjoSUQgbRHWMkU = -6.732507E+35F; 
long GuAwIZazqEABJf = 78079411581621869; 
byte ajgAyLzDg =  160; 
double DUDpRwzEDqYQIHNeumP = -2.784189E-30; 
if(DUDpRwzEDqYQIHNeumP == -0.0001969517){
DUDpRwzEDqYQIHNeumP = Math.Pow(double.MinValue, double.MaxValue);
Console.Write(DUDpRwzEDqYQIHNeumP.ToString());
}long mydcmmYcLOSVHkhOQzMhg = 88440406310572048; 

}public void YXUYXkHMmXEYcYTW(){
long OynnDAExLAOUymSlQU = 33066975510713043; 
int OOLqLkW = 22526007; 
if(OOLqLkW == 408697){
OOLqLkW = 403838;
}uint gYsBpfRiYVZpRpjc = 89679943; 
ushort XPyjlpfafBGnOxafTnnd = 62184; 
int wMVfycIGBaSDsBinZZQdyp = 54775736; 
while(wMVfycIGBaSDsBinZZQdyp == 54775736){
wMVfycIGBaSDsBinZZQdyp += 177823;
}uint pLscbyqkJXyVemVUIGL = 29; 
string CnfasL = "ZzeULgj"; 
short LWPqTGDkzHBHyzBq = -16905; 
int OWyZABYZRkDOKsgZdBMnWMUDM = 5137; 
if(OWyZABYZRkDOKsgZdBMnWMUDM == 575875){
OWyZABYZRkDOKsgZdBMnWMUDM = OWyZABYZRkDOKsgZdBMnWMUDM + 562095;
}uint tDEnXoRUHMaVSRoqA = 828792; 
uint VbbVnoZyZmH = 23; 
short DMfkVuoaTHTtljBgMXlzBCtK = -32337; 
float IztRHCAONqGFWG = -3.058032E-27F; 
byte VOnzfVgZRpbtx =  51; 
string ICBQlSNwOPfspTzueoediiszKHKi = "tVwBZYAIyhcbSLqPOKYRIng"; 
sbyte Pzi =  -119; 
sbyte iEJuTKSpUZMIgcbpzbJ =  103; 
double sffPpAjApGqDTyjpyeWtdR = 1.407304E+10; 
sffPpAjApGqDTyjpyeWtdR = Math.Pow(5, 2);
for( ; ;) {
Console.WriteLine(8.779095E-27);
}byte bsLFmKiPpwUwsREPmdRK =  91; 
ulong gRwUAUkKNq = 52664600916062871; 
ushort wsftJGz = 56308; 
float DoldEOSPJUeLhdTLqV = 8.380818E+17F; 
float XIUPLblUwsSh = 3.731609E-24F; 
float pGAhJBc = 3.844349E-23F; 
string XnEZeLjqEiPcQDOz = "TZoKQiFmmMKxFkEKDoHSsNy"; 
int qZCEJGUbpQkMQgeYNSZBVYYksn = 1542; 
if(qZCEJGUbpQkMQgeYNSZBVYYksn == 120718){
qZCEJGUbpQkMQgeYNSZBVYYksn = 543699;
}long zjcJEfPuousXMhCUGUydXtbql = 13060600605817043; 
double VVeoTMCJESwCgHMOleHT = -1.026458E-26; 
double LKWNAGYCFFsxKy = 2.857986E-35;
LKWNAGYCFFsxKy = Math.Sqrt(3);
VVeoTMCJESwCgHMOleHT = LKWNAGYCFFsxKy;
try{
int fJhzFufDN = 7789476;
if(fJhzFufDN == 64689){
fJhzFufDN++;
}else{
fJhzFufDN--;
Console.Write(fJhzFufDN.ToString());
}
}catch(Exception ex){

}string mHAoywLnHnGWokmeCIWSPUy = "JTtFoqfDeVURefIh"; 
string aTzMnfHdXgLhZtiZGFbgYoidRuq = "awZOQTTEcWGnYl"; 
ushort RXWBIfBcfWXLLwVTOkdCgWqfAGWJZ = 49704; 
sbyte VjlUzQOUVWJUWhTIbMANhSR =  97; 
int aciRdsOkQBiawYyiHjnyjsNo = 26515484; 
while(aciRdsOkQBiawYyiHjnyjsNo == 26515484){
aciRdsOkQBiawYyiHjnyjsNo = 59522;
}ushort oIPLMdTpgLss = 31353; 
string CpYkHGhGJoU = "QetGNhQM"; 

}
} public class jMyEqYqAawQGXMdHSMNgbfw{public void TdkNeFjJpDsFBIpIRgJUdFSso(){
double xdFMdpHKmNEuwkhpwuAfjxfoUxC = 1.156299E-16; 
if(xdFMdpHKmNEuwkhpwuAfjxfoUxC == 0.0006961491){
double LQnnUYcUnNnKWnxXQTL = -5.47714E+13;
xdFMdpHKmNEuwkhpwuAfjxfoUxC = LQnnUYcUnNnKWnxXQTL / 3;
try{
Console.WriteLine(xdFMdpHKmNEuwkhpwuAfjxfoUxC.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}ulong KyXbtmaybO = 71221887292958508; 
short TIIGzsXdd = 6918; 
uint TgpjDZk = 97674039; 
long OExzmsTxRnBubQs = 77567390573654602; 
string ooil = "FnjkUbbUKMkjJUs"; 
ulong ukwjzcpbAc = 51985347570719647; 
byte RWpCfGkj =  61; 
int MpbVYQmfkUiuYJzNJBZ = 2092; 
while(MpbVYQmfkUiuYJzNJBZ == 2092){
MpbVYQmfkUiuYJzNJBZ = 462362;
}float dTRfPTxHeWhMufEbqICsTISDJm = -8.233602E-08F; 
int FyeWjmuOuCCcuRBPFiHdkPWC = 43; 
while(FyeWjmuOuCCcuRBPFiHdkPWC == 43){
FyeWjmuOuCCcuRBPFiHdkPWC += 533693;
}uint Zpq = 64179765; 
uint qIDnaQQnbbUJhOEEPBDVNlfqSYBc = 48; 
ulong gmpxJBW = 37286144044976021; 
byte NDjfDjeDlxbpNj =  138; 
byte DVzhUnFRLEnRNYFmHpXHcE =  13; 
byte xeUwyYxcRqAPoSnGnSHRLzyydg =  225; 
int KYTmnzjKwmFRPWIfzq = 671829; 
while(KYTmnzjKwmFRPWIfzq == 671829){
KYTmnzjKwmFRPWIfzq = KYTmnzjKwmFRPWIfzq + 430087;
}ushort XHysqeR = 30717; 
sbyte HDbudVTFbZVD =  75; 
short PxWEVnxSeBDldWAZpO = -13151; 
uint pdFfVfuwmwSgNCJVVlejKWTCKPJC = 600528983; 
ulong SJztqzahbyRfLfpu = 21806962026347094; 
byte wfyT =  102; 
long snecpEylXhmeOQFznLV = 45736770611683097; 
string KibjztRlsCCYpngYowmQHjhAzuc = "IYgouqmZlVpyOmWiukWbwz"; 
ulong bmmBaDLepeEtulFYahNtfPc = 55110002742004029; 
byte tnhdNZJ =  50; 
byte JKDOiH =  244; 
string LxIljTmbxz = "nPukbXxUnidiUItLLkWG"; 
uint czM = 5449295; 
double BXlMFTUo = -2.879313E+18; 
if(BXlMFTUo == -2.339583E-34){
BXlMFTUo = Math.Ceiling(Math.Tan(1));
try{

}catch(Exception ex){

}
}ulong KZQeauwuJPdeES = 58999743083511131; 
short aIz = 13116; 
ulong QOXxNGsoSb = 76801702109933167; 

}public void bMJpwMHLOetWLHuZohg(){
sbyte YNIaxutfknuSfLDgyC =  51; 
sbyte EjAflKpjDFfCadYapYqmGEQYeGW =  86; 
float GxYJuEiUkLcVJKopeIEghDtTo = -2.26596E-10F; 
uint HOz = 7097; 
double DsJwSwOGkKVgJOBnnMKcc = -7.326079E-10; 
while(DsJwSwOGkKVgJOBnnMKcc == -8.515065E+10){
DsJwSwOGkKVgJOBnnMKcc = Math.Sqrt(4);
for( ; ;) {
Console.WriteLine(-3.211946E-25);
}
}sbyte XmoyOnXPHzzuYWO =  124; 
short KWdjFecAiROmqjdhdJqajbo = -6295; 
double klhaHdJaGjtNJkuoffbtgn = -3.72049E+24; 
klhaHdJaGjtNJkuoffbtgn = Math.Ceiling(Math.Tanh(0.1));
for( ; ;) {
Console.WriteLine(-951.3692);
}byte nIxSHLkewMBtKWYk =  4; 
int eeZJTOKOOikZzMuOqmyFkwmIgL = 325887; 
while(eeZJTOKOOikZzMuOqmyFkwmIgL == 325887){
eeZJTOKOOikZzMuOqmyFkwmIgL = eeZJTOKOOikZzMuOqmyFkwmIgL + 48077;
}byte gwnZEeXXxWjNbIpiHYEYJKSsChst =  194; 
ulong iPDFY = 44503005051542381; 
ushort JNaRsfzlnyEWy = 27459; 
long jIRbYAdsKookpwLCVh = 52108963682054769; 
short wEVmIe = -28485; 
short wwjKIhdRnWSMbboX = -24922; 
byte lSqwjWXft =  9; 
int swbAMoVLP = 1553; 
while(swbAMoVLP == 1553){
swbAMoVLP = 389986;
}ushort HpCTQeHeZZNLbKPLL = 33031; 
short DDIkBzULVQODEFNMbtCqwuMoiZNIt = 14882; 
float ctZdIpsqjWjjlkhyXcJsYaFhQtcC = 5.896793E-21F; 
short URAjFKd = 14586; 
string gzkDdYRp = "pdDYbPSnonQGXGapwLXTn"; 
double qNUgWnoeKwCafqSzwMwPyjFiTklJ = -3.837642E+28; 
while(qNUgWnoeKwCafqSzwMwPyjFiTklJ == -5.888287E-14){
double IWZajgkx = -1.276775E-21;
qNUgWnoeKwCafqSzwMwPyjFiTklJ = Math.Round(IWZajgkx);
int[] mDgSkAgiMnMbHRQYfJ = { 3074284 , 24526 } ;
Random fIpNQxOnduNnonfaqpFgHecxujYtt= new Random();
Console.WriteLine(mDgSkAgiMnMbHRQYfJ[fIpNQxOnduNnonfaqpFgHecxujYtt.Next(0,2)]);
}short VSVWQqxxldOYZxNBQAIBDsaFUsG = -3216; 
ushort OhxsuwkeBZhLwbnywNkTDQFQtES = 20137; 
string LYaqoHDIcsoLYJs = "XZqyXSlDsXDGYREVdcniKdMHU"; 
ulong TERRWyDGSZEBSMzZseXJlO = 13024860652700453; 
long ihtYBfGixkLWACxxld = 9002187122245232; 
double nwMGoasLtBAlzOmad = -4.387614E-27; 
while(nwMGoasLtBAlzOmad == 2.121194E+18){
nwMGoasLtBAlzOmad = Math.Exp(2);
Console.ReadLine();
}double humceLibwUkBXRKjlJjkLdy = -2.716234E-26; 
if(humceLibwUkBXRKjlJjkLdy == -1.589564E-15){
humceLibwUkBXRKjlJjkLdy = Math.Ceiling(Math.Acos(0));
try{
Console.WriteLine(humceLibwUkBXRKjlJjkLdy.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}short ONNXbADLePjfUzJPw = 12789; 
long UbPcydmXAqODSmQJ = 85112698616240397; 
short YTMqBe = -768; 
float NXKBYlGkOaImIJZtNDXW = 1.342391E+20F; 

}public void NpeukiQKcbFcLM(){
sbyte zZdAsIT =  -46; 
byte XNDCUcYCWjTRucXqFldGoI =  153; 
short XDodexOkzlcgCgCd = -21076; 
ulong pIFbCbdImFyohjcXBKySNfHVRVjd = 17124538480117101; 
short NcbPBGbWUXAibtMWk = -20355; 
string jHSZCMcqwBgkFYXwQXGa = "UamFWBiuWxTRKqfkbmPELBuQoE"; 
string qgmFaPpHGwGkUX = "dnuVHSjOLRxjtHx"; 
short yMdQjoQVbzGH = 9912; 
int NcpCVJxEgtDNFGWaifmOh = 670997031; 
if(NcpCVJxEgtDNFGWaifmOh == 575049){
NcpCVJxEgtDNFGWaifmOh = NcpCVJxEgtDNFGWaifmOh + 1210;
}int FhcmMxqqUwLb = 92; 
while(FhcmMxqqUwLb == 92){
FhcmMxqqUwLb = 217267;
}short zxBwP = 3335; 
float wDqIQkCkBBSi = 1.373387E+13F; 
byte CNZXlmzsKBUhHHyjDYVsdc =  54; 
double lgatgAPiQTcEWh = -3.420377E-32; 
lgatgAPiQTcEWh = Math.Ceiling(Math.Atan(-5));
long iqHAcaFJYVeUPiiJpfiIgLOXOg = 22795159933191969; 
long KFfchYw = 37720666559915414; 
int aNyEJLWMD = 2451; 
if(aNyEJLWMD == 36816){
aNyEJLWMD += 311789;
}uint YMZ = 49; 
sbyte PXxmMFVswQR =  59; 
byte aTjqFKibXpEexAIDfB =  146; 
sbyte MoJaXtUDmjEmxYSQPVzgNZfJ =  33; 
sbyte LSLqidQnFBpHpn =  -6; 
long aSZCQN = 75864166316567941; 
ushort YjKCTwiHLmozniSnnaJPiUUf = 24776; 
long cPBw = 16115365920319574; 
int tQDySqWiNAisFGdoEhhsf = 188078657; 
if(tQDySqWiNAisFGdoEhhsf == 524530){
tQDySqWiNAisFGdoEhhsf = tQDySqWiNAisFGdoEhhsf + 430063;
}ulong AeNdfQdsJCugUcbmhslnPkb = 73943235010053153; 
float unwwnz = 5.092332E+37F; 
double HOklPiFHijCZndJRDoyIajAbAOG = 8.476791E+29; 
HOklPiFHijCZndJRDoyIajAbAOG = Math.Ceiling(Math.Acos(0));
object pUynjNaDNNoOmhXJqQ;
pUynjNaDNNoOmhXJqQ = -9.51334E+31;double MCxWcSFLWtnUWqmcJwGLXEOUxAM = 4.610568E-21; 
if(MCxWcSFLWtnUWqmcJwGLXEOUxAM == 4.620893E-18){
MCxWcSFLWtnUWqmcJwGLXEOUxAM = Math.Ceiling(Math.Sin(2));
object fVzIKZDeQHBWfIefBH;
fVzIKZDeQHBWfIefBH = 526.74;
}short ZlMTLtwzNFOqjlJEkhSIgulVAd = 30865; 
double EzkyPBLuOtMfhuLUoJoawl = 6.614832E-37; 
EzkyPBLuOtMfhuLUoJoawl = Math.Ceiling(Math.Acos(0));
try{
Console.WriteLine(EzkyPBLuOtMfhuLUoJoawl.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}ushort sPnHbb = 51840; 
float OXKPhfWcCtnGpplNlA = 3.237035E-21F; 
double ZXpWPPGOgfoi = 171.2532; 
if(ZXpWPPGOgfoi == -2.458448E-32){
ZXpWPPGOgfoi = Math.Ceiling(Math.Cos(2));
 bool? lphyOliqhzLHTwaME = new bool?();
lphyOliqhzLHTwaME = true;
}
}public void WIExFuUTotXDZXiTbq(){
byte hhVcsiVKqawLOIlKFcflYCMtGBO =  228; 
int oxz = 44; 
if(oxz == 514386){
oxz = oxz + 68124;
}ushort jDoYmOsLAoYkbXGx = 35704; 
ushort wacpMYlLkyWdYLNPZxutMxihM = 20040; 
byte gRskdlazgmDZbdg =  70; 
int aAniGZhdf = 833068332; 
if(aAniGZhdf == 420445){
aAniGZhdf = 764943;
}ulong tfMZNLoINUwsJoUFBp = 33653359288056896; 
string lJglmANiRlmPZBTfMMxJ = "pdfcEGiNJZHJHxeNjeiDMOncWkys"; 
byte mKtLVFaiJUF =  106; 
sbyte HfsePtDIn =  -95; 
byte QdgtiQCsNWpWBAMbw =  71; 
double HbwQTtETPGw = -1.079403; 
while(HbwQTtETPGw == 4.913161E+34){
double UPAKPwEpHBmZjVWOIpBPmIEg = Math.IEEERemainder(3, 4);
HbwQTtETPGw = UPAKPwEpHBmZjVWOIpBPmIEg;
Console.ReadKey();
}short LZwGTDg = -4103; 
string flWodZZxpOebRLJz = "pHIiFiCoz"; 
ushort tMlZbfqPtxMXCQVFeJhK = 62561; 
ushort RQtpBduaFF = 23205; 
string DfaMuTeQEJftJEyGpL = "YxuiIPRLZxhsiNqcYJxpd"; 
short nhnXRAWuoUJehhXs = 28457; 
int etxpWHs = 494235; 
if(etxpWHs == 398331){
etxpWHs = etxpWHs + 609273;
}long fbuRZlccSzBIiUapANVkfSRqqAXx = 5835777621329678; 
double EWbqE = 6.515083E-07; 
EWbqE = Math.Ceiling(Math.Cos(2));
object ZVtppXPaRtZu;
ZVtppXPaRtZu = 5.091998E-26;
Console.WriteLine(ZVtppXPaRtZu.ToString().ToLower());long xhelEctyPgsNeigXFeCEBbtJK = 1590658695806023; 
float LUCINaRkqsCXPejDfxepw = 2.724929E-07F; 
uint kTJpoIZkz = 59195; 
string PEJQbphubwFWMm = "EoVzaxCgiUcKCNqJgAKI"; 
uint FNftSjzeuEwqIiBFcan = 505703; 
double HGHLNyycGgWLitHFqJeBAojfhiDxK = -4.385067E-32; 
if(HGHLNyycGgWLitHFqJeBAojfhiDxK == 7.884362E+20){
double LHXBQkLzskxwWNwwA = -6.087468E+18;
HGHLNyycGgWLitHFqJeBAojfhiDxK = Math.Round(LHXBQkLzskxwWNwwA);
object FVatkbQbuUTtSXyEYA;
FVatkbQbuUTtSXyEYA = -3.162603E-07;
}uint xgSOEZeNLfPeNCLVRMHQURBnS = 44; 
byte CNwOoVDeshXdS =  117; 
ulong TEcqhYlkLXFGFVEDTsit = 69706663401837113; 
long ctMGZxwUmEBjcspWAyEujAo = 5832020400494010; 
long YUlUYWhhYeGSinfol = 14565263289225352; 
short oYLXdTJeyecBCwl = -17810; 
ushort EenahYYzIlwshEHzSjmgD = 64556; 
uint JjiRBFd = 1419; 

}public void NmjnQNZFlxKfW(){
uint dDcYngFFesXPgYciekOQunFtBh = 762159682; 
string EmKXxbasTmiJfWSY = "GinFUiJAPYJzfiQBFPKfjCahN"; 
short HMhIBDpsFQVJeayI = -29167; 
short NdshChiSJdhfSEJcOIqhVQRlKJf = 7035; 
sbyte UYPSFxoEHBsKInxKdfxXJZ =  58; 
byte WmgzSbDNWjjpZYlOkbQQliDPZGajh =  13; 
float GTIjheOzP = 7.823601E+15F; 
string tdgkjzYVu = "OKqzlbtGUoYLfpbGgmsaoyz"; 
int DUYXFWpkgUBRHDywqRP = 6604; 
if(DUYXFWpkgUBRHDywqRP == 174314){
DUYXFWpkgUBRHDywqRP = DUYXFWpkgUBRHDywqRP + 585463;
}short EtzpkOSKgwJosIBZxCPALU = 6464; 
double JoXFdQtmlNRIEodOSpaI = 1.521248E-05; 
while(JoXFdQtmlNRIEodOSpaI == -1.141949E+35){
double aKOAJDzUfdFpcgVJsSmMMSEFpf = -6.617689E+10;
JoXFdQtmlNRIEodOSpaI = aKOAJDzUfdFpcgVJsSmMMSEFpf / 3;
object KhDwkcchUNFTNmwsAqMA;
KhDwkcchUNFTNmwsAqMA = -1.217128E+12;
Console.WriteLine(KhDwkcchUNFTNmwsAqMA.ToString().ToLower());
}uint SfqiYG = 936860691; 
short iNz = -13449; 
string TYpwOOnTYFSwTTxEVXmbPbkYwUPz = "KnqRVNkI"; 
string eRyjtNAdxEGRC = "bgRO"; 
short TZQInqBEXglVREiksRRkWQsUGSNoP = 18128; 
double oeAebQVcmVfpqK = -3.259393E+35; 
oeAebQVcmVfpqK = Math.Pow(5, 2);
Console.Write(oeAebQVcmVfpqK.ToString());double VAgayygNAzbgWfCdnUPJmdhjcsHsg = 2.68793E+21; 
VAgayygNAzbgWfCdnUPJmdhjcsHsg = Math.Ceiling(Math.Asin(0.2));
try{
int lqLRlECniHzISmiDLpoZUi = 565222;
if(lqLRlECniHzISmiDLpoZUi == 62916){
lqLRlECniHzISmiDLpoZUi++;
}else{
lqLRlECniHzISmiDLpoZUi--;
Console.Write(lqLRlECniHzISmiDLpoZUi.ToString());
}
}catch(Exception ex){

}ushort mjALODfzqWPdQlunbNMt = 25064; 
short lCHzHB = 32722; 
double LSlmzRZZKO = 185609.8; 
LSlmzRZZKO = Math.Ceiling(Math.Sinh(-5));
uint SonyCZFuiLtMWGC = 840702779; 
uint VagFePulKdllVMNstEzJQXig = 429736; 
sbyte gCIpWUHyMKjIhOVpWAzZ =  -81; 
byte qtPYYSZ =  0; 
long NfhwVobfoGKuQBcbTjfYf = 67467613469435206; 
int KCXwINUbjxUUIzXEnWWZQU = 59197903; 
while(KCXwINUbjxUUIzXEnWWZQU == 59197903){
KCXwINUbjxUUIzXEnWWZQU += 98102;
}double fyKUZS = 5.655279E-31; 
while(fyKUZS == -2.794286E+35){
fyKUZS = Math.Ceiling(Math.Sin(2));
try{

}catch(Exception ex){

}
}ushort cULGngLdXkZAZZqifTkPsxw = 18487; 
long zfFnQKZssddxPjLfQnfki = 9007230744965647; 
int xXEJGAKuoBIHneKe = 8; 
while(xXEJGAKuoBIHneKe == 8){
xXEJGAKuoBIHneKe = 178603;
}uint WPNNKNuXMEHAkCeHg = 808381393; 
long VcNRbWBhasmAbMRMW = 68227575840329944; 
float ZGzmtNabNZVIRUQM = -5.042213E-10F; 
double PnVkinF = 1.578719E-38; 
while(PnVkinF == -3.361435E+09){
PnVkinF = Math.Ceiling(Math.Sin(2));
 bool? efhibuKoAus = new bool?();
efhibuKoAus = true;
}
}
} 