#include "include/opengl_renderer.h"
#include "glad.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

OpenGLRenderer::OpenGLRenderer() 
    : shaderProgram(0), VAO(0), VBO(0), EBO(0), 
      timeLocation(-1), resolutionLocation(-1),
      vertexModTime(0), fragmentModTime(0) {
}

OpenGLRenderer::~OpenGLRenderer() {
    cleanup();
}

bool OpenGLRenderer::initialize() {
    return true;
}

void OpenGLRenderer::setupQuad() {
    float vertices[] = {
        -1.0f, -1.0f,
         1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f,  1.0f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        1, 2, 3
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

bool OpenGLRenderer::loadShaders(const std::string& vertexPath, const std::string& fragmentPath) {
    vertexShaderPath = vertexPath;
    fragmentShaderPath = fragmentPath;
    
    shaderProgram = createShaderProgram(vertexPath, fragmentPath);
    if (shaderProgram == 0) {
        return false;
    }
    
    vertexModTime = getFileModTime(vertexPath);
    fragmentModTime = getFileModTime(fragmentPath);
    
    glUseProgram(shaderProgram);
    timeLocation = glGetUniformLocation(shaderProgram, "u_time");
    resolutionLocation = glGetUniformLocation(shaderProgram, "u_resolution");
    
    return true;
}

void OpenGLRenderer::render(float time, int width, int height) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    
    glUniform1f(timeLocation, time);
    glUniform2f(resolutionLocation, width, height);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void OpenGLRenderer::cleanup() {
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
    if (shaderProgram != 0) {
        glDeleteProgram(shaderProgram);
        shaderProgram = 0;
    }
}

bool OpenGLRenderer::checkShadersModified() {
    time_t newVertexModTime = getFileModTime(vertexShaderPath);
    time_t newFragmentModTime = getFileModTime(fragmentShaderPath);
    
    if (newVertexModTime > vertexModTime || newFragmentModTime > fragmentModTime) {
        std::cout << "Shaders modified, reloading..." << std::endl;
        glDeleteProgram(shaderProgram);
        shaderProgram = createShaderProgram(vertexShaderPath, fragmentShaderPath);
        timeLocation = glGetUniformLocation(shaderProgram, "u_time");
        resolutionLocation = glGetUniformLocation(shaderProgram, "u_resolution");
        
        vertexModTime = newVertexModTime;
        fragmentModTime = newFragmentModTime;
        return true;
    }
    
    return false;
}

std::string OpenGLRenderer::loadShaderSource(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open shader file: " << filepath << std::endl;
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint OpenGLRenderer::compileShader(const std::string& source, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR: Shader compilation failed\n" << infoLog << std::endl;
    }

    return shader;
}

GLuint OpenGLRenderer::createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexSource = loadShaderSource(vertexPath);
    std::string fragmentSource = loadShaderSource(fragmentPath);

    GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "ERROR: Shader program linking failed\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

time_t OpenGLRenderer::getFileModTime(const std::string& filepath) {
    struct stat fileInfo;
    if (stat(filepath.c_str(), &fileInfo) != 0) {
        return 0;
    }
    return fileInfo.st_mtime;
}