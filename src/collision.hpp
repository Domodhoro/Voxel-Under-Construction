#ifndef COLLISION_HPP
#define COLLISION_HPP

namespace collision {

struct collision {
    bool detection(const AABB &cam_AABB, const AABB &object_AABB) const {
        if (cam_AABB.max.x < object_AABB.min.x || cam_AABB.min.x > object_AABB.max.x) return false;
        if (cam_AABB.max.y < object_AABB.min.y || cam_AABB.min.y > object_AABB.max.y) return false;
        if (cam_AABB.max.z < object_AABB.min.z || cam_AABB.min.z > object_AABB.max.z) return false;

        return true;
    }

    void response(camera::camera &cam, const AABB &object_AABB) const {
        const auto overlap {overlap_calculate<float>(cam, object_AABB)};

        auto new_position {cam.get_position()};

        if (overlap.x < overlap.y && overlap.x < overlap.z) {
            if (cam.get_AABB().min.x < object_AABB.min.x) {
                new_position.x -= overlap.x;
            } else {
                new_position.x += overlap.x;
            }
        } else if (overlap.y < overlap.x && overlap.y < overlap.z) {
            if (cam.get_AABB().min.y < object_AABB.min.y) {
                new_position.y -= overlap.y;
            } else {
                new_position.y += overlap.y;
            }
        } else if (overlap.z < overlap.x && overlap.z < overlap.y) {
            if (cam.get_AABB().min.z < object_AABB.min.z) {
                new_position.z -= overlap.z;
            } else {
                new_position.z += overlap.z;
            }
        }

        cam.set_position(new_position);
    }

protected:
    template<typename T>
    vec3<T> overlap_calculate(const camera::camera &cam, const AABB &object_AABB) const {
        vec3<T> overlap {0.0f, 0.0f, 0.0f};

        if (cam.get_AABB().max.x > object_AABB.min.x && cam.get_AABB().min.x < object_AABB.max.x) {
            overlap.x = std::min(cam.get_AABB().max.x, object_AABB.max.x) - std::max(cam.get_AABB().min.x, object_AABB.min.x);
        }

        if (cam.get_AABB().max.y > object_AABB.min.y && cam.get_AABB().min.y < object_AABB.max.y) {
            overlap.y = std::min(cam.get_AABB().max.y, object_AABB.max.y) - std::max(cam.get_AABB().min.y, object_AABB.min.y);
        }

        if (cam.get_AABB().max.z > object_AABB.min.z && cam.get_AABB().min.z < object_AABB.max.z) {
            overlap.z = std::min(cam.get_AABB().max.z, object_AABB.max.z) - std::max(cam.get_AABB().min.z, object_AABB.min.z);
        }

        return overlap;
    }
};

}

#endif
