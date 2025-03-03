#pragma once

#include "Saba/Core/Core.h"
#include "Saba/Core/Buffer.h"

namespace Saba {

enum class BufferLayoutElementDataType : uint8_t {
    None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, UInt, UInt2, UInt3, UInt4
};
inline uint32_t BufferLayoutElementDataTypeSize(BufferLayoutElementDataType type) {
    switch (type) {
        case BufferLayoutElementDataType::Float:    return 1 * sizeof(float);
        case BufferLayoutElementDataType::Float2:   return 2 * sizeof(float);
        case BufferLayoutElementDataType::Float3:   return 3 * sizeof(float);
        case BufferLayoutElementDataType::Float4:   return 4 * sizeof(float);
        case BufferLayoutElementDataType::Mat3:     return 9 * sizeof(float);
        case BufferLayoutElementDataType::Mat4:     return 16 * sizeof(float);
        case BufferLayoutElementDataType::Int:      return 1 * sizeof(int);
        case BufferLayoutElementDataType::Int2:     return 2 * sizeof(int);
        case BufferLayoutElementDataType::Int3:     return 3 * sizeof(int);
        case BufferLayoutElementDataType::Int4:     return 4 * sizeof(int);
        case BufferLayoutElementDataType::UInt:     return 1 * sizeof(uint32_t);
        case BufferLayoutElementDataType::UInt2:    return 2 * sizeof(uint32_t);
        case BufferLayoutElementDataType::UInt3:    return 3 * sizeof(uint32_t);
        case BufferLayoutElementDataType::UInt4:    return 4 * sizeof(uint32_t);
        default: break;
    }

    SB_CORE_THROW_INFO("Unknown buffer layout element data type");
    return 0;
}
struct BufferLayoutElement {
    std::string Name;
    uint32_t Size = 0u, Offset = 0u, InstanceStepRate = 0u;
    BufferLayoutElementDataType Type = BufferLayoutElementDataType::None;
    bool Normalized = false;

    BufferLayoutElement() = default;
    BufferLayoutElement(std::string name, BufferLayoutElementDataType type, bool normalized = false, uint32_t instanceStepRate = 0u)
        : Name(std::move(name)), Size(BufferLayoutElementDataTypeSize(type)), InstanceStepRate(instanceStepRate), Type(type), Normalized(normalized) {}

    uint32_t GetComponentCount() const {
        switch (Type) {
            case BufferLayoutElementDataType::Float:    return 1;
            case BufferLayoutElementDataType::Float2:   return 2;
            case BufferLayoutElementDataType::Float3:   return 3;
            case BufferLayoutElementDataType::Float4:   return 4;
            case BufferLayoutElementDataType::Mat3:     return 9;
            case BufferLayoutElementDataType::Mat4:     return 16;
            case BufferLayoutElementDataType::Int:      return 1;
            case BufferLayoutElementDataType::Int2:     return 2;
            case BufferLayoutElementDataType::Int3:     return 3;
            case BufferLayoutElementDataType::Int4:     return 4;
            case BufferLayoutElementDataType::UInt:     return 1;
            case BufferLayoutElementDataType::UInt2:    return 2;
            case BufferLayoutElementDataType::UInt3:    return 3;
            case BufferLayoutElementDataType::UInt4:    return 4;
            default: break;
        }

        SB_CORE_THROW_INFO("Unknown buffer layout element data type");
        return 0;
    }
};
class BufferLayout {
public:
    using BufferLayoutElements = std::vector<BufferLayoutElement>;

    BufferLayout() = default;
    BufferLayout(const std::initializer_list<BufferLayoutElement>& elements)
        : m_Elements(elements) { Calculate(); }

    uint32_t GetStride() const { return m_Stride; }
    const BufferLayoutElements& GetElements() const { return m_Elements; }

    void AddElement(BufferLayoutElement element) {
        m_Elements.push_back(std::move(element));
        auto& e = m_Elements.back();
        e.Offset = m_Stride;
        m_Stride += e.Size;
    }

    BufferLayoutElements::const_iterator begin() const { return m_Elements.begin(); }
    BufferLayoutElements::const_iterator end() const { return m_Elements.end(); }
private:
    void Calculate() {
        m_Stride = 0u;
        for (auto& element : m_Elements) {
            element.Offset = m_Stride;
            m_Stride += element.Size;
        }
    }
private:
    BufferLayoutElements m_Elements;
    uint32_t m_Stride;
};
enum class BufferUsage : uint8_t { None = 0, Default, Dynamic, Immutable };

class VertexBuffer : public RefCounted {
public:
    SB_CORE virtual ~VertexBuffer() = default;

