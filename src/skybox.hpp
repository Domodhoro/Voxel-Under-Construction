#ifndef SKYBOX_HPP
#define SKYBOX_HPP

namespace skybox {

struct skybox {
    skybox() {
        mesh      ();
        mesh_setup();
    }

    ~skybox() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers     (1, &m_VBO);
        glDeleteBuffers     (1, &m_EBO);
    }

    void draw(const shader::shader_program &shader, const unsigned int &texture, camera::camera &cam) const {
        glm::mat4 model {1.0f};

        glDepthMask(false);

        shader.use     ();
        shader.set_mat4("Model", model);
        shader.set_mat4("View", static_cast<glm::mat4>(static_cast<glm::mat3>(cam.get_view_matrix())));
        shader.set_mat4("Projection", cam.get_projection_matrix());

        glBindTexture    (GL_TEXTURE_CUBE_MAP, texture);
        glBindVertexArray(m_VAO);
        glDrawElements   (GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, (void*)(0));
        glBindVertexArray(0);

        glDepthMask(true);
    }

private:
    unsigned int m_VAO {0u};
    unsigned int m_VBO {0u};
    unsigned int m_EBO {0u};

    std::vector<tools::vertex_3d> m_vertice;
    std::vector<unsigned int> m_indices;

    void mesh_setup() {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers     (1, &m_VBO);
        glGenBuffers     (1, &m_EBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_vertice.size() * 5 * sizeof(float), &m_vertice.at(0), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices.at(0), GL_STATIC_DRAW);

        glVertexAttribPointer    (0, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)(0 * sizeof(float)));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer    (1, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        if (m_VAO == 0u) my_exception {__FILE__, __LINE__, "falha ao criar VAO do 'skybox'"};

        glBindVertexArray(0);
    }

    void mesh() {
        m_vertice.push_back({-0.5f, 0.5f,-0.5f, 0.0f, 0.0f});
        m_vertice.push_back({-0.5f,-0.5f,-0.5f, 0.0f, 1.0f});
        m_vertice.push_back({ 0.5f,-0.5f,-0.5f, 1.0f, 1.0f});
        m_vertice.push_back({ 0.5f, 0.5f,-0.5f, 1.0f, 0.0f});
        m_vertice.push_back({-0.5f, 0.5f, 0.5f, 0.0f, 0.0f});
        m_vertice.push_back({-0.5f,-0.5f, 0.5f, 0.0f, 1.0f});
        m_vertice.push_back({ 0.5f,-0.5f, 0.5f, 1.0f, 1.0f});
        m_vertice.push_back({ 0.5f, 0.5f, 0.5f, 1.0f, 0.0f});
        m_vertice.push_back({ 0.5f, 0.5f,-0.5f, 0.0f, 0.0f});
        m_vertice.push_back({ 0.5f,-0.5f,-0.5f, 0.0f, 1.0f});
        m_vertice.push_back({ 0.5f,-0.5f, 0.5f, 1.0f, 1.0f});
        m_vertice.push_back({ 0.5f, 0.5f, 0.5f, 1.0f, 0.0f});
        m_vertice.push_back({-0.5f, 0.5f,-0.5f, 0.0f, 0.0f});
        m_vertice.push_back({-0.5f,-0.5f,-0.5f, 0.0f, 1.0f});
        m_vertice.push_back({-0.5f,-0.5f, 0.5f, 1.0f, 1.0f});
        m_vertice.push_back({-0.5f, 0.5f, 0.5f, 1.0f, 0.0f});
        m_vertice.push_back({-0.5f, 0.5f, 0.5f, 0.0f, 0.0f});
        m_vertice.push_back({-0.5f, 0.5f,-0.5f, 0.0f, 1.0f});
        m_vertice.push_back({ 0.5f, 0.5f,-0.5f, 1.0f, 1.0f});
        m_vertice.push_back({ 0.5f, 0.5f, 0.5f, 1.0f, 0.0f});
        m_vertice.push_back({-0.5f,-0.5f, 0.5f, 0.0f, 0.0f});
        m_vertice.push_back({-0.5f,-0.5f,-0.5f, 0.0f, 1.0f});
        m_vertice.push_back({ 0.5f,-0.5f,-0.5f, 1.0f, 1.0f});
        m_vertice.push_back({ 0.5f,-0.5f, 0.5f, 1.0f, 0.0f});

        const std::initializer_list<unsigned int> indices {
             0u,  1u,  3u,  3u,  1u,  2u,
             5u,  4u,  7u,  5u,  7u,  6u,
             8u,  9u, 11u, 11u,  9u, 10u,
            13u, 12u, 15u, 13u, 15u, 14u,
            16u, 17u, 19u, 19u, 17u, 18u,
            21u, 20u, 23u, 21u, 23u, 22u
        };

        m_indices.insert(m_indices.end(), indices);
    }
};

}

#endif
