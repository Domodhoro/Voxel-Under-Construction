#ifndef CHUNK_HPP
#define CHUNK_HPP

enum class BLOCK_TYPE : int {
    AIR = 0,
    GRASS,
    DIRT,
    STONE
};

constexpr auto CHUNK_SIZE_X {16}, CHUNK_SIZE_Y {128}, CHUNK_SIZE_Z {CHUNK_SIZE_X};

void terrain(std::vector<BLOCK_TYPE> &block, FastNoiseLite &noise, int X, int Z);

class Chunk {
public:
    Chunk(int X, int Z, unsigned int &texture, FastNoiseLite &noise) : m_texture {texture} {
        terrain(m_block, noise, X, Z);

        for (auto x = 0; x != CHUNK_SIZE_X; ++x) {
            for (auto y = 0; y != CHUNK_SIZE_Y; ++y) {
                for (auto z = 0; z != CHUNK_SIZE_Z; ++z) {
                    if (m_block.at(x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y) == BLOCK_TYPE::AIR) {
                        continue;
                    }

                    auto F {true}, B {true}, R {true}, L {true}, U {true}, D {true};

                    if (x > 0 && m_block.at((x - 1) + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y) != BLOCK_TYPE::AIR) {
                        L = false;
                    }

                    if (y > 0 && m_block.at(x + (y - 1) * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y) != BLOCK_TYPE::AIR) {
                        D = false;
                    }

                    if (z > 0 && m_block.at(x + y * CHUNK_SIZE_X + (z - 1) * CHUNK_SIZE_X * CHUNK_SIZE_Y) != BLOCK_TYPE::AIR) {
                        F = false;
                    }

                    if (x < (CHUNK_SIZE_X - 1) && m_block.at((x + 1) + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y) != BLOCK_TYPE::AIR) {
                        R = false;
                    }

                    if (y < (CHUNK_SIZE_Y - 1) && m_block.at(x + (y + 1) * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y) != BLOCK_TYPE::AIR) {
                        U = false;
                    }

                    if (z < (CHUNK_SIZE_Z - 1) && m_block.at(x + y * CHUNK_SIZE_X + (z + 1) * CHUNK_SIZE_X * CHUNK_SIZE_Y) != BLOCK_TYPE::AIR) {
                        B = false;
                    }

                    m_count += Mesh(m_vertice, x + X, y, z + Z, F, B, R, L, U, D, static_cast<int>(m_block.at(x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y)));
                }
            }
        }

        setup();
    }

    ~Chunk() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
    }

    void draw(Shader &shader, glm::mat4 &View, glm::mat4 &Projection) {
        glCullFace(GL_FRONT);

        shader.use();

        glBindTexture(GL_TEXTURE_2D, m_texture);
        glBindVertexArray(m_VAO);

        glm::mat4 Model {1.0f};

        shader.setMatrix4fv("Model", Model);
        shader.setMatrix4fv("View", View);
        shader.setMatrix4fv("Projection", Projection);

        glDrawArrays(GL_TRIANGLES, 0, m_count);

        glCullFace(GL_BACK);
    }

private:
    unsigned int m_VAO {0u}, m_VBO {0u}, m_count {0u}, m_texture {0u};

    std::vector<Vertex> m_vertice;
    std::vector<BLOCK_TYPE> m_block;

    void setup() {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_vertice.size() * 6 * sizeof(float), &m_vertice.at(0), GL_STATIC_DRAW);

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

void terrain(std::vector<BLOCK_TYPE> &block, FastNoiseLite &noise, int X, int Z) {
    auto base {64};
    auto amplitude {8.0f};

    for (auto x = 0; x != CHUNK_SIZE_X; ++x) {
        for (auto y = 0; y != CHUNK_SIZE_Y; ++y) {
            for (auto z = 0; z != CHUNK_SIZE_Z; ++z) {
                auto MAX {
                    base + std::abs(std::floor(amplitude * noise.GetNoise(static_cast<float>(x + Z), static_cast<float>(z + X))))
                };

                if (y <= 16) {
                    block.emplace_back(BLOCK_TYPE::STONE);
                } else if (y > 0 && y < MAX) {
                    block.emplace_back(BLOCK_TYPE::DIRT);
                } else if (y == MAX) {
                    block.emplace_back(BLOCK_TYPE::GRASS);
                } else {
                    block.emplace_back(BLOCK_TYPE::AIR);
                }
            }
        }
    }
}

#endif
