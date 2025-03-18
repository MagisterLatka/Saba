#include <pch.h>
#include "Mesh.h"

namespace Saba {

Mesh::Mesh(std::vector<MeshVertex> vertices, std::vector<uint32_t> indices)
    : m_Vertices(std::move(vertices)), m_Indices(std::move(indices)) {}

}
