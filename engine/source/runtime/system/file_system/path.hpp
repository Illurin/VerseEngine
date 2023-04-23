#pragma once

#include <filesystem>

namespace engine {

    std::wstring StringToWString(const std::string& source);
    std::string WStringToString(const std::wstring& source);

    class Path {
    public:
        static void SetDirectory(std::filesystem::path directory) { Path::directory = directory; }
        static std::filesystem::path GetDirectory() { return directory; }
        static std::filesystem::path GetAbsolutePath(std::filesystem::path path) { return directory / path; }

    private:
        static std::filesystem::path directory;
    };

}