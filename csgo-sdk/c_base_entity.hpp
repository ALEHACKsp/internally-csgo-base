#pragma once
#include "math/math.hpp"
#include <array>
#include "../csgo-sdk/classes/collideable.hpp"
#include "../csgo-sdk/classes/client_class.hpp"
#include "netvars/netvars.hpp"

enum move_type {
	movetype_none = 0,
	movetype_isometric,
	movetype_walk,
	movetype_step,
	movetype_fly,
	movetype_flygravity,
	movetype_vphysics,
	movetype_push,
	movetype_noclip,
	movetype_ladder,
	movetype_observer,
	movetype_custom,
	movetype_last = movetype_custom,
	movetype_max_bits = 4,
	max_movetype
};

enum ObserverType_t {
	OBS_MODE_NONE = 0,	// not in spectator mode
	OBS_MODE_DEATHCAM,	// special mode for death cam animation
	OBS_MODE_FREEZECAM,	// zooms to a target, and freeze-frames on them
	OBS_MODE_FIXED,		// view from a fixed camera position
	OBS_MODE_IN_EYE,	// follow a player in first person view
	OBS_MODE_CHASE,		// follow a player in third person view
	OBS_MODE_ROAMING,	// free roaming

	NUM_OBSERVER_MODES,
};

enum entity_flags {
	fl_onground = (1 << 0),
	fl_ducking = (1 << 1),
	fl_waterjump = (1 << 2),
	fl_ontrain = (1 << 3),
	fl_inrain = (1 << 4),
	fl_frozen = (1 << 5),
	fl_atcontrols = (1 << 6),
	fl_client = (1 << 7),
	fl_fakeclient = (1 << 8),
	fl_inwater = (1 << 9),
	fl_fly = (1 << 10),
	fl_swim = (1 << 11),
	fl_conveyor = (1 << 12),
	fl_npc = (1 << 13),
	fl_godmode = (1 << 14),
	fl_notarget = (1 << 15),
	fl_aimtarget = (1 << 16),
	fl_partialground = (1 << 17),
	fl_staticprop = (1 << 18),
	fl_graphed = (1 << 19),
	fl_grenade = (1 << 20),
	fl_stepmovement = (1 << 21),
	fl_donttouch = (1 << 22),
	fl_basevelocity = (1 << 23),
	fl_worldbrush = (1 << 24),
	fl_object = (1 << 25),
	fl_killme = (1 << 26),
	fl_onfire = (1 << 27),
	fl_dissolving = (1 << 28),
	fl_transragdoll = (1 << 29),
	fl_unblockable_by_player = (1 << 30)
};

template< typename Function > Function call_vfunc(PVOID Base, DWORD Index)
{
	PDWORD* VTablePointer = (PDWORD*)Base;
	PDWORD VTableFunctionBase = *VTablePointer;
	DWORD dwAddress = VTableFunctionBase[Index];
	return (Function)(dwAddress);
}
class entity_t {
public:
	void* animating() {
		return reinterpret_cast<void*>(uintptr_t(this) + 0x4);
	}
	void* networkable() {
		return reinterpret_cast<void*>(uintptr_t(this) + 0x8);
	}
	collideable_t* collideable() {
		using original_fn = collideable_t * (__thiscall*)(void*);
		return (*(original_fn**)this)[3](this);
	}
	c_client_class* client_class() {
		using original_fn = c_client_class * (__thiscall*)(void*);
		return (*(original_fn**)networkable())[2](networkable());
	}
	int index() {
		using original_fn = int(__thiscall*)(void*);
		return (*(original_fn**)networkable())[10](networkable());
	}
	bool is_player() {
		using original_fn = bool(__thiscall*)(entity_t*);
		return (*(original_fn**)this)[157](this);
	}
	bool is_weapon() {
		using original_fn = bool(__thiscall*)(entity_t*);
		return (*(original_fn**)this)[165](this);
	}

	[[whynot]] bool is_an_dormant() // say me how work this getting dormant offset ((-_-
	{
		unsigned int a = (unsigned int)this;
		unsigned int* b = (unsigned int*)(a + 8);
		unsigned int c = *(unsigned int*)(*b + 32);

		return ((bool(__thiscall*)(unsigned int*))c)(b);
	}

