#ifndef GENERATE_TERRAIN_HPP
#define GENERATE_TERRAIN_HPP

namespace terrain_generator {

struct terrain_generator {
    terrain_generator(const int seed) : m_noise {seed} {
        m_noise.SetNoiseType     (FastNoiseLite::NoiseType::NoiseType_Perlin);
        m_noise.SetFractalType   (FastNoiseLite::FractalType::FractalType_Ridged);
        m_noise.SetRotationType3D(FastNoiseLite::RotationType3D::RotationType3D_ImproveXZPlanes);
        m_noise.SetFractalOctaves(1);
        m_noise.SetFractalGain   (4.0f);
        m_noise.SetFrequency     (0.01f);
    }

    void use(std::vector<BLOCK_TYPE> &block, int X, int Y, int Z) {
        for (auto z = 0; z != CHUNK_SIZE_Z; ++z) for (auto y = 0; y != CHUNK_SIZE_Y; ++y) for (auto x = 0; x != CHUNK_SIZE_X; ++x) {
            const auto height {get_height(x + X, y + Y, z + Z)};

            if      (y <=           3) block.push_back(BLOCK_TYPE::MAGMA);
            else if (y <= height - 24) block.push_back(BLOCK_TYPE::FELDSPAR);
            else if (y <= height -  4) block.push_back(BLOCK_TYPE::STONE);
            else if (y <= height     ) block.push_back(BLOCK_TYPE::DIRT);
            else if (y == height +  1) block.push_back(BLOCK_TYPE::GRASS);
            else                       block.push_back(BLOCK_TYPE::AIR);
        }
    }

private:
    FastNoiseLite m_noise;

    int get_height(float x, float y, float z) { return 64 + abs(floor(16.0f * m_noise.GetNoise(x, z))); }
};

}

#endif
