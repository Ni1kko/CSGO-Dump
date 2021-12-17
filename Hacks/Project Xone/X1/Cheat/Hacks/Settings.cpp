#include "Settings.h"
#include "Setup.h"
#include "../GUI/Gui.h"

bool isDirectoryExists(const char *filename)
{
	DWORD dwFileAttributes = GetFileAttributes(filename);
	if (dwFileAttributes == 0xFFFFFFFF)
		return false;
	return dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
}

void CGSettings::SaveEx(nlohmann::json &j)
{
	if (GP_Misc)
		GP_Misc->Config.Save(j);

	if (GP_Radar)
		GP_Radar->Config.Save(j);

	if (GP_GHelper)
		GP_GHelper->Config.Save(j);

	if (GP_Esp)
	{
		GP_Esp->Config.Save(j);
		GP_Esp->SaveVisuals(j);
	}

	if (GP_LegitAim)
	{
		GP_LegitAim->Config.Save(j);
		GP_LegitAim->SaveWeapons(j);
	}
	if (GP_Skins)
	{
		GP_Skins->Config.Save(j);
		GP_Skins->SaveSkins(j);
	}
	

	if (GP_Inventory)
	{
		GP_Inventory->Config.Save(j);
		GP_Inventory->SaveInventory(j);
	}

	//MainSettings().Config.Save(j);

#define MCC XorStr("MainCheatConfig")
#define SSS(a,b) j[MCC][XorStr(b)] = a;

	if (GP_Enable3DMenu)
	{
		SSS(GP_Enable3DMenu->Button, "Bind3DMenu_Button");
		SSS(GP_Enable3DMenu->Enable, "Bind3DMenu_Enable");
	}

	SSS(MainSettings().Russian, "RusLanguage");
	//RV(MenuAnimation, "MenuAnimation");
	SSS(MainSettings().Watermark, "Watermark");
	//RV(BackupSettings, "BackupSettings");
	SSS(MainSettings().MenuButton, "MenuButton");
	//RV(MenuAlpha, "MenuAlpha");
	//RV(AutoLoadEnable, "AutoLoadEnable");

	CConfig Config = CConfig("MainCheatConfig");

	RV(MainSettings().BackgroundColor, "BackgroundColor");
	RV(MainSettings().TitleColor, "TitleColor");
	RV(MainSettings().TextColor, "TextColor");
	RV(MainSettings().FrameColor, "FrameColor");
	RV(MainSettings().ButtonColor, "ButtonColor");
	RV(MainSettings().DisableButtonColor, "DisableButtonColor");
	RV(MainSettings().LineColor, "LineColor");
	Config.Save(j);
}


	

