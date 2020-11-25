#include <pch.h>
#include <Saba.h>
#include "HierarchyPanel.h"

#include "Saba/ImGui/SabaImGui.h"
#include <imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Saba {

	Saba::SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
		: m_Scene(scene) {}

	void Saba::SceneHierarchyPanel::SetScene(const Ref<Scene>& scene)
	{
		m_Scene = scene;
		m_Selected = {};
	}

	void Saba::SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene hierarchy");

		m_Scene->m_Registry.each([&](auto id)
		{
			Entity entity(id, m_Scene.get());
			DrawEntityNode(entity);
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_Selected = {};

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create entity"))
				m_Selected = m_Scene->CreateEntity();

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");

		DrawComponents(m_Selected);

		ImGui::End();
	}

	void Saba::SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		if (ImGui::Selectable((tag + "###" + std::to_string((uint32_t)entity)).c_str(), m_Selected == entity, ImGuiSelectableFlags_AllowDoubleClick))
			m_Selected = entity;

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Destroy entity"))
			{
				m_Scene->DestroyEntity(entity);
				if (m_Selected == entity)
					m_Selected = {};
			}

			ImGui::EndPopup();
		}
	}

	template<typename T, typename F>
	static void DrawComponent(const Ref<Scene>& scene, const std::string& name, Entity entity, F func, bool removable = true)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 regionAvail = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool opened = ImGui::TreeNodeEx(name.c_str(), treeNodeFlags);
			ImGui::PopStyleVar();

			ImGui::SameLine(regionAvail.x - lineHeight * 0.5f);
			if (ImGui::Button("+", { lineHeight, lineHeight }))
				ImGui::OpenPopup("settings");

			bool removeComponent = false;
			if (removable)
			{
				if (ImGui::BeginPopup("settings"))
				{
					if (ImGui::MenuItem("Remove component"))
						removeComponent = true;

					ImGui::EndPopup();
				}
			}

			if (opened)
			{
				func(component, scene);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (!entity) return;

		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			static char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("###Tag", buffer, sizeof(buffer)))
				tag = buffer;
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add component"))
			ImGui::OpenPopup("add");

		if (ImGui::BeginPopup("add"))
		{
			if (!entity.HasComponent<CameraComponent>())
			{
				if (ImGui::MenuItem("Camera"))
				{
					entity.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}
			}
			if (!entity.HasComponent<SpriteComponent>())
			{
				if (ImGui::MenuItem("Sprite"))
				{
					entity.AddComponent<SpriteComponent>();
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();


		DrawComponent<TransformComponent>(m_Scene, "Transform component", entity, [](TransformComponent& component, auto& scene)
		{
			Saba::DragFloat3("Pos", component.Pos, 0.0f, 0.0f, 0.0f, 0.1f);

			glm::vec3 orientationDegrees = glm::degrees(component.Orientation);
			Saba::DragFloat3("Orientation", orientationDegrees, 0.0f, 0.0f, 0.0f, 1.0f);
			component.Orientation = glm::radians(orientationDegrees);

			Saba::DragFloat3("Scale", component.Scale, 1.0f, 0.01f, 10.0f, 0.01f);
		}, false);
		DrawComponent<SpriteComponent>(m_Scene, "Sprite component", entity, [](SpriteComponent& component, auto& scene)
		{
			Saba::ColorEdit4("Color", component.Color);
			Saba::DragFloat("Tilling factor", component.TillingFactor, 1.0f, 0.01f, 10.0f, 0.01f);

			static char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), component.TextureID.c_str());
			if (ImGui::InputText("Texture", buffer, sizeof(buffer)))
			{
				std::string textureID(buffer);
 				if (TextureManager::Has2D(textureID))
				{
					component.Texture = TextureManager::Get2D(textureID);
					component.TextureID = buffer;
				}
				else
				{
					if (auto a = TextureManager::GetFromFilepath(textureID); a.first)
					{
						component.Texture = a.first;
						component.TextureID = buffer;
					}
				}
			}
		});
		DrawComponent<CameraComponent>(m_Scene, "Camera component", entity, [](CameraComponent& component, auto& scene)
		{
			ImGui::Checkbox("Primary 2D camera", &component.Primary2D);
			ImGui::Checkbox("Primary 3D camera", &component.Primary3D);

			static constexpr char* typeStrings[] = { "Perspective", "Orthographic" };
			const char* currentType = typeStrings[(int)component.Camera.m_Type];
			if (ImGui::BeginCombo("Projection", currentType))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = currentType == typeStrings[i];
					if (ImGui::Selectable(typeStrings[i], isSelected))
					{
						currentType = typeStrings[i];
						component.Camera.m_Type = (SceneCamera::Type)i;
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (component.Camera.m_Type == SceneCamera::Type::Perspective)
			{
				Saba::DragFloat("FOV", component.Camera.m_PerspectiveFov, glm::half_pi<float>(), glm::pi<float>() / 18.0f, glm::quarter_pi<float>() * 3.0f, 0.1f);
				Saba::DragFloat("Near clip", component.Camera.m_PerspectiveNear, 0.01f, 0.01f, 0.5f, 0.01f);
				Saba::DragFloat("Far clip", component.Camera.m_PerspectiveFar, 10.0f, 1.0f, 100.0f, 1.0f);
			}
			else
			{
				Saba::DragFloat("Size", component.Camera.m_OrthographicSize, 10.0f, 0.5f, 20.0f, 0.1f);
				Saba::DragFloat("Near clip", component.Camera.m_OrthographicNear, -1.0f, -10.0f, -1.0f, 0.1f);
				Saba::DragFloat("Far clip", component.Camera.m_OrthographicFar, 1.0f, 1.0f, 10.0f, 0.1f);
			}

			ImGui::Checkbox("Fixed aspect ratio", &component.FixedAspectRatio);
			if (component.FixedAspectRatio)
				Saba::DragFloat("Aspect ratio", component.Camera.m_AspectRatio, (float)scene->m_ViewportSize.x / (float)scene->m_ViewportSize.y, 0.25f, 4.0f, 0.01f);
			else component.Camera.m_AspectRatio = (float)scene->m_ViewportSize.x / (float)scene->m_ViewportSize.y;

			component.Camera.Recalculate();
		});
	}
}
