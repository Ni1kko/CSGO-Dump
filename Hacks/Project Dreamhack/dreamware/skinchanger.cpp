#include <Windows.h>
#include "../../MISC/includes.h"
#include "../../UTILS/interfaces.h"
#include "../../UTILS/offsets.h"
#include "../../SDK/CBaseAnimState.h"
#include "../../SDK/CInput.h"
#include "../../SDK/IClient.h"
#include "../../SDK/CPanel.h"
#include "../../UTILS/render.h"
#include "../../SDK/ConVar.h"
#include "../../SDK/CGlowObjectManager.h"
#include "../../SDK/IEngine.h"
#include "../../SDK/CTrace.h"
#include "../../SDK/CClientEntityList.h"
#include "../../SDK/CBaseWeapon.h"
#include "../../SDK/ModelInfo.h"
#include "../../SDK/ModelRender.h"
#include "../../SDK/RenderView.h"
#include "../../SDK/CTrace.h"
#include "../../SDK/CViewSetup.h"
#include "../../SDK/CGlobalVars.h"
#include "../../UTILS/NetvarHookManager.h"
#include "../../SDK/CBaseEntity.h"
#include "../../SDK/RecvData.h"
#include "skinchanger.h"


#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);
std::unordered_map<char*, char*> killIcons = {};
#define INVALID_EHANDLE_INDEX 0xFFFFFFFF
HANDLE worldmodel_handle;
SDK::CBaseWeapon* worldmodel;
SDK::CBaseWeapon* pWeapon;
#define MakePtr(cast, ptr, addValue) (cast)( (DWORD)(ptr) + (DWORD)(addValue))
int custommodel;




enum class EStickerAttributeType
{
	Index,
	Wear,
	Scale,
	Rotation
};

static uint16_t s_iwoff = 0;

static void* o_uint_fn;

static unsigned int __fastcall hooked_uint_fn(void* thisptr, void*, int slot, EStickerAttributeType attribute, unsigned fl)
{
	SDK::CBaseWeapon* item = reinterpret_cast<SDK::CBaseWeapon*>(uintptr_t(thisptr) - s_iwoff);

	if (attribute == EStickerAttributeType::Index)
	{
		switch (slot)
		{
		case 0:
			return SETTINGS::settings.allin1skins[*item->fixskins()].Stikers1;
			break;
		case 1:
			return SETTINGS::settings.allin1skins[*item->fixskins()].Stikers2;
			break;
		case 2:
			return SETTINGS::settings.allin1skins[*item->fixskins()].Stikers3;
			break;
		case 3:
			return SETTINGS::settings.allin1skins[*item->fixskins()].Stikers4;
			break;
		default:
			break;
		}
	}

	return reinterpret_cast<decltype(hooked_uint_fn)*>(o_uint_fn)(thisptr, nullptr, slot, attribute, fl);
}


void ApplyStickerHooks(SDK::CBaseWeapon* item)
{
	if (!s_iwoff)
		s_iwoff = 0x2DC0 + 0xC; //m_Item

	void**& iw_vt = *reinterpret_cast<void***>(uintptr_t(item) + s_iwoff);

	static void** iw_hook_vt = nullptr;

	if (!iw_hook_vt)
	{
		size_t len = 0;
		for (; iw_vt[len]; ++len);
		iw_hook_vt = new void*[len];

		memcpy(iw_hook_vt, iw_vt, len * sizeof(void*));

		o_uint_fn = iw_hook_vt[5];
		iw_hook_vt[5] = &hooked_uint_fn;
	}

	iw_vt = iw_hook_vt;
}




