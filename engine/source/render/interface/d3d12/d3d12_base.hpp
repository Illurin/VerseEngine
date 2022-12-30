#pragma once

#ifdef _WIN32

#include "d3d12_util.hpp"

namespace engine {

	class D3D12WrapperDevice final : public RHIDevice {
	public:
		D3D12WrapperDevice& SetDevice(ComPtr<ID3D12Device> device) { this->device = device; return *this; }
		ComPtr<ID3D12Device> GetDevice() const { return device; }

	private:
		ComPtr<ID3D12Device> device;
	};

	class D3D12WrapperSwapchain final : public RHISwapchain {
	public:
		void Set(ComPtr<IDXGISwapChain3> res) { resource = res; }
		ComPtr<IDXGISwapChain3> Get() const { return resource; }

	private:
		ComPtr<IDXGISwapChain3> resource;
	};

	class D3D12EnumFormat {
	public:
		D3D12EnumFormat(RHIFormat format) {
			switch (format) {
			case RHIFormat::Undefined: format_ = DXGI_FORMAT_UNKNOWN; break;
			case RHIFormat::R8Unorm: format_ = DXGI_FORMAT_R8_UNORM; break;
			case RHIFormat::R8G8Unorm: format_ = DXGI_FORMAT_R8G8_UNORM; break;
			//case RHIFormat::R8G8B8Unorm: format_ = DXGI_FORMAT_R8G8B8_UNORM; break;
			case RHIFormat::R8G8B8A8Unorm: format_ = DXGI_FORMAT_R8G8B8A8_UNORM; break;
			case RHIFormat::R8Snorm: format_ = DXGI_FORMAT_R8_SNORM; break;
			case RHIFormat::R8G8Snorm: format_ = DXGI_FORMAT_R8G8_SNORM; break;
			//case RHIFormat::R8G8B8Snorm: format_ = DXGI_FORMAT_R8G8B8_SNORM; break;
			case RHIFormat::R8G8B8A8Snorm: format_ = DXGI_FORMAT_R8G8B8A8_SNORM; break;
			case RHIFormat::R8Uint: format_ = DXGI_FORMAT_R8_UINT; break;
			case RHIFormat::R8G8Uint: format_ = DXGI_FORMAT_R8G8_UINT; break;
			//case RHIFormat::R8G8B8Uint: format_ = DXGI_FORMAT_R8G8B8_UINT; break;
			case RHIFormat::R8G8B8A8Uint: format_ = DXGI_FORMAT_R8G8B8A8_UINT; break;
			case RHIFormat::R8Sint: format_ = DXGI_FORMAT_R8_SINT; break;
			case RHIFormat::R8G8Sint: format_ = DXGI_FORMAT_R8G8_SINT; break;
			//case RHIFormat::R8G8B8Sint: format_ = DXGI_FORMAT_R8G8B8_SINT; break;
			case RHIFormat::R8G8B8A8Sint: format_ = DXGI_FORMAT_R8G8B8A8_SINT; break;
			case RHIFormat::R16Unorm: format_ = DXGI_FORMAT_R16_UNORM; break;
			case RHIFormat::R16G16Unorm: format_ = DXGI_FORMAT_R16G16_UNORM; break;
			//case RHIFormat::R16G16B16Unorm: format_ = DXGI_FORMAT_R16G16B16_UNORM; break;
			case RHIFormat::R16G16B16A16Unorm: format_ = DXGI_FORMAT_R16G16B16A16_UNORM; break;
			case RHIFormat::R16Snorm: format_ = DXGI_FORMAT_R16_SNORM; break;
			case RHIFormat::R16G16Snorm: format_ = DXGI_FORMAT_R16G16_SNORM; break;
			//case RHIFormat::R16G16B16Snorm: format_ = DXGI_FORMAT_R16G16B16_SNORM; break;
			case RHIFormat::R16G16B16A16Snorm: format_ = DXGI_FORMAT_R16G16B16A16_SNORM; break;
			case RHIFormat::R16Uint: format_ = DXGI_FORMAT_R16_UINT; break;
			case RHIFormat::R16G16Uint: format_ = DXGI_FORMAT_R16G16_UINT; break;
			//case RHIFormat::R16G16B16Uint: format_ = DXGI_FORMAT_R16G16B16_UINT; break;
			case RHIFormat::R16G16B16A16Uint: format_ = DXGI_FORMAT_R16G16B16A16_UINT; break;
			case RHIFormat::R16Sint: format_ = DXGI_FORMAT_R16_SINT; break;
			case RHIFormat::R16G16Sint: format_ = DXGI_FORMAT_R16G16_SINT; break;
			//case RHIFormat::R16G16B16Sint: format_ = DXGI_FORMAT_R16G16B16_SINT; break;
			case RHIFormat::R16G16B16A16Sint: format_ = DXGI_FORMAT_R16G16B16A16_SINT; break;
			case RHIFormat::R16Sfloat: format_ = DXGI_FORMAT_R16_FLOAT; break;
			case RHIFormat::R16G16Sfloat: format_ = DXGI_FORMAT_R16G16_FLOAT; break;
			//case RHIFormat::R16G16B16Sfloat: format_ = DXGI_FORMAT_R16G16B16_FLOAT; break;
			case RHIFormat::R16G16B16A16Sfloat: format_ = DXGI_FORMAT_R16G16B16A16_FLOAT; break;
			case RHIFormat::R32Uint: format_ = DXGI_FORMAT_R32_UINT; break;
			case RHIFormat::R32G32Uint: format_ = DXGI_FORMAT_R32G32_UINT; break;
			case RHIFormat::R32G32B32Uint: format_ = DXGI_FORMAT_R32G32B32_UINT; break;
			case RHIFormat::R32G32B32A32Uint: format_ = DXGI_FORMAT_R32G32B32A32_UINT; break;
			case RHIFormat::R32Sint: format_ = DXGI_FORMAT_R32_SINT; break;
			case RHIFormat::R32G32Sint: format_ = DXGI_FORMAT_R32G32_SINT; break;
			case RHIFormat::R32G32B32Sint: format_ = DXGI_FORMAT_R32G32B32_SINT; break;
			case RHIFormat::R32G32B32A32Sint: format_ = DXGI_FORMAT_R32G32B32A32_SINT; break;
			case RHIFormat::R32Sfloat: format_ = DXGI_FORMAT_R32_FLOAT; break;
			case RHIFormat::R32G32Sfloat: format_ = DXGI_FORMAT_R32G32_FLOAT; break;
			case RHIFormat::R32G32B32Sfloat: format_ = DXGI_FORMAT_R32G32B32_FLOAT; break;
			case RHIFormat::R32G32B32A32Sfloat: format_ = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
			//case RHIFormat::R64Uint: format_ = DXGI_FORMAT_R64_UINT; break;
			//case RHIFormat::R64G64Uint: format_ = DXGI_FORMAT_R64G64_UINT; break;
			//case RHIFormat::R64G64B64Uint: format_ = DXGI_FORMAT_R64G64B64_UINT; break;
			//case RHIFormat::R64G64B64A64Uint: format_ = DXGI_FORMAT_R64G64B64A64_UINT; break;
			//case RHIFormat::R64Sint: format_ = DXGI_FORMAT_R64_SINT; break;
			//case RHIFormat::R64G64Sint: format_ = DXGI_FORMAT_R64G64_SINT; break;
			//case RHIFormat::R64G64B64Sint: format_ = DXGI_FORMAT_R64G64B64_SINT; break;
			//case RHIFormat::R64G64B64A64Sint: format_ = DXGI_FORMAT_R64G64B64A64_SINT; break;
			//case RHIFormat::R64Sfloat: format_ = DXGI_FORMAT_R64_FLOAT; break;
			//case RHIFormat::R64G64Sfloat: format_ = DXGI_FORMAT_R64G64_FLOAT; break;
			//case RHIFormat::R64G64B64Sfloat: format_ = DXGI_FORMAT_R64G64B64_FLOAT; break;
			//case RHIFormat::R64G64B64A64Sfloat: format_ = DXGI_FORMAT_R64G64B64A64_FLOAT; break;
			case RHIFormat::D16Unorm: format_ = DXGI_FORMAT_D16_UNORM; break;
			case RHIFormat::D32Sfloat: format_ = DXGI_FORMAT_D32_FLOAT; break;
			//case RHIFormat::S8Uint: format_ = DXGI_FORMAT_S8_UINT; break;
			//case RHIFormat::D16UnormS8Uint: format_ = DXGI_FORMAT_D16_UNORM_S8_UINT; break;
			case RHIFormat::D24UnormS8Uint: format_ = DXGI_FORMAT_D24_UNORM_S8_UINT; break;
			case RHIFormat::D32SfloatS8Uint: format_ = DXGI_FORMAT_D32_FLOAT_S8X24_UINT; break;
			}
		}

		DXGI_FORMAT Get() { return format_; }

	private:
		DXGI_FORMAT format_{ DXGI_FORMAT_UNKNOWN };
	};

}

#endif // _WIN32