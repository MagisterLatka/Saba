#pragma once

namespace Saba {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Shader* Create(const char* vertexSrc, const char* fragmentSrc);
	};

}
