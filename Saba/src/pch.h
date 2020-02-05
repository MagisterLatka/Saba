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

#include "Saba\Log.h"

#define uint unsigned int

#define BIT(x) (1 << x)
#define SB_BIND_EVENT_FUNC(x) std::bind(&x, this, std::placeholders::_1)
