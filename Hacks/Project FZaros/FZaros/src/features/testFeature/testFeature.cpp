#include "testFeature.hpp"

void CTestFeature::onMove(float sampleTime, CUserCmd* userCmd)
{

}

void CTestFeature::onFrameStage(clientFrameStage_t frameStage)
{
	static bool insertPressed = false;

	if (!insertPressed && GetAsyncKeyState(VK_DELETE))
	{
		insertPressed = true;
	}
	else if (insertPressed && !GetAsyncKeyState(VK_DELETE))
	{
		insertPressed = false;

		if (g_Renderer->isUsingSurface())
			g_Renderer->useDX9();
		else if (g_Renderer->isUsingDX9())
			g_Renderer->useSurface();
	}
}

void CTestFeature::onPaint(unsigned int panel)
{
	//if (g_Renderer->isUsingDX9())
		//g_Renderer->fillRectangle(16, 16, 16, 16, Color::red());

	if (!Globals::localPlayer)
		return;

	if (Globals::localPlayer->alive())
	{
		g_Renderer->text(10, 30, g_Fonts->visuals, "nigga is alive", Color::White);
	}

	//g_Renderer->texturedRectangle(16, 16, g_Textures->testTexture);
}