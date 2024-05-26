#include "fleet_size_selection.hpp"
#include "ui/button.hpp"
#include "ui/label.hpp"
#include <common/font_type.hpp>
#include <common/resource_manager.hpp>
#include <ui/grid_layout.hpp>

using namespace ui;

FleetSizeSelection::FleetSizeSelection(ServiceProvider& service_provider)
    : Scene{ service_provider, create_user_interface(service_provider) } {
    dynamic_cast<Button&>(m_user_interface->find_widget_by_name("button_ok").value()).on_click([&](Button const&) {
        m_result = Result::Ok;
    });
    dynamic_cast<Button&>(m_user_interface->find_widget_by_name("button_cancel").value()).on_click([&](Button const&) {
        m_result = Result::Cancel;
    });
}

[[nodiscard]] Scene::UpdateResult FleetSizeSelection::update() {
    if (m_result.has_value()) {
        service_provider().scene_manager().end_scene();
    }
    return UpdateResult::StopUpdating;
}

std::unique_ptr<Widget> FleetSizeSelection::create_user_interface(ServiceProvider const& service_provider) {
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
            std::make_unique<GridLayout>(
                    7,
                    5,
                    GridLayout::Area{
                            { 1, 1 },
                            { 5, 1 }
    },
                    GridLayout::Area{ { 1, 3 }, { 2, 1 } },
                    GridLayout::Area{ { 4, 3 }, { 2, 1 } }
            ),
            utils::Color::LightGray
    );
    sub_panel->add_widgets(
            std::make_unique<Label>("This is a test!", font, 40, utils::Color::Black, Alignment::Center),
            std::make_unique<Button>(WidgetName{ "button_ok" }, "OK", 0, font),
            std::make_unique<Button>(WidgetName{ "button_cancel" }, "Cancel", 1, font)
    );
    window->add_widgets(std::move(sub_panel));
    return window;
}
