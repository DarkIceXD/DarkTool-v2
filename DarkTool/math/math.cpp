#include "math.h"
#include "../game/offsets.h"
#include "../imgui/imgui.h"

bool math::world_to_screen(const matrix4x4& matrix, const vector3& position, vector2& screen) noexcept
{
	const auto w = matrix._41 * position.x + matrix._42 * position.y + matrix._43 * position.z + matrix._44;
	if (w < 0.001f)
		return false;

	const auto& display = ImGui::GetIO().DisplaySize;
	screen.x = display.x / 2.0f;
	screen.y = display.y / 2.0f;
	screen.x *= 1.0f + (matrix._11 * position.x + matrix._12 * position.y + matrix._13 * position.z + matrix._14) / w;
	screen.y *= 1.0f - (matrix._21 * position.x + matrix._22 * position.y + matrix._23 * position.z + matrix._24) / w;
	return true;
}

vector2 math::calculate_angle(const vector3& src, const vector3& dst) noexcept
{
	const auto delta = src - dst;
	const auto hyp = std::sqrt(delta.x * delta.x + delta.y * delta.y);
	vector2 angle(std::atan(delta.z / hyp) * rad2deg, std::atan(delta.y / delta.x) * rad2deg);
	if (delta.x >= 0.0)
		angle.y += 180.0f;

	return angle;
}

float math::fov(const vector2& a, const vector2& b) noexcept
{
	return sin((b - a).normalized().length() * math::deg2rad / 2.0f) * 180.0f;
}