	vec3_t get_absolute_origin() {
		__asm {
			MOV ECX, this
			MOV EAX, DWORD PTR DS : [ECX]
			CALL DWORD PTR DS : [EAX + 0x28]
		}
	}
	bool setup_bones(matrix_t* out, int max_bones, int mask, float time) {
		if (!this) {
			return false;
		}

		using original_fn = bool(__thiscall*)(void*, matrix_t*, int, int, float);
		return (*(original_fn**)animating())[13](animating(), out, max_bones, mask, time);
	}
	model_t* model() {
		using original_fn = model_t * (__thiscall*)(void*);
		return (*(original_fn**)animating())[8](animating());
	}
	void update() {
		using original_fn = void(__thiscall*)(entity_t*);
		(*(original_fn**)this)[218](this);
	}
	int draw_model(int flags, uint8_t alpha) {
		using original_fn = int(__thiscall*)(void*, int, uint8_t);
		return (*(original_fn**)animating())[9](animating(), flags, alpha);
	}
	void set_angles(vec3_t angles) {
		using original_fn = void(__thiscall*)(void*, const vec3_t&);
		static original_fn set_angles_fn = (original_fn)((DWORD)utilities::pattern_scan(("client.dll"), "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"));
		set_angles_fn(this, angles);
	}
	void set_position(vec3_t position) {
		using original_fn = void(__thiscall*)(void*, const vec3_t&);
		static original_fn set_position_fn = (original_fn)((DWORD)utilities::pattern_scan(("client.dll"), "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8"));
		set_position_fn(this, position);
	}
	vec3_t& get_world_space_center() {
		vec3_t vec_origin = origin();

		vec3_t min = this->collideable()->mins() + vec_origin;
		vec3_t max = this->collideable()->maxs() + vec_origin;

		vec3_t size = max - min;
		size /= 2.f;
		size += min;

		return size;
	}
	NETVAR("DT_CSPlayer", "m_fFlags", flags, int);
	NETVAR("DT_BaseEntity", "m_hOwnerEntity", owner_handle, unsigned long);
	NETVAR("DT_CSPlayer", "m_flSimulationTime", simulation_time, float);
	NETVAR("DT_BasePlayer", "m_vecOrigin", origin, vec3_t);
	NETVAR("DT_BasePlayer", "m_vecViewOffset[0]", view_offset, vec3_t);
	NETVAR("DT_CSPlayer", "m_iTeamNum", team, int);
	NETVAR("DT_BaseEntity", "m_bSpotted", spotted, bool);
	NETVAR("DT_CSPlayer", "m_flHealthShotBoostExpirationTime", healthshot, float);

	DORMANT(bool, dormant, 0xED); // dormant offset big shit
};

class ccs_bomb : public entity_t
{
public:
	NETVAR("DT_PlantedC4", "m_flDefuseCountDown", m_flDefuseCountDown, float);
	NETVAR("DT_PlantedC4", "m_hBombDefuser", m_hBombDefuser, int);
	NETVAR("DT_PlantedC4", "m_flC4Blow", m_flC4Blow, float);
	NETVAR("DT_PlantedC4", "m_bBombDefused", m_bBombDefused, bool);
};

class econ_view_item_t {
public:
	NETVAR("DT_ScriptCreatedItem", "m_bInitialized", is_initialized, bool);
	NETVAR("DT_ScriptCreatedItem", "m_iEntityLevel", entity_level, int);
	NETVAR("DT_ScriptCreatedItem", "m_iAccountID", account_id, int);
	NETVAR("DT_ScriptCreatedItem", "m_iItemIDLow", item_id_low, int);

};

class base_view_model : public entity_t {
public:
	NETVAR("DT_BaseViewModel", "m_nModelIndex", model_index, int);
	NETVAR("DT_BaseViewModel", "m_nViewModelIndex", view_model_index, int);
	NETVAR("DT_BaseViewModel", "m_hWeapon", m_hweapon, int);
	NETVAR("DT_BaseViewModel", "m_hOwner", m_howner, int);
};

