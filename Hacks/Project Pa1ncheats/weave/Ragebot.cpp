#include "Hooks.h"
#include "Ragebot.h"
#include "RageBackTracking.h"
#include "Resolver.h"
#include "EnginePrediction.h"
#include "AntiAims.h"

std::vector<ShotSnapshot> shot_snapshots;

CWeaponConfig Ragebot::CurrentSettings() {
	if (csgo->weapon->IsAuto() && vars.ragebot.weapon[weap_type::scar].enable)
		return vars.ragebot.weapon[weap_type::scar];
	else if (csgo->weapon->GetItemDefinitionIndex() == WEAPON_SSG08 && vars.ragebot.weapon[weap_type::scout].enable)
		return vars.ragebot.weapon[weap_type::scout];
	else if (csgo->weapon->GetItemDefinitionIndex() == WEAPON_AWP && vars.ragebot.weapon[weap_type::_awp].enable)
		return vars.ragebot.weapon[weap_type::_awp];
	else if (csgo->weapon->isRifle() && vars.ragebot.weapon[weap_type::rifles].enable)
		return vars.ragebot.weapon[weap_type::rifles];
	else if (csgo->weapon->IsHeavyPistol() && vars.ragebot.weapon[weap_type::heavy_pistols].enable)
		return vars.ragebot.weapon[weap_type::heavy_pistols];
	else if (csgo->weapon->isPistol() && vars.ragebot.weapon[weap_type::pistols].enable)
		return vars.ragebot.weapon[weap_type::pistols];
	else
		return vars.ragebot.weapon[weap_type::def];
}

