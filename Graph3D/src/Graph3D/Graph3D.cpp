#include <pch.h>
#include <Graph3D.h>
#include <Camera.h>
#include <Renderer.h>
#include <Timer.h>

extern glm::mat4 g_Proj;
extern Camera g_Cam;
extern std::vector<Graph3D> g_Graphs;

//void DeleteGraph(unsigned int i)
//{
//	g_Graphs.erase(g_Graphs.begin() + i);
//
//	for (unsigned int i = 0; i < g_Graphs.size(); i++)
//	{
//		g_Graphs[i].SetIds(i);
//	}
//}


//Constructor
//Graph3D::Graph3D(const std::shared_ptr<Expression>& exp, const glm::vec4& color, const glm::vec2& pos, unsigned int id) :
Graph3D::Graph3D(const std::string& exp, const glm::vec4& color, const glm::vec2& pos, unsigned int id) :
	//m_Expression(exp), //m_Expression = exp
	m_Color(color), //m_Color = color
	m_Region(pos), //m_Range = range
	m_PrevRegion(pos),

	m_Id(id),
	m_sId(std::to_string(id)),

	/*m_InputString(exp->GetStrExpression()),*/
	m_InputString(exp),

	m_Side(20),
	m_PrevSide(20),
	m_Step(0.1f), //m_Step = step
	m_PrevStep(0.1f),

	m_Show(true),
	m_Mesh(false),
	//m_LabelShow("Show##" + m_sId),
	//m_LabelColor("Color##" + m_sId),
	//m_LabelInput("Expression##" + m_sId),
	//m_LabelCompile("Enter##" + m_sId),
	//m_HeaderLabel(m_InputString + "##" + m_sId),
	//m_MeshLabel("Mesh##" + m_sId),
	//m_LabelRegion("Region##" + m_sId),
	//m_LabelSide("Side Length##" + m_sId),
	//m_LabelStep("Step##" + m_sId),
	//m_LabelEnterRegionSide("Apply##" + m_sId),
	//m_LabelDelete("Delete##" + m_sId),
	//m_LabelTranslate("Translate (X, Y, Z)##" + m_sId),
	//m_LabelRotate("Rotate (X, Y, Z)##" + m_sId),
	//m_LabelScale("Scale##" + m_sId),

	m_Model(glm::mat4(1.f)),
	m_Angle(glm::vec3(0.f)),
	m_Translate(glm::vec3(0.f)),
	m_Scale(1.f)
{
	m_Expression = std::make_unique<Expression>(exp);
	
	//Opengl
	m_Shader = std::make_unique<ShaderProgram>("Graph3D.glsl"); //Creates the Shader
	m_Shader->Bind(); //Binds the Shader
	m_Shader->setUniformMat4f("u_Model", m_Model); //Sets the uniform "u_Model" to m_Model
	m_Shader->setUniform4f("u_Color", m_Color); //Sets the uniform "u_Color" to m_Color
	m_Shader->Unbind();


	CalculateGraph();
}

//Destructor
Graph3D::~Graph3D()
{
}

void Graph3D::Draw() const
{
	if (m_Show && m_Expression->IsCompiled())
	{
		m_Shader->Bind(); //Binds the shader
		m_Shader->setUniformMat4f("u_View", g_Cam.GetViewMatrix());
		m_Shader->setUniformMat4f("u_Proj", g_Proj);
		if (m_Mesh)
		{
			Renderer render;
			render.Draw(*m_Shader, *m_VAO, *m_EBO);
		}
		else
		{
			m_VBO->Bind(); //Binds the vertex buffer
			m_VAO->Bind(); //Binds the vertex array

			glDrawArrays(GL_POINTS, 0, m_Vertices.size() / 3); //Draw the Graph3D (OpenGL call)
		}


		m_Shader->Unbind(); //Unbinds the shader
		m_VBO->Unbind(); //Unbinds the vertex buffer
		m_VAO->Unbind(); //Unbinds the vertex array
	}
}

