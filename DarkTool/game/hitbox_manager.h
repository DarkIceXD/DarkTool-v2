#pragma once
#include <cstdint>
#include <array>
#include "structs.h"

namespace hitbox_manager
{
	[[nodiscard]] const std::array<int, 19>& get_hitbox_lookup_table(const uintptr_t studio_hdr);
};