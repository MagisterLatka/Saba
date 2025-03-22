#pragma once

#include "Saba/Core/Core.h"
#include "Saba/Renderer/Mesh.h"
#include "Saba/Renderer/Material.h"
#include "Saba/Renderer/Shader.h"

namespace Saba {

class Renderer3D {
    friend class Renderer;
public:
    SB_CORE static void SetShader(Ref<Shader> shader);
    SB_CORE static void SetViewProjectionMatrix(const glm::mat4& viewProjMat);
    SB_CORE static void SetCameraPosition(const glm::vec3& pos);

    SB_CORE static void Draw();
    SB_CORE static void DrawMesh(Ref<Mesh> mesh);
    SB_CORE static void DrawMesh(UUID meshID);

    SB_CORE static void SubmitMesh(Ref<Mesh> mesh);
    SB_CORE static void SubmitMeshInstance(Ref<Mesh> mesh, const glm::mat4& transform, Ref<Material> material = {}, uint32_t entityID = std::numeric_limits<uint32_t>::max());
    SB_CORE static void SubmitMeshInstance(UUID meshID, const glm::mat4& transform, Ref<Material> material = {}, uint32_t entityID = std::numeric_limits<uint32_t>::max());

    SB_CORE static void SubmitLight(const glm::vec3& lightPos, const glm::vec3& lightColor, float radius);
    SB_CORE static void ResetLights();
private:
    SB_CORE static void Init();
    SB_CORE static void InitMesh(Ref<Mesh> mesh);
    SB_CORE static void Shutdown();
};

}