//void Graph3D::ImguiDraw()
//{
//	//Color picker
//	auto ColorFlags = ImGuiColorEditFlags_NoInputs + ImGuiColorEditFlags_NoLabel;//Color Flags
//	if (ImGui::ColorEdit4(m_LabelColor.c_str(), &m_Color[0], ColorFlags))
//	{
//		m_Shader->Bind(); //Binds the shader
//		m_Shader->setUniform4f("u_Color", m_Color);
//		m_Shader->Unbind();
//	}
//
//	ImGui::SameLine(); //Puts others ImGui objects in the same line
//
//	if (ImGui::CollapsingHeader(m_HeaderLabel.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
//	{
//		ImGui::Indent(35.f);
//
//		ImGui::Text("Graph Input:");
//		ImGui::PushItemWidth(250.f);
//		ImGui::PushID(m_LabelInput.c_str()); //Pushs an ID for not enter a name in the ImGui::InputText()
//		bool InputExp = ImGui::InputText("", &m_InputString, ImGuiInputTextFlags_EnterReturnsTrue);
//		ImGui::PopID(); //ImGui PopID
//		ImGui::PopItemWidth();
//		ImGui::SameLine();
//		if ((ImGui::Button(m_LabelCompile.c_str()) || InputExp) && m_InputString != m_Expression->GetStrExpression())
//		{
//			m_HeaderLabel = m_InputString + "##" + m_sId;
//			m_Expression->SetExpression(m_InputString); //Sets the string expression of m_Expression to strExp
//
//			CalculateGraph();
//		}
//
//		ImGui::Checkbox(m_LabelShow.c_str(), &m_Show); //ImGui Checkbox
//		ImGui::SameLine();
//		if (ImGui::Checkbox(m_MeshLabel.c_str(), &m_Mesh))
//		{
//			if (m_Mesh)
//				m_Color.a = 0.5f;
//			else
//				m_Color.a = 1.f;
//
//			m_Shader->Bind(); //Binds the shader
//			m_Shader->setUniform4f("u_Color", m_Color);
//			m_Shader->Unbind();
//		}
//
//		ImGui::Spacing();
//		ImGui::Spacing();
//		ImGui::Spacing();
//		
//		ImGui::Text("Graph Configuration:");
//		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.7f, 0.15f, 0.7f));
//		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 1.f, 0.f, 0.7f));
//		if (ImGui::Button(m_LabelEnterRegionSide.c_str()) && (m_Region != m_PrevRegion || m_Side != m_PrevSide || m_Step != m_PrevStep) && (m_Side > 0 && m_Step > 0.f))
//		{
//			m_PrevRegion = m_Region;
//			m_PrevSide = m_Side;
//			m_PrevStep = m_Step;
//
//			CalculateGraph();
//		}
//		ImGui::PopStyleColor();
//		ImGui::PopStyleColor();
//		ImGui::PushItemWidth(250.f);
//		ImGui::InputFloat2(m_LabelRegion.c_str(), &m_Region[0]);
//		ImGui::InputFloat(m_LabelStep.c_str(), &m_Step);
//		ImGui::InputInt(m_LabelSide.c_str(), &m_Side);
//		ImGui::PopItemWidth();
//
//		ImGui::Spacing();
//		ImGui::Spacing();
//		ImGui::Spacing();
//
//		ImGui::Text("Graph Transformation:");
//		ImGui::PushItemWidth(400.f);
//		bool change = ImGui::InputFloat3(m_LabelTranslate.c_str(), &m_Translate[0]);
//		change |= ImGui::SliderFloat3(m_LabelRotate.c_str(), &m_Angle[0], 0.f, 360.f);
//		ImGui::PopItemWidth();
//		ImGui::PushItemWidth(100.f);
//		change |= ImGui::InputFloat(m_LabelScale.c_str(), &m_Scale);
//		ImGui::PopItemWidth();
//		if (change)
//		{
//			m_Model = glm::translate(glm::mat4(1.f), m_Translate);
//			m_Model = glm::rotate(m_Model, glm::radians(m_Angle.x), glm::vec3(1.f, 0.f, 0.f));
//			m_Model = glm::rotate(m_Model, glm::radians(m_Angle.y), glm::vec3(0.f, 1.f, 0.f));
//			m_Model = glm::rotate(m_Model, glm::radians(m_Angle.z), glm::vec3(0.f, 0.f, 1.f));
//			m_Model = glm::scale(m_Model, glm::vec3(m_Scale));
//
//			m_Shader->Bind(); //Binds the Shader
//			m_Shader->setUniformMat4f("u_Model", m_Model); //Sets the uniform "u_Model" to m_Model
//			m_Shader->Unbind();
//		}
//
//		ImGui::Spacing();
//		ImGui::Spacing();
//		ImGui::Spacing();
//
//		/*ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(.7f, 0.15f, 0.15f, 0.7f));
//		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.f, 0.f, 0.f, 0.7f));
//		if (ImGui::Button(m_LabelDelete.c_str()))
//		{
//			DeleteGraph(m_Id);
//		}
//		ImGui::PopStyleColor();
//		ImGui::PopStyleColor();*/
//
//
//		ImGui::Unindent(35.f);
//	}
//
//
//	ImGui::Spacing();
//	ImGui::Separator();
//	ImGui::Spacing();
//}

