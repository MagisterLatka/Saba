#pragma once

#include "Saba\Timestep.h"
#include "VertexArray.h"
#include "Shader.h"

namespace Saba {

	struct ParticleProps
	{
		glm::vec2 Position;
		glm::vec2 Velocity, VelocityVariation;
		glm::vec4 ColorBegin, ColorEnd;
		float SizeBegin, SizeEnd, SizeVariation;
		float LifeTime = 1.0f;
	};

	class ParticleSystem
	{
	public:
		ParticleSystem();

		void OnUpdate(Timestep ts);
		virtual void OnRender();

		void Emit(ParticleProps& props);
	protected:
		struct Particle
		{
			glm::vec2 Position;
			glm::vec2 Velocity;
			glm::vec4 ColorBegin, ColorEnd;
			float Rotation = 0.0f;
			float SizeBegin, SizeEnd;

			float LifeTime = 1.0f;
			float LifeCounter = 0.0f;

			bool Active = false;
		};
		std::vector<Particle> m_Particles;
		uint m_Index = 9999;

		Ref<VertexArray> m_VAO;
		Ref<Shader> m_Shader;
	};

}
