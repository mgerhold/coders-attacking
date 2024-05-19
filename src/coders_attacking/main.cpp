#include "scene_stack.hpp"
#include "scenes/test_scene.hpp"
#include <gfx/font.hpp>
#include <gfx/window.hpp>

int main() {
    using namespace utils;

    auto window = gfx::Window{
        { 800, 600 },
        "coders attacking"
    };
    window.set_resizable(true);

    auto roboto = window.load_font("assets/fonts/Roboto/Roboto-Regular.ttf", 40);
    auto const font = std::make_shared<gfx::Font>(std::move(roboto).value());

    auto scene_stack = SceneStack{ window };
    scene_stack.emplace<TestScene>(font);

    while (not scene_stack.is_empty() and not window.should_close()) {
        window.poll_events();
        if (window.was_resized()) {
            scene_stack.recalculate_layout(window.area());
        }
        while (auto const event = window.next_event()) {
            scene_stack.handle_event(event.value());
        }
        scene_stack.update(window.event_system(), window.delta_seconds());
        auto renderer = window.renderer();
        scene_stack.render(renderer);
    }
}
