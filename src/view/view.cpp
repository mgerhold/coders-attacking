#include <view/view.hpp>

void view::render_game(Galaxy const&, utils::IntRect const& viewport, gfx::Renderer& renderer) {
    renderer.draw_filled_rectangle(viewport, utils::Color::Black);
}