std::vector<int> Ragebot::GetHitboxesToScan(IBasePlayer* pEntity)
{
	auto settings = CurrentSettings();
	std::vector< int > hitboxes;

	if (csgo->weapon->IsZeus()) {
		hitboxes.push_back((int)CSGOHitboxID::Chest);
		hitboxes.push_back((int)CSGOHitboxID::Stomach);
		hitboxes.push_back((int)CSGOHitboxID::Pelvis);
		return hitboxes;
	}

	if (GetCurrentPriorityHitbox(pEntity) == (int)CSGOHitboxID::Pelvis) // baim
	{
		if (settings.hitscan_baim & 1)
			hitboxes.push_back((int)CSGOHitboxID::Chest);

		if (settings.hitscan_baim & 2)
			hitboxes.push_back((int)CSGOHitboxID::Stomach);

		if (settings.hitscan_baim & 4)
			hitboxes.push_back((int)CSGOHitboxID::Pelvis);

		if (settings.hitscan_baim & 8)
		{
			hitboxes.push_back((int)CSGOHitboxID::LeftShin);
			hitboxes.push_back((int)CSGOHitboxID::RightShin);
		}
		if (settings.hitscan_baim & 16) {
			hitboxes.push_back((int)CSGOHitboxID::LeftFoot);
			hitboxes.push_back((int)CSGOHitboxID::RightFoot);
		}
		return hitboxes;
	}

	if (settings.hitscan & 1)
		hitboxes.push_back((int)CSGOHitboxID::Head);
	if (settings.hitscan & 2)
		hitboxes.push_back((int)CSGOHitboxID::Neck);
	if (settings.hitscan & 4) {
		hitboxes.push_back((int)CSGOHitboxID::UpperChest);
	}

	if (settings.hitscan & 8)
		hitboxes.push_back((int)CSGOHitboxID::Chest);

	if (settings.hitscan & 16)
		hitboxes.push_back((int)CSGOHitboxID::Stomach);

	if (settings.hitscan & 32)
		hitboxes.push_back((int)CSGOHitboxID::Pelvis);

	if (settings.hitscan & 64)
	{
		hitboxes.push_back((int)CSGOHitboxID::LeftHand);
		hitboxes.push_back((int)CSGOHitboxID::RightHand);
		hitboxes.push_back((int)CSGOHitboxID::LeftLowerArm);
		hitboxes.push_back((int)CSGOHitboxID::RightLowerArm);
		hitboxes.push_back((int)CSGOHitboxID::LeftUpperArm);
		hitboxes.push_back((int)CSGOHitboxID::RightUpperArm);
	}
	if (settings.hitscan & 128)
	{
		hitboxes.push_back((int)CSGOHitboxID::LeftShin);
		hitboxes.push_back((int)CSGOHitboxID::RightShin);
	}
	if (settings.hitscan & 256) {
		hitboxes.push_back((int)CSGOHitboxID::LeftFoot);
		hitboxes.push_back((int)CSGOHitboxID::RightFoot);
	}

	return hitboxes;
}
std::vector<Vector> Ragebot::GetMultipoints(IBasePlayer* pBaseEntity, int iHitbox, matrix BoneMatrix[128])
{
	std::vector<Vector> vPoints;

	if (!pBaseEntity)
		return vPoints;

	studiohdr_t* pStudioModel = interfaces.models.model_info->GetStudioModel(pBaseEntity->GetModel());
	mstudiohitboxset_t* set = pStudioModel->pHitboxSet(0);

	if (!set)
		return vPoints;

	mstudiobbox_t* untransformedBox = set->pHitbox(iHitbox);
	if (!untransformedBox)
		return vPoints;

	Vector vecMin = { 0, 0, 0 };
	Math::VectorTransform_Wrapper(untransformedBox->bbmin, BoneMatrix[untransformedBox->bone], vecMin);

	Vector vecMax = { 0, 0, 0 };
	Math::VectorTransform_Wrapper(untransformedBox->bbmax, BoneMatrix[untransformedBox->bone], vecMax);

	float mod = untransformedBox->radius != -1.f ? untransformedBox->radius : 0.f;
	Vector max;
	Vector min;

	float ps = 0.75f;
	if (pBaseEntity->GetVelocity().Length() > 300.f && iHitbox > 0)
		ps = 0.f;
	else {
		if (iHitbox <= (int)CSGOHitboxID::Neck)
			ps = CurrentSettings().pointscale_head / 100.f;
		else if (iHitbox <= (int)CSGOHitboxID::RightThigh)
			ps = CurrentSettings().pointscale_body / 100.f;
	}

	Math::VectorTransform(untransformedBox->bbmax + mod, BoneMatrix[untransformedBox->bone], max);
	Math::VectorTransform(untransformedBox->bbmin - mod, BoneMatrix[untransformedBox->bone], min);

	auto center = (min + max) * 0.5f;
	if (ps <= 0.05f) {
		vPoints.push_back(center);
		return vPoints;
	}

	auto clamp_shit = [](float val, float min, float max) {
		if (val < min)
			return min;
		if (val > max)
			return max;
		return val;
	};
	Vector curAngles = Math::CalculateAngle(center, csgo->local->GetEyePosition());
	Vector forward;
	Math::AngleVectors(curAngles, forward);
	Vector right = forward.Cross(Vector(0, 0, 1));
	Vector left = Vector(-right.x, -right.y, right.z);
	if (iHitbox == 0) {
		for (auto i = 0; i < 4; ++i)
			vPoints.push_back(center);
		vPoints[1].x += untransformedBox->radius * clamp_shit(0.f, ps - 0.2f, 0.87f); // near left ear
		vPoints[2].x -= untransformedBox->radius * clamp_shit(0.f, ps - 0.2f, 0.87f); // near right ear
		vPoints[3].z += untransformedBox->radius * ps - 0.05f; // forehead
	}
	else if (iHitbox == (int)CSGOHitboxID::Neck)
		vPoints.push_back(center);
	else if (iHitbox == (int)CSGOHitboxID::RightThigh ||
		iHitbox == (int)CSGOHitboxID::LeftThigh ||
		iHitbox == (int)CSGOHitboxID::RightShin ||
		iHitbox == (int)CSGOHitboxID::LeftShin ||
		iHitbox == (int)CSGOHitboxID::RightFoot ||
		iHitbox == (int)CSGOHitboxID::LeftFoot) {

		if (iHitbox == (int)CSGOHitboxID::RightThigh ||
			iHitbox == (int)CSGOHitboxID::LeftThigh) {
			vPoints.push_back(center);
		}
		else if (iHitbox == (int)CSGOHitboxID::RightShin ||
			iHitbox == (int)CSGOHitboxID::LeftShin) {
			vPoints.push_back(center);
		}
		else if (iHitbox == (int)CSGOHitboxID::RightFoot ||
			iHitbox == (int)CSGOHitboxID::LeftFoot) {
			vPoints.push_back(center);
			vPoints[0].z += 5.f;
		}
	}
	else if (iHitbox == (int)CSGOHitboxID::RightHand ||
		iHitbox == (int)CSGOHitboxID::LeftHand ||
		iHitbox == (int)CSGOHitboxID::RightUpperArm ||
		iHitbox == (int)CSGOHitboxID::RightLowerArm ||
		iHitbox == (int)CSGOHitboxID::LeftUpperArm ||
		iHitbox == (int)CSGOHitboxID::LeftLowerArm) {
		vPoints.push_back(center);
	}
	else {
		for (auto i = 0; i < 3; ++i)
			vPoints.push_back(center);
		vPoints[1] += right * (untransformedBox->radius * ps);
		vPoints[2] += left * (untransformedBox->radius * ps);
	}


	return vPoints;
}
Vector Ragebot::GetPoint(IBasePlayer* pBaseEntity, int iHitbox, matrix BoneMatrix[128])
{
	std::vector<Vector> vPoints;

	if (!pBaseEntity)
		return Vector(0, 0, 0);

	studiohdr_t* pStudioModel = interfaces.models.model_info->GetStudioModel(pBaseEntity->GetModel());
	mstudiohitboxset_t* set = pStudioModel->pHitboxSet(0);

	if (!set)
		return Vector(0, 0, 0);

	mstudiobbox_t* untransformedBox = set->pHitbox(iHitbox);
	if (!untransformedBox)
		return Vector(0, 0, 0);

	Vector vecMin = { 0, 0, 0 };
	Math::VectorTransform_Wrapper(untransformedBox->bbmin, BoneMatrix[untransformedBox->bone], vecMin);

	Vector vecMax = { 0, 0, 0 };
	Math::VectorTransform_Wrapper(untransformedBox->bbmax, BoneMatrix[untransformedBox->bone], vecMax);

	float mod = untransformedBox->radius != -1.f ? untransformedBox->radius : 0.f;
	Vector max;
	Vector min;

	Math::VectorTransform(untransformedBox->bbmax + mod, BoneMatrix[untransformedBox->bone], max);
	Math::VectorTransform(untransformedBox->bbmin - mod, BoneMatrix[untransformedBox->bone], min);

	return (min + max) * 0.5f;
}
int Ragebot::GetCurrentPriorityHitbox(IBasePlayer* pEntity)
{
	auto weapon = csgo->local->GetWeapon();
	if (!weapon)
		return -1;

	if (!csgo->local->isAlive())
		return -1;

	bool can_baim_on_miss = vars.ragebot.weapon[vars.ragebot.active_index].max_misses > 0
		&& csgo->missedshots[pEntity->GetIndex()] > vars.ragebot.weapon[vars.ragebot.active_index].max_misses;

	if (!pEntity->isAlive())
		return -1;
	if (weapon->IsZeus())
		return (int)CSGOHitboxID::Pelvis;
	if (CurrentSettings().baim & 1 && !(pEntity->GetFlags() & FL_ONGROUND))
		return (int)CSGOHitboxID::Pelvis;
	if (CurrentSettings().baim & 2 && can_baim_on_miss)
		return (int)CSGOHitboxID::Pelvis;
	if (pEntity->GetHealth() <= CurrentSettings().baim_under_hp)
		return (int)CSGOHitboxID::Pelvis;
	if (vars.ragebot.force_body->active)
		return (int)CSGOHitboxID::Pelvis;
	if (pEntity->BadMatrix())
		return (int)CSGOHitboxID::Pelvis;
	return 0;
}

