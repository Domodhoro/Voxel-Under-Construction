#ifndef SKYBOX_HPP
#define SKYBOX_HPP

namespace skybox {

struct skybox {
    skybox() {
        mesh      ();
        mesh_setup();
    }

    ~skybox() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers     (1, &VBO);
        glDeleteBuffers     (1, &EBO);
    }

    void draw(shader::shader_program &shader, const unsigned int &texture, camera::camera &cam) const {
        glDepthMask(false);

        shader.use     ();
        shader.set_mat4("View", static_cast<glm::mat4>(static_cast<glm::mat3>(cam.get_view_matrix())));
        shader.set_mat4("Projection", cam.get_projection_matrix());

        glBindTexture    (GL_TEXTURE_CUBE_MAP, texture);
        glBindVertexArray(VAO);
        glDrawElements   (GL_TRIANGLES, indice.size(), GL_UNSIGNED_INT, (void*)(0));
        glBindVertexArray(0);

        glDepthMask(true);
    }

protected:
    unsigned int VAO {0u};
    unsigned int VBO {0u};
    unsigned int EBO {0u};

    std::vector<vertex_3d<float>> vertice;
    std::vector<unsigned int>     indice;

    void mesh_setup() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers     (1, &VBO);
        glGenBuffers     (1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertice.size() * sizeof(vertex_3d<float>), &vertice.at(0), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indice.size() * sizeof(unsigned int), &indice.at(0), GL_STATIC_DRAW);

        glVertexAttribPointer    (0, 3, GL_FLOAT, false, sizeof(vertex_3d<float>), (void*)(0 * sizeof(float)));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer    (1, 2, GL_FLOAT, false, sizeof(vertex_3d<float>), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        if (VAO == 0u) error_log(__FILE__, __LINE__, "falha ao criar VAO do 'skybox'");

        glBindVertexArray(0);
    }

    void mesh() {
        vertice.push_back({{-0.5f, 0.5f,-0.5f}, {0.0f, 0.0f}});
        vertice.push_back({{-0.5f,-0.5f,-0.5f}, {0.0f, 1.0f}});
        vertice.push_back({{ 0.5f,-0.5f,-0.5f}, {1.0f, 1.0f}});
        vertice.push_back({{ 0.5f, 0.5f,-0.5f}, {1.0f, 0.0f}});
        vertice.push_back({{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f}});
        vertice.push_back({{-0.5f,-0.5f, 0.5f}, {0.0f, 1.0f}});
        vertice.push_back({{ 0.5f,-0.5f, 0.5f}, {1.0f, 1.0f}});
        vertice.push_back({{ 0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}});
        vertice.push_back({{ 0.5f, 0.5f,-0.5f}, {0.0f, 0.0f}});
        vertice.push_back({{ 0.5f,-0.5f,-0.5f}, {0.0f, 1.0f}});
        vertice.push_back({{ 0.5f,-0.5f, 0.5f}, {1.0f, 1.0f}});
        vertice.push_back({{ 0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}});
        vertice.push_back({{-0.5f, 0.5f,-0.5f}, {0.0f, 0.0f}});
        vertice.push_back({{-0.5f,-0.5f,-0.5f}, {0.0f, 1.0f}});
        vertice.push_back({{-0.5f,-0.5f, 0.5f}, {1.0f, 1.0f}});
        vertice.push_back({{-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}});
        vertice.push_back({{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f}});
        vertice.push_back({{-0.5f, 0.5f,-0.5f}, {0.0f, 1.0f}});
        vertice.push_back({{ 0.5f, 0.5f,-0.5f}, {1.0f, 1.0f}});
        vertice.push_back({{ 0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}});
        vertice.push_back({{-0.5f,-0.5f, 0.5f}, {0.0f, 0.0f}});
        vertice.push_back({{-0.5f,-0.5f,-0.5f}, {0.0f, 1.0f}});
        vertice.push_back({{ 0.5f,-0.5f,-0.5f}, {1.0f, 1.0f}});
        vertice.push_back({{ 0.5f,-0.5f, 0.5f}, {1.0f, 0.0f}});

        const std::initializer_list<unsigned int> indice {
             0u,  1u,  3u,  3u,  1u,  2u,
             5u,  4u,  7u,  5u,  7u,  6u,
             8u,  9u, 11u, 11u,  9u, 10u,
            13u, 12u, 15u, 13u, 15u, 14u,
            16u, 17u, 19u, 19u, 17u, 18u,
            21u, 20u, 23u, 21u, 23u, 22u
        };

        this->indice.insert(this->indice.end(), indice);
    }
};

}

#endif
