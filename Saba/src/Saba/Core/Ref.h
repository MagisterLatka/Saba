#pragma once

namespace Saba {

template<typename T>
class Ref;

class RefCounted {
    template<typename T>
    friend class Ref;
public:
    virtual ~RefCounted() = default;
    uint32_t GetRefCount() const noexcept { return m_RefCount; }
private:
    void IncrementRefCount() const noexcept { ++m_RefCount; }
    void DecrementRefCount() const noexcept { --m_RefCount; }
private:
    mutable uint32_t m_RefCount = 0u;
};

template<typename T>
class Ref {
    static_assert(std::is_base_of_v<RefCounted, T>);
public:
    Ref() noexcept
        : m_Instance(nullptr) {}
    Ref(std::nullptr_t) noexcept
        : m_Instance(nullptr) {}
    Ref(T* instance) noexcept
        : m_Instance(instance)
    {
        Increment();
    }
    Ref(const Ref<T>& other) noexcept
        : m_Instance(other.m_Instance)
    {
        Increment();
    }
    Ref(Ref<T>&& other) noexcept
        : m_Instance(other.m_Instance)
    {
        other.m_Instance = nullptr;
    }
    template<typename T2> requires (std::is_base_of_v<RefCounted, T2>)
    Ref(const Ref<T2>& other) noexcept(std::is_base_of_v<T, T2>) {
        if constexpr (std::is_base_of_v<T, T2>) {
            m_Instance = static_cast<T*>(other.m_Instance);
        }
        else if constexpr (std::is_base_of_v<T2, T>) {
            m_Instance = dynamic_cast<T*>(other.m_Instance);
            SB_CORE_ASSERT(other.m_Instance == nullptr || m_Instance != nullptr,
                "Upcasting from {0} to {1} failed", typeid(T2).name(), typeid(T).name());
        }
        else {
            SB_CORE_THROW_INFO("Cannot cast from {0} to {1}",
                typeid(T2).name(), typeid(T).name());
        }
        Increment();
    }
    template<typename T2> requires (std::is_base_of_v<RefCounted, T2>)
    Ref(Ref<T2>&& other) noexcept(std::is_base_of_v<T, T2>) {
        if constexpr (std::is_base_of_v<T, T2>) {
            m_Instance = static_cast<T*>(other.m_Instance);
        }
        else if constexpr (std::is_base_of_v<T2, T>) {
            m_Instance = dynamic_cast<T*>(other.m_Instance);
            SB_CORE_ASSERT(other.m_Instance == nullptr || m_Instance != nullptr,
                "Upcasting from {0} to {1} failed", typeid(T2).name(), typeid(T).name());
        }
        else {
            SB_CORE_THROW_INFO("Cannot cast from {0} to {1}",
                typeid(T2).name(), typeid(T).name());
        }
        other.m_Instance = nullptr;
    }
    ~Ref() { Decrement(); }

