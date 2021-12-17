#include "util.hpp"

#include <thread>

#define INRANGE(x,a,b)  (x >= a && x <= b)
#define GETBITS( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define GETBYTE( x )    (GETBITS(x[0]) << 4 | GETBITS(x[1]))

namespace util {
	int epoch_time() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
#define IsInRange(x, a, b) (x >= a && x <= b)
#define GetBits(x) (IsInRange(x, '0', '9') ? (x - '0') : ((x&(~0x20)) - 'A' + 0xA))
#define GetByte(x) (GetBits(x[0]) << 4 | GetBits(x[1]))
	DWORD FindSig(DWORD dwAddress, DWORD dwLength, const char* szPattern)
	{
		if (!dwAddress || !dwLength || !szPattern)
			return 0;

		const char* pat = szPattern;
		DWORD firstMatch = NULL;

		for (DWORD pCur = dwAddress; pCur < dwLength; pCur++)
		{
			if (!*pat)
				return firstMatch;

			if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == GetByte(pat))
			{
				if (!firstMatch)
					firstMatch = pCur;

				if (!pat[2])
					return firstMatch;

				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
					pat += 3;

				else pat += 2;
			}
			else
			{
				pat = szPattern;
				firstMatch = 0;
			}
		}

		return 0;
	}
	DWORD WaitOnModuleHandle(std::string moduleName)
	{
		DWORD ModuleHandle = NULL;
		while (!ModuleHandle)
		{
			ModuleHandle = (DWORD)GetModuleHandle(moduleName.c_str());
			if (!ModuleHandle)
				Sleep(50);
		}
		return ModuleHandle;
	}

	bool bCompare(const BYTE* Data, const BYTE* Mask, const char* szMask)
	{
		for (; *szMask; ++szMask, ++Mask, ++Data)
		{
			if (*szMask == 'x' && *Mask != *Data)
			{
				return false;
			}
		}
		return (*szMask) == 0;
	}
	DWORD FindPattern(std::string moduleName, BYTE* Mask, char* szMask)
	{
		DWORD Address = WaitOnModuleHandle(moduleName.c_str());
		MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
		DWORD Length = ModInfo.SizeOfImage;
		for (DWORD c = 0; c < Length; c += 1)
		{
			if (bCompare((BYTE*)(Address + c), Mask, szMask))
			{
				return (DWORD)(Address + c);
			}
		}
		return 0;
	}
	uint64_t FindSignature(const char* szModule, const char* szSignature)
	{
		MODULEINFO modInfo;
		GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(szModule), &modInfo, sizeof(MODULEINFO));
		DWORD startAddress = (DWORD)modInfo.lpBaseOfDll;
		DWORD endAddress = startAddress + modInfo.SizeOfImage;
		const char* pat = szSignature;
		DWORD firstMatch = 0;
		for (DWORD pCur = startAddress; pCur < endAddress; pCur++) {
			if (!*pat) return firstMatch;
			if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == GETBYTE(pat)) {
				if (!firstMatch) firstMatch = pCur;
				if (!pat[2]) return firstMatch;
				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?') pat += 3;
				else pat += 2;    //one ?
			}
			else {
				pat = szSignature;
				firstMatch = 0;
			}
		}
		return NULL;
	}
	DWORD FindSignaturenew(const char* szModuleName, const char* PatternName, char* szPattern)
	{
		HMODULE hModule = GetModuleHandleA(szModuleName);
		PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)hModule;
		PIMAGE_NT_HEADERS pNTHeaders = (PIMAGE_NT_HEADERS)(((DWORD)hModule) + pDOSHeader->e_lfanew);

		DWORD ret = FindSig(((DWORD)hModule) + pNTHeaders->OptionalHeader.BaseOfCode, ((DWORD)hModule) + pNTHeaders->OptionalHeader.SizeOfCode, szPattern);;

		return ret;
	}
	unsigned long pattern_scan(const char * module_name, const char * signature) {
		MODULEINFO module_info;

		GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(module_name), &module_info, sizeof(MODULEINFO));

		DWORD start_addr = (DWORD)module_info.lpBaseOfDll;
		DWORD end_addr = start_addr + module_info.SizeOfImage;
		const char* pat = signature;

		DWORD first_match = 0;

		for (DWORD cur = start_addr; cur < end_addr; cur++) {
			if (!*pat) return first_match;
			if (*(PBYTE)pat == '\?' || *(BYTE*)cur == GETBYTE(pat)) {
				if (!first_match) first_match = cur;
				if (!pat[2]) return first_match;
				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?') pat += 3;
				else pat += 2;
			}
			else {
				pat = signature;
				first_match = 0;
			}
		}

		return NULL;
	}

	bool visible(const Vector & start, const Vector & end, player_t * player, player_t * local)
	{
		Ray_t ray;
		trace_t tr;
		ray.Init(start, end);

		CTraceFilter filter;
		filter.pSkip = local;

		g_csgo.m_trace()->TraceRay(ray, MASK_SHOT, &filter, &tr);

		if (tr.hit_entity == player || tr.fraction > 0.97f)
		{
			return true;
		}

		return false;
	}

	void movement_fix(vec3_t & wish_angle, CUserCmd * m_pcmd) {
		auto angle_vectors = [](const vec3_t &angles, vec3_t *forward, vec3_t *right, vec3_t *up) {
			static auto sincos = [](float radians, float *sine, float *cosine) {
				*sine = sin(radians);
				*cosine = cos(radians);
			};

			float sr, sp, sy, cr, cp, cy;

			sincos(DEG2RAD(angles[1]), &sy, &cy);
			sincos(DEG2RAD(angles[0]), &sp, &cp);
			sincos(DEG2RAD(angles[2]), &sr, &cr);

			if (forward) {
				forward->x = cp * cy;
				forward->y = cp * sy;
				forward->z = -sp;
			}

			if (right) {
				right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
				right->y = (-1 * sr*sp*sy + -1 * cr*cy);
				right->z = -1 * sr*cp;
			}

			if (up) {
				up->x = (cr*sp*cy + -sr * -sy);
				up->y = (cr*sp*sy + -sr * cy);
				up->z = cr * cp;
			}
		};

		vec3_t view_fwd, view_right, view_up, cmd_fwd, cmd_right, cmd_up;
		auto viewangles = m_pcmd->m_viewangles;
		viewangles.Normalized();

		angle_vectors(wish_angle, &view_fwd, &view_right, &view_up);
		angle_vectors(viewangles, &cmd_fwd, &cmd_right, &cmd_up);

		float v8 = sqrtf((view_fwd.x * view_fwd.x) + (view_fwd.y * view_fwd.y));
		float v10 = sqrtf((view_right.x * view_right.x) + (view_right.y * view_right.y));
		float v12 = sqrtf(view_up.z * view_up.z);

		vec3_t norm_view_fwd((1.f / v8) * view_fwd.x, (1.f / v8) * view_fwd.y, 0.f);
		vec3_t norm_view_right((1.f / v10) * view_right.x, (1.f / v10) * view_right.y, 0.f);
		vec3_t norm_view_up(0.f, 0.f, (1.f / v12) * view_up.z);

		float v14 = sqrtf((cmd_fwd.x * cmd_fwd.x) + (cmd_fwd.y * cmd_fwd.y));
		float v16 = sqrtf((cmd_right.x * cmd_right.x) + (cmd_right.y * cmd_right.y));
		float v18 = sqrtf(cmd_up.z * cmd_up.z);

		vec3_t norm_cmd_fwd((1.f / v14) * cmd_fwd.x, (1.f / v14) * cmd_fwd.y, 0.f);
		vec3_t norm_cmd_right((1.f / v16) * cmd_right.x, (1.f / v16) * cmd_right.y, 0.f);
		vec3_t norm_cmd_up(0.f, 0.f, (1.f / v18) * cmd_up.z);

		float v22 = norm_view_fwd.x * m_pcmd->m_forwardmove;
		float v26 = norm_view_fwd.y * m_pcmd->m_forwardmove;
		float v28 = norm_view_fwd.z * m_pcmd->m_forwardmove;
		float v24 = norm_view_right.x * m_pcmd->m_sidemove;
		float v23 = norm_view_right.y * m_pcmd->m_sidemove;
		float v25 = norm_view_right.z * m_pcmd->m_sidemove;
		float v30 = norm_view_up.x * m_pcmd->m_upmove;
		float v27 = norm_view_up.z * m_pcmd->m_upmove;
		float v29 = norm_view_up.y * m_pcmd->m_upmove;

		m_pcmd->m_forwardmove = ((((norm_cmd_fwd.x * v24) + (norm_cmd_fwd.y * v23)) + (norm_cmd_fwd.z * v25))
			+ (((norm_cmd_fwd.x * v22) + (norm_cmd_fwd.y * v26)) + (norm_cmd_fwd.z * v28)))
			+ (((norm_cmd_fwd.y * v30) + (norm_cmd_fwd.x * v29)) + (norm_cmd_fwd.z * v27));
		m_pcmd->m_sidemove = ((((norm_cmd_right.x * v24) + (norm_cmd_right.y * v23)) + (norm_cmd_right.z * v25))
			+ (((norm_cmd_right.x * v22) + (norm_cmd_right.y * v26)) + (norm_cmd_right.z * v28)))
			+ (((norm_cmd_right.x * v29) + (norm_cmd_right.y * v30)) + (norm_cmd_right.z * v27));
		m_pcmd->m_upmove = ((((norm_cmd_up.x * v23) + (norm_cmd_up.y * v24)) + (norm_cmd_up.z * v25))
			+ (((norm_cmd_up.x * v26) + (norm_cmd_up.y * v22)) + (norm_cmd_up.z * v28)))
			+ (((norm_cmd_up.x * v30) + (norm_cmd_up.y * v29)) + (norm_cmd_up.z * v27));

		m_pcmd->m_forwardmove = math::clamp(m_pcmd->m_forwardmove, -450.f, 450.f);
		m_pcmd->m_sidemove = math::clamp(m_pcmd->m_sidemove, -450.f, 450.f);
		m_pcmd->m_upmove = math::clamp(m_pcmd->m_upmove, -320.f, 320.f);
	}

	unsigned int find_in_datamap(datamap_t * map, const char *name) {
		while (map) {
			for (int i = 0; i < map->m_data_num_fields; i++) {
				if (map->m_data_desc[i].fieldName == NULL)
					continue;

				if (strcmp(name, map->m_data_desc[i].fieldName) == 0)
					return map->m_data_desc[i].fieldOffset[TD_OFFSET_NORMAL];

				if (map->m_data_desc[i].fieldType == FIELD_EMBEDDED) {
					if (map->m_data_desc[i].td) {
						unsigned int offset;

						if ((offset = find_in_datamap(map->m_data_desc[i].td, name)) != 0)
							return offset;
					}
				}
			}

			map = map->m_base_map;
		}

		return 0;
	}

	bool get_bbox(player_t * e, Box& box) {
		/*vec3_t trans = e->m_vecOrigin( );

		vec3_t min;
		vec3_t max;

		min = e->GetCollideable( )->OBBMins( );
		max = e->GetCollideable( )->OBBMaxs( );

		vec3_t point_list[ ] = {
			vec3_t( min.x, min.y, min.z ),
			vec3_t( min.x, max.y, min.z ),
			vec3_t( max.x, max.y, min.z ),
			vec3_t( max.x, min.y, min.z ),
			vec3_t( max.x, max.y, max.z ),
			vec3_t( min.x, max.y, max.z ),
			vec3_t( min.x, min.y, max.z ),
			vec3_t( max.x, min.y, max.z )
		};

		vec3_t Distance = point_list[ 0 ] - point_list[ 1 ];
		int dst = Distance.Length( );
		dst /= 1.3f;
		vec3_t angs = math::calculate_angle( trans, g_ctx.m_local->get_eye_pos( ) );

		vec3_t all[ 8 ];
		angs.y += 45;
		for ( int i = 0; i < 4; i++ ) {
			math::angle_vectors( angs, all[ i ] );
			all[ i ] *= dst;
			all[ i + 4 ] = all[ i ];
			all[ i ].z = max.z;
			all[ i + 4 ].z = min.z;
			VectorAdd( all[ i ], trans, all[ i ] );
			VectorAdd( all[ i + 4 ], trans, all[ i + 4 ] );
			angs.y += 90;
		}

		vec3_t flb, brt, blb, frt, frb, brb, blt, flt;

		if ( !math::world_to_screen( all[ 3 ], flb ) )
			return false;
		if ( !math::world_to_screen( all[ 0 ], blb ) )
			return false;
		if ( !math::world_to_screen( all[ 2 ], frb ) )
			return false;
		if ( !math::world_to_screen( all[ 6 ], blt ) )
			return false;
		if ( !math::world_to_screen( all[ 5 ], brt ) )
			return false;
		if ( !math::world_to_screen( all[ 4 ], frt ) )
			return false;
		if ( !math::world_to_screen( all[ 1 ], brb ) )
			return false;
		if ( !math::world_to_screen( all[ 7 ], flt ) )
			return false;

		vec3_t arr[ ] = { flb, brt, blb, frt, frb, brb, blt, flt };

		float left = flb.x;
		float top = flb.y;
		float right = flb.x;
		float bottom = flb.y;

		for ( int i = 0; i < 8; i++ ) {
			if ( left > arr[ i ].x )
				left = arr[ i ].x;
			if ( top > arr[ i ].y )
				top = arr[ i ].y;
			if ( right < arr[ i ].x )
				right = arr[ i ].x;
			if ( bottom < arr[ i ].y )
				bottom = arr[ i ].y;
		}

		box.x = left;
		box.h = bottom - top;
		box.w = right - left;
		box.y = top;

		return true;*/

		vec3_t top, down, air, s[2];

		vec3_t adjust = vec3_t(0, 0, -18) * e->m_flDuckAmount();

		if (!(e->m_fFlags() & FL_ONGROUND) && (e->get_move_type() != MOVETYPE_LADDER))
			air = vec3_t(0, 0, 10);
		else
			air = vec3_t(0, 0, 0);

		down = e->m_vecOrigin() + air;
		top = down + vec3_t(0, 0, 72) + adjust;

		if (math::world_to_screen(top, s[1]) && math::world_to_screen(down, s[0])) {
			vec3_t delta = s[1] - s[0];

			box.h = fabsf(delta.y);
			box.w = box.h / 2.0f;

			box.x = s[1].x - (box.w / 2);
			box.y = s[1].y;

			return true;
		}

		return false;
	}

	bool get_bbox_world(entity_t * e, Box & box) {
		vec3_t pointstransf[8];
		auto collideable = e->GetCollideable();

		if (!collideable)
			return false;

		auto min = collideable->OBBMins();
		auto max = collideable->OBBMaxs();

		const matrix3x4_t &trans = e->m_rgflCoordinateFrame();

		vec3_t points[] = {
			vec3_t(min.x, min.y, min.z),
			vec3_t(min.x, max.y, min.z),
			vec3_t(max.x, max.y, min.z),
			vec3_t(max.x, min.y, min.z),
			vec3_t(max.x, max.y, max.z),
			vec3_t(min.x, max.y, max.z),
			vec3_t(min.x, min.y, max.z),
			vec3_t(max.x, min.y, max.z)
		};

		vec3_t pointsTransformed[8];
		for (int i = 0; i < 8; i++)
			math::vector_transform(points[i], trans, pointsTransformed[i]);

		vec3_t pos = e->m_vecOrigin();
		vec3_t screen_points[8] = { };

		for (int i = 0; i < 8; i++)
			if (!math::world_to_screen(pointsTransformed[i], screen_points[i]))
				return false;
			else
				pointstransf[i] = screen_points[i];

		auto left = screen_points[0].x;
		auto top = screen_points[0].y;
		auto right = screen_points[0].x;
		auto bottom = screen_points[0].y;

		for (int i = 1; i < 8; i++) {
			if (left > screen_points[i].x)
				left = screen_points[i].x;
			if (top < screen_points[i].y)
				top = screen_points[i].y;
			if (right < screen_points[i].x)
				right = screen_points[i].x;
			if (bottom > screen_points[i].y)
				bottom = screen_points[i].y;
		}

		box.x = left;
		box.y = top - (top - bottom);
		box.w = right - left;
		box.h = top - bottom;

		return true;
	}

	void trace_line(vec3_t & start, vec3_t & end, unsigned int mask, IClientEntity * ignore, CGameTrace * tr) {
		Ray_t ray;
		ray.Init(start, end);

		CTraceFilter filter;
		filter.pSkip = ignore;

		g_csgo.m_trace()->TraceRay(ray, mask, &filter, tr);
	}

	float server_time(CUserCmd * m_pcmd) {
		static int tick = 0;
		static CUserCmd * last_command;

		if (!m_pcmd)
			return tick * g_csgo.m_globals()->m_interval_per_tick;

		if (!last_command || last_command->m_predicted)
			tick = g_ctx.m_local->m_nTickBase();
		else
			tick++;

		last_command = m_pcmd;
	}

	float lerp_time() {
		int ud_rate = g_csgo.m_cvar()->FindVar("cl_updaterate")->GetInt();
		ConVar *min_ud_rate = g_csgo.m_cvar()->FindVar("sv_minupdaterate");
		ConVar *max_ud_rate = g_csgo.m_cvar()->FindVar("sv_maxupdaterate");

		if (min_ud_rate && max_ud_rate)
			ud_rate = max_ud_rate->GetInt();

		float ratio = g_csgo.m_cvar()->FindVar("cl_interp_ratio")->GetFloat();

		if (ratio == 0)
			ratio = 1.0f;

		float lerp = g_csgo.m_cvar()->FindVar("cl_interp")->GetFloat();
		ConVar *c_min_ratio = g_csgo.m_cvar()->FindVar("sv_client_min_interp_ratio");
		ConVar *c_max_ratio = g_csgo.m_cvar()->FindVar("sv_client_max_interp_ratio");

		if (c_min_ratio && c_max_ratio && c_min_ratio->GetFloat() != 1)
			ratio = math::clamp(ratio, c_min_ratio->GetFloat(), c_max_ratio->GetFloat());

		return max(lerp, (ratio / ud_rate));
	}

	bool good_backtrack_tick(float m_flSimulationTime) {
		INetChannelInfo *nci = g_csgo.m_engine()->GetNetChannelInfo();

		if (!nci)
			return false;

		float correct = 0;
		correct += nci->GetLatency(FLOW_OUTGOING);
		correct += nci->GetLatency(FLOW_INCOMING);
		correct += lerp_time();

		correct = math::clamp(correct, 0.f, 1.f);

		float delta = correct - (g_csgo.m_globals()->m_curtime - m_flSimulationTime);
		return fabsf(delta) <= 0.2f;
	}
}