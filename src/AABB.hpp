#ifndef AABB_HPP
#define AABB_HPP

namespace AABB {

float distance(const tools::sphere &a, const tools::sphere &b) {
    const auto x_sum {pow((a.x - b.x), 2)};
    const auto y_sum {pow((a.y - b.y), 2)};
    const auto z_sum {pow((a.z - b.z), 2)};

    return sqrt(x_sum + y_sum + z_sum);
}

bool collision_detection(const tools::sphere &a, const tools::sphere &b) {
    if (distance(a, b) < a.r + b.r) return true;

    return false;
}

void collision_resolution(const tools::sphere &a, const tools::sphere &b, camera::camera &cam) {
    const auto penetration {a.r + b.r - distance(a, b)};

    cam.set_position(cam.get_position() - (penetration * cam.get_front()));
}

}

#endif
