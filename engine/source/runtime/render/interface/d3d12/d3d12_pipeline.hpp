#pragma once

#include "d3d12_util.hpp"

#ifdef _DIRECT3D12

struct alignas(void*) D3D12StreamingRasterizerDesc {
private:
	D3D12_PIPELINE_STATE_SUBOBJECT_TYPE type{ D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_RASTERIZER };
public:
	D3D12_RASTERIZER_DESC value;
};

struct alignas(void*) D3D12StreamingDepthStencilDesc1 {
private:
	D3D12_PIPELINE_STATE_SUBOBJECT_TYPE type{ D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL1 };
public:
	D3D12_DEPTH_STENCIL_DESC1 value;
};

struct alignas(void*) D3D12StreamingBlendDesc {
private:
	D3D12_PIPELINE_STATE_SUBOBJECT_TYPE type{ D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_BLEND };
public:
	D3D12_BLEND_DESC value;
};

struct alignas(void*) D3D12StreamingPrimitiveTopologyType {
private:
	D3D12_PIPELINE_STATE_SUBOBJECT_TYPE type{ D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_PRIMITIVE_TOPOLOGY };
public:
	D3D12_PRIMITIVE_TOPOLOGY_TYPE value;
};

struct alignas(void*) D3D12StreamingIBStripCutValue {
private:
	D3D12_PIPELINE_STATE_SUBOBJECT_TYPE type{ D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_IB_STRIP_CUT_VALUE };
public:
	D3D12_INDEX_BUFFER_STRIP_CUT_VALUE value;
};

struct alignas(void*) D3D12StreamingSampleDesc {
private:
	D3D12_PIPELINE_STATE_SUBOBJECT_TYPE type{ D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_SAMPLE_DESC };
public:
	DXGI_SAMPLE_DESC value;
};

struct alignas(void*) D3D12StreamingRenderTargetFormats {
private:
	D3D12_PIPELINE_STATE_SUBOBJECT_TYPE type{ D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_RENDER_TARGET_FORMATS };
public:
	D3D12_RT_FORMAT_ARRAY value;
};

struct alignas(void*) D3D12StreamingDepthStencilFormat {
private:
	D3D12_PIPELINE_STATE_SUBOBJECT_TYPE type{ D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL_FORMAT };
public:
	DXGI_FORMAT value;
};

struct alignas(void*) D3D12StreamingVS {
private:
	D3D12_PIPELINE_STATE_SUBOBJECT_TYPE type{ D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_VS };
public:
	D3D12_SHADER_BYTECODE value;
};

struct alignas(void*) D3D12StreamingHS {
private:
	D3D12_PIPELINE_STATE_SUBOBJECT_TYPE type{ D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_HS };
public:
	D3D12_SHADER_BYTECODE value;
};

struct alignas(void*) D3D12StreamingDS {
private:
	D3D12_PIPELINE_STATE_SUBOBJECT_TYPE type{ D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DS };
public:
	D3D12_SHADER_BYTECODE value;
};

struct alignas(void*) D3D12StreamingGS {
private:
	D3D12_PIPELINE_STATE_SUBOBJECT_TYPE type{ D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_GS };
public:
	D3D12_SHADER_BYTECODE value;
};

struct alignas(void*) D3D12StreamingPS {
private:
	D3D12_PIPELINE_STATE_SUBOBJECT_TYPE type{ D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_PS };
public:
	D3D12_SHADER_BYTECODE value;
};

struct alignas(void*) D3D12StreamingRootSignature {
private:
	D3D12_PIPELINE_STATE_SUBOBJECT_TYPE type{ D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_ROOT_SIGNATURE };
public:
	ID3D12RootSignature* value;
};

struct D3D12StreamingGraphicsPipelineDesc {
	D3D12StreamingVS vs;
	D3D12StreamingHS hs;
	D3D12StreamingDS ds;
	D3D12StreamingGS gs;
	D3D12StreamingPS ps;
	D3D12StreamingRootSignature rootSignature;
	D3D12StreamingRasterizerDesc rasterizationDesc;
	D3D12StreamingDepthStencilDesc1 depthStencilDesc;
	D3D12StreamingBlendDesc blendDesc;
	D3D12StreamingPrimitiveTopologyType primitiveTopologyType;
	D3D12StreamingIBStripCutValue ibStripCutValue;
	D3D12StreamingSampleDesc sampleDesc;
	D3D12StreamingRenderTargetFormats renderTargetFormats;
	D3D12StreamingDepthStencilFormat depthStencilFormat;
};

#endif // _DIRECT3D12