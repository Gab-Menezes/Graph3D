#include <pch.h>
#include <Graph3D.h>
#include <Camera.h>
#include <Renderer.h>
#include <Timer.h>

extern glm::mat4 g_ViewProj;
extern std::unique_ptr<ShaderProgram> g_Shader;

Graph3D::Graph3D(std::string&& inputString, const glm::vec4& color, const glm::vec2& pos, long long unsigned int id) :
	m_Id(id),
	m_sId(std::to_string(id)),
	m_Color(color),
	m_Region(pos),
	m_PrevRegion(pos),
	m_Expression(std::move(inputString)),
	m_InputString(m_Expression.GetStrExpression())
{
	CalculateGraph();
}

Graph3D::Graph3D(const Graph3D& rhs)
{
	m_Id = rhs.m_Id;
	m_sId = rhs.m_sId;
	m_Color = rhs.m_Color;
	m_Region = rhs.m_Region;
	m_PrevRegion = rhs.m_PrevRegion;
	m_Expression = rhs.m_Expression;
	m_InputString = rhs.m_InputString;
	m_Show = rhs.m_Show;
	m_Mesh = rhs.m_Mesh;
	m_Grid = rhs.m_Grid;
	m_Vertices = rhs.m_Vertices;
	m_Index = rhs.m_Index;
	m_Step = rhs.m_Step;
	m_PrevStep = rhs.m_PrevStep;
	m_Side = rhs.m_Side;
	m_PrevSide = rhs.m_PrevSide;
	m_Model = rhs.m_Model;
	m_Angle = rhs.m_Angle;
	m_Translate = rhs.m_Translate;
	m_Scale = rhs.m_Scale;
	SetOpenGLObjects();
}

Graph3D::Graph3D(Graph3D&& rhs) noexcept
{
	std::cout << "Move\n";
	m_Id = rhs.m_Id;
	m_sId = std::move(rhs.m_sId);
	m_Color = rhs.m_Color;
	m_Region = rhs.m_Region;
	m_PrevRegion = rhs.m_PrevRegion;
	m_Expression = std::move(rhs.m_Expression);
	m_InputString = std::move(rhs.m_InputString);
	m_Show = rhs.m_Show;
	m_Mesh = rhs.m_Mesh;
	m_Grid = rhs.m_Grid;
	m_Vertices = std::move(rhs.m_Vertices);
	m_Index = std::move(rhs.m_Index);
	m_Step = rhs.m_Step;
	m_PrevStep = rhs.m_PrevStep;
	m_Side = rhs.m_Side;
	m_PrevSide = rhs.m_PrevSide;
	m_Model = rhs.m_Model;
	m_Angle = rhs.m_Angle;
	m_Translate = rhs.m_Translate;
	m_Scale = rhs.m_Scale;
	SetOpenGLObjects();
}

Graph3D& Graph3D::operator=(const Graph3D& rhs)
{
	m_Id = rhs.m_Id;
	m_sId = rhs.m_sId;
	m_Color = rhs.m_Color;
	m_Region = rhs.m_Region;
	m_PrevRegion = rhs.m_PrevRegion;
	m_Expression = rhs.m_Expression;
	m_InputString = rhs.m_InputString;
	m_Show = rhs.m_Show;
	m_Mesh = rhs.m_Mesh;
	m_Grid = rhs.m_Grid;
	m_Vertices = rhs.m_Vertices;
	m_Index = rhs.m_Index;
	m_Step = rhs.m_Step;
	m_PrevStep = rhs.m_PrevStep;
	m_Side = rhs.m_Side;
	m_PrevSide = rhs.m_PrevSide;
	m_Model = rhs.m_Model;
	m_Angle = rhs.m_Angle;
	m_Translate = rhs.m_Translate;
	m_Scale = rhs.m_Scale;
	SetOpenGLObjects();

	return *this;
}

