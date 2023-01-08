#ifndef CHUNK_HPP
#define CHUNK_HPP

namespace Chunk {

constexpr auto SIZE_X {16}, SIZE_Y {16}, SIZE_Z {16};

struct Vertex {
    float x {0.0f}, y {0.0f}, z {0.0f}, u {0.0f}, v {0.0f};
};

unsigned int build(std::vector<Vertex>& vertice, int X, int Y, int Z, bool F, bool B, bool R, bool L, bool U, bool D);

class Chunk {
public:
    Chunk(const unsigned int& texture) : m_texture {texture} {
        for (auto x = 0; x != SIZE_X; ++x) {
            for (auto y = 0; y != SIZE_Y; ++y) {
                for (auto z = 0; z != SIZE_Z; ++z) {
                    m_count += build(m_vertice, x, y, z, true, true, true, true, true, true);
                }
            }
        }

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_vertice.size() * 6 * sizeof(float), &m_vertice.at(0), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        if (m_VAO == 0u) {
            throw std::string {"Falha ao criar VAO."};
        }

        m_shader = std::make_unique<Shader::Shader>("./glsl/vertex.glsl", "./glsl/fragment.glsl");
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

    std::vector<Vertex> m_vertice;
};

unsigned int build(std::vector<Vertex>& vertice, int X, int Y, int Z, bool F, bool B, bool R, bool L, bool U, bool D) {
    const auto x {static_cast<float>(X)}, y {static_cast<float>(Y)}, z {static_cast<float>(Z)};

    auto amount {0u};

    if (F) {
        vertice.push_back({x - 0.5f, y - 0.5f, z - 0.5f, 1.0f, 0.0f});
        vertice.push_back({x + 0.5f, y - 0.5f, z - 0.5f, 0.0f, 0.0f});
        vertice.push_back({x + 0.5f, y + 0.5f, z - 0.5f, 0.0f, 1.0f});

        vertice.push_back({x - 0.5f, y - 0.5f, z - 0.5f, 1.0f, 0.0f});
        vertice.push_back({x + 0.5f, y + 0.5f, z - 0.5f, 0.0f, 1.0f});
        vertice.push_back({x - 0.5f, y + 0.5f, z - 0.5f, 1.0f, 1.0f});

        amount += 6u;
    }

    if (B) {
        vertice.push_back({x - 0.5f, y - 0.5f, z + 0.5f, 0.0f, 0.0f});
        vertice.push_back({x + 0.5f, y + 0.5f, z + 0.5f, 1.0f, 1.0f});
        vertice.push_back({x + 0.5f, y - 0.5f, z + 0.5f, 1.0f, 0.0f});

        vertice.push_back({x - 0.5f, y + 0.5f, z + 0.5f, 0.0f, 1.0f});
        vertice.push_back({x + 0.5f, y + 0.5f, z + 0.5f, 1.0f, 1.0f});
        vertice.push_back({x - 0.5f, y - 0.5f, z + 0.5f, 0.0f, 0.0f});

        amount += 6u;
    }

    if (R) {
        vertice.push_back({x + 0.5f, y - 0.5f, z - 0.5f, 1.0f, 0.0f});
        vertice.push_back({x + 0.5f, y - 0.5f, z + 0.5f, 0.0f, 0.0f});
        vertice.push_back({x + 0.5f, y + 0.5f, z + 0.5f, 0.0f, 1.0f});

        vertice.push_back({x + 0.5f, y - 0.5f, z - 0.5f, 1.0f, 0.0f});
        vertice.push_back({x + 0.5f, y + 0.5f, z + 0.5f, 0.0f, 1.0f});
        vertice.push_back({x + 0.5f, y + 0.5f, z - 0.5f, 1.0f, 1.0f});

        amount += 6u;
    }

    if (L) {
        vertice.push_back({x - 0.5f, y - 0.5f, z + 0.5f, 1.0f, 0.0f});
        vertice.push_back({x - 0.5f, y - 0.5f, z - 0.5f, 0.0f, 0.0f});
        vertice.push_back({x - 0.5f, y + 0.5f, z + 0.5f, 1.0f, 1.0f});

        vertice.push_back({x - 0.5f, y + 0.5f, z - 0.5f, 0.0f, 1.0f});
        vertice.push_back({x - 0.5f, y + 0.5f, z + 0.5f, 1.0f, 1.0f});
        vertice.push_back({x - 0.5f, y - 0.5f, z - 0.5f, 0.0f, 0.0f});

        amount += 6u;
    }

    if (U) {
        vertice.push_back({x + 0.5f, y + 0.5f, z - 0.5f, 0.0f, 1.0f});
        vertice.push_back({x + 0.5f, y + 0.5f, z + 0.5f, 1.0f, 1.0f});
        vertice.push_back({x - 0.5f, y + 0.5f, z + 0.5f, 1.0f, 0.0f});

        vertice.push_back({x + 0.5f, y + 0.5f, z - 0.5f, 0.0f, 1.0f});
        vertice.push_back({x - 0.5f, y + 0.5f, z + 0.5f, 1.0f, 0.0f});
        vertice.push_back({x - 0.5f, y + 0.5f, z - 0.5f, 0.0f, 0.0f});

        amount += 6u;
    }

    if (D) {
        vertice.push_back({x - 0.5f, y - 0.5f, z + 0.5f, 1.0f, 1.0f});
        vertice.push_back({x + 0.5f, y - 0.5f, z + 0.5f, 1.0f, 0.0f});
        vertice.push_back({x + 0.5f, y - 0.5f, z - 0.5f, 0.0f, 0.0f});

        vertice.push_back({x - 0.5f, y - 0.5f, z - 0.5f, 0.0f, 1.0f});
        vertice.push_back({x - 0.5f, y - 0.5f, z + 0.5f, 1.0f, 1.0f});
        vertice.push_back({x + 0.5f, y - 0.5f, z - 0.5f, 0.0f, 0.0f});

        amount += 6u;
    }

    return amount;
}

}

#endif
