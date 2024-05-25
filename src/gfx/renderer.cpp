#include <gfx/renderer.hpp>
#include <raylib.h>

[[nodiscard]] static Color to_raylib(utils::Color const color) {
    return Color{ color.r, color.g, color.b, color.a };
}

[[nodiscard]] static Rectangle to_raylib(utils::FloatRect const& rect) {
    return Rectangle{ rect.top_left.x, rect.top_left.y, rect.size.x, rect.size.y };
}

namespace gfx {

    void Renderer::clear(utils::Color const color) {
        ClearBackground(to_raylib(color));
    }

    void Renderer::draw_text(
            Font const& font,
            char const* text,
            utils::Vec2i const position,
            float const size,
            utils::Color const color
    ) {
        DrawTextEx(
                *font.m_handle,
                text,
                ::Vector2{ static_cast<float>(position.x), static_cast<float>(position.y) },
                size,
                0.0f,
                to_raylib(color)
        );
    }

    void Renderer::draw_rectangle_outline(utils::IntRect const& area, utils::Color const color) {
        DrawRectangleLines(area.top_left.x, area.top_left.y, area.size.x, area.size.y, to_raylib(color));
    }

    void Renderer::draw_filled_rectangle(utils::IntRect const& area, utils::Color const color) {
        DrawRectangle(area.top_left.x, area.top_left.y, area.size.x, area.size.y, to_raylib(color));
    }

    // clang-format off
    void Renderer::draw_filled_rounded_rectangle(
        utils::IntRect const& area,
        float const roundness,
        int const segments,
        utils::Color const color
    ) { // clang-format on
        DrawRectangleRounded(to_raylib(utils::FloatRect{ area }), roundness, segments, to_raylib(color));
    }

    // clang-format off
    void Renderer::draw_rounded_rectangle_outline(
            utils::IntRect const& area,
            float const roundness,
            int const segments,
            float const line_thickness,
            utils::Color const color
    ) { // clang-format on
        DrawRectangleRoundedLines(
                to_raylib(utils::FloatRect{ area }),
                roundness,
                segments,
                line_thickness,
                to_raylib(color)
        );
    }

    void Renderer::draw_circle(utils::Vec2i const center, float const radius, utils::Color const color) {
        DrawCircleLines(center.x, center.y, radius, to_raylib(color));
    }

    void Renderer::draw_filled_circle(utils::Vec2i const center, float const radius, utils::Color const color) {
        DrawCircle(center.x, center.y, radius, to_raylib(color));
    }

    void Renderer::draw_fps(utils::Vec2i const position) {
        DrawFPS(position.x, position.y);
    }

    void Renderer::draw_line(utils::Vec2i const start, utils::Vec2i const end, utils::Color const color) {
        DrawLine(start.x, start.y, end.x, end.y, to_raylib(color));
    }
} // namespace gfx
