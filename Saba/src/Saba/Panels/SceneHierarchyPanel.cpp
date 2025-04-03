#include <pch.h>
#include "SceneHierarchyPanel.h"

#include "Saba/ImGui/ImGui.h"
#include "Saba/Panels/ContentBrowserPanel.h"

namespace Saba {

void SceneHierarchyPanel::SetScene(Ref<Scene> scene) noexcept {
    m_Scene = std::move(scene);
    m_Selected = {};
}
void SceneHierarchyPanel::SetSelected(Entity entity) noexcept {
    if (!entity)
        m_Selected = {};
    if (m_Scene.Raw() == entity.GetScene())
        m_Selected = entity;
}

void SceneHierarchyPanel::Open() {
    m_Open = true;
}

void SceneHierarchyPanel::OnUIRender() {
    if (!m_Open)
        return;

    ImGui::Begin("Scene hierarchy", &m_Open);

    if (m_Scene) {
        std::string buffer = m_Scene->m_Name;
        buffer.reserve(256);
        if (ImGui::InputText("Scene name###Tag", buffer.data(), buffer.capacity() + 1))
            m_Scene->m_Name = buffer;

        for (auto [id] : m_Scene->m_Registry.view<entt::entity>().each()) {
            Entity entity(id, m_Scene.Raw());
            DrawEntityNode(entity);
        }

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            m_Selected = {};

        if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverExistingPopup)) {
            if (ImGui::MenuItem("Create entity"))
                m_Selected = m_Scene->CreateEntity();

            ImGui::EndPopup();
        }
    }

    ImGui::End();

    ImGui::Begin("Properties");
    if (m_Scene)
        DrawComponents(m_Selected);
    ImGui::End();
}

void SceneHierarchyPanel::DrawEntityNode(Entity entity) {
    if (m_Scene->m_Registry.all_of<Scene::SceneComponent>(entity))
        return;

    const auto& tag = entity.GetComponent<TagComponent>().Tag;
    std::string nameID = tag + "###" + std::to_string(static_cast<uint32_t>(entity));
    if (ImGui::Selectable(nameID.c_str(), m_Selected == entity, ImGuiSelectableFlags_AllowDoubleClick))
        m_Selected = entity;

    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Destroy entity")) {
            m_Scene->DestroyEntity(entity);
            if (m_Selected == entity)
                m_Selected = {};
        }

        ImGui::EndPopup();
    }
}

