#include "test_scene.hpp"

#include "common/resource_manager.hpp"
#include "fleet_size_selection.hpp"
#include "gfx/window.hpp"
#include <algorithm>
#include <array>
#include <lib2k/file_utils.hpp>
#include <ui/grid_layout.hpp>
#include <ui/input_field.hpp>

using namespace ui;
using namespace utils;
using namespace c2k;

struct SaveGame final {
    Random::Seed seed;
    Galaxy galaxy;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SaveGame, seed, galaxy);

TestScene::TestScene(ServiceProvider& service_provider)
    : Scene{ service_provider, create_user_interface(service_provider) },
      m_seed{ Random{}.next_integral<Random::Seed>() },
      m_galaxy{ create_galaxy(m_seed) },
      m_game_view{ service_provider, m_seed } {
    find_widget<Button>("button_save").on_click([&](Button&) { on_save_clicked(); });
    find_widget<Button>("button_load").on_click([&](Button&) { on_load_clicked(); });
    find_widget<Button>("button_regenerate").on_click([&](Button&) { on_regenerate_clicked(); });
    m_coordinates_label = &find_widget<Label>("label_coordinates");
    m_zoom_label = &find_widget<Label>("label_zoom");
    find_widget<Button>("button_exit").on_click([&](Button&) { m_running = false; });
    m_focused_planet_label = &find_widget<Label>("label_focused_planet");
}

Scene::UpdateResult TestScene::update() {
    if (not m_running) {
        end_scene();
    }
    m_game_view.update(m_galaxy);
    update_focused_planet_label();
    m_coordinates_label->caption(
            std::format("Offset: ({:.2f},{:.2f})", m_game_view.camera().offset().x, m_game_view.camera().offset().y)
    );
    m_zoom_label->caption(std::format("Zoom: {:.2f}", m_game_view.camera().zoom()));
    if (m_current_command.has_value()) {
        service_provider().scene_manager().enqueue(std::make_unique<FleetSizeSelection>(service_provider()));
        m_current_command.reset();
    }
    if (auto const result = service_provider().scene_manager().pop_value<FleetSizeSelectionResult>()) {
        if (auto const fleet_size = std::get_if<FleetSize>(&result.value())) {
            spdlog::info("selected fleet size: {}", std::to_underlying(*fleet_size));
        } else {
            spdlog::info("fleet size selection canceled");
        }
    }
    return UpdateResult::KeepUpdating;
}

HandleEventResult TestScene::handle_event(Event const& event) {
    std::ignore = Scene::handle_event(event);
    std::ignore = m_game_view.handle_event(event);
    if (auto const selection = m_game_view.pop_selection()) {
        auto const& [start, end] = selection.value();
        auto const start_name = m_galaxy.find_game_object(start)->get_component<Planet>()->name;
        auto const end_name = m_galaxy.find_game_object(end)->get_component<Planet>()->name;
        spdlog::info("ordered to move from {} to {}", start_name, end_name);
        m_current_command = std::tuple{ start, end };
    }
    return HandleEventResult::EventHandled;
}

void TestScene::render(gfx::Renderer& renderer) const {
    m_game_view.render_game(m_galaxy, renderer);
    Scene::render(renderer);
}

void TestScene::on_window_resized() {
    Scene::on_window_resized();
    m_game_view.on_window_resized(game_viewport());
}

void TestScene::on_save_clicked() const {
    auto const save_game = SaveGame{ m_seed, m_galaxy };
    // cannot use initializer list for the json constructor, since that would create an array
    auto const json = nlohmann::json(save_game);
    if (not write_file(savegame_filename, json.dump(4)).has_value()) {
        throw std::runtime_error{ "failed to write savegame" };
    }
}

void TestScene::on_load_clicked() {
    auto save_game = nlohmann::json::parse(read_file(savegame_filename).value()).get<SaveGame>();
    m_seed = save_game.seed;
    m_galaxy = std::move(save_game).galaxy;
    m_game_view.regenerate_background_stars(m_seed);
}

