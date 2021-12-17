#include "../../utilities/includes.h"

class CTestFeature : public CFeature
{
public:
	void onMove(float sampleTime, CUserCmd *userCmd) override;
	void onFrameStage(clientFrameStage_t frameStage) override;
	void onPaint(unsigned int panel) override;

};