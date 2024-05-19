#pragma once

#include "key.hpp"
#include "mouse_button.hpp"
#include <utils/rect.hpp>
#include <utils/vec2.hpp>
#include <variant>

namespace ui {
    struct KeyPressed final {
        Key key;
    };

    struct KeyReleased final {
        Key key;
    };

    struct MouseClicked final {
        utils::Vec2i position;
        MouseButton button;
    };

    struct MouseReleased final {
        utils::Vec2i position;
        MouseButton button;
    };

    struct MouseMoved final {
        utils::Vec2i delta;
        utils::Vec2i position;
    };

    struct MouseWheelMoved final {
        utils::Vec2f delta;
    };

    struct WindowResized final {
        utils::IntRect new_area;
    };

    // clang-format off
    using Event = std::variant<
        KeyPressed,
        KeyReleased,
        MouseClicked,
        MouseReleased,
        MouseMoved,
        MouseWheelMoved,
        WindowResized
    >;
    // clang-format on
} // namespace ui
