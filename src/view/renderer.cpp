#include <raylib.h>
#include <view/renderer.hpp>

[[nodiscard]] static Color to_raylib(utils::Color const color) {
    return Color{ color.r, color.g, color.b, color.a };
}

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
