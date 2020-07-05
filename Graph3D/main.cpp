//Includes
#include <pch.h>
#include "Plane3D.h"
#include "Graph3D.h"
#include "Camera.h"
#include "ShaderProgram.h"

//Global Varialbes
unsigned int g_WIDTH = 1000; //Window Witdh
unsigned int g_HEIGHT = 1000; //Window Height

exprtk::parser<float> g_Parser; //Mathematical expression parser

GLFWwindow* g_Window;

std::vector<Graph3D> g_Graphs;

//Matrix
Camera g_Cam(glm::vec3(3.0f, -7.0f, 4.0f), glm::vec3(.0f, .0f, -1.0f), glm::vec3(.0f, 0.0f, 1.0f), 166.f , -25.f);
glm::mat4 g_Proj = glm::perspective(glm::radians(45.0f), (float)g_WIDTH / g_HEIGHT, 0.01f, 100.0f);

glm::vec4 GenRandomColor()
{
	glm::vec4 color;
	for (unsigned int i = 0; i < 3; i++)
	{
		color[i] = (float)(rand()) / (float)(RAND_MAX);
	}
	color.a = 1.f;
	return color;
}

size_t g_Alloc = 0;
size_t g_Delet = 0;

void* operator new(size_t size)
{
	g_Alloc += size;
	return malloc(size);
}

void operator delete(void* ptr, size_t size)
{
	g_Delet += size;
	free(ptr);
}


int main()
{
	srand((unsigned int)time(NULL));

	//GLFW Setup
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	g_Window = glfwCreateWindow(g_WIDTH, g_HEIGHT, "Graph3D", nullptr, nullptr);
	if (g_Window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(g_Window);
	glfwSwapInterval(0);//0 -> Vsync Off | 1 -> Vsync On
	glfwSetInputMode(g_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//GLAD Setup
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, g_WIDTH, g_HEIGHT); //Position coordinates -> Screen coordinates

	//ImGui Setup
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	{
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigDockingTabBarOnSingleWindows = true;
			

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(g_Window, true);
		ImGui_ImplOpenGL3_Init("#version 450");
	}


	//Line configuration
	glEnable(GL_LINE_SMOOTH); //Makes the line smoother
	glEnable(GL_BLEND); //Enables the Blend Function
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Blend Function config
	glLineWidth(2.f); //Line Width

	glPointSize(3.f);

	//Depth Test
	glEnable(GL_DEPTH_TEST);

	//smooth things
	float currentFrame = 0;
	float lastFrame = 0;
	float deltaTime = 0;

	//variables
	Plane3D plane;

	glm::vec3& camPos = g_Cam.GetCamPosRef();
	std::string expressionInput = "";
	glm::vec4 colorInput = GenRandomColor();
	glm::vec2 regionInput(0.f, 0.f);

	const char* LabelPosition = "Position##Input"; //Expression input label string
	const char* LabelTextInput = "Expression##Input"; //Expression input label string
	const char* LabelCompileButton = "Enter##Input"; //Enter button label string
	const char* LabelColor = "Color##Input"; //Enter button label string
	const char* LabelRegion = "Region##Input";

	const char* ImGuiName = "Graph3D Control";
	const char* NewGraphText = "New Graph Input:";

	bool ConfirmDelete = false;
	bool Deleting = false;
	unsigned int DeletingIndex = -1;

	glfwSetWindowFocusCallback(g_Window, [](GLFWwindow* window, int focused) 
	{
		if (focused)
		{
			glfwSetInputMode(g_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	});

	//Draw Loop
	while (!glfwWindowShouldClose(g_Window))
	{
		if (glfwGetKey(g_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetInputMode(g_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);


		//Background color
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//Plane and Graphs Draw
		currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		g_Cam.MouseMove();
		g_Cam.KeyboardMove(deltaTime);
		plane.Draw();

		for (const Graph3D& graph: g_Graphs)
		{
			graph.Draw();
		}

		//ImGui Draw
		ImGui::Begin(ImGuiName);

		ImGui::PushItemWidth(250.f);
		ImGui::InputFloat3(LabelPosition, &camPos[0]);
		ImGui::PopItemWidth();

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		//Graphs Control
		for (unsigned int i = 0; i < g_Graphs.size(); i++)
		{
			std::string del = "Delete##" + std::to_string(i);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(.7f, 0.15f, 0.15f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.f, 0.f, 0.f, 0.7f));
			if (ImGui::Button(del.c_str()))
			{
				DeletingIndex = i;
			}
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			if (DeletingIndex == i)
			{
				ImGui::SameLine();
				if (ImGui::Button("Confirm"))
				{
					//size_t temp = g_Delet;
					g_Graphs.erase(g_Graphs.begin() + DeletingIndex);
					//std::cout << "Deleted: " << g_Delet - temp << std::endl;

					for (unsigned int i = 0; i < g_Graphs.size(); i++)
					{
						g_Graphs[i].SetIds(i);
					}
					DeletingIndex = -1;
					continue;
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel"))
				{
					DeletingIndex = -1;
				}
			}

			g_Graphs[i].ImguiDraw();
		}

		//New Graph Input
		ImGui::Text(NewGraphText);
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::ColorEdit4(LabelColor, &colorInput[0], ImGuiColorEditFlags_NoInputs + ImGuiColorEditFlags_NoLabel);
		ImGui::SameLine();
		//Expression input
		ImGui::PushItemWidth(250.f);
		ImGui::PushID(LabelTextInput); //Pushs an ID for not enter a name in the ImGui::InputText()
		bool EnteringExp = ImGui::InputText("", &expressionInput, ImGuiInputTextFlags_EnterReturnsTrue);
		ImGui::PopID(); //ImGui PopID
		ImGui::PopItemWidth();
		ImGui::SameLine();
		if ((ImGui::Button(LabelCompileButton) || EnteringExp) && expressionInput != "") //ImGui Input Text
		{
			//size_t temp = g_Alloc;
			g_Graphs.emplace_back(expressionInput, colorInput, regionInput, g_Graphs.size());
			//std::cout << "Alloc: " << g_Alloc - temp << std::endl;

			expressionInput = "";
			colorInput = GenRandomColor();
			regionInput = glm::vec2(0.f, 0.f);
		}
		
		ImGui::PushItemWidth(250.f);
		ImGui::InputFloat2(LabelRegion, &regionInput[0]);
		ImGui::PopItemWidth();

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);//Frame Time and FPS
		ImGui::End();

		//ImGui Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//ImGui window backup
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		//Buffer
		glfwSwapBuffers(g_Window);

		//Events
		glfwPollEvents();
	}

	//ImGui finalization
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	//GLFW finalization
	glfwTerminate();

	g_Graphs.clear();
	//std::cout << g_Alloc << "\n" << g_Delet << std::endl;


	return 0;
}