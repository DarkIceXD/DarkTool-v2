#include "overlay.hpp"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/implot.h"
#include <d3d9.h>
#pragma comment( lib, "d3d9.lib" )
#include <dwmapi.h>
#pragma comment( lib, "dwmapi.lib" )

constexpr auto title = L"Hammer & Chisel Inc.";
constexpr auto allow_screen_capture = false;

struct window_rect : public RECT
{
	constexpr int width() const { return right - left; }
	constexpr int height() const { return bottom - top; }
};

namespace directx9 {
	inline IDirect3D9Ex* direct3d9ex = NULL;
	inline IDirect3DDevice9* device = NULL;
	inline D3DPRESENT_PARAMETERS params = {};
	void reset()
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		device->Reset(&params);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam))
		return true;

	switch (Message) {
	case WM_DESTROY:
		if (directx9::device)
			directx9::device->Release();
		if (directx9::direct3d9ex)
			directx9::direct3d9ex->Release();
		PostQuitMessage(0);
		exit(4);
		break;
	case WM_SIZE:
		if (directx9::device != NULL && wParam != SIZE_MINIMIZED) {
			directx9::params.BackBufferWidth = LOWORD(lParam);
			directx9::params.BackBufferHeight = HIWORD(lParam);
			directx9::reset();
		}
		break;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}

static void mouse_handler(WPARAM wParam)
{
	auto& io = ImGui::GetIO();
	switch (wParam)
	{
	case WM_LBUTTONDOWN:
		io.MouseDown[0] = true;
		break;
	case WM_RBUTTONDOWN:
		io.MouseDown[1] = true;
		break;
	case WM_MBUTTONDOWN:
		io.MouseDown[2] = true;
		break;
	case WM_LBUTTONUP:
		io.MouseDown[0] = false;
		break;
	case WM_RBUTTONUP:
		io.MouseDown[1] = false;
		break;
	case WM_MBUTTONUP:
		io.MouseDown[2] = false;
		break;
	}
}

static void keyboard_handler(WPARAM wParam, KBDLLHOOKSTRUCT* info)
{
	
	auto& io = ImGui::GetIO();
	switch (wParam) {
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (info->vkCode < 512)
			io.KeysDown[info->vkCode] = true;
		switch (info->vkCode)
		{
		case VK_CONTROL:
		case VK_LCONTROL:
		case VK_RCONTROL:
			io.KeyCtrl = true;
			break;
		case VK_MENU:
			io.KeyAlt = true;
			break;
		case VK_SHIFT:
		case VK_LSHIFT:
		case VK_RSHIFT:
			io.KeyShift = true;
			break;
		}
		GetKeyState(info->vkCode);
		BYTE keys[256];
		if (GetKeyboardState(keys)) {
			constexpr auto size = 10;
			WCHAR keyPressed[size];
			if (ToUnicodeEx(info->vkCode, info->scanCode, (BYTE*)keys, keyPressed, size, 2, GetKeyboardLayout(NULL)))
			{
				for (int i = 0; i < size; i++)
				{
					if (keyPressed[i] > 0 && keyPressed[i] < 0x10000)
						io.AddInputCharacterUTF16((unsigned short)keyPressed[i]);
				}
			}
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		if (info->vkCode < 512)
			io.KeysDown[info->vkCode] = false;
		switch (info->vkCode)
		{
		case VK_CONTROL:
		case VK_LCONTROL:
		case VK_RCONTROL:
			io.KeyCtrl = false;
			break;
		case VK_MENU:
			io.KeyAlt = false;
			break;
		case VK_SHIFT:
		case VK_LSHIFT:
		case VK_RSHIFT:
			io.KeyShift = false;
			break;
		}
		break;
	}
}

LRESULT CALLBACK mouse_hook(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION)
		mouse_handler(wParam);
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK keyboard_hook(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION)
		keyboard_handler(wParam, reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam));
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

BOOL CALLBACK EnumWindowsProcMy(HWND hwnd, LPARAM lParam)
{
	DWORD lpdwProcessId;
	GetWindowThreadProcessId(hwnd, &lpdwProcessId);
	if (lpdwProcessId == lParam)
	{
		target::hwnd = hwnd;
		return FALSE;
	}
	return TRUE;
}

