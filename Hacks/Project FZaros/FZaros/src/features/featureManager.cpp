#include "featureManager.hpp"

void CFeatureManager::registerFeature(const std::string &featureName, CFeature *feature)
{
	if (this->features.find(featureName) == this->features.end())
		this->features[featureName] = feature;
}

void CFeatureManager::unregisterFeature(const std::string &featureName)
{
	if (this->features.find(featureName) != this->features.end())
		this->features.erase(featureName);
}

void CFeatureManager::onMove(float sampleTime, CUserCmd *userCmd)
{
	for (auto &[_, feature] : this->features)
	{
		feature->onMove(sampleTime, userCmd);
	}
}

void CFeatureManager::onFrameStage(clientFrameStage_t frameStage)
{
	for (auto &[_, feature] : this->features)
	{
		feature->onFrameStage(frameStage);
	}
}

void CFeatureManager::onPaint(unsigned int panel)
{
	for (auto &[_, feature] : this->features)
	{
		feature->onPaint(panel);
	}
}

void CFeatureManager::onEvent(IGameEvent* pEvent)
{
	for (auto &[_, feature] : this->features)
	{
		feature->onEvent(pEvent);
	}
}

CFeatureManager* g_Features = new CFeatureManager();