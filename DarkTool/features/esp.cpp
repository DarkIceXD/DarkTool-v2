#include "features.h"
#include "../config/config.h"
#include "../math/math.h"

void features::esp(const data::game& data, ImDrawList* d, const matrix4x4& view_matrix)
{
	cfg->esp.bind.run();
	if (!cfg->esp.bind.enabled)
		return;

	const auto box_base_color = cfg->esp.box.base.to_u32();
	const auto box_visible_color = cfg->esp.box.visible.to_u32();
	const auto skeleton_base_color = cfg->esp.skeleton.base.to_u32();
	const auto skeleton_visible_color = cfg->esp.skeleton.visible.to_u32();
	for (const auto& player : data.players)
	{
		if (!player.valid)
			continue;

		if (player.friendly)
			continue;

		vector2 min, max;
		if (!player::get_bounding_box_fallback(min, max, view_matrix, player.origin, player.flags))
			continue;

		const auto box_color = player.visible ? box_visible_color : box_base_color;
		d->AddRect(min, max, box_color);
		/*const auto middle = (max.x - min.x) / 2 + min.x;
		char name[64] = "test";
		const auto name_size = ImGui::CalcTextSize(name);
		d->AddText({ middle - name_size.x / 2, max.y - name_size.y }, IM_COL32_WHITE, name);*/
		const ImVec2 hp_min = { max.x + 2, min.y };
		const auto hp = player.health / 100.f;
		const ImVec2 hp_max = { hp_min.x + 3, hp_min.y + (max.y - min.y) * hp };
		d->AddRectFilled(hp_min, hp_max, rgb::scale({ 0, 1, 0, 1 }, { 1, 0, 0, 1 }, hp).to_u32());
		d->AddRect(hp_min, hp_max, IM_COL32_BLACK);

		const auto skeleton_color = player.visible ? skeleton_visible_color : skeleton_base_color;
		if ((skeleton_color & 0xFF000000))
			for (const auto& bone_connection : player::hitbox_connections)
			{
				const auto& a = player.hitboxes_screen[static_cast<size_t>(bone_connection.first)];
				if (!a.valid)
					continue;

				const auto& b = player.hitboxes_screen[static_cast<size_t>(bone_connection.second)];
				if (!b.valid)
					continue;

				d->AddLine(a.screen, b.screen, skeleton_color);
			}

	}
}