void skinchanger()
{
	auto pLocal = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!pLocal) return;
	auto weapons = pLocal->m_hMyWeapons();
	//if (Shonax.Skinchanger.skinenabled || Shonax.Skinchanger.customchams)
	if (weapons)
	for (size_t i = 0; weapons[i] != INVALID_EHANDLE_INDEX; i++)
	{
	//	if (weapons[i] != nullptr)
		{
			SDK::CBaseEntity *pEntity = (SDK::CBaseEntity*)INTERFACES::ClientEntityList->GetClientEntityFromHandle(weapons[i]/*crash here*/);
			if (pEntity)
			{
				pWeapon = (SDK::CBaseWeapon*)pEntity;
				if (SETTINGS::settings.stickers)
					if (SETTINGS::settings.number == 59 || SETTINGS::settings.number == 500 || SETTINGS::settings.number == 42 || SETTINGS::settings.number == 507 || SETTINGS::settings.number == 506 || SETTINGS::settings.number == 508 || SETTINGS::settings.number == 509 || SETTINGS::settings.number == 515 || SETTINGS::settings.number == 516 || SETTINGS::settings.number == 505 || SETTINGS::settings.number == 512 || SETTINGS::settings.number > 510)
					{
						//do nothing, fix crash when we force update and trying to add stickers for knife
					}
					else
						ApplyStickerHooks(pWeapon);

				if (SETTINGS::settings.skinenabled || SETTINGS::settings.customchams)
				{
					int Model = SETTINGS::settings.Knife;
					int weapon = *pWeapon->fixskins();
					custommodel = SETTINGS::settings.knifecustom;
					switch (weapon)
					{
					case 7: // AK47 
					{
						switch (SETTINGS::settings.AK47Skin)
						{
						case 0:  *pWeapon->FallbackPaintKit() = 0;   break; //none
						case 1:  *pWeapon->FallbackPaintKit() = 341; break;  //fire serpent
						case 2:  *pWeapon->FallbackPaintKit() = 524; break;  //Fuel Injector
						case 3:  *pWeapon->FallbackPaintKit() = 639; break;  //Bloodsport
						case 4:  *pWeapon->FallbackPaintKit() = 302; break;  //vulcan
						case 5:  *pWeapon->FallbackPaintKit() = 44;  break;  //case hardened
						case 6:  *pWeapon->FallbackPaintKit() = 456; break;  //Hydroponic
						case 7:  *pWeapon->FallbackPaintKit() = 474; break;  //Aquamarine Revenge
						case 8:  *pWeapon->FallbackPaintKit() = 490; break;  //Frontside Misty
						case 9:  *pWeapon->FallbackPaintKit() = 506; break;  //Point Disarray
						case 10: *pWeapon->FallbackPaintKit() = 600; break;  //Neon Revolution
						case 11: *pWeapon->FallbackPaintKit() = 14;  break;  //red laminate
						case 12: *pWeapon->FallbackPaintKit() = 282; break;  //redline
						case 13: *pWeapon->FallbackPaintKit() = 316; break;  //jaguar
						case 14: *pWeapon->FallbackPaintKit() = 340; break;  //jetset
						case 15: *pWeapon->FallbackPaintKit() = 380; break;  //wasteland rebel
						case 16: *pWeapon->FallbackPaintKit() = 675; break;  //The Empress
						case 17: *pWeapon->FallbackPaintKit() = 422; break;  //Elite Build
						default: break;
						}





					}break;
					case 16: // M4A4
					{

						switch (SETTINGS::settings.M4A4Skin)
						{
						case 0:  *pWeapon->FallbackPaintKit() = 0;   break; //none
						case 1:  *pWeapon->FallbackPaintKit() = 255; break; //Asiimov
						case 2:  *pWeapon->FallbackPaintKit() = 309; break; //Howl
						case 3:  *pWeapon->FallbackPaintKit() = 400; break; //Dragon King
						case 4:  *pWeapon->FallbackPaintKit() = 449; break; //Poseidon
						case 5:  *pWeapon->FallbackPaintKit() = 471; break; //Daybreak
						case 6:  *pWeapon->FallbackPaintKit() = 512; break; //Royal Paladin
						case 7:  *pWeapon->FallbackPaintKit() = 533; break; //BattleStar
						case 8:  *pWeapon->FallbackPaintKit() = 588; break; //Desolate Space
						case 9:  *pWeapon->FallbackPaintKit() = 632; break; //Buzz Kill
						case 10: *pWeapon->FallbackPaintKit() = 155; break; //Bullet Rain
						case 11: *pWeapon->FallbackPaintKit() = 664; break; //Hell Fire
						case 12: *pWeapon->FallbackPaintKit() = 480; break; //Evil Daimyo
						case 13: *pWeapon->FallbackPaintKit() = 384; break; //Griffin
						case 14: *pWeapon->FallbackPaintKit() = 187; break; //Zirka
						case 15: *pWeapon->FallbackPaintKit() = 167; break; //Radiation Harzard
						default: break;
						}


					} break;
					case 2: // dual
					{
						switch (SETTINGS::settings.DualSkin)
						{
						case 0:  *pWeapon->FallbackPaintKit() = 0;   break; //none
						case 1:  *pWeapon->FallbackPaintKit() = 276; break;
						case 2:  *pWeapon->FallbackPaintKit() = 491; break;
						case 3:  *pWeapon->FallbackPaintKit() = 658; break;
						case 4:  *pWeapon->FallbackPaintKit() = 625; break;
						case 5:  *pWeapon->FallbackPaintKit() = 447; break;
						default: break;
						}
					} break;
					case 60: // M4A1
					{
						switch (SETTINGS::settings.M4A1SSkin)
						{
						case 0:  *pWeapon->FallbackPaintKit() = 0;   break; //none
						case 1:  *pWeapon->FallbackPaintKit() = 644; break; //
						case 2:  *pWeapon->FallbackPaintKit() = 326; break; //
						case 3:  *pWeapon->FallbackPaintKit() = 548; break; //
						case 4:  *pWeapon->FallbackPaintKit() = 497; break; //
						case 5:  *pWeapon->FallbackPaintKit() = 430; break; //
						case 6:  *pWeapon->FallbackPaintKit() = 321; break; //
						case 7:  *pWeapon->FallbackPaintKit() = 445; break; //
						case 8:  *pWeapon->FallbackPaintKit() = 587; break; //
						case 9:  *pWeapon->FallbackPaintKit() = 360; break; //
						case 10: *pWeapon->FallbackPaintKit() = 440; break; //
						case 11: *pWeapon->FallbackPaintKit() = 631; break; //
						case 12: *pWeapon->FallbackPaintKit() = 681; break; //
						case 13: *pWeapon->FallbackPaintKit() = 430; break; //
						case 14: *pWeapon->FallbackPaintKit() = 301; break; //
						case 15: *pWeapon->FallbackPaintKit() = 257; break; //
						case 16: *pWeapon->FallbackPaintKit() = 663; break; //
						default: break;
						}
					} break;
					case 9: // AWP
					{
						switch (SETTINGS::settings.AWPSkin)
						{
						case 0:  *pWeapon->FallbackPaintKit() = 0;   break; //none
						case 1:  *pWeapon->FallbackPaintKit() = 279; break; //
						case 2:  *pWeapon->FallbackPaintKit() = 344; break; //
						case 3:  *pWeapon->FallbackPaintKit() = 640; break; //
						case 4:  *pWeapon->FallbackPaintKit() = 446; break; //
						case 5:  *pWeapon->FallbackPaintKit() = 475; break; //
						case 6:  *pWeapon->FallbackPaintKit() = 174; break; //
						case 7:  *pWeapon->FallbackPaintKit() = 51; break; //
						case 8:  *pWeapon->FallbackPaintKit() = 84; break; //
						case 9:  *pWeapon->FallbackPaintKit() = 181; break; //
						case 10: *pWeapon->FallbackPaintKit() = 259; break; //
						case 11: *pWeapon->FallbackPaintKit() = 395; break; //
						case 12: *pWeapon->FallbackPaintKit() = 212; break; //
						case 13: *pWeapon->FallbackPaintKit() = 227; break; //
						case 14: *pWeapon->FallbackPaintKit() = 451; break; //
						case 15: *pWeapon->FallbackPaintKit() = 475; break; //
						case 16: *pWeapon->FallbackPaintKit() = 251; break; //
						case 17: *pWeapon->FallbackPaintKit() = 584; break; //
						case 18: *pWeapon->FallbackPaintKit() = 525; break; //
						case 19: *pWeapon->FallbackPaintKit() = 424; break; //
						case 20: *pWeapon->FallbackPaintKit() = 662; break; //
						case 21: *pWeapon->FallbackPaintKit() = 640; break; //
						default: break;
						}
					} break;
					case 61: // USP
					{
						switch (SETTINGS::settings.USPSkin)
						{
						case 0:  *pWeapon->FallbackPaintKit() = 0;   break; //none
						case 1:  *pWeapon->FallbackPaintKit() = 653; break; //
						case 2:  *pWeapon->FallbackPaintKit() = 637; break; //
						case 3:  *pWeapon->FallbackPaintKit() = 313; break; //
						case 4:  *pWeapon->FallbackPaintKit() = 504; break; //
						case 5:  *pWeapon->FallbackPaintKit() = 183; break; //
						case 6:  *pWeapon->FallbackPaintKit() = 339; break; //
						case 7:  *pWeapon->FallbackPaintKit() = 221; break; //
						case 8:  *pWeapon->FallbackPaintKit() = 290; break; //
						case 9:  *pWeapon->FallbackPaintKit() = 318; break; //
						default: break;
						}
					} break;
					case 4: // Glock
					{
						switch (SETTINGS::settings.GlockSkin)
						{
						case 0:	*pWeapon->FallbackPaintKit() = 0; break;
						case 1: *pWeapon->FallbackPaintKit() = 38; break;
						case 2: *pWeapon->FallbackPaintKit() = 48; break;
						case 3: *pWeapon->FallbackPaintKit() = 437; break;
						case 4: *pWeapon->FallbackPaintKit() = 586; break;
						case 5: *pWeapon->FallbackPaintKit() = 353; break;
						case 6: *pWeapon->FallbackPaintKit() = 680; break;
						case 7: *pWeapon->FallbackPaintKit() = 607; break;
						case 8: *pWeapon->FallbackPaintKit() = 532; break;
						case 9: *pWeapon->FallbackPaintKit() = 381; break;
						case 10: *pWeapon->FallbackPaintKit() = 230; break;
						case 11: *pWeapon->FallbackPaintKit() = 159; break;
						case 12: *pWeapon->FallbackPaintKit() = 623; break;
						case 13: *pWeapon->FallbackPaintKit() = 479; break;
						case 14: *pWeapon->FallbackPaintKit() = 680; break;
						case 15: *pWeapon->FallbackPaintKit() = 367; break;
						default: break;
						}
					}
					break;
					case 1: // Deagle
					{
						switch (SETTINGS::settings.DeagleSkin)
						{
						case 0:
							*pWeapon->FallbackPaintKit() = 0;//none
							break;
						case 1:
							*pWeapon->FallbackPaintKit() = 37;
							break;
						case 2:
							*pWeapon->FallbackPaintKit() = 527;
							break;
						case 3:
							*pWeapon->FallbackPaintKit() = 645;
							break;
						case 4:
							*pWeapon->FallbackPaintKit() = 185;
							break;
						case 5:
							*pWeapon->FallbackPaintKit() = 37;
							break;
						case 6:
							*pWeapon->FallbackPaintKit() = 645;
							break;
						case 7:
							*pWeapon->FallbackPaintKit() = 231;
							break;
						case 8:
							*pWeapon->FallbackPaintKit() = 603;
							break;
						default:
							break;
						}
					}
					break;
					case 3: // Five Seven
					{
						switch (SETTINGS::settings.FiveSkin)
						{
						case 0:
							*pWeapon->FallbackPaintKit() = 0;//none
							break;
						case 1:
							*pWeapon->FallbackPaintKit() = 427;
							break;
						case 2:
							*pWeapon->FallbackPaintKit() = 660;
							break;
						case 3:
							*pWeapon->FallbackPaintKit() = 352;
							break;
						case 4:
							*pWeapon->FallbackPaintKit() = 530;
							break;
						case 5:
							*pWeapon->FallbackPaintKit() = 510;
							break;
						case 6:
							*pWeapon->FallbackPaintKit() = 646;
							break;
						case 7:
							*pWeapon->FallbackPaintKit() = 585;
							break;
						default:
							break;
						}
					}
					break;
					case 8: // AUG
					{
						switch (SETTINGS::settings.AUGSkin)
						{
						case 0:
							*pWeapon->FallbackPaintKit() = 0;//none
							break;
						case 1:
							*pWeapon->FallbackPaintKit() = 9;
							break;
						case 2:
							*pWeapon->FallbackPaintKit() = 33;
							break;
						case 3:
							*pWeapon->FallbackPaintKit() = 280;
							break;
						case 4:
							*pWeapon->FallbackPaintKit() = 455;
							break;
						default:
							break;
						}
					}
					break;
					case 10: // Famas
					{
						switch (SETTINGS::settings.FAMASSkin)
						{
						case 0:
							*pWeapon->FallbackPaintKit() = 0;//none
							break;
						case 1:
							*pWeapon->FallbackPaintKit() = 429;
							break;
						case 2:
							*pWeapon->FallbackPaintKit() = 371;
							break;
						case 3:
							*pWeapon->FallbackPaintKit() = 477;
							break;
						case 4:
							*pWeapon->FallbackPaintKit() = 492;
							break;
						default:
							break;
						}
					}
					break;
					case 11: // G3SG1
					{
						switch (SETTINGS::settings.G3sg1Skin)
						{
						case 0:
							*pWeapon->FallbackPaintKit() = 0;//none
							break;
						case 1:
							*pWeapon->FallbackPaintKit() = 677;
							break;
						case 2:
							*pWeapon->FallbackPaintKit() = 511;
							break;
						case 3:
							*pWeapon->FallbackPaintKit() = 463;
							break;
						case 4:
							*pWeapon->FallbackPaintKit() = 464;
							break;
						case 5:
							*pWeapon->FallbackPaintKit() = 465;
							break;
						case 6:
							*pWeapon->FallbackPaintKit() = 170;
							break;
						case 7:
							*pWeapon->FallbackPaintKit() = 438;
							break;
						default:
							break;
						}
					}
					break;
					case 13: // Galil
					{
						switch (SETTINGS::settings.GalilSkin)
						{
						case 0:
							*pWeapon->FallbackPaintKit() = 0;//none
							break;
						case 1:
							*pWeapon->FallbackPaintKit() = 398;
							break;
						case 2:
							*pWeapon->FallbackPaintKit() = 647;
							break;
						case 3:
							*pWeapon->FallbackPaintKit() = 661;
							break;
						case 4:
							*pWeapon->FallbackPaintKit() = 428;
							break;
						case 5:
							*pWeapon->FallbackPaintKit() = 379;
							break;
						default:
							break;
						}
					}
					break;
					case 14: // M249
					{
						switch (SETTINGS::settings.M249Skin)
						{
						case 0:
							*pWeapon->FallbackPaintKit() = 0;
							break;
						case 1:
							*pWeapon->FallbackPaintKit() = 496;
							break;
						case 2:
							*pWeapon->FallbackPaintKit() = 401;
							break;
						case 3:
							*pWeapon->FallbackPaintKit() = 266;
							break;
						default:
							break;
						}
					}
					break;
					case 17: // Mac 10
					{
						switch (SETTINGS::settings.Mac10Skin)
						{
						case 0:
							*pWeapon->FallbackPaintKit() = 0;//none
							break;
						case 1:
							*pWeapon->FallbackPaintKit() = 433;
							break;
						case 2:
							*pWeapon->FallbackPaintKit() = 651;
							break;
						case 3:
							*pWeapon->FallbackPaintKit() = 310;
							break;
						case 4:
							*pWeapon->FallbackPaintKit() = 498;
							break;
						default:
							break;
						}
					}
					break;
					case 19: // P90
					{
						*pWeapon->FallbackPaintKit() = 156;
					}
					break;
					case 24: // UMP-45
					{
						switch (SETTINGS::settings.UMP45Skin)
						{
						case 0:
							*pWeapon->FallbackPaintKit() = 0;//none
							break;
						case 1:
							*pWeapon->FallbackPaintKit() = 37;
							break;
						case 2:
							*pWeapon->FallbackPaintKit() = 441;
							break;
						case 3:
							*pWeapon->FallbackPaintKit() = 448;
							break;
						case 4:
							*pWeapon->FallbackPaintKit() = 556;
							break;
						case 5:
							*pWeapon->FallbackPaintKit() = 688;
							break;
						default:
							break;
						}
					}
					break;
					case 32: // P2000
					{
						switch (SETTINGS::settings.P2000Skin)
						{
						case 0:
							*pWeapon->FallbackPaintKit() = 0;//none
							break;
						case 1:
							*pWeapon->FallbackPaintKit() = 485;
							break;
						case 2:
							*pWeapon->FallbackPaintKit() = 38;
							break;
						case 3:
							*pWeapon->FallbackPaintKit() = 184;
							break;
						case 4:
							*pWeapon->FallbackPaintKit() = 211;
							break;
						case 5:
							*pWeapon->FallbackPaintKit() = 389;
							break;
						case 6:
							*pWeapon->FallbackPaintKit() = 442;
							break;
						case 7:
							*pWeapon->FallbackPaintKit() = 443;
							break;
						case 8:
							*pWeapon->FallbackPaintKit() = 515;
							break;
						case 9:
							*pWeapon->FallbackPaintKit() = 550;
							break;
						case 10:
							*pWeapon->FallbackPaintKit() = 591;
							break;
						default:
							break;
						}
					}
					break;
					case 33: // MP7
					{
						switch (SETTINGS::settings.Mp9Skin)
						{
						case 0:
							*pWeapon->FallbackPaintKit() = 0;
							break;
						case 1:
							*pWeapon->FallbackPaintKit() = 481;
							break;
						case 2:
							*pWeapon->FallbackPaintKit() = 536;
							break;
						case 3:
							*pWeapon->FallbackPaintKit() = 500;
							break;
						default:
							break;
						}
					}
					break;
					case 34: // MP9
					{
						switch (SETTINGS::settings.Mp9Skin)
						{
						case 0:
							*pWeapon->FallbackPaintKit() = 0;
							break;
						case 1:
							*pWeapon->FallbackPaintKit() = 262;
							break;
						case 2:
							*pWeapon->FallbackPaintKit() = 482;
							break;
						case 3:
							*pWeapon->FallbackPaintKit() = 609;
							break;
						default:
							break;
						}
					}
					break;
					case 36: // P250
					{
						switch (SETTINGS::settings.P250Skin)
						{
						case 0:
							*pWeapon->FallbackPaintKit() = 0;//none
							break;
						case 1:
							*pWeapon->FallbackPaintKit() = 102;
							break;
						case 2:
							*pWeapon->FallbackPaintKit() = 466;
							break;
						case 3:
							*pWeapon->FallbackPaintKit() = 467;
							break;
						case 4:
							*pWeapon->FallbackPaintKit() = 501;
							break;
						case 5:
							*pWeapon->FallbackPaintKit() = 551;
							break;
						case 6:
							*pWeapon->FallbackPaintKit() = 678;
							break;
						default:
							break;
						}
					}
					break;
					case 38: // Scar 20
					{
						switch (SETTINGS::settings.SCAR20Skin)
						{
						case 0:
							*pWeapon->FallbackPaintKit() = 0;//none
							break;
						case 1:
							*pWeapon->FallbackPaintKit() = 165;//Splash Jam
							break;
						case 2:
							*pWeapon->FallbackPaintKit() = 100;//Storm
							break;
						case 3:
							*pWeapon->FallbackPaintKit() = 46;//Contractor
							break;
						case 4:
							*pWeapon->FallbackPaintKit() = 70;//Carbon Fiber
							break;
						case 5:
							*pWeapon->FallbackPaintKit() = 116;//Sand Mesh
							break;
						case 6:
							*pWeapon->FallbackPaintKit() = 157;//Palm
							break;
						case 7:
							*pWeapon->FallbackPaintKit() = 232;//Crimson Web
							break;
						case 8:
							*pWeapon->FallbackPaintKit() = 391;//Cardiac
							break;
						case 9:
							*pWeapon->FallbackPaintKit() = 298;//Army Sheen
							break;
						case 10:
							*pWeapon->FallbackPaintKit() = 312;//Cyrex
							break;
						case 11:
							*pWeapon->FallbackPaintKit() = 406;//Grotto
							break;
						case 12:
							*pWeapon->FallbackPaintKit() = 597;//Bloodsport
							break;
						default:
							break;
						}
					}
					break;
					case 39: // SG553
					{
						switch (SETTINGS::settings.Sg553Skin)
						{
						case 0:
							*pWeapon->FallbackPaintKit() = 0;//none
							break;
						case 1:
							*pWeapon->FallbackPaintKit() = 519;
							break;
						case 2:
							*pWeapon->FallbackPaintKit() = 487;
							break;
						case 3:
							*pWeapon->FallbackPaintKit() = 287;
							break;
						case 4:
							*pWeapon->FallbackPaintKit() = 586;
							break;
						default:
							break;
						}
					}
					break;
					case 40: // SSG08
					{
						switch (SETTINGS::settings.SSG08Skin)
						{
						case 0:
							*pWeapon->FallbackPaintKit() = 0;//none
							break;
						case 1:
							*pWeapon->FallbackPaintKit() = 26;//26 - Lichen Dashed
							break;
						case 2:
							*pWeapon->FallbackPaintKit() = 60;//60 - Dark Water
							break;
						case 3:
							*pWeapon->FallbackPaintKit() = 96;//96 - Blue Spruce
							break;
						case 4:
							*pWeapon->FallbackPaintKit() = 99;//99 - Sand Dune
							break;
						case 5:
							*pWeapon->FallbackPaintKit() = 157;//157 - Palm
							break;
						case 6:
							*pWeapon->FallbackPaintKit() = 200;//200 - Mayan Dreams
							break;
						case 7:
							*pWeapon->FallbackPaintKit() = 222;//222 - Blood in the Water
							break;
						case 8:
							*pWeapon->FallbackPaintKit() = 233;//233 - Tropical Storm
							break;
						case 9:
							*pWeapon->FallbackPaintKit() = 253;//253 - Acid Fade
							break;
						case 10:
							*pWeapon->FallbackPaintKit() = 304;//304 - Slashed
							break;
						case 11:
							*pWeapon->FallbackPaintKit() = 319;//319 - Detour
							break;
						case 12:
							*pWeapon->FallbackPaintKit() = 361;//361 - Abyss
							break;
						case 13:
							*pWeapon->FallbackPaintKit() = 503;//503: 'Big Iron'
							break;
						case 14:
							*pWeapon->FallbackPaintKit() = 538;//538: 'Necropos'
							break;
						case 15:
							*pWeapon->FallbackPaintKit() = 554;//554 - Ghost Crusader
							break;
						case 16:
							*pWeapon->FallbackPaintKit() = 624;//624: 'Dragonfire'
							break;
						default:
							break;
						}
					}
					break;
					case 64: // Revolver
					{
						switch (SETTINGS::settings.RevolverSkin)
						{
						case 0:
							*pWeapon->FallbackPaintKit() = 0;//none
							break;
						case 1:
							*pWeapon->FallbackPaintKit() = 683;
							break;
						case 2:
							*pWeapon->FallbackPaintKit() = 522;
							break;
						case 3:
							*pWeapon->FallbackPaintKit() = 12;
							break;
						case 4:
							*pWeapon->FallbackPaintKit() = 595;
							break;
						case 5:
							*pWeapon->FallbackPaintKit() = 798;
							break;
						case 6:
							*pWeapon->FallbackPaintKit() = 27;
							break;
						default:

							break;
						}
					}
					break;
					default:
						break;
					}

					if (pEntity->GetClientClass()->m_ClassID == (int)105)
					{
						auto pCustomName1 = MakePtr(char*, pWeapon, 0x301C);

						HANDLE worldmodel_handle2 = pWeapon->m_hWeaponWorldModel();
						if (worldmodel_handle2) worldmodel = (SDK::CBaseWeapon*)INTERFACES::ClientEntityList->GetClientEntityFromHandle(pWeapon->m_hWeaponWorldModel());
						if (Model == 0) // Bayonet
						{
							int iBayonet = INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
							*pWeapon->ModelIndex() = iBayonet; // m_nModelIndex
							*pWeapon->ViewModelIndex() = iBayonet;
							if (worldmodel) *worldmodel->ModelIndex() = iBayonet + 1;
							*pWeapon->fixskins() = 500;
							*pWeapon->GetEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "bayonet";
							killIcons["knife_t"] = "bayonet";
							int Skin = SETTINGS::settings.KnifeSkin;
							if (Skin == 0)
							{
								*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
							}
							else if (Skin == 1)
							{
								*pWeapon->FallbackPaintKit() = 12; // Crimson Web
							}
							else if (Skin == 2)
							{
								*pWeapon->FallbackPaintKit() = 27; // Bone Mask
							}
							else if (Skin == 3)
							{
								*pWeapon->FallbackPaintKit() = 38; // Fade
							}
							else if (Skin == 4)
							{
								*pWeapon->FallbackPaintKit() = 40; // Night
							}
							else if (Skin == 5)
							{
								*pWeapon->FallbackPaintKit() = 42; // Blue Steel
							}
							else if (Skin == 6)
							{
								*pWeapon->FallbackPaintKit() = 43; // Stained
							}
							else if (Skin == 7)
							{
								*pWeapon->FallbackPaintKit() = 44; // Case Hardened
							}
							else if (Skin == 8)
							{
								*pWeapon->FallbackPaintKit() = 59; // Slaughter
							}
							else if (Skin == 9)
							{
								*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
							}
							else if (Skin == 10)
							{
								*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
							}
							else if (Skin == 11)
							{
								*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
							}
							else if (Skin == 12)
							{
								*pWeapon->FallbackPaintKit() = 143; // Urban Masked
							}
							else if (Skin == 13)
							{
								*pWeapon->FallbackPaintKit() = 175; // Scorched
							}
							else if (Skin == 14)
							{
								*pWeapon->FallbackPaintKit() = 323; // Rust Coat
							}
							else if (Skin == 15)
							{
								*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
							}
							else if (Skin == 16)
							{
								*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
							}
							else if (Skin == 17)
							{
								*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
							}
							else if (Skin == 18)
							{
								*pWeapon->FallbackPaintKit() = 413; // Marble Fade
							}
							else if (Skin == 19)
							{
								*pWeapon->FallbackPaintKit() = 414; // Rust Coat
							}
							else if (Skin == 20)
							{
								*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
							}
							else if (Skin == 21)
							{
								*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
							}
							else if (Skin == 22)
							{
								*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
							}
							else if (Skin == 23)
							{
								*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
							}
							else if (Skin == 24)
							{
								*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
							}
							else if (Skin == 25)
							{
								*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
							}
							else if (Skin == 26)
							{
								*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
							}
							else if (Skin == 27)
							{
								*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
							}
							else if (Skin == 28)
							{
								*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
							}
							else if (Skin == 29)
							{
								*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
							}
							else if (Skin == 30)
							{
								*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
							}
							else if (Skin == 31)
							{
								*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
							}
							else if (Skin == 32)
							{
								*pWeapon->FallbackPaintKit() = 558; // Lore
							}
							else if (Skin == 33)
							{
								*pWeapon->FallbackPaintKit() = 563; // Black Laminate
							}
							else if (Skin == 34)
							{
								*pWeapon->FallbackPaintKit() = 573; // Autotronic
							}
							else if (Skin == 35)
							{
								*pWeapon->FallbackPaintKit() = 580; // Freehand
							}
						}
						else if (Model == 9) // Bowie Knife
						{
							int iBowie = INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");
							*pWeapon->ModelIndex() = iBowie; // m_nModelIndex
							*pWeapon->ViewModelIndex() = iBowie;
							if (worldmodel) *worldmodel->ModelIndex() = iBowie + 1;
							*pWeapon->fixskins() = 514;
							*pWeapon->GetEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "knife_survival_bowie";
							killIcons["knife_t"] = "knife_survival_bowie";
							int Skin = SETTINGS::settings.KnifeSkin;
							if (Skin == 0)
							{
								*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
							}
							else if (Skin == 1)
							{
								*pWeapon->FallbackPaintKit() = 12; // Crimson Web
							}
							else if (Skin == 2)
							{
								*pWeapon->FallbackPaintKit() = 27; // Bone Mask
							}
							else if (Skin == 3)
							{
								*pWeapon->FallbackPaintKit() = 38; // Fade
							}
							else if (Skin == 4)
							{
								*pWeapon->FallbackPaintKit() = 40; // Night
							}
							else if (Skin == 5)
							{
								*pWeapon->FallbackPaintKit() = 42; // Blue Steel
							}
							else if (Skin == 6)
							{
								*pWeapon->FallbackPaintKit() = 43; // Stained
							}
							else if (Skin == 7)
							{
								*pWeapon->FallbackPaintKit() = 44; // Case Hardened
							}
							else if (Skin == 8)
							{
								*pWeapon->FallbackPaintKit() = 59; // Slaughter
							}
							else if (Skin == 9)
							{
								*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
							}
							else if (Skin == 10)
							{
								*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
							}
							else if (Skin == 11)
							{
								*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
							}
							else if (Skin == 12)
							{
								*pWeapon->FallbackPaintKit() = 143; // Urban Masked
							}
							else if (Skin == 13)
							{
								*pWeapon->FallbackPaintKit() = 175; // Scorched
							}
							else if (Skin == 14)
							{
								*pWeapon->FallbackPaintKit() = 323; // Rust Coat
							}
							else if (Skin == 15)
							{
								*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
							}
							else if (Skin == 16)
							{
								*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
							}
							else if (Skin == 17)
							{
								*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
							}
							else if (Skin == 18)
							{
								*pWeapon->FallbackPaintKit() = 413; // Marble Fade
							}
							else if (Skin == 19)
							{
								*pWeapon->FallbackPaintKit() = 414; // Rust Coat
							}
							else if (Skin == 20)
							{
								*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
							}
							else if (Skin == 21)
							{
								*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
							}
							else if (Skin == 22)
							{
								*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
							}
							else if (Skin == 23)
							{
								*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
							}
							else if (Skin == 24)
							{
								*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
							}
							else if (Skin == 25)
							{
								*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
							}
							else if (Skin == 26)
							{
								*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
							}
							else if (Skin == 27)
							{
								*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
							}
							else if (Skin == 28)
							{
								*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
							}
							else if (Skin == 29)
							{
								*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
							}
							else if (Skin == 30)
							{
								*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
							}
							else if (Skin == 31)
							{
								*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
							}
							else if (Skin == 32)
							{
								*pWeapon->FallbackPaintKit() = 558; // Lore
							}
							else if (Skin == 33)
							{
								*pWeapon->FallbackPaintKit() = 567; // Black Laminate
							}
							else if (Skin == 34)
							{
								*pWeapon->FallbackPaintKit() = 574; // Autotronic
							}
							else if (Skin == 35)
							{
								*pWeapon->FallbackPaintKit() = 580; // Freehand
							}

						}
						else if (Model == 6) // Butterfly Knife
						{
							int iButterfly = INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
							*pWeapon->ModelIndex() = iButterfly; // m_nModelIndex
							*pWeapon->ViewModelIndex() = iButterfly;
							if (worldmodel) *worldmodel->ModelIndex() = iButterfly + 1;
							*pWeapon->fixskins() = 515;
							*pWeapon->GetEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "knife_butterfly";
							killIcons["knife_t"] = "knife_butterfly";
							int Skin = SETTINGS::settings.KnifeSkin;
							if (Skin == 0)
							{
								*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
							}
							else if (Skin == 1)
							{
								*pWeapon->FallbackPaintKit() = 12; // Crimson Web
							}
							else if (Skin == 2)
							{
								*pWeapon->FallbackPaintKit() = 27; // Bone Mask
							}
							else if (Skin == 3)
							{
								*pWeapon->FallbackPaintKit() = 38; // Fade
							}
							else if (Skin == 4)
							{
								*pWeapon->FallbackPaintKit() = 40; // Night
							}
							else if (Skin == 5)
							{
								*pWeapon->FallbackPaintKit() = 42; // Blue Steel
							}
							else if (Skin == 6)
							{
								*pWeapon->FallbackPaintKit() = 43; // Stained
							}
							else if (Skin == 7)
							{
								*pWeapon->FallbackPaintKit() = 44; // Case Hardened
							}
							else if (Skin == 8)
							{
								*pWeapon->FallbackPaintKit() = 59; // Slaughter
							}
							else if (Skin == 9)
							{
								*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
							}
							else if (Skin == 10)
							{
								*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
							}
							else if (Skin == 11)
							{
								*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
							}
							else if (Skin == 12)
							{
								*pWeapon->FallbackPaintKit() = 143; // Urban Masked
							}
							else if (Skin == 13)
							{
								*pWeapon->FallbackPaintKit() = 175; // Scorched
							}
							else if (Skin == 14)
							{
								*pWeapon->FallbackPaintKit() = 323; // Rust Coat
							}
							else if (Skin == 15)
							{
								*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
							}
							else if (Skin == 16)
							{
								*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
							}
							else if (Skin == 17)
							{
								*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
							}
							else if (Skin == 18)
							{
								*pWeapon->FallbackPaintKit() = 413; // Marble Fade
							}
							else if (Skin == 19)
							{
								*pWeapon->FallbackPaintKit() = 414; // Rust Coat
							}
							else if (Skin == 20)
							{
								*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
							}
							else if (Skin == 21)
							{
								*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
							}
							else if (Skin == 22)
							{
								*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
							}
							else if (Skin == 23)
							{
								*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
							}
							else if (Skin == 24)
							{
								*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
							}
							else if (Skin == 25)
							{
								*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
							}
							else if (Skin == 26)
							{
								*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
							}
							else if (Skin == 27)
							{
								*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
							}
							else if (Skin == 28)
							{
								*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
							}
							else if (Skin == 29)
							{
								*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
							}
							else if (Skin == 30)
							{
								*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
							}
							else if (Skin == 31)
							{
								*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
							}
							else if (Skin == 32)
							{
								*pWeapon->FallbackPaintKit() = 558; // Lore
							}
							else if (Skin == 33)
							{
								*pWeapon->FallbackPaintKit() = 567; // Black Laminate
							}
							else if (Skin == 34)
							{
								*pWeapon->FallbackPaintKit() = 574; // Autotronic
							}
							else if (Skin == 35)
							{
								*pWeapon->FallbackPaintKit() = 580; // Freehand
							}
						}
						else if (Model == 7) // Falchion Knife
						{
							int iFalchion = INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
							*pWeapon->ModelIndex() = iFalchion; // m_nModelIndex
							*pWeapon->ViewModelIndex() = iFalchion;
							if (worldmodel) *worldmodel->ModelIndex() = iFalchion + 1;
							*pWeapon->fixskins() = 512;
							*pWeapon->GetEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "knife_falchion";
							killIcons["knife_t"] = "knife_falchion";
							int Skin = SETTINGS::settings.KnifeSkin;
							if (Skin == 0)
							{
								*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
							}
							else if (Skin == 1)
							{
								*pWeapon->FallbackPaintKit() = 12; // Crimson Web
							}
							else if (Skin == 2)
							{
								*pWeapon->FallbackPaintKit() = 27; // Bone Mask
							}
							else if (Skin == 3)
							{
								*pWeapon->FallbackPaintKit() = 38; // Fade
							}
							else if (Skin == 4)
							{
								*pWeapon->FallbackPaintKit() = 40; // Night
							}
							else if (Skin == 5)
							{
								*pWeapon->FallbackPaintKit() = 42; // Blue Steel
							}
							else if (Skin == 6)
							{
								*pWeapon->FallbackPaintKit() = 43; // Stained
							}
							else if (Skin == 7)
							{
								*pWeapon->FallbackPaintKit() = 44; // Case Hardened
							}
							else if (Skin == 8)
							{
								*pWeapon->FallbackPaintKit() = 59; // Slaughter
							}
							else if (Skin == 9)
							{
								*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
							}
							else if (Skin == 10)
							{
								*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
							}
							else if (Skin == 11)
							{
								*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
							}
							else if (Skin == 12)
							{
								*pWeapon->FallbackPaintKit() = 143; // Urban Masked
							}
							else if (Skin == 13)
							{
								*pWeapon->FallbackPaintKit() = 175; // Scorched
							}
							else if (Skin == 14)
							{
								*pWeapon->FallbackPaintKit() = 323; // Rust Coat
							}
							else if (Skin == 15)
							{
								*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
							}
							else if (Skin == 16)
							{
								*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
							}
							else if (Skin == 17)
							{
								*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
							}
							else if (Skin == 18)
							{
								*pWeapon->FallbackPaintKit() = 413; // Marble Fade
							}
							else if (Skin == 19)
							{
								*pWeapon->FallbackPaintKit() = 414; // Rust Coat
							}
							else if (Skin == 20)
							{
								*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
							}
							else if (Skin == 21)
							{
								*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
							}
							else if (Skin == 22)
							{
								*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
							}
							else if (Skin == 23)
							{
								*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
							}
							else if (Skin == 24)
							{
								*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
							}
							else if (Skin == 25)
							{
								*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
							}
							else if (Skin == 26)
							{
								*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
							}
							else if (Skin == 27)
							{
								*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
							}
							else if (Skin == 28)
							{
								*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
							}
							else if (Skin == 29)
							{
								*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
							}
							else if (Skin == 30)
							{
								*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
							}
							else if (Skin == 31)
							{
								*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
							}
							else if (Skin == 32)
							{
								*pWeapon->FallbackPaintKit() = 558; // Lore
							}
							else if (Skin == 33)
							{
								*pWeapon->FallbackPaintKit() = 567; // Black Laminate
							}
							else if (Skin == 34)
							{
								*pWeapon->FallbackPaintKit() = 574; // Autotronic
							}
							else if (Skin == 35)
							{
								*pWeapon->FallbackPaintKit() = 580; // Freehand
							}
						}
						else if (Model == 1) // Flip Knife
						{
							int iFlip = INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
							*pWeapon->ModelIndex() = iFlip; // m_nModelIndex
							*pWeapon->ViewModelIndex() = iFlip;
							if (worldmodel) *worldmodel->ModelIndex() = iFlip + 1;
							*pWeapon->fixskins() = 505;
							*pWeapon->GetEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "knife_flip";
							killIcons["knife_t"] = "knife_flip";
							int Skin = SETTINGS::settings.KnifeSkin;
							if (Skin == 0)
							{
								*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
							}
							else if (Skin == 1)
							{
								*pWeapon->FallbackPaintKit() = 12; // Crimson Web
							}
							else if (Skin == 2)
							{
								*pWeapon->FallbackPaintKit() = 27; // Bone Mask
							}
							else if (Skin == 3)
							{
								*pWeapon->FallbackPaintKit() = 38; // Fade
							}
							else if (Skin == 4)
							{
								*pWeapon->FallbackPaintKit() = 40; // Night
							}
							else if (Skin == 5)
							{
								*pWeapon->FallbackPaintKit() = 42; // Blue Steel
							}
							else if (Skin == 6)
							{
								*pWeapon->FallbackPaintKit() = 43; // Stained
							}
							else if (Skin == 7)
							{
								*pWeapon->FallbackPaintKit() = 44; // Case Hardened
							}
							else if (Skin == 8)
							{
								*pWeapon->FallbackPaintKit() = 59; // Slaughter
							}
							else if (Skin == 9)
							{
								*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
							}
							else if (Skin == 10)
							{
								*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
							}
							else if (Skin == 11)
							{
								*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
							}
							else if (Skin == 12)
							{
								*pWeapon->FallbackPaintKit() = 143; // Urban Masked
							}
							else if (Skin == 13)
							{
								*pWeapon->FallbackPaintKit() = 175; // Scorched
							}
							else if (Skin == 14)
							{
								*pWeapon->FallbackPaintKit() = 323; // Rust Coat
							}
							else if (Skin == 15)
							{
								*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
							}
							else if (Skin == 16)
							{
								*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
							}
							else if (Skin == 17)
							{
								*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
							}
							else if (Skin == 18)
							{
								*pWeapon->FallbackPaintKit() = 413; // Marble Fade
							}
							else if (Skin == 19)
							{
								*pWeapon->FallbackPaintKit() = 414; // Rust Coat
							}
							else if (Skin == 20)
							{
								*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
							}
							else if (Skin == 21)
							{
								*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
							}
							else if (Skin == 22)
							{
								*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
							}
							else if (Skin == 23)
							{
								*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
							}
							else if (Skin == 24)
							{
								*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
							}
							else if (Skin == 25)
							{
								*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
							}
							else if (Skin == 26)
							{
								*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
							}
							else if (Skin == 27)
							{
								*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
							}
							else if (Skin == 28)
							{
								*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
							}
							else if (Skin == 29)
							{
								*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
							}
							else if (Skin == 30)
							{
								*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
							}
							else if (Skin == 31)
							{
								*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
							}
							else if (Skin == 32)
							{
								*pWeapon->FallbackPaintKit() = 559; // Lore
							}
							else if (Skin == 33)
							{
								*pWeapon->FallbackPaintKit() = 564; // Black Laminate
							}
							else if (Skin == 34)
							{
								*pWeapon->FallbackPaintKit() = 574; // Autotronic
							}
							else if (Skin == 35)
							{
								*pWeapon->FallbackPaintKit() = 580; // Freehand
							}

						}
						else if (Model == 2) // Gut Knife
						{
							int iGut = INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
							*pWeapon->ModelIndex() = iGut; // m_nModelIndex
							*pWeapon->ViewModelIndex() = iGut;
							if (worldmodel) *worldmodel->ModelIndex() = iGut + 1;
							*pWeapon->fixskins() = 506;
							*pWeapon->GetEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "knife_gut";
							killIcons["knife_t"] = "knife_gut";
							int Skin = SETTINGS::settings.KnifeSkin;
							if (Skin == 0)
							{
								*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
							}
							else if (Skin == 1)
							{
								*pWeapon->FallbackPaintKit() = 12; // Crimson Web
							}
							else if (Skin == 2)
							{
								*pWeapon->FallbackPaintKit() = 27; // Bone Mask
							}
							else if (Skin == 3)
							{
								*pWeapon->FallbackPaintKit() = 38; // Fade
							}
							else if (Skin == 4)
							{
								*pWeapon->FallbackPaintKit() = 40; // Night
							}
							else if (Skin == 5)
							{
								*pWeapon->FallbackPaintKit() = 42; // Blue Steel
							}
							else if (Skin == 6)
							{
								*pWeapon->FallbackPaintKit() = 43; // Stained
							}
							else if (Skin == 7)
							{
								*pWeapon->FallbackPaintKit() = 44; // Case Hardened
							}
							else if (Skin == 8)
							{
								*pWeapon->FallbackPaintKit() = 59; // Slaughter
							}
							else if (Skin == 9)
							{
								*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
							}
							else if (Skin == 10)
							{
								*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
							}
							else if (Skin == 11)
							{
								*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
							}
							else if (Skin == 12)
							{
								*pWeapon->FallbackPaintKit() = 143; // Urban Masked
							}
							else if (Skin == 13)
							{
								*pWeapon->FallbackPaintKit() = 175; // Scorched
							}
							else if (Skin == 14)
							{
								*pWeapon->FallbackPaintKit() = 323; // Rust Coat
							}
							else if (Skin == 15)
							{
								*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
							}
							else if (Skin == 16)
							{
								*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
							}
							else if (Skin == 17)
							{
								*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
							}
							else if (Skin == 18)
							{
								*pWeapon->FallbackPaintKit() = 413; // Marble Fade
							}
							else if (Skin == 19)
							{
								*pWeapon->FallbackPaintKit() = 414; // Rust Coat
							}
							else if (Skin == 20)
							{
								*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
							}
							else if (Skin == 21)
							{
								*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
							}
							else if (Skin == 22)
							{
								*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
							}
							else if (Skin == 23)
							{
								*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
							}
							else if (Skin == 24)
							{
								*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
							}
							else if (Skin == 25)
							{
								*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
							}
							else if (Skin == 26)
							{
								*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
							}
							else if (Skin == 27)
							{
								*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
							}
							else if (Skin == 28)
							{
								*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
							}
							else if (Skin == 29)
							{
								*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
							}
							else if (Skin == 30)
							{
								*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
							}
							else if (Skin == 31)
							{
								*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
							}
							else if (Skin == 32)
							{
								*pWeapon->FallbackPaintKit() = 560; // Lore
							}
							else if (Skin == 33)
							{
								*pWeapon->FallbackPaintKit() = 565; // Black Laminate
							}
							else if (Skin == 34)
							{
								*pWeapon->FallbackPaintKit() = 575; // Autotronic
							}
							else if (Skin == 35)
							{
								*pWeapon->FallbackPaintKit() = 580; // Freehand
							}
						}
						else if (Model == 5) // Huntsman Knife
						{
							int iHuntsman = INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
							*pWeapon->ModelIndex() = iHuntsman; // m_nModelIndex
							*pWeapon->ViewModelIndex() = iHuntsman;
							if (worldmodel) *worldmodel->ModelIndex() = iHuntsman + 1;
							*pWeapon->fixskins() = 509;
							*pWeapon->GetEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "knife_tactical";
							killIcons["knife_t"] = "knife_tactical";
							int Skin = SETTINGS::settings.KnifeSkin;
							if (Skin == 0)
							{
								*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
							}
							else if (Skin == 1)
							{
								*pWeapon->FallbackPaintKit() = 12; // Crimson Web
							}
							else if (Skin == 2)
							{
								*pWeapon->FallbackPaintKit() = 27; // Bone Mask
							}
							else if (Skin == 3)
							{
								*pWeapon->FallbackPaintKit() = 38; // Fade
							}
							else if (Skin == 4)
							{
								*pWeapon->FallbackPaintKit() = 40; // Night
							}
							else if (Skin == 5)
							{
								*pWeapon->FallbackPaintKit() = 42; // Blue Steel
							}
							else if (Skin == 6)
							{
								*pWeapon->FallbackPaintKit() = 43; // Stained
							}
							else if (Skin == 7)
							{
								*pWeapon->FallbackPaintKit() = 44; // Case Hardened
							}
							else if (Skin == 8)
							{
								*pWeapon->FallbackPaintKit() = 59; // Slaughter
							}
							else if (Skin == 9)
							{
								*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
							}
							else if (Skin == 10)
							{
								*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
							}
							else if (Skin == 11)
							{
								*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
							}
							else if (Skin == 12)
							{
								*pWeapon->FallbackPaintKit() = 143; // Urban Masked
							}
							else if (Skin == 13)
							{
								*pWeapon->FallbackPaintKit() = 175; // Scorched
							}
							else if (Skin == 14)
							{
								*pWeapon->FallbackPaintKit() = 323; // Rust Coat
							}
							else if (Skin == 15)
							{
								*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
							}
							else if (Skin == 16)
							{
								*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
							}
							else if (Skin == 17)
							{
								*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
							}
							else if (Skin == 18)
							{
								*pWeapon->FallbackPaintKit() = 413; // Marble Fade
							}
							else if (Skin == 19)
							{
								*pWeapon->FallbackPaintKit() = 414; // Rust Coat
							}
							else if (Skin == 20)
							{
								*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
							}
							else if (Skin == 21)
							{
								*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
							}
							else if (Skin == 22)
							{
								*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
							}
							else if (Skin == 23)
							{
								*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
							}
							else if (Skin == 24)
							{
								*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
							}
							else if (Skin == 25)
							{
								*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
							}
							else if (Skin == 26)
							{
								*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
							}
							else if (Skin == 27)
							{
								*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
							}
							else if (Skin == 28)
							{
								*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
							}
							else if (Skin == 29)
							{
								*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
							}
							else if (Skin == 30)
							{
								*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
							}
							else if (Skin == 31)
							{
								*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
							}
							else if (Skin == 32)
							{
								*pWeapon->FallbackPaintKit() = 559; // Lore
							}
							else if (Skin == 33)
							{
								*pWeapon->FallbackPaintKit() = 567; // Black Laminate
							}
							else if (Skin == 34)
							{
								*pWeapon->FallbackPaintKit() = 574; // Autotronic
							}
							else if (Skin == 35)
							{
								*pWeapon->FallbackPaintKit() = 580; // Freehand
							}
						}
						else if (Model == 3) // Karambit
						{
							int iKarambit = INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
							*pWeapon->ModelIndex() = iKarambit; // m_nModelIndex
							*pWeapon->ViewModelIndex() = iKarambit;
							if (worldmodel) *worldmodel->ModelIndex() = iKarambit + 1;
							*pWeapon->fixskins() = 507;
							*pWeapon->GetEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "knife_karambit";
							killIcons["knife_t"] = "knife_karambit";
							int Skin = SETTINGS::settings.KnifeSkin;
							if (Skin == 0)
							{
								*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
							}
							else if (Skin == 1)
							{
								*pWeapon->FallbackPaintKit() = 12; // Crimson Web
							}
							else if (Skin == 2)
							{
								*pWeapon->FallbackPaintKit() = 27; // Bone Mask
							}
							else if (Skin == 3)
							{
								*pWeapon->FallbackPaintKit() = 38; // Fade
							}
							else if (Skin == 4)
							{
								*pWeapon->FallbackPaintKit() = 40; // Night
							}
							else if (Skin == 5)
							{
								*pWeapon->FallbackPaintKit() = 42; // Blue Steel
							}
							else if (Skin == 6)
							{
								*pWeapon->FallbackPaintKit() = 43; // Stained
							}
							else if (Skin == 7)
							{
								*pWeapon->FallbackPaintKit() = 44; // Case Hardened
							}
							else if (Skin == 8)
							{
								*pWeapon->FallbackPaintKit() = 59; // Slaughter
							}
							else if (Skin == 9)
							{
								*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
							}
							else if (Skin == 10)
							{
								*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
							}
							else if (Skin == 11)
							{
								*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
							}
							else if (Skin == 12)
							{
								*pWeapon->FallbackPaintKit() = 143; // Urban Masked
							}
							else if (Skin == 13)
							{
								*pWeapon->FallbackPaintKit() = 175; // Scorched
							}
							else if (Skin == 14)
							{
								*pWeapon->FallbackPaintKit() = 323; // Rust Coat
							}
							else if (Skin == 15)
							{
								*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
							}
							else if (Skin == 16)
							{
								*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
							}
							else if (Skin == 17)
							{
								*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
							}
							else if (Skin == 18)
							{
								*pWeapon->FallbackPaintKit() = 413; // Marble Fade
							}
							else if (Skin == 19)
							{
								*pWeapon->FallbackPaintKit() = 414; // Rust Coat
							}
							else if (Skin == 20)
							{
								*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
							}
							else if (Skin == 21)
							{
								*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
							}
							else if (Skin == 22)
							{
								*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
							}
							else if (Skin == 23)
							{
								*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
							}
							else if (Skin == 24)
							{
								*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
							}
							else if (Skin == 25)
							{
								*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
							}
							else if (Skin == 26)
							{
								*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
							}
							else if (Skin == 27)
							{
								*pWeapon->FallbackPaintKit() = 570; // Doppler Phase 4
							}
							else if (Skin == 28)
							{
								*pWeapon->FallbackPaintKit() = 568; // Doppler Phase 4
							}
							else if (Skin == 27)
							{
								*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
							}
							else if (Skin == 28)
							{
								*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
							}
							else if (Skin == 29)
							{
								*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
							}
							else if (Skin == 30)
							{
								*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
							}
							else if (Skin == 31)
							{
								*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
							}
							else if (Skin == 32)
							{
								*pWeapon->FallbackPaintKit() = 561; // Lore
							}
							else if (Skin == 33)
							{
								*pWeapon->FallbackPaintKit() = 566; // Black Laminate
							}
							else if (Skin == 34)
							{
								*pWeapon->FallbackPaintKit() = 576; // Autotronic
							}
							else if (Skin == 35)
							{
								*pWeapon->FallbackPaintKit() = 582; // Freehand
							}

						}
						else if (Model == 4) // M9 Bayonet
						{
							auto iM9Bayonet = INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
							*pWeapon->ModelIndex() = iM9Bayonet; // m_nModelIndex
							*pWeapon->ViewModelIndex() = iM9Bayonet;
							if (worldmodel) *worldmodel->ModelIndex() = iM9Bayonet + 1;
							*pWeapon->fixskins() = 508;
							*pWeapon->GetEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "knife_m9_bayonet";
							killIcons["knife_t"] = "knife_m9_bayonet";
							int Skin = SETTINGS::settings.KnifeSkin;
							if (Skin == 0)
							{
								*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
							}
							else if (Skin == 1)
							{
								*pWeapon->FallbackPaintKit() = 12; // Crimson Web
							}
							else if (Skin == 2)
							{
								*pWeapon->FallbackPaintKit() = 27; // Bone Mask
							}
							else if (Skin == 3)
							{
								*pWeapon->FallbackPaintKit() = 38; // Fade
							}
							else if (Skin == 4)
							{
								*pWeapon->FallbackPaintKit() = 40; // Night
							}
							else if (Skin == 5)
							{
								*pWeapon->FallbackPaintKit() = 42; // Blue Steel
							}
							else if (Skin == 6)
							{
								*pWeapon->FallbackPaintKit() = 43; // Stained
							}
							else if (Skin == 7)
							{
								*pWeapon->FallbackPaintKit() = 44; // Case Hardened
							}
							else if (Skin == 8)
							{
								*pWeapon->FallbackPaintKit() = 59; // Slaughter
							}
							else if (Skin == 9)
							{
								*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
							}
							else if (Skin == 10)
							{
								*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
							}
							else if (Skin == 11)
							{
								*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
							}
							else if (Skin == 12)
							{
								*pWeapon->FallbackPaintKit() = 143; // Urban Masked
							}
							else if (Skin == 13)
							{
								*pWeapon->FallbackPaintKit() = 175; // Scorched
							}
							else if (Skin == 14)
							{
								*pWeapon->FallbackPaintKit() = 323; // Rust Coat
							}
							else if (Skin == 15)
							{
								*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
							}
							else if (Skin == 16)
							{
								*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
							}
							else if (Skin == 17)
							{
								*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
							}
							else if (Skin == 18)
							{
								*pWeapon->FallbackPaintKit() = 413; // Marble Fade
							}
							else if (Skin == 19)
							{
								*pWeapon->FallbackPaintKit() = 414; // Rust Coat
							}
							else if (Skin == 20)
							{
								*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
							}
							else if (Skin == 21)
							{
								*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
							}
							else if (Skin == 22)
							{
								*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
							}
							else if (Skin == 23)
							{
								*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
							}
							else if (Skin == 24)
							{
								*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
							}
							else if (Skin == 25)
							{
								*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
							}
							else if (Skin == 26)
							{
								*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
							}
							else if (Skin == 27)
							{
								*pWeapon->FallbackPaintKit() = 570; // Doppler Phase 4
							}
							else if (Skin == 28)
							{
								*pWeapon->FallbackPaintKit() = 568; // Doppler Phase 4
							}
							else if (Skin == 27)
							{
								*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
							}
							else if (Skin == 28)
							{
								*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
							}
							else if (Skin == 29)
							{
								*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
							}
							else if (Skin == 30)
							{
								*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
							}
							else if (Skin == 31)
							{
								*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
							}
							else if (Skin == 32)
							{
								*pWeapon->FallbackPaintKit() = 562; // Lore
							}
							else if (Skin == 33)
							{
								*pWeapon->FallbackPaintKit() = 567; // Black Laminate
							}
							else if (Skin == 34)
							{
								*pWeapon->FallbackPaintKit() = 577; // Autotronic
							}
							else if (Skin == 35)
							{
								*pWeapon->FallbackPaintKit() = 581; // Freehand
							}

						}
						else if (Model == 8)
						{
							int iDagger = INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
							*pWeapon->ModelIndex() = iDagger; // m_nModelIndex
							*pWeapon->ViewModelIndex() = iDagger;
							if (worldmodel) *worldmodel->ModelIndex() = iDagger + 1;
							*pWeapon->fixskins() = 516;
							*pWeapon->GetEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "knife_push";
							killIcons["knife_t"] = "knife_push";
							int Skin = SETTINGS::settings.KnifeSkin;
							if (Skin == 0)
							{
								*pWeapon->FallbackPaintKit() = 5; // Forest DDPAT
							}
							else if (Skin == 1)
							{
								*pWeapon->FallbackPaintKit() = 12; // Crimson Web
							}
							else if (Skin == 2)
							{
								*pWeapon->FallbackPaintKit() = 27; // Bone Mask
							}
							else if (Skin == 3)
							{
								*pWeapon->FallbackPaintKit() = 38; // Fade
							}
							else if (Skin == 4)
							{
								*pWeapon->FallbackPaintKit() = 40; // Night
							}
							else if (Skin == 5)
							{
								*pWeapon->FallbackPaintKit() = 42; // Blue Steel
							}
							else if (Skin == 6)
							{
								*pWeapon->FallbackPaintKit() = 43; // Stained
							}
							else if (Skin == 7)
							{
								*pWeapon->FallbackPaintKit() = 44; // Case Hardened
							}
							else if (Skin == 8)
							{
								*pWeapon->FallbackPaintKit() = 59; // Slaughter
							}
							else if (Skin == 9)
							{
								*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
							}
							else if (Skin == 10)
							{
								*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
							}
							else if (Skin == 11)
							{
								*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
							}
							else if (Skin == 12)
							{
								*pWeapon->FallbackPaintKit() = 143; // Urban Masked
							}
							else if (Skin == 13)
							{
								*pWeapon->FallbackPaintKit() = 175; // Scorched
							}
							else if (Skin == 14)
							{
								*pWeapon->FallbackPaintKit() = 323; // Rust Coat
							}
							else if (Skin == 15)
							{
								*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
							}
							else if (Skin == 16)
							{
								*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
							}
							else if (Skin == 17)
							{
								*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
							}
							else if (Skin == 18)
							{
								*pWeapon->FallbackPaintKit() = 413; // Marble Fade
							}
							else if (Skin == 19)
							{
								*pWeapon->FallbackPaintKit() = 414; // Rust Coat
							}
							else if (Skin == 20)
							{
								*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
							}
							else if (Skin == 21)
							{
								*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
							}
							else if (Skin == 22)
							{
								*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
							}
							else if (Skin == 23)
							{
								*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
							}
							else if (Skin == 24)
							{
								*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
							}
							else if (Skin == 25)
							{
								*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
							}
							else if (Skin == 26)
							{
								*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
							}
							else if (Skin == 27)
							{
								*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
							}
							else if (Skin == 28)
							{
								*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
							}
							else if (Skin == 29)
							{
								*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
							}
							else if (Skin == 30)
							{
								*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
							}
							else if (Skin == 31)
							{
								*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
							}
							else if (Skin == 32)
							{
								*pWeapon->FallbackPaintKit() = 561; // Lore
							}
							else if (Skin == 33)
							{
								*pWeapon->FallbackPaintKit() = 567; // Black Laminate
							}
							else if (Skin == 34)
							{
								*pWeapon->FallbackPaintKit() = 574; // Autotronic
							}
							else if (Skin == 35)
							{
								*pWeapon->FallbackPaintKit() = 580; // Freehand
							}
						}
						else if (Model == 10)
						{
							int Navaja = INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_gypsy_jackknife.mdl");
							*pWeapon->ModelIndex() = Navaja; // m_nModelIndex
							*pWeapon->ViewModelIndex() = Navaja;
							if (worldmodel) *worldmodel->ModelIndex() = Navaja + 1;
							*pWeapon->fixskins() = 520;
							*pWeapon->GetEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "knife_push";
							killIcons["knife_t"] = "knife_push";
							int Skin = SETTINGS::settings.KnifeSkin;
							if (Skin == 0)
							{
								*pWeapon->FallbackPaintKit() = 5; // Forest DDPAT
							}
							else if (Skin == 1)
							{
								*pWeapon->FallbackPaintKit() = 12; // Crimson Web
							}
							else if (Skin == 2)
							{
								*pWeapon->FallbackPaintKit() = 27; // Bone Mask
							}
							else if (Skin == 3)
							{
								*pWeapon->FallbackPaintKit() = 38; // Fade
							}
							else if (Skin == 4)
							{
								*pWeapon->FallbackPaintKit() = 40; // Night
							}
							else if (Skin == 5)
							{
								*pWeapon->FallbackPaintKit() = 42; // Blue Steel
							}
							else if (Skin == 6)
							{
								*pWeapon->FallbackPaintKit() = 43; // Stained
							}
							else if (Skin == 7)
							{
								*pWeapon->FallbackPaintKit() = 44; // Case Hardened
							}
							else if (Skin == 8)
							{
								*pWeapon->FallbackPaintKit() = 59; // Slaughter
							}
							else if (Skin == 9)
							{
								*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
							}
							else if (Skin == 10)
							{
								*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
							}
							else if (Skin == 11)
							{
								*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
							}
							else if (Skin == 12)
							{
								*pWeapon->FallbackPaintKit() = 143; // Urban Masked
							}
							else if (Skin == 13)
							{
								*pWeapon->FallbackPaintKit() = 175; // Scorched
							}
							else if (Skin == 14)
							{
								*pWeapon->FallbackPaintKit() = 323; // Rust Coat
							}
							else if (Skin == 15)
							{
								*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
							}
							else if (Skin == 16)
							{
								*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
							}
							else if (Skin == 17)
							{
								*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
							}
							else if (Skin == 18)
							{
								*pWeapon->FallbackPaintKit() = 413; // Marble Fade
							}
							else if (Skin == 19)
							{
								*pWeapon->FallbackPaintKit() = 414; // Rust Coat
							}
							else if (Skin == 20)
							{
								*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
							}
							else if (Skin == 21)
							{
								*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
							}
							else if (Skin == 22)
							{
								*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
							}
							else if (Skin == 23)
							{
								*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
							}
							else if (Skin == 24)
							{
								*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
							}
							else if (Skin == 25)
							{
								*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
							}
							else if (Skin == 26)
							{
								*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
							}
							else if (Skin == 27)
							{
								*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
							}
							else if (Skin == 28)
							{
								*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
							}
							else if (Skin == 29)
							{
								*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
							}
							else if (Skin == 30)
							{
								*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
							}
							else if (Skin == 31)
							{
								*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
							}
							else if (Skin == 32)
							{
								*pWeapon->FallbackPaintKit() = 561; // Lore
							}
							else if (Skin == 33)
							{
								*pWeapon->FallbackPaintKit() = 567; // Black Laminate
							}
							else if (Skin == 34)
							{
								*pWeapon->FallbackPaintKit() = 574; // Autotronic
							}
							else if (Skin == 35)
							{
								*pWeapon->FallbackPaintKit() = 580; // Freehand
							}
						}
						else if (Model == 11) // Shadow Daggers
						{
							int Stiletto = INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_stiletto.mdl");
							*pWeapon->ModelIndex() = Stiletto; // m_nModelIndex
							*pWeapon->ViewModelIndex() = Stiletto;
							if (worldmodel) *worldmodel->ModelIndex() = Stiletto + 1;
							*pWeapon->fixskins() = 522;
							*pWeapon->GetEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "knife_push";
							killIcons["knife_t"] = "knife_push";
							int Skin = SETTINGS::settings.KnifeSkin;
							if (Skin == 0)
							{
								*pWeapon->FallbackPaintKit() = 5; // Forest DDPAT
							}
							else if (Skin == 1)
							{
								*pWeapon->FallbackPaintKit() = 12; // Crimson Web
							}
							else if (Skin == 2)
							{
								*pWeapon->FallbackPaintKit() = 27; // Bone Mask
							}
							else if (Skin == 3)
							{
								*pWeapon->FallbackPaintKit() = 38; // Fade
							}
							else if (Skin == 4)
							{
								*pWeapon->FallbackPaintKit() = 40; // Night
							}
							else if (Skin == 5)
							{
								*pWeapon->FallbackPaintKit() = 42; // Blue Steel
							}
							else if (Skin == 6)
							{
								*pWeapon->FallbackPaintKit() = 43; // Stained
							}
							else if (Skin == 7)
							{
								*pWeapon->FallbackPaintKit() = 44; // Case Hardened
							}
							else if (Skin == 8)
							{
								*pWeapon->FallbackPaintKit() = 59; // Slaughter
							}
							else if (Skin == 9)
							{
								*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
							}
							else if (Skin == 10)
							{
								*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
							}
							else if (Skin == 11)
							{
								*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
							}
							else if (Skin == 12)
							{
								*pWeapon->FallbackPaintKit() = 143; // Urban Masked
							}
							else if (Skin == 13)
							{
								*pWeapon->FallbackPaintKit() = 175; // Scorched
							}
							else if (Skin == 14)
							{
								*pWeapon->FallbackPaintKit() = 323; // Rust Coat
							}
							else if (Skin == 15)
							{
								*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
							}
							else if (Skin == 16)
							{
								*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
							}
							else if (Skin == 17)
							{
								*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
							}
							else if (Skin == 18)
							{
								*pWeapon->FallbackPaintKit() = 413; // Marble Fade
							}
							else if (Skin == 19)
							{
								*pWeapon->FallbackPaintKit() = 414; // Rust Coat
							}
							else if (Skin == 20)
							{
								*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
							}
							else if (Skin == 21)
							{
								*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
							}
							else if (Skin == 22)
							{
								*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
							}
							else if (Skin == 23)
							{
								*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
							}
							else if (Skin == 24)
							{
								*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
							}
							else if (Skin == 25)
							{
								*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
							}
							else if (Skin == 26)
							{
								*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
							}
							else if (Skin == 27)
							{
								*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
							}
							else if (Skin == 28)
							{
								*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
							}
							else if (Skin == 29)
							{
								*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
							}
							else if (Skin == 30)
							{
								*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
							}
							else if (Skin == 31)
							{
								*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
							}
							else if (Skin == 32)
							{
								*pWeapon->FallbackPaintKit() = 561; // Lore
							}
							else if (Skin == 33)
							{
								*pWeapon->FallbackPaintKit() = 567; // Black Laminate
							}
							else if (Skin == 34)
							{
								*pWeapon->FallbackPaintKit() = 574; // Autotronic
							}
							else if (Skin == 35)
							{
								*pWeapon->FallbackPaintKit() = 580; // Freehand
							}
						}
						else if (Model == 12)
						{
							int Ursus = INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_ursus.mdl");
							*pWeapon->ModelIndex() = Ursus; // m_nModelIndex
							*pWeapon->ViewModelIndex() = Ursus;
							if (worldmodel) *worldmodel->ModelIndex() = Ursus + 1;
							*pWeapon->fixskins() = 519;
							*pWeapon->GetEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "knife_push";
							killIcons["knife_t"] = "knife_push";
							int Skin = SETTINGS::settings.KnifeSkin;
							if (Skin == 0)
							{
								*pWeapon->FallbackPaintKit() = 5; // Forest DDPAT
							}
							else if (Skin == 1)
							{
								*pWeapon->FallbackPaintKit() = 12; // Crimson Web
							}
							else if (Skin == 2)
							{
								*pWeapon->FallbackPaintKit() = 27; // Bone Mask
							}
							else if (Skin == 3)
							{
								*pWeapon->FallbackPaintKit() = 38; // Fade
							}
							else if (Skin == 4)
							{
								*pWeapon->FallbackPaintKit() = 40; // Night
							}
							else if (Skin == 5)
							{
								*pWeapon->FallbackPaintKit() = 42; // Blue Steel
							}
							else if (Skin == 6)
							{
								*pWeapon->FallbackPaintKit() = 43; // Stained
							}
							else if (Skin == 7)
							{
								*pWeapon->FallbackPaintKit() = 44; // Case Hardened
							}
							else if (Skin == 8)
							{
								*pWeapon->FallbackPaintKit() = 59; // Slaughter
							}
							else if (Skin == 9)
							{
								*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
							}
							else if (Skin == 10)
							{
								*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
							}
							else if (Skin == 11)
							{
								*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
							}
							else if (Skin == 12)
							{
								*pWeapon->FallbackPaintKit() = 143; // Urban Masked
							}
							else if (Skin == 13)
							{
								*pWeapon->FallbackPaintKit() = 175; // Scorched
							}
							else if (Skin == 14)
							{
								*pWeapon->FallbackPaintKit() = 323; // Rust Coat
							}
							else if (Skin == 15)
							{
								*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
							}
							else if (Skin == 16)
							{
								*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
							}
							else if (Skin == 17)
							{
								*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
							}
							else if (Skin == 18)
							{
								*pWeapon->FallbackPaintKit() = 413; // Marble Fade
							}
							else if (Skin == 19)
							{
								*pWeapon->FallbackPaintKit() = 414; // Rust Coat
							}
							else if (Skin == 20)
							{
								*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
							}
							else if (Skin == 21)
							{
								*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
							}
							else if (Skin == 22)
							{
								*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
							}
							else if (Skin == 23)
							{
								*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
							}
							else if (Skin == 24)
							{
								*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
							}
							else if (Skin == 25)
							{
								*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
							}
							else if (Skin == 26)
							{
								*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
							}
							else if (Skin == 27)
							{
								*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
							}
							else if (Skin == 28)
							{
								*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
							}
							else if (Skin == 29)
							{
								*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
							}
							else if (Skin == 30)
							{
								*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
							}
							else if (Skin == 31)
							{
								*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
							}
							else if (Skin == 32)
							{
								*pWeapon->FallbackPaintKit() = 561; // Lore
							}
							else if (Skin == 33)
							{
								*pWeapon->FallbackPaintKit() = 567; // Black Laminate
							}
							else if (Skin == 34)
							{
								*pWeapon->FallbackPaintKit() = 574; // Autotronic
							}
							else if (Skin == 35)
							{
								*pWeapon->FallbackPaintKit() = 580; // Freehand
							}
						}
						else if (Model == 13)
						{
							int Talon = INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_widowmaker.mdl");
							*pWeapon->ModelIndex() = Talon; // m_nModelIndex
							*pWeapon->ViewModelIndex() = Talon;
							if (worldmodel) *worldmodel->ModelIndex() = Talon + 1;
							*pWeapon->fixskins() = 523;
							*pWeapon->GetEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "knife_push";
							killIcons["knife_t"] = "knife_push";
							int Skin = SETTINGS::settings.KnifeSkin;
							if (Skin == 0)
							{
								*pWeapon->FallbackPaintKit() = 5; // Forest DDPAT
							}
							else if (Skin == 1)
							{
								*pWeapon->FallbackPaintKit() = 12; // Crimson Web
							}
							else if (Skin == 2)
							{
								*pWeapon->FallbackPaintKit() = 27; // Bone Mask
							}
							else if (Skin == 3)
							{
								*pWeapon->FallbackPaintKit() = 38; // Fade
							}
							else if (Skin == 4)
							{
								*pWeapon->FallbackPaintKit() = 40; // Night
							}
							else if (Skin == 5)
							{
								*pWeapon->FallbackPaintKit() = 42; // Blue Steel
							}
							else if (Skin == 6)
							{
								*pWeapon->FallbackPaintKit() = 43; // Stained
							}
							else if (Skin == 7)
							{
								*pWeapon->FallbackPaintKit() = 44; // Case Hardened
							}
							else if (Skin == 8)
							{
								*pWeapon->FallbackPaintKit() = 59; // Slaughter
							}
							else if (Skin == 9)
							{
								*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
							}
							else if (Skin == 10)
							{
								*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
							}
							else if (Skin == 11)
							{
								*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
							}
							else if (Skin == 12)
							{
								*pWeapon->FallbackPaintKit() = 143; // Urban Masked
							}
							else if (Skin == 13)
							{
								*pWeapon->FallbackPaintKit() = 175; // Scorched
							}
							else if (Skin == 14)
							{
								*pWeapon->FallbackPaintKit() = 323; // Rust Coat
							}
							else if (Skin == 15)
							{
								*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
							}
							else if (Skin == 16)
							{
								*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
							}
							else if (Skin == 17)
							{
								*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
							}
							else if (Skin == 18)
							{
								*pWeapon->FallbackPaintKit() = 413; // Marble Fade
							}
							else if (Skin == 19)
							{
								*pWeapon->FallbackPaintKit() = 414; // Rust Coat
							}
							else if (Skin == 20)
							{
								*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
							}
							else if (Skin == 21)
							{
								*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
							}
							else if (Skin == 22)
							{
								*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
							}
							else if (Skin == 23)
							{
								*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
							}
							else if (Skin == 24)
							{
								*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
							}
							else if (Skin == 25)
							{
								*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
							}
							else if (Skin == 26)
							{
								*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
							}
							else if (Skin == 27)
							{
								*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
							}
							else if (Skin == 28)
							{
								*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
							}
							else if (Skin == 29)
							{
								*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
							}
							else if (Skin == 30)
							{
								*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
							}
							else if (Skin == 31)
							{
								*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
							}
							else if (Skin == 32)
							{
								*pWeapon->FallbackPaintKit() = 561; // Lore
							}
							else if (Skin == 33)
							{
								*pWeapon->FallbackPaintKit() = 567; // Black Laminate
							}
							else if (Skin == 34)
							{
								*pWeapon->FallbackPaintKit() = 574; // Autotronic
							}
							else if (Skin == 35)
							{
								*pWeapon->FallbackPaintKit() = 580; // Freehand
							}
						}



					}

					
						*pWeapon->OwnerXuidLow() = 0;
						*pWeapon->OwnerXuidHigh() = 0;
						*pWeapon->FallbackWear() = 0.001;
						*pWeapon->ItemIDHigh() = 1;
					

				}
			}
		}
		
	}
}


