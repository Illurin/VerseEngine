#pragma once

#ifdef _WIN32

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include <array>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <string>

using Microsoft::WRL::ComPtr;

#endif // _WIN32