bool overlay::create_overlay(const uint32_t pid)
{
	WNDCLASSEX window_class = {
		sizeof(WNDCLASSEX), 0, WinProc, 0, 0, nullptr, LoadIcon(nullptr, IDI_APPLICATION), LoadCursor(nullptr, IDC_ARROW), nullptr, nullptr, title, LoadIcon(nullptr, IDI_APPLICATION)
	};
	RegisterClassEx(&window_class);
	overlay_window::hInstance = window_class.hInstance;
	do {
		EnumWindows(EnumWindowsProcMy, pid);
		Sleep(1000);
	} while (!target::hwnd);

	window_rect rect;
	if (!GetWindowRect(target::hwnd, &rect))
	{
		UnregisterClass(title, overlay_window::hInstance);
		return false;
	}
	overlay_window::hwnd = CreateWindowEx(NULL, title, title, WS_POPUP | WS_VISIBLE, rect.left, rect.top, rect.width(), rect.height(), nullptr, nullptr, nullptr, nullptr);
	SetWindowLong(overlay_window::hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE);
	MARGINS margin = { -1 };
	if (!SUCCEEDED(DwmExtendFrameIntoClientArea(overlay_window::hwnd, &margin)))
	{
		DestroyWindow(overlay_window::hwnd);
		UnregisterClass(title, overlay_window::hInstance);
		return false;
	}

	if (!allow_screen_capture)
		if (!SetWindowDisplayAffinity(overlay_window::hwnd, WDA_EXCLUDEFROMCAPTURE))
		{
			DestroyWindow(overlay_window::hwnd);
			UnregisterClass(title, overlay_window::hInstance);
			return false;
		}

	if (!ShowWindow(overlay_window::hwnd, SW_SHOW))
	{
		DestroyWindow(overlay_window::hwnd);
		UnregisterClass(title, overlay_window::hInstance);
		return false;
	}

	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &directx9::direct3d9ex)))
	{
		DestroyWindow(overlay_window::hwnd);
		UnregisterClass(title, overlay_window::hInstance);
		return false;
	}

	directx9::params.Windowed = TRUE;
	directx9::params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	directx9::params.hDeviceWindow = overlay_window::hwnd;
	directx9::params.BackBufferFormat = D3DFMT_A8R8G8B8;
	directx9::params.BackBufferWidth = rect.width();
	directx9::params.BackBufferHeight = rect.height();
	directx9::params.hDeviceWindow = overlay_window::hwnd;
	directx9::params.EnableAutoDepthStencil = TRUE;
	directx9::params.AutoDepthStencilFormat = D3DFMT_D16;
	directx9::params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	if (FAILED(directx9::direct3d9ex->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, overlay_window::hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &directx9::params, &directx9::device))) {
		directx9::direct3d9ex->Release();
		DestroyWindow(overlay_window::hwnd);
		UnregisterClass(title, overlay_window::hInstance);
		return false;
	}

	ImGui::CreateContext();
	ImPlot::CreateContext();
	if (!ImGui_ImplWin32_Init(overlay_window::hwnd))
	{
		ImGui::DestroyContext();
		ImPlot::DestroyContext();
		directx9::device->EndScene();
		directx9::device->Release();
		directx9::direct3d9ex->Release();
		DestroyWindow(overlay_window::hwnd);
		UnregisterClass(title, overlay_window::hInstance);
		return false;
	}
	if (!ImGui_ImplDX9_Init(directx9::device))
	{
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
		ImPlot::DestroyContext();
		directx9::device->EndScene();
		directx9::device->Release();
		directx9::direct3d9ex->Release();
		DestroyWindow(overlay_window::hwnd);
		UnregisterClass(title, overlay_window::hInstance);
		return false;
	}
	directx9::direct3d9ex->Release();

	{ // Load ImGui theme
		auto& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.63f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.67f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.67f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.18f, 0.18f, 0.18f, 0.39f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.61f, 0.00f, 1.00f, 0.24f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.61f, 0.00f, 1.00f, 0.39f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.61f, 0.00f, 1.00f, 0.39f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.61f, 0.00f, 1.00f, 0.24f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.61f, 0.00f, 1.00f, 0.73f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.61f, 0.00f, 1.00f, 0.47f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.61f, 0.00f, 1.00f, 0.75f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.61f, 0.00f, 1.00f, 0.78f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.61f, 0.00f, 1.00f, 0.47f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.61f, 0.00f, 1.00f, 0.47f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.61f, 0.00f, 1.00f, 0.24f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.61f, 0.00f, 1.00f, 0.39f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.61f, 0.00f, 1.00f, 0.39f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.61f, 0.00f, 1.00f, 0.24f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.61f, 0.00f, 1.00f, 0.24f);
		style.GrabRounding = style.FrameRounding = 6;
		style.ItemSpacing = ImVec2(8, 6);
	}
	return true;
}

void overlay::render(data::game& data)
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	draw(data, ImGui::GetBackgroundDrawList());

	static auto show_menu = false;
	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		static HHOOK mouse, keyboard;
		show_menu = !show_menu;
		if (show_menu)
		{
			mouse = SetWindowsHookEx(WH_MOUSE, mouse_hook, 0, GetCurrentThreadId());
			keyboard = SetWindowsHookEx(WH_KEYBOARD_LL, keyboard_hook, 0, 0);
		}
		else
		{
			UnhookWindowsHookEx(mouse);
			UnhookWindowsHookEx(keyboard);
		}
		const auto style = show_menu ? WS_EX_TRANSPARENT | WS_EX_NOACTIVATE : WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE;
		SetWindowLong(overlay_window::hwnd, GWL_EXSTYLE, style);
	}
	if (show_menu)
		menu();

	ImGui::EndFrame();

	directx9::device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	if (directx9::device->BeginScene() >= 0) {
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		directx9::device->EndScene();
	}

	const auto result = directx9::device->Present(NULL, NULL, NULL, NULL);
	if (result == D3DERR_DEVICELOST && directx9::device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		directx9::reset();
}

void overlay::destroy()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	if (directx9::device)
		directx9::device->Release();
	if (directx9::direct3d9ex)
		directx9::direct3d9ex->Release();
	DestroyWindow(overlay_window::hwnd);
	UnregisterClass(title, overlay_window::hInstance);
}