#include "Glow.h"
#include "..\Settings.h"

Do* DoGlow;

void Do::DrawGlow()
{
	CGlowObjectManager* GlowObjectManager = (CGlowObjectManager*)g_pGlowManager;
	g::pLocalEntity = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

	for (int i = 0; i < GlowObjectManager->m_GlowObjectDefinitions.Count(); ++i)
	{
		if (GlowObjectManager->m_GlowObjectDefinitions[i].IsUnused() || !GlowObjectManager->m_GlowObjectDefinitions[i].GetEnt()) 
			continue;
		
		auto * object = &GlowObjectManager->m_GlowObjectDefinitions[i];
		auto entity = reinterpret_cast<C_BaseEntity*>(object->GetEnt());

		if (!entity || entity->IsDormant()) 
			continue;

		switch (entity->GetClientClass()->m_ClassID)
		{
			default:
			if (g_Settings.bItemGlow[1] && entity->GetClientClass()->m_ClassID == (int)EClassIds::CBaseCSGrenadeProjectile || entity->GetClientClass()->m_ClassID == (int)EClassIds::CMolotovProjectile || 
				entity->GetClientClass()->m_ClassID == (int)EClassIds::CFlashbang || entity->GetClientClass()->m_ClassID == (int)EClassIds::CDecoyProjectile ||
				entity->GetClientClass()->m_ClassID == (int)EClassIds::CSmokeGrenadeProjectile)
				object->Set(Color(g_Settings.projectiledroppedcolor, g_Settings.projectiledroppedcolor[3]), .55, 0);
			if(g_Settings.bItemGlow[0] && (strstr(entity->GetClientClass()->pNetworkName, "Weapon") || entity->GetClientClass()->m_ClassID == (int)EClassIds::CDEagle
				|| entity->GetClientClass()->m_ClassID == (int)EClassIds::CAK47))
				object->Set(Color(g_Settings.projectiledroppedcolor, g_Settings.projectiledroppedcolor[3]), .55, 0);
		case 40:	
			if (entity->GetClientClass()->m_ClassID == (int)EClassIds::CCSPlayer && (g::pLocalEntity != entity) && g::pLocalEntity->GetTeam() != entity->GetTeam())
			{
				if(g_Settings.iGlowStyle == 1)
					object->Set(Color(g_Settings.glowcolorother, g_Settings.glowcolorother[3] * 255), 1, 0);
				else if(g_Settings.iGlowStyle == 2)
					object->Set(Color(g_Settings.glowcolorother, g_Settings.glowcolorother[3] * 255), 0, 1);

				/*if(Globals::shot[entity->EntIndex()] && g_pGlobalVars->curtime + 0.4f >= g_pGlobalVars->curtime)
					object->Set(Color(100, 100, 100, 30), -5, 1);
				else
					object->Set(Color(0, 0, 0, 0), 0, 1);*/
			}
			else if (entity->GetClientClass()->m_ClassID == (int)EClassIds::CCSPlayer && (g::pLocalEntity != entity) && g_Settings.bShowTeammates && g::pLocalEntity->GetTeam() == entity->GetTeam())
			{
				if (g_Settings.iGlowStyle == 1)
					object->Set(Color(55, 255, 55, 150), 1, 0);
				else if (g_Settings.iGlowStyle == 2)
					object->Set(Color(55, 255, 55, 150), 0, 1);
			}
			else if (entity->GetClientClass()->m_ClassID == (int)EClassIds::CCSPlayer && g::pLocalEntity == entity)
			{
				if (g_Settings.iLocalGlowStyle == 1)
					object->Set(Color(g_Settings.glowcolorlocal, g_Settings.glowcolorlocal[3] * 255), 1, 0);
				else if (g_Settings.iLocalGlowStyle == 2)
					object->Set(Color(g_Settings.glowcolorlocal, g_Settings.glowcolorlocal[3] * 255), 0, 1);
			}
		}
	}
}