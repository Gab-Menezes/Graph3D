#include <pch.h>
#include "Plane3D.h"

#include <Camera.h>
//#include <ShaderProgram.h>
#include <VertexBufferAttLayout.h>
#include <Renderer.h>

extern glm::mat4 g_Proj;
//extern glm::mat4 g_Model;
extern Camera g_Cam;

Plane3D::Plane3D()
{
	float m_AxisVertices[36] =
	{
		//   X    Y    Z       R    G    B

		//Left X
		-1000.f, 0.f, 0.f,   1.f, 0.f, 0.f,
		//Right X	  	     			  
		 1000.f, 0.f, 0.f,   1.f, 0.f, 0.f,
						     			  
		//Near Y		     			  
		0.f, -1000.f, 0.f,   0.f, 1.f, 0.f,
		//Far Y			     			  
		0.f,  1000.f, 0.f,   0.f, 1.f, 0.f,
						     			 
		//Bottom Z		     			 
		0.f, 0.f, -1000.f,   0.f, 0.f, 1.f,
		//Top Z	             
		0.f, 0.f,  1000.f,   0.f, 0.f, 1.f
	};

	m_Shader = std::make_unique<ShaderProgram>("Plane3D.glsl");
	/*m_Shader->Bind();
	m_Shader->setUniformMat4f("u_Model", g_Model);
	m_Shader->Unbind();*/

	m_VAO = std::make_unique<VertexArray>();
	m_VAO->Bind();

	m_VBO = std::make_unique<VertexBuffer>(&m_AxisVertices, sizeof(m_AxisVertices));
	m_VBO->Bind();
	VertexBufferAttLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	m_VAO->AddAtt(*m_VBO, layout);
}

void Plane3D::Draw()
{
	glClear(GL_DEPTH_BUFFER_BIT);//Z-Buffer

	m_Shader->Bind();
	m_Shader->setUniformMat4f("u_View", g_Cam.GetViewMatrix());
	m_Shader->setUniformMat4f("u_Proj", g_Proj);

	m_VAO->Bind();
	glDrawArrays(GL_LINES, 0, 6);
	m_VAO->Unbind();

	m_Shader->Unbind();
}