template<Component T, typename F>
requires(std::is_invocable_v<F, T&>)
static void DrawComponent(const std::string& name, Entity entity, F func, bool removable = true) {
    if (!entity.HasComponent<T>())
        return;

    auto& component = entity.GetComponent<T>();
    const ImVec2 regionAvail = ImGui::GetContentRegionAvail();

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
    const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImGui::Separator();
    const bool opened = ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
        ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding);
    ImGui::PopStyleVar();

    ImGui::SameLine(regionAvail.x - lineHeight * 0.5f);
    if (ImGui::Button("...", { lineHeight, lineHeight }))
        ImGui::OpenPopup("Settings");

    bool removeComponent = false;
    if (removable) {
        if (ImGui::BeginPopup("Settings")) {
            if (ImGui::MenuItem("Remove component"))
                removeComponent = true;

            ImGui::EndPopup();
        }
    }

    if (opened) {
        func(component);
        ImGui::TreePop();
    }

    if (removeComponent)
        entity.RemoveComponent<T>();
}
void SceneHierarchyPanel::DrawComponents(Entity entity) {
    if (!entity)
        return;

    if (entity.HasComponent<TagComponent>()) {
        auto& tag = entity.GetComponent<TagComponent>().Tag;

        std::string buffer = tag;
        buffer.reserve(256);
        if (ImGui::InputText("###Tag", buffer.data(), buffer.capacity() + 1))
            tag = buffer;
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(-1.0f);

    if (ImGui::Button("Add component"))
        ImGui::OpenPopup("add");

    if (ImGui::BeginPopup("add")) {
        bool hasOneOfExcluding = entity.HasComponent<CameraComponent>() || entity.HasComponent<SpriteComponent>()
            || entity.HasComponent<CircleComponent>() || entity.HasComponent<ModelComponent>() || entity.HasComponent<LightComponent>();
        if (!hasOneOfExcluding) {
            if (ImGui::MenuItem("Camera")) {
                entity.AddComponent<CameraComponent>();
                ImGui::CloseCurrentPopup();
            }
        }
        if (!hasOneOfExcluding) {
            if (ImGui::MenuItem("Sprite")) {
                entity.AddComponent<SpriteComponent>();
                ImGui::CloseCurrentPopup();
            }
        }
        if (!hasOneOfExcluding) {
            if (ImGui::MenuItem("Circle")) {
                entity.AddComponent<CircleComponent>();
                ImGui::CloseCurrentPopup();
            }
        }
        if (!hasOneOfExcluding) {
            if (ImGui::MenuItem("Light")) {
                entity.AddComponent<LightComponent>();
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::EndPopup();
    }
    ImGui::PopItemWidth();

    constexpr float columnWidth = 85.0f;
    if (!entity.HasComponent<LightComponent>()) {
        DrawComponent<TransformComponent>("Transform component", entity, [](TransformComponent& component) {
            UI::DragFloat3("Pos", component.Position, 0.0f, 0.0f, 0.0f, 0.1f, columnWidth);

            auto orientationDegrees = glm::degrees(component.Orientation);
            if (UI::DragFloat3("Orientation", orientationDegrees, 0.0f, 0.0f, 0.0f, 1.0f, columnWidth))
                component.Orientation = glm::radians(orientationDegrees);

            UI::DragFloat3("Scale", component.Size, 1.0f, 0.01f, 10.0f, 0.01f, columnWidth);
        }, false);
    }
    DrawComponent<CircleComponent>("Circle component", entity, [](CircleComponent& component) {
        UI::ColorEdit4("Color", component.Color, columnWidth);
        UI::DragFloat("Thickness", component.Thickness, 1.0f, 0.01f, 1.0f, 0.01f, columnWidth);
        UI::DragFloat("Fade", component.Fade, 0.005f, 0.001f, 1.0f, 0.001f, columnWidth);
    });
    DrawComponent<SpriteComponent>("Sprite component", entity, [](SpriteComponent& component) {
        UI::ColorEdit4("Color", component.Color, columnWidth);

        ImGui::Button("Texture", ImVec2(ImGui::GetColumnWidth(), 0.0f));
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                const char* path = reinterpret_cast<const char*>(payload->Data);
                Texture2DProps props;
                props.Filepath = g_AssetsPath / path;
                auto texture = Texture2D::Create(props);
                if (texture->IsLoaded())
                    component.Texture = texture;
                else
                    SB_CORE_WARN("Loading texture {0} failed", props.Filepath.string());
            }
            ImGui::EndDragDropTarget();
        }

        UI::DragFloat("Tilling factor", component.TillingFactor, 1.0f, 0.01f, 10.0f, 0.01f, columnWidth);
    });
    DrawComponent<CameraComponent>("Camera component", entity, [](CameraComponent& component) {
        if (!component.Camera) {
            component.Camera = Ref<OrthographicCamera>::Create();
        }

        static auto options = std::to_array<std::string>({
            "Orthographic",
            "Perspective"
        });
        uint32_t option = component.Camera.CanConvert<PerspectiveCamera>();
        if (ImGui::BeginCombo("Camera type", options[option].c_str(), ImGuiComboFlags_HeightSmall)) {
            for (const auto& [i, line] : options | std::views::enumerate) {
                const bool isSelected = option == i;
                if (ImGui::Selectable(line.c_str(), isSelected)) {
                    option = static_cast<uint32_t>(i);
                }

                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        if (component.Camera.CanConvert<OrthographicCamera>()) {
            if (option != 0u) {
                component.Camera = Ref<PerspectiveCamera>::Create();
                return;
            }

            auto camera = component.Camera.As<OrthographicCamera>();

            bool changed = false;
            changed |= UI::DragFloat("Aspect ratio", camera->m_AspectRatio, 16.0f / 9.0f, 0.01f, 5.0f, 0.01f, columnWidth);
            changed |= UI::DragFloat("Size", camera->m_Size, 1.0f, 0.5f, 20.0f, 0.1f, columnWidth);
            changed |= UI::DragFloat("Near clip", camera->m_NearClip, -1.0f, -10.0f, -1.0f, 0.1f, columnWidth);
            changed |= UI::DragFloat("Far clip", camera->m_FarClip, 1.0f, 1.0f, 10.0f, 0.1f, columnWidth);
            if (changed)
                camera->Recalc();
        }
        else {
            if (option != 1u) {
                component.Camera = Ref<OrthographicCamera>::Create();
                return;
            }

            auto camera = component.Camera.As<PerspectiveCamera>();

            bool changed = false;
            changed |= UI::DragFloat("Aspect ratio", camera->m_AspectRatio, 16.0f / 9.0f, 0.01f, 5.0f, 0.01f, columnWidth);
            changed |= UI::DragFloat("FOV", camera->m_Fov, glm::half_pi<float>(), glm::radians(10.0f), glm::radians(120.0f), 0.1f, columnWidth);
            changed |= UI::DragFloat("Near clip", camera->m_NearClip, -1.0f, -10.0f, -1.0f, 0.1f, columnWidth);
            changed |= UI::DragFloat("Far clip", camera->m_FarClip, 1.0f, 1.0f, 10.0f, 0.1f, columnWidth);
            if (changed)
                camera->Recalc();
        }
    });
    DrawComponent<LightComponent>("Light component", entity, [](LightComponent& component) {
        UI::DragFloat3("Pos", component.LightPos, 0.0f, -10.0f, 10.0f, 0.1f, columnWidth);
        glm::vec3 color = component.LightColor;
        UI::ColorEdit3("Color", color, columnWidth);
        float intensity = component.LightColor.a;
        UI::DragFloat("Intensity", intensity, 1.0f, 0.1f, 10.0f, 0.1f, columnWidth);
        component.LightColor = glm::vec4(color, intensity);
        UI::DragFloat("Radius", component.Radius, 10.0f, 1.0f, 100.0f, 0.1f, columnWidth);
    });
}

}