void CGSettings::LoadEx(nlohmann::json &j)
{
	if (GP_Misc)
		GP_Misc->Config.Load(j);

	if (GP_Radar)
		GP_Radar->Config.Load(j);
	
	if (GP_GHelper)
		GP_GHelper->Config.Load(j);

	if (GP_Esp)
	{
		GP_Esp->Config.Load(j);
		GP_Esp->LoadVisuals(j);
	}

	if (GP_LegitAim)
	{
		GP_LegitAim->Config.Load(j);
		GP_LegitAim->LoadWeapons(j);
	}
	if (GP_Skins)
	{
		GP_Skins->Config.Load(j);
		GP_Skins->LoadSkins(j);
	}

	if (GP_Inventory)
	{
		GP_Inventory->Config.Load(j);
		GP_Inventory->LoadInventory(j);
	}

	//MainSettings().Config.Load(j);

	
	

#define MCC XorStr("MainCheatConfig")
#define SSS(a,b) if(!j[MCC][b].is_null()){ a = j[MCC][XorStr(b)];}

	if (!j[MCC].is_null())
	{
		if (GP_Enable3DMenu)
		{
			SSS(GP_Enable3DMenu->Button, "Bind3DMenu_Button");
			SSS(GP_Enable3DMenu->Enable, "Bind3DMenu_Enable");
		}
	}
	SSS(MainSettings().Russian, "RusLanguage");
	//RV(MenuAnimation, "MenuAnimation");
	SSS(MainSettings().Watermark, "Watermark");
	//RV(BackupSettings, "BackupSettings");
	SSS(MainSettings().MenuButton, "MenuButton");
	//RV(MenuAlpha, "MenuAlpha");
	//RV(AutoLoadEnable, "AutoLoadEnable");

	CConfig Config = CConfig("MainCheatConfig");

	RV(MainSettings().BackgroundColor, "BackgroundColor");
	RV(MainSettings().TitleColor, "TitleColor");
	RV(MainSettings().TextColor, "TextColor");
	RV(MainSettings().FrameColor, "FrameColor");
	RV(MainSettings().ButtonColor, "ButtonColor");
	RV(MainSettings().DisableButtonColor, "DisableButtonColor");
	RV(MainSettings().LineColor, "LineColor");
	Config.Load(j);
}
string StrToU8(const char *str)
{
	string res;
	int result_u, result_c;

	if (!str)
		return 0;

	result_u = FastCall::G().t_MultiByteToWideChar(1251, 0, str, -1, 0, 0);

	if (!result_u)
		return 0;

	wchar_t *ures = new wchar_t[result_u];

	if (!FastCall::G().t_MultiByteToWideChar(1251, 0, str, -1, ures, result_u))
	{
		delete[] ures;
		return 0;
	}

	result_c = FastCall::G().t_WideCharToMultiByte(CP_UTF8, 0, ures, -1, 0, 0, 0, 0);

	if (!result_c)
	{
		delete[] ures;
		return 0;
	}

	char *cres = new char[result_c];

	if (!FastCall::G().t_WideCharToMultiByte(CP_UTF8, 0, ures, -1, cres, result_c, 0, 0))
	{
		delete[] cres;
		return 0;
	}

	delete[] ures;
	res.append(cres);
	delete[] cres;

	return res;
}


bool CGSettings::Save(string name, bool _cloud)
{

	/*if (MainCheatConfig.BackupSettings && _cloud)
	{
		SYSTEMTIME Time;
		FastCall::G().t_GetLocalTime(&Time);
		string old_cfg = config_name;
		string old_name = name;
		if (name.size() > 5)
			name.resize(name.size() - 5);

		string cfg_name = name + "_copy_" + to_string(Time.wDay) + "_" + to_string(Time.wHour) + "_" + to_string(Time.wMinute);

		config_name = SystemDisk + XorStr("XONE\\Settings\\Backup");
		if (!isDirectoryExists(config_name.c_str()))
			CreateDirectory(config_name.c_str(), NULL);

		config_name += "\\" + cfg_name + ".json";

		Save("", false);

		name = old_name;
		config_name = old_cfg;
	}*/

	nlohmann::json JFile;

	SaveEx(JFile);

	if (_cloud)
		CCloudSets::CCloudGet().Save(MainSettings().UserName + "_" + MainSettings().id, name, JFile.dump());

	std::ofstream o(config_name);


	if (!o)
	{
		o.clear();
		JFile.clear();
		return false;
	}

	o << std::setw(4) << JFile << std::endl;

	JFile.clear();
	return true;
}

bool CGSettings::Load()
{
	string JCont = readFile(config_name);

	if (JCont == string(XorStr("Read Error")))
		return false;

	nlohmann::json JFile = nlohmann::json::parse(JCont);

	LoadEx(JFile);

	JFile.clear();
	GP_Skins->UpdateSkins();
	/*GP_Skins->UpdateSkins();

	GP_Inventory->SendClientHello();
	GP_Inventory->SendMMHello();*/

	return true;
}

