#include "radar.h"
#include "RenderManager.h"
#define M_PI		3.14159265358979323846f
Vector RotatePoint(Vector p, Vector c, float ang)
{
	float angleInRadians = ang * (M_PI / 180.f);
	float cosTheta = cos(angleInRadians);
	float sinTheta = sin(angleInRadians);
	return Vector(
		(cosTheta * (p.x - c.x) -
			sinTheta * (p.y - c.y) + c.x),
			(sinTheta * (p.x - c.x) +
				cosTheta * (p.y - c.y) + c.y),
		0);
}
void DrawRadar()
{
	int W, H;
	int centerW, centerh, topH;
	centerW = W / 2;
	centerh = H / 2;
	Interfaces::Engine->GetScreenSize(W, H);
	Vector TL(Options::Menu.MiscTab.RadarX.GetValue(), Options::Menu.MiscTab.RadarY.GetValue(), 0);
	Vector TR(TL.x + 200, TL.y, 0);
	Vector BL(TL.x, TL.y + 200, 0);
	Vector BR(TL.x + 200, TL.y + 200, 0);
	Render::drawRECT(TL.x, TL.y, TL.x + 200, TL.y + 200, Color(5, 5, 5, 255));
	Render::drawRECT(TL.x + 1, TL.y + 1, TL.x + 199, TL.y + 199, Color(75, 75, 75, 255));
	Interfaces::Surface->DrawSetColor(22, 22, 22, 255);
	Interfaces::Surface->DrawFilledRect(TL.x + 2, TL.y + 2, TL.x + 198, TL.y + 198);
	Interfaces::Surface->DrawSetColor(75, 75, 75, 255);
	Interfaces::Surface->DrawLine(TL.x + 50, TL.y + 50, TL.x + 100, TL.y + 100);
	Interfaces::Surface->DrawLine(TR.x - 50, TL.y + 50, TL.x + 100, TL.y + 100);
	Interfaces::Surface->DrawLine(TL.x + 100, TL.y, TL.x + 100, TL.y + 200);
	Interfaces::Surface->DrawLine(TL.x, TL.y + 100, TL.x + 200, TL.y + 100);
	Vector center = Vector((TL.x + TR.x) / 2, (TL.y + BL.y) / 2, 0.f);
	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity* pEntity = Interfaces::EntList->GetClientEntity(i);
		if (pEntity)
		{
			auto m_local = hackManager.pLocal();

			if (m_local == nullptr || !m_local)
				return;

			if (!pEntity->isValidPlayer() || m_local == nullptr || m_local->GetIndex() == pEntity->GetIndex())
				continue;

			Color c = pEntity->team() == m_local->team() ? Color(50, 50, 255, 255) : Color(255, 50, 50, 255);
			Vector relPos = (pEntity->GetAbsOriginlol() - m_local->GetAbsOriginlol()) * 0.05f;
			relPos.y *= -1.f;
			Vector drawAt = Vector(relPos.x + center.x, relPos.y + center.y, 0);
			Vector localAng;
			Interfaces::Engine->GetViewAngles(localAng);
			drawAt = RotatePoint(drawAt, center, localAng.y - 90);
			if (drawAt.x < TR.x && drawAt.x > TL.x && drawAt.y < BL.y && drawAt.y > TL.y)
				Render::drawRECT(drawAt.x - 1, drawAt.y - 1, drawAt.x + 2, drawAt.y + 2, c);
		}
	}
}