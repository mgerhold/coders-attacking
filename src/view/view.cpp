#include <cmath>
#include <common/resource_manager.hpp>
#include <gfx/window.hpp>
#include <lib2k/random.hpp>
#include <numbers>
#include <ranges>
#include <spdlog/spdlog.h>
#include <view/background_star.hpp>
#include <view/view.hpp>

namespace view {
    using namespace utils;

    View::View(
            ServiceProvider& service_provider,
            GameObject const& current_player,
            c2k::Random::Seed background_stars_seed
    )
        : m_current_player{ &current_player },
          m_camera{
              0.9f,
              10.0f,
              service_provider.window().area(),
              FloatRect::unit().move({ -0.5f, -0.5f }).scaled_from_center(1.4f),
          },
          m_service_provider{ &service_provider } {
        regenerate_background_stars(background_stars_seed);
    }

    void View::render_game(Galaxy const& galaxy, gfx::Renderer& renderer) const {
        using namespace utils;

        static constexpr auto min_radio_radius = 5.0;
        auto const max_radio_radius = m_camera.world_to_view_coords(Vec2f{ 1.0f, 1.0f } * 100.0f).magnitude();

        auto const first_player_it = std::ranges::find_if(galaxy.game_objects(), [](GameObject const& game_object) {
            return game_object.get_component<Player>().has_value();
        });
        auto const current_player_uuid = first_player_it->uuid();

        auto const& font = m_service_provider->resource_manager().font(FontType::Roboto);

        // clear background
        renderer.draw_filled_rectangle(m_camera.viewport(), Color::Black);

        // draw current command selection
        if (m_selection_start.has_value()) {
            auto const start_position =
                    m_camera.world_to_screen_coords(m_selection_start->get_component<Transform>().value().position);
            auto const end_position = m_service_provider->window().event_system().mouse_position();
            renderer.draw_line(start_position, end_position, Color{ 173, 216, 230 });
        }

        for (auto const& star : m_background_stars) {
            star.render(renderer, m_camera, m_service_provider->window().elapsed_seconds());
        }

        for (auto const& game_object : galaxy.game_objects()) {
            if (auto const planet = game_object.get_component<Planet>()) {
                auto const transform = game_object.get_component<Transform>();
                auto const visibility = determine_visibility(0.2f, galaxy, current_player_uuid, transform->position);
                if (not visibility.has_value()) {
                    continue;
                }
                auto const view_coords = m_camera.world_to_view_coords(transform->position);
                auto const screen_coords = m_camera.view_to_screen_coords(view_coords);

                if (auto const ownership = game_object.get_component<Ownership>()) {
                    auto const owner_uuid = ownership->owner;
                    static constexpr auto num_radio_circles = 6;
                    static constexpr auto radio_animation_speed = 0.1;
                    for (auto i = 0; i < num_radio_circles; ++i) {
                        auto const player = galaxy.find_game_object(owner_uuid).value().get_component<Player>();
                        auto const time = m_service_provider->window().elapsed_seconds() / (1.0 / radio_animation_speed)
                                          + static_cast<double>(i) * 1.0 / num_radio_circles;
                        auto const relative = std::abs(time - std::floor(time));
                        auto const radius = static_cast<float>(
                                min_radio_radius + relative * (static_cast<double>(max_radio_radius) - min_radio_radius)
                        );
                        renderer.draw_circle(
                                screen_coords,
                                radius,
                                Color{
                                        player->color.r,
                                        player->color.g,
                                        player->color.b,
                                        static_cast<u8>(255.0 * (1.0 - relative)),
                                }
                        );
                    }
                }

                renderer.draw_filled_circle(screen_coords, 5.0f, planet->color);

                if ((m_focused_planet.has_value() and m_focused_planet == game_object)
                    or (m_selection_start.has_value() and m_selection_start.value() == game_object)) {
                    renderer.draw_circle(screen_coords, 9.0f, planet->color);
                }

                auto const text = std::invoke([&] {
                    auto result = planet->name;
                    if (auto const fleet = game_object.get_component<Fleet>()) {
                        result += std::format(" ({} ships)", fleet->count);
                    }
                    return result;
                });

                static constexpr auto font_size = 20;
                auto text_size = Vec2i{ font.measure_text(text.c_str(), font_size) };
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
                renderer.draw_text(font, text.c_str(), Vec2i{ x, y }, font_size, Color::White);
            }
        }
    }

    // clang-format off
    [[nodiscard]] ui::HandleEventResult View::handle_event(
        ui::Event const& event
    ) { // clang-format on
        auto const& event_system = m_service_provider->window().event_system();
        if (auto const mouse_wheel_moved = std::get_if<ui::MouseWheelMoved>(&event)) {
            if (m_camera.viewport().contains(event_system.mouse_position())) {
                m_camera.zoom_towards(1.0f + mouse_wheel_moved->delta.y * 0.1f, event_system.mouse_position());
                return ui::HandleEventResult::EventHandled;
            }
        }
        if (auto const mouse_clicked = std::get_if<ui::MouseClicked>(&event)) {
            if (mouse_clicked->button == ui::MouseButton::Left and m_focused_planet.has_value()) {
                if (auto const ownership = m_focused_planet->get_component<Ownership>();
                    ownership.has_value() and ownership->owner == m_current_player->uuid()) {
                    m_selection_start = m_focused_planet.value();
                }
            }
        } else if (auto const mouse_released = std::get_if<ui::MouseReleased>(&event)) {
            if (mouse_released->button == ui::MouseButton::Left) {
                if (m_focused_planet.has_value() and m_selection_start.has_value()
                    and m_focused_planet.value() != m_selection_start.value()) {
                    m_selection_end = m_focused_planet;
                } else {
                    m_selection_start = tl::nullopt;
                }
            }
        }
        if (auto const key_pressed = std::get_if<ui::KeyPressed>(&event)) {
            if (key_pressed->key == ui::Key::Space) {
                m_camera.reset_zoom();
            }
        }
        return ui::HandleEventResult::EventNotHandled;
    }

