#pragma once

#include "../scene.hpp"
#include <game/galaxy.hpp>
#include <ui/button.hpp>
#include <ui/panel.hpp>
#include <view/view.hpp>

class TestScene final : public Scene {
private:
    c2k::Random::Seed m_seed;
    Galaxy m_galaxy;
    view::View m_game_view;
    bool m_running{ true };
    ui::Label* m_focused_planet_label;
    ui::Label* m_coordinates_label;
    ui::Label* m_zoom_label;
    tl::optional<std::tuple<uuids::uuid, uuids::uuid>> m_current_command;

    static constexpr auto savegame_filename = "my_savegame.json";

public:
    explicit TestScene(ServiceProvider& service_provider);

    // prohibit (copy and) move to avoid problems when passing pointers to members around
    TestScene(TestScene const& other) = delete;
    TestScene(TestScene&& other) noexcept = delete;
    TestScene& operator=(TestScene const& other) = delete;
    TestScene& operator=(TestScene&& other) noexcept = delete;
    ~TestScene() override = default;

    [[nodiscard]] UpdateResult update() override;
    [[nodiscard]] ui::HandleEventResult handle_event(ui::Event const& event) override;
    void render(gfx::Renderer& renderer) const override;
    void on_window_resized() override;

private:
    template<typename T>
    [[nodiscard]] T& find_widget(std::string_view const name) {
        return dynamic_cast<T&>(m_user_interface->find_widget_by_name(name).value());
    }

    void on_save_clicked() const;
    void on_load_clicked();
    void on_regenerate_clicked();

    [[nodiscard]] utils::IntRect game_viewport() const;
    [[nodiscard]] static std::unique_ptr<ui::Widget> create_user_interface(ServiceProvider& service_provider);
    [[nodiscard]] static Galaxy create_galaxy(c2k::Random::Seed seed);
    void update_focused_planet_label() const;
};
