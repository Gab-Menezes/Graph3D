#pragma once
//Includes
#include <pch.h>
#include <Expression.h>

#include <VertexBuffer.h>
#include <VertexArray.h>
#include <VertexBufferAttLayout.h>
#include <ShaderProgram.h>
#include <IndexBuffer.h>

class Graph3D
{
public:
	Graph3D(std::string&& inputString, const glm::vec4& color, const glm::vec2& pos, long long unsigned int id); //Graph3D Constructor
	Graph3D(const Graph3D& rhs); //= Operator Overload
	Graph3D(Graph3D&& rhs) noexcept; //= Operator Overload
	Graph3D& operator=(const Graph3D& rhs); //= Operator Overload
	Graph3D& operator=(Graph3D&& rhs) noexcept; //= Operator Overload

	~Graph3D() = default; //Graph3D Destructor

	void Draw() const; //Draws the Graph3D
	void ImguiDraw();

private:
	long long unsigned int m_Id;
	std::string m_sId;
	glm::vec4 m_Color; //Color vector
	glm::vec2 m_Region, m_PrevRegion; //Range of draw and evaluation of the Graph3D
	//std::unique_ptr<Expression> m_Expression; //Expression Shared Pointer
	Expression m_Expression;
	std::string m_InputString;
	
	//Member variables
	bool m_Show = true, m_Mesh = false, m_Grid = true;

	std::vector<float> m_Vertices; //Vertices of the Graph3D (xy vector)
	std::vector<unsigned int> m_Index; //Vertices of the Graph3D (xy vector)
	int m_Side = 20, m_PrevSide = 20;
	float m_Step = 0.1f, m_PrevStep = 0.1f; //Incrementation step

	std::unique_ptr<VertexBuffer> m_VBO; //Vertex Buffer Shared Pointer
	std::unique_ptr<VertexArray> m_VAO; //Vertex Array Shared Pointer
	std::unique_ptr<IndexBuffer> m_EBO; //Vertex Array Shared Pointer

	glm::mat4 m_Model = glm::mat4(1.f);
	glm::vec3 m_Angle = glm::vec3(0.f);
	glm::vec3 m_Translate = glm::vec3(0.f);
	float m_Scale = 1.f;

	void SetOpenGLObjects();
	void SetIndexBuffer();
	void CalculateGraph();
};