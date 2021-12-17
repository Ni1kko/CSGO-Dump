#pragma once

#include <string>
#include <unordered_map>

#include "../sdk/gameEventManager.hpp"
#include "../sdk/userCmd.hpp"
#include "../sdk/baseClient.hpp"

class CFeature
{
public:
	virtual void onMove(float sampleTime, CUserCmd *userCmd) = 0;
	virtual void onFrameStage(clientFrameStage_t frameStage) = 0;
	virtual void onPaint(unsigned int panel) = 0;
	virtual void onEvent(IGameEvent* pEvent) = 0;

};

class CFeatureManager //cool idea btw :P //LaMe     thanks :PP //czapek
{
private:
	std::unordered_map< std::string, CFeature* > features;

public:
	void registerFeature(const std::string &featureName, CFeature *feature);
	void unregisterFeature(const std::string &featureName);

	void onMove(float sampleTime, CUserCmd *userCmd);
	void onFrameStage(clientFrameStage_t frameStage);
	void onPaint(unsigned int panel = -1);
	void onEvent(IGameEvent* pEvent);

};

extern CFeatureManager* g_Features;