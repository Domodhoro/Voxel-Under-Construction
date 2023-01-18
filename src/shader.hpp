#ifndef SHADER_HPP
#define SHADER_HPP

namespace shader {

struct shader_program {
    shader_program(const char *vertex_path, const char *fragment_path) : m_shader {glCreateProgram()} {
        try {
            glAttachShader(m_shader, compile_shader_data(vertex_path, GL_VERTEX_SHADER));
            glAttachShader(m_shader, compile_shader_data(fragment_path, GL_FRAGMENT_SHADER));
            glLinkProgram (m_shader);

            auto success {0};

            glGetProgramiv(m_shader, GL_LINK_STATUS, &success);

            if (!success) throw util::program_exception {"Falha ao compilar 'shader'."};
        } catch (util::program_exception &e) {
            printf("%s", e.get_description());
        }
    }

    ~shader_program() { glDeleteProgram(m_shader); }

    void use      ()                                                 const { glUseProgram      (m_shader); }
    void set_bool (const char *name, bool &value)                    const { glUniform1i       (glGetUniformLocation(m_shader, name), static_cast<int>(value)); }
    void set_int  (const char *name, int &value)                     const { glUniform1i       (glGetUniformLocation(m_shader, name), value); }
    void set_float(const char *name, float &value)                   const { glUniform1f       (glGetUniformLocation(m_shader, name), value); }
    void set_vec2 (const char *name, const glm::tvec2<float> &value) const { glUniform2fv      (glGetUniformLocation(m_shader, name), 1, glm::value_ptr(value)); }
    void set_vec3 (const char *name, const glm::tvec3<float> &value) const { glUniform3fv      (glGetUniformLocation(m_shader, name), 1, glm::value_ptr(value)); }
    void set_mat4 (const char *name, const glm::mat4 &matrix)        const { glUniformMatrix4fv(glGetUniformLocation(m_shader, name), 1, false, glm::value_ptr(matrix)); }

private:
    unsigned int m_shader {0u};

    unsigned int compile_shader_data(const char *data_path, const GLenum type) const {
        auto data_code   {read_file(data_path)};
        auto shader_code {data_code.c_str()};
        auto data        {glCreateShader(type)};

        try {
            glShaderSource (data, 1, &shader_code, nullptr);
            glCompileShader(data);

            auto success {0};

            glGetShaderiv(data, GL_COMPILE_STATUS, &success);

            if (success == 0u) {
                if (type == GL_VERTEX_SHADER)   throw util::program_exception {"Falha ao compilar 'vertex'."};
                if (type == GL_FRAGMENT_SHADER) throw util::program_exception {"Falha ao compilar 'fragment'."};
            }
        } catch (util::program_exception &e) {
            printf("%s", e.get_description());
        }

        return data;
    }

    std::string read_file(const char *file_path) const {
        std::ifstream file     {file_path};
        std::stringstream code {};

        try {
            if (!file.is_open()) throw util::program_exception {"Falha ao abrir arquivo GLSL."};

            code << file.rdbuf();

            file.close();
        } catch (util::program_exception &e) {
            printf("%s", e.get_description());
        }

        return code.str();
    }
};

}

#endif