void Graph3D::ImguiDraw()
{
	//Color picker
	auto ColorFlags = ImGuiColorEditFlags_NoInputs + ImGuiColorEditFlags_NoLabel;//Color Flags
	std::string LabelColor = "Color##" + m_sId;
	if (ImGui::ColorEdit4(LabelColor.c_str(), &m_Color[0], ColorFlags))
	{
		m_Shader->Bind(); //Binds the shader
		m_Shader->setUniform4f("u_Color", m_Color);
		m_Shader->Unbind();
	}

	ImGui::SameLine(); //Puts others ImGui objects in the same line

	std::string HeaderLabel = m_Expression->GetStrExpression() + "##" + m_sId;
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
		if ((ImGui::Button(LabelCompile.c_str()) || InputExp) && m_InputString != m_Expression->GetStrExpression())
		{
			HeaderLabel = m_InputString + "##" + m_sId;
			m_Expression->SetExpression(m_InputString); //Sets the string expression of m_Expression to strExp

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

			m_Shader->Bind(); //Binds the shader
			m_Shader->setUniform4f("u_Color", m_Color);
			m_Shader->Unbind();
		}

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

			m_Shader->Bind(); //Binds the Shader
			m_Shader->setUniformMat4f("u_Model", m_Model); //Sets the uniform "u_Model" to m_Model
			m_Shader->Unbind();
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		/*ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(.7f, 0.15f, 0.15f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.f, 0.f, 0.f, 0.7f));
		if (ImGui::Button(m_LabelDelete.c_str()))
		{
			DeleteGraph(m_Id);
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();*/


		ImGui::Unindent(35.f);
	}


	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
}

void Graph3D::SetIds(unsigned int id)
{
	m_Id = id;
	m_sId = std::to_string(id);

	//m_LabelShow = "Show##" + m_sId;
	//m_LabelColor = "Color##" + m_sId;
	//m_LabelInput = "Expression##" + m_sId;
	//m_LabelCompile = "Enter##" + m_sId;
	//m_HeaderLabel = m_InputString + "##" + m_sId;
	//m_MeshLabel = "Mesh##" + m_sId;
	//m_LabelRegion = "Region##" + m_sId;
	//m_LabelSide = "Side Length##" + m_sId;
	//m_LabelStep = "Step##" + m_sId;
	//m_LabelEnterRegionSide = "Apply##" + m_sId;
	//m_LabelDelete = "Delete##" + m_sId;
}

Graph3D& Graph3D::operator=(const Graph3D& rhs)
{
	// TODO: inserir instrução return aqui
	m_Id = rhs.m_Id;
	m_sId = rhs.m_sId;
	m_Show = rhs.m_Show;
	m_Mesh = rhs.m_Mesh;
	m_Color = rhs.m_Color;
	m_Expression = std::make_unique<Expression>(rhs.m_Expression->GetStrExpression());
	m_Vertices = rhs.m_Vertices;
	m_Index = rhs.m_Index;
	m_Region = rhs.m_Region;
	m_PrevRegion = rhs.m_PrevRegion;
	m_Step = rhs.m_Step;
	m_PrevStep = rhs.m_PrevStep;
	m_Side = rhs.m_Side;
	m_PrevSide = rhs.m_PrevSide;

	m_Model = rhs.m_Model;
	m_Angle = rhs.m_Angle;
	m_Translate = rhs.m_Translate;
	m_Scale = rhs.m_Scale;

	m_Shader = std::make_unique<ShaderProgram>("Graph3D.glsl"); //Creates the Shader
	m_Shader->Bind(); //Binds the Shader
	m_Shader->setUniformMat4f("u_Model", m_Model); //Sets the uniform "u_Model" to m_Model
	m_Shader->setUniform4f("u_Color", m_Color); //Sets the uniform "u_Color" to m_Color
	m_Shader->Unbind();

	SetOpenGLObjects();

	return *this;
}

