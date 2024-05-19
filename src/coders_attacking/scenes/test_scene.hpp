#pragma once

#include "../scene.hpp"
#include <game/galaxy.hpp>
#include <ui/button.hpp>
#include <ui/grid_layout.hpp>
#include <ui/panel.hpp>
#include <view/view.hpp>

class TestScene final : public Scene {
private:
    Galaxy m_galaxy;
    std::shared_ptr<gfx::Font> m_font;
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

    void render(gfx::Renderer& renderer) const override {
        view::render_game(m_galaxy, viewport(), renderer, *m_font);
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
};
