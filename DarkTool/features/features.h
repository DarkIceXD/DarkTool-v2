#pragma once
#include "../imgui/imgui.h"
#include "../game/data.h"

namespace features {
	void esp(const data::game& data, ImDrawList* d, const matrix4x4& view_matrix);
	void aimbot(data::game& data, ImDrawList* d, const matrix4x4& view_matrix);
	void trigger(const data::game& data);
	void clan_tag_changer(const data::game& data);
	void skin_changer(const data::game& data);
	void bhop(const data::game& data);
	void no_flash(const data::game& data);
	void nade_helper(const data::game& data);
}