#pragma once
#include "../classes/client_class.hpp"

class i_base_client_dll {
public:
	bool dispatch_user_message(int msg_type, unsigned int arg1, unsigned int length, const void* data = nullptr) {
		using original_fn = bool(__thiscall*)(void*, int, unsigned int, unsigned int, const void*);
		return (*(original_fn**)this)[38](this, msg_type, arg1, length, data);
	}
	c_client_class * get_client_classes() {
		using original_fn = c_client_class * (__thiscall*)(i_base_client_dll*);
		return (*(original_fn**)this)[8](this);
	}
};