void Ragebot::BackupPlayer(animation* anims) {
	auto i = anims->player->GetIndex();
	backup_anims[i].origin = anims->player->GetOrigin();
	backup_anims[i].abs_origin = anims->player->GetAbsOrigin();
	backup_anims[i].obb_mins = anims->player->GetMins();
	backup_anims[i].obb_maxs = anims->player->GetMaxs();
	backup_anims[i].bone_cache = anims->player->GetBoneCache().Base();
}
void Ragebot::SetAnims(animation* anims) {

	anims->player->GetOrigin() = anims->origin;
	anims->player->SetAbsOrigin(anims->abs_origin);
	anims->player->GetMins() = anims->obb_mins;
	anims->player->GetMaxs() = anims->obb_maxs;
	anims->player->SetBoneCache(anims->bones);
}
void Ragebot::RestorePlayer(animation* anims) {
	auto i = anims->player->GetIndex();
	anims->player->GetOrigin() = backup_anims[i].origin;
	anims->player->SetAbsOrigin(backup_anims[i].abs_origin);
	anims->player->GetMins() = backup_anims[i].obb_mins;
	anims->player->GetMaxs() = backup_anims[i].obb_maxs;
	anims->player->SetBoneCache(backup_anims[i].bone_cache);
}

Vector Ragebot::HeadScan(animation* anims, int& hitbox, float& best_damage, float min_dmg) {
	Vector best_point = Vector(0, 0, 0);
	memcpy(BoneMatrix, anims->bones, sizeof(matrix[128]));
	SetAnims(anims);
	int health = anims->player->GetHealth();
	if (min_dmg > health)
		min_dmg = health + 1;
	std::vector<Vector> Points = GetMultipoints(anims->player, 0, BoneMatrix);
	for (auto HitBox : Points) {

		auto info = g_AutoWall.Think(HitBox, anims->player);
		if (info.m_damage > min_dmg && info.m_damage > best_damage)
		{
			hitbox = 0;
			best_point = HitBox;
			best_damage = info.m_damage;
		}
	}
	RestorePlayer(anims);
	return best_point;
}
Vector Ragebot::PrimaryScan(animation* anims, int& hitbox, float& simtime, float& best_damage, float min_dmg) {
	memcpy(BoneMatrix, anims->bones, sizeof(matrix[128]));
	simtime = anims->sim_time;
	SetAnims(anims);

	best_damage = -1;
	const auto damage = min_dmg;
	auto best_point = Vector(0, 0, 0);
	auto health = anims->player->GetHealth();
	if (min_dmg > health)
		min_dmg = health + 1;
	auto priority_hitbox = GetCurrentPriorityHitbox(anims->player);

	static const vector<int> hitboxes = {
		(int)CSGOHitboxID::Head,
		(int)CSGOHitboxID::Chest,
		(int)CSGOHitboxID::Stomach,
		(int)CSGOHitboxID::Pelvis,
		(int)CSGOHitboxID::LeftShin,
		(int)CSGOHitboxID::RightShin,
	};

	for (auto HitboxID : hitboxes)
	{
		auto point = GetPoint(anims->player, HitboxID, BoneMatrix);
		auto info = g_AutoWall.Think(point, anims->player);
		if ((info.m_damage > min_dmg && info.m_damage > best_damage) || info.m_damage > health)
		{
			hitbox = HitboxID;
			best_point = point;
			best_damage = info.m_damage;
		}
	}
	RestorePlayer(anims);
	return best_point;
}
Vector Ragebot::FullScan(animation* anims, int& hitbox, float& simtime, float& best_damage, float min_dmg) {
	memcpy(BoneMatrix, anims->bones, sizeof(matrix[128]));
	simtime = anims->sim_time;
	best_damage = -1;
	Vector best_point = Vector(0, 0, 0);
	SetAnims(anims);
	int priority_hitbox = GetCurrentPriorityHitbox(anims->player);
	int health = anims->player->GetHealth();
	if (min_dmg > health)
		min_dmg = health + 1;
	auto hitboxes = GetHitboxesToScan(anims->player);

	static const vector<int> upper_hitboxes = {
		(int)CSGOHitboxID::Head,
		(int)CSGOHitboxID::Neck,
		(int)CSGOHitboxID::UpperChest,
		(int)CSGOHitboxID::Chest,
	};
	static const vector<int> baim_hitboxes = {
		(int)CSGOHitboxID::Chest,
		(int)CSGOHitboxID::Stomach,
		(int)CSGOHitboxID::Pelvis,
	};
	bool baim_if_lethal = CurrentSettings().baim & 4;
	if (baim_if_lethal || CurrentSettings().adaptive_baim) {
		for (auto HitboxID : baim_hitboxes) {
			std::vector<Vector> Points = GetMultipoints(anims->player, HitboxID, BoneMatrix);
			for (int k = 0; k < Points.size(); k++)
			{
				auto info = g_AutoWall.Think(Points[k], anims->player);
				if ((info.m_damage > min_dmg && info.m_damage > best_damage))
				{
					hitbox = HitboxID;
					best_point = Points[k];
					best_damage = info.m_damage;
				}
			}
		}
		if (baim_if_lethal && best_damage > health + 2) {
			target_lethal = true;
			RestorePlayer(anims);
			return best_point;
		}
		if (best_damage > 0 && CurrentSettings().adaptive_baim) {
			if (CanDT() && csgo->dt_charged) {
				if (best_damage * 2.f > health) {
					target_lethal = true;
					RestorePlayer(anims);
					return best_point;
				}
			}
			else {
				if (best_damage < health)
					target_lethal = false;
				RestorePlayer(anims);
				return best_point;
			}

		}
	}

	for (auto HitboxID : hitboxes)
	{
		std::vector<Vector> Points = GetMultipoints(anims->player, HitboxID, BoneMatrix);
		for (int k = 0; k < Points.size(); k++)
		{
			auto info = g_AutoWall.Think(Points[k], anims->player);
			if ((info.m_damage > min_dmg && info.m_damage > best_damage))
			{
				hitbox = HitboxID;
				best_point = Points[k];
				best_damage = info.m_damage;
			}
		}
	}

	if (best_damage > anims->player->GetHealth() + 2)
		target_lethal = true;
	RestorePlayer(anims);
	return best_point;
}

