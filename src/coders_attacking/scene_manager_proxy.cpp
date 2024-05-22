#include "scene_manager_proxy.hpp"

void SceneManagerProxy::end_scene() {
    if (m_should_delete_current_scene) {
        throw std::logic_error{ "trying to delete current scene twice" };
    }
    m_should_delete_current_scene = true;
}

void SceneManagerProxy::enqueue(std::unique_ptr<Scene> scene) {
    m_scenes_to_push.emplace_back(std::move(scene));
}
