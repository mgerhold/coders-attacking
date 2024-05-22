#include "scene_stack.hpp"
#include <algorithm>
#include <lib2k/types.hpp>
#include <ranges>

void SceneStack::update(ui::EventSystem const& event_system, float const delta_seconds) {
    if (m_service_provider->window().was_resized()) {
        recalculate_layout();
    }

    if (m_scene_manager_proxy.m_should_delete_current_scene or not m_scene_manager_proxy.m_scenes_to_push.empty()) {
        throw std::runtime_error{ "manipulating the scene stack is only allowed during update" };
    }

    auto scenes_to_delete = std::vector<usize>{};
    for (auto i = usize{ 0 }; i < m_scenes.size(); ++i) {
        auto const index = m_scenes.size() - i - 1;
        auto const result = m_scenes.at(index)->update(event_system, delta_seconds);
        if (m_scene_manager_proxy.m_should_delete_current_scene) {
            scenes_to_delete.push_back(index);
            m_scene_manager_proxy.m_should_delete_current_scene = false;
        }
        if (result == Scene::UpdateResult::StopUpdating) {
            break;
        }
    }
    for (auto const i : scenes_to_delete) {
        auto it = m_scenes.begin();
        std::advance(it, i);
        m_scenes.erase(it);
    }
    std::move(
            m_scene_manager_proxy.m_scenes_to_push.begin(),
            m_scene_manager_proxy.m_scenes_to_push.end(),
            std::back_inserter(m_scenes)
    );
    assert(m_scene_manager_proxy.m_scenes_to_push.empty());
}

void SceneStack::handle_event(ui::Event const& event, ui::EventSystem const& event_system) {
    using std::ranges::views::reverse;
    for (auto const& scene : m_scenes | reverse) {
        auto const result = scene->handle_event(event, event_system);
        if (result == ui::HandleEventResult::EventHandled) {
            break;
        }
    }
}

void SceneStack::render(gfx::Renderer& renderer) const {
    for (auto const& scene : m_scenes) {
        scene->render(renderer);
    }
}

void SceneStack::recalculate_layout() {
    for (auto const& scene : m_scenes) {
        scene->on_window_resized(m_service_provider->window().area());
    }
}
