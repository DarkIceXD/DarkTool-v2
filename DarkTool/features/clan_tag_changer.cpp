#include "features.h"
#include "../config/config.h"
#include "../memory/memory.h"
#include "../game/offsets.h"

static void set_clantag(const char* tag)
{
	constexpr auto MAX_TEXT_SIZE = 15;
	unsigned char Shellcode[] =
		"\xB9\x00\x00\x00\x00"
		"\xBA\x00\x00\x00\x00"
		"\xB8\x00\x00\x00\x00"
		"\xFF\xD0"
		"\xC3"
		"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";

	constexpr auto SHELLCODE_SIZE = sizeof(Shellcode);
	auto TAG_SIZE = strlen(tag);
	if (TAG_SIZE > MAX_TEXT_SIZE)
		TAG_SIZE = MAX_TEXT_SIZE;

	const auto address = memory::alloc(SHELLCODE_SIZE);
	if (!address)
		return;

	const auto dwTagAddress = (uintptr_t)address + 18;
	memcpy(Shellcode + 1, &dwTagAddress, sizeof(dwTagAddress));
	memcpy(Shellcode + 6, &dwTagAddress, sizeof(dwTagAddress));
	memcpy(Shellcode + 11, &offsets::dwSetClanTag, sizeof(offsets::dwSetClanTag));
	memcpy(Shellcode + 18, tag, TAG_SIZE);

	memory::write((uintptr_t)address, Shellcode, SHELLCODE_SIZE);
	memory::run_remote_thread((LPTHREAD_START_ROUTINE)address);
	memory::free(address);
}

void features::clan_tag_changer(const data::game& data)
{
	static auto clan_tag_changed = false;
	if (!cfg->clan_tag_changer.enabled)
	{
		if (clan_tag_changed)
		{
			set_clantag("");
			clan_tag_changed = false;
		}
		return;
	}
	const auto& tag = cfg->clan_tag_changer.get_selected();
	const auto size = tag.tags.size();
	static size_t index = 0;
	const auto temp_index = static_cast<size_t>(data.vars.currenttime * 1000 / tag.delay) % size;
	if (index != temp_index || (size == 1 && !(data.vars.tick_count % 300)))
	{
		index = temp_index;
		set_clantag(tag.tags[index].c_str());
		clan_tag_changed = true;
	}
}