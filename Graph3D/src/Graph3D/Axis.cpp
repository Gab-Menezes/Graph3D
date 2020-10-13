#include <pch.h>
#include "Axis.h"
#include "Camera.h"

extern Camera g_Cam;
extern glm::mat4 g_ViewProj;

Axis::Axis()
{
	float Axis[3][6] =
	{
		{
			//Left X		 
			-1000.f, 0.f, 0.f,
			//Right X	  	 
			 1000.f, 0.f, 0.f,
		},
		{
			//Near Y		 
			0.f, -1000.f, 0.f,
			//Far  Y	  	 
			0.f,  1000.f, 0.f,
		},
		{
			//Down Z		 
			0.f, 0.f, -1000.f,
			//Up Z	  	 
			0.f, 0.f,  1000.f,
		}
	};

	m_AxisShader = std::make_unique<ShaderProgram>("Axis.glsl");
	VertexBufferAttLayout axisLayout;
	axisLayout.Push<float>(3);

	for (size_t i = 0; i < 3; i++)
	{
		m_AxisVAO[i] = std::make_unique<VertexArray>();
		m_AxisVAO[i]->Bind();
		m_AxisVBO[i] = std::make_unique<VertexBuffer>(&Axis[i], sizeof(Axis[i]));
		m_AxisVBO[i]->Bind();
		m_AxisVAO[i]->AddAtt(*m_AxisVBO[i], axisLayout);
		m_AxisVBO[i]->Unbind();
		m_AxisVAO[i]->Unbind();
	}
}

void Axis::Draw() const
{
	if (m_Axis)
	{
		glm::mat4 model;// = glm::translate(glm::mat4(1.f), glm::vec3(g_Cam.GetCamPosRef()[0], 0.f, 0.f));
		m_AxisShader->Bind();
		m_AxisShader->setUniformMat4f("u_ViewProj", g_ViewProj);

		model = glm::translate(glm::mat4(1.f), glm::vec3(g_Cam.GetCamPosRef()[0], 0.f, 0.f));
		m_AxisShader->setUniformMat4f("u_Model", model);
		m_AxisShader->setUniform3f("u_Color", glm::vec3(1.f, 0.f, 0.f));
		m_AxisVAO[0]->Bind();
		glDrawArrays(GL_LINES, 0, 2);
		m_AxisVAO[0]->Unbind();

		model = glm::translate(glm::mat4(1.f), glm::vec3(0.f, g_Cam.GetCamPosRef()[1], 0.f));
		m_AxisShader->setUniformMat4f("u_Model", model);
		m_AxisShader->setUniform3f("u_Color", glm::vec3(0.f, 1.f, 0.f));
		m_AxisVAO[1]->Bind();
		glDrawArrays(GL_LINES, 0, 2);
		m_AxisVAO[1]->Unbind();

		model = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, g_Cam.GetCamPosRef()[2]));
		m_AxisShader->setUniformMat4f("u_Model", model);
		m_AxisShader->setUniform3f("u_Color", glm::vec3(0.f, 0.f, 1.f));
		m_AxisVAO[2]->Bind();
		glDrawArrays(GL_LINES, 0, 2);
		m_AxisVAO[2]->Unbind();


		m_AxisShader->Unbind();
	}
}

void Axis::ImGuiDraw()
{
	ImGui::Text("Axis:");
	ImGui::Indent();
	ImGui::Checkbox("Show##Axis", &m_Axis);
	ImGui::Unindent();
}
