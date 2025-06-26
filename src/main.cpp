#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

int main() {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW.\n";
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(800, 600, "Boundforge", nullptr, nullptr);
	if (!window) {
		std::cerr << "Failed to create window.\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window)) {
		float time = glfwGetTime();
		glClearColor((sin(time) + 1.0f) / 2.0f, (sin(time * 2.0f) + 1.0f) / 2.0f, (sin(time * 4.0f) + 1.0f) / 2.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
