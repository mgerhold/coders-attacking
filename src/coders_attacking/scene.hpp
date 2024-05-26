#pragma once

#include "scene_manager.hpp"
#include <common/service_provider.hpp>
#include <gfx/renderer.hpp>
#include <gfx/window.hpp>
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
          m_focus_manager{ service_provider, *m_user_interface } { }

    Scene(Scene const& other) = delete;
    Scene(Scene&& other) noexcept = default;
    Scene& operator=(Scene const& other) = delete;
    Scene& operator=(Scene&& other) noexcept = default;
    virtual ~Scene() = default;

    enum class UpdateResult {
        KeepUpdating,
        StopUpdating,
    };

    [[nodiscard]] virtual UpdateResult update() {
        return UpdateResult::KeepUpdating;
    };

    [[nodiscard]] virtual ui::HandleEventResult handle_event(ui::Event const& event) {
        std::ignore = m_focus_manager.handle_event(event);
        std::ignore = m_user_interface->handle_event(event);
        return ui::HandleEventResult::EventHandled;
    }

    virtual void render(gfx::Renderer& renderer) const {
        m_user_interface->render(renderer);
    }

    virtual void on_window_resized() {
        m_user_interface->recalculate_absolute_area(m_service_provider->window().area());
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
