#include "ui/bumper.hpp"
#include "ui/button.hpp"
#include "ui/column_layout.hpp"
#include "ui/grid_layout.hpp"
#include "ui/label.hpp"
#include "ui/panel.hpp"
#include <gfx/font.hpp>
#include <gfx/window.hpp>
#include <spdlog/spdlog.h>

int main() {
    using utils::Color;
    using utils::Vec2i;
    auto window = gfx::Window{ 800, 600, "coders attacking" };
    window.set_resizable(true);
    auto const window_area = utils::IntRect{ Vec2i{}, window.size() };

    auto roboto = window.load_font("assets/fonts/Roboto/Roboto-Regular.ttf", 40);
    auto const font = std::make_shared<gfx::Font>(std::move(roboto).value());

    auto user_interface_root = ui::Panel{
        std::make_unique<ui::GridLayout>(3, 4),
        Color::Brown,
    };
    auto const on_click_handler = [](ui::Button const& button) {
        spdlog::info("button with text \"{}\" has been clicked", button.caption());
    };
    user_interface_root.add_widgets<5>(
            std::make_unique<ui::Button>("OK", font, on_click_handler),
            std::make_unique<ui::Button>("Cancel", font, on_click_handler)
    );
    user_interface_root.recalculate_absolute_area(window_area);

    while (not window.should_close()) {
        window.poll_events();
        if (window.was_resized()) {
            user_interface_root.recalculate_absolute_area(window.area());
        }
        while (auto const event = window.next_event()) {
            std::ignore = user_interface_root.handle_event(event.value());
            if (auto const key_pressed = get_if<ui::KeyPressed>(&*event)) {
                spdlog::info("key pressed: {}", to_string(key_pressed->key));
            } else if (auto const key_released = get_if<ui::KeyReleased>(&*event)) {
                spdlog::info("key released: {}", to_string(key_released->key));
            } else if (auto const mouse_wheel_moved = get_if<ui::MouseWheelMoved>(&*event)) {
                spdlog::info("mouse wheel moved by ({},{})", mouse_wheel_moved->delta.x, mouse_wheel_moved->delta.y);
            }
        }
        auto renderer = window.renderer();
        renderer.clear(Color::Beige);
        user_interface_root.render(renderer);
    }
}
