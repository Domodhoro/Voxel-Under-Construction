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

constexpr auto WINDOW_WIDTH  {1200};
constexpr auto WINDOW_HEIGHT {600};
constexpr auto WINDOW_TITLE  {"Voxel"};
constexpr auto FPS           {60};

#include "./src/shader.hpp"
#include "./src/stb_image_wrapper.hpp"
#include "./src/camera.hpp"
#include "./src/chunk.hpp"

camera::camera cam {};

FastNoiseLite noise {1007};

static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
static void keyboard_callback        (GLFWwindow *window);
static void mouse_callback           (GLFWwindow *window, double x, double y);

int main(int argc, char *argv[]) {
    printf("%s\n", argv[0]);

    if (glfwInit() == GLFW_NOT_INITIALIZED) {
        printf("Falha ao iniciar o GLFW.");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, false);
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow *window {
        glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr)
    };

    if (!window) {
        printf("Falha ao criar a janela de visualização.");
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    auto window_pos_x {100}, window_pos_y {100};

    glfwSetWindowPos(window, window_pos_x, window_pos_y);

    if (!stb_image_wrapper::load_window_icon(window, "./img/icon.bmp")) {
        printf("Falha ao carregar textura do ícone da janela de visualização.");
        return 1;
    }

    glewExperimental = true;

    if (glewInit() != GLEW_OK) {
        printf("Falha ao iniciar GLEW.");
        return 1;
    }

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    const GLFWvidmode *mode {
        glfwGetVideoMode(glfwGetPrimaryMonitor())
    };

    window_pos_x = (mode->width  - WINDOW_WIDTH)  / 2;
    window_pos_y = (mode->height - WINDOW_HEIGHT) / 2;

    glfwSetWindowPos(window, window_pos_x, window_pos_y);

    cam.set_speed   (0.5f);
    cam.set_FOV     (60.0f);
    cam.set_aspect  (static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT));
    cam.set_position(glm::tvec3<float>(0.0f, 100.0f, 0.0f));

    auto chunk_shader  { shader::shader("./glsl/chunkVertex.glsl", "./glsl/chunkFragment.glsl") };
    auto chunk_texture { stb_image_wrapper::load_texture("./img/blocks.bmp")};

    struct world_coords {
        int x {0};
        int y {0};
        int z {0};
    };

    std::vector<std::pair<world_coords, std::unique_ptr<chunk::chunk>>> chunks;

    auto add_chunk = [&](glm::tvec3<float> position) -> void {
        world_coords coordinates {
            static_cast<int>(std::floor(position.x / static_cast<float>(chunk::CHUNK_SIZE_X))),
            0,
            static_cast<int>(std::floor(position.z / static_cast<float>(chunk::CHUNK_SIZE_Z)))
        };

        auto predicate = [&](std::pair<world_coords, std::unique_ptr<chunk::chunk>> &chunk) -> bool {
            return chunk.first.x == coordinates.x && chunk.first.y == coordinates.y && chunk.first.z == coordinates.z;
        };

        if (std::find_if(chunks.begin(), chunks.end(), predicate) == chunks.end()) {
            chunks.emplace_back(coordinates, std::make_unique<chunk::chunk>(coordinates.x * chunk::CHUNK_SIZE_X, coordinates.z * chunk::CHUNK_SIZE_Z, chunk_texture, noise));
        }
    };

    glEnable   (GL_DEPTH_TEST);
    glEnable   (GL_CULL_FACE);
    glFrontFace(GL_CCW);

    auto last_frame {0.0f}, current_frame {0.0f};

    while (!glfwWindowShouldClose(window)) {
        current_frame = static_cast<float>(glfwGetTime());

        add_chunk(cam.get_position());

        if ((current_frame - last_frame) > (1.0f / static_cast<float>(FPS))) {
            keyboard_callback(window);

            glClear     (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.7f, 0.8f, 1.0f, 1.0f);

            for (auto &it : chunks) {
                it.second->draw(chunk_shader, cam);
            }

            glfwSwapBuffers(window);
            glfwPollEvents ();

            last_frame = current_frame;
        }
    }

    glfwTerminate();

    return 0;
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); };

static void keyboard_callback(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)      cam.keyboard_process(camera::MOVEMENTS::FORWARD);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)      cam.keyboard_process(camera::MOVEMENTS::BACKWARD);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)      cam.keyboard_process(camera::MOVEMENTS::RIGHT);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)      cam.keyboard_process(camera::MOVEMENTS::LEFT);
}

static void mouse_callback(GLFWwindow *window, double x, double y) {
    static auto firstMouse {true};
    static auto lastX      {0.0f};
    static auto lastY      {0.0f};

    if (firstMouse) {
        lastX = static_cast<float>(x);
        lastY = static_cast<float>(y);

        firstMouse = false;
    }

    auto offSetX {x - lastX};
    auto offSetY {lastY - y};

    lastX = x;
    lastY = y;

    const auto sensitivity {0.1f};

    offSetX *= sensitivity;
    offSetY *= sensitivity;

    cam.mouse_process(offSetX, offSetY);
}
