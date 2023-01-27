#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif // _WIN32

#include <cstdint>
#include <array>
#include <vector>
#include <algorithm>
#include <codecvt>
#include <filesystem>

namespace engine {

	class GlobalConfig {
	public:
		static const char* configPath;
	};

}