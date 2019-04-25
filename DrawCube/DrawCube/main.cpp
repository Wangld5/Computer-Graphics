#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader_s.h"
#include "Camera.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace std;
using namespace ImGui;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//初始化ImGUI
	//-----------
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
	StyleColorsDark();

	//enable depth test
	//-----------------
	glEnable(GL_DEPTH_TEST);

	//生成着色器程序
	//--------------
	Shader ourShader("shader.vs", "shader.fs");

	//要用到的数据及存储
	//------------------
	float vertices[] = {
		// 背面
		-2.0f, -2.0f, -2.0f,  0.8f, 0.2f, 0.2f,
		2.0f, -2.0f, -2.0f,   0.8f, 0.2f, 0.2f,
		2.0f,  2.0f, -2.0f,   0.8f, 0.2f, 0.2f,
		2.0f,  2.0f, -2.0f,   0.8f, 0.2f, 0.2f,
		-2.0f,  2.0f, -2.0f,  0.8f, 0.2f, 0.2f,
		-2.0f, -2.0f, -2.0f,  0.8f, 0.2f, 0.2f,
		// 左
		-2.0f,  2.0f,  2.0f,  0.2f, 0.8f, 0.2f,
		-2.0f,  2.0f, -2.0f,  0.2f, 0.8f, 0.2f,
		-2.0f, -2.0f, -2.0f,  0.2f, 0.8f, 0.2f,
		-2.0f, -2.0f, -2.0f,  0.2f, 0.8f, 0.2f,
		-2.0f, -2.0f,  2.0f,  0.2f, 0.8f, 0.2f,
		-2.0f,  2.0f,  2.0f,  0.2f, 0.8f, 0.2f,
		// 正面
		-2.0f, -2.0f,  2.0f,  0.2f, 0.2f, 0.8f,
		2.0f, -2.0f,  2.0f,   0.2f, 0.2f, 0.8f,
		2.0f,  2.0f,  2.0f,   0.2f, 0.2f, 0.8f,
		2.0f,  2.0f,  2.0f,   0.2f, 0.2f, 0.8f,
		-2.0f,  2.0f,  2.0f,  0.2f, 0.2f, 0.8f,
		-2.0f, -2.0f,  2.0f,  0.2f, 0.2f, 0.8f,
		// 右
		2.0f,  2.0f,  2.0f,   0.2f, 0.8f, 0.8f,
		2.0f,  2.0f, -2.0f,   0.2f, 0.8f, 0.8f,
		2.0f, -2.0f, -2.0f,   0.2f, 0.8f, 0.8f,
		2.0f, -2.0f, -2.0f,   0.2f, 0.8f, 0.8f,
		2.0f, -2.0f,  2.0f,   0.2f, 0.8f, 0.8f,
		2.0f,  2.0f,  2.0f,   0.2f, 0.8f, 0.8f,
		// 下
		-2.0f, -2.0f, -2.0f,  0.8f, 0.2f, 0.8f,
		2.0f, -2.0f, -2.0f,   0.8f, 0.2f, 0.8f,
		2.0f, -2.0f,  2.0f,   0.8f, 0.2f, 0.8f,
		2.0f, -2.0f,  2.0f,   0.8f, 0.2f, 0.8f,
		-2.0f, -2.0f,  2.0f,  0.8f, 0.2f, 0.8f,
		-2.0f, -2.0f, -2.0f,  0.8f, 0.2f, 0.8f,
		// 上
		-2.0f,  2.0f, -2.0f,  0.8f, 0.8f, 0.2f,
		2.0f,  2.0f, -2.0f,   0.8f, 0.8f, 0.2f,
		2.0f,  2.0f,  2.0f,   0.8f, 0.8f, 0.2f,
		2.0f,  2.0f,  2.0f,   0.8f, 0.8f, 0.2f,
		-2.0f,  2.0f,  2.0f,  0.8f, 0.8f, 0.2f,
		-2.0f,  2.0f, -2.0f,  0.8f, 0.8f, 0.2f
	};

	//generate VAO and VBO
	//--------------------
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3 + 3) * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (3 + 3) * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	//bonus
	//-----
	float plantVertices[] = {
		// 背面
		-0.05f, -0.05f, -0.05f,  0.8f, 0.2f, 0.2f,
		0.05f, -0.05f, -0.05f,   0.8f, 0.2f, 0.2f,
		0.05f,  0.05f, -0.05f,   0.8f, 0.2f, 0.2f,
		0.05f,  0.05f, -0.05f,   0.8f, 0.2f, 0.2f,
		-0.05f,  0.05f, -0.05f,  0.8f, 0.2f, 0.2f,
		-0.05f, -0.05f, -0.05f,  0.8f, 0.2f, 0.2f,
		// 左
		-0.05f,  0.05f,  0.05f,  0.2f, 0.8f, 0.2f,
		-0.05f,  0.05f, -0.05f,  0.2f, 0.8f, 0.2f,
		-0.05f, -0.05f, -0.05f,  0.2f, 0.8f, 0.2f,
		-0.05f, -0.05f, -0.05f,  0.2f, 0.8f, 0.2f,
		-0.05f, -0.05f,  0.05f,  0.2f, 0.8f, 0.2f,
		-0.05f,  0.05f,  0.05f,  0.2f, 0.8f, 0.2f,
		// 正面
		-0.05f, -0.05f,  0.05f,  0.2f, 0.2f, 0.8f,
		0.05f, -0.05f,  0.05f,   0.2f, 0.2f, 0.8f,
		0.05f,  0.05f,  0.05f,   0.2f, 0.2f, 0.8f,
		0.05f,  0.05f,  0.05f,   0.2f, 0.2f, 0.8f,
		-0.05f,  0.05f,  0.05f,  0.2f, 0.2f, 0.8f,
		-0.05f, -0.05f,  0.05f,  0.2f, 0.2f, 0.8f,
		// 右
		0.05f,  0.05f,  0.05f,   0.2f, 0.8f, 0.8f,
		0.05f,  0.05f, -0.05f,   0.2f, 0.8f, 0.8f,
		0.05f, -0.05f, -0.05f,   0.2f, 0.8f, 0.8f,
		0.05f, -0.05f, -0.05f,   0.2f, 0.8f, 0.8f,
		0.05f, -0.05f,  0.05f,   0.2f, 0.8f, 0.8f,
		0.05f,  0.05f,  0.05f,   0.2f, 0.8f, 0.8f,
		// 下
		-0.05f, -0.05f, -0.05f,  0.8f, 0.2f, 0.8f,
		0.05f, -0.05f, -0.05f,   0.8f, 0.2f, 0.8f,
		0.05f, -0.05f,  0.05f,   0.8f, 0.2f, 0.8f,
		0.05f, -0.05f,  0.05f,   0.8f, 0.2f, 0.8f,
		-0.05f, -0.05f,  0.05f,  0.8f, 0.2f, 0.8f,
		-0.05f, -0.05f, -0.05f,  0.8f, 0.2f, 0.8f,
		// 上
		-0.05f,  0.05f, -0.05f,  0.8f, 0.8f, 0.2f,
		0.05f,  0.05f, -0.05f,   0.8f, 0.8f, 0.2f,
		0.05f,  0.05f,  0.05f,   0.8f, 0.8f, 0.2f,
		0.05f,  0.05f,  0.05f,   0.8f, 0.8f, 0.2f,
		-0.05f,  0.05f,  0.05f,  0.8f, 0.8f, 0.2f,
		-0.05f,  0.05f, -0.05f,  0.8f, 0.8f, 0.2f
	};

	//generate VAO and VBO
	//--------------------
	unsigned int vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plantVertices), plantVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3 + 3) * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (3 + 3) * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	//render loop control parameter
	//-----------------------------
	bool project = false;
	bool orthographic = false;
	bool perspect = false;
	bool viewChange = false;
	bool cameraMove = false;

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//create gui
		//----------
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		Begin("HW4");
		ImGui::Text("choose the mode you want");

		/*the project part*/
		//------------------
		//checkbox setting
		//----------------
		ImGui::Checkbox("Projection Mode", &project);
		ImGui::Checkbox("ViewChanging Mode", &viewChange);
		ImGui::Checkbox("camera can free to move", &cameraMove);
		ImGui::End();

		//activate shader
		//---------------
		ourShader.use();
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -15.0f)); 
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("view", view);
		ourShader.setMat4("projection", projection);

		// mode process
		//-------------
		//dealing with projection mode
		if (project) {
			model = glm::translate(model, glm::vec3(-1.5, 0.5, -1.5));
			ourShader.setMat4("model", model);
			Begin("choose orthographic projection or perspective projection");
			ImGui::Checkbox("orthographic projection", &orthographic);
			ImGui::Checkbox("perspective projection", &perspect);
			if (ImGui::Button("close"))
			{
				orthographic = false;
				perspect = false;
			}
			ImGui::End();
			if (perspect)
			{
				static float fov = 0.0f;
				static float rate = 0.0f;
				static float close = 0.0f;
				static float away = 0.0f;
				Begin("change the parameter");
				ImGui::Text("field of view");
				ImGui::SliderFloat("fov", &fov, 0.0f, 180.0f);
				ImGui::Text("rate of width and hight");
				ImGui::SliderFloat("rate", &rate, 0.0f, 10.0f);
				ImGui::Text("close distance");
				ImGui::SliderFloat("close", &close, 0.0f, 10.0f);
				ImGui::Text("far away distance");
				ImGui::SliderFloat("away", &away, 11.0f, 100.0f);
				ImGui::End();

				projection = glm::perspective(glm::radians(fov), rate, close, away);
				ourShader.setMat4("projection", projection);
			}
			else if (orthographic) {
				static float left = 0.0f;
				static float right = 0.0f;
				static float bottom = 0.0f;
				static float top = 0.0f;
				static float nearPar = 0.0f;
				static float farPar = 0.0f;
				Begin("change the parameter");
				ImGui::Text("left coordinate");
				ImGui::SliderFloat("left", &left, -10.0f, 10.0f);
				ImGui::Text("right coordinate");
				ImGui::SliderFloat("right", &right, -10.0f, 10.0f);
				ImGui::Text("top coordinate");
				ImGui::SliderFloat("top", &top, -10.0f, 10.0f);
				ImGui::Text("bottom coordinate");
				ImGui::SliderFloat("bottom", &bottom, -10.0f, 10.0f);
				ImGui::Text("near coordinate");
				ImGui::SliderFloat("near", &nearPar, 0.0f, 800.0f);
				ImGui::Text("far coordinate");
				ImGui::SliderFloat("far", &farPar, 0.0f, 800.0f);
				ImGui::End();

				view = glm::lookAt(
					glm::vec3(0.0f, 0.0f, -15.0f),
					glm::vec3(0.0f, 0.0f, 0.0f),
					glm::vec3(0.0f, 1.0f, 0.0f)
				);
				ourShader.setMat4("view", view);
				projection = glm::ortho(left, right, top, bottom, nearPar, farPar);
				ourShader.setMat4("projection", projection);
			}

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//dealing with view changing
		if (viewChange) {
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
			ourShader.setMat4("model", model);
			float radius = 10.0f;
			float camX = sin(glfwGetTime()) * radius;
			float camZ = cos(glfwGetTime()) * radius;
			glm::mat4 view;
			view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
			ourShader.setMat4("view", view);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//camera can move free
		if (cameraMove) {
			ourShader.setMat4("model", model);
			view = camera.getViewMatrix();
			ourShader.setMat4("view", view);
			projection = glm::perspective(glm::radians(camera.getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			ourShader.setMat4("projection", projection);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		
		/*---------------------------------------------------------------------------------*/

		//render window
		//-------------
		Render();
		ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//end
	//---
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyBoard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyBoard(BACK, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyBoard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyBoard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.processMouseMovement(xoffset, yoffset, true);
	
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.processMouseScroll(yoffset);
}



