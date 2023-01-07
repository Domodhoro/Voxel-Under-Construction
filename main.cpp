/*
    MIT License

    Copyright (c) 2022 Guilherme M. Aguiar (guilhermemaguiar2022@gmail.com)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <sstream>
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

#include "./src/camera.hpp"
#include "./src/shader.hpp"
#include "./src/stb_image_wrapper.h"
#include "./src/mesh.hpp"
#include "./src/chunk.hpp"

auto camera {
    std::make_unique<Camera::Camera>()
};

void keyboardCallback(GLFWwindow *window);
void mouseCallback(GLFWwindow *window, double x, double y);

int main(int argc, char *argv[]) {
    std::cout << argv[0] << std::endl;

    GLFWwindow *window {nullptr};

    try {
        constexpr auto WINDOW_WIDTH {800}, WINDOW_HEIGHT {500}, FPS {60};

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
        glfwSetWindowPos(window, 0, 0);

        if (!STB_IMAGE::loadWindowIcon(window, "./img/icon.bmp")) {
            throw std::string {"Falha ao carregar ícone da janela de visualização."};
        }

        glewExperimental = true;

        if (glewInit() != GLEW_OK) {
            throw std::string {"Falha ao iniciar GLEW."};
        }

        glfwSetCursorPosCallback(window, mouseCallback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        const GLFWvidmode *mode {
            glfwGetVideoMode(glfwGetPrimaryMonitor())
        };

        camera->setFov(60.0f);
        camera->setAspect(static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT));
        camera->setPosition(glm::tvec3<float>(8.0f, 16.0f, 8.0f));
        camera->setSpeed(0.5f);

        const int seed {1007};

        auto chunks {
            std::make_unique<Chunk::Chunk>(seed)
        };

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

                auto view {camera->getViewMatrix()}, projection {camera->getProjectionMatrix()};

                chunks->draw(view, projection);
                chunks->addChunk(camera);

                glfwSwapBuffers(window);
                glfwPollEvents();

                lastFrame = currentFrame;
            }
        }

        glfwDestroyWindow(window);
        glfwTerminate();

        return 0;
    } catch (const std::string& error) {
        glfwDestroyWindow(window);
        glfwTerminate();

        std::cerr << error << std::endl;

        return 1;
    }
}

void keyboardCallback(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera->keyboardProcess(Camera::MOVEMENTS::FORWARD);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera->keyboardProcess(Camera::MOVEMENTS::BACKWARD);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera->keyboardProcess(Camera::MOVEMENTS::RIGHT);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->keyboardProcess(Camera::MOVEMENTS::LEFT);
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

    camera->mouseProcess(&offSetX, &offSetY);
}
