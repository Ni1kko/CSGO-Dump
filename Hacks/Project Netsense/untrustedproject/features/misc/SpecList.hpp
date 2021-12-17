#pragma once
#include "../../valve_sdk/sdk.hpp"
#include "../../valve_sdk/csgostructs.hpp"
#include "../../config.hpp"

class SpecList {
private:
	struct speclist_cfg_t {
		bool enabled = false;
		int x = 100;
		int y = 300;
	};
public:
	speclist_cfg_t params;
	void Draw();
	void SetupValues();
};

extern SpecList* g_SpecList;

class WaterMark {
private:
	struct watermark_cfg_t {
		bool enabled = false;
		int x = 100;
		int y = 100;
	};
public:
	watermark_cfg_t params;
	void DrawMark();
	void SetupValuesMark();
};

extern WaterMark* g_WaterMark;