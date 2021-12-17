#include "client_state.h"
#include "../memory/memory.h"
#include "../game/offsets.h"

client_state::client_state(const uintptr_t base) : base(base) {}

int client_state::get_local_player_index() const
{
	return memory::read<int>(base + offsets::dwClientState_GetLocalPlayer);
}

int client_state::get_game_state() const
{
	return memory::read<int>(base + offsets::dwClientState_State);
}

uintptr_t client_state::get_model_precache() const
{
	return memory::read<uintptr_t>(base + offsets::m_dwModelPrecache);
}

uintptr_t client_state::get_player_info() const
{
	return memory::read<uintptr_t>(base + offsets::dwClientState_PlayerInfo);
}

vector2 client_state::get_view_angles() const
{
	return memory::read<vector2>(base + offsets::dwClientState_ViewAngles);
}

bool client_state::set_view_angles(const vector2 view_angles) const
{
	return memory::write(base + offsets::dwClientState_ViewAngles, view_angles);
}

bool client_state::set_delta_ticks(const int delta_ticks) const
{
	return memory::write(base + offsets::clientstate_delta_ticks, delta_ticks);
}