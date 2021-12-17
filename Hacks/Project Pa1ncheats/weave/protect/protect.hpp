#include <string>

#define _(namespace_fix, string_format) class namespace_fix { public: inline static std::string s() { return (std::string)string_format; }   }  


namespace hs {
	/* modules */
	_(serverbrowser_dll, "serverbrowser.dll"); // serverbrowser.dll
	_(engine_dll, "engine.dll"); // engine.dll
	_(client_dll, "client.dll"); // client.dll
	_(vstdlib_dll, "vstdlib.dll"); // vstdlib.dll
	_(input_system_dll, "inputsystem.dll"); // inputsystem.dll
	_(vgui_mat_surface_dll, "vguimatsurface.dll"); // vguimatsurface.dll
	_(server_dll, "server.dll"); // server.dll
	_(vgui2_dll, "vgui2.dll"); // vgui2.dll
	_(mat_sys_dll, "materialsystem.dll"); // materialsystem.dll
	_(studio_render_dll, "studiorender.dll"); // studiorender.dll
	_(physics_dll, "vphysics.dll"); // vphysics.dll
	_(data_cache_dll, "datacache.dll"); // datacache.dll
	_(tier0_dll, "tier0.dll"); // tier0.dll
	_(gameoverlayrenderer_dll, "gameoverlayrenderer.dll"); // gameoverlayrenderer.dll

	/* patterns & pattern names */

	_(reset, "reset"); // reset
	_(reset_p, "53 57 C7 45"); // 53 57 C7 45

	_(client_state, "client_state"); // client_state
	_(client_state_p, "A1 ? ? ? ? 8B 80 ? ? ? ? C3"); // A1 ? ? ? ? 8B 80 ? ? ? ? C3

	_(move_helper, "move_helper"); // move_helper
	_(move_helper_p, "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01"); // 8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01

	_(input, "input"); // input
	_(input_p, "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10"); // B9 ? ? ? ? F3 0F 11 04 24 FF 50 10

	_(glow_manager, "glow_manager"); // glow_manager
	_(glow_manager_p, "0F 11 05 ? ? ? ? 83 C8 01"); // 0F 11 05 ? ? ? ? 83 C8 01

	_(beams, "beams"); // beams
	_(beams_p, "A1 ? ? ? ? 56 8B F1 B9 ? ? ? ? FF 50 08"); // A1 ? ? ? ? 56 8B F1 B9 ? ? ? ? FF 50 08

	_(update_clientside_anim, "update_clientside_anim"); // update_clientside_anim
	_(update_clientside_anim_p, "8B 0D ? ? ? ? 53 56 57 8B 99 ? ? ? ? 85 DB 74 1C"); // 55 8B EC 51 56 8B F1 80 BE ?? ?? 00 00 00 74 36 8B 06 FF 90 ?? ?? 00 00

	_(debp, "debp"); // debp
	_(debp_p, "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 56 8B F1 57 89 74 24 1C"); // 55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 56 8B F1 57 89 74 24 1C

	_(standard_blending_rules, "standard_blending_rules"); // standard_blending_rules
	_(standard_blending_rules_p, "55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? 56 8B 75 08 57 8B F9 85 F6"); // 55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? 56 8B 75 08 57 8B F9 85 F6

	_(should_skip_anim_frame, "should_skip_anim_frame"); // should_skip_anim_frame
	_(should_skip_anim_frame_p, "57 8B F9 8B 07 8B 80 ? ? ? ? FF D0 84 C0 75 02 5F C3"); // 57 8B F9 8B 07 8B 80 ? ? ? ? FF D0 84 C0 75 02 5F C3

	_(get_foreign_fall_back_name, "get_foreign_fall_back_name"); // get_foreign_fall_back_name
	_(get_foreign_fall_back_name_p, "80 3D ? ? ? ? ? 74 06 B8"); // 80 3D ? ? ? ? ? 74 06 B8

