#pragma once

#ifdef _WIN32
#define _DIRECT3D12
#endif // _WIN32

#ifdef _DIRECT3D12

#include "d3dx12.hpp"
#include <dxgi1_6.h>
#include <wrl.h>

#include <array>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <string>

using Microsoft::WRL::ComPtr;

#endif // _DIRECT3D12