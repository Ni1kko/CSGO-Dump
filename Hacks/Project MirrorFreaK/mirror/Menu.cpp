#include "Menu.h"
#include "Controls.h"
#include "Hooks.h" 
#include "Interfaces.h"
#include "CRC32.h"
#include <fstream>
#include "XorStr.hpp"
#define WINDOW_WIDTH 575
#define WINDOW_HEIGHT 510 // 507
mirror_window Options::Menu;
struct Config_t {
	int id;
	std::string name;
};
std::vector<Config_t> configs;
typedef void(__cdecl* MsgFn)(const char* msg, va_list);
void MsgX(const char* msg, ...)
{
	if (msg == nullptr)
		return; //If no string was passed, or it was null then don't do anything
	static MsgFn fn = (MsgFn)GetProcAddress(GetModuleHandle("tier0.dll"), "Msg"); 	char buffer[989];
	va_list list;
	va_start(list, msg);
	vsprintf(buffer, msg, list);
	va_end(list);
	fn(buffer, list); //Calls the function, we got the address above.
}
void save_callback()
{
	int should_save = Options::Menu.ColorsTab.ConfigListBox.GetIndex();
	std::string config_directory = "miroawr\\cfg\\";
	config_directory += configs[should_save].name; config_directory += ".xml";
	GUI.SaveWindowState(&Options::Menu, XorStr(config_directory.c_str()));
	Interfaces::CVar->ConsoleColorPrintf(Color(140, 10, 250, 255), "Mirror v6 ");
	std::string uremam = "Saved configuration.     \n";
	MsgX(uremam.c_str());
}
void load_callback()
{
	int should_load = Options::Menu.ColorsTab.ConfigListBox.GetIndex();
	std::string config_directory = "miroawr\\cfg\\";
	config_directory += configs[should_load].name; config_directory += ".xml";
	GUI.LoadWindowState(&Options::Menu, XorStr(config_directory.c_str()));
	Interfaces::CVar->ConsoleColorPrintf(Color(140, 10, 250, 255), "Mirror v6 ");
	std::string uremam = "Loaded configuration.     \n";
	MsgX(uremam.c_str());
}

void list_configs() {
	configs.clear();
	Options::Menu.ColorsTab.ConfigListBox.ClearItems();
	std::ifstream file_in;
	file_in.open("miroawr\\cfg\\miroawr_configs.txt");
	if (file_in.fail()) {
		std::ofstream("miroawr\\cfg\\miroawr_configs.txt");
		file_in.open("miroawr\\cfg\\miroawr_configs.txt");
	}
	int line_count;
	while (!file_in.eof()) {
		Config_t config;
		file_in >> config.name;
		config.id = line_count;
		configs.push_back(config);
		line_count++;
		Options::Menu.ColorsTab.ConfigListBox.AddItem(config.name);
	}
	file_in.close();
	if (configs.size() > 7) Options::Menu.ColorsTab.ConfigListBox.AddItem(" ");
}

void add_config() {
	std::fstream file;
	file.open("miroawr\\cfg\\miroawr_configs.txt", std::fstream::app);
	if (file.fail()) {
		std::fstream("miroawr\\cfg\\miroawr_configs.txt");
		file.open("miroawr\\cfg\\miroawr_configs.txt", std::fstream::app);
	}
	file << std::endl << Options::Menu.ColorsTab.NewConfigName.getText();
	file.close();
	list_configs();
	int should_add = Options::Menu.ColorsTab.ConfigListBox.GetIndex();
	std::string config_directory = "miroawr\\cfg\\";
	config_directory += Options::Menu.ColorsTab.NewConfigName.getText(); config_directory += ".xml";
	GUI.SaveWindowState(&Options::Menu, XorStr(config_directory.c_str()));
	Options::Menu.ColorsTab.NewConfigName.SetText("");
}

void remove_config() {
	int should_remove = Options::Menu.ColorsTab.ConfigListBox.GetIndex();
	std::string config_directory = "miroawr\\cfg\\";
	config_directory += configs[should_remove].name; config_directory += ".xml";
	std::remove(config_directory.c_str());
	std::ofstream ofs("miroawr\\cfg\\miroawr_configs.txt", std::ios::out | std::ios::trunc);
	ofs.close();
	std::fstream file;
	file.open("miroawr\\cfg\\miroawr_configs.txt", std::fstream::app);
	if (file.fail()) {
		std::fstream("miroawr\\cfg\\miroawr_configs.txt");
		file.open("miroawr\\cfg\\miroawr_configs.txt", std::fstream::app);
	}
	for (int i = 0; i < configs.size(); i++) {
		if (i == should_remove) continue;
		Config_t config = configs[i];
		file << std::endl << config.name;
	}
	file.close();
	list_configs();
}

void KnifeApplyCallbk()
{
	static ConVar* Meme = Interfaces::CVar->FindVar("cl_fullupdate");
	Meme->nFlags &= ~FCVAR_CHEAT;
	Interfaces::Engine->ClientCmd_Unrestricted("cl_fullupdate");
}

