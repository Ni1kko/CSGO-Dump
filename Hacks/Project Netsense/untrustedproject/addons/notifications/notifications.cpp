#include "notifications.hpp"
#include "../../helpers/utils.hpp"
#include "../../globals.hpp"
#include <chrono>
#include <algorithm>

static const auto WINDOW_OFFSET = ImVec2(30, 70);
static const auto DURATION = 0.1f; // 200 msec

CNotification g_Notification;

std::string random_string(size_t length)
{
	auto randchar = []() -> char {
		const char charset[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		const size_t max_index = (sizeof(charset) - 1);
		return charset[rand() % max_index];
	};
	std::string str(length, 0);
	std::generate_n(str.begin(), length, randchar);
	return str;
}

void CNotification::Push(const char* _Title, const char* _Message)
{
	NotificationData data;
	data.count = m_Data.size();
	data.drawState = DrawState_Appearance;
	data.title = _Title;
	data.message = _Message;
	data.id = random_string(5);
	data.time = g_GlobalVars->curtime;

	m_Data.push_back(data);
}

void CNotification::Delete()
{
	m_Data.pop_front();

	if (m_Data.empty())
		return;

	for (auto& data : m_Data)
		data.count--;

	m_IsDescent = true;
	m_flDescentTime = g_GlobalVars->curtime;
}

void CNotification::Draw()
{
	if (m_Data.empty())
		return;

	for (auto& data : m_Data)
	{
		switch (data.drawState)
		{
		case DrawState_Appearance:
			if (!IsAppearance(data.time))
				data.drawState = DrawState_Normal;
			break;
		case DrawState_Normal:
			if (!IsNormal(data.time))
				data.drawState = DrawState_Disappearance;
			break;
		case DrawState_Disappearance:
			if (!IsDisappearance(data.time))
				Delete();
		}

		RenderWindow(data);
	}
}


float CNotification::GetAppearanceReachTime(float time)
{
	return time + DURATION;
}

float CNotification::GetNormalReachTime(float time)
{
	return GetAppearanceReachTime(time) + 5.f;
}

float CNotification::GetDisappearanceReachTime(float time)
{
	return GetNormalReachTime(time) + DURATION;
}

bool CNotification::IsAppearance(float time)
{
	return g_GlobalVars->curtime < GetAppearanceReachTime(time);
}

bool CNotification::IsNormal(float time)
{
	return g_GlobalVars->curtime < GetNormalReachTime(time);
}

bool CNotification::IsDisappearance(float time)
{
	return g_GlobalVars->curtime < GetDisappearanceReachTime(time);
}

bool CNotification::IsCursorFocused(float window_pos_x, float window_pos_y, ImVec2 w_size)
{
	const auto cursor_pos = ImGui::GetIO().MousePos;
	return cursor_pos.x > window_pos_x&& cursor_pos.x <  CheatGVars::SWidth &&
		cursor_pos.y > window_pos_y&& cursor_pos.y < window_pos_y + w_size.y;
}

void CNotification::SwipeProcessing(NotificationData& data, float* window_pos_x, ImVec2 w_size)
{
	static float old_pos_x, drag_pos_x;
	static float different_pos_x;

	const auto& io = ImGui::GetIO();

	if (io.MouseDown[0])
	{
		data.drawState = DrawState_Normal;
		different_pos_x = io.MousePos.x - old_pos_x;

		if (old_pos_x - io.MousePos.x < 0.f)
			*window_pos_x = io.MousePos.x - drag_pos_x;
	}
	else
	{
		if (different_pos_x > 5.f)
		{
			data.drawState = DrawState_Disappearance;
			data.time = g_GlobalVars->curtime - GetNormalReachTime(0.f);
			data.time -= GetInterpolateTime(data.time, DURATION, CheatGVars::SWidth - (w_size.x + WINDOW_OFFSET.x),
				io.MousePos.x - drag_pos_x, CheatGVars::SWidth) - data.time;
		}

		different_pos_x = 0.f;
		old_pos_x = io.MousePos.x;
		drag_pos_x = io.MousePos.x - *window_pos_x;
	}
}

float CNotification::GetInterpolateTime(float time, float duration, float start, float current, float end)
{
	return ((current - start) + time / duration * (end - start)) / (end - start) * duration;
}

float CNotification::Interpolate(float time, float curtime, float duration, float start, float end)
{
	return start + (curtime - time) / duration * (end - start);
}

void CNotification::GetScreenPosition(const NotificationData& data, float* pos_x, float* pos_y, ImVec2 w_size)
{
	static const float flWindowSizeWithOffsetX = w_size.x + WINDOW_OFFSET.x;
	static const float flWindowSizeWithOffsetY = w_size.y + WINDOW_OFFSET.y;
	static const float flWindowInterval = w_size.y + 5.f;

	switch (data.drawState)
	{
	case DrawState_Appearance:
		*pos_x = CheatGVars::SWidth - Interpolate(data.time, g_GlobalVars->curtime, DURATION, 0.f, flWindowSizeWithOffsetX);
		*pos_y = CheatGVars::SHeight - flWindowSizeWithOffsetY - flWindowInterval * static_cast<float>(data.count);
		break;
	case DrawState_Normal:
		*pos_x = CheatGVars::SWidth - flWindowSizeWithOffsetX;
		*pos_y = CheatGVars::SHeight - flWindowSizeWithOffsetY - flWindowInterval * static_cast<float>(data.count);
		break;
	case DrawState_Disappearance:
		*pos_x = CheatGVars::SWidth - Interpolate(GetNormalReachTime(data.time), g_GlobalVars->curtime, DURATION, flWindowSizeWithOffsetX, 0.f);
		*pos_y = CheatGVars::SHeight - flWindowSizeWithOffsetY - flWindowInterval * static_cast<float>(data.count);
	}

	if (m_IsDescent)
	{
		if (g_GlobalVars->curtime < m_flDescentTime + DURATION)
			*pos_y -= Interpolate(m_flDescentTime, g_GlobalVars->curtime, DURATION, flWindowInterval, 0.f);
		else
			m_IsDescent = false;
	}
}

void CNotification::GetAlpha(const NotificationData& data, float default_alpha, float* out_alpha)
{
	switch (data.drawState)
	{
	case DrawState_Appearance:
		*out_alpha = Interpolate(data.time, g_GlobalVars->curtime, DURATION, 0.00f, default_alpha);
		break;
	case DrawState_Normal:
		*out_alpha = default_alpha;
		break;
	case DrawState_Disappearance:
		*out_alpha = Interpolate(data.time + DURATION + 5.f, g_GlobalVars->curtime, DURATION, default_alpha, 0.00f);
	}
}

ImVec4 CNotification::GetWindowBgColor(const NotificationData& data)
{
	auto windowbg_color = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);
	GetAlpha(data, windowbg_color.w, &windowbg_color.w);
	return windowbg_color;
}

