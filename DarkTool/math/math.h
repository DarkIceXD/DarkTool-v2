#pragma once
#include "../game/structs.h"
#include "vector2.hpp"

namespace math {
	constexpr float pi = 3.14159265358979323846f;
	constexpr float deg2rad = pi / 180;
	constexpr float rad2deg = 180 / pi;
	[[nodiscard]] bool world_to_screen(const matrix4x4& matrix, const vector3& worldPosition, vector2& screenPosition) noexcept;
	[[nodiscard]] vector2 calculate_angle(const vector3& src, const vector3& dst) noexcept;
	[[nodiscard]] float fov(const vector2& a, const vector2& b) noexcept;
}