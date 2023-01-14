#ifndef STB_IMAGE_WRAPPER_HPP
#define STB_IMAGE_WRAPPER_HPP

void loadWindowIcon(GLFWwindow *window, const char *iconPath) {
    GLFWimage Img;

    Img.pixels = stbi_load(iconPath, &Img.width, &Img.height, 0, 4);

    if (Img.pixels == nullptr) {
        throw std::runtime_error {
            "Falha ao carregar textura do ícone da janela de visualização."
        };
    }

    glfwSetWindowIcon(window, 1, &Img);

    stbi_image_free(Img.pixels);
}

unsigned int loadTexture(const char *texturePath) {
    auto Texture {0u};

    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(true);

    auto width {0}, height {0}, channels {0};

    auto pixels {
        stbi_load(texturePath, &width, &height, &channels, 0)
    };

    if (pixels == nullptr) {
        throw std::runtime_error {
            "Falha ao carregar textura."
        };
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(pixels);

    return Texture;
}

#endif
