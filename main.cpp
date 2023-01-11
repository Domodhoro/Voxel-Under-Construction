#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <algorithm>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "./lib/stb_image.h"

#include "./lib/FastNoiseLite.h"

constexpr auto WINDOW_WIDTH {800}, WINDOW_HEIGHT {600}, FPS {60};

#include "./src/camera.hpp"
#include "./src/shader.hpp"
#include "./src/stb_image_wrapper.h"
#include "./src/chunkMesh.hpp"
#include "./src/chunk.hpp"

Camera::Camera camera;

void keyboardCallback(GLFWwindow *window);
void mouseCallback(GLFWwindow *window, double x, double y);

int main(int argc, char *argv[]) {
    GLFWwindow *window {nullptr};

    try {
        if (glfwInit() == GLFW_NOT_INITIALIZED) {
            throw std::string {"Falha ao iniciar o GLFW."};
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_RESIZABLE, false);

        window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Voxel", nullptr, nullptr);

        if (window == nullptr) {
            throw std::string {"Falha ao criar a janela de visualização."};
        }

        glfwMakeContextCurrent(window);

        auto framebufferSizeCallback = [](GLFWwindow *window, int width, int height) -> void {
            glViewport(0, 0, width, height);
        };

        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        glfwSetWindowPos(window, 100, 100);

        STB_IMAGE::loadWindowIcon(window, "./img/icon.bmp");

        glewExperimental = true;

        if (glewInit() != GLEW_OK) {
            throw std::string {"Falha ao iniciar GLEW."};
        }

        glfwSetCursorPosCallback(window, mouseCallback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        const GLFWvidmode *mode {
            glfwGetVideoMode(glfwGetPrimaryMonitor())
        };

        camera.setSpeed(0.5f);
        camera.setFov(60.0f);
        camera.setAspect(static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT));
        camera.setPosition(glm::tvec3<float>(0.0f, 0.0f, -5.0f));

        const auto seed {1007};

        FastNoiseLite noise {seed};

        Chunk::Chunk chunks {0, 0, noise};

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);

        auto lastFrame {0.0f}, currentFrame {0.0f};

        while (!glfwWindowShouldClose(window)) {
            currentFrame = static_cast<float>(glfwGetTime());

            if ((currentFrame - lastFrame) > (1.0f / static_cast<float>(FPS))) {
                glfwSetWindowPos(window, (mode->width - WINDOW_WIDTH) / 2, (mode->height - WINDOW_HEIGHT) / 2);

                keyboardCallback(window);

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glClearColor(0.5f, 0.5f, 1.0f, 1.0f);

                auto view {camera.getViewMatrix()}, projection {camera.getProjectionMatrix()};

                chunks.draw(view, projection);

                glfwSwapBuffers(window);
                glfwPollEvents();

                lastFrame = currentFrame;
            }
        }

        glfwDestroyWindow(window);
        glfwTerminate();

    } catch (const std::string& error) {
        std::ofstream file {"./log.txt"};

        file << "Erro: " << error;

        file.close();

        glfwDestroyWindow(window);
        glfwTerminate();

        return 1;
    }

    return 0;
}

void keyboardCallback(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.keyboardProcess(Camera::MOVEMENTS::FORWARD);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.keyboardProcess(Camera::MOVEMENTS::BACKWARD);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.keyboardProcess(Camera::MOVEMENTS::RIGHT);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.keyboardProcess(Camera::MOVEMENTS::LEFT);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.getPosition().y += 0.5f;
    }
}

void mouseCallback(GLFWwindow *window, double x, double y) {
    static auto firstMouse {true};
    static auto lastX {0.0f}, lastY {0.0f};

    if (firstMouse) {
        lastX = static_cast<float>(x);
        lastY = static_cast<float>(y);

        firstMouse = false;
    }

    auto offSetX {x - lastX}, offSetY {lastY - y};

    lastX = x;
    lastY = y;

    const auto sensitivity {0.1f};

    offSetX *= sensitivity;
    offSetY *= sensitivity;

    camera.mouseProcess(&offSetX, &offSetY);
}
