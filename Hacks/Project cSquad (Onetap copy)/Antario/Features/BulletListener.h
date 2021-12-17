#include "..\Singleton.h"
#include "..\SDK\CInput.h"
#include "..\SDK\CEntity.h"
#include "..\Utils\GlobalVars.h"
#include "..\SDK\ClientClass.h"
#include "..\SDK\ConVar.h"
#include "..\Settings.h"
#include "..\SDK\CTrace.h"
#include "..\SDK\ISurfaceData.h"
#include "..\SDK\Materials.h"
#include "..\SDK\Definitions.h"
#include "..\SDK\IViewRenderBeams.h"
#include "..\SDK\IGameEvent.h"
#include "..\SDK\ISurface.h"
#include "..\Globals.h"
#include "..\SDK\IVDebugOverlay.h"

class bullettracers : public Singleton< bullettracers > {
private:
	class trace_info {
	public:
		trace_info(Vector starts, Vector positions, float times) {
			this->start = starts;
			this->position = positions;
			this->time = times;
		}

		Vector position;
		Vector start;
		float time;
	};

	std::vector<trace_info> logs;

	void draw_beam(Vector src, Vector end, Color color);
public:
	void events(IGameEvent* event);
};

class otheresp : public Singleton< otheresp >
{
public:
	void hitmarker_event(IGameEvent * event);
	void hitmarkerdynamic_paint();

};

struct impact_info
{
	float x, y, z;
	long long time;
};

struct hitmarker_info
{
	impact_info impact;
	int alpha;
};

extern std::vector<impact_info> impacts;
extern std::vector<hitmarker_info> hitmarkers;