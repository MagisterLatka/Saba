#pragma once

#include <chrono>
namespace Saba {

class Timer
{
public:
    SB_CORE Timer() noexcept { Reset(); }

    SB_CORE void Reset() noexcept
    {
        m_Start = std::chrono::high_resolution_clock::now();
    }

    SB_CORE double Elapsed() noexcept
    {
        return static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count()) * 0.001 * 0.001 * 0.001;
    }
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
};

} //namespace Saba
