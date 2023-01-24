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

extern "C" {
#include "./lualib/lua.h"
#include "./lualib/lualib.h"
#include "./lualib/lauxlib.h"
#include "./lualib/luaconf.h"
}

constexpr auto FPS          {60};
constexpr auto NOISE_MAX    {3};
constexpr auto CHUNK_SIZE_X {16};
constexpr auto CHUNK_SIZE_Y {64};
constexpr auto CHUNK_SIZE_Z {CHUNK_SIZE_X};

struct my_exception {
    my_exception(const char *file, int line, const char *description) {
        printf("Ops! Uma falha ocorreu...\n\n");
        printf("File:        %s\n", basename(file));
        printf("Line:        %i\n", line);
        printf("Description: %s\n", description);
    }

    ~my_exception() {
        exit(EXIT_FAILURE);
    }
};

auto WORLD_SEED         {1007};
auto WORLD_SIZE         {2};
auto WINDOW_WIDTH       {1200};
auto WINDOW_HEIGHT      {600};
auto WINDOW_TITLE       {"Voxel-Engine"};
auto CAMERA_SPEED       {0.5f};
auto CAMERA_FOV         {60.0f};
auto CAMERA_SENSITIVITY {0.1f};

#include "./src/lua_script.hpp"
#include "./src/tools.hpp"
#include "./src/camera.hpp"
#include "./src/shader.hpp"
#include "./src/stb_image_wrapper.hpp"
#include "./src/framebuffer.hpp"
#include "./src/skybox.hpp"
#include "./src/noise.hpp"
#include "./src/chunk.hpp"
#include "./src/chunk_manager.hpp"

lua_script::lua_script lua {"./script.lua"};
camera::camera cam         {static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT)};
noise::noise chunk_noise   {WORLD_SEED};

static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
static void keyboard_callback        (GLFWwindow *window);
static void mouse_callback           (GLFWwindow *window, double x, double y);

int main(int argc, char *argv[]) {
    printf("%s\n", argv[0]);

    if (glfwInit() == GLFW_NOT_INITIALIZED) my_exception {__FILE__, __LINE__, "falha ao iniciar o GLFW"};

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_DECORATED, false);
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow *window {
        glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr)
    };

    if (window == nullptr) my_exception {__FILE__, __LINE__, "falha ao criar a janela de visualização"};

    glfwMakeContextCurrent(window);

    auto window_pos_x {100}, window_pos_y {100};

    glfwSetWindowPos(window, window_pos_x, window_pos_y);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    stb_image_wrapper::load_window_icon(window, "./img/icon.bmp");

    glewExperimental = true;

    if (glewInit() != GLEW_OK) my_exception {__FILE__, __LINE__, "falha ao iniciar GLEW"};

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    const GLFWvidmode *mode {
        glfwGetVideoMode(glfwGetPrimaryMonitor())
    };

    window_pos_x = (mode->width  - WINDOW_WIDTH)  / 2;
    window_pos_y = (mode->height - WINDOW_HEIGHT) / 2;

    glfwSetWindowPos(window, window_pos_x, window_pos_y);

    cam.set_speed      (CAMERA_SPEED);
    cam.set_sensitivity(CAMERA_SENSITIVITY);
    cam.set_FOV        (CAMERA_FOV);
    cam.set_position   (glm::tvec3<float>(8.0f, 52.0f, 8.0f));

    auto chunk_shader  {shader::shader_program("./glsl/chunk_vertex.glsl", "./glsl/chunk_fragment.glsl")};
    auto chunk_texture {stb_image_wrapper::load_texture("./img/blocks.bmp")};

    chunk_manager::chunk_manager chunk_manager {};

    std::vector<std::string> sky_texture {
        "img/skybox/right.bmp",
        "img/skybox/left.bmp",
        "img/skybox/down.bmp",
        "img/skybox/up.bmp",
        "img/skybox/front.bmp",
        "img/skybox/back.bmp"
    };

    auto skybox_shader  {shader::shader_program("./glsl/skybox_vertex.glsl", "./glsl/skybox_fragment.glsl")};
    auto skybox_texture {stb_image_wrapper::load_cube_map_texture(sky_texture)};

    skybox::skybox world_skybox {};

    auto framebuffer_shader {
        shader::shader_program("./glsl/framebuffer_vertex.glsl", "./glsl/framebuffer_fragment.glsl")
    };

    framebuffer::framebuffer window_framebuffer {
        WINDOW_WIDTH, WINDOW_HEIGHT, tools::FRAMEBUFFER_TYPE::DEFAULT
    };

    glEnable   (GL_DEPTH_TEST);
    glEnable   (GL_CULL_FACE);
    glFrontFace(GL_CCW);

    auto last_frame {0.0f}, current_frame {0.0f};

    while (!glfwWindowShouldClose(window)) {
        current_frame = static_cast<float>(glfwGetTime());

        for (auto x = 1 - WORLD_SIZE; x != WORLD_SIZE; ++x) for (auto z = 1 - WORLD_SIZE; z != WORLD_SIZE; ++z) {
            const glm::tvec3<float> aux {
                static_cast<float>(x * CHUNK_SIZE_X),
                0.0f,
                static_cast<float>(z * CHUNK_SIZE_Z)
            };

            chunk_manager.add_chunk(cam.get_position() + aux, chunk_noise);
        }

        if ((current_frame - last_frame) > (1.0f / static_cast<float>(FPS))) {
            keyboard_callback(window);

            window_framebuffer.clear_color(0.0f, 0.0f, 0.0f);
            world_skybox.draw             (skybox_shader, skybox_texture, cam);
            chunk_manager.draw            (chunk_shader, chunk_texture, cam);
            window_framebuffer.apply      (framebuffer_shader);

            glfwSwapBuffers(window);
            glfwPollEvents ();

            last_frame = current_frame;
        }
    }

    glDeleteTextures(1, &chunk_texture);
    glDeleteTextures(1, &skybox_texture);

    glfwDestroyWindow(window);
    glfwTerminate    ();

    return 0;
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

static void keyboard_callback(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cam.keyboard_process(tools::CAMERA_MOVEMENTS::FORWARD);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cam.keyboard_process(tools::CAMERA_MOVEMENTS::BACKWARD);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cam.keyboard_process(tools::CAMERA_MOVEMENTS::RIGHT);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cam.keyboard_process(tools::CAMERA_MOVEMENTS::LEFT);
}

static void mouse_callback(GLFWwindow *window, double x, double y) {
    static auto first_mouse {true};
    static auto last_x      {0.0f};
    static auto last_y      {0.0f};

    if (first_mouse) {
        last_x      = static_cast<float>(x);
        last_y      = static_cast<float>(y);
        first_mouse = false;
    }

    auto off_set_x {x - last_x};
    auto off_set_y {last_y - y};

    last_x = x;
    last_y = y;

    cam.mouse_process(off_set_x, off_set_y);
}
