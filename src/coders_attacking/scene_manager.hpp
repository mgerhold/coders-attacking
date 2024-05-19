#pragma once

#include <memory>

class Scene;

class SceneManager {
public:
    SceneManager() = default;
    SceneManager(SceneManager const& other) = default;
    SceneManager(SceneManager&& other) noexcept = default;
    SceneManager& operator=(SceneManager const& other) = default;
    SceneManager& operator=(SceneManager&& other) noexcept = default;
    virtual ~SceneManager() = default;

    virtual void delete_scene() = 0;
    virtual void enqueue_scene(std::unique_ptr<Scene> scene) = 0;
    [[nodiscard]] virtual utils::IntRect viewport() const = 0;
};
