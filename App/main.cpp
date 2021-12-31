#include "GLContext/GLContext.h"
#include "InputSystem/InputSystem.h"
#include "InputSystem/window.h"
#include "InputSystem/Mouse.h"
#include "InputSystem/Keyboard.h"
#include "GameTimer/GameTimer.h"
#include <glad/glad.h>
#include <gl/GL.h>
#include <iostream>

const char *pVsShaderCode = "						\n\
	#version 330 core								\n\
	layout(location = 0) in vec3 inPosition;		\n\
	layout(location = 1) in vec3 inColor;			\n\
	out vec3 ourColor;								\n\
	void main() {									\n\
		gl_Position = vec4(inPosition, 1);			\n\
		ourColor = inColor;							\n\
	}												\n\
";

const char *pPsShaderCode = "						\n\
	#version 330 core								\n\
	out vec4 outColor;								\n\
	in vec3 ourColor;								\n\
	void main() {									\n\
		outColor = vec4(ourColor, 1.0);				\n\
	}												\n\
";

void initGlad();
GLuint compileShader();

int main() {
	std::shared_ptr<com::InputSystem> pInputSystem = std::make_shared<com::InputSystem>("Test", 800, 600);
	std::shared_ptr<com::GameTimer> pGameTimer = std::make_shared<com::GameTimer>();
	com::GLContext context(pInputSystem->window->getHWND());
	initGlad();

	std::vector<float> vertices = {
		// position					color
		-0.5f, -0.5f, +0.0f,  +1.0f, +0.0f, +0.0f,
		+0.0f, +0.5f, +0.0f,  +0.0f, +1.0f, +0.0f,
		+0.5f, -0.5f, +0.0f,  +0.0f, +0.0f, +1.0f,
	};

	GLuint VBO;
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	{
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
		// position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)0);
		// color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(sizeof(float) * 3));
	}
	glBindVertexArray(0);

	GLuint shaderProgram = compileShader();
	if (shaderProgram == -1)
		exit(EXIT_FAILURE);

	glUseProgram(shaderProgram);
	while (!pInputSystem->window->shouldClose()) {
		pGameTimer->nextFrame();

		pInputSystem->beginTick(pGameTimer);
		glClearColor(0.88f, 1.0f, 1.0f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		pInputSystem->tick(pGameTimer);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// todo ...
		pInputSystem->endTick(pGameTimer);
		context.swapBuffers();
	}

	glDeleteShader(shaderProgram);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	return 0;
}

void initGlad() {
	if (!gladLoadGL()) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		exit(EXIT_FAILURE);
	}
}


GLuint compileShader() {
	int success = 0;
	char infoLog[512] = { '\0' };

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &pVsShaderCode, nullptr);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, sizeof(infoLog), nullptr, infoLog);
		std::cerr << infoLog << std::endl;
		return -1;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &pPsShaderCode, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, sizeof(infoLog), nullptr, infoLog);
		std::cerr << infoLog << std::endl;
		return -1;
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), nullptr, infoLog);
		std::cerr << infoLog << std::endl;
		return -1;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}


