#pragma once
#include "../utilities/json.hpp"
#include <Windows.h>
#include "../game/structs.h"

struct rgb {
	float r{ 1 }, g{ 1 }, b{ 1 }, a{ 1 };
	bool enabled{ false };
	constexpr static rgb scale(const rgb& base, const rgb& end, const float c) noexcept
	{
		constexpr auto scale = [](const float max, const float min, const float scale) { return min + (max - min) * scale; };
		return {
			scale(base.r, end.r, c),
			scale(base.g, end.g, c),
			scale(base.b, end.b, c),
			scale(base.a, end.a, c),
			true
		};
	}
	constexpr uint32_t to_u32() const noexcept
	{
		return ((uint32_t)(r * 255) << 0) | ((uint32_t)(g * 255) << 8) | ((uint32_t)(b * 255) << 16) | ((uint32_t)(a * 255) << 24);
	}
	constexpr void rainbow(const float step) noexcept
	{
		if (r > 0 && b == 0)
		{
			r = (std::max)(r - step, 0.f);
			g = (std::min)(g + step, 1.f);
		}
		else if (g > 0 && r == 0)
		{
			g = (std::max)(g - step, 0.f);
			b = (std::min)(b + step, 1.f);
		}
		else if (b > 0 && g == 0)
		{
			r = (std::min)(r + step, 1.f);
			b = (std::max)(b - step, 0.f);
		}
		else
		{
			r = 1;
			g = 0;
			b = 0;
		}
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(rgb, r, g, b, a, enabled)
};

struct keybind {
	bool enabled{ false };
	int type{ 0 };
	int key_bind{ 0 };
	constexpr void run()
	{
		if (type == 1 && GetAsyncKeyState(key_bind) & 1)
			enabled = !enabled;
		else if (type == 2)
			enabled = GetAsyncKeyState(key_bind);
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(keybind, enabled, type, key_bind)
};

struct config {
	static constexpr const char* weapon_list_all = "Knife\0CZ75-Auto\0Desert Eagle\0Dual Berettas\0Five-SeveN\0Glock-18\0P2000\0P250\0R8 Revolver\0Tec-9\0USP-S\0AK-47\0AUG\0AWP\0FAMAS\0G3SG1\0Galil AR\0M4A1-S\0M4A4\0SCAR-20\0SG 553\0SSG 08\0MAC-10\0MP5-SD\0MP7\0MP9\0PP-Bizon\0P90\0UMP-45\0Nova\0MAG-7\0Sawed-Off\0XM1014\0M249\0Negev\0";
	static constexpr const char* weapon_list = weapon_list_all + 6;
	static constexpr int get_weapon_index(const int weapon) noexcept
	{
		switch (weapon)
		{
		case WEAPON_KNIFE:
		case WEAPON_KNIFE_T:
		case WEAPON_KNIFE_BAYONET:
		case WEAPON_KNIFE_CSS:
		case WEAPON_KNIFE_FLIP:
		case WEAPON_KNIFE_GUT:
		case WEAPON_KNIFE_KARAMBIT:
		case WEAPON_KNIFE_M9_BAYONET:
		case WEAPON_KNIFE_TACTICAL:
		case WEAPON_KNIFE_FALCHION:
		case WEAPON_KNIFE_SURVIVAL_BOWIE:
		case WEAPON_KNIFE_BUTTERFLY:
		case WEAPON_KNIFE_PUSH:
		case WEAPON_KNIFE_CORD:
		case WEAPON_KNIFE_CANIS:
		case WEAPON_KNIFE_URSUS:
		case WEAPON_KNIFE_GYPSY_JACKKNIFE:
		case WEAPON_KNIFE_OUTDOOR:
		case WEAPON_KNIFE_STILETTO:
		case WEAPON_KNIFE_WIDOWMAKER:
		case WEAPON_KNIFE_SKELETON:
			return 0;
		case WEAPON_CZ75A: return 1;
		case WEAPON_DEAGLE: return 2;
		case WEAPON_ELITE: return 3;
		case WEAPON_FIVESEVEN: return 4;
		case WEAPON_GLOCK: return 5;
		case WEAPON_HKP2000: return 6;
		case WEAPON_P250: return 7;
		case WEAPON_REVOLVER: return 8;
		case WEAPON_TEC9: return 9;
		case WEAPON_USP_SILENCER: return 10;
		case WEAPON_AK47: return 11;
		case WEAPON_AUG: return 12;
		case WEAPON_AWP: return 13;
		case WEAPON_FAMAS: return 14;
		case WEAPON_G3SG1: return 15;
		case WEAPON_GALILAR: return 16;
		case WEAPON_M4A1_SILENCER: return 17;
		case WEAPON_M4A1: return 18;
		case WEAPON_SCAR20: return 19;
		case WEAPON_SG556: return 20;
		case WEAPON_SSG08: return 21;
		case WEAPON_MAC10: return 22;
		case WEAPON_MP5_SD: return 23;
		case WEAPON_MP7: return 24;
		case WEAPON_MP9: return 25;
		case WEAPON_BIZON: return 26;
		case WEAPON_P90: return 27;
		case WEAPON_UMP45: return 28;
		case WEAPON_NOVA: return 29;
		case WEAPON_MAG7: return 30;
		case WEAPON_SAWEDOFF: return 31;
		case WEAPON_XM1014: return 32;
		case WEAPON_M249: return 33;
		case WEAPON_NEGEV: return 34;
		default: return -1;
		}
	}
	static constexpr const char* conf_name = "DarkTool.json";
	config();
	void save() const;
	struct esp {
		keybind bind{};
		struct color {
			rgb base, visible;
			NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(color, base, visible)
		}
		box{ { 1, 0, 0, 1 }, { 0, 1, 0, 1 } }, skeleton{ { 1, 0, 0, 0.5f }, { 0, 1, 0, 0.5f } }, glow{ { 1, 0, 0, 1 }, { 0, 1, 0, 1 } };
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(esp, bind, box, skeleton, glow)
	} esp;
	struct aimbot {
		keybind bind{};
		float fov{ 5.f };
		float smoothness{ 1.f };
		bool head{ true };
		bool chest{ false };
		bool stomach{ true };
		bool visibility_check{ true };
		bool show_aim_spot{ true };
		constexpr bool is_hitbox_enabled(const hitbox hitbox) const
		{
			switch (hitbox)
			{
			case hitbox::HEAD:
			case hitbox::NECK:
				return head;
			case hitbox::LOWER_CHEST:
			case hitbox::UPPER_CHEST:
				return chest;
			case hitbox::THORAX:
			case hitbox::PELVIS:
			case hitbox::BELLY:
				return stomach;
			default:
				return false;
			}
		}
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(aimbot, bind, fov, smoothness, head, chest, stomach, visibility_check, show_aim_spot)
	} aimbot;
	struct trigger {
		keybind bind{};
		int weapon_selection{ 0 };
		std::array<int, 34> hitchance{ {20, 75, 55, 60, 40, 40, 50, 50, 40, 55, 70, 70, 90, 60, 85, 60, 60, 60, 85, 1, 80, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1} };
		constexpr int& get_selection()
		{
			return hitchance[weapon_selection];
		}
		constexpr int get_hitchance(const int weapon) noexcept
		{
			const auto i = get_weapon_index(weapon) - 2;
			if (i < 0)
				return 0;
			return hitchance[i];
		}
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(trigger, bind, weapon_selection, hitchance)
	} trigger;
	struct skin_changer
	{
		bool enabled{ true };
		bool update_now{ false };
		bool auto_save{ true };
		int selection{ 0 };
		struct skin {
			skin()
			{
				nametag.reserve(20);
			}
			int paint_kit{ 0 }, stat_trak{ 0 }, seed{ 0 }, quality{ 0 };
			float wear{ FLT_MIN };
			std::string nametag;
			bool count_kills{ true };
			NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(skin, paint_kit, stat_trak, seed, quality, wear, nametag, count_kills)
		};
		std::array<skin, 35> skins;
		int knife_selection{ 0 };
		constexpr skin* get_skin(const int weapon) noexcept
		{
			const auto i = get_weapon_index(weapon);
			if (i < 0)
				return nullptr;
			return &skins.at(i);
		}
		constexpr skin& get_selected() noexcept
		{
			return skins.at(selection);
		}
		constexpr short get_knife_index() const noexcept
		{
			switch (knife_selection)
			{
			case 1: return WEAPON_KNIFE_BAYONET;
			case 2: return WEAPON_KNIFE_CSS;
			case 3: return WEAPON_KNIFE_FLIP;
			case 4: return WEAPON_KNIFE_GUT;
			case 5: return WEAPON_KNIFE_KARAMBIT;
			case 6: return WEAPON_KNIFE_M9_BAYONET;
			case 7: return WEAPON_KNIFE_TACTICAL;
			case 8: return WEAPON_KNIFE_FALCHION;
			case 9: return WEAPON_KNIFE_SURVIVAL_BOWIE;
			case 10: return WEAPON_KNIFE_BUTTERFLY;
			case 11: return WEAPON_KNIFE_PUSH;
			case 12: return WEAPON_KNIFE_CORD;
			case 13: return WEAPON_KNIFE_CANIS;
			case 14: return WEAPON_KNIFE_URSUS;
			case 15: return WEAPON_KNIFE_GYPSY_JACKKNIFE;
			case 16: return WEAPON_KNIFE_OUTDOOR;
			case 17: return WEAPON_KNIFE_STILETTO;
			case 18: return WEAPON_KNIFE_WIDOWMAKER;
			case 19: return WEAPON_KNIFE_SKELETON;
			default: return 0;
			}
		}
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(skin_changer, enabled, auto_save, selection, skins, knife_selection)
	}skin_changer;
	struct clan_tag_changer {
		bool enabled{ false };
		size_t selected{ 0 };
		struct clan_tag {
			clan_tag()
			{
				name.reserve(15);
			}
			clan_tag(const char* name, const int delay, const std::vector<std::string>& tags) :
				name(name), delay(delay), tags(tags)
			{
				this->name.reserve(15);
			}
			std::string name{ "New Clan Tag" };
			int delay{ 400 };
			std::vector<std::string> tags{};
			NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(clan_tag, name, delay, tags)
		};
		std::vector<clan_tag> tags{ {"DarkToolX", 400, {".........","........D",".......D.","......D..",".....D...","....D....","...D.....","..D......",".D.......","D........","D.......a","D......a.","D.....a..","D....a...","D...a....","D..a.....","D.a......","Da.......","Da......r","Da.....r.","Da....r..","Da...r...","Da..r....","Da.r.....","Dar......","Dar.....k","Dar....k.","Dar...k..","Dar..k...","Dar.k....","Dark.....","Dark....T","Dark...T.","Dark..T..","Dark.T...","DarkT....","DarkT...o","DarkT..o.","DarkT.o..","DarkTo...","DarkTo..o","DarkTo.o.","DarkToo..","DarkToo.l","DarkTool.","DarkToolX","DarkTool.","DarkToo.l","DarkToo..","DarkTo.o.","DarkTo..o","DarkTo...","DarkT.o..","DarkT..o.","DarkT...o","DarkT....","Dark.T...","Dark..T..","Dark...T.","Dark....T","Dark.....","Dar.k....","Dar..k...","Dar...k..","Dar....k.","Dar.....k","Dar......","Da.r.....","Da..r....","Da...r...","Da....r..","Da.....r.","Da......r","Da.......","D.a......","D..a.....","D...a....","D....a...","D.....a..","D......a.","D.......a","D........",".D.......","..D......","...D.....","....D....",".....D...","......D..",".......D.","........D"}} };
		clan_tag& get_selected()
		{
			if (tags.size() == 0)
				tags.push_back({ "DarkToolX", 400, {".........","........D",".......D.","......D..",".....D...","....D....","...D.....","..D......",".D.......","D........","D.......a","D......a.","D.....a..","D....a...","D...a....","D..a.....","D.a......","Da.......","Da......r","Da.....r.","Da....r..","Da...r...","Da..r....","Da.r.....","Dar......","Dar.....k","Dar....k.","Dar...k..","Dar..k...","Dar.k....","Dark.....","Dark....T","Dark...T.","Dark..T..","Dark.T...","DarkT....","DarkT...o","DarkT..o.","DarkT.o..","DarkTo...","DarkTo..o","DarkTo.o.","DarkToo..","DarkToo.l","DarkTool.","DarkToolX","DarkTool.","DarkToo.l","DarkToo..","DarkTo.o.","DarkTo..o","DarkTo...","DarkT.o..","DarkT..o.","DarkT...o","DarkT....","Dark.T...","Dark..T..","Dark...T.","Dark....T","Dark.....","Dar.k....","Dar..k...","Dar...k..","Dar....k.","Dar.....k","Dar......","Da.r.....","Da..r....","Da...r...","Da....r..","Da.....r.","Da......r","Da.......","D.a......","D..a.....","D...a....","D....a...","D.....a..","D......a.","D.......a","D........",".D.......","..D......","...D.....","....D....",".....D...","......D..",".......D.","........D"} });
			selected = std::clamp(selected, 0U, tags.size() - 1);
			return tags.at(selected);
		}
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(clan_tag_changer, enabled, selected, tags)
	}clan_tag_changer;
	struct misc {
		bool bhop{ false };
		bool no_flash{ false };
		bool nade_helper{ false };
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(misc, bhop, no_flash, nade_helper)
	}misc;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(config, esp, aimbot, trigger, skin_changer, clan_tag_changer, misc)
};

inline config* cfg;