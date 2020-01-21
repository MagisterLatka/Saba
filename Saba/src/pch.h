#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <memory>
#include <functional>

#define uint unsigned int

#define BIT(x) (1 << x)
#define SB_BIND_EVENT_FUNC(x) std::bind(&x, this, std::placeholders::_1)
