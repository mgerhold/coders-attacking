#pragma once

#include "scene_manager.hpp"
#include <common/service_provider.hpp>
#include <gfx/renderer.hpp>
#include <ui/event.hpp>
#include <ui/focus_manager.hpp>
#include <ui/panel.hpp>

class Scene {
private:
    ServiceProvider* m_service_provider;

protected:
    std::unique_ptr<ui::Widget> m_user_interface;
    ui::FocusManager m_focus_manager;

public:
    explicit Scene(ServiceProvider& service_provider, std::unique_ptr<ui::Widget> user_interface)
        : m_service_provider{ &service_provider },
          m_user_interface{ std::move(user_interface) },
          m_focus_manager{ *m_user_interface } { }

    Scene(Scene const& other) = delete;
    Scene(Scene&& other) noexcept = default;
    Scene& operator=(Scene const& other) = delete;
    Scene& operator=(Scene&& other) noexcept = default;
    virtual ~Scene() = default;

    enum class UpdateResult {
        KeepUpdating,
        StopUpdating,
    };

    [[nodiscard]] virtual UpdateResult update(ui::EventSystem const& event_system, float delta_seconds) = 0;

    // clang-format off
    [[nodiscard]] virtual ui::HandleEventResult handle_event(
        ui::Event const& event,
        ui::EventSystem const& event_system
    ) { // clang-format on
        auto const focus_manager_result = m_focus_manager.handle_event(event, event_system);
        auto const user_interface_result = m_user_interface->handle_event(event);
        return (focus_manager_result == ui::HandleEventResult::EventHandled
                or user_interface_result == ui::HandleEventResult::EventHandled)
                       ? ui::HandleEventResult::EventHandled
                       : ui::HandleEventResult::EventNotHandled;
    }

    virtual void render(gfx::Renderer& renderer) const {
        m_user_interface->render(renderer);
    }

    virtual void on_window_resized(utils::IntRect const area) {
        m_user_interface->recalculate_absolute_area(area);
    }

protected:
    [[nodiscard]] ServiceProvider& service_provider() {
        return *m_service_provider;
    }

    [[nodiscard]] ServiceProvider const& service_provider() const {
        return *m_service_provider;
    }

    void end_scene() {
        service_provider().scene_manager().end_scene();
    }

    template<std::derived_from<Scene> T, typename... Args>
    void push_scene(Args&&... args) {
        service_provider().scene_manager().enqueue(std::make_unique<T>(service_provider(), std::forward<Args>(args)...)
        );
    }
};
