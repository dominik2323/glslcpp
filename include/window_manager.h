#pragma once

#include <GLFW/glfw3.h>

class WindowManager {
public:
    WindowManager(int width, int height, const char* title);
    ~WindowManager();

    bool initialize();
    void makeContextCurrent();
    bool shouldClose();
    void swapBuffers();
    void pollEvents();
    void getFramebufferSize(int* width, int* height);
    float getTime();
    void processInput();
    void terminate();

private:
    GLFWwindow* window;
    int windowWidth;
    int windowHeight;
    const char* windowTitle;

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};