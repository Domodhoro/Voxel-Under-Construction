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

bool collide(volume &volume_1, volume &volume_2) {
    const auto x_axis_collision {volume_1.x < volume_2.x + volume_2.length && volume_1.x + volume_1.length > volume_2.x};
    const auto y_axis_collision {volume_1.y < volume_2.y + volume_2.height && volume_1.y + volume_1.height > volume_2.y};
    const auto z_axis_collision {volume_1.z < volume_2.z + volume_2.width  && volume_1.z + volume_1.width  > volume_2.z};

    if(x_axis_collision && y_axis_collision && z_axis_collision) return true;

    return false;
}

}

#endif
