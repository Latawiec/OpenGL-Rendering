
// IMGUI
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
// IMGUI

#include <iostream>
#include <chrono>
#include <numeric>
#include <thread>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Importer.hpp"
#include <Scene/Base/Texture.hpp>
#include <Scene/Scene.hpp>
#include <SceneDrawing/SceneDrawingManager.hpp>

#include <UI/Components/TransformEditor.hpp>
#include <UI/Components/SceneHierarchy.hpp>
#include <UI/Components/CameraController.hpp>
#include <UI/Manager/InputManager.hpp>
#include <UI/Manager/UIManager.hpp>

struct InputParameters {
    // Parameter flags
    constexpr static std::string_view SceneFilePathFlag = "-f";

    // Arguments
    std::string sceneFilePath = "";
};

InputParameters ParseProgramInput(int argc, char** argv) {
    std::map<std::string_view, int> argumentsFound = {
        {InputParameters::SceneFilePathFlag, -1}
    };


    // Iterate over passed arguments and find anything that matches our flags.
    for (int i = 0; i < argc; ++i) {
        const char* argumentValue = argv[i];
        auto found = std::find_if(argumentsFound.begin(), argumentsFound.end(), [argumentValue](const std::pair<std::string_view, int>& element) {
            return element.first.compare(argumentValue) == 0;
        });

        if (found != argumentsFound.end()) {
            found->second = i;
        }
    }

    // Prepare parsed input.
    InputParameters parameters;
    if (argumentsFound[InputParameters::SceneFilePathFlag] != -1) {
        parameters.sceneFilePath = argv[argumentsFound[InputParameters::SceneFilePathFlag] + 1];
    }

    return parameters;
}

int main(int argc, char** argv) {

    InputParameters inputParams = ParseProgramInput(argc, argv);

    constexpr int windowWidth = 1600;
    constexpr int windowHeight= 1200;
    constexpr float FPS = 15.0;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
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

#if DEPTH_REVERSE_Z
    {
        GLint major, minor;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);

        if ((major > 4 || (major == 4 && minor >= 5)) ||
            glfwExtensionSupported("GL_ARB_clip_control"))
        {
            glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
            glDepthFunc(GL_GREATER);
            glClearDepth(0.0f);
        }
        else
        {
            fprintf(stderr, "glClipControl required, sorry.\n");
            exit(1);
        }
    }
#endif

    Renderer::Scene::Scene mainScene;
    Renderer::Importer::Importer gltfImporter;
    Renderer::UI::Manager::UIManager uiManager(window, "#version 410");
    Renderer::UI::Manager::InputManager inputManager{};
    if (inputParams.sceneFilePath.empty()) {
        std::cout << "No scene file path has been provided." << std::endl;
    } else {
        std::cout << "Scene file: " << inputParams.sceneFilePath << std::endl;
        if (gltfImporter.importGltf(inputParams.sceneFilePath, mainScene)) {
            std::cout << "Scene loaded successfuly." << std::endl;
        } else {
            std::cout << "Failed to load scene." << std::endl;
        }
    }
#ifndef NDEBUG
    std::cout << mainScene << std::endl;
