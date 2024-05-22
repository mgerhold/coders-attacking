#include "test_scene.hpp"

#include "common/resource_manager.hpp"
#include "gfx/window.hpp"

#include <algorithm>
#include <array>
#include <lib2k/file_utils.hpp>
#include <ui/grid_layout.hpp>

using namespace ui;
using namespace utils;
using namespace c2k;

TestScene::TestScene(ServiceProvider& service_provider)
    : Scene{ service_provider, create_user_interface(service_provider) },
      m_galaxy{ create_galaxy() },
      m_game_view{ service_provider } {
    find_widget<Button>("button_save").on_click([&](Button&) { save(); });
    find_widget<Button>("button_load").on_click([&](Button&) { load(); });
    find_widget<Button>("button_exit").on_click([&](Button&) { m_running = false; });
    m_focused_planet_label = &find_widget<Label>("label_focused_planet");
}

Scene::UpdateResult TestScene::update(EventSystem const& event_system, float const delta_seconds) {
    if (not m_running) {
        end_scene();
    }
    m_game_view.update(m_galaxy, event_system, delta_seconds);
    m_focused_planet_label->caption(
            m_game_view.focused_planet().has_value() ? m_game_view.focused_planet()->get_component<Planet>()->name : ""
    );
    return UpdateResult::KeepUpdating;
}

HandleEventResult TestScene::handle_event(Event const& event, EventSystem const& event_system) {
    if (Scene::handle_event(event, event_system) == HandleEventResult::EventHandled) {
        return HandleEventResult::EventHandled;
    }
    return m_game_view.handle_event(event, event_system);
}

void TestScene::render(gfx::Renderer& renderer) const {
    m_game_view.render_game(m_galaxy, renderer);
    Scene::render(renderer);
}

void TestScene::on_window_resized(IntRect const area) {
    Scene::on_window_resized(area);
    m_game_view.on_window_resized(game_viewport());
}

void TestScene::save() const {
    // cannot use initializer list for the json constructor, since that would create an array
    auto const json = nlohmann::json(m_galaxy);
    if (not write_file(savegame_filename, json.dump(4)).has_value()) {
        throw std::runtime_error{ "failed to write savegame" };
    }
}

void TestScene::load() {
    m_galaxy = nlohmann::json::parse(read_file(savegame_filename).value()).get<Galaxy>();
}

[[nodiscard]] IntRect TestScene::game_viewport() const {
    auto const size = service_provider().window().area().size;
    return IntRect{
        Vec2i{      size.x / 16,                0 },
        Vec2i{ size.x * 15 / 16, size.y * 11 / 12 },
    };
}

[[nodiscard]] std::unique_ptr<Widget> TestScene::create_user_interface(ServiceProvider& service_provider) {
    // clang-format off
        auto layout = std::make_unique<GridLayout>(
                16,
                12,
                GridLayout::Area{{0,0},{1,12}},
                GridLayout::Area{{1,11},{15,1}},
                GridLayout::Area{{0,0},{1,1}},
                GridLayout::Area{{0,1},{1,1}},
                GridLayout::Area{ { 14, 11 }, { 2, 1 } },
                GridLayout::Area{ { 1, 11 }, { 5, 1 } }
            );
    // clang-format on
    auto const& font = service_provider.resource_manager().font(FontType::Roboto);
    auto user_interface = std::make_unique<Panel>(std::move(layout));
    static constexpr auto frame_color = Color{ 54, 59, 79 };
    user_interface->add_widgets(
            std::make_unique<Panel>(frame_color),
            std::make_unique<Panel>(frame_color),
            std::make_unique<Button>(WidgetName{ "button_save" }, "Save", 1, font),
            std::make_unique<Button>(WidgetName{ "button_load" }, "Load", 2, font),
            std::make_unique<Button>(WidgetName{ "button_exit" }, "Exit", 0, font),
            std::make_unique<Label>(
                    WidgetName{ "label_focused_planet" },
                    "",
                    font,
                    40,
                    Color::White,
                    Alignment::Left,
                    VerticalAlignment::Middle
            )
    );
    return user_interface;
}

[[nodiscard]] Galaxy TestScene::create_galaxy() {
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
    auto random = Random{};
    auto shuffled_planet_names = std::vector<std::string>{ planet_names.begin(), planet_names.end() };
    std::shuffle(shuffled_planet_names.begin(), shuffled_planet_names.end(), std::mt19937{ std::random_device{}() });
    for (auto i = usize{ 0 }; i < galaxy.game_settings().num_planets; ++i) {
        auto const position = Vec2f{
            random.next_float() - 0.5f,
            random.next_float() - 0.5f,
        };
        auto game_object = GameObject{};
        game_object.emplace_component<Transform>(position);
        auto const color = Color::random(random);
        assert(i < shuffled_planet_names.size());
        auto planet_name = shuffled_planet_names.at(i);
        game_object.emplace_component<Planet>(std::move(planet_name), 10, color);
        galaxy.game_objects().push_back(game_object);
    }
    return galaxy;
}