Vector Ragebot::GetAimVector(IBasePlayer* pTarget, float& simtime, Vector& origin, animation*& best_anims, int& hitbox)
{
	if (GetHitboxesToScan(pTarget).size() == 0)
		return Vector(0, 0, 0);

	float m_damage = 0.f;
	if (vars.ragebot.override_dmg->active)
	{
		m_damage = csgo->weapon->IsZeus() ? 100.f : CurrentSettings().mindamage_override;
	}
	else
		m_damage = csgo->weapon->IsZeus() ? 100.f : CurrentSettings().mindamage;
	auto latest_animation = g_Animfix->get_latest_animation(pTarget);
	auto record = latest_animation;
	if (!record.has_value() || !record.value()->player)
		return Vector(0, 0, 0);
	BackupPlayer(record.value());
	if (!vars.ragebot.posadj) {
		if (record.has_value())
		{
			float damage = -1.f;
			best_anims = record.value();

			return FullScan(record.value(), hitbox, simtime, damage, m_damage);
		}
	}

	if (vars.ragebot.backshoot_bt && !vars.ragebot.force_body->active) {
		record = g_Animfix->get_latest_firing_animation(pTarget);
		if (record.has_value() && record.value()->player) {
			float damage = -1.f;
			best_anims = record.value();
			simtime = record.value()->sim_time;
			Vector backshoot = HeadScan(record.value(), hitbox, damage, m_damage);
			if (backshoot != Vector(0, 0, 0))
				return backshoot;
		}

	}


	auto oldest_animation = g_Animfix->get_oldest_animation(pTarget);
	Vector latest_origin = Vector(0, 0, 0);
	float best_damage_0 = -1.f, best_damage_1 = -1.f;

	record = latest_animation;
	if (record.has_value())
	{
		latest_origin = record.value()->origin;
		float damage = -1.f;
		Vector full = PrimaryScan(record.value(), hitbox, simtime, damage, m_damage);
		if (full != Vector(0, 0, 0))
			best_damage_0 = damage;
	}

	record = oldest_animation;

	if (record.has_value() && record.value()->resolver == latest_animation.value()->resolver) // stupid fix, but works sometimes
	{
		float damage = -1.f;
		Vector full = PrimaryScan(record.value(), hitbox, simtime, damage, m_damage);
		if (full != Vector(0, 0, 0))
			best_damage_1 = damage;
	}

	if (best_damage_0 >= best_damage_1)
		record = latest_animation;
	else
		record = oldest_animation;

	if (record.has_value())
	{
		float damage = -1.f;
		best_anims = record.value();
		return FullScan(record.value(), hitbox, simtime, damage, m_damage);
	}
	return Vector(0, 0, 0);
}

static std::vector<std::tuple<float, float, float>> precomputed_seeds = {};

typedef void(*RandomSeed_t)(UINT);
RandomSeed_t m_RandomSeed = 0;
void random_seed(uint32_t seed) {
	if (m_RandomSeed == NULL)
		m_RandomSeed = (RandomSeed_t)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomSeed");
	m_RandomSeed(seed);
}

typedef float(*RandomFloat_t)(float, float);
RandomFloat_t m_RandomFloat;
float random_float(float flLow, float flHigh)
{
	if (m_RandomFloat == NULL)
		m_RandomFloat = (RandomFloat_t)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat");

	return m_RandomFloat(flLow, flHigh);
}

static const int total_seeds = 255;

void build_seed_table()
{
	if (!precomputed_seeds.empty())
		return;

	for (auto i = 0; i < total_seeds; i++) {
		random_seed(i + 1);

		const auto pi_seed = random_float(0.f, PI * 2);

		precomputed_seeds.emplace_back(random_float(0.f, 1.f),
			sin(pi_seed), cos(pi_seed));
	}
}

bool CanHitHitbox(const Vector start, const Vector end, animation* _animation, studiohdr_t* hdr, int box)
{
	studiohdr_t* pStudioModel = interfaces.models.model_info->GetStudioModel(_animation->player->GetModel());
	mstudiohitboxset_t* set = pStudioModel->pHitboxSet(0);

	if (!set)
		return false;

	mstudiobbox_t* studio_box = set->pHitbox(box);
	if (!studio_box)
		return false;

	Vector min, max;

	const auto is_capsule = studio_box->radius != -1.f;

	if (is_capsule)
	{
		Math::VectorTransform(studio_box->bbmin, _animation->bones[studio_box->bone], min);
		Math::VectorTransform(studio_box->bbmax, _animation->bones[studio_box->bone], max);
		const auto dist = Math::segment_to_segment(start, end, min, max);

		if (dist < studio_box->radius)
			return true;
	}

	if (!is_capsule)
	{
		Math::VectorTransform(Math::vector_rotate(studio_box->bbmin, studio_box->bbmin), _animation->bones[studio_box->bone], min);
		Math::VectorTransform(Math::vector_rotate(studio_box->bbmax, studio_box->rotation), _animation->bones[studio_box->bone], max);

		Math::vector_i_transform(start, _animation->bones[studio_box->bone], min);
		Math::vector_i_rotate(end, _animation->bones[studio_box->bone], max);

		if (Math::intersect_line_with_bb(min, max, studio_box->bbmin, studio_box->bbmax))
			return true;
	}

	return false;
}

