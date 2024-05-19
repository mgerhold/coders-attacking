#include <gfx/font.hpp>
#include <raylib.h>

namespace gfx {
    void Font::Deleter::operator()(::Font const* const handle) const {
        UnloadFont(*handle);
        delete handle;
    }

    // clang-format off
    [[nodiscard]] utils::Vec2f Font::measure_text(
        char const* const text,
        float const font_size
    ) const { // clang-format on
        auto const size = MeasureTextEx(*m_handle, text, font_size, 0.0f);
        return utils::Vec2f{ size.x, size.y };
    }
} // namespace gfx
