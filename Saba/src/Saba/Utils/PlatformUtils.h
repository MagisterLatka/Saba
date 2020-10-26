#pragma once

#include "Saba/Core.h"

namespace Saba {

	class FileDialogs
	{
	public:
		static std::string Open(const char* filter);
		static std::string Save(const char* filter);
	};
}
