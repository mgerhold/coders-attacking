#pragma once

#include "font.hpp"
#include <utils/rect.hpp>
#include <utils/color.hpp>
#include <utils/vec2.hpp>

namespace gfx {

    class RenderContext;

    class Renderer final {
        friend class RenderContext;

        [[nodiscard]] Renderer() = default;

    public:
        // should only be used as reference
        Renderer(Renderer const& other) = delete;
        Renderer(Renderer&& other) noexcept = delete;
        Renderer& operator=(Renderer const& other) = delete;
        Renderer& operator=(Renderer&& other) noexcept = delete;
        ~Renderer() = default;

        void clear(utils::Color color);
        void draw_text(Font const& font, char const* text, utils::Vec2i position, float size, utils::Color color);
        void draw_filled_rectangle(utils::IntRect const& area, utils::Color color);
        // clang-format off
        void draw_filled_rounded_rectangle(
            utils::IntRect const& area,
            float roundness,
            int segments,
            utils::Color color
        );
        // clang-format on
        void draw_rounded_rectangle_outline(
                utils::IntRect const& area,
                float roundness,
                int segments,
                float line_thickness,
                utils::Color color
        );
        void draw_circle(utils::Vec2i center, float radius, utils::Color color);
        void draw_filled_circle(utils::Vec2i center, float radius, utils::Color color);
        void draw_fps(utils::Vec2i position);
        void draw_line(utils::Vec2i start, utils::Vec2i end, utils::Color color);
    };
} // namespace gfx
