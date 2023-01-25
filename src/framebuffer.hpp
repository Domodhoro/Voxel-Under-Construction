#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

namespace framebuffer {

enum struct FRAMEBUFFER_TYPE : int {
    DEFAULT = 0,
    INVERT_COLOR,
    GRAY_SCALE
};

struct framebuffer {
    framebuffer(const int width, const int height, const FRAMEBUFFER_TYPE type) : m_type {type} {
        mesh();
        mesh_setup();

        auto RBO {0u};

        glGenFramebuffers(1, &m_FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

        glGenTextures         (1, &m_texture_color_buffer);
        glBindTexture         (GL_TEXTURE_2D, m_texture_color_buffer);
        glTexImage2D          (GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri       (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri       (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture_color_buffer, 0);

        glGenRenderbuffers       (1, &RBO);
        glBindRenderbuffer       (GL_RENDERBUFFER, RBO);
        glRenderbufferStorage    (GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) my_exception {__FILE__, __LINE__, "falha ao criar 'framebuffer'"};

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    ~framebuffer() {
        glDeleteFramebuffers(1, &m_FBO);
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers     (1, &m_VBO);
        glDeleteTextures    (1, &m_texture_color_buffer);
    }

    void clear_color(const float r, const float g, const float b) const {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
        glEnable         (GL_DEPTH_TEST);
        glClearColor     (r, g, b, 1.0f);
        glClear          (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void draw(const shader::shader_program &shader) const {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable        (GL_DEPTH_TEST);
        glClearColor     (1.0f, 1.0f, 1.0f, 1.0f);
        glClear          (GL_COLOR_BUFFER_BIT);

        shader.use    ();
        shader.set_int("Type", static_cast<int>(m_type));

        glBindVertexArray(m_VAO);
        glBindTexture    (GL_TEXTURE_2D, m_texture_color_buffer);
        glDrawArrays     (GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

private:
    std::vector<tools::vertex_2d> m_vertice;

    FRAMEBUFFER_TYPE m_type {FRAMEBUFFER_TYPE::DEFAULT};

    unsigned int m_FBO                  {0u};
    unsigned int m_VAO                  {0u};
    unsigned int m_VBO                  {0u};
    unsigned int m_texture_color_buffer {0u};

    void mesh_setup() {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers     (1, &m_VBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_vertice.size() * 4 * sizeof(float), &m_vertice.at(0), GL_STATIC_DRAW);

        glVertexAttribPointer    (0, 2, GL_FLOAT, false, 4 * sizeof(float), (void*)(0 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer    (1, 2, GL_FLOAT, false, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        if (m_VAO == 0u) my_exception {__FILE__, __LINE__, "falha ao criar VAO do 'framebuffer'"};

        glBindVertexArray(0);
    }

    void mesh() {
        m_vertice.push_back({-1.0f, 1.0f, 0.0f, 1.0f});
        m_vertice.push_back({-1.0f,-1.0f, 0.0f, 0.0f});
        m_vertice.push_back({ 1.0f,-1.0f, 1.0f, 0.0f});
        m_vertice.push_back({-1.0f, 1.0f, 0.0f, 1.0f});
        m_vertice.push_back({ 1.0f,-1.0f, 1.0f, 0.0f});
        m_vertice.push_back({ 1.0f, 1.0f, 1.0f, 1.0f});
    }
};

}

#endif
