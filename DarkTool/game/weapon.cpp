#include "weapon.h"
#include "../memory/memory.h"
#include "../game/offsets.h"

weapon::weapon(const uintptr_t entity) : entity(entity) {}

weapon::weapon() : weapon(0) {}

short weapon::get_definition_index() const
{
	return memory::read<short>(entity + offsets::m_iItemDefinitionIndex);
}

bool weapon::set_definition_index(const short index) const
{
	return memory::write(entity + offsets::m_iItemDefinitionIndex, index);
}

int weapon::get_clip() const
{
	return memory::read<int>(entity + offsets::m_iClip1);
}

float weapon::get_next_primary_attack() const
{
	return memory::read<float>(entity + offsets::m_flNextPrimaryAttack);
}

float weapon::get_inaccuracy() const
{
	return memory::read<float>(entity + offsets::m_fAccuracyPenalty);
}

int weapon::get_xuid_low() const
{
	return memory::read<int>(entity + offsets::m_OriginalOwnerXuidLow);
}

bool weapon::set_model_index(const unsigned int index) const
{
	return memory::write(entity + offsets::m_nModelIndex, index);
}

int weapon::get_item_id_high() const
{
	return memory::read<int>(entity + offsets::m_iItemIDHigh);
}

bool weapon::set_item_id_high(const int item_id) const
{
	return memory::write(entity + offsets::m_iItemIDHigh, item_id);
}

bool weapon::set_account_id(const int account_id) const
{
	return memory::write(entity + offsets::m_iAccountID, account_id);
}

bool weapon::set_entity_quality(const int quality) const
{
	return memory::write(entity + offsets::m_iEntityQuality, quality);
}

bool weapon::set_name(const std::string_view name) const
{
	return memory::write(entity + offsets::m_szCustomName, (void*)name.data(), name.size());
}

bool weapon::set_paint_kit(const int paint_kit) const
{
	return memory::write(entity + offsets::m_nFallbackPaintKit, paint_kit);
}

bool weapon::set_seed(const int seed) const
{
	return memory::write(entity + offsets::m_nFallbackSeed, seed);
}

bool weapon::set_wear(const float wear) const
{
	return memory::write(entity + offsets::m_flFallbackWear, wear);
}

bool weapon::set_stattrak(const int stattrak) const
{
	return memory::write(entity + offsets::m_nFallbackStatTrak, stattrak);
}