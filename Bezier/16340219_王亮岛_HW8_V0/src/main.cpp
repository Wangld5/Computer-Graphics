#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader_s.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

using namespace std;
using namespace ImGui;
const unsigned int SCR_WIDTH = 750;
const unsigned int SCR_HEIGHT = 750;
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
int zuhe(int c, int k);
int JieCheng(int n);
glm::vec2 Q(double t);


//global param
vector<glm::vec2> p;
bool run = false;
double running_t = 0.0f;


int main() {
	 //glfw: initialize and configure
	 //------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	 //glfw window creation
	 //--------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	 //glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//��ʼ��ImGUI
	//-----------
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
	StyleColorsDark();

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	//shader
	Shader curveShader(".\\Shader\\curve.vs", ".\\Shader\\curve.fs");

	//VAO,VBO
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	curveShader.use();

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		int scr_width, scr_height;
		glfwGetFramebufferSize(window, &scr_width, &scr_height);
		glViewport(0, 0, scr_width, scr_height);
		//create gui
		//----------
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		Begin("HW8");
		ImGui::Text("use left mouse button to set control point");
		ImGui::Text("use right mouse button to remove control point");
		ImGui::Text("use space to see the process");
		ImGui::End();

		curveShader.setVec3("curveColor", glm::vec3(0.7, 0.3, 0.3));
		curveShader.use();
		//��ֱ�߻�����
		glPointSize(10.0f);
		vector<float> controlPoints;
		for (int i = 0; i < p.size(); ++i) {
			vector<float> renderP = { p[i].x, p[i].y, 0.0f };
			controlPoints.insert(controlPoints.end(), renderP.begin(), renderP.end());
			//һ��һ����
			if (i > 0) {
				float* ptr = controlPoints.data() + (i - 1) * 3;
				glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), ptr, GL_STATIC_DRAW);
				glDrawArrays(GL_LINES, 0, 2);
			}
		}
		//�������Ƶ�
		float *ptr = controlPoints.data();
		glBufferData(GL_ARRAY_BUFFER, controlPoints.size() * sizeof(float), ptr, GL_STATIC_DRAW);
		glDrawArrays(GL_POINTS, 0, p.size());

		//bezier ����
		if (p.size() > 2)
		{
			vector<float> bezierPoints;
			int count = 0;
			for (double i = 0.0f; i < 1.0f; i+=0.0005)
			{
				glm::vec2 Bpoint(Q(i));
				vector<float> point{
					Bpoint.x, Bpoint.y, 0.0f
				};
				bezierPoints.insert(bezierPoints.end(), point.begin(), point.end());
				if (count > 0)
				{
					float *ptr = bezierPoints.data() + (count - 1) * 3;
					glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), ptr, GL_STATIC_DRAW);
					glDrawArrays(GL_LINES, 0, 2);
				}
				count++;
			}

			//���ж���
			if (run)
			{
				vector<glm::vec2> runningPoints(p);
				while (runningPoints.size() > 1)
				{
					vector<glm::vec2> nextLevel;
					for (int i = 0; i < runningPoints.size()-1; i++)
					{
						float presPoint[] = {
							runningPoints[i].x, runningPoints[i].y, 0.0f,
							runningPoints[i + 1].x, runningPoints[i + 1].y, 0.0f
						};
						glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), presPoint, GL_STATIC_DRAW);
						glDrawArrays(GL_LINES, 0, 2);
						glDrawArrays(GL_POINTS, 0, 2);
						glm::vec2 nextPoint;
						nextPoint.x = (1-running_t)*runningPoints[i].x + (running_t) * runningPoints[i + 1].x;
						nextPoint.y = (1-running_t)*runningPoints[i].y + (running_t) * runningPoints[i + 1].y;
						nextLevel.push_back(nextPoint);
					}
					runningPoints = nextLevel;
					cout << runningPoints.size() << endl;
				}
				running_t += 0.005f;
				if (running_t >= 1.0f)
				{
					run = false;
				}
			}

		}
		else {
			run = false;
		}


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		run = true;
		running_t = 0.0f;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (run)
	{
		return;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		p.push_back(glm::vec2((float(xpos) / float(SCR_WIDTH) * 2.0f) - 1, -((float(ypos) / float(SCR_HEIGHT) * 2.0f) - 1)));
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {

		p.pop_back();
	}
}

int JieCheng(int n) {
	if (n == 1 || n==0) return 1;
	return n * JieCheng(n - 1);
}

//�������
int zuhe(int n, int k) {
	return JieCheng(n) / (JieCheng(k)*(JieCheng(n - k)));
}

//��Q(t)
glm::vec2 Q(double t) {
	int n = p.size() - 1;
	double x = 0, y = 0;
	double p1 = pow((1 - t), n), p2 = 1;
	for (int i = 0; i <= n; i++) {
		x += zuhe(n, i)*p1*p2*p[i].x;
		y += zuhe(n, i)*p1*p2*p[i].y;
		p1 /= (1 - t);
		p2 *= t;
	}
	return glm::vec2(x, y);
}