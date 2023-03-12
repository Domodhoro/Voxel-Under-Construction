#ifndef TERRAIN_GENERATOR_HPP
#define TERRAIN_GENERATOR_HPP

namespace terrain_generator {

struct terrain_generator {
    terrain_generator(const int seed) : noise {seed} {
        noise.SetRotationType3D(FastNoiseLite::RotationType3D::RotationType3D_ImproveXZPlanes);
    }

    void use(std::vector<BLOCK> &block, int X, int Y, int Z) const {
        for (auto z = 0; z != CHUNK_SIZE_Z; ++z) for (auto y = 0; y != CHUNK_SIZE_Y; ++y) for (auto x = 0; x != CHUNK_SIZE_X; ++x) {
            const auto height {get_height(x + X, y + Y, z + Z)};

            if      (y <=          1) block.push_back(BLOCK::MAGMA);
            else if (y <= height - 2) block.push_back(BLOCK::FELDSPAR);
            else if (y <= height - 3) block.push_back(BLOCK::STONE);
            else if (y <= height    ) block.push_back(BLOCK::DIRT);
            else if (y == height + 1) block.push_back(BLOCK::GRASS);
            else                      block.push_back(BLOCK::AIR);
        }
    }

protected:
    FastNoiseLite noise {};

    int get_height(float x, float y, float z) const { return 64 + abs(floor(4.0f * noise.GetNoise(x, z))); }
};

}

#endif
