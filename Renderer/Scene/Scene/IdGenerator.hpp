#pragma once

#include <stdint.h>

namespace Render {
namespace Common {

/**
 * This is very simple. It'll just return last value +1 because I just don't
 * expect to have too many elements in the scene.
 * 
 * If at any point I need something more sophisticated, I should be able
 * to change the type with overwritten ++ operator, that'd do some more magic.
 * No need for it now...
 */
template<class T>
struct IdGenerator {
    using Type = T;
    Type GenerateId() {
        return _lastGeneratedId++;
    }
private:
    Type _lastGeneratedId = 1; // 0 is invalid.
};

} // namespace Common
} // namespace Render