    SB_CORE virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0u) = 0;
    SB_CORE virtual void SetData(const Buffer& buffer, uint32_t offset = 0u) = 0;
    SB_CORE virtual void SetData(Buffer&& buffer, uint32_t offset = 0u) = 0;

    SB_CORE virtual const BufferLayout& GetLayout() const noexcept = 0;
    SB_CORE virtual uint32_t GetSize() const noexcept = 0;

    SB_CORE static Ref<VertexBuffer> Create(BufferLayout layout, const void* data, uint32_t size, BufferUsage usage = BufferUsage::Default);
    SB_CORE static Ref<VertexBuffer> Create(BufferLayout layout, const Buffer& buffer, BufferUsage usage = BufferUsage::Default);
    SB_CORE static Ref<VertexBuffer> Create(BufferLayout layout, Buffer&& buffer, BufferUsage usage = BufferUsage::Default);
};
class IndexBuffer : public RefCounted {
public:
    SB_CORE virtual ~IndexBuffer() = default;

    SB_CORE virtual void SetData(const uint32_t* data, uint32_t size, uint32_t offset = 0u) = 0;
    SB_CORE virtual void SetData(const Buffer& buffer, uint32_t offset = 0u) = 0;
    SB_CORE virtual void SetData(Buffer&& buffer, uint32_t offset = 0u) = 0;

    SB_CORE virtual uint32_t GetSize() const noexcept = 0;
    SB_CORE virtual uint32_t GetIndicesCount() const noexcept = 0;

    SB_CORE static Ref<IndexBuffer> Create(const uint32_t* data, uint32_t size, BufferUsage usage = BufferUsage::Default);
    SB_CORE static Ref<IndexBuffer> Create(const Buffer& buffer, BufferUsage usage = BufferUsage::Default);
    SB_CORE static Ref<IndexBuffer> Create(Buffer&& buffer, BufferUsage usage = BufferUsage::Default);
};

enum class UniformType : uint8_t {
    None = 0,
    Float, Float2, Float3, Float4,
    Int, Int2, Int3, Int4,
    Mat3x3, Mat4x4
};
struct UniformDecl {
    std::string Name;
    uint32_t Offset;
    UniformType Type;
};
struct UniformBufferBase {
    virtual ~UniformBufferBase() = default;
    virtual const uint8_t* GetBuffer() const noexcept = 0;
    virtual uint32_t GetBufferSize() const noexcept = 0;
    virtual const UniformDecl* GetUniforms() const noexcept = 0;
    virtual uint32_t GetUniformCount() const noexcept = 0;
};
template<uint32_t N, uint32_t U>
struct UniformBuffer : public UniformBufferBase {
    static constexpr uint32_t c_Alignment = 16u;    
    static_assert(N % c_Alignment == 0, "Uniform buffer size must be a multiple of 16 bytes");

    UniformDecl Uniforms[U];
    uint8_t Buffer[N];
    uint32_t Offset = 0u, Index = 0u;

    const uint8_t* GetBuffer() const noexcept override { return Buffer; }
    uint32_t GetBufferSize() const noexcept override { return N; }
    const UniformDecl* GetUniforms() const noexcept override { return Uniforms; }
    uint32_t GetUniformCount() const noexcept override { return U; }

