#pragma once
#include "../../imgui/imgui.h"
#include <vector>
#include <string>
#include <deque>

struct NotificationData
{
	unsigned drawState;
	unsigned count;
	std::string title;
	std::string message;
	std::string id;
	float time;
};

class CNotification
{
public:
	template<typename T>
	using deque = std::deque<T>;

	CNotification() {}
	CNotification(const CNotification&) = delete;

	void Push(const char* _Title, const char* _Message);
	void Draw();

private:
	enum DrawStatesList : unsigned
	{
		DrawState_Appearance,
		DrawState_Normal,
		DrawState_Disappearance
	};

	deque<NotificationData> m_Data;

	bool m_IsDescent = false;
	float m_flDescentTime = 0.f;

	void Delete();

	float GetAppearanceReachTime(float time);
	float GetNormalReachTime(float time);
	float GetDisappearanceReachTime(float time);

	bool IsAppearance(float time);
	bool IsNormal(float time);
	bool IsDisappearance(float time);

	float GetInterpolateTime(float time, float duration, float start, float current, float end);
	float Interpolate(float time, float curtime, float duration, float start, float end);

	bool IsCursorFocused(float window_pos_x, float window_pos_y, ImVec2 w_size);
	void SwipeProcessing(NotificationData& data, float* window_pos_x, ImVec2 w_size);

	ImVec4 GetWindowBgColor(const NotificationData& data);
	ImVec4 GetTextColor(const NotificationData& data);
	ImVec4 GetTextDisabledColor(const NotificationData& data);

	void GetScreenPosition(const NotificationData& data, float* pos_x, float* pos_y, ImVec2 w_size);
	void GetAlpha(const NotificationData& data, float default_alpha, float* out_alpha);

	void RenderWindow(NotificationData& data);
};

extern CNotification g_Notification;