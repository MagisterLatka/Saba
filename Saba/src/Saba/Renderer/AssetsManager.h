#pragma once

#include "Saba/Core.h"
#include "Saba/Renderer/Shader.h"
#include "Saba/Renderer/Texture.h"

namespace Saba {

	class ShaderManager
	{
	public:
		static Ref<Shader> Add(const std::string& name, Ref<Shader> shader);
		static Ref<Shader> Add(const std::string& filepath);
		template<typename ...Args>
		static Ref<Shader> Add(const std::string& name, Args&&... args)
		{
			SB_CORE_ASSERT((s_Shaders.find(name) == s_Shaders.end()), "Shader \"{0}\" already exist", name.c_str());
			s_Shaders[name] = Shader::Create(args...);
			return s_Shaders[name];
		}

		static Ref<Shader> Get(const std::string& name) { SB_CORE_ASSERT((s_Shaders.find(name) != s_Shaders.end()), "Shader \"{0}\" does not exist", name.c_str()); return s_Shaders[name]; }
	private:
		static std::unordered_map<std::string, Ref<Shader>> s_Shaders;
	};

	class TextureManager
	{
	public:
		static Ref<Texture2D> Add2D(const std::string& name, Ref<Texture2D> texture);
		template<typename ...Args>
		static Ref<Texture2D> Add2D(const std::string& name, Args&& ...args)
		{
			SB_CORE_ASSERT((s_Textures2D.find(name) == s_Textures2D.end()), "Texture \"{0}\" already exist", name.c_str());
			s_Textures2D[name] = Texture2D::Create(args...);
			return s_Textures2D[name];
		}

		static Ref<Texture2D> Get2D(const std::string& name) { SB_CORE_ASSERT((s_Textures2D.find(name) != s_Textures2D.end()), "Texture \"{0}\" does not exist", name.c_str()); return s_Textures2D[name]; }
	private:
		static std::unordered_map<std::string, Ref<Texture2D>> s_Textures2D;
	};
}

