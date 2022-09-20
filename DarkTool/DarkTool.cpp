#include "memory/memory.h"
#include "overlay/overlay.hpp"
#include "config/config.h"
#include "utilities/license.h"
#include <iostream>
#include <thread>
#include <mutex>
#include "game/offsets.h"

static std::mutex mtx;
static data::game game_data;

static void overlay_execute()
{
	if (!overlay::create_overlay(memory::pid()))
	{
		std::cout << "Cannot create overlay.\n";
		return;
	}
	MSG message;
	do {
		if (PeekMessage(&message, overlay_window::hwnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		const auto forground_window = GetForegroundWindow();
		if (forground_window == target::hwnd) {
			const auto previous_window = GetWindow(forground_window, GW_HWNDPREV);
			SetWindowPos(overlay_window::hwnd, previous_window, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
		{
			std::lock_guard lock(mtx);
			overlay::render(game_data);
		}
		if (GetAsyncKeyState(VK_END) & 1)
			break;
	} while (message.message != WM_QUIT);
	overlay::destroy();
}

static void collect_data()
{
	while (true)
	{
		if (!memory::still_active())
		{
			memory::detach();
			while (!memory::initialize(L"csgo.exe"))
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		{
			std::lock_guard lock(mtx);
			data::update(game_data);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(40));
	}
}

static void fast_loop()
{
	while (true)
	{
		{
			std::lock_guard lock(mtx);
			data::fast_loop(game_data);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

int main()
{
	std::cout << "Checking license... ";
	const auto user = license::check(user_db::user::DARKTOOL);
	std::cout << license::status_to_str(user.status) << '\n';
	if (user.status != license::status::VALID)
	{
		std::cout << "HWID: " << std::hex << user.my_hwid << '\n';
		std::getchar();
		return 0;
	}
	std::cout << "Waiting for csgo... ";
	while (!memory::initialize(L"csgo.exe"))
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	std::cout << "Done\n";
	offsets::initialize();
	std::cout
		<< "Close this to disable DarkTool Overlay\n"
		<< "Press INS to open Menu\n"
		<< "Press END to close (panic button)\n";
	cfg = new config();
	std::thread worker(collect_data);
	worker.detach();
	std::thread fast_worker(fast_loop);
	fast_worker.detach();
	overlay_execute();
	cfg->save();
	delete cfg;
}