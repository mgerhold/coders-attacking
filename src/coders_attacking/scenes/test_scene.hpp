#pragma once

#include "../scene.hpp"
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

public:
    TestScene(SceneManager& scene_manager, std::shared_ptr<gfx::Font> font)
        : Scene{ scene_manager },
          m_galaxy{ create_galaxy() },
          m_font{ std::move(font) } {
        setup_user_interface();
    }

    TestScene(TestScene const& other) = delete;
    TestScene(TestScene&& other) noexcept = delete;
    TestScene& operator=(TestScene const& other) = delete;
    TestScene& operator=(TestScene&& other) noexcept = delete;
    ~TestScene() override = default;

    [[nodiscard]] UpdateResult update(float) override {
        if (not m_running) {
            end_scene();
        }
        return UpdateResult::KeepUpdating;
    }

    [[nodiscard]] ui::HandleEventResult handle_event(ui::Event const& event) override {
        if (Scene::handle_event(event) == ui::HandleEventResult::EventHandled) {
            return ui::HandleEventResult::EventHandled;
        }
        return m_game_view.handle_event(event);
    }

    void render(gfx::Renderer& renderer) const override {
        m_game_view.render_game(m_galaxy, viewport(), renderer, *m_font);
        Scene::render(renderer);
    }

private:
    void setup_user_interface() {
        // clang-format off
        m_user_interface.set_layout(
            std::make_unique<ui::GridLayout>(
                16,
                12,
                ui::GridLayout::Area{ { 14, 11 }, { 2, 1 } }
            )
        );
        // clang-format on
        m_user_interface.add_widgets(std::make_unique<ui::Button>("Exit", m_font, [&](auto const&) {
            m_running = false;
        }));
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
        for (auto i = usize{ 0 }; i < galaxy.game_settings().num_planets; ++i) {
            auto const position = utils::Vec2f{
                random.next_float() * galaxy.game_settings().map_size.x,
                random.next_float() * galaxy.game_settings().map_size.y,
            };
            auto game_object = GameObject{};
            game_object.emplace_component<Transform>(position);
            auto const color = utils::Color::random(random);
            auto const planet_name = planet_names.at(random.next_integral<usize>(planet_names.size()));
            game_object.emplace_component<Planet>(planet_name, 10, color);
            galaxy.game_objects().push_back(game_object);
        }
        return galaxy;
    }
};
