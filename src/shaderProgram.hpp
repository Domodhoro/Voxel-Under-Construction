#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

class Shader {
public:
    Shader(const char *vertexPath, const char *fragmentPath) : m_Shader {glCreateProgram()} {
        glAttachShader(m_Shader, compileShaderData(vertexPath, GL_VERTEX_SHADER));
        glAttachShader(m_Shader, compileShaderData(fragmentPath, GL_FRAGMENT_SHADER));
        glLinkProgram(m_Shader);

        auto success {0};

        glGetProgramiv(m_Shader, GL_LINK_STATUS, &success);

        if (!success) {
            throw std::runtime_error {
                "Falha ao compilar 'shader'."
            };
        }
    }

    ~Shader() {
        glDeleteProgram(m_Shader);
    }

    void use() {
        glUseProgram(m_Shader);
    }

    void setBool(const char *name, bool &value) {
        glUniform1i(glGetUniformLocation(m_Shader, name), static_cast<int>(value));
    }

    void setInt(const char *name, int &value) {
        glUniform1i(glGetUniformLocation(m_Shader, name), value);
    }

    void setFloat(const char *name, float &value) {
        glUniform1f(glGetUniformLocation(m_Shader, name), value);
    }

    void setVec2(const char *name, const glm::tvec2<float> &value) {
        glUniform2fv(glGetUniformLocation(m_Shader, name), 1, glm::value_ptr(value));
    }

    void setVec3(const char *name, const glm::tvec3<float> &value) {
        glUniform3fv(glGetUniformLocation(m_Shader, name), 1, glm::value_ptr(value));
    }

    void setMatrix4fv(const char *name, const glm::mat4 &matrix) {
        glUniformMatrix4fv(glGetUniformLocation(m_Shader, name), 1, false, glm::value_ptr(matrix));
    }

private:
    unsigned int m_Shader {0u};

    unsigned int compileShaderData(const char *dataPath, const GLenum type) {
        auto dataCode {read(dataPath).str()};
        auto shaderCode {dataCode.c_str()};
        auto Data {glCreateShader(type)};

        glShaderSource(Data, 1, &shaderCode, nullptr);
        glCompileShader(Data);

        auto success {0};

        glGetShaderiv(Data, GL_COMPILE_STATUS, &success);

        if (success == 0) {
            if (type == GL_VERTEX_SHADER) {
                throw std::runtime_error {
                    "Falha ao compilar 'vertex'."
                };
            } else if (type == GL_FRAGMENT_SHADER) {
                throw std::runtime_error {
                    "Falha ao compilar 'fragment'."
                };
            }
        }

        return Data;
    }

    std::stringstream read(const char *filePath) {
        std::ifstream file {filePath};

        if (!file.is_open()) {
            throw std::runtime_error {
                "Falha ao abrir arquivo GLSL."
            };
        }

        std::stringstream code;

        code << file.rdbuf();

        file.close();

        return code;
    }
};

#endif
