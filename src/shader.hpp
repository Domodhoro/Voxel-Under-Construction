#ifndef SHADER_HPP
#define SHADER_HPP

namespace shader {

struct shader {
    shader(const char *vertex_path, const char *fragment_path) : m_shader {glCreateProgram()} {
        glAttachShader(m_shader, compile_shader_data(vertex_path, GL_VERTEX_SHADER));
        glAttachShader(m_shader, compile_shader_data(fragment_path, GL_FRAGMENT_SHADER));
        glLinkProgram (m_shader);

        auto success {0};

        glGetProgramiv(m_shader, GL_LINK_STATUS, &success);

        if (!success) printf("Falha ao compilar 'shader'.");
    }

    ~shader() { glDeleteProgram(m_shader); }

    void use      ()                                                 { glUseProgram      (m_shader); }
    void set_bool (const char *name, bool &value)                    { glUniform1i       (glGetUniformLocation(m_shader, name), static_cast<int>(value)); }
    void set_int  (const char *name, int &value)                     { glUniform1i       (glGetUniformLocation(m_shader, name), value); }
    void set_float(const char *name, float &value)                   { glUniform1f       (glGetUniformLocation(m_shader, name), value); }
    void set_vec2 (const char *name, const glm::tvec2<float> &value) { glUniform2fv      (glGetUniformLocation(m_shader, name), 1, glm::value_ptr(value)); }
    void set_vec3 (const char *name, const glm::tvec3<float> &value) { glUniform3fv      (glGetUniformLocation(m_shader, name), 1, glm::value_ptr(value)); }
    void set_mat4 (const char *name, const glm::mat4 &matrix)        { glUniformMatrix4fv(glGetUniformLocation(m_shader, name), 1, false, glm::value_ptr(matrix)); }

private:
    unsigned int m_shader {0u};

    unsigned int compile_shader_data(const char *data_path, GLenum type) {
        auto data_code   {read_file(data_path)};
        auto shader_code {data_code.c_str()};
        auto data        {glCreateShader(type)};

        glShaderSource (data, 1, &shader_code, nullptr);
        glCompileShader(data);

        auto success {0};

        glGetShaderiv(data, GL_COMPILE_STATUS, &success);

        if (success == 0u) {
            if (type == GL_VERTEX_SHADER)   printf("Falha ao compilar 'vertex'.");
            if (type == GL_FRAGMENT_SHADER) printf("Falha ao compilar 'fragment'.");
        }

        return data;
    }

    std::string read_file(const char *file_path) {
        std::ifstream file {file_path};

        if (!file.is_open()) printf("Falha ao abrir arquivo GLSL.");

        std::stringstream code;

        code << file.rdbuf();

        file.close();

        return code.str();
    }
};

}

#endif
