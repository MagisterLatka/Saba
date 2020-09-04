#pragma once

#include "Panels/HierarchyPanel.h"

namespace Saba {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Saba::Event& event) override;
		virtual void OnUpdate(Saba::Timestep ts) override;
		virtual void OnImGuiRender() override;
	private:
		Ref<Framebuffer> m_FBO;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportPos = { 0.0f, 0.0f };
		bool m_ViewportFocused = false, m_ViewportHovered = false;

		ParticleSystem m_ParticleSystem;

		Ref<Scene> m_Scene;
		Entity m_ColoredQuad;
		Entity m_Camera;
		Entity m_ParticleSystemController;

		SceneHierarchyPanel m_HierarchyPanel;
#if SB_DEBUG
		static constexpr float m_QuadFrequency = 0.25f;
#else
		static constexpr float m_QuadFrequency = 0.1f;
#endif
	};
}
