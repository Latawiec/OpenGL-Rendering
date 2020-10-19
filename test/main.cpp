#include <iostream>
#include <numeric>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "BasicCamera.hpp"
#include "Mesh.hpp"
#include "Program.hpp"
#include "GraphicBuffer.hpp"
#include "TextureProgram.hpp"
#include "DrawingManager.hpp"
#include "Texture.hpp"
#include "Importer.hpp"

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

    Render::Camera::BasicCamera camera;
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

    camera.updateRotation(0.f, -110.0f);
    camera.updatePosition(-10.f, 4.f, 1.f);

    VertexDataBase cubeVertexData = VertexData<Layout::Interleaving, Vec3, Vec3, Vec2>(indices, 36, reinterpret_cast<std::byte*>(vertices.data()));

    GraphicBuffer deferredBuffers(windowWidth, windowHeight);
    DebugUtils::TextureProgram textureDrawProgram;
    auto  cube = std::make_unique<Contour::Mesh>(std::move(cubeVertexData));
    Contour::Program program;

     glClearColor(0.1f, 0.1f, 0.3f, 1.0f);

    DrawingManager drawingManager;
    auto rootNode = std::make_unique<Node>();
    auto cubeNode = std::make_unique<Node>();
    auto imported = Importer::importGltf(ASSETS_DIR "/scene_test.gltf");
    //imported->SetTransform(glm::translate(imported->GetTransform(), glm::vec3(2, 0, 0)));
    rootNode->AddChildNode(std::move(imported));
    // cubeNode->SetMesh(std::move(cube));
    // cubeNode->SetTransform(glm::translate(cubeNode->GetTransform(), glm::vec3(2, 0, 0)));
    // rootNode->AddChildNode(std::move(cubeNode));

    while(!glfwWindowShouldClose(window)) {
        //rootNode->SetTransform(glm::rotate(rootNode->GetTransform(), 0.01f, glm::vec3(0, 1, 0)));
        {
            FramebufferBase::ScopedBinding bind(deferredBuffers);
            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            //program.Draw(camera.getViewTransform(), camera.getProjectionTransform(), glm::mat4{1}, cube);
            drawingManager.QueueNodes(*rootNode);
            drawingManager.Draw(camera.getViewTransform(), camera.getProjectionTransform());
        }
        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        textureDrawProgram.draw(deferredBuffers.getTexture(GraphicBuffer::Output::Normals));
        glfwSwapBuffers(window);
        glfwPollEvents();

        #ifndef NDEBUG
        while (auto error = glGetError()) {
            std::cerr << "Error: " << error << '\n';
        }
        #endif
    }
	glfwTerminate();
	return 0;
}