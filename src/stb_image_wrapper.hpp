#ifndef STB_IMAGE_WRAPPER_HPP
#define STB_IMAGE_WRAPPER_HPP

#include <GLFW/glfw3.h>

void loadWindowIcon(GLFWwindow *window, const char *iconPath);

unsigned int loadTexture(const char *texturePath);

#endif
