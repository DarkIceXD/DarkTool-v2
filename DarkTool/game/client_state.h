#pragma once
#include <cstdint>
#include "structs.h"

struct client_state {
	client_state(const uintptr_t base);
	[[nodiscard]] int get_local_player_index() const;
	[[nodiscard]] int get_game_state() const;
	[[nodiscard]] uintptr_t get_model_precache() const;
	[[nodiscard]] uintptr_t get_player_info() const;
	[[nodiscard]] vector2 get_view_angles() const;
	bool set_view_angles(const vector2 view_angles) const;
	bool set_delta_ticks(const int delta_ticks) const;
	uintptr_t base;
};