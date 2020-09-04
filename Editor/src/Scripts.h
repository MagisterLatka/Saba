#pragma once

namespace Saba {

	class CameraController : public ScriptableEntity
	{
	public:
		virtual void OnEvent(Event& event) override
		{
			Dispatcher dispatcher(event);
			dispatcher.Dispatch<MouseScrolledEvent>(SB_BIND_EVENT_FUNC(CameraController::OnScrollEvent));
		}
		virtual void OnUpdate(Timestep ts) override
		{
			if (GetScene()->IsViewportFocused())
			{
				auto& transform = GetComponent<TransformComponent>().Transform;

				if (p_RotationEnabled)
				{
					float rotation = 0.0f;
					if (Input::IsKeyPressed(KeyCode::Q))
						rotation += m_CameraRotationSpeed * (float)ts;
					if (Input::IsKeyPressed(KeyCode::E))
						rotation -= m_CameraRotationSpeed * (float)ts;

					if (rotation != 0.0f)
					{
						transform = glm::rotate(transform, glm::radians(rotation), { 0.0f, 0.0f, 1.0f });
					}
				}

				glm::vec2 translate(0.0f);
				if (Input::IsKeyPressed(KeyCode::W))
					translate.y += m_CameraMovementSpeed * (float)ts;
				if (Input::IsKeyPressed(KeyCode::S))
					translate.y -= m_CameraMovementSpeed * (float)ts;
				if (Input::IsKeyPressed(KeyCode::A))
					translate.x -= m_CameraMovementSpeed * (float)ts;
				if (Input::IsKeyPressed(KeyCode::D))
					translate.x += m_CameraMovementSpeed * (float)ts;
				if (translate != glm::vec2(0.0f))
				{
					transform = glm::translate(transform, glm::vec3(translate, 0.0f));
				}
			}
		}
	private:
		bool OnScrollEvent(MouseScrolledEvent& event)
		{
			m_Zoom -= event.GetYOffset() * 0.5f;
			m_Zoom = glm::max(m_Zoom, 0.5f);
			GetComponent<CameraComponent>().Camera.SetOrthographicSize(m_Zoom);
			m_CameraMovementSpeed = m_Zoom * 0.5f;
			return false;
		}
	public:
		bool p_RotationEnabled = true;
	private:
		float m_Zoom = 10.0f;

		float m_CameraMovementSpeed = 5.0f;
		float m_CameraRotationSpeed = 90.0f;
	};

	class ParticleSystemController : public ScriptableEntity
	{
	public:
		ParticleSystemController()
		{
			p_Particle.ColorBegin = glm::vec4(0.8f, 0.3f, 0.3f, 1.0f);
			p_Particle.ColorEnd = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			p_Particle.Position = glm::vec3(0.0f, 0.0f, 0.0f);
			p_Particle.SizeBegin = 0.5f;
			p_Particle.SizeEnd = 0.1f;
			p_Particle.SizeVariation = 0.05f;
			p_Particle.Velocity = glm::vec2(0.0f, 0.0f);
			p_Particle.VelocityVariation = glm::vec2(3.0f, 3.0f);
		}

		virtual void OnUpdate(Timestep ts) override
		{
			if (!p_ParticleSystem)
				return;

			if (p_EnableParticles)
			{
				if (Input::IsMouseButtonPressed(MouseCode::Button0) && GetScene()->IsViewportHovered())
				{
					auto [x, y] = Input::GetMousePos();
					auto& camera = p_Camera.GetComponent<CameraComponent>().Camera;
					auto viewportPos = GetScene()->GetViewportPos();
					auto viewportSize = GetScene()->GetViewportSize();

					glm::vec2 bounds = { camera.GetWidth(), camera.GetHeight() };
					x = (x - viewportPos.x) / viewportSize.x * bounds.x - bounds.x * 0.5f;
					y = bounds.y * 0.5f - (y - viewportPos.y) / viewportSize.y * bounds.y;

					p_Particle.Position = { x, y, 0.1f };
					p_Particle.Position = p_Camera.GetComponent<TransformComponent>().Transform * glm::vec4(p_Particle.Position, 1.0f);
					for (int i = 0; i < 1000 * ts; i++)
						p_ParticleSystem->Emit(p_Particle);
				}
			}
		}
	public:
		ParticleSystem* p_ParticleSystem = nullptr;
		ParticleProps p_Particle;
		bool p_EnableParticles = false;

		Entity p_Camera;
	};
}
