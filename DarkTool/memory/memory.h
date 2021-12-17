#pragma once
#define NOGDICAPMASKS
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NORASTEROPS
#define NOSHOWWINDOW
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOGDI
#define NOKERNEL
#define NOMB
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX    
#include <Windows.h>
#include <string_view>

namespace memory
{
	struct module
	{
		uintptr_t base;
		uintptr_t size;
	};
	[[nodiscard]] bool initialize(const std::wstring_view process);
	[[nodiscard]] uint32_t pid();
	bool detach();
	[[nodiscard]] bool still_active();
	bool read(const uintptr_t address, void* buffer, const int size);
	template <class T>
	[[nodiscard]] T read(const uintptr_t address)
	{
		T buf;
		memory::read(address, (void*)&buf, sizeof(T));
		return buf;
	}
	bool write(const uintptr_t address, void* buffer, const int size);
	template <class T>
	bool write(const uintptr_t address, const T& value)
	{
		return memory::write(address, (void*)&value, sizeof(T));
	}
	[[nodiscard]] LPVOID alloc(const SIZE_T size);
	void free(const LPVOID mem);
	void run_remote_thread(const LPTHREAD_START_ROUTINE fn, const LPVOID param = nullptr);
	[[nodiscard]] memory::module get_module(const std::wstring_view module_name);
	[[nodiscard]] uintptr_t get_offset(const module mod, const char* pattern, const char* mask, const int offset = -1);
};