bool CGSettings::CloudLoad(string hash, string &name)
{
	string JCont = CCloudSets::CCloudGet().Load(hash, name);

	if (JCont == string(XorStr("CFG: NOT FOUND")) || JCont == string(XorStr("EMPTY")) || JCont.empty())
		return false;

	//string JCont_compress = JCont/*base64_decode__(JCont)*/;

	//const char * JCont_compress_ptr = JCont_compress.data();
	//size_t JCont_compress_size = JCont_compress.size();
	//JCont = gzip::decompress(JCont_compress_ptr, JCont_compress_size);

	nlohmann::json JFile = nlohmann::json::parse(JCont);

	LoadEx(JFile);

	JFile.clear();

	GP_Skins->UpdateSkins();

	//GP_Inventory->SendClientHello();
	//GP_Inventory->SendMMHello();

	return true;
}

void CGSettings::UpdateColors()
{
	if (GP_LegitAim)
		GP_LegitAim->Config.UpdateColors();

	if (GP_Misc)
		GP_Misc->Config.UpdateColors();

	if (GP_Esp)
		GP_Esp->Config.UpdateColors();

	if (GP_Radar)
		GP_Radar->Config.UpdateColors();
}

void CGSettings::SetName(const string _name)
{
	config_name = _name;
}



void CGSettings::UpdateList()
{
	AllSettings.clear();
	CGlobal::RefreshConfigs();

	vector<CCloudSets::cfg_info_s> CloudList = CCloudSets::CCloudGet().UpdateList(MainSettings().UserName +  "_" + MainSettings().id);

	if (CGlobal::ConfigList.size() > CloudList.size())
	{
		for (auto &v : CGlobal::ConfigList)
		{
			CfgInfo_s cfg_entry;
			cfg_entry.name = v;
			cfg_entry.cloud_hash = "";
			cfg_entry.id = "";
			cfg_entry.cloud = false;
			cfg_entry.pc_cloud = false;
			AllSettings.push_back(cfg_entry);
		}
		for (auto &v_cloud : CloudList)
		{
			bool finded = false;
			for (auto &v : AllSettings)
			{
				if (v_cloud.name == v.name)
				{
					v.pc_cloud = true;
					v.cloud_hash = v_cloud.hash;
					v.id = v_cloud.id;
					finded = true;
				}
			}
			if (!finded)
			{
				CfgInfo_s cfg_entry;
				cfg_entry.name = v_cloud.name;
				cfg_entry.cloud_hash = v_cloud.hash;
				cfg_entry.id = v_cloud.id;
				cfg_entry.cloud = true;
				cfg_entry.pc_cloud = false;
				AllSettings.push_back(cfg_entry);
			}
		}
	}
	else
	{
		for (auto &v : CloudList)
		{
			CfgInfo_s cfg_entry;
			cfg_entry.name = v.name;
			cfg_entry.cloud_hash = v.hash;
			cfg_entry.id = v.id;
			cfg_entry.cloud = true;
			cfg_entry.pc_cloud = false;
			AllSettings.push_back(cfg_entry);
		}
		for (auto &v_pc : CGlobal::ConfigList)
		{
			bool finded = false;
			for (auto &v : AllSettings)
			{
				if (v.name == v_pc)
				{
					v.cloud = false;
					v.pc_cloud = true;
					finded = true;
				}
			}
			if (!finded)
			{
				CfgInfo_s cfg_entry;
				cfg_entry.name = v_pc;
				cfg_entry.cloud_hash = "";
				cfg_entry.id = "";
				cfg_entry.cloud = false;
				cfg_entry.pc_cloud = false;
				AllSettings.push_back(cfg_entry);
			}
		}
	}

	if (!AllSettings.empty())
	{
		vector<CfgInfo_s> TempSettings = AllSettings;
		vector<int> skip_idx;
		AllSettings.clear();

		for (int i(0); i < (int)TempSettings.size(); i++)
		{
			if (TempSettings[i].cloud || TempSettings[i].pc_cloud)
			{
				AllSettings.push_back(TempSettings[i]);
				skip_idx.push_back(i);
			}
		}
		for (int i(0); i < (int)TempSettings.size(); i++)
		{
			bool cont = false;
			for (int j(0); j < (int)skip_idx.size(); j++)
			{
				if (skip_idx[j] == i)
				{
					cont = true;
					break;
				}
			}
			if (cont)
				continue;
			AllSettings.push_back(TempSettings[i]);
		}
	}
}

