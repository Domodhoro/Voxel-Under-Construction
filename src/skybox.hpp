#ifndef SKYBOX_HPP
#define SKYBOX_HPP

namespace skybox {

struct skybox {
    skybox() {
        m_vertice.push_back({-0.5f,-0.5f,-0.5f, 1.0f, 0.0f});
        m_vertice.push_back({ 0.5f,-0.5f,-0.5f, 0.0f, 0.0f});
        m_vertice.push_back({ 0.5f, 0.5f,-0.5f, 0.0f, 1.0f});
        m_vertice.push_back({-0.5f,-0.5f,-0.5f, 1.0f, 0.0f});
        m_vertice.push_back({ 0.5f, 0.5f,-0.5f, 0.0f, 1.0f});
        m_vertice.push_back({-0.5f, 0.5f,-0.5f, 1.0f, 1.0f});

        m_vertice.push_back({-0.5f,-0.5f, 0.5f, 0.0f, 0.0f});
        m_vertice.push_back({ 0.5f, 0.5f, 0.5f, 1.0f, 1.0f});
        m_vertice.push_back({ 0.5f,-0.5f, 0.5f, 1.0f, 0.0f});
        m_vertice.push_back({-0.5f, 0.5f, 0.5f, 0.0f, 1.0f});
        m_vertice.push_back({ 0.5f, 0.5f, 0.5f, 1.0f, 1.0f});
        m_vertice.push_back({-0.5f,-0.5f, 0.5f, 0.0f, 0.0f});

        m_vertice.push_back({ 0.5f,-0.5f,-0.5f, 1.0f, 0.0f});
        m_vertice.push_back({ 0.5f,-0.5f, 0.5f, 0.0f, 0.0f});
        m_vertice.push_back({ 0.5f, 0.5f, 0.5f, 0.0f, 1.0f});
        m_vertice.push_back({ 0.5f,-0.5f,-0.5f, 1.0f, 0.0f});
        m_vertice.push_back({ 0.5f, 0.5f, 0.5f, 0.0f, 1.0f});
        m_vertice.push_back({ 0.5f, 0.5f,-0.5f, 1.0f, 1.0f});

        m_vertice.push_back({-0.5f,-0.5f, 0.5f, 1.0f, 0.0f});
        m_vertice.push_back({-0.5f,-0.5f,-0.5f, 0.0f, 0.0f});
        m_vertice.push_back({-0.5f, 0.5f, 0.5f, 1.0f, 1.0f});
        m_vertice.push_back({-0.5f, 0.5f,-0.5f, 0.0f, 1.0f});
        m_vertice.push_back({-0.5f, 0.5f, 0.5f, 1.0f, 1.0f});
        m_vertice.push_back({-0.5f,-0.5f,-0.5f, 0.0f, 0.0f});

        m_vertice.push_back({ 0.5f, 0.5f,-0.5f, 0.0f, 1.0f});
        m_vertice.push_back({ 0.5f, 0.5f, 0.5f, 1.0f, 1.0f});
        m_vertice.push_back({-0.5f, 0.5f, 0.5f, 1.0f, 0.0f});
        m_vertice.push_back({ 0.5f, 0.5f,-0.5f, 0.0f, 1.0f});
        m_vertice.push_back({-0.5f, 0.5f, 0.5f, 1.0f, 0.0f});
        m_vertice.push_back({-0.5f, 0.5f,-0.5f, 0.0f, 0.0f});

        m_vertice.push_back({-0.5f,-0.5f, 0.5f, 1.0f, 1.0f});
        m_vertice.push_back({ 0.5f,-0.5f, 0.5f, 1.0f, 0.0f});
        m_vertice.push_back({ 0.5f,-0.5f,-0.5f, 0.0f, 0.0f});
        m_vertice.push_back({-0.5f,-0.5f,-0.5f, 0.0f, 1.0f});
        m_vertice.push_back({-0.5f,-0.5f, 0.5f, 1.0f, 1.0f});
        m_vertice.push_back({ 0.5f,-0.5f,-0.5f, 0.0f, 0.0f});

        setup();
    }

    ~skybox() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers     (1, &m_VBO);
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

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glDepthMask(true);
        glCullFace(GL_FRONT);
    }

private:
    unsigned int m_VAO {0u};
    unsigned int m_VBO {0u};

    std::vector<util::vertex_3d> m_vertice;

    void setup() {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers     (1, &m_VBO);
        glBindVertexArray(m_VAO);

        try {
            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            glBufferData(GL_ARRAY_BUFFER, m_vertice.size() * 5 * sizeof(float), &m_vertice.at(0), GL_STATIC_DRAW);

            glVertexAttribPointer    (0, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)(0 * sizeof(float)));
            glEnableVertexAttribArray(0);
            glVertexAttribPointer    (1, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            if (m_VAO == 0u) throw util::program_exception {"Falha ao criar VAO do 'skybox'."};
        } catch (util::program_exception &e) {
            printf("%s", e.get_description());
        }
    }
};

}

#endif
