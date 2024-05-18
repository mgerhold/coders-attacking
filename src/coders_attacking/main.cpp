#include <iostream>
#include <spdlog/spdlog.h>
#include <view/window.hpp>

int main() {
    using utils::Color;
    using utils::Vec2i;
    auto window = Window{ 800, 600, "coders attacking" };
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
            } else if (auto const mouse_moved = get_if<ui::MouseMoved>(&*event)) {
                spdlog::info(
                        "mouse moved by ({},{}) to ({},{})",
                        mouse_moved->delta.x,
                        mouse_moved->delta.y,
                        mouse_moved->position.x,
                        mouse_moved->position.y
                );
            } else if (auto const mouse_wheel_moved = get_if<ui::MouseWheelMoved>(&*event)) {
                spdlog::info("mouse wheel moved by ({},{})", mouse_wheel_moved->delta.x, mouse_wheel_moved->delta.y);
            }
        }
        auto renderer = window.renderer();
        renderer.clear(Color::Beige);
        renderer.draw_text("Hello, dear folks on Twitch!", Vec2i{ 190, 200 }, 30, Color::Magenta);
    }
}
