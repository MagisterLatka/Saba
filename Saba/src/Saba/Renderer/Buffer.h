#pragma once

namespace Saba {

	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint ShaderDataTypeSize(ShaderDataType type)
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
		const char* Name;
		ShaderDataType Type;
		uint Size;
		uint Offset;
		bool Normalized;

		BufferElement() {}
		BufferElement(const char* name, ShaderDataType type, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{
		}

		uint GetComponentCount() const
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
		BufferLayout() {}
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements)
		{
			Calculate();
		}

		inline uint GetStride() const { return m_Stride; }
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
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
		uint m_Stride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static VertexBuffer* Create(float* data, uint size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint GetCount() const = 0;

		static IndexBuffer* Create(uint* data, uint count);
	};

}