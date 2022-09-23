#include "../overlay/overlay.hpp"
#include "features.h"
#include "../math/math.h"
#include "../game/offsets.h"
#include "../game/hitbox_manager.h"
#include "../config/config.h"
#include "../memory/memory.h"

void overlay::draw(data::game& data, ImDrawList* d)
{
	if (!data.valid)
		return;

	const auto view_matrix = memory::read<matrix4x4>(offsets::dwViewMatrix);

	if ((cfg->esp.skeleton.base.to_u32() & 0xFF000000) || (cfg->esp.skeleton.visible.to_u32() & 0xFF000000))
		for (auto& player : data.players)
		{
			if (!player.valid)
				continue;

			for (size_t i = 0; i < player.hitboxes.size(); i++)
			{
				const auto& bone = player.hitboxes[i];
				auto& screen = player.hitboxes_screen[i];
				screen.valid = false;

				if (!math::world_to_screen(view_matrix, bone, screen.screen))
					continue;

				screen.valid = true;
			}
		}
	features::esp(data, d, view_matrix);
	features::aimbot(data, d, view_matrix);
}

void data::update(data::game& data)
{
	data.valid = false;
	data.client_state = memory::read<uintptr_t>(offsets::dwClientState);
	if (data.client_state.get_game_state() < 6)
		return;

	data.local_player.entity = memory::read<uintptr_t>(offsets::dwLocalPlayer);
	if (!data.local_player.entity.entity)
		return;

	const auto local_team = data.local_player.entity.get_team();
	data.local_player.origin = data.local_player.entity.get_origin();
	data.local_player.head = data.local_player.origin + data.local_player.entity.get_view_offset();
	data.local_player.aim_punch_angle = data.local_player.entity.get_aim_punch_angle() * 2;
	data.local_player.view_angles = data.client_state.get_view_angles();
	data.local_player.cross_entity = data.local_player.entity.get_cross();
	data.vars = memory::read<global_vars>(offsets::dwGlobalVars);
	const weapon active_weapon(memory::read<uintptr_t>(offsets::dwEntityList + data.local_player.entity.get_active_weapon() * 0x10));
	data.local_player.weapon_def_index = active_weapon.get_definition_index();
	data.local_player.clip = active_weapon.get_clip();
	data.local_player.next_fire_time = active_weapon.get_next_primary_attack() - data.vars.currenttime;
	data.local_player.inaccuracy = active_weapon.get_inaccuracy();
	const auto local_player_index = data.client_state.get_local_player_index();
	const auto items = memory::read<uintptr_t>(memory::read<uintptr_t>(data.client_state.get_player_info() + 0x40) + 0xC);
	auto local_player_info = player::get_player_info(items, player::base_to_index(data.local_player.entity.entity));
	local_player_info.steam_id64 = _byteswap_uint64(local_player_info.steam_id64);
	data.local_player.xuid_low = local_player_info.xuid_low;
	for (int i = 1; i < data.vars.max_clients; i++)
	{
		auto& player_data = data.players[i - 1];
		player_data.valid = false;

		if (i == local_player_index)
			continue;

		const player p(memory::read<uintptr_t>(offsets::dwEntityList + i * 0x10));

		const auto dormant = p.is_dormant();
		if (dormant)
			continue;

		player_data.health = p.get_health();
		if (player_data.health < 1)
			continue;

		const auto info = player::get_player_info(items, i);
		strcpy_s(player_data.name, info.name);
		player_data.flags = p.get_flags();
		player_data.gun_game_immunity = p.has_gun_game_immunity();
		player_data.friendly = local_team == p.get_team();
		player_data.visible = p.get_spotted_mask() & (1 << local_player_index);
		player_data.origin = p.get_origin();
		const auto& lookup = hitbox_manager::get_hitbox_lookup_table(p.get_studio_hdr_ptr());
		const auto bones = p.get_bones(p.get_bone_matrix_ptr());
		for (size_t j = 0; j < player_data.hitboxes.size(); j++)
			player_data.hitboxes[j] = bones[lookup[j]].origin();

		player_data.valid = true;
	}
	for (int i = data.vars.max_clients; i < data.players.size(); i++)
		data.players[i].valid = false;

	data.valid = true;
	features::no_flash(data);
	features::nade_helper(data);
	features::clan_tag_changer(data);
}

void data::fast_loop(const data::game& data)
{
	if (!data.valid)
		return;

	features::trigger(data);
	features::skin_changer(data);
	features::bhop(data);
}