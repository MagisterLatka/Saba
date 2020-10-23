#include "pch.h"
#include "Saba/Renderer/AssetsManager.h"

namespace Saba {

	std::unordered_map<std::string, Ref<Shader>> ShaderManager::s_Shaders;

	Ref<Shader> ShaderManager::Add(const std::string& name, Ref<Shader> shader)
	{
		SB_CORE_ASSERT((s_Shaders.find(name) == s_Shaders.end()), "Shader \"{0}\" already exist", name.c_str());
		s_Shaders[name] = shader;
		return shader;
	}
	Ref<Shader> ShaderManager::Add(const std::string& filepath)
	{
		auto a = filepath.find_last_of("\\/");
		auto b = filepath.find(".glsl");
		std::string name(&filepath[a + 1], b - a - 1);
		SB_CORE_ASSERT(s_Shaders.find(name) == s_Shaders.end(), "Shader \"{0}\" already exist", name.c_str());
		s_Shaders[name] = Shader::Create(filepath);
		return s_Shaders[name];
	}

	Ref<Shader> ShaderManager::GetFromFilepath(const std::string& filepath)
	{
		auto a = filepath.find_last_of("\\/");
		auto b = filepath.find(".glsl");
		std::string name(&filepath[a + 1], b - a - 1);
		if (s_Shaders.find(name) != s_Shaders.end())
			return s_Shaders[name];
		else return Add(filepath);
	}



	std::unordered_map<std::string, Ref<Texture2D>> TextureManager::s_Textures2D;

	Ref<Texture2D> TextureManager::Add2D(const std::string& name, Ref<Texture2D> texture)
	{
		SB_CORE_ASSERT((s_Textures2D.find(name) == s_Textures2D.end()), "Texture \"{0}\" already exist", name.c_str());
		s_Textures2D[name] = texture;
		return texture;
	}
}