enum data_update_type_t {
	DATA_UPDATE_CREATED = 0,
	DATA_UPDATE_DATATABLE_CHANGED,
};

class attributable_item_t : public entity_t {
public:
	NETVAR("DT_BaseViewModel", "m_nModelIndex", model_index, int);
	NETVAR("DT_BaseAttributableItem", "m_OriginalOwnerXuidLow", original_owner_xuid_low, int);
	NETVAR("DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh", original_owner_xuid_high, int);
	NETVAR("DT_BaseAttributableItem", "m_nFallbackStatTrak", fallback_stattrak, int);
	NETVAR("DT_BaseAttributableItem", "m_nFallbackPaintKit", fallback_paint_kit, int);
	NETVAR("DT_BaseAttributableItem", "m_nFallbackSeed", fallback_seed, int);
	NETVAR("DT_BaseAttributableItem", "m_iEntityQuality", entity_quality, int);
	NETVAR("DT_BaseAttributableItem", "m_flFallbackWear", fallback_wear, float);
	NETVAR("DT_BaseCombatWeapon", "m_hWeaponWorldModel", world_model_handle, unsigned long);
	NETVAR("DT_BaseAttributableItem", "m_iItemDefinitionIndex", item_definition_index, short);
	NETVAR("DT_BaseAttributableItem", "m_iItemIDHigh", item_id_high, int);

	void net_pre_data_update(int update_type)
	{
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)networkable())[6](networkable(), update_type);
	}

	void net_release() {
		using original_fn = void(__thiscall*)(void*);
		return (*(original_fn**)networkable())[1](networkable());
	}

	int net_set_destroyed_on_recreate_entities() {
		using original_fn = int(__thiscall*)(void*);
		return (*(original_fn**)networkable())[13](networkable());
	}


	void set_model_index(int index) {
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)this)[75](this, index);
	}

	base_view_model* get_view_model()
	{
		return (base_view_model*)(DWORD)this;
	}

	econ_view_item_t& item() {
		return *(econ_view_item_t*)this;
	}
};

class weapon_t : public entity_t {
public:
	NETVAR("DT_BaseCombatWeapon", "m_flNextPrimaryAttack", next_primary_attack, float);
	NETVAR("DT_BaseCombatWeapon", "m_flNextSecondaryAttack", next_secondary_attack, float);
	NETVAR("DT_BaseCombatWeapon", "m_iClip1", clip1_count, int);
	NETVAR("DT_BaseCombatWeapon", "m_iClip2", clip2_count, int);
	NETVAR("DT_BaseCombatWeapon", "m_iPrimaryReserveAmmoCount", primary_reserve_ammo_acount, int);
	NETVAR("DT_WeaponCSBase", "m_flRecoilIndex", recoil_index, float);
	NETVAR("DT_WeaponCSBase", "m_flPostponeFireReadyTime", m_flPostponeFireReadyTime, int);
	NETVAR("DT_WeaponCSBaseGun", "m_zoomLevel", zoom_level, float);
	NETVAR("DT_BaseAttributableItem", "m_iItemDefinitionIndex", item_definition_index, short)
	NETVAR("DT_BaseCombatWeapon", "m_iEntityQuality", entity_quality, int);

	float get_innacuracy() {
		using original_fn = float(__thiscall*)(void*);
		return (*(original_fn**)this)[482](this);
	}

	float get_spread() {
		using original_fn = float(__thiscall*)(void*);
		return (*(original_fn**)this)[452](this);
	}

	void update_accuracy_penalty() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[483](this);
	}

	//weapon_info_t* get_weapon_data() {
	//	using original_fn = weapon_info_t * (__thiscall*)(void*);
	//	return (*(original_fn**)this)[460](this);
	//}

	weapon_info_t* get_weapon_data() {
		return interfaces::weapon_system->get_weapon_data(
			this->item_definition_index());
	}
};

class player_t : public entity_t {
private:
	template <typename T>
	T& read(uintptr_t offset) {
		return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset);
	}

	template <typename T>
	void write(uintptr_t offset, T data) {
		*reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset) = data;
	}

