#include "SDK.h"

#define MOVING_MINIMUM_SPEED 0.5f //stand anim will be triggered instead of slow playback, walk anim
#define MAIN_IDLE_SEQUENCE_LAYER 0
#define AIMSEQUENCE_LAYER 1
#define NUM_AIMSEQUENCE_LAYERS 4

class LegAnimType_t {

};
class CModAnimConfig {
public: 
	float m_flMaxBodyYawDegrees;
	LegAnimType_t m_LegAnimType;
	bool m_bUseAimSequences;
};
enum {
	TURN_NONE = 0,
	TURN_LEFT,
	TURN_RIGHT
};