	_(setup_bones, "setup_bones"); // setup_bones
	_(setup_bones_p, "55 8B EC 83 E4 F0 B8 D8"); // 55 8B EC 83 E4 F0 B8 D8

	_(build_transformations, "build_transformations"); // build_transformations
	_(build_transformations_p, "55 8B EC 83 E4 F0 81 EC ? ? ? ? 56 57 8B F9 8B 0D ? ? ? ? 89 7C 24 1C"); // 55 8B EC 83 E4 F0 81 EC ? ? ? ? 56 57 8B F9 8B 0D ? ? ? ? 89 7C 24 1C

	_(effects, "effects"); // effects
	_(effects_p, "8B 35 ? ? ? ? 85 F6 0F 84 ? ? ? ? 0F"); // 8B 35 ? ? ? ? 85 F6 0F 84 ? ? ? ? 0F

	/* without hash map */

	/* misc */
	_(content_type1, "Content-Type"); // Content-Type
	_(content_type2, "application/x-www-form-urlencoded"); // application/x-www-form-urlencoded
	_(post, "POST"); // POST
	_(hwid_, "hwid="); // hwid=
	_(localhost, "127.0.0.1"); // 127.0.0.1
	_(watermark, "PA1N CHEATS [alpha]");
	_(group_vk, "https://discord.gg/pFtJwQPY");
	_(open, "open");
	_(weave_su, "PA1N");
	_(weave_logo, "PA1N");

	_(clantag1, "  paincheats.pw ||  ");
	_(clantag2, "  pa1ncheats.pw ||  ");
	_(clantag3, "  paincheats.pw ||  ");
	_(clantag4, "  pa1ncheats.pw ||  ");
	_(clantag5, "  paincheats.pw ||  ");
	_(clantag6, "  pa1ncheats.pw ||  ");
	_(clantag7, "  paincheats.pw ||  ");
	_(clantag8, "  pa1ncheats.pw ||  ");
	_(clantag9, "  paincheats.pw ||  ");
	_(clantag10, " pa1ncheats.pw ||  ");


	// netvar tables
	_(DT_BaseEntity, "DT_BaseEntity"); // DT_BaseEntity
	_(DT_BasePlayer, "DT_BasePlayer"); // DT_BasePlayer
	_(DT_CSPlayer, "DT_CSPlayer"); // DT_CSPlayer
	_(DT_LocalPlayerExclusive, "DT_LocalPlayerExclusive"); // DT_LocalPlayerExclusive
	_(DT_BaseAnimating, "DT_BaseAnimating"); // DT_BaseAnimating
	_(DT_WeaponCSBase, "DT_WeaponCSBase"); // DT_WeaponCSBase
	_(DT_WeaponCSBaseGun, "DT_WeaponCSBaseGun"); // DT_WeaponCSBaseGun
	_(DT_BaseCombatWeapon, "DT_BaseCombatWeapon"); // DT_BaseCombatWeapon
	_(DT_Ragdoll, "DT_Ragdoll"); // DT_BaseCombatWeapon
	_(DT_BaseAttributableItem, "DT_BaseAttributableItem"); // DT_BaseAttributableItem
	_(DT_BaseCSGrenade, "DT_BaseCSGrenade"); // DT_BaseCSGrenade

