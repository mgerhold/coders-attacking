#include "ui/bumper.hpp"
#include "ui/button.hpp"
#include "ui/column_layout.hpp"
#include "ui/grid_layout.hpp"
#include "ui/label.hpp"
#include "ui/panel.hpp"
#include <gfx/font.hpp>
#include <gfx/window.hpp>
#include <spdlog/spdlog.h>

static constexpr auto lorem_ipsum =
        "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et "
        "dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet "
        "clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, "
        "consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, "
        "sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea "
        "takimata sanctus est Lorem ipsum dolor sit amet.";

int main() {
    using utils::Color;
    using utils::Vec2i;
    auto window = gfx::Window{ 800, 600, "coders attacking" };
    window.set_resizable(true);
    auto const window_area = utils::IntRect{ Vec2i{}, window.size() };

    auto roboto = window.load_font("assets/fonts/Roboto/Roboto-Regular.ttf", 100);
    auto const font = std::make_shared<gfx::Font>(std::move(roboto).value());

    auto panel = ui::Panel{
        std::make_unique<ui::GridLayout>(3, 4),
        Color::Brown,
    };
    panel.add_widgets<5>(
            std::make_unique<ui::Panel>(std::make_unique<ui::ColumnLayout>(3), Color::Gold),
            std::make_unique<ui::Button>("OK", font, [](auto&) {})
    );
    panel.recalculate_absolute_area(window_area);

    while (not window.should_close()) {
        window.poll_events();
        while (auto const event = window.next_event()) {
            if (auto const key_pressed = get_if<ui::KeyPressed>(&*event)) {
                spdlog::info("key pressed: {}", to_string(key_pressed->key));
            } else if (auto const key_released = get_if<ui::KeyReleased>(&*event)) {
                spdlog::info("key released: {}", to_string(key_released->key));
            } else if (auto const mouse_clicked = get_if<ui::MouseClicked>(&*event)) {
                spdlog::info(
                        "mouse button {} clicked at ({},{})",
                        std::to_underlying(mouse_clicked->button),
                        mouse_clicked->position.x,
                        mouse_clicked->position.y
                );
            } else if (auto const mouse_released = get_if<ui::MouseReleased>(&*event)) {
                spdlog::info(
                        "mouse button {} released at ({},{})",
                        std::to_underlying(mouse_released->button),
                        mouse_released->position.x,
                        mouse_released->position.y
                );
            } /*else if (auto const mouse_moved = get_if<ui::MouseMoved>(&*event)) {
                spdlog::info(
                        "mouse moved by ({},{}) to ({},{})",
                        mouse_moved->delta.x,
                        mouse_moved->delta.y,
                        mouse_moved->position.x,
                        mouse_moved->position.y
                );
            }*/
            else if (auto const mouse_wheel_moved = get_if<ui::MouseWheelMoved>(&*event)) {
                spdlog::info("mouse wheel moved by ({},{})", mouse_wheel_moved->delta.x, mouse_wheel_moved->delta.y);
            } else if (auto const window_resized = get_if<ui::WindowResized>(&*event)) {
                panel.recalculate_absolute_area(window_resized->new_area);
            }
        }
        auto renderer = window.renderer();
        renderer.clear(Color::Beige);
        panel.render(renderer);
    }
}
