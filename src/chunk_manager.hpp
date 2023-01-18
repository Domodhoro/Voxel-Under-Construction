#ifndef CHUNK_MANAGER_HPP
#define CHUNK_MANAGER_HPP

namespace chunk_manager {

struct chunk_manager {
    chunk_manager(unsigned int &texture) : m_texture {texture} {}

    void add(const glm::tvec3<float> &position, FastNoiseLite &noise) {
        util::world_coords coordinates {
            static_cast<int>(floor(position.x / static_cast<float>(settings::CHUNK_SIZE_X))),
            0,
            static_cast<int>(floor(position.z / static_cast<float>(settings::CHUNK_SIZE_Z)))
        };

        auto predicate = [&](std::pair<util::world_coords, std::unique_ptr<chunk::chunk>> &chunk) -> bool {
            return chunk.first == coordinates;
        };

        if (std::find_if(m_chunks.begin(), m_chunks.end(), predicate) == m_chunks.end()) {
            m_chunks.emplace_back(coordinates, std::make_unique<chunk::chunk>(coordinates.x * settings::CHUNK_SIZE_X, coordinates.z * settings::CHUNK_SIZE_Z, m_texture, noise));
        }
    }

    void draw(shader::shader_program &shader, camera::camera &cam) const {
        for (auto &it : m_chunks) {
            it.second->draw(shader, cam);
        }
    }

private:
    std::vector<std::pair<util::world_coords, std::unique_ptr<chunk::chunk>>> m_chunks;

    unsigned int m_texture {0u};
};

}

#endif
