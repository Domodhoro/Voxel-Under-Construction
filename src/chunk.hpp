#ifndef CHUNK_HPP
#define CHUNK_HPP

namespace chunk {

struct chunk {
    chunk(const int X, const int Y, const int Z, terrain_generator::terrain_generator &terrain) : Position {X, Y, Z} {
        terrain.use(block, X, Y, Z);

        // test...................

        block.at(0 + 90 * CHUNK_SIZE_X + 0 * CHUNK_SIZE_X * CHUNK_SIZE_Y) = BLOCK_TYPE::STONE;
        block.at(0 + 92 * CHUNK_SIZE_X + 0 * CHUNK_SIZE_X * CHUNK_SIZE_Y) = BLOCK_TYPE::GRASS;

        // test...................

        auto i {0u};

        for (auto z = 0; z != CHUNK_SIZE_Z; ++z) for (auto y = 0; y != CHUNK_SIZE_Y; ++y) for (auto x = 0; x != CHUNK_SIZE_X; ++x) {
            if (get_block_type(x, y, z) == BLOCK_TYPE::AIR) continue;

            face<bool> faces {true, true, true, true, true, true};

            if (x > 0                  && get_block_type(x - 1, y, z) != BLOCK_TYPE::AIR) faces.L = false;
            if (y > 0                  && get_block_type(x, y - 1, z) != BLOCK_TYPE::AIR) faces.D = false;
            if (z > 0                  && get_block_type(x, y, z - 1) != BLOCK_TYPE::AIR) faces.F = false;
            if (x < (CHUNK_SIZE_X - 1) && get_block_type(x + 1, y, z) != BLOCK_TYPE::AIR) faces.R = false;
            if (y < (CHUNK_SIZE_Y - 1) && get_block_type(x, y + 1, z) != BLOCK_TYPE::AIR) faces.U = false;
            if (z < (CHUNK_SIZE_Z - 1) && get_block_type(x, y, z + 1) != BLOCK_TYPE::AIR) faces.B = false;

            mesh(i, x + X, y + Y, z + Z, faces, static_cast<int>(get_block_type(x, y, z)));
        }

        mesh_setup();
    }

    ~chunk() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers     (1, &VBO);
        glDeleteBuffers     (1, &EBO);
    }

    BLOCK_TYPE get_block_type(int x, int y, int z) const { return block.at(x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y); }