	// netvar names
	_(m_vecMins, "m_vecMins"); // m_vecMins
	_(m_vecMaxs, "m_vecMaxs"); // m_vecMaxs
	_(m_hObserverTarget, "m_hObserverTarget"); // m_hObserverTarget
	_(m_vecAbsVelocity, "m_vecAbsVelocity"); // m_vecAbsVelocity
	_(m_hGroundEntity, "m_hGroundEntity"); // m_hGroundEntity
	_(m_fFlags, "m_fFlags"); // m_fFlags
	_(m_flHealthShotBoostExpirationTime, "m_flHealthShotBoostExpirationTime"); // m_flHealthShotBoostExpirationTime
	_(m_vecBaseVelocity, "m_vecBaseVelocity"); // m_vecBaseVelocity
	_(m_nButtons, "m_nButtons"); // m_nButtons
	_(m_afButtonLast, "m_afButtonLast"); // m_afButtonLast
	_(m_afButtonPressed, "m_afButtonPressed"); // m_afButtonPressed
	_(m_afButtonReleased, "m_afButtonReleased"); // m_afButtonReleased
	_(nNextThinkTick, "nNextThinkTick"); // nNextThinkTick
	_(m_surfaceFriction, "m_surfaceFriction"); // m_surfaceFriction
	_(m_flPoseParameter, "m_flPoseParameter"); // m_flPoseParameter
	_(m_vecViewOffset, "m_vecViewOffset[0]"); // m_vecViewOffset[0]
	_(m_flDuckSpeed, "m_flDuckSpeed"); // m_flDuckSpeed
	_(m_flDuckAmount, "m_flDuckAmount"); // m_flDuckAmount
	_(m_iObserverMode, "m_iObserverMode"); // m_iObserverMode
	_(m_angEyeAngles, "m_angEyeAngles[0]"); // m_angEyeAngles[0]
	_(m_flVelocityModifier, "m_flVelocityModifier"); // m_flVelocityModifier
	_(m_nSequence, "m_nSequence"); // m_nSequence
	_(m_flFallVelocity, "m_flFallVelocity"); // m_flFallVelocity
	_(m_iTeamNum, "m_iTeamNum"); // m_iTeamNum
	_(m_flFlashMaxAlpha, "m_flFlashMaxAlpha"); // m_flFlashMaxAlpha
	_(m_ArmorValue, "m_ArmorValue"); // m_ArmorValue
	_(m_bReloadVisuallyComplete, "m_bReloadVisuallyComplete"); // m_bReloadVisuallyComplete
	_(m_zoomLevel, "m_zoomLevel"); // m_zoomLevel
	_(m_iClip1, "m_iClip1"); // m_iClip1
	_(m_iClip2, "m_iClip2"); // m_iClip2
	_(m_bHasHelmet, "m_bHasHelmet"); // m_bHasHelmet
	_(m_bHasHeavyArmor, "m_bHasHeavyArmor"); // m_bHasHeavyArmor
	_(m_vecAbsOrigin, "m_vecAbsOrigin"); // m_vecAbsOrigin
	_(m_flSimulationTime, "m_flSimulationTime"); // m_flSimulationTime
	_(m_iEFlags, "m_iEFlags"); // m_iEFlags
	_(m_fEffects, "m_fEffects"); // m_fEffects
	_(m_fLastShotTime, "m_fLastShotTime"); // m_fLastShotTime
	_(m_flRecoilIndex, "m_flRecoilIndex"); // m_flRecoilIndex
	_(m_fAccuracyPenalty, "m_fAccuracyPenalty"); // m_fAccuracyPenalty
	_(m_bStartedArming, "m_bStartedArming"); // m_bStartedArming
	_(m_bGunGameImmunity, "m_bGunGameImmunity"); // m_bGunGameImmunity
	_(m_weaponMode, "m_weaponMode"); // m_weaponMode
	_(m_hConstraintEntity, "m_hConstraintEntity"); // m_hConstraintEntity
	_(m_nTickBase, "m_nTickBase"); // m_nTickBase
	_(m_nImpulse, "m_nImpulse"); // m_nImpulse
	_(m_flAnimTime, "m_flAnimTime"); // m_flAnimTime
	_(m_flCycle, "m_flCycle"); // m_flCycle
	_(m_flLowerBodyYawTarget, "m_flLowerBodyYawTarget"); // m_flLowerBodyYawTarget
	_(m_flPostponeFireReadyTime, "m_flPostponeFireReadyTime"); // m_flPostponeFireReadyTime
	_(m_bIsScoped, "m_bIsScoped"); // m_bIsScoped
	_(m_CollisionGroup, "m_CollisionGroup"); // m_CollisionGroup
	_(m_hViewModel, "m_hViewModel[0]"); // m_hViewModel
	_(m_hActiveWeapon, "m_hActiveWeapon"); // m_hActiveWeapon
	_(m_aimPunchAngle, "m_aimPunchAngle"); // m_aimPunchAngle
	_(m_aimPunchAngleVel, "m_aimPunchAngleVel"); // m_aimPunchAngleVel
	_(m_viewPunchAngle, "m_viewPunchAngle"); // m_viewPunchAngle
	_(m_angAbsRotation, "m_angAbsRotation"); // m_angAbsRotation
	_(m_ragPos, "m_ragPos"); // m_ragPos
	_(m_bClientSideAnimation, "m_bClientSideAnimation"); // m_bClientSideAnimation
	_(m_iItemDefinitionIndex, "m_iItemDefinitionIndex"); // m_iItemDefinitionIndex
	_(m_flNextSecondaryAttack, "m_flNextSecondaryAttack"); // m_flNextSecondaryAttack
	_(m_flNextPrimaryAttack, "m_flNextPrimaryAttack"); // m_flNextPrimaryAttack
	_(m_fThrowTime, "m_fThrowTime"); // m_fThrowTime

