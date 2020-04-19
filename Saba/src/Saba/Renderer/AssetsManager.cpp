#include "pch.h"
#include "AssetsManager.h"

namespace Saba {

	std::unordered_map<std::string, Ref<Shader>> ShaderManager::s_Shaders;

	void ShaderManager::Add(const std::string& name, Ref<Shader> shader)
	{
		SB_CORE_ASSERT((s_Shaders.find(name) == s_Shaders.end()), "Shader \"{0}\" already exist", name.c_str());
		s_Shaders[name] = shader;
	}
	void ShaderManager::Add(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		SB_CORE_ASSERT((s_Shaders.find(name) == s_Shaders.end()), "Shader \"{0}\" already exist", name.c_str());
		s_Shaders[name] = Shader::Create(vertexSrc, fragmentSrc);
	}
	void ShaderManager::Add(const std::string& filepath)
	{
		auto a = filepath.rfind('/');
		auto b = filepath.find(".glsl");
		std::string name(&filepath[a + 1], b - a - 1);
		SB_CORE_ASSERT((s_Shaders.find(name) == s_Shaders.end()), "Shader \"{0}\" already exist", name.c_str());
		s_Shaders[name] = Shader::Create(filepath);
	}
	void ShaderManager::Add(const std::string& name, const std::string& filepath)
	{
		SB_CORE_ASSERT((s_Shaders.find(name) == s_Shaders.end()), "Shader \"{0}\" already exist", name.c_str());
		s_Shaders[name] = Shader::Create(filepath);
	}



	std::unordered_map<std::string, Ref<Texture2D>> TextureManager::s_Textures2D;

	void TextureManager::Add2D(const std::string& name, Ref<Texture2D> texture)
	{
		SB_CORE_ASSERT((s_Textures2D.find(name) == s_Textures2D.end()), "Texture \"{0}\" already exist", name.c_str());
		s_Textures2D[name] = texture;
	}
	void TextureManager::Add2D(const std::string& name, const std::string& filepath)
	{
		SB_CORE_ASSERT((s_Textures2D.find(name) == s_Textures2D.end()), "Texture \"{0}\" already exist", name.c_str());
		s_Textures2D[name] = Texture2D::Create(filepath);
	}
	void TextureManager::Add2D(const std::string& name, int width, int height)
	{
		SB_CORE_ASSERT((s_Textures2D.find(name) == s_Textures2D.end()), "Texture \"{0}\" already exist", name.c_str());
		s_Textures2D[name] = Texture2D::Create(width, height);
	}
}
