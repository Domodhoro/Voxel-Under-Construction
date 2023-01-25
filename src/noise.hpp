#ifndef NOISE_HPP
#define NOISE_HPP

namespace noise {

struct noise {
    noise(const int seed) {
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

    int get(const float X, const float Z) const {
        const auto result {
            m_noise[0].GetNoise(X, Z) + m_noise[1].GetNoise(X, Z) + m_noise[2].GetNoise(X, Z)
        };

        return m_minimum_height + abs(floor(m_amplitude_variation * result));
    }

private:
    FastNoiseLite m_noise[NOISE_MAX];

    int   m_minimum_height      {4};
    float m_amplitude_variation {2.0f};
};

}

#endif
