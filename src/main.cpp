#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include "shader.h" // not Shader.hpp
using boundforge::Shader;

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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD.\n";
        return -1;
    }

    // Triangle vertices (in NDC)
    float vertices[] = {
        // X, Y
         0.0f,  0.5f,
        -0.5f, -0.5f,
         0.5f, -0.5f
    };

    // Set up VAO and VBO
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Only position (2D)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Shader (assumes shaders exist in `build/shaders/`)
    Shader shader("shaders/basic.vert", "shaders/basic.frag");

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        float time = glfwGetTime();

        glClearColor((sin(time) + 1.0f) / 2.0f,
                     (sin(time * 2.0f) + 1.0f) / 2.0f,
                     (sin(time * 4.0f) + 1.0f) / 2.0f,
                     1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    shader.~Shader();

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

