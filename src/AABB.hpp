#ifndef AABB_HPP
#define AABB_HPP

namespace AABB {

struct volume {
    float x      {0.0f};
    float y      {0.0f};
    float z      {0.0f};
    float length {0.0f};
    float height {0.0f};
    float width  {0.0f};
};

bool collision_detection(volume &vol_1, volume &vol_2) {
    const auto x_axis_collision {vol_1.x < vol_2.x + vol_2.length && vol_1.x + vol_1.length > vol_2.x};
    const auto y_axis_collision {vol_1.y < vol_2.y + vol_2.height && vol_1.y + vol_1.height > vol_2.y};
    const auto z_axis_collision {vol_1.z < vol_2.z + vol_2.width  && vol_1.z + vol_1.width  > vol_2.z};

    if (x_axis_collision && y_axis_collision && z_axis_collision) return true;

    return false;
}

}

#endif
