#include "scene_stack.hpp"
#include <algorithm>
#include <lib2k/types.hpp>
#include <ranges>

void SceneStack::update(ui::EventSystem const& event_system, float const delta_seconds) {
    auto scenes_to_delete = std::vector<usize>{};
    auto scenes_to_enqueue = std::vector<std::unique_ptr<Scene>>{};
    m_scene_manager_calls_allowed = true;
    for (auto i = usize{ 0 }; i < m_scenes.size(); ++i) {
        auto const index = m_scenes.size() - i - 1;
        m_should_delete_current_scene = false;
        auto const result = m_scenes.at(index)->update(event_system, delta_seconds);
        if (m_should_delete_current_scene) {
            scenes_to_delete.push_back(index);
        }
        if (m_scene_to_enqueue != nullptr) {
            scenes_to_enqueue.push_back(std::move(m_scene_to_enqueue));
        }
        if (result == Scene::UpdateResult::StopUpdating) {
            break;
        }
    }
    m_scene_manager_calls_allowed = false;
    for (auto const i : scenes_to_delete) {
        auto it = m_scenes.begin();
        std::advance(it, i);
        m_scenes.erase(it);
    }
    std::move(scenes_to_enqueue.begin(), scenes_to_enqueue.end(), std::back_inserter(m_scenes));
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

void SceneStack::recalculate_layout(utils::IntRect const area) {
    m_window_area = area;
    for (auto const& scene : m_scenes) {
        scene->on_window_resized(area);
    }
}

void SceneStack::delete_scene() {
    if (not m_scene_manager_calls_allowed) {
        throw std::runtime_error{ "scene manager calls are only allowed within update()" };
    }
    m_should_delete_current_scene = true;
}

void SceneStack::enqueue_scene(std::unique_ptr<Scene> scene) {
    if (not m_scene_manager_calls_allowed) {
        throw std::runtime_error{ "scene manager calls are only allowed within update()" };
    }
    m_scene_to_enqueue = std::move(scene);
}
