#ifndef CHUNK_HPP
#define CHUNK_HPP

namespace chunk {

struct chunk {
    chunk(const int X, const int Z, const noise::noise &noise) {
        for (auto x = 0; x != CHUNK_SIZE_X; ++x) for (auto y = 0; y != CHUNK_SIZE_Y; ++y) for (auto z = 0; z != CHUNK_SIZE_Z; ++z) {
            auto MAX {
                32 + abs(floor(8.0f * noise.get(static_cast<float>(x + Z), static_cast<float>(z + X))))
            };

            if      (y == 0)           m_block.push_back(tools::BLOCK_TYPE::MAGMA);
            else if (y <= MAX - 2)     m_block.push_back(tools::BLOCK_TYPE::STONE);
            else if (y > 0 && y < MAX) m_block.push_back(tools::BLOCK_TYPE::DIRT);
            else if (y == MAX)         m_block.push_back(tools::BLOCK_TYPE::GRASS);
            else                       m_block.push_back(tools::BLOCK_TYPE::AIR);
        }

        auto index {0u};

        for (auto x = 0; x != CHUNK_SIZE_X; ++x) for (auto y = 0; y != CHUNK_SIZE_Y; ++y) for (auto z = 0; z != CHUNK_SIZE_Z; ++z) {
            if (get_block_type(x, y, z) == tools::BLOCK_TYPE::AIR)  continue;

            tools::face face;

            if (x > 0                  && get_block_type(x - 1, y, z) != tools::BLOCK_TYPE::AIR) face.L = false;
            if (y > 0                  && get_block_type(x, y - 1, z) != tools::BLOCK_TYPE::AIR) face.D = false;
            if (z > 0                  && get_block_type(x, y, z - 1) != tools::BLOCK_TYPE::AIR) face.F = false;
            if (x < (CHUNK_SIZE_X - 1) && get_block_type(x + 1, y, z) != tools::BLOCK_TYPE::AIR) face.R = false;
            if (y < (CHUNK_SIZE_Y - 1) && get_block_type(x, y + 1, z) != tools::BLOCK_TYPE::AIR) face.U = false;
            if (z < (CHUNK_SIZE_Z - 1) && get_block_type(x, y, z + 1) != tools::BLOCK_TYPE::AIR) face.B = false;

            mesh(index, x + X, y, z + Z, face, static_cast<int>(get_block_type(x, y, z)));
        }

        setup();
    }

    ~chunk() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers     (1, &m_VBO);
        glDeleteBuffers     (1, &m_EBO);
    }

    void draw(const shader::shader_program &shader, const unsigned int &texture, camera::camera &camera) const {
        glCullFace(GL_FRONT);

        shader.use();

        glBindTexture    (GL_TEXTURE_2D, texture);
        glBindVertexArray(m_VAO);

        shader.set_mat4("View", camera.get_view_matrix());
        shader.set_mat4("Projection", camera.get_projection_matrix());

        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        glCullFace(GL_BACK);
    }

private:
    unsigned int m_VAO {0u};
    unsigned int m_VBO {0u};
    unsigned int m_EBO {0u};

    std::vector<tools::BLOCK_TYPE>  m_block;
    std::vector<tools::vertex_3d_t> m_vertice;
    std::vector<unsigned int>       m_indices;

    void setup() {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers     (1, &m_VBO);
        glGenBuffers     (1, &m_EBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_vertice.size() * 6 * sizeof(float), &m_vertice.at(0), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices.at(0), GL_STATIC_DRAW);

        glVertexAttribPointer    (0, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(0 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer    (1, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        if (m_VAO == 0u) my_exception {__FILE__, __LINE__, "falha ao criar VAO do 'chunk'"};
    }

    tools::BLOCK_TYPE get_block_type(int x, int y, int z) const {
        return m_block.at(x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y);
    }

    void mesh(unsigned int &i, const int x, const int y, const int z, const tools::face &face, const int block_type) {
        const auto X {static_cast<float>(x)};
        const auto Y {static_cast<float>(y)};
        const auto Z {static_cast<float>(z)};

        tools::face_texture tex;

        switch (block_type) {
        case static_cast<int>(tools::BLOCK_TYPE::GRASS):
            tex = {2.0f, 2.0f, 2.0f, 2.0f, 1.0f, 3.0f};
            break;
        case static_cast<int>(tools::BLOCK_TYPE::DIRT):
            tex = {3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f};
            break;
        case static_cast<int>(tools::BLOCK_TYPE::STONE):
            tex = {4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f};
            break;
        case static_cast<int>(tools::BLOCK_TYPE::SAND):
            tex = {5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f};
            break;
        case static_cast<int>(tools::BLOCK_TYPE::MAGMA):
            tex = {6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f};
            break;
        }

        if (face.F) {
            m_vertice.push_back({X - 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, tex.F});
            m_vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, tex.F});
            m_vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, tex.F});
            m_vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 1.0f, 1.0f, tex.F});

            std::initializer_list indices {
                i + 0, i + 1, i + 3, i + 3, i + 1, i + 2
            };

            m_indices.insert(m_indices.end(), indices);

            i += 4u;
        }

        if (face.B) {
            m_vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 0.0f, 1.0f, tex.B});
            m_vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 0.0f, 0.0f, tex.B});
            m_vertice.push_back({X + 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 0.0f, tex.B});
            m_vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, tex.B});

            std::initializer_list indices {
                i + 1, i + 0, i + 3, i + 1, i + 3, i + 2
            };

            m_indices.insert(m_indices.end(), indices);

            i += 4u;
        }

        if (face.R) {
            m_vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, tex.R});
            m_vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, tex.R});
            m_vertice.push_back({X + 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 0.0f, tex.R});
            m_vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, tex.R});

            std::initializer_list indices {
                i + 0, i + 1, i + 3, i + 3, i + 1, i + 2
            };

            m_indices.insert(m_indices.end(), indices);

            i += 4u;
        }

        if (face.L) {
            m_vertice.push_back({X - 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, tex.L});
            m_vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, tex.L});
            m_vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 0.0f, tex.L});
            m_vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, tex.L});

            std::initializer_list indices {
                i + 1, i + 0, i + 3, i + 1, i + 3, i + 2
            };

            m_indices.insert(m_indices.end(), indices);

            i += 4u;
        }

        if (face.U) {
            m_vertice.push_back({X-0.5f,Y+0.5f,Z+0.5f, 0.0f, 1.0f, tex.U});
            m_vertice.push_back({X-0.5f,Y+0.5f,Z-0.5f, 0.0f, 0.0f, tex.U});
            m_vertice.push_back({X+0.5f,Y+0.5f,Z-0.5f, 1.0f, 0.0f, tex.U});
            m_vertice.push_back({X+0.5f,Y+0.5f,Z+0.5f, 1.0f, 1.0f, tex.U});

            std::initializer_list indices {
                i + 0, i + 1, i + 3, i + 3, i + 1, i + 2
            };

            m_indices.insert(m_indices.end(), indices);

            i += 4u;
        }

        if (face.D) {
            m_vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 0.0f, 1.0f, tex.D});
            m_vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, tex.D});
            m_vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, tex.D});
            m_vertice.push_back({X + 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 1.0f, tex.D});

            std::initializer_list indices {
                i + 1, i + 0, i + 3, i + 1, i + 3, i + 2
            };

            m_indices.insert(m_indices.end(), indices);

            i += 4u;
        }
    }
};

}

#endif
