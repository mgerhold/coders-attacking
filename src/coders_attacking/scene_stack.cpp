#include "scene_stack.hpp"
#include <algorithm>
#include <lib2k/types.hpp>
#include <ranges>

void SceneStack::update() {
    if (m_service_provider->window().was_resized()) {
        recalculate_layout();
    }

    if (m_scene_manager_proxy.m_should_delete_current_scene or not m_scene_manager_proxy.m_scenes_to_push.empty()) {
        throw std::runtime_error{ "manipulating the scene stack is only allowed during update" };
    }

    auto scenes_to_delete_indices = std::vector<usize>{};
    for (auto i = usize{ 0 }; i < m_scenes.size(); ++i) {
        auto const index = m_scenes.size() - i - 1;
        auto const result = m_scenes.at(index)->update();
        if (m_scene_manager_proxy.reset_should_delete_current_scene()) {
            assert(not m_scene_manager_proxy.m_should_delete_current_scene and "reset should have cleared the flag");
            scenes_to_delete_indices.push_back(index);
        }
        if (result == Scene::UpdateResult::StopUpdating) {
            break;
        }
    }
    for (auto const i : scenes_to_delete_indices) {
        m_scenes.erase(std::next(m_scenes.begin(), static_cast<decltype(m_scenes)::difference_type>(i)));
    }
    std::ranges::move(m_scene_manager_proxy.take_scenes_to_push(), std::back_inserter(m_scenes));
    for (auto const& scene : m_scenes) {
        scene->on_window_resized();
    }
    assert(m_scene_manager_proxy.m_scenes_to_push.empty() and "all scenes should have been moved out");
}

void SceneStack::handle_event(ui::Event const& event) {
    using std::ranges::views::reverse;
    for (auto const& scene : m_scenes | reverse) {
        auto const result = scene->handle_event(event);
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
        scene->on_window_resized();
    }
}