	_(CCSPlayer, "CCSPlayer"); //CCSPlayer

	// window
	_(Valve001, "Valve001"); // Valve001

	// hwid
	_(weave_su_subs_check, "https://weave.su/subs_check"); // https://weave.su/subs_check

	// interface
	_(create_interface, "CreateInterface"); // CreateInterface
	_(create_interface_ex, "Can't create interface"); // Can't create interface

	// hook name
	_(impact, "Impact"); // Impact

	// world name
	_(World, "World"); // World
	_(StaticProp, "StaticProp"); // StaticProp
	_(SkyBox, "SkyBox"); // SkyBox

	// prefix
	_(prefix, "[PA1N] "); // weave

	// sky names
	_(sky_dust, "sky_dust"); // sky_dust
	_(sky_csgo_night02, "sky_csgo_night02"); // sky_csgo_night02

	// model group
	_(mdl_textures, "Model textures"); // debug/Model textures

	// model name
	_(player, "models/player"); // player
	_(arms, "arms"); // arms
	_(v, "weapons\v_"); // weapons/v_
	_(uid, "uid"); // uid
	_(stattrack, "stattrack"); // stattrack
	_(propz, "props"); // props

	// model material
	_(debugambientcube, "debug/debugambientcube"); // debug/debugambientcube
	_(debugdrawflat, "debug/debugdrawflat"); // debug/debugdrawflat
	_(regular_ref, "regular_ref"); // regular_ref
	_(glow_armsrace, "dev/glow_armsrace"); // glow_armsrace
	_(regular_glow, "regular_glow"); // regular_glow
	_(glowOverlay, "GlowOverlay"); // aimware glow b1g meme by Syrex#2192 lol

	// material settings name
	_(phongexponent, "$phongexponent"); // phongexponent
	_(phongboost, "$phongboost"); // $phongboost
	_(rimlightexponent, "$rimlightexponent"); // $rimlightexponent
	_(rimlightboost, "$rimlightboost"); // $rimlightboost
	_(pearlescent, "$pearlescent"); // $pearlescent
	_(envmaptint, "$envmaptint"); // $envmaptint
	_(phongtint, "$phongtint"); // $phongtint

