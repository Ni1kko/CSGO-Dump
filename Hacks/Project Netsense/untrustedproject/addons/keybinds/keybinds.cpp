#include "keybinds.hpp"
#include "../../imgui/imgui.h"
#include "../../features/misc/misc.hpp"
#include "..\..\render.hpp"



CKeyBinds* g_KeyBinds = new CKeyBinds();


void CKeyBinds::DeclareKeybind(CKeyBind* kbind) {
	this->KeyBinds.push_back(kbind);
}

std::vector<CKeyBind> CKeyBinds::ExportActiveKeybinds() {
	std::vector<CKeyBind> result;
	for (CKeyBind* kbind : this->KeyBinds) {
		if (kbind->state != KeyBindState_None)
			result.push_back(*kbind);
	}
	return result;
}

void CKeyBinds::ExecuteKeyBinds() {
	for (CKeyBind* kbind : this->KeyBinds) {
		if (kbind->mfuncpointer && !*kbind->mfuncpointer) {
			*kbind->gvar = false;
			kbind->state = KeyBindState_None;
			continue;
		}

		switch (kbind->mode) {
		case KeyBindMode_Always:
			*kbind->gvar = true;
			kbind->state = KeyBindState_Always;
			break;
		case KeyBindMode_HoldOff:
			if (InputSys::Get().IsKeyDown(kbind->key)) {
				*kbind->gvar = false;
				kbind->state = KeyBindState_HoldingOff;
			}
			else {
				*kbind->gvar = true;
				kbind->state = KeyBindState_None;
			}
			break;
		case KeyBindMode_HoldOn:
			if (InputSys::Get().IsKeyDown(kbind->key)) {
				*kbind->gvar = true;
				kbind->state = KeyBindState_HoldingOn;
			}
			else {
				*kbind->gvar = false;
				kbind->state = KeyBindState_None;
			}
			break;
		case KeyBindMode_Toggle:
			if (InputSys::Get().WasKeyPressed(kbind->key))
				*kbind->gvar = !*kbind->gvar;
			kbind->state = *kbind->gvar ? KeyBindState_Toggled : KeyBindState_None;
			break;
		default:
			kbind->state = KeyBindState_None;
			break;
		}
	}
}

void CKeyBinds::Draw() {
	if (!g_Misc->options.show_keybinds)
		return;



	if (ImGui::Begin("##keybinds", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize)) {
		ImGui::PushFont(g_pSecondFont);
		std::string names = "";
		std::string modes = "";
		std::string tabs = "         ";
		ImGui::PopFont();
		for (auto kbind : g_KeyBinds->ExportActiveKeybinds()) {
			std::string mode = "";
			switch (kbind.state) {
			case KeyBindState_Always:
				mode = "Always";
				break;
			case KeyBindState_HoldingOff:
				mode = "Holding OFF";
				break;
			case KeyBindState_HoldingOn:
				mode = "Holding ON";
				break;
			case KeyBindState_Toggled:
				mode = "Toggled";
				break;
			};
			names += kbind.name + "\n";
			modes += mode + "\n";
			tabs += tabs + "\n";
		}

		ImGui::Text("Keybinds");
		auto txtSz = ImGui::CalcTextSize("Keybinds");
		auto p = ImGui::GetCursorPos();
		auto pos = ImGui::GetWindowPos();
		auto list = ImGui::GetWindowDrawList();
		auto size = ImGui::GetWindowSize();
		list->AddRectFilled(ImVec2(pos.x, p.y + pos.y + 1), ImVec2(pos.x + size.x, p.y + pos.y + 2), g_Misc->colors.accent_color);
		ImGui::Dummy(ImVec2(2, 2));



		ImVec2 pWsize = ImGui::CalcTextSize(names.c_str() + 10);
		ImGui::SetWindowSize(ImVec2(200, 50 + pWsize.y));
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::BeginGroup();
		ImGui::Columns(2, nullptr, false);
		ImGui::Text(names.c_str());
		ImGui::Text("             ");
		ImGui::NextColumn();
		ImGui::Text(modes.c_str());
		ImGui::EndGroup();


		ImGui::PopStyleVar();
	}
	ImGui::End();
}