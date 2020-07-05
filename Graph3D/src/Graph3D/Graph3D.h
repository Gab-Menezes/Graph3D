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
	//Graph3D(const std::shared_ptr<Expression>& exp, const glm::vec4& color, const glm::vec2& pos, unsigned int id); //Graph3D Constructor

	Graph3D(const std::string& exp, const glm::vec4& color, const glm::vec2& pos, unsigned int id); //Graph3D Constructor
	Graph3D& operator=(const Graph3D& rhs); //= Operator Overload
	Graph3D(const Graph3D& rhs); //= Operator Overload

	~Graph3D(); //Graph3D Destructor

	void Draw() const; //Draws the Graph3D
	void ImguiDraw();

	void SetIds(unsigned int id);



private:
	//bool window = true;
	
	unsigned int m_Id;
	std::string m_sId;
	
	void SetOpenGLObjects();
	void SetIndexBuffer();
	void CalculateGraph();

	//Member variables
	bool m_Show, m_Mesh;
	std::string m_InputString;
	//std::string m_LabelShow, m_LabelColor, m_LabelInput, m_LabelCompile, m_HeaderLabel, m_MeshLabel, m_LabelRegion, m_LabelSide, m_LabelStep, m_LabelEnterRegionSide, m_LabelDelete, m_LabelTranslate, m_LabelRotate, m_LabelScale;
	glm::vec4 m_Color; //Color vector

	std::unique_ptr<Expression> m_Expression; //Expression Shared Pointer

	std::vector<float> m_Vertices; //Vertices of the Graph3D (xy vector)
	std::vector<unsigned int> m_Index; //Vertices of the Graph3D (xy vector)
	glm::vec2 m_Region, m_PrevRegion; //Range of draw and evaluation of the Graph3D
	float m_Step, m_PrevStep; //Incrementation step
	int m_Side, m_PrevSide;

	std::unique_ptr<ShaderProgram> m_Shader; //Shader Shared Pointer
	std::unique_ptr<VertexBuffer> m_VBO; //Vertex Buffer Shared Pointer
	std::unique_ptr<VertexArray> m_VAO; //Vertex Array Shared Pointer
	std::unique_ptr<IndexBuffer> m_EBO; //Vertex Array Shared Pointer

	glm::mat4 m_Model;
	glm::vec3 m_Angle;
	glm::vec3 m_Translate;
	float m_Scale;
};