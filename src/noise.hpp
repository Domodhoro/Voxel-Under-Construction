#ifndef NOISE_HPP
#define NOISE_HPP

class Noise {
public:
    Noise() : m_Noise {1007} {

    }

    FastNoiseLite &use() {
        return m_Noise;
    }

    void setSeed(int seed) {
        m_Noise.SetSeed(seed);
    }

private:
    FastNoiseLite m_Noise;
};

#endif
