#pragma once

#include "scene_manager.hpp"
#include <gfx/renderer.hpp>
#include <ui/event.hpp>
#include <ui/panel.hpp>

class Scene {
private:
    SceneManager* m_scene_manager;

protected:
    ui::Panel m_user_interface;

public:
    explicit Scene(SceneManager& scene_manager) : m_scene_manager{ &scene_manager } { }

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

    [[nodiscard]] virtual ui::HandleEventResult handle_event(ui::Event const& event) {
        return m_user_interface.handle_event(event);
    }

    virtual void render(gfx::Renderer& renderer) const {
        m_user_interface.render(renderer);
    }

    void recalculate_layout(utils::IntRect const area) {
        m_user_interface.recalculate_absolute_area(area);
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
