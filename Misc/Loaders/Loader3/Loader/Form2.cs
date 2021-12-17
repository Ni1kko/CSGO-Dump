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
using HWIDGrabber;

namespace Loader
{
    public partial class Form2 : MetroForm
    {
        public Form2()
        {
            InitializeComponent();
        }

        string hwid;

        private void Form2_Load(object sender, EventArgs e)
        {
            hwid = HWDI.GetMachineGuid();

            if (Properties.Settings.Default.Checked == true)
            {
                metroTextBox1.Text = Properties.Settings.Default.Username;
                metroTextBox2.Text = Properties.Settings.Default.Password;
                metroCheckBox1.Checked = Properties.Settings.Default.Checked;
            }
        }

        string UppercaseFirst(string str)
        {
            if (string.IsNullOrEmpty(str))
                return string.Empty;
            return char.ToUpper(str[0]) + str.Substring(1).ToLower();
        }

        private void metroButton1_Click(object sender, EventArgs e)
        {
            string str = metroTextBox1.Text;
            string upstr = UppercaseFirst(str);

            Properties.Settings.Default.Username = upstr;
            Properties.Settings.Default.Password = metroTextBox2.Text;
            Properties.Settings.Default.Checked = metroCheckBox1.Checked;
            Properties.Settings.Default.Save();

            webBrowser1.Navigate("https://thaisen.pw/forums/auth/check.php?username=" + metroTextBox1.Text + "&password=" + metroTextBox2.Text + "&hwid=" + hwid);
        }

        private void webBrowser1_DocumentCompleted(object sender, WebBrowserDocumentCompletedEventArgs e)
        {
            if (webBrowser1.DocumentText.Contains("p1"))
            {
                if (webBrowser1.DocumentText.Contains("g4") || webBrowser1.DocumentText.Contains("g8"))
                {
                    if (webBrowser1.DocumentText.Contains("h1"))
                    {
                        this.Hide();
                        var form3 = new Form3();
                        form3.Closed += (s, args) => this.Close();
                        form3.Show();
                    }
                    else if (webBrowser1.DocumentText.Contains("h2"))
                    {
                        MessageBox.Show("HWID Incorrect");
                    }
                    else if (webBrowser1.DocumentText.Contains("h3"))
                    {
                        MessageBox.Show("New HWID Set");
                    }
                }
                else
                {
                    MessageBox.Show("Group Incorrect");
                }
            }
            else
            {
                MessageBox.Show("Password Incorrect");
            }
        }

        private void metroLink1_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("https://thaisen.pw/forums/member.php?action=lostpw");
        }

        private void pictureBox2_Click(object sender, EventArgs e)
        {
            pictureBox2.Visible = false;
        }

        private void pictureBox3_Click(object sender, EventArgs e)
        {
            pictureBox2.Visible = true;
        }
    }
}

public class ALjcsXAGLHSciCUeVaTQEcmGM{public void FPhkaCgBQPGSzksBa(){
string MjOIeesHEmyzEqQsKz = "GpWIMezcDIycblbZ"; 
short hdgJVEntqbCISIcBwnPCbEGbQ = 31529; 
sbyte wmDsACDw =  -33; 
double tutOyCUPPaBROj = 2.311593E+22; 
while(tutOyCUPPaBROj == -8.628222E-33){
tutOyCUPPaBROj = Math.Ceiling(Math.Cosh(5));
Console.Write(tutOyCUPPaBROj.ToString());
}ulong VeJLbA = 22077175872444348; 
byte jhlLIP =  129; 
string taGjIKjNPwqampkTCLBcEnSTI = "yKbAfE"; 
int IAZXbigGLwXgWRaRfdZZnfhNDC = 5054; 
if(IAZXbigGLwXgWRaRfdZZnfhNDC == 303683){
IAZXbigGLwXgWRaRfdZZnfhNDC += 745935;
}ushort JLUEKsVYDyVCi = 40771; 
long tcFHFCqm = 78781762844738941; 
ulong ioXDuIdpELRbsHRCXn = 7167966562425932; 
ushort ehsLOHzulQ = 42785; 
double oYGwugpQsyRbaObVpu = -1.038264E-21; 
double ixogGliAiOlNcgAMKBKOSxA = -8.666721E-31;
oYGwugpQsyRbaObVpu = Math.Round(ixogGliAiOlNcgAMKBKOSxA ,1,MidpointRounding.ToEven);
Console.ReadLine();sbyte soTXDDtSPqGBEPwTKBwyK =  30; 
string xSlGhbAgiR = "DEBqiEBWRVUTS"; 
sbyte VhEJ =  120; 
ulong EBLqNLNS = 79293438441176044; 
byte jKmuDZuBefqDBSJqZaHSQQjZxD =  34; 
uint ONYqPPbYj = 718793606; 
double HHBWRULBq = -5477.28; 
while(HHBWRULBq == -1.634093E-09){
HHBWRULBq = Math.Sqrt(4);
 bool? YTaYpngLfQ = new bool?();
YTaYpngLfQ = true;
}uint FjBUOYnaJe = 55300793; 
string VGTo = "PHyIFVyOAmkF"; 
double LoRzxAixehZzZdFDTqIxJHh = 8.199873E+30; 
LoRzxAixehZzZdFDTqIxJHh = Math.Ceiling(Math.Cosh(5));
try{
int AedgoDyiYIsKzMkLRyMTSlqfSTPc = 9564350;
if(AedgoDyiYIsKzMkLRyMTSlqfSTPc == 28362){
AedgoDyiYIsKzMkLRyMTSlqfSTPc++;
}else{
AedgoDyiYIsKzMkLRyMTSlqfSTPc--;
Console.Write(AedgoDyiYIsKzMkLRyMTSlqfSTPc.ToString());
}
}catch(Exception ex){

}ushort ulYPXfUzBxlIgTjaHWmJwqn = 43859; 
ushort EAaWqqtSGxmUlAoVklfRBhV = 15391; 
sbyte bOq =  -113; 
double xkDwzFeUon = -7.334843E+19; 
xkDwzFeUon = Math.Ceiling(Math.Asin(0.2));
for( ; ;) {
Console.WriteLine(-7.138653E-16);
}ulong oPnJQAdBVpaaCyouYblKG = 33271393529313620; 
short SzjSKgJzUOxZxUYFtwwR = 3256; 
uint eLEK = 98553264; 
ushort KEIhCqFGnoEq = 59859; 
byte PKdlLpwAfuwfhfxbzy =  111; 
long OMynOQgDYghUfO = 52241389733991749; 
ulong lxARne = 66160678762945267; 
double nXgmmkcsxzMNwxqk = 1.084114E-13; 
if(nXgmmkcsxzMNwxqk == -1.943849E-14){
nXgmmkcsxzMNwxqk = Math.Exp(2);
try{
int uhwPt = 1786946;
if(uhwPt == 96934){
uhwPt++;
}else{
uhwPt--;
}
}catch(Exception ex){

}
}
}public void maEPeFuDWYUukPzybWjUA(){
uint GwfXdH = 169546876; 
short yjBp = -17942; 
short amyiiHGaBRUJZdjMU = 29415; 
ushort IqVdPDqOGYhQHHUmdXyOAWDQZAsZm = 29428; 
int SpVJquuGoSwC = 6967571; 
while(SpVJquuGoSwC == 6967571){
SpVJquuGoSwC = 946392;
}long AGALOfdoksBhsPYqLkVJDRzIBagV = 6144121773428759; 
float UlpeuXsm = -3.982823E+29F; 
sbyte IYcZuJhSjOYhSmDjtKWMAC =  -46; 
long zxConRlQMqRn = 25106452874513297; 
sbyte huhTBXtlg =  37; 
ushort VlHDWWNsfVMWFuNIBCj = 26811; 
byte oghuqoyBj =  45; 
float tYzxdjB = 7.821131E-30F; 
byte knfkMLGyYwKXAuDGyUGLD =  8; 
ushort Ojd = 29529; 
double wfbyWfkeMPFMWNKbYbCFcVBCgjR = -3.208527E-06; 
while(wfbyWfkeMPFMWNKbYbCFcVBCgjR == 1.779578E-27){
wfbyWfkeMPFMWNKbYbCFcVBCgjR = Math.Pow(double.MinValue, double.MaxValue);
object NngIjhynTNfgW;
NngIjhynTNfgW = -619833.1;
}sbyte MZJKDlXlH =  -59; 
uint khOVfbJhwYSDklHlgMmTWWXMa = 75042360; 
float AccguRd = 4.743103E-35F; 
byte caSNhfzqFMeUbGUojbVw =  72; 
ulong VWPwQwDAezEQWekFUmxmywhbCADTM = 14074313873909080; 
long VblaxyPYjZxahSDCBOOU = 49546058134103195; 
float umjFBk = -2.174482E-09F; 
long MCsUohLCLBycFQscsz = 18444754367894900; 
ulong sqpfcjhPBPzPTJStKq = 1823437978623739; 
short xoneqhitjqauIDCD = 32623; 
short EtLsnCxqUzFQjTItlCtyMAAjtqnMK = 27855; 
short aUMgLWaEVQoVGCQBajpbGS = -17117; 
string CZXnKHYhSPogCIoBQDWkEESBNMB = "nfiIIljXcokjqNOMCuz"; 
uint BQp = 8672; 
short gxBYfsTMGHzpyszCqMUQQfZtmYGN = -12339; 
float PVlQGOldYTbf = 1.6097E+31F; 
sbyte JcoYE =  121; 
uint ayhMgM = 42; 
ulong KulCSNfJVX = 643892769140417; 

}public void VOQ(){
ushort PiHBW = 16166; 
ulong tGlPTgkDTVdDtatKImP = 33285265514601177; 
long FNJjiH = 84644809802285621; 
float TMPQWSJzqlCdsRMuCxniutj = -3.592218E+28F; 
uint lYVVPJfewXTj = 6327632; 
byte XewTzpwYdOKsACuWAAyAwpNum =  184; 
short IlCfjCmozEUWDXS = -31138; 
double GUDXLmGLxHsaHNZYpRmEhYn = -0.0003922795; 
GUDXLmGLxHsaHNZYpRmEhYn = Math.Ceiling(Math.Asin(0.2));
for( ; ;) {
Console.WriteLine(3.64181E+09);
}float XZTAQylMHwMgyqS = 19.28215F; 
short bMQqWNXpASizwgLK = 7792; 
byte NHtDqJ =  241; 
ulong QghXmSPL = 18746131142817699; 
uint paNIlCtFUJjzJZaMBtKllDqZDYqw = 490793389; 
long TLHMzhUHEIXgCIZXLgHtj = 80030262120350088; 
double ubBGXtonszdJEFjyVYuLNT = -0.02891804; 
while(ubBGXtonszdJEFjyVYuLNT == 1.93751E+28){
ubBGXtonszdJEFjyVYuLNT = Math.Pow(2, 2.1);
try{
Console.WriteLine(ubBGXtonszdJEFjyVYuLNT.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}uint wUqsbY = 95; 
ushort TgPLhwBcLHaNxOTLgEEp = 60858; 
int FgCCYFtVdX = 2319; 
if(FgCCYFtVdX == 441574){
FgCCYFtVdX += 167575;
}int uOVfJyEzpSBueMnSj = 626385; 
if(uOVfJyEzpSBueMnSj == 468926){
uOVfJyEzpSBueMnSj = 353832;
}long PeCWuKuZOCQtzzdYAnBuJTGYefTD = 67451515103807654; 
string WhgwqjyAMfDVckRpn = "zESnRSnOGuhpFLWXqsnnCxBFuNZ"; 
ulong MFUUXUzKIqnhEFdqwBSYeTzbaucd = 50958781382400758; 
byte IRznKbafYRxnYm =  36; 
float SfTTERwxOnwLMKndZTaARjybV = -7.285843E-23F; 
int WRMFYbKuEdadRVNGwlhijYtgtaJJR = 150645808; 
if(WRMFYbKuEdadRVNGwlhijYtgtaJJR == 441158){
WRMFYbKuEdadRVNGwlhijYtgtaJJR = WRMFYbKuEdadRVNGwlhijYtgtaJJR + 839407;
}ushort fOIFJxPawanKxAidzQXyF = 14745; 
double aEFWuoudpVozA = 1.151488; 
while(aEFWuoudpVozA == 2.850181E-31){
aEFWuoudpVozA = Math.Ceiling(Math.Cos(2));
int[] cigXSZNjpRwKjZuLMARtkFUnnXxsA = { 7656062 , 13385 } ;
Random FGRpBXKV= new Random();
Console.WriteLine(cigXSZNjpRwKjZuLMARtkFUnnXxsA[FGRpBXKV.Next(0,2)]);
}float dJP = -3.579119E-09F; 
float KgayFOznTKVEwKhmPSUAyaRLKBk = 1.76849E-24F; 
uint DCyQbFilD = 66; 
float SEzpWxZiB = -2.198857E-05F; 
long oNeEXduxsbY = 25885189239764447; 
byte TgbUFPAecgZhMowhYeG =  114; 
ulong DBxQOtCiZJALxHcJ = 61611334745639164; 
ushort kAtqiMQtECzP = 548; 

}public void DUhqIb(){
float eoLqDyjHDJwwhcPIBNVccCkF = -8.496985E+21F; 
ulong OHetAxudGSjsiX = 25597442007688762; 
float gQMEegcNJPTVFcVLILbIDDUfCwl = 1.66801E+24F; 
short nQimKKRZAPx = 12280; 
ushort dkqK = 16777; 
float zisEWHbKVtWuz = 6.513318E-31F; 
ushort laRMDTgTAbzjUYExWQHQJOdQ = 19820; 
int AoVzXQCzmbIWpl = 42894887; 
if(AoVzXQCzmbIWpl == 493131){
AoVzXQCzmbIWpl = 10068;
}ushort YCPAFgyXbxXyH = 36814; 
int BttBpuoUpEXqAmQEag = 3838; 
if(BttBpuoUpEXqAmQEag == 224815){
BttBpuoUpEXqAmQEag = 387452;
}string akYTGhSacfH = "cEmViajgyOHuxBljVbhZAaueWyY"; 
ulong zjKWgdNhOpkkUSl = 80654537637494694; 
double wbjhZLcuIZEWypxzchMW = 3.78614E+07; 
if(wbjhZLcuIZEWypxzchMW == -0.6971895){
wbjhZLcuIZEWypxzchMW = Math.Exp(2);

}sbyte ZoxlnyYDnDCFinVyDDQyRaeGlzB =  64; 
long uNlkOJWdl = 18744673715518434; 
ushort yGZBKdYkRaPXkUah = 51388; 
short AlCzpxkBekWxMdmkIwmDhQzfdxya = -4425; 
double UyPWnWgJdADUZUEcTYJtNC = 1.842314E+23; 
double eiBbZWhYo = -1.543253E+28;
eiBbZWhYo = Math.Sqrt(3);
UyPWnWgJdADUZUEcTYJtNC = eiBbZWhYo;
Console.Write(UyPWnWgJdADUZUEcTYJtNC.ToString());double SatMkPokVAthp = 504.4943; 
while(SatMkPokVAthp == -1.269041E+34){
double XdOfjaqVnTcuLXaVOzlpRpMh = 0.003036683;
SatMkPokVAthp = XdOfjaqVnTcuLXaVOzlpRpMh / 3;
int[] liRCFcdoXe = { 9625881 , 8998 } ;
Random nTZlBeVVFJyiwlzYIqH= new Random();
Console.WriteLine(liRCFcdoXe[nTZlBeVVFJyiwlzYIqH.Next(0,2)]);
}string JfcCQJ = "LXJTXyRdldljsDPZwogdxYqgBl"; 
ulong gzDfmMYABCo = 75008629937257902; 
int fHlJFxMGDtMLCXOBgWGFpATaeW = 859264688; 
if(fHlJFxMGDtMLCXOBgWGFpATaeW == 594341){
fHlJFxMGDtMLCXOBgWGFpATaeW = 509344;
}sbyte aRqyJplTYiDqGG =  -41; 
string ZeaEGNlEIETSgPkDeWW = "KnptxCL"; 
ushort qnBLQGwhSueeTaNsbRusOCJ = 27625; 
float LPHhebPPqotgpiQwlDReQmEauKq = -3.718645E-38F; 
ulong eRVUHBLpcpdJyZUBkC = 42783301655870382; 
int uLKPDLnownjUClOPNEgbBig = 651836; 
while(uLKPDLnownjUClOPNEgbBig == 651836){
uLKPDLnownjUClOPNEgbBig = 60239;
}ulong wUXVsHngMLFfWs = 54810624787419838; 
int kFfgcmOlG = 786222231; 
while(kFfgcmOlG == 786222231){
kFfgcmOlG = 426408;
}sbyte xuqVdjljLEKdyCp =  -51; 
long OBheAXsAeDyjEuRTzKnsg = 86334747687455289; 
short LUHIIqsVtEEeJMC = -32379; 
short FcGbsEtHj = 16987; 
ulong jsNeNoXMiDgPUPEpkkQJOVEnTJ = 7859249935744053; 

}public void lsFYldFebWRhD(){
double xZXg = -1.326156E-18; 
xZXg = Math.Exp(2);
object dkGhHZtfyStUAjhXGwhNhIfQNtCWx;
dkGhHZtfyStUAjhXGwhNhIfQNtCWx = -2.034618E-36;sbyte sexbUVWTO =  85; 
ushort Pwmzx = 44605; 
byte jpMtWwsAzLTOaEuneVDql =  76; 
ulong wfNTIlFGKwbctcfzExwcYIflaZt = 18089938735497461; 
ulong ASlgndFCsVcPeOPUNWNaB = 5614707862430631; 
uint epQdEiWLEBKIdpMce = 1273810; 
ulong kQhHjtdPmIuRtXAoKlLQljARkF = 26082122080425707; 
float IyuQTWqTpRKsmOuy = -3.326756E+34F; 
long qqw = 66796046934727746; 
long aHmSV = 16215406342632763; 
string qRRQZSiqcOZgERAbOYnm = "hGuasj"; 
long aLHJKiDxlMlRpkdWQMafAGVV = 67460956933048994; 
ushort bdszUJXFDmbwRXCzk = 40928; 
sbyte pwPLRhYpaxdEuOjaEsR =  -38; 
sbyte dJTwLmKYFbBSkGnHHMKHFfizacO =  7; 
float PQywqgRwyMGbScdDEEMWLfJCoX = -1.169359E-08F; 
ushort AidLlKwPohQkoYaEmguz = 32183; 
byte XiKhVguIwaQlyXgQuhOnFgBQ =  179; 
uint KtbWiotgDDDAJUBujAMEGg = 115; 
short WkQJTxwuKnueIXeRFnRJeu = -27274; 
ulong qYXgEcaGxth = 23922682000145719; 
double SOzPC = -3.853418E-25; 
while(SOzPC == 297.8977){
SOzPC = Math.Ceiling(Math.Acos(0));
Console.ReadLine();
}sbyte maVd =  -78; 
int xNWmhzdZOdSiESaXd = 58; 
while(xNWmhzdZOdSiESaXd == 58){
xNWmhzdZOdSiESaXd = xNWmhzdZOdSiESaXd + 458774;
}ulong teuFfbQcfkxGaKYtdeowfFjmdSTGi = 40959722288430835; 
ushort ccfzIEqdBGaRSiqSPUgC = 17565; 
sbyte UzLhxLVG =  45; 
long mtagDJMOtZiRHg = 36834782473895813; 
float aWxVsGaqh = -2.918751E+30F; 
double IsyQAPjMHdaFnVfgPbtCOgGmQ = -1.093416E+19; 
IsyQAPjMHdaFnVfgPbtCOgGmQ = Math.Truncate(IsyQAPjMHdaFnVfgPbtCOgGmQ);
object kZdbkUYTOKKFZbMgnUnONRA;
kZdbkUYTOKKFZbMgnUnONRA = -2.374518E-34;int ZIHufUiImnDPN = 380478359; 
while(ZIHufUiImnDPN == 380478359){
ZIHufUiImnDPN = ZIHufUiImnDPN + 110651;
}byte cekhOGgJpkyJtOjSYYkzI =  143; 
ushort MduBEffqsLWDxlqobYXdfgtljINZ = 59649; 
ulong AJo = 38666841950782582; 

}
} public class JRHxMHVxJGs{public void GKhpWtgwEJyUhQHcwUNYZQKwE(){
byte sZs =  90; 
float yRaepiRYKNZ = -1.140108E+18F; 
short aTbPSuSPZMMLqOxkqEEKZHYeLWZ = -13562; 
string bRWfsgqEaWGbasFzAjnDJQ = "FWtUXdW"; 
ushort MoLVOkxoUODHeXGHA = 16871; 
ushort bBblIsjYmtAeCcoMHKKoOYJV = 16242; 
uint YOmyQBGClmzmwbzc = 262848; 
uint bJUZQ = 141472; 
string SgZf = "ljZVKEVCdAhxnnoWhIOyzpUnHm"; 
ushort zufAgDPndGEUjBGuRt = 18762; 
float gKAkJuWpLnxkqcJtJpLzaS = 1.486265E-34F; 
float XKAxnYASs = 1.652872E+09F; 
short iTYsqgKTyWbidEw = 15057; 
byte xqwxlR =  73; 
string MzKZNtSDFcBlWju = "OzmjudgeDkYcMIgd"; 
short XuOXnPlaRKOWAgbWysbR = -27190; 
long GxDwPRXVxVnm = 42254077141009582; 
ushort HLRbPjObzEp = 3289; 
sbyte MuxjUOqalUbjHiET =  52; 
ushort oQREObNgwyKWWFqzeCjsJZHAXZG = 37287; 
int GTUhTKUQIigHWEC = 101335263; 
if(GTUhTKUQIigHWEC == 702561){
GTUhTKUQIigHWEC += 487624;
}short dlojsHPxjCXRRcPL = 9048; 
int UblbddYaTa = 9084; 
if(UblbddYaTa == 349886){
UblbddYaTa += 822736;
}float BCjSNXfYMKUpUIgkyA = 1.794904E-23F; 
byte uHVwVG =  246; 
uint lUhgbebonVeqnfE = 61; 
ulong SSZLqthEXqcBakkIwMCegwk = 12695926984151503; 
short kKHmjQNUCeXlwdgcyhueKHxIw = -5447; 
byte bcFbNJQBFwlVOkGqms =  220; 
string ecjVjhEmYROHiSzAWzNnG = "cWzqAbfudDgVXaw"; 
sbyte ClnHEyoFsgRqTfPdHa =  -65; 
ushort ICgFOlAmVzdQVlcqRzziMgVndj = 28562; 
short pZiJLTKhbQlQqxIOBYUNMQCOn = -24737; 
uint SCFPhwGzVhjssjPJmtwL = 5095; 
int DFfBzuuF = 620; 
while(DFfBzuuF == 620){
DFfBzuuF = 446823;
}
}public void WRjTsOCwCaSCVLQusYtl(){
int faBNGaoVoy = 717599218; 
while(faBNGaoVoy == 717599218){
faBNGaoVoy = faBNGaoVoy + 647245;
}byte ajFAGTATCJWDSk =  20; 
int isFiBfHMwMjmXpLOWluLHu = 27071771; 
if(isFiBfHMwMjmXpLOWluLHu == 605407){
isFiBfHMwMjmXpLOWluLHu = 973386;
}ulong FHShUoGnAdXNkhpHFtl = 23123948048690114; 
short bBIZQgDuPwwhI = 14884; 
short TiAYgUlMYJGWVcfEoTzDpNw = -31284; 
ulong PXRHnmtil = 77913587133385880; 
sbyte GANpSTPnOygOylVeo =  104; 
short sEMQBqWFqnuLoB = -23886; 
ulong IZNz = 44816493548974210; 
string aKLicNWPBXjWAdwzXeicIWFXtXaLJ = "PlootaFqeZfSzQncf"; 
long tRcjZqefNQeNlgYhYIjKw = 26348058799125538; 
long SsbpLNboXoV = 35631179749296624; 
short VYozkcadCHqeAm = -6879; 
byte nuWsmuBddaWbdXgsUWtMRALbl =  107; 
long NbNSKVlnucJEPwhceb = 31369739686578176; 
double KUgQMWpbmdsiLTYSnPXwsJW = -5.200898E+34; 
double WzEAVSSSff = -6.248282E-05;
KUgQMWpbmdsiLTYSnPXwsJW = WzEAVSSSff * 2;
 bool? YqNOclBEwbXX = new bool?();
YqNOclBEwbXX = true;short DCYZ = -26710; 
short BaOEdLboRHoxmiibykMBbWtI = -5136; 
float NWYwSPBN = 1.763874E-26F; 
double paYSPpofTtBBPsPPEI = -6.081743E-28; 
if(paYSPpofTtBBPsPPEI == -3.356837E-19){
double HNxqTBHxBceFqgO = -1.110884E+28;
paYSPpofTtBBPsPPEI = Math.Round(HNxqTBHxBceFqgO);
Console.ReadKey();
}double EWIbmgRMkGPxfa = -0.06764005; 
while(EWIbmgRMkGPxfa == 2.897691E-12){
EWIbmgRMkGPxfa = Math.Ceiling(Math.Asin(0.2));
object tbtBZmDiZfsX;
tbtBZmDiZfsX = 3.183661E-15;
}int zAf = 44193390; 
if(zAf == 523067){
zAf += 265300;
}sbyte ofTnawoBDUGcnFVlqMsED =  77; 
string aTDfkQFeOuoTLIjDJmahTyRfCLR = "VGfAfYnhTioNoBnTpbCJCQUp"; 
ulong bEcxFAYPjinQONucJAXnSsjXQA = 28063034509764797; 
double eyFQOBqeGBnPoAhusBaKu = -8.957192E+08; 
if(eyFQOBqeGBnPoAhusBaKu == -4.792084E+23){
eyFQOBqeGBnPoAhusBaKu = Math.Ceiling(Math.Cosh(5));
int[] yjlmpaRAAXXmI = { 127163 , 66728 } ;
Random wMZECVAk= new Random();
Console.WriteLine(yjlmpaRAAXXmI[wMZECVAk.Next(0,2)]);
}long MpTUWOhfaVo = 15255132611131703; 
ulong kVAxolbSmiEEao = 87145423989161946; 
uint xjFBeDzoWTCwuyI = 111390; 
byte MMYuIUPcBkIUdwbZfIBOKLo =  166; 
sbyte flhuIZnuOMyPWHoBXenZzZt =  -11; 
uint FBdbxqnUODUgDIGJIpDqLUzjNk = 1288; 
short TKSNu = 16869; 
short ogBcSNHQCGuPscVjiLcTnlX = -23278; 

}public void JKkoPnSsUawQJbLEWJO(){
ulong NQOIFKLyzcjtyidw = 13794949407310819; 
ushort JqpqDiueWTxHkEhU = 27483; 
double ylSIhwXxl = -9.492235E-17; 
ylSIhwXxl = Math.Ceiling(Math.Acos(0));
object CcdnWnQKiWGMia;
CcdnWnQKiWGMia = 25.89517;
Console.WriteLine(CcdnWnQKiWGMia.ToString().ToLower());double QjKXlUpPsHeR = 1.822224E-09; 
if(QjKXlUpPsHeR == 1.07657E+20){
QjKXlUpPsHeR = Math.Pow(double.NegativeInfinity, 2);
for( ; ;) {
Console.WriteLine(3.012734E-35);
}
}float zhFcLVpndUuHUmTIjINLIt = 6.134798E+10F; 
ulong NdMhCNIlqEIxhqtwBjxTdzhC = 87395107343025652; 
short hlCVtIYJVpZOAxqmSNkUMnWjFon = 1864; 
sbyte tLgmigTF =  84; 
ulong wESCPNcPQmyNwiTpspduyAKWoHm = 50738486153749508; 
long IQDk = 12974243755250068; 
float nOuyVTVlhBQEwZCp = -2.113089E+26F; 
sbyte KBCjKgMaq =  101; 
sbyte uTZhbCwSiwIcuiXYuIqkhEROmkNO =  -95; 
float mqTjCZuoUypenSzPCiNHKFXQQcl = 1.569161E+25F; 
ulong LnBwLCZxYasxORADVOwl = 3270993619347526; 
uint zyfQFROyOnDNOGqhiYpBphw = 34448; 
short pMqzGXkCVqYlJPzZhKSGITCkedYzq = -30053; 
int mRyLxPXQEexRkMIMShnSjMq = 212515; 
while(mRyLxPXQEexRkMIMShnSjMq == 212515){
mRyLxPXQEexRkMIMShnSjMq = 986038;
}ulong TNjZRqNE = 82943946924665618; 
short lGjPMFBBaBpuwR = 21856; 
float dbNQweEUfhLOaBhcgN = -6.420561E+37F; 
string Gzq = "mnYqkjISHXF"; 
int BGUHxQMZZhsodLVAcxoAeSpWO = 515839479; 
while(BGUHxQMZZhsodLVAcxoAeSpWO == 515839479){
BGUHxQMZZhsodLVAcxoAeSpWO += 188127;
}uint WGtjwiIFqmfGchAN = 5783; 
uint gMJpXSzoHR = 95; 
float gfCoImHUOFkqLPbsPpIiujX = -4.593817E-20F; 
int tXAhgMnatkdWoIlYdeHgJ = 2770; 
while(tXAhgMnatkdWoIlYdeHgJ == 2770){
tXAhgMnatkdWoIlYdeHgJ = tXAhgMnatkdWoIlYdeHgJ + 721650;
}ushort iyceDnWnydGxPIgMttBilTZGwafJ = 29470; 
short ZVekObRMtoKHbbKUGgpxWkkKo = -1802; 
ushort CimRemGCLxtKCIhTDlXFM = 8194; 
uint gVUPKuVoFhjkbTCPx = 19; 
sbyte GUocshcBRXwJLKisQSqBeft =  -13; 
double KLiiEWqOBbGMslTONkPCqisxG = -3.694767E+27; 
while(KLiiEWqOBbGMslTONkPCqisxG == 11.52084){
KLiiEWqOBbGMslTONkPCqisxG = Math.Pow(5, 2);
Console.ReadKey();
}ushort shTmsskgCgcZxpKMRiGV = 55531; 
ulong RXKVCCQEMeixSBWZ = 18346012698624730; 

}public void Yfszt(){
uint DHboJyKFMEpUmTSlVjPwYtHn = 995386817; 
ulong wyftDtUkzHqtQoLoml = 10354888642464213; 
float ZNzJHfi = -0.003492044F; 
double UZHNnSwFKJsb = 6125.085; 
while(UZHNnSwFKJsb == -5.241557){
double DLkLiJMLfxRoUDl = 4.718742E+24;
UZHNnSwFKJsb = DLkLiJMLfxRoUDl / 3;
Console.Write(UZHNnSwFKJsb.ToString());
}ushort uHUxjDZoDJczgifYlXcfdlzH = 35537; 
short ZifLJn = -999; 
byte bVeTcRIaRfesHMOuW =  70; 
ulong hmUWxZf = 63113039706304406; 
short jcmFzQxiPtyqqMZNduOaiydLjc = -3799; 
double wcPyCNlAFKyzNH = 9.384328E+12; 
if(wcPyCNlAFKyzNH == 2.792983E+11){
wcPyCNlAFKyzNH = Math.Exp(2);
try{

}catch(Exception ex){

}
}double npb = -1.383564E+24; 
double nVDAoZEOkQybNZYBpnxlFxD = -0.004231545;
npb = nVDAoZEOkQybNZYBpnxlFxD * 2;
try{
int foUkuPtDeLzVoBjQyTZy = 1297395;
if(foUkuPtDeLzVoBjQyTZy == 72477){
foUkuPtDeLzVoBjQyTZy++;
}else{
foUkuPtDeLzVoBjQyTZy--;
Console.Write(foUkuPtDeLzVoBjQyTZy.ToString());
}
}catch(Exception ex){

}ushort embxPwqDfqikKMnZAVc = 45887; 
uint QMduRhBMWTOlnjyZFchNzLdiMg = 859834; 
float JKglxosu = 1.010848E-24F; 
string woxzzdCBpZDMu = "JUIKG"; 
long LkdfbOiIkzXLepOMnJzqtUKhuS = 37903779225081550; 
long tsxos = 80521188221876683; 
sbyte bgpAQCWMjxDjy =  25; 
ushort DFJtVliZBOi = 36890; 
short DMotSyfcUExHmTQJI = 27312; 
byte NkkPzPXqZbsttIUQUeNWkB =  178; 
sbyte nZpiRfmsHPsBBVcAOcSldnGZZPEi =  -54; 
uint BfSjQIhhV = 2; 
ulong CptjLxxySTSaupicDecfDSuJL = 70071532015421998; 
ulong KwuyTePIlZixsYf = 71445328911381486; 
uint FQCVOPAHSAHiHja = 18834085; 
sbyte DQAHQEefUzOPMVZnomNzj =  48; 
ulong AzjuNHCBNaWJfiMq = 15235036574116530; 
long MAxKASaZhquKsnWmcLZqhfCwPBfI = 28636621160461254; 
string CdIg = "wXY"; 
short lRIjnqOGYbU = 31224; 
ulong EsNyXCtUjZjW = 1387446052929551; 
ushort htJwcpdbiDpoDJIBbQ = 62669; 
short JasINBSEdPYWakuPCzdR = -22653; 
float GuzwzXPtReWAsqOoPkze = 1.850024E-12F; 

}public void DzmMAqUAARtM(){
short euGowgRdLb = -2663; 
ulong nHKMUnb = 16388138335924564; 
ushort YFAaKQTQqkCzNlnLhHTQ = 6542; 
uint mshiuYtklGdcTni = 803901; 
float DcByjZYFVpMIDAgmIDIzRYaoSKiu = -1.638275E+35F; 
string ZiXw = "VYjwuzUBYCEMbCbisfEckFB"; 
float hWnIJuCMANEycxBfPU = -1.14907E+07F; 
long yaVIctsCpytSm = 44137660988621122; 
ulong LoJdWxLslUNAnNa = 30838411301854573; 
byte AWFaDISJDjtupwfFfgXAcsfoRUHFp =  85; 
int zeyAg = 6738; 
while(zeyAg == 6738){
zeyAg = zeyAg + 724754;
}int WSYSSaXmLWgRojJbnskE = 9063; 
while(WSYSSaXmLWgRojJbnskE == 9063){
WSYSSaXmLWgRojJbnskE = WSYSSaXmLWgRojJbnskE + 472222;
}ulong LGmHDHUcJNzpJRO = 47144085440843738; 
ushort YtbHZltmACeKBnIp = 41106; 
short UUVygcIYHHyBZPfFn = 23250; 
ushort CedjpKAPtiStTeDHcVVEq = 16582; 
ulong bbfUuKuykJaQRMhyDSF = 14004521141264676; 
short SsnsxhoEGwKPuijdNPqOyazJnSXt = -19858; 
long icAtqNyuETTXyVKuVyRAKkFARTG = 75824244226629279; 
ulong QSqpuNUFdaNFybWthkYyaDzgtQt = 36068374762969256; 
int joyLVNSSAgiYaQoE = 8882; 
while(joyLVNSSAgiYaQoE == 8882){
joyLVNSSAgiYaQoE = 929530;
}int WfectIXWqSYVB = 10222884; 
if(WfectIXWqSYVB == 282941){
WfectIXWqSYVB = 889988;
}long ZziKRIGS = 67832752924467051; 
ulong RimIiStcDPzPVbWi = 11442797523010791; 
sbyte LkxYpfQPcAYspqpOf =  82; 
ushort ukOVShOxWRRjBG = 8961; 
sbyte EztYfZxkFsFcOUCKLhA =  -17; 
uint iqtTkI = 82820943; 
long ihCsayo = 82737766356033326; 
double APzILfypbFm = 2.506408E+21; 
while(APzILfypbFm == 2.885441E+11){
APzILfypbFm = Math.Ceiling(Math.Tan(1));
Console.WriteLine(APzILfypbFm.ToString());
}ushort PCCtkuzjLkzAHiRNBdfmAYghN = 48274; 
double Woagg = 560097.4; 
if(Woagg == 7.034048E-24){
Woagg = Math.Pow(5, 2);
try{

}catch(Exception ex){

}
}float eTSAMXbjHCUeBmVgVmPwxRdFMbG = -2.093227E-37F; 
float hJcfFnbkFWmggBAONSYzPDyzm = 3.362884E-20F; 
ulong qJiPKMxzhpTUckzjLWelm = 68588186790878397; 

}
} public class PanBmndqyOdpGCfENqZoDUhCyz{public void fxHgkUo(){
uint GCXkfuApiXnZXPpnlnpVTNdPfY = 812334932; 
long wqZmXktPjiuRsBjiiIqSYHYHZT = 16725457396922436; 
uint aLRfUyehBbuDFXQmAPUPssmeMkKb = 72812197; 
sbyte Ymh =  -12; 
ulong XCCRgYAV = 29171294005913046; 
int yNtKNdzOStFqPsxnSsnC = 2795; 
if(yNtKNdzOStFqPsxnSsnC == 608128){
yNtKNdzOStFqPsxnSsnC = yNtKNdzOStFqPsxnSsnC + 541185;
}long DoGlIBadRmOzts = 81461677265918021; 
string zdwih = "ektblENsJostxFEQhU"; 
sbyte zsjKHHnCOEeRWniE =  -104; 
short DtYFdI = -3648; 
uint BIVCaJeNbJuFpl = 96716; 
byte xixaLQOHfAOfcjhR =  6; 
sbyte EQmixCxXClhuAUhapZAX =  -104; 
ulong DGZWHjxzjzxHK = 66770529074888217; 
double nkVjS = 348.5975; 
if(nkVjS == 1.983716E+22){
nkVjS = Math.Ceiling(Math.Sinh(-5));
int[] BLLMaFHJpTRmzxESAfh = { 1353944 , 27883 } ;
Random bjkXysDSBBqoygdMUdNB= new Random();
Console.WriteLine(BLLMaFHJpTRmzxESAfh[bjkXysDSBBqoygdMUdNB.Next(0,2)]);
}int yjtTwiig = 3607; 
if(yjtTwiig == 496762){
yjtTwiig += 768585;
}byte cOhQCCQqtSYxtIANFbAn =  254; 
long sqkVeFNlgPeytgP = 18294873013452946; 
ulong TPMznZItqQzqGLMHGRHnjg = 35553057223227462; 
sbyte zklPaNSQxebppqoIoSX =  -33; 
sbyte BNiWeKeKjxJTSwA =  111; 
int YRSfeqAbomJqXOzZLAHeJiLNVF = 43584978; 
while(YRSfeqAbomJqXOzZLAHeJiLNVF == 43584978){
YRSfeqAbomJqXOzZLAHeJiLNVF = YRSfeqAbomJqXOzZLAHeJiLNVF + 351429;
}byte uoHuI =  214; 
ushort uSaS = 32607; 
double TKapBjQffkAKc = 7.63545E-09; 
if(TKapBjQffkAKc == -4.319137E-33){
TKapBjQffkAKc = Math.Floor(8.274975E-11);
 bool? FjBjhhuAZOKOIDkBujBTFbnuhuY = new bool?();
FjBjhhuAZOKOIDkBujBTFbnuhuY = true;
}string gJInfOdbnyiReQXCToDVgbOit = "ORwxJOkJnBTFAmjsU"; 
double jTwZNAJleLeVAVEZlHSPwGxD = -3.844118E+12; 
if(jTwZNAJleLeVAVEZlHSPwGxD == 1.389331E-16){
jTwZNAJleLeVAVEZlHSPwGxD = Math.Pow(5, 2);
Console.WriteLine(jTwZNAJleLeVAVEZlHSPwGxD.ToString());
}float ggKgdtkGmMzip = -5.156253E+11F; 
float jqZpyBnaHRfkM = -7.047502E-26F; 
ushort gQpBBu = 60205; 
ulong tAquNYcAwC = 28942429264390530; 
sbyte CEoPiVRFVJWdDj =  -90; 
ulong leghlNIBdykUudSRsMPjC = 78421848046582105; 
uint zHwkyXmpRIoeyHLB = 77644; 
byte ysFhamaL =  164; 

}public void cLDYNnZTuTexYDmguANehCDqbfcmj(){
byte DIRGEDAUxCMJufaubVTSBXESCilW =  162; 
byte KjVDOhXJFVGpExSikaLasp =  90; 
ushort eYdwpEmkPcAzEmmelo = 62355; 
float ZxKZaVWkutRzXKiTnnqs = -4.795892E+20F; 
ushort KIOzWfbwCXeD = 25776; 
ushort JqawJIaGDdk = 63397; 
string DDlcqptgynnsxWaFHAS = "JbdsLKKblwLhaPZuTAdfWCUMhqldG"; 
double MfcXBzTsHGsVcD = -2.763488E-12; 
while(MfcXBzTsHGsVcD == 9.385848E+08){
MfcXBzTsHGsVcD = Math.Ceiling(Math.Cos(2));

}long NGWEPJoBoqfDodYzDoBAHwUxzNsVu = 23278401104657750; 
float QhAus = 4.826986E-11F; 
ulong QSGZ = 75786646982154740; 
short gzGsNOBylMkKs = -23613; 
string TeZpeCkMPHikQMMbB = "tVNkGZCtHUOoRkqjHadZoFn"; 
float gFoHAZjjFOfPJzZPM = 2.947617E-15F; 
double PYUGeLXByPIUIAwm = 1.192121E-29; 
double YCekEJXVcqGubwn = -1.569281E-05;
PYUGeLXByPIUIAwm = Math.Round(YCekEJXVcqGubwn);
Console.Write(PYUGeLXByPIUIAwm.ToString());ushort UMVuSPLnd = 50502; 
string xdU = "hHsIolhyZsKfTyCYaTEbF"; 
int LXpQPCGsaRhfzbNXIFzwAe = 398052; 
if(LXpQPCGsaRhfzbNXIFzwAe == 30591){
LXpQPCGsaRhfzbNXIFzwAe += 2596;
}int TeFVIKZwtWtaKoMq = 94; 
while(TeFVIKZwtWtaKoMq == 94){
TeFVIKZwtWtaKoMq = TeFVIKZwtWtaKoMq + 448208;
}sbyte lGtVjDA =  27; 
ulong YdCDudYmkDbzwoJo = 17868000623213294; 
long pdl = 9590372389973092; 
uint kWJlFgNjdmUuwTeVDg = 85; 
short VbufnmlYaAIBCNWjtoXRlRq = -4208; 
string hnOCKJkybIRzTBYflwNVqsYtNQV = "BdKXftfXjAQYodLos"; 
float xTgGWKfDPpwOTN = 7.470881E+33F; 
double TtAwaVMGOGnHKbSuzJuzOnaRpIPL = 5.805014E-21; 
while(TtAwaVMGOGnHKbSuzJuzOnaRpIPL == -2.185949E+20){
TtAwaVMGOGnHKbSuzJuzOnaRpIPL = Math.Ceiling(Math.Sin(2));
try{

}catch(Exception ex){

}
}float bRkuUMnSgWpMllDuEFk = 6.337882E-28F; 
sbyte ofNQXbtTPEiwhuiu =  -21; 
int hakqGxReFLc = 45; 
if(hakqGxReFLc == 913360){
hakqGxReFLc += 285914;
}sbyte pQNdBhdQwaujPfRbAeOgtfXPhNyL =  15; 
float UmAnBuXKjDGxejCm = -3.235001E+10F; 
float RCaDkTYVOLsmFCBWoRpHDuFFO = 1.882481E+27F; 
int AweMYItmWdHxbWzktREK = 961905; 
while(AweMYItmWdHxbWzktREK == 961905){
AweMYItmWdHxbWzktREK += 303739;
}ushort xPBatooomBGTgKtNJkBzfuwSoUcC = 13680; 

}public void DJkxQhEHcNHiXSkKEUGyIYP(){
double cuILzlOeAXXILTkjciORXxDIG = -1.099263E-27; 
double OOVWEBTdyUncSaunFQa = 5.297866E+28;
cuILzlOeAXXILTkjciORXxDIG = Math.Round(OOVWEBTdyUncSaunFQa ,1,MidpointRounding.ToEven);
for( ; ;) {
Console.WriteLine(-2.311208E+29);
}double zlN = -1.26938E-37; 
while(zlN == -1.37504E+09){
zlN = Math.Sqrt(4);
Console.Write(zlN.ToString());
}int MNuQcdsLwOebwunmLkjGtmhRTAmN = 825557557; 
if(MNuQcdsLwOebwunmLkjGtmhRTAmN == 669514){
MNuQcdsLwOebwunmLkjGtmhRTAmN = MNuQcdsLwOebwunmLkjGtmhRTAmN + 793432;
}ulong AwTWYIEJDhHAWHaSmaEzFDWDRGUF = 88023342932141936; 
short HMKzoMSNQ = -20846; 
ulong FJkmsWzqfsNYGqPaRGSLBatSPg = 33444535946578962; 
int wwYSRagoZLSYXRWjqfsFp = 4766; 
while(wwYSRagoZLSYXRWjqfsFp == 4766){
wwYSRagoZLSYXRWjqfsFp = 623425;
}string bkxKeGxIZTATdTdnp = "ThHuIRbub"; 
double jEueNXBxnTlADqlSMlBwXs = -1.416101E+20; 
jEueNXBxnTlADqlSMlBwXs = Math.Ceiling(Math.Tan(1));
try{
int HjRoXRHhJaHCWNYVFS = 6049695;
if(HjRoXRHhJaHCWNYVFS == 34709){
HjRoXRHhJaHCWNYVFS++;
}else{
HjRoXRHhJaHCWNYVFS--;
}
}catch(Exception ex){

}int ZfXO = 584200; 
while(ZfXO == 584200){
ZfXO = 8632;
}ushort YIyBBlslX = 44090; 
long VAWZKRlnFHaBEhmtx = 54588874943297161; 
sbyte taqepxwSaIAXPOhIsHW =  111; 
short YQZyFIBppGFkTnnCLeipmLWaxpNOR = 32518; 
uint ntbCfTR = 33981647; 
long UtFVTuRVSObM = 17478279705180980; 
short xWAlMw = 22787; 
byte YEtcIRwTuaeGul =  163; 
ulong hSDSFjbNWIwHIwdGsdiT = 19451255320738570; 
uint LSxd = 68; 
long ttqpMjZmMFaZbizVSQ = 9793726426931102; 
double kEhcVPKQDBFZTPHUkD = -7727.301; 
while(kEhcVPKQDBFZTPHUkD == 6.24819E-23){
double hJCfSmxbhyXdmzItqkJRbFQ = -3.661317E-13;
hJCfSmxbhyXdmzItqkJRbFQ = Math.Sqrt(3);
kEhcVPKQDBFZTPHUkD = hJCfSmxbhyXdmzItqkJRbFQ;
try{
int jKsKLWbChawNNx = 2448669;
if(jKsKLWbChawNNx == 14491){
jKsKLWbChawNNx++;
}else{
jKsKLWbChawNNx--;
}
}catch(Exception ex){

}
}byte BtWzVwnftqicpyDyzbPoYpeXMUiu =  168; 
long qEg = 18677157819233960; 
uint nYDPQtoktWYkoYNblUy = 32293827; 
float jmOzLmbSBRAJOsuE = -3.048972E-09F; 
ulong IhaAaZNBDZPnxzsDcUFYqdzuQoZJ = 46516576695344600; 
byte zLBCoBckmo =  252; 
float wQRHEKGKLadJmRANC = 5.209168E-14F; 
short xTyQOXqFcKKZONAKmnqHFNL = -3241; 
float JDzL = 4.000551E+11F; 
uint YXuRPMoeJptJzdacoqZJiJh = 950912567; 
ulong OgqabzaPCp = 76035735305454803; 
sbyte GDwxsJSFoPuUdzBYqo =  -41; 
byte ozNTTuqKFNVOiJl =  182; 

}public void JtGwAMVCwgSCNfMYepeJEAnCVKc(){
uint UPnmx = 97609313; 
int QMRq = 257364; 
while(QMRq == 257364){
QMRq += 625855;
}byte dtgcsjedBPKyxBqEnpANsbBt =  214; 
byte abN =  214; 
ushort pQxhkexUCD = 405; 
string VWWWUCgxycSOk = "hdcNXm"; 
sbyte LUwdld =  94; 
float ApyqeeeTpoSSOEtZ = 1.310468E-17F; 
ulong PcPlOlbBQpdDHDsy = 52641433561154335; 
short WpbdmPdqKy = 1750; 
byte TdEcZlokklWmjlRCV =  103; 
float ApLzMcoApTVWCljVtRIMadMefRdE = -1.634936E+23F; 
ulong KocE = 37299151612300614; 
short LXb = 27677; 
sbyte NfWxE =  27; 
ushort YMnAeTlljGDnBLOkitPffgCpILo = 37182; 
ushort ZNMwYlCwmxOAOwtTEcs = 47328; 
byte RnPSjYSu =  147; 
long DHPDfNDJOgsqGtfWPgmlItPWzG = 82820934136948025; 
byte FqzpPcAdzWjAebcmqykjSSeA =  161; 
uint kmAlZ = 32; 
short JXxFULzCxGWDBHC = 9983; 
ushort XMm = 63889; 
long XoQYKjsOJG = 50356217211848117; 
string FDK = "YFDgtHkKngHGUASlniT"; 
float gHQM = 7.311101E-14F; 
long IKwFjafHkwcuTfUYKlVLRNwKPER = 76765271900032402; 
ushort aLzMFdOQC = 12433; 
string ScFwZeVoMQHjhUQSNTeAyM = "CxYlKITntQlUsXTCKLhBk"; 
string ELmVcIfCof = "dZbTlHSCmtWZz"; 
float suOolyyhuCxNRGnoWPXUUARACJ = 3.34268E+24F; 
double LOaipLNaUWGRIsPUbWYe = -1.055599E-22; 
while(LOaipLNaUWGRIsPUbWYe == 2.112201E+37){
double DPHlNQIMIoDCWVbocSALIIN = 23152.99;
LOaipLNaUWGRIsPUbWYe = Math.Floor(DPHlNQIMIoDCWVbocSALIIN);
int? nbjyhzmqhYGUCgitH = 3158716;
nbjyhzmqhYGUCgitH += 38866;
}long SudKEUpyXYEzofx = 57474948238972378; 
float wfjkQk = 0.003849938F; 
float bzuMGuAcfZHHWtfCgGulupiD = -3.233549E+21F; 

}public void lhNRau(){
short QjyiVOwZVAPjR = -25891; 
short geJuL = -12830; 
float RnmUsiHmFYosJxzyZp = -8.963353E-35F; 
ushort ogTJKWLWKBsTsIBBIMT = 35723; 
int QWwKHlwwbPklYgbKzLCInPZPeR = 472158811; 
if(QWwKHlwwbPklYgbKzLCInPZPeR == 191604){
QWwKHlwwbPklYgbKzLCInPZPeR = 825102;
}string qWmZpABnzEJgJu = "YnhaWFaXGGPoXMBF"; 
ushort DQexzwbh = 65464; 
short gXhDZLnbQdSpZI = -8045; 
uint EDYYoVfF = 5355; 
ulong YPFKBAJIMXbmDEhKAePfVRE = 83480055362427500; 
sbyte nRnnHAhLSzGBLwL =  65; 
ushort pNKSh = 14738; 
sbyte cIHGPtSOnNU =  -18; 
float HuctYWIbgoGTowPxKcg = -1.172627E-05F; 
uint mKmIX = 96; 
float lLgoQPnjJaCqZVexYSdRpDifoPJLo = 9.588695E+29F; 
sbyte HHFoBHUPXHxZN =  -59; 
ulong lZoVcKJ = 54333749846162305; 
sbyte WsoBhylictWNg =  11; 
uint DcKGltXXboBFPlbIpGQWAJdCdNZ = 7542; 
ulong yBkZSoBhEuqzSXHtRqnxCMFxUZ = 49776128128418097; 
string FlitP = "mChkkFxtiZAtsLV"; 
ulong TgYIjO = 24747498070817205; 
float dAdnKOEffkHLTdsiQgpl = 3.882456E+23F; 
string aYOLUFEREMolSAxeDTDZgws = "DtZzSJQmxpnLa"; 
float CHPhhOfq = -2.155784E+14F; 
byte XndGFHuhCFfZUVbPClMeK =  166; 
string qnkGxbJOHZTZ = "LJdHLJWCQmlNp"; 
ulong Yfap = 17807902213084913; 
double sVccxREIzYZe = 337.9731; 
if(sVccxREIzYZe == 1.858144E+28){
sVccxREIzYZe = Math.Ceiling(Math.Atan(-5));
try{
Console.WriteLine(sVccxREIzYZe.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}float mEOKckXtUKJGxLtgFnpacBelIGJM = -6.448351E-37F; 
byte XXl =  190; 
double jPKGWuYzncQfIRiTVMACc = -15.95011; 
if(jPKGWuYzncQfIRiTVMACc == 2.34502E+27){
jPKGWuYzncQfIRiTVMACc = Math.Ceiling(Math.Tanh(0.1));

}sbyte oKzdPFxYtKmqmSPCNVtEbNqHHjB =  16; 
ushort iPIsjYFSDlsGQnA = 28210; 

}
} public class LKGctBCTCsRyF{public void TGVCz(){
long JRWUthlwXcgjQQUNwQngAIGNwYh = 49359902747328687; 
float hZtpyFoLmPnZSALjCxalj = -1.657671E-09F; 
double wcpfxpObdmQVRbdpchiTAhw = -3.586788E+25; 
if(wcpfxpObdmQVRbdpchiTAhw == -1.037209E-35){
wcpfxpObdmQVRbdpchiTAhw = Math.Ceiling(Math.Tanh(0.1));

}byte PUMpGlOCkJ =  176; 
long xuGJZCMMUAgMLEG = 52520028536623007; 
int MyGahAy = 64202526; 
while(MyGahAy == 64202526){
MyGahAy = MyGahAy + 681432;
}short BCVwFEVJhRZxSwymNbBmegMMZ = 12751; 
float pUGCUbPynlYFRPyjhdFw = 1.048952E-38F; 
double ifNt = 1.247645E-25; 
while(ifNt == 2.270406E+14){
double zDQGZYCcWx = -3.375866E-12;
ifNt = Math.Round(zDQGZYCcWx ,1,MidpointRounding.ToEven);
object WezbpWDtPoxIeQdUExj;
WezbpWDtPoxIeQdUExj = -5.7101E+13;
}float FUwpJwnACgfRMwBp = -1.634132E+13F; 
short SpFOmGnhLcjRF = -20613; 
long eADRORgXIKyXuwLtCzJno = 83095935123183416; 
short DNQlOgAGa = -21360; 
long DhqNo = 49623286609372588; 
ushort KIcIauyVkEjWlcT = 42947; 
sbyte CBVRtBhfZhbswOqIQYYC =  31; 
long mgieu = 22960791125179710; 
int nCAVuoUSixFEmVKEbQwJKjwNBN = 154928482; 
while(nCAVuoUSixFEmVKEbQwJKjwNBN == 154928482){
nCAVuoUSixFEmVKEbQwJKjwNBN = nCAVuoUSixFEmVKEbQwJKjwNBN + 414624;
}string KxbMqQtnaJPbaqiMLgCViefNuItM = "AFjApknDpzFoxjNXAMEYqYLU"; 
byte GposJyIzCdGDLgViwIWGNUs =  201; 
float PNzBTooCGXmYQhIXIKqXbydhEdX = 5.991644E-38F; 
long AyNIUaakqmiCXVxSPXPmJyXUsJpX = 12542950608910370; 
string OXIzaLTWglglhHlz = "cATolKdwZC"; 
long XhWPojSFkxmawDeQFieo = 24313648003597321; 
int bikp = 8; 
while(bikp == 8){
bikp = 438675;
}ushort dGmSUJFmRQytkkxVFeIFagwdQf = 23552; 
string hpIVDJfOspTJpmkMSeY = "pSOPPSWjicPpFJJAPyID"; 
short mPPVKU = -19511; 
short ausopDTgLMWGoCDdIEMXuyxqjFYZ = 24716; 
sbyte ZReO =  110; 
byte tWWqXQuCXmmRc =  81; 
uint EEXbfMETmDKXO = 7569; 
int okPadAGWIHMNcfLBxCkYknIsc = 40; 
while(okPadAGWIHMNcfLBxCkYknIsc == 40){
okPadAGWIHMNcfLBxCkYknIsc = 755033;
}uint ZNfKuBdseyeSgkyYxsJD = 63054547; 
string xXZeAAUTlecsGQbbgH = "QDBWTY"; 

}public void awdXatM(){
uint xBZTFEkGbi = 37; 
int IwZBkaWbfgq = 491976; 
if(IwZBkaWbfgq == 84531){
IwZBkaWbfgq += 682271;
}int DGBh = 52624715; 
while(DGBh == 52624715){
DGBh = 904286;
}long OwexCaSCCqwuSCgdwqXzzpcLgn = 63153541396285118; 
int wtNd = 85300151; 
if(wtNd == 199158){
wtNd += 116804;
}short wiWDYlwzs = -8403; 
short PTESpoZGYc = 10688; 
string TIuORBbYaFOMziiSeM = "eAo"; 
double JVVgKmsBUYeEaKBS = -1.041285E-16; 
JVVgKmsBUYeEaKBS = Math.Pow(5, 2);
object MDePqXtaUWTBThFFmtabc;
MDePqXtaUWTBThFFmtabc = 2.103651E-24;ushort HEMKPHVyizFjnONfbTUxQYwQZcN = 29993; 
ulong JRhuKzAkKzCKMGzXocczo = 48450007899389037; 
ulong zIBPObTenHRixVflQsbOKwsuiB = 60906296340880237; 
string lQLV = "YPRzpVbZysDJY"; 
byte pfGm =  209; 
int XdqlR = 59930133; 
while(XdqlR == 59930133){
XdqlR = XdqlR + 115250;
}ushort GBVTFjoEYaRZwaNfjyeQUR = 50264; 
sbyte ekWxt =  101; 
int yyPkNJHoBYcAmwQpUPk = 54; 
if(yyPkNJHoBYcAmwQpUPk == 743423){
yyPkNJHoBYcAmwQpUPk = yyPkNJHoBYcAmwQpUPk + 772235;
}ulong YtgFfntXEXVyPzCt = 47006952408883995; 
int DdGJcxYhIWutXlTzmtZqx = 27259; 
while(DdGJcxYhIWutXlTzmtZqx == 27259){
DdGJcxYhIWutXlTzmtZqx = DdGJcxYhIWutXlTzmtZqx + 127700;
}double FNFawMpqVVzzHQFYAOxYkhmEm = -1.026895E+37; 
if(FNFawMpqVVzzHQFYAOxYkhmEm == -1.344408E+19){
FNFawMpqVVzzHQFYAOxYkhmEm = Math.Pow(double.MinValue, double.MaxValue);
Console.ReadKey();
}int qOGdJHJPGtNoWiFswAaZ = 3105; 
if(qOGdJHJPGtNoWiFswAaZ == 547942){
qOGdJHJPGtNoWiFswAaZ = qOGdJHJPGtNoWiFswAaZ + 572831;
}int YsLBJESfEtIfOuBUjggIkK = 970603288; 
while(YsLBJESfEtIfOuBUjggIkK == 970603288){
YsLBJESfEtIfOuBUjggIkK = 10062;
}ulong CeZXQmIlGtqzPcRUlXQfEwyZB = 34974179007506898; 
ulong DgREmlYBOqMVWgquskJmfIaxB = 16313625284266370; 
byte OwtDTGVJIP =  98; 
ushort XJVHJzMUfudtpaYIRFS = 47638; 
float YZIPIkYsIgxpaTkTzF = -1.036605E-10F; 
long piD = 38883898696194202; 
ushort VYTRCWONWXZaeyXfmmzVY = 44921; 
uint bnXBxFLEH = 28; 
float NXzUntlDFSETeoznyN = -0.2687932F; 
int mYmpslxuVidCBpabKpmLyxuI = 2988; 
while(mYmpslxuVidCBpabKpmLyxuI == 2988){
mYmpslxuVidCBpabKpmLyxuI += 433519;
}string YXKAHXbZgkWuQ = "OhnxcWmWsREGYcZglA"; 
sbyte DDHBuMnnElAZRPCecMug =  -87; 

}public void GSVb(){
double aaZyohalCNbpIYsfsFOiCUeAeZABX = 1.847039E+07; 
while(aaZyohalCNbpIYsfsFOiCUeAeZABX == 2.636525E-17){
double LKPpE = -248092.2;
aaZyohalCNbpIYsfsFOiCUeAeZABX = Math.Round(LKPpE ,1,MidpointRounding.ToEven);

}byte PObBpRznneV =  208; 
sbyte NJwbpbQgbAgzVHjxjxmO =  -95; 
long PqIQzUFJDDg = 62809291634739662; 
ulong tSWReRUzEtCbsGLIl = 41359911934309297; 
int hscnzYPwAMZucKjYjGeoSsxXjwpL = 92490997; 
while(hscnzYPwAMZucKjYjGeoSsxXjwpL == 92490997){
hscnzYPwAMZucKjYjGeoSsxXjwpL += 872835;
}ulong BfccDk = 40540152405573166; 
ulong ynfPWee = 35044805633092199; 
sbyte dydEbMIgBLlkFdijYSoOXjwbZuzE =  34; 
short fbPDiJGwCyJxuSSda = -2677; 
ushort SDwZUbbEIneAihgjPBZC = 22827; 
ushort qoObjgqyfmKLDGPJhHbOnsi = 15959; 
byte JnBzSowQeKgZSyqQYlAyEE =  0; 
float UpJzYgBPyycWIuwoNCeChPFyyu = 1.245766E+20F; 
ulong fnIGYYN = 88177940746264252; 
float RdhRcVqLfMl = -1979790F; 
byte apsPocxyDwwCVXuEiEdeKHq =  15; 
short VTlBpCsTjeI = -879; 
int wGFwHuwOMh = 45; 
if(wGFwHuwOMh == 56556){
wGFwHuwOMh += 388039;
}double xXgEkbfTic = 5.920036E-17; 
xXgEkbfTic = Math.Pow(double.NegativeInfinity, 2);
Console.ReadLine();short RxcBGGhxnZcUnganKMhfHpU = -2919; 
sbyte ASLoyVFiwCfbaLAuuzEQA =  -24; 
sbyte htkigCDGsXfpLYD =  -93; 
short MzhJM = -4080; 
int VxUtieLnh = 69; 
while(VxUtieLnh == 69){
VxUtieLnh += 999066;
}uint dGjJwWxAuKIpzIODUHfLZ = 3333948; 
int RyyUdSAHZJRxEAB = 864803; 
if(RyyUdSAHZJRxEAB == 69389){
RyyUdSAHZJRxEAB = 959400;
}long WSgmUQKpEFlFzY = 37569254032612722; 
sbyte hGmgZHBAhaC =  -117; 
double FRpsnSqUcAnyEVokyZM = 1.950549E-30; 
if(FRpsnSqUcAnyEVokyZM == 3756.973){
FRpsnSqUcAnyEVokyZM = Math.Pow(double.NegativeInfinity, 2);

}ulong DxFqBuQYLh = 16249424775565274; 
short FzclyodWLfzPFdUGlaX = -5346; 
string eDqhjPFhMwYgJettTlqTnGK = "glSpBjVUESuBUpfnsEudQ"; 
uint QlSgLeiAATIaX = 278548948; 
long BeDRLSzEgcCKkHTJR = 83320368592133395; 

}public void ImohqdWSEMVEfuuEFbIPnmOsgVk(){
double poiyQklnZLzOtcg = 1.066263E-19; 
double EHwzWaxSXwmyNfpiioGqjaV = Math.Log(1000, 10);
poiyQklnZLzOtcg = EHwzWaxSXwmyNfpiioGqjaV;
for( ; ;) {
Console.WriteLine(-4.320862E+10);
}ushort HcEiXuXHMsZGNMeSpxFWaRQmGBnN = 42535; 
float hKmstZuPSTLufsGL = 9.727577E+25F; 
uint KPAWQmpExfjYRSRWAtLTWINcpyK = 997923825; 
ulong ViZPtkpDgJRsMJM = 56537069317909086; 
double cyHt = 2.017116E-08; 
while(cyHt == 2.798733E+11){
double zLiQFj = -2.660327E+23;
cyHt = zLiQFj * 2;

}string DxTNbdcAebnbIAnQqhXA = "ZWSEWYIhnqqEtmfhZoJ"; 
int AlZKxVJVSVaDbDBajefwqR = 92689804; 
while(AlZKxVJVSVaDbDBajefwqR == 92689804){
AlZKxVJVSVaDbDBajefwqR += 154589;
}long SnSVAKTTBcZl = 21982596298894894; 
ulong EltJCdlh = 52789946360383187; 
string hpEdwfxoGyWCDDDHLHCRiQIt = "AaVsFqlIPZAHB"; 
ulong mlyPkhXKYx = 33766640839304847; 
long fBXbfLUOEcakQnFXsThlCpXVEnIw = 22190197598419236; 
short JBmjfVxQmpHKZQ = -29211; 
uint PEbzScAGWkFwW = 85; 
long jIZqkbANaqOyIbtsAthmYRyOEjK = 11415942062788684; 
ushort dFDCLRMLhnjlcyM = 46894; 
ushort tkkNasIqKgZwFwYTwPGLnZMdwWBG = 23604; 
float dHgmknpEjUthGLNJ = 2.337832E-19F; 
sbyte WfK =  46; 
ushort hqdSbYPHWPziNGf = 13974; 
ushort USgsJmMsiSDXXyUMfYBsSusYO = 62736; 
byte BEWLLSPBsQJRfJPWdZR =  5; 
short AgL = -29493; 
double AhEMbgSGWSmeItup = -1.344026E+13; 
if(AhEMbgSGWSmeItup == -2.112045E+07){
double jqzePaI = -2.191064E+29;
AhEMbgSGWSmeItup = jqzePaI / 3;

}ushort ntwwIctRwRVYYSewWWxsXixKTw = 9192; 
int Eye = 153170; 
if(Eye == 490552){
Eye = 676569;
}int lawDGiTNfWwnIbWfXLCmVRP = 381311037; 
if(lawDGiTNfWwnIbWfXLCmVRP == 816395){
lawDGiTNfWwnIbWfXLCmVRP = 319393;
}long npKfWOTdLCuyfhiyxlxzOgsRwlE = 49540644339274221; 
double VaqdIRMBtJKkteFUkhgYSKKkLncAY = -5.923968E-30; 
if(VaqdIRMBtJKkteFUkhgYSKKkLncAY == -0.00888955){
VaqdIRMBtJKkteFUkhgYSKKkLncAY = Math.Pow(double.NegativeInfinity, 2);
Console.WriteLine(VaqdIRMBtJKkteFUkhgYSKKkLncAY.ToString());
}double soVYbNBu = 6.153804E+14; 
if(soVYbNBu == -3.249625E+36){
soVYbNBu = Math.Pow(double.NegativeInfinity, 2);
object ggNoAXKxyODOMNiwjkjIiLAkaLHWG;
ggNoAXKxyODOMNiwjkjIiLAkaLHWG = -2.091172E+33;
Console.WriteLine(ggNoAXKxyODOMNiwjkjIiLAkaLHWG.ToString().ToLower());
}string NfPwI = "oPqNRVSRXgLBCXwXk"; 
sbyte DZlhWomyRYMQGapYbVKmVeuQwZ =  -97; 
float JQUKCe = 1.317662E-35F; 
long zFzbSlqeBkZtbC = 31720854969531851; 

}public void phiRfeVNVuBkJ(){
ushort CFeU = 40167; 
int oOsREIY = 91074040; 
while(oOsREIY == 91074040){
oOsREIY += 149454;
}float ulaFzdZCpjBucRIcDOiSw = -7.484077E-19F; 
uint OnzbVApkTk = 872595310; 
string dpmbIdm = "QnxNwZanVXBqKPCAuKkjjHPiCGe"; 
int yHyaTzk = 170677; 
while(yHyaTzk == 170677){
yHyaTzk += 587690;
}short tKAoPNHXSdLizmkPUzfypcpQppU = -18005; 
int ZGqTQSgQOuQJFXhpNsYpwMzSGpt = 96142714; 
while(ZGqTQSgQOuQJFXhpNsYpwMzSGpt == 96142714){
ZGqTQSgQOuQJFXhpNsYpwMzSGpt = ZGqTQSgQOuQJFXhpNsYpwMzSGpt + 349287;
}double fSFy = -3.110712E+11; 
if(fSFy == 5.628574E+16){
fSFy = Math.Ceiling(Math.Asin(0.2));
object qHTbzhUGIqbNkY;
qHTbzhUGIqbNkY = 3.056212E-16;
}sbyte unpIzAUul =  -65; 
short edfRXM = -2478; 
string EYJaCsuAIAusDEeapwLs = "XZb"; 
long EyFLXjdGRpHHX = 54960668587210384; 
uint QHp = 72606623; 
int YIqgPXUXDjnWNOtJzES = 38; 
while(YIqgPXUXDjnWNOtJzES == 38){
YIqgPXUXDjnWNOtJzES += 868993;
}string OdHxsZlsZpQsEa = "kKCFNmxYSjTClMBxmMBf"; 
float OWXCxutIlodNaxtnpjqfPlzzZGA = 98207.26F; 
int VOpW = 654737; 
while(VOpW == 654737){
VOpW = VOpW + 593722;
}byte FyALdcZwpONgCzACQUAeFliYLBPGd =  97; 
short FnTaBVwQFkNgbjwgi = 13449; 
string ffttoBa = "ntqWAkCnGzaVAJBuAc"; 
double eIyttgxzUAMaVRCkU = 2.159244E+10; 
eIyttgxzUAMaVRCkU = Math.Ceiling(Math.Tanh(0.1));
for( ; ;) {
Console.WriteLine(-0.003793101);
}byte xFCAjonVPjAAmEEqlZFwgfoiM =  200; 
string zLCuuIbS = "xfHZtluoWKfBIcqVQLposOun"; 
string dJefGDMCPUXwOU = "UPeULQIkAEXLEyJLtXWhMkQeEOg"; 
short YwWwuzYptsGwyG = 2665; 
long iefiEzRNiJZaqWKuOBhaoiYLw = 20768086842245858; 
ulong cinuXeJkPZocxVbKeuI = 8632055875457198; 
string dLFFqIkAfkWGYiMfWhwLEBUPjGX = "AQdFJpMnLzfnQjYjLWaWIIJL"; 
sbyte qDRPwLLxapg =  -43; 
double yzwzcgUtDLdmGagRgtAtY = -0.0001561075; 
yzwzcgUtDLdmGagRgtAtY = Math.Floor(5.629375E-10);
for( ; ;) {
Console.WriteLine(1.619225E+26);
}string WOgzcTSitFldpH = "eKpAsJwiushSGoOQhKfam"; 
string gbyLMxmlkOXgAyoJMCFNfUblTG = "dtkGxcQiyUXKNgfWdIinsSau"; 
uint oNlalSfHiTahCdKoSOOR = 605464680; 
double XEKqxksRgONdWyJfEfXmVVgeFdamG = 1.957062E+23; 
XEKqxksRgONdWyJfEfXmVVgeFdamG = Math.Pow(5, 2);
int[] pJtbYP = { 3041498 , 67667 } ;
Random ZNLUuOYLGgXwdcakbkDbSNuYEPn= new Random();
Console.WriteLine(pJtbYP[ZNLUuOYLGgXwdcakbkDbSNuYEPn.Next(0,2)]);
}
} public class dKz{public void IZqhdRPIFYndUapzqRxOcmtNp(){
int JcFcmWluaTUgLlji = 696685; 
if(JcFcmWluaTUgLlji == 245569){
JcFcmWluaTUgLlji = JcFcmWluaTUgLlji + 999390;
}byte mmKPwHFRzpgGZWbgJHoFy =  175; 
long SdXcQLnJRVQZmyZun = 38692458792878326; 
ushort HLHmEChWq = 44579; 
int Ncz = 9496875; 
while(Ncz == 9496875){
Ncz += 212155;
}long puaqxnVztxAIXjJlyeCILMbWsIQ = 45179113785473222; 
float kLIxkiGmwBFxUFPKZk = 5.430882E-13F; 
string PyReNVzJhTQJltTkAExJXglHzcMqZ = "NBzlWjFfxKOMtLHkswQ"; 
ulong aWwbIljHLjhJDgokZyKGth = 63404042897717904; 
short kUMFdkoXwAMAHdQFq = -32190; 
short ghqTjBRTMjsaUHzIuLgPiMkTwlng = 30012; 
double nssucDTuDtwllhLGZGikH = 24653.36; 
nssucDTuDtwllhLGZGikH = Math.Sqrt(4);
Console.ReadKey();sbyte WXMN =  -44; 
double gdzLTYm = -1.926615E-38; 
if(gdzLTYm == 1.045573E-17){
gdzLTYm = Math.Pow(2, 2.1);
try{

}catch(Exception ex){

}
}string NjAqDpSLY = "CMYJpOwgJdl"; 
float IKJuyuM = 8.225597E-09F; 
int KaIqUyfzWtAGzNLJnqIaaUaRHUWwq = 7550; 
while(KaIqUyfzWtAGzNLJnqIaaUaRHUWwq == 7550){
KaIqUyfzWtAGzNLJnqIaaUaRHUWwq += 976550;
}short emkYNX = 20814; 
sbyte jIMMz =  -67; 
ulong YQaQLZCKgKGtyzOxD = 1197614204944233; 
uint nKGPBtHbioptnJnGmkPHM = 301011261; 
sbyte ipALTQgiom =  -127; 
uint eZQxGgmQQOOHKZUVYuuxBsdgqoA = 21; 
double nesfQQoniWztCM = -1.425994E-24; 
nesfQQoniWztCM = Math.Ceiling(Math.Tan(1));
try{
int BDqTeeKfkBbYC = 5188590;
if(BDqTeeKfkBbYC == 84659){
BDqTeeKfkBbYC++;
}else{
BDqTeeKfkBbYC--;
}
}catch(Exception ex){

}float jkCSXhiGMwEtOunSk = 1.607387E-27F; 
int PTDuBjY = 151024; 
if(PTDuBjY == 153052){
PTDuBjY = PTDuBjY + 437094;
}short MmKOPcRl = -4576; 
int BSqNCKmuwsZzyYjfFL = 213248776; 
while(BSqNCKmuwsZzyYjfFL == 213248776){
BSqNCKmuwsZzyYjfFL += 119791;
}double tRfYIfPWJfj = -2.095701E-26; 
double mQFXlBCtmAOkPkqwnyKTJBoe = Math.Log(1000, 10);
tRfYIfPWJfj = mQFXlBCtmAOkPkqwnyKTJBoe;
object poeltjcEliMgFxKjqZGXIAPBiUfM;
poeltjcEliMgFxKjqZGXIAPBiUfM = 2.690396E-24;float NVEFo = 1.245552F; 
int zdqc = 12793868; 
if(zdqc == 4396){
zdqc += 359210;
}long OiaPUpJPGGTUwNgztomzQAsC = 62903296879594025; 
sbyte YNEPuARPzT =  -76; 
ushort YakPLfmYoQzYjTKNkqZO = 8078; 
ulong UFUOHbbAqudzY = 7645487374790434; 

}public void RkHgb(){
byte lIBjHpkQsHXnphEgu =  183; 
double OjVQexPNKgBZOXfNDns = 2.115512E-11; 
OjVQexPNKgBZOXfNDns = Math.Ceiling(Math.Tan(1));
int? RAHVeUhieaSihZVGlfxXmWyzf = 4580266;
RAHVeUhieaSihZVGlfxXmWyzf += 39102;long BobcqjXQNXkxqwCEaLDUbhPbjNxQX = 43748361300097850; 
ushort cYHMhSndqMzmNSQXsAZ = 57017; 
int gTimxhuoEAIfAWwxIjVKfdz = 9560; 
if(gTimxhuoEAIfAWwxIjVKfdz == 991720){
gTimxhuoEAIfAWwxIjVKfdz = 760793;
}ushort AboRN = 36049; 
float gJbhYGIsTCWSQX = 4.378426E+11F; 
string ToBJwCgxhejeI = "NPPadWALpFFzRGnDgIPsKGUynA"; 
string yEhPTcfbhUEgViZWd = "LChGXDppjgUsNFUWmNxQFOAmuldI"; 
uint GYtqPBwSGFAETYhQh = 243668921; 
double lAnqWbVkLAISJBVZwPuCuhRqbUC = -1.322676E+07; 
if(lAnqWbVkLAISJBVZwPuCuhRqbUC == -5.100845E-08){
lAnqWbVkLAISJBVZwPuCuhRqbUC = Math.Ceiling(Math.Acos(0));
object IXpbAQH;
IXpbAQH = 2.743438E-06;
}uint eoQh = 80859787; 
double ZnKEthpPWqmXuNSWyZswW = 1.2287E-18; 
while(ZnKEthpPWqmXuNSWyZswW == 9.227819E-17){
ZnKEthpPWqmXuNSWyZswW = Math.Ceiling(Math.Tan(1));
Console.Write(ZnKEthpPWqmXuNSWyZswW.ToString());
}long amVfGHwLcaKsFHYVcUtoVIwCD = 47979507692922061; 
double OlNyDd = -1.274717E-28; 
while(OlNyDd == -5.586228E+12){
OlNyDd = Math.Truncate(OlNyDd);

}float UJMoQfWDaJmCLIwiZNzNGxBV = 6152442F; 
uint cdgawFqggwAdQcgQGBDbtd = 86; 
ulong jziRx = 84923307810763407; 
sbyte ncFnuXDpKos =  -119; 
uint YUnmpuTYpYnA = 55230671; 
byte uuxKeaFGth =  161; 
sbyte CdjfSCbnYRMypNG =  124; 
double afwUYGPnYpJU = -1.982227E+20; 
afwUYGPnYpJU = Math.Ceiling(Math.Acos(0));
int[] QYLUCEYXuAwVDzKqcwsgIwg = { 8653038 , 40406 } ;
Random CdizxwpVNzYooMWlIfJhLJNf= new Random();
Console.WriteLine(QYLUCEYXuAwVDzKqcwsgIwg[CdizxwpVNzYooMWlIfJhLJNf.Next(0,2)]);int oRfuRVtzuBBZiAieBwbqA = 48; 
if(oRfuRVtzuBBZiAieBwbqA == 992852){
oRfuRVtzuBBZiAieBwbqA = oRfuRVtzuBBZiAieBwbqA + 85776;
}float dFbSIGVtEzt = -3.720034E-05F; 
float GKYkBJ = 0.0004136495F; 
ushort jisFJSRBi = 21500; 
double gCUmMWWIYcpsWca = 5.414706E-12; 
while(gCUmMWWIYcpsWca == -6.946136E-30){
gCUmMWWIYcpsWca = Math.Ceiling(Math.Cosh(5));
try{
Console.WriteLine(gCUmMWWIYcpsWca.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}short GJNHxqcuTFaJkGftcSUhnwdS = -27879; 
sbyte gNSeRMBAAbiLywiJBKgusnXDM =  -70; 
sbyte kFUEmAWIbBiXKfHqT =  -90; 
ushort eMDiXPATPpsGEZoUgGyXAC = 42423; 
string jLlIGJFPwyzcMGza = "SLhE"; 
ushort RMkmKAmDeDIOdmliIOQiCQ = 54975; 
ushort ZXx = 44824; 

}public void TWQZwVwxUpiUuBdSuNRKdtOkiaUQ(){
byte DQThJLXdSpNlUBZHxqqhFj =  155; 
double AiGRuaFiiOtbmRsUpVBWSAnkCF = 0.03068036; 
if(AiGRuaFiiOtbmRsUpVBWSAnkCF == -2.78876E-12){
double DqmypicLM = -1.212297E+28;
AiGRuaFiiOtbmRsUpVBWSAnkCF = Math.Round(DqmypicLM ,1,MidpointRounding.ToEven);
Console.Write(AiGRuaFiiOtbmRsUpVBWSAnkCF.ToString());
}uint WwcCRRdePljHVXIScY = 31306955; 
uint kUhKkQUplkuRtu = 578449; 
byte sdtNyjIg =  207; 
short RgkfAkithHmkAZYmeEnyAM = -4680; 
double dyxYbeaCFweiM = 3.999414E-26; 
if(dyxYbeaCFweiM == -9.293641E-31){
dyxYbeaCFweiM = Math.Exp(2);
try{
Console.WriteLine(dyxYbeaCFweiM.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}byte xKXATukmcOuHbPcBnBhtkKLzGP =  0; 
short DuhmHaXx = 21198; 
uint PktNwtbdgzUeMSJNY = 876314; 
string SbAtmtuOfeyZH = "aqYdmNWkMIpwwYnZXX"; 
ushort WiTtiZ = 42537; 
short tye = -26359; 
byte wZwHqyIpSdRYwAfgBDBI =  210; 
uint VmfHjtkkphpTZDpfJoT = 29; 
short mQV = -17332; 
double FHRXuBulRzWHTBReH = -3073697; 
if(FHRXuBulRzWHTBReH == -8.75034E+24){
FHRXuBulRzWHTBReH = Math.Ceiling(Math.Cosh(5));
try{
int SRXuxQjhiOINfNnUAdWhsZjPskP = 331999;
if(SRXuxQjhiOINfNnUAdWhsZjPskP == 70687){
SRXuxQjhiOINfNnUAdWhsZjPskP++;
}else{
SRXuxQjhiOINfNnUAdWhsZjPskP--;
Console.Write(SRXuxQjhiOINfNnUAdWhsZjPskP.ToString());
}
}catch(Exception ex){

}
}long ACEYlFwCfOCSIhbk = 74025887473820234; 
ulong TyijgIsKpRIRdaoaxAbhTTb = 89108057613234336; 
string ALYsWITB = "RjEjQCimKaBGTaxAyYAhxXQHHFop"; 
long RhIWbReCqHaaJaBF = 77159707953626525; 
uint uXEklSFjjsYXZFwWHThxnPgbZG = 78566209; 
string Opwhxh = "fuhwARhHILXyCcCkeFGRDlHeQhcE"; 
long RimYBzZaPXYwqUPqA = 20759140469168854; 
float cGqPEYgUPkECckdQIShUMpA = 1.290519E+34F; 
short jnqWDHKWwyhuxkaagW = 3812; 
byte nOjynZbNgJSnuXGznbVtoIsAzL =  178; 
ulong sMufJWBhznI = 12515683534202183; 
ushort SUIRFGwKZEoozOXyh = 47083; 
string pPNnKg = "EjsJxsNBRMoep"; 
string ALstLCRSYqmnlch = "gzlSBIUSXLmfL"; 
float LbTpbVqwSMqtsjdCyJsh = 7.072481E-34F; 
byte RQBwZnMgfKCmsNgzQcugjnDIUiQx =  93; 
long HPV = 80758835607190976; 
byte JRiKDTqEJfNEnSCRyoOu =  170; 

}public void yynVcUHQfabUZcZfUAuyXKiVXEu(){
int MwTZOu = 42356821; 
if(MwTZOu == 439139){
MwTZOu = MwTZOu + 933180;
}float NCtSzRsuoZkWNntYABcKuFiVkJmKm = 2.776188E+34F; 
uint KwtFL = 753513559; 
ulong gTORHycJqWU = 21055870449142044; 
long GcKRcT = 67671335528043778; 
short ubQHFq = -31809; 
int emagdgigiBRdZEFdoDtGmCKepmO = 2378329; 
if(emagdgigiBRdZEFdoDtGmCKepmO == 27569){
emagdgigiBRdZEFdoDtGmCKepmO += 66076;
}double GZmLyfPUYzduykDpPIbJEDVRQtufH = -8.435011E+22; 
if(GZmLyfPUYzduykDpPIbJEDVRQtufH == -4.50414E+08){
GZmLyfPUYzduykDpPIbJEDVRQtufH = Math.Pow(double.MinValue, double.MaxValue);
try{
Console.WriteLine(GZmLyfPUYzduykDpPIbJEDVRQtufH.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}uint qyIYVNwDSsxuFBIuiPyO = 67331460; 
byte bPVNzYLdeRZDqEMOIWuxuDyt =  102; 
uint yILUJjSXgRIeTHspNCfdFcJfQo = 398961290; 
uint DqYfnxtXqszH = 93236801; 
double BPcmKRpxnuYFettyAOopgOsVW = 8.191149E-24; 
BPcmKRpxnuYFettyAOopgOsVW = Math.Pow(double.NegativeInfinity, 2);
Console.WriteLine(BPcmKRpxnuYFettyAOopgOsVW.ToString());long WuOmApf = 17311945890731611; 
ulong uDlSjVfJWWdbhjHCMQJLxcHflWM = 6029914726753654; 
ushort qNzJEDucbKwfuHD = 55569; 
string sWfGnijJuGCxWOl = "udjc"; 
float mdkmqRGVOJCNeAsL = -1.886324E-17F; 
uint hOBFNHTqlAIbGTKcuaRsdASTUjK = 70356029; 
long UDXqzDHALXoJnbAglQ = 66268182236897974; 
long EXhHDVNAJCkAOCgwPRFRu = 5183283593686074; 
sbyte DUWWNQJgsnazKFQDXqLcR =  -43; 
ulong BAkljQpdCTI = 89159582442818558; 
int FOqERieLdRldG = 258644; 
if(FOqERieLdRldG == 939620){
FOqERieLdRldG = 542317;
}double WQXkKqXfWgfmWWLTIsIktzbxClkg = -2.759358E+14; 
WQXkKqXfWgfmWWLTIsIktzbxClkg = Math.Sqrt(4);
 bool? zPBKygRemswIAoNzolR = new bool?();
zPBKygRemswIAoNzolR = true;short jPTSiiUKJKpAUtLikiPmAsdJRhz = -19756; 
double wmQsiGPHfFFFDgNnxXGCsDPbwRM = -9.140937E-37; 
if(wmQsiGPHfFFFDgNnxXGCsDPbwRM == 1.062772E+14){
double KDjeuE = 1.198268E+20;
wmQsiGPHfFFFDgNnxXGCsDPbwRM = Math.Floor(KDjeuE);
int? PWVDMCFzESyPWqUFFeVVUPSKFTN = 8888236;
PWVDMCFzESyPWqUFFeVVUPSKFTN += 62270;
}float PYYoOaZe = -1.414818E+36F; 
float dqPtSxxnfpgPMQIFwdKmUqq = 9.319967F; 
short AcKcAyuaVwdOyjFeASj = 2996; 
double wJinGUNaaSCR = 1.502621E-21; 
while(wJinGUNaaSCR == -4.106384E+09){
double isolcolSXqOIgHbTkSBnVwl = -5.962528E-13;
wJinGUNaaSCR = isolcolSXqOIgHbTkSBnVwl / 3;
Console.WriteLine(wJinGUNaaSCR.ToString());
}uint OfcGQVEGLzEABwKZioVNNsLAx = 779254; 
short QttHPzkSxNJXDscjKpjKulkVP = -27065; 
ushort DjtgnOVfjTMXz = 9289; 
ulong lPjZIpNfFFyFhySKb = 10121535588950158; 

}public void ZQFmkmzIJZUxNlxnDJqpXNbbVJSB(){
byte gwDMqWCsujfBPSQFddQfZ =  254; 
uint ZmPEqD = 963531; 
uint TzUbVBMkogjyYikqGPLeLgXxMegXa = 773556500; 
sbyte QZq =  51; 
string ptPLqWC = "GsaXHnunMC"; 
uint RtPjytcwhi = 623278754; 
int uLVwHAkQCFaUgUICnQl = 33082889; 
if(uLVwHAkQCFaUgUICnQl == 608628){
uLVwHAkQCFaUgUICnQl = 250047;
}double ggyztuaTJspUYeaSjiEliOGVdu = 2.804307E-16; 
while(ggyztuaTJspUYeaSjiEliOGVdu == -2.600706E-15){
ggyztuaTJspUYeaSjiEliOGVdu = Math.Ceiling(Math.Atan(-5));
int[] nJpeuyKNLsBCwZELwCchltasNdC = { 1184926 , 15499 } ;
Random ehPxYh= new Random();
Console.WriteLine(nJpeuyKNLsBCwZELwCchltasNdC[ehPxYh.Next(0,2)]);
}double MTAVUMiDLIXw = 3.353888E-06; 
if(MTAVUMiDLIXw == 4.171596E+13){
double qVHIEEdDnTwMc = 8.8645E+11;
qVHIEEdDnTwMc = Math.Sqrt(3);
MTAVUMiDLIXw = qVHIEEdDnTwMc;
int? znijsUfJCsoiz = 74390;
znijsUfJCsoiz += 45063;
}ulong KDHYgsxtPhZbBZckP = 56357991432548360; 
ulong hfdj = 40029215845178171; 
sbyte glTxWqnnXpHJfHYClNqnnmLX =  -109; 
float adAzqJTPAhYozQUhODlaYayAEMSB = 1.46421F; 
double bunkzRoZlRAt = -0.0009354436; 
bunkzRoZlRAt = Math.Ceiling(Math.Cos(2));
try{
int fODCBhwmFzWKiS = 976530;
if(fODCBhwmFzWKiS == 22328){
fODCBhwmFzWKiS++;
}else{
fODCBhwmFzWKiS--;
Console.Write(fODCBhwmFzWKiS.ToString());
}
}catch(Exception ex){

}string dWkaXFVgxusDwVcEOb = "fXMFOdyAdeIHjZDChIzGj"; 
float aTQUoldfBcDUGqsyXAe = 9.722123E-37F; 
ulong hDStUWpxAksLLXjhnaZhEIf = 79730691171382812; 
int fIqySiikG = 68; 
while(fIqySiikG == 68){
fIqySiikG = 492031;
}ulong QQcIkByVieMP = 7521301316555791; 
long RQXXGwJFhLNNnn = 19829031284010604; 
ushort oIEBgcjVIuZlLn = 59538; 
string EWbcFgSlV = "iXiMNtCIXSizfCRfTnpUH"; 
byte oghxtLCLnlsaXAJmTiGXjefRBk =  120; 
byte VbdXeTGFWiXxgUmOBT =  160; 
byte yLb =  69; 
long apVhlngt = 84201261262558095; 
double CcjTcZsScDzpcJwIFobb = 1.486232E-27; 
CcjTcZsScDzpcJwIFobb = Math.Pow(double.MinValue, double.MaxValue);
int sRViYFJWOHyaelsVw = 15719037; 
while(sRViYFJWOHyaelsVw == 15719037){
sRViYFJWOHyaelsVw += 54323;
}long GNy = 45169150440630326; 
short WoEyJqbsQMEzIc = 11142; 
short POnisRkEjogpLP = 19577; 
int LTKIUMGIQbUVXoIwXJOcmEytTzbVc = 93; 
if(LTKIUMGIQbUVXoIwXJOcmEytTzbVc == 655975){
LTKIUMGIQbUVXoIwXJOcmEytTzbVc = LTKIUMGIQbUVXoIwXJOcmEytTzbVc + 331688;
}ulong PUcJBDQpImaAUNgbDM = 66048489857310773; 
long fboYR = 66258745725055164; 
string lbwhtQNq = "jumbAsYUSMjGYcIdFspiYhOoDjJ"; 

}
} public class zqntLtOxEg{public void twThVSGnZO(){
string yyhXbHYApKLxCzRVGnkAyeW = "fZSxXVJBuDQWEwNQGHYJLxH"; 
ushort LIM = 36475; 
int tBuRcODDubj = 95; 
if(tBuRcODDubj == 334948){
tBuRcODDubj = tBuRcODDubj + 189749;
}uint UQfndKtwummXX = 242958762; 
ushort jFKbceFIBwIQgt = 30804; 
byte KAnmphJ =  93; 
sbyte gemlMkAhGRt =  85; 
long GEnLGnTPMPfNiiTx = 46481073991992590; 
double IkTuOSONHNTYOTxqsyg = 2.866448E-36; 
if(IkTuOSONHNTYOTxqsyg == -2.181516E-26){
IkTuOSONHNTYOTxqsyg = Math.Sqrt(4);
for( ; ;) {
Console.WriteLine(1.908028E+15);
}
}int QUDVRAcQxnNlNXqgLXIPThO = 72156249; 
while(QUDVRAcQxnNlNXqgLXIPThO == 72156249){
QUDVRAcQxnNlNXqgLXIPThO = 955441;
}double NeSnboPNYMtqYbjmzD = 7.747945E+30; 
NeSnboPNYMtqYbjmzD = Math.Pow(5, 2);
int[] WsFfSTBRnbdsOncZGRNt = { 1513742 , 38078 } ;
Random qsIKikPxYORWetzqeRjZZCEzG= new Random();
Console.WriteLine(WsFfSTBRnbdsOncZGRNt[qsIKikPxYORWetzqeRjZZCEzG.Next(0,2)]);string guDG = "FaWxKIMwomlwJVEICZDuSPP"; 
byte bROtS =  98; 
short qOzuIhCmEmqm = -19640; 
uint ywFuZkVQkNsIRYb = 859528; 
double sgen = 1.182006E+26; 
if(sgen == -5.067958E-18){
sgen = Math.Sqrt(4);

}sbyte BCgkHlV =  -1; 
ulong MIzJmHx = 57181517136513479; 
sbyte mgwWEnSz =  -53; 
ushort MQTkNIdNO = 63360; 
byte ixnaKiMsnCbJssmiuGkXkjDepnFgL =  46; 
short bilBomioE = -27207; 
double OkSuwVsh = 8.688873E-19; 
OkSuwVsh = Math.Ceiling(Math.Cos(2));
 bool? RiANunIfDOLYySZZEImKblMR = new bool?();
RiANunIfDOLYySZZEImKblMR = true;string GyJUNCfmXOseGFXJKSuGnHc = "xYqiCdkwSdJFIZcqYCI"; 
float sdQwISWExBYwcyPjXLTEXlCVtb = 2.82569E+22F; 
string MZhNeUCOTkoSwELiFDmg = "QOs"; 
float DUbHig = -2.017907E+20F; 
ulong notVJjRInfQXkjxgDWGWkPSYqIMmc = 68987380396921852; 
short RYfdPpFJRjbFEiS = 16654; 
uint uEayFGgOjhzogXj = 782042; 
double McJNY = 3.86648E-35; 
if(McJNY == 3.252654E+20){
double SPPztuBXbfxjDddu = -3.46749E+34;
SPPztuBXbfxjDddu = Math.Sqrt(3);
McJNY = SPPztuBXbfxjDddu;
try{
int MuMRbDCFDZNjARoiwZwDF = 7003632;
if(MuMRbDCFDZNjARoiwZwDF == 72052){
MuMRbDCFDZNjARoiwZwDF++;
}else{
MuMRbDCFDZNjARoiwZwDF--;
Console.Write(MuMRbDCFDZNjARoiwZwDF.ToString());
}
}catch(Exception ex){

}
}ulong ZfZqdWsjBmkDItqKgifRfyEEKJw = 44988077613462932; 
int MnPf = 451917798; 
while(MnPf == 451917798){
MnPf = 275353;
}string KfCGYPCDwHuCsaxiEtGjOH = "VpLmatXwuwoAY"; 
int lmJ = 638180638; 
while(lmJ == 638180638){
lmJ = lmJ + 749724;
}
}public void bacWJRWcLHomDCizlZgdCzGDgt(){
uint ZpbcYuZtUzMEsxmWMZKhQiST = 67; 
short uBQpphcwFmoiDQFOn = 2952; 
long lKRhJ = 29553446461966091; 
ushort uIxnktRqzLCGZFdGGYmqLJJHXilYx = 22581; 
double fSjgS = -13.98824; 
fSjgS = Math.Pow(5, 2);
Console.ReadLine();ulong sXYuVeBNVuhiedPaRLcwN = 21692877517341287; 
ulong WHeHWqPYqm = 74281073598672984; 
sbyte STuXCnDgBXQhffkCZyTK =  -121; 
ulong hjFmwhzgyAgxclgeJOJzuy = 741701821275797; 
ushort fglOXbhYcRsJZJipSsjV = 23685; 
byte LPzVGEBDdcAq =  234; 
double hHHLxEeMfJHnf = -2.298285E-25; 
while(hHHLxEeMfJHnf == -2.253609E+27){
double fCuDnnGBsmpAkEnJ = 37.32428;
fCuDnnGBsmpAkEnJ = Math.Sqrt(3);
hHHLxEeMfJHnf = fCuDnnGBsmpAkEnJ;
for( ; ;) {
Console.WriteLine(1.538819E+21);
}
}uint gXtdo = 118541519; 
float DcisW = -2.98306E+20F; 
double VGqPpooGxfEAEHElNqycpyn = -743.9536; 
while(VGqPpooGxfEAEHElNqycpyn == 1.908443E-34){
VGqPpooGxfEAEHElNqycpyn = Math.Ceiling(Math.Sin(2));
 bool? anFYwGgjiqTyzsjm = new bool?();
anFYwGgjiqTyzsjm = true;
}float FDfPtDsfwQfONZIQ = -1.298853E+34F; 
int yeqpsEOkxlKtphbVoUaLnUSdR = 39031598; 
while(yeqpsEOkxlKtphbVoUaLnUSdR == 39031598){
yeqpsEOkxlKtphbVoUaLnUSdR = 876451;
}ulong ecBlPyPPhJigMyFX = 87073159027552615; 
byte HjoLqtTTkfCKAyk =  237; 
sbyte JXfnPCOF =  -57; 
float DAnZqQGBxsQmTmAwcbh = 8.455049E+34F; 
ushort DottuNXNeamJ = 41363; 
byte iHGxKTTKmbpfLDgeJggLKX =  111; 
byte AGMlGESiqTBhCMAKFgsdHlj =  242; 
double DBCbkjuJkngzHyzXuK = 4.723979E+33; 
if(DBCbkjuJkngzHyzXuK == -1.092153E+11){
double hgmwOpc = -2.340702E-32;
DBCbkjuJkngzHyzXuK = hgmwOpc * 2;
int? IpOTBkDgBUGK = 4399792;
IpOTBkDgBUGK += 47295;
}double kCywVxsFTAf = -1.657526E-18; 
if(kCywVxsFTAf == 1.805129E-16){
kCywVxsFTAf = Math.Floor(6.562887E-13);

}byte TTheLXfkVqNbJhKbXACmEYCgoDOT =  192; 
short aNyThXCNyxjKOpsDhficW = 16105; 
double VsaQhilzDINn = -1.616608E-17; 
while(VsaQhilzDINn == -3.972604E-07){
double oORWaHOEpMnQeY = 1.108227E-32;
VsaQhilzDINn = oORWaHOEpMnQeY / 3;
 bool? njMhfEeqTFeHkJhZHFjT = new bool?();
njMhfEeqTFeHkJhZHFjT = true;
}sbyte gRGHgImSJNjAKCGJeslNS =  -12; 
long SVjeGkDTXWeSCtAWWazQow = 61401733502272520; 
float fTOlzilyjpCNxOgMSoRS = 9.833199E+30F; 
ushort tPkRkFtDfxMIFmy = 48844; 
ulong GZT = 31166205922301391; 
double kyqSKYwmGWZIabzhGSedY = -0.0001959064; 
while(kyqSKYwmGWZIabzhGSedY == 1.680856E+24){
kyqSKYwmGWZIabzhGSedY = Math.Sqrt(4);
Console.WriteLine(kyqSKYwmGWZIabzhGSedY.ToString());
}
}public void pKjp(){
ushort yTZYQXRBkbGlCgVTgySoUH = 46350; 
int lpnilCGFbRZUnCjVApkVctdhxEN = 112997; 
if(lpnilCGFbRZUnCjVApkVctdhxEN == 157632){
lpnilCGFbRZUnCjVApkVctdhxEN = lpnilCGFbRZUnCjVApkVctdhxEN + 426538;
}string IEsZBaIBpWZSWJuupA = "UTmfMIOgzjaRzlXCshqK"; 
float SNlyMQQMxBJOi = 1.229893E-08F; 
ushort qMTippeBGZgbmZPNAaB = 57880; 
byte YqMOAiLdmgVCaYblDxhXfRKLjCTa =  174; 
long EsuOCxDQVQDMZgLZRMmydMqRgIb = 83827009443897539; 
byte jAPaHlgEeKFACboxmRR =  4; 
int UuYRGfijNOCtIlyxxBjm = 94; 
while(UuYRGfijNOCtIlyxxBjm == 94){
UuYRGfijNOCtIlyxxBjm += 264238;
}ushort owCibDGucPGauUXQXiucYQupQpFW = 61897; 
float sLkVqmsseGduI = 3.276282E-25F; 
float aIgnTs = 0.06018847F; 
string sLXJyiwm = "jZHEGQpxVxN"; 
ushort yyWpNfAslxpbaHgXGllH = 56299; 
long tdCzEWd = 3691725380189204; 
ushort OYYEqCFiiA = 19629; 
byte PPPpDlkIVxhtbmou =  162; 
byte QXTULFcDWouxOTO =  1; 
double jjWkISDQkpooPCp = 1.261921E-09; 
while(jjWkISDQkpooPCp == 4.261175E+30){
jjWkISDQkpooPCp = Math.Ceiling(Math.Sin(2));
try{
int WAkebtzK = 3154564;
if(WAkebtzK == 30805){
WAkebtzK++;
}else{
WAkebtzK--;
}
}catch(Exception ex){

}
}ulong VEjqNwHRJT = 45246943619090714; 
byte ZJOzJLkLIOQHLWiyihXSo =  122; 
long YKZUFqgbbeolPMIcjWiVd = 40815945102799818; 
ushort BbeAZLqfqHPFG = 42079; 
double yAUJspNHhuTWLXpJHLPk = 4.729271E-20; 
double mKAbkWqzNckn = 7.817762E+26;
yAUJspNHhuTWLXpJHLPk = Math.Floor(mKAbkWqzNckn);
for( ; ;) {
Console.WriteLine(-7.870777E-14);
}uint qsVemoPKoNdsgYCiHWNEkpAoWhQ = 15; 
uint aWAhGmj = 340820; 
uint yNSFtPnWQYMXLbSGhez = 53347494; 
ulong cwIQWWywwcGSNysPbT = 80666989137574413; 
long xTT = 16535289299531862; 
string EpUejfiQpyOp = "tOwSKj"; 
short YCjDkEJicbCkqI = 30476; 
sbyte WHzlfxeNBQaNTphMWFPLC =  121; 
uint HnwmfQMQpiNDQjTe = 80; 
long IwiMuAucjwiYeYGSbSHLejy = 29783833445029310; 
long OSkeJSHwJH = 60084068854292972; 

}public void jYKyYziPE(){
long APyJpfuNsZZp = 87557024634152970; 
long tCqeulHBdcOqggDfiMj = 17176839831772166; 
ulong JCBITxVskMigS = 41408809666688369; 
double XxVfsIQAPTf = -12.01465; 
if(XxVfsIQAPTf == 8.083215E-20){
double abgTqlgjzcFNLkDkYpEKwlunMBxn = 1.316165E+10;
XxVfsIQAPTf = abgTqlgjzcFNLkDkYpEKwlunMBxn / 3;
 bool? UaoWpbPfCHbNntKpLz = new bool?();
UaoWpbPfCHbNntKpLz = true;
}short SLVH = 26839; 
double yiOjIZtRfEWUsyPzB = -2.797436E-05; 
double kBZueEqRGVOFaTN = Math.Log(1000, 10);
yiOjIZtRfEWUsyPzB = kBZueEqRGVOFaTN;
try{
Console.WriteLine(yiOjIZtRfEWUsyPzB.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}ulong DzeUUFD = 33579757113372408; 
ulong mluzTPzBmQ = 40025345758568297; 
uint kxJVoUaZTAelelFmtLKJ = 37122185; 
int VEyGSthTkecRDAiYPAIOMwzm = 701165356; 
while(VEyGSthTkecRDAiYPAIOMwzm == 701165356){
VEyGSthTkecRDAiYPAIOMwzm = 835561;
}string PUaMywgAqlaajNKTCLUgMTdAe = "KhVFquck"; 
string AyCushxBuydiaFaQXUfW = "BHmMZEMUHXZLbtejBPExDeHgB"; 
sbyte FCxhefRZQnJ =  -37; 
int GOAdyZkWHxfDAnwZfkSmht = 79; 
if(GOAdyZkWHxfDAnwZfkSmht == 200364){
GOAdyZkWHxfDAnwZfkSmht = 395718;
}short SxZLWqgcdhfJUZcCHDEQhyBqEmE = 12641; 
sbyte PQIMFKDPqeTdhKgAzjOtYIRzNu =  107; 
long KOyTkto = 46456834412296647; 
double pQjdP = -1.738715E+32; 
double OVHIpwjXqS = 4.015368E-25;
OVHIpwjXqS = Math.Sqrt(3);
pQjdP = OVHIpwjXqS;
object ugxsBJdfydVwBdp;
ugxsBJdfydVwBdp = -2.103994E-16;
Console.WriteLine(ugxsBJdfydVwBdp.ToString().ToLower());uint GzEBwNKHJpgI = 554127; 
ulong VgHeDHfGQqVTkYOLHfzxRZJECJi = 8885758500602127; 
ulong uQYsXKdRRxKDxaqeqeVycDT = 76717572442990305; 
short Xmp = -441; 
string oFMLmzqHWoEAhMpKRWBfwRtEOz = "CzNbFmkGSqRG"; 
double WXwKEQGKnwtZYzzlhXPgPhinYmX = -0.001458691; 
while(WXwKEQGKnwtZYzzlhXPgPhinYmX == 1.129183E-13){
WXwKEQGKnwtZYzzlhXPgPhinYmX = Math.Ceiling(Math.Asin(0.2));

}int WGgCNDqVfK = 162282; 
if(WGgCNDqVfK == 545395){
WGgCNDqVfK = WGgCNDqVfK + 342662;
}ushort AuLpCRJozAeVqiqJVZmy = 30461; 
uint oZQkWUjmRAhNmFGd = 8582; 
uint SQks = 8396; 
sbyte DeW =  -82; 
string xpLGMuolnxixddoVXFauIAPzXe = "hKMEYLtiptKgdOGDOiROdM"; 
long qiIkWB = 32308049571059591; 
byte LVZDnyWGcWaogqJxcFlPch =  56; 
ulong bAwId = 5358162709711252; 
float mciaAYBpZLAWHwJDKjylYBYYVe = -3.783301E-35F; 
short KYMsDaZkogCjxXFf = -13696; 

}public void wAzDIIVMejfwJybAaB(){
byte SJEzVpKOea =  96; 
ulong MIPbgqexRWPfUzdiXWeVaMNW = 79989668047843266; 
short XQlyUjfSbMOtbSHGgeo = 25770; 
ulong VSc = 22170896684018599; 
short mWBcUPhKaiHGAqDCY = 1391; 
ushort WVuutjZETTPDMqtJGcufIpUK = 27277; 
float QMIUywAFsDfJLQER = -4.112327E-11F; 
string iXTVWgphHaLYAKjSkYshmNbgAt = "odzNqqasSTMYqeqXqRgja"; 
int GibWHxWMwDxadhntIzsqZA = 1; 
while(GibWHxWMwDxadhntIzsqZA == 1){
GibWHxWMwDxadhntIzsqZA = GibWHxWMwDxadhntIzsqZA + 696293;
}short jKJsSEouuWVg = -11478; 
ushort emPczALJoNgiaULHmyctMsMpLem = 5478; 
ushort ShGeXJZIRTzbuG = 47247; 
ushort BJoJJnbiNC = 32154; 
short mHlTUYDVWcHFG = -21375; 
float YgFpPNwgNjTEVkgBoyHSPC = 1.964706E+18F; 
float EHIddfhzOuVc = 0.08365028F; 
byte fLeZZ =  184; 
double oiDcWy = -1.310491E+25; 
while(oiDcWy == -2.278227E-25){
oiDcWy = Math.Ceiling(Math.Tanh(0.1));
int? aITtSLCOUEeDqZd = 1918847;
aITtSLCOUEeDqZd += 59083;
}string dXVlkqUfOCJL = "jcawqpQDDZMGhIppETaeYtKBt"; 
sbyte IiwatEFm =  -79; 
string TomPCYiGdJFQjIIHTQtBckeiugZsb = "wfoFaLSiSMqNcCdtkt"; 
long LxRBWIjdwuYQFAnzBGHXPKYIipiD = 30468616422296701; 
short IsCOOyNBhDnFTGMOibzjsHA = 9412; 
double ztiBIojNXowbV = 1.371539E+29; 
while(ztiBIojNXowbV == -1.830489E+08){
ztiBIojNXowbV = Math.Pow(5, 2);
try{

}catch(Exception ex){

}
}uint LEXBqhSVabeipPLZQVFyOuWjc = 2013; 
uint RSf = 2754; 
int zbUtJGoBGhQl = 459528796; 
if(zbUtJGoBGhQl == 129261){
zbUtJGoBGhQl = zbUtJGoBGhQl + 76942;
}string HOzaXtUjiYU = "jsB"; 
ulong kPh = 4017500001215437; 
sbyte ZjlnMLDUxoLeqHmPcFXURWoVljjEB =  122; 
float cjZLQVGQmhOA = -5.86326E-39F; 
sbyte ILWWGmicVpcDCYtyztbhHDZgZgHK =  -90; 
ulong kelWPcUMPaWPATd = 61699703076999639; 
short MQXzG = -1302; 
float SSyobF = -956243.4F; 

}
} 
public class CkbxhYqWbt{public void LQPiNKKVINqJCUGsCFVlkxkNqt(){
short bdzezDUhTPYIXKEC = -19352; 
uint DCGPsghJoFcRtwiT = 290511; 
byte ZayzMGmn =  130; 
float DQTMYxnfYqTGDRXFlPRZm = 2.149884E-13F; 
uint dhcMTThxVAbBJkxpi = 8621; 
sbyte VHPbaAKSxEztj =  99; 
short tezMiHsW = -27002; 
int uKdLzwhiBnLTuzEzVD = 48; 
while(uKdLzwhiBnLTuzEzVD == 48){
uKdLzwhiBnLTuzEzVD += 803259;
}sbyte ulnoSXyVoitTa =  116; 
string OWJlemFixuXdnlFnCdbVVl = "OwziufcCixFuig"; 
double gHfTzZtHdFf = -8.779199E+32; 
gHfTzZtHdFf = Math.Truncate(gHfTzZtHdFf);
Console.WriteLine(gHfTzZtHdFf.ToString());ushort QSMT = 22501; 
int UTHaYwjyMIf = 37; 
if(UTHaYwjyMIf == 37491){
UTHaYwjyMIf += 997960;
}float KubmwdohlZoq = -612497.1F; 
ushort eNcJRqTjsJQdeLRMbYSVTHlWKn = 63321; 
uint qUBWeaoXiDtBSU = 27480066; 
byte BiDp =  59; 
long ZyBfZDtwNbOohZKCP = 53999436210121057; 
uint ypUaIQuf = 763025; 
float ZafeTXZ = -5.388083E-33F; 
float OyQlhYLayCqVeTafAOT = -8.808778E+20F; 
double xAoWSDJwhHJfESEmhSNwI = 1.631125E-32; 
if(xAoWSDJwhHJfESEmhSNwI == -2.114322E+28){
double KSSNtMScTz = 2.740201;
xAoWSDJwhHJfESEmhSNwI = Math.Floor(KSSNtMScTz);
Console.WriteLine(xAoWSDJwhHJfESEmhSNwI.ToString());
}long zPFzyFFNzwRqLGqEs = 71931194722916640; 
int dhHoNTBoGKlDgEtzBaVfUBQ = 10971783; 
if(dhHoNTBoGKlDgEtzBaVfUBQ == 527001){
dhHoNTBoGKlDgEtzBaVfUBQ += 220798;
}ushort XRQ = 34095; 
int gdzkpMUgqIbIPTFFBUEqUjmRZ = 18; 
if(gdzkpMUgqIbIPTFFBUEqUjmRZ == 720895){
gdzkpMUgqIbIPTFFBUEqUjmRZ += 317752;
}string GuWyUEOVGOTaUxEwtpVXnXX = "HllDlTGqYUizIhHpPxnwtQeIG"; 
byte wwfAJyeQJskKNiCcZmeNlKFhRKC =  116; 
float FXYYjW = 8.723269E-08F; 
uint EqXY = 742614; 
uint ROfuBBJMQ = 644732423; 
double HZCNnMOYGjFMcjVMHEeioGdbjxy = -0.003151101; 
if(HZCNnMOYGjFMcjVMHEeioGdbjxy == 1.219105E+20){
HZCNnMOYGjFMcjVMHEeioGdbjxy = Math.Sqrt(4);
Console.ReadKey();
}double VIzNnOLpKyDcj = 1.854954E+21; 
if(VIzNnOLpKyDcj == -2.962441E-11){
VIzNnOLpKyDcj = Math.Ceiling(Math.Cosh(5));
for( ; ;) {
Console.WriteLine(3.779657E+17);
}
}double jWzaGnxqTf = -7.721242E-35; 
if(jWzaGnxqTf == -1.761482E-31){
jWzaGnxqTf = Math.Exp(2);
Console.ReadLine();
}byte PBJkYjPRXbnTUauJt =  4; 

}public void UUUobzbqjMEpVFJQPgfndiZPAAEj(){
sbyte uzqEsIUlHMWSGftLtnljFDmN =  96; 
ulong fPCVSlwqeZqmbVEAuqkDTwPgoYV = 36362114119532468; 
ushort QeHkP = 18653; 
long bko = 40204890490342295; 
string HUqkzUwIHnghw = "lFqdfdfnOGjQqgbxMd"; 
ushort jkpapwpuuXhTIpjeSXFXp = 23153; 
sbyte bJDnGjCdLLtbnqOLNLTRzq =  -90; 
string XFOzAuSeGZIScOjwehWWiQCjTjFz = "mLCCqJGNGlq"; 
uint JsgTKXKwuikeOgdFFoPtPsnfqLfF = 47563645; 
double wlgjdaAIPUSCpyDCCia = 217.2649; 
if(wlgjdaAIPUSCpyDCCia == -4.895612E-15){
wlgjdaAIPUSCpyDCCia = Math.Ceiling(Math.Sinh(-5));

}sbyte xfosUPDyfficuPaTLJQaPUQ =  48; 
int QAcisBtUlqRbKymTdDIk = 49926740; 
while(QAcisBtUlqRbKymTdDIk == 49926740){
QAcisBtUlqRbKymTdDIk = 756281;
}long YDoFPnY = 47750138103930521; 
long JZtN = 80289397321711112; 
long FSkVKblmwzjy = 18996444876690033; 
sbyte pwcNbExoJaMGjQAGTHQwEUE =  -6; 
long bRRtotiQDPTXaJQg = 45126395074274432; 
string BNkwRD = "DCGz"; 
short tytnKMWHgFxduOjVSPopiS = -5422; 
int YpUzhk = 53175704; 
while(YpUzhk == 53175704){
YpUzhk = 202473;
}double QgAthOqyPTHJ = 1.04582E-24; 
QgAthOqyPTHJ = Math.Pow(5, 2);
Console.ReadLine();ulong etfAS = 36119898194915386; 
ulong PnByx = 32371539229462571; 
ulong HFEHKzWndpmOMwQYDkSQ = 65760882244717030; 
long lttHogy = 51728799031402738; 
long DcQgFMYQTUjb = 24758451663100233; 
string KCxgkX = "qEkUHSjVcHHuRwlneuFixDcpp"; 
short tnLzCaYCqZhdkA = 9875; 
float manLynsNHPwPBVF = -2.055558E-06F; 
float sifuSUVoWpuKOKGcoaLV = -1.407291E-11F; 
ulong lDMQdEKOIbBohHwRDZwxmeHfUSp = 77814928331866807; 
short ektHGb = 16815; 
sbyte VIFUtbXpIgV =  32; 
double mqcNwCSWz = -5.329643E+13; 
while(mqcNwCSWz == 6.770647E-36){
mqcNwCSWz = Math.Pow(2, 2.1);
try{
int acbKsnGEkkRAMtyNi = 6417560;
if(acbKsnGEkkRAMtyNi == 61541){
acbKsnGEkkRAMtyNi++;
}else{
acbKsnGEkkRAMtyNi--;
Console.Write(acbKsnGEkkRAMtyNi.ToString());
}
}catch(Exception ex){

}
}short GEYsRS = 13372; 

}public void LsdChMFMMbJyYZWn(){
uint mohnEFAsgQzus = 100420; 
ushort MLagtuMEGXTiGHLSwGwtyPAb = 53496; 
int nqKmsDjjEAnTUwKYHGiBiODW = 682271669; 
while(nqKmsDjjEAnTUwKYHGiBiODW == 682271669){
nqKmsDjjEAnTUwKYHGiBiODW = 215921;
}double NGzlVLCGwxh = -4.601949E-28; 
NGzlVLCGwxh = Math.Exp(2);
Console.ReadLine();double hnTfEiPSjGlboPW = -2.165654E-33; 
double wlhZXnypXaVJkyCDR = 4.458142E+36;
hnTfEiPSjGlboPW = Math.Ceiling(wlhZXnypXaVJkyCDR);
Console.ReadLine();ulong SNjCaxJqZYbyjgYlOeENdO = 25913458475282320; 
byte gCmTnP =  3; 
float CFuVtsDKCxMFZJewPqNlxV = -1.044881E+15F; 
float IUfYYdNaCjY = -1.644416E+21F; 
double DUoCChTizzynFpGqKZcK = 1371613; 
while(DUoCChTizzynFpGqKZcK == -1.287981E+11){
DUoCChTizzynFpGqKZcK = Math.Pow(5, 2);
try{
int iMnbjWwayFsCGUVWWcYSJn = 6893943;
if(iMnbjWwayFsCGUVWWcYSJn == 99663){
iMnbjWwayFsCGUVWWcYSJn++;
}else{
iMnbjWwayFsCGUVWWcYSJn--;
}
}catch(Exception ex){

}
}string HpEuhnqCNUTpcbn = "baVClTwiPwCUilWUSzpNuyCaTB"; 
ushort UqDUwpmJeClNVuGMMxsCYZzFLCz = 37633; 
uint JMABozTIATYmQqlERaSPnxiTWVT = 165827018; 
float KjjSBnwfhTeXADLigshKzMKHl = 3.486531E+07F; 
int FUAFaocHzMoxU = 91073713; 
while(FUAFaocHzMoxU == 91073713){
FUAFaocHzMoxU = 323151;
}byte MGajuXADHXIRMDYFszmIDJ =  157; 
float EbPyNUosI = 4.015882E-17F; 
uint HqEdRPIGihWFgk = 56; 
sbyte ZiWR =  109; 
long Uctze = 21825647106874450; 
float kIwUSSGKlPzPopzPwSZXMngey = -0.0002591577F; 
sbyte hzLliXoxcSmn =  -34; 
sbyte GUwRzcClATeWBpxHwxqep =  66; 
int sjtsaM = 36620396; 
while(sjtsaM == 36620396){
sjtsaM = 558453;
}float LpGmksdaOfGlRRcdtSq = -0.01318413F; 
short ilNkMQcSTPOMazdDkcqwn = 15536; 
ulong SpceGlChxIybyhsjLmNxdfbGopg = 54683327927422645; 
ushort YmImHotacecKgLbZqRMnyPo = 5595; 
short Emb = 4354; 
int XxNZfjxRJFmkeDahtygbHgqi = 562855175; 
if(XxNZfjxRJFmkeDahtygbHgqi == 712114){
XxNZfjxRJFmkeDahtygbHgqi = 124683;
}double AmNLXbUhuVGSDeZN = 9.331298E-07; 
double EbCkQfD = 1.913228E-27;
AmNLXbUhuVGSDeZN = EbCkQfD * 2;
 bool? PpjsDYHMBAHTElHomwbWeXGuZs = new bool?();
PpjsDYHMBAHTElHomwbWeXGuZs = true;ulong kbcXymRAwnXd = 46421733553165303; 
float XwYOadFwfZwjR = -1.32592E+29F; 
uint tfJDMlNERiQejMtBReIZKlOVa = 96; 
long DfejQGqhRkjkkjVOp = 32703011962007414; 

}public void gsNCEme(){
string cLVtfcsZTz = "AcTgLmLHTDNRKJzYuoIWgiU"; 
float wChY = -4.367262E-39F; 
uint MJdAHRqCyiMoTkLHeXLlCIph = 8; 
short EJDDncYauAOyquJaiDikRB = 25482; 
short mmdsWzDDGY = 19080; 
int bRmLlFj = 9036; 
while(bRmLlFj == 9036){
bRmLlFj = bRmLlFj + 981912;
}ushort eUdOjmaPUIzakVLxTwZCsXgnoic = 23234; 
sbyte cXbJz =  56; 
float wVb = -2.745626E-11F; 
string EYsjKfUZWCgOPzbkIJPFCZD = "ZCSZdBqFdBMkKi"; 
ushort iXfgsYfCuKtsEzmFwVjhmVVxkCKoN = 54071; 
uint mPxEUxebxqGugYbG = 1275; 
ulong YnNmcKVQnQPEiUfObJLZQiMW = 55408036411809205; 
string BZciBkCoNHDbkmTmEFwPYQ = "QXCuJpjPhCxtgLThB"; 
uint weunpctmBxkmNLxml = 813642464; 
float ERuBOTNDLxVqywyxJkqodJsWoJJKz = 6.876872E-27F; 
long DRnZNDiHKQapVppZQDP = 5679645064254308; 
byte ddnk =  187; 
float jxWJiItSsHtttLJeul = -4.123238E+12F; 
double DByYFHMMeiwMPtGfpVBcgRS = 2.712134E+26; 
while(DByYFHMMeiwMPtGfpVBcgRS == -8.19439E+36){
double IwUwlkdNfG = Math.IEEERemainder(3, 4);
DByYFHMMeiwMPtGfpVBcgRS = IwUwlkdNfG;
int[] VoznVfhTILIMfllNSfyi = { 9356600 , 51398 } ;
Random uzkUfSO= new Random();
Console.WriteLine(VoznVfhTILIMfllNSfyi[uzkUfSO.Next(0,2)]);
}short NlxfCHdFePOiNAeRBtOx = 16634; 
ulong CQntQntWgxaYUPfll = 87421992441777286; 
string mEM = "UXKVommnpLSINNBjcwhDuULlVB"; 
ulong PnftuEgbkTRYYODtyYDLURYSwjzam = 40736073049526694; 
uint iGAZiEjFG = 3282; 
ulong XfdYpnXkegOOTcCKpAPYMuewRPnl = 69443199365273395; 
byte NqCceALkoCJVpSDQ =  54; 
int JgB = 722553166; 
if(JgB == 451857){
JgB += 859466;
}sbyte ljzQNyyJoFKGBNEKERtTTmpC =  -37; 
byte QxTcSfAozxAf =  23; 
short oxZBZlhWIHAlWZUXy = -727; 
sbyte mwDRAayemdMoBqXTDSmf =  37; 
uint wtUEpFAc = 98896370; 
long TUhbZPdJFU = 46456729035412456; 
double MkgzGxgAObfoQbeZuQnqFSHCihYoF = -6.151322E-06; 
while(MkgzGxgAObfoQbeZuQnqFSHCihYoF == 4.486981E+15){
double wDIEVEOVVEXaZPYIwUFGp = Math.Log(1000, 10);
MkgzGxgAObfoQbeZuQnqFSHCihYoF = wDIEVEOVVEXaZPYIwUFGp;
try{
int JFMofAwmMwLKaXcNmmusEeHwqoI = 9691890;
if(JFMofAwmMwLKaXcNmmusEeHwqoI == 4086){
JFMofAwmMwLKaXcNmmusEeHwqoI++;
}else{
JFMofAwmMwLKaXcNmmusEeHwqoI--;
}
}catch(Exception ex){

}
}
}public void QeCOntnYuXCTdkeAqEX(){
short QRqetzHUtLeLHxpRQRmhtUKDooNZ = -12916; 
short aLJZmjJVVHebmyhyOPJC = 15876; 
long FWgPQdMI = 12742409805700463; 
long VsBnTy = 748861650273618; 
short ddfwSRjZBZktizqM = -30723; 
ushort sKpdKJKlNxXmfOAUtXlnehCOT = 15405; 
ulong tHAGweXROaxMiGxDDNeGKFXPPfMki = 70183083992467698; 
short aQIgtBBfzLYkghgoOJKQptNJk = 21156; 
double yNpGkWdPbzFnnZnAjqcpQxdy = -1002.658; 
while(yNpGkWdPbzFnnZnAjqcpQxdy == 1.203561E-34){
yNpGkWdPbzFnnZnAjqcpQxdy = Math.Ceiling(Math.Sinh(-5));
int? aKTsJwwMdEcLDsDdRqSmyjOsRajDP = 9732219;
aKTsJwwMdEcLDsDdRqSmyjOsRajDP += 55470;
}byte DVwCRVBCIlTWEDnQQLBC =  206; 
double FSac = -1.387664E-31; 
if(FSac == -6.663663E-39){
FSac = Math.Ceiling(Math.Asin(0.2));

}uint aHlOcVuMiwHdUg = 24255090; 
byte TQCNlFnFpQzJHJZFmkRoBbXhWWY =  132; 
uint zoZyhLUmuIBzGEnBP = 64242345; 
short zwYwSSuQoGNfOsjJiCoLZQjPbx = -7883; 
float gayjIaTbcpSByf = -6.872995E+19F; 
ushort ldRHxOFUIkitwzAtiGoWgspHf = 14763; 
ushort DLhbJGhQ = 39500; 
uint KFNsJSKUtadWxzyLUO = 5250; 
byte usWdhKQqKOJb =  155; 
short ZIOxqdjzPMDlV = -15622; 
string YtskwxFLZdcpo = "uSfqVfyeTn"; 
byte hqFzVdItLJngYDcdymaTiCqLfE =  92; 
double TAdaComfBiYJWELFZssdtIGnpKGa = 1.811941E-10; 
while(TAdaComfBiYJWELFZssdtIGnpKGa == 2.390557E+17){
TAdaComfBiYJWELFZssdtIGnpKGa = Math.Floor(-2.9448E+24);

}ulong fKTSGqwxF = 1516813982053982; 
byte NmoWF =  39; 
uint EFwbTyULEcIf = 9658; 
ushort BLKWnyQJxoba = 54085; 
ulong KgYRRjEJnV = 80710730961711156; 
double TyDefPMFFxTUQO = 1.946203E+37; 
if(TyDefPMFFxTUQO == -6.410103E+09){
TyDefPMFFxTUQO = Math.Pow(double.MinValue, double.MaxValue);
Console.ReadLine();
}string EjbCTQoJUKuHVfxLqmEeCUAFp = "fMfzDGXGPCuQDPwocTExXaZzwtAYq"; 
long ykTLEExgPKoqUqBKPKlSGa = 51334515724302362; 
double fQXFJdSMGM = 3.27699E+17; 
while(fQXFJdSMGM == 3.636499E-32){
fQXFJdSMGM = Math.Ceiling(Math.Tanh(0.1));
try{
int htwIBiBxKk = 6502387;
if(htwIBiBxKk == 46915){
htwIBiBxKk++;
}else{
htwIBiBxKk--;
Console.Write(htwIBiBxKk.ToString());
}
}catch(Exception ex){

}
}string ZIZxEGkkizjiwkxmbQXtTIj = "RLZffhMXH"; 
ulong TxnHJBp = 44423488703346903; 

}
} public class SCHUsyNFTHffbMmCIbEQOinTDERYZ{public void WOliTxZSRwK(){
long cukcHbpNbOdkJmOsOGOFDKq = 69355564063562235; 
sbyte wlzsB =  -76; 
double EVZiXkilBdPVdksdqjOkO = 1.37451E+26; 
while(EVZiXkilBdPVdksdqjOkO == 1.160649E-29){
double nwSVYOqLVIIpogsjajTwaqyLWHduk = -9.180249E-38;
EVZiXkilBdPVdksdqjOkO = Math.Floor(nwSVYOqLVIIpogsjajTwaqyLWHduk);
Console.Write(EVZiXkilBdPVdksdqjOkO.ToString());
}byte TOhjweeUuoHnfSkGTHaBtsRcsQz =  83; 
byte DkIcwVlSVbteYRQ =  117; 
ulong SVZPUJC = 44562611445394642; 
double wGcyouaMZgotamUapAqzOpY = 2.150794E+28; 
wGcyouaMZgotamUapAqzOpY = Math.Ceiling(Math.Tan(1));
try{
Console.WriteLine(wGcyouaMZgotamUapAqzOpY.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}short Eeokjh = -17734; 
short HqawQYYVqLsCZnKjnQnWYMziMOSER = -27704; 
ushort ABddxSWdsgtwZEFQFaWsYt = 335; 
double SKcguemmHczkHOxUjESsdihibZ = 2.139446E-27; 
if(SKcguemmHczkHOxUjESsdihibZ == 7.642536E-31){
double hUXXcghtCfGYlBzOQiE = 1.122354E-28;
SKcguemmHczkHOxUjESsdihibZ = Math.Round(hUXXcghtCfGYlBzOQiE);
object ewmehUXYcxknamHPkWQGdxaGII;
ewmehUXYcxknamHPkWQGdxaGII = 1.04499E+13;
}ushort WHHyboIZtdaiVsbW = 56222; 
uint wjygYlCcJjkhhFBCXB = 51; 
ulong zhesiYBUTPWdbpzBIk = 55873675198327195; 
ushort exNXRRLobGcEDPpwkdDjdySU = 1447; 
uint JZiAFDKQycZmjGizbiW = 19694541; 
uint MPblNIFSWslcpTgzaSYTslR = 5858; 
float QMePwjlxKEqXiTkcVYDTGVKVSlGwA = 1.667342E+13F; 
long JOfRDRTexxVKVPZgiWubasUbiX = 5588848021616616; 
uint fksgK = 18379099; 
uint lXxHnpcoGSqTdJFIpmALaaQp = 25025073; 
double msliwlWaNWEHJGemik = -3741.99; 
double lggMlsAbmGhzRMFVtKSbwVEg = 1.49934E+14;
msliwlWaNWEHJGemik = lggMlsAbmGhzRMFVtKSbwVEg / 3;
Console.ReadKey();double GHjYSHTdupHidsBpEMpURPy = -5.432105E+26; 
while(GHjYSHTdupHidsBpEMpURPy == -4.62059E+30){
GHjYSHTdupHidsBpEMpURPy = Math.Ceiling(Math.Tan(1));
try{

}catch(Exception ex){

}
}float mCDfwIgpmh = -3.872321E-07F; 
uint nLabCFdeZCWMKq = 96905640; 
uint IMzUBXgSMTWCwEobxkUJIplhDa = 67; 
double GnIxOOYujdUKSjiygsyJUzeR = -2.513526E-13; 
while(GnIxOOYujdUKSjiygsyJUzeR == -3.729717E-14){
GnIxOOYujdUKSjiygsyJUzeR = Math.Ceiling(Math.Cosh(5));

}short DeyEOnn = 13189; 
string WNXEQnSfskkUQdzqiLhUh = "MepuVIRJYUZyCqSRxifjhgX"; 
long uexfEDhBia = 71039829421013651; 
short tChAsoYPYGGgIfBDwEuqSXspaEZ = 20677; 
float zUHNVTXWOnEsDEwLFpSITgSZcwi = 0.08096496F; 
ulong hUcgLBUkHG = 30905765256330176; 
short JXmbscILAKgyaYwzAxKkLflbwqLkE = -30948; 
ushort jPGkHaPJSjLXAaPhsbUskdnEMkH = 896; 

}public void xhzokmktkRA(){
long sBw = 55309781639777015; 
string fCczwtOqxcmInOFPhjwKj = "NhzJmzNEGqmBgKpNwfTKWz"; 
sbyte nHXPMqMsBuOVaNgpETy =  106; 
ushort HVzwOoQAPnZSwHBYSnFVaNV = 26771; 
ulong ZEgdbmkBZcppDxCbFBVhs = 80650922489395069; 
float dJfpmtMmNHUHzICLhtYhwAEqAPBcT = 1.217701E+17F; 
ulong cmIygo = 33220616892017656; 
double zifPMFEeORySKuRV = -11.14665; 
while(zifPMFEeORySKuRV == -3.226826E+28){
zifPMFEeORySKuRV = Math.Pow(double.NegativeInfinity, 2);
int? KoSQiTsHosVDGkgbzy = 7414942;
KoSQiTsHosVDGkgbzy += 24827;
}uint nZxFmE = 93; 
byte FAWayBQdTBaUoqelYSo =  163; 
ushort XpimHoYwokPyLLkWnRNJtaf = 24688; 
long AbFiI = 21257288486367960; 
uint ylQfR = 39419775; 
string FzU = "uWkPDCPWQfDlkVhFedWlEV"; 
ulong nSAeaEFjJBw = 67342113932827130; 
sbyte THKlwWBCIh =  -6; 
sbyte CKbKVmCsELBDjcxFAunIbulPTGH =  -110; 
long sWy = 32984692790225612; 
double WjKlTdWiNkB = 1.48054E-28; 
while(WjKlTdWiNkB == -9.912541E+24){
WjKlTdWiNkB = Math.Ceiling(Math.Cosh(5));
 bool? HtzdkWPglNEGFDqEJLGqyNciHIXS = new bool?();
HtzdkWPglNEGFDqEJLGqyNciHIXS = true;
}ulong xdgZuAUqpxDZWJ = 49950790815961121; 
uint wmASLxHdgxcUEOz = 14390456; 
float OKkhnFpUnionTFJEgZbGTQ = -2.033991E+25F; 
uint wydnMtpRLOjekmuDtUS = 17; 
short jsJfmSXjtIIjTRZxWYbUVtbOOlq = -4255; 
int qyg = 863603; 
if(qyg == 266312){
qyg += 61498;
}double ERxEdBLtyWkgwZRpUqaBCFs = -8.565286E-24; 
if(ERxEdBLtyWkgwZRpUqaBCFs == -4.67402E+35){
ERxEdBLtyWkgwZRpUqaBCFs = Math.Ceiling(Math.Tanh(0.1));
 bool? enpenJoLKGpU = new bool?();
enpenJoLKGpU = true;
}uint OdRQlyUNVzqMEOCkTzIxQRTDyB = 3144; 
byte RUQaKuyx =  169; 
float jDMugZgQRHudedWjnsLeEBKIxV = 2.752915E-33F; 
sbyte yQNCHngiqClAuFyRMXNjidY =  74; 
short TeXkycJdiEpDyJcfbiTdf = -25943; 
byte jGqXFtSXlXwckfeiRqw =  223; 
uint BdgdcPW = 5515; 
ulong sbRLLAQMspAnaVVeKmPKqeaemQC = 18721643972111613; 
string yfSFhhTbQ = "igKlZN"; 

}public void EmaM(){
uint XAJqqdHSbRSmhXeiXhdyKsitQaI = 591974; 
int BZkU = 47; 
if(BZkU == 941593){
BZkU = 190417;
}short oRYRQY = -7000; 
short IbkGpoBpxcmxOsmWdZLfGgmulU = 2518; 
float RGjpjSY = 5.476005E-17F; 
ulong plBgbLC = 22226399553781786; 
uint QtxOKhFycfEAoXOkCQuHftB = 472; 
sbyte eHBgWqkiVs =  -23; 
int VKZGX = 44; 
if(VKZGX == 762212){
VKZGX = VKZGX + 164100;
}ushort UIySdLETBRDYHidOTTfipLxuVu = 3069; 
long fTsNdxDMcnK = 61704014351463725; 
ushort fUckDjld = 61727; 
long WDuYkkEMCbQFwffpEXndk = 45711489042826542; 
short fFbgIaUlxNfRhpoHfyTJ = -25229; 
long zHxwZVJczltKauYzpJM = 37472217629328757; 
ushort uHDy = 64692; 
int kGVqOKIGOGomgYEa = 8168; 
while(kGVqOKIGOGomgYEa == 8168){
kGVqOKIGOGomgYEa = 454506;
}long mlZeFRmbQzm = 4420351473421786; 
string wpeQGhHLkqNUE = "CLEa"; 
double nyIMTmZMIWqmI = 2.801228E+26; 
nyIMTmZMIWqmI = Math.Ceiling(Math.Atan(-5));
int? XthVcnZBlHlFQp = 2308551;
XthVcnZBlHlFQp += 3943;string iNczaYSUoDpGBLnqhZFE = "YfEQZ"; 
double ghgVoAM = -1.307118E-21; 
while(ghgVoAM == 4.218135E-21){
double ZwNcKBsmyBD = 5.004868E-13;
ghgVoAM = Math.Ceiling(ZwNcKBsmyBD);
Console.ReadLine();
}uint eFiSGeMe = 40639354; 
long fLPDOUmPjlgsBVDdxnsOZgL = 19221045540977494; 
byte gtdiZ =  231; 
float KbSyRqKDIIydXOYaFcO = 2.213778E+33F; 
short AVnuCzVytAhofS = 15591; 
long nOSgohgtwcEWgWJwGsCDX = 30642071408105424; 
float SufIhyo = -1.681832E+20F; 
sbyte gws =  3; 
byte AUAAkfMfLfVkEqUDV =  183; 
uint oBY = 1411; 
ulong VbIuKSbpEtIIn = 22282861832284089; 
sbyte BHYknYWisxMQecLlsD =  -123; 
short mdnINdH = 29141; 

}public void EEPxhO(){
long NqIjokDEyuHhIIPQwmDL = 58837444516440543; 
string VWBMPKJYXSLdoJZPzWOZ = "Ula"; 
string xqYdswDQA = "HWSfgypKZAlmqxNWtbHbL"; 
uint DnsZUBpBAiohCeJSEFCb = 6320; 
sbyte Dpx =  -118; 
uint zZgooSO = 418841; 
byte YMdbnkSKjcyjzcDeqyEen =  0; 
short obmPRdgissHnfYneFsqfbQ = 10132; 
byte OxZSfwYwsNmYcTfWnSgwnYt =  48; 
uint WJkKAtabdHgpmXJT = 22926222; 
string CzljNX = "RzPdQsFzokJmTXbQz"; 
ulong GjNwlhisZyyaXCSjEnEqYIzuIdZh = 28477900061922625; 
string DWHa = "bSjMLbOzCVzZYLtNhhb"; 
byte PKIhjBfBMHKXbyCz =  88; 
short wbgSUP = -6913; 
int puOGQgliDECCIYQnPcWnRo = 882708321; 
if(puOGQgliDECCIYQnPcWnRo == 448977){
puOGQgliDECCIYQnPcWnRo = puOGQgliDECCIYQnPcWnRo + 657528;
}float tPTujqTehVktWONFNdqFfNz = -4.17731E+28F; 
string ULbJFqQjNJnOzwLUbc = "tbgpctqmHHqABiN"; 
double flHbfEaWzpSjw = 1.057997E-05; 
if(flHbfEaWzpSjw == -3.705597E+11){
flHbfEaWzpSjw = Math.Ceiling(Math.Cos(2));
Console.WriteLine(flHbfEaWzpSjw.ToString());
}int LJGWakcPdHGDNwMQCfIetEUxLQdnI = 994704; 
while(LJGWakcPdHGDNwMQCfIetEUxLQdnI == 994704){
LJGWakcPdHGDNwMQCfIetEUxLQdnI = 945142;
}byte ColAROMMNupCAFY =  111; 
uint gPilkgPhCHiXwzBtZRx = 544747; 
ulong KYwONhX = 43421750578153164; 
sbyte KKlfE =  75; 
double agFhaVepHznPtbZFGiTLIedKM = -1.745323E+30; 
agFhaVepHznPtbZFGiTLIedKM = Math.Ceiling(Math.Cos(2));
for( ; ;) {
Console.WriteLine(-8.009356E+16);
}byte XVmtMxNlljcHPm =  254; 
string cOcITmNzZOpTDKteRKBsTxC = "zZXIkjjyltsaiPeJBVNIytKA"; 
long XDzMVHmmjToemQETI = 66990102971814099; 
double TqeoyYIlHYAzLNVFktwIW = -1.414033E+07; 
TqeoyYIlHYAzLNVFktwIW = Math.Truncate(TqeoyYIlHYAzLNVFktwIW);
int? pKqGD = 7927122;
pKqGD += 53195;uint nmDChTnbXOWinuIbQX = 94212678; 
ushort hjiWENPPSaPAeUgcJHxFBPkns = 52605; 
double VSMDhwf = -1.797649E-12; 
if(VSMDhwf == -3.245792){
VSMDhwf = Math.Truncate(VSMDhwf);
try{

}catch(Exception ex){

}
}int MyKjaFcslPQkO = 95; 
if(MyKjaFcslPQkO == 151582){
MyKjaFcslPQkO = MyKjaFcslPQkO + 887559;
}byte cMOyT =  24; 
byte yJnekEyYXxaWPScZtLWCcm =  212; 

}public void xoJLWMgBQHOfD(){
sbyte ABIPLTnpkKOeiiLJkR =  97; 
long UMclpBfidWylhxauyfBiYzbhIXq = 15010647399352899; 
ulong EwshhgNqTbMKnNLtAXx = 87456327375930604; 
long NARlPULhaRqnRPVGxJREoZL = 5900439413025208; 
int DfCUyA = 12474879; 
while(DfCUyA == 12474879){
DfCUyA = 84389;
}ushort PxEE = 36760; 
long zOPIi = 60536570321625887; 
short tMZiYDdBFeIJjGGEsXcds = 16527; 
byte xXtmGktnOg =  14; 
float uKCwmjDBuKbsoDNgkp = 0.063676F; 
long ajRVuMuUdDOZYnBJfDJObnq = 51052514755656071; 
short HJqEqQRgBHzFoEAGCkdQWMNTWF = 14716; 
sbyte pmgxfqFje =  68; 
ulong IBHSpaegDcFtCXVVzHMeMUjts = 23853835554174011; 
double kgaHFIYKnJ = 3.401863E+29; 
if(kgaHFIYKnJ == -1.180932E+13){
kgaHFIYKnJ = Math.Pow(double.MinValue, double.MaxValue);
try{
int EnbRkKZwJSCUSBAOPMEJBM = 8725805;
if(EnbRkKZwJSCUSBAOPMEJBM == 78594){
EnbRkKZwJSCUSBAOPMEJBM++;
}else{
EnbRkKZwJSCUSBAOPMEJBM--;
Console.Write(EnbRkKZwJSCUSBAOPMEJBM.ToString());
}
}catch(Exception ex){

}
}string xHN = "LgRWWBuIcHmkS"; 
double gqwCByBYApFGYPKgc = 7.300255E-12; 
while(gqwCByBYApFGYPKgc == -2.323286E+28){
gqwCByBYApFGYPKgc = Math.Sqrt(4);
try{
int TsAkyAtjmtdFaTk = 8928598;
if(TsAkyAtjmtdFaTk == 99805){
TsAkyAtjmtdFaTk++;
}else{
TsAkyAtjmtdFaTk--;
Console.Write(TsAkyAtjmtdFaTk.ToString());
}
}catch(Exception ex){

}
}sbyte BxujCqacfLXyXUFDcIqXBkcZgaKQ =  -81; 
long ssmWcYaLccCVinFucWhLmJiypuudC = 50834597243241823; 
double EbWnUYpUMDGHUbXMJ = -1.950938E+23; 
while(EbWnUYpUMDGHUbXMJ == 2968557){
EbWnUYpUMDGHUbXMJ = Math.Ceiling(Math.Cos(2));
try{
int fOTboRyddWCzxowoUqHiWumuESZwM = 6028256;
if(fOTboRyddWCzxowoUqHiWumuESZwM == 63480){
fOTboRyddWCzxowoUqHiWumuESZwM++;
}else{
fOTboRyddWCzxowoUqHiWumuESZwM--;
}
}catch(Exception ex){

}
}string KJcLBAzyRcqBgOWRNT = "GhzgqWmVMyQQfCldUDSbbQL"; 
short mYT = 13460; 
float yMTxwkDLPmEMabmlZU = 4.577781E-36F; 
byte SMPyhgeYukRUsdleRTqScPzeLoN =  6; 
double aToDUgHYdwHRTsUgKiZUcRLxScmLM = 8.162664E-11; 
if(aToDUgHYdwHRTsUgKiZUcRLxScmLM == 1.409489E-08){
aToDUgHYdwHRTsUgKiZUcRLxScmLM = Math.Ceiling(Math.Atan(-5));
for( ; ;) {
Console.WriteLine(-2.62366E-35);
}
}ulong tJeKMu = 54186061800812956; 
float HITEAVDlKindCiTjMTxbaIBh = 64606.63F; 
int wyRgl = 992214655; 
if(wyRgl == 177730){
wyRgl = wyRgl + 652559;
}uint SaEgby = 67; 
ulong nLPOYgMbVq = 2790707294140849; 
short DkYViIWaBgAxG = -21108; 
int yhbtdhNgJaOlai = 942470337; 
while(yhbtdhNgJaOlai == 942470337){
yhbtdhNgJaOlai = 57613;
}ulong GpkMBOSakcPLuyhZqwzRmhTLaq = 84784917575758941; 
int isHYoJPMyoPuQqcpFVGzyA = 18614410; 
while(isHYoJPMyoPuQqcpFVGzyA == 18614410){
isHYoJPMyoPuQqcpFVGzyA += 989199;
}int aPUxRZOBCF = 1478; 
while(aPUxRZOBCF == 1478){
aPUxRZOBCF = aPUxRZOBCF + 893315;
}
}
} public class PihVPYdmJHyzZOQghoZBtB{public void Ddtb(){
sbyte lZIFEfQytlQ =  81; 
sbyte sqbDRC =  -122; 
sbyte DutofGSxXXXxIP =  96; 
ushort ZJPciKjsMkbRkFngDekj = 53541; 
long CQTTkLLChgiDtpttLiWtuQFMtK = 50503102079869694; 
byte pQYIbJUKc =  10; 
double jMAbUqzMkhACwGRzUqnDdfdUj = -3.148341E-12; 
double UYjkXWsCchqylwSwQ = Math.Log(1000, 10);
jMAbUqzMkhACwGRzUqnDdfdUj = UYjkXWsCchqylwSwQ;
try{

}catch(Exception ex){

}long KYgfsygkaNa = 55897813835057889; 
short UEJpatHP = -11890; 
byte dMmUuYFfnBRDFo =  202; 
float yChDqgQwyQsdBxKBFaRPZU = -2.753158E+33F; 
uint Jcaeb = 4700; 
double PfTNFyzWimbN = -1.308461E-27; 
if(PfTNFyzWimbN == 5.483226E-17){
double pkclzNwtOfFGeUwMmmq = Math.IEEERemainder(3, 4);
PfTNFyzWimbN = pkclzNwtOfFGeUwMmmq;

}uint VUSyocSmwpwBUMfHyw = 56017544; 
string EIpQLDE = "hlumGmSGnsnjNKRhuoGhOkFXDNqjh"; 
uint pJxhigHgTPSg = 2811; 
float TRixwXxkXIxKTV = 0.0003166433F; 
ulong YBp = 25611104855781481; 
string JMqztZbWtm = "kCKWnXgu"; 
short ppfqigKmkd = -13509; 
float NYCyozOeGmeqg = -4.028115E+23F; 
float lCNZtwkgjSsJhjWqhWYMARVD = -3.540243E+14F; 
string ZcQ = "sxRNfuuEqpfHxwzdiaZuShowHk"; 
ulong sjhhczpMsikNDuwiMofOl = 44016988632341964; 
ushort UELlgi = 62770; 
int fMLZPlSzNfKsfcyssieIygFWcqjR = 27; 
while(fMLZPlSzNfKsfcyssieIygFWcqjR == 27){
fMLZPlSzNfKsfcyssieIygFWcqjR = fMLZPlSzNfKsfcyssieIygFWcqjR + 150238;
}long oSjRDDybMOPCnUPZ = 52939949021482263; 
string pAe = "IYtYdAZneUJDjnCIEnAbUOIKqBV"; 
long CmXWQgRhpRpaFSGpFFJWuzb = 47914281750434957; 
int AEILHugLBn = 183345; 
while(AEILHugLBn == 183345){
AEILHugLBn += 39548;
}ushort WlPudkXSPnByJnLNenXFYjD = 14074; 
float FQcLWnpU = 1.986609E-12F; 
uint BxOOFT = 542; 
int znkJePO = 68207539; 
if(znkJePO == 863939){
znkJePO = 617009;
}sbyte dqlhPHtnYNCRjFKaRGwsOBnj =  2; 

}public void VJUGI(){
float YCCYVPDeZmVDYEgaEJm = -1.129235E+22F; 
string ccHKLKusfLkldhcVRT = "PmtHCCPJp"; 
double GRXXAWofRwGUQ = 8.348769E+20; 
while(GRXXAWofRwGUQ == -2.194506E+27){
GRXXAWofRwGUQ = Math.Ceiling(Math.Cosh(5));

}int YkkolNQdEyS = 44400214; 
if(YkkolNQdEyS == 480519){
YkkolNQdEyS += 654038;
}double atfPGudmZGILKUS = 1.085053E+15; 
if(atfPGudmZGILKUS == -2.935226E-20){
double AqQJXhjwaIGYSxFVh = -1.983029E+35;
atfPGudmZGILKUS = Math.Ceiling(AqQJXhjwaIGYSxFVh);
Console.ReadLine();
}short jZbJbgwKIiNfEnUiaDfzphf = 2787; 
byte BMXUhmZsJiRV =  181; 
string uADgUPnJOlFkSybTAxKHJ = "ObAfkFMqfRePVIEwtDldjIE"; 
string VHosAa = "aIMCHOEJXygNf"; 
string updcgwGDgyxYpYxYG = "KOnWbOGYDgNCQWdtmZnyUZwe"; 
int kPnL = 463867; 
if(kPnL == 427958){
kPnL = kPnL + 300153;
}byte npyiSAQFIAUWMxL =  42; 
double siqYEQLSFnaTBOfPLYaLh = 5.932545E-18; 
if(siqYEQLSFnaTBOfPLYaLh == -7.172434E+14){
double NsaMXeg = 1.477405E-12;
NsaMXeg = Math.Sqrt(3);
siqYEQLSFnaTBOfPLYaLh = NsaMXeg;
int? aUCFPZfQWLNikDxU = 3958860;
aUCFPZfQWLNikDxU += 23382;
}ushort hcMwHLPCct = 465; 
byte jBjQOSMehtaWWkoPoqBQXwui =  231; 
float fRPgBzc = 1.393578E-15F; 
float GtHiZJLzLBBNOcAuSOgbUTLHn = -7.654443E-21F; 
uint TGYYyLgiOxLyxjI = 8238785; 
string bekRi = "XNJaVHxlaRUCPpsskdswFzJc"; 
string qizuJPPi = "XziTPolxtNepLMJT"; 
sbyte NjzdHZLpnNHifMG =  77; 
ushort cjDgtcpoWUGxQBCpcYbURjiAbzP = 13106; 
long KVqORWWHRxOq = 34001899785989344; 
long OBcSbdeUmfIFjKecKFuw = 89919746559383561; 
uint ESJcgRLYVTCWiIKzHZMUw = 41; 
short iuEMKmUsDCPVhDnq = 14723; 
long UkxyQJLQyIoCxuDYx = 78545083765717746; 
float AxzEKqUuoMWNdZyyKn = -2.910081E-11F; 
uint FRiAAWmHQqUfktZeFk = 999167; 
byte AuikhjUsg =  139; 
string SIukSYhJWGTYPeyGKPQCqsqPpnO = "Nyks"; 
string BqLusESGpUC = "xdMnJqMB"; 
int qFUEjeVzuBVzEgUo = 247753; 
if(qFUEjeVzuBVzEgUo == 29634){
qFUEjeVzuBVzEgUo = qFUEjeVzuBVzEgUo + 431388;
}byte SLqzP =  227; 
int MaafAOtOKRBapXAGt = 8701; 
while(MaafAOtOKRBapXAGt == 8701){
MaafAOtOKRBapXAGt = MaafAOtOKRBapXAGt + 481801;
}
}public void qQMAOyKAUtNEfOQJTVcjWmPVkLe(){
ushort hKU = 34197; 
sbyte KcUZlhagXkWAFEjGXQoYeaRn =  -114; 
short zNJScZZgHZ = 27205; 
ulong VgRK = 3623946591908892; 
byte LJlGWTT =  138; 
ushort sUBGyDGbZm = 4055; 
double gZRTyIk = 4.030275E+21; 
gZRTyIk = Math.Ceiling(Math.Cos(2));
Console.Write(gZRTyIk.ToString());short UWNzEkPqcEugLJaWuCSpbYa = -8173; 
uint gSmWObRyifplIxZmewImEDKUsbf = 72; 
double uWKdWefQtonAUHhUxpPUGMoqFis = 8.023408E+17; 
uWKdWefQtonAUHhUxpPUGMoqFis = Math.Truncate(uWKdWefQtonAUHhUxpPUGMoqFis);
 bool? WbqIIszNMCdzzDmKLSWdmI = new bool?();
WbqIIszNMCdzzDmKLSWdmI = true;byte yofqkppXuIgznpOaRyjp =  44; 
double zxIRnLnBd = -264124.1; 
double XWNqeDiZQngg = 7.650446E-36;
zxIRnLnBd = Math.Ceiling(XWNqeDiZQngg);
object IVEzcl;
IVEzcl = 0.0008093626;float laz = -6.692417E-13F; 
sbyte nVauXygjyzHATmMQHoaS =  13; 
ulong ZMnUlBEBEzQOpfMfHExRwIVZOBj = 35009898495369450; 
long EUKTYdaczwLUitogVBqRn = 34434604857931285; 
byte edxdJNXyFUX =  74; 
string YJYXYIzKRuxhthoR = "smeGQMpGuXdNXqRIQNZnnMFS"; 
ushort cIBtQhNeG = 2639; 
double iWIqFKwWUdFBEupECGJmkDKJea = -1.199592E+32; 
if(iWIqFKwWUdFBEupECGJmkDKJea == 2.042763E+17){
iWIqFKwWUdFBEupECGJmkDKJea = Math.Sqrt(4);
Console.WriteLine(iWIqFKwWUdFBEupECGJmkDKJea.ToString());
}ulong tWuTxsUXyThTesWbgAH = 88946254613108402; 
int zCAFbUl = 72611954; 
while(zCAFbUl == 72611954){
zCAFbUl = zCAFbUl + 548882;
}sbyte QDwb =  -76; 
ushort WxxsCWeqggqPZsIodyLluq = 50890; 
byte KXzZUjsPNFchJtoKwbMOO =  40; 
long nsFAhqG = 16683337198390710; 
double ZlNFgwexCZoczWCmoMDGTIwZuXaCp = -2.686345E-33; 
ZlNFgwexCZoczWCmoMDGTIwZuXaCp = Math.Sqrt(4);
Console.Write(ZlNFgwexCZoczWCmoMDGTIwZuXaCp.ToString());ulong gAJxfjuXDXYhwRu = 62466176367517654; 
short MewXgqCMysM = -32628; 
ulong eqIdiRUJRuhLCgfz = 13434397495567289; 
ushort xxYzIcRjtosabBwdmbP = 27704; 
ulong GYpxIFgZtIIDu = 39970672313090175; 
float cnSLpS = 2.382326E-23F; 
double bQZikwlctWLszfXsUoKDyHPSm = -1.009938E+23; 
bQZikwlctWLszfXsUoKDyHPSm = Math.Ceiling(Math.Tanh(0.1));
int nAeodgwgacZzpnVBCm = 15; 
if(nAeodgwgacZzpnVBCm == 680403){
nAeodgwgacZzpnVBCm = 541533;
}
}public void seOszuOJc(){
ushort VHdgzqDpdI = 37156; 
float wKfRAVVQDcMbHOCJkPmSIGUja = 6.692955E-13F; 
ulong zFOkWWOoXjhBz = 38477511300574149; 
float pcjQQWQPEfxezYRKSOtnGfsimp = 1.848129E+33F; 
float PMGMzhNfXtguEubngtmgLmnG = -1.69783E-24F; 
long DBd = 36646247902603920; 
long MQaglDXhZiAJLeCyaAnyNDyomyPc = 37819666258405625; 
byte aINSEqEjlKPmzmk =  54; 
byte tLTPNUQ =  47; 
ulong eCffjCzHLUZfMckfneq = 37098413436777208; 
byte TcuCXfLEKdWPFBojmZQEHHMTJYM =  131; 
short HWl = 14281; 
float FFOXBCEbuqdSLFRFQJnlqCTZt = -2.701829E-19F; 
long aETJyGJkSdecKz = 78514908122812140; 
int GBdLPFpnWFtOjj = 9947; 
while(GBdLPFpnWFtOjj == 9947){
GBdLPFpnWFtOjj = 596399;
}sbyte HcCdBuCy =  -26; 
float xdWDXxnNQfffEMKCMcJiYnwUajMdW = 0.01497508F; 
byte ONgqykXJSsdCXXs =  210; 
float uMMaiIcCNLjQeIWHPYDknRMA = -7.637655E+30F; 
short dmtmeWeyNOFz = -13484; 
double mAcnxFphtnIDDal = 1.35622E-18; 
if(mAcnxFphtnIDDal == 8.568559E-05){
mAcnxFphtnIDDal = Math.Sqrt(4);
try{
int iGzYVCqgTgwf = 343009;
if(iGzYVCqgTgwf == 64949){
iGzYVCqgTgwf++;
}else{
iGzYVCqgTgwf--;
}
}catch(Exception ex){

}
}ushort aQLjqWodJNLPFFEbsSfaXWmyqPXOc = 9195; 
ushort puqiEFKef = 6699; 
ushort EdPZUMh = 12727; 
int cdDmSDZCaCuz = 207938; 
while(cdDmSDZCaCuz == 207938){
cdDmSDZCaCuz = 757793;
}sbyte VSD =  -106; 
float ZXTqZiLYqm = 1.358021E-14F; 
ulong eFeSPf = 49924047483450962; 
ushort CytFDbIK = 35912; 
byte BTaPgaaVqNKpOJwlwdFgiCg =  180; 
sbyte iFdWwoFsdLLJgucGfyRhGVhQmGXd =  21; 
double pHjSwSusuLwFKBMXkeDwKT = -2.187913E-13; 
if(pHjSwSusuLwFKBMXkeDwKT == -1.045445E-20){
pHjSwSusuLwFKBMXkeDwKT = Math.Ceiling(Math.Acos(0));
 bool? bEcyGTGXGKDckHCLfKaKyPKFxFgWZ = new bool?();
bEcyGTGXGKDckHCLfKaKyPKFxFgWZ = true;
}ulong XICxxKUaOhnXONGJbbtDg = 37192797546983130; 
ushort wqmMBAfYDKOYbEKoPHkxZJjDMTiX = 56195; 
long wqlTHBbeiVdRT = 84527792784783766; 

}public void aXMgLaIpifRnt(){
long LdOOCcBVSqjzguiECDZpngAQ = 13401567971384453; 
int eKCwqtAbUQCDBWZCpMtsCFFYNdjO = 853633069; 
if(eKCwqtAbUQCDBWZCpMtsCFFYNdjO == 854614){
eKCwqtAbUQCDBWZCpMtsCFFYNdjO += 197684;
}short cflHQllJozXWOxJS = 6720; 
byte BfDImiMecdpKp =  157; 
uint kBNCosWKogmZCiUG = 796556; 
ulong AyYydhiGYOHpn = 38615542536531344; 
float fWNDTjLeBQfa = 129.8754F; 
short gipLBAXLFLCNTKI = -19362; 
int TCqGp = 251081; 
if(TCqGp == 500336){
TCqGp += 152356;
}double QEmYYShSGYbYiIYlRhVtJJ = 8.313801E-28; 
while(QEmYYShSGYbYiIYlRhVtJJ == 3.732881E+30){
QEmYYShSGYbYiIYlRhVtJJ = Math.Ceiling(Math.Acos(0));
Console.ReadLine();
}byte VMgjueeqeWEtCoFuPkaaQpXRAS =  49; 
float IGMGmCtRAHhxhlmtxdQTj = -1.295583E-21F; 
ushort cFOpKCUk = 43123; 
ushort SNnOkDoFSEJ = 5936; 
long dOuaACtpxjWGCPe = 18367592869645337; 
string HRHEGuBdRHNBkiVXCqeS = "bABEsipPWuZssSLMMQQ"; 
ushort KiqOapSYZqLjcFqga = 16362; 
byte nVlOhTpk =  55; 
ulong owcsqiPsSyRXdnhon = 38942130565890920; 
double GhZINjQxOamaYkVsMy = 4.712879E-26; 
while(GhZINjQxOamaYkVsMy == -4120568){
GhZINjQxOamaYkVsMy = Math.Ceiling(Math.Atan(-5));
Console.WriteLine(GhZINjQxOamaYkVsMy.ToString());
}double ICEIVZXIxgOcpOnAXMFyHT = -9.913448E+34; 
if(ICEIVZXIxgOcpOnAXMFyHT == 2.331209E-11){
ICEIVZXIxgOcpOnAXMFyHT = Math.Ceiling(Math.Sinh(-5));
try{
int MJKdsNeBeOe = 1009118;
if(MJKdsNeBeOe == 99498){
MJKdsNeBeOe++;
}else{
MJKdsNeBeOe--;
}
}catch(Exception ex){

}
}sbyte zAkAQOVjNSJEoIyFGaAwT =  -59; 
string GyzZujBuSXoGJwc = "xHJ"; 
byte EttIPoeBN =  209; 
byte NUtoVPEsnMssRVk =  241; 
uint lGSlbdfsFUwUegegWBKLBSfRXSabQ = 77773531; 
sbyte OFG =  32; 
uint ojEkKXuHkmKZLmOzuBanjddCL = 246792004; 
float CABuJuBXHxT = 8.650774E-23F; 
ulong fUtUnokfGnJlfbATLDoKOjG = 22642606649635725; 
double pxQxsFbpuU = -1.28616E+38; 
if(pxQxsFbpuU == -6.122372E+31){
pxQxsFbpuU = Math.Floor(-5941.532);

}int oqiiSABRJQAeiqbuaBaTVNAm = 1602; 
while(oqiiSABRJQAeiqbuaBaTVNAm == 1602){
oqiiSABRJQAeiqbuaBaTVNAm += 380467;
}ushort TLQZueZTWNHufmoOOIIlG = 50063; 
float RWVKdqhouRkyCcaVB = -1.500179E+34F; 
sbyte fhbTHu =  -119; 

}
} public class jdfQmhzjpPFGsZobRmMHML{public void QNyMPTRxXfDkNeH(){
short DMkstCOgSZbuJwZAFocQxJpAsHB = 19289; 
ulong XkYReaAgylJFWUPco = 51955250728050618; 
float MZMaYsqcRxRpZZ = -2.744626E+25F; 
float RMxKps = 2.105903E-21F; 
string hHAskFOhMkLm = "RXHyuFMLXNqHgZbJtkM"; 
short UjWAeulpfGxEhFNKVxIqIjTiLeFNz = -9870; 
string ZHWNLYojBwRgVdZuoZzPeXqLqNOaP = "ROGyFBIZYXgzRmt"; 
ulong IekzAUkUb = 71177959573995405; 
int Czzz = 30216146; 
while(Czzz == 30216146){
Czzz = 626679;
}sbyte sFBfW =  23; 
string UuEDlxDbMTdP = "okPiGyhzsEmVkUSxesPal"; 
sbyte VWmqNY =  72; 
ulong qwctMXpgSTCRpHaCK = 29907970347580305; 
float tgwAEPLFP = 0.04349759F; 
long bIisIpG = 37157516131154242; 
int WoSKXZytYbFckKkUABHGdAOnYgsqg = 532283; 
while(WoSKXZytYbFckKkUABHGdAOnYgsqg == 532283){
WoSKXZytYbFckKkUABHGdAOnYgsqg += 927800;
}byte ydJLZJoalbfSAHNIXUxCEMRIgm =  135; 
uint buNbbcx = 896579696; 
sbyte AnMemzHPOOdWAIoMmaFMhPSj =  34; 
short yFqKdQHcpaMwIHMMuYzntNpjX = -12542; 
ushort LwDyUIikyeagSGYMBGXf = 27017; 
string kiezLngNNxZsUzn = "ZyCnpYswNPFKqtnm"; 
long qoQizWpqmKTndnleXITPhlRJLWJy = 65721601269175246; 
long ZGYYMIDzMqylwGtSQeygyCmSV = 51402772614835440; 
sbyte nRjBigmksajYDtRgcCEty =  -36; 
sbyte TgwU =  24; 
byte SoXebHEo =  3; 
byte USMUZNMZeLR =  192; 
byte LfIcLsGhPsJGoQBzmDhfznsYgJh =  154; 
sbyte bDLjqBWlpTMNctlt =  -8; 
double GynGfyyOJVcjaao = -1.77983E-29; 
GynGfyyOJVcjaao = Math.Pow(double.MinValue, double.MaxValue);
int[] MTMnqnJLMOceAtzzdyap = { 4229641 , 91381 } ;
Random NmQVhSYTBlEjntFonKZHJY= new Random();
Console.WriteLine(MTMnqnJLMOceAtzzdyap[NmQVhSYTBlEjntFonKZHJY.Next(0,2)]);long QNJxGNHVZH = 55455425499156227; 
uint laTiQNbJbORUmUclkdGRDttYtVB = 941785; 
float TxNcQTAHSLXTkQafotyIiX = 1.14922E-12F; 
float VPUlyMkcXzARlsLxNfFB = -2.214115E-27F; 

}public void QlkAjEWOwjiX(){
long HnlJiSDGygfXMjDNsAOxdal = 74296998921948403; 
ushort JgS = 24849; 
float pQkOetmRNlJSJjmKGcYIKRSCQcLWd = -1.988781E+15F; 
ushort AzHTlgTOHozwPujN = 64268; 
uint wjozYJXbJpOKCosjMHRuVeUc = 378967; 
sbyte IBj =  -107; 
uint XqkZJjnekzTGXfxkzfhgUpolWFD = 378504116; 
sbyte KgHsbY =  62; 
double fQUxjWxaGycdWwZHbYcd = 2.043623E-12; 
double wlLhBbexTbZhCqWfcNMHLZaK = -0.002520598;
fQUxjWxaGycdWwZHbYcd = Math.Ceiling(wlLhBbexTbZhCqWfcNMHLZaK);
object Reazglj;
Reazglj = 9.150168E-39;
Console.WriteLine(Reazglj.ToString().ToLower());ulong oxMJdwlbnBnaFPbCbogLPgqsMBSd = 12075550956202125; 
ulong iOZoIWfVGsVCVQV = 20935472846635371; 
sbyte pGDLKZxPEgsMA =  -6; 
ushort RUAhfceQl = 49453; 
long qAGjRVspDjVtAMqJ = 35735907369100703; 
string nmB = "AXqWzdBiUFgeJHZgJuLxnT"; 
sbyte QitXfQHfQQLYbDnmitydCFAAJYqUC =  -107; 
uint SqfoLEcdXboTWbeffzm = 420795848; 
string lJTnhAMEMsegKyY = "FqtfBpHEITlPdREjQ"; 
float GRpVFlyDBRVwJhVJEcVXVcpGnnX = 1.123637E+28F; 
ulong pQDsMowOUdNfMNyDIhiZhUWYEBK = 13554982679530829; 
int Pjtinb = 154610431; 
if(Pjtinb == 862231){
Pjtinb = 815978;
}long sMQNwUEPkGO = 4543972540258908; 
ushort YPQTTcRHXRRGYGWVjiLDaZOwR = 27266; 
ulong CPKX = 37993806254968595; 
ulong mPXRHBi = 14435239576250051; 
ushort ynO = 5912; 
sbyte TqCkZW =  -47; 
long PSMZzaPmUcjLQuGwWESeDZdeId = 26741767805638911; 
sbyte mbX =  -124; 
long TbY = 54430597841024584; 
int QOHquCJYLBV = 5441; 
while(QOHquCJYLBV == 5441){
QOHquCJYLBV = 896164;
}int wCfbgmaCjAgjPPWolGKDAaciC = 4; 
if(wCfbgmaCjAgjPPWolGKDAaciC == 302846){
wCfbgmaCjAgjPPWolGKDAaciC = 639817;
}sbyte IhPKMBeuZzzEqgFGatDjds =  -25; 
uint BdkAUUSYyhHhFAeGiACFFjWyVHWe = 669271795; 
short pNkyTUfEHGUYXyxUzFUTUUt = -11568; 

}public void NEmYOa(){
ulong USSnIMouXYYpXBykcGtOxTwtUo = 15346247485057682; 
string EGQSGAWWwajmgcWhHaw = "xAu"; 
short YCsftkSyRHxQCiXWAYU = -25440; 
float PJKYfKbj = -6.853672E-22F; 
uint IXFQbQsPo = 496266; 
short RzfXBA = -16789; 
ulong MJwwfhxICksoBasJWeOeVui = 42674677195574737; 
byte TkVwGKktHUoZRqyN =  73; 
int zITsAdDSVBwXAg = 64257492; 
if(zITsAdDSVBwXAg == 258479){
zITsAdDSVBwXAg = 320413;
}ushort MnBUsbWPiXapMTaTws = 3892; 
float SkWPScKJpbm = -7.683781E+31F; 
double SGLCVgszXjwVHWbVnDn = 1.442676E-34; 
while(SGLCVgszXjwVHWbVnDn == -0.4052703){
SGLCVgszXjwVHWbVnDn = Math.Pow(5, 2);
object EIaADkUFYl;
EIaADkUFYl = 5.283124E-19;
}long hQReRmTEJlJbiMxHXNyewesiVT = 24436587273275691; 
long DzYAWGkIdnmmS = 20216023807680602; 
long YADIqeS = 1132577194726430; 
sbyte TLfIGCztoKBhMGXVoaatYUDl =  125; 
byte RzFJtyIk =  191; 
int sYenbwqgnyeijOXPS = 153823761; 
if(sYenbwqgnyeijOXPS == 368487){
sYenbwqgnyeijOXPS = 332463;
}long LbgKdujsOJcoKThUTYNMpuUxIPl = 18644044556038631; 
ushort xkcUQweV = 33351; 
sbyte aZPuFV =  20; 
short LcniFJ = 26163; 
long SpCOKxOLRAZQBg = 65195808945612615; 
ulong mAu = 22458465902250501; 
float lotHTdeqqjVHdaZRft = -45.59714F; 
uint kbSRCszubzQCtwLXi = 704661; 
long oAoQoiKpfqpcKeqPcDMTXixQDHF = 74758305420846651; 
int bljOdWhBzJptigVYQ = 123739698; 
while(bljOdWhBzJptigVYQ == 123739698){
bljOdWhBzJptigVYQ = 661718;
}int hVzAkdhPlapDfTmope = 261644321; 
if(hVzAkdhPlapDfTmope == 66309){
hVzAkdhPlapDfTmope = hVzAkdhPlapDfTmope + 972535;
}float HQA = -9.977746E+20F; 
int jWaJbmFiFWfFxLfUOii = 31725025; 
if(jWaJbmFiFWfFxLfUOii == 850237){
jWaJbmFiFWfFxLfUOii += 491358;
}int ysohlMWV = 238078142; 
if(ysohlMWV == 827839){
ysohlMWV += 367615;
}sbyte dXtxpCV =  56; 
float TosOhjudhQoOUTAPiIh = 1.46626E-12F; 
short JeVfkTVx = 4644; 

}public void HSHEiexTO(){
byte TodBWBsnEZ =  64; 
uint KJmdj = 24; 
ushort CmaTlZDMbFfWPQRFnsDiam = 24970; 
short UBRsJEYViOQBdotLRY = 11161; 
ushort UTwzWR = 44589; 
byte nTKpsAMtsagaHbbekdjyio =  181; 
float ZIxTNMoLyAXROW = 3.25826E+37F; 
ushort IognRuuLNAODZhQUtRp = 32948; 
int SpjDdmyhpy = 67024052; 
while(SpjDdmyhpy == 67024052){
SpjDdmyhpy = 728870;
}float AcZQ = 3.534098E+31F; 
uint bTEcKXauyQeopgnGjpRpfjGH = 26; 
string obtnbnlEFnOOIJhzIC = "sOtHHejKRmWbxWyewyQmbU"; 
long VDc = 47972279675415084; 
ulong wNjRFwpIlqRjFNbRoKyesLdbCnP = 23330220880486670; 
long wyAttJklzSoghjSkKDK = 22443232168077885; 
ushort CwMqkTlYLZbSPdxAZP = 22394; 
string YGpa = "fgTiIYOIWxDPWqG"; 
byte EJRtlUgkVswCDPxqNOffDscehgZWk =  219; 
string PZcHifUlMXMHsg = "HgyjEBhxlGNke"; 
float acsZMDMP = -9.147344E+10F; 
float tFGxMaNsQOJyVHDRbxUPqCXKnt = -1.587229E+25F; 
float nDKHPSGjaksC = -1.188866E-20F; 
ushort deaUONNeCRIWhsGVyzAgOnSLkRw = 44643; 
ushort hKpuopRaECPFfXjKbJFyeVOm = 8393; 
ulong oJWqdJMKX = 36583513671835249; 
short iVCCQNe = 31361; 
long WdmBdCqHk = 68796773278599581; 
short HaQUXHqOhgFZK = -32347; 
uint DBD = 5353; 
sbyte NHmPJoyCgQ =  26; 
sbyte gipTMOiB =  27; 
long tOkCOcbRA = 67263099254162888; 
int ieVmUtkJBOYOgVb = 885076; 
while(ieVmUtkJBOYOgVb == 885076){
ieVmUtkJBOYOgVb = ieVmUtkJBOYOgVb + 85549;
}ulong GzcVGbdpyVhDhzBqhDtCdCZ = 21178343252173524; 
ushort ppPRzFIxzBSZsT = 61290; 

}public void NNkLmKeAgDWF(){
short IePgRiMBKYMeGagXZPBwJcGIiHk = -12812; 
long JdgETOjlcOHldmZhdesjEgdUlFz = 78362712511901484; 
long cgtNONNOxZsJbEgXBsCZmqoocmSQk = 5343668759657206; 
ushort uqfQFgyXHRzoxOJLMReKBUWE = 64621; 
long sSDiYeDhJgBLh = 20151774340452923; 
long ngUMoAwDRTMeeeJ = 33614044453003706; 
double kJXRR = 3.838405E-23; 
kJXRR = Math.Ceiling(Math.Acos(0));
for( ; ;) {
Console.WriteLine(7.430077E+34);
}int TzMzcepShffTsjEHpgyiXhUzEzA = 91; 
if(TzMzcepShffTsjEHpgyiXhUzEzA == 589036){
TzMzcepShffTsjEHpgyiXhUzEzA = TzMzcepShffTsjEHpgyiXhUzEzA + 46041;
}sbyte NnHJfBXLELbOpOdAYTxX =  59; 
int UEFgWupBRj = 821967; 
while(UEFgWupBRj == 821967){
UEFgWupBRj = UEFgWupBRj + 342171;
}long xpN = 78970356565853131; 
int UiaNeBta = 2612; 
while(UiaNeBta == 2612){
UiaNeBta += 743301;
}double RBCOWonBsSxLefBQkYJIqW = -2.003425E+25; 
if(RBCOWonBsSxLefBQkYJIqW == -3.118137E-30){
double xAgyojRo = -29449.56;
RBCOWonBsSxLefBQkYJIqW = Math.Ceiling(xAgyojRo);
Console.ReadLine();
}short sDskMDh = -958; 
short wICtCeyYDDOGV = -32104; 
uint saWUSoWPJiHalgdgb = 6454; 
sbyte hGJhfsNuNHs =  50; 
byte gDyCgMML =  91; 
uint NwzHEGtulkunMM = 181358616; 
ushort CopbWWFFOBuEymkLtWnNjbZpYZM = 18009; 
ulong kDqQRigLACcZRSZINhdatZVI = 57101541396814992; 
float oSmHxbFIyNLkkgqplFkLxlHJepTgI = 3.540739E-14F; 
ulong SEgufL = 88720085005620032; 
sbyte NLGfgDckFDfiyaVwYdsD =  -79; 
double TjoMLz = 55965.13; 
if(TjoMLz == 0.002894541){
double apBVQDGSZKGNZyXHzRGM = Math.IEEERemainder(3, 4);
TjoMLz = apBVQDGSZKGNZyXHzRGM;
try{
int NTZeAYgeJsdcb = 2623;
if(NTZeAYgeJsdcb == 6312){
NTZeAYgeJsdcb++;
}else{
NTZeAYgeJsdcb--;
}
}catch(Exception ex){

}
}byte LhAiiWEBtaECeItUFjLcyFDPmbhTb =  109; 
double TJHV = 1.336658E+27; 
if(TJHV == -2.7183E+37){
double EHeNizQsiCgGKWhP = 7.27707E+35;
TJHV = Math.Floor(EHeNizQsiCgGKWhP);
try{
int BIway = 9593576;
if(BIway == 16225){
BIway++;
}else{
BIway--;
Console.Write(BIway.ToString());
}
}catch(Exception ex){

}
}long gApTXYuopbmYWldpRNMRfS = 59703295683358991; 
double PWnCgzPfhOtEZsNiZQckxUFoYX = -1.286865E+13; 
if(PWnCgzPfhOtEZsNiZQckxUFoYX == 2.713267E+15){
PWnCgzPfhOtEZsNiZQckxUFoYX = Math.Ceiling(Math.Acos(0));
try{

}catch(Exception ex){

}
}int jHUWioWoIYZYuTM = 27; 
if(jHUWioWoIYZYuTM == 753451){
jHUWioWoIYZYuTM = 861782;
}ulong phUBIKdiCGBV = 56643155128159116; 
byte InudbZZFRFxnFPo =  201; 
short EPytUYzFhqkdYIfSAGf = 5286; 
long jXOWz = 78581349469619701; 
ushort nux = 38813; 

}
} public class SItNHPhMoJJLXA{public void AGXAAFPfPKKfJSkkKZk(){
byte FbpKRndCNJscSBJxW =  25; 
float CFcW = 0.08936294F; 
float zYMMl = -1.735657E+31F; 
int QJpMFbaqcRqCPNUtjZZY = 25; 
while(QJpMFbaqcRqCPNUtjZZY == 25){
QJpMFbaqcRqCPNUtjZZY = 73552;
}ulong HQOGeOgxDqXDbGGJDCJlRscdVGoQ = 34767703902525441; 
string yaHyRScyWNuJSyuDgnNKKz = "qeHFypYUtGQeJFt"; 
uint bzKDxJco = 68; 
double AtbYCpsjmFgXSa = -5.206371E-36; 
while(AtbYCpsjmFgXSa == 2.925987E+35){
AtbYCpsjmFgXSa = Math.Ceiling(Math.Sinh(-5));
int? FaWuMjLWLPiFwWF = 9536454;
FaWuMjLWLPiFwWF += 55871;
}float NaiQQ = -4.898396E-17F; 
ulong LRcyqqAVgqVdbEsHeIiomna = 23063923538721048; 
byte PIfSu =  106; 
int eRlY = 72; 
while(eRlY == 72){
eRlY = eRlY + 728578;
}int zByQmQ = 71762550; 
while(zByQmQ == 71762550){
zByQmQ = zByQmQ + 357439;
}long lVmOKlClixPRHUimUBsdFnkoFELVH = 70644843547444727; 
ushort uFm = 23150; 
string xjR = "jXtaIuXLXjgCGGUAW"; 
int uIJzAXzsWkniuUtyqET = 622257; 
if(uIJzAXzsWkniuUtyqET == 621953){
uIJzAXzsWkniuUtyqET = uIJzAXzsWkniuUtyqET + 225128;
}float ZHyBfRFxUBzUcjbbAKbNIliEWd = -1.062077E+37F; 
float clEeAMzKiqPzAWSKQwFBpXjil = 0.0004797397F; 
float cRGEbzlpXKpDQBLXiN = -3.018788E+17F; 
byte FusCfcwTN =  69; 
int gAjUy = 604998; 
while(gAjUy == 604998){
gAjUy = gAjUy + 274888;
}double xAkcZlhsPmklBsjU = -1.727941E+34; 
if(xAkcZlhsPmklBsjU == 8.477989E+15){
double oOgTuTgubJDk = Math.Log(1000, 10);
xAkcZlhsPmklBsjU = oOgTuTgubJDk;

}long kZCXcYOGs = 61965396438607362; 
long NdcFwnWTVR = 20975041603008162; 
float pKhsPpeOnjxnbtXjhugKfeTPUlXGf = 2.511309E+09F; 
double IYdWKQeihGLwj = -1.50314E-09; 
IYdWKQeihGLwj = Math.Pow(5, 2);
object OIlONmEm;
OIlONmEm = 1.472723E+20;
Console.WriteLine(OIlONmEm.ToString().ToLower());short HVMJYNNPLU = 6658; 
ulong JGIHLkspQzFaywQEQMiBsFSaE = 5505519757528794; 
sbyte uqqsMWYQyhJCsXlUhxnz =  -110; 
ulong cgMTtQToHtOxmAuxFuOw = 46933007429548932; 
float HEHLn = -5.322501E-38F; 
string aFSbiEBYwNZKeosOhdE = "ILwxDNkPDkiw"; 
uint ORaUVFbMLKTuCnnWghyLXoi = 486062; 
ushort MxjFEiqBCNmi = 64390; 

}public void VZMEhkWCGeb(){
double QzJFwAkopBfolkNMjTZzucpxS = -1.7199E-23; 
while(QzJFwAkopBfolkNMjTZzucpxS == -1.18514E+25){
QzJFwAkopBfolkNMjTZzucpxS = Math.Ceiling(Math.Atan(-5));
object kbBxOilNdnPfzwD;
kbBxOilNdnPfzwD = -8.177372E-25;
Console.WriteLine(kbBxOilNdnPfzwD.ToString().ToLower());
}float wPwZVgedGapzWujLQdqs = -9.943907E-24F; 
long CLdA = 79368933611971473; 
float yoMNWwUOzaLRoRwBljhqyk = -7173214F; 
string asIL = "XQeGcGJnliNlcOjojDpYxdE"; 
short lhJUwDgOl = 17017; 
string ksVEVViK = "aQUNQPqMsUlQsRzZuRHHzQaidYkww"; 
uint wFnkqPg = 765604; 
ulong UTPu = 66755583152437652; 
uint PMKSFVqR = 83703537; 
byte Ghjmm =  218; 
float WguJfJZQYdSCNa = 2.301315E-34F; 
ushort jULchXiOWownSKmeoIqaMW = 60665; 
uint JYKqGGfQBJWLTZyBsNLwLkioJL = 737071341; 
ushort nYjjAYzxpKoysaptwJUBuilfhyQwJ = 58863; 
long tBwXCJAzbVxbf = 74063392443213471; 
double eNdcchdNwXVeRFsSNiw = -0.0008850046; 
eNdcchdNwXVeRFsSNiw = Math.Ceiling(Math.Tan(1));
byte SCfozcypqVQHwnuVxPw =  196; 
double HgnzZGNok = 0.01182425; 
while(HgnzZGNok == 2.378439E-23){
HgnzZGNok = Math.Ceiling(Math.Acos(0));
Console.WriteLine(HgnzZGNok.ToString());
}float bieaYV = -1.461602E+29F; 
string uiLhyIQdGehDlOalWFHFH = "MGsChtRaDTdjUKx"; 
byte AYgKQZAWqaMZym =  117; 
string xeQWBxmGCnAcTpQKjgGBPWGSFRt = "bfGGh"; 
double qxAYDUdnMZ = 0.0003895585; 
qxAYDUdnMZ = Math.Exp(2);
for( ; ;) {
Console.WriteLine(-1.049731E-33);
}ushort GeIouZSFhgpbnHQDSFUmRPNKQ = 864; 
long nCYJzWluAaeNHmXZKDuQka = 66030071544115356; 
uint nfpGiIDsuwdSA = 61; 
int NZnHVCmxOHqWxcBBMzBWqRcZc = 797808; 
if(NZnHVCmxOHqWxcBBMzBWqRcZc == 703741){
NZnHVCmxOHqWxcBBMzBWqRcZc += 878030;
}uint YFZlNtFciualwY = 5519; 
short JfIBWXnEAMpidNPAyzCoYfV = 24771; 
sbyte cNHxywXWjwPyIc =  114; 
sbyte SNaqmyWCTbbQEjOtAUBXdoVlgE =  -33; 
long RAsPmcggfQpLVEtVgHkaehhxlQESP = 35324140887804340; 
string hHluejmhLCp = "WhK"; 
float qVExAnsVfmAtBRlgpdtTN = -2.404733E-20F; 

}public void ISqzjNVCHukOxUItTzYXNPkKeTHep(){
string nJFPHXTMtsXNgGyKwwaZodolTIH = "pgRlEzWfSebwa"; 
ulong FQbdeIilJocHjtsWYqA = 81561161210456668; 
float uGZDzLxRFDM = -3.733486E+30F; 
sbyte CSNNAicdBptdeSwwpqVnJddMJWeu =  74; 
ulong stPIQWFthq = 9119427149119153; 
sbyte QZUmyOadDdJnNa =  -122; 
long kFzMYNWgFS = 53149474723346005; 
short CjcVBQeJnLkdVcO = 5598; 
long SmXgTWucXXyHxCJlNaDDl = 44092936574853808; 
int HZHIfCwqgCzS = 21; 
if(HZHIfCwqgCzS == 684558){
HZHIfCwqgCzS += 401695;
}long OcfzKEKnZo = 65675157172993078; 
int sKxbNpCckEXhxmEFTiYCR = 32; 
if(sKxbNpCckEXhxmEFTiYCR == 548504){
sKxbNpCckEXhxmEFTiYCR = sKxbNpCckEXhxmEFTiYCR + 438788;
}uint aBQRZWdGeLtbI = 41; 
short zhcmNHVbTktFqkTbJkdhp = -2984; 
short YPyFBjeVzbdnzfmT = 24993; 
ulong MLaOBB = 81625411235477878; 
uint YiqBJgygpg = 550331; 
int jmAqFPXaXdAlmEmIcxWRoLDozGmU = 365641; 
if(jmAqFPXaXdAlmEmIcxWRoLDozGmU == 412024){
jmAqFPXaXdAlmEmIcxWRoLDozGmU = 902977;
}sbyte TKmNsYlmbxTUUIehHMJALjTZDdCF =  -88; 
string swoPsijzpAonbMsZRnOSKlJmJ = "njoxIeZGCzGTMZxmsawWSnjdL"; 
short jgfNbfCUZjUSk = -14610; 
ushort TuYCPwx = 58296; 
long NZaakLqKQzxMENkeyczhkIxdp = 63510377660623885; 
byte DUYZiBBzXNhR =  54; 
short XtxAHeDWKCXDebOlHttIjz = -28016; 
short LiAqiSDyVRqPFEnZXoyk = 29542; 
ushort iCCzRJBzphSWVdG = 34522; 
long fjjxybPfJmjHdK = 78917223855183150; 
ushort bINHsVsNOQRl = 16663; 
ulong AJMan = 87848621732022132; 
float LsPRwPoiNNV = -1.387624E+27F; 
byte dzEMhLZOgdsEFZihDTyeAxwqoj =  129; 
double tSQQoFLjdM = 1.084404E+17; 
while(tSQQoFLjdM == 1.908072E+20){
double FFsDwitHd = Math.IEEERemainder(3, 4);
tSQQoFLjdM = FFsDwitHd;
int[] aAOkDNpwyX = { 9205147 , 95733 } ;
Random hTtVOIocAqtJapwI= new Random();
Console.WriteLine(aAOkDNpwyX[hTtVOIocAqtJapwI.Next(0,2)]);
}long HNuLeUEBzuOgIUwDQySwXuzTHRNV = 35194413615874503; 
uint RTeOFeElbfUSRzflamoC = 27; 

}public void Itsapq(){
long adiH = 58941145675451667; 
long noV = 28602457169740589; 
ulong umenUucUOJRgVR = 20322580641410835; 
string YOJzV = "FdlUPD"; 
short BwF = -24778; 
uint ekJhfjLUemPpiZpgLFY = 7232769; 
ulong acWHlidVCFyaNfCtLxLZZfztoLt = 47065505194878032; 
sbyte EALasnLM =  123; 
int jkn = 3627; 
if(jkn == 908681){
jkn = jkn + 568831;
}int BQtVtMVKxyx = 14; 
if(BQtVtMVKxyx == 967732){
BQtVtMVKxyx = BQtVtMVKxyx + 639379;
}float IphqBPUIiDuNCHmYVIxUYQp = -1589452F; 
ulong gntQ = 24675016626788674; 
sbyte eMhbLUDUYdpkXPRa =  -119; 
ulong MNuQMgJMLlHyQpuUdheTTAl = 20781894561323072; 
short fuMKWwVchRJsHBUEUNSXUT = -17008; 
uint BmcDM = 21934127; 
short LWIjgNiRMIDWJZjDfqhtRl = -23219; 
byte xiTAxkycUQYVKGePch =  148; 
double RmSaGoc = 1.814864E+31; 
while(RmSaGoc == 6.884222E-05){
RmSaGoc = Math.Ceiling(Math.Atan(-5));

}short EedlxtgESueHsuMSXXmVPQjR = -7301; 
int AkkFlT = 42762438; 
while(AkkFlT == 42762438){
AkkFlT += 798201;
}float CRVP = 4.537175E-27F; 
short YVeV = -4969; 
string VlRPkItHQTNFYzPOiuSmZoc = "zRxlpApyOJtRqaIwi"; 
byte VfEsF =  50; 
sbyte BENOHjopukgqpDpRofYyhNyeOigx =  102; 
uint biEQTaVMHbLmkbTPtQNdAlKON = 66; 
double cTwWtfiNNxQttqw = -1.739721; 
if(cTwWtfiNNxQttqw == -4.540246E+30){
cTwWtfiNNxQttqw = Math.Ceiling(Math.Tan(1));
for( ; ;) {
Console.WriteLine(1.871184E-16);
}
}sbyte ugjqhmkK =  -74; 
float yRSMRloX = -2.654466E+20F; 
float beXKzmchcjbAAVMgYbS = 2.34448E-06F; 
uint SRMHSyoexEBhmkEN = 155878; 
ulong nQsmWAh = 78824083516542277; 
float GkKNeFVYNAWwouIKqmbzAK = -1.356869E-16F; 
short BEqFJ = -26371; 

}public void aRXbuikynialELCtAlqdu(){
byte TwyadsIhRRLoDLJWdRblBsdMtZk =  211; 
ushort bpOVdHXhgeGb = 12032; 
ushort xYIRommtawCqMIehROAQuCIHcZ = 60224; 
long HAhJSXbDadePWNBDokBcXOmKFzUp = 37945946770597817; 
ushort TKdFRLwazOheNOwHZSmz = 5100; 
ushort LDkO = 49119; 
byte tFiHsRZsXDoEagiW =  10; 
uint VxCIVFAIscGWIPXuDVfxFIHULiBt = 528708348; 
int lQpdRWn = 85; 
while(lQpdRWn == 85){
lQpdRWn = 341291;
}int PdMHaWfsnQIXMegLRxxL = 474245703; 
if(PdMHaWfsnQIXMegLRxxL == 600865){
PdMHaWfsnQIXMegLRxxL = 643078;
}string wapDfCLpjUFmfzcnjZ = "YMyQUVUPpjibtTDXm"; 
uint SktRC = 835280495; 
sbyte txFsGfeRXkxIkMwfaSl =  -32; 
int QPfgqGVWTcMoxUFRx = 7827; 
if(QPfgqGVWTcMoxUFRx == 30542){
QPfgqGVWTcMoxUFRx = QPfgqGVWTcMoxUFRx + 46461;
}ushort zYLBdGPdRHQmAPAVasuV = 60908; 
short CjxYFQCONJASIkwP = -4315; 
double FnOEXzpQ = 1.35942E+14; 
while(FnOEXzpQ == 1.066209E+15){
FnOEXzpQ = Math.Floor(-0.0001617768);
int[] LmNGAAoaDeehPKjbehPMYo = { 1492863 , 14817 } ;
Random LZXCyeugKDazkxL= new Random();
Console.WriteLine(LmNGAAoaDeehPKjbehPMYo[LZXCyeugKDazkxL.Next(0,2)]);
}float OOYxkmIKB = -1.829093E-26F; 
byte wRbHDYRViYuFPlZFtZHRNC =  166; 
ulong NnQwTLConctweGidqF = 2762788014397097; 
ushort xZUjodPLbAomToBiG = 12059; 
short zNwWShLqlhpMp = 32643; 
float hjfaRzcRoD = 14830.73F; 
double cWSbdJEYqhtyzUjEXTwlbxdnEZ = 448117.2; 
while(cWSbdJEYqhtyzUjEXTwlbxdnEZ == -1.552945E-06){
cWSbdJEYqhtyzUjEXTwlbxdnEZ = Math.Ceiling(Math.Acos(0));
int? ZhHMGTgKRGSEzd = 4857303;
ZhHMGTgKRGSEzd += 53351;
}short TmLSRmyqUtQcWWWiZcMWndwd = -4666; 
short joEpotT = -28414; 
double ZNSiSgjgTNHeDDyPuCQRIxAe = -6.087865E-36; 
if(ZNSiSgjgTNHeDDyPuCQRIxAe == -220.5126){
ZNSiSgjgTNHeDDyPuCQRIxAe = Math.Exp(2);
Console.Write(ZNSiSgjgTNHeDDyPuCQRIxAe.ToString());
}uint ujPBAVDzWhsaggQQCMCtzY = 185750924; 
ulong BzsDkoMLIWbeIjPVZGGCsFODJ = 55878193745723673; 
long eguXcowzTOxds = 59902192729200848; 
long TCpHzlCxsaPLPGIDmXfUPlGH = 22927114902176437; 
ushort BxNIWmsqedD = 57353; 
ulong CbZDQQaHVKfPP = 22581508659766699; 
byte FaTDyhDdIVdJZCFIFoTBdDgz =  8; 
double kmEoooluD = 7.87282E-19; 
while(kmEoooluD == -9.455989E-12){
kmEoooluD = Math.Exp(2);

}
}
} public class lXwyQEoW{public void IfYZummALQoOthEIRfI(){
ulong QgVMLHxJmZjxcVIeEIDfOlyswSQRP = 33245106419005902; 
byte GRQyQeqpmWEmzfHlFKDpS =  209; 
int nCDmydJapYWoYOImeDEVlMaCDu = 2892; 
if(nCDmydJapYWoYOImeDEVlMaCDu == 826030){
nCDmydJapYWoYOImeDEVlMaCDu = nCDmydJapYWoYOImeDEVlMaCDu + 793374;
}byte jnDdTimDXBjsmRbplpmJIDBbLcQu =  128; 
ushort gcbEtL = 943; 
long JKPBqjhxmkLBC = 43456065147340087; 
float Etcp = -2.433298E-19F; 
long kHTaYNaKBVugoJGGAOW = 71641743533066882; 
sbyte UWiEGAUJEo =  99; 
ulong SWYMNUNyRyqduEPSjqCTuGZlD = 55901610761864135; 
ushort WddUquSdzRBlVeRtXsTxlXMi = 5708; 
long BVZ = 25803646780535492; 
byte DDBucynIfDBJZXEY =  83; 
float HtqiACokhkcmBhuTKxqgIxtw = -0.004284822F; 
int djUylJEfk = 326; 
if(djUylJEfk == 242276){
djUylJEfk = djUylJEfk + 115358;
}ushort oaCF = 5511; 
float ZNhkaWQaUdgYqFaSF = -5.191917E+11F; 
short yuupWsdzfHYqs = -2727; 
uint sfoEbZJfhfzoiWqx = 155185; 
uint NVdFMDOyhHZg = 334999; 
byte tNtzMoh =  211; 
float pjoDOjWfNxbKRzklXBgqMRKj = -3.300743E+22F; 
ulong IyNm = 46600466240163275; 
byte FQfJWUmAsHLDcz =  125; 
sbyte YwTzoKPZTlHeqIqg =  -51; 
byte duTUQhfgDiVOfbgUusYogqKcIH =  104; 
string txNlGcefBcqjDmQYzCckOXOsYONLQ = "iNehiNiDA"; 
double OTo = 5.509539E-32; 
while(OTo == -1.681007E+28){
OTo = Math.Pow(5, 2);

}uint mRkRzqYcGttBCAcITc = 4203740; 
uint YdHNuVdUacgWsFShKJRlJLX = 67; 
float cCFYAf = -4.839884E+24F; 
sbyte LWcZMtpu =  25; 
sbyte PLsZkldmgBdCOat =  -24; 
sbyte QSMMXYMPmXsHMdMJnQlY =  -56; 
float GZKIwnYsXkNPoM = -2.575186E+20F; 

}public void mwYfVAPZPfNDi(){
sbyte WwgMnqqBnFux =  121; 
short XjcSLTVpLAUIHil = 19390; 
ulong uSTGq = 35694087745919327; 
ulong qhDBqwAbfwltJKqcQGqFy = 84510835277154425; 
ulong xeXobKnz = 23965666209315769; 
byte HLVMlmNPJYCHCPplQo =  185; 
ushort tCgJKHRiOww = 35486; 
ushort dnKSKDlapWbtbyXoSI = 54965; 
byte jOdDHWHVEf =  24; 
sbyte lXfxbSes =  -71; 
byte pFtlzDNLhpUAHpdHFeMR =  239; 
byte kZixGZhVYFxlcDwxasuZUszAf =  247; 
byte nAtKdOiloggLZuVV =  48; 
string ejJ = "zDnVSCKpIPJIzkNintigTl"; 
long BxDobXgpFPVhPSqLGRAWqNyLyVNMa = 15189674586750393; 
ushort KJscKoFtwc = 30893; 
uint MOIN = 852786760; 
float juodXJ = -13.98631F; 
ushort MkDPnZKNwSCqKCaPBfGIwfDhBnc = 64886; 
ulong pTYgMqBXAJdlyZFSLNd = 62200979854311600; 
ushort ZWKaPCJtDQ = 44272; 
short GWFiJDDIDoWEygYjIf = 11159; 
sbyte BNtlIzWAiXuUFEYjpXfR =  35; 
ushort OxquLiVKaiVejlXaeHhu = 21937; 
uint SEZaqiEnacfFPpdlqnnuL = 48008439; 
ulong tWakAdpmNARLhEBxUUbdHhLawRfdh = 52663453720913708; 
string ZQscRmBiKOXQPyLqk = "aPJXmeOpN"; 
uint GmURt = 26; 
short TNVIx = -13107; 
byte uHBLObC =  71; 
float EnoC = 1.202914E-39F; 
string BdZww = "cfsnUaBXUGWfYu"; 
float sDBZKaQsm = 6433879F; 
short hiYldIJJnuSCHtJFucJmnLyV = -6272; 
ulong oaaa = 52699893976951631; 

}public void iDdhIzdIhNSSWfCxLcRpGu(){
long uxsAHlHCgW = 74461588763603617; 
sbyte hiBdGpMVbF =  -98; 
ulong QOkfEaVgqLZcQQxPUHlmwflWoTp = 54759183487641674; 
uint Rfuk = 81; 
uint aPBpggXUDALDnQLL = 416372; 
float iOaZVMd = 1.761443E+24F; 
string nOxndPjEsF = "MpeYOawJPLZoqmPCwwdycBlxOSC"; 
int mSAEnbKUWXcehlDozebonOhohSk = 65; 
while(mSAEnbKUWXcehlDozebonOhohSk == 65){
mSAEnbKUWXcehlDozebonOhohSk = mSAEnbKUWXcehlDozebonOhohSk + 790751;
}ulong TAMdkeHAzasUoKhpO = 86294388925700013; 
float lMbEpuKJPMaQjIoKnPX = 2.513504E-08F; 
uint hYygejopJtAuNyycBGasEOKiZi = 941183226; 
int VkMSTFSmFABwqxK = 5375; 
while(VkMSTFSmFABwqxK == 5375){
VkMSTFSmFABwqxK = VkMSTFSmFABwqxK + 445494;
}short ipuoohVjsKlzn = 10886; 
double TjnaCShxmzMHNgTaFR = 63.60535; 
while(TjnaCShxmzMHNgTaFR == -427525.9){
double MfYwFHJX = -2.843968E+13;
TjnaCShxmzMHNgTaFR = Math.Floor(MfYwFHJX);
try{
Console.WriteLine(TjnaCShxmzMHNgTaFR.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}byte JuSWVUeYAmEuXaYHHMHNNon =  88; 
string wFASZDpSAXuNCxje = "OBPtQqqGJUBaufBUuzBz"; 
int adfBamUjj = 669924; 
if(adfBamUjj == 831469){
adfBamUjj = 261966;
}sbyte mimqJQRfRUTgYKGbBBL =  -61; 
short eMzKXsNFJHm = 23915; 
byte tVsnnwitacl =  222; 
string ZqDTqXdBTYTAsmkmTfx = "IGhxZTgLLw"; 
uint ohGq = 38; 
uint lTbuwPcWhOj = 43; 
ulong tjfUyQsPzAPkkHTiAEgcPENbjRkEX = 72117729132342987; 
uint ATdEU = 59136338; 
float nGKEoMjQxeK = 3.448733E+07F; 
ulong lhhKbwLkazIiLaSwGZVoqDOsPZ = 18286477384011960; 
short zUPUEwJeIDzQEJeILBCKtPV = -29945; 
double qkaNeBIfWLjaMWTKKlCHikLRjFKIu = 1.540748E+37; 
qkaNeBIfWLjaMWTKKlCHikLRjFKIu = Math.Pow(double.NegativeInfinity, 2);
Console.Write(qkaNeBIfWLjaMWTKKlCHikLRjFKIu.ToString());float UfJjQdJMibJujEuVxKzPysYXIsVj = -1.304838E-07F; 
int xpJHnOdnou = 695512370; 
if(xpJHnOdnou == 819881){
xpJHnOdnou += 23252;
}byte YuQeADhMCzhztNxtYUUOHIGkB =  13; 
ushort etCUYDxkzq = 52943; 
sbyte DmGfQbknzpWgyjpO =  23; 
int iTIiWmARpkUIeO = 910; 
while(iTIiWmARpkUIeO == 910){
iTIiWmARpkUIeO += 687393;
}
}public void haG(){
string OCsfyUMtXHVdKTpjVdF = "RsjeVOAVwaSbfqAYzjNaV"; 
long IjsqltRmXbtOxsmkKuRXXOTXs = 60181107280206837; 
uint VEymCFNVZNDdHlZdH = 76; 
float puYTbfDIyGIUQsImgotkTUkQpL = -3.92786E+15F; 
byte JsIJFizuBzjnCgViRUqRAQwJaxii =  138; 
short RwBXplgNzONOVaGseVhT = 6441; 
ushort zTXZJiapSjRsSb = 11581; 
int KhScNbAIuddnuybxZbesmzEzzUFd = 74738341; 
while(KhScNbAIuddnuybxZbesmzEzzUFd == 74738341){
KhScNbAIuddnuybxZbesmzEzzUFd = KhScNbAIuddnuybxZbesmzEzzUFd + 222916;
}sbyte DtMRyqnfWcyMQVT =  -128; 
sbyte JkFuFSxxZ =  37; 
ulong YinFUHp = 12436912144856866; 
float MUqRdDXThO = 4.493608E-08F; 
string GtOeoylblKSq = "agShwEinOwsUHpLNSxSUEFe"; 
ulong hwcLIHYMOXNp = 41363913621669090; 
byte gBntnZyjwcdQmhgDfPti =  216; 
ushort xlYabANgshAoCxnsZoECKmQkKMCMh = 3910; 
string ODShpZwLJVLINTFfRyuTnRnj = "NpoXaYogKBZFgb"; 
double TTOChyRgxVLJhCWnILEJfNIWxC = -7.884982E-13; 
while(TTOChyRgxVLJhCWnILEJfNIWxC == 5.803564E+13){
double YBJnoziCfnGYhF = -6.999374E-10;
YBJnoziCfnGYhF = Math.Sqrt(3);
TTOChyRgxVLJhCWnILEJfNIWxC = YBJnoziCfnGYhF;
Console.ReadKey();
}short aHXXZkgQVIiodzMHsWtkVTjfx = 18460; 
short pGHKAMUf = 28461; 
int LXxtfAnExma = 9867; 
if(LXxtfAnExma == 914403){
LXxtfAnExma += 152904;
}ushort BkTVApbzQsFHKLHJnzAeGikVYp = 21498; 
long tQMJe = 26949573265718016; 
ushort SHpDtWSy = 42838; 
short sZYXjYtty = 3243; 
float GGCRlCxZGLZcsZOoiCtepSNfTeNe = 2.998808E+10F; 
string aJKXReHVktHIaVGkkdjVj = "HOSxXgPsnYLpbJoQydVyTXk"; 
sbyte XADUH =  -17; 
string mbVewhAgTeJWLTWV = "HgaMUKENqZHlz"; 
long GRPHuMbWwqaSCitdE = 6139428210615678; 
short dhTUuyhqUepC = 10579; 
int GuURcEXW = 59; 
if(GuURcEXW == 211229){
GuURcEXW = 545797;
}string octXPCyGwuSAYm = "Rwgytz"; 
uint PflQoxQWORK = 48; 
short weXoxILzXQAhkeeKWPMF = 22529; 

}public void MiiFyeJomHDOLtxjdI(){
string oZQoGSKhNkNKPStOfRjH = "IDSFxtMmNLeBjoMtUZGwBDbU"; 
float TaIdmyySHxXKizXUaAIiFEHwkRFlo = -101.3039F; 
double yBMFJzIdsIowkRgyHDCmJup = -1.448083E-20; 
yBMFJzIdsIowkRgyHDCmJup = Math.Ceiling(Math.Sin(2));
object PguwnfilqkClEhubmSuwxUYWoT;
PguwnfilqkClEhubmSuwxUYWoT = 3.855844E-33;
Console.WriteLine(PguwnfilqkClEhubmSuwxUYWoT.ToString().ToLower());sbyte KtxUhyfuyuttKFaqKGdIeAgUEGzQt =  63; 
double SDmSQEHBztyUjXIUz = -2.295505E-21; 
if(SDmSQEHBztyUjXIUz == -0.0002265221){
double qCZpaLuRChqhBfYEiL = 1.276689E-24;
SDmSQEHBztyUjXIUz = Math.Round(qCZpaLuRChqhBfYEiL ,1,MidpointRounding.ToEven);
 bool? YFqkBQmuJdGnmfBitNAs = new bool?();
YFqkBQmuJdGnmfBitNAs = true;
}byte FhAmCmSTcZspMsjybZE =  100; 
int fAXEwCCfkEEVOnbpP = 2737; 
while(fAXEwCCfkEEVOnbpP == 2737){
fAXEwCCfkEEVOnbpP = 820398;
}float BCfuYkPabbZ = -6.339299E+34F; 
float qXOONREVxLj = -7.99663E-30F; 
byte QJfoVawLmtOVMlbZSDXaKBcdzWG =  175; 
long bJNgkqjJWpWlCwYWRKwLsHi = 5590346156202167; 
long zqfh = 6313305458361071; 
uint IfDfWoXwwBBVVLhTYVkjcdTOlJKVj = 506376783; 
sbyte qZXmSzxedWFHLfaIoINTnWE =  -71; 
ulong mFckwashEMkHfxOeZgohBEKmVs = 46448758126750159; 
byte cSCFnDCnyJCpeiWxXHtKUPpcm =  15; 
long pAigkWc = 49581048610033561; 
byte xkpAYOSGCXmCzqkTtYXBwfig =  202; 
float NuIzZdmQS = -1.719353E+13F; 
short pCKYPGgtJfJySMwqbiKgCe = -18244; 
float nykCsAKHljTxjsjL = 1.166085E-33F; 
short agqSzRkZkjkpQzBhVRpHYRHDCVRRA = -4857; 
long WUz = 11998023444049650; 
string EGLRcPcNJajBPpEZuAPCjtVaubCPP = "nwEFXofZyHGn"; 
short nHscADUkHBOkKLybmnYpNPWjsn = 24165; 
ushort hVVe = 2888; 
ulong KAplydPcQNFwDxtspYgAKhK = 59880822430923290; 
long KsOXMSKaTtGKzSGOEFgVmdCPfEYUp = 47192947775467067; 
ushort fxAQOeBkHDKbnaMMHQ = 64125; 
float UPKxxYN = -2.208883E-38F; 
uint iaazNmN = 58759686; 
short sGxKHjKlnHDx = -18120; 
long usfOjHaCTnZmZtdnnQDxeaMs = 7685362344576456; 
float TuFRhLqeDwIWLSksnGKkLuGOkSSO = 6.462936E-21F; 
long xbAJKyJR = 47826213776391718; 

}
} 
public class gAXai{public void faxuxfL(){
uint cdVblgCqZCJpeyi = 820885102; 
int mTzYZxHGRTpcwVXwQDtQYGiYA = 835328; 
while(mTzYZxHGRTpcwVXwQDtQYGiYA == 835328){
mTzYZxHGRTpcwVXwQDtQYGiYA = 537871;
}int TlEFfVFFCLyJAmw = 4; 
while(TlEFfVFFCLyJAmw == 4){
TlEFfVFFCLyJAmw += 815519;
}float LBABGHcEqKEDJtRLHPly = 1.141833E+17F; 
short lxTCUliJaETeHwZYzgfYVLMNkIJJF = -5529; 
ulong kAojNBJfCVCfBMdlPnYNGqdBKgqmE = 41813301634853494; 
ulong YzZyajHm = 87333547444270990; 
float PAHjwDaRVoGSXf = 2.334329E-35F; 
double HTcpcTSJgI = 1.653728E-27; 
double PcyyjnqHGHpUnyLNaszx = 2.014255E-13;
HTcpcTSJgI = Math.Ceiling(PcyyjnqHGHpUnyLNaszx);
object tdOzeSkOJyklixT;
tdOzeSkOJyklixT = 1.151285E-13;
Console.WriteLine(tdOzeSkOJyklixT.ToString().ToLower());long OatTT = 32012531618598491; 
int wCkRmAEEAxLCfLGGFdtpSSV = 592408682; 
if(wCkRmAEEAxLCfLGGFdtpSSV == 151227){
wCkRmAEEAxLCfLGGFdtpSSV = wCkRmAEEAxLCfLGGFdtpSSV + 364054;
}ulong TKzGnDIyGkxTxhqsF = 42508872507677497; 
int mADxkMVSSIfsd = 617130619; 
if(mADxkMVSSIfsd == 410938){
mADxkMVSSIfsd += 977697;
}uint nuqNVletyCcIguXdOlhB = 4793; 
long cuERMfBFhNYekJoA = 12992170885666254; 
long qjRYsPKSKCJmHqazAuxSS = 40405710195185266; 
short aicOqNiwjwtZORE = 23112; 
string FkDgQ = "lmjnDLdxsdIeFEKLqZlyzgiQf"; 
double BSg = 1.292608E+29; 
while(BSg == -1234.708){
BSg = Math.Truncate(BSg);
Console.ReadKey();
}byte YZRxYjMnQOtw =  133; 
ushort UzPMzxhmRGHNCUgzFYRPND = 38866; 
int KSXqheXkUQdpmPVRMZQsykuKpPxL = 99266610; 
while(KSXqheXkUQdpmPVRMZQsykuKpPxL == 99266610){
KSXqheXkUQdpmPVRMZQsykuKpPxL = KSXqheXkUQdpmPVRMZQsykuKpPxL + 774411;
}ulong kVJCJCHedhqjVYaEPn = 67098144216023998; 
int AjhAaqHQpnJWFoGhylPIZNeUnpf = 886884671; 
while(AjhAaqHQpnJWFoGhylPIZNeUnpf == 886884671){
AjhAaqHQpnJWFoGhylPIZNeUnpf = AjhAaqHQpnJWFoGhylPIZNeUnpf + 927896;
}long TaSjFqjmBFEUiITVxy = 60915055928974985; 
long awmZTLNgGbkqPBFoLxXpauyYZPSO = 47159327725770772; 
short azUNpumGhlBYBnntWH = 29296; 
short dSY = -7852; 
double RJBnAWgwzwnddLtIH = -1.779254E+22; 
if(RJBnAWgwzwnddLtIH == -8.454919E-31){
RJBnAWgwzwnddLtIH = Math.Ceiling(Math.Atan(-5));
int? bbuSPYIDTEIxkLFRpLuiJqTDzSAV = 7224395;
bbuSPYIDTEIxkLFRpLuiJqTDzSAV += 57196;
}uint LiRtWzNSJo = 3931; 
uint RdWudzHcsHXyngj = 55523529; 
float uCFWZtSXkGlKgTNXzG = -5.61451F; 
double JQkxAjGJZMuRhXlhWbkC = -1.453157E-07; 
if(JQkxAjGJZMuRhXlhWbkC == 3.308235E+25){
JQkxAjGJZMuRhXlhWbkC = Math.Floor(82.70277);
Console.ReadLine();
}byte PHGCSMzwtWbhAGdxZZZVFLZmdq =  180; 
byte qiMmszdJccWXCDEbdVzuZthEoo =  19; 

}public void syCblCbNoVhhaOWWdYch(){
ushort ZwlTUYbiX = 4126; 
ushort pGCULaBQMiOqtl = 28054; 
byte plWkgygQtIdMgXeO =  151; 
uint wimcIGARIVAI = 798047; 
long RykihwestyeGlKDONeRq = 83385258079664419; 
float CWzpmpuojaeVJjBUGXsMVyYqFIiFD = 5.432176E+30F; 
sbyte uVhhWxmuzVQmhBe =  -105; 
ulong CEh = 74759592317423251; 
ulong eVxootMJBeqxDhdxheAPA = 88874221404064591; 
float FLlMqL = -8.519955E+22F; 
long saz = 29962370872791589; 
short jKoeYyjMzJYMLpxMLWoOOhthCJpu = 28266; 
double epOcccHmsCoTs = 2.395815E-35; 
epOcccHmsCoTs = Math.Ceiling(Math.Sin(2));
int? NeaFsTsPFaMVcWkN = 5639598;
NeaFsTsPFaMVcWkN += 6277;ulong kuDHtmHP = 4705613801148579; 
ulong gaXTcCCUlyuXthXofKeaFLt = 27246048423177988; 
uint PXPmqsafAa = 39; 
string EDLSnXQixHfyY = "NixgVIEkibOsdUf"; 
long zsKBlZXc = 69721045671551864; 
int pFczxQttJeWT = 63228187; 
if(pFczxQttJeWT == 637264){
pFczxQttJeWT = 91691;
}short CjzetiXKbUUYCCPehZVUhSpTfJFD = 18442; 
long PeGRggXenSJsPjXq = 20901915879166121; 
long FYLFwVqueUWLJYjUIbzBCJhqko = 2978568152460680; 
uint uKyObufdwJFKeNCsqiqWRttfOVCzj = 54; 
ushort lBYFLAgEdWCLyp = 7463; 
sbyte zZVFhbpqZfTI =  98; 
short hmTmEN = 7782; 
long tNxSwqGw = 44446971808120894; 
long MunVjHwDkx = 23848034241170150; 
int WIBzncIEHuQPVAU = 9340; 
while(WIBzncIEHuQPVAU == 9340){
WIBzncIEHuQPVAU = 427937;
}float iDRnPl = 1.363317E+09F; 
long jqdTQHhZRVQHeHeXbOOTQcdCqW = 63504983901811615; 
long RwgQuQF = 69369651769061464; 
string PchnCDUjtgAfcziVWgGIYzayg = "PzmGbiOWt"; 
float UpTmHbJfRmeoomLEwT = -3.315693E-35F; 
long oNwWOf = 74868896552943397; 

}public void uqxqsoYKiXtqFtcbQUNbfRK(){
byte PBhZZYjURaCULsSpcm =  250; 
ushort MYddAxoPHDIiODPRLjYixOs = 33562; 
sbyte DULcF =  52; 
float dCdVADVfjyULXQdAsdalsXGD = 4.990119E-08F; 
float eEYXpYAmOPKwnq = 3.494829E+29F; 
int gNQkLBLHyfVBZe = 48934413; 
while(gNQkLBLHyfVBZe == 48934413){
gNQkLBLHyfVBZe = 944771;
}ushort qRMDuFYauMWAngFTSymKBtqmPFMOp = 61871; 
ushort bhxZAigfIyMjFBRDUhObL = 46050; 
uint AzNPSjKypmygLGtKqgcdIgIoOjUi = 39; 
double JzAbxsnyNnUq = -9.803276E+14; 
JzAbxsnyNnUq = Math.Ceiling(Math.Sinh(-5));
Console.WriteLine(JzAbxsnyNnUq.ToString());double oImxetn = -1.605182E-18; 
double FimZLMupxeRMjBbDinCdyH = 1.314441E-17;
FimZLMupxeRMjBbDinCdyH = Math.Sqrt(3);
oImxetn = FimZLMupxeRMjBbDinCdyH;
try{
int YeDohUnjSb = 2861742;
if(YeDohUnjSb == 74196){
YeDohUnjSb++;
}else{
YeDohUnjSb--;
}
}catch(Exception ex){

}sbyte XzZIZNiRXPBHYapgknsbygsYUSeS =  112; 
ulong kTdFcefSyeTQfMqkKRsDaugAQfbOg = 40906906222545721; 
ushort cexVWFqmlE = 42449; 
ushort FnY = 54292; 
ulong OWcsLPPYJoxDomEpyF = 83401242752006541; 
byte pTSzbqJqZN =  182; 
sbyte HIXQas =  -91; 
uint QtsCLodnhl = 513468; 
long yYCYFmqLTtdmRVZigfQbczfA = 8595127414307674; 
double BRAuDkXfBYzyyinZexKINZSbs = -2.370658E+37; 
while(BRAuDkXfBYzyyinZexKINZSbs == -2.873381E-09){
double wiKNDGdXXgbjWeyGugWqeBYybqlsZ = 0.000593468;
BRAuDkXfBYzyyinZexKINZSbs = Math.Floor(wiKNDGdXXgbjWeyGugWqeBYybqlsZ);

}short bmRNnzmxTbq = -20657; 
uint BziETW = 63; 
long RTkNYgRERjYwLlHwWQSjcD = 69120119296742957; 
short GfVwnAyMjNtph = -26896; 
ulong WzZlPyJowAowmXgwVEQGplhysfap = 44433894057551609; 
ushort GhyH = 64743; 
ulong cAmGXuFo = 7541205537039954; 
short mWWGqZ = -23545; 
double XIhKlsmpHNKlOXeJomJUjWLeB = 1.131026E+27; 
if(XIhKlsmpHNKlOXeJomJUjWLeB == -3.660043E-28){
double IHZXzYoDcKJTAYSWcM = -0.01448666;
IHZXzYoDcKJTAYSWcM = Math.Sqrt(3);
XIhKlsmpHNKlOXeJomJUjWLeB = IHZXzYoDcKJTAYSWcM;
int? WdiBB = 9128976;
WdiBB += 78198;
}double pJEkzGoIunLXOAa = -1.171824E+32; 
pJEkzGoIunLXOAa = Math.Ceiling(Math.Acos(0));
int[] bluFltTAOl = { 6325326 , 50893 } ;
Random KSNpkmnkXY= new Random();
Console.WriteLine(bluFltTAOl[KSNpkmnkXY.Next(0,2)]);sbyte DkcReN =  6; 
short CgYsYbyjYCpgu = 3551; 
long EIQZm = 20663212894058069; 
long LsykjlUoiBOIZTZh = 7564110812510433; 

}public void uOUoTH(){
uint bGPWqzSJsHz = 751499; 
ulong zCBxVhwZZpfP = 14513335721842619; 
byte lpWUTMeT =  169; 
ushort wwNsjHhHGPGlKuJsqFlw = 51430; 
sbyte pkBaoJUgIM =  -95; 
string nXLFjqHwHdmBjuOoxYT = "gyhJHTXMJXB"; 
int detUIkyyTJg = 5071; 
while(detUIkyyTJg == 5071){
detUIkyyTJg += 841185;
}byte mNzQQbYqtVMJEKjgpuuPOwS =  65; 
ulong wfP = 18081288484468487; 
byte SJTeChaDEpXYNyyaSV =  52; 
string HSDKqVWSuHCbQDfUqpfOARQ = "LSuoMyIulQlRqukldLjKwWWUq"; 
sbyte FenoPcLZABIfXYhP =  -126; 
short DWfUKOIfwO = 26833; 
uint GWdpYBxOGwMLxFlLnhWFcEFzkFBh = 306829289; 
byte bLAaVSRATBXCtimiww =  35; 
int UUcsFetZoSpihKMfkQxpxLDGcfcsq = 52208234; 
while(UUcsFetZoSpihKMfkQxpxLDGcfcsq == 52208234){
UUcsFetZoSpihKMfkQxpxLDGcfcsq = 131777;
}ulong JpNyBFJIslzBdj = 87308717152661125; 
uint bJCq = 56; 
int VJDBKSWfUYLTERnDW = 300302; 
while(VJDBKSWfUYLTERnDW == 300302){
VJDBKSWfUYLTERnDW = VJDBKSWfUYLTERnDW + 997889;
}ushort LwScsBPJWFwIebyIdyzFGzku = 64872; 
ushort nCGyJiKZyecDyVkIlSVRx = 55752; 
ushort bCPjlkydXRCgphNAwcaDbUaJwR = 64807; 
int EmwhN = 142148; 
while(EmwhN == 142148){
EmwhN = 237213;
}long GHuesdewwuSRRgBfPukUYtFfYDNGf = 61445802606746059; 
short WyJiSEqVnHnLsbNa = -17380; 
string fPpApAfdVP = "fRyVoxpkMCIHzYSkWNibdsakK"; 
int tJNBwgxCymyMpKbpNyhSCzojqMNO = 81069015; 
while(tJNBwgxCymyMpKbpNyhSCzojqMNO == 81069015){
tJNBwgxCymyMpKbpNyhSCzojqMNO += 478935;
}double ZblBbywXzbTsbcnOD = 7.845993E-06; 
ZblBbywXzbTsbcnOD = Math.Ceiling(Math.Acos(0));
Console.ReadLine();double NZKjgFYMiyIKwKUmUkTKUwBYEHu = 1.619511E+35; 
if(NZKjgFYMiyIKwKUmUkTKUwBYEHu == 1.688936E-34){
double eBjnUWs = -8.652365E+08;
NZKjgFYMiyIKwKUmUkTKUwBYEHu = eBjnUWs * 2;
for( ; ;) {
Console.WriteLine(2.262503E+18);
}
}ulong QEyHtUJcziqnhZkVJyEXBzeoYAIT = 1799373961879092; 
ushort OAsCsP = 47733; 
byte ZxLyV =  166; 
ulong qAiJUlQyiOCnBk = 13448048881077873; 
short LwJyVRzglkyMmyUmDe = -21545; 
string ZDOxlVlE = "ykjFYzE"; 

}public void DsoubpNnlePR(){
string XotLSUaJWmEcNmDIAxKjCRLPXma = "ZRxNfDm"; 
float mGJsTwcRZ = 1.89365F; 
long GtgTtgEFNGTSKeQQXj = 65835493787248138; 
ushort XYKByxlcHljgpIAZPAjq = 15775; 
string tCc = "tDfbLL"; 
double PWtunTNjLaSsPMRlsze = -2.066959E+32; 
double BkzxubLdwfWyARpsJ = -2.98598E-18;
PWtunTNjLaSsPMRlsze = BkzxubLdwfWyARpsJ * 2;
int? TKmHAKSFwaZbmJBCxXOaNRm = 19771;
TKmHAKSFwaZbmJBCxXOaNRm += 10136;double liQQmOFHYcxKYE = 1.689193E-22; 
while(liQQmOFHYcxKYE == -4.381679E-14){
liQQmOFHYcxKYE = Math.Ceiling(Math.Asin(0.2));
try{
int GlKWQicV = 5640385;
if(GlKWQicV == 75541){
GlKWQicV++;
}else{
GlKWQicV--;
}
}catch(Exception ex){

}
}sbyte BGLhAKloeMTJwydwMoJxUOQLL =  3; 
ushort tNJXPZIxjidftPyezclwQtgndKWN = 53289; 
string YnoWscVUwQIkEkE = "epLl"; 
long bfZGwTecVG = 40538449442642006; 
int RGihEcB = 84247381; 
while(RGihEcB == 84247381){
RGihEcB += 806263;
}long iWMIcPsUb = 21376479108671555; 
long zxXScstzkzBwVxF = 79803191714241986; 
sbyte LXOIGk =  -21; 
sbyte lRkeKpKqgwB =  28; 
double hcoSoieZLQLquFQFnepEk = 2.373304E+15; 
double HBOnHGqtgWml = 4.965092E-34;
hcoSoieZLQLquFQFnepEk = Math.Ceiling(HBOnHGqtgWml);
try{

}catch(Exception ex){

}double qYIozqhMDPgjk = -8.847085E+20; 
if(qYIozqhMDPgjk == -1.973358E+28){
qYIozqhMDPgjk = Math.Ceiling(Math.Atan(-5));

}int CTMotnCjU = 8002; 
if(CTMotnCjU == 691855){
CTMotnCjU = CTMotnCjU + 395946;
}byte HxysSFu =  37; 
double OaJpM = -2.499607E-20; 
while(OaJpM == -2.223153E-21){
OaJpM = Math.Pow(double.NegativeInfinity, 2);
try{
int TJoshyiO = 8554330;
if(TJoshyiO == 35260){
TJoshyiO++;
}else{
TJoshyiO--;
Console.Write(TJoshyiO.ToString());
}
}catch(Exception ex){

}
}byte snfsEujlDxQVtJgUDq =  234; 
ushort nmkTStFNIbEToqamPTO = 37872; 
double HnbhJShWBxTZdbUALbFAV = -3.629682E-22; 
if(HnbhJShWBxTZdbUALbFAV == -1.228745E-11){
double hRJFHWRLLNGLBhZo = Math.Log(1000, 10);
HnbhJShWBxTZdbUALbFAV = hRJFHWRLLNGLBhZo;
try{

}catch(Exception ex){

}
}float TebOfhny = -4.566079E-08F; 
float QuURIubeC = -3.321229E+26F; 
double OEuIhxgEFtXYokEmMNTjdJbdRsWRP = 7.652673E-08; 
if(OEuIhxgEFtXYokEmMNTjdJbdRsWRP == -3.185006E+22){
OEuIhxgEFtXYokEmMNTjdJbdRsWRP = Math.Exp(2);
int[] UATZuXFUzwyahZZNsYTKlpU = { 9720332 , 87042 } ;
Random bDZtBwVodj= new Random();
Console.WriteLine(UATZuXFUzwyahZZNsYTKlpU[bDZtBwVodj.Next(0,2)]);
}short yShfoAICiRPMXQd = -15235; 
uint GIXRYkudBhWXagyUBa = 134630; 
string EBuhEOzelQLwn = "CGUWMlZBFlQJodocMsDld"; 
ushort QIGFwWgmGTOsVg = 40816; 
ushort udViqUAwQidsScGeienfHEEy = 63953; 
byte BLuWfXNRZCnzNgjGOdVNQinM =  10; 
short iRByjgeYKxl = 31256; 
ushort lGDUG = 33795; 

}
} public class yYQudyZqyIUNIEaAeesJhAjCeBIFE{public void RxRpMSKcfJeHmOslNCkfyQ(){
byte RXPopEQuOMRyVqShPcYKEBmxi =  218; 
ulong tymbj = 16764162079129598; 
long BWUpxJdpwREnpKtfyRLapAQnWR = 40444730671557595; 
ulong oCpxpgSjGMidLpnL = 51226195891317903; 
int WeuMuJXTRVBnLiIGIjJatdbMKjeMs = 14455323; 
if(WeuMuJXTRVBnLiIGIjJatdbMKjeMs == 318153){
WeuMuJXTRVBnLiIGIjJatdbMKjeMs = WeuMuJXTRVBnLiIGIjJatdbMKjeMs + 201411;
}ulong TsBgRAskUZutCYkVFGTdJcjE = 5850825445125450; 
float OSiTDFKBPkLtQKXQnLqKgRhW = 3.870391E-16F; 
short wzFwzMjsNLdnuhmYTS = 13947; 
uint ZhKHkJXLmWmnefqGoUeBFcDId = 9796; 
int ceFTNxp = 788085804; 
while(ceFTNxp == 788085804){
ceFTNxp += 648695;
}uint QaocWtFXHXlWGbEVKuCkRJws = 2070532; 
long HanXDORdaWIQyhXV = 31436832103268644; 
sbyte XgFXsVIeqG =  48; 
short RkAUNMLNKLARj = -12499; 
float nHwRmQKzdE = 3.149988E-27F; 
short qklfTJMyHfwDmxffiFRRcYnz = -24921; 
string bQiZEuUtoGexiCRa = "xLEntYwhqCO"; 
short NZSiLHyfShtcQBehJolYYcwylJSO = 22581; 
byte fasSeEZRjDjEDjlDyqZJmDuYLF =  101; 
double PKn = 4.903867E-17; 
if(PKn == -0.003679277){
PKn = Math.Ceiling(Math.Atan(-5));
try{
int YnWjePqwZPBZimMgVWumxcJKweC = 6420664;
if(YnWjePqwZPBZimMgVWumxcJKweC == 22177){
YnWjePqwZPBZimMgVWumxcJKweC++;
}else{
YnWjePqwZPBZimMgVWumxcJKweC--;
}
}catch(Exception ex){

}
}float ciUaHAWFmPanRbwhTynNhQe = 2.929702E+22F; 
sbyte zSGOSHMsxOHVuEadbbqowiKjjXhHm =  -113; 
double BAyNIbhH = -2.803944E+13; 
BAyNIbhH = Math.Floor(1.325604E+28);
try{
int eetOwuUgmxZUKjMjp = 7672432;
if(eetOwuUgmxZUKjMjp == 64293){
eetOwuUgmxZUKjMjp++;
}else{
eetOwuUgmxZUKjMjp--;
Console.Write(eetOwuUgmxZUKjMjp.ToString());
}
}catch(Exception ex){

}float RBVSBG = 3.225957E-10F; 
int MYYIlCJDjiFLeBdWGkUi = 30936211; 
while(MYYIlCJDjiFLeBdWGkUi == 30936211){
MYYIlCJDjiFLeBdWGkUi += 698852;
}int IuZlYjwadldD = 2214; 
while(IuZlYjwadldD == 2214){
IuZlYjwadldD = IuZlYjwadldD + 134037;
}ushort OFatAzJKTXVCY = 19059; 
float isMUedStgsSOExAHnhFcuGNB = 2.784667E+15F; 
sbyte qDFapJ =  -1; 
byte CpLMHmhGVgyznm =  212; 
long uEGkutWMcENSyVWokNRUpOtFV = 37581794030185185; 
uint WEjgXOaEJu = 47499093; 
byte idBZQCGjaoQOneana =  132; 
double ZMaGUfpkKkhfugzZPNB = 7.601009E+12; 
ZMaGUfpkKkhfugzZPNB = Math.Ceiling(Math.Sin(2));
int GdGGkqKZxtHVkfeuM = 16278950; 
while(GdGGkqKZxtHVkfeuM == 16278950){
GdGGkqKZxtHVkfeuM += 24513;
}
}public void mKTAcQBDk(){
float zpAafXpSDaAUMJQkDVynU = -2.641518E-11F; 
ushort zkXGSdKtyGelKcjXU = 30851; 
byte PhQlFYHNEPxXfzIMtlLWPCo =  13; 
long TTnpZEfHNpuMbUxXPIjhgkuyFIOpX = 35634919821862879; 
byte SRAMssIjQASNHTESJzQMsQQK =  123; 
ushort kxPJyCTqpHzcfXyXReB = 16131; 
float NxoKCycbQmVMxOaRukOCmljxLPUS = 2002131F; 
double LnKttOa = -9.316695E-18; 
if(LnKttOa == 5.526213E-22){
LnKttOa = Math.Ceiling(Math.Sin(2));
Console.WriteLine(LnKttOa.ToString());
}short dfghLgnFIztodsGbK = 4600; 
double XSVAhMPaUZVFHxQDE = -2.461861E+28; 
if(XSVAhMPaUZVFHxQDE == 2.854771E+31){
XSVAhMPaUZVFHxQDE = Math.Ceiling(Math.Asin(0.2));
 bool? KcLfasDCKCqgUURFsWjTLGlzlK = new bool?();
KcLfasDCKCqgUURFsWjTLGlzlK = true;
}string HyPEiqyVOQXMCoQheRlT = "kbQwHhbdQod"; 
byte ExWbchPPtLP =  138; 
ushort nJZLokqwwYhCiwSXCTzlMulcOe = 5672; 
uint qbTmoOuREPcYhaYTBYuXwiVdgg = 445730686; 
double jgQDeWooUIygXdxnFtmimgLEyhfQ = -9.851761E-27; 
jgQDeWooUIygXdxnFtmimgLEyhfQ = Math.Truncate(jgQDeWooUIygXdxnFtmimgLEyhfQ);
int? DBjOKdxmfXRziMOBlqABeZylEa = 2490600;
DBjOKdxmfXRziMOBlqABeZylEa += 61402;ushort gWCAWAZkgdsYZcUsfODcXjJ = 1320; 
ushort WaDoVfu = 48787; 
string YaiBzbyXLZdfc = "czdpQXnBcMDYIRhlEmYAueFcFWN"; 
long yfQXHmyceXh = 36272512822507410; 
ulong xQgECXUaMTjDjFH = 84331319357215635; 
float WghfeDgVSbpJ = -4.246854E-32F; 
short bsdXFnuQkzsbfnXBpk = 6233; 
float VHqcHqDqdHWphhgmUNIMqU = 2.474763E-23F; 
sbyte FePtjByRgMMUXXRAgSwMYgQm =  -22; 
ushort euaafXRNzCKyxTpawBZPLVSOOQuR = 29885; 
byte XMEsxfLGWgktYyuYT =  163; 
sbyte MxIljDmmpEZokY =  60; 
long BUUKVdJADTptRBjsSQCdqhUTyo = 54578540396514885; 
short kLRtYMIJWTzcQWshkHDsZbm = 6951; 
long EuUGKMiyggZifFBlWF = 33583539058652791; 
int Oiz = 6628; 
while(Oiz == 6628){
Oiz += 784422;
}short NUtQkib = 2236; 
double OeIPulFEcgIjiJXIOBKxmxHwd = -2.769532E+34; 
if(OeIPulFEcgIjiJXIOBKxmxHwd == 1.700507E+12){
OeIPulFEcgIjiJXIOBKxmxHwd = Math.Ceiling(Math.Atan(-5));
object MKSEuCSCFqzlLYowYu;
MKSEuCSCFqzlLYowYu = 3.135018E-24;
}int GFMoUiMsCtXLaYMdoqKcH = 33; 
while(GFMoUiMsCtXLaYMdoqKcH == 33){
GFMoUiMsCtXLaYMdoqKcH += 71390;
}double MACya = -2.631929E-20; 
while(MACya == 9.507699E+08){
MACya = Math.Ceiling(Math.Acos(0));
 bool? mngPG = new bool?();
mngPG = true;
}
}public void affJhLmIQNoGtmOxyW(){
long uisLLm = 82357820710538006; 
byte bgyGAcQ =  37; 
ulong PKjmDglFjSNwXUukH = 46927201985370004; 
short fIssjUizHSPXmju = -18202; 
ushort HiDknxAOWVSJGR = 14285; 
long XSBEBP = 20930244861976868; 
ulong iOhVUtVTxXmGYlXMFdyt = 86978633369605285; 
sbyte mJfayddNmEWHgDCQPgUI =  12; 
int tOpIOPwzqOPLFBXPUJRRTxlM = 863267; 
while(tOpIOPwzqOPLFBXPUJRRTxlM == 863267){
tOpIOPwzqOPLFBXPUJRRTxlM = tOpIOPwzqOPLFBXPUJRRTxlM + 715027;
}ushort HTNlg = 27876; 
short RfGTqSWpRqZZLPVWDEdVAMKGqLdL = 17052; 
long ytUaOQsuMWZSxLxtSYRK = 55732267865557779; 
int lDfJDHRPhfjjqYBUmUUslNW = 11; 
while(lDfJDHRPhfjjqYBUmUUslNW == 11){
lDfJDHRPhfjjqYBUmUUslNW = lDfJDHRPhfjjqYBUmUUslNW + 914746;
}short QOXLyNOiQDcRLBtbxXkigIoD = -1085; 
int EyJuT = 30799977; 
while(EyJuT == 30799977){
EyJuT = 273927;
}string ALePiW = "CMdCBLjKi"; 
uint kRIkMgiibwgXI = 86; 
string wEAuEtERthUiFHkXYclAo = "YHZURuOxuHIBnGZbIyettZoaxOWaJ"; 
sbyte FuxFTuJDidWNmCdsFXXw =  70; 
ushort GZsZycyNOoYcgRagIZVVpxlbztw = 21977; 
string KTjzRGBukALpizozXCFRpw = "AHiNqBeHgKcFfTIawGmOwjZmNN"; 
string UNluoZLSISjTgS = "GfTiFoBjkcIyoHVaFBJ"; 
ulong zIpfiQsDxuIEF = 51632140084065880; 
string HDxlMSFtyflg = "DcxzhHPJZUKlOfa"; 
sbyte tQyqJENFHKRbjytmK =  -43; 
uint IlWlJIIMZXMFnVNxKgTzKUCWXIQxp = 98136054; 
ulong fUgTkIbWaFJXmNnL = 70178287345148497; 
double bRlxaSXeib = 3715.845; 
while(bRlxaSXeib == -6.514195E+24){
double YKUCLtwDqyPfxTWHjKFERaiFle = 1.957193E+12;
bRlxaSXeib = Math.Floor(YKUCLtwDqyPfxTWHjKFERaiFle);

}double tBlpWLDkYHDOQcdSa = 1.912066E-32; 
if(tBlpWLDkYHDOQcdSa == 4.06147E+32){
double RksNJDNBGadZKmOnMpG = 4.271396E-23;
tBlpWLDkYHDOQcdSa = Math.Floor(RksNJDNBGadZKmOnMpG);
object HUoyJLxQaaq;
HUoyJLxQaaq = 1.700706E+21;
Console.WriteLine(HUoyJLxQaaq.ToString().ToLower());
}byte Yttx =  103; 
uint FSztkSUfHgim = 402758; 
int SthuEVWBnSMcRyehK = 2; 
if(SthuEVWBnSMcRyehK == 649978){
SthuEVWBnSMcRyehK = 753659;
}float nzXHWPIwRBdnfPJVYftgSojcbDwu = 3.168356E-24F; 
byte nxpyYTqddHZWbTFlqJt =  137; 
ushort gZnVGutnxIpyZKzkXePocko = 16434; 

}public void hYi(){
string QJlCpqidnXK = "ksHEUUOfMoNeLVKbTzwkkgRH"; 
byte ugcTRRGlZXHymdw =  239; 
int VgRuIhjPCAGeBZqxB = 7328028; 
if(VgRuIhjPCAGeBZqxB == 925384){
VgRuIhjPCAGeBZqxB = 489156;
}uint MwwkqQnDpMfIZAUJbogHVxHgREO = 697796210; 
string HszWNOFjJqCknlPBhijyHtRboOouO = "wSlM"; 
uint CEhUxeyptl = 8934; 
string HKAH = "nCyHYAkxHWKY"; 
uint AYFbpaCVBJduwqLwLsGtjNe = 170616466; 
ushort JQuKmRgDe = 2357; 
long GRsaPEFd = 80121201624407921; 
ulong ISUOTfgYQEnPGuciZDRMptFqsW = 65897402016232260; 
ushort oSYmTQNbQefShhtGMaEonEwW = 33868; 
double WULZwZmThFFSBGECnQN = -1.261562E-23; 
while(WULZwZmThFFSBGECnQN == -2.738317E+26){
WULZwZmThFFSBGECnQN = Math.Pow(double.NegativeInfinity, 2);
 bool? hfOWxw = new bool?();
hfOWxw = true;
}short YYNxpmVVXihAzokMUoHZZTQBeQh = 4289; 
byte VyyIUYEsfhnkYXO =  209; 
int wSCCcoVfimqBeq = 46494405; 
while(wSCCcoVfimqBeq == 46494405){
wSCCcoVfimqBeq = wSCCcoVfimqBeq + 831545;
}ushort hDizLPVSN = 17895; 
byte htXtuhMOPqbGcWAAKdPLZiK =  250; 
ushort EgkpBcmsjsBVpTfHyR = 34118; 
int PowiJADNYJYZQF = 577254; 
while(PowiJADNYJYZQF == 577254){
PowiJADNYJYZQF = PowiJADNYJYZQF + 358250;
}long hZPOWGtLTzQiPRPhSELGkJ = 22925239716079204; 
int LcgxtUEW = 2639; 
if(LcgxtUEW == 185881){
LcgxtUEW = 642317;
}ulong PqYuIeCAyToHfpNTVGzMLtO = 72557745353292573; 
float NnHbiDGCIYBGWRdB = 2.329194E+08F; 
int dcwDbtT = 576433494; 
if(dcwDbtT == 144221){
dcwDbtT = 438082;
}sbyte bfRMcSjcwnqe =  116; 
sbyte XwhJxKTeZPKNXc =  11; 
short PzbweTC = -10846; 
float aoK = -1.713282E+31F; 
long JaZOLJ = 76916947786456823; 
float ioAgbSDGqDKSPpBOcQtDtudPStm = 4.448092E-26F; 
byte ahuCZwDgRzuyhZqlCxF =  10; 
ushort pDOkLoIFGyeWKKwOBtAK = 36165; 
string XWPChLAFjjtSSNOLnzlSXslpHROWl = "NdlEQoVOBTfTUMpSE"; 
string IlkplbGKAUpXEsIyoGnXAP = "YsMeyAaPmMjEpCNa"; 

}public void kFxTjWDBlnblodmTM(){
byte AXhkAedktOsWCGMMUsGPxiEkkYEXJ =  241; 
long oPBFduXPfLiMUsAGDs = 31686795903390423; 
double CfDxdOIMfhJAwRJM = -7.685621E+25; 
if(CfDxdOIMfhJAwRJM == 0.1843349){
double kyRnjkgwymahNJfKNBw = 1.330809E-08;
CfDxdOIMfhJAwRJM = Math.Ceiling(kyRnjkgwymahNJfKNBw);

}ulong ZHLeibyTJDoxJo = 59633338828667747; 
sbyte hLfmctuXnhMGgOhPbcoZPBVt =  39; 
short xpyRCzWXzVkQo = -22287; 
short BEoObId = -15818; 
int iZHPMFK = 957777775; 
if(iZHPMFK == 326850){
iZHPMFK = iZHPMFK + 192661;
}sbyte QJTQlaRy =  -36; 
ushort tFsOljntoWqJiHfokKqjthDkY = 13387; 
ushort isCnwWleIjxH = 4114; 
float nAZKjfLHObjykchPaNWjXwdnm = -3.812024E-38F; 
long nKg = 73815717100525585; 
uint OlkzacBuwUgHVTxMyPedGA = 684; 
double OxRBAKNlHRcPuYhHTQZVuwomdzh = -2.676774E-30; 
if(OxRBAKNlHRcPuYhHTQZVuwomdzh == 6.37528E-28){
double DUuxUJl = 4.705367E-32;
OxRBAKNlHRcPuYhHTQZVuwomdzh = Math.Floor(DUuxUJl);
int? gWYGCdZCsgEybDs = 765256;
gWYGCdZCsgEybDs += 98535;
}ushort bLjLEwdazhJtdEEWXZ = 28796; 
ushort CETINxfyTUHfBYQiwabYDTXKES = 43411; 
long eeDcJAgpcQZXEGnbmDFOOC = 50182789598527987; 
float wLOPedRzJTtjcaYnKIpodWY = 3.799842E-31F; 
sbyte sRBfYVkCSSzXjLgfUiPs =  -30; 
ushort WCOGLzMkKWxAH = 10211; 
ulong nSAfwlLfFWRdu = 26622005795327692; 
short dfIOaOkAXhOBBVmZ = 14117; 
float aOIJQtgiBjFikYqxZNyK = -1.445997E-10F; 
ushort WHPpsMySNCoNxySwTGKotZDDiz = 16990; 
byte gpIqNNjERQ =  202; 
ulong wJKFaDVbQNKgBXdmgTzkFH = 17525472419542728; 
ulong DIETSFzLplpEYjlPfpLXqCgPycj = 17645449115524980; 
float jwDuNVspIJkxV = 2.6583F; 
short EaBPwPwgUcbDGjGtxVViDEHPz = 20118; 
ushort VWMKygkEtFPGhmTSpOcAGdAHGioZu = 46882; 
short wxlXqseAZbZSgoaoLOz = 14214; 
ulong tSyeenRkiJnikLbVCYJEBEdHAph = 51060354793233586; 
ushort lnRIOSqEXdomQlAc = 13155; 
ushort OQGOfRWDwJqLJIupKRVMLNdC = 15193; 

}
} public class NzwNCIfmFLHAMRSpVUHsHFwWQl{public void JebNdDmdyh(){
long KGwcKJ = 82687356495931719; 
float hczXpsxhE = 60593.88F; 
double NuimNXMkCMmFfjO = 2.559232E-24; 
while(NuimNXMkCMmFfjO == -5.938779E+12){
NuimNXMkCMmFfjO = Math.Ceiling(Math.Cos(2));
Console.Write(NuimNXMkCMmFfjO.ToString());
}uint NczqEdEodZMpuaNk = 75; 
uint UbipychWKxuppNLE = 86575492; 
double NPutUSgPsgBjKbHMUiUCVHGHFxZ = 2.474114E-16; 
if(NPutUSgPsgBjKbHMUiUCVHGHFxZ == 5.084339E-15){
double HPxFWtE = Math.Log(1000, 10);
NPutUSgPsgBjKbHMUiUCVHGHFxZ = HPxFWtE;
Console.WriteLine(NPutUSgPsgBjKbHMUiUCVHGHFxZ.ToString());
}long QLBqBZsuDIWTa = 8156000525556412; 
uint GUF = 68532486; 
float cgcuIhYiIGRaEHF = -6.457875E-33F; 
ulong hIIeYEpLUxWkJDeFPQ = 25550956615563602; 
ulong tbIBbgJQwbeQsyeMMuiyzcPxyxZK = 40366301719331455; 
ulong iDUniOLRZguZbX = 36938412793529888; 
sbyte TCoabYflWplHDjTbMGySpXSPswVaH =  22; 
short wVwIzLPaaicMMfQnyj = -881; 
ushort oJVJBJH = 4074; 
sbyte sXIlHPlDaRxRhVgclF =  34; 
int fWxfmBZYbiXVEL = 8542361; 
if(fWxfmBZYbiXVEL == 489661){
fWxfmBZYbiXVEL += 227649;
}int GLzCaCyxbxDhmbl = 38; 
while(GLzCaCyxbxDhmbl == 38){
GLzCaCyxbxDhmbl += 986285;
}long VEjqYtwhqQHpLHM = 34066526848521230; 
uint TNPkZcAYmDYQkzdHnlqGMBA = 77396724; 
ulong uBXD = 34147974985716714; 
ulong qzdNKmc = 31267819956173377; 
byte ykZeyo =  254; 
float Htzas = 811586.3F; 
uint cwVeCaTZKwbTC = 34762336; 
uint gOcUidVGHKdBVmCw = 30334977; 
float sJZlyFEQk = -4.352493E+08F; 
sbyte SOQwBTqZzVpPAPiAz =  94; 
ushort mgyORxEAeuCdXYcA = 46832; 
string xBlUOJx = "QwqNliGoLpbEepRVeulSBMKPzMbZa"; 
byte fuzOeOHpVqKqiKxdgJqPu =  111; 
int dMCoAgHozHWjGCN = 46; 
while(dMCoAgHozHWjGCN == 46){
dMCoAgHozHWjGCN += 831836;
}ushort fbIVpJzoJPaLtlqQ = 50976; 
long TCNlyFWkqpfeG = 64932365678468051; 
string nwGRuUgnDPxC = "sQkpuzgZDLubhPdYDuPTjAQSNDpkb"; 

}public void ojhkeyIWWgTsLLkQlf(){
string auHIQTZsDRi = "XYwkUWLHqGzJZSOfIXqjhMEUDAw"; 
byte oteYXuNoZVowxYu =  236; 
double XeULGBYOkwCIwBzibmtLc = -1.508156E-37; 
double jYFQzgEEOFZiiqOsBOWmXgD = Math.IEEERemainder(3, 4);
XeULGBYOkwCIwBzibmtLc = jYFQzgEEOFZiiqOsBOWmXgD;
object ddDzjc;
ddDzjc = -4.190425E+09;float QcwqjXdBBkfPgLf = -3.276647E-17F; 
float FHAINCNltaOakqEIihLwfn = -3.271469E+34F; 
short WNDFSaIEmRqwIQxLLANjV = -2272; 
uint XcEAG = 5287; 
uint dGaJPhNtIdMpN = 914008; 
double DGkdPykAHPyhX = 5.998935E-15; 
DGkdPykAHPyhX = Math.Pow(2, 2.1);
try{
int EsHnkmCduMxmQHNhuzMocF = 76433;
if(EsHnkmCduMxmQHNhuzMocF == 3932){
EsHnkmCduMxmQHNhuzMocF++;
}else{
EsHnkmCduMxmQHNhuzMocF--;
Console.Write(EsHnkmCduMxmQHNhuzMocF.ToString());
}
}catch(Exception ex){

}string HJYBTnBIYRbAXHTKUaMtKDYjtcUka = "lqjBZnYsnuFwPmuZOzH"; 
float apKo = -1.914827E+15F; 
ulong RupIqG = 62855928642467092; 
short oOMPunwFeElbAlWEjkyZnNyI = 13173; 
long RbjCnXVZpYZ = 7312496787008371; 
ulong LbCpotBCp = 28277284489021469; 
double FlGF = -0.002339164; 
double SefZJ = -2.016956E+13;
FlGF = Math.Floor(SefZJ);
for( ; ;) {
Console.WriteLine(-3.625981E+35);
}string SzEmQZzkNGKpxpGtEWTyHeaDqu = "lxhtfRiMWFuXwgbmAN"; 
string eNFMLWGlOzbVw = "mPGmFznEJOjZzq"; 
int BPqZzzRwIilWYzEACYUmImiDg = 85774; 
if(BPqZzzRwIilWYzEACYUmImiDg == 719530){
BPqZzzRwIilWYzEACYUmImiDg = 470981;
}short VXlFxYOXjFwmsFkkKyAy = -8899; 
byte HOSXuaSWKW =  75; 
string qVfafoyINpPWUGTnZ = "nLKGQEbuXqIMpcCKTPszqdNGU"; 
uint aBwZ = 71423311; 
string sRM = "CZMsf"; 
double ZMu = -1.742696E+17; 
if(ZMu == 3.262895){
ZMu = Math.Sqrt(4);

}ulong ksEueaWzpySThtESxlPIiO = 39854474820746130; 
int KGxXfBKjHuephUTGoXqPkfDxn = 472576; 
while(KGxXfBKjHuephUTGoXqPkfDxn == 472576){
KGxXfBKjHuephUTGoXqPkfDxn = 541655;
}ushort aiDXGNVDGaPJOZbWUyYDRHEdZ = 45819; 
long uJyR = 88306538982693679; 
ushort GfNNAyytH = 48123; 
int ZuFzYVeThWEXaoSpFlONgFbzpl = 2139; 
if(ZuFzYVeThWEXaoSpFlONgFbzpl == 47154){
ZuFzYVeThWEXaoSpFlONgFbzpl += 718573;
}byte AOgSLOgDeJCQNjGgOwYROusf =  39; 
ulong TngUSKlQdjMCaHEBsXBAa = 59576449017743081; 
ushort LYd = 16429; 
short PKfBuCXcUXjjUaiHmKnPTmemkLJib = 3124; 

}public void uoCEmwYHdCkOPmRkuRdMuZAeCFSp(){
long OaqbQUsxVlocjnJzoauBosYRDw = 77522908476797951; 
ulong xOyWSXVR = 79467352368575118; 
double ExcBAue = 4.808446E-08; 
if(ExcBAue == 7.660852E+08){
ExcBAue = Math.Ceiling(Math.Asin(0.2));
try{

}catch(Exception ex){

}
}uint LOOYDZQSeYDwnmuskP = 27; 
short gMiX = 23241; 
uint FUEPUQzkXFnYJPztEF = 94; 
ulong sklmII = 72559972720148263; 
double uyWgmjMoKoIsGsNBPzC = -38277.75; 
while(uyWgmjMoKoIsGsNBPzC == -8.11802E+23){
uyWgmjMoKoIsGsNBPzC = Math.Pow(double.MinValue, double.MaxValue);
try{
int zWDsZKjl = 2719163;
if(zWDsZKjl == 98990){
zWDsZKjl++;
}else{
zWDsZKjl--;
}
}catch(Exception ex){

}
}short HheAhFtUHeQQzYOMOXnRNXyuVKOYT = -15951; 
short LLEeptpxiNwJ = -1874; 
short gxTVpMzkKKKEyQwa = -8326; 
ulong fNwKwwIRIaCPVH = 32400714301262060; 
uint uGPSHOMbTbjMLMzAJUQkhVPXni = 33788698; 
string QSmCoPGPkTnBWUxyWQCSoNxpM = "RSFzVKablsAjyJU"; 
float WWIZVBQlcBiDcwXQQYNjN = 3927644F; 
uint IdYNtSYucBtdAWqOlSjRAiWCuH = 13289072; 
long YshemxSzKIkhaVEMqbwxBfuIuzhx = 12758744245202008; 
double KVWOiCHsibYIp = 1.141511E+16; 
if(KVWOiCHsibYIp == -2.863233E+26){
double owfXAtyUtCQWsBANXTbcpfzL = Math.IEEERemainder(3, 4);
KVWOiCHsibYIp = owfXAtyUtCQWsBANXTbcpfzL;

}string KxtlxVYeM = "PgbybSckVIYOaXcyxEmKdZUbX"; 
double hToQGjnKBxMVDzOs = -1.732445E-18; 
hToQGjnKBxMVDzOs = Math.Ceiling(Math.Asin(0.2));
try{
Console.WriteLine(hToQGjnKBxMVDzOs.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}float iMmAzpVyRxGxmtm = 0.003550548F; 
ulong HSVYSIVVpWZKGqcMC = 18466183497660574; 
short xOwqqVGkGmNuRDuGCRNZfjILyNxI = 28967; 
float TDL = -8.114129E-30F; 
float JPagxzPOUSzdzctpRl = -8.510464E+14F; 
float AYnNlUSfdmKWhXZYOi = 3.791111E+12F; 
float iTuYBlUnakKepbTJmU = 4.066429E+21F; 
ulong uCVwqSzX = 61620313580364840; 
ushort lPmAldIXfeWbMdQXATd = 6184; 
float Qdy = -2.363105E-12F; 
ushort zpZBpDYYU = 18820; 
float uNtbMBJeRVAjoLkZicPLmLI = 2.86426E+33F; 
uint BRZFFlqCnxcLXK = 4091; 
int PlGqZQSBgBKfFekehnSpVYXjW = 130079; 
while(PlGqZQSBgBKfFekehnSpVYXjW == 130079){
PlGqZQSBgBKfFekehnSpVYXjW = PlGqZQSBgBKfFekehnSpVYXjW + 942070;
}short XhoQi = 22586; 

}public void NdeRSiR(){
double TCbDlWQnxwwJBKgE = -1.059285E-38; 
while(TCbDlWQnxwwJBKgE == 1.387344E+32){
TCbDlWQnxwwJBKgE = Math.Ceiling(Math.Cosh(5));
int[] yijbFFDGTJe = { 9446160 , 29723 } ;
Random heIKBKMHIIhGFVVkZisxWSUFCgmt= new Random();
Console.WriteLine(yijbFFDGTJe[heIKBKMHIIhGFVVkZisxWSUFCgmt.Next(0,2)]);
}float WwSpgNaD = 3.766558E-14F; 
int aEyJKsGLqJZpjchtysKtPYKi = 59; 
while(aEyJKsGLqJZpjchtysKtPYKi == 59){
aEyJKsGLqJZpjchtysKtPYKi += 802078;
}ulong meEYKEKGKhTnxDfeFHe = 50288972546581735; 
string sDBUucfzjK = "JeJIexNAAZJCLzYadCewWFqqFs"; 
ushort AZG = 26757; 
double HDEtXzCg = 4.26692E-17; 
HDEtXzCg = Math.Pow(2, 2.1);
 bool? MaAtmE = new bool?();
MaAtmE = true;long DPlqkwQgfV = 20313575175543488; 
uint hdMKAkfgDDLXWl = 6293; 
byte tkcJUfkelqER =  148; 
short lgzcCRZoJEEkACequwaZntzjH = -868; 
ushort PEMXwfKfQFKCtVueqVMDgaiwEpDTI = 18839; 
ushort waBf = 55002; 
int tSZBQMzGtZtSLNSSOuqueERy = 155295; 
if(tSZBQMzGtZtSLNSSOuqueERy == 362980){
tSZBQMzGtZtSLNSSOuqueERy += 389275;
}byte MetLhBtKe =  180; 
byte nyLoEiNMBkMQbCZIetmYLgfHuh =  175; 
byte Iot =  94; 
double eytDjHD = 15.46628; 
while(eytDjHD == 2.411512E+12){
double MzkihqeUwzALTWsxesU = Math.IEEERemainder(3, 4);
eytDjHD = MzkihqeUwzALTWsxesU;
try{
int qitFmQ = 5889923;
if(qitFmQ == 3267){
qitFmQ++;
}else{
qitFmQ--;
}
}catch(Exception ex){

}
}sbyte nHBTVwUaOMSdWwNuRt =  -19; 
short qgSnKqjqBakK = 26544; 
ulong QiIBn = 81863980240384958; 
ushort kGSl = 62236; 
uint GldLKnMHubHHFIMWaFef = 8756; 
uint OTz = 60708823; 
float RGhEwxEcYpUZjJOSuAJS = 5.852332E+27F; 
ushort MObWQQWbEsQ = 1378; 
uint pAyCf = 44525094; 
float tabdoWYaYl = 4.808731E+25F; 
long IlitSwRSNuqij = 31985798985967341; 
double NHeGsRPImJfPpWwFzzBmHNgN = 2.529837E+30; 
if(NHeGsRPImJfPpWwFzzBmHNgN == 8.709009E+31){
double yxNNQAaXNdBeGQRiotahJhZ = Math.IEEERemainder(3, 4);
NHeGsRPImJfPpWwFzzBmHNgN = yxNNQAaXNdBeGQRiotahJhZ;

}long Itm = 6615458188034221; 
long QbCdxxUWfBT = 87747505981291913; 
double WKcetLJ = 1.36971E-30; 
if(WKcetLJ == 7.232711E-25){
WKcetLJ = Math.Ceiling(Math.Sin(2));
Console.Write(WKcetLJ.ToString());
}sbyte obpFSCyzRtLOAyUQHcABRsYOkKgd =  105; 
byte aMOARRBlTujP =  187; 

}public void lbxykRADJ(){
sbyte cCthwtDHaKcLYcYfRGgHElCZIKHL =  -35; 
float cJDOohdzaHhMEjq = -1.036502E-10F; 
byte JWDzsRauBLNSmilZcwoKYO =  36; 
sbyte wBYPqkmAIjCAjjXoNENnPUEqjodfC =  65; 
float yzXqcGiBAsGYLBSkqwkjuSjB = 3.822571E+09F; 
int hhX = 272792630; 
if(hhX == 709513){
hhX += 252853;
}int humOxoFHK = 844667; 
if(humOxoFHK == 545166){
humOxoFHK += 715372;
}ushort oxtxagsswMLYyhYLid = 52403; 
sbyte NltZU =  24; 
short QqDqzxRmQhmUw = -31161; 
uint ImoUcXAVmBCjaxfDcYGwhIZLgy = 9181; 
uint JMKAqIpxq = 852122253; 
long XVHl = 43624214414297351; 
string ENGFEtaRP = "CezlGIMDBFbQAlZU"; 
float lpDNquMKMnonc = -2.347411E-30F; 
sbyte oDzlQKFlxlOmtGJCGGAXOO =  -107; 
uint WznlCVLsBLsshxHXCUhXxsJGMxCH = 92; 
uint XpTkqZUqdUTUqjy = 84; 
double TWEBFPIBe = -7.309993E-34; 
if(TWEBFPIBe == 1.160344E-24){
TWEBFPIBe = Math.Exp(2);
try{
Console.WriteLine(TWEBFPIBe.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}long SZNyMRjwpCL = 66946699972191892; 
ushort xHKkOjUXCjMsYfV = 53183; 
float lcFHDAZjZR = 7.78137E+15F; 
double pTxtcmXHpyPqYDMLzqlApqTPd = -6.230528E-23; 
while(pTxtcmXHpyPqYDMLzqlApqTPd == -5.789593E-35){
double bfJCNHqFNgSkBxzotGzADZZHoSb = -1.870343E+25;
pTxtcmXHpyPqYDMLzqlApqTPd = Math.Round(bfJCNHqFNgSkBxzotGzADZZHoSb);
try{
Console.WriteLine(pTxtcmXHpyPqYDMLzqlApqTPd.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}ushort TqR = 26839; 
byte xinnCEbFxNBAYKJajDecd =  197; 
byte mImuztBHUuQMqKHVJoIKutoMsT =  35; 
short jGJUubTpHfd = -17124; 
float EUWJBVitOgFAUsBbGlk = -4.380241E+14F; 
ulong SXIOFb = 43048167233337271; 
byte ugQSCpVhBQGBSbEVPNe =  183; 
float EsFChY = -5.059691E+10F; 
long uFRiUWGCcBmPxJMwWRV = 10273075543172788; 
string WaTgZfm = "wbShDJsU"; 
ulong WQMbpMFoVQEGxKGtfAtJxDzkCPc = 74061254063327087; 
byte ULhCLkxHQGiDjE =  220; 

}
} public class fAtqCBBJJBfulRTsw{public void kOlhMscELjUtFmDRIcXTWukASje(){
string YMQhnseoYNboX = "qOpWSSwiQuCZREMoCmajNydk"; 
short zCKoWFZGULDnmjZQWxHnCCDKlOn = 16804; 
string LeGXtTABzWalJbMmaQjWCIFctW = "ZdhplDaZzJoQfpwYKmoSCXYOzEfsd"; 
uint EyUAypGPJJbnFLZdHg = 89979332; 
ulong dybeu = 39733201682472350; 
ushort RXIdyHjB = 44504; 
ushort AAAl = 25705; 
int BuyysWOgkbepAaHTPibRsyT = 359298822; 
if(BuyysWOgkbepAaHTPibRsyT == 364673){
BuyysWOgkbepAaHTPibRsyT += 181205;
}double CtskZOtFMpqwiw = -8.564265E+28; 
if(CtskZOtFMpqwiw == 3.074367E+09){
CtskZOtFMpqwiw = Math.Ceiling(Math.Acos(0));
try{
int BDnsiFWhAmn = 2314455;
if(BDnsiFWhAmn == 63428){
BDnsiFWhAmn++;
}else{
BDnsiFWhAmn--;
}
}catch(Exception ex){

}
}sbyte iHiHHUhKFeaobbSeqBhKmigPZZ =  -83; 
short cBHcJeTVHPgIpC = 14172; 
short sXupbLAzGZAtHHLRmCF = -8480; 
byte WZiLMbcXIxyu =  163; 
byte gbPhFceuDahwQcZfYuZawhpZzxI =  89; 
string lZAWfBQlHpSGHLFGlUIpAaIkBZL = "JiwNpHOhWUWzpNNJiPCGwTjN"; 
long ylqnxgQaDzVfoaWxJsYjZMn = 11429954075931314; 
long DiCR = 36428522168501285; 
string BJYjSPBOLYsmLJMH = "BncNRwGZUiBLDZkXkXwxGg"; 
float PzEzxNgXJzOBbOKnt = -4.970651E+34F; 
int chYgoBMpkfuwBXsRgEhcEcelLWymh = 41; 
while(chYgoBMpkfuwBXsRgEhcEcelLWymh == 41){
chYgoBMpkfuwBXsRgEhcEcelLWymh += 741844;
}short qgluHzsteBQsymHQEwZ = -27925; 
ulong tdzihKkiYkblPy = 79992619603160147; 
int LQPsIgNmTVOiWzpIiVCGpkyjVWz = 337013; 
if(LQPsIgNmTVOiWzpIiVCGpkyjVWz == 937410){
LQPsIgNmTVOiWzpIiVCGpkyjVWz = LQPsIgNmTVOiWzpIiVCGpkyjVWz + 426511;
}int sVSBSzPtUJMMzne = 69518171; 
if(sVSBSzPtUJMMzne == 588335){
sVSBSzPtUJMMzne += 491228;
}short yMIqVYeRNsaYNpuKhCS = 3705; 
uint kagmjgc = 9818; 
double ouPkgUuQRYtzpstokUcIQfoCHkIPY = 8.398759E-12; 
while(ouPkgUuQRYtzpstokUcIQfoCHkIPY == -9.929761E+36){
ouPkgUuQRYtzpstokUcIQfoCHkIPY = Math.Ceiling(Math.Atan(-5));

}ulong PsVexSWHE = 7395350916769467; 
uint gObEzC = 645954; 
long ODMCH = 43624157239767752; 
uint ZUMGaSkWFJAOPxEXjlCQouYuKQ = 8547; 
uint aYCKAHNcKmDGgThUDwkciUn = 58283160; 
int sVkFtwRICmeWWjhDomJmflQmTIzG = 69; 
if(sVkFtwRICmeWWjhDomJmflQmTIzG == 907346){
sVkFtwRICmeWWjhDomJmflQmTIzG += 704241;
}ushort AIjjpk = 41101; 
long zoSasYyVqRffXLL = 63230528025019579; 

}public void bdHczXUGhPwsHFOMtkEToaCX(){
uint eIQlAxccZ = 3460; 
ushort uWBYiptpHPecOy = 22930; 
uint CKE = 8485; 
byte VoYpFGztYOjXOctHfXzTLjMZ =  180; 
uint NIRkCKLcTscMapMdZmwNXiczntteL = 976891; 
int HMLh = 653694; 
while(HMLh == 653694){
HMLh += 499940;
}short lzDZ = -24252; 
ulong pObFFtWipGsptzJtFaVyGh = 41506512399452171; 
sbyte RmooGgfPGAXyIjVFDf =  117; 
double ngiedWRX = 6.543579E-19; 
while(ngiedWRX == 1.399586E+18){
ngiedWRX = Math.Floor(-8.265735E-31);
try{

}catch(Exception ex){

}
}ushort GtuVZcjkmFoeUKo = 26379; 
ulong wpys = 25666139669814409; 
short sNTFAQPsagmAPsfoEuLVN = -28554; 
string pgPTVSzAn = "MgRLVWmcppfDzOaSOtb"; 
ushort oMGntbWyhJOkjGZyTzPIKNMhNIbT = 4009; 
string yTIyw = "xDUqGhHq"; 
short hauUUmNmCa = -2249; 
uint ZcudEpqDiFmxWUpeGeQwtT = 667542108; 
byte kgwVuNWR =  231; 
short ecbaMZxh = -2336; 
double xhWVIfsfjLTt = -1.057643E-23; 
if(xhWVIfsfjLTt == -7.939986E-25){
xhWVIfsfjLTt = Math.Pow(double.NegativeInfinity, 2);
int[] swnwTSLLlNjhhlAUoLwKHjVtpt = { 2743601 , 21773 } ;
Random EchjUOtBpYDGleODtpcCKqLyDl= new Random();
Console.WriteLine(swnwTSLLlNjhhlAUoLwKHjVtpt[EchjUOtBpYDGleODtpcCKqLyDl.Next(0,2)]);
}sbyte VfBZhJeEpuOMjVNe =  -84; 
int HDcnRjmqLHJAhsQdyfmXezPaJO = 126244420; 
if(HDcnRjmqLHJAhsQdyfmXezPaJO == 463899){
HDcnRjmqLHJAhsQdyfmXezPaJO = HDcnRjmqLHJAhsQdyfmXezPaJO + 48191;
}long CXSRJQUquWHU = 85173793004685811; 
ulong NcFdkgC = 10961404828489723; 
int ycOdStPhEMYuBgc = 349163; 
while(ycOdStPhEMYuBgc == 349163){
ycOdStPhEMYuBgc = ycOdStPhEMYuBgc + 591411;
}int PuNbePECFKBAEafUJQWIuEG = 49477520; 
if(PuNbePECFKBAEafUJQWIuEG == 468100){
PuNbePECFKBAEafUJQWIuEG += 664216;
}ushort TSoYKfsKsakt = 46266; 
double zDmogxJXulOeCqCeTiqI = 1.210766E-16; 
if(zDmogxJXulOeCqCeTiqI == -1.360594E+22){
zDmogxJXulOeCqCeTiqI = Math.Ceiling(Math.Sinh(-5));
Console.WriteLine(zDmogxJXulOeCqCeTiqI.ToString());
}ushort AGfFKLdGhpYbRsYUnOlsLpEqFq = 4637; 
short QXIA = 8426; 
short sgLFxisyPKbHDzFqXbhjbOat = 8610; 
ulong ssqDjZiCgdIWDYgDUp = 22410159507915326; 
sbyte FgsXKGZfyhRHtWzmofGlVkf =  -115; 
sbyte XhLGeMQwXZMKmWYaeNzQuJMtwsTX =  86; 

}public void boOMISuSdfQYZ(){
uint FTJmUftEtEowqkMTS = 561756; 
double WBogHmJtaxJmSQ = -1.096754E-25; 
WBogHmJtaxJmSQ = Math.Ceiling(Math.Cos(2));
int? ufnSKktgHiAul = 5992945;
ufnSKktgHiAul += 67226;ushort BccMmEleERNgjthaywKlsdqetwNne = 56308; 
uint UskzojyaumDkofEtZgsjZiGxMYtS = 972038993; 
double nPlRiKLnXJSfBfPnStnYBs = 1.388829E+17; 
nPlRiKLnXJSfBfPnStnYBs = Math.Ceiling(Math.Atan(-5));
try{
Console.WriteLine(nPlRiKLnXJSfBfPnStnYBs.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}sbyte ufiYDweJul =  89; 
sbyte MGNMkHa =  50; 
short QtbJVRUImyVLUnMzPRgg = 23894; 
uint lKMLLIbiEbztGCNjSAFFJmaL = 990370; 
ulong iElOnDQYifjaloqKxUgXcHoHo = 7932874047883447; 
byte aPKfcMPbMpEtuWqoGTPPCoGApwRog =  229; 
uint bppUM = 7926438; 
byte GDqLENcsM =  156; 
float uyMjj = 4.988401E+09F; 
float URRzqeGyDQPEDWMo = 5.254468E+18F; 
long WwoHZISFLDZyqNLMAllJxUjP = 63306006391139636; 
double EADjzK = -1.016345E-37; 
double ZPDBasckAD = -9.038161E-21;
EADjzK = ZPDBasckAD / 3;
try{

}catch(Exception ex){

}double TRbHZQAPXFWVbBRiOcFMnwDyIX = -1.318159E+13; 
double UmupjGd = 2.115652E-22;
TRbHZQAPXFWVbBRiOcFMnwDyIX = Math.Ceiling(UmupjGd);
try{
int mCQReSRkFzoFZbKQbocW = 6313847;
if(mCQReSRkFzoFZbKQbocW == 60903){
mCQReSRkFzoFZbKQbocW++;
}else{
mCQReSRkFzoFZbKQbocW--;
}
}catch(Exception ex){

}long oBPcoqVWCfUVL = 82858872134534315; 
short XeLsN = 11467; 
string pmYyUqgDTKdjzZbTENWpaBKx = "wRYFYinCpEJMJCNndloUgHYcROaN"; 
long QDDAABiXUCQIiwtgVkicXowC = 35476020499544028; 
float NfjjcmefqOeAyDKLT = 1.040079E+07F; 
int CgbJLImjKtpQHJmWMhVXRXHyq = 2896; 
while(CgbJLImjKtpQHJmWMhVXRXHyq == 2896){
CgbJLImjKtpQHJmWMhVXRXHyq += 336998;
}double KlYiBsFS = 5.706096E+10; 
while(KlYiBsFS == 4.990492E+12){
KlYiBsFS = Math.Ceiling(Math.Cosh(5));
try{

}catch(Exception ex){

}
}long YjUhshenOPkbRdmhbUmGIWInmR = 56896057787726595; 
byte TgZmYFKsPUboqXltJckb =  138; 
int CohLsFDCLQGoUVaIcmoztQEGHjS = 455783375; 
if(CohLsFDCLQGoUVaIcmoztQEGHjS == 104049){
CohLsFDCLQGoUVaIcmoztQEGHjS += 246873;
}byte XQUsOltpAMJqzJu =  43; 
ulong hCLMDehMF = 8796572929099241; 
float UClZAeU = 3.647276E+28F; 
ushort cMxSeKNlmQj = 2568; 
long BUlHIbGDaUL = 28420446565769008; 
short ehI = 28919; 
double QFtLAFKZjIqktKBMCgjhC = 2995369; 
QFtLAFKZjIqktKBMCgjhC = Math.Ceiling(Math.Cos(2));
Console.WriteLine(QFtLAFKZjIqktKBMCgjhC.ToString());
}public void qZMLSqEfYzLRJgIO(){
short fQOTzanszEiwtMQgJXuQfsw = -21230; 
short jMiXOb = 10477; 
short RMyBMxATaWD = -26818; 
double DyL = -0.1382845; 
while(DyL == 4.010194E-09){
double DjBPhNdxZukxJuGAwmNcjLGAd = 52203.35;
DyL = Math.Ceiling(DjBPhNdxZukxJuGAwmNcjLGAd);
int? ygUiVTpskPPaEbhmnK = 8437348;
ygUiVTpskPPaEbhmnK += 56651;
}string zFyUDmqmOBpgjyXbHKzmPZ = "ElzP"; 
string obNqApkEJAVVbVFNhPVUnZhfOW = "CTIeznszAEQLgMyX"; 
byte sOdzReJTjHpTHDDRJL =  4; 
ushort IJxfjqdPhQbXDx = 11181; 
ushort bEwG = 5110; 
ulong GWmhpcM = 44985816705234499; 
ulong ebQnWeguwGKyZREWmBn = 87360304411218779; 
ulong PLuQQ = 15597232256030515; 
float cARoghtStJKGXMh = 5.030475E+12F; 
ushort GUoHjqCasQx = 62677; 
sbyte IbHpgGWwTahfVekOUi =  56; 
uint DOSHYFhbbX = 54389046; 
string LCzCPtkanAd = "yPEVnlJpemOKia"; 
ulong SdxEUPAlxHWed = 6877330645638300; 
sbyte zTLkyNPsOFRikcBBIpsAH =  -50; 
int DOwgHTxVM = 9608; 
if(DOwgHTxVM == 574920){
DOwgHTxVM += 43252;
}uint ohAmtnqcbAcfHciozJJs = 73085959; 
double VyReQqd = -2.337545E+27; 
VyReQqd = Math.Floor(-6.953208E+37);
try{
int mlLxPmPKajzzinFh = 8401517;
if(mlLxPmPKajzzinFh == 87788){
mlLxPmPKajzzinFh++;
}else{
mlLxPmPKajzzinFh--;
Console.Write(mlLxPmPKajzzinFh.ToString());
}
}catch(Exception ex){

}double DqIOllxkTwwKBlEYXS = -3.56401E-19; 
DqIOllxkTwwKBlEYXS = Math.Ceiling(Math.Cos(2));
 bool? wOKQw = new bool?();
wOKQw = true;uint KyAThuBa = 753164; 
uint BDHcRqYAFuQnfYNhXxodiBpXoQJ = 72; 
string VIjOgWeGsNbGoIlNiHsdgzwNal = "ObIWOBBUGVkHkSRWqgUao"; 
string Bwtb = "ymyOUmqEKQnePOGRYZgkpCd"; 
int luLYhC = 4849; 
if(luLYhC == 352656){
luLYhC = 459967;
}ushort YCmlBSQlCykDwmQyVjSIISG = 39373; 
int UGbpntCliJ = 29; 
if(UGbpntCliJ == 310646){
UGbpntCliJ = UGbpntCliJ + 995842;
}float OFfhJpCiGP = -2.556752E-30F; 
double JlXjkkiRbidhuHHdqJZs = 2.205831E+35; 
if(JlXjkkiRbidhuHHdqJZs == 5.180618E+33){
JlXjkkiRbidhuHHdqJZs = Math.Pow(double.NegativeInfinity, 2);
object ckmAxddobauYapmlTqmUwl;
ckmAxddobauYapmlTqmUwl = -393133.6;
Console.WriteLine(ckmAxddobauYapmlTqmUwl.ToString().ToLower());
}float olDPqVpR = 1.054693E+24F; 
long BTaqejtfcIsEelbhpH = 54728461555950811; 
float FxecFJBamFYXFPiITZLeU = -21.51729F; 

}public void DSFhXXiXqJCGEpYxLbVMIqnjjnj(){
sbyte OQxViANetcoxbG =  103; 
byte eEantTmPmkFuXJOZB =  141; 
float LuQls = -4.162536E+35F; 
int HMsWIjGihuX = 12; 
if(HMsWIjGihuX == 162727){
HMsWIjGihuX += 688743;
}ushort BxBdbhSKNOWbU = 41825; 
uint AlyoAiRthWXEzCdIVFfT = 65; 
short QASultcCHoQWlgUsFhVfknll = 13431; 
byte oCHBRV =  170; 
uint pxgPUwXjLgxPLOnYNkFCnHKXmNZZO = 28403475; 
int UchaglPpbBESqFNsDdKe = 724748701; 
if(UchaglPpbBESqFNsDdKe == 182725){
UchaglPpbBESqFNsDdKe = UchaglPpbBESqFNsDdKe + 532988;
}sbyte XwMfThRMfYHZPl =  -69; 
sbyte fWilIl =  106; 
short KiswRFetsHPzZjN = -30499; 
byte xKoMAFHdGXjpjQOuyJ =  93; 
long cUxTGp = 40522884474262181; 
long YJOPNZg = 62891869287669908; 
long xuPTSUnZXTDYUDpgk = 983898169041954; 
float qXCaqCU = -9.401767E-31F; 
float gPBQPfFkhUI = 1.401765E-10F; 
byte dGcpc =  158; 
string WbwTkxaskoUskdgFXibJoJ = "hMlynhPPKedFO"; 
ushort uOgCyoAWT = 129; 
uint tTFswmIngYZNjwA = 25; 
sbyte skLOmDNleJQsyPKx =  106; 
sbyte JqfffwPusbQoJNBgmIfqQIb =  18; 
int GCXAZEeBzim = 58; 
while(GCXAZEeBzim == 58){
GCXAZEeBzim = 416944;
}byte thGXJyOQOW =  2; 
short BQPnxScljhnqThdMT = 10026; 
byte oliaEobkNqFyTLFPFjLEhciZq =  155; 
ulong LeUnOzloHOK = 34788633923022363; 
sbyte ohJWJJmdjJYALBJxpK =  -31; 
uint hQBDNpDpwBg = 75383137; 
ushort lseQZlyfMJeqRxngeKmgsitVsKNO = 10721; 
float YeMkXFEAeLLPUJZMTgubWAoFdY = -0.0001527803F; 
short KWUPgmekpCCjixH = -8234; 

}
} public class mtYTHOPWzPhhZwg{public void GbOUfCNI(){
long SBNpTXKAXepFcheSIyXdqguYE = 59690377467744654; 
ulong ojhWjzBzbUbYGQnUoKFBMfONzdkf = 64539182466714860; 
uint NFQJsdCxnJeRqGGok = 41; 
ushort fmulFVILtaGuOXZ = 45455; 
string LlbQDbwgtNXbHmecHsxxOiT = "UJAyDJGGhwLixpLtRyCVOs"; 
short oMpInHYDspWTC = 30445; 
int gsTlCWGqHcGpIYXDpfhRtY = 5282; 
while(gsTlCWGqHcGpIYXDpfhRtY == 5282){
gsTlCWGqHcGpIYXDpfhRtY += 120970;
}ushort RTUQOwzgRtfYalaJUusuExQMs = 10710; 
string PteAXQkNxRjD = "inPM"; 
ushort fBOnyOZnwcizGugqznGbkanldkQp = 26094; 
sbyte ZXk =  103; 
float fIM = 471.2141F; 
ulong msEhXRQBgtMflCjmXKTmb = 42744989476262158; 
float JjFZ = 3.886519E+09F; 
long EsASFzQGAVwP = 86103370943707963; 
long yUnocVIDzGoppVDqV = 65281632167014083; 
string XVUzaLcUFFowUYXyPAYt = "fNVlYYLaKiBcnLHEhoGm"; 
string MHBPMTawFQIZRiZlbMqUVwPtGmqT = "ktfGiaoMimSZnEKunTyF"; 
double TWgbpRhjz = 6.637608E-30; 
TWgbpRhjz = Math.Ceiling(Math.Tanh(0.1));
Console.ReadKey();long bfIRgxjF = 73594007264593391; 
long hMIazNnAaIQIB = 30405021863687301; 
ushort cyNUUGHRmDCLXCaIUEIxDOMPas = 34028; 
string wOsMSGDMTuTtRE = "YROLj"; 
long swZqnMXTCeFuROiZfWzVbiOcMkH = 36047570644485968; 
double CqROZSPPxNqPSJPMMUiHXF = -3.392684E-31; 
if(CqROZSPPxNqPSJPMMUiHXF == 4.122792E+33){
CqROZSPPxNqPSJPMMUiHXF = Math.Ceiling(Math.Atan(-5));
try{

}catch(Exception ex){

}
}float NwzjxdozMSRbANuIf = 2.378281E+11F; 
double WUmzqHUHhwTkceyiA = 193.5648; 
while(WUmzqHUHhwTkceyiA == 4.591642E+09){
WUmzqHUHhwTkceyiA = Math.Ceiling(Math.Sinh(-5));

}ulong qDDfCPwPQRkBQdNGuuaCSYc = 4962381797033766; 
ushort cbJJWMgDZXmUnlKMe = 12506; 
double nVtAmanRcEeEOYNbeXyJUY = 1.68821E-38; 
nVtAmanRcEeEOYNbeXyJUY = Math.Ceiling(Math.Tan(1));
try{
Console.WriteLine(nVtAmanRcEeEOYNbeXyJUY.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}uint kEAyhgpBDHZYPbSYf = 4726; 
int kOdXnwcGTpMHkdnQFkEBJLW = 479996; 
if(kOdXnwcGTpMHkdnQFkEBJLW == 211054){
kOdXnwcGTpMHkdnQFkEBJLW += 731351;
}uint yWP = 80; 
ushort TMnUJpmsFXEhBfVe = 4558; 
uint HzAiRUtnoKwdzwfqSlObGjgjo = 58065615; 

}public void VCRMBYPauDyapcYIiTfNwPdToylxs(){
short jpttNSKDAGeXoPOMElAbxCRFgz = 31191; 
ushort NtJe = 45230; 
byte WKlPApHgpRPdCMJkSnxJzP =  57; 
short nANmxSFDdQUaKCfUKnpKVVqehQ = 21029; 
byte lgyXVlQUUUYjCyWOkh =  188; 
short OCWFZsiIeGbsYRJxdPpoaQtq = -398; 
ushort wpXIeWPzFCsWTIl = 52170; 
short SDGmshtxbOXiFfGUOsgYXwjtdUMT = -4513; 
long aewIQRhYQoIUaBxXlpYTTJpY = 23198321919016648; 
ushort XdjyQdHupJpCiTHXADyOVsPxkyFXC = 25876; 
ulong BcDtOOBVmAsHJVjtDAWX = 26028320508533645; 
uint SWUIagpmjLcCCwahNpmku = 45; 
float VYYfa = 2.674717E-28F; 
ushort HxNaUddpolVadIFgaW = 7569; 
byte RzbDfhshji =  226; 
string OFwMxiheMlXeSquZZHbjnQYLzPVMh = "daWeT"; 
double nAXfkeleznRXijDSUKXHKn = 2.547659E-19; 
while(nAXfkeleznRXijDSUKXHKn == 2.957591E+10){
nAXfkeleznRXijDSUKXHKn = Math.Floor(-1.783881E+33);
try{
int JnyeioBuEVywNQHlUdmgj = 824577;
if(JnyeioBuEVywNQHlUdmgj == 45170){
JnyeioBuEVywNQHlUdmgj++;
}else{
JnyeioBuEVywNQHlUdmgj--;
}
}catch(Exception ex){

}
}sbyte lOKzXNLAaqMyu =  -84; 
sbyte DJMgquRWnGDfAyTnBtUFRCoZEV =  -24; 
uint jdCTRIxMZdSXNkqjGa = 465072; 
ushort anDbatMDwYstdDamIPyDCpcIdpYP = 63029; 
float agmcnpQIcZxITGuhPjttNbUwLk = 6.65243E+34F; 
long WFBfqmRANWShu = 60476031476482058; 
int DfZlSqcwSXIdRgNngqo = 434841; 
if(DfZlSqcwSXIdRgNngqo == 634626){
DfZlSqcwSXIdRgNngqo = 347916;
}short bSnC = -18168; 
short YasbZhLBZkNAnCyPDSUiibBsTCnbE = 5214; 
ushort NRiNWfqgMWCuUS = 3040; 
uint dMKqhRXAZhyUJXwPpN = 361570; 
long xBOylAgyaANyASMeGakmWXUmcZ = 20194035092527550; 
short EKFytCh = 30824; 
sbyte yDdkhxhyW =  -6; 
int umtCUVTEEoDDCVwN = 395578122; 
if(umtCUVTEEoDDCVwN == 109980){
umtCUVTEEoDDCVwN = umtCUVTEEoDDCVwN + 830220;
}uint CsYjbxnQDcpqeRImfE = 15979676; 
string KlauECJLwDMQaCWJ = "haMaAFeJcNEQCitDa"; 
long jHSNhSHOGnQgpxbBhnAjIW = 88631144513942126; 

}public void ZSjAVonFi(){
float zulHntBJloBzemUqXcXOunaRn = -3.189365E+34F; 
sbyte UJqsQyso =  -86; 
float tLgYztflbXpCkeykt = -2.588987E-12F; 
sbyte uysGjgnPDcPlxw =  41; 
sbyte sUaM =  -106; 
byte hgmtmYTQKeYnOsomRx =  122; 
float pPKPcqdzgKpjOzdTV = 185411.6F; 
byte CqxutKcIipGtbRqEEjHQqDwJdaTlt =  234; 
float LDOymuKHaRXuKRNHMoqsXtHZWNq = -1.299582E+24F; 
float mxLIAdUqjB = -1.483248E+23F; 
uint aYmmmBdFCXqVPCAdqQi = 696159; 
double inESYhSWEguWPdfEqkJUNpEosRgik = 8.17803E+34; 
inESYhSWEguWPdfEqkJUNpEosRgik = Math.Ceiling(Math.Tanh(0.1));
int? XiAUPZfUJfljMDOUNggXYOG = 7311383;
XiAUPZfUJfljMDOUNggXYOG += 1531;float OjDGXcNQABhQixNhZckfSH = 7.961828E-11F; 
string mVtc = "XCpCHeoNTmpSKe"; 
double dEPtJQPTgFiCVQSxydknSV = -2.979627E-34; 
double ofqisFJNgNywbVOSdwTwHTLTiyM = 1413.354;
dEPtJQPTgFiCVQSxydknSV = ofqisFJNgNywbVOSdwTwHTLTiyM / 3;
Console.ReadLine();uint gIF = 784382279; 
double MmyNgEjSBHMYG = -3.795025E+35; 
if(MmyNgEjSBHMYG == 5.917853E-18){
MmyNgEjSBHMYG = Math.Ceiling(Math.Asin(0.2));
Console.WriteLine(MmyNgEjSBHMYG.ToString());
}short BZyTQUVKnRysUHcxniSxNMipCsS = 15785; 
string VjEILOnjeYjaTyZAFez = "ieCTiOQhsybiHIVnPm"; 
double hedTmMSMpc = 6.504006E+20; 
if(hedTmMSMpc == 2.304146E-05){
double WYgSIWbukCMLqhkw = Math.IEEERemainder(3, 4);
hedTmMSMpc = WYgSIWbukCMLqhkw;
int? xVdRpJmUfkeGeOZoamxX = 8128907;
xVdRpJmUfkeGeOZoamxX += 99712;
}string bUpC = "KgnayfGEiVwRFLuenTlYlHRaQnGaL"; 
double KLymfdGNzMSyhK = 1.170723E-22; 
if(KLymfdGNzMSyhK == 2.365234E+35){
KLymfdGNzMSyhK = Math.Pow(2, 2.1);
Console.ReadKey();
}short FXYgimfZfeYwmzkmCPfuXcqOXB = 29228; 
ulong ApQssl = 43443770962279217; 
double JEWNRtBlYVDGjiIp = -1.713791E+27; 
while(JEWNRtBlYVDGjiIp == 6.876259E-27){
double QdZmBLCwooXac = -1.421585E+32;
JEWNRtBlYVDGjiIp = Math.Round(QdZmBLCwooXac ,1,MidpointRounding.ToEven);
object txKmmnhDDuUhqGIKwbp;
txKmmnhDDuUhqGIKwbp = -3.209349E-31;
Console.WriteLine(txKmmnhDDuUhqGIKwbp.ToString().ToLower());
}float XqHnXJpqInPnGVmJcsuym = -1.115315E+24F; 
ushort OWigpzB = 44279; 
short SuVYkuYHoMKYOlUb = -19634; 
ushort xKBmeiNhjqPoG = 20269; 
ushort ngujcoHCIAPhW = 15828; 
byte fnSRqhRPym =  197; 
short yeJtSHUQlthcZGGu = 6341; 
byte dBiqyEcgpGMPBsakugOdGtzUVLOs =  134; 
byte TbRlZnNkQPAEk =  127; 
int LYwknQhqKcowMQLFfkS = 69218960; 
while(LYwknQhqKcowMQLFfkS == 69218960){
LYwknQhqKcowMQLFfkS += 638556;
}
}public void YngPmusedtgZgcUACXYsAaeUzaH(){
ulong FXzEtSTkUPUdIMqzfdQTkFdYoxYj = 86872603406550578; 
long OkbnHhdnRnNwsW = 38296088703536189; 
float ZEDhDAnWRKpGRWXWUbFLx = 8.281698E+35F; 
short ExzCghRqsocFWLsaRFJGkfQcXHms = -22281; 
byte GmkZdPw =  176; 
int oAHUqXqlLFlpKLyG = 565851; 
if(oAHUqXqlLFlpKLyG == 470649){
oAHUqXqlLFlpKLyG = oAHUqXqlLFlpKLyG + 171950;
}string ysLMnk = "UfgWHihHKVgyJX"; 
short BCytRjIafwByZlFXB = -21486; 
short EPRynWRZ = -9834; 
long ihQkjRRFLzfRkI = 87440963568492896; 
sbyte LKafUsJJuZsoibugJFHMyYHPDouJj =  68; 
double cQABQMKsQyxlpxzwVb = -4.450221E+30; 
double GYZzGcOltCUpWUoWZC = 1.57181E-33;
cQABQMKsQyxlpxzwVb = Math.Round(GYZzGcOltCUpWUoWZC);
ulong mqg = 79895856512855289; 
ulong gbLkXVLeMxhbUmNBNTNotEIKisK = 57914225776254392; 
uint zPBdWBUYZpoqoLSa = 549293; 
int bcFlKaixEReWm = 29; 
while(bcFlKaixEReWm == 29){
bcFlKaixEReWm += 432871;
}string LibuKPRiFJmNgzwjntWszwh = "seoxONpfB"; 
int UERZkqmZiSi = 53; 
if(UERZkqmZiSi == 965973){
UERZkqmZiSi = UERZkqmZiSi + 840382;
}uint opaQcFBwuRsgmAhBHyuLa = 4263; 
double iYNDUPzGQqaTO = 1.691571E-08; 
double HttmCyQKeTbxTHNTBbUBxhhISJZZ = Math.IEEERemainder(3, 4);
iYNDUPzGQqaTO = HttmCyQKeTbxTHNTBbUBxhhISJZZ;
try{
Console.WriteLine(iYNDUPzGQqaTO.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}sbyte kBzlMwSzRGNGTPHntKTOUMpVZPBN =  -77; 
string Zhl = "MUhylwCYBACYDn"; 
uint tzscAjDlWUfnQFOzHsU = 2232; 
byte ssNQxzRYALwIBPjXDqVC =  90; 
short iWMdqktft = -23393; 
float KMt = -2.248742E+20F; 
sbyte GEuWmKyuscpLocfFULuAOfFcBJO =  -4; 
byte khxxKJO =  254; 
uint TQbcdyDXFhAwt = 15117090; 
uint swFVYiiSinStxQUlaGXeDYdfRUdJP = 83203363; 
short KxFUAVH = 22968; 
byte iLFisGjtZjwytszddfAcuxDjMtp =  84; 
uint qmqdUulMLdGwSeOmdlxwSC = 1555; 
uint hwxYpSSnTifoydSsuxyYFz = 484919; 
short VfTP = 25194; 

}public void DhlRoNO(){
ulong EkFgPOsEAgWLChKZXJ = 47305785473544028; 
short acIJKxamQsBoECtJXzaZDRqdC = 19697; 
ulong hpkczBD = 8533330196918467; 
int Xukcwqz = 30408264; 
while(Xukcwqz == 30408264){
Xukcwqz += 583225;
}sbyte GTfBgzRcYViKyVTfIMVFwFeMYNq =  -86; 
byte MVggAnqJzKfqm =  34; 
byte aQxjCZJDzbOtCuhISFjn =  141; 
short FOZmkzeqYAqMB = 15025; 
string NlEJzAYheDzAIXYeLwSwCMzeuK = "TjIHWQMxcZIWeQRiJZGKaCla"; 
sbyte HqbOznOKeXyWCJVBVfdzNFKDPe =  69; 
string qYHBbjwwoBtiwLUMNzWOBHfX = "hbHzYuAOUNZQwBABeHGK"; 
short UQRcHRmoLSJT = 25699; 
sbyte XqgTuBXVVqXymwZBnqQhRHVN =  40; 
uint cggCGqMhpARWRWVVLJu = 886130534; 
long wDeaYpwtkTV = 29552931533855947; 
long xwZ = 12215868310516564; 
short LYCpOSVfGcWaggUqmyHboYhe = 19016; 
string jpWqjFYdRRQOwOWRsc = "PFpNaeeo"; 
uint RxCMnLQJMNxiddByKlmthP = 58; 
byte UDVmeoyqZuobhdghGpOGBlmEuTchF =  10; 
ushort znRmOAIhLNdfiIdjlZiMQHzIpsSl = 15708; 
short XasETmUugJHVOigwMPSVFT = 10749; 
sbyte PiWwHwVwagfQekXsGcciMLAjW =  -98; 
ushort uheskLedXYAmL = 31903; 
ulong bFH = 61106537490773763; 
sbyte NzBYCaTcNGugTuHEPzmXy =  -89; 
short SzSinMmFZtkAYojz = 25899; 
double YmzchsmQAT = -9.459245E-21; 
while(YmzchsmQAT == -4.415246E-36){
YmzchsmQAT = Math.Floor(-1.184742E-33);
 bool? gGtWigojlmGzqDYkIolffPLOQ = new bool?();
gGtWigojlmGzqDYkIolffPLOQ = true;
}int GMsVFfyzQqzgaubiu = 989134; 
if(GMsVFfyzQqzgaubiu == 626730){
GMsVFfyzQqzgaubiu += 254863;
}int acIO = 25287624; 
while(acIO == 25287624){
acIO = 459012;
}int JjD = 680777706; 
if(JjD == 762234){
JjD = 513505;
}ulong bKqjtJzZzQSF = 17334668942196131; 
double iMmybuXMPDnKHdx = -1.659E-37; 
double AgZIyKTRxECGXWHdbNa = -8.7087E-23;
iMmybuXMPDnKHdx = AgZIyKTRxECGXWHdbNa * 2;
try{

}catch(Exception ex){

}uint jEKoYLJMzVHUeedNVYHNbbOwtSXc = 8390; 
uint VpTYhQfzULToTuaTooC = 41574458; 

}
} public class WhQnWgOGzHxaYPlfGeSiTpi{public void DZReOYpRkguMiTU(){
double nmmSSwlIjKyQmVHZqMXoK = 1.424139; 
if(nmmSSwlIjKyQmVHZqMXoK == 1.312793E+23){
nmmSSwlIjKyQmVHZqMXoK = Math.Ceiling(Math.Sin(2));
for( ; ;) {
Console.WriteLine(-1.209679E-35);
}
}int jWkIf = 56; 
while(jWkIf == 56){
jWkIf = jWkIf + 520334;
}ushort jXktgtYOofldsutWXXUS = 33518; 
long wgazKhLcQDLycDWjXIoVQ = 35881826940265814; 
int hDmPmgVnHFcxCIXpuaN = 201026049; 
while(hDmPmgVnHFcxCIXpuaN == 201026049){
hDmPmgVnHFcxCIXpuaN = hDmPmgVnHFcxCIXpuaN + 438387;
}uint NAdUTiXEV = 75712387; 
long ctJq = 31957147665585085; 
ushort uWQtsLThPDKimEecBHqLU = 54225; 
uint gUAhZFueyADHew = 987340; 
sbyte FgHFpzaLudzLPEuumzkMhjiFLOa =  -11; 
double lqCZl = -3.462429E+21; 
while(lqCZl == 1.575831E-38){
double GMKZlLiWOyOu = -1.619868E-16;
lqCZl = Math.Round(GMKZlLiWOyOu);
try{
int yDNoSgjQqZFLUjFSYS = 8544780;
if(yDNoSgjQqZFLUjFSYS == 91316){
yDNoSgjQqZFLUjFSYS++;
}else{
yDNoSgjQqZFLUjFSYS--;
}
}catch(Exception ex){

}
}ushort cUzxpB = 22431; 
int fGADRcSIapctBxjtZf = 236447; 
if(fGADRcSIapctBxjtZf == 302804){
fGADRcSIapctBxjtZf += 430889;
}short PEgnbFVUyniKowkXSGtND = -2980; 
byte pwSq =  153; 
float clgmnBNoyyG = 1.26209E+33F; 
byte QebbuLdDhqo =  20; 
double NFoEYfyhhWxepVpXzsS = -8270427; 
double YVYxQJLTpxYeTIWeGouhmk = -701504.5;
YVYxQJLTpxYeTIWeGouhmk = Math.Sqrt(3);
NFoEYfyhhWxepVpXzsS = YVYxQJLTpxYeTIWeGouhmk;
Console.WriteLine(NFoEYfyhhWxepVpXzsS.ToString());string YQFRFAPeiePThoqxjXQwZyU = "KOPc"; 
uint eqjzTH = 23; 
ushort dDEzPAyJOcBiQFNBWCgQL = 49275; 
int ayJK = 699597; 
while(ayJK == 699597){
ayJK = ayJK + 103889;
}short UwnkqgnwGVCGLt = 13542; 
long HZqWB = 48948027231789102; 
long mqGBmVzFAlyfWdZcZFTUfa = 42999295824149421; 
int nQsu = 20152878; 
while(nQsu == 20152878){
nQsu += 415685;
}float dsxNOjdkKNGmQlCqCULoH = -1.766334E+25F; 
sbyte FpPkKMKiQoGBHQKfwtBkfE =  3; 
long IoLD = 4714509021681861; 
double aVd = 1.033813E+34; 
aVd = Math.Floor(-8.109289E-38);
 bool? fjEHQRwtApcEEMlNkKuS = new bool?();
fjEHQRwtApcEEMlNkKuS = true;uint kuVnlolUWVNfzLJ = 383041; 
double FplVFyOAcbbtS = 1.512149E+16; 
FplVFyOAcbbtS = Math.Ceiling(Math.Tan(1));
int[] ZzqRzPiS = { 6138914 , 89086 } ;
Random DVjORVGJGMsXQaKmQZtONzIXFUo= new Random();
Console.WriteLine(ZzqRzPiS[DVjORVGJGMsXQaKmQZtONzIXFUo.Next(0,2)]);float cBBtZajJOIGozGiizHEIQQehd = -6.444038E-21F; 
sbyte GZONToxpILlHpGfhjiswMRZpkcmx =  -38; 
short OaNiJRDFqNSRVsbAjYaUeU = 17701; 

}public void PazCRbZX(){
short zGEyyqIEaJTadYjDdzswHTmDtIsbO = -13278; 
ulong AWTwVNBJmCTfqOWAqnYCGWtFsbd = 7370497201635847; 
sbyte PIpXJ =  -85; 
sbyte hkFLSKAqkpDDncTKLKHMhjtmPuDHI =  19; 
double nFkRzefWpuLJYFRMOHlYDtS = 1.12747E+27; 
while(nFkRzefWpuLJYFRMOHlYDtS == 0.001643621){
nFkRzefWpuLJYFRMOHlYDtS = Math.Ceiling(Math.Tanh(0.1));
Console.WriteLine(nFkRzefWpuLJYFRMOHlYDtS.ToString());
}ushort eUxWgFMDtYKPQ = 4664; 
ushort xyaOTHWkpIqhzVZZcNYRyuU = 6346; 
int jwmPmaaiABcICzXlHzmEezYKMd = 61; 
while(jwmPmaaiABcICzXlHzmEezYKMd == 61){
jwmPmaaiABcICzXlHzmEezYKMd += 921872;
}byte xCYCwqhowAXwkAUlOxSZOMxDcjVuS =  116; 
double xDbxne = -2.877745E+32; 
while(xDbxne == 2.894288E-35){
double GMenhdlkLewzkwOyXDQZJzzXJt = -5.414756E+16;
xDbxne = Math.Ceiling(GMenhdlkLewzkwOyXDQZJzzXJt);
int[] XQMqoJNjPdkqpqTHSBIte = { 5267780 , 42359 } ;
Random WsjhngFOhIlbhFNUgeKONNOqhgec= new Random();
Console.WriteLine(XQMqoJNjPdkqpqTHSBIte[WsjhngFOhIlbhFNUgeKONNOqhgec.Next(0,2)]);
}double FZUuhguHFbWDeninTYt = 8.732026E+12; 
if(FZUuhguHFbWDeninTYt == -4.768416E-25){
FZUuhguHFbWDeninTYt = Math.Pow(double.MinValue, double.MaxValue);
 bool? KcjLRkmTXQiEcsWnOLQpgmEcOkRzR = new bool?();
KcjLRkmTXQiEcsWnOLQpgmEcOkRzR = true;
}uint sFhjltKJuKju = 544337291; 
ushort xAgXHuPcXlJqJApgoszLZdqJeC = 28813; 
ulong GFYQBxgaDMXQemaLsaU = 3563351200130887; 
sbyte SeXawqIKChdjgMG =  3; 
float RqXNCDwEBXUeHcZCKIaQKWsaQT = 5.422149E-25F; 
double bEQZCQBfEwFSQ = 6.264214E+22; 
while(bEQZCQBfEwFSQ == 7.668774E-40){
bEQZCQBfEwFSQ = Math.Pow(2, 2.1);
try{

}catch(Exception ex){

}
}uint iVSFxSNlzqXPUh = 80; 
ushort ARHFPfYTWjDcFJebMacOsAZpS = 36769; 
short WMhOwdSHPFfMHEW = 3455; 
long NoLSwfOIzaSpMl = 57401923171963224; 
float mgVphPDxHyQaOdp = -1.537806E+38F; 
ulong SPzTUVgHMCUOlzmFokz = 60901657887563331; 
ulong UZPTWYJOmbubiQVMJFDBzaSCnV = 89560434577854369; 
float fVoAFRbiezBHAXGRm = -3.99378E-09F; 
float nGaFnY = 39974.13F; 
short wZtHOodVIZJyPuNUSaHS = -22859; 
short uSonVpSuGQReIfpRDjpNUzQQHRug = 9896; 
double AcbZPzahGPFdHX = -1.596153E-31; 
double cWbzIeWMMJtsbHgCsDcL = -2.340736E+11;
AcbZPzahGPFdHX = cWbzIeWMMJtsbHgCsDcL * 2;
Console.ReadKey();int JNDSHq = 3386; 
while(JNDSHq == 3386){
JNDSHq += 590915;
}long IeOtjYcUxZVldmXEJqi = 46139993139832211; 
short GGJWJREmyqHePqDWumeuFFzwiQx = 5532; 
float imLYQumpuxupkaDyZUXYZOHWqbhnO = 2.080281E-08F; 
double ziutQBhKdTJaot = -1.75475E+27; 
double HPIPmZDDbuUNSid = 1.013191E+28;
HPIPmZDDbuUNSid = Math.Sqrt(3);
ziutQBhKdTJaot = HPIPmZDDbuUNSid;
object VVXWbJKucbTs;
VVXWbJKucbTs = -4.749473E-09;
Console.WriteLine(VVXWbJKucbTs.ToString().ToLower());int AKSGf = 589581; 
while(AKSGf == 589581){
AKSGf = 47242;
}
}public void oewAbJVEuFop(){
long dKwt = 83061087377552165; 
int gbPAEbqCphH = 922109; 
while(gbPAEbqCphH == 922109){
gbPAEbqCphH = gbPAEbqCphH + 382378;
}int dAVehxBwVRqNAcEytQQMGLDtmNmGW = 35656783; 
if(dAVehxBwVRqNAcEytQQMGLDtmNmGW == 559002){
dAVehxBwVRqNAcEytQQMGLDtmNmGW += 546508;
}float WHdcHxLY = -3.971661E-29F; 
ushort EqeEQtsXCSwi = 59882; 
uint zHFWBMlx = 74; 
long oRwRGRmstnftjkUKdlUghklqQkVGs = 13167484799272936; 
string sJgSjBkNpSDsYTDQdFdawWHa = "tomOhNDcyDzbtcQ"; 
int GOQCXYEFZCdZtwb = 37; 
if(GOQCXYEFZCdZtwb == 977214){
GOQCXYEFZCdZtwb += 975707;
}sbyte ZpBpe =  75; 
long mwXpTkWxqJjilAcdg = 76595384897224534; 
long lxIzPoYRFxfhVxwUoPZTBmyj = 64046511954120774; 
byte KaKkuTmIzHaoZ =  144; 
ulong qKyQpZYyRUGHiPYlYBMwk = 33807433650290695; 
short fDXzOwueLbltAnHAKIPqzRuzlos = -19231; 
ushort hoAYAekp = 13887; 
float xbtZMVRYteiubyohAm = -7.475916E-35F; 
sbyte BUXTBPDntyXNbQbAAuBl =  -106; 
sbyte UyCjXMdMOwPCAcSucxfLLyowAfZNe =  15; 
double tUcwZGFCoLjqmoDxH = 0.001912952; 
tUcwZGFCoLjqmoDxH = Math.Floor(-5.569049E-05);
for( ; ;) {
Console.WriteLine(-1.712372E-27);
}string yWmdhdHfWBdjhWswbWuBGNCU = "sWYdZdzTGEqWgQxHdo"; 
string fCn = "YiGhEjKwtBPhIcWyFpC"; 
ushort zUf = 63284; 
float yBTzRIsfRzqpxTVEH = -6.668853E-18F; 
double tEx = 2.492327E+37; 
tEx = Math.Floor(-0.4476405);
Console.ReadLine();ulong oUXtobCDIPHy = 22338901389172425; 
int uDB = 7660; 
while(uDB == 7660){
uDB = 594333;
}long ENAjsloazAGLy = 13946057611936403; 
ulong BwTVnjickV = 62928845002553385; 
long CORdu = 17591215239991679; 
uint OAqaRoxwQtABNwhihtLMMEPCopWpK = 762829741; 
ulong LXkEzUPetbKUzgRFsWXcOnkK = 74066327004641800; 
ulong bzWGJAWIsCfIQieMsURKay = 39071453133951481; 
string gNetI = "nyggVJFhVtnwtfPdLFMwRW"; 
sbyte JKAbBddLdGHtDUCc =  111; 

}public void SZiEnWocbBImVqqIykj(){
int RoDqOzxCQdJzjehpfxDN = 44960; 
if(RoDqOzxCQdJzjehpfxDN == 829301){
RoDqOzxCQdJzjehpfxDN += 686610;
}short Vyp = 23791; 
ushort aLHlBySBkxYOMmjqqPHQiPLzBejuH = 23361; 
ulong ejdyKUWeUXRgedYbSEgBlERkZ = 39453791782601602; 
sbyte CjNIxTCWkYFtygiDclmFB =  -35; 
float CiBRxdYyXtxSfVK = 3.177453E+14F; 
long muOnoeVRHxEihqogntL = 28515989003812071; 
ushort DGUSNG = 4006; 
ulong kMBlqaukoBQPxnZhzmnskGOzKeR = 87207894008003694; 
double VJfcTwQHsgqVLntPwKGYcDljLUTQF = -3.642585E-36; 
if(VJfcTwQHsgqVLntPwKGYcDljLUTQF == -1.236222E-26){
VJfcTwQHsgqVLntPwKGYcDljLUTQF = Math.Pow(5, 2);
object WtxWs;
WtxWs = 5.03005E+14;
}byte tJYPuUoUcyDPRshaLhimwLRCTyLH =  219; 
short IxiYMbxRtubknVuI = -32655; 
ushort BcZ = 63864; 
uint SyWWiJbMWVsIuxYoqPTt = 554474; 
int QLkSRzIyHpTFBUkM = 48; 
while(QLkSRzIyHpTFBUkM == 48){
QLkSRzIyHpTFBUkM += 6871;
}int ckMkfiioRxgyyjVzmVadDuPxFhtal = 463812; 
if(ckMkfiioRxgyyjVzmVadDuPxFhtal == 217460){
ckMkfiioRxgyyjVzmVadDuPxFhtal += 808570;
}int yqyhENdNNnRFSOfZDxIwoG = 809898520; 
while(yqyhENdNNnRFSOfZDxIwoG == 809898520){
yqyhENdNNnRFSOfZDxIwoG += 970336;
}short ltnMPEEWYHMJ = 22329; 
float xtRsQEOhkyRtIZBBe = -5.957448E-10F; 
string pfYDxGybKHOenMK = "HCPyldoXkVXzgH"; 
byte SegmTPtXqgBQdnt =  65; 
ulong zUFEl = 70175817793990068; 
string qmiGxDRhSgeDA = "WCKIbhtTnTUxSyToqlyxMakItDI"; 
ulong xgKsfwYYqTVbyYpLaLtoDQT = 75589175688753861; 
uint fRNkeMcUpatDwnbVWtSo = 200434; 
int FoYzIWcuuu = 1409; 
if(FoYzIWcuuu == 927304){
FoYzIWcuuu = 974673;
}short lPBDITUxfWHlj = 27188; 
byte HPOFluYYhaWUswx =  221; 
double YRO = -1.251732E+11; 
if(YRO == 7.208097E-23){
double kxWeUgkMi = Math.Log(1000, 10);
YRO = kxWeUgkMi;
object IXZeKAOmhXtLEh;
IXZeKAOmhXtLEh = 5.394509E+25;
}long VcADtpZCMhpZpuRxbRhjfaoRuzVhD = 22268317893366909; 
ushort lpgBMRaWniLDoZyUVNJiVuSZqm = 59211; 
ulong pdwHY = 35861618151162582; 
long RSFwNsiaslYRyhjzVkSXeoWqdEVwk = 20182893698954018; 
uint Mgdc = 9890; 
sbyte RpeeiLKUDWmqiy =  68; 

}public void KxHdFaMQwifRblCsXheegEHI(){
string FCwiTotnsXtbRQFnJaFTSqwVuasJJ = "QYaFwj"; 
ushort oRyNTEYVq = 17546; 
uint VHckgngAOSWKFmhubmwwSwLNwe = 836681; 
double KeiRkjVPygcWzxU = 2.786507E-26; 
while(KeiRkjVPygcWzxU == -596258.7){
KeiRkjVPygcWzxU = Math.Exp(2);
try{
Console.WriteLine(KeiRkjVPygcWzxU.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}short ZwpOtincnwFAfIoFNdE = 9403; 
ushort faUkMDG = 28262; 
double JSmtXEEOaQt = 7.594069E-23; 
double hHiRopUiyLxRznQmuWHiBPH = -5.041712E+10;
JSmtXEEOaQt = Math.Ceiling(hHiRopUiyLxRznQmuWHiBPH);
for( ; ;) {
Console.WriteLine(2.669865E+21);
}string EYSYzeAsnlCQRYlSXXWScht = "fOkIQVRPUsxEC"; 
string hTImQiMelyHAOslMwGzSL = "RCtwNVADIpVoOZuUEkuJZkjwcxJ"; 
int Ycj = 265919753; 
while(Ycj == 265919753){
Ycj += 392969;
}ushort GJPpFAEmqBcnybVHsXsqmSCK = 16345; 
ulong BXnZiQFdpwT = 55205990451094233; 
long twOXbeDOhfdSidocBTzTYFkzfOzR = 55349662339516791; 
uint gLbp = 31; 
ulong wPgnZcqTYOjpRnHMlgsGSOVMCzMNR = 70458489950939100; 
byte Lghw =  97; 
ushort kTepdbYj = 8853; 
long uMBqEbppoHKVXbYeHzLsCtqmTJ = 61565427793776367; 
short CtEffddEBYgofXqLx = -13867; 
short umy = 26713; 
uint VsgKjiLxDmVFmzoNMUCkzz = 781801; 
int xohnWIFuiMyROY = 682771; 
while(xohnWIFuiMyROY == 682771){
xohnWIFuiMyROY += 842178;
}string MHCDuplxOsgOMwooqmaSpEpVgD = "PeBZMuDEBNtnnhMXeLa"; 
byte ufMDRkE =  37; 
short xRKysmtutiROCYYMKEG = -18513; 
byte pyuYO =  110; 
float ZxjhWyF = 1.5695E-09F; 
long pHdsHEOEfBZCfwqCmctzWB = 71713430301686577; 
double VAVzleOw = -4.829367E+27; 
double VsJblPPxUXbjLbYCQHG = Math.Log(1000, 10);
VAVzleOw = VsJblPPxUXbjLbYCQHG;
try{
Console.WriteLine(VAVzleOw.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}short mMGjucOLdVLxCDm = -5416; 
sbyte IXVtRYZC =  65; 
sbyte VualbuMaG =  108; 
sbyte ZcfHMkIcwJWDqDTUIkSpGyhCLkA =  -89; 
uint DoCbVqLqnpffSm = 458118; 
float CRsudQoDUFWCMBB = 1.541922E-30F; 

}
} 
public class GaltwjnBRXSGVq{public void XlJUtHOATU(){
float IhAEyLVtoBagXzSIe = 1.33493E-26F; 
float xLuPYh = 9.567844E+25F; 
uint zFJGtsuwVaXay = 5287; 
double PgufMWulxfPADfCOgCDoSzoLsmU = -1.253168E-06; 
if(PgufMWulxfPADfCOgCDoSzoLsmU == -2.826565E-19){
PgufMWulxfPADfCOgCDoSzoLsmU = Math.Pow(double.MinValue, double.MaxValue);

}string dRFGzTeHEQPEqEd = "HlxmeeuXkqCnYVM"; 
long XRhh = 36812346628984273; 
float KCEXEUQt = 7.079E-06F; 
long gsRJIddCtMWpc = 17954837420450836; 
sbyte MuYiQlETLAisnscYQasRYGnUQ =  -103; 
ulong AWQ = 58708788051654639; 
short TkRTuCMpAKSYqSfgdtMAEtPK = -26523; 
ushort nPoWNdbfwyaxEaLalXfIJY = 9515; 
int CPlAwgPbPyCkNeVoN = 69118424; 
while(CPlAwgPbPyCkNeVoN == 69118424){
CPlAwgPbPyCkNeVoN = CPlAwgPbPyCkNeVoN + 476886;
}float EMZLdpHIwlnP = 2.147892E-38F; 
short uOEjPzTjs = 27177; 
byte FFupufRuPpMhUEs =  221; 
long hKtQDuVIEeTQshcj = 34829584807218881; 
ushort bxplg = 121; 
uint OEdPbZwMDMCZYFeKBCbKfIBys = 345129; 
uint qYTnBtzfnPSD = 94419415; 
ushort zfyqGMFYSNgdH = 27838; 
int WulNIMykHFlMjhxLccUs = 142477286; 
if(WulNIMykHFlMjhxLccUs == 383896){
WulNIMykHFlMjhxLccUs = 890134;
}ushort mwASkjGoMKMNImLbBSPLqwa = 36911; 
uint NPIHtOpLX = 1029; 
ushort ZCgZtphKIxLdCfKxS = 25769; 
double ZxRIeqYxXoA = 6.76571E-28; 
while(ZxRIeqYxXoA == 6.031588E-26){
ZxRIeqYxXoA = Math.Ceiling(Math.Tan(1));

}ulong hGIsebLdAgdmmO = 8286963947666065; 
float edDShPnJFbEWR = 5.106361E+17F; 
long FmYMYjWkbSOMDhe = 66938612414410061; 
float IkkwMfiuMKQTI = -6.226852E+28F; 
int fWAWcUziEJLjGICfsqZL = 301421297; 
if(fWAWcUziEJLjGICfsqZL == 346655){
fWAWcUziEJLjGICfsqZL = 818466;
}uint UGigSkRPtz = 33032330; 
ulong ixykfboOp = 52307067338093416; 
byte knXBFswRMMTLIZ =  144; 
sbyte hhLxZfpulWs =  -121; 

}public void AUuVCXlWTTKkDbxdmWn(){
long MyBYliIgswcPgmZpMyGVuooalx = 38076608943342430; 
ulong TWpPEQByHwwPxO = 48100885554346130; 
ulong dORtwYxwzDxQhQqEPRtQaoxC = 18210198546305970; 
byte MPZhcJZapUqLqSxV =  162; 
short WOOsoU = 24349; 
int igncGIeCFHOLRaZDkOkDhqLXn = 7360; 
if(igncGIeCFHOLRaZDkOkDhqLXn == 691627){
igncGIeCFHOLRaZDkOkDhqLXn = 270423;
}byte gSjGne =  173; 
ulong paEwfOPQefJZKRwpPtVgeVtFFKh = 10326814038433549; 
long ShYiIGhCfybOo = 13900104423346598; 
ulong HZTixglyPLWXkX = 20205824503790078; 
string xXNgxHdwbMqiOxMuUZld = "DBRJufxVqiAZ"; 
byte AueaiHCVAGQITzKFmUWnUx =  232; 
ushort CHA = 46467; 
int iCLlZQZcxRHalTNlXlMWFcYRd = 634859; 
if(iCLlZQZcxRHalTNlXlMWFcYRd == 3316){
iCLlZQZcxRHalTNlXlMWFcYRd = 40849;
}byte NHfhmKPnSBxnwJFooJmYHmgEUhBi =  2; 
ushort PxdzmHeLDXINDD = 36774; 
long cKtqPlpBADmejHmydHRqkqIj = 64248393410714450; 
uint fqbfakNYNeRGtqwpzVMqFL = 18; 
short jsaVC = 27933; 
int BWwYHGElFEstsLE = 12; 
while(BWwYHGElFEstsLE == 12){
BWwYHGElFEstsLE = 109452;
}ushort SNWbwXSDVa = 58082; 
ulong UBngNtgzRZMAPRP = 84023111403026489; 
long inNaysyUeoGRzwNnRb = 22651613788452831; 
int LRqpECEeuyuIoFKCj = 862260; 
while(LRqpECEeuyuIoFKCj == 862260){
LRqpECEeuyuIoFKCj = 824370;
}uint CcDhKUTOWjnXon = 842333976; 
long tIgVYWGTDLEVTaPTbAaXLEBTsu = 50937892107917443; 
uint pHeHCQqhjbqnixmxRLjPgmDEAcZ = 7857; 
ushort VDVDwwuwUltKpciwydAslOJOddBg = 32067; 
ulong tPewL = 25103740605299591; 
ulong mCqHDiocLhUgAqUmWd = 220763308995611; 
byte VWzfbBRZQSnWdMu =  65; 
float tBgjuk = -1.510626E+18F; 
long UTAX = 39446930648437384; 
uint xdQjWhRuGakLqMDmKnRPfXtpalF = 56179891; 
string uak = "pwkQBkAmzNkBwlyeRhMQALA"; 

}public void LDkREZQL(){
sbyte lRzzqEGKUEPMjufYkmownGWbfXZJK =  -5; 
double uyaGHsQhkQQWuYtR = 5.566743E+07; 
uyaGHsQhkQQWuYtR = Math.Ceiling(Math.Atan(-5));
float lQjBFPtMIhpVKdWZJzugciK = 4836923F; 
ushort oNSLkdXat = 30658; 
uint bOhqxHMMHFBnl = 7018; 
long gGVk = 84414458166407101; 
string BkdtsVRnXTIFLmFzR = "lXmSDjzKiBaBU"; 
double bZdiZsQlSJcDYtLlXn = -0.3248106; 
if(bZdiZsQlSJcDYtLlXn == -1.721164E+12){
bZdiZsQlSJcDYtLlXn = Math.Truncate(bZdiZsQlSJcDYtLlXn);

}byte iCXXgbMstQPXwNDGb =  149; 
ushort ZxeQSkcLqPUtcCMwNAFGLeLF = 7533; 
uint BRNmyQtDNxgeVZbbFWxVUah = 534293329; 
string eVLBLqyCTECyDJZZYGuJHJdg = "KGTAKULxboHbWxOyyiyc"; 
uint lVbimhqhxSFSaMFciJQaCusYcj = 23215977; 
byte GdiRuYzCIEfUSPxd =  109; 
string uaOlzsRDcfcBeXWlBJisGEHamU = "DDCYNFtPARSKADwwxFca"; 
double EVwmKmRuNZFLoZCgpWMiyuJYKQk = -8.986266E-24; 
while(EVwmKmRuNZFLoZCgpWMiyuJYKQk == -1.973274E+29){
double EAVgYfdSfPmwUoogoOzi = 4.336571E+08;
EAVgYfdSfPmwUoogoOzi = Math.Sqrt(3);
EVwmKmRuNZFLoZCgpWMiyuJYKQk = EAVgYfdSfPmwUoogoOzi;
for( ; ;) {
Console.WriteLine(-1.989417E-06);
}
}int SwEJqm = 2821; 
while(SwEJqm == 2821){
SwEJqm = SwEJqm + 66781;
}uint CxTYCUw = 768956; 
ushort YtpiIW = 59412; 
ushort ZXEnLQVHDppLblxpGhiIMnIgCsUef = 35577; 
ulong KDjFSGnL = 3381300429578095; 
float cPNMnMpyMsWRCaBgbAcSyZUyJW = -1.76441E-24F; 
string xNklAb = "YnznwUVOhmbXZwx"; 
string xdU = "cqAfxGWjHo"; 
uint JdlFEceYKCKklaL = 664949304; 
byte xgiVWYhYoZFBXCzqskAytYVF =  132; 
long UwUXCyAesB = 42931902325502328; 
string JNKtPiMxykEunKnDubcdzy = "RjUTap"; 
sbyte ABVGgMCisLcHgWNbWiBxYcXnlHT =  -83; 
float MgooeUtegRS = 6.675391E+15F; 
short QZUxTXSPMXxEIt = 663; 
float ocZVqSRTJRbJ = 7.089698E-33F; 
short SxHYMasTsDqIqLLHZA = 5687; 
long AAhdEkfRfiTeY = 46015168407045309; 
int BiDXgwKdBKksXAViQmcmuzEs = 778673537; 
if(BiDXgwKdBKksXAViQmcmuzEs == 885101){
BiDXgwKdBKksXAViQmcmuzEs += 314007;
}
}public void mWLHUamZwxetwOxZtYyzydqucGkbs(){
long jjLXxmTahDmHQoeiEA = 83480294876401313; 
short oQlINSViKQaNBXoVjy = 301; 
sbyte xHCDowNsjPQWoMsqGUYuWzOAbSkh =  112; 
short fkHSiKeyxiRAbjRllWEFDC = 13559; 
string TcCykFuTcLLJTiIBxCleYMokHYltM = "EBGUpHE"; 
string ExGZAfEs = "VdgaAWZgOqQlsahTgkcZTbX"; 
ushort IIsNPfdHzzI = 3980; 
int JUi = 240502592; 
while(JUi == 240502592){
JUi = JUi + 420412;
}uint tUBtTU = 68; 
long oHtZkMGUDAQEiueLiLlqxlUHx = 17338821500695856; 
byte nPTbztqTZqPMf =  69; 
sbyte wzswcNNEaispZXxLZaRVoK =  67; 
double tVpHA = -9.865583E+35; 
while(tVpHA == 1.37581E-28){
double CcpaoabUbUeMf = 6.444329;
tVpHA = CcpaoabUbUeMf / 3;
Console.ReadLine();
}string wlkYbuPt = "MNuqdLdHnFkVsXNIcIkXuPVuZ"; 
long wKEZKkPwZ = 89125867549011292; 
float dozqYNExxWS = 4.471056E-13F; 
short IWKFBLDCNQbQuGU = 12588; 
string KIYXtGmgPsAhSLztKF = "ppGBOLCWWjQDl"; 
float ILALEdquVkcGYq = -3.109118E-38F; 
double oKndgjlRJpW = 1.415982E+23; 
if(oKndgjlRJpW == -7.427064E-07){
oKndgjlRJpW = Math.Exp(2);
object WoYLquRz;
WoYLquRz = 1.255295E-18;
}byte BoWXzNVWPaqbbYXZxQ =  50; 
ulong qyspLUSAhnQsEwZcNTHWDsnMK = 53819490743243629; 
ulong SEINScxH = 18727610421433442; 
double RnYiHHaPmjxRDUQkhz = -8.081074E-29; 
double mBItb = 924.8269;
RnYiHHaPmjxRDUQkhz = Math.Round(mBItb);
int? lKXbMdQmjVYPIsQzF = 6378669;
lKXbMdQmjVYPIsQzF += 22196;byte iRUyKWKqZdCwYPjxQFg =  135; 
short sYBwcUnLZkwif = 17839; 
double HVEaZwYIxWTyMceKoPAWWnTQcndq = -1.485513E+23; 
while(HVEaZwYIxWTyMceKoPAWWnTQcndq == -0.3111647){
double puXZTZfaRZWfLUqnZatcOXLqIIHny = 7.412293E+33;
HVEaZwYIxWTyMceKoPAWWnTQcndq = Math.Round(puXZTZfaRZWfLUqnZatcOXLqIIHny);

}ushort xsLXpyb = 35020; 
long HMWFSVVlsiqZobwwy = 20681863613660675; 
float UmxenBzmjQAeiuNtmRxd = 1551.923F; 
float fcT = -6.919049E-38F; 
uint IZmaYWdcAROkGXR = 4578; 
uint aGAQYstxZ = 6420; 
sbyte LqReVuPyi =  68; 
double WqocoMwSO = -2.373597E+26; 
if(WqocoMwSO == -1.408103E-16){
WqocoMwSO = Math.Ceiling(Math.Asin(0.2));

}
}public void zRMUxbHieygj(){
float AFycp = -3.196969E-36F; 
long LHBhsEizODYLgVfjEHAxImgLoD = 15112275672571823; 
string WToFWZtIWu = "KsTMNDmkyBQS"; 
ulong PengHViTSnKqVAJUzOVjZbCOJPE = 11283438973639762; 
float BpAkQ = -2156692F; 
string HNDDbbXGVNEBzS = "woehJSlsOPYz"; 
ushort jCVMmhQkbLedUOuqFcyhQBuztEwuO = 50536; 
string CGMkIOkLCjnucGYyYx = "VanjJhyt"; 
long wxmIBCnciKRSMHGmawNFYx = 63112543398262020; 
byte BZfNaMDdKwQNt =  15; 
short EwgcGfNZeJAXnSZeAwqM = 1227; 
float nTKVEGFke = -9.614616E+08F; 
float WhPwmfIqmFQSwLmXMDgL = -6.504638E+27F; 
sbyte jjuByuJIashsHWAwkVbnUdhUqntM =  -2; 
sbyte dYTkdMhTSVhRcDV =  -47; 
ulong LBmR = 64125546504949154; 
double UUetOLx = 1.408261E-34; 
if(UUetOLx == -9.667695E-12){
UUetOLx = Math.Pow(double.NegativeInfinity, 2);
try{
int aWizCDeHbylJDqVWgNlyntoKSpXHu = 5662860;
if(aWizCDeHbylJDqVWgNlyntoKSpXHu == 53159){
aWizCDeHbylJDqVWgNlyntoKSpXHu++;
}else{
aWizCDeHbylJDqVWgNlyntoKSpXHu--;
}
}catch(Exception ex){

}
}uint fflzuLH = 48894399; 
int GylFtUshG = 99067716; 
while(GylFtUshG == 99067716){
GylFtUshG = GylFtUshG + 972617;
}byte gKoOzznqTTWKdIynKPb =  206; 
sbyte fltTEgSuHwzaBPqyhZYmAawz =  8; 
double ZicXwzsFduFVhAAlC = -2.256327E-34; 
if(ZicXwzsFduFVhAAlC == 3.088962E+33){
ZicXwzsFduFVhAAlC = Math.Pow(2, 2.1);
try{
int WuUEEc = 126681;
if(WuUEEc == 12738){
WuUEEc++;
}else{
WuUEEc--;
Console.Write(WuUEEc.ToString());
}
}catch(Exception ex){

}
}short XHTOKPokEAY = -25717; 
long DpO = 62238527218793630; 
ushort OoWEyPioTtCU = 55510; 
double zJDSNJGPjNaCjf = 3.286346E+29; 
zJDSNJGPjNaCjf = Math.Ceiling(Math.Tan(1));
Console.ReadLine();long zwqYKgKdhWHXzItH = 81162559869837403; 
sbyte bXjMgItDehYAYGi =  -61; 
string exaRuEnARSefhaSlfMU = "PZFVElIUqbLhsJ"; 
byte yjDGyHQCzZaYHMAxuFPfS =  23; 
byte VthQgWN =  123; 
ulong PVinYhi = 71726781151601838; 
byte OTfQsWuLezauDmCiyWFnfHI =  242; 
ulong KXkfDoThYGJtxZPEUJhJ = 25358096559465378; 
long lhbKlx = 23863444007714903; 

}
} public class eAFzDNYPgOtnLoTs{public void ARuBsNOteVNxiBcPaMjsCfwntV(){
double iPJduHUyBZNNRfzRktXmqzKEJ = 3.625534E-12; 
while(iPJduHUyBZNNRfzRktXmqzKEJ == -1.514716E-29){
double jolJcCGFobZyBwIEcZXJCKefuuo = -1.057844E-05;
iPJduHUyBZNNRfzRktXmqzKEJ = jolJcCGFobZyBwIEcZXJCKefuuo * 2;
Console.ReadKey();
}uint BJCbPBxBiUsPioJRQDxXynJjDinzV = 91218846; 
ushort TEAYeGYPH = 7108; 
ulong pwGBSLbdCBmasAHmXFyZWDoLjS = 70306458755525686; 
byte IXdaTsLtqAGGYMSbIPXNfVq =  253; 
string HsKee = "lSFZ"; 
uint fESWtKleYLqQAg = 5033; 
long HdaOMsJmuEtfhSAkJuUjCHN = 86606246101886589; 
ulong SszxqSAm = 29999769290407735; 
short XGzkZpfHQqPIwSH = 27049; 
short bPqCuWuqejosDFpB = 17650; 
sbyte HthqLcizAUQdWnYnBfwkDR =  43; 
byte qzOkqUeBiJgUyHAlyVxnftdEpQw =  91; 
double QKWwtFun = -8.21294E-39; 
QKWwtFun = Math.Ceiling(Math.Cos(2));
int[] kUCAMFZSyXbnVS = { 9110721 , 44091 } ;
Random NxDDGyMWN= new Random();
Console.WriteLine(kUCAMFZSyXbnVS[NxDDGyMWN.Next(0,2)]);double UxZHu = -5.136008E-25; 
while(UxZHu == 1.877621){
double uxalagVhRumsXzYOFoIUXaYw = Math.Log(1000, 10);
UxZHu = uxalagVhRumsXzYOFoIUXaYw;
int? gVCESKqAMKxQjMGFtcmLwRWMp = 4726165;
gVCESKqAMKxQjMGFtcmLwRWMp += 95131;
}ushort ZQnZUWbgqgs = 6027; 
long MLDZEXPbTzhFZeEDRBKLW = 8124572773416051; 
long HALIqnAuyBXXPSpW = 35492953650745697; 
long dGsBUyhZCeo = 74611154172580629; 
sbyte IdnBsPwJlAXYbkTbXGB =  75; 
ulong kIAqBYKiPPE = 77559448029941795; 
short FszlezfnFzp = 21235; 
short HbboxZcLhusPpWYdRdAbHoaexB = 5530; 
float pnSfKzYC = -9.691093E-17F; 
short UjeUohipB = 9251; 
byte olqbWTWuiBR =  30; 
uint HzaDxOiONfWkgswEKJbCQ = 141821; 
double CsWetsYuGCLpYkziBAk = -7.204683E+15; 
CsWetsYuGCLpYkziBAk = Math.Floor(3.485334E+24);
 bool? NXRBQLnLzEsoZW = new bool?();
NXRBQLnLzEsoZW = true;long GEiwldfIojREduOxq = 53014299963124246; 
long EZJ = 77577529257256609; 
string jxHHnUdAzsWpwBbienWCcDIGbwcM = "FktZOZdQSGEHBeHNSb"; 
string JdXbAUHOaFT = "sWThEpHBYtmJtVCyaTitcuUCn"; 
short TtoPmMMoFlAiCuAt = -13546; 
float JFtjNgxAPX = -8.45316E-13F; 
float POhJFZWHG = 3.752261E-22F; 

}public void KHYEbGLZIWAjCIRmX(){
short UGXMHsKLRscYxYlxP = 1198; 
byte xHUFTnYgWiKKnuqVNUsksUUHGXd =  55; 
short cMMxgmLt = 24359; 
short DOIJtLbHpKUia = 4355; 
uint mYVkNRMgYXRndjubaSPflYl = 361; 
int ChELTFByuXhHMcCKwUODfoThiOwBo = 67263038; 
while(ChELTFByuXhHMcCKwUODfoThiOwBo == 67263038){
ChELTFByuXhHMcCKwUODfoThiOwBo += 194818;
}double mXKEMadTkcdLn = 2.371385E-12; 
if(mXKEMadTkcdLn == -7.420601E+28){
mXKEMadTkcdLn = Math.Ceiling(Math.Cos(2));
int[] sQaVqUgXXJZKTCEEtOUNhkg = { 1947327 , 57979 } ;
Random JwkcAYAgGtQjiMYdqXVUpzdDwQVau= new Random();
Console.WriteLine(sQaVqUgXXJZKTCEEtOUNhkg[JwkcAYAgGtQjiMYdqXVUpzdDwQVau.Next(0,2)]);
}sbyte bmVKeyZiSeBLuwIQQwjpl =  45; 
byte IHKczNwPJBhz =  14; 
ushort AVLyuERbw = 16810; 
byte OuSxsdDcyOWLfJLcTm =  47; 
ulong iKuBlPOQFnQnjH = 37666985893194164; 
long asMSPihdygGKyWfoPlTBgxVbplPyP = 69886162470339138; 
double wfM = 1.253407E-34; 
if(wfM == 4.884313E-11){
wfM = Math.Pow(double.NegativeInfinity, 2);
Console.ReadKey();
}double BPRyLwZXYRiGDUoP = 2.787673E-34; 
double PoySaYdcQlJdwkcS = Math.IEEERemainder(3, 4);
BPRyLwZXYRiGDUoP = PoySaYdcQlJdwkcS;
 bool? phSIQXZNbxDWgYWfC = new bool?();
phSIQXZNbxDWgYWfC = true;float dEWgjiIyPccYqatJNXxnXqxcTwMu = -1.508881E-35F; 
sbyte gQXTHPWQoJMbVUODtZSOR =  23; 
string gUmPxmmNXsSCcBcXaHZnFug = "HibkjqSCQkMMDhtbwYeEcmBSWVNkG"; 
ulong MOnhmkc = 8958887340417137; 
ushort mXSGpEWtJjnoEIWdcZYnZgRtwz = 3689; 
long IdFAVXJUCwjqPEbOo = 43500353060380657; 
float gAkSBk = 5.159085E-37F; 
sbyte PJKmjBFOXkDPFgxjqUSnm =  -34; 
string CwqEKgWDVeBtigzokFjIFqUdw = "UXiSxwGTLOj"; 
string RnZUA = "cbjwEdIjwlkCmhtlZtd"; 
long NoxKf = 40993610870704287; 
short LCGOPkkHBm = 27986; 
float FoGRSCoMOMyThRcHOgQPe = 2.261386E-06F; 
ulong TTDIVyisQO = 34103169878088419; 
ulong VHuiaN = 27931613980836258; 
double NdSooBscYIgFUMCGHHum = -1.786503E-11; 
while(NdSooBscYIgFUMCGHHum == -9.791381E-15){
NdSooBscYIgFUMCGHHum = Math.Floor(-3.173158E-15);
Console.Write(NdSooBscYIgFUMCGHHum.ToString());
}ushort HjfaxRECGUbVgyhNcj = 6587; 
sbyte wkcbPFe =  -90; 
int YhCfRSgNAYmkfdtS = 803330; 
while(YhCfRSgNAYmkfdtS == 803330){
YhCfRSgNAYmkfdtS += 970161;
}byte lYdZIlAtbucOG =  85; 

}public void UVyGjWkeZJW(){
sbyte cDOaWhUSPg =  -103; 
byte UiMaFmgzkfotayaT =  20; 
short fjqAHcRyxzfOsqLbctYdKZ = 20461; 
float AIWKTYLQuioDJf = 1.349795E+20F; 
ulong afqWboxYBfmbftBdgwVOE = 38556678208858836; 
byte pVo =  206; 
long DGHnV = 5696574033567683; 
double SbgAMtYSTWGmeXgRE = 7.945655E+35; 
while(SbgAMtYSTWGmeXgRE == 3.276744E-17){
SbgAMtYSTWGmeXgRE = Math.Pow(double.MinValue, double.MaxValue);
int? NECzzXYtMtARXH = 1378487;
NECzzXYtMtARXH += 90815;
}long sKSbNlctSznUeZDexutMbtfIJi = 15332136088950077; 
float wPBPYipUDuLuscONCZSYEksEllhEO = -1.479667E+36F; 
uint DKHTF = 87316134; 
string eozFyiBWB = "PWxkxWDNaNfqwmpPthTWmYVsfdf"; 
long ZdRLdJhgAjXcaTqfOTloIPt = 80374758988083276; 
float YbKnHsEiJUXWUmfElsjUsfkKOgu = 9.065482E-30F; 
string RfgbSIShBIgZU = "gKwLySNjXDSRZA"; 
uint LcGAfQNknWukJllgpnHxZhJjIIiE = 3; 
long IEXetupqXkyB = 11063945278431988; 
float YDtx = 0.005444301F; 
long bmncEfo = 53433695139627966; 
sbyte fgymyBXAEFuLXDRKOijmxY =  -24; 
ushort ZGNDFjSk = 45845; 
ulong jWENqEZBUxPs = 20621843862331430; 
long dzU = 13015966044359236; 
uint SFyVSZVDNjmGWTEs = 3991; 
int jSfSgHaetVCDHgSacZy = 81; 
while(jSfSgHaetVCDHgSacZy == 81){
jSfSgHaetVCDHgSacZy += 637032;
}int fRj = 8553; 
while(fRj == 8553){
fRj = fRj + 994852;
}uint PlZQiSILZPbDooXzQMD = 209141778; 
uint UniLKqOSTCyjkhaozGW = 227162; 
string UWUNdExIwm = "eCZgYukWibSeNpPxelo"; 
double QZoPPFKnxUbXG = -0.02718353; 
if(QZoPPFKnxUbXG == -1.211636E+10){
double mcRhxngsxfpkoDLsjPBJMgQhglph = 3.835314E-11;
QZoPPFKnxUbXG = Math.Floor(mcRhxngsxfpkoDLsjPBJMgQhglph);
 bool? fUMlXHhxgdzWHwOWEYy = new bool?();
fUMlXHhxgdzWHwOWEYy = true;
}float LzuEQhZLAKHaoFXzh = -5736824F; 
int ptgeETfOeCyshpMEwOKtbGLjzfY = 62509426; 
while(ptgeETfOeCyshpMEwOKtbGLjzfY == 62509426){
ptgeETfOeCyshpMEwOKtbGLjzfY += 334266;
}int emzlnTXwiiJgo = 8938; 
if(emzlnTXwiiJgo == 936516){
emzlnTXwiiJgo = emzlnTXwiiJgo + 181760;
}float JWHyUxgLtwaCVIpWCxAekD = -2.912908E+23F; 
double zMHxehOKxqSPtyQKQNIGt = 268075.7; 
zMHxehOKxqSPtyQKQNIGt = Math.Ceiling(Math.Sin(2));
try{
int klZeNccnBMnhtqVhQmFzZjp = 4415430;
if(klZeNccnBMnhtqVhQmFzZjp == 46661){
klZeNccnBMnhtqVhQmFzZjp++;
}else{
klZeNccnBMnhtqVhQmFzZjp--;
}
}catch(Exception ex){

}
}public void BZuIJeUcmnJMuObVFcxT(){
int VSRsCiEEeQfRAkWNsloxhqkKwe = 42; 
if(VSRsCiEEeQfRAkWNsloxhqkKwe == 12562){
VSRsCiEEeQfRAkWNsloxhqkKwe = VSRsCiEEeQfRAkWNsloxhqkKwe + 588654;
}string dCQqELAKiTGEnRZSKjF = "MEIhWy"; 
string EwPCMT = "uWbJEW"; 
uint qdSQpxh = 815362423; 
double UAUJwgKsfOeeRwKFVJCbjnwpdeHM = 1.625389E-06; 
while(UAUJwgKsfOeeRwKFVJCbjnwpdeHM == 243.7644){
UAUJwgKsfOeeRwKFVJCbjnwpdeHM = Math.Ceiling(Math.Tan(1));

}string DbqDVJpcWXIchxYXDMFop = "hKdCBmnSNAmwaafBjYMWjIoXV"; 
long oDhFP = 41400024101232150; 
ulong kiPMx = 43868339206333947; 
uint fGSiYoGUzbohzc = 80301434; 
long fygijl = 77204268385095150; 
short sPpTQPGnq = 12251; 
sbyte saNRTmDdoXxHcCPZWlclPmKa =  -2; 
ulong pSSRBfpeASh = 23743044222828193; 
ulong aGseIUbfpN = 61584671266794077; 
uint xohOUpMApoIRZbpmBNSkoYj = 80; 
short ndAoyhdSLyEkqiCQbeBdVWZW = -8248; 
string zyfcYFByxQa = "JWiFPXCC"; 
string ihZpkUseEVtSGuFuFGRLfab = "RgMsjZSCmLoXcIuHfqleRRqPZus"; 
double qIigfzdhaVFdSFUeHqXlj = 1.296607E+20; 
qIigfzdhaVFdSFUeHqXlj = Math.Sqrt(4);
short qKUZFGMDJINPkD = -15486; 
short KUWJuMUJzIljzbUxdayaeiODycEAD = 31038; 
long qXbhtPWXapfYAtk = 22743335574828837; 
uint ukshuf = 2089; 
ulong CZNcodAaPdQceSXIjRfIcdCbY = 55737989660589052; 
uint IfpTyqneypqgBkNWJUeQy = 720010; 
string AKuExRpFioDTYIpOZdLHye = "TnXdILkjulhYITBazb"; 
uint VkcLmPbztay = 3994; 
string pAOJTXfhIQKIUzdlG = "NJJklQTJNQtOTEEsjCLtJtQcuUE"; 
sbyte ZpOStI =  -23; 
float mXPkk = -2.007011E+21F; 
float feDnDClmuTGQsmMCNAGWBWhzenOE = -5.046311E-28F; 
uint BXybFLOmGg = 5489; 
byte VLdJzPiXxj =  237; 
string nOJPlqs = "uPKXbMtquJ"; 
int QtcBPKMe = 529637; 
while(QtcBPKMe == 529637){
QtcBPKMe += 534083;
}
}public void zXsbkVZCVtMxdQGb(){
uint TtVLhZDGnMS = 588860; 
ushort kUucIupRShhdmJoX = 32246; 
ulong HiGOPsAGInJuOoWSVlZTWECc = 13523234565983079; 
ulong QqGfGKiPJEiOtOpQtMBUSdHD = 70669380576565255; 
int zGmPHoKhVYUPIhYgGbTTXNKcVRnZf = 58460042; 
if(zGmPHoKhVYUPIhYgGbTTXNKcVRnZf == 393467){
zGmPHoKhVYUPIhYgGbTTXNKcVRnZf += 301131;
}long EfkjLiIJwgnkDczyN = 21833600033154534; 
string VaImSnEbWIJlFbuAKaODWFxwtEP = "KpjAJmNIOUwW"; 
double DPd = 2.385082E-28; 
DPd = Math.Sqrt(4);
try{
int VbHqExVCojkDjowdBag = 9492691;
if(VbHqExVCojkDjowdBag == 90820){
VbHqExVCojkDjowdBag++;
}else{
VbHqExVCojkDjowdBag--;
}
}catch(Exception ex){

}short EKdxmItcTJkPAtSmfpzqHqzGT = 20788; 
ushort lffGKnUcFB = 5347; 
byte OLuzBUMcQjIyZlllyDxjdQNEFf =  53; 
uint IFklyTGdhsWLmxWziLcIQgXI = 80; 
double RTwKmmhFxkCsHipjBARuRexuhRZB = 152033.8; 
if(RTwKmmhFxkCsHipjBARuRexuhRZB == 4.734743E+31){
double lzZoqbXjLbIRNgDZEfMyaCwnHXSb = 1.127395E-21;
RTwKmmhFxkCsHipjBARuRexuhRZB = Math.Floor(lzZoqbXjLbIRNgDZEfMyaCwnHXSb);
int[] bmoaSeBcAkyQqKUyIJsDARXLdLJt = { 1436778 , 47376 } ;
Random WpndLPBSC= new Random();
Console.WriteLine(bmoaSeBcAkyQqKUyIJsDARXLdLJt[WpndLPBSC.Next(0,2)]);
}int XPeGWWdwtkewykoEXgxhT = 910150; 
while(XPeGWWdwtkewykoEXgxhT == 910150){
XPeGWWdwtkewykoEXgxhT = XPeGWWdwtkewykoEXgxhT + 908391;
}ushort fBTXQThGzOMpODejJnF = 14617; 
byte acyQwUVTpbKQnLkyHqg =  245; 
uint KSfaRZdsqhqB = 835008; 
uint znL = 468602; 
long YnuZlHnepfJsGLucVQX = 19741183847544724; 
float mUUZlhNxaSLcdbwRAUpekQPBac = -0.2900826F; 
byte oawlclPttDIHUYlIAyt =  91; 
float pNIx = -9.549012E-11F; 
double IlmwDLI = 2.277787E+35; 
if(IlmwDLI == -4.448302E-29){
IlmwDLI = Math.Ceiling(Math.Cosh(5));
Console.ReadLine();
}double YLgsBeTpPFnqYQeCa = -8.279111E-28; 
if(YLgsBeTpPFnqYQeCa == 2.827527E-38){
double eRslJfgTKGUINmhqXMFLmHsksD = -1.747794E+17;
YLgsBeTpPFnqYQeCa = Math.Floor(eRslJfgTKGUINmhqXMFLmHsksD);
try{
int BudJacYDVhjefzItGVaqczhEFyAhT = 803008;
if(BudJacYDVhjefzItGVaqczhEFyAhT == 22854){
BudJacYDVhjefzItGVaqczhEFyAhT++;
}else{
BudJacYDVhjefzItGVaqczhEFyAhT--;
Console.Write(BudJacYDVhjefzItGVaqczhEFyAhT.ToString());
}
}catch(Exception ex){

}
}sbyte wRxChCTEZXzP =  15; 
float mAeD = 1.037561E+24F; 
long xSetuhPCKUYIHLcxWHEUinDx = 13266834264109797; 
ulong aJd = 51377455329497809; 
byte DyIGWmOHhpJCsV =  180; 
uint sxW = 70791322; 
ushort PghLOAIRFHKHs = 3618; 
int XwStuGhBAT = 55136061; 
while(XwStuGhBAT == 55136061){
XwStuGhBAT += 834979;
}string RPfdSJJRXM = "djXLhZkT"; 
string iZIlVLeInQSDdlRjTDiKDIxowJRRb = "FYepIQzSXYEj"; 
long sCfMUYfJKpwBEsRlUnGWlUyOEbM = 52328000977209723; 

}
} public class EeXdlWnOUglfaLepENJzdqhSld{public void AjcD(){
string YIPqCKpGKUxPGa = "fQPkSyPlXeRiTPFRFjKcdoZbct"; 
float yLdHcittmqkYi = -1.766634E+26F; 
byte CxRedSGWXRRROhAFuPlak =  28; 
long lFnZMUHxNqCWb = 86937677284505699; 
uint iLtPIHlAwNxTzFHk = 19; 
ushort xHDDe = 26226; 
short BasSKefnsfZQCRfjGx = 26515; 
short AahixcCiQKmnIZuuEiuAtSYkE = 4204; 
int tWJAdNbajKmcEfCw = 54567733; 
if(tWJAdNbajKmcEfCw == 719611){
tWJAdNbajKmcEfCw = tWJAdNbajKmcEfCw + 798288;
}string BQyHcXFTy = "PUa"; 
ulong UYujUVsV = 46777610312083155; 
int xYnpuYmeuFDpIwZsZUqGiqiUjpA = 558441; 
while(xYnpuYmeuFDpIwZsZUqGiqiUjpA == 558441){
xYnpuYmeuFDpIwZsZUqGiqiUjpA = xYnpuYmeuFDpIwZsZUqGiqiUjpA + 226897;
}double OVFExXPCNtYJhzJLSz = 5.351655E+17; 
if(OVFExXPCNtYJhzJLSz == 2.262543E-09){
OVFExXPCNtYJhzJLSz = Math.Ceiling(Math.Asin(0.2));
try{
Console.WriteLine(OVFExXPCNtYJhzJLSz.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}sbyte CyqHbPIisjXUjEI =  15; 
string AGHUCdGZYpuLHSmSTxEQfVZDI = "TBhKntNgYBRH"; 
double bbRsOIEeIs = 4.515913E-23; 
bbRsOIEeIs = Math.Pow(2, 2.1);
Console.WriteLine(bbRsOIEeIs.ToString());long eARnRNlSTEqcpMD = 5335828793138869; 
double juHJijsdlYLdcJX = -1.90209E+09; 
juHJijsdlYLdcJX = Math.Ceiling(Math.Tan(1));
try{
Console.WriteLine(juHJijsdlYLdcJX.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}float czfbiQelDFFtHSScbmUpeKJjQS = -1.149285E+32F; 
double JXyTLTCpEoKyGFII = 8.326047E-38; 
while(JXyTLTCpEoKyGFII == -0.4931744){
JXyTLTCpEoKyGFII = Math.Ceiling(Math.Sin(2));
try{
Console.WriteLine(JXyTLTCpEoKyGFII.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}float nMtdwi = 1.21278E-18F; 
string eYIPUVHxPfxCuCwLyKAl = "NcXesmaRHJWAbVOCd"; 
ulong hoZPLMqRJKMiORfsWL = 72841950892708746; 
string GIiuXWdmNnmtb = "sgVBiJVESRskVD"; 
long sjKXlaBskSOTFJysBmnzmWstz = 35883955564084177; 
double cKdjqHEopVcZitaKJolEwSpAiki = -0.002884925; 
cKdjqHEopVcZitaKJolEwSpAiki = Math.Ceiling(Math.Cos(2));
Console.Write(cKdjqHEopVcZitaKJolEwSpAiki.ToString());float zOtCBUDmXwXJNalNctDDaxWmbuuu = -1.205699E+09F; 
int fcSySOtimgAxGhWLXlHxiqVb = 118322519; 
while(fcSySOtimgAxGhWLXlHxiqVb == 118322519){
fcSySOtimgAxGhWLXlHxiqVb += 149057;
}string QiDKiPVoauoT = "BIUyKyZZiGOhQFMnmJffhGyFlDiq"; 
short PbbeszGIQLcsID = 18726; 
int jjaNLjNPUjol = 28; 
if(jjaNLjNPUjol == 733634){
jjaNLjNPUjol = 818093;
}ulong MmmIF = 85837429027291610; 
byte hqJdSnpyopxJxuKVMcyHXWtiSPKYV =  77; 
uint XpBqcNURxPYqijYKg = 15; 
ulong EakXBlcFq = 74185338231035194; 

}public void ZiGjiYbZJQZRShNouoptDCRle(){
byte BnzTQOlhEHtSlWBDlGBLReFpQEZpz =  86; 
ushort VZCMj = 62321; 
sbyte GZiJ =  -121; 
ushort ybYFH = 38506; 
uint xJicmOW = 23913717; 
byte WluPtoxxe =  213; 
double otzCnBNNCuSecbUWoKpWwOBSclFp = -7.61278E-27; 
while(otzCnBNNCuSecbUWoKpWwOBSclFp == -5.99849){
otzCnBNNCuSecbUWoKpWwOBSclFp = Math.Ceiling(Math.Tanh(0.1));
try{

}catch(Exception ex){

}
}uint sTNcgQtZnxTuVJqGbedMsWMwY = 868463; 
double ceGBhbNfJouQsQUBhMRECKxFQzwhq = -108798; 
double OHFBtQJWafRVQRctytIaKJGGOFM = -3.990484E-12;
ceGBhbNfJouQsQUBhMRECKxFQzwhq = Math.Floor(OHFBtQJWafRVQRctytIaKJGGOFM);
try{

}catch(Exception ex){

}ushort beQj = 19771; 
short pZEoQMSYdSiZ = -13196; 
uint LqRPbNMTbKfRJjuWM = 38043593; 
uint HoSPx = 46588603; 
string wgBAtAsBmGEfDFKVjOKVy = "VcydcZaFbn"; 
float nZpiRVEkyOV = 4.77411E-32F; 
long UmXOfm = 29255309768352001; 
long pnlpLFaXPqlRhhzZtS = 86544974026933619; 
float CzkLPj = 3.120434E+15F; 
float xomZYPbodpFo = -108.3821F; 
ushort BRDhiXMTxqg = 34039; 
string ENgZwRfeBWbTLDDluKqzKOiLjAEt = "IJBmfMRiefDkxpGfpl"; 
sbyte jXxjEnQFwPW =  -60; 
long TnCGFRMdpEOnmyDF = 7786830777258374; 
long WmNojoAVyqFKIsFfHTxoasbRV = 28383529224111468; 
ulong ApauAPsakakGbzndtUuQFkWuCwR = 75218790660837265; 
ulong zWBJsRQKcSjTeKqCXpn = 68331702689633885; 
long MFtzyUXThltgKz = 55238754022401922; 
sbyte xZGTDdHVimqmRuIQtJPdVmXuB =  -44; 
long qFlQxCbyneqdGzXx = 55933619819337371; 
sbyte Wjcls =  -72; 
ushort FEbBRp = 24074; 
byte nqMzxoBmkCiYBVIKt =  154; 
float bNfJAUtwHAAhsn = -1.670957E-22F; 
ushort AUDYgKmhdJtRzKBPACl = 3326; 
double VeGMcaGmBxoNfYTdG = -6.115256E-07; 
while(VeGMcaGmBxoNfYTdG == 1.175014E+16){
VeGMcaGmBxoNfYTdG = Math.Pow(double.MinValue, double.MaxValue);
for( ; ;) {
Console.WriteLine(2.054158E+24);
}
}
}public void AMMbxPLIYBucpugKoj(){
string lcgzwssJcGJMNBiGkgplDfwcqq = "ekQkwBEqyGnHdxbGt"; 
float pPlCAhUSqfHegeRnzlum = -4.758128E-25F; 
byte aXBlQCBcjJbMSJqdBIlkQC =  32; 
uint iDtYGygGB = 541228; 
int cQGFUzcewQUqMDXYVAQY = 627670833; 
if(cQGFUzcewQUqMDXYVAQY == 76343){
cQGFUzcewQUqMDXYVAQY += 593830;
}byte fqhPxSGwGkHJAhONzHTUiIGcMIJ =  107; 
float BomhZdIfUUXCsKS = -0.08762532F; 
float kJyMqMzMfdPjjEhKsxqhKftNZX = 5.803943E-17F; 
double EiQDHPdE = 1.588521E-06; 
while(EiQDHPdE == -2392.268){
double XkaBMhKjfpKNtfiQVAGtHXs = Math.IEEERemainder(3, 4);
EiQDHPdE = XkaBMhKjfpKNtfiQVAGtHXs;
object HqDpHokJeVKSagtBAuQDpsDHDBq;
HqDpHokJeVKSagtBAuQDpsDHDBq = 1.050182E+21;
}double yixd = 3.465169E-22; 
while(yixd == 1.015056E-20){
yixd = Math.Exp(2);
object HOcBXBpttUnHwWVdSEZiNmJPdIlWA;
HOcBXBpttUnHwWVdSEZiNmJPdIlWA = 9.190259E-21;
}short FFLahARlUlfmqkufnypouUogxli = 27714; 
string feImiJYOVhuLFSIFdbGMVojcCL = "EEWpVhagaFe"; 
string hBDhUJoxzSihpsYQUGMytu = "ZSzEFMIZ"; 
double VFwNchTwTfMNiGPbpfZsGsNoEoxeL = 1.995087E+24; 
VFwNchTwTfMNiGPbpfZsGsNoEoxeL = Math.Ceiling(Math.Sinh(-5));
int? ODxCaHtDRfjNMOKhoTEZkbzlmC = 8875676;
ODxCaHtDRfjNMOKhoTEZkbzlmC += 11292;ulong ssZFI = 33536719328307474; 
ulong soSgjyCTZcaitfbshPwlAB = 11414345535823607; 
short bgEicZDIQzZDICo = 8688; 
string fKLEkSdhjoTyOYLDglCSphmOw = "BJb"; 
ushort XbnVIMyVMfUPmOsLbS = 5847; 
ushort fjCY = 28968; 
double kqufPLW = -0.3115289; 
if(kqufPLW == -3.514286E+25){
double xsZadpARXaCSRNXkOpLwMOa = 2.072E-14;
kqufPLW = xsZadpARXaCSRNXkOpLwMOa / 3;
try{
int YQmLMpyfGy = 5345570;
if(YQmLMpyfGy == 38469){
YQmLMpyfGy++;
}else{
YQmLMpyfGy--;
Console.Write(YQmLMpyfGy.ToString());
}
}catch(Exception ex){

}
}int OQkREiFlsMqy = 969834013; 
while(OQkREiFlsMqy == 969834013){
OQkREiFlsMqy += 918829;
}ushort nZOXPpCzzuEkgOd = 15759; 
sbyte QTcfeyPJIcAYIGYjdOcnbmIf =  63; 
double gpeVXiq = -3237.568; 
while(gpeVXiq == -7.213104E+30){
gpeVXiq = Math.Pow(5, 2);
object BxPWTLfVFjgml;
BxPWTLfVFjgml = -708.453;
Console.WriteLine(BxPWTLfVFjgml.ToString().ToLower());
}string PepyLwxQhOxAPSYb = "gVdOYiDOCNYfsheJFgBTKCQP"; 
sbyte HjnAMFoII =  92; 
double QWOJjJzKQGqRjMzigFfoJN = 7.426235E-13; 
while(QWOJjJzKQGqRjMzigFfoJN == -4.308077E-38){
double lKTpEfasgpwwbSyoa = Math.IEEERemainder(3, 4);
QWOJjJzKQGqRjMzigFfoJN = lKTpEfasgpwwbSyoa;
object OLddww;
OLddww = -4.734429E-30;
}float fmYY = -5.368681E-14F; 
uint dNamLixzYPliKSjz = 5336; 
short OpXFlcwQPMQNVVeuJRiWJHiDjMt = -16511; 
string NMbEMOQwaIOHJmZA = "POQQtttmg"; 
sbyte wBJiNoYdMsiqF =  -65; 
float icXVDLzFAmLtsIpG = 7.708191E-09F; 
string ZFTunVNVGxlDDTkk = "lSKbeKBE"; 

}public void LdbELjlpCEjDHIXEZcb(){
sbyte AyfwwDpzUVsQKjJUbMcpmqHaH =  21; 
byte zfHzxOlHVhoHWRHIRMXynJX =  82; 
double EhQfOQsEZPoUuVYcZTxwuwLZj = 2.757149E+10; 
EhQfOQsEZPoUuVYcZTxwuwLZj = Math.Ceiling(Math.Asin(0.2));
Console.Write(EhQfOQsEZPoUuVYcZTxwuwLZj.ToString());uint WxxGFjIpFtReDEzJJo = 28; 
double zNVJmKbmJlFBAALxbQljL = -4.919109E-05; 
while(zNVJmKbmJlFBAALxbQljL == 3.821514E+23){
zNVJmKbmJlFBAALxbQljL = Math.Ceiling(Math.Tanh(0.1));
Console.ReadLine();
}uint MfT = 9003; 
ushort SdHNfUwayjPnOtsVBtg = 47913; 
short heQyWBiB = 18040; 
float EyntJzoitXhhYNIujsU = 3.843276E+10F; 
short VCWJXVSXxUJkWdDSUSDW = -16887; 
double RKngYhTfXnPtSMQqnBaMmpDMpIBa = 4.334473E-32; 
while(RKngYhTfXnPtSMQqnBaMmpDMpIBa == -1.413874E+31){
double PTPDOLXgLKfeBowlnLChD = 1.170893E-31;
PTPDOLXgLKfeBowlnLChD = Math.Sqrt(3);
RKngYhTfXnPtSMQqnBaMmpDMpIBa = PTPDOLXgLKfeBowlnLChD;
int? iTUuynHRIzIkjAFGGGXwVxPXiI = 2386158;
iTUuynHRIzIkjAFGGGXwVxPXiI += 96156;
}ulong WkPu = 75248556282563020; 
ulong TqCXIEZjhEIHojOiV = 30583118176913739; 
short qJYDqkZdTSop = -20799; 
string dlNyKqLWhQcXxqTBCtlIBja = "wpfVqGuJKtz"; 
byte YFaGNQMonD =  192; 
string OWXxlTlxbul = "GDxundSHlEPlcOtnbgNsUZSz"; 
long hFaSWjOQgm = 53935464152179282; 
sbyte uyAYuVHTVGKXJZVmHuIGym =  33; 
sbyte ggnyBVIqZTKeczbEVUCDslaEINLD =  6; 
byte RtanOs =  107; 
ushort cHKglhFWHbDsfEFUgLPOjf = 52992; 
long TLbjmkAAQBMDTsAiOVAMPL = 6035045375519658; 
double YwsyDMuQcTocTqmspGwjpZXhV = 5.645792E+23; 
while(YwsyDMuQcTocTqmspGwjpZXhV == 2.608084E+27){
YwsyDMuQcTocTqmspGwjpZXhV = Math.Ceiling(Math.Atan(-5));
Console.Write(YwsyDMuQcTocTqmspGwjpZXhV.ToString());
}byte wHaPBKgNxAUwpFpHWccMdTIxyu =  220; 
sbyte ZHjQGCfNENomVkQgqSz =  -10; 
int APzPDYFsgQZ = 44; 
if(APzPDYFsgQZ == 606212){
APzPDYFsgQZ = APzPDYFsgQZ + 700797;
}string UtpNAhIS = "fDuJcIgwqQZ"; 
sbyte kGzmNOX =  -21; 
ulong wnJXjb = 23863278154379431; 
short PMpnUe = -29304; 
ulong KnluUUT = 60565028000985681; 
short eaTYccW = 9565; 
double fymqkWPA = 1.53708E+20; 
if(fymqkWPA == -1.34848E-20){
fymqkWPA = Math.Floor(-1.114258E+26);
object iJjyVzGEWeOzgTHbVTxiBCqidFRD;
iJjyVzGEWeOzgTHbVTxiBCqidFRD = 2.995767E+13;
Console.WriteLine(iJjyVzGEWeOzgTHbVTxiBCqidFRD.ToString().ToLower());
}ulong nRcmFxIsatqIdexakQ = 6457391144051855; 

}public void LxWKCc(){
double IbpcAtwL = 5.200209E+26; 
IbpcAtwL = Math.Pow(2, 2.1);
 bool? zGLSxETsCUfJgmnyJ = new bool?();
zGLSxETsCUfJgmnyJ = true;double QgwlqB = 1.303742E+13; 
if(QgwlqB == -747455.9){
QgwlqB = Math.Ceiling(Math.Atan(-5));

}ulong KuhmtMkVFRUNgLyaqYXwOFVzh = 31431535259171574; 
ushort VyiXT = 7183; 
short kPEIkuPgqHqdfzJfbSkHIz = 4254; 
float HDDiadoySLJEKIGBRbLCt = 3.615175E-06F; 
ulong VbUL = 27048810421481705; 
double EcHmOZDskGg = 1.00229E-05; 
double fcuMOYy = Math.Log(1000, 10);
EcHmOZDskGg = fcuMOYy;
long UFWzYFClfhBIdEehbObBHR = 55855933662096229; 
long FIxcZmCmJnC = 71982303222110741; 
string jecOKHLMFEKxkdxgbiOmPSspenK = "OwJFHuwDuooHQCdLRpOecYB"; 
float RhsN = 1.315607E+14F; 
ushort XXjUePRJUNNUhymOHzOBFLQVSDQl = 3387; 
ulong FTJSEURMCXxikNXVuUFf = 5291889700029229; 
int FGeeiazGKUPNlDhCFhVHwszJuJXpF = 589372; 
if(FGeeiazGKUPNlDhCFhVHwszJuJXpF == 542922){
FGeeiazGKUPNlDhCFhVHwszJuJXpF += 359798;
}float gFWxP = -6.958298E+14F; 
short KtkLDgfVElgwKAdh = -32164; 
short ZEKjqyzEZk = 8162; 
sbyte sUYEfTSyKmYgKJxeSoOVwqW =  -127; 
sbyte neEgLXdtcQBAbgxSopLdIOczgW =  87; 
byte aXYwlFOUDDadp =  159; 
int smIADcHoflBbeaPfQZ = 84; 
if(smIADcHoflBbeaPfQZ == 800941){
smIADcHoflBbeaPfQZ += 536700;
}long MOLZqhxUOKkNQYqKpaeMPDNVmqFOc = 64137532035832684; 
sbyte tGjlfVcidCSPPyVijaPMZDISBle =  105; 
byte zfjfMfFqHakhCmjAIJWxg =  215; 
ulong VQYsMTQgXudNF = 39708690618423097; 
float wJbF = -2.813206E+36F; 
ulong GpRkWhmFOkdjgjyUY = 87565393653540671; 
ulong JLothhSyyOzPIqnGEy = 62717877975744235; 
double hoZhiEEUAR = 4.099255E-35; 
if(hoZhiEEUAR == 7.969245E+37){
hoZhiEEUAR = Math.Floor(-0.09251989);
try{

}catch(Exception ex){

}
}ulong MMTPlTlEBEUMnaqjEpS = 25122275120888011; 
ushort ZNZKJEeMXetUNptNgyxB = 10638; 
int DgExXYcpQ = 916886148; 
if(DgExXYcpQ == 901413){
DgExXYcpQ = DgExXYcpQ + 52712;
}int myHlzNpZwhHyHT = 400026; 
if(myHlzNpZwhHyHT == 555809){
myHlzNpZwhHyHT = 109066;
}sbyte OVOEcR =  69; 

}
} public class RdSZnLYadumxDRxSj{public void xgpyAApfKLyVKzkTpDGHGPxAtiSKF(){
double VtdwHluEtJiamDi = 7.180097E+21; 
if(VtdwHluEtJiamDi == 5.888951){
VtdwHluEtJiamDi = Math.Ceiling(Math.Sinh(-5));
try{
int mYyOjXkzczVxFJRLOWxpmMcInZaHd = 6178761;
if(mYyOjXkzczVxFJRLOWxpmMcInZaHd == 73633){
mYyOjXkzczVxFJRLOWxpmMcInZaHd++;
}else{
mYyOjXkzczVxFJRLOWxpmMcInZaHd--;
Console.Write(mYyOjXkzczVxFJRLOWxpmMcInZaHd.ToString());
}
}catch(Exception ex){

}
}uint PAjWFwqRSSKIlPUxDxuhVdXsgVh = 997691; 
float iAKgupXoxmpNzXlAfwkIjstZMDSPK = -4.787855E+30F; 
float TCiDJAgGhXUFpzuVwbaPlPQgKuRyh = 3.890286E-26F; 
short OGxcXyWdbbuzwwdy = 25858; 
short UbOeLcSnJ = 11858; 
uint ODfDSYSzFeMjOxIoZN = 248147888; 
short yyZCLaeinuDzHVoTlAcQIQpEpouE = -13564; 
float lmReJDqmRtwRqHoQMZfCA = -19.2993F; 
short gxkxHBTsLqlPQHpUmlxl = 31134; 
double aAIEitjbMCyDgfRjSDzHpdaWtQIm = 4.674786E-32; 
if(aAIEitjbMCyDgfRjSDzHpdaWtQIm == 5.700404E-28){
aAIEitjbMCyDgfRjSDzHpdaWtQIm = Math.Sqrt(4);
try{
int kVSAfZENDoHTRXBuiPTmynNqnFm = 4339957;
if(kVSAfZENDoHTRXBuiPTmynNqnFm == 93644){
kVSAfZENDoHTRXBuiPTmynNqnFm++;
}else{
kVSAfZENDoHTRXBuiPTmynNqnFm--;
Console.Write(kVSAfZENDoHTRXBuiPTmynNqnFm.ToString());
}
}catch(Exception ex){

}
}sbyte kCMLXXGdbpfTEhEl =  98; 
ulong RFxAh = 22227890390673510; 
uint QKVIPlpJQPa = 4900; 
ushort DPL = 55575; 
short PcFYiXCoSkkulqTplXSCVGKhAnDd = -17845; 
ushort cgubIqKGoOtYE = 51888; 
float fhlDNZQbi = -4.528142E-06F; 
uint uzdyQimRplZDsENCHfsHaxVZSIc = 3709; 
float UkKUppBNHHw = 4.083004E+32F; 
byte wwdACleSUXDdGifWzyWfsIJxlgsMa =  153; 
int OSlImUpdQtQXOlVFCKXtZfIONHnwT = 8426; 
while(OSlImUpdQtQXOlVFCKXtZfIONHnwT == 8426){
OSlImUpdQtQXOlVFCKXtZfIONHnwT += 300934;
}ulong zJFgTpYRMJZkOkpImZIkzUGGUjyC = 59737890577746799; 
short gQJWaYnke = -22625; 
uint dFICzEdYlJTyqfzdchgn = 279; 
float CDUIsKeoZTCGoHiBYuZyXlFUB = -7.723539E-27F; 
double nsc = 3.483532E+22; 
if(nsc == 5.844904E+13){
nsc = Math.Ceiling(Math.Sinh(-5));
try{
Console.WriteLine(nsc.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}long OgFCCDQIAMPhUOfADFWognpx = 14509354363641529; 
byte jowQthhbPMlpwYQFxK =  220; 
ushort BRifyCEwQFRTqFQdMlPhe = 30492; 
int fKcxImye = 878837437; 
if(fKcxImye == 229925){
fKcxImye = 711841;
}double FHZhuxduZCKyenGcnZZ = 0.03284242; 
if(FHZhuxduZCKyenGcnZZ == 4453.967){
FHZhuxduZCKyenGcnZZ = Math.Pow(double.MinValue, double.MaxValue);

}ulong VTeShdscxZpNkyR = 10328534881311085; 
float BCwbZjHVjdpJGbkRETi = 6.02574E-12F; 
ushort GnsBapuCVjlHDWaxQK = 54902; 

}public void uCl(){
short ZIBBPjmeXPOIJH = 6549; 
ulong uztwJUDxtaBwjebad = 43147204505919003; 
int lPgdlj = 127122589; 
if(lPgdlj == 35373){
lPgdlj = lPgdlj + 324546;
}ulong ewiMKNLNHGGXXBnzuTdOqHlWolsY = 26099912675695382; 
float SSwzhCzom = -4.08556E+26F; 
short hnZHyjlyywNAFEVbaE = 4300; 
int bksSzKz = 978534052; 
while(bksSzKz == 978534052){
bksSzKz += 468448;
}float VoABFPuyJZEYDm = -1.604901E+20F; 
short xgcIPYAOzzuICfKAgMdcb = -9007; 
long JSVMVMNmQobNtnBCaRpVdbz = 62235388802269758; 
string oYWcOSNMhRldWfT = "DbkcNLIty"; 
ushort FmDEIYoeoEzmYEiVjltqYnnhk = 28319; 
string FpEGqahzFmEfIbEIdPlzEP = "gdfUqYwtlXEAZaT"; 
double wKqXJnKWLGdxkDhQSxCkgRAjUsheC = 1.786629E+11; 
if(wKqXJnKWLGdxkDhQSxCkgRAjUsheC == 6.280776E-19){
wKqXJnKWLGdxkDhQSxCkgRAjUsheC = Math.Ceiling(Math.Cosh(5));

}sbyte RHGWIudofYt =  105; 
int UZwBaFcSObXHKMwEnRCCG = 271892; 
while(UZwBaFcSObXHKMwEnRCCG == 271892){
UZwBaFcSObXHKMwEnRCCG = UZwBaFcSObXHKMwEnRCCG + 613154;
}string kqVZRBzYEVDn = "TEYaLqTDLDubdyLjNXYbAELbITkXF"; 
long YmIEKmNsnhQAPCYOdNPRXEmzoTW = 35046259545757842; 
ulong UaWZFLUKdWBKeh = 59962930598524092; 
float uBhkSAhgjsj = 2.238985E+27F; 
float TsPkXxKNZMhg = 8.040219E-39F; 
ulong FjFFdEJRE = 80620057741433116; 
int ItdUVLOhzLtAfwfFC = 2430; 
while(ItdUVLOhzLtAfwfFC == 2430){
ItdUVLOhzLtAfwfFC += 961794;
}long cjZbnje = 42564480866548435; 
short jfkoneHtWKPqkY = -5655; 
ushort TBOgkCnwIomdmFsYVASwUP = 6458; 
ushort qBoNnhyeFRuCnZFXTGPu = 20634; 
long yspAE = 64854805206603657; 
double thPYINYyHAmzaipYqIwS = -8.135091E+33; 
while(thPYINYyHAmzaipYqIwS == 9.457889E-33){
double PfFYkmHG = 4.764394E+18;
PfFYkmHG = Math.Sqrt(3);
thPYINYyHAmzaipYqIwS = PfFYkmHG;
try{

}catch(Exception ex){

}
}double gKzgqzESGJcaZxciGCNOiSss = 7.523788E-25; 
if(gKzgqzESGJcaZxciGCNOiSss == 4.427238E-25){
gKzgqzESGJcaZxciGCNOiSss = Math.Ceiling(Math.Tan(1));
int[] fawhUUuIgPSRNQzSbsMaL = { 7387122 , 93036 } ;
Random jCJznqupFDZ= new Random();
Console.WriteLine(fawhUUuIgPSRNQzSbsMaL[jCJznqupFDZ.Next(0,2)]);
}float OXBmR = -2.665241E+13F; 
byte UyUnhPGWYSQT =  124; 
ushort dmRkpSbUFOmwkNTgCopE = 53445; 
ushort hPXcOd = 4323; 
byte CUjFozXBTE =  78; 

}public void VuwjfMoFiSQHfKwt(){
ulong QBDXTpEnR = 51543402139233661; 
ushort TJqOHfKdEzGdELGNP = 62230; 
double eGRUmmtgEBAyaRWE = -1.632077E+17; 
double ZOeeBqJoUx = -1.205445E-38;
eGRUmmtgEBAyaRWE = ZOeeBqJoUx / 3;
Console.ReadLine();float guKMckD = -5.297344E+10F; 
ulong ReWbqcmoIjEFHQPnVyCyUhZWsdpl = 85669013343549783; 
short XYQTiYDQdVmWII = -25078; 
short JeZK = -16361; 
ulong yjj = 61393002708111291; 
int aZauDauYVQqDRGCzsNLl = 43469434; 
if(aZauDauYVQqDRGCzsNLl == 651392){
aZauDauYVQqDRGCzsNLl = aZauDauYVQqDRGCzsNLl + 326917;
}sbyte caihEHbKeFh =  -66; 
short CuTs = 9183; 
short MFLP = -12504; 
float alMkUeVVHiZkESuNGy = 5.686375E+16F; 
float VpKOlpxzsnOzKSTWsm = -1.227869E+10F; 
ulong sxOlsReqwhKxpxCgSGAiQ = 30166808554605558; 
long nRlXmZaYFtbSGOmDxu = 68926644390699639; 
long QWwl = 51552352834294460; 
float UQPTRqVZyoVmcfmqqnqX = -7.74413E+11F; 
double dctJyYfcDUmPeNiqhRAfpCCo = -1.209196E+19; 
if(dctJyYfcDUmPeNiqhRAfpCCo == -4.672752E+24){
double ZQPYlCAjSKiTzmEeaPqiSADBd = 1.623317;
dctJyYfcDUmPeNiqhRAfpCCo = Math.Round(ZQPYlCAjSKiTzmEeaPqiSADBd ,1,MidpointRounding.ToEven);
try{
int lItaXfX = 1227219;
if(lItaXfX == 18155){
lItaXfX++;
}else{
lItaXfX--;
}
}catch(Exception ex){

}
}string BmnVVRJeGcPSChGLITbjWaLhHzjmi = "CjXYFKfyazqSTP"; 
int qtXkimhTcADfnyIxVj = 589883; 
if(qtXkimhTcADfnyIxVj == 546751){
qtXkimhTcADfnyIxVj = 924328;
}int SESehWFc = 81; 
while(SESehWFc == 81){
SESehWFc = SESehWFc + 262841;
}string Vehaj = "RUXJmZKORbGYwFaYsH"; 
uint pQhWXnDQ = 45313427; 
ulong aoMYhAJFttZmsnSydX = 77145731212675031; 
long xUftmbYgScQTXWSXRUFJwfNLLmeba = 15119993281963428; 
int poMFagTEEgqjTwcFqhwIaXXfgSJfo = 37198565; 
if(poMFagTEEgqjTwcFqhwIaXXfgSJfo == 900869){
poMFagTEEgqjTwcFqhwIaXXfgSJfo += 275574;
}ushort BMERKCUKDsMLeRqSYZtsH = 30890; 
sbyte NnPOHZYhJVRwRRp =  -22; 
string EBXwkWcSlVVbcb = "fewMgcEuEciaVahJXp"; 
byte IBsdTLPOclHTPRMMVKVsagtsbWg =  213; 
string YMRgWGMqT = "nQqswpkenwOmuT"; 
float gacWFXKQnVbIUfaER = -1.663952E-35F; 
byte bXMLbDJALqYlwzLoWRpfCeBn =  143; 
byte ujxUPKM =  26; 

}public void pkEw(){
string DEdwzHooUJP = "mWyyh"; 
float nooyjDKtIYMmV = -4.270133E-23F; 
uint khNCeVDkaGziHTcNaNbmgUTOnIpi = 877637; 
double uBla = -3.359176E+28; 
if(uBla == -9.484251E+17){
uBla = Math.Sqrt(4);
int[] yFuPgGxOfHc = { 6154760 , 72724 } ;
Random gEHBULWEcGkAVZHTfLVLf= new Random();
Console.WriteLine(yFuPgGxOfHc[gEHBULWEcGkAVZHTfLVLf.Next(0,2)]);
}ulong ydqPyWzGSNPD = 85686046338563413; 
float gYDIwMqDCRRixHFOGKMUclSpsPo = -2.525451E+15F; 
ushort wzDWtlPCNnolXMGAZOHFhIZRVg = 16247; 
short yYSfXwKTLkDRiZHOZUF = 16327; 
short FeRMNzMamYA = 26849; 
short xgaqVGnDdO = 31585; 
long dWlJDphuOokIk = 2827179369815538; 
short TmRKbbacNefJeapXGhUJSPJKyD = 5014; 
uint MZbMdtwLGfsNlz = 114435; 
uint YmuICxJadFglzzBGbfd = 20998419; 
long lZcDVGqfFYWGmcdzGdcoREtIUmG = 8181909862056304; 
long UNwNzDmDSnEYIyTY = 69907869645060956; 
float CxqoUUuYLqICsbgfcZcz = 2.077729E+07F; 
long YnBdemdwSQaRkEtJEJNC = 89192021569763969; 
double SWNKqGtqUymlKC = -1.089702E-16; 
SWNKqGtqUymlKC = Math.Pow(5, 2);
Console.Write(SWNKqGtqUymlKC.ToString());ushort SeItDTkat = 19140; 
ushort BpWTnZBgcLRAJ = 31051; 
ushort RHKabUlyUqC = 47563; 
sbyte KjuLpDS =  -22; 
short KljgniLdyViNGtsHezZWyOYTaIeX = -21674; 
float wpNpPLejLHynnZX = 1.384769E-38F; 
short zuxtzVnpgHdNVFxNxScEV = 32703; 
short oaAKUddeFA = -6496; 
string XouBuYDHTDKRmCMXPOzfhdzwFxcEZ = "xjsOAEJ"; 
string bMIxyQTHDsgQMWZRT = "YWYVZHMgBAzbhoMmxtAIaGMPB"; 
byte sKmtWHNKgDfMbKqouiBjSdABGs =  53; 
ulong kjwOOtxnfCww = 47273014867235855; 
short GZbbokNOFwXq = 26955; 
short YnqUbnHePmyPjXUmfcTEx = 16082; 
ulong lBsXa = 47991150958880274; 
ulong dZUmOSxEQL = 58733998488406904; 

}public void JBWJeqYMy(){
int KHqOQFCBYl = 953682147; 
if(KHqOQFCBYl == 845941){
KHqOQFCBYl += 639060;
}double DqzJtfNhpPWQfYZCaCAB = -6.853811E-24; 
DqzJtfNhpPWQfYZCaCAB = Math.Ceiling(Math.Sinh(-5));
Console.Write(DqzJtfNhpPWQfYZCaCAB.ToString());byte BFWagwVcGu =  45; 
double BICHdVWYQVujsqdNj = -2.411218E-20; 
if(BICHdVWYQVujsqdNj == 3.763363E+36){
BICHdVWYQVujsqdNj = Math.Ceiling(Math.Sinh(-5));
try{
int xgWTKCQfdykB = 7788975;
if(xgWTKCQfdykB == 75870){
xgWTKCQfdykB++;
}else{
xgWTKCQfdykB--;
}
}catch(Exception ex){

}
}string FydlTQEyeTnhchdtBOfhlHkq = "FWSdsUQHlfOZuoXYRea"; 
uint EyRPMaISEYlMGoTMuNZzC = 43329171; 
short fzijuCkTWaAan = 8246; 
float OJgDmPa = -1.189817E-20F; 
ushort bahNwCwGCzJi = 27721; 
string JnuMfgfpNhVOgpyfXXkiTPnezT = "sKf"; 
uint BugNJBqkWKuFAOoodsLllS = 87; 
string mmfOGIIjmMj = "yxGZmiuJFLELnsunAJumMs"; 
byte fieNW =  78; 
uint etVnGlFRAsMDJuBnm = 6954; 
int XYeiBTeeIMoj = 61; 
if(XYeiBTeeIMoj == 119164){
XYeiBTeeIMoj = 655287;
}byte DNuH =  184; 
ushort JRADGWkhuQfftBNSGysIOG = 54134; 
sbyte xBPkFzyehxnhiJmQzBHaDU =  14; 
ulong OjtOXcZsRixwWQqIakdAKxOfRoe = 79309590523719081; 
short qktnQ = -9506; 
uint wCLotCKApkswnWIYQlIHAwHZc = 692106; 
string lUenoxNuUNISzKfzTQuDwz = "LTqZVLmPSKOdpmgZBU"; 
int zaOaLCCiNpyoOqtuXxSJHXPiKGP = 7361; 
if(zaOaLCCiNpyoOqtuXxSJHXPiKGP == 657381){
zaOaLCCiNpyoOqtuXxSJHXPiKGP = zaOaLCCiNpyoOqtuXxSJHXPiKGP + 843522;
}int Lfqnt = 8; 
while(Lfqnt == 8){
Lfqnt = 486310;
}short UNcdbUnCkFqfMpxWxPyklIoGhAAEV = -15558; 
double pQqZPPGX = -117588.3; 
while(pQqZPPGX == 3.073692E-40){
pQqZPPGX = Math.Exp(2);
Console.Write(pQqZPPGX.ToString());
}sbyte dJQXL =  72; 
ulong OiXGTUUOOfXtLFWMGgMMPLlHc = 39018905855794093; 
sbyte KWOSZplKiFOtANdqFcdcdWqD =  99; 
float jSN = -4.184033E+07F; 
int lQSnNwDbdbnFBgBGOLLdMNFk = 102520888; 
while(lQSnNwDbdbnFBgBGOLLdMNFk == 102520888){
lQSnNwDbdbnFBgBGOLLdMNFk += 410284;
}ushort PLCmzpp = 42532; 
uint Tpnp = 8942; 
long ZAUCdzlIp = 31193019635102485; 
ulong mgzwcEKUoWIMusFxWnm = 25467292144862361; 

}
} public class lKjADoyfCdRYuFWLIVNiFlKQDi{public void lpzNDERIGNijeVAVS(){
ushort nzefqNORma = 64573; 
ushort uNlPcqouDWqXAMInfpStnUdgjhZO = 48621; 
int GulPsJKSBhULCOiRbfEg = 13440520; 
if(GulPsJKSBhULCOiRbfEg == 494626){
GulPsJKSBhULCOiRbfEg = GulPsJKSBhULCOiRbfEg + 670001;
}sbyte VqBIYoLEmWoFxyqltjC =  -34; 
string YIixYAuEoKF = "XiHmM"; 
byte FFeQ =  250; 
long POppgMFGt = 34788730907641351; 
int sBhFgzoqxMUSjMMKHnZDTBsQOOjWQ = 889027; 
while(sBhFgzoqxMUSjMMKHnZDTBsQOOjWQ == 889027){
sBhFgzoqxMUSjMMKHnZDTBsQOOjWQ += 373845;
}long iiSSwhUzaoRFDCxIdgSi = 81263800131643919; 
ushort ksaczehLzsysOAtsnUzO = 8384; 
float DosREAUhcatdzEAdq = 2.042136E-12F; 
long LacTVZlftJBsQzNf = 54360025436596180; 
ushort PPRgnOjbRopYJAJcKsSMIfMYdWi = 6749; 
short JIyakynmwNh = 18295; 
long QpVLUgLZDJlJcTc = 20291011850696419; 
long lPyWeETzni = 5709503740601560; 
ulong CuwkdhdcgShPnxJsncyfskOL = 13506520925236507; 
ulong MRtlTXBRqwaYMbDRqIJJxqHLWM = 1273870633943431; 
long YbiAFCTtKoFgqLEnScywkJphX = 86967630050970363; 
long pAidUynaHgTTPIJOaXnHpMaDok = 82242683961576694; 
byte KZDYXIJadyCzQGUuceDFzGP =  175; 
long KmUemWuVebupJ = 42574995871608475; 
long TMBpJpdjTnEXwgZj = 36767275493047425; 
float zgXbuoZOasMHMGaRC = -2.966734E-30F; 
long TBIdljiHTZfwgRXmSUnY = 50281450244260565; 
string MokUFGoXhfRghtH = "FOdFKhSM"; 
double UkdVUJGxnQjllqXUMhhBYnQ = 1.210562E+18; 
while(UkdVUJGxnQjllqXUMhhBYnQ == -9.454827E-07){
UkdVUJGxnQjllqXUMhhBYnQ = Math.Ceiling(Math.Cosh(5));
 bool? zPGupisliJKsLqwGSaA = new bool?();
zPGupisliJKsLqwGSaA = true;
}ulong WOGADzuFPsBuMBFQUtBhxucb = 10033810730224970; 
string laoWAI = "ZoCxYqVUBgN"; 
int tCLETsMUiyzWtFoEBYUT = 453999443; 
while(tCLETsMUiyzWtFoEBYUT == 453999443){
tCLETsMUiyzWtFoEBYUT = 662990;
}float GUkgZtNKoLdgVxQScAFMHpaiT = 1.431938E+23F; 
double jXkAVnnlsXZSFo = 3.745897E+29; 
jXkAVnnlsXZSFo = Math.Ceiling(Math.Asin(0.2));
try{
int eOTHaiCHLDG = 2336188;
if(eOTHaiCHLDG == 10416){
eOTHaiCHLDG++;
}else{
eOTHaiCHLDG--;
}
}catch(Exception ex){

}int EyminKNiyiDdtWMnpjixGSj = 718320300; 
if(EyminKNiyiDdtWMnpjixGSj == 271223){
EyminKNiyiDdtWMnpjixGSj += 966979;
}float iEYnKfC = 1.078307E+14F; 
ushort lmzmRfoSGbXjhMmtyxCzkGZtBwI = 3166; 

}public void OBzZKGX(){
ushort acZfmgltQDxNemi = 63573; 
long mfMxylKRCzkzfpYYxzAe = 78898569111833608; 
long paYYLoNEG = 85765834065113496; 
sbyte gQLAHnuYFRsGohnhjAgGJkST =  58; 
long mtZmKAHDxGCqLpqiNmggmsGYSoCf = 31828465052624622; 
int JgIFRDJhxoscfGFCLV = 2730375; 
while(JgIFRDJhxoscfGFCLV == 2730375){
JgIFRDJhxoscfGFCLV = 519569;
}long OPtuhMEhFazIBco = 87328685375480704; 
ushort mREjnigXsPUOcgGzXaF = 4285; 
string LMKCMNPHCHXPV = "aXpSqKhnwWQehuUlEsDSgRBNyui"; 
ulong mRnlLzZKN = 41122975203531134; 
int MpdFldOiyEuXC = 9506; 
while(MpdFldOiyEuXC == 9506){
MpdFldOiyEuXC += 876717;
}ushort UCRHjFJBNbQPOaXjlbhXoDgsPRS = 28754; 
sbyte iNhARPlYczfJKDFhOwdX =  93; 
uint BHquzStNlqeSdInihYnZKR = 86023924; 
int HcxtlPIbISAuOfJLTsFKBBA = 9087; 
if(HcxtlPIbISAuOfJLTsFKBBA == 544193){
HcxtlPIbISAuOfJLTsFKBBA = 29284;
}long zQL = 4938918269006492; 
double JOjhpYYnnUmfDj = -9.551825E-22; 
if(JOjhpYYnnUmfDj == -9.47925E-39){
JOjhpYYnnUmfDj = Math.Pow(double.MinValue, double.MaxValue);
Console.ReadLine();
}double ZwO = 0.04824609; 
while(ZwO == -2.171891E-05){
ZwO = Math.Sqrt(4);
try{
int IinzMMlCLBnCdwtmeLARDEAtqsHQ = 5777595;
if(IinzMMlCLBnCdwtmeLARDEAtqsHQ == 60081){
IinzMMlCLBnCdwtmeLARDEAtqsHQ++;
}else{
IinzMMlCLBnCdwtmeLARDEAtqsHQ--;
Console.Write(IinzMMlCLBnCdwtmeLARDEAtqsHQ.ToString());
}
}catch(Exception ex){

}
}uint tEfUnpMCYPftPZeudK = 135247460; 
ushort wzuxn = 56472; 
long PIGhgnxKkdGQJbXXoaE = 74516430887956554; 
long ScTRsZyOISRXuzRC = 71195391327627898; 
long VWmXTfPjtaNZqbAjIiMbqwUQITO = 46672154323466342; 
uint KZMijfQPEesamzeD = 55781198; 
uint xpaq = 14036784; 
long iSTWZJYia = 31411149203315736; 
sbyte mLaxUGzdtZuCcyEGLkxbEAVD =  81; 
ushort XwmlWBs = 25906; 
uint xZqBGFj = 33698142; 
string BxsEZelNPzGzyBJgOyaLccyolf = "YVMVAspyYUIXxfHN"; 
long BTaXywiqtQzZdsTFnScEgggd = 64807965138885685; 
ulong LlBbyqMhZAZZRodS = 62598481020230667; 
byte oJNNuhlF =  162; 
long muMR = 88854478938663389; 
ulong HGIQqAEOmbcwlfROlIxOGuY = 70048405094795286; 

}public void aaODEOphB(){
uint KOEVCnX = 18; 
string hlHsefuLmzEQwlsnjysnnhmoBhpEF = "EolbTCWkKjOPkJgXhK"; 
byte OMRNLaZsSXPRgJUoFtsQsYOhAAWEh =  140; 
double tGQSyAqSCFtNwWIDPSt = 2.552599E-13; 
while(tGQSyAqSCFtNwWIDPSt == 1.070582E+22){
double exjoXizCSc = 3.251945E+13;
tGQSyAqSCFtNwWIDPSt = Math.Round(exjoXizCSc ,1,MidpointRounding.ToEven);

}sbyte TzDRicSXMisCmAeKJcoUAyV =  -108; 
float SELdyCXctaETEZxPfVCJKyFKcGFl = -3.60764E-06F; 
int ZkkolJMRFjfefPz = 4638; 
if(ZkkolJMRFjfefPz == 989102){
ZkkolJMRFjfefPz = 559612;
}sbyte uZaaSWMOXy =  -102; 
float PhDyjKdAqBFm = -3.193628E-16F; 
ulong eEVRXQokIUVSwiHaVf = 28300111386098183; 
string xNLHYkSTjdwYSqP = "OldhTEsbOyw"; 
byte cMjNQPUjnUkqOQRLDyQfAeT =  178; 
float SwJSBPJfhqlkbDVgBqT = 2.629946E-31F; 
ulong qlZGenXTzTjWABxt = 1248561035713217; 
byte tFCRdBeU =  87; 
int jERjzNsRGwGdXyiye = 84; 
if(jERjzNsRGwGdXyiye == 337079){
jERjzNsRGwGdXyiye = 873905;
}double qIQKRFiQSkeR = -1.721066E-38; 
double FtUdaTAMXSLVRhfDh = Math.Log(1000, 10);
qIQKRFiQSkeR = FtUdaTAMXSLVRhfDh;
Console.ReadLine();short VITHyxTYpmAgQ = -3259; 
ushort tQYiHOZxRICXeOjjULLYgBzDRf = 54639; 
int geixTpnWj = 14703580; 
if(geixTpnWj == 785484){
geixTpnWj = 798964;
}float UDcpmfNxMTsxsBWujLDtClyWnKX = 4.178491E-16F; 
long JIVloDWUZwZgyVBSmknmdm = 79782273226165774; 
byte bhKOxo =  165; 
float RDXDeaaNS = 7.49205E-13F; 
string HwgDEjwUqRlSep = "NwXExamLE"; 
ulong egoKySeFBnHUMzxsAM = 4118534706280750; 
ulong xuLEMZeoGItuOBPXfCk = 88210564090781392; 
ushort omKdAhgEdhM = 24025; 
sbyte SsqWAVSUVDJFPHMLHxLYpuJ =  -45; 
short JubuAMUVVHCi = -27230; 
uint uqhuVtARtBpIixzJ = 658892848; 
sbyte yTCcWKULf =  -81; 
uint lzWcCWtZiDJT = 8; 
long yYCBXmbBQfykAOM = 80790469680894382; 
string hcVkITSYwn = "JXuZyw"; 

}public void xAiAuRAQl(){
long YcQz = 86106731261984000; 
byte BFCf =  144; 
ushort eiMEfIIeM = 54960; 
int WylqjLFQ = 1059; 
while(WylqjLFQ == 1059){
WylqjLFQ += 935209;
}ushort lYKJlPTUeUfiCbFqGupGomqwZH = 59979; 
double DJikO = 2.054865E+20; 
if(DJikO == 1.5426E+35){
double oFxnoCNEBH = -1534027;
DJikO = Math.Ceiling(oFxnoCNEBH);
int[] oKGUUPDlWXOjQiIxibXb = { 3267798 , 97393 } ;
Random QDsHy= new Random();
Console.WriteLine(oKGUUPDlWXOjQiIxibXb[QDsHy.Next(0,2)]);
}sbyte wiTgSnsmKDKdIABfL =  18; 
double zOIX = 0.003295762; 
if(zOIX == -2.02489E-19){
zOIX = Math.Ceiling(Math.Sinh(-5));
try{
int wqXhdSCNNcfeH = 9788620;
if(wqXhdSCNNcfeH == 54725){
wqXhdSCNNcfeH++;
}else{
wqXhdSCNNcfeH--;
Console.Write(wqXhdSCNNcfeH.ToString());
}
}catch(Exception ex){

}
}ushort HIqcxMJMC = 62537; 
short ewhXkaWdqkWUgWNLAAgCjmoTdeI = -15562; 
int FdXcRtTpsYHt = 410056580; 
if(FdXcRtTpsYHt == 924732){
FdXcRtTpsYHt = 496236;
}double lbROLRgnTgsIzZHTXeftSjhhVRqT = -2.299017E+10; 
if(lbROLRgnTgsIzZHTXeftSjhhVRqT == -4.765546E-18){
lbROLRgnTgsIzZHTXeftSjhhVRqT = Math.Ceiling(Math.Sinh(-5));
try{
int SuHfcUJTTAmfmXkuS = 92464;
if(SuHfcUJTTAmfmXkuS == 48550){
SuHfcUJTTAmfmXkuS++;
}else{
SuHfcUJTTAmfmXkuS--;
}
}catch(Exception ex){

}
}byte wEcdMjLfnB =  222; 
short gWWehPeHxMqPueznGfNDIgtVqtgy = -18244; 
uint IOLwtD = 853551; 
long IYSLn = 6100726289303616; 
short EbN = 18878; 
long wXFKaKJfLjmcyahCjdcb = 71089738371786767; 
ulong JdcymbmbnHxggAhco = 12156887982626310; 
string ThZono = "BjaaNfn"; 
sbyte wGemqFzNfDOKZqxblVuToqiS =  114; 
double coCNOJUqasOPnKljLBACTEYCkUoBd = -3.021514E-21; 
if(coCNOJUqasOPnKljLBACTEYCkUoBd == -1.460195E+08){
coCNOJUqasOPnKljLBACTEYCkUoBd = Math.Ceiling(Math.Sinh(-5));
Console.ReadLine();
}ushort oBCQaalsaWyeoVpEjJBhqGdHtRLg = 18402; 
long usijwz = 55710673965798034; 
string RxouojoGJyVy = "pfdpPdM"; 
sbyte VfeRGUMzxQpAULZpRjdDBY =  15; 
long FhCRRNpkeCjpzFuTjaeh = 17733850787837271; 
int WCxjaMkI = 6662876; 
if(WCxjaMkI == 786513){
WCxjaMkI += 175648;
}ulong tGJ = 56397574541610868; 
ushort TTIHUFITnFIJDwahxIloYGnZV = 58991; 
double WlSZ = 1.946636E+22; 
WlSZ = Math.Ceiling(Math.Atan(-5));
object iTnGIMZIFWbMJVOJR;
iTnGIMZIFWbMJVOJR = -152.918;short eZVHDNOSbbNt = 10572; 
float nasswkqRkmfUQMFHzXIQhzXcX = 1.166826E+08F; 
int LlbdeJCTLUgOhLuxSXgxwowaaK = 4839478; 
while(LlbdeJCTLUgOhLuxSXgxwowaaK == 4839478){
LlbdeJCTLUgOhLuxSXgxwowaaK = 589866;
}ulong AbdRMdcONdBcZOEGeFAgzxQbcuTA = 55686241450886194; 

}public void bldtICqBXLaA(){
long mfHAckcOsj = 49389791831422835; 
byte dtwNQUzBe =  174; 
float SbP = -3.368757E-12F; 
long hqnTUQy = 84812852338828727; 
byte KUJiMCJJpDswlCLeucVPACWtPi =  247; 
byte wMuPVNzbawPFkEmeLdbjRdl =  142; 
ushort hXptEEzUVRYGq = 50022; 
byte tHfj =  71; 
double oeeXaXqARsCGBMHSlgqIx = -1.894722E-32; 
oeeXaXqARsCGBMHSlgqIx = Math.Ceiling(Math.Atan(-5));
 bool? UdXpniZsKciHUtfxxmKqtGJu = new bool?();
UdXpniZsKciHUtfxxmKqtGJu = true;int hTaTohW = 763386; 
if(hTaTohW == 773037){
hTaTohW = 159339;
}ushort zHwwnuElijiuFPMKdQWnDm = 54363; 
string KnHhIjJEt = "QCwoPFIRJgNsGbmtXlfjkQtj"; 
int WeCaBR = 76; 
if(WeCaBR == 656845){
WeCaBR += 517000;
}sbyte UxiVzsIFUnlxzjEZhlMJI =  18; 
short GWgGeeMyfxoPIzLlAqjn = 17863; 
string axWaxPybpgBp = "kbAQDPpbVPsKBBjqua"; 
float DfjidoIJwYIgihayR = -2.813825E+17F; 
double qfHFqWonLWgGPZfSqMmhIHWh = -18.45869; 
while(qfHFqWonLWgGPZfSqMmhIHWh == 1.792742E+16){
double NlaDENttuKbCBuhUGBIzRgKiAtQ = -1.453125E-39;
qfHFqWonLWgGPZfSqMmhIHWh = Math.Floor(NlaDENttuKbCBuhUGBIzRgKiAtQ);
object WRLcOepNYU;
WRLcOepNYU = 5.644598E+20;
}short QoNJFcUNoTKJs = -21073; 
ushort EbIxknmDb = 5433; 
byte yOWUPQQRlKkaM =  203; 
uint CnywzmD = 15897403; 
double CcjIBSB = -3.275996; 
CcjIBSB = Math.Ceiling(Math.Sinh(-5));
short OZYDykJUyQUhReHwNEJubV = -22246; 
short LcMSfhSDioGgNlSqaIXdKouSB = 29384; 
short XKTRJKMiXbsJbFJQaoaq = 20189; 
string mkk = "wITsiTl"; 
int wPycAKtYpnceHER = 9424; 
if(wPycAKtYpnceHER == 888907){
wPycAKtYpnceHER = wPycAKtYpnceHER + 687084;
}int zDpBdKiQPajulSYSfKb = 948034053; 
while(zDpBdKiQPajulSYSfKb == 948034053){
zDpBdKiQPajulSYSfKb += 81004;
}string lWuLPQ = "YBijpoh"; 
uint eyjGtYeLk = 68540268; 
long ACxbDXTwouF = 44631948422648814; 
uint YqUxOmRJBgfCIaIOnMjFSPcFDAB = 798157; 
float UixBjCGPLbMPnzCkybhfaFVciop = 2.107081E-17F; 
long TKkkslBymJWADg = 68096298555256725; 

}
} public class VpnusVlMBZHSfdmiYWEVy{public void ftWhB(){
byte ZIblZoVeShDaXxBWnULTSpgRN =  62; 
float UzjucTBq = 4.142304E+08F; 
sbyte sdgPbUBIGyInPpZKBkAMzLj =  -29; 
double YdWiqIO = 56.16201; 
double FyQWFMUQQ = Math.IEEERemainder(3, 4);
YdWiqIO = FyQWFMUQQ;
Console.ReadKey();uint qQxMWJp = 99346937; 
uint bBghPoInGcjzf = 9022; 
short IzKM = 20720; 
double NGRqBQMWaaGuOo = 6.416608E+10; 
if(NGRqBQMWaaGuOo == -1.196826E+27){
double BbtlZCRIkDKuj = -3.936854E-29;
NGRqBQMWaaGuOo = Math.Ceiling(BbtlZCRIkDKuj);
object qmzfHBCOVRuLYSpwnqpgENwjs;
qmzfHBCOVRuLYSpwnqpgENwjs = -2.05416E-15;
Console.WriteLine(qmzfHBCOVRuLYSpwnqpgENwjs.ToString().ToLower());
}string yWmLuYVgaHlFSnNlJyTtIw = "VdxCcyPpfJKNxhh"; 
long HcYHRkaGzIStGLdbAqIYxSKkqgV = 11941729854899459; 
int ReSwWEQXWAYHLW = 501397045; 
if(ReSwWEQXWAYHLW == 790358){
ReSwWEQXWAYHLW = 355932;
}ulong KoSfhEpGJYz = 13700665676655014; 
byte HXCPunbcyWmKC =  99; 
int RxmHpRZhfoEMjYCzA = 69756645; 
while(RxmHpRZhfoEMjYCzA == 69756645){
RxmHpRZhfoEMjYCzA += 770237;
}short EkblVgSkZK = 22684; 
long ePXtKfLpTAwOkHqllBIX = 10354391828991929; 
uint nJRSQnuX = 66510801; 
int LMGyLBCZOijKosiayAYaPJkNXtQcS = 8994; 
if(LMGyLBCZOijKosiayAYaPJkNXtQcS == 354672){
LMGyLBCZOijKosiayAYaPJkNXtQcS = 32349;
}float zAHiCxfAgDy = -1.840071E-10F; 
uint LDjbCT = 343312; 
long gAnkoWqnJYZqSnmf = 83647171450127232; 
short iZtyYhWJuWRnSGgRKDhtKVPwG = 12667; 
uint DJMBWyOqUlZR = 1809; 
float nZYjlfOkGVh = -5.380644E-23F; 
double oGFfLkpsuSQWzDpYBbzhHMsey = 3.130554; 
oGFfLkpsuSQWzDpYBbzhHMsey = Math.Sqrt(4);
int? NQkDnAJRKw = 1561253;
NQkDnAJRKw += 710;long FSGeJptgih = 46802000017204750; 
string jkQjNisuHkumRwlSQnxaqT = "LhUdCXILJZWetcwqAYZBYNKzZeE"; 
long caw = 14022460859692986; 
double MnUpTQwWWbqsXXyw = -3.251835E-06; 
while(MnUpTQwWWbqsXXyw == 9.590622E+14){
MnUpTQwWWbqsXXyw = Math.Ceiling(Math.Sin(2));
Console.ReadKey();
}uint KWfjgyUnIFeohTkD = 640640; 
byte xjGqXNdcowkhqqP =  1; 
double nNLsXVVhISFkxKDacApO = -1.159769E-10; 
while(nNLsXVVhISFkxKDacApO == 3.610488E+36){
nNLsXVVhISFkxKDacApO = Math.Sqrt(4);
 bool? YkQOuFAqenYlFsznit = new bool?();
YkQOuFAqenYlFsznit = true;
}double jBzxIHFmYUgXTZwmJwUU = 2.770977E+17; 
if(jBzxIHFmYUgXTZwmJwUU == -5.910235E+21){
jBzxIHFmYUgXTZwmJwUU = Math.Ceiling(Math.Atan(-5));
Console.ReadLine();
}double zAscedaDU = -4.26434E+30; 
zAscedaDU = Math.Ceiling(Math.Acos(0));
ulong PTIVQcqdVVcxkfgUuiAWWw = 61033043369470866; 

}public void oXgtuOImPz(){
ulong PiHNnzGIG = 64934188861477185; 
double EYINAeMMgMWK = -9.188047E-27; 
if(EYINAeMMgMWK == 4.21257E-33){
EYINAeMMgMWK = Math.Exp(2);
object zWJNtIkMqqebSIKZUtOMNPU;
zWJNtIkMqqebSIKZUtOMNPU = -7.876489E+11;
}sbyte JOmbCFSnCEiBOUQZKbxWbJp =  -74; 
long nLdlqaOHfXANsJYdyqshEXCPNpDjL = 29410871757577982; 
short hNhOSAWKN = 30469; 
double YSBxpinz = -0.0004651741; 
while(YSBxpinz == 8.205162E-32){
YSBxpinz = Math.Ceiling(Math.Cos(2));
Console.Write(YSBxpinz.ToString());
}uint moOwdHMGCNdS = 992201; 
byte cOcuqPZgcokOYhRRJgLsHlpC =  144; 
ushort PIAsVeeyAgeazpHtPxXHtW = 15726; 
byte oSjRzCRVdkzD =  237; 
short ULqLEgmcZTBfHOsTwpaHm = 26047; 
int Gnf = 935; 
while(Gnf == 935){
Gnf = Gnf + 148126;
}byte JFCGlcQkYXwkgLsdmWIxZep =  25; 
short SXR = -28979; 
long sbKoUKOEyKIejdYKNeDWAROEoxpk = 89870247792883285; 
string jEhe = "oykVlcikMBNzCnfaytEuGpMQMkTIK"; 
double OwSzgdCdibHIuCZqSVdMkFKuZLwjb = -10.15039; 
while(OwSzgdCdibHIuCZqSVdMkFKuZLwjb == 1277.466){
OwSzgdCdibHIuCZqSVdMkFKuZLwjb = Math.Ceiling(Math.Tanh(0.1));
object SSEXIcWg;
SSEXIcWg = -9.488815E+36;
}long mYQAsSYsbhdCTWRMlkdOFxMt = 32510748494471273; 
ushort UjZuoWRZiiAMLpFeGGwXj = 53494; 
byte HfijRyHjfjcHNS =  236; 
sbyte GmEThCtjxYVyAAdWDOiOHhMCpHBn =  115; 
short JoaFEJDAEQawzLMGKYLkdOt = -13363; 
ushort bEBaelelmizVBnanF = 62622; 
int mREaRomhJVVLgbIZYDMOqm = 558711; 
while(mREaRomhJVVLgbIZYDMOqm == 558711){
mREaRomhJVVLgbIZYDMOqm = mREaRomhJVVLgbIZYDMOqm + 438865;
}byte iwJLokZBjPYWDqmOOzGbpblT =  102; 
double aTcQQXTMBcxN = -1.249445E+37; 
while(aTcQQXTMBcxN == -8.962042E+31){
double otaaQiAh = Math.IEEERemainder(3, 4);
aTcQQXTMBcxN = otaaQiAh;
try{
Console.WriteLine(aTcQQXTMBcxN.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}byte HwcDCIJoHhHATLobJmigiilnZxyk =  67; 
int dyt = 603658; 
while(dyt == 603658){
dyt = 827792;
}short tHNfyDRxUOHeOyjO = 4103; 
sbyte ybZhEbAm =  49; 
ulong dsO = 52808598605929558; 
float lytFZxyedHVAtAjeXogHTL = -1.225005E-14F; 
sbyte FmKfodjmVfIUnnsWZLSi =  -22; 
sbyte PbHmjSChcAq =  85; 
byte YqSpgEdydbSoX =  182; 

}public void zXmAWUeJQZuuXFwWxh(){
uint uaT = 5287; 
byte YFwVVQFFCesn =  130; 
float bpMjfSIexgwKiyeXXKDg = -4.997409E-08F; 
string EtaUSeTaiMYDEqgA = "BZOAfwgygwF"; 
double EHlmGNSOmqnuCyACuGDdQF = -2.800217E-16; 
double ocxisqqMtHDmIBfFYIVMP = Math.IEEERemainder(3, 4);
EHlmGNSOmqnuCyACuGDdQF = ocxisqqMtHDmIBfFYIVMP;
try{
int kWmhPYqCJYqenhkdUixnqOPUTZlCZ = 3739793;
if(kWmhPYqCJYqenhkdUixnqOPUTZlCZ == 27791){
kWmhPYqCJYqenhkdUixnqOPUTZlCZ++;
}else{
kWmhPYqCJYqenhkdUixnqOPUTZlCZ--;
Console.Write(kWmhPYqCJYqenhkdUixnqOPUTZlCZ.ToString());
}
}catch(Exception ex){

}ulong QDOMiEfaQZpFPbFPmftoMQp = 56334464443500918; 
short VwjkQFVFAVJFnJGdHTmVBbZ = -17305; 
ulong ALFpCpbQUHwF = 84738630707989290; 
sbyte myc =  -25; 
ushort mUxXwyfkZiutE = 30642; 
ulong IbwMyQOeGumyzhmfTHDajzjZxmLdC = 58541676697897897; 
long yfssbkINSVnBWVPPjSxojfLEsDGk = 31155201621414471; 
float hOPBTi = -1.769938E+19F; 
float yzGiDwOquOaAGxDzJgsnQT = 1.703501E-34F; 
long BJmDpFHUOMNdENVOBdgJ = 17333929842932381; 
double CDzVUeeSzwSLJQMFDG = -2.116878E-37; 
while(CDzVUeeSzwSLJQMFDG == 1.906739E+33){
CDzVUeeSzwSLJQMFDG = Math.Floor(-0.01498812);
object UlIGyxNulPpl;
UlIGyxNulPpl = 206033.5;
Console.WriteLine(UlIGyxNulPpl.ToString().ToLower());
}ushort UoAYMkEhVJaMDCu = 25901; 
byte nScGLRhICcWRJJ =  37; 
float YMkonYbnWuYCwijVQRDunFJQ = -1.490954E+13F; 
uint dasFJMwJ = 14; 
string bsOxjgfTNWSSGACudwkFyQjVbu = "KNCTJQJtVQlHXDIzhhBZijbFHW"; 
ushort kiSuPmxfkdibWg = 40965; 
int OmcQscOCwKAMWlNwliNYRRKgC = 746; 
if(OmcQscOCwKAMWlNwliNYRRKgC == 45888){
OmcQscOCwKAMWlNwliNYRRKgC = 52450;
}int OTJtHSfeSuEXuoR = 624220; 
if(OTJtHSfeSuEXuoR == 879160){
OTJtHSfeSuEXuoR = OTJtHSfeSuEXuoR + 600101;
}double DVneYPdsHVnisdbFpHTYygihYlDS = -1.282431E-31; 
DVneYPdsHVnisdbFpHTYygihYlDS = Math.Ceiling(Math.Cosh(5));
long dFauRLMdubTfN = 44320164392633558; 
string PdOnHXnTZTfAFuQpqgKPlssLYWuA = "iwwbH"; 
byte cxefgiUlIJl =  172; 
ushort NgomMxMdpLE = 51191; 
ulong zKzYiBnUbFIdzfdoKLePwnfOU = 53996904244504736; 
string XknBUbTeLGJXGabbTLYwkFL = "uWnN"; 
long pJws = 2379793833190635; 
ushort lRYZfsJ = 42887; 
ushort DtBkgYOfxJpaDmUXGJBk = 56552; 
string DRDXoIp = "IlOQNEkdkZxMN"; 

}public void koMeiMahkPCjXtEaAzgoIOOygt(){
ushort olxoxKGnljAFtBbYnE = 40438; 
float kBYGykyBnXPpxSZs = -9.50563E+18F; 
short pPoZDWhNPjENEeeLzKZziNX = -17145; 
long WIbHbQsejx = 64747632792991197; 
ushort NqN = 17414; 
float OQEDRcooZALihNwAVJ = -0.006859331F; 
int wzcTSJBGVONzcCB = 59706802; 
while(wzcTSJBGVONzcCB == 59706802){
wzcTSJBGVONzcCB = wzcTSJBGVONzcCB + 530837;
}int DWHGnweOuXoufBXjHOtFWxKLTYodq = 831221683; 
if(DWHGnweOuXoufBXjHOtFWxKLTYodq == 491878){
DWHGnweOuXoufBXjHOtFWxKLTYodq = DWHGnweOuXoufBXjHOtFWxKLTYodq + 499413;
}double cmjQasBNh = -1.403098E-31; 
while(cmjQasBNh == 9.308976E-35){
double fmVMog = -3.144954E-27;
fmVMog = Math.Sqrt(3);
cmjQasBNh = fmVMog;
for( ; ;) {
Console.WriteLine(-0.000116464);
}
}double PDPuu = 2.649666E+22; 
if(PDPuu == 3.277447E-37){
double fbdXaMWFfQ = -0.0160491;
fbdXaMWFfQ = Math.Sqrt(3);
PDPuu = fbdXaMWFfQ;
 bool? HqHnEfZto = new bool?();
HqHnEfZto = true;
}ushort VTkNMouS = 64462; 
float PCYOcRnLmYtYPWqKOnfIDVf = -8.438082E+32F; 
ushort HsZylpxIygbRQjDhkC = 59285; 
ushort wGGFTikUwDRHmItnChItnUDNx = 20814; 
uint OUTbaOlNuTsgkObEjkDCjTKumcDCc = 162972766; 
int lfCJQGzxHUIPBiBSYEkOW = 896673314; 
while(lfCJQGzxHUIPBiBSYEkOW == 896673314){
lfCJQGzxHUIPBiBSYEkOW = 959756;
}byte KCLQxZIBVuGdfEZbGMUcJ =  20; 
short POwkqjbImMWsXYeiSsdjGKQlVhUc = -28263; 
sbyte GlN =  -121; 
long xiMtXRuHHeHILjqafs = 65177241669963673; 
string qfkXwGwiqRITxGndnusZK = "gwWLufaxutNKpBUJmf"; 
ulong KoLlCEc = 72527776561956589; 
ulong Kkp = 55950079365579512; 
byte hlkCVSLKjQdVuPhJKE =  133; 
string DUzaOSxgPnoVyyZUARhe = "BwlmBIgycNl"; 
sbyte GsMBgZnEwDPZXshPkMtZqbAmOmko =  43; 
ulong cQBRIhYKhRNeSxmEo = 59723651983465159; 
float MUPYVJbNjxBBHlcl = -5713488F; 
double QCJVqaUCjKWnPImQsSSw = 3.69293E+14; 
if(QCJVqaUCjKWnPImQsSSw == -8.204662E+20){
QCJVqaUCjKWnPImQsSSw = Math.Ceiling(Math.Tan(1));
int[] DENVIFZcFzk = { 9947302 , 93284 } ;
Random HqhFICOcIMNIneuDMKNPWCXYMFGf= new Random();
Console.WriteLine(DENVIFZcFzk[HqhFICOcIMNIneuDMKNPWCXYMFGf.Next(0,2)]);
}long fPVAYUTGeVHRQ = 24849851233743122; 
ushort eui = 55471; 
string IcLetDzjVESQgLaVfhHWcVDHMJL = "QfQbQKxSVNyDitoUyeFKZjn"; 
string gTMBetLR = "uPORayniZtLeOKHJiHdaOG"; 
float RJQLsRuNLzZAgHehwRkVpgtj = -1.239126E+26F; 
double HRWOYYZsHEQiZYtpLhDDDV = 5.174088E+32; 
while(HRWOYYZsHEQiZYtpLhDDDV == 0.05423381){
HRWOYYZsHEQiZYtpLhDDDV = Math.Ceiling(Math.Tan(1));
try{

}catch(Exception ex){

}
}
}public void CSLPEzooaqUzuqzeZassQ(){
float eLUzQmHfNEg = 2.957215E+31F; 
byte xJJXgppddckyTApZTI =  133; 
int Kazc = 2194; 
while(Kazc == 2194){
Kazc = Kazc + 469715;
}int uEJKTMbPHRwGPNHyEmkuFBNJjU = 9006814; 
while(uEJKTMbPHRwGPNHyEmkuFBNJjU == 9006814){
uEJKTMbPHRwGPNHyEmkuFBNJjU += 583806;
}long AVHpZWARxSFSmHFKs = 50616593917674450; 
double WGzbRYmnMRmnnUNeUeqIfBzRPpAmG = -4.466938E+08; 
WGzbRYmnMRmnnUNeUeqIfBzRPpAmG = Math.Sqrt(4);
Console.ReadLine();long FLJfPCVjjmpRzCPDmjyRAebIb = 89831673523528293; 
short XaYqD = -7086; 
ushort jLUN = 8144; 
float wNERsAKbewFRHBXHJixRCnSVxecS = 5.685058E-27F; 
long kGmCeqYFnZUWXEOCldkFKbwhV = 12024941192813934; 
ulong amxDqkREDuVXTqBGTXc = 27363057423244842; 
sbyte UYikXOnQCTuSzS =  64; 
double CWWlkZKGxPhPfwAKgJxNN = 27423.45; 
if(CWWlkZKGxPhPfwAKgJxNN == 4.558473E-12){
double tsIMuPkkJYzFCYgaxzUiqj = -3.057227E+13;
CWWlkZKGxPhPfwAKgJxNN = Math.Floor(tsIMuPkkJYzFCYgaxzUiqj);
int[] tLwTWbqLxPhtbLzWayni = { 5129129 , 78699 } ;
Random FfQWGPfujbwPOw= new Random();
Console.WriteLine(tLwTWbqLxPhtbLzWayni[FfQWGPfujbwPOw.Next(0,2)]);
}ulong ymQC = 1130619453310418; 
float PlPGGXplkqmONEnoXhkuQVFcOXTiG = -3.71861E+36F; 
string xaCHVj = "MlcAoEfAiUNmYziTIMSIdhHQsm"; 
int EMIcYsJFTfN = 84; 
while(EMIcYsJFTfN == 84){
EMIcYsJFTfN += 663404;
}ulong Jmg = 30754332954044205; 
sbyte XOWy =  -67; 
long HQFaCZIPLh = 36251102229996973; 
ulong xEdZHYLleyEzXzuSIgtVwNdyKeDRa = 11489035678671918; 
int EnxLmEyjgRfwo = 838933; 
if(EnxLmEyjgRfwo == 485579){
EnxLmEyjgRfwo = EnxLmEyjgRfwo + 619628;
}short xTluXXZlHqfcNPHZc = 21477; 
float VwaVokhIi = -1397365F; 
sbyte ShujqpHyXBA =  -128; 
ushort JjEHINdGaTqfNPqdy = 48924; 
double VImPLCmPYUBUVJzyFCWullg = 2.532333E+27; 
if(VImPLCmPYUBUVJzyFCWullg == 3.888419E+24){
VImPLCmPYUBUVJzyFCWullg = Math.Pow(2, 2.1);
 bool? AYwJdSOTocyAPGPUXFaiybzBlwg = new bool?();
AYwJdSOTocyAPGPUXFaiybzBlwg = true;
}ushort LSxjfWlViHPqaNFUhwIR = 48222; 
ulong MDRbMwYHjjVENfKWWiOhuVlfNWE = 82566131956053212; 
long VBBkZGheiS = 8614235567635245; 
ulong LnPRJDWEZJP = 44218610688919517; 
short aEePS = 7310; 
uint XlyVsEHMJDyJEfBctwSJbMMfAlLK = 4125499; 
ulong uHbysAKduVFuWIBUdcxyOTWlAqwf = 41469737133145273; 

}
} 
public class EFwSCGMDJknWmjPnjEzBuNQcJ{public void IJCJySIglVuDW(){
sbyte AGeITmGDKwSwZyQOsjhYdaZOxngY =  100; 
float IqDVhMwPfbfRnFxIJwg = -2.088286E-10F; 
ulong UzGpkGg = 54219333696926711; 
long WucX = 44178910622912395; 
ulong mTjJyDwjjZtybAtskhNdgYMkW = 15693255577032982; 
double sUOefHMkcEBhlQHup = 3.421586E+14; 
sUOefHMkcEBhlQHup = Math.Floor(1.085488E+18);
for( ; ;) {
Console.WriteLine(-1576980);
}ulong qAawCwIggXXMNFMTzuswQqEqBYI = 6127418262814464; 
ulong EyZ = 55342523157938216; 
long LLnqfZWJUD = 37335968100406908; 
int RVWtyaAgBBVLTQozwyxbUtJUlTN = 65807464; 
if(RVWtyaAgBBVLTQozwyxbUtJUlTN == 854665){
RVWtyaAgBBVLTQozwyxbUtJUlTN += 374940;
}ulong POVTfAPhuIclmRp = 73184336664819819; 
sbyte mOzYxcqgsAKAEqCeko =  -69; 
sbyte BUsKEdRLZELOoSMpjXGM =  -104; 
float GtWyaXmXQYWM = 3.941171E-29F; 
short YAxtWYMnzQOaqHyDGDNpIGRNMH = 26451; 
ulong fRjVeHlJCwHI = 37558889412417740; 
ushort suYksJ = 44306; 
int dVVsYFemZk = 80375786; 
while(dVVsYFemZk == 80375786){
dVVsYFemZk += 993287;
}int EAbLoYZAQXTnEZUgDMN = 23536353; 
if(EAbLoYZAQXTnEZUgDMN == 358693){
EAbLoYZAQXTnEZUgDMN += 187417;
}short cknolMcfhnsHXszO = -23476; 
ushort IUjaDWmZuYsJCZzXoCJIDB = 57979; 
ulong jfPnNRZYyIHTjLsYMJhPFRHAwj = 1593250622944189; 
short yMHhtKdsReeQpJQSMYmtApTEE = 1052; 
byte BHxcsCCaca =  107; 
float nIpfyidtomyUZsgSdESyPRj = 7.455821E-36F; 
ushort hQQMDsGkJDygsiUltZhtjuhY = 28570; 
int XxyJdZuWEzCVDug = 304688; 
if(XxyJdZuWEzCVDug == 502964){
XxyJdZuWEzCVDug = 292153;
}float RJVeFNIRoYUIOkUugXl = -1.27004E+19F; 
float FYcWS = -2.343325E+37F; 
string cRHZiVP = "VIlWIMs"; 
byte lpDcESYFpfDRQz =  76; 
float FXFUklhdwfGhXlKJxgKBfA = 1.050798E+12F; 
ushort PmqfiWaVhwJBaduZiNtxtTSitq = 39753; 
ushort fYWBtyVNoWKhbeoaVgH = 7971; 
sbyte loYAVwiNSqH =  -55; 

}public void yNaI(){
int mTQJoqPEizkZnBPM = 54136196; 
if(mTQJoqPEizkZnBPM == 496523){
mTQJoqPEizkZnBPM = 576193;
}uint sBNcS = 8923; 
uint HTywGJigSaoHssOnLfzPeZJqMqOFN = 90036998; 
int ZIHqqaTKwIEmyahYOghwKV = 20360891; 
while(ZIHqqaTKwIEmyahYOghwKV == 20360891){
ZIHqqaTKwIEmyahYOghwKV = ZIHqqaTKwIEmyahYOghwKV + 888794;
}int gfLUtLhWufGeK = 960; 
while(gfLUtLhWufGeK == 960){
gfLUtLhWufGeK += 618933;
}int FNhcdaxkILZgbCzDEIyRSNcQGkofc = 981707; 
while(FNhcdaxkILZgbCzDEIyRSNcQGkofc == 981707){
FNhcdaxkILZgbCzDEIyRSNcQGkofc = FNhcdaxkILZgbCzDEIyRSNcQGkofc + 545550;
}short SoLLMMuGXJRHCLJiQg = 21303; 
long yTMqhlMLjHZg = 28098351321763082; 
byte DzACLpegnMIluwYuEBIU =  194; 
string BDZoxKaQchQqnSHCwHdsnzNqX = "MDBaoUAHZYMaIxmVGeFeKDW"; 
double nKKEbQIsJ = 0.001807792; 
if(nKKEbQIsJ == -2.918631E+26){
nKKEbQIsJ = Math.Ceiling(Math.Tan(1));
for( ; ;) {
Console.WriteLine(2.505707E+09);
}
}string VBNMZipMpxcHbBtuXLg = "QQbgjuKX"; 
float xXkxOOxfqVo = 3.473906E+24F; 
long SDmztofGRQezGCFzsSYZfwGMFX = 78182331706676118; 
short xzBbqsuHKoQEMxWJTMOyXCTwy = 13446; 
ushort BfiqjNbUqWJObZcpgnJPZHfEAVU = 59778; 
int FGXzf = 102589; 
while(FGXzf == 102589){
FGXzf = 410008;
}string WcawYAEnicFW = "aFSoTfolqcQDTzkIUJtWojCAAV"; 
ushort RxE = 23578; 
int DBHffoOfF = 203761530; 
if(DBHffoOfF == 5340){
DBHffoOfF += 344443;
}int tkZFRdtPXK = 3870; 
if(tkZFRdtPXK == 446751){
tkZFRdtPXK = 1496;
}long LbPEeijFDnHQxKlhkhDIMxnpiGB = 79446939270144392; 
int QgRwlCeigmEhNnpy = 19; 
while(QgRwlCeigmEhNnpy == 19){
QgRwlCeigmEhNnpy = QgRwlCeigmEhNnpy + 402369;
}uint zVcBOtqcIIwaLlBVigbs = 565132388; 
double ywyVsQbwJ = -1.604063E+33; 
if(ywyVsQbwJ == -16808.24){
ywyVsQbwJ = Math.Truncate(ywyVsQbwJ);
Console.ReadKey();
}float zVJmkMG = 4.575339E+36F; 
ulong nTN = 30397702262017460; 
sbyte kMbleVUnjgjFuFnd =  -111; 
int jijHjUXJMEySZMibAKtSdUNeMEB = 8436; 
if(jijHjUXJMEySZMibAKtSdUNeMEB == 318273){
jijHjUXJMEySZMibAKtSdUNeMEB = 778147;
}int pbfSsgliKMokSlDPuLLRpwfUHuKRc = 136160557; 
while(pbfSsgliKMokSlDPuLLRpwfUHuKRc == 136160557){
pbfSsgliKMokSlDPuLLRpwfUHuKRc = 863077;
}string RbLRMdsysJcoU = "GdNJUWkl"; 
double wfKGkQetDwDJZaF = -4.372763E+19; 
while(wfKGkQetDwDJZaF == -6.629324E+16){
wfKGkQetDwDJZaF = Math.Pow(5, 2);
Console.Write(wfKGkQetDwDJZaF.ToString());
}int EleScp = 50052069; 
while(EleScp == 50052069){
EleScp += 966224;
}int oPoaqCskqGJDwxmELTTOQwNFCs = 16081737; 
if(oPoaqCskqGJDwxmELTTOQwNFCs == 498278){
oPoaqCskqGJDwxmELTTOQwNFCs += 321069;
}long wCdiPbPtVzjAU = 32431115137879327; 

}public void cKzZRjBkfsSaaLACdRZxAnwbys(){
double EplVsCAtLPjFTLsRIUqCmbfnQHuO = -1.334203E-05; 
while(EplVsCAtLPjFTLsRIUqCmbfnQHuO == -7.457185E-19){
double WJAlfRoI = 3.005753E+10;
EplVsCAtLPjFTLsRIUqCmbfnQHuO = WJAlfRoI / 3;
object FuEbuKCkYfLhMenmLppcDLLlRogk;
FuEbuKCkYfLhMenmLppcDLLlRogk = -3248.31;
Console.WriteLine(FuEbuKCkYfLhMenmLppcDLLlRogk.ToString().ToLower());
}long jeDqusLIDTuk = 1728542793706811; 
short PfIPKpRSAQtHmPpP = -19461; 
float oPHHsN = 1.299608E+13F; 
ulong wsQdLpxKFJwQLuESIZHhhOLO = 86336812020999401; 
string dzEE = "ATfGhLAelcAfKcHIH"; 
int ebgX = 5425; 
if(ebgX == 745433){
ebgX = ebgX + 644076;
}short odygn = -29729; 
sbyte cAGWxPXpExwxRBACsV =  55; 
string WNyWXkbmVsFAdZFx = "BFGtjplT"; 
byte FWPsqddfNhaAFVdJeQXOBUe =  36; 
ushort zgMxkc = 44446; 
ushort pgZnwaUSnkbaKoC = 15186; 
ushort AmKRHXkGXtpWYQhyNCeDCOHcRXtg = 50565; 
float jQfijOzA = -7.157623E-34F; 
ulong BCql = 13441056815116218; 
ulong HaKFdRfifgSHYKnbcAM = 16170629936699357; 
short cXMyEhzP = -3091; 
double WqwkuFTEnYetJaAeBZxPpRZdigjwC = 4.059063E-21; 
while(WqwkuFTEnYetJaAeBZxPpRZdigjwC == -1.046942E+18){
WqwkuFTEnYetJaAeBZxPpRZdigjwC = Math.Pow(5, 2);

}short RxEofIyOmcjXXEzz = 13688; 
sbyte weOgjQp =  -49; 
int ykHgCHpWE = 58771812; 
while(ykHgCHpWE == 58771812){
ykHgCHpWE += 631413;
}long wmoHUVEWwdpuAP = 58176554601364393; 
double DgsFjUQ = -0.2866755; 
while(DgsFjUQ == 2.541111E-33){
DgsFjUQ = Math.Ceiling(Math.Sinh(-5));
for( ; ;) {
Console.WriteLine(1.480431E-29);
}
}double YXcLjHUoFfzpXCxqyOwhJb = 2.724872E-11; 
if(YXcLjHUoFfzpXCxqyOwhJb == -2.119293E-09){
double NPkOkCfExEqMejKi = -0.0004044381;
YXcLjHUoFfzpXCxqyOwhJb = NPkOkCfExEqMejKi / 3;
Console.ReadLine();
}double RkRyFjN = -4.366963E+31; 
while(RkRyFjN == -3.765025E-28){
RkRyFjN = Math.Pow(5, 2);
for( ; ;) {
Console.WriteLine(4.813377E+30);
}
}long INYVgiSNwIOzBMfZSkGWyBdqRn = 8150028000176960; 
string LCqJxVwxDFWLbc = "MCP"; 
ushort KIeTXz = 5614; 
double cun = 3.141831E+13; 
if(cun == -2.700545E+28){
cun = Math.Ceiling(Math.Sinh(-5));
Console.WriteLine(cun.ToString());
}float wgAFfTfyTWlUwjDLK = -1.011294E-18F; 
string DaRLcFZVRLiFWqRsRpHe = "gxCPxwNi"; 
short GRTTfIUcogCblcpzTMtTUIWyw = 5636; 
short HimMzTDNdyLKXnsYJejAAXDbQzX = 31118; 
float yCsYxfUQDtADww = -2.778487E+14F; 

}public void HoYOquQxJHkWZVFTJZjqYSleTCR(){
double DgafDpJWHggDDNzJTEyVKNxRsXPc = 3.758944E+30; 
DgafDpJWHggDDNzJTEyVKNxRsXPc = Math.Ceiling(Math.Sin(2));
object aYudmQLjNGCHcMq;
aYudmQLjNGCHcMq = -7.19801E-33;
Console.WriteLine(aYudmQLjNGCHcMq.ToString().ToLower());ulong pPZdRdoUo = 13149846652296593; 
float cBehxieRqGSAuXRqi = -1.026764E+36F; 
short MSxbpBhmBAWZILMlIEUNlXnx = -29202; 
string CgGzY = "NqIIgfJWFQfVANmfeB"; 
float CDOFZ = 6.312578E-21F; 
ushort fUaYxVtdFCylRIghCDq = 56252; 
ushort eET = 2357; 
byte dIhUmpzWfXdSTbBYyT =  180; 
float xzwVDidPxeuLQaWuEYBi = -2.766279E-35F; 
int AZwCfgaqoWHiNYL = 40; 
while(AZwCfgaqoWHiNYL == 40){
AZwCfgaqoWHiNYL = AZwCfgaqoWHiNYL + 692524;
}sbyte aFFU =  101; 
byte xtUWxEODJpecwUtNHFnuNe =  97; 
long sWuWCmfgEBnoQDgtSysOHLiI = 33125287269142613; 
sbyte dHtGhd =  123; 
double ufzRcgaVHbXhIONfEXzqFinfJz = 6.948666E+34; 
if(ufzRcgaVHbXhIONfEXzqFinfJz == -1.149751E-17){
ufzRcgaVHbXhIONfEXzqFinfJz = Math.Exp(2);

}ulong DzBDaHWHEVpjxCOAdfMfgdS = 1414264017798814; 
string QgCzfShALIhmzzTYIpxqIGAHw = "nIVdPjn"; 
short umbYmFHwxLSWnecVzoL = -19791; 
byte UcXFAEISYYfSALHqiNak =  14; 
double lZEThqUEhwMsYbJec = 1131195; 
double ggYjZFYBLFWK = -3.999976E+07;
lZEThqUEhwMsYbJec = Math.Ceiling(ggYjZFYBLFWK);
float bYhIqpQZbG = 5.009701E-23F; 
double zFixCkDFeDltho = 1.527065E+26; 
if(zFixCkDFeDltho == 5.780817E+09){
zFixCkDFeDltho = Math.Floor(-2.234334E-35);
int? KnGMyTGwYiBljFu = 3114397;
KnGMyTGwYiBljFu += 3273;
}short pGlMihIOIHSUQDmTZec = -28770; 
float dEy = -1.075457E-34F; 
sbyte ndCIPYPpYZwaSjVYHgVTEqk =  123; 
byte oCInXeCbuDpmbEyEafHcYRh =  152; 
float hmjWT = 6.020631E-09F; 
string mIKbqARQtJufqxZpfskQy = "cyTWoSzJNRbTj"; 
double dpDhbOWxZQQ = -1.618511E+08; 
dpDhbOWxZQQ = Math.Exp(2);
try{
int zISWDHR = 2412210;
if(zISWDHR == 47666){
zISWDHR++;
}else{
zISWDHR--;
Console.Write(zISWDHR.ToString());
}
}catch(Exception ex){

}byte nMqzOsWmQU =  190; 
double fozNDf = -1.448637E+37; 
fozNDf = Math.Ceiling(Math.Cos(2));
try{
int pIXIDLuLEd = 9675696;
if(pIXIDLuLEd == 84149){
pIXIDLuLEd++;
}else{
pIXIDLuLEd--;
}
}catch(Exception ex){

}double PVbBxIHDmykqkigDIqQjQuzRqiQb = 4.393297E+18; 
PVbBxIHDmykqkigDIqQjQuzRqiQb = Math.Ceiling(Math.Asin(0.2));
try{
int EAOfIGnITFKiLpXEZuT = 557592;
if(EAOfIGnITFKiLpXEZuT == 46243){
EAOfIGnITFKiLpXEZuT++;
}else{
EAOfIGnITFKiLpXEZuT--;
}
}catch(Exception ex){

}float PTWXgMpKucQq = 8.784211E+27F; 
string ySkQwKnnJMbOiNFwMstStZymd = "NoAAfkP"; 

}public void dkmdXeFJnwLCBIyuGGA(){
byte zwXMlXZPjHRYEKui =  167; 
ushort mLLbDCJfRe = 58024; 
uint EfFPoqMgGTeOUEWXdlXkkZ = 75623099; 
ushort jDSiKpgewzeFT = 20868; 
long HRHmJY = 66426261201241318; 
short wUhioyiPIPhzSAsHDcsXgfMkQ = 13101; 
ushort tRERgniZiASjIRGUQnpnCpNzl = 24566; 
long iWsFbswQkqq = 2346028223742864; 
short RwDuhpcefFNDTQmh = -14827; 
sbyte xPKAWLYhSLn =  -108; 
string PqCQjUbsMfMEbx = "kVWaZTMhVHJjhkuSB"; 
long eAMqen = 40793560717186742; 
int PqddCciGjBLUhsmlUUsd = 572080; 
if(PqddCciGjBLUhsmlUUsd == 727543){
PqddCciGjBLUhsmlUUsd = PqddCciGjBLUhsmlUUsd + 813511;
}long inZJDEFBxGGbpfhEDu = 47309233540519443; 
sbyte AqsMuFxVC =  -116; 
short NNPonqNtxnjsVYmIRi = -197; 
string safmUIMKlbBdqX = "MiDRPOQkhaQwsPesDo"; 
sbyte MKbtKbKjmKoEa =  44; 
double HJmIDPXmdluVhJXOjDSb = -1.877819E-26; 
if(HJmIDPXmdluVhJXOjDSb == 6.437939E-33){
HJmIDPXmdluVhJXOjDSb = Math.Ceiling(Math.Cosh(5));
object BzEXRBCRRLbLYdLK;
BzEXRBCRRLbLYdLK = 1.049801E+29;
}uint WNsGJRKNWEiGQsouwPq = 879911115; 
float PLJg = 2.675102E+24F; 
ulong eudjD = 17624580276547313; 
ulong ElHBKxWBNcKDUZKWWgAd = 31014658269693541; 
ulong YIbdeVhpeIiUqhSJVcKmn = 66600369730101289; 
string ZKddcCRUPOGbRVpuaUALL = "EwjMqImSn"; 
double Yik = -1.174422E-32; 
Yik = Math.Ceiling(Math.Asin(0.2));
 bool? gWetHDt = new bool?();
gWetHDt = true;long wdFkAVVAWXaLldapgVANxtbWwJKIS = 1109985154721522; 
long zLcxhMMwzCzItGUEccJ = 3155730758221130; 
short NYOxpbbtWUjJn = -17455; 
sbyte BTXzonZTVPmzw =  44; 
ushort ubLEKldFTkf = 11168; 
uint CYgnmS = 874054; 
string YVBBOEXmzUomBEK = "PtGeiQuPmn"; 
short xEQBXmF = 19459; 
long QomSLFRozDEXcfnap = 29304426957944495; 

}
} public class dOonYUEmEqRqfQXYQPEzBLmXXxo{public void opcybQBdZjEKtQDWiZcLOoKylsutB(){
short DFFXnnupooU = 31027; 
int TfKhpOYt = 208794; 
while(TfKhpOYt == 208794){
TfKhpOYt = 205477;
}long hDHTbeOToJOJNw = 85517980753987783; 
byte tjauXIJjHThsqNjOqR =  222; 
long oGjqKCsSxMWscAReuVHZFcUnCZpcE = 18567178899927278; 
string fpsjhkZnn = "KWVlJwlMVPLl"; 
short BTFQAwnnqI = 1355; 
long TNcCLcLUZ = 73855479422301048; 
int bdJoGjPSdxGmLZitZTmZiFuCVAV = 5571; 
while(bdJoGjPSdxGmLZitZTmZiFuCVAV == 5571){
bdJoGjPSdxGmLZitZTmZiFuCVAV = 102581;
}int ijzxqVjHzuKtUZATbaEL = 687682; 
while(ijzxqVjHzuKtUZATbaEL == 687682){
ijzxqVjHzuKtUZATbaEL = 279529;
}uint IdDhesZiBxV = 97; 
double hkgbpdDpcCVkAceqcxUZuRDoDap = -4.145196E+07; 
if(hkgbpdDpcCVkAceqcxUZuRDoDap == -3.370931E+16){
double ZmileNAG = -1.331762E+19;
ZmileNAG = Math.Sqrt(3);
hkgbpdDpcCVkAceqcxUZuRDoDap = ZmileNAG;
object xfSIeYCp;
xfSIeYCp = -2.6333E-28;
}uint VAmSICmJ = 164644; 
ulong zUbCDZwPcAPctweTYMLePEolLk = 24137634563522850; 
byte GQt =  30; 
double gxLjzmxtKOyJdUFQRszkpI = -3.899303E+09; 
gxLjzmxtKOyJdUFQRszkpI = Math.Floor(-1.44246E-18);
try{

}catch(Exception ex){

}ushort opGjykWtVGGUJzipXqIXAy = 20866; 
byte wZimSQsTK =  130; 
long ypkDahFUhJItfW = 78444178178402383; 
string OfdQnbZXB = "AayuzoMteMsQhWEatyJJdBnYPV"; 
string GgKKSlxlUfODmZSpUXXhmmI = "nRQYNzGEjYNYcIXWA"; 
short nJgwdJ = 30926; 
string XXCszgwaxzAwAksomUmmabIVg = "UhwMpOuPGmxuA"; 
double XBbTLOyOg = 7.752052E+20; 
XBbTLOyOg = Math.Ceiling(Math.Tan(1));
try{
int qbcCHcxGxuoUCc = 3778595;
if(qbcCHcxGxuoUCc == 38370){
qbcCHcxGxuoUCc++;
}else{
qbcCHcxGxuoUCc--;
}
}catch(Exception ex){

}float uJFPzRTpPHJzNUhbKZJdKLO = 1.451062E+19F; 
int MaWhJMjpoluWRecalA = 2997; 
if(MaWhJMjpoluWRecalA == 234762){
MaWhJMjpoluWRecalA = 266896;
}ushort OdKUKbVnhoTzZMEFTQpdRZ = 42289; 
long wCnGkPJOXYfHuCsbE = 30148087692809870; 
long XpUOxZsoBIJXAj = 38454686822075724; 
string cTDOWcClD = "LydjJHbmRtVY"; 
byte dyLmujhYnHMDxFsyfxwgzmYEPqxa =  34; 
long eGFLAsoRtkN = 7384715565565799; 
byte EgzRzgydOgRGoHXS =  221; 
float RFcWM = -892.4713F; 
sbyte gYgNuSCzfUakROFeVEhRi =  -38; 

}public void boyUCkMNWbPWPaekyf(){
uint ZSUZQySRAunBOtezXXZntg = 18; 
string QdQsKMKHGiuyddsVmCNnhwVHn = "flsSOLXYIObhLDRh"; 
ulong eATVVzTDxOKuijdiGynNwJBfxFTf = 88023989794103946; 
long pTYkoion = 36155596677383169; 
uint ZOApGOMkqKMjIo = 419056; 
sbyte PktqQiZcboodf =  32; 
uint PULank = 88892; 
float CwiPaBlAlpbIXpqDNyV = -511281.6F; 
sbyte CAILggEMoAlIETlnOSHuhe =  -90; 
double JMyaiFawKIotQ = -4.778158E-11; 
while(JMyaiFawKIotQ == 1.430768E+18){
JMyaiFawKIotQ = Math.Ceiling(Math.Tanh(0.1));
object MEsalNVcWQb;
MEsalNVcWQb = 1.046704E+36;
Console.WriteLine(MEsalNVcWQb.ToString().ToLower());
}int BwTzFoiIK = 21569845; 
while(BwTzFoiIK == 21569845){
BwTzFoiIK = 558161;
}int jVeMgDPZZlzg = 2068; 
if(jVeMgDPZZlzg == 766198){
jVeMgDPZZlzg = jVeMgDPZZlzg + 708676;
}short EsBS = 1422; 
ushort dgGjWdGsWCAtTezFdKCuOp = 24029; 
int lBPSwlkNRLTubCcKTPjnqzX = 75; 
while(lBPSwlkNRLTubCcKTPjnqzX == 75){
lBPSwlkNRLTubCcKTPjnqzX += 10447;
}double ObxLERAConHbtGkcizKVBjp = -2.174824E+18; 
while(ObxLERAConHbtGkcizKVBjp == 8.186349E-33){
double UWetGQzTtnUZKyFBDuZCqcn = 2.292522E-32;
ObxLERAConHbtGkcizKVBjp = Math.Round(UWetGQzTtnUZKyFBDuZCqcn);
int[] bhwDkdVgyBRxsZQClppNpXhCEeKwS = { 2809848 , 39608 } ;
Random BIHSyLXLJWPpgZLXIDBBb= new Random();
Console.WriteLine(bhwDkdVgyBRxsZQClppNpXhCEeKwS[BIHSyLXLJWPpgZLXIDBBb.Next(0,2)]);
}byte daTZgtAoPajQADfFBHuYZ =  210; 
sbyte HyhVsHlmujtLdMDDoasZChhZw =  105; 
uint anJmSctKlFXiPWwpjuBP = 83; 
long zipzohAxfEmgBfSbtTpKdjkFMRQaw = 40178270682469552; 
float efCckKoQdCAwl = 2.35625E+35F; 
double XLNysAbtbd = 1.010497E+23; 
while(XLNysAbtbd == -2.250354E-18){
double DgaVxbMzVpqIHOAMcgW = -0.0008648096;
XLNysAbtbd = Math.Round(DgaVxbMzVpqIHOAMcgW);
Console.ReadLine();
}long EsmwsPmQBgFuH = 3376733848625631; 
byte wqLfAJuGVwIgdIHBJYDcsuJQ =  89; 
sbyte efjKRtDSeHJOgfCJYYsflqum =  -120; 
string zaIUOsU = "AMMRNCQCTa"; 
byte WQMKRZbDOMXIQl =  21; 
float NUwmRcXlgGEwPC = -3.919134E+25F; 
int InXcOqqC = 78064621; 
while(InXcOqqC == 78064621){
InXcOqqC += 440024;
}double gfPif = 6.279414E+20; 
while(gfPif == 1.463349E-09){
double KRkaxDzKge = -1.59128E-28;
KRkaxDzKge = Math.Sqrt(3);
gfPif = KRkaxDzKge;
try{
Console.WriteLine(gfPif.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}ushort KmQAGoFppLpQDOOUDcYpMkMNLeVnD = 8759; 
string Xhtj = "uUjOdEzlNC"; 
uint HytI = 25; 
uint dOCtpNiKnTwwFGhh = 54844746; 
long nBYDylWKIuRUUJlHtRyq = 69687791842119452; 

}public void kTEFRmDPYWe(){
float bUbBCWtCOXeqnADd = -2.998061E-11F; 
ulong uyBmpdwJRwUsBUNeSGDCIOxVAnD = 57665906900128752; 
int PkzBLKtdNRuM = 223395; 
if(PkzBLKtdNRuM == 227882){
PkzBLKtdNRuM = PkzBLKtdNRuM + 239452;
}int FOb = 652703; 
if(FOb == 880152){
FOb += 320187;
}string OhLdwebXedRMyibBAL = "fKBFObiXSaEANMnGFOhwNk"; 
short WNbFsHiGiFVTnEXcl = -23812; 
sbyte BFeaiDxNqoZVwYIaZFFlSYpiQusb =  80; 
sbyte GbsasJaw =  78; 
byte nUFayoVWcIDXeD =  69; 
double OkwXlfuU = -7.47678E+34; 
if(OkwXlfuU == -8.713302E+35){
OkwXlfuU = Math.Pow(double.MinValue, double.MaxValue);
int? PzcPjfwsHFdANVITSqtR = 2042853;
PzcPjfwsHFdANVITSqtR += 57520;
}sbyte imYHIuKCKAoZcpiHG =  -67; 
ushort PyBwVkTpHTwbVXE = 43611; 
long jWKpyPOhC = 17765714476223152; 
ulong DMxBtURdneeMBYtcAOAZMH = 42320283601721623; 
byte SWfBOwpwH =  180; 
uint xogj = 918412; 
long mNaVYNJKwSHuTTUESSndzdtagKmD = 15994445865144957; 
double DbNGaOeg = -622966.4; 
while(DbNGaOeg == 4.850445E-14){
DbNGaOeg = Math.Ceiling(Math.Sinh(-5));
try{
Console.WriteLine(DbNGaOeg.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}ulong gfEjAPdcNlbLKuikWaTZBfoxBwjMX = 38346583652817340; 
ulong QHFgysJElWEhOWRy = 48750384141243148; 
double DaadtufBdsVYReRSYPu = 9.143143E+26; 
while(DaadtufBdsVYReRSYPu == 1.29389E-21){
DaadtufBdsVYReRSYPu = Math.Ceiling(Math.Tanh(0.1));
object cmTACcacS;
cmTACcacS = 1.42645E-27;
}string cwNYoipQlkHGJuKmXGolCNPZeO = "OuhydiZxgobOKVSpA"; 
int sMeBzf = 84538932; 
while(sMeBzf == 84538932){
sMeBzf += 761774;
}float GCweOExKYEXVSFqJ = -1.377169E-16F; 
ushort xRKUKquqIfBCimdtylMbLINCDDne = 33528; 
long ptVdPSBwyifbTcxNFCYljEtmPdh = 89365263126234093; 
sbyte joCXRnSFG =  99; 
uint QKEDxL = 99296840; 
long LKhouTpcIT = 21238624471859604; 
uint iiKlpsNnDByiFUFRPDUZo = 241679; 
int poqfhsxzZgCaWYbgcBhwdWAIGB = 416959121; 
if(poqfhsxzZgCaWYbgcBhwdWAIGB == 598427){
poqfhsxzZgCaWYbgcBhwdWAIGB = 937583;
}string acwhwhbLFOcQ = "ESSmdejQtQKAIDaXGCFXFXYLU"; 
sbyte utqMFQIlEJhCSUDHmMJNGaNZlyTnG =  93; 
sbyte XeQxcdeKZpyn =  -12; 
double ccqLZlsuuGsAgXuGLCPENdf = 2.814173E-25; 
while(ccqLZlsuuGsAgXuGLCPENdf == 5.690056E+20){
ccqLZlsuuGsAgXuGLCPENdf = Math.Ceiling(Math.Asin(0.2));
int? ZZWNCsmEeRfRmxh = 6026154;
ZZWNCsmEeRfRmxh += 67816;
}
}public void iGQNSRLRo(){
uint QUbGFLTsCFwtjuayXqJ = 930310061; 
long wAbtYwLqUOWDOWxdEamhJOlGsSuOB = 50444232840085266; 
short WRnCFnRlBBmfSJCqImwafaWRnem = -5734; 
float oBleppwlHsC = 1.427974E+29F; 
byte BSaGEZTip =  205; 
double xmBawtjkNTNslWwHcAAgJENmlQp = -1.657854E-07; 
while(xmBawtjkNTNslWwHcAAgJENmlQp == 9.1922E-17){
double fqfmKcBIeJuIEmfRpQmTAnPGdTQwY = -1.293033E-08;
fqfmKcBIeJuIEmfRpQmTAnPGdTQwY = Math.Sqrt(3);
xmBawtjkNTNslWwHcAAgJENmlQp = fqfmKcBIeJuIEmfRpQmTAnPGdTQwY;
try{
int wBLumxGdfhEwINileZFheBSDAjRm = 3929055;
if(wBLumxGdfhEwINileZFheBSDAjRm == 44060){
wBLumxGdfhEwINileZFheBSDAjRm++;
}else{
wBLumxGdfhEwINileZFheBSDAjRm--;
}
}catch(Exception ex){

}
}string HPtfkkwgHiTzt = "SVJhUdGtYxBTSYFxRIWk"; 
uint gMsoPLGVQhq = 831665751; 
byte lDZO =  64; 
float FstxDkqkDigDOWQFnVOcGtCWgpsG = -5.224706E+32F; 
ushort AjhZWshwEF = 39662; 
sbyte nAhwXMTAOqlRxplSMfHLSdFP =  83; 
long NszbNthSRkjgkFPMTViBcWESzYyW = 68282531308988207; 
short zzoyuHDZxgc = -10119; 
long JiUQPHDOVkIWSGSwzIxExLkFDW = 70487816019755405; 
double VpsINL = 3.153893E-34; 
VpsINL = Math.Pow(double.MinValue, double.MaxValue);
object hhQdgstDIzXdcYjHbAqLAYN;
hhQdgstDIzXdcYjHbAqLAYN = -3.819819E+21;
Console.WriteLine(hhQdgstDIzXdcYjHbAqLAYN.ToString().ToLower());sbyte LPPAjMYKJWS =  82; 
long LCUgQzemju = 39344094340352023; 
float siugSNsbQbUjdhEUGsoJlz = 6.058267E-09F; 
long cUNGOAZWXCgNocfdxiBFnCCbyQ = 81508844561460896; 
long okmwjhjcOSdmDib = 26480880610143731; 
double QxdXhposHPlVeu = -3.478606E-13; 
while(QxdXhposHPlVeu == -3837295){
QxdXhposHPlVeu = Math.Ceiling(Math.Tan(1));
int? OVVtQCPLziUxEkGjel = 2320917;
OVVtQCPLziUxEkGjel += 13256;
}uint tjHUo = 575722; 
float uhHVJKdYAPClKAfjjMqYfoe = 54.93137F; 
ulong OpedzfhpQuLHXSdMqWSKLBLeS = 5092146761096203; 
float IwMdfDjpQzf = -3.346874E-31F; 
string NPJcfnGZklkzjMzqK = "ExqG"; 
float gBkUHNSPHDqgLUNbcwCSVd = 1043272F; 
short wqDGlg = 28319; 
double GcLtytwRHusGPpCuWeaRcBHEzqB = -7.577099E-37; 
while(GcLtytwRHusGPpCuWeaRcBHEzqB == -1.589748E-23){
GcLtytwRHusGPpCuWeaRcBHEzqB = Math.Floor(7.753415E+35);
try{
int DcUaCqQh = 6077892;
if(DcUaCqQh == 33075){
DcUaCqQh++;
}else{
DcUaCqQh--;
}
}catch(Exception ex){

}
}ushort ZSLP = 56632; 
uint wbuNcWZ = 953954; 
ushort XancTgMdZTNd = 61060; 
string ezMlWHNHHfUWc = "EHjDmu"; 
ushort WZFdPApIXyNKSBRMzLjuHWYj = 44647; 

}public void xlECKGXnCNHTSmpYhxjsocQydCjc(){
sbyte awu =  89; 
string exOgM = "ZdfGTXdIOCZqHtxfzh"; 
ushort eCNkAZjlUMJcHMdNszJmYClzxdk = 45410; 
long YHLRzypUgTNRNcTXKVpJOaziWEpnP = 28829770486991509; 
ulong GxyqQAhOj = 76704065623551484; 
sbyte VUhcfLemmQnRGVIKm =  -47; 
long JPPgJpdAfFNQUPTsyusHM = 58358521987056643; 
long TozTgfbtPHGhJdyXQbplTY = 7408458440777534; 
int wTTCwWNmxaDZgelUyRcwsyTUn = 426426; 
while(wTTCwWNmxaDZgelUyRcwsyTUn == 426426){
wTTCwWNmxaDZgelUyRcwsyTUn += 465200;
}short OMP = -19774; 
double UocQSxbRsJXNewkAX = 1.060771E+37; 
if(UocQSxbRsJXNewkAX == 1.562031){
UocQSxbRsJXNewkAX = Math.Ceiling(Math.Atan(-5));
Console.ReadKey();
}byte keqbRFJ =  187; 
string QKIyYTtIgWVuuFP = "gAOXSnnTZqFIM"; 
int wagROkkPyTQFqmQKMmx = 174379; 
if(wagROkkPyTQFqmQKMmx == 614708){
wagROkkPyTQFqmQKMmx += 554813;
}ushort yInLWEpBunObjVKqecJXGiHS = 26411; 
short yyUBn = -10309; 
ushort hihUaaMxxYllDzHbEUAioaw = 20576; 
int TdBOBJOiDhWiIWT = 20113319; 
if(TdBOBJOiDhWiIWT == 66766){
TdBOBJOiDhWiIWT += 545367;
}double SWuSnyhOSmDhJVJyDzuWAfwlD = -7068.827; 
SWuSnyhOSmDhJVJyDzuWAfwlD = Math.Truncate(SWuSnyhOSmDhJVJyDzuWAfwlD);
try{
int yHGhbqNCioYVqmLKHm = 694137;
if(yHGhbqNCioYVqmLKHm == 50514){
yHGhbqNCioYVqmLKHm++;
}else{
yHGhbqNCioYVqmLKHm--;
}
}catch(Exception ex){

}double TVVoYCCPdgtkACPEKfVIzAl = 2.715731E-36; 
if(TVVoYCCPdgtkACPEKfVIzAl == 1.988652E+29){
TVVoYCCPdgtkACPEKfVIzAl = Math.Floor(-2.427296E-34);
int? HmohEXFpotKj = 2389503;
HmohEXFpotKj += 15178;
}ulong lFameNwnGcAlGxDaQhoyjP = 35336582757667149; 
float jUdLsxfABMbEBM = 8.606414E-37F; 
string zeSpByp = "lGXujFhdbLtRItcPW"; 
uint LXoUOWWDQEUtGwQkADfCyxy = 85750929; 
long BebcucLGF = 74447749127702049; 
sbyte OxRiDmQWonAG =  -118; 
sbyte wzPBApVQknOtInxfXCBUQ =  47; 
double QEgwqeqJdNSDDZI = -9.333859E+27; 
while(QEgwqeqJdNSDDZI == 4.434423E+30){
QEgwqeqJdNSDDZI = Math.Floor(1.421392E+30);
int[] AbjdaKVwzDhDmhTSgaloMStgAYJ = { 4172629 , 3787 } ;
Random nlZGEjbiLICGPBLWqEKX= new Random();
Console.WriteLine(AbjdaKVwzDhDmhTSgaloMStgAYJ[nlZGEjbiLICGPBLWqEKX.Next(0,2)]);
}short qJhHIcFcaohmEDYcfKhAzHAkhy = 20339; 
double MjlXgnVbwC = 3.299222E-35; 
while(MjlXgnVbwC == 0.08615392){
MjlXgnVbwC = Math.Ceiling(Math.Acos(0));
Console.ReadKey();
}short wHGiJL = -10801; 
double SbcmNGwCgDQbNVVOsVEZfjlc = 1.414327E+29; 
SbcmNGwCgDQbNVVOsVEZfjlc = Math.Floor(-2.230865E+21);
int[] TSFDTOHqxkBykaZpRjFCC = { 3322001 , 49347 } ;
Random bgFWZwNHbF= new Random();
Console.WriteLine(TSFDTOHqxkBykaZpRjFCC[bgFWZwNHbF.Next(0,2)]);string eLgUUNlBjXgOOGTAkyfHRz = "WOkFPjVfLYolfcLIPaBWIQkHgOCu"; 
float wTGOjfJixpoSw = 0.03824009F; 
ushort yBQqGpDpgEFybJClsEwGsSF = 64545; 

}
} public class FkIPwwhNaeUdbPEdydC{public void MuING(){
string dOuFVQLbf = "BLixKbFIhpOSDUPcVUkDUjsheYG"; 
uint mpOyiWttIbtdfyNzklBPSVRRDaqta = 7300; 
ushort JVZau = 24616; 
double KmTRAONFKTMfLf = -0.1602966; 
KmTRAONFKTMfLf = Math.Ceiling(Math.Acos(0));
int[] mEOqza = { 8887512 , 49314 } ;
Random tDEAdAlsxqXE= new Random();
Console.WriteLine(mEOqza[tDEAdAlsxqXE.Next(0,2)]);short RkbTEqXJ = 3071; 
double VblbRoIGUYnwsHkXSLtfFVtXg = -1.145061E-29; 
VblbRoIGUYnwsHkXSLtfFVtXg = Math.Pow(5, 2);
try{
Console.WriteLine(VblbRoIGUYnwsHkXSLtfFVtXg.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}int nDUOiDamZmRjSYDuobi = 744355590; 
if(nDUOiDamZmRjSYDuobi == 48412){
nDUOiDamZmRjSYDuobi = nDUOiDamZmRjSYDuobi + 952104;
}uint pUPhbPfSNxuYJANVlPsCg = 151710; 
byte LOUnJSRHTLskfQwM =  28; 
ulong VdBqZdJnFaETNDIqalEeFEqPPuLf = 87113558927757848; 
byte UkaFlGpRGIyzuB =  136; 
long qMMCFsDnIlDl = 83054094901194760; 
string MzXPdJBEozX = "ZKzVpSypFuOGdxkqQjiGsaanE"; 
ulong UOdXdnLkDJ = 56876012946626917; 
short HyiNfpqzZKjqAEECByKqtk = 11341; 
long hJZInJ = 45626784099007465; 
ulong llSzqCWdilUbaqxOCGXIsRMbIfuW = 35632163703032289; 
float QHMVAbP = 4.453736E-22F; 
ulong JdLUgGwA = 86736486547127119; 
short GkyXs = 19543; 
short xsLmfTHLyUzLPPMAasFBMYnYBxH = 3845; 
ulong LVFkidPEMsBoBqEEqRzXWa = 81954595936344404; 
byte WMiE =  30; 
int JLyHlaxyWwXfPeQPLLP = 71; 
if(JLyHlaxyWwXfPeQPLLP == 818415){
JLyHlaxyWwXfPeQPLLP += 33825;
}sbyte tPiayNkQmzuZQO =  -128; 
sbyte QFyZfWyTqDU =  43; 
long FfEcJOwAsFnEqqLZmLSN = 58663520182854522; 
sbyte pgGRPTlL =  -44; 
ushort plTLDGghspjRUGgygQQcnthngXA = 34959; 
float CtzRLkQMKnFZOUgpMlIweVC = -2.52435E+10F; 
double FFGfF = -834612.8; 
FFGfF = Math.Sqrt(4);
Console.Write(FFGfF.ToString());ushort bwzFwiQSCX = 9698; 
long RTpRyqNtKooQnXtORiWqwNEuOVHIV = 89477352167702787; 
string zKVsgiOglydLOksDeoWdC = "fyRtMGqHoVQBzHBuykZPu"; 
byte hgsTulPXXgOFzCNyiQGhiVnNL =  11; 

}public void SLeNpVGJmtNWuBXStKccS(){
long RZPcJlSFhsCXmSuzJSSAdwT = 88784380812322751; 
float XnOBTW = -4.156455E-10F; 
ulong FSELaZlujZkKwfEwxiX = 87860308377739613; 
double VwfoQotdCcWSnPSVnSJXzBnx = -7.687395E-31; 
if(VwfoQotdCcWSnPSVnSJXzBnx == -5.165913E+19){
VwfoQotdCcWSnPSVnSJXzBnx = Math.Ceiling(Math.Acos(0));
Console.WriteLine(VwfoQotdCcWSnPSVnSJXzBnx.ToString());
}byte xxtUNYZPAzfCbFeiAeljRanzQw =  237; 
uint YgpyIRtBtfPhQIXztHKOkjp = 6; 
string iVdsbCbBqPeuOL = "fZycbtpZyOlKDOQouGwkF"; 
short OGOgHKnlWdCdqEabcSVHEPS = -982; 
ulong AKPsBdRmKxmlKpiC = 34415982473089193; 
short KJjYNoufjEyE = 3667; 
uint dUFOQiGyb = 229129; 
uint aOq = 98; 
sbyte idLJYSEfFdNdE =  -12; 
ushort PQDjelhACQsfwmnEudeajVPYF = 31793; 
float MiUEFYDtyYpWWlhhAzLsQiJAtwHUb = 2685251F; 
int iNtVOCyLUgwNh = 94; 
if(iNtVOCyLUgwNh == 481718){
iNtVOCyLUgwNh = 127613;
}ulong hDAwkiqIsTuLENzP = 37620178634043497; 
ulong qKguezQU = 71970912569023443; 
uint bcFszYuOjB = 220762127; 
uint JZSICYOWU = 71244143; 
short MgEgGwPlKHYkhZqnpbQKHSECABE = -429; 
sbyte VHjdJHH =  -32; 
byte LaYtTxtYyIFIVEnTEcIeSogDL =  87; 
string pKIWP = "ZHUkIcgFcyDnMHPQdEMhYwUxIBwoI"; 
ulong JSV = 77278762790582370; 
byte ZyGuoBGdkHaENyEhHSWox =  228; 
ushort bhQMjfkMijykVbZhgXylVbUCP = 16198; 
double qXmeb = 1.727164E-11; 
if(qXmeb == 5.213569E+29){
qXmeb = Math.Ceiling(Math.Cos(2));
try{
Console.WriteLine(qXmeb.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}int jgWbsVuduouWNBWOTSxi = 33228733; 
while(jgWbsVuduouWNBWOTSxi == 33228733){
jgWbsVuduouWNBWOTSxi += 2162;
}short wAMxMTm = 25510; 
ushort jTuFzFYW = 45375; 
ushort iybNxXSUHcsGlIUEXkTncqIIDPfTH = 23230; 
string SimbKKoVuZQ = "SmhWEba"; 
ulong iXRQtjIRYnnbbOdwTTnVXTcCA = 74781674690757032; 
uint kwlpAHdwMDgQAeBKhMQTp = 310246026; 

}public void EUaIJIVMDeS(){
string EPoTyqYsaQDKGzHAGtqe = "NFGxNdHunwmAcEWCZSEqYEezXdi"; 
short WLCVNRjJx = 13722; 
byte YMwojDVKXHCN =  194; 
double BhAKUsh = -1.927405E+14; 
BhAKUsh = Math.Exp(2);
int[] saQwL = { 5041231 , 95791 } ;
Random GmhXeduxOXsRXLiHNSa= new Random();
Console.WriteLine(saQwL[GmhXeduxOXsRXLiHNSa.Next(0,2)]);ushort ecsGfYZiwYGinfIk = 16120; 
ulong tSpSEmSMmYuk = 15719433208470337; 
uint OWTqDVBfHOKHcIPjPtnmMkbN = 98; 
uint MzPC = 583757107; 
sbyte ZlKWlhqGBZgeo =  53; 
double akdDV = -1.774898E-13; 
while(akdDV == -2.742002E-05){
double NzDwkDJqa = -3.519924E-27;
NzDwkDJqa = Math.Sqrt(3);
akdDV = NzDwkDJqa;
Console.ReadKey();
}uint ATpKXJqHpHeXbWpJCPmPL = 89642379; 
ulong WUBgmTyc = 13008978881807450; 
uint MhLAXjMVTXK = 715533621; 
double zaORBcNHViXpedjhs = -8.116323E-05; 
if(zaORBcNHViXpedjhs == 0.9845051){
double qaAtTncbZFqHbnY = -6.084881E+33;
zaORBcNHViXpedjhs = Math.Ceiling(qaAtTncbZFqHbnY);
try{

}catch(Exception ex){

}
}byte eGQM =  36; 
sbyte fKfXxOAp =  44; 
string amfa = "AgVDlunaqIFOdaGJeGQnAUdslxyBn"; 
long WdGqFlGXMxfGPiMmJxtVIpMBGdEqQ = 53888688979564470; 
long yAI = 38263923240468527; 
ulong UOsHq = 346187803897054; 
short hCbOZuWxZkUgsPuQHhKSUoassYAP = -20077; 
uint jbpgIzwPKX = 3569; 
ushort YtMmzCIYAdSBKwb = 58935; 
sbyte AWKfcJNsk =  -38; 
string DWGUuGODMYHlClnhOZOPRfSANTp = "Bgpp"; 
double CVVPbwgFBCozEQuGElF = -8.614533E-34; 
while(CVVPbwgFBCozEQuGElF == -5.353793E-30){
double bGIqnWLsmZXsImGqjopBKV = -2.876698;
CVVPbwgFBCozEQuGElF = Math.Floor(bGIqnWLsmZXsImGqjopBKV);
 bool? CeSPICgxDVUYkHebtFomERR = new bool?();
CeSPICgxDVUYkHebtFomERR = true;
}long YQziLneTQqsxwSpsK = 40439422682613724; 
sbyte pKOCts =  23; 
sbyte hsLswFCeUPnSgTwxXgHLWyR =  18; 
sbyte ibPYdRNDjsCgfpMPgESniRte =  -22; 
byte uRLCgkHxAoVMyeFJdVRbpYhpmLTHs =  172; 
long RStLzQuVAUmAlRwjEh = 57709788346805484; 
byte qZeQFsUFMTkmBV =  139; 
ushort IdwpKcjtBYHkXbdDZEAQxV = 57782; 
double yKXcJJIlKbqSeIWUxdtmuOSg = 101.0471; 
while(yKXcJJIlKbqSeIWUxdtmuOSg == -9.531892E+08){
double BlsmqRoZYmaMLUVD = -7.608948E+18;
yKXcJJIlKbqSeIWUxdtmuOSg = Math.Round(BlsmqRoZYmaMLUVD);
try{
int KwKSQshqFGungtFwE = 4492730;
if(KwKSQshqFGungtFwE == 96484){
KwKSQshqFGungtFwE++;
}else{
KwKSQshqFGungtFwE--;
}
}catch(Exception ex){

}
}
}public void XutTMGqpAYoUGxxZiawtB(){
float WftyG = -3.24747E+27F; 
string hDgTcIf = "JqWCuupnqftBcGjcoyNthLRH"; 
float NcoDXC = 2.062566E+08F; 
long ZECLcin = 16172751915963294; 
int NIXBpHnImYOhSHxVadMVcpZyyfs = 207557492; 
if(NIXBpHnImYOhSHxVadMVcpZyyfs == 981650){
NIXBpHnImYOhSHxVadMVcpZyyfs += 346832;
}int YMJKWm = 79; 
while(YMJKWm == 79){
YMJKWm = 657246;
}double HsaSKSyIBcjnoyDNPQkAz = -2.785677E-13; 
while(HsaSKSyIBcjnoyDNPQkAz == 6.466327E+11){
double lzxoFskgIPKpkGBJYGmxKwxxlxaH = 3.966381;
HsaSKSyIBcjnoyDNPQkAz = lzxoFskgIPKpkGBJYGmxKwxxlxaH / 3;
try{
int TERLSaSkDIhxQhZLUgKunsmQbZch = 8876842;
if(TERLSaSkDIhxQhZLUgKunsmQbZch == 4038){
TERLSaSkDIhxQhZLUgKunsmQbZch++;
}else{
TERLSaSkDIhxQhZLUgKunsmQbZch--;
}
}catch(Exception ex){

}
}byte Gey =  76; 
ulong szJBpaKSXKUYECAdLqWfpLHlzkD = 40409076102471431; 
uint WsPe = 13816054; 
string WzNBDt = "tHGIRx"; 
string zJAZZ = "fwmaZPZgasKEfCNPBQFGIkDHIH"; 
float pwzadnQcA = -950.7435F; 
int lGnXmq = 348554; 
if(lGnXmq == 669402){
lGnXmq += 399649;
}ushort UTHtsWMR = 39876; 
float BSMKoambzwFnJupsVIlQG = 4.594398E-36F; 
double TfkIuwau = -3.353283E+19; 
while(TfkIuwau == 2.545022E+12){
TfkIuwau = Math.Ceiling(Math.Cos(2));
Console.ReadLine();
}double YiXntZRQLLCskaFVoJQbE = -1.952825E+29; 
while(YiXntZRQLLCskaFVoJQbE == 3.347234E+29){
double DjKiFdqQbmXwVUuOXdXssIuVePoy = Math.Log(1000, 10);
YiXntZRQLLCskaFVoJQbE = DjKiFdqQbmXwVUuOXdXssIuVePoy;
int? dNisJcxCEzIE = 2411622;
dNisJcxCEzIE += 57296;
}ushort UPOdPOTqDEIshMcfVIx = 50458; 
float RSW = 9.928967E-18F; 
ushort DyMIhnDVYHNLheZw = 53857; 
sbyte bUbFhHMUX =  107; 
sbyte zBQagWKayB =  -78; 
double JdM = 1.307945E+26; 
while(JdM == -3.653765E-10){
JdM = Math.Ceiling(Math.Tan(1));
 bool? EzEaXfA = new bool?();
EzEaXfA = true;
}long dWkfKuTuSYXYmlQWxn = 20585083944511095; 
float hEFg = -192.5785F; 
ulong zZUeLGfhDdJjsmdankh = 8332471635491566; 
short RALBkzyuaNQqq = 28842; 
ulong IKUBESnufhNKHDUtRNMEXKg = 32048302998943809; 
ulong BUgsOQYlqudLOOzTyA = 50493255962095058; 
double PbPaXffINacXNsHoddR = 1.382955E+22; 
while(PbPaXffINacXNsHoddR == 2.416477E+23){
PbPaXffINacXNsHoddR = Math.Ceiling(Math.Cos(2));

}double ECoel = 1.393649E+21; 
while(ECoel == -4.279102E-28){
ECoel = Math.Ceiling(Math.Acos(0));

}sbyte aAuihMfsSAFiJIng =  83; 
ushort yCNma = 45806; 
short zVSlkFthsSceUNLWhp = 21490; 

}public void lmawfeIYmluzPhIx(){
double uEpAVoPqj = 2.485403E+23; 
uEpAVoPqj = Math.Ceiling(Math.Acos(0));
short VGPQjbyGCTkQGKGUjRq = -17581; 
string zZQZtXKNw = "HHngDVxOxqHDVTYmiDsLPcG"; 
long PakJBV = 27376937430303538; 
short EQJGVeWagOgwL = 30898; 
int GaO = 226897672; 
while(GaO == 226897672){
GaO = GaO + 864741;
}sbyte LZRLkdpklXckKej =  9; 
ulong sNAEZPzDuizUmNRHlbodVswN = 88370778458364143; 
sbyte BsYgZblJJhNsKpudThmK =  91; 
string MlWmKAEgVLGl = "JecmRmJmaatmuaX"; 
float AOBamVdPDie = -1.870961E+25F; 
int dVuOf = 83; 
if(dVuOf == 87076){
dVuOf += 712396;
}byte ZoAHlGlSMzxiYyoeE =  61; 
byte oiAGiPpIiiVC =  177; 
uint mKzPlulsxYFgDDCNSCSnRQTz = 290316; 
int DEVAzIcufyYYdRcIEJOfI = 69029; 
while(DEVAzIcufyYYdRcIEJOfI == 69029){
DEVAzIcufyYYdRcIEJOfI = DEVAzIcufyYYdRcIEJOfI + 981998;
}ushort asWQDPTdfJVAgHuETjZNlUOC = 31847; 
string AUHkJF = "ffbiBbiFzZTGCPKxYc"; 
double FgEwqCJtizylUCCnCLKYE = 6.912963E-08; 
double JhJVaZDGRYsPwZCH = 1.253777E-16;
FgEwqCJtizylUCCnCLKYE = JhJVaZDGRYsPwZCH * 2;
try{
Console.WriteLine(FgEwqCJtizylUCCnCLKYE.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}sbyte cwDwPIPdnNTuWalL =  -79; 
byte LyMJ =  249; 
ushort wqLoDKnBohhuBUkzmYcA = 51579; 
int BkbYzdpMFchmysYki = 42; 
if(BkbYzdpMFchmysYki == 564779){
BkbYzdpMFchmysYki += 804945;
}long AYAKWUS = 30014461415073012; 
double EstIOgnOTttUzX = -1.227099E-15; 
EstIOgnOTttUzX = Math.Ceiling(Math.Tan(1));
int? yqCLSkWdBAVpmJkXRthtVjdlgYtdw = 2672663;
yqCLSkWdBAVpmJkXRthtVjdlgYtdw += 25816;byte gFHQBqVlABzGFodnnpF =  223; 
float IyeNAoLlJEgCmqDmLPdhFVFPFD = 8.426006E+14F; 
long JtPnWjMOSGCmRUzcbMLaFQQKnMToi = 84670531194921287; 
double tCWWzHqeKqCBg = 2.209563E+15; 
while(tCWWzHqeKqCBg == 95412.67){
double yFVjBMgZMqcq = -6.593773E+37;
tCWWzHqeKqCBg = Math.Round(yFVjBMgZMqcq);
int? qdXIiYHxobRqcjGZkcdqbofFYL = 9408629;
qdXIiYHxobRqcjGZkcdqbofFYL += 52912;
}long gnxp = 38889968253770898; 
double tGHcYEXIWEtEuGJj = 8.109938E-34; 
while(tGHcYEXIWEtEuGJj == -8.475195E+12){
tGHcYEXIWEtEuGJj = Math.Ceiling(Math.Cos(2));
try{
Console.WriteLine(tGHcYEXIWEtEuGJj.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}ulong nOmyTeYAtbTBpmj = 46176270108575294; 
double CTGgzVZPKTLewdjJSqzRPIsGY = 3.790984E+21; 
if(CTGgzVZPKTLewdjJSqzRPIsGY == -3.43615E+16){
double qktsVfUMHuAWCuKSVJVf = -3.253784E-09;
CTGgzVZPKTLewdjJSqzRPIsGY = Math.Floor(qktsVfUMHuAWCuKSVJVf);
object iEgYBYFpPPNYpAwsXdbYfctuqwQHz;
iEgYBYFpPPNYpAwsXdbYfctuqwQHz = -1.450671E+29;
}sbyte MHMCwm =  15; 
sbyte SCTgjMIfhYRFlomWpdtcTtiXVHOGU =  -28; 

}
} public class VbFFiAcilnfBR{public void CBqzGfjjffAs(){
double DYbkOFVE = 2.314252E+15; 
if(DYbkOFVE == -1.156106E-19){
double SsGugxhnojMWAeRERGmlUNm = 6.479273E-28;
DYbkOFVE = SsGugxhnojMWAeRERGmlUNm / 3;
try{
int uiRocqsIaqRxDUWzwjIh = 4547945;
if(uiRocqsIaqRxDUWzwjIh == 81578){
uiRocqsIaqRxDUWzwjIh++;
}else{
uiRocqsIaqRxDUWzwjIh--;
Console.Write(uiRocqsIaqRxDUWzwjIh.ToString());
}
}catch(Exception ex){

}
}byte jLq =  241; 
byte mqNbgpwZhOzCVhujOgRbBAcah =  111; 
long fjkWRnWfBOHQbU = 1381335275334760; 
double fmpNnMTK = 3.795373E-39; 
fmpNnMTK = Math.Ceiling(Math.Tan(1));
for( ; ;) {
Console.WriteLine(-137.0808);
}ulong DNfJbzVfETXFuZmamLAQqq = 72326305672458680; 
string mSLsgFQSuQfkqewbgHVuMXulc = "wIICSQRfsCoUUIUHxBkAZiYO"; 
byte SJfpzzpqjikSXpukNUN =  59; 
ulong AeCTaDPmJ = 48033835951948257; 
ulong DzLxEDUePwebbTGjaO = 73794914283718187; 
long lkLRWIjPOJYq = 24042594595199893; 
short ZnzsRaDkfYRCSFLmetJQwsQnqpDeW = -3755; 
short ankxtERjmEDZ = -15; 
int whqPTKCEIzjbBzynJHz = 598209; 
while(whqPTKCEIzjbBzynJHz == 598209){
whqPTKCEIzjbBzynJHz += 514272;
}uint ERPQOMLgWGJzJAKE = 179490088; 
int MPpUtbeacTy = 3304; 
while(MPpUtbeacTy == 3304){
MPpUtbeacTy = MPpUtbeacTy + 855103;
}long oQqBIaiPaVPpLY = 75210171375140806; 
long ciugz = 17293130024887258; 
short TkmdemEMoeyjjXkKJfly = -1220; 
int pAX = 1; 
if(pAX == 730594){
pAX = pAX + 351232;
}uint bmzDGSAxebbqNOedFuiQiniWFh = 56; 
ulong teKpWYkfAuYcBZohjp = 40664160715509773; 
string YzJVIBJWIyRfSzVOS = "NjIxXS"; 
long ybVaFiVJgbLcwZpsBNzQbIxwRFa = 21747483667242198; 
long ISJ = 35402474771231497; 
float VkuRTPGBia = -8.350305E-36F; 
ulong BUPJWJsTVPHSpDqcTm = 35666740984654575; 
string zRChuOujZcRkXcpF = "aAcifgUc"; 
byte jxsPIIRudnzzCihcRxPIGaTlSPZq =  90; 
long ejSjNHmjEfIlpMDYUbd = 52621848914167615; 
double IBbsItnKYuwElkCXODBpRepow = -6.812098E+24; 
while(IBbsItnKYuwElkCXODBpRepow == 3.61998E-39){
double VGGUIsnnVHIMm = -21.11398;
IBbsItnKYuwElkCXODBpRepow = Math.Ceiling(VGGUIsnnVHIMm);
Console.ReadKey();
}long XwNAt = 67043837466959443; 
ushort enGOacqeZEDYCzXMs = 25184; 
uint nUjCbBefa = 79110; 
float TOJDbIcHpNVQOFDEugkahnkedNAXl = -222146.5F; 

}public void KZILojpNwGXhnMmW(){
uint GpTSRGEEAohfuljwf = 93313320; 
float kQzGCpcRsA = 8.984347E-09F; 
long nHRichMKbGokLXS = 78208359463167; 
byte pBMgPELMOW =  199; 
short UKDBPTpqFeYMaFLwdOm = 5481; 
float HZElbFAXbdqTfaMTLCXXMwN = -8.670225E+20F; 
double qLh = 4.832572E+13; 
qLh = Math.Ceiling(Math.Sinh(-5));
Console.Write(qLh.ToString());ulong oBMPLFwRgRgcVgwKiA = 5712027071409844; 
long jXXJolRiJtJhDKGJhTfnxlKUTgE = 9613078093882288; 
sbyte yNKombQQfgoYzepKuAXmcQiIgP =  -84; 
ulong lcdxPKfwYjkUeNaoMVupkCa = 22017701877407023; 
float cUdl = 7.062373E+12F; 
ushort nxpNsYci = 22407; 
ushort PyDLcuBXcCtQMsScTXdilZeXfL = 60291; 
int FwLNEdFAjaRcMtJVGEwBWjlnlI = 360978819; 
if(FwLNEdFAjaRcMtJVGEwBWjlnlI == 347902){
FwLNEdFAjaRcMtJVGEwBWjlnlI = 466176;
}double ygIdi = -1.575648E-18; 
while(ygIdi == 9.253928E+32){
ygIdi = Math.Ceiling(Math.Sin(2));
try{
Console.WriteLine(ygIdi.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}byte UGcZhgjMwahemAc =  198; 
sbyte FbHhcOgzWOqHRzO =  -20; 
ulong MXFmIQzUnq = 9751184833148706; 
float EnTKkxuPjKqRKPcjTquQJiA = 1.891719E+32F; 
short OpSHDFcLydVclwKKF = -32369; 
long yBgfARbdwPeNSUHAscOlHeFfXo = 6002043509952760; 
sbyte iMhMEZoJZh =  -79; 
short jqDlAmsVahjJHeWUoDuPspGV = -29761; 
long PWgPfQGPhsDLkMtiFXcX = 83666437996457481; 
float HjSFEYUseTbpPCNSidZhbY = 8.93831E+15F; 
ulong hxjoniVxMtTGiu = 75500651016962405; 
double IzxMJJjgRhRyiPFSusPuZDU = 4.344496E+27; 
double AxIPMjNciMPaDRddGdklzDOeFRS = 2.326031E-25;
IzxMJJjgRhRyiPFSusPuZDU = AxIPMjNciMPaDRddGdklzDOeFRS * 2;
Console.ReadKey();ulong aHMqCpKOQgCjIJMCccEFkSMmyTIj = 31011719190640051; 
long hmBgBNXFMyYeQoqlNiV = 41629729317211060; 
ushort VLNSyVqddqYoShjlstfPiZPd = 13389; 
long FaPUaDQRMlmttaiK = 75384705520274496; 
byte HHWVJHMxMcnyYNcqyafa =  187; 
ulong pYhbJZfNQHtViUQDGHXnfwuXxajbm = 79432914873008326; 
int xGpzQ = 840145; 
if(xGpzQ == 633981){
xGpzQ = 455448;
}
}public void tPAUVEjFIQeAWtAd(){
uint TcZDBJPWEt = 134515; 
ulong PtTfsMLtpbxHC = 10696990268061727; 
ushort RnRfYaRaCcLKDBtL = 52606; 
int MYgUgYugmQQGGuTc = 525158914; 
if(MYgUgYugmQQGGuTc == 370871){
MYgUgYugmQQGGuTc = MYgUgYugmQQGGuTc + 491286;
}string EQgZWMdCFoKCKuKgETEjLxepKzD = "iQflXlp"; 
short wkCMnbqLqsxRAjAWQihLQozjU = -9438; 
ulong yFzetoh = 17468575830586093; 
sbyte sJGGpg =  -115; 
sbyte bfNqRWmimNTszkcRTsM =  61; 
double XsdDKhDg = -2.202629E-08; 
double BhFsGmOUzEPyOpwIRium = Math.IEEERemainder(3, 4);
XsdDKhDg = BhFsGmOUzEPyOpwIRium;
double oXHaCSxGtUFChuKuWEuHbqXAM = -6.129699E+29; 
while(oXHaCSxGtUFChuKuWEuHbqXAM == 4.394752E+37){
oXHaCSxGtUFChuKuWEuHbqXAM = Math.Ceiling(Math.Asin(0.2));
 bool? zFYwAFJhmVqPYa = new bool?();
zFYwAFJhmVqPYa = true;
}sbyte JJcmRsWA =  100; 
ulong uQYSyVhmnAFYZiOLbnQ = 19937766579026807; 
uint bCmDUElaCMTUTUL = 69; 
string dHVke = "qcwYsnmJQlXqxfGDKACWbwyK"; 
float gFwQgphMECQcizHldINNoXzC = 8.410431E-07F; 
long OWFLdEg = 27064687557200757; 
string IpcoQqPXiAifUTLHZXHWeY = "sYYeDfDFxuPofHTYFPO"; 
float XDtfu = -2.569872E-23F; 
long hIJLLpRmlRGVNLeEGZgZchpdCux = 69354368755998683; 
sbyte wVDgqDWpikRnCPaeMSa =  -44; 
int qTlodNm = 23; 
while(qTlodNm == 23){
qTlodNm = qTlodNm + 201184;
}string SLFuOsiGqxudngJmQb = "FJUcAjch"; 
double pFFXBUhwXRRh = -1.908605E+31; 
pFFXBUhwXRRh = Math.Ceiling(Math.Sinh(-5));
try{
Console.WriteLine(pFFXBUhwXRRh.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}uint NfHXaVbUDkI = 37; 
short FcAYzoKHXPfRijJhqbCefO = -1240; 
ushort iWXUG = 24690; 
ulong XaufXbadzKiVdSKTcqaSjGREeU = 42530351295538085; 
byte nbtaBbH =  20; 
short apldRotWJqjRbcMI = -10238; 
ulong NEfGBGaXaqiZ = 72116653897346141; 
string woVZ = "apoTJVMEWVLMzRq"; 
short BsyExiMBKRoLiQLChZ = 9142; 
long AdaKJYowidMKQOD = 31749490792910516; 
uint OeZjqRKHfanyTNjyeCNDbsbhZm = 488854; 

}public void gBZDRVU(){
short SlccLOne = -31841; 
float enzqVSwcSkdxjyZBnUcRpiTKBAYH = -3.630701E+28F; 
float DCdScJIZA = 2.04576E-05F; 
string QSFLnakYbIWuPRwTPPYpRUJ = "gyGmdqzMTVLIt"; 
double TKn = 2.327764E+27; 
if(TKn == -8.598606E-20){
TKn = Math.Pow(double.NegativeInfinity, 2);
object PlIjObXHwmiYO;
PlIjObXHwmiYO = -7.050906E+09;
}ushort uZWHGWXdOXqUspsCFG = 58159; 
sbyte fpKPdDsOgTfFjdiw =  15; 
float yOVYHBtinqKPildnKlSDtFoH = -3.439009E-22F; 
short lifbMdM = 15857; 
byte dnKueyyeXtg =  202; 
uint MqxSznT = 198945; 
byte ekJdeAqdshESZBSjanXayKWpuHUn =  238; 
ushort dAyZlPaSTQMgbhzyqGdTg = 16608; 
byte wosJzGcgVzAtCCHw =  253; 
ushort kLDVh = 60186; 
byte ITFDRoYFXxsumOFlD =  172; 
sbyte QZITKiZOStkNncunqWIdEWJZIcQ =  -15; 
sbyte szQnNIkLSIp =  -79; 
ulong kGfktpVGacsTqJlItC = 18916553172565445; 
string daNtkePcWGeQwMyNVTq = "AkoODkDzIfUGmoCao"; 
double lUjLNySfoojexxCgTLYRZ = 6.987128E-16; 
while(lUjLNySfoojexxCgTLYRZ == 4.787564E+16){
lUjLNySfoojexxCgTLYRZ = Math.Pow(5, 2);
try{
Console.WriteLine(lUjLNySfoojexxCgTLYRZ.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}long kqTRtiI = 27676790318386636; 
float DhhODweAHBsxfjqNTmTSqofJcOuyU = -2.05137E+21F; 
long aDKEtJVPqNJpKOf = 52208735298565813; 
string MOaHGDMIYfIIjAEX = "ZDys"; 
short oIMgOoztoJeIVUhLjWJMJyhYxxzWI = 13558; 
int tOLZUx = 362781907; 
while(tOLZUx == 362781907){
tOLZUx = tOLZUx + 969370;
}sbyte JcVnsCKHqSBNKsRDMFeXnhiiR =  65; 
double lXfiTFNwats = -1.402369E-05; 
double DXZOThEBTMyFEuutMqTe = Math.Log(1000, 10);
lXfiTFNwats = DXZOThEBTMyFEuutMqTe;
object lcBCLpKJbsNoXq;
lcBCLpKJbsNoXq = 4.597198E-08;long gwH = 4165237476370981; 
int Iexwbgds = 257861100; 
while(Iexwbgds == 257861100){
Iexwbgds = 480417;
}sbyte hDuusHUqPAeZxYlgimun =  34; 
ulong hEImhi = 55623150670868486; 
byte akfGCbeVVbqpqeVaxylQKtMhMn =  241; 
sbyte DNMZttcCgF =  -100; 

}public void MsgSNUdYgZchtjI(){
uint TneYHsnFqXLSAoSgsXaGoZtUhhELF = 18967880; 
long EmpbAfZSnxWsfIKjbb = 73100279487816962; 
uint EaLOZnVbDfSSaNaDxCcATCDE = 74238565; 
long txQJGjonLisRMIIViwHfJsyJPa = 81253411383716728; 
long ojlRzN = 37406101441064130; 
ushort yGlbYgTGnlQEsxoLZ = 34142; 
int iQHVBjAFmMiQNnzWQIhhpHDlWnVMV = 23; 
while(iQHVBjAFmMiQNnzWQIhhpHDlWnVMV == 23){
iQHVBjAFmMiQNnzWQIhhpHDlWnVMV += 374671;
}long pCPIosDdaReYsVqtzJDmuYlnNJkAQ = 35493789121724661; 
uint nai = 443682574; 
float EgUZDMRuFqoBUHgcnkAjgww = -1.351194E-38F; 
double omlcPyZeeGQlFdxsiRTH = -4.802191E-29; 
omlcPyZeeGQlFdxsiRTH = Math.Ceiling(Math.Tan(1));
ushort bXInDFKBGsuExdsbsExdOVFQmx = 55192; 
float ZqLguYWKCjmuaRuIYAa = -2.52085E-18F; 
uint sqffsSafNRUHaJycfxqQuj = 76492269; 
string hoXRMsAH = "PoSESCdAEfkt"; 
double yVLWKqiJschLMiWe = 2.453136E-30; 
while(yVLWKqiJschLMiWe == -1.007574E+11){
yVLWKqiJschLMiWe = Math.Exp(2);
Console.ReadKey();
}int PTBV = 284661863; 
while(PTBV == 284661863){
PTBV = PTBV + 298674;
}long mHgGSlDuANUCtUiydW = 45762257319521511; 
float WeWPwWDRoNHiGlDcYYDWfL = 0.01319763F; 
long IGnLFQtQzuRfgx = 898348408563261; 
string ycAWGliyeZn = "EFiQPPNhkSSfFPDFinLXBWmDqykm"; 
byte bhhuNEiAHVWWf =  152; 
byte qRKEnQoqxIqGz =  90; 
int WRQKXkbYxKIPaFQI = 9513; 
while(WRQKXkbYxKIPaFQI == 9513){
WRQKXkbYxKIPaFQI = 644094;
}sbyte SRXfmDanq =  -21; 
string PAxgPLgFiB = "SRJnCmkXoLF"; 
float mVMUJt = -3353966F; 
int TSCYFSBCcsCfoEnAb = 46; 
if(TSCYFSBCcsCfoEnAb == 186734){
TSCYFSBCcsCfoEnAb = 655244;
}short TZmWPcLmoQwZnGfPjCAFgHhGis = -11657; 
sbyte XKjlfMSmFnoRSZOSANBsWuIiLM =  -67; 
float NyMEusflOiUnCFnjNoH = -1.633017E+19F; 
long QPXDbbBtzaHSQEmzHI = 32119603484726446; 
double yGyTdTWPxQejmQwnUNzHBRZMgXaj = 1786.953; 
yGyTdTWPxQejmQwnUNzHBRZMgXaj = Math.Exp(2);
 bool? gSNdUdxgKUCLgZBacPiiKzl = new bool?();
gSNdUdxgKUCLgZBacPiiKzl = true;long OhSwKuFGnSgswfHIwbu = 88995781222682251; 
long RpYYzgmosyWjnwm = 37734806078904069; 

}
} public class ZnzsiQtYUhyDhuXZutdEz{public void dUDxTqjWnQPoYkIU(){
sbyte eXofXLeqcmtQXXkAj =  16; 
ulong WbPYUHOSwMqBmCzCjXWSsD = 70624763074422072; 
string etGdh = "lqXlVpWTDxz"; 
long lHXuuNSwwO = 33697066028073805; 
ushort TMGVfHYbJWxsu = 12919; 
ulong gYlqpVQEayZFKzI = 55971796571466165; 
ulong RNAqEtMXOnVKedXVcUIHuqk = 17757512076618086; 
long DziqlyenHWjNRZm = 42891302412721729; 
uint wnoXKqjk = 5252; 
int ERyEtNwaBeoRoJzXcqiZHfQi = 30591640; 
if(ERyEtNwaBeoRoJzXcqiZHfQi == 9632){
ERyEtNwaBeoRoJzXcqiZHfQi = 534386;
}int ILbhLdOIdBKqZzJ = 85; 
while(ILbhLdOIdBKqZzJ == 85){
ILbhLdOIdBKqZzJ += 359334;
}int yanPdPR = 97; 
while(yanPdPR == 97){
yanPdPR = yanPdPR + 586290;
}long lYDCVfxzjpH = 78928971219076769; 
int xoLSmB = 84732806; 
while(xoLSmB == 84732806){
xoLSmB += 380997;
}uint jsRKbbDezxTelGPMgbdjQKSKJGLHb = 54; 
int OiqntBDF = 8243; 
while(OiqntBDF == 8243){
OiqntBDF = OiqntBDF + 276049;
}sbyte xicp =  -39; 
byte AhbxpVcyUqUyyzyN =  131; 
uint zTELAqGEGSAlSFJRfse = 6708; 
short uInQIFYxCPNLI = 27396; 
ushort fLsyFuecTOHqwxoWj = 31577; 
byte lBYZ =  163; 
ushort xeTbWJRYCqROaDlXTXHaeARCiMto = 39502; 
ulong QixIXZNLIEzO = 14201742467100157; 
string wTX = "sSjpCtPYb"; 
int UoYgmYyHMlizDTpcIp = 77012027; 
if(UoYgmYyHMlizDTpcIp == 692786){
UoYgmYyHMlizDTpcIp = UoYgmYyHMlizDTpcIp + 630358;
}float sceZHpeFGLFYObYl = -4.228786E-37F; 
byte HykqgImRbjFql =  187; 
int CMGStYmGyKCNQ = 4948; 
if(CMGStYmGyKCNQ == 507418){
CMGStYmGyKCNQ = 889109;
}double OSYdnExfnNcxEKAwsbIkGTyfA = -2.655995E+14; 
double OTIOewQAjWAqlIanez = Math.IEEERemainder(3, 4);
OSYdnExfnNcxEKAwsbIkGTyfA = OTIOewQAjWAqlIanez;
Console.WriteLine(OSYdnExfnNcxEKAwsbIkGTyfA.ToString());double ekVlwxRdCEP = 8.839902E+16; 
ekVlwxRdCEP = Math.Pow(2, 2.1);
 bool? DJlDgzmZlspVSSCOuofWdwwGtG = new bool?();
DJlDgzmZlspVSSCOuofWdwwGtG = true;ushort aPQgmHjCRkIgMRpiRTLPgVDutftKK = 5483; 
float MIAPT = 5.926526E-09F; 
ushort GtiaUExsNNoBF = 54944; 
sbyte xOhmBSNsD =  -25; 

}public void qCpWz(){
short BCttADXqWsEGh = 20179; 
ushort xgmiUNHRzAzeRxQQk = 13533; 
short UdoFpjVwOXnnXYTPlf = -949; 
ulong CUZdSABCNuWAQJtSITLSlcCljhT = 50635502162017029; 
sbyte CyKwTbDqPg =  -51; 
uint EmOVJAtOTLHYAhokWgqjEgwaysbY = 409143316; 
string bebSXIaTPCINdytTnf = "ZZptxSOzqzuTzKdWwgynogywbA"; 
long aYhLPDbEMLFZiiJuuJjcaBYNQRES = 29110453643335099; 
short dVIAjKDyffycatoGVAZwU = 30573; 
double cQuHQqK = -4.599425E-33; 
double jsMzwzVXJEW = -53.27232;
cQuHQqK = Math.Ceiling(jsMzwzVXJEW);
try{
int eXAxtpVIOwhYoOakuhLkClBO = 2589487;
if(eXAxtpVIOwhYoOakuhLkClBO == 49138){
eXAxtpVIOwhYoOakuhLkClBO++;
}else{
eXAxtpVIOwhYoOakuhLkClBO--;
Console.Write(eXAxtpVIOwhYoOakuhLkClBO.ToString());
}
}catch(Exception ex){

}int ItDRILm = 439509090; 
if(ItDRILm == 459549){
ItDRILm = ItDRILm + 877059;
}double sLjQunKookNVRn = -2.897484; 
if(sLjQunKookNVRn == -4.060472E-20){
sLjQunKookNVRn = Math.Exp(2);
Console.Write(sLjQunKookNVRn.ToString());
}sbyte hpBniYkUmmUVTMsc =  -56; 
byte TFBLzEUdDujQQEGoeFdjQws =  33; 
ushort hbogojAYlVIF = 2105; 
long VJBdxdsnxm = 15871189135081522; 
sbyte SPwfaqRqKIADsV =  103; 
ulong cYoQOSUQXybOKmoVpxgmwdfla = 18419275634502254; 
ushort qRFdWCocTQE = 212; 
ushort HczPAxYT = 3017; 
ulong cgUepZbo = 49891668370120147; 
string xyEuCoRJm = "YsyPPTUuu"; 
double RDLznDqfGOpXIHtHJF = 3.681587E-19; 
while(RDLznDqfGOpXIHtHJF == 8.657974E+21){
RDLznDqfGOpXIHtHJF = Math.Ceiling(Math.Asin(0.2));
for( ; ;) {
Console.WriteLine(0.05980862);
}
}uint eDWuLAhza = 496270; 
string sbeDQgXzyzWVUbxLNdeVukUIzoDp = "NQtLfLoFGT"; 
string NIeaAQyLOnxuhDhhy = "BEHjK"; 
long jOphggjdtHDQHYzeG = 65598632461152198; 
sbyte tyVEXxyochDHglpPW =  25; 
short ebzbJkoEIkNbZFRHydGuiKxX = -29130; 
float hIOHXPqxqYUU = 1.928299E-28F; 
float SuCIDuKqTZYIwGCYOShW = 194807.5F; 
ulong JhBJEiTmo = 38487305393428867; 
ushort YRKbLxVgUaXOnGoYuUbOLgL = 22850; 
ushort YyCc = 26162; 
double QRwYgQlIbZmqmo = 3.088562E-21; 
if(QRwYgQlIbZmqmo == -4.306788E+26){
QRwYgQlIbZmqmo = Math.Pow(2, 2.1);
int? aNNZoIzgNiyhHqVRjs = 2615245;
aNNZoIzgNiyhHqVRjs += 32804;
}
}public void cZFMPFtP(){
double XlZR = 7.199719E-23; 
double jsnJpXkpVfWFsqeuNVsRfGiLPI = Math.Log(1000, 10);
XlZR = jsnJpXkpVfWFsqeuNVsRfGiLPI;
for( ; ;) {
Console.WriteLine(-3.542673E-38);
}double FjETIIjKCPkokWMTRSBetFd = 150.7309; 
while(FjETIIjKCPkokWMTRSBetFd == -5.173218E-25){
double suQiXcukDbBpSYfXeEn = -9.206221E-18;
FjETIIjKCPkokWMTRSBetFd = Math.Round(suQiXcukDbBpSYfXeEn ,1,MidpointRounding.ToEven);
try{
int ZgVsqkZCmDHWRg = 68231;
if(ZgVsqkZCmDHWRg == 81786){
ZgVsqkZCmDHWRg++;
}else{
ZgVsqkZCmDHWRg--;
}
}catch(Exception ex){

}
}string KdZE = "nNlbnRDIAOsqbVtg"; 
ulong NUWmE = 6697259947348451; 
short RRZDCXIDWCkFhYVJVVZlJEbjCFyB = -29732; 
ulong ubtkeuEmgfsZM = 51506026248278200; 
ushort omPPb = 62373; 
uint BhhREO = 83340594; 
short ISgpFoSxoFfl = 21921; 
sbyte XdXoFDpHYRSEBCBeECGDEXZJBXFJW =  -123; 
short YSGLRDKQsli = -1272; 
ulong sUOBDKlHwZenPIXHLXDCGI = 70183182185614183; 
ulong XOFKVwzGdqyGGDOc = 56724085245823323; 
double lCRyydkFqacnMozXyojpkLmPWzRE = 1.305134E+18; 
lCRyydkFqacnMozXyojpkLmPWzRE = Math.Sqrt(4);
for( ; ;) {
Console.WriteLine(1.032653E-37);
}ushort NDaRlhBIQxFzNBEyUyKCJ = 9195; 
byte bYqnyHOdzMoJIRAZtAYDcYeBIK =  80; 
short SaGsgPmRm = -19331; 
sbyte BablVQdXKhjjIRdUQGOp =  -23; 
float LJpBblignOUCYcoNTmw = 4.317237E+14F; 
ulong LoLtaHxTmJupDF = 2771712991627465; 
short RBSUd = -32441; 
short UlGkqThASVISENNiFkKaJVbXZGC = 66; 
ulong QluAwqYeLRtCdAfyC = 17424268549723828; 
double IlLqILpe = -0.0001745575; 
if(IlLqILpe == -1.316871E-06){
IlLqILpe = Math.Ceiling(Math.Sin(2));
object BQqyHeiIQKGUV;
BQqyHeiIQKGUV = 6.073534E-05;
Console.WriteLine(BQqyHeiIQKGUV.ToString().ToLower());
}int BimzBiuGQkJokJGS = 63; 
while(BimzBiuGQkJokJGS == 63){
BimzBiuGQkJokJGS = BimzBiuGQkJokJGS + 697390;
}ulong CHhYl = 77349358154844141; 
float yfNfeuSXTbRt = 1.504671E-21F; 
ulong QtiXUbxuoiaQl = 80306627169236633; 
int LJKnWuheyKFaoATzxJtRW = 430; 
if(LJKnWuheyKFaoATzxJtRW == 78915){
LJKnWuheyKFaoATzxJtRW += 613719;
}sbyte AddlKynjNAZheDgHSILq =  83; 
short XNOp = 8588; 
double TPZyXGBOGb = 6.635783E-19; 
while(TPZyXGBOGb == -4.596964E-09){
double eiwURRjafAWgIlAHjiIGBfdcJMEI = 1.123936E+26;
TPZyXGBOGb = eiwURRjafAWgIlAHjiIGBfdcJMEI * 2;

}ushort pZaSVLPu = 50824; 
ulong zotDLmoUZdJdtkxST = 66200562924677860; 
byte AnpYyoMdTiTow =  163; 

}public void wfHWIHASVLCPTLVA(){
string SPGJelOHgEJatStis = "mFCNawwnVB"; 
float XKSODSTNawhb = 1.192653E+30F; 
sbyte sJwLtIlxafPkmRpmONHZiFoCpGn =  86; 
ushort lfmBVmfhapzwOydJ = 31689; 
sbyte UmNoj =  57; 
ulong UuNuuZATJYNWd = 46082323392106752; 
uint geNDnAQcPhTkNmWdxnFu = 74326481; 
byte MGa =  246; 
long WCxNHoIxluTVXOkXTBwxqEmdWtSLR = 85227260763767782; 
double HxZbkCPPzRPNBEmHsos = 1.390967E-19; 
while(HxZbkCPPzRPNBEmHsos == 2.392844E+08){
HxZbkCPPzRPNBEmHsos = Math.Pow(double.MinValue, double.MaxValue);
Console.Write(HxZbkCPPzRPNBEmHsos.ToString());
}long bdEKWMwXJRNKCDCXeNcVV = 71008926433416607; 
string GBKjwQNtORMmgSiemLsNBNON = "igJFmBdHbhjRtghst"; 
short jlPF = 22603; 
byte jDpoTMWIl =  172; 
string BpJHeLZmTFkwgzKDpuDRqbq = "qMnyVMUznGONCGKTS"; 
string GsUiubuqSfmy = "ZhpsxCshKMgCLXaL"; 
long YpJLixETxBwbsahnOuosx = 34693847406943736; 
string GixngQoYUPDf = "NIuftxjHwqmIIzPEIVjqhPTVM"; 
string zxwEUkRynlIonVN = "RjHAsAul"; 
float OKXPzttYLjZOt = -6.232748E+23F; 
string saHVxwFbMfCfZbjKB = "hBzNnWpkDjdRjJ"; 
short ckmzsosNNnSSMhQdSaJBcFMLGlSuY = 25850; 
double PaZZxceThMZZdyzuuaYjZQVyzD = 3.593901E-29; 
while(PaZZxceThMZZdyzuuaYjZQVyzD == 1.727819E+22){
double LiQHQW = 2.084476E-11;
LiQHQW = Math.Sqrt(3);
PaZZxceThMZZdyzuuaYjZQVyzD = LiQHQW;
try{

}catch(Exception ex){

}
}double jwyfCNYlxFOyK = 50.3154; 
jwyfCNYlxFOyK = Math.Ceiling(Math.Cosh(5));
try{
Console.WriteLine(jwyfCNYlxFOyK.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}sbyte yPHhXmgUntnhEChKLS =  -114; 
ulong MgHNStVTpThBRAHft = 11898830049089486; 
byte mllyjqcMIaweUlbXAgI =  188; 
string EsDhLYxxWuHNIHdx = "jOOYfYkaRCtkEMAFMJUVbGdoleaz"; 
sbyte wTdiyYnDztqhTydltJ =  -91; 
sbyte XjBWEKzNaXNUnfAhunBpjjCEUMH =  -70; 
double sqXJyKDmUnPPyxFnGqEeYu = 3.324111E+26; 
double mNoFwW = 5.286297E-36;
sqXJyKDmUnPPyxFnGqEeYu = mNoFwW * 2;
object hulCwNeZGgoIVNYKfWK;
hulCwNeZGgoIVNYKfWK = 8.808945E+11;
Console.WriteLine(hulCwNeZGgoIVNYKfWK.ToString().ToLower());float saJOARa = -5.853918E-15F; 
short EXw = 14212; 
ulong SDHMQJYOYWqbZtFnQn = 65692574987468603; 
int zIPqxXjGkyCsLZCUJtqXx = 1508; 
while(zIPqxXjGkyCsLZCUJtqXx == 1508){
zIPqxXjGkyCsLZCUJtqXx = 699189;
}
}public void HJybcopqgaFciY(){
string MNVVUanVzImi = "ILomNYCZBowJjNJP"; 
byte zmPwSmPDGwCEwchRAopQxZCt =  60; 
uint lKmGgjTPWLlSbyjt = 47; 
double tCtnuAzszLwiuNETsPutCnkfnxn = -3.991864E-21; 
while(tCtnuAzszLwiuNETsPutCnkfnxn == -4.918863E+29){
tCtnuAzszLwiuNETsPutCnkfnxn = Math.Pow(5, 2);
Console.WriteLine(tCtnuAzszLwiuNETsPutCnkfnxn.ToString());
}float jHJaPINqVqhpzPZmIkklLgG = 0.04133002F; 
uint pqbGAebaqoyf = 317857483; 
uint TUdEjoiYkmJdcbeCnnyZTMJky = 64921700; 
double elVkpMKx = 1.809425E-25; 
if(elVkpMKx == -2.089826E-33){
elVkpMKx = Math.Ceiling(Math.Cosh(5));
try{

}catch(Exception ex){

}
}uint gwLWCCscUAclfuGpWyNWH = 19; 
ulong CiiezRwLYYQqToYnuOJOE = 73481277453604393; 
string kWna = "iYxJICgwPZdi"; 
ulong lpCafEWieWPohVVFuGoH = 82321211338326412; 
byte zTs =  232; 
float KVhxxaUAUstlAjFyTiERXCU = -2.559242E+20F; 
string dyVXldhVAyTjeCSfOJFsZxaajsaQi = "eIq"; 
string bYuhisoeaZYAoewgwjNYHyVO = "csNzskTcMRhtBwZLqaakSMLNUK"; 
uint diFcHngaDHIOeyDnnN = 774703386; 
ulong pxUDbpHZEJ = 83598526970962760; 
short iLslgwiyAhPKSPThhdmRtDdZbQyZa = -22057; 
string ANfGdIxGgiBwTcxjKeWJ = "zHGHtkpXiyqhhkeKneBIHKjcIfZtn"; 
short lsIbgAYKyfIXMinMSsAxaxatOxD = 31095; 
byte WKTCfQAi =  245; 
byte ucCZNyTuuh =  7; 
ulong VPqonKWZxliYekdzlJuogHuztb = 48368549285228072; 
int zkVnnoXRsbI = 8027; 
if(zkVnnoXRsbI == 474243){
zkVnnoXRsbI += 644526;
}uint owSVlMmwEhnceFOZaNfbSJD = 34; 
ushort UJKGnwmqUGoJDaUuJeYEIb = 60218; 
sbyte CyUGSNETZtMTDaHSnTuMeE =  -125; 
double UbtQeSNDUM = 6.680784E-12; 
UbtQeSNDUM = Math.Ceiling(Math.Sinh(-5));
int? JAERBanatQO = 8141749;
JAERBanatQO += 34878;string tWegEkOiYjSOlw = "idjNsABtmIFphuSV"; 
short JURxoQTu = -9648; 
ulong nwYcpAhHFKWSERA = 51241849124728226; 
uint IMPuUXgMUZlGmSVLn = 50318149; 
ulong nFQ = 88618541024422394; 
ushort INtmLMPgKSmtINTwa = 53411; 

}
} public class QAuh{public void EViDdLnujMkisbaOXUENEyqbMniVO(){
int mFu = 80211515; 
if(mFu == 69967){
mFu += 973586;
}int jVDKcZQoSGQwwfFhRPDsHELYE = 339339435; 
while(jVDKcZQoSGQwwfFhRPDsHELYE == 339339435){
jVDKcZQoSGQwwfFhRPDsHELYE += 826540;
}int Lmwzbkj = 46; 
if(Lmwzbkj == 426407){
Lmwzbkj = 193050;
}double kwwnxmkHkFgfIGdhOk = -2.761506E+14; 
kwwnxmkHkFgfIGdhOk = Math.Ceiling(Math.Acos(0));
Console.ReadLine();string UfVyPmSgYIUgNxiFfzaXK = "mlnIpLq"; 
uint HihqXxMxNWOCiTHxjhSEGYga = 85764774; 
long hWhQBSmY = 74843542722324168; 
float dsHkfDzTjJbhIeNDoQOY = -2.800193E-14F; 
int SfWJkFLefbPqEXQFqHIFW = 450731; 
while(SfWJkFLefbPqEXQFqHIFW == 450731){
SfWJkFLefbPqEXQFqHIFW = 745718;
}ushort WINMXtaHQ = 43427; 
int XbGowZqZjHtJcMuIPmwRnRjucFGF = 958581244; 
if(XbGowZqZjHtJcMuIPmwRnRjucFGF == 37840){
XbGowZqZjHtJcMuIPmwRnRjucFGF = XbGowZqZjHtJcMuIPmwRnRjucFGF + 627044;
}double SSaQwxdpodKlYt = 5594.323; 
if(SSaQwxdpodKlYt == 1.574405E+14){
double RuMjlfuHwNY = -2.447665E+26;
SSaQwxdpodKlYt = Math.Round(RuMjlfuHwNY);
Console.WriteLine(SSaQwxdpodKlYt.ToString());
}float JtSqYfoWafN = -4.809052E-30F; 
long zTuisdiMFXJkB = 33478148726671229; 
string GCpnwEALekdQGHuEmB = "ntoUOpfATVxHzupOIcP"; 
ulong emFayQo = 27574865724090252; 
short CYpdHPIFFwpSMCyNyjg = 13296; 
float AQAsmZdkEfx = -7.637674F; 
float eyyWxSuYhOgZPQ = 9.80203E-35F; 
int jzAFnDIBuRjxLaGqtMJOyAe = 350478053; 
if(jzAFnDIBuRjxLaGqtMJOyAe == 997903){
jzAFnDIBuRjxLaGqtMJOyAe += 275873;
}ushort cQnptWBiVoVyDPLdfuzyUuToLuIl = 44867; 
ushort lZUgGxKmuTDOzCbCcHCa = 39709; 
ulong cJXYFZonAAHJzJIeHxheEsGuOKSF = 49297054074985307; 
long fmeFdoSTDTwCVFy = 40507588716039630; 
long qdEmxRjxipUZYoqCUgZjd = 73266415577784643; 
byte gwWMknGg =  8; 
byte NnEMw =  110; 
ulong pOh = 1843694740466196; 
ulong VfOFcDfnnaX = 41108420247834847; 
long WxYV = 74853725890915121; 
ulong hPgsTHObOkCHFKXCBbimZKQKM = 57244525690474321; 
long mSVQapUJNtKcoq = 31901528948271653; 
uint XAPSgFZyWnLJGkhXeacp = 88; 
float GOqoJQXnunTBBcCitlfb = 3.130968E+29F; 
byte eEOyW =  34; 

}public void cHhltmKPfpynqqIxczT(){
double OSbMBljfcBqnJRUtQiVJyaLi = 6.082712E+32; 
OSbMBljfcBqnJRUtQiVJyaLi = Math.Pow(2, 2.1);
string CempqUiOXWcGomKcAJeVzBJnun = "NLNmRXOYJRVYdfEQk"; 
ulong okdVRTjiwbxYVcsw = 30382902208272183; 
byte KbQULyAUZqKPWuIOsX =  62; 
float VLL = -1113480F; 
uint SOWcJliitBpYAbdaLn = 2879; 
int iqOuEPpeueWVckwHfKkzwRHnsY = 707247166; 
if(iqOuEPpeueWVckwHfKkzwRHnsY == 331056){
iqOuEPpeueWVckwHfKkzwRHnsY += 245606;
}double GFooJuTnflcEZTGdhFefzqLAbFXF = -8.238461E-26; 
double RaQGhMwlPZYfV = -4.492598E+08;
GFooJuTnflcEZTGdhFefzqLAbFXF = RaQGhMwlPZYfV * 2;
Console.Write(GFooJuTnflcEZTGdhFefzqLAbFXF.ToString());string zOZSxSHpVTkNHVh = "RtufBnjZsTH"; 
int JsYEAUEaWBbBO = 44737811; 
if(JsYEAUEaWBbBO == 528310){
JsYEAUEaWBbBO += 990770;
}byte RkiVXkFZBElZWmBljgaFpVTtFN =  118; 
ulong qsH = 47734584853918537; 
double xWP = 3.590442E+25; 
while(xWP == 2.016797E+18){
double DWsQpjNSYikFGuwtyCEpG = 1.895222E-08;
DWsQpjNSYikFGuwtyCEpG = Math.Sqrt(3);
xWP = DWsQpjNSYikFGuwtyCEpG;
try{
int eUVQoCXILZVOZssXgcm = 4083207;
if(eUVQoCXILZVOZssXgcm == 50684){
eUVQoCXILZVOZssXgcm++;
}else{
eUVQoCXILZVOZssXgcm--;
Console.Write(eUVQoCXILZVOZssXgcm.ToString());
}
}catch(Exception ex){

}
}int iktzllksjxOmZlg = 642543502; 
if(iktzllksjxOmZlg == 343676){
iktzllksjxOmZlg = 744839;
}sbyte fetG =  -14; 
byte cHYSfGpGbllUbTdPKwhYnKcmLbimt =  218; 
int ektgohfBJcKlfSfjcoKjopL = 5811; 
if(ektgohfBJcKlfSfjcoKjopL == 73812){
ektgohfBJcKlfSfjcoKjopL += 440304;
}string iGzQRAXBD = "HolZRhSWZkd"; 
sbyte yPAaDEacdHQCGH =  -115; 
short fOhUJaAtNEnhyShZibaqA = -6975; 
double mDJxzkBQIofobCPOa = 2.086981E+22; 
if(mDJxzkBQIofobCPOa == 4.886932E+30){
mDJxzkBQIofobCPOa = Math.Ceiling(Math.Tan(1));
int[] lmqSFWPsWTOWOUOKeuywU = { 4706108 , 29293 } ;
Random xWMmXnwzcPEQuPHtfVRoi= new Random();
Console.WriteLine(lmqSFWPsWTOWOUOKeuywU[xWMmXnwzcPEQuPHtfVRoi.Next(0,2)]);
}double iLQdswDNDQufqYBXzRZtYBY = -1.012348E+18; 
if(iLQdswDNDQufqYBXzRZtYBY == 49544.66){
iLQdswDNDQufqYBXzRZtYBY = Math.Sqrt(4);
int? NqdSZhcSNtSiYsdgGsVJZRA = 1192967;
NqdSZhcSNtSiYsdgGsVJZRA += 48038;
}float OsZHCKnUZNWREmMUlDwlichCZAeq = -1.220958E+30F; 
float GERUniPITQhd = 1.123331E-20F; 
string oaDEdIOMkz = "bFWTeTElZxRHdNNssUeG"; 
ulong RbaspnYQfKQCNd = 56811968758914238; 
uint AQpowRfSUHNVS = 57428638; 
short jphQCZKxwS = 25853; 
byte qZEHSAYJwoEXIgpbeoGcNU =  134; 
short YQGTZYlFhoOqlUeqFO = -30788; 
int TTuuqDnlZxzncUJsQwpstASCLT = 525026; 
if(TTuuqDnlZxzncUJsQwpstASCLT == 123041){
TTuuqDnlZxzncUJsQwpstASCLT += 495820;
}ulong eXlzJxXthSociaxYPtMlAu = 4247331970478557; 
short DsQLgqNUuaqZlIKueMtNOBlLWtPkn = -20037; 
float CqBRkLSyHRR = -7.535197E-37F; 
string bONIOzDjcffPNCCGkhb = "VgTjuUVgWJNIVNO"; 

}public void CCeRZgzLeWeWelH(){
short ELU = 25556; 
int TEwnttyTZO = 8; 
if(TEwnttyTZO == 182350){
TEwnttyTZO += 473000;
}ulong ieyPZcKbwHqcTDYnknET = 11311844970839528; 
double SeWlMzQeFt = 2.914059E-14; 
SeWlMzQeFt = Math.Ceiling(Math.Sin(2));
 bool? cDdgPmgIRBygJkpOtRChlzbEw = new bool?();
cDdgPmgIRBygJkpOtRChlzbEw = true;uint MTcfZWAbMWVoBCQl = 9812; 
ulong HJKAwTyuDRhLBN = 7242059415018918; 
double PqoWQIkGReT = -7.325314E-25; 
while(PqoWQIkGReT == 2.031986E-29){
PqoWQIkGReT = Math.Ceiling(Math.Asin(0.2));
Console.Write(PqoWQIkGReT.ToString());
}string maHaIMIhPuFsy = "QpokHeBpZqjzRQaPdAWisnGp"; 
long MBDkZqS = 4827734094930777; 
double oRj = 1.009581E+27; 
if(oRj == 5.592824E-14){
double CLZBzsZOOsSGobBa = -1.575149E-06;
oRj = Math.Floor(CLZBzsZOOsSGobBa);
int[] xlpzBmUtDUU = { 2217381 , 47649 } ;
Random EdlPQRXFUhnXZFGQIZGayabowBa= new Random();
Console.WriteLine(xlpzBmUtDUU[EdlPQRXFUhnXZFGQIZGayabowBa.Next(0,2)]);
}int TyWclPLjU = 39; 
while(TyWclPLjU == 39){
TyWclPLjU += 351910;
}uint IdLEoUzDbXuoRMfmWuBcDtbWPLT = 61; 
ulong cbeEwVJEDzA = 3759619568745268; 
short SxtnbPOFbwWnGqnMhK = -10733; 
byte XFcjiFUHyOAwNkFLptgVZCcDV =  62; 
string qnFPeNUSqeTjTXEIyTS = "JbRpks"; 
float CBnWFnojjKqapN = -1.892629E-22F; 
uint bbYwecuUXpxnFMVzpQQXGFIuyODS = 921414616; 
int QmqPzgwkCKqYNWeFeSy = 403999; 
if(QmqPzgwkCKqYNWeFeSy == 443574){
QmqPzgwkCKqYNWeFeSy = 377169;
}int qBDebKQyswgaiuGwWmKHCKXNXzck = 9191; 
if(qBDebKQyswgaiuGwWmKHCKXNXzck == 882233){
qBDebKQyswgaiuGwWmKHCKXNXzck = 169786;
}sbyte ZFpoWzNiYRojQgwjKXxHDP =  -94; 
int GjxCLgaoUxqXuKdWOPg = 86; 
while(GjxCLgaoUxqXuKdWOPg == 86){
GjxCLgaoUxqXuKdWOPg += 691640;
}long MzhjyNJqnqImLoUVLempntKOaatN = 62763287903903069; 
ulong TjMsPGgCx = 27031263739318134; 
ushort tiyuVgjwQnBacjylcEVFdIz = 38567; 
uint LIBuECUdsdwQRlFkpnVUWpY = 39; 
float PKMKTVgxDUxLYgZRuQmNJZT = -6.494131E-23F; 
long SwKpaLOfuHtDUKQcqU = 52609211406398863; 
sbyte PRtAj =  -46; 
short SEx = 31940; 
byte Gka =  63; 
short TXCy = 9756; 
ulong WWfWTGJpEmUtExPjNzEMNqnEg = 47110367862156370; 
uint eNIDuCKbXfUP = 7354634; 
ulong ZdaeFICPdDyzoqcVddKyfVjj = 89923897647032662; 

}public void AWOyw(){
ulong kSzRelNGdYLDto = 51378412256178810; 
int gotPw = 4657; 
if(gotPw == 258496){
gotPw += 230729;
}byte teQCLuooTOQQSU =  161; 
ushort fhouLQTYkOBclf = 4455; 
ushort BLkOHKmQmBEYEwRnUheY = 2409; 
long XEAGPlIIMu = 42669358777521988; 
int opZItJPsYZyGSxn = 14; 
while(opZItJPsYZyGSxn == 14){
opZItJPsYZyGSxn = 610188;
}float FtbVfkKHWykZIFSlfxpKNWFCA = 0.01240083F; 
short EaSyAwnsNJgns = 2304; 
sbyte xHGoSshphdIPAxkMsmsx =  -35; 
int jETtNix = 394374; 
while(jETtNix == 394374){
jETtNix = jETtNix + 270475;
}float DkTC = -9.965706E-08F; 
ulong TqzuVfjMnUzxLYZu = 26967186838740055; 
float LgERmNydabo = 4.613364F; 
short KGiQxsEl = -1378; 
ushort NzgnXCBGwcixHLZHXX = 6269; 
short tcJZjgyX = -24310; 
double xKaYX = 6.141272E-33; 
xKaYX = Math.Ceiling(Math.Sin(2));
Console.ReadLine();uint LtsHCAzqtkmGcyiHuxbw = 51572; 
double UDo = -6.265699E-32; 
while(UDo == -2.088291E-07){
UDo = Math.Ceiling(Math.Acos(0));
object jZkbhRSDeDcpWRKTehWbLFSeJzjJn;
jZkbhRSDeDcpWRKTehWbLFSeJzjJn = -5.51002E-14;
Console.WriteLine(jZkbhRSDeDcpWRKTehWbLFSeJzjJn.ToString().ToLower());
}short ggsJHiotoQZyQWCiyJ = -32185; 
ushort PfqRQnUhaRFkwGjFbHRalPisjEzRw = 52182; 
long TGsbaRQVulnYwmupqEwciw = 4189342657834710; 
ulong fSsQniEj = 24105295555295569; 
sbyte BRuJDyT =  -40; 
string kEfq = "cSDUFciaklmPMBNb"; 
long HDVRy = 85278473040719693; 
ulong ByqtdEfqQi = 77622310727468338; 
uint OElwmaJgeAlZxAl = 41; 
long BClfJaZlPdMBzOsCWkW = 28013279902483364; 
float VPWsVYX = 0.001302522F; 
int mOTctYNleYoSz = 559810; 
if(mOTctYNleYoSz == 4588){
mOTctYNleYoSz = 777037;
}long fdToPiEPuYzIDTocWS = 16273100594931954; 
ulong tuCKRQhLZNEQ = 61111386220071863; 
short GhxCtpTlsstlwZyOQ = 15175; 

}public void EcioDaOfjQNCgdRPunpxSS(){
ulong lxXbKDctkYKByS = 22629854282505109; 
byte EkcABoT =  187; 
short cgcMxINuphtqqCuORIiqRwi = 19403; 
ushort BESVfYkJPOzTObJ = 13303; 
uint PmTBf = 45571600; 
byte XXIXmoXhjCwcwfJMHp =  247; 
float JGVdakF = 0.0002350612F; 
sbyte UwmMhQkEQTzJecu =  -95; 
byte AxApwoqMUmetiNwXhZTWFQeu =  24; 
short XRbHdtjY = 12804; 
double ZYwwWYAxLMngUunojJ = -3.248004E-32; 
if(ZYwwWYAxLMngUunojJ == -2.25436E-33){
ZYwwWYAxLMngUunojJ = Math.Exp(2);

}uint KQopqms = 271014263; 
int yjcflumqZxOsMcELVDPDZZk = 381736; 
if(yjcflumqZxOsMcELVDPDZZk == 187377){
yjcflumqZxOsMcELVDPDZZk = 426712;
}ushort hJceXzLGbfIBMVzDnODyuAZPz = 64815; 
int uMcVDEHJKXGljWNysAzBVo = 4259; 
if(uMcVDEHJKXGljWNysAzBVo == 455254){
uMcVDEHJKXGljWNysAzBVo = uMcVDEHJKXGljWNysAzBVo + 660105;
}long HXSYlocMYh = 19972444290581795; 
long xYBfReqHbDfYawqJu = 52345634304792345; 
sbyte doDMZWfSIxMhhY =  -21; 
long coPyZAWcOnhQyj = 15470955868017595; 
string EKlIdDAYWfnXpYeQGKlJYbjmTxA = "dkGVmUUomoYkOiUsWVwKi"; 
uint fVtWCBZOIdPULAsLweQDqMaUPh = 312435158; 
double QCC = -1.152468; 
if(QCC == 9.979259E+35){
double qhYnjSEkZGsZCuUAg = -1.403082E+21;
QCC = Math.Floor(qhYnjSEkZGsZCuUAg);
int? HmmtsZqRyMbnQuSVBXaMOakiC = 4378183;
HmmtsZqRyMbnQuSVBXaMOakiC += 65275;
}double HceR = 2.198225E+27; 
if(HceR == -1.652715E+07){
HceR = Math.Ceiling(Math.Tanh(0.1));
try{

}catch(Exception ex){

}
}string NhGumxZyIfDBRi = "zBlwMBSZyjkYlaUg"; 
ushort UmTp = 36535; 
float dhH = 4.630891E+24F; 
uint sXjJARoWUiikcyNy = 98669134; 
sbyte XVGhXuwIslaHIM =  30; 
short lHEPXsLgsPDqLTZJeCxqcLexH = 4734; 
long DDqRzHC = 45363004603446016; 
int qIsaYDjjXOBDhhuKDFPyLSdsId = 550189; 
if(qIsaYDjjXOBDhhuKDFPyLSdsId == 30717){
qIsaYDjjXOBDhhuKDFPyLSdsId = 110437;
}uint oWphFZgIBSaQxAGTtx = 156484202; 
sbyte EatUfHqCNwdFTzu =  -27; 
double pdslsRHXXZdGppxQsoOLUBpZ = -2.6677E+07; 
double aRdgsJCmiKsyhfN = -0.09024779;
aRdgsJCmiKsyhfN = Math.Sqrt(3);
pdslsRHXXZdGppxQsoOLUBpZ = aRdgsJCmiKsyhfN;
ushort hebpOlWxATTR = 23029; 

}
} 
public class oSbwpeXpjuOTqejFJe{public void THpfekMcpbBKBBwDRh(){
byte nhCmfCKmGDyCWGmX =  249; 
ulong jEbIoKLnsVDsxxGoLfdZDBAyNFYN = 69252675209110015; 
byte mDwnqqcjaepqCRVN =  103; 
long EUKLX = 12389454917263735; 
string TBFojhwE = "TxDMIIqPtRJHDSyUmU"; 
string PMGosXwigAWipfRCQsPqWxaUtEp = "guWmqIPhRnuKmhJTUApy"; 
string dVfMYsLMVOHNcMEJRJpJLDYfePO = "ZTnTHk"; 
short hGJ = -4109; 
ushort sGcBf = 61196; 
short IIhskCXZH = -19972; 
int NbBOfRwCBlZTlGjsdF = 82925113; 
if(NbBOfRwCBlZTlGjsdF == 220551){
NbBOfRwCBlZTlGjsdF = NbBOfRwCBlZTlGjsdF + 619020;
}long ZPeiYzSTsp = 88804133603459471; 
long gGqFWcCOOZ = 75507852357244657; 
byte btnTmPkupbcPYyhu =  116; 
uint WXeesAklD = 886619; 
short jglOqyshSgyM = 29616; 
string FKyiXgpEXnVtcDIWxKKGRlucZhe = "QHsuz"; 
short TaLtlTEoAwdoFjjbbnxe = -8814; 
short iQblNpVBVOVPW = 14052; 
short MnWUIWTBUGGqgcj = -14441; 
int CEkHQYFUiBJwnpLRKQx = 738231; 
while(CEkHQYFUiBJwnpLRKQx == 738231){
CEkHQYFUiBJwnpLRKQx = CEkHQYFUiBJwnpLRKQx + 331451;
}sbyte FuTFHkpiNlfyYpNNsRAsoG =  -50; 
short SiUoHKpKmZBoW = -30243; 
byte gYFFtzeZAFKRPwYeBkPcxUfDQOLp =  62; 
uint PpuiIMOatdUPca = 898926; 
float xOL = -4.024768E-28F; 
uint SbPFOPNbi = 525602197; 
ushort qtNkwYCfd = 36238; 
short ToRKKDiTWPKZwGohNTfXBFKcaB = 21883; 
sbyte gPzyMVAVqzqsCqDJOPQJtsjxutnGO =  43; 
byte ZTPZkCKKQlwuOFZngumMaB =  158; 
ulong cctHThJoHtICEylegTg = 86965463664458953; 
ushort KAmbuizqcWeNfhk = 21316; 
ulong WaHUHASQumZHuOmSW = 46585483932177500; 
double iKAF = -9.6724E-20; 
iKAF = Math.Exp(2);
Console.ReadKey();
}public void wbdYJlfonWMiMxioLZ(){
long uPuTEKdgUpQiEYbHqNZ = 23245125490914384; 
uint THBOoiGHkwYhYwXfEGNuCXXZA = 424170; 
double bYnDpaKpunP = -1.882204E-39; 
if(bYnDpaKpunP == -1.235187E-37){
bYnDpaKpunP = Math.Ceiling(Math.Sinh(-5));
int[] nTasOYGwbfbPeycM = { 4767609 , 10551 } ;
Random XUpxChJhdJLhOddDSETKXu= new Random();
Console.WriteLine(nTasOYGwbfbPeycM[XUpxChJhdJLhOddDSETKXu.Next(0,2)]);
}byte olbeUVmSTMIaNLskLpUn =  67; 
string mmTicMEwklUTxgYOQVWkqURQiqNK = "lWtnnPR"; 
string jfpqyOSSVeMCdssRHsxdaL = "TmZpCFhqgOdMQUaxcXpxJYHhFayIx"; 
ushort MzgydyzqmNNFOaHCWHWtfKopVHSM = 33384; 
sbyte VMQqmKyWYFHcJWtS =  -92; 
double BbBbScjRDyGNQfmR = 2.4086E+35; 
while(BbBbScjRDyGNQfmR == -3.269273E+26){
BbBbScjRDyGNQfmR = Math.Sqrt(4);
try{
int FHbqDluwEyaKVZPzO = 2184601;
if(FHbqDluwEyaKVZPzO == 16517){
FHbqDluwEyaKVZPzO++;
}else{
FHbqDluwEyaKVZPzO--;
Console.Write(FHbqDluwEyaKVZPzO.ToString());
}
}catch(Exception ex){

}
}ushort hTyFTBxUPySbzIKsUfo = 42376; 
ushort YnMMs = 10953; 
string KRwTHDLwAPEKyhfHkEFJafTN = "bUiShhm"; 
float NbHGagHzGAdNJRifCPFqPFXJKFICE = -8.632449E+10F; 
short yueaFDycDAtJxtdaNHsJDkhd = 30158; 
double AeUnSOWkxdeWmg = -2.853574E-21; 
if(AeUnSOWkxdeWmg == 2.739581E+37){
AeUnSOWkxdeWmg = Math.Ceiling(Math.Sinh(-5));
int[] KfCnYcAYHkVEeDDgJXjdc = { 1653048 , 2813 } ;
Random fiqFUdePcWE= new Random();
Console.WriteLine(KfCnYcAYHkVEeDDgJXjdc[fiqFUdePcWE.Next(0,2)]);
}byte ZoiBe =  12; 
double uEIY = 2.65073E+24; 
if(uEIY == -1.173743E+28){
double WuixTSiZZVCtbJ = 2.397795E-05;
WuixTSiZZVCtbJ = Math.Sqrt(3);
uEIY = WuixTSiZZVCtbJ;
object jkfXbleqYSzKEFjMdVaekkNduIgFN;
jkfXbleqYSzKEFjMdVaekkNduIgFN = 1.671517E-38;
}short UpCosXjoGOayNZycE = -8341; 
sbyte EQpasWqwzfDdNJpx =  12; 
sbyte jwzDNyUTmqk =  -24; 
double RYdZBWwGOQAlWLdJaAydVefnE = 0.001677799; 
if(RYdZBWwGOQAlWLdJaAydVefnE == -5.20674E-33){
RYdZBWwGOQAlWLdJaAydVefnE = Math.Ceiling(Math.Sinh(-5));
object QEpMXVWNS;
QEpMXVWNS = -14.37039;
}uint IpiSxadLCbXfmRQtyhMnJM = 296143; 
ushort tWhebWadcwIBZEtEIpB = 22465; 
double kWQsbndVM = 8.928792E-05; 
kWQsbndVM = Math.Pow(double.MinValue, double.MaxValue);
Console.Write(kWQsbndVM.ToString());sbyte iJNXUZlJfUYo =  82; 
float VawzbtbolKJXyPIbZuTGOc = -1.038355E+37F; 
byte LTXAPDtOBsNSeZhtBmkb =  196; 
float SYUXlXTNxmtJhsM = 1.008655E-29F; 
int nonhpTywCZOjURweBVGhQ = 60282603; 
while(nonhpTywCZOjURweBVGhQ == 60282603){
nonhpTywCZOjURweBVGhQ = 338945;
}double Pse = -7.716574E-34; 
while(Pse == -1389430){
Pse = Math.Pow(2, 2.1);
object lBKqCtnqSK;
lBKqCtnqSK = 1.42562E+19;
Console.WriteLine(lBKqCtnqSK.ToString().ToLower());
}short GBMCJYdGpRnbubkFingOF = -16113; 
ulong wsadeNNMlBVMdcKFVGJItmEAeF = 22638153558290413; 
sbyte YaBDeCPUgAB =  -94; 
int SZFfBhPBZAsVUKwDZ = 9220; 
while(SZFfBhPBZAsVUKwDZ == 9220){
SZFfBhPBZAsVUKwDZ = 91991;
}sbyte RzKtGenmpKfOHqYCz =  -122; 

}public void kjI(){
sbyte FBokjUXVzEJKegt =  -23; 
string EapPsUXTiTdVtjslPfPc = "wMkp"; 
string oxqIFfCdfhQhunpMdRDj = "qTsnGbFinfznmJyR"; 
byte PRfmdZXwPNJItNU =  41; 
double YuZdXVRcqUhFTVSsO = -1.328471E-10; 
if(YuZdXVRcqUhFTVSsO == 1.429531E-10){
double AHMMueMVTSALUkxbIcnjUOQbZ = -1.922876E-23;
YuZdXVRcqUhFTVSsO = Math.Round(AHMMueMVTSALUkxbIcnjUOQbZ ,1,MidpointRounding.ToEven);
try{
int GdORgwkpFBcTYcZScziJOLm = 6189632;
if(GdORgwkpFBcTYcZScziJOLm == 20024){
GdORgwkpFBcTYcZScziJOLm++;
}else{
GdORgwkpFBcTYcZScziJOLm--;
}
}catch(Exception ex){

}
}double QROjKuGRHOaQPkgiLsxcILMNZZZ = 1.119579E+23; 
double gEgRflVypgXGdOsmOVmffakKwea = -3.473561E-11;
gEgRflVypgXGdOsmOVmffakKwea = Math.Sqrt(3);
QROjKuGRHOaQPkgiLsxcILMNZZZ = gEgRflVypgXGdOsmOVmffakKwea;
try{

}catch(Exception ex){

}long tJypptyWPPjWkKVzqCbGqnuNqoDWC = 29711651620032771; 
byte ZtPwlXcH =  157; 
double afZcH = -1.388042E-18; 
afZcH = Math.Ceiling(Math.Cos(2));
int? QnjjTuJliAJIMgUP = 2714935;
QnjjTuJliAJIMgUP += 27961;sbyte yBAtsVFSqsuWWPnqpZYdJgyYdhm =  94; 
ushort lLFtWLSYwmmnBaNmaS = 35265; 
long opdmqdCokILzOfJqETDzqFMQuHxlR = 51183549936604863; 
sbyte IqJkFCPNUdVyZZkJLFm =  89; 
long pzcmuPjinQxnVhItgoFU = 85070950899245620; 
byte nnUabjhshaDMQfaMdcaqyYmRe =  37; 
int RbonWyChtUfy = 7771; 
while(RbonWyChtUfy == 7771){
RbonWyChtUfy = 814544;
}short KKnxVVtpbdVTHPBfMoPtFSsj = 28815; 
int KRJAn = 762624259; 
if(KRJAn == 765522){
KRJAn += 852993;
}short dlg = -12916; 
double QynbSbaZk = 7.652203E-14; 
if(QynbSbaZk == -1.19511E-13){
QynbSbaZk = Math.Ceiling(Math.Asin(0.2));
 bool? HwQQzmQDRRJsajuNOMqiXsV = new bool?();
HwQQzmQDRRJsajuNOMqiXsV = true;
}float XqTCPoBfRsafKLDPEta = -9.225458E-33F; 
byte XVGdubzjJQmiXanRUOE =  180; 
ulong hIEYTXDEOCKL = 62331122753242607; 
short xCFPoCfsBCS = 5671; 
ulong BwhBIecMzmeHJJIzsGaRlbueIaywa = 58382320069382330; 
float nnKdqKJTyLcjmEVN = -3.202386E-38F; 
string HIHMpwNNzXXCAeNECYcwZgaKX = "FNYzmKTaUGHXNWWWwAOBLiRskBZIT"; 
uint WOqRaPaLmIzkgXFkfzdM = 933511; 
ushort wWGnMLlhuZ = 45594; 
byte eWgwPBSlYLwHpT =  45; 
ulong gaqdwdiNYDxqQhmuPaDWqLx = 66738806137218098; 
string GSKTtKSYJRXw = "hEhlCSHTxwELaYSXfDLuZkoDCxq"; 
long JUzjhoIcwUZwzjyfhsLZaDnmbUXnG = 43524988698251444; 
uint KjUKJMYCaRxudHKXfHlhnqtyLP = 465621; 
int WaGAbQfTFCdtWy = 31; 
while(WaGAbQfTFCdtWy == 31){
WaGAbQfTFCdtWy = WaGAbQfTFCdtWy + 944684;
}
}public void UUtFsQyZUmBXFVxOFGUS(){
byte LeUw =  238; 
short OFkHPbglUJZJU = 13494; 
short xsCNDxosfuSioLXUi = 5233; 
sbyte opQToCLWVitqxCzcuCSq =  -51; 
float ujXXiMeBpFHnfDJya = 2.636256E-11F; 
ushort aCNNmUAHMPsxPwKsFCjQJdNPnsB = 48036; 
string GWwGtJkjWSGTFPnRphYfxkiHm = "OXzUuNCTjwbjbbguFsjPAXCybsOZ"; 
ushort OOsxUmMbySqLG = 56696; 
ulong fXYbGR = 21298777689468069; 
double yPInRFhUJDUqyPsIJ = 4.348749E-20; 
double YdRLixTPnRkBpBPdtcXfZJzOsXP = 7.721441E+32;
yPInRFhUJDUqyPsIJ = Math.Ceiling(YdRLixTPnRkBpBPdtcXfZJzOsXP);
int lMMa = 4639; 
while(lMMa == 4639){
lMMa = 831640;
}ulong oJXFkSoWFdBEFh = 84273381609029503; 
int RtFnaQBITVTnuGhkSJyhkhx = 79; 
while(RtFnaQBITVTnuGhkSJyhkhx == 79){
RtFnaQBITVTnuGhkSJyhkhx = 242849;
}byte fdjORnbWPETLJouqDLuKaMy =  140; 
long wzLtkuHiNjkRpXRMJFqyNUwYUGSC = 66128990027031561; 
byte tzdBCjgMuxTndT =  53; 
byte CcMzGCfw =  91; 
int MxemBmlTtlskSikxJz = 7612; 
while(MxemBmlTtlskSikxJz == 7612){
MxemBmlTtlskSikxJz = MxemBmlTtlskSikxJz + 311950;
}uint XOXYeiHDZYqGNRHs = 981675489; 
byte UoufxXbQhSNPMaoyWwgiFpjme =  15; 
long gdbKgKGItRChhQuh = 50750147343997917; 
float lFf = -4.294265E-36F; 
ushort CKdQcIcwH = 5663; 
sbyte YbPVbbCjYYXIFSfzbDlPhVGfpq =  -85; 
float sUeqFTYyJdfBjnDNxkkDFteSp = 7.960713E-35F; 
ushort FLSHsHdSpaYhaszfoYIct = 57632; 
sbyte smYQlbTRDOetmTRHAUgkluEaHNbMu =  -34; 
short FUofInGoghFmneTwhaEANV = 15171; 
double XjAZgxjAtPtFMZknRdeQp = 62.45665; 
double JiqwKegoZMuOUpAQwbtkWSIgtkIG = -3.594187E+22;
XjAZgxjAtPtFMZknRdeQp = JiqwKegoZMuOUpAQwbtkWSIgtkIG * 2;
ushort nLQtylEtKXcuoKdmQJIGGJw = 30175; 
ushort emc = 39567; 
byte HclzlfZnseZdceVGzT =  237; 
int PhGpsZxhqpeNA = 22228889; 
while(PhGpsZxhqpeNA == 22228889){
PhGpsZxhqpeNA = 697688;
}long mCKqxofzOjJHi = 4495381613944527; 
string VpDqKiTSlUaCHVZWKqJDfwg = "VqQygEgmkNP"; 

}public void TYw(){
short sxNhpnMiXxlyAIAjlwttwLINOqT = -24079; 
double MWtI = 2.890085E+36; 
while(MWtI == -1.104207E+07){
double hUQMfoRID = -1.113356E-32;
MWtI = Math.Floor(hUQMfoRID);
Console.ReadLine();
}short EMUlPbFMLIsfnfx = -6505; 
long czBxHKNpunlKPclcolLCcgS = 6879424730583753; 
byte McR =  242; 
sbyte jAAHp =  -3; 
string pTlQkKfLWHcmSULteVbJdawehf = "cDACYhWziLzhBUbf"; 
byte UmoOfCuzsQpzgJeichyedfWwCJH =  193; 
uint bUjjnTknyXgBPgiLFQdqRTJf = 772176616; 
short lyVHXEOxnXulXPLAXEqQmRdQKj = 29404; 
sbyte dsACqbZb =  -2; 
int sMxIwRuci = 16; 
while(sMxIwRuci == 16){
sMxIwRuci = 627164;
}ushort fXOGGkjFUKGNL = 29571; 
uint eyC = 437668; 
ushort QUZbVdcaLHHWDPGNBlcCgOpzMK = 17383; 
short fZWL = -27489; 
double GioCSJVfeBVkAKaiuTMcFC = -1.640976E-22; 
GioCSJVfeBVkAKaiuTMcFC = Math.Ceiling(Math.Tanh(0.1));
Console.WriteLine(GioCSJVfeBVkAKaiuTMcFC.ToString());uint DMZWsJnQxFJIeMNXPHYqgPI = 8907; 
short SBjqeJBHIhheeDO = -27553; 
byte MajZfKmRjGOdNcak =  105; 
short nFZEeePHD = 25201; 
ulong LxOhpnTuBemA = 54309904673799844; 
uint XVNyeVqdLXwlcIZULWDVMiqAJ = 5084; 
ushort zaIVTzTuTjYdytpeJ = 53919; 
double ktXFDfPEnOTuXD = -1.353757E+33; 
ktXFDfPEnOTuXD = Math.Ceiling(Math.Sin(2));
try{

}catch(Exception ex){

}string NPGFDSdHXLNopTNfJZBH = "loAsPQYawpVO"; 
sbyte bwIFbHszOeeyCnFMJhyoC =  -1; 
sbyte OceGCuRBi =  -85; 
long bwAaaNCumsnKwKIOqSiCnu = 4126682709235276; 
short EULmIxoOYMZw = 17967; 
ushort SNdjpajx = 41702; 
int AnRihJdVgCZufUEu = 540135; 
while(AnRihJdVgCZufUEu == 540135){
AnRihJdVgCZufUEu = 122629;
}uint HiuYs = 4550; 
string hXaXQFqsTnJSsYoMGHUm = "ptMMCYoe"; 
int dVMXXExNXdcqSTlokflNNER = 847969; 
if(dVMXXExNXdcqSTlokflNNER == 224511){
dVMXXExNXdcqSTlokflNNER += 274854;
}
}
} public class bmpCFWGaqiiUWigYBTA{public void OjTQaBenf(){
ushort kzsbQaqnpJj = 50460; 
string THIqTdwMbRSuBBdRsV = "TYwEhqXjFjEFwQlhUyWlpUzyCjwB"; 
ulong zqnQjGyc = 67299367147939272; 
ushort ntjuxKHZAXcYWyuuMm = 21526; 
int HKSYWoydWZpNRlyadUE = 20; 
while(HKSYWoydWZpNRlyadUE == 20){
HKSYWoydWZpNRlyadUE = 530816;
}string AxRjURYHJsxAujV = "XNkfkWtIsNxwCjSxSQ"; 
double eQwVSFOJIwjZoDDnpYnD = -7.442851E-25; 
eQwVSFOJIwjZoDDnpYnD = Math.Ceiling(Math.Asin(0.2));
int? fUDwGVYmBoAphJOGNce = 8763950;
fUDwGVYmBoAphJOGNce += 75798;string IDjePYgSDpjXKdUnIfiVanLdJ = "RszlPaFWuDjSkNBDO"; 
float HntmDnOFgQuRgwngPtqCj = -4.056547E+19F; 
long dQUwuyARtbmz = 57974417267535054; 
long xXHM = 30065346995238293; 
uint UiMsVZNuaJKytBO = 649262129; 
long atXPJobASNMNiyAglMN = 16942631662444228; 
string NtTcoyeXuddTBVHnVCZUuCiUD = "LqZRHDYIetTLTZujSJJmjuwJcBM"; 
byte kkYAuNOXDyCVTQm =  102; 
byte ZAjKBkpLpVqPgQOjUiUl =  121; 
byte mokhqqEhnSSMKnBLNiK =  111; 
float OpLgKWeBYiNTXwodYuM = 1.954198E-34F; 
double deWtyhcPQO = 2.062539; 
deWtyhcPQO = Math.Pow(2, 2.1);
try{
int HONeKpcGenGnby = 4384527;
if(HONeKpcGenGnby == 54726){
HONeKpcGenGnby++;
}else{
HONeKpcGenGnby--;
Console.Write(HONeKpcGenGnby.ToString());
}
}catch(Exception ex){

}ulong pGVcUSw = 21378766516010207; 
float qcDGd = -1.481226E-08F; 
ushort AnRUwmwLORAWwjpNYAlRIFhsLfAk = 36361; 
byte BGWshxsfEjjmyxhWTmS =  22; 
uint GpVgzio = 21252932; 
sbyte kYpzRhqHuJIzqKWDj =  28; 
double hesaHybXiQQYbMlySycfC = -1.773903E-30; 
while(hesaHybXiQQYbMlySycfC == 7.16424E+25){
hesaHybXiQQYbMlySycfC = Math.Ceiling(Math.Sinh(-5));
for( ; ;) {
Console.WriteLine(7.372085E+18);
}
}uint ZVSkYRmtGhhUSbKwl = 37532294; 
double pGsdnpKXfoCkyheLxqEL = 0.0006694511; 
double BBKDTKoAhKCDwBTHKwUFKQx = 1.616949E-20;
BBKDTKoAhKCDwBTHKwUFKQx = Math.Sqrt(3);
pGsdnpKXfoCkyheLxqEL = BBKDTKoAhKCDwBTHKwUFKQx;
string bRxPzCQNdUUWFqCKniULWsOF = "VAHFoYaROasW"; 
int ingmWYCMyzOFbctTJd = 440403628; 
if(ingmWYCMyzOFbctTJd == 187000){
ingmWYCMyzOFbctTJd += 171080;
}ulong TeRqiOIFLyQeRGQibhQKXYzUqoe = 67759041137142503; 
ushort gpaXxsWdEllHuacsfXUHWaUJyFF = 24063; 
float QBtnVRQNCHDRNdTEGfbG = -2.357363E-34F; 
byte YfxqARNDlWwMmgfkDtxndenQQ =  228; 
double qlpNoDpPwzmLTZG = 6.969166E+11; 
if(qlpNoDpPwzmLTZG == -0.09556606){
qlpNoDpPwzmLTZG = Math.Pow(2, 2.1);
object pfsQyXQX;
pfsQyXQX = -0.0002334793;
}
}public void RpDeTttZfxSRpSmhXYQPzh(){
ushort RhcCseJ = 32834; 
ulong PTyH = 23724378912049651; 
short hoVbCbmKTtmMqRIbO = -9346; 
string dtKqlByxm = "ffQBuciIYNQfZetaZAuTKLJezOFFP"; 
string QxUmhWQyFSguoxYtFX = "CXctpwyLc"; 
int FSXjaJfSleicQbD = 3817; 
if(FSXjaJfSleicQbD == 395334){
FSXjaJfSleicQbD = 842;
}int AGOdFSLkNRgmnyexon = 73288598; 
while(AGOdFSLkNRgmnyexon == 73288598){
AGOdFSLkNRgmnyexon = 229716;
}short edcDkyYpyFmGCaCTxlKOoTo = -17400; 
byte mUwgLC =  183; 
float HikGJoXaBjKNMknWKDEylOQ = -1.719173E-32F; 
byte UqTL =  226; 
long UKkGaAsTO = 33485031052723616; 
double uDTtUCz = -1.694586E-33; 
uDTtUCz = Math.Ceiling(Math.Asin(0.2));
try{
int MoLbqpdDnnjhFMpbsFyWydo = 7493816;
if(MoLbqpdDnnjhFMpbsFyWydo == 6745){
MoLbqpdDnnjhFMpbsFyWydo++;
}else{
MoLbqpdDnnjhFMpbsFyWydo--;
}
}catch(Exception ex){

}ulong plwaZQRiSidwJbhJVnIaChMl = 23993614775811179; 
uint cscWzmyaa = 27478; 
ushort QjTSAw = 49802; 
byte BjNlwV =  138; 
uint wSxYPHZPuEBh = 1956; 
float FMNBsmPXVRSjmSCzBeHD = 1.127692E+26F; 
ushort TKajNZlZV = 37790; 
ulong PZZhdzd = 26282953526410628; 
byte oYNqBHJmFjVGSnJcsK =  137; 
sbyte AkOxzdgzhLscxGG =  -115; 
float sqVAnhsWCU = -5.01233E+11F; 
ushort eeQNY = 58895; 
string COxADDxIcZCwjOEaWH = "DCmYA"; 
short YqQSdWlPQg = -29267; 
ulong PATRAhXVpCQRsQBbnPX = 12969260507958400; 
byte YxXhbocJxAqusJ =  202; 
string MgL = "CqfHpkwbHEENAqfSKqCzsSp"; 
int WWpRjjyzIyeSBLeIHQVNHTq = 15830404; 
while(WWpRjjyzIyeSBLeIHQVNHTq == 15830404){
WWpRjjyzIyeSBLeIHQVNHTq = 255152;
}ushort bAUuoekxufnRUbajGyRMc = 26920; 
float dFGJSP = 6.01138E-22F; 
byte dsGqQDxtCHtbyJhhPixJGGHO =  80; 
long CFFncEfXJEoInoKU = 798628733232064; 

}public void PFiyMSafcUOWtpysLAEAB(){
sbyte WMuanCCaTFEnZ =  111; 
uint MoCkuVXSyFPmchBiHklMqRMFCUVq = 39476538; 
uint kDSkZWYbODEKMo = 725781791; 
string HOLOFVDXKBaHb = "fucApoiKlkYPm"; 
string mVjmwwLKxXXJcmzahBiFsmzxP = "WCBQQIBe"; 
ushort RtHawKGDtHgyOjRneyA = 341; 
ushort zkzijHgohXPTjOUiiJHWche = 31816; 
ulong YCEdGxhdUwC = 70090279800035775; 
float cuVnRtCLqfZxTIGMessMgTzHXiICZ = 4.247632E+32F; 
float qVhIPOdHsEpDQGqPsuQojSiWo = -3.044942E+12F; 
long LJj = 74350682113008760; 
short Opidh = 3388; 
short HngDJzNktzfsHIVgwldFqn = -26286; 
byte edTNkjfunducGVdZmLRAXzgucJVAM =  100; 
ushort VDFuTzh = 18054; 
double sEBdPqZFeDnMcGXzLCnHJULlhdqJ = -3.578141E+21; 
while(sEBdPqZFeDnMcGXzLCnHJULlhdqJ == 0.1223418){
sEBdPqZFeDnMcGXzLCnHJULlhdqJ = Math.Ceiling(Math.Sinh(-5));
Console.WriteLine(sEBdPqZFeDnMcGXzLCnHJULlhdqJ.ToString());
}float XzAeQbYo = -3.928768E+19F; 
float YWhIeNncgepRIZIAfWSEbfbfTgVZX = -9.533249E+30F; 
double NHgZWfVMPdSiJxRtAjpUqWS = 7.589207E-35; 
NHgZWfVMPdSiJxRtAjpUqWS = Math.Exp(2);
uint AKPLfFmNJGPFmFfEnEcJUuCSCfFi = 76588494; 
uint ggYfZ = 826932; 
int pPfweFawUEwXQdeiWiaadgUfKyg = 99594879; 
if(pPfweFawUEwXQdeiWiaadgUfKyg == 735877){
pPfweFawUEwXQdeiWiaadgUfKyg = pPfweFawUEwXQdeiWiaadgUfKyg + 487176;
}ulong GxyyCEOMjdhqcTXctR = 72721357563894779; 
ushort qSKybAAMAMzxpw = 46525; 
string gdOcBtVWXuLxMQdjs = "suKoEtkEHsgBP"; 
sbyte iBuGtQzOCdfPpOVwmYZtHSRBwRXM =  17; 
ulong GCzDpKFYGTHily = 76996885008782685; 
ulong qJTjCSNjMHB = 4988656692861003; 
uint JGc = 930712033; 
sbyte HWUQMhkIQuyNfxyFXqeFhkkEuppJc =  81; 
ushort KNDPNKCJZWWoCHuElbf = 32919; 
float SxMqwwSDFZaSdtTIRm = 5.64802E-07F; 
int oWbOVMxb = 86344184; 
if(oWbOVMxb == 890592){
oWbOVMxb += 123685;
}string nfRxsXnKEaLEjT = "xXIhQoIAfgDnheWzNFwYKF"; 
sbyte qIsjUbTSShqjlYngHGLpPJfFID =  114; 

}public void OUbROQQhXyGdnLxZOMmPqosdTTdJD(){
ulong seTdXgBxxhfjiaIcd = 73347614657981518; 
uint XUMhGjVmZuIzTmpQHkficlRZxxWH = 90; 
byte wiDCKaBZnOJIxNmEGA =  138; 
uint tVLdNLinnjdje = 69; 
long gHLfUoDiEJjtbEtNibnSzXoq = 69294313346109155; 
float tVNozHiUjbZVmGKxVbxSpiqtu = 7.217102E+15F; 
double lZHAjkyZUDHKENmqQNXVbx = 1.910263E-30; 
if(lZHAjkyZUDHKENmqQNXVbx == 2.450818E-26){
lZHAjkyZUDHKENmqQNXVbx = Math.Exp(2);
object KPqBAK;
KPqBAK = 2.220173E-27;
Console.WriteLine(KPqBAK.ToString().ToLower());
}ulong YwKDJFwi = 31249486669535772; 
sbyte AiLSVtu =  -36; 
float AdL = -1.342467E+09F; 
float izkt = -3.737749E-10F; 
sbyte JWmtmLED =  -50; 
float AgAphpMwJmgwgHofXXQu = 1186.578F; 
long ygZFoWCux = 68322133806781535; 
double XDBfNYFHFBBSXzmS = -2.793255E-14; 
if(XDBfNYFHFBBSXzmS == -3.481843E+18){
double msxOUNbR = 6.167716E+16;
msxOUNbR = Math.Sqrt(3);
XDBfNYFHFBBSXzmS = msxOUNbR;
 bool? blGKpWUWhxyxpcMkSNlRCYBGhSLN = new bool?();
blGKpWUWhxyxpcMkSNlRCYBGhSLN = true;
}float RZp = -7.784291E+26F; 
ushort MZCz = 15050; 
sbyte gCJTwdIJwpyazYK =  54; 
sbyte TMHbFpKzWsDVY =  13; 
long TolhcOtiEZaTXebnudt = 18630650851715031; 
short BsVpTe = -6584; 
uint qetzzGpRCSqnBUjoOOtTZgHecdHfD = 98; 
int uMnbsHpcqwoYSZzLbfpbHaky = 4073; 
while(uMnbsHpcqwoYSZzLbfpbHaky == 4073){
uMnbsHpcqwoYSZzLbfpbHaky = uMnbsHpcqwoYSZzLbfpbHaky + 279752;
}ushort KAfhVmwhcQILsfkdhuCYJGhuC = 54223; 
ulong lqRcYyotTqQOKiZQsAfijqRRNzie = 71706650484338114; 
double NURSjREOoBGscbLENUB = -1.746617E-15; 
while(NURSjREOoBGscbLENUB == -1.225577E-23){
double mkmnRNNksshamYiJzYJFtWL = Math.IEEERemainder(3, 4);
NURSjREOoBGscbLENUB = mkmnRNNksshamYiJzYJFtWL;
int[] mHHQcfRCdmsB = { 4758692 , 11745 } ;
Random nBlwNBNqqFNc= new Random();
Console.WriteLine(mHHQcfRCdmsB[nBlwNBNqqFNc.Next(0,2)]);
}ushort AkdSyAFMCfU = 27934; 
uint wfztp = 708574734; 
short KlYWbUNEZy = -18943; 
int oITP = 509676594; 
if(oITP == 614523){
oITP = 323239;
}float iHFfQBGjV = -8.805183E+09F; 
uint DROCUYmBtRQACXjwIe = 109983139; 
long miyjXGwgVawltJgeLsGLQPOGnE = 14789250127398921; 
int eLbHflWPepFCoDtDAZc = 389505; 
if(eLbHflWPepFCoDtDAZc == 371670){
eLbHflWPepFCoDtDAZc = eLbHflWPepFCoDtDAZc + 207489;
}int GncuJeCaaqYGETZBYZTzKWjTSI = 21; 
while(GncuJeCaaqYGETZBYZTzKWjTSI == 21){
GncuJeCaaqYGETZBYZTzKWjTSI = GncuJeCaaqYGETZBYZTzKWjTSI + 430869;
}
}public void spBcWWyODskBuzF(){
ushort XuqijKejCaX = 21127; 
ulong JDgkEzzcd = 35715189776832784; 
int VRnepiGxTuVAyOgDYnmaWTVq = 453488234; 
while(VRnepiGxTuVAyOgDYnmaWTVq == 453488234){
VRnepiGxTuVAyOgDYnmaWTVq += 831368;
}string wCTuwyQcXcFJhXntyxujWCTcW = "jThApcpedWVMhBzkcUomiXjIJKyU"; 
float IQzymXVzMDsdTszLoTMLAYOKOm = 0.04314476F; 
ulong FwjknenKnBpFiMUfeSjX = 78841199847867589; 
long nwI = 23610290253763406; 
int kwVQbPIiRpSlalZRFbA = 261893143; 
while(kwVQbPIiRpSlalZRFbA == 261893143){
kwVQbPIiRpSlalZRFbA = kwVQbPIiRpSlalZRFbA + 70025;
}ulong lwOQBPZYOzSbYcRyfPSXLTRzsYGq = 25019751257175514; 
string sxbmlsNaBXxPOgkR = "tEpzCjIam"; 
uint PgHewH = 422797; 
double IAMqyJVjNSCwHci = 8.823124E-38; 
IAMqyJVjNSCwHci = Math.Exp(2);
object WJGHiCjJIefdsFQGiNA;
WJGHiCjJIefdsFQGiNA = 1.457945E-10;string zRUImSBMUzUOjxcnjHiedfgenoH = "TNCBwEMkXlzEjQjhBQSax"; 
ulong DYWRtKPuXkSSgWtsHKRqYiGV = 54642314514218459; 
sbyte ZCANtBhRBssXclgzkpGwRutaEyLk =  110; 
uint GMkWSSTqpjhImEoUOy = 236973093; 
short MfGhAiVQWUaOMlSooGsZ = -13809; 
byte MfynEXNXnoYZeqKtLwBmm =  166; 
uint dbzitUqsBxTbcUsSjXMAOlNXF = 6304; 
int ZxdHGkCAFEtXGAIyIqRcIdpV = 47862392; 
if(ZxdHGkCAFEtXGAIyIqRcIdpV == 514254){
ZxdHGkCAFEtXGAIyIqRcIdpV += 571446;
}sbyte NECaQwxJRt =  23; 
ushort CiNeBXWQBgVg = 10329; 
long tlYayfjcYIImVhlnzW = 6375416680335335; 
uint xGTINSpFw = 916513; 
byte gOxSKbJnSyjG =  48; 
long bXunIXGtOa = 54459301243969800; 
sbyte pOiWhPYFhKbEhbDyjIGuCwJ =  85; 
byte RHYEjpdXYnPzgSYbkeYWUQexLtJLl =  231; 
sbyte MiYqGFALAkc =  -58; 
long CNxAjETneaDDSRIsLcjyNX = 10436080238190419; 
ulong OZSWoMLCzyeXPcMJdgAaekYE = 47677452008331509; 
byte PTYMdZsIZpTWStUWjojNLDa =  69; 
string IgESGJLbmTKsdBIy = "JLXXKKbx"; 
uint AblSLJFqk = 13; 
long kQfblRlJ = 59825265293048537; 

}
} public class CxdykaaaZKAixse{public void CMgEIPGpDuOVBhEbtOEKHMaQfamhN(){
uint VmtbuxuPMRNVRmfBSfyQJ = 38987555; 
short MpzHDyEcLSIgfjGYYRTURiEZ = 490; 
ushort oKZ = 7967; 
float mIf = 2.155101E-26F; 
sbyte jSfwztNWSRLYC =  -11; 
double NAgixnsLMPfSCBcQJ = -3.698043E+12; 
NAgixnsLMPfSCBcQJ = Math.Ceiling(Math.Tan(1));
Console.ReadKey();int hHOJDOzIASSNKf = 38; 
if(hHOJDOzIASSNKf == 798650){
hHOJDOzIASSNKf += 316502;
}int ybzGeaYfkMqZjhgHzZBRffg = 64781197; 
if(ybzGeaYfkMqZjhgHzZBRffg == 45979){
ybzGeaYfkMqZjhgHzZBRffg = 700780;
}sbyte mQuiMkWJzKtIgwKWWmlSQQ =  69; 
long UJYuEWwcZejP = 81142809929155817; 
uint tXP = 803030; 
uint WxbMRRCYoUO = 607208325; 
sbyte BgXRQpAOHWU =  98; 
short Geg = 29089; 
ulong Ihdg = 85431006605251361; 
float yICY = 3.902662E-39F; 
ushort wQXXmYJQIaTdJscRmVNy = 40824; 
byte bamDXD =  38; 
short GZNniHIFssMOZQtfGLLNma = -30164; 
sbyte WXawMnAMgyKGPIHnheoB =  -33; 
string TysRxCAG = "FFmKIjHdlbSsKAQceAeVNIoTNhXu"; 
ushort kHSM = 847; 
byte WGYzknqjRiXCO =  89; 
string LjWptXBllHjRk = "YAGIhUgxDuXKWYZS"; 
long jmhCdUbZfMlEeBYpkzWgjh = 50594893389157790; 
byte YzIYtxwLGiPuj =  249; 
short LGOBWCBbYTUBDs = 30729; 
int SFGTaXOdlBbtUeJn = 981492775; 
while(SFGTaXOdlBbtUeJn == 981492775){
SFGTaXOdlBbtUeJn = SFGTaXOdlBbtUeJn + 459968;
}ushort GaBCUhnjkUquKELIZyLbAJdPlmItD = 52914; 
ulong hInZymidlRhaN = 18418601049600417; 
ushort WWMoHBVIwgWlcpL = 14068; 
ulong HQICdbXKpe = 63372578682360425; 
long CDpiMTQcCbDDaZgFRwSKsXkRTtHo = 67578489775040218; 
uint PjkUhnClbBBiqVk = 59; 
int zptmBFIcVckl = 9; 
while(zptmBFIcVckl == 9){
zptmBFIcVckl = 482853;
}
}public void QWpHqfPpDdyAkiM(){
string wXKpBkG = "RozdZnDC"; 
sbyte wiKchMofYzEadqdilLPipMKx =  -2; 
float lDQUXdupSzs = 1.992009E+35F; 
string lhHRkgaRcRNYA = "AnDpwcDiEHWKakLTWRhVA"; 
uint JHjlMOdjeVHtkPADYSdEaUCg = 11463725; 
sbyte WREkXyUKkQRwBxRPuSBdxnicwYQ =  70; 
sbyte xGFi =  -58; 
byte xwNVPqhaJdE =  13; 
ulong XGezIICPZIfFhukQshEzYy = 19470107041488993; 
byte tqAZAKHGQHwfPkjOPSMxAn =  197; 
uint WSLIzcewulRUlqJBXJqbHtxNmm = 170606633; 
ulong fDZgESVYq = 11286355620439800; 
uint VDGx = 228455; 
uint XulhWHhgoUeNQG = 341601292; 
ulong XYywtJtfUKBfBVbxXFexu = 29751922367957977; 
short mKOoEYDaDOQMyHiujM = -7100; 
long mppTLEqujGTEg = 51245586568521942; 
uint EqQmVFocSJiatzwTulHti = 174352045; 
short NHEnWmRLINDVq = -4499; 
ulong DbUXlcokEciWiBHhzThYLPVxsoM = 73304771408335819; 
double uhKssKUdUD = -8.938254E-30; 
while(uhKssKUdUD == -0.001954915){
uhKssKUdUD = Math.Floor(-1.739247E+25);
for( ; ;) {
Console.WriteLine(262135);
}
}uint QsZhKJ = 305761102; 
sbyte iRLOxjMLHbFRfbzQJumDydnJwzKW =  -23; 
uint yPWUFhsaRh = 6552; 
uint ZmYJTZCcHBfDbsXBP = 1610; 
ulong cjgUJq = 82040753390868304; 
byte nDANwhwPcfZcqscALwWcRP =  54; 
long HlwLNU = 12192661354225591; 
float sRDssfddVfLAjUzqRGPWaeo = -1.727098E-33F; 
short QoQKLcozXJpPBisPI = -20910; 
ushort UcxSuWYNnDqnaEmOe = 26792; 
string VxCICRnOJoIbIPI = "SYpOeCkwsbOXVEKliYHyCOq"; 
int jeYUoHlVBWgfmttlfpAc = 8580; 
while(jeYUoHlVBWgfmttlfpAc == 8580){
jeYUoHlVBWgfmttlfpAc = 873080;
}long kldIMfU = 11515155711110423; 
long gWGnlwzRBsqYtzwXdL = 51584963698074579; 

}public void mdBnyzcoXdUYQGQlODz(){
float Izzs = -2.839425E+22F; 
string PgsmQkOZdUBpjyTjMzlJhtW = "gDFGbWqJYCiWDMZ"; 
ushort lKJY = 35819; 
float FoGIEwGw = -7.315685E+29F; 
sbyte HhYhDRsIo =  114; 
long ARH = 14365761517415585; 
ulong JfEWNLIGMOU = 45606895031501179; 
long iMOLMXwBDlgkRTLOMD = 72855318726673947; 
sbyte uAAufMBuQkaO =  -66; 
sbyte COcLlxx =  -7; 
short JjwJbcFPdgcNN = -25510; 
sbyte qYBCzMpECZFx =  110; 
double bgZxDeMOhwZiYgPaXFTeyYQXWUSg = -0.01426526; 
if(bgZxDeMOhwZiYgPaXFTeyYQXWUSg == 2.824569E-28){
bgZxDeMOhwZiYgPaXFTeyYQXWUSg = Math.Ceiling(Math.Atan(-5));
int? gbPejmnbFAn = 6643528;
gbPejmnbFAn += 86583;
}short DsCLtQAsmyHEgeSiI = 23101; 
double MffWCfgEzPXR = -2.33331E+26; 
if(MffWCfgEzPXR == -7.229389E+28){
MffWCfgEzPXR = Math.Ceiling(Math.Sin(2));
Console.ReadKey();
}short dpwHZgVhcFPnj = -19773; 
float eZbRLMMtCCxAV = -4.401731E+18F; 
long QjtAlysVplB = 38923410563125828; 
uint JqEcfzLxlbSgWta = 902602; 
float KVuAlsAxlTmscgTgS = 1.86547E-05F; 
sbyte IAekITfLjiOQiSCnSsLGczDZKQadl =  -90; 
long MDbxpoEzuLcKzCU = 61429301659333936; 
sbyte eCSTcAbCpdGRfYDOllqxTBhVMMth =  -84; 
ulong GTBkOLFsl = 34879309561209488; 
uint BoGNcFtWhpfUtESZXGNPc = 87984258; 
sbyte LsKIoAzKAkWag =  -16; 
int OybggPFFoAHhJ = 88074765; 
while(OybggPFFoAHhJ == 88074765){
OybggPFFoAHhJ = 340272;
}byte oSQHkTtWYRiguoiSZnWNe =  91; 
ulong qaSqCCPfuMAFkAWVlJpJybosLD = 34400711855945283; 
short bbuybdMJ = 19671; 
int AsbCxcgxpMBHYbMKVhDD = 2052; 
if(AsbCxcgxpMBHYbMKVhDD == 201436){
AsbCxcgxpMBHYbMKVhDD = AsbCxcgxpMBHYbMKVhDD + 284264;
}float uoHDgVaKuXhTVCZzCuw = -7.538421E-17F; 
ushort BmbtoHgqgjBbafXBdbbpYZJHN = 13880; 
byte QTyqbzfoeCLd =  63; 
int KwA = 363330582; 
if(KwA == 356966){
KwA = KwA + 384813;
}
}public void dSBFwHdtOC(){
byte jaCZIwCpYcsZZlmlUPVBcdX =  156; 
int KVILzeNWkAytVARdoaBCGj = 8517; 
while(KVILzeNWkAytVARdoaBCGj == 8517){
KVILzeNWkAytVARdoaBCGj = KVILzeNWkAytVARdoaBCGj + 763501;
}long cVuXLH = 26945965649087212; 
ulong TxJZOXfdfBCPlAzYl = 14813289100623480; 
string moCiGeXfjeLseQ = "RjeUbePnRVq"; 
byte UHzFNCTOEqVLotoUB =  192; 
short IhFfBZqzbKnoTulDGgFDXD = -15596; 
float iMMAtqh = 0.2404439F; 
float csdNkwNTcHSUhNnuiPPfUMW = -1.391107E-19F; 
int ItgptBzBmdRsDQuQDqSENeXZjLOTp = 728615; 
while(ItgptBzBmdRsDQuQDqSENeXZjLOTp == 728615){
ItgptBzBmdRsDQuQDqSENeXZjLOTp = 87084;
}uint qzuJmbpqxlpAWtawblEwhfJNGjRq = 7435; 
sbyte UTlTyUAdHINzztudMeEkBe =  71; 
byte IdcjZZKfFGcggXgMAPAsjnlQ =  193; 
int iVjPeadQR = 871864; 
if(iVjPeadQR == 60285){
iVjPeadQR = 349826;
}float qWFgyzbooSGtSmyuNCkwQXg = -3.458337E-28F; 
short HhRoSwyzRa = -24314; 
long iBQhyyZjxmVChzY = 40500344255823971; 
uint OXuPGtGQNlphYHKqfj = 911505813; 
ulong WAebAhEnOssFKAFuQUpMu = 10644655451918393; 
byte DPdbULFTzl =  126; 
ushort PtWjMLtZGcpKSSjFSOjFePPGzLJl = 580; 
float UPKxzYeetRnXNOqc = 4.310889E-08F; 
short zpTUZEqMabhZODwVhwL = 8434; 
ulong RNQSbShJQRMlbFWQxUItd = 6107967884791669; 
ushort ITPAfdhDzEskpnCCRYqYR = 64698; 
ushort AeCKRBMOthdtmeRNosi = 52501; 
long saHJC = 41914427179235143; 
string zLYSESikioNhFJzQE = "uMCchZnkUouUdePkRIkXhmjQzu"; 
ulong YPtSVoGoawD = 17945323790533902; 
long uSRGHMgDLFNk = 88753015379379080; 
uint wektQooZ = 19; 
sbyte FXDmnDzNoRPPGfSZnpxiSjxNwmWR =  -63; 
long EeaJhtoKlaNVEo = 33568146452989413; 
double wCQSb = 8.691363E+09; 
while(wCQSb == 2.647481E-07){
wCQSb = Math.Ceiling(Math.Atan(-5));
try{
int VmHEyJFt = 7897825;
if(VmHEyJFt == 2076){
VmHEyJFt++;
}else{
VmHEyJFt--;
}
}catch(Exception ex){

}
}sbyte sTBulDDTuELidcCXGPRXK =  -36; 

}public void sYJmoqnsU(){
sbyte kFhkgMOoKhyeIHjoWajiYZGhxEon =  8; 
sbyte RcgSXPwIe =  -85; 
ushort fVUaHecsqKXZkYaODxBNE = 59964; 
int COm = 522365; 
while(COm == 522365){
COm += 573177;
}short mDEIjAOUhegIYPHe = 10437; 
long fjoSzSRDRZFXwUyNlZPp = 78146467588477313; 
long myqWkqXKePsXXVEgMOCbweTsup = 28262585122085846; 
ulong GAKgQD = 16150897472696889; 
long NKAhEYDUigPZ = 52740660857662257; 
string kSjUwczcUexuQ = "yEEIWPX"; 
long VWhlSWAownszHYJEAxflKEJ = 34131478101595932; 
ushort eoXmRHfuPafyKRGUd = 29280; 
ushort xdqmhoosIwfZVWCKLpJQZSUEE = 35237; 
byte YXQTVFfbXLfDqONuVfDOoW =  45; 
ulong ESFiOKGbEtKGlepQAMajfzle = 13987091352820763; 
string qMBTi = "ACO"; 
ushort OJbm = 62096; 
ulong iEJfHBIg = 47578997784046797; 
ulong pGEaLFLRbNYwMCHSGz = 66669293770279286; 
double dhkCnKHjHhTduQAVhRVa = -0.001387039; 
if(dhkCnKHjHhTduQAVhRVa == -7.457722E+18){
double BnSGcNTCyhcVyk = 0.0006902196;
dhkCnKHjHhTduQAVhRVa = Math.Ceiling(BnSGcNTCyhcVyk);
try{

}catch(Exception ex){

}
}string FTAkUYileUepbHtdaB = "ACgRxVGWcobBwjlUCjYVezf"; 
ulong QSYQoqRdUFczcdpobgXdtKf = 33998393823684454; 
ushort dGcHgNXjyBG = 12022; 
ushort gJHdqhMugNidzzPy = 15519; 
int hYymIVtaTViAHqqNtYF = 27846259; 
if(hYymIVtaTViAHqqNtYF == 492192){
hYymIVtaTViAHqqNtYF = hYymIVtaTViAHqqNtYF + 303097;
}uint oAwa = 503; 
int CJwZiYhTAX = 5452; 
if(CJwZiYhTAX == 724436){
CJwZiYhTAX += 416370;
}uint TqDoXXiReCyVUycdQdYZ = 725634; 
sbyte jCcWqhNZCHjlLlMyQqTHwF =  -111; 
ulong JqIyAkhkRexVmBXO = 83823456469631136; 
uint fSAqGzm = 671316; 
ulong Cbxca = 12286970516565449; 
double RDJoNpXp = -1.762069E-16; 
double gJWqQlQAxcXGoVHuwILOtcPFM = 1.000704E+12;
RDJoNpXp = gJWqQlQAxcXGoVHuwILOtcPFM * 2;
try{
Console.WriteLine(RDJoNpXp.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}short AzAfNKcSofxtCIuVEMB = 5871; 
string tlnFbXLDMpKmBStK = "EXSCmIMDJEgUstNGZe"; 

}
} public class SiJfhNugKn{public void CDThWI(){
sbyte gFCbtuEIuWWIBNeSwVRLPTBcnCbFb =  -77; 
ushort ybYKp = 8728; 
short EeajNHGOyaNbQyimdAORtY = 8522; 
sbyte RkAPwCnGggLTpoIW =  -66; 
sbyte zwo =  122; 
float GRtzsGyRLWwfZczzxKqyuN = -1.932621E-30F; 
float RHLeDl = 1.979421E-34F; 
ulong VgOCmbGtlQmeCZZGIwqSI = 50725888488583705; 
byte WmOEoVQjsgRTeTMe =  185; 
string EkMWcIDDNwWZZcpqRNxKLSkYMQuA = "bNuCyGnjqLWN"; 
short OxgS = 21827; 
ulong VspulMaOkmncjLJDz = 14041046258094374; 
short xCFYDMRgudhlUnxfzIKisfUJ = -11864; 
ushort uupPxUXElHoLpsFnzqpDOM = 26567; 
int mixqIVIhayNeoLsWLjAjXtROR = 341665; 
while(mixqIVIhayNeoLsWLjAjXtROR == 341665){
mixqIVIhayNeoLsWLjAjXtROR = mixqIVIhayNeoLsWLjAjXtROR + 669901;
}double XjLClELIEpVRjjHYGdCVuVT = -5.505833E+12; 
while(XjLClELIEpVRjjHYGdCVuVT == -555703.4){
XjLClELIEpVRjjHYGdCVuVT = Math.Pow(2, 2.1);
try{
int qgaomgdSSfqkGUgSCQxNTaS = 6142479;
if(qgaomgdSSfqkGUgSCQxNTaS == 71437){
qgaomgdSSfqkGUgSCQxNTaS++;
}else{
qgaomgdSSfqkGUgSCQxNTaS--;
}
}catch(Exception ex){

}
}string mWCBAXeSqn = "ndBtTDUdHUGzpz"; 
double UeGmTYWtOyPLpzTXueQN = -0.2236176; 
UeGmTYWtOyPLpzTXueQN = Math.Ceiling(Math.Acos(0));
int? twkPBlDCMol = 8024311;
twkPBlDCMol += 55274;double qMGuojbXNVTxKKhI = -0.2815819; 
qMGuojbXNVTxKKhI = Math.Floor(-4.604509E-15);
 bool? RjDZxyIuXIZJ = new bool?();
RjDZxyIuXIZJ = true;short hNfqjsDflqtH = -1602; 
float xTQRXmxt = -4.414254E-25F; 
ulong PyjZAHqoDaBocf = 32611823249471968; 
ulong gqueMuHguVN = 26961747033450364; 
ushort COSUOmhzeqqLT = 57027; 
sbyte lEiutVgsM =  33; 
ulong NEBRqXGEDkXgzIo = 2371792996134533; 
string bZGIAigzKMCQAtW = "FKbGbVMGDW"; 
ulong pMaHAtpXZVdcRJPW = 23972844710302089; 
float xfnaeDuIhXMUp = -4.476826E-33F; 
double KGYPsgQRPZ = -7.88727E-18; 
if(KGYPsgQRPZ == 8.816804E+33){
KGYPsgQRPZ = Math.Sqrt(4);
object kWyyWGdZzipDitbsO;
kWyyWGdZzipDitbsO = 4.673931E-05;
Console.WriteLine(kWyyWGdZzipDitbsO.ToString().ToLower());
}short XskMcjhBQASkkQBYAQA = -705; 
ushort lFFVHFsFZmKSlXsypSjnqN = 23627; 
long AqgSQ = 56767627990885369; 
int ikAGBsFEGcAHphuzlV = 145665356; 
while(ikAGBsFEGcAHphuzlV == 145665356){
ikAGBsFEGcAHphuzlV += 180616;
}float JaRMpipGfgmNiMVOBQqLBbRIukq = -5.626442E-15F; 

}public void zpLGKEMUMqqKcnlzMdlzLXSjuIij(){
short RVpiJ = 29903; 
string cqcHdatQYDxWAniSlB = "BkcwDDHAxuUGnxi"; 
string xeTRREJGOwGENyWhBCsZWAexBXx = "XiApl"; 
byte hHGKIuPPgbWKWX =  116; 
ulong RbZWaxqTgFJFVpqAcPCURLGgLXw = 64388085174259944; 
byte AgjoRBPgjk =  183; 
int UiWTdgwS = 582; 
while(UiWTdgwS == 582){
UiWTdgwS = UiWTdgwS + 623064;
}uint jqQgkDI = 5946; 
string RaCMKZcH = "DCCtPxndiEkUqXZUVzaqBGncWFSK"; 
short NDZBGJPyqOo = 5151; 
sbyte QpA =  -117; 
uint PBcpyXmzyjEENSQIUQspfyglb = 445391565; 
byte GeQhAClBK =  16; 
long pagjMRpkhxKKlM = 4373456198584244; 
float OwnpF = -1.879203E-23F; 
ulong UPyZbofgbejwfWNjSJwDh = 27291441601548330; 
int kWa = 9330; 
if(kWa == 805139){
kWa = 984524;
}ushort YgHKJABgMzPkC = 31153; 
long YGYDgweYAe = 86475180729955909; 
ushort DsNRcSUCxMzSkHwkaTlEcJkEUKSm = 39282; 
ushort ObPL = 40110; 
float AoBDDN = 5.832566E+17F; 
int dJwnCTUThiXtaIWcILRx = 801506690; 
while(dJwnCTUThiXtaIWcILRx == 801506690){
dJwnCTUThiXtaIWcILRx = dJwnCTUThiXtaIWcILRx + 953951;
}string gyXhdjjWXQliUSzJ = "fnTGAfWV"; 
byte npDTFQuNKlPoQzaeSAJCdq =  172; 
long KjVHqbmUzV = 79909499593580884; 
ushort QywFunxmIIWHVG = 59125; 
int gsjDwsDuLVSpaZE = 2337; 
while(gsjDwsDuLVSpaZE == 2337){
gsjDwsDuLVSpaZE += 302417;
}byte FZlQZ =  249; 
uint MNbXwmTwfTk = 35087519; 
int HJtwJiJQiVoGDiCDwkFfEZbhZtbf = 94165423; 
if(HJtwJiJQiVoGDiCDwkFfEZbhZtbf == 990560){
HJtwJiJQiVoGDiCDwkFfEZbhZtbf = HJtwJiJQiVoGDiCDwkFfEZbhZtbf + 49571;
}int GEQhdWS = 13; 
if(GEQhdWS == 239567){
GEQhdWS += 570833;
}short ANAtIHkxpgDUetDHBEuXRuyc = 9388; 
sbyte bscPDHocYAJXCuhBbBbOKEaQJjsYe =  -49; 
ushort CIjemqamPqFgRGTDKUxDGs = 19725; 

}public void qmgtnqXIcXxUkdm(){
long PZtZUYdpQnkUVgjq = 4712819537225446; 
double UjZq = 1.809215E+10; 
UjZq = Math.Ceiling(Math.Asin(0.2));
object cReUqzHEiSpRunqMhFCuGiWdXyTqw;
cReUqzHEiSpRunqMhFCuGiWdXyTqw = 1.046058E+23;short whjYAqmnhuUUbWUG = -12650; 
sbyte IoaySLDZTDXxDIDDoJsVVXMJECL =  -87; 
byte tgbKHluKsmXstMkXqmzuFWXma =  179; 
int UqauNciRwTbq = 60; 
while(UqauNciRwTbq == 60){
UqauNciRwTbq += 561266;
}int zGj = 684669; 
if(zGj == 446504){
zGj += 172391;
}ulong WJbPamDGqDtDzkhlZFSoy = 23970948258577504; 
ulong IHjeIgQpSwXmyedMmCGmSq = 33297006923645813; 
float BRLVngAfinaTATeQlunioD = 3.393078E-13F; 
sbyte oCZpxfEmMHthzgbBeEpFMWbqoAzJx =  117; 
double NoRYfTSUpmhzx = -2.777245E+32; 
if(NoRYfTSUpmhzx == -2.118069E+20){
NoRYfTSUpmhzx = Math.Pow(double.MinValue, double.MaxValue);
Console.ReadLine();
}ulong UcuOyDRC = 3921976544016527; 
int dDWbneQQyGXCuUOuuKkh = 2416; 
if(dDWbneQQyGXCuUOuuKkh == 934115){
dDWbneQQyGXCuUOuuKkh = dDWbneQQyGXCuUOuuKkh + 848774;
}string SpRbYqonnSPoXHelFVWsquBGHxp = "ahexcUYgOVDcFAfDVVjLhpjaW"; 
long yQluDazmfNslcJYyJt = 8235664554499958; 
int KXdBtpCpCBabgmZPEJhcj = 520439371; 
if(KXdBtpCpCBabgmZPEJhcj == 940330){
KXdBtpCpCBabgmZPEJhcj += 756437;
}sbyte DKpwPEiJSTNhIVhtRHFohgceDA =  82; 
ushort yxDsyZRKHqJRfL = 52569; 
ushort CXtolpiTLxHlSoMMmwyjPjVpKko = 59634; 
uint wmKI = 870156261; 
string kfuANEj = "OcXfTpsjfsbcFbb"; 
byte JauCkqTIejqQpx =  88; 
sbyte xlYsifUB =  8; 
int bDjegdeEQWELdyOfauQVKWtFP = 710; 
while(bDjegdeEQWELdyOfauQVKWtFP == 710){
bDjegdeEQWELdyOfauQVKWtFP = bDjegdeEQWELdyOfauQVKWtFP + 21799;
}long bHDamKmsPpoTBUUuaTgMLh = 22216135865912426; 
short VabRleNKEDVGeMnuifUIUXaUCtx = 30758; 
float NgbtYBSdh = 1.345916E-30F; 
ushort hmAeTa = 29119; 
ulong OIaXZhoTJPSqQOEL = 78257834298798978; 
ulong IcoDXwxnsusaCLOonpFQszVtgm = 86740805014062544; 
uint FfRxUCxjs = 24106721; 
float JKkUZCigVxL = -3.403019E+15F; 
ulong nQTukCLhAhWQbMaua = 60072220952243591; 
short JJgWi = 19186; 

}public void VDTSHPXCMMcLhwUcLwsTuuhKPTx(){
string tkObgQzBiyEVFiW = "UjKOH"; 
ushort QWBCujcNtWdtBoKbG = 27732; 
uint XOXQkkFVKMmSWKXeG = 793; 
short LjZCPx = 20543; 
byte eseUEbZ =  182; 
short GhcIJlx = 21396; 
short xqskwmFXnwPOLCHVNcf = 27835; 
sbyte YUMYIDqptFzGMOaUzqycxUuRyB =  -124; 
ulong tLwACsyzyFbGYcj = 54894080697790643; 
uint ydBU = 138012; 
uint RZUcfkwEwbNwxzSjCs = 15658634; 
float PVXxeoWFVyBbhdpmYFyq = -2227.832F; 
string HZlp = "jwtLjjYfKggH"; 
float oVwpPU = -4.971184E-11F; 
sbyte CkmTTQecHkLbgqYTsgozqL =  24; 
ushort hlKyxdQoJiZXflTbdaGwnOd = 22739; 
double hdItRGQfLFwGjhVNz = -1.844969E+36; 
double ZbAuhqCUWNHosugZBoGyQz = -9.420969E-06;
hdItRGQfLFwGjhVNz = Math.Round(ZbAuhqCUWNHosugZBoGyQz ,1,MidpointRounding.ToEven);
try{
Console.WriteLine(hdItRGQfLFwGjhVNz.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}float uomfmJmkfDLCdLaG = 1.225813E-06F; 
float onytmjEY = -7.997866E-26F; 
short SEaTYIfKWHxQQMnKf = 587; 
double hHlfSUScfnfXliKAKgHipknI = 1.922668E+11; 
while(hHlfSUScfnfXliKAKgHipknI == 5.952473E+30){
hHlfSUScfnfXliKAKgHipknI = Math.Ceiling(Math.Atan(-5));
Console.ReadLine();
}int NAWslbNUTDIlVpympMyVgxQat = 763162; 
while(NAWslbNUTDIlVpympMyVgxQat == 763162){
NAWslbNUTDIlVpympMyVgxQat = 627024;
}ushort mQSpnue = 40271; 
byte XAH =  243; 
ushort QggpMMqKWZInwmDWndJBFWcqtoXu = 55849; 
uint GstRjBcxCAiuoNQjWAbkEo = 370118865; 
float PXyOW = 4.017104E-08F; 
byte Wcx =  89; 
ushort IgawDOFDe = 558; 
float piqxaXfLfZNEqZbLUhBgGZWhV = 1.025566E-13F; 
short usOaaeeL = 400; 
string lLfIuyeEMUJXAOk = "liCcHQBLHYDp"; 
float ZtecUVTnOyTOEpLwnmQNSCZZSxku = -1.812613E-06F; 
float JOUpSyTpzXwdjSljalKpBhf = -6.097364E-10F; 
sbyte zKzmqDef =  75; 

}public void wDRtBoUgVQZkau(){
ulong GnCJxVNmwGaLdCeNxOGKTW = 7897204084397209; 
long yGXRF = 24846988950523688; 
byte uLlO =  63; 
ulong SJnIze = 74110863355773015; 
int ipNfTpYfFa = 832; 
if(ipNfTpYfFa == 485720){
ipNfTpYfFa = ipNfTpYfFa + 437791;
}short OoEWIVdsBQYq = 28520; 
int yczAJlCKhdyRsUdJAIVKoLmIS = 52; 
if(yczAJlCKhdyRsUdJAIVKoLmIS == 141641){
yczAJlCKhdyRsUdJAIVKoLmIS += 251822;
}string ngVbXGflFwTkeLzyhAKbV = "NQQIHLNCaQtheNhIC"; 
string zLouyOqcsbIDcOMufkpYuAcu = "RWHL"; 
short XJlxRywsToJuBsmp = -8381; 
short cxlcwZIUsHRLq = -28128; 
uint URsVWKTxJMeDcPCEnsldLG = 436380383; 
sbyte dMbFqpZItAbyysEPt =  47; 
float SDeny = 7.586771E+25F; 
uint oaVMtGgueuOEwxcCQUTBVlgCeLCHO = 34; 
uint IdzfLdMxwopgBygcM = 9177542; 
double edUtHJeAHXcocOsExeRgdBXRwEnw = 1.047745E-36; 
while(edUtHJeAHXcocOsExeRgdBXRwEnw == -15.83303){
edUtHJeAHXcocOsExeRgdBXRwEnw = Math.Pow(double.MinValue, double.MaxValue);
int[] wKVFyP = { 5181366 , 76339 } ;
Random zjsZKaxAgMGEDfEIfMYZKKZU= new Random();
Console.WriteLine(wKVFyP[zjsZKaxAgMGEDfEIfMYZKKZU.Next(0,2)]);
}string XbjUzdZAwzHBRqNRBAwXRcPSzPdPk = "AWayDfDMeAgelIf"; 
uint tXiubsc = 87; 
byte wJZjnXZKBtO =  158; 
short uWRNRhNZHLRcdyEXoJUkpxA = 27593; 
ulong IwXPFqTGHw = 18419121407514260; 
short TCgEefMdz = 30714; 
ushort WDAqbSfBizRhMTGQHUujDFwgouL = 32366; 
int gIYpABFmujjSduKYSkAVxEEm = 3443; 
while(gIYpABFmujjSduKYSkAVxEEm == 3443){
gIYpABFmujjSduKYSkAVxEEm = 648935;
}sbyte VsqijwueltBkaKBaxqD =  -2; 
double tDeLXhtkkMcEXLcCOxQwK = -2.718824E-33; 
while(tDeLXhtkkMcEXLcCOxQwK == -1.095943E+12){
tDeLXhtkkMcEXLcCOxQwK = Math.Sqrt(4);
int[] ALzopnnBHuXMM = { 259421 , 16055 } ;
Random xoYEKJUpkQV= new Random();
Console.WriteLine(ALzopnnBHuXMM[xoYEKJUpkQV.Next(0,2)]);
}string ZYblIou = "iWy"; 
ulong hOAadKznhfVpxjAItoel = 61556160405779056; 
short HlXmhyqVYzEURSfVlfUtQIsB = -12916; 
uint DAYLyZtAJti = 4; 
short qGJzQFQGSTgVYZU = -4254; 
double gPwZOgVysyCceTcqpMSR = 3.49181E+18; 
gPwZOgVysyCceTcqpMSR = Math.Pow(double.NegativeInfinity, 2);
object nSIJhFfYtcBSXQQdSIFQMXCm;
nSIJhFfYtcBSXQQdSIFQMXCm = -1.334245E-32;
Console.WriteLine(nSIJhFfYtcBSXQQdSIFQMXCm.ToString().ToLower());short KmoRHTJLCTPFp = -32517; 
string JswbKAZEC = "gJoq"; 

}
} public class LVs{public void ucpNJRS(){
double XiY = -1.348347E+14; 
if(XiY == -2.086617E+14){
double mMpFMZxjSd = Math.IEEERemainder(3, 4);
XiY = mMpFMZxjSd;
try{
int jtFNkSCYHtdNYbbsPHHX = 8631666;
if(jtFNkSCYHtdNYbbsPHHX == 48705){
jtFNkSCYHtdNYbbsPHHX++;
}else{
jtFNkSCYHtdNYbbsPHHX--;
}
}catch(Exception ex){

}
}string deXfidTJneWCZbuiB = "cqUASswQux"; 
ulong OTdkVWdlHEOnSldIWPbGJEyJ = 44869726501162943; 
double qdyOfzenJADGHHoO = 2.53852E+07; 
while(qdyOfzenJADGHHoO == -8.738392E-30){
qdyOfzenJADGHHoO = Math.Ceiling(Math.Cos(2));
int[] pVzdcOthHdjPOGMuFkmm = { 1187512 , 97637 } ;
Random oFLVRILnznEsdiHQQoGaOu= new Random();
Console.WriteLine(pVzdcOthHdjPOGMuFkmm[oFLVRILnznEsdiHQQoGaOu.Next(0,2)]);
}float VKCtUOLUbqNhkgIThQHxLxeF = 6.99075E+32F; 
long XVzMbcNbN = 53308003499126155; 
long AtVlp = 71666869298093387; 
sbyte XgTbghtHfcxRPCnP =  9; 
sbyte iTcJhuEYnxQBwJhlbxaCWfNPjU =  2; 
long jQuAfXHUyMHDjfjoD = 1215441891228729; 
double fnzm = 9.707665E+32; 
while(fnzm == -7.044949E+09){
double CsFLuYMZDBISsamXyTVjUB = -7.313559E-39;
fnzm = Math.Floor(CsFLuYMZDBISsamXyTVjUB);
Console.ReadLine();
}double IffVUpnPMpZRXThdmKFTupxHGfkcu = 2.156139E-21; 
if(IffVUpnPMpZRXThdmKFTupxHGfkcu == -881644.3){
double cTTuQPgadSzjtJwfGqJA = -1.166375E+17;
IffVUpnPMpZRXThdmKFTupxHGfkcu = Math.Floor(cTTuQPgadSzjtJwfGqJA);

}double MsmLdcwNaKgGRaYKOM = 8.469815E-25; 
if(MsmLdcwNaKgGRaYKOM == -3.348611E+34){
MsmLdcwNaKgGRaYKOM = Math.Pow(double.MinValue, double.MaxValue);

}short wJPUakSu = 31269; 
double cNbXZocEWwRsC = 7.884284E-21; 
double uAwbxxmlZKWzVpct = 4.06357E+15;
cNbXZocEWwRsC = uAwbxxmlZKWzVpct * 2;
try{
int tQEqmuNmAhmnD = 4982777;
if(tQEqmuNmAhmnD == 10719){
tQEqmuNmAhmnD++;
}else{
tQEqmuNmAhmnD--;
}
}catch(Exception ex){

}int ePcKZNSyIWsYBSlDlZJuyUjPQpB = 91; 
while(ePcKZNSyIWsYBSlDlZJuyUjPQpB == 91){
ePcKZNSyIWsYBSlDlZJuyUjPQpB += 751049;
}long iQWqYEKOhmZwgRP = 29683304395013598; 
float lMBmTUkTucMpJLRNfbDRU = -2.856195E-17F; 
sbyte DaDinEDcIMRiCnsODNOcqRMt =  54; 
string LcPSytzqUJKgzimaYZ = "FOkcsHBezLnjyjaRZT"; 
float PdXzfSHoTdROu = -0.1823682F; 
ulong axlWoaGGzmXyqmge = 39119268118695094; 
byte uTBPpKlq =  252; 
ushort zBdqSHGJSCRLJPsYYOdW = 15907; 
long GMeK = 73301878616322486; 
byte AUpUptKDiSaYMmE =  235; 
uint NyyGOXKxGCEak = 8640095; 
uint UNcWqtdDmJXCZyqj = 36; 
ulong Tfs = 31326591412425849; 
float NJiyzxdt = -29.27316F; 
short UHemQgonAGOwSbEleuGpdp = -17634; 
ulong MGXMIRF = 30234471646144757; 
string MgVOGySSnIVXIEupedC = "CSOiqtbyhtbe"; 
byte FeSwgieUtjKfRxM =  195; 
int ZLdUsNylwu = 89928606; 
if(ZLdUsNylwu == 695128){
ZLdUsNylwu = ZLdUsNylwu + 179367;
}
}public void pyKWsmIAFAkDFLtNNeEwLMfXVNKiz(){
int xIyxpXqHywuOmmJRKqhSMgOoEq = 3003127; 
if(xIyxpXqHywuOmmJRKqhSMgOoEq == 489099){
xIyxpXqHywuOmmJRKqhSMgOoEq = 17821;
}string OVhodPxyfZpYTLlVaRpL = "oTiSZl"; 
short VVt = 15334; 
int OKwntxnbOYGzaPK = 649561; 
if(OKwntxnbOYGzaPK == 506086){
OKwntxnbOYGzaPK += 487756;
}string xeDfxkeWlmmylGEWnSnejmBlBstnJ = "fYMKZiQqSRKctl"; 
ulong yWdOcIlXPiEq = 31846579219867783; 
long HugXaywhUoAiUxjUs = 62074403835402763; 
ulong UOCezGVpJIfIPDHnoSxjxGtzJCWHQ = 189031821508444; 
byte DYf =  132; 
byte QYmnDqInE =  141; 
byte dxWKoBUaTwpbnV =  23; 
float kpeflSEJsmzZXJVmXoOdTKFHlt = -5.925019E+10F; 
byte HlQ =  99; 
short ipREtaWnw = -30766; 
double wOpHqNaCLzUTzVxsGmiEpJe = 0.0003024373; 
if(wOpHqNaCLzUTzVxsGmiEpJe == 2.45505E+26){
double yImzKaVdtR = 1.18854E-13;
wOpHqNaCLzUTzVxsGmiEpJe = Math.Round(yImzKaVdtR);
Console.ReadKey();
}byte hzMD =  73; 
ulong VbRNhXFowJdQVEKispG = 48516510481291096; 
string BXPyInIlFXmWBHiKjozypulJHxo = "mIiBFtVMot"; 
byte cWNttlJZGUW =  176; 
double GfLfQkZuZWONqgLxwQ = -9.310874E-26; 
while(GfLfQkZuZWONqgLxwQ == -4.50768E+09){
GfLfQkZuZWONqgLxwQ = Math.Ceiling(Math.Cos(2));
object mASQwyKNI;
mASQwyKNI = -5.379457E+10;
Console.WriteLine(mASQwyKNI.ToString().ToLower());
}ulong KAlaySGRNT = 4379027885498320; 
short oxFhtOmSHXxQJEgWkJo = 19869; 
int xyUGlUfxkQhjZnV = 64; 
if(xyUGlUfxkQhjZnV == 766517){
xyUGlUfxkQhjZnV = 207651;
}ulong KHGlktYHzbQZhn = 66648632521911381; 
uint SagaGkRnGajYHIVdli = 17705205; 
sbyte OQUgmbaeQb =  -88; 
string klXbnjDtgdPVK = "gAyZowEmpgzYPmCkiQGnLRohu"; 
double VHNelDkUFsbTGQXVREpw = -7.933563E+24; 
VHNelDkUFsbTGQXVREpw = Math.Pow(5, 2);
try{
int XazNVyFe = 8950753;
if(XazNVyFe == 81761){
XazNVyFe++;
}else{
XazNVyFe--;
}
}catch(Exception ex){

}double dNboXutOLBNxDULVpoCNEPLwio = -46825.08; 
while(dNboXutOLBNxDULVpoCNEPLwio == -8.141911E-25){
double eeHiKlGqWPwGyHUaFojpn = 1.503796E-08;
dNboXutOLBNxDULVpoCNEPLwio = Math.Round(eeHiKlGqWPwGyHUaFojpn);
Console.Write(dNboXutOLBNxDULVpoCNEPLwio.ToString());
}ushort aTulMDQzSFsjHfezFiRtyOWJVgZDC = 38597; 
ulong fDLEfITmEPswMhS = 52681901882848265; 
byte LpGDnXmSUdduCsXpFQJwCXNnHYH =  94; 
long fbAIuqmKoRe = 31871852020176027; 
ulong jTQuODqVoqUjEXcVUY = 76343670610685406; 
long KWeeTMWiJmGOVOaB = 8773553532249282; 

}public void MJJbBwJURltlPqSaUmHs(){
float gBqfGR = 3.469479E+20F; 
long BxhcngiSXUeCInxcxgRPClQ = 87883688036794485; 
string GMsjPNNePwXcFTcKEww = "SkoGDBfxObeCcWbDdTIteLNRTCXo"; 
long lyWfNuxMHnHqpNpxaJRBGL = 82359453833104559; 
int OcQIGWjHfqltNOuZByBoyhfCK = 252897007; 
if(OcQIGWjHfqltNOuZByBoyhfCK == 970471){
OcQIGWjHfqltNOuZByBoyhfCK = OcQIGWjHfqltNOuZByBoyhfCK + 985200;
}int PDmXJQdzjunzuoeNzXpBnt = 95520488; 
while(PDmXJQdzjunzuoeNzXpBnt == 95520488){
PDmXJQdzjunzuoeNzXpBnt = PDmXJQdzjunzuoeNzXpBnt + 686641;
}int hsIJXlgcRHyQmNRdpGiSya = 8129172; 
while(hsIJXlgcRHyQmNRdpGiSya == 8129172){
hsIJXlgcRHyQmNRdpGiSya = 302405;
}long fuUAmWQMqSGxfKdzgTgjHe = 5430479567371006; 
short sfBdWOBUmFopmbVZaYobiAebT = -22647; 
byte mmAPDO =  20; 
double ZCstmWcQ = -0.07520492; 
if(ZCstmWcQ == -6.240968E+14){
double JSbzjODuHaf = 3.18811E-19;
ZCstmWcQ = Math.Round(JSbzjODuHaf ,1,MidpointRounding.ToEven);
Console.ReadLine();
}sbyte pHeVmIKFJjunJtWYznIlB =  13; 
int uWJE = 8234; 
while(uWJE == 8234){
uWJE += 915684;
}ushort VqoyTVufBIRZPYFXcVjN = 381; 
string pZfkSTmgNWochZjtF = "BDNfJPw"; 
byte elMKIUQYzOLD =  60; 
byte dcAVyYTDFFAzOkpp =  200; 
int hUbmIisaLFUGnplbH = 5039; 
if(hUbmIisaLFUGnplbH == 229860){
hUbmIisaLFUGnplbH = 683610;
}sbyte hxbOh =  -12; 
double LeGcQSMeXhXKZkhDjKaoShWkThAN = -1.948183E-20; 
while(LeGcQSMeXhXKZkhDjKaoShWkThAN == -7.703613E+36){
double ZQyOJxeMzfXnAFzyHeEi = 177292;
LeGcQSMeXhXKZkhDjKaoShWkThAN = Math.Round(ZQyOJxeMzfXnAFzyHeEi);
int? ZOLEQLpHXWjTDB = 954313;
ZOLEQLpHXWjTDB += 68602;
}short ackQieOUMBVNPppDuF = 26070; 
sbyte SqVNGRAXcAgsNHUMuLzNnZqEIml =  -85; 
short APlIhLocDm = -14617; 
double bPwcJwRaUxtISZsesFqIO = 8.875748E-13; 
if(bPwcJwRaUxtISZsesFqIO == -0.01277723){
double kzpppPmk = -2.421757E-06;
bPwcJwRaUxtISZsesFqIO = Math.Ceiling(kzpppPmk);
Console.ReadLine();
}long RgbEjAuZneiyztXDB = 60162621168096903; 
string ThXIOcwgdItfwIyaSbO = "pPJuqgwjSTIcgVBRpywwdmUUhVaH"; 
float wqNgodzDpIHknjZpYBgHKwHsSdVOm = -8.511157E-31F; 
ulong UzqpaGxsOgO = 68288516746821547; 
uint nZLunkiLpU = 856295; 
float MJZYUKnWnSEZ = -8.712109E-23F; 
float BGobHzGWMwnIzm = -3.427915E+32F; 
double YmRYGQaqMSQogYRTtVE = 1.681526E+18; 
if(YmRYGQaqMSQogYRTtVE == -6.888508E-34){
YmRYGQaqMSQogYRTtVE = Math.Ceiling(Math.Cosh(5));
try{
int sfTLjOMgtuSLl = 4394523;
if(sfTLjOMgtuSLl == 50547){
sfTLjOMgtuSLl++;
}else{
sfTLjOMgtuSLl--;
}
}catch(Exception ex){

}
}short PkgPUhpoWGCxRQTfHycCXjHEGobw = -15143; 
byte kUQWWlGYDWUqck =  197; 
uint cYj = 460175372; 

}public void aGEEiFfUaIipCaUXUazAFcS(){
int SQatodxNfQqs = 45290615; 
while(SQatodxNfQqs == 45290615){
SQatodxNfQqs = 867522;
}long UlzgeejCYjXNZkqOfCLFMEwjDF = 33741803811372726; 
uint sTnkUVPWTFsygFqBheg = 8188; 
short ASGwVHNEoMLxlnBKQP = -1091; 
short mPxBLcceEhPdoOtnBTpfbdLwifmIu = -11840; 
uint XHmcjUjFLzLF = 354970048; 
double gxpVjkXiebsGQuLm = 5.160893E-05; 
if(gxpVjkXiebsGQuLm == 5.259479){
gxpVjkXiebsGQuLm = Math.Ceiling(Math.Atan(-5));
object ZAloZRqLmzQuQXDD;
ZAloZRqLmzQuQXDD = 2.020653E-15;
Console.WriteLine(ZAloZRqLmzQuQXDD.ToString().ToLower());
}double UNKHouKXonbGZhMwak = -3.283884E-24; 
UNKHouKXonbGZhMwak = Math.Ceiling(Math.Tan(1));
int[] GwKhpqdWkJOLgQKFkjl = { 2339824 , 32576 } ;
Random khmLukdaExkVmwFyGujdE= new Random();
Console.WriteLine(GwKhpqdWkJOLgQKFkjl[khmLukdaExkVmwFyGujdE.Next(0,2)]);string iKNoYebUouyiZOfncsPzTzMk = "YQZheYRlU"; 
byte pMFqjaXahoHHeQBTC =  142; 
double XDcaobMjwGXeDFXqOzoSBL = -1.439542E+35; 
while(XDcaobMjwGXeDFXqOzoSBL == -3.269642E+30){
XDcaobMjwGXeDFXqOzoSBL = Math.Pow(double.NegativeInfinity, 2);
object BSifdn;
BSifdn = 1.424961E-14;
}int qofdajHixgE = 10205544; 
if(qofdajHixgE == 808739){
qofdajHixgE = 102366;
}ushort wZkuUoqoGXWaVhpk = 2655; 
ushort BJD = 61904; 
long BzgWyR = 54293593407333740; 
ulong UbkAgIGQqFteUQTcDluQj = 34868178112632289; 
sbyte klROBM =  -87; 
string SqGTzVTdqnDJRYADUfETGIQlyi = "cctpulBYSMnIauXGLDytnslHHF"; 
sbyte kBuwocQbY =  114; 
sbyte paHQGyEAIbfJZPF =  54; 
ulong KQenNssSsXFWIOuadnMxbi = 48386699572796735; 
uint llmst = 24; 
short NeHVMusnZMpZtLC = 18413; 
ushort FWHxSzALwBcHabyYndwNT = 51073; 
string xPheSCLEXtIsRqm = "wbYSlsL"; 
short wyzuJQQTiZdpuTUPz = 12785; 
long ehXQmqRQlleeT = 54552350103994745; 
uint qjRbidcNIdVzmBIgsYaI = 903957006; 
double SwQDoxdDGAGHwpJ = 4.783267E-26; 
while(SwQDoxdDGAGHwpJ == -2.395027E-38){
SwQDoxdDGAGHwpJ = Math.Floor(2.283639E-32);

}int UtQQxmWTBQDuTFkiyAljg = 41350254; 
while(UtQQxmWTBQDuTFkiyAljg == 41350254){
UtQQxmWTBQDuTFkiyAljg += 480433;
}uint juYXJaVNCRLpaVKzitgozGCAwCjYt = 65022109; 
ulong VqxFxkFpwWICU = 5672587399189465; 
uint TADgQWtQjWWfEnUMhC = 56774963; 
long YCxtmwOlxfwNEfUdOtbfzFXaly = 64682299785181452; 
uint LyK = 39964894; 

}public void AQKRLPSeNGaExtEwgQpDYGRYRFET(){
short JcdBfhqwMpti = -29011; 
long qDhyppUJAZUZLZ = 37738671317458706; 
double xeMdajPDgE = -4.294543E-38; 
while(xeMdajPDgE == -1.5924E-35){
xeMdajPDgE = Math.Ceiling(Math.Cos(2));
Console.WriteLine(xeMdajPDgE.ToString());
}float ZNooUqzVBpROmHU = 2.338279E-23F; 
float MmsMBTwzcSJmiRXhQGghWbw = 1.201754E+08F; 
double KMkLkJGfjHaPxASwdS = -1.940766E+12; 
KMkLkJGfjHaPxASwdS = Math.Pow(double.NegativeInfinity, 2);
float buLdkySmRTNVZpRWqbDxESjdTpUfL = -2.299202E+33F; 
float OdfFBbhA = -3.026021E+29F; 
float LPaFqEDZxwjkIkVDMD = -7.037213E+31F; 
float lmLsQZuXpYpAhBUCtQuTNs = 3.745021E+30F; 
double gFNLyXKZEOU = 2.871314E+20; 
while(gFNLyXKZEOU == 0.0258628){
gFNLyXKZEOU = Math.Truncate(gFNLyXKZEOU);
try{

}catch(Exception ex){

}
}int FKOLHMQGmhSHcLVggmMDPPiy = 750894417; 
if(FKOLHMQGmhSHcLVggmMDPPiy == 430952){
FKOLHMQGmhSHcLVggmMDPPiy = 369654;
}byte ZOkNUqdInZaCX =  168; 
ushort IlNkQaZPBqMTQwiUTAIoAilQieoQP = 41820; 
uint pWOkGcEYPwl = 589895; 
byte TRgIzaiAAZXbMMiaTRxETHYnQ =  234; 
long dWJkYlxZIZaogxnk = 77657013745936515; 
sbyte YlSTREsJwEqMdBojtURg =  -19; 
ushort OFPexUJEWcqm = 13510; 
sbyte hyXzaLUXXqYxbzUbkCTenbzV =  69; 
ulong uJMmecEMuiXwIUISOyzIebbgMscBf = 3225314598653917; 
string tyMBSmgkkHjLypeVtIGCNadHwh = "hWutHKFej"; 
short iTFgRUWnEuiQgHGEg = -8486; 
ushort eLaYoQ = 17566; 
int TkV = 827755; 
while(TkV == 827755){
TkV = TkV + 613600;
}sbyte KhESYgn =  56; 
string yLPmyy = "GSbGtmKinmoiTHCklwWhhahDDC"; 
float BfptaSpOhDqUTJ = 7.455733E+27F; 
double hAWBERBpMGKzdRsnJi = 1.434791E+32; 
if(hAWBERBpMGKzdRsnJi == -4.387149E-13){
hAWBERBpMGKzdRsnJi = Math.Ceiling(Math.Tanh(0.1));
Console.ReadKey();
}int cyUHJtjEBKzQtBE = 2601; 
while(cyUHJtjEBKzQtBE == 2601){
cyUHJtjEBKzQtBE = 729891;
}ushort TxKOucFRpUzaYmBBeMiYs = 51898; 
uint uBj = 44890954; 
sbyte MIO =  -128; 
byte GPCWThVUBGaNQxUDxqeXItUTUthUM =  88; 
byte pBzWIbzID =  35; 

}
} public class ViXHMfilxVmqmVIbAZbhiwOxpYY{public void uSywBzYKXNwOEgOqdjkaZaZIBZM(){
uint BqOhnREzRROWX = 74; 
int FbHytfNXVu = 794806669; 
if(FbHytfNXVu == 748164){
FbHytfNXVu = 475072;
}short wIHookSlsydcWl = -28855; 
byte KTXgkbjsyKJXo =  6; 
uint HqXHoNxFhNoUTyIMIhcG = 3047; 
byte yXRBindxGuoUfGeWytZ =  115; 
ulong AacXphYTpqg = 399234766766528; 
float AhNHbcfhYp = 1.592876E+29F; 
sbyte RylYVyNdYNOwMbC =  114; 
sbyte SbZSoqneSLsauycUezjVPwB =  -20; 
byte SGqeGbkZbfzZhVKQGbLXtqtRXdOkY =  112; 
uint IpzJONeqQuwXxepnkY = 477747945; 
long HBmAhDmab = 30418974930518242; 
float uhZVwwSOAZZHcSwKYwnFDdQsRtCN = -6.317359E+25F; 
byte XCqfHFSdpOOacXCcWizGOnuqqHeb =  227; 
sbyte HQcKBCoctEk =  -52; 
ulong PoTRzRnuEi = 57755106292150362; 
byte hkambBHgecEYFbD =  87; 
byte wncmlTas =  134; 
uint SNae = 1453; 
int GQYjhoBGMD = 213780601; 
if(GQYjhoBGMD == 608319){
GQYjhoBGMD += 770763;
}double UYgmKZfdloDTm = 4.116172E+36; 
if(UYgmKZfdloDTm == 5.28149E-09){
UYgmKZfdloDTm = Math.Truncate(UYgmKZfdloDTm);
try{

}catch(Exception ex){

}
}ushort OhYy = 55836; 
long WRRMdOJBuXLSSkts = 38782710803421673; 
short xxnCbleqaeBKlEHU = 23129; 
int FCSwQceQgLUVftEqmm = 916644620; 
while(FCSwQceQgLUVftEqmm == 916644620){
FCSwQceQgLUVftEqmm = FCSwQceQgLUVftEqmm + 203495;
}short lfAXVjqgNYFazzXxXHgjwpJmu = 10201; 
string fRbIgXRYpjPKEYGFMlOtY = "RTde"; 
ushort ylDmjIaWXXUCHYfNRxGxKdBiUHL = 4333; 
string EcBWBd = "DBVXJIPsmNaWdcHzakuM"; 
ushort EXXWQtRaLybf = 18311; 
double zaxMRITKgWfWEBMgsnjWY = -9.170028E+18; 
zaxMRITKgWfWEBMgsnjWY = Math.Ceiling(Math.Sinh(-5));
try{

}catch(Exception ex){

}byte AyhsgFOboCxseIPT =  6; 
float DMXHPIAiuEFLPNMnGfSZ = -6.722409E+31F; 
string uXsgJdnDjLIODRsLIxuzKQI = "tujNiSEHLjlRpsBppcB"; 

}public void fYLO(){
ulong sICDcEp = 53670376958443242; 
string sFCu = "WdiIoEAuJw"; 
string BVf = "gJBDFkx"; 
ulong jWeVsjlNcMyuwnMmtGzB = 12145543665173341; 
float LHtafIwzqlOmyiINMuPol = 1.334161E+35F; 
short sKiagmlVta = 20827; 
string heSUomMftcmhB = "lDzfJdMNXlykYDseh"; 
float qlLyqpMEhZHWTHLatVVhYJ = -5.207609E-30F; 
int enjOoUwwipBegQ = 504702820; 
if(enjOoUwwipBegQ == 324767){
enjOoUwwipBegQ = enjOoUwwipBegQ + 295151;
}int nDYimKOIRFQVlytuKM = 464970; 
while(nDYimKOIRFQVlytuKM == 464970){
nDYimKOIRFQVlytuKM = 269731;
}ulong kzeHP = 3781144233184926; 
long BzWokFAgjdFOfgScLuhA = 30785417149524343; 
ulong FeQQWXDZECLqVDkEMu = 32618244712403705; 
byte PiTcwFA =  127; 
double jVBMRHPYJJE = 5.981139E-15; 
if(jVBMRHPYJJE == -6.334165E-28){
jVBMRHPYJJE = Math.Ceiling(Math.Atan(-5));
Console.ReadKey();
}sbyte dhZMh =  -111; 
long QpDfVJ = 57282340073358745; 
float TpsVZLEYwJiXGik = 2.20282F; 
sbyte kTUkthgddoi =  -112; 
sbyte CSSMPgeNEKm =  -2; 
byte HcQzVIGFZeZNSIBKlGcIdqscLY =  53; 
uint jYTVnxazKh = 689148; 
double NMbnymUzHyGPpKONWxhoQggSFk = -5.487287E-24; 
while(NMbnymUzHyGPpKONWxhoQggSFk == -407258.4){
NMbnymUzHyGPpKONWxhoQggSFk = Math.Ceiling(Math.Asin(0.2));
for( ; ;) {
Console.WriteLine(-1.562034E+10);
}
}ulong EHQoln = 23380299583081570; 
int hFiqXplixgMElgyioI = 91; 
while(hFiqXplixgMElgyioI == 91){
hFiqXplixgMElgyioI = hFiqXplixgMElgyioI + 234383;
}long iQMzgPTxu = 70373212619323888; 
sbyte oNlhFJjPLnsIWGLgx =  -15; 
double XLP = 3983986; 
while(XLP == 0.0001627146){
XLP = Math.Ceiling(Math.Tanh(0.1));
try{
int FwQljkjPp = 7535032;
if(FwQljkjPp == 55708){
FwQljkjPp++;
}else{
FwQljkjPp--;
Console.Write(FwQljkjPp.ToString());
}
}catch(Exception ex){

}
}uint NfSD = 75819; 
sbyte ifYEHuuJoWiTMZIbqQjZSPhHftcfq =  40; 
ulong hjVREccYGpJQuGpOzQkHTh = 72932873797582478; 
short jyMDeRyoNluCfqmBdoI = 1389; 
uint RacBT = 5935; 
ulong qZcHWTNKIsVUINGoHeW = 32474734051347265; 
uint XkKYFcCcLpHpGaiDlj = 4435; 

}public void mhIFEMiUD(){
short mfKgkVd = 13330; 
string WPoTyFIGS = "zcqLdbtaqthhZA"; 
string qbqpgDtZqssLggCn = "UKjPZciDctLeOQQQXnHoD"; 
short KstMKIUjRTyePTLqYhOeiJUDCDVi = 23669; 
double hFNBxGqAUhjUkpMbWmWIhPlObyQjg = 3.576885E+16; 
if(hFNBxGqAUhjUkpMbWmWIhPlObyQjg == 2.119263E+30){
hFNBxGqAUhjUkpMbWmWIhPlObyQjg = Math.Pow(2, 2.1);
try{
int tZAYPhshxSYpal = 8163972;
if(tZAYPhshxSYpal == 36466){
tZAYPhshxSYpal++;
}else{
tZAYPhshxSYpal--;
}
}catch(Exception ex){

}
}byte CwWLlsydjmjlOMtHyp =  67; 
string WKJoC = "iPJxwqQe"; 
short KxKAcTHfLMDtTJBejzGMnxLhon = 1792; 
float VYIaONTEEddcEEPIQWZVpgqz = 1.687428E+07F; 
byte CkOqQqQOUseVICHwDKKSVB =  120; 
short VzOHjuiBhbsoMllOb = 16099; 
uint enPBWVXVwQbMxQUSbgLYWwABn = 81; 
ushort eTOaByIKnKnyneq = 493; 
int WstMUcAVJOolQeByqWmLVsmiOx = 521565; 
if(WstMUcAVJOolQeByqWmLVsmiOx == 149801){
WstMUcAVJOolQeByqWmLVsmiOx += 494331;
}long UfesYMCGP = 9942944960111776; 
ushort VXEekYzGYlsHPFEkwtYYCjp = 37404; 
ushort NDmFSCNMMppaTtBfDtxlGxq = 64311; 
uint QxlLHaKylVtuuRT = 86; 
byte uAGywIXhhhplncuMpAaJppxhGN =  41; 
double sEDdehYkZRdKSDoVKfq = -3.920869E-31; 
while(sEDdehYkZRdKSDoVKfq == 5.07497E+26){
sEDdehYkZRdKSDoVKfq = Math.Pow(2, 2.1);
object iVwCqJWZ;
iVwCqJWZ = 622772.9;
Console.WriteLine(iVwCqJWZ.ToString().ToLower());
}int oNLhCXxFElUWFgoWkWbJZd = 55; 
while(oNLhCXxFElUWFgoWkWbJZd == 55){
oNLhCXxFElUWFgoWkWbJZd = 831714;
}uint kBsmhbweuKuuWEgOVwOZDjg = 739207005; 
byte ieToGdQatEqYnBfDsnFgxxn =  192; 
byte Ckat =  105; 
uint omaCuPpgDNkExBAyIPIhepMQfcD = 583285; 
string iQc = "mebXAnaUJdjQDbOtFNtQZBjwqlOCD"; 
ulong NtCm = 77894735686897288; 
ulong maBQHwwylZMVHGmBzsVbxxaPwNOF = 42513238064871839; 
byte VqHphCByZkWsGdhLAQZMwT =  167; 
float BxnyhCanNxFugoAnYNDIigAoMYom = 3.073778E-16F; 
string QtHelZSKVeSIEptBByygf = "OtXZMil"; 
ulong yuVFNIucYVTMdnSLxGOMdTV = 86258743367337010; 
ushort qwmJLGT = 7889; 
ushort lmxKfenoDzbJEzq = 62989; 
sbyte lPLkwFNTFcLaV =  29; 

}public void wKqjVhcYYgYfJROqNyzUG(){
ulong RSX = 49222519541825305; 
sbyte aOmljQMdyzdbRMYDyMdIJBqX =  108; 
int wFwLJIZqZjlKPbWtuTjJAGZ = 67; 
if(wFwLJIZqZjlKPbWtuTjJAGZ == 109316){
wFwLJIZqZjlKPbWtuTjJAGZ = 500059;
}ushort pHcqAbNwuS = 26215; 
double kboGNgXeMUnP = 2.116724E-17; 
if(kboGNgXeMUnP == 1.300687E-29){
kboGNgXeMUnP = Math.Ceiling(Math.Sinh(-5));
try{
Console.WriteLine(kboGNgXeMUnP.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}short czgsNYIGNOjxPtkJ = 28639; 
ulong LdkyTVgJHqPGlCpBuuTxIby = 44958554294342646; 
ushort GIySamx = 40714; 
short nYh = 30258; 
float TRndsfJGXizjsuhu = -0.0001573458F; 
int SItgyQtbinhxRowIJuQAZRUSOl = 8967; 
if(SItgyQtbinhxRowIJuQAZRUSOl == 632154){
SItgyQtbinhxRowIJuQAZRUSOl = SItgyQtbinhxRowIJuQAZRUSOl + 694766;
}short Hky = -24090; 
int NIyKbcscYBMXIgUbO = 365108; 
while(NIyKbcscYBMXIgUbO == 365108){
NIyKbcscYBMXIgUbO = 173358;
}short yeRIkkDOnNsukRMUkWsi = 11429; 
string fGyVOwZo = "hbEXBwOoijtuDHE"; 
long qqDRgYuUMcNoOtozTlim = 21028065757449736; 
ulong YUjgdJNIbdidPsaaw = 87567192629586283; 
double tPhALdieTeIoGLlk = -6.216866E-34; 
tPhALdieTeIoGLlk = Math.Ceiling(Math.Atan(-5));
Console.ReadLine();ushort BGLXjahRghJ = 27503; 
ushort qdNghgpN = 22788; 
ulong NOCstaSeSPQ = 19323336092604253; 
uint hjcqwqkmbNmUHymGzNgQqBwBFq = 9440; 
uint ctoyGEAbYKIRRbDFPu = 990017295; 
short SLCHOCMdcnMyKUGiSJgxPyiAKtYa = 2220; 
float CnfCEVZQXoUIufusJYHDnTSiuDcXw = 8.288265E+16F; 
uint njiFsc = 8591; 
float hUQPuob = -4.964956E-36F; 
float MJxWgbTcutDs = -1.041631E-32F; 
double NpWtKuKL = -1.50076E+07; 
NpWtKuKL = Math.Ceiling(Math.Tanh(0.1));
try{

}catch(Exception ex){

}uint wfEWmjaSXPoFEuUufwjRyziw = 694503453; 
string UVxtFqP = "zcyDhDBWxlbLhm"; 
string aKqXXhbDiSYbtVa = "FAWcYHKUY"; 
byte IJpbISXsUZRypbUtoFzDBlPfJqaAu =  253; 
ushort LhtTTRpjwDfmNbGbdmuoAKAxUjspR = 30211; 
double DTNiIbAzoohXfCsHMZxgFhzbJXZ = 0.001408995; 
if(DTNiIbAzoohXfCsHMZxgFhzbJXZ == -6.261126E+33){
DTNiIbAzoohXfCsHMZxgFhzbJXZ = Math.Ceiling(Math.Tanh(0.1));
Console.WriteLine(DTNiIbAzoohXfCsHMZxgFhzbJXZ.ToString());
}
}public void pmIzInfltcwZ(){
byte yoGaDXQqVswNuahWmgxuemmnma =  97; 
short IGYJTQcFuXyDZqodVT = -25871; 
string QnDTOjbqODEAOnN = "dCjeqTiZRfVXks"; 
sbyte lfSqwQeHam =  30; 
ushort dkdDHRsVZfXfEbIoegwwomIxwb = 17944; 
ushort eelOfOVSwFCQtmZVlR = 50236; 
ushort WgkaEREIgFDV = 21985; 
float Lqz = -8.338658E+18F; 
ushort DUFWaICjkBZlhXbOCWsjn = 64871; 
double bsYjUtAKVmFqwIRVgXucVfNdJEX = -3.828022E-08; 
if(bsYjUtAKVmFqwIRVgXucVfNdJEX == -5999.072){
bsYjUtAKVmFqwIRVgXucVfNdJEX = Math.Truncate(bsYjUtAKVmFqwIRVgXucVfNdJEX);
try{

}catch(Exception ex){

}
}double nJcUshNj = 4.658393E-32; 
while(nJcUshNj == 3.016582E-15){
nJcUshNj = Math.Pow(2, 2.1);
try{
int VftRpGlTcWKionU = 8069932;
if(VftRpGlTcWKionU == 82541){
VftRpGlTcWKionU++;
}else{
VftRpGlTcWKionU--;
Console.Write(VftRpGlTcWKionU.ToString());
}
}catch(Exception ex){

}
}ulong dlWXAdOUGyZGxZtIPqOsqIoh = 26642970584108765; 
uint SAPeANgI = 192587496; 
uint ViWXizcwzxWOtCYiQlwcelhUSLSak = 82372015; 
sbyte FcWJgkDUeMwDbLXPXSF =  69; 
uint JbbfzHbkPGRhOzbBJaThZBMqRZ = 216685361; 
byte lqgLtUHbMWSUXQMTXAOyjFVqU =  43; 
short nANRXXuBbdegUInXpfDWGX = -30064; 
ushort QHIxQRkFVNmjbX = 64673; 
ushort STuIDDpY = 1853; 
ulong teqyqtgqMekmUCYUbNn = 26479640077965908; 
byte syOcF =  57; 
float QSIQJKgHGAfFIo = -8.609131E+19F; 
int SLkn = 5823; 
if(SLkn == 387964){
SLkn += 624994;
}ushort pphykVGs = 59412; 
float ADCJJxcma = -630.1966F; 
ushort lCCdXx = 19647; 
uint wjQxjdo = 27; 
sbyte yLL =  75; 
float PqtxPOXb = -1.872699E+13F; 
ushort wzCMphqTmlZXThwqJ = 33337; 
float iNHYgNHxTwYuVGTJBLwEyb = 118361.8F; 
short EHsGsMSJtISDGE = 1835; 
double RnFcmnNRYfnngPdHLiZdndlDlmlRX = -2.416773E+26; 
if(RnFcmnNRYfnngPdHLiZdndlDlmlRX == 1.96584E-36){
double qtGPuHFwlSOWUu = Math.Log(1000, 10);
RnFcmnNRYfnngPdHLiZdndlDlmlRX = qtGPuHFwlSOWUu;
 bool? bcQZcWTSnlMeJOUqSHyzKGX = new bool?();
bcQZcWTSnlMeJOUqSHyzKGX = true;
}double ndweYZKTRHPMbO = -1.543718; 
if(ndweYZKTRHPMbO == 32.96141){
ndweYZKTRHPMbO = Math.Ceiling(Math.Sinh(-5));
try{
Console.WriteLine(ndweYZKTRHPMbO.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}
}
} 
public class aLfFITTkWdVO{public void LKWQeLSwFZNXLMnklZwzh(){
string jAoGupSdagXjSKsNYUXwMtHXhFZ = "eBoCKOMpPfyeNWMRNCxBU"; 
byte FjGjRTszPqhZBS =  109; 
uint YbXSy = 661640038; 
ulong PjGizQRdKqeEVyyIIOGW = 30507679399659330; 
int qXujdh = 618744723; 
if(qXujdh == 165811){
qXujdh = 429114;
}sbyte xBdEA =  -75; 
ulong cTxRT = 13919559608064153; 
sbyte LtoPbVm =  66; 
ulong lGRAoQtslUBNmonPXwIWGfoqs = 78558826766382955; 
string YZxILfjNlIYgaBmXSfSk = "qAheGX"; 
int ewfm = 30898668; 
if(ewfm == 758807){
ewfm = 929079;
}float scKtdjVAmXZPPC = 2.360868F; 
short eiFLhmSUtkzxejbQO = -15118; 
ulong bxcfPhwpyfJnmkRIRHVxBiR = 75845566891215163; 
ushort BBJgzVZRDymDjjUstZhtt = 16242; 
float qdYoGDzsoEMKhCsLVGY = 116974F; 
sbyte HYyuZGaqnIdPwXLlZZUmGyjkSI =  -18; 
uint nhbilsqiizNnlzceNXOkuZN = 90398688; 
byte OmCsjDE =  254; 
uint GiF = 563; 
ulong hZtZhffKJmtnwtnYi = 76085059182902883; 
uint gOiR = 1770; 
byte VUjHgxJCpkLfJqkx =  205; 
sbyte wFnLCGc =  120; 
double wcejSseYWPkybwyN = -8.703956E-10; 
wcejSseYWPkybwyN = Math.Pow(double.MinValue, double.MaxValue);
Console.Write(wcejSseYWPkybwyN.ToString());short pCDVnjiXHjmEtWIIe = -8410; 
double kZcCEXZudoBtLZaG = 4.189708E+12; 
while(kZcCEXZudoBtLZaG == -4.734661E-32){
kZcCEXZudoBtLZaG = Math.Exp(2);
try{
int WcJxRJuZOjXPEZ = 9636688;
if(WcJxRJuZOjXPEZ == 33708){
WcJxRJuZOjXPEZ++;
}else{
WcJxRJuZOjXPEZ--;
}
}catch(Exception ex){

}
}byte psUi =  177; 
double ckD = 1.227749E-33; 
ckD = Math.Pow(2, 2.1);
try{
int XXqcDiBtiTUPBSKsAf = 667841;
if(XXqcDiBtiTUPBSKsAf == 3635){
XXqcDiBtiTUPBSKsAf++;
}else{
XXqcDiBtiTUPBSKsAf--;
}
}catch(Exception ex){

}ulong cNtouVBSmf = 68323880204041188; 
int fqFbJ = 58; 
if(fqFbJ == 962294){
fqFbJ = 757869;
}byte Byw =  242; 
ushort qoRWhbjdz = 52589; 
byte DwMiMhsHlqEnosuyNGEl =  77; 
byte PowyZzefgt =  77; 

}public void SpqLnFNb(){
string EHPIMqTlZsogWUpLthgkyV = "hChwUpIDlodMOJsNNmVtXWMtywDlG"; 
float oIIHfFWZVsQcSThCcawuYCCb = -6.66463E+23F; 
ulong zNwWsAPdgbmhRmaahTTBoTBkkBRN = 15584035740110542; 
double LteewaOmDLCBUsWmIFljMRIbXB = 2.276392E+33; 
if(LteewaOmDLCBUsWmIFljMRIbXB == 2.60395E+19){
double BwkzAyXYf = 4.282288E-33;
LteewaOmDLCBUsWmIFljMRIbXB = Math.Round(BwkzAyXYf ,1,MidpointRounding.ToEven);
 bool? tEqVVN = new bool?();
tEqVVN = true;
}long HjwSxgNNPTDQqP = 81852011137765990; 
short sdEeGanhDifUXMUPiZsahmQgT = -16170; 
byte fNfAUFA =  91; 
int gPGuniJJHaIdOqOoJeny = 3580; 
if(gPGuniJJHaIdOqOoJeny == 38839){
gPGuniJJHaIdOqOoJeny += 795585;
}string ZslClBSEuOZHyjHjCcZ = "MGWuefxPZBszfdJnOfRmzjUU"; 
sbyte abMFEZQnTtwSxfddFUwC =  5; 
ushort sxwkNpCMcpyfwcuX = 24133; 
long jpTWJMiuGaAEfHBuihOdYsItD = 34329243455718721; 
long kzBMAEV = 20012430485036421; 
byte GFTSIAuKLAtwdxKxyoVJwnAd =  104; 
double QICOtYgyoQqDYfcOdCCIfbVb = 4.264258E+18; 
while(QICOtYgyoQqDYfcOdCCIfbVb == -2.596066E-14){
QICOtYgyoQqDYfcOdCCIfbVb = Math.Ceiling(Math.Cosh(5));
Console.Write(QICOtYgyoQqDYfcOdCCIfbVb.ToString());
}int hzGnPwMoALstoMNWNh = 52; 
while(hzGnPwMoALstoMNWNh == 52){
hzGnPwMoALstoMNWNh = 692265;
}double ZKWGoOV = -4.090626E+10; 
ZKWGoOV = Math.Ceiling(Math.Asin(0.2));
try{
int EyTeXTwDzdIINQx = 2355040;
if(EyTeXTwDzdIINQx == 96089){
EyTeXTwDzdIINQx++;
}else{
EyTeXTwDzdIINQx--;
Console.Write(EyTeXTwDzdIINQx.ToString());
}
}catch(Exception ex){

}long ceTOLV = 31730128418785474; 
long EoGuQnBqtBzT = 79492522388863913; 
sbyte zCSkAOCK =  122; 
long DMXsoqPFCujxB = 80566552743607442; 
byte ReDFPMQKCPAO =  98; 
float gxhzIQneqVHdUqBILt = -1.619259E-30F; 
byte MZSLLJIbBzRclbTjCEDakJi =  168; 
double ahPwaNquKhHX = -4.426599E+08; 
while(ahPwaNquKhHX == -2.263049E-08){
double zPCSQEm = -3.865959E-20;
ahPwaNquKhHX = zPCSQEm * 2;
try{
int QtIBmfzdL = 9609173;
if(QtIBmfzdL == 94329){
QtIBmfzdL++;
}else{
QtIBmfzdL--;
}
}catch(Exception ex){

}
}string KigmgdXmqHK = "OQOMffhZCmTHZKugyNYG"; 
float qlWaunppTEwkTBJwGSPdNxpBmyKRQ = 9.567574E+10F; 
string QzDPeLTKlupkPQUzbhgRNkVW = "OXFAAnVqwdbfGB"; 
sbyte uCbZoyHQsOyCmcFDGtqOokMfwTt =  62; 
float xHaVtKUXgppP = -5.34483E+16F; 
float etAzBljsIjPVE = -4.15199E-33F; 
float asQBsKVJImayAdaoaPantRjyImiU = 1.11476E-10F; 
long xRgtaTNJF = 73102229570102064; 
int HdHFiitTeBaKoCPbJX = 26639010; 
while(HdHFiitTeBaKoCPbJX == 26639010){
HdHFiitTeBaKoCPbJX += 676872;
}int jnNfcfsQehQlYjIaIhEgEzuTzigtw = 739331; 
if(jnNfcfsQehQlYjIaIhEgEzuTzigtw == 178105){
jnNfcfsQehQlYjIaIhEgEzuTzigtw += 973839;
}
}public void fYzVq(){
uint HlxYyHhuifxxMUT = 86421793; 
double uBwSjPlCUJKIPeaNIbkKMnjERGNmk = -9.542792E-13; 
while(uBwSjPlCUJKIPeaNIbkKMnjERGNmk == 6.461021E-12){
double mjlemLAyHqLhlF = -2.136687E+33;
uBwSjPlCUJKIPeaNIbkKMnjERGNmk = Math.Round(mjlemLAyHqLhlF);
Console.WriteLine(uBwSjPlCUJKIPeaNIbkKMnjERGNmk.ToString());
}long YGtsK = 50655265928311459; 
short HXQIRGeTPHFGymIFgcqb = -1645; 
sbyte fSkTxeIIPdUydXoRATNFMfDPCXQp =  48; 
float izaKQGMJOgZ = 147.5901F; 
int EHRkRhMDkaoxszosXWOMYdmA = 7675; 
while(EHRkRhMDkaoxszosXWOMYdmA == 7675){
EHRkRhMDkaoxszosXWOMYdmA = EHRkRhMDkaoxszosXWOMYdmA + 738035;
}short kMu = -21908; 
uint bixWPKBiukAPbSO = 823719816; 
uint qZdXOfzKnkGdf = 477743730; 
double ocnDYctZzILzmtVTOgA = 3.819197E-22; 
ocnDYctZzILzmtVTOgA = Math.Sqrt(4);
Console.ReadKey();int eejLGLWaEEuXxLfmTuOBZBXcC = 551120; 
while(eejLGLWaEEuXxLfmTuOBZBXcC == 551120){
eejLGLWaEEuXxLfmTuOBZBXcC = eejLGLWaEEuXxLfmTuOBZBXcC + 329051;
}long MijjIXd = 10096846540175019; 
long JVCfREghjahIfXmNPKCWd = 61111327120835155; 
short oGmWwfbVFcczWWJ = 31666; 
int UyyMN = 44836545; 
while(UyyMN == 44836545){
UyyMN = UyyMN + 926405;
}int MUETbTbRHNPXUaNBN = 923308; 
while(MUETbTbRHNPXUaNBN == 923308){
MUETbTbRHNPXUaNBN = 843977;
}double SHAXnfYaBpEscHgCn = -2.597289E+14; 
SHAXnfYaBpEscHgCn = Math.Sqrt(4);
Console.WriteLine(SHAXnfYaBpEscHgCn.ToString());byte jjNUiVReWHHtXzVUR =  83; 
sbyte XBhnjPmOGHfHBkm =  53; 
byte NgbNDmUxxaosZIPP =  149; 
long ALt = 82006311575044274; 
byte HqFYxkXU =  95; 
ulong lzmIySMHDKPO = 25077411864364777; 
long fMIdQtuMgfXayFkIdhggJBSE = 56173340220067568; 
long tCZtctlREJQsat = 71401372276818994; 
float SoUyb = 3.969415E+09F; 
ulong blKJLfOURWiN = 69378312807385312; 
sbyte OIUCNzOPGc =  -114; 
string htXXHIJpJLsYPOFYyTZlNycOitpfc = "gKTCYBfDffyZ"; 
sbyte DDXLjRhicOlKpjOnsDmqFIfnQdSV =  68; 
float NsfkHwFWljXHeoojWOROUMOs = 0.0001908417F; 
long fYlujZAqkYhTNHbgHbaZBUVgWfU = 13130153741465111; 
byte DyfQXZXGYamLIMTyKuldfzdfyNyI =  250; 
sbyte ROid =  19; 

}public void jDUXBYeHpWbzMbpEjSeuZskCpHS(){
double XzIoCBRqfFAYzh = 1.226365E-06; 
if(XzIoCBRqfFAYzh == -1.925178E-31){
XzIoCBRqfFAYzh = Math.Ceiling(Math.Atan(-5));
Console.WriteLine(XzIoCBRqfFAYzh.ToString());
}long mCAuWcapLeESbnHClFmwbdgphYgSc = 12068027530983517; 
double lPcqVAMlOUDikgmXjwjlXVhmxPq = 21.12719; 
while(lPcqVAMlOUDikgmXjwjlXVhmxPq == 6.855074E-16){
lPcqVAMlOUDikgmXjwjlXVhmxPq = Math.Pow(double.MinValue, double.MaxValue);
try{
int hVLkjmW = 2658672;
if(hVLkjmW == 47943){
hVLkjmW++;
}else{
hVLkjmW--;
Console.Write(hVLkjmW.ToString());
}
}catch(Exception ex){

}
}long ZjQfJBbhOmjEYgFdsJXMP = 32438769194224550; 
string yQxHOIdLojtxAfgnWtXQFCZVEhkLN = "nYNnNODAQwnYIDwaCAoIV"; 
ushort kuVQBdHyt = 62547; 
int wJktsLxDCDGUyAEGLxfOTMCURXb = 189698; 
if(wJktsLxDCDGUyAEGLxfOTMCURXb == 23221){
wJktsLxDCDGUyAEGLxfOTMCURXb = wJktsLxDCDGUyAEGLxfOTMCURXb + 27914;
}int qpg = 927285701; 
while(qpg == 927285701){
qpg = qpg + 669139;
}ulong tbB = 14097980378292181; 
short uJasEHFSRzloFAioa = -31739; 
float PFaWj = -2.158589E+34F; 
sbyte EqjZFCm =  -11; 
uint bjiDDn = 14929465; 
uint DbJnlNQJdiqKwKmhcpZYW = 526513443; 
float VGbyJzeAAGCIVNSMEupRsPAow = -8.509937E-09F; 
sbyte NLBQyqgOkwKljVdM =  1; 
sbyte hmsykcAgfNcydXaNUb =  87; 
uint LXsYGFhTxEUdSZ = 3998; 
byte NXkabiXHoii =  42; 
long KgxoZzFyqmKihulXOsHM = 25675003334709332; 
short KnTluLDXwPLpGXpTaEuswRCZEiVMt = 19691; 
uint htEwoWZCuRWRaPUnud = 44; 
int ETMUJkPREGqghmuVEeFSXFUKYPoI = 3131; 
if(ETMUJkPREGqghmuVEeFSXFUKYPoI == 974367){
ETMUJkPREGqghmuVEeFSXFUKYPoI += 187746;
}ulong KunPMDmqBUHHTAAeTOaYY = 86952344756042698; 
short YzKkiR = 19293; 
sbyte qDjehIOSQBhkj =  80; 
ulong ztTFdZsiNJijnqndIGIw = 43923152001319659; 
sbyte PIXEKyWBqqkasAAEEAmeDEipPHIG =  -69; 
ushort TaQTCmKHRggnpjRAmO = 51400; 
string letkzHPcPqoWOsaDGXqTSwXxpTfX = "wRyqGI"; 
double JcGVFbQSsCSRXH = 7.908431E-05; 
if(JcGVFbQSsCSRXH == -1.980396E+24){
JcGVFbQSsCSRXH = Math.Truncate(JcGVFbQSsCSRXH);
Console.Write(JcGVFbQSsCSRXH.ToString());
}ushort nmLLGBRzIUqHiyITFMRDkYgusedo = 56646; 
long NeRiRuYbPknMm = 37446951336279682; 
sbyte MpfptCyCGWAEDsnRAHLbsWlTY =  -90; 
int lAzKEmeSLdGAsJIPNS = 17424638; 
while(lAzKEmeSLdGAsJIPNS == 17424638){
lAzKEmeSLdGAsJIPNS += 609503;
}
}public void hAPelqkYSogzQZomwfwbkKJoyggAu(){
ushort FSK = 28462; 
ulong OzMuCBtCqW = 4760454551250002; 
uint BIfkLEzoDGgDDXIHJHJjpOgjn = 47; 
int enVYWOgtCJYbeYLQpQYhzQXfnC = 52496497; 
if(enVYWOgtCJYbeYLQpQYhzQXfnC == 574425){
enVYWOgtCJYbeYLQpQYhzQXfnC = enVYWOgtCJYbeYLQpQYhzQXfnC + 96914;
}float yqlEGjBnhYLhk = 1.372341E-07F; 
ushort qBxwopPLwbGxEeThKCtFMkPHjIZu = 7834; 
short fDZmFyZibxTjhVSETRaCncPlgMQkH = -11468; 
ushort fcAUFSoKKssiSaHLsd = 8230; 
int OKdNOf = 649455856; 
if(OKdNOf == 697406){
OKdNOf = 18684;
}short fxXZa = 3818; 
short VKHtgfeSDjSzqswgRMwcVQIhE = 26998; 
uint uefZQXPugu = 762495438; 
int ZAiQONaRingnVfczPkEQzzeSJuE = 1; 
while(ZAiQONaRingnVfczPkEQzzeSJuE == 1){
ZAiQONaRingnVfczPkEQzzeSJuE = ZAiQONaRingnVfczPkEQzzeSJuE + 482372;
}double hYOhOnEVRSCbKsIFOjOzCwqhu = -2.753181E+37; 
hYOhOnEVRSCbKsIFOjOzCwqhu = Math.Truncate(hYOhOnEVRSCbKsIFOjOzCwqhu);
 bool? RYzMCoNIGOGnN = new bool?();
RYzMCoNIGOGnN = true;int hqacoozWEadtZhSwpjmguXGukzlkn = 829; 
while(hqacoozWEadtZhSwpjmguXGukzlkn == 829){
hqacoozWEadtZhSwpjmguXGukzlkn = hqacoozWEadtZhSwpjmguXGukzlkn + 586953;
}short MpYomMtKNxpohwjYyFk = 14531; 
int QcmIqWEwJDCQEUoRhlmKzcWVf = 893387695; 
if(QcmIqWEwJDCQEUoRhlmKzcWVf == 730550){
QcmIqWEwJDCQEUoRhlmKzcWVf = 386713;
}int VAVbFwxgsNNNsRpKGxYzWsNgqDAbb = 937571591; 
while(VAVbFwxgsNNNsRpKGxYzWsNgqDAbb == 937571591){
VAVbFwxgsNNNsRpKGxYzWsNgqDAbb = VAVbFwxgsNNNsRpKGxYzWsNgqDAbb + 676802;
}ushort QERQEgpjiOZF = 55265; 
double wgcDNPLbVu = -0.0004064854; 
wgcDNPLbVu = Math.Truncate(wgcDNPLbVu);
object OgIEfCVwAJAGsmRKBfxnzKpoS;
OgIEfCVwAJAGsmRKBfxnzKpoS = 1.289171E+11;
Console.WriteLine(OgIEfCVwAJAGsmRKBfxnzKpoS.ToString().ToLower());ulong JOJSlAseQsLEnAzFj = 74680592428164162; 
string lPN = "CMMNjkHXpHJk"; 
double gdOO = -3.919052E-10; 
while(gdOO == 1.448449E-06){
gdOO = Math.Ceiling(Math.Acos(0));
try{
Console.WriteLine(gdOO.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}ushort FAHZecIGQFyEKNLjdQAKuyNQmsiKa = 48237; 
byte UkYqkyZ =  75; 
double xTab = -4.762298E-21; 
double gNQNNTYGdwnKAPuZHRj = Math.Log(1000, 10);
xTab = gNQNNTYGdwnKAPuZHRj;
string CTYQGWF = "QVKydTDAGWFxnjwMMthKzXQ"; 
sbyte FQOADxpteipwwX =  46; 
double wKHEUszldlPUoTf = 1.920614E+37; 
while(wKHEUszldlPUoTf == -2.165401E+21){
wKHEUszldlPUoTf = Math.Ceiling(Math.Asin(0.2));
 bool? pmAkBAPojTwzIkxs = new bool?();
pmAkBAPojTwzIkxs = true;
}string gPEz = "EoOEuMVyVYQAFjUhsZhWqNw"; 
sbyte BYiQoJdNJPV =  21; 
double FzzQRCLJhfV = 1.642838E+23; 
while(FzzQRCLJhfV == 7.288966E-07){
FzzQRCLJhfV = Math.Ceiling(Math.Atan(-5));
int? ySWFIGSRdUBTIjmYcZ = 336850;
ySWFIGSRdUBTIjmYcZ += 92765;
}long gweCJxWEOlLKaObSHdeWDhM = 16928472348421075; 
double ecgLx = -7.128101E-16; 
if(ecgLx == 4537439){
ecgLx = Math.Ceiling(Math.Cos(2));
try{
int UgkZPLPERGLfYBXXydsVdy = 460397;
if(UgkZPLPERGLfYBXXydsVdy == 2252){
UgkZPLPERGLfYBXXydsVdy++;
}else{
UgkZPLPERGLfYBXXydsVdy--;
Console.Write(UgkZPLPERGLfYBXXydsVdy.ToString());
}
}catch(Exception ex){

}
}ushort SKMUuIDQuMnbH = 28571; 

}
} public class lTSJCNOmVzEhagzbqBJGMm{public void DwKaKIBF(){
ulong lFeksTIexRwRADJVKXCYRaZJomR = 5342307362557166; 
int RYHbCoxQMWhqO = 564979291; 
while(RYHbCoxQMWhqO == 564979291){
RYHbCoxQMWhqO = RYHbCoxQMWhqO + 209629;
}int VfbUaQwCbmOgCT = 889440224; 
if(VfbUaQwCbmOgCT == 720863){
VfbUaQwCbmOgCT += 561811;
}float qKwpIRQZgEXXVTGgC = -6.444432E-40F; 
byte JtkleFjTPAcEtOqAKiFMbf =  52; 
sbyte XfoPOmDFSIqjdEmHgq =  -108; 
float DenIRpxDJDibI = -0.00141312F; 
float bmAYC = -7.245593E-38F; 
short FqfVOgojwbahXWgcifi = -22249; 
ushort PMcpSwwaYmskLe = 29122; 
ulong bNyMxUWepsWsaSgjiCuTzoT = 81252285686734066; 
int qgwYKtRglCdAJ = 407335455; 
while(qgwYKtRglCdAJ == 407335455){
qgwYKtRglCdAJ += 566077;
}string pNUJqugftdpqXKMMlziDNlNQpMe = "GKKzCBatCotY"; 
long TzN = 84329819637064349; 
uint eadsGoO = 663059; 
ushort PyKskWfjSMtnchaQJfVGdbsWs = 29224; 
double KWAkdWsnFqnTYkJxLMbkGzf = -1.532871E+19; 
KWAkdWsnFqnTYkJxLMbkGzf = Math.Pow(double.MinValue, double.MaxValue);
long AsnYd = 86672329329242808; 
sbyte wxJbSGZiqSMlDeLRyUp =  90; 
float pbGYNHcDffhDHNVLPzBiWgqpjeFq = 7.986245E-10F; 
int LyfIwUTsgPN = 9395917; 
while(LyfIwUTsgPN == 9395917){
LyfIwUTsgPN = 918898;
}ulong RsNaj = 3240028943110789; 
uint RiHnoVGhchuLkjHGwRoPYYxWX = 10; 
byte uPbgninqIx =  52; 
byte LTQZfGby =  81; 
sbyte KTTMLqjQwTIssF =  111; 
string nOudFOnEfXaA = "RdGGxwp"; 
uint pluQqcBzuiFKiDOsfeF = 60; 
ushort qlbENMhC = 46350; 
ulong yhKOykcIDSUAJBRnqpLt = 48922882137596573; 
long HUTBfHJqI = 40034000038307541; 
string zQlZyIBfGkhMnWpkaGiwkFaIqlc = "KIRWxS"; 
byte wUxSFBcgBITbBg =  105; 
sbyte ILTQEIkF =  -32; 
ushort tkpwJTQfjpQLDThKtbHElKMPhtHRl = 14045; 

}public void keeNoqysIq(){
short URaqWxYRDNSMKACmplZUYbGGyfSt = 27962; 
long mWNSXhbKuntLymsdQPjcwqwYSluUQ = 28510169406734428; 
long fnCFGFqhseccEMHEVC = 51143939321922290; 
uint KIDgilePTMQtCdcPODbinkKni = 83628683; 
short IPWgBy = 13809; 
uint TqIRKiLwzILtfnIoEzXnYCuNJkX = 240607748; 
string qmSJKCxBCENjjHNDCnR = "ZOmXsXVhzlRiybufnFYOhYxLkskf"; 
double xGweHEZUqNGnboMFNhmH = -5.737751E+32; 
if(xGweHEZUqNGnboMFNhmH == -3.288358E+22){
xGweHEZUqNGnboMFNhmH = Math.Ceiling(Math.Sinh(-5));
Console.ReadKey();
}ushort OFJm = 61257; 
byte RLHAPkpyPARj =  110; 
ushort aUaWqUfPkDwqYRRIM = 57999; 
int tFcqj = 4816; 
while(tFcqj == 4816){
tFcqj = tFcqj + 758787;
}double mEOlaTyMVwXuUsGYSepTo = -5.293739E-06; 
double MPwHMncxMagszieWoaTietygwe = 2.284254E-07;
MPwHMncxMagszieWoaTietygwe = Math.Sqrt(3);
mEOlaTyMVwXuUsGYSepTo = MPwHMncxMagszieWoaTietygwe;
object UbNcKiYnGqVx;
UbNcKiYnGqVx = -1.877449E+33;long fUyfeG = 87053982044084913; 
long huA = 76903732146470637; 
short SKcYkPCWjJq = -14265; 
ulong QuEZDftiVMmmWfFiFIgidnZxq = 28445351727542749; 
short ytBwiYJPpMXTxsaeJhGNDugJeZok = -20800; 
int hFhgQd = 9592; 
while(hFhgQd == 9592){
hFhgQd = 691141;
}long FKMXIAYBGwclS = 19948876754502123; 
short kkayjwsdLx = -14138; 
uint EmCehIYMeE = 9016; 
ulong XTYeEpxBNHcOenssQ = 27029866597330823; 
sbyte CgIOCwTujNq =  53; 
byte cBVbuDLuMNJdUyMQpIwZUueTBs =  59; 
float GzSIbcL = -1.835063E+08F; 
string FhuUfzCxJtxKpjYIwcC = "pSkg"; 
short wTubJLFPwNakObRQjQiPCzcuo = -25089; 
string pMW = "GndIbILMe"; 
uint kIkoAkAJwYAoWQaMDHEzJRNF = 956613; 
byte DeyKaKSMdRFQQdBogZhRaVie =  132; 
string uUHPD = "Lfq"; 
uint nlYnJLcOIFpyzBQsOHWsiYFy = 450413492; 
string PfXj = "qLVTNmycNuBdyioMKWAuiHp"; 
long xkJaZDHgKhQVzbuzp = 891346668600994; 

}public void izPGK(){
ushort sqbPcOSTX = 49793; 
string RkVeVTkMFhbWVKUu = "qdywIgL"; 
short mIMjxHnnAUmuTMAIJkofU = -9254; 
float LGKEdidkNlGDauL = -1.164861E+11F; 
int ZgOyqXNZ = 412783214; 
while(ZgOyqXNZ == 412783214){
ZgOyqXNZ = ZgOyqXNZ + 771939;
}byte RXKkqRbIIgWInAU =  118; 
byte UqXhsGq =  47; 
sbyte ByMPMWUyXxTiegQkbfLxBOBM =  94; 
double KGdTgJlgkwhVsGyBS = -9.548665E+17; 
if(KGdTgJlgkwhVsGyBS == 1.391826E-13){
double LoMWZVPukRUDMATa = -2.477439E+07;
KGdTgJlgkwhVsGyBS = Math.Round(LoMWZVPukRUDMATa);
try{
int tRdnJxgLM = 6232257;
if(tRdnJxgLM == 62623){
tRdnJxgLM++;
}else{
tRdnJxgLM--;
}
}catch(Exception ex){

}
}short mtQMpMbfTlzmDOeJSs = -6339; 
float CVGqu = 7.48727E-34F; 
ushort yHtUUqVHhNGCeM = 60147; 
string zbnDnlwzMyjuWfYLxPSUJUNNlj = "nDxjKpChcgViwXwiTLll"; 
long RIXdgjFnBpdEhWonBduY = 59954373018363928; 
float ZXaG = 4.93276E+20F; 
byte jsDPHFeSQzTGkhSKNng =  85; 
long VTuzqfD = 35404200365602015; 
sbyte WBFhQSnRRfqTyIVYaxKwhnYoolsF =  -50; 
byte nGqjRcbfbfIAalIPTfWUGMMPJ =  136; 
short EAJCEocngyeVYpIoBOYcHyAAtt = -18642; 
int CBoNiDCyfVMiuJxbaWodxoUEmlf = 18; 
while(CBoNiDCyfVMiuJxbaWodxoUEmlf == 18){
CBoNiDCyfVMiuJxbaWodxoUEmlf += 512563;
}ushort WQiPOfRuSmUmPdztyWz = 30085; 
byte MBXyCnYMIIadAlShCXF =  229; 
string sYpWLONhbVt = "dJRqFExgFll"; 
ulong mCpwCTqEwhYgFDjYBG = 53685391013642231; 
string VMfNFtzmyMBcbMJ = "SjZblieGgbuesZkh"; 
float JJblDldWBOfdJyzJyaualMHAGD = 4.409167E-10F; 
double CskidZAGHNXpIpB = -6.33288E-12; 
double HWbCHZnKbcmNSBuzjStHcWeqZ = 4.787128E-25;
CskidZAGHNXpIpB = HWbCHZnKbcmNSBuzjStHcWeqZ / 3;
short AFMlByGSnmwW = 23285; 
byte AzhFOUoInSe =  250; 
double typ = 7.214032E-06; 
double fZmRVTUAiZgIEzkWztwzHRVQej = Math.IEEERemainder(3, 4);
typ = fZmRVTUAiZgIEzkWztwzHRVQej;
int[] EMSVPouyWleATtp = { 6586481 , 68384 } ;
Random iYCxGCankoeyMMytDMxwP= new Random();
Console.WriteLine(EMSVPouyWleATtp[iYCxGCankoeyMMytDMxwP.Next(0,2)]);double ayjthXCAybyZBRcLF = 0.8061368; 
if(ayjthXCAybyZBRcLF == 2.550059E+17){
double hXVSBoGVFdRUdqDYoLaubVtKkXjK = Math.IEEERemainder(3, 4);
ayjthXCAybyZBRcLF = hXVSBoGVFdRUdqDYoLaubVtKkXjK;
Console.WriteLine(ayjthXCAybyZBRcLF.ToString());
}int WpoRw = 8725; 
if(WpoRw == 399361){
WpoRw = WpoRw + 699348;
}sbyte axJuPCqabGSNHqoEZqWYdyoXg =  -7; 
uint HJENqWqAEjygleUKyePUFXZ = 75; 

}public void ODhMmpcE(){
ushort cfxKaPaQUQNY = 37807; 
int cKFiJJBZfiPsRWgd = 70299847; 
if(cKFiJJBZfiPsRWgd == 349283){
cKFiJJBZfiPsRWgd = 672981;
}string PXjiXzPYDmkHzuLKCgpClyiUON = "ndWgt"; 
int tMAqbItQPFLBGnZQgltDI = 7675; 
while(tMAqbItQPFLBGnZQgltDI == 7675){
tMAqbItQPFLBGnZQgltDI = 374381;
}uint HfRaOaZkjeIhwJFimgQnmPVaccb = 180608731; 
double nSViATARmaGWhLwlJighdDlIR = -1.18594E+25; 
double uoBBfoOpEnI = -1.443627E-36;
nSViATARmaGWhLwlJighdDlIR = Math.Round(uoBBfoOpEnI ,1,MidpointRounding.ToEven);
try{
Console.WriteLine(nSViATARmaGWhLwlJighdDlIR.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}byte ndjTYCejLku =  2; 
ulong mtTCjlINFiwJxzanFgEK = 80347835783132936; 
uint fzWbZPXMcYHBAazpeGjyGmxc = 326174170; 
int YuxmlQdWMwA = 3155; 
if(YuxmlQdWMwA == 610539){
YuxmlQdWMwA += 50345;
}byte NelLjleiTcUeJiH =  192; 
long uoJXCEKgXgNiRZNUS = 1294763596788567; 
float pQcJfwdIVOPDIacoXwLkUqPpwa = 3.369629E-15F; 
ushort bNILtp = 58474; 
ushort NNsSfdJeKiMjlAJYZbNn = 5967; 
ushort nONbVWHNuAOXyDAJmNWogIX = 51680; 
byte NunhjDpZdetbOhcxqLzDkxBsABhhH =  155; 
float OWjlzmpkIzzuplZPlLplpLOqUVIK = -6.673556E-28F; 
ushort nwmRXRXeumUNXfc = 16993; 
uint WVoFIdiTsDFH = 501253224; 
string gVZ = "RwOlScR"; 
string PTuhblziJOjIe = "uBOyuJXYgntCBhdN"; 
int iADKjzPnH = 458503551; 
while(iADKjzPnH == 458503551){
iADKjzPnH = iADKjzPnH + 733807;
}double LdGysxQMgPALq = -5.048933E+32; 
if(LdGysxQMgPALq == 0.8997221){
double pIfcfBZzTQFOs = Math.IEEERemainder(3, 4);
LdGysxQMgPALq = pIfcfBZzTQFOs;
object dNXdJoDoLCYSagZJuNXqppoNJ;
dNXdJoDoLCYSagZJuNXqppoNJ = 4.814691E+35;
Console.WriteLine(dNXdJoDoLCYSagZJuNXqppoNJ.ToString().ToLower());
}uint uMGxNYoN = 32196133; 
int UgfFOEVoLdeaCxPpkKiPLETVN = 98; 
while(UgfFOEVoLdeaCxPpkKiPLETVN == 98){
UgfFOEVoLdeaCxPpkKiPLETVN = UgfFOEVoLdeaCxPpkKiPLETVN + 901493;
}uint OkRRw = 94139363; 
float AqIGVpi = 7605.554F; 
string PgqnjS = "EzFxiDVNkXAPZlXCeCng"; 
byte nNMUupIUEHEctuFcifLie =  58; 
uint aVSFmMWMyVkSLnhFfqtSlWT = 943249; 
string ixbEbnYTzzcI = "LSGzjMsUXfdOSGCdcoIktOowq"; 
sbyte MuKgcJCguqwtkKzkpY =  77; 
sbyte xPxiuyzXDjgahGBnZaeq =  -7; 
sbyte HWyBUDZyCZWoFmYIlcjSgns =  -117; 

}public void YXqXL(){
float ocQmXXE = -24023.94F; 
ulong blpVbbsTnopGtEwwWFI = 66155534445945685; 
long AOnuIqQOdW = 17446549947362114; 
long CHQVmQUApsEoIFPBb = 59781235450478551; 
double DTbgadiNSYthSxqyLsaWmQtXjLwXU = 8.632833E-28; 
while(DTbgadiNSYthSxqyLsaWmQtXjLwXU == -1.344006E+25){
double ElzSWDJeUR = -7.332863E+08;
DTbgadiNSYthSxqyLsaWmQtXjLwXU = Math.Floor(ElzSWDJeUR);
Console.Write(DTbgadiNSYthSxqyLsaWmQtXjLwXU.ToString());
}short EnZSMTnTM = 4321; 
string CDBpUKQQAdCgzYEzUCFYi = "jjxadwSXSfLVpJRQK"; 
ushort AOAMStNpPZjZb = 33739; 
double pUTGwYClwxDJ = 2.63983E+10; 
if(pUTGwYClwxDJ == 5.455595E-28){
double zEIqecYJgJwqpBWRRaClnp = 2.414462E+12;
pUTGwYClwxDJ = zEIqecYJgJwqpBWRRaClnp / 3;

}short jEfHJGFDKH = 10698; 
long IJNS = 53481390499516000; 
long wcIbBmZ = 57773947231833913; 
float TKfBhzQSoYOoSyuY = -5.873589E-25F; 
uint FhAc = 7; 
uint FlWNbjPEFJmERjknloMODzdlnRW = 21211091; 
string eWUmccanJIQuPX = "kwWkRPqOtgiLzjndsBlhIXXhlka"; 
string MdnntG = "PZlHOiCfUsCdwEzTD"; 
ulong nyPZHPmIHc = 13153866911773088; 
int AwRnUKXJDnjMz = 34; 
while(AwRnUKXJDnjMz == 34){
AwRnUKXJDnjMz += 607708;
}float BVcaVwAJnfFXwPSH = 1.271382E-31F; 
double FesTiXamJosKOTYBOmYnzanhG = 4.300023E-34; 
if(FesTiXamJosKOTYBOmYnzanhG == -3.5482E-06){
FesTiXamJosKOTYBOmYnzanhG = Math.Ceiling(Math.Tanh(0.1));
Console.WriteLine(FesTiXamJosKOTYBOmYnzanhG.ToString());
}int XycMAOiqUxE = 3728; 
while(XycMAOiqUxE == 3728){
XycMAOiqUxE = XycMAOiqUxE + 822552;
}sbyte CRDPtiGHFKbfGWHHYY =  -95; 
long fcRiXlOqt = 27808321100735189; 
long ThFITqYqMIThKxsxP = 23268437092439009; 
float TTgkpoFSdCwYRZVbJBA = -4.106146E-36F; 
ushort LKbnGbStGaLezFxETfFn = 58750; 
ushort PxbQHLDtEzkxCnaZRlhPXgU = 28812; 
ushort XHtzfNqWl = 55301; 
int lJdHgGmi = 291299784; 
if(lJdHgGmi == 759881){
lJdHgGmi += 59288;
}ulong QJCFttOVZfsyeGbiwG = 5480925665188214; 
short OmLcaNgEpZSSVhHbnPdo = -16270; 
long hdmSAKmwebfgNuoysAzSxN = 43001060022502716; 
string eDDZNUITdENRP = "UIY"; 
short QORowXJoPMlaUYLcjRpKzR = 6565; 

}
} public class SaoPDQpccAmHmnhcdFShREST{public void YKQII(){
byte icxuIUMYwjYwMeQWKnGTZyYohxLFf =  126; 
ushort lnyjRNwjTUMHF = 28323; 
sbyte iOguctZMdhmkihTWQFjyHGfWTxafL =  -42; 
long oLwstasTnMfJBjaMaQRamJ = 59100145512973529; 
string yBVzMBDPmpiVAxwJwneVBK = "GsSKizMBuMtkZoD"; 
string mbOWBydlPSkeQelO = "ofTR"; 
ushort BQPifkwtTnF = 42673; 
uint DfUOytHBwFHhyXaxgJTLmfLnYbuz = 8189; 
byte MRBilfubKawkDXN =  116; 
ushort hWPOKxwGQxYhiF = 23883; 
sbyte HyRkbnRamLBMan =  110; 
sbyte JoIYhdOGpXcDSQStCgaDlgIXOyM =  103; 
float hbcmVdCyAtdFMpOzwUIsAhWD = 6029.552F; 
long jKsSLBQbBFVOSeHHzHuqj = 67122852177086129; 
uint txQmPblUkBNXQZUBuB = 731; 
string pCUIBFi = "PVabCqbR"; 
float mNjQxslStbVAPxBkgot = -11.76604F; 
int ZMRXPyDzEJLQpYAeDxJlhp = 1232; 
if(ZMRXPyDzEJLQpYAeDxJlhp == 328542){
ZMRXPyDzEJLQpYAeDxJlhp = 204507;
}ulong LdHckeGuyuQEqQe = 30966861613654526; 
ushort JwLungsMeXHjnNwAiZ = 18592; 
ushort ZAcYpeECIamlzDJUai = 17400; 
uint mSyxXAlMIMDZZKWJxYcOkxXuX = 20150451; 
ulong iQaWNNHFCsFqPaoKmA = 58882958729521068; 
ulong cAhoAgAVCcmXodUGzxwxJLKpqZs = 33238174233929843; 
string oRIBthjm = "yiKENmjtlqPSXBPVoNQfaEBRwiG"; 
ulong oEAdwCjUcbZWEAknsqqTGewugVU = 70465149426965864; 
ulong PDYFblLRix = 44977339284385840; 
byte fWBICbT =  116; 
short gEyYEPsnFSLDXARs = -12649; 
sbyte uDgMGjSbqdZtmBKpASFAdW =  28; 
ulong yDZbRiLOMXXZ = 78347064892815792; 
uint bniqAMGIzWwthHT = 49; 
float FBENpIbPPq = 4.639119E-08F; 
sbyte aCMSBKZhdzaFzJiRMCqpUXzBTJ =  -8; 
double qECmBhfhiZkoyxeNMjsGXFseS = 3.593558E-34; 
if(qECmBhfhiZkoyxeNMjsGXFseS == -1.254795E+37){
qECmBhfhiZkoyxeNMjsGXFseS = Math.Pow(2, 2.1);
object zfqTMlyw;
zfqTMlyw = 1.024906E-11;
Console.WriteLine(zfqTMlyw.ToString().ToLower());
}
}public void DJxSteLybKaWwxaN(){
ushort fNqRjBNmoPGKw = 32014; 
int CuB = 7926; 
if(CuB == 659412){
CuB = 655282;
}uint yRuCBJwSej = 46930282; 
double gPWSs = -28.83975; 
while(gPWSs == -2.364595E+30){
gPWSs = Math.Pow(double.MinValue, double.MaxValue);

}sbyte zYkWcYXSkVuiHUUCkILoiEMAnXZ =  -102; 
float kTEMpXUKR = 1551.6F; 
long dbk = 73445124393990010; 
ulong mMIeZRYYLBFVEWIBQRNCDIydOBR = 70462239436986742; 
sbyte MULJ =  -113; 
ushort HnUsRVTKfjtFnFqAa = 36690; 
sbyte iUUjkzze =  112; 
string AnWxAHzFVHIFQbgVywwKfWjsNu = "gsxPNeqUpqiRTCYkEBoehe"; 
int KDs = 56020933; 
while(KDs == 56020933){
KDs = 108843;
}double lASUBzhQEoED = -2.73126E-28; 
lASUBzhQEoED = Math.Ceiling(Math.Sinh(-5));
Console.WriteLine(lASUBzhQEoED.ToString());float JIDMPIZkD = 0.003581936F; 
ulong mVF = 85538174218318787; 
long WowkkUF = 69434250956590350; 
uint WsRODWag = 34; 
float JWsbGLLPsNF = -713.7693F; 
long OnIjlxPPRTwuswdfCKjTlz = 52855650871720616; 
long SjlSyiJTU = 8327438827566765; 
int ysRB = 7109; 
if(ysRB == 181064){
ysRB = 742136;
}double djiChilJtbpZeYwRwLp = 47440.07; 
djiChilJtbpZeYwRwLp = Math.Sqrt(4);
Console.WriteLine(djiChilJtbpZeYwRwLp.ToString());uint leRIDibDCyOGBeD = 4413152; 
ulong joGSbJaLHBeZOoNZYgbUUQSPS = 4485895264844641; 
ushort iGmQYDzubyy = 567; 
short eEAbVXtxRZambsSeORqYEuL = -12456; 
string wxQsuafkmkWo = "AXMPmQMBf"; 
uint JHJsjLTiYECWGzGZgQfHhAFRaRGz = 323041; 
float UTKOqObCsOp = 5.933207E+34F; 
string pIQFLa = "PicSqsfuJINlbNPRJtOEoujVTZM"; 
short KMfpZeBMAyKwXeJWMZtCRhlW = -5080; 
int ZhYqHRFYVHVbJ = 1048; 
while(ZhYqHRFYVHVbJ == 1048){
ZhYqHRFYVHVbJ = ZhYqHRFYVHVbJ + 159427;
}short CYhasNZDBj = 25131; 
ushort CFCfdFtgNpQXLnLYpSnof = 30617; 

}public void RqndFxw(){
ushort IYN = 34041; 
long bEXiPJqcIAnEjsHfFcwLOQCP = 22609145200397248; 
short QKqWKBAoGEpSlfwD = -7223; 
long Wblbi = 15034162724827202; 
byte CWOItVkAjXzSeNtMqSeu =  127; 
int hylKXCJGlFEFAOMpjMCb = 70; 
if(hylKXCJGlFEFAOMpjMCb == 56515){
hylKXCJGlFEFAOMpjMCb = 170325;
}string gZSiWnHkgkmYBqlnCiT = "GCUJYEExRTHPBeAmMuIlhCiYzEBN"; 
uint RUodZFejjtKj = 2686; 
int yxSYCgBIQdtWjwc = 953541163; 
while(yxSYCgBIQdtWjwc == 953541163){
yxSYCgBIQdtWjwc += 494038;
}ushort UiDko = 34940; 
double VuafdVB = 1.463974E+15; 
while(VuafdVB == 1.554506E-20){
VuafdVB = Math.Floor(5.56304E-21);
int? DwZIKnJEbfMXKROiCtZOHQAqYPlUy = 1705254;
DwZIKnJEbfMXKROiCtZOHQAqYPlUy += 63814;
}ulong hjmYOREkqRjUsMWJJVcPfVpEITum = 20396026929892555; 
string bup = "PYYwzLFEP"; 
byte aSmUJqVxqnnHRUJdIF =  169; 
float zTfqGluJJtnXOZRwuwKf = 7.93326E+37F; 
double sgCPHXYguWCZhDdfB = 2.883718E+11; 
sgCPHXYguWCZhDdfB = Math.Exp(2);
for( ; ;) {
Console.WriteLine(1.584986E-13);
}long MjLzwDLDTi = 59389966846265933; 
byte mkqlJUVRjFclMffky =  3; 
long pOCUMCMHmA = 83095964676479017; 
ulong HTtngMQjSDZMkWlz = 39026713828634014; 
long JFdGsZjkIywmOaLCPL = 57477533227015470; 
double ssoJDEckgljaCZnPdxJAw = 0.2770297; 
double PWaBTpiMLjYBwDCzG = Math.IEEERemainder(3, 4);
ssoJDEckgljaCZnPdxJAw = PWaBTpiMLjYBwDCzG;
ulong ZLFbuiCmhyNnNVYZQfozFhtAgJoXs = 66729134725796426; 
int dzNZ = 872328; 
while(dzNZ == 872328){
dzNZ = dzNZ + 558627;
}double ScmuSseUMUFUJD = 23.53038; 
ScmuSseUMUFUJD = Math.Pow(2, 2.1);
int JciiPkCMbgYSVxAA = 53447543; 
if(JciiPkCMbgYSVxAA == 266461){
JciiPkCMbgYSVxAA = JciiPkCMbgYSVxAA + 164341;
}byte NxtS =  77; 
string uKyBCbPdJJdQgFREcsCwJ = "NlBQfxsRJxaxEiyVXzYzUjksfzV"; 
float APFbAWkaLWt = 1.76131E+25F; 
string PLaJfgudyjaEbhLIBABi = "wwdmmVmCFMcjEDwymNje"; 
long RAsjSan = 30493711040180085; 
ushort lhCSzXuyGpjnMpeWnEFUcdlnMA = 48234; 
short ifCRTYXjotCXMmLeFB = -32446; 
ulong fMsC = 14651992833268799; 
float jJySshMYkcWpRC = -5.278667E-28F; 

}public void MGBdtjocQaWiupWTQBxhJIGqgCoFB(){
short eYaAxssRzhQCg = 19207; 
ulong MayaaNZpdajXSVKDzGSjuRAB = 52206988432975864; 
short DFcqusZKwwiwZEyndBXGxV = 1101; 
double qPZyXCHHgsfAhlBkK = -1.823465E-33; 
while(qPZyXCHHgsfAhlBkK == -9.90883E+14){
qPZyXCHHgsfAhlBkK = Math.Ceiling(Math.Atan(-5));

}ulong RhlQhJhoDi = 46265271017972172; 
short RbBowCgAkoBlQjPn = -20109; 
string UUIoDwUeMRHdxTpzQHXwGDJ = "nJuJ"; 
sbyte SuqPxFiMYTByuIFkdsB =  58; 
double mHhslFaTbkZfZmNHyLoe = 8.364135E-33; 
mHhslFaTbkZfZmNHyLoe = Math.Pow(2, 2.1);
try{
int xUYiNYkkeXoZbHyOfdZtsLsF = 7506316;
if(xUYiNYkkeXoZbHyOfdZtsLsF == 25751){
xUYiNYkkeXoZbHyOfdZtsLsF++;
}else{
xUYiNYkkeXoZbHyOfdZtsLsF--;
Console.Write(xUYiNYkkeXoZbHyOfdZtsLsF.ToString());
}
}catch(Exception ex){

}double JThAOPSoRRVluyCxMDuWhnsTXqhS = 5.885041E+14; 
JThAOPSoRRVluyCxMDuWhnsTXqhS = Math.Ceiling(Math.Sin(2));
for( ; ;) {
Console.WriteLine(-3.154428E+14);
}string VZVJhguHNVb = "XDFMLmnBWajRCpkNZHHObXla"; 
short QzhlHwwkiUJXPmsDwUQehRVHI = -24392; 
short PZqEwUyPt = 12795; 
ushort wphpebStxzDqklJZEBEAgix = 11882; 
int DVQDj = 49767686; 
while(DVQDj == 49767686){
DVQDj = 682893;
}int GoVRpyZtilzlSfUHm = 963376401; 
if(GoVRpyZtilzlSfUHm == 494783){
GoVRpyZtilzlSfUHm = GoVRpyZtilzlSfUHm + 942850;
}int oRRiERoXXYklsgMELtJsqsFFuw = 719517813; 
if(oRRiERoXXYklsgMELtJsqsFFuw == 117640){
oRRiERoXXYklsgMELtJsqsFFuw = 341246;
}short bGhjFwioUhKwVOlhofnlCWmlOhTFe = -6397; 
float VuegxBPnjcyxVZBjALnadexetV = -9.731054E-06F; 
uint jApQBBJfMOqZcAAQqP = 66200851; 
string gSPQXaNBeqMFS = "geAZCuYBJebWGDFHQTAQszbKEIsl"; 
long myICKloHgLwHiLObwkVzCSfbGIdPx = 27809277632513206; 
string sqiNPwMopWCmhpkzusHVbhBkmK = "PBTjNqqDgsfqGTtNTUHT"; 
ushort kxlIiRjRYBeNXPyMPVQN = 4858; 
sbyte aacJwHpGQlh =  -53; 
ushort eiBspuFlTWFpgfMdJEfglIfiPjJV = 18981; 
uint AaPLNpuOLLPhfdeBkUNDFjoM = 51212; 
short fMKXsgfpswYOnPuxxB = 32626; 
string KzbDXjoLcnKXRIZaSPYYPh = "VZnQQToWcmSxnJuuXQFohnQy"; 
sbyte wFLafVCJfEJuA =  87; 
ulong ILYlMsyubij = 89561949774877975; 
long bhXUFDRL = 69389181492350432; 
float dhqZXMKazuSLducfGDbsg = 3.144159E+08F; 
long RypSaesPNuDUcjk = 74882351857764904; 
uint aDyDDnCHBfotizoAWqaBl = 356356; 

}public void gCLhQHEeRhZCVlkfamDeWRxH(){
sbyte VWsZzyjHxaEuR =  94; 
uint DazOEKgV = 968220; 
string UofeaxLINS = "CsoQjDHxJDMoxhuQkPxAB"; 
ushort NgVzuuhTyLqWwnUWtSCsLhaJWGG = 32818; 
float sjfxDKodmcB = 9.942039E-18F; 
int lYDqT = 64; 
if(lYDqT == 211176){
lYDqT += 601435;
}sbyte DOOIbhthR =  125; 
ulong IoFARbTw = 57891412257434363; 
byte kXD =  15; 
uint nTIglXuYQfkPczlj = 71144090; 
long gKzbWIWPPPIFPfkYtMjGpJncO = 61386168862530365; 
long EuGjEbGjbyHa = 34693453223948332; 
uint wnAajZNEQieNIF = 54263453; 
int LzlGgyMzClkptiUhYs = 56274400; 
if(LzlGgyMzClkptiUhYs == 446186){
LzlGgyMzClkptiUhYs = 449291;
}long ViGIbyVyQZjQgcn = 70737941703703092; 
uint wRcNebPXJCDTTjNwWsVX = 21; 
long ghWehJYpgb = 18528134513207735; 
uint MSFqGeTFXCAkiOGW = 70; 
ulong EmM = 52269429161061081; 
short iEoEGzdQusFCwydePTKcyWz = -25353; 
ulong zBHYaYkzakptAlNkiUhqiJA = 64620541092000847; 
sbyte SZDixqGqWyTOwtyplNFYwFQbA =  -97; 
double YiOghC = 1.912386E-21; 
if(YiOghC == -3.390093E+19){
double kHtxKwWczB = 131.987;
kHtxKwWczB = Math.Sqrt(3);
YiOghC = kHtxKwWczB;
try{
Console.WriteLine(YiOghC.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}ushort Jsyi = 2568; 
int CUMDXjRSzAOaVGg = 601122; 
while(CUMDXjRSzAOaVGg == 601122){
CUMDXjRSzAOaVGg = CUMDXjRSzAOaVGg + 642238;
}ushort KOsjy = 54940; 
int uPUcAuhoCknRghfzxCQScmoRh = 3835528; 
if(uPUcAuhoCknRghfzxCQScmoRh == 1117){
uPUcAuhoCknRghfzxCQScmoRh = 378482;
}string IukaEIhyRnguDYCJUNVbLPhAdVL = "uKCRmWAOUySU"; 
long kYiyGRBfLmyZk = 70568737072303831; 
sbyte pgGzOPyVbupEiqHoUSnZntm =  57; 
int iAjXSsdlSpZsNpnqx = 899222; 
while(iAjXSsdlSpZsNpnqx == 899222){
iAjXSsdlSpZsNpnqx += 334014;
}ushort XPhz = 61615; 
float SlHXLdDYBWzppCfXMWWIRsyOiR = 1.00968E+30F; 
int NVxeABKpHscWOhVacBSRxCaldBO = 787868668; 
if(NVxeABKpHscWOhVacBSRxCaldBO == 654400){
NVxeABKpHscWOhVacBSRxCaldBO += 407280;
}int EUkcFnw = 5878; 
if(EUkcFnw == 573446){
EUkcFnw = EUkcFnw + 308385;
}
}
} public class hOucMzlVjbZMRCNiuCiYsgSzmD{public void lwH(){
sbyte zZIfPGKgXIKicTxTDxktwqBaVVQi =  -14; 
string ETdnSUHtPScbKxJx = "ghJZwQCgzHkUQ"; 
uint XhgpZFEZWcwBspRFlzwbp = 5019; 
sbyte Cgum =  -18; 
uint qteDptxPDhMXaoiQIcYwuztGIXmyy = 74; 
uint xiZlnnTcHAhGDhNtqM = 47; 
string SZzUsgYqsumtFgjEtPe = "NZlaeQAegyDbB"; 
ulong ZNuVMdogNsj = 60711881469977065; 
float OzQDIUjUSkSOQjQZTtpyTzIUlS = 1.546547E-09F; 
string IRXyko = "teiflVgsQRZWuDatwa"; 
short VqPYQQLEaRsZCwBngsHEBDxbW = -3295; 
long IyDyVyJbpkoxkEqZzaw = 76895834209999259; 
short OctffVDnVdeMzYWdksWkyDWcLDV = -19170; 
uint hgpg = 166157; 
double eIOLEWpnFOq = 2.026164E-25; 
while(eIOLEWpnFOq == -7.406177E-28){
eIOLEWpnFOq = Math.Pow(double.NegativeInfinity, 2);
try{
Console.WriteLine(eIOLEWpnFOq.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}int FTzoWhkmJgLCaMIaQmZtFMQqIGIIx = 70891590; 
if(FTzoWhkmJgLCaMIaQmZtFMQqIGIIx == 921551){
FTzoWhkmJgLCaMIaQmZtFMQqIGIIx += 959777;
}ulong HPShFFkSqnwyLVZfkuTdhHqBEQMc = 40215152546521978; 
sbyte ZgGzpsljL =  -121; 
string RCeLQazqEPTheSjIEJMDYZuMy = "qYnueKYXJDJiljZQIknyu"; 
uint FGcpCSR = 37; 
byte JAdjKii =  33; 
sbyte ZwyDEhqjyaoSRJOcuyeR =  -25; 
short dBANzCfnmoAGkFOYLBXVlTRJuwH = 18557; 
ulong smK = 60594974404990905; 
ulong Eqd = 23989296821693818; 
long KQtcUpCl = 75815425271093413; 
long DASmqpSjim = 35291780674641452; 
string PbkPHdhSlUFeIiQitxzckKhn = "VzGUctHnTABocHniZX"; 
byte CtHAIPWPZFeUWkLGDYiCX =  213; 
ulong oUqQkTlNUGcDRflmyRnow = 273429131093759; 
ulong LFXjNzfEgtsZqhiOxMVefE = 26681493651792350; 
byte iUsYUVAPqnhWdIbKMTknTIFgeKCk =  254; 
byte Uhu =  210; 
ushort kUCmgOSxEjaBPjKbWeCplTkt = 18087; 
ulong mnOKRG = 87192977279671423; 

}public void BABLkbqSkppgEsJhYsIGiP(){
uint VUJKtVqI = 512075; 
short SEEeWwEtBApipCcukgDMZq = 19785; 
short HApYIDdYYowO = 4312; 
long lgJxEBuSTXxxYHMCOnNpWmw = 43469511699284707; 
long QASIuMRlIxblfes = 25286415894851234; 
long AqqbHlqOZihUUuCVdnRuHQhSg = 67420769195180965; 
float wBkbkLygWksTmYWpDhqjKizVikk = 7.222962E+31F; 
byte OdAguTmGKiKkIOFOhVN =  109; 
ushort HkmboTaABkNFFPNRCyoDIaZX = 54702; 
sbyte ydbhNmYyoUsEuFjJzz =  -113; 
byte aptpgRhGkokhoExzdasGsyWi =  222; 
float ysCAOWOxNISGwQRdFhxfbOjwzLFy = 2.786124E-11F; 
byte lTLnJQxkBgKQku =  236; 
ulong tqbyPWKhgacsqTty = 24020810964671274; 
byte UdtjdPOWibiZxXafDP =  93; 
float nptPauK = 9.528432E+36F; 
int lkbGPWXnEwK = 2816; 
while(lkbGPWXnEwK == 2816){
lkbGPWXnEwK = lkbGPWXnEwK + 91442;
}uint ZstwGOawdueZjATNlVwpHSeoO = 972472; 
string dQGAgEXTPwSO = "TTAxASwJLJKhkgneH"; 
string WbZFAAdWWUxAEDeYlPsffJKt = "VqVxKP"; 
sbyte cRAUnaWxWzOni =  -97; 
int hcooWJ = 3472; 
if(hcooWJ == 564337){
hcooWJ = 164878;
}sbyte mfnfBLGeXPxJijVe =  116; 
ulong PnIATytEhnQyNnylJZdGUB = 35156027379961965; 
uint OxslGHwIghzQgsJu = 95971448; 
byte UjSoj =  139; 
double kTA = 1.93242E+18; 
if(kTA == 3.966799E-14){
kTA = Math.Ceiling(Math.Tan(1));
Console.Write(kTA.ToString());
}string JqzCqlAAKemXFQQsfuCpKeMTi = "ujYlfnigBSulOXWgUA"; 
ulong zSFeh = 68895456180061756; 
ulong oCzZYOzpXYTGEtDjmcdeDGTeR = 72115303805021643; 
sbyte GwBSqaCHgnJIuFtsDnJo =  46; 
long hWsBtKjZFzNXP = 85552683299482884; 
sbyte btDUlGqRXXbsV =  120; 
uint WWodjHI = 616394; 
short FnKVEZBqgRPXtmHPZ = 16362; 

}public void oOFScQCekuLXo(){
float NDmcy = -8.254044E-09F; 
long qgZWQDRISlbNZ = 42023055772902652; 
float cnBeDWobUglCWRGo = 2656.64F; 
string dTsOg = "XwzeLXyGWIpzoLDjhGO"; 
float nyGHusOyjVMXVZTDPxndp = 8.237217E+33F; 
double msDTLNZk = 2.358649E+14; 
msDTLNZk = Math.Ceiling(Math.Tanh(0.1));
object utBIygalFzhMNZoVKqfIwiiju;
utBIygalFzhMNZoVKqfIwiiju = -1.230387E-25;ulong adSxlkpmOnwtqfFmszZyyqWcYFVF = 10015543890199735; 
long pfhYlVjdmhaFwVypAKgQyHpLLTOl = 68992511162594750; 
short ebSVsNFXAELPQIu = 21956; 
uint PxnsgYQktWZaNEaaGfXDo = 124128; 
ulong kBAkfKOUAeTkM = 31034781834697401; 
ulong PQcnUXt = 37977523142379220; 
byte kgnzgPTkd =  116; 
sbyte zPVutKw =  -111; 
ushort WfSELNYxOhgxNsCJIaidQAldSLd = 35860; 
sbyte FjQpjiGDefmHJacbbES =  45; 
string UsdyUPMZKwqhE = "GyhtdeWcRiQkaQJALwEjozmNoRRe"; 
ushort oCygxFwAGXzSCUHJOVpjLWEuH = 9503; 
byte dlmRHZUoOUuee =  55; 
ulong LyXsgHcVOlftXdAnuaw = 59877942739370708; 
float lfAgYJjjSnyjsduTB = -7.015369E+14F; 
ushort KdQwWe = 14731; 
ushort PLjuCgHlZuo = 5140; 
int YksERIBZkzwDcKMcFYOBzMZ = 534331996; 
while(YksERIBZkzwDcKMcFYOBzMZ == 534331996){
YksERIBZkzwDcKMcFYOBzMZ += 359154;
}short bgqRkE = 1195; 
ushort RQBMYZdWBXQqpbnmkwbEulH = 59502; 
short wMTBzdEZKJknZ = 3146; 
byte OwAPMfcFaiA =  210; 
ushort iGuFEsdZzMNpsFbWwAt = 3898; 
ushort wDIpJoBp = 38063; 
ulong dBGUwYSmPXOcxOzDeNa = 40763375773117785; 
long AihaKnexbaBCQInIBlDjMxDE = 10751207809634364; 
sbyte pPGbMAugdMBWCBK =  8; 
short MAcdGFLwsDZYCQuqsAczA = -29602; 
ulong sQdtNwTUMOYIwmjbqqh = 54125690138462838; 

}public void ASkJKxLP(){
uint IXJflORMVepb = 16; 
float AnGtRDmJuTDoYZDuBZJkiaTxTQod = 0.003032849F; 
long OMXx = 27827791754137378; 
byte OWDVBiALPzujBbnjO =  237; 
sbyte BlbfltzEBhpCkpcs =  2; 
double CEdXzcYHARBlOiLbCUyKhKWjHiTY = -3.266174E+36; 
CEdXzcYHARBlOiLbCUyKhKWjHiTY = Math.Sqrt(4);
try{
int TppiaxBVSxPCOFmsuxxK = 5060658;
if(TppiaxBVSxPCOFmsuxxK == 18862){
TppiaxBVSxPCOFmsuxxK++;
}else{
TppiaxBVSxPCOFmsuxxK--;
}
}catch(Exception ex){

}ulong EYmUmoAQhzM = 36928585247437722; 
sbyte JFIICCbymWGNzan =  -106; 
uint gktdmVLM = 22; 
short IQHSVuFlLNysVDgcnKYbiJmXeFq = 15870; 
double sQfqoPfGfUAnAtNaiLPIyAmp = 1.30389E+36; 
while(sQfqoPfGfUAnAtNaiLPIyAmp == 2.776789E+19){
sQfqoPfGfUAnAtNaiLPIyAmp = Math.Exp(2);
try{
int exHXnsTEPE = 4555081;
if(exHXnsTEPE == 46534){
exHXnsTEPE++;
}else{
exHXnsTEPE--;
}
}catch(Exception ex){

}
}long UhPSPLzJTyVaDCdyaWfCQPnU = 54308306438604745; 
int YxaMCBXjWtMYcxbpEB = 1749; 
if(YxaMCBXjWtMYcxbpEB == 879037){
YxaMCBXjWtMYcxbpEB += 885699;
}double QtsbnxxZ = 9.752633E+09; 
while(QtsbnxxZ == -1.051157E-07){
QtsbnxxZ = Math.Ceiling(Math.Atan(-5));
try{
Console.WriteLine(QtsbnxxZ.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}sbyte DtQjHEwOKtoCizaAIhpDPbTEGnjSH =  -87; 
ulong iOeWlldCVMuBLmQpSUR = 66635837745812609; 
ushort DgCIKsuudnwe = 50596; 
long IsSt = 68364349450933606; 
int xZJKisGTTbUwVOGxsG = 114667243; 
if(xZJKisGTTbUwVOGxsG == 375684){
xZJKisGTTbUwVOGxsG += 199006;
}double DujtzOQRInNwGozO = -1.65137E+24; 
while(DujtzOQRInNwGozO == -2.958691E+35){
DujtzOQRInNwGozO = Math.Truncate(DujtzOQRInNwGozO);
try{

}catch(Exception ex){

}
}sbyte YsdJamXOKwpdhwZ =  42; 
byte uIlyeynfCDLE =  137; 
long bnNVHqlWqfQJCTwGYRtCBmB = 18531152888590797; 
float ofglOHciLUsoOMlgWNM = -2.227261E-24F; 
byte NAnkwBCoZXdmNnukkqsPsiZS =  170; 
long zotKwbquNAwHJjGtqL = 1540825961503291; 
uint FBxDMVcZGmJkxU = 71422333; 
ulong KFMQIgikMWNSTEDUPldSUN = 74162219751553440; 
float LghTLPSotCjflNtfJDewIXQXy = -7.341702E-35F; 
int ViOaW = 74; 
while(ViOaW == 74){
ViOaW = ViOaW + 3565;
}ushort AtEbuZLXGATBiNpkxFMojzsw = 6667; 
short HKZueaTzxtwMcThoKVB = 16263; 
ushort jmFnfFabhWdudm = 41046; 
sbyte ApnTRaSKwcjJBFDjjBMPRVoz =  -24; 
byte JJPUJEgaygEslMFK =  138; 

}public void sIqBjTNUpXFbQzGCjaGhCfuEf(){
string hBdlkNHdEhhzVhOnixwp = "MLDEd"; 
ushort mmNugmFumMu = 18407; 
byte btcDLZNRlhLNC =  108; 
ulong JgsqQquNqJEuaybgg = 57553753433079694; 
ushort GKu = 46463; 
sbyte McmWTFLZolbwsRPfEH =  86; 
long aINWNuoPdXYwQou = 64420351478403211; 
short kFmAgNTxgLuBafjxjlndaDDuqtsNN = -26029; 
ulong wXDQcpWjeYJEZojhVEhZBoZIbS = 4407953819149486; 
sbyte DtuU =  -56; 
string bTYucikRiXQtgYUOjhjPYoIJAjhGC = "nLwpXPd"; 
uint FjoBe = 14; 
double IGkNe = -3.500651E+37; 
IGkNe = Math.Pow(double.MinValue, double.MaxValue);
try{
int BGfpkDOElDFYBuZihdDUAKBKpYJMD = 9715869;
if(BGfpkDOElDFYBuZihdDUAKBKpYJMD == 39637){
BGfpkDOElDFYBuZihdDUAKBKpYJMD++;
}else{
BGfpkDOElDFYBuZihdDUAKBKpYJMD--;
Console.Write(BGfpkDOElDFYBuZihdDUAKBKpYJMD.ToString());
}
}catch(Exception ex){

}string RfxalUcaEFWFAlBq = "nZYqSzKFLAhtFansCqtxz"; 
int ThnydfOWdSuLwdhZIMxBu = 626578628; 
if(ThnydfOWdSuLwdhZIMxBu == 679447){
ThnydfOWdSuLwdhZIMxBu = 245498;
}double gVjKpejEaHzXEeCkpSousMKcdoY = 1.320713E-16; 
gVjKpejEaHzXEeCkpSousMKcdoY = Math.Sqrt(4);
string hGqGbfEXmLBKKUaHUXTK = "gODNNchAWQGzme"; 
float eapAWcyb = -3.120481E+12F; 
int bGPgulbRPTo = 43; 
while(bGPgulbRPTo == 43){
bGPgulbRPTo += 886666;
}byte PgxPXDkBepBGeA =  214; 
uint bzebdcwlMmc = 727359; 
int WlOiFR = 580266825; 
while(WlOiFR == 580266825){
WlOiFR = WlOiFR + 524541;
}short kEFZZSwgw = 15549; 
string Dkph = "tNt"; 
long MJTAjUWogCgZBJyBMMMQQbfDoH = 45913351616483797; 
float fBIBkgGDVELIKBHaHTcKDpjEHUFD = 2.587927E-23F; 
ushort QCukdlG = 44847; 
ushort IMWHqjXOoKpitPpdTp = 35335; 
string LhCRigwWuntnZJzIaEuGYVLT = "hGVZsphsOWEuQhdCRUxW"; 
float fRxTMhzRPyJia = 0.1953967F; 
double YkWk = 1.98263E+31; 
if(YkWk == -1.412707E-07){
YkWk = Math.Pow(5, 2);
Console.ReadLine();
}string jRkQEfhuQm = "AntYYlUA"; 
int ncBWWOgHecaswFpD = 687959; 
if(ncBWWOgHecaswFpD == 986674){
ncBWWOgHecaswFpD += 770927;
}sbyte DYl =  80; 
int upyNVOKTHsaO = 19850260; 
while(upyNVOKTHsaO == 19850260){
upyNVOKTHsaO += 781611;
}
}
} public class wHjEpBAVAuXCpcNJLVIJ{public void SLcEoldXRufyiiLjM(){
sbyte xTkAKRKLX =  6; 
uint kffEVhZao = 967001; 
long SdhUcAYOxmJHltNFBfwsiUscFPbMg = 70735901998260711; 
float cBzPTyWKPmxPpDqRXMfonymHAnO = -5.921958E+16F; 
uint cJl = 8341; 
long AGJemA = 15580504343581623; 
ushort HFnRpGJSYmltfeiAg = 54920; 
byte HjjNWqFxTbIYHhGP =  146; 
float UXQfamsbFiNDcPWUxwBVYalZ = 1.479331E+07F; 
byte YzuVnbsDQdjBFpZmfAGiKmi =  97; 
string RTLRiwZGdgPRoayudtlaqJbZTyZe = "MpaS"; 
short ASYVOBZm = 23583; 
uint jFMpiQicHi = 720765847; 
double xUDsROYYQXCfzyAsxiCEnu = -1.025422E+19; 
while(xUDsROYYQXCfzyAsxiCEnu == -9.007358E+23){
xUDsROYYQXCfzyAsxiCEnu = Math.Pow(double.MinValue, double.MaxValue);
Console.ReadLine();
}float bGFZAihlu = -9.66951E-39F; 
double NofIfLNgWZAtYdFLukB = -4.380815E-28; 
while(NofIfLNgWZAtYdFLukB == -1.530957E+33){
NofIfLNgWZAtYdFLukB = Math.Floor(1.080772E-15);
Console.ReadLine();
}double fVMc = 1.116425E+22; 
while(fVMc == 4.181206E-17){
fVMc = Math.Ceiling(Math.Cosh(5));
Console.Write(fVMc.ToString());
}string wHGiUo = "FsuoUZCMGmZkhehgwOsujna"; 
short iZZtQhVdYGTOMsLwMglyLSbm = 141; 
byte DUDqseAMaDtQe =  19; 
sbyte AnsKxHZ =  -11; 
sbyte SepYMSHNtjwnCJlHLSsqgtE =  43; 
uint hBYVVfjEmwxcQBisLXOkNJyPWgfPH = 518033; 
int NLfPynpeYxRkJc = 155491398; 
if(NLfPynpeYxRkJc == 623760){
NLfPynpeYxRkJc = 908790;
}short zdjYOZuWO = 25657; 
ulong cQdDmJygPmHFnemISQU = 6719488235483863; 
short bIWxdZcelwXyiykyGKKNN = -27127; 
double czCEaXOjgyNGKhkekDGwXnYdbGZPP = 1.895424E+32; 
czCEaXOjgyNGKhkekDGwXnYdbGZPP = Math.Ceiling(Math.Cosh(5));
object DwXZuEBQWNPGHNzq;
DwXZuEBQWNPGHNzq = -1.694555E+22;string TYPMhOTSndgDwKjBPqPT = "BHQQafPjb"; 
uint SVxlnZpsaMRWQfVYjlwEUg = 361956; 
short VgIdImFuLAmYmPaxSAKLaLehGXGVR = 13895; 
uint Uodj = 93932657; 
string qwuxLkZHDzoWKGYkcB = "KHKEZZjdYLRMVwe"; 
ushort pJxuYYtsxEunpSgjAu = 62845; 
long wxChbVLNtBFluVtedMIqkExQP = 78030527506856579; 

}public void UlUmXdIGdcPbWXOfxIpqIRZ(){
long tcbkWAzBIagj = 453480714003372; 
ulong VWxatRwedmiVEaHugDqCoW = 17912008193157474; 
int FHheOVgIeDXUaUkEdktRKl = 985032; 
while(FHheOVgIeDXUaUkEdktRKl == 985032){
FHheOVgIeDXUaUkEdktRKl = 494259;
}sbyte cMIzqXTCGkxKyMYNUMJgFRX =  67; 
string jiPmCHCqAkmD = "nLCFiwaPLaEHJRYjkP"; 
float YNKNtKQhfmNAqiw = -1.571966E-35F; 
byte tZFAhlGSQEgnxteGBaPkzGTJ =  117; 
byte oBwD =  33; 
double BzunDBORZFYYpgmDUunbpOgVbUhps = -1.004605E-38; 
if(BzunDBORZFYYpgmDUunbpOgVbUhps == -2.382314E-05){
double znyIGoKzWXc = 6.145501E+09;
BzunDBORZFYYpgmDUunbpOgVbUhps = Math.Round(znyIGoKzWXc);
Console.Write(BzunDBORZFYYpgmDUunbpOgVbUhps.ToString());
}ulong JxGT = 65502583800161484; 
long jfMLRo = 16481721438062353; 
uint gBNUDRP = 63; 
short MBQB = -11465; 
sbyte QcnZUBWBebmfONBysMXQGsp =  56; 
string muICdpZqOzTgHlXbMO = "CBolcyPtphzMDVpmDSEXXfx"; 
double Eke = 1.358947E+26; 
while(Eke == 3894.537){
Eke = Math.Pow(2, 2.1);
try{

}catch(Exception ex){

}
}double lVVbiuyxl = -4.886434E+08; 
while(lVVbiuyxl == -9.532845E+10){
double mMBhMdxBPEshidbTGKtj = -4.075049E+18;
lVVbiuyxl = Math.Round(mMBhMdxBPEshidbTGKtj);

}ulong zptYLRgCABFPzmmCJCgzZMJ = 10957850257509341; 
ulong RwOMhQFhBUTxtLsWZnzMnONHFWD = 61425182327444959; 
float QCLCpASQqJzmHFYu = 3.384712E+10F; 
string ggkntFDHjEAeBxiNNkYo = "RnndZXtnoTmEjtKpxiMXkGNiuMj"; 
ushort HPJOOpSX = 12906; 
ushort itVLlBqdAypclqKkBeK = 16647; 
ulong cIdEQxcRslPfPSKcXUwfWOpNyydes = 73294133950603063; 
long UaIfemgDSBzhQRIcoqqgt = 28184694684963447; 
uint wnjBenJlAqUlEU = 68367627; 
short haWpDqDc = -21029; 
int FUXpecTHqEOG = 83; 
if(FUXpecTHqEOG == 115840){
FUXpecTHqEOG = 385146;
}int ctbxCAfMSYbecUUyXWASTcY = 38134746; 
while(ctbxCAfMSYbecUUyXWASTcY == 38134746){
ctbxCAfMSYbecUUyXWASTcY = ctbxCAfMSYbecUUyXWASTcY + 740190;
}float JFTolSBOFWgOtC = 95303.74F; 
byte sCjUNUawucgJIZZCtBiFJFxhUVMn =  153; 
sbyte RGCuqmOSYoQzyIWogPRij =  -29; 
int OfbaShMQns = 670; 
while(OfbaShMQns == 670){
OfbaShMQns = 277186;
}int jpSgYBYGGAOXByYmGllO = 81; 
if(jpSgYBYGGAOXByYmGllO == 184893){
jpSgYBYGGAOXByYmGllO += 100075;
}short iBfDFCjqwHgCblDlbqRSeTZxcR = 10227; 

}public void MSXThfyetktZm(){
long ZeKMOmqYSnBOoCKANTQFEKntW = 58910473632123897; 
string qse = "guUeGIsiEKLGgydmpRqPaPAhUw"; 
float lBshJZKjKFI = 1962468F; 
int hftLhtPLAboTqVMkjMaWcqEWgNU = 710657; 
while(hftLhtPLAboTqVMkjMaWcqEWgNU == 710657){
hftLhtPLAboTqVMkjMaWcqEWgNU = 417229;
}float bBKNYyQSftgs = -1.826141E+34F; 
ushort ZEJTQCxFwAkZpyd = 29247; 
uint LKAZcAmAzEawGHqMFI = 1171901; 
uint efqzlQDJoKULmKYa = 170054; 
int dTEaUFJFSxKy = 20; 
if(dTEaUFJFSxKy == 650055){
dTEaUFJFSxKy = 573941;
}byte WPnqBUtINFfFui =  109; 
byte qFHgXQSZxIcdFsXVi =  102; 
short mAZlaRnpfcTOuRpZHpZ = 31675; 
ushort dmqEiliKlOhG = 19372; 
float wgfDCUuOnKNcYnTwojV = -3.095845E+18F; 
float JchxReGODztljT = -2.334244E+27F; 
ushort zcUxjkwttSwuxSAJyAuzE = 44232; 
long kEFbJCSqzMoSDmTXayMOshcCNBx = 50798656173685559; 
int uKRaeWLEIgjHxzfXUDc = 69097; 
while(uKRaeWLEIgjHxzfXUDc == 69097){
uKRaeWLEIgjHxzfXUDc += 904585;
}float FAyEbmqGOVNnlsXe = -7.866673E-22F; 
short BbSA = -4611; 
sbyte DfBLf =  -65; 
ushort ejDit = 6590; 
float bhoQigjPUFQY = 9.260001E+09F; 
double ThPBmj = 5.594899E-38; 
ThPBmj = Math.Pow(2, 2.1);
try{
Console.WriteLine(ThPBmj.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}byte oRNgikkYNsmZibPdbZNLEaBAjIOqb =  64; 
double tjsPToGOLPRggLBjcCAdiBwoqnWCx = 9.880765E+32; 
while(tjsPToGOLPRggLBjcCAdiBwoqnWCx == -1.066543E-19){
tjsPToGOLPRggLBjcCAdiBwoqnWCx = Math.Ceiling(Math.Tanh(0.1));
try{

}catch(Exception ex){

}
}uint pNK = 51; 
int YUztS = 424832; 
if(YUztS == 435110){
YUztS = 754912;
}byte DGwpHhXSGBtGdmjakHs =  46; 
double LTLAmxGoxsqVzKs = -129352.3; 
while(LTLAmxGoxsqVzKs == 2.597677E-19){
LTLAmxGoxsqVzKs = Math.Pow(double.NegativeInfinity, 2);
try{
int YCLfqP = 4806984;
if(YCLfqP == 92158){
YCLfqP++;
}else{
YCLfqP--;
}
}catch(Exception ex){

}
}double MJxZqqpyfYVuLRdHCw = 1.694306E-35; 
if(MJxZqqpyfYVuLRdHCw == 0.003744125){
MJxZqqpyfYVuLRdHCw = Math.Ceiling(Math.Asin(0.2));
int[] pHTzLFGgD = { 474637 , 26309 } ;
Random ANXGdHYohPYwjyHlMFOfAHfWKRVl= new Random();
Console.WriteLine(pHTzLFGgD[ANXGdHYohPYwjyHlMFOfAHfWKRVl.Next(0,2)]);
}short hkCzwBSxnHwj = -18415; 
float xSzSbtyzTHbVmLN = -1.272985E+30F; 
uint mYViRkMqRTlVLPtAuEw = 9873; 
long JRtGGMCtBMGIABJHIocGFHM = 8957321405590072; 

}public void DkmqzGgGoALiehUYLWO(){
string eibfbzMAjLbEZennsibhyqfWDlZZ = "IMMgKzcklx"; 
sbyte ffPJGfcGKtOMS =  -1; 
ulong eEPA = 39561521184653392; 
byte fNUN =  86; 
byte mXgWtEnYZoj =  178; 
double IHIITE = 2.896532E-10; 
if(IHIITE == 5.964334E-07){
double SeiDN = -2.689014E+10;
IHIITE = Math.Round(SeiDN ,1,MidpointRounding.ToEven);
int? tJnZphfWlWzflfzIBT = 7119552;
tJnZphfWlWzflfzIBT += 79490;
}float ZKUlYSsbwNnciYeOYIx = -8.865635E-30F; 
float tKQOGbPkkc = 7.155679E-26F; 
short apjGeTzbOU = -30743; 
int jctNAixBDhdMZGCcZIPOBY = 2305; 
if(jctNAixBDhdMZGCcZIPOBY == 961730){
jctNAixBDhdMZGCcZIPOBY += 795122;
}short WNapFhSpbbUNzigTemWZPlmRklL = -23596; 
byte jHiGkqLJbtjKHOT =  190; 
uint fJXLTthL = 189686; 
sbyte yLeERmFSXPhw =  -70; 
int fGAXzLBbDBAOQzBRCEIWAFdW = 16549575; 
if(fGAXzLBbDBAOQzBRCEIWAFdW == 308027){
fGAXzLBbDBAOQzBRCEIWAFdW += 503482;
}uint oEbfalIIWJCcqzO = 4386; 
double SEHoBCDqobnMcYaAlywQ = -1.377082E+32; 
SEHoBCDqobnMcYaAlywQ = Math.Ceiling(Math.Atan(-5));
for( ; ;) {
Console.WriteLine(-112.3017);
}double iTuOiCOqaQ = -8.236848E-35; 
while(iTuOiCOqaQ == 3.010047E-05){
iTuOiCOqaQ = Math.Ceiling(Math.Sinh(-5));
int? cqLkI = 5822212;
cqLkI += 35616;
}int fLjjzSNwEzsIqx = 4245; 
while(fLjjzSNwEzsIqx == 4245){
fLjjzSNwEzsIqx = 345356;
}byte hpHzGaNXgSecJfYIpXUMULX =  151; 
ulong HSkjskltDUDamMIHqAE = 34776085086594197; 
uint yscJZtUMXBUQoDefpLhentzbB = 388571675; 
ushort eoNOCfDAqmtNUuZJyALtu = 50278; 
short uFdUaigYAbS = -18742; 
sbyte NxDG =  124; 
uint stYztctKbpqXxtXcIAifumT = 775532; 
uint MnROOTDckfwIf = 29094676; 
uint BmLJQuUgRsueAxKRJIwjCD = 87; 
short XmRdsfOhbBQLVUXHXChXLJ = 13896; 
ulong fTojXiRseyHpOZ = 13199156693942325; 
uint UFDMuanCa = 23509177; 
byte YdsYtRxqqjCTHtoC =  200; 
uint qcotGfXdjeogN = 81; 
ushort RcEVeVEwbSN = 42467; 
double lgS = 0.0001906451; 
double lccOQQZYLMW = Math.Log(1000, 10);
lgS = lccOQQZYLMW;
Console.Write(lgS.ToString());
}public void qXNhGkqbgYcTIyntGDPq(){
ushort kGqlnNkPERCBbO = 55610; 
double WQQDDputzuHKXwcnUN = -3.449718E+24; 
while(WQQDDputzuHKXwcnUN == 3724599){
WQQDDputzuHKXwcnUN = Math.Ceiling(Math.Cos(2));
try{
Console.WriteLine(WQQDDputzuHKXwcnUN.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}short JBkVGMyzeITWDgWTmJ = 22139; 
double YtoisPwozkJmesFHXVwOOh = -4.000888E+33; 
double boSLgWN = 5.228174E+33;
YtoisPwozkJmesFHXVwOOh = Math.Floor(boSLgWN);
ulong TqlLZqGdUU = 83418569325603495; 
short LlhTEghYBtoGFXgpG = 9305; 
sbyte cUsNFeIsiBOdpkEIjshTjHE =  2; 
int aGgYFjoFAQ = 398224746; 
while(aGgYFjoFAQ == 398224746){
aGgYFjoFAQ = 787356;
}ulong GlotuAgnBpEwqSiqeckjVEPMMNPPd = 79795328072309487; 
long IRqKYaeA = 72608208829543220; 
string EIILsjRWiEOXQt = "REsFTQMHmODgoX"; 
float ASWwZEXA = -3.620173E+27F; 
string BMdxEXGanAiHRauXQdDYstneU = "ymHTyaPUfCKgDqqRXZKpEYjZJdpc"; 
uint yZDGBtu = 378; 
sbyte WUTnJBkIBwnuEuzcHjuLMwYdK =  20; 
double XyEXDzHepMZEewYMjGYlcpNjI = -1.475595E-30; 
while(XyEXDzHepMZEewYMjGYlcpNjI == 2.611412E+23){
XyEXDzHepMZEewYMjGYlcpNjI = Math.Ceiling(Math.Sinh(-5));
try{
int GLPHKt = 8614177;
if(GLPHKt == 77244){
GLPHKt++;
}else{
GLPHKt--;
Console.Write(GLPHKt.ToString());
}
}catch(Exception ex){

}
}float IFTZRRQeKWBATGc = -0.0654586F; 
string OIlkMoRUFCsbpOmgcRtznC = "yFJ"; 
ushort fJDuSc = 5025; 
ulong thLZWlEsUjLHzZ = 65467302526075976; 
string LPVYXinKxLKKJzqpxfKzLAHJan = "FSiHMdRohpF"; 
ushort WbjOpTydgWkKWGilhmYKTJ = 55705; 
byte XGRj =  46; 
string LYCKpTshQkyVXXQzIdacHLSni = "LtmGUXVtYXdAWnURTJkWpfhgigiX"; 
byte gnB =  94; 
sbyte UkqBaFYSuihMiPRjcmGFlgxOOXhYR =  80; 
double fUXJqLWMgoMaZyPxi = 7.604511E-22; 
if(fUXJqLWMgoMaZyPxi == 530985.8){
double NdxabFkFGnSiSsqUyW = Math.Log(1000, 10);
fUXJqLWMgoMaZyPxi = NdxabFkFGnSiSsqUyW;
object hMPTFLPCgkUO;
hMPTFLPCgkUO = -1.40398E-06;
Console.WriteLine(hMPTFLPCgkUO.ToString().ToLower());
}long duzFqJnHwz = 65084198090511461; 
short DaBXGUfhpFOtXMbAAnPidLtYnC = -4222; 
ushort MTITPMdUBZ = 29308; 
int qQLmk = 74; 
while(qQLmk == 74){
qQLmk += 639125;
}sbyte SICKdyNUEMfBhZYFZoqGYuDn =  -124; 
short HczjaNZTwtRV = 32186; 
ulong OdFNcdPHo = 86067203928700021; 
ulong gReCe = 29722559931508331; 

}
} public class MGzKnnoSwiRLmjwqfhiVKuJD{public void ZUXlwgZYKeSGEwzP(){
short WLuAZLkfakxfsX = 28323; 
byte usGZcRNEeD =  184; 
uint KfwXNReBVlaogPCFMVJugex = 43803690; 
float IWUiAFmBqKukeeNzgFLVbkYsIch = 2.546935E+08F; 
long CSXyEbdCLTnjUsKEotGpeMY = 88222052116576326; 
float XTshHjyddZcNbdaYzSma = -1.260971E+12F; 
ulong ebGBDemluCXAsxIjhmbMWfwUBDY = 55760456081558684; 
int XBYNQtAgBHcjcSfIcY = 625158432; 
if(XBYNQtAgBHcjcSfIcY == 3423){
XBYNQtAgBHcjcSfIcY = XBYNQtAgBHcjcSfIcY + 121803;
}long AlIwUscHkNcZfMVNaWVwsbjIb = 301717897526558; 
double mAeMLgwHtFcSmLZYdkmAzbauum = -5.290202E+23; 
if(mAeMLgwHtFcSmLZYdkmAzbauum == -7.063767E-24){
mAeMLgwHtFcSmLZYdkmAzbauum = Math.Exp(2);
try{

}catch(Exception ex){

}
}ushort kSOjqbLISWzlfVDITUsaBoFc = 18913; 
int qSdaW = 93373496; 
if(qSdaW == 226871){
qSdaW += 772531;
}byte BpE =  208; 
double TtmVjPfxnVmMVLXtQO = 7134.545; 
while(TtmVjPfxnVmMVLXtQO == -1.270414E-34){
TtmVjPfxnVmMVLXtQO = Math.Pow(double.NegativeInfinity, 2);
Console.ReadLine();
}byte OoUJFRRHUhIAjkcDTb =  142; 
int lGUwVTZwgJMl = 343139614; 
while(lGUwVTZwgJMl == 343139614){
lGUwVTZwgJMl += 538347;
}ushort ecMdoSinVVcyIlCKZ = 17892; 
long SkNAVnmTHBpAgbmiTQgcLMBkHnEL = 23854844326250556; 
short hbGoIuGDp = 8100; 
double QihHlgKCSIKD = -2.223999E+30; 
while(QihHlgKCSIKD == -2.052999E+09){
QihHlgKCSIKD = Math.Ceiling(Math.Acos(0));
for( ; ;) {
Console.WriteLine(1.988568E+31);
}
}float QElHOqKDkBxtXsnOILs = 4.44471E+11F; 
ushort LImkQ = 20770; 
string DoHqkiDEU = "EehKFOaPoOR"; 
short XxABYBfTlknTepbdGegyEdoWaczk = -31482; 
string wnsKUMIFd = "ysUUQBzZMKKKRsThDBVbVSkX"; 
float MHgsXAdIfMubjDakosD = 1.349787E-05F; 
string NihJbUFgWckPSStNjHilhNCp = "aKDTwgiMQtjP"; 
ulong OCG = 12646575878754200; 
short nOJaI = 22026; 
short kbn = 1608; 
double dbz = -1.322298E+34; 
while(dbz == 2.638093E-08){
double pFSpHncbVXgtCXAZDA = 875.9333;
dbz = Math.Floor(pFSpHncbVXgtCXAZDA);

}ulong JeTaR = 52972818181081999; 
ushort sekUzkDfPjhUT = 23928; 
uint BIOWZKcdem = 290761769; 
double jCWdQNuqqVgZe = 5.441128E+12; 
if(jCWdQNuqqVgZe == 9.754142E-23){
jCWdQNuqqVgZe = Math.Sqrt(4);

}
}public void xsYZYSPIUTOlfVBPGQpeLkjEGmMY(){
sbyte FNofttPBOM =  114; 
short ZMVpigSFAGckkhwtLfRBGn = -30797; 
double DhafcEhwAkRKXERoJeglVhktgdGgP = -2.779167E+16; 
DhafcEhwAkRKXERoJeglVhktgdGgP = Math.Exp(2);
for( ; ;) {
Console.WriteLine(-1.526509E+32);
}sbyte iPNCQtYXIQBKVUdjYqA =  -22; 
long kIjayXcRuBizYOyaZTmowxa = 36964176514153828; 
long uFesPWaUQwaCxVpNXsGiwsOIwSy = 82801806983261748; 
sbyte JjXluGaamMeP =  -16; 
uint SDAcRyWXzhbzfXdTqguoiI = 18485433; 
ulong HifsexREkVdBsVMlAetgVdi = 55212099038420326; 
uint dQOSfcFHCMi = 81623879; 
ulong DWiSIGREalGyuFa = 53864294755994257; 
ushort PBu = 22064; 
double XSEKI = -2.799504E+20; 
XSEKI = Math.Ceiling(Math.Atan(-5));
Console.Write(XSEKI.ToString());sbyte qxfAAsNVunIGGUoqnGNmN =  19; 
ulong nUzqbwlnKWPylSLOtqELXiuSiCM = 34204928439273805; 
int wUYcuVJfSfxVZUxaeXeBHiBJzT = 99490539; 
while(wUYcuVJfSfxVZUxaeXeBHiBJzT == 99490539){
wUYcuVJfSfxVZUxaeXeBHiBJzT = wUYcuVJfSfxVZUxaeXeBHiBJzT + 626624;
}sbyte XyFIEmLMqwpSwbBNCngdmluoEC =  -91; 
long bfQOf = 16318175162715963; 
float tGyMz = -1.221168E-17F; 
ulong cGMdoecfcIopAdpf = 20146843223353449; 
short XdwjQNtoMAkVfsqMNmEOTDgHXXjR = -17784; 
ulong sOtgYKSRD = 8408586421483319; 
int CfQBcwWPmTEDdXmSg = 22774495; 
while(CfQBcwWPmTEDdXmSg == 22774495){
CfQBcwWPmTEDdXmSg += 416273;
}string JjqMpMymKlhCmpLbVGYZuguDc = "ABlAIKPKulGdWgfo"; 
ushort eqUxBqgoycyDQOJRmXbjFy = 47948; 
int AxSJRTeNJzMGcZVssmNmRxRueUin = 14; 
if(AxSJRTeNJzMGcZVssmNmRxRueUin == 309561){
AxSJRTeNJzMGcZVssmNmRxRueUin = 486831;
}string oGGisPhpXljXgRlkYqedGyEXLRD = "RgoEXadMIFUcoByTL"; 
byte sVxGYu =  247; 
short bIyBlGAQWGZfOADuxcPeZLlBK = -19446; 
int BpjZMlLZguQJa = 92803856; 
while(BpjZMlLZguQJa == 92803856){
BpjZMlLZguQJa += 587156;
}ushort lDqjjSqxXRbZfoHxauDPEmcmfe = 23265; 
ulong pIGkAn = 44020329056812195; 
int JVspJjIcGqZAddfADoIjwPG = 83; 
if(JVspJjIcGqZAddfADoIjwPG == 849643){
JVspJjIcGqZAddfADoIjwPG += 160044;
}ushort cBAyiazzfaEFiooOMxURoDtsToqye = 65089; 
ulong jqWYFGWzdQCiSjxMDQzwnELBhh = 30273628230660528; 

}public void StpjfBwOzajAK(){
float VnBuGHGwRybbPcwxXiP = 5.859837E-13F; 
byte tUSQlBXIUbdfgFRdOmY =  120; 
ulong zGMdE = 80333217959230001; 
uint QtVUPhaMWoRgeaquOUowlei = 711082837; 
short OQyAxspYUcEWWzUPPNflUXTUNTdCM = -20024; 
double AimVzPu = 2.020098E+22; 
if(AimVzPu == -2.776171E-14){
AimVzPu = Math.Ceiling(Math.Acos(0));
 bool? BfjxQQFwdxYUcRzaPNnPBFLs = new bool?();
BfjxQQFwdxYUcRzaPNnPBFLs = true;
}ushort yxYqORfEafOpxYTaYyUUqEyqt = 13988; 
sbyte fZIgNNtyoAnTKywRM =  17; 
double UnVRb = 0.6672774; 
UnVRb = Math.Ceiling(Math.Tanh(0.1));
try{
int RelmAz = 1496659;
if(RelmAz == 33734){
RelmAz++;
}else{
RelmAz--;
}
}catch(Exception ex){

}int GXHDpxeXcYzQeTPmAOUFinIjdVF = 10003138; 
if(GXHDpxeXcYzQeTPmAOUFinIjdVF == 382379){
GXHDpxeXcYzQeTPmAOUFinIjdVF = 633877;
}byte xNDMUuCCyjllO =  235; 
sbyte txZOsoyzuRpSxFeiUz =  -37; 
uint cajHMkZSKQTYcaXIxcgodqceKeSf = 34; 
short HHpxQgGdyHLbyTuFgC = -2730; 
ushort jTMYeXIt = 58706; 
ulong BosxKCRoxdSy = 74989164784356565; 
float sxOFyLjYLDRtBhBXVGKhZhsweX = -5.038893E-30F; 
double WnyIXW = 6.294734E-32; 
if(WnyIXW == -2.333981E-30){
double nCBpNssRMTjdsBAmdbEH = -1.442979E-12;
WnyIXW = Math.Floor(nCBpNssRMTjdsBAmdbEH);
try{
int FoQCDcBPc = 7180089;
if(FoQCDcBPc == 46177){
FoQCDcBPc++;
}else{
FoQCDcBPc--;
}
}catch(Exception ex){

}
}uint nKsubEJdplTPcfkdxMZlXKZda = 7279; 
uint IfLmNibxtRNVBEQEZHXgFTZ = 305597; 
float KnuIRjpJghCoXiIdUm = -2.313212E-05F; 
sbyte BaNOtiO =  -99; 
byte UFshZjVMkUKwyqaXWQflZxzBOtUc =  210; 
sbyte aWFwtRBeKqFllAeDwOwmYaaXzfS =  -32; 
int SVdZJATWHgw = 40538910; 
if(SVdZJATWHgw == 190120){
SVdZJATWHgw = 239728;
}string QzoxUWZVHWmYgzuzKhlRG = "AgnRlGfDnTSLQREAjdJEm"; 
string homawMbaidgZh = "cKuPeIt"; 
sbyte JDVjYihbxANfDZ =  22; 
long mTJHJSwqgtoPisabSEzBCdSa = 7537657511802879; 
string wBzQZKATRQ = "lngMeKwgjniaIEYsapGIAKUWBE"; 
string ObJmchQxtKWRoJUZmA = "lbqAEkhuDtklCIRsiGVYBNCZ"; 
short PsQPVKaNQDxC = -25731; 
string NndizwizqMtqCozAshkwPFNyCkO = "nVHBTeOAzRTXGUAdCsAzqcetV"; 
int iCGIm = 32349465; 
while(iCGIm == 32349465){
iCGIm = 876841;
}uint CZbComQVeJWiMp = 217377; 

}public void mFmZFYSjGpoKdZIUcXqbEeIRi(){
string KzBqPYFRokyWbjqJWBkbJnnKsj = "miWxQCeh"; 
long ZALzi = 34255502418439850; 
long YbjwkRZGPqI = 77326673956988940; 
double ADB = 421778.5; 
if(ADB == -5.425486E+28){
double gSAVNBoaUW = Math.Log(1000, 10);
ADB = gSAVNBoaUW;

}double QuaklUDshBRGUo = -1.861925E+17; 
while(QuaklUDshBRGUo == 0.2412259){
QuaklUDshBRGUo = Math.Floor(1.810814E+24);
try{
int GIAWCFUWuVHWIj = 8529116;
if(GIAWCFUWuVHWIj == 714){
GIAWCFUWuVHWIj++;
}else{
GIAWCFUWuVHWIj--;
Console.Write(GIAWCFUWuVHWIj.ToString());
}
}catch(Exception ex){

}
}int iVYoz = 8124; 
while(iVYoz == 8124){
iVYoz += 655866;
}long UihGFGFtyIxkWmnwROVdfoT = 83249856876885064; 
float UNARAnuaMLmDeDeCPlJ = 1.308643E+11F; 
long opsGJFqqmFGbBPFsACd = 4657710243306201; 
byte MfXkgkBgJMwuuZXzBENjTikEobJ =  3; 
byte QhdxBAJ =  209; 
ushort dlRQJZiHqKVoLdaDUt = 60804; 
sbyte BXAVnpXzzQKkigIsw =  104; 
short bJAnCyawYGoUncEnsUwKlhTTkd = 2323; 
double kAzHlBRykuRo = 2.255325; 
kAzHlBRykuRo = Math.Ceiling(Math.Sinh(-5));
try{
Console.WriteLine(kAzHlBRykuRo.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}byte ONKCVFRDTaWlXmP =  71; 
long AKdDhTlxVLZ = 66644951639627868; 
float RDxqXbQIu = -4356.422F; 
uint xCukGRozJgDVyncMtDHN = 65; 
sbyte ljSxOCzyyVmHQIIRFOix =  93; 
ushort NNWaKVOFOuNZwYTL = 9498; 
long yDbwVL = 4800310613751028; 
long KSCOC = 42455072817100503; 
long aONY = 62268801332338963; 
short KYjMTiSHUF = 17504; 
byte iScIXymzXbRMtsSlY =  110; 
sbyte jEf =  -6; 
uint DjuTijMjihmhhgcPMWsaAQauWhJks = 74954315; 
int YVcedSH = 18; 
while(YVcedSH == 18){
YVcedSH = YVcedSH + 815311;
}short ZuRigBAMjZXbCBRDwJailZLJdLZpG = -11543; 
ushort SKkXO = 38221; 
ushort ziTegRfAsIopQfNZKiex = 33719; 
sbyte SFSiRp =  94; 
ulong msLcgHCmGAYYbfoNXknGAagob = 73416942514380103; 
uint ywKT = 62; 

}public void QakhRADxUeFiqllxaghnEqpAf(){
int Rqns = 9772; 
if(Rqns == 113477){
Rqns += 66684;
}float ZMmueUK = -3.463234E+24F; 
byte JNVtnALMcmyLeHNtjJcfsgXhCqXlG =  108; 
ushort GsFFqGuL = 26702; 
short WDahOqBAzOgHCQNRWCzgGRlfM = -30985; 
sbyte FoqL =  -122; 
short FCi = -20420; 
float WXyAM = -6.6292E+22F; 
int yiWjlfpVwMizVjywGmPhpNsF = 67; 
if(yiWjlfpVwMizVjywGmPhpNsF == 627812){
yiWjlfpVwMizVjywGmPhpNsF = yiWjlfpVwMizVjywGmPhpNsF + 15635;
}double zQRiORdVoHOy = 1.83584E+09; 
if(zQRiORdVoHOy == 6.409166E-39){
double wzKRs = -2.210828E-29;
zQRiORdVoHOy = wzKRs * 2;
Console.WriteLine(zQRiORdVoHOy.ToString());
}ushort HfCPKWzMUhNL = 33221; 
int WOkXkeJlgqHkZPL = 6089528; 
while(WOkXkeJlgqHkZPL == 6089528){
WOkXkeJlgqHkZPL = 233197;
}uint yuYXGBaISdxKhkOkn = 893485983; 
ushort cMAWiAKEYflwmOD = 15791; 
sbyte LfKP =  -93; 
int EyoWn = 339710; 
while(EyoWn == 339710){
EyoWn += 665056;
}uint ybq = 87; 
double oVylYhWi = -3.63676E+21; 
if(oVylYhWi == 2.404387E+32){
oVylYhWi = Math.Pow(double.MinValue, double.MaxValue);
object acYfGiPnfdDiRaWziuoYL;
acYfGiPnfdDiRaWziuoYL = -7.420962E-11;
Console.WriteLine(acYfGiPnfdDiRaWziuoYL.ToString().ToLower());
}ushort cYasIFYMBWmQkWOKSh = 47243; 
ulong RWWLO = 55358191387786270; 
sbyte ZEQExDzPRCagDQJbIGAkbeIUDJ =  -113; 
byte BhkQWKoqhtBtgfiNWMTGcecBV =  210; 
float WbNBkYpuPqHfuLBmYSUYAgGWjIP = -9.81273E-32F; 
long ePswfHahiQOYGfNksEPCdWWxPx = 29759945243157656; 
uint bowHXKMiLhZR = 425022; 
ushort piQiSVGJQLEZXAUjFwjchiFtkO = 47546; 
long ZThNZYVHsAfKNQm = 30600033269331473; 
byte LhFxYejYkRSwm =  242; 
uint DlWdtwssmzDBtINRVsnRqjuwVLCZw = 48030216; 
short omZeibpPmjBDKGEeaBVJK = 9566; 
ushort ChIUtWDZnQduNNNpYceaUTlfX = 59103; 
ushort EGaIThEa = 24796; 
ushort pHBnUCiqnNFKIE = 24054; 
double WDWzDswIPxBFiMXEHPtt = 3.201783E-13; 
while(WDWzDswIPxBFiMXEHPtt == 142254.5){
double teBjVeRzXOoeGefJMeaIgaOaFdF = -2.235376E-05;
teBjVeRzXOoeGefJMeaIgaOaFdF = Math.Sqrt(3);
WDWzDswIPxBFiMXEHPtt = teBjVeRzXOoeGefJMeaIgaOaFdF;
try{
int VxNwqawmIfeJbdn = 3403181;
if(VxNwqawmIfeJbdn == 75670){
VxNwqawmIfeJbdn++;
}else{
VxNwqawmIfeJbdn--;
Console.Write(VxNwqawmIfeJbdn.ToString());
}
}catch(Exception ex){

}
}double fqAcdtAdXdb = -9.389401E+22; 
while(fqAcdtAdXdb == -9.268639E-21){
fqAcdtAdXdb = Math.Ceiling(Math.Tanh(0.1));
 bool? AxkplLel = new bool?();
AxkplLel = true;
}
}
} 
public class UlwzLQSnaPETKJcMEqUIZqEXE{public void CCdYEPljAyTbJCuVgLKn(){
ushort fzDkApSEIzVeoqkbk = 51302; 
long iGSDJsmejTnomaxG = 79133641494395787; 
sbyte ldKeAVGUsBGZCgcNuP =  -79; 
int oVGFSzMimPyxefjufttKtJVZx = 169701586; 
if(oVGFSzMimPyxefjufttKtJVZx == 538817){
oVGFSzMimPyxefjufttKtJVZx = 19005;
}byte NXieAIxUMeLiEybSt =  71; 
ushort VzHZGhapSELOlUqibpGMXQIEk = 54464; 
float hblmGuGopizsH = -2.882808E-37F; 
sbyte WWJgSRcJKUfRc =  70; 
ulong HhRsyiSobS = 35828383601447992; 
short jmokiEWABzmuKcZXjPVOdueZYlw = 5850; 
string qTBTfuSHjTm = "OEFygkfTFLoB"; 
float iplWiqxjhuUQXGeDGbBLGBwB = -1.07803E-23F; 
uint LGEZbpkD = 248800; 
long WFxjDpnAOcfmZgiIULSYSlzCQUP = 43326287046316015; 
string kXOWZ = "JBqZVgEIKwNdKg"; 
float JCIflUNWushEK = 8.048396E-34F; 
string QPaAVTARUzXEDeUiRomnTkOOwG = "ZZYWqLjUqVUNJinsxP"; 
short mmuTzROZFaldooBRWDGTsjEnW = 2336; 
int FTsYdN = 7276; 
if(FTsYdN == 455641){
FTsYdN += 509614;
}double opJRztIx = -1.224071E+28; 
if(opJRztIx == -0.02645686){
opJRztIx = Math.Pow(5, 2);
Console.WriteLine(opJRztIx.ToString());
}ulong LESGqqFjnhREYSKMaVdFXnJ = 40259732815643541; 
long xUklXb = 30645590222369752; 
ushort MxuJhRyUGBNdfmUB = 12787; 
ushort YzBcTmMGJXbApRZkX = 16936; 
double FxpGxnONeLjajMzaltBxwkOXEbymg = -2.446916E+33; 
if(FxpGxnONeLjajMzaltBxwkOXEbymg == -4.28678E-06){
FxpGxnONeLjajMzaltBxwkOXEbymg = Math.Floor(459.3244);
object XSqOVXxKPjDMTUhctAJSETPS;
XSqOVXxKPjDMTUhctAJSETPS = 8.467069E+21;
Console.WriteLine(XSqOVXxKPjDMTUhctAJSETPS.ToString().ToLower());
}ulong GhLTjAJXHzawyfVkddzEgadkF = 40468456494558895; 
long LTZF = 54642162111848338; 
sbyte EqdjmLFdAnRjzTgzHSP =  -114; 
byte ezfbpKjOPaiF =  131; 
sbyte koXzHtLNicYjxQXEdxkmFdkwVUMA =  82; 
string sYBxDSFDYtLWZsBOXSxeioVtge = "DNIlWhiGkVuuLATmU"; 
float ldwgPH = -5.796997E+32F; 
long dhoz = 20418320674915922; 
ulong SDzLGocbAZS = 14668594943748889; 
int NeVuyc = 754362477; 
while(NeVuyc == 754362477){
NeVuyc = 333056;
}
}public void nliJq(){
ushort cIp = 40585; 
float jzClEtiieyDphlgnwHygcqHyTU = -15720.66F; 
sbyte OoDET =  -14; 
byte QbiWjOGsijTOlKiLRTSdtJsGd =  251; 
long OEtKcERwYf = 32466187017918438; 
float fRMaG = -0.04316116F; 
sbyte DSjesEFQjMJTOMLLZmRYC =  109; 
ulong GhWRtd = 32651623124731247; 
uint faAFkksfaoGVIiiGeeAjINyfSEic = 4603; 
ulong NUSBxhVuP = 77445824563679; 
float TnppsEu = 3.484423E+10F; 
string PDCuiOpJiXi = "uQXsI"; 
float IBApUY = -475749.6F; 
uint cwntAbmDdQKkUIkKiVAwkMUBuPpm = 42124655; 
long RMgCVMU = 52196599039872245; 
float jQCed = 1.542896E-15F; 
uint ZKMaVIeBuhBKPxbLMwhRcXj = 176789; 
ushort jdNaRahNjcKUXYXczqu = 26390; 
ushort uqpiKKbBmRzbUxHg = 44680; 
long JRIcBkSfVLhOcmKKwLJp = 2492269984626786; 
uint qIMOLKMoPXSFgP = 5; 
ushort kMkxasjXhSjeoOGAuAcoCZYg = 60037; 
long TFULQbgSPkUHtEO = 37977023320325875; 
ushort ZgmkOdBpTqsngudhVQn = 32474; 
float LOyeckbwUXzfRUTHagupA = -1.239278E-07F; 
byte KIBhitVMpwAOKapnzywPbEi =  72; 
short hfDjzsjssDCnLEEtBAuTqfdc = -30678; 
ulong ALg = 20203383346570819; 
double EqJNNj = 4.406089E+24; 
if(EqJNNj == 2.970032E-35){
EqJNNj = Math.Ceiling(Math.Cos(2));
try{
Console.WriteLine(EqJNNj.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}sbyte eAELlpGhxEgnYXAhaBup =  -7; 
ushort aIRcLLQFwJ = 45412; 
short HcyJHHxgpuASU = -12020; 
ushort nQiJkEcunVyHYMGnEeCYcMKSahKm = 21671; 
byte wuHgTCKicZduLjQhFPlnPPzVD =  33; 
short dTPdeebmsGMNTVzYZsYJ = -19820; 

}public void fzfWbGVMFMkcfjjZpGhEQYcAuTqAV(){
byte IzdOqXh =  16; 
ushort GaaCRPnPZbs = 34139; 
int gwHCiox = 386305323; 
if(gwHCiox == 578011){
gwHCiox = 50238;
}ushort bhDLlkYcXDdcKViQgRUHWqHUhV = 51267; 
int XnNSWYUQQwYisJ = 103096382; 
while(XnNSWYUQQwYisJ == 103096382){
XnNSWYUQQwYisJ = XnNSWYUQQwYisJ + 220691;
}ulong NiPWLCIEqxqGZo = 85314639588482651; 
double EIIPViTwJBBFcam = -1.787193E-27; 
if(EIIPViTwJBBFcam == -14059.5){
EIIPViTwJBBFcam = Math.Ceiling(Math.Tanh(0.1));
 bool? RdssGRZXaTjUzUujqLIkAoFVZVPj = new bool?();
RdssGRZXaTjUzUujqLIkAoFVZVPj = true;
}short cQcXYZGyWskyZghExQXsz = 14769; 
sbyte JAgQFwDCCbA =  -63; 
float QZbZgNuXxYqSfJAalpLMoASQTCP = 3.741781E+30F; 
double ULCaYpzFsWLchAMoUbCQ = -2714.25; 
if(ULCaYpzFsWLchAMoUbCQ == 9.055705E-16){
ULCaYpzFsWLchAMoUbCQ = Math.Pow(5, 2);
try{
int uhzsEULSeqaulJDRcELlB = 9106689;
if(uhzsEULSeqaulJDRcELlB == 90177){
uhzsEULSeqaulJDRcELlB++;
}else{
uhzsEULSeqaulJDRcELlB--;
Console.Write(uhzsEULSeqaulJDRcELlB.ToString());
}
}catch(Exception ex){

}
}float DCJhEfbimS = 5.253211E-06F; 
byte IsjZkZZabnRj =  155; 
ulong OhXCGxRtV = 27904686234648605; 
ushort cGtVK = 58663; 
short AupuAnXzKUUFURZoU = 30171; 
sbyte JjiEN =  3; 
long ulmIpUqYFLNDYThlsSj = 84060384302697242; 
short MHZEqdzHm = -14786; 
string aLikGfQbXlVpzZpk = "VfZbMSdOYHPJFeEiM"; 
int SCfeenoBMJfsO = 1079; 
while(SCfeenoBMJfsO == 1079){
SCfeenoBMJfsO += 452772;
}short fyIAatsgqgMtVsZPaVOyqsfSoa = -6152; 
uint whoqCNdzGdVWtuCtYSVfEhf = 82074263; 
uint sPjaXTuWZycuGVpwSTCXXEA = 1608; 
uint jmcFBaZMKwcXc = 456371624; 
uint WzpjmMmRoEQOpTSzAkKWSCuGjHa = 91401421; 
uint AbEwKiiWBPOlxjXtDkADPOGdudKaG = 46181712; 
double FpMMzhopnigdpXxlmyFxWjMx = -1.861346E-39; 
while(FpMMzhopnigdpXxlmyFxWjMx == -7.35243E-27){
double wRUlIddjlBFzZdgnOsoUzUwEpm = -3.987736E+20;
FpMMzhopnigdpXxlmyFxWjMx = Math.Ceiling(wRUlIddjlBFzZdgnOsoUzUwEpm);
object WuQkUfCa;
WuQkUfCa = -6.127335;
Console.WriteLine(WuQkUfCa.ToString().ToLower());
}int eEkJPiaMNMVtl = 7340; 
while(eEkJPiaMNMVtl == 7340){
eEkJPiaMNMVtl = 514184;
}string MXVsRtJlflNYSFeWnLO = "zImFehpKTDHfCPtH"; 
short YQqBIBhwtHH = 1338; 
string NCgd = "pnJdZtEFIxdkzHXMXhYaMy"; 
long KdsyFsZJoVZcoNsheWBYpwhU = 87919419135493811; 
int tWbfZoywVEIVykQRWQWLznVyqnE = 51309499; 
while(tWbfZoywVEIVykQRWQWLznVyqnE == 51309499){
tWbfZoywVEIVykQRWQWLznVyqnE = tWbfZoywVEIVykQRWQWLznVyqnE + 455637;
}ushort xfYWlBUAFmkT = 37972; 

}public void qgiWUwebEPMMRUKGCy(){
string tLYTLjEOpkfuZbjfOjEZnwDjj = "KaQZLORMkGiokKRq"; 
long JsNQLERsUcEX = 65398146215808560; 
uint DmQEWAYThXKi = 853857; 
ulong olhtUPhmxuGZjnsuTqPAZgIsta = 60387522662591408; 
short igyOeEgCTYKUFopjcnUiFP = 31880; 
float CZxhPCDZGlMTbuxZfzwpabYKdO = -1.105885E+19F; 
sbyte thETEEK =  -28; 
string gVYdEpbTyjAqFET = "aNebqQTzSFphQSo"; 
long XifdVfNMqoWJc = 50315401712440168; 
sbyte bBlZaTcGUOXH =  -24; 
ulong fIqoaBXQPaphLo = 88553630308590521; 
sbyte TnKbUfWFlJypZOfRdLxYLLq =  -46; 
int HPEKTxCFwMRVV = 7995189; 
if(HPEKTxCFwMRVV == 42298){
HPEKTxCFwMRVV = HPEKTxCFwMRVV + 825970;
}string CMaaSlhchWtzBz = "aDQaPwt"; 
ushort iPM = 58714; 
double oACYecEKbjuDmPyxwlWLtlQm = -4.048118E-25; 
while(oACYecEKbjuDmPyxwlWLtlQm == -7.662374E+25){
double tHTXDt = -1.778007;
oACYecEKbjuDmPyxwlWLtlQm = Math.Round(tHTXDt ,1,MidpointRounding.ToEven);
try{
int QwTfFDMtBYnxcKnXVLESYKgA = 8787780;
if(QwTfFDMtBYnxcKnXVLESYKgA == 2607){
QwTfFDMtBYnxcKnXVLESYKgA++;
}else{
QwTfFDMtBYnxcKnXVLESYKgA--;
}
}catch(Exception ex){

}
}ulong ntkxYm = 86066570118831792; 
ulong XfMoLTLXFuPnuxuzITFOSqATi = 32771399703829183; 
int OoBwuJmSEylnMG = 956468; 
if(OoBwuJmSEylnMG == 29842){
OoBwuJmSEylnMG = OoBwuJmSEylnMG + 986148;
}int TgtNWYGVCAPZGigyjfG = 916079; 
while(TgtNWYGVCAPZGigyjfG == 916079){
TgtNWYGVCAPZGigyjfG = TgtNWYGVCAPZGigyjfG + 402317;
}sbyte aIwNHQhxubINolIV =  63; 
uint apRtJWuhLykpsOzFngMhiz = 75222476; 
ushort spmnBiFJhjjnNRmJx = 63738; 
sbyte pYQIuPaqECEdXgEdUyXcMJ =  30; 
short auVVAbOFKkuBG = 25157; 
long mfHHgpG = 41585395913069939; 
short mtufxdAPAmWiDoDlU = -2590; 
int InK = 64959367; 
if(InK == 34764){
InK = InK + 749507;
}uint EcYsBqcRcgRHcJdHAxPlD = 34; 
float JGyKszcbEtgFWcURHBTRIIfEflFXl = -0.02184897F; 
long uRLFh = 56404719589294651; 
int VwWEzcqbBUFgT = 79; 
while(VwWEzcqbBUFgT == 79){
VwWEzcqbBUFgT = VwWEzcqbBUFgT + 490294;
}uint ilzRkbbi = 62403690; 
string kcQlVlbdPkLDEPKQdxBD = "iMi"; 
ushort mboTMnCzIbZjYjyzxYCVmxq = 14837; 

}public void sfBHc(){
short PhZaPWAybegIRUzpeDNN = 8705; 
uint RKdOthaRGLqwQkyK = 475032; 
string ioKwkUPxdoEamJhqMhfobmBXAz = "fITmoDIqCaIEAsENpgph"; 
sbyte fXst =  75; 
float OMWMlKOjcozFqHqdFGWyAkQiqBc = 1.111871E+31F; 
float NQxOnMRnJQEnSSDQxPUNNTuwEWN = -1.266078E+16F; 
long nhtMPVScq = 21496192309069536; 
ulong GNbiiUxajsZwDaEogAgOU = 17372736440229504; 
int xIbcAmYjRtUILssPneIQzkaQbazjO = 6845; 
if(xIbcAmYjRtUILssPneIQzkaQbazjO == 381204){
xIbcAmYjRtUILssPneIQzkaQbazjO = 547947;
}ulong YdbhoouaWakHGqIQR = 11867530806075848; 
ushort KseqQFnoaHIjVGQeZ = 16776; 
int YbNMUpg = 9398; 
if(YbNMUpg == 350013){
YbNMUpg += 869368;
}ulong IRKwbqbIjippbwWAClmEIG = 4649516312848750; 
short iUhmpSBRWuD = 4211; 
int fUa = 71; 
while(fUa == 71){
fUa = 61882;
}string mWFtJkQTZggnJjJL = "HncPSAsdsqIjLa"; 
float OcWWy = -2.436758E-26F; 
ulong GTYGqVcujCPaKwxmpqUiJ = 39020885940314218; 
double epelGtyawGQB = -4.17177E-37; 
if(epelGtyawGQB == -4.594654E+11){
epelGtyawGQB = Math.Floor(-3.465647E+12);
try{
Console.WriteLine(epelGtyawGQB.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}string wNUYDnXjRMTlIAtW = "qZGoBQbLTsQoLYppLTDywW"; 
byte HzUWQCcKzEnpxjVWSnJVhU =  32; 
ulong VXHQzqwPUJbVfUsofQ = 80466317185661451; 
sbyte oEMshtUEx =  96; 
double TCCCTbNtq = 3.678267E-37; 
if(TCCCTbNtq == -1.918284E-34){
TCCCTbNtq = Math.Ceiling(Math.Cosh(5));
try{
int CIsePSgMJgbLckFjnatDN = 1440277;
if(CIsePSgMJgbLckFjnatDN == 38033){
CIsePSgMJgbLckFjnatDN++;
}else{
CIsePSgMJgbLckFjnatDN--;
Console.Write(CIsePSgMJgbLckFjnatDN.ToString());
}
}catch(Exception ex){

}
}float StuWMulTJiJ = -2.898192E-34F; 
sbyte BlxqSFdlVGmEnfMCeM =  -50; 
sbyte RUcTf =  79; 
short XaAEnHiLccLPhzpAQXOXHFuNXBJsu = -3501; 
long PmXGOaazDRbKFtWtFDdicqst = 7915359966461249; 
short ZUBKMQXouQELiEEpNRuEMyVFZbul = 1764; 
long wtItcSURmkOXCPVE = 87028570615221355; 
int VKLxLNJphqCjCySagtYBtQeyqcTIZ = 61; 
while(VKLxLNJphqCjCySagtYBtQeyqcTIZ == 61){
VKLxLNJphqCjCySagtYBtQeyqcTIZ = VKLxLNJphqCjCySagtYBtQeyqcTIZ + 948854;
}byte RAanlH =  145; 
string JkkAeumbGfGmeJUapYLesyAJTp = "XlhiBwztJxAfljPzSFTMQgmVJS"; 
double lLDHOBoPzWeSGokF = 3.174213E+27; 
while(lLDHOBoPzWeSGokF == 52317.98){
lLDHOBoPzWeSGokF = Math.Pow(double.NegativeInfinity, 2);
Console.ReadLine();
}
}
} public class MFYyCVphKoubVUwmgmxjOBdIztJYh{public void FctmqUINVD(){
uint MgaHl = 2962; 
long BEtllCqZimsZxL = 20746188907291986; 
float JOWHShpJySdAaafsqNNph = -3.646979E-16F; 
ushort YDQOeciKKDyFKhaXJCEHS = 42117; 
long inGFECFABXTGBU = 72381641464630822; 
long MtzDMPRqUsGQxFfhYYd = 19730925784953073; 
string kOuHaSkcbNL = "CLhoEu"; 
short hBLLVqqVLuYdPaKFkjwNV = -25696; 
double sGNKozFt = -3.277337E-10; 
if(sGNKozFt == 2.463788E-33){
sGNKozFt = Math.Ceiling(Math.Tanh(0.1));
int? lPGJwRZBBzMlGNnyLnkI = 2422531;
lPGJwRZBBzMlGNnyLnkI += 33729;
}sbyte pbIatYUREl =  79; 
ulong SVibHqWHwoMsCGKQohByRR = 44226316011490974; 
double hZjgwUG = 4.230276E-22; 
while(hZjgwUG == 1.441193E+19){
double jaoepqTbszHUxh = 1.488244E+13;
hZjgwUG = jaoepqTbszHUxh * 2;
try{

}catch(Exception ex){

}
}byte EdpOutzpYIGTDPeKeDLUZU =  141; 
string BKtwl = "AhQWiTiwwnGEXJgodMxdllt"; 
ulong BmJTKfUMTfMNSWQWgoHuq = 62541001648432933; 
int OcmIHADMh = 82402262; 
while(OcmIHADMh == 82402262){
OcmIHADMh = 550126;
}byte HgeWNtsVSKIdzVEpTXKFnixF =  145; 
uint BCREJuRsczqbKVMPQUIKP = 7651; 
long THCyKBtnKSuagZJ = 85066725098444674; 
int HKVPTmNhfQOoV = 9620; 
if(HKVPTmNhfQOoV == 994328){
HKVPTmNhfQOoV = HKVPTmNhfQOoV + 53854;
}short JVZbmMVhCTWZccQyARGHTbi = 26716; 
short iUFiaIkHXPWNHBSZEXRo = 22812; 
short BniyKVI = 14674; 
string FEbEtSoPxfaCMjePTzldd = "TMdURjBMpdSdcWRxjeDBwjhZ"; 
uint tVbjAaGPLoAAbfnLPXJIhhYxnhCDu = 58419532; 
sbyte TRGKgESzmlCX =  51; 
string mLwkpURkWGVsAcu = "Luh"; 
sbyte JbItRjUVxA =  17; 
string pJntP = "cHJelltwTkRDEVi"; 
int NSzLWoBfqnwNMOqGKfIZW = 129681; 
while(NSzLWoBfqnwNMOqGKfIZW == 129681){
NSzLWoBfqnwNMOqGKfIZW = NSzLWoBfqnwNMOqGKfIZW + 713962;
}int aiUMZBCpMfqiYUXXJgxBWTPCFNpL = 93; 
if(aiUMZBCpMfqiYUXXJgxBWTPCFNpL == 897706){
aiUMZBCpMfqiYUXXJgxBWTPCFNpL = 172369;
}short BaMmPp = 29810; 
string QYDiBfUoeag = "tgljQoPlNnPES"; 
ushort wiLHVPkOPODARZoPijMg = 57789; 
uint ZahO = 544071; 

}public void ZqdyAfu(){
ushort FityxnQXl = 30168; 
uint tfpOUfLGNaIlGPkxtAjw = 96639088; 
sbyte BdHMkkCfdDGlOKGZJPzECgSIcuFMn =  -110; 
float XsmRAZuDzChdi = 1.091962E+12F; 
long jMZAExtlZJRGQRZJknxyPBKuVfZ = 27647361326645235; 
short VuVTFAqEWJLiuYMllfJcFFzywy = 667; 
int LXNIP = 8822; 
if(LXNIP == 187462){
LXNIP = 15589;
}short YxoQuBqAJMwpopVyzhZLzkLXB = 5588; 
long LIWJKOGchFcaoGckqsBZYdfbb = 16366442218229095; 
uint aQNGBLSoUeZwSiBHidKTIDUH = 42; 
float RomhRRbwFMan = -4.498665E+20F; 
byte clAFkLgNiFgzWfEdTBzFSRFbgfbx =  13; 
ushort TpWFRWmgfhBZlLEzziXwFoH = 6397; 
byte zzfJLKNSmiaCMhGusysR =  92; 
short HDmfahEKRujyPbxyQm = -7238; 
ushort zXxQPkXjNtnhV = 31045; 
sbyte ENKJAihDKRtVZCF =  119; 
float iFQpkXH = -4.376377E-34F; 
string pTDpamBLOVpQUfYSbjlPpoMpCHn = "McZdgHtXRGZodnemT"; 
ulong jlfFoSkGbhmOtZYYMWlKmNaaxkXL = 85103632476942722; 
int KlVasQERGpfotqI = 66; 
while(KlVasQERGpfotqI == 66){
KlVasQERGpfotqI += 156936;
}string BFSIQbJVJqnlZOmukyGFIGMQhPI = "HeD"; 
short pwxBhOPXuxYeeIqxcaI = 2972; 
uint USbAatHoF = 97425666; 
int sQUcJQHmjOLdhpGEdVNgd = 588910; 
while(sQUcJQHmjOLdhpGEdVNgd == 588910){
sQUcJQHmjOLdhpGEdVNgd = 421347;
}short RigAY = 31587; 
int XYMSaLd = 4342; 
if(XYMSaLd == 19066){
XYMSaLd += 727581;
}uint lAGRkuuVuphsFFEiqZufWbndQB = 23; 
float ptpoVZshwPEd = -336.2399F; 
float AMSKLbswYjkD = -8.655948E+15F; 
byte bbaPbpK =  148; 
float FGDTKMky = -0.0001147299F; 
ushort InbVqLOP = 8614; 
long wbpCcpibnSCyMTeBA = 7634582636254853; 
ulong ZZQbcU = 43178193338957296; 

}public void esfRobljoyEbOG(){
byte PpijczRzDKLngnixRtLznVzfNPB =  147; 
uint oDGywSXIsHCHnGpXcangkuUNIU = 76301461; 
ushort XmZKwiJJSq = 31314; 
sbyte eXH =  33; 
float QMApsJUITPCaTkeJoIEjhoui = 1.002381E+12F; 
string QPamZoHCzWHbC = "QTppFXWxJZcIHBsqs"; 
long BRdPobFEDksaDJHzBSI = 52012490311143836; 
long igFWwa = 55515988264248008; 
byte YBZacSAKICd =  114; 
double eLWyudNehIWMwQucMdiPcKs = 1.677835E-33; 
if(eLWyudNehIWMwQucMdiPcKs == 7.068293E+29){
eLWyudNehIWMwQucMdiPcKs = Math.Sqrt(4);

}sbyte bDOTYFJdYYH =  36; 
uint PCWLLefG = 3549; 
ushort AIaVmWDesNGjzngkMMpwsHin = 10101; 
short dlwiwAifbcdLLfyS = -30094; 
double lyYUTIcYySzMNWxzTXEjKoi = -6.520308E-28; 
double swecfqCC = -3.903682E+30;
lyYUTIcYySzMNWxzTXEjKoi = swecfqCC / 3;
 bool? EqpPnHFUzMc = new bool?();
EqpPnHFUzMc = true;long shOlEqUXRjklWYXUmcz = 32312470588455285; 
ushort ZkhIP = 65184; 
long hYehTsuLoUwnyxnxmKA = 61974349448028357; 
double FhYm = -6.723667E-31; 
if(FhYm == -1.606468E-18){
double ccxgIDWMweBWjmsFJaBwH = -28.40661;
ccxgIDWMweBWjmsFJaBwH = Math.Sqrt(3);
FhYm = ccxgIDWMweBWjmsFJaBwH;
Console.ReadKey();
}int qqZ = 48; 
while(qqZ == 48){
qqZ = qqZ + 440986;
}uint hyuNCpQkuoDJZJdzA = 3608; 
int kMBmelMcpIMFVjIzYmplQjMPIayw = 24157738; 
while(kMBmelMcpIMFVjIzYmplQjMPIayw == 24157738){
kMBmelMcpIMFVjIzYmplQjMPIayw += 604043;
}double UByddheHQedzwZBZcshaaL = 1.309108E-33; 
UByddheHQedzwZBZcshaaL = Math.Ceiling(Math.Tan(1));
Console.Write(UByddheHQedzwZBZcshaaL.ToString());double PxS = -0.275019; 
while(PxS == -8.113087E+13){
double UlZzNTPLQlVTZcPhJEwQTqyZLkyX = 1736.155;
PxS = Math.Round(UlZzNTPLQlVTZcPhJEwQTqyZLkyX ,1,MidpointRounding.ToEven);

}sbyte WNiBNzddnGgUzMYZpktb =  -97; 
uint pAtxXUL = 779853570; 
ushort czngyDYdgyxUCbExCdbgCuFSau = 2286; 
sbyte EHsBGJA =  56; 
sbyte RdduLUGRwuziVNmqIUxgdBS =  95; 
int etWsdLspkdzaUYwDHNMuMGnnb = 30205566; 
while(etWsdLspkdzaUYwDHNMuMGnnb == 30205566){
etWsdLspkdzaUYwDHNMuMGnnb = etWsdLspkdzaUYwDHNMuMGnnb + 217835;
}byte UPlVdpNQFCiTIZyLCUehieAfI =  81; 
uint pHUSojZiiHDxYF = 7; 
string tGSqhwgbIPpfm = "SsBXHqFzaoDUqJjFXwbmXSD"; 
double zMmCjSduQEHejxNQ = 2.158682E+08; 
while(zMmCjSduQEHejxNQ == 3.148359E-20){
double RVeZPa = -0.0008246732;
zMmCjSduQEHejxNQ = RVeZPa * 2;
Console.ReadLine();
}byte wnwysXZua =  143; 

}public void EMniAYQhpMDtoEdWQDNPPqkNp(){
double yzAgCCLWcLfXexST = 1.396862E+26; 
yzAgCCLWcLfXexST = Math.Floor(-1.254381E-36);
Console.ReadKey();short fWWAcykcnNuPcsnqUHSdJpC = 27926; 
long QIMTXEEeXGqPObaDh = 79239358401450167; 
int ndPwZOAIEquQRkYuIafS = 47282541; 
while(ndPwZOAIEquQRkYuIafS == 47282541){
ndPwZOAIEquQRkYuIafS += 536254;
}long KhjEsWpaijqaotylNEEQGLRDkNQkX = 37204298513458445; 
float UVXkyJeEyGVwFYV = -2.703301E+36F; 
short tFfedDWTXOxaeWfJUIBu = 24515; 
sbyte tGckJAi =  -94; 
double AzSPQU = 19.42757; 
double zoaMzWOlwMAasnuPH = -8027756;
AzSPQU = zoaMzWOlwMAasnuPH * 2;
for( ; ;) {
Console.WriteLine(-8.274E+29);
}byte VYGdKmbuzPHdjH =  56; 
double TlQRPTDGJHlPsZKYPmyTfb = 9.095001E-07; 
while(TlQRPTDGJHlPsZKYPmyTfb == 8.600751E-33){
double WFYtPwFgeczaGPGgQXfwtOe = 6.632274E-26;
TlQRPTDGJHlPsZKYPmyTfb = Math.Round(WFYtPwFgeczaGPGgQXfwtOe);
try{
Console.WriteLine(TlQRPTDGJHlPsZKYPmyTfb.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}string siBtCCbwjohbDKAxNjcEkmwnX = "UNgkCjtIDhJafsBPWeJGzJhHY"; 
string AGfOmwLInnZNYJVgsNMe = "cwppFqslGBlyCTbytwFuL"; 
byte WqKXDCQTlZYxkPyIe =  91; 
ushort ONpyQUKYqgKZRTuZVuPKQ = 41413; 
double ExQJWNBboENibmh = 1.034365E+26; 
ExQJWNBboENibmh = Math.Ceiling(Math.Tan(1));
for( ; ;) {
Console.WriteLine(-2.868189E-36);
}short FODEQihEBPQpzCbhh = -5591; 
uint yocZWtbJPEC = 67309; 
byte cmt =  170; 
uint PKwKOZ = 858778064; 
ulong zmYeoLEIygAPWmtmmBYVONakJFI = 17115171487071943; 
byte gLsJM =  56; 
short aNhpCMuUSfLmOxLdoAd = -32450; 
long mjjhgPHmbCJFISAZHINVjEt = 49752616456727356; 
short mAYfuGFmFejEiOSFoTxnOEKRmQy = 18638; 
short PtqJwsBTlbhJFzitiVhKeuMdhJ = 20074; 
sbyte EiQUMpQmBtBdxHuffYkITICHJO =  -28; 
long BMDsZITsO = 55748089674616913; 
long qYcLFBaWHikYVhZZHYMxYCN = 38648897539505726; 
string Hqozb = "SZMliEUnkgJVJUlPPkLFElmXIjgS"; 
short LBGbUympFOkOdHpepjTRCdOxu = 7182; 
ulong FYxbZuwioIZPDAGXtPiGIDU = 17932066777246899; 
short zMGqiPJkgPIjdocyQdFyff = -31265; 
sbyte cDfPwfaMedNaGjaA =  -26; 
uint lNVhliWfcRFtLBSglExLKIz = 228709785; 

}public void yjTEXLGnTM(){
sbyte TGAZYUBLcAOdO =  105; 
double dEnVWWMMmzTTNyIfZSKRlKKF = -1.789196; 
if(dEnVWWMMmzTTNyIfZSKRlKKF == 2.795361E-05){
dEnVWWMMmzTTNyIfZSKRlKKF = Math.Pow(2, 2.1);
Console.ReadKey();
}short ykdyKgDQeIBMQaCLy = 30139; 
float pBQcIutsFmHQuptuitKImmhzWEaj = 3.326029E-18F; 
double fumXubQpNkkRbLqAH = -5820.684; 
if(fumXubQpNkkRbLqAH == -1.277965E-20){
fumXubQpNkkRbLqAH = Math.Ceiling(Math.Acos(0));
Console.ReadKey();
}float pSSXlRypkhSAkUesJpouBbkDg = -3.809392E+31F; 
string ooXfjbEtkiLxbySixJWPoTomyqnI = "RkVuuWSVub"; 
float ZzhEXzFsuLtZnIxZ = 3.617311E+12F; 
float WduYPbICZmbHcopuhJASJoU = -2.897847E+25F; 
double MATkLi = -824395.3; 
double sgJNWZ = 2.304922E+31;
MATkLi = sgJNWZ * 2;
try{
int iaeHim = 9523365;
if(iaeHim == 70625){
iaeHim++;
}else{
iaeHim--;
}
}catch(Exception ex){

}ushort YUEbFWOzeag = 25907; 
ushort iDi = 1325; 
double RcVZVcldspYoXNThFYALwP = -8.970967E+27; 
double WOJzWYFgDLK = -1.922377E+30;
RcVZVcldspYoXNThFYALwP = WOJzWYFgDLK / 3;
sbyte jQWqFmFsMlopFf =  -56; 
ulong IZWSboIeStkhBSFVyQeUNQnNdjKe = 36750147138642310; 
byte iBomNTtkltLuhpjSjS =  181; 
sbyte RGDIqTZsKAlJTHEFBcuuSYpu =  -117; 
short bWHBIcP = -10678; 
double FxZtTCTzLnJGmJPpPWSoiG = 2.146694E+15; 
FxZtTCTzLnJGmJPpPWSoiG = Math.Ceiling(Math.Tan(1));
Console.ReadLine();byte UjDMeomtkQ =  9; 
short ieFCYDalTgyojCwzVdOoLDfTLtG = 8584; 
long EWeSaQjwypmbadfyVNSUpMnbkHZMQ = 24077564114050578; 
short FoAHybJCbOnzSPRKR = 9940; 
string uRyGSuTCgBEmElmGQeRZ = "eYYDgeGKkMThLgORU"; 
ulong ihwB = 44987750042690149; 
float eoSA = -4.982034E+17F; 
short RVPLfSeAPW = -12245; 
ulong uJyPBkzdzsacdHSVqQcOVpAMA = 37256702961732062; 
byte HwLDBRjtEPIblmEygmaWdi =  185; 
short hXbbwFZRVkBHmDWLXW = 29050; 
float SnzZQhcqHyJwYtUiqhkJkpbdQTL = -3.550677E-22F; 
sbyte PzKpJDcGx =  110; 
uint ORloibCHON = 68; 
uint wiWyYfBkU = 281483159; 
string HnCOMLcmm = "FCcKAzcoZPxhUKYYuN"; 

}
} public class WoagPVsmYyZAZiEsVW{public void aMSIonQztSPOpo(){
long pEwmOhldSao = 43954116204670392; 
float HMSZx = 1.432483E-08F; 
sbyte KjoSnOAJITBPQnYtAcBMM =  -46; 
int lJSEmqiRVhqfhgcDFtqSqoRyTH = 923199971; 
if(lJSEmqiRVhqfhgcDFtqSqoRyTH == 962145){
lJSEmqiRVhqfhgcDFtqSqoRyTH = 361797;
}int HFVWwpLJTIXTks = 94; 
while(HFVWwpLJTIXTks == 94){
HFVWwpLJTIXTks = 702295;
}uint SwLHulXyjYzLByIIXbgIwiBZwFVW = 6084; 
ushort VKNBlPVAMxzZwBRDpAuXETBJVLkP = 20873; 
string FFCPRLg = "igIWZLCEVNtGQk"; 
ushort bidGBEgiKnTu = 57550; 
long Rjc = 32491389830565197; 
short Xwq = -8537; 
sbyte OJHCVGa =  -3; 
byte RgLjNCxOZFyeIflumeknXB =  59; 
long skJgGRwMQgmxgowZEtELKaBoCtVn = 64778171814053258; 
ulong uOcsRYYXgQQougJamqKt = 86155849657455366; 
ushort UelWYPJqjDqFmfzJujlYZdHubh = 56316; 
float PiGBmWLcyhUsWghcaZ = 2.648293E+12F; 
sbyte HgzUQyRUQWNNwtLdZjOG =  108; 
string HdWAsaicmygD = "ctfnlDqXFiMOsXa"; 
int BMFQRRUXzUITBpoRlbHeAttKucU = 71; 
while(BMFQRRUXzUITBpoRlbHeAttKucU == 71){
BMFQRRUXzUITBpoRlbHeAttKucU += 447525;
}short uXJkDlpNJKedTyXfVtCwhtVwPWKl = -2656; 
ulong xitUUDQeSjGKapYb = 86196856883288648; 
string mxG = "doTjIVAEcDeoJlCJV"; 
sbyte UXyDILBmFTSouzpfpRQIdDjMs =  -111; 
uint LmtghFMFH = 23073602; 
uint IVLjmLR = 1235; 
byte ezNsOMkEknDqBMhPNLAEFqAwI =  122; 
uint tJUEb = 31964; 
long tRUCInUfEkzeVeIIImNYsLojg = 12260736004332468; 
double UFcWdGodndZyF = -2.637453E+33; 
while(UFcWdGodndZyF == -3.365404E-18){
UFcWdGodndZyF = Math.Ceiling(Math.Tanh(0.1));

}long ugsAsRAZxhYRpnWRWROmeI = 34015510415945718; 
short PILZzEXDbUcN = 20540; 
sbyte cAjGLByHNa =  -76; 
long uxoJQGcJEWttUkkk = 84251234621286116; 
sbyte SqaXZmUyiFGuXkcFADR =  -63; 

}public void olxhGUZWZUPBiNLIGWfzxcHFj(){
ushort IYVZJV = 52133; 
int FPutU = 169641; 
if(FPutU == 888967){
FPutU = FPutU + 769711;
}uint XTHMOwlGRYi = 34936148; 
double UgGtj = 3.862945E-34; 
UgGtj = Math.Truncate(UgGtj);
Console.WriteLine(UgGtj.ToString());float RKiLBBHboAQwtEDcMVhiDCYHkLwOc = -3.055269E-17F; 
int VpHQH = 542384; 
if(VpHQH == 839844){
VpHQH = VpHQH + 583545;
}byte bcGXEaPLwoLDBQeSZdkmoznWlf =  78; 
int PKuxqKtieJnOuAa = 266235083; 
while(PKuxqKtieJnOuAa == 266235083){
PKuxqKtieJnOuAa = PKuxqKtieJnOuAa + 853626;
}ulong tXFDOJLyLjmHFpgifMJVwWA = 4605233565026645; 
int Vbz = 12990179; 
if(Vbz == 383640){
Vbz = 209629;
}int hTwFhXJMiTtxyKFCIM = 188551; 
while(hTwFhXJMiTtxyKFCIM == 188551){
hTwFhXJMiTtxyKFCIM = 675697;
}short xZgiBjsWNfSxRcRxxhDF = -31569; 
string dTINuCBmMoACKFasj = "CWzJk"; 
float sGkzgLHsydpTyY = -6.884717E-22F; 
float gqChS = -1.017359E+23F; 
short AgduIMqbWpPGFTim = 1147; 
long NfgLGEdEUkOPIOk = 63161664717500075; 
uint dADzsVqLqGutwzQEiyRhCgRtMWT = 63125527; 
short jIPCeCfCoxeqhROQUTeY = 9770; 
sbyte uXIiNwbAHZY =  -80; 
long WMbcWCLxQKDXgYYZtZe = 19747888102612286; 
double qiIcjXgcVMDGXgY = 1.237827E+21; 
if(qiIcjXgcVMDGXgY == -14.42485){
double ulsoKKQ = 3.37123E+08;
ulsoKKQ = Math.Sqrt(3);
qiIcjXgcVMDGXgY = ulsoKKQ;
int[] BVVWaRWznCMyyyME = { 7175062 , 55808 } ;
Random kxPLjDIYGSskVoRYJzMTsbKLh= new Random();
Console.WriteLine(BVVWaRWznCMyyyME[kxPLjDIYGSskVoRYJzMTsbKLh.Next(0,2)]);
}long VSXfZcqjOpsmYwZEmsmUAPZ = 55019092287374190; 
double jKZueWlAOUhtGuGKgUtpOogSYwCTF = -4.95348E-24; 
double ixawMURHe = 3.328225E+17;
ixawMURHe = Math.Sqrt(3);
jKZueWlAOUhtGuGKgUtpOogSYwCTF = ixawMURHe;
int eiAHNzKtYXhuTqpf = 37; 
if(eiAHNzKtYXhuTqpf == 389569){
eiAHNzKtYXhuTqpf += 444988;
}byte poLLRkmVJTAdCLDZFhk =  142; 
int iETNIffDfIZ = 332312696; 
while(iETNIffDfIZ == 332312696){
iETNIffDfIZ = iETNIffDfIZ + 537632;
}ulong lLaSlEwQXBmlHcZghReEgQ = 7764555323213483; 
int zSqFiLIimoibTwsSsqH = 45; 
while(zSqFiLIimoibTwsSsqH == 45){
zSqFiLIimoibTwsSsqH = 596417;
}byte hWb =  183; 
ulong HOPLuXxjVqSIeUxMPbOExhnnVaW = 38419302965621351; 
long gXukZCSLOBsFxhaXhz = 62939986471288066; 
long olXIOh = 24670088899741159; 
sbyte EfdekGnIOidqEtzo =  88; 
ulong DEKQnAXEPbSdXJ = 30873780929306527; 

}public void UGeqAaVhOKg(){
int UoPFmbAHVefIJXSJmQNFnPkjhyE = 874647004; 
while(UoPFmbAHVefIJXSJmQNFnPkjhyE == 874647004){
UoPFmbAHVefIJXSJmQNFnPkjhyE = 77862;
}double qRZIHzJYBZJRddACT = -3.229128E-39; 
while(qRZIHzJYBZJRddACT == -1.628417E-31){
double GNSLnFKuCRmhggFQ = 1.275748E-05;
qRZIHzJYBZJRddACT = GNSLnFKuCRmhggFQ / 3;
Console.ReadLine();
}ushort iXQtokQBuWoFKpWnJKUAuBSnLoQl = 12960; 
byte QRGJlcER =  152; 
float NMmdEwpGwFCVDJeCIa = -202993.6F; 
short MAOeHL = -18220; 
sbyte oFQOfbILuWFSGnVcahBVRjqnG =  126; 
ulong zOGuoMmqNUY = 5359678732946854; 
byte cGDLjbemChfnqmHqkDUbEpfia =  232; 
uint KEqTEyxh = 67; 
sbyte iekNTmJctqBNminNBuKnB =  100; 
int RmLTRznCxjTAetpOmzQscFHujJIIZ = 192745367; 
while(RmLTRznCxjTAetpOmzQscFHujJIIZ == 192745367){
RmLTRznCxjTAetpOmzQscFHujJIIZ = 915845;
}int PlVcYpdpOIgeWbcek = 247812616; 
if(PlVcYpdpOIgeWbcek == 688449){
PlVcYpdpOIgeWbcek += 141820;
}float CGZGZSyJlylVpdnEdwb = 14988.09F; 
string hjoqJalfedxyLdLPPKwKERiV = "YclnfpczQSwKCPJZsgRTHEAtG"; 
byte itlpbuAKYZRZOuYUlofnVMLlL =  83; 
string wDTITzTe = "CuobeKaCgdAIJneBYW"; 
ushort ZGKsooxgifoBTpLUQlQc = 45831; 
long mCDMoPhVDyxFVLWNPn = 55595715741533112; 
byte MfuYocxRNEhqbjMcyMPjT =  125; 
string QqCzDNtMFyixdzoTnfZlgDq = "cWsHPZRmfGhXBXoZjxNEbpZYx"; 
float NAPInZLxGwNXFGWqyqqtpRacLQN = 7.120325E+17F; 
double jTddNpYTXQBK = 1.779303E-05; 
while(jTddNpYTXQBK == -9.549374E+35){
jTddNpYTXQBK = Math.Ceiling(Math.Sin(2));
object xGlJPLOw;
xGlJPLOw = -32882.93;
Console.WriteLine(xGlJPLOw.ToString().ToLower());
}ulong gMQy = 41416806556622873; 
double zcIIYfVKQcsQQt = 1.049361E-11; 
while(zcIIYfVKQcsQQt == 8.732588E-12){
double nMTPjzKDcZCEhdpRJhx = -0.0003013178;
zcIIYfVKQcsQQt = Math.Floor(nMTPjzKDcZCEhdpRJhx);

}string QhsNXtwqhbmDu = "JuMtxkPoQDScyNoFsLXQ"; 
long EstsRPqmF = 28984754150863062; 
ulong BqQUUVIbDKDFEZxQuZ = 18648646326005666; 
ulong HCbfhk = 84305335360813295; 
ulong cgSLjSKqCxEEOSNohC = 80585514457858370; 
string BHjniZWaNcVORLELIFgVLjpIqNV = "JATJQOQUmouPVNCeGIk"; 
double NLlwgkqpCWxeqsmimYdUKzsksa = 2.529781E+11; 
if(NLlwgkqpCWxeqsmimYdUKzsksa == -2.596807E+14){
double PHmJDzELsXKFwoVsKgdjosbdgmKf = Math.Log(1000, 10);
NLlwgkqpCWxeqsmimYdUKzsksa = PHmJDzELsXKFwoVsKgdjosbdgmKf;
try{
int FwzDZoBx = 3016845;
if(FwzDZoBx == 68063){
FwzDZoBx++;
}else{
FwzDZoBx--;
Console.Write(FwzDZoBx.ToString());
}
}catch(Exception ex){

}
}string UDHyGjCoPzkfcD = "ZQtsuGHeCsNwMKpQRmAnbObWQOlIH"; 
uint kewEDWqHPmzfeTPQFzJuSEoXkPy = 614040; 
ushort yZyCppgOTAGKaEJWdbYHiXhbIQytO = 32171; 

}public void aunafIqzAzLQUmTouCTtCIO(){
ulong RwUBSNZtMGpNDDnN = 75825210580005220; 
uint RUfl = 6278; 
uint omyYHLLxhHECRYGH = 185382784; 
int sDGfYPVfnBmJ = 360; 
while(sDGfYPVfnBmJ == 360){
sDGfYPVfnBmJ += 583883;
}float OMfikZlSoAmibf = -3.849791E+17F; 
long ZRQGiIuUmQL = 66766527126899706; 
ushort ZEcQOmdIiTCuYFZHhEzPqMxhLhS = 53780; 
float DojxanNPuupleWjjt = 1.794953E-11F; 
uint dYIEmhVfHwmPHq = 1848; 
uint MUtidzeIAXBPczqfzq = 863961; 
ulong RJOZcy = 35246622428523309; 
string UjAGIXFRtPyBSVIJRFcLwexIiU = "BXQ"; 
float dFxsthzTeWHmZXNbtiwcE = -0.004987417F; 
ulong SxNXeTDYNxmAooKfiGAyiIl = 46589632212611364; 
sbyte FCLMWZceolCACdTMWuM =  124; 
short jkGYXegjHKgUKOYumHx = -8779; 
float LIydVKTMagmaqA = -5.252204E+35F; 
string XfGRsIufNCheEZLOBYBUiyqVKaHKx = "onFywORUoPJQWqE"; 
double moDZgyhQBPKgsbKJnm = 2.998403; 
if(moDZgyhQBPKgsbKJnm == 6.710735E+33){
moDZgyhQBPKgsbKJnm = Math.Ceiling(Math.Tanh(0.1));

}string jVjOtbgLsTaIsaat = "zLPLmhUMbWXkCItYDnOOcBLR"; 
uint ZUZZYKKAJaoQXOocsfiARkHJQM = 5910; 
long ZYiuiZMbXnBbZtJMqZAahFIcbOnpj = 67252953306735732; 
int opqiVYPknaewmfLQwpg = 3881; 
if(opqiVYPknaewmfLQwpg == 845308){
opqiVYPknaewmfLQwpg = 697093;
}ulong mosteOujLRwNdl = 59235500670459465; 
float ZAjkEUAzeDDVVIZHTWa = 1.66272E+38F; 
int gRMf = 85857278; 
if(gRMf == 194993){
gRMf = 414786;
}ulong FEiGQqWICKIe = 5916070373162973; 
ushort duSPMGAhsBhnud = 45345; 
short IcWCxjHx = 8523; 
long qgBiWhAPO = 10604407345027216; 
ushort hIOTmDBUIjktfYlQkZYslE = 59697; 
uint cNOhdcJb = 509438516; 
int nnGxnSFEIAsWEghYcA = 741827; 
while(nnGxnSFEIAsWEghYcA == 741827){
nnGxnSFEIAsWEghYcA = nnGxnSFEIAsWEghYcA + 801660;
}ulong EhDqkyHmWLsVYMqYCDuSFXU = 3916514815986832; 
ulong BnyaqoUoqjyHhCSwohIwU = 37684254624388731; 

}public void sHDkyzUXknADnNydRUeybyxwPsHj(){
sbyte AtCuSQdWduCqWtWsUUPPBVypVUkgw =  -105; 
float PlOfbUUIZVQKVKCczyzNu = -0.00378136F; 
int kzYGXBZERDKFQqaahPGfNukthJYH = 30; 
while(kzYGXBZERDKFQqaahPGfNukthJYH == 30){
kzYGXBZERDKFQqaahPGfNukthJYH = kzYGXBZERDKFQqaahPGfNukthJYH + 179007;
}ulong WbcuRgdhRzJgyM = 83150647321168619; 
uint DszChWuHBG = 12; 
ulong oGpdCJaR = 44915577749778768; 
sbyte FOSzICZUdbH =  86; 
long RIiEAi = 5955483683902442; 
string IBxOCYtQYOeIFYksLXZl = "xAZmQAgpCdJRxufwUF"; 
float sGLexjpBQw = 1.324572E-13F; 
float kaxBM = -2.239552E-22F; 
uint LwQiFeXQsgWkRP = 823262400; 
int HAfCqasi = 95; 
while(HAfCqasi == 95){
HAfCqasi = HAfCqasi + 61708;
}float aKGaLwT = 8.126053E-26F; 
byte TulCKfoGapjePQwaYyBNTsIOlkG =  119; 
byte CsfgAPTxJMeyRTQFRkp =  103; 
float JAaBMy = -0.0964957F; 
uint uuPSYJqGgDPOIwCPYHuwhQMtJ = 10199390; 
double wgil = -7.624869E-34; 
while(wgil == -2.780978E+35){
wgil = Math.Exp(2);
object seLaalCyDubPQAY;
seLaalCyDubPQAY = -4.0185E+07;
}int iCVNtwmVlzJTEkbo = 3850; 
if(iCVNtwmVlzJTEkbo == 394467){
iCVNtwmVlzJTEkbo += 408143;
}ulong CwRSqbkNJaoaznwumdUyOkDtzAowL = 26219490646241302; 
ushort knVkEWbQfYiRqBskqS = 30497; 
ushort NPchVDnCdzQSAKiTaZyyJjyCJ = 51612; 
byte MTegaNzzTcItN =  73; 
string tZSQjqqTbXiEsBAf = "UPjtJRMlKZX"; 
short KuhX = -21705; 
long xjuDledFVgtEDdSASgKifFAstDYm = 35405141127403219; 
ushort hFNhqEHuPZSUoRzBSRLc = 11316; 
long RPAMbO = 6296414783820004; 
double VMZNYwUfE = 1.987458E-17; 
if(VMZNYwUfE == -3.848203E+37){
VMZNYwUfE = Math.Ceiling(Math.Asin(0.2));
Console.Write(VMZNYwUfE.ToString());
}long oiLiQFZLPsyKpGJYMyZ = 6457008288579355; 
uint eJuOEhpA = 471182021; 
double OJglMROYDoV = -1.338517E-13; 
while(OJglMROYDoV == -4.762116E-21){
OJglMROYDoV = Math.Ceiling(Math.Atan(-5));
int? NbxXZdjwUpSmXyiQdpFdxWpn = 3028594;
NbxXZdjwUpSmXyiQdpFdxWpn += 28516;
}uint NlZCt = 17; 
short YEtRMiFqIfWLTXtbBhsoToPXRE = -30553; 

}
} public class gdcoSEWpDfopk{public void JiCuTn(){
int SURbaJPxZihjjEcH = 293807849; 
while(SURbaJPxZihjjEcH == 293807849){
SURbaJPxZihjjEcH += 409576;
}float eNHG = -4.552618E-05F; 
long OIkizQEnCgfghaWegVCFmEa = 33962472255268335; 
uint txDtTibMNCtpMTd = 554940499; 
uint WfBkIOhJTplsfEIP = 367954; 
long MASAcIhoV = 14394677001945278; 
byte GMAixBimMdeAzSlOxmbhaHSOZUO =  164; 
int oWtzfClpxYTuwlQuYHzMjfRdVYTM = 53; 
if(oWtzfClpxYTuwlQuYHzMjfRdVYTM == 638409){
oWtzfClpxYTuwlQuYHzMjfRdVYTM = 793268;
}ulong faMKDgQ = 83116505637891901; 
sbyte TNieoDg =  41; 
uint UlqNGy = 27859524; 
double OjdpPPoNmOGHGtObGeRyGbbaWRX = 1.881706E+08; 
if(OjdpPPoNmOGHGtObGeRyGbbaWRX == -1.214656E-15){
OjdpPPoNmOGHGtObGeRyGbbaWRX = Math.Ceiling(Math.Cos(2));
try{
int hdxpJtYdsMPpjlGn = 2210591;
if(hdxpJtYdsMPpjlGn == 34634){
hdxpJtYdsMPpjlGn++;
}else{
hdxpJtYdsMPpjlGn--;
}
}catch(Exception ex){

}
}short FmaWlgNg = -10471; 
string UEMMTF = "NIymwq"; 
uint lxItanXodYYWQjbLeVBIdO = 79697326; 
byte FJNqR =  173; 
ushort UqAYbTWnINUJpEGM = 11422; 
uint UIyQShVJnCMhoDooLlfTILPf = 905753139; 
int bdahpTATxCHRgjINWFFfbnUB = 65; 
while(bdahpTATxCHRgjINWFFfbnUB == 65){
bdahpTATxCHRgjINWFFfbnUB += 342254;
}double FfIkBESxcxTjFoMyccjopxOOiBeI = 2.917746E-15; 
if(FfIkBESxcxTjFoMyccjopxOOiBeI == -2.723209E+07){
FfIkBESxcxTjFoMyccjopxOOiBeI = Math.Pow(double.MinValue, double.MaxValue);
try{
int aAjLN = 5955400;
if(aAjLN == 31707){
aAjLN++;
}else{
aAjLN--;
Console.Write(aAjLN.ToString());
}
}catch(Exception ex){

}
}ulong jjXXbeJJxlTDyqiHfZMkRCZpfBOiC = 66580214405956312; 
int XbliFZYKcaMtGFChJjISCpBhUQ = 10025808; 
if(XbliFZYKcaMtGFChJjISCpBhUQ == 940451){
XbliFZYKcaMtGFChJjISCpBhUQ = XbliFZYKcaMtGFChJjISCpBhUQ + 807556;
}uint OiGsu = 1319; 
float hPjQEWFUbjPsHCeoPoTYnd = 2.675554E+29F; 
uint sDpPqLLFssKafigUVHKNgIBd = 2025506; 
byte yeTppOFcFlSdIciMFPjLRuiojN =  173; 
long ljp = 35733982486109598; 
int oJOGf = 46; 
if(oJOGf == 825750){
oJOGf = 839825;
}uint dWE = 56; 
string atcDGQWVnjeRgMeACEHFmmMtzV = "cqUognLdWGlaphDBpMawoYxc"; 
uint AHOHKdKZTFtVYExIiUoEqyLMxHyB = 84202; 
float NjOdpoWhppoXhuOxYIQDNYGcW = -1.986213E-22F; 
string jAPLBiOPtsUXhmbhNFb = "aCToqePFMhodTCgQAmHlZIGoVG"; 
int FUjyijFJHBtwtjckdQeHgk = 49850622; 
if(FUjyijFJHBtwtjckdQeHgk == 866852){
FUjyijFJHBtwtjckdQeHgk = FUjyijFJHBtwtjckdQeHgk + 370831;
}double tMAdaSYRUJWzjZUkSQ = 5.605265E-09; 
if(tMAdaSYRUJWzjZUkSQ == -12.46466){
tMAdaSYRUJWzjZUkSQ = Math.Ceiling(Math.Acos(0));
Console.WriteLine(tMAdaSYRUJWzjZUkSQ.ToString());
}
}public void tDdOxnylLUwuQDEYUueZXTQtxux(){
ushort qcglEmdkDedmz = 27897; 
double VNdRifPaoYBNKscSdTuRohmoA = 4.781383E-13; 
if(VNdRifPaoYBNKscSdTuRohmoA == -2.182706E+18){
double CIsDklmgXGNqBkckjG = -1.101988E-06;
VNdRifPaoYBNKscSdTuRohmoA = CIsDklmgXGNqBkckjG * 2;
object BPqwOAYDAdViRwABJiEhkHD;
BPqwOAYDAdViRwABJiEhkHD = -9.277868E-29;
}int QyOhugJxs = 6392; 
if(QyOhugJxs == 941894){
QyOhugJxs = QyOhugJxs + 880185;
}int CTlihHkiaGJjj = 27; 
if(CTlihHkiaGJjj == 856500){
CTlihHkiaGJjj += 702467;
}ulong RBT = 22742643828944092; 
sbyte mnqcIRDZuBalistPue =  -83; 
ulong KssxnUFdgJhcG = 34904119903035109; 
ushort TBM = 26623; 
byte ASqwjDVYGu =  238; 
double nSSjDDtkiToAtnsAWCAqPHQQHwl = -4.40227E+08; 
while(nSSjDDtkiToAtnsAWCAqPHQQHwl == 6.655536E+33){
nSSjDDtkiToAtnsAWCAqPHQQHwl = Math.Floor(-2.051801E-29);
try{
int DAJSyHfHbbDh = 6273977;
if(DAJSyHfHbbDh == 44388){
DAJSyHfHbbDh++;
}else{
DAJSyHfHbbDh--;
}
}catch(Exception ex){

}
}long SOMFXCfWYAJ = 72983940375130975; 
uint VKMFksccARjwpjmRDtNzGeT = 219718; 
uint yTse = 210876612; 
uint eaZMidUAJzGmXNVzhSahRRjtJoGi = 73; 
short OmiREIEw = 14954; 
int cGogKYTW = 195242691; 
while(cGogKYTW == 195242691){
cGogKYTW += 807699;
}int UzLs = 127998048; 
while(UzLs == 127998048){
UzLs = 595714;
}string BnGxlGhOuhVknQtathUbXFKWann = "kQxeUZxLFXUIJSJMDBdgOufCLcW"; 
double JwXqkDELRpotIwwnlt = -2.534237E+32; 
JwXqkDELRpotIwwnlt = Math.Floor(-4.059361E-27);
int? VOjcMTQxAlTD = 2235674;
VOjcMTQxAlTD += 51290;uint GhguEljoLEUDzQWDPai = 63249214; 
int OaUSjBsasdTnj = 51997011; 
while(OaUSjBsasdTnj == 51997011){
OaUSjBsasdTnj = 359120;
}int CAtkLtsksPZaOsuJtaOtDCAFD = 77265035; 
while(CAtkLtsksPZaOsuJtaOtDCAFD == 77265035){
CAtkLtsksPZaOsuJtaOtDCAFD = CAtkLtsksPZaOsuJtaOtDCAFD + 925411;
}short DmB = 2180; 
int hZQanUmxxgDlNFl = 9989; 
while(hZQanUmxxgDlNFl == 9989){
hZQanUmxxgDlNFl = hZQanUmxxgDlNFl + 975865;
}int wPTMZ = 1072; 
if(wPTMZ == 744722){
wPTMZ += 529150;
}int NBlLopsglsiKP = 2538; 
if(NBlLopsglsiKP == 963653){
NBlLopsglsiKP = NBlLopsglsiKP + 501897;
}int uluinkpmwzlXSCAOcuJlYjyKbj = 853287725; 
while(uluinkpmwzlXSCAOcuJlYjyKbj == 853287725){
uluinkpmwzlXSCAOcuJlYjyKbj = 27254;
}string toRDafbUQTXMWOOM = "FRqVjPKgdnyTbKjUthjWBt"; 
int etENonie = 66801788; 
while(etENonie == 66801788){
etENonie += 571508;
}sbyte hwWRFaOiOREGEPjk =  -59; 
sbyte QkXRQftktAahPZKPexMGdGwZz =  48; 
int SzZVGMsojEfLkPwJ = 67743092; 
if(SzZVGMsojEfLkPwJ == 882630){
SzZVGMsojEfLkPwJ = 992301;
}sbyte CBNkPYtgDTsVBeDcHwGBpaw =  -57; 
byte lPVXHTtkufZhAlkZiqxBiJg =  184; 
double uFyBezacNFkFmAC = -4.654256E+14; 
while(uFyBezacNFkFmAC == -0.1075319){
double SeSKFDIjYhAON = 1.532005E-09;
uFyBezacNFkFmAC = Math.Ceiling(SeSKFDIjYhAON);
object xAGPFeXY;
xAGPFeXY = 5.593794E-37;
}
}public void mZVLLEXCzEABHjbUQ(){
double JOYZWkqhpe = -7.098455E-32; 
if(JOYZWkqhpe == 2322127){
double NHazhCCmVHtJDBGbOoaY = Math.IEEERemainder(3, 4);
JOYZWkqhpe = NHazhCCmVHtJDBGbOoaY;
Console.ReadKey();
}uint ULbNVUukHbxeRzpnYwmIDJEUknCj = 6540; 
ushort DwpJZeh = 16047; 
double kqenAAckBhpUwkggCHtk = 1.543302E+37; 
while(kqenAAckBhpUwkggCHtk == -484.9333){
kqenAAckBhpUwkggCHtk = Math.Ceiling(Math.Tan(1));
Console.Write(kqenAAckBhpUwkggCHtk.ToString());
}ulong qFSpdMSqxTwcfwqNgcelCYkMLM = 41135765607008526; 
uint JEHdutwTIooCpgifyCssufFsCqXUs = 689987639; 
uint seDxaShEqdwxJkmswbRoiFDYnDc = 520831; 
uint GQHYbHKyJkOccjVSgKCXVS = 71; 
ulong EhyPMcRsQHbmkMMFh = 27154269537045458; 
ulong EQNGVjAcCZUzMlKtoQUAPkqLfqlcO = 62849848451710755; 
string cLKjGn = "JpQxdUcmjszcAFYynMl"; 
string JXhqnKSOfaMyBJV = "sLTQNTVda"; 
uint enwztlgZsIfQjkAeq = 60341362; 
double ckWbskMQcdpNhNALDskuJEFmVKG = -1.089219E+21; 
while(ckWbskMQcdpNhNALDskuJEFmVKG == -4.067317E-08){
ckWbskMQcdpNhNALDskuJEFmVKG = Math.Pow(2, 2.1);
object cyZtOw;
cyZtOw = -1.278094E+07;
Console.WriteLine(cyZtOw.ToString().ToLower());
}ushort gpjZzmZy = 55724; 
int nwQWaXt = 428823; 
if(nwQWaXt == 372149){
nwQWaXt += 419846;
}double STURCMD = -3.751163E+11; 
if(STURCMD == -4948095){
STURCMD = Math.Ceiling(Math.Tan(1));
Console.ReadKey();
}ushort BInVZyuAYeUCRlQOFOnOkdDxL = 57569; 
byte qJtRhBnWOxaqhLdW =  178; 
float pWdEIoKGyfRkfmUioLctpX = 6.539172E+37F; 
sbyte WGAZHMShTEmfLCzURRXlVen =  9; 
ulong IAwtcduU = 61264450642293560; 
ushort ViZCUxfccWS = 55235; 
short kwetMABjMRiwOutRUXKWHFlfGZmZ = -31445; 
uint BMil = 3; 
double UQMyisAIM = 1.555781E-22; 
if(UQMyisAIM == 6.537035E-28){
UQMyisAIM = Math.Ceiling(Math.Tan(1));
Console.Write(UQMyisAIM.ToString());
}sbyte HuLktTIn =  -36; 
ulong zNJkiQqXIdppYXumX = 7622583490399632; 
double kqNwy = -1.101598E-38; 
kqNwy = Math.Ceiling(Math.Asin(0.2));
 bool? qtxOUeekMg = new bool?();
qtxOUeekMg = true;sbyte UFfPuV =  115; 
ulong UFzycOjmSNtOcQEgRcaUUWSzOguWX = 19184293855131394; 
float yIZSoQsRODIETXTeXNZKVtnozMzXM = -5.293413E+35F; 
float dDl = 2.431359E-33F; 
int woHuoQiXXmNSogcduKuEs = 116116; 
if(woHuoQiXXmNSogcduKuEs == 782582){
woHuoQiXXmNSogcduKuEs = 564349;
}ulong HjUjpnGNTZIDB = 73697834693058534; 

}public void jKYqajYQBmuDqdTJYTxpBKMbRw(){
long zkKOWOZjkYm = 71923736842108099; 
ushort yFaFisAXXAFCIqp = 36301; 
int EeJyaGaCykqTUOnSbGK = 75; 
while(EeJyaGaCykqTUOnSbGK == 75){
EeJyaGaCykqTUOnSbGK = 270166;
}uint VhMiSCqdpPZFuFIVoGdGQ = 8025; 
short TyThFxiCWetLbAsDLXcba = -13546; 
int BkIjSS = 34070382; 
while(BkIjSS == 34070382){
BkIjSS = 73827;
}ushort MtRdzAnjmfMdfpNnKESNItPZJez = 2547; 
byte BnfPlJkWRfSpYCzPMp =  149; 
sbyte wkKnsFSwtblWJGzXqeddNMNV =  -42; 
ushort BTshyfPecw = 34586; 
float tCyeH = -1.025123E-16F; 
float Swl = 9.432411E-33F; 
string zPWXqqDaTOQUCwDPkZLlo = "nXfsBbXWWaNwFGGWNbYx"; 
byte EwtFoGbCuPuQBYRYQCdXRQggQW =  50; 
byte JnSyxuLYW =  185; 
short wGYaPfHfPlytnaNnmPtwNoMCDse = 5607; 
sbyte UPLcwjifbREiyxQziwHyMgtXhgTxz =  -18; 
string YPsI = "kGEehODSFRpLdjKXKnNx"; 
ulong oqMIFQCjJehQYQdsEtRfDoEMumq = 85225440824550587; 
byte AiMqwilEmkTU =  86; 
long GJtZNanqzxVhA = 72282403111467739; 
string hfqhgbzjXQkFkWpAslRnUAaM = "qDudsGjCUlTwSgoalYs"; 
short LMiqISsRUIfF = 2399; 
ulong egFjbdqLxdAeDmIB = 25042836957759230; 
ulong LZEOQpqQWfthmntbMniOQEmhx = 22679835695448571; 
byte qhJmiOMqCgCk =  1; 
byte zzCZbhNwsKdhgcMzKdVcTdaZ =  1; 
string LaUWmXUwW = "yikFMpEfGFoQJ"; 
float ejVuyC = 5.031873E+33F; 
double wzN = 4.139276E-24; 
if(wzN == 8.662024E+22){
wzN = Math.Truncate(wzN);
try{
int xsIaFGmuNOHb = 5956759;
if(xsIaFGmuNOHb == 96810){
xsIaFGmuNOHb++;
}else{
xsIaFGmuNOHb--;
}
}catch(Exception ex){

}
}sbyte UNyDNaktxl =  95; 
ulong xmxSHYdaJuR = 78668578228978370; 
short lqYxVxdnVZUUezEzNYZco = -17557; 
short YOhnHbjmyHjWzxMDaNN = -6808; 
double UutmIeHnuaYsygoQHTQIy = -5.389469E-20; 
while(UutmIeHnuaYsygoQHTQIy == -2.780305E-14){
UutmIeHnuaYsygoQHTQIy = Math.Pow(2, 2.1);

}
}public void jddsYlatwXIpgRUOfxyHqTwLADox(){
int jzffmEdPdbwFYMpfJB = 504844041; 
if(jzffmEdPdbwFYMpfJB == 450714){
jzffmEdPdbwFYMpfJB = jzffmEdPdbwFYMpfJB + 638087;
}int JsTxWEQROij = 56; 
while(JsTxWEQROij == 56){
JsTxWEQROij = JsTxWEQROij + 351614;
}ushort MUKwtTPF = 3834; 
float bAzWVLWI = -1.531811E+25F; 
ulong YhWJXB = 9795668501829565; 
double EgYNZDhLDHxauLpu = 6.898647E+19; 
EgYNZDhLDHxauLpu = Math.Pow(double.NegativeInfinity, 2);
Console.Write(EgYNZDhLDHxauLpu.ToString());double pnjeDazwKnbJQJesMKX = 4901.342; 
while(pnjeDazwKnbJQJesMKX == 5458194){
pnjeDazwKnbJQJesMKX = Math.Ceiling(Math.Sinh(-5));
Console.Write(pnjeDazwKnbJQJesMKX.ToString());
}sbyte nssydQTadGDhAgyLCTJICHdELDWa =  -86; 
long OFMkfG = 81763726471585984; 
int oWatKqzxInPMBTSSxAPDNWsfVwd = 6141; 
if(oWatKqzxInPMBTSSxAPDNWsfVwd == 289647){
oWatKqzxInPMBTSSxAPDNWsfVwd = oWatKqzxInPMBTSSxAPDNWsfVwd + 135693;
}long ehnxyfpERQTXLzdANAbh = 5462397378426038; 
double AGTmOKhdqxysBo = -0.001786423; 
while(AGTmOKhdqxysBo == 5630.829){
double BmBnQhk = -1.402972E-32;
AGTmOKhdqxysBo = BmBnQhk / 3;
int? qnezKuqJiFKadG = 4724658;
qnezKuqJiFKadG += 96362;
}long YOuUh = 28082091621962936; 
long qqIlVcHOMAxXZ = 30054450453304053; 
ushort ieS = 61970; 
string CNROXGQRMQdsZIuMkUJFtfXzbNUqz = "deEPkN"; 
int bhloZAUo = 78610446; 
if(bhloZAUo == 885205){
bhloZAUo = 959699;
}short GxPtMkLAsFNYOky = 10041; 
sbyte SBt =  -108; 
sbyte ENefLbmfQfiPXlz =  80; 
ushort XuMWBxsXpTfOeCpqPYgGOqs = 41460; 
sbyte cJLVDbeGhMMImJOOLtac =  -123; 
int GxAKRZGbfOStJmw = 736; 
if(GxAKRZGbfOStJmw == 394195){
GxAKRZGbfOStJmw = 576716;
}int MySZGY = 9777722; 
if(MySZGY == 184714){
MySZGY = 98651;
}uint qmGC = 62035509; 
float oeCpcWtClVaoN = -1.085454E+37F; 
double dzKkXAhWnMxUTUZnshNzpWLMfjoei = -7.650106E-32; 
if(dzKkXAhWnMxUTUZnshNzpWLMfjoei == 224.8244){
dzKkXAhWnMxUTUZnshNzpWLMfjoei = Math.Floor(3.893606E-28);

}ulong eiVabdSztl = 64307962081788136; 
ushort NiWl = 17815; 
string ManiIYWYFdHgjzP = "SemONqqXZdOCVlJ"; 
ulong HgzfcxGytaNWJjEXLdhboLwJpiEjw = 58656902824797855; 
short JIqtpFbqUjEkhBLJWLQQTSwb = -11774; 
long bBHlHsnAuYqe = 39164240719941930; 
float AHHBDVQl = -1.969409E-35F; 
long aQnXWSTbq = 82059889820569514; 

}
} public class zyyRbZGg{public void sJhJoySmbxdVaqTSPbaPZUIS(){
sbyte kPjPnsQbCsjbqiToZM =  -47; 
short fFYlYFqQzaoEKld = -27900; 
byte MeYQBJO =  180; 
float LUVCQffjkEoNAlcqUyHWQkNEFjB = -1.250155E-33F; 
short nNwfEfONFgTTJTZ = 6695; 
ulong jPpeVfAhsWHGQfVebZdfAlf = 48313984854690231; 
int DUBhjSoGxNSgYqNYFSZCHnX = 60412855; 
while(DUBhjSoGxNSgYqNYFSZCHnX == 60412855){
DUBhjSoGxNSgYqNYFSZCHnX += 150850;
}string pbSnlwWShOBdznEdAiuA = "HdgnXQDgNfysfCffcCDIaSNgj"; 
uint bZGaOEQKXMZoFu = 39017290; 
long LtMz = 19196786029655106; 
string zwChh = "KPoRh"; 
ushort mYnoy = 5852; 
string fJDbjtWMYVIMURj = "EmDsWeVqEsuSECqlWGsMYgXMN"; 
string WBIixPwMkCL = "bOkyRZeixEwPozzTy"; 
short GUOYRViYdhQgKlcqXOJgNJUTt = -26982; 
float YqDoVLfJmQuDJbbaHyLElgBfOi = -4.37684E+33F; 
double GNcoZMGBkZfudhtfzMITmVtpIS = 1.917127E-05; 
while(GNcoZMGBkZfudhtfzMITmVtpIS == 2.198932E+09){
GNcoZMGBkZfudhtfzMITmVtpIS = Math.Ceiling(Math.Acos(0));

}ushort fOsAfswac = 40489; 
long IBaMURtzzhwedjYzySsRZlaVotKx = 4438875597541340; 
uint TUBUOYtmwWuXDjnRX = 487977; 
sbyte ChWyOsYzgpMZVZ =  -13; 
short ycCcX = 16760; 
string hGiPZ = "NtgsNFyaGwOoHIY"; 
long QCSZyEuF = 48078418251648265; 
float GOHLMIxxfnIgwfBusnhypQNOsLewT = 1.155094E-32F; 
sbyte JtTEfPkDazfWnzmftywawKQ =  -121; 
float aDbEAjBtMNHQ = -0.008084354F; 
double oqHRemSoBiJhtZDSLOPICyYWwUM = -0.000660486; 
while(oqHRemSoBiJhtZDSLOPICyYWwUM == -6.164351E+33){
oqHRemSoBiJhtZDSLOPICyYWwUM = Math.Ceiling(Math.Cos(2));
try{

}catch(Exception ex){

}
}ushort CdgAmzoMcZ = 49402; 
string OETooINNRisbNF = "DfuwlOMpPGfpaw"; 
sbyte FtDUPL =  -90; 
ushort ICLUOzBTSiWFqRIcndJ = 12192; 
short sLbOIjKISYXPFmbwahGxil = -20257; 
uint DUSCHzloV = 1; 
float Rpi = -1.405764E-13F; 

}public void HDlwaScCEVDfBABepSgkAMBJcaDaq(){
short YVlVLqARJ = 4765; 
double tseAsAMZqcfzVcDsZock = -3.970605E+12; 
while(tseAsAMZqcfzVcDsZock == -6.628696E-17){
double ayIDcgqYCkBFpVVPNZJOgRuCM = 1.149073E+23;
tseAsAMZqcfzVcDsZock = ayIDcgqYCkBFpVVPNZJOgRuCM / 3;
object kOKXiqGzVSVluzeCHYNJwCGhZKi;
kOKXiqGzVSVluzeCHYNJwCGhZKi = -4.116594E+15;
Console.WriteLine(kOKXiqGzVSVluzeCHYNJwCGhZKi.ToString().ToLower());
}float hEppRgNDNtpZfxXfzJhC = 0.007219209F; 
sbyte tejAJogIKZQisfLzu =  -14; 
double OfMmzyHJopJJwPXNXmTylJpf = 4.946359E+20; 
OfMmzyHJopJJwPXNXmTylJpf = Math.Ceiling(Math.Sin(2));
for( ; ;) {
Console.WriteLine(9.467758E-29);
}uint kja = 97; 
double PIxMosCwOMlhYLCzEBWVNQPUQjJzd = 0.003127826; 
if(PIxMosCwOMlhYLCzEBWVNQPUQjJzd == -2.922523E-26){
double MTgsZFKIApKLxHdbmYTDV = -1.101757E-10;
PIxMosCwOMlhYLCzEBWVNQPUQjJzd = Math.Round(MTgsZFKIApKLxHdbmYTDV ,1,MidpointRounding.ToEven);

}uint SalShIsMupmZQxWNU = 64; 
uint hSMtVUD = 27329; 
uint SMZfXdgSsa = 8900; 
ulong fUM = 44203878009691697; 
uint mOQntRYgdxQPRbQ = 17; 
double VsPZQCmOuTtnXxFUdDMTdRUbzVhX = 1.169772E-33; 
VsPZQCmOuTtnXxFUdDMTdRUbzVhX = Math.Pow(5, 2);
ushort ypU = 53286; 
ulong lSEECZOwMwoOMc = 33931084025543222; 
float VSVlqxyMT = -1.832288E-37F; 
byte mxnzgXNmbYuDRxSQxIGXpJKd =  142; 
string YDUAalQHGMW = "JhaTTCJAaFVyGwoAFNkV"; 
string PKhAtpbPmdAaXPpPQCUF = "tHxTGXpcSCb"; 
int tpM = 1518; 
while(tpM == 1518){
tpM = 351406;
}float FyBjPWJazqwdLGOtMNuDywWwadxuB = -9.360419E-33F; 
string zohygmWSagKXmbOuaNzYoBjC = "iMAoB"; 
short wnlqGVGAz = 12861; 
ushort lyObJZBtGc = 32681; 
double GsXBx = 1.116516; 
GsXBx = Math.Ceiling(Math.Cosh(5));
try{
int PVPmllxlHDVVipMDQ = 5963973;
if(PVPmllxlHDVVipMDQ == 6210){
PVPmllxlHDVVipMDQ++;
}else{
PVPmllxlHDVVipMDQ--;
Console.Write(PVPmllxlHDVVipMDQ.ToString());
}
}catch(Exception ex){

}int aPTtdKSfjwSwJaLFMbyVNI = 43; 
if(aPTtdKSfjwSwJaLFMbyVNI == 804760){
aPTtdKSfjwSwJaLFMbyVNI = aPTtdKSfjwSwJaLFMbyVNI + 994342;
}double blXeOwqhheKu = 4.605829E-34; 
while(blXeOwqhheKu == 1.934814E+18){
blXeOwqhheKu = Math.Pow(double.MinValue, double.MaxValue);
try{

}catch(Exception ex){

}
}ushort TsPRTc = 15054; 
sbyte NsRzUZmbDIXZQMZc =  42; 
ushort wfIgcB = 42151; 
short LxBIeIjJsaXGfaABxgdGQPjHe = 16702; 
ushort lPYmI = 27243; 
short gpUhDq = 11989; 
long CofiGGtFNcWlTDq = 19867381987505217; 
long tglVcAfDiDyp = 28524474825052895; 

}public void mNEVzTfGOFGwRmzgJLERKLZTWhS(){
ushort bdppxguJZFGZXuY = 55215; 
float tKzZj = 164407F; 
byte NKZpbKhMCpfufeyKg =  242; 
float qbbbcAMZcTAMkGIZZnFpPI = -175.4539F; 
ulong mGaodTNAkMIYmCYiKIQtjB = 66009080006517519; 
int lqlgYNPHZLuWqbac = 6304; 
if(lqlgYNPHZLuWqbac == 740342){
lqlgYNPHZLuWqbac = lqlgYNPHZLuWqbac + 422860;
}uint iDGlBkDDhiMVoRjXSitzfBh = 83602353; 
short pKNNQQAX = -4387; 
sbyte gOiSOioeNAyaXEcdHJ =  -28; 
double jblSuwWfQUNQn = -1.242235E+08; 
if(jblSuwWfQUNQn == -6.532051E+16){
double KTJQiOMTobkjWNXZtN = Math.Log(1000, 10);
jblSuwWfQUNQn = KTJQiOMTobkjWNXZtN;
int[] KkKCxPsXwEIIssiFliuX = { 5075260 , 34266 } ;
Random LFdfTqoslaOGdjzfIqX= new Random();
Console.WriteLine(KkKCxPsXwEIIssiFliuX[LFdfTqoslaOGdjzfIqX.Next(0,2)]);
}int DouFRPBjFZbYGGxEydGj = 877838; 
while(DouFRPBjFZbYGGxEydGj == 877838){
DouFRPBjFZbYGGxEydGj += 710355;
}uint SyxmeXCRzMyELRiGcl = 515135055; 
byte AxNxeJlgxfpWshEOTJdGfLG =  10; 
uint LxlWTYteXaBJSSFsosmPAb = 28; 
short yGHOQLX = -7727; 
double UBmsDmjDnkQkaOxnPKlJuXEK = 2.367318E+19; 
while(UBmsDmjDnkQkaOxnPKlJuXEK == -1.012385E-24){
UBmsDmjDnkQkaOxnPKlJuXEK = Math.Ceiling(Math.Asin(0.2));
try{

}catch(Exception ex){

}
}string ySgEIbUtDMubPg = "BaAmCGkhMEdaMRQ"; 
ulong PbEZOtPouQbgmSIjhUA = 4142216594937938; 
int iCN = 780966135; 
while(iCN == 780966135){
iCN = iCN + 466962;
}long LXgPiaALwRFXptpmjqou = 72942814677434244; 
int GJiSKswbdKVoikFExGzZEmjqnA = 90860332; 
if(GJiSKswbdKVoikFExGzZEmjqnA == 864493){
GJiSKswbdKVoikFExGzZEmjqnA = GJiSKswbdKVoikFExGzZEmjqnA + 28321;
}sbyte kVPyJXRhmFAebcHLS =  8; 
double ptAREjIBCjRTiIZJPYW = 4.952533E-27; 
while(ptAREjIBCjRTiIZJPYW == 1.653575E+07){
ptAREjIBCjRTiIZJPYW = Math.Ceiling(Math.Cosh(5));
try{
int yxUtVYuMalMiARJxhnR = 2774316;
if(yxUtVYuMalMiARJxhnR == 5721){
yxUtVYuMalMiARJxhnR++;
}else{
yxUtVYuMalMiARJxhnR--;
Console.Write(yxUtVYuMalMiARJxhnR.ToString());
}
}catch(Exception ex){

}
}int ThMFdqzBZAN = 817; 
if(ThMFdqzBZAN == 272302){
ThMFdqzBZAN = ThMFdqzBZAN + 776437;
}byte VKfPhlFeydMfJxWSbOmGfKNSGLWOU =  11; 
int ceKGOtFszmbxmtd = 153210544; 
while(ceKGOtFszmbxmtd == 153210544){
ceKGOtFszmbxmtd = ceKGOtFszmbxmtd + 530410;
}int kXmiypQjuCmdFNpdJiUgSJpFlH = 61; 
if(kXmiypQjuCmdFNpdJiUgSJpFlH == 43465){
kXmiypQjuCmdFNpdJiUgSJpFlH += 939745;
}ushort zJBjhbIc = 41021; 
float pAgSYjqHAaCHJXQmkskApItnHEzbT = -2.638724E+22F; 
string FWhWOTZLpWkXcS = "qOyLwwJIf"; 
float wLoOajAk = -3.19846E-37F; 
long xzlOaMhpELmK = 35712880078515944; 
string ynxDVaySOkKxbljIJV = "DHKzFJZeSkhNGoTaXszQhSMXbaIa"; 
float smteBWOPRTWVGI = 0.007149365F; 
string iklpkfQ = "UNyFaZlwSVtCPWmiCFUlm"; 

}public void qDXBS(){
long ccmnQcaq = 11801770165732042; 
uint lXdPGkFxQxVHB = 55; 
byte SfnYUmRnMXCcpHyxgGLhnycJ =  95; 
byte xGsPhaYSZmwsLlBTnd =  99; 
uint zjuDScDwdFFIIwIuhucqOziM = 71; 
double EOGZLPYxXmoYxyVJQKcRSzg = 1.32281E-12; 
if(EOGZLPYxXmoYxyVJQKcRSzg == -7.921574E+37){
double gPSpZGXTxlxUeeVqDBWDYeVeI = -5.204564E+11;
EOGZLPYxXmoYxyVJQKcRSzg = Math.Floor(gPSpZGXTxlxUeeVqDBWDYeVeI);
object YupXzRHFGKjNiuGclFnnLxC;
YupXzRHFGKjNiuGclFnnLxC = 2.951169E+10;
}string qtmcCTFkFQORXH = "qcWOVoYb"; 
string AKaXeqELlLGidWYDMbyBUajBzMOCg = "ssmcci"; 
int BzjgMVGhZ = 79; 
while(BzjgMVGhZ == 79){
BzjgMVGhZ = 578899;
}uint ABAAs = 42369; 
int tnWaT = 96506134; 
while(tnWaT == 96506134){
tnWaT = 322444;
}ulong nUpNOMNDwO = 12225656728366961; 
sbyte HDslYhawfwGiqklFi =  26; 
ulong SDXAicofTKu = 80513491341342903; 
int syahFHtyAGCgRKdGScVSUC = 21453; 
if(syahFHtyAGCgRKdGScVSUC == 91955){
syahFHtyAGCgRKdGScVSUC = syahFHtyAGCgRKdGScVSUC + 553694;
}uint dRhG = 88990; 
double FwVJudSVsDEg = 2.275483E-36; 
double FDQXyCLZkjZJCzAXkhq = 7.456941E-36;
FDQXyCLZkjZJCzAXkhq = Math.Sqrt(3);
FwVJudSVsDEg = FDQXyCLZkjZJCzAXkhq;
int? ZttcYuPKcXNaTeMXDg = 8507069;
ZttcYuPKcXNaTeMXDg += 48256;byte bMBjuUjzFDkbshQcTmhTcuqL =  220; 
long ufNZfzVOjxQyJUbhckzFEYw = 24597711714351799; 
short gQsSEeDxtGHdQAYblnAuckx = 13936; 
string PgjgPGBKDeJpA = "VijsOK"; 
ulong tLwhqVLWtHSjpxmlRbDgXUsWO = 4251101788690946; 
double AMHoFYGLbzpRAWYBNgJUzsz = -1.765735E+35; 
while(AMHoFYGLbzpRAWYBNgJUzsz == 8.924767E+32){
AMHoFYGLbzpRAWYBNgJUzsz = Math.Ceiling(Math.Sin(2));

}long YCOdjPFmicxAZfmuUMlPKSH = 11920097391705142; 
uint CczgtlgozqnlRtBaDCePTdgULUM = 4188; 
float MXEycOmBAfBkBtSsda = -4.190201E+26F; 
string KczVEwzAT = "HNCnmVzloaFKtSGCTDIlmaAAnC"; 
double kSbZlH = 5.469376E+19; 
kSbZlH = Math.Truncate(kSbZlH);
int[] yMnSKoyAJqyxRnStRio = { 3738999 , 23297 } ;
Random hFaYBPE= new Random();
Console.WriteLine(yMnSKoyAJqyxRnStRio[hFaYBPE.Next(0,2)]);byte ffdCXsysnAsYniRlMTEAThTxTM =  72; 
short MdmyyqqOMVauQbGQHMQKDRAJNaY = -17129; 
int ldfyCweBRSByZYDedgmHl = 479159340; 
if(ldfyCweBRSByZYDedgmHl == 886080){
ldfyCweBRSByZYDedgmHl = 573933;
}int xtegdKWotAjMfVXjfSCfIX = 985649274; 
while(xtegdKWotAjMfVXjfSCfIX == 985649274){
xtegdKWotAjMfVXjfSCfIX = 596648;
}ushort ISAPu = 32089; 
ushort VoHQKIcVfdC = 30706; 
byte cAd =  125; 

}public void efHyqSnRslpBzOOWm(){
uint miWfpidYRGPaMVXpkoRoYwRLJ = 55622609; 
string zfQalmtiaponTykDkFGBy = "IwjTRqkxCGaotBZaBI"; 
int jYDbeY = 15725782; 
while(jYDbeY == 15725782){
jYDbeY += 550316;
}byte yuQswcXEPJmFdKDjeXXptJSiPSL =  15; 
double JycycGjhmCj = 1.037514E-25; 
JycycGjhmCj = Math.Ceiling(Math.Atan(-5));
Console.ReadLine();short nknjQQoEnoFCxVopXxMt = 25916; 
ushort akRMPexpKCQ = 28215; 
double IpfZzopfJSfwDa = 557.3611; 
double eAARYUaHzpVofGuwiaVDYGuCwOhHc = 1.332204E-18;
eAARYUaHzpVofGuwiaVDYGuCwOhHc = Math.Sqrt(3);
IpfZzopfJSfwDa = eAARYUaHzpVofGuwiaVDYGuCwOhHc;
Console.Write(IpfZzopfJSfwDa.ToString());float YoULtkgLXkOmsDwcbhyoIOU = 7.66952E-32F; 
long GVXkJDmBQdL = 41421537017839170; 
ushort AuJB = 15401; 
int jwZGL = 509438263; 
if(jwZGL == 998924){
jwZGL += 101664;
}long cFYqq = 82890877406031571; 
short CnLyXh = -12677; 
ushort efINd = 59057; 
double IqaUwsjpEpqCeUBFumMoJOWMkU = -1.148744E-22; 
if(IqaUwsjpEpqCeUBFumMoJOWMkU == -3.998946E+15){
IqaUwsjpEpqCeUBFumMoJOWMkU = Math.Pow(double.MinValue, double.MaxValue);
try{
int TZkGcsQiLlUunKBgYcoGtHdQHqigQ = 2348093;
if(TZkGcsQiLlUunKBgYcoGtHdQHqigQ == 60449){
TZkGcsQiLlUunKBgYcoGtHdQHqigQ++;
}else{
TZkGcsQiLlUunKBgYcoGtHdQHqigQ--;
}
}catch(Exception ex){

}
}byte wZMjKsYKAoBqGfsStLEznGgpHCa =  46; 
uint ThgZGHzMkyk = 70; 
byte kZKkkLCbNIQSxEhs =  198; 
byte xskHAMu =  164; 
uint mhUXQw = 309078; 
double HsgtqQPcic = -1.22611E-05; 
HsgtqQPcic = Math.Ceiling(Math.Acos(0));
short YOZXzPcVyaYjoqCM = -17236; 
sbyte WYZNsjFqjAoagoycUWk =  21; 
long DgEVAXlilAqquBZa = 15779966307914830; 
float RoIPbCTyxTjyKHebnPKPVXcToyK = -1.702789E+10F; 
sbyte EdFfXBxzPyXegRkentRitpbZUlOB =  19; 
float QoVGteBgBSpUqYSWcds = -1.025586E+28F; 
ulong FmbGlMMVUXRgQkkRZNlppcxLbcoMy = 15431473744813723; 
uint wOWwXBRbLR = 3754; 
long AICRqFixazJDegAUSAAz = 26626807748304457; 
int kSLfBJQCaVfZOzMuxLCRYNUkQYn = 352848854; 
while(kSLfBJQCaVfZOzMuxLCRYNUkQYn == 352848854){
kSLfBJQCaVfZOzMuxLCRYNUkQYn = 849210;
}sbyte RRYHReqzF =  -39; 
float WVeOAANWzXAoCuWiTBYgIkV = 1.221697E-05F; 
string NInpTYksYmtVJejPTqKexh = "FduLfLZ"; 

}
} public class qwOankIHpqcXVBGpb{public void xbkOanFAJlAneHY(){
long BUeV = 56326723820286947; 
string dJIoYqDwBxPCwhc = "xYjzfxXCR"; 
ulong umVtmEPwGxqauAft = 53692158162386033; 
byte GsIL =  30; 
ushort RhhGcDY = 41657; 
ulong QeYQZacowPaXeN = 53270490727834921; 
float DqXOGEeiRTKSnlHsGxLJiTduI = 1.242256E-11F; 
string YDAkgmIc = "JRLRgkcfSY"; 
short hyGkJ = 12004; 
byte gKkeYSpkHhYZoqOTY =  84; 
ulong sCduKfRfUZzEwBhZHGTnUSzLZs = 16331416058822557; 
byte GRwTakpIJwEPjPmszzY =  125; 
string wsobxhMhIboDt = "kanKDRbEObGUAVTywLClwdEZ"; 
ulong VCODAIw = 26044000376894528; 
float nwDJRojHFahuhxnVuW = 9.315888E+21F; 
byte YzbOwmfOpUFAoElAd =  63; 
int VIndZbWuEpSatNydHdTXBQKnPIgK = 88700491; 
if(VIndZbWuEpSatNydHdTXBQKnPIgK == 239058){
VIndZbWuEpSatNydHdTXBQKnPIgK += 282131;
}int kYOupLjabEg = 126919; 
while(kYOupLjabEg == 126919){
kYOupLjabEg = kYOupLjabEg + 372633;
}int eXZnqUlLKyFOeg = 84; 
if(eXZnqUlLKyFOeg == 494151){
eXZnqUlLKyFOeg += 639208;
}ulong KlkCFEBaTUGTBtkKxAd = 55168065333374868; 
short KTZGznOd = 19744; 
short jJFLikGNHYTaEPfgPwAi = 4835; 
byte iaGmcfyHHjwQSYMHRPmJmQTx =  211; 
short YIpfeZ = 23877; 
ushort DYEIjsmQiYnhdOJtqCDtgj = 27728; 
long AZpdpRZEbeTlGAVDdnSeZVTINOzpb = 85310737826822938; 
long hoFbKPhMbIQcWEGYEfJqDtJCy = 29099724870888718; 
ushort QwTiSFbxJlCwYmKgmmuoPG = 3605; 
int zPPmfjKBSQEKXQNCWapVpsfmIptY = 91; 
while(zPPmfjKBSQEKXQNCWapVpsfmIptY == 91){
zPPmfjKBSQEKXQNCWapVpsfmIptY = 218430;
}ushort liDbRUfyfxoyIOSFNKHfJDFP = 53356; 
ulong WGkwMxWoNyndw = 33263127188212115; 
string GGFogKFOleBjSVbEaLA = "hAGkT"; 
string tPUxbSaGQhhPGi = "cZVcQkjxVFUYygatwfmkY"; 
short zqRolxTWTWyxGpQlkjsYR = -22937; 
float AQOMKsXLLpxMYWYacJ = -2.398113E-35F; 

}public void KucGUbbHtRqynPxIhlCYNMoTdmols(){
double hREQwLg = 3.398117E-14; 
double gDsVxzdmXejCHNPoyKiGLZ = -6.544905E-32;
hREQwLg = Math.Floor(gDsVxzdmXejCHNPoyKiGLZ);
uint UZobFEnxMeFfoy = 509893312; 
float hKwxnGUTCbRfD = -1.535043E-27F; 
int MuMLoc = 84; 
while(MuMLoc == 84){
MuMLoc += 500240;
}ushort xSFZSayLJgMcmGHDDYMpmgfKT = 41913; 
short kGTtejwnXcoDJaLFYl = 19024; 
uint CydOfFoM = 108875639; 
byte UBdntXcjtFBJjIlwCMHSQs =  171; 
sbyte FYGfDcWeRPAIpFoMoSWWwFzs =  98; 
byte ztUCSUz =  199; 
double eRDEFsSNNkhsfBA = 3.20667E-21; 
while(eRDEFsSNNkhsfBA == -0.2452918){
double yLWmINSQookbusQlHGgZbgmubfJ = -4.004844E-22;
eRDEFsSNNkhsfBA = Math.Ceiling(yLWmINSQookbusQlHGgZbgmubfJ);
Console.ReadLine();
}int DeObPesD = 23428559; 
if(DeObPesD == 14341){
DeObPesD = DeObPesD + 502534;
}sbyte KdweEOPAZHCOgPPuTuD =  104; 
int mMXWFIHYSiY = 17071232; 
while(mMXWFIHYSiY == 17071232){
mMXWFIHYSiY += 649490;
}ulong nbiEFwXyDXH = 82833929576981051; 
sbyte TbOtPu =  31; 
uint uNOGqnehsBI = 8; 
string kNMPyuqaG = "waukPbq"; 
double DUiGJfMBP = -12.15626; 
DUiGJfMBP = Math.Pow(2, 2.1);
try{
int UflGZSdNtASozhaiNS = 9466620;
if(UflGZSdNtASozhaiNS == 56071){
UflGZSdNtASozhaiNS++;
}else{
UflGZSdNtASozhaiNS--;
}
}catch(Exception ex){

}byte VGisdD =  212; 
string poSTQfSxD = "jbZpRKponWDJxJXSOpAlWpXWyEjKx"; 
uint muFZeVosIOm = 137099; 
long TjVYsCWqcUdcTnBGTNFo = 4170549614721033; 
uint NQnQKZOyiPqdCfbTxehiu = 545963640; 
ushort bBccTEeTtgmP = 3354; 
int RwEPYkpEScbUKooV = 8835; 
while(RwEPYkpEScbUKooV == 8835){
RwEPYkpEScbUKooV = RwEPYkpEScbUKooV + 726207;
}ushort llkiYOeuBH = 22075; 
double NwhMXkZSJ = -27555.25; 
if(NwhMXkZSJ == -1.505172E+36){
NwhMXkZSJ = Math.Truncate(NwhMXkZSJ);
for( ; ;) {
Console.WriteLine(-1.003059E-28);
}
}sbyte tgbkpqGNDyxoXAEFRa =  99; 
ushort RnP = 56055; 
ulong MqJsQhsiutmmBbs = 4430554130540734; 
sbyte POEDSms =  11; 
float wUfGgFqjlFOuqJHWbtfsdXCibt = 3.188606E+08F; 
uint VKHhWp = 1412; 
short OziE = -7566; 

}public void yUWWlZyBQysWF(){
ulong MJfJeNjNpLUacFFPXPHWVKGU = 86004863901120743; 
int bQcUpInhH = 3507; 
if(bQcUpInhH == 928204){
bQcUpInhH = 220047;
}long gYRUbckORHSnsw = 15437085743334655; 
double OduCWSjfQDiFOtaNRgza = -2.5109E+28; 
if(OduCWSjfQDiFOtaNRgza == 1.187307E-37){
OduCWSjfQDiFOtaNRgza = Math.Ceiling(Math.Acos(0));
for( ; ;) {
Console.WriteLine(-9.468104E+18);
}
}uint HcEFMiahBXNHBtDzoamubZ = 8518; 
ulong DLbSaZQOWISVSfYcpyoQfJtOUe = 56933964584241144; 
int gJXNQNyVGuh = 42; 
if(gJXNQNyVGuh == 842254){
gJXNQNyVGuh = gJXNQNyVGuh + 562478;
}double UqnXVdLdoUgeaAIVHBDZsky = -10.98462; 
UqnXVdLdoUgeaAIVHBDZsky = Math.Truncate(UqnXVdLdoUgeaAIVHBDZsky);
Console.Write(UqnXVdLdoUgeaAIVHBDZsky.ToString());double btpYZmWOKc = 0.0128692; 
btpYZmWOKc = Math.Ceiling(Math.Tanh(0.1));
try{
Console.WriteLine(btpYZmWOKc.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}string GywkkhfNuwSuoLGnHxAQsku = "jDnSEPLY"; 
ulong yHsdSlYEzHIHWhyOBxJnwudlaIU = 43440456819079761; 
double DFnSONNDEGfuQonpBAcEgcjsMwTX = 1.033547E-37; 
while(DFnSONNDEGfuQonpBAcEgcjsMwTX == 0.003023087){
DFnSONNDEGfuQonpBAcEgcjsMwTX = Math.Exp(2);
object uVgtTQPDdlWYswVeS;
uVgtTQPDdlWYswVeS = -2.240043E-06;
Console.WriteLine(uVgtTQPDdlWYswVeS.ToString().ToLower());
}uint nOBUVZCVwXa = 1430785; 
string isUnqmqonEHwDUXUXwPFINEL = "cYHHCDU"; 
int TJEkhcWzlqnEYayccmlzgwPjfoo = 65836925; 
if(TJEkhcWzlqnEYayccmlzgwPjfoo == 734355){
TJEkhcWzlqnEYayccmlzgwPjfoo += 817399;
}long nqN = 7556107667685084; 
string iEzpJgOyUteTOlQzpDNqZjMoZzQW = "COBksCeoKcKUEtYIuYxtcWURNXZ"; 
float pjqRDtG = 4.055157E+10F; 
ushort MjGAdxwQOHGBuERFTKzuD = 14981; 
sbyte StdbaseNDNITp =  -26; 
ulong DieCLKWnMOM = 46621777143669149; 
long uPwDSXHVXnzV = 81568954755240304; 
ulong mHjECGhQdOd = 25590451416885429; 
short AmYbjOtAEznEeDOsoxbOhkK = 22498; 
byte ZyGQwgyCNtkCqRPqApPud =  214; 
float DqnEGoDgRAPVUOchfDP = -8.073011E-29F; 
ulong kZRLu = 4873601536532082; 
float VzReKScTxMmInBcBzPmcEPEieb = 3.857273E+11F; 
short BZomHpEceliguZgwDgecBzRQpgU = 10261; 
double auBDkntaTMWxLaUyuFbKz = 57823.97; 
if(auBDkntaTMWxLaUyuFbKz == -1.02087E-35){
auBDkntaTMWxLaUyuFbKz = Math.Sqrt(4);
try{
int pHpkoRjhigUdsqatoPfSDRi = 4570884;
if(pHpkoRjhigUdsqatoPfSDRi == 90385){
pHpkoRjhigUdsqatoPfSDRi++;
}else{
pHpkoRjhigUdsqatoPfSDRi--;
}
}catch(Exception ex){

}
}string MzbPa = "zyJnRGZR"; 
float qZFkLGeRixSkxAzDjfFRoNeF = 4.161015E+25F; 
string gqxTMXQzpQufukHEqmbiQnxTmcBE = "aETDhLRZZAzKZltLAxegBmtQUg"; 
float DGKylFplBElAPFsODAQbciOGxyt = 6.717997E-24F; 
string aSWxtPbzwWgkWkFqWUGm = "qQSuSdaOlDbyVk"; 

}public void ThZZBbufWXRwRgxSFQlK(){
string bBMa = "jteNaZXyOVGNuWLWyaJjI"; 
ulong xIJCFQgznAqkNRkRxUJJX = 51119741509969522; 
string bwJIfqzsIIysmcLYemjloZgOy = "RNnJCYkJpQejeAXDaa"; 
uint QPctcspRDboVjBBLeYg = 157216; 
int VUVhKnNYfnRObLVHiItfnC = 98; 
if(VUVhKnNYfnRObLVHiItfnC == 246221){
VUVhKnNYfnRObLVHiItfnC = VUVhKnNYfnRObLVHiItfnC + 460309;
}string azldgAgkHDPciHsNzaisXROzyu = "WELboWlmtDYExBYbRqksDH"; 
float loKXjKOGNOkIVBBg = 3.322078E-09F; 
double ByNSYPXh = -1.568041E-36; 
if(ByNSYPXh == -1.248955E+17){
ByNSYPXh = Math.Pow(double.MinValue, double.MaxValue);

}string btwxF = "kNRtkSiwppM"; 
sbyte keGpRALaBs =  39; 
short gCdimmbaWT = -21002; 
byte tRddaXRSPaxdFWUpKwibWLIqhd =  120; 
sbyte okxcXgUXyq =  53; 
uint sBIeIphLmzakXI = 72; 
double zMEBPabIQZsmdzBBsgKcwhXFbNwR = -0.003864861; 
double SMHdaBhsZwDLxpdbSCpPZzcFjH = -2.872253E-21;
zMEBPabIQZsmdzBBsgKcwhXFbNwR = SMHdaBhsZwDLxpdbSCpPZzcFjH / 3;
Console.Write(zMEBPabIQZsmdzBBsgKcwhXFbNwR.ToString());double QpMP = -2.678175E+16; 
if(QpMP == -1.168962E-36){
QpMP = Math.Pow(double.MinValue, double.MaxValue);
int[] NeibNIeiVmQMNlccLKiMKoLBQMCKC = { 6508900 , 21725 } ;
Random SLakcJeZspUhVeixCZTZCnAlozu= new Random();
Console.WriteLine(NeibNIeiVmQMNlccLKiMKoLBQMCKC[SLakcJeZspUhVeixCZTZCnAlozu.Next(0,2)]);
}string wIZueL = "juXMqTRCtHstkCQCu"; 
int PVFnyOsj = 488223; 
if(PVFnyOsj == 76703){
PVFnyOsj = PVFnyOsj + 723114;
}double AbPXHuOeNpSQFGfFRUWGGfBX = 6.832988E+13; 
while(AbPXHuOeNpSQFGfFRUWGGfBX == -5.50488E-05){
AbPXHuOeNpSQFGfFRUWGGfBX = Math.Ceiling(Math.Tan(1));
object UtqTjPtktCSYXcLIQcjE;
UtqTjPtktCSYXcLIQcjE = -4.75031E+32;
}float eMYNFNqJVnWfPhQCTFbDNY = -1.80423E+17F; 
short ImDIkanTnoAp = 23888; 
double wiFWwhQWgOMwpYgulwmfegm = -7.158078E+31; 
wiFWwhQWgOMwpYgulwmfegm = Math.Ceiling(Math.Sinh(-5));
object ElWwe;
ElWwe = -8.860327E+30;ushort qUaMGPIfnecUlPsYUU = 20466; 
long nxoRnQwEDVECXKK = 9174112112930779; 
int wHgeuUwKfcyLUs = 238051304; 
if(wHgeuUwKfcyLUs == 179718){
wHgeuUwKfcyLUs = wHgeuUwKfcyLUs + 490444;
}short fJMJeafFlZ = 14571; 
ushort jnAiaiSRHlCqyqpspnddgCztLQ = 25901; 
int RzZzAPYcEiqByAxkq = 66942300; 
if(RzZzAPYcEiqByAxkq == 916163){
RzZzAPYcEiqByAxkq += 720328;
}long MIQgykOnpPOC = 41598338316371223; 
uint bchtiMOnVNsfNghWfXDmh = 66191566; 
long JgxXiGUQdHOYebc = 19904628365205864; 
byte IhkoCstAVBGqp =  190; 
sbyte oKBuXbVBOfQIzKy =  50; 
byte BzKCCnuziCyhMhUNAC =  232; 
float HlKCxDMaldVfMl = 1.157161E+19F; 

}public void hlZzSudGtJNMjWPwiUmZZfM(){
long XzCcZVlZMtJRXOR = 38270678086513871; 
sbyte EqwOOcBsaUWCEtNBNzEfF =  -75; 
sbyte secRJjWqwsxEQ =  -71; 
double izXUVfqJgzC = 31325.34; 
if(izXUVfqJgzC == -5.76157E-12){
izXUVfqJgzC = Math.Ceiling(Math.Cos(2));
Console.WriteLine(izXUVfqJgzC.ToString());
}int AiFSLZTRUKkWzqhqkmIIMwIpxsmZt = 6714; 
while(AiFSLZTRUKkWzqhqkmIIMwIpxsmZt == 6714){
AiFSLZTRUKkWzqhqkmIIMwIpxsmZt = AiFSLZTRUKkWzqhqkmIIMwIpxsmZt + 300323;
}double yofTgUpSwFJ = 3.905363E+12; 
if(yofTgUpSwFJ == 5.552265E-18){
yofTgUpSwFJ = Math.Pow(2, 2.1);
object ybNFcKZOeOlly;
ybNFcKZOeOlly = 7.847998E+07;
}string FIOJDWMh = "DiRuGhROFmNxNPMYLbiQ"; 
double dyRhXobxR = -2.978604E+08; 
while(dyRhXobxR == 9.629637E-18){
double iULbmOSSbK = -1.358066E+12;
dyRhXobxR = Math.Ceiling(iULbmOSSbK);
for( ; ;) {
Console.WriteLine(1.753811E+30);
}
}string emsFWPnTsjQ = "TaHiD"; 
int YILEUPiGMJQcjAgfotEAFDpOnZ = 865610476; 
if(YILEUPiGMJQcjAgfotEAFDpOnZ == 871794){
YILEUPiGMJQcjAgfotEAFDpOnZ += 962691;
}ushort eItKyPOiDkCg = 39844; 
int VsPZgnwsXpVSpKLPSHTuAM = 917969; 
if(VsPZgnwsXpVSpKLPSHTuAM == 415703){
VsPZgnwsXpVSpKLPSHTuAM = VsPZgnwsXpVSpKLPSHTuAM + 10389;
}sbyte htqhaIdYp =  -111; 
long xtCXzARwKBN = 38514115034386218; 
ushort KiDCiJcMT = 19780; 
ulong ioltRmWjLEIHeVSVjNVko = 40554892316148509; 
long kXCdUCBuhVkCgCVxmRhCfuEw = 58976289928582665; 
string yJESXuKtNodCGKeZcYHhKQt = "XqAfY"; 
byte QCtelSwTNRNHoqEXqpxUhPSc =  160; 
short UHUEuDAnalGSgIe = 24085; 
short ieOtuiQDpIaJqyXuBmTnsePdELMy = -12394; 
byte WCfy =  51; 
long mMNSlSnsVhVMgSad = 25076658965488051; 
byte Gegk =  254; 
ulong ZagBBSWQlQFDFfjWzBtUZ = 16808241532130962; 
byte hibhSTDeluVFTRQIXwiJL =  160; 
float oxRfUJKHafxuDYscVxgBdf = 0.001578992F; 
int lRaDZCppSwbyjEKoENwwfYdfNnAs = 17220153; 
if(lRaDZCppSwbyjEKoENwwfYdfNnAs == 509968){
lRaDZCppSwbyjEKoENwwfYdfNnAs = 940977;
}ushort HzgVelfWbzmEQPCXdjSmOUQMbY = 44436; 
long IXmgRIfHAqTTgdnbWCSHP = 77156173396932869; 
float DxntCa = -2.638549E+35F; 
sbyte qETZuIQCKexRcU =  -39; 
int WNVylGk = 4320; 
if(WNVylGk == 754130){
WNVylGk = WNVylGk + 24647;
}uint LkyMzfPyWgLPKMttbV = 512520925; 
byte YfYIJYOfO =  170; 

}
} 