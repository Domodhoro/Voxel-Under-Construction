#ifndef CHUNK_HPP
#define CHUNK_HPP

namespace chunk {

static void mesh(std::vector<util::vertex_3d_t> &vertice, unsigned int &count, const int x, const int y, const int z, const util::face &face, const int block_type);

struct chunk {
    chunk(const int X, const int Z, const noise::noise &noise) {
        for (auto x = 0; x != settings::CHUNK_SIZE_X; ++x) for (auto y = 0; y != settings::CHUNK_SIZE_Y; ++y) for (auto z = 0; z != settings::CHUNK_SIZE_Z; ++z) {
            auto MAX {
                32 + abs(floor(8.0f * noise.get(static_cast<float>(x + Z), static_cast<float>(z + X))))
            };

            if      (y == 0)           m_block.push_back(util::BLOCK_TYPE::MAGMA);
            else if (y <= MAX - 2)     m_block.push_back(util::BLOCK_TYPE::STONE);
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

            mesh(m_vertice, m_count, x + X, y, z + Z, face, static_cast<int>(get_block_type(x, y, z)));
        }

        setup();
    }

    ~chunk() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers     (1, &m_VBO);
    }

    void draw(const shader::shader_program &shader, const unsigned int &texture, camera::camera &camera) const {
        glCullFace(GL_FRONT);

        shader.use();

        glBindTexture    (GL_TEXTURE_2D, texture);
        glBindVertexArray(m_VAO);

        shader.set_mat4("View", camera.get_view_matrix());
        shader.set_mat4("Projection", camera.get_projection_matrix());

        glDrawArrays(GL_TRIANGLES, 0, m_count);

        glCullFace(GL_BACK);
    }

private:
    unsigned int m_VAO   {0u};
    unsigned int m_VBO   {0u};
    unsigned int m_count {0u};

    std::vector<util::vertex_3d_t>     m_vertice;
    std::vector<util::BLOCK_TYPE> m_block;

    void setup() {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers     (1, &m_VBO);
        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_vertice.size() * 6 * sizeof(float), &m_vertice.at(0), GL_STATIC_DRAW);

        glVertexAttribPointer    (0, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(0 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer    (1, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        if (m_VAO == 0u) my_exception {__FILE__, __LINE__, "falha ao criar VAO do 'chunk'"};
    }

    util::BLOCK_TYPE get_block_type(int x, int y, int z) const {
        return m_block.at(x + y * settings::CHUNK_SIZE_X + z * settings::CHUNK_SIZE_X * settings::CHUNK_SIZE_Y);
    }
};

static void mesh(std::vector<util::vertex_3d_t> &vertice, unsigned int &count, const int x, const int y, const int z, const util::face &face, const int block_type) {
    const auto X {static_cast<float>(x)};
    const auto Y {static_cast<float>(y)};
    const auto Z {static_cast<float>(z)};

    util::face_texture tex;

    switch (block_type) {
    case static_cast<int>(util::BLOCK_TYPE::GRASS):
        tex = {2.0f, 2.0f, 2.0f, 2.0f, 1.0f, 3.0f};
        break;
    case static_cast<int>(util::BLOCK_TYPE::DIRT):
        tex = {3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f};
        break;
    case static_cast<int>(util::BLOCK_TYPE::STONE):
        tex = {4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f};
        break;
    case static_cast<int>(util::BLOCK_TYPE::SAND):
        tex = {5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f};
        break;
    case static_cast<int>(util::BLOCK_TYPE::MAGMA):
        tex = {6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f};
        break;
    }

    if (face.F) {
        vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, tex.F});
        vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, tex.F});
        vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, tex.F});
        vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, tex.F});
        vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, tex.F});
        vertice.push_back({X - 0.5f, Y + 0.5f, Z - 0.5f, 1.0f, 1.0f, tex.F});

        count += 6u;
    }

    if (face.B) {
        vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 0.0f, 0.0f, tex.B});
        vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, tex.B});
        vertice.push_back({X + 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 0.0f, tex.B});
        vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 0.0f, 1.0f, tex.B});
        vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, tex.B});
        vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 0.0f, 0.0f, tex.B});

        count += 6u;
    }

    if (face.R) {
        vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, tex.R});
        vertice.push_back({X + 0.5f, Y - 0.5f, Z + 0.5f, 0.0f, 0.0f, tex.R});
        vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 0.0f, 1.0f, tex.R});
        vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, tex.R});
        vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 0.0f, 1.0f, tex.R});
        vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 1.0f, 1.0f, tex.R});

        count += 6u;
    }

    if (face.L) {
        vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 0.0f, tex.L});
        vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, tex.L});
        vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, tex.L});
        vertice.push_back({X - 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, tex.L});
        vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, tex.L});
        vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, tex.L});

        count += 6u;
    }

    if (face.U) {
        vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, tex.U});
        vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, tex.U});
        vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 0.0f, tex.U});
        vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, tex.U});
        vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 0.0f, tex.U});
        vertice.push_back({X - 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 0.0f, tex.U});

        count += 6u;
    }

    if (face.D) {
        vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 1.0f, tex.D});
        vertice.push_back({X + 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 0.0f, tex.D});
        vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, tex.D});
        vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 1.0f, tex.D});
        vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 1.0f, tex.D});
        vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, tex.D});

        count += 6u;
    }
}

}

#endif