	// hitboxes
	_(gear, "gear"); // gear
	_(h_generic, "generic"); // generic
	_(question, "?"); // ?
	_(head, "head"); // head
	_(neck, "neck"); // neck
	_(pelvis, "pelvis"); // pelvis
	_(stomach, "stomach"); // stomach
	_(lower_chest, "lower chest"); // lower chest
	_(chest, "chest"); // chest
	_(upper_chest, "upper chest"); // upper chest
	_(right_thigh, "right thigh"); // right thigh
	_(left_thigh, "left thigh"); // left thigh
	_(right_leg, "right leg"); // right leg
	_(left_leg, "left leg"); // left leg
	_(right_foot, "right foot"); // right foot
	_(left_foot, "left foot"); // left foot
	_(right_hand, "right hand"); // right hand
	_(left_hand, "left hand"); // left hand
	_(right_arm, "right arm"); // right arm
	_(left_arm, "left arm"); // left arm
	_(right_upper_arm, "right upper arm"); // right upper arm
	_(right_lower_arm, "right lower arm"); // right lower arm
	_(left_upper_arm, "left upper arm"); // left upper arm
	_(left_lower_arm, "left lower arm"); // left lower arm

	// eventlog shit
	_(H_, " [H:"); // H:
	_(R_, " [R:"); // H:
	_(B_, " [B:"); // H:
	_(SHOT, " [SHOT] "); // H:
	_(prefix_end, "] "); // ]
	

	// resolver mode
	_(AB, "AB"); // AB
	_(AC, "AC"); // AC
	_(B, "B"); // B
	_(C, "C"); // C
	_(A, "A"); // A
	_(DA, "DA"); // DA
	_(DB, "DB"); // DB

	/* cvar names */
	_(cl_interp, "cl_interp"); // cl_interp
	_(cl_updaterate, "cl_updaterate"); // cl_updaterate
	_(sv_gravity, "sv_gravity"); // sv_gravity
	_(sv_jump_impulse, "sv_jump_impulse"); // sv_jump_impulse
	_(mp_damage_scale_ct_head, "mp_damage_scale_ct_head"); // mp_damage_scale_ct_head
	_(mp_damage_scale_t_head, "mp_damage_scale_t_head"); // mp_damage_scale_t_head
	_(mp_damage_scale_ct_body, "mp_damage_scale_ct_body"); // mp_damage_scale_ct_body
	_(mp_damage_scale_t_body, "mp_damage_scale_t_body"); // mp_damage_scale_t_body
	_(cl_sidespeed, "cl_sidespeed"); // cl_sidespeed
	_(r_DrawSpecificStaticProp, "r_DrawSpecificStaticProp"); // cl_sidespeed
	_(cl_foot_contact_shadows, "cl_foot_contact_shadows"); // cl_foot_contact_shadows
	_(viewmodel_offset_x, "viewmodel_offset_x"); // viewmodel_offset_x
	_(viewmodel_offset_y, "viewmodel_offset_y"); // viewmodel_offset_y
	_(viewmodel_offset_z, "viewmodel_offset_z"); // viewmodel_offset_z
	_(r_aspectratio, "r_aspectratio"); // r_aspectratio
	_(mat_postprocess_enable, "mat_postprocess_enable"); // mat_postprocess_enable
	_(weapon_debug_spread_show, "weapon_debug_spread_show"); // weapon_debug_spread_show
	_(sv_friction, "sv_friction"); // sv_friction
	_(sv_stopspeed, "sv_stopspeed"); // sv_stopspeed
	_(sv_skyname, "sv_skyname"); // sv_skyname
	_(r_3dsky, "r_3dsky"); // r_3dsky
	_(sv_cheats, "sv_cheats"); // sv_cheats
	_(camera, "cl_camera_height_restriction_debug"); // cl_camera_height_restriction_debug
	_(shadows, "cl_csm_shadows"); // cl_csm_shadows

	/* cmd commands */
	_(clear, "clear"); // clear
	_(unload_message, "echo Cheat has been unloaded!"); // echo Cheat has been unloaded!

