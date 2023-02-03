#pragma once

#ifdef _WIN32
#include <Unknwnbase.h>
#endif // _WIN32

#include "dxcapi.h"

#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>

namespace engine {

	struct ShaderCompileArgument {
		// compilation config
		static constexpr const wchar_t* backward_compatibility_mode = L"-Gec";
		static constexpr const wchar_t* strict_mode = L"-Ges";
		static constexpr const wchar_t* avoid_flow_control_constructs = L"-Gfa";
		static constexpr const wchar_t* prefer_flow_control_constructs = L"-Gfp";
		static constexpr const wchar_t* force_IEEE_strictness = L"-Gis";
		static constexpr const wchar_t* enable_16_bit_types = L"-enable-16bit-types";
		static constexpr const wchar_t* pack_optimized = L"-pack-optimized";
		static constexpr const wchar_t* pack_prefix_stable = L"-pack-prefix-stable";
		static constexpr const wchar_t* recompile = L"-recompile";
		static constexpr const wchar_t* matrix_column_major = L"-Zpc";
		static constexpr const wchar_t* matrix_row_major = L"-Zpr";

		// debug config
		static constexpr const wchar_t* no_warnings = L"-no-warnings";
		static constexpr const wchar_t* disable_validation = L"-Vd";
		static constexpr const wchar_t* treat_warnings_as_errors = L"-WX";
		static constexpr const wchar_t* enable_debug_info = L"-Zi";
		static constexpr const wchar_t* embed_debug = L"-Qembed_debug";

		// optimization config
		static constexpr const wchar_t* disable_optimizations = L"-Od";
		static constexpr const wchar_t* optimization_level_0 = L"-O0";
		static constexpr const wchar_t* optimization_level_1 = L"-O1";
		static constexpr const wchar_t* optimization_level_2 = L"-O2";
		static constexpr const wchar_t* optimization_level_3 = L"-O3";

		// spir-v code gen
		static constexpr const wchar_t* spirv = L"-spirv";
		static constexpr const wchar_t* spirv_debug = L"-fspv-debug=vulkan-with-source";
		static constexpr const wchar_t* spirv_flatten_resource_arrays = L"-fspv-flatten-resource-arrays";
		static constexpr const wchar_t* spirv_reduce_load_size = L"-fspv-reduce-load-size";
		static constexpr const wchar_t* spirv_reflect = L"-fspv-reflect";
		static constexpr const wchar_t* spirv_auto_shift_bindings = L"-fvk-auto-shift-bindings";
		static constexpr const wchar_t* spirv_support_nonzero_base_instance = L"-fvk-support-nonzero-base-instance";
		static constexpr const wchar_t* spirv_invert_y = L"-fvk-invert-y";
		static constexpr const wchar_t* spirv_use_dx_position_w = L"-fvk-use-dx-position-w";
		static constexpr const wchar_t* spirv_use_dx_layout = L"-fvk-use-dx-layout";
		static constexpr const wchar_t* spirv_use_gl_layout = L"-fvk-use-gl-layout";
		static constexpr const wchar_t* spirv_use_scalar_layout = L"-fvk-use-scalar-layout";

		// compile profile
		static constexpr const wchar_t* vs_profile = L"vs_6_6";
		static constexpr const wchar_t* hs_profile = L"hs_6_6";
		static constexpr const wchar_t* ds_profile = L"ds_6_6";
		static constexpr const wchar_t* gs_profile = L"gs_6_6";
		static constexpr const wchar_t* ps_profile = L"ps_6_6";
		static constexpr const wchar_t* cs_profile = L"cs_6_6";
	};

	class ShaderCompiler {
	public:
		ShaderCompiler();
		~ShaderCompiler();

		// compiler arguments
		void Define(const wchar_t* macro);
		void AddIncludePath(const wchar_t* includePath);
		void SetEncoding(const wchar_t* utf8_or_utf16);

		// spir-v code gen
		void AddSpirvExtension(const wchar_t* extension);
		void SetSpirvTargetEnv(const wchar_t* targetEnv);
		void SetSpirvRegisterShift(const wchar_t* registerType, const wchar_t* bindingShift, const wchar_t* space);
		void SetSpirvBindGlobals(const wchar_t* binding, const wchar_t* descSet);
		void SetSpirvBindRegister(const wchar_t* registerSlot, const wchar_t* space, const wchar_t* binding, const wchar_t* descSet);

		// see more optimization information in http://khr.io/hlsl2spirv#optimization
		void ConfigSpirvOptimization(const wchar_t* info);

		void AddArgument(const wchar_t* argument);
		void AddArgument(const wchar_t* profile, const wchar_t* argument);
		void ClearArguments();

		void Compile(char* pSourceCode, uint32_t sourceCodeSize, const wchar_t* entryPoint, const wchar_t* profile);
		void Compile(const wchar_t* sourcePath, const wchar_t* entryPoint, const wchar_t* profile);

		// get compile info
		bool GetSourceObject(std::vector<uint8_t>& sourceObject) const;
		bool GetErrorInfo(std::string& errorInfo) const;

		bool GenerateSourceObjectFile(const wchar_t* path) const;
		bool GenerateErrorInfoFile(const wchar_t* path) const;

	private:
		IDxcUtils* dxcUtils{ nullptr };
		IDxcCompiler3* dxcCompiler{ nullptr };
		IDxcIncludeHandler* dxcIncludeHandler{ nullptr };

		IDxcResult* dxcCompileResult{ nullptr };

		std::vector<std::wstring> arguments;
		std::unordered_map<const wchar_t*, std::vector<std::wstring>> profileArguments;
	};

}