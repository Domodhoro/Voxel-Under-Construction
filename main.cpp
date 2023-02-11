/*
    MIT License

    Copyright (c) 2023 Guilherme M. Aguiar (guilhermemaguiar2022@gmail.com)

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

#ifdef __cplusplus

extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./lib/stb_image.h"
}

#endif

#include <memory>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "./lib/FastNoiseLite.hpp"

constexpr auto FPS                {60};
constexpr auto WINDOW_WIDTH       {800};
constexpr auto WINDOW_HEIGHT      {500};
constexpr auto WINDOW_TITLE       {"Voxel-Engine"};
constexpr auto CAMERA_SPEED       {0.05f};
constexpr auto CAMERA_FOV         {72.0f};
constexpr auto CAMERA_SENSITIVITY {0.1f};
constexpr auto CHUNK_SIZE_X       {16};
constexpr auto CHUNK_SIZE_Y       {128};
constexpr auto CHUNK_SIZE_Z       {CHUNK_SIZE_X};

struct my_exception {
    my_exception(const char *file, int line, const char *description) {
        puts  ("Ops! Uma falha ocorreu.");
        printf("File:        %s\n", file);
        printf("Line:        %i\n", line);
        printf("Description: %s\n", description);
    }

    ~my_exception() { exit(EXIT_FAILURE); }
};

template<typename T>
struct vec2 {
    T x;
    T y;
};

template<typename T>
struct vec3 {
    T x;
    T y;
    T z;
};

template<typename T>
struct vertex_2d {
    T x;
    T y;
    T u;
    T v;
};

template<typename T>
struct vertex_3d {
    T x;
    T y;
    T z;
    T u;
    T v;
};

template<typename T>
struct vertex_2d_t {
    T x;
    T y;
    T u;
    T v;
    T type;
};

template<typename T>
struct vertex_3d_t {
    T x;
    T y;
    T z;
    T u;
    T v;
    T type;
};

template<typename T>
struct face {
    T front;
    T back;
    T right;
    T left;
    T up;
    T down;
};

struct AABB {
    glm::tvec3<float> min {0.0f};
    glm::tvec3<float> max {1.0f};

    AABB(glm::tvec3<float> center, float hw, float hh, float hd) {
        min = center - glm::tvec3<float>(hw, hh, hd);
        max = center + glm::tvec3<float>(hw, hh, hd);
    }
};

enum struct CAMERA_MOVEMENTS : int {
    FORWARD = 0,
    BACKWARD,
    RIGHT,
    LEFT
};

enum struct FRAMEBUFFER_TYPE : int {
    DEFAULT = 0,
    INVERT_COLOR,
    GRAY_SCALE
};

enum struct BLOCK_TYPE : int {
    AIR = 0,
    GRASS,
    DIRT,
    STONE,
    SAND,
    MAGMA,
    FELDSPAR
};

#include "./src/camera.hpp"
#include "./src/collision.hpp"
#include "./src/shader.hpp"
#include "./src/stb_image_wrapper.hpp"
#include "./src/framebuffer.hpp"
#include "./src/skybox.hpp"
#include "./src/terrain_generator.hpp"
#include "./src/chunk.hpp"

static void keyboard_callback(GLFWwindow *window, camera::camera &cam) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cam.keyboard_update(CAMERA_MOVEMENTS::FORWARD);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cam.keyboard_update(CAMERA_MOVEMENTS::BACKWARD);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cam.keyboard_update(CAMERA_MOVEMENTS::RIGHT);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cam.keyboard_update(CAMERA_MOVEMENTS::LEFT);
}

static void mouse_callback(GLFWwindow *window, camera::camera &cam) {
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

    cam.mouse_update(off_set_x, off_set_y);
}

int main(int argc, char *argv[]) {
    puts(argv[0]);

    if (glfwInit() == GLFW_NOT_INITIALIZED) my_exception {__FILE__, __LINE__, "falha ao iniciar o GLFW"};

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, true);
    glfwWindowHint(GLFW_RESIZABLE, false);

    auto window {
        glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr)
    };

    if (window == nullptr) my_exception {__FILE__, __LINE__, "falha ao criar a janela de visualização"};

    glfwMakeContextCurrent(window);

    auto mode {glfwGetVideoMode(glfwGetPrimaryMonitor())};

    auto window_pos_x {(mode->width  - WINDOW_WIDTH)  / 2};
    auto window_pos_y {(mode->height - WINDOW_HEIGHT) / 2};

    glfwSetWindowPos(window, window_pos_x, window_pos_y);

    glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    stb_image_wrapper::load_window_icon(window, "./img/icon.bmp");

    glewExperimental = true;

    if (glewInit() != GLEW_OK) my_exception {__FILE__, __LINE__, "falha ao iniciar GLEW"};

    camera::camera cam {
        static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT)
    };

    cam.disable_cursor     (window);
    cam.set_speed          (CAMERA_SPEED);
    cam.set_sensitivity    (CAMERA_SENSITIVITY);
    cam.set_FOV            (CAMERA_FOV);
    cam.set_position       ({8.0f, 92.0f, 8.0f});

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

    terrain_generator::terrain_generator terrain {1007};
    framebuffer::framebuffer window_framebuffer  {WINDOW_WIDTH, WINDOW_HEIGHT, FRAMEBUFFER_TYPE::DEFAULT};
    skybox::skybox world_skybox                  {};
    chunk::chunk spawn_chunk                     {0, 0, 0, terrain};

    AABB object_AABB {glm::tvec3<float>(0.5f, 90.5f, 0.5f), 0.5f, 0.5f, 0.5f};

    glEnable   (GL_DEPTH_TEST);
    glEnable   (GL_CULL_FACE);
    glFrontFace(GL_CCW);

    static auto last_frame    {0.0f};
    static auto current_frame {0.0f};

    while (!glfwWindowShouldClose(window)) {
        current_frame = glfwGetTime();

        if ((current_frame - last_frame) > (1.0f / FPS)) {
            keyboard_callback(window, cam);
            mouse_callback   (window, cam);

            if (collision::collision_detection(cam.get_AABB(), object_AABB)) collision::collision_resolution(cam, object_AABB);

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
