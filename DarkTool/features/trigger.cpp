#include "features.h"
#include "../config/config.h"
#include "../memory/memory.h"
#include "../game/offsets.h"

float hitchance(const float inaccuracy, const hitbox hitbox, const float distance) {
	float multiplier = 1;
	switch (hitbox) {
	case hitbox::HEAD:
	case hitbox::NECK:
	case hitbox::PELVIS:
	case hitbox::BELLY:
	case hitbox::THORAX:
	case hitbox::LOWER_CHEST:
	case hitbox::UPPER_CHEST:
		multiplier = 2;
		break;
	case hitbox::LEFT_THIGH:
	case hitbox::RIGHT_THIGH:
		multiplier = 1.5f;
		break;
	}
	const auto inaccuracy_clamped = (std::max)(inaccuracy, 0.0000001f);
	const auto b = sqrt(tan(inaccuracy_clamped * 3.932f) * distance);
	return 5.1432f * 100.f * multiplier / b;
}

void features::trigger(const data::game& data)
{
	cfg->trigger.bind.run();
	if (!cfg->trigger.bind.enabled)
		return;

	if (!data.local_player.clip)
		return;

	const auto def_index = data.local_player.weapon_def_index;
	if (!is_good_weapon(def_index))
		return;

	const auto& cross = data.local_player.cross_entity;
	if (cross < 1 || cross > 64)
		return;

	const auto& player = data.players[cross - 2];
	if (!player.valid)
		return;

	if (player.friendly)
		return;

	if (player.gun_game_immunity)
		return;

	if (hitchance(data.local_player.inaccuracy, hitbox::HEAD, (data.local_player.origin - player.origin).length()) < cfg->trigger.get_hitchance(def_index))
		return;

	memory::write(offsets::dwForceAttack, 6);
}