    void View::update(Galaxy const& galaxy) {
        auto const& event_system = m_service_provider->window().event_system();
        auto const delta_seconds = m_service_provider->window().delta_seconds();
        static constexpr auto scroll_speed = 0.1f;
        static constexpr auto zoom_factor = 1.2f;
        m_camera.update(*m_service_provider);
        if (event_system.is_key_down(ui::Key::Left)) {
            m_camera.move({ -scroll_speed * static_cast<float>(delta_seconds), 0.0f });
        }
        if (event_system.is_key_down(ui::Key::Right)) {
            m_camera.move({ scroll_speed * static_cast<float>(delta_seconds), 0.0f });
        }
        if (event_system.is_key_down(ui::Key::Up)) {
            m_camera.move({ 0.0f, -scroll_speed * static_cast<float>(delta_seconds) });
        }
        if (event_system.is_key_down(ui::Key::Down)) {
            m_camera.move({ 0.0f, scroll_speed * static_cast<float>(delta_seconds) });
        }
        if (event_system.is_key_down(ui::Key::KpAdd)) {
            m_camera.zoom((zoom_factor - 1.0f) * static_cast<float>(delta_seconds) + 1.0f);
        }
        if (event_system.is_key_down(ui::Key::KpSubtract)) {
            m_camera.zoom(-(zoom_factor - 1.0f) * static_cast<float>(delta_seconds) + 1.0f);
        }
        if (event_system.is_mouse_button_down(ui::MouseButton::Right)
            and m_camera.viewport().contains(event_system.mouse_position())) {
            auto const size = Vec2f{ m_camera.viewport().size };
            auto const offset =
                    Vec2f{ event_system.mouse_delta() }.hadamard_product(Vec2f{ 1.0f / size.x, 1.0f / size.y });
            m_camera.move(-offset);
        }
        auto const mouse_position = Vec2f{ event_system.mouse_position() };
        auto a_planet_is_focused = false;
        for (auto const& game_object : galaxy.game_objects()) {
            if ([[maybe_unused]] auto const planet = game_object.get_component<Planet>()) {
                auto const& transform = game_object.get_component<Transform>();
                auto const screen_coords = Vec2f{ m_camera.world_to_screen_coords(transform->position) };
                assert(transform.has_value());
                auto const distance = (mouse_position - screen_coords).magnitude();
                if (distance <= 20.0f) {
                    m_focused_planet = game_object;
                    a_planet_is_focused = true;
                }
            }
        }
        if (not a_planet_is_focused) {
            m_focused_planet = tl::nullopt;
        }
    }

    void View::regenerate_background_stars(c2k::Random::Seed const seed) {
        m_background_stars.clear();
        auto random = c2k::Random{ seed };
        for ([[maybe_unused]] auto const i : std::views::iota(usize{ 0 }, num_background_stars)) {
            m_background_stars.emplace_back(BackgroundStar::random(random));
        }
    }

    [[nodiscard]] tl::optional<std::tuple<uuids::uuid, uuids::uuid>> View::pop_selection() {
        if (m_selection_start.has_value() and m_selection_end.has_value()) {
            auto result = std::tuple{ m_selection_start->uuid(), m_selection_end->uuid() };
            m_selection_start = tl::nullopt;
            m_selection_end = tl::nullopt;
            return result;
        }
        return tl::nullopt;
    }

    [[nodiscard]] GameObject const& View::current_player() const {
        return *m_current_player;
    }

    [[nodiscard]] tl::optional<float> View::determine_visibility(
            float const max_radius,
            Galaxy const& galaxy,
            uuids::uuid const current_player_uuid,
            Vec2f world_position
    ) const {
        float min_distance_to_owned_planet = std::numeric_limits<float>::max();
        for (auto const& game_object : galaxy.game_objects()) {
            if (auto const components = game_object.get_components<Transform, Planet, Ownership>()) {
                auto const& [transform, planet, ownership] = components.value();
                if (ownership.owner != current_player_uuid) {
                    continue;
                }
                auto const distance = (transform.position - world_position).magnitude();
                if (distance < min_distance_to_owned_planet) {
                    min_distance_to_owned_planet = distance;
                }
            }
        }
        if (min_distance_to_owned_planet > max_radius) {
            return tl::nullopt;
        }
        auto const brightness = (min_distance_to_owned_planet <= 0.5f * max_radius)
                                        ? 1.0f
                                        : 1.0f - (min_distance_to_owned_planet / (0.5f * max_radius) - 1.0f);
        return brightness;
    }
} // namespace view
