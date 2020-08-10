#pragma once

#include "Saba/Core.h"

namespace Saba {

	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static unsigned int ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case Saba::ShaderDataType::Float:	return 4;
			case Saba::ShaderDataType::Float2:	return 4 * 2;
			case Saba::ShaderDataType::Float3:	return 4 * 3;
			case Saba::ShaderDataType::Float4:	return 4 * 4;
			case Saba::ShaderDataType::Mat3:	return 4 * 3 * 3;
			case Saba::ShaderDataType::Mat4:	return 4 * 4 * 4;
			case Saba::ShaderDataType::Int:		return 4;
			case Saba::ShaderDataType::Int2:	return 4 * 2;
			case Saba::ShaderDataType::Int3:	return 4 * 3;
			case Saba::ShaderDataType::Int4:	return 4 * 4;
			case Saba::ShaderDataType::Bool:	return 1;
		}

		SB_CORE_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		size_t Offset;
		bool Normalized;

		BufferElement() = default;
		BufferElement(const std::string& name, ShaderDataType type, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case Saba::ShaderDataType::Float:	return 1;
				case Saba::ShaderDataType::Float2:	return 2;
				case Saba::ShaderDataType::Float3:	return 3;
				case Saba::ShaderDataType::Float4:	return 4;
				case Saba::ShaderDataType::Mat3:	return 3 * 3;
				case Saba::ShaderDataType::Mat4:	return 4 * 4;
				case Saba::ShaderDataType::Int:		return 1;
				case Saba::ShaderDataType::Int2:	return 2;
				case Saba::ShaderDataType::Int3:	return 3;
				case Saba::ShaderDataType::Int4:	return 4;
				case Saba::ShaderDataType::Bool:	return 1;
			}

			SB_CORE_ASSERT(false, "Unknown ShaderDataType");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements)
		{
			Calculate();
		}

		unsigned int GetStride() const { return m_Stride; }
		const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin()				{ return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end()					{ return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const	{ return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const		{ return m_Elements.end(); }
	private:
		void Calculate()
		{
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = m_Stride;
				m_Stride += element.Size;
			}
		}
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	enum BufferUsage
	{
		None = 0, Static, Dynamic, Stream
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void SetData(void* data, uint32_t size, uint32_t offset = 0) = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static Ref<VertexBuffer> Create(void* data, uint32_t size, BufferUsage usage = BufferUsage::Static);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void SetData(uint32_t* data, uint32_t count, uint32_t offset = 0) = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32_t* data, uint32_t count);
	};

}
