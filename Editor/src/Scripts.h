#pragma once

namespace Saba {

	class OrthographicCameraController : public ScriptableEntity
	{
	public:
		virtual void OnEvent(Event& event) override
		{
			Dispatcher dispatcher(event);
			dispatcher.Dispatch<MouseScrolledEvent>(SB_BIND_EVENT_FUNC(OrthographicCameraController::OnScrollEvent));
		}
		virtual void OnUpdate(Timestep ts) override
		{
			if (GetScene()->IsViewportFocused())
			{
				auto& tc = GetComponent<TransformComponent>();

				if (p_RotationEnabled)
				{
					float& rotation = tc.EulerAngles.z;
					if (Input::IsKeyPressed(KeyCode::Q))
						rotation += m_CameraRotationSpeed * (float)ts;
					if (Input::IsKeyPressed(KeyCode::E))
						rotation -= m_CameraRotationSpeed * (float)ts;
				}

				glm::vec3& translate = tc.Pos;
				if (Input::IsKeyPressed(KeyCode::W))
					translate.y += m_CameraMovementSpeed * (float)ts;
				if (Input::IsKeyPressed(KeyCode::S))
					translate.y -= m_CameraMovementSpeed * (float)ts;
				if (Input::IsKeyPressed(KeyCode::A))
					translate.x -= m_CameraMovementSpeed * (float)ts;
				if (Input::IsKeyPressed(KeyCode::D))
					translate.x += m_CameraMovementSpeed * (float)ts;
			}
		}
	private:
		bool OnScrollEvent(MouseScrolledEvent& event)
		{
			auto& camera = GetComponent<CameraComponent>().Camera;
			float zoom = camera.GetOrthographicSize();
			zoom -= event.GetYOffset() * 0.5f;
			zoom = glm::max(zoom, 0.5f);
			GetComponent<CameraComponent>().Camera.SetOrthographicSize(zoom);
			m_CameraMovementSpeed = zoom * 0.5f;
			return false;
		}
	public:
		bool p_RotationEnabled = true;
	private:
		float m_CameraMovementSpeed = 10.0f;
		float m_CameraRotationSpeed = glm::half_pi<float>();
	};
	class PerspectiveCameraController : public ScriptableEntity
	{
	public:
		virtual void OnCreate() override
		{
			m_LastMousePos = { Input::GetMouseXPos(), Input::GetMouseYPos() };
			GetComponent<TransformComponent>().Pos = { 0.0f, 0.0f, 3.0f };
		}
		virtual void OnEvent(Event& e) override
		{
			Dispatcher dispatcher(e);
			dispatcher.Dispatch<MouseScrolledEvent>(SB_BIND_EVENT_FUNC(PerspectiveCameraController::OnScrollEvent));
			dispatcher.Dispatch<MouseMovedEvent>(SB_BIND_EVENT_FUNC(PerspectiveCameraController::OnMouseMoveEvent));
		}
		virtual void OnUpdate(Timestep ts) override
		{
			m_LastMousePos = Input::GetMousePos();

			auto& tc = GetComponent<TransformComponent>();

			glm::vec3& translate = tc.Pos;
			glm::vec3 dir(-glm::sin(tc.EulerAngles.y), 0.0f, -glm::cos(tc.EulerAngles.y));

			if (Input::IsKeyPressed(Key::W))
				translate += dir * p_CameraMovementSpeed * (float)ts;
			if (Input::IsKeyPressed(Key::S))
				translate -= dir * p_CameraMovementSpeed * (float)ts;
			if (Input::IsKeyPressed(Key::A))
				translate += glm::normalize(glm::cross({ 0.0f, 1.0f, 0.0f }, dir)) * p_CameraMovementSpeed * (float)ts;
			if (Input::IsKeyPressed(Key::D))
				translate -= glm::normalize(glm::cross({ 0.0f, 1.0f, 0.0f }, dir)) * p_CameraMovementSpeed * (float)ts;
			if (Input::IsKeyPressed(Key::Space))
				translate.y += p_CameraMovementSpeed * (float)ts;
			if (Input::IsKeyPressed(Key::C))
				translate.y -= p_CameraMovementSpeed * (float)ts;
		}
	private:
		bool OnScrollEvent(MouseScrolledEvent& e)
		{
			auto& camera = GetComponent<CameraComponent>().Camera;
			camera.SetFov(glm::clamp(camera.GetFov() - e.GetYOffset() * p_ScrollSensivity, glm::radians(10.0f), glm::radians(90.0f)));
			m_MouseSensivity = m_MouseSens * glm::pow(glm::two_over_pi<float>() * camera.GetFov(), 1 / 2.0f);
			return false;
		}
		bool OnMouseMoveEvent(MouseMovedEvent& e)
		{
			auto& tc = GetComponent<TransformComponent>();
			glm::vec2 currentMousePos = { e.GetXPos(), e.GetYPos() };
			glm::vec2 offset = currentMousePos - m_LastMousePos;
			m_LastMousePos = currentMousePos;

			if (p_EnableRotations)
			{
				tc.EulerAngles.y += -offset.x * m_MouseSensivity;
				tc.EulerAngles.x += -offset.y * m_MouseSensivity;
			}
			return false;
		}
	public:
		float p_CameraMovementSpeed = 5.0f;

		bool p_EnableRotations = false;
		float p_ScrollSensivity = glm::pi<float>() / 36.0f; //5* per scroll
	private:
		static constexpr float m_MouseSens = 0.1f * glm::pi<float>() / 180.0f;
		float m_MouseSensivity = m_MouseSens;
		glm::vec2 m_LastMousePos = { 0, 0 };
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
					glm::vec2 pos = Input::GetMousePos();
					auto& camera = p_Camera.GetComponent<CameraComponent>().Camera;
					auto viewportPos = GetScene()->GetViewportPos();
					auto viewportSize = GetScene()->GetViewportSize();

					glm::vec2 bounds = { camera.GetWidth(), camera.GetHeight() };
					pos.x = (pos.x - viewportPos.x) / viewportSize.x * bounds.x - bounds.x * 0.5f;
					pos.y = bounds.y * 0.5f - (pos.y - viewportPos.y) / viewportSize.y * bounds.y;

					p_Particle.Position = { pos.x, pos.y, 0.1f };
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
