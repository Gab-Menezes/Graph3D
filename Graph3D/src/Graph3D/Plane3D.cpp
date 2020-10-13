#include <pch.h>
#include "Plane3D.h"

#include "Camera.h"
#include "VertexBufferAttLayout.h"
#include "Renderer.h"

extern glm::mat4 g_ViewProj;
extern std::unique_ptr<ShaderProgram> g_Shader;

Plane3D::Plane3D()
{
	float PlaneVertices[] =
	{
		-20.f, -20.f, 0.f,
		 20.f, -20.f, 0.f,
		 20.f,  20.f, 0.f,
		-20.f,  20.f, 0.f
	};
	unsigned int PlaneIndex[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	m_PlaneVAO = std::make_unique<VertexArray>();
	m_PlaneVAO->Bind();
	m_PlaneVBO = std::make_unique<VertexBuffer>(&PlaneVertices, sizeof(PlaneVertices));
	m_PlaneVBO->Bind();
	VertexBufferAttLayout planeLayout;
	planeLayout.Push<float>(3);
	m_PlaneVAO->AddAtt(*m_PlaneVBO, planeLayout);
	m_PlaneEBO = std::make_unique<IndexBuffer>(&PlaneIndex, 6);
	m_PlaneEBO->Bind();
	m_PlaneEBO->Unbind();
	m_PlaneVBO->Unbind();
	m_PlaneVAO->Unbind();

}

void Plane3D::Draw() const
{
	if (m_Plane)
	{
		Renderer renderer;
		g_Shader->Bind();
		g_Shader->setUniformMat4f("u_Model", glm::mat4(1.f));
		g_Shader->setUniformMat4f("u_ViewProj", g_ViewProj);
		g_Shader->setUniform1i("u_Grid", (int)m_PlaneGrid);
		g_Shader->setUniform4f("u_Color", m_PlaneColor);
		renderer.Draw(*g_Shader, *m_PlaneVAO, *m_PlaneEBO);
		g_Shader->Unbind();
	}
}

void Plane3D::ImGuiDraw()
{
	ImGui::Text("Plane:");
	ImGui::Indent();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Checkbox("Show##Plane", &m_Plane);
	ImGui::SameLine();
	ImGui::Checkbox("Grid", &m_PlaneGrid);
	ImGui::SameLine();
	ImGui::ColorEdit4("Color", &m_PlaneColor[0], ImGuiColorEditFlags_NoInputs);
	ImGui::SameLine();
	if (ImGui::Button("Default Color"))
		m_PlaneColor = glm::vec4(0.8f, 0.8f, 0.8f, .6f);
	ImGui::Unindent();
}

bool Plane3D::IsSolid() const
{
	return m_PlaneColor.a == 1.f;;
}
