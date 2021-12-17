#pragma once
#include <cstdint>
#include <string_view>
#include <array>

struct weapon {
	weapon(const uintptr_t base);
	weapon();
	[[nodiscard]] short get_definition_index() const;
	bool set_definition_index(const short index) const;
	[[nodiscard]] int get_clip() const;
	[[nodiscard]] float get_next_primary_attack() const;
	[[nodiscard]] float get_inaccuracy() const;
	[[nodiscard]] int get_xuid_low() const;
	bool set_model_index(const unsigned int index) const;
	[[nodiscard]] int get_item_id_high() const;
	bool set_item_id_high(const int item_id) const;
	bool set_account_id(const int account_id) const;
	bool set_entity_quality(const int quality) const;
	bool set_name(const std::string_view name) const;
	bool set_paint_kit(const int paint_kit) const;
	bool set_seed(const int seed) const;
	bool set_wear(const float wear) const;
	bool set_stattrak(const int stattrak) const;
	uintptr_t entity;
};
