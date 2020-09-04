#include <pch.h>
#include <Saba.h>
#include "HierarchyPanel.h"

#include "Saba/Scene/Components.h"
#include <imGui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Saba {

	Saba::SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
		: m_Scene(scene)
	{}

	void Saba::SceneHierarchyPanel::SetScene(const Ref<Scene>& scene)
	{
		m_Scene = scene;
	}

	void Saba::SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene hierarchy");

		m_Scene->m_Registry.each([&](auto id)
		{
			Entity entity(id, m_Scene.get());
			DrawEntityNode(entity);
		});

		ImGui::End();
	}

	void Saba::SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_Selected == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
			m_Selected = entity;

		if (opened)
		{
			DrawComponents(entity);
			ImGui::TreePop();
		}
	}
	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TransformComponent>())
		{
			auto& transform = entity.GetComponent<TransformComponent>().Transform;
			if (ImGui::TreeNode("Transform component"))
			{
				ImGui::Text("[%.3f  %.3f  %.3f  %.3f]", transform[0][0], transform[1][0], transform[2][0], transform[3][0]);
				ImGui::Text("[%.3f  %.3f  %.3f  %.3f]", transform[0][1], transform[1][1], transform[2][1], transform[3][1]);
				ImGui::Text("[%.3f  %.3f  %.3f  %.3f]", transform[0][2], transform[1][2], transform[2][2], transform[3][2]);
				ImGui::Text("[%.3f  %.3f  %.3f  %.3f]", transform[0][3], transform[1][3], transform[2][3], transform[3][3]);
				ImGui::TreePop();
			}
		}
		if (entity.HasComponent<SpriteComponent>())
		{
			auto& sprc = entity.GetComponent<SpriteComponent>();
			if (ImGui::TreeNode("Sprite component"))
			{
				ImGui::DragFloat3("Position", glm::value_ptr(sprc.Pos), 0.1f, -10.000f, 10.000f);
				ImGui::DragFloat2("Size", glm::value_ptr(sprc.Size), 0.1f, 0.001f, 10.0f);
				ImGui::ColorEdit4("Size", glm::value_ptr(sprc.Color));
				ImGui::DragFloat("Tilling factor", &sprc.TillingFactor, 0.1f, 0.01f, 10.0f);
				ImGui::TreePop();
			}
		}
	}
}
