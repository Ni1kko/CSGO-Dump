#pragma once
#include "Autowall.h"
#include "AnimationFix.h"
struct tick_record;
class ShotSnapshot
{
public:
	IBasePlayer* entity;
	string hitbox_where_shot;
	string resolver;
	float time;
	float first_processed_time;
	bool weapon_fire, bullet_impact;
	int hitgroup_hit;
	int damage;
	int hitbox;
	animation* record;
	Vector eyeangles, impact, start;
	int backtrack;
	matrix* pMat;

	string get_info();

};
extern std::vector<ShotSnapshot> shot_snapshots;
extern bool CanHitHitbox(const Vector start, const Vector end, animation* _animation, studiohdr_t* hdr, int box);
struct CLastHitted {
	int hitbox, point_id;
	animation* anims;
};
struct AutostopInfo
{
	float call_time;
	bool did_stop;
};
extern AutostopInfo& get_autostop_info();
class Ragebot : public Singleton<Ragebot>
{
public:
	animation backup_anims[65];
	void BackupPlayer(animation*);
	void SetAnims(animation*);
	void RestorePlayer(animation*);
	Vector HeadScan(animation* backshoot, int& hitbox, float& best_damage, float min_dmg);
	Vector PrimaryScan(animation* anims, int& hitbox, float& simtime, float& best_damage, float min_dmg);
	CWeaponConfig CurrentSettings();
	std::vector<int> GetHitboxesToScan(IBasePlayer*);
	std::vector<Vector> GetMultipoints(IBasePlayer*, int, matrix[128]);
	Vector FullScan(animation* anims, int& hitbox, float& simtime, float& best_damage, float min_dmg);
	Vector GetPoint(IBasePlayer* pBaseEntity, int iHitbox, matrix BoneMatrix[128]);
	int GetTicksToShoot();
	int GetTicksToStop();
	bool HoldFiringAnimation();
	void FastStop();
	Vector GetVisualHitbox(IBasePlayer* ent, int ihitbox);
	Vector GetAimVector(IBasePlayer*, float&, Vector&, animation*&, int&);
	bool Hitchance(Vector, bool, animation*, int&);
	bool IsAbleToShoot();
	void DropTarget();
	int target_index = -1;
	float best_distance;
	bool did_dt;
	bool aimbotted_in_current_tick;
	bool fired_in_that_tick;
	float current_aim_simulationtime;
	int current_minusticks;
	Vector current_aim_position;
	bool lby_backtrack;
	Vector current_aim_player_origin;
	bool shot;
	bool Shooting;
	Vector Target;
	bool hitchanced;
	bool fired;
	Vector Angles;
	void Run();
	bool last_tick_shooted;
	bool target_lethal;
	bool Shooted[65];
	bool Hitted[65];
	matrix BoneMatrix[128];
	int GetCurrentPriorityHitbox(IBasePlayer* pEntity);
	bool HeadAiming;
	animation* target_anims;
	ShotSnapshot last_hitted[65];
	Vector last_shot_angle;
	float LerpTime();
	clock_t last_shot_tick;
	void DrawCapsule(animation*);
};
