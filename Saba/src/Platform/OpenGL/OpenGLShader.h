#pragma once

#include "Saba\Renderer\Shader.h"

namespace Saba {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const char* vertexSrc, const char* fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;
	private:
		void Init(const char* vertexSrc, const char* fragmentSrc);
	private:
		uint m_ID;
	};

}