    void draw(shader::shader_program &shader, const unsigned int &texture, camera::camera &camera) const {
        glCullFace(GL_FRONT);

        shader.use     ();
        shader.set_mat4("View", camera.get_view_matrix());
        shader.set_mat4("Projection", camera.get_projection_matrix());

        glBindTexture    (GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements   (GL_TRIANGLES, indice.size(), GL_UNSIGNED_INT, (void*)(0));
        glBindVertexArray(0);

        glCullFace(GL_BACK);
    }

protected:
    glm::tvec3<int> Position {0};

    unsigned int VAO {0u};
    unsigned int VBO {0u};
    unsigned int EBO {0u};

    std::vector<BLOCK_TYPE>         block;
    std::vector<vertex_3d_t<float>> vertice;
    std::vector<unsigned int>       indice;

    void mesh_setup() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers     (1, &VBO);
        glGenBuffers     (1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertice.size() * sizeof(vertex_3d_t<float>), &vertice.at(0), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indice.size() * sizeof(unsigned int), &indice.at(0), GL_STATIC_DRAW);

        glVertexAttribPointer    (0, 3, GL_FLOAT, false, sizeof(vertex_3d_t<float>), (void*)(0 * sizeof(float)));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer    (1, 3, GL_FLOAT, false, sizeof(vertex_3d_t<float>), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        if (VAO == 0u) my_exception {__FILE__, __LINE__, "falha ao criar VAO do 'chunk'"};

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

        if (faces.F) {
            vertice.push_back({x - 0.0f, y + 1.0f, z - 0.0f, 0.0f, 1.0f, textures.F});
            vertice.push_back({x - 0.0f, y - 0.0f, z - 0.0f, 0.0f, 0.0f, textures.F});
            vertice.push_back({x + 1.0f, y - 0.0f, z - 0.0f, 1.0f, 0.0f, textures.F});
            vertice.push_back({x + 1.0f, y + 1.0f, z - 0.0f, 1.0f, 1.0f, textures.F});

            indice.insert(indice.end(), {i + 0u, i + 1u, i + 3u, i + 3u, i + 1u, i + 2u});

            i += 4u;
        }

        if (faces.B) {
            vertice.push_back({x - 0.0f, y + 1.0f, z + 1.0f, 0.0f, 1.0f, textures.B});
            vertice.push_back({x - 0.0f, y - 0.0f, z + 1.0f, 0.0f, 0.0f, textures.B});
            vertice.push_back({x + 1.0f, y - 0.0f, z + 1.0f, 1.0f, 0.0f, textures.B});
            vertice.push_back({x + 1.0f, y + 1.0f, z + 1.0f, 1.0f, 1.0f, textures.B});

            indice.insert(indice.end(), {i + 1u, i + 0u, i + 3u, i + 1u, i + 3u, i + 2u});

            i += 4u;
        }

        if (faces.R) {
            vertice.push_back({x + 1.0f, y + 1.0f, z - 0.0f, 0.0f, 1.0f, textures.R});
            vertice.push_back({x + 1.0f, y - 0.0f, z - 0.0f, 0.0f, 0.0f, textures.R});
            vertice.push_back({x + 1.0f, y - 0.0f, z + 1.0f, 1.0f, 0.0f, textures.R});
            vertice.push_back({x + 1.0f, y + 1.0f, z + 1.0f, 1.0f, 1.0f, textures.R});

            indice.insert(indice.end(), {i + 0u, i + 1u, i + 3u, i + 3u, i + 1u, i + 2u});

            i += 4u;
        }

        if (faces.L) {
            vertice.push_back({x - 0.0f, y + 1.0f, z - 0.0f, 0.0f, 1.0f, textures.L});
            vertice.push_back({x - 0.0f, y - 0.0f, z - 0.0f, 0.0f, 0.0f, textures.L});
            vertice.push_back({x - 0.0f, y - 0.0f, z + 1.0f, 1.0f, 0.0f, textures.L});
            vertice.push_back({x - 0.0f, y + 1.0f, z + 1.0f, 1.0f, 1.0f, textures.L});

            indice.insert(indice.end(), {i + 1u, i + 0u, i + 3u, i + 1u, i + 3u, i + 2u});

            i += 4u;
        }

        if (faces.U) {
            vertice.push_back({x - 0.0f, y + 1.0f, z + 1.0f, 0.0f, 1.0f, textures.U});
            vertice.push_back({x - 0.0f, y + 1.0f, z - 0.0f, 0.0f, 0.0f, textures.U});
            vertice.push_back({x + 1.0f, y + 1.0f, z - 0.0f, 1.0f, 0.0f, textures.U});
            vertice.push_back({x + 1.0f, y + 1.0f, z + 1.0f, 1.0f, 1.0f, textures.U});

            indice.insert(indice.end(), {i + 0u, i + 1u, i + 3u, i + 3u, i + 1u, i + 2u});

            i += 4u;
        }

        if (faces.D) {
            vertice.push_back({x - 0.0f, y - 0.0f, z + 1.0f, 0.0f, 1.0f, textures.D});
            vertice.push_back({x - 0.0f, y - 0.0f, z - 0.0f, 0.0f, 0.0f, textures.D});
            vertice.push_back({x + 1.0f, y - 0.0f, z - 0.0f, 1.0f, 0.0f, textures.D});
            vertice.push_back({x + 1.0f, y - 0.0f, z + 1.0f, 1.0f, 1.0f, textures.D});

            indice.insert(indice.end(), {i + 1u, i + 0u, i + 3u, i + 1u, i + 3u, i + 2u});

            i += 4u;
        }
    }
};

}

#endif
