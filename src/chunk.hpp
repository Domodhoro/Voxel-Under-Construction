#ifndef CHUNK_HPP
#define CHUNK_HPP

enum class BLOCK_TYPE : int {
    AIR = 0,
    GRASS,
    DIRT,
    STONE
};

constexpr auto CHUNK_SIZE_X {16}, CHUNK_SIZE_Y {128}, CHUNK_SIZE_Z {CHUNK_SIZE_X};

void populate(int X, int Z, Noise &Noise, std::vector<int> &Block);
void optimize(int x, int y, int z, std::vector<int> &Block, bool &F, bool &B, bool &R, bool &L, bool &U, bool &D);

class Chunk {
public:
    Chunk(int X, int Z, unsigned int &Texture, Noise &Noise) : m_Texture {Texture} {
        populate(X, Z, Noise, m_Block);

        for (auto x = 0; x != CHUNK_SIZE_X; ++x) {
            for (auto y = 0; y != CHUNK_SIZE_Y; ++y) {
                for (auto z = 0; z != CHUNK_SIZE_Z; ++z) {
                    if (m_Block.at(x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y) == static_cast<int>(BLOCK_TYPE::AIR)) {
                        continue;
                    }

                    auto F {true}, B {true}, R {true}, L {true}, U {true}, D {true};

                    optimize(x, y, z, m_Block, F, B, R, L, U, D);

                    m_count += Mesh(m_Vertice, x + X, y, z + Z, F, B, R, L, U, D, static_cast<float>(BLOCK_TYPE::GRASS));
                }
            }
        }

        setup();
    }

    ~Chunk() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
    }

    void draw(Shader &ShaderProgram, const glm::mat4 &View, const glm::mat4 &Projection) {
        glCullFace(GL_FRONT);

        ShaderProgram.use();

        glBindTexture(GL_TEXTURE_2D, m_Texture);
        glBindVertexArray(m_VAO);

        glm::mat4 Model {1.0f};

        ShaderProgram.setMatrix4fv("Model", Model);
        ShaderProgram.setMatrix4fv("View", View);
        ShaderProgram.setMatrix4fv("Projection", Projection);

        glDrawArrays(GL_TRIANGLES, 0, m_count);

        glCullFace(GL_BACK);
    }

private:
    unsigned int m_VAO {0u}, m_VBO {0u}, m_count {0u}, m_Texture {0u};

    std::vector<Vertex> m_Vertice;
    std::vector<int> m_Block;

    void setup() {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_Vertice.size() * 6 * sizeof(float), &m_Vertice.at(0), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        if (m_VAO == 0u) {
            throw std::runtime_error {
                "Falha ao criar 'VAO'."
            };
        }
    }
};

void populate(int X, int Z, Noise &Noise, std::vector<int> &Block) {
    auto base {64};
    auto amplitude {8.0f};

    for (auto x = 0; x != CHUNK_SIZE_X; ++x) {
        for (auto y = 0; y != CHUNK_SIZE_Y; ++y) {
            for (auto z = 0; z != CHUNK_SIZE_Z; ++z) {
                auto MAX {
                    Noise.use().GetNoise(static_cast<float>(x + Z), static_cast<float>(z + X))
                };

                if (y < base + std::abs(std::floor(amplitude * MAX))) {
                    Block.emplace_back(static_cast<short int>(BLOCK_TYPE::GRASS));
                } else {
                    Block.emplace_back(static_cast<short int>(BLOCK_TYPE::AIR));
                }
            }
        }
    }
}

void optimize(int x, int y, int z, std::vector<int> &Block, bool &F, bool &B, bool &R, bool &L, bool &U, bool &D) {
    if (x > 0 && Block.at((x - 1) + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y) != static_cast<int>(BLOCK_TYPE::AIR)) {
        L = false;
    }

    if (y > 0 && Block.at(x + (y - 1) * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y) != static_cast<int>(BLOCK_TYPE::AIR)) {
        D = false;
    }

    if (z > 0 && Block.at(x + y * CHUNK_SIZE_X + (z - 1) * CHUNK_SIZE_X * CHUNK_SIZE_Y) != static_cast<int>(BLOCK_TYPE::AIR)) {
        F = false;
    }

    if (x < (CHUNK_SIZE_X - 1) && Block.at((x + 1) + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y) != static_cast<int>(BLOCK_TYPE::AIR)) {
        R = false;
    }

    if (y < (CHUNK_SIZE_Y - 1) && Block.at(x + (y + 1) * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y) != static_cast<int>(BLOCK_TYPE::AIR)) {
        U = false;
    }

    if (z < (CHUNK_SIZE_Z - 1) && Block.at(x + y * CHUNK_SIZE_X + (z + 1) * CHUNK_SIZE_X * CHUNK_SIZE_Y) != static_cast<int>(BLOCK_TYPE::AIR)) {
        B = false;
    }
}

#endif
