#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <memory>
#include <functional>
#include <thread>
#include <chrono>

#include <glm\glm.hpp>

#include <Saba\Log.h>

#define BIT(x) (1 << x)
#define SB_BIND_EVENT_FUNC(x) std::bind(&x, this, std::placeholders::_1)

namespace Saba {

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> MakeRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> MakeScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

}
