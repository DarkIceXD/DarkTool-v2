#include "netvar.h"
#include "../memory/memory.h"

constexpr auto nCRecvPropSize = 0x3C;
// CRecvProp struct offsets
constexpr auto m_pVarName = 0x0;
constexpr auto m_pDataTable = 0x28;
constexpr auto m_iOffset = 0x2C;
// CRecvTable struct offsets
constexpr auto m_pProps = 0x0;
constexpr auto m_nProps = 0x4;
constexpr auto m_pNetTableName = 0xC;
// CClientClass struct offsets
constexpr auto m_pRecvTable = 0xC;
constexpr auto m_pNext = 0x10;

uintptr_t scan_table(const uintptr_t dwTableAddr, const char* prop, const uintptr_t dwLevel) noexcept
{
	const auto count = memory::read<int>(dwTableAddr + m_nProps);
	for (auto i = 0; i < count; i++)
	{
		const auto dwPropAddr = memory::read<uintptr_t>(dwTableAddr + m_pProps) + nCRecvPropSize * i;
		if (!dwPropAddr)
			continue;

		char szPropName[128] = { 0 };
		const auto dwNameAddr = memory::read<uintptr_t>(dwPropAddr + m_pVarName);
		if (!memory::read(dwNameAddr, szPropName, sizeof(szPropName)) || isdigit(szPropName[0]))
			continue;

		const auto iOffset = memory::read<int>(dwPropAddr + m_iOffset);
		if (strcmp(szPropName, prop) == 0)
			return dwLevel + iOffset;

		const auto dwTableAddr = memory::read<uintptr_t>(dwPropAddr + m_pDataTable);
		if (!dwTableAddr)
			continue;

		const auto dwResult = scan_table(dwTableAddr, prop, dwLevel + iOffset);
		if (dwResult)
			return dwResult;
	}
	return 0;
}

uintptr_t netvar::find(const uintptr_t dwGetAllClasses, const char* table, const char* prop) noexcept
{
	for (auto dwClass = dwGetAllClasses; dwClass; dwClass = memory::read<uintptr_t>(dwClass + m_pNext))
	{
		const auto dwTableAddr = memory::read<uintptr_t>(dwClass + m_pRecvTable);

		char szTableName[128] = { 0 };
		const auto dwNameAddr = memory::read<uintptr_t>(dwTableAddr + m_pNetTableName);
		if (!memory::read(dwNameAddr, szTableName, 128))
			continue;

		if (strcmp(szTableName, table) == 0)
			return scan_table(dwTableAddr, prop, 0);
	}
	return 0;
}