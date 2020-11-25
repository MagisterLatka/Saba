#include "pch.h"
#include "Saba/Renderer/Renderer3D.h"

#include "Saba/Renderer/RenderCommand.h"
#include "Saba/Renderer/VertexArray.h"

namespace Saba {

	static constexpr uint32_t c_MaxVertices = 10000;
	static constexpr uint32_t c_MaxIndices = 12500;

	static constexpr uint8_t c_AlbedoTexID = 0;
	static constexpr uint8_t c_NormalTexID = 1;
	static constexpr uint8_t c_HeightTexID = 2;
	static constexpr uint8_t c_RoughnessTexID = 3;
	static constexpr uint8_t c_MetallicTexID = 4;
	static constexpr uint8_t c_AoTexID = 5;

	struct Renderer3DData
	{
		Ref<Shader> defaultShader;
		Ref<Shader> activeShader;

		Ref<VertexArray> vao;

		Mesh::VertexData* buffer = new Mesh::VertexData[c_MaxVertices];
		uint32_t verticesCount = 0;
		uint32_t* indicesBuffer = new uint32_t[c_MaxIndices];
		uint32_t indicesCount = 0;

		uint32_t currentMeshID = 4294967294;
		uint32_t nextMeshID = 0;

		Ref<Texture> whiteTex;

		Renderer3D::Stats stats;
	};
	static Renderer3DData s_Data3D;

	void Renderer3D::Init()
	{
		Mesh::s_MaxVertices = c_MaxVertices;
		Mesh::s_MaxIndices = c_MaxIndices;

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 i_Pos;
			layout(location = 4) in vec4 i_Color;

			out vec4 l_Color;

			uniform mat4 u_ViewProjMat;
			uniform mat4 u_ModelMat = mat4(1.0f);

			void main()
			{
				gl_Position = u_ViewProjMat * u_ModelMat * vec4(i_Pos, 1.0f);
				l_Color = i_Color;
			}
		)";
		std::string fragmentSrc = R"(
			#version 330 core

			out vec4 o_Color;

			in vec4 l_Color;

