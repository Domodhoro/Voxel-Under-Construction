#ifndef AABB_HPP
#define AABB_HPP

namespace AABB {

struct box {
    float x      {0.0f};
    float y      {0.0f};
    float z      {0.0f};
    float length {0.0f};
    float width  {0.0f};
    float height {0.0f};
};

static bool collision_detection(const box &a, const box &b) {
    const auto x_axis_collision {a.x < b.x + b.length && a.x + a.length > b.x};
    const auto y_axis_collision {a.y < b.y + b.width  && a.y + a.width  > b.y};
    const auto z_axis_collision {a.z < b.z + b.height && a.z + a.height > b.z};

    return x_axis_collision && y_axis_collision && z_axis_collision;
}

static void collision_resolution(const box &a, const box &b, camera::camera &cam) {
    printf("collide...\n");

    // ...
}

/*
struct sphere {
    float x {0.0f};
    float y {0.0f};
    float z {0.0f};
    float r {0.0f};
};

static float distance(const sphere &a, const sphere &b) {
    const auto x_sum {pow((a.x - b.x), 2)};
    const auto y_sum {pow((a.y - b.y), 2)};
    const auto z_sum {pow((a.z - b.z), 2)};

    return sqrt(x_sum + y_sum + z_sum);
}

static bool collision_detection(const sphere &a, const sphere &b) {
    if (distance(a, b) < a.r + b.r) return true;

    return false;
}

static void collision_resolution(const sphere &a, const sphere &b, camera::camera &cam) {
    const auto penetration {a.r + b.r - distance(a, b)};

    auto direction {glm::tvec3<float>(a.x, a.y, a.z) - glm::tvec3<float>(b.x, b.y, b.z)};

    cam.set_position(cam.get_position() - penetration * glm::normalize(direction));
}
*/

}

#endif
