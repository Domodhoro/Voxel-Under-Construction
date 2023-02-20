#ifndef COLLISION_HPP
#define COLLISION_HPP

namespace collision {

void collision(camera::camera &cam, const AABB &object_AABB) {
    AABB cam_AABB {cam.get_position(), 0.5f, 1.0f, 0.5f};

    auto detection = [&]() -> bool {
        if (cam_AABB.max.x < object_AABB.min.x || cam_AABB.min.x > object_AABB.max.x) return false;
        if (cam_AABB.max.y < object_AABB.min.y || cam_AABB.min.y > object_AABB.max.y) return false;
        if (cam_AABB.max.z < object_AABB.min.z || cam_AABB.min.z > object_AABB.max.z) return false;

        return true;
    };

    if (detection()) {
        vec3<float> overlap {0.0f, 0.0f, 0.0f};

        if (cam_AABB.max.x > object_AABB.min.x && cam_AABB.min.x < object_AABB.max.x) {
            overlap.x = std::min(cam_AABB.max.x, object_AABB.max.x) - std::max(cam_AABB.min.x, object_AABB.min.x);
        }

        if (cam_AABB.max.y > object_AABB.min.y && cam_AABB.min.y < object_AABB.max.y) {
            overlap.y = std::min(cam_AABB.max.y, object_AABB.max.y) - std::max(cam_AABB.min.y, object_AABB.min.y);
        }

        if (cam_AABB.max.z > object_AABB.min.z && cam_AABB.min.z < object_AABB.max.z) {
            overlap.z = std::min(cam_AABB.max.z, object_AABB.max.z) - std::max(cam_AABB.min.z, object_AABB.min.z);
        }

        auto new_position {cam.get_position()};

        if (overlap.x < overlap.y && overlap.x < overlap.z) {
            if (cam_AABB.min.x < object_AABB.min.x) {
                new_position.x -= overlap.x;
            } else {
                new_position.x += overlap.x;
            }
        } else if (overlap.y < overlap.x && overlap.y < overlap.z) {
            if (cam_AABB.min.y < object_AABB.min.y) {
                new_position.y -= overlap.y;
            } else {
                new_position.y += overlap.y;
            }
        } else if (overlap.z < overlap.x && overlap.z < overlap.y) {
            if (cam_AABB.min.z < object_AABB.min.z) {
                new_position.z -= overlap.z;
            } else {
                new_position.z += overlap.z;
            }
        }

        cam.set_position(new_position);
    }
}

}

#endif
