#include "config.h"
#include <fstream>

config::config()
{
	std::ifstream stream(conf_name);
	if (stream.good())
	{
		const auto json = nlohmann::json::parse(stream, nullptr, false);
		if (!json.is_discarded())
		{
			esp = json.value<struct esp>("esp", {});
			aimbot = json.value<struct aimbot>("aimbot", {});
			trigger = json.value<struct trigger>("trigger", {});
			skin_changer = json.value<struct skin_changer>("skin_changer", {});
			clan_tag_changer = json.value<struct clan_tag_changer>("clan_tag_changer", {});
			misc = json.value<struct misc>("misc", {});
		}
	}
}

void config::save() const
{
	nlohmann::json json(*this);
	std::ofstream stream(conf_name);
	stream << json;
}