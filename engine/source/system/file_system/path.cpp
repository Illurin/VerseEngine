#include "path.hpp"

namespace engine {

	std::wstring StringToWString(const std::string& source) {
#ifdef _WIN32
		auto charSize = MultiByteToWideChar(CP_ACP, 0, source.c_str(), -1, nullptr, 0);
		std::vector<wchar_t> dest(charSize);
		MultiByteToWideChar(CP_ACP, 0, source.c_str(), -1, dest.data(), charSize);
		std::wstring output(dest.data());
		return output;
#else
		std::wstring_convert<std::codecvt_byname<wchar_t, char, std::mbstate_t>> converter;
		return converter.from_bytes(source);
#endif // _WIN32
	}

	std::string WStringToString(const std::wstring& source) {
#ifdef _WIN32
		auto charSize = WideCharToMultiByte(CP_ACP, 0, source.c_str(), -1, nullptr, 0, nullptr, nullptr);
		std::vector<char> dest(charSize);
		WideCharToMultiByte(CP_ACP, 0, source.c_str(), -1, dest.data(), charSize, nullptr, nullptr);
		std::string output(dest.data());
		return output;
#else
		std::wstring_convert<std::codecvt_byname<wchar_t, char, std::mbstate_t>> converter;
		return converter.to_bytes(source);
#endif // _WIN32
	}

	void Path::Init() {
		std::replace(path.begin(), path.end(), '\\', '/');
		if (path.rfind('/') == path.length() - 1) path = path.substr(0, path.length() - 1);
	}

	std::string Path::GetAbsolutePath() {
		return std::string(GlobalConfig::configPath) + '/' + path;
	}

	std::string Path::GetRelativePath() {
		return path.replace(path.find(GlobalConfig::configPath), std::string(GlobalConfig::configPath).length() + 1, "");
	}

	std::string Path::GetParentPath() {
		return path.substr(0, path.rfind('/'));
	}

}