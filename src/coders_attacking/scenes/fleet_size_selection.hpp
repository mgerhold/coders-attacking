#pragma once

#include "../scene.hpp"

class FleetSizeSelection final : public Scene {
private:
    enum class Result {
        Ok,
        Cancel,
    };

    tl::optional<Result> m_result;

public:
    explicit FleetSizeSelection(ServiceProvider& service_provider);
    [[nodiscard]] UpdateResult update() override;

private:
    [[nodiscard]] static std::unique_ptr<ui::Widget> create_user_interface(ServiceProvider const& service_provider);
};
