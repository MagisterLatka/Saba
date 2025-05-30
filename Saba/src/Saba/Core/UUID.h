#pragma once

namespace Saba {

class UUID {
public:
    SB_CORE UUID();
    UUID(uint64_t uuid) noexcept
        : m_ID(uuid) {}

    operator uint64_t() const { return m_ID; }
private:
    uint64_t m_ID;
};

} //namespace Saba

namespace std {

template<>
struct hash<Saba::UUID> {
    std::size_t operator()(const Saba::UUID& uuid) const {
        return hash<uint64_t>()(uuid);
    }
};

} //namespace std
