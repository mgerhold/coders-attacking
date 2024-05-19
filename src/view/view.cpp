#include <spdlog/spdlog.h>
#include <view/view.hpp>

namespace view {
    void View::render_game(
            Galaxy const& galaxy,
            utils::IntRect const& viewport,
            gfx::Renderer& renderer,
            gfx::Font const& font
    ) const {
        using namespace utils;

        renderer.draw_filled_rectangle(viewport, Color::Black);

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

                static constexpr auto font_size = 20;
                auto text_size = Vec2i{ font.measure_text(planet->name.c_str(), font_size) };
                auto const x = std::invoke([&] {
                    if (relative_position.x < 0.5f) {
                        return absolute_position.x + 7;
                    }
                    return absolute_position.x - 7 - text_size.x;
                });
                auto const y = std::invoke([&] {
                    if (relative_position.y < 0.5f) {
                        return absolute_position.y;
                    }
                    return absolute_position.y - text_size.y;
                });
                renderer.draw_text(font, planet->name.c_str(), Vec2i{ x, y }, font_size, Color::White);
            }
        }
    }

    [[nodiscard]] ui::HandleEventResult View::handle_event(ui::Event const& event) {
        if (auto const mouse_wheel_moved = std::get_if<ui::MouseWheelMoved>(&event)) {
            spdlog::info("mouse wheel moved: {}", mouse_wheel_moved->delta.y);
            return ui::HandleEventResult::EventHandled;
        }
        return ui::HandleEventResult::EventNotHandled;
    }
} // namespace view
