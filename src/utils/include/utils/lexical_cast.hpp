#pragma once

#include "concepts.hpp"
#include <charconv>
#include <string>
#include <string_view>
#include <tl/optional.hpp>

namespace utils {
    template<IsNumeric T>
    [[nodiscard]] tl::optional<T> lexical_cast(std::string_view const string) {
        auto value = T{};
        auto const begin = string.data();
        auto const end = std::next(string.data(), static_cast<std::string_view::difference_type>(string.size()));
        auto const result = std::from_chars(begin, end, value);
        if (result.ptr != end or result.ec != std::errc{}) {
            return tl::nullopt;
        }
        return value;
    }

    template<IsNumeric T>
    [[nodiscard]] std::string lexical_cast(T const value) {
        return std::to_string(value);
    }
} // namespace utils
