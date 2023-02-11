#ifndef AABB_HPP
#define AABB_HPP

namespace AABB {

struct AABB {
    glm::tvec3<float> min {0.0f};
    glm::tvec3<float> max {1.0f};

    AABB(glm::tvec3<float> center, float hw, float hh, float hd) {
        min = center - glm::tvec3<float>(hw, hh, hd);
        max = center + glm::tvec3<float>(hw, hh, hd);
    }
};

bool check_camera_collision(const AABB &cam_AABB, const AABB &object_AABB) {
    if (cam_AABB.max.x < object_AABB.min.x || cam_AABB.min.x > object_AABB.max.x) return false;
    if (cam_AABB.max.y < object_AABB.min.y || cam_AABB.min.y > object_AABB.max.y) return false;
    if (cam_AABB.max.z < object_AABB.min.z || cam_AABB.min.z > object_AABB.max.z) return false;

    return true;
}

}

#endif
