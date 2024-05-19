#include "ui/bumper.hpp"
#include "ui/button.hpp"
#include "ui/column_layout.hpp"
#include "ui/grid_layout.hpp"
#include "ui/ingame_view.hpp"
#include "ui/label.hpp"
#include "ui/panel.hpp"
#include <gfx/font.hpp>
#include <gfx/window.hpp>
#include <lib2k/random.hpp>
#include <spdlog/spdlog.h>

[[nodiscard]] Galaxy create_galaxy() {
    auto galaxy = Galaxy{};
    auto random = c2k::Random{};
    for (auto i = usize{ 0 }; i < galaxy.game_settings().num_planets; ++i) {
        auto const position = utils::Vec2f{
            random.next_float() * galaxy.game_settings().map_size.x,
            random.next_float() * galaxy.game_settings().map_size.y,
        };
        auto game_object = GameObject{};
        game_object.emplace_component<Transform>(position);
        auto const color = utils::Color::random(random);
        spdlog::info("{},{},{},{}", color.r, color.g, color.b, color.a);
        game_object.emplace_component<Planet>("Earth", 10, color);
        galaxy.game_objects().push_back(game_object);
    }
    return galaxy;
}

int main() {
    using utils::Color;
    using utils::Vec2i;
    auto window = gfx::Window{
        { 800, 600 },
        "coders attacking"
    };
    window.set_resizable(true);
    auto const window_area = utils::IntRect{ Vec2i{}, window.size() };

    auto roboto = window.load_font("assets/fonts/Roboto/Roboto-Regular.ttf", 40);
    auto const font = std::make_shared<gfx::Font>(std::move(roboto).value());

    auto const galaxy = create_galaxy();

    auto user_interface_root = ui::Panel{
        std::make_unique<ui::GridLayout>(
                7,
                8,
                ui::GridLayout::Area{ { 0, 0 }, { 7, 5 } },
                ui::GridLayout::Area{ { 1, 6 }, { 2, 1 } },
                ui::GridLayout::Area{ { 4, 6 }, { 2, 1 } }
        ),
        Color::Brown,
    };
    auto running = true;
    user_interface_root.add_widgets(
            std::make_unique<ui::IngameView>(galaxy, *font),
            std::make_unique<ui::Button>("OK", font, [](auto const&) { spdlog::info("OK"); }),
            std::make_unique<ui::Button>("Cancel", font, [&running](auto const&) { running = false; })
    );
    user_interface_root.recalculate_absolute_area(window_area);

    while (running and not window.should_close()) {
        window.poll_events();
        if (window.was_resized()) {
            user_interface_root.recalculate_absolute_area(window.area());
        }
        while (auto const event = window.next_event()) {
            std::ignore = user_interface_root.handle_event(event.value());
        }
        auto renderer = window.renderer();
        renderer.clear(Color::Beige);
        user_interface_root.render(renderer);
    }
}
