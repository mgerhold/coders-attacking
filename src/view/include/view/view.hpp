#pragma once

#include "gfx/renderer.hpp"
#include "utils/rect.hpp"
#include <game/galaxy.hpp>

namespace view {
    void render_game(Galaxy const& galaxy, utils::IntRect const& viewport, gfx::Renderer& renderer);
}
