#pragma once

#include "Saba/Core/Core.h"
#include "Saba/Renderer/Mesh.h"

namespace Saba {

class Renderer3D {
    friend class Renderer;
public:
    SB_CORE static void SetViewProjectionMatrix(const glm::mat4& viewProjMat);

    SB_CORE static void Draw();
    SB_CORE static void DrawMesh(Ref<Mesh> mesh);
    SB_CORE static void DrawMesh(UUID meshID);

    SB_CORE static void SubmitMesh(Ref<Mesh> mesh);
    SB_CORE static void SubmitMeshInstance(Ref<Mesh> mesh, const glm::mat4& transform, uint32_t entityID = std::numeric_limits<uint32_t>::max());
    SB_CORE static void SubmitMeshInstance(UUID meshID, const glm::mat4& transform, uint32_t entityID = std::numeric_limits<uint32_t>::max());
private:
    SB_CORE static void Init();
    SB_CORE static void InitMesh(Ref<Mesh> mesh);
    SB_CORE static void Shutdown();
};

}
