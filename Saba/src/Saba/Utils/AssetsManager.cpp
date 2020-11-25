#include "pch.h"
#include "Saba/Utils/AssetsManager.h"

#include <filesystem>

namespace Saba {

	std::unordered_map<std::string, std::pair<std::string, Ref<Shader>>> ShaderManager::s_Shaders;

	Ref<Shader> ShaderManager::Add(const std::string& name, const Ref<Shader>& shader)
	{
		SB_CORE_ASSERT(!Has(name), "Shader \"{0}\" already exist", name);
		s_Shaders[name] = { {}, shader };
		return shader;
	}
	std::optional<Ref<Shader>> ShaderManager::Add(const std::string& filepath)
	{
		if (filepath == "" || filepath.empty()) return {};
		auto a = filepath.find_last_of("\\/");
		auto b = filepath.find(".glsl", a);

		std::string name;
		if (b == -1) return {};
		else if (a == -1)
			name = std::string(&filepath[0], b);
		else
			name = std::string(&filepath[a + 1], b - a - 1);

		return Add(name, filepath);
	}
	std::optional<Ref<Shader>> ShaderManager::Add(const std::string& name, const std::string& filepath)
	{
		SB_CORE_ASSERT(!Has(name), "Shader \"{0}\" already exist", name);
		if (std::filesystem::is_regular_file(filepath))
		{
			s_Shaders[name] = { filepath, Shader::Create(filepath) };
			return s_Shaders[name].second;
		}
		else return {};
	}

	std::pair<Ref<Shader>, bool> ShaderManager::GetFromFilepath(const std::string& filepath)
	{
		if (filepath == "" || filepath.empty()) return {};
		auto a = filepath.find_last_of("\\/");
		auto b = filepath.find(".glsl");

		std::string name;
		if (b == -1) return {};
		else if (a == -1)
			name = std::string(&filepath[0], b);
		else
			name = std::string(&filepath[a + 1], b - a - 1);

		if (Has(name))
		{
			if (s_Shaders[name].first == filepath)
				return { s_Shaders[name].second, false };
			else return {};
		}
		else
		{
			auto value = Add(filepath);
			if (value)
				return { *value, true };
			else return {};
		}
	}

	std::string ShaderManager::GetName(const Ref<Shader>& shader)
	{
		for (auto [name, data] : s_Shaders)
		{
			if (shader == data.second) return name;
		}
		return {};
	}

	bool ShaderManager::Has(const std::string& name)
	{
		return s_Shaders.find(name) != s_Shaders.end();
	}



	std::unordered_map<std::string, std::pair<std::string, Ref<Texture2D>>> TextureManager::s_Textures2D;

	Ref<Texture2D> TextureManager::Add2D(const std::string& name, const Ref<Texture2D>& texture)
	{
		SB_CORE_ASSERT(!Has2D(name), "Texture \"{0}\" already exist", name);
		s_Textures2D[name] = { {}, texture };
		return texture;
	}

	std::optional<Ref<Texture2D>> TextureManager::Add2D(const std::string& filepath)
	{
		if (filepath == "" || filepath.empty() || !std::filesystem::is_regular_file(filepath)) return {};
		auto a = filepath.find_last_of("\\/");
		auto b = filepath.find_last_of('.');

		std::string name;
		if (b == -1) return {};
		else if (a == -1)
			name = std::string(&filepath[0], b);
		else
			name = std::string(&filepath[a + 1], b - a - 1);

		return Add2D(name, filepath);
	}
	std::optional<Ref<Texture2D>> TextureManager::Add2D(const std::string& name, const std::string& filepath)
	{
		SB_CORE_ASSERT(!Has2D(name), "Texture \"{0}\" already exist", name);
		if (std::filesystem::is_regular_file(filepath))
		{
			s_Textures2D[name] = { filepath, Texture2D::Create(filepath) };
			return s_Textures2D[name].second;
		}
		else return {};
	}
	std::optional<Ref<Texture2D>> TextureManager::Add2D(const TextureData& data)
	{
		if (!data.Filepath.empty())
		{
			if (!std::filesystem::is_regular_file(data.Filepath)) return {};
			auto a = data.Filepath.find_last_of("\\/");
			auto b = data.Filepath.find_last_of('.');

			std::string name;
			if (b == -1) return {};
			else if (a == -1)
				name = std::string(&data.Filepath[0], b);
			else
				name = std::string(&data.Filepath[a + 1], b - a - 1);

			return Add2D(name, data);
		}
		else
		{
			std::string name = std::to_string(s_Textures2D.size());
			s_Textures2D[name] = { {}, Texture2D::Create(data) };
			return s_Textures2D[name].second;
		}
	}
	std::optional<Ref<Texture2D>> TextureManager::Add2D(const std::string& name, const TextureData& data)
	{
		SB_CORE_ASSERT(!Has2D(name), "Texture \"{0}\" already exist", name);
		if (!data.Filepath.empty() && !std::filesystem::is_regular_file(data.Filepath)) return {};
		s_Textures2D[name] = { data.Filepath, Texture2D::Create(data) };
		return s_Textures2D[name].second;
	}

	std::pair<Ref<Texture2D>, bool> TextureManager::GetFromFilepath(const std::string& filepath)
	{
		if (filepath == "" || filepath.empty()) return {};
		auto a = filepath.find_last_of("\\/");
		auto b = filepath.find_last_of('.');
		
		std::string name;
		if (b == -1) return {};
		else if (a == -1)
			name = std::string(&filepath[0], b);
		else
			name = std::string(&filepath[a + 1], b - a - 1);

		if (Has2D(name))
		{
			if (s_Textures2D[name].first == filepath)
				return { s_Textures2D[name].second, false };
			else return {};
		}
		else
		{
			auto value = Add2D(filepath);
			if (value)
				return { *value, true };
			else return {};
		}
	}

	std::string TextureManager::GetName(const Ref<Texture2D>& texture)
	{
		for (auto [name, data] : s_Textures2D)
		{
			if (texture == data.second) return name;
		}
		return {};
	}

	bool TextureManager::Has2D(const std::string& name)
	{
		return s_Textures2D.find(name) != s_Textures2D.end();
	}
}
