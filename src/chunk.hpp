#ifndef CHUNK_HPP
#define CHUNK_HPP

namespace Chunk {

constexpr auto SIZE_X {16}, SIZE_Y {128}, SIZE_Z {16};

class Chunk {
public:
    Chunk() {
        for (auto x = 0; x != SIZE_X; ++x) {
            for (auto y = 0; y != SIZE_Y; ++y) {
                for (auto z = 0; z != SIZE_Z; ++z) {
                    m_count += ChunkMesh::Mesh(m_vertice, x, y, z, true, true, true, true, true, true);
                }
            }
        }

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

        m_shader = std::make_unique<Shader::Shader>("./glsl/vertex.glsl", "./glsl/fragment.glsl");
        m_texture = STB_IMAGE::loadTexture("./img/blocks.bmp");
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
};

}

#endif