bool HitTraces(animation* _animation, const Vector position, const float chance, int box)
{
	build_seed_table();

	const auto weapon = csgo->weapon;

	if (!weapon)
		return false;

	const auto info = weapon->GetCSWpnData();

	if (!info)
		return false;

	const auto studio_model = interfaces.models.model_info->GetStudioModel(_animation->player->GetModel());

	if (!studio_model)
		return false;

	// performance optimization.
	if ((csgo->local->GetEyePosition() - position).Length2D() > info->m_flRange)
		return false;

	// setup calculation parameters.
	const auto id = weapon->GetItemDefinitionIndex();
	const auto round_acc = [](const float accuracy) { return roundf(accuracy * 1000.f) / 1000.f; };
	const auto sniper = weapon->isSniper();
	const auto crouched = csgo->local->GetFlags() & FL_DUCKING;

	// calculate inaccuracy.
	const auto weapon_inaccuracy = weapon->GetInaccuracy();

	if (id == WEAPON_REVOLVER)
		return weapon_inaccuracy < (crouched ? .0020f : .0055f);

	// calculate start and angle.
	auto start = csgo->local->GetEyePosition();
	const auto aim_angle = Math::CalculateAngle(start, position);
	Vector forward, right, up;
	Math::AngleVectors(aim_angle, forward, right, up);

	// keep track of all traces that hit the enemy.
	auto current = 0;

	// setup calculation parameters.
	Vector total_spread, spread_angle, end;
	float inaccuracy, spread_x, spread_y;
	std::tuple<float, float, float>* seed;

	// use look-up-table to find average hit probability.
	for (auto i = 0u; i < total_seeds; i++)  // NOLINT(modernize-loop-convert)
	{
		// get seed.
		seed = &precomputed_seeds[i];

		// calculate spread.
		inaccuracy = std::get<0>(*seed) * weapon_inaccuracy;
		spread_x = std::get<2>(*seed) * inaccuracy;
		spread_y = std::get<1>(*seed) * inaccuracy;
		total_spread = (forward + right * spread_x + up * spread_y).Normalize();

		// calculate angle with spread applied.
		Math::VectorAngles(total_spread, spread_angle);

		// calculate end point of trace.
		Math::AngleVectors(spread_angle, end);
		end = start + end.Normalize() * info->m_flRange;

		// did we hit the hitbox?
		if (vars.ragebot.hitchance_consider_hitbox
			&& box != (int)CSGOHitboxID::LeftFoot
			&& box != (int)CSGOHitboxID::RightFoot) {
			if (CanHitHitbox(start, end, _animation, studio_model, box))
				current++;
		}
		else
		{
			trace_t tr;
			Ray_t ray;

			ray.Init(start, end);
			interfaces.trace->ClipRayToEntity(ray, MASK_SHOT | CONTENTS_GRATE, _animation->player, &tr);

			if (auto ent = tr.m_pEnt; ent)
			{
				if (ent == _animation->player)
					current++;
			}
		}
		// abort if hitchance is already sufficent.
		if (static_cast<float>(current) / static_cast<float>(total_seeds) >= chance)
			return true;

		// abort if we can no longer reach hitchance.
		if (static_cast<float>(current + total_seeds - i) / static_cast<float>(total_seeds) < chance)
			return false;
	}

	return static_cast<float>(current) / static_cast<float>(total_seeds) >= chance;
}

bool Ragebot::Hitchance(Vector Aimpoint, bool backtrack, animation* best, int& hitbox)
{
	bool r8 = csgo->weapon->GetItemDefinitionIndex() == weapon_revolver;
	if (CurrentSettings().hitchancetype == 1 || r8)
		return csgo->weapon->hitchance() > CurrentSettings().hitchance * (1.7 * (1.f - r8));
	else
		return HitTraces(best, Aimpoint, csgo->weapon->IsZeus() ? 0.8f : CurrentSettings().hitchance / 100.f, hitbox);
}