Graph3D& Graph3D::operator=(Graph3D&& rhs) noexcept
{
	std::cout << "Move=\n";
	m_Id = rhs.m_Id;
	m_sId = std::move(rhs.m_sId);
	m_Color = rhs.m_Color;
	m_Region = rhs.m_Region;
	m_PrevRegion = rhs.m_PrevRegion;
	m_Expression = std::move(rhs.m_Expression);
	m_InputString = std::move(rhs.m_InputString);
	m_Show = rhs.m_Show;
	m_Mesh = rhs.m_Mesh;
	m_Grid = rhs.m_Grid;
	m_Vertices = std::move(rhs.m_Vertices);
	m_Index = std::move(rhs.m_Index);
	m_Step = rhs.m_Step;
	m_PrevStep = rhs.m_PrevStep;
	m_Side = rhs.m_Side;
	m_PrevSide = rhs.m_PrevSide;
	m_Model = rhs.m_Model;
	m_Angle = rhs.m_Angle;
	m_Translate = rhs.m_Translate;
	m_Scale = rhs.m_Scale;
	SetOpenGLObjects();
	return *this;
}

void Graph3D::Draw() const
{
	if (m_Show && m_Expression.IsCompiled())
	{
		g_Shader->Bind(); //Binds the shader
		g_Shader->setUniformMat4f("u_Model", m_Model);
		g_Shader->setUniformMat4f("u_ViewProj", g_ViewProj);
		g_Shader->setUniform4f("u_Color", m_Color);
		g_Shader->setUniform1i("u_Grid", (int)m_Grid);
		if (m_Mesh)
		{
			Renderer render;
			render.Draw(*g_Shader, *m_VAO, *m_EBO);
		}
		else
		{
			m_VBO->Bind(); //Binds the vertex buffer
			m_VAO->Bind(); //Binds the vertex array

			glDrawArrays(GL_POINTS, 0, m_Vertices.size() / 3); //Draw the Graph3D (OpenGL call)
		}


		g_Shader->Unbind(); //Unbinds the shader
		m_VBO->Unbind(); //Unbinds the vertex buffer
		m_VAO->Unbind(); //Unbinds the vertex array
	}
}

