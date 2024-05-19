#pragma once

#include "gfx/renderer.hpp"
#include "utils/rect.hpp"
#include <game/galaxy.hpp>

namespace view {
    // clang-format off
    void render_game(
        Galaxy const& galaxy,
        utils::IntRect const& viewport,
        gfx::Renderer& renderer,
        gfx::Font const& font
    );
    // clang-format on
} // namespace view
