#pragma once

#include <vector>
#include "../../Include/GlobalIncludes.h"
#include "../../SDK/CBaseEntity.h"
#include "../../SDK/CViewSetup.h"
namespace Features
{
	struct EntityBoxInfo_t
	{
		int x, y, w, h, alpha, alpha1, alpha2, alpha3, alpha4, alpha_anim;
		float dormancy = 0.f;

		int hp = -1;
		int hpDifference = 0;

		float hpDiffTime = 0.f;

		CBaseEntity* pEnt;
	};

	struct SideInfo_t
	{
		std::string str;
		CColor color;
	};

	struct BottomInfo_t
	{
		bool isString;
		std::wstring str;
		double maxValue;
		double percentage;
		CColor color;
	};

	struct Indicator_t
	{
		std::string str;
		CColor color;
	};

	class CVisuals
	{
		public:
			void Run();
			void RunHitmarker();

			void RunSpectators();
			void DrawCircle(int x, int y);
			void remove_smoke();
			//void desyncdraw();
			void DrawZeusRange();

			std::vector<Indicator_t> custom_inds;
			std::vector<Indicator_t> bomb_inds;
		private:
			void DrawPlayers();
			void DrawBoundingBox( EntityBoxInfo_t inf );
			void DrawHealthBar( EntityBoxInfo_t inf );
			void DrawSideInfo( EntityBoxInfo_t inf );
			void DrawName( EntityBoxInfo_t inf );
			void DrawSkeleton(EntityBoxInfo_t inf);
			void DrawGlow(EntityBoxInfo_t inf);
			void DrawBottomInfo( EntityBoxInfo_t inf );

			void DrawWeaponAmmo( EntityBoxInfo_t inf );
			void DrawWeaponName( EntityBoxInfo_t inf );
			void DrawProjectiles (CBaseEntity* pEnt);
			void DrawBombInterface( CBaseEntity* pEnt );
			void DrawArrow( CBaseEntity* pEnt );
			void DrawIndicators();

			EntityBoxInfo_t infos[4096];
			int hitmarkerAlpha = 0;
	};

	class grenade_prediction : public singleton<grenade_prediction>
	{
	private:
		std::vector<Vector> path;

		int type = 0;
		int act = 0;

		void Setup(Vector& vecSrc, Vector& vecThrow, Vector viewangles);
		int Step(Vector& vecSrc, Vector& vecThrow, int tick, float interval);
		bool CheckDetonate(const Vector& vecThrow, const trace_t& tr, int tick, float interval);
		void Simulate(CViewSetup* setup);
		void TraceHull(Vector& src, Vector& end, trace_t& tr);
		void AddGravityMove(Vector& move, Vector& vel, float frametime, bool onground);
		void PushEntity(Vector& src, const Vector& move, trace_t& tr);
		void ResolveFlyCollisionCustom(trace_t& tr, Vector& vecVelocity, float interval);
		int PhysicsClipVelocity(const Vector& in, const Vector& normal, Vector& out, float overbounce);
	public:
		void Tick(int buttons);
		void View(CViewSetup* setup);
		void Paint();
		void PaintCircle();
	};

	extern CVisuals* Visuals;
}