#ifndef SKYBOX_HPP
#define SKYBOX_HPP

namespace skybox {

struct skybox {
    skybox() {
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

        m_indices.push_back(0);
        m_indices.push_back(1);
        m_indices.push_back(3);

        m_indices.push_back(3);
        m_indices.push_back(1);
        m_indices.push_back(2);

        m_indices.push_back(5);
        m_indices.push_back(4);
        m_indices.push_back(7);

        m_indices.push_back(5);
        m_indices.push_back(7);
        m_indices.push_back(6);

        m_indices.push_back(8);
        m_indices.push_back(9);
        m_indices.push_back(11);

        m_indices.push_back(11);
        m_indices.push_back(9);
        m_indices.push_back(10);

        m_indices.push_back(13);
        m_indices.push_back(12);
        m_indices.push_back(15);

        m_indices.push_back(13);
        m_indices.push_back(15);
        m_indices.push_back(14);

        m_indices.push_back(16);
        m_indices.push_back(17);
        m_indices.push_back(19);

        m_indices.push_back(19);
        m_indices.push_back(17);
        m_indices.push_back(18);

        m_indices.push_back(21);
        m_indices.push_back(20);
        m_indices.push_back(23);

        m_indices.push_back(21);
        m_indices.push_back(23);
        m_indices.push_back(22);

        setup();
    }

    ~skybox() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers     (1, &m_VBO);
        glDeleteBuffers     (1, &m_EBO);
    }

    void draw(const shader::shader_program &shader, const unsigned int &texture, camera::camera &cam) const {
        glm::mat4 model {1.0f};

        model = glm::rotate(model, static_cast<float>(glm::radians(180.0f)), glm::tvec3<float>(1.0f, 0.0f, 0.0f));

        glCullFace(GL_BACK);
        glDepthMask(false);

        shader.use();

        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
        glBindVertexArray(m_VAO);

        shader.set_mat4("Model", model);
        shader.set_mat4("View", static_cast<glm::mat4>(static_cast<glm::mat3>(cam.get_view_matrix())));
        shader.set_mat4("Projection", cam.get_projection_matrix());

        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        glDepthMask(true);
        glCullFace(GL_FRONT);
    }

private:
    unsigned int m_VAO {0u};
    unsigned int m_VBO {0u};
    unsigned int m_EBO {0u};

    std::vector<tools::vertex_3d> m_vertice;
    std::vector<unsigned int> m_indices;

    void setup() {
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
    }
};

}

#endif
