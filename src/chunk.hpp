#ifndef CHUNK_HPP
#define CHUNK_HPP

namespace chunk {

struct chunk {
    chunk(const int X, const int Y, const int Z, terrain_generator::terrain_generator &terrain) : Position {X, Y, Z} {
        terrain.use(block, X, Y, Z);

        auto i {0u};

        for (auto z = 0; z != CHUNK_SIZE_Z; ++z) for (auto y = 0; y != CHUNK_SIZE_Y; ++y) for (auto x = 0; x != CHUNK_SIZE_X; ++x) {
            if (get_block_at(x, y, z) == BLOCK::AIR) continue;

            block_face<bool> faces {true, true, true, true, true, true};

            if (x > 0                  && get_block_at(x - 1, y, z) != BLOCK::AIR) faces.Left  = false;
            if (y > 0                  && get_block_at(x, y - 1, z) != BLOCK::AIR) faces.Down  = false;
            if (z > 0                  && get_block_at(x, y, z - 1) != BLOCK::AIR) faces.Front = false;
            if (x < (CHUNK_SIZE_X - 1) && get_block_at(x + 1, y, z) != BLOCK::AIR) faces.Right = false;
            if (y < (CHUNK_SIZE_Y - 1) && get_block_at(x, y + 1, z) != BLOCK::AIR) faces.Up    = false;
            if (z < (CHUNK_SIZE_Z - 1) && get_block_at(x, y, z + 1) != BLOCK::AIR) faces.Back  = false;

            mesh(i, x + X, y + Y, z + Z, faces, static_cast<int>(get_block_at(x, y, z)));
        }

        mesh_setup();
    }

    ~chunk() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers     (1, &VBO);
        glDeleteBuffers     (1, &EBO);
    }

    BLOCK get_block_at(int x, int y, int z) const { return block.at(x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y); }

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

    std::vector<BLOCK>               block;
    std::vector<vertex_3d_id<float>> vertice;
    std::vector<unsigned int>        indice;

    void mesh_setup() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers     (1, &VBO);
        glGenBuffers     (1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertice.size() * sizeof(vertex_3d_id<float>), &vertice.at(0), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indice.size() * sizeof(unsigned int), &indice.at(0), GL_STATIC_DRAW);

        glVertexAttribPointer    (0, 3, GL_FLOAT, false, sizeof(vertex_3d_id<float>), (void*)(0 * sizeof(float)));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer    (1, 3, GL_FLOAT, false, sizeof(vertex_3d_id<float>), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        if (VAO == 0u) error_log(__FILE__, __LINE__, "falha ao criar VAO do 'chunk'");

        glBindVertexArray(0);
    }

