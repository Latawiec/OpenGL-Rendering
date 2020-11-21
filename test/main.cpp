#include <iostream>
#include <numeric>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "BasicCamera.hpp"
#include "Common/Mesh.hpp"
#include <Common/Scene.hpp>
#include <Pipeline/SceneDrawingManager.hpp>
#include "Texture.hpp"
#include "Importer.hpp"

using namespace Render;

int main() {

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

    glClearColor(0.1f, 0.1f, 0.3f, 1.0f);

    //auto shadowingLight = std::make_unique<LightNode>();
    //auto& stolenLight = *shadowingLight;
    //shadowingLight->SetTransform(glm::translate(glm::mat4(1), glm::vec3(5, 2.5, 1)));
    Common::Scene mainScene;
    Importer gltfImporter;
    auto imported = gltfImporter.importGltf(ASSETS_DIR "/scene_test.gltf", mainScene);
    std::cout << "SCENE!\n" << std::endl;
    std::cout << mainScene << std::endl;
    //imported->AddChildNode(std::move(shadowingLight));

    Pipeline::SceneDrawingManager sceneDrawingManager(mainScene, 800, 600);
    while(!glfwWindowShouldClose(window)) {
        //stolenLight.SetTransform(glm::translate(glm::mat4(1), glm::vec3(5 * glm::sin(glfwGetTime()), 2.5, 1)));
        sceneDrawingManager.Draw();

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