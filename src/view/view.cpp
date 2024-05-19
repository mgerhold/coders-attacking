#include <cmath>
#include <lib2k/random.hpp>
#include <numbers>
#include <ranges>
#include <spdlog/spdlog.h>
#include <view/view.hpp>

namespace view {
    View::View() {
        static constexpr auto num_background_stars = usize{ 1500 };
        auto random = c2k::Random{};
        for ([[maybe_unused]] auto const i : std::views::iota(usize{ 0 }, num_background_stars)) {
            auto const position = utils::Vec2f{ random.next_float(), random.next_float() };
            auto const size = random.next_float() * 2.0f;
            auto const brightness = random.next_float() * 0.5f + 0.1f;
            auto const period = random.next_float() * 10.0f + 5.0f;
            auto const amplitude = random.next_float() * 0.4f;
            m_background_stars.emplace_back(position, size, brightness, period, amplitude);
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
            auto const brightness = std::clamp(
                    std::sin(m_elapsed_time * 2.0f * pi / star.period) * star.amplitude + star.base_brightness,
                    0.0f,
                    1.0f
            );
            auto const color = Color{
                static_cast<u8>(255.0f * brightness),
                static_cast<u8>(255.0f * brightness),
                static_cast<u8>(255.0f * brightness),
            };
            renderer.draw_circle(
                    Vec2i{ FloatRect{ viewport }.relative_to_absolute_position(star.position) },
                    star.size,
                    color
            );
        }

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

    void View::update(float const delta_seconds) {
        m_elapsed_time += delta_seconds;
    }
} // namespace view
