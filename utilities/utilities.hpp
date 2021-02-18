#pragma once
#include <cstdint>
#include "../csgo-sdk/vectors/vector3d.hpp"

#define M_PI 3.14159265358979323846

namespace utilities {
	template<typename FuncType>
	__forceinline static FuncType call_virtual(void* ppClass, int index) {
		int* pVTable = *(int**)ppClass;
		int dwAddress = pVTable[index];
		return (FuncType)(dwAddress);
	}
	namespace math {
		template <typename t> t clamp_value(t value, t min, t max) {
			if (value > max) {
				return max;
			}
			if (value < min) {
				return min;
			}
			return value;
		}
	}
	struct hud_weapons_t {
		std::int32_t* get_weapon_count() {
			return reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + 0x80);
		}
	};

	void console_warning(const char* msg, ...);

	std::uint8_t* pattern_scan2(void* module, const char* signature);
	std::uint8_t* pattern_scan(const char* module_name, const char* signature) noexcept;

	void apply_clan_tag(const char* name);
	void clamp_angles(vec3_t& angles);
	void material_setup();

	template<class T>
	static T* find_hud_element(const char* name);
}