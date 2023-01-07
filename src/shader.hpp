#ifndef SHADER_HPP
#define SHADER_HPP

namespace Shader {

class Shader {
public:
    Shader(const char *vertexPath, const char *fragmentPath) {
        auto vertex {
            compileShaderData(vertexPath, GL_VERTEX_SHADER)
        };

        auto fragment {
            compileShaderData(fragmentPath, GL_FRAGMENT_SHADER)
        };

        m_shader = glCreateProgram();

        glAttachShader(m_shader, vertex);
        glAttachShader(m_shader, fragment);
        glLinkProgram(m_shader);

        auto success {0};

        glGetProgramiv(m_shader, GL_LINK_STATUS, &success);

        if (!success) {
            char infoLog[512] {};

            glGetProgramInfoLog(m_shader, 512, nullptr, infoLog);

            throw std::string {"Falha ao compilar shader.\n" + std::string(infoLog)};
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    virtual ~Shader() {
        glDeleteProgram(m_shader);
    }

    void use() {
        glUseProgram(m_shader);;
    }

    void setBool(const char *name, bool value) {
        glUniform1i(glGetUniformLocation(m_shader, name), static_cast<int>(value));
    }

    void setInt(const char *name, int value) {
        glUniform1i(glGetUniformLocation(m_shader, name), value);
    }

    void setFloat(const char *name, float value) {
        glUniform1f(glGetUniformLocation(m_shader, name), value);
    }

    void setMatrix4fv(const char *name, const glm::mat4& matrix) {
        glUniformMatrix4fv(glGetUniformLocation(m_shader, name), 1, false, glm::value_ptr(matrix));
    }

    void setVec2(const char *name, const glm::tvec2<float>& value) {
        glUniform2fv(glGetUniformLocation(m_shader, name), 1, glm::value_ptr(value));
    }

    void setVec3(const char *name, const glm::tvec3<float>& value) {
        glUniform3fv(glGetUniformLocation(m_shader, name), 1, glm::value_ptr(value));
    }

protected:
    unsigned int m_shader {0u};

    unsigned int compileShaderData(const char *dataPath, const GLenum type) {
        std::string dataCode {read(dataPath).str()};

        auto dataCodeSource {dataCode.c_str()};
        auto data {glCreateShader(type)};

        glShaderSource(data, 1, &dataCodeSource, 0);
        glCompileShader(data);

        auto success {0};

        glGetShaderiv(data, GL_COMPILE_STATUS, &success);

        if (success == 0) {
            char infoLog[512] {};

            glGetShaderInfoLog(data, 512, nullptr, infoLog);

            throw std::string {"Falha ao compilar shader.\n" + std::string(infoLog)};
        }

        return data;
    }

    std::stringstream read(const char *filePath) {
        std::ifstream file {filePath};

        if (!file.is_open()) {
            throw std::string {"Falha ao abrir arquivo GLSL."};
        }

        std::stringstream code;

        code << file.rdbuf();

        file.close();

        return code;
    }
};

}

#endif
