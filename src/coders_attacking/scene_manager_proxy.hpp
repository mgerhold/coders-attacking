#pragma once

#include "scene.hpp"
#include "scene_manager.hpp"
#include <memory>
#include <vector>

class SceneStack;

class SceneManagerProxy final : public SceneManager {
    friend class SceneStack;

private:
    bool m_should_delete_current_scene{ false };
    std::vector<std::unique_ptr<Scene>> m_scenes_to_push;

public:
    void end_scene() override;
    void enqueue(std::unique_ptr<Scene> scene) override;
};
