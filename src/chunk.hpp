#ifndef CHUNK_HPP
#define CHUNK_HPP

namespace chunk {

constexpr auto CHUNK_SIZE_X {16};
constexpr auto CHUNK_SIZE_Y {64};
constexpr auto CHUNK_SIZE_Z {CHUNK_SIZE_X};
constexpr auto BASE         {32};
constexpr auto AMPLITUDE    {8.0f};

enum struct BLOCK_TYPE : int {
    AIR = 0,
    GRASS,
    DIRT,
    STONE,
    SAND
};

struct vertex {
    float X {0.0f};
    float Y {0.0f};
    float Z {0.0f};
    float U {0.0f};
    float V {0.0f};
    float T {0.0f};
};

struct face {
    bool F {true};
    bool B {true};
    bool R {true};
    bool L {true};
    bool U {true};
    bool D {true};
};

void mesh(std::vector<vertex> &vertice, unsigned int &count, int x, int y, int z, face &face, int block_type);

struct chunk {
    chunk(int X, int Z, unsigned int &texture, FastNoiseLite &noise) : m_texture {texture} {
        for (auto x = 0; x != CHUNK_SIZE_X; ++x) for (auto y = 0; y != CHUNK_SIZE_Y; ++y) for (auto z = 0; z != CHUNK_SIZE_Z; ++z) {
            auto MAX {
                BASE + std::abs(std::floor(AMPLITUDE * noise.GetNoise(static_cast<float>(x + Z), static_cast<float>(z + X))))
            };

            if      (y <= 16)          m_block.push_back(BLOCK_TYPE::STONE);
            else if (y > 0 && y < MAX) m_block.push_back(BLOCK_TYPE::DIRT);
            else if (y == MAX)         m_block.push_back(BLOCK_TYPE::GRASS);
            else                       m_block.push_back(BLOCK_TYPE::AIR);
        }

        for (auto x = 0; x != CHUNK_SIZE_X; ++x) for (auto y = 0; y != CHUNK_SIZE_Y; ++y) for (auto z = 0; z != CHUNK_SIZE_Z; ++z) {
            if (m_block.at(x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y) == BLOCK_TYPE::AIR)  continue;

            face face;

            if (x > 0                  && m_block.at((x - 1)  +  y        * CHUNK_SIZE_X +  z      * CHUNK_SIZE_X * CHUNK_SIZE_Y) != BLOCK_TYPE::AIR) face.L = false;
            if (y > 0                  && m_block.at( x       + (y - 1)   * CHUNK_SIZE_X +  z      * CHUNK_SIZE_X * CHUNK_SIZE_Y) != BLOCK_TYPE::AIR) face.D = false;
            if (z > 0                  && m_block.at( x       +  y        * CHUNK_SIZE_X + (z - 1) * CHUNK_SIZE_X * CHUNK_SIZE_Y) != BLOCK_TYPE::AIR) face.F = false;
            if (x < (CHUNK_SIZE_X - 1) && m_block.at( (x + 1) +  y        * CHUNK_SIZE_X +  z      * CHUNK_SIZE_X * CHUNK_SIZE_Y) != BLOCK_TYPE::AIR) face.R = false;
            if (y < (CHUNK_SIZE_Y - 1) && m_block.at( x       + (y + 1)   * CHUNK_SIZE_X +  z      * CHUNK_SIZE_X * CHUNK_SIZE_Y) != BLOCK_TYPE::AIR) face.U = false;
            if (z < (CHUNK_SIZE_Z - 1) && m_block.at( x       +  y        * CHUNK_SIZE_X + (z + 1) * CHUNK_SIZE_X * CHUNK_SIZE_Y) != BLOCK_TYPE::AIR) face.B = false;

            mesh(m_vertice, m_count, x + X, y, z + Z, face, static_cast<int>(m_block.at(x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y)));
        }

        setup();
    }

    ~chunk() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers     (1, &m_VBO);
    }

