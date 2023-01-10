#ifndef CHUNK_HPP
#define CHUNK_HPP

namespace Chunk {

enum class BLOCK_TYPE : int {
    AIR = 0,
    GRASS
};

constexpr auto SIZE_X {16}, SIZE_Y {128}, SIZE_Z {16};

void populate(const int X, const int Z, FastNoiseLite& noise, std::vector<int>& block);
void otimization(int x, int y, int z, std::vector<int>& block, bool& F, bool& B, bool& R, bool& L, bool& U, bool& D);

class Chunk {
public:
    Chunk(const int X, const int Z, FastNoiseLite& noise) {
        m_shader = std::make_unique<Shader::Shader>("./glsl/vertex.glsl", "./glsl/fragment.glsl");

        m_texture = STB_IMAGE::loadTexture("./img/blocks.bmp");

        populate(X, Z, noise, m_block);

        for (auto x = 0; x != SIZE_X; ++x) {
            for (auto y = 0; y != SIZE_Y; ++y) {
                for (auto z = 0; z != SIZE_Z; ++z) {
                    if (m_block.at(x + y * SIZE_X + z * SIZE_X * SIZE_Y) == static_cast<short int>(BLOCK_TYPE::AIR)) {
                        continue;
                    }

                    auto F {true}, B {true}, R {true}, L {true}, U {true}, D {true};

                    otimization(x, y, z, m_block, F, B, R, L, U, D);

                    m_count += ChunkMesh::Mesh(m_vertice, x + Z, y, z + X, F, B, R, L, U, D);
                }
            }
        }

        setup();
    }

    ~Chunk() {
        glDeleteTextures(1, &m_texture);

        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
    }

    void draw(const glm::mat4& view, const glm::mat4& projection) {
        glCullFace(GL_FRONT);

        m_shader->use();

        glBindTexture(GL_TEXTURE_2D, m_texture);
        glBindVertexArray(m_VAO);

        glm::mat4 model {1.0f};

        m_shader->setMatrix4fv("model", model);
        m_shader->setMatrix4fv("view", view);
        m_shader->setMatrix4fv("projection", projection);

        glDrawArrays(GL_TRIANGLES, 0, m_count);

        glCullFace(GL_BACK);
    }

private:
    unsigned int m_VAO {0u}, m_VBO {0u}, m_count {0u}, m_texture {0u};

    std::unique_ptr<Shader::Shader> m_shader;

    std::vector<ChunkMesh::Vertex> m_vertice;
    std::vector<int> m_block;

    void setup() {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_vertice.size() * 5 * sizeof(float), &m_vertice.at(0), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        if (m_VAO == 0u) {
            throw std::string {"Falha ao criar VAO."};
        }
    }
};

void populate(const int X, const int Z, FastNoiseLite& noise, std::vector<int>& block) {
    for (auto x = 0; x != SIZE_X; ++x) {
        for (auto y = 0; y != SIZE_Y; ++y) {
            for (auto z = 0; z != SIZE_Z; ++z) {
                auto MAX {
                    noise.GetNoise(static_cast<float>(x + Z), static_cast<float>(z + X))
                };

                if (y < 64 + std::abs(std::floor(16.0f * MAX))) {
                    block.emplace_back(static_cast<short int>(BLOCK_TYPE::GRASS));
                } else {
                    block.emplace_back(static_cast<short int>(BLOCK_TYPE::AIR));
                }
            }
        }
    }
}

void otimization(int x, int y, int z, std::vector<int>& block, bool& F, bool& B, bool& R, bool& L, bool& U, bool& D) {
    if (x > 0 && block.at((x - 1) + y * SIZE_X + z * SIZE_X * SIZE_Y) != static_cast<int>(BLOCK_TYPE::AIR)) {
        L = false;
    }

    if (y > 0 && block.at(x + (y - 1) * SIZE_X + z * SIZE_X * SIZE_Y) != static_cast<int>(BLOCK_TYPE::AIR)) {
        D = false;
    }

    if (z > 0 && block.at(x + y * SIZE_X + (z - 1) * SIZE_X * SIZE_Y) != static_cast<int>(BLOCK_TYPE::AIR)) {
        F = false;
    }

    if (x < (SIZE_X - 1) && block.at((x + 1) + y * SIZE_X + z * SIZE_X * SIZE_Y) != static_cast<int>(BLOCK_TYPE::AIR)) {
        R = false;
    }

    if (y < (SIZE_Y - 1) && block.at(x + (y + 1) * SIZE_X + z * SIZE_X * SIZE_Y) != static_cast<int>(BLOCK_TYPE::AIR)) {
        U = false;
    }

    if (z < (SIZE_Z - 1) && block.at(x + y * SIZE_X + (z + 1) * SIZE_X * SIZE_Y) != static_cast<int>(BLOCK_TYPE::AIR)) {
        B = false;
    }
}

}

#endif
