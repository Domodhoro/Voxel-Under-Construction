#ifndef CHUNK_HPP
#define CHUNK_HPP

namespace chunk {

struct chunk {
    chunk(const int X, const int Y, const int Z, terrain_generator::terrain_generator &terrain) : m_Position {X, Y, Z} {
        terrain.use(m_block, X, Y, Z);

        // test...................

        m_block.at(0 + 100 * CHUNK_SIZE_X + 0 * CHUNK_SIZE_X * CHUNK_SIZE_Y) = BLOCK_TYPE::STONE;

        // test...................

        auto i {0u};

        for (auto z = 0; z != CHUNK_SIZE_Z; ++z) for (auto y = 0; y != CHUNK_SIZE_Y; ++y) for (auto x = 0; x != CHUNK_SIZE_X; ++x) {
            if (get_block_type(x, y, z) == BLOCK_TYPE::AIR) continue;

            face<bool> faces {true, true, true, true, true, true};

            if (x > 0                  && get_block_type(x - 1, y, z) != BLOCK_TYPE::AIR) faces.left  = false;
            if (y > 0                  && get_block_type(x, y - 1, z) != BLOCK_TYPE::AIR) faces.down  = false;
            if (z > 0                  && get_block_type(x, y, z - 1) != BLOCK_TYPE::AIR) faces.front = false;
            if (x < (CHUNK_SIZE_X - 1) && get_block_type(x + 1, y, z) != BLOCK_TYPE::AIR) faces.right = false;
            if (y < (CHUNK_SIZE_Y - 1) && get_block_type(x, y + 1, z) != BLOCK_TYPE::AIR) faces.up    = false;
            if (z < (CHUNK_SIZE_Z - 1) && get_block_type(x, y, z + 1) != BLOCK_TYPE::AIR) faces.back  = false;

            mesh(i, x + X, y + Y, z + Z, faces, static_cast<int>(get_block_type(x, y, z)));
        }

        mesh_setup();
    }

    ~chunk() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers     (1, &m_VBO);
        glDeleteBuffers     (1, &m_EBO);
    }

    BLOCK_TYPE get_block_type(int x, int y, int z) const { return m_block.at(x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y); }

    AABB get_AABB(int x, int y, int z) const {
        const AABB aabb {glm::tvec3<float>(x + 0.5f, y + 0.5f, z + 0.5f), 0.5f, 0.5f, 0.5f};

        if (get_block_type(x, y, z) != BLOCK_TYPE::AIR) return aabb;
    }

    void draw(shader::shader_program &shader, const unsigned int &texture, camera::camera &camera) const {
        glCullFace(GL_FRONT);

        shader.use     ();
        shader.set_mat4("View", camera.get_view_matrix());
        shader.set_mat4("Projection", camera.get_projection_matrix());

        glBindTexture    (GL_TEXTURE_2D, texture);
        glBindVertexArray(m_VAO);
        glDrawElements   (GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, (void*)(0));
        glBindVertexArray(0);

        glCullFace(GL_BACK);
    }

protected:
    glm::tvec3<int> m_Position {0};

    unsigned int m_VAO {0u};
    unsigned int m_VBO {0u};
    unsigned int m_EBO {0u};

    std::vector<BLOCK_TYPE>         m_block;
    std::vector<vertex_3d_t<float>> m_vertice;
    std::vector<unsigned int>       m_indices;