	/* interface names */
	_(client, "VClient018"); // VClient018
	_(ent_list, "VClientEntityList003"); // VClientEntityList003
	_(leaf_sys, "ClientLeafSystem002"); // ClientLeafSystem002
	_(engine_model, "VEngineModel016"); // VEngineModel016
	_(model_info, "VModelInfoClient004"); // VModelInfoClient004
	_(cvar, "VEngineCvar007"); // VEngineCvar007
	_(engine_client, "VEngineClient014"); // VEngineClient014
	_(engine_trace, "EngineTraceClient004"); // EngineTraceClient004
	_(input_sys, "InputSystemVersion001"); // InputSystemVersion001
	_(vgui_surface, "VGUI_Surface031"); // VGUI_Surface031
	_(server_game_dll, "ServerGameDLL0"); // ServerGameDLL0
	_(vgui_panel, "VGUI_Panel009"); // VGUI_Panel009
	_(mat_sys, "VMaterialSystem080"); // VMaterialSystem080
	_(render_view, "VEngineRenderView014"); // VEngineRenderView014
	_(debug_overlay, "VDebugOverlay004"); // VDebugOverlay004
	_(events_manager, "GAMEEVENTSMANAGER002"); // GAMEEVENTSMANAGER002
	_(studio_render, "VStudioRender026"); // VStudioRender026
	_(phys_props, "VPhysicsSurfaceProps001"); // VPhysicsSurfaceProps001
	_(game_movement, "GameMovement001"); // GameMovement001
	_(engine_vgui, "VEngineVGui001"); // VEngineVGui001
	_(engine_sound, "IEngineSoundClient003"); // IEngineSoundClient003
	_(mdl_cache, "MDLCache004"); // MDLCache004
	_(client_prediction, "VClientPrediction001"); // VClientPrediction001
	_(mem_alloc, "g_pMemAlloc"); // g_pMemAlloc