bool CheckRus(string &check)
{
	string alph_rus = "јаЅб¬в√гƒд≈е®Є∆ж«з»и…й кЋлћмЌнќоѕп–р—с“т”у‘ф’х÷ц„чЎшўщЏъџы№ьЁэёюя€";

	for (auto &v_check : check)
		for (auto &v_rus : alph_rus)
			if (v_check == v_rus)
				return true;

	return false;
}
void FixHash(char* arr, int size) //переписать\убрать
{
	string check_arr = "1B3D5H7G90R1234567890-";
	string fixed = "";

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < check_arr.size(); j++)
		{
			if (arr[i] == check_arr[j])
			{
				fixed += arr[i];
				break;
			}
		}
	}

	for (int i = 0; i < size; i++)
		arr[i] = '\0';

	for (int i = 0; i < fixed.size(); i++)
		arr[i] = fixed[i];
}

void CGSettings::Menu()
{

	static int ConfigSelect = 0;
	static char NewConfigName[64] = { 0 };
	static char NewConfigHash[64] = { 0 };

	float long_item_w = X1Gui().GetThis()->Info.Size.x - (X1Gui().GetStyle().wndPadding.x * 2);

	if(X1Gui().ListBoxHeader(XorStr("##0"), Vec2(long_item_w, 250)))
	{
		X1Gui().Spacing();
		X1Gui().Separator();
		X1Gui().Text("Name");
		X1Gui().SameLine(200 + X1Gui().GetStyle().itmInPadding.x);
		X1Gui().Text("Status");
		X1Gui().SameLine(300 + X1Gui().GetStyle().itmInPadding.x);
		X1Gui().Text("Hash");
		X1Gui().Separator();

		for (int i = 0; i < (int)AllSettings.size(); i++)
		{
			bool selected = i == ConfigSelect;
			if (X1Gui().SelectLabel(AllSettings[i].name.c_str(), selected, Vec2(200, 19)))
				ConfigSelect = i;
			X1Gui().SameLine(200 - X1Gui().GetStyle().wndPadding.x);
			if (AllSettings[i].cloud)
				if (X1Gui().SelectLabel(("Online##" + to_string(i)).c_str(), selected, Vec2(100, 19)))
					ConfigSelect = i;

			if (AllSettings[i].pc_cloud)
				if (X1Gui().SelectLabel(("PC & Online##" + to_string(i)).c_str(), selected, Vec2(100, 19)))
					ConfigSelect = i;

			if (!AllSettings[i].pc_cloud && !AllSettings[i].cloud)
				if (X1Gui().SelectLabel(("PC##" + to_string(i)).c_str(), selected, Vec2(100, 19)))
					ConfigSelect = i;

			if (!AllSettings[i].cloud_hash.empty())
			{
				X1Gui().SameLine(300 - X1Gui().GetStyle().wndPadding.x);
				string hash_str = AllSettings[i].cloud_hash;

				if (hash_str.size() > 3)
				{
					hash_str[hash_str.size() - 1] = '*';
					hash_str[hash_str.size() - 2] = '*';
					hash_str[hash_str.size() - 3] = '*';
				}

				X1Gui().Text(hash_str.c_str());
				X1Gui().SameLine(415);
				//	X1Gui().SetCursorPos(Vec2(sp_c.x + (true/*!MainCheatConfig.RusLanguage*/ ? 100 : 87), sp_c.y));
				if (X1Gui().Button(((!MainSettings().Russian ? XorStr("Copy##") : XorStr(u8" опир.##")) + AllSettings[i].cloud_hash).c_str(), Vec2(0, 19)))
				{
					string source = AllSettings[i].cloud_hash + " - " + AllSettings[i].name;
					if (FastCall::G().t_OpenClipboard(NULL))
					{
						HGLOBAL hgBuffer;
						char* chBuffer;

						FastCall::G().t_EmptyClipboard();

						hgBuffer = FastCall::G().t_GlobalAlloc(GMEM_DDESHARE, source.length() + 1);
						chBuffer = (char*)FastCall::G().t_GlobalLock(hgBuffer);
						strcpy(chBuffer, source.c_str());
						FastCall::G().t_GlobalUnlock(hgBuffer);

						FastCall::G().t_SetClipboardData(CF_TEXT, hgBuffer);
						FastCall::G().t_CloseClipboard();
					}
				}
			}
		}
		X1Gui().VerticalSeparator(200);
		X1Gui().VerticalSeparator(300);
	}
	X1Gui().ListBoxFooter();

	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();

	X1Gui().PushItemWidth(200.f);
	X1Gui().InputText(XorStr("##Name"), NewConfigName, 28);
	X1Gui().SameLine();

	string SettignBaseDir = CGlobal::SystemDisk + XorStr("XONE\\Settings\\");
	string SettingsFormat = XorStr(".json");

	if (X1Gui().Button(!MainSettings().Russian ? XorStr("Create & Save new Settings") : XorStr(u8"—оздать и сохранить новые настройки")))
	{
		//CheckRus
		if (!CheckRus(string(NewConfigName)))
		{
			string ConfigFileName = NewConfigName;

			if (ConfigFileName.size() < 1)
			{
				ConfigFileName = XorStr("Settings");
			}

			SetName(SettignBaseDir + ConfigFileName + SettingsFormat);

			if (Save(ConfigFileName + SettingsFormat))
				Message::Get().Start(!MainSettings().Russian ? XorStr("Settings saved!") : XorStr(u8"Ќастройки сохранены!"));
			else
				Message::Get().Start(!MainSettings().Russian ? XorStr("Error save!") : XorStr(u8"ќшибка сохранени€!"));

			UpdateList();
			strcpy(NewConfigName,"");
		}
		//else
		//	Menu::Message::Get().Start(!MainCheatConfig.RusLanguage ? XorStr("Error save(RUS)!") : XorStr(u8"ќшибка сохранени€(RUS)!"));
	}
	X1Gui().Spacing();
	X1Gui().InputText(XorStr("##Hash"), NewConfigHash, 28);
	X1Gui().SameLine();
	if (X1Gui().Button(!MainSettings().Russian ? XorStr("Load Shared Settings") : XorStr(u8"«агрузить общие настройки")))
	{
		FixHash(NewConfigHash, 64);

		string _shared_name = "";
		bool _shared_finded = false;
		for (auto v : AllSettings)
		{
			if (v.cloud_hash == string(NewConfigHash))
			{
				_shared_finded = true;
				break;
			}
		}

		if (!_shared_finded && !string(NewConfigHash).empty() && CloudLoad(string(NewConfigHash), _shared_name))
		{
			_shared_name.resize(_shared_name.size() - 5);
			SetName(SettignBaseDir + _shared_name + "_sd.json");
			Save(_shared_name + "_sd.json");
			UpdateList();
			Message::Get().Start(!MainSettings().Russian ? XorStr("Settings loaded!") : XorStr(u8"Ќастройки загружены!"));
		}
		else
			Message::Get().Start(!MainSettings().Russian ? XorStr("Error load!") : XorStr(u8"ќшибка загрузки!"));
	}

	if (ConfigSelect >= 0 && ConfigSelect < (int)AllSettings.size())
	{
		if (!AllSettings[ConfigSelect].cloud && !AllSettings[ConfigSelect].pc_cloud)
		{
			X1Gui().Spacing();
			X1Gui().Separator();
			X1Gui().Spacing();
			if (X1Gui().Button(!MainSettings().Russian ? XorStr("Upload settings to the server") : XorStr(u8"«агрузить настройки на сервер"), Vec2(long_item_w, 0)))
			{
				string JCont = readFile(SettignBaseDir + AllSettings[ConfigSelect].name);

				if (JCont == string(XorStr("Read Error")))
					Message::Get().Start(!MainSettings().Russian ? XorStr("Error upload!") : XorStr(u8"ќшибка загрузки!"));
				else
				{

					nlohmann::json JFile = nlohmann::json::parse(JCont);
					CCloudSets::CCloudGet().Save(MainSettings().UserName + "_" + MainSettings().id, AllSettings[ConfigSelect].name, JFile.dump());
					JFile.clear();
					UpdateList();
					Message::Get().Start(!MainSettings().Russian ? XorStr("Settings loaded!") : XorStr(u8"Ќастройки загружены!"));
				}
			}
		}
	}
	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();

	if (X1Gui().Button(!MainSettings().Russian ? XorStr("Update Settings List") : XorStr(u8"ќбновить список настроек"), Vec2(long_item_w, 0)))
	{
		UpdateList();
		Message::Get().Start(!MainSettings().Russian ? XorStr("List updated!") : XorStr(u8"—писок обновлен!"));
	}

	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();

	if (X1Gui().Button(!MainSettings().Russian ? XorStr("Save Settings") : XorStr(u8"—охранить настройки"), Vec2(long_item_w, 0)))
	{
		if (ConfigSelect >= 0 && ConfigSelect < (int)AllSettings.size())
		{
			SetName(SettignBaseDir + AllSettings[ConfigSelect].name);
			if (Save(AllSettings[ConfigSelect].name))
			{
				UpdateList();
				Message::Get().Start(!MainSettings().Russian ? XorStr("Settings saved!") : XorStr(u8"Ќастройки сохранены!"));
				/*if(MainCheatConfig.AutoLoadEnable)
					MainCheatConfig.UpdateStartupFile(ConfigList[ConfigSelect]);
				else
					MainCheatConfig.UpdateStartupFile("<>");*/
			}
			else
				Message::Get().Start(!MainSettings().Russian ? XorStr("Error save!") : XorStr(u8"ќшибка сохранени€!"));
		}
	}
	X1Gui().Spacing();
	if (X1Gui().Button(!MainSettings().Russian ? XorStr("Load Settings") : XorStr(u8"«агрузить настройки"), Vec2(long_item_w, 0)))
	{
		if (ConfigSelect >= 0 && ConfigSelect < (int)AllSettings.size())
		{
			SetName(SettignBaseDir + AllSettings[ConfigSelect].name);
			if (!AllSettings[ConfigSelect].cloud)
			{
				if (Load())
					Message::Get().Start(!MainSettings().Russian ? XorStr("Settings loaded!") : XorStr(u8"Ќастройки загружены!"));
				else
					Message::Get().Start(!MainSettings().Russian ? XorStr("Error load!") : XorStr(u8"ќшибка загрузки!"));
			}
			else
			{
				string _shared_name = "";
				if (CloudLoad(AllSettings[ConfigSelect].cloud_hash, _shared_name))
					Message::Get().Start(!MainSettings().Russian ? XorStr("Settings loaded!") : XorStr(u8"Ќастройки загружены!"));
				else
					Message::Get().Start(!MainSettings().Russian ? XorStr("Error load!") : XorStr(u8"ќшибка загрузки!"));
			}
		}
	}
	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();
	if (X1Gui().Button(!MainSettings().Russian ? XorStr("Delete Settings") : XorStr(u8"”далить настройки"), Vec2(long_item_w, 0)))
	{
		if (ConfigSelect >= 0 && ConfigSelect < (int)AllSettings.size())
		{
			auto DeleteBackup = [&](string pref)->void
			{
				/*if (MainCheatConfig.BackupSettings)
				{
					SYSTEMTIME Time;
					FastCall::G().t_GetLocalTime(&Time);

					string name = AllSettings[ConfigSelect].name;
					if (name.size() > 5)
						name.resize(name.size() - 5);

					string cfg_name = name + pref + to_string(Time.wDay) + "_" + to_string(Time.wHour) + "_" + to_string(Time.wMinute);

					string config_name_back = SystemDisk + XorStr("XONE\\Settings\\Backup");
					if (!isDirectoryExists(config_name_back.c_str()))
						CreateDirectory(config_name_back.c_str(), NULL);

					config_name_back += "\\" + cfg_name + ".json";
					CopyFile__(config_name_back, SystemDisk + XorStr("XONE\\Settings\\") + AllSettings[ConfigSelect].name);
				}*/
			};

			if (!AllSettings[ConfigSelect].cloud && !AllSettings[ConfigSelect].pc_cloud)
			{
				DeleteBackup("_delete_copy_");
				string FulDel = SettignBaseDir + AllSettings[ConfigSelect].name;

				if (AllSettings.size() > 0)
				{
					DeleteFile(FulDel.c_str());
				}
			}
			if (AllSettings[ConfigSelect].cloud)
			{
				CCloudSets::CCloudGet().Delete(MainSettings().UserName + "_" + MainSettings().id, AllSettings[ConfigSelect].id, AllSettings[ConfigSelect].name);
			}
			if (AllSettings[ConfigSelect].pc_cloud)
			{
				DeleteBackup("_delete_copy_");
				string FulDel = SettignBaseDir + AllSettings[ConfigSelect].name;

				if (AllSettings.size() > 0)
				{
					DeleteFile(FulDel.c_str());
				}

				CCloudSets::CCloudGet().Delete(MainSettings().UserName + "_" + MainSettings().id, AllSettings[ConfigSelect].id, AllSettings[ConfigSelect].name);
			}

			UpdateList();
			Message::Get().Start(!MainSettings().Russian ? XorStr("Settings delited!") : XorStr(u8"Ќастройки удалены!"));
		}
	}

	X1Gui().Spacing();
	X1Gui().Separator();
	X1Gui().Spacing();

	//DCheckBox("Backup Settings", u8"–езервное копирование настроек", MainCheatConfig.BackupSettings, u8"");

	////AutoLoadSaveEnable
	//CheckBox("AutoLoadSaveEnable", u8"AutoLoadSaveEnable", MainCheatConfig.AutoSaveEnable, u8"");
	//if (MainCheatConfig.AutoSaveEnable)
	//{
	//	if (ConfigSelect >= 0 && ConfigSelect < ConfigList.size())
	//		MainCheatConfig.AutoSaveFile = ConfigList[ConfigSelect];
	//}
	//else
	//	MainCheatConfig.AutoSaveFile = XorStr("<>");
	////AutoSaveFile
	//CheckBox("AutoLoad", u8"AutoLoad", MainCheatConfig.AutoLoadEnable, u8"");
	////X1Gui().Text("AutoLoad File: %s", MainCheatConfig.AutoLoadFile.c_str());



}

