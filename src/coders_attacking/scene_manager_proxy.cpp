#include "scene_manager_proxy.hpp"

[[nodiscard]] bool SceneManagerProxy::is_value_stack_empty() const {
    return m_value_stack.empty();
}

[[nodiscard]] tl::optional<std::any> SceneManagerProxy::pop_value() {
    if (is_value_stack_empty()) {
        return tl::nullopt;
    }
    auto result = std::move(m_value_stack.back());
    m_value_stack.erase(m_value_stack.end() - 1);
    return std::move(result);
}

void SceneManagerProxy::end_scene() {
    if (m_should_delete_current_scene) {
        throw std::logic_error{ "trying to delete current scene twice" };
    }
    m_should_delete_current_scene = true;
}

void SceneManagerProxy::enqueue(std::unique_ptr<Scene> scene) {
    m_scenes_to_push.emplace_back(std::move(scene));
}

void SceneManagerProxy::push_value(std::any value) {
    m_value_stack.emplace_back(std::move(value));
}
