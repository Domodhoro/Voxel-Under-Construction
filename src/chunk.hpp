#ifndef CHUNK_HPP
#define CHUNK_HPP

namespace chunk {

struct chunk {
    chunk(const int X, const int Z, const unsigned int &texture, const FastNoiseLite &noise) : m_texture {texture} {
        for (auto x = 0; x != settings::CHUNK_SIZE_X; ++x) for (auto y = 0; y != settings::CHUNK_SIZE_Y; ++y) for (auto z = 0; z != settings::CHUNK_SIZE_Z; ++z) {
            const auto base      {32};
            const auto amplitude {8.0f};

            auto MAX {
                base + abs(floor(amplitude * noise.GetNoise(static_cast<float>(x + Z), static_cast<float>(z + X))))
            };

            if      (y <= 16)          m_block.push_back(util::BLOCK_TYPE::STONE);
            else if (y > 0 && y < MAX) m_block.push_back(util::BLOCK_TYPE::DIRT);
            else if (y == MAX)         m_block.push_back(util::BLOCK_TYPE::GRASS);
            else                       m_block.push_back(util::BLOCK_TYPE::AIR);
        }

        for (auto x = 0; x != settings::CHUNK_SIZE_X; ++x) for (auto y = 0; y != settings::CHUNK_SIZE_Y; ++y) for (auto z = 0; z != settings::CHUNK_SIZE_Z; ++z) {
            if (get_block_type(x, y, z) == util::BLOCK_TYPE::AIR)  continue;

            util::face face;

            if (x > 0                            && get_block_type(x - 1, y, z) != util::BLOCK_TYPE::AIR) face.L = false;
            if (y > 0                            && get_block_type(x, y - 1, z) != util::BLOCK_TYPE::AIR) face.D = false;
            if (z > 0                            && get_block_type(x, y, z - 1) != util::BLOCK_TYPE::AIR) face.F = false;
            if (x < (settings::CHUNK_SIZE_X - 1) && get_block_type(x + 1, y, z) != util::BLOCK_TYPE::AIR) face.R = false;
            if (y < (settings::CHUNK_SIZE_Y - 1) && get_block_type(x, y + 1, z) != util::BLOCK_TYPE::AIR) face.U = false;
            if (z < (settings::CHUNK_SIZE_Z - 1) && get_block_type(x, y, z + 1) != util::BLOCK_TYPE::AIR) face.B = false;

            chunk_mesh::mesh(m_vertice, m_count, x + X, y, z + Z, face, static_cast<int>(get_block_type(x, y, z)));
        }

        setup();
    }

    ~chunk() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers     (1, &m_VBO);
    }

    void draw(shader::shader_program &shader, camera::camera &camera) const {
        glCullFace(GL_FRONT);

        shader.use();

        glBindTexture    (GL_TEXTURE_2D, m_texture);
        glBindVertexArray(m_VAO);

        shader.set_mat4("View", camera.get_view_matrix());
        shader.set_mat4("Projection", camera.get_projection_matrix());

        glDrawArrays(GL_TRIANGLES, 0, m_count);

        glCullFace(GL_BACK);
    }

private:
    unsigned int m_VAO     {0u};
    unsigned int m_VBO     {0u};
    unsigned int m_count   {0u};
    unsigned int m_texture {0u};

    std::vector<util::vertex>     m_vertice;
    std::vector<util::BLOCK_TYPE> m_block;

    void setup() {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers     (1, &m_VBO);
        glBindVertexArray(m_VAO);

        try {
            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            glBufferData(GL_ARRAY_BUFFER, m_vertice.size() * 6 * sizeof(float), &m_vertice.at(0), GL_STATIC_DRAW);

            glVertexAttribPointer    (0, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(0 * sizeof(float)));
            glEnableVertexAttribArray(0);
            glVertexAttribPointer    (1, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            if (m_VAO == 0u) throw util::program_exception {"Falha ao criar VAO."};
        } catch (util::program_exception &e) {
            printf("%s", e.get_description());
        }
    }

    util::BLOCK_TYPE get_block_type(int x, int y, int z) const {
        return m_block.at(x + y * settings::CHUNK_SIZE_X + z * settings::CHUNK_SIZE_X * settings::CHUNK_SIZE_Y);
    }
};

}

#endif
