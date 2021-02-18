#include "interfaces.hpp"
#include "../includes.hpp"

#include <windows.h>
#include <cstdint>
#include <string>
#include <stdexcept>

i_base_client_dll* interfaces::client = nullptr;
i_input* interfaces::input = nullptr;
i_client_entity_list* interfaces::entity_list = nullptr;
iv_engine_client* interfaces::engine = nullptr;
i_client_mode* interfaces::clientmode = nullptr;
i_client_state* interfaces::clientstate = nullptr;
i_panel* interfaces::panel = nullptr;
i_surface* interfaces::surface = nullptr;
c_global_vars_base* interfaces::globals = nullptr;
i_material_system* interfaces::material_system = nullptr;
iv_model_info* interfaces::model_info = nullptr;
iv_model_render* interfaces::model_render = nullptr;
i_render_view* interfaces::render_view = nullptr;
iv_effects* interfaces::effects = nullptr;
i_console* interfaces::console = nullptr;
i_localize* interfaces::localize = nullptr;
i_game_event_manager2* interfaces::event_manager = nullptr;
i_inputsytem* interfaces::inputsystem = nullptr;
iv_debug_overlay* interfaces::debug_overlay = nullptr;
IDirect3DDevice9* interfaces::directx = nullptr;
iv_engine_sound* interfaces::engine_sound = nullptr;
trace* interfaces::trace_ray = nullptr;
glow_manager_t* interfaces::glow_manager = nullptr;
player_game_movement* interfaces::game_movement = nullptr;
player_prediction* interfaces::prediction = nullptr;
player_move_helper* interfaces::move_helper = nullptr;
i_view_render_beams* interfaces::render_beams = nullptr;
i_weapon_system* interfaces::weapon_system = nullptr;
ISteamUser* interfaces::steam_user = nullptr;


enum class interface_type { index, bruteforce };

template <typename ret, interface_type type>
ret* get_interface(const std::string& module_name, const std::string& interface_name) {
	using create_interface_fn = void* (*)(const char*, int*);
	const auto fn = reinterpret_cast<create_interface_fn>(GetProcAddress(GetModuleHandle(module_name.c_str()), "CreateInterface"));

	if (fn) {
		void* result = nullptr;

		switch (type) {
		case interface_type::index:
			result = fn(interface_name.c_str(), nullptr);

			break;
		case interface_type::bruteforce:
			char buf[128];

			for (uint32_t i = 0; i <= 100; ++i) {
				memset(static_cast<void*>(buf), 0, sizeof buf);

				result = fn(interface_name.c_str(), nullptr);

				if (result)
					break;
			}

			break;
		}

		if (!result)
			throw std::runtime_error(interface_name + " wasn't found in " + module_name);

		return static_cast<ret*>(result);
	}

	throw std::runtime_error(module_name + " wasn't found");
}


static bool steamsdk = false;
void interfaces::initialize() {
	client = get_interface<i_base_client_dll, interface_type::index>("client.dll", "VClient018");
	entity_list = get_interface<i_client_entity_list, interface_type::index>("client.dll", "VClientEntityList003");
	engine = get_interface<iv_engine_client, interface_type::index>("engine.dll", "VEngineClient014");
	panel = get_interface<i_panel, interface_type::index>("vgui2.dll", "VGUI_Panel009");
	surface = get_interface<i_surface, interface_type::index>("vguimatsurface.dll", "VGUI_Surface031");
	material_system = get_interface<i_material_system, interface_type::index>("materialsystem.dll", "VMaterialSystem080");
	model_info = get_interface<iv_model_info, interface_type::index>("engine.dll", "VModelInfoClient004");
	model_render = get_interface<iv_model_render, interface_type::index>("engine.dll", "VEngineModel016");
	render_view = get_interface<i_render_view, interface_type::index>("engine.dll", "VEngineRenderView014");
	console = get_interface<i_console, interface_type::index>("vstdlib.dll", "VEngineCvar007");
	localize = get_interface<i_localize, interface_type::index>("localize.dll", "Localize_001");
	event_manager = get_interface<i_game_event_manager2, interface_type::index>("engine.dll", "GAMEEVENTSMANAGER002");
	debug_overlay = get_interface<iv_debug_overlay, interface_type::index>("engine.dll", "VDebugOverlay004");
	inputsystem = get_interface<i_inputsytem, interface_type::index>("inputsystem.dll", "InputSystemVersion001");
	trace_ray = get_interface<trace, interface_type::index>("engine.dll", "EngineTraceClient004");
	game_movement = get_interface<player_game_movement, interface_type::index>("client.dll", "GameMovement001");
	prediction = get_interface<player_prediction, interface_type::index>("client.dll", "VClientPrediction001");
	engine_sound = get_interface<iv_engine_sound, interface_type::index>("engine.dll", "IEngineSoundClient003");

	clientmode = **reinterpret_cast<i_client_mode***>((*reinterpret_cast<uintptr_t**>(client))[10] + 5);
	globals = **reinterpret_cast<c_global_vars_base***>((*reinterpret_cast<uintptr_t**>(client))[11] + 10);

	if (!steamsdk) {
		typedef uint32_t SteamPipeHandle;
		typedef uint32_t SteamUserHandle;
		SteamUserHandle hSteamUser = ((SteamUserHandle(__cdecl*)(void))GetProcAddress(GetModuleHandle("steam_api.dll"), "SteamAPI_GetHSteamUser"))();
		SteamPipeHandle hSteamPipe = ((SteamPipeHandle(__cdecl*)(void))GetProcAddress(GetModuleHandle("steam_api.dll"), "SteamAPI_GetHSteamPipe"))();
		auto SteamClient = ((ISteamClient * (__cdecl*)(void))GetProcAddress(GetModuleHandle("steam_api.dll"), "SteamClient"))();
		auto SteamHTTP = SteamClient->GetISteamHTTP(hSteamUser, hSteamPipe, "STEAMHTTP_INTERFACE_VERSION002");
		steam_user = SteamClient->GetISteamUser(hSteamUser, hSteamPipe, "SteamUser019");
		auto SteamFriends = SteamClient->GetISteamFriends(hSteamUser, hSteamPipe, "SteamFriends015");
		auto SteamInventory = SteamClient->GetISteamInventory(hSteamUser, hSteamPipe, "STEAMINVENTORY_INTERFACE_V002");

		steamsdk = true;
	}

	clientstate = **reinterpret_cast<i_client_state***>(utilities::pattern_scan("engine.dll", "A1 ? ? ? ? 8B 80 ? ? ? ? C3") + 1);
	directx = **reinterpret_cast<IDirect3DDevice9***>(utilities::pattern_scan("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);
	input = *reinterpret_cast<i_input**>(utilities::pattern_scan("client.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 1);
	glow_manager = reinterpret_cast<glow_manager_t*>(*reinterpret_cast<uintptr_t*>(utilities::pattern_scan("client.dll", "0F 11 05 ? ? ? ? 83 C8 01 C7 05 ? ? ? ? 00 00 00 00") + 3));
	move_helper = **reinterpret_cast<player_move_helper***>(utilities::pattern_scan("client.dll", "8B 0D ? ? ? ? 8B 46 08 68") + 2);
	weapon_system = *reinterpret_cast<i_weapon_system**>(utilities::pattern_scan("client.dll", "8B 35 ? ? ? ? FF 10 0F B7 C0") + 2);
}