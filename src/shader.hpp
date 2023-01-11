#ifndef SHADER_HPP
#define SHADER_HPP

class Shader {
public:
    Shader(const char *vertexPath, const char *fragmentPath) : m_Shader {glCreateProgram()} {
        glAttachShader(m_Shader, compileShaderData(vertexPath, GL_VERTEX_SHADER));
        glAttachShader(m_Shader, compileShaderData(fragmentPath, GL_FRAGMENT_SHADER));
        glLinkProgram(m_Shader);

        auto success {0};

        glGetProgramiv(m_Shader, GL_LINK_STATUS, &success);

        if (!success) {
            char infoLog[512] {};

            glGetProgramInfoLog(m_Shader, 512, nullptr, infoLog);

            throw std::string {"Falha ao compilar shader.\n" + std::string(infoLog)};
        }
    }

    ~Shader() {
        glDeleteProgram(m_Shader);
    }

    void use() {
        glUseProgram(m_Shader);;
    }

    void setBool(const char *name, bool value) {
        glUniform1i(glGetUniformLocation(m_Shader, name), static_cast<int>(value));
    }

    void setInt(const char *name, int value) {
        glUniform1i(glGetUniformLocation(m_Shader, name), value);
    }

    void setFloat(const char *name, float value) {
        glUniform1f(glGetUniformLocation(m_Shader, name), value);
    }

    void setMatrix4fv(const char *name, const glm::mat4& matrix) {
        glUniformMatrix4fv(glGetUniformLocation(m_Shader, name), 1, false, glm::value_ptr(matrix));
    }

    void setVec2(const char *name, const glm::tvec2<float>& value) {
        glUniform2fv(glGetUniformLocation(m_Shader, name), 1, glm::value_ptr(value));
    }

    void setVec3(const char *name, const glm::tvec3<float>& value) {
        glUniform3fv(glGetUniformLocation(m_Shader, name), 1, glm::value_ptr(value));
    }

private:
    unsigned int m_Shader {0u};

    unsigned int compileShaderData(const char *dataPath, const GLenum type) {
        auto dataCode {read(dataPath).str()};
        auto shaderCode {dataCode.c_str()};
        auto data {glCreateShader(type)};

        glShaderSource(data, 1, &shaderCode, nullptr);
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

#endif
