#pragma once
#include <windows.h>
#include <iostream>
#include <cstdint>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <array>
#include <fstream>
#include <unordered_map>

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "utility/singleton.hpp"
#include "utilities/fnv.hpp"
#include "utilities/utilities.hpp"
#include "drawings/drawings.hpp"
#include "utilities/hook.hpp"
#include "interfaces/interfaces.hpp"
#include "csgo-sdk/include_sdk.hpp"
#include "hooks/hooks.hpp"
#include "csgo-sdk/math/math.hpp"
#include "hooks/minhooks/minhook.h"
#include "utility/xorstr.hpp"

/*
#include "utility/singleton.hpp"
#include "utilities/utilities.hpp"
#include "hooks/hooks.hpp"
#include "interfaces/interfaces.hpp"
#include "drawings/drawings.hpp"
#include "csgo-sdk/math/math.hpp"
#include "hooks/minhooks/minhook.h"
#include "csgo-sdk/include_sdk.hpp"
#include "utility/xorstr.hpp"
*/