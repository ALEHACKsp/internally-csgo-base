#pragma once
#include "../includes.hpp"
#include <algorithm>
#include <cmath>
#include <sstream>

uint8_t* present_address;
hooks::present_fn original_present;
uint8_t* reset_address;
hooks::reset_fn original_reset;
HWND hooks::window;
WNDPROC hooks::wndproc_original = NULL;

hooks::create_move_fn original_create_move_hook;
hooks::paint_traverse_fn original_panel;

void hooks::initialize() {
	const auto create_move_hook = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 24));
	const auto panel_hook = reinterpret_cast<void*>(get_virtual(interfaces::panel, 41));
	const auto surface_hook = reinterpret_cast<void*>(get_virtual(interfaces::surface, 67));
	const auto present_hook = reinterpret_cast<void*>(get_virtual(interfaces::directx, 17));
	const auto reset_hook = reinterpret_cast<void*>(get_virtual(interfaces::directx, 16));

	if (MH_Initialize() != MH_OK)
		throw std::runtime_error(XorStr("failed to initialize MH_Initialize."));

	if (MH_CreateHook(create_move_hook, &create_move, reinterpret_cast<void**>(&original_create_move_hook)) != MH_OK)
		throw std::runtime_error(XorStr("failed to initialize create_move_hook. (outdated index?)"));

	if (MH_CreateHook(panel_hook, &paint_traverse, reinterpret_cast<void**>(&original_panel)) != MH_OK)
		throw std::runtime_error(XorStr("failed to initialize paint_traverse_hook. (outdated index?)"));

	if (MH_CreateHook(present_hook, &present, reinterpret_cast<void**>(&original_present)) != MH_OK)
		throw std::runtime_error("failed to initialize paint_traverse_hook. (outdated index?)");

	if (MH_CreateHook(reset_hook, &reset, reinterpret_cast<void**>(&original_reset)) != MH_OK)
		throw std::runtime_error("failed to initialize scene_end_hook. (outdated index?)");

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		throw std::runtime_error(XorStr("failed to enable hooks."));

	window = FindWindow("Valve001", NULL);
	wndproc_original = (WNDPROC)SetWindowLongPtrA(window, GWL_WNDPROC, (LONG)wndproc);

	RENDER::get().setup_fonts();
}

void hooks::shutdown() {
	MH_Uninitialize();

	MH_DisableHook(MH_ALL_HOOKS);

	SetWindowLongPtrA(FindWindow("Valve001", NULL), GWL_WNDPROC, (LONG)wndproc_original);
}

void clampangles(c_usercmd* user_cmd)
{
	auto forward = user_cmd->forwardmove;
	auto right = user_cmd->sidemove;
	auto up = user_cmd->upmove;

	if (forward > 450)
		forward = 450;

	if (right > 450)
		right = 450;

	if (up > 450)
		up = 450;

	if (forward < -450)
		forward = -450;

	if (right < -450)
		right = -450;

	if (up < -450)
		up = -450;

	utilities::clamp_angles(user_cmd->viewangles);
}

bool __stdcall hooks::create_move(float frame_time, c_usercmd* user_cmd) {
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	original_create_move_hook(interfaces::clientmode, frame_time, user_cmd);

	if (!user_cmd || !user_cmd->command_number)
		return original_create_move_hook;

	if (!interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()))
		return original_create_move_hook;

	if (interfaces::engine->is_connected() && interfaces::engine->is_in_game()) {
		//bunny
		if ((user_cmd->buttons & in_jump) && !(local_player->flags() & fl_onground))
			user_cmd->buttons &= ~in_jump;

		clampangles(user_cmd);
	}

	return false;
}

void __stdcall hooks::paint_traverse(unsigned int panel, bool force_repaint, bool allow_force) {
	std::string panel_name = interfaces::panel->get_panel_name(panel);
	static unsigned int _panel = 0;

	PCHAR panel_char = (PCHAR)interfaces::panel->get_panel_name(panel);

	static bool once = false;
	if (!once) {
		PCHAR panel_char = (PCHAR)interfaces::panel->get_panel_name(panel);
		if (strstr(panel_char, "MatSystemTopPanel")) {
			_panel = panel;
			once = true;
		}
	}

	else if (_panel == panel) {
		RENDER::get().draw_text(3, 3, drawing::get().verdana, "internally-csgo-base", false, color::white());

	}

	original_panel(interfaces::panel, panel, force_repaint, allow_force);
}

LRESULT __stdcall hooks::wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
	return CallWindowProcA(wndproc_original, hwnd, message, wparam, lparam);
}

long __stdcall hooks::present(IDirect3DDevice9* device, RECT* source_rect, RECT* dest_rect, HWND dest_window_override, RGNDATA* dirty_region) {
	// directx

	return original_present(device, source_rect, dest_rect, dest_window_override, dirty_region);
}

long __stdcall hooks::reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters) {
	long hr = original_reset(device, present_parameters);
	return hr;
}