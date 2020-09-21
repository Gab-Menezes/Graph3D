#include <pch.h>
#include "Plane3D.h"

#include <Camera.h>
#include <VertexBufferAttLayout.h>
#include <Renderer.h>

extern glm::mat4 g_ViewProj;
extern std::unique_ptr<ShaderProgram> g_Shader;

Plane3D::Plane3D()
{
	float AxisVertices[36] =
	{
		//   X    Y    Z      R    G    B
						    
		//Left X		    
		-1000.f, 0.f, 0.f,   1.f, 0.f, 0.f,
		//Right X	  	     			  
		1000.f, 0.f, 0.f,    1.f, 0.f, 0.f,
						     			  
		//Near Y		     			  
		0.f, -1000.f, 0.f,   0.f, 1.f, 0.f,
		//Far Y			     			  
		0.f,  1000.f, 0.f,   0.f, 1.f, 0.f,
						     			 
		//Bottom Z		     			 
		0.f, 0.f, -1000.f,   0.f, 0.f, 1.f,
		//Top Z	             
		0.f, 0.f,  1000.f,   0.f, 0.f, 1.f
	};

	m_AxisShader = std::make_unique<ShaderProgram>("Axis.glsl");

	m_AxisVAO = std::make_unique<VertexArray>();
	m_AxisVAO->Bind();
	m_AxisVBO = std::make_unique<VertexBuffer>(&AxisVertices, sizeof(AxisVertices));
	m_AxisVBO->Bind();
	VertexBufferAttLayout axisLayout;
	axisLayout.Push<float>(3);
	axisLayout.Push<float>(3);
	m_AxisVAO->AddAtt(*m_AxisVBO, axisLayout);
	m_AxisVBO->Unbind();
	m_AxisVAO->Unbind();

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
	if (m_Axis)
	{
		m_AxisShader->Bind();
		m_AxisShader->setUniformMat4f("u_ViewProj", g_ViewProj);
		m_AxisVAO->Bind();
		glDrawArrays(GL_LINES, 0, 6);
		m_AxisVAO->Unbind();
		m_AxisShader->Unbind();
	}
}

void Plane3D::ImGuiDraw()
{
	if (ImGui::CollapsingHeader("Configuration"))
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

		ImGui::Text("Axis:");
		ImGui::Indent();
		ImGui::Checkbox("Show##Axis", &m_Axis);
		ImGui::Unindent();
	}
}
