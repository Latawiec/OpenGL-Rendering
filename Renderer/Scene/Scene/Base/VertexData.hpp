#pragma once 

#include <tuple>
#include <vector>
#include <stdint.h>
#include <glad/glad.h>
#include <cstddef>


namespace Renderer {
namespace Scene {
namespace Base {
    
enum class Layout : uint8_t {
    Sequential,
    Interleaving
};

// This is pretty straight forward. vec3 -> VertexAttribute<3, float>... right?
template<int Count, typename T>
struct VertexAttribute {
    using value_type = std::decay_t<T>;
    using elements_count = std::integral_constant<int, Count>;
    using byte_size = std::integral_constant<int, Count * sizeof(value_type)>;
    constexpr static int glType() {
        if constexpr(std::is_same_v<value_type, float>) return GL_FLOAT;
        return -1;
    }
    static_assert(glType() != -1, "Given type is not supported or does not have a valid GL type assigned.");
};

using Vec3 = VertexAttribute<3, float>;
using Vec2 = VertexAttribute<2, float>;
using Float = VertexAttribute<1, float>;
// More?

class VertexDataBase {   
protected:
    unsigned int _VAO = -1;
    // I should be able to only keep VAO and OpenGL would keep all VBOs and EBOs bound to it automagically... but it doesnt. So I keep them.
    std::vector<GLuint> _buffers = {};
    size_t _elementsCount = 0;

    VertexDataBase& operator=(const VertexDataBase&) = delete;
    VertexDataBase(const VertexDataBase&) = delete;
public:
    VertexDataBase() {}

    VertexDataBase(const std::vector<unsigned int>& indices) :
    _elementsCount(indices.size()) {}

    VertexDataBase(const GLuint VAO, const size_t elementsCount, std::vector<GLuint> buffers = {}) :
    _VAO(VAO),
    _buffers(std::move(buffers)),
    _elementsCount(elementsCount) {}

    VertexDataBase(VertexDataBase&& other) noexcept {
        std::swap(this->_VAO, other._VAO);
        std::swap(this->_buffers, other._buffers);
        std::swap(this->_elementsCount, other._elementsCount);
    }

    VertexDataBase& operator=(VertexDataBase&& other) noexcept {
        std::swap(this->_VAO, other._VAO);
        std::swap(this->_buffers, other._buffers);
        std::swap(this->_elementsCount, other._elementsCount);

        return *this;
    }

    ~VertexDataBase() {
        if (_VAO != -1) {
            glDeleteVertexArrays(1, &_VAO);
        }

        if (_buffers.size() > 0) {
            glDeleteBuffers(_buffers.size(), _buffers.data());
        }
    }

    constexpr int vertexCount() const {
        return _elementsCount;
    }

    struct ScopedBinding {
        ScopedBinding(const VertexDataBase& vertexData) { glBindVertexArray(vertexData._VAO); }
        ~ScopedBinding() { glBindVertexArray(0); }
    };
};



template <Layout, class ... >
class VertexData {};


template <class ... VertexAttributeDescription>
class VertexData<Layout::Interleaving, VertexAttributeDescription...>
: public VertexDataBase {
    using TupleOfDescriptions = std::tuple<VertexAttributeDescription...>;
    size_t _size;
public:    
    constexpr VertexData() = default;

    constexpr VertexData(const std::vector<unsigned int>& indices, const size_t size, const std::byte* data)
    : VertexDataBase(indices), _size(size)
    {
        glGenVertexArrays(1, &_VAO);
        glBindVertexArray(_VAO);

        GLuint EBO, VBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        
        layoutInterleavingData(data);
        layoutInterleavingAttributes<VertexAttributeDescription...>();

        _buffers = { EBO, VBO };

        glBindVertexArray(0);
    }

private:
    inline void layoutInterleavingData(const std::byte* data) {
        const int bufferByteSize = _size * (VertexAttributeDescription::byte_size::value + ...);
        glBufferData(GL_ARRAY_BUFFER, bufferByteSize, data, GL_STATIC_DRAW);
    }

    template<class LastOfDescriptions>
    inline void layoutInterleavingAttributes(const int index = 0, const int offset = 0) {
        const int stride = (VertexAttributeDescription::byte_size::value + ...);
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, LastOfDescriptions::elements_count::value, LastOfDescriptions::glType(), GL_FALSE, stride, (const void*)offset);
    }

