#include "Memory.h"
#include <tlhelp32.h>

HANDLE _handle;
uint32_t _pid;

bool memory::initialize(const std::wstring_view process)
{
	const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (snapshot == INVALID_HANDLE_VALUE)
		return false;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(entry);
	if (Process32First(snapshot, &entry))
	{
		do {
			if (process == entry.szExeFile)
			{
				CloseHandle(snapshot);
				_pid = entry.th32ProcessID;
				_handle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_QUERY_LIMITED_INFORMATION, false, _pid);
				return true;
			}
		} while (Process32Next(snapshot, &entry));
	}
	CloseHandle(snapshot);
	return false;
}

uint32_t memory::pid()
{
	return _pid;
}

bool memory::detach()
{
	return CloseHandle(_handle);
}

bool memory::still_active()
{
	DWORD exitCodeOut;
	if (GetExitCodeProcess(_handle, &exitCodeOut) == 0)
		return false;
	return exitCodeOut == STILL_ACTIVE;
}

bool memory::read(const uintptr_t address, void* buffer, const int size)
{
	return ReadProcessMemory(_handle, (LPVOID)address, buffer, size, nullptr);
}

bool memory::write(const uintptr_t address, void* buffer, const int size)
{
	return WriteProcessMemory(_handle, (LPVOID)address, buffer, size, nullptr);
}

LPVOID memory::alloc(const SIZE_T size)
{
	return VirtualAllocEx(_handle, 0, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
}

void memory::free(const LPVOID mem)
{
	VirtualFreeEx(_handle, mem, 0, MEM_RELEASE);
}

void memory::run_remote_thread(const LPTHREAD_START_ROUTINE fn, const LPVOID param)
{
	const auto thread = CreateRemoteThread(_handle, nullptr, NULL, fn, param, NULL, nullptr);
	if (thread)
	{
		WaitForSingleObject(thread, INFINITE);
		CloseHandle(thread);
	}
}

memory::module memory::get_module(const std::wstring_view module_name)
{
	const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, _pid);
	if (snapshot == INVALID_HANDLE_VALUE)
		return { 0, 0 };
	MODULEENTRY32 entry;
	entry.dwSize = sizeof(entry);
	if (Module32First(snapshot, &entry)) {
		do {
			if (module_name == entry.szModule)
			{
				CloseHandle(snapshot);
				return { (uintptr_t)entry.modBaseAddr, entry.modBaseSize };
			}
		} while (Module32Next(snapshot, &entry));
	}
	CloseHandle(snapshot);
	return { 0, 0 };
}

uintptr_t find_pattern(const memory::module mod, const char* pattern, const char* mask)
{
	auto buf = new char[mod.size];
	if (memory::read(mod.base, buf, mod.size))
	{
		const auto size = mod.size - strlen(pattern);
		for (auto i = 0; i < size; i++)
		{
			for (auto j = 0;; j++)
			{
				if (mask[j] == 'x' && buf[i + j] != pattern[j])
				{
					break;
				}
				else if (mask[j] == 0)
				{
					delete[] buf;
					return mod.base + i;
				}
			}
		}
	}
	delete[] buf;
	return 0;
}

uintptr_t memory::get_offset(const memory::module mod, const char* pattern, const char* mask, const int offset)
{
	auto pattern_at = find_pattern(mod, pattern, mask);
	if (offset > -1)
		pattern_at = read<uintptr_t>(pattern_at + offset);
	return pattern_at;
}