public:
	NETVAR("DT_BasePlayer", "m_hViewModel[0]", view_model, int); //tutaj
	NETVAR("DT_CSPlayer", "m_bHasDefuser", has_defuser, bool);
	NETVAR("DT_CSPlayer", "m_bGunGameImmunity", has_gun_game_immunity, bool);
	NETVAR("DT_CSPlayer", "m_iShotsFired", shots_fired, int);
	NETVAR("DT_CSPlayer", "m_angEyeAngles", eye_angles, vec3_t);
	NETVAR("DT_CSPlayer", "m_ArmorValue", armor, int);
	NETVAR("DT_CSPlayer", "m_bHasHelmet", has_helmet, bool);
	NETVAR("DT_CSPlayer", "m_bIsScoped", is_scoped, bool);
	NETVAR("DT_CSPlayer", "m_bIsDefusing", is_defusing, bool);
	NETVAR("DT_CSPlayer", "m_iAccount", money, int);
	NETVAR("DT_CSPlayer", "m_flLowerBodyYawTarget", lower_body_yaw, float);
	NETVAR("DT_CSPlayer", "m_flNextAttack", next_attack, float);
	NETVAR("DT_CSPlayer", "m_flFlashDuration", flash_duration, float);
	NETVAR("DT_CSPlayer", "m_flFlashMaxAlpha", flash_alpha, float);
	NETVAR("DT_CSPlayer", "m_bHasNightVision", m_bHasNightVision, float);
	NETVAR("DT_CSPlayer", "m_bNightVisionOn", m_bNightVisionOn, float);
	NETVAR("DT_CSPlayer", "m_iHealth", health, int);
	NETVAR("DT_CSPlayer", "m_lifeState", life_state, int);
	NETVAR("DT_CSPlayer", "m_fFlags", flags, int);
	NETVAR("DT_BasePlayer", "m_viewPunchAngle", punch_angle, vec3_t);
	NETVAR("DT_BasePlayer", "m_aimPunchAngle", aim_punch_angle, vec3_t);
	NETVAR("DT_BasePlayer", "m_vecVelocity[0]", velocity, vec3_t);

	NETVAR("DT_CSPlayerResource", "m_iCompetitiveWins", get_wins_player, int);

	NETVAR("DT_BasePlayer", "m_flMaxspeed", max_speed, float);
	NETVAR("DT_BaseEntity", "m_flShadowCastDistance", m_flFOVTime, float);
	NETVAR("CBasePlayer", "m_iObserverMode", m_iObserverMode, int);
	NETVAR("DT_BasePlayer", "m_hObserverTarget", observer_target, unsigned long);
	NETVAR("DT_BasePlayer", "m_nHitboxSet", hitbox_set, int);
	NETVAR("DT_CSPlayer", "m_flDuckAmount", duck_amount, float);
	NETVAR("DT_CSPlayer", "m_bHasHeavyArmor", has_heavy_armor, bool);
	NETVAR("DT_PlantedC4", "m_flC4Blow", c4_blow_time, float);
	NETVAR("DT_SmokeGrenadeProjectile", "m_nSmokeEffectTickBegin", smoke_grenade_tick_begin, int);
	NETVAR("DT_CSPlayer", "m_nTickBase", get_tick_base, int);
	NETVAR("DT_CSPlayer", "m_iObserverMode", observer_mode, int long);

	vec3_t get_head_pos()
	{
		return this->get_bone_position(6);
	}

	int get_wins(int Index){
		DWORD dwTemp = *(DWORD*)(GetModuleHandleA("client.dll") + 0x2E7D6DC);
		return *(int*)((DWORD)dwTemp + get_wins_player() + (int)Index * 4);
	}


	int ammo()
	{
		static int m_iClip1 = sdk::util::getNetVar(sdk::util::fnv::hash("DT_BaseCombatWeapon"), sdk::util::fnv::hash("m_iClip1"));
		return *(int*)((uintptr_t)this + m_iClip1);
	}

	weapon_t* active_weapon() {
		auto active_weapon = read<DWORD>(sdk::util::getNetVar(sdk::util::fnv::hash("DT_CSPlayer"), sdk::util::fnv::hash("m_hActiveWeapon"))) & 0xFFF;
		return reinterpret_cast<weapon_t*>(interfaces::entity_list->get_client_entity(active_weapon));
	}

	UINT* get_wearables()
	{
		return (UINT*)((DWORD)this + (sdk::util::getNetVar(sdk::util::fnv::hash("DT_CSPlayer"), sdk::util::fnv::hash("m_hMyWearables"))));
	}

	bool has_c4() {
		static auto ret = reinterpret_cast<bool(__thiscall*)(void*)>(utilities::pattern_scan(("client.dll"), "56 8B F1 85 F6 74 31"));
		return ret(this);
	}

	vec3_t get_eye_pos() {
		return origin() + view_offset(); //hazedumper
	}

	bool can_see_player_pos(player_t* player, const vec3_t& pos) {
		trace_t tr;
		ray_t ray;
		trace_filter filter;
		filter.skip = this;

		auto start = get_eye_pos();
		auto dir = (pos - start).normalized();

		ray.initialize(start, pos);
		interfaces::trace_ray->trace_ray(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

		return tr.entity == player || tr.flFraction > 0.97f;
	}

	vec3_t get_bone_position(int bone) {
		matrix_t bone_matrices[128];
		if (setup_bones(bone_matrices, 128, 256, 0.0f))
			return vec3_t{ bone_matrices[bone][0][3], bone_matrices[bone][1][3], bone_matrices[bone][2][3] };
		else
			return vec3_t{ };
	}

	vec3_t get_hitbox_position(player_t* entity, int hitbox_id) {
		matrix_t bone_matrix[MAXSTUDIOBONES];

		if (entity->setup_bones(bone_matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f)) {
			auto studio_model = interfaces::model_info->get_studio_model(entity->model());

			if (studio_model) {
				auto hitbox = studio_model->hitbox_set(0)->hitbox(hitbox_id);

				if (hitbox) {
					auto min = vec3_t{}, max = vec3_t{};

					c_math::get().transform_vector(hitbox->mins, bone_matrix[hitbox->bone], min);
					c_math::get().transform_vector(hitbox->maxs, bone_matrix[hitbox->bone], max);

					return vec3_t((min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f, (min.z + max.z) * 0.5f);
				}
			}
		}
		return vec3_t{};
	}

	bool is_in_local_team() {
		return utilities::call_virtual<bool(__thiscall*)(void*)>(this, 92)(this);
	}

	bool alive() {
		using original_fn = bool(__thiscall*)(entity_t*);
		return (*(original_fn**)this)[155](this);
	}

	bool is_alive() {
		return life_state() == 0;
	}

	bool is_moving() {
		if (this->velocity().length() > 0.1f)
			return true;

		return false;
	}

	bool is_in_air() {
		if (this->flags() & fl_onground)
			return false;

		return true;
	}

	bool is_flashed() {
		if (this->flash_duration() > 0.0f)
			return true;

		return false;
	}

	void update_client_side_animations() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[223](this);
	}

	vec3_t& abs_origin() {
		using original_fn = vec3_t & (__thiscall*)(void*);
		return (*(original_fn**)this)[10](this);;
	}
	vec3_t& abs_angles() {
		using original_fn = vec3_t & (__thiscall*)(void*);
		return (*(original_fn**)this)[11](this);;
	}
	inline void* const get_client_renderable()
	{
		return reinterpret_cast<void*>(reinterpret_cast<DWORD>(this) + 0x4);
	}
	int	move_type() {
		return *reinterpret_cast<int*> (reinterpret_cast<uintptr_t>(this) + 0x25C); //hazedumper
	}
	vec3_t eye_pos() {
		vec3_t ret;
		utilities::call_virtual<void(__thiscall*)(void*, vec3_t&)>(this, 281)(this, ret); // this is the real eye pos
		return ret;
	}

	bool is_enemy()
	{
		const auto local = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

		if (!local)
			return false;

		return local->team() != team();
	}
};
