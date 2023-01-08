#ifndef STB_IMAGE_WRAPPER_H
#define STB_IMAGE_WRAPPER_H

namespace STB_IMAGE {

void loadWindowIcon(GLFWwindow *window, const char *iconPath) {
    GLFWimage img;

    img.pixels = stbi_load(iconPath, &img.width, &img.height, 0, 4);

    if (img.pixels == nullptr) {
        throw std::string {"Falha ao carregar textura do ícone da janela de visualização."};
    }

    glfwSetWindowIcon(window, 1, &img);

    stbi_image_free(img.pixels);
}

unsigned int loadTexture(const char *texturePath) {
    auto texture {0u};

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

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
        throw std::string {"Falha ao carregar textura."};
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(pixels);

    return texture;
}

}

#endif
