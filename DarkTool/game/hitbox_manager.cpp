#include "hitbox_manager.h"
#include "../memory/memory.h"
#include "../game/offsets.h"
#include <unordered_map>

constexpr auto size = static_cast<size_t>(hitbox::MAX);
using output = std::array<int, size>;

static std::unordered_map<int, output> hitboxes;
constexpr output fallback = { {8, 7, 0, 3, 4, 5, 6, 69, 76, 70, 77, 71, 78, 13, 42, 11, 12, 40, 41} };

const output& hitbox_manager::get_hitbox_lookup_table(const uintptr_t studio_hdr)
{
	const auto studio = memory::read<struct studio_hdr>(studio_hdr);
	if (studio.numHitboxSets < 1 || !studio.checksum)
		return fallback;

	const auto& key = studio.checksum;
	const auto found = hitboxes.find(key);
	if (found != hitboxes.end())
		return found->second;

	if (!studio.hitboxSetIndex)
		return fallback;

	const auto hitbox_set_ptr = studio_hdr + studio.hitboxSetIndex;
	const auto hitbox_set = memory::read<StudioHitboxSet>(hitbox_set_ptr);
	if (!hitbox_set.hitboxIndex || hitbox_set.numHitboxes != size)
		return fallback;

	const auto bboxes = memory::read<std::array<studio_bbox, size>>(hitbox_set_ptr + hitbox_set.hitboxIndex);
	auto& table = hitboxes[key];
	for (size_t i = 0; i < table.size(); i++)
	{
		const auto& bbox = bboxes[i];
		if (bbox.bone < 0 || bbox.bone >= 256)
		{
			hitboxes.erase(key);
			return fallback;
		}

		table[i] = bbox.bone;
	}
	return table;
}