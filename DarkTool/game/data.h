#pragma once
#include "player.h"
#include "weapon.h"
#include "client_state.h"
#include <array>

namespace data {
	constexpr auto bone_count = 19;
	struct player_data {
		vector3 origin;
		std::array<vector3, bone_count> hitboxes;
		struct bone_screen {
			vector2 screen;
			bool valid;
		};
		std::array<bone_screen, bone_count> hitboxes_screen;
		char name[128];
		int health;
		int flags;
		bool visible;
		bool gun_game_immunity;
		bool friendly;
		bool valid;
	};
	struct local_player {
		vector3 head, origin;
		vector2 aim_punch_angle;
		vector2 view_angles;
		player entity{0};
		uint32_t xuid_low;
		float next_fire_time;
		float inaccuracy;
		int clip;
		int cross_entity;
		short weapon_def_index;
		bool can_attack;
	};
	struct game {
		client_state client_state{0};
		global_vars vars;
		std::array<player_data, 64> players;
		local_player local_player;
		bool valid;
	};
	void update(game& data);
	void fast_loop(const game& data);
}