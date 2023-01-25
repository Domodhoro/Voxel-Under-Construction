#ifndef GENERATE_TERRAIN_HPP
#define GENERATE_TERRAIN_HPP

namespace terrain_generator {

struct terrain_generator {
    terrain_generator(const int seed) {
        m_noise[0].SetSeed          (seed);
        m_noise[0].SetNoiseType     (FastNoiseLite::NoiseType_Perlin);
        m_noise[0].SetFractalType   (FastNoiseLite::FractalType_DomainWarpProgressive);
        m_noise[0].SetFractalOctaves(1);
        m_noise[0].SetFractalGain   (4.0f);
        m_noise[0].SetFrequency     (0.01f);

        m_noise[1].SetSeed          (seed);
        m_noise[1].SetNoiseType     (FastNoiseLite::NoiseType_Perlin);
        m_noise[1].SetFractalType   (FastNoiseLite::FractalType_DomainWarpProgressive);
        m_noise[1].SetFractalOctaves(2);
        m_noise[1].SetFractalGain   (2.0f);
        m_noise[1].SetFrequency     (0.02f);

        m_noise[2].SetSeed          (seed);
        m_noise[2].SetNoiseType     (FastNoiseLite::NoiseType_Perlin);
        m_noise[2].SetFractalType   (FastNoiseLite::FractalType_DomainWarpProgressive);
        m_noise[2].SetFractalOctaves(3);
        m_noise[2].SetFractalGain   (1.0f);
        m_noise[2].SetFrequency     (0.04f);
    }

    void set_minimum_height     (const int minimum_height)        { m_minimum_height = minimum_height; }
    void set_amplitude_variation(const float amplitude_variation) { m_amplitude_variation = amplitude_variation; }

    void terrain(std::vector<tools::BLOCK_TYPE> &block, const int X, const int Z) {
        for (auto x = 0; x != CHUNK_SIZE_X; ++x) for (auto y = 0; y != CHUNK_SIZE_Y; ++y) for (auto z = 0; z != CHUNK_SIZE_Z; ++z) {
            auto MAX {
                get(static_cast<float>(x + Z), static_cast<float>(z + X))
            };

            if      (y <= 3)           block.push_back(tools::BLOCK_TYPE::MAGMA);
            else if (y <= MAX - 24)    block.push_back(tools::BLOCK_TYPE::FELDSPAR);
            else if (y <= MAX - 4)     block.push_back(tools::BLOCK_TYPE::STONE);
            else if (y > 0 && y < MAX) block.push_back(tools::BLOCK_TYPE::DIRT);
            else if (y == MAX)         block.push_back(tools::BLOCK_TYPE::GRASS);
            else                       block.push_back(tools::BLOCK_TYPE::AIR);
        }
    }

private:
    FastNoiseLite m_noise[NOISE_MAX];

    int   m_minimum_height      {4};
    float m_amplitude_variation {2.0f};

    int get(const float x, const float z) const {
        auto result {0.0f};

        for (auto i = 0; i != NOISE_MAX; ++i) result += m_noise[i].GetNoise(x, z);

        return m_minimum_height + abs(floor(m_amplitude_variation * result));
    }
};

}

#endif
