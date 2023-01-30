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

constexpr auto FPS                {60};
constexpr auto WINDOW_WIDTH       {1200};
constexpr auto WINDOW_HEIGHT      {600};
constexpr auto WINDOW_TITLE       {"Voxel-Engine"};
constexpr auto ASPECT             {static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT)};
constexpr auto CAMERA_SPEED       {0.1f};
constexpr auto CAMERA_FOV         {72.0f};
constexpr auto CAMERA_SENSITIVITY {0.1f};
constexpr auto WORLD_SEED         {1007};
constexpr auto CHUNK_SIZE_X       {16};
constexpr auto CHUNK_SIZE_Y       {128};
constexpr auto CHUNK_SIZE_Z       {CHUNK_SIZE_X};

struct my_exception {
    my_exception(const char *file, int line, const char *description) {
        printf("ops! uma falha ocorreu...\n\n");
        printf("File:        %s\n", file);
        printf("Line:        %i\n", line);
        printf("Description: %s\n", description);
    }

    ~my_exception() { exit(EXIT_FAILURE); }
};

#include "./src/tools.hpp"
#include "./src/camera.hpp"
#include "./src/AABB.hpp"
#include "./src/shader.hpp"
#include "./src/stb_image_wrapper.hpp"
#include "./src/framebuffer.hpp"
#include "./src/skybox.hpp"
#include "./src/terrain_generator.hpp"
#include "./src/chunk.hpp"

camera::camera cam                           {ASPECT};
terrain_generator::terrain_generator terrain {WORLD_SEED};

static GLFWwindow *initialization(int window_width, int window_height, const char *window_title);
static void keyboard_callback    (GLFWwindow *window);
static void mouse_callback       (GLFWwindow *window);

int main(int argc, char *argv[]) {

    auto window {initialization(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE)};

    cam.disable_cursor (window);
    cam.set_speed      (CAMERA_SPEED);
    cam.set_sensitivity(CAMERA_SENSITIVITY);
    cam.set_FOV        (CAMERA_FOV);
    cam.set_position   ({8.0f, 92.0f, 8.0f});

    auto framebuffer_shader {shader::shader_program("./glsl/framebuffer_vertex.glsl", "./glsl/framebuffer_fragment.glsl")};
    auto skybox_shader      {shader::shader_program("./glsl/skybox_vertex.glsl", "./glsl/skybox_fragment.glsl")};
    auto chunk_shader       {shader::shader_program("./glsl/chunk_vertex.glsl", "./glsl/chunk_fragment.glsl")};

    std::vector<std::string> sky_texture {
        "img/skybox/right.bmp",
        "img/skybox/left.bmp",
        "img/skybox/up.bmp",
        "img/skybox/down.bmp",
        "img/skybox/front.bmp",
        "img/skybox/back.bmp"
    };

    auto skybox_texture {stb_image_wrapper::load_cube_map_texture(sky_texture)};
    auto chunk_texture  {stb_image_wrapper::load_texture("./img/blocks.bmp")};

    framebuffer::framebuffer window_framebuffer {WINDOW_WIDTH, WINDOW_HEIGHT, tools::FRAMEBUFFER_TYPE::DEFAULT};
    skybox::skybox world_skybox                 {};
    chunk::chunk spawn_chunk                    {0, 0, 0, terrain};

    // test .....................................................................................

    const auto radius {0.5f};

    tools::sphere a {0.5f, 90.5f, 0.5f, radius};
    tools::sphere b {0.5f,  0.0f, 0.5f, radius};

    // test .....................................................................................

    glEnable   (GL_DEPTH_TEST);
    glEnable   (GL_CULL_FACE);
    glFrontFace(GL_CCW);

    auto last_frame    {0.0f};
    auto current_frame {0.0f};

    while (!glfwWindowShouldClose(window)) {
        current_frame = glfwGetTime();

        if ((current_frame - last_frame) > (1.0f / FPS)) {
            keyboard_callback(window);
            mouse_callback   (window);

            // test .....................................................................................

            b = {cam.get_position().x, cam.get_position().y, cam.get_position().z, 0.5f};

            if (AABB::collision_detection(a, b)) AABB::collision_resolution(a, b, cam);

            // test .....................................................................................

            window_framebuffer.clear_color(0.0f, 0.0f, 0.0f);
            world_skybox.draw             (skybox_shader, skybox_texture, cam);
            spawn_chunk.draw              (chunk_shader, chunk_texture, cam);
            window_framebuffer.draw       (framebuffer_shader);

            glfwSwapBuffers(window);
            glfwPollEvents ();

            last_frame = current_frame;
        }
    }

    glDeleteTextures (1, &chunk_texture);
    glDeleteTextures (1, &skybox_texture);
    glfwDestroyWindow(window);
    glfwTerminate    ();

    return 0;
}

static GLFWwindow *initialization(const int window_width, const int window_height, const char *window_title) {
    if (glfwInit() == GLFW_NOT_INITIALIZED) my_exception {__FILE__, __LINE__, "falha ao iniciar o GLFW"};

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, false);
    glfwWindowHint(GLFW_RESIZABLE, false);

    auto window {glfwCreateWindow(window_width, window_height, window_title, nullptr, nullptr)};

    if (window == nullptr) my_exception {__FILE__, __LINE__, "falha ao criar a janela de visualização"};

    glfwMakeContextCurrent(window);

    auto mode {glfwGetVideoMode(glfwGetPrimaryMonitor())};

    auto window_pos_x {(mode->width  - window_width)  / 2};
    auto window_pos_y {(mode->height - window_height) / 2};

    glfwSetWindowPos(window, window_pos_x, window_pos_y);

    stb_image_wrapper::load_window_icon(window, "./img/icon.bmp");

    glewExperimental = true;

    if (glewInit() != GLEW_OK) my_exception {__FILE__, __LINE__, "falha ao iniciar GLEW"};

    return window;
}

static void keyboard_callback(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cam.keyboard_process(tools::CAMERA_MOVEMENTS::FORWARD);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cam.keyboard_process(tools::CAMERA_MOVEMENTS::BACKWARD);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cam.keyboard_process(tools::CAMERA_MOVEMENTS::RIGHT);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cam.keyboard_process(tools::CAMERA_MOVEMENTS::LEFT);
}

static void mouse_callback(GLFWwindow *window) {
    auto x {0.0d};
    auto y {0.0d};

    glfwGetCursorPos(window, &x, &y);

    static auto first_mouse {true};
    static auto last_x      {0.0f};
    static auto last_y      {0.0f};

    if (first_mouse) {
        last_x      = x;
        last_y      = y;
        first_mouse = false;
    }

    auto off_set_x {x - last_x};
    auto off_set_y {last_y - y};

    last_x = x;
    last_y = y;

    cam.mouse_process(off_set_x, off_set_y);
}
