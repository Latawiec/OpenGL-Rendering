#include "UI/Components/TransformEditor.hpp"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace Renderer {
namespace UI {
namespace Components {

void TransformEditor::Draw() const {

    ImGui::Begin("Transform Editor");                          // Create a window called "Hello, world!" and append into it.

    if (_node != nullptr ) {
        const glm::mat4& nodeTransform = _node->GetTransform();
        
        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 translation;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(nodeTransform, scale, rotation, translation, skew, perspective);
        //rotation = glm::conjugate(rotation);

        _translate[0] = translation.x;
        _translate[1] = translation.y;
        _translate[2] = translation.z;

        const auto eulerRotation = glm::eulerAngles(rotation) * 180.f / 3.14159f;

        _rotate[0] = eulerRotation.x;
        _rotate[1] = eulerRotation.y;
        _rotate[2] = eulerRotation.z;

        _scale[0] = scale.x;
        _scale[1] = scale.y;
        _scale[2] = scale.z;

        const float changeSpeed = 0.1f;
        ImGui::DragFloat3("Translation", _translate, changeSpeed);
        ImGui::DragFloat3("Rotation", _rotate, changeSpeed);
        ImGui::DragFloat3("Scale", _scale, changeSpeed);
    }
    ImGui::End();
}

void TransformEditor::Apply() {

    if (_node != nullptr) {
        const glm::mat4 rotationTransform = glm::mat4(glm::quat(glm::vec3(_rotate[0] * 3.14159f / 180.f, _rotate[1] * 3.14159f / 180.f, _rotate[2] * 3.14159f / 180.f)));
        const glm::mat4 scaleTransform = glm::scale(glm::mat4{1}, glm::vec3(_scale[0], _scale[1], _scale[2]));
        const glm::mat4 translateTransform = glm::translate(glm::mat4{1}, glm::vec3(_translate[0], _translate[1], _translate[2]));

        _node->SetTransform(translateTransform * rotationTransform * scaleTransform);
    }
}

void TransformEditor::SetNode(Scene::Base::Node* nodePtr) {
    _node = nodePtr;
}

}
}
}