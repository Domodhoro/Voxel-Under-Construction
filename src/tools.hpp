#ifndef TOOLS_HPP
#define TOOLS_HPP

namespace tools {

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

struct vertex_2d {
    float X {0.0f};
    float Y {0.0f};
    float U {0.0f};
    float V {0.0f};
};

struct vertex_3d {
    float X {0.0f};
    float Y {0.0f};
    float Z {0.0f};
    float U {0.0f};
    float V {0.0f};
};

struct vertex_2d_t {
    vertex_2d vertice;

    float T {0.0f};
};

struct vertex_3d_t {
    vertex_3d vertice;

    float T {0.0f};
};

struct face {
    bool F {true};
    bool B {true};
    bool R {true};
    bool L {true};
    bool U {true};
    bool D {true};
};

struct face_texture {
    float F {0.0f};
    float B {0.0f};
    float R {0.0f};
    float L {0.0f};
    float U {0.0f};
    float D {0.0f};
};

struct world_coords {
    int x {0};
    int y {0};
    int z {0};

    friend bool operator==(const world_coords &lhs, const world_coords &rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
    }
};

}

#endif
