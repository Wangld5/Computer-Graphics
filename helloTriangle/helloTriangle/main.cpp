#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

using namespace std;
using namespace ImGui;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 myColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0f);\n"
"	myColor = aColor;\n"
"}\n\0";
const char *fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 myColor;"
"void main(){\n"
"FragColor = vec4(myColor, 1.0f);\n"
"}\n\0";


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {
	//initialize a window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//new a window object
	GLFWwindow* window = glfwCreateWindow(800, 600, "learn opengl", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//initialize glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "fail to initialize glad" << endl;
		return -1;
	}
	//初始化ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
	StyleColorsDark();



	/*float vertice[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};*/

	//顶点数组对象VAO

	//顶点着色器
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//判断编译是否成功
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//片段着色器
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//着色器程序
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//判断链接是否失败
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "Error: link failed " << infoLog << endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//初始化数据
	ImVec4 top_color = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
	ImVec4 left_color = ImVec4(1.0f, 0.0f, 0.0f, 1.00f);
	ImVec4 right_color = ImVec4(0.0f, 0.0f, 1.0f, 1.00f);
	ImVec4 bottom_color = ImVec4(0.0f, 1.0f, 1.0f, 1.00f);
	ImVec4 same_color = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
	bool ImGui = true;
	bool the_same_color = false;
	bool draw_trangle_without_render = false;
	bool draw_trangle = false;
	bool bonus_draw_line = false;
	bool bonus_draw_another_trangle = false;
	unsigned int VBO, VAO, EBO;

	//render loop
	while (!glfwWindowShouldClose(window))
	{
		//input
		/*processInput(window);*/

		//绘制物体
		/*glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);*/

		if (the_same_color)
		{
			top_color = same_color;
			left_color = same_color;
			right_color = same_color;
			bottom_color = same_color;
		}

		float vertices[] = {
			0.2f, -0.2f, 0.0f,  right_color.x, right_color.y, right_color.z,  // bottom right
			-0.2f, -0.2f, 0.0f, left_color.x, left_color.y, left_color.z,  // bottom left
			0.0f,  0.2f, 0.0f,  top_color.x, top_color.y, top_color.z,   // top 
			0.0f,  -0.8f, 0.0f,  bottom_color.x, bottom_color.y, bottom_color.z,   // bottom

			0.25f, 0.5f, 0.0f,  right_color.x, right_color.y, right_color.z,   // 左上
			0.25f, -0.5f, 0.0f,  top_color.x, top_color.y, top_color.z,   // 左下
			0.75f, -0.5f, 0.0f,  left_color.x, left_color.y, left_color.z,   // 右下
			0.75f,  0.5f, 0.0f,  bottom_color.x, bottom_color.y, bottom_color.z,
			//points
			0.5f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
			0.5f, 0.4f, 0.0f,  0.0f, 1.0f, 0.0f,
			0.5f, 0.3f, 0.0f,  0.0f, 1.0f, 0.0f,
			0.5f, 0.2f, 0.0f,  0.0f, 1.0f, 0.0f,
			0.5f, 0.1f, 0.0f,  0.0f, 1.0f, 0.0f,
			0.5f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
			0.5f, -0.1f, 0.0f,  0.0f, 1.0f, 0.0f,
			0.5f, -0.2f, 0.0f,  0.0f, 1.0f, 0.0f,
			0.5f, -0.3f, 0.0f,  0.0f, 1.0f, 0.0f,
			0.5f, -0.4f, 0.0f,  0.0f, 1.0f, 0.0f,
			0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f
		};

		unsigned int indices[] = { // 注意索引从0开始
			0, 1, 2, // 第一个三角形
			0, 1, 3  // 第二个三角形
		};

		//生成VAO，VBO，EBO对象
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		//绑定顶点数组
		glBindVertexArray(VAO);
		//将顶点数组复制进入缓冲
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		//将索引数组复制进缓冲
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		//设置顶点属性指针
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		//设置颜色属性指针
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		/*glEnableVertexAttribArray(1);*/
		glUseProgram(shaderProgram);


		////创建ImGUI
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		Begin("Edit color", &ImGui, ImGuiWindowFlags_MenuBar);
		ImGui::ColorEdit3("Basic triangle -- top color", (float*)&top_color);
		ImGui::ColorEdit3("Basic triangle -- left color", (float*)&left_color);
		ImGui::ColorEdit3("Basic triangle -- right color", (float*)&right_color);
		ImGui::ColorEdit3("Optional -- the same color", (float*)&same_color);

		ImGui::Checkbox("the same color", &the_same_color);
		ImGui::Checkbox("Draw triangle without rendering", &draw_trangle_without_render);
		ImGui::Checkbox("Basic draw triangle", &draw_trangle);
		ImGui::Checkbox("Bonus draw line", &bonus_draw_line);
		ImGui::Checkbox("Bonus draw another triangle", &bonus_draw_another_trangle);
		ImGui::End();

		//渲染窗口颜色
		int view_width, view_height;
		glfwGetFramebufferSize(window, &view_width, &view_height);
		glViewport(0, 0, view_width, view_height);		
		Render();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
		//基本三角形
		if (draw_trangle_without_render) {
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		// 渲染Basic 三角形
		if (draw_trangle) {
			glEnableVertexAttribArray(1);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		//渲染bonus线条和点
		if (bonus_draw_line)
		{
			glEnableVertexAttribArray(1);
			glBindVertexArray(VAO);
			glDrawArrays(GL_LINE_STRIP, 4, 4);
			glDrawArrays(GL_POINTS, 8, 11);
		}

		//渲染bonus三角形
		if(bonus_draw_another_trangle) {
			glEnableVertexAttribArray(1);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}


		//glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;

}