void InventarNewSkinChanger()
{
	SDK::CBaseEntity *pLocal = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	auto weapons = pLocal->m_hMyWeapons();
	for (size_t i = 0; weapons[i] != INVALID_EHANDLE_INDEX; i++)
	{
		SDK::CBaseEntity *pEntity = INTERFACES::ClientEntityList->GetClientEntityFromHandle(weapons[i]);
		if (pEntity)
		{
			SDK::CBaseWeapon* pWeapon = (SDK::CBaseWeapon*)pEntity;
			{
				int Model = SETTINGS::settings.Knife;
				int weapon = *pWeapon->fixskins();
				switch (weapon)
				{
				case 7:
				{
					*pWeapon->FallbackPaintKit() = 656;
					break;
				}
				break;
				case 16: // M4A4
				{
					*pWeapon->FallbackPaintKit() = 632;
					break;
				}
				break;
				case 2: // dual
				{
					*pWeapon->FallbackPaintKit() = 544;
					break;
				}
				break;
				case 60: // M4A1
				{
					*pWeapon->FallbackPaintKit() = 383;
					break;
				}
				break;
				case 9: // AWP
				{
					*pWeapon->FallbackPaintKit() = 344;
					break;
				}
				break;
				case 61: // USP
				{
					*pWeapon->FallbackPaintKit() = 653;
					break;
				}
				break;
				case 4: // Glock
				{
					*pWeapon->FallbackPaintKit() = 586;
					break;
				}
				break;
				case 1: // Deagle
				{
					*pWeapon->FallbackPaintKit() = 527;
					break;
				}
				break;
				case 3: // Five Seven
				{
					*pWeapon->FallbackPaintKit() = 387;
					break;
				}
				break;
				case 8: // AUG
				{
					*pWeapon->FallbackPaintKit() = 305;
					break;
				}
				break;
				case 10: // Famas
				{
					*pWeapon->FallbackPaintKit() = 260;
					break;
				}
				break;
				case 11: // G3SG1
				{
					*pWeapon->FallbackPaintKit() = 628;
					break;
				}
				break;
				case 13: // Galil
				{
					*pWeapon->FallbackPaintKit() = 629;
					break;
				}
				break;
				case 14: // M249
				{
					*pWeapon->FallbackPaintKit() = 401;
					break;
				}
				break;
				case 17: // Mac 10
				{
					*pWeapon->FallbackPaintKit() = 433;
					break;
				}
				break;
				case 19: // P90
				{
					*pWeapon->FallbackPaintKit() = 283;
					break;
				}
				break;
				case 24: // UMP-45
				{
					*pWeapon->FallbackPaintKit() = 688;
					break;
				}
				break;
				case 25: // XM1014
				{
					*pWeapon->FallbackPaintKit() = 616;
					break;
				}
				break;
				case 63: // CZ75-Auto
				{
					*pWeapon->FallbackPaintKit() = 270;
					break;
				}
				break;
				case 26: // Bizon
				{
					*pWeapon->FallbackPaintKit() = 306;
					break;
				}
				break;
				case 27: // Mag 7
				{
					*pWeapon->FallbackPaintKit() = 198;
					break;
				}
				break;
				case 28: // Negev
				{
					*pWeapon->FallbackPaintKit() = 483;
					break;
				}
				break;
				case 29: // Sawed Off
				{
					*pWeapon->FallbackPaintKit() = 434;
					break;
				}
				break;
				case 30: // Tec 9
				{
					*pWeapon->FallbackPaintKit() = 652;
					break;
				}
				break;
				case 32: // P2000
				{
					*pWeapon->FallbackPaintKit() = 338;
					break;
				}
				break;
				case 33: // MP7
				{
					*pWeapon->FallbackPaintKit() = 481;
					break;
				}
				break;
				case 34: // MP9
				{
					*pWeapon->FallbackPaintKit() = 262;
					break;
				}
				break;
				case 35: // Nova
				{
					*pWeapon->FallbackPaintKit() = 450;
					break;
				}
				break;
				case 36: // P250
				{
					*pWeapon->FallbackPaintKit() = 404;
					break;
				}
				break;
				case 38: // Scar 20
				{
					*pWeapon->FallbackPaintKit() = 232;
					break;
				}
				break;
				case 39: // SG553
				{
					*pWeapon->FallbackPaintKit() = 311;
					break;
				}
				break;
				case 40: // SSG08
				{
					*pWeapon->FallbackPaintKit() = 538;
					break;
				}
				break;
				case 64: // Revolver
				{
					*pWeapon->FallbackPaintKit() = 538;
					break;
				}
				break;
				default:
					break;
				}
				if (pEntity->GetClientClass()->m_ClassID == (int)ClassID::CKnife)
				{
					auto pCustomName1 = MakePtr(char*, pWeapon, 0x301C);
					worldmodel_handle = pWeapon->m_hWeaponWorldModel();
					if (worldmodel_handle) worldmodel = (SDK::CBaseWeapon*)INTERFACES::ClientEntityList->GetClientEntityFromHandleknife(worldmodel_handle);
					Model == 8;
					if (Model == 8) // M9 Bayonet
					{
						int iM9Bayonet = INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
						*pWeapon->ModelIndex() = iM9Bayonet; // m_nModelIndex
						*pWeapon->ViewModelIndex() = iM9Bayonet;
						if (worldmodel) *worldmodel->ModelIndex() = iM9Bayonet + 1;
						*pWeapon->fixskins() = 508;
						*pWeapon->GetEntityQuality() = 3;
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_m9_bayonet";
						killIcons["knife_t"] = "knife_m9_bayonet";
						int Skin = SETTINGS::settings.KnifeSkin;
						*pWeapon->FallbackPaintKit() = 622;

					}
					else if (Model == 9) // Shadow Daggers
					{
						int iDagger = INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
						*pWeapon->ModelIndex() = iDagger; // m_nModelIndex
						*pWeapon->ViewModelIndex() = iDagger;
						if (worldmodel) *worldmodel->ModelIndex() = iDagger + 1;
						*pWeapon->fixskins() = 516;
						*pWeapon->GetEntityQuality() = 3;
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_push";
						killIcons["knife_t"] = "knife_push";
						int Skin = SETTINGS::settings.KnifeSkin;
						if (Skin == 0)
						{
							*pWeapon->FallbackPaintKit() = 5; // Forest DDPAT
						}
						else if (Skin == 1)
						{
							*pWeapon->FallbackPaintKit() = 12; // Crimson Web
						}
						else if (Skin == 2)
						{
							*pWeapon->FallbackPaintKit() = 27; // Bone Mask
						}
						else if (Skin == 3)
						{
							*pWeapon->FallbackPaintKit() = 38; // Fade
						}
						else if (Skin == 4)
						{
							*pWeapon->FallbackPaintKit() = 40; // Night
						}
						else if (Skin == 5)
						{
							*pWeapon->FallbackPaintKit() = 42; // Blue Steel
						}
						else if (Skin == 6)
						{
							*pWeapon->FallbackPaintKit() = 43; // Stained
						}
						else if (Skin == 7)
						{
							*pWeapon->FallbackPaintKit() = 44; // Case Hardened
						}
						else if (Skin == 8)
						{
							*pWeapon->FallbackPaintKit() = 59; // Slaughter
						}
						else if (Skin == 9)
						{
							*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
						}
						else if (Skin == 10)
						{
							*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
						}
						else if (Skin == 11)
						{
							*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
						}
						else if (Skin == 12)
						{
							*pWeapon->FallbackPaintKit() = 143; // Urban Masked
						}
						else if (Skin == 13)
						{
							*pWeapon->FallbackPaintKit() = 175; // Scorched
						}
						else if (Skin == 14)
						{
							*pWeapon->FallbackPaintKit() = 323; // Rust Coat
						}
						else if (Skin == 15)
						{
							*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
						}
						else if (Skin == 16)
						{
							*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
						}
						else if (Skin == 17)
						{
							*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
						}
						else if (Skin == 18)
						{
							*pWeapon->FallbackPaintKit() = 413; // Marble Fade
						}
						else if (Skin == 19)
						{
							*pWeapon->FallbackPaintKit() = 414; // Rust Coat
						}
						else if (Skin == 20)
						{
							*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
						}
						else if (Skin == 21)
						{
							*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
						}
						else if (Skin == 22)
						{
							*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
						}
						else if (Skin == 23)
						{
							*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
						}
						else if (Skin == 24)
						{
							*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
						}
						else if (Skin == 25)
						{
							*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
						}
						else if (Skin == 26)
						{
							*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
						}
						else if (Skin == 27)
						{
							*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
						}
						else if (Skin == 28)
						{
							*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
						}
						else if (Skin == 29)
						{
							*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
						}
						else if (Skin == 30)
						{
							*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
						}
						else if (Skin == 31)
						{
							*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
						}
						else if (Skin == 32)
						{
							*pWeapon->FallbackPaintKit() = 561; // Lore
						}
						else if (Skin == 33)
						{
							*pWeapon->FallbackPaintKit() = 567; // Black Laminate
						}
						else if (Skin == 34)
						{
							*pWeapon->FallbackPaintKit() = 574; // Autotronic
						}
						else if (Skin == 35)
						{
							*pWeapon->FallbackPaintKit() = 580; // Freehand
						}
					}
				}
				*pWeapon->OwnerXuidLow() = 0;
				*pWeapon->OwnerXuidHigh() = 0;
				*pWeapon->FallbackWear() = 0.001f;
				*pWeapon->ItemIDHigh() = 1;
			}
		}
	}

}

