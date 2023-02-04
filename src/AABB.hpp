#ifndef AABB_HPP
#define AABB_HPP

namespace AABB {

bool collision_detection(const tools::box &a, const tools::box &b) {
    const auto x_axis_collision {a.x < b.x + b.length && a.x + a.length > b.x};
    const auto y_axis_collision {a.y < b.y + b.width  && a.y + a.width  > b.y};
    const auto z_axis_collision {a.z < b.z + b.height && a.z + a.height > b.z};

    return x_axis_collision && y_axis_collision && z_axis_collision;
}

void collision_resolution(const tools::box &a, const tools::box &b, camera::camera &cam) {
    // ...
}

}

#endif
