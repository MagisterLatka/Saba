#pragma once

#include <Saba/Core/Platform.h>

#include <algorithm>
#include <array>
#include <bitset>
#include <chrono>
#include <exception>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <set>
#include <ranges>

#include <Saba/Core/Log.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

#if defined(SB_PLATFORM_WINDOWS)
#   ifndef NOMIXMAX
#       define NOMINMAX
#   endif
#   define WIN32_LEAN_AND_MEAN
#   define NOGDICAPMASKS
#   define NOSYSMETRICS
#   define NOMENUS
#   define NOICONS
#   define NOSYSCOMMANDS
#   define NORASTEROPS
#   define OEMRESOURCE
#   define NOATOM
#   define NOCLIPBOARD
#   define NOCOLOR
#   define NOCTLMGR
#   define NODRAWTEXT
#   define NOKERNEL
#   define NONLS
#   define NOMEMMGR
#   define NOMETAFILE
#   define NOOPENFILE
#   define NOSCROLL
#   define NOSERVICE
#   define NOSOUND
#   define NOTEXTMETRIC
#   define NOWH
#   define NOCOMM
#   define NOKANJI
#   define NOHELP
#   define NOPROFILER
#   define NODEFERWINDOWPOS
#   define NOMCX
#   define NORPC
#   define NOPROXYSTUB
#   define NOIMAGE
#   define NOTAPE
#   include <Windows.h>

#   include <wrl.h>
    template<typename T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;
#endif
