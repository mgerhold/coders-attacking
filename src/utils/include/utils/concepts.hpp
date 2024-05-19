#pragma once

#include <concepts>

namespace utils {
    template<typename T>
    concept IsNumeric = std::integral<T> or std::floating_point<T>;
}
