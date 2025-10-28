#include "include/window_manager.h"
#include "glad.h"
#include <iostream>

WindowManager::WindowManager(int width, int height, const char* title)
    : window(nullptr), windowWidth(width), windowHeight(height), windowTitle(title) {
}

WindowManager::~WindowManager() {
    terminate();
}

bool WindowManager::initialize() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    return true;
}

void WindowManager::makeContextCurrent() {
    glfwMakeContextCurrent(window);
}

bool WindowManager::shouldClose() {
    return glfwWindowShouldClose(window);
}

void WindowManager::swapBuffers() {
    glfwSwapBuffers(window);
}

void WindowManager::pollEvents() {
    glfwPollEvents();
}

void WindowManager::getFramebufferSize(int* width, int* height) {
    glfwGetFramebufferSize(window, width, height);
}

float WindowManager::getTime() {
    return glfwGetTime();
}

void WindowManager::processInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void WindowManager::terminate() {
    if (window) {
        glfwTerminate();
        window = nullptr;
    }
}

void WindowManager::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
}