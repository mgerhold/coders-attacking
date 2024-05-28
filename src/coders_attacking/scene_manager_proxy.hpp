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
    std::vector<std::any> m_value_stack;

    [[nodiscard]] bool reset_should_delete_current_scene() {
        return std::exchange(m_should_delete_current_scene, false);
    }

    [[nodiscard]] std::vector<std::unique_ptr<Scene>> take_scenes_to_push() {
        return std::move(m_scenes_to_push);
    }

    [[nodiscard]] bool is_value_stack_empty() const;
    [[nodiscard]] tl::optional<std::any> pop_value() override;

public:
    void end_scene() override;
    void enqueue(std::unique_ptr<Scene> scene) override;
    void push_value(std::any value) override;
};
