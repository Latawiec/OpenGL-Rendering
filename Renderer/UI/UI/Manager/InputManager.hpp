#pragma once

#include <id_generator.hpp>

#include <unordered_map>
#include <memory>

namespace Renderer {
namespace UI {
namespace Manager {

struct InputManager {

using KeyboardListenerIdGenerator = Utils::IdGenerator<uint64_t>;

struct IKeyboardListener {
    virtual ~IKeyboardListener() {}
    virtual void OnHandleInput(bool (&keysDown)[512]) {};
};

InputManager();

KeyboardListenerIdGenerator::Type RegisterKeyboardListener(std::unique_ptr<IKeyboardListener> pListener);
bool RemoveKeyboardListener(const KeyboardListenerIdGenerator::Type& listenerId);

void HandleInput() const;

private:
    KeyboardListenerIdGenerator _keyboardListenersIdGenerator;
    std::unordered_map<KeyboardListenerIdGenerator::Type, std::unique_ptr<IKeyboardListener>> _keyboardListeners;
};

} // namespace Manager
} // namespace UI
} // namespace Renderer