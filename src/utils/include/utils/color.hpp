#pragma once

#include <lib2k/random.hpp>
#include <lib2k/types.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <stdexcept>

namespace utils {
    struct Color {
        u8 r{};
        u8 g{};
        u8 b{};
        u8 a{};

        enum class Constant {
            LightGray,
            Gray,
            DarkGray,
            Yellow,
            Gold,
            Orange,
            Pink,
            Red,
            Maroon,
            Green,
            Lime,
            DarkGreen,
            SkyBlue,
            Blue,
            DarkBlue,
            Purple,
            Violet,
            DarkPurple,
            Beige,
            Brown,
            DarkBrown,
            White,
            Black,
            Blank,
            Magenta,
            RayWhite,
        };

        using enum Constant;

        constexpr Color() = default;
        constexpr Color(u8 const r, u8 const g, u8 const b) : r{ r }, g{ g }, b{ b }, a{ 255 } { }
        constexpr Color(u8 const r, u8 const g, u8 const b, u8 const a) : r{ r }, g{ g }, b{ b }, a{ a } { }

        constexpr Color(Constant const color) { // NOLINT
            switch (color) {
                case LightGray:
                    *this = { 200, 200, 200, 255 };
                    break;
                case Gray:
                    *this = { 130, 130, 130, 255 };
                    break;
                case DarkGray:
                    *this = { 80, 80, 80, 255 };
                    break;
                case Yellow:
                    *this = { 253, 249, 0, 255 };
                    break;
                case Gold:
                    *this = { 255, 203, 0, 255 };
                    break;
                case Orange:
                    *this = { 255, 161, 0, 255 };
                    break;
                case Pink:
                    *this = { 255, 109, 194, 255 };
                    break;
                case Red:
                    *this = { 230, 41, 55, 255 };
                    break;
                case Maroon:
                    *this = { 190, 33, 55, 255 };
                    break;
                case Green:
                    *this = { 0, 228, 48, 255 };
                    break;
                case Lime:
                    *this = { 0, 158, 47, 255 };
                    break;
                case DarkGreen:
                    *this = { 0, 117, 44, 255 };
                    break;
                case SkyBlue:
                    *this = { 102, 191, 255, 255 };
                    break;
                case Blue:
                    *this = { 0, 121, 241, 255 };
                    break;
                case DarkBlue:
                    *this = { 0, 82, 172, 255 };
                    break;
                case Purple:
                    *this = { 200, 122, 255, 255 };
                    break;
                case Violet:
                    *this = { 135, 60, 190, 255 };
                    break;
                case DarkPurple:
                    *this = { 112, 31, 126, 255 };
                    break;
                case Beige:
                    *this = { 211, 176, 131, 255 };
                    break;
                case Brown:
                    *this = { 127, 106, 79, 255 };
                    break;
                case DarkBrown:
                    *this = { 76, 63, 47, 255 };
                    break;
                case White:
                    *this = { 255, 255, 255, 255 };
                    break;
                case Blank:
                    *this = { 0, 0, 0, 0 };
                    break;
                case Magenta:
                    *this = { 255, 0, 255, 255 };
                    break;
                case RayWhite:
                    *this = { 245, 245, 245, 255 };
                    break;
                case Black:
                default:
                    *this = { 0, 0, 0, 255 };
                    break;
            }
        }

        template<std::floating_point T>
        [[nodiscard]] Color operator*(T const scalar) const {
            return Color{
                static_cast<u8>(std::clamp(static_cast<T>(r) * scalar, T{ 0.0 }, T{ 255.0 })),
                static_cast<u8>(std::clamp(static_cast<T>(g) * scalar, T{ 0.0 }, T{ 255.0 })),
                static_cast<u8>(std::clamp(static_cast<T>(b) * scalar, T{ 0.0 }, T{ 255.0 })),
                a,
            };
        }

        template<std::floating_point T>
        [[nodiscard]] friend Color operator*(T const scalar, Color const color) {
            return color * scalar;
        }

        [[nodiscard]] static Color random(c2k::Random& random) {
            auto const r = static_cast<u8>(random.next_integral<int>(0, 256));
            auto const g = static_cast<u8>(random.next_integral<int>(0, 256));
            auto const b = static_cast<u8>(random.next_integral<int>(0, 256));
            return Color{ r, g, b };
        }
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Color, r, g, b, a);
} // namespace utils
