#ifndef CHUNK_MANAGER_HPP
#define CHUNK_MANAGER_HPP

namespace chunk_manager {

struct chunk_manager {
    chunk_manager(const noise::noise &noise) {
        tools::world_coords position {0, 0, 0};

        m_chunks.emplace_back(position, std::make_unique<chunk::chunk>(0, 0, noise));
    }

    void add_chunk(const glm::tvec3<float> &camera_position, const noise::noise &noise) {
        auto coords {chunk_coords(camera_position)};

        auto predicate = [&](std::pair<tools::world_coords, std::unique_ptr<chunk::chunk>> &chunk) -> bool {
            return chunk.first == coords;
        };

        if (std::find_if(m_chunks.begin(), m_chunks.end(), predicate) == m_chunks.end()) {
            const auto X {coords.x * CHUNK_SIZE_X};
            const auto Z {coords.z * CHUNK_SIZE_Z};

            m_chunks.emplace_back(coords, std::make_unique<chunk::chunk>(X, Z, noise));
        }
    }

    void draw(const shader::shader_program &shader, const unsigned int &texture, camera::camera &cam) const {
        for (auto &it : m_chunks) {
            it.second->draw(shader, texture, cam);
        }
    }

private:
    std::vector<std::pair<tools::world_coords, std::unique_ptr<chunk::chunk>>> m_chunks;

    tools::world_coords chunk_coords(const glm::tvec3<float> &position) const {
        tools::world_coords coords {
            static_cast<int>(floor(position.x / static_cast<float>(CHUNK_SIZE_X))),
            0,
            static_cast<int>(floor(position.z / static_cast<float>(CHUNK_SIZE_Z)))
        };

        return coords;
    }
};

}

#endif
