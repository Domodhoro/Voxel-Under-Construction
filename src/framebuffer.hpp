#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

namespace framebuffer {

struct framebuffer {
    framebuffer(const int width, const int height, const FRAMEBUFFER_TYPE type) : m_width {width}, m_height {height}, m_type {type} {
        mesh             ();
        mesh_setup       ();
        framebuffer_setup();
    }

    ~framebuffer() {
        glDeleteFramebuffers(1, &m_FBO);
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers     (1, &m_VBO);
        glDeleteBuffers     (1, &m_EBO);
        glDeleteTextures    (1, &m_texture_color_buffer);
    }

    void clear_color(const float r, const float g, const float b) const {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
        glEnable         (GL_DEPTH_TEST);
        glClearColor     (r, g, b, 1.0f);
        glClear          (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void draw(shader::shader_program &shader) const {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable        (GL_DEPTH_TEST);
        glClearColor     (1.0f, 1.0f, 1.0f, 1.0f);
        glClear          (GL_COLOR_BUFFER_BIT);

        shader.use    ();
        shader.set_int("Type", static_cast<int>(m_type));

        glBindVertexArray(m_VAO);
        glBindTexture    (GL_TEXTURE_2D, m_texture_color_buffer);
        glDrawElements   (GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, (void*)(0));
        glBindVertexArray(0);
    }

protected:
    int m_width  {800};
    int m_height {600};

    unsigned int m_FBO                  {0u};
    unsigned int m_VAO                  {0u};
    unsigned int m_VBO                  {0u};
    unsigned int m_EBO                  {0u};
    unsigned int m_texture_color_buffer {0u};

    std::vector<vertex_2d<float>> m_vertice;
    std::vector<unsigned int>     m_indices;

    FRAMEBUFFER_TYPE m_type {FRAMEBUFFER_TYPE::DEFAULT};

    void framebuffer_setup() {
        auto RBO {0u};

        glGenFramebuffers(1, &m_FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

        stb_image_wrapper::gen_framebuffer_texture(m_width, m_height, m_texture_color_buffer);

        glGenRenderbuffers       (1, &RBO);
        glBindRenderbuffer       (GL_RENDERBUFFER, RBO);
        glRenderbufferStorage    (GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) my_exception {__FILE__, __LINE__, "falha ao criar 'framebuffer'"};

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void mesh_setup() {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers     (1, &m_VBO);
        glGenBuffers     (1, &m_EBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_vertice.size() * 4 * sizeof(float), &m_vertice.at(0), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices.at(0), GL_STATIC_DRAW);

        glVertexAttribPointer    (0, 2, GL_FLOAT, false, 4 * sizeof(float), (void*)(0 * sizeof(float)));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer    (1, 2, GL_FLOAT, false, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        if (m_VAO == 0u) my_exception {__FILE__, __LINE__, "falha ao criar VAO do 'framebuffer'"};

        glBindVertexArray(0);
    }

    void mesh() {
        m_vertice.push_back({-1.0f,-1.0f, 0.0f, 0.0f});
        m_vertice.push_back({ 1.0f,-1.0f, 1.0f, 0.0f});
        m_vertice.push_back({ 1.0f, 1.0f, 1.0f, 1.0f});
        m_vertice.push_back({-1.0f, 1.0f, 0.0f, 1.0f});

        m_indices.insert(m_indices.end(), {0u,  1u,  3u,  3u,  1u,  2u});
    }
};

}

#endif