string CGSettings::readFile(const string& fileName)
{
	ifstream f(fileName);
	if (!f)
		return XorStr("Read Error");
	stringstream ss;
	ss << f.rdbuf();
	return ss.str();
}

void CConfig::Save(nlohmann::json &j)
{
	for (auto &v : VecBool)
		j[SectionName][v.Key] = v.Var;

	for (auto &v : VecInt)
		j[SectionName][v.Key] = v.Var;

	for (auto &v : VecFloat)
		j[SectionName][v.Key] = v.Var;

	for (auto &v : VecDouble)
		j[SectionName][v.Key] = v.Var;

	for (auto &v : VecString)
		j[SectionName][v.Key] = v.Var;

	for (auto &v : VecColor)
	{
		j[SectionName][v.Key][XorStr("R")] = v.Var.r();
		j[SectionName][v.Key][XorStr("G")] = v.Var.g();
		j[SectionName][v.Key][XorStr("B")] = v.Var.b();
		j[SectionName][v.Key][XorStr("A")] = v.Var.a();
		j[SectionName][v.Key][XorStr("Rainbow")] = v.Var.brainbow;
		j[SectionName][v.Key][XorStr("flRainbow")] = v.Var.rainbow;
	}

	for (auto &v : VecBind)
	{
		j[SectionName][v.Key][XorStr("Enable")] = v.Var.Enable;
		j[SectionName][v.Key][XorStr("Button")] = v.Var.Button;
		j[SectionName][v.Key][XorStr("Hold")] = v.Var.Hold;
	}

	ADD_LOG("Settings: %s Saved\n", SectionName.c_str());
}

