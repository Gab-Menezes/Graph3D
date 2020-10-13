#pragma once
#include <pch.h>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"

class Axis
{
public:
	Axis();
	void Draw() const;
	void ImGuiDraw();

private:
	std::array<std::unique_ptr<VertexArray>,   3> m_AxisVAO;
	std::array<std::unique_ptr<VertexBuffer>,  3> m_AxisVBO;
	std::unique_ptr<ShaderProgram> m_AxisShader;
	bool m_Axis = true;
};
