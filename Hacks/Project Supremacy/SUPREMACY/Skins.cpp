//
//#include "includes.hpp"
//#include "Skins.h"
//#include "Recv.h"
//#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);
//std::unordered_map<char*, char*> killIcons = {};
//#define INVALID_EHANDLE_INDEX 0xFFFFFFFF
//CBaseHandle worldmodel_handle;
//weapon_t* worldmodel;
//#define MakePtr(cast, ptr, addValue) (cast)( (DWORD)(ptr) + (DWORD)(addValue))
//void skinchanger()
//{
//	player_t *pLocal = (player_t*)g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer());
//	auto weapons = pLocal->m_hMyWeapons();
//	for (size_t i = 0; weapons[i] != INVALID_EHANDLE_INDEX; i++)
//	{
//		entity_t *pEntity = (entity_t*)g_csgo.m_entitylist()->GetClientEntityFromHandle(weapons[i]);
//		if (pEntity)
//		{
//			weapon_t* pWeapon = (weapon_t*)pEntity;
//			if (g_cfg.skins.skinenabled || g_cfg.skins.knifes)
//			{
//				int Model = g_cfg.skins.Knife;
//				int weapon = pWeapon->m_iItemDefinitionIndex();
//				switch (weapon)
//				{
//				case 7: // AK47 
//				{
//					switch (g_cfg.skins.AK47Skin)
//					{
//					case 0:  pWeapon->m_nFallbackPaintKit() = 0;   break; //none
//					case 1:  pWeapon->m_nFallbackPaintKit() = 341; break;  //fire serpent
//					case 2:  pWeapon->m_nFallbackPaintKit() = 524; break;  //Fuel Injector
//					case 3:  pWeapon->m_nFallbackPaintKit() = 639; break;  //Bloodsport
//					case 4:  pWeapon->m_nFallbackPaintKit() = 302; break;  //vulcan
//					case 5:  pWeapon->m_nFallbackPaintKit() = 44;  break;  //case hardened
//					case 6:  pWeapon->m_nFallbackPaintKit() = 456; break;  //Hydroponic
//					case 7:  pWeapon->m_nFallbackPaintKit() = 474; break;  //Aquamarine Revenge
//					case 8:  pWeapon->m_nFallbackPaintKit() = 490; break;  //Frontside Misty
//					case 9:  pWeapon->m_nFallbackPaintKit() = 506; break;  //Point Disarray
//					case 10: pWeapon->m_nFallbackPaintKit() = 600; break;  //Neon Revolution
//					case 11: pWeapon->m_nFallbackPaintKit() = 14;  break;  //red laminate
//					case 12: pWeapon->m_nFallbackPaintKit() = 282; break;  //redline
//					case 13: pWeapon->m_nFallbackPaintKit() = 316; break;  //jaguar
//					case 14: pWeapon->m_nFallbackPaintKit() = 340; break;  //jetset
//					case 15: pWeapon->m_nFallbackPaintKit() = 380; break;  //wasteland rebel
//					case 16: pWeapon->m_nFallbackPaintKit() = 675; break;  //The Empress
//					case 17: pWeapon->m_nFallbackPaintKit() = 422; break;  //Elite Build
//					default: break;
//					}
//				}break;
//				case 16: // M4A4
//				{
//					switch (g_cfg.skins.M4A4Skin)
//					{
//					case 0:  pWeapon->m_nFallbackPaintKit() = 0;   break; //none
//					case 1:  pWeapon->m_nFallbackPaintKit() = 255; break; //Asiimov
//					case 2:  pWeapon->m_nFallbackPaintKit() = 309; break; //Howl
//					case 3:  pWeapon->m_nFallbackPaintKit() = 400; break; //Dragon King
//					case 4:  pWeapon->m_nFallbackPaintKit() = 449; break; //Poseidon
//					case 5:  pWeapon->m_nFallbackPaintKit() = 471; break; //Daybreak
//					case 6:  pWeapon->m_nFallbackPaintKit() = 512; break; //Royal Paladin
//					case 7:  pWeapon->m_nFallbackPaintKit() = 533; break; //BattleStar
//					case 8:  pWeapon->m_nFallbackPaintKit() = 588; break; //Desolate Space
//					case 9:  pWeapon->m_nFallbackPaintKit() = 632; break; //Buzz Kill
//					case 10: pWeapon->m_nFallbackPaintKit() = 155; break; //Bullet Rain
//					case 11: pWeapon->m_nFallbackPaintKit() = 664; break; //Hell Fire
//					case 12: pWeapon->m_nFallbackPaintKit() = 480; break; //Evil Daimyo
//					case 13: pWeapon->m_nFallbackPaintKit() = 384; break; //Griffin
//					case 14: pWeapon->m_nFallbackPaintKit() = 187; break; //Zirka
//					case 15: pWeapon->m_nFallbackPaintKit() = 167; break; //Radiation Harzard
//					default: break;
//					}
//				} break;
//				case 2: // dual
//				{
//					switch (g_cfg.skins.DualSkin)
//					{
//					case 0:  pWeapon->m_nFallbackPaintKit() = 0;   break; //none
//					case 1:  pWeapon->m_nFallbackPaintKit() = 276; break;
//					case 2:  pWeapon->m_nFallbackPaintKit() = 491; break;
//					case 3:  pWeapon->m_nFallbackPaintKit() = 658; break;
//					case 4:  pWeapon->m_nFallbackPaintKit() = 625; break;
//					case 5:  pWeapon->m_nFallbackPaintKit() = 447; break;
//					default: break;
//					}
//				} break;
//				case 60: // M4A1
//				{
//					switch (g_cfg.skins.M4A1SSkin)
//					{
//					case 0:  pWeapon->m_nFallbackPaintKit() = 0;   break; //none
//					case 1:  pWeapon->m_nFallbackPaintKit() = 644; break; //
//					case 2:  pWeapon->m_nFallbackPaintKit() = 326; break; //
//					case 3:  pWeapon->m_nFallbackPaintKit() = 548; break; //
//					case 4:  pWeapon->m_nFallbackPaintKit() = 497; break; //
//					case 5:  pWeapon->m_nFallbackPaintKit() = 430; break; //
//					case 6:  pWeapon->m_nFallbackPaintKit() = 321; break; //
//					case 7:  pWeapon->m_nFallbackPaintKit() = 445; break; //
//					case 8:  pWeapon->m_nFallbackPaintKit() = 587; break; //
//					case 9:  pWeapon->m_nFallbackPaintKit() = 360; break; //
//					case 10: pWeapon->m_nFallbackPaintKit() = 440; break; //
//					case 11: pWeapon->m_nFallbackPaintKit() = 631; break; //
//					case 12: pWeapon->m_nFallbackPaintKit() = 681; break; //
//					case 13: pWeapon->m_nFallbackPaintKit() = 430; break; //
//					case 14: pWeapon->m_nFallbackPaintKit() = 301; break; //
//					case 15: pWeapon->m_nFallbackPaintKit() = 257; break; //
//					case 16: pWeapon->m_nFallbackPaintKit() = 663; break; //
//					default: break;
//					}
//				} break;
//				case 9: // AWP
//				{
//					switch (g_cfg.skins.AWPSkin)
//					{
//					case 0:  pWeapon->m_nFallbackPaintKit() = 0;   break; //none
//					case 1:  pWeapon->m_nFallbackPaintKit() = 279; break; //
//					case 2:  pWeapon->m_nFallbackPaintKit() = 344; break; //
//					case 3:  pWeapon->m_nFallbackPaintKit() = 640; break; //
//					case 4:  pWeapon->m_nFallbackPaintKit() = 446; break; //
//					case 5:  pWeapon->m_nFallbackPaintKit() = 475; break; //
//					case 6:  pWeapon->m_nFallbackPaintKit() = 174; break; //
//					case 7:  pWeapon->m_nFallbackPaintKit() = 51; break; //
//					case 8:  pWeapon->m_nFallbackPaintKit() = 84; break; //
//					case 9:  pWeapon->m_nFallbackPaintKit() = 181; break; //
//					case 10: pWeapon->m_nFallbackPaintKit() = 259; break; //
//					case 11: pWeapon->m_nFallbackPaintKit() = 395; break; //
//					case 12: pWeapon->m_nFallbackPaintKit() = 212; break; //
//					case 13: pWeapon->m_nFallbackPaintKit() = 227; break; //
//					case 14: pWeapon->m_nFallbackPaintKit() = 451; break; //
//					case 15: pWeapon->m_nFallbackPaintKit() = 475; break; //
//					case 16: pWeapon->m_nFallbackPaintKit() = 251; break; //
//					case 17: pWeapon->m_nFallbackPaintKit() = 584; break; //
//					case 18: pWeapon->m_nFallbackPaintKit() = 525; break; //
//					case 19: pWeapon->m_nFallbackPaintKit() = 424; break; //
//					case 20: pWeapon->m_nFallbackPaintKit() = 662; break; //
//					case 21: pWeapon->m_nFallbackPaintKit() = 640; break; //
//					default: break;
//					}
//				} break;
//				case 61: // USP
//				{
//					switch (g_cfg.skins.USPSkin)
//					{
//					case 0:  pWeapon->m_nFallbackPaintKit() = 0;   break; //none
//					case 1:  pWeapon->m_nFallbackPaintKit() = 653; break; //
//					case 2:  pWeapon->m_nFallbackPaintKit() = 637; break; //
//					case 3:  pWeapon->m_nFallbackPaintKit() = 313; break; //
//					case 4:  pWeapon->m_nFallbackPaintKit() = 504; break; //
//					case 5:  pWeapon->m_nFallbackPaintKit() = 183; break; //
//					case 6:  pWeapon->m_nFallbackPaintKit() = 339; break; //
//					case 7:  pWeapon->m_nFallbackPaintKit() = 221; break; //
//					case 8:  pWeapon->m_nFallbackPaintKit() = 290; break; //
//					case 9:  pWeapon->m_nFallbackPaintKit() = 318; break; //
//					default: break;
//					}
//				} break;
//				case 4: // Glock
//				{
//					switch (g_cfg.skins.GlockSkin)
//					{
//					case 0: pWeapon->m_nFallbackPaintKit() = 0; break;
//					case 1:
//						pWeapon->m_nFallbackPaintKit() = 38;
//						break;
//					case 2:
//						pWeapon->m_nFallbackPaintKit() = 48;
//						break;
//					case 3:
//						pWeapon->m_nFallbackPaintKit() = 437;
//						break;
//					case 4:
//						pWeapon->m_nFallbackPaintKit() = 586;
//						break;
//					case 5:
//						pWeapon->m_nFallbackPaintKit() = 353;
//						break;
//					case 6:
//						pWeapon->m_nFallbackPaintKit() = 680;
//						break;
//					case 7:
//						pWeapon->m_nFallbackPaintKit() = 607;
//						break;
//					case 8:
//						pWeapon->m_nFallbackPaintKit() = 532;
//						break;
//					case 9:
//						pWeapon->m_nFallbackPaintKit() = 381;
//						break;
//					case 10:
//						pWeapon->m_nFallbackPaintKit() = 230;
//						break;
//					case 11:
//						pWeapon->m_nFallbackPaintKit() = 159;
//						break;
//					case 12:
//						pWeapon->m_nFallbackPaintKit() = 623;
//						break;
//					case 13:
//						pWeapon->m_nFallbackPaintKit() = 479;
//						break;
//					case 14:
//						pWeapon->m_nFallbackPaintKit() = 680;
//						break;
//					case 15:
//						pWeapon->m_nFallbackPaintKit() = 367;
//						break;
//					default:
//						break;
//					}
//				}
//				break;
//				case 1: // Deagle
//				{
//					switch (g_cfg.skins.DeagleSkin)
//					{
//					case 0:
//						pWeapon->m_nFallbackPaintKit() = 0;//none
//						break;
//					case 1:
//						pWeapon->m_nFallbackPaintKit() = 37;
//						break;
//					case 2:
//						pWeapon->m_nFallbackPaintKit() = 527;
//						break;
//					case 3:
//						pWeapon->m_nFallbackPaintKit() = 645;
//						break;
//					case 4:
//						pWeapon->m_nFallbackPaintKit() = 185;
//						break;
//					case 5:
//						pWeapon->m_nFallbackPaintKit() = 37;
//						break;
//					case 6:
//						pWeapon->m_nFallbackPaintKit() = 645;
//						break;
//					case 7:
//						pWeapon->m_nFallbackPaintKit() = 231;
//						break;
//					case 8:
//						pWeapon->m_nFallbackPaintKit() = 603;
//						break;
//					default:
//						break;
//					}
//				}
//				break;
//				case 3: // Five Seven
//				{
//					switch (g_cfg.skins.FiveSkin)
//					{
//					case 0:
//						pWeapon->m_nFallbackPaintKit() = 0;//none
//						break;
//					case 1:
//						pWeapon->m_nFallbackPaintKit() = 427;
//						break;
//					case 2:
//						pWeapon->m_nFallbackPaintKit() = 660;
//						break;
//					case 3:
//						pWeapon->m_nFallbackPaintKit() = 352;
//						break;
//					case 4:
//						pWeapon->m_nFallbackPaintKit() = 530;
//						break;
//					case 5:
//						pWeapon->m_nFallbackPaintKit() = 510;
//						break;
//					case 6:
//						pWeapon->m_nFallbackPaintKit() = 646;
//						break;
//					case 7:
//						pWeapon->m_nFallbackPaintKit() = 585;
//						break;
//					default:
//						break;
//					}
//				}
//				break;
//				case 8: // AUG
//				{
//					switch (g_cfg.skins.AUGSkin)
//					{
//					case 0:
//						pWeapon->m_nFallbackPaintKit() = 0;//none
//						break;
//					case 1:
//						pWeapon->m_nFallbackPaintKit() = 9;
//						break;
//					case 2:
//						pWeapon->m_nFallbackPaintKit() = 33;
//						break;
//					case 3:
//						pWeapon->m_nFallbackPaintKit() = 280;
//						break;
//					case 4:
//						pWeapon->m_nFallbackPaintKit() = 455;
//						break;
//					default:
//						break;
//					}
//				}
//				break;
//				case 10: // Famas
//				{
//					switch (g_cfg.skins.FAMASSkin)
//					{
//					case 0:
//						pWeapon->m_nFallbackPaintKit() = 0;//none
//						break;
//					case 1:
//						pWeapon->m_nFallbackPaintKit() = 429;
//						break;
//					case 2:
//						pWeapon->m_nFallbackPaintKit() = 371;
//						break;
//					case 3:
//						pWeapon->m_nFallbackPaintKit() = 477;
//						break;
//					case 4:
//						pWeapon->m_nFallbackPaintKit() = 492;
//						break;
//					default:
//						break;
//					}
//				}
//				break;
//				case 11: // G3SG1
//				{
//					switch (g_cfg.skins.G3sg1Skin)
//					{
//					case 0:
//						pWeapon->m_nFallbackPaintKit() = 0;//none
//						break;
//					case 1:
//						pWeapon->m_nFallbackPaintKit() = 677;
//						break;
//					case 2:
//						pWeapon->m_nFallbackPaintKit() = 511;
//						break;
//					case 3:
//						pWeapon->m_nFallbackPaintKit() = 463;
//						break;
//					case 4:
//						pWeapon->m_nFallbackPaintKit() = 464;
//						break;
//					case 5:
//						pWeapon->m_nFallbackPaintKit() = 465;
//						break;
//					case 6:
//						pWeapon->m_nFallbackPaintKit() = 170;
//						break;
//					case 7:
//						pWeapon->m_nFallbackPaintKit() = 438;
//						break;
//					default:
//						break;
//					}
//				}
//				break;
//				case 13: // Galil
//				{
//					switch (g_cfg.skins.GalilSkin)
//					{
//					case 0:
//						pWeapon->m_nFallbackPaintKit() = 0;//none
//						break;
//					case 1:
//						pWeapon->m_nFallbackPaintKit() = 398;
//						break;
//					case 2:
//						pWeapon->m_nFallbackPaintKit() = 647;
//						break;
//					case 3:
//						pWeapon->m_nFallbackPaintKit() = 661;
//						break;
//					case 4:
//						pWeapon->m_nFallbackPaintKit() = 428;
//						break;
//					case 5:
//						pWeapon->m_nFallbackPaintKit() = 379;
//						break;
//					default:
//						break;
//					}
//				}
//				break;
//				case 14: // M249
//				{
//					switch (g_cfg.skins.M249Skin)
//					{
//					case 0:
//						pWeapon->m_nFallbackPaintKit() = 0;
//						break;
//					case 1:
//						pWeapon->m_nFallbackPaintKit() = 496;
//						break;
//					case 2:
//						pWeapon->m_nFallbackPaintKit() = 401;
//						break;
//					case 3:
//						pWeapon->m_nFallbackPaintKit() = 266;
//						break;
//					default:
//						break;
//					}
//				}
//				break;
//				case 17: // Mac 10
//				{
//					switch (g_cfg.skins.Mac10Skin)
//					{
//					case 0:
//						pWeapon->m_nFallbackPaintKit() = 0;//none
//						break;
//					case 1:
//						pWeapon->m_nFallbackPaintKit() = 433;
//						break;
//					case 2:
//						pWeapon->m_nFallbackPaintKit() = 651;
//						break;
//					case 3:
//						pWeapon->m_nFallbackPaintKit() = 310;
//						break;
//					case 4:
//						pWeapon->m_nFallbackPaintKit() = 498;
//						break;
//					default:
//						break;
//					}
//				}
//				break;
//				case 19: // P90
//				{
//					pWeapon->m_nFallbackPaintKit() = 156;
//				}
//				break;
//				case 24: // UMP-45
//				{
//					switch (g_cfg.skins.UMP45Skin)
//					{
//					case 0: pWeapon->m_nFallbackPaintKit() = 0; break;//none
//					case 1: pWeapon->m_nFallbackPaintKit() = 37; break;
//					case 2: pWeapon->m_nFallbackPaintKit() = 441; break;
//					case 3: pWeapon->m_nFallbackPaintKit() = 448; break;
//					case 4: pWeapon->m_nFallbackPaintKit() = 556; break;
//					case 5: pWeapon->m_nFallbackPaintKit() = 688; break;
//					default:
//						break;
//					}
//				}
//				break;
//				case 25: // XM1014
//				{
//					switch (g_cfg.skins.XmSkin)
//					{
//					case 0:
//						pWeapon->m_nFallbackPaintKit() = 0;//none
//						break;
//					case 1:
//						pWeapon->m_nFallbackPaintKit() = 654;
//						break;
//					case 2:
//						pWeapon->m_nFallbackPaintKit() = 363;
//						break;
//					case 3:
//						pWeapon->m_nFallbackPaintKit() = 689;
//						break;
//					default:
//						break;
//					}
//				}
//				break;
//				case 63: // CZ75-Auto
//				{
//					switch (g_cfg.skins.Cz75Skin)
//					{
//					case 0:
//						pWeapon->m_nFallbackPaintKit() = 0;//none
//						break;
//					case 1:
//						pWeapon->m_nFallbackPaintKit() = 543;
//						break;
//					case 2:
//						pWeapon->m_nFallbackPaintKit() = 435;
//						break;
//					case 3:
//						pWeapon->m_nFallbackPaintKit() = 270;
//						break;
//					case 4:
//						pWeapon->m_nFallbackPaintKit() = 643;
//						break;
//					default:
//						break;
//					}
//				}
//				break;
//				case 26: // Bizon
//				{
//					switch (g_cfg.skins.BizonSkin)
//					{
//					case 0:
//						pWeapon->m_nFallbackPaintKit() = 0;
//						break;
//					case 1:
//						pWeapon->m_nFallbackPaintKit() = 676;
//						break;
//					case 2:
//						pWeapon->m_nFallbackPaintKit() = 542;
//						break;
//					case 3:
//						pWeapon->m_nFallbackPaintKit() = 508;
//						break;
//					default:
//						break;
//					}
//				}
//				break;
//				case 27: // Mag 7
//				{
//					switch (g_cfg.skins.MagSkin)
//					{
//					case 0:
//						pWeapon->m_nFallbackPaintKit() = 0;//none
//						break;
//					case 1:
//						pWeapon->m_nFallbackPaintKit() = 39;
//						break;
//					case 2:
//						pWeapon->m_nFallbackPaintKit() = 431;
//						break;
//					case 3:
//						pWeapon->m_nFallbackPaintKit() = 608;
//						break;
//					default:
//						break;
//					}
//				}
//				break;
//				case 28: // Negev
//				{
//					switch (g_cfg.skins.NegevSkin)
//					{
//					case 0:
//						pWeapon->m_nFallbackPaintKit() = 0;//none
//						break;
//					case 1:
//						pWeapon->m_nFallbackPaintKit() = 514;
//						break;
//					case 2:
//						pWeapon->m_nFallbackPaintKit() = 483;
//						break;
//					case 3:
//						pWeapon->m_nFallbackPaintKit() = 432;
//						break;
//					default:
//						break;
//					}
//				}
//				break;
//				case 29: // Sawed Off
//				{
//					switch (g_cfg.skins.SawedSkin)
//					{
//					case 0:
//						pWeapon->m_nFallbackPaintKit() = 0;//none
//						break;
//					case 1:
//						pWeapon->m_nFallbackPaintKit() = 638;
//						break;
//					case 2:
//						pWeapon->m_nFallbackPaintKit() = 256;
//						break;
//					case 3:
//						pWeapon->m_nFallbackPaintKit() = 517;
//						break;
//					default:
//						break;
//					}
//				}
//				break;
//				case 30: // Tec 9
//				{
//					switch (g_cfg.skins.tec9Skin)
//					{
//					case 0:
//						pWeapon->m_nFallbackPaintKit() = 0;//none
//						break;
//					case 1:
//						pWeapon->m_nFallbackPaintKit() = 179;
//						break;
//					case 2:
//						pWeapon->m_nFallbackPaintKit() = 248;
//						break;
//					case 3:
//						pWeapon->m_nFallbackPaintKit() = 216;
//						break;
//					case 4:
//						pWeapon->m_nFallbackPaintKit() = 272;
//						break;
//					case 5:
//						pWeapon->m_nFallbackPaintKit() = 289;
//						break;
//					case 6:
//						pWeapon->m_nFallbackPaintKit() = 303;
//						break;
//					case 7:
//						pWeapon->m_nFallbackPaintKit() = 374;
//						break;
//					case 8:
//						pWeapon->m_nFallbackPaintKit() = 555;
//						break;
//					case 9:
//						pWeapon->m_nFallbackPaintKit() = 614;
//						break;
//					default:
//						break;
//					}
//				}
//				break;
//				case 32: // P2000
//				{
//					switch (g_cfg.skins.P2000Skin)
//					{
//					case 0:
//						pWeapon->m_nFallbackPaintKit() = 0;//none
//						break;
//					case 1:
//						pWeapon->m_nFallbackPaintKit() = 485;
//						break;
//					case 2:
//						pWeapon->m_nFallbackPaintKit() = 38;
//						break;
//					case 3:
//						pWeapon->m_nFallbackPaintKit() = 184;
//						break;
//					case 4:
//						pWeapon->m_nFallbackPaintKit() = 211;
//						break;
//					case 5:
//						pWeapon->m_nFallbackPaintKit() = 389;
//						break;
//					case 6:
//						pWeapon->m_nFallbackPaintKit() = 442;
//						break;
//					case 7:
//						pWeapon->m_nFallbackPaintKit() = 443;
//						break;
//					case 8:
//						pWeapon->m_nFallbackPaintKit() = 515;
//						break;
//					case 9:
//						pWeapon->m_nFallbackPaintKit() = 550;
//						break;
//					case 10:
//						pWeapon->m_nFallbackPaintKit() = 591;
//						break;
//					default:
//						break;
//					}
//				}
//				break;
//				case 33: // MP7
//				{
//					switch (g_cfg.skins.Mp9Skin)
//					{
//					case 0:
//						pWeapon->m_nFallbackPaintKit() = 0;
//						break;
//					case 1:
//						pWeapon->m_nFallbackPaintKit() = 481;
//						break;
//					case 2:
//						pWeapon->m_nFallbackPaintKit() = 536;
//						break;
//					case 3:
//						pWeapon->m_nFallbackPaintKit() = 500;
//						break;
//					default:
//						break;
//					}
//				}
//				break;
//				case 34: // MP9
//				{
//					switch (g_cfg.skins.Mp9Skin)
//					{
//					case 0:
//						pWeapon->m_nFallbackPaintKit() = 0;
//						break;
//					case 1:
//						pWeapon->m_nFallbackPaintKit() = 262;
//						break;
//					case 2:
//						pWeapon->m_nFallbackPaintKit() = 482;
//						break;
//					case 3:
//						pWeapon->m_nFallbackPaintKit() = 609;
//						break;
//					default:
//						break;
//					}
//				}
//				break;
//				case 35: // Nova
//				{
//					switch (g_cfg.skins.NovaSkin)
//					{
//					case 0:
//						pWeapon->m_nFallbackPaintKit() = 0;
//						break;
//					case 1:
//						pWeapon->m_nFallbackPaintKit() = 537;
//						break;
//					case 2:
//						pWeapon->m_nFallbackPaintKit() = 356;
//						break;
//					case 3:
//						pWeapon->m_nFallbackPaintKit() = 286;
//						break;
//					default:
//						break;
//					}
//				}
//				break;
//				case 36: // P250
//				{
//					switch (g_cfg.skins.P250Skin)
//					{
//					case 0:
//						pWeapon->m_nFallbackPaintKit() = 0;//none
//						break;
//					case 1:
//						pWeapon->m_nFallbackPaintKit() = 102;
//						break;
//					case 2:
//						pWeapon->m_nFallbackPaintKit() = 466;
//						break;
//					case 3:
//						pWeapon->m_nFallbackPaintKit() = 467;
//						break;
//					case 4:
//						pWeapon->m_nFallbackPaintKit() = 501;
//						break;
//					case 5:
//						pWeapon->m_nFallbackPaintKit() = 551;
//						break;
//					case 6:
//						pWeapon->m_nFallbackPaintKit() = 678;
//						break;
//					default:
//						break;
//					}
//				}
//				break;
//				case 38: // Scar 20
//				{
//					switch (g_cfg.skins.SCAR20Skin)
//					{
//					case 0:
//						pWeapon->m_nFallbackPaintKit() = 0;//none
//						break;
//					case 1:
//						pWeapon->m_nFallbackPaintKit() = 165;//Splash Jam
//						break;
//					case 2:
//						pWeapon->m_nFallbackPaintKit() = 100;//Storm
//						break;
//					case 3:
//						pWeapon->m_nFallbackPaintKit() = 46;//Contractor
//						break;
//					case 4:
//						pWeapon->m_nFallbackPaintKit() = 70;//Carbon Fiber
//						break;
//					case 5:
//						pWeapon->m_nFallbackPaintKit() = 116;//Sand Mesh
//						break;
//					case 6:
//						pWeapon->m_nFallbackPaintKit() = 157;//Palm
//						break;
//					case 7:
//						pWeapon->m_nFallbackPaintKit() = 232;//Crimson Web
//						break;
//					case 8:
//						pWeapon->m_nFallbackPaintKit() = 391;//Cardiac
//						break;
//					case 9:
//						pWeapon->m_nFallbackPaintKit() = 298;//Army Sheen
//						break;
//					case 10:
//						pWeapon->m_nFallbackPaintKit() = 312;//Cyrex
//						break;
//					case 11:
//						pWeapon->m_nFallbackPaintKit() = 406;//Grotto
//						break;
//					case 12:
//						pWeapon->m_nFallbackPaintKit() = 597;//Bloodsport
//						break;
//					default:
//						break;
//					}
//				}
//				break;
//				case 39: // SG553
//				{
//					switch (g_cfg.skins.Sg553Skin)
//					{
//					case 0:
//						pWeapon->m_nFallbackPaintKit() = 0;//none
//						break;
//					case 1:
//						pWeapon->m_nFallbackPaintKit() = 519;
//						break;
//					case 2:
//						pWeapon->m_nFallbackPaintKit() = 487;
//						break;
//					case 3:
//						pWeapon->m_nFallbackPaintKit() = 287;
//						break;
//					case 4:
//						pWeapon->m_nFallbackPaintKit() = 586;
//						break;
//					default:
//						break;
//					}
//				}
//				break;
//				case 40: // SSG08
//				{
//					switch (g_cfg.skins.SSG08Skin)
//					{
//					case 0:
//						pWeapon->m_nFallbackPaintKit() = 0;//none
//						break;
//					case 1:
//						pWeapon->m_nFallbackPaintKit() = 26;//26 - Lichen Dashed
//						break;
//					case 2:
//						pWeapon->m_nFallbackPaintKit() = 60;//60 - Dark Water
//						break;
//					case 3:
//						pWeapon->m_nFallbackPaintKit() = 96;//96 - Blue Spruce
//						break;
//					case 4:
//						pWeapon->m_nFallbackPaintKit() = 99;//99 - Sand Dune
//						break;
//					case 5:
//						pWeapon->m_nFallbackPaintKit() = 157;//157 - Palm
//						break;
//					case 6:
//						pWeapon->m_nFallbackPaintKit() = 200;//200 - Mayan Dreams
//						break;
//					case 7:
//						pWeapon->m_nFallbackPaintKit() = 222;//222 - Blood in the Water
//						break;
//					case 8:
//						pWeapon->m_nFallbackPaintKit() = 233;//233 - Tropical Storm
//						break;
//					case 9:
//						pWeapon->m_nFallbackPaintKit() = 253;//253 - Acid Fade
//						break;
//					case 10:
//						pWeapon->m_nFallbackPaintKit() = 304;//304 - Slashed
//						break;
//					case 11:
//						pWeapon->m_nFallbackPaintKit() = 319;//319 - Detour
//						break;
//					case 12:
//						pWeapon->m_nFallbackPaintKit() = 361;//361 - Abyss
//						break;
//					case 13:
//						pWeapon->m_nFallbackPaintKit() = 503;//503: 'Big Iron'
//						break;
//					case 14:
//						pWeapon->m_nFallbackPaintKit() = 538;//538: 'Necropos'
//						break;
//					case 15:
//						pWeapon->m_nFallbackPaintKit() = 554;//554 - Ghost Crusader
//						break;
//					case 16:
//						pWeapon->m_nFallbackPaintKit() = 624;//624: 'Dragonfire'
//						break;
//					default:
//						break;
//					}
//				}
//				break;
//				case 64: // Revolver
//				{
//					switch (g_cfg.skins.RevolverSkin)
//					{
//					case 0:
//						pWeapon->m_nFallbackPaintKit() = 0;//none
//						break;
//					case 1:
//						pWeapon->m_nFallbackPaintKit() = 683;
//						break;
//					case 2:
//						pWeapon->m_nFallbackPaintKit() = 522;
//						break;
//					case 3:
//						pWeapon->m_nFallbackPaintKit() = 12;
//						break;
//					case 4:
//						pWeapon->m_nFallbackPaintKit() = 595;
//						break;
//					case 5:
//						pWeapon->m_nFallbackPaintKit() = 798;
//						break;
//					case 6:
//						pWeapon->m_nFallbackPaintKit() = 27;
//						break;
//					default:
//
//						break;
//					}
//				}
//				break;
//				default:
//					break;
//				}
//				if (pEntity->GetClientClass()->m_ClassID == 105)
//				{
//
//					auto pCustomName1 = MakePtr(char*, pWeapon, 0x301C);
//					worldmodel_handle = pWeapon->m_hWeaponWorldModel();
//					HANDLE worldmodel_handle2 = pWeapon->m_hWeaponWorldModel2();
//
//					if (worldmodel_handle2)
//						worldmodel = (weapon_t*)g_csgo.m_entitylist()->GetClientEntityFromHandle(worldmodel_handle);
//
//					if (Model == 0) // Bayonet
//					{
//						int iBayonet = g_csgo.m_modelinfo()->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
//						pWeapon->m_nModelIndex() = iBayonet;
//						pWeapon->m_iViewModelIndex() = iBayonet;
//						if (worldmodel) pWeapon->m_nModelIndex() = iBayonet + 1;
//						*pWeapon->fixskins() = 500;
//						pWeapon->m_bPinPulled() = 3;
//						killIcons.clear();
//						killIcons["knife_default_ct"] = "bayonet";
//						killIcons["knife_t"] = "bayonet";
//						int Skin = g_cfg.skins.KnifeSkin;
//						if (Skin == 0)
//						{
//							pWeapon->m_nFallbackPaintKit() = 0; // Forest DDPAT
//						}
//						else if (Skin == 1)
//						{
//							pWeapon->m_nFallbackPaintKit() = 12; // Crimson Web
//						}
//						else if (Skin == 2)
//						{
//							pWeapon->m_nFallbackPaintKit() = 27; // Bone Mask
//						}
//						else if (Skin == 3)
//						{
//							pWeapon->m_nFallbackPaintKit() = 38; // Fade
//						}
//						else if (Skin == 4)
//						{
//							pWeapon->m_nFallbackPaintKit() = 40; // Night
//						}
//						else if (Skin == 5)
//						{
//							pWeapon->m_nFallbackPaintKit() = 42; // Blue Steel
//						}
//						else if (Skin == 6)
//						{
//							pWeapon->m_nFallbackPaintKit() = 43; // Stained
//						}
//						else if (Skin == 7)
//						{
//							pWeapon->m_nFallbackPaintKit() = 44; // Case Hardened
//						}
//						else if (Skin == 8)
//						{
//							pWeapon->m_nFallbackPaintKit() = 59; // Slaughter
//						}
//						else if (Skin == 9)
//						{
//							pWeapon->m_nFallbackPaintKit() = 72; // Safari Mesh
//						}
//						else if (Skin == 10)
//						{
//							pWeapon->m_nFallbackPaintKit() = 77; // Boreal Forest
//						}
//						else if (Skin == 11)
//						{
//							pWeapon->m_nFallbackPaintKit() = 98; // Ultraviolet
//						}
//						else if (Skin == 12)
//						{
//							pWeapon->m_nFallbackPaintKit() = 143; // Urban Masked
//						}
//						else if (Skin == 13)
//						{
//							pWeapon->m_nFallbackPaintKit() = 175; // Scorched
//						}
//						else if (Skin == 14)
//						{
//							pWeapon->m_nFallbackPaintKit() = 323; // Rust Coat
//						}
//						else if (Skin == 15)
//						{
//							pWeapon->m_nFallbackPaintKit() = 409; // Tiger Tooth
//						}
//						else if (Skin == 16)
//						{
//							pWeapon->m_nFallbackPaintKit() = 410; // Damascus Steel
//						}
//						else if (Skin == 17)
//						{
//							pWeapon->m_nFallbackPaintKit() = 411; // Damascus Steel
//						}
//						else if (Skin == 18)
//						{
//							pWeapon->m_nFallbackPaintKit() = 413; // Marble Fade
//						}
//						else if (Skin == 19)
//						{
//							pWeapon->m_nFallbackPaintKit() = 414; // Rust Coat
//						}
//						else if (Skin == 20)
//						{
//							pWeapon->m_nFallbackPaintKit() = 415; // Doppler Ruby
//						}
//						else if (Skin == 21)
//						{
//							pWeapon->m_nFallbackPaintKit() = 416; // Doppler Sapphire
//						}
//						else if (Skin == 22)
//						{
//							pWeapon->m_nFallbackPaintKit() = 417; // Doppler Blackpearl
//						}
//						else if (Skin == 23)
//						{
//							pWeapon->m_nFallbackPaintKit() = 418; // Doppler Phase 1
//						}
//						else if (Skin == 24)
//						{
//							pWeapon->m_nFallbackPaintKit() = 419; // Doppler Phase 2
//						}
//						else if (Skin == 25)
//						{
//							pWeapon->m_nFallbackPaintKit() = 420; // Doppler Phase 3
//						}
//						else if (Skin == 26)
//						{
//							pWeapon->m_nFallbackPaintKit() = 421; // Doppler Phase 4
//						}
//						else if (Skin == 27)
//						{
//							pWeapon->m_nFallbackPaintKit() = 569; // Gamma Doppler Phase1
//						}
//						else if (Skin == 28)
//						{
//							pWeapon->m_nFallbackPaintKit() = 570; // Gamma Doppler Phase2
//						}
//						else if (Skin == 29)
//						{
//							pWeapon->m_nFallbackPaintKit() = 571; // Gamma Doppler Phase3
//						}
//						else if (Skin == 30)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Gamma Doppler Phase4
//						}
//						else if (Skin == 31)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Gamma Doppler Emerald
//						}
//						else if (Skin == 32)
//						{
//							pWeapon->m_nFallbackPaintKit() = 558; // Lore
//						}
//						else if (Skin == 33)
//						{
//							pWeapon->m_nFallbackPaintKit() = 563; // Black Laminate
//						}
//						else if (Skin == 34)
//						{
//							pWeapon->m_nFallbackPaintKit() = 573; // Autotronic
//						}
//						else if (Skin == 35)
//						{
//							pWeapon->m_nFallbackPaintKit() = 361; // Freehand
//						}
//					}
//					else if (Model == 9) // Bowie Knife
//					{
//						int iBowie = g_csgo.m_modelinfo()->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");
//						pWeapon->m_nModelIndex() = iBowie; // m_nModelIndex
//						pWeapon->m_iViewModelIndex() = iBowie;
//						if (worldmodel) worldmodel->m_nModelIndex() = iBowie + 1;
//						*pWeapon->fixskins() = 514;
//						pWeapon->m_iEntityQuality() = 3;
//						killIcons.clear();
//						killIcons["knife_default_ct"] = "knife_survival_bowie";
//						killIcons["knife_t"] = "knife_survival_bowie";
//						int Skin = g_cfg.skins.KnifeSkin;
//						if (Skin == 0)
//						{
//							pWeapon->m_nFallbackPaintKit() = 0; // Forest DDPAT
//						}
//						else if (Skin == 1)
//						{
//							pWeapon->m_nFallbackPaintKit() = 12; // Crimson Web
//						}
//						else if (Skin == 2)
//						{
//							pWeapon->m_nFallbackPaintKit() = 27; // Bone Mask
//						}
//						else if (Skin == 3)
//						{
//							pWeapon->m_nFallbackPaintKit() = 38; // Fade
//						}
//						else if (Skin == 4)
//						{
//							pWeapon->m_nFallbackPaintKit() = 40; // Night
//						}
//						else if (Skin == 5)
//						{
//							pWeapon->m_nFallbackPaintKit() = 42; // Blue Steel
//						}
//						else if (Skin == 6)
//						{
//							pWeapon->m_nFallbackPaintKit() = 43; // Stained
//						}
//						else if (Skin == 7)
//						{
//							pWeapon->m_nFallbackPaintKit() = 44; // Case Hardened
//						}
//						else if (Skin == 8)
//						{
//							pWeapon->m_nFallbackPaintKit() = 59; // Slaughter
//						}
//						else if (Skin == 9)
//						{
//							pWeapon->m_nFallbackPaintKit() = 72; // Safari Mesh
//						}
//						else if (Skin == 10)
//						{
//							pWeapon->m_nFallbackPaintKit() = 77; // Boreal Forest
//						}
//						else if (Skin == 11)
//						{
//							pWeapon->m_nFallbackPaintKit() = 98; // Ultraviolet
//						}
//						else if (Skin == 12)
//						{
//							pWeapon->m_nFallbackPaintKit() = 143; // Urban Masked
//						}
//						else if (Skin == 13)
//						{
//							pWeapon->m_nFallbackPaintKit() = 175; // Scorched
//						}
//						else if (Skin == 14)
//						{
//							pWeapon->m_nFallbackPaintKit() = 323; // Rust Coat
//						}
//						else if (Skin == 15)
//						{
//							pWeapon->m_nFallbackPaintKit() = 409; // Tiger Tooth
//						}
//						else if (Skin == 16)
//						{
//							pWeapon->m_nFallbackPaintKit() = 410; // Damascus Steel
//						}
//						else if (Skin == 17)
//						{
//							pWeapon->m_nFallbackPaintKit() = 411; // Damascus Steel
//						}
//						else if (Skin == 18)
//						{
//							pWeapon->m_nFallbackPaintKit() = 413; // Marble Fade
//						}
//						else if (Skin == 19)
//						{
//							pWeapon->m_nFallbackPaintKit() = 414; // Rust Coat
//						}
//						else if (Skin == 20)
//						{
//							pWeapon->m_nFallbackPaintKit() = 415; // Doppler Ruby
//						}
//						else if (Skin == 21)
//						{
//							pWeapon->m_nFallbackPaintKit() = 416; // Doppler Sapphire
//						}
//						else if (Skin == 22)
//						{
//							pWeapon->m_nFallbackPaintKit() = 417; // Doppler Blackpearl
//						}
//						else if (Skin == 23)
//						{
//							pWeapon->m_nFallbackPaintKit() = 418; // Doppler Phase 1
//						}
//						else if (Skin == 24)
//						{
//							pWeapon->m_nFallbackPaintKit() = 419; // Doppler Phase 2
//						}
//						else if (Skin == 25)
//						{
//							pWeapon->m_nFallbackPaintKit() = 420; // Doppler Phase 3
//						}
//						else if (Skin == 26)
//						{
//							pWeapon->m_nFallbackPaintKit() = 421; // Doppler Phase 4
//						}
//						else if (Skin == 27)
//						{
//							pWeapon->m_nFallbackPaintKit() = 569; // Gamma Doppler Phase1
//						}
//						else if (Skin == 28)
//						{
//							pWeapon->m_nFallbackPaintKit() = 570; // Gamma Doppler Phase2
//						}
//						else if (Skin == 29)
//						{
//							pWeapon->m_nFallbackPaintKit() = 571; // Gamma Doppler Phase3
//						}
//						else if (Skin == 30)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Gamma Doppler Phase4
//						}
//						else if (Skin == 31)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Gamma Doppler Emerald
//						}
//						else if (Skin == 32)
//						{
//							pWeapon->m_nFallbackPaintKit() = 558; // Lore
//						}
//						else if (Skin == 33)
//						{
//							pWeapon->m_nFallbackPaintKit() = 567; // Black Laminate
//						}
//						else if (Skin == 34)
//						{
//							pWeapon->m_nFallbackPaintKit() = 574; // Autotronic
//						}
//						else if (Skin == 35)
//						{
//							pWeapon->m_nFallbackPaintKit() = 361; // Freehand
//						}
//
//					}
//					else if (Model == 6) // Butterfly Knife
//					{
//						int iButterfly = g_csgo.m_modelinfo()->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
//						pWeapon->m_nModelIndex() = iButterfly; // m_nModelIndex
//						pWeapon->m_iViewModelIndex() = iButterfly;
//						if (worldmodel) worldmodel->m_nModelIndex() = iButterfly + 1;
//						*pWeapon->fixskins() = 515;
//						pWeapon->m_iEntityQuality() = 3;
//						killIcons.clear();
//						killIcons["knife_default_ct"] = "knife_butterfly";
//						killIcons["knife_t"] = "knife_butterfly";
//						int Skin = g_cfg.skins.KnifeSkin;
//						if (Skin == 0)
//						{
//							pWeapon->m_nFallbackPaintKit() = 0; // Forest DDPAT
//						}
//						else if (Skin == 1)
//						{
//							pWeapon->m_nFallbackPaintKit() = 12; // Crimson Web
//						}
//						else if (Skin == 2)
//						{
//							pWeapon->m_nFallbackPaintKit() = 27; // Bone Mask
//						}
//						else if (Skin == 3)
//						{
//							pWeapon->m_nFallbackPaintKit() = 38; // Fade
//						}
//						else if (Skin == 4)
//						{
//							pWeapon->m_nFallbackPaintKit() = 40; // Night
//						}
//						else if (Skin == 5)
//						{
//							pWeapon->m_nFallbackPaintKit() = 42; // Blue Steel
//						}
//						else if (Skin == 6)
//						{
//							pWeapon->m_nFallbackPaintKit() = 43; // Stained
//						}
//						else if (Skin == 7)
//						{
//							pWeapon->m_nFallbackPaintKit() = 44; // Case Hardened
//						}
//						else if (Skin == 8)
//						{
//							pWeapon->m_nFallbackPaintKit() = 59; // Slaughter
//						}
//						else if (Skin == 9)
//						{
//							pWeapon->m_nFallbackPaintKit() = 72; // Safari Mesh
//						}
//						else if (Skin == 10)
//						{
//							pWeapon->m_nFallbackPaintKit() = 77; // Boreal Forest
//						}
//						else if (Skin == 11)
//						{
//							pWeapon->m_nFallbackPaintKit() = 98; // Ultraviolet
//						}
//						else if (Skin == 12)
//						{
//							pWeapon->m_nFallbackPaintKit() = 143; // Urban Masked
//						}
//						else if (Skin == 13)
//						{
//							pWeapon->m_nFallbackPaintKit() = 175; // Scorched
//						}
//						else if (Skin == 14)
//						{
//							pWeapon->m_nFallbackPaintKit() = 323; // Rust Coat
//						}
//						else if (Skin == 15)
//						{
//							pWeapon->m_nFallbackPaintKit() = 409; // Tiger Tooth
//						}
//						else if (Skin == 16)
//						{
//							pWeapon->m_nFallbackPaintKit() = 410; // Damascus Steel
//						}
//						else if (Skin == 17)
//						{
//							pWeapon->m_nFallbackPaintKit() = 411; // Damascus Steel
//						}
//						else if (Skin == 18)
//						{
//							pWeapon->m_nFallbackPaintKit() = 413; // Marble Fade
//						}
//						else if (Skin == 19)
//						{
//							pWeapon->m_nFallbackPaintKit() = 414; // Rust Coat
//						}
//						else if (Skin == 20)
//						{
//							pWeapon->m_nFallbackPaintKit() = 415; // Doppler Ruby
//						}
//						else if (Skin == 21)
//						{
//							pWeapon->m_nFallbackPaintKit() = 416; // Doppler Sapphire
//						}
//						else if (Skin == 22)
//						{
//							pWeapon->m_nFallbackPaintKit() = 417; // Doppler Blackpearl
//						}
//						else if (Skin == 23)
//						{
//							pWeapon->m_nFallbackPaintKit() = 418; // Doppler Phase 1
//						}
//						else if (Skin == 24)
//						{
//							pWeapon->m_nFallbackPaintKit() = 419; // Doppler Phase 2
//						}
//						else if (Skin == 25)
//						{
//							pWeapon->m_nFallbackPaintKit() = 420; // Doppler Phase 3
//						}
//						else if (Skin == 26)
//						{
//							pWeapon->m_nFallbackPaintKit() = 421; // Doppler Phase 4
//						}
//						else if (Skin == 27)
//						{
//							pWeapon->m_nFallbackPaintKit() = 569; // Gamma Doppler Phase1
//						}
//						else if (Skin == 28)
//						{
//							pWeapon->m_nFallbackPaintKit() = 570; // Gamma Doppler Phase2
//						}
//						else if (Skin == 29)
//						{
//							pWeapon->m_nFallbackPaintKit() = 571; // Gamma Doppler Phase3
//						}
//						else if (Skin == 30)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Gamma Doppler Phase4
//						}
//						else if (Skin == 31)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Gamma Doppler Emerald
//						}
//						else if (Skin == 32)
//						{
//							pWeapon->m_nFallbackPaintKit() = 558; // Lore
//						}
//						else if (Skin == 33)
//						{
//							pWeapon->m_nFallbackPaintKit() = 567; // Black Laminate
//						}
//						else if (Skin == 34)
//						{
//							pWeapon->m_nFallbackPaintKit() = 574; // Autotronic
//						}
//						else if (Skin == 35)
//						{
//							pWeapon->m_nFallbackPaintKit() = 361; // Freehand
//						}
//					}
//					else if (Model == 7) // Falchion Knife
//					{
//						int iFalchion = g_csgo.m_modelinfo()->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
//						pWeapon->m_nModelIndex() = iFalchion; // m_nModelIndex
//						pWeapon->m_iViewModelIndex() = iFalchion;
//						if (worldmodel) worldmodel->m_nModelIndex() = iFalchion + 1;
//						*pWeapon->fixskins() = 512;
//						pWeapon->m_iEntityQuality() = 3;
//						killIcons.clear();
//						killIcons["knife_default_ct"] = "knife_falchion";
//						killIcons["knife_t"] = "knife_falchion";
//						int Skin = g_cfg.skins.KnifeSkin;
//						if (Skin == 0)
//						{
//							pWeapon->m_nFallbackPaintKit() = 0; // Forest DDPAT
//						}
//						else if (Skin == 1)
//						{
//							pWeapon->m_nFallbackPaintKit() = 12; // Crimson Web
//						}
//						else if (Skin == 2)
//						{
//							pWeapon->m_nFallbackPaintKit() = 27; // Bone Mask
//						}
//						else if (Skin == 3)
//						{
//							pWeapon->m_nFallbackPaintKit() = 38; // Fade
//						}
//						else if (Skin == 4)
//						{
//							pWeapon->m_nFallbackPaintKit() = 40; // Night
//						}
//						else if (Skin == 5)
//						{
//							pWeapon->m_nFallbackPaintKit() = 42; // Blue Steel
//						}
//						else if (Skin == 6)
//						{
//							pWeapon->m_nFallbackPaintKit() = 43; // Stained
//						}
//						else if (Skin == 7)
//						{
//							pWeapon->m_nFallbackPaintKit() = 44; // Case Hardened
//						}
//						else if (Skin == 8)
//						{
//							pWeapon->m_nFallbackPaintKit() = 59; // Slaughter
//						}
//						else if (Skin == 9)
//						{
//							pWeapon->m_nFallbackPaintKit() = 72; // Safari Mesh
//						}
//						else if (Skin == 10)
//						{
//							pWeapon->m_nFallbackPaintKit() = 77; // Boreal Forest
//						}
//						else if (Skin == 11)
//						{
//							pWeapon->m_nFallbackPaintKit() = 98; // Ultraviolet
//						}
//						else if (Skin == 12)
//						{
//							pWeapon->m_nFallbackPaintKit() = 143; // Urban Masked
//						}
//						else if (Skin == 13)
//						{
//							pWeapon->m_nFallbackPaintKit() = 175; // Scorched
//						}
//						else if (Skin == 14)
//						{
//							pWeapon->m_nFallbackPaintKit() = 323; // Rust Coat
//						}
//						else if (Skin == 15)
//						{
//							pWeapon->m_nFallbackPaintKit() = 409; // Tiger Tooth
//						}
//						else if (Skin == 16)
//						{
//							pWeapon->m_nFallbackPaintKit() = 410; // Damascus Steel
//						}
//						else if (Skin == 17)
//						{
//							pWeapon->m_nFallbackPaintKit() = 411; // Damascus Steel
//						}
//						else if (Skin == 18)
//						{
//							pWeapon->m_nFallbackPaintKit() = 413; // Marble Fade
//						}
//						else if (Skin == 19)
//						{
//							pWeapon->m_nFallbackPaintKit() = 414; // Rust Coat
//						}
//						else if (Skin == 20)
//						{
//							pWeapon->m_nFallbackPaintKit() = 415; // Doppler Ruby
//						}
//						else if (Skin == 21)
//						{
//							pWeapon->m_nFallbackPaintKit() = 416; // Doppler Sapphire
//						}
//						else if (Skin == 22)
//						{
//							pWeapon->m_nFallbackPaintKit() = 417; // Doppler Blackpearl
//						}
//						else if (Skin == 23)
//						{
//							pWeapon->m_nFallbackPaintKit() = 418; // Doppler Phase 1
//						}
//						else if (Skin == 24)
//						{
//							pWeapon->m_nFallbackPaintKit() = 419; // Doppler Phase 2
//						}
//						else if (Skin == 25)
//						{
//							pWeapon->m_nFallbackPaintKit() = 420; // Doppler Phase 3
//						}
//						else if (Skin == 26)
//						{
//							pWeapon->m_nFallbackPaintKit() = 421; // Doppler Phase 4
//						}
//						else if (Skin == 27)
//						{
//							pWeapon->m_nFallbackPaintKit() = 569; // Gamma Doppler Phase1
//						}
//						else if (Skin == 28)
//						{
//							pWeapon->m_nFallbackPaintKit() = 570; // Gamma Doppler Phase2
//						}
//						else if (Skin == 29)
//						{
//							pWeapon->m_nFallbackPaintKit() = 571; // Gamma Doppler Phase3
//						}
//						else if (Skin == 30)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Gamma Doppler Phase4
//						}
//						else if (Skin == 31)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Gamma Doppler Emerald
//						}
//						else if (Skin == 32)
//						{
//							pWeapon->m_nFallbackPaintKit() = 558; // Lore
//						}
//						else if (Skin == 33)
//						{
//							pWeapon->m_nFallbackPaintKit() = 567; // Black Laminate
//						}
//						else if (Skin == 34)
//						{
//							pWeapon->m_nFallbackPaintKit() = 574; // Autotronic
//						}
//						else if (Skin == 35)
//						{
//							pWeapon->m_nFallbackPaintKit() = 361; // Freehand
//						}
//					}
//					else if (Model == 1) // Flip Knife
//					{
//						int iFlip = g_csgo.m_modelinfo()->GetModelIndex("models/weapons/v_knife_flip.mdl");
//						pWeapon->m_nModelIndex() = iFlip; // m_nModelIndex
//						pWeapon->m_iViewModelIndex() = iFlip;
//						if (worldmodel) worldmodel->m_nModelIndex() = iFlip + 1;
//						*pWeapon->fixskins() = 505;
//						pWeapon->m_iEntityQuality() = 3;
//						killIcons.clear();
//						killIcons["knife_default_ct"] = "knife_flip";
//						killIcons["knife_t"] = "knife_flip";
//						int Skin = g_cfg.skins.KnifeSkin;
//						if (Skin == 0)
//						{
//							pWeapon->m_nFallbackPaintKit() = 0; // Forest DDPAT
//						}
//						else if (Skin == 1)
//						{
//							pWeapon->m_nFallbackPaintKit() = 12; // Crimson Web
//						}
//						else if (Skin == 2)
//						{
//							pWeapon->m_nFallbackPaintKit() = 27; // Bone Mask
//						}
//						else if (Skin == 3)
//						{
//							pWeapon->m_nFallbackPaintKit() = 38; // Fade
//						}
//						else if (Skin == 4)
//						{
//							pWeapon->m_nFallbackPaintKit() = 40; // Night
//						}
//						else if (Skin == 5)
//						{
//							pWeapon->m_nFallbackPaintKit() = 42; // Blue Steel
//						}
//						else if (Skin == 6)
//						{
//							pWeapon->m_nFallbackPaintKit() = 43; // Stained
//						}
//						else if (Skin == 7)
//						{
//							pWeapon->m_nFallbackPaintKit() = 44; // Case Hardened
//						}
//						else if (Skin == 8)
//						{
//							pWeapon->m_nFallbackPaintKit() = 59; // Slaughter
//						}
//						else if (Skin == 9)
//						{
//							pWeapon->m_nFallbackPaintKit() = 72; // Safari Mesh
//						}
//						else if (Skin == 10)
//						{
//							pWeapon->m_nFallbackPaintKit() = 77; // Boreal Forest
//						}
//						else if (Skin == 11)
//						{
//							pWeapon->m_nFallbackPaintKit() = 98; // Ultraviolet
//						}
//						else if (Skin == 12)
//						{
//							pWeapon->m_nFallbackPaintKit() = 143; // Urban Masked
//						}
//						else if (Skin == 13)
//						{
//							pWeapon->m_nFallbackPaintKit() = 175; // Scorched
//						}
//						else if (Skin == 14)
//						{
//							pWeapon->m_nFallbackPaintKit() = 323; // Rust Coat
//						}
//						else if (Skin == 15)
//						{
//							pWeapon->m_nFallbackPaintKit() = 409; // Tiger Tooth
//						}
//						else if (Skin == 16)
//						{
//							pWeapon->m_nFallbackPaintKit() = 410; // Damascus Steel
//						}
//						else if (Skin == 17)
//						{
//							pWeapon->m_nFallbackPaintKit() = 411; // Damascus Steel
//						}
//						else if (Skin == 18)
//						{
//							pWeapon->m_nFallbackPaintKit() = 413; // Marble Fade
//						}
//						else if (Skin == 19)
//						{
//							pWeapon->m_nFallbackPaintKit() = 414; // Rust Coat
//						}
//						else if (Skin == 20)
//						{
//							pWeapon->m_nFallbackPaintKit() = 415; // Doppler Ruby
//						}
//						else if (Skin == 21)
//						{
//							pWeapon->m_nFallbackPaintKit() = 416; // Doppler Sapphire
//						}
//						else if (Skin == 22)
//						{
//							pWeapon->m_nFallbackPaintKit() = 417; // Doppler Blackpearl
//						}
//						else if (Skin == 23)
//						{
//							pWeapon->m_nFallbackPaintKit() = 418; // Doppler Phase 1
//						}
//						else if (Skin == 24)
//						{
//							pWeapon->m_nFallbackPaintKit() = 419; // Doppler Phase 2
//						}
//						else if (Skin == 25)
//						{
//							pWeapon->m_nFallbackPaintKit() = 420; // Doppler Phase 3
//						}
//						else if (Skin == 26)
//						{
//							pWeapon->m_nFallbackPaintKit() = 421; // Doppler Phase 4
//						}
//						else if (Skin == 27)
//						{
//							pWeapon->m_nFallbackPaintKit() = 569; // Gamma Doppler Phase1
//						}
//						else if (Skin == 28)
//						{
//							pWeapon->m_nFallbackPaintKit() = 570; // Gamma Doppler Phase2
//						}
//						else if (Skin == 29)
//						{
//							pWeapon->m_nFallbackPaintKit() = 571; // Gamma Doppler Phase3
//						}
//						else if (Skin == 30)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Gamma Doppler Phase4
//						}
//						else if (Skin == 31)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Gamma Doppler Emerald
//						}
//						else if (Skin == 32)
//						{
//							pWeapon->m_nFallbackPaintKit() = 559; // Lore
//						}
//						else if (Skin == 33)
//						{
//							pWeapon->m_nFallbackPaintKit() = 564; // Black Laminate
//						}
//						else if (Skin == 34)
//						{
//							pWeapon->m_nFallbackPaintKit() = 574; // Autotronic
//						}
//						else if (Skin == 35)
//						{
//							pWeapon->m_nFallbackPaintKit() = 361; // Freehand
//						}
//
//					}
//					else if (Model == 2) // Gut Knife
//					{
//						int iGut = g_csgo.m_modelinfo()->GetModelIndex("models/weapons/v_knife_gut.mdl");
//						pWeapon->m_nModelIndex() = iGut; // m_nModelIndex
//						pWeapon->m_iViewModelIndex() = iGut;
//						if (worldmodel) worldmodel->m_nModelIndex() = iGut + 1;
//						*pWeapon->fixskins() = 506;
//						pWeapon->m_iEntityQuality() = 3;
//						killIcons.clear();
//						killIcons["knife_default_ct"] = "knife_gut";
//						killIcons["knife_t"] = "knife_gut";
//						int Skin = g_cfg.skins.KnifeSkin;
//						if (Skin == 0)
//						{
//							pWeapon->m_nFallbackPaintKit() = 0; // Forest DDPAT
//						}
//						else if (Skin == 1)
//						{
//							pWeapon->m_nFallbackPaintKit() = 12; // Crimson Web
//						}
//						else if (Skin == 2)
//						{
//							pWeapon->m_nFallbackPaintKit() = 27; // Bone Mask
//						}
//						else if (Skin == 3)
//						{
//							pWeapon->m_nFallbackPaintKit() = 38; // Fade
//						}
//						else if (Skin == 4)
//						{
//							pWeapon->m_nFallbackPaintKit() = 40; // Night
//						}
//						else if (Skin == 5)
//						{
//							pWeapon->m_nFallbackPaintKit() = 42; // Blue Steel
//						}
//						else if (Skin == 6)
//						{
//							pWeapon->m_nFallbackPaintKit() = 43; // Stained
//						}
//						else if (Skin == 7)
//						{
//							pWeapon->m_nFallbackPaintKit() = 44; // Case Hardened
//						}
//						else if (Skin == 8)
//						{
//							pWeapon->m_nFallbackPaintKit() = 59; // Slaughter
//						}
//						else if (Skin == 9)
//						{
//							pWeapon->m_nFallbackPaintKit() = 72; // Safari Mesh
//						}
//						else if (Skin == 10)
//						{
//							pWeapon->m_nFallbackPaintKit() = 77; // Boreal Forest
//						}
//						else if (Skin == 11)
//						{
//							pWeapon->m_nFallbackPaintKit() = 98; // Ultraviolet
//						}
//						else if (Skin == 12)
//						{
//							pWeapon->m_nFallbackPaintKit() = 143; // Urban Masked
//						}
//						else if (Skin == 13)
//						{
//							pWeapon->m_nFallbackPaintKit() = 175; // Scorched
//						}
//						else if (Skin == 14)
//						{
//							pWeapon->m_nFallbackPaintKit() = 323; // Rust Coat
//						}
//						else if (Skin == 15)
//						{
//							pWeapon->m_nFallbackPaintKit() = 409; // Tiger Tooth
//						}
//						else if (Skin == 16)
//						{
//							pWeapon->m_nFallbackPaintKit() = 410; // Damascus Steel
//						}
//						else if (Skin == 17)
//						{
//							pWeapon->m_nFallbackPaintKit() = 411; // Damascus Steel
//						}
//						else if (Skin == 18)
//						{
//							pWeapon->m_nFallbackPaintKit() = 413; // Marble Fade
//						}
//						else if (Skin == 19)
//						{
//							pWeapon->m_nFallbackPaintKit() = 414; // Rust Coat
//						}
//						else if (Skin == 20)
//						{
//							pWeapon->m_nFallbackPaintKit() = 415; // Doppler Ruby
//						}
//						else if (Skin == 21)
//						{
//							pWeapon->m_nFallbackPaintKit() = 416; // Doppler Sapphire
//						}
//						else if (Skin == 22)
//						{
//							pWeapon->m_nFallbackPaintKit() = 417; // Doppler Blackpearl
//						}
//						else if (Skin == 23)
//						{
//							pWeapon->m_nFallbackPaintKit() = 418; // Doppler Phase 1
//						}
//						else if (Skin == 24)
//						{
//							pWeapon->m_nFallbackPaintKit() = 419; // Doppler Phase 2
//						}
//						else if (Skin == 25)
//						{
//							pWeapon->m_nFallbackPaintKit() = 420; // Doppler Phase 3
//						}
//						else if (Skin == 26)
//						{
//							pWeapon->m_nFallbackPaintKit() = 421; // Doppler Phase 4
//						}
//						else if (Skin == 27)
//						{
//							pWeapon->m_nFallbackPaintKit() = 569; // Gamma Doppler Phase1
//						}
//						else if (Skin == 28)
//						{
//							pWeapon->m_nFallbackPaintKit() = 570; // Gamma Doppler Phase2
//						}
//						else if (Skin == 29)
//						{
//							pWeapon->m_nFallbackPaintKit() = 571; // Gamma Doppler Phase3
//						}
//						else if (Skin == 30)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Gamma Doppler Phase4
//						}
//						else if (Skin == 31)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Gamma Doppler Emerald
//						}
//						else if (Skin == 32)
//						{
//							pWeapon->m_nFallbackPaintKit() = 560; // Lore
//						}
//						else if (Skin == 33)
//						{
//							pWeapon->m_nFallbackPaintKit() = 565; // Black Laminate
//						}
//						else if (Skin == 34)
//						{
//							pWeapon->m_nFallbackPaintKit() = 575; // Autotronic
//						}
//						else if (Skin == 35)
//						{
//							pWeapon->m_nFallbackPaintKit() = 361; // Freehand
//						}
//					}
//					else if (Model == 5) // Huntsman Knife
//					{
//						int iHuntsman = g_csgo.m_modelinfo()->GetModelIndex("models/weapons/v_knife_tactical.mdl");
//						pWeapon->m_nModelIndex() = iHuntsman; // m_nModelIndex
//						pWeapon->m_iViewModelIndex() = iHuntsman;
//						if (worldmodel) worldmodel->m_nModelIndex() = iHuntsman + 1;
//						*pWeapon->fixskins() = 509;
//						pWeapon->m_iEntityQuality() = 3;
//						killIcons.clear();
//						killIcons["knife_default_ct"] = "knife_tactical";
//						killIcons["knife_t"] = "knife_tactical";
//						int Skin = g_cfg.skins.KnifeSkin;
//						if (Skin == 0)
//						{
//							pWeapon->m_nFallbackPaintKit() = 0; // Forest DDPAT
//						}
//						else if (Skin == 1)
//						{
//							pWeapon->m_nFallbackPaintKit() = 12; // Crimson Web
//						}
//						else if (Skin == 2)
//						{
//							pWeapon->m_nFallbackPaintKit() = 27; // Bone Mask
//						}
//						else if (Skin == 3)
//						{
//							pWeapon->m_nFallbackPaintKit() = 38; // Fade
//						}
//						else if (Skin == 4)
//						{
//							pWeapon->m_nFallbackPaintKit() = 40; // Night
//						}
//						else if (Skin == 5)
//						{
//							pWeapon->m_nFallbackPaintKit() = 42; // Blue Steel
//						}
//						else if (Skin == 6)
//						{
//							pWeapon->m_nFallbackPaintKit() = 43; // Stained
//						}
//						else if (Skin == 7)
//						{
//							pWeapon->m_nFallbackPaintKit() = 44; // Case Hardened
//						}
//						else if (Skin == 8)
//						{
//							pWeapon->m_nFallbackPaintKit() = 59; // Slaughter
//						}
//						else if (Skin == 9)
//						{
//							pWeapon->m_nFallbackPaintKit() = 72; // Safari Mesh
//						}
//						else if (Skin == 10)
//						{
//							pWeapon->m_nFallbackPaintKit() = 77; // Boreal Forest
//						}
//						else if (Skin == 11)
//						{
//							pWeapon->m_nFallbackPaintKit() = 98; // Ultraviolet
//						}
//						else if (Skin == 12)
//						{
//							pWeapon->m_nFallbackPaintKit() = 143; // Urban Masked
//						}
//						else if (Skin == 13)
//						{
//							pWeapon->m_nFallbackPaintKit() = 175; // Scorched
//						}
//						else if (Skin == 14)
//						{
//							pWeapon->m_nFallbackPaintKit() = 323; // Rust Coat
//						}
//						else if (Skin == 15)
//						{
//							pWeapon->m_nFallbackPaintKit() = 409; // Tiger Tooth
//						}
//						else if (Skin == 16)
//						{
//							pWeapon->m_nFallbackPaintKit() = 410; // Damascus Steel
//						}
//						else if (Skin == 17)
//						{
//							pWeapon->m_nFallbackPaintKit() = 411; // Damascus Steel
//						}
//						else if (Skin == 18)
//						{
//							pWeapon->m_nFallbackPaintKit() = 413; // Marble Fade
//						}
//						else if (Skin == 19)
//						{
//							pWeapon->m_nFallbackPaintKit() = 414; // Rust Coat
//						}
//						else if (Skin == 20)
//						{
//							pWeapon->m_nFallbackPaintKit() = 415; // Doppler Ruby
//						}
//						else if (Skin == 21)
//						{
//							pWeapon->m_nFallbackPaintKit() = 416; // Doppler Sapphire
//						}
//						else if (Skin == 22)
//						{
//							pWeapon->m_nFallbackPaintKit() = 417; // Doppler Blackpearl
//						}
//						else if (Skin == 23)
//						{
//							pWeapon->m_nFallbackPaintKit() = 418; // Doppler Phase 1
//						}
//						else if (Skin == 24)
//						{
//							pWeapon->m_nFallbackPaintKit() = 419; // Doppler Phase 2
//						}
//						else if (Skin == 25)
//						{
//							pWeapon->m_nFallbackPaintKit() = 420; // Doppler Phase 3
//						}
//						else if (Skin == 26)
//						{
//							pWeapon->m_nFallbackPaintKit() = 421; // Doppler Phase 4
//						}
//						else if (Skin == 27)
//						{
//							pWeapon->m_nFallbackPaintKit() = 569; // Gamma Doppler Phase1
//						}
//						else if (Skin == 28)
//						{
//							pWeapon->m_nFallbackPaintKit() = 570; // Gamma Doppler Phase2
//						}
//						else if (Skin == 29)
//						{
//							pWeapon->m_nFallbackPaintKit() = 571; // Gamma Doppler Phase3
//						}
//						else if (Skin == 30)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Gamma Doppler Phase4
//						}
//						else if (Skin == 31)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Gamma Doppler Emerald
//						}
//						else if (Skin == 32)
//						{
//							pWeapon->m_nFallbackPaintKit() = 559; // Lore
//						}
//						else if (Skin == 33)
//						{
//							pWeapon->m_nFallbackPaintKit() = 567; // Black Laminate
//						}
//						else if (Skin == 34)
//						{
//							pWeapon->m_nFallbackPaintKit() = 574; // Autotronic
//						}
//						else if (Skin == 35)
//						{
//							pWeapon->m_nFallbackPaintKit() = 361; // Freehand
//						}
//					}
//					else if (Model == 3) // Karambit
//					{
//						int iKarambit = g_csgo.m_modelinfo()->GetModelIndex("models/weapons/v_knife_karam.mdl");
//						pWeapon->m_nModelIndex() = iKarambit; // m_nModelIndex
//						pWeapon->m_iViewModelIndex() = iKarambit;
//						if (worldmodel) worldmodel->m_nModelIndex() = iKarambit + 1;
//						*pWeapon->fixskins() = 507;
//						pWeapon->m_iEntityQuality() = 3;
//						killIcons.clear();
//						killIcons["knife_default_ct"] = "knife_karambit";
//						killIcons["knife_t"] = "knife_karambit";
//						int Skin = g_cfg.skins.KnifeSkin;
//						if (Skin == 0)
//						{
//							pWeapon->m_nFallbackPaintKit() = 0; // Forest DDPAT
//						}
//						else if (Skin == 1)
//						{
//							pWeapon->m_nFallbackPaintKit() = 12; // Crimson Web
//						}
//						else if (Skin == 2)
//						{
//							pWeapon->m_nFallbackPaintKit() = 27; // Bone Mask
//						}
//						else if (Skin == 3)
//						{
//							pWeapon->m_nFallbackPaintKit() = 38; // Fade
//						}
//						else if (Skin == 4)
//						{
//							pWeapon->m_nFallbackPaintKit() = 40; // Night
//						}
//						else if (Skin == 5)
//						{
//							pWeapon->m_nFallbackPaintKit() = 42; // Blue Steel
//						}
//						else if (Skin == 6)
//						{
//							pWeapon->m_nFallbackPaintKit() = 43; // Stained
//						}
//						else if (Skin == 7)
//						{
//							pWeapon->m_nFallbackPaintKit() = 44; // Case Hardened
//						}
//						else if (Skin == 8)
//						{
//							pWeapon->m_nFallbackPaintKit() = 59; // Slaughter
//						}
//						else if (Skin == 9)
//						{
//							pWeapon->m_nFallbackPaintKit() = 72; // Safari Mesh
//						}
//						else if (Skin == 10)
//						{
//							pWeapon->m_nFallbackPaintKit() = 77; // Boreal Forest
//						}
//						else if (Skin == 11)
//						{
//							pWeapon->m_nFallbackPaintKit() = 98; // Ultraviolet
//						}
//						else if (Skin == 12)
//						{
//							pWeapon->m_nFallbackPaintKit() = 143; // Urban Masked
//						}
//						else if (Skin == 13)
//						{
//							pWeapon->m_nFallbackPaintKit() = 175; // Scorched
//						}
//						else if (Skin == 14)
//						{
//							pWeapon->m_nFallbackPaintKit() = 323; // Rust Coat
//						}
//						else if (Skin == 15)
//						{
//							pWeapon->m_nFallbackPaintKit() = 409; // Tiger Tooth
//						}
//						else if (Skin == 16)
//						{
//							pWeapon->m_nFallbackPaintKit() = 410; // Damascus Steel
//						}
//						else if (Skin == 17)
//						{
//							pWeapon->m_nFallbackPaintKit() = 411; // Damascus Steel
//						}
//						else if (Skin == 18)
//						{
//							pWeapon->m_nFallbackPaintKit() = 413; // Marble Fade
//						}
//						else if (Skin == 19)
//						{
//							pWeapon->m_nFallbackPaintKit() = 414; // Rust Coat
//						}
//						else if (Skin == 20)
//						{
//							pWeapon->m_nFallbackPaintKit() = 415; // Doppler Ruby
//						}
//						else if (Skin == 21)
//						{
//							pWeapon->m_nFallbackPaintKit() = 416; // Doppler Sapphire
//						}
//						else if (Skin == 22)
//						{
//							pWeapon->m_nFallbackPaintKit() = 417; // Doppler Blackpearl
//						}
//						else if (Skin == 23)
//						{
//							pWeapon->m_nFallbackPaintKit() = 418; // Doppler Phase 1
//						}
//						else if (Skin == 24)
//						{
//							pWeapon->m_nFallbackPaintKit() = 419; // Doppler Phase 2
//						}
//						else if (Skin == 25)
//						{
//							pWeapon->m_nFallbackPaintKit() = 420; // Doppler Phase 3
//						}
//						else if (Skin == 26)
//						{
//							pWeapon->m_nFallbackPaintKit() = 421; // Doppler Phase 4
//						}
//						else if (Skin == 27)
//						{
//							pWeapon->m_nFallbackPaintKit() = 570; // Doppler Phase 4
//						}
//						else if (Skin == 28)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Doppler Phase 4
//						}
//						else if (Skin == 27)
//						{
//							pWeapon->m_nFallbackPaintKit() = 569; // Gamma Doppler Phase1
//						}
//						else if (Skin == 28)
//						{
//							pWeapon->m_nFallbackPaintKit() = 570; // Gamma Doppler Phase2
//						}
//						else if (Skin == 29)
//						{
//							pWeapon->m_nFallbackPaintKit() = 571; // Gamma Doppler Phase3
//						}
//						else if (Skin == 30)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Gamma Doppler Phase4
//						}
//						else if (Skin == 31)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Gamma Doppler Emerald
//						}
//						else if (Skin == 32)
//						{
//							pWeapon->m_nFallbackPaintKit() = 561; // Lore
//						}
//						else if (Skin == 33)
//						{
//							pWeapon->m_nFallbackPaintKit() = 566; // Black Laminate
//						}
//						else if (Skin == 34)
//						{
//							pWeapon->m_nFallbackPaintKit() = 576; // Autotronic
//						}
//						else if (Skin == 35)
//						{
//							pWeapon->m_nFallbackPaintKit() = 361; // Freehand
//						}
//
//					}
//					else if (Model == 4) // M9 Bayonet
//					{
//						auto iM9Bayonet = g_csgo.m_modelinfo()->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
//						pWeapon->m_nModelIndex() = iM9Bayonet; // m_nModelIndex
//						pWeapon->m_iViewModelIndex() = iM9Bayonet;
//						if (worldmodel) worldmodel->m_nModelIndex() = iM9Bayonet + 1;
//						*pWeapon->fixskins() = 508;
//						pWeapon->m_iEntityQuality() = 3;
//						killIcons.clear();
//						killIcons["knife_default_ct"] = "knife_m9_bayonet";
//						killIcons["knife_t"] = "knife_m9_bayonet";
//						int Skin = g_cfg.skins.KnifeSkin;
//						if (Skin == 0)
//						{
//							pWeapon->m_nFallbackPaintKit() = 0; // Forest DDPAT
//						}
//						else if (Skin == 1)
//						{
//							pWeapon->m_nFallbackPaintKit() = 12; // Crimson Web
//						}
//						else if (Skin == 2)
//						{
//							pWeapon->m_nFallbackPaintKit() = 27; // Bone Mask
//						}
//						else if (Skin == 3)
//						{
//							pWeapon->m_nFallbackPaintKit() = 38; // Fade
//						}
//						else if (Skin == 4)
//						{
//							pWeapon->m_nFallbackPaintKit() = 40; // Night
//						}
//						else if (Skin == 5)
//						{
//							pWeapon->m_nFallbackPaintKit() = 42; // Blue Steel
//						}
//						else if (Skin == 6)
//						{
//							pWeapon->m_nFallbackPaintKit() = 43; // Stained
//						}
//						else if (Skin == 7)
//						{
//							pWeapon->m_nFallbackPaintKit() = 44; // Case Hardened
//						}
//						else if (Skin == 8)
//						{
//							pWeapon->m_nFallbackPaintKit() = 59; // Slaughter
//						}
//						else if (Skin == 9)
//						{
//							pWeapon->m_nFallbackPaintKit() = 72; // Safari Mesh
//						}
//						else if (Skin == 10)
//						{
//							pWeapon->m_nFallbackPaintKit() = 77; // Boreal Forest
//						}
//						else if (Skin == 11)
//						{
//							pWeapon->m_nFallbackPaintKit() = 98; // Ultraviolet
//						}
//						else if (Skin == 12)
//						{
//							pWeapon->m_nFallbackPaintKit() = 143; // Urban Masked
//						}
//						else if (Skin == 13)
//						{
//							pWeapon->m_nFallbackPaintKit() = 175; // Scorched
//						}
//						else if (Skin == 14)
//						{
//							pWeapon->m_nFallbackPaintKit() = 323; // Rust Coat
//						}
//						else if (Skin == 15)
//						{
//							pWeapon->m_nFallbackPaintKit() = 409; // Tiger Tooth
//						}
//						else if (Skin == 16)
//						{
//							pWeapon->m_nFallbackPaintKit() = 410; // Damascus Steel
//						}
//						else if (Skin == 17)
//						{
//							pWeapon->m_nFallbackPaintKit() = 411; // Damascus Steel
//						}
//						else if (Skin == 18)
//						{
//							pWeapon->m_nFallbackPaintKit() = 413; // Marble Fade
//						}
//						else if (Skin == 19)
//						{
//							pWeapon->m_nFallbackPaintKit() = 414; // Rust Coat
//						}
//						else if (Skin == 20)
//						{
//							pWeapon->m_nFallbackPaintKit() = 415; // Doppler Ruby
//						}
//						else if (Skin == 21)
//						{
//							pWeapon->m_nFallbackPaintKit() = 416; // Doppler Sapphire
//						}
//						else if (Skin == 22)
//						{
//							pWeapon->m_nFallbackPaintKit() = 417; // Doppler Blackpearl
//						}
//						else if (Skin == 23)
//						{
//							pWeapon->m_nFallbackPaintKit() = 418; // Doppler Phase 1
//						}
//						else if (Skin == 24)
//						{
//							pWeapon->m_nFallbackPaintKit() = 419; // Doppler Phase 2
//						}
//						else if (Skin == 25)
//						{
//							pWeapon->m_nFallbackPaintKit() = 420; // Doppler Phase 3
//						}
//						else if (Skin == 26)
//						{
//							pWeapon->m_nFallbackPaintKit() = 421; // Doppler Phase 4
//						}
//						else if (Skin == 27)
//						{
//							pWeapon->m_nFallbackPaintKit() = 570; // Doppler Phase 4
//						}
//						else if (Skin == 28)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Doppler Phase 4
//						}
//						else if (Skin == 27)
//						{
//							pWeapon->m_nFallbackPaintKit() = 569; // Gamma Doppler Phase1
//						}
//						else if (Skin == 28)
//						{
//							pWeapon->m_nFallbackPaintKit() = 570; // Gamma Doppler Phase2
//						}
//						else if (Skin == 29)
//						{
//							pWeapon->m_nFallbackPaintKit() = 571; // Gamma Doppler Phase3
//						}
//						else if (Skin == 30)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Gamma Doppler Phase4
//						}
//						else if (Skin == 31)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Gamma Doppler Emerald
//						}
//						else if (Skin == 32)
//						{
//							pWeapon->m_nFallbackPaintKit() = 562; // Lore
//						}
//						else if (Skin == 33)
//						{
//							pWeapon->m_nFallbackPaintKit() = 567; // Black Laminate
//						}
//						else if (Skin == 34)
//						{
//							pWeapon->m_nFallbackPaintKit() = 577; // Autotronic
//						}
//						else if (Skin == 35)
//						{
//							pWeapon->m_nFallbackPaintKit() = 361; // Freehand
//						}
//
//					}
//					else if (Model == 8)
//					{
//						int iDagger = g_csgo.m_modelinfo()->GetModelIndex("models/weapons/v_knife_push.mdl");
//						pWeapon->m_nModelIndex() = iDagger; // m_nModelIndex
//						pWeapon->m_iViewModelIndex() = iDagger;
//						if (worldmodel) worldmodel->m_nModelIndex() = iDagger + 1;
//						*pWeapon->fixskins() = 516;
//						pWeapon->m_iEntityQuality() = 3;
//						killIcons.clear();
//						killIcons["knife_default_ct"] = "knife_push";
//						killIcons["knife_t"] = "knife_push";
//						int Skin = g_cfg.skins.KnifeSkin;
//						if (Skin == 0)
//						{
//							pWeapon->m_nFallbackPaintKit() = 5; // Forest DDPAT
//						}
//						else if (Skin == 1)
//						{
//							pWeapon->m_nFallbackPaintKit() = 12; // Crimson Web
//						}
//						else if (Skin == 2)
//						{
//							pWeapon->m_nFallbackPaintKit() = 27; // Bone Mask
//						}
//						else if (Skin == 3)
//						{
//							pWeapon->m_nFallbackPaintKit() = 38; // Fade
//						}
//						else if (Skin == 4)
//						{
//							pWeapon->m_nFallbackPaintKit() = 40; // Night
//						}
//						else if (Skin == 5)
//						{
//							pWeapon->m_nFallbackPaintKit() = 42; // Blue Steel
//						}
//						else if (Skin == 6)
//						{
//							pWeapon->m_nFallbackPaintKit() = 43; // Stained
//						}
//						else if (Skin == 7)
//						{
//							pWeapon->m_nFallbackPaintKit() = 44; // Case Hardened
//						}
//						else if (Skin == 8)
//						{
//							pWeapon->m_nFallbackPaintKit() = 59; // Slaughter
//						}
//						else if (Skin == 9)
//						{
//							pWeapon->m_nFallbackPaintKit() = 72; // Safari Mesh
//						}
//						else if (Skin == 10)
//						{
//							pWeapon->m_nFallbackPaintKit() = 77; // Boreal Forest
//						}
//						else if (Skin == 11)
//						{
//							pWeapon->m_nFallbackPaintKit() = 98; // Ultraviolet
//						}
//						else if (Skin == 12)
//						{
//							pWeapon->m_nFallbackPaintKit() = 143; // Urban Masked
//						}
//						else if (Skin == 13)
//						{
//							pWeapon->m_nFallbackPaintKit() = 175; // Scorched
//						}
//						else if (Skin == 14)
//						{
//							pWeapon->m_nFallbackPaintKit() = 323; // Rust Coat
//						}
//						else if (Skin == 15)
//						{
//							pWeapon->m_nFallbackPaintKit() = 409; // Tiger Tooth
//						}
//						else if (Skin == 16)
//						{
//							pWeapon->m_nFallbackPaintKit() = 410; // Damascus Steel
//						}
//						else if (Skin == 17)
//						{
//							pWeapon->m_nFallbackPaintKit() = 411; // Damascus Steel
//						}
//						else if (Skin == 18)
//						{
//							pWeapon->m_nFallbackPaintKit() = 413; // Marble Fade
//						}
//						else if (Skin == 19)
//						{
//							pWeapon->m_nFallbackPaintKit() = 414; // Rust Coat
//						}
//						else if (Skin == 20)
//						{
//							pWeapon->m_nFallbackPaintKit() = 415; // Doppler Ruby
//						}
//						else if (Skin == 21)
//						{
//							pWeapon->m_nFallbackPaintKit() = 416; // Doppler Sapphire
//						}
//						else if (Skin == 22)
//						{
//							pWeapon->m_nFallbackPaintKit() = 417; // Doppler Blackpearl
//						}
//						else if (Skin == 23)
//						{
//							pWeapon->m_nFallbackPaintKit() = 418; // Doppler Phase 1
//						}
//						else if (Skin == 24)
//						{
//							pWeapon->m_nFallbackPaintKit() = 419; // Doppler Phase 2
//						}
//						else if (Skin == 25)
//						{
//							pWeapon->m_nFallbackPaintKit() = 420; // Doppler Phase 3
//						}
//						else if (Skin == 26)
//						{
//							pWeapon->m_nFallbackPaintKit() = 421; // Doppler Phase 4
//						}
//						else if (Skin == 27)
//						{
//							pWeapon->m_nFallbackPaintKit() = 569; // Gamma Doppler Phase1
//						}
//						else if (Skin == 28)
//						{
//							pWeapon->m_nFallbackPaintKit() = 570; // Gamma Doppler Phase2
//						}
//						else if (Skin == 29)
//						{
//							pWeapon->m_nFallbackPaintKit() = 571; // Gamma Doppler Phase3
//						}
//						else if (Skin == 30)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Gamma Doppler Phase4
//						}
//						else if (Skin == 31)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Gamma Doppler Emerald
//						}
//						else if (Skin == 32)
//						{
//							pWeapon->m_nFallbackPaintKit() = 561; // Lore
//						}
//						else if (Skin == 33)
//						{
//							pWeapon->m_nFallbackPaintKit() = 567; // Black Laminate
//						}
//						else if (Skin == 34)
//						{
//							pWeapon->m_nFallbackPaintKit() = 574; // Autotronic
//						}
//						else if (Skin == 35)
//						{
//							pWeapon->m_nFallbackPaintKit() = 361; // Freehand
//						}
//					}
//					else if (Model == 10)
//					{
//						int Navaja = g_csgo.m_modelinfo()->GetModelIndex("models/weapons/v_knife_gypsy_jackknife.mdl");
//						pWeapon->m_nModelIndex() = Navaja; // m_nModelIndex
//						pWeapon->m_iViewModelIndex() = Navaja;
//						if (worldmodel) worldmodel->m_nModelIndex() = Navaja + 1;
//						*pWeapon->fixskins() = 520;
//						pWeapon->m_iEntityQuality() = 3;
//						killIcons.clear();
//						killIcons["knife_default_ct"] = "knife_push";
//						killIcons["knife_t"] = "knife_push";
//						int Skin = g_cfg.skins.KnifeSkin;
//						if (Skin == 0)
//						{
//							pWeapon->m_nFallbackPaintKit() = 5; // Forest DDPAT
//						}
//						else if (Skin == 1)
//						{
//							pWeapon->m_nFallbackPaintKit() = 12; // Crimson Web
//						}
//						else if (Skin == 2)
//						{
//							pWeapon->m_nFallbackPaintKit() = 27; // Bone Mask
//						}
//						else if (Skin == 3)
//						{
//							pWeapon->m_nFallbackPaintKit() = 38; // Fade
//						}
//						else if (Skin == 4)
//						{
//							pWeapon->m_nFallbackPaintKit() = 40; // Night
//						}
//						else if (Skin == 5)
//						{
//							pWeapon->m_nFallbackPaintKit() = 42; // Blue Steel
//						}
//						else if (Skin == 6)
//						{
//							pWeapon->m_nFallbackPaintKit() = 43; // Stained
//						}
//						else if (Skin == 7)
//						{
//							pWeapon->m_nFallbackPaintKit() = 44; // Case Hardened
//						}
//						else if (Skin == 8)
//						{
//							pWeapon->m_nFallbackPaintKit() = 59; // Slaughter
//						}
//						else if (Skin == 9)
//						{
//							pWeapon->m_nFallbackPaintKit() = 72; // Safari Mesh
//						}
//						else if (Skin == 10)
//						{
//							pWeapon->m_nFallbackPaintKit() = 77; // Boreal Forest
//						}
//						else if (Skin == 11)
//						{
//							pWeapon->m_nFallbackPaintKit() = 98; // Ultraviolet
//						}
//						else if (Skin == 12)
//						{
//							pWeapon->m_nFallbackPaintKit() = 143; // Urban Masked
//						}
//						else if (Skin == 13)
//						{
//							pWeapon->m_nFallbackPaintKit() = 175; // Scorched
//						}
//						else if (Skin == 14)
//						{
//							pWeapon->m_nFallbackPaintKit() = 323; // Rust Coat
//						}
//						else if (Skin == 15)
//						{
//							pWeapon->m_nFallbackPaintKit() = 409; // Tiger Tooth
//						}
//						else if (Skin == 16)
//						{
//							pWeapon->m_nFallbackPaintKit() = 410; // Damascus Steel
//						}
//						else if (Skin == 17)
//						{
//							pWeapon->m_nFallbackPaintKit() = 411; // Damascus Steel
//						}
//						else if (Skin == 18)
//						{
//							pWeapon->m_nFallbackPaintKit() = 413; // Marble Fade
//						}
//						else if (Skin == 19)
//						{
//							pWeapon->m_nFallbackPaintKit() = 414; // Rust Coat
//						}
//						else if (Skin == 20)
//						{
//							pWeapon->m_nFallbackPaintKit() = 415; // Doppler Ruby
//						}
//						else if (Skin == 21)
//						{
//							pWeapon->m_nFallbackPaintKit() = 416; // Doppler Sapphire
//						}
//						else if (Skin == 22)
//						{
//							pWeapon->m_nFallbackPaintKit() = 417; // Doppler Blackpearl
//						}
//						else if (Skin == 23)
//						{
//							pWeapon->m_nFallbackPaintKit() = 418; // Doppler Phase 1
//						}
//						else if (Skin == 24)
//						{
//							pWeapon->m_nFallbackPaintKit() = 419; // Doppler Phase 2
//						}
//						else if (Skin == 25)
//						{
//							pWeapon->m_nFallbackPaintKit() = 420; // Doppler Phase 3
//						}
//						else if (Skin == 26)
//						{
//							pWeapon->m_nFallbackPaintKit() = 421; // Doppler Phase 4
//						}
//						else if (Skin == 27)
//						{
//							pWeapon->m_nFallbackPaintKit() = 569; // Gamma Doppler Phase1
//						}
//						else if (Skin == 28)
//						{
//							pWeapon->m_nFallbackPaintKit() = 570; // Gamma Doppler Phase2
//						}
//						else if (Skin == 29)
//						{
//							pWeapon->m_nFallbackPaintKit() = 571; // Gamma Doppler Phase3
//						}
//						else if (Skin == 30)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Gamma Doppler Phase4
//						}
//						else if (Skin == 31)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Gamma Doppler Emerald
//						}
//						else if (Skin == 32)
//						{
//							pWeapon->m_nFallbackPaintKit() = 561; // Lore
//						}
//						else if (Skin == 33)
//						{
//							pWeapon->m_nFallbackPaintKit() = 567; // Black Laminate
//						}
//						else if (Skin == 34)
//						{
//							pWeapon->m_nFallbackPaintKit() = 574; // Autotronic
//						}
//						else if (Skin == 35)
//						{
//							pWeapon->m_nFallbackPaintKit() = 361; // Freehand
//						}
//					}
//					else if (Model == 11) // Shadow Daggers
//					{
//						int Stiletto = g_csgo.m_modelinfo()->GetModelIndex("models/weapons/v_knife_stiletto.mdl");
//						pWeapon->m_nModelIndex() = Stiletto; // m_nModelIndex
//						pWeapon->m_iViewModelIndex() = Stiletto;
//						if (worldmodel) worldmodel->m_nModelIndex() = Stiletto + 1;
//						*pWeapon->fixskins() = 522;
//						pWeapon->m_iEntityQuality() = 3;
//						killIcons.clear();
//						killIcons["knife_default_ct"] = "knife_push";
//						killIcons["knife_t"] = "knife_push";
//						int Skin = g_cfg.skins.KnifeSkin;
//						if (Skin == 0)
//						{
//							pWeapon->m_nFallbackPaintKit() = 5; // Forest DDPAT
//						}
//						else if (Skin == 1)
//						{
//							pWeapon->m_nFallbackPaintKit() = 12; // Crimson Web
//						}
//						else if (Skin == 2)
//						{
//							pWeapon->m_nFallbackPaintKit() = 27; // Bone Mask
//						}
//						else if (Skin == 3)
//						{
//							pWeapon->m_nFallbackPaintKit() = 38; // Fade
//						}
//						else if (Skin == 4)
//						{
//							pWeapon->m_nFallbackPaintKit() = 40; // Night
//						}
//						else if (Skin == 5)
//						{
//							pWeapon->m_nFallbackPaintKit() = 42; // Blue Steel
//						}
//						else if (Skin == 6)
//						{
//							pWeapon->m_nFallbackPaintKit() = 43; // Stained
//						}
//						else if (Skin == 7)
//						{
//							pWeapon->m_nFallbackPaintKit() = 44; // Case Hardened
//						}
//						else if (Skin == 8)
//						{
//							pWeapon->m_nFallbackPaintKit() = 59; // Slaughter
//						}
//						else if (Skin == 9)
//						{
//							pWeapon->m_nFallbackPaintKit() = 72; // Safari Mesh
//						}
//						else if (Skin == 10)
//						{
//							pWeapon->m_nFallbackPaintKit() = 77; // Boreal Forest
//						}
//						else if (Skin == 11)
//						{
//							pWeapon->m_nFallbackPaintKit() = 98; // Ultraviolet
//						}
//						else if (Skin == 12)
//						{
//							pWeapon->m_nFallbackPaintKit() = 143; // Urban Masked
//						}
//						else if (Skin == 13)
//						{
//							pWeapon->m_nFallbackPaintKit() = 175; // Scorched
//						}
//						else if (Skin == 14)
//						{
//							pWeapon->m_nFallbackPaintKit() = 323; // Rust Coat
//						}
//						else if (Skin == 15)
//						{
//							pWeapon->m_nFallbackPaintKit() = 409; // Tiger Tooth
//						}
//						else if (Skin == 16)
//						{
//							pWeapon->m_nFallbackPaintKit() = 410; // Damascus Steel
//						}
//						else if (Skin == 17)
//						{
//							pWeapon->m_nFallbackPaintKit() = 411; // Damascus Steel
//						}
//						else if (Skin == 18)
//						{
//							pWeapon->m_nFallbackPaintKit() = 413; // Marble Fade
//						}
//						else if (Skin == 19)
//						{
//							pWeapon->m_nFallbackPaintKit() = 414; // Rust Coat
//						}
//						else if (Skin == 20)
//						{
//							pWeapon->m_nFallbackPaintKit() = 415; // Doppler Ruby
//						}
//						else if (Skin == 21)
//						{
//							pWeapon->m_nFallbackPaintKit() = 416; // Doppler Sapphire
//						}
//						else if (Skin == 22)
//						{
//							pWeapon->m_nFallbackPaintKit() = 417; // Doppler Blackpearl
//						}
//						else if (Skin == 23)
//						{
//							pWeapon->m_nFallbackPaintKit() = 418; // Doppler Phase 1
//						}
//						else if (Skin == 24)
//						{
//							pWeapon->m_nFallbackPaintKit() = 419; // Doppler Phase 2
//						}
//						else if (Skin == 25)
//						{
//							pWeapon->m_nFallbackPaintKit() = 420; // Doppler Phase 3
//						}
//						else if (Skin == 26)
//						{
//							pWeapon->m_nFallbackPaintKit() = 421; // Doppler Phase 4
//						}
//						else if (Skin == 27)
//						{
//							pWeapon->m_nFallbackPaintKit() = 569; // Gamma Doppler Phase1
//						}
//						else if (Skin == 28)
//						{
//							pWeapon->m_nFallbackPaintKit() = 570; // Gamma Doppler Phase2
//						}
//						else if (Skin == 29)
//						{
//							pWeapon->m_nFallbackPaintKit() = 571; // Gamma Doppler Phase3
//						}
//						else if (Skin == 30)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Gamma Doppler Phase4
//						}
//						else if (Skin == 31)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Gamma Doppler Emerald
//						}
//						else if (Skin == 32)
//						{
//							pWeapon->m_nFallbackPaintKit() = 561; // Lore
//						}
//						else if (Skin == 33)
//						{
//							pWeapon->m_nFallbackPaintKit() = 567; // Black Laminate
//						}
//						else if (Skin == 34)
//						{
//							pWeapon->m_nFallbackPaintKit() = 574; // Autotronic
//						}
//						else if (Skin == 35)
//						{
//							pWeapon->m_nFallbackPaintKit() = 361; // Freehand
//						}
//					}
//					else if (Model == 12)
//					{
//						int Ursus = g_csgo.m_modelinfo()->GetModelIndex("models/weapons/v_knife_ursus.mdl");
//						pWeapon->m_nModelIndex() = Ursus; // m_nModelIndex
//						pWeapon->m_iViewModelIndex() = Ursus;
//						if (worldmodel) worldmodel->m_nModelIndex() = Ursus + 1;
//						*pWeapon->fixskins() = 519;
//						pWeapon->m_iEntityQuality() = 3;
//						killIcons.clear();
//						killIcons["knife_default_ct"] = "knife_push";
//						killIcons["knife_t"] = "knife_push";
//						int Skin = g_cfg.skins.KnifeSkin;
//						if (Skin == 0)
//						{
//							pWeapon->m_nFallbackPaintKit() = 5; // Forest DDPAT
//						}
//						else if (Skin == 1)
//						{
//							pWeapon->m_nFallbackPaintKit() = 12; // Crimson Web
//						}
//						else if (Skin == 2)
//						{
//							pWeapon->m_nFallbackPaintKit() = 27; // Bone Mask
//						}
//						else if (Skin == 3)
//						{
//							pWeapon->m_nFallbackPaintKit() = 38; // Fade
//						}
//						else if (Skin == 4)
//						{
//							pWeapon->m_nFallbackPaintKit() = 40; // Night
//						}
//						else if (Skin == 5)
//						{
//							pWeapon->m_nFallbackPaintKit() = 42; // Blue Steel
//						}
//						else if (Skin == 6)
//						{
//							pWeapon->m_nFallbackPaintKit() = 43; // Stained
//						}
//						else if (Skin == 7)
//						{
//							pWeapon->m_nFallbackPaintKit() = 44; // Case Hardened
//						}
//						else if (Skin == 8)
//						{
//							pWeapon->m_nFallbackPaintKit() = 59; // Slaughter
//						}
//						else if (Skin == 9)
//						{
//							pWeapon->m_nFallbackPaintKit() = 72; // Safari Mesh
//						}
//						else if (Skin == 10)
//						{
//							pWeapon->m_nFallbackPaintKit() = 77; // Boreal Forest
//						}
//						else if (Skin == 11)
//						{
//							pWeapon->m_nFallbackPaintKit() = 98; // Ultraviolet
//						}
//						else if (Skin == 12)
//						{
//							pWeapon->m_nFallbackPaintKit() = 143; // Urban Masked
//						}
//						else if (Skin == 13)
//						{
//							pWeapon->m_nFallbackPaintKit() = 175; // Scorched
//						}
//						else if (Skin == 14)
//						{
//							pWeapon->m_nFallbackPaintKit() = 323; // Rust Coat
//						}
//						else if (Skin == 15)
//						{
//							pWeapon->m_nFallbackPaintKit() = 409; // Tiger Tooth
//						}
//						else if (Skin == 16)
//						{
//							pWeapon->m_nFallbackPaintKit() = 410; // Damascus Steel
//						}
//						else if (Skin == 17)
//						{
//							pWeapon->m_nFallbackPaintKit() = 411; // Damascus Steel
//						}
//						else if (Skin == 18)
//						{
//							pWeapon->m_nFallbackPaintKit() = 413; // Marble Fade
//						}
//						else if (Skin == 19)
//						{
//							pWeapon->m_nFallbackPaintKit() = 414; // Rust Coat
//						}
//						else if (Skin == 20)
//						{
//							pWeapon->m_nFallbackPaintKit() = 415; // Doppler Ruby
//						}
//						else if (Skin == 21)
//						{
//							pWeapon->m_nFallbackPaintKit() = 416; // Doppler Sapphire
//						}
//						else if (Skin == 22)
//						{
//							pWeapon->m_nFallbackPaintKit() = 417; // Doppler Blackpearl
//						}
//						else if (Skin == 23)
//						{
//							pWeapon->m_nFallbackPaintKit() = 418; // Doppler Phase 1
//						}
//						else if (Skin == 24)
//						{
//							pWeapon->m_nFallbackPaintKit() = 419; // Doppler Phase 2
//						}
//						else if (Skin == 25)
//						{
//							pWeapon->m_nFallbackPaintKit() = 420; // Doppler Phase 3
//						}
//						else if (Skin == 26)
//						{
//							pWeapon->m_nFallbackPaintKit() = 421; // Doppler Phase 4
//						}
//						else if (Skin == 27)
//						{
//							pWeapon->m_nFallbackPaintKit() = 569; // Gamma Doppler Phase1
//						}
//						else if (Skin == 28)
//						{
//							pWeapon->m_nFallbackPaintKit() = 570; // Gamma Doppler Phase2
//						}
//						else if (Skin == 29)
//						{
//							pWeapon->m_nFallbackPaintKit() = 571; // Gamma Doppler Phase3
//						}
//						else if (Skin == 30)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Gamma Doppler Phase4
//						}
//						else if (Skin == 31)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Gamma Doppler Emerald
//						}
//						else if (Skin == 32)
//						{
//							pWeapon->m_nFallbackPaintKit() = 561; // Lore
//						}
//						else if (Skin == 33)
//						{
//							pWeapon->m_nFallbackPaintKit() = 567; // Black Laminate
//						}
//						else if (Skin == 34)
//						{
//							pWeapon->m_nFallbackPaintKit() = 574; // Autotronic
//						}
//						else if (Skin == 35)
//						{
//							pWeapon->m_nFallbackPaintKit() = 361; // Freehand
//						}
//					}
//					else if (Model == 13)
//					{
//						int Talon = g_csgo.m_modelinfo()->GetModelIndex("models/weapons/v_knife_widowmaker.mdl");
//						pWeapon->m_nModelIndex() = Talon; // m_nModelIndex
//						pWeapon->m_iViewModelIndex() = Talon;
//						if (worldmodel) worldmodel->m_nModelIndex() = Talon + 1;
//						*pWeapon->fixskins() = 523;
//						pWeapon->m_iEntityQuality() = 3;
//						killIcons.clear();
//						killIcons["knife_default_ct"] = "knife_push";
//						killIcons["knife_t"] = "knife_push";
//						int Skin = g_cfg.skins.KnifeSkin;
//						if (Skin == 0)
//						{
//							pWeapon->m_nFallbackPaintKit() = 5; // Forest DDPAT
//						}
//						else if (Skin == 1)
//						{
//							pWeapon->m_nFallbackPaintKit() = 12; // Crimson Web
//						}
//						else if (Skin == 2)
//						{
//							pWeapon->m_nFallbackPaintKit() = 27; // Bone Mask
//						}
//						else if (Skin == 3)
//						{
//							pWeapon->m_nFallbackPaintKit() = 38; // Fade
//						}
//						else if (Skin == 4)
//						{
//							pWeapon->m_nFallbackPaintKit() = 40; // Night
//						}
//						else if (Skin == 5)
//						{
//							pWeapon->m_nFallbackPaintKit() = 42; // Blue Steel
//						}
//						else if (Skin == 6)
//						{
//							pWeapon->m_nFallbackPaintKit() = 43; // Stained
//						}
//						else if (Skin == 7)
//						{
//							pWeapon->m_nFallbackPaintKit() = 44; // Case Hardened
//						}
//						else if (Skin == 8)
//						{
//							pWeapon->m_nFallbackPaintKit() = 59; // Slaughter
//						}
//						else if (Skin == 9)
//						{
//							pWeapon->m_nFallbackPaintKit() = 72; // Safari Mesh
//						}
//						else if (Skin == 10)
//						{
//							pWeapon->m_nFallbackPaintKit() = 77; // Boreal Forest
//						}
//						else if (Skin == 11)
//						{
//							pWeapon->m_nFallbackPaintKit() = 98; // Ultraviolet
//						}
//						else if (Skin == 12)
//						{
//							pWeapon->m_nFallbackPaintKit() = 143; // Urban Masked
//						}
//						else if (Skin == 13)
//						{
//							pWeapon->m_nFallbackPaintKit() = 175; // Scorched
//						}
//						else if (Skin == 14)
//						{
//							pWeapon->m_nFallbackPaintKit() = 323; // Rust Coat
//						}
//						else if (Skin == 15)
//						{
//							pWeapon->m_nFallbackPaintKit() = 409; // Tiger Tooth
//						}
//						else if (Skin == 16)
//						{
//							pWeapon->m_nFallbackPaintKit() = 410; // Damascus Steel
//						}
//						else if (Skin == 17)
//						{
//							pWeapon->m_nFallbackPaintKit() = 411; // Damascus Steel
//						}
//						else if (Skin == 18)
//						{
//							pWeapon->m_nFallbackPaintKit() = 413; // Marble Fade
//						}
//						else if (Skin == 19)
//						{
//							pWeapon->m_nFallbackPaintKit() = 414; // Rust Coat
//						}
//						else if (Skin == 20)
//						{
//							pWeapon->m_nFallbackPaintKit() = 415; // Doppler Ruby
//						}
//						else if (Skin == 21)
//						{
//							pWeapon->m_nFallbackPaintKit() = 416; // Doppler Sapphire
//						}
//						else if (Skin == 22)
//						{
//							pWeapon->m_nFallbackPaintKit() = 417; // Doppler Blackpearl
//						}
//						else if (Skin == 23)
//						{
//							pWeapon->m_nFallbackPaintKit() = 418; // Doppler Phase 1
//						}
//						else if (Skin == 24)
//						{
//							pWeapon->m_nFallbackPaintKit() = 419; // Doppler Phase 2
//						}
//						else if (Skin == 25)
//						{
//							pWeapon->m_nFallbackPaintKit() = 420; // Doppler Phase 3
//						}
//						else if (Skin == 26)
//						{
//							pWeapon->m_nFallbackPaintKit() = 421; // Doppler Phase 4
//						}
//						else if (Skin == 27)
//						{
//							pWeapon->m_nFallbackPaintKit() = 569; // Gamma Doppler Phase1
//						}
//						else if (Skin == 28)
//						{
//							pWeapon->m_nFallbackPaintKit() = 570; // Gamma Doppler Phase2
//						}
//						else if (Skin == 29)
//						{
//							pWeapon->m_nFallbackPaintKit() = 571; // Gamma Doppler Phase3
//						}
//						else if (Skin == 30)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Gamma Doppler Phase4
//						}
//						else if (Skin == 31)
//						{
//							pWeapon->m_nFallbackPaintKit() = 568; // Gamma Doppler Emerald
//						}
//						else if (Skin == 32)
//						{
//							pWeapon->m_nFallbackPaintKit() = 561; // Lore
//						}
//						else if (Skin == 33)
//						{
//							pWeapon->m_nFallbackPaintKit() = 567; // Black Laminate
//						}
//						else if (Skin == 34)
//						{
//							pWeapon->m_nFallbackPaintKit() = 574; // Autotronic
//						}
//						else if (Skin == 35)
//						{
//							pWeapon->m_nFallbackPaintKit() = 361; // Freehand
//						}
//					}
//				}
//				pWeapon->m_OriginalOwnerXuidLow() = 0;
//				pWeapon->m_OriginalOwnerXuidHigh() = 0;
//				pWeapon->m_flFallbackWear() = 0.001f;
//				pWeapon->m_iItemIDHigh() = 1;
//			}
//		}
//	}
//}
