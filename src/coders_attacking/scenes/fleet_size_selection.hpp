#pragma once

#include "../scene.hpp"
#include "ui/button.hpp"

#include <ui/input_field.hpp>
#include <variant>

enum class FleetSize : usize {};

using FleetSizeSelectionResult = std::variant<FleetSize, std::monostate>;

class FleetSizeSelection final : public Scene {
private:
    enum class DialogResult {
        Ok,
        Cancel,
    };

    tl::optional<DialogResult> m_result;
    ui::Button* m_ok_button;
    ui::InputField const* m_input_field;
    usize m_max_fleet_size;

public:
    explicit FleetSizeSelection(ServiceProvider& service_provider, usize max_fleet_size);
    [[nodiscard]] UpdateResult update() override;

private:
    // clang-format off
    [[nodiscard]] static std::unique_ptr<ui::Widget> create_user_interface(
        ServiceProvider const& service_provider,
        usize max_fleet_size
    );
    // clang-format on
};
