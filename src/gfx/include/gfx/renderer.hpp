#pragma once

#include "font.hpp"
#include "utils/rect.hpp"
#include <lib2k/unique_value.hpp>
#include <utils/color.hpp>
#include <utils/vec2.hpp>
#include <variant>

namespace gfx {
    class Window;

    class Renderer final {
        friend class Window;

    private:
        struct Deleter {
            void operator()(std::monostate) const;
        };

        c2k::UniqueValue<std::monostate, Deleter> m_handle;

        Renderer();

    public:
        void clear(utils::Color color);
        void draw_text(Font const& font, char const* text, utils::Vec2i position, float size, utils::Color color);
        void draw_filled_rectangle(utils::IntRect const& area, utils::Color color);
    };
} // namespace gfx
