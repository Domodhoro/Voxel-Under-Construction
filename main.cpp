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

#include "./src/shaderProgram.hpp"
#include "./src/stb_image_wrapper.hpp"
#include "./src/camera.hpp"
#include "./src/noise.hpp"
#include "./src/chunkMesh.hpp"
#include "./src/chunk.hpp"

Camera Camera;

Noise Noise;

struct worldCoordinate {
    long int x {0}, y {0}, z {0};

    friend bool operator==(const worldCoordinate &lhs, const worldCoordinate &rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
    }
};

std::vector<std::pair<worldCoordinate, std::unique_ptr<Chunk>>> chunks;

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void keyboardCallback(GLFWwindow *window);
void mouseCallback(GLFWwindow *window, double x, double y);
void addChunk(unsigned int &Texture);

int main(int argc, char *argv[]) {
    GLFWwindow *window {nullptr};

    try {
        if (glfwInit() == GLFW_NOT_INITIALIZED) {
            throw std::runtime_error {
                "Falha ao iniciar o GLFW."
            };
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_RESIZABLE, false);

        window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Voxel", nullptr, nullptr);

        if (window == nullptr) {
            throw std::runtime_error {
                "Falha ao criar a janela de visualização."
            };
        }

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        glfwSetWindowPos(window, 100, 100);

        loadWindowIcon(window, "./img/icon.bmp");

        glewExperimental = true;

        if (glewInit() != GLEW_OK) {
            throw std::runtime_error {
                "Falha ao iniciar GLEW."
            };
        }

        glfwSetCursorPosCallback(window, mouseCallback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        const GLFWvidmode *mode {
            glfwGetVideoMode(glfwGetPrimaryMonitor())
        };

        Camera.setSpeed(0.5f);
        Camera.setFov(60.0f);
        Camera.setAspect(static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT));
        Camera.setPosition(glm::tvec3<float>(0.0f, 100.0f, 0.0f));

        auto ShaderProgram {
            Shader("./glsl/vertex.glsl", "./glsl/fragment.glsl")
        };

        auto Texture {
            loadTexture("./img/blocks.bmp")
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
                glClearColor(0.7f, 0.8f, 1.0f, 1.0f);

                auto View {Camera.getViewMatrix()}, Projection {Camera.getProjectionMatrix()};

                for (auto &it : chunks) {
                    it.second->draw(ShaderProgram, View, Projection);
                }

                addChunk(Texture);

                glfwSwapBuffers(window);
                glfwPollEvents();

                lastFrame = currentFrame;
            }
        }

        glDeleteTextures(1, &Texture);

        glfwDestroyWindow(window);
        glfwTerminate();

    } catch (std::exception &e) {
        glfwDestroyWindow(window);
        glfwTerminate();

        std::cout << e.what() << std::endl;
        std::cin.get();

        return 1;
    }

    return 0;
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
};

void keyboardCallback(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        Camera.keyboardProcess(MOVEMENTS::FORWARD);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        Camera.keyboardProcess(MOVEMENTS::BACKWARD);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        Camera.keyboardProcess(MOVEMENTS::RIGHT);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        Camera.keyboardProcess(MOVEMENTS::LEFT);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        Camera.getPosition().y += 0.5f;
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

    Camera.mouseProcess(&offSetX, &offSetY);
}

void addChunk(unsigned int &Texture) {
    worldCoordinate coord {
        static_cast<long int>(std::floor(Camera.getPosition().x / static_cast<float>(CHUNK_SIZE_X))),
        0,
        static_cast<long int>(std::floor(Camera.getPosition().z / static_cast<float>(CHUNK_SIZE_Z)))
    };

    auto Predicate = [&](std::pair<worldCoordinate, std::unique_ptr<Chunk>> &chunk) -> bool {
        return chunk.first == coord;
    };

    if (std::find_if(chunks.begin(), chunks.end(), Predicate) == chunks.end()) {
        chunks.emplace_back(coord, std::make_unique<Chunk>(coord.x * CHUNK_SIZE_X, coord.z * CHUNK_SIZE_Z, Texture, Noise));
    }
}
