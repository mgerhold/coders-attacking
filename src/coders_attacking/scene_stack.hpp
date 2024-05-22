#pragma once

#include "gfx/window.hpp"
#include "scene.hpp"
#include "scene_manager.hpp"
#include "scene_manager_proxy.hpp"
#include <common/service_provider.hpp>
#include <memory>
#include <vector>

class SceneStack final {
private:
    ServiceProvider* m_service_provider;
    std::vector<std::unique_ptr<Scene>> m_scenes;
    SceneManagerProxy m_scene_manager_proxy;

public:
    explicit SceneStack(ServiceProvider& service_provider) : m_service_provider{ &service_provider } { }

    template<std::derived_from<Scene> T, typename... Args>
    void emplace(Args&&... args) {
        m_scenes.emplace_back(std::make_unique<T>(*m_service_provider, std::forward<Args>(args)...));
        m_scenes.back()->on_window_resized(m_service_provider->window().area());
    }

    void update(ui::EventSystem const& event_system, float delta_seconds);
    void handle_event(ui::Event const& event, ui::EventSystem const& event_system);
    void render(gfx::Renderer& renderer) const;
    void recalculate_layout();

    [[nodiscard]] usize size() const {
        return m_scenes.size();
    }

    [[nodiscard]] bool is_empty() const {
        return size() == 0;
    }

    [[nodiscard]] SceneManager& scene_manager() {
        return m_scene_manager_proxy;
    }
};
