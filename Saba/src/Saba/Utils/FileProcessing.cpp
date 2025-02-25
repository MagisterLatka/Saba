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
    else SB_CORE_THROW_INFO("Could not load from " + filepath.string());

    in.close();
    return result;
}

}
