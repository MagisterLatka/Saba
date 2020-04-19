#pragma once

#include "Saba\Renderer\Shader.h"
#include "Saba\Renderer\Texture.h"

namespace Saba {

	class ShaderManager
	{
	public:
		static void Add(const std::string& name, Ref<Shader> shader);
		static void Add(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		static void Add(const std::string& filepath);
		static void Add(const std::string& name, const std::string& filepath);

		static inline Ref<Shader> Get(const std::string& name) { SB_CORE_ASSERT((s_Shaders.find(name) != s_Shaders.end()), "Shader \"{0}\" does not exist", name.c_str());
																	return s_Shaders[name]; }
	private:
		static std::unordered_map<std::string, Ref<Shader>> s_Shaders;
	};

	class TextureManager
	{
	public:
		static void Add2D(const std::string& name, Ref<Texture2D> texture);
		static void Add2D(const std::string& name, const std::string& filepath);
		static void Add2D(const std::string& name, int width, int height);

		static inline Ref<Texture2D> Get2D(const std::string& name) { SB_CORE_ASSERT((s_Textures2D.find(name) != s_Textures2D.end()), "Texture \"{0}\" does not exist", name.c_str());
																		return s_Textures2D[name]; }
	private:
		static std::unordered_map<std::string, Ref<Texture2D>> s_Textures2D;
	};
}