Vector Ragebot::GetVisualHitbox(IBasePlayer* ent, int ihitbox)
{
	if (ihitbox < 0 || ihitbox > 19) return Vector(0, 0, 0);

	if (!ent) return Vector(0, 0, 0);

	const auto model = ent->GetModel();

	if (!model)
		return Vector(0, 0, 0);

	auto pStudioHdr = interfaces.models.model_info->GetStudioModel(model);

	if (!pStudioHdr)
		return Vector(0, 0, 0);

	auto hitbox = pStudioHdr->pHitbox(ihitbox, 0);

	if (!hitbox)
		return Vector(0, 0, 0);

	Vector min, max;
	Math::VectorTransform(hitbox->bbmin, ent->GetBoneCache()[hitbox->bone], min);
	Math::VectorTransform(hitbox->bbmax, ent->GetBoneCache()[hitbox->bone], max);

	auto center = (min + max) / 2.f;

	return center;
}
static bool can_not_shoot_due_to_cock = false;
bool Ragebot::IsAbleToShoot()
{
	auto time = TICKS_TO_TIME(csgo->local->GetTickBase() - csgo->m_nTickbaseShift);

	if (!csgo->local || !csgo->local->GetWeapon())
		return false;

	if (csgo->cmd->weaponselect)
	{
		return false;
	}

	if (!csgo->weapon)
	{
		return false;
	}

	const auto info = csgo->weapon->GetCSWpnData();

	if (!info)
	{
		return false;
	}

	const auto is_zeus = csgo->weapon->GetItemDefinitionIndex() == weapon_taser;
	const auto is_knife = !is_zeus && info->m_iWeaponType == WEAPONTYPE_KNIFE;

	if (csgo->weapon->GetItemDefinitionIndex() == weapon_c4 || !csgo->weapon->IsGun())
	{
		return false;
	}

	if (csgo->weapon->GetAmmo(false) < 1 && !is_knife)
	{
		return false;
	}

	if (csgo->weapon->InReload())
	{
		return false;
	}

	if ((csgo->local->m_flNextAttack() > time && !CanDT())
		|| csgo->weapon->NextPrimaryAttack() > time
		|| csgo->weapon->NextSecondaryAttack() > time)
	{
		if (csgo->weapon->GetItemDefinitionIndex() != weapon_revolver && info->m_iWeaponType == WEAPONTYPE_PISTOL)
			csgo->cmd->buttons &= ~IN_ATTACK;

		return false;
	}

	return true;
}
float Ragebot::LerpTime() {
	static auto cl_interp = interfaces.cvars->FindVar(hs::cl_interp::s().c_str());
	static auto cl_updaterate = interfaces.cvars->FindVar(hs::cl_updaterate::s().c_str());
	const auto update_rate = cl_updaterate->GetInt();
	const auto interp_ratio = cl_interp->GetFloat();

	auto lerp = interp_ratio / update_rate;

	if (lerp <= interp_ratio)
		lerp = interp_ratio;

	return lerp;
};

void Ragebot::DropTarget()
{
	target_index = -1;
	best_distance = INT_MAX;
	fired_in_that_tick = false;
	current_aim_position = Vector();
	shot = false;
	csgo->should_stop_slide = false;
	csgo->last_forced_tickcount = -1;
	csgo->should_stop = false;
	g_AutoWall.reset();
}
bool CockRevolver()
{
	if (!vars.ragebot.enable)
		return false;

	if (!csgo->local)
		return false;

	static auto r8cock_flag = true;
	static auto r8cock_time = 0.0f;

	//ZALUPA
	float REVOLVER_COCK_TIME = 0.2421875f;
	const int count_needed = floor(REVOLVER_COCK_TIME / interfaces.global_vars->interval_per_tick);
	static int cocks_done = 0;

	if (!csgo->weapon ||
		csgo->weapon->GetItemDefinitionIndex() != WEAPON_REVOLVER ||
		csgo->weapon->NextPrimaryAttack() > interfaces.global_vars->curtime)
	{
		if (csgo->weapon && csgo->weapon->GetItemDefinitionIndex() == WEAPON_REVOLVER)
			csgo->cmd->buttons &= ~IN_ATTACK;
		Ragebot::Get().shot = false;
		csgo->weapon_struct.work = false;
		return false;
	}

	csgo->weapon_struct.work = true;

	if (cocks_done < count_needed)
	{
		csgo->cmd->buttons |= IN_ATTACK;
		++cocks_done;
		return false;
	}
	else
	{
		csgo->cmd->buttons &= ~IN_ATTACK;
		cocks_done = 0;
		return true;
	}

	csgo->cmd->buttons |= IN_ATTACK;
	float curtime = csgo->local->GetTickBase() * interfaces.global_vars->interval_per_tick;
	static float next_shoot_time = 0.f;

	bool ret = false;

	if (fabsf(next_shoot_time - curtime) < 0.5)
		next_shoot_time = curtime + 0.2f - interfaces.global_vars->interval_per_tick; // -1 because we already cocked THIS tick ???

	if (next_shoot_time - curtime - interfaces.global_vars->interval_per_tick <= 0.f)
	{
		next_shoot_time = curtime + 0.2f;
		ret = true;
		// should still go for one more tick but if we do, we're gonna shoot sooo idk how2do rn, its late
		// the aimbot should decide whether to shoot or not yeh
	}
	return ret;
}

string HitboxToString(int id)
{
	switch (id)
	{
	case 0: return "head";  break;
	case 1: return "neck"; break;
	case 2: return "pelvis"; break;
	case 3: return "stomach"; break;
	case 4: return "lower chest"; break;
	case 5: return "chest"; break;
	case 6: return "upper chest"; break;
	case 7: return "right thigh"; break;
	case 8: return "left thigh"; break;
	case 9: return "right leg"; break;
	case 10: return "left leg"; break;
	case 11: return "right foot"; break;
	case 12: return "left foot"; break;
	case 13: return "right hand"; break;
	case 14: return "left hand"; break;
	case 15: return "right arm"; break;
	case 16: return "left arm"; break;
	case 17: return "right lower arm"; break;
	case 18: return "right upper arm"; break;
		break;
	}
}
string ShotSnapshot::get_info() {
	string ret;
	ret += " [H:" + hitbox_where_shot + "] ";
	if (vars.ragebot.resolver && resolver.size() > 0)
		ret += " [R:" + resolver + "] ";
	if (vars.ragebot.posadj) {
		ret += " [B:" + std::to_string(backtrack) + "] ";
		if (record->didshot)
			ret += " [SHOT] ";
	}
	return ret;
}
AutostopInfo& get_autostop_info()
{
	static AutostopInfo info{ -FLT_MAX, false };
	return info;
}

