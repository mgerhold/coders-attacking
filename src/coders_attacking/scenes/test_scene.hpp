#pragma once

#include "../scene.hpp"
#include <algorithm>
#include <array>
#include <game/galaxy.hpp>
#include <ui/button.hpp>
#include <ui/grid_layout.hpp>
#include <ui/panel.hpp>
#include <view/view.hpp>

class TestScene final : public Scene {
private:
    Galaxy m_galaxy;
    std::shared_ptr<gfx::Font> m_font;
    view::View m_game_view;
    bool m_running{ true };
    ui::Label* m_focused_planet_label;

public:
    TestScene(SceneManager& scene_manager, std::shared_ptr<gfx::Font> font)
        : Scene{ scene_manager, create_user_interface(font) },
          m_galaxy{ create_galaxy() },
          m_font{ std::move(font) },
          m_game_view{ game_viewport() } {
        auto& exit_button = dynamic_cast<ui::Button&>(m_user_interface->find_widget_by_name("button_exit").value());
        exit_button.on_click([&](ui::Button&) { m_running = false; });
        m_focused_planet_label =
                &dynamic_cast<ui::Label&>(m_user_interface->find_widget_by_name("label_focused_planet").value());
    }

    TestScene(TestScene const& other) = delete;
    TestScene(TestScene&& other) noexcept = delete;
    TestScene& operator=(TestScene const& other) = delete;
    TestScene& operator=(TestScene&& other) noexcept = delete;
    ~TestScene() override = default;

    [[nodiscard]] UpdateResult update(ui::EventSystem const& event_system, float const delta_seconds) override {
        if (not m_running) {
            end_scene();
        }
        m_game_view.update(m_galaxy, event_system, delta_seconds);
        m_focused_planet_label->caption(
                m_game_view.focused_planet().has_value() ? m_game_view.focused_planet()->get_component<Planet>()->name
                                                         : ""
        );
        return UpdateResult::KeepUpdating;
    }

    [[nodiscard]] ui::HandleEventResult handle_event(ui::Event const& event, ui::EventSystem const& event_system)
            override {
        if (Scene::handle_event(event, event_system) == ui::HandleEventResult::EventHandled) {
            return ui::HandleEventResult::EventHandled;
        }
        return m_game_view.handle_event(event, event_system);
    }

    void render(gfx::Renderer& renderer) const override {
        m_game_view.render_game(m_galaxy, renderer, *m_font);
        Scene::render(renderer);
    }

    void on_window_resized(utils::IntRect const area) override {
        Scene::on_window_resized(area);
        m_game_view.on_window_resized(game_viewport());
    }

private:
    [[nodiscard]] utils::IntRect game_viewport() {
        auto const size = viewport().size;
        return utils::IntRect{
            utils::Vec2i{      size.x / 16,                0 },
            utils::Vec2i{ size.x * 15 / 16, size.y * 11 / 12 },
        };
    }

    [[nodiscard]] static std::unique_ptr<ui::Widget> create_user_interface(std::shared_ptr<gfx::Font> const& font) {
        // clang-format off
        auto layout = std::make_unique<ui::GridLayout>(
                16,
                12,
                ui::GridLayout::Area{{0,0},{1,12}},
                ui::GridLayout::Area{{1,11},{15,1}},
                ui::GridLayout::Area{ { 14, 11 }, { 2, 1 } },
                ui::GridLayout::Area{ { 1, 11 }, { 5, 1 } }
            );
        // clang-format on
        auto user_interface = std::make_unique<ui::Panel>(std::move(layout));
        auto const frame_color = utils::Color{ 54, 59, 79 };
        auto focused_planet_label = std::make_unique<ui::Label>(
                "",
                font,
                40,
                utils::Color::White,
                ui::Alignment::Left,
                ui::VerticalAlignment::Middle
        );
        focused_planet_label->name("label_focused_planet");
        auto exit_button = std::make_unique<ui::Button>("Exit", 0, font);
        exit_button->name("button_exit");
        user_interface->add_widgets(
                std::make_unique<ui::Panel>(frame_color),
                std::make_unique<ui::Panel>(frame_color),
                std::move(exit_button),
                std::move(focused_planet_label)
        );
        return user_interface;
    }

    [[nodiscard]] static Galaxy create_galaxy() {
        static constexpr auto planet_names = std::array{
            "Mercury",
            "Venus",
            "Earth",
            "Mars",
            "Jupiter",
            "Saturn",
            "Uranus",
            "Neptune",
            "Proxima Centauri b",
            "Kepler-186f",
            "TRAPPIST-1e",
            "Kepler-452b",
            "GJ 1214 b",
            "HD 209458 b",
            "55 Cancri e",
            "WASP-12b",
            "Kepler-22b",
            "Gliese 581d",
            "Kepler-69c",
            "Kepler-62f",
            "Kepler-442b",
            "Kepler-1649c",
            "LHS 1140 b",
            "HD 40307 g",
            "Tau Ceti e",
            "Ross 128 b",
            "Wolf 1061 c",
            "Luyten b",
            "K2-18b",
            "GJ 667 Cc",
            "Teegarden's Star b",
            "KELT-9b",
            "GJ 357 d",
            "HD 189733 b",
            "55 Cancri f",
            "Kepler-62e",
            "Kepler-1229b",
            "Kepler-1652b",
            "Kepler-442b",
            "Kepler-62c",
            "Kepler-61b",
            "Kapteyn b",
            "K2-141b",
            "GJ 876 d",
            "HD 85512 b",
            "EPIC 201238110.02",
            "Kepler-22c",
            "Kepler-37b",
            "GJ 3293 d",
            "HD 147379 b",
            "K2-72e",
            "HD 95086 b",
            "HIP 65426 b",
            "HR 8799 b",
        };

        auto galaxy = Galaxy{};
        auto random = c2k::Random{};
        auto shuffled_planet_names = std::vector<std::string>{ planet_names.begin(), planet_names.end() };
        std::shuffle(
                shuffled_planet_names.begin(),
                shuffled_planet_names.end(),
                std::mt19937{ std::random_device{}() }
        );
        for (auto i = usize{ 0 }; i < galaxy.game_settings().num_planets; ++i) {
            auto const position = utils::Vec2f{
                random.next_float() - 0.5f,
                random.next_float() - 0.5f,
            };
            auto game_object = GameObject{};
            game_object.emplace_component<Transform>(position);
            auto const color = utils::Color::random(random);
            assert(i < shuffled_planet_names.size());
            auto planet_name = shuffled_planet_names.at(i);
            game_object.emplace_component<Planet>(std::move(planet_name), 10, color);
            galaxy.game_objects().push_back(game_object);
        }
        return galaxy;
    }
};
