#include <raylib.h>
#include <gfx/renderer.hpp>

[[nodiscard]] static Color to_raylib(utils::Color const color) {
    return Color{ color.r, color.g, color.b, color.a };
}

namespace gfx {
    void Renderer::Deleter::operator()(std::monostate) const {
        EndDrawing();
    }

    Renderer::Renderer() : m_handle{ std::monostate{} } {
        BeginDrawing();
    }

    void Renderer::clear(utils::Color const color) {
        ClearBackground(to_raylib(color));
    }

    // clang-format off
    void Renderer::draw_text(
        char const* const text,
        utils::Vec2i const position,
        int const size,
        utils::Color const color
    ) { // clang-format on
        DrawText(text, position.x, position.y, size, to_raylib(color));
    }

    void Renderer::draw_filled_rectangle(utils::IntRect const& area, utils::Color const color) {
        DrawRectangle(area.top_left.x, area.top_left.y, area.size.x, area.size.y, to_raylib(color));
    }
} // namespace gfx
