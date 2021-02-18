#pragma once
#include "../csgo-sdk/classes/i_base_client_dll.hpp"
#include "../csgo-sdk/classes/i_client_entity_list.hpp"
#include "../csgo-sdk/classes/iv_engine_client.hpp"
#include "../csgo-sdk/classes/i_client_mode.hpp"
#include "../csgo-sdk/classes/i_client_state.hpp"
#include "../csgo-sdk/classes/i_panel.hpp"
#include "../csgo-sdk/classes/i_surface.hpp"
#include "../csgo-sdk/classes/c_global_vars_base.hpp"
#include "../csgo-sdk/classes/i_material_system.hpp"
#include "../csgo-sdk/classes/iv_model_info.hpp"
#include "../csgo-sdk/classes/iv_model_render.hpp"
#include "../csgo-sdk/classes/iv_debug_overlay.hpp"
#include "../csgo-sdk/classes/i_console.hpp"
#include "../csgo-sdk/classes/i_localize.hpp"
#include "../csgo-sdk/classes/i_game_event_manager.hpp"
#include "../csgo-sdk/classes/i_input.hpp"
#include "../csgo-sdk/classes/iv_effects.hpp"
#include "../csgo-sdk/classes/i_input_system.hpp"
#include "../csgo-sdk/classes/engine_sounds.hpp"
#include "../csgo-sdk/classes/i_trace.hpp"
#include "../csgo-sdk/classes/i_render_view.hpp"
#include "../csgo-sdk/classes/glow_manager.hpp"
#include "../csgo-sdk/classes/i_player_movement.hpp"
#include "../csgo-sdk/classes/iv_render_beams.hpp"
#include "../csgo-sdk/classes/i_weapon_system_dumped.hpp"

#include "../steam-sdk/steam_api.h"

namespace interfaces {

	extern i_base_client_dll* client;
	extern i_input* input;
	extern i_client_entity_list* entity_list;
	extern iv_engine_client* engine;
	extern i_client_mode* clientmode;
	extern i_client_state* clientstate;
	extern i_panel* panel;
	extern i_surface* surface;
	extern c_global_vars_base* globals;
	extern i_material_system* material_system;
	extern iv_model_info* model_info;
	extern iv_model_render* model_render;
	extern i_render_view* render_view;
	extern iv_effects* effects;
	extern iv_debug_overlay* debug_overlay;
	extern i_console* console;
	extern i_localize* localize;
	extern i_game_event_manager2* event_manager;
	extern i_inputsytem* inputsystem;
	extern IDirect3DDevice9* directx;
	extern iv_engine_sound* engine_sound;
	extern trace* trace_ray;
	extern glow_manager_t* glow_manager;
	extern player_game_movement* game_movement;
	extern player_prediction* prediction;
	extern player_move_helper* move_helper;
	extern i_view_render_beams* render_beams;
	extern i_weapon_system* weapon_system;
	extern ISteamUser* steam_user;

	void initialize();
}