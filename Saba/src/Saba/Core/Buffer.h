#pragma once

#include "Saba/Core/Core.h"

namespace Saba {

struct SB_CORE Buffer {
    enum class Allocator { None = 0, New, Malloc };
private:
    bool m_Delete = false;
    Allocator m_Allocator = Allocator::None;
public:
    void *Data = nullptr;
    uint32_t Size = 0u;

    Buffer() = default;
    Buffer(void *data, uint32_t size, bool takeControl = false, Allocator allocator = Allocator::New)
      : Data(data), Size(size), m_Delete(takeControl), m_Allocator(allocator) {
        SB_CORE_ASSERT(!takeControl || allocator != Allocator::None, "Saba::Buffer expects an allocator to be specified");
    }
    Buffer(const Buffer &other)
        : m_Delete(true), m_Allocator(other.m_Allocator), Size(other.Size) {
        if (m_Allocator == Allocator::New)
            Data = new uint8_t[Size];
        else if (m_Allocator == Allocator::Malloc)
            Data = malloc(Size);
        else return;

        memcpy(Data, other.Data, Size);
    }
    Buffer(Buffer &&other) noexcept
        : Size(other.Size), m_Delete(true), m_Allocator(other.m_Allocator) {
        Data = other.Data;
        other.Data = nullptr;
        other.Size = 0;
        other.m_Allocator = Allocator::None;
    }
    ~Buffer() {
        if (m_Delete) {
            switch (m_Allocator) {
                case Allocator::New:
                    delete[] (uint8_t*)Data;
                    break;
                case Allocator::Malloc:
                    free(Data);
                    break;
            }
        }
    }

    Buffer& operator=(const Buffer &other) {
        if (m_Delete && Data) {
            switch (m_Allocator) {
                case Allocator::None:
                default:
                    SB_CORE_THROW_INFO("Invalid allocator");
                case Allocator::New:
                    delete[] (uint8_t*)Data;
                    break;
                case Allocator::Malloc:
                    free(Data);
                    break;
            }
        }
        m_Delete = true;
        m_Allocator = other.m_Allocator;
        Size = other.Size;

        switch (m_Allocator) {
            case Saba::Buffer::Allocator::None:
                SB_CORE_THROW_INFO("Invalid allocator");
                m_Allocator = Allocator::New;
                Data = new uint8_t[Size];
                break;
            case Saba::Buffer::Allocator::New:
                Data = new uint8_t[Size];
                break;
            case Saba::Buffer::Allocator::Malloc:
                Data = malloc(Size);
                break;
        }

        memcpy(Data, other.Data, Size);
        return *this;
    }
    Buffer& operator=(Buffer &&other) noexcept {
        if (m_Delete && Data) {
            switch (m_Allocator) {
                case Allocator::None:
                    break;
                case Allocator::New:
                    delete[] (uint8_t*)Data;
                    break;
                case Allocator::Malloc:
                    free(Data);
                    break;
            }
        }

        m_Delete = other.m_Delete;
        m_Allocator = other.m_Allocator;
        Size = other.Size;
        Data = other.Data;
        other.m_Delete = false;
        other.m_Allocator = Allocator::None;
        other.Size = 0;
        other.Data = nullptr;
        return *this;
    }

    static Buffer Copy(const void *data, uint32_t size, Allocator allocator = Allocator::New) {
        Buffer buffer;
        buffer.Allocate(size, allocator);
        memcpy(buffer.Data, data, size);
        return buffer;
    }

    void Allocate(uint32_t size, Allocator newAllocator = Allocator::None) {
        if (size > Size) {
            void *oldData = Data;
            switch (newAllocator) {
                case Saba::Buffer::Allocator::None: {
                    if (m_Allocator == Allocator::New)
                        Data = new uint8_t[size];
                    else if (m_Allocator == Allocator::Malloc)
                        Data = malloc(size);
                    else
                        Data = new uint8_t[size];
                    break;
                }
                case Saba::Buffer::Allocator::New:
                    Data = new uint8_t[size];
                    break;
                case Saba::Buffer::Allocator::Malloc:
                    Data = malloc(size);
                    break;
            }

            if (oldData) memcpy(Data, oldData, Size);
            Size = size;
            m_Delete = true;

            if (m_Delete && oldData) {
                switch (m_Allocator) {
                    case Allocator::None:
                    default:
                        SB_CORE_THROW_INFO("Invalid allocator");
                        delete[] (uint8_t*)Data;
                        break;
                    case Allocator::New:
                        delete[] (uint8_t*)Data;
                        break;
                    case Allocator::Malloc:
                        free(Data);
                        break;
                }
            }

            if (newAllocator != Allocator::None)
                m_Allocator = newAllocator;
        }
    }

    void Zero() noexcept {
        if (Data) memset(Data, 0, Size);
    }

    template<typename T>
    T& Read(uint32_t offset = 0) {
        SB_CORE_ASSERT(offset + sizeof(T) <= Size, "Accesing data out of buffer");
        return *(T*)(Data + offset);
    }

    void Write(void *data, uint32_t size, uint32_t offset = 0) {
        SB_CORE_ASSERT(Data, "Buffer has no memory allocated");
        SB_CORE_ASSERT(size + offset <= Size, "Accesing data out of buffer");
        memcpy((uint8_t *)Data + offset, data, size);
    }

    operator bool() noexcept { return Data; }
    template<typename T>
    T& operator[](uint32_t index) {
        SB_CORE_ASSERT(index * sizeof(T) < Size);
        return ((T *)Data)[index];
    }
    template<typename T>
    T operator[](uint32_t index) const {
        SB_CORE_ASSERT(index * sizeof(T) < Size);
        return ((T *)Data)[index];
    }

    template<typename T>
    T* As() noexcept { return (T *)Data; }

    uint32_t GetSize() const noexcept { return Size; }
};

} // namespace Saba
