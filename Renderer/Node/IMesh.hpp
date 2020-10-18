#pragma once

#include <variant>

namespace Render {

// We only need to be able to reach it by reference so...
namespace Contour { class Mesh; }
namespace Skybox { class Mesh; }

using MeshVariant = typename std::variant<
    const Contour::Mesh*,
    const Skybox::Mesh*
>;

// We need this thing to dispatch drawing to correct program, but keeping 
// the ability to keep abstract model in Nodes.
class IMesh {
public:
    // Return std:variant that can be visited. This way client decides on 
    // which things to handle and which to discard with default implementation.
    // If we made interface for visitor, we'd have to make method for each type... ugh.
    // With this - just add any option variant might be to the ModelsVariant type.
    virtual const MeshVariant specify() const = 0;
};

// Also this CRTP so that we don't really have to implement it manually.
template<class T>
class MeshImpl : public IMesh {

    const MeshVariant specify() const override {
        return static_cast<const T*>(this);
    }
};

} // namespace Render