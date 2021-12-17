#pragma once
#include "../../utilities/includes.h"

#include "autostrafe.hpp"

class CMisc : public CFeature
{
public:
	void onMove(float sampleTime, CUserCmd *userCmd) override;
	void onFrameStage(clientFrameStage_t frameStage) override;
	void onPaint(unsigned int panel) override;
	void onEvent(IGameEvent* pEvent) override;

	void movementFix( Vector3 & oldang );

	void drawIndicators( );
	void clanTag( );

private:
	void Bunnyhop(CUserCmd* pCmd);
	void autoStrafe(CUserCmd* pCmd);
	void slowWalk(CUserCmd* pCmd);
	void removeDuckDelay(CUserCmd* pCmd);

	void edge_jump_preprediction(CUserCmd* pCmd);
	void edge_jump_postprediction(CUserCmd* pCmd);

	void duckInAir(CUserCmd* pCmd);

	void thirdPerson( clientFrameStage_t frameStage );

	void crosshair();
	void spectatorList( );

	void removeFlash();
	void removeSmoke();

	float forward, right, up;
	Vector3 viewforward, viewright, viewup, aimforward, aimright, aimup, vForwardNorm, vRightNorm, vUpNorm;
	Vector3 old_angle;

}; extern CMisc g_Misc;