    void draw(shader::shader_program &shader, camera::camera &camera) {
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

    std::vector<vertex>     m_vertice;
    std::vector<BLOCK_TYPE> m_block;

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

            if (m_VAO == 0u) throw program_exception {"Falha ao criar VAO."};
        } catch (program_exception &e) { printf("%s", e.get_description()); }
    }
};

void mesh(std::vector<vertex> &vertice, unsigned int &count, int x, int y, int z, face &face, int block_type) {
    const auto X {static_cast<float>(x)}, Y {static_cast<float>(y)}, Z {static_cast<float>(z)};

    struct Textures {
        float F {0.0f};
        float B {0.0f};
        float R {0.0f};
        float L {0.0f};
        float U {0.0f};
        float D {0.0f};
    };

    Textures textures;

    switch (block_type) {
    case static_cast<int>(BLOCK_TYPE::GRASS):
        textures.F = 2.0f;
        textures.B = 2.0f;
        textures.R = 2.0f;
        textures.L = 2.0f;
        textures.U = 1.0f;
        textures.D = 3.0f;
        break;
    case static_cast<int>(BLOCK_TYPE::DIRT):
        textures.F = 3.0f;
        textures.B = 3.0f;
        textures.R = 3.0f;
        textures.L = 3.0f;
        textures.U = 3.0f;
        textures.D = 3.0f;
        break;
    case static_cast<int>(BLOCK_TYPE::STONE):
        textures.F = 4.0f;
        textures.B = 4.0f;
        textures.R = 4.0f;
        textures.L = 4.0f;
        textures.U = 4.0f;
        textures.D = 4.0f;
        break;
    case static_cast<int>(BLOCK_TYPE::SAND):
        textures.F = 5.0f;
        textures.B = 5.0f;
        textures.R = 5.0f;
        textures.L = 5.0f;
        textures.U = 5.0f;
        textures.D = 5.0f;
        break;
    default:
        textures.F = 0.0f;
        textures.B = 0.0f;
        textures.R = 0.0f;
        textures.L = 0.0f;
        textures.U = 0.0f;
        textures.D = 0.0f;
    }

    if (face.F) {
        vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, textures.F});
        vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, textures.F});
        vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, textures.F});

        vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, textures.F});
        vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, textures.F});
        vertice.push_back({X - 0.5f, Y + 0.5f, Z - 0.5f, 1.0f, 1.0f, textures.F});

        count += 6u;
    }

    if (face.B) {
        vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 0.0f, 0.0f, textures.B});
        vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, textures.B});
        vertice.push_back({X + 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 0.0f, textures.B});

        vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 0.0f, 1.0f, textures.B});
        vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, textures.B});
        vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 0.0f, 0.0f, textures.B});

        count += 6u;
    }

    if (face.R) {
        vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, textures.R});
        vertice.push_back({X + 0.5f, Y - 0.5f, Z + 0.5f, 0.0f, 0.0f, textures.R});
        vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 0.0f, 1.0f, textures.R});

        vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, textures.R});
        vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 0.0f, 1.0f, textures.R});
        vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 1.0f, 1.0f, textures.R});

        count += 6u;
    }

    if (face.L) {
        vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 0.0f, textures.L});
        vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, textures.L});
        vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, textures.L});

        vertice.push_back({X - 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, textures.L});
        vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, textures.L});
        vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, textures.L});

        count += 6u;
    }

    if (face.U) {
        vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, textures.U});
        vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, textures.U});
        vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 0.0f, textures.U});

        vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, textures.U});
        vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 0.0f, textures.U});
        vertice.push_back({X - 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 0.0f, textures.U});

        count += 6u;
    }

    if (face.D) {
        vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 1.0f, textures.D});
        vertice.push_back({X + 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 0.0f, textures.D});
        vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, textures.D});

        vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 1.0f, textures.D});
        vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 1.0f, textures.D});
        vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, textures.D});

        count += 6u;
    }
}

}

#endif