ImVec4 CNotification::GetTextColor(const NotificationData& data)
{
	auto text_color = ImGui::GetStyleColorVec4(ImGuiCol_Text);
	GetAlpha(data, text_color.w, &text_color.w);
	return text_color;
}

ImVec4 CNotification::GetTextDisabledColor(const NotificationData& data)
{
	auto textdisabled_color = ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled);
	GetAlpha(data, textdisabled_color.w, &textdisabled_color.w);
	return textdisabled_color;
}

void CNotification::RenderWindow(NotificationData& data)
{
	static float prevtime = g_GlobalVars->curtime;

	ImVec2 w_size;
	w_size.x += ImGui::CalcTextSize(data.message.c_str()).x;
	w_size.x += 120.f;
	w_size.y += 63.f;

	float pos_x, pos_y;
	GetScreenPosition(data, &pos_x, &pos_y, w_size);

	if (!data.count && data.drawState == DrawState_Normal)
	{
		static bool bMouseDownMissed;
		static bool bMouseDownCatch;

		const auto& io = ImGui::GetIO();

		if (IsCursorFocused(pos_x, pos_y, w_size))
		{
			if (io.MouseDown[0] && !bMouseDownMissed && !bMouseDownCatch)
			{
				data.time += 5.f - (g_GlobalVars->curtime - data.time);
				bMouseDownCatch = true;
			}
		}
		else
		{
			if (io.MouseDown[0] && !bMouseDownMissed && !bMouseDownCatch)
				bMouseDownMissed = true;
		}

		if (!io.MouseDown[0] && bMouseDownMissed)
			bMouseDownMissed = false;

		if (!io.MouseDown[0] && bMouseDownCatch)
			bMouseDownCatch = false;

		if (IsCursorFocused(pos_x, pos_y, w_size) || bMouseDownCatch)
		{
			SwipeProcessing(data, &pos_x, w_size);

			if (bMouseDownCatch)
				data.time += g_GlobalVars->curtime - prevtime;
		}

		if (data.drawState == DrawState_Disappearance)
			GetScreenPosition(data, &pos_x, &pos_y, w_size);
	}

	prevtime = g_GlobalVars->curtime;

	if (data.id.c_str()[0] != '\0') {
		ImGuiStyle& style = ImGui::GetStyle();
		style.Alpha = 1.00f;

		ImGui::SetNextWindowPos(ImVec2(pos_x, pos_y));
		ImGui::SetNextWindowSize(w_size);

		ImGui::PushStyleColor(ImGuiCol_WindowBg, GetWindowBgColor(data));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);

		if (ImGui::Begin(data.id.c_str(), static_cast<bool*>(0), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing |
			ImGuiWindowFlags_Notification))
		{
			ImGui::BeginGroup();

			ImGui::PushStyleColor(ImGuiCol_Text, GetTextColor(data));
			ImGui::Text(data.title.c_str());
			ImGui::PopStyleColor();

			ImGui::Spacing();

			ImGui::PushStyleColor(ImGuiCol_Text, GetTextDisabledColor(data));
			ImGui::Text(data.message.c_str());
			ImGui::PopStyleColor();

			ImGui::EndGroup();
			ImGui::End();
		}

		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
	}
}