Graph3D::Graph3D(const Graph3D& rhs)
{
	m_Id = rhs.m_Id;
	m_sId = rhs.m_sId;
	m_Show = rhs.m_Show;
	m_Mesh = rhs.m_Mesh;
	m_Color = rhs.m_Color;
	m_Expression = std::make_unique<Expression>(rhs.m_Expression->GetStrExpression());
	m_Vertices = rhs.m_Vertices;
	m_Index = rhs.m_Index;
	m_Region = rhs.m_Region;
	m_PrevRegion = rhs.m_PrevRegion;
	m_Step = rhs.m_Step;
	m_PrevStep = rhs.m_PrevStep;
	m_Side = rhs.m_Side;
	m_PrevSide = rhs.m_PrevSide;

	m_Model = rhs.m_Model;
	m_Angle = rhs.m_Angle;
	m_Translate = rhs.m_Translate;
	m_Scale = rhs.m_Scale;

	m_Shader = std::make_unique<ShaderProgram>("Graph3D.glsl"); //Creates the Shader
	m_Shader->Bind(); //Binds the Shader
	m_Shader->setUniformMat4f("u_Model", m_Model); //Sets the uniform "u_Model" to m_Model
	m_Shader->setUniform4f("u_Color", m_Color); //Sets the uniform "u_Color" to m_Color
	m_Shader->Unbind();

	SetOpenGLObjects();
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

static void ThreadIndexBuffer(std::vector<unsigned int>* vec, int begin, int end, int n)
{
	for (unsigned int i = begin; i < end; i++)
	{
		for (unsigned int j = 0; j < n - 1; j++)
		{
			vec->push_back(i * n + j);
			vec->push_back(i * n + j + n);
			vec->push_back(i * n + j + 1);

			vec->push_back(i * n + j + 1);
			vec->push_back(i * n + j + 1 + n);
			vec->push_back(i * n + j + n);
		}
	}
	//std::cout << "Acabou\n";
}

void Graph3D::SetIndexBuffer()
{
	m_Index.clear();
	unsigned int n = sqrt(m_Vertices.size() / 3);
	m_Index.reserve(6 * (n - 1) * (n - 1));


	{
		/*Timer t("Data");
		int threadPerN = 50;
		std::vector<std::vector<unsigned int>> VecIndex;
		std::vector<std::thread> VecThread;
		int numOfThread = (n-1)/threadPerN;
		VecThread.reserve(numOfThread);
		VecIndex.reserve(numOfThread);

		for (int i = 0; i < numOfThread; i++)
		{
			VecIndex.push_back(std::vector<unsigned int>());
			int begin = threadPerN * i;
			int end = begin + threadPerN > (n-1) ? n-1 : begin + threadPerN;
			VecIndex[i].reserve(threadPerN * (n - 1) * 6);
			VecThread.push_back(std::thread(ThreadIndexBuffer, &VecIndex[i], begin, end, n));
		}

		for (int i = 0; i < numOfThread; i++)
		{
			VecThread[i].join();
		}

		for (int i = 0; i < numOfThread; i++)
		{
			m_Index.insert(m_Index.end(), std::make_move_iterator(VecIndex[i].begin()), std::make_move_iterator(VecIndex[i].end()));
		}*/


		/*int end1 = (n - 1) / 2;
		
		std::vector<unsigned int> vec1, vec2;
		std::thread thread1(ThreadIndexBuffer, &vec1, 0, end1, n);
		std::thread thread2(ThreadIndexBuffer, &vec2, end1, (n - 1), n);
		thread1.join();
		thread2.join();
		m_Index.insert(m_Index.end(), std::make_move_iterator(vec1.begin()), std::make_move_iterator(vec1.end()));
		m_Index.insert(m_Index.end(), std::make_move_iterator(vec2.begin()), std::make_move_iterator(vec2.end()));*/


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
}

void Graph3D::CalculateGraph()
{
	{
		//Timer t1("Calculate Graph");
		if (m_Expression->IsCompiled())
		{
			{
				//Timer t("Vertices Calc");
				m_Vertices = m_Expression->Calculate(m_Region, m_Side, m_Step); //Calculates the vertices of the Graph3D
			}

			{
				//Timer t("Index Calc");
				SetIndexBuffer();
			}

			SetOpenGLObjects();
		}
	}
		//std::cout << "----------------\n";
}

//void Graph3D::SetPos(const std::pair<double, double>& pos)
//{
//	m_Pos = pos; //Sets the Graph3D range to the new range (range)
//	unsigned int size = round(log10(abs(m_Range.first) + abs(m_Range.second)));
//	if (size == 0)
//	{
//		std::cout << "oi" << std::endl;
//	}
//	m_Step = 0.0001 * pow(10, size);
//	std::cout << size << std::endl;
//	m_Vertices = m_Expression->Calculate(m_Range, m_Step);//Re-calculates the vertice of the Graph3D
//
//	//Re - does the vertex buffer and vertex array
//	m_VBO = std::make_shared<VertexBuffer>(&m_Vertices[0], m_Vertices.size() * sizeof(double));
//	m_VBO->Bind();
//
//	m_VAO = std::make_shared<VertexArray>();
//	m_VAO->Bind();
//	VertexBufferAttLayout attLayout;
//	attLayout.Push<double>(2);
//	m_VAO->AddAtt(*m_VBO, attLayout);
//}

//Graph3D & Graph3D::operator=(const Graph3D & rhs)
//{
//	//Warning!!
//
//	//Make all member variables equals to the rhs member variables
//	m_Show = rhs.m_Show;
//	m_Expression = rhs.m_Expression;
//	m_Vertices = rhs.m_Vertices;
//	m_Pos = rhs.m_Pos;
//	m_Step = rhs.m_Step;
//	m_Shader = rhs.m_Shader;
//	m_VBO = rhs.m_VBO;
//	m_VAO = rhs.m_VAO;
//	m_Color = rhs.m_Color;
//	m_Side = rhs.m_Side;
//
//	return *this; //Return a dereferenced pointer
//}
