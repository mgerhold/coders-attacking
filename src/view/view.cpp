#include <cmath>
#include <lib2k/random.hpp>
#include <numbers>
#include <ranges>
#include <spdlog/spdlog.h>
#include <view/view.hpp>

namespace view {
    View::View() {
        static constexpr auto num_background_stars = usize{ 1500 * 3 * 3 };
        auto random = c2k::Random{};
        for ([[maybe_unused]] auto const i : std::views::iota(usize{ 0 }, num_background_stars)) {
            auto const position =
                    utils::Vec2f{ (random.next_float() - 0.5f) * 3.0f, (random.next_float() - 0.5f) * 3.0f };
            auto const size = random.next_float() * 2.0f;
            auto const brightness = random.next_float() * 0.5f + 0.1f;
            auto const period = random.next_float() * 10.0f + 5.0f;
            auto const amplitude = random.next_float() * 0.4f;
            auto const distance = random.next_float() * 10.0f + 0.1f;
            auto const planet_type = random.next_integral<int>(3);
            auto const color = std::invoke([&] {
                switch (planet_type) {
                    case 0:
                        // yellow
                        return utils::Color{
                            static_cast<u8>(random.next_integral(240, 256)),
                            static_cast<u8>(random.next_integral(240, 256)),
                            static_cast<u8>(random.next_integral(220, 240)),
                        };
                    case 1:
                        // blue
                        return utils::Color{
                            static_cast<u8>(random.next_integral(220, 240)),
                            static_cast<u8>(random.next_integral(220, 240)),
                            static_cast<u8>(random.next_integral(240, 256)),
                        };
                    case 2:
                        // white
                        return utils::Color{
                            static_cast<u8>(random.next_integral(240, 256)),
                            static_cast<u8>(random.next_integral(240, 256)),
                            static_cast<u8>(random.next_integral(240, 256)),
                        };
                }
                assert(false and "unreachable");
                return utils::Color{ utils::Color::Red };
            });
            m_background_stars.emplace_back(position, size, brightness, period, amplitude, distance, color);
        }
    }

    void View::render_game(
            Galaxy const& galaxy,
            utils::IntRect const& viewport,
            gfx::Renderer& renderer,
            gfx::Font const& font
    ) const {
        using namespace utils;

        static constexpr auto pi = std::numbers::pi_v<float>;

        renderer.draw_filled_rectangle(viewport, Color::Black);

        for (auto const& star : m_background_stars) {
            auto relative_position = star.position - m_offset / star.distance;
            auto const brightness = std::clamp(
                    std::sin(m_elapsed_time * 2.0f * pi / star.period) * star.amplitude + star.base_brightness,
                    0.0f,
                    1.0f
            );
            auto const color = Color{
                static_cast<u8>(static_cast<float>(star.color.r) * brightness),
                static_cast<u8>(static_cast<float>(star.color.g) * brightness),
                static_cast<u8>(static_cast<float>(star.color.b) * brightness),
            };
            auto const absolute_position =
                    Vec2i{ FloatRect{ viewport }.relative_to_absolute_position(relative_position * m_zoom) };
            if (not viewport.contains(absolute_position)) {
                continue;
            }
            renderer.draw_circle(absolute_position, star.size, color);
        }

        auto const& settings = galaxy.game_settings();

        for (auto const& game_object : galaxy.game_objects()) {
            if (auto const planet = game_object.get_component<Planet>()) {
                auto const transform = game_object.get_component<Transform>();
                auto const relative_position = Vec2f{
                    transform->position.x / settings.map_size.x,
                    transform->position.y / settings.map_size.y,
                } * m_zoom - m_offset;
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
            m_zoom *= 1.0f + mouse_wheel_moved->delta.y * 0.1f;
            return ui::HandleEventResult::EventHandled;
        }
        return ui::HandleEventResult::EventNotHandled;
    }

    void View::update(ui::EventSystem const& event_system, float const delta_seconds) {
        static constexpr auto scroll_speed = 0.1f;
        static constexpr auto zoom_speed = 0.1f;
        if (event_system.is_key_down(ui::Key::Left)) {
            m_offset.x -= scroll_speed * delta_seconds;
        }
        if (event_system.is_key_down(ui::Key::Right)) {
            m_offset.x += scroll_speed * delta_seconds;
        }
        if (event_system.is_key_down(ui::Key::Up)) {
            m_offset.y -= scroll_speed * delta_seconds;
        }
        if (event_system.is_key_down(ui::Key::Down)) {
            m_offset.y += scroll_speed * delta_seconds;
        }
        if (event_system.is_key_down(ui::Key::KpAdd)) {
            m_zoom += zoom_speed * delta_seconds;
        }
        if (event_system.is_key_down(ui::Key::KpSubtract)) {
            m_zoom -= zoom_speed * delta_seconds;
        }
        m_elapsed_time += delta_seconds;
    }
} // namespace view
