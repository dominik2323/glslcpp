#pragma once

#include <string>
#include <ctime>
#include "glad.h"

class OpenGLRenderer {
public:
    OpenGLRenderer();
    ~OpenGLRenderer();

    bool initialize();
    void setupQuad();
    bool loadShaders(const std::string& vertexPath, const std::string& fragmentPath);
    void render(float time, int width, int height);
    void cleanup();
    bool checkShadersModified();

private:
    GLuint shaderProgram;
    GLuint VAO, VBO, EBO;
    GLint timeLocation;
    GLint resolutionLocation;
    time_t vertexModTime;
    time_t fragmentModTime;
    std::string vertexShaderPath;
    std::string fragmentShaderPath;

    std::string loadShaderSource(const std::string& filepath);
    GLuint compileShader(const std::string& source, GLenum shaderType);
    GLuint createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
    time_t getFileModTime(const std::string& filepath);
};