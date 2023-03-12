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

#include "./lua54/luaconf.h"
#include "./lua54/lua.h"
#include "./lua54/lualib.h"
#include "./lua54/lauxlib.h"
}

#endif

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
constexpr auto CAMERA_SPEED       {0.1f};
constexpr auto CAMERA_FOV         {72.0f};
constexpr auto CAMERA_SENSITIVITY {0.1f};
constexpr auto CHUNK_SIZE_X       {16};
constexpr auto CHUNK_SIZE_Y       {128};
constexpr auto CHUNK_SIZE_Z       {CHUNK_SIZE_X};

static void error_log(const char *file, const int line, const char *description) {
    puts  ("A failure occurred!");
    printf("File:        %s.\n", file);
    printf("Line:        %i.\n", line);
    printf("Description: %s.\n", description);

    exit(EXIT_FAILURE);
}

template<typename T>
struct vertex_2d {
    glm::tvec2<T> position;
    glm::tvec2<T> texture_UV;
};

template<typename T>
struct vertex_2d_id {
    glm::tvec2<T> position;
    glm::tvec3<T> texture_UV;
};

template<typename T>
struct vertex_3d {
    glm::tvec3<T> position;
    glm::tvec2<T> texture_UV;
};

template<typename T>
struct vertex_3d_id {
    glm::tvec3<T> position;
    glm::tvec3<T> texture_UV;
};

template<typename T>
struct block_face {
    T Front;
    T Back;
    T Right;
    T Left;
    T Up;
    T Down;
};

struct AABB {
    glm::tvec3<float> min {0.0f};
    glm::tvec3<float> max {1.0f};

    AABB(const glm::tvec3<float> center, const float half_width, const float half_heigth, const float half_depth) {
        min = center - glm::tvec3<float>(half_width, half_heigth, half_depth);
        max = center + glm::tvec3<float>(half_width, half_heigth, half_depth);
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

enum struct BLOCK : int {
    AIR = 0,
    GRASS,
    DIRT,
    STONE,
    SAND,
    MAGMA,
    FELDSPAR
};

#include "./src/lua_script.hpp"
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

    cam.mouse_update(off_set_x, off_set_y, CAMERA_SENSITIVITY);
}

int main(int argc, char *argv[]) {
    puts(argv[0]);

    lua_script::lua_script {"./script.lua"};

    if (glfwInit() == GLFW_NOT_INITIALIZED) error_log(__FILE__, __LINE__, "falha ao iniciar o GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, true);
    glfwWindowHint(GLFW_RESIZABLE, false);

    const auto window {
        glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr)
    };

    if (window == nullptr) error_log(__FILE__, __LINE__, "falha ao criar a janela de visualização");

    glfwMakeContextCurrent(window);

    const auto video_mode   {glfwGetVideoMode(glfwGetPrimaryMonitor())};
    const auto window_pos_x {(video_mode->width  - WINDOW_WIDTH)  / 2};
    const auto window_pos_y {(video_mode->height - WINDOW_HEIGHT) / 2};

    glfwSetWindowPos(window, window_pos_x, window_pos_y);
    glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    stb_image_wrapper::load_window_icon(window, "./img/icon.bmp");

    glewExperimental = true;

    if (glewInit() != GLEW_OK) error_log(__FILE__, __LINE__, "falha ao iniciar GLEW");

    camera::camera cam {WINDOW_WIDTH, WINDOW_HEIGHT};

    cam.disable_cursor(window);
    cam.set_speed     (CAMERA_SPEED);
    cam.set_FOV       (CAMERA_FOV);
    cam.set_position  ({8.0f, 72.0f, 8.0f});

    auto framebuffer_shader {shader::shader_program {"./glsl/framebuffer_vertex.glsl", "./glsl/framebuffer_fragment.glsl"}};
    auto skybox_shader      {shader::shader_program {"./glsl/skybox_vertex.glsl", "./glsl/skybox_fragment.glsl"}};
    auto chunk_shader       {shader::shader_program {"./glsl/chunk_vertex.glsl", "./glsl/chunk_fragment.glsl"}};

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

    sky_texture.clear();

    terrain_generator::terrain_generator terrain {1007};
    framebuffer::framebuffer window_framebuffer  {WINDOW_WIDTH, WINDOW_HEIGHT, FRAMEBUFFER_TYPE::DEFAULT};
    skybox::skybox world_skybox                  {};
    chunk::chunk spawn_chunk                     {0, 0, 0, terrain};

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
