#pragma once
#include <type_traits>


namespace Utils {

template<auto T>
struct always_fail : std::false_type {};

} // namespace Utils