    void mesh_setup() {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers     (1, &m_VBO);
        glGenBuffers     (1, &m_EBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_vertice.size() * sizeof(vertex_3d_t<float>), &m_vertice.at(0), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices.at(0), GL_STATIC_DRAW);

        glVertexAttribPointer    (0, 3, GL_FLOAT, false, sizeof(vertex_3d_t<float>), (void*)(0 * sizeof(float)));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer    (1, 3, GL_FLOAT, false, sizeof(vertex_3d_t<float>), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        if (m_VAO == 0u) my_exception {__FILE__, __LINE__, "falha ao criar VAO do 'chunk'"};

        glBindVertexArray(0);
    }

    void mesh(unsigned int &i, const int x, const int y, const int z, const face<bool> &faces, const int block_type) {
        face<float> textures {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

        switch (block_type) {
        case static_cast<int>(BLOCK_TYPE::GRASS):
            textures = {2.0f, 2.0f, 2.0f, 2.0f, 1.0f, 3.0f};
            break;
        case static_cast<int>(BLOCK_TYPE::DIRT):
            textures = {3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f};
            break;
        case static_cast<int>(BLOCK_TYPE::STONE):
            textures = {4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f};
            break;
        case static_cast<int>(BLOCK_TYPE::SAND):
            textures = {5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f};
            break;
        case static_cast<int>(BLOCK_TYPE::MAGMA):
            textures = {6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f};
            break;
        case static_cast<int>(BLOCK_TYPE::FELDSPAR):
            textures = {7.0f, 7.0f, 7.0f, 7.0f, 7.0f, 7.0f};
            break;
        }

        if (faces.front) {
            m_vertice.push_back({x - 0.0f, y + 1.0f, z - 0.0f, 0.0f, 1.0f, textures.front});
            m_vertice.push_back({x - 0.0f, y - 0.0f, z - 0.0f, 0.0f, 0.0f, textures.front});
            m_vertice.push_back({x + 1.0f, y - 0.0f, z - 0.0f, 1.0f, 0.0f, textures.front});
            m_vertice.push_back({x + 1.0f, y + 1.0f, z - 0.0f, 1.0f, 1.0f, textures.front});

            m_indices.insert(m_indices.end(), {i + 0u, i + 1u, i + 3u, i + 3u, i + 1u, i + 2u});

            i += 4u;
        }

        if (faces.back) {
            m_vertice.push_back({x - 0.0f, y + 1.0f, z + 1.0f, 0.0f, 1.0f, textures.back});
            m_vertice.push_back({x - 0.0f, y - 0.0f, z + 1.0f, 0.0f, 0.0f, textures.back});
            m_vertice.push_back({x + 1.0f, y - 0.0f, z + 1.0f, 1.0f, 0.0f, textures.back});
            m_vertice.push_back({x + 1.0f, y + 1.0f, z + 1.0f, 1.0f, 1.0f, textures.back});

            m_indices.insert(m_indices.end(), {i + 1u, i + 0u, i + 3u, i + 1u, i + 3u, i + 2u});

            i += 4u;
        }

        if (faces.right) {
            m_vertice.push_back({x + 1.0f, y + 1.0f, z - 0.0f, 0.0f, 1.0f, textures.right});
            m_vertice.push_back({x + 1.0f, y - 0.0f, z - 0.0f, 0.0f, 0.0f, textures.right});
            m_vertice.push_back({x + 1.0f, y - 0.0f, z + 1.0f, 1.0f, 0.0f, textures.right});
            m_vertice.push_back({x + 1.0f, y + 1.0f, z + 1.0f, 1.0f, 1.0f, textures.right});

            m_indices.insert(m_indices.end(), {i + 0u, i + 1u, i + 3u, i + 3u, i + 1u, i + 2u});

            i += 4u;
        }

        if (faces.left) {
            m_vertice.push_back({x - 0.0f, y + 1.0f, z - 0.0f, 0.0f, 1.0f, textures.left});
            m_vertice.push_back({x - 0.0f, y - 0.0f, z - 0.0f, 0.0f, 0.0f, textures.left});
            m_vertice.push_back({x - 0.0f, y - 0.0f, z + 1.0f, 1.0f, 0.0f, textures.left});
            m_vertice.push_back({x - 0.0f, y + 1.0f, z + 1.0f, 1.0f, 1.0f, textures.left});

            m_indices.insert(m_indices.end(), {i + 1u, i + 0u, i + 3u, i + 1u, i + 3u, i + 2u});

            i += 4u;
        }

        if (faces.up) {
            m_vertice.push_back({x - 0.0f, y + 1.0f, z + 1.0f, 0.0f, 1.0f, textures.up});
            m_vertice.push_back({x - 0.0f, y + 1.0f, z - 0.0f, 0.0f, 0.0f, textures.up});
            m_vertice.push_back({x + 1.0f, y + 1.0f, z - 0.0f, 1.0f, 0.0f, textures.up});
            m_vertice.push_back({x + 1.0f, y + 1.0f, z + 1.0f, 1.0f, 1.0f, textures.up});

            m_indices.insert(m_indices.end(), {i + 0u, i + 1u, i + 3u, i + 3u, i + 1u, i + 2u});

            i += 4u;
        }

        if (faces.down) {
            m_vertice.push_back({x - 0.0f, y - 0.0f, z + 1.0f, 0.0f, 1.0f, textures.down});
            m_vertice.push_back({x - 0.0f, y - 0.0f, z - 0.0f, 0.0f, 0.0f, textures.down});
            m_vertice.push_back({x + 1.0f, y - 0.0f, z - 0.0f, 1.0f, 0.0f, textures.down});
            m_vertice.push_back({x + 1.0f, y - 0.0f, z + 1.0f, 1.0f, 1.0f, textures.down});

            m_indices.insert(m_indices.end(), {i + 1u, i + 0u, i + 3u, i + 1u, i + 3u, i + 2u});

            i += 4u;
        }
    }
};

}

#endif