void TestScene::on_regenerate_clicked() {
    m_seed = Random{}.next_integral<Random::Seed>();
    m_galaxy = create_galaxy(m_seed);
    m_game_view.regenerate_background_stars(m_seed);
}

[[nodiscard]] IntRect TestScene::game_viewport() const {
    auto const size = service_provider().window().area().size;
    return FloatRect{
        Vec2f{          size.x / 16.f,                   0.0f },
        Vec2f{ size.x * 15.0f / 16.0f, size.y * 11.0f / 12.0f },
    }
            .round();
}

[[nodiscard]] std::unique_ptr<Widget> TestScene::create_user_interface(ServiceProvider& service_provider) {
    // clang-format off
        auto layout = std::make_unique<GridLayout>(
                16,
                12,
                GridLayout::Area{{0,0},{1,12}},          // left panel
                GridLayout::Area{{1,11},{15,1}},         // bottom panel
                GridLayout::Area{{0,0},{1,1}},           // save button
                GridLayout::Area{{0,1},{1,1}},           // load button
                GridLayout::Area{{0,2},{1,1}},           // regenerate button
                GridLayout::Area{{0,3},{1,1}},           // coordinates label
                GridLayout::Area{{0,4},{1,1}},           // zoom label
                GridLayout::Area{ { 1, 11 }, { 5, 1 } }, // focused planet label
                GridLayout::Area{ { 6, 11 }, { 3, 1 } }, // test input field
                GridLayout::Area{ { 14, 11 }, { 2, 1 } } // exit button
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
            std::make_unique<Button>(WidgetName{ "button_regenerate" }, "Regenerate", 3, font),
            std::make_unique<Label>(
                    WidgetName{ "label_coordinates" },
                    "",
                    font,
                    40,
                    Color::White,
                    Alignment::Left,
                    VerticalAlignment::Middle
            ),
            std::make_unique<Label>(
                    WidgetName{ "label_zoom" },
                    "",
                    font,
                    40,
                    Color::White,
                    Alignment::Left,
                    VerticalAlignment::Middle
            ),
            std::make_unique<Label>(
                    WidgetName{ "label_focused_planet" },
                    "",
                    font,
                    40,
                    Color::White,
                    Alignment::Left,
                    VerticalAlignment::Middle
            ),
            std::make_unique<InputField>(WidgetName{ "input_field_test" }, "test", 4, font),
            std::make_unique<Button>(WidgetName{ "button_exit" }, "Exit", 0, font)
    );
    return user_interface;
}

[[nodiscard]] Galaxy TestScene::create_galaxy(Random::Seed const seed) {
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
    auto random = Random{ seed };

    auto player0 = GameObject{ "Player0" };
    player0.add_component<Player>(Player{
            "coder2k",
            { 63, 22, 126 },
    });
    galaxy.game_objects().push_back(player0);

    auto player1 = GameObject{ "Player1" };
    player1.add_component<Player>(Player{
            "r00tifant",
            { 241, 196, 15 },
    });
    galaxy.game_objects().push_back(player1);

    auto const home_planet_0 = random.next_integral(usize{ 0 }, galaxy.game_settings().num_planets);
    auto const home_planet_1 = std::invoke([&]() {
        while (true) {
            auto const number = random.next_integral(usize{ 0 }, galaxy.game_settings().num_planets);
            if (number != home_planet_0) {
                return number;
            }
        }
    });

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
        if (i == home_planet_0) {
            game_object.add_component(Ownership{ player0.uuid() });
        } else if (i == home_planet_1) {
            game_object.add_component(Ownership{ player1.uuid() });
        }
        galaxy.game_objects().push_back(game_object);
    }


    return galaxy;
}

void TestScene::update_focused_planet_label() const {
    if (auto const& planet = m_game_view.focused_planet()) {
        auto caption = m_game_view.focused_planet()->get_component<Planet>()->name;
        if (auto const owner_uuid = planet->get_component<Ownership>()) {
            auto const owner = m_galaxy.find_game_object(owner_uuid->owner).value().get_component<Player>().value();
            caption += std::format(" ({})", owner.name);
        }
        m_focused_planet_label->caption(caption);

        return;
    }

    m_focused_planet_label->caption("");
}
