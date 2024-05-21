#pragma once

#include "scene_manager.hpp"
#include <gfx/renderer.hpp>
#include <ui/event.hpp>
#include <ui/focus_manager.hpp>
#include <ui/panel.hpp>

class Scene {
private:
    SceneManager* m_scene_manager;

protected:
    std::unique_ptr<ui::Widget> m_user_interface;
    ui::FocusManager m_focus_manager;

public:
    explicit Scene(SceneManager& scene_manager, std::unique_ptr<ui::Widget> user_interface)
        : m_scene_manager{ &scene_manager },
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
        auto const focus_manager_result = m_focus_manager.handle_event(event);
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
    void end_scene() const { // NOLINT (not marked const)
        m_scene_manager->delete_scene();
    }

    template<std::derived_from<Scene> T, typename... Args>
    void push_scene(Args&&... args) const {
        m_scene_manager->enqueue_scene(std::make_unique<T>(*m_scene_manager, std::forward<Args>(args)...));
    }

    [[nodiscard]] utils::IntRect viewport() const {
        return m_scene_manager->viewport();
    }
};
