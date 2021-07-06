#pragma once

namespace Renderer {
namespace UI {
namespace Components {

struct ComponentBase {
    virtual void Draw() const = 0;
    virtual ~ComponentBase() {}
};

}; // Components 
}; // UI
}; // Renderer