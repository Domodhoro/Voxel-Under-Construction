#ifndef AABB_HPP
#define AABB_HPP

namespace AABB {

bool collision(chunk::chunk &chunk, camera::camera &camera) {
    const auto X {chunk.get_position().x};
    const auto Y {chunk.get_position().y};
    const auto Z {chunk.get_position().z};
    auto x       {static_cast<int>(floor(camera.get_position().x))};
    auto y       {static_cast<int>(floor(camera.get_position().y))};
    auto z       {static_cast<int>(floor(camera.get_position().z))};

    if (x >= X && x < X + CHUNK_SIZE_X && y >= Y && y < Y + CHUNK_SIZE_Y && z >= Z && z < Z + CHUNK_SIZE_Z) {
        x %= CHUNK_SIZE_X;
        y %= CHUNK_SIZE_Y;
        z %= CHUNK_SIZE_Z;

        if (chunk.get_block_type(abs(x), abs(y), abs(z)) != tools::BLOCK_TYPE::AIR) return true;
    } else {
        return false;
    }
}

}

#endif
