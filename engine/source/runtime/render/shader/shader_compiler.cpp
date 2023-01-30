#include "shader_compiler.hpp"

namespace engine {

	ShaderCompiler::ShaderCompiler() {
		DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
		DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
		dxcUtils->CreateDefaultIncludeHandler(&dxcIncludeHandler);
	}

	ShaderCompiler::~ShaderCompiler() {
		dxcUtils->Release();
		dxcCompiler->Release();
		dxcIncludeHandler->Release();

		if (dxcCompileResult) {
			dxcCompileResult->Release();
		}
	}

	void ShaderCompiler::Define(const wchar_t* macro) {
		arguments.emplace_back(L"-D");
		arguments.emplace_back(macro);
	}

	void ShaderCompiler::AddIncludePath(const wchar_t* includePath) {
		arguments.emplace_back(L"-I");
		arguments.emplace_back(includePath);
	}

	void ShaderCompiler::SetEncoding(const wchar_t* utf8_or_utf16) {
		arguments.emplace_back(L"-encoding");
		arguments.emplace_back(utf8_or_utf16);
	}

	void ShaderCompiler::AddSpirvExtension(const wchar_t* extension) {
		std::wstring argument = L"-fspv-extension=";
		argument += extension;
		arguments.emplace_back(argument);
	}

	void ShaderCompiler::SetSpirvTargetEnv(const wchar_t* targetEnv) {
		arguments.emplace_back(std::wstring(L"-fspv-target-env=") + targetEnv);
	}

	void ShaderCompiler::SetSpirvRegisterShift(const wchar_t* registerType, const wchar_t* bindingShift, const wchar_t* space) {
		arguments.emplace_back(std::wstring(L"-fvk-") + registerType + std::wstring(L"-shift"));
		arguments.emplace_back(bindingShift);
		arguments.emplace_back(space);
	}

	void ShaderCompiler::SetSpirvBindGlobals(const wchar_t* binding, const wchar_t* descSet) {
		arguments.emplace_back(L"-fvk-bind-globals");
		arguments.emplace_back(binding);
		arguments.emplace_back(descSet);
	}

	void ShaderCompiler::SetSpirvBindRegister(const wchar_t* registerSlot, const wchar_t* space, const wchar_t* binding, const wchar_t* descSet) {
		arguments.emplace_back(L"-fvk-bind-register");
		arguments.emplace_back(registerSlot);
		arguments.emplace_back(space);
		arguments.emplace_back(binding);
		arguments.emplace_back(descSet);
	}

	void ShaderCompiler::ConfigSpirvOptimization(const wchar_t* info) {
		arguments.emplace_back(std::wstring(L"-Oconfig=") + info);
	}

	void ShaderCompiler::AddArgument(const wchar_t* argument) {
		arguments.emplace_back(argument);
	}

	void ShaderCompiler::AddArgument(const wchar_t* profile, const wchar_t* argument) {
		profileArguments[profile].emplace_back(argument);
	}

	void ShaderCompiler::ClearArguments() {
		{ decltype(arguments) temp; temp.swap(arguments); }
		{ decltype(profileArguments) temp; temp.swap(profileArguments); }
	}

	void ShaderCompiler::Compile(char* pSourceCode, uint32_t sourceCodeSize, const wchar_t* entryPoint, const wchar_t* profile) {
		std::vector<const wchar_t*> arguments;
		arguments.emplace_back(L"-E");
		arguments.emplace_back(entryPoint);
		arguments.emplace_back(L"-T");
		arguments.emplace_back(profile);

		for (auto& argument : this->arguments) {
			arguments.emplace_back(argument.c_str());
		}
		
		if (profileArguments.find(profile) != profileArguments.end()) {
			for (auto& argument : profileArguments.at(profile)) {
				arguments.emplace_back(argument.c_str());
			}
		}

		IDxcBlobEncoding* dxcEncoding;
		dxcUtils->CreateBlob(pSourceCode, sourceCodeSize, CP_UTF8, &dxcEncoding);

		DxcBuffer dxcSourceBuffer;
		dxcSourceBuffer.Ptr = dxcEncoding->GetBufferPointer();
		dxcSourceBuffer.Size = dxcEncoding->GetBufferSize();
		dxcSourceBuffer.Encoding = 0;

		if (dxcCompileResult) {
			dxcCompileResult->Release();
			dxcCompileResult = nullptr;
		}

		dxcCompiler->Compile(&dxcSourceBuffer, arguments.data(), static_cast<uint32_t>(arguments.size()), dxcIncludeHandler, IID_PPV_ARGS(&dxcCompileResult));

		if (!dxcCompileResult) {
			throw std::runtime_error("Cannot compile the shader file");
		}

		dxcEncoding->Release();
	}

	void ShaderCompiler::Compile(const wchar_t* sourcePath, const wchar_t* entryPoint, const wchar_t* profile) {
		std::ifstream loadFile(sourcePath, std::ios::ate | std::ios::binary);

		if (!loadFile.is_open()) {
			throw std::runtime_error("Cannot open the shader file");
		}

		uint32_t sourceCodeSize = static_cast<uint32_t>(loadFile.tellg());

		std::vector<char> sourceCode(sourceCodeSize);
		loadFile.seekg(0);
		loadFile.read(sourceCode.data(), sourceCodeSize);
		loadFile.close();

		Compile(sourceCode.data(), sourceCodeSize, entryPoint, profile);
	}

	bool ShaderCompiler::GetSourceObject(std::vector<char>& sourceObject) const {
		if (!dxcCompileResult) {
			throw std::runtime_error("No shader is compiled");
		}

		IDxcBlob* dxcObjectInfo;
		IDxcBlobWide* outputName;
		dxcCompileResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&dxcObjectInfo), &outputName);

		if (outputName) outputName->Release();

		if (!dxcObjectInfo || dxcObjectInfo->GetBufferSize() < 0) {
			return false;
		}

		sourceObject.resize(dxcObjectInfo->GetBufferSize());
		char* ptr = static_cast<char*>(dxcObjectInfo->GetBufferPointer());
		std::copy(ptr, ptr + dxcObjectInfo->GetBufferSize(), sourceObject.begin());

		dxcObjectInfo->Release();

		return true;
	}

	bool ShaderCompiler::GetErrorInfo(std::string& errorInfo) const {
		if (!dxcCompileResult) {
			throw std::runtime_error("No shader is compiled");
		}

		IDxcBlobUtf8* dxcErrorInfo;
		IDxcBlobWide* outputName;
		dxcCompileResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&dxcErrorInfo), &outputName);

		if (outputName) outputName->Release();

		if (!dxcErrorInfo || dxcErrorInfo->GetStringLength() < 0) {
			return false;
		}
		
		errorInfo = static_cast<const char*>(dxcErrorInfo->GetStringPointer());

		dxcErrorInfo->Release();

		return true;
	}

	bool ShaderCompiler::GenerateSourceObjectFile(const wchar_t* path) const {
		std::vector<char> sourceObject;
		if (!GetSourceObject(sourceObject)) {
			return false;
		}

		std::ofstream outputFile(path, std::ios::trunc | std::ios::binary);
		outputFile.write(sourceObject.data(), sourceObject.size());
		outputFile.close();

		return true;
	}

	bool ShaderCompiler::GenerateErrorInfoFile(const wchar_t* path) const {
		std::string errorInfo;
		if (!GetErrorInfo(errorInfo)) {
			return false;
		}

		std::ofstream outputFile(path, std::ios::trunc | std::ios::binary);
		outputFile.write(errorInfo.data(), errorInfo.size());
		outputFile.close();

		return true;
	}

}