#ifndef COLLISION_HPP
#define COLLISION_HPP

namespace collision {

static bool collision_detection(const AABB &cam_AABB, const AABB &object_AABB) {
    if (cam_AABB.max.x < object_AABB.min.x || cam_AABB.min.x > object_AABB.max.x) return false;
    if (cam_AABB.max.y < object_AABB.min.y || cam_AABB.min.y > object_AABB.max.y) return false;
    if (cam_AABB.max.z < object_AABB.min.z || cam_AABB.min.z > object_AABB.max.z) return false;

    return true;
}

static void collision_resolution(camera::camera &cam, const AABB &object_AABB) {
    auto x_overlap {0.0f};
    auto y_overlap {0.0f};
    auto z_overlap {0.0f};

    if (cam.get_AABB().max.x > object_AABB.min.x && cam.get_AABB().min.x < object_AABB.max.x) {
        x_overlap = std::min(cam.get_AABB().max.x, object_AABB.max.x) - std::max(cam.get_AABB().min.x, object_AABB.min.x);
    }

    if (cam.get_AABB().max.y > object_AABB.min.y && cam.get_AABB().min.y < object_AABB.max.y) {
        y_overlap = std::min(cam.get_AABB().max.y, object_AABB.max.y) - std::max(cam.get_AABB().min.y, object_AABB.min.y);
    }

    if (cam.get_AABB().max.z > object_AABB.min.z && cam.get_AABB().min.z < object_AABB.max.z) {
        z_overlap = std::min(cam.get_AABB().max.z, object_AABB.max.z) - std::max(cam.get_AABB().min.z, object_AABB.min.z);
    }

    auto new_position {cam.get_position()};

    if (x_overlap < y_overlap && x_overlap < z_overlap) {
        if (cam.get_AABB().min.x < object_AABB.min.x) {
            new_position.x -= x_overlap;
        } else {
            new_position.x += x_overlap;
        }
    } else if (y_overlap < x_overlap && y_overlap < z_overlap) {
        if (cam.get_AABB().min.y < object_AABB.min.y) {
            new_position.y -= y_overlap;
        } else {
            new_position.y += y_overlap;
        }
    } else if (z_overlap < x_overlap && z_overlap < y_overlap) {
        if (cam.get_AABB().min.z < object_AABB.min.z) {
            new_position.z -= z_overlap;
        } else {
            new_position.z += z_overlap;
        }
    }

    cam.set_position(new_position);
}

}

#endif
