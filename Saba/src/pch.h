#pragma once

#include <Saba/Platform.h>

#ifdef SB_PLATFORM_WINDOWS
#	ifndef NOMINMAX
#		define NOMINMAX
#	endif
#endif

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <array>
#include <memory>
#include <functional>
#include <thread>
#include <chrono>
#include <random>
#include <optional>

#include <glm/glm.hpp>

#include <Saba/Log.h>

#ifdef SB_PLATFORM_WINDOWS
#	include <Windows.h>
#endif
