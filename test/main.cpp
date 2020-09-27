#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.hpp"
#include "Renderer.hpp"


using namespace Render;

int main() {

    Camera camera;
    constexpr int windowWidth = 800;
    constexpr int windowHeight= 400;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
    glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

    glViewport(0, 0, windowWidth, windowHeight);
    camera.updateAspectRatio(static_cast<float>(windowWidth)/static_cast<float>(windowHeight));

    Renderer renderer(windowWidth, windowHeight);

    while(!glfwWindowShouldClose(window)) {
        renderer.Draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
	glfwTerminate();
	return 0;
}