void CConfig::Load(nlohmann::json j)
{
	if (!j[SectionName].is_null())
	{
		for (auto &v : VecBool)
			if (!j[SectionName][v.Key].is_null())
				v.Var = j[SectionName][v.Key];

		for (auto &v : VecInt)
			if (!j[SectionName][v.Key].is_null())
				v.Var = j[SectionName][v.Key];

		for (auto &v : VecFloat)
			if (!j[SectionName][v.Key].is_null())
				v.Var = j[SectionName][v.Key];

		for (auto &v : VecDouble)
			if (!j[SectionName][v.Key].is_null())
				v.Var = j[SectionName][v.Key];

		for (auto &v : VecString)
			if (!j[SectionName][v.Key].is_null())
				v.Var = j[SectionName][v.Key].get<string>();

		for (auto &v : VecColor)
		{
			if (!j[SectionName][v.Key].is_null())
			{
				if (!j[SectionName][v.Key][XorStr("R")].is_null())
					v.Var[0] = j[SectionName][v.Key][XorStr("R")];

				if (!j[SectionName][v.Key][XorStr("G")].is_null())
					v.Var[1] = j[SectionName][v.Key][XorStr("G")];

				if (!j[SectionName][v.Key][XorStr("B")].is_null())
					v.Var[2] = j[SectionName][v.Key][XorStr("B")];

				if (!j[SectionName][v.Key][XorStr("A")].is_null())
					v.Var[3] = j[SectionName][v.Key][XorStr("A")];

				if (!j[SectionName][v.Key][XorStr("Rainbow")].is_null())
					v.Var.brainbow = j[SectionName][v.Key][XorStr("Rainbow")];

				if (!j[SectionName][v.Key][XorStr("flRainbow")].is_null())
					v.Var.rainbow = j[SectionName][v.Key][XorStr("flRainbow")];
			}
		}

		for (auto &v : VecBind)
		{
			if (!j[SectionName][v.Key].is_null())
			{
				if (!j[SectionName][v.Key][XorStr("Enable")].is_null())
					v.Var.Enable = j[SectionName][v.Key][XorStr("Enable")];

				if (!j[SectionName][v.Key][XorStr("Button")].is_null())
					v.Var.Button = j[SectionName][v.Key][XorStr("Button")];

				if (!j[SectionName][v.Key][XorStr("Hold")].is_null())
					v.Var.Hold = j[SectionName][v.Key][XorStr("Hold")];
			}
		}
		ADD_LOG("Settings: %s Loaded\n", SectionName.c_str());
	}
}

void CConfig::UpdateColors()
{
	for (auto &v : VecColor)
		if (v.Var.brainbow)
			v.Var.UpdateRainbow(0.99f, 1.f);
}