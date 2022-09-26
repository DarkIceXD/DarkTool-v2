#include "features.h"
#include "../config/config.h"
#include "../memory/memory.h"
#include "../game/offsets.h"

void features::bhop(const data::game& data)
{
	if (!cfg.misc.bhop)
		return;

	if (!GetAsyncKeyState(VK_SPACE))
		return;

	if (data.local_player.entity.get_flags() & ONGROUND)
		memory::write(offsets::dwForceJump, 6);
}

void features::no_flash(const data::game& data)
{
	if (!cfg.misc.no_flash)
		return;

	data.local_player.entity.set_max_flash_alpha(cfg.misc.no_flash ? 0.f : 255.f);
}

void features::nade_helper(const data::game& data)
{
	if (!cfg.misc.nade_helper)
		return;


}