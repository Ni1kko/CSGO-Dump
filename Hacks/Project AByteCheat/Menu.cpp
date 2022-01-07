#include "Menu.h"
#include "Controls.h"
#include "Hooks.h" 
#include "Interfaces.h"
#include "CRC32.h"
#include <fstream>
#include "XorStr.hpp"
#define WINDOW_WIDTH 635
#define WINDOW_HEIGHT 645 // 507
mirror_window options::menu;
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
void update_world()
{
	options::menu.visuals_tab.colmodupdate.SetState(1);
}
void save_callback()
{
	int should_save = options::menu.MiscTab.ConfigListBox.GetIndex();
	std::string config_directory = "ByteCheats\\";
	config_directory += configs[should_save].name; config_directory += ".cfg";
	GUI.SaveWindowState(&options::menu, XorStr(config_directory.c_str()));
	interfaces::cvar->ConsoleColorPrintf(Color(116, 154, 30, 255), "[ByteCheats] ");
	std::string uremam = "Saved configuration.     \n";
	MsgX(uremam.c_str());
}
void load_callback()
{
	int should_load = options::menu.MiscTab.ConfigListBox.GetIndex();
	std::string config_directory = "ByteCheats\\";
	config_directory += configs[should_load].name; config_directory += ".cfg";
	GUI.LoadWindowState(&options::menu, XorStr(config_directory.c_str()));
	interfaces::cvar->ConsoleColorPrintf(Color(116, 154, 30, 255), "[ByteCheats] ");
	std::string uremam = "Loaded configuration.     \n";
	MsgX(uremam.c_str());
}

void list_configs() {
	configs.clear();
	options::menu.MiscTab.ConfigListBox.ClearItems();
	std::ifstream file_in;
	file_in.open("ByteCheats\\ByteCheats_ConfigList.txt");
	if (file_in.fail()) {
		std::ofstream("ByteCheats\\ByteCheats_ConfigList.txt");
		file_in.open("ByteCheats\\ByteCheats_ConfigList.txt");
	}
	int line_count;
	while (!file_in.eof()) {
		Config_t config;
		file_in >> config.name;
		config.id = line_count;
		configs.push_back(config);
		line_count++;
		options::menu.MiscTab.ConfigListBox.AddItem(config.name);
	}
	file_in.close();
	if (configs.size() > 7) options::menu.MiscTab.ConfigListBox.AddItem(" ");
}

void add_config() {
	std::fstream file;
	file.open("ByteCheats\\bytecheats_ConfigList.txt", std::fstream::app);
	if (file.fail()) {
		std::fstream("ByteCheats\\ByteCheats_ConfigList.txt");
		file.open("ByteCheats\\bytecheats_ConfigList.txt", std::fstream::app);
	}
	file << std::endl << options::menu.MiscTab.NewConfigName.getText();
	file.close();
	list_configs();
	int should_add = options::menu.MiscTab.ConfigListBox.GetIndex();
	std::string config_directory = "ByteCheats\\";
	config_directory += options::menu.MiscTab.NewConfigName.getText(); config_directory += ".cfg";
	GUI.SaveWindowState(&options::menu, XorStr(config_directory.c_str()));
	options::menu.MiscTab.NewConfigName.SetText("");
}

