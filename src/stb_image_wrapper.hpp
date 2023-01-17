#ifndef STB_IMAGE_WRAPPER_HPP
#define STB_IMAGE_WRAPPER_HPP

namespace stb_image_wrapper {

void load_window_icon(GLFWwindow *window, const char *icon_path) {
    GLFWimage img;

    try {
        img.pixels = stbi_load(icon_path, &img.width, &img.height, 0, 4);

        if (!img.pixels) throw program_exception {"Falha ao carregar ícone da janela de visualização"};

        glfwSetWindowIcon(window, 1, &img);

        stbi_image_free(img.pixels);
    } catch (program_exception &e) { printf("%s", e.get_description()); }
}

unsigned int load_texture(const char *texture_path) {
    auto texture {0u};

    glGenTextures  (1, &texture);
    glBindTexture  (GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(true);

    auto width    {0};
    auto height   {0};
    auto channels {0};

    try {
        auto pixels {stbi_load(texture_path, &width, &height, &channels, 0)};

        if (!pixels) throw program_exception {"Falha ao carregar textura."};

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(pixels);
    } catch (program_exception &e) { printf("%s\n", e.get_description()); }

    return texture;
}

}

#endif
