#include "License.h"
#include "utils.h"
#include "SimpleIni.h"
#include "xorstr.h"
#include <ctime>

uint32_t license::generate_hwid()
{
	constexpr auto size = 50;
	char hwid[size];
	GetVolumeNameForVolumeMountPointA(xorstr("C:\\"), hwid, size);
	hwid[size - 1] = 0;
	return util::hash(hwid);
}

license::user license::check()
{
	const auto tool = xorstr("darktool");
	const auto server = xorstr("darkicexd.github.io");
	const auto object = xorstr("/darktool/keys.ini");
	user user = {};
	user.hwid = license::generate_hwid();
	CSimpleIniA ini;
	if (util::hash(server) == 729258906494008491U && util::hash(object) == 3076906676292336621U)
	{
		const auto data = util::download(server, object);
		ini.LoadData(data.data(), data.size());
	}
	const auto section = std::to_string(user.hwid);
	if (!ini.GetBoolValue(section.c_str(), tool))
		return user;
	user.uid = ini.GetLongValue(section.c_str(), xorstr("uid"));
	const time_t expire_time = std::stoll(ini.GetValue(section.c_str(), xorstr("time"), xorstr("0")));
	const auto time = std::time(nullptr);
	if (expire_time == 0)
	{
		user.status = status::VALID;
	}
	else if (expire_time > time)
	{
		user.status = status::VALID;
		user.time_left = expire_time - time;
	}
	else
	{
		user.status = status::EXPIRED;
	}
	return user;
}

const char* license::status_to_str(const status s)
{
	switch (s)
	{
	case status::EXPIRED:
		return "Expired";
	case status::VALID:
		return "Valid";
	default:
		return "Invalid";
	}
}