//void Ragebot::FastStop() {
//	Vector velocity = csgo->vecUnpredictedVel;
//	float speed = velocity.Length2D();
//
//	if (speed <= 10.f) {
//		csgo->stop_speed = 0.01f;
//		csgo->should_stop_slide = true;
//		return;
//	}
//
//	Vector direction;
//	Math::VectorAngles(velocity, direction);
//	direction.y = csgo->original.y - direction.y;
//	Vector forward;
//	Math::AngleVectors(direction, forward);
//	Vector negated_direction = forward * -speed;
//	csgo->cmd->forwardmove = negated_direction.x;
//	csgo->cmd->sidemove = negated_direction.y;
//}



void Ragebot::FastStop() {
	auto wpn_info = csgo->weapon->GetCSWpnData();
	if (!wpn_info)
		return;
	auto get_standing_accuracy = [&]() -> const float
	{
		const auto max_speed = csgo->weapon->GetZoomLevel() > 0 ? wpn_info->m_flMaxSpeedAlt : wpn_info->m_flMaxSpeed;
		return max_speed / 3.f;
	};
	auto velocity = csgo->local->GetVelocity();
	float speed = velocity.Length2D();
	float max_speed = (csgo->weapon->GetZoomLevel() == 0 ? wpn_info->m_flMaxSpeed : wpn_info->m_flMaxSpeedAlt) * 0.1f;
	if (speed > max_speed) {
		csgo->should_stop_slide = false;
	}
	else {
		csgo->should_stop_slide = true;
		return;
	}
	if (speed <= get_standing_accuracy())
		return;
	Vector direction;
	Math::VectorAngles(velocity, direction);
	direction.y = csgo->original.y - direction.y;
	Vector forward;
	Math::AngleVectors(direction, forward);
	Vector negated_direction = forward * -speed;
	csgo->cmd->forwardmove = negated_direction.x;
	csgo->cmd->sidemove = negated_direction.y;
}


int Ragebot::GetTicksToShoot() {
	if (Ragebot::Get().IsAbleToShoot())
		return -1;
	auto flServerTime = (float)csgo->local->GetTickBase() * interfaces.global_vars->interval_per_tick;
	auto flNextPrimaryAttack = csgo->local->GetWeapon()->NextPrimaryAttack();

	return TIME_TO_TICKS(fabsf(flNextPrimaryAttack - flServerTime));
}
int Ragebot::GetTicksToStop() {
	static auto predict_velocity = [](Vector* velocity)
	{
		float speed = velocity->Length2D();
		static auto sv_friction = interfaces.cvars->FindVar(hs::sv_friction::s().c_str());
		static auto sv_stopspeed = interfaces.cvars->FindVar(hs::sv_stopspeed::s().c_str());

		if (speed >= 1.f)
		{
			float friction = sv_friction->GetFloat();
			float stop_speed = std::max< float >(speed, sv_stopspeed->GetFloat());
			float time = std::max< float >(interfaces.global_vars->interval_per_tick, interfaces.global_vars->frametime);
			*velocity *= std::max< float >(0.f, speed - friction * stop_speed * time / speed);
		}
	};
	Vector vel = csgo->vecUnpredictedVel;
	int ticks_to_stop = 0;
	while (true)
	{
		if (vel.Length2D() < 1.f)
			break;
		predict_velocity(&vel);
		ticks_to_stop++;
	}
	return ticks_to_stop;
}
bool Ragebot::HoldFiringAnimation() {
	return (csgo->weapon && !IsAbleToShoot() &&
		fabsf(csgo->weapon->LastShotTime() - ((float)csgo->local->GetTickBase() * interfaces.global_vars->interval_per_tick)) < 0.2f);
}

