#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <random>
#include <iomanip>
#include <map>
#include <stdexcept>

#include "shader.h"

void MouseButtonCallback(GLFWwindow* window, GLuint button, GLuint action, bool &is_pressed);

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);;

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "First lab", nullptr, nullptr);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
		});

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader shader("text.vs", "text.fs");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(WIDTH), 0.0f, static_cast<float>(HEIGHT));
	shader.use();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	bool is_pressed = false;

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.9f, 0.9f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			MouseButtonCallback(window, GLFW_MOUSE_BUTTON_LEFT,
				GLFW_PRESS, is_pressed);
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) ==
			GLFW_RELEASE) {
			MouseButtonCallback(window, GLFW_MOUSE_BUTTON_LEFT,
				GLFW_RELEASE, is_pressed);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}

void MouseButtonCallback(GLFWwindow* window, GLuint button, GLuint action, bool &is_pressed) {
	double x_pos;
	double y_pos;
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && is_pressed) {
		glfwGetCursorPos(window, &x_pos, &y_pos);
		is_pressed = false;
		std::cout << "Mouse clicked at: " << x_pos << ", " << y_pos << std::endl;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !is_pressed) {
		glfwGetCursorPos(window, &x_pos, &y_pos);
		std::cout << "Mouse dragged at: " << x_pos << ", " << y_pos << std::endl;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !is_pressed) {
		glfwGetCursorPos(window, &x_pos, &y_pos);
		is_pressed = true;
		std::cout << "Mouse released at: " << x_pos << ", " << y_pos << std::endl;
	}
}