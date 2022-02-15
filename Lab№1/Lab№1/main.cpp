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

#include <iostream>
#include <random>
#include <iomanip>
#include <map>
#include <stdexcept>

#include "c_matrix.h"
#include "character.h"
#include "shader.h"
#include "lib_graph.h"

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

void processInput(GLFWwindow* window);
void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);
void RenderMatrix(Shader& shader, float x, float y, float scale, glm::vec3 color, c_matrix matrix);

std::map<char, character> characters;
unsigned int VAO, VBO;

int main() {
	try {
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

		FT_Library ft;
		if (FT_Init_FreeType(&ft)) {
			std::cout << "Could not init FreeType Library" << std::endl;
			return -1;
		}

		FT_Face face;
		if (FT_New_Face(ft, "fonts/JetBrainsMono-Regular.ttf", 0, &face)) {
			std::cout << "Failed to load font" << std::endl;
			return -1;
		}
		else {
			FT_Set_Pixel_Sizes(face, 0, 14);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			for (unsigned int c = 0; c < 256; c++) {
				if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
					std::cout << "Failed to load Glyph" << std::endl;
					continue;
				}

				unsigned int texture;
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RED,
					face->glyph->bitmap.width,
					face->glyph->bitmap.rows,
					0,
					GL_RED,
					GL_UNSIGNED_BYTE,
					face->glyph->bitmap.buffer
				);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				character ch = {
					texture,
					glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
					static_cast<unsigned int>(face->glyph->advance.x)
				};
				characters.insert(std::pair<char, character>(c, ch));
			}

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		c_matrix A(3, 3);
		c_matrix B(3, 3);
		c_matrix V1(3, 1);
		c_matrix V2(3, 1);
		c_matrix C1(3, 3);
		c_matrix C2(3, 3);
		c_matrix D(3, 1);
		c_matrix Q(1, 1);
		c_matrix P(1, 1);
		c_matrix VP(3, 1);
		c_matrix SP(3, 1);
		A.Random();
		B.Random();
		V1.Random();
		V2.Random();

		C1 = A + B;
		C2 = A * B;
		D = A * V1;
		Q = V1.Transpose() * V2;
		P = V1.Transpose() * A * V2;

		VP = vectorProduct(V1, V2);
		double scalar = scalarProduct(V1, V2);
		double MV1 = moduleVector(V1);
		double MV2 = moduleVector(V2);
		double cosV1V2 = cosVectors(V1, V2);
		SP = sphereToCart(V1);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460");
		ImGui::MyStyle();
		std::string font_path = "fonts/Inter-VariableFont_slnt,wght.ttf";
		ImGui::GetIO().Fonts->AddFontFromFileTTF(font_path.c_str(), 14.0f);

		bool matrix_test = true;
		bool functions_test = false;

		while (!glfwWindowShouldClose(window)) {
			processInput(window);

			glClearColor(0.9f, 0.9f, 0.8f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			if (matrix_test) {
				RenderText(shader, "A:", 10.0f, 680.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
				RenderMatrix(shader, 10.0f, 680.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), A);
				RenderText(shader, "B:", 10.0f, 580.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
				RenderMatrix(shader, 10.0f, 580.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), B);
				RenderText(shader, "V1:", 10.0f, 480.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
				RenderMatrix(shader, 10.0f, 480.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), V1);
				RenderText(shader, "V2:", 10.0f, 380.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
				RenderMatrix(shader, 10.0f, 380.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), V2);

				RenderText(shader, "C1 = A + B:", 300.0f, 680.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
				RenderMatrix(shader, 300.0f, 680.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), C1);

				RenderText(shader, "C1 = A * B:", 300.0f, 580.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
				RenderMatrix(shader, 300.0f, 580.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), C2);

				RenderText(shader, "D = A * V1:", 300.0f, 480.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
				RenderMatrix(shader, 300.0f, 480.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), D);

				RenderText(shader, "Q = V1.Transpose() * V2:", 300.0f, 380.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
				RenderMatrix(shader, 300.0f, 380.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), Q);

				RenderText(shader, "P = V1.Transpose() * A * V2:", 300.0f, 320.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
				RenderMatrix(shader, 300.0f, 320.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), P);
			}
			else if (functions_test) {
				RenderText(shader, "V1:", 10.0f, 680.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
				RenderMatrix(shader, 10.0f, 680.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), V1);
				RenderText(shader, "V2:", 10.0f, 580.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
				RenderMatrix(shader, 10.0f, 580.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), V2);

				RenderText(shader, "vectorProduct(V1, V2):", 300.0f, 680.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
				RenderMatrix(shader, 300.0f, 680.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), VP);
				RenderText(shader, "scalarProduct(V1, V2): " + std::to_string(scalar), 300.0f, 580.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
				RenderText(shader, "moduleVector(V1): " + std::to_string(MV1), 300.0f, 560.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
				RenderText(shader, "moduleVector(V2): " + std::to_string(MV2), 300.0f, 540.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
				RenderText(shader, "cosVectors(V1, V2): " + std::to_string(cosV1V2), 300.0f, 520.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
				RenderText(shader, "sphereToCart(V1):", 300.0f, 480.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
				RenderMatrix(shader, 300.0f, 480.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), SP);
			}

			if (ImGui::BeginMainMenuBar()) {
				if (ImGui::BeginMenu("File")) {
					if (ImGui::MenuItem("Exit")) {
						glfwSetWindowShouldClose(window, true);
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Tests")) {
					if (ImGui::MenuItem("Matrix")) {
						matrix_test = true;
						functions_test = false;
					}
					if (ImGui::MenuItem("Functions")) {
						functions_test = true;
						matrix_test = false;
					}
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwTerminate();
	}
	catch (std::out_of_range ex) {
		std::cerr << ex.what() << std::endl;
	}
	catch (std::invalid_argument ex) {
		std::cerr << ex.what() << std::endl;
	}
}

void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color) {
	shader.use();
	glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		character ch = characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};

		glBindTexture(GL_TEXTURE_2D, ch.TextureID);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (ch.Advance >> 6) * scale; 
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderMatrix(Shader& shader, float x, float y, float scale, glm::vec3 color, c_matrix matrix) {
	std::string string_row = "";
	std::string string_element = "";
	float y_scale = y - 20.0f;
	for (const auto& row : matrix.GetMatrix()) {
		for (const auto& element : row) {
			string_element = std::to_string(round(element * 100) / 100);
			string_element.erase(string_element.find_last_not_of('0') + 1, std::string::npos);
			if (string_element.ends_with('.'))
				string_element += "0";
			while (string_element.length() < 10)
				string_element += " ";
			string_row += string_element;
		}
		RenderText(shader, string_row, x, y_scale, scale, color);
		y_scale -= 20.0f;
		string_row.clear();
	}
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}