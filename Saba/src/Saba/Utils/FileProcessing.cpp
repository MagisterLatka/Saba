#include <pch.h>
#include "FileProcessing.h"

namespace Saba {

std::string FileProcessing::ReadFromFile(const std::string& filepath) {
    return ReadFromFile(std::filesystem::path(filepath));
}
std::string FileProcessing::ReadFromFile(const std::filesystem::path& filepath) {
    std::string result;
    std::ifstream in(filepath, std::ios_base::in | std::ios_base::binary);
    if (in) {
        in.seekg(0, std::ios::end);
        result.resize(static_cast<uint64_t>(in.tellg()));
        in.seekg(0, std::ios::beg);
        in.read(result.data(), static_cast<int64_t>(result.size()));
    }
    else SB_CORE_THROW_INFO("Could not load from {0}", filepath.string());

    in.close();
    return result;
}

#if defined(SB_PLATFORM_WINDOWS)
extern std::filesystem::path WindowsOpen();
extern std::filesystem::path WindowsSave();

std::filesystem::path FileProcessing::ChooseFileToOpenFrom() {
    return WindowsOpen();
}
std::filesystem::path FileProcessing::ChooseFileToSaveTo() {
    return WindowsSave();
}
#else
std::filesystem::path FileProcessing::ChooseFileToOpenFrom() {
    SB_CORE_INFO("Choose file to open from:");
    std::filesystem::path path;
    std::cin >> path;
    return path;
}
std::filesystem::path FileProcessing::ChooseFileToSaveTo() {
    SB_CORE_INFO("Choose file to save to:");
    std::filesystem::path path;
    std::cin >> path;
    return path;
}
#endif

}
