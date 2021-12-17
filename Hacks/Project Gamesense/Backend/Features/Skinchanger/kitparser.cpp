//#include "kitparser.h"
//#include "../../Utility/Utilities.h"
//#include "../../Configuration/Settings.h"
//#include "../../Interfaces/Interfaces.h"
//
//#include <algorithm>
//
//template <typename Key, typename Value>
//struct Node_t
//{
//	int previous_id;		//0x0000
//	int next_id;			//0x0004
//	void* _unknown_ptr;		//0x0008
//	int _unknown;			//0x000C
//	Key key;				//0x0010
//	Value value;			//0x0014
//};
//
//template <typename Key, typename Value>
//struct Head_t
//{
//	Node_t<Key, Value>* memory;		//0x0000
//	int allocation_count;			//0x0004
//	int grow_size;					//0x0008
//	int start_element;				//0x000C
//	int next_available;				//0x0010
//	int _unknown;					//0x0014
//	int last_element;				//0x0018
//}; //Size=0x001C
//
//// could use CUtlString but this is just easier and CUtlString isn't needed anywhere else
//struct String_t
//{
//	char* buffer;	//0x0000
//	int capacity;	//0x0004
//	int grow_size;	//0x0008
//	int length;		//0x000C
//}; //Size=0x0010
//
//struct CPaintKit
//{
//	int id;						//0x0000
//
//	String_t name;				//0x0004
//	String_t description;		//0x0014
//	String_t item_name;			//0x0024
//	String_t material_name;		//0x0034
//	String_t image_inventory;	//0x0044
//
//	char pad_0x0054[0x8C];		//0x0054
//}; //Size=0x00E0
//
//struct CStickerKit
//{
//	int id;
//
//	int item_rarity;
//
//	String_t name;
//	String_t description;
//	String_t item_name;
//	String_t material_name;
//	String_t image_inventory;
//
//	int tournament_event_id;
//	int tournament_team_id;
//	int tournament_player_id;
//	bool is_custom_sticker_material;
//
//	float rotate_end;
//	float rotate_start;
//
//	float scale_min;
//	float scale_max;
//
//	float wear_min;
//	float wear_max;
//
//	String_t image_inventory2;
//	String_t image_inventory_large;
//
//	std::uint32_t pad0[4];
//};
//
//class CCStrike15ItemSchema;
//class CCStrike15ItemSystem;
//
//using namespace Cheat;
//
//auto c_kitparser::initialize_kits() -> void {
//	const auto V_UCS2ToUTF8 = (int(*)(const wchar_t* ucs2, char* utf8, int len))(GetProcAddress(GetModuleHandleA("vstdlib.dll"), "V_UCS2ToUTF8"));
//	const auto sig_address = Utilities->Memory_PatternScan(Settings->ClientDLL, "E8 ? ? ? ? FF 76 0C 8D 48 04 E8");
//
//	const auto item_system_offset = *reinterpret_cast<std::int32_t*>(sig_address + 1);
//	const auto item_system_fn = reinterpret_cast<CCStrike15ItemSystem* (*)()>(sig_address + 5 + item_system_offset);
//	const auto item_schema = reinterpret_cast<CCStrike15ItemSchema*>(std::uintptr_t(item_system_fn()) + sizeof(void*));
//
//	const auto get_paint_kit_definition_offset = *reinterpret_cast<std::int32_t*>(sig_address + 11 + 1);
//	const auto get_paint_kit_definition_fn = reinterpret_cast<CPaintKit*(__thiscall*)(CCStrike15ItemSchema*, int)>(sig_address + 11 + 5 + get_paint_kit_definition_offset);
//
//	const auto start_element_offset = *reinterpret_cast<std::intptr_t*>(std::uintptr_t(get_paint_kit_definition_fn) + 8 + 2);
//	const auto head_offset = start_element_offset - 12;
//
//	const auto map_head = reinterpret_cast<Head_t<int, CPaintKit*>*>(std::uintptr_t(item_schema) + head_offset);
//
//	for (auto i = 0; i <= map_head->last_element; ++i)
//	{
//		const auto paint_kit = map_head->memory[i].value;
//
//		if (paint_kit->id == 9001)
//			continue;
//
//		const auto wide_name = Interfaces::Localize->Find(paint_kit->item_name.buffer + 1);
//		char name[256];
//		V_UCS2ToUTF8(wide_name, name, sizeof(name));
//
//		if (paint_kit->id < 10000)
//			skin_kits.push_back({ paint_kit->id, name });
//		else
//			glove_kits.push_back({ paint_kit->id, name });
//	}
//
//	std::sort(skin_kits.begin(), skin_kits.end());
//	std::sort(glove_kits.begin(), glove_kits.end());
//
//	const auto sticker_sig = Utilities->Memory_PatternScan(Settings->ClientDLL, "53 8D 48 04 E8 ? ? ? ? 8B 4D 10") + 4;
//
//	const auto get_sticker_kit_definition_offset = *reinterpret_cast<std::intptr_t*>(sticker_sig + 1);
//	const auto get_sticker_kit_definition_fn = reinterpret_cast<CPaintKit*(__thiscall*)(CCStrike15ItemSchema*, int)>(sticker_sig + 5 + get_sticker_kit_definition_offset);
//
//	const auto start_element_offset = *reinterpret_cast<intptr_t*>(std::uintptr_t(get_sticker_kit_definition_fn) + 8 + 2);
//	const auto head_offset = start_element_offset - 12;
//
//	const auto map_head = reinterpret_cast<Head_t<int, CStickerKit*>*>(std::uintptr_t(item_schema) + head_offset);
//
//	for (auto i = 0; i <= map_head->last_element; ++i)
//	{
//		const auto sticker_kit = map_head->memory[i].value;
//
//		char sticker_name_if_valve_fucked_up_their_translations[64];
//
//		auto sticker_name_ptr = sticker_kit->item_name.buffer + 1;
//
//		if (strstr(sticker_name_ptr, "StickerKit_dhw2014_dignitas"))
//		{
//			strcpy_s(sticker_name_if_valve_fucked_up_their_translations, "StickerKit_dhw2014_teamdignitas");
//			strcat_s(sticker_name_if_valve_fucked_up_their_translations, sticker_name_ptr + 27);
//			sticker_name_ptr = sticker_name_if_valve_fucked_up_their_translations;
//		}
//
//		const auto wide_name = Interfaces::Localize->Find(sticker_name_ptr);
//		char name[256];
//		V_UCS2ToUTF8(wide_name, name, sizeof(name));
//
//		sticker_kits.push_back({ sticker_kit->id, name });
//	}
//
//	std::sort(sticker_kits.begin(), sticker_kits.end());
//
//	sticker_kits.insert(sticker_kits.begin(), { 0, "None" });
//}