void Ragebot::DrawCapsule(animation* anims) {
	studiohdr_t* pStudioModel = interfaces.models.model_info->GetStudioModel(anims->player->GetModel());
	if (!pStudioModel)
		return;
	mstudiohitboxset_t* pHitboxSet = pStudioModel->pHitboxSet(0);
	if (!pHitboxSet)
		return;

	for (int i = 0; i < pHitboxSet->numhitboxes; i++)
	{
		mstudiobbox_t* pHitbox = pHitboxSet->pHitbox(i);
		if (!pHitbox)
			continue;

		Vector vMin, vMax;
		Math::VectorTransform(pHitbox->bbmin, BoneMatrix[pHitbox->bone], vMin);
		Math::VectorTransform(pHitbox->bbmax, BoneMatrix[pHitbox->bone], vMax);

		int r = vars.ragebot.shot_clr.get_red(),
			g = vars.ragebot.shot_clr.get_green(),
			b = vars.ragebot.shot_clr.get_blue(),
			a = vars.ragebot.shot_clr.get_alpha();

		if (pHitbox->radius != -1)
			interfaces.debug_overlay->add_capsule_overlay(vMin, vMax, pHitbox->radius, r, g, b, a, 5.f);
	}
}
void Ragebot::Run()
{
	auto weapon = csgo->weapon;

	if (!weapon->IsGun())
		return;

	int curhitbox;
	animation* best_anims = nullptr;
	int hitbox = -1;

	float simtime = 0;
	Vector minus_origin = Vector(0, 0, 0);
	animation* anims = nullptr;
	int box;

	Ragebot::Get().shot = false;
	csgo->should_stop_slide = false;
	csgo->stop_speed = 0.f;

	bool in_air = !(csgo->local->GetFlags() & FL_ONGROUND);
	bool cock_revolver = CockRevolver();

	bool is_able_to_shoot = IsAbleToShoot() || (weapon->GetItemDefinitionIndex() == WEAPON_REVOLVER && cock_revolver);

	for (auto i = 1; i <= interfaces.global_vars->maxClients; i++)
	{
		auto pEntity = interfaces.ent_list->GetClientEntity(i);
		if (pEntity == nullptr)
			continue;
		if (pEntity == csgo->local)
			continue;
		if (!pEntity->isAlive()) {
			csgo->missedshots[pEntity->GetIndex()] = 0;
			continue;
		}
		if (pEntity->GetHealth() <= 0)
			continue;
		if (pEntity->GetTeam() == csgo->local->GetTeam())
			continue;
		if (pEntity->IsDormant())
			continue;
		if (pEntity->HasGunGameImmunity())
			continue;
		if (vars.ragebot.delayshot) {
			if (pEntity->GetSimulationTime() == pEntity->GetOldSimulationTime())
				continue;
		}
		target_lethal = false;
		Vector aim_position = GetAimVector(pEntity, simtime, minus_origin, anims, box);

		if (!anims)
			continue;
		int health = pEntity->GetHealth();
		if (best_distance > health
			&& anims->player == pEntity && aim_position != Vector(0, 0, 0))
		{
			best_distance = health;
			target_index = i;
			current_aim_position = aim_position;
			current_aim_simulationtime = simtime;
			current_aim_player_origin = minus_origin;
			best_anims = anims;
			hitbox = box;
			target_anims = best_anims;
		}
	}

	static int delay = 0;
	did_dt = false;
	if (hitbox != -1 && target_index != -1 && best_anims && current_aim_position != Vector(0, 0, 0))
	{
		if (vars.ragebot.autoscope && weapon->isSniper() && csgo->weapon->GetZoomLevel() == 0)
		{
			csgo->cmd->buttons |= IN_ATTACK2;
			return;
		}
		auto can_shoot_if_fakeduck = !csgo->fake_duck || csgo->stand;
		bool htchance = Hitchance(current_aim_position, false, best_anims, hitbox);

		csgo->should_stop_slide = false;

		static int dt_shot_tick = 20;
		auto wpn_info = weapon->GetCSWpnData();
		if (csgo->local->GetFlags() & FL_ONGROUND && !vars.antiaim.slowwalk->active) {
			/*auto get_standing_accuracy = [&]() -> const float
			{
				const auto max_speed = weapon->GetZoomLevel() > 0 ? wpn_info->m_flMaxSpeedAlt : wpn_info->m_flMaxSpeed;
				return max_speed / 3.f;
			};
			auto velocity = csgo->local->GetVelocity().Length2D();
			if (CurrentSettings().quickstop && velocity > get_standing_accuracy()) {
				float max_speed = 0.f;
				if (csgo->weapon) {
					auto info = csgo->weapon->GetCSWpnData();
					if (info)
						max_speed = (csgo->weapon->GetZoomLevel() == 0 ? info->m_flMaxSpeed : info->m_flMaxSpeedAlt) * 0.1f;
				}
				if (velocity <= max_speed
					|| (CurrentSettings().quickstop_options & 1 && !is_able_to_shoot)
					|| (CurrentSettings().quickstop_options & 2) && target_lethal) {

					FastStop();
				}
				else
				{
					csgo->stop_speed = 0.05f;
					csgo->should_stop_slide = true;
				}
			}*/
			if (!csgo->weapon->IsZeus()) {
				bool should_stop = GetTicksToShoot() <= GetTicksToStop()
					|| ((CurrentSettings().quickstop_options & 1) && !is_able_to_shoot
						|| (CurrentSettings().quickstop_options & 2) && target_lethal);
				if (should_stop && CurrentSettings().quickstop)
				{
					if (!csgo->should_stop_slide)
						FastStop();

					csgo->stop_speed = 0.1f;
					csgo->should_stop_slide = true;
				}
				else
					csgo->should_stop_slide = false;
			}
		}

		if (vars.ragebot.autoshoot) {

			if (htchance && can_shoot_if_fakeduck && is_able_to_shoot)
			{
				if (!csgo->fake_duck || !vars.antiaim.fakelag_onshot) {
					csgo->send_packet = true;
					csgo->max_fakelag_choke = CanDT() ? 1 : 2;
				}

				csgo->cmd->buttons |= IN_ATTACK;
			}
		}
		if (htchance && can_shoot_if_fakeduck && is_able_to_shoot)
		{
			if (csgo->cmd->buttons & IN_ATTACK) {
				csgo->last_forced_tickcount = csgo->cmd->tick_count;
				csgo->cmd->viewangles = Math::CalculateAngle(csgo->local->GetEyePosition(), current_aim_position) - csgo->local->GetPunchAngle() * 2.f;

				csgo->cmd->tick_count = TIME_TO_TICKS(best_anims->sim_time + LerpTime());
				last_shot_angle = csgo->cmd->viewangles;
				ShotSnapshot snapshot;
				tick_record record;
				snapshot.entity = best_anims->player;
				snapshot.hitbox_where_shot = HitboxToString(hitbox);
				//snapshot.resolver = ResolverMode[best_anims->player->GetIndex()];
				snapshot.time = interfaces.global_vars->interval_per_tick * csgo->local->GetTickBase();
				snapshot.first_processed_time = 0.f;
				snapshot.bullet_impact = false;
				snapshot.weapon_fire = false;
				snapshot.damage = -1;
				snapshot.start = csgo->local->GetEyePosition()/*csgo->unpred_eyepos*/;
				snapshot.hitgroup_hit = -1;
				snapshot.backtrack = TIME_TO_TICKS(fabsf(best_anims->player->GetSimulationTime() - current_aim_simulationtime));
				snapshot.eyeangles = Math::normalize(best_anims->player->GetEyeAngles());
				snapshot.hitbox = hitbox;
				snapshot.record = best_anims;
				shot_snapshots.push_back(snapshot);
				shot = true;
				last_shot_tick = clock();
				csgo->firedshots[best_anims->player->GetIndex()]++;
				if (vars.ragebot.shotrecord) {
					DrawCapsule(best_anims);
				}
				last_tick_shooted = true;
			}
		}
	}
	if (is_able_to_shoot && csgo->cmd->buttons & IN_ATTACK)
		shot = true;
}
