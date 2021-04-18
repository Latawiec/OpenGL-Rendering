#include <iostream>
#include <numeric>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Importer.hpp"
#include <Scene/Base/Mesh.hpp>
#include <Scene/Base/Texture.hpp>
#include <Scene/Scene.hpp>
#include <SceneDrawing/SceneDrawingManager.hpp>

int main() {

    constexpr int windowWidth = 800;
    constexpr int windowHeight= 600;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
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

    glClearColor(0.1f, 0.1f, 0.3f, 1.0f);

    //auto shadowingLight = std::make_unique<LightNode>();
    //auto& stolenLight = *shadowingLight;
    //shadowingLight->SetTransform(glm::translate(glm::mat4(1), glm::vec3(5, 2.5, 1)));
    Renderer::Scene::Scene mainScene;
    Renderer::Importer::Importer gltfImporter;
    auto imported = gltfImporter.importGltf(ASSETS_DIR "/scene_test.gltf", mainScene);
    std::cout << "SCENE!\n" << std::endl;
    std::cout << mainScene << std::endl;
    //imported->AddChildNode(std::move(shadowingLight));

    Renderer::SceneDrawing::SceneDrawingManager sceneDrawingManager(mainScene, 800, 600);
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