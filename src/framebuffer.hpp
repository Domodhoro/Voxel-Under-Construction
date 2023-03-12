#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

namespace framebuffer {

struct framebuffer {
    framebuffer(const int width, const int height, const FRAMEBUFFER_TYPE type) : width {width}, height {height}, type {type} {
        mesh             ();
        mesh_setup       ();
        framebuffer_setup();
    }

    ~framebuffer() {
        glDeleteFramebuffers(1, &FBO);
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers     (1, &VBO);
        glDeleteBuffers     (1, &EBO);
        glDeleteTextures    (1, &texture_color_buffer);
    }

    void clear_color(const float r, const float g, const float b) const {
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
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
        shader.set_int("Type", static_cast<int>(type));

        glBindVertexArray(VAO);
        glBindTexture    (GL_TEXTURE_2D, texture_color_buffer);
        glDrawElements   (GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)(0));
        glBindVertexArray(0);
    }

protected:
    int width  {800};
    int height {600};

    unsigned int FBO                  {0u};
    unsigned int VAO                  {0u};
    unsigned int VBO                  {0u};
    unsigned int EBO                  {0u};
    unsigned int texture_color_buffer {0u};

    std::vector<vertex_2d<float>> vertice;
    std::vector<unsigned int>     indices;

    FRAMEBUFFER_TYPE type {FRAMEBUFFER_TYPE::DEFAULT};

    void framebuffer_setup() {
        auto RBO {0u};

        glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        stb_image_wrapper::gen_framebuffer_texture(width, height, texture_color_buffer);

        glGenRenderbuffers       (1, &RBO);
        glBindRenderbuffer       (GL_RENDERBUFFER, RBO);
        glRenderbufferStorage    (GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) error_log(__FILE__, __LINE__, "falha ao criar 'framebuffer'");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void mesh_setup() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers     (1, &VBO);
        glGenBuffers     (1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertice.size() * 4 * sizeof(float), &vertice.at(0), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices.at(0), GL_STATIC_DRAW);

        glVertexAttribPointer    (0, 2, GL_FLOAT, false, 4 * sizeof(float), (void*)(0 * sizeof(float)));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer    (1, 2, GL_FLOAT, false, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        if (VAO == 0u) error_log(__FILE__, __LINE__, "falha ao criar VAO do 'framebuffer'");

        glBindVertexArray(0);
    }

    void mesh() {
        vertice.push_back({{-1.0f,-1.0f}, {0.0f, 0.0f}});
        vertice.push_back({{ 1.0f,-1.0f}, {1.0f, 0.0f}});
        vertice.push_back({{ 1.0f, 1.0f}, {1.0f, 1.0f}});
        vertice.push_back({{-1.0f, 1.0f}, {0.0f, 1.0f}});

        indices.insert(indices.end(), {0u,  1u,  3u,  3u,  1u,  2u});
    }
};

}

#endif