void UnLoadCallbk()
{
	DoUnload = true;
}
void mirror_window::Setup()
{
	SetPosition(350, 50);
	SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	SetTitle("");
	RegisterTab(&RageBotTab);
	//	RegisterTab(&LegitBotTab);
	RegisterTab(&VisualsTab);
	RegisterTab(&MiscTab);
	RegisterTab(&ColorsTab);

	RECT Client = GetClientArea();
	Client.bottom -= 29;
	RageBotTab.Setup();
	//	LegitBotTab.Setup();
	VisualsTab.Setup();
	MiscTab.Setup();
	ColorsTab.Setup();

#pragma region Bottom Buttons
#pragma endregion
}
void CRageBotTab::Setup()
{
	SetTitle("J");
#pragma region Aimbot
	AimbotGroup.SetPosition(4, 25); // 15, 25
	AimbotGroup.SetText("Aimbot");
	AimbotGroup.SetSize(550, 210);
	AimbotGroup.AddTab(CGroupTab("Main", 1));
	AimbotGroup.AddTab(CGroupTab("Accuracy", 2));
	AimbotGroup.AddTab(CGroupTab("BodyAim", 3));
	//	AimbotGroup.AddTab(CGroupTab("Resolver Mods", 4));
	RegisterControl(&AimbotGroup);

	AimbotEnable.SetFileId("aim_enable");
	AimbotGroup.PlaceLabledControl(1, "Enable", this, &AimbotEnable);

	AimbotAutoFire.SetFileId("aim_autofire");
	AimbotAutoFire.SetState(true);

	AimbotSilentAim.SetFileId("aim_silent");
	AimbotSilentAim.SetState(true);

	AccuracyAutoScope.SetFileId("acc_scope");
	AimbotGroup.PlaceLabledControl(1, "Auto Scope", this, &AccuracyAutoScope);

	lag_pred.SetFileId("lag_pred");
	AimbotGroup.PlaceLabledControl(1, "Position Adjustment", this, &lag_pred);

	delay_shot.SetFileId("delay_shot");
	AimbotGroup.PlaceLabledControl(1, "Delay Shot", this, &delay_shot);

	AccuracyHitchance.SetFileId("base_hc");
	AccuracyHitchance.SetBoundaries(0.f, 100.f);
	AccuracyHitchance.SetValue(20);

	AccuracyMinimumDamage.SetFileId("base_md");
	AccuracyMinimumDamage.SetBoundaries(0.f, 100.f);
	AccuracyMinimumDamage.SetValue(20);

	preso.SetFileId("acc_zeusisgay");
	preso.AddItem("Default");
	preso.AddItem("Down");
	preso.AddItem("Up");
	preso.AddItem("Zero");
	preso.AddItem("Automatic Spread");
	AimbotGroup.PlaceLabledControl(1, "Pitch Adjustment", this, &preso);

	resolver.SetFileId("acc_aaa");
	resolver.AddItem("Default");
	resolver.AddItem("Desync Prediction");
	resolver.AddItem("Auto Direction Brute");
	AimbotGroup.PlaceLabledControl(1, "Yaw Adjustment", this, &resolver);

	AimbotFov.SetFileId("aim_fov");
	AimbotFov.SetBoundaries(0.f, 180.f);
	AimbotFov.extension = XorStr("°");
	AimbotFov.SetValue(180.f);

	extrapolation.SetFileId("acc_extra_P_lation");
	AimbotGroup.PlaceLabledControl(2, "Extrapolation", this, &extrapolation);

	TargetPointscale.SetFileId("acc_hitbox_Scale");
	TargetPointscale.SetBoundaries(0, 100);
	TargetPointscale.SetValue(50);
	TargetPointscale.extension = ("%%");
	AimbotGroup.PlaceLabledControl(2, "Hitbox Scale", this, &TargetPointscale);

	Multienable.SetFileId("multipoint_enable");
	AimbotGroup.PlaceLabledControl(2, "Toggle Multipoint", this, &Multienable);

	Multival2.SetFileId("hitbox_scale_head");
	Multival2.SetBoundaries(0.1, 100);
	Multival2.SetValue(20);
	Multival2.extension = XorStr("%%");
	AimbotGroup.PlaceLabledControl(2, "Head Multipoint", this, &Multival2);

	Multival.SetFileId("hitbox_scale_body");
	Multival.SetBoundaries(0.1, 100);
	Multival.SetValue(20);
	Multival.extension = XorStr("%%");
	AimbotGroup.PlaceLabledControl(2, "Body Multipoint", this, &Multival);

	flip180.SetFileId("flip180");
	AimbotGroup.PlaceLabledControl(2, "Flip 180", this, &flip180);

	baim_fake.SetFileId("bodyaim_fake");
	AimbotGroup.PlaceLabledControl(3, "If Fake", this, &baim_fake); // if we have to resort to a brute

	baim_fakewalk.SetFileId("bodyaim_fakewalk");
	AimbotGroup.PlaceLabledControl(3, "If Slow Walk", this, &baim_fakewalk);

	baim_inair.SetFileId("bodyaim_inair");
	AimbotGroup.PlaceLabledControl(3, "If In Air", this, &baim_inair); //if they be flyin like a plane

	baim_muslim.SetFileId("bodyaim_if_muslim");
	AimbotGroup.PlaceLabledControl(3, "If Is Muslim", this, &baim_muslim);
	BaimIfUnderXHealth.SetFileId("acc_BaimIfUnderXHealth");
	BaimIfUnderXHealth.SetBoundaries(0, 100);
	BaimIfUnderXHealth.extension = XorStr("HP");
	BaimIfUnderXHealth.SetValue(0);
	AimbotGroup.PlaceLabledControl(3, "If HP Lower Than", this, &BaimIfUnderXHealth);

	bigbaim.SetFileId("acc_bigbaim");
	AimbotGroup.PlaceLabledControl(3, "On Key", this, &bigbaim);

	// -<--------------------------------------------------------------->- //

	// -<--------------------------------------------------------------->- //

	weapongroup.SetText("Main");
	weapongroup.SetPosition(4, 245); // 15, 230
	weapongroup.SetSize(550, 195);
	weapongroup.AddTab(CGroupTab("Auto Sniper", 1));
	weapongroup.AddTab(CGroupTab("Pistols", 2));
	weapongroup.AddTab(CGroupTab("Scout", 3));
	weapongroup.AddTab(CGroupTab("Awp", 4));
	weapongroup.AddTab(CGroupTab("Others", 5));
	RegisterControl(&weapongroup);

	target_auto.SetFileId("tgt_hitbox_auto");
	target_auto.items.push_back(dropdownboxitem(false, XorStr("Head")));
	target_auto.items.push_back(dropdownboxitem(false, XorStr("Upper Body")));
	target_auto.items.push_back(dropdownboxitem(false, XorStr("Lower Body")));
	target_auto.items.push_back(dropdownboxitem(false, XorStr("Arms")));
	target_auto.items.push_back(dropdownboxitem(false, XorStr("Legs")));
	weapongroup.PlaceLabledControl(1, "HitScan", this, &target_auto);

	/*	target_scout2.SetFileId("tgt_hitscan_auto");
	target_scout2.AddItem("Off");
	target_scout2.AddItem("Minimal");
	target_scout2.AddItem("Essential");
	target_scout2.AddItem("Maximal");
	weapongroup.PlaceLabledControl(1, "Hitscan", this, &target_scout2); */

	hc_auto.SetFileId("auto_hitchance");
	hc_auto.SetBoundaries(0, 100);
	hc_auto.SetValue(25);
	hc_auto.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(1, "Hitchance", this, &hc_auto);

	md_auto.SetFileId("auto_minimumdamage");
	md_auto.SetBoundaries(0, 100);
	md_auto.SetValue(25);
	md_auto.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(1, "Minimum Damage", this, &md_auto);

	//	preset_auto.SetFileId("auto_automatic_cfg");
	//	weapongroup.PlaceLabledControl(1, "Automatic Auto Sniper Configuration", this, &preset_auto);
	//----------------------------------------------------------------------

	target_pistol.SetFileId("tgt_hitbox_pistol");
	target_pistol.items.push_back(dropdownboxitem(false, XorStr("Head")));
	target_pistol.items.push_back(dropdownboxitem(false, XorStr("Upper Body")));
	target_pistol.items.push_back(dropdownboxitem(false, XorStr("Lower Body")));
	target_pistol.items.push_back(dropdownboxitem(false, XorStr("Arms")));
	target_pistol.items.push_back(dropdownboxitem(false, XorStr("Legs")));
	weapongroup.PlaceLabledControl(2, "HitScan", this, &target_pistol);

	hc_pistol.SetFileId("pistol_hitchance");
	hc_pistol.SetBoundaries(0, 100);
	hc_pistol.SetValue(25);
	hc_pistol.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(2, "Hitchance", this, &hc_pistol);

	md_pistol.SetFileId("pistol_minimumdamage");
	md_pistol.SetBoundaries(0, 100);
	md_pistol.SetValue(25);
	md_pistol.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(2, "Minimum Damage", this, &md_pistol);


	//	preset_pistol.SetFileId("pistol_automatic_cfg");
	//	weapongroup.PlaceLabledControl(2, "Automatic Pistol Configuration", this, &preset_pistol);

	//----------------------------------------------------------------------

	target_scout.SetFileId("tgt_hitbox_scout");
	target_scout.items.push_back(dropdownboxitem(false, XorStr("Head")));
	target_scout.items.push_back(dropdownboxitem(false, XorStr("Upper Body")));
	target_scout.items.push_back(dropdownboxitem(false, XorStr("Lower Body")));
	target_scout.items.push_back(dropdownboxitem(false, XorStr("Arms")));
	target_scout.items.push_back(dropdownboxitem(false, XorStr("Legs")));
	weapongroup.PlaceLabledControl(3, "HitScan", this, &target_scout);

	hc_scout.SetFileId("scout_hitchance");
	hc_scout.SetBoundaries(0, 100);
	hc_scout.SetValue(25);
	hc_scout.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(3, "Hitchance", this, &hc_scout);

	md_scout.SetFileId("scout_minimumdamage");
	md_scout.SetBoundaries(0, 100);
	md_scout.SetValue(25);
	md_scout.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(3, "Minimum Damage", this, &md_scout);

//	headonly_if_vis_scout.SetFileId("headonly_if_vis_scout");
//	weapongroup.PlaceLabledControl(3, "Headshot Only If Hittable", this, &headonly_if_vis_scout);

	//	preset_scout.SetFileId("scout_automatic_cfg");
	//	weapongroup.PlaceLabledControl(3, "Automatic Scout Configuration", this, &preset_scout);
	//----------------------------------------------------------------------

	target_awp.SetFileId("tgt_hitbox_awp");
	target_awp.items.push_back(dropdownboxitem(false, XorStr("Head")));
	target_awp.items.push_back(dropdownboxitem(false, XorStr("Upper Body")));
	target_awp.items.push_back(dropdownboxitem(false, XorStr("Lower Body")));
	target_awp.items.push_back(dropdownboxitem(false, XorStr("Arms")));
	target_awp.items.push_back(dropdownboxitem(false, XorStr("Legs")));
	weapongroup.PlaceLabledControl(4, "HitScan", this, &target_awp);

	hc_awp.SetFileId("awp_hitchance");
	hc_awp.SetBoundaries(0, 100);
	hc_awp.SetValue(25);
	hc_awp.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(4, "Hitchance", this, &hc_awp);

	md_awp.SetFileId("awp_minimumdamage");
	md_awp.SetBoundaries(0, 100);
	md_awp.SetValue(25);
	md_awp.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(4, "Minimum Damage", this, &md_awp);

//	headonly_if_vis_awp.SetFileId("headonly_if_vis_awp");
//	weapongroup.PlaceLabledControl(4, "Headshot Only If Hittable", this, &headonly_if_vis_awp);

	//	preset_awp.SetFileId("awp_automatic_cfg");
	//	weapongroup.PlaceLabledControl(4, "Automatic AWP Configuration", this, &preset_awp);

	//----------------------------------------------------------------------

	target_otr.SetFileId("tgt_hitbox_otr");
	target_otr.items.push_back(dropdownboxitem(false, XorStr("Head")));
	target_otr.items.push_back(dropdownboxitem(false, XorStr("Upper Body")));
	target_otr.items.push_back(dropdownboxitem(false, XorStr("Lower Body")));
	target_otr.items.push_back(dropdownboxitem(false, XorStr("Arms")));
	target_otr.items.push_back(dropdownboxitem(false, XorStr("Legs")));
	weapongroup.PlaceLabledControl(5, "HitScan", this, &target_otr);

	hc_otr.SetFileId("otr_hitchance");
	hc_otr.SetBoundaries(0, 100);
	hc_otr.SetValue(25);
	hc_otr.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(5, "Hitchance", this, &hc_otr);

	md_otr.SetFileId("otr_minimumdamage");
	md_otr.SetBoundaries(0, 100);
	md_otr.SetValue(25);
	md_otr.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(5, "Minimum Damage", this, &md_otr);

//	headonly_if_vis_otr.SetFileId("headonly_if_vis_otr");
//	weapongroup.PlaceLabledControl(5, "Headshot Only If Hittable", this, &headonly_if_vis_otr);

	//	preset_otr.SetFileId("otr_automatic_cfg");
	//	weapongroup.PlaceLabledControl(5, "Automatic  Weapon Configuration", this, &preset_otr);
#pragma endregion  AntiAim controls get setup in here
}
/*
void CLegitBotTab::Setup()
{	/*
_      ______ _____ _____ _______
| |    |  ____/ ____|_   _|__   __|
| |    | |__ | |  __  | |    | |
| |    |  __|| | |_ | | |    | |
| |____| |___| |__| |_| |_   | |
|______|______\_____|_____|  |_|


SetTitle("B");
AimbotGroup.SetText("Main");
AimbotGroup.SetPosition(4, 25);
AimbotGroup.SetSize(550, 222);
AimbotGroup.AddTab(CGroupTab("Aimbot", 1));
AimbotGroup.AddTab(CGroupTab("Triggerbot", 2));
AimbotGroup.AddTab(CGroupTab("Filters", 3));
RegisterControl(&AimbotGroup);
RegisterControl(&Active);
Active.SetFileId("active");
AimbotGroup.PlaceLabledControl(1, "Activate Legitbot", this, &Active);
AimbotEnable.SetFileId("l_aimbot");
AimbotGroup.PlaceLabledControl(1, "Aimbot Enable", this, &AimbotEnable);
aimbotfiremode.SetFileId("l_autoaimbot");
aimbotfiremode.AddItem("On Shot");
aimbotfiremode.AddItem("Automatic");
AimbotGroup.PlaceLabledControl(1, "Fire Mode", this, &aimbotfiremode);
AimbotKeyBind.SetFileId("l_aimkey");
AimbotGroup.PlaceLabledControl(1, "KeyBind", this, &AimbotKeyBind);
BackTrack.SetFileId("l_backtrack");
AimbotGroup.PlaceLabledControl(1, "Backtrack", this, &BackTrack);
AimbotSmokeCheck.SetFileId("l_smokeaimbot");
AimbotGroup.PlaceLabledControl(1, "Smoke Check", this, &AimbotSmokeCheck);

legitresolver.SetFileId("l_B1GresolverTappingSkeet");
AimbotGroup.PlaceLabledControl(1, "Resolver", this, &legitresolver);

//---- Trigger ---//
TriggerEnable.SetFileId("t_triggerbotenable");
AimbotGroup.PlaceLabledControl(2, "Activate Trigger", this, &TriggerEnable);
triggertype.SetFileId("t_triggerbottyp");
triggertype.AddItem("None");
triggertype.AddItem("Automatic");
AimbotGroup.PlaceLabledControl(2, "Trigger Mode", this, &triggertype);
TriggerHitChanceAmmount.SetFileId("l_trigHC");
TriggerHitChanceAmmount.SetBoundaries(0, 100);
TriggerHitChanceAmmount.extension = XorStr("%%");
TriggerHitChanceAmmount.SetValue(0);
AimbotGroup.PlaceLabledControl(2, "Hitchance", this, &TriggerHitChanceAmmount);
TriggerSmokeCheck.SetFileId("l_trigsmoke");
AimbotGroup.PlaceLabledControl(2, "Smoke Check", this, &TriggerSmokeCheck);
TriggerRecoil.SetFileId("l_trigRCS");
TriggerRecoil.SetBoundaries(0.f, 100.f);
TriggerRecoil.extension = XorStr("%%");
TriggerRecoil.SetValue(5.00f);
AimbotGroup.PlaceLabledControl(2, "Recoil", this, &TriggerRecoil);
TriggerKeyBind.SetFileId("l_trigkey");
AimbotGroup.PlaceLabledControl(2, "KeyBind", this, &TriggerKeyBind);
// ---- Hitboxes ---- //
TriggerHead.SetFileId("l_trighead");
AimbotGroup.PlaceLabledControl(3, "Head", this, &TriggerHead);
TriggerChest.SetFileId("l_trigchest");
AimbotGroup.PlaceLabledControl(3, "Chest", this, &TriggerChest);
TriggerStomach.SetFileId("l_trigstomach");
AimbotGroup.PlaceLabledControl(3, "Stomach", this, &TriggerStomach);
TriggerArms.SetFileId("l_trigarms");
AimbotGroup.PlaceLabledControl(3, "Arms", this, &TriggerArms);
TriggerLegs.SetFileId("l_triglegs");
AimbotGroup.PlaceLabledControl(3, "Legs", this, &TriggerLegs);

//--------------------------legitaa--------------------------((
/*	aaenable.SetFileId("AA_anable");
AimbotGroup.PlaceLabledControl(4, "Legit AA Enable", this, &aaenable);

aatyp.SetFileId("AA_aatyp");
aatyp.AddItem("Static");
aatyp.AddItem("Lowerbody");
aatyp.AddItem("Freestanding");
AimbotGroup.PlaceLabledControl(4, "Anti-Aim", this, &aatyp);

aatyp2.SetFileId("AA_aatyp2");
aatyp2.AddItem("Default");
aatyp2.AddItem("Jitter");
aatyp2.AddItem("Shuffle");
aatyp2.AddItem("Spin");
AimbotGroup.PlaceLabledControl(4, "Anti-Aim Type", this, &aatyp2);

aatyp3.SetFileId("AA_aatyp3");
aatyp3.SetBoundaries(0, 90);
aatyp3.SetValue(20);
AimbotGroup.PlaceLabledControl(4, "Anti-Aim Value", this, &aatyp3);

aafl.SetFileId("AA_aaFL");
aafl.SetBoundaries(1, 4);
aafl.SetValue(1);
AimbotGroup.PlaceLabledControl(4, "Legit FakeLag", this, &aafl); */
//----------------------solid kys---------------------//
/*
weapongroup.SetText("Main");
weapongroup.SetPosition(4, 256);
weapongroup.SetSize(550, 183);
weapongroup.AddTab(CGroupTab("Rifle", 1));
weapongroup.AddTab(CGroupTab("Pistol", 2));
weapongroup.AddTab(CGroupTab("Sniper", 3));
weapongroup.AddTab(CGroupTab("SMG", 4));
weapongroup.AddTab(CGroupTab("Heavy", 5));
RegisterControl(&weapongroup);
RegisterControl(&Active);
WeaponMainHitbox.SetFileId("l_rhitbox");
WeaponMainHitbox.AddItem("Head");
WeaponMainHitbox.AddItem("Neck");
WeaponMainHitbox.AddItem("Chest");
WeaponMainHitbox.AddItem("Stomach");
WeaponMainHitbox.AddItem("Nearest");
weapongroup.PlaceLabledControl(1, "Hitbox", this, &WeaponMainHitbox);
WeaponMainSpeed.SetFileId("l_rspeed");
WeaponMainSpeed.SetBoundaries(0, 75);
WeaponMainSpeed.SetValue(5);
weapongroup.PlaceLabledControl(1, "Speed", this, &WeaponMainSpeed);
WeaponMainRecoil.SetFileId("l_rRecoil");
WeaponMainRecoil.SetBoundaries(0, 200);
WeaponMainRecoil.SetValue(165);
weapongroup.PlaceLabledControl(1, "Recoil", this, &WeaponMainRecoil);
WeaponMainFoV.SetFileId("l_fov");
WeaponMainFoV.SetBoundaries(0, 45);
WeaponMainFoV.SetValue(10);
weapongroup.PlaceLabledControl(1, "Field Of View", this, &WeaponMainFoV);
// --- Pistols --- //
WeaponPistHitbox.SetFileId("l_phitbox");
WeaponPistHitbox.AddItem("Head");
WeaponPistHitbox.AddItem("Neck");
WeaponPistHitbox.AddItem("Chest");
WeaponPistHitbox.AddItem("Stomach");
WeaponPistHitbox.AddItem("Nearest");
weapongroup.PlaceLabledControl(2, "Hitbox", this, &WeaponPistHitbox);
WeaponPistSpeed.SetFileId("l_pspeed");
WeaponPistSpeed.SetBoundaries(0, 75);
WeaponPistSpeed.SetValue(5);
weapongroup.PlaceLabledControl(2, "Speed", this, &WeaponPistSpeed);
WeaponPistRecoil.SetFileId("l_pRecoil");
WeaponPistRecoil.SetBoundaries(0, 200);
WeaponPistRecoil.SetValue(165);
weapongroup.PlaceLabledControl(2, "Recoil", this, &WeaponPistRecoil);
WeaponPistFoV.SetFileId("l_pfov");
WeaponPistFoV.SetBoundaries(0, 45);
WeaponPistFoV.SetValue(10);
weapongroup.PlaceLabledControl(2, "Field Of View", this, &WeaponPistFoV);
// --- Sniper --- //
WeaponSnipHitbox.SetFileId("l_shitbox");
WeaponSnipHitbox.AddItem("Head");
WeaponSnipHitbox.AddItem("Neck");
WeaponSnipHitbox.AddItem("Chest");
WeaponSnipHitbox.AddItem("Stomach");
WeaponSnipHitbox.AddItem("Nearest");
weapongroup.PlaceLabledControl(3, "Hitbox", this, &WeaponSnipHitbox);
WeaponSnipSpeed.SetFileId("l_sspeed");
WeaponSnipSpeed.SetBoundaries(0, 75);
WeaponSnipSpeed.SetValue(5);
weapongroup.PlaceLabledControl(3, "Speed", this, &WeaponSnipSpeed);
WeaponSnipRecoil.SetFileId("l_sRecoil");
WeaponSnipRecoil.SetBoundaries(0, 200);
WeaponSnipRecoil.SetValue(165);
weapongroup.PlaceLabledControl(3, "Recoil", this, &WeaponSnipRecoil);
WeaponSnipFoV.SetFileId("l_sfov");
WeaponSnipFoV.SetBoundaries(0, 45);
WeaponSnipFoV.SetValue(10);
weapongroup.PlaceLabledControl(3, "Field Of View", this, &WeaponSnipFoV);
// --- SMG --- //
WeaponMpHitbox.SetFileId("l_sniphitbox");
WeaponMpHitbox.AddItem("Head");
WeaponMpHitbox.AddItem("Neck");
WeaponMpHitbox.AddItem("Chest");
WeaponMpHitbox.AddItem("Stomach");
WeaponMpHitbox.AddItem("Nearest");
weapongroup.PlaceLabledControl(4, "Hitbox", this, &WeaponMpHitbox);
WeaponMpSpeed.SetFileId("l_sspeed");
WeaponMpSpeed.SetBoundaries(0, 75);
WeaponMpSpeed.SetValue(5);
weapongroup.PlaceLabledControl(4, "Speed", this, &WeaponMpSpeed);
WeaponMpRecoil.SetFileId("l_sRecoil");
WeaponMpRecoil.SetBoundaries(0, 200);
WeaponMpRecoil.SetValue(165);
weapongroup.PlaceLabledControl(4, "Recoil", this, &WeaponMpRecoil);
WeaponMpFoV.SetFileId("l_sfov");
WeaponMpFoV.SetBoundaries(0, 45);
WeaponMpFoV.SetValue(10);
weapongroup.PlaceLabledControl(4, "Field Of View", this, &WeaponMpFoV);
// --- MachineGun --- //
WeaponMGHitbox.SetFileId("l_mghitbox");
WeaponMGHitbox.AddItem("Head");
WeaponMGHitbox.AddItem("Neck");
WeaponMGHitbox.AddItem("Chest");
WeaponMGHitbox.AddItem("Stomach");
WeaponMGHitbox.AddItem("Nearest");
weapongroup.PlaceLabledControl(5, "Hitbox", this, &WeaponMGHitbox);
WeaponMGSpeed.SetFileId("l_mgspeed");
WeaponMGSpeed.SetBoundaries(0, 75);
WeaponMGSpeed.SetValue(5);
weapongroup.PlaceLabledControl(5, "Speed", this, &WeaponMGSpeed);
WeaponMGRecoil.SetFileId("l_mgRecoil");
WeaponMGRecoil.SetBoundaries(0, 200);
WeaponMGRecoil.SetValue(165);
weapongroup.PlaceLabledControl(5, "Recoil", this, &WeaponMGRecoil);
WeaponMGFoV.SetFileId("l_mgfov");
WeaponMGFoV.SetBoundaries(0, 45);
WeaponMGFoV.SetValue(10);
weapongroup.PlaceLabledControl(5, "Field Of View", this, &WeaponMGFoV);
}
*/
void CVisualTab::Setup()
{

	SetTitle("D");
#pragma region Options
	OptionsGroup.SetText("Options");
	OptionsGroup.SetPosition(4, 30);
	OptionsGroup.SetSize(280, 410);
	OptionsGroup.AddTab(CGroupTab("Main", 1));
	OptionsGroup.AddTab(CGroupTab("Filters", 2));
	OptionsGroup.AddTab(CGroupTab("Misc", 3));
	OptionsGroup.AddTab(CGroupTab("Other", 4));
	RegisterControl(&OptionsGroup);
	RegisterControl(&Active);

	Active.SetFileId("active");
	OptionsGroup.PlaceLabledControl(1, "Activate Visuals", this, &Active);

	OptionsBox.SetFileId("opt_box");
	OptionsBox.AddItem("Off");
	OptionsBox.AddItem("Default");
	OptionsBox.AddItem("Genuine");
	OptionsBox.AddItem("Corners");
	OptionsGroup.PlaceLabledControl(1, "Box", this, &OptionsBox);

	OptionsName.SetFileId("opt_name");
	OptionsName.AddItem("Off");
	OptionsName.AddItem("Top");
	OptionsName.AddItem("Right");
	OptionsGroup.PlaceLabledControl(1, "Name", this, &OptionsName);

	OptionsHealth.SetFileId("opt_hp");
	OptionsHealth.AddItem("Off");
	OptionsHealth.AddItem("Default");
	OptionsHealth.AddItem("Battery");
	OptionsHealth.AddItem("Bottom");

	OptionsGroup.PlaceLabledControl(1, "Health", this, &OptionsHealth);

	OptionsArmor.SetFileId("otr_armor");
	OptionsArmor.AddItem("Off");
	OptionsArmor.AddItem("Default");
	OptionsArmor.AddItem("Battery");
	OptionsArmor.AddItem("Bottom");
	OptionsGroup.PlaceLabledControl(1, "Armor Bar", this, &OptionsArmor); // here

	OptionsInfo.SetFileId("opt_info");
	OptionsGroup.PlaceLabledControl(1, "Info", this, &OptionsInfo);

	OptionsSkeleton.SetFileId("opt_bone");
	OptionsGroup.PlaceLabledControl(1, "Skeleton", this, &OptionsSkeleton);

	Weapons.SetFileId("kysquecest");
	OptionsGroup.PlaceLabledControl(1, "Weapons", this, &Weapons);

	Ammo.SetFileId("urmomsucksass");
	OptionsGroup.PlaceLabledControl(1, "Ammo Bar", this, &Ammo);

	//	OffscreenESP.SetFileId("otr_offscreenESP");
	//	OptionsGroup.PlaceLabledControl(1, "Offscreen ESP", this, &OffscreenESP);

	GlowZ.SetFileId("opt_glowz");
	GlowZ.SetValue(0.f);
	GlowZ.SetBoundaries(0.f, 100.f);
	GlowZ.extension = XorStr("%%");
	OptionsGroup.PlaceLabledControl(1, "Enemy Glow", this, &GlowZ);

	team_glow.SetFileId("opt_team_glow");
	team_glow.SetValue(0.f);
	team_glow.SetBoundaries(0.f, 100.f);
	team_glow.extension = XorStr("%%");
	OptionsGroup.PlaceLabledControl(1, "Team Glow", this, &team_glow);

	Glowz_lcl.SetFileId("opt_glowz_lcl");
	Glowz_lcl.SetValue(0.f);
	Glowz_lcl.SetBoundaries(0.f, 100.f);
	Glowz_lcl.extension = XorStr("%%");
	OptionsGroup.PlaceLabledControl(1, "Local Glow", this, &Glowz_lcl);

	FiltersAll.SetFileId("ftr_all");
	OptionsGroup.PlaceLabledControl(2, "All", this, &FiltersAll);
	show_players.SetFileId("ftr_players");
	show_players.SetState(true);
	OptionsGroup.PlaceLabledControl(2, "Players", this, &show_players);
	show_team.SetFileId("ftr_enemyonly");
	OptionsGroup.PlaceLabledControl(2, "Show Team", this, &show_team);
	FiltersWeapons.SetFileId("ftr_weaps");
	OptionsGroup.PlaceLabledControl(2, "Weapons", this, &FiltersWeapons);
	FiltersNades.SetFileId("ftr_nades");
	OptionsGroup.PlaceLabledControl(2, "Nades", this, &FiltersNades);
	FiltersC4.SetFileId("ftr_c4");
	OptionsGroup.PlaceLabledControl(2, "C4", this, &FiltersC4);

	//	asus_type.SetFileId("asus_wall_type");
	//	asus_type.AddItem("Props Only");
	//	asus_type.AddItem("Walls and Props");
	//	OptionsGroup.PlaceLabledControl(2, "Asus Type", this, &asus_type);


	//----------------------------------------------//


	SpreadCrosshair.SetFileId(XorStr("v_spreadcrosshair"));
	SpreadCrosshair.AddItem("Off");
	SpreadCrosshair.AddItem("Standard");
	SpreadCrosshair.AddItem("Colour");
	SpreadCrosshair.AddItem("Rainbow");
	SpreadCrosshair.AddItem("Rainbow Rotate");
	OptionsGroup.PlaceLabledControl(3, XorStr("Spread crosshair"), this, &SpreadCrosshair);

/*	SpreadCrossSize.SetFileId("otr_spreadcross_size");
	SpreadCrossSize.SetBoundaries(1.f, 100.f); //we should take smth like 650 as max so i guess *6.5?
	SpreadCrossSize.extension = XorStr("%%");
	SpreadCrossSize.SetValue(45.f);
	OptionsGroup.PlaceLabledControl(3, "Size", this, &SpreadCrossSize); */

	AutowallCrosshair.SetFileId("otr_autowallcross");
	OptionsGroup.PlaceLabledControl(3, "Crosshair", this, &AutowallCrosshair);

	//	DamageIndicator.SetFileId("otr_btracers");
	//	OptionsGroup.PlaceLabledControl(3, "Damage Indicator", this, &DamageIndicator);

	OtherNoVisualRecoil.SetFileId("otr_visrecoil");
	OptionsGroup.PlaceLabledControl(3, "No visual recoil", this, &OtherNoVisualRecoil);

	OtherNoScope.SetFileId("otr_noscope");
	OptionsGroup.PlaceLabledControl(3, "Remove scope", this, &OtherNoScope);

	RemoveZoom.SetFileId("otr_remv_zoom");
	OptionsGroup.PlaceLabledControl(3, "Remove zoom", this, &RemoveZoom);

	OtherNoFlash.SetFileId("otr_noflash");
	OptionsGroup.PlaceLabledControl(3, "Remove flash effect", this, &OtherNoFlash);

	flashAlpha.SetFileId("otr_stolen_from_punknown_muahahaha");
	flashAlpha.SetBoundaries(0, 100);
	flashAlpha.extension = XorStr("%%");
	flashAlpha.SetValue(10);
	OptionsGroup.PlaceLabledControl(3, "Flash Alpha", this, &flashAlpha);

	nosmoke.SetFileId("otr_nosmoke");
	OptionsGroup.PlaceLabledControl(3, "Remove smoke", this, &nosmoke);

	OtherViewmodelFOV.SetFileId("otr_viewfov");
	OtherViewmodelFOV.SetBoundaries(30.f, 120.f);
	OtherViewmodelFOV.SetValue(90.f);
	OptionsGroup.PlaceLabledControl(3, "Viewmodel fov", this, &OtherViewmodelFOV);

	OtherFOV.SetFileId("otr_fov");
	OtherFOV.SetBoundaries(0.f, 50.f);
	OtherFOV.SetValue(0.f);
	OptionsGroup.PlaceLabledControl(3, "Override fov", this, &OtherFOV);

	override_viewmodel.SetFileId("otr_override_viewmodel_offset");
	OptionsGroup.PlaceLabledControl(3, "Override Viewmodel Offset", this, &override_viewmodel);

	offset_x.SetFileId("otr_offset_x");
	offset_x.SetBoundaries(-7.5, 7.5);
	offset_x.SetValue(2.5);
	OptionsGroup.PlaceLabledControl(3, "Offset X", this, &offset_x);

	offset_y.SetFileId("otr_offset_Y");
	offset_y.SetBoundaries(-7.5, 7.5);
	offset_y.SetValue(2.0);
	OptionsGroup.PlaceLabledControl(3, "Offset Y", this, &offset_y);

	offset_z.SetFileId("otr_offset_z");
	offset_z.SetBoundaries(-7.5, 7.5);
	offset_z.SetValue(-2.0);
	OptionsGroup.PlaceLabledControl(3, "Offset z", this, &offset_z);

	LBYIndicator.SetFileId("otr_LBYIndicator");
	LBYIndicator.AddItem("Off");
	LBYIndicator.AddItem("Classic");
	LBYIndicator.AddItem("Genuine");
	OptionsGroup.PlaceLabledControl(4, "LBY Indicator", this, &LBYIndicator);

	LCIndicator.SetFileId("otr_LCIndicator");
	LCIndicator.AddItem("Off");
	LCIndicator.AddItem("Classic");
	LCIndicator.AddItem("Genuine");
	OptionsGroup.PlaceLabledControl(4, "LC Indicator", this, &LCIndicator);

	manualaa_type.SetFileId("manualaa");
	manualaa_type.AddItem("Off");
	manualaa_type.AddItem("Single Arrow");
	manualaa_type.AddItem("All Arrows");
	OptionsGroup.PlaceLabledControl(4, "Manual Indicator", this, &manualaa_type); // requested by: https://steamcommunity.com/id/9a-

	logs.SetFileId("otr_skeetpaste");
	OptionsGroup.PlaceLabledControl(4, "Event Log", this, &logs);

//	killfeed.SetFileId("otr_killfeed");
//	OptionsGroup.PlaceLabledControl(4, "Preserve Killfeed", this, &killfeed); // requested by: https://steamcommunity.com/id/B1GN1GNN

//	cheatinfo.SetFileId("cheatinfox");
//	OptionsGroup.PlaceLabledControl(4, "Debug Local Player Info", this, &cheatinfo);

	OtherHitmarker.SetFileId("otr_hitmarker");
	OptionsGroup.PlaceLabledControl(4, "Hitmarker", this, &OtherHitmarker);

	watermark.SetFileId("otr_watermark");
	watermark.SetState(true);
	OptionsGroup.PlaceLabledControl(4, "Watermark", this, &watermark);

	//----------------------------------------------//

	ChamsGroup.SetText("Chams");
	ChamsGroup.AddTab(CGroupTab("Main", 1));
	ChamsGroup.AddTab(CGroupTab("Mods", 2));
	ChamsGroup.SetPosition(294, 30); // 225, 30
	ChamsGroup.SetSize(260, 245);
	RegisterControl(&ChamsGroup);

	ChamsEnemyVis.SetFileId("chams_en_vis");
	ChamsGroup.PlaceLabledControl(1, "Enemies (Visible)", this, &ChamsEnemyVis);

	ChamsEnemyNoVis.SetFileId("chams_en_novis");
	ChamsGroup.PlaceLabledControl(1, "Enemies (Invisible)", this, &ChamsEnemyNoVis); // *1

	ChamsTeamVis.SetFileId("ChamsTeamVis");
	ChamsGroup.PlaceLabledControl(1, "Team", this, &ChamsTeamVis);

	ChamsLocal.SetFileId("chams_local");
	ChamsGroup.PlaceLabledControl(1, "Local Chams", this, &ChamsLocal);

	HandCHAMS.SetFileId("chams_local_hand");
	HandCHAMS.AddItem("off");
	HandCHAMS.AddItem("Simple");
	HandCHAMS.AddItem("Wireframe");
	HandCHAMS.AddItem("Golden");
	HandCHAMS.AddItem("Glass");
	ChamsGroup.PlaceLabledControl(1, "Hand Chams", this, &HandCHAMS);

	GunCHAMS.SetFileId("chams_local_weapon");
	GunCHAMS.AddItem("off");
	GunCHAMS.AddItem("Simple");
	GunCHAMS.AddItem("Wireframe");
	GunCHAMS.AddItem("Golden");
	GunCHAMS.AddItem("Glass");
	ChamsGroup.PlaceLabledControl(1, "Weapon Chams", this, &GunCHAMS);

	localmaterial.SetFileId("esp_localscopedmat");
	localmaterial.AddItem("Default");
	localmaterial.AddItem("Clear");
	localmaterial.AddItem("Cham");
	localmaterial.AddItem("Wireframe");
	localmaterial.AddItem("LSD");
	localmaterial.AddItem("Dark");
//	localmaterial.AddItem("Pulse");
	ChamsGroup.PlaceLabledControl(1, "Scoped Materials", this, &localmaterial);

	enemy_blend.SetFileId("esp_blendvis");
	enemy_blend.SetBoundaries(0, 100);
	enemy_blend.SetValue(75);
	enemy_blend.extension = XorStr("%%");
	ChamsGroup.PlaceLabledControl(2, "Visible Chams Blend", this, &enemy_blend);

	enemy_blend_invis.SetFileId("esp_blendinvis");
	enemy_blend_invis.SetBoundaries(0, 100);
	enemy_blend_invis.SetValue(75);
	enemy_blend_invis.extension = XorStr("%%");
	ChamsGroup.PlaceLabledControl(2, "Invisible Chams Blend", this, &enemy_blend_invis); // *! -> requested an update by: https://steamcommunity.com/id/Skatergirllzz

	blend_local.SetFileId("esp_teamblend");
	blend_local.SetBoundaries(0, 100);
	blend_local.SetValue(75);
	blend_local.extension = XorStr("%%");
	ChamsGroup.PlaceLabledControl(2, "Local Player Chams Blend", this, &blend_local);

	transparency.SetFileId("esp_transparency");
	transparency.SetBoundaries(0, 100);
	transparency.SetValue(20);
	transparency.extension = XorStr("%%");
	ChamsGroup.PlaceLabledControl(2, "Scoped Transparency", this, &transparency);

	hand_transparency.SetFileId("esp_hand_transparency");
	hand_transparency.SetBoundaries(0, 100);
	hand_transparency.SetValue(20);
	hand_transparency.extension = XorStr("%%");
	ChamsGroup.PlaceLabledControl(2, "Arm Transparency", this, &hand_transparency);

	gun_transparency.SetFileId("esp_gun_transparency");
	gun_transparency.SetBoundaries(0, 100);
	gun_transparency.SetValue(20);
	gun_transparency.extension = XorStr("%%");
	ChamsGroup.PlaceLabledControl(2, "Gun Transparency", this, &gun_transparency);

#pragma region WORLD IS DOOMED AND FLAT AND GAY AND TRUMP IS LITERALLY HITLER AND FREAK IS THE QUEEN OF MARS
	worldgroup.SetText("World");
	worldgroup.SetPosition(294, 285);  // 225, 285
	worldgroup.SetSize(260, 155);
	RegisterControl(&worldgroup);
	colmodupdate.SetFileId("otr_night");
	worldgroup.PlaceLabledControl(0, "Force update Materials", this, &colmodupdate); //you could've just made this a button lol

	customskies.SetFileId("otr_skycustom");
	customskies.AddItem("Default");
	customskies.AddItem("Night");
	customskies.AddItem("NoSky");
	customskies.AddItem("Cloudy");
	//	customskies.AddItem("NukeBlank2");
	worldgroup.PlaceLabledControl(0, "Change Sky", this, &customskies);

	colmod.SetFileId("night_amm");
	colmod.SetBoundaries(000.000f, 100.00f);
	colmod.extension = XorStr("%%");
	colmod.SetValue(020.0f);
	worldgroup.PlaceLabledControl(0, "Brightness percentage", this, &colmod);

	asusamount.SetFileId("otr_asusprops");
	asusamount.SetBoundaries(1.f, 100.f);
	asusamount.extension = XorStr("%%");
	asusamount.SetValue(95.f);
	worldgroup.PlaceLabledControl(0, "Asus percantage", this, &asusamount);

	OtherEntityGlow.SetFileId("otr_world_ent_glow");
	worldgroup.PlaceLabledControl(0, "Glow world entities", this, &OtherEntityGlow);


#pragma endregion Setting up the Other controls
}
void CMiscTab::Setup()
{
	/*
	__  __ _____  _____  _____
	|  \/  |_   _|/ ____|/ ____|
	| \  / | | | | (___ | |
	| |\/| | | |  \___ \| |
	| |  | |_| |_ ____) | |____
	|_|  |_|_____|_____/ \_____|

	
	*/
	SetTitle("C");
#pragma region Other
	OtherGroup.SetText("Other");
	OtherGroup.AddTab(CGroupTab("Main", 1));
	OtherGroup.AddTab(CGroupTab("BuyBot", 2));
	OtherGroup.SetPosition(4, 30);
	OtherGroup.SetSize(270, 400);
	RegisterControl(&OtherGroup);

	OtherSafeMode.SetFileId("otr_safemode");
	OtherSafeMode.AddItem("Anti Untrusted");
	OtherSafeMode.AddItem("Anti VAC Kick");
	OtherSafeMode.AddItem("Unrestricted (!)");
	OtherGroup.PlaceLabledControl(1, "Safety Mode", this, &OtherSafeMode);

	OtherAutoJump.SetFileId("otr_autojump");
	OtherGroup.PlaceLabledControl(1, "BunnyHop", this, &OtherAutoJump);

	OtherAutoStrafe.SetFileId("otr_strafe");
	OtherGroup.PlaceLabledControl(1, "Air Strafe", this, &OtherAutoStrafe);

	hitmarker_sound.SetFileId("hitmarker_sound");
	hitmarker_sound.AddItem("Off");
	hitmarker_sound.AddItem("Cod");
	hitmarker_sound.AddItem("ArenaSwitch");
	hitmarker_sound.AddItem("Bubble");
	hitmarker_sound.AddItem("Bameware");
	hitmarker_sound.AddItem("Anime");
	hitmarker_sound.AddItem("Hitler"); // this is for CruZZ and Crytec. Heil hitler
									   //	hitmarker_sound.AddItem("Steam Notification");
	OtherGroup.PlaceLabledControl(1, "Hitmarker Sound", this, &hitmarker_sound);

	OtherThirdperson.SetFileId("aa_1thirpbind");
	OtherGroup.PlaceLabledControl(1, "Thirdperson", this, &OtherThirdperson);
	ThirdPersonKeyBind.SetFileId("aa_thirpbind");
	OtherGroup.PlaceLabledControl(1, "", this, &ThirdPersonKeyBind);

	infinite_duck.SetFileId("infinteduck");
	OtherGroup.PlaceLabledControl(1, "No Crouch Delay", this, &infinite_duck);

	Radar.SetFileId("Radar");
	OtherGroup.PlaceLabledControl(1, "Draw Radar", this, &Radar);

	RadarX.SetFileId("misc_radar_xax1");
	RadarX.SetBoundaries(0, 1920);
	RadarX.SetValue(0);
	OtherGroup.PlaceLabledControl(1, "X-Axis", this, &RadarX);

	RadarY.SetFileId("misc_radar_yax2");
	RadarY.SetBoundaries(0, 1080);
	RadarY.SetValue(0);
	OtherGroup.PlaceLabledControl(1, "Y-Axis", this, &RadarY);

	autojump_type.SetFileId("misc_autojump_type");
	autojump_type.AddItem("Normal");
	autojump_type.AddItem("Always On");
	OtherGroup.PlaceLabledControl(1, "BunnyHop Type", this, &autojump_type);

	airduck_type.SetFileId("misc_airduck_type");
	airduck_type.AddItem("Off");
	airduck_type.AddItem("Normal");
	airduck_type.AddItem("Fake");
	OtherGroup.PlaceLabledControl(1, "AirDuck", this, &airduck_type);

	NameChanger.SetFileId("misc_NameChanger.");
	OtherGroup.PlaceLabledControl(1, "Name Stealer", this, &NameChanger);

	buybot_primary.SetFileId("buybot_primary");
	buybot_primary.AddItem("Off");
	buybot_primary.AddItem("Auto-Sniper");
	buybot_primary.AddItem("Scout (ssg08)");
	buybot_primary.AddItem("Awp");
	buybot_primary.AddItem("Ak-47 / M4");
	buybot_primary.AddItem("Aug / Sg553");
	buybot_primary.AddItem("Mp9 / Mac10");
	OtherGroup.PlaceLabledControl(2, "Primary Weapon", this, &buybot_primary);

	buybot_secondary.SetFileId("buybot_secondary");
	buybot_secondary.AddItem("Off");
	buybot_secondary.AddItem("Dual Berretas");
	buybot_secondary.AddItem("Revolver / Desert Eagle");
	buybot_secondary.AddItem("Five-Seven");
	OtherGroup.PlaceLabledControl(2, "Secondary Weapon", this, &buybot_secondary);

	buybot_otr.SetFileId("buybot_other");
	buybot_otr.items.push_back(dropdownboxitem(false, XorStr("kevlar")));
	buybot_otr.items.push_back(dropdownboxitem(false, XorStr("he-grenade")));
	buybot_otr.items.push_back(dropdownboxitem(false, XorStr("flashbang")));
	buybot_otr.items.push_back(dropdownboxitem(false, XorStr("smoke grenade")));
	buybot_otr.items.push_back(dropdownboxitem(false, XorStr("molotov")));
	buybot_otr.items.push_back(dropdownboxitem(false, XorStr("zeus")));
	buybot_otr.items.push_back(dropdownboxitem(false, XorStr("defuse-kit")));
	OtherGroup.PlaceLabledControl(2, "Others", this, &buybot_otr);
	
	AntiAimGroup.SetPosition(286, 30);
	AntiAimGroup.SetText("Anti-Aim");
	AntiAimGroup.SetSize(270, 400);
	AntiAimGroup.AddTab(CGroupTab("Main", 1));
	AntiAimGroup.AddTab(CGroupTab("Modifiers", 2));
	AntiAimGroup.AddTab(CGroupTab("Misc", 3));
	RegisterControl(&AntiAimGroup);
	AntiAimEnable.SetFileId("aa_enable");
	AntiAimGroup.PlaceLabledControl(1, "Enable", this, &AntiAimEnable);

	AntiAimPitch.SetFileId("aa_x");
	AntiAimPitch.AddItem("Off");
	AntiAimPitch.AddItem("Down");
	AntiAimPitch.AddItem("Up");
	AntiAimPitch.AddItem("Jitter");
	AntiAimPitch.AddItem("Random");
	AntiAimPitch.AddItem("Zero");
	AntiAimPitch.AddItem("Fake Down (!)");
	AntiAimPitch.AddItem("Fake Up (!)");
	AntiAimGroup.PlaceLabledControl(1, "Pitch", this, &AntiAimPitch);

	AntiAimYaw.SetFileId("aa_y");
	AntiAimYaw.AddItem("Off");
	AntiAimYaw.AddItem("Backward");
	AntiAimYaw.AddItem("Backward Jitter");
	AntiAimYaw.AddItem("Random Lowerbody");
	AntiAimYaw.AddItem("Manual");
	AntiAimYaw.AddItem("Jitter Crooked");
	AntiAimYaw.AddItem("Freestanding");
	AntiAimGroup.PlaceLabledControl(1, "Standing Yaw", this, &AntiAimYaw);

	desync_aa_stand.SetFileId("v_desync_aa_stand");
	AntiAimGroup.PlaceLabledControl(1, "Desync", this, &desync_aa_stand);

	AntiAimYawrun.SetFileId("aa_y2");
	AntiAimYawrun.AddItem("Off");
	AntiAimYawrun.AddItem("Backward");
	AntiAimYawrun.AddItem("Backward Jitter");
	AntiAimYawrun.AddItem("Random Lowerbody");
	AntiAimYawrun.AddItem("Manual");
	AntiAimYawrun.AddItem("Jitter Crooked");

	AntiAimYawrun.AddItem("Freestanding");
	AntiAimGroup.PlaceLabledControl(1, "Moving Yaw", this, &AntiAimYawrun);

	desync_aa_move.SetFileId("v_desync_aa_move");
	AntiAimGroup.PlaceLabledControl(1, "Desync", this, &desync_aa_move);

	AntiAimYaw3.SetFileId("aa_y3");
	AntiAimYaw3.AddItem("Off");
	AntiAimYaw3.AddItem("Backward");
	AntiAimYaw3.AddItem("Backward Jitter");
	AntiAimYaw3.AddItem("Random Lowerbody");
	AntiAimYaw3.AddItem("Manual");
	AntiAimYaw3.AddItem("Jitter Crooked");
//	AntiAimYaw3.AddItem("Freestanding");
	AntiAimGroup.PlaceLabledControl(1, "InAir Yaw", this, &AntiAimYaw3);

//	air_desync.SetFileId("v_air_desync");
//	AntiAimGroup.PlaceLabledControl(1, "Air Desync", this, &air_desync);

	pitch_up.SetFileId("pitch_up");
	AntiAimGroup.PlaceLabledControl(1, "Pitch Flick", this, &pitch_up);

	disable_on_dormant.SetFileId("disable_on_dormant");
	AntiAimGroup.PlaceLabledControl(1, "Disable On Dormant", this, &disable_on_dormant);

	minimal_walk.SetFileId("minimal_walk");
	AntiAimGroup.PlaceLabledControl(1, "Minimal Walk Key", this, &minimal_walk);

	freerange.SetFileId("freestanding_range");
	freerange.SetBoundaries(0, 90);
	freerange.SetValue(35);
	AntiAimGroup.PlaceLabledControl(1, "Freestanding Value", this, &freerange);

	choked_shot.SetFileId("choke_shot");
	AntiAimGroup.PlaceLabledControl(1, "Choke Shot", this, &choked_shot);
	//-<------------------------------------->-//


	FakelagStand.SetFileId("fakelag_stand_val");
	FakelagStand.SetBoundaries(1, 14);
	FakelagStand.SetValue(1);
	AntiAimGroup.PlaceLabledControl(2, "Fakelag Standing", this, &FakelagStand);

	FakelagMove.SetFileId("fakelag_move_val");
	FakelagMove.SetBoundaries(1, 14);
	FakelagMove.SetValue(1);
	AntiAimGroup.PlaceLabledControl(2, "Fakelag Moving", this, &FakelagMove);

	Fakelagjump.SetFileId("fakelag_jump_val");
	Fakelagjump.SetBoundaries(1, 14);
	Fakelagjump.SetValue(1);
	AntiAimGroup.PlaceLabledControl(2, "Fakelag in Air", this, &Fakelagjump);
	 
	//	target_pistol.items.push_back(dropdownboxitem(false, XorStr("Head")));
	fl_spike.SetFileId("fakelag_spike");
	fl_spike.items.push_back(dropdownboxitem(false, XorStr("Shot")));
	fl_spike.items.push_back(dropdownboxitem(false, XorStr("Peak Of Jump")));
	fl_spike.items.push_back(dropdownboxitem(false, XorStr("Enemy Sight")));
	fl_spike.items.push_back(dropdownboxitem(false, XorStr("Velocity Increase")));
	fl_spike.items.push_back(dropdownboxitem(false, XorStr("UnDuck")));
	fl_spike.items.push_back(dropdownboxitem(false, XorStr("On Key")));
	AntiAimGroup.PlaceLabledControl(2, "Fakelag Spike Factors", this, &fl_spike);

	fakelag_key.SetFileId("fakelag_onkey");
	AntiAimGroup.PlaceLabledControl(2, "Fakelag Spike Key", this, &fakelag_key);

	FakelagBreakLC.SetFileId("fakelag_breaklc");
	AntiAimGroup.PlaceLabledControl(2, "Break Lag Compensation", this, &FakelagBreakLC);

	antilby.SetFileId("otr_meh");
	antilby.AddItem("Off");
	antilby.AddItem("One Flick");
	antilby.AddItem("Two Flicks");
	antilby.AddItem("Relative");
	AntiAimGroup.PlaceLabledControl(2, "Anti-LBY", this, &antilby);

	BreakLBYDelta2.SetFileId("b_antilby2");
	BreakLBYDelta2.SetBoundaries(-180, 180);
	BreakLBYDelta2.SetValue(90);
	AntiAimGroup.PlaceLabledControl(2, "Anti-LBY First Flick", this, &BreakLBYDelta2);

	BreakLBYDelta.SetFileId("b_antilby");
	BreakLBYDelta.SetBoundaries(-180, 180);
	BreakLBYDelta.SetValue(-90);
	AntiAimGroup.PlaceLabledControl(2, "Anti-LBY Second Flick", this, &BreakLBYDelta);

//	auto_fakelag.SetFileId("fakelag_auto");
//	AntiAimGroup.PlaceLabledControl(2, "Dynamic Fakelag", this, &auto_fakelag);

	manualleft.SetFileId("otr_keybasedleft");
	AntiAimGroup.PlaceLabledControl(3, "Manual Right", this, &manualleft);

	manualright.SetFileId("otr_keybasedright");
	AntiAimGroup.PlaceLabledControl(3, "Manual Left", this, &manualright);

	manualback.SetFileId("otr_keybasedback");
	AntiAimGroup.PlaceLabledControl(3, "Manual Back", this, &manualback);

	manualfront.SetFileId("otr_manualfrontk");
	AntiAimGroup.PlaceLabledControl(3, "Manual Front", this, &manualfront);

	fw.SetFileId("fakewalk_key");
	AntiAimGroup.PlaceLabledControl(3, "FakeWalk Key", this, &fw);

	FakeWalkSpeed.SetFileId("fakewalk_speed");
	FakeWalkSpeed.SetBoundaries(1, 100);
	FakeWalkSpeed.SetValue(40);
	FakeWalkSpeed.extension = XorStr("%%");
	AntiAimGroup.PlaceLabledControl(3, "Fakewalk Slowdown", this, &FakeWalkSpeed);

	randlbyr.SetFileId("b_randlbyr");
	randlbyr.SetBoundaries(20, 180);
	randlbyr.SetValue(60);
	AntiAimGroup.PlaceLabledControl(3, "Random Lowerbody Ammount", this, &randlbyr);

	fake_crouch.SetFileId("fake_crouch");
	AntiAimGroup.PlaceLabledControl(3, "Fake Crouch", this, &fake_crouch);

	fake_crouch_key.SetFileId("fake_crouch_key");
	AntiAimGroup.PlaceLabledControl(3, "Fake Crouch Key", this, &fake_crouch_key);

	QuickStop.SetFileId("acc_quickstop");
	AntiAimGroup.PlaceLabledControl(3, "Quickstop", this, &QuickStop);

}

