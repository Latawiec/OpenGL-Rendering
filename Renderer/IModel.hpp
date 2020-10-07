#pragma once

#include <variant>

namespace Render {

// We only need to be able to reach it by reference so...
namespace Contour { class Model; }
namespace Skybox { class Model; }

using ModelsVariant = typename std::variant<
    Contour::Model*,
    Skybox::Model*
>;

// We need this thing to dispatch drawing to correct program, but keeping 
// the ability to keep abstract model in Nodes.
class IModel {
public:
    // Return std:variant that can be visited. This way client decides on 
    // which things to handle and which to discard with default implementation.
    // If we made interface for visitor, we'd have to make method for each type... ugh.
    // With this - just add any option variant might be to the ModelsVariant type.
    virtual ModelsVariant specify() = 0;
};

// Also this CRTP so that we don't really have to implement it manually.
template<class T>
class ModelImpl : public IModel {

    ModelsVariant specify() override {
        return static_cast<T*>(this);
    }
};

} // namespace Render