	/*patterns, netvars & netvar tables*/
	_(shoot_pos, "55 8B EC 56 8B 75 08 57 8B F9 56 8B 07 FF 90 ? ? ? ?"); // 55 8B EC 56 8B 75 08 57 8B F9 56 8B 07 FF 90 ? ? ? ?
	_(bone_cache, "FF B7 ?? ?? ?? ?? 52"); // FF B7 ?? ?? ?? ?? 52
	_(visibility, "E8 ? ? ? ? 83 7D D8 00 7C 0F"); // E8 ? ? ? ? 83 7D D8 00 7C 0F
	_(server_hitbox, "55 8B EC 81 EC ? ? ? ? 53 56 8B 35 ? ? ? ? 8B D9 57 8B CE"); // 55 8B EC 81 EC ? ? ? ? 53 56 8B 35 ? ? ? ? 8B D9 57 8B CE
	_(server_edict, "8B 15 ? ? ? ? 33 C9 83 7A 18 01"); // 8B 15 ? ? ? ? 33 C9 83 7A 18 01
	_(reset_animstate, "56 6A 01 68 ? ? ? ? 8B F1"); // 56 6A 01 68 ? ? ? ? 8B F1
	_(create_animstate, "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46"); // 55 8B EC 56 8B F1 B9 ? ? ? ? C7 46
	_(update_animstate, "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24"); // 55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24
	_(post_think, "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9"); // 55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9
	_(simulate_player, "56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 72"); // 56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 72
	_(physics_run_think, "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87"); // 55 8B EC 83 EC 10 53 56 57 8B F9 8B 87
	_(set_abs_angles, "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8"); // 55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8
	_(set_abs_origin, "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8"); // 55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8
	_(unk_func, "55 8B EC 56 57 8B F9 8B B7 ? ? ? ? 8B C6"); // 55 8B EC 56 57 8B F9 8B B7 ? ? ? ? 8B C6
	_(select_item, "55 8B EC 56 8B F1 ? ? ? 85 C9 74 71 8B 06"); // 55 8B EC 56 8B F1 ? ? ? 85 C9 74 71 8B 06
	_(seq_activity, "55 8B EC 53 8B 5D 08 56 8B F1 83"); // 55 8B EC 53 8B 5D 08 56 8B F1 83
	_(physics_recursive, "55 8B EC 83 E4 F8 83 EC 0C 53 8B 5D 08 8B C3 56"); // 55 8B EC 83 E4 F8 83 EC 0C 53 8B 5D 08 8B C3 56
	_(trace_filter, "55 8B EC 83 E4 F0 83 EC 7C 56 52"); // 55 8B EC 83 E4 F0 83 EC 7C 56 52
	_(is_breakable, "55 8B EC 51 56 8B F1 85 F6 74 68"); // 55 8B EC 51 56 8B F1 85 F6 74 68
	_(bone_cache_validation, "C6 05 ? ? ? ? ? 89 47 70"); // C6 05 ? ? ? ? ? 89 47 70
	_(load_named_sky, "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45"); // C6 05 ? ? ? ? ? 89 47 70
	_(set_clantag, "53 56 57 8B DA 8B F9 FF 15"); // 53 56 57 8B DA 8B F9 FF 15
	_(seq_transition, "84 C0 74 17 8B 87"); // 84 C0 74 17 8B 87
	_(choke_limit, "B8 ? ? ? ? 3B F0 0F 4F F0 89 5D FC"); // B8 ? ? ? ? 3B F0 0F 4F F0 89 5D FC
	_(write_user_cmd, "55 8B EC 83 E4 F8 51 53 56 8B D9 8B 0D"); // 55 8B EC 83 E4 F8 51 53 56 8B D9 8B 0D
	_(write_user_cmd_retn, "84 C0 74 04 B0 01 EB 02 32 C0 8B FE 46 3B F3 7E C9 84 C0 0F 84 ? ? ? ?"); // 84 C0 74 04 B0 01 EB 02 32 C0 8B FE 46 3B F3 7E C9 84 C0 0F 84 ? ? ? ?
	_(modify_eye_pos, "85 C0 0F 84 ? ? ? ? 8B 45 08 8B 0D"); // 85 C0 0F 84 ? ? ? ? 8B 45 08 8B 0D
	_(ret_engine, "\x85\xC0\x74\x2D\x83\x7D\x10\x00\x75\x1C"); // \x85\xC0\x74\x2D\x83\x7D\x10\x00\x75\x1C
	_(setup_vel, "84 C0 75 38 8B 0D ? ? ? ? 8B 01 8B 80"); // 84 C0 75 38 8B 0D ? ? ? ? 8B 01 8B 80
	_(accum_layers, "84 C0 75 0D F6 87"); // 84 C0 75 38 8B 0D ? ? ? ? 8B 01 8B 80
	_(retn_camera, "85 C0 75 30 38 86"); // 85 C0 75 30 38 86
	_(game_rules, "A1 ? ? ? ? 8B 0D ? ? ? ? 6A 00 68 ? ? ? ? C6"); // A1 ? ? ? ? 8B 0D ? ? ? ? 6A 00 68 ? ? ? ? C6
	_(view_matrix, "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9"); // 0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9
	_(smoke_effect, "A3 ? ? ? ? 57 8B CB"); // A3 ? ? ? ? 57 8B CB

	_(d3dx9fix, "shaderapidx9.dll"); 
	_(d3dx9fix_pattern, "A1 ? ? ? ? 50 8B 08 FF 51 0C");

}



namespace fnv_1a {
	template< typename S >
	struct fnv_internal;
	template< typename S >
	struct fnv1a;

	template< >
	struct fnv_internal< uint32_t > {
		constexpr static uint32_t default_offset_basis = 2;
		//constexpr static uint32_t prime = 0x01000193;
		constexpr static uint32_t prime = 2;
	};

	template< >
	struct fnv1a< uint32_t > : public fnv_internal< uint32_t > {
		 static uint32_t hash(char const* string, const uint32_t val = default_offset_basis) {
			unsigned int t = 0;
			for (auto i = 0; i < strlen(string) + 1; i++)
				t += (int)string[i];
			return t;

		}
	};
}

using fnv = fnv_1a::fnv1a< uint32_t >;



extern std::string g_Modules[9999999];
extern std::string g_Patterns[9999999];