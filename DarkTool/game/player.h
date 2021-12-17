#pragma once
#include <cstdint>
#include <array>
#include "structs.h"

struct player {
	static constexpr auto MAXSTUDIOBONES = 256;
	static constexpr std::array<std::pair<hitbox, hitbox>, 18> hitbox_connections = { {
		{hitbox::HEAD, hitbox::NECK},
		{hitbox::NECK, hitbox::UPPER_CHEST},
		{hitbox::UPPER_CHEST, hitbox::LOWER_CHEST},
		{hitbox::LOWER_CHEST, hitbox::THORAX},
		{hitbox::THORAX, hitbox::BELLY},
		{hitbox::BELLY, hitbox::PELVIS},

		{hitbox::NECK, hitbox::RIGHT_UPPER_ARM},
		{hitbox::RIGHT_UPPER_ARM, hitbox::RIGHT_FORE_ARM},
		{hitbox::RIGHT_FORE_ARM, hitbox::RIGHT_HAND},

		{hitbox::NECK, hitbox::LEFT_UPPER_ARM},
		{hitbox::LEFT_UPPER_ARM, hitbox::LEFT_FORE_ARM},
		{hitbox::LEFT_FORE_ARM, hitbox::LEFT_HAND},

		{hitbox::PELVIS, hitbox::RIGHT_THIGH},
		{hitbox::RIGHT_THIGH, hitbox::RIGHT_CALF},
		{hitbox::RIGHT_CALF, hitbox::RIGHT_FOOT},

		{hitbox::PELVIS, hitbox::LEFT_THIGH},
		{hitbox::LEFT_THIGH, hitbox::LEFT_CALF},
		{hitbox::LEFT_CALF, hitbox::LEFT_FOOT},
	} };
	player(const uintptr_t base);
	[[nodiscard]] bool is_dormant() const;
	[[nodiscard]] bool has_gun_game_immunity() const;
	[[nodiscard]] int get_team() const;
	[[nodiscard]] int get_health() const;
	[[nodiscard]] int get_spotted_mask() const;
	[[nodiscard]] int get_flags() const;
	[[nodiscard]] int get_cross() const;
	[[nodiscard]] float get_max_flash_alpha() const;
	bool set_max_flash_alpha(const float max_alpha) const;
	[[nodiscard]] vector3 get_origin() const;
	[[nodiscard]] vector3 get_view_offset() const;
	[[nodiscard]] vector2 get_aim_punch_angle() const;
	[[nodiscard]] uintptr_t get_view_model() const;
	[[nodiscard]] uintptr_t get_active_weapon() const;
	[[nodiscard]] uintptr_t get_weapon(const int index) const;
	[[nodiscard]] uintptr_t get_studio_hdr_ptr() const;
	[[nodiscard]] uintptr_t get_bone_matrix_ptr() const;
	[[nodiscard]] std::array<matrix3x4, MAXSTUDIOBONES> get_bones(const uintptr_t bone_matrix) const;
	[[nodiscard]] matrix3x4 get_bone(const uintptr_t bone_matrix, const int bone) const;
	[[nodiscard]] static bool get_bounding_box_fallback(vector2& min, vector2& max, const matrix4x4& matrix, const vector3& origin_pos, const int flags);
	[[nodiscard]] static float estimate_head_position(const int flags);
	uintptr_t entity;
};