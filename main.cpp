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

extern "C" {
#include "lualib/lua.h"
#include "lualib/lualib.h"
#include "lualib/luaconf.h"
#include "lualib/lauxlib.h"
}

#include "./src/luaScript.hpp"
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
        LuaScript::LuaScript luaScript {"./script.lua"};

        luaScript.readTable("window");

        auto windowWidth {luaScript.readNumber<int>("width")}, windowHeight {luaScript.readNumber<int>("height")};

        if (glfwInit() == GLFW_NOT_INITIALIZED) {
            throw std::string {"Falha ao iniciar o GLFW."};
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_RESIZABLE, false);

        window = glfwCreateWindow(windowWidth, windowHeight, "Voxel", nullptr, nullptr);

        if (window == nullptr) {
            throw std::string {"Falha ao criar a janela de visualização."};
        }

        glfwMakeContextCurrent(window);

        auto framebufferSizeCallback = [](GLFWwindow *window, int width, int height) -> void {
            glViewport(0, 0, width, height);
        };

        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        glfwSetWindowPos(window, 0, 0);

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

        luaScript.readTable("camera");

        camera.setSpeed(luaScript.readNumber<float>("speed"));
        camera.setFov(luaScript.readNumber<float>("fov"));
        camera.setAspect(static_cast<float>(windowWidth) / static_cast<float>(windowHeight));
        camera.setPosition(glm::tvec3<float>(0.0f, 0.0f, -5.0f));

        Chunk::Chunk chunks;

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);

        const auto FPS {60};

        auto lastFrame {0.0f}, currentFrame {0.0f};

        while (!glfwWindowShouldClose(window)) {
            currentFrame = static_cast<float>(glfwGetTime());

            if ((currentFrame - lastFrame) > (1.0f / static_cast<float>(FPS))) {
                glfwSetWindowPos(window, (mode->width - windowWidth) / 2, (mode->height - windowHeight) / 2);

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
