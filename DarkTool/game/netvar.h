#pragma once
#include <cstdint>

namespace netvar {
	[[nodiscard]] uintptr_t find(const uintptr_t dwGetAllClasses, const char* table, const char* prop) noexcept;
}