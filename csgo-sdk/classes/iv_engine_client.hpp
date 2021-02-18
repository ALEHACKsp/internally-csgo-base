#pragma once
#include <cstdint>
#include "../vectors/vector3d.hpp"
#include "../vectors/vector2d.hpp"
#include "../vectors/view_matrix.hpp"
#include "../classes/net_channel.hpp"
#include "../../utilities/utilities.hpp"

struct player_info_t {
	int64_t __pad0;
	union {
		int64_t xuid;
		struct {
			int xuidlow;
			int xuidhigh;
		};
	};
	char name[128];
	int userid;
	char guid[33];
	unsigned int friendsid;
	char friendsname[128];
	bool fakeplayer;
	bool ishltv;
	unsigned int customfiles[4];
	unsigned char filesdownloaded;
};
class net_channel {
public:
	uint8_t pad_0x0000[0x17];
	bool m_bShouldDelete;
	int m_nOutSequenceNr;
	int m_nInSequenceNr;
	int m_nOutSequenceNrAck;
	int m_nOutReliableState;
	int m_nInReliableState;
	int m_nChokedPackets;
};
class iv_engine_client {
public:
	i_net_channel_info* get_net_channel_info() {
		using original_fn = i_net_channel_info * (__thiscall*)(iv_engine_client*);
		return (*(original_fn**)this)[78](this);
	}
	net_channel* get_net_channel() {
		using original_fn = net_channel * (__thiscall*)(iv_engine_client*);
		return (*(original_fn**)this)[78](this);
	}
	virtual int                   GetLocalPlayer(void) = 0;
	int get_local_player() {
		using original_fn = int(__thiscall*)(iv_engine_client*);
		return (*(original_fn**)this)[12](this);
	}
	int get_player_for_user_id(int user_id) {
		using original_fn = int(__thiscall*)(iv_engine_client*, int);
		return (*(original_fn**)this)[9](this, user_id);
	}
	unsigned int get_engline_build_number() //thx for carteridja
	{
		using original_fn = unsigned int(__thiscall*)(iv_engine_client*);
		return (*(original_fn**)this)[104](this);
	}
	unsigned int get_client_version(int build) { //thx for carteridja
		using original_fn = unsigned int(__thiscall*)(iv_engine_client*, unsigned int);
		return (*(original_fn**)this)[219](this, build);
	}
	void get_player_info(int index, player_info_t* info) {
		using original_fn = void(__thiscall*)(iv_engine_client*, int, player_info_t*);
		return (*(original_fn**)this)[8](this, index, info);
	}
	void get_screen_size(int& width, int& height) {
		using original_fn = void(__thiscall*)(iv_engine_client*, int&, int&);
		return (*(original_fn**)this)[5](this, width, height);
	}
	bool is_in_game() {
		using original_fn = bool(__thiscall*)(iv_engine_client*);
		return (*(original_fn**)this)[26](this);
	}
	bool is_connected() {
		using original_fn = bool(__thiscall*)(iv_engine_client*);
		return (*(original_fn**)this)[27](this);
	}
	void execute_cmd(const char* cmd) {
		using original_fn = void(__thiscall*)(iv_engine_client*, const char*);
		return (*(original_fn**)this)[108](this, cmd); // this always seems to crash whilst debugging, just feel free to continue.
	}
	void set_view_angles(vec3_t& angles) {
		using original_fn = void(__thiscall*)(iv_engine_client*, vec3_t&);
		return (*(original_fn**)this)[19](this, angles);
	}

	vec3_t get_view_angles() {
		vec3_t temp;
		using original_fn = void(__thiscall*)(iv_engine_client*, vec3_t&);
		(*(original_fn**)this)[18](this, temp);
		return temp;
	}

	void get_view_angles_alternative(vec3_t& angles) {
		return utilities::call_virtual<void(__thiscall*)(void*, vec3_t&)>(this, 18)(this, angles);
	}

	view_matrix_t& world_to_screen_matrix() {
		view_matrix_t temp;
		using original_fn = view_matrix_t & (__thiscall*)(iv_engine_client*);
		return (*(original_fn**)this)[37](this);
	}

	bool is_taking_screenshot() {
		using original_fn = bool(__thiscall*)(iv_engine_client*);
		return (*(original_fn**)this)[92](this);
	}
};