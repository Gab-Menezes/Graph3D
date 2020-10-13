#pragma once
#include <pch.h>
#include "Surface.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"

class Plane3D : public Surface
{
public:
	Plane3D();
	void Draw () const override;
	void ImGuiDraw() override;
	bool IsSolid() const override;

private:
	std::unique_ptr<VertexArray> m_PlaneVAO;
	std::unique_ptr<VertexBuffer> m_PlaneVBO;
	std::unique_ptr<IndexBuffer> m_PlaneEBO;
	bool m_Plane = true, m_PlaneGrid = true;
	glm::vec4 m_PlaneColor = glm::vec4(0.8f, 0.8f, 0.8f, .6f);
};