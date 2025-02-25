#pragma once

#include "Saba/Core/Core.h"

namespace Saba {

class FileProcessing {
public:
    SB_CORE static std::string ReadFromFile(const std::string& filepath);
    SB_CORE static std::string ReadFromFile(const std::filesystem::path& filepath);
};

}
