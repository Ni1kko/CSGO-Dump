#include "..\hooks.hpp"

using FindMDL_t = void(__thiscall*)(void*, char*);

void __fastcall hooks::hooked_findmdl(void* ecx, void* edx, char* FilePath)
{
	static auto original_fn = modelcache_hook->get_func_address<FindMDL_t>(10);

	if (g_cfg.models.Enabled)
	{
		if (strstr(FilePath, "knife_default_ct.mdl") || strstr(FilePath, "knife_default_t.mdl")) {

			switch (g_cfg.models.KnifeModel)
			{
			case 0:  return original_fn(ecx, "models/weapons/wurstnurst/m9glitch.mdl");  break;
			case 1:  return original_fn(ecx, "models/weapons/knife_pickaxe.mdl");  break;
			case 2:  return original_fn(ecx, "models/weapons/dickknifebywurst00.mdl");  break;
			case 3:  return original_fn(ecx, "models/KARAM_PurpleShit.vtf");  break;
			}

		}

		if (strstr(FilePath, "awp.mdl")) {

			switch (g_cfg.models.AwpModel)
			{
			case 0:  return original_fn(ecx, "models/weapons/wurstnurst/awp_as50.mdl");  break;
			case 1:  return original_fn(ecx, "Path 2");  break;
			}

		}


		if (strstr(FilePath, "ak47.mdl")) {

			switch (g_cfg.models.AwpModel)
			{
			case 0:  return original_fn(ecx, "models/weapons/wurstnurst/royal_guard_ak.mdl");  break;
			case 1:  return original_fn(ecx, "Path 2");  break;
			}

		}
		// Player Tab - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

		if (strstr(FilePath, "arms")) {

			switch (g_cfg.models.ArmModel)
			{
			case 0:  return original_fn(ecx, "models/weapons/v_models/arms/wurstnurst/ringarms.mdl");  break;
			case 1:  return original_fn(ecx, "models/player/custom_player/kaesar2018/thanos/thanos_arms.mdl");  break;
			case 2:  return original_fn(ecx, "models/player/custom_player/kuristaja/octodad/octodad_black_arms.mdl");  break;
			}

		}
		return original_fn(ecx, FilePath);
	}
	else
	{
		return original_fn(ecx, FilePath);
	}



	////Weapon Tab - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	//if (strstr(FilePath, "knife_default_ct.mdl") || strstr(FilePath, "knife_default_t.mdl")) {

	//	switch (g_cfg.models.KnifeModel)
	//	{
	//	case 0:  return original_fn(ecx, "models/weapons/wurstnurst/m9glitch.mdl");  break;
	//	case 1:  return original_fn(ecx, "models/weapons/knife_pickaxe.mdl");  break;
	//	case 2:  return original_fn(ecx, "models/weapons/dickknifebywurst00.mdl");  break;
	//	case 3:  return original_fn(ecx, "models/KARAM_PurpleShit.vtf");  break;
	//	}

	//}

	//if (strstr(FilePath, "awp.mdl")) {

	//	switch (g_cfg.models.AwpModel)
	//	{
	//	case 0:  return original_fn(ecx, "models/weapons/wurstnurst/awp_as50.mdl");  break;
	//	case 1:  return original_fn(ecx, "Path 2");  break;
	//	}

	//}


	//if (strstr(FilePath, "ak47.mdl")) {

	//	switch (g_cfg.models.AwpModel)
	//	{
	//	case 0:  return original_fn(ecx, "models/weapons/wurstnurst/royal_guard_ak.mdl");  break;
	//	case 1:  return original_fn(ecx, "Path 2");  break;
	//	}

	//}
	//// Player Tab - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	//if (strstr(FilePath, "arms")) {

	//	switch (g_cfg.models.ArmModel)
	//	{
	//	case 0:  return original_fn(ecx, "models/weapons/v_models/arms/wurstnurst/ringarms.mdl");  break;
	//	case 1:  return original_fn(ecx, "models/player/custom_player/kaesar2018/thanos/thanos_arms.mdl");  break;
	//	case 2:  return original_fn(ecx, "models/player/custom_player/kuristaja/octodad/octodad_black_arms.mdl");  break;
	//	}

	//}
	return original_fn(ecx, FilePath);
}