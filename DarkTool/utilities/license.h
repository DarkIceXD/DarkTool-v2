#pragma once
#include <string>

namespace license
{
	enum class status
	{
		INVALID,
		EXPIRED,
		VALID
	};
	struct user {
		uint32_t hwid;
		uint32_t uid;
		status status;
		time_t time_left;
	};
	uint32_t generate_hwid();
	user check();
	const char* status_to_str(const status s);
}