    void Push(std::string name, const float& data) {
        SB_CORE_ASSERT(Index < U && Offset + sizeof(float) <= N, "Uniform buffer overflow");
        Uniforms[Index++] = { std::move(name), Offset, UniformType::Float };
        memcpy(Buffer + Offset, &data, sizeof(float));
        Offset += sizeof(float);
    }
    void Push(std::string name, const glm::vec2& data) {
        if (Offset % c_Alignment > 2 * sizeof(float))
            Offset = (Offset / c_Alignment + 1) * c_Alignment;

        SB_CORE_ASSERT(Index < U && Offset + sizeof(glm::vec2) <= N, "Uniform buffer overflow");
        Uniforms[Index++] = { std::move(name), Offset, UniformType::Float2 };
        memcpy(Buffer + Offset, glm::value_ptr(data), sizeof(glm::vec2));
        Offset += sizeof(glm::vec2);
    }
    void Push(std::string name, const glm::vec3& data) {
        if (Offset % c_Alignment > sizeof(float))
            Offset = (Offset / c_Alignment + 1) * c_Alignment;

        SB_CORE_ASSERT(Index < U && Offset + sizeof(glm::vec3) <= N, "Uniform buffer overflow");
        Uniforms[Index++] = { std::move(name), Offset, UniformType::Float3 };
        memcpy(Buffer + Offset, glm::value_ptr(data), sizeof(glm::vec3));
        Offset += sizeof(glm::vec3);
    }
    void Push(std::string name, const glm::vec4& data) {
        if (Offset % c_Alignment != 0u)
            Offset = (Offset / c_Alignment + 1) * c_Alignment;

        SB_CORE_ASSERT(Index < U && Offset + sizeof(glm::vec4) <= N, "Uniform buffer overflow");
        Uniforms[Index++] = { std::move(name), Offset, UniformType::Float4 };
        memcpy(Buffer + Offset, glm::value_ptr(data), sizeof(glm::vec4));
        Offset += sizeof(glm::vec4);
    }
    void Push(std::string name, const int& data) {
        SB_CORE_ASSERT(Index < U && Offset + sizeof(int) <= N, "Uniform buffer overflow");
        Uniforms[Index++] = { std::move(name), Offset, UniformType::Int };
        memcpy(Buffer + Offset, &data, sizeof(int));
        Offset += sizeof(int);
    }
    void Push(std::string name, const glm::ivec2& data) {
        if (Offset % c_Alignment > 2 * sizeof(int))
            Offset = (Offset / c_Alignment + 1) * c_Alignment;

        SB_CORE_ASSERT(Index < U && Offset + sizeof(glm::ivec2) <= N, "Uniform buffer overflow");
        Uniforms[Index++] = { std::move(name), Offset, UniformType::Int2 };
        memcpy(Buffer + Offset, glm::value_ptr(data), sizeof(glm::ivec2));
        Offset += sizeof(glm::ivec2);
    }
    void Push(std::string name, const glm::ivec3& data) {
        if (Offset % c_Alignment > sizeof(int))
            Offset = (Offset / c_Alignment + 1) * c_Alignment;

        SB_CORE_ASSERT(Index < U && Offset + sizeof(glm::ivec3) <= N, "Uniform buffer overflow");
        Uniforms[Index++] = { std::move(name), Offset, UniformType::Int3 };
        memcpy(Buffer + Offset, glm::value_ptr(data), sizeof(glm::ivec3));
        Offset += sizeof(glm::ivec3);
    }
    void Push(std::string name, const glm::ivec4& data) {
        if (Offset % c_Alignment != 0)
            Offset = (Offset / c_Alignment + 1) * c_Alignment;

        SB_CORE_ASSERT(Index < U && Offset + sizeof(glm::ivec4) <= N, "Uniform buffer overflow");
        Uniforms[Index++] = { std::move(name), Offset, UniformType::Int4 };
        memcpy(Buffer + Offset, glm::value_ptr(data), sizeof(glm::ivec4));
        Offset += sizeof(glm::ivec4);
    }
    void Push(std::string name, const glm::mat3& data) {
        if (Offset % c_Alignment != 0)
            Offset = (Offset / c_Alignment + 1) * c_Alignment;

        SB_CORE_ASSERT(Index < U && Offset + sizeof(glm::mat3) <= N, "Uniform buffer overflow");
        Uniforms[Index++] = { std::move(name), Offset, UniformType::Mat3x3 };
        memcpy(Buffer + Offset, glm::value_ptr(data), sizeof(glm::mat3));
        Offset += sizeof(glm::mat3);
    }
    void Push(std::string name, const glm::mat4& data) {
        if (Offset % c_Alignment != 0)
            Offset = (Offset / c_Alignment + 1) * c_Alignment;

        SB_CORE_ASSERT(Index < U && Offset + sizeof(glm::mat4) <= N, "Uniform buffer overflow");
        Uniforms[Index++] = { std::move(name), Offset, UniformType::Mat4x4 };
        memcpy(Buffer + Offset, glm::value_ptr(data), sizeof(glm::mat4));
        Offset += sizeof(glm::mat4);
    }
};
enum class BufferShaderBinding : uint8_t { None = 0, Vertex, Fragment };

class ConstantBuffer : public RefCounted {
public:
    SB_CORE virtual ~ConstantBuffer() = default;

    SB_CORE virtual void SetData(const void* data, uint32_t size) = 0;
    SB_CORE virtual void SetData(const Buffer& buffer) = 0;
    SB_CORE virtual void SetData(Buffer&& buffer) = 0;
    SB_CORE virtual void SetData(const UniformBufferBase& buffer) = 0;

    SB_CORE virtual void Bind(uint32_t slot = 0) const = 0;

    SB_CORE virtual uint32_t GetSize() const noexcept = 0;

    SB_CORE static Ref<ConstantBuffer> Create(BufferShaderBinding binding, const void* data, uint32_t size);
    SB_CORE static Ref<ConstantBuffer> Create(BufferShaderBinding binding, const Buffer& buffer);
    SB_CORE static Ref<ConstantBuffer> Create(BufferShaderBinding binding, Buffer&& buffer);
    SB_CORE static Ref<ConstantBuffer> Create(BufferShaderBinding binding, const UniformBufferBase& buffer);
};

}
