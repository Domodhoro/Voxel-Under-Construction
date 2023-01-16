#ifndef CHUNK_HPP
#define CHUNK_HPP

constexpr auto CHUNK_SIZE_X {16}, CHUNK_SIZE_Y {128}, CHUNK_SIZE_Z {CHUNK_SIZE_X}, BASE {64};
constexpr auto AMPLITUDE {8.0f};

enum class BLOCK_TYPE : int {
    AIR = 0,
    GRASS,
    DIRT,
    STONE,
    SAND
};

struct Vertex {
    float X {0.0f}, Y {0.0f}, Z {0.0f}, U {0.0f}, V {0.0f}, T {0.0f};
};

struct Faces {
    bool F {true}, B {true}, R {true}, L {true}, U {true}, D {true};
};

unsigned int mesh(std::vector<Vertex> &Vertice, int x, int y, int z, Faces &faces, int blockType);

void generateTerrain(std::vector<BLOCK_TYPE> &block, FastNoiseLite &noise, int X, int Z);
void otimizeFaces(Faces &faces, int x, int y, int z, std::vector<BLOCK_TYPE> &block);

class Chunk {
public:
    Chunk(int X, int Z, unsigned int &texture, FastNoiseLite &noise) : m_texture {texture} {
        generateTerrain(m_block, noise, X, Z);

        for (auto x = 0; x != CHUNK_SIZE_X; ++x) {
            for (auto y = 0; y != CHUNK_SIZE_Y; ++y) {
                for (auto z = 0; z != CHUNK_SIZE_Z; ++z) {
                    if (m_block.at(x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y) == BLOCK_TYPE::AIR) {
                        continue;
                    }

                    Faces faces;

                    otimizeFaces(faces, x, y, z, m_block);

                    m_count += mesh(m_vertice, x + X, y, z + Z, faces, static_cast<int>(m_block.at(x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y)));
                }
            }
        }

        setup();
    }

    void draw(Shader &shader, glm::mat4 &view, glm::mat4 &projection) {
        glCullFace(GL_FRONT);

        shader.use();

        glBindTexture(GL_TEXTURE_2D, m_texture);
        glBindVertexArray(m_VAO);

        auto model {glm::mat4(1.0f)};

        shader.setMatrix4fv("model", model);
        shader.setMatrix4fv("view", view);
        shader.setMatrix4fv("projection", projection);

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

void generateTerrain(std::vector<BLOCK_TYPE> &block, FastNoiseLite &noise, int X, int Z) {
    for (auto x = 0; x != CHUNK_SIZE_X; ++x) {
        for (auto y = 0; y != CHUNK_SIZE_Y; ++y) {
            for (auto z = 0; z != CHUNK_SIZE_Z; ++z) {
                auto MAX {
                    BASE + std::abs(std::floor(AMPLITUDE * noise.GetNoise(static_cast<float>(x + Z), static_cast<float>(z + X))))
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

void otimizeFaces(Faces &faces, int x, int y, int z, std::vector<BLOCK_TYPE> &block) {
    if (x > 0 && block.at((x - 1) + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y) != BLOCK_TYPE::AIR) {
        faces.L = false;
    }

    if (y > 0 && block.at(x + (y - 1) * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y) != BLOCK_TYPE::AIR) {
        faces.D = false;
    }

    if (z > 0 && block.at(x + y * CHUNK_SIZE_X + (z - 1) * CHUNK_SIZE_X * CHUNK_SIZE_Y) != BLOCK_TYPE::AIR) {
        faces.F = false;
    }

    if (x < (CHUNK_SIZE_X - 1) && block.at((x + 1) + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y) != BLOCK_TYPE::AIR) {
        faces.R = false;
    }

    if (y < (CHUNK_SIZE_Y - 1) && block.at(x + (y + 1) * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y) != BLOCK_TYPE::AIR) {
        faces.U = false;
    }

    if (z < (CHUNK_SIZE_Z - 1) && block.at(x + y * CHUNK_SIZE_X + (z + 1) * CHUNK_SIZE_X * CHUNK_SIZE_Y) != BLOCK_TYPE::AIR) {
        faces.B = false;
    }
}

unsigned int mesh(std::vector<Vertex> &Vertice, int x, int y, int z, Faces &faces, int blockType) {
    const auto X {static_cast<float>(x)}, Y {static_cast<float>(y)}, Z {static_cast<float>(z)};

    struct Textures {
        float F {0.0f}, B {0.0f}, R {0.0f}, L {0.0f}, U {0.0f}, D {0.0f};
    };

    Textures textures;

    switch (blockType) {
    case static_cast<int>(BLOCK_TYPE::GRASS):
        textures.F = 2.0f;
        textures.B = 2.0f;
        textures.R = 2.0f;
        textures.L = 2.0f;
        textures.U = 1.0f;
        textures.D = 3.0f;
        break;
    case static_cast<int>(BLOCK_TYPE::DIRT):
        textures.F = 3.0f;
        textures.B = 3.0f;
        textures.R = 3.0f;
        textures.L = 3.0f;
        textures.U = 3.0f;
        textures.D = 3.0f;
        break;
    case static_cast<int>(BLOCK_TYPE::STONE):
        textures.F = 4.0f;
        textures.B = 4.0f;
        textures.R = 4.0f;
        textures.L = 4.0f;
        textures.U = 4.0f;
        textures.D = 4.0f;
        break;
    case static_cast<int>(BLOCK_TYPE::SAND):
        textures.F = 5.0f;
        textures.B = 5.0f;
        textures.R = 5.0f;
        textures.L = 5.0f;
        textures.U = 5.0f;
        textures.D = 5.0f;
        break;
    default:
        textures.F = 0.0f;
        textures.B = 0.0f;
        textures.R = 0.0f;
        textures.L = 0.0f;
        textures.U = 0.0f;
        textures.D = 0.0f;
    }

    auto count {0u};

    if (faces.F) {
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, textures.F});
        Vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, textures.F});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, textures.F});

        Vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, textures.F});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, textures.F});
        Vertice.push_back({X - 0.5f, Y + 0.5f, Z - 0.5f, 1.0f, 1.0f, textures.F});

        count += 6u;
    }

    if (faces.B) {
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 0.0f, 0.0f, textures.B});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, textures.B});
        Vertice.push_back({X + 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 0.0f, textures.B});

        Vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 0.0f, 1.0f, textures.B});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, textures.B});
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 0.0f, 0.0f, textures.B});

        count += 6u;
    }

    if (faces.R) {
        Vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, textures.R});
        Vertice.push_back({X + 0.5f, Y - 0.5f, Z + 0.5f, 0.0f, 0.0f, textures.R});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 0.0f, 1.0f, textures.R});

        Vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, textures.R});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 0.0f, 1.0f, textures.R});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 1.0f, 1.0f, textures.R});

        count += 6u;
    }

    if (faces.L) {
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 0.0f, textures.L});
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, textures.L});
        Vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, textures.L});

        Vertice.push_back({X - 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, textures.L});
        Vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, textures.L});
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, textures.L});

        count += 6u;
    }

    if (faces.U) {
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, textures.U});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, textures.U});
        Vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 0.0f, textures.U});

        Vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, textures.U});
        Vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 0.0f, textures.U});
        Vertice.push_back({X - 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 0.0f, textures.U});

        count += 6u;
    }

    if (faces.D) {
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 1.0f, textures.D});
        Vertice.push_back({X + 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 0.0f, textures.D});
        Vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, textures.D});

        Vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 1.0f, textures.D});
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 1.0f, textures.D});
        Vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, textures.D});

        count += 6u;
    }

    return count;
}

#endif
