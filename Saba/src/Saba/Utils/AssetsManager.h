#pragma once

#include "Saba/Core.h"
#include "Saba/Renderer/Shader.h"
#include "Saba/Renderer/Texture.h"

namespace Saba {

	class ShaderManager
	{
	public:
		static Ref<Shader> Add(const std::string& name, const Ref<Shader>& shader);
		static std::optional<Ref<Shader>> Add(const std::string& filepath);
		static std::optional<Ref<Shader>> Add(const std::string& name, const std::string& filepath);
		template<typename... Args>
		static Ref<Shader> Add(const std::string& name, Args&&... args)
		{
			SB_CORE_ASSERT(!Has(name), "Shader \"{0}\" already exist", name.c_str());
			s_Shaders[name] = { std::string(), Shader::Create(args...) };
			return s_Shaders[name].second;
		}

		static Ref<Shader> Get(const std::string& name) { SB_CORE_ASSERT(Has(name), "Shader \"{0}\" does not exist", name.c_str()); return s_Shaders[name].second; }
		static std::pair<Ref<Shader>, bool> GetFromFilepath(const std::string& filepath);

		static std::string GetName(const Ref<Shader>& shader);

		static bool Has(const std::string& name);
	private:
		static std::unordered_map<std::string, std::pair<std::string, Ref<Shader>>> s_Shaders;
	};

	class TextureManager
	{
	public:
		static Ref<Texture2D> Add2D(const std::string& name, const Ref<Texture2D>& texture);
		static std::optional<Ref<Texture2D>> Add2D(const std::string& filepath);
		static std::optional<Ref<Texture2D>> Add2D(const std::string& name, const std::string& filepath);
		static std::optional<Ref<Texture2D>> Add2D(const TextureData& data);
		static std::optional<Ref<Texture2D>> Add2D(const std::string& name, const TextureData& data);
		template<typename... Args>
		static Ref<Texture2D> Add2D(const std::string& name, Args&& ...args)
		{
			SB_CORE_ASSERT(!Has2D(name), "Texture \"{0}\" already exist", name);
			s_Textures2D[name] = { std::string(), Texture2D::Create(args...) };
			return s_Textures2D[name].second;
		}

		static Ref<Texture2D> Get2D(const std::string& name) { SB_CORE_ASSERT(Has2D(name), "Texture \"{0}\" does not exist", name); return s_Textures2D[name].second; }
		static std::pair<Ref<Texture2D>, bool> GetFromFilepath(const std::string& filepath);

		static std::string GetName(const Ref<Texture2D>& texture);

		static bool Has2D(const std::string& name);
	private:
		static std::unordered_map<std::string, std::pair<std::string, Ref<Texture2D>>> s_Textures2D;
	};
}
