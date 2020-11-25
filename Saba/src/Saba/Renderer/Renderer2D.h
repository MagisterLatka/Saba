#pragma once

#include "Saba/Renderer/Shader.h"
#include "Saba/Renderer/Texture.h"
#include "Saba/Renderer/Camera.h"

namespace Saba {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static Ref<Shader> GetCurrentShader();
		static void SetShader(Ref<Shader> shader = {});

		static void BeginScene(const Camera& camera, const glm::mat4& tranform = glm::mat4(1.0f));
		static void BeginScene(const glm::mat4& viewProjectionMatrix);
		static void Flush();

		static void DrawQuad(glm::vec2 pos, glm::vec2 size, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, const Ref<Texture2D>& texture = {}, float tillingFactor = 1.0f);
		static void DrawQuad(glm::vec3 pos, glm::vec2 size, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, const Ref<Texture2D>& texture = {}, float tillingFactor = 1.0f);
		static void DrawQuad(glm::mat4 transform, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, const Ref<Texture2D>& texture = {}, float tillingFactor = 1.0f);
		static void DrawRotatedQuad(glm::vec2 pos, glm::vec2 size, float angle = 0.0f, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, const Ref<Texture2D>& texture = {}, float tillingFactor = 1.0f);
		static void DrawRotatedQuad(glm::vec3 pos, glm::vec2 size, float angle = 0.0f, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, const Ref<Texture2D>& texture = {}, float tillingFactor = 1.0f);

		struct Stats
		{
			uint32_t quadCount = 0;
			uint32_t drawCalls = 0;
		};

		static void ResetStats();
		static const Stats& GetStats();
	};

}
