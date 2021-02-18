#include "utilities.hpp"
#include "../includes.hpp"
#include <psapi.h>

template<class T>
static T* utilities::find_hud_element(const char* name) {
	static auto fn = *reinterpret_cast<DWORD**>(utilities::pattern_scan(("client.dll"), ("B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08")) + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(utilities::pattern_scan(("client.dll"), ("55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28")));
	return (T*)find_hud_element(fn, name);
}

void utilities::console_warning(const char* msg, ...) {
	if (msg == nullptr)
		return;
	typedef void(__cdecl* console_warning_fn)(const char* msg, va_list);
	static console_warning_fn fn = (console_warning_fn)GetProcAddress(GetModuleHandle("tier0.dll"), "Warning");
	char buffer[989];
	va_list list;
	va_start(list, msg);
	vsprintf(buffer, msg, list);
	va_end(list);
	fn(buffer, list);
}

std::uint8_t* utilities::pattern_scan(const char* module_name, const char* signature) noexcept {
	const auto module_handle = GetModuleHandleA((LPCSTR)module_name);

	if (!module_handle)
		return nullptr;

	static auto pattern_to_byte = [](const char* pattern) {
		auto bytes = std::vector<int>{};
		auto start = const_cast<char*>(pattern);
		auto end = const_cast<char*>(pattern) + std::strlen(pattern);

		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				++current;

				if (*current == '?')
					++current;

				bytes.push_back(-1);
			}
			else {
				bytes.push_back(std::strtoul(current, &current, 16));
			}
		}
		return bytes;
	};

	auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
	auto nt_headers =
		reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module_handle) + dos_header->e_lfanew);

	auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	auto pattern_bytes = pattern_to_byte(signature);
	auto scan_bytes = reinterpret_cast<std::uint8_t*>(module_handle);

	auto s = pattern_bytes.size();
	auto d = pattern_bytes.data();

	for (auto i = 0ul; i < size_of_image - s; ++i) {
		bool found = true;

		for (auto j = 0ul; j < s; ++j) {
			if (scan_bytes[i + j] != d[j] && d[j] != -1) {
				found = false;
				break;
			}
		}
		if (found)
			return &scan_bytes[i];
	}

	throw std::runtime_error(std::string(" [!] Wrong signature: ") + signature);
}

std::uint8_t* utilities::pattern_scan2(void* module, const char* signature)
{
	static auto pattern_to_byte = [](const char* pattern) {
		auto bytes = std::vector<int>{};
		auto start = const_cast<char*>(pattern);
		auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				++current;
				if (*current == '?')
					++current;
				bytes.push_back(-1);
			}
			else {
				bytes.push_back(strtoul(current, &current, 16));
			}
		}
		return bytes;
	};

	auto dosHeader = (PIMAGE_DOS_HEADER)module;
	auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

	auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
	auto patternBytes = pattern_to_byte(signature);
	auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

	auto s = patternBytes.size();
	auto d = patternBytes.data();

	for (auto i = 0ul; i < sizeOfImage - s; ++i) {
		bool found = true;
		for (auto j = 0ul; j < s; ++j) {
			if (scanBytes[i + j] != d[j] && d[j] != -1) {
				found = false;
				break;
			}
		}
		if (found) {
			return &scanBytes[i];
		}
	}
	return nullptr;
}

void utilities::apply_clan_tag(const char* name) {
	using Fn = int(__fastcall*)(const char*, const char*);
	static auto apply_clan_tag_fn = reinterpret_cast<Fn>(utilities::pattern_scan(("engine.dll"), "53 56 57 8B DA 8B F9 FF 15"));
	apply_clan_tag_fn(name, name);
};

void utilities::clamp_angles(vec3_t& angles) {
	if (angles.x > 89.0f) {
		angles.x = 89.0f;
	}
	else if (angles.x < -89.0f) {
		angles.x = -89.0f;
	}

	if (angles.y > 180.0f) {
		angles.y = 180.0f;
	}
	else if (angles.y < -180.0f) {
		angles.y = -180.0f;
	}

	angles.z = 0;
}

void utilities::material_setup() {
	std::ofstream("csgo/materials/aristois_material.vmt") << R"#("VertexLitGeneric" {
            "$basetexture" "vgui/white_additive"
            "$ignorez"      "0"
            "$envmap"       ""
            "$nofog"        "1"
            "$model"        "1"
            "$nocull"       "0"
            "$selfillum"    "1"
            "$halflambert"  "1"
            "$znearer"      "0"
            "$flat"         "0"
			"$phong"		"1"
			"$rimlight"		"1"
        })#";

	std::ofstream("csgo/materials/aristois_reflective.vmt") << R"#("VertexLitGeneric" {
			"$basetexture" "vgui/white_additive"
			"$ignorez" "0"
			"$envmap" "env_cubemap"
			"$normalmapalphaenvmapmask" "1"
			"$envmapcontrast"  "1"
			"$nofog" "1"
			"$model" "1"
			"$nocull" "0"
			"$selfillum" "1"
			"$halflambert" "1"
			"$znearer" "0"
			"$flat" "1"
		})#";
}