void remove_config() {
	int should_remove = options::menu.MiscTab.ConfigListBox.GetIndex();
	std::string config_directory = "ByteCheats\\cfg\\";
	config_directory += configs[should_remove].name; config_directory += ".cfg";
	std::remove(config_directory.c_str());
	std::ofstream ofs("ByteCheats\\ByteCheats_ConfigList.txt", std::ios::out | std::ios::trunc);
	ofs.close();
	std::fstream file;
	file.open("ByteCheats\\ByteCheats_ConfigList.txt", std::fstream::app);
	if (file.fail()) {
		std::fstream("ByteCheats\\ByteCheats_ConfigList.txt");
		file.open("ByteCheats\\ByteCheats_ConfigList.txt", std::fstream::app);
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
	static ConVar* Meme = interfaces::cvar->FindVar("cl_fullupdate");
	Meme->nFlags &= ~FCVAR_CHEAT;
	interfaces::engine->ClientCmd_Unrestricted("cl_fullupdate");
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

	RegisterTab(&aimbot_tab);
	RegisterTab(&AntiAimTab);
	RegisterTab(&LegitBotTab);
	RegisterTab(&visuals_tab);
	RegisterTab(&MiscTab);
	RegisterTab(&SkinTab);
	RegisterTab(&ColorsTab);

	RECT Client = GetClientArea();
	Client.bottom -= 29;

	aimbot_tab.Setup();
	LegitBotTab.Setup();
	AntiAimTab.Setup();
	visuals_tab.Setup();
	MiscTab.Setup();
	SkinTab.Setup();
	ColorsTab.Setup();

#pragma endregion
}
void CAimbotTab::Setup()
{
	SetTitle("A");
#pragma region Aimbot
	AimbotGroup.SetPosition(25, 25); // 15, 25
	AimbotGroup.SetText("Aimbot");
	AimbotGroup.SetSize(279, 555);
	RegisterControl(&AimbotGroup);

	AimbotEnable.SetFileId("aim_enable");
	AimbotGroup.PlaceLabledControl(0, "Enable", this, &AimbotEnable);

	TargetSelection.SetFileId("ragebot_targetselection");
	TargetSelection.AddItem("Cycle");
	TargetSelection.AddItem("Cycle (2x)");
	TargetSelection.AddItem("Closest");
	TargetSelection.AddItem("Highest damage");
	TargetSelection.AddItem("Highest accuracy");
	AimbotGroup.PlaceLabledControl(0, "Target selection", this, &TargetSelection);

	AvoidLimbs.SetFileId("bodyaim_ifmoving");
	AimbotGroup.PlaceLabledControl(0, "Avoid limbs if moving", this, &AvoidLimbs);

	baim_inair.SetFileId("bodyaim_inair");
	AimbotGroup.PlaceLabledControl(0, "Avoid head if jumping", this, &baim_inair);

	MultipointMultiplier.SetFileId("ragebot_multiplier");
	MultipointMultiplier.AddItem("Low");
	MultipointMultiplier.AddItem("Medium");
	MultipointMultiplier.AddItem("High");
	AimbotGroup.PlaceLabledControl(0, "", this, &MultipointMultiplier);

	TargetPointscale.SetFileId("acc_hitbox_Scale");
	TargetPointscale.SetBoundaries(0, 100);
	TargetPointscale.SetValue(50);
	TargetPointscale.extension = ("%%");
	AimbotGroup.PlaceLabledControl(0, "Multi-point scale", this, &TargetPointscale);

	//custom_hitscan.SetFileId("auto_hitscan_auto");
	//AimbotGroup.PlaceLabledControl(0, "Dynamic multipoint", this, &custom_hitscan);;

	AimbotAutoFire.SetFileId("aim_autofire");
	AimbotGroup.PlaceLabledControl(0, "Automatic fire", this, &AimbotAutoFire);

	AccuracyAutoWall.SetFileId("ragebot_autowall");
	AimbotGroup.PlaceLabledControl(0, "Automatic penetration", this, &AccuracyAutoWall);

	AimbotSilentAim.SetFileId("aim_silent");
	AimbotGroup.PlaceLabledControl(0, "Silent aim", this, &AimbotSilentAim);

	hc.SetFileId("ab_hitchance");
	hc.SetBoundaries(0, 100);
	hc.SetValue(40);
	hc.extension = XorStr("%%");
	AimbotGroup.PlaceLabledControl(0, "Minimum hit chance", this, &hc);

	md.SetFileId("ab_minimumdamage");
	md.SetBoundaries(0, 100);
	md.SetValue(10);
	md.extension = XorStr("%%");
	AimbotGroup.PlaceLabledControl(0, "Minimum damage", this, &md);

	OverrideAWP.SetFileId("ragebot_overrideawp");
	AimbotGroup.PlaceLabledControl(0, "Override AWP", this, &OverrideAWP);

	AccuracyAutoScope.SetFileId("acc_scope");
	AimbotGroup.PlaceLabledControl(0, "Auto Scope", this, &AccuracyAutoScope);

	AimStep.SetFileId("ragebot_aimstep");
	AimbotGroup.PlaceLabledControl(0, "Reduce aim step", this, &AimStep);

	AimbotFov.SetFileId("aim_fov");
	AimbotFov.SetBoundaries(0.f, 180.f);
	AimbotFov.extension = XorStr("");
	AimbotFov.SetValue(180.f);
	AimbotGroup.PlaceLabledControl(0, "Maximum FOV", this, &AimbotFov);

	LogMisses.SetFileId("ragebot_logmissed");
	AimbotGroup.PlaceLabledControl(0, "Log misses due to spread", this, &LogMisses);

	LowFPS.SetFileId("ragebot_lowfps");
	LowFPS.AddItem("Off");
	LowFPS.AddItem("Force low backtrack");
	LowFPS.AddItem("Force low multipoint");
	AimbotGroup.PlaceLabledControl(0, "Low FPS mitigations", this, &LowFPS);

	prefer_head.SetFileId("acc_prefer_head");
	prefer_head.items.push_back(dropdownboxitem(false, XorStr("If Moving")));
	//	prefer_head.items.push_back(dropdownboxitem(false, XorStr("Lower body Is Unhittable")));
	prefer_head.items.push_back(dropdownboxitem(false, XorStr("No Fake")));
	//	prefer_head.items.push_back(dropdownboxitem(false, XorStr("Head Is Visible")));
	//	prefer_head.items.push_back(dropdownboxitem(false, XorStr("")));
	AimbotGroup.PlaceLabledControl(0, "Prefer Head Aim Factors", this, &prefer_head);

	AccuracyHitchance.SetFileId("base_hc");
	AccuracyHitchance.SetBoundaries(0.f, 100.f);
	AccuracyHitchance.SetValue(20);

	weapongroup.SetText("Other");
	weapongroup.SetPosition(322, 25); // 15, 230
	weapongroup.SetSize(271, 555);

	RegisterControl(&weapongroup);

	weapongroup.AddTab(CGroupTab("AS", 1));
	weapongroup.AddTab(CGroupTab("Pistols", 2));
	weapongroup.AddTab(CGroupTab("Scout", 3));
	weapongroup.AddTab(CGroupTab("Awp", 4));
	weapongroup.AddTab(CGroupTab("SMG", 5));
	weapongroup.AddTab(CGroupTab("Others", 6));
	weapongroup.AddTab(CGroupTab("Misc", 7));
	RegisterControl(&weapongroup);

	target_auto.SetFileId("tgt_hitbox_auto");
	target_auto.items.push_back(dropdownboxitem(false, XorStr("Head")));
	target_auto.items.push_back(dropdownboxitem(false, XorStr("Upper Body")));
	target_auto.items.push_back(dropdownboxitem(false, XorStr("Lower Body")));
	target_auto.items.push_back(dropdownboxitem(false, XorStr("Arms")));
	target_auto.items.push_back(dropdownboxitem(false, XorStr("Legs")));
	weapongroup.PlaceLabledControl(1, "HitScan", this, &target_auto);

	custom_hitscan.SetFileId("auto_hitscan_auto");
	weapongroup.PlaceLabledControl(1, "Automatic HitScan", this, &custom_hitscan);

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

	//----------------------------------------------------------------------

	target_smg.SetFileId("tgt_hitbox_smg");
	target_smg.items.push_back(dropdownboxitem(false, XorStr("Head")));
	target_smg.items.push_back(dropdownboxitem(false, XorStr("Upper Body")));
	target_smg.items.push_back(dropdownboxitem(false, XorStr("Lower Body")));
	target_smg.items.push_back(dropdownboxitem(false, XorStr("Arms")));
	target_smg.items.push_back(dropdownboxitem(false, XorStr("Legs")));
	weapongroup.PlaceLabledControl(5, "HitScan", this, &target_smg);

	hc_smg.SetFileId("smg_hitchance");
	hc_smg.SetBoundaries(0, 100);
	hc_smg.SetValue(25);
	hc_smg.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(5, "Hitchance", this, &hc_smg);

	md_smg.SetFileId("smg_minimumdamage");
	md_smg.SetBoundaries(0, 100);
	md_smg.SetValue(25);
	md_smg.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(5, "Minimum Damage", this, &md_smg);

	//----------------------------------------------------------------------

	target_otr.SetFileId("tgt_hitbox_otr");
	target_otr.items.push_back(dropdownboxitem(false, XorStr("Head")));
	target_otr.items.push_back(dropdownboxitem(false, XorStr("Upper Body")));
	target_otr.items.push_back(dropdownboxitem(false, XorStr("Lower Body")));
	target_otr.items.push_back(dropdownboxitem(false, XorStr("Arms")));
	target_otr.items.push_back(dropdownboxitem(false, XorStr("Legs")));
	weapongroup.PlaceLabledControl(6, "HitScan", this, &target_otr);

	hc_otr.SetFileId("otr_hitchance");
	hc_otr.SetBoundaries(0, 100);
	hc_otr.SetValue(25);
	hc_otr.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(6, "Hitchance", this, &hc_otr);

	md_otr.SetFileId("otr_minimumdamage");
	md_otr.SetBoundaries(0, 100);
	md_otr.SetValue(25);
	md_otr.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(6, "Minimum Damage", this, &md_otr);

	//----------------------------------------------------------------------

	RemoveSpread.SetFileId("other_removespread");
	weapongroup.PlaceLabledControl(7, "Remove spread", this, &RemoveSpread);

	RemoveRecoil.SetFileId("other_removerecoil");
	weapongroup.PlaceLabledControl(7, "Remove recoil", this, &RemoveRecoil);

	AccuracyBoost.SetFileId("ragebot_accuracyboost");
	AccuracyBoost.AddItem("Off");
	AccuracyBoost.AddItem("Default");
	AccuracyBoost.AddItem("Maximum");
	weapongroup.PlaceLabledControl(7, "Accuracy boost", this, &AccuracyBoost);

	AccuracyBoostOptions.SetFileId("ragebot_accuracyboostoptions");
	AccuracyBoostOptions.AddItem("None");
	AccuracyBoostOptions.AddItem("Accurate history");
	AccuracyBoostOptions.AddItem("Accurate un-lag");
	weapongroup.PlaceLabledControl(7, "Accuracy boost options", this, &AccuracyBoostOptions);

	QuickStopKey.SetFileId("ragebot_quickstop_onkey");
	weapongroup.PlaceLabledControl(7, "", this, &QuickStopKey);

	QuickStopCheck.SetFileId("ragebot_quickstop_check");
	weapongroup.PlaceLabledControl(7, "Quickstop", this, &QuickStopCheck);

	QuickStop.SetFileId("ragebot_quickstop");
	QuickStop.AddItem("Off");
	QuickStop.AddItem("Automatic");
	QuickStop.AddItem("On key");
	weapongroup.PlaceLabledControl(7, "", this, &QuickStop);

	PreferBodyAim.SetFileId("ragebot_accuracyboostoptions");
	PreferBodyAim.AddItem("Off");
	PreferBodyAim.AddItem("Not accurate history");
	PreferBodyAim.AddItem("Inaccuracy");
	weapongroup.PlaceLabledControl(7, "Prefer body aim", this, &PreferBodyAim);

	BaimOnKey.SetFileId("ragebot_forcebaimkey");
	weapongroup.PlaceLabledControl(7, "", this, &BaimOnKey);

	ForceBaimCheck.SetFileId("ragebot_forcebaimcheck");
	weapongroup.PlaceLabledControl(7, "Force body aim", this, &ForceBaimCheck);

	msc_otr.SetFileId("acc_extra_P_lation");
	weapongroup.PlaceLabledControl(7, "Extrapolation", this, &extrapolation);

	preso.SetFileId("acc_zeusisgay");
	preso.AddItem("Down");
	preso.AddItem("Up");
	preso.AddItem("Automatic Spread");
	weapongroup.PlaceLabledControl(7, "Pitch Adjustment", this, &preso);

	resolver.SetFileId("acc_aaa");
	resolver.AddItem("Default");
	resolver.AddItem("Primary");
	resolver.AddItem("Beta");
	//	resolver.AddItem("experimental");
	weapongroup.PlaceLabledControl(7, "Resolver", this, &resolver);

	Multienable.SetFileId("multipoint_enable");
	weapongroup.PlaceLabledControl(7, "Toggle Multipoint", this, &Multienable);

	Multival2.SetFileId("hitbox_scale_head");
	Multival2.SetBoundaries(0.1, 100);
	Multival2.SetValue(20);
	Multival2.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(7, "Head Multipoint", this, &Multival2);

	Multival4.SetFileId("hitbox_scale_upperbody");
	Multival4.SetBoundaries(0.1, 100);
	Multival4.SetValue(20);
	Multival4.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(7, "Upper Body Multipoint", this, &Multival4);

	Multival.SetFileId("hitbox_scale_body");
	Multival.SetBoundaries(0.1, 100);
	Multival.SetValue(20);
	Multival.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(7, "Lower Body Multipoint", this, &Multival);

	MultiVal3.SetFileId("hitbox_scale_legs");
	MultiVal3.SetBoundaries(0.1, 100);
	MultiVal3.SetValue(20);
	MultiVal3.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(7, "Leg Multipoint", this, &MultiVal3);

	lag_pred.SetFileId("lag_pred");
	weapongroup.PlaceLabledControl(7, "Position Adjustment", this, &lag_pred);

	//misc_otr.SetFileId("bodyaim_fake");
	//weapongroup.PlaceLabledControl(7, "If Fake", this, &baim_fake); // if we have to resort to a brute

	//misc_otr.SetFileId("bodyaim_fakewalk");
	//weapongroup.PlaceLabledControl(7, "If Slow Walk", this, &baim_fakewalk);

	//misc_otr.SetFileId("bodyaim_inair");
	//weapongroup.PlaceLabledControl(7, "If In Air", this, &baim_inair); //if they be flyin like a plane

	//misc_otr.SetFileId("acc_BaimIfUnderXHealth");
	//BaimIfUnderXHealth.SetBoundaries(0, 100);
	//BaimIfUnderXHealth.extension = XorStr("HP");
	//BaimIfUnderXHealth.SetValue(0);
	//weapongroup.PlaceLabledControl(7, "If HP Lower Than", this, &BaimIfUnderXHealth);

	//misc_otr.SetFileId("acc_bigbaim");
	//weapongroup.PlaceLabledControl(7, "On Key", this, &bigbaim);

	//	headonly_if_vis_otr.SetFileId("headonly_if_vis_otr");
	//	weapongroup.PlaceLabledControl(5, "Headshot Only If Hittable", this, &headonly_if_vis_otr);

	//	preset_otr.SetFileId("otr_automatic_cfg");
	//	weapongroup.PlaceLabledControl(5, "Automatic  Weapon Configuration", this, &preset_otr);

#pragma endregion  AntiAim controls get setup in here
}

void CLegitBotTab::Setup()
{
	SetTitle("B");
	AimbotGroup.SetText("Aimbot");
	AimbotGroup.SetPosition(25, 25);
	AimbotGroup.SetSize(271, 555);
	RegisterControl(&AimbotGroup);
	RegisterControl(&Active);
	AimbotKeyBind.SetFileId("l_aimkey");
	AimbotGroup.PlaceLabledControl(1, "", this, &AimbotKeyBind);
	Active.SetFileId("active");
	AimbotGroup.PlaceLabledControl(1, "Enabled", this, &Active);
	WeaponMainSpeed.SetFileId("l_rspeed"); 
	WeaponMainSpeed.SetBoundaries(0, 10);
	WeaponMainSpeed.SetValue(5);
	AimbotGroup.PlaceLabledControl(1, "Speed", this, &WeaponMainSpeed);
	TriggerHitChanceAmmount.SetFileId("l_trigHC");
	TriggerHitChanceAmmount.SetBoundaries(0, 100);
	TriggerHitChanceAmmount.extension = XorStr("");
	TriggerHitChanceAmmount.SetValue(0);
	AimbotGroup.PlaceLabledControl(2, "Speed (in attack)", this, &TriggerHitChanceAmmount);
	TriggerRecoil.SetFileId("l_trigRCS");
    TriggerRecoil.SetBoundaries(0.f, 100.f);
    TriggerRecoil.extension = XorStr("%%");
    TriggerRecoil.SetValue(5.00f);
    AimbotGroup.PlaceLabledControl(2, "Speed scale - FOV", this, &TriggerRecoil);
	WeaponMainRecoil.SetFileId("l_rRecoil");
    WeaponMainRecoil.SetBoundaries(0, 100);
	WeaponMainRecoil.extension = XorStr("%%");
    WeaponMainRecoil.SetValue(100);
	AimbotGroup.PlaceLabledControl(1, "Maximum lock-on time", this, &WeaponMainRecoil);
	WeaponMGSkeet.SetFileId("l_mgRecoil");
	WeaponMGSkeet.extension = XorStr("ms");
	WeaponMGSkeet.SetBoundaries(0, 200);
	WeaponMGSkeet.SetValue(100);
	AimbotGroup.PlaceLabledControl(5, "Reaction time", this, &WeaponMGSkeet);
	WeaponSnipFoV.SetFileId("l_sfov");
	WeaponSnipFoV.extension = XorStr("");
    WeaponSnipFoV.SetBoundaries(0, 100);
    WeaponSnipFoV.SetValue(10);
	AimbotGroup.PlaceLabledControl(3, "Maximum FOV", this, &WeaponSnipFoV);
	WeaponSnipRecoil.SetFileId("l_sRecoil");
	WeaponSnipRecoil.extension = XorStr("%%");
	WeaponSnipRecoil.SetBoundaries(0, 100);
	WeaponSnipRecoil.SetValue(100);
	AimbotGroup.PlaceLabledControl(3, "Recoil compensation (P/Y)", this, &WeaponSnipRecoil);
	WeaponMGFoV.SetFileId("l_mgfov");
	WeaponMGFoV.extension = XorStr("%%");
	WeaponMGFoV.SetBoundaries(0, 100);
	WeaponMGFoV.SetValue(10);
	AimbotGroup.PlaceLabledControl(5, "", this, &WeaponMGFoV);
	AimbotSmokeCheck.SetFileId("l_smokeaimbot");
    AimbotGroup.PlaceLabledControl(1, "Quick stop", this, &AimbotSmokeCheck);
	BackTrack.SetFileId("l_backtrack");
    AimbotGroup.PlaceLabledControl(1, "Aim through smoke", this, &BackTrack);
	TriggerHead.SetFileId("l_trighead");
	AimbotGroup.PlaceLabledControl(3, "Aim while blind", this, &TriggerHead);
	TriggerLegs.SetFileId("l_triglegs");
    AimbotGroup.PlaceLabledControl(3, "Head", this, &TriggerLegs);
    TriggerStomach.SetFileId("l_trigstomach");
    AimbotGroup.PlaceLabledControl(3, "Chest", this, &TriggerStomach);
    TriggerArms.SetFileId("l_trigarms");
    AimbotGroup.PlaceLabledControl(3, "Stomach", this, &TriggerArms);

	weapongroup.SetText("Triggerbot");
	weapongroup.SetPosition(322, 25);
	weapongroup.SetSize(271, 360);
	RegisterControl(&weapongroup);
	RegisterControl(&Active);
	AimbotKeyBind.SetFileId("l_aimkey");
    weapongroup.PlaceLabledControl(1, "", this, &AimbotKeyBind);
	weapongroup.PlaceLabledControl(1, "Enabled", this, &legitresolver);
	WeaponPistSpeed.SetFileId("l_pspeed");
	WeaponPistSpeed.extension = XorStr("%%");
    WeaponPistSpeed.SetBoundaries(0, 75);
    WeaponPistSpeed.SetValue(5);
    weapongroup.PlaceLabledControl(2, "Minimum hit chance", this, &WeaponPistSpeed);
	TriggerRecoilSkeet.SetFileId("l_trigRCS");
	TriggerRecoilSkeet.SetBoundaries(0.f, 380.f);
	TriggerRecoilSkeet.extension = XorStr("ms");
	TriggerRecoilSkeet.SetValue(5.00f);
	weapongroup.PlaceLabledControl(2, "Reaction time", this, &TriggerRecoilSkeet);
	weapongroup.PlaceLabledControl(2, "Burst fire", this, &TriggerEnable);
	WeaponMGRecoil.SetFileId("l_mgRecoil");
	WeaponMGRecoil.SetBoundaries(0, 200);
	WeaponMGRecoil.SetValue(165);
	weapongroup.PlaceLabledControl(5, "", this, &WeaponMGRecoil);
	WeaponMGSpeed.SetFileId("l_mgspeed");
	WeaponMGSpeed.SetBoundaries(1, 100);
	WeaponMGSpeed.SetValue(5);
	weapongroup.PlaceLabledControl(5, "Minimum damage", this, &WeaponMGSpeed);
	TriggerAuto.SetFileId("t_triggerbotenable");
	weapongroup.PlaceLabledControl(2, "Automatic penetration", this, &TriggerAuto);
	TriggerHeadSkeet.SetFileId("t_triggerbotenable");
	weapongroup.PlaceLabledControl(2, "Head", this, &TriggerHeadSkeet);
	TriggerChestSkeet.SetFileId("t_triggerbotenable");
	weapongroup.PlaceLabledControl(2, "Chest", this, &TriggerChestSkeet);
	WeaponMpHitbox.AddItem("Stomach");
	TriggerStomach.SetFileId("t_triggerbotenable");
	weapongroup.PlaceLabledControl(2, "Burst fire", this, &TriggerStomachSkeet);

	normalgroup.SetText("Other");
	normalgroup.SetPosition(322, 410);
	normalgroup.SetSize(271, 170);
	RegisterControl(&normalgroup);
	RegisterControl(&Active);
	WeaponMainHitbox.SetFileId("l_rhitbox");
    WeaponMainHitbox.AddItem("Low");
    WeaponMainHitbox.AddItem("High");
	normalgroup.PlaceLabledControl(1, "Accuracy boost", this, &WeaponMainHitbox);
	aaenable.SetFileId("AA_anable");
	normalgroup.PlaceLabledControl(4, "Standalone recoil compensation", this, &aaenable);



	//aimbotfiremode.AddItem("On Shot");
	//aimbotfiremode.AddItem("Automatic");
	//AimbotGroup.PlaceLabledControl(1, "Fire Mode", this, &aimbotfiremode);
	//AimbotKeyBind.SetFileId("l_aimkey");
	//AimbotGroup.PlaceLabledControl(1, "KeyBind", this, &AimbotKeyBind);
	//BackTrack.SetFileId("l_backtrack");
	//AimbotGroup.PlaceLabledControl(1, "Backtrack", this, &BackTrack);
	//AimbotSmokeCheck.SetFileId("l_smokeaimbot");
	//AimbotGroup.PlaceLabledControl(1, "Smoke Check", this, &AimbotSmokeCheck);

	//legitresolver.SetFileId("l_B1GresolverTappingSkeet");
	//AimbotGroup.PlaceLabledControl(1, "Resolver", this, &legitresolver);//

	//---- Trigger ---//
	//TriggerEnable.SetFileId("t_triggerbotenable");
	//AimbotGroup.PlaceLabledControl(2, "Activate Trigger", this, &TriggerEnable);
	//triggertype.SetFileId("t_triggerbottyp");
	//triggertype.AddItem("None");
	//triggertype.AddItem("Automatic");
	//AimbotGroup.PlaceLabledControl(2, "Trigger Mode", this, &triggertype);
	//TriggerHitChanceAmmount.SetFileId("l_trigHC");
	//TriggerHitChanceAmmount.SetBoundaries(0, 100);
	//TriggerHitChanceAmmount.extension = XorStr("%%");
	////TriggerHitChanceAmmount.SetValue(0);
	//AimbotGroup.PlaceLabledControl(2, "Hitchance", this, &TriggerHitChanceAmmount);
	////TriggerSmokeCheck.SetFileId("l_trigsmoke");
	//AimbotGroup.PlaceLabledControl(2, "Smoke Check", this, &TriggerSmokeCheck);
	//TriggerRecoil.SetFileId("l_trigRCS");
	//TriggerRecoil.SetBoundaries(0.f, 100.f);
	//TriggerRecoil.extension = XorStr("%%");
	//TriggerRecoil.SetValue(5.00f);
	//AimbotGroup.PlaceLabledControl(2, "Recoil", this, &TriggerRecoil);
	//TriggerKeyBind.SetFileId("l_trigkey");
	//AimbotGroup.PlaceLabledControl(2, "KeyBind", this, &TriggerKeyBind);
	// ---- Hitboxes ---- //
	//TriggerHead.SetFileId("l_trighead");
	//AimbotGroup.PlaceLabledControl(3, "Head", this, &TriggerHead);
	//TriggerChest.SetFileId("l_trigchest");
	//AimbotGroup.PlaceLabledControl(3, "Chest", this, &TriggerChest);
	//TriggerStomach.SetFileId("l_trigstomach");
	//AimbotGroup.PlaceLabledControl(3, "Stomach", this, &TriggerStomach);
	//TriggerArms.SetFileId("l_trigarms");
	//AimbotGroup.PlaceLabledControl(3, "Arms", this, &TriggerArms);
	//TriggerLegs.SetFileId("l_triglegs");
	//AimbotGroup.PlaceLabledControl(3, "Legs", this, &TriggerLegs);

	//--------------------------legitaa--------------------------((
	//	aaenable.SetFileId("AA_anable");
	//AimbotGroup.PlaceLabledControl(4, "Legit AA Enable", this, &aaenable);

	//aatyp.SetFileId("AA_aatyp");
	//aatyp.AddItem("Static");
	//aatyp.AddItem("Lowerbody");
	//aatyp.AddItem("Freestanding");
	//AimbotGroup.PlaceLabledControl(4, "Anti-Aim", this, &aatyp);

	//aatyp2.SetFileId("AA_aatyp2");
	//aatyp2.AddItem("Default");
	//aatyp2.AddItem("Jitter");
	//aatyp2.AddItem("Shuffle");
	//aatyp2.AddItem("Spin");
	//AimbotGroup.PlaceLabledControl(4, "Anti-Aim Type", this, &aatyp2);
	//
	//aatyp3.SetFileId("AA_aatyp3");
	//aatyp3.SetBoundaries(0, 90);
	//aatyp3.SetValue(20);
	//AimbotGroup.PlaceLabledControl(4, "Anti-Aim Value", this, &aatyp3);

	//aafl.SetFileId("AA_aaFL");
	//aafl.SetBoundaries(1, 4);
//	a//afl.SetValue(1);
	//AimbotGroup.PlaceLabledControl(4, "Legit FakeLag", this, &aafl);
	//----------------------solid kys---------------------//

	//weapongroup.SetText("Main");
	//weapongroup.SetPosition(4, 256);
	//weapongroup.SetSize(550, 183);
	//weapongroup.AddTab(CGroupTab("Rifle", 1));
	//weapongroup.AddTab(CGroupTab("Pistol", 2));
	//weapongroup.AddTab(CGroupTab("Sniper", 3));
	//weapongroup.AddTab(CGroupTab("SMG", 4));
	//weapongroup.AddTab(CGroupTab("Heavy", 5));
	//RegisterControl(&weapongroup);
	//RegisterControl(&Active);
	//WeaponMainHitbox.SetFileId("l_rhitbox");
	//WeaponMainHitbox.AddItem("Head");
	//WeaponMainHitbox.AddItem("Neck");
	//WeaponMainHitbox.AddItem("Chest");
	//WeaponMainHitbox.AddItem("Stomach");
	//WeaponMainHitbox.AddItem("Nearest");
	//weapongroup.PlaceLabledControl(1, "Hitbox", this, &WeaponMainHitbox);
	//WeaponMainSpeed.SetFileId("l_rspeed");
	//WeaponMainSpeed.SetBoundaries(0, 75);
	//WeaponMainSpeed.SetValue(5);
	//weapongroup.PlaceLabledControl(1, "Speed", this, &WeaponMainSpeed);
	//WeaponMainRecoil.SetFileId("l_rRecoil");
	//WeaponMainRecoil.SetBoundaries(0, 200);
	////WeaponMainRecoil.SetValue(165);
	//weapongroup.PlaceLabledControl(1, "Recoil", this, &WeaponMainRecoil);
	//WeaponMainFoV.SetFileId("l_fov");
	//WeaponMainFoV.SetBoundaries(0, 45);
	//WeaponMainFoV.SetValue(10);
	//weapongroup.PlaceLabledControl(1, "Field Of View", this, &WeaponMainFoV);
	// --- Pistols --- //
	//WeaponPistHitbox.SetFileId("l_phitbox");
	//WeaponPistHitbox.AddItem("Head");
	//WeaponPistHitbox.AddItem("Neck");
	//WeaponPistHitbox.AddItem("Chest");
	//.AddItem("Stomach");
	//WeaponPistHitbox.AddItem("Nearest");
	//weapongroup.PlaceLabledControl(2, "Hitbox", this, &WeaponPistHitbox);
	//WeaponPistSpeed.SetFileId("l_pspeed");
	//WeaponPistSpeed.SetBoundaries(0, 75);
	//WeaponPistSpeed.SetValue(5);
	//weapongroup.PlaceLabledControl(2, "Speed", this, &WeaponPistSpeed);
//	WeaponPistRecoil.SetFileId("l_pRecoil");
	//	WeaponPistRecoil.SetBoundaries(0, 200);
//	WeaponPistRecoil.SetValue(165);
//	weapongroup.PlaceLabledControl(2, "Recoil", this, &WeaponPistRecoil);
//	WeaponPistFoV.SetFileId("l_pfov");
//	WeaponPistFoV.SetBoundaries(0, 45);
	//	WeaponPistFoV.SetValue(10);
//	weapongroup.PlaceLabledControl(2, "Field Of View", this, &WeaponPistFoV);
	// --- Sniper --- //
	//WeaponSnipHitbox.SetFileId("l_shitbox");
	//WeaponSnipHitbox.AddItem("Head");
	//.AddItem("Neck");
	//WeaponSnipHitbox.AddItem("Chest");
	//WeaponSnipHitbox.AddItem("Stomach");
	//WeaponSnipHitbox.AddItem("Nearest");
	//weapongroup.PlaceLabledControl(3, "Hitbox", this, &WeaponSnipHitbox);
	//WeaponSnipSpeed.SetFileId("l_sspeed");
	//WeaponSnipSpeed.SetBoundaries(0, 75);
//	WeaponSnipSpeed.SetValue(5);
	//	weapongroup.PlaceLabledControl(3, "Speed", this, &WeaponSnipSpeed);
//	WeaponSnipRecoil.SetFileId("l_sRecoil");
//	WeaponSnipRecoil.SetBoundaries(0, 200);
	//	WeaponSnipRecoil.SetValue(165);
//	weapongroup.PlaceLabledControl(3, "Recoil", this, &WeaponSnipRecoil);
	//WeaponSnipFoV.SetFileId("l_sfov");
	//WeaponSnipFoV.SetBoundaries(0, 45);
	//WeaponSnipFoV.SetValue(10);
	//weapongroup.PlaceLabledControl(3, "Field Of View", this, &WeaponSnipFoV);
	// --- SMG --- //
	//WeaponMpHitbox.SetFileId("l_sniphitbox");
	//WeaponMpHitbox.AddItem("Head");
	//	WeaponMpHitbox.AddItem("Neck");
	//WeaponMpHitbox.AddItem("Chest");
	//WeaponMpHitbox.AddItem("Stomach");
	//WeaponMpHitbox.AddItem("Nearest");
	//weapongroup.PlaceLabledControl(4, "Hitbox", this, &WeaponMpHitbox);
	//WeaponMpSpeed.SetFileId("l_sspeed");
	//WeaponMpSpeed.SetBoundaries(0, 75);
	//WeaponMpSpeed.SetValue(5);
	//weapongroup.PlaceLabledControl(4, "Speed", this, &WeaponMpSpeed);
	//WeaponMpRecoil.SetFileId("l_sRecoil");
	//WeaponMpRecoil.SetBoundaries(0, 200);
	//WeaponMpRecoil.SetValue(165);
	//weapongroup.PlaceLabledControl(4, "Recoil", this, &WeaponMpRecoil);
	//WeaponMpFoV.SetFileId("l_sfov");
	//WeaponMpFoV.SetBoundaries(0, 45);
	//WeaponMpFoV.SetValue(10);
	//weapongroup.PlaceLabledControl(4, "Field Of View", this, &WeaponMpFoV);
	// --- MachineGun --- //
	//WeaponMGHitbox.SetFileId("l_mghitbox");
	//WeaponMGHitbox.AddItem("Head");
//	WeaponMGHitbox.AddItem("Neck");
	////WeaponMGHitbox.AddItem("Chest");
	//WeaponMGHitbox.AddItem("Stomach");
	//	WeaponMGHitbox.AddItem("Nearest");
//	weapongroup.PlaceLabledControl(5, "Hitbox", this, &WeaponMGHitbox);
	//WeaponMGSpeed.SetFileId("l_mgspeed");
//	WeaponMGSpeed.SetBoundaries(0, 75);
	//	//WeaponMGSpeed.SetValue(5);
	//weapongroup.PlaceLabledControl(5, "Speed", this, &WeaponMGSpeed);
	//WeaponMGRecoil.SetFileId("l_mgRecoil");
	//WeaponMGRecoil.SetBoundaries(0, 200);
	//WeaponMGRecoil.SetValue(165);
	//weapongroup.PlaceLabledControl(5, "Recoil", this, &WeaponMGRecoil);
	//WeaponMGFoV.SetFileId("l_mgfov");
	//WeaponMGFoV.SetBoundaries(0, 45);
	//WeaponMGFoV.SetValue(10);
	//weapongroup.PlaceLabledControl(5, "Field Of View", this, &WeaponMGFoV);
}

void CAntiAimTab::Setup()
{
	SetTitle("G");

	AntiAimbotAngles.SetText("Anti-aimbot angles");
	AntiAimbotAngles.SetPosition(25, 25);
	AntiAimbotAngles.SetSize(271, 555);
	RegisterControl(&AntiAimbotAngles);

	AAOnKey.SetFileId("antiaim_enable_onkey");
	AntiAimbotAngles.PlaceLabledControl(0, "", this, &AAOnKey);

	AEnable.SetFileId("antiaim_enable");
	AntiAimbotAngles.PlaceLabledControl(0, "Enable", this, &AEnable);

	Pitch.SetFileId("antiaim_pitch");
	Pitch.AddItem("Off");
	Pitch.AddItem("Default");
	Pitch.AddItem("Up");
	Pitch.AddItem("Zero");
	Pitch.AddItem("Random");
	AntiAimbotAngles.PlaceLabledControl(0, "Pitch", this, &Pitch);

	YawBase.SetFileId("antiaim_yawbase");
	YawBase.AddItem("Local view");
	YawBase.AddItem("Automatic");
	AntiAimbotAngles.PlaceLabledControl(0, "Yaw base", this, &YawBase);

	Yaw.SetFileId("antiaim_yaw");
	Yaw.AddItem("Off");
	Yaw.AddItem("Backward");
	AntiAimbotAngles.PlaceLabledControl(0, "Yaw", this, &Yaw);

	YawOffset.SetFileId("antiaim_yawoffset");
	YawOffset.SetBoundaries(-180, 180);
	YawOffset.SetValue(0);
	//YawOffset.extension = XorStr("���");
	AntiAimbotAngles.PlaceLabledControl(0, "", this, &YawOffset);

	YawJitter.SetFileId("antiaim_yawjiter");
	YawJitter.AddItem("Off");
	YawJitter.AddItem("Addition");
	YawJitter.AddItem("Removal");
	YawJitter.AddItem("Random");
	AntiAimbotAngles.PlaceLabledControl(0, "Yaw jitter", this, &YawJitter);

	YawJitterOffset.SetFileId("antiaim_yawjitteroffset");
	YawJitterOffset.SetBoundaries(-180, 180);
	YawJitterOffset.SetValue(0);
	//YawJitterOffset.extension = XorStr("���");
	AntiAimbotAngles.PlaceLabledControl(0, "", this, &YawJitterOffset);

	FakeYaw.SetFileId("antiaim_fakeyaw");
	FakeYaw.AddItem("Off");
	FakeYaw.AddItem("Simple");
	FakeYaw.AddItem("Balance");
	FakeYaw.AddItem("Stretch");
	FakeYaw.AddItem("Crooked");
	FakeYaw.AddItem("Nae Nae");
	FakeYaw.AddItem("Jitter");
	FakeYaw.AddItem("Static");
	FakeYaw.AddItem("Shuffle");	AntiAimbotAngles.PlaceLabledControl(0, "Fake yaw", this, &FakeYaw);

	FakeYawOffset.SetFileId("antiaim_fakeyawoffset");
	FakeYawOffset.SetBoundaries(-58, 58);
	FakeYawOffset.SetValue(0);
	//FakeYawOffset.extension = XorStr("���");
	AntiAimbotAngles.PlaceLabledControl(0, "", this, &FakeYawOffset);

	//BodyYaw.SetFileId("antiaim_bodyyaw");
	//BBodyYaw.AddItem("Off");
	//BBodyYaw.AddItem("Side");
	//BBodyYaw.AddItem("Opposite");
	//BBodyYaw.AddItem("Random");
	//BBodyYaw.AddItem("Automatic");
	//BAntiAimbotAngles.PlaceLabledControl(0, "Body yaw", this, &BodyYaw);

	EdgeYaw.SetFileId("antiaim_edgeyaw");
	EdgeYaw.AddItem("Off");
	EdgeYaw.AddItem("On");
	AntiAimbotAngles.PlaceLabledControl(0, "Edge yaw", this, &EdgeYaw);

	Freestanding.SetFileId("antiaim_freestanding");
	Freestanding.AddItem("-");
	Freestanding.AddItem("Default");
	Freestanding.AddItem("Desynchronization");
	AntiAimbotAngles.PlaceLabledControl(0, "Freestanding", this, &Freestanding);

	Twist.SetFileId("antiaim_twist");
	AntiAimbotAngles.PlaceLabledControl(0, "Twist", this, &Twist);

	LowerBodyYaw.SetFileId("antiaim_lby");
	AntiAimbotAngles.PlaceLabledControl(0, "Lower body yaw", this, &LowerBodyYaw);


	Fakelag.SetText("Fakelag");
	Fakelag.SetPosition(322, 25);
	Fakelag.SetSize(271, 360);
	RegisterControl(&Fakelag);

	FakelagKey.SetFileId("antiaim_fakelag_onkey");
	Fakelag.PlaceLabledControl(0, "", this, &FakelagKey);

	FEnable.SetFileId("fakelag_enable");
	Fakelag.PlaceLabledControl(0, "Enable", this, &FEnable);

	Triggers.SetFileId("fakelag_triggers");
	Triggers.items.push_back(dropdownboxitem(false, XorStr("Standing")));
	Triggers.items.push_back(dropdownboxitem(false, XorStr("Moving")));
	Triggers.items.push_back(dropdownboxitem(false, XorStr("In air")));
	Triggers.items.push_back(dropdownboxitem(false, XorStr("Unducking")));
	Fakelag.PlaceLabledControl(0, "Triggers", this, &Triggers);

	Amount.SetFileId("fakelag_amount");
	Amount.AddItem("Static");
	Amount.AddItem("Dynamic");
	Amount.AddItem("Break");
	Fakelag.PlaceLabledControl(0, "Amount", this, &Amount);

	Variance.SetFileId("fakelag_variance");
	Variance.SetBoundaries(0, 100);
	Variance.extension = "%%";
	Fakelag.PlaceLabledControl(0, "Variance", this, &Variance);

	Limit.SetFileId("fakelag_limit");
	Limit.SetBoundaries(0, 15);
	Fakelag.PlaceLabledControl(0, "Limit", this, &Limit);

	WhileShooting.SetFileId("fakelag_whileshooting");
	Fakelag.PlaceLabledControl(0, "Fake lag while shooting", this, &WhileShooting);

	BHopReset.SetFileId("fakelag_reset");
	Fakelag.PlaceLabledControl(0, "Reset on bunny hop", this, &BHopReset);


	Other.SetText("Other");
	Other.SetPosition(322, 410);
	Other.SetSize(271, 170);
	RegisterControl(&Other);

	SlowMotionKey.SetFileId("aa_slowmotion_onkey");
	Other.PlaceLabledControl(1, "", this, &SlowMotionKey);

	SlowMotion.SetFileId("other_slowmotion");
	Other.PlaceLabledControl(0, "Slow motion", this, &SlowMotion);

	FakeCrouchKey.SetFileId("fake_crouch_key");
	Other.PlaceLabledControl(3, "", this, &FakeCrouchKey);

	FakeCrouch.SetFileId("fake_crouch");
	Other.PlaceLabledControl(3, "Fake duck", this, &FakeCrouch);

	InfiniteDuck.SetFileId("other_infduck");
	Other.PlaceLabledControl(0, "Infinite duck", this, &InfiniteDuck);

	OnShotAA.SetFileId("other_onshotaa");
	Other.PlaceLabledControl(0, "On shot anti-aim", this, &OnShotAA);
}

void CVisualTab::Setup()
{
	show_players.SetState(true);
	SetTitle("C");
#pragma region Options
	OptionsGroup.SetText("Player ESP");
	OptionsGroup.SetPosition(25, 25);
	OptionsGroup.SetSize(271, 360);
	RegisterControl(&OptionsGroup);
	RegisterControl(&Active);

	Active.SetFileId("active");
	OptionsGroup.PlaceLabledControl(0, "Enable", this, &Active);

	BoxCol.SetFileId(XorStr("player_espbox_color"));
	OptionsGroup.PlaceLabledControl(0, XorStr(""), this, &BoxCol);;

	OptionsBox.SetFileId("opt_box");
	OptionsGroup.PlaceLabledControl(0, "Box", this, &OptionsBox);

	NameCol.SetFileId(XorStr("player_espname_color"));
	OptionsGroup.PlaceLabledControl(0, XorStr(""), this, &NameCol);

	OptionsName.SetFileId("opt_name");
	OptionsGroup.PlaceLabledControl(0, "Name", this, &OptionsName);

	OptionsHealth.SetFileId("opt_hp");
	OptionsGroup.PlaceLabledControl(0, "Health", this, &OptionsHealth);

	OptionsInfo.SetFileId("opt_info");
	OptionsGroup.PlaceLabledControl(0, "Flags", this, &OptionsInfo);

	Skeleton.SetFileId(XorStr("player_skeleton_color"));
	OptionsGroup.PlaceLabledControl(0, XorStr(""), this, &Skeleton);

	OptionsSkeleton.SetFileId("opt_bone");
	OptionsGroup.PlaceLabledControl(0, "Skeleton", this, &OptionsSkeleton);

	misc_lagcompBones.SetFileId(XorStr("misc_lagcompBones"));
	misc_lagcompBones.SetColor(250, 250, 250, 255);
	OptionsGroup.PlaceLabledControl(0, XorStr(""), this, &misc_lagcompBones);

	BackTrackBones2.SetFileId(XorStr("spookybonesOwOomg"));
	OptionsGroup.PlaceLabledControl(0, XorStr("Skeleton history"), this, &BackTrackBones2);

	WeaponTextColor.SetFileId(XorStr("player_ammo_coasdlor"));
	WeaponTextColor.SetColor(255, 255, 255, 255);
	OptionsGroup.PlaceLabledControl(0, XorStr(""), this, &WeaponTextColor);

	Weapons.SetFileId("kysquecest");
	OptionsGroup.PlaceLabledControl(0, "Weapon", this, &Weapons);

	WeaponArray.SetFileId("xjnfglsdnflksndflks");
	WeaponArray.AddItem("Text");
	WeaponArray.AddItem("Icon");
	WeaponArray.AddItem("Both");
	OptionsGroup.PlaceLabledControl(0, "", this, &WeaponArray);

	AmmoC.SetFileId(XorStr("player_ammasdasdasdasdo_color"));
	AmmoC.SetColor(100, 160, 220, 255);
	OptionsGroup.PlaceLabledControl(0, XorStr(""), this, &AmmoC);

	Ammo.SetFileId("urmomsucksass");
	OptionsGroup.PlaceLabledControl(0, "Ammo bar", this, &Ammo);

	HitHitboxColor.SetFileId(XorStr("sdfsdfsdf"));
	HitHitboxColor.SetColor(255, 255, 255, 255);
	OptionsGroup.PlaceLabledControl(0, XorStr(""), this, &HitHitboxColor);

	DebugLagComp.SetFileId(XorStr("lagcompensationyes"));
	OptionsGroup.PlaceLabledControl(0, XorStr("Draw shot hitboxes"), this, &DebugLagComp);

	Offscreen.SetFileId(XorStr("player_offscreen_color"));
	Offscreen.SetColor(255, 255, 255, 255);
	OptionsGroup.PlaceLabledControl(0, XorStr(""), this, &Offscreen);

	OffscreenESP.SetFileId("otr_offscreenESP");
	OptionsGroup.PlaceLabledControl(0, "Out of view arrows", this, &OffscreenESP);

	/*team_glow.SetFileId("opt_team_glow");
	team_glow.SetValue(0.f);
	team_glow.SetBoundaries(0.f, 100.f);
	team_glow.extension = XorStr("%%");
	OptionsGroup.PlaceLabledControl(0, "Team Glow", this, &team_glow);

	Glowz_lcl.SetFileId("opt_glowz_lcl");
	Glowz_lcl.SetValue(0.f);
	Glowz_lcl.SetBoundaries(0.f, 100.f);
	Glowz_lcl.extension = XorStr("%%");
	OptionsGroup.PlaceLabledControl(0, "Local Glow", this, &Glowz_lcl);*/

	HitmarkerColor.SetFileId(XorStr("player_adfghdfghmmo_color"));
	HitmarkerColor.SetColor(255, 255, 255, 255);
	OptionsGroup.PlaceLabledControl(0, XorStr(""), this, &HitmarkerColor);

	OtherHitmarker.SetFileId("otr_hitmarker");
	OptionsGroup.PlaceLabledControl(0, "Hit marker", this, &OtherHitmarker);

	hitmarker_sound.SetFileId("hitmarker_sound");
	OptionsGroup.PlaceLabledControl(0, "Hit marker sound", this, &hitmarker_sound);


	//	show_hostage.SetFileId("ftr_hostage");
	//	OptionsGroup.PlaceLabledControl(2, "Hostage", this, &show_hostage);

	//	asus_type.SetFileId("asus_wall_type");
	//	asus_type.AddItem("Props Only");
	//	asus_type.AddItem("Walls and Props");
	//	OptionsGroup.PlaceLabledControl(2, "Asus Type", this, &asus_type);


	//----------------------------------------------//


	/*	SpreadCrossSize.SetFileId("otr_spreadcross_size");
	SpreadCrossSize.SetBoundaries(1.f, 100.f); //we should take smth like 650 as max so i guess *6.5?
	SpreadCrossSize.extension = XorStr("%%");
	SpreadCrossSize.SetValue(45.f);
	OptionsGroup.PlaceLabledControl(3, "Size", this, &SpreadCrossSize); */

	//	DamageIndicator.SetFileId("otr_btracers");
	//	OptionsGroup.PlaceLabledControl(3, "Damage Indicator", this, &DamageIndicator);


	LBYIndicator.SetFileId("otr_LBYIndicator");
	OptionsGroup.PlaceLabledControl(4, "Fake indicator", this, &LBYIndicator);

	LCIndicator.SetFileId("otr_LCIndicator");
	OptionsGroup.PlaceLabledControl(4, "LC indicator", this, &LCIndicator);

	//	killfeed.SetFileId("otr_killfeed");
	//	OptionsGroup.PlaceLabledControl(4, "Preserve Killfeed", this, &killfeed); // requested by: https://steamcommunity.com/id/B1GN1GNN

		//	CompRank.SetFileId("otr_reveal__rank");
		//	OptionsGroup.PlaceLabledControl(4, "Rank Reveal", this, &CompRank);

		/*	BulletTrace.SetFileId("otr_bullet_tracers_local");
		OptionsGroup.PlaceLabledControl(4, "Local Bullet Tracers", this, &BulletTrace);

		BulletTrace_enemy.SetFileId("otr_bullet_tracers_enemy");
		OptionsGroup.PlaceLabledControl(4, "Enemy Bullet Tracers", this, &BulletTrace_enemy); */
		//----------------------------------------------//

	ChamsGroup.SetText("Colored models");
	ChamsGroup.SetPosition(25, 410); // 225, 30
	ChamsGroup.SetSize(271, 170);
	ChamsGroup.AddTab(CGroupTab("Enemy", 1));
	//	ChamsGroup.AddTab(CGroupTab("Team", 2));
//	ChamsGroup.AddTab(CGroupTab("Local", 3));
	RegisterControl(&ChamsGroup);

	visible_chams_type.SetFileId("otr_visiblechams_type");
	visible_chams_type.AddItem("Normal");
	visible_chams_type.AddItem("Flat"); // like my ex :)
	visible_chams_type.AddItem("Pulse");
	ChamsGroup.PlaceLabledControl(1, "Colored models material", this, &visible_chams_type);

	ChamsEnemyVis.SetFileId(XorStr("player_chamsEVIS_color"));
	ChamsEnemyVis.SetColor(170, 220, 80, 255);
	ChamsGroup.PlaceLabledControl(1, XorStr(""), this, &ChamsEnemyVis);

	ChamsEnemyvisible.SetFileId("chams_en_vis");
	ChamsGroup.PlaceLabledControl(1, "Player", this, &ChamsEnemyvisible);

	ChamsEnemyNotVis.SetFileId(XorStr("player_chamsENVIS_color"));
	ChamsEnemyNotVis.SetColor(100, 160, 220, 255);
	ChamsGroup.PlaceLabledControl(1, XorStr(""), this, &ChamsEnemyNotVis);

	ChamsEnemyinVisible.SetFileId("chams_en_novis");
	ChamsGroup.PlaceLabledControl(1, "Player behind wall", this, &ChamsEnemyinVisible); // *1

	GlowEnemy.SetFileId(XorStr("player_glowenemy_color"));
	GlowEnemy.SetColor(250, 20, 255, 255);
	ChamsGroup.PlaceLabledControl(1, XorStr(""), this, &GlowEnemy);

	EnemyGlowCheck.SetFileId("enemy_glow");
	ChamsGroup.PlaceLabledControl(1, "Glow", this, &EnemyGlowCheck);

	GlowZ.SetFileId("opt_glowz");
	GlowZ.SetValue(0.f);
	GlowZ.SetBoundaries(0.f, 100.f);
	GlowZ.extension = XorStr("%%");
	ChamsGroup.PlaceLabledControl(1, "", this, &GlowZ);

	//ChamsTeamVis.SetFileId("ChamsTeamVis");
	//ChamsGroup.PlaceLabledControl(2, "Team", this, &ChamsTeamVis);

	//ChamsLocal.SetFileId("chams_local");
	//ChamsGroup.PlaceLabledControl(3, "Local Chams", this, &ChamsLocal);

#pragma region I am literally hitler, ruined is 20% gay, DK is a jew and yall can suck it

	worldgroup.SetText("Other ESP");
	worldgroup.SetPosition(322, 25);  // 225, 285
	worldgroup.SetSize(271, 230);
	RegisterControl(&worldgroup);

	// radar

	DroppedWeaponNameCLR.SetFileId("andlandfkljsdbfkjsdbfkljlsd");
	DroppedWeaponNameCLR.SetColor(255, 255, 255, 255);
	worldgroup.PlaceLabledControl(0, "", this, &DroppedWeaponNameCLR);

	FiltersWeapons.SetFileId("ftr_weaps");
	worldgroup.PlaceLabledControl(0, "Dropped weapon", this, &FiltersWeapons);

	DroppedWeaponsType.SetFileId("ftr_ansdas");
	DroppedWeaponsType.AddItem("Text");
	DroppedWeaponsType.AddItem("Icon");
	DroppedWeaponsType.AddItem("Both");
	worldgroup.PlaceLabledControl(0, "", this, &DroppedWeaponsType);

	DroppedWeaponAmmoCLR.SetFileId("andlandsgffkljsdbfkjsdbfkljlsd");
	DroppedWeaponAmmoCLR.SetColor(100, 160, 220, 255);
	worldgroup.PlaceLabledControl(0, "", this, &DroppedWeaponAmmoCLR);

	DroppedWeaponAmmo.SetFileId("ftr_weapsa");
	worldgroup.PlaceLabledControl(0, "Dropped weapon ammo", this, &DroppedWeaponAmmo);

	ThrownNadesColor.SetFileId("hjkhjkhjkhjkhjkhjkhdrfgt");
	ThrownNadesColor.SetColor(255, 255, 255, 255);
	worldgroup.PlaceLabledControl(0, "", this, &ThrownNadesColor);

	FiltersNades.SetFileId("ftr_nades");
	worldgroup.PlaceLabledControl(0, "Grenades", this, &FiltersNades);

	spreadcrosscol.SetFileId(XorStr("weapon_spreadcross_col"));
	spreadcrosscol.SetColor(0, 0, 0, 10);
	worldgroup.PlaceLabledControl(0, XorStr(""), this, &spreadcrosscol);

	SpreadCrosshair.SetFileId("v_spreadcrosshair");
	worldgroup.PlaceLabledControl(0, "Inaccuracy overlay", this, &SpreadCrosshair);

	//recoil overlay

	CrosshairColor.SetFileId(XorStr("weapon_spreaaadcross_col"));
	CrosshairColor.SetColor(100, 160, 220, 255);
	worldgroup.PlaceLabledControl(0, XorStr(""), this, &CrosshairColor);

	AutowallCrosshair.SetFileId("otr_autowallcross");
	worldgroup.PlaceLabledControl(0, "Crosshair", this, &AutowallCrosshair);

	FiltersC4.SetFileId("ftr_c4");
	worldgroup.PlaceLabledControl(0, "Bomb", this, &FiltersC4);

	//nade trajectory

	SpecList.SetFileId("otr_spectator");
	worldgroup.PlaceLabledControl(0, "Spectator list", this, &SpecList);


	OtherViewmodelFOV.SetFileId("otr_viewfov");
	OtherViewmodelFOV.SetBoundaries(0.f, 180.f);
	OtherViewmodelFOV.SetValue(60.f);
	worldgroup.PlaceLabledControl(0, "Override viewmodel FOV", this, &OtherViewmodelFOV);

	OtherFOV.SetFileId("otr_fov");
	OtherFOV.SetBoundaries(0.f, 180.f);
	OtherFOV.SetValue(0.f);
	worldgroup.PlaceLabledControl(0, "Override viewangle FOV", this, &OtherFOV);


	Effects.SetText("Effects");
	Effects.SetPosition(322, 280);
	Effects.SetSize(271, 300);
	RegisterControl(&Effects);

	OtherNoFlash.SetFileId("otr_noflash");
	Effects.PlaceLabledControl(0, "Remove flashbang effects", this, &OtherNoFlash);

	nosmoke.SetFileId("otr_nosmoke");
	Effects.PlaceLabledControl(0, "Remove smoke grenades", this, &nosmoke);

	// no fog

	OtherNoVisualRecoil.SetFileId("otr_visrecoil");
	Effects.PlaceLabledControl(0, "Remove visual recoil", this, &OtherNoVisualRecoil);

	OtherNoScope.SetFileId("otr_noscope");
	Effects.PlaceLabledControl(0, "Remove scope overlay", this, &OtherNoScope);

	RemoveZoom.SetFileId("otr_remv_zoom");
	Effects.PlaceLabledControl(0, "Remove zoom effects", this, &RemoveZoom);

	SleeveChams.SetFileId("remove_Sleeve");
	Effects.PlaceLabledControl(0, "Remove sleeve", this, &SleeveChams);

	optimize.SetFileId("ftr_optimize");
	Effects.PlaceLabledControl(0, "Disable post processing", this, &optimize);

	ThirdPersonKeyBind.SetFileId("aa_thirpbind");
	Effects.PlaceLabledControl(0, "", this, &ThirdPersonKeyBind);

	OtherThirdperson.SetFileId("aa_1thirpbind");
	Effects.PlaceLabledControl(0, "Force third person", this, &OtherThirdperson);

	customskies.SetFileId("otr_skycustom");
	customskies.AddItem("Default");
	customskies.AddItem("Night");
	customskies.AddItem("NoSky");
	customskies.AddItem("Cloudy");
	Effects.PlaceLabledControl(0, "Custom skybox", this, &customskies);

	colmod.SetFileId("night_amm");
	colmod.SetBoundaries(000.000f, 100.00f);
	colmod.extension = XorStr("%%");
	colmod.SetValue(020.0f);
	Effects.PlaceLabledControl(0, "Brightness adjustment", this, &colmod);

	asusamount.SetFileId("otr_asusprops");
	asusamount.SetBoundaries(1.f, 100.f);
	asusamount.extension = XorStr("%%");
	asusamount.SetValue(95.f);
	Effects.PlaceLabledControl(0, "Transparent props", this, &asusamount);

	UpdateWorld.SetText(XorStr("Update world"));
	UpdateWorld.SetCallback(&update_world);
	Effects.PlaceLabledControl(0, "", this, &UpdateWorld);


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
	SetTitle("D");
#pragma region Other
	OtherGroup.SetText("Miscellaneous");
	OtherGroup.SetPosition(25, 25);
	OtherGroup.SetSize(271, 555);
	RegisterControl(&OtherGroup);

	OtherAutoJump.SetFileId("otr_autojump");
	OtherGroup.PlaceLabledControl(0, "Bunny hop", this, &OtherAutoJump);

	OtherAutoStrafe.SetFileId("otr_strafe");
	OtherGroup.PlaceLabledControl(0, "Air strafe", this, &OtherAutoStrafe);

	airstrafe.SetFileId("l_rairstrafe");
	airstrafe.AddItem("View angles");
	airstrafe.AddItem("Movement keys");
	OtherGroup.PlaceLabledControl(1, "Air strafe direction", this, &airstrafe);

	Airstrafesmoothing.SetFileId("l_Airstrafesmoothing");
	Airstrafesmoothing.SetBoundaries(0.f, 100.f);
	Airstrafesmoothing.extension = XorStr("%%");
	Airstrafesmoothing.SetValue(5.00f);
	OtherGroup.PlaceLabledControl(2, "Air strafe smoothing", this, &Airstrafesmoothing);

	ZHop.SetFileId("settings_Z-Hop");
	OtherGroup.PlaceLabledControl(0, "Z-Hop", this, &ZHop);

	Prespeed.SetFileId("settings_Pre-speed");
	OtherGroup.PlaceLabledControl(0, "Pre-speed", this, &Prespeed);

	airduck_type.SetFileId("l_rairduck_type");
	airduck_type.AddItem("Off");
	airduck_type.AddItem("Standard");
	OtherGroup.PlaceLabledControl(1, "Air duck", this, &airduck_type);

	Knifebot.SetFileId("settings_Knifebot");
	OtherGroup.PlaceLabledControl(0, "Knifebot", this, &Knifebot);

	Zeusbot.SetFileId("settings_Zeusbot");
	OtherGroup.PlaceLabledControl(0, "Zeusbot", this, &Zeusbot);

	Blockbot.SetFileId("settings_Blockbot");
	OtherGroup.PlaceLabledControl(0, "Blockbot", this, &Blockbot);

	Automaticweapons.SetFileId("settings_Automaticweapons");
	OtherGroup.PlaceLabledControl(0, "Automatic weapons", this, &Automaticweapons);

	Edgejump.SetFileId("settings_Jumpatedge");
	OtherGroup.PlaceLabledControl(0, "Jump at edge", this, &Edgejump);

	Rank.SetFileId("settings_Rank");
	OtherGroup.PlaceLabledControl(0, "Reveal competitive ranks", this, &Rank);

	Autommm.SetFileId("settings_Autommm");
	OtherGroup.PlaceLabledControl(0, "Auto-accept matchmaking", this, &Autommm);

	ClanTag.SetFileId("otr_clantg");
	OtherGroup.PlaceLabledControl(0, XorStr("Clan tag spammer"), this, &ClanTag);

	Logdmgpurchases.SetFileId("otr_Logdmgpurchases");
	OtherGroup.PlaceLabledControl(4, "Log weapon purchases", this, &Logdmgpurchases);

	logs.SetFileId("otr_skeetpaste");
	OtherGroup.PlaceLabledControl(4, "Log damage dealt", this, &logs);

	Automaticgrenade.SetFileId("otr_skeetpaste");
	OtherGroup.PlaceLabledControl(4, "Automatic grenade release", this, &Automaticgrenade);
	
	Pingspike.SetFileId("otr_Pingspike");
	OtherGroup.PlaceLabledControl(4, "Ping spike", this, &Pingspike);

	Fastwalk.SetFileId("otr_Fastwalk");
	OtherGroup.PlaceLabledControl(4, "Fast walk", this, &Fastwalk);

	stealplayername.SetText(XorStr("Steal player name"));
	stealplayername.SetCallback(&add_config);
	OtherGroup.PlaceLabledControl(0, "", this, &stealplayername);

	dumpmmwins.SetText(XorStr("Dump MM wins"));
	dumpmmwins.SetCallback(&add_config);
	OtherGroup.PlaceLabledControl(0, "", this, &dumpmmwins);

	//buybot_primary.SetFileId("buybot_primary"); //only use when ingame
	//buybot_primary.AddItem("Off");
	//buybot_primary.AddItem("Auto-Sniper");
	//buybot_primary.AddItem("Scout");
	//buybot_primary.AddItem("Awp");
	//OtherGroup.PlaceLabledControl(2, "Primary Weapon", this, &buybot_primary);

	//buybot_secondary.SetFileId("buybot_secondary");
	//buybot_secondary.AddItem("Off");
	//buybot_secondary.AddItem("Dual Berretas");
	//buybot_secondary.AddItem("Revolver / Desert Eagle");
	//OtherGroup.PlaceLabledControl(2, "Secondary Weapon", this, &buybot_secondary);

	//buybot_otr.SetFileId("buybot_other");
	//buybot_otr.items.push_back(dropdownboxitem(false, XorStr("kevlar")));
	//buybot_otr.items.push_back(dropdownboxitem(false, XorStr("he-grenade")));
	//buybot_otr.items.push_back(dropdownboxitem(false, XorStr("flashbang")));
	//buybot_otr.items.push_back(dropdownboxitem(false, XorStr("smoke grenade")));
	////buybot_otr.items.push_back(dropdownboxitem(false, XorStr("molotov")));
	//buybot_otr.items.push_back(dropdownboxitem(false, XorStr("zeus")));
	//buybot_otr.items.push_back(dropdownboxitem(false, XorStr("defuse-kit")));
	//OtherGroup.PlaceLabledControl(2, "Others", this, &buybot_otr);


	SettingsGroup.SetText("Settings");
	SettingsGroup.SetPosition(322, 25);
	SettingsGroup.SetSize(271, 130);
	RegisterControl(&SettingsGroup);

	MenuKey.SetFileId("settings_menukey");
	MenuKey.SetKey(VK_INSERT);
	SettingsGroup.PlaceLabledControl(0, "", this, &MenuKey);
	SettingsGroup.PlaceLabledControl(0, "Menu key", this, &MenuKeyText);

	MenuColor.SetFileId("menu_color");
	MenuColor.SetColor(100, 160, 220, 255);
	SettingsGroup.PlaceLabledControl(0, "", this, &MenuColor);
	SettingsGroup.PlaceLabledControl(0, "Menu color", this, &MenuColorText);

	AntiUntrusted.SetFileId("settings_antiuntrusted");
	AntiUntrusted.SetState(1);
	SettingsGroup.PlaceLabledControl(0, "Anti-untrusted", this, &AntiUntrusted);

	HideFromOBS.SetFileId("settings_hidefromobs");
	SettingsGroup.PlaceLabledControl(0, "Hide from OBS", this, &HideFromOBS);

	LowFPSWarning.SetFileId("settings_lowfpswarning");
	SettingsGroup.PlaceLabledControl(0, "Low FPS warning", this, &LowFPSWarning);

	LockMenuLayout.SetFileId("settings_lockmenulayout");
	SettingsGroup.PlaceLabledControl(0, "Lock menu layout", this, &LockMenuLayout);

	ConfigGroup.SetText("Other");
	ConfigGroup.SetPosition(322, 165);
	ConfigGroup.SetSize(271, 410);
	RegisterControl(&ConfigGroup);

	ConfigListBox.SetHeightInItems(7);
	list_configs();
	ConfigGroup.PlaceLabledControl(0, XorStr(""), this, &ConfigListBox);

	ConfigGroup.PlaceLabledControl(0, "", this, &NewConfigName);

	AddConfig.SetText(XorStr("Add"));
	AddConfig.SetCallback(&add_config);
	ConfigGroup.PlaceLabledControl(0, "", this, &AddConfig);

	RemoveConfig.SetText(XorStr("Remove"));
	RemoveConfig.SetCallback(&remove_config);
	ConfigGroup.PlaceLabledControl(0, "", this, &RemoveConfig);

	LoadConfig.SetText(XorStr("Load"));
	LoadConfig.SetCallback(&load_callback);
	ConfigGroup.PlaceLabledControl(0, "", this, &LoadConfig);

	SaveConfig.SetText(XorStr("Save"));
	SaveConfig.SetCallback(&save_callback);
	ConfigGroup.PlaceLabledControl(0, "", this, &SaveConfig);

	Restmenu.SetText(XorStr("Reset menu layout"));
	Restmenu.SetCallback(&add_config);
	ConfigGroup.PlaceLabledControl(0, "", this, &Restmenu);

	luaapi.SetText(XorStr("Lua script manager"));
	luaapi.SetCallback(&add_config);
	ConfigGroup.PlaceLabledControl(0, "", this, &luaapi);

	Unload.SetText(XorStr("Unload"));
	Unload.SetCallback(&add_config);
	ConfigGroup.PlaceLabledControl(0, "", this, &Unload);
}

void CSkinTab::Setup()
{
	SetTitle("E");
	//knifegroup.SetText("Knife options");
	//knifegroup.SetPosition(25, 25);
	//knifegroup.SetSize(271, 170);
	//RegisterControl(&knifegroup);

	//Overrideknife.SetFileId("l_rOverrideknife");
	//Overrideknife.AddItem("Bayonet");
	//Overrideknife.AddItem("High");
	//knifegroup.PlaceLabledControl(1, "Force yaw", this, &Overrideknife);
	
	//t_sniperSCAR_skin_id.SetFileId("t_scar20_skin");
	//t_sniperSCAR_skin_id.AddItem("Default");
	//t_sniperSCAR_skin_id.AddItem("Crimson Web");

	//knifegroup.SetText("Glove options");
	//knifegroup.SetPosition(322, 25);
	//knifegroup.SetSize(271, 300);
	//RegisterControl(&knifegroup);
	//t_sniperSCAR_skin_id.AddItem("Splash Jam");
	//t_sniperSCAR_skin_id.AddItem("Emerald");

	//knifegroup.SetText("Weapon skin");
	//knifegroup.SetPosition(322, 25);
	//knifegroup.SetSize(271, 555);
	//RegisterControl(&knifegroup);
	//t_sniperSCAR_skin_id.AddItem("Cardiac");
	//t_sniperSCAR_skin_id.AddItem("Cyrex");
	////t_sniperSCAR_skin_id.AddItem("Whiteout");
	//t_sniperSCAR_skin_id.AddItem("The Fuschia Is Now");
	//knifegroup.PlaceLabledControl(0, XorStr("Scar Skin"), this, &t_knife_index);
	//
	//t_sniperSCAR_wear.SetFileId("t_sniperSCAR_wear");
	//t_sniperSCAR_wear.SetBoundaries(1, 100);
	////t_sniperSCAR_wear.SetValue(1);
	//t_sniperSCAR_wear.extension = XorStr("%%");
	//snipergroup.PlaceLabledControl(1, XorStr("Wear"), this, &t_sniperSCAR_wear);

	// --



	// --

	
}

void CColorTab::Setup()
{
	SetTitle("F");
#pragma region Visual Colors
	ColorsGroup.SetText("Players");
	ColorsGroup.SetPosition(25, 25);
	//	ColorsGroup.AddTab(CGroupTab("Misc", 6));
	ColorsGroup.SetSize(271, 555);
	RegisterControl(&ColorsGroup);

	ConfigGroup.SetText("Adjustments");
	ConfigGroup.SetPosition(322, 25);
	ConfigGroup.SetSize(271, 555);
	RegisterControl(&ConfigGroup);

	whitelist.SetFileId("otr_whitelist");
	ConfigGroup.PlaceLabledControl(0, "Add to whitelist", this, &whitelist);

	AllowSharedEsp.SetFileId("otr_AllowSharedEsp");
	ConfigGroup.PlaceLabledControl(0, "Allow shared ESP updates", this, &AllowSharedEsp);

	DisableVisuals.SetFileId("otr_DisableVisuals");
	ConfigGroup.PlaceLabledControl(0, "Disable visuals", this, &DisableVisuals);

	PlayerlistHighPriority.SetFileId("otr_PlayerlistHighPriority");
	ConfigGroup.PlaceLabledControl(0, "high priority", this, &PlayerlistHighPriority);

	Forceyaw.SetFileId("l_rForceyaw");
	Forceyaw.AddItem("-");
	ConfigGroup.PlaceLabledControl(1, "Force yaw", this, &Forceyaw);

	Forceyawvalue.SetFileId("l_trigRCS");
	Forceyawvalue.SetBoundaries(0.f, 100.f);
	Forceyawvalue.extension = XorStr("%%");
	Forceyawvalue.SetValue(5.00f);
	ConfigGroup.PlaceLabledControl(2, "Force yaw value", this, &Forceyawvalue);

	Forcepitch.SetFileId("otr_Forcepitch");
	ConfigGroup.PlaceLabledControl(0, "Disable visuals", this, &Forcepitch);

	Forcepitchvalue.SetFileId("l_trigRCS");
	Forcepitchvalue.SetBoundaries(0.f, 100.f);
	Forcepitchvalue.extension = XorStr("%%");
	Forcepitchvalue.SetValue(5.00f);
	ConfigGroup.PlaceLabledControl(2, "Force pitch value", this, &Forcepitchvalue);

	Overrideantiaim.SetFileId("l_rForceyaw");
	Overrideantiaim.AddItem("-");
	ConfigGroup.PlaceLabledControl(1, "Override anti-aim correction", this, &Overrideantiaim);

	Edgecorrection.SetFileId("otr_Edgecorrection");
	ConfigGroup.PlaceLabledControl(0, "Edge correction", this, &Edgecorrection);

	Overridepreferbodyaim.SetFileId("l_rOverridepreferbodyaim");
	Overridepreferbodyaim.AddItem("-");
	ConfigGroup.PlaceLabledControl(1, "Override prefer body aim", this, &Overridepreferbodyaim);

	Overrideaccuracyboost.SetFileId("l_rOverridepreferbodyaim");
	Overrideaccuracyboost.AddItem("-");
	ConfigGroup.PlaceLabledControl(1, "Override accuracy boost", this, &Overrideaccuracyboost);

	Applytoall.SetText(XorStr("Apply to all"));
	Applytoall.SetCallback(&add_config);
	ConfigGroup.PlaceLabledControl(0, "", this, &Applytoall);
	
}

void options::SetupMenu()
{
	menu.Setup();
	GUI.RegisterWindow(&menu);
	GUI.BindWindow(options::menu.MiscTab.MenuKey.GetKey(), &menu);
}
void options::DoUIFrame()
{
	if (menu.visuals_tab.FiltersAll.GetState())
	{
		menu.visuals_tab.FiltersC4.SetState(true);
		menu.visuals_tab.show_players.SetState(true);
		menu.visuals_tab.FiltersWeapons.SetState(true);
		menu.visuals_tab.FiltersNades.SetState(true);
	}
	GUI.Update();
	GUI.Draw();
}

























































































































































































