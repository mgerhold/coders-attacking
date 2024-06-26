#include "fleet_size_selection.hpp"
#include <common/font_type.hpp>
#include <common/resource_manager.hpp>
#include <ui/button.hpp>
#include <ui/grid_layout.hpp>
#include <ui/input_field.hpp>
#include <ui/label.hpp>
#include <utils/lexical_cast.hpp>

using namespace ui;

FleetSizeSelection::FleetSizeSelection(ServiceProvider& service_provider, usize const max_fleet_size)
    : Scene{ service_provider, create_user_interface(service_provider, max_fleet_size) },
      m_max_fleet_size{ max_fleet_size } {
    m_ok_button = &dynamic_cast<Button&>(m_user_interface->find_widget_by_name("button_ok").value());
    m_ok_button->on_click([&](Button const&) { m_result = DialogResult::Ok; });
    dynamic_cast<Button&>(m_user_interface->find_widget_by_name("button_cancel").value()).on_click([&](Button const&) {
        m_result = DialogResult::Cancel;
    });

    auto& input_field =
            dynamic_cast<InputField&>(m_user_interface->find_widget_by_name("input_field_fleet_size").value());
    ;
    m_input_field = &input_field;
    input_field.on_enter_pressed([&](InputField&) { m_ok_button->click(); });
}

[[nodiscard]] Scene::UpdateResult FleetSizeSelection::update() {
    if (m_result.has_value()) {
        switch (m_result.value()) {
            case DialogResult::Ok: {
                if (auto const amount = utils::lexical_cast<usize>(m_input_field->text().c_str());
                    amount.has_value() and amount.value() >= 1 and amount.value() <= m_max_fleet_size) {
                    service_provider().scene_manager().push_value(FleetSizeSelectionResult{
                            FleetSize{ amount.value() },
                    });
                    service_provider().scene_manager().end_scene();
                } else {
                    m_result.reset();
                }
                break;
            }
            case DialogResult::Cancel:
                service_provider().scene_manager().push_value(FleetSizeSelectionResult{ std::monostate{} });
                service_provider().scene_manager().end_scene();
                break;
        }
    }
    return UpdateResult::StopUpdating;
}

// clang-format off
std::unique_ptr<Widget> FleetSizeSelection::create_user_interface(
    ServiceProvider const& service_provider,
    usize const max_fleet_size
) { // clang-format on
    auto const& font = service_provider.resource_manager().font(FontType::Roboto);
    auto window = std::make_unique<Panel>(
            std::make_unique<GridLayout>(
                    16,
                    12,
                    GridLayout::Area{
                            { 4, 3 },
                            { 8, 6 }
    }
            ),
            utils::Color{ 0, 0, 0, 128 }
    );
    auto sub_panel = std::make_unique<Panel>(
            // clang-format off
            std::make_unique<GridLayout>(
                    7,
                    6,
                    GridLayout::Area{ { 1, 1 }, { 5, 1 } },
                    GridLayout::Area{ { 2, 2 }, { 3, 1 } },
                    GridLayout::Area{ { 1, 4 }, { 2, 1 } },
                    GridLayout::Area{ { 4, 4 }, { 2, 1 } }
            ),
            utils::Color::LightGray
            // clang-format on
    );
    sub_panel->add_widgets(
            std::make_unique<Label>("How many ships to send?", font, 40, utils::Color::Black, Alignment::Center),
            std::make_unique<InputField>(
                    WidgetName{ "input_field_fleet_size" },
                    std::format("fleet size (1 - {})", max_fleet_size),
                    0,
                    font
            ),
            std::make_unique<Button>(WidgetName{ "button_ok" }, "OK", 1, font),
            std::make_unique<Button>(WidgetName{ "button_cancel" }, "Cancel", 2, font)
    );
    window->add_widgets(std::move(sub_panel));
    return window;
}
