#include <iostream>
#include "include/window_manager.h"
#include "include/opengl_renderer.h"

const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    WindowManager windowManager(SCR_WIDTH, SCR_HEIGHT, "GLSL Shader Renderer");
    if (!windowManager.initialize()) {
        return -1;
    }

    OpenGLRenderer renderer;
    if (!renderer.initialize()) {
        return -1;
    }

    renderer.setupQuad();
    if (!renderer.loadShaders("shaders/vertex.glsl", "shaders/fragment.glsl")) {
        return -1;
    }

    while (!windowManager.shouldClose()) {
        windowManager.processInput();

        renderer.checkShadersModified();

        int width, height;
        windowManager.getFramebufferSize(&width, &height);
        
        float timeValue = windowManager.getTime();
        renderer.render(timeValue, width, height);

        windowManager.swapBuffers();
        windowManager.pollEvents();
    }

    return 0;
}

