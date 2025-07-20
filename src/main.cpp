#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <cstring>
#include "Shader.hpp"
#include "scene/Entity.hpp"
#include "scene/Scene.hpp"

#define PI 3.14159265358979323846264338327950288

using boundforge::Shader;
using boundforge::Entity;
using boundforge::Scene;

// Simple test entity with position and radius
class TestEntity : public Entity {
public:
    glm::vec3 position;
    float radius;
    glm::vec3 emissive;
    glm::vec3 albedo;

    TestEntity(const glm::vec3& pos, float r, glm::vec3 emi, glm::vec3 alb)
        : Entity(nullptr), position(pos), radius(r), emissive(emi), albedo(alb) {}
};

// Camera state
glm::vec3 camPos(0.0f, 0.0f, -3.0f);
float camYaw = 0.0f;   // Yaw in radians
float camPitch = 0.0f; // Pitch in radians
const float moveSpeed = 3.0f; // units per second
const float rotSpeed = glm::radians(60.0f); // radians per second

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

    float vertices[] = {
        -1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f
    };
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Shader shader("shaders/screen_centered.vert", "shaders/basic.frag");

    // --- Entity/Scene setup ---
    Scene scene;
    scene.addEntity(std::make_unique<TestEntity>(
        glm::vec3(0.0f, 0.0f, 5.0f), 1.0f, glm::vec3(2.0f, 1.5f, 0.2f), glm::vec3(1.0f, 0.8f, 0.2f))); // Sun (emissive)
    scene.addEntity(std::make_unique<TestEntity>(
        glm::vec3(3.0f, 0.0f, 5.0f), 0.3f, glm::vec3(0.0f), glm::vec3(0.2f, 0.5f, 1.0f))); // Blue planet
    scene.addEntity(std::make_unique<TestEntity>(
        glm::vec3(-2.0f, 1.0f, 6.0f), 0.5f, glm::vec3(0.0f), glm::vec3(0.8f, 0.2f, 0.2f))); // Red planet
    scene.addEntity(std::make_unique<TestEntity>(
        glm::vec3(0.0f, -1.5f, 4.5f), 0.4f, glm::vec3(0.0f, 0.8f, 0.0f), glm::vec3(0.2f, 1.0f, 0.2f))); // Green glowing

    const int MAX_ENTITIES = 32;
    std::vector<float> entityData(4 + 4 * MAX_ENTITIES * 3, 0.0f); // 4 for count+padding, then 3x vec4 arrays

    GLuint entityUBO;
    glGenBuffers(1, &entityUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, entityUBO);

    float lastTime = glfwGetTime();

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // --- Camera movement ---
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camPos += glm::vec3(sin(camYaw), 0, cos(camYaw)) * moveSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camPos -= glm::vec3(sin(camYaw), 0, cos(camYaw)) * moveSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camPos -= glm::normalize(glm::cross(glm::vec3(sin(camYaw), 0, cos(camYaw)), glm::vec3(0,1,0))) * moveSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camPos += glm::normalize(glm::cross(glm::vec3(sin(camYaw), 0, cos(camYaw)), glm::vec3(0,1,0))) * moveSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camPos += glm::normalize(glm::vec3(0.0, 1.0, 0.0)) * moveSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
            camPos -= glm::normalize(glm::vec3(0.0, 1.0, 0.0)) * moveSpeed * deltaTime;

        // --- Camera rotation ---
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            camYaw += rotSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            camYaw -= rotSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            camPitch += rotSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            camPitch -= rotSpeed * deltaTime;
        camPitch = glm::clamp(
            camPitch,
            static_cast<float>(-PI * 0.5 + 0.01),
            static_cast<float>(PI * 0.5 - 0.01)
        );

        // Calculate camera direction
        glm::vec3 camDir;
        camDir.x = cos(camPitch) * sin(camYaw);
        camDir.y = sin(camPitch);
        camDir.z = cos(camPitch) * cos(camYaw);
        camDir = glm::normalize(camDir);

        glClearColor(1.0, 0.0, 1.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Gather entity data for rendering
        const auto& entities = scene.getEntities();
        int numEntities = entities.size();
        memcpy(&entityData[0], &numEntities, sizeof(int));

        // Fill spheres, emissive, and albedo arrays
        for (size_t i = 0; i < entities.size(); ++i) {
            auto* testEnt = dynamic_cast<TestEntity*>(entities[i].get());
            if (testEnt) {
                // spheres (vec4)
                entityData[4 + i * 4 + 0] = testEnt->position.x;
                entityData[4 + i * 4 + 1] = testEnt->position.y;
                entityData[4 + i * 4 + 2] = testEnt->position.z;
                entityData[4 + i * 4 + 3] = testEnt->radius;
                // emissive (vec4)
                entityData[4 + (MAX_ENTITIES * 4) + i * 4 + 0] = testEnt->emissive.x;
                entityData[4 + (MAX_ENTITIES * 4) + i * 4 + 1] = testEnt->emissive.y;
                entityData[4 + (MAX_ENTITIES * 4) + i * 4 + 2] = testEnt->emissive.z;
                entityData[4 + (MAX_ENTITIES * 4) + i * 4 + 3] = 0.0f;
                // albedo (vec4)
                entityData[4 + (MAX_ENTITIES * 8) + i * 4 + 0] = testEnt->albedo.x;
                entityData[4 + (MAX_ENTITIES * 8) + i * 4 + 1] = testEnt->albedo.y;
                entityData[4 + (MAX_ENTITIES * 8) + i * 4 + 2] = testEnt->albedo.z;
                entityData[4 + (MAX_ENTITIES * 8) + i * 4 + 3] = 0.0f;
            }
        }

        glBufferData(GL_UNIFORM_BUFFER, entityData.size() * sizeof(float), entityData.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, entityUBO);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, entityUBO);

        // Render
        shader.use();
        shader.setFloat("uAspect", (float)800 / (float)600);
        shader.setVec3("uCamPos", camPos);
        shader.setVec3("uCamDir", camDir);

        // Set emission falloff power (try 2.0f for inverse-square, or experiment)
        shader.setFloat("emissionFalloffPower", 2.0f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &entityUBO);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

