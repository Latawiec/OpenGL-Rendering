
#include<glm/glm.hpp>


namespace Renderer {
namespace Utils {


#ifdef DEPTH_REVERSE_Z
// With Reverse-Z we need to flip zNear and zFar.
template<typename T>
GLM_FUNC_QUALIFIER glm::mat<4, 4, T, glm::defaultp> ortho(T left, T right, T bottom, T top, T zNear, T zFar)
{
    return glm::ortho(left, right, bottom, top, zFar, zNear);
}

template<typename T>
GLM_FUNC_QUALIFIER glm::mat<4, 4, T, glm::defaultp> perspective(T fovy, T aspect, T zNear, T zFar)
{
    return glm::perspective(fovy, aspect, zFar, zNear);
}

#else 
// Without Reverse-Z just forward.
template<typename T>
GLM_FUNC_QUALIFIER glm::mat<4, 4, T, glm::defaultp> ortho(T left, T right, T bottom, T top, T zNear, T zFar)
{
    return glm::ortho(left, right, bottom, top, zNear, zFar);
}

template<typename T>
GLM_FUNC_QUALIFIER glm::mat<4, 4, T, glm::defaultp> perspective(T fovy, T aspect, T zNear, T zFar)
{
    return glm::perspective(fovy, aspect, zNear, zFar);
}

#endif

} // namespace Utils
} // namespace Renderer