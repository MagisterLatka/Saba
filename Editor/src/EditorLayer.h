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
		bool OnKeyPress(KeyPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveScene();
		void SaveAsScene();
	private:
		Ref<Framebuffer> m_FBO;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportPos = { 0.0f, 0.0f };
		bool m_ViewportFocused = false, m_ViewportHovered = false;

		ParticleSystem m_ParticleSystem;

		Ref<Scene> m_Scene;
		Entity m_Camera;
		std::string m_CurrentScene;

		SceneHierarchyPanel m_HierarchyPanel;
		int m_GuimzoType = -1;
	};
}
