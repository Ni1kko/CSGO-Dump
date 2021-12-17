#pragma once


class CResolver
{
public:
	float Overriding;
	void DoCM();
	bool AntiFreestanding(SDK::CBaseEntity * entity, float & yaw);
	void OverrideYaw();
	bool AntiPener(SDK::CBaseEntity * entity, float & yaw, int damage_tolerance);
	float apply_freestanding_low_performance(SDK::CBaseEntity * enemy);
	void Purves(SDK::CBaseEntity * ent);
	void resolve(SDK::CBaseEntity* entity);
	void antifreestand(SDK::CBaseEntity * entity);
	void AnimationFix(SDK::CBaseEntity * pEnt);
	void TranslateRecord(SDK::CBaseEntity* entity, CColor color, Vector top, Vector pos);
	void tommyisweartogodillkillu(SDK::CBaseEntity * entity);



	Vector networked_angles;
	Vector resolved_angles;
	Vector pos1, pos2;
	float dmg1, dmg2;
	int mode[65]; // used to determine the resolver mode

	void UpdateResolverRecord(SDK::CBaseEntity * entity);
	SDK::CAnimationLayer backup_layer, prev_layer;
	SDK::CAnimationLayer prev[15];

	void AACMain();

	void VisualResolver(SDK::CBaseEntity * entity);

	void AntiAimCorrection(SDK::CBaseEntity * entity);
	void resolvo(SDK::CBaseEntity* entity);
	int damage_tolerance = 10.f;
	float last_time_movingr;

	float right_damage = 0.f, left_damage = 0.f, back_damage = 0.f;
	float right_fraction = 0.f, left_fraction = 0.f, back_fraction = 0.f;
};

/*class animation_info {
private:

public:
	animation_info() {
		spawn_time = 0.f;
		angle.Init();
		ref_handle = nullptr;
		server_state = nullptr;
	}
	animation_info(animation_info& src) {
		spawn_time = src.spawn_time;
		ref_handle = src.ref_handle;
		server_state = src.server_state;
		for (int j = 0; j < 15; j++)
			backup_layers[j] = src.backup_layers[j];
		//    for (int j = 0; j < 24; j++)
		//        poses[j] = src.poses[j];
		angle = src.angle;
	}
	float spawn_time;
	SDK::CBaseHandle ref_handle;
	SDK::CBaseAnimState* server_state;
	SDK::CAnimationLayer backup_layers[15];
	float poses[24];
	matrix3x4_t matrix[128];
	Vector angle;
};*/

extern CResolver* resolver;