			void main()
			{
				o_Color = l_Color;
			}
		)";
		s_Data3D.defaultShader = Shader::Create(vertexSrc, fragmentSrc);
		s_Data3D.activeShader = s_Data3D.defaultShader;

		for (uint32_t i = 0; i < c_MaxVertices; i++)
		{
			s_Data3D.buffer[i].normal_Use.w = 0.0f;
			s_Data3D.buffer[i].tangent_Use.w = 0.0f;
			s_Data3D.buffer[i].uv = { 0.0f, 0.0f };
			s_Data3D.buffer[i].color = { 0.0f, 0.0f, 0.0f, 1.0f };
		}

		s_Data3D.vao = VertexArray::Create();
		Ref<VertexBuffer> vbo = VertexBuffer::Create(nullptr, c_MaxVertices * sizeof(Mesh::VertexData), BufferUsage::Dynamic);
		vbo->SetLayout({
			{ "i_Pos", ShaderDataType::Float3 },
			{ "i_Normal_Use", ShaderDataType::Float4 },
			{ "i_Tangent_Use", ShaderDataType::Float4 },
			{ "i_UV", ShaderDataType::Float2 },
			{ "i_Color", ShaderDataType::Float4 }
		});
		s_Data3D.vao->AddVertexBuffer(vbo);

		Ref<IndexBuffer> ibo = IndexBuffer::Create(nullptr, c_MaxIndices * sizeof(uint32_t), BufferUsage::Dynamic);
		s_Data3D.vao->SetIndexBuffer(ibo);

		s_Data3D.whiteTex = Texture2D::Create(1, 1);
		uint32_t white = 0xFFFFFFFF;
		s_Data3D.whiteTex->SetData(&white, sizeof(uint32_t));
	}
	void Renderer3D::Shutdown()
	{
		s_Data3D.defaultShader.reset();
		delete[] s_Data3D.buffer;
		delete[] s_Data3D.indicesBuffer;
	}

	Ref<Shader> Renderer3D::GetCurrentShader()
	{
		return s_Data3D.activeShader;
	}
	void Renderer3D::SetShader(Ref<Shader> shader)
	{
		if (shader)
			s_Data3D.activeShader = shader;
		else
			s_Data3D.activeShader = s_Data3D.defaultShader;
	}

	void Renderer3D::BeginScene(const Camera& camera, const glm::mat4& tranform)
	{
		s_Data3D.activeShader->Bind();
		s_Data3D.activeShader->SetUniformMat4("u_ViewProjMat", camera.GetProjection() * glm::inverse(tranform));
	}
	void Renderer3D::BeginScene(const glm::mat4& viewProjectionMatrix)
	{
		s_Data3D.activeShader->Bind();
		s_Data3D.activeShader->SetUniformMat4("u_ViewProjMat", viewProjectionMatrix);
	}

	void Renderer3D::DrawMesh(Ref<Mesh> mesh, glm::mat4 transform)
	{
		if (mesh->GetVerticesCount() < 0)
		{
			SB_CORE_ASSERT(false, "Invalid mesh");
			return;
		}

		s_Data3D.activeShader->SetUniformMat4("u_ModelMat", transform);

		if (mesh->m_MeshID != s_Data3D.currentMeshID)
		{
			if (mesh->m_MeshID == 4294967295)
			{
				mesh->m_MeshID = s_Data3D.nextMeshID;
				s_Data3D.currentMeshID = s_Data3D.nextMeshID;
				s_Data3D.nextMeshID++;
			}
			else s_Data3D.currentMeshID = mesh->m_MeshID;

			Mesh::VertexData* at = s_Data3D.buffer;
			auto& vertices = mesh->GetVertexData();
			s_Data3D.verticesCount = 0;
			for (auto& vertex : vertices)
			{
				if (mesh->GetMeshType() & Mesh::MeshType::Position)
					at->pos = vertex.pos;
				else at->pos = { 0.0f, 0.0f, 0.0f };
				if (mesh->GetMeshType() & Mesh::MeshType::Normal)
					at->normal_Use = vertex.normal_Use;
				else at->normal_Use.w = 0.0f;
				if (mesh->GetMeshType() & Mesh::MeshType::Tangent)
					at->tangent_Use = vertex.tangent_Use;
				else at->tangent_Use.w = 0.0f;
				if (mesh->GetMeshType() & Mesh::MeshType::UV)
					at->uv = vertex.uv;
				else at->uv = { 0.0f, 0.0f };
				if (mesh->GetMeshType() & Mesh::MeshType::Color)
					at->color = vertex.color;
				else at->color = { 1.0f, 1.0f, 1.0f, 1.0f };

				at++;
				s_Data3D.verticesCount++;
			}
			s_Data3D.vao->GetVertexBuffers()[0]->SetData(s_Data3D.buffer, sizeof(Mesh::VertexData) * s_Data3D.verticesCount);

			if (mesh->GetIndicesCount() > 0)
			{
				memcpy(s_Data3D.indicesBuffer, &mesh->GetIndices()[0], sizeof(uint32_t) * mesh->GetIndicesCount());
				s_Data3D.indicesCount = mesh->GetIndicesCount();
				s_Data3D.vao->GetIndexBuffer()->SetData(s_Data3D.indicesBuffer, sizeof(uint32_t) * s_Data3D.indicesCount);
			}
			else s_Data3D.indicesCount = 0;
		}

		if (mesh->m_AlbedoTex)		mesh->m_AlbedoTex->Bind(c_AlbedoTexID);			else s_Data3D.whiteTex->Bind(c_AlbedoTexID);
		if (mesh->m_NormalTex)		mesh->m_NormalTex->Bind(c_NormalTexID);			else s_Data3D.whiteTex->Bind(c_NormalTexID);
		if (mesh->m_HeightTex)		mesh->m_HeightTex->Bind(c_HeightTexID);			else s_Data3D.whiteTex->Bind(c_HeightTexID);
		if (mesh->m_RoughnessTex)	mesh->m_RoughnessTex->Bind(c_RoughnessTexID);	else s_Data3D.whiteTex->Bind(c_RoughnessTexID);
		if (mesh->m_MetallicTex)	mesh->m_MetallicTex->Bind(c_MetallicTexID);		else s_Data3D.whiteTex->Bind(c_MetallicTexID);
		if (mesh->m_AOTex)			mesh->m_AOTex->Bind(c_AoTexID);					else s_Data3D.whiteTex->Bind(c_AoTexID);

		if (s_Data3D.indicesCount)
			RenderCommand::DrawIndexed(s_Data3D.vao, s_Data3D.indicesCount);
		else
			RenderCommand::DrawVertices(s_Data3D.vao, s_Data3D.verticesCount);

		s_Data3D.stats.verticesCount += s_Data3D.verticesCount;
		s_Data3D.stats.indicesCount += s_Data3D.indicesCount;
		s_Data3D.stats.drawCalls++;
	}

	void Renderer3D::ResetStats()
	{
		memset(&s_Data3D.stats, 0, sizeof(Stats));
	}
	const Renderer3D::Stats& Renderer3D::GetStats()
	{
		return s_Data3D.stats;
	}
}
