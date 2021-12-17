#pragma once
#include "../math/vector2.hpp"

struct matrix4x4 {
	union {
		struct {
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;

		};
		float m[4][4];
	};
};

struct matrix3x4 {
	float mat[3][4];
	constexpr vector3 origin() const noexcept
	{
		return { mat[0][3], mat[1][3], mat[2][3] };
	};
};

struct glow_object_manager {
	uintptr_t objects;
	int allocationCount;
	int growSize;
	int size;
};

struct glow_object_definition {
	int next_free_slot;
	uintptr_t entity;
	float r, g, b, a;
	bool glow_alpha_capped_by_render_alpha;
	float glow_alpha_function_of_max_velocity;
	float glow_alpha_max;
	float glow_pulse_overdrive;
	bool render_when_occluded;
	bool render_when_unoccluded;
	bool full_bloom_render;
	int full_bloom_stencil_test_value;
	int glow_style;
	int split_screen_slot;
	constexpr bool is_unused() const noexcept { return next_free_slot != ENTRY_IN_USE; }
	static constexpr int END_OF_FREE_LIST = -1;
	static constexpr int ENTRY_IN_USE = -2;
};

struct global_vars {
	float realtime;
	int framecount;
	float absolute_frame_time;
	uint8_t pad0[4];
	float currenttime;
	float frametime;
	int max_clients;
	int tick_count;
	float interval_per_tick;
};

enum class hitbox {
	HEAD,
	NECK,
	PELVIS,
	BELLY,
	THORAX,
	LOWER_CHEST,
	UPPER_CHEST,
	RIGHT_THIGH,
	LEFT_THIGH,
	RIGHT_CALF,
	LEFT_CALF,
	RIGHT_FOOT,
	LEFT_FOOT,
	RIGHT_HAND,
	LEFT_HAND,
	RIGHT_UPPER_ARM,
	RIGHT_FORE_ARM,
	LEFT_UPPER_ARM,
	LEFT_FORE_ARM,
	MAX
};

struct studio_bbox {
	int bone;
	int group;
	vector3 bb_min;
	vector3 bb_max;
	int hitbox_name_index;
	vector3 offset_orientation;
	float capsule_radius;
	int	unused[4];
};

struct StudioHitboxSet {
	int nameIndex;
	int numHitboxes;
	int hitboxIndex;
};

struct studio_hdr {
	int id;
	int version;
	int checksum;
	char name[64];
	int length;
	vector3 eyePosition;
	vector3 illumPosition;
	vector3 hullMin;
	vector3 hullMax;
	vector3 bbMin;
	vector3 bbMax;
	int flags;
	int numBones;
	int boneIndex;
	int numBoneControllers;
	int boneControllerIndex;
	int numHitboxSets;
	int hitboxSetIndex;
};

enum item_definition_index : short
{
	WEAPON_DEAGLE = 1,
	WEAPON_ELITE = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALILAR = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_MP5_SD = 23,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_KNIFE = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKEGRENADE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCGRENADE = 48,
	WEAPON_C4 = 49,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER = 60,
	WEAPON_USP_SILENCER = 61,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_CSS = 503,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516,
	WEAPON_KNIFE_CORD = 517,
	WEAPON_KNIFE_CANIS = 518,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE = 520,
	WEAPON_KNIFE_OUTDOOR = 521,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER = 523,
	WEAPON_KNIFE_SKELETON = 525,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034,
	GLOVE_HYDRA = 5035
};

constexpr bool is_good_weapon(const short weapon)
{
	switch (weapon)
	{
	case 0:
	case WEAPON_KNIFE:
	case WEAPON_FLASHBANG:
	case WEAPON_HEGRENADE:
	case WEAPON_SMOKEGRENADE:
	case WEAPON_MOLOTOV:
	case WEAPON_DECOY:
	case WEAPON_INCGRENADE:
	case WEAPON_C4:
	case WEAPON_KNIFE_T:
	case WEAPON_KNIFE_BAYONET:
	case WEAPON_KNIFE_CSS:
	case WEAPON_KNIFE_FLIP:
	case WEAPON_KNIFE_GUT:
	case WEAPON_KNIFE_KARAMBIT:
	case WEAPON_KNIFE_M9_BAYONET:
	case WEAPON_KNIFE_TACTICAL:
	case WEAPON_KNIFE_FALCHION:
	case WEAPON_KNIFE_SURVIVAL_BOWIE:
	case WEAPON_KNIFE_BUTTERFLY:
	case WEAPON_KNIFE_PUSH:
	case WEAPON_KNIFE_CORD:
	case WEAPON_KNIFE_CANIS:
	case WEAPON_KNIFE_URSUS:
	case WEAPON_KNIFE_GYPSY_JACKKNIFE:
	case WEAPON_KNIFE_OUTDOOR:
	case WEAPON_KNIFE_STILETTO:
	case WEAPON_KNIFE_WIDOWMAKER:
	case WEAPON_KNIFE_SKELETON:
		return false;
	default:
		return true;
	}
}

enum entity_flags {
	ONGROUND = (1 << 0),
	DUCKING = (1 << 1),
	ANIMDUCKING = (1 << 2),
	WATERJUMP = (1 << 3),
	ONTRAIN = (1 << 4),
	INRAIN = (1 << 5),
	FROZEN = (1 << 6),
	ATCONTROLS = (1 << 7),
	CLIENT = (1 << 8),
	FAKECLIENT = (1 << 9),
	INWATER = (1 << 10),
	FLY = (1 << 11),
	SWIM = (1 << 12),
	CONVEYOR = (1 << 13),
	NPC = (1 << 14),
	GODMODE = (1 << 15),
	NOTARGET = (1 << 16),
	AIMTARGET = (1 << 17),
	PARTIALGROUND = (1 << 18),
	STATICPROP = (1 << 19),
	GRAPHED = (1 << 20),
	GRENADE = (1 << 21),
	STEPMOVEMENT = (1 << 22),
	DONTTOUCH = (1 << 23),
	BASEVELOCITY = (1 << 24),
	WORLDBRUSH = (1 << 25),
	OBJECT = (1 << 26),
	KILLME = (1 << 27),
	ONFIRE = (1 << 28),
	DISSOLVING = (1 << 29),
	TRANSRAGDOLL = (1 << 30),
	UNBLOCKABLE_BY_PLAYER = (1 << 31)
};