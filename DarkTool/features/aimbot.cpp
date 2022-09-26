#include "features.h"
#include "../config/config.h"
#include "../math/math.h"
#include "../memory/memory.h"
#include "../game/offsets.h"

void aim_at(const vector2& screen, const bool absolute)
{
	INPUT input = { 0 };
	input.type = INPUT_MOUSE;
	if (absolute)
	{
		input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
		input.mi.dx = screen.x * (65536.f / GetSystemMetrics(SM_CXSCREEN));
		input.mi.dy = screen.y * (65536.f / GetSystemMetrics(SM_CYSCREEN));
	}
	else
	{
		input.mi.dwFlags = MOUSEEVENTF_MOVE;
		input.mi.dx = screen.x;
		input.mi.dy = screen.y;
	}
	SendInput(1, &input, sizeof(input));
}

void features::aimbot(data::game& data, ImDrawList* d, const matrix4x4& view_matrix)
{
	cfg.aimbot.bind.run();
	if (!cfg.aimbot.bind.enabled)
		return;

	if (!is_good_weapon(data.local_player.weapon_def_index))
		return;

	auto smallest_fov = FLT_MAX;
	data::player_data const* p = nullptr;
	for (const auto& player : data.players)
	{
		if (!player.valid)
			continue;

		if (player.friendly)
			continue;

		if (player.gun_game_immunity)
			continue;

		if (cfg.aimbot.visibility_check && !player.visible)
			continue;

		const auto& hitbox = player.hitboxes[static_cast<size_t>(hitbox::UPPER_CHEST)];
		const auto angle = (math::calculate_angle(data.local_player.head, hitbox) - data.local_player.aim_punch_angle).normalized();
		const auto fov = math::fov(data.local_player.view_angles, angle);
		if (fov < smallest_fov)
		{
			smallest_fov = fov;
			p = &player;
		}
	}

	if (smallest_fov >= FLT_MAX || smallest_fov > cfg.aimbot.fov)
		return;

	smallest_fov = FLT_MAX;
	vector3 best_hitbox;
	vector2 best_angle;
	for (size_t i = 0; i <= static_cast<size_t>(hitbox::MAX); i++)
	{
		if (!cfg.aimbot.is_hitbox_enabled(static_cast<hitbox>(i)))
			continue;

		const auto& hitbox = p->hitboxes[i];
		const auto angle = (math::calculate_angle(data.local_player.head, hitbox) - data.local_player.aim_punch_angle).normalized();
		const auto fov = math::fov(data.local_player.view_angles, angle);
		if (fov < smallest_fov)
		{
			smallest_fov = fov;
			best_hitbox = hitbox;
			best_angle = angle;
		}
	}

	if (cfg.aimbot.show_aim_spot)
	{
		vector2 best_hitbox_screen;
		if (math::world_to_screen(view_matrix, best_hitbox, best_hitbox_screen))
		{
			d->AddLine(best_hitbox_screen - vector2(5, 5), best_hitbox_screen + vector2(5, 5), IM_COL32_WHITE);
			d->AddLine(best_hitbox_screen - vector2(5, -5), best_hitbox_screen + vector2(5, -5), IM_COL32_WHITE);
		}
	}

	if (smallest_fov > 1)
	{
		auto diff = (best_angle - data.local_player.view_angles).normalized();
		best_angle = (data.local_player.view_angles + diff / cfg.aimbot.smoothness).normalized();
	}
	data.client_state.set_view_angles(best_angle);
	data.local_player.view_angles = best_angle;
}