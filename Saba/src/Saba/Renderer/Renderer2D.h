#pragma once

#include "Texture.h"
#include "Camera\OrthographicCamera.h"

namespace Saba {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void Flush();

		static void DrawQuad(glm::vec2 pos, glm::vec2 size, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, Ref<Texture2D> texture = {}, float tillingFactor = 1.0f);
		static void DrawQuad(glm::vec3 pos, glm::vec2 size, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, Ref<Texture2D> texture = {}, float tillingFactor = 1.0f);
		static void DrawRotatedQuad(glm::vec2 pos, glm::vec2 size, float angleD = 0.0f, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, Ref<Texture2D> texture = {}, float tillingFactor = 1.0f);
		static void DrawRotatedQuad(glm::vec3 pos, glm::vec2 size, float angleD = 0.0f, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, Ref<Texture2D> texture = {}, float tillingFactor = 1.0f);

		struct Stats
		{
			uint32_t quadCount = 0;
			uint32_t drawCalls = 0;
		};

		static void ResetStats();
		static const Stats& GetStats();
	};

}
