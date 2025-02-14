#pragma once

namespace Saba {

class SB_CORE Timestep
{
public:
    Timestep() = default;
    Timestep(double time) noexcept
        : m_Seconds(time) {}

    operator double() const noexcept { return m_Seconds; }

    double GetSeconds() const noexcept { return m_Seconds; }
    double GetMillliseconds() const noexcept { return m_Seconds * 1000.0; }
    double GetMicroseconds() const noexcept { return m_Seconds * 1000000.0; }
private:
    double m_Seconds = 0.0;
};

} //namespace Saba
