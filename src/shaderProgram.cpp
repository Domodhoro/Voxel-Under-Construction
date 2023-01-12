#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "./shaderProgram.hpp"

auto success {0};

unsigned int compileShaderData(const char *dataPath, const GLenum type);

unsigned int shaderProgram(const char *vertexPath, const char *fragmentPath) {
    auto Shader {glCreateProgram()};

    glAttachShader(Shader, compileShaderData(vertexPath, GL_VERTEX_SHADER));
    glAttachShader(Shader, compileShaderData(fragmentPath, GL_FRAGMENT_SHADER));
    glLinkProgram(Shader);

    glGetProgramiv(Shader, GL_LINK_STATUS, &success);

    if (!success) {
        throw std::runtime_error {
            "Falha ao compilar 'shader'."
        };
    }

    return Shader;
}

void setBool(unsigned int &Shader, const char *name, bool &value) {
    glUniform1i(glGetUniformLocation(Shader, name), static_cast<int>(value));
}

void setInt(unsigned int &Shader, const char *name, int &value) {
    glUniform1i(glGetUniformLocation(Shader, name), value);
}

void setFloat(unsigned int &Shader, const char *name, float &value) {
    glUniform1f(glGetUniformLocation(Shader, name), value);
}

void setVec2(unsigned int &Shader, const char *name, const glm::tvec2<float> &value) {
    glUniform2fv(glGetUniformLocation(Shader, name), 1, glm::value_ptr(value));
}

void setVec3(unsigned int &Shader, const char *name, const glm::tvec3<float> &value) {
    glUniform3fv(glGetUniformLocation(Shader, name), 1, glm::value_ptr(value));
}

void setMatrix4fv(unsigned int &Shader, const char *name, const glm::mat4 &matrix) {
    glUniformMatrix4fv(glGetUniformLocation(Shader, name), 1, false, glm::value_ptr(matrix));
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

unsigned int compileShaderData(const char *dataPath, const GLenum type) {
    auto dataCode {read(dataPath).str()};
    auto shaderCode {dataCode.c_str()};
    auto Data {glCreateShader(type)};

    glShaderSource(Data, 1, &shaderCode, nullptr);
    glCompileShader(Data);

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
