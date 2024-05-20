#include "view/background_star.hpp"


#include <cmath>
#include <lib2k/random.hpp>
#include <numbers>
#include <ranges>
#include <spdlog/spdlog.h>
#include <view/view.hpp>

namespace view {
    View::View(utils::IntRect const viewport) : m_camera{ 0.9f, 4.0f, viewport } {
        static constexpr auto num_background_stars = usize{ 1500 * 3 };
        auto random = c2k::Random{};
        for ([[maybe_unused]] auto const i : std::views::iota(usize{ 0 }, num_background_stars)) {
            m_background_stars.emplace_back(BackgroundStar::random(random));
        }
    }

    void View::render_game(Galaxy const& galaxy, gfx::Renderer& renderer, gfx::Font const& font) const {
        using namespace utils;

        renderer.draw_filled_rectangle(m_camera.viewport(), Color::Black);

        for (auto const& star : m_background_stars) {
            star.render(renderer, m_camera, m_elapsed_time);
        }

        for (auto const& game_object : galaxy.game_objects()) {
            if (auto const planet = game_object.get_component<Planet>()) {
                auto const transform = game_object.get_component<Transform>();
                auto const view_coords = m_camera.world_to_view_coords(transform->position);
                auto const screen_coords = m_camera.view_to_screen_coords(view_coords);
                renderer.draw_circle(screen_coords, 5.0f, planet->color);

                static constexpr auto font_size = 20;
                auto text_size = Vec2i{ font.measure_text(planet->name.c_str(), font_size) };
                auto const x = std::invoke([&] {
                    if (view_coords.x < 0.5f) {
                        return screen_coords.x + 7;
                    }
                    return screen_coords.x - 7 - text_size.x;
                });
                auto const y = std::invoke([&] {
                    if (view_coords.y < 0.5f) {
                        return screen_coords.y;
                    }
                    return screen_coords.y - text_size.y;
                });
                renderer.draw_text(font, planet->name.c_str(), Vec2i{ x, y }, font_size, Color::White);
            }
        }
    }

    [[nodiscard]] ui::HandleEventResult View::handle_event(ui::Event const& event) {
        if (auto const mouse_wheel_moved = std::get_if<ui::MouseWheelMoved>(&event)) {
            m_camera.zoom(1.0f + mouse_wheel_moved->delta.y * 0.1f);
            return ui::HandleEventResult::EventHandled;
        }
        if (auto const key_pressed = std::get_if<ui::KeyPressed>(&event)) {
            switch (key_pressed->key) { // NOLINT (not all cases handled)
                case ui::Key::Space:
                    m_camera.reset_zoom();
                    break;
            }
        }
        return ui::HandleEventResult::EventNotHandled;
    }

    void View::update(ui::EventSystem const& event_system, float const delta_seconds) {
        static constexpr auto scroll_speed = 0.1f;
        static constexpr auto zoom_factor = 1.2f;
        if (event_system.is_key_down(ui::Key::Left)) {
            m_camera.move({ -scroll_speed * delta_seconds, 0.0f });
        }
        if (event_system.is_key_down(ui::Key::Right)) {
            m_camera.move({ scroll_speed * delta_seconds, 0.0f });
        }
        if (event_system.is_key_down(ui::Key::Up)) {
            m_camera.move({ 0.0f, -scroll_speed * delta_seconds });
        }
        if (event_system.is_key_down(ui::Key::Down)) {
            m_camera.move({ 0.0f, scroll_speed * delta_seconds });
        }
        if (event_system.is_key_down(ui::Key::KpAdd)) {
            m_camera.zoom((zoom_factor - 1.0f) * delta_seconds + 1.0f);
        }
        if (event_system.is_key_down(ui::Key::KpSubtract)) {
            m_camera.zoom(-(zoom_factor - 1.0f) * delta_seconds + 1.0f);
        }
        spdlog::info("mouse position: {},{}", event_system.mouse_position().x, event_system.mouse_position().y);
        auto const view_coords = m_camera.screen_to_view_coords(event_system.mouse_position());
        spdlog::info("view coords: {},{}", view_coords.x, view_coords.y);
        auto const world_coords = m_camera.view_to_world_coords(view_coords);
        spdlog::info("world coords: {},{}", world_coords.x, world_coords.y);
        m_elapsed_time += delta_seconds;
    }
} // namespace view
