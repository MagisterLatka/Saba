#pragma once

namespace Saba {

class SB_CORE RefCounted
{
public:
    void IncrementRefCount() const noexcept { ++m_RefCount; }
    void DecrementRefCount() const noexcept { --m_RefCount; }

    uint32_t GetRefCount() const noexcept { return m_RefCount; }
private:
    mutable uint32_t m_RefCount = 0u;
};

template<typename T>
class Ref
{
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
    template<typename T2>
    Ref(const Ref<T2>& other) noexcept
    {
        static_assert(std::is_base_of_v<RefCounted, T2>);
        static_assert(std::is_convertible_v<T2*, T*> || std::is_convertible_v<T*, T2*>);
        m_Instance = static_cast<T*>(other.m_Instance);
        Increment();
    }
    template<typename T2>
    Ref(Ref<T2>&& other) noexcept
    {
        static_assert(std::is_base_of_v<RefCounted, T2>);
        static_assert(std::is_convertible_v<T2*, T*> || std::is_convertible_v<T*, T2*>);
        m_Instance = static_cast<T*>(other.m_Instance);
        other.m_Instance = nullptr;
    }
    ~Ref() { Decrement(); }

    Ref& operator=(std::nullptr_t) noexcept
    {
        Decrement();
        m_Instance = nullptr;
        return *this;
    }
    Ref& operator=(const Ref<T>& other) noexcept
    {
        other.Increment();
        Decrement();

        m_Instance = other.m_Instance;
        return *this;
    }
    Ref& operator=(Ref<T>&& other) noexcept
    {
        Decrement();

        m_Instance = other.m_Instance;
        other.m_Instance = nullptr;
        return *this;
    }
    template<typename T2>
    Ref& operator=(const Ref<T2>& other) noexcept
    {
        static_assert(std::is_base_of_v<RefCounted, T2>);
        static_assert(std::is_convertible_v<T2*, T*> || std::is_convertible_v<T*, T2*>);
        other.Increment();
        Decrement();

        m_Instance = static_cast<T*>(other.m_Instance);
        return *this;
    }
    template<typename T2>
    Ref& operator=(Ref<T2>&& other) noexcept
    {
        static_assert(std::is_base_of_v<RefCounted, T2>);
        static_assert(std::is_convertible_v<T2*, T*> || std::is_convertible_v<T*, T2*>);
        Decrement();

        m_Instance = static_cast<T*>(other.m_Instance);
        other.m_Instance = nullptr;
        return *this;
    }

    operator bool() noexcept { return m_Instance != nullptr; }
    operator bool() const noexcept { return m_Instance != nullptr; }
    bool operator==(const Ref<T>& other) noexcept { return m_Instance == other.m_Instance; }
    bool operator==(const Ref<T>& other) const noexcept { return m_Instance == other.m_Instance; }
    T* operator->() noexcept { return m_Instance; }
    const T* operator->() const noexcept { return m_Instance; }
    T& operator*() noexcept { return *m_Instance; }
    const T& operator*() const noexcept { return *m_Instance; }

    T* Raw() noexcept { return  m_Instance; }
    const T* Raw() const noexcept { return  m_Instance; }

    void Reset(T* instance = nullptr) noexcept
    {
        Decrement();
        m_Instance = instance;
    }

    template<typename T2>
    static constexpr bool IsConvertible() noexcept { return std::is_convertible_v<T2*, T*> || std::is_convertible_v<T*, T2*>; }
    template<typename T2>
    bool CanConvert() noexcept { return dynamic_cast<T2*>(m_Instance) != nullptr; }

    template<typename T2>
    Ref<T2> As() noexcept { return Ref<T2>(*this); }

    template<typename ...Args>
    static Ref<T> Create(Args&& ...args) noexcept
    {
        return Ref<T>(new T(std::forward<Args>(args)...));
    }
private:
    void Increment() const noexcept
    {
        if (m_Instance)
            m_Instance->IncrementRefCount();
    }
    void Decrement() const noexcept
    {
        if (m_Instance)
        {
            m_Instance->DecrementRefCount();
            if (m_Instance->GetRefCount() == 0u)
                delete m_Instance;
        }
    }
private:
    template<typename T2>
    friend class Ref;

    T* m_Instance;
};

} //namespace Saba

template<typename T>
using Ref = Saba::Ref<T>;
