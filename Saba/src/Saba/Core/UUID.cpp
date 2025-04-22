#include <pch.h>
#include "UUID.h"

namespace Saba {

static std::random_device s_RandomDevice;
static std::mt19937_64 s_Engine(s_RandomDevice());
static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

UUID::UUID()
    : m_ID(s_UniformDistribution(s_Engine)) {}

} //namespace Saba