void Graph3D::ImguiDraw()
{
	//Color picker
	auto ColorFlags = ImGuiColorEditFlags_NoInputs + ImGuiColorEditFlags_NoLabel;//Color Flags
	std::string LabelColor = "Color##" + m_sId;
	ImGui::ColorEdit4(LabelColor.c_str(), &m_Color[0], ColorFlags);

	ImGui::SameLine(); //Puts others ImGui objects in the same line

	std::string HeaderLabel = m_Expression.GetStrExpression() + "##" + m_sId;
	if (ImGui::CollapsingHeader(HeaderLabel.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Indent(35.f);

		ImGui::Text("Graph Input:");
		ImGui::PushItemWidth(250.f);
		std::string LabelInput = "Expression##" + m_sId;
		ImGui::PushID(LabelInput.c_str()); //Pushs an ID for not enter a name in the ImGui::InputText()
		bool InputExp = ImGui::InputText("", &m_InputString, ImGuiInputTextFlags_EnterReturnsTrue);
		ImGui::PopID(); //ImGui PopID
		ImGui::PopItemWidth();
		ImGui::SameLine();
		std::string LabelCompile = "Enter##" + m_sId;
		if ((ImGui::Button(LabelCompile.c_str()) || InputExp) && m_InputString != m_Expression.GetStrExpression())
		{
			HeaderLabel = m_InputString + "##" + m_sId;
			m_Expression.SetExpression(m_InputString); //Sets the string expression of m_Expression to strExp

			CalculateGraph();
		}

		std::string LabelShow = "Show##" + m_sId;
		ImGui::Checkbox(LabelShow.c_str(), &m_Show); //ImGui Checkbox
		ImGui::SameLine();
		std::string LabelMesh = "Mesh##" + m_sId;
		if (ImGui::Checkbox(LabelMesh.c_str(), &m_Mesh))
		{
			if (m_Mesh)
				m_Color.a = 0.5f;
			else
				m_Color.a = 1.f;
		}
		ImGui::SameLine();

		std::string LabelGrid = "Grid##" + m_sId;
		ImGui::Checkbox(LabelGrid.c_str(), &m_Grid);

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Graph Configuration:");
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.7f, 0.15f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 1.f, 0.f, 0.7f));
		std::string LabelEnterRegionSide = "Apply##" + m_sId;
		if (ImGui::Button(LabelEnterRegionSide.c_str()) && (m_Region != m_PrevRegion || m_Side != m_PrevSide || m_Step != m_PrevStep) && (m_Side > 0 && m_Step > 0.f))
		{
			m_PrevRegion = m_Region;
			m_PrevSide = m_Side;
			m_PrevStep = m_Step;

			CalculateGraph();
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PushItemWidth(250.f);
		std::string LabelRegion = "Region##" + m_sId;
		std::string LabelSide = "Side Length##" + m_sId;
		std::string LabelStep = "Step##" + m_sId;
		ImGui::InputFloat2(LabelRegion.c_str(), &m_Region[0]);
		ImGui::InputFloat(LabelStep.c_str(), &m_Step);
		ImGui::InputInt(LabelSide.c_str(), &m_Side);
		ImGui::PopItemWidth();

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Graph Transformation:");
		ImGui::PushItemWidth(400.f);
		std::string LabelTranslate = "Translate (X, Y, Z)##" + m_sId;
		std::string LabelRotate = "Rotate (X, Y, Z)##" + m_sId;
		std::string LabelScale = "Scale##" + m_sId;
		bool change = ImGui::InputFloat3(LabelTranslate.c_str(), &m_Translate[0]);
		change |= ImGui::SliderFloat3(LabelRotate.c_str(), &m_Angle[0], 0.f, 360.f);
		ImGui::PopItemWidth();
		ImGui::PushItemWidth(100.f);
		change |= ImGui::InputFloat(LabelScale.c_str(), &m_Scale);
		ImGui::PopItemWidth();
		if (change)
		{
			m_Model = glm::translate(glm::mat4(1.f), m_Translate);
			m_Model = glm::rotate(m_Model, glm::radians(m_Angle.x), glm::vec3(1.f, 0.f, 0.f));
			m_Model = glm::rotate(m_Model, glm::radians(m_Angle.y), glm::vec3(0.f, 1.f, 0.f));
			m_Model = glm::rotate(m_Model, glm::radians(m_Angle.z), glm::vec3(0.f, 0.f, 1.f));
			m_Model = glm::scale(m_Model, glm::vec3(m_Scale));
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Unindent(35.f);
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
}

void Graph3D::SetOpenGLObjects()
{
	//Re - does the vertex buffer and vertex array
	m_VAO = std::make_unique<VertexArray>(); //Creates the Vertex Array
	m_VAO->Bind(); //Binds the Vertex Array

	m_VBO = std::make_unique<VertexBuffer>(&m_Vertices[0], m_Vertices.size() * sizeof(float)); //Creates the Vertex Buffer
	m_VBO->Bind(); //Binds the Vertex Buffer

	VertexBufferAttLayout attLayout; //Creates a Attribute Layout
	attLayout.Push<float>(3); //Pushs 2 doubles as read mode
	m_VAO->AddAtt(*m_VBO, attLayout); //Adds the Attribute Layout (attLayout) to the Vertex Array (m_VAO)

	m_EBO = std::make_unique<IndexBuffer>(&m_Index[0], m_Index.size());
	m_EBO->Bind();


	m_VAO->Unbind();
	m_VBO->Unbind();
	m_EBO->Unbind();
}

void Graph3D::SetIndexBuffer()
{
	m_Index.clear();
	unsigned int n = (unsigned int)sqrt(m_Vertices.size() / 3);
	m_Index.reserve(6 * (n - 1) * (n - 1));
	for (unsigned int i = 0; i < n - 1; i++)
	{
		for (unsigned int j = 0; j < n - 1; j++)
		{
			m_Index.push_back(i * n + j);
			m_Index.push_back(i * n + j + n);
			m_Index.push_back(i * n + j + 1);

			m_Index.push_back(i * n + j + 1);
			m_Index.push_back(i * n + j + 1 + n);
			m_Index.push_back(i * n + j + n);
		}
	}
}

void Graph3D::CalculateGraph()
{
	if (m_Expression.IsCompiled())
	{
		m_Vertices = m_Expression.Calculate(m_Region, m_Side, m_Step); //Calculates the vertices of the Graph3D
		SetIndexBuffer();
		SetOpenGLObjects();
	}
}