    Ref& operator=(std::nullptr_t) noexcept {
        Decrement();
        m_Instance = nullptr;
        return *this;
    }
    Ref& operator=(const Ref<T>& other) noexcept {
        if (this == &other) return *this;

        other.Increment();
        Decrement();

        m_Instance = other.m_Instance;
        return *this;
    }
    Ref& operator=(Ref<T>&& other) noexcept {
        Decrement();

        m_Instance = other.m_Instance;
        other.m_Instance = nullptr;
        return *this;
    }
    template<typename T2> requires (std::is_base_of_v<RefCounted, T2>)
    Ref& operator=(const Ref<T2>& other) noexcept(std::is_base_of_v<T, T2>) {
        if (this == &other) return *this;

        other.Increment();
        Decrement();

        if constexpr (std::is_base_of_v<T, T2>) {
            m_Instance = static_cast<T*>(other.m_Instance);
        }
        else if constexpr (std::is_base_of_v<T2, T>) {
            m_Instance = dynamic_cast<T*>(other.m_Instance);
            SB_CORE_ASSERT(other.m_Instance == nullptr || m_Instance != nullptr,
                "Upcasting from {0} to {1} failed", typeid(T2).name(), typeid(T).name());
        }
        else {
            SB_CORE_THROW_INFO("Cannot cast from {0} to {1}",
                typeid(T2).name(), typeid(T).name());
        }
        return *this;
    }
    template<typename T2> requires (std::is_base_of_v<RefCounted, T2>)
    Ref& operator=(Ref<T2>&& other) noexcept(std::is_base_of_v<T, T2>) {
        Decrement();

        if constexpr (std::is_base_of_v<T, T2>) {
            m_Instance = static_cast<T*>(other.m_Instance);
        }
        else if constexpr (std::is_base_of_v<T2, T>) {
            m_Instance = dynamic_cast<T*>(other.m_Instance);
            SB_CORE_ASSERT(other.m_Instance == nullptr || m_Instance != nullptr,
                "Upcasting from {0} to {1} failed", typeid(T2).name(), typeid(T).name());
        }
        else {
            SB_CORE_THROW_INFO("Cannot cast from {0} to {1}",
                typeid(T2).name(), typeid(T).name());
        }
        other.m_Instance = nullptr;
        return *this;
    }

    operator bool() const noexcept { return m_Instance != nullptr; }
    bool operator==(const Ref<T>& other) const noexcept { return m_Instance == other.m_Instance; }
    template<typename T2> requires(std::is_base_of_v<RefCounted, T2>)
    bool operator==(const Ref<T2>& other) const noexcept { return m_Instance == other.m_Instance; }

    T* operator->() { SB_CORE_ASSERT(m_Instance != nullptr); return m_Instance; }
    const T* operator->() const { SB_CORE_ASSERT(m_Instance != nullptr); return m_Instance; }
    T& operator*() { SB_CORE_ASSERT(m_Instance != nullptr); return *m_Instance; }
    const T& operator*() const { SB_CORE_ASSERT(m_Instance != nullptr); return *m_Instance; }

    T* Raw() { SB_CORE_ASSERT(m_Instance != nullptr); return m_Instance; }
    const T* Raw() const { SB_CORE_ASSERT(m_Instance != nullptr); return m_Instance; }

    void Reset(T* instance = nullptr) noexcept {
        Decrement();
        m_Instance = instance;
    }

    template<typename T2> requires(std::is_base_of_v<RefCounted, T2>)
    static constexpr bool IsConvertible() noexcept {
        return std::is_convertible_v<T2*, T*> || std::is_convertible_v<T*, T2*>;
    }
    template<typename T2> requires(std::is_base_of_v<RefCounted, T2>)
    bool CanConvert() const noexcept {
        if constexpr (!IsConvertible<T2>()) return false;

        if constexpr (std::is_base_of_v<T2, T>)
            return true;
        if constexpr (std::is_base_of_v<T, T2>)
            return m_Instance == nullptr || dynamic_cast<T2*>(m_Instance) != nullptr;
        return false;
    }

    template<typename T2> requires(std::is_base_of_v<RefCounted, T2>)
    Ref<T2> As() const {
        SB_CORE_ASSERT(CanConvert<T2>(), "Cannot convert from {0} to {1}",
            typeid(T).name(), typeid(T2).name());
        return Ref<T2>(*this);
    }

    template<typename ...Args> requires (std::is_constructible_v<T, Args...>)
    static Ref<T> Create(Args&& ...args) noexcept {
        return Ref<T>(new T(std::forward<Args>(args)...));
    }
private:
    void Increment() const noexcept {
        if (m_Instance)
            m_Instance->IncrementRefCount();
    }
    void Decrement() const noexcept {
        if (m_Instance == nullptr)
            return;

        m_Instance->DecrementRefCount();
        if (m_Instance->GetRefCount() == 0u)
            delete m_Instance;
    }
private:
    template<typename T2>
    friend class Ref;

    T* m_Instance;
};

} //namespace Saba

template<typename T>
using Ref = Saba::Ref<T>;
