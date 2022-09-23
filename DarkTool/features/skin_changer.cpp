#include "features.h"
#include "../config/config.h"
#include "../memory/memory.h"
#include "../game/offsets.h"

constexpr const char* get_model_name(const short item_index)
{
	switch (item_index)
	{
	case WEAPON_KNIFE_BAYONET:
		return "models/weapons/v_knife_bayonet.mdl";
	case WEAPON_KNIFE_FLIP:
		return "models/weapons/v_knife_flip.mdl";
	case WEAPON_KNIFE_GUT:
		return "models/weapons/v_knife_gut.mdl";
	case WEAPON_KNIFE_KARAMBIT:
		return "models/weapons/v_knife_karam.mdl";
	case WEAPON_KNIFE_M9_BAYONET:
		return "models/weapons/v_knife_m9_bay.mdl";
	case WEAPON_KNIFE_TACTICAL:
		return "models/weapons/v_knife_tactical.mdl";
	case WEAPON_KNIFE_FALCHION:
		return "models/weapons/v_knife_falchion_advanced.mdl";
	case WEAPON_KNIFE_SURVIVAL_BOWIE:
		return "models/weapons/v_knife_survival_bowie.mdl";
	case WEAPON_KNIFE_BUTTERFLY:
		return "models/weapons/v_knife_butterfly.mdl";
	case WEAPON_KNIFE_PUSH:
		return "models/weapons/v_knife_push.mdl";
	case WEAPON_KNIFE_URSUS:
		return "models/weapons/v_knife_ursus.mdl";
	case WEAPON_KNIFE_GYPSY_JACKKNIFE:
		return "models/weapons/v_knife_gypsy_jackknife.mdl";
	case WEAPON_KNIFE_STILETTO:
		return "models/weapons/v_knife_stiletto.mdl";
	case WEAPON_KNIFE_WIDOWMAKER:
		return "models/weapons/v_knife_widowmaker.mdl";
	case WEAPON_KNIFE_CSS:
		return "models/weapons/v_knife_css.mdl";
	case WEAPON_KNIFE_CORD:
		return "models/weapons/v_knife_cord.mdl";
	case WEAPON_KNIFE_CANIS:
		return "models/weapons/v_knife_canis.mdl";
	case WEAPON_KNIFE_OUTDOOR:
		return "models/weapons/v_knife_outdoor.mdl";
	case WEAPON_KNIFE_SKELETON:
		return "models/weapons/v_knife_skeleton.mdl";
	default:
		return nullptr;
	}
}

constexpr unsigned int get_model_index(const short item_index, const data::game& data)
{
	const auto model_name = get_model_name(item_index);
	if (!model_name)
		return 0;

	const auto nsdi = memory::read<uintptr_t>(memory::read<uintptr_t>(data.client_state.get_model_precache() + 0x40) + 0xC) + 0xC;
	for (unsigned int i = 0; i < 1024; i++)
	{
		const auto nsdi_i = memory::read<uintptr_t>(nsdi + i * 0x34);
		char current_model_name[128] = { 0 };
		if (memory::read(nsdi_i, current_model_name, sizeof(current_model_name)) && !strcmp(current_model_name, model_name))
			return i;
	}
	return 0;
}

void features::skin_changer(const data::game& data)
{
	static unsigned int modelindex = 0;
	auto rewrite = cfg->skin_changer.update_now;
	const auto knife = cfg->skin_changer.get_knife_index();
	for (int i = 0; i < 4; i++)
	{
		const weapon weapon = memory::read<uintptr_t>(offsets::dwEntityList + data.local_player.entity.get_weapon(i) * 0x10);
		if (!weapon.entity)
			continue;

		const auto owner = weapon.get_xuid_low();
		if (owner != data.local_player.xuid_low)
			continue;

		const auto def_index = weapon.get_definition_index();
		if (knife > 0 && (def_index == WEAPON_KNIFE || def_index == WEAPON_KNIFE_T))
		{
			if (!modelindex)
				modelindex = get_model_index(knife, data);
			if (!modelindex)
				return;
			weapon.set_definition_index(knife);
			weapon.set_model_index(modelindex);
		}

		const auto skin = cfg->skin_changer.get_skin(def_index);
		if (!skin)
			continue;

		if (!skin->paint_kit)
			continue;

		if (weapon.get_item_id_high() == -1)
			continue;

		weapon.set_item_id_high(-1);
		weapon.set_paint_kit(skin->paint_kit);
		weapon.set_wear(skin->wear);
		if (skin->stat_trak)
		{
			weapon.set_account_id(data.local_player.xuid_low);
			weapon.set_stattrak(skin->stat_trak);
		}
		if (skin->seed)
		{
			weapon.set_seed(skin->seed);
		}
		if (!skin->nametag.empty())
		{
			weapon.set_name(skin->nametag);
		}
		else if (skin->quality)
		{
			weapon.set_entity_quality(skin->quality);
		}
		rewrite = true;
	}
	if (rewrite)
	{
		data.client_state.set_delta_ticks(-1);
		cfg->skin_changer.update_now = false;
	}
	else
	{
		if (data.local_player.weapon_def_index == knife)
		{
			const auto activeViewModel = memory::read<uintptr_t>(offsets::dwEntityList + data.local_player.entity.get_view_model() * 0x10);
			if (!activeViewModel)
				return;
			memory::write(activeViewModel + offsets::m_nModelIndex, modelindex);
		}
		/*const auto new_kills = memory::read<int>(memory::read<uintptr_t>(offsets::dwPlayerResource) + offsets::m_iKills + memory::read<int>(localplayer + 0x64) * 4);
			if (new_kills > kills && memory::read<int>(activeWeapon + offsets::m_OriginalOwnerXuidLow) == steamID.xUIDLow)
				conf->add_kill(weaponIndex);
			kills = new_kills;*/
	}
}