#pragma once
#include <pch.h>
#include <VertexArray.h>
#include <VertexBuffer.h>
#include <IndexBuffer.h>
#include <ShaderProgram.h>

class Plane3D
{
public:
	Plane3D();
	void Draw();

private:
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VBO;
	std::unique_ptr<IndexBuffer> m_EBO;
	std::unique_ptr<ShaderProgram> m_Shader;
};