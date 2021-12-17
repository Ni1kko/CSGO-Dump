#pragma once

#include "..\..\includes.hpp"
#include "..\..\sdk\structs.hpp"
#include <deque>

class otheresp : public singleton< otheresp > {
public:
	void antiaim_indicator();
	void watermark();
	void penetration_reticle();
	void lby_indicator();
	void menubar();
	void hitmarker_paint();
	//std::vector<onetap_ind_t> data;
	void hitmarkerdynamic_paint();
	void hitmarker_event(IGameEvent * event);
	void spread_crosshair(IDirect3DDevice9*);
	void spread_crosshair();
	void bomb_timer();
	//void onetap_indigos();
	//void onetap_indigos_event(IGameEvent * event);
private:
	float hurt_time;

	struct onetap_str
	{
		onetap_str(const float onetap_time, std::string message, const Color color) {
			this->onetap_time = onetap_time;
			this->message = message;
			this->color = color;

			this->x = 6.f;
			this->y = 0.f;
		}

		float onetap_time;
		std::string message;
		Color color;
		float x, y;
	};

	std::deque<onetap_str> onetap_t;
};

struct impact_info {
	float x, y, z;
	long long time;
};
struct hitmarker_info
{
	impact_info impact;
	int alpha;
};
//struct onetap_str
//{
//	onetap_str(const float onetap_time, std::string message, const Color color) {
//		this->onetap_time = onetap_time;
//		this->message = message;
//		this->color = color;
//
//		this->x = 6.f;
//		this->y = 0.f;
//	}
//
//	float onetap_time;
//	std::string message;
//	Color color;
//	float x, y;
//};
//
//std::deque<onetap_str> onetap_t;

extern std::vector<impact_info> impacts;
extern std::vector<hitmarker_info> hitmarkers;
//extern std::vector<onetap_su> ind_hit;