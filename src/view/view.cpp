#include <spdlog/spdlog.h>
#include <view/view.hpp>

void view::render_game(
        Galaxy const& galaxy,
        utils::IntRect const& viewport,
        gfx::Renderer& renderer,
        gfx::Font const& font
) {
    using namespace utils;

    renderer.draw_filled_rectangle(viewport, utils::Color::Black);

    auto const& settings = galaxy.game_settings();

    for (auto const& game_object : galaxy.game_objects()) {
        if (auto const planet = game_object.get_component<Planet>()) {
            auto const transform = game_object.get_component<Transform>();
            auto const relative_position = Vec2f{
                transform->position.x / settings.map_size.x,
                transform->position.y / settings.map_size.y,
            };
            auto const absolute_position =
                    Vec2i{ FloatRect{ viewport }.relative_to_absolute_position(relative_position) };
            renderer.draw_circle(absolute_position, 5.0f, planet->color);
            renderer.draw_text(font, planet->name.c_str(), absolute_position + Vec2i{ 7, 0 }, 20, Color::White);
        }
    }
}
