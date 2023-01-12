#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

unsigned int shaderProgram(const char *vertexPath, const char *fragmentPath);

void setBool(unsigned int &Shader, const char *name, bool &value);
void setInt(unsigned int &Shader, const char *name, int &value);
void setFloat(unsigned int &Shader, const char *name, float &value);
void setVec2(unsigned int &Shader, const char *name, const glm::tvec2<float> &value);
void setVec3(unsigned int &Shader, const char *name, const glm::tvec3<float> &value);
void setMatrix4fv(unsigned int &Shader, const char *name, const glm::mat4 &matrix);

#endif