#endif

    // auto& cameraNode = mainScene.GetNode(mainScene.GetSceneViews().begin()->nodeId);
    // const auto cacheTransform = cameraNode.GetTransform();

    // auto& lightNode = mainScene.GetNode(mainScene.GetSceneLights().begin()->nodeId);
    // const auto cacheLightTransform = lightNode.GetTransform();

    Renderer::SceneDrawing::SceneDrawingManager sceneDrawingManager(mainScene, windowWidth, windowHeight);

    sceneDrawingManager.SetResolution(windowWidth / 2, windowHeight / 2);

    const std::chrono::duration<float> frameTimeCap(1.f / FPS);

    uiManager.Initialize();
    auto transformEditor = std::make_shared<Renderer::UI::Components::TransformEditor>();
    auto sceneHierarchy = std::make_shared<Renderer::UI::Components::SceneHierarchy>();
    auto cameraController = std::make_shared<Renderer::UI::Components::CameraController>();
    uiManager.AddComponent(transformEditor);
    uiManager.AddComponent(sceneHierarchy);
    uiManager.AddComponent(cameraController);

    // Just stick it in there...
    struct WindowData {
        Renderer::SceneDrawing::SceneDrawingManager* drawingManager;
    } windowData { &sceneDrawingManager };

    glfwSetWindowUserPointer(window, &windowData);    
    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        WindowData* userData = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        assert(userData != nullptr);

        userData->drawingManager->SetWindowSize(width, height);
    });

    sceneHierarchy->SetScene(&mainScene);
    cameraController->SetScene(&mainScene);

    struct CameraControllerHandling : public Renderer::UI::Manager::InputManager::IKeyboardListener {

        CameraControllerHandling(const Renderer::UI::Components::CameraController& cameraController, const Renderer::SceneDrawing::SceneDrawingManager& sceneDrawingManager, Renderer::Scene::Scene& scene)
        : _scene(scene)
        , _sceneDrawingManager(sceneDrawingManager)
        , _cameraController(cameraController)
        {}

        virtual void OnHandleInput(bool (&keysDown)[512]) {

            if (_cameraController.IsCameraPossessed()) {
                
                const auto& selectedSceneView = _cameraController.GetSelectedSceneView();
                const auto& selectedCamera = _scene.GetCamera(selectedSceneView.cameraId);
                auto& cameraNode = _scene.GetNode(selectedSceneView.nodeId);
                const auto& cameraTransform = cameraNode.GetTransform();

                const float scale = 0.005f * _cameraController.GetCameraSpeed();

                if (keysDown[GLFW_KEY_W]) {
                    cameraNode.SetTransform(
                        glm::translate(cameraTransform, scale * glm::vec3(_sceneDrawingManager.GetNodeWorldTransform(selectedSceneView.nodeId) * selectedCamera.GetCameraOrientation() * glm::vec4{0, 0, -1.0, 0}))
                    );
                
                }

                if (keysDown[GLFW_KEY_S]) {
                    cameraNode.SetTransform(
                        glm::translate(cameraTransform, scale * glm::vec3(_sceneDrawingManager.GetNodeWorldTransform(selectedSceneView.nodeId) * selectedCamera.GetCameraOrientation() * glm::vec4{0, 0, 1.0, 0}))
                    );
                }

                if (keysDown[GLFW_KEY_A]) {
                    cameraNode.SetTransform(
                        glm::translate(cameraTransform, scale * glm::vec3(_sceneDrawingManager.GetNodeWorldTransform(selectedSceneView.nodeId) * selectedCamera.GetCameraOrientation() * glm::vec4{-1.0, 0, 0, 0}))
                    );
                
                }

                if (keysDown[GLFW_KEY_D]) {
                    cameraNode.SetTransform(
                        glm::translate(cameraTransform, scale * glm::vec3(_sceneDrawingManager.GetNodeWorldTransform(selectedSceneView.nodeId) * selectedCamera.GetCameraOrientation() * glm::vec4{1.0, 0, 0, 0}))
                    );
                }

                if (keysDown[GLFW_KEY_SPACE]) {
                    cameraNode.SetTransform(
                        glm::translate(cameraTransform, scale * glm::vec3(_sceneDrawingManager.GetNodeWorldTransform(selectedSceneView.nodeId) * selectedCamera.GetCameraOrientation() * glm::vec4{0, 1.0, 0, 0}))
                    );
                }

            }
        };

        protected:
            Renderer::Scene::Scene& _scene;
            const Renderer::SceneDrawing::SceneDrawingManager& _sceneDrawingManager;
            const Renderer::UI::Components::CameraController& _cameraController;
    };

    inputManager.RegisterKeyboardListener(std::make_unique<CameraControllerHandling>(*cameraController, sceneDrawingManager, mainScene));

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        const std::chrono::time_point<std::chrono::system_clock> startFrame = std::chrono::system_clock::now();
        sceneDrawingManager.Draw();

        // const auto transform = cacheLightTransform * glm::rotate(glm::mat4{1}, glm::radians(90.f), glm::vec3(0, 1, 0));
        // const auto transform2 = glm::rotate(cacheLightTransform, float(glfwGetTime())/1.f, glm::vec3(0, 1, 0));
        //lightNode.SetTransform(transform2);

        const static glm::mat4 cameraCachedTransform = [&]() {
            // just once.
            return sceneDrawingManager.GetNodeWorldTransform(mainScene.GetSceneViews().begin()->nodeId);
        }();

        //cameraNode.SetTransform(glm::rotate(cacheTransform, float(glm::radians(3.f * glfwGetTime())), glm::vec3(0, 1, 0)));

        uiManager.Render();

        glfwSwapBuffers(window);

        #ifndef NDEBUG
        while (auto error = glGetError()) {
            std::cerr << "Error: " << error << '\n';
        }
        #endif
        const std::chrono::time_point<std::chrono::system_clock> endFrame = std::chrono::system_clock::now();
        const auto frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(endFrame - startFrame);
        const auto frameTimeLeft = std::chrono::duration_cast<std::chrono::milliseconds>(frameTimeCap - frameTime);

        std::this_thread::sleep_for(frameTimeLeft);

        transformEditor->Apply();
        transformEditor->SetNode(sceneHierarchy->GetSelected() ? &mainScene.GetNode(sceneHierarchy->GetSelected()->nodeId) : nullptr );

        inputManager.HandleInput();
    }
	glfwTerminate();
	return 0;
}