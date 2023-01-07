#ifndef CHUNK_HPP
#define CHUNK_HPP

namespace Chunk {

class Chunk {
public:
    Chunk(const int seed) {
        m_noise = std::make_unique<FastNoiseLite>(seed);

        m_noise->SetNoiseType(FastNoiseLite::NoiseType::NoiseType_OpenSimplex2);
        m_noise->SetFrequency(0.01f);
        m_noise->SetFractalLacunarity(100.0f);
        m_noise->SetFractalGain(0.1f);

        m_shader = std::make_unique<Shader::Shader>("./glsl/chunkVertex.glsl", "./glsl/chunkFragment.glsl");

        m_texture = STB_IMAGE::loadTexture("img/blocks.bmp");

        if (m_texture == 0u) {
            throw std::string {"Falha ao carregar textura."};
        }

        m_mesh.emplace_back(glm::tvec3<long int>(0), std::make_unique<Mesh::Mesh>(glm::tvec3<long int>(0), m_noise));
    }

    virtual ~Chunk() {
        glDeleteTextures(1, &m_texture);
    }

    void addChunk(const std::unique_ptr<Camera::Camera>& camera) {
        glm::tvec3<long int> coords {
            floor(camera->getPosition().x / static_cast<float>(Mesh::SIZE)),
            0.0f,
            floor(camera->getPosition().z / static_cast<float>(Mesh::SIZE))
        };

        auto predicate = [&](std::pair<glm::tvec3<long int>, std::unique_ptr<Mesh::Mesh>>& chunk) -> bool {
            return chunk.first == coords;
        };

        if (std::find_if(m_mesh.begin(), m_mesh.end(), predicate) == m_mesh.end()) {
            m_mesh.emplace_back(coords, std::make_unique<Mesh::Mesh>(coords, m_noise));
        }
    }

    void removeChunk() {
        if (m_mesh.size() > 1) {
            m_mesh.erase(m_mesh.begin());
        }
    }

    void draw(const glm::mat4& view, const glm::mat4& projection) {
        glCullFace(GL_FRONT);

        m_shader->use();

        glBindTexture(GL_TEXTURE_2D, m_texture);

        glm::mat4 model {1.0f};

        m_shader->setMatrix4fv("model", model);
        m_shader->setMatrix4fv("view", view);
        m_shader->setMatrix4fv("projection", projection);

        for (const auto& it : m_mesh) {
            glBindVertexArray(it.second->getVertexArray());

            glDrawArrays(GL_TRIANGLES, 0, it.second->getCount());
        }

        glCullFace(GL_BACK);
    }

protected:
    unsigned int m_texture {0u};

    std::unique_ptr<Shader::Shader> m_shader;
    std::unique_ptr<FastNoiseLite> m_noise;

    std::vector<std::pair<glm::tvec3<long int>, std::unique_ptr<Mesh::Mesh>>> m_mesh;
};

}

#endif