    void mesh(unsigned int &i, const int x, const int y, const int z, const block_face<bool> &faces, const int BLOCK) {
        block_face<float> face {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

        switch (BLOCK) {
        case static_cast<int>(BLOCK::GRASS):
            face = {2.0f, 2.0f, 2.0f, 2.0f, 1.0f, 3.0f};
            break;
        case static_cast<int>(BLOCK::DIRT):
            face = {3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f};
            break;
        case static_cast<int>(BLOCK::STONE):
            face = {4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f};
            break;
        case static_cast<int>(BLOCK::SAND):
            face = {5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f};
            break;
        case static_cast<int>(BLOCK::MAGMA):
            face = {6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f};
            break;
        case static_cast<int>(BLOCK::FELDSPAR):
            face = {7.0f, 7.0f, 7.0f, 7.0f, 7.0f, 7.0f};
            break;
        }

        if (faces.Front) {
            vertice.push_back({{x - 0.0f, y + 1.0f, z - 0.0f}, {0.0f, 1.0f, face.Front}});
            vertice.push_back({{x - 0.0f, y - 0.0f, z - 0.0f}, {0.0f, 0.0f, face.Front}});
            vertice.push_back({{x + 1.0f, y - 0.0f, z - 0.0f}, {1.0f, 0.0f, face.Front}});
            vertice.push_back({{x + 1.0f, y + 1.0f, z - 0.0f}, {1.0f, 1.0f, face.Front}});

            indice.insert(indice.end(), {i + 0u, i + 1u, i + 3u, i + 3u, i + 1u, i + 2u});

            i += 4u;
        }

        if (faces.Back) {
            vertice.push_back({{x - 0.0f, y + 1.0f, z + 1.0f}, {0.0f, 1.0f, face.Back}});
            vertice.push_back({{x - 0.0f, y - 0.0f, z + 1.0f}, {0.0f, 0.0f, face.Back}});
            vertice.push_back({{x + 1.0f, y - 0.0f, z + 1.0f}, {1.0f, 0.0f, face.Back}});
            vertice.push_back({{x + 1.0f, y + 1.0f, z + 1.0f}, {1.0f, 1.0f, face.Back}});

            indice.insert(indice.end(), {i + 1u, i + 0u, i + 3u, i + 1u, i + 3u, i + 2u});

            i += 4u;
        }

        if (faces.Right) {
            vertice.push_back({{x + 1.0f, y + 1.0f, z - 0.0f}, {0.0f, 1.0f, face.Right}});
            vertice.push_back({{x + 1.0f, y - 0.0f, z - 0.0f}, {0.0f, 0.0f, face.Right}});
            vertice.push_back({{x + 1.0f, y - 0.0f, z + 1.0f}, {1.0f, 0.0f, face.Right}});
            vertice.push_back({{x + 1.0f, y + 1.0f, z + 1.0f}, {1.0f, 1.0f, face.Right}});

            indice.insert(indice.end(), {i + 0u, i + 1u, i + 3u, i + 3u, i + 1u, i + 2u});

            i += 4u;
        }

        if (faces.Left) {
            vertice.push_back({{x - 0.0f, y + 1.0f, z - 0.0f}, {0.0f, 1.0f, face.Left}});
            vertice.push_back({{x - 0.0f, y - 0.0f, z - 0.0f}, {0.0f, 0.0f, face.Left}});
            vertice.push_back({{x - 0.0f, y - 0.0f, z + 1.0f}, {1.0f, 0.0f, face.Left}});
            vertice.push_back({{x - 0.0f, y + 1.0f, z + 1.0f}, {1.0f, 1.0f, face.Left}});

            indice.insert(indice.end(), {i + 1u, i + 0u, i + 3u, i + 1u, i + 3u, i + 2u});

            i += 4u;
        }

        if (faces.Up) {
            vertice.push_back({{x - 0.0f, y + 1.0f, z + 1.0f}, {0.0f, 1.0f, face.Up}});
            vertice.push_back({{x - 0.0f, y + 1.0f, z - 0.0f}, {0.0f, 0.0f, face.Up}});
            vertice.push_back({{x + 1.0f, y + 1.0f, z - 0.0f}, {1.0f, 0.0f, face.Up}});
            vertice.push_back({{x + 1.0f, y + 1.0f, z + 1.0f}, {1.0f, 1.0f, face.Up}});

            indice.insert(indice.end(), {i + 0u, i + 1u, i + 3u, i + 3u, i + 1u, i + 2u});

            i += 4u;
        }

        if (faces.Down) {
            vertice.push_back({{x - 0.0f, y - 0.0f, z + 1.0f}, {0.0f, 1.0f, face.Down}});
            vertice.push_back({{x - 0.0f, y - 0.0f, z - 0.0f}, {0.0f, 0.0f, face.Down}});
            vertice.push_back({{x + 1.0f, y - 0.0f, z - 0.0f}, {1.0f, 0.0f, face.Down}});
            vertice.push_back({{x + 1.0f, y - 0.0f, z + 1.0f}, {1.0f, 1.0f, face.Down}});

            indice.insert(indice.end(), {i + 1u, i + 0u, i + 3u, i + 1u, i + 3u, i + 2u});

            i += 4u;
        }
    }
};

}

#endif
