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

#include "./src/settings.hpp"
#include "./src/util.hpp"
#include "./src/lua_script.hpp"
#include "./src/camera.hpp"
#include "./src/shader.hpp"
#include "./src/stb_image_wrapper.hpp"
#include "./src/framebuffer.hpp"
#include "./src/skybox.hpp"
#include "./src/noise.hpp"
#include "./src/chunk.hpp"
#include "./src/chunk_manager.hpp"

lua_script::lua_script lua {"./script.lua"};

const auto aspect {
    static_cast<float>(settings::WINDOW_WIDTH) / static_cast<float>(settings::WINDOW_HEIGHT)
};

camera::camera cam       {aspect};
noise::noise chunk_noise {settings::WORLD_SEED};

static void keyboard_callback(GLFWwindow *window);
static void mouse_callback   (GLFWwindow *window, double x, double y);

int main(int argc, char *argv[]) {
    printf("%s\n", argv[0]);

    setlocale(LC_ALL, "portuguese");

    try {
        if (glfwInit() == GLFW_NOT_INITIALIZED) throw util::program_exception {"Falha ao iniciar o GLFW."};

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    } catch (util::program_exception &e) {
        printf("%s", e.get_description());
    }

    glfwWindowHint(GLFW_DECORATED, false);
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow *window {nullptr};

    try {
        window = glfwCreateWindow(settings::WINDOW_WIDTH, settings::WINDOW_HEIGHT, settings::WINDOW_TITLE, nullptr, nullptr);

        if (!window) throw util::program_exception {"Falha ao criar a janela de visualização."};

        glfwMakeContextCurrent(window);
    } catch (util::program_exception &e) {
        printf("%s", e.get_description());
    }

    auto window_pos_x {100}, window_pos_y {100};

    glfwSetWindowPos(window, window_pos_x, window_pos_y);
    glfwSetFramebufferSizeCallback(window, framebuffer::framebuffer_size_callback);

    stb_image_wrapper::load_window_icon(window, "./img/icon.bmp");

    try {
        glewExperimental = true;

        if (glewInit() != GLEW_OK) throw util::program_exception {"Falha ao iniciar GLEW."};
    } catch (util::program_exception &e) {
        printf("%s", e.get_description());
    }

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    const GLFWvidmode *mode {
        glfwGetVideoMode(glfwGetPrimaryMonitor())
    };

    window_pos_x = (mode->width  - settings::WINDOW_WIDTH)  / 2;
    window_pos_y = (mode->height - settings::WINDOW_HEIGHT) / 2;

    glfwSetWindowPos(window, window_pos_x, window_pos_y);

    cam.set_speed      (settings::CAMERA_SPEED);
    cam.set_sensitivity(settings::CAMERA_SENSITIVITY);
    cam.set_FOV        (settings::CAMERA_FOV);
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
        settings::WINDOW_WIDTH, settings::WINDOW_HEIGHT, util::FRAMEBUFFER_TYPE::DEFAULT
    };

    glEnable   (GL_DEPTH_TEST);
    glEnable   (GL_CULL_FACE);
    glFrontFace(GL_CCW);

    auto last_frame {0.0f}, current_frame {0.0f};

    while (!glfwWindowShouldClose(window)) {
        current_frame = static_cast<float>(glfwGetTime());

        chunk_manager.add_chunk   (cam.get_position(), chunk_noise);
        chunk_manager.remove_chunk(cam.get_position());

        if ((current_frame - last_frame) > (1.0f / static_cast<float>(settings::FPS))) {
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

    glfwDestroyWindow(window);
    glfwTerminate    ();

    return 0;
}

static void keyboard_callback(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cam.keyboard_process(util::CAMERA_MOVEMENTS::FORWARD);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cam.keyboard_process(util::CAMERA_MOVEMENTS::BACKWARD);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cam.keyboard_process(util::CAMERA_MOVEMENTS::RIGHT);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cam.keyboard_process(util::CAMERA_MOVEMENTS::LEFT);
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
