#pragma once

#include "system/global_config.hpp"

#include <filesystem>

namespace engine {

	std::wstring StringToWString(const std::string& source);
	std::string WStringToString(const std::wstring& source);

	class Path {
	public:
		Path(const char* path) : path(path) { Init(); }
		Path(std::string& path) : path(path) { Init(); }

		std::string Get() const { return path; }

		std::string GetAbsolutePath();
		std::string GetRelativePath();

		std::string GetParentPath();

	private:
		void Init();

	private:
		std::string path;
	};

}