#include "player.h"
#include "../memory/memory.h"
#include "../game/offsets.h"
#include "../math/math.h"

player::player(const uintptr_t entity) : entity(entity) {}

bool player::is_dormant() const
{
	return memory::read<bool>(entity + offsets::m_bDormant);
}

bool player::has_gun_game_immunity() const
{
	return memory::read<bool>(entity + offsets::m_bGunGameImmunity);
}

int player::get_team() const
{
	return memory::read<int>(entity + offsets::m_iTeamNum);
}

int player::get_health() const
{
	return memory::read<int>(entity + offsets::m_iHealth);
}

int player::get_spotted_mask() const
{
	return memory::read<int>(entity + offsets::m_bSpottedByMask);
}

int player::get_flags() const
{
	return memory::read<int>(entity + offsets::m_fFlags);
}

int player::get_cross() const
{
	return memory::read<int>(entity + offsets::m_iCrosshairId);
}

float player::get_max_flash_alpha() const
{
	return memory::read<float>(entity + offsets::m_flFlashMaxAlpha);
}

bool player::set_max_flash_alpha(const float max_alpha) const
{
	return memory::write(entity + offsets::m_flFlashMaxAlpha, max_alpha);
}

vector3 player::get_origin() const
{
	return memory::read<vector3>(entity + offsets::m_vecOrigin);
}

vector3 player::get_view_offset() const
{
	return memory::read<vector3>(entity + offsets::m_vecViewOffset);
}

vector2 player::get_aim_punch_angle() const
{
	return memory::read<vector2>(entity + offsets::m_aimPunchAngle);
}

uintptr_t player::get_view_model() const
{
	return (memory::read<uintptr_t>(entity + offsets::m_hViewModel) & 0xFFF) - 1;
}

uintptr_t player::get_active_weapon() const
{
	return (memory::read<uintptr_t>(entity + offsets::m_hActiveWeapon) & 0xFFF) - 1;
}

uintptr_t player::get_weapon(const int index) const
{
	return ((memory::read<uintptr_t>(entity + offsets::m_hMyWeapons + index * sizeof(uintptr_t)) & 0xFFF) - 1);
}

uintptr_t player::get_studio_hdr_ptr() const
{
	const auto ptr = memory::read<uintptr_t>(entity + offsets::m_pStudioHdr);
	return memory::read<uintptr_t>(ptr);
}

uintptr_t player::get_bone_matrix_ptr() const
{
	return memory::read<uintptr_t>(entity + offsets::m_dwBoneMatrix);
}

std::array<matrix3x4, player::MAXSTUDIOBONES> player::get_bones(const uintptr_t bone_matrix) const
{
	return memory::read<std::array<matrix3x4, player::MAXSTUDIOBONES>>(bone_matrix);
}

matrix3x4 player::get_bone(const uintptr_t bone_matrix, const int bone) const
{
	return memory::read<matrix3x4>(bone_matrix + 0x30 * bone);
}

bool player::get_bounding_box_fallback(vector2& min, vector2& max, const matrix4x4& matrix, const vector3& origin_pos, const int flags)
{
	const auto head_pos = origin_pos + vector3(0, 0, estimate_head_position(flags));

	vector2 head_pos_screen, feet_pos_screen;
	if (!math::world_to_screen(matrix, head_pos, head_pos_screen) ||
		!math::world_to_screen(matrix, origin_pos, feet_pos_screen))
		return false;

	const auto width = (feet_pos_screen.y - head_pos_screen.y) * 0.3f;
	min = { feet_pos_screen.x - width, feet_pos_screen.y };
	max = { feet_pos_screen.x + width, head_pos_screen.y };

	return true;
}

float player::estimate_head_position(const int flags)
{
	if (flags & DUCKING)
		return 55;
	else
		return 72;
}