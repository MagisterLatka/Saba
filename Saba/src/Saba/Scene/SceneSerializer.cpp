#include <pch.h>
#include "SceneSerializer.h"

#include "Saba/Scene/Entity.h"

#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>

namespace YAML {

template<>
struct convert<glm::vec3> {
    static Node encode(const glm::vec3& value) {
        Node node;
        node.push_back(value.x);
        node.push_back(value.y);
        node.push_back(value.z);
        return node;
    }
    static bool decode(const Node& node, glm::vec3& value) {
        if (!node.IsSequence() || node.size() != 3)
            return false;

        value.x = node[0].as<float>();
        value.y = node[1].as<float>();
        value.z = node[2].as<float>();
        return true;
    }
};
template<>
struct convert<glm::vec4> {
    static Node encode(const glm::vec4& value) {
        Node node;
        node.push_back(value.x);
        node.push_back(value.y);
        node.push_back(value.z);
        node.push_back(value.w);
        return node;
    }
    static bool decode(const Node& node, glm::vec4& value) {
        if (!node.IsSequence() || node.size() != 4)
            return false;

        value.x = node[0].as<float>();
        value.y = node[1].as<float>();
        value.z = node[2].as<float>();
        value.w = node[3].as<float>();
        return true;
    }
};

}

namespace Saba {

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& value) {
    out << YAML::Flow;
    out << YAML::BeginSeq << value.x << value.y << value.z << YAML::EndSeq;
    return out;
}
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& value) {
    out << YAML::Flow;
    out << YAML::BeginSeq << value.x << value.y << value.z << value.w << YAML::EndSeq;
    return out;
}

static void SerializeEntity(YAML::Emitter& out, Entity entity) {
    out << YAML::BeginMap; // Entity
    out << YAML::Key << "Entity" << YAML::Value << (uint32_t)entity; //TODO: UUID

    if (entity.HasComponent<TagComponent>()) {
        out << YAML::Key << "Tag component";
        out << YAML::BeginMap; // TagComponent

        auto& tag = entity.GetComponent<TagComponent>().Tag;
        out << YAML::Key << "Tag" << YAML::Value << tag;

        out << YAML::EndMap; // TagComponent
    }
    if (entity.HasComponent<TransformComponent>()) {
        out << YAML::Key << "Transform component";
        out << YAML::BeginMap; // TransformComponent

        auto& tc = entity.GetComponent<TransformComponent>();
        out << YAML::Key << "Position" << YAML::Value << tc.Position;
        out << YAML::Key << "Orientation" << YAML::Value << tc.Orientation;
        out << YAML::Key << "Size" << YAML::Value << tc.Size;

        out << YAML::EndMap; // TransformComponent
    }
    if (entity.HasComponent<CameraComponent>()) {
        out << YAML::Key << "Camera component";
        out << YAML::BeginMap; // CameraComponent

        auto& cameraComponent = entity.GetComponent<CameraComponent>();

        if (cameraComponent.Camera.CanConvert<OrthographicCamera>()) {
            auto camera = cameraComponent.Camera.As<OrthographicCamera>();
            out << YAML::Key << "Type" << YAML::Value << "Orthographic";
            out << YAML::Key << "Size" << YAML::Value << camera->GetSize();
            out << YAML::Key << "Aspect ratio" << YAML::Value << camera->GetAspectRatio();
            out << YAML::Key << "Near clip" << YAML::Value << camera->GetNearClip();
            out << YAML::Key << "Far clip" << YAML::Value << camera->GetFarClip();
        }

        out << YAML::EndMap; // CameraComponent
    }

    if (entity.HasComponent<SpriteComponent>()) {
        out << YAML::Key << "Sprite component";
        out << YAML::BeginMap; // SpriteRendererComponent

        auto& src = entity.GetComponent<SpriteComponent>();
        out << YAML::Key << "Color" << YAML::Value << src.Color;
        //TODO texture
        out << YAML::Key << "Tilling factor" << YAML::Value << src.TillingFactor;

        out << YAML::EndMap; // SpriteRendererComponent
    }

    out << YAML::EndMap; // Entity
}
void SceneSerializer::Serialize(const std::filesystem::path& filepath)
{
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene" << YAML::Value << m_Scene->m_Name;
    out << YAML::Key << "Scene ID" << YAML::Value << m_Scene->m_ID;
    out << YAML::Key << "Scene camera" << YAML::Value << static_cast<uint32_t>(m_Scene->m_Camera);
    out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

    for (auto [id] : m_Scene->m_Registry.view<entt::entity>().each()) {
        Entity entity = { id, m_Scene.Raw() };
        if (!entity || m_Scene->m_Registry.all_of<Scene::SceneComponent>(entity))
            continue;

        SerializeEntity(out, entity);
    }

    out << YAML::EndSeq;
    out << YAML::EndMap;

    std::ofstream fout(filepath);
    fout << out.c_str();
}

bool SceneSerializer::Deserialize(const std::filesystem::path& filepath) {
    const YAML::Node data = YAML::LoadFile(filepath.string());
    if (!data["Scene"])
        return false;

    const auto sceneName = data["Scene"].as<std::string>();
    SB_CORE_TRACE("Deserializing scene '{0}'", sceneName);
    m_Scene->m_Name = sceneName;
    m_Scene->m_ID = data["Scene ID"].as<uint32_t>();
    const auto sceneCamera = data["Scene camera"].as<uint32_t>();

    for (const auto entity : data["Entities"]) {
        const auto id = entity["Entity"].as<uint32_t>();
        //uint64_t uuid = entity["Entity"].as<uint64_t>(); // TODO

        std::string name;
        const auto tagComponent = entity["Tag component"];
        if (tagComponent)
            name = tagComponent["Tag"].as<std::string>();

        SB_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", id, name);

        Entity deserializedEntity = m_Scene->CreateEntity(name);

        const auto transformComponent = entity["Transform component"];
        if (transformComponent) {
            auto& tc = deserializedEntity.GetComponent<TransformComponent>();
            tc.Position = transformComponent["Position"].as<glm::vec3>();
            tc.Orientation = transformComponent["Orientation"].as<glm::vec3>();
            tc.Size = transformComponent["Size"].as<glm::vec3>();
        }

        const auto cameraComponent = entity["Camera component"];
        if (cameraComponent) {
            auto cameraType = cameraComponent["Type"].as<std::string>();
            if (cameraType == "Orthographic") {
                const auto size = cameraComponent["Size"].as<float>();
                const auto aspectRatio = cameraComponent["Aspect ratio"].as<float>();
                const auto nearClip = cameraComponent["Near clip"].as<float>();
                const auto farClip = cameraComponent["Far clip"].as<float>();
                Ref<OrthographicCamera> camera = Ref<OrthographicCamera>::Create(aspectRatio, nearClip, farClip);
                camera->SetSize(size);
                deserializedEntity.AddComponent<CameraComponent>(camera);
            }
        }

        const auto spriteComponent = entity["Sprite component"];
        if (spriteComponent) {
            auto& src = deserializedEntity.AddComponent<SpriteComponent>();
            src.Color = spriteComponent["Color"].as<glm::vec4>();
            src.TillingFactor = spriteComponent["Tilling factor"].as<float>();
        }

        if (id == sceneCamera)
            m_Scene->SetCameraEntity(deserializedEntity);
    }
    return true;
}

}
