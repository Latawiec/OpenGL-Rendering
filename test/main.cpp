#include <iostream>
#include <numeric>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.hpp"
#include "Renderer.hpp"


using namespace Render;


std::vector<float> vertices = {
	// pos				   // normal		    // texcoord   
    -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f, 	0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,     1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,     1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,     1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,     0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,     0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f, 	    0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f, 	    1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f, 	    1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f, 	    1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f, 	    0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f, 	    0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,     1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,     1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,     0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,     0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,     0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,     1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,     0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,     1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,     1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,     1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,     0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,     0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,     0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,     1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,     1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,     1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,     0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,     0.0f, 1.0f
};
std::vector<unsigned int> indices(36);

int main() {
    std::iota(indices.begin(), indices.end(), 0);

    Camera camera;
    constexpr int windowWidth = 800;
    constexpr int windowHeight= 600;

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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

    glViewport(0, 0, windowWidth, windowHeight);
    camera.updateAspectRatio(static_cast<float>(windowWidth)/static_cast<float>(windowHeight));

    VertexDataBase cubeVertexData = VertexData<Layout::Interleaving, Vec3, Vec3, Vec2>(indices, 36, reinterpret_cast<std::byte*>(vertices.data()));
    Mesh cube(cubeVertexData);

    Renderer renderer(windowWidth, windowHeight);
    renderer.SetCamera(&camera);
    renderer.AddMesh(&cube);

    while(!glfwWindowShouldClose(window)) {
        renderer.Draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
	glfwTerminate();
	return 0;
}