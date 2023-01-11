#ifndef SHADER_HPP
#define SHADER_HPP

unsigned int compileShaderData(const char *dataPath, GLenum type);

unsigned int shaderProgram(const char *vertexPath, const char *fragmentPath) {
    auto Shader {glCreateProgram()};

    glAttachShader(Shader, compileShaderData(vertexPath, GL_VERTEX_SHADER));
    glAttachShader(Shader, compileShaderData(fragmentPath, GL_FRAGMENT_SHADER));
    glLinkProgram(Shader);

    auto success {0};

    glGetProgramiv(Shader, GL_LINK_STATUS, &success);

    if (!success) {
        throw std::runtime_error {
            "Falha ao compilar 'shader'."
        };
    }

    return Shader;
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

unsigned int compileShaderData(const char *dataPath, GLenum type) {
    auto dataCode {read(dataPath).str()};
    auto shaderCode {dataCode.c_str()};
    auto data {glCreateShader(type)};

    glShaderSource(data, 1, &shaderCode, nullptr);
    glCompileShader(data);

    auto success {0};

    glGetShaderiv(data, GL_COMPILE_STATUS, &success);

    if (success == 0) {
        if (type == GL_VERTEX_SHADER) {
            throw std::runtime_error {
                "Falha ao compilar 'vertex'."
            };
        } else {
            throw std::runtime_error {
                "Falha ao compilar 'fragment'."
            };
        }
    }

    return data;
}

#endif
