#pragma once
#include <pch.h>
#include "ShaderProgram.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

class Renderer
{
public:
	void Draw(const ShaderProgram& ShaderProgram, const VertexArray& VAO, const IndexBuffer& EBO) const
	{
		ShaderProgram.Bind();
		VAO.Bind();
		EBO.Bind();
		glDrawElements(GL_TRIANGLES, EBO.GetCount(), GL_UNSIGNED_INT, 0);
		ShaderProgram.Unbind();
		VAO.Unbind();
		EBO.Unbind();
	}

private:
};