void CColorTab::Setup()
{
	SetTitle("G");
#pragma region Visual Colors
	ColorsGroup.SetText("Settings");
	ColorsGroup.SetPosition(4, 30);
	ColorsGroup.AddTab(CGroupTab("ESP", 1));
	ColorsGroup.AddTab(CGroupTab("ESP 2", 2));
	ColorsGroup.AddTab(CGroupTab("Menu 1", 3));
	ColorsGroup.AddTab(CGroupTab("Menu 2", 4));
	//	ColorsGroup.AddTab(CGroupTab("Misc", 6));
	ColorsGroup.SetSize(270, 335);
	RegisterControl(&ColorsGroup);

	/*---------------------- COL ----------------------*/
	/*---------------------- COL ----------------------*/
	/*---------------------- COL ----------------------*/

	NameCol.SetFileId(XorStr("player_espname_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("Name"), this, &NameCol);

	BoxCol.SetFileId(XorStr("player_espbox_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("Box Enemy"), this, &BoxCol);;

	Skeleton.SetFileId(XorStr("player_skeleton_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("Skeleton Enemy"), this, &Skeleton);

	GlowEnemy.SetFileId(XorStr("player_glowenemy_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("Glow Enemy"), this, &GlowEnemy);

	GlowTeam.SetFileId(XorStr("player_glowteam_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("Glow Team"), this, &GlowTeam);

	GlowOtherEnt.SetFileId(XorStr("player_glowother_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("Glow World"), this, &GlowOtherEnt);

	GlowLocal.SetFileId(XorStr("player_glowlocal_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("Glow Local"), this, &GlowLocal);

	Weapons.SetFileId(XorStr("player_weapons_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("Weapons Enemy"), this, &Weapons);

	Weaponsteam.SetFileId(XorStr("player_weapons_color_team"));
	ColorsGroup.PlaceLabledControl(1, XorStr("Weapons Team"), this, &Weaponsteam);

	Ammo.SetFileId(XorStr("player_ammo_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("Ammo Enemy"), this, &Ammo);

	Money.SetFileId(XorStr("player_money_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("Money"), this, &Money);
	Offscreen.SetFileId(XorStr("player_offscreen_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("Offscreen"), this, &Offscreen);
	ChamsLocal.SetFileId(XorStr("player_chamslocal_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("Chams Local"), this, &ChamsLocal);
	ChamsEnemyVis.SetFileId(XorStr("player_chamsEVIS_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("Chams Enemy"), this, &ChamsEnemyVis);
	ChamsEnemyNotVis.SetFileId(XorStr("player_chamsENVIS_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("Chams Enemy (Behind Wall)"), this, &ChamsEnemyNotVis);

	ChamsTeamVis.SetFileId(XorStr("player_ChamsTeamVis"));
	ColorsGroup.PlaceLabledControl(1, XorStr("Chams Team"), this, &ChamsTeamVis);

//	Bullettracer.SetFileId(XorStr("player_beam_color"));
//	ColorsGroup.PlaceLabledControl(2, XorStr("Bullet tracers"), this, &Bullettracer);

	scoped_c.SetFileId(XorStr("scope_c"));
	ColorsGroup.PlaceLabledControl(2, XorStr("Local Scoped Material Colour"), this, &scoped_c);

	misc_lagcomp.SetFileId(XorStr("misc_lagcomp"));
	misc_lagcomp.SetColor(250, 250, 250, 255);
	ColorsGroup.PlaceLabledControl(2, XorStr("Lag Compensation"), this, &misc_lagcomp);

	misc_lagcompBones.SetFileId(XorStr("misc_lagcompBones"));
	misc_lagcompBones.SetColor(250, 250, 250, 255);
	ColorsGroup.PlaceLabledControl(2, XorStr("Backtrack Bones"), this, &misc_lagcompBones);

	spreadcrosscol.SetFileId(XorStr("weapon_spreadcross_col"));
	ColorsGroup.PlaceLabledControl(2, XorStr("Spread Crosshair"), this, &spreadcrosscol);
	HandChamsCol.SetFileId(XorStr("handchams_col"));
	ColorsGroup.PlaceLabledControl(2, XorStr("Hand Chams"), this, &HandChamsCol);
	GunChamsCol.SetFileId(XorStr("gunchams_col"));
	ColorsGroup.PlaceLabledControl(2, XorStr("Weapon Chams"), this, &GunChamsCol);
	//---

	ModulateSkyBox.SetFileId(XorStr("sky_box_color_enable"));
	ColorsGroup.PlaceLabledControl(2, XorStr("Enable Sky Color Changer"), this, &ModulateSkyBox);
	sky_r.SetFileId("sky_r");
	sky_r.SetBoundaries(0.f, 25.f);
	sky_r.SetValue(10.f);
	ColorsGroup.PlaceLabledControl(2, "Sky: Red", this, &sky_r);

	sky_g.SetFileId("sky_g");
	sky_g.SetBoundaries(0.f, 25.f);
	sky_g.SetValue(1.f);
	ColorsGroup.PlaceLabledControl(2, "Sky: Green", this, &sky_g);

	sky_b.SetFileId("sky_b");
	sky_b.SetBoundaries(0.f, 25.f);
	sky_b.SetValue(20.f);
	ColorsGroup.PlaceLabledControl(2, "Sky: Blue", this, &sky_b);

	MenuBar.SetFileId(XorStr("menu_bar_mode"));
	MenuBar.AddItem("Static");
	MenuBar.AddItem("Animated");
	MenuBar.AddItem("Fade");
	ColorsGroup.PlaceLabledControl(3, XorStr("Menu Bar"), this, &MenuBar);
	outl_r.SetFileId("outlred");
	outl_r.SetBoundaries(0.f, 255.f);
	outl_r.SetValue(55.f);
	ColorsGroup.PlaceLabledControl(3, "Outer Left: Red", this, &outl_r);
	outl_g.SetFileId("outlgreen");
	outl_g.SetBoundaries(0.f, 255.f);
	outl_g.SetValue(15.f);
	ColorsGroup.PlaceLabledControl(3, "Outer Left: Green", this, &outl_g);
	outl_b.SetFileId("outlblue");
	outl_b.SetBoundaries(0.f, 255.f);
	outl_b.SetValue(210.f);
	ColorsGroup.PlaceLabledControl(3, "Outer Left: Blue", this, &outl_b);
	inl_r.SetFileId("inlred");
	inl_r.SetBoundaries(0.f, 255.f);
	inl_r.SetValue(185.f);
	ColorsGroup.PlaceLabledControl(3, "Inner Left: Red", this, &inl_r);
	inl_g.SetFileId("inlgreen");
	inl_g.SetBoundaries(0.f, 255.f);
	inl_g.SetValue(25.f);
	ColorsGroup.PlaceLabledControl(3, "Inner Left: Green", this, &inl_g);
	inl_b.SetFileId("inlblue");
	inl_b.SetBoundaries(0.f, 255.f);
	inl_b.SetValue(230.f);
	ColorsGroup.PlaceLabledControl(3, "Inner Left: Blue", this, &inl_b);
	inr_r.SetFileId("inrred");
	inr_r.SetBoundaries(0.f, 255.f);
	inr_r.SetValue(185.f);
	ColorsGroup.PlaceLabledControl(4, "Inner Right: Red", this, &inr_r);

	inr_g.SetFileId("inrgreen");
	inr_g.SetBoundaries(0.f, 255.f);
	inr_g.SetValue(25.f);
	ColorsGroup.PlaceLabledControl(4, "Inner Right: Green", this, &inr_g);

	inr_b.SetFileId("inrblue");
	inr_b.SetBoundaries(0.f, 255.f);
	inr_b.SetValue(230.f);
	ColorsGroup.PlaceLabledControl(4, "Inner Right: Blue", this, &inr_b);
	outr_r.SetFileId("outrred");
	outr_r.SetBoundaries(0.f, 255.f);
	outr_r.SetValue(55.f);
	ColorsGroup.PlaceLabledControl(4, "Outer Right: Red", this, &outr_r);
	outr_g.SetFileId("outrgreen");
	outr_g.SetBoundaries(0.f, 255.f);
	outr_g.SetValue(15.f);
	ColorsGroup.PlaceLabledControl(4, "Outer Right: Green", this, &outr_g);
	outr_b.SetFileId("outrblue");
	outr_b.SetBoundaries(0.f, 255.f);
	outr_b.SetValue(210.f);
	ColorsGroup.PlaceLabledControl(4, "Outer Right: Blue", this, &outr_b);

	bomb_timer.SetFileId(XorStr("bomb_timer"));
	bomb_timer.SetColor(250, 10, 90, 230);
	ColorsGroup.PlaceLabledControl(4, XorStr("Bomb Timer"), this, &bomb_timer);

	Menu.SetFileId(XorStr("menu_color"));
	Menu.SetColor(170, 20, 250, 255);
	ColorsGroup.PlaceLabledControl(4, XorStr("Controls"), this, &Menu);


	//	misc_backtrackchams.SetFileId(XorStr("misc_backtrackchams"));
	//	misc_backtrackchams.SetColor(250, 250, 250, 255);
	//	ColorsGroup.PlaceLabledControl(5, XorStr("Backtrack Chams"), this, &misc_backtrackchams);

	ConfigGroup.SetText("Configs");
	ConfigGroup.SetPosition(290, 30);
	ConfigGroup.SetSize(264, 335);
	RegisterControl(&ConfigGroup); ConfigListBox.SetHeightInItems(7);
	list_configs();
	ConfigGroup.PlaceLabledControl(0, XorStr(""), this, &ConfigListBox);
	LoadConfig.SetText(XorStr("Load"));
	LoadConfig.SetCallback(&load_callback);
	ConfigGroup.PlaceLabledControl(0, "", this, &LoadConfig);
	SaveConfig.SetText(XorStr("Save"));
	SaveConfig.SetCallback(&save_callback);
	ConfigGroup.PlaceLabledControl(0, "", this, &SaveConfig);
	RemoveConfig.SetText(XorStr("Remove"));
	RemoveConfig.SetCallback(&remove_config);
	ConfigGroup.PlaceLabledControl(0, "", this, &RemoveConfig);
	ConfigGroup.PlaceLabledControl(0, "", this, &NewConfigName);
	AddConfig.SetText(XorStr("Add"));
	AddConfig.SetCallback(&add_config);
	ConfigGroup.PlaceLabledControl(0, "", this, &AddConfig);
	/*---------------------- OTHERS ----------------------*/
	/*---------------------- OTHERS ----------------------*/
	/*---------------------- OTHERS ----------------------*/
	OtherOptions.SetText("other");
	OtherOptions.SetPosition(4, 373);
	OtherOptions.SetSize(270, 70);
	RegisterControl(&OtherOptions);

	DebugLagComp.SetFileId(XorStr("lagcompensationyes"));
	OtherOptions.PlaceLabledControl(0, XorStr("Draw Lag Compensation"), this, &DebugLagComp);

	BackTrackBones2.SetFileId(XorStr("spookybonesOwOomg"));
	OtherOptions.PlaceLabledControl(0, XorStr("BackTrack Bones"), this, &BackTrackBones2);

	quickstop_speed.SetFileId(XorStr("quickstop_speed"));
	quickstop_speed.SetBoundaries(1, 40);
	quickstop_speed.SetValue(30);
	OtherOptions.PlaceLabledControl(0, XorStr("Quick Stop Speed"), this, &quickstop_speed);
	//	BackTrackBones.SetFileId(XorStr("spookybonesOwO"));
	//	OtherOptions.PlaceLabledControl(0, XorStr("BackTrack Chams"), this, &BackTrackBones);
	// your fps will look beyond the gates of the next life and will raise their middle fingers for having caused their suicide

	OtherOptions2.SetText("other 2");
	OtherOptions2.SetPosition(290, 373);
	OtherOptions2.SetSize(264, 70);
	RegisterControl(&OtherOptions2);

	//	experimental_backtrack.SetFileId(XorStr("experimental_backtrack"));
	//	OtherOptions2.PlaceLabledControl(0, XorStr("Experimental Position Adjustment"), this, &experimental_backtrack);

	ClanTag.SetFileId("otr_clantg");
//	ClanTag.AddItem("Off");
//	ClanTag.AddItem("Default");
//	ClanTag.AddItem("FreaK Rats Kids");
	OtherOptions2.PlaceLabledControl(0, XorStr("Clan Tag"), this, &ClanTag); // requested by: https://steamcommunity.com/id/hitoridekun and https://steamcommunity.com/id/123x456x789

}

void Options::SetupMenu()
{
	Menu.Setup();
	GUI.RegisterWindow(&Menu);
	GUI.BindWindow(VK_INSERT, &Menu);
}
void Options::DoUIFrame()
{
	if (Menu.VisualsTab.FiltersAll.GetState())
	{
		Menu.VisualsTab.FiltersC4.SetState(true);
		Menu.VisualsTab.show_players.SetState(true);
		Menu.VisualsTab.FiltersWeapons.SetState(true);
		Menu.VisualsTab.FiltersNades.SetState(true);
	}
	GUI.Update();
	GUI.Draw();
}