    template<class HeadOfDescriptions, class NextDescrption, class ... RestOfDescriptions>
    inline void layoutInterleavingAttributes(const int index = 0, const int offset = 0) {
        const int stride = (VertexAttributeDescription::byte_size::value + ...);
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, HeadOfDescriptions::elements_count::value, HeadOfDescriptions::glType(), GL_FALSE, stride, (const void*)offset);
        layoutInterleavingAttributes<NextDescrption, RestOfDescriptions...>(index + 1, offset + HeadOfDescriptions::byte_size::value);
    }
};

template <class ... VertexAttributeDescription>
class VertexData<Layout::Sequential, VertexAttributeDescription...>
: public VertexDataBase {
    using TupleOfDescriptions = std::tuple<VertexAttributeDescription...>;
    size_t _size;
public:    
    constexpr VertexData() = default;

    constexpr VertexData(const std::vector<unsigned int>& indices, const size_t size, const typename VertexAttributeDescription::value_type* ... data)
    : VertexDataBase(indices), _size(size)
    {
        glGenVertexArrays(1, &_VAO);
        glBindVertexArray(_VAO);

        GLuint EBO, VBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        allocateBuffer();
        layoutSequentialAttributes<VertexAttributeDescription...>();
        layoutSequentialData(data...);

        _buffers = { EBO, VBO };

        glBindVertexArray(0);
    }

    template<int Index>
    void setData(const typename std::tuple_element_t<Index, TupleOfDescriptions>::value_type* data) {
        glBufferSubData(GL_ARRAY_BUFFER, sum_byte_size_until<Index>() * _size, std::tuple_element_t<Index, TupleOfDescriptions>::byte_size::value * _size, data);
    }

private:

    template<size_t ... Index>
    constexpr std::size_t sum_byte_size_of(std::integer_sequence<std::size_t, Index...>) {
        if constexpr (sizeof...(Index) == 0) return 0;
        else return (std::tuple_element_t<Index, TupleOfDescriptions>::byte_size::value + ...);
    }

    template<size_t EndIndex>
    constexpr std::size_t sum_byte_size_until() {
        return sum_byte_size_of(std::make_index_sequence<EndIndex>{});
    }

    inline void allocateBuffer() {
        const int bufferByteSize = _size * (VertexAttributeDescription::byte_size::value + ...);
        glBufferData(GL_ARRAY_BUFFER, bufferByteSize, NULL, GL_STATIC_DRAW);
    }

    inline void layoutSequentialData(const typename VertexAttributeDescription::value_type* ... data) {
        [&]<std::size_t ... I>(std::index_sequence<I...>) { (setData<I>(std::get<I>(std::make_tuple(data...))), ...); }(std::make_index_sequence<sizeof...(VertexAttributeDescription)>{});
    }

    template<class LastOfDescriptions>
    inline void layoutSequentialAttributes(const int index = 0, const int offset = 0) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, LastOfDescriptions::elements_count::value, LastOfDescriptions::glType(), GL_FALSE, LastOfDescriptions::byte_size::value, (char *)NULL + offset);
    }

    template<class HeadOfDescriptions, class NextDescription, class ... RestOfDescriptions>
    inline void layoutSequentialAttributes(const int index = 0, const int offset = 0) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, HeadOfDescriptions::elements_count::value, HeadOfDescriptions::glType(), GL_FALSE, HeadOfDescriptions::byte_size::value, (char *)NULL + offset);
        layoutSequentialAttributes<NextDescription, RestOfDescriptions...>(index + 1, offset + HeadOfDescriptions::byte_size::value * _size);
    }
};

} // namespace Base
